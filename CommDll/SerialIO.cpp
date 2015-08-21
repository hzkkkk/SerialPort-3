#include "stdafx.h"
#include "CommDll.h"
#include "AutoLock.h"
#include "Win32Wrap.h"
#include "inline.h"
#include "SerialIO.h"
#include "atlstr.h"

#include <iostream>

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

bool SerialIO::ReadChunk(char** lpBuffer, int* outlen, DWORD dwTimeoutMs)
{
    AutoLock lock(&readlock_);

    bool success = TryWin32(::PurgeComm(handle_, PURGE_RXCLEAR | PURGE_RXABORT), __FUNCTION__, __LINE__);
    char c = 0;
    int readlen = 0;
    if (success) {
        readlen = Read(handle_, &c, 1, dwTimeoutMs);
        success = readlen != 0;
    }

    COMSTAT stat = { 0 };
    DWORD dwErrors = 0;
    if (success) {
        success = TryClearCommError(handle_, &dwErrors, &stat, __FUNCTION__, __LINE__);
    }

    char* buffer = NULL;
    if (success) {
        buffer = new char[stat.cbInQue + 1];
        buffer[0] = c;
        readlen += Read(handle_, buffer + 1, stat.cbInQue, 0);
    }

    *lpBuffer = success ? buffer : NULL;
    *outlen = success ? readlen : 0;

    return success;
}
int SerialIO::Read(HANDLE handle, char* lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs)
{
    AutoLock lock(&readlock_);

    DWORD nread = 0;
    bool success = TryWin32AsyncIO(::ReadFile(handle, lpBuffer, nNumberOfBytesToRead, &nread, &readov_), __FUNCTION__, __LINE__);
    DWORD reason = 0;
    if (success) {
        success = TryWin32(WaitForSingleObjectB(readov_.hEvent, dwTimeoutMs, &reason), __FUNCTION__, __LINE__);
    }
    if (success) {
        if (reason == WAIT_OBJECT_0) {
            success = TryWin32(::GetOverlappedResult(handle, &readov_, &nread, TRUE), __FUNCTION__, __LINE__);
        }
        else if (reason == WAIT_TIMEOUT) {
            TryWin32(::CancelIo(handle), __FUNCTION__, __LINE__);
        }
    }
    return nread;
}

int SerialIO::Write(HANDLE handle, const char* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs)
{
    AutoLock lock(&writelock_);

    DWORD nwritten = 0;
    bool success = TryWin32AsyncIO(::WriteFile(handle, lpBuffer, nNumberOfBytesToWrite, &nwritten, &writeov_), __FUNCTION__, __LINE__);
    DWORD reason = 0;
    if (success) {
        success = TryWin32(WaitForSingleObjectB(writeov_.hEvent, dwTimeoutMs, &reason), __FUNCTION__, __LINE__);
    }

    if (success) {
        if (reason == WAIT_OBJECT_0) {
            success = TryWin32(::GetOverlappedResult(handle, &writeov_, &nwritten, TRUE), __FUNCTION__, __LINE__);
        }
        else if (reason == WAIT_TIMEOUT) {
            TryWin32(CancelIo(handle), __FUNCTION__, __LINE__);
        }
    }
    return nwritten;
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

