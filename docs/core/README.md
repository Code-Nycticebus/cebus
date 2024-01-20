# [defines.h](src/core/defines.h)
# [asserts.h](src/core/asserts.h)
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
# [sorting.h](src/core/sorting.h)
# [error.h](src/core/error.h)
## Usage
If you have a function that can fail it should take an Error as parameter.
It then sets the error at Error*.
```c
void function_that_can_fail(Error* error)
  int ret = -1; // Function returns a bad value
  if (ret == -1) {
    Err(error, errno, "function_that_can_fail(): encountered error: %d", errno);
    return;
  }
  // Further processing
  return;
```

Create a new Error with:
```c
Error error = ErrCreate;
```

Now you can pass the error to a function that takes an Error.
```c
function_that_can_fail(&error);
if (error.failure) {
  // error occured
}
```

You can also pass a NULL pointer to that
function.It will abort if it encouters an error.
```c
function_that_can_fail(NULL); // abort() if it encouters an error
```
# [platform.h](src/core/platform.h)
# [arena.h](src/core/arena.h)
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
# [logging.h](src/core/logging.h)
