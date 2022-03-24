/* NewProcess.cpp - BetterThanCaptureBAT

				Records all newly created (or attacked if pre-existing) processes and their childrens' process activity.
				Copies RWX (shellcode) buffer to shellcode file for static (or dyanamic via emulation, or direct injection on bare metal if you want to get jiggy) analysis.
				Also monitors any rogue DLLs which are injected within target processes.

				We even free the monitoredRWX_LL_t objects.  Not that it should be needed.  (see dllmain.cpp)
				*/

#include "stdafx.h"

#ifndef NEWPROCESS_H
#include "NewProcess.h"
#endif

// Extern redefinitions.
BOOL (WINAPI* pCreateProcessW)(LPCWSTR lpApplicationName,
							 LPWSTR lpCommandLine,
							 LPSECURITY_ATTRIBUTES lpProcessAttributes,
							 LPSECURITY_ATTRIBUTES lpThreadAttributes,
							 BOOL bInheritHandles,
							 DWORD dwCreationFlags,
							 LPVOID lpEnvironment,
							 LPCWSTR lpCurrentDirectory,
							 LPSTARTUPINFOW lpStartupInfo,
							 LPPROCESS_INFORMATION lpProcessInformation) = CreateProcess;
HANDLE (WINAPI* pCreateRemoteThread)(HANDLE hProcess,
									 LPSECURITY_ATTRIBUTES lpThreadAttributes,
									 SIZE_T dwStackSize,
									 LPTHREAD_START_ROUTINE lpStartAddress,
									 LPVOID lpParameter,
									 DWORD dwCreationFlags,
									 LPDWORD lpThreadId) = CreateRemoteThread;
LPVOID (WINAPI* pVirtualAllocEx)(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flAllocationType,
								 DWORD flProtect) = VirtualAllocEx;
BOOL (WINAPI* pVirtualProtectEx)(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flNewProtect,
								 PDWORD lpflOldProtect) = VirtualProtectEx;
BOOL (WINAPI* pWriteProcessMemory)(HANDLE hProcess,
								  LPVOID lpBaseAddress,
								  LPCVOID lpBuffer,
								  SIZE_T nSize,
								  SIZE_T* lpNumberOfBytesWritten) = WriteProcessMemory;
bool bFollowProcesses = true;
monitoredRWX_LL_t* pMonitoredRWX_LL_head, * pMonitoredRWX_LL_prev;



BOOL WINAPI hooked_CreateProcessW(LPCWSTR lpApplicationName,
							 LPWSTR lpCommandLine,
							 LPSECURITY_ATTRIBUTES lpProcessAttributes,
							 LPSECURITY_ATTRIBUTES lpThreadAttributes,
							 BOOL bInheritHandles,
							 DWORD dwCreationFlags,
							 LPVOID lpEnvironment,
							 LPCWSTR lpCurrentDirectory,
							 LPSTARTUPINFOW lpStartupInfo,
							 LPPROCESS_INFORMATION lpProcessInformation)
{
	FILE*	logFile;
	BOOL	bCreateProcess = false;

	logFile	= _tfopen(L"capture.txt", L"a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open capture.txt in write mode\n");
		return pCreateProcessW(lpApplicationName,
								lpCommandLine,
								lpProcessAttributes,
								lpThreadAttributes,
								bInheritHandles,
								dwCreationFlags,
								lpEnvironment,
								lpCurrentDirectory,
								lpStartupInfo,
								lpProcessInformation);
	}

	if (bFollowProcesses)
	{
		bCreateProcess = DetourCreateProcessWithDll(lpApplicationName,
													lpCommandLine,
													lpProcessAttributes,
													lpThreadAttributes,
													bInheritHandles,
													dwCreationFlags,
													lpEnvironment,
													lpCurrentDirectory,
													lpStartupInfo,
													lpProcessInformation,
													"C:\\BetterThanCaptureBAT.dll",
													pCreateProcessW);	// pCreateProcessW

		if (!bCreateProcess)
		{
			fprintf(stderr, "Import rebuilding problem following child process(es).  Usually not critical.  Error (%d)\n", GetLastError());

			//return pCreateProcessW(lpApplicationName,
			//						lpCommandLine,
			//						lpProcessAttributes,
			//						lpThreadAttributes,
			//						bInheritHandles,
			//						dwCreationFlags,
			//						lpEnvironment,
			//						lpCurrentDirectory,
			//						lpStartupInfo,
			//						lpProcessInformation);
		}
	}

	_ftprintf(logFile,	L"New Process Created%s:\n"
						L"Application Name:  %s\n"
						L"Command Line:		%s\n"
						L"Creation Flags:	0x%x\n"
						L"Current Directory:	%s\n",	(dwCreationFlags & CREATE_SUSPENDED) ? _T(" Suspended") : _T(""),
													lpApplicationName,
													lpCommandLine,
													dwCreationFlags,
													lpCurrentDirectory);
	fclose(logFile);

	if (bFollowProcesses && bCreateProcess)
		return bCreateProcess;

	return pCreateProcessW(lpApplicationName,
							lpCommandLine,
							lpProcessAttributes,
							lpThreadAttributes,
							bInheritHandles,
							dwCreationFlags,
							lpEnvironment,
							lpCurrentDirectory,
							lpStartupInfo,
							lpProcessInformation);
}

