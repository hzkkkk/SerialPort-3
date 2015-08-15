#ifndef COMDLL_ASYNCIO_H
#define COMDLL_ASYNCIO_H


class DLLAPI AsyncIO
{
public:
    AsyncIO();
    virtual ~AsyncIO();
    int Read(HANDLE handle, char* lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs);
    int Write(HANDLE handle, const char* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs);
private:
    AsyncIO(const AsyncIO&); 
    void operator=(const AsyncIO&);

    DWORD lastError_;
    OVERLAPPED readov_;
    OVERLAPPED writeov_;
    CRITICAL_SECTION readlock_;
    CRITICAL_SECTION writelock_;
};
#endif

