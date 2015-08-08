#ifndef COMDLL_INLINE_H
#define COMDLL_INLINE_H

#include "stdafx.h"

BOOL createCommNumber(TCHAR *buffer, size_t sizeOfBuffer,int num) {
	if ((_stprintf_s(buffer, sizeOfBuffer, _T("COM%d"), num)) == -1) {
		return FALSE;
	}
	return TRUE;
}
#endif