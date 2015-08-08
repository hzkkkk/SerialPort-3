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
		success = createCommHandle(comName, &mComHandle);
	}

	DCB dcb = { 0 };
	if (success) {
		success = ::GetCommState(mComHandle, &dcb);
	}

	TCHAR def[BUFSIZ] = {0};
	if (success) {
		success = createCommParam(def, BUFSIZ);
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
