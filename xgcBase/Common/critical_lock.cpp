#include "stdafx.h"
#include "critical_lock.h"

CCritical::CCritical()
{
	InitializeCriticalSection( &m_lock );
}

CCritical::~CCritical()
{
	DeleteCriticalSection( &m_lock );
}

void CCritical::Lock()
{
	EnterCriticalSection( &m_lock );
}

void CCritical::UnLock()
{
	LeaveCriticalSection( &m_lock );
}

CCriticalLock::CCriticalLock( CRITICAL_SECTION& Critical )
	: m_CriticalRef( Critical )
{
	EnterCriticalSection( &m_CriticalRef );
}

CCriticalLock::~CCriticalLock(void)
{
	LeaveCriticalSection( &m_CriticalRef );
}

void CCriticalLock::operator=( CONST CRITICAL_SECTION& Critical )
{
	m_CriticalRef = Critical;
}

CCriticalTryLock::CCriticalTryLock( CRITICAL_SECTION& Critical )
:m_CriticalRef( Critical )
{
	m_bLocked = TryEnterCriticalSection( &Critical ) == TRUE;
}

CCriticalTryLock::~CCriticalTryLock(void)
{
	if( m_bLocked )
		LeaveCriticalSection( &m_CriticalRef );
}