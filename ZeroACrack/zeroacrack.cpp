#include <Windows.h>
#include <Ntddndis.h>
#include <ObjBase.h>

#include <stdio.h>
#include <locale.h>
#include <conio.h>
#include <time.h>
#include "NMApi.h"
#include "Commonlib.h"

#include "def.h"
//////////////////////////////////////////////////////////////////////////
// typedef
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
//////////////////////////////////////////////////////////////////////////

ULONG uFrameCount = 0;

//////////////////////////////////////////////////////////////////////////
extern void SetProfile( GUID guidIntf, char *essid, char *pwd );
extern void dumpacket( char *packet, int len );
extern void freenetwork( network* network_ptr );
//////////////////////////////////////////////////////////////////////////
extern network * networktable;

// Frame indication callback called each time a frame appears.
void __stdcall myFrameIndication(HANDLE hCapEng, ULONG AdpIdx, PVOID pContext, HANDLE hRawFrame)
{
	//HANDLE capFile = (HANDLE)pContext;
	//NmAddFrame(capFile, hRawFrame);

	BYTE szBuffer[1024*32];
	ULONG nRead = 0;
	NmGetRawFrame( hRawFrame, sizeof(szBuffer), szBuffer, &nRead );
	// printf( "%02x %02x %02x %02x - %ld\n", szBuffer[0], szBuffer[1], szBuffer[2], szBuffer[3], nRead );

	if( nRead >= sizeof(szBuffer) )
		return;

	header *phdr = (header*)szBuffer;

	short len =phdr->length;
	char *h80211 = (char*)szBuffer + len;

	//printf( "version %d\n", phdr->version );
	//printf( "length %d\n", len );

	dumpacket( h80211, nRead - len );
	uFrameCount++;
}

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

int main( int argc, char **argv )
{
	InstallCrashHandle();

	ULONG ret;
	ULONG adapterIndex = 0;

	setlocale( LC_ALL, "chs" );
	HANDLE myCaptureEngine;
	ret = NmOpenCaptureEngine(&myCaptureEngine);
	if(ret != ERROR_SUCCESS)
	{
		printf( "Error openning capture engine, 0x%X\n", ret);
		return ret;
	}

	ULONG adapterCount;
	NM_NIC_ADAPTER_INFO AdapterInfo;
	AdapterInfo.Size = sizeof(AdapterInfo);

	ret = NmGetAdapterCount( myCaptureEngine, &adapterCount );
	if( ret != ERROR_SUCCESS || adapterCount == 0 )
	{
		printf("no adapter.\n");
		return ret;
	}

	int n80211 = 0;
	int nMenu[128];
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
				"GUID : %S\n"
				//"FriendlyName : %S\n"
				//"MediumType : %s\n"
				//"PhysicalMediumType : %s\n\n"
				, n80211
				, AdapterInfo.PermanentAddr[0],AdapterInfo.PermanentAddr[1],AdapterInfo.PermanentAddr[2]
				, AdapterInfo.PermanentAddr[3],AdapterInfo.PermanentAddr[4],AdapterInfo.PermanentAddr[5]
				, AdapterInfo.CurrentAddr[0],AdapterInfo.CurrentAddr[1],AdapterInfo.CurrentAddr[2]
				, AdapterInfo.CurrentAddr[3],AdapterInfo.CurrentAddr[4],AdapterInfo.CurrentAddr[5]
				, AdapterInfo.ConnectionName
				, AdapterInfo.Guid
				//, AdapterInfo.FriendlyName
				//, MediumName[AdapterInfo.MediumType]
				//, PhysicalMediumName[AdapterInfo.PhysicalMediumType]
			);
			nMenu[n80211] = i;
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
	long nChoice = _ttol( szBuffer );

	//int nChoice = 0;
	if( nChoice > n80211 )
	{
		puts( "error input." );
		return -1;
	}

	nChoice = nMenu[n80211];
	NmGetAdapter( myCaptureEngine, nChoice, &AdapterInfo );

	WCHAR szGuid[MAX_PATH];
	WCHAR *beg = wcschr( AdapterInfo.Guid, '{' ) + 1;
	WCHAR *end = wcschr( AdapterInfo.Guid, '}' );
	wcsncpy( szGuid, beg, end - beg );
	szGuid[end-beg] = 0;

	GUID guidIntf;
	// get the interface GUID
	if (UuidFromStringW((RPC_WSTR)szGuid, &guidIntf) != RPC_S_OK)
	{
		puts( "guid got error." );
		return 0;
	}

	ret = NmConfigAdapter(myCaptureEngine, nChoice, myFrameIndication, NULL );
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
			printf( "Capture frame : %ld\n", uFrameCount );
			printf( "%17s", "ESSID" );

			POS.Y = 2;
			network * ap = networktable;
			while( ap )
			{
				if( ap->security & STD_WEP )
				{
					SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), POS );
					puts("------------------------------------------------------------");

					char szMac[32];
					char szKeyHex[256];
					char szKeyAscii[64];
					_snprintf( szMac, sizeof(szMac), "%02x-%02x-%02x %02x-%02x-%02x", ap->bssid[0], ap->bssid[1], ap->bssid[2], ap->bssid[3], ap->bssid[4], ap->bssid[5] );
					int n = 0;
					for( int i = 0; ap->key[i] != 0 && i < 13; ++i )
					{
						n += _snprintf( szKeyHex+n, sizeof(szKeyHex)-n, "%02X ", ap->key[i] );
						szKeyAscii[i] = isprint( ap->key[i] )?ap->key[i]:'.';
					}

					printf( "[%-16s|%d]\n\t[MAC:%s] [%d/%d]\n\tCrack [HEX %s| ASC %s]",
						ap->essid, ap->bcn, (ap->security&STD_WEP?"WEP":"OTHER"), 
						szMac, szKeyHex, szKeyAscii );

					if( ap->rcrack )
					{
						POS.Y += 1;
						printf( "\n%s cracked key = %s profile %s", ap->essid, ap->key, ap->sprofile?"set":"not set" );
						if( ap->sprofile == false )
						{
							SetProfile( guidIntf, (char*)ap->essid, (char*)ap->key );
							ap->sprofile = true;
						}
					}

					POS.Y += 4;
				}

				ap = ap->next;
			}

			c = clock();
		}
		Sleep(1);
	}
	NmStopCapture( myCaptureEngine, nChoice );
	NmCloseHandle( myCaptureEngine );

	network * ap = networktable;
	while( ap )
	{
		network *aptmp = ap;
		ap = ap->next;

		// fclose( aptmp->ivs );
		// DeleteCriticalSection( &aptmp->lock );
		freenetwork( aptmp );
	}
	return 0;
}