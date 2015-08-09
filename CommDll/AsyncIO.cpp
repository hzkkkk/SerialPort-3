#include "stdafx.h"
#include "CommDll.h"
#include "Win32Wrap.h"

AsyncIO::AsyncIO()
    : lastError_(0)
{
    ::ZeroMemory(&readov_, sizeof(readov_));
    ::ZeroMemory(&writeov_, sizeof(writeov_));
    readov_.hEvent = CreateEventWrap(NULL, FALSE, FALSE, NULL, &lastError_);
    writeov_.hEvent = CreateEventWrap(NULL, FALSE, FALSE, NULL, &lastError_);

    ::InitializeCriticalSection(&readlock_);
    ::InitializeCriticalSection(&writelock_);
}

AsyncIO::~AsyncIO()
{
    if (readov_.hEvent != INVALID_HANDLE_VALUE) {
        ::CloseHandle(readov_.hEvent);
    }
    if (writeov_.hEvent != INVALID_HANDLE_VALUE) {
        ::CloseHandle(writeov_.hEvent);
    }

    ::DeleteCriticalSection(&readlock_);
    ::DeleteCriticalSection(&writelock_);
}

DWORD AsyncIO::Read(HANDLE handle, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs)
{
    AutoLock lock(&readlock_);

    BOOL success = TRUE;
    DWORD nread = 0;
    ::ReadFile(handle, lpBuffer, nNumberOfBytesToRead, &nread, &readov_);
    if ((lastError_ = GetLastError()) != ERROR_IO_PENDING) {
        return 0;
    }
    DWORD reason = WaitForSingleObjectWrap(readov_.hEvent, dwTimeoutMs, &lastError_);
    if (reason == WAIT_OBJECT_0) {
        success = GetOverlappedResultWrap(handle, &readov_, &nread, TRUE, &lastError_);
    }
    else if (reason == WAIT_TIMEOUT) {
        CancelIoWrap(handle,&lastError_);
        return 0;
    }
    return nread;
}

DWORD AsyncIO::Write(HANDLE handle, LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs)
{
    AutoLock lock(&writelock_);

    BOOL success = TRUE;
    DWORD nwritten = 0;
    ::WriteFile(handle, lpBuffer, nNumberOfBytesToWrite, &nwritten, &writeov_);
    if ((lastError_ = GetLastError()) != ERROR_IO_PENDING) {
        return 0;
    }
    DWORD reason = WaitForSingleObjectWrap(writeov_.hEvent, dwTimeoutMs, &lastError_);
    if (reason == WAIT_OBJECT_0) {
        success = GetOverlappedResultWrap(handle, &writeov_, &nwritten, TRUE, &lastError_);
    }
    else if (reason == WAIT_TIMEOUT) {
        CancelIoWrap(handle,&lastError_);
        return 0;
    }
    return nwritten;
}

