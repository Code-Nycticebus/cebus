/* DOCUMENTATION
## Functions

- **Output**:
  - `io_write(file, fmt, ...)`: Writes a formated string into the file
  - `io_write_bytes(file, bytes, error)`: Writes byte data to a file or stream.

- **Input**:
  - `io_read_bytes(file, size, buffer, error)`: Reads a specified amount of byte
data from a file or stream into a buffer.
  - `io_read_line(file, size, buffer, error)`: Reads a line of text from a file
or stream into a buffer.
  - `input(prefix)`: Displays a prompt and reads a line of text from standard
input.

## Usage Example

```c
Error e = ErrNew;
io_write(stdout, BYTES_STR("Hello, World"), &e);
error_context(&e, { error_raise(); });

Str ret = input(STR(":> "));
printf("input: '"STR_FMT"'\n", STR_ARG(ret));
```

Output when running the example:
```console
:> name
input: 'name'
```

*/

#ifndef __CLIB_IO_H__
#define __CLIB_IO_H__

#include "clib/core/defines.h"
#include "clib/core/error.h"

////////////////////////////////////////////////////////////////////////////

FMT(2) usize io_write(FILE *file, const char *fmt, ...);
void io_write_bytes(FILE *file, Bytes bytes, Error *error);

Bytes io_read_bytes(FILE *file, usize size, void *buffer, Error *error);
Str io_read_line(FILE *file, usize size, char *buffer, Error *error);

////////////////////////////////////////////////////////////////////////////

Str input(Str prefix);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_IO_H__ */
