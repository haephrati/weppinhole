#include "stdafx.h"
#include <time.h>
#include "logger.h"
#include "logger_adapter.h"
//--------------------------------------------------------------------------------------------------------//
// class CLogger 
// 日志管理者，日至记录入口
//--------------------------------------------------------------------------------------------------------//
struct CLogger::IMPL
{
	typedef std::list< CLoggerAdapter* >	CLoggerAdapterList;
	typedef std::map< _string, logobj >		CLoggerInstanceMap;
	typedef std::queue< std::pair< void*, size_t > >	CLoggerBufferList;

	CLoggerAdapterList			m_AdapterList;
	CLoggerBufferList			m_LoggerList;
	CCritical					m_AdapterListLock;
	CCritical					m_LoggerListLock;
	intptr_t					m_hWriteThread;
	bool						m_bWriterWork;

	static CLoggerInstanceMap	m_LoggerMap;
	static CLoggerAdapterList	m_CaptrueAdapterList;
	static CCritical			m_LoggerMapLock;
	static CCritical			m_CaptrueAdapterListLock;
	static intptr_t				m_CaptrueThreadHandle;
	static bool					m_bCaptrueWorkFlag;

	logger_mode					m_mode;
	logger_level				m_level;
	_string						m_strLoggerName;
	unsigned long				m_counter;
};

CLogger::IMPL::CLoggerInstanceMap	CLogger::IMPL::m_LoggerMap;
CLogger::IMPL::CLoggerAdapterList	CLogger::IMPL::m_CaptrueAdapterList;
CCritical							CLogger::IMPL::m_LoggerMapLock;
CCritical							CLogger::IMPL::m_CaptrueAdapterListLock;
intptr_t							CLogger::IMPL::m_CaptrueThreadHandle = -1;
bool								CLogger::IMPL::m_bCaptrueWorkFlag = false;

CLogger::CLogger( _lpctstr szLoggerName )
: m_pImpl( new IMPL )
{
	m_pImpl->m_mode = CLogger::normal;
	m_pImpl->m_strLoggerName = szLoggerName;
	m_pImpl->m_counter = 0;
	m_pImpl->m_hWriteThread = _beginthreadex( NULL, 0, writer_thread, (void*)m_pImpl, 0, NULL );
}

CLogger::~CLogger(void)
{
	ASSERT( m_pImpl->m_AdapterList.empty() );
	m_pImpl->m_bWriterWork = false;
	WaitForSingleObject( (HANDLE)m_pImpl->m_hWriteThread, INFINITE );
	CloseHandle( (HANDLE)m_pImpl->m_hWriteThread );
	delete m_pImpl;
	m_pImpl = NULL;
}

//--------------------------------------------------------------------------------------------------------//
// 取得日志实例
// szLogger		:	实例名
//--------------------------------------------------------------------------------------------------------//
CLogger* CLogger::GetInstance( _lpctstr szLogger )
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );
	IMPL::CLoggerInstanceMap::const_iterator citer = IMPL::m_LoggerMap.find( szLogger );
	CLogger* pLogger = NULL;
	if( citer == IMPL::m_LoggerMap.end() )
	{
		pLogger = new CLogger( szLogger );
		IMPL::m_LoggerMap[szLogger] = pLogger;
	}
	else
	{
		pLogger = citer->second;
	}
	return pLogger;
}

//--------------------------------------------------------------------------------------------------------//
// 设置日志输出模式
//--------------------------------------------------------------------------------------------------------//
VOID CLogger::SetLoggerMode( logger_mode en_mode )
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );

	IMPL::CLoggerInstanceMap::const_iterator citer = IMPL::m_LoggerMap.begin();
	while( citer != IMPL::m_LoggerMap.end() )
	{
		citer->second->SetMode( en_mode );
		++citer;
	}
}

//--------------------------------------------------------------------------------------------------------//
// 日志实例是否存在
// szLogger		:	实例名
//--------------------------------------------------------------------------------------------------------//
bool CLogger::Have( _lpctstr szLogger )
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );

	IMPL::CLoggerInstanceMap::const_iterator citer = IMPL::m_LoggerMap.find( szLogger );
	return citer != IMPL::m_LoggerMap.end();
}

