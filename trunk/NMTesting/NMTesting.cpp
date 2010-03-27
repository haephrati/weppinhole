// NMTesting.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include <locale.h>
#include <conio.h>
#include <time.h>
#include "NMApi.h"
#include "AirDump.h"

HANDLE myNplParser;
HANDLE myFrameParserConfig;
HANDLE myFrameParser;

ULONG myHTTPFilterID;
ULONG myHTTPFieldID;

BOOL CaptureDone = FALSE;
ULONG FrameCount = 0;

// Frame indication callback called each time a frame appears.
void __stdcall 
myFrameIndication(HANDLE hCapEng, ULONG AdpIdx, PVOID pContext, HANDLE hRawFrame)
{
	//HANDLE capFile = (HANDLE)pContext;
	//NmAddFrame(capFile, hRawFrame);

	BYTE szBuffer[4096];
	ULONG nRead = 0;
	NmGetRawFrame( hRawFrame, sizeof(szBuffer), szBuffer, &nRead );
	// printf( "%02x %02x %02x %02x - %ld\n", szBuffer[0], szBuffer[1], szBuffer[2], szBuffer[3], nRead );

#pragma  pack( push, 1 )
	struct header
	{
		unsigned	char	version;
		unsigned	short	length;
		unsigned	int		opmode;
		unsigned	int		flags;
		unsigned	int		phytype;
		unsigned	int		channel;
					int		lRSSI;
		unsigned	char	rate;
		unsigned	long	tv_low;
		unsigned	long	tv_high;
	};
#pragma pack( pop )

	header *phdr = (header*)szBuffer;

	short len =phdr->length;
	unsigned char *h80211 = (unsigned char*)szBuffer + len;

	//printf( "version %d\n", phdr->version );
	//printf( "length %d\n", len );

	dump_add_packet( *(global*)pContext, h80211, nRead - len );
	FrameCount++;
}

char *MediumName[] =
{
	"NdisMedium802_3",
	"NdisMedium802_5",
	"NdisMediumFddi",
	"NdisMediumWan",
	"NdisMediumLocalTalk",
	"NdisMediumDix",              // defined for convenience, not a real medium
	"NdisMediumArcnetRaw",
	"NdisMediumArcnet878_2",
	"NdisMediumAtm",
	"NdisMediumWirelessWan",
	"NdisMediumIrda",
	"NdisMediumBpc",
	"NdisMediumCoWan",
	"NdisMedium1394",
	"NdisMediumInfiniBand",
#if ((NTDDI_VERSION >= NTDDI_LONGHORN) || NDIS_SUPPORT_NDIS6)
	"NdisMediumTunnel",
	"NdisMediumNative802_11",
	"NdisMediumLoopback",
#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)
};

char *PhysicalMediumName[] = 
{
	"NdisPhysicalMediumUnspecified",
	"NdisPhysicalMediumWirelessLan",
	"NdisPhysicalMediumCableModem",
	"NdisPhysicalMediumPhoneLine",
	"NdisPhysicalMediumPowerLine",
	"NdisPhysicalMediumDSL",      // includes ADSL and UADSL (G.Lite)
	"NdisPhysicalMediumFibreChannel",
	"NdisPhysicalMedium1394",
	"NdisPhysicalMediumWirelessWan",
	"NdisPhysicalMediumNative802_11",
	"NdisPhysicalMediumBluetooth",
	"NdisPhysicalMediumInfiniband",
	"NdisPhysicalMediumWiMax",
	"NdisPhysicalMediumUWB",
	"NdisPhysicalMedium802_3",
	"NdisPhysicalMedium802_5",
	"NdisPhysicalMediumIrda",
	"NdisPhysicalMediumWiredWAN",
	"NdisPhysicalMediumWiredCoWan",
	"NdisPhysicalMediumOther",
};

void cls( HANDLE hConsole )
{   
	COORD   coordScreen   =   {   0,   0   };
	BOOL   bSuccess;   
	DWORD   cCharsWritten;   
	CONSOLE_SCREEN_BUFFER_INFO   csbi;
	DWORD   dwConSize;

	bSuccess   =   GetConsoleScreenBufferInfo( hConsole, &csbi );   
	dwConSize  =   csbi.dwSize.X * csbi.dwSize.Y;

	bSuccess   =   FillConsoleOutputCharacter( hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten );   

	bSuccess   =   GetConsoleScreenBufferInfo( hConsole, &csbi );

	bSuccess   =   FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );

	bSuccess   =   SetConsoleCursorPosition( hConsole, coordScreen );
	return;   
}   

