#include "stdafx.h"
#include "logger_adapter.h"
#include <time.h>

////--------------------------------------------------------------------------------------------------------//
//// 
//CSocketAdapter::~CSocketAdapter()
//{
//	if( m_socket != INVALID_SOCKET )
//	{
//		shutdown( m_socket, SD_BOTH );
//		closesocket( m_socket );
//	}
//}
//
////--------------------------------------------------------------------------------------------------------//
//// 连接日志服务器
//// szHost		:	日志服务器ip
//// nPort		:	日志服务器端口
//// return		:	连接是否成功
////--------------------------------------------------------------------------------------------------------//
//bool CSocketAdapter::Connect( _lpctstr szHost, int nPort )
//{
//	SOCKADDR_IN		ServerAddr;
//
//	ServerAddr.sin_family		= AF_INET;
//	ServerAddr.sin_addr.s_addr	= inet_addr( szHost );
//	ServerAddr.sin_port			= htons( nPort );
//
//	m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
//
//	int hResult = connect( m_socket, ( SOCKADDR *)&ServerAddr, sizeof( ServerAddr ) );
//	if( hResult == SOCKET_ERROR )
//	{
//		int nErrorCode = WSAGetLastError();
//		closesocket( m_socket );
//		return false;
//	}
//
//	return true;
//}
//
////--------------------------------------------------------------------------------------------------------//
//// 关联一个Socket
//// socket		:	指定的套接字
//// return		:	是否成功
////--------------------------------------------------------------------------------------------------------//
//bool CSocketAdapter::AttachSocket( DWORD s )
//{
//	if( s == INVALID_SOCKET )	return false;
//	m_socket = (SOCKET)s;
//	return true;
//}
//
////--------------------------------------------------------------------------------------------------------//
//// 写日志
//// szBuffer		:	日志缓冲区，由用户提供
//// size			:	缓冲区大小
////--------------------------------------------------------------------------------------------------------//
//int CSocketAdapter::WriteLog( _string strLoggerName, _lpctstr szBuffer, size_t size )
//{
//	TCHAR szLog[4096];
//	CNetInStream in( (char*)szLog, sizeof( szLog ) );
//	try
//	{
//		in	<< DWORD( 0 )
//			<< BYTE( 0x53 )
//			<< strLoggerName.c_str() << GetName()
//			<< size;
//		in.append( szBuffer, size*sizeof(TCHAR) );
//		in << end;
//		in.moveto( 0 );
//		in << in.length() - sizeof(DWORD);
//	}
//	catch( std::runtime_error& e )
//	{
//		e.what();
//		return 0;
//	}
//	
//	return send( m_socket, in.get(), (int)in.length(), 0 );
//}

//--------------------------------------------------------------------------------------------------------//
// class CFileAdapter
//--------------------------------------------------------------------------------------------------------//
CFileAdapter::~CFileAdapter()
{
	Close();
}

int CFileAdapter::CombinedFileName( _lptstr lptszBuf, size_t nCount )
{
	int nRet = 0;
	switch( m_enSplitMode )
	{
	case split_none:
		{
			nRet = _sntprintf( lptszBuf, nCount, _T("%s.%s"), m_strFilename.c_str(), m_strExt.c_str() );
		}
		break;
	case split_date:
		{
			time_t ltm = time( &ltm );
			tm* ptm = localtime( &ltm );
			nRet = _sntprintf( lptszBuf, nCount, _T("%s_%02d%02d.%s"), m_strFilename.c_str(), ptm->tm_mon, ptm->tm_mday, m_strExt.c_str() );
			m_lpSplitParam = ptm->tm_mday;
		}
		break;
	case split_line:
		{
			nRet = _sntprintf( lptszBuf, nCount, _T("%s_%d.%s"), m_strFilename.c_str(), m_nCurLine%m_lpSplitParam, m_strExt.c_str() );
		}
		break;
	}
	return nRet;
}