HANDLE WINAPI hooked_CreateRemoteThread(HANDLE hProcess,
									 LPSECURITY_ATTRIBUTES lpThreadAttributes,
									 SIZE_T dwStackSize,
									 LPTHREAD_START_ROUTINE lpStartAddress,
									 LPVOID lpParameter,
									 DWORD dwCreationFlags,
									 LPDWORD lpThreadId)
{
	FILE*	logFile;
	TCHAR	injectedFileName[256];

	logFile	= _tfopen(L"capture.txt", L"a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open capture.txt in write mode\n");
		
		return pCreateRemoteThread(hProcess,
									lpThreadAttributes,
									dwStackSize,
									lpStartAddress,
									lpParameter,
									dwCreationFlags,
									lpThreadId);
	}

	if (!fpGetProcessImageFileName(hProcess, injectedFileName, 256))
	{
		fprintf(stderr, "Unable to find injected process\n");
		fclose(logFile);

		return pCreateRemoteThread(hProcess,
									lpThreadAttributes,
									dwStackSize,
									lpStartAddress,
									lpParameter,
									dwCreationFlags,
									lpThreadId);
	}

	_ftprintf(logFile, _T("Thread injected into process %s\n"), injectedFileName);
	fclose(logFile);

	return pCreateRemoteThread(hProcess,
									lpThreadAttributes,
									dwStackSize,
									lpStartAddress,
									lpParameter,
									dwCreationFlags,
									lpThreadId);
}

