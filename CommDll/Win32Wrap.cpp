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
HANDLE CreateEventWrap(
    LPSECURITY_ATTRIBUTES lpEventAttributes, // �Z�L�����e�B�L�q�q
    BOOL bManualReset,                       // ���Z�b�g�̃^�C�v
    BOOL bInitialState,                      // �������
    LPCTSTR lpName,                           // �C�x���g�I�u�W�F�N�g�̖��O
    DWORD* lpLastError) {
    HANDLE success = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
DWORD WaitForSingleObjectWrap(
    HANDLE hHandle,        // �I�u�W�F�N�g�̃n���h��
    DWORD dwMilliseconds,   // �^�C���A�E�g����
    DWORD* lpLastError) {
    DWORD reason = ::WaitForSingleObject(hHandle, dwMilliseconds);
    if (!(reason == WAIT_ABANDONED || reason == WAIT_OBJECT_0 || reason == WAIT_TIMEOUT)) {
        *lpLastError = ::GetLastError();
    }
    return reason;
}
BOOL GetOverlappedResultWrap(
    HANDLE hFile,                       // �t�@�C���A�p�C�v�A�ʐM�f�o�C�X�̃n���h��
    LPOVERLAPPED lpOverlapped,          // �I�[�o�[���b�v�\����
    LPDWORD lpNumberOfBytesTransferred, // �]�����ꂽ�o�C�g��
    BOOL bWait,                         // �ҋ@�I�v�V����
    DWORD* lpLastError) {
    BOOL success = ::GetOverlappedResult(hFile, lpOverlapped, lpNumberOfBytesTransferred, bWait);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
BOOL CancelIoWrap(
    HANDLE hFile, // I/O ������������t�@�C���n���h��
    DWORD* lpLastError) {
    BOOL success = ::CancelIo(hFile);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
DWORD FormatMessageWrap(
    DWORD dwFlags,      // ���͌��Ə������@�̃I�v�V����
    LPCVOID lpSource,   // ���b�Z�[�W�̓��͌�
    DWORD dwMessageId,  // ���b�Z�[�W���ʎq
    DWORD dwLanguageId, // ���ꎯ�ʎq
    LPTSTR lpBuffer,    // ���b�Z�[�W�o�b�t�@
    DWORD nSize,        // ���b�Z�[�W�o�b�t�@�̍ő�T�C�Y
    DWORD* lpLastError,
    va_list *Arguments) {  // �����̃��b�Z�[�W�}���V�[�P���X����Ȃ�z��
    DWORD numbers = ::FormatMessage(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, Arguments);
    if (numbers == 0) {
        *lpLastError = ::GetLastError();
    }
    return numbers;
}
bool CreateFileB(
    LPCTSTR lpFileName,                         // �t�@�C����
    DWORD dwDesiredAccess,                      // �A�N�Z�X���[�h
    DWORD dwShareMode,                          // ���L���[�h
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // �Z�L�����e�B�L�q�q
    DWORD dwCreationDisposition,                // �쐬���@
    DWORD dwFlagsAndAttributes,                 // �t�@�C������
    HANDLE hTemplateFile,                       // �e���v���[�g�t�@�C���̃n���h��
    HANDLE* handle) {
    HANDLE got = ::CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    if (got != INVALID_HANDLE_VALUE) {
        *handle = got;
    }
    return got != INVALID_HANDLE_VALUE;
}

bool CreateEventB(
    LPSECURITY_ATTRIBUTES lpEventAttributes, // �Z�L�����e�B�L�q�q
    BOOL bManualReset,                       // ���Z�b�g�̃^�C�v
    BOOL bInitialState,                      // �������
    LPCTSTR lpName,                           // �C�x���g�I�u�W�F�N�g�̖��O
    HANDLE* handle) {
    HANDLE got = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
    if (!got) {
        *handle = got;
    }
    return got != NULL;
}

bool WaitForSingleObjectB(
    HANDLE hHandle,        // �I�u�W�F�N�g�̃n���h��
    DWORD dwMilliseconds,   // �^�C���A�E�g����
    DWORD* lpReason) {
    DWORD reason = ::WaitForSingleObject(hHandle, dwMilliseconds);
    if (reason == WAIT_ABANDONED || reason == WAIT_OBJECT_0 || reason == WAIT_TIMEOUT) {
        *lpReason = reason;
        return true;
    }
    return false;
}
bool FormatMessageB(
    DWORD dwFlags,      // ���͌��Ə������@�̃I�v�V����
    LPCVOID lpSource,   // ���b�Z�[�W�̓��͌�
    DWORD dwMessageId,  // ���b�Z�[�W���ʎq
    DWORD dwLanguageId, // ���ꎯ�ʎq
    LPTSTR lpBuffer,    // ���b�Z�[�W�o�b�t�@
    DWORD nSize,        // ���b�Z�[�W�o�b�t�@�̍ő�T�C�Y
    DWORD* written,
    va_list *Arguments) {  // �����̃��b�Z�[�W�}���V�[�P���X����Ȃ�z��
    DWORD number = ::FormatMessage(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, Arguments);
    if (0 <= number) {
        *written = number;
    }
    return !(number <= 0);
}