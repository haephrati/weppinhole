// WepPinhole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include <wlanapi.h>
#include <rpcsal.h>
#include <Rpc.h>

#define BUFSIZE 0x3E8000
#define print printf
struct cstate
{
	HMODULE cs_lib;
	int		cs_chan;
	char	(__cdecl *cs_F1)(int Code);
	char	(__cdecl *cs_F2)(void);
	char	(__cdecl *cs_T1)(int Size, unsigned char *Buffer);
	char	(__cdecl *cs_CC)(int Channel);
	char	(__cdecl *cs_S1)(int Channel);
	int		(__cdecl *cs_S5)(unsigned char *Buffer, int Length);
	int		(__cdecl *cs_GN)(wchar_t *);
	int		(*cs_SC)(int band);
}cs;

static int do_init_lib(struct cstate *cs)
{
	/* init */
	if (!cs->cs_F1(BUFSIZE))
		return print("F1");

	/* start monitor */
	if (!cs->cs_S1(cs->cs_chan))
		return print("S1");

	/* change chan */
	if (!cs->cs_CC(cs->cs_chan))
		return print("CC");

	return 0;
}

static int init_lib(struct cstate *cs)
{
	char *lib = "ca2k.dll";
	HMODULE ca2k_dll;

	cs->cs_chan = 6;
	ca2k_dll = LoadLibraryA( lib );
	if (!ca2k_dll)
		return print("dlopen(%s)", lib);
	cs->cs_lib = ca2k_dll;

	// Initialise
	cs->cs_F1 = reinterpret_cast< char (__cdecl *)(int) >( GetProcAddress(ca2k_dll, "F1") );
	// Transmit
	cs->cs_T1 = reinterpret_cast< char (__cdecl *)(int, unsigned char *) >( GetProcAddress(ca2k_dll, "T1") );
	// Change monitoring channel
	cs->cs_CC = reinterpret_cast< char (__cdecl *)(int) > ( GetProcAddress(ca2k_dll, "CC") );
	// Start monitoring
	cs->cs_S1 =  reinterpret_cast< char (__cdecl *)(int) >( GetProcAddress(ca2k_dll, "S1") );
	// Read packets
	cs->cs_S5 = reinterpret_cast< int (__cdecl *)(unsigned char *, int) >( GetProcAddress(ca2k_dll, "S5") );
	// Finalize
	cs->cs_F2 = reinterpret_cast< char (__cdecl *)(void) >( GetProcAddress(ca2k_dll, "F2") );
	// Get Adapter Name 
	cs->cs_GN =  reinterpret_cast< int (__cdecl *)(wchar_t *) >( GetProcAddress(ca2k_dll, "GN") );

	if (!(cs->cs_F1 && cs->cs_T1 && cs->cs_CC && cs->cs_S1 && cs->cs_S5
		&& cs->cs_F2 && cs->cs_GN))
		return print("Can't find syms");

	return do_init_lib(cs);
}

// open a WLAN client handle and check version
DWORD
OpenHandleAndCheckVersion(
						  PHANDLE phClient
						  )
{
	DWORD dwError = ERROR_SUCCESS;
	DWORD dwServiceVersion;
	HANDLE hClient = NULL;

	__try
	{
		*phClient = NULL;

		// open a handle to the service
		if ((dwError = WlanOpenHandle(
			WLAN_API_VERSION,
			NULL,               // reserved
			&dwServiceVersion,
			&hClient
			)) != ERROR_SUCCESS)
		{
			__leave;
		}

		// check service version
		if (WLAN_API_VERSION_MAJOR(dwServiceVersion) < WLAN_API_VERSION_MAJOR(WLAN_API_VERSION_2_0))
		{
			// No-op, because the version check is for demonstration purpose only.
			// You can add your own logic here.
		}

		*phClient = hClient;

		// set hClient to NULL so it will not be closed
		hClient = NULL;
	}
	__finally
	{
		if (hClient != NULL)
		{
			// clean up
			WlanCloseHandle(
				hClient, 
				NULL            // reserved
				);
		}
	}

	return dwError;
}

