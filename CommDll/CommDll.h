#ifndef COMDLL_COMMDLL_H
#define COMDLL_COMMDLL_H

#ifdef COMMDLL_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

class DLLAPI CommUnit
{
public:
	CommUnit();
	~CommUnit();

	BOOL Open(int comNumber);
	BOOL Close();

#ifdef UNIT_TEST
public:
#else
private:
#endif


// endof unit test
private:
	BOOL CloseHandle();
	HANDLE mComHandle;
};
#endif

