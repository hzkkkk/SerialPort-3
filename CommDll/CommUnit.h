#ifndef COMDLL_COMMUNIT_H
#define COMDLL_COMMUNIT_H

class AsyncIO;
class DLLAPI CommUnit
{
public:
	CommUnit();
	virtual ~CommUnit();

	BOOL Open(int comNumber);
	BOOL Close();
    BOOL Send(const LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs);
    DWORD Recv(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs);
    DWORD GetLastError();
    LPTSTR GetLastErrorMsg();
    VOID FreeLastErrorMsg(LPTSTR msgBuf);

#ifdef UNIT_TEST
public:
    HANDLE GetHandle();
#else
private:
#endif

// endof unit test
private:
	BOOL CloseHandle();
    BOOL CreateCommHandle(TCHAR * portName);

private:
    AsyncIO *asyncIO_;
	HANDLE comHandle_;
	DWORD lastError_;
};

#endif

