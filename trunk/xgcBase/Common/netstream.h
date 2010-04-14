#pragma once
#include <windows.h>
#pragma warning (disable: 4251)
#include <string>
#include <tchar.h>
//#include "bufstream.h"
//
//template< typename Protocol >
//class netstream :	public bufstream
//{
//public:
//	netstream( const char* _ptr, _uint32 _length )
//		: bufstream( _ptr, _length, sizeof(protrol) )
//	{
//		m_pProtocol = (Protocol*)_ptr;
//	}
//
//	__inline const Protocol& protocol_head()const{ return *m_pProtocol; }
//
//private:
//	Protocol *m_pProtocol;
//};

template <typename T>
struct Type2Type
{
	typedef T OriginalType;
};

#define readtype( T )	read( Type2Type< T >() )
struct buffer
{
	const char*	data;
	size_t	size;
};

class CNetInStream
{
public:
	CNetInStream(void)
		: m_pbuf( NULL )
		, m_nBufSize( 0 )
		, m_nNowSite( 0 )
		, m_nLength( -1 )
	{
	}

	CNetInStream( char* data, size_t size )
		: m_pbuf( data )
		, m_nBufSize( size )
		, m_nNowSite( 0 )
		, m_nLength( -1 )
	{}

	// attach a buffer and return now buffer point
	inline char*	attach_buffer( char* pNewBuffer, size_t nSize )
	{
		char* pbuf	= m_pbuf;
		m_pbuf		= pNewBuffer;
		m_nBufSize	= nSize;
		m_nNowSite	= 0;
		return pbuf;
	}

	// detach a buffer
	inline char*	detach_buffer()
	{
		m_nBufSize = 0;
		m_nNowSite = 0;
		char* pbuf = m_pbuf;
		return pbuf;
	}

	// reset all state of this class 
	// for example, now site , len, buffer point and so on
	inline void	reset(){ m_nNowSite = 0; }

	// get buf point
	inline char*	get()const{ return m_pbuf;}

	// get now length
	inline size_t	length()const{ ASSERT( m_nLength != -1 ); return m_nLength; }

	// move pos to
	inline bool	moveto( size_t pos )
	{ 
		if( pos < 0 || pos >= m_nBufSize )
			return false;
		else
		{
			m_nNowSite = pos;
			return true;
		}
	}

	inline bool	offset( size_t delta )
	{ 
		if( m_nNowSite + delta < 0 || m_nNowSite + delta > m_nBufSize )
			return false;
		else
		{
			m_nNowSite += delta;
			return true;
		}
	}

	inline void	move_end(){ m_nNowSite = m_nLength; }

	inline void	end(){ m_nLength = m_nNowSite;};

	inline CNetInStream& write( const char* pszbuf, size_t size )
	{
		if( size > m_nBufSize - m_nNowSite )
		{
			throw( _T("buffer over out.") );
		}
		if( m_pbuf + m_nNowSite != pszbuf )	memcpy( m_pbuf + m_nNowSite, pszbuf, size );
		m_nNowSite += size;
		return *this;
	}

	template< typename T >
	void write( T* );

	template< typename T >
	void write( CONST T* );

	template< typename T >
	inline CNetInStream& write( T& val )
	{
		if( m_nNowSite + sizeof( T ) <= m_nBufSize )
		{
			*( (T*)( m_pbuf + m_nNowSite ) ) = val;
			m_nNowSite += sizeof( T );
		}
		else
		{
			throw( _T("buffer over out.") );
		}
		return *this;
	}

	template< typename T >
	inline CNetInStream& write( const T& val )
	{
		if( m_nNowSite + sizeof( T ) <= m_nBufSize )
		{
			*( (T*)( m_pbuf + m_nNowSite ) ) = val;
			m_nNowSite += sizeof( T );
		}
		else
		{
			throw( _T("buffer over out.") );
		}
		return *this;
	}

	inline CNetInStream& write( bool	val ){ return write< bool >		(val); }
	inline CNetInStream& write( char	val ){ return write< char >		(val); }
	inline CNetInStream& write( short	val ){ return write< short >	(val); }
	inline CNetInStream& write( int		val ){ return write< int >		(val); }
	inline CNetInStream& write( long	val ){ return write< long >		(val); }
	inline CNetInStream& write( float	val ){ return write< float >	(val); }
	inline CNetInStream& write( double	val ){ return write< double >	(val); }

