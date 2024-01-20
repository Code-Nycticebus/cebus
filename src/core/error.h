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
  ErrRaise(&error);
}
```

You can also pass ```ErrThrow``` to that function to automatically call
```ErrRaise``` if it encouters an error inside the function.
```c
function_that_can_fail(ErrThrow);
```
*/

#include "core/defines.h"
#include "core/platform.h"

#include <stdlib.h> // IWYU pragma: export

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

////////////////////////////////////////////////////////////////////////////

#define Err(E, error, ...)                                                     \
  do {                                                                         \
    _error_init(E, error, __VA_ARGS__);                                        \
    if ((E)->raise) {                                                          \
      ErrRaise(E);                                                             \
    }                                                                          \
  } while (0)

#define ErrRaise(E)                                                            \
  do {                                                                         \
    _error_dump(E);                                                            \
    abort();                                                                   \
  } while (0)

////////////////////////////////////////////////////////////////////////////

void error_add_note(Error *err, const char *fmt, ...) CLIB_FMT(2, 3);

////////////////////////////////////////////////////////////////////////////

void _error_init(Error *err, i32 error, const char *fmt, ...) CLIB_FMT(3, 4);
void _error_dump(Error *err);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_ERROR_H__ */
