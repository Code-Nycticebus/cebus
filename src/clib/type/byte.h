/* DOCUMENTATION
### Usage
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
