#include "stdafx.h"
#include "CommDll.h"

AutoLock::AutoLock(CRITICAL_SECTION *lock)
    :lock_(lock)
{
    ::EnterCriticalSection(lock_);
}

AutoLock::~AutoLock()
{
    ::LeaveCriticalSection(lock_);
}
