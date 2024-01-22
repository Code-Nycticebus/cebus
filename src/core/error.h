#ifndef __CLIB_ERROR_H__
#define __CLIB_ERROR_H__

/* DOCUMENTATION
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
*/

#include "core/defines.h"

////////////////////////////////////////////////////////////////////////////

#define ERROR_MESSAGE_MAX 1024

typedef struct {
  bool failure;
  bool raise;
  const char *file;
  i32 line;
  i32 code;
  usize msg_size;
  char message[ERROR_MESSAGE_MAX];
} Error;

#define ErrNew                                                                 \
  ((Error){                                                                    \
      .raise = false,                                                          \
      .line = __LINE__,                                                        \
      .file = __FILE__,                                                        \
  })

#define ErrPanic                                                               \
  ((Error[]){{                                                                 \
      .raise = true,                                                           \
      .line = __LINE__,                                                        \
      .file = __FILE__,                                                        \
  }})

#define ErrDefault ((Error *)NULL)

////////////////////////////////////////////////////////////////////////////

#define error_emit(E, code, ...)                                               \
  _error_emit((E) == ErrDefault ? ErrPanic : (E), code, __FILE__, __LINE__,    \
              __VA_ARGS__);

#define error_handle(E, ...)                                                   \
  do {                                                                         \
    if (_error_occured(E)) {                                                   \
      unused bool __error_context_missing = true;                              \
      do {                                                                     \
        __VA_ARGS__                                                            \
      } while (0);                                                             \
    }                                                                          \
  } while (0)

#define error_panic(E) _error_panic(E, __error_context_missing)
#define error_warn(E) _error_warn(E, __error_context_missing)
#define error_ignore(E) _error_ignore(E, __error_context_missing)
#define error_set_code(E, code)                                                \
  _error_set_code(E, __error_context_missing, code)
#define error_add_note(E, ...)                                                 \
  _error_add_note(E, __error_context_missing, __FILE__, __LINE__, __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////

void fmt_args(5) _error_emit(Error *err, i32 code, const char *file, int line,
                             const char *fmt, ...);

bool _error_occured(Error *err);

void _error_panic(Error *err, bool _err_ctx);
void _error_warn(Error *err, bool _err_ctx);
void _error_ignore(Error *err, bool _err_ctx);

void _error_set_code(Error *err, bool _err_ctx, i32 code);
void fmt_args(5) _error_add_note(Error *err, bool _err_ctx, const char *file,
                                 int line, const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////
///
#endif /* !__CLIB_ERROR_H__ */
