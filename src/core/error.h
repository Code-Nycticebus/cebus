#ifndef __CLIB_ERROR_H__
#define __CLIB_ERROR_H__

/* DOCUMENTATION
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
#define ErrRaise                                                               \
  ((Error[]){{                                                                 \
      .raise = true,                                                           \
      .line = __LINE__,                                                        \
      .file = __FILE__,                                                        \
  }})

#define ErrDefault ((Error *)NULL)

////////////////////////////////////////////////////////////////////////////

#define error_init(E, code, ...)                                               \
  _error_init((E) == ErrDefault ? ErrRaise : (E), code, __FILE__, __LINE__,    \
              __VA_ARGS__);

bool error_occured(Error *err);

////////////////////////////////////////////////////////////////////////////

void no_return error_raise(Error *err);
void error_warn(Error *err);
#define error_add_note(E, ...)                                                 \
  _error_add_note(E, __FILE__, __LINE__, __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////

void fmt_args(5) _error_init(Error *err, i32 code, const char *file, int line,
                             const char *fmt, ...);
void fmt_args(4) _error_add_note(Error *err, const char *file, int line,
                                 const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////
///
#endif /* !__CLIB_ERROR_H__ */
