#ifndef INLINE_WRAPWIN32API_H_
#define INLINE_WRAPWIN32API_H_

bool CreateFileB(LPCTSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE, HANDLE*);
bool CreateEventB(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCTSTR, HANDLE*);
bool WaitForSingleObjectB(HANDLE, DWORD, DWORD*);
bool FormatMessageB(DWORD, LPCVOID, DWORD, DWORD, LPTSTR, DWORD, DWORD*, va_list *);
bool TryWin32(bool, const  char *, int);
bool TryWin32AsyncIO(bool, const char *, int);
bool TryClearCommError(HANDLE, LPDWORD, LPCOMSTAT, const char *, int);

#endif
