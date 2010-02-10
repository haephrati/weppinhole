#include "stdafx.h"

#ifndef NDIS_STATUS
#define NDIS_STATUS     ULONG
#endif

#if DBG
#define DEBUGP(stmt)    printf stmt
#else
#define DEBUGP(stmt)
#endif

#define PRINTF(stmt)    printf stmt

#ifndef MAC_ADDR_LEN
#define MAC_ADDR_LEN                    6
#endif

#define MAX_NDIS_DEVICE_NAME_LEN        256

BOOLEAN         DoEnumerate = FALSE;
BOOLEAN         DoReads = FALSE;
INT             NumberOfPackets = -1;
ULONG           PacketLength = 100;
UCHAR           SrcMacAddr[MAC_ADDR_LEN];
UCHAR           DstMacAddr[MAC_ADDR_LEN];
BOOLEAN         bDstMacSpecified = FALSE;
CHAR *          pNdisDeviceName = "JUNK";
USHORT          EthType = 0x8e88;
BOOLEAN         bUseFakeAddress = FALSE;
UCHAR           FakeSrcMacAddr[MAC_ADDR_LEN] = {0};


#include <pshpack1.h>

typedef struct _ETH_HEADER
{
	UCHAR       DstAddr[MAC_ADDR_LEN];
	UCHAR       SrcAddr[MAC_ADDR_LEN];
	USHORT      EthType;
} ETH_HEADER, *PETH_HEADER;

#include <poppack.h>

HANDLE
OpenHandle(
		   __in __nullterminated TCHAR *pDeviceName
		   )
{
	DWORD   DesiredAccess;
	DWORD   ShareMode;
	LPSECURITY_ATTRIBUTES   lpSecurityAttributes = NULL;

	DWORD   CreationDistribution;
	DWORD   FlagsAndAttributes;
	HANDLE  Handle;
	DWORD   BytesReturned;

	DesiredAccess = GENERIC_READ|GENERIC_WRITE;
	ShareMode = 0;
	CreationDistribution = OPEN_EXISTING;
	FlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

	Handle = CreateFile(
		pDeviceName,
		DesiredAccess,
		ShareMode,
		lpSecurityAttributes,
		CreationDistribution,
		FlagsAndAttributes,
		NULL
		);
	if (Handle == INVALID_HANDLE_VALUE)
	{
		//DEBUGP(("Creating file failed, error %x\n", GetLastError()));
		return Handle;
	}
	//
	//  Wait for the driver to finish binding.
	//
	if (!DeviceIoControl(
		Handle,
		IOCTL_NDISPROT_BIND_WAIT,
		NULL,
		0,
		NULL,
		0,
		&BytesReturned,
		NULL))
	{
		//DEBUGP(("IOCTL_NDISIO_BIND_WAIT failed, error %x\n", GetLastError()));
		CloseHandle(Handle);
		Handle = INVALID_HANDLE_VALUE;
	}

	return (Handle);
}

BOOL
GetSrcMac(
		  HANDLE  Handle,
		  PUCHAR  pSrcMacAddr
		  )
{
	DWORD       BytesReturned;
	BOOLEAN     bSuccess;
	UCHAR       QueryBuffer[sizeof(NDISPROT_QUERY_OID) + MAC_ADDR_LEN];
	PNDISPROT_QUERY_OID  pQueryOid;


	//DEBUGP(("Trying to get src mac address\n"));

	pQueryOid = (PNDISPROT_QUERY_OID)&QueryBuffer[0];
	pQueryOid->Oid = OID_802_3_CURRENT_ADDRESS;
	pQueryOid->PortNumber = 0;

	bSuccess = (BOOLEAN)DeviceIoControl(
		Handle,
		IOCTL_NDISPROT_QUERY_OID_VALUE,
		(LPVOID)&QueryBuffer[0],
		sizeof(QueryBuffer),
		(LPVOID)&QueryBuffer[0],
		sizeof(QueryBuffer),
		&BytesReturned,
		NULL);

	if (bSuccess)
	{
		//DEBUGP(("GetSrcMac: IoControl success, BytesReturned = %d\n",
		//	BytesReturned));

#pragma prefast(suppress:__WARNING_WRITE_OVERRUN __WARNING_BUFFER_OVERRUN_NONSTACK __WARNING_READ_OVERRUN __WARNING_BUFFER_OVERRUN, "enough space allocated in QueryBuffer")
		memcpy(pSrcMacAddr, pQueryOid->Data, MAC_ADDR_LEN);                    
	}
	else
	{
		//DEBUGP(("GetSrcMac: IoControl failed: %d\n", GetLastError()));
	}

	return (bSuccess);
}



