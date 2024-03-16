/* DOCUMENTATION
## Features and Functions

- **Creating Byte Arrays**:
  - `BYTES`: Create a byte array from byte literals.
  - `BYTES_STR`: Create a byte array from a string literal.

- **Creating from Pointer and Size**:
  - `bytes_from_parts(size, data)`: Creates a byte array from given size and
data pointer.

- **Copying Byte Arrays**:
  - `bytes_copy(bytes, arena)`: Performs a deep copy of a byte array, allocating
memory from the specified arena.

- **Slicing and Taking Bytes**:
  - `bytes_slice(bytes, idx1, idx2)`: Returns a new byte array that is a slice
from the original, from `idx1` to `idx2`.
  - `bytes_take(bytes, count)`: Modifies the original byte array to keep only
the first `count` bytes, returning the removed portion.

- **Checking Equality**:
  - `bytes_eq(b1, b2)`: Checks if two byte arrays are equal.

- **Hexadecimal Conversion**:
  - `bytes_to_hex(bytes, arena)`: Converts a byte array into a hexadecimal
string representation, using memory from the arena.
  - `bytes_from_hex(s, arena)`: Creates a byte array from a hexadecimal string,
allocating memory from the arena.

## Usage
Create new Bytes from a literal.
```c
Bytes bytes = BYTES(0xff, 0x11);
Bytes bytes_str = BYTES_STR("Bytes from a string");
```

Or from a pointer with size.
```c
int a = 69;
Bytes bytes = bytes_from_parts(sizeof(a), &a);
```

if you need to take ownership of the bytes you can copy it.
```c
int a = 69;
Bytes bytes = bytes_from_parts(sizeof(a), &a);

Arena arena = {0};
Bytes owned_bytes = bytes_copy(bytes, &arena);
```
*/

#ifndef __CLIB_BYTES_H__
#define __CLIB_BYTES_H__

#include "clib/core/arena.h"
#include "clib/core/defines.h"

////////////////////////////////////////////////////////////////////////////

Bytes bytes_from_parts(usize size, const void *data);

////////////////////////////////////////////////////////////////////////////

Bytes bytes_copy(Bytes bytes, Arena *arena);

////////////////////////////////////////////////////////////////////////////

Bytes bytes_slice(Bytes bytes, usize idx1, size_t idx2);
Bytes bytes_take(Bytes *bytes, usize count);

////////////////////////////////////////////////////////////////////////////

bool bytes_eq(Bytes b1, Bytes b2);

////////////////////////////////////////////////////////////////////////////

Str bytes_to_hex(Bytes bytes, Arena *arena);
Bytes bytes_from_hex(Str s, Arena *arena);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_BYTES_H__ */
