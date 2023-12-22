#ifndef __CLIB_UTF8_H__
#define __CLIB_UTF8_H__

#include "clib/arena.h"
#include "datatypes.h" // IWYU pragma: private: include "utf8.h"

// TODO utf8_validate()

#define UTF8(s) utf8_decode(BYTES_STR(s));
#define UTF8_FMT "%.*s"
#define UTF8_ARG(s) (int)(s).size, (s).data

size_t utf8_cstrlen(const char *cstr);
Utf8 utf8_decode(Bytes bytes);

Utf8 utf8_next(Utf8 *str);

Utf8 utf8_copy(Utf8 str, Arena *arena);

#endif /* ifndef __CLIB_UTF8_H__ */
