/* Registry.cpp - BetterThanCaptureBAT
				*/
#include "stdafx.h"

#ifndef REGISTRY_H
#include "Registry.h"
#endif

// These functions are defined in kernel32.dll on NT 6.1.

// Extern redefinition.
// Interesting function at _RegKrnGetGlobalState()
LONG (WINAPI* pRegOpenKeyExA)(HKEY hKey,
							 LPCSTR lpSubKey,
							 DWORD ulOptions,
							 REGSAM samDesired,
							 PHKEY phkResult) = RegOpenKeyExA;
LONG (WINAPI* pRegOpenKeyExW)(HKEY hKey,
							 LPCWSTR lpSubKey,
							 DWORD ulOptions,
							 REGSAM samDesired,
							 PHKEY phkResult) = RegOpenKeyExW;
LONG (WINAPI* pRegCreateKeyExA)(HKEY hKey,
								LPCSTR lpSubKey,
								DWORD Reserved,
								LPSTR lpClass,
								DWORD dwOptions,
								REGSAM samDesired,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								PHKEY phkResult,
								LPDWORD lpdwDisposition) = RegCreateKeyExA;
LONG (WINAPI* pRegCreateKeyExW)(HKEY hKey,
								LPCWSTR lpSubKey,
								DWORD Reserved,
								LPWSTR lpClass,
								DWORD dwOptions,
								REGSAM samDesired,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								PHKEY phkResult,
								LPDWORD lpdwDisposition) = RegCreateKeyExW;
LONG (WINAPI* pRegDeleteKeyExA)(HKEY hKey,
							  LPCSTR lpSubKey,
							  REGSAM samDesired,
							  DWORD Reserved) = RegDeleteKeyExA;
LONG (WINAPI* pRegDeleteKeyExW)(HKEY hKey,
							  LPCWSTR lpSubKey,
							  REGSAM samDesired,
							  DWORD Reserved) = RegDeleteKeyExW;
LONG (WINAPI* pRegQueryValueExA)(HKEY hKey,
								LPCSTR lpValueName,
								LPDWORD lpReserved,
								LPDWORD lpType,
								LPBYTE lpData,
								LPDWORD lpcbData) = RegQueryValueExA;
LONG (WINAPI* pRegQueryValueExW)(HKEY hKey,
								LPCWSTR lpValueName,
								LPDWORD lpReserved,
								LPDWORD lpType,
								LPBYTE lpData,
								LPDWORD lpcbData) = RegQueryValueExW;
LONG (WINAPI* pRegEnumKeyExA)(HKEY hKey,
								DWORD dwIndex,
								LPSTR lpName,
								LPDWORD lpcName,
								LPDWORD lpReserved,
								LPSTR lpClass,
								LPDWORD lpcClass,
								PFILETIME lpftLastWriteTime) = RegEnumKeyExA;
LONG (WINAPI* pRegEnumKeyExW)(HKEY hKey,
								DWORD dwIndex,
								LPWSTR lpName,
								LPDWORD lpcName,
								LPDWORD lpReserved,
								LPWSTR lpClass,
								LPDWORD lpcClass,
								PFILETIME lpftLastWriteTime) = RegEnumKeyExW;
// For whatever reason this can't be resolved at load time.
LONG (WINAPI* pRegGetValueA)(HKEY hKey,
							LPCSTR lpSubKey,
							LPCSTR lpValue,
							DWORD dwFlags,
							LPDWORD pdwType,
							PVOID pvData,
							LPDWORD pcbData) = RegGetValueA;
LONG (WINAPI* pRegGetValueW)(HKEY hKey,
							LPCWSTR lpSubKey,
							LPCWSTR lpValue,
							DWORD dwFlags,
							LPDWORD pdwType,
							PVOID pvData,
							LPDWORD pcbData) = RegGetValueW;
LONG (WINAPI* pRegSetValueExA)(HKEY hKey,
							LPCSTR lpValueName,
							DWORD Reserved,
							DWORD dwType,
							const BYTE* lpData,
							DWORD cbData) = RegSetValueExA;
LONG (WINAPI* pRegSetValueExW)(HKEY hKey,
							LPCWSTR lpValueName,
							DWORD Reserved,
							DWORD dwType,
							const BYTE* lpData,
							DWORD cbData) = RegSetValueExW;

