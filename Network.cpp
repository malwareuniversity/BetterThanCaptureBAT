/* Network.cpp - BetterThanCaptureBAT

	You should just use Wireshark or, better yet, NetworkMiner.
*/

#include "stdafx.h"

#ifndef NETWORK_H
#include "Network.h"
#endif

// Extern redefinitions.
int (WINAPI* pConnect)(SOCKET s,
					   const struct sockaddr* name,
					   int namelen) = connect;
int (WINAPI* pBind)(SOCKET s,
					const struct sockaddr* name,
					int namelen) = bind;
int (WINAPI* pSend)(SOCKET s,
				   const char* buf,
				   int len,
				   int flags) = send;
int (WINAPI* pRecv)(SOCKET s,
					char* buf,
					int len,
					int flags) = recv;
int (WINAPI* pWSAConnect)(SOCKET s,
						const struct sockaddr* name,
						int namelen,
						LPWSABUF lpCallerData,
						LPWSABUF lpCalleeData,
						LPQOS lpSQOS,
						LPQOS lpGQOS) = WSAConnect;
int (WINAPI* pWSASend)(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesSent,
					   DWORD dwFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) = WSASend;
int (WINAPI* pWSARecv)(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesRecvd,
					   LPDWORD lpFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) = WSARecv;


/* The following is the method of using WinINet functions for HTTP connectivity */
HINTERNET (WINAPI* pInternetOpenA)(LPCSTR lpszAgent,
								   DWORD dwAccessType,
								   LPCSTR lpszProxyName,
								   LPCSTR lpszProxyBypass,
								   DWORD dwFlags) = InternetOpenA;
HINTERNET (WINAPI* pInternetOpenW)(LPCWSTR lpszAgent,
								   DWORD dwAccessType,
								   LPCWSTR lpszProxyName,
								   LPCWSTR lpszProxyBypass,
								   DWORD dwFlags) = InternetOpenW;
HINTERNET (WINAPI* pInternetConnectA)(HINTERNET hInternet,
									  LPCSTR lpszServerName,
									  INTERNET_PORT nServerPort,
									  LPCSTR lpszUsername,
									  LPCSTR lpszPassword,
									  DWORD dwService,
									  DWORD dwFlags,
									  DWORD_PTR dwContext) = InternetConnectA;
HINTERNET (WINAPI* pInternetConnectW)(HINTERNET hInternet,
									  LPCWSTR lpszServerName,
									  INTERNET_PORT nServerPort,
									  LPCWSTR lpszUsername,
									  LPCWSTR lpszPassword,
									  DWORD dwService,
									  DWORD dwFlags,
									  DWORD_PTR dwContext) = InternetConnectW;
HINTERNET (WINAPI* pInternetOpenUrlA)(HINTERNET hInternet,
									  LPCSTR lpszUrl,
									  LPCSTR lpszHeaders,
									  DWORD dwHeadersLength,
									  DWORD dwFlags,
									  DWORD_PTR dwContext) = InternetOpenUrlA;
HINTERNET (WINAPI* pInternetOpenUrlW)(HINTERNET hInternet,
									  LPCWSTR lpszUrl,
									  LPCWSTR lpszHeaders,
									  DWORD dwHeadersLength,
									  DWORD dwFlags,
									  DWORD_PTR dwContext) = InternetOpenUrlW;
HINTERNET (WINAPI* pHttpOpenRequestA)(HINTERNET hConnect,
									 LPCSTR lpszVerb,
									 LPCSTR lpszObjectName,
									 LPCSTR lpszVersion,
									 LPCSTR lpszReferer,
									 LPCSTR* lplpszAcceptTypes,
									 DWORD dwFlags,
									 DWORD_PTR dwContext) = HttpOpenRequestA;
HINTERNET (WINAPI* pHttpOpenRequestW)(HINTERNET hConnect,
									 LPCWSTR lpszVerb,
									 LPCWSTR lpszObjectName,
									 LPCWSTR lpszVersion,
									 LPCWSTR lpszReferer,
									 LPCWSTR* lplpszAcceptTypes,
									 DWORD dwFlags,
									 DWORD_PTR dwContext) = HttpOpenRequestW;
BOOL (WINAPI* pHttpAddRequestHeadersA)(HINTERNET hRequest,
									   LPCSTR lpszHeaders,
									   DWORD dwHeadersLength,
									   DWORD dwModifiers) = HttpAddRequestHeadersA;
BOOL (WINAPI* pHttpAddRequestHeadersW)(HINTERNET hRequest,
									   LPCWSTR lpszHeaders,
									   DWORD dwHeadersLength,
									   DWORD dwModifiers) = HttpAddRequestHeadersW;
BOOL (WINAPI* pHttpQueryInfoA)(HINTERNET hRequest,
							   DWORD dwInfoLevel,
							   LPVOID lpvBuffer,
							   LPDWORD lpdwBufferLength,
							   LPDWORD lpdwIndex) = HttpQueryInfoA;
