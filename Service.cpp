/* Network.cpp - BetterThanCaptureBAT

			Monitors the service handler.
			*/

#include "stdafx.h"

#ifndef SERVICE_H
#include "Service.h"
#endif

// Extern redefinitions.
BOOL (WINAPI* pControlService)(SC_HANDLE hService,
									  DWORD dwControl,
									  LPSERVICE_STATUS lpServiceStatus) = ControlService;
BOOL (WINAPI* pControlServiceExA)(SC_HANDLE hService,
									  DWORD dwControl,
									  DWORD dwInfoLevel,
									  PVOID pControlParams) = ControlServiceExA;
BOOL (WINAPI* pControlServiceExW)(SC_HANDLE hService,
									  DWORD dwControl,
									  DWORD dwInfoLevel,
									  PVOID pControlParams) = ControlServiceExW;
SC_HANDLE (WINAPI* pCreateServiceA)(SC_HANDLE hSCManager,
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
										   LPCSTR lpPassword) = CreateServiceA;
SC_HANDLE (WINAPI* pCreateServiceW)(SC_HANDLE hSCManager,
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
										   LPCWSTR lpPassword) = CreateServiceW;
BOOL (WINAPI* pDeleteService)(SC_HANDLE hService) = DeleteService;
BOOL (WINAPI* pEnumServicesStatusA)(SC_HANDLE hSCManager,
										  DWORD dwServiceType,
										  DWORD dwServiceState,
										  LPENUM_SERVICE_STATUSA lpServices,
										  DWORD cbBufSize,
										  LPDWORD pcbBytesNeeded,
										  LPDWORD lpServicesReturned,
										  LPDWORD lpResumeHandle) = EnumServicesStatusA;
BOOL (WINAPI* pEnumServicesStatusW)(SC_HANDLE hSCManager,
										  DWORD dwServiceType,
										  DWORD dwServiceState,
										  LPENUM_SERVICE_STATUSW lpServices,
										  DWORD cbBufSize,
										  LPDWORD pcbBytesNeeded,
										  LPDWORD lpServicesReturned,
										  LPDWORD lpResumeHandle) = EnumServicesStatusW;
BOOL (WINAPI* pEnumServicesStatusExA)(SC_HANDLE hSCManager,
											 SC_ENUM_TYPE InfoLevel,
											 DWORD dwServiceType,
											 DWORD dwServiceState,
											 LPBYTE lpServices,
											 DWORD cbBufSize,
											 LPDWORD pcbBytesNeeded,
											 LPDWORD lpServicesReturned,
											 LPDWORD lpResumeHandle,
											 LPCSTR pszGroupName) = EnumServicesStatusExA;
BOOL (WINAPI* pEnumServicesStatusExW)(SC_HANDLE hSCManager,
											 SC_ENUM_TYPE InfoLevel,
											 DWORD dwServiceType,
											 DWORD dwServiceState,
											 LPBYTE lpServices,
											 DWORD cbBufSize,
											 LPDWORD pcbBytesNeeded,
											 LPDWORD lpServicesReturned,
											 LPDWORD lpResumeHandle,
											 LPCWSTR pszGroupName) = EnumServicesStatusExW;
SC_HANDLE (WINAPI* pOpenSCManagerA)(LPCSTR lpMachineName,
										   LPCSTR lpDatabaseName,
										   DWORD dwDesiredAccess) = OpenSCManagerA;
SC_HANDLE (WINAPI* pOpenSCManagerW)(LPCWSTR lpMachineName,
										   LPCWSTR lpDatabaseName,
										   DWORD dwDesiredAccess) = OpenSCManagerW;
SC_HANDLE (WINAPI* pOpenServiceA)(SC_HANDLE hSCManager,
										LPCSTR lpServiceName,
										DWORD dwDesiredAccess) = OpenServiceA;
SC_HANDLE (WINAPI* pOpenServiceW)(SC_HANDLE hSCManager,
										LPCWSTR lpServiceName,
										DWORD dwDesiredAccess) = OpenServiceW;
BOOL (WINAPI* pStartServiceA)(SC_HANDLE hService,
									 DWORD dwNumSErviceArgs,
									 LPCSTR* lpServiceArgVectors) = StartServiceA;
