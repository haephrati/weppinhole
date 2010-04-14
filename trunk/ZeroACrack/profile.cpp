#include <Windows.h>
#include <wlanapi.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
wchar_t szProfileStringFmt[] = 
	L"<?xml version=\"1.0\"?>\n"
	L"	<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\n"
	L"	<name>%S</name>\n"
	L"	<SSIDConfig>\n"
	L"		<SSID>\n"
	L"			<name>%S</name>\n"
	L"		</SSID>\n"
	L"		<nonBroadcast>false</nonBroadcast>\n"
	L"	</SSIDConfig>\n"
	L"	<connectionType>ESS</connectionType>\n"
	L"	<connectionMode>auto</connectionMode>\n"
	L"	<MSM>\n"
	L"		<security>\n"
	L"			<authEncryption>\n"
	L"				<authentication>open</authentication>\n"
	L"				<encryption>WEP</encryption>\n"
	L"				<useOneX>false</useOneX>\n"
	L"			</authEncryption>\n"
	L"			<sharedKey>\n"
	L"				<keyType>networkKey</keyType>\n"
	L"				<protected>false</protected>\n"
	L"				<keyMaterial>%S</keyMaterial>\n"
	L"			</sharedKey>\n"
	L"		</security>\n"
	L"	</MSM>\n"
	L"</WLANProfile>\n"
;

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

void SetProfile( GUID guidIntf, char *essid, char *pwd )
{
	DWORD dwError;
	HANDLE hClient = NULL;
	DWORD dwReason;

	wchar_t strXml[1024];
	_snwprintf( strXml, _countof(strXml), szProfileStringFmt, essid, essid, pwd );

	// open handle
	if ((dwError = OpenHandleAndCheckVersion(
		&hClient
		)) != ERROR_SUCCESS)
	{
		puts( "error open handle" );
		return;
	}

	// set profile
	dwError = WlanSetProfile(
		hClient, 
		&guidIntf, 
		0,          // no flags for the profile 
		strXml, 
		NULL,       // use the default ACL
		TRUE,		// overwrite a profile if it already exists
		NULL,       // reserved
		&dwReason
		);

	switch(dwError)
	{
	case ERROR_INVALID_PARAMETER:
		puts( "The profile has invalid params." );
		break;
	case ERROR_BAD_PROFILE:
		puts( "The profile is bad." );
		break;
	case ERROR_ALREADY_EXISTS:
		puts( "The profile is already exists." );
	}

	// clean up
	if (hClient != NULL)
	{
		WlanCloseHandle(
			hClient, 
			NULL            // reserved
			);
	}
}
