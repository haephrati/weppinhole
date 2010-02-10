// WepPinhole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include <wlanapi.h>
#include <rpcsal.h>
#include <Rpc.h>

#include "ndisPort.h"

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

int TestWlanApi(int argc, _TCHAR* argv[])
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

	BOOL bRet = ReadFile( hClient, szBuffer, sizeof(szBuffer), &dwRead, NULL );

	_getch();
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

TCHAR            NdisProtDevice[] = _T("\\\\.\\\\WifiCapture");
TCHAR *          pNdisProtDevice = &NdisProtDevice[0];

BOOL InitializeDevice( HANDLE hDeviceHandle, CONST WCHAR* pszDeviceName, UINT nNameLength )
{
	DWORD BytesReturned = 0;
	BOOL bRet = DeviceIoControl(
		hDeviceHandle,
		IOCTL_NDISPROT_OPEN_DEVICE,
		(LPVOID)pszDeviceName,
		nNameLength,
		NULL,
		0,
		&BytesReturned,
		NULL);

	if( bRet == FALSE )
	{
		puts( "Open Device failed." );
		return FALSE;
	}

	//{
	//	UCHAR SetBuffer[sizeof(NDISPROT_SET_OID)+sizeof(ULONG)];

	//	PNDISPROT_SET_OID pSetOid = (PNDISPROT_SET_OID)&SetBuffer[0];
	//	pSetOid->Oid = OID_GEN_CURRENT_PACKET_FILTER;
	//	pSetOid->PortNumber = 0;
	//	ULONG* pFilter = (ULONG*)pSetOid->Data;
	//	*pFilter = 
	//		NDIS_PACKET_TYPE_DIRECTED |
	//		NDIS_PACKET_TYPE_MULTICAST |
	//		NDIS_PACKET_TYPE_BROADCAST |
	//		NDIS_PACKET_TYPE_PROMISCUOUS |
	//		NDIS_PACKET_TYPE_802_11_RAW_DATA |
	//		//NDIS_PACKET_TYPE_802_11_PROMISCUOUS_MGMT |
	//		//NDIS_PACKET_TYPE_802_11_PROMISCUOUS_CTRL |
	//		//NDIS_PACKET_TYPE_802_11_BROADCAST_MGMT |
	//		//NDIS_PACKET_TYPE_802_11_BROADCAST_CTRL |
	//		//NDIS_PACKET_TYPE_802_11_MULTICAST_MGMT |
	//		//NDIS_PACKET_TYPE_802_11_ALL_MULTICAST_MGMT |
	//		//NDIS_PACKET_TYPE_802_11_DIRECTED_MGMT |
	//		//NDIS_PACKET_TYPE_802_11_DIRECTED_CTRL
	//		0;

	//	if( FALSE == (BOOLEAN)DeviceIoControl(
	//		hDeviceHandle,
	//		IOCTL_NDISPROT_SET_OID_VALUE,
	//		(LPVOID)&SetBuffer[0],
	//		sizeof(SetBuffer),
	//		(LPVOID)&SetBuffer[0],
	//		sizeof(SetBuffer),
	//		&BytesReturned,
	//		NULL) )
	//	{
	//		puts( "set filter failed!" );
	//		return FALSE;
	//	}
	//}

	{
		UCHAR SetBuffer[sizeof(NDISPROT_SET_OID)+sizeof(ULONG)];
		PNDISPROT_SET_OID pSetOid = (PNDISPROT_SET_OID)&SetBuffer[0];
		pSetOid->Oid = OID_DOT11_CURRENT_OPERATION_MODE;
		pSetOid->PortNumber = 0;
		PDOT11_CURRENT_OPERATION_MODE pMode = (PDOT11_CURRENT_OPERATION_MODE)pSetOid->Data;
		pMode->uReserved = 0;
		pMode->uCurrentOpMode = DOT11_OPERATION_MODE_NETWORK_MONITOR;

		if( FALSE == (BOOLEAN)DeviceIoControl(
			hDeviceHandle,
			IOCTL_NDISPROT_SET_OID_VALUE,
			(LPVOID)&SetBuffer[0],
			sizeof(SetBuffer),
			(LPVOID)&SetBuffer[0],
			sizeof(SetBuffer),
			&BytesReturned,
			NULL) )
		{
			puts( "enter monitor mode failed." );
			return FALSE;
		}

	}

	return TRUE;
}

VOID TerminateDevice( HANDLE hDeviceHandle )
{
	DWORD BytesReturned = 0;
	UCHAR SetBuffer[sizeof(NDISPROT_SET_OID)+sizeof(DOT11_CURRENT_OPERATION_MODE)];

	PNDISPROT_SET_OID pSetOid = (PNDISPROT_SET_OID)&SetBuffer[0];
	pSetOid->Oid = OID_DOT11_CURRENT_OPERATION_MODE;
	pSetOid->PortNumber = 0;
	PDOT11_CURRENT_OPERATION_MODE pMode = (PDOT11_CURRENT_OPERATION_MODE)pSetOid->Data;
	pMode->uCurrentOpMode = DOT11_OPERATION_MODE_EXTENSIBLE_STATION;
	pMode->uReserved = 0;

	BOOL bSuccess = (BOOLEAN)DeviceIoControl(
		hDeviceHandle,
		IOCTL_NDISPROT_SET_OID_VALUE,
		(LPVOID)&SetBuffer[0],
		sizeof(SetBuffer),
		(LPVOID)&SetBuffer[0],
		sizeof(SetBuffer),
		&BytesReturned,
		NULL);

	CloseHandle( hDeviceHandle );

}

int _tmain(int argc, _TCHAR* argv[])
{
	TestWlanApi( argc, argv );

	HANDLE DeviceHandle = OpenHandle(pNdisProtDevice);

	if( DeviceHandle == INVALID_HANDLE_VALUE )
	{
		_tprintf((_T("Failed to open %s\n"), pNdisProtDevice));
		return -1;
	}

	wstring strDeviceName[256][2];
	int nCount = EnumerateDevices( DeviceHandle, strDeviceName );
	if( nCount == 0 )
	{
		puts( "no device bind." );
		return -1;
	}

	for( int i = 0; i < nCount; ++i )
	{
		printf( "%2d.%S\n	%S\n\n", i, strDeviceName[i][0].c_str(), strDeviceName[i][1].c_str() );
	}
	TCHAR szBuffer[256];
	DWORD dwRead = 0;
	//if( ReadConsole( GetStdHandle(STD_INPUT_HANDLE), szBuffer, _countof(szBuffer), &dwRead, NULL ) == FALSE )
	//{
	//	puts( "error input" );
	//	return -1;
	//}
	//szBuffer[dwRead] = 0;
	//int nChoice = _ttoi( szBuffer );

	int nChoice = 0;
	if( nChoice > nCount )
	{
		puts( "error input." );
		return -1;
	}

	InitializeDevice( DeviceHandle, strDeviceName[nChoice][0].c_str(), strDeviceName[nChoice][0].length()*sizeof(WCHAR) );

	for( int i = 0; i < 3; ++i )
	{
		if( ReadFile( DeviceHandle, szBuffer, sizeof(szBuffer), &dwRead, NULL ) )
		{

		}
	}
	TerminateDevice( DeviceHandle );
}