BOOL (WINAPI* pStartServiceW)(SC_HANDLE hService,
									 DWORD dwNumSErviceArgs,
									 LPCWSTR* lpServiceArgVectors) = StartServiceW;



BOOL WINAPI hooked_ControlService(SC_HANDLE hService,
									  DWORD dwControl,
									  LPSERVICE_STATUS lpServiceStatus)
{
	FILE*					logFile;
	QUERY_SERVICE_CONFIGW	qsCfg;
	TCHAR*					serviceName;
	DWORD					bytesNeeded;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pControlService(hService, dwControl, lpServiceStatus);
	}

	if (!QueryServiceConfig(hService, &qsCfg, sizeof(qsCfg), &bytesNeeded))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			fprintf(stderr, "Insufficient buffer allocation for ControlService handler\n");
		fprintf(stderr, "Error querying service configuration\n");

		serviceName = _T("Unknown");
	}

	else
		serviceName = qsCfg.lpDisplayName;
	if (serviceName)	// should be unneccessary
		if (!*serviceName)	// check for empty string
			serviceName = _T("Unknown");

	// Can only have one control code per call.
	_ftprintf(logFile, _T("Sending control code %s to %s\n\n"), (dwControl & SERVICE_CONTROL_CONTINUE) ? _T("SERVICE_CONTROL_CONTINUE") :
																	(dwControl & SERVICE_CONTROL_INTERROGATE) ? _T("SERVICE_CONTROL_INTERROGATE") :
																	(dwControl & SERVICE_CONTROL_NETBINDADD) ? _T("SERVICE_CONTROL_NETBINDADD") :
																	(dwControl & SERVICE_CONTROL_NETBINDDISABLE) ? _T("SERVICE_CONTROL_NETBINDDISABLE") :
																	(dwControl & SERVICE_CONTROL_NETBINDENABLE) ? _T("SERVICE_CONTROL_NETBINDENABLE") :
																	(dwControl & SERVICE_CONTROL_NETBINDREMOVE) ? _T("SERVICE_CONTROL_NETBINDREMOVE") :
																	(dwControl & SERVICE_CONTROL_PARAMCHANGE) ? _T("SERVICE_CONTROL_PARAMCHANGE") :
																	(dwControl & SERVICE_CONTROL_PAUSE) ? _T("SERVICE_CONTROL_PAUSE") :
																	(dwControl & SERVICE_CONTROL_STOP) ? _T("SERVICE_CONTROL_STOP") : _T("UNKNOWN"), serviceName);
	fclose(logFile);

	return pControlService(hService, dwControl, lpServiceStatus);
}

// Don't handle the SERVICE_CONTROL_STATUS_REASON_PARAMS yet.
BOOL WINAPI hooked_ControlServiceExA(SC_HANDLE hService,
									  DWORD dwControl,
									  DWORD dwInfoLevel,
									  PVOID pControlParams)
{
	FILE*					logFile;
	QUERY_SERVICE_CONFIGW	qsCfg;
	TCHAR*					serviceName;
	DWORD					bytesNeeded;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pControlServiceExA(hService, dwControl, dwInfoLevel, pControlParams);
	}

	if (!QueryServiceConfig(hService, &qsCfg, sizeof(qsCfg), &bytesNeeded))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			fprintf(stderr, "Insufficient buffer allocation for ControlService handler\n");
		fprintf(stderr, "Error querying service configuration\n");

		serviceName = _T("Unknown");
	}

	else
		serviceName = qsCfg.lpDisplayName;
	if (serviceName)	// should be unneccessary
		if (!*serviceName)	// check for empty string
			serviceName = _T("Unknown");

	// Can only have one control code per call.
	_ftprintf(logFile, _T("Sending control code %s to %s\n\n"), (dwControl & SERVICE_CONTROL_CONTINUE) ? _T("SERVICE_CONTROL_CONTINUE") :
																	(dwControl & SERVICE_CONTROL_INTERROGATE) ? _T("SERVICE_CONTROL_INTERROGATE") :
																	(dwControl & SERVICE_CONTROL_NETBINDADD) ? _T("SERVICE_CONTROL_NETBINDADD") :
																	(dwControl & SERVICE_CONTROL_NETBINDDISABLE) ? _T("SERVICE_CONTROL_NETBINDDISABLE") :
																	(dwControl & SERVICE_CONTROL_NETBINDENABLE) ? _T("SERVICE_CONTROL_NETBINDENABLE") :
																	(dwControl & SERVICE_CONTROL_NETBINDREMOVE) ? _T("SERVICE_CONTROL_NETBINDREMOVE") :
																	(dwControl & SERVICE_CONTROL_PARAMCHANGE) ? _T("SERVICE_CONTROL_PARAMCHANGE") :
																	(dwControl & SERVICE_CONTROL_PAUSE) ? _T("SERVICE_CONTROL_PAUSE") :
																	(dwControl & SERVICE_CONTROL_STOP) ? _T("SERVICE_CONTROL_STOP") : 
																	(dwControl >= 128 && dwControl <= 255) ? _T("SERVICE_USER_DEINFED_CONTROL") : _T("UNKNOWN"), serviceName);
	fclose(logFile);

	return pControlServiceExA(hService, dwControl, dwInfoLevel, pControlParams);
}

