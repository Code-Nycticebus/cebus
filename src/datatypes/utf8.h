#ifndef __CLIB_UTF8_H__
#define __CLIB_UTF8_H__

#include "clib/arena.h"
#include "datatypes.h" // IWYU pragma: private: include "utf8.h"

#define UTF8(s) utf8_decode(BYTES_STR(s))
#define UTF8_FMT "%.*s"
#define UTF8_ARG(s) (i32)(s).size, (s).data

bool utf8_try_decode(Bytes bytes, Utf8 *out);
Utf8 utf8_decode(Bytes bytes);

bool utf8_try_encode(Utf8 s, Bytes *out);
Bytes utf8_encode(Utf8 s);

bool utf8_eq(Utf8 s1, Utf8 s2);

bool utf8_try_next(Utf8 *str, Utf8 *out);
Utf8 utf8_next(Utf8 *str);

Utf8 utf8_copy(Utf8 str, Arena *arena);

bool utf8_validate(Utf8 s);
bool utf8_validate_bytes(Bytes s);

#endif /* ifndef __CLIB_UTF8_H__ */
