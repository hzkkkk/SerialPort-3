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
HANDLE CreateEventWrap(
    LPSECURITY_ATTRIBUTES lpEventAttributes, // セキュリティ記述子
    BOOL bManualReset,                       // リセットのタイプ
    BOOL bInitialState,                      // 初期状態
    LPCTSTR lpName,                           // イベントオブジェクトの名前
    DWORD* lpLastError) {
    HANDLE success = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
DWORD WaitForSingleObjectWrap(
    HANDLE hHandle,        // オブジェクトのハンドル
    DWORD dwMilliseconds,   // タイムアウト時間
    DWORD* lpLastError) {
    DWORD reason = ::WaitForSingleObject(hHandle, dwMilliseconds);
    if (!(reason == WAIT_ABANDONED || reason == WAIT_OBJECT_0 || reason == WAIT_TIMEOUT)) {
        *lpLastError = ::GetLastError();
    }
    return reason;
}
BOOL GetOverlappedResultWrap(
    HANDLE hFile,                       // ファイル、パイプ、通信デバイスのハンドル
    LPOVERLAPPED lpOverlapped,          // オーバーラップ構造体
    LPDWORD lpNumberOfBytesTransferred, // 転送されたバイト数
    BOOL bWait,                         // 待機オプション
    DWORD* lpLastError) {
    BOOL success = ::GetOverlappedResult(hFile, lpOverlapped, lpNumberOfBytesTransferred, bWait);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
BOOL CancelIoWrap(
    HANDLE hFile, // I/O 操作を取り消すファイルハンドル
    DWORD* lpLastError) {
    BOOL success = ::CancelIo(hFile);
    if (!success) {
        *lpLastError = ::GetLastError();
    }
    return success;
}
DWORD FormatMessageWrap(
    DWORD dwFlags,      // 入力元と処理方法のオプション
    LPCVOID lpSource,   // メッセージの入力元
    DWORD dwMessageId,  // メッセージ識別子
    DWORD dwLanguageId, // 言語識別子
    LPTSTR lpBuffer,    // メッセージバッファ
    DWORD nSize,        // メッセージバッファの最大サイズ
    DWORD* lpLastError,
    va_list *Arguments) {  // 複数のメッセージ挿入シーケンスからなる配列
    DWORD numbers = ::FormatMessage(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, Arguments);
    if (numbers == 0) {
        *lpLastError = ::GetLastError();
    }
    return numbers;
}
bool CreateFileB(
    LPCTSTR lpFileName,                         // ファイル名
    DWORD dwDesiredAccess,                      // アクセスモード
    DWORD dwShareMode,                          // 共有モード
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // セキュリティ記述子
    DWORD dwCreationDisposition,                // 作成方法
    DWORD dwFlagsAndAttributes,                 // ファイル属性
    HANDLE hTemplateFile,                       // テンプレートファイルのハンドル
    HANDLE* handle) {
    HANDLE got = ::CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    if (got != INVALID_HANDLE_VALUE) {
        *handle = got;
    }
    return got != INVALID_HANDLE_VALUE;
}

bool CreateEventB(
    LPSECURITY_ATTRIBUTES lpEventAttributes, // セキュリティ記述子
    BOOL bManualReset,                       // リセットのタイプ
    BOOL bInitialState,                      // 初期状態
    LPCTSTR lpName,                           // イベントオブジェクトの名前
    HANDLE* handle) {
    HANDLE got = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
    if (!got) {
        *handle = got;
    }
    return got != NULL;
}

bool WaitForSingleObjectB(
    HANDLE hHandle,        // オブジェクトのハンドル
    DWORD dwMilliseconds,   // タイムアウト時間
    DWORD* lpReason) {
    DWORD reason = ::WaitForSingleObject(hHandle, dwMilliseconds);
    if (reason == WAIT_ABANDONED || reason == WAIT_OBJECT_0 || reason == WAIT_TIMEOUT) {
        *lpReason = reason;
        return true;
    }
    return false;
}
bool FormatMessageB(
    DWORD dwFlags,      // 入力元と処理方法のオプション
    LPCVOID lpSource,   // メッセージの入力元
    DWORD dwMessageId,  // メッセージ識別子
    DWORD dwLanguageId, // 言語識別子
    LPTSTR lpBuffer,    // メッセージバッファ
    DWORD nSize,        // メッセージバッファの最大サイズ
    DWORD* written,
    va_list *Arguments) {  // 複数のメッセージ挿入シーケンスからなる配列
    DWORD number = ::FormatMessage(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, Arguments);
    if (0 <= number) {
        *written = number;
    }
    return !(number <= 0);
}