BOOL WINAPI hooked_ControlServiceExW(SC_HANDLE hService,
									  DWORD dwControl,
									  DWORD dwInfoLevel,
									  PVOID pControlParams)
{
	FILE*					logFile;
	QUERY_SERVICE_CONFIGW	qsCfg;
	TCHAR*					serviceName;
	DWORD					bytesNeeded;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pControlServiceExW(hService, dwControl, dwInfoLevel, pControlParams);
	}

	if (!QueryServiceConfig(hService, &qsCfg, sizeof(qsCfg), &bytesNeeded))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			fprintf(stderr, "Insufficient buffer allocation for ControlService handler\n");
		fprintf(stderr, "Error querying service configuration\n");

		serviceName = _T("Unknown");
	}

	else
		serviceName = qsCfg.lpDisplayName;
	if (serviceName)	// should be unneccessary
		if (!*serviceName)	// check for empty string
			serviceName = _T("Unknown");

	// Can only have one control code per call.
	_ftprintf(logFile, _T("Sending control code %s to %s\n\n"), (dwControl & SERVICE_CONTROL_CONTINUE) ? _T("SERVICE_CONTROL_CONTINUE") :
																	(dwControl & SERVICE_CONTROL_INTERROGATE) ? _T("SERVICE_CONTROL_INTERROGATE") :
																	(dwControl & SERVICE_CONTROL_NETBINDADD) ? _T("SERVICE_CONTROL_NETBINDADD") :
																	(dwControl & SERVICE_CONTROL_NETBINDDISABLE) ? _T("SERVICE_CONTROL_NETBINDDISABLE") :
																	(dwControl & SERVICE_CONTROL_NETBINDENABLE) ? _T("SERVICE_CONTROL_NETBINDENABLE") :
																	(dwControl & SERVICE_CONTROL_NETBINDREMOVE) ? _T("SERVICE_CONTROL_NETBINDREMOVE") :
																	(dwControl & SERVICE_CONTROL_PARAMCHANGE) ? _T("SERVICE_CONTROL_PARAMCHANGE") :
																	(dwControl & SERVICE_CONTROL_PAUSE) ? _T("SERVICE_CONTROL_PAUSE") :
																	(dwControl & SERVICE_CONTROL_STOP) ? _T("SERVICE_CONTROL_STOP") : 
																	(dwControl >= 128 && dwControl <= 255) ? _T("SERVICE_USER_DEINFED_CONTROL") : _T("UNKNOWN"), serviceName);
	fclose(logFile);

	return pControlServiceExW(hService, dwControl, dwInfoLevel, pControlParams);
}

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
										   LPCSTR lpPassword)
{
	FILE*		logFile;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pCreateServiceA(hSCManager, lpServiceName, lpDisplayName, dwDesiredAccess, dwServiceType,
								dwStartType, dwErrorControl, lpBinaryPathName, lpLoadOrderGroup, lpdwTagId,
								lpDependencies, lpServiceStartName, lpPassword);
	}

	_ftprintf(logFile, _T("Creating Service\n"));
	_ftprintf(logFile, _T("Service Name:  %s\n"), lpServiceName);
	_ftprintf(logFile, _T("Display Name:  %s\n"), lpDisplayName);
	_ftprintf(logFile, _T("Access Rights:  N/A"));
	_ftprintf(logFile, _T("Service Type:  %s"), (dwServiceType == SERVICE_ADAPTER) ? _T("SERVICE_ADAPTER") :
													(dwServiceType == SERVICE_FILE_SYSTEM_DRIVER) ? _T("SERVICE_FILE_SYSTEM_DRIVER") :
													(dwServiceType == SERVICE_KERNEL_DRIVER) ? _T("SERVICE_KERNEL_DRIVER") :
													(dwServiceType == SERVICE_RECOGNIZER_DRIVER) ? _T("SERVICE_RECOGNIZER_DRIVER") :
													(dwServiceType | SERVICE_WIN32_OWN_PROCESS) ? _T("SERVICE_WIN32_OWN_PROCESS") :
													(dwServiceType | SERVICE_WIN32_SHARE_PROCESS) ? _T("SERVICE_WIN32_SHARE_PROCESS") : _T("Unknown"));
	if (dwServiceType | SERVICE_WIN32_OWN_PROCESS || dwServiceType | SERVICE_WIN32_SHARE_PROCESS)
		if (dwServiceType | SERVICE_INTERACTIVE_PROCESS)
			_ftprintf(logFile, _T(" | SERVICE_INTERACTIVE_PROCESS"));
	_ftprintf(logFile, _T("\n"));
	_ftprintf(logFile, _T("Start Type:  %s\n"), (dwStartType == SERVICE_AUTO_START) ? _T("SERVICE_AUTO_START") :
												(dwStartType == SERVICE_BOOT_START) ? _T("SERVICE_BOOT_START") :
												(dwStartType == SERVICE_DEMAND_START) ? _T("SERVICE_DEMAND_START") :
												(dwStartType == SERVICE_DISABLED) ? _T("SERVICE_DISABLED") :
												(dwStartType == SERVICE_SYSTEM_START) ? _T("SERVICE_SYSTEM_START") : _T("Unknown"));
	_ftprintf(logFile, _T("Error Control:  %s\n"), (dwErrorControl == SERVICE_ERROR_CRITICAL) ? _T("SERVICE_ERROR_CRITICAL") :
													(dwErrorControl == SERVICE_ERROR_IGNORE) ? _T("SERVICE_ERROR_IGNORE") :
													(dwErrorControl == SERVICE_ERROR_NORMAL) ? _T("SERVICE_ERROR_NORMAL") :
													(dwErrorControl == SERVICE_ERROR_SEVERE) ? _T("SERVICE_ERROR_SEVERE") : _T("Unknown"));
	if (lpBinaryPathName)
		_ftprintf(logFile, _T("Binary Path:  %s\n"), lpBinaryPathName);
	if (lpLoadOrderGroup)
		_ftprintf(logFile, _T("Load Order Group:  %s\n"), lpLoadOrderGroup);
	if (lpServiceStartName)
		_ftprintf(logFile, _T("Service Start Name:  %s\n"), lpServiceStartName);

	fclose(logFile);

	return pCreateServiceA(hSCManager, lpServiceName, lpDisplayName, dwDesiredAccess, dwServiceType,
								dwStartType, dwErrorControl, lpBinaryPathName, lpLoadOrderGroup, lpdwTagId,
								lpDependencies, lpServiceStartName, lpPassword);
}

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
										   LPCWSTR lpPassword)
{
	FILE*		logFile;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pCreateServiceW(hSCManager, lpServiceName, lpDisplayName, dwDesiredAccess, dwServiceType,
								dwStartType, dwErrorControl, lpBinaryPathName, lpLoadOrderGroup, lpdwTagId,
								lpDependencies, lpServiceStartName, lpPassword);
	}

	_ftprintf(logFile, _T("Creating Service\n"));
	_ftprintf(logFile, _T("Service Name:  %s\n"), lpServiceName);
	_ftprintf(logFile, _T("Display Name:  %s\n"), lpDisplayName);
	_ftprintf(logFile, _T("Access Rights:  N/A"));
	_ftprintf(logFile, _T("Service Type:  %s"), (dwServiceType == SERVICE_ADAPTER) ? _T("SERVICE_ADAPTER") :
													(dwServiceType == SERVICE_FILE_SYSTEM_DRIVER) ? _T("SERVICE_FILE_SYSTEM_DRIVER") :
													(dwServiceType == SERVICE_KERNEL_DRIVER) ? _T("SERVICE_KERNEL_DRIVER") :
													(dwServiceType == SERVICE_RECOGNIZER_DRIVER) ? _T("SERVICE_RECOGNIZER_DRIVER") :
													(dwServiceType | SERVICE_WIN32_OWN_PROCESS) ? _T("SERVICE_WIN32_OWN_PROCESS") :
													(dwServiceType | SERVICE_WIN32_SHARE_PROCESS) ? _T("SERVICE_WIN32_SHARE_PROCESS") : _T("Unknown"));
	if (dwServiceType | SERVICE_WIN32_OWN_PROCESS || dwServiceType | SERVICE_WIN32_SHARE_PROCESS)
		if (dwServiceType | SERVICE_INTERACTIVE_PROCESS)
			_ftprintf(logFile, _T(" | SERVICE_INTERACTIVE_PROCESS"));
	_ftprintf(logFile, _T("\n"));
	_ftprintf(logFile, _T("Start Type:  %s\n"), (dwStartType == SERVICE_AUTO_START) ? _T("SERVICE_AUTO_START") :
												(dwStartType == SERVICE_BOOT_START) ? _T("SERVICE_BOOT_START") :
												(dwStartType == SERVICE_DEMAND_START) ? _T("SERVICE_DEMAND_START") :
												(dwStartType == SERVICE_DISABLED) ? _T("SERVICE_DISABLED") :
												(dwStartType == SERVICE_SYSTEM_START) ? _T("SERVICE_SYSTEM_START") : _T("Unknown"));
	_ftprintf(logFile, _T("Error Control:  %s\n"), (dwErrorControl == SERVICE_ERROR_CRITICAL) ? _T("SERVICE_ERROR_CRITICAL") :
													(dwErrorControl == SERVICE_ERROR_IGNORE) ? _T("SERVICE_ERROR_IGNORE") :
													(dwErrorControl == SERVICE_ERROR_NORMAL) ? _T("SERVICE_ERROR_NORMAL") :
													(dwErrorControl == SERVICE_ERROR_SEVERE) ? _T("SERVICE_ERROR_SEVERE") : _T("Unknown"));
	if (lpBinaryPathName)
		_ftprintf(logFile, _T("Binary Path:  %s\n"), lpBinaryPathName);
	if (lpLoadOrderGroup)
		_ftprintf(logFile, _T("Load Order Group:  %s\n"), lpLoadOrderGroup);
	if (lpServiceStartName)
		_ftprintf(logFile, _T("Service Start Name:  %s\n"), lpServiceStartName);

	fclose(logFile);

	return pCreateServiceW(hSCManager, lpServiceName, lpDisplayName, dwDesiredAccess, dwServiceType,
								dwStartType, dwErrorControl, lpBinaryPathName, lpLoadOrderGroup, lpdwTagId,
								lpDependencies, lpServiceStartName, lpPassword);
}

