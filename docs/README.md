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
   - [hm.h](#hmh)
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
## [hm.h](https://github.com/Code-Nycticebus/clib/blob/main/src/collection/hm.h)
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
### Usage
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

Work inside of an error context with ```error_context()```. It will call
```error_panic()``` if it falls through.
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_context(&error, {
  // Do error handling
});
```

Or propagate the error with ```error_propagate()```
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_propagate(&error, {
  // Do error handling
});
```

Add notes to ```Error``` with ```error_add_note()``` or set a different error
code with ```error_set_code()```
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_propagate(&error, {
  error_add_note("Note added to error");
  error_set_code(69);
});
```

On error: ```error_panic()``` prints message and aborts.
```error_except()``` excepts and resets the error.
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_context(&error, {
  error_panic();
  error_except();
});
```

Pass ```ErrPanic``` to that function to automatically call
```error_panic()``` if it encouters an error inside the function.
```c
function_that_can_fail(ErrPanic);
```

```ErrDefault``` is the same as ```ErrPanic``` but the ```Error``` gets
created at the first occurence of an error. Only use this if you really don't
care about the error.
```c
function_that_can_fail(ErrDefault);
```

Match your error types with ```error_code(T)``` inside an ```error_context()```
and get the message with ```error_msg()```.
```c
Error error = ErrNew;
function_that_can_fail(true, &error);
error_context(&error, {
  if (error_code(i32) == 420) {
    printf("%s\n", error_msg());
    error_except();
  }
});
```
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
