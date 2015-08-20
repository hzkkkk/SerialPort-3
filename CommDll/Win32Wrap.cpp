#include "stdafx.h"
#include <iostream>

static void PrintErrorMsg(DWORD, const char *, int);
static LPTSTR GetLastErrorMsg(DWORD);

bool CreateFileB(
    LPCTSTR lpFileName,                         // ファイル名
    DWORD dwDesiredAccess,                      // アクセスモード
    DWORD dwShareMode,                          // 共有モード
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // セキュリティ記述子
    DWORD dwCreationDisposition,                // 作成方法
    DWORD dwFlagsAndAttributes,                 // ファイル属性
    HANDLE hTemplateFile,                       // テンプレートファイルのハンドル
    HANDLE* handle)
{
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
    HANDLE* handle)
{
    HANDLE got = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
    if (got) {
        *handle = got;
    }
    return got != NULL;
}

bool WaitForSingleObjectB(
    HANDLE hHandle,        // オブジェクトのハンドル
    DWORD dwMilliseconds,   // タイムアウト時間
    DWORD* lpReason)
{
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
    if (0 <= number)
    {
        *written = number;
    }
    return !(number <= 0);
}

bool TryWin32(bool success, const char * function, int linenumber)
{
    if (!success) {
        DWORD dwLastError = ::GetLastError();
        PrintErrorMsg(dwLastError, function, linenumber);
    }
    return success;
}

bool TryWin32AsyncIO(bool success, const char * function, int linenumber)
{
    if (!success) {
        DWORD dwLastError = ::GetLastError();
        if (dwLastError != ERROR_IO_PENDING) {
            PrintErrorMsg(dwLastError, function, linenumber);
            return false;
        }
    }
    return true;
}

bool Try(bool success, const char * function, int linenumber)
{
    if (!success) {
        std::wcout << "reason = [error]" << "func=" << function << " line=" << linenumber << std::endl;
    }
    return success;
}

static void PrintErrorMsg(DWORD dwLastError, const char * function, int linenumber)
{
    LPTSTR msg = GetLastErrorMsg(dwLastError);
    if (msg) {
        std::wcout << "reason = [" << msg << "]" << "func=" << function << " line=" << linenumber << std::endl;
    }
    ::LocalFree(msg);
}

static LPTSTR GetLastErrorMsg(DWORD dwLastError)
{
    LPTSTR lpMsgBuf = NULL;
    if (FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwLastError,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        reinterpret_cast<LPTSTR>(&lpMsgBuf),
        0,
        NULL)) {
        return lpMsgBuf;
    }
    return NULL;
}
