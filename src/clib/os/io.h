/* DOCUMENTATION
### Usage
Use the functions:
```c
Error e = ErrCreate;
io_write(stdout, BYTES_STR("Hello, World"), &e);
```

The input function is just like the one in python:
```c
Str ret = input(STR(":> "));
printf("input: '"STR_FMT"'\n", STR_ARG(ret));
```
Outputs:
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