// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

#include <Ntddndis.h>
#include <ObjBase.h>

// TODO: reference additional headers your program requires here
#include <Winsock2.h>
#pragma comment( lib, "ws2_32.lib")
#include "uniqueiv.h"
#include <memory.h>