BOOL (WINAPI* pHttpQueryInfoW)(HINTERNET hRequest,
							   DWORD dwInfoLevel,
							   LPVOID lpvBuffer,
							   LPDWORD lpdwBufferLength,
							   LPDWORD lpdwIndex) = HttpQueryInfoW;
BOOL (WINAPI* pHttpSendRequestA)(HINTERNET hRequest,
								LPCSTR lpszHeaders,
								DWORD dwHeadersLength,
								LPVOID lpOptional,
								DWORD dwOptionalLength) = HttpSendRequestA;
BOOL (WINAPI* pHttpSendRequestW)(HINTERNET hRequest,
								LPCWSTR lpszHeaders,
								DWORD dwHeadersLength,
								LPVOID lpOptional,
								DWORD dwOptionalLength) = HttpSendRequestW;
BOOL (WINAPI* pHttpSendRequestExA)(HINTERNET hRequest,
								   LPINTERNET_BUFFERSA lpBuffersIn,
								   LPINTERNET_BUFFERSA lpBuffersOut,
								   DWORD dwFlags,
								   DWORD_PTR dwContext) = HttpSendRequestExA;
BOOL (WINAPI* pHttpSendRequestExW)(HINTERNET hRequest,
								   LPINTERNET_BUFFERSW lpBuffersIn,
								   LPINTERNET_BUFFERSW lpBuffersOut,
								   DWORD dwFlags,
								   DWORD_PTR dwContext) = HttpSendRequestExW;
BOOL (WINAPI* pInternetReadFile)(HINTERNET hFile,
								 LPVOID lpBuffer,
								 DWORD dwNumberOfBytesToRead,
								 LPDWORD lpdwNumberOfBytesRead) = InternetReadFile;
BOOL (WINAPI* pInternetReadFileExA)(HINTERNET hFile,
									LPINTERNET_BUFFERSA lpBuffersOut,
									DWORD dwFlags,
									DWORD_PTR dwContext) = InternetReadFileExA;
BOOL (WINAPI* pInternetReadFileExW)(HINTERNET hFile,
									LPINTERNET_BUFFERSW lpBuffersOut,
									DWORD dwFlags,
									DWORD_PTR dwContext) = InternetReadFileExW;



int WINAPI hooked_connect(SOCKET s,
					   const struct sockaddr* name,
					   int namelen)
{
	FILE*	logFile;
	struct sockaddr_in* temp = (struct sockaddr_in*) name;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pConnect(s, name, namelen);
	}

	fprintf(logFile, "Attempting to connect to %s:%d", inet_ntoa(temp->sin_addr), ntohs(temp->sin_port));
	// Cannot put '\n' in binary files.
	binNewline(logFile);
	fclose(logFile);

	return pConnect(s, name, namelen);
}

int WINAPI hooked_bind(SOCKET s,
					const struct sockaddr* name,
					int namelen)
{
	FILE*	logFile;
	struct sockaddr_in* temp = (struct sockaddr_in*) name;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pBind(s, name, namelen);
	}

	fprintf(logFile, "Binding to %s:%d", inet_ntoa(temp->sin_addr), ntohs(temp->sin_port));
	// Cannot put '\n' in binary files.
	binNewline(logFile);
	fclose(logFile);

	return pBind(s, name, namelen);
}

int WINAPI hooked_send(SOCKET s,
				   const char* buf,
				   int len,
				   int flags)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pSend(s, buf, len, flags);
	}

	// Cannot put '\n' in binary files.
	fprintf(logFile, "Sending %d bytes data:", len);
	binNewline(logFile);
	for (int i = 0; i < len; i++)
		fputc(*(buf + i), logFile);
	binNewline(logFile);
	binNewline(logFile);
	binNewline(logFile);
	fclose(logFile);

	return pSend(s, buf, len, flags);
}

int WINAPI hooked_recv(SOCKET s,
					char* buf,
					int len,
					int flags)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pRecv(s, buf, len, flags);
	}

	fprintf(logFile, "Receiving %d bytes data:\n", len);
	binNewline(logFile);
	for (int i = 0; i < len; i++)
		fputc(*(buf + i), logFile);
	binNewline(logFile);
	binNewline(logFile);
	binNewline(logFile);
	fclose(logFile);

	return pRecv(s, buf, len, flags);
}

int WINAPI hooked_WSAConnect(SOCKET s,
						const struct sockaddr* name,
						int namelen,
						LPWSABUF lpCallerData,		// not supported in TCP/IP
						LPWSABUF lpCalleeData,		// not supported in TCP/IP
						LPQOS lpSQOS,				// unimportant
						LPQOS lpGQOS)				// reserved
{
	FILE*	logFile;
	struct sockaddr_in* temp = (struct sockaddr_in*) name;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pWSAConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
	}

	// Cannot put '\n' in binary files.
	fprintf(logFile, "Attempting to connect to %s:%d", inet_ntoa(temp->sin_addr), ntohs(temp->sin_port));
	binNewline(logFile);
	fclose(logFile);

	return pWSAConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
}

