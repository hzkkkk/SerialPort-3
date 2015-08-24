#ifndef _SERIAL_IO_H_
#define _SERIAL_IO_H_


class DLLAPI SerialIO
{
public:
    SerialIO();
    virtual ~SerialIO();
    bool Open(const TCHAR*, const TCHAR*);
    bool Close();
    int ReadChunk(char**, int*, DWORD);
    int Read(char*, DWORD, DWORD, DWORD*);
    int Write(const char*, DWORD, DWORD, DWORD*);
    enum {
        IO_SUCCESS,
        IO_TIME_OUT,
        IO_ERROR,
    };

private:
    SerialIO(const AsyncIO&);
    void operator=(const SerialIO&);
    bool IsInitialized();
    bool CloseSerialHandle();

    HANDLE handle_;
    OVERLAPPED readov_;
    OVERLAPPED writeov_;
    CRITICAL_SECTION readlock_;
    CRITICAL_SECTION writelock_;
};
#endif

