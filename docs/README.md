# Types
## [bytes.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/bytes.h)
## Usage
Create new Bytes with:
```c
Bytes bytes = BYTES(0xff, 0x11);
Bytes bytes_str = BYTES_STR("Bytes from a string");
```
## [str.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/str.h)
## Usage
Create a new Str with:
```c
Str str = STR("Hello World");
```

You can print the strings using the ```STR_FMT``` and ```STR_ARG()``` macro:
```c
printf(STR_FMT"\n", STR_ARG(str));
```

I always treat strings as immutable.
So you always have to provide an Arena on all manipulation functions.
```c
Arena arena = {0};

Str lower = str_lower(str, &arena);
Str upper = str_upper(str, &arena);

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
## [integers.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/integers.h)
## Usage
Just use the functions.
## [char.h](https://github.com/Code-Nycticebus/clib/blob/main/src/types/char.h)

Just a wrapper around ```#include <ctype.h>```

# Os
## [io.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/io.h)
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
## [fs.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/fs.h)
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
# Collections
## [vec.h](https://github.com/Code-Nycticebus/clib/blob/main/src/collections/vec.h)
## Usage:
Create a new Vec with:
```c
Arena arena = {0};
VEC(int) vec = {0};
vec_init(&vec, &arena);
```

Then you can push elements to the vector.
```c
vec_push(&vec, 69);
vec_push(&vec, 420);
```
## [hm.h](https://github.com/Code-Nycticebus/clib/blob/main/src/collections/hm.h)
## Usage
Create a new HashMap with:
```c
Arena arena = {0};
HashMap hm = hm_create(&arena);
hm_reserve(&hm, 10); // optional
```

Then you can add elements by hash to the HashMap.
```c
hm_insert(&set, str_hash(STR("Hello")), HashValue(i64, 69));
hm_insert(&set, str_hash(STR("World")), HashValue(i64, 420));
```

Now you can get the values by passing in the hash of the element.
```c
hm_get(&set, str_hash(STR("Hello")))->as.i64;
hm_get(&set, str_hash(STR("World")))->as.i64;
```
## [set.h](https://github.com/Code-Nycticebus/clib/blob/main/src/collections/set.h)
## Usage
Create a new Set with:
```c
Arena arena = {0};
Set set = set_create(&arena);
```

Then you can add elements by hash to the Set.
```c
set_add(&set, str_hash(STR("Hello")));
set_add(&set, str_hash(STR("World")));
```

Then you can test if an element is in the Set.
```c
set_contains(&set, str_hash(STR("Hello"))) == true;
set_contains(&set, str_hash(STR("World"))) == true;
```
# Core
## [asserts.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/asserts.h)
### Usage
You can assert if something is true with:
```c
clib_assert(1 == 1, "One should be one");
clib_assert(2 == 2, "It can even handle arguments: %d", 420);
```

Here are all the available macros
```c
clib_assert(EXPR, FMT, ...);
clib_assert_warn(EXPR, FMT, ...);
clib_assert_debug(EXPR, FMT, ...);
clib_assert_return(EXPR, RETURN_VALUE);
```
## [sorting.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/sorting.h)
## Example
```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```
## [error.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/error.h)
## Usage
If a function can fail it should take an ```Error*``` as parameter. If an error
occurs, initialize the Error with ```error_emit()```.
```c
void function_that_can_fail(Error* error)
  int error_code = -1; // Function returns a bad value
  if (error_code < 0) {
    error_emit(error, error_code, "error: %d", error_code);
  }
}
```

Create new ```Error``` with ```ErrNew``` to except errors that occure
inside the function.
```c
Error error = ErrNew;
function_that_can_fail(&error);
```

Work inside of an error context with ```error_context()```
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_context(&error, {
  // Do error handling
});
```

Add notes to ```Error``` with ```error_add_note()``` or set a different error
code with ```error_set_code()```
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_context(&error, {
  error_add_note("Note added to error");
  error_set_code(69);
});
```

```error_panic()``` panics and aborts if it
encounters an error, ```error_warn()``` just prints a warning message and
```error_ignore()``` ignores error completely.
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_context(&error, {
  error_warn();
  error_ignore();
  error_panic();
});
```

Pass ```ErrPanic``` to that function to automatically call
```error_panic()``` if it encouters an error inside the function.
```c
function_that_can_fail(ErrPanic);
```

```ErrDefault``` is the same as ```ErrorPanic``` but the ```Error``` gets
created at the first occurence of an error.
```c
function_that_can_fail(ErrDefault);
```

Match your error types with ```error_match()```.
```c
  Error error = ErrNew;
  function_that_can_fail(true, &err2);
  error_context(&error, {
    error_match({
      case 69:
        error_panic();
      case 420: {
        error_ignore();
      } break;
    });
  });
```
## [platform.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/platform.h)
Here are various macros for figuring out what Platform and compiler is used.
## [arena.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/arena.h)
## Usage
Create a new Arena with:
```c
Arena arena = {0};
```

Now you can allocate from this arena.
```c
int* i1 = arena_alloc(&arena, sizeof(int));
int* i2 = arena_alloc(&arena, sizeof(int));
int* i3 = arena_alloc(&arena, sizeof(int));
```

Don't forget to free the arena once you're done. This frees all allocated
integers at once.
```c
arena_free(&arena);
```
