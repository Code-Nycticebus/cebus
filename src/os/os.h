#ifndef __CLIB_OS_H__
#define __CLIB_OS_H__

#include "core/arena.h"
#include "core/defines.h"
#include "core/error.h"

////////////////////////////////////////////////////////////////////////////

Str os_getenv(const char *env, Error *error);
void os_chdir(Str path);
Str os_getcwd(Arena *arena);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_OS_H__ */
