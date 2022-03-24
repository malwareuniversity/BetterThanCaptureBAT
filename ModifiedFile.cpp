/* ModifiedFile.cpp - BetterThanCaptureBAT
					
					Hooks CreateFileW and DeleteFileW.
					No need to hook WriteFile, because are only concerned with the end result.
					*/

#include "stdafx.h"

#ifndef MODIFIEDFILE_H
#include "ModifiedFile.h"
#endif

// Extern redefinitions.
bool bCopyFileBeforeDelete = true;
HANDLE (WINAPI* pCreateFileW)(LPCTSTR lpFileName,
							 DWORD dwDesiredAccess,
							 DWORD dwShareMode,
							 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							 DWORD dwCreationDisposition,
							 DWORD dwFlagsAndAttributes,
							 HANDLE hTemplateFile) = CreateFileW;
BOOL (WINAPI* pDeleteFileW)(LPCTSTR lpFileName) = DeleteFileW;



// This function will cause problems because we are calling our hook during _tfopen().
HANDLE WINAPI hooked_CreateFileW(LPCTSTR lpFileName,
							 DWORD dwDesiredAccess,
							 DWORD dwShareMode,
							 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							 DWORD dwCreationDisposition,
							 DWORD dwFlagsAndAttributes,
							 HANDLE hTemplateFile)
{
	HANDLE	hLogFile;
	HANDLE	retVal;
	DWORD	numBytesWritten;
	char*	ansiFileName;
	char*	sLogFile;

	hLogFile = pCreateFileW(L"create.txt",
							GENERIC_WRITE,
							FILE_SHARE_READ,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if (!hLogFile)
	{
		fprintf(stderr, "Unable to open create.txt in write mode\n");
		return pCreateFileW(lpFileName,
							dwDesiredAccess,
							dwShareMode,
							lpSecurityAttributes,
							dwCreationDisposition,
							dwFlagsAndAttributes,
							hTemplateFile);
	}
	
	// Check to see if the program wants to modify or create a file.
	if (!(dwDesiredAccess | GENERIC_WRITE) || !(dwDesiredAccess | GENERIC_ALL))
		return pCreateFileW(lpFileName,
							dwDesiredAccess,
							dwShareMode,
							lpSecurityAttributes,
							dwCreationDisposition,
							dwFlagsAndAttributes,
							hTemplateFile);

	sLogFile = (char* ) calloc((_tcslen(lpFileName) * sizeof(TCHAR)) + strlen("Opening file   with permissions   ") + strlen("GENERIC_WRITE"), sizeof(char));
	if (!sLogFile)
	{
		fprintf(stderr, "Unable to allocate memory for writing to file\n");
		CloseHandle(hLogFile);

		return pCreateFileW(lpFileName,
							dwDesiredAccess,
							dwShareMode,
							lpSecurityAttributes,
							dwCreationDisposition,
							dwFlagsAndAttributes,
							hTemplateFile);
	}

	ansiFileName = (char* ) malloc(_tcslen(lpFileName) * sizeof(TCHAR));
	if (!ansiFileName)
	{
		fprintf(stderr, "Unable to allocate memory for copying file name\n");
		free(sLogFile);
		CloseHandle(hLogFile);

		return pCreateFileW(lpFileName,
							dwDesiredAccess,
							dwShareMode,
							lpSecurityAttributes,
							dwCreationDisposition,
							dwFlagsAndAttributes,
							hTemplateFile);
	}

	ZeroMemory(ansiFileName, _tcslen(lpFileName) * sizeof(TCHAR));

	if (!WideCharToMultiByte(CP_ACP,
							0,
							lpFileName,
							_tcslen(lpFileName),
							ansiFileName,
							_tcslen(lpFileName) * sizeof(TCHAR),
							NULL,
							NULL))
	{
		fprintf(stderr, "Unable to convert Unicode string to ANSI\n");
		free(sLogFile);
		free(ansiFileName);
		CloseHandle(hLogFile);

		return pCreateFileW(lpFileName,
							dwDesiredAccess,
							dwShareMode,
							lpSecurityAttributes,
							dwCreationDisposition,
							dwFlagsAndAttributes,
							hTemplateFile);
	}

	sprintf(sLogFile, "Opening file %s with permissions %s", ansiFileName,
							(dwDesiredAccess | GENERIC_ALL) ? "GENERIC_ALL" : "GENERIC_WRITE");

	if (!WriteFile(hLogFile,
					sLogFile,
					strlen(sLogFile),
					&numBytesWritten,
					NULL))
	{
		fprintf(stderr, "Unable to write logs to file\n");
		free(sLogFile);
		free(ansiFileName);
		CloseHandle(hLogFile);

		return pCreateFileW(lpFileName,
							dwDesiredAccess,
							dwShareMode,
							lpSecurityAttributes,
							dwCreationDisposition,
							dwFlagsAndAttributes,
							hTemplateFile);
	}

	free(sLogFile);
	free(ansiFileName);
	CloseHandle(hLogFile);

	return pCreateFileW(lpFileName,
						dwDesiredAccess,
						dwShareMode,
						lpSecurityAttributes,
						dwCreationDisposition,
						dwFlagsAndAttributes,
						hTemplateFile);
}


BOOL WINAPI hooked_DeleteFileW(LPCTSTR lpFileName)
{
	BOOL	retVal;
	HANDLE	hLogFile;
	DWORD	numBytesWritten;
	char*	sLogFile;
	char*	ansiFileName;

	hLogFile = pCreateFileW(L"delete.txt",
							GENERIC_WRITE,
							FILE_SHARE_READ,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if (!hLogFile)
	{
		fprintf(stderr, "Unable to open create.txt in write mode\n");

		return pDeleteFileW(lpFileName);
	}

	sLogFile = (char* ) calloc(2048, sizeof(char));
	if (!sLogFile)
	{
		fprintf(stderr, "Unable to allocate memory for file delete\n");
		CloseHandle(hLogFile);

		return pDeleteFileW(lpFileName);
	}

	ansiFileName = (char* ) malloc(_tcslen(lpFileName) * sizeof(TCHAR));
	if (!ansiFileName)
	{
		fprintf(stderr, "Unable to allocate memory for copying file name\n");
		free(sLogFile);
		CloseHandle(hLogFile);

		return pDeleteFileW(lpFileName);
	}

	ZeroMemory(ansiFileName, _tcslen(lpFileName) * sizeof(TCHAR));

	if (!WideCharToMultiByte(CP_ACP,
							0,
							lpFileName,
							_tcslen(lpFileName),
							ansiFileName,
							_tcslen(lpFileName) * sizeof(TCHAR),
							NULL,
							NULL))
	{
		fprintf(stderr, "Unable to convert Unicode string to ANSI\n");
		free(sLogFile);
		free(ansiFileName);
		CloseHandle(hLogFile);

		return pDeleteFileW(lpFileName);
	}

	sprintf(sLogFile, "Delete file %s\r\n", ansiFileName);

	if (!WriteFile(hLogFile,
					sLogFile,
					strlen(sLogFile),
					&numBytesWritten,
					NULL))
	{
		fprintf(stderr, "Unable to write logs to file\n");
		free(sLogFile);
		free(ansiFileName);
		CloseHandle(hLogFile);

		return pDeleteFileW(lpFileName);
	}

	

	if (bCopyFileBeforeDelete)
	{
		int sizeOfString = _tcslen(lpFileName) + 4;
		char*	copiedFileName = (char*) calloc(sizeOfString, sizeof(char));

		if (!copiedFileName)
		{
			fprintf(stderr, "Unable to allocate memory for file copy\n");
			free(sLogFile);
			free(ansiFileName);
			CloseHandle(hLogFile);

			return pDeleteFileW(lpFileName);
		}

		sprintf(copiedFileName, "%s.bak", ansiFileName);
		if (!CopyFileA(ansiFileName, copiedFileName, false))
		{
			fprintf(stderr, "CopyFile failed with error %d\n", GetLastError());
			free(sLogFile);
			free(ansiFileName);
			CloseHandle(hLogFile);

			return pDeleteFileW(lpFileName);
		}

		if (!WideCharToMultiByte(CP_ACP,
							0,
							lpFileName,
							_tcslen(lpFileName),
							ansiFileName,
							_tcslen(lpFileName) * sizeof(TCHAR),
							NULL,
							NULL))
		{
			fprintf(stderr, "Unable to convert Unicode string to ANSI\n");
			free(sLogFile);
			free(ansiFileName);
			CloseHandle(hLogFile);

			return pDeleteFileW(lpFileName);
		}

		ZeroMemory(sLogFile, 2048);
		sprintf(sLogFile, "File %s saved to %s.bak\n", ansiFileName, ansiFileName);
		if (!WriteFile(hLogFile,
						sLogFile,
						strlen(sLogFile),
						&numBytesWritten,
						NULL))
		{
			fprintf(stderr, "Unable to write logs to file\n");
			free(sLogFile);
			free(ansiFileName);
			CloseHandle(hLogFile);

			return pDeleteFileW(lpFileName);
		}
	}

	free(sLogFile);
	free(ansiFileName);
	CloseHandle(hLogFile);
	
	return pDeleteFileW(lpFileName);
}