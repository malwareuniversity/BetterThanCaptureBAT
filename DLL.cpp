#include "stdafx.h"

#ifndef DLL_H
#include "DLL.h"
#endif

// Extern redefinitions.
HMODULE (WINAPI* pLoadLibraryExW)(LPCWSTR lpFileName,
										 HANDLE hFile,
										 DWORD dwFlags) = LoadLibraryExW;
FARPROC (WINAPI* pGetProcAddress)(HMODULE hModule,
										 LPCSTR lpProcName) = GetProcAddress;
BOOL (WINAPI* pFreeLibrary)(HMODULE hModule) = FreeLibrary;




HMODULE WINAPI hooked_LoadLibraryExW(LPCWSTR lpFileName,
										 HANDLE hFile,
										 DWORD dwFlags)
{
	return pLoadLibraryExW(lpFileName, hFile, dwFlags);
}

FARPROC WINAPI hooked_GetProcAddress(HMODULE hModule,
									 LPCSTR lpProcName)
{
	return pGetProcAddress(hModule, lpProcName);
}

BOOL WINAPI hooked_FreeLibrary(HMODULE hModule)
{
	return pFreeLibrary(hModule);
}