LONG WINAPI hooked_RegOpenKeyExA(HKEY hKey,
							 LPCSTR lpSubKey,
							 DWORD ulOptions,
							 REGSAM samDesired,
							 PHKEY phkResult)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR	samString[256];
	int		samStringIndex = 0;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegOpenKeyExA(hKey,
							 lpSubKey,
							 ulOptions,
							 samDesired,
							 phkResult);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	ZeroMemory(samString, 256);

	if ((unsigned int) samDesired == KEY_ALL_ACCESS)
	{
		_stprintf(samString, _T("%s"), _T("KEY_ALL_ACCESS"));
		samStringIndex += _tcslen(_T("KEY_ALL_ACCESS"));
	}

	else
	{
		// This is rough.
		// KEY_WRITE contains KEY_CREATE_SUB_KEY.
		if ((unsigned int) samDesired | KEY_CREATE_SUB_KEY && !((unsigned int) samDesired | KEY_WRITE))
		{
			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_CREATE_SUB_KEY"));
			samStringIndex += _tcslen(_T("KEY_CREATE_SUB_KEY"));
		}

		if ((unsigned int) samDesired | KEY_ENUMERATE_SUB_KEYS)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_ENUMERATE_SUB_KEYS"));
			samStringIndex += _tcslen(_T("KEY_ENUMERATE_SUB_KEYS"));
		}

		// Same as KEY_READ.
		if ((unsigned int) samDesired | KEY_EXECUTE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_EXECUTE/READ"));
			samStringIndex += _tcslen(_T("KEY_EXECUTE/READ"));
		}

		if ((unsigned int) samDesired | KEY_NOTIFY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_NOTIFY"));
			samStringIndex += _tcslen(_T("KEY_NOTIFY"));
		}

		if ((unsigned int) samDesired | KEY_QUERY_VALUE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_QUERY_VALUE"));
			samStringIndex += _tcslen(_T("KEY_QUERY_VALUE"));
		}

		// KEY_WRITE contains KEY_SET_VALUE.
		if ((unsigned int) samDesired | KEY_SET_VALUE && !((unsigned int) samDesired | KEY_WRITE))
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_SET_VALUE"));
			samStringIndex += _tcslen(_T("KEY_SET_VALUE"));
		}

		if ((unsigned int) samDesired | KEY_WOW64_32KEY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WOW64_32KEY"));
			samStringIndex += _tcslen(_T("KEY_WOW64_32KEY"));
		}

		if ((unsigned int) samDesired | KEY_WOW64_64KEY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WOW64_64KEY"));
			samStringIndex += _tcslen(_T("KEY_WOW64_64KEY"));
		}

		if ((unsigned int) samDesired | KEY_WRITE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WRITE (STANDARD_RIGHTS_RIGHT | KEY_SET_VALUE | KEY_CREATE_SUB_KEY)"));
			samStringIndex += _tcslen(_T("KEY_WRITE (STANDARD_RIGHTS_RIGHT | KEY_SET_VALUE | KEY_CREATE_SUB_KEY)"));
		}
	}

	_ftprintf(logFile,	_T("Opening registry key:\n")
						_T("HKEY:		%s\n")
						_T("SubKey:		%s\n")
						_T("Access:		%s\n"), (predefinedKey) ? predefinedKey : _T("Custom"), lpSubKey, samString);

	// No need to free predefinedKey, stack variable.
	fclose(logFile);

	return pRegOpenKeyExA(hKey,
							 lpSubKey,
							 ulOptions,
							 samDesired,
							 phkResult);
}

LONG WINAPI hooked_RegOpenKeyExW(HKEY hKey,
							 LPCWSTR lpSubKey,
							 DWORD ulOptions,
							 REGSAM samDesired,
							 PHKEY phkResult)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR	samString[256];
	int		samStringIndex = 0;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegOpenKeyExW(hKey,
							 lpSubKey,
							 ulOptions,
							 samDesired,
							 phkResult);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	ZeroMemory(samString, 256);

	if ((unsigned int) samDesired == KEY_ALL_ACCESS)
	{
		_stprintf(samString, _T("%s"), _T("KEY_ALL_ACCESS"));
		samStringIndex += _tcslen(_T("KEY_ALL_ACCESS"));
	}

	else
	{
		// This is rough.
		// KEY_WRITE contains KEY_CREATE_SUB_KEY.
		if ((unsigned int) samDesired | KEY_CREATE_SUB_KEY && !((unsigned int) samDesired | KEY_WRITE))
		{
			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_CREATE_SUB_KEY"));
			samStringIndex += _tcslen(_T("KEY_CREATE_SUB_KEY"));
		}

		if ((unsigned int) samDesired | KEY_ENUMERATE_SUB_KEYS)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_ENUMERATE_SUB_KEYS"));
			samStringIndex += _tcslen(_T("KEY_ENUMERATE_SUB_KEYS"));
		}

		// Same as KEY_READ.
		if ((unsigned int) samDesired | KEY_EXECUTE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_EXECUTE/READ"));
			samStringIndex += _tcslen(_T("KEY_EXECUTE/READ"));
		}

		if ((unsigned int) samDesired | KEY_NOTIFY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_NOTIFY"));
			samStringIndex += _tcslen(_T("KEY_NOTIFY"));
		}

		if ((unsigned int) samDesired | KEY_QUERY_VALUE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_QUERY_VALUE"));
			samStringIndex += _tcslen(_T("KEY_QUERY_VALUE"));
		}

		// KEY_WRITE contains KEY_SET_VALUE.
		if ((unsigned int) samDesired | KEY_SET_VALUE && !((unsigned int) samDesired | KEY_WRITE))
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_SET_VALUE"));
			samStringIndex += _tcslen(_T("KEY_SET_VALUE"));
		}

		if ((unsigned int) samDesired | KEY_WOW64_32KEY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WOW64_32KEY"));
			samStringIndex += _tcslen(_T("KEY_WOW64_32KEY"));
		}

		if ((unsigned int) samDesired | KEY_WOW64_64KEY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WOW64_64KEY"));
			samStringIndex += _tcslen(_T("KEY_WOW64_64KEY"));
		}

		if ((unsigned int) samDesired | KEY_WRITE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WRITE (STANDARD_RIGHTS_RIGHT | KEY_SET_VALUE | KEY_CREATE_SUB_KEY)"));
			samStringIndex += _tcslen(_T("KEY_WRITE (STANDARD_RIGHTS_RIGHT | KEY_SET_VALUE | KEY_CREATE_SUB_KEY)"));
		}
	}

	_ftprintf(logFile,	_T("Opening registry key:\n")
						_T("HKEY:		%s\n")
						_T("SubKey:		%s\n")
						_T("Access:		%s\n"), (predefinedKey) ? predefinedKey : _T("Custom"), lpSubKey, samString);

	// No need to free predefinedKey, stack variable.
	fclose(logFile);

	return pRegOpenKeyExW(hKey,
							 lpSubKey,
							 ulOptions,
							 samDesired,
							 phkResult);
}