int WINAPI hooked_WSASend(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesSent,
					   DWORD dwFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	FILE*	logFile;
	char*	tempBuffer;
	int		totalBuffersByteCount;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pWSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
	}

	// Iterate through all WSABUF structures.
	for (int i = 0; i < dwBufferCount; i++)
	{
		if (!lpBuffers[i].buf)
		{
			fprintf(stderr, "lpBuffers[%d] is empty for some reason\n", i);
			fclose(logFile);

			return pWSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
		}

		tempBuffer = (char*) calloc(lpBuffers[i].len, sizeof(char));
		if (!tempBuffer)
		{
			fprintf(stderr, "Unable to allocate buffer for WSASend buffers\n");
			fclose(logFile);

			return pWSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
		}

		if (!memcpy(tempBuffer, lpBuffers[i].buf, lpBuffers[i].len))
		{
			fprintf(stderr, "Did not copy WSASend buffer\n");
			fclose(logFile);
			free(tempBuffer);

			return pWSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
		}

		for (int k = 0; k < lpBuffers[i].len; k++)
			fputc(tempBuffer[k], logFile);

		totalBuffersByteCount += lpBuffers[i].len;
		free(tempBuffer);
	}

	binNewline(logFile);
	// Cannot put '\n' in binary files.
	fprintf(logFile, "Sent %d bytes of data", totalBuffersByteCount);
	for (int i = 0; i < 3; i++)
		binNewline(logFile);
	fclose(logFile);

	return pWSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
}

int WINAPI hooked_WSARecv(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesRecvd,
					   LPDWORD lpFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	FILE*	logFile;
	char*	tempBuffer;
	int		totalBuffersByteCount;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pWSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
	}

	// Iterate through all WSABUF structures.
	for (int i = 0; i < dwBufferCount; i++)
	{
		if (!lpBuffers[i].buf)
		{
			fprintf(stderr, "lpBuffers[%d] is empty for some reason\n", i);
			fclose(logFile);

			return pWSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
		}

		tempBuffer = (char*) calloc(lpBuffers[i].len, sizeof(char));
		if (!tempBuffer)
		{
			fprintf(stderr, "Unable to allocate buffer for WSARecv buffers\n");
			fclose(logFile);

			return pWSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
		}

		if (!memcpy(tempBuffer, lpBuffers[i].buf, lpBuffers[i].len))
		{
			fprintf(stderr, "Did not copy WSARecv buffer\n");
			fclose(logFile);
			free(tempBuffer);

			return pWSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
		}

		for (int k = 0; k < lpBuffers[i].len; k++)
			fputc(tempBuffer[k], logFile);

		totalBuffersByteCount += lpBuffers[i].len;
		free(tempBuffer);
	}

	binNewline(logFile);
	// Cannot put '\n' in binary files.
	fprintf(logFile, "Received %d bytes of data", totalBuffersByteCount);
	for (int i = 0; i < 3; i++)
		binNewline(logFile);
	fclose(logFile);

	return pWSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
}


