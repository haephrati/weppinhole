#pragma once 
#define BUFFER_SIZE 512
#include <WinSock2.h>

#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif

class EXPORT_LIB CNetSelect
{
public:
//------------------------------------------------------------------//
// 方法
//------------------------------------------------------------------//
	enum 
	{
		SUCCESS		= 0x0000,
		RETRY		= 0x1000,
		DISCONNECT	= 0x1001,
		SOCKETERROR	= 0x1002,
	};

	CNetSelect();
	~CNetSelect();

	void			SendErrorCodeEx( int nErrorCode );
	BOOL			InitSocket();	// 初始化Socket
	HRESULT			Connect( const char *address, unsigned short port );	// 连接主机
	BOOL			Disconnect();

	HRESULT			Send( const char* pszBuffer, int size, int& nResult );
	HRESULT			Recv( char* pszBuffer, int size, int& nResult );

	int				GetLastError(){ return m_nErrorCode; }

protected:
	int				m_nErrorCode;
	SOCKET			m_socket;
};
