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
# [char.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/char.h)

Just a wrapper around ```#include <ctype.h>```

```c_is_alnum()```\
checks for an alphanumeric character; it is equivalent to
```c_is_alpha(c) || c_is_digit(c)```.

```c_is_alpha()```\
checks for an alphabetic character; in the standard "C" locale, it is
equivalent to ```c_is_upper(c) || c_is_lower(c)```. In some locales, there may
be additional characters for which ```c_is_alpha()``` is true-letters which are
neither upper case nor lower case.

```c_is_blank()```\
checks for a blank character; that is, a space or a tab.

```c_is_cntrl()```\
checks for a control character.

```c_is_digit()```\
checks for a digit (0 through 9).

```c_is_graph()```\
checks for any printable character except space.

```c_is_lower()```\
checks for a lower-case character.

```c_is_print()```\
checks for any printable character including space.

```c_is_punct()```\
checks for any printable character which is not a space or an alphanumeric
character.
```c_is_space()```\
checks for white-space characters. In the "C" and "POSIX"
locales, these are: space, form-feed ```\f```, newline ```\n```, carriage return
```\r```, horizontal tab ```\t```, and vertical tab ```\v```.

```c_is_upper()```\
checks for an uppercase letter.

```c_is_xdigit()```\
checks for a hexadecimal digits, that is, one of 0 1 2 3 4 5 6 7 8 9 a b c d e f
A B C D E F.

