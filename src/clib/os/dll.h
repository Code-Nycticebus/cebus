#ifndef __CLIB_DLL_H__
#define __CLIB_DLL_H__

#include "clib/core/error.h"

typedef void Dll;
typedef void Function;

Dll *dll_load(Str path, Error *error);
void dll_close(Dll *handle);

Function *dll_symbol(Dll *handle, const char *symbol, Error *error);

#endif /*  !__CLIB_DLL_H__ */
