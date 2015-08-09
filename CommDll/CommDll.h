#ifndef COMDLL_COMMDLL_H
#define COMDLL_COMMDLL_H

#ifdef COMMDLL_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#include "CommUnit.h"
#include "AsyncIO.h"

#endif

