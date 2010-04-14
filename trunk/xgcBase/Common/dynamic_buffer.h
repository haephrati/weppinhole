#pragma once
#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif
#include <vector>
#pragma warning( disable:4251 )
class EXPORT_LIB CDynamicBuffer
{
public:
	CDynamicBuffer( _uint32 chunksize = 4 * 1024 );
	~CDynamicBuffer();
	//--------------------------------------------------------------------------------------------------------//
	// 得到缓冲区地址
	// return	:	缓冲区地址
	//--------------------------------------------------------------------------------------------------------//
	char*	Get();

	//--------------------------------------------------------------------------------------------------------//
	// 得到缓冲区长度
	// return	:	缓冲区长度
	//--------------------------------------------------------------------------------------------------------//
	_uint32	Size()const;

	//--------------------------------------------------------------------------------------------------------//
	// 填充缓冲区
	// offset	:	缓冲区偏移量
	// buf		:	需要填入的缓冲区
	// size		:	缓冲区大小
	// return	:	实际写入的字节数
	//--------------------------------------------------------------------------------------------------------//
	_uint32	Fill( _uint32 offset, _lpcstr buf, _uint32 size );

	//--------------------------------------------------------------------------------------------------------//
	// 填充类型
	// offset	:	缓冲区偏移量
	// buf		:	值
	// bReversal:	是否反续
	// return	:	实际写入的字节数
	//--------------------------------------------------------------------------------------------------------//
	template< typename T >
	_uint32 FillType( _uint32 offset, T buf, bool bReversal = false )
	{
		return Fill( offset, (_lpcstr)&buf, sizeof( T ) );
	}

	//------------- Warning ---------------//
	// 如果使用指针类型 则会产生一个编译期错误, 用于提醒你， 使用指针是非法的！！
	// 产生错误的原因是因为没有返回值
	//--------------------------------------------------------------------------------------------------------//
	template< typename T >
	_uint32 FillType( _uint32 offset, T* buf, bool bReversal = true ){}

protected:
	void CleanUp();	

private:
	typedef std::vector< _lpstr >	CChunkVec;

	CChunkVec	m_chunkvec;		// 以16k为一个块组成的缓冲区列表
	char*		m_head;			// 整理缓冲，整理chunklist后所有的缓冲数据被放置在头所指的缓冲区中。
	_uint32		m_headsize;		// 整理缓冲区大小
	_uint32		m_size;			// 有效缓冲的大小。
	_uint32		m_chunksize;	// 块大小
};