// enumerate wireless interfaces
UINT EnumInterface( HANDLE hClient, WLAN_INTERFACE_INFO sInfo[64] )
{
	DWORD dwError = ERROR_SUCCESS;
	PWLAN_INTERFACE_INFO_LIST pIntfList = NULL;
	UINT i = 0;

	__try
	{
		// enumerate wireless interfaces
		if ((dwError = WlanEnumInterfaces(
			hClient,
			NULL,               // reserved
			&pIntfList
			)) != ERROR_SUCCESS)
		{
			__leave;
		}

		// print out interface information
		for (i = 0; i < pIntfList->dwNumberOfItems; i++)
		{
			memcpy( &sInfo[i], &pIntfList->InterfaceInfo[i], sizeof(WLAN_INTERFACE_INFO) );
		}

		return pIntfList->dwNumberOfItems;
	}
	__finally
	{
		// clean up
		if (pIntfList != NULL)
		{
			WlanFreeMemory(pIntfList);
		}
	}
	return 0;
}

// get interface state string
LPWSTR
GetInterfaceStateString( __in WLAN_INTERFACE_STATE wlanInterfaceState )
{
	LPWSTR strRetCode;

	switch(wlanInterfaceState)
	{
	case wlan_interface_state_not_ready:
		strRetCode = L"\"not ready\"";
		break;
	case wlan_interface_state_connected:
		strRetCode = L"\"connected\"";
		break;
	case wlan_interface_state_ad_hoc_network_formed:
		strRetCode = L"\"ad hoc network formed\"";
		break;
	case wlan_interface_state_disconnecting:
		strRetCode = L"\"disconnecting\"";
		break;
	case wlan_interface_state_disconnected:
		strRetCode = L"\"disconnected\"";
		break;
	case wlan_interface_state_associating:
		strRetCode = L"\"associating\"";
		break;
	case wlan_interface_state_discovering:
		strRetCode = L"\"discovering\"";
		break;
	case wlan_interface_state_authenticating:
		strRetCode = L"\"authenticating\"";
		break;
	default:
		strRetCode = L"\"invalid interface state\"";
	}

	return strRetCode;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// init_lib( &cs );
	// return 0;
	HANDLE hClient = NULL;
	WLAN_INTERFACE_INFO sInfo[64];
	RPC_CSTR strGuid = NULL;

	TCHAR szBuffer[256];
	DWORD dwRead;
	if( OpenHandleAndCheckVersion( &hClient ) != ERROR_SUCCESS )
		return -1;

	UINT nCount = EnumInterface( hClient, sInfo );
	for( UINT i = 0; i < nCount; ++i )
	{
		if (UuidToStringA( &sInfo[i].InterfaceGuid, &strGuid) == RPC_S_OK)
		{
			printf( ("%d. %s\n\tDescription: %S\n\tState: %S\n"), 
				i, 
				strGuid, 
				sInfo[i].strInterfaceDescription, 
				GetInterfaceStateString(sInfo[i].isState) );

			RpcStringFreeA(&strGuid);
		}
	}

	UINT nChoice = 0;
	printf( "for choice wireless card:" );

	if( ReadConsole( GetStdHandle(STD_INPUT_HANDLE), szBuffer, _countof(szBuffer), &dwRead, NULL ) == FALSE )
	{
		puts( "error input" );
		return -1;
	}
	szBuffer[dwRead] = 0;
	nChoice = _ttoi( szBuffer );

	if( nChoice > nCount )
	{
		puts( "error input." );
		return -1;
	}

	ULONG ulOperatorCode = DOT11_OPERATION_MODE_NETWORK_MONITOR;
	if( ERROR_SUCCESS != WlanSetInterface( 
		hClient, 
		&sInfo[nChoice].InterfaceGuid, 
		wlan_intf_opcode_current_operation_mode,
		sizeof(ULONG),
		&ulOperatorCode,
		NULL ) )
	{
		puts( "enter monitor mode failed!" );
		return -1;
	}

	ControlIoDevice( 
	ulOperatorCode = DOT11_OPERATION_MODE_EXTENSIBLE_STATION;
	if( ERROR_SUCCESS != WlanSetInterface( 
		hClient, 
		&sInfo[nChoice].InterfaceGuid, 
		wlan_intf_opcode_current_operation_mode,
		sizeof(ULONG),
		&ulOperatorCode,
		NULL ) )
	{
		puts( "enter monitor mode failed!" );
		return -1;
	}

	WlanCloseHandle( hClient, NULL );
	return 0;
}

