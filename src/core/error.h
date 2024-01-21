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

Pass ```ErrThrow``` or ```ErrNone``` to that function to automatically call
```error_raise()``` if it encouters an error inside the function.
```c
function_that_can_fail(ErrThrow);
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

#define ErrCreate ((Error){.raise = false, .line = __LINE__, .file = __FILE__})
#define ErrThrow                                                               \
  ((Error[]){{.raise = true, .line = __LINE__, .file = __FILE__}})
#define ErrNone ((Error *)NULL)

////////////////////////////////////////////////////////////////////////////

#define Err(E, error, ...)                                                     \
  do {                                                                         \
    _error_init(E ? E : ErrThrow, error, __VA_ARGS__);                         \
  } while (0)

////////////////////////////////////////////////////////////////////////////

void noreturn error_raise(Error *err);
void error_warn(Error *err);
void fmt_args(2) error_add_note(Error *err, const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////

void fmt_args(3) _error_init(Error *err, i32 error, const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_ERROR_H__ */
