#ifndef COMDLL_ASYNCIO_H
#define COMDLL_ASYNCIO_H


class DLLAPI AsyncIO
{
public:
    AsyncIO();
    virtual ~AsyncIO();
    bool Init();
    int Read(HANDLE, char*, DWORD, DWORD);
    int Write(HANDLE, const char*, DWORD, DWORD);
private:
    AsyncIO(const AsyncIO&);
    void operator=(const AsyncIO&);

    bool IsInitialized();

    OVERLAPPED readov_;
    OVERLAPPED writeov_;
    CRITICAL_SECTION readlock_;
    CRITICAL_SECTION writelock_;
};
#endif