HINTERNET WINAPI hooked_InternetOpenA(LPCSTR lpszAgent,
								   DWORD dwAccessType,
								   LPCSTR lpszProxyName,
								   LPCSTR lpszProxyBypass,
								   DWORD dwFlags)
{
	FILE*	logFile;
	char*	tempBuffer;
	int		totalBuffersByteCount;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pInternetOpenA(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
	}

	_ftprintf(logFile, _T("Prepairing connection to internet with the following configuration:"));
	binNewline(logFile);
	_ftprintf(logFile, _T("   User Agent:  %s"), lpszAgent);
	binNewline(logFile);
	_ftprintf(logFile, _T("   Access Type:  %s"), (dwAccessType == INTERNET_OPEN_TYPE_DIRECT) ? _T("Direct") : 
													(dwAccessType == INTERNET_OPEN_TYPE_PRECONFIG) ? _T("Depends on registry setting (direct/proxy)") :
													(dwAccessType == INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY) ? _T("Depends on registry setting (direct/proxy) and ")
														_T("preventions startup of JScript or INS file") :
													(dwAccessType == INTERNET_OPEN_TYPE_PROXY) ? _T("Proxy") : _T("Unknown"));
	binNewline(logFile);
	_ftprintf(logFile, _T("   Proxy Name:  %s"), lpszProxyName);
	binNewline(logFile);
	_ftprintf(logFile, _T("   Proxy Bypass:  %s"), lpszProxyBypass);
	binNewline(logFile);
	
	binNewline(logFile);
	fclose(logFile);

	return pInternetOpenA(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
}

HINTERNET WINAPI hooked_InternetOpenW(LPCWSTR lpszAgent,
								   DWORD dwAccessType,
								   LPCWSTR lpszProxyName,
								   LPCWSTR lpszProxyBypass,
								   DWORD dwFlags)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pInternetOpenW(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
	}

	_ftprintf(logFile, _T("Prepairing connection to internet with the following configuration:"));
	binNewline(logFile);
	_ftprintf(logFile, _T("   User Agent:  %s"), lpszAgent);
	binNewline(logFile);
	_ftprintf(logFile, _T("   Access Type:  %s"), (dwAccessType == INTERNET_OPEN_TYPE_DIRECT) ? _T("Direct") : 
													(dwAccessType == INTERNET_OPEN_TYPE_PRECONFIG) ? _T("Depends on registry setting (direct/proxy)") :
													(dwAccessType == INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY) ? _T("Depends on registry setting (direct/proxy) and ")
														_T("preventions startup of JScript or INS file") :
													(dwAccessType == INTERNET_OPEN_TYPE_PROXY) ? _T("Proxy") : _T("Unknown"));
	binNewline(logFile);
	_ftprintf(logFile, _T("   Proxy Name:  %s"), lpszProxyName);
	binNewline(logFile);
	_ftprintf(logFile, _T("   Proxy Bypass:  %s"), lpszProxyBypass);
	binNewline(logFile);
	
	binNewline(logFile);
	fclose(logFile);

	return pInternetOpenW(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
}

HINTERNET WINAPI hooked_InternetConnectA(HINTERNET hInternet,
									  LPCSTR lpszServerName,
									  INTERNET_PORT nServerPort,
									  LPCSTR lpszUsername,
									  LPCSTR lpszPassword,
									  DWORD dwService,
									  DWORD dwFlags,
									  DWORD_PTR dwContext)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pInternetConnectA(hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
	}

	// Only FTP causes an automatic connection request.
	// HTTP and Gopher must specifically connect later.
	if (dwService == INTERNET_SERVICE_FTP)
		_ftprintf(logFile, _T("Connecting to %s:%d"), lpszServerName, nServerPort);
	else
		_ftprintf(logFile, _T("Setting up connection to %s:%d"), lpszServerName, nServerPort);
	binNewline(logFile);

	// Check for auth credentials
	if (lpszUsername)
	{
		_ftprintf(logFile, _T("   Username:  %s"), lpszUsername);
		binNewline(logFile);
		_ftprintf(logFile, _T("   Password:  %s"), lpszPassword);
		binNewline(logFile);
	}

	binNewline(logFile);
	fclose(logFile);

	return pInternetConnectA(hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
}

HINTERNET WINAPI hooked_InternetConnectW(HINTERNET hInternet,
									  LPCWSTR lpszServerName,
									  INTERNET_PORT nServerPort,
									  LPCWSTR lpszUsername,
									  LPCWSTR lpszPassword,
									  DWORD dwService,
									  DWORD dwFlags,
									  DWORD_PTR dwContext)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pInternetConnectW(hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
	}
	
	// Only FTP causes an automatic connection request.
	// HTTP and Gopher must specifically connect later.
	if (dwService == INTERNET_SERVICE_FTP)
		_ftprintf(logFile, _T("Connecting to %s:%d"), lpszServerName, nServerPort);
	else
		_ftprintf(logFile, _T("Setting up connection to %s:%d"), lpszServerName, nServerPort);
	binNewline(logFile);

	// Check for auth credentials
	if (lpszUsername)
	{
		_ftprintf(logFile, _T("   Username:  %s"), lpszUsername);
		binNewline(logFile);
		_ftprintf(logFile, _T("   Password:  %s"), lpszPassword);
		binNewline(logFile);
	}

	binNewline(logFile);
	fclose(logFile);

	return pInternetConnectW(hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
}

HINTERNET WINAPI hooked_InternetOpenUrlA(HINTERNET hInternet,
									  LPCSTR lpszUrl,
									  LPCSTR lpszHeaders,
									  DWORD dwHeadersLength,
									  DWORD dwFlags,
									  DWORD_PTR dwContext)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pInternetOpenUrlA(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
	}

	_ftprintf(logFile, _T("Connecting to %s"), lpszUrl);
	binNewline(logFile);
	if (dwHeadersLength == -1)
		_ftprintf(logFile, _T("   Headers:  %s"), lpszHeaders);
	else if ((signed int) dwHeadersLength > 0)
	{
		_ftprintf(logFile, _T("   Headers:  "));
		for (int i = 0; i < dwHeadersLength; i++)
			fputc(*(lpszHeaders + i), logFile);
	}
	binNewline(logFile);

	if (dwFlags & INTERNET_FLAG_NO_CACHE_WRITE)
	{
		_ftprintf(logFile, _T("   Do not write request to cache"));
		binNewline(logFile);
	}

	if (dwFlags & INTERNET_FLAG_RELOAD)
	{
		_ftprintf(logFile, _T("   Force reload from server, no cache requests"));
		binNewline(logFile);
	}

	binNewline(logFile);
	fclose(logFile);

	return pInternetOpenUrlA(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}

HINTERNET WINAPI hooked_InternetOpenUrlW(HINTERNET hInternet,
									  LPCWSTR lpszUrl,
									  LPCWSTR lpszHeaders,
									  DWORD dwHeadersLength,
									  DWORD dwFlags,
									  DWORD_PTR dwContext)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pInternetOpenUrlW(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
	}

	_ftprintf(logFile, _T("Connecting to %s"), lpszUrl);
	binNewline(logFile);
	if (dwHeadersLength == -1)
		_ftprintf(logFile, _T("   Headers:  %s"), lpszHeaders);
	else if ((signed int) dwHeadersLength > 0)
	{
		_ftprintf(logFile, _T("   Headers:  "));
		for (int i = 0; i < dwHeadersLength; i++)
			fputc(*(lpszHeaders + i), logFile);
	}
	binNewline(logFile);

	if (dwFlags & INTERNET_FLAG_NO_CACHE_WRITE)
	{
		_ftprintf(logFile, _T("   Do not write request to cache"));
		binNewline(logFile);
	}

	if (dwFlags & INTERNET_FLAG_RELOAD)
	{
		_ftprintf(logFile, _T("   Force reload from server, no cache requests"));
		binNewline(logFile);
	}

	binNewline(logFile);
	fclose(logFile);

	return pInternetOpenUrlW(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}

HINTERNET WINAPI hooked_HttpOpenRequestA(HINTERNET hConnect,
									 LPCSTR lpszVerb,
									 LPCSTR lpszObjectName,
									 LPCSTR lpszVersion,
									 LPCSTR lpszReferer,
									 LPCSTR* lplpszAcceptTypes,
									 DWORD dwFlags,
									 DWORD_PTR dwContext)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pHttpOpenRequestA(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferer, lplpszAcceptTypes, dwFlags, dwContext);
	}

	_ftprintf(logFile, _T("HTTP request:"));
	binNewline(logFile);
	if (!lpszVerb)
		_ftprintf(logFile, _T("   GET "));
	else
		_ftprintf(logFile, _T("   %s "), lpszVerb);

	if (!lpszObjectName)
		_ftprintf(logFile, _T("index.html"));
	else
		_ftprintf(logFile, _T("%s"), lpszObjectName);

	if (!lpszVersion)
		_ftprintf(logFile, _T(" HTTP/1.1"));
	else
		_ftprintf(logFile, _T(" %s"), lpszVersion);

	binNewline(logFile);
	if (lpszReferer)
	{
		_ftprintf(logFile, _T("   Referer:  %s"), lpszReferer);
		binNewline(logFile);
	}

	_ftprintf(logFile, _T("   Accepted types:  "));
	// Heavy potential for a bug right here.  The last entry is supposed to be a NULL pointer.  Not enforced, however.
	if (lplpszAcceptTypes)
	{
		do
		{
			_ftprintf(logFile, _T("%s"), (LPCSTR) *lplpszAcceptTypes);
			lplpszAcceptTypes++;	// lplpszAcceptTypes += sizeof(LPCSTR*);
			if (lplpszAcceptTypes)
				fputc(',', logFile);
		} while (lplpszAcceptTypes);
	}

	else
		_ftprintf(logFile, _T("text/*"));
	binNewline(logFile);

	binNewline(logFile);
	fclose(logFile);

	return pHttpOpenRequestA(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferer, lplpszAcceptTypes, dwFlags, dwContext);
}

