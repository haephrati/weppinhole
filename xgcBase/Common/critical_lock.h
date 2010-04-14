#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif

#ifndef _CRITICAL_LOCK_H
#define _CRITICAL_LOCK_H

class EXPORT_LIB CCritical
{
public:
	CCritical();
	~CCritical();

	void Lock();
	void UnLock();

	operator CRITICAL_SECTION&(){ return m_lock; }

private:
	CRITICAL_SECTION m_lock;
};

class EXPORT_LIB CCriticalLock
{
public:
	CCriticalLock( CRITICAL_SECTION& Critical );
	virtual ~CCriticalLock(void)throw();

	void operator=( CONST CCriticalLock& lock );
	void operator=( CONST CRITICAL_SECTION& Critical );
private:
	CRITICAL_SECTION& m_CriticalRef;
};

class EXPORT_LIB CCriticalTryLock
{
public:
	explicit CCriticalTryLock( CRITICAL_SECTION& Critical );
	virtual ~CCriticalTryLock(void);

	bool IsLock(){ return m_bLocked; }
private:
	CRITICAL_SECTION& m_CriticalRef;
	bool m_bLocked;
};
#endif //_CRITICAL_LOCK_H