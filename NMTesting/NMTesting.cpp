// NMTesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "objbase.h"
#include "ntddndis.h"
#include "NMApi.h"
#include <locale.h>
#include <conio.h>
#include <time.h>
HANDLE myNplParser;
HANDLE myFrameParserConfig;
HANDLE myFrameParser;

ULONG myHTTPFilterID;
ULONG myHTTPFieldID;

BOOL CaptureDone = FALSE;
ULONG FrameCount = 0;

void __stdcall
MyParserBuild(PVOID Context, ULONG StatusCode, LPCWSTR lpDescription, ULONG ErrorType)
{
	wprintf(L"%s\n", lpDescription);
}

// Returns a frame parser with a filter and one data field.
// INVALID_HANDLE_VALUE indicates failure.
HANDLE
MyLoadNPL(void)
{
	HANDLE myFrameParser = INVALID_HANDLE_VALUE;
	ULONG ret;

	// Use NULL to load the default NPL parser set.
	ret = NmLoadNplParser(NULL, NmAppendRegisteredNplSets, MyParserBuild, 0, &myNplParser);

	if(ret == ERROR_SUCCESS){
		ret = NmCreateFrameParserConfiguration(myNplParser, MyParserBuild, 0, &myFrameParserConfig);

		if(ret == ERROR_SUCCESS)
		{
			ret = NmAddFilter(myFrameParserConfig, L"http.request.command == \"GET\"", &myHTTPFilterID);
			if(ret != 0)
			{
				wprintf(L"Failed to add fitler, error 0x%X\n", ret);
			}

			ret = NmAddField(myFrameParserConfig, L"http.request.uri", &myHTTPFieldID);
			if(ret != ERROR_SUCCESS)
			{
				wprintf(L"Failed to add field, error 0x%X\n", ret);
			}

			ret = NmCreateFrameParser(myFrameParserConfig, &myFrameParser);
			if(ret != ERROR_SUCCESS)
			{
				wprintf(L"Failed to create frame parser, error 0x%X\n", ret);
				NmCloseHandle(myFrameParserConfig);
				NmCloseHandle(myNplParser);
				return INVALID_HANDLE_VALUE;
			}
		}
		else
		{
			wprintf(L"Unable to load parser config, error 0x%X\n", ret);
			NmCloseHandle(myNplParser);
			return INVALID_HANDLE_VALUE;
		}

	}
	else
	{
		wprintf(L"Unable to load NPL\n");
		return INVALID_HANDLE_VALUE;
	}

	return(myFrameParser);
}

void
UnLoadNPL(void)
{
	NmCloseHandle(myNplParser);
	NmCloseHandle(myFrameParserConfig);
}

DWORD WINAPI  
myFrameEval(PVOID pContext)
{
	HANDLE capFile = (HANDLE)pContext;
	HANDLE hRawFrame;
	ULONG ret;
	ULONG curFrame = 0;

	while(!CaptureDone)
	{
		if(FrameCount > curFrame)
		{
			for(;curFrame < FrameCount; curFrame++)
			{
				ret = NmGetFrame(capFile, curFrame, &hRawFrame);

				if(ret == ERROR_SUCCESS)
				{
					HANDLE myParsedFrame;

					ret = NmParseFrame(myFrameParser, hRawFrame, curFrame, 0, &myParsedFrame, NULL);
					if(ret != ERROR_SUCCESS)
					{
						wprintf(L"Error: 0x%X, trying to parse frame\n", ret);
					}
					else
					{

						// Test to see if this frame passed the filter.
						BOOL passed;
						NmEvaluateFilter(myParsedFrame, myHTTPFilterID, &passed);
						if(passed)
						{
							// Obtain the value of http.request.uri from the frame.
							// Strings are passed as bstrVal in the variant.
							WCHAR value[256];
							ret = NmGetFieldValueString(myParsedFrame, myHTTPFieldID, 256, value);
							if(ret == ERROR_SUCCESS)
							{
								wprintf(L"HTTP: %s\n", value);
							}
						}

						NmCloseHandle(myParsedFrame);
					}
				}

				NmCloseHandle(hRawFrame);
			}

			Sleep(5000);
		}
	}

	return 0;
}

