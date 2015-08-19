#ifndef COMDLL_COMMUNIT_H
#define COMDLL_COMMUNIT_H

class AsyncIO;
class DLLAPI CommUnit
{
public:
    CommUnit();
    virtual ~CommUnit();

    bool Open(int comNumber);
    bool Close();
    bool Send(const char* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs);
    bool Recv(char* lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs);
#ifdef UNIT_TEST
public:
    HANDLE GetHandle();
#else
private:
#endif
private:
    CommUnit(const CommUnit&);
    void operator=(const CommUnit&);

    bool CloseHandle();

private:
    AsyncIO *asyncIO_;
    HANDLE comHandle_;
};

#endif

