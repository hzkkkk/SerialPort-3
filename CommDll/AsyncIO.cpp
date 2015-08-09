#include "stdafx.h"
#include "CommDll.h"
#include "Win32Wrap.h"

AsyncIO::AsyncIO()
    : lastError_(0)
{
    ::ZeroMemory(&ov_, sizeof(ov_));
    ov_.hEvent = CreateEventWrap(NULL, FALSE, FALSE, NULL, &lastError_);
}

AsyncIO::~AsyncIO()
{
}

DWORD AsyncIO::Read(HANDLE handle, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs)
{
    BOOL success = TRUE;
    DWORD nread = 0;
    ::ReadFile(handle, lpBuffer, nNumberOfBytesToRead, &nread, &ov_);
    if ((lastError_ = GetLastError()) != ERROR_IO_PENDING) {
        return 0;
    }
    DWORD reason = WaitForSingleObjectWrap(ov_.hEvent, dwTimeoutMs, &lastError_);
    if (reason == WAIT_OBJECT_0) {
        success = GetOverlappedResultWrap(handle, &ov_, &nread, TRUE, &lastError_);
    }
    else if (reason == WAIT_TIMEOUT) {
        CancelIo(handle);
        return 0;
    }
    return nread;
}