BOOL WINAPI hooked_DeleteService(SC_HANDLE hService)
{
	FILE*					logFile;
	QUERY_SERVICE_CONFIGW	qsCfg;
	TCHAR*					serviceName;
	DWORD					bytesNeeded;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pDeleteService(hService);
	}

	if (!QueryServiceConfig(hService, &qsCfg, sizeof(qsCfg), &bytesNeeded))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			fprintf(stderr, "Insufficient buffer allocation for ControlService handler\n");
		fprintf(stderr, "Error querying service configuration\n");

		serviceName = _T("Unknown");
	}

	else
		serviceName = qsCfg.lpDisplayName;
	if (serviceName)	// should be unneccessary
		if (!*serviceName)	// check for empty string
			serviceName = _T("Unknown");

	_ftprintf(logFile, _T("Deleting Service %s\n"), serviceName);
	fclose(logFile);

	return pDeleteService(hService);
}

BOOL WINAPI hooked_EnumServicesStatusA(SC_HANDLE hSCManager,
										  DWORD dwServiceType,
										  DWORD dwServiceState,
										  LPENUM_SERVICE_STATUSA lpServices,
										  DWORD cbBufSize,
										  LPDWORD pcbBytesNeeded,
										  LPDWORD lpServicesReturned,
										  LPDWORD lpResumeHandle)
{
	return pEnumServicesStatusA(hSCManager, dwServiceType, dwServiceState, lpServices,
								cbBufSize, pcbBytesNeeded, lpServicesReturned, lpResumeHandle);
}

