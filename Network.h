#ifndef NETWORK_H
#define NETWORK_H

// This is better than macros.
__forceinline void binNewline(FILE* fp)
{
	fputc('\x0D', fp);
	fputc('\x0A', fp);
}

// We must declare, not define, function pointers in the header.  Otherwise, linker fails.
extern int (WINAPI* pConnect)(SOCKET s,
					   const struct sockaddr* name,
					   int namelen);
int WINAPI hooked_connect(SOCKET s,
					   const struct sockaddr* name,
					   int namelen);
extern int (WINAPI* pBind)(SOCKET s,
					const struct sockaddr* name,
					int namelen);
int WINAPI hooked_bind(SOCKET s,
					const struct sockaddr* name,
					int namelen);

extern int (WINAPI* pSend)(SOCKET s,
				   const char* buf,
				   int len,
				   int flags);
int WINAPI hooked_send(SOCKET s,
				   const char* buf,
				   int len,
				   int flags);
extern int (WINAPI* pRecv)(SOCKET s,
					char* buf,
					int len,
					int flags);
int WINAPI hooked_recv(SOCKET s,
					char* buf,
					int len,
					int flags);

extern int (WINAPI* pWSAConnect)(SOCKET s,
						const struct sockaddr* name,
						int namelen,
						LPWSABUF lpCallerData,
						LPWSABUF lpCalleeData,
						LPQOS lpSQOS,
						LPQOS lpGQOS);
int WINAPI hooked_WSAConnect(SOCKET s,
						const struct sockaddr* name,
						int namelen,
						LPWSABUF lpCallerData,
						LPWSABUF lpCalleeData,
						LPQOS lpSQOS,
						LPQOS lpGQOS);