LONG WINAPI hooked_RegCreateKeyExA(HKEY hKey,
								LPCSTR lpSubKey,
								DWORD Reserved,
								LPSTR lpClass,
								DWORD dwOptions,
								REGSAM samDesired,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								PHKEY phkResult,
								LPDWORD lpdwDisposition)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR	optionsString[256];
	int		optionsStringIndex = 0;
	TCHAR	samString[256];
	int		samStringIndex = 0;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegCreateKeyExA(hKey,
							 lpSubKey,
							 Reserved,
							 lpClass,
							 dwOptions,
							 samDesired,
							 lpSecurityAttributes,
							 phkResult,
							 lpdwDisposition);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	ZeroMemory(optionsString, 256);

	if (dwOptions | REG_OPTION_BACKUP_RESTORE)
	{
		_stprintf(optionsString, _T("%s"), _T("REG_OPTION_BACKUP_RESTORE"));
		optionsStringIndex += _tcslen(_T("REG_OPTION_BACKUP_RESTORE"));
	}

	if (dwOptions | REG_OPTION_CREATE_LINK)
	{
		if (optionsStringIndex)
		{
			_stprintf(optionsString + optionsStringIndex, _T(" | "));
			optionsStringIndex += _tcslen(_T(" | "));
		}
		_stprintf(optionsString + optionsStringIndex, _T("%s"), _T("REG_OPTION_CREATE_LINK"));
		optionsStringIndex += _tcslen(_T("REG_OPTION_CREATE_LINK"));
	}

	if (dwOptions | REG_OPTION_NON_VOLATILE)
	{
		if (optionsStringIndex)
		{
			_stprintf(optionsString + optionsStringIndex, _T(" | "));
			optionsStringIndex += _tcslen(_T(" | "));
		}
		_stprintf(optionsString + optionsStringIndex, _T("%s"), _T("REG_OPTION_BACKUP_RESTORE"));
		optionsStringIndex += _tcslen(_T("REG_OPTION_BACKUP_RESTORE"));
	}

	if (dwOptions | REG_OPTION_VOLATILE)
	{
		if (optionsStringIndex)
		{
			_stprintf(optionsString + optionsStringIndex, _T(" | "));
			optionsStringIndex += _tcslen(_T(" | "));
		}
		_stprintf(optionsString + optionsStringIndex, _T("%s"), _T("REG_OPTION_BACKUP_RESTORE"));
		optionsStringIndex += _tcslen(_T("REG_OPTION_BACKUP_RESTORE"));
	}

	ZeroMemory(samString, 256);

	if ((unsigned int) samDesired == KEY_ALL_ACCESS)
	{
		_stprintf(samString, _T("%s"), _T("KEY_ALL_ACCESS"));
		samStringIndex += _tcslen(_T("KEY_ALL_ACCESS"));
	}

	else
	{
		// This is rough.
		// KEY_WRITE contains KEY_CREATE_SUB_KEY.
		if ((unsigned int) samDesired | KEY_CREATE_SUB_KEY && !((unsigned int) samDesired | KEY_WRITE))
		{
			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_CREATE_SUB_KEY"));
			samStringIndex += _tcslen(_T("KEY_CREATE_SUB_KEY"));
		}

		if ((unsigned int) samDesired | KEY_ENUMERATE_SUB_KEYS)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_ENUMERATE_SUB_KEYS"));
			samStringIndex += _tcslen(_T("KEY_ENUMERATE_SUB_KEYS"));
		}

		// Same as KEY_READ.
		if ((unsigned int) samDesired | KEY_EXECUTE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_EXECUTE/READ"));
			samStringIndex += _tcslen(_T("KEY_EXECUTE/READ"));
		}

		if ((unsigned int) samDesired | KEY_NOTIFY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_NOTIFY"));
			samStringIndex += _tcslen(_T("KEY_NOTIFY"));
		}

		if ((unsigned int) samDesired | KEY_QUERY_VALUE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_QUERY_VALUE"));
			samStringIndex += _tcslen(_T("KEY_QUERY_VALUE"));
		}

		// KEY_WRITE contains KEY_SET_VALUE.
		if ((unsigned int) samDesired | KEY_SET_VALUE && !((unsigned int) samDesired | KEY_WRITE))
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_SET_VALUE"));
			samStringIndex += _tcslen(_T("KEY_SET_VALUE"));
		}

		if ((unsigned int) samDesired | KEY_WOW64_32KEY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WOW64_32KEY"));
			samStringIndex += _tcslen(_T("KEY_WOW64_32KEY"));
		}

		if ((unsigned int) samDesired | KEY_WOW64_64KEY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WOW64_64KEY"));
			samStringIndex += _tcslen(_T("KEY_WOW64_64KEY"));
		}

		if ((unsigned int) samDesired | KEY_WRITE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WRITE (STANDARD_RIGHTS_RIGHT | KEY_SET_VALUE | KEY_CREATE_SUB_KEY)"));
			samStringIndex += _tcslen(_T("KEY_WRITE (STANDARD_RIGHTS_RIGHT | KEY_SET_VALUE | KEY_CREATE_SUB_KEY)"));
		}
	}

	_ftprintf(logFile,	_T("Opening registry key:\n")
						_T("HKEY:		%s\n")
						_T("SubKey:		%s\n")
						_T("Access:		%s\n"), (predefinedKey) ? predefinedKey : _T("Custom"), lpSubKey, samString);

	// No need to free predefinedKey, stack variable.
	fclose(logFile);

	return pRegCreateKeyExA(hKey,
							 lpSubKey,
							 Reserved,
							 lpClass,
							 dwOptions,
							 samDesired,
							 lpSecurityAttributes,
							 phkResult,
							 lpdwDisposition);
}

