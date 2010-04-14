#ifndef _MACRO_DEFINE_H
#define _MACRO_DEFINE_H

#include <tchar.h>
#include "commonspp.h"
//-------------------------------------------------------------------------------------------------------------------------------------------------//
// 安全删除定义
//-------------------------------------------------------------------------------------------------------------------------------------------------//
#ifndef SAFE_DELETE
#define SAFE_DELETE( p )		delete(p);(p) = NULL;
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( p )	delete[] (p); (p) = NULL;
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( p )		if(p){ (p)->Release(); (p) = NULL; }
#endif
//-------------------------------------------------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------------------------------------------------//
// 流读取宏定义
//-------------------------------------------------------------------------------------------------------------------------------------------------//
#define READ_STREAM( stream, msg )	try{ stream >> msg; } catch( std::exception e ){ e.what(); return -1; }
//-------------------------------------------------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------------------------------------------------//
// 定义基本类型
//-------------------------------------------------------------------------------------------------------------------------------------------------//
#include <string>
#if defined( _UNICODE )
	typedef std::wstring			_string;
	typedef std::wstringstream		_stringstream;
	typedef wchar_t					_tchar;

	#define INCSZ( pch )			( ++pch )
	#define INCNSZ( pch, count )	( pch+=count )
	#define CMPSZ( pch1, pch2 )		( pch1 == pch2 )
	#define EQUSZ( pch1, pch2 )		( *pch1 = *pch2 )
	#define DEFCH( ch )				( ch )
#else
	typedef std::string				_string;
	typedef std::stringstream		_stringstream;
	typedef char					_tchar;

	#define INCSZ( pch )			( pch = _mbsinc( pch ) )
	#define INCNSZ( pch, count )	( pch = _mbsninc( pch, count ) )
	#define CMPSZ( pch1, pch2 )		( _mbccmp( pch1, ( const UCHAR* )pch2 ) )
	#define EQUSZ( pch1, pch2 )		( _mbccpy( pch1, pch2 ) )
	#define DEFCH( ch )				( ( const unsigned char* )&ch[0] )
#endif

typedef std::string		_astring;
typedef std::wstring	_wstring;

#if !defined(_point64)
	#if !defined(__midl) && (defined(_X86_) || defined(_MIX86)) && _MSC_VER >= 1300
		#define _point64 __w64
	#else
		#define	_point64
	#endif
#endif

#ifndef _DEFINES
#define _DEFINES
	typedef wchar_t		_wchar;
	//typedef char		_int8;
	//typedef short		_int16;
	//typedef int			_int32;
	//typedef __int64		_int64;
	typedef long		_long32;
	
	typedef unsigned char		_byte;
	typedef unsigned char		_uint8;
	typedef unsigned short		_uint16;
	typedef unsigned int		_uint32;
	typedef unsigned long		_ulong, _ulong32;
	typedef unsigned __int64	_uint64;
	
	typedef const _tchar*		_lpctstr;
	typedef _tchar*				_lptstr;
	
	typedef const char*			_lpcstr;
	typedef char*				_lpstr;
	
	typedef const wchar_t*		_lpcwstr;
	typedef wchar_t*			_lpwstr;
	
	typedef int					_point64 int_ptr, *pint_ptr;
	typedef unsigned int		_point64 uint_ptr, *puint_ptr;

	typedef long				_point64 long_ptr, *plong_ptr;	
	typedef unsigned long		_point64 ulong_ptr, *pulong_ptr;	

	typedef const void			*_lpcvoid;
	typedef void				*_lpvoid;
#endif // _DEFINES

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// ASSERT 定义
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include <crtdbg.h>
#ifdef ASSERT
#undef ASSERT
#endif

#ifdef ASSERT_MSG
#undef ASSERT_MSG
#endif

#ifdef ASSERT_POINTER
#undef ASSERT_POINTER
#endif

#ifdef VERIFY
#undef VERIFY
#endif

#define WIDEN2(x)	L ## x
#define WIDEN(x)	WIDEN2(x)

#define TIDEN2(x)	_T( ## x ## )
#define TIDEN(x)	WIDEN2(x)

#define XGC_WRITELOG( log, FMT, ... )	CLogger::GetInstance(log)->WriteLog( FMT, __VA_ARGS__ )
#define XGC_WRITELOGA( log, FMT, ... )	CLogger::GetInstance(log)->WriteLogA( FMT, __VA_ARGS__ )
#define XGC_WRITELOGW( log, FMT, ... )	CLogger::GetInstance(log)->WriteLogW( FMT, __VA_ARGS__ )
#define XGC_WRITEASSERT( msg )			XGC_WRITELOG( _T("ASSERT"), _T("ASSERT AT: %s %d %s - %s"), TIDEN(__FILE__), __LINE__, TIDEN(__FUNCTION__), msg )

#define XGC_BIT(w)		(1<<w)
#define XGC_GETBIT(v,w) ((XGC_BIT(w)&v)>>w)
#define XGC_SETBIT(v,w)	(XGC_BIT(w)|v)

#ifdef _DEBUG
	#define ASSERT(expr)			_ASSERT(expr)
	#define ASSERT_MSG(expr,msg)	_ASSERT_EXPR(expr,msg)
	#define ASSERT_POINTER(expr)	_ASSERT_EXPR(expr,_T("invalid point"))
	#define VERIFY(expr)			_ASSERT(expr)
	#define DEBUG_CODE( ... )		__VA_ARGS__
#elif defined( _ASSERT_LOG )
	#include "logger.h"
	#define ASSERT(expr)			if(!(expr)){ XGC_WRITEASSERT( _T("") );}
	#define ASSERT_MSG(expr,msg)	if(!(expr)){ XGC_WRITEASSERT( msg );}
	#define ASSERT_POINTER(expr)	if(!(expr)){ XGC_WRITEASSERT( _T("invalid point") );}
	#define VERIFY(expr)			ASSERT(expr)
	#define DEBUG_CODE( ... )
#else
	#define ASSERT(expr)			(void)(0)
	#define ASSERT_MSG(expr,msg)	(void)(0)
	#define ASSERT_POINTER(expr)	(void)(0)
	#define VERIFY(expr)			(void)(expr)
	#define DEBUG_CODE( ... )
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// LOKI 库配置预定义
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#if defined(_DEBUG) 
	#define LOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT
	#define LESS std::less
	#define ALLOCATOR std::allocator
#elif defined(USING_LOKI_LIBRARY)
	#define LESS std::less
	#define ALLOCATOR Loki::LokiAllocator
#else
	#define LOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT
	#define LESS std::less
	#define ALLOCATOR std::allocator
#endif
#define LOKI_OBJECT_LEVEL_THREADING

#endif //_MACRO_DEFINE_H