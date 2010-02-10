#ifndef _NDIS_PORT_H
#define _NDIS_PORT_H
HANDLE	OpenHandle( __in __nullterminated TCHAR *pDeviceName );
BOOL	GetSrcMac( HANDLE Handle, PUCHAR pSrcMacAddr );
VOID	DoReadProc( HANDLE Handle );
VOID	DoWriteProc( HANDLE  Handle );
ULONG	EnumerateDevices( HANDLE  Handle, wstring strDeviceName[256][2] );
#endif