LONG WINAPI hooked_RegCreateKeyExW(HKEY hKey,
								LPCWSTR lpSubKey,
								DWORD Reserved,
								LPWSTR lpClass,
								DWORD dwOptions,
								REGSAM samDesired,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								PHKEY phkResult,
								LPDWORD lpdwDisposition)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR	optionsString[256];
	int		optionsStringIndex = 0;
	TCHAR	samString[256];
	int		samStringIndex = 0;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegCreateKeyExW(hKey,
							 lpSubKey,
							 Reserved,
							 lpClass,
							 dwOptions,
							 samDesired,
							 lpSecurityAttributes,
							 phkResult,
							 lpdwDisposition);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	ZeroMemory(optionsString, 256);

	if (dwOptions | REG_OPTION_BACKUP_RESTORE)
	{
		_stprintf(optionsString, _T("%s"), _T("REG_OPTION_BACKUP_RESTORE"));
		optionsStringIndex += _tcslen(_T("REG_OPTION_BACKUP_RESTORE"));
	}

	if (dwOptions | REG_OPTION_CREATE_LINK)
	{
		if (optionsStringIndex)
		{
			_stprintf(optionsString + optionsStringIndex, _T(" | "));
			optionsStringIndex += _tcslen(_T(" | "));
		}
		_stprintf(optionsString + optionsStringIndex, _T("%s"), _T("REG_OPTION_CREATE_LINK"));
		optionsStringIndex += _tcslen(_T("REG_OPTION_CREATE_LINK"));
	}

	if (dwOptions | REG_OPTION_NON_VOLATILE)
	{
		if (optionsStringIndex)
		{
			_stprintf(optionsString + optionsStringIndex, _T(" | "));
			optionsStringIndex += _tcslen(_T(" | "));
		}
		_stprintf(optionsString + optionsStringIndex, _T("%s"), _T("REG_OPTION_BACKUP_RESTORE"));
		optionsStringIndex += _tcslen(_T("REG_OPTION_BACKUP_RESTORE"));
	}

	if (dwOptions | REG_OPTION_VOLATILE)
	{
		if (optionsStringIndex)
		{
			_stprintf(optionsString + optionsStringIndex, _T(" | "));
			optionsStringIndex += _tcslen(_T(" | "));
		}
		_stprintf(optionsString + optionsStringIndex, _T("%s"), _T("REG_OPTION_BACKUP_RESTORE"));
		optionsStringIndex += _tcslen(_T("REG_OPTION_BACKUP_RESTORE"));
	}

	ZeroMemory(samString, 256);

	if ((unsigned int) samDesired == KEY_ALL_ACCESS)
	{
		_stprintf(samString, _T("%s"), _T("KEY_ALL_ACCESS"));
		samStringIndex += _tcslen(_T("KEY_ALL_ACCESS"));
	}

	else
	{
		// This is rough.
		// KEY_WRITE contains KEY_CREATE_SUB_KEY.
		if ((unsigned int) samDesired | KEY_CREATE_SUB_KEY && !((unsigned int) samDesired | KEY_WRITE))
		{
			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_CREATE_SUB_KEY"));
			samStringIndex += _tcslen(_T("KEY_CREATE_SUB_KEY"));
		}

		if ((unsigned int) samDesired | KEY_ENUMERATE_SUB_KEYS)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_ENUMERATE_SUB_KEYS"));
			samStringIndex += _tcslen(_T("KEY_ENUMERATE_SUB_KEYS"));
		}

		// Same as KEY_READ.
		if ((unsigned int) samDesired | KEY_EXECUTE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_EXECUTE/READ"));
			samStringIndex += _tcslen(_T("KEY_EXECUTE/READ"));
		}

		if ((unsigned int) samDesired | KEY_NOTIFY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_NOTIFY"));
			samStringIndex += _tcslen(_T("KEY_NOTIFY"));
		}

		if ((unsigned int) samDesired | KEY_QUERY_VALUE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_QUERY_VALUE"));
			samStringIndex += _tcslen(_T("KEY_QUERY_VALUE"));
		}

		// KEY_WRITE contains KEY_SET_VALUE.
		if ((unsigned int) samDesired | KEY_SET_VALUE && !((unsigned int) samDesired | KEY_WRITE))
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_SET_VALUE"));
			samStringIndex += _tcslen(_T("KEY_SET_VALUE"));
		}

		if ((unsigned int) samDesired | KEY_WOW64_32KEY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WOW64_32KEY"));
			samStringIndex += _tcslen(_T("KEY_WOW64_32KEY"));
		}

		if ((unsigned int) samDesired | KEY_WOW64_64KEY)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WOW64_64KEY"));
			samStringIndex += _tcslen(_T("KEY_WOW64_64KEY"));
		}

		if ((unsigned int) samDesired | KEY_WRITE)
		{
			// Check if the buffer contains any data and add a separator.
			if (samStringIndex)
			{
				_stprintf(samString + samStringIndex, _T(" | "));
				samStringIndex += _tcslen(_T(" | "));
			}

			_stprintf(samString + samStringIndex, _T("%s"), _T("KEY_WRITE (STANDARD_RIGHTS_RIGHT | KEY_SET_VALUE | KEY_CREATE_SUB_KEY)"));
			samStringIndex += _tcslen(_T("KEY_WRITE (STANDARD_RIGHTS_RIGHT | KEY_SET_VALUE | KEY_CREATE_SUB_KEY)"));
		}
	}

	_ftprintf(logFile,	_T("Opening registry key:\n")
						_T("HKEY:		%s\n")
						_T("SubKey:		%s\n")
						_T("Access:		%s\n"), (predefinedKey) ? predefinedKey : _T("Custom"), lpSubKey, samString);

	// No need to free predefinedKey, stack variable.
	fclose(logFile);

	return pRegCreateKeyExW(hKey,
							 lpSubKey,
							 Reserved,
							 lpClass,
							 dwOptions,
							 samDesired,
							 lpSecurityAttributes,
							 phkResult,
							 lpdwDisposition);
}

