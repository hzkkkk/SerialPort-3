#ifndef COMDLL_INLINE_H_
#define COMDLL_INLINE_H_

#include "stdafx.h"

bool createCommNumber(TCHAR *buffer, size_t sizeOfBuffer,int num) {
	return _stprintf_s(buffer, sizeOfBuffer, _T("COM%d"), num) != -1;
}

bool createCommParam(TCHAR *buffer, size_t sizeOfBuffer) {
	return _stprintf_s(buffer, sizeOfBuffer, _T("baud=%d parity=%c data=%d stop=%d"), 115200, 'N', 8, 1) != -1;
}
#endif