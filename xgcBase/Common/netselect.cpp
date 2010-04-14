#include "stdafx.h"
#include "NetSelect.h"
#include <winsock.h>

CNetSelect::CNetSelect()
: m_socket( INVALID_SOCKET )
{
}

CNetSelect::~CNetSelect()
{
}

VOID CNetSelect::SendErrorCodeEx( int nErrorCode )
{
	m_nErrorCode = nErrorCode;
}

int	CNetSelect::InitSocket()
{
	WSADATA wsd;
	// 加载winsock库
	int nResult = WSAStartup(MAKEWORD(1,1), &wsd);
	if (0 != nResult)		return FALSE;

	// 检查版本
	if ( LOBYTE( wsd.wVersion ) != 1 || HIBYTE( wsd.wVersion ) != 1 )
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

HRESULT	CNetSelect::Connect( const char *address, unsigned short port )	// 连接主机
{
	SOCKADDR_IN		ServerAddr;

	ServerAddr.sin_family		= AF_INET;
	ServerAddr.sin_addr.s_addr	= inet_addr( address );
	ServerAddr.sin_port			= htons( port );

	m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	int overtime = 1000;
	// SO_RCVTIMEO 和 SO_SNDTIMEO 有可能引起潜在的数据丢失情况发生.
	// setsockopt( m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&overtime, sizeof( overtime ) );
	// setsockopt( m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&overtime, sizeof( overtime ) );
	int hResult = connect( m_socket, ( SOCKADDR *)&ServerAddr, sizeof( ServerAddr ) );
	if( hResult == SOCKET_ERROR )
	{
		int nErrorCode = WSAGetLastError();
		SendErrorCodeEx( nErrorCode );
		closesocket( m_socket );
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	return TRUE;
}

BOOL CNetSelect::Disconnect()
{
	//安全关闭套接字
	int hResult = shutdown( m_socket, 0 );
	if( hResult == SOCKET_ERROR )
	{
		SendErrorCodeEx( WSAGetLastError() );
	}
	hResult = closesocket( m_socket );
	m_socket = INVALID_SOCKET;
	if( hResult == SOCKET_ERROR )
	{
		SendErrorCodeEx( WSAGetLastError() );
		return FALSE;
	}
	return TRUE;
}

HRESULT CNetSelect::Send( const char* pszBuffer, int size, int& nResult )
{
	// 发送消息头
	nResult = send( m_socket, pszBuffer, size, 0 );
	if( nResult == SOCKET_ERROR )
	{
		nResult = WSAGetLastError();
		SendErrorCodeEx( nResult );
		int nRet = SUCCESS;
		if( nResult == WSAETIMEDOUT )
		{
			nRet = RETRY;
		}
		if( nResult == WSAECONNABORTED ||
			nResult == WSAECONNRESET ||
			nResult == WSAENOTSOCK ||
			nResult == WSAENOTCONN )	// 超时或者协议错误
		{
			// 这里直接返回，等待下一次发送调用。
			//Disconnect();
			nRet = DISCONNECT;
		}
		return nRet;
	}
	return SUCCESS;
}

HRESULT CNetSelect::Recv( char* pszBuffer, int size, int& nResult )
{
	fd_set fdread;
	timeval tv;
	FD_ZERO(&fdread);//初始化fd_set

	tv.tv_sec = 0;//这里我们打算让select等待200mil后返回，避免被锁死，也避免马上返回
	tv.tv_usec = 10;

	FD_SET(m_socket, &fdread);//分配套接字句柄到相应的fd_set
	nResult = select(0, &fdread, NULL, NULL, &tv);
	if( nResult == SOCKET_ERROR )
	{
		nResult = WSAGetLastError();
		return nResult == WSAECONNRESET || nResult == WSAENOTSOCK?DISCONNECT:SOCKETERROR;
	}
	else if( FD_ISSET( m_socket, &fdread ) )
	{
		nResult = recv( m_socket, pszBuffer, size, 0 );
		if( nResult == SOCKET_ERROR )
		{
			nResult = WSAGetLastError();
			//Disconnect();
			return (nResult == WSAENOTSOCK||nResult== WSAECONNRESET)?DISCONNECT:SOCKETERROR;
		}
		else if( nResult == 0 )
		{
			//Disconnect();
			return DISCONNECT;
		}
		return SUCCESS;
	}
	return RETRY;
}