	inline CNetInStream& write( unsigned char	val ){ return write< unsigned char >	(val); }
	inline CNetInStream& write( unsigned short	val ){ return write< unsigned short >	(val); }
	inline CNetInStream& write( unsigned long	val ){ return write< unsigned long >	(val); }

	inline CNetInStream& write( const char* val )
	{
		char* pbuf = m_pbuf + m_nNowSite;
		size_t nLen = strlen( val ) + 1;
		if( m_nNowSite + nLen < m_nBufSize )
		{
			memcpy( pbuf, val, nLen );
			m_nNowSite += nLen;
		}
		else
		{
			throw( _T("buffer over out.") );
		}
		return *this;
	}

	inline CNetInStream& write( const wchar_t* val )
	{
		char* pbuf = m_pbuf + m_nNowSite;
		size_t nLen = ( wcslen( val ) + 1 ) * sizeof( wchar_t );
		if( m_nNowSite + nLen < m_nBufSize )
		{
			memcpy( pbuf, val, nLen );
			m_nNowSite += nLen;
		}
		else
		{
			throw( _T("buffer over out.") );
		}
		return *this;
	}

	inline CNetInStream& write( std::string& val ){ return write( val.c_str() ); }
	inline CNetInStream& write( const std::string& val ){ return write( val.c_str() ); }

	inline CNetInStream& write( const buffer& val )
	{
		write( val.size );
		write( val.data, val.size );
		return *this;
	}

	inline CNetInStream& write( char* val )
	{
		return write( (const char*)val );
	}

	inline CNetInStream& write( wchar_t* val )
	{
		return write( (const wchar_t*)val );
	}

	template< typename T >
	inline CNetInStream& operator << ( T val )
	{
		write( val );
		return *this;
	}

	inline CNetInStream& operator << ( const char* val )
	{
		write( val );
		return *this;
	}
	//////////////////////////////////////////////////////////////////////////

	inline CNetInStream& operator << ( const wchar_t* val )
	{
		write(val);
		return *this;
	}

	inline CNetInStream& operator << ( buffer& val )
	{
		write(val);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////

	typedef void ( *pfnControl )( CNetInStream& netstream );
	template< >
		CNetInStream& operator << ( pfnControl pfn )
	{
		pfn( *this );
		return *this;
	}

	friend void __cdecl end( CNetInStream& netstreams );
private:
	char*	m_pbuf;
	size_t	m_nBufSize;
	size_t	m_nNowSite;
	size_t	m_nLength;
};

class CNetOutStream
{
public:
	CNetOutStream( const char* data, size_t size )
	: m_pbuf( data )
	, m_nBufSize( size )
	, m_nNowSite( 0 )
	{}

	CNetOutStream( void* data, size_t size )
		: m_pbuf( (const char*)data )
		, m_nBufSize( size )
		, m_nNowSite( 0 )
	{}

	inline void	reset(){ m_nNowSite = 0; }

	// move pos to
	inline bool	moveto( size_t pos )
	{ 
		if( pos < 0 || pos >= m_nBufSize )
			return false;
		else
		{
			m_nNowSite = pos;
			return true;
		}
	}

	inline bool	offset( int delta )
	{ 
		if( m_nNowSite + delta < 0 || m_nNowSite + delta > m_nBufSize )
			return false;
		else
		{
			m_nNowSite += delta;
			return true;
		}
	}

	inline void	move_end(){ m_nNowSite = m_nBufSize; }

	// get now release length
	inline size_t	length()const{ return m_nBufSize - m_nNowSite; }
	// get now position
	inline size_t	position()const{ return m_nNowSite; }
	// get buffer size;
	inline size_t	size()const{ return m_nBufSize; }

	inline const char* get()const{ return m_pbuf + m_nNowSite; }

	inline void read(char* buf, size_t size)
	{
		if( m_nNowSite + size > m_nBufSize )
			throw( _T("too many read bitys" ) );
		memcpy( buf, m_pbuf + m_nNowSite, size );
		m_nNowSite += size;
	}

