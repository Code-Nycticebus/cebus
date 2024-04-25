#include "error.h"

#include "clib/collection/da.h"
#include "clib/collection/string_builder.h"
#include "clib/core/arena.h"
#include "clib/core/defines.h"
#include "clib/core/error.h"
#include "clib/type/string.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

static void error_dump(ErrorInfo *info) {
  fprintf(stderr, "[Error]: %s:%d\n", info->location.file, info->location.line);
  fprintf(stderr, "  [Message]: " STR_FMT "\n", STR_ARG(info->msg));
  Str message = sb_to_str(&info->message);
  for (Str note = {0}; str_try_chop_by_delim(&message, '\n', &note);) {
    if (!str_eq(note, info->msg)) {
      fprintf(stderr, "  [NOTE]: " STR_FMT "\n", STR_ARG(note));
    }
  }

  // Stack trace
  fprintf(stderr, "[STACK TRACE]\n");
  usize location_count = info->locations.len;
  for (usize i = 0; i < location_count; ++i) {
    ErrorLocation *location = &da_pop(&info->locations);
    fprintf(stderr, "  [%" USIZE_FMT "]: %s:%d\n", i + 1, location->file,
            location->line);
  }
}

////////////////////////////////////////////////////////////////////////////

void _error_emit(Error *err, i32 code, const char *file, int line,
                 const char *fmt, ...) {
  err->failure = true;
  err->info.code = code;

  err->info.message = sb_init(&err->info.arena);
  da_init(&err->info.locations, &err->info.arena);

  da_push(&err->info.locations, (ErrorLocation){file, line});

  va_list va;
  va_start(va, fmt);
  sb_append_va(&err->info.message, fmt, va);
  va_end(va);
  err->info.msg = sb_to_str(&err->info.message);
  sb_append_c(&err->info.message, '\n');

  if (err->panic_on_emit) {
    _error_panic(err);
  }
}

bool _error_occured(Error *err) { return err && err->failure; }

void _error_panic(Error *err) {
  error_dump(&err->info);
  arena_free(&err->info.arena);
  abort();
}

void _error_except(Error *err) {
  arena_free(&err->info.arena);
  err->info = (ErrorInfo){0};
  err->failure = false;
}

void _error_set_code(Error *err, i32 code) { err->info.code = code; }

void _error_set_msg(Error *err, const char *fmt, ...) {
  (void)err;
  va_list va;
  va_start(va, fmt);
  usize size = sb_append_va(&err->info.message, fmt, va);
  va_end(va);

  err->info.msg = str_from_parts(
      size, &err->info.message.items[err->info.message.len - size - 1]);
  sb_append_c(&err->info.message, '\n');
}

void _error_add_note(Error *err, const char *fmt, ...) {
  (void)err;
  va_list va;
  va_start(va, fmt);
  sb_append_va(&err->info.message, fmt, va);
  sb_append_c(&err->info.message, '\n');
  va_end(va);
}

void _error_add_location(Error *err, const char *file, int line) {
  da_push(&err->info.locations, (ErrorLocation){file, line});
}

////////////////////////////////////////////////////////////////////////////
