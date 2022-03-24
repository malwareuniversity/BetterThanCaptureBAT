#ifndef NEWPROCESS_H
#define NEWPROCESS_H

// We must declare, not define, function pointers in the header.  Otherwise, linker fails.
extern BOOL (WINAPI* pCreateProcessW)(LPCWSTR lpApplicationName,
							 LPWSTR lpCommandLine,
							 LPSECURITY_ATTRIBUTES lpProcessAttributes,
							 LPSECURITY_ATTRIBUTES lpThreadAttributes,
							 BOOL bInheritHandles,
							 DWORD dwCreationFlags,
							 LPVOID lpEnvironment,
							 LPCWSTR lpCurrentDirectory,
							 LPSTARTUPINFOW lpStartupInfo,
							 LPPROCESS_INFORMATION lpProcessInformation);
BOOL WINAPI hooked_CreateProcessW(LPCWSTR lpApplicationName,
							 LPWSTR lpCommandLine,
							 LPSECURITY_ATTRIBUTES lpProcessAttributes,
							 LPSECURITY_ATTRIBUTES lpThreadAttributes,
							 BOOL bInheritHandles,
							 DWORD dwCreationFlags,
							 LPVOID lpEnvironment,
							 LPCWSTR lpCurrentDirectory,
							 LPSTARTUPINFOW lpStartupInfo,
							 LPPROCESS_INFORMATION lpProcessInformation);

extern HANDLE (WINAPI* pCreateRemoteThread)(HANDLE hProcess,
									 LPSECURITY_ATTRIBUTES lpThreadAttributes,
									 SIZE_T dwStackSize,
									 LPTHREAD_START_ROUTINE lpStartAddress,
									 LPVOID lpParameter,
									 DWORD dwCreationFlags,
									 LPDWORD lpThreadId);
HANDLE WINAPI hooked_CreateRemoteThread(HANDLE hProcess,
									 LPSECURITY_ATTRIBUTES lpThreadAttributes,
									 SIZE_T dwStackSize,
									 LPTHREAD_START_ROUTINE lpStartAddress,
									 LPVOID lpParameter,
									 DWORD dwCreationFlags,
									 LPDWORD lpThreadId);

extern LPVOID (WINAPI* pVirtualAllocEx)(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flAllocationType,
								 DWORD flProtect);
LPVOID WINAPI hooked_VirtualAllocEx(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flAllocationType,
								 DWORD flProtect);

extern BOOL (WINAPI* pVirtualProtectEx)(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flNewProtect,
								 PDWORD lpflOldProtect);
BOOL WINAPI hooked_VirtualProtectEx(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flNewProtect,
								 PDWORD lpflOldProtect);

extern BOOL (WINAPI* pWriteProcessMemory)(HANDLE hProcess,
								  LPVOID lpBaseAddress,
								  LPCVOID lpBuffer,
								  SIZE_T nSize,
								  SIZE_T* lpNumberOfBytesWritten);
BOOL WINAPI hooked_WriteProcessMemory(HANDLE hProcess,
								  LPVOID lpBaseAddress,
								  LPCVOID lpBuffer,
								  SIZE_T nSize,
								  SIZE_T* lpNumberOfBytesWritten);



// Dynamic resolution.
typedef DWORD (WINAPI* GetProcessImageFileName_t)(HANDLE hProcess,
											LPTSTR lpImageFileName,
											DWORD nSize);
static GetProcessImageFileName_t fpGetProcessImageFileName = NULL;


extern bool bFollowProcesses;
BOOL WINAPI hooked_CreateProcessW(LPCWSTR lpApplicationName,
							 LPWSTR lpCommandLine,
							 LPSECURITY_ATTRIBUTES lpProcessAttributes,
							 LPSECURITY_ATTRIBUTES lpThreadAttributes,
							 BOOL bInheritHandles,
							 DWORD dwCreationFlags,
							 LPVOID lpEnvironment,
							 LPCWSTR lpCurrentDirectory,
							 LPSTARTUPINFOW lpStartupInfo,
							 LPPROCESS_INFORMATION lpProcessInformation);

HANDLE WINAPI hooked_CreateRemoteThread(HANDLE hProcess,
									 LPSECURITY_ATTRIBUTES lpThreadAttributes,
									 SIZE_T dwStackSize,
									 LPTHREAD_START_ROUTINE lpStartAddress,
									 LPVOID lpParameter,
									 DWORD dwCreationFlags,
									 LPDWORD lpThreadId);

// We keep track of all VirtualProtectEx calls for RWX in order to "grab" shellcode during WriteProcessMemory().
// DO NOT USE pMonitoredRWX_LL_prev for search.  That's what head is for.
typedef struct monitoredRWX_LL_t
{
	HANDLE				hProcess;
	LPVOID				lpAddress;
	SIZE_T				dwSize;
	monitoredRWX_LL_t*	next;
};
extern monitoredRWX_LL_t* pMonitoredRWX_LL_head, * pMonitoredRWX_LL_prev;

LPVOID WINAPI hooked_VirtualAllocEx(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flAllocationType,
								 DWORD flProtect);

BOOL WINAPI hooked_VirtualProtectEx(HANDLE hProcess,
								 LPVOID lpAddress,
								 SIZE_T dwSize,
								 DWORD flNewProtect,
								 PDWORD lpflOldProtect);

BOOL WINAPI hooked_WriteProcessMemory(HANDLE hProcess,
								  LPVOID lpBaseAddress,
								  LPCVOID lpBuffer,
								  SIZE_T nSize,
								  SIZE_T* lpNumberOfBytesWritten);
#endif