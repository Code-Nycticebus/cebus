#ifndef __CLIB_BYTES_H__
#define __CLIB_BYTES_H__

#include "clib/arena.h"
#include "clib/str.h"
#include "defines.h"

typedef struct {
  size_t size;
  const uint8_t *data;
} Bytes;

#define BYTES(...)                                                             \
  (Bytes) {                                                                    \
    sizeof((uint8_t[]){__VA_ARGS__}), (uint8_t[]) { __VA_ARGS__ }              \
  }

#define BYTES_STR(s)                                                           \
  (Bytes) {                                                                    \
    sizeof(s) - 1, (uint8_t[]) { (s) }                                         \
  }

Bytes bytes_from_parts(size_t size, const uint8_t *data);
Bytes bytes_copy(Bytes bytes, Arena *arena);

bool bytes_eq(Bytes b1, Bytes b2);

Str bytes_hex(Bytes bytes, Arena *arena);

#endif /* !__CLIB_BYTES_H__ */
