#include "StdAfx.h"
#include "bufstream.h"
#include <set>
#include <map>
#include <list>
#include <deque>
#include <vector>

#define BUFSTREAM_IN_BODY( TYPE )	{ stream.putn( (char*)&c, sizeof(TYPE) ); return stream; }
#define BUFSTREAM_OUT_BODY( TYPE )	{ stream.getn( (char*)&c, sizeof(TYPE) ); return stream; }

bufstream& operator << ( bufstream& stream, bool c )			BUFSTREAM_IN_BODY( bool );
bufstream& operator << ( bufstream& stream, char c )			BUFSTREAM_IN_BODY( char );
bufstream& operator << ( bufstream& stream, short c )			BUFSTREAM_IN_BODY( short );
bufstream& operator << ( bufstream& stream, int c )				BUFSTREAM_IN_BODY( int );
bufstream& operator << ( bufstream& stream, long c )			BUFSTREAM_IN_BODY( long );
bufstream& operator << ( bufstream& stream, long long c )		BUFSTREAM_IN_BODY( long long );
bufstream& operator << ( bufstream& stream, unsigned char c )	BUFSTREAM_IN_BODY( unsigned char );
bufstream& operator << ( bufstream& stream, unsigned short c )	BUFSTREAM_IN_BODY( unsigned short );
bufstream& operator << ( bufstream& stream, unsigned int c )	BUFSTREAM_IN_BODY( unsigned int );
bufstream& operator << ( bufstream& stream, unsigned long c )	BUFSTREAM_IN_BODY( unsigned long );
bufstream& operator << ( bufstream& stream, float c )			BUFSTREAM_IN_BODY( float );
bufstream& operator << ( bufstream& stream, double c )			BUFSTREAM_IN_BODY( double );
bufstream& operator << ( bufstream& stream, long double c )		BUFSTREAM_IN_BODY( long double );
bufstream& operator << ( bufstream& stream, const std::string &c )
{
	stream.putn( c.c_str(), c.length()+1 );
	return stream;
}

bufstream& operator << ( bufstream& stream, const std::wstring &c )
{
	stream.putn( (char*)c.c_str(), (c.length()<<1)+sizeof( wchar_t/*std::wstring::traits_type::char_type*/ ) );
	return stream;
}

bufstream& operator >> ( bufstream& stream, bool &c )			BUFSTREAM_OUT_BODY( bool );
bufstream& operator >> ( bufstream& stream, char &c )			BUFSTREAM_OUT_BODY( char );
bufstream& operator >> ( bufstream& stream, short &c )			BUFSTREAM_OUT_BODY( short );
bufstream& operator >> ( bufstream& stream, int &c )			BUFSTREAM_OUT_BODY( int );
bufstream& operator >> ( bufstream& stream, long &c )			BUFSTREAM_OUT_BODY( long );
bufstream& operator >> ( bufstream& stream, long long &c )		BUFSTREAM_OUT_BODY( long long );
bufstream& operator >> ( bufstream& stream, unsigned char &c )	BUFSTREAM_OUT_BODY( unsigned char );
bufstream& operator >> ( bufstream& stream, unsigned short &c )	BUFSTREAM_OUT_BODY( unsigned short );
bufstream& operator >> ( bufstream& stream, unsigned int &c )	BUFSTREAM_OUT_BODY( unsigned int );
bufstream& operator >> ( bufstream& stream, unsigned long &c )	BUFSTREAM_OUT_BODY( unsigned long );
bufstream& operator >> ( bufstream& stream, float &c )			BUFSTREAM_OUT_BODY( float );
bufstream& operator >> ( bufstream& stream, double &c )			BUFSTREAM_OUT_BODY( double );
bufstream& operator >> ( bufstream& stream, long double &c )	BUFSTREAM_OUT_BODY( long double );
bufstream& operator >> ( bufstream& stream, std::string &c )
{
	c = stream.rd_ptr();
	stream.seekoff( c.length()+1, bufstream::cur, bufstream::out );
	return stream;
}

bufstream& operator >> ( bufstream& stream, std::wstring &c )
{
	c = (wchar_t*)stream.rd_ptr();
	stream.seekoff( (c.length()<<1)+sizeof(wchar_t), bufstream::cur, bufstream::out );
	return stream;
}

#include <set>
#include <map>
#include <queue>
#include <deque>
struct __ttt__
{
	int a;
	int b;
};

bufstream& operator << ( bufstream& stream, const __ttt__& c )
{
	return stream << c.a << c.b;
}

bufstream& operator >> ( bufstream& stream, __ttt__& c )
{
	return stream >> c.a >> c.b;
}

void do_bufstream_test()
{
	bufstream buf( 1024 );
	int n[11] = {1,2,3,4,5,6,7,8,9,0,1};
	buf << 1 << 0.5f << 43l << short(558);
	buf << "1234656";

	buf << bufstream( (char*)n, sizeof(n), bufstream::in, sizeof(n) );

	std::vector< int > vec_in, vec_out;
	std::list<int> list_in, list_out;
	std::deque<int> deque_in, deque_out;
	std::set<int> set_in, set_out;
	std::map<int, int> map_in, map_out;

	std::vector< __ttt__* > pvec_in, pvec_out;
	std::list< __ttt__* >	plist_in, plist_out;
	std::deque< __ttt__* >	pdeque_in, pdeque_out;
	std::set< __ttt__* >	pset_in, pset_out;
	std::map<int, __ttt__*>	pmap_in, pmap_out;
	for( int i = 0; i < 10; ++i )
	{
		__ttt__ *p = new __ttt__;
		p->a = 1 * i;
		p->b = 2 * i;
		vec_in.push_back(i);
		pvec_in.push_back(p);

		list_in.push_back(i);
		plist_in.push_back(p);

		deque_in.push_back(i);
		pdeque_in.push_back(p);

		set_in.insert(i);
		pset_in.insert(p);

		map_in.insert( std::make_pair( i, i ) );
		pmap_in.insert( std::make_pair( i, p ) );
	}
	buf << vec_in << pvec_in;
	buf << list_in << plist_in;
	buf << deque_in << pdeque_in;
	buf << set_in << pset_in;
	buf << map_in << pmap_in;

	int a;
	float b;
	long c;
	short d;
	std::string e;
	int on[11];
	buf >> a >> b >> c >> d >> e;
	buf >> bufstream( (const char*)on, sizeof(on) );
	buf >> vec_out >> pvec_out 
		>> list_out >> plist_out
		>> deque_out >> pdeque_out
		>> set_out >> pset_out
		>> map_out >> pmap_out;
}