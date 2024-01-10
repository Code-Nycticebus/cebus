#ifndef __CLIB_ERROR_H__
#define __CLIB_ERROR_H__

#include "core/defines.h"
#include "core/platform.h"

#include <stdlib.h> // IWYU pragma: export

#define ERROR_MESSAGE_MAX 512

typedef struct {
  bool failure;
  const char *file;
  i32 line;
  i32 error;
  char message[ERROR_MESSAGE_MAX];
} Error;

#define Err(E, error, ...)                                                     \
  do {                                                                         \
    if (E) {                                                                   \
      error_init(E, __FILE__, __LINE__, error, __VA_ARGS__);                   \
    } else {                                                                   \
      Error __e = {0};                                                         \
      error_init(&__e, __FILE__, __LINE__, error, __VA_ARGS__);                \
      ErrRaise(&__e);                                                          \
    }                                                                          \
  } while (0)

#define ErrRaise(E)                                                            \
  do {                                                                         \
    error_dump(E);                                                             \
    abort();                                                                   \
  } while (0)

void error_init(Error *err, const char *file, int line, i32 error,
                const char *fmt, ...) CLIB_FMT(5, 6);
void error_dump(Error *err);

#endif // !__CLIB_ERROR_H__