BOOL WINAPI hooked_EnumServicesStatusW(SC_HANDLE hSCManager,
										  DWORD dwServiceType,
										  DWORD dwServiceState,
										  LPENUM_SERVICE_STATUSW lpServices,
										  DWORD cbBufSize,
										  LPDWORD pcbBytesNeeded,
										  LPDWORD lpServicesReturned,
										  LPDWORD lpResumeHandle)
{
	return pEnumServicesStatusW(hSCManager, dwServiceType, dwServiceState, lpServices,
								cbBufSize, pcbBytesNeeded, lpServicesReturned, lpResumeHandle);
}

BOOL WINAPI hooked_EnumServicesStatusExA(SC_HANDLE hSCManager,
											 SC_ENUM_TYPE InfoLevel,
											 DWORD dwServiceType,
											 DWORD dwServiceState,
											 LPBYTE lpServices,
											 DWORD cbBufSize,
											 LPDWORD pcbBytesNeeded,
											 LPDWORD lpServicesReturned,
											 LPDWORD lpResumeHandle,
											 LPCSTR pszGroupName)
{
	return pEnumServicesStatusExA(hSCManager, InfoLevel, dwServiceType, dwServiceState,
									lpServices, cbBufSize, pcbBytesNeeded, lpServicesReturned,
									lpResumeHandle, pszGroupName);
}