LONG WINAPI hooked_RegDeleteKeyExA(HKEY hKey,
							  LPCSTR lpSubKey,
							  REGSAM samDesired,
							  DWORD Reserved)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR*	samDesiredString;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegDeleteKeyExA(hKey,
								lpSubKey,
								samDesired,
								Reserved);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	switch(samDesired)
	{
	case KEY_WOW64_32KEY:
		samDesiredString = _T("KEY_WOW64_32KEY");
		break;
		
	case KEY_WOW64_64KEY:
		samDesiredString = _T("KEY_WOW64_64KEY");
		break;
	}

	_ftprintf(logFile,	_T("Deleting registry key:\n")
						_T("HKEY:			%s\n")
						_T("SubKey:			%s\n")
						_T("samDesired:		0x%04x\n"), (predefinedKey) ? predefinedKey : _T("Custom"), lpSubKey, (samDesired) ? samDesiredString : _T("None"));
	fclose(logFile);

	return pRegDeleteKeyExA(hKey,
								lpSubKey,
								samDesired,
								Reserved);
}

LONG WINAPI hooked_RegDeleteKeyExW(HKEY hKey,
							  LPCWSTR lpSubKey,
							  REGSAM samDesired,
							  DWORD Reserved)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR*	samDesiredString;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegDeleteKeyExW(hKey,
								lpSubKey,
								samDesired,
								Reserved);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	switch(samDesired)
	{
	case KEY_WOW64_32KEY:
		samDesiredString = _T("KEY_WOW64_32KEY");
		break;
		
	case KEY_WOW64_64KEY:
		samDesiredString = _T("KEY_WOW64_64KEY");
		break;
	}

	_ftprintf(logFile,	_T("Deleting registry key:\n")
						_T("HKEY:			%s\n")
						_T("SubKey:			%s\n")
						_T("samDesired:		0x%04x\n"), (predefinedKey) ? predefinedKey : _T("Custom"), lpSubKey, (samDesired) ? samDesiredString : _T("None"));
	fclose(logFile);

	return pRegDeleteKeyExW(hKey,
								lpSubKey,
								samDesired,
								Reserved);
}

