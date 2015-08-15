#ifndef COMDLL_AUTOLOCK_H
#define COMDLL_AUTOLOCK_H

class AutoLock
{
public:
    AutoLock(CRITICAL_SECTION* lock);
	virtual ~AutoLock();
private:
   CRITICAL_SECTION* lock_;
};
#endif
