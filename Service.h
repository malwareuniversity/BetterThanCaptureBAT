#ifndef SERVICE_H
#define SERVICE_H

// All of these are defined in sechost.dll.
extern BOOL (WINAPI* pControlService)(SC_HANDLE hService,
									  DWORD dwControl,
									  LPSERVICE_STATUS lpServiceStatus);
BOOL WINAPI hooked_ControlService(SC_HANDLE hService,
									  DWORD dwControl,
									  LPSERVICE_STATUS lpServiceStatus);
extern BOOL (WINAPI* pControlServiceExA)(SC_HANDLE hService,
									  DWORD dwControl,
									  DWORD dwInfoLevel,
									  PVOID pControlParams);
BOOL WINAPI hooked_ControlServiceExA(SC_HANDLE hService,
									  DWORD dwControl,
									  DWORD dwInfoLevel,
									  PVOID pControlParams);
extern BOOL (WINAPI* pControlServiceExW)(SC_HANDLE hService,
									  DWORD dwControl,
									  DWORD dwInfoLevel,
									  PVOID pControlParams);
BOOL WINAPI hooked_ControlServiceExW(SC_HANDLE hService,
									  DWORD dwControl,
									  DWORD dwInfoLevel,
									  PVOID pControlParams);

extern SC_HANDLE (WINAPI* pCreateServiceA)(SC_HANDLE hSCManager,
										   LPCSTR lpServiceName,
										   LPCSTR lpDisplayName,
										   DWORD dwDesiredAccess,
										   DWORD dwServiceType,
										   DWORD dwStartType,
										   DWORD dwErrorControl,
										   LPCSTR lpBinaryPathName,
										   LPCSTR lpLoadOrderGroup,
										   LPDWORD lpdwTagId,
										   LPCSTR lpDependencies,
										   LPCSTR lpServiceStartName,
										   LPCSTR lpPassword);
SC_HANDLE WINAPI hooked_CreateServiceA(SC_HANDLE hSCManager,
										   LPCSTR lpServiceName,
										   LPCSTR lpDisplayName,
										   DWORD dwDesiredAccess,
										   DWORD dwServiceType,
										   DWORD dwStartType,
										   DWORD dwErrorControl,
										   LPCSTR lpBinaryPathName,
										   LPCSTR lpLoadOrderGroup,
										   LPDWORD lpdwTagId,
										   LPCSTR lpDependencies,
										   LPCSTR lpServiceStartName,
										   LPCSTR lpPassword);
extern SC_HANDLE (WINAPI* pCreateServiceW)(SC_HANDLE hSCManager,
										   LPCWSTR lpServiceName,
										   LPCWSTR lpDisplayName,
										   DWORD dwDesiredAccess,
										   DWORD dwServiceType,
										   DWORD dwStartType,
										   DWORD dwErrorControl,
										   LPCWSTR lpBinaryPathName,
										   LPCWSTR lpLoadOrderGroup,
										   LPDWORD lpdwTagId,
										   LPCWSTR lpDependencies,
										   LPCWSTR lpServiceStartName,
										   LPCWSTR lpPassword);
SC_HANDLE WINAPI hooked_CreateServiceW(SC_HANDLE hSCManager,
										   LPCWSTR lpServiceName,
										   LPCWSTR lpDisplayName,
										   DWORD dwDesiredAccess,
										   DWORD dwServiceType,
										   DWORD dwStartType,
										   DWORD dwErrorControl,
										   LPCWSTR lpBinaryPathName,
										   LPCWSTR lpLoadOrderGroup,
										   LPDWORD lpdwTagId,
										   LPCWSTR lpDependencies,
										   LPCWSTR lpServiceStartName,
										   LPCWSTR lpPassword);

extern BOOL (WINAPI* pDeleteService)(SC_HANDLE hService);
BOOL WINAPI hooked_DeleteService(SC_HANDLE hService);

extern BOOL (WINAPI* pEnumServicesStatusA)(SC_HANDLE hSCManager,
										  DWORD dwServiceType,
										  DWORD dwServiceState,
										  LPENUM_SERVICE_STATUSA lpServices,
										  DWORD cbBufSize,
										  LPDWORD pcbBytesNeeded,
										  LPDWORD lpServicesReturned,
										  LPDWORD lpResumeHandle);
BOOL WINAPI hooked_EnumServicesStatusA(SC_HANDLE hSCManager,
										  DWORD dwServiceType,
										  DWORD dwServiceState,
										  LPENUM_SERVICE_STATUSA lpServices,
										  DWORD cbBufSize,
										  LPDWORD pcbBytesNeeded,
										  LPDWORD lpServicesReturned,
										  LPDWORD lpResumeHandle);
