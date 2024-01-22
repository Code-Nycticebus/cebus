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

Match your error types with ```error_match()``` inside the context.
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

#define error_context(E, ...)                                                  \
  do {                                                                         \
    if ((E) && (E)->failure) {                                                 \
      Error *__error_context__ = (E);                                          \
      __VA_ARGS__                                                              \
    }                                                                          \
  } while (0)

#define error_panic() _error_panic(__error_context__)
#define error_warn() _error_warn(__error_context__)
#define error_ignore() _error_ignore(__error_context__)

#define error_set_code(code) _error_set_code(__error_context__, code)
#define error_add_note(...)                                                    \
  _error_add_note(__error_context__, __FILE__, __LINE__, __VA_ARGS__)

#define error_match(...)                                                       \
  do {                                                                         \
    switch (__error_context__->code) { __VA_ARGS__ }                           \
  } while (0)

////////////////////////////////////////////////////////////////////////////

void fmt_args(5) _error_emit(Error *err, i32 code, const char *file, int line,
                             const char *fmt, ...);

void _error_panic(Error *err);
void _error_warn(Error *err);
void _error_ignore(Error *err);

void _error_set_code(Error *err, i32 code);
void fmt_args(4) _error_add_note(Error *err, const char *file, int line,
                                 const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////
///
#endif /* !__CLIB_ERROR_H__ */
