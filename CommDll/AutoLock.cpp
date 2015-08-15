#include "stdafx.h"
#include "AutoLock.h"

AutoLock::AutoLock(CRITICAL_SECTION *lock)
    :lock_(lock)
{
    ::EnterCriticalSection(lock_);
}

AutoLock::~AutoLock()
{
    ::LeaveCriticalSection(lock_);
}
