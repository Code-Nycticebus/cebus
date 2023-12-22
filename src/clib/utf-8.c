#include "utf-8.h"
#include <string.h>
#include <wchar.h>

#include "asserts.h"

Utf8 utf8_from_parts(size_t len, const wchar_t *s) {
  return (Utf8){.len = len, .data = s};
}

Utf8 utf8_from_cstr(const char *cstr, Arena *arena) {
  size_t len = strlen(cstr);
  wchar_t *buffer = arena_calloc(arena, (len * sizeof(wchar_t)) + 1);
  for (size_t i = 0; i < len; i++) {
    buffer[i] = cstr[i];
  }
  return utf8_from_parts(len, buffer);
}

Utf8 utf8_decode(size_t size, const char *mbytes, Arena *arena) {
  clib_assert(size % sizeof(wchar_t) == 0, "bytes are not uft-8 decodable!");
  wchar_t *buffer = arena_calloc(arena, size);
  mbrtowc(buffer, mbytes, size, NULL);
  return utf8_from_parts(size / sizeof(wchar_t), (wchar_t *)buffer);
}