LONG WINAPI hooked_RegQueryValueExA(HKEY hKey,
								LPCSTR lpValueName,
								LPDWORD lpReserved,
								LPDWORD lpType,
								LPBYTE lpData,
								LPDWORD lpcbData)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR*	typeString;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegQueryValueExA(hKey,
								lpValueName,
								lpReserved,
								lpType,
								lpData,
								lpcbData);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_PERFORMANCE_DATA:
		predefinedKey = _T("HKEY_PERFORMANCE_DATA");
		break;

	case HKEY_PERFORMANCE_NLSTEXT:
		predefinedKey = _T("HKEY_PERFORMANCE_NLSTEXT");
		break;

	case HKEY_PERFORMANCE_TEXT:
		predefinedKey = _T("HKEY_PERFORMANCE_TEXT");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	switch((unsigned int) lpType)
	{
	case REG_BINARY:
		typeString = _T("REG_BINARY");
		break;

	case REG_DWORD:
		typeString = _T("REG_DWORD");
		break;

	case REG_EXPAND_SZ:
		typeString = _T("REG_EXPAND_SZ");
		break;

	case REG_LINK:
		typeString = _T("REG_LINK");
		break;

	case REG_MULTI_SZ:
		typeString = _T("REG_MULTI_SZ");
		break;

	case REG_NONE:
		typeString = _T("REG_NONE");
		break;

	case REG_QWORD:
		typeString = _T("REG_QWORD");
		break;

	case REG_SZ:
		typeString = _T("REG_SZ");
		break;
	}

	_ftprintf(logFile,	_T("Deleting registry key:\n")
						_T("HKEY:			%s\n")
						_T("ValueName:		%s\n")
						_T("Reserved:		0\n")
						_T("Type:			0x%08x\n")
						_T("Data:			0x%08x\n")
						_T("lpcbData		0x%08x\n"), (predefinedKey) ? predefinedKey : _T("Custom"), 
														lpValueName, 
														(lpType) ? lpType : 0,
														(lpData) ? lpData : 0,
														(lpcbData) ? lpcbData : 0);
	fclose(logFile);

	return pRegQueryValueExA(hKey,
								lpValueName,
								lpReserved,
								lpType,
								lpData,
								lpcbData);
}

LONG WINAPI hooked_RegQueryValueExW(HKEY hKey,
								LPCWSTR lpValueName,
								LPDWORD lpReserved,
								LPDWORD lpType,
								LPBYTE lpData,
								LPDWORD lpcbData)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR*	typeString;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegQueryValueExW(hKey,
								lpValueName,
								lpReserved,
								lpType,
								lpData,
								lpcbData);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_PERFORMANCE_DATA:
		predefinedKey = _T("HKEY_PERFORMANCE_DATA");
		break;

	case HKEY_PERFORMANCE_NLSTEXT:
		predefinedKey = _T("HKEY_PERFORMANCE_NLSTEXT");
		break;

	case HKEY_PERFORMANCE_TEXT:
		predefinedKey = _T("HKEY_PERFORMANCE_TEXT");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	switch((unsigned int) lpType)
	{
	case REG_BINARY:
		typeString = _T("REG_BINARY");
		break;

	case REG_DWORD:
		typeString = _T("REG_DWORD");
		break;

	case REG_EXPAND_SZ:
		typeString = _T("REG_EXPAND_SZ");
		break;

	case REG_LINK:
		typeString = _T("REG_LINK");
		break;

	case REG_MULTI_SZ:
		typeString = _T("REG_MULTI_SZ");
		break;

	case REG_NONE:
		typeString = _T("REG_NONE");
		break;

	case REG_QWORD:
		typeString = _T("REG_QWORD");
		break;

	case REG_SZ:
		typeString = _T("REG_SZ");
		break;
	}

	_ftprintf(logFile,	_T("Deleting registry key:\n")
						_T("HKEY:			%s\n")
						_T("ValueName:		%s\n")
						_T("Reserved:		0\n")
						_T("Type:			0x%08x\n")
						_T("Data:			0x%08x\n")
						_T("lpcbData		0x%08x\n"), (predefinedKey) ? predefinedKey : _T("Custom"), 
														lpValueName, 
														(lpType) ? lpType : 0,
														(lpData) ? lpData : 0,
														(lpcbData) ? lpcbData : 0);
	fclose(logFile);

	return pRegQueryValueExW(hKey,
								lpValueName,
								lpReserved,
								lpType,
								lpData,
								lpcbData);
}

