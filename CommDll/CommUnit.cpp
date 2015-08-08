#include "stdafx.h"
#include "CommDll.h"
#include "CommUnit.h"
#include "inline.h"
#include "inline_wrapwin32api.h"

CommUnit::CommUnit()
    :mComHandle(INVALID_HANDLE_VALUE)
    , mLastError(0)
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
        success = GetCommStateWrap(mComHandle, &dcb, &mLastError);
    }

    TCHAR def[BUFSIZ] = { 0 };
    if (success) {
        success = createCommParam(def, BUFSIZ);
    }

    if (success) {
        success = BuildCommDCBWrap(def, &dcb, &mLastError);
    }

    if (success) {
        success = SetCommStateWrap(mComHandle, &dcb,&mLastError);
    }
    return success;
}
BOOL CommUnit::Close() {
    return CloseHandle();
}

DWORD CommUnit::GetLastError()
{
    return mLastError;
}

BOOL CommUnit::CloseHandle() {
    if (mComHandle != INVALID_HANDLE_VALUE) {
        BOOL success = ::CloseHandle(mComHandle);
        mComHandle = INVALID_HANDLE_VALUE;
        return success;
    }
    return TRUE;
}

BOOL CommUnit::CreateCommHandle(TCHAR *portName) {
    mComHandle = CreateFileWrap(
        portName, /* シリアルポートの文字列 */
        GENERIC_READ | GENERIC_WRITE, /* アクセスモード */
        0,                            /* 共有モード */
        NULL,                         /* セキュリティ属性 */
        OPEN_EXISTING,                /* 作成フラグ */
        FILE_ATTRIBUTE_NORMAL,        /* 属性 */
        NULL,                          /* テンプレートのハンドル */
        &mLastError);

    return mComHandle != INVALID_HANDLE_VALUE;
}
