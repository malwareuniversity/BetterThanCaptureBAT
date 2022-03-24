// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <WinSock2.h>
#include <WinInet.h>
#include <WS2tcpip.h>
#include <windows.h>
//#include <winhttp.h> Can't use with wininet :(
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include "detours.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "detours.lib")