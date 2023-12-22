#ifndef __CLIB_BYTES_H__
#define __CLIB_BYTES_H__

#include "clib/arena.h"
#include "clib/str.h"
#include "defines.h"

typedef struct {
  size_t size;
  uint8_t *data;
} Bytes;

#define BYTES(...)                                                             \
  (Bytes) {                                                                    \
    .size = sizeof(b), .data = (uint8_t[]) { __VA_ARGS__ }                     \
  }

Bytes bytes_from_hex(Str hex, Arena *arena);

#endif /* !__CLIB_BYTES_H__ */
