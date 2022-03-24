 // dllmain.cpp : Defines the entry point for the DLL application.
/*	dllmain.cpp - BetterThanCaptureBAT
				by Trey Lykes
				April 2013
				PENTCIRT

				EntryPoint of DLL.  This is the center of functionality.
				*/
#include "stdafx.h"

#ifndef MODIFIEDFILE_H
#include "ModifiedFile.h"
#endif

#ifndef NETWORK_H
#include "Network.h"
#endif

#ifndef NEWPROCESS_H
#include "NewProcess.h"
#endif

#ifndef REGISTRY_H
#include "Registry.h"
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	int error = 0;
	monitoredRWX_LL_t*	pTemp1MonitoredRWX_LL;
	monitoredRWX_LL_t*	pTemp2MonitoredRWX_LL;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// ModifiedFile
		while (!IsDebuggerPresent())
			;
		DetourTransactionBegin();
		DetourAttach(&(PVOID& ) pCreateFileW, hooked_CreateFileW);
		DetourAttach(&(PVOID& ) pDeleteFileW, hooked_DeleteFileW);
		DetourUpdateThread(GetCurrentThread());
		error = DetourTransactionCommit();

		if (error)
		{
			// Print error
			_ftprintf(stderr, _T("Unable to hook File Modification library\n"));
		}

		// Network
		DetourTransactionBegin();
		DetourAttach(&(PVOID& ) pSend, hooked_send);
		DetourAttach(&(PVOID& ) pRecv, hooked_recv);
		DetourAttach(&(PVOID& ) pConnect, hooked_connect);
		DetourAttach(&(PVOID& ) pBind, hooked_bind);
		DetourAttach(&(PVOID& ) pWSASend, hooked_WSASend);
		DetourAttach(&(PVOID& ) pWSARecv, hooked_WSARecv);
		DetourAttach(&(PVOID& ) pWSAConnect, hooked_WSAConnect);
		DetourAttach(&(PVOID& ) pInternetOpenA, hooked_InternetOpenA);
		DetourAttach(&(PVOID& ) pInternetOpenW, hooked_InternetOpenW);
		DetourAttach(&(PVOID& ) pInternetConnectA, hooked_InternetConnectA);
		DetourAttach(&(PVOID& ) pInternetConnectW, hooked_InternetConnectW);
		DetourAttach(&(PVOID& ) pInternetOpenUrlA, hooked_InternetOpenUrlA);
		DetourAttach(&(PVOID& ) pInternetOpenUrlW, hooked_InternetOpenUrlW);
		DetourAttach(&(PVOID& ) pHttpOpenRequestA, hooked_HttpOpenRequestA);
		DetourAttach(&(PVOID& ) pHttpOpenRequestW, hooked_HttpOpenRequestW);
		DetourAttach(&(PVOID& ) pHttpAddRequestHeadersA, hooked_HttpAddRequestHeadersA);
		DetourAttach(&(PVOID& ) pHttpAddRequestHeadersW, hooked_HttpAddRequestHeadersW);
		DetourAttach(&(PVOID& ) pHttpQueryInfoA, hooked_HttpQueryInfoA);
		DetourAttach(&(PVOID& ) pHttpQueryInfoW, hooked_HttpQueryInfoW);
		DetourAttach(&(PVOID& ) pHttpSendRequestA, hooked_HttpSendRequestA);
		DetourAttach(&(PVOID& ) pHttpSendRequestW, hooked_HttpSendRequestW);
		DetourAttach(&(PVOID& ) pHttpSendRequestExA, hooked_HttpSendRequestA);
		DetourAttach(&(PVOID& ) pHttpSendRequestExW, hooked_HttpSendRequestW);
		DetourAttach(&(PVOID& ) pInternetReadFile, hooked_InternetReadFile);
		DetourAttach(&(PVOID& ) pInternetReadFileExA, hooked_InternetReadFileExA);
		DetourAttach(&(PVOID& ) pInternetReadFileExW, hooked_InternetReadFileExW);
		DetourUpdateThread(GetCurrentThread());
		error = DetourTransactionCommit();
		
		if (error)
		{
			// Print error
			_ftprintf(stderr, _T("Unable to hook Network library\n"));
		}

		// NewProcess
		DetourTransactionBegin();
		DetourAttach(&(PVOID& ) pCreateProcessW, hooked_CreateProcessW);
		DetourAttach(&(PVOID& ) pCreateRemoteThread, hooked_CreateRemoteThread);
		DetourAttach(&(PVOID& ) pVirtualAllocEx, hooked_VirtualAllocEx);
		DetourAttach(&(PVOID& ) pVirtualProtectEx, hooked_VirtualProtectEx);
		DetourAttach(&(PVOID& ) pWriteProcessMemory, hooked_WriteProcessMemory);
		DetourUpdateThread(GetCurrentThread());
		error = DetourTransactionCommit();

		if (error)
		{
			// Print error
			_ftprintf(stderr, _T("Unable to hook Process Creation library\n"));
		}

		// Registry
		DetourTransactionBegin();
		DetourAttach(&(PVOID& ) pRegOpenKeyExA, hooked_RegOpenKeyExA);
		DetourAttach(&(PVOID& ) pRegOpenKeyExW, hooked_RegOpenKeyExW);
		DetourAttach(&(PVOID& ) pRegCreateKeyExA, hooked_RegCreateKeyExA);
		DetourAttach(&(PVOID& ) pRegCreateKeyExW, hooked_RegCreateKeyExW);
		DetourAttach(&(PVOID& ) pRegDeleteKeyExA, hooked_RegDeleteKeyExA);
		DetourAttach(&(PVOID& ) pRegDeleteKeyExW, hooked_RegDeleteKeyExW);
		DetourAttach(&(PVOID& ) pRegQueryValueExA, hooked_RegQueryValueExA);
		DetourAttach(&(PVOID& ) pRegQueryValueExW, hooked_RegQueryValueExW);
		DetourAttach(&(PVOID& ) pRegEnumKeyExA, hooked_RegEnumKeyExA);
		DetourAttach(&(PVOID& ) pRegEnumKeyExA, hooked_RegEnumKeyExA);
		DetourAttach(&(PVOID& ) pRegGetValueA, hooked_RegGetValueA);
		DetourAttach(&(PVOID& ) pRegGetValueW, hooked_RegGetValueW);
		DetourAttach(&(PVOID& ) pRegSetValueExA, hooked_RegSetValueExA);
		DetourAttach(&(PVOID& ) pRegSetValueExW, hooked_RegSetValueExW);
		DetourUpdateThread(GetCurrentThread());
		error = DetourTransactionCommit();
		
		if (error)
		{
			// Print error
			_ftprintf(stderr, _T("Unable to hook Registry library\n"));
		}

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		// ModifiedFile
		DetourTransactionBegin();
		DetourDetach(&(PVOID& ) pCreateFileW, hooked_CreateFileW);
		DetourDetach(&(PVOID& ) pDeleteFileW, hooked_DeleteFileW);
		DetourUpdateThread(GetCurrentThread());
		error = DetourTransactionCommit();
		
		if (error)
		{
			// Print error
			_ftprintf(stderr, _T("Unable to unhook File Modification library\n"));
		}

		// Network
		DetourTransactionBegin();
		DetourDetach(&(PVOID& ) pSend, hooked_send);
		DetourDetach(&(PVOID& ) pRecv, hooked_recv);
		DetourDetach(&(PVOID& ) pConnect, hooked_connect);
		DetourDetach(&(PVOID& ) pBind, hooked_bind);
		DetourDetach(&(PVOID& ) pWSASend, hooked_WSASend);
		DetourDetach(&(PVOID& ) pWSARecv, hooked_WSARecv);
		DetourDetach(&(PVOID& ) pWSAConnect, hooked_WSAConnect);
		DetourDetach(&(PVOID& ) pInternetOpenA, hooked_InternetOpenA);
		DetourDetach(&(PVOID& ) pInternetOpenW, hooked_InternetOpenW);
		DetourDetach(&(PVOID& ) pInternetConnectA, hooked_InternetConnectA);
		DetourDetach(&(PVOID& ) pInternetConnectW, hooked_InternetConnectW);
		DetourDetach(&(PVOID& ) pInternetOpenUrlA, hooked_InternetOpenUrlA);
		DetourDetach(&(PVOID& ) pInternetOpenUrlW, hooked_InternetOpenUrlW);
		DetourDetach(&(PVOID& ) pHttpOpenRequestA, hooked_HttpOpenRequestA);
		DetourDetach(&(PVOID& ) pHttpOpenRequestW, hooked_HttpOpenRequestW);
		DetourDetach(&(PVOID& ) pHttpAddRequestHeadersA, hooked_HttpAddRequestHeadersA);
		DetourDetach(&(PVOID& ) pHttpAddRequestHeadersW, hooked_HttpAddRequestHeadersW);
		DetourDetach(&(PVOID& ) pHttpQueryInfoA, hooked_HttpQueryInfoA);
		DetourDetach(&(PVOID& ) pHttpQueryInfoW, hooked_HttpQueryInfoW);
		DetourDetach(&(PVOID& ) pHttpSendRequestA, hooked_HttpSendRequestA);
		DetourDetach(&(PVOID& ) pHttpSendRequestW, hooked_HttpSendRequestW);
		DetourDetach(&(PVOID& ) pHttpSendRequestExA, hooked_HttpSendRequestA);
		DetourDetach(&(PVOID& ) pHttpSendRequestExW, hooked_HttpSendRequestW);
		DetourDetach(&(PVOID& ) pInternetReadFile, hooked_InternetReadFile);
		DetourDetach(&(PVOID& ) pInternetReadFileExA, hooked_InternetReadFileExA);
		DetourDetach(&(PVOID& ) pInternetReadFileExW, hooked_InternetReadFileExW);
		DetourUpdateThread(GetCurrentThread());
		error = DetourTransactionCommit();
		
		if (error)
		{
			// Print error
			_ftprintf(stderr, _T("Unable to unhook Network library\n"));
		}

		// NewProcess
		DetourTransactionBegin();
		DetourDetach(&(PVOID& ) pCreateProcessW, hooked_CreateProcessW);
		DetourDetach(&(PVOID& ) pCreateRemoteThread, hooked_CreateRemoteThread);
		DetourDetach(&(PVOID& ) pVirtualAllocEx, hooked_VirtualAllocEx);
		DetourDetach(&(PVOID& ) pVirtualProtectEx, hooked_VirtualProtectEx);
		DetourDetach(&(PVOID& ) pWriteProcessMemory, hooked_WriteProcessMemory);
		DetourUpdateThread(GetCurrentThread());
		error = DetourTransactionCommit();
		
		if (error)
		{
			// Print error
			_ftprintf(stderr, _T("Unable to unhook Process Creation library\n"));
		}

		// Registry
		DetourTransactionBegin();
		DetourDetach(&(PVOID& ) pRegOpenKeyExA, hooked_RegOpenKeyExA);
		DetourDetach(&(PVOID& ) pRegOpenKeyExW, hooked_RegOpenKeyExW);
		DetourDetach(&(PVOID& ) pRegCreateKeyExA, hooked_RegCreateKeyExA);
		DetourDetach(&(PVOID& ) pRegCreateKeyExW, hooked_RegCreateKeyExW);
		DetourDetach(&(PVOID& ) pRegDeleteKeyExA, hooked_RegDeleteKeyExA);
		DetourDetach(&(PVOID& ) pRegDeleteKeyExW, hooked_RegDeleteKeyExW);
		DetourDetach(&(PVOID& ) pRegQueryValueExA, hooked_RegQueryValueExA);
		DetourDetach(&(PVOID& ) pRegQueryValueExW, hooked_RegQueryValueExW);
		DetourDetach(&(PVOID& ) pRegEnumKeyExA, hooked_RegEnumKeyExA);
		DetourDetach(&(PVOID& ) pRegEnumKeyExA, hooked_RegEnumKeyExA);
		DetourDetach(&(PVOID& ) pRegGetValueA, hooked_RegGetValueA);
		DetourDetach(&(PVOID& ) pRegGetValueW, hooked_RegGetValueW);
		DetourDetach(&(PVOID& ) pRegSetValueExA, hooked_RegSetValueExA);
		DetourDetach(&(PVOID& ) pRegSetValueExW, hooked_RegSetValueExW);
		DetourUpdateThread(GetCurrentThread());
		error = DetourTransactionCommit();

		if (error)
		{
			// Print error
			_ftprintf(stderr, _T("Unable to unhook Registry library\n"));
		}

		// Free the linked list.
		if (pMonitoredRWX_LL_head)
		{
			// If there is only one element.
			if (pMonitoredRWX_LL_head == pMonitoredRWX_LL_prev)
			{
				free(pMonitoredRWX_LL_head);
				goto ___end;
			}
			pTemp1MonitoredRWX_LL = pMonitoredRWX_LL_head;

			while (pTemp1MonitoredRWX_LL->next)
			{
				pTemp2MonitoredRWX_LL = pTemp1MonitoredRWX_LL->next;
				free(pTemp1MonitoredRWX_LL);
				pTemp1MonitoredRWX_LL = pTemp2MonitoredRWX_LL;
			}

			free(pTemp1MonitoredRWX_LL);	// Technically we could free pMonitoredRWX_LL_prev, either would work.
		}
___end:
		break;
	}
	return TRUE;
}



// Detours must have at least one export in order to function.
extern "C" __declspec(dllexport) void dummy()
{
	return;
}