LPVOID WINAPI hooked_VirtualAllocEx(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flAllocationType,
								 DWORD flProtect)
{
	FILE*	logFile;
	TCHAR	injectedFileName[256];
	TCHAR	newProtect[256];
	monitoredRWX_LL_t*	tempMonitoredRWX;

	logFile	= _tfopen(L"capture.txt", L"a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open capture.txt in write mode\n");
		
		return pVirtualAllocEx(hProcess,
								lpAddress,
								dwSize,
								flAllocationType,
								flProtect);
	}

	ZeroMemory(injectedFileName, 256);
	ZeroMemory(newProtect, 256);

	// If it's not injecting into itself.
	if ((unsigned int) hProcess != 0xFFFFFFFF)
	{
		if (!fpGetProcessImageFileName(hProcess, injectedFileName, 256))
		{
			fprintf(stderr, "Unable to find injected process (%d)\n", GetLastError());
			fclose(logFile);

			return pVirtualAllocEx(hProcess,
								lpAddress,
								dwSize,
								flAllocationType,
								flProtect);
		}
	}
	
	else
		_stprintf(injectedFileName, _T("%s"), _T("itself"));

	// I wonder if the compiler optimizes.
	if (flProtect == PAGE_EXECUTE_READWRITE)
		_ftprintf(logFile, _T("Self-modifying code area created within %s\n"), injectedFileName);	

	// We have a winner.
	if (flProtect == PAGE_EXECUTE_READWRITE)
	{
		tempMonitoredRWX = (monitoredRWX_LL_t*) calloc(sizeof(monitoredRWX_LL_t), sizeof(char));
		if (!tempMonitoredRWX)
		{
			fprintf(stderr, "Unable to record monitored region\n");
			fclose(logFile);

			return pVirtualAllocEx(hProcess,
								lpAddress,
								dwSize,
								flAllocationType,
								flProtect);
		}


		tempMonitoredRWX->dwSize = dwSize;
		tempMonitoredRWX->hProcess = hProcess;
		// Must check for empty lpAddress, as user may want the OS to determine which address is best.
		if (lpAddress)
		{
			tempMonitoredRWX->lpAddress = lpAddress;
			tempMonitoredRWX->next = NULL;
			if (!pMonitoredRWX_LL_head)
				pMonitoredRWX_LL_head = tempMonitoredRWX;
			if (pMonitoredRWX_LL_prev)
				pMonitoredRWX_LL_prev->next = tempMonitoredRWX;
			pMonitoredRWX_LL_prev = tempMonitoredRWX;

			fclose(logFile);

			return pVirtualAllocEx(hProcess,
								lpAddress,
								dwSize,
								flAllocationType,
								flProtect);
		}

		else
		{
			lpAddress = pVirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
			if (!lpAddress)
			{
				fprintf(stderr, "There was an error obtaining an address from VirtualAllocEx()");
				fclose(logFile);
				free(tempMonitoredRWX);

				return NULL;
			}

			tempMonitoredRWX->lpAddress = lpAddress;
		}
		tempMonitoredRWX->next = NULL;

		if (!pMonitoredRWX_LL_head)
			pMonitoredRWX_LL_head = tempMonitoredRWX;
		if (pMonitoredRWX_LL_prev)
			pMonitoredRWX_LL_prev->next = tempMonitoredRWX;
		pMonitoredRWX_LL_prev = tempMonitoredRWX;
	}

	fclose(logFile);

	return lpAddress;
}

BOOL WINAPI hooked_VirtualProtectEx(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flNewProtect,
								 PDWORD lpflOldProtect)
{
	FILE*	logFile;
	TCHAR	injectedFileName[256];
	TCHAR	newProtect[256];
	monitoredRWX_LL_t*	tempMonitoredRWX;

	logFile	= _tfopen(L"capture.txt", L"a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open capture.txt in write mode\n");
		
		return pVirtualProtectEx(hProcess,
									lpAddress,
									dwSize,
									flNewProtect,
									lpflOldProtect);
	}

	ZeroMemory(injectedFileName, 256);
	ZeroMemory(newProtect, 256);

	if ((unsigned int) hProcess != 0xFFFFFFFF)
	{
		if (!fpGetProcessImageFileName(hProcess, injectedFileName, 256))
		{
			fprintf(stderr, "Unable to find injected process\n");
			fclose(logFile);

			return pVirtualProtectEx(hProcess,
									lpAddress,
									dwSize,
									flNewProtect,
									lpflOldProtect);
		}
	}
	
	else
		_stprintf(injectedFileName, _T("%s"), _T("itself"));

	

	_stprintf(newProtect, _T("%s"), injectedFileName);
	if (flNewProtect == PAGE_EXECUTE_READWRITE)
		_ftprintf(logFile, _T("Self-modifying code area created within %s\n"), injectedFileName);

	if (flNewProtect == PAGE_EXECUTE_READWRITE && lpAddress)
	{
		tempMonitoredRWX = (monitoredRWX_LL_t*) calloc(sizeof(monitoredRWX_LL_t), sizeof(char));
		if (!tempMonitoredRWX)
		{
			fprintf(stderr, "Unable to record monitored region\n");
			fclose(logFile);

			return pVirtualProtectEx(hProcess,
										lpAddress,
										dwSize,
										flNewProtect,
										lpflOldProtect);
		}


		tempMonitoredRWX->dwSize = dwSize;
		tempMonitoredRWX->hProcess = hProcess;
		tempMonitoredRWX->lpAddress = lpAddress;
		tempMonitoredRWX->next = NULL;

		if (!pMonitoredRWX_LL_head)
			pMonitoredRWX_LL_head = tempMonitoredRWX;
		if (pMonitoredRWX_LL_prev)
			pMonitoredRWX_LL_prev->next = tempMonitoredRWX;
		pMonitoredRWX_LL_prev = tempMonitoredRWX;
	}

	fclose(logFile);

	return pVirtualProtectEx(hProcess,
									lpAddress,
									dwSize,
									flNewProtect,
									lpflOldProtect);
}

