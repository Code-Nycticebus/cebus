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

Create new ```Error``` with ```ErrorCreate``` to except errors that occure
inside the function.
```c
Error error = ErrCreate;
function_that_can_fail(&error);
```

Always check with ```error_handle()``` before doing any
calls to the error api. Or else the ```__error_context_missing``` identifier is
not specified.
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_handle(&error, {
  // Do error handling
});
```

Add notes to ```Error``` with ```error_add_note()``` or set a different error
code with ```error_set_code()```
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_handle(&error, {
  error_add_note(&error, "Note added to error");
  error_set_code(&error, 69);
});
```

Call what you want to do with these errors. ```error_panic()``` panics if it
encounters an error, ```error_warn()``` just prints a warning message and
```error_ignore()``` ignores error completely.
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_handle(&error, {
  error_warn(&error);
  error_ignore(&error);
  error_panic(&error);
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
# [logging.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/logging.h)
# [platform.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/platform.h)
Here are various macros for figuring out what Platform and compiler is used.
# [sorting.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/sorting.h)
## Example
```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```
