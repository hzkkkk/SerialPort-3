#ifndef _COMMDLL_SERIAL_IO_H_
#define _COMMDLL_SERIAL_IO_H_

#ifdef COMMDLL_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

class DLLAPI SerialIO
{
public:
	SerialIO();
	virtual ~SerialIO();

	bool Open(const TCHAR*, const TCHAR*);
	bool Close();

	typedef enum code {
		SUCCESS,
		TIME_OUT,
		FAIL,
	} Result;

	Result ReadChunk(char**, int*, DWORD);
	Result Read(char*, DWORD, DWORD*, DWORD);
	Result Write(const char*, DWORD, DWORD*, DWORD);

private:
    SerialIO(const SerialIO&); 
	void operator=(const SerialIO&);
	bool IsInitialized();
	bool CloseResource();

	HANDLE handle_;
	OVERLAPPED readov_;
	OVERLAPPED writeov_;
};
#endif

