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
			comName, /* シリアルポートの文字列 */
			GENERIC_READ | GENERIC_WRITE, /* アクセスモード */
			0,                            /* 共有モード */
			NULL,                         /* セキュリティ属性 */
			OPEN_EXISTING,                /* 作成フラグ */
			FILE_ATTRIBUTE_NORMAL,        /* 属性 */
			NULL                          /* テンプレートのハンドル */
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
