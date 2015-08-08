#ifndef COMDLL_COMMDLL_H
#define COMDLL_COMMDLL_H

#ifdef COMMDLL_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif
#include <winnt.h>
class DLLAPI CommUnit
{
public:
	CommUnit();
	~CommUnit();

	BOOL Open(int comNumber);
	BOOL Close();


private:
	BOOL CloseHandle();
	HANDLE mComHandle;
};
#endif

