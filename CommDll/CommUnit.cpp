#include "stdafx.h"
#include "CommDll.h"
#include "CommUnit.h"
#include "inline.h"

CommUnit::CommUnit()
	:mComHandle(INVALID_HANDLE_VALUE)
{
}


CommUnit::~CommUnit()
{
	CloseHandle();
}

BOOL CommUnit::Open(int comNumber) {
	BOOL success = TRUE;

	TCHAR comName[BUFSIZ] = {0};
	success = createCommNumber(comName, BUFSIZ, comNumber);
	if (success) {
		mComHandle = ::CreateFile(
			comName, /* �V���A���|�[�g�̕����� */
			GENERIC_READ | GENERIC_WRITE, /* �A�N�Z�X���[�h */
			0,                            /* ���L���[�h */
			NULL,                         /* �Z�L�����e�B���� */
			OPEN_EXISTING,                /* �쐬�t���O */
			FILE_ATTRIBUTE_NORMAL,        /* ���� */
			NULL                          /* �e���v���[�g�̃n���h�� */
			);
		success = mComHandle != INVALID_HANDLE_VALUE;
	}

	DCB dcb = { 0 };
	if (success) {
		success = ::GetCommState(mComHandle, &dcb);
	}

	TCHAR def[BUFSIZ] = {0};
	if ((_stprintf_s(def, BUFSIZ, _T("baud=%d parity=%c data=%d stop=%d"), 115200, 'N', 8, 1)) == -1) {
		success = FALSE;
	}

	if (success) {
		success = ::BuildCommDCB(def, &dcb);
	}

	if (success) {
		success = ::SetCommState(mComHandle, &dcb);
	}
	return success;
}
BOOL CommUnit::Close() {
	return CloseHandle();
}



BOOL CommUnit::CloseHandle() {
	if (mComHandle != INVALID_HANDLE_VALUE) {
		BOOL success = ::CloseHandle(mComHandle);
		mComHandle = INVALID_HANDLE_VALUE;
		return success;
	}
	return TRUE;
}
