#ifndef DLL_H
#define DLL_H

// All functions defined in KernelBase.dll on NT 6.1.
// All LoadLibrary calls call LoadLibraryExW.
extern HMODULE (WINAPI* pLoadLibraryExW)(LPCWSTR lpFileName,
										 HANDLE hFile,
										 DWORD dwFlags);
HMODULE WINAPI hooked_LoadLibraryExW(LPCWSTR lpFileName,
										 HANDLE hFile,
										 DWORD dwFlags);
// GetModuleHandleA calls GetModuleHandleW.
// LdrLoadDll is defined in ntdll.dll.
extern FARPROC (WINAPI* pGetProcAddress)(HMODULE hModule,
										 LPCSTR lpProcName);
FARPROC WINAPI hooked_GetProcAddress(HMODULE hModule,
									 LPCSTR lpProcName);

extern BOOL (WINAPI* pFreeLibrary)(HMODULE hModule);
BOOL WINAPI hooked_FreeLibrary(HMODULE hModule);
#endif