#ifndef COMDLL_COMMUNIT_H
#define COMDLL_COMMUNIT_H

class DLLAPI CommUnit
{
public:
	CommUnit();
	virtual ~CommUnit();

	BOOL Open(int comNumber);
	BOOL Close();
    DWORD GetLastError();

#ifdef UNIT_TEST
public:
#else
private:
#endif


// endof unit test
private:
	BOOL CloseHandle();
    BOOL CreateCommHandle(TCHAR * portName);

private:
	HANDLE comHandle_;
	DWORD lastError_;
};

#endif

