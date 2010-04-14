#pragma once
#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif
#include <set>
#include <map>
#include <list>
#include <deque>
#include <vector>

class EXPORT_LIB bufstream
{
protected:
	void _init_buf( int mode, _uint32 off )
	{
		if( mode & in )
		{
			_wd = off;
			_wsize = _length;
		}

		if( mode & out )
		{
			_rd = off;
			_rsize = _length;
		}
	}

public:
	enum _mode{ in = 1, out = 2, inout = 3 };
	enum _way{ beg = 0, cur = 1, end = 2 };
	bufstream(void)
		: _ptr(NULL)
		, _length(0)
		, _rsize(0)
		, _wsize(0)
		, _rd(0)
		, _wd(0)
		, _owner(false)
	{

	}

	bufstream( _uint32 length, int mode = bufstream::inout, _uint32 off = 0 )
		: _ptr( (const char*)malloc(length) )
		, _length(length)
		, _rsize(0)
		, _wsize(0)
		, _rd(0)
		, _wd(0)
		, _owner(true)
	{
		_init_buf( mode, off );
	}

	bufstream( const char *ptr, int length, int mode = bufstream::inout, _uint32 off = 0 )
		: _ptr(ptr)
		, _length(length)
		, _rsize(0)
		, _wsize(0)
		, _rd(0)
		, _wd(0)
		, _owner(false)
	{
		_init_buf( mode, off );
	}

	bufstream( void *ptr, int length, int mode = bufstream::inout, _uint32 off = 0 )
		: _ptr( (char *)ptr )
		, _length(length)
		, _rsize(0)
		, _wsize(0)
		, _rd(0)
		, _wd(0)
		, _owner(false)
	{
		_init_buf( mode, off );
	}

	virtual ~bufstream(void)
	{
		if( _owner )
			free( (void*)_ptr );
	}

	bufstream& operator=( const bufstream &rsh )
	{
		if( space() < rsh.wd() )
			throw( std::exception( "overflow" ) );
		putn( rsh.rd_ptr(), rsh.leave() );
		return *this;
	}

	bool setbuf( const char *ptr, int length, int mode = bufstream::inout, _uint32 off = 0 )
	{
		if( _owner )
			return false;

		_ptr = ptr;
		_length = length;
		_init_buf( mode, off );
		return true;
	}

	_uint32 seekpos( _uint32 pos, int mode )
	{
		if( pos > _length )
			pos = _length;

		if( mode & in )
			_wd = pos;

		if( mode & out )
			_rd = pos;

		return pos;
	}

	_uint32 seekoff( int off, int way, int mode = bufstream::inout )
	{
		switch( way )
		{
		case beg:
			return seekpos( off, mode );
			break;
		case end:
			ASSERT( off <= 0 );
			if( mode & in )
				_wd = _wsize + off;
			if( mode & out )
				_rd = _rsize + off;
		case cur:
			if( mode & in )
			{
				_wd += off;
				//if( _wd + off > _wsize )
				//	_wd = _wsize;
				//if( _wd + off < 0 )
				//	_wd = 0;
			}

			if( mode & out )
			{
				_rd += off;
				//if( _rd + off > _rsize )
				//	_rd = _rsize;
				//if( _rd + off < 0 )
				//	_rd = 0;
			}
			break;
		}
		return off;
	}

	_uint32 capacity()const
	{
		return _length;
	}

	// 返回未写入空间大小
	_uint32 space()const
	{
		return _wsize - _wd;
	}

	// 返回未读取空间大小
	_uint32 leave()const
	{
		return _rsize - _rd;
	}

	const char *rd_ptr()const
	{
		return _ptr + _rd;
	}

	const char *wd_ptr()const
	{
		return _ptr + _wd;
	}

	const char *base()const
	{
		return _ptr;
	}

	const char *back()const
	{
		return _ptr + _length;
	}

	_uint32 rd()const
	{
		return _rd;
	}

	_uint32 wd()const
	{
		return _wd;
	}

	int putn( const char *p, size_t n )
	{
		int cpy = __min( space(), n );
		if( cpy != n )
			throw( std::exception( "overflow" ) );
		memcpy( (void*)(_ptr+_wd), p, cpy );
		_wd += cpy;
		return cpy;
	}

	int getn( char *p, size_t n )
	{
		int cpy = __min( _rsize - rd(), n );
		if( cpy != n )
			throw( std::exception( "overflow" ) );
		memcpy( p, _ptr + rd(), n );
		_rd += cpy;
		return cpy;
	}

	bufstream& operator << ( const bufstream &stream )
	{
		_uint32 size = stream.wd();
		putn( (char*)&size, sizeof(_uint32) );
		putn( stream.base(), stream.wd() );
		return *this;
	}