// Frame indication callback called each time a frame appears.
void __stdcall 
myFrameIndication(HANDLE hCapEng, ULONG AdpIdx, PVOID pContext, HANDLE hRawFrame)
{
	//HANDLE capFile = (HANDLE)pContext;
	//NmAddFrame(capFile, hRawFrame);

	BYTE szBuffer[4096];
	ULONG nRead = 0;
	NmGetRawFrame( hRawFrame, sizeof(szBuffer), szBuffer, &nRead );
	printf( "%02x %02x %02x %02x - %ld\n", szBuffer[0], szBuffer[1], szBuffer[2], szBuffer[3], nRead );
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

int __cdecl wmain(int argc, WCHAR* argv[])
{
	ULONG ret;
	ULONG adapterIndex = 0;

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
	for( ULONG i = 0; i < adapterCount; ++i )
	{
		NmGetAdapter( myCaptureEngine, i, &AdapterInfo );
		if( AdapterInfo.MediumType == NdisMediumNative802_11 && AdapterInfo.PhysicalMediumType == NdisPhysicalMediumNative802_11 )
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
	}

	TCHAR szBuffer[256];
	DWORD dwRead = 0;
	if( ReadConsole( GetStdHandle(STD_INPUT_HANDLE), szBuffer, _countof(szBuffer), &dwRead, NULL ) == FALSE )
	{
		puts( "error input" );
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

	// Configure the adapter's callback and pass a capture handle as a context value.
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
		if( kbhit() )
		{
			char ch = getch();
			if( ch == 27 )
				break;
		}
		if( clock() - c > 1000 )
		{
			printf( "capture frame %ld\n", FrameCount );
			c = clock();
		}
		Sleep(1);
	}
	NmStopCapture( myCaptureEngine, nChoice );
	NmCloseHandle( myCaptureEngine );
	return 0;
}

//int __cdecl wmain(int argc, WCHAR* argv[])
//{
//	ULONG ret;
//	ULONG adapterIndex = 3;
//
//	if(2 == argc)
//		adapterIndex = _wtol(argv[1]);
//
//	// Open the capture engine.
//	HANDLE myCaptureEngine;
//	ret = NmOpenCaptureEngine(&myCaptureEngine);
//	if(ret != ERROR_SUCCESS)
//	{
//		wprintf(L"Error openning capture engine, 0x%X\n", ret);
//		return ret;
//	}
//
//	// Initialize the parser engine and return a frame parser.
//	//myFrameParser = MyLoadNPL();
//	//if(myFrameParser == INVALID_HANDLE_VALUE)
//	//{
//	//	wprintf(L"Errors creating frame parser\n");
//	//	return -1;
//	//}
//
//	//HANDLE myTempCap;
//	//ULONG CapSize;
//	//ret = NmCreateCaptureFile(L"temp_capfiltlive.cap", 20000000, NmCaptureFileWrapAround, &myTempCap, &CapSize);
//	//if(ret != ERROR_SUCCESS)
//	//{
//	//	wprintf(L"Error creating the capture file.\n");
//	//	NmCloseHandle(myCaptureEngine);
//	//	NmCloseHandle(myFrameParser);
//	//	UnLoadNPL();
//	//	return ret;
//	//}
//
//	// Configure the adapter's callback and pass a capture handle as a context value.
//	ret = NmConfigAdapter(myCaptureEngine, adapterIndex, myFrameIndication, NULL);
//	if(ret != ERROR_SUCCESS)
//	{
//		wprintf(L"Error configuring the adapter.\n");
//		NmCloseHandle(myCaptureEngine);
//		NmCloseHandle(myFrameParser);
//		//NmCloseHandle(myTempCap);
//		//UnLoadNPL();
//		return ret;
//	}
//
//	// Create a thread to handle asynchronously captured frames.
//	//HANDLE eThreadHandle = CreateThread(NULL, 0, myFrameEval, myTempCap, 0, NULL);
//
//	//if(NULL != eThreadHandle)
//	{
//		wprintf(L"Capturing for 20 seconds\n");
//		ret = NmStartCapture(myCaptureEngine, adapterIndex, NmLocalOnly);
//		if(ret == ERROR_SUCCESS)
//		{
//			Sleep(20000);
//
//			wprintf(L"Stopping capture\n");
//			NmStopCapture(myCaptureEngine, adapterIndex);
//		}
//		else
//		{
//			wprintf(L"Failed to start capture engine\n");
//		}
//
//		// Terminate evaluation thread.
//		//CaptureDone = TRUE;
//		//ret = WaitForSingleObject(eThreadHandle, INFINITE);
//		//if(0 != ret)
//		//{
//		//	wprintf(L"Failed to terminate evaluation thread (%d)\n", ret);
//		//}
//
//		//if(!CloseHandle(eThreadHandle))
//		//{
//		//	wprintf(L"Failed to close thread handle\n");
//		//}
//	}
//
//	NmCloseHandle(myCaptureEngine);
//	NmCloseHandle(myFrameParser);
//
//	UnLoadNPL();
//
//	return 0;
//}