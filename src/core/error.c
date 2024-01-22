#include "error.h"

#include "core/asserts.h"
#include "core/logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////

void _error_emit(Error *err, i32 code, const char *file, int line,
                 const char *fmt, ...) {
  err->failure = true;
  err->code = code;
  err->msg_size = 0;
  err->msg_size += (usize)snprintf(&err->message[err->msg_size],
                                   ERROR_MESSAGE_MAX - err->msg_size,
                                   "  [ERROR] %s:%d:\n", file, line);
  va_list va;
  va_start(va, fmt);
  err->msg_size += (usize)vsnprintf(&err->message[err->msg_size],
                                    ERROR_MESSAGE_MAX - err->msg_size, fmt, va);
  va_end(va);
  if (err->raise) {
    _error_panic(err, true);
  }
}

bool _error_occured(Error *err) { return err && err->failure; }

void _error_panic(Error *err, unused bool _err_ctx) {
  clib_log_fatal("%s:%d:\n%s", err->file, err->line, err->message);
  abort();
}

void _error_warn(Error *err, unused bool _err_ctx) {
  clib_log_warning("%s:%d:\n%s", err->file, err->line, err->message);
  _error_ignore(err, _err_ctx);
}

void _error_ignore(Error *err, unused bool _err_ctx) {
  err->msg_size = 0;
  err->failure = false;
}

void _error_set_code(Error *err, unused bool _err_ctx, i32 code) {
  err->code = code;
}

void _error_add_note(Error *err, unused bool _err_ctx, const char *file,
                     int line, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  err->msg_size += (usize)snprintf(&err->message[err->msg_size],
                                   ERROR_MESSAGE_MAX - err->msg_size,
                                   "\n  [NOTE] %s:%d:\n", file, line);
  err->msg_size += (usize)vsnprintf(&err->message[err->msg_size],
                                    ERROR_MESSAGE_MAX - err->msg_size, fmt, va);
  va_end(va);
}

////////////////////////////////////////////////////////////////////////////
