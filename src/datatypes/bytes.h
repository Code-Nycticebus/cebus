#ifndef __CLIB_BYTES_H__
#define __CLIB_BYTES_H__

#include "clib/arena.h"
#include "clib/defines.h"
#include "datatypes.h" // IWYU pragma: private: include "bytes.h"

#define BYTES(...)                                                             \
  (Bytes) {                                                                    \
    sizeof((u8[]){__VA_ARGS__}), (u8[]) { __VA_ARGS__ }                        \
  }

#define BYTES_STR(s)                                                           \
  (Bytes) {                                                                    \
    sizeof(s) - 1, (void *) { (s) }                                            \
  }

Bytes bytes_from_parts(usize size, const u8 *data);
Bytes bytes_copy(Bytes bytes, Arena *arena);

Bytes bytes_slice(Bytes bytes, usize idx1, size_t idx2);

bool bytes_eq(Bytes b1, Bytes b2);

Str bytes_hex(Bytes bytes, Arena *arena);

#endif /* !__CLIB_BYTES_H__ */