LONG WINAPI hooked_RegEnumKeyExA(HKEY hKey,
								DWORD dwIndex,
								LPSTR lpName,
								LPDWORD lpcName,
								LPDWORD lpReserved,
								LPSTR lpClass,
								LPDWORD lpcClass,
								PFILETIME lpftLastWriteTime)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegEnumKeyExA(hKey,
								dwIndex,
								lpName,
								lpcName,
								lpReserved,
								lpClass,
								lpcClass,
								lpftLastWriteTime);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_PERFORMANCE_DATA:
		predefinedKey = _T("HKEY_PERFORMANCE_DATA");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	_ftprintf(logFile,	_T("Enumerating registry key:\n")
						_T("HKEY:			%s\n")
						_T("Index:			0x%08x\n"), (predefinedKey) ? predefinedKey : _T("Custom"), 
														dwIndex);
	fclose(logFile);

	return pRegEnumKeyExA(hKey,
								dwIndex,
								lpName,
								lpcName,
								lpReserved,
								lpClass,
								lpcClass,
								lpftLastWriteTime);
}

LONG WINAPI hooked_RegEnumKeyExW(HKEY hKey,
								DWORD dwIndex,
								LPWSTR lpName,
								LPDWORD lpcName,
								LPDWORD lpReserved,
								LPWSTR lpClass,
								LPDWORD lpcClass,
								PFILETIME lpftLastWriteTime)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegEnumKeyExW(hKey,
								dwIndex,
								lpName,
								lpcName,
								lpReserved,
								lpClass,
								lpcClass,
								lpftLastWriteTime);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_PERFORMANCE_DATA:
		predefinedKey = _T("HKEY_PERFORMANCE_DATA");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	_ftprintf(logFile,	_T("Enumerating registry key:\n")
						_T("HKEY:			%s\n")
						_T("Index:			0x%08x\n"), (predefinedKey) ? predefinedKey : _T("Custom"), 
														dwIndex);
	fclose(logFile);

	return pRegEnumKeyExW(hKey,
								dwIndex,
								lpName,
								lpcName,
								lpReserved,
								lpClass,
								lpcClass,
								lpftLastWriteTime);
}

LONG WINAPI hooked_RegGetValueA(HKEY hKey,
							LPCSTR lpSubKey,
							LPCSTR lpValue,
							DWORD dwFlags,
							LPDWORD pdwType,
							PVOID pvData,
							LPDWORD pcbData)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR*	typeString;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegGetValueA(hKey,
							lpSubKey,
							lpValue,
							dwFlags,
							pdwType,
							pvData,
							pcbData);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_PERFORMANCE_DATA:
		predefinedKey = _T("HKEY_PERFORMANCE_DATA");
		break;

	case HKEY_PERFORMANCE_NLSTEXT:
		predefinedKey = _T("HKEY_PERFORMANCE_NLSTEXT");
		break;

	case HKEY_PERFORMANCE_TEXT:
		predefinedKey = _T("HKEY_PERFORMANCE_TEXT");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	_ftprintf(logFile,	_T("Retrieving registry key:\n")
						_T("HKEY:			%s\n")
						_T("SubKey:			%s\n")
						_T("Value:			%s\n"),
						_T("Flags:			0x%08x\n"), (predefinedKey) ? predefinedKey : _T("Custom"), 
													(lpSubKey) ? lpSubKey : "None", 
													(lpValue) ? lpValue : "None",
													dwFlags);
	fclose(logFile);

	return pRegGetValueA(hKey,
							lpSubKey,
							lpValue,
							dwFlags,
							pdwType,
							pvData,
							pcbData);
}

LONG WINAPI hooked_RegGetValueW(HKEY hKey,
							LPCWSTR lpSubKey,
							LPCWSTR lpValue,
							DWORD dwFlags,
							LPDWORD pdwType,
							PVOID pvData,
							LPDWORD pcbData)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR*	typeString;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegGetValueW(hKey,
							lpSubKey,
							lpValue,
							dwFlags,
							pdwType,
							pvData,
							pcbData);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_PERFORMANCE_DATA:
		predefinedKey = _T("HKEY_PERFORMANCE_DATA");
		break;

	case HKEY_PERFORMANCE_NLSTEXT:
		predefinedKey = _T("HKEY_PERFORMANCE_NLSTEXT");
		break;

	case HKEY_PERFORMANCE_TEXT:
		predefinedKey = _T("HKEY_PERFORMANCE_TEXT");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	_ftprintf(logFile,	_T("Retrieving registry key:\n")
						_T("HKEY:			%s\n")
						_T("SubKey:			%s\n")
						_T("Value:			%s\n"),
						_T("Flags:			0x%08x\n"), (predefinedKey) ? predefinedKey : _T("Custom"), 
													(lpSubKey) ? lpSubKey : _T("None"), 
													(lpValue) ? lpValue : _T("None"),
													dwFlags);
	fclose(logFile);

	return pRegGetValueW(hKey,
							lpSubKey,
							lpValue,
							dwFlags,
							pdwType,
							pvData,
							pcbData);
}