int __cdecl wmain(int argc, WCHAR* argv[])
{
	ULONG ret;
	ULONG adapterIndex = 0;

	if( argc > 1 )
	{
		return 0;
	}

	setlocale( LC_ALL, "chs" );
	HANDLE myCaptureEngine;
	ret = NmOpenCaptureEngine(&myCaptureEngine);
	if(ret != ERROR_SUCCESS)
	{
		wprintf(L"Error openning capture engine, 0x%X\n", ret);
		return ret;
	}

	ULONG adapterCount;
	NM_NIC_ADAPTER_INFO AdapterInfo;
	AdapterInfo.Size = sizeof(AdapterInfo);

	ret = NmGetAdapterCount( myCaptureEngine, &adapterCount );
	if( ret != ERROR_SUCCESS || adapterCount == 0 )
	{
		printf("no adapter.\n");
		_getch();
		return ret;
	}

	int n80211 = 0;
	for( ULONG i = 0; i < adapterCount; ++i )
	{
		NmGetAdapter( myCaptureEngine, i, &AdapterInfo );
		if( AdapterInfo.MediumType == NdisMediumNative802_11 && AdapterInfo.PhysicalMediumType == NdisPhysicalMediumNative802_11 )
		{
			printf( 
				"AdapterIndex :%d\n"
				"PermanentAddr : %02X:%02X:%02X:%02X:%02X:%02X\n" 
				"CurrentAddr : %02X:%02X:%02X:%02X:%02X:%02X\n"
				"ConnectionName : %S\n"
				"FriendlyName : %S\n"
				"MediumType : %s\n"
				"PhysicalMediumType : %s\n\n"
				, i
				, AdapterInfo.PermanentAddr[0],AdapterInfo.PermanentAddr[1],AdapterInfo.PermanentAddr[2]
				, AdapterInfo.PermanentAddr[3],AdapterInfo.PermanentAddr[4],AdapterInfo.PermanentAddr[5]
				, AdapterInfo.CurrentAddr[0],AdapterInfo.CurrentAddr[1],AdapterInfo.CurrentAddr[2]
				, AdapterInfo.CurrentAddr[3],AdapterInfo.CurrentAddr[4],AdapterInfo.CurrentAddr[5]
				, AdapterInfo.ConnectionName
				, AdapterInfo.FriendlyName
				, MediumName[AdapterInfo.MediumType]
				, PhysicalMediumName[AdapterInfo.PhysicalMediumType]
				);
			++n80211;
		}
	}

	if( n80211 == 0 )
	{
		puts( "Cannot found wifi card." );
		return -1;
	}

	TCHAR szBuffer[256];
	DWORD dwRead = 0;
	if( ReadConsole( GetStdHandle(STD_INPUT_HANDLE), szBuffer, _countof(szBuffer), &dwRead, NULL ) == FALSE )
	{
		puts( "error input." );
		return -1;
	}

	if( dwRead == 0 )
	{
		puts( "cancel input." );
		return -1;
	}

	szBuffer[dwRead] = 0;
	ULONG nChoice = _ttol( szBuffer );

	//int nChoice = 0;
	if( nChoice > adapterCount )
	{
		puts( "error input." );
		return -1;
	}

	global G;
	memset( &G, 0, sizeof(global) );
	ret = NmConfigAdapter(myCaptureEngine, nChoice, myFrameIndication, (void*)&G );
	if(ret != ERROR_SUCCESS)
	{
		wprintf(L"Error configuring the adapter.\n");
		NmCloseHandle(myCaptureEngine);
		return ret;
	}

	NmStartCapture( myCaptureEngine, nChoice, NmPromiscuous );

	clock_t c = clock();
	while( true )
	{
		if( _kbhit() )
		{
			char ch = _getch();
			if( ch == 27 )
				break;
		}
		if( clock() - c > 2000 )
		{
			cls( GetStdHandle( STD_OUTPUT_HANDLE ) );

			COORD POS;
			POS.X = 0;
			POS.Y = 0;
			SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), POS );
			printf( "capture frame %ld\n", FrameCount );
			printf( "%16s|%4s|%3s|%3s(M)|%6s|%6s|%6s|%8s|%s", "ESSID", "CHAN", "POW", "SPD", "PACKET", "MGMT", "DATA", "SECURITY", "BSSID" );

			POS.Y = 2;
			apinfo * ap = aplst;
			while( ap )
			{
				++POS.Y;
				SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), POS );

				char szMac[32];
				_snprintf( szMac, sizeof(szMac), "%02x-%02x-%02x %02x-%02x-%02x", ap->bssid[0], ap->bssid[1], ap->bssid[2], ap->bssid[3], ap->bssid[4], ap->bssid[5] );
				printf( "%16s %4d %3d %5dM %6d %6d %6d %8s \n%s %d/%d %s",
					ap->essid, ap->channel, ap->power, ap->max_speed, ap->pkt, ap->bcn, ap->nb_data, (ap->security&STD_WEP?"WEP":"OTHER"), 
					szMac, ap->nb_ivs_clean, ap->nb_ivs_vague, ap->key );
				ap = ap->next;
			}

			c = clock();
		}
		Sleep(1);
	}
	NmStopCapture( myCaptureEngine, nChoice );
	NmCloseHandle( myCaptureEngine );

	apinfo * ap = aplst;
	while( ap )
	{
		apinfo *aptmp = ap;
		ap = ap->next;

		if( SUCCESS == crack_wep_ptw( aptmp ) )
		{
			puts( (const char*)aptmp->key );
			_getch();
		}
		fclose( aptmp->ivs );
		uniqueiv_wipe( aptmp->uiv_root );
		free( aptmp );
	}
	return 0;
}
