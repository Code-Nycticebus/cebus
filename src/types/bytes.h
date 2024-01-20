#ifndef __CLIB_BYTES_H__
#define __CLIB_BYTES_H__

#include "core/arena.h"
#include "core/defines.h"

/*
 * Creates new Bytes from 'u8' array and its size.
 */
Bytes bytes_from_parts(usize size, const u8 *data);

/*
 * Returns a new exact copy of Bytes.
 */
Bytes bytes_copy(Bytes bytes, Arena *arena);

/*
 * Returns a slice of the bytes.
 */
Bytes bytes_slice(Bytes bytes, usize idx1, size_t idx2);

/*
 * Returns the 'count' amount of bytes taken from 'bytes'.
 */
Bytes bytes_take(Bytes *bytes, usize count);

/*
 * Returns true if 'b1' and 'b2' are equal.
 */
bool bytes_eq(Bytes b1, Bytes b2);

/*
 * Returns new Str of bytes hex representation.
 */
Str bytes_to_hex(Bytes bytes, Arena *arena);

/*
 * Returns new bytes of a Str hex representation.
 */
Bytes bytes_from_hex(Str s, Arena *arena);

#endif /* !__CLIB_BYTES_H__ */
