#ifndef COMDLL_INLINE_H
#define COMDLL_INLINE_H

#include "stdafx.h"

BOOL createCommNumber(TCHAR *buffer, size_t sizeOfBuffer,int num) {
	return _stprintf_s(buffer, sizeOfBuffer, _T("COM%d"), num) != -1;
}
BOOL createCommHandle(TCHAR *portName, HANDLE* commHandle) {
	*commHandle = ::CreateFile(
		portName, /* シリアルポートの文字列 */
		GENERIC_READ | GENERIC_WRITE, /* アクセスモード */
		0,                            /* 共有モード */
		NULL,                         /* セキュリティ属性 */
		OPEN_EXISTING,                /* 作成フラグ */
		FILE_ATTRIBUTE_NORMAL,        /* 属性 */
		NULL                          /* テンプレートのハンドル */
		);

	return *commHandle != INVALID_HANDLE_VALUE;
}

BOOL createCommParam(TCHAR *buffer, size_t sizeOfBuffer) {
	return _stprintf_s(buffer, sizeOfBuffer, _T("baud=%d parity=%c data=%d stop=%d"), 115200, 'N', 8, 1) != -1;
}
#endif