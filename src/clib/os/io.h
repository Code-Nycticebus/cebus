/* DOCUMENTATION
## Functions

- **Output**:
  - `io_write(file, bytes, error)`: Writes byte data to a file or stream.

- **Input**:
  - `io_read(file, size, buffer, error)`: Reads a specified amount of byte data from a file or stream into a buffer.
  - `io_read_line(file, size, buffer, error)`: Reads a line of text from a file or stream into a buffer.
  - `input(prefix)`: Displays a prompt and reads a line of text from standard input.

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

void io_write(FILE *file, Bytes bytes, Error *error);
Bytes io_read(FILE *file, usize size, void *buffer, Error *error);
Str io_read_line(FILE *file, usize size, char *buffer, Error *error);

////////////////////////////////////////////////////////////////////////////

Str input(Str prefix);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_IO_H__ */
