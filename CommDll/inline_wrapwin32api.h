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
    LPCTSTR lpFileName,                         // �t�@�C����
    DWORD dwDesiredAccess,                      // �A�N�Z�X���[�h
    DWORD dwShareMode,                          // ���L���[�h
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // �Z�L�����e�B�L�q�q
    DWORD dwCreationDisposition,                // �쐬���@
    DWORD dwFlagsAndAttributes,                 // �t�@�C������
    HANDLE hTemplateFile,                       // �e���v���[�g�t�@�C���̃n���h��
    DWORD* lpLastError) {
    HANDLE handle = ::CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    if (handle == INVALID_HANDLE_VALUE) {
        *lpLastError = ::GetLastError();
    }
    return handle;
}
BOOL BuildCommDCBWrap(
    LPCTSTR lpDef,  // �f�o�C�X���䕶����ւ̃|�C���^
    LPDCB lpDCB,    // �f�o�C�X����u���b�N�ւ̃|�C���^
    DWORD* lpLastError) {
    BOOL success = ::BuildCommDCB(lpDef, lpDCB);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
BOOL SetCommStateWrap(
    HANDLE hFile,  // �ʐM�f�o�C�X�̃n���h��
    LPDCB lpDCB,    // DCB�i �f�o�C�X����u���b�N�j�\���̂ւ̃|�C���^
    DWORD* lpLastError) {
    BOOL success = ::SetCommState(hFile, lpDCB);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
#endif