BOOL WINAPI hooked_EnumServicesStatusExW(SC_HANDLE hSCManager,
											 SC_ENUM_TYPE InfoLevel,
											 DWORD dwServiceType,
											 DWORD dwServiceState,
											 LPBYTE lpServices,
											 DWORD cbBufSize,
											 LPDWORD pcbBytesNeeded,
											 LPDWORD lpServicesReturned,
											 LPDWORD lpResumeHandle,
											 LPCWSTR pszGroupName)
{
	return pEnumServicesStatusExW(hSCManager, InfoLevel, dwServiceType, dwServiceState,
									lpServices, cbBufSize, pcbBytesNeeded, lpServicesReturned,
									lpResumeHandle, pszGroupName);
}

SC_HANDLE WINAPI hooked_OpenSCManagerA(LPCSTR lpMachineName,
										   LPCSTR lpDatabaseName,
										   DWORD dwDesiredAccess)
{
	FILE*		logFile;
	TCHAR		accessRights[256];
	int			accessRightsIndex = 0;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pOpenSCManagerA(lpMachineName, lpDatabaseName, dwDesiredAccess);
	}

	ZeroMemory(accessRights, 256);

	// SC_MANAGER_ALL_ACCESS encapsulates everything.
	if (dwDesiredAccess == SC_MANAGER_ALL_ACCESS)
		_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_ALL_ACCESS"));
	else
	{
		if (dwDesiredAccess | SC_MANAGER_CREATE_SERVICE)
		{
			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_CREATE_SERVICE"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_CREATE_SERVICE"));
		}

		if (dwDesiredAccess | SC_MANAGER_CONNECT)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_CONNECT"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_CONNECT"));
		}

		if (dwDesiredAccess | SC_MANAGER_ENUMERATE_SERVICE)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_ENUMERATE_SERVICE"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_ENUMERATE_SERVICE"));
		}

		if (dwDesiredAccess | SC_MANAGER_LOCK)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_LOCK"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_LOCK"));
		}

		if (dwDesiredAccess | SC_MANAGER_MODIFY_BOOT_CONFIG)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_MODIFY_BOOT_CONFIG"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_MODIFY_BOOT_CONFIG"));
		}

		if (dwDesiredAccess | SC_MANAGER_ENUMERATE_SERVICE)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_QUERY_LOCK_STATUS"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_QUERY_LOCK_STATUS"));
		}
	}

	_ftprintf(logFile, _T("Opening SC Manager\n"));
	if (lpMachineName)
		_ftprintf(logFile, _T("Machine Name:  %s\n"), lpMachineName);
	_ftprintf(logFile, _T("Databse Name:  %s\n"), (lpDatabaseName) ? lpDatabaseName : "SERVICES_ACTIVE_DATABASE");
	_ftprintf(logFile, _T("Desired Access:  %s\n"), accessRights);
	fclose(logFile);

	return pOpenSCManagerA(lpMachineName, lpDatabaseName, dwDesiredAccess);
}

