# [arena.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/arena.h)
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
# [asserts.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/asserts.h)
## Usage
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
# [defines.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/defines.h)
# [error.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/error.h)
## Usage
If a function can fail it should take an ```Error*``` as parameter. Initialize
the Error with ```error_init()```.
```c
void function_that_can_fail(Error* error)
  int error_code = -1; // Function returns a bad value
  if (error_code < 0) {
    error_init(error, error_code, "error: %d", error_code);
    return;
  }
  // Further processing
  return;
}
```

Create new ```Error``` with ```ErrorCreate``` to except errors that occure
inside the function. Always check with ```error_occured()``` before doing any
calls to the error api.
```c
Error error = ErrCreate;
function_that_can_fail(&error);
if (error_occured(&error)) {
  error_raise(&error);
}
```

Add notes to ```Error``` with ```error_add_note()```
```c
Error error = ErrCreate;
function_that_can_fail(&error);
if (error_occured(&error)) {
  error_add_note(&error, "Note added to error");
  return;
}
```

Pass ```ErrRaise``` to that function to automatically call
```error_raise()``` if it encouters an error inside the function.
```c
function_that_can_fail(ErrRaise);
```

If the function is called with ```ErrDefault``` a new ```ErrorRaise``` gets
created at the first occurence of an error and ```error_raise()``` get called.
```c
function_that_can_fail(ErrDefault);
```
# [logging.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/logging.h)
# [platform.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/platform.h)
Here are various macros for figuring out what Platform and compiler is used.
# [sorting.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/sorting.h)
## Example
```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```
