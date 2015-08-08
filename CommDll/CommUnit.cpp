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
