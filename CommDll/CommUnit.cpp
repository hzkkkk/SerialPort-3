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
        portName, /* �V���A���|�[�g�̕����� */
        GENERIC_READ | GENERIC_WRITE, /* �A�N�Z�X���[�h */
        0,                            /* ���L���[�h */
        NULL,                         /* �Z�L�����e�B���� */
        OPEN_EXISTING,                /* �쐬�t���O */
        FILE_ATTRIBUTE_NORMAL,        /* ���� */
        NULL,                          /* �e���v���[�g�̃n���h�� */
        &mLastError);

    return mComHandle != INVALID_HANDLE_VALUE;
}