//--------------------------------------------------------------------------------------------------------//
// 清除日至
// szLogger		:	实例名
//--------------------------------------------------------------------------------------------------------//
void CLogger::Clear( _lpctstr szLogger )
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );

	IMPL::CLoggerInstanceMap::iterator iter = IMPL::m_LoggerMap.find( szLogger );
	if( iter != IMPL::m_LoggerMap.end() )
	{
		IMPL::m_LoggerMap.erase( iter );
	}
}

//--------------------------------------------------------------------------------------------------------//
// 清除所有日至
//--------------------------------------------------------------------------------------------------------//
void CLogger::ClearAll()
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );

	IMPL::m_LoggerMap.clear();
}

//--------------------------------------------------------------------------------------------------------//
// 添加一个日至适配器
// pAdapter		:	适配器指针
//--------------------------------------------------------------------------------------------------------//
bool CLogger::AddAdapter( CLoggerAdapter* pAdapter, bool bCaptrueDebugString )
{
	CCriticalLock lock( m_pImpl->m_AdapterListLock );

	IMPL::CLoggerAdapterList::const_iterator citer = std::find( m_pImpl->m_AdapterList.begin(), m_pImpl->m_AdapterList.end(), pAdapter );
	if( citer == m_pImpl->m_AdapterList.end() )
	{
		m_pImpl->m_AdapterList.push_back( pAdapter );
	}

	if( bCaptrueDebugString )
	{
		m_pImpl->m_CaptrueAdapterListLock.Lock();
		m_pImpl->m_CaptrueAdapterList.push_back( pAdapter );
		m_pImpl->m_CaptrueAdapterListLock.UnLock();

		if( IMPL::m_bCaptrueWorkFlag == false )
		{
			IMPL::m_bCaptrueWorkFlag = true;
			IMPL::m_CaptrueThreadHandle = _beginthreadex( NULL, 0, captrue_debug_thread, NULL, 0, NULL );
		}
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------//
// 删除一个适配器
// szAdapterName	:	适配器名
//--------------------------------------------------------------------------------------------------------//
CLoggerAdapter* CLogger::RemoveAdapter( _lpctstr szAdapterName )
{
	CCriticalLock lock( m_pImpl->m_AdapterListLock );

	IMPL::CLoggerAdapterList::iterator iter		= m_pImpl->m_AdapterList.begin();
	IMPL::CLoggerAdapterList::iterator iter_end	= m_pImpl->m_AdapterList.end();
	CLoggerAdapter* pAdapter = NULL;
	while( iter != iter_end )
	{
		if( _tcscmp( szAdapterName, (*iter)->GetName() ) == 0 )
		{
			pAdapter = *iter;
			m_pImpl->m_AdapterList.erase( iter );
			break;
		}
		++iter;
	}

	// 从DebugOutput捕获列表里删除
	IMPL::m_CaptrueAdapterListLock.Lock();
	iter = m_pImpl->m_CaptrueAdapterList.begin();
	iter_end = m_pImpl->m_CaptrueAdapterList.end();
	iter = std::find( iter, iter_end, pAdapter );
	if( iter != iter_end )
		m_pImpl->m_CaptrueAdapterList.erase( iter );

	IMPL::m_CaptrueAdapterListLock.UnLock();

	// 列表为空则将线程退出
	if( m_pImpl->m_CaptrueAdapterList.empty() && (HANDLE)m_pImpl->m_CaptrueThreadHandle != INVALID_HANDLE_VALUE )
	{
		m_pImpl->m_bCaptrueWorkFlag = false;
		DWORD ret = WaitForSingleObject( (HANDLE)m_pImpl->m_CaptrueThreadHandle, 10000 );
		if( ret == WAIT_TIMEOUT )
		{
			TerminateThread( (HANDLE)m_pImpl->m_CaptrueThreadHandle, -1 );
		}
	}
	return pAdapter;
}

//--------------------------------------------------------------------------------------------------------//
// 写日至
// szBuffer		:	日至缓冲区，由用户提供
// size			:	缓冲区大小
// fmt			:	格式串
// ...			:	参数列表
//--------------------------------------------------------------------------------------------------------//
int CLogger::SWriteLogW( _lpwstr szBuffer, size_t nCount, _lpcwstr szFmt, ... )
{
	ASSERT( this );
	ASSERT( szBuffer );
	ASSERT( szFmt );
	if( m_pImpl->m_AdapterList.empty() ) return 0;

	wchar_t szDate[128];
	wchar_t szTime[128];
	_wstrdate( szDate );
	_wstrtime( szTime );

#ifdef _UNICODE
	int len = _snwprintf( szBuffer, nCount, L"- %s %s %x [%s,%ld] : ", szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );
#else
	int len = _snwprintf( szBuffer, nCount, L"- %s %s %x [%S,%ld] : ", szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );
#endif

	if( len < 0 || len >= (int)nCount )
		return -1;

	va_list args;
	va_start(args, szFmt);

	int size = _vsnwprintf( szBuffer + len, nCount - len, szFmt, args );
	va_end(args);

	if( size < 0 )
		return -1;

	if( size >= (int)nCount - len )
		szBuffer[nCount-1] = 0;

	size += len;
	void *data = malloc( size*sizeof(wchar_t) );
	wmemcpy( (wchar_t*)data, szBuffer, size );
	CCriticalLock( m_pImpl->m_LoggerListLock );
	m_pImpl->m_LoggerList.push( std::make_pair< void*, size_t >( data, size|0x01000000 ) );
	return size;
}

//--------------------------------------------------------------------------------------------------------//
// 写日至
// szBuffer		:	日至缓冲区，由用户提供
// ...			:	参数列表
// return		:	写入的字符数
// remark		:	小心，不能写如果长的字符，255个字符刚好。
//--------------------------------------------------------------------------------------------------------//
int CLogger::WriteLogW( _lpcwstr szFmt, ... )
{
	wchar_t szLog[4096];
	ASSERT( this );
	ASSERT( szFmt );
	if( m_pImpl->m_AdapterList.empty() ) return 0;

	wchar_t szDate[128];
	wchar_t szTime[128];
	_wstrdate( szDate );
	_wstrtime( szTime );

#ifdef _UNICODE
	int len = _snwprintf( szLog, _countof(szLog), L"- %s %s %x [%s,%ld] : ", szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );
#else
	int len = _snwprintf( szLog, _countof(szLog), L"- %s %s %x [%S,%ld] : ", szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );
#endif

	va_list args;
	va_start(args, szFmt);

	int size = _vsnwprintf( szLog + len, _countof(szLog) - len, szFmt, args );

	if( size < 0 )
		return -1;
	va_end(args);

	if( len >= _countof(szLog) )
		return -1;

	size += len + 1;
	void *data = malloc( size*sizeof(wchar_t) );
	wmemcpy( (wchar_t*)data, szLog, size );
	CCriticalLock( m_pImpl->m_LoggerListLock );
	m_pImpl->m_LoggerList.push( std::make_pair< void*, size_t >( data, size|0x01000000 ) );
	return size;
}

//--------------------------------------------------------------------------------------------------------//
// 写日至
// szBuffer		:	日至缓冲区，由用户提供
// size			:	缓冲区大小
// fmt			:	格式串
// ...			:	参数列表
//--------------------------------------------------------------------------------------------------------//
int CLogger::WriteLogA( _lpstr szBuffer, size_t nCount, _lpcstr szFmt, ... )
{
	ASSERT( this );
	ASSERT( szBuffer );
	ASSERT( szFmt );
	if( m_pImpl->m_AdapterList.empty() ) return 0;

	char szDate[128];
	char szTime[128];
	_strdate( szDate );
	_strtime( szTime );

#ifdef _UNICODE
	int len = _snprintf( szBuffer, nCount, "- %s %s %x [%S,%ld] : ", szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );
#else
	int len = _snprintf( szBuffer, nCount, "- %s %s %x [%s,%ld] : ", szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );
#endif
	
	if( len < 0 || len >= (int)nCount )
		return -1;

	va_list args;
	va_start(args, szFmt);

	int size = _vsnprintf( szBuffer + len, nCount - len, szFmt, args );
	va_end(args);

	if( size < 0 )
		return -1;

	if( size >= (int)nCount - len )
		szBuffer[nCount-1] = 0;

	size += len + 1;
	void *data = malloc( size );
	memcpy( data, szBuffer, size );
	CCriticalLock( m_pImpl->m_LoggerListLock );
	m_pImpl->m_LoggerList.push( std::make_pair< void*, size_t >( data, size|0x02000000 ) );
	return size;
}

//--------------------------------------------------------------------------------------------------------//
// 写日至
// szBuffer		:	日至缓冲区，由用户提供
// ...			:	参数列表
// return		:	写入的字符数
// remark		:	小心，不能写如果长的字符，255个字符刚好。
//--------------------------------------------------------------------------------------------------------//
int CLogger::WriteLogA( _lpcstr szFmt, ... )
{
	char szLog[4096];
	ASSERT( this );
	ASSERT( szFmt );
	if( m_pImpl->m_AdapterList.empty() ) return 0;

	char szDate[128];
	char szTime[128];
	_strdate( szDate );
	_strtime( szTime );

#ifdef _UNICODE
	int len = _snprintf( szLog, _countof(szLog), "- %s %s %x [%S,%ld] : ", szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );
#else
	int len = _snprintf( szLog, _countof(szLog), "- %s %s %x [%s,%ld] : ", szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );
#endif

	va_list args;
	va_start(args, szFmt);

	int size = _vsnprintf( szLog + len, _countof(szLog) - len, szFmt, args );
	va_end(args);

	if( size < 0 )
		return -1;

	if( size >= (int)_countof(szLog) - len )
		szLog[_countof(szLog)-1] = 0;

	size += len + 1;
	void *data = malloc( size );
	memcpy( data, szLog, size );
	CCriticalLock( m_pImpl->m_LoggerListLock );
	m_pImpl->m_LoggerList.push( std::make_pair< void*, size_t >( data, size|0x02000000 ) );
	return size;
}

//--------------------------------------------------------------------------------------------------------//
// 写日至
// szBuffer		:	日至缓冲区，由用户提供
// size			:	缓冲区大小
// fmt			:	格式串
// ...			:	参数列表
// return		:	写入的字符数
//--------------------------------------------------------------------------------------------------------//
int CLogger::WriteLog( _lptstr szBuffer, size_t nCount, _lpctstr szFmt, ... )
{
	ASSERT( this );
	ASSERT( szBuffer );
	ASSERT( szFmt );
	if( m_pImpl->m_AdapterList.empty() ) return 0;

	_tchar szDate[128];
	_tchar szTime[128];
	_tstrdate( szDate );
	_tstrtime( szTime );

	int len = _sntprintf( szBuffer, nCount, _T("- %s %s %x [%s,%ld] : "), szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );

	if( len < 0 || len >= (int)nCount )
		return -1;

	va_list args;
	va_start(args, szFmt);

	int size = _vsntprintf( szBuffer + len, nCount - len, szFmt, args );
	va_end(args);

	if( size < 0 )
		return -1;

	if( size >= (int)nCount - len )
		szBuffer[nCount-1] = 0;

	size += len + 1;
	void *data = malloc( size*sizeof(_tchar) );
	memcpy( data, szBuffer, size*sizeof(_tchar) );
	CCriticalLock( m_pImpl->m_LoggerListLock );
#if defined(_UNICODE) || defined(UNICODE)
	m_pImpl->m_LoggerList.push( std::make_pair< void*, size_t >( data, size|0x01000000 ) );
#else
	m_pImpl->m_LoggerList.push( std::make_pair< void*, size_t >( data, size|0x02000000 ) );
#endif
	return size;
}

//--------------------------------------------------------------------------------------------------------//
// 写日至
// szBuffer		:	日至缓冲区，由用户提供
// ...			:	参数列表
// return		:	写入的字符数
// remark		:	小心，不能写过长的字符，4096个字符刚好。
//--------------------------------------------------------------------------------------------------------//
int CLogger::WriteLog( _lpctstr szFmt, ... )
{
	_tchar szLog[4096];
	ASSERT( this );
	ASSERT( szFmt );
	if( m_pImpl->m_AdapterList.empty() ) return 0;

	_tchar szDate[128];
	_tchar szTime[128];
	_tstrdate( szDate );
	_tstrtime( szTime );

	int len = _sntprintf( szLog, _countof(szLog), _T("- %s %s %x [%s,%ld] : "), szDate, szTime, GetCurrentThreadId(), m_pImpl->m_strLoggerName.c_str(), ++m_pImpl->m_counter );

	va_list args;
	va_start(args, szFmt);

	int size = _vsntprintf( szLog + len, _countof(szLog) - len, szFmt, args );
	va_end(args);

	if( size < 0 )
		return -1;

	if( size >= (int)_countof(szLog) - len )
		szLog[_countof(szLog)-1] = 0;

	size += len + 1;
	void *data = malloc( size*sizeof(_tchar) );
	memcpy( data, szLog, size*sizeof(_tchar) );
	CCriticalLock( m_pImpl->m_LoggerListLock );
#if defined(_UNICODE) || defined(UNICODE)
	m_pImpl->m_LoggerList.push( std::make_pair< void*, size_t >( data, size|0x01000000 ) );
#else
	m_pImpl->m_LoggerList.push( std::make_pair< void*, size_t >( data, size|0x02000000 ) );
#endif
	return size;
}

//--------------------------------------------------------------------------------------------------------//
// 写日至
// szLog		:	日志串
// nSize		:	串长度
//--------------------------------------------------------------------------------------------------------//
int CLogger::WriteData( const char *data, size_t size )
{
	ASSERT( this );
	ASSERT( data );
	void *d = malloc( size );
	memcpy( d, data, size );

	CCriticalLock( m_pImpl->m_LoggerListLock );
	m_pImpl->m_LoggerList.push( std::make_pair< void*, size_t >( d, size ) );
	return size;
}

//--------------------------------------------------------------------------------------------------------//
// 设置日志输出模式
//--------------------------------------------------------------------------------------------------------//
VOID CLogger::SetMode( logger_mode en_mode )
{
	if( m_pImpl->m_mode == CLogger::pause && en_mode != CLogger::pause )
	{
		DWORD ret = ResumeThread( (HANDLE)m_pImpl->m_hWriteThread );
		if( ret == -1 )
			return;
	}
	else if( en_mode == CLogger::pause && m_pImpl->m_mode != CLogger::pause )
	{
		DWORD ret = SuspendThread( (HANDLE)m_pImpl->m_hWriteThread );
		if( ret == -1 )
			return;
	}

	m_pImpl->m_mode = en_mode;
}

//--------------------------------------------------------------------------------------------------------//
// 设置日志输出等级
// logger_mode	:	debug - 调试输出, trace - 包含DebugOutputString输出, warning - 警告输出, error - 错误输出
//--------------------------------------------------------------------------------------------------------//
void CLogger::SetLevel( logger_level en_level )
{
	m_pImpl->m_level = en_level;
}

//--------------------------------------------------------------------------------------------------------//
// 分发日至到适配器
// szLog		:	日志串
// nSize		:	串长度
//--------------------------------------------------------------------------------------------------------//
void CLogger::DispatchLog( CLogger::IMPL* pImpl, void* data, size_t size )
{
	int flag = size&0xff000000;
	size &= 0x00ffffff;

	wchar_t *pszLog = (wchar_t*)data;
	if( flag == 0x02000000 )
	{
		char *log = (char*)data;
		pszLog = (wchar_t*)realloc( NULL, size*sizeof(wchar_t) );
		mbstowcs( pszLog, log, size );
		free( data );
	}

	CCriticalLock lock( pImpl->m_AdapterListLock );

	IMPL::CLoggerAdapterList::const_iterator citer		= pImpl->m_AdapterList.begin();
	IMPL::CLoggerAdapterList::const_iterator citer_end	= pImpl->m_AdapterList.end();
	while( citer != citer_end )
	{
		(*citer)->Write( pszLog, size );
		++citer;
	}

	free( (void*)pszLog );
}

CLogger::logobj::~logobj()
{
	if( m_pLog )
	{ 
		delete m_pLog;
		m_pLog = NULL;
	}
}

unsigned int __stdcall CLogger::captrue_debug_thread( void *param )
{
	typedef struct dbwin_buffer 
	{
		DWORD   dwProcessId;
		char    data[4096-sizeof(DWORD)];
	} DEBUGBUFFER,*PDEBUGBUFFER;

	DWORD dwProcessID = GetCurrentProcessId();

	// 创建事件    
	HANDLE hAckEvent = OpenEvent( EVENT_ALL_ACCESS, FALSE, TEXT( "DBWIN_BUFFER_READY" ) );
	if( hAckEvent == NULL )
		hAckEvent = CreateEvent( NULL, FALSE, TRUE, TEXT( "DBWIN_BUFFER_READY" ) );

	HANDLE hReadyEvent = OpenEvent( SYNCHRONIZE, FALSE, TEXT( "DBWIN_DATA_READY" ) );
	if( hReadyEvent == NULL )
		hReadyEvent = CreateEvent( NULL, FALSE, FALSE, TEXT( "DBWIN_DATA_READY" ) );    

	// 创建文件映射    
	HANDLE hMapping = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(DEBUGBUFFER), TEXT("DBWIN_BUFFER") );

	// 映射调试缓冲区    
	PDEBUGBUFFER pDebugBuffer = (PDEBUGBUFFER)MapViewOfFile( hMapping, SECTION_MAP_READ, 0, 0, 0 );

	// 循环
	while( CLogger::IMPL::m_bCaptrueWorkFlag )  
	{
		// 激活事件
		SetEvent( hAckEvent ); 
		// 等待缓冲区数据 
		if ( WaitForSingleObject( hReadyEvent, INFINITE ) == WAIT_OBJECT_0 )      
		{	
			if( dwProcessID == pDebugBuffer->dwProcessId )
			{	
				// 只打印自己进程输出的日志
				// 添加新日志
				CCriticalLock lock( CLogger::IMPL::m_CaptrueAdapterListLock );
				
				CLogger::IMPL::CLoggerAdapterList::iterator iter = CLogger::IMPL::m_CaptrueAdapterList.begin();
				while( iter != CLogger::IMPL::m_CaptrueAdapterList.end() )
				{
#ifdef _UNICODE
					const char* _Source = pDebugBuffer->data;
					size_t _Dsize = strlen( _Source ) + 1;
					wchar_t *_Dest = (wchar_t*)_alloca(_Dsize*sizeof(wchar_t) );
					wmemset(_Dest, 0, _Dsize);
					mbstowcs(_Dest,_Source,_Dsize);
#else
					char * _Dest = pDebugBuffer->data;
					size_t _Dsize = strlen(pDebugBuffer->data) + 1;
#endif
					(*iter)->Write( _Dest, _Dsize );
					++iter;
				}
				
			}
			else
			{
				Sleep(1);
			}
		}
	}

	if( pDebugBuffer )  
	{
		UnmapViewOfFile( pDebugBuffer );    
	}
	CloseHandle( hMapping );  
	CloseHandle( hReadyEvent );  
	CloseHandle( hAckEvent );
	
	return 0;
}

unsigned int __stdcall CLogger::writer_thread( void *param )
{
	CLogger::IMPL* pImpl = (CLogger::IMPL*)param;
	ASSERT( pImpl );
	pImpl->m_bWriterWork = true;
	while( pImpl->m_bWriterWork  )
	{
		size_t count = pImpl->m_LoggerList.size();
		if( count > 0 )
		{
			pImpl->m_LoggerListLock.Lock();
			std::pair< void*, size_t > p = pImpl->m_LoggerList.front();
			pImpl->m_LoggerList.pop();
			pImpl->m_LoggerListLock.UnLock();
			DispatchLog( pImpl, p.first, p.second );
		}
		else
		{
			Sleep( 100 );
		}
	}
	return 0;
};