HINTERNET WINAPI hooked_HttpOpenRequestW(HINTERNET hConnect,
									 LPCWSTR lpszVerb,
									 LPCWSTR lpszObjectName,
									 LPCWSTR lpszVersion,
									 LPCWSTR lpszReferer,
									 LPCWSTR* lplpszAcceptTypes,
									 DWORD dwFlags,
									 DWORD_PTR dwContext)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pHttpOpenRequestW(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferer, lplpszAcceptTypes, dwFlags, dwContext);
	}

	_ftprintf(logFile, _T("HTTP request:"));
	binNewline(logFile);
	if (!lpszVerb)
		_ftprintf(logFile, _T("   GET "));
	else
		_ftprintf(logFile, _T("   %s "), lpszVerb);

	if (!lpszObjectName)
		_ftprintf(logFile, _T("index.html"));
	else
		_ftprintf(logFile, _T("%s"), lpszObjectName);

	if (!lpszVersion)
		_ftprintf(logFile, _T(" HTTP/1.1"));
	else
		_ftprintf(logFile, _T(" %s"), lpszVersion);

	binNewline(logFile);
	if (lpszReferer)
	{
		_ftprintf(logFile, _T("   Referer:  %s"), lpszReferer);
		binNewline(logFile);
	}

	_ftprintf(logFile, _T("   Accepted types:  "));
	// Heavy potential for a bug right here.  The last entry is supposed to be a NULL pointer.  Not enforced, however.
	if (lplpszAcceptTypes)
	{
		do
		{
			_ftprintf(logFile, _T("%s"), (LPCSTR) *lplpszAcceptTypes);
			lplpszAcceptTypes++;	// lplpszAcceptTypes += sizeof(LPCSTR*);
			if (lplpszAcceptTypes)
				fputc(',', logFile);
		} while (lplpszAcceptTypes);
	}

	else
		_ftprintf(logFile, _T("text/*"));
	binNewline(logFile);

	binNewline(logFile);
	fclose(logFile);

	return pHttpOpenRequestW(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferer, lplpszAcceptTypes, dwFlags, dwContext);
}

