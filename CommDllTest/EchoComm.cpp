#include "stdafx.h"
#include <process.h>
#include "EchoComm.h"

static bool stop = false;

static const TCHAR *sname;
static const TCHAR* sparam;
static HANDLE shandle;
static HANDLE thread;

static bool CreateCom();

void ThreadShutdown()
{
    stop = true;
    WaitForSingleObject(thread, INFINITE);
}

static unsigned __stdcall Echo(void* )
{
    if (!CreateCom()) {
        return 1;
    }
    while (!stop) {
        char c;
        DWORD read = 0;
        ::ReadFile(shandle, &c, 1, &read, NULL);
        if (c == 'q') {
            break;
        }
        if ((::WriteFile(shandle, &c, 1, &read, NULL)) != 1) {
            break;
        }
    }
    ::CloseHandle(shandle);
    _endthreadex(0);
    return 1;
}
void StartEchoServer(const TCHAR *name, const TCHAR* param)
{
    sname = name;
    sparam = param;

    stop = false;
    unsigned threadID;
    thread = (HANDLE)_beginthreadex(NULL, 0, &Echo, NULL, 0, &threadID);
}

static unsigned __stdcall Bark(void* )
{
    if (!CreateCom()) {
        return 1;
    }

    while (!stop) {
        char c = 'a';
        DWORD read = 0;
        if ((::WriteFile(shandle, &c, 1, &read, NULL)) != 1) {
            break;
        }
        Sleep(1000);
    }
    ::CloseHandle(shandle);
    _endthreadex(0);
    return 1;
}
void StartBarkServer(const TCHAR *name, const TCHAR* param)
{
    sname = name;
    sparam = param;

    stop = false;
    unsigned threadID;
    thread = (HANDLE)_beginthreadex(NULL, 0, &Bark, NULL, 0, &threadID);
}

static bool CreateCom()
{
    shandle = ::CreateFile(sname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (shandle == INVALID_HANDLE_VALUE)
        return false;

    DCB dcb = { 0 };
    if (!::GetCommState(shandle, &dcb)) {
        return false;
    }

    if (!::BuildCommDCB(sparam, &dcb)) {
        return false;
    }

    if (!::SetCommState(shandle, &dcb)) {
        return false;
    }
    return true;
}

