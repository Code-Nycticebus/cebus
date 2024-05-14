#include "error.h"

#include "cebus/core/arena.h"
#include "cebus/core/error.h"
#include "cebus/type/string.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////

static void error_dump(Error *error) {

  fprintf(stderr, "[Error]: %s:%d: %s()\n", error->location.file,
          error->location.line, error->location.function);
  fprintf(stderr, "  [Message]: " STR_FMT "\n", STR_ARG(error->info->msg));
  Str message = sb_to_str(&error->info->message);
  for (Str note = {0}; str_try_chop_by_delim(&message, '\n', &note);) {
    if (!str_eq(note, error->info->msg)) {
      fprintf(stderr, "  [NOTE]: " STR_FMT "\n", STR_ARG(note));
    }
  }

  // Stack trace
  fprintf(stderr, "[STACK TRACE]\n");
  usize location_count = error->info->locations.len;
  for (usize i = 0; i < location_count; ++i) {
    FileLocation *location = &da_pop(&error->info->locations);
    fprintf(stderr, "  [%" USIZE_FMT "]: %s:%d: %s()\n", i + 1, location->file,
            location->line, location->function);
  }
}

////////////////////////////////////////////////////////////////////////////

void _error_internal_emit(Error *err, i32 code, FileLocation location,
                          const char *fmt, ...) {
  if (err == ErrDefault) {
    err = ((Error[]){{
        .failure = true,
        .panic_on_emit = true,
        .location = location,
        .arena = {0},
    }});
  }

  err->failure = true;

  err->info = arena_calloc(&err->arena, sizeof(ErrorInfo));
  err->info->code = code;

  err->info->message = sb_init(&err->arena);
  da_init(&err->info->locations, &err->arena);

  da_push(&err->info->locations, location);

  va_list va;
  va_start(va, fmt);
  sb_append_va(&err->info->message, fmt, va);
  va_end(va);
  err->info->msg = sb_to_str(&err->info->message);
  sb_append_c(&err->info->message, '\n');

  if (err->panic_on_emit) {
    _error_internal_panic(err);
  }
}

bool _error_internal_occured(Error *err) { return err && err->failure; }

void _error_internal_panic(Error *err) {
  error_dump(err);
  arena_free(&err->arena);
  abort();
}

void _error_internal_except(Error *err) {
  arena_free(&err->arena);
  err->info = NULL;
  err->failure = false;
}

void _error_internal_set_code(Error *err, i32 code) { err->info->code = code; }

void _error_internal_set_msg(Error *err, const char *fmt, ...) {
  (void)err;
  va_list va;
  va_start(va, fmt);
  usize size = sb_append_va(&err->info->message, fmt, va);
  va_end(va);

  err->info->msg =
      str_from_parts(size, &da_last(&err->info->message) - size - 1);
  sb_append_c(&err->info->message, '\n');
}

void _error_internal_add_note(Error *err, const char *fmt, ...) {
  (void)err;
  va_list va;
  va_start(va, fmt);
  sb_append_va(&err->info->message, fmt, va);
  sb_append_c(&err->info->message, '\n');
  va_end(va);
}

void _error_internal_add_location(Error *err, FileLocation location) {
  da_push(&err->info->locations, location);
}

////////////////////////////////////////////////////////////////////////////