VOID
DoReadProc(
		   HANDLE  Handle
		   )
{
	PUCHAR      pReadBuf = NULL;
	INT         ReadCount = 0;
	BOOLEAN     bSuccess;
	ULONG       BytesRead;

	//DEBUGP(("DoReadProc\n"));

	do
	{
		pReadBuf = (PUCHAR)malloc(PacketLength);

		if (pReadBuf == NULL)
		{
			PRINTF(("DoReadProc: failed to alloc %d bytes\n", PacketLength));
			break;
		}

		ReadCount = 0; 
		while (TRUE)
		{
#pragma prefast(suppress: 8193, "bSuccess is examined below")
			bSuccess = (BOOLEAN)ReadFile(
				Handle,
				(LPVOID)pReadBuf,
				PacketLength,
				&BytesRead,
				NULL);

			if (!bSuccess)
			{
				PRINTF(("DoReadProc: ReadFile failed on Handle %p, error %x\n",
					Handle, GetLastError()));
				break;
			}
			ReadCount++;

			//DEBUGP(("DoReadProc: read pkt # %d, %d bytes\n", ReadCount, BytesRead));

			if ((NumberOfPackets != -1) && (ReadCount == NumberOfPackets))
			{
				break;
			}
		}
	}
	while (FALSE);

	if (pReadBuf)
	{
		free(pReadBuf);
	}

	PRINTF(("DoReadProc finished: read %d packets\n", ReadCount));

}


VOID
DoWriteProc(
			HANDLE  Handle
			)
{
	PUCHAR      pWriteBuf = NULL;
	PUCHAR      pData;
	UINT        i;
	INT         SendCount;
	PETH_HEADER pEthHeader;
	DWORD       BytesWritten;
	BOOLEAN     bSuccess;

	//DEBUGP(("DoWriteProc\n"));
	SendCount = 0;

	do
	{
		pWriteBuf = (PUCHAR)malloc(PacketLength);

		if (pWriteBuf == NULL)
		{
			//DEBUGP(("DoWriteProc: Failed to malloc %d bytes\n", PacketLength));
			break;
		}
		pEthHeader = (PETH_HEADER)pWriteBuf;
#pragma prefast(suppress: __WARNING_POTENTIAL_BUFFER_OVERFLOW, "pWriteBuf is PacketLength(100 bytes long");        
		pEthHeader->EthType = EthType;

		if (bUseFakeAddress)
		{
			memcpy(pEthHeader->SrcAddr, FakeSrcMacAddr, MAC_ADDR_LEN);
		}
		else
		{
			memcpy(pEthHeader->SrcAddr, SrcMacAddr, MAC_ADDR_LEN);
		}

		memcpy(pEthHeader->DstAddr, DstMacAddr, MAC_ADDR_LEN);

		pData = (PUCHAR)(pEthHeader + 1);
		for (i = 0; i < PacketLength - sizeof(ETH_HEADER); i++)
		{
			*pData++ = (UCHAR)i;
		}

		SendCount = 0;

		while (TRUE)
		{

			bSuccess = (BOOLEAN)WriteFile(
				Handle,
				pWriteBuf,
				PacketLength,
				&BytesWritten,
				NULL);
			if (!bSuccess)
			{
				PRINTF(("DoWriteProc: WriteFile failed on Handle %p\n", Handle));
				break;
			}
			SendCount++;

			//DEBUGP(("DoWriteProc: sent %d bytes\n", BytesWritten));

			if ((NumberOfPackets != -1) && (SendCount == NumberOfPackets))
			{
				break;
			}
		}

	}
	while (FALSE);

	if (pWriteBuf)
	{
		free(pWriteBuf);
	}

	PRINTF(("DoWriteProc: finished sending %d packets of %d bytes each\n",
		SendCount, PacketLength));
}

ULONG EnumerateDevices( HANDLE  Handle, wstring strDeviceName[256][2] )
{
	typedef int __declspec(align(MEMORY_ALLOCATION_ALIGNMENT)) QueryBindingCharBuf;
	QueryBindingCharBuf		Buf[1024];
	DWORD       		BufLength = sizeof(Buf);
	DWORD       		BytesWritten;
	DWORD       		i;
	PNDISPROT_QUERY_BINDING 	pQueryBinding;

	pQueryBinding = (PNDISPROT_QUERY_BINDING)Buf;

	i = 0;
	for (pQueryBinding->BindingIndex = i;
		/* NOTHING */;
		pQueryBinding->BindingIndex = ++i)
	{
#pragma prefast(suppress: __WARNING_READ_OVERRUN, "Okay to read sizeof(NDISPROT_QUERY_BINDING) from pQueryBinding");    
		if (DeviceIoControl(
			Handle,
			IOCTL_NDISPROT_QUERY_BINDING,
			pQueryBinding,
			sizeof(NDISPROT_QUERY_BINDING),
			Buf,
			BufLength,
			&BytesWritten,
			NULL))
		{
			//PRINTF(("%2d. %ws\n     - %ws\n",
			//	pQueryBinding->BindingIndex,
			//	(WCHAR *)((PUCHAR)pQueryBinding + pQueryBinding->DeviceNameOffset),
			//	(WCHAR *)((PUCHAR )pQueryBinding + pQueryBinding->DeviceDescrOffset)));

			strDeviceName[pQueryBinding->BindingIndex][0] = (WCHAR *)((PUCHAR)pQueryBinding + pQueryBinding->DeviceNameOffset);
			strDeviceName[pQueryBinding->BindingIndex][1] = (WCHAR *)((PUCHAR )pQueryBinding + pQueryBinding->DeviceDescrOffset);

			memset(Buf, 0, BufLength);
		}
		else
		{
			ULONG   rc = GetLastError();
			if (rc != ERROR_NO_MORE_ITEMS)
			{
				PRINTF(("EnumerateDevices: terminated abnormally, error %d\n", rc));
			}
			break;
		}
	}
	return pQueryBinding->BindingIndex;
}

