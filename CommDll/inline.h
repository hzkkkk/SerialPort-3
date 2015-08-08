#ifndef COMDLL_INLINE_H
#define COMDLL_INLINE_H

#include "stdafx.h"

BOOL createCommNumber(TCHAR *buffer, size_t sizeOfBuffer,int num) {
	return _stprintf_s(buffer, sizeOfBuffer, _T("COM%d"), num) != -1;
}
BOOL createCommHandle(TCHAR *portName, HANDLE* commHandle) {
	*commHandle = ::CreateFile(
		portName, /* �V���A���|�[�g�̕����� */
		GENERIC_READ | GENERIC_WRITE, /* �A�N�Z�X���[�h */
		0,                            /* ���L���[�h */
		NULL,                         /* �Z�L�����e�B���� */
		OPEN_EXISTING,                /* �쐬�t���O */
		FILE_ATTRIBUTE_NORMAL,        /* ���� */
		NULL                          /* �e���v���[�g�̃n���h�� */
		);

	return *commHandle != INVALID_HANDLE_VALUE;
}

BOOL createCommParam(TCHAR *buffer, size_t sizeOfBuffer) {
	return _stprintf_s(buffer, sizeOfBuffer, _T("baud=%d parity=%c data=%d stop=%d"), 115200, 'N', 8, 1) != -1;
}
#endif