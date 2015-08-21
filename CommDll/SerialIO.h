#ifndef _SERIAL_IO_H_
#define _SERIAL_IO_H_


class DLLAPI SerialIO
{
public:
    SerialIO();
    virtual ~SerialIO();
    bool Open(const TCHAR*, const TCHAR*);
    bool Close();
    bool ReadChunk(char**, int*, DWORD);
    int Read(HANDLE, char*, DWORD, DWORD);
    int Write(HANDLE, const char*, DWORD, DWORD);
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