//--------------------------------------------------------------------------------------------------------//
// 打开日志文件
// szFilename	:	日志文件名
// return		:	文件是否打开
//--------------------------------------------------------------------------------------------------------//
bool CFileAdapter::Open( _lpctstr szFilename, _lpctstr szExt, CFileAdapter::filemode enSplitMode, ulong_ptr lpParam )
{
	if( m_file )
	{
		return false;
	}
	m_strFilename	= szFilename;
	m_strExt		= szExt;
	m_enSplitMode	= enSplitMode;
	m_lpSplitParam	= lpParam&0xffff;

	TCHAR tszFile[_MAX_PATH];
	CombinedFileName( tszFile, sizeof( tszFile )/sizeof( tszFile[0] ) );

	if( ( m_file = _tfopen( tszFile, _T("a") ) ) == NULL )
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------//
// 关闭日志文件
//--------------------------------------------------------------------------------------------------------//
void CFileAdapter::Close()
{
	if( m_file )
	{
		fclose(m_file);
		m_file = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------//
// 写日志
// szBuffer		:	日志缓冲区，由用户提供
// size			:	缓冲区大小
//--------------------------------------------------------------------------------------------------------//
int CFileAdapter::Write( _lpcwstr szBuffer, size_t size )
{
	if( !m_file ) return 0;

	switch( m_enSplitMode )
	{
	case split_line:
		if( m_lpSplitParam && !(m_nCurLine%m_lpSplitParam) )
		{
			TCHAR szFilename[_MAX_PATH];
			if( CombinedFileName( szFilename, _countof( szFilename ) ) > 0 )
			{
				FILE* fp = NULL;
				if( ( fp = _tfopen( szFilename, _TEXT("a") ) ) != NULL )
				{
					Close();
					m_file = fp;
				}
			}
			else
			{
				return 0;
			}
		}
		break;
	case split_date:
		{
			time_t ltm = time( &ltm );
			tm* ptm = localtime( &ltm );
			if( ptm->tm_mday != m_lpSplitParam )
			{
				TCHAR szFilename[_MAX_PATH];
				if( CombinedFileName( szFilename, _countof( szFilename ) ) > 0 )
				{
					FILE* fp = NULL;
					if( ( fp = _tfopen( szFilename, _TEXT("a") ) ) != NULL )
					{
						Close();
						m_file = fp;
						m_lpSplitParam = ptm->tm_mday;
					}
				}
				else
				{
					return 0;
				}
			}
		}
		break;
	}
	fputws( szBuffer, m_file );
	if( m_nWriteVelocity && !(++m_nCurLine%m_nWriteVelocity) )
	{
		fflush( m_file );
	}

	return size;
}

//--------------------------------------------------------------------------------------------------------//
// WindowsAdapter [4/4/2007]
// CWindowAdapter
//--------------------------------------------------------------------------------------------------------//

CWindowAdapter::CWindowAdapter( HWND hEditHwnd, WPARAM wTextLimitLen/* = 1024*16*/, _lpctstr lpszName )
: CLoggerAdapter( lpszName, CLoggerAdapter::debug )
, m_hEditHwnd( hEditHwnd )
, m_nMaxLen( wTextLimitLen )
, m_nTotalLen( 0 )
{
	SendMessage( hEditHwnd, EM_SETLIMITTEXT, m_nMaxLen, 0 );
	m_nMaxLen = SendMessage( hEditHwnd, EM_GETLIMITTEXT, 0, 0 );
}

CWindowAdapter::~CWindowAdapter(void)
{
}

//--------------------------------------------------------------------------------------------------------//
// 写日志
// szBuffer		:	日志缓冲区，由用户提供
// size			:	缓冲区大小
//--------------------------------------------------------------------------------------------------------//
int CWindowAdapter::Write( _lpcwstr szBuffer, size_t size )
{
	DWORD dwResult;
	wchar_t szLog[16*1024];
	size = _snwprintf( szLog, _countof(szLog), L"%s\r\n", szBuffer );
	_int32 retry = 10;
	if( m_hEditHwnd && size > 0 )
	{
		LRESULT ret = 0;
		if( (WPARAM)m_nTotalLen > m_nMaxLen )
		{
			if( retry )	do 
			{
				ret = ::SendMessageTimeoutW(m_hEditHwnd, EM_SETSEL, 0, m_nMaxLen/8, SMTO_BLOCK, 100, &dwResult );	
			} while ( --retry && ret == 0?Sleep(100),GetLastError() == ERROR_TIMEOUT:false );

			if( retry )	do 
			{
				ret = ::SendMessageTimeoutW(m_hEditHwnd, EM_REPLACESEL, 0, (LPARAM)"", SMTO_BLOCK, 100, &dwResult );
			} while ( --retry && ret == 0?Sleep(100),GetLastError() == ERROR_TIMEOUT:false );
			m_nTotalLen = 0;
		}

		if( retry )	do 
		{
			ret = ::SendMessageTimeoutW(m_hEditHwnd, EM_SETSEL, -2, -1, SMTO_BLOCK, 100, &dwResult );	//将当前位置设在Edit控件的最后并清除当前选择   
		}while( --retry && ret == 0?Sleep(100),GetLastError() == ERROR_TIMEOUT:false );

		if( retry )	do 
		{
			ret = ::SendMessageTimeoutW(m_hEditHwnd, EM_REPLACESEL, 0, (LPARAM)szLog, SMTO_BLOCK, 100, &dwResult);	//添加一行字符
		}while( --retry && ret == 0?Sleep(100),GetLastError() == ERROR_TIMEOUT:false );
		
		m_nTotalLen += size + sizeof(wchar_t);
	}
	//::SendMessage( m_hDlgHwnd, WM_USER + 0x4031, 0, (LPARAM)szLog );
	return 0;
}

//--------------------------------------------------------------------------------------------------------//
// CConsoleAdapter [10/29/2007]
// CConsoleAdapter
//--------------------------------------------------------------------------------------------------------//
CConsoleAdapter::CConsoleAdapter( HANDLE hOutPut, _lpctstr lpszName/* = _T("WindowLog")*/, _uint16 nCodePage, CLoggerAdapter::logger_level enLevel )
: CLoggerAdapter( lpszName, enLevel )
, m_hOutput( hOutPut )
{
}

CConsoleAdapter::~CConsoleAdapter(void)
{
}
//--------------------------------------------------------------------------------------------------------//
// 写日志
// szBuffer		:	日志缓冲区，由用户提供
// size			:	缓冲区大小
//--------------------------------------------------------------------------------------------------------//
int CConsoleAdapter::Write( _lpcwstr szBuffer, size_t size )
{
	//wchar_t szLog[16*1024];
	//size = _snwprintf( szLog, _countof(szLog), L"%s\n", szBuffer ) + 1;
	DWORD dwWrite = 0;
	WriteConsoleW( m_hOutput, szBuffer, (DWORD)size, &dwWrite, NULL );
	WriteConsoleW( m_hOutput, L"\n", 1, &dwWrite, NULL );
	return 0;
}

//--------------------------------------------------------------------------------------------------------//
// CConsoleAdapter [10/29/2007]
// CConsoleAdapter
//--------------------------------------------------------------------------------------------------------//
CDebugViewAdapter::CDebugViewAdapter( _lpctstr lpszName/* = _T("WindowLog")*/, _uint16 nCodePage, CLoggerAdapter::logger_level enLevel )
: CLoggerAdapter( lpszName, enLevel )
{
}

CDebugViewAdapter::~CDebugViewAdapter(void)
{
}
//--------------------------------------------------------------------------------------------------------//
// 写日志
// szBuffer		:	日志缓冲区，由用户提供
// size			:	缓冲区大小
//--------------------------------------------------------------------------------------------------------//
int CDebugViewAdapter::Write( _lpcwstr szBuffer, size_t size )
{
	wchar_t szLog[16*1024];
	size = _snwprintf( szLog, _countof(szLog), L"%s\r\n", szBuffer ) + sizeof(wchar_t);
	OutputDebugStringW( szBuffer );
	return 0;
}