	//////////////////////////////////////////////////////////////////////////
	// Group : 1
	// void read( T& val )
	//////////////////////////////////////////////////////////////////////////
	template< typename T >
	inline 	void read( T& val )
	{
		val = *( (T*)( m_pbuf + m_nNowSite ) );
		m_nNowSite += sizeof( T );
		if( m_nNowSite > m_nBufSize )
			throw( _T("too many read bitys" ) );
	}

	// 指针类型限制,提供编译期错误
	template< typename T >
		void read( const T *&val );

	template< >
	inline 	void read( const char*& val )
		{
			val = m_pbuf + m_nNowSite;
			m_nNowSite += strlen( val ) + 1;
			if( m_nNowSite > m_nBufSize )	
				throw( _T("too many read bitys" ) );
		}

	template< >
	inline 	void read( const wchar_t*& val )
		{
			val = (wchar_t*)(m_pbuf + m_nNowSite);
			m_nNowSite += ( wcslen( val ) + 1 )*sizeof(wchar_t);
			if( m_nNowSite > m_nBufSize )	
				throw( _T("too many read bitys" ) );
		}

	template< >
	inline void read( std::string& val )
	{

	}

	template< >
	inline 	void read( buffer& val )
		{
			val.size = *(size_t*)(m_pbuf+m_nNowSite);
			m_nNowSite += sizeof(size_t);
			val.data = get();
			m_nNowSite += val.size;
		}


	//////////////////////////////////////////////////////////////////////////
	// Group : 2
	// T read( Type2Type< T > )
	//////////////////////////////////////////////////////////////////////////
	template< typename T >
	inline T read( Type2Type< T > )
		{
			T* pVal = (T*)( m_pbuf + m_nNowSite );
			m_nNowSite += sizeof( T );
			if( m_nNowSite > m_nBufSize )	
				throw( _T("too many read bitys" ) );
			return *pVal;
		}

	// 指针限制, 防止传入指针.
	template< typename T >
		typename const T*& read( Type2Type< const T*& > );
	template< typename T >
		typename T*& read( Type2Type< T*& > );

	template< >
	inline 	const char* read( Type2Type< const char* > )
		{
			const char* val = m_pbuf + m_nNowSite;
			m_nNowSite += strlen( val ) + 1;
			if( m_nNowSite > m_nBufSize )	
				throw( _T("too many read bitys" ) );
			return val;
		}

	template< >
	inline 	const wchar_t* read( Type2Type< const wchar_t* > )
		{
			const wchar_t* val = (wchar_t*)(m_pbuf + m_nNowSite);
			m_nNowSite += ( wcslen( val ) + 1 )*sizeof(wchar_t);
			if( m_nNowSite > m_nBufSize )	
				throw( _T("too many read bitys" ) );
			return val;
		}

	template< >
	inline 	buffer read( Type2Type< buffer > )
		{
			buffer val;
			val.size = *(size_t*)(m_pbuf+m_nNowSite);
			m_nNowSite += sizeof(size_t);
			val.data = get();
			return val;
		}

	//////////////////////////////////////////////////////////////////////////
	// Group : 3
	// operator >> ( T& val )
	//////////////////////////////////////////////////////////////////////////
	// operator 
	template< typename T >
	inline 	CNetOutStream& operator >> ( T& val )
	{
		read( val );
		return *this;
	}
	
	// 指针限制, 防止传入指针.
	template< typename T >
		CNetOutStream& operator >> ( const T *&val );
	template< typename T >
		CNetOutStream& operator >> ( T *&val );

	template< >
	inline 	CNetOutStream& operator >> ( const char*& val )
	{
		read( val );
		return *this;
	}

	template< >
	inline 	CNetOutStream& operator >> ( const wchar_t*& val )
	{
		read( val );
		return *this;
	}

	template< >
	inline 	CNetOutStream& operator >> ( buffer& val )
	{
		read( val );
		return *this;
	}

private:
	const char*	m_pbuf;
	size_t		m_nBufSize;
	size_t		m_nNowSite;
};

inline void __cdecl end( CNetInStream& netstream )
{
	netstream.m_nLength = netstream.m_nNowSite;
}