extern int (WINAPI* pWSASend)(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesSent,
					   DWORD dwFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int WINAPI hooked_WSASend(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesSent,
					   DWORD dwFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
extern int (WINAPI* pWSARecv)(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesRecvd,
					   LPDWORD lpFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int WINAPI hooked_WSARecv(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesRecvd,
					   LPDWORD lpFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

/* The following is the method of using WinINet functions for HTTP connectivity */
extern HINTERNET (WINAPI* pInternetOpenA)(LPCSTR lpszAgent,
								   DWORD dwAccessType,
								   LPCSTR lpszProxyName,
								   LPCSTR lpszProxyBypass,
								   DWORD dwFlags);
HINTERNET WINAPI hooked_InternetOpenA(LPCSTR lpszAgent,
								   DWORD dwAccessType,
								   LPCSTR lpszProxyName,
								   LPCSTR lpszProxyBypass,
								   DWORD dwFlags);
extern HINTERNET (WINAPI* pInternetOpenW)(LPCWSTR lpszAgent,
								   DWORD dwAccessType,
								   LPCWSTR lpszProxyName,
								   LPCWSTR lpszProxyBypass,
								   DWORD dwFlags);
HINTERNET WINAPI hooked_InternetOpenW(LPCWSTR lpszAgent,
								   DWORD dwAccessType,
								   LPCWSTR lpszProxyName,
								   LPCWSTR lpszProxyBypass,
								   DWORD dwFlags);
extern HINTERNET (WINAPI* pInternetConnectA)(HINTERNET hInternet,
									  LPCSTR lpszServerName,
									  INTERNET_PORT nServerPort,
									  LPCSTR lpszUsername,
									  LPCSTR lpszPassword,
									  DWORD dwService,
									  DWORD dwFlags,
									  DWORD_PTR dwContext);
HINTERNET WINAPI hooked_InternetConnectA(HINTERNET hInternet,
									  LPCSTR lpszServerName,
									  INTERNET_PORT nServerPort,
									  LPCSTR lpszUsername,
									  LPCSTR lpszPassword,
									  DWORD dwService,
									  DWORD dwFlags,
									  DWORD_PTR dwContext);
extern HINTERNET (WINAPI* pInternetConnectW)(HINTERNET hInternet,
									  LPCWSTR lpszServerName,
									  INTERNET_PORT nServerPort,
									  LPCWSTR lpszUsername,
									  LPCWSTR lpszPassword,
									  DWORD dwService,
									  DWORD dwFlags,
									  DWORD_PTR dwContext);
HINTERNET WINAPI hooked_InternetConnectW(HINTERNET hInternet,
									  LPCWSTR lpszServerName,
									  INTERNET_PORT nServerPort,
									  LPCWSTR lpszUsername,
									  LPCWSTR lpszPassword,
									  DWORD dwService,
									  DWORD dwFlags,
									  DWORD_PTR dwContext);
extern HINTERNET (WINAPI* pInternetOpenUrlA)(HINTERNET hInternet,
									  LPCSTR lpszUrl,
									  LPCSTR lpszHeaders,
									  DWORD dwHeadersLength,
									  DWORD dwFlags,
									  DWORD_PTR dwContext);
HINTERNET WINAPI hooked_InternetOpenUrlA(HINTERNET hInternet,
									  LPCSTR lpszUrl,
									  LPCSTR lpszHeaders,
									  DWORD dwHeadersLength,
									  DWORD dwFlags,
									  DWORD_PTR dwContext);
extern HINTERNET (WINAPI* pInternetOpenUrlW)(HINTERNET hInternet,
									  LPCWSTR lpszUrl,
									  LPCWSTR lpszHeaders,
									  DWORD dwHeadersLength,
									  DWORD dwFlags,
									  DWORD_PTR dwContext);
HINTERNET WINAPI hooked_InternetOpenUrlW(HINTERNET hInternet,
									  LPCWSTR lpszUrl,
									  LPCWSTR lpszHeaders,
									  DWORD dwHeadersLength,
									  DWORD dwFlags,
									  DWORD_PTR dwContext);
extern HINTERNET (WINAPI* pHttpOpenRequestA)(HINTERNET hConnect,
									 LPCSTR lpszVerb,
									 LPCSTR lpszObjectName,
									 LPCSTR lpszVersion,
									 LPCSTR lpszReferer,
									 LPCSTR* lplpszAcceptTypes,
									 DWORD dwFlags,
									 DWORD_PTR dwContext);
HINTERNET WINAPI hooked_HttpOpenRequestA(HINTERNET hConnect,
									 LPCSTR lpszVerb,
									 LPCSTR lpszObjectName,
									 LPCSTR lpszVersion,
									 LPCSTR lpszReferer,
									 LPCSTR* lplpszAcceptTypes,
									 DWORD dwFlags,
									 DWORD_PTR dwContext);
extern HINTERNET (WINAPI* pHttpOpenRequestW)(HINTERNET hConnect,
									 LPCWSTR lpszVerb,
									 LPCWSTR lpszObjectName,
									 LPCWSTR lpszVersion,
									 LPCWSTR lpszReferer,
									 LPCWSTR* lplpszAcceptTypes,
									 DWORD dwFlags,
									 DWORD_PTR dwContext);
HINTERNET WINAPI hooked_HttpOpenRequestW(HINTERNET hConnect,
									 LPCWSTR lpszVerb,
									 LPCWSTR lpszObjectName,
									 LPCWSTR lpszVersion,
									 LPCWSTR lpszReferer,
									 LPCWSTR* lplpszAcceptTypes,
									 DWORD dwFlags,
									 DWORD_PTR dwContext);
extern BOOL (WINAPI* pHttpAddRequestHeadersA)(HINTERNET hRequest,
									   LPCSTR lpszHeaders,
									   DWORD dwHeadersLength,
									   DWORD dwModifiers);
BOOL WINAPI hooked_HttpAddRequestHeadersA(HINTERNET hRequest,
									   LPCSTR lpszHeaders,
									   DWORD dwHeadersLength,
									   DWORD dwModifiers);
extern BOOL (WINAPI* pHttpAddRequestHeadersW)(HINTERNET hRequest,
									   LPCWSTR lpszHeaders,
									   DWORD dwHeadersLength,
									   DWORD dwModifiers);
BOOL WINAPI hooked_HttpAddRequestHeadersW(HINTERNET hRequest,
									   LPCWSTR lpszHeaders,
									   DWORD dwHeadersLength,
									   DWORD dwModifiers);
extern BOOL (WINAPI* pHttpQueryInfoA)(HINTERNET hRequest,
							   DWORD dwInfoLevel,
							   LPVOID lpvBuffer,
							   LPDWORD lpdwBufferLength,
							   LPDWORD lpdwIndex);
BOOL WINAPI hooked_HttpQueryInfoA(HINTERNET hRequest,
							   DWORD dwInfoLevel,
							   LPVOID lpvBuffer,
							   LPDWORD lpdwBufferLength,
							   LPDWORD lpdwIndex);
extern BOOL (WINAPI* pHttpQueryInfoW)(HINTERNET hRequest,
							   DWORD dwInfoLevel,
							   LPVOID lpvBuffer,
							   LPDWORD lpdwBufferLength,
							   LPDWORD lpdwIndex);
BOOL WINAPI hooked_HttpQueryInfoW(HINTERNET hRequest,
							   DWORD dwInfoLevel,
							   LPVOID lpvBuffer,
							   LPDWORD lpdwBufferLength,
							   LPDWORD lpdwIndex);
extern BOOL (WINAPI* pHttpSendRequestA)(HINTERNET hRequest,
								LPCSTR lpszHeaders,
								DWORD dwHeadersLength,
								LPVOID lpOptional,
								DWORD dwOptionalLength);
BOOL WINAPI hooked_HttpSendRequestA(HINTERNET hRequest,
								LPCSTR lpszHeaders,
								DWORD dwHeadersLength,
								LPVOID lpOptional,
								DWORD dwOptionalLength);
extern BOOL (WINAPI* pHttpSendRequestW)(HINTERNET hRequest,
								LPCWSTR lpszHeaders,
								DWORD dwHeadersLength,
								LPVOID lpOptional,
								DWORD dwOptionalLength);
BOOL WINAPI hooked_HttpSendRequestW(HINTERNET hRequest,
								LPCWSTR lpszHeaders,
								DWORD dwHeadersLength,
								LPVOID lpOptional,
								DWORD dwOptionalLength);
extern BOOL (WINAPI* pHttpSendRequestExA)(HINTERNET hRequest,
								   LPINTERNET_BUFFERSA lpBuffersIn,
								   LPINTERNET_BUFFERSA lpBuffersOut,
								   DWORD dwFlags,
								   DWORD_PTR dwContext);
BOOL WINAPI hooked_HttpSendRequestExA(HINTERNET hRequest,
								   LPINTERNET_BUFFERSA lpBuffersIn,
								   LPINTERNET_BUFFERSA lpBuffersOut,
								   DWORD dwFlags,
								   DWORD_PTR dwContext);
extern BOOL (WINAPI* pHttpSendRequestExW)(HINTERNET hRequest,
								   LPINTERNET_BUFFERSW lpBuffersIn,
								   LPINTERNET_BUFFERSW lpBuffersOut,
								   DWORD dwFlags,
								   DWORD_PTR dwContext);
BOOL WINAPI hooked_HttpSendRequestExW(HINTERNET hRequest,
								   LPINTERNET_BUFFERSW lpBuffersIn,
								   LPINTERNET_BUFFERSW lpBuffersOut,
								   DWORD dwFlags,
								   DWORD_PTR dwContext);
// Make sure we debug this function heavily.
extern BOOL (WINAPI* pInternetReadFile)(HINTERNET hFile,
								 LPVOID lpBuffer,
								 DWORD dwNumberOfBytesToRead,
								 LPDWORD lpdwNumberOfBytesRead);
BOOL WINAPI hooked_InternetReadFile(HINTERNET hFile,
								 LPVOID lpBuffer,
								 DWORD dwNumberOfBytesToRead,
								 LPDWORD lpdwNumberOfBytesRead);
extern BOOL (WINAPI* pInternetReadFileExA)(HINTERNET hFile,
									LPINTERNET_BUFFERSA lpBuffersOut,
									DWORD dwFlags,
									DWORD_PTR dwContext);
BOOL WINAPI hooked_InternetReadFileExA(HINTERNET hFile,
									LPINTERNET_BUFFERSA lpBuffersOut,
									DWORD dwFlags,
									DWORD_PTR dwContext);
extern BOOL (WINAPI* pInternetReadFileExW)(HINTERNET hFile,
									LPINTERNET_BUFFERSW lpBuffersOut,
									DWORD dwFlags,
									DWORD_PTR dwContext);
BOOL WINAPI hooked_InternetReadFileExW(HINTERNET hFile,
									LPINTERNET_BUFFERSW lpBuffersOut,
									DWORD dwFlags,
									DWORD_PTR dwContext);


int WINAPI hooked_connect(SOCKET s,
					   const struct sockaddr* name,
					   int namelen);

int WINAPI hooked_bind(SOCKET s,
					const struct sockaddr* name,
					int namelen);

int WINAPI hooked_send(SOCKET s,
				   const char* buf,
				   int len,
				   int flags);

int WINAPI hooked_recv(SOCKET s,
					char* buf,
					int len,
					int flags);

int WINAPI hooked_WSAConnect(SOCKET s,
						const struct sockaddr* name,
						int namelen,
						LPWSABUF lpCallerData,		// not supported in TCP/IP
						LPWSABUF lpCalleeData,		// not supported in TCP/IP
						LPQOS lpSQOS,				// unimportant
						LPQOS lpGQOS);				// reserved

int WINAPI hooked_WSASend(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesSent,
					   DWORD dwFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

int WINAPI hooked_WSARecv(SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesRecvd,
					   LPDWORD lpFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
#endif