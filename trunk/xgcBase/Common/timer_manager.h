#pragma once
//////////////////////////////////////////////////////////////////////////
// Loki 头文件
#include "loki\Function.h"
//#include "Loki\SmallObj.h"
//#include "loki\allocator.h"

#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif

//typedef Loki::SmallObject< LOKI_DEFAULT_THREADING_NO_OBJ_LEVEL, 1024*4, 1024, 4 >  CPoolAllocator;

//////////////////////////////////////////////////////////////////////////
// 定时器管理类
// base		:	基准时间
//////////////////////////////////////////////////////////////////////////
class EXPORT_LIB CTimerManager
{
public:
	CTimerManager(void);
	~CTimerManager(void);

public:
	void active();
	//////////////////////////////////////////////////////////////////////////
	// 定时器事件接口
	//////////////////////////////////////////////////////////////////////////
	typedef Loki::Function< bool (unsigned int /*handle*/, unsigned short& /*repeat*/, unsigned int& /*timer*/) > event_function;
	struct ITimerEvent	/*:	public CPoolAllocator*/
	{
		friend class CTimerManager;
	protected:
		//------------------------------------------------//
		// [7/28/2008] Write by Albert.xu
		// Description		: 构造时间事件对象
		// _function	: 函数指针或者重载operator()的结构体
		// _repeat		: 重复次数, 至少执行一次.
		// _timer		: 事件的触发时间间隔
		//------------------------------------------------//
		ITimerEvent( event_function &_function, unsigned short _repeat, unsigned int _timer, uintptr_t _user_ptr = 0 )
			: function( _function )
			, handle(-1)
			, repeat(_repeat)
			, timer(_timer)
			, index(0)
			, user_ptr( _user_ptr )
		{

		};

		virtual ~ITimerEvent()
		{
			function = NULL;
			handle	= -1;
			repeat	= 0;
			timer	= 0;
			index	= 0;
			user_ptr = 0;
		}

	public:
		__inline unsigned int get_timer()const	{ return timer; } 
		__inline unsigned int get_handle()const	{ return handle; } 
		__inline uintptr_t&	get_user_ptr(){ return user_ptr; }

	private:
		void release(){ delete this; }

		event_function	function;
		unsigned int	handle;	// 句柄
		unsigned int	timer;	// 时间间隔, 基准之间的倍数
		unsigned short	repeat;	// 执行次数, 执行次数为降为0 则被销毁
		unsigned short	index;	// 索引号, 代表在那个数组中
		uintptr_t		user_ptr;	// 用户数据

		size_t	round;	// 循环编号,编号和TimerManager的循环编号相等时,该对象才被执行.
	};

	typedef ITimerEvent	event_interface;
public:
	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	初始化时间事件管理器
	// length	:	容器大小,该值越大效率越高
	// return	:
	//------------------------------------------------//
	bool initialize( unsigned short length, unsigned short max_event_object );

	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	插入事件
	// event	:	事件对象
	// return	:	事件句柄
	//------------------------------------------------//
	__inline unsigned int insert_event( event_interface* event, unsigned int delay = 0 );

	//---------------------------------------------//
	// [11/21/2008 write by albert]
	// 函数用途	:	insert event
	// function	:	回调函数,可以是成员函数,仿函数或函数指针
	// repeat	:	重复次数
	// timer	:	间隔时间
	// return	:	定时器句柄
	//---------------------------------------------//
	unsigned int insert_event( event_function function, unsigned short repeat, unsigned int timer, unsigned int delay = 0 );

	//---------------------------------------------------//
	// [9/17/2009 Albert]
	// Description:	延迟处理定时器事件
	// handle	:	事件句柄
	// delay	:	延迟多少滴答
	//---------------------------------------------------//
	bool delay_event( unsigned int handle, unsigned int delay );

	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	得到事件对象
	// handle	:	事件句柄
	// return	:	事件对象,未找到返回NULL
	//------------------------------------------------//
	event_interface* get_event( unsigned int handle );

	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	删除事件
	// event	:	事件对象
	//------------------------------------------------//
	unsigned int remove_event( event_interface* event );

	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	删除事件
	// handle	:	事件句柄
	//------------------------------------------------//
	__inline unsigned int remove_event( unsigned int handle );

protected:
	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	插入对象到定时器列表中
	//	handle	:	时间事件对象句柄
	//------------------------------------------------//
	void inner_insert( unsigned int handle, unsigned int delay = 0 );

private:
	struct Impl;
	Impl* m_Impl;

	struct timer_handle
	{
		unsigned int	position:24;	// 位置位域
		unsigned int	round:8;		// 轮询位域
	};

	// 定时器变量
	size_t	m_next_list;	// 下一个列表
	size_t	m_timer_round;	// 下一个循环

	// 对象池变量
	size_t	m_object_count;	// 事件对象个数
	size_t	m_cur_pos;		// 当前空闲的单元
	size_t	m_round;		// 循环计数
};

#define INVALID_TIMER_HANDLE (-1)
#define TIMER_IMMEDIATELY (-1)
typedef CTimerManager::event_interface	TimerEvent;
typedef CTimerManager::event_function	TimerFunction;
// #include "TimerManager.inl"