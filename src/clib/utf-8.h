#ifndef __CLIB_UTF8_H__
#define __CLIB_UTF8_H__

#include "clib/arena.h"
#include "clib/bytes.h"
#include <wchar.h>

typedef struct {
  size_t len;
  const wchar_t *data;
} Utf8;

#define UTF8(str)                                                              \
  (Utf8) { .len = (sizeof(L##str) / sizeof(wchar_t)) - 1, .data = L##str }
#define UTF8_FMT "%.*ls"
#define UTF8_ARG(s) (int)((s).len * sizeof(wchar_t)), (s).data

Utf8 utf8_from_parts(size_t len, const wchar_t *s);
Utf8 utf8_from_cstr(const char *cstr, Arena *arena);

Utf8 utf8_decode(Bytes bytes, Arena *arena);

#endif /* ifndef __CLIB_UTF8_H__ */