extern BOOL (WINAPI* pEnumServicesStatusW)(SC_HANDLE hSCManager,
										  DWORD dwServiceType,
										  DWORD dwServiceState,
										  LPENUM_SERVICE_STATUSW lpServices,
										  DWORD cbBufSize,
										  LPDWORD pcbBytesNeeded,
										  LPDWORD lpServicesReturned,
										  LPDWORD lpResumeHandle);
BOOL WINAPI hooked_EnumServicesStatusW(SC_HANDLE hSCManager,
										  DWORD dwServiceType,
										  DWORD dwServiceState,
										  LPENUM_SERVICE_STATUSW lpServices,
										  DWORD cbBufSize,
										  LPDWORD pcbBytesNeeded,
										  LPDWORD lpServicesReturned,
										  LPDWORD lpResumeHandle);

extern BOOL (WINAPI* pEnumServicesStatusExA)(SC_HANDLE hSCManager,
											 SC_ENUM_TYPE InfoLevel,
											 DWORD dwServiceType,
											 DWORD dwServiceState,
											 LPBYTE lpServices,
											 DWORD cbBufSize,
											 LPDWORD pcbBytesNeeded,
											 LPDWORD lpServicesReturned,
											 LPDWORD lpResumeHandle,
											 LPCSTR pszGroupName);
BOOL WINAPI hooked_EnumServicesStatusExA(SC_HANDLE hSCManager,
											 SC_ENUM_TYPE InfoLevel,
											 DWORD dwServiceType,
											 DWORD dwServiceState,
											 LPBYTE lpServices,
											 DWORD cbBufSize,
											 LPDWORD pcbBytesNeeded,
											 LPDWORD lpServicesReturned,
											 LPDWORD lpResumeHandle,
											 LPCSTR pszGroupName);
extern BOOL (WINAPI* pEnumServicesStatusExW)(SC_HANDLE hSCManager,
											 SC_ENUM_TYPE InfoLevel,
											 DWORD dwServiceType,
											 DWORD dwServiceState,
											 LPBYTE lpServices,
											 DWORD cbBufSize,
											 LPDWORD pcbBytesNeeded,
											 LPDWORD lpServicesReturned,
											 LPDWORD lpResumeHandle,
											 LPCWSTR pszGroupName);
BOOL WINAPI hooked_EnumServicesStatusExW(SC_HANDLE hSCManager,
											 SC_ENUM_TYPE InfoLevel,
											 DWORD dwServiceType,
											 DWORD dwServiceState,
											 LPBYTE lpServices,
											 DWORD cbBufSize,
											 LPDWORD pcbBytesNeeded,
											 LPDWORD lpServicesReturned,
											 LPDWORD lpResumeHandle,
											 LPCWSTR pszGroupName);

extern SC_HANDLE (WINAPI* pOpenSCManagerA)(LPCSTR lpMachineName,
										   LPCSTR lpDatabaseName,
										   DWORD dwDesiredAccess);
SC_HANDLE WINAPI hooked_OpenSCManagerA(LPCSTR lpMachineName,
										   LPCSTR lpDatabaseName,
										   DWORD dwDesiredAccess);
extern SC_HANDLE (WINAPI* pOpenSCManagerW)(LPCWSTR lpMachineName,
										   LPCWSTR lpDatabaseName,
										   DWORD dwDesiredAccess);
SC_HANDLE WINAPI hooked_OpenSCManagerW(LPCWSTR lpMachineName,
										   LPCWSTR lpDatabaseName,
										   DWORD dwDesiredAccess);

extern SC_HANDLE (WINAPI* pOpenServiceA)(SC_HANDLE hSCManager,
										LPCSTR lpServiceName,
										DWORD dwDesiredAccess);
SC_HANDLE WINAPI hooked_OpenServiceA(SC_HANDLE hSCManager,
										LPCSTR lpServiceName,
										DWORD dwDesiredAccess);
extern SC_HANDLE (WINAPI* pOpenServiceW)(SC_HANDLE hSCManager,
										LPCWSTR lpServiceName,
										DWORD dwDesiredAccess);
SC_HANDLE WINAPI hooked_OpenServiceW(SC_HANDLE hSCManager,
										LPCWSTR lpServiceName,
										DWORD dwDesiredAccess);

extern BOOL (WINAPI* pStartServiceA)(SC_HANDLE hService,
									 DWORD dwNumServiceArgs,
									 LPCSTR* lpServiceArgVectors);
BOOL WINAPI hooked_StartServiceA(SC_HANDLE hService,
									 DWORD dwNumServiceArgs,
									 LPCSTR* lpServiceArgVectors);
extern BOOL (WINAPI* pStartServiceW)(SC_HANDLE hService,
									 DWORD dwNumServiceArgs,
									 LPCWSTR* lpServiceArgVectors);
BOOL WINAPI hooked_StartServiceW(SC_HANDLE hService,
									 DWORD dwNumServiceArgs,
									 LPCWSTR* lpServiceArgVectors);
#endif