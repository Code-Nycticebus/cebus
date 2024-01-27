# Clib
 
This is an extension of the C standart library. 

### Quickstart

#### [pybuildc](https://github.com/Code-Nycticebus/pybuildc)
```console
pybuildc build
```

#### Gcc or Clang
Build the static library.
```console
gcc -I./src -c ./src/**/*.c && ar rcs libclib.a *.o && rm *.o
```
Set the ```CLIB_DIRECTORY``` variable and add ```-I"$CLIB DIRECTORY/src" -L"$CLIB_DIRECTORY" -lclib``` to your compiler flags.
```console
gcc -o info examples/info.c -I"$CLIB_DIRECTORY/src" -L"$CLIB_DIRECTORY" -lclib
```

## Documentation
<!-- DOCUMENTATION -->
- [Collection](#Collection)
   - [da.h](#dah)
   - [hashmap.h](#hashmaph)
   - [set.h](#seth)
- [Core](#Core)
   - [arena.h](#arenah)
   - [assert.h](#asserth)
   - [defines.h](#definesh)
   - [error.h](#errorh)
   - [logging.h](#loggingh)
   - [platform.h](#platformh)
   - [sorting.h](#sortingh)
- [Os](#Os)
   - [fs.h](#fsh)
   - [io.h](#ioh)
   - [os.h](#osh)
- [Type](#Type)
   - [byte.h](#byteh)
   - [char.h](#charh)
   - [float.h](#floath)
   - [integer.h](#integerh)
   - [string.h](#stringh)
   - [utf8.h](#utf8h)
## Collection
## [da.h](https://github.com/Code-Nycticebus/clib/blob/main/src/collection/da.h)
### Usage
Create a new dynamic array with:
```c
Arena arena = {0};
DA(int) vec = {0};
da_init(&vec, &arena);
```

Then you can push elements to the dynamic array.
```c
da_push(&vec, 69);
da_push(&vec, 420);
```

Then you can push elements to the dynamic array.
```c
da_push(&vec, 69);
da_push(&vec, 420);
```
## [hashmap.h](https://github.com/Code-Nycticebus/clib/blob/main/src/collection/hashmap.h)
### Overview
My HashMap does not safe anything other that the hash of the key and a numeric
value. For use cases see test examples.
### Usage
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
## [set.h](https://github.com/Code-Nycticebus/clib/blob/main/src/collection/set.h)
### Usage
Create a new Set with:
```c
Arena arena = {0};
Set set = set_create(&arena);
```

Then you can add elements by hash to the Set. ```set_add()``` returns false if
the Set already contains element.
```c
set_add(&set, str_hash(STR("Hello")));
set_add(&set, str_hash(STR("World")));
```

Test if an element is in the Set.
```c
set_contains(&set, str_hash(STR("Hello")));
set_contains(&set, str_hash(STR("World")));
```
## Core
## [arena.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/arena.h)
### Usage
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
## [assert.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/assert.h)
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
## [error.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/error.h)
### Initialization Macros
- `ErrNew`: Initializes a new Error instance.
- `ErrPanic`: Initializes an Error that will trigger a panic on `error_emit()`.
- `ErrDefault`: Empty Error that will panic on `error_emit()`.

### Error Emitting and Context
- `error_emit()` initializes the passed in error.
```c
void function_that_fails(Error *error) {
  // ...
  if (failure_condition) {
    error_emit(error, error_code, "Error: %s", reason);
  }
}
```

- `error_context()` creates a context for you to handle the error. Panics if it
falls through
```c
Error error = ErrNew;
function_that_fails(&error);
error_context(&error, {
  // Error Handling
});
```

- `error_propagate()` creates a context. Does not panic if it falls through but
also does not reset the error.
```c
Error error = ErrNew;
function_that_fails(&error);
error_propagate(&error, {
  // Error Handling
});
```

### Error Handling
- `error_panic()`: Triggers a panic with the current error.
- `error_except()`: Resets the error state.
- `error_msg()`: Retrieves the error message.
- `error_code(T)`: Retrieves the error code and casts it to `T`.
- `error_set_code()`: Sets a new error code.
- `error_set_msg()`: Sets a new error message.
- `error_add_location()`: Adds current file and line location.
- `error_add_note()`: Adds a note to the error.

## [platform.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/platform.h)
Here are various macros for figuring out what Platform and compiler is used.
## [sorting.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/sorting.h)
### Usage
```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```
## Os
## [fs.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/fs.h)
### Usage
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
## [io.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/io.h)
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
## Type
## [byte.h](https://github.com/Code-Nycticebus/clib/blob/main/src/type/byte.h)
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
## [string.h](https://github.com/Code-Nycticebus/clib/blob/main/src/type/string.h)
### Usage
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
