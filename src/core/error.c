#include "error.h"

#include "clib/logging.h"

#include <stdarg.h>
#include <stdio.h>

void error_init(Error *err, const char *file, int line, u32 error,
                const char *fmt, ...) {
  err->failure = true;
  err->file = file;
  err->line = line;
  err->error = error;
  va_list va;
  va_start(va, fmt);
  vsnprintf(err->message, ERROR_MESSAGE_MAX, fmt, va);
  va_end(va);
}
void error_dump(Error *err) {
  clib_log_error("%s:%d: %s", err->file, err->line, err->message);
}
