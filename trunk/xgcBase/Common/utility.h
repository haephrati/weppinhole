#pragma once
#ifndef _COMMON_H
#define _COMMON_H

#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif

//--------------------------------------------------------------------------------------------------------//
// 规格化路径，将路径规格化为绝对路径 类似于"E:\tools\Downloads\"
// 规格化后的路径如果传入绝对路径则只判断路径是否合法，如果传入相对路径则以当前程序所在的目录为当前目录
// 目录规格化后都以'\'结尾。
// [in]lpszPath				:	需要规格化的路径
// [out]lpszNormalizePath	:	规格化后的输出缓冲区
// [in]dwSize				:	输出缓冲区大小
//--------------------------------------------------------------------------------------------------------//
EXPORT_LIB bool NormalizePath( _lpctstr lpszPath, LPTSTR lpszNormalizePath, DWORD dwSize );

// 判断是否UTF8文本
EXPORT_LIB _int32 IsTextUTF8( LPSTR lpstrInputStream, INT iLen );

// 为API打补丁（IAT）
EXPORT_LIB bool patchimport( HMODULE importmodule, _lpcstr exportmodulename, _lpcstr exportmodulepath, _lpcstr importname, _lpcvoid replacement );

// 清除补丁
EXPORT_LIB void restoreimport (HMODULE importmodule, _lpcstr exportmodulename, _lpcstr exportmodulepath, _lpcstr importname, _lpcvoid replacement);

//--------------------------------------------------------//
//	created:	3:12:2009   16:04
//	filename: 	utility
//	author:		Albert.xu
//
//	purpose:	获取最后一次系统错误的描述
//--------------------------------------------------------//
EXPORT_LIB _lpctstr GetErrorDescription();

// 串转换
EXPORT_LIB std::string ws2s(const std::wstring& ws);
EXPORT_LIB std::string ws2s(const wchar_t* ws);

EXPORT_LIB std::wstring s2ws(const std::string& s);
EXPORT_LIB std::wstring s2ws(const char* s);

#ifdef _UNICODE
	#define XA2T( a )	s2ws( a ).c_str()
	#define XT2A( t )	ws2s( t ).c_str()
	

	#define XW2T( w )	w
	#define XT2W( t )	t

	#define XA2TSTR( a )	s2ws( a )
	#define XT2ASTR( t )	ws2s( t )
	#define XW2TSTR( w )	std::wstring( w )
	#define XT2WSTR( t )	std::wstring( t )

#else
	#define XA2T( a )	a
	#define XT2A( t )	t
	#define XW2T( w )	ws2s( a ).c_str()
	#define XT2W( t )	s2ws( t ).c_str()

	#define XA2TSTR( a )	std::string( a )
	#define XT2ASTR( t )	std::string( t )
	#define XW2TSTR( w )	ws2s( w )
	#define XT2WSTR( t )	s2ws( t )
#endif
	#define XA2W( a )	s2ws( a ).c_str()
	#define XW2A( w )	ws2s( w ).c_str()

EXPORT_LIB _lpcstr gethost( _lpcstr pszAny );

//--------------------------------------------------------------------------------------------------------//
// D2Hex
// pData		:	数据首址
// dwDataSize	:	数据长度（按字符计）
// pOut			:	输出缓冲首址
// dwOutSize	:	缓冲区长度（按字符计）
// dwFlags		:	标志 （未定义）
// return		:	返回转换了多少字符
//--------------------------------------------------------------------------------------------------------//
#define HEX_WIDTH_8		0x00000000
#define HEX_WIDTH_16	0x00000001
#define HEX_WIDTH_32	0x00000002
#define HEX_WIDTH_64	0x00000003

EXPORT_LIB _uint32 data2hex( _lpcstr pData, _uint32 nDataSize, _lptstr pOut, _uint32 nOutSize, _uint32 nFlags = 0 );

EXPORT_LIB char* strntok( char *string, const char *end, const char *control, char **next );

//--------------------------------------------------------------------------------------------------------//
#define INET_SERVICE_UNK        0x1000
//--------------------------------------------------------------------------------------------------------//
// inet 辅助函数
#if defined( _USE_WININET )
BOOL EXPORT_LIB ParseURLEx(_lpctstr pstrURL, DWORD& dwServiceType, _string& strServer, _string& strObject, INTERNET_PORT& nPort, _string& strUsername, _string& strPassword, DWORD dwFlags/* = 0*/);
BOOL EXPORT_LIB ParseURL(_lpctstr pstrURL, DWORD& dwServiceType, _string& strServer, _string& strObject, INTERNET_PORT& nPort);
#endif

#endif // _COMMON_H