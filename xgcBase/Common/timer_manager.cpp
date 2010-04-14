#include "stdafx.h"
#include "timer_manager.h"

struct CTimerManager::Impl
{
	typedef std::list	< unsigned int, ALLOCATOR< unsigned int > >		CTimerEventList;
	typedef std::vector	< CTimerEventList, ALLOCATOR< CTimerEventList > >	CTimerEventListVec;
	typedef std::vector	< event_interface*, ALLOCATOR< event_interface* > >	CTimerEventObjectVec;

	CTimerEventListVec		m_TimerVec;
	CTimerEventObjectVec	m_EventVec;
};

CTimerManager::CTimerManager(void)
: m_object_count(0)
, m_cur_pos(0x01000000)
, m_round(1)
, m_next_list(0)
, m_timer_round(0)
, m_Impl( new Impl() )
{
}

CTimerManager::~CTimerManager(void)
{
	m_Impl->m_TimerVec.clear();
	for( size_t i = 0; i < m_Impl->m_EventVec.size(); ++i )
	{
		TimerEvent* event = m_Impl->m_EventVec.at(i);
		delete event;
	}
	m_Impl->m_EventVec.clear();

	SAFE_DELETE( m_Impl );
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	初始化时间事件管理器
// length	:	容器大小,该值越大效率越高
// return	:
//------------------------------------------------//
bool CTimerManager::initialize( unsigned short length, unsigned short max_event_object )
{
	m_Impl->m_TimerVec.resize( length );
	m_Impl->m_EventVec.resize( max_event_object, 0 );
	return true;
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	插入事件
// event	:	事件对象
// return	:	事件句柄
//------------------------------------------------//
unsigned int CTimerManager::insert_event( event_interface* event, unsigned int delay )
{
	if( m_object_count == m_Impl->m_EventVec.size() ) return -1;
	if( !event ) return -1;

	if( delay == TIMER_IMMEDIATELY )
	{
		// 立即执行的timer事件
		bool ret = event->function( INVALID_TIMER_HANDLE, event->repeat, event->timer );
		if( ret || event->repeat == 0 )
		{
			return INVALID_TIMER_HANDLE;
		}
	}

	// 将对象注册到对象池中
	event->handle = (unsigned int)m_cur_pos;
	timer_handle& h = (timer_handle&)m_cur_pos;

	m_Impl->m_EventVec[h.position] = event;
	do
	{
		++h.position;
		if( h.position >= m_Impl->m_EventVec.size() )
		{
			++h.round;
			++m_round;
			h.position = 0;
		}
	}while( m_Impl->m_EventVec[h.position] != NULL );

	++m_object_count;

	// 按对象中的数据插入到刷新列表中
	inner_insert( event->handle, delay );
//	printf("******************* inner_insert(%d)\n", event->handle);
	return event->handle;
}

//---------------------------------------------//
// [11/21/2008 write by albert]
// 函数用途	:	insert event
// function	:	回调函数,可以是成员函数,仿函数或函数指针
// repeat	:	重复次数
// timer	:	间隔时间
// return	:	定时器句柄
//---------------------------------------------//
unsigned int CTimerManager::insert_event( event_function function, unsigned short repeat, unsigned int timer, unsigned int delay )
{
	return insert_event( new TimerEvent( function, repeat, timer ), delay );
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	得到事件对象
// handle	:	事件句柄
// return	:	事件对象,未找到返回NULL
//------------------------------------------------//
CTimerManager::event_interface* CTimerManager::get_event( unsigned int handle )
{
	timer_handle& h = (timer_handle&)handle;
	if( h.position < m_Impl->m_EventVec.size() )
	{
		ITimerEvent* event = m_Impl->m_EventVec[h.position];
		if( event && event->handle == handle )
		{
			return event;
		}
	}
	return NULL;
}

//---------------------------------------------------//
// [9/17/2009 Albert]
// Description:	延迟处理定时器事件
// handle	:	事件句柄
// delay	:	延迟多少滴答
//---------------------------------------------------//
bool CTimerManager::delay_event( unsigned int handle, unsigned int delay )
{
	timer_handle& h = (timer_handle&)handle;
	if( h.position < m_Impl->m_EventVec.size() )
	{
		ITimerEvent* event = m_Impl->m_EventVec[h.position];
		if( event && event->handle == handle )
		{
			inner_insert( handle, event->timer + delay );
			return true;
		}
	}

	return false;
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	删除事件
// event	:	事件对象
//------------------------------------------------//
unsigned int CTimerManager::remove_event( event_interface* event )
{
	if( event )
	{
		timer_handle& h = (timer_handle&)event->handle;
		event_interface* _event = m_Impl->m_EventVec[h.position];
		if( _event == event )
		{
			// 计算剩余滴答
			unsigned int ret = event->index - m_next_list + ( event->round - m_timer_round )*m_Impl->m_TimerVec.size();

			m_Impl->m_EventVec[h.position] = NULL;
			event->release();
			--m_object_count;

			return ret;
		}
	}
	return 0;
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	删除事件
// handle	:	事件句柄
//------------------------------------------------//
unsigned int CTimerManager::remove_event( unsigned int handle )
{
	ITimerEvent* event = get_event( handle );
	return remove_event( event );
}

///function	:	超时处理函数
///return	:	无
void CTimerManager::active()
{
	Impl::CTimerEventList& timer_list = m_Impl->m_TimerVec[m_next_list++];
	Impl::CTimerEventList::iterator iter = timer_list.begin();
	while( iter != timer_list.end() )
	{
		Impl::CTimerEventList::iterator olditer = iter++;
		unsigned int handle = *olditer;
		ITimerEvent* event = get_event( handle );
		if( !event )
		{
			timer_list.erase( olditer );
			continue;
		}

		if( event->round == m_timer_round )
		{
			unsigned int handle = event->handle;
			bool ret = event->function( handle, event->repeat==-1?event->repeat:--event->repeat, event->timer);
			// [9/18/2008 Albert]
			// 有可能已经被删除了, 这里需要判断是否被删除.
			if( get_event( handle ) )
			{
				if( ret || event->repeat == 0 )
				{
					remove_event( event );
				}
				else
				{
					inner_insert( event->handle, event->timer );
				}
			}
			timer_list.erase( olditer );
		}
	}

	if( m_next_list >= m_Impl->m_TimerVec.size() )
	{
		++m_timer_round;
		m_next_list = 0;
	}
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	插入对象到定时器列表中
//	handle	:	时间事件对象句柄
//------------------------------------------------//
void CTimerManager::inner_insert( unsigned int handle, unsigned int delay )
{
	ITimerEvent* event = get_event( handle );
	ASSERT( event );
	if( !event ) 
		return;
	
	size_t index = delay + m_next_list;
	size_t round = m_timer_round + index/m_Impl->m_TimerVec.size();
	index = index%m_Impl->m_TimerVec.size();
	event->index = (unsigned short)index;
	event->round = (unsigned short)round;
	m_Impl->m_TimerVec[index].push_back( handle );
}