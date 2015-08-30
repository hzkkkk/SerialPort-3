#include <windows.h>
#include "Win32Wrap.h"
#include "inline.h"
#include "SerialIO.h"


SerialIO::SerialIO()
    :handle_(INVALID_HANDLE_VALUE)
{
    ::ZeroMemory(&readov_, sizeof(readov_));
    ::ZeroMemory(&writeov_, sizeof(writeov_));

    ::InitializeCriticalSection(&readlock_);
    ::InitializeCriticalSection(&writelock_);
}

SerialIO::~SerialIO()
{
    CloseSerialHandle();
    if (readov_.hEvent != INVALID_HANDLE_VALUE) {
        TryWin32(::CloseHandle(readov_.hEvent), __FUNCTION__, __LINE__);
    }
    if (writeov_.hEvent != INVALID_HANDLE_VALUE) {
        TryWin32(::CloseHandle(writeov_.hEvent), __FUNCTION__, __LINE__);
    }

    ::DeleteCriticalSection(&readlock_);
    ::DeleteCriticalSection(&writelock_);
}

bool SerialIO::Open(const TCHAR* name, const TCHAR* param)
{
    bool success = TryWin32(CreateEventB(NULL, FALSE, FALSE, NULL, &readov_.hEvent), __FUNCTION__, __LINE__);

    if (success) {
        success = TryWin32(CreateEventB(NULL, FALSE, FALSE, NULL, &writeov_.hEvent), __FUNCTION__, __LINE__);
    }

    if (success) {
        success = TryWin32(CreateFileB(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL, &handle_), __FUNCTION__, __LINE__);
    }

    DCB dcb = { 0 };
    if (success) {
        success = TryWin32(::GetCommState(handle_, &dcb), __FUNCTION__, __LINE__);
    }

    if (success) {
        success = TryWin32(::BuildCommDCB(param, &dcb), __FUNCTION__, __LINE__);
    }

    if (success) {
        success = TryWin32(::SetCommState(handle_, &dcb), __FUNCTION__, __LINE__);
    }
    return success;
}

bool SerialIO::Close()
{
    return   CloseSerialHandle();
}

int SerialIO::ReadChunk(char** lpBuffer, int* outlen, DWORD dwTimeoutMs)
{

    bool success = true;
    int result = IO_SUCCESS;
    DWORD firstread = 0;
    char c = 0;
    if ((result = Read(&c, 1, dwTimeoutMs, &firstread)) != IO_SUCCESS) {
        success = false;
    }

    COMSTAT stat = { 0 };
    DWORD dwErrors = 0;
    if (success) {
        if (!(success = TryClearCommError(handle_, &dwErrors, &stat, __FUNCTION__, __LINE__))) {
            result = IO_ERROR;
        }
    }

    char* buffer = NULL;
    DWORD seconderead = 0;
    if (success) {
        buffer = new char[stat.cbInQue + 1];
        buffer[0] = c;
        if (stat.cbInQue) {
            if ((result = Read(buffer + 1, stat.cbInQue, 0, &seconderead)) != IO_SUCCESS) {
                success = false;
            }
        }
    }

    *lpBuffer = success ? buffer : NULL;
    *outlen = success ? firstread + seconderead : 0;

    return result;
}

int SerialIO::Read( char* lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs, DWORD* readlen)
{

    bool success = TryWin32AsyncIO(::ReadFile(handle_, lpBuffer, nNumberOfBytesToRead, NULL, &readov_), __FUNCTION__, __LINE__);
    DWORD reason = 0;
    if (success) {
        success = TryWin32(WaitForSingleObjectB(readov_.hEvent, dwTimeoutMs, &reason), __FUNCTION__, __LINE__);
    }
    if (success) {
        if (reason == WAIT_OBJECT_0) {
            success = TryWin32(::GetOverlappedResult(handle_, &readov_, readlen, TRUE), __FUNCTION__, __LINE__);
        }
        else if (reason == WAIT_TIMEOUT) {
            success = TryWin32(::CancelIo(handle_), __FUNCTION__, __LINE__);
        }
    }

    if (success && reason == WAIT_OBJECT_0) {
        return IO_SUCCESS;
    }
    else if (success && reason == WAIT_TIMEOUT) {
        return IO_TIME_OUT;
    }
    return IO_ERROR;
}

int SerialIO::Write( const char* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs, DWORD* written)
{

    bool success = TryWin32AsyncIO(::WriteFile(handle_, lpBuffer, nNumberOfBytesToWrite, NULL, &writeov_), __FUNCTION__, __LINE__);
    DWORD reason = 0;
    if (success) {
        success = TryWin32(WaitForSingleObjectB(writeov_.hEvent, dwTimeoutMs, &reason), __FUNCTION__, __LINE__);
    }

    if (success) {
        if (reason == WAIT_OBJECT_0) {
            success = TryWin32(::GetOverlappedResult(handle_, &writeov_, written, TRUE), __FUNCTION__, __LINE__);
        }
        else if (reason == WAIT_TIMEOUT) {
            success = TryWin32(::CancelIo(handle_), __FUNCTION__, __LINE__);
        }
    }

    if (success && reason == WAIT_OBJECT_0) {
        return IO_SUCCESS;
    }
    else if (success && reason == WAIT_TIMEOUT) {
        return IO_TIME_OUT;
    }
    return IO_ERROR;
}

bool SerialIO::IsInitialized()
{
    return !(readov_.hEvent == NULL || writeov_.hEvent == NULL);
}

bool SerialIO::CloseSerialHandle() {
    if (handle_ == INVALID_HANDLE_VALUE) {
        return true;
    }
    BOOL success = TryWin32(::CloseHandle(handle_), __FUNCTION__, __LINE__);
    handle_ = INVALID_HANDLE_VALUE;
    return success;
}