	bufstream& operator >> ( bufstream &stream )
	{
		_uint32 size;
		getn( (char*)&size, sizeof(_uint32) );
		if( stream.base() )
			_rd += stream.putn( rd_ptr(), size );
		else if( stream.setbuf( rd_ptr(), size ) )
			_rd += size;
		return *this;
	}
private:
	const bool	_owner;		// 是否所有者
	const char	*_ptr;		// 缓冲区指针
	_uint32	_length;	// 缓冲区长度
	_uint32	_rsize;		// 读缓冲长度
	_uint32	_wsize;		// 写缓冲长度
	_uint32	_rd;		// 读偏移
	_uint32	_wd;		// 写偏移
};

EXPORT_LIB bufstream& operator << ( bufstream& stream, bool c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, char c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, short c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, int c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, long c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, long long c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, unsigned char c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, unsigned short c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, unsigned int c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, unsigned long c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, float c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, double c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, long double c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, const std::string& c );
EXPORT_LIB bufstream& operator << ( bufstream& stream, const std::wstring& c );

template< class type, class alloc >
bufstream& operator << ( bufstream& stream, const std::vector< type, alloc >& c )
{
	stream << c.size();
	std::vector< type, alloc >::const_iterator iter = c.begin();
	std::vector< type, alloc >::const_iterator iend = c.end();
	while( iter != iend )
		stream << *iter++;
	return stream;
}

template< class type, class alloc >
bufstream& operator << ( bufstream& stream, const std::vector< type*, alloc >& c )
{
	stream << c.size();
	std::vector< type*, alloc >::const_iterator iter = c.begin();
	std::vector< type*, alloc >::const_iterator iend = c.end();
	while( iter != iend )
		stream << **iter++;
	return stream;
}

template< class type, class alloc >
bufstream& operator << ( bufstream& stream, const std::list< type, alloc >& c )
{
	stream << c.size();
	std::list< type, alloc >::const_iterator iter = c.begin();
	std::list< type, alloc >::const_iterator iend = c.end();
	while( iter != iend )
		stream << *iter++;
	return stream;
}

template< class type, class alloc >
bufstream& operator << ( bufstream& stream, const std::list< type*, alloc >& c )
{
	stream << c.size();
	std::list< type*, alloc >::const_iterator iter = c.begin();
	std::list< type*, alloc >::const_iterator iend = c.end();
	while( iter != iend )
		stream << **iter++;
	return stream;
}

template< class type, class alloc >
bufstream& operator << ( bufstream& stream, const std::deque< type, alloc >& c )
{
	stream << c.size();
	std::deque< type, alloc >::const_iterator iter = c.begin();
	std::deque< type, alloc >::const_iterator iend = c.end();
	while( iter != iend )
		stream << *iter++;
	return stream;
}

template< class type, class alloc >
bufstream& operator << ( bufstream& stream, const std::deque< type*, alloc >& c )
{
	stream << c.size();
	std::deque< type*, alloc >::const_iterator iter = c.begin();
	std::deque< type*, alloc >::const_iterator iend = c.end();
	while( iter != iend )
		stream << **iter++;
	return stream;
}

template< class type, class compair, class alloc >
bufstream& operator << ( bufstream& stream, const std::set< type, compair, alloc >& c )
{
	stream << c.size();
	std::set< type, compair, alloc >::const_iterator iter = c.begin();
	std::set< type, compair, alloc >::const_iterator iend = c.end();
	while( iter != iend )
		stream << *iter++;
	return stream;
}

template< class type, class compair, class alloc >
bufstream& operator << ( bufstream& stream, const std::set< type*, compair, alloc >& c )
{
	stream << c.size();
	std::set< type*, compair, alloc >::const_iterator iter = c.begin();
	std::set< type*, compair, alloc >::const_iterator iend = c.end();
	while( iter != iend )
		stream << **iter++;
	return stream;
}

template< class key, class value, class compair, class alloc >
bufstream& operator << ( bufstream& stream, const std::map< key, value, compair, alloc >& c )
{
	stream << c.size();
	std::map< key, value, compair, alloc >::const_iterator iter = c.begin();
	std::map< key, value, compair, alloc >::const_iterator iend = c.end();
	while( iter != iend )
	{
		stream << *iter;
		++iter;
	}
	return stream;
}

template< class key, class value >
bufstream& operator << ( bufstream& stream, const std::pair< key, value >& c )
{
	return stream << c.first << c.second;
}

template< class key, class value >
bufstream& operator << ( bufstream& stream, const std::pair< key, value* >& c )
{
	return stream << c.first << *c.second;
}

