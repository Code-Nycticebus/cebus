#ifndef __CLIB_ERROR_H__
#define __CLIB_ERROR_H__

/* DOCUMENTATION
#### Usage
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
*/

#include "core/defines.h"

////////////////////////////////////////////////////////////////////////////

#define ERROR_BUFFER_MAX 512
#define ERROR_LOCATIONS_MAX 10
#define FILE_LOC __FILE__, __LINE__

typedef struct {
  i32 code;
  const char *file;
  i32 line;
  Str msg;
  usize msg_len;
  char msg_buffer[ERROR_BUFFER_MAX];
  usize location_count;
  usize location_idx;
  struct {
    const char *file;
    int line;
  } locations[ERROR_LOCATIONS_MAX];
} ErrorInfo;

typedef struct {
  bool failure;
  bool panic_instantly;
  ErrorInfo info;
} Error;

#define ErrNew                                                                 \
  ((Error){                                                                    \
      .panic_instantly = false,                                                \
      .info.line = __LINE__,                                                   \
      .info.file = __FILE__,                                                   \
  })

#define ErrPanic                                                               \
  ((Error[]){{                                                                 \
      .panic_instantly = true,                                                 \
      .info.line = __LINE__,                                                   \
      .info.file = __FILE__,                                                   \
  }})

#define ErrDefault ((Error *)NULL)

////////////////////////////////////////////////////////////////////////////

#define error_emit(E, code, ...)                                               \
  _error_emit((E) == ErrDefault ? ErrPanic : (E), code, FILE_LOC, __VA_ARGS__);

#define error_context(E, ...)                                                  \
  do {                                                                         \
    if ((E) && (E)->failure) {                                                 \
      Error *__error_context__ = (E);                                          \
      __VA_ARGS__                                                              \
    }                                                                          \
  } while (0)

#define error_panic() _error_panic(__error_context__)
#define error_except() _error_except(__error_context__)

#define error_msg() (__error_context__->info.msg)

#define error_set_code(code) _error_set_code(__error_context__, code)
#define error_set_msg(...) _error_set_msg(__error_context__, __VA_ARGS__)

#define error_add_location(...) _error_add_location(__error_context__, FILE_LOC)
#define error_add_note(...) _error_add_note(__error_context__, __VA_ARGS__)

#define error_match(...)                                                       \
  do {                                                                         \
    switch (__error_context__->info.code) { __VA_ARGS__ }                      \
  } while (0)

////////////////////////////////////////////////////////////////////////////

void fmt_args(5) _error_emit(Error *err, i32 code, const char *file, int line,
                             const char *fmt, ...);

void _error_panic(Error *err);
void _error_except(Error *err);

void _error_set_code(Error *err, i32 code);
void fmt_args(2) _error_set_msg(Error *err, const char *fmt, ...);
void _error_add_location(Error *err, const char *file, int line);
void fmt_args(2) _error_add_note(Error *err, const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////
///
#endif /* !__CLIB_ERROR_H__ */