LONG WINAPI hooked_RegSetValueExA(HKEY hKey,
							LPCSTR lpValueName,
							DWORD Reserved,
							DWORD dwType,
							const BYTE* lpData,
							DWORD cbData)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR*	typeString;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegSetValueExA(hKey,
							lpValueName,
							Reserved,
							dwType,
							lpData,
							cbData);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	switch((unsigned int) dwType)
	{
	case REG_BINARY:
		typeString = _T("REG_BINARY");
		break;

	case REG_DWORD:
		typeString = _T("REG_DWORD");
		break;

	case REG_EXPAND_SZ:
		typeString = _T("REG_EXPAND_SZ");
		break;

	case REG_LINK:
		typeString = _T("REG_LINK");
		break;

	case REG_MULTI_SZ:
		typeString = _T("REG_MULTI_SZ");
		break;

	case REG_NONE:
		typeString = _T("REG_NONE");
		break;

	case REG_QWORD:
		typeString = _T("REG_QWORD");
		break;

	case REG_SZ:
		typeString = _T("REG_SZ");
		break;
	}

	_ftprintf(logFile,	_T("Setting registry value:\n")
						_T("HKEY:			%s\n")
						_T("ValueName:		%s\n")
						_T("Type:			%s\n"),
						_T("Data:			"), (predefinedKey) ? predefinedKey : _T("Custom"), 
													(lpValueName) ? lpValueName : "None", 
													(dwType) ? typeString : _T("None"));

	if ((unsigned int) dwType != REG_EXPAND_SZ && (unsigned int) dwType != REG_SZ)
	{
		for (int i = 0; i < cbData; i++)
			_ftprintf(logFile, _T("0x%02x "), lpData[i]);
		_ftprintf(logFile, _T("\n"));
	}
	else
		_ftprintf(logFile, _T("%s\n"), lpData);

	fclose(logFile);

	return pRegSetValueExA(hKey,
							lpValueName,
							Reserved,
							dwType,
							lpData,
							cbData);
}

LONG WINAPI hooked_RegSetValueExW(HKEY hKey,
							LPCWSTR lpValueName,
							DWORD Reserved,
							DWORD dwType,
							const BYTE* lpData,
							DWORD cbData)
{
	FILE*	logFile;
	TCHAR*	predefinedKey;
	TCHAR*	typeString;

	logFile = fopen("registry.txt", "a");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open registry.txt\n");

		return pRegSetValueExW(hKey,
							lpValueName,
							Reserved,
							dwType,
							lpData,
							cbData);
	}

	switch ((unsigned int) hKey)
	{
	case HKEY_CLASSES_ROOT:
		predefinedKey = _T("HKEY_CLASSES_ROOT");
		break;

	case HKEY_CURRENT_CONFIG:
		predefinedKey = _T("HKEY_CURRENT_CONFIG");
		break;

	case HKEY_CURRENT_USER:
		predefinedKey = _T("HKEY_CURRENT_USER");
		break;

	case HKEY_LOCAL_MACHINE:
		predefinedKey = _T("HKEY_LOCAL_MACHINE");
		break;

	case HKEY_USERS:
		predefinedKey = _T("HKEY_USERS");
		break;
	}

	switch((unsigned int) dwType)
	{
	case REG_BINARY:
		typeString = _T("REG_BINARY");
		break;

	case REG_DWORD:
		typeString = _T("REG_DWORD");
		break;

	case REG_EXPAND_SZ:
		typeString = _T("REG_EXPAND_SZ");
		break;

	case REG_LINK:
		typeString = _T("REG_LINK");
		break;

	case REG_MULTI_SZ:
		typeString = _T("REG_MULTI_SZ");
		break;

	case REG_NONE:
		typeString = _T("REG_NONE");
		break;

	case REG_QWORD:
		typeString = _T("REG_QWORD");
		break;

	case REG_SZ:
		typeString = _T("REG_SZ");
		break;
	}

	_ftprintf(logFile,	_T("Setting registry value:\n")
						_T("HKEY:			%s\n")
						_T("ValueName:		%s\n")
						_T("Type:			%s\n"),
						_T("Data:			"), (predefinedKey) ? predefinedKey : _T("Custom"), 
													(lpValueName) ? lpValueName : _T("None"), 
													(dwType) ? typeString : _T("None"));

	if ((unsigned int) dwType != REG_EXPAND_SZ && (unsigned int) dwType != REG_SZ)
	{
		for (int i = 0; i < cbData; i++)
			_ftprintf(logFile, _T("0x%02x "), lpData[i]);
		_ftprintf(logFile, _T("\n"));
	}
	else
		_ftprintf(logFile, _T("%s\n"), lpData);

	fclose(logFile);

	return pRegSetValueExW(hKey,
							lpValueName,
							Reserved,
							dwType,
							lpData,
							cbData);
}