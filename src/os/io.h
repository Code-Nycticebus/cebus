#ifndef __CLIB_IO_H__
#define __CLIB_IO_H__

#include "core/defines.h"

void print(const char *fmt, ...) CLIB_FMT(1, 2);
Str input(Str prefix);

#endif // !__CLIB_IO_H__
