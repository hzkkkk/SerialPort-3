#ifndef _SERIAL_IO_H_
#define _SERIAL_IO_H_

#ifdef COMMDLL_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

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