BOOL WINAPI hooked_HttpAddRequestHeadersA(HINTERNET hRequest,
									   LPCSTR lpszHeaders,
									   DWORD dwHeadersLength,
									   DWORD dwModifiers)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pHttpAddRequestHeadersA(hRequest, lpszHeaders, dwHeadersLength, dwModifiers);
	}

	_ftprintf(logFile, _T("Additional HTTP request header:"));
	binNewline(logFile);

	if ((signed int) dwHeadersLength == -1)
		_ftprintf(logFile, _T("   Header:  %s"), lpszHeaders);
	else if ((signed int) dwHeadersLength > 0)
	{
		for (int i = 0; i < dwHeadersLength; i++)
			fputc(*(lpszHeaders + i), logFile);
	}

	binNewline(logFile);
	binNewline(logFile);
	fclose(logFile);

	return pHttpAddRequestHeadersA(hRequest, lpszHeaders, dwHeadersLength, dwModifiers);
}

BOOL WINAPI hooked_HttpAddRequestHeadersW(HINTERNET hRequest,
									   LPCWSTR lpszHeaders,
									   DWORD dwHeadersLength,
									   DWORD dwModifiers)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pHttpAddRequestHeadersW(hRequest, lpszHeaders, dwHeadersLength, dwModifiers);
	}

	_ftprintf(logFile, _T("Additional HTTP request header:"));
	binNewline(logFile);

	if ((signed int) dwHeadersLength == -1)
		_ftprintf(logFile, _T("   Header:  %s"), lpszHeaders);
	else if ((signed int) dwHeadersLength > 0)
	{
		for (int i = 0; i < dwHeadersLength; i++)
			fputc(*(lpszHeaders + i), logFile);
	}

	binNewline(logFile);
	binNewline(logFile);
	fclose(logFile);

	return pHttpAddRequestHeadersW(hRequest, lpszHeaders, dwHeadersLength, dwModifiers);
}

BOOL WINAPI hooked_HttpQueryInfoA(HINTERNET hRequest,
							   DWORD dwInfoLevel,
							   LPVOID lpvBuffer,
							   LPDWORD lpdwBufferLength,
							   LPDWORD lpdwIndex)
{
	return pHttpQueryInfoA(hRequest, dwInfoLevel, lpvBuffer, lpdwBufferLength, lpdwIndex);
}

BOOL WINAPI hooked_HttpQueryInfoW(HINTERNET hRequest,
								  DWORD dwInfoLevel,
								  LPVOID lpvBuffer,
								  LPDWORD lpdwBufferLength,
								  LPDWORD lpdwIndex)
{
	return pHttpQueryInfoW(hRequest, dwInfoLevel, lpvBuffer, lpdwBufferLength, lpdwIndex);
}

BOOL WINAPI hooked_HttpSendRequestA(HINTERNET hRequest,
								LPCSTR lpszHeaders,
								DWORD dwHeadersLength,
								LPVOID lpOptional,
								DWORD dwOptionalLength)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pHttpSendRequestA(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
	}

	_ftprintf(logFile, _T("Sending HTTP request"));
	binNewline(logFile);

	if (lpszHeaders)
	{
		_ftprintf(logFile, _T("   Addtional headers:  "));
		if ((signed int) dwHeadersLength == -1)
			_ftprintf(logFile, _T("%s"), lpszHeaders);
		else if ((signed int) dwHeadersLength > 0)
			for (int i = 0; i < dwHeadersLength; i++)
				fputc(*(lpszHeaders + i), logFile);
		binNewline(logFile);
	}

	if (dwOptionalLength)
	{
		if ((signed int) dwOptionalLength > 0)
		{
			_ftprintf(logFile, _T("Optional data:  "));
			for (int i = 0; i < dwOptionalLength; i++)
				fputc(*((char* )lpOptional + i), logFile);
			binNewline(logFile);
		}
	}

	binNewline(logFile);
	fclose(logFile);

	return pHttpSendRequestA(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
}

