#include "stdafx.h"
#include "CommDll.h"
#include "AsyncIO.h"
#include "AutoLock.h"
#include "inline.h"
#include "Win32Wrap.h"

CommUnit::CommUnit()
    :comHandle_(INVALID_HANDLE_VALUE)
    , lastError_(0)
{
    asyncIO_ = new AsyncIO();
}


CommUnit::~CommUnit()
{
    CloseHandle();
}

bool CommUnit::Open(int comNumber) {
    bool success = TRUE;

    TCHAR comName[BUFSIZ] = { 0 };
    success = createCommNumber(comName, BUFSIZ, comNumber);
    if (success) {
        success = CreateCommHandle(comName);
    }

    DCB dcb = { 0 };
    if (success) {
        success = GetCommStateWrap(comHandle_, &dcb, &lastError_);
    }

    TCHAR def[BUFSIZ] = { 0 };
    if (success) {
        success = createCommParam(def, BUFSIZ);
    }

    if (success) {
        success = BuildCommDCBWrap(def, &dcb, &lastError_);
    }

    if (success) {
        success = SetCommStateWrap(comHandle_, &dcb, &lastError_);
    }
    return success;
}
bool CommUnit::Close() {
    return CloseHandle();
}

bool CommUnit::Send(const char* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD dwTimeoutMs)
{
    DWORD written = asyncIO_->Write(comHandle_, lpBuffer, nNumberOfBytesToWrite, dwTimeoutMs);
    return written == nNumberOfBytesToWrite;
}

bool CommUnit::Recv(char* lpBuffer, DWORD nNumberOfBytesToRead, DWORD dwTimeoutMs)
{
    DWORD read = asyncIO_->Read(comHandle_, lpBuffer, nNumberOfBytesToRead, dwTimeoutMs);
    return read == nNumberOfBytesToRead;
}

DWORD CommUnit::GetLastError()
{
    return lastError_;
}

LPTSTR CommUnit::GetLastErrorMsg()
{
    LPTSTR lpMsgBuf = NULL;
    FormatMessageWrap(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        lastError_,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        reinterpret_cast<LPTSTR>(&lpMsgBuf),
        0,
        &lastError_,
        NULL
        );

    return lpMsgBuf;
}

void CommUnit::FreeLastErrorMsg(LPTSTR msgBuf)
{
    ::LocalFree(msgBuf);
}


bool CommUnit::CloseHandle() {
    if (comHandle_ != INVALID_HANDLE_VALUE) {
        BOOL success = ::CloseHandle(comHandle_);
        comHandle_ = INVALID_HANDLE_VALUE;
        return success;
    }
    return TRUE;
}

bool CommUnit::CreateCommHandle(TCHAR *portName) {
    comHandle_ = CreateFileWrap(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL, &lastError_);

    return comHandle_ != INVALID_HANDLE_VALUE;
}

#ifdef UNIT_TEST
HANDLE CommUnit::GetHandle()
{
    return comHandle_;
}
#endif