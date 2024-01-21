# [utf8.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/utf8.h)
# [bytes.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/bytes.h)
## Usage
Create new Bytes with:
```c
Bytes bytes = BYTES(0xff, 0x11);
Bytes bytes_str = BYTES_STR("Bytes from a string");
```
# [str.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/str.h)
## Usage
Create a new Str with:
```c
Str str = STR("Hello World");
```

You can print the strings using the STR_FMT and STR_ARG() macro:
```c
printf(STR_FMT"\n", STR_ARG(str));
```

I always treat strings as immutable.
So you always have to provide an Arena to do allocations in.
```c
Arena arena = {0};
Str new_str = str_lower(str, &arena);
arena_free(&arena);
```


Iterating over a string is easy
```c
Str content = STR("This is a line")
for (Str word = {0}; str_try_chop_by_delim(&content, ' ', &word)) {
  printf(STR_FMT"\n", STR_ARG(word));
}
```
Outputs:
```console
This
is
a
line
```
# [floats.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/floats.h)
# [integers.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/integers.h)
## Usage
Just use the functions.
