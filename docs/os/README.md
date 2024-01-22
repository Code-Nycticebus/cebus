# [fs.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/fs.h)
## Usage
To read in the entire file as Str
```c
Arena arena = {0};
Error error = ErrCreate;
Str content = file_read_str(STR("filename.txt"), &arena, &error);
if (error_occured(&error)) {
  error_raise(&error);
}
arena_free(&arena);
```
# [io.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/io.h)
## Usage
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
# [os.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/os.h)