BOOL WINAPI hooked_HttpSendRequestW(HINTERNET hRequest,
								LPCWSTR lpszHeaders,
								DWORD dwHeadersLength,
								LPVOID lpOptional,
								DWORD dwOptionalLength)
{
	FILE*	logFile;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pHttpSendRequestW(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
	}

	_ftprintf(logFile, _T("Sending HTTP request"));
	binNewline(logFile);

	if (lpszHeaders)
	{
		_ftprintf(logFile, _T("   Addtional headers:  "));
		if ((signed int) dwHeadersLength == -1)
			_ftprintf(logFile, _T("%s"), lpszHeaders);
		else if ((signed int) dwHeadersLength > 0)
			for (int i = 0; i < dwHeadersLength; i++)
				fputwc(*(lpszHeaders + i), logFile);
		binNewline(logFile);
	}

	// Not sure if this is Unicode.
	// Assuming ANSI.
	if (dwOptionalLength)
	{
		if ((signed int) dwOptionalLength > 0)
		{
			_ftprintf(logFile, _T("Optional data:  "));
			for (int i = 0; i < dwOptionalLength; i++)
				fputc(*((char* )lpOptional + i), logFile);
			binNewline(logFile);
		}
	}

	binNewline(logFile);
	fclose(logFile);

	return pHttpSendRequestW(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
}

BOOL WINAPI hooked_HttpSendRequestExA(HINTERNET hRequest,
								   LPINTERNET_BUFFERSA lpBuffersIn,
								   LPINTERNET_BUFFERSA lpBuffersOut,
								   DWORD dwFlags,
								   DWORD_PTR dwContext)
{
	FILE*				logFile;
	INTERNET_BUFFERSA	currInternetBuffers;
	INTERNET_BUFFERSA*	pNextInternetBuffer;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pHttpSendRequestExA(hRequest, lpBuffersIn, lpBuffersOut, dwFlags, dwContext);
	}

	_ftprintf(logFile, _T("Sending HTTP request"));
	binNewline(logFile);

	// Must handle INTERNET_BUFFERS structure(s).
	if (lpBuffersIn && ((*lpBuffersIn).dwStructSize == sizeof(INTERNET_BUFFERSA)))
	{
		pNextInternetBuffer = lpBuffersIn;

		do
		{
			currInternetBuffers = *pNextInternetBuffer;

			if (currInternetBuffers.dwStructSize != sizeof(INTERNET_BUFFERSA))
				break;
			
			if (currInternetBuffers.lpcszHeader)
			{
				_ftprintf(logFile, _T("   Header:  "));
				for (int i = 0; i < (signed int) currInternetBuffers.dwHeadersLength; i++)
					fputc(*(currInternetBuffers.lpcszHeader + i), logFile);
				binNewline(logFile);
			}

			if (currInternetBuffers.lpvBuffer)
			{
				_ftprintf(logFile, _T("   Buffer:  "));
				for (int i = 0; i < (signed int) currInternetBuffers.dwBufferLength; i++)
					fputc(*((char* )currInternetBuffers.lpvBuffer), logFile);
				binNewline(logFile);
			}

			pNextInternetBuffer = pNextInternetBuffer->Next;
		} while (pNextInternetBuffer);
	}

	binNewline(logFile);
	fclose(logFile);

	return pHttpSendRequestExA(hRequest, lpBuffersIn, lpBuffersOut, dwFlags, dwContext);
}

BOOL WINAPI hooked_HttpSendRequestExW(HINTERNET hRequest,
								   LPINTERNET_BUFFERSW lpBuffersIn,
								   LPINTERNET_BUFFERSW lpBuffersOut,
								   DWORD dwFlags,
								   DWORD_PTR dwContext)
{
	FILE*				logFile;
	INTERNET_BUFFERSW	currInternetBuffers;
	INTERNET_BUFFERSW*	pNextInternetBuffer;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return pHttpSendRequestExW(hRequest, lpBuffersIn, lpBuffersOut, dwFlags, dwContext);
	}

	_ftprintf(logFile, _T("Sending HTTP request"));
	binNewline(logFile);

	// Must handle INTERNET_BUFFERS structure(s).
	if (lpBuffersIn && ((*lpBuffersIn).dwStructSize == sizeof(INTERNET_BUFFERSW)))
	{
		pNextInternetBuffer = lpBuffersIn;

		do
		{
			currInternetBuffers = *pNextInternetBuffer;

			if (currInternetBuffers.dwStructSize != sizeof(INTERNET_BUFFERSW))
				break;
			
			if (currInternetBuffers.lpcszHeader)
			{
				_ftprintf(logFile, _T("   Header:  "));
				for (int i = 0; i < (signed int) currInternetBuffers.dwHeadersLength; i++)
					fputwc(*(currInternetBuffers.lpcszHeader + i), logFile);
				binNewline(logFile);
			}

			if (currInternetBuffers.lpvBuffer)
			{
				_ftprintf(logFile, _T("   Buffer:  "));
				for (int i = 0; i < (signed int) currInternetBuffers.dwBufferLength; i++)
					fputc(*((char* )currInternetBuffers.lpvBuffer), logFile);
				binNewline(logFile);
			}

			pNextInternetBuffer = pNextInternetBuffer->Next;
		} while (pNextInternetBuffer);
	}

	binNewline(logFile);
	fclose(logFile);

	return pHttpSendRequestExW(hRequest, lpBuffersIn, lpBuffersOut, dwFlags, dwContext);
}

