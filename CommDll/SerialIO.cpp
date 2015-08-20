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
        TryWin32(::CloseHandle(readov_.hEvent), __FILE__, __LINE__);
    }
    if (writeov_.hEvent != INVALID_HANDLE_VALUE) {
        TryWin32(::CloseHandle(writeov_.hEvent), __FILE__, __LINE__);
    }

    ::DeleteCriticalSection(&readlock_);
    ::DeleteCriticalSection(&writelock_);
}

bool SerialIO::Open(int number)
{
    // TODO __FILE__ -> __FUNC__
    // TODO COMMparamÇìnÇ∑
    // TODO COMMnameÇìnÇ∑
    bool success = TryWin32(CreateEventB(NULL, FALSE, FALSE, NULL, &readov_.hEvent), __FILE__, __LINE__);

    if (success) {
        success = TryWin32(CreateEventB(NULL, FALSE, FALSE, NULL, &writeov_.hEvent), __FILE__, __LINE__);
    }

    TCHAR comName[BUFSIZ] = { 0 };
    if (success) {
        success = Try(createCommNumber(comName, BUFSIZ, number), __FILE__, __LINE__);
    }

    if (success) {
        success = TryWin32(CreateFileB(comName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL, &handle_), __FILE__, __LINE__);
    }

    DCB dcb = { 0 };
    if (success) {
        success = TryWin32(::GetCommState(handle_, &dcb), __FILE__, __LINE__);
    }

    TCHAR def[BUFSIZ] = { 0 };
    if (success) {
        success = Try(createCommParam(def, BUFSIZ), __FILE__, __LINE__);
    }

    if (success) {
        success = TryWin32(::BuildCommDCB(def, &dcb), __FILE__, __LINE__);
    }

    if (success) {
        success = TryWin32(::SetCommState(handle_, &dcb), __FILE__, __LINE__);
    }
    return success;
}

bool SerialIO::ReadByEvent(char** lpBuffer, int* outlen, DWORD dwTimeoutMs)
{
    //TODO lock
    bool success = TryWin32(::PurgeComm(handle_, PURGE_RXCLEAR | PURGE_RXABORT), __FILE__, __LINE__);
    char c = 0;
    int len = 0;
    if (success) {
        len = Read(handle_, &c, 1, dwTimeoutMs);
        if (len == 0) {
            success = false;
        }
    }
    COMSTAT stat = { 0 };
    DWORD dwErrors = 0;
    if (success) {
        success = TryWin32(::ClearCommError(handle_, &dwErrors, &stat), __FILE__, __LINE__);
    }
    if (success) {
        if (dwErrors != 0) {
            success = false;
        }
    }
    char* buffer = NULL;
    if (success) {
        buffer = new char[stat.cbInQue + 1];
        // TODO bufferÇÃÉTÉCÉY
        buffer[0] = c;
        int read = Read(handle_, buffer + 1, stat.cbInQue, 0);
        if (read != stat.cbInQue) {
            //TODO è¡Ç∑
            std::cout << read << "@@@@@@@@@@@:" << stat.cbInQue << std::endl;

        }
        *lpBuffer = buffer;
        *outlen = stat.cbInQue + 1;
    }
    if (!success) {
        *lpBuffer = NULL;
        *outlen = 0;

    }
    return success;
}
int SerialIO::Read(HANDLE handle, char* lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs)
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

int SerialIO::Write(HANDLE handle, const char* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs)
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

bool SerialIO::IsInitialized()
{
    return !(readov_.hEvent == NULL || writeov_.hEvent == NULL);
}

bool SerialIO::CloseSerialHandle() {
    if (handle_ == INVALID_HANDLE_VALUE) {
        return true;
    }
    BOOL success = TryWin32(::CloseHandle(handle_), __FILE__, __LINE__);
    handle_ = INVALID_HANDLE_VALUE;
    return success;
}

