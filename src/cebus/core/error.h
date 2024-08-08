/* DOCUMENTATION
### Initialization Macros
- `ErrNew`: Initializes a new Error instance.
- `ErrPanic`: Initializes an Error that will trigger a panic on `error_emit()`.
- `ErrDefault`: Empty Error that will panic on `error_emit()`.

### Error Emitting and Context
- `error_emit(E, code, fmt, ...)` initializes the passed in error.
```c
void function_that_fails(Error *error) {
  // ...
  if (failure_condition) {
    error_emit(error, error_code, "Error: %s", reason);
    return;
  }
}
```

- `error_context()` creates a context for you to handle the error. Panics if it
falls through
```c
Error error = ErrNew;
function_that_fails(&error);
error_context(&error, {
  error_raise();
});
```

- `error_propagate()` creates a context. Does not panic if it falls through but
also does not reset the error.
:warning: if the error is never handled there will be a memory leak.
```c
Error error = ErrNew;
function_that_fails(&error);
error_propagate(&error, {
  return;
});
```

### Error Handling
- `error_panic()`: Triggers a panic with the current error.
- `error_except()`: Resets the error state.
- `error_msg()`: Retrieves the error message.
- `error_code(T)`: Retrieves the error code and casts it to `T`.
- `error_set_code()`: Sets a new error code.
- `error_set_msg(fmt, ...)`: Sets a new error message and clears all notes.
- `error_add_location()`: Adds current file and line location.
- `error_add_note(fmt, ...)`: Adds a note to the error.

*/

#ifndef __CEBUS_ERROR_H__
#define __CEBUS_ERROR_H__

#include "cebus/collection/da.h"
#include "cebus/collection/string_builder.h"
#include "cebus/core/arena.h"
#include "cebus/core/defines.h"

////////////////////////////////////////////////////////////////////////////

#define ERROR_LOCATION_MAX 10

typedef struct {
  i64 code;
  Str msg;
  StringBuilder message;
  DA(FileLocation) locations;
} ErrorInfo;

typedef struct {
  Arena arena;
  bool failure;
  bool panic_on_emit;
  FileLocation location;
  ErrorInfo *info;
} Error;

#define ErrNew                                                                                     \
  ((Error){                                                                                        \
      .failure = false,                                                                            \
      .panic_on_emit = false,                                                                      \
      .location = FILE_LOCATION_CURRENT,                                                           \
      .arena = {0},                                                                                \
  })

#define ErrPanic                                                                                   \
  ((Error[]){{                                                                                     \
      .failure = false,                                                                            \
      .panic_on_emit = true,                                                                       \
      .location = FILE_LOCATION_CURRENT,                                                           \
      .arena = {0},                                                                                \
  }})

#define ErrDefault ((Error *)NULL)

////////////////////////////////////////////////////////////////////////////

#define error_emit(E, code, ...) _error_internal_emit(E, code, FILE_LOCATION_CURRENT, __VA_ARGS__);

#define error_context(E, ...)                                                                      \
  if (_error_internal_occured(E)) {                                                                \
    Error *__error_context__ = (E);                                                                \
    __VA_ARGS__                                                                                    \
    if ((E)->failure) {                                                                            \
      _error_internal_panic(E);                                                                    \
    }                                                                                              \
  }

#define error_propagate(E, ...)                                                                    \
  if (_error_internal_occured(E)) {                                                                \
    Error *__error_context__ = (E);                                                                \
    error_add_location();                                                                          \
    __VA_ARGS__                                                                                    \
  }

#define error_panic() _error_internal_panic(__error_context__)
#define error_except() _error_internal_except(__error_context__)

#define error_msg() (__error_context__->info->msg)
#define error_code(T) ((T)__error_context__->info->code)

#define error_set_code(code) _error_internal_set_code(__error_context__, (i64)code)
#define error_set_msg(...) _error_internal_set_msg(__error_context__, __VA_ARGS__)

#define error_add_location(...)                                                                    \
  _error_internal_add_location(__error_context__, FILE_LOCATION_CURRENT)
#define error_add_note(...) _error_internal_add_note(__error_context__, __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////

void FMT(4) _error_internal_emit(Error *err, i32 code, FileLocation location, const char *fmt, ...);
bool _error_internal_occured(Error *err);
void NORETURN _error_internal_panic(Error *err);
void _error_internal_except(Error *err);
void _error_internal_set_code(Error *err, i32 code);
void FMT(2) _error_internal_set_msg(Error *err, const char *fmt, ...);
void _error_internal_add_location(Error *err, FileLocation location);
void FMT(2) _error_internal_add_note(Error *err, const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_ERROR_H__ */
