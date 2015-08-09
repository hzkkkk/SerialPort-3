#include "stdafx.h"
#include "CommDll.h"
#include "inline.h"
#include "Win32Wrap.h"

CommUnit::CommUnit()
    :comHandle_(INVALID_HANDLE_VALUE)
    , lastError_(0)
{
}


CommUnit::~CommUnit()
{
    CloseHandle();
}

BOOL CommUnit::Open(int comNumber) {
    BOOL success = TRUE;

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
        success = SetCommStateWrap(comHandle_, &dcb,&lastError_);
    }
    return success;
}
BOOL CommUnit::Close() {
    return CloseHandle();
}

DWORD CommUnit::GetLastError()
{
    return lastError_;
}


BOOL CommUnit::CloseHandle() {
    if (comHandle_ != INVALID_HANDLE_VALUE) {
        BOOL success = ::CloseHandle(comHandle_);
        comHandle_ = INVALID_HANDLE_VALUE;
        return success;
    }
    return TRUE;
}

BOOL CommUnit::CreateCommHandle(TCHAR *portName) {
    comHandle_ = CreateFileWrap(
        portName, /* シリアルポートの文字列 */
        GENERIC_READ | GENERIC_WRITE, /* アクセスモード */
        0,                            /* 共有モード */
        NULL,                         /* セキュリティ属性 */
        OPEN_EXISTING,                /* 作成フラグ */
        FILE_FLAG_OVERLAPPED,        /* 属性 */
        NULL,                          /* テンプレートのハンドル */
        &lastError_);

    return comHandle_ != INVALID_HANDLE_VALUE;
}

#ifdef UNIT_TEST
HANDLE CommUnit::GetHandle()
{
    return comHandle_;
}
#endif