SC_HANDLE WINAPI hooked_OpenSCManagerW(LPCWSTR lpMachineName,
										   LPCWSTR lpDatabaseName,
										   DWORD dwDesiredAccess)
{
	FILE*		logFile;
	TCHAR		accessRights[256];
	int			accessRightsIndex = 0;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pOpenSCManagerW(lpMachineName, lpDatabaseName, dwDesiredAccess);
	}

	ZeroMemory(accessRights, 256);

	// SC_MANAGER_ALL_ACCESS encapsulates everything.
	if (dwDesiredAccess == SC_MANAGER_ALL_ACCESS)
		_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_ALL_ACCESS"));
	else
	{
		if (dwDesiredAccess | SC_MANAGER_CREATE_SERVICE)
		{
			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_CREATE_SERVICE"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_CREATE_SERVICE"));
		}

		if (dwDesiredAccess | SC_MANAGER_CONNECT)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_CONNECT"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_CONNECT"));
		}

		if (dwDesiredAccess | SC_MANAGER_ENUMERATE_SERVICE)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_ENUMERATE_SERVICE"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_ENUMERATE_SERVICE"));
		}

		if (dwDesiredAccess | SC_MANAGER_LOCK)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_LOCK"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_LOCK"));
		}

		if (dwDesiredAccess | SC_MANAGER_MODIFY_BOOT_CONFIG)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_MODIFY_BOOT_CONFIG"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_MODIFY_BOOT_CONFIG"));
		}

		if (dwDesiredAccess | SC_MANAGER_ENUMERATE_SERVICE)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_QUERY_LOCK_STATUS"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_QUERY_LOCK_STATUS"));
		}
	}

	_ftprintf(logFile, _T("Opening SC Manager\n"));
	if (lpMachineName)
		_ftprintf(logFile, _T("Machine Name:  %s\n"), lpMachineName);
	_ftprintf(logFile, _T("Databse Name:  %s\n"), (lpDatabaseName) ? lpDatabaseName : L"SERVICES_ACTIVE_DATABASE");
	_ftprintf(logFile, _T("Desired Access:  %s\n"), accessRights);
	fclose(logFile);

	return pOpenSCManagerW(lpMachineName, lpDatabaseName, dwDesiredAccess);
}

SC_HANDLE WINAPI hooked_OpenServiceA(SC_HANDLE hSCManager,
										LPCSTR lpServiceName,
										DWORD dwDesiredAccess)
{
	FILE*		logFile;
	TCHAR		accessRights[256];
	int			accessRightsIndex = 0;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pOpenServiceA(hSCManager, lpServiceName, dwDesiredAccess);
	}

	ZeroMemory(accessRights, 256);

	// SC_MANAGER_ALL_ACCESS encapsulates everything.
	if (dwDesiredAccess == SC_MANAGER_ALL_ACCESS)
		_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_ALL_ACCESS"));
	else
	{
		if (dwDesiredAccess | SC_MANAGER_CREATE_SERVICE)
		{
			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_CREATE_SERVICE"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_CREATE_SERVICE"));
		}

		if (dwDesiredAccess | SC_MANAGER_CONNECT)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_CONNECT"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_CONNECT"));
		}

		if (dwDesiredAccess | SC_MANAGER_ENUMERATE_SERVICE)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_ENUMERATE_SERVICE"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_ENUMERATE_SERVICE"));
		}

		if (dwDesiredAccess | SC_MANAGER_LOCK)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_LOCK"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_LOCK"));
		}

		if (dwDesiredAccess | SC_MANAGER_MODIFY_BOOT_CONFIG)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_MODIFY_BOOT_CONFIG"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_MODIFY_BOOT_CONFIG"));
		}

		if (dwDesiredAccess | SC_MANAGER_ENUMERATE_SERVICE)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_QUERY_LOCK_STATUS"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_QUERY_LOCK_STATUS"));
		}
	}

	_ftprintf(logFile, _T("Opening Service %s\n"), lpServiceName);
	_ftprintf(logFile, _T("Desired Access:  %s\n"), accessRights);
	fclose(logFile);

	return pOpenServiceA(hSCManager, lpServiceName, dwDesiredAccess);
}

