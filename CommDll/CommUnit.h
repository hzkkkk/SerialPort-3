#ifndef COMDLL_COMMUNIT_H
#define COMDLL_COMMUNIT_H

#ifdef COMMDLL_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllinport)

#endif
class DLLAPI CommUnit
{
public:
	CommUnit();
	~CommUnit();

	int add(int a, int b);
	int sub(int a, int b);

};
#endif

