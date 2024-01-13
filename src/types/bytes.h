#ifndef __CLIB_BYTES_H__
#define __CLIB_BYTES_H__

#include "core/arena.h"
#include "core/defines.h"

Bytes bytes_from_parts(usize size, const u8 *data);
Bytes bytes_copy(Bytes bytes, Arena *arena);

Bytes bytes_slice(Bytes bytes, usize idx1, size_t idx2);
Bytes bytes_take(Bytes *bytes, usize count);

bool bytes_eq(Bytes b1, Bytes b2);

Str bytes_hex(Bytes bytes, Arena *arena);
Bytes bytes_from_hex(Str s, Arena *arena);

#endif /* !__CLIB_BYTES_H__ */
