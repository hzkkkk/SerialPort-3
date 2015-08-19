#include "stdafx.h"
#include "CommDll.h"
#include "AutoLock.h"
#include "Win32Wrap.h"
#include "AsyncIO.h"

AsyncIO::AsyncIO()
{
    ::ZeroMemory(&readov_, sizeof(readov_));
    ::ZeroMemory(&writeov_, sizeof(writeov_));

    ::InitializeCriticalSection(&readlock_);
    ::InitializeCriticalSection(&writelock_);
}

AsyncIO::~AsyncIO()
{
    if (readov_.hEvent != INVALID_HANDLE_VALUE) {
        TryWin32(::CloseHandle(readov_.hEvent), __FILE__, __LINE__);
    }
    if (writeov_.hEvent != INVALID_HANDLE_VALUE) {
        TryWin32(::CloseHandle(writeov_.hEvent), __FILE__, __LINE__);
    }

    ::DeleteCriticalSection(&readlock_);
    ::DeleteCriticalSection(&writelock_);
}

bool AsyncIO::Init()
{
    bool success = TryWin32(CreateEventB(NULL, FALSE, FALSE, NULL, &readov_.hEvent), __FILE__, __LINE__);
    if (success) {
       success = TryWin32(CreateEventB(NULL, FALSE, FALSE, NULL, &writeov_.hEvent), __FILE__, __LINE__);
    }
    return success;
}

int AsyncIO::Read(HANDLE handle ,char* lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs)
{
    AutoLock lock(&readlock_);

    DWORD nread = 0;
    bool success = TryWin32AsyncIO(::ReadFile(handle, lpBuffer, nNumberOfBytesToRead, &nread, &readov_), __FILE__, __LINE__);
    DWORD reason = 0;
    if (success) {
        success = TryWin32(WaitForSingleObjectB(readov_.hEvent, dwTimeoutMs, &reason), __FILE__, __LINE__);
    }
    if (success) {
        if (reason == WAIT_OBJECT_0) {
            success = TryWin32(::GetOverlappedResult(handle, &readov_, &nread, TRUE), __FILE__, __LINE__);
        }
        else if (reason == WAIT_TIMEOUT) {
            TryWin32(::CancelIo(handle), __FILE__, __LINE__);
        }
    }
    return nread;
}

int AsyncIO::Write(HANDLE handle, const char* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs)
{
    AutoLock lock(&writelock_);

    DWORD nwritten = 0;
    bool success = TryWin32AsyncIO(::WriteFile(handle, lpBuffer, nNumberOfBytesToWrite, &nwritten, &writeov_), __FILE__, __LINE__);
    DWORD reason = 0;
    if (success) {
        success = TryWin32(WaitForSingleObjectB(writeov_.hEvent, dwTimeoutMs, &reason), __FILE__, __LINE__);
    }

    if (success) {
        if (reason == WAIT_OBJECT_0) {
            success = TryWin32(::GetOverlappedResult(handle, &writeov_, &nwritten, TRUE), __FILE__, __LINE__);
        }
        else if (reason == WAIT_TIMEOUT) {
            TryWin32(CancelIo(handle), __FILE__, __LINE__);
        }
    }
    return nwritten;
}

bool AsyncIO::IsInitialized()
{
    return !(readov_.hEvent == NULL || writeov_.hEvent == NULL);
}
