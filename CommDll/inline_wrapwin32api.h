#ifndef INLINE_WRAPWIN32API_H_
#define INLINE_WRAPWIN32API_H_

#include "stdafx.h"

BOOL GetCommStateWrap(HANDLE hFile, LPDCB lpDCB, DWORD* lpLastError) {

    BOOL success = ::GetCommState(hFile, lpDCB);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
HANDLE CreateFileWrap(
    LPCTSTR lpFileName,                         // ファイル名
    DWORD dwDesiredAccess,                      // アクセスモード
    DWORD dwShareMode,                          // 共有モード
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // セキュリティ記述子
    DWORD dwCreationDisposition,                // 作成方法
    DWORD dwFlagsAndAttributes,                 // ファイル属性
    HANDLE hTemplateFile,                       // テンプレートファイルのハンドル
    DWORD* lpLastError) {
    HANDLE handle = ::CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    if (handle == INVALID_HANDLE_VALUE) {
        *lpLastError = ::GetLastError();
    }
    return handle;
}
BOOL BuildCommDCBWrap(
    LPCTSTR lpDef,  // デバイス制御文字列へのポインタ
    LPDCB lpDCB,    // デバイス制御ブロックへのポインタ
    DWORD* lpLastError) {
    BOOL success = ::BuildCommDCB(lpDef, lpDCB);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
BOOL SetCommStateWrap(
    HANDLE hFile,  // 通信デバイスのハンドル
    LPDCB lpDCB,    // DCB（ デバイス制御ブロック）構造体へのポインタ
    DWORD* lpLastError) {
    BOOL success = ::SetCommState(hFile, lpDCB);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
#endif
