#include "stdafx.h"
#include "dynamic_buffer.h"

//--------------------------------------------------------------------------------------------------------//
// CDynamicBuffer
// 动态缓冲区，不需要预先分配缓冲区大小
//--------------------------------------------------------------------------------------------------------//
CDynamicBuffer::CDynamicBuffer( _uint32 chunksize/* = 4 * 1024*/ )
: m_chunksize( chunksize )
, m_head( (char*)malloc(chunksize) )
, m_headsize( chunksize )
, m_size( 0 )
{
	memset( m_head, 0, chunksize );
}


CDynamicBuffer::~CDynamicBuffer()
{
	CChunkVec::const_iterator citer = m_chunkvec.begin();
	while( citer != m_chunkvec.end() )
	{
		free(*citer);
		++citer;
	}

	if( m_head )
	{
		free( m_head );
		m_head = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------//
// 得到缓冲区地址
// return	:	缓冲区地址
//--------------------------------------------------------------------------------------------------------//
_lpstr CDynamicBuffer::Get()
{
	CleanUp();
	return m_head;
}

//--------------------------------------------------------------------------------------------------------//
// 得到缓冲区长度
// return	:	缓冲区长度
//--------------------------------------------------------------------------------------------------------//
_uint32 CDynamicBuffer::Size()const
{
	return m_size;
}

//--------------------------------------------------------------------------------------------------------//
// 填充缓冲区
// offset	:	缓冲区偏移量
// buf		:	需要填入的缓冲区
// size		:	缓冲区大小
// return	:	实际写入的字节数
//--------------------------------------------------------------------------------------------------------//
_uint32	CDynamicBuffer::Fill( _uint32 offset, _lpcstr buf, _uint32 size )
{
	_uint32 write_count = 0;

	// Buffer 的起始数据在head所指的缓冲区中，另外，
	if( offset < m_headsize )
	{
		_uint32 release = m_headsize - offset;
		_uint32 write = min( release, size );
		_lpstr head_buf = m_head + offset;
		memcpy( head_buf, buf + write_count, write );

		write_count += write;
		size -= write;
		offset += write;
	}

	while( size )
	{
		_uint32 chunkindex = (offset-m_headsize)/m_chunksize;
		if( m_chunkvec.size() <= chunkindex )
		{
			m_chunkvec.resize( chunkindex + 1, NULL );
		}

		char* chunk_buf = m_chunkvec.at( chunkindex );
		if( chunk_buf == NULL )
		{
			chunk_buf = (_lpstr)malloc(m_chunksize);
			m_chunkvec.at( chunkindex ) = chunk_buf;
		}

		_uint32 release = m_chunksize - ((offset-m_headsize)%m_chunksize);

		_uint32 write =  min( release, size );

		chunk_buf += (offset-m_headsize) % m_chunksize;
		memcpy( chunk_buf, buf + write_count, write );

		write_count += write;
		size -= write;
		offset += write;
	}
	m_size = max( m_size, offset + size );
	return write_count;
}

//--------------------------------------------------------------------------------------------------------//
// 整理缓冲区链
//--------------------------------------------------------------------------------------------------------//
void CDynamicBuffer::CleanUp()
{
	_uint32 size = (_uint32)m_chunkvec.size();
	_uint32 inc = size * m_chunksize;
	if( inc > 0 )
	{
		size_t offset = m_headsize;
		m_headsize += inc;
		_lpstr buf = (_lpstr)realloc( m_head, m_headsize );

		ASSERT( buf );
		if( buf )
		{
			m_head	= buf;

			buf += offset;
			CChunkVec::iterator iter = m_chunkvec.begin();
			while( m_chunkvec.end() != iter )
			{
				if( *iter == NULL )
				{
					memset( buf, 0, m_chunksize );
				}
				else
				{
					memcpy( buf, *iter, m_chunksize );
					free(*iter);
					*iter = NULL;
				}
				buf += m_chunksize;
				++iter;
			}
			m_chunkvec.clear();
		}
	}
}
//--------------------------------------------------------------------------------------------------------//

