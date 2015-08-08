#include "stdafx.h"
#include "CommDll.h"
#include "CommUnit.h"

CommUnit::CommUnit()
	:mComHandle(INVALID_HANDLE_VALUE)
{
}


CommUnit::~CommUnit()
{
	CloseHandle();
}

BOOL CommUnit::Open(int comNumber) {
	TCHAR comName[BUFSIZ] = {0};
	BOOL success = TRUE;

	if ((_stprintf_s(comName, BUFSIZ, _T("COM%d"), comNumber)) == -1) {
		success = FALSE;
	}
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