SC_HANDLE WINAPI hooked_OpenServiceW(SC_HANDLE hSCManager,
										LPCWSTR lpServiceName,
										DWORD dwDesiredAccess)
{
	FILE*		logFile;
	TCHAR		accessRights[256];
	int			accessRightsIndex = 0;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pOpenServiceW(hSCManager, lpServiceName, dwDesiredAccess);
	}

	ZeroMemory(accessRights, 256);

	// SC_MANAGER_ALL_ACCESS encapsulates everything.
	if (dwDesiredAccess == SC_MANAGER_ALL_ACCESS)
		_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_ALL_ACCESS"));
	else
	{
		if (dwDesiredAccess | SC_MANAGER_CREATE_SERVICE)
		{
			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_CREATE_SERVICE"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_CREATE_SERVICE"));
		}

		if (dwDesiredAccess | SC_MANAGER_CONNECT)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_CONNECT"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_CONNECT"));
		}

		if (dwDesiredAccess | SC_MANAGER_ENUMERATE_SERVICE)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_ENUMERATE_SERVICE"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_ENUMERATE_SERVICE"));
		}

		if (dwDesiredAccess | SC_MANAGER_LOCK)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_LOCK"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_LOCK"));
		}

		if (dwDesiredAccess | SC_MANAGER_MODIFY_BOOT_CONFIG)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_MODIFY_BOOT_CONFIG"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_MODIFY_BOOT_CONFIG"));
		}

		if (dwDesiredAccess | SC_MANAGER_ENUMERATE_SERVICE)
		{
			if (accessRightsIndex)
			{
				_tprintf(accessRights + accessRightsIndex, _T(" | "));
				accessRightsIndex += _tcslen(_T(" | "));
			}

			_tprintf(accessRights + accessRightsIndex, _T("SC_MANAGER_QUERY_LOCK_STATUS"));
			accessRightsIndex += _tcslen(_T("SC_MANAGER_QUERY_LOCK_STATUS"));
		}
	}

	_ftprintf(logFile, _T("Opening Service %s\n"), lpServiceName);
	_ftprintf(logFile, _T("Desired Access:  %s\n"), accessRights);
	fclose(logFile);

	return pOpenServiceW(hSCManager, lpServiceName, dwDesiredAccess);
}

BOOL WINAPI hooked_StartServiceA(SC_HANDLE hService,
									 DWORD dwNumServiceArgs,
									 LPCSTR* lpServiceArgVectors)
{
	FILE*		logFile;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pStartServiceA(hService, dwNumServiceArgs, lpServiceArgVectors);
	}

	_ftprintf(logFile, _T("Starting Service\n"));
	if (dwNumServiceArgs)
	{
		_ftprintf(logFile, _T("Service Name:  %s\n"), lpServiceArgVectors[0]);
		for (int i = 1; i < (signed int) dwNumServiceArgs; i++)
			_ftprintf(logFile, _T("Argument %d:  %s\n"), i, lpServiceArgVectors[i]);
	}
	fclose(logFile);

	return pStartServiceA(hService, dwNumServiceArgs, lpServiceArgVectors);
}

BOOL WINAPI hooked_StartServiceW(SC_HANDLE hService,
									 DWORD dwNumServiceArgs,
									 LPCWSTR* lpServiceArgVectors)
{
	FILE*		logFile;

	logFile = fopen("service.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open service log file\n");

		return pStartServiceW(hService, dwNumServiceArgs, lpServiceArgVectors);
	}

	_ftprintf(logFile, _T("Starting Service\n"));
	if (dwNumServiceArgs)
	{
		_ftprintf(logFile, _T("Service Name:  %s\n"), lpServiceArgVectors[0]);
		for (int i = 1; i < (signed int) dwNumServiceArgs; i++)
			_ftprintf(logFile, _T("Argument %d:  %s\n"), i, lpServiceArgVectors[i]);
	}
	fclose(logFile);

	return pStartServiceW(hService, dwNumServiceArgs, lpServiceArgVectors);
}