EXPORT_LIB bufstream& operator >> ( bufstream& stream, bool &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, char &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, short &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, int &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, long &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, long long &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, unsigned char &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, unsigned short &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, unsigned int &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, unsigned long &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, float &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, double &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, long double &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, std::string &c );
EXPORT_LIB bufstream& operator >> ( bufstream& stream, std::wstring &c );

template< class type, class alloc >
bufstream& operator >> ( bufstream& stream, std::vector< type, alloc >& c )
{
	std::vector< type, alloc >::size_type size;
	stream >> size;
	c.resize( size );

	for( std::vector< type, alloc >::size_type i = 0; i < size; ++i )
	{
		stream >> c[i];
	}
	return stream;
}

template< class type, class alloc >
bufstream& operator >> ( bufstream& stream, std::vector< type*, alloc >& c )
{
	std::vector< type*, alloc >::size_type size;
	stream >> size;
	c.resize( size );

	for( std::vector< type*, alloc >::size_type i = 0; i < size; ++i )
	{
		c[i] = new type();
		stream >> *c[i];
	}
	return stream;
}

template< class type, class alloc >
bufstream& operator >> ( bufstream& stream, std::list< type, alloc >& c )
{
	std::list< type, alloc >::size_type		size;
	std::list< type, alloc >::value_type	value;
	stream >> size;

	for( std::list< type, alloc >::size_type i = 0; i < size; ++i )
	{
		stream >> value;
		c.push_back( value );
	}
	return stream;
}

template< class type, class alloc >
bufstream& operator >> ( bufstream& stream, std::list< type*, alloc >& c )
{
	std::list< type*, alloc >::size_type	size;
	std::list< type*, alloc >::value_type	_value;
	stream >> size;

	for( std::list< type*, alloc >::size_type i = 0; i < size; ++i )
	{
		_value = new type();
		stream >> *_value;
		c.push_back( _value );
	}
	return stream;
}

template< class type, class alloc >
bufstream& operator >> ( bufstream& stream, std::deque< type, alloc >& c )
{
	std::deque< type, alloc >::size_type	size;
	std::deque< type, alloc >::value_type	value;
	stream >> size;

	for( std::queue< type, alloc >::size_type i = 0; i < size; ++i )
	{
		stream >> value;
		c.push_back( value );
	}
	return stream;
}

template< class type, class alloc >
bufstream& operator >> ( bufstream& stream, std::deque< type*, alloc >& c )
{
	std::deque< type*, alloc >::size_type	size;
	std::deque< type*, alloc >::value_type	_value;
	stream >> size;

	for( std::queue< type*, alloc >::size_type i = 0; i < size; ++i )
	{
		_value = new type();
		stream >> *_value;
		c.push_back( _value );
	}
	return stream;
}

template< class type, class compair, class alloc >
bufstream& operator >> ( bufstream& stream, std::set< type, compair, alloc >& c )
{
	std::set< type, compair, alloc >::size_type	size;
	std::set< type, compair, alloc >::value_type	value;
	stream >> size;

	for( std::set< type, compair, alloc >::size_type i = 0; i < size; ++i )
	{
		stream >> value;
		c.insert( value );
	}

	return stream;
}

template< class type, class compair, class alloc >
bufstream& operator >> ( bufstream& stream, std::set< type*, compair, alloc >& c )
{
	std::set< type*, compair, alloc >::size_type		size;
	std::set< type*, compair, alloc >::value_type	_value;
	stream >> size;

	for( std::set< type*, compair, alloc >::size_type i = 0; i < size; ++i )
	{
		_value = new type();
		stream >> *_value;
		c.insert( _value );
	}

	return stream;
}

template< class key, class value, class compair, class alloc >
bufstream& operator >> ( bufstream& stream, std::map< key, value, compair, alloc >& c )
{
	size_t	size;
	std::pair< key, value > _value;
	stream >> size;

	for( size_t i = 0; i < size; ++i )
	{
		stream >> _value;
		c.insert( _value );
	}

	return stream;
}

template< class key, class value >
bufstream& operator >> ( bufstream& stream, std::pair< key, value >& c )
{
	stream >> c.first >> c.second;
	return stream;
}

template< class key, class value >
bufstream& operator >> ( bufstream& stream, std::pair< const key, value >& c )
{
	stream >> const_cast< key& >( c.first ) >> c.second;
	return stream;
}

template< class key, class value >
bufstream& operator >> ( bufstream& stream, std::pair< key, value* >& c )
{
	c.second = new value();
	stream >> c.first >> *c.second;
	return stream;
}

template< class key, class value >
bufstream& operator >> ( bufstream& stream, std::pair< const key, value* >& c )
{
	c.second = new value();
	stream >> const_cast< key& >( c.first ) >> *c.second;
	return stream;
}
