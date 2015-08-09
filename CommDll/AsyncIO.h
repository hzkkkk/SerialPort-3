#ifndef COMDLL_ASYNCIO_H
#define COMDLL_ASYNCIO_H


class DLLAPI AsyncIO
{
public:
    AsyncIO();
    virtual ~AsyncIO();
    DWORD Read(HANDLE handle, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs);
    DWORD Write(HANDLE handle, const LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs);
private:
    DWORD lastError_;
    OVERLAPPED readov_;
    OVERLAPPED writeov_;
    CRITICAL_SECTION readlock_;
    CRITICAL_SECTION writelock_;
};
#endif