//
//
//
// This function really needs to be debugged.  Lots of room for error.
BOOL WINAPI hooked_InternetReadFile(HINTERNET hFile,
								 LPVOID lpBuffer,
								 DWORD dwNumberOfBytesToRead,
								 LPDWORD lpdwNumberOfBytesRead)
{
	FILE*			logFile;
	BOOL			bReadSuccess = false;
	const int		maxAttempts = 3;	// Maximum attempts at pInternetReadFile() when lpdwNumberOfBytesRead does not change from the previous call
	int				attempts = 0;
	int				prevNumberOfBytesRead = 0;

	bReadSuccess = pInternetReadFile(hFile, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
	prevNumberOfBytesRead = *lpdwNumberOfBytesRead;

	while (!bReadSuccess)
	{
		bReadSuccess = pInternetReadFile(hFile, (char* ) lpBuffer + *lpdwNumberOfBytesRead, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
		if ((signed int) lpdwNumberOfBytesRead == prevNumberOfBytesRead)
			attempts++;
		else
			attempts = 0;

		// Fail if we can't get any progress on reading.
		if (attempts == maxAttempts)
			return bReadSuccess;
	}

	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return bReadSuccess;
	}

	_ftprintf(logFile, _T("Internet Read data:  "));
	for (int i = 0; i < (signed int) dwNumberOfBytesToRead; i++)
		fputc(*((char* ) lpBuffer + i), logFile);

	binNewline(logFile);
	binNewline(logFile);
	fclose(logFile);

	return bReadSuccess;
}

BOOL WINAPI hooked_InternetReadFileExA(HINTERNET hFile,
									LPINTERNET_BUFFERSA lpBuffersOut,
									DWORD dwFlags,
									DWORD_PTR dwContext)
{
	FILE*				logFile;
	INTERNET_BUFFERSA	currInternetBuffers;
	INTERNET_BUFFERSA*	pNextInternetBuffer;
	BOOL				bReadSuccess = false;
	
	bReadSuccess = pInternetReadFileExA(hFile, lpBuffersOut, dwFlags, dwContext);
	if (!bReadSuccess)
		return bReadSuccess;

	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return bReadSuccess;
	}

	_ftprintf(logFile, _T("Reading HTTP request"));
	binNewline(logFile);

	// Must handle INTERNET_BUFFERS structure(s).
	if (lpBuffersOut && ((*lpBuffersOut).dwStructSize == sizeof(INTERNET_BUFFERSA)))
	{
		pNextInternetBuffer = lpBuffersOut;

		do
		{
			currInternetBuffers = *pNextInternetBuffer;

			if (currInternetBuffers.dwStructSize != sizeof(INTERNET_BUFFERSA))
				break;
			
			if (currInternetBuffers.lpcszHeader)
			{
				_ftprintf(logFile, _T("   Header:  "));
				for (int i = 0; i < (signed int) currInternetBuffers.dwHeadersLength; i++)
					fputc(*(currInternetBuffers.lpcszHeader + i), logFile);
				binNewline(logFile);
			}

			if (currInternetBuffers.lpvBuffer)
			{
				_ftprintf(logFile, _T("   Buffer:  "));
				for (int i = 0; i < (signed int) currInternetBuffers.dwBufferLength; i++)
					fputc(*((char* )currInternetBuffers.lpvBuffer), logFile);
				binNewline(logFile);
			}

			pNextInternetBuffer = pNextInternetBuffer->Next;
		} while (pNextInternetBuffer);
	}

	binNewline(logFile);
	fclose(logFile);

	return pInternetReadFileExA(hFile, lpBuffersOut, dwFlags, dwContext);
}

BOOL WINAPI hooked_InternetReadFileExW(HINTERNET hFile,
									LPINTERNET_BUFFERSW lpBuffersOut,
									DWORD dwFlags,
									DWORD_PTR dwContext)
{
	FILE*				logFile;
	INTERNET_BUFFERSW	currInternetBuffers;
	INTERNET_BUFFERSW*	pNextInternetBuffer;
	BOOL				bReadSuccess = false;
	
	bReadSuccess = pInternetReadFileExW(hFile, lpBuffersOut, dwFlags, dwContext);
	if (!bReadSuccess)
		return bReadSuccess;
	
	logFile = fopen("network.txt", "ab");
	if (!logFile)
	{
		fprintf(stderr, "Unable to open network log file\n");

		return bReadSuccess;
	}

	_ftprintf(logFile, _T("Reading HTTP request"));
	binNewline(logFile);

	// Must handle INTERNET_BUFFERS structure(s).
	if (lpBuffersOut && ((*lpBuffersOut).dwStructSize == sizeof(INTERNET_BUFFERSW)))
	{
		pNextInternetBuffer = lpBuffersOut;

		do
		{
			currInternetBuffers = *pNextInternetBuffer;

			if (currInternetBuffers.dwStructSize != sizeof(INTERNET_BUFFERSW))
				break;
			
			if (currInternetBuffers.lpcszHeader)
			{
				_ftprintf(logFile, _T("   Header:  "));
				for (int i = 0; i < (signed int) currInternetBuffers.dwHeadersLength; i++)
					fputwc(*(currInternetBuffers.lpcszHeader + i), logFile);
				binNewline(logFile);
			}

			if (currInternetBuffers.lpvBuffer)
			{
				_ftprintf(logFile, _T("   Buffer:  "));
				for (int i = 0; i < (signed int) currInternetBuffers.dwBufferLength; i++)
					fputc(*((char* )currInternetBuffers.lpvBuffer), logFile);
				binNewline(logFile);
			}

			pNextInternetBuffer = pNextInternetBuffer->Next;
		} while (pNextInternetBuffer);
	}

	binNewline(logFile);
	fclose(logFile);

	return pInternetReadFileExW(hFile, lpBuffersOut, dwFlags, dwContext);
}