#include "stdafx.h"
#include "CommDll.h"
#include "AsyncIO.h"
#include "AutoLock.h"
#include "inline.h"
#include "Win32Wrap.h"

CommUnit::CommUnit()
    :comHandle_(INVALID_HANDLE_VALUE)
{
    asyncIO_ = new AsyncIO();
}


CommUnit::~CommUnit()
{
    delete asyncIO_;
    CloseHandle();
}

bool CommUnit::Open(int comNumber) {
    bool success = TRUE;

    TCHAR comName[BUFSIZ] = { 0 };
    success = Try(createCommNumber(comName, BUFSIZ, comNumber), __FUNCTION__, __LINE__);
    if (success) {
        success = TryWin32(CreateFileB(comName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL, &comHandle_), __FUNCTION__, __LINE__);
    }

    DCB dcb = { 0 };
    if (success) {
        success = TryWin32(::GetCommState(comHandle_, &dcb), __FUNCTION__, __LINE__);
    }

    TCHAR def[BUFSIZ] = { 0 };
    if (success) {
        success = Try(createCommParam(def, BUFSIZ), __FUNCTION__, __LINE__);
    }

    if (success) {
        success = TryWin32(::BuildCommDCB(def, &dcb), __FUNCTION__, __LINE__);
    }

    if (success) {
        success = TryWin32(::SetCommState(comHandle_, &dcb), __FUNCTION__, __LINE__);
    }
    asyncIO_->Init();

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

bool CommUnit::CloseHandle() {
    if (comHandle_ == INVALID_HANDLE_VALUE) {
        return true;
    }
    BOOL success = TryWin32(::CloseHandle(comHandle_), __FUNCTION__, __LINE__);
    comHandle_ = INVALID_HANDLE_VALUE;
    return success;
}

#ifdef UNIT_TEST
HANDLE CommUnit::GetHandle()
{
    return comHandle_;
}
#endif