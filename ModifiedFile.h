#ifndef MODIFIEDFILE_H
#define MODIFIEDFILE_H

// Assembly instructions defined in CreateFileWImplementation() in kernel32.dll on NT 6.1.
// WE CANNOT use fopen() or any other C/C++ library to open files because it leads to CreateFileW.
// We must declare, not define, function pointers in the header.  Otherwise, linker fails.
// CreateFileA() calls CreateFileW() ultimately.
extern HANDLE (WINAPI* pCreateFileW)(LPCTSTR lpFileName,
							 DWORD dwDesiredAccess,
							 DWORD dwShareMode,
							 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							 DWORD dwCreationDisposition,
							 DWORD dwFlagsAndAttributes,
							 HANDLE hTemplateFile);
// This function will cause problems because we are calling our hook during _tfopen().
HANDLE WINAPI hooked_CreateFileW(LPCTSTR lpFileName,
							 DWORD dwDesiredAccess,
							 DWORD dwShareMode,
							 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							 DWORD dwCreationDisposition,
							 DWORD dwFlagsAndAttributes,
							 HANDLE hTemplateFile);
// Assembly instructions defined in DeleteFileTransactedW() in kernel32.dll on NT 6.1.
// DeleteFileA() calls DeleteFileW() ultimately.
extern BOOL (WINAPI* pDeleteFileW)(LPCTSTR lpFileName);
BOOL WINAPI hooked_DeleteFileW(LPCTSTR lpFileName);
extern bool bCopyFileBeforeDelete;
#endif