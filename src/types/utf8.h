#ifndef __CLIB_UTF8_H__
#define __CLIB_UTF8_H__

#include "core/arena.h"
#include "core/defines.h" // IWYU pragma: private: include "utf8.h"
#include "core/error.h"

typedef enum {
  UTF8_OK,
  UTF8_DECODE,
  UTF8_ENCODE,
} Utf8Error;

Utf8 utf8_decode(Bytes bytes, Error *error);
Bytes utf8_encode(Utf8 s, Error *error);

bool utf8_eq(Utf8 s1, Utf8 s2);
bool utf8_starts_with(Utf8 s, Utf8 prefix);
bool utf8_ends_with(Utf8 s, Utf8 suffix);

bool utf8_try_next(Utf8 *str, Utf8 *out);
Utf8 utf8_next(Utf8 *str);

Utf8 utf8_copy(Utf8 str, Arena *arena);
Utf8 utf8_append(Utf8 s, Utf8 suffix, Arena *arena);
Utf8 utf8_prepend(Utf8 s, Utf8 prefix, Arena *arena);
Utf8 utf8_join(Utf8 sep, usize count, Utf8 *s, Arena *arena);
Utf8 utf8_upper(Utf8 s, Arena *arena);
Utf8 utf8_lower(Utf8 s, Arena *arena);
// Utf8 utf8_replace(Utf8 s, Utf8 old, Utf8 new, Arena *arena);
// Utf8 utf8_center(Utf8 s, usize width, char fillchar, Arena *arena);
// Utf8 utf8_ljust(Utf8 s, usize width, char fillchar, Arena *arena);
// Utf8 utf8_rjust(Utf8 s, usize width, char fillchar, Arena *arena);
// Utf8 utf8_repeat(Utf8 s, usize count, Arena *arena);
// Utf8 utf8_reverse(Utf8 s, Arena *arena);

bool utf8_validate_bytes(Bytes s);
bool utf8_validate(Utf8 s);

#endif /* !__CLIB_UTF8_H__ */