BOOL WINAPI hooked_WriteProcessMemory(HANDLE hProcess,
								  LPVOID lpBaseAddress,
								  LPCVOID lpBuffer,
								  SIZE_T nSize,
								  SIZE_T* lpNumberOfBytesWritten)
{
	FILE*				logFile;
	FILE*				fShellcodeOutput;
	TCHAR				shellcodeFileName[256];
	monitoredRWX_LL_t*	tempMonitoredRWX;

	// If we haven't found any RWX sections.
	if (!pMonitoredRWX_LL_head)
		return pWriteProcessMemory(hProcess,
									lpBaseAddress,
									lpBuffer,
									nSize,
									lpNumberOfBytesWritten);
	tempMonitoredRWX = pMonitoredRWX_LL_head;

	// Efficiency:  Linear Search (Worst case:  O(N))
	do
	{
		if (hProcess == tempMonitoredRWX->hProcess)
			if (lpBaseAddress == tempMonitoredRWX->lpAddress)
			{
				// Do not filter based on buffer size.  They may write less than requested.
				ZeroMemory(shellcodeFileName, 256);

				// If there are more than 256 shellcode stubs, someone is greedy.
				for (int i = 0; i < 256; i++)
				{
					// Check to see if file exists.
					_stprintf(shellcodeFileName, _T("shellcode.out.%d"), i);
					fShellcodeOutput = _tfopen(shellcodeFileName, _T("rb"));
					if (!fShellcodeOutput)
						break;
					fclose(fShellcodeOutput);
				}

				// shellcodeFileName now contains the name we can use to write as a fresh new file.
				fShellcodeOutput = _tfopen(shellcodeFileName, _T("wb"));
				if (!fShellcodeOutput)
				{
					fprintf(stderr, "Not allowed to create shellcode file.  Are you Administrator?");

					return pWriteProcessMemory(hProcess,
									lpBaseAddress,
									lpBuffer,
									nSize,
									lpNumberOfBytesWritten);
				}

				logFile	= _tfopen(L"capture.txt", L"a");
				if (!logFile)
				{
					fprintf(stderr, "Unable to open capture.txt in write mode\n");
		
					return pWriteProcessMemory(hProcess,
									lpBaseAddress,
									lpBuffer,
									nSize,
									lpNumberOfBytesWritten);
				}

				// WriteProcessMemory will add a NULL.
				for (int i = 0; i < nSize - 1; i++)
					fputc(*((char* )lpBuffer + i), fShellcodeOutput);
				_ftprintf(logFile, _T("Copied shellcode to %s\n"), shellcodeFileName);

				fclose(fShellcodeOutput);
				fclose(logFile);

				// No need to coalesce list.
				// The buffer may be written to other areas later in execution.
				break;
			}

		tempMonitoredRWX = tempMonitoredRWX->next;
	} while (tempMonitoredRWX);
	
	return pWriteProcessMemory(hProcess,
									lpBaseAddress,
									lpBuffer,
									nSize,
									lpNumberOfBytesWritten);
}