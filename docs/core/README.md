# [defines.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/defines.h)
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
# [sorting.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/sorting.h)
## Example
```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```
# [error.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/error.h)
## Usage
If a function can fail it should take an ```Error*``` as parameter.
```c
void function_that_can_fail(Error* error)
  int error_code = -1; // Function returns a bad value
  if (error_code < 0) {
    Err(error, error_code, "function_that_can_fail(): error: %d", error_code);
    return;
  }
  // Further processing
  return;
}
```

Create a new ```Error``` to pass it to a function.
```c
Error error = ErrCreate;
function_that_can_fail(&error);
if (error.failure) {
  ErrRaise(&error);
}
```

You can also pass ```ErrThrow``` to that function to automatically call
```ErrRaise``` if it encouters an error inside the function.
```c
function_that_can_fail(ErrThrow);
```
# [platform.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/platform.h)
Here are various macros for figuring out what Platform and compiler is used.
# [arena.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/arena.h)
## Usage
Create a new Arena with:
```c
Arena arena = {0};
```

Now you can allocate from this arena.
```c
int* i = arena_alloc(&arena, sizeof(int));
```

Don't forget to free the arena once you're done.
```c
arena_free(&arena);
```
# [logging.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/logging.h)
