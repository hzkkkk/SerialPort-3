#ifndef _COMDLL_AUTOLOCK_H
#define _COMDLL_AUTOLOCK_H

class AutoLock
{
public:
    explicit AutoLock(CRITICAL_SECTION* lock);
    virtual ~AutoLock();
private:
    AutoLock(const AutoLock&); 
    void operator=(const AutoLock&);

    CRITICAL_SECTION* lock_;
};
#endif
