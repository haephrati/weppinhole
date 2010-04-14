#pragma once
class sync_object
{
public:
	explicit sync_object()
		: m_object_h( NULL )
	{

	}

	virtual ~sync_object()
	{
		if (m_object_h != NULL)
		{
			::CloseHandle(m_object_h);
			m_object_h = NULL;
		}
	}

protected:
	HANDLE  m_object_h;

	// Attributes
public:
	operator HANDLE() const{ return m_object_h; }
};

/////////////////////////////////////////////////////////////////////////////
// CSemaphore

class semaphore : public sync_object
{
public:
	semaphore( long init = 1, long max = 1, _lpctstr name = NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL )
	{
		m_object_h = ::CreateSemaphore( lpsaAttributes, init, max, name );
		if( m_object_h == NULL )
			throw( std::exception( "create semaphore failed!" ) );
	}

	// Implementation
public:
	inline int unlock(long count, long* prevcount_ptr = NULL)
	{
		return ::ReleaseSemaphore( m_object_h, count, prevcount_ptr );
	}

	inline int unlock()
	{
		return unlock( 1, NULL );
	}
};

/////////////////////////////////////////////////////////////////////////////
// CMutex

class mutex : public sync_object
{
public:
	mutex(int owner = false, _lpctstr name = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
	{
		m_object_h = ::CreateMutex( lpsaAttribute, owner, name );
		if( m_object_h == NULL )
			throw( std::exception( "create mutex failed!" ) );
	}

	// Implementation
public:
	int unlock()
	{
		return ::ReleaseMutex( m_object_h );
	}
};

/////////////////////////////////////////////////////////////////////////////
// CEvent

class event : public sync_object
{
public:
	event( int init = false, int manual_reset = false, _lpctstr name = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
	{
		m_object_h = ::CreateEvent( lpsaAttribute, manual_reset, init, name );
		if( m_object_h == NULL )
			throw( std::exception( "create event failed!" ) );
	}

	// Operations
public:
	int set(){ return ::SetEvent(m_object_h); }
	int pulse(){ return ::PulseEvent(m_object_h); }
	int reset(){ return ::ResetEvent(m_object_h); }
};
