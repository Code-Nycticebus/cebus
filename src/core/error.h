#ifndef __CLIB_ERROR_H__
#define __CLIB_ERROR_H__

/* DOCUMENTATION
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
  error_raise(&error);
}
```

Pass ```ErrRaise``` to that function to automatically call
```error_raise()``` if it encouters an error inside the function.
```c
function_that_can_fail(ErrRaise);
```

Pass ```ErrDefault``` to create a new ```Error``` inside the function. Meaning
it will show the file and line number where the error occured.
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
  i32 error;
  usize msg_size;
  char message[ERROR_MESSAGE_MAX];
} Error;

#define ErrNew                                                                 \
  ((Error){                                                                    \
      .raise = false,                                                          \
      .line = __LINE__,                                                        \
      .file = __FILE__,                                                        \
  })
#define ErrRaise                                                               \
  ((Error[]){{                                                                 \
      .raise = true,                                                           \
      .line = __LINE__,                                                        \
      .file = __FILE__,                                                        \
  }})

#define ErrDefault ((Error *)NULL)

////////////////////////////////////////////////////////////////////////////

#define Err(E, error, ...)                                                     \
  do {                                                                         \
    _error_init((E) == ErrDefault ? ErrRaise : (E), error, __FILE__, __LINE__, \
                __VA_ARGS__);                                                  \
  } while (0)

#define ErrFail(E) (E && (E)->failure)
#define ErrNote(E, ...) _error_add_note(E, __FILE__, __LINE__, __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////

void noreturn error_raise(Error *err);
void error_warn(Error *err);

////////////////////////////////////////////////////////////////////////////

void fmt_args(5) _error_init(Error *err, i32 error, const char *file, int line,
                             const char *fmt, ...);
void fmt_args(4) _error_add_note(Error *err, const char *file, int line,
                                 const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_ERROR_H__ */
