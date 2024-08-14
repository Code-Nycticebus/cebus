#include "args.h"

#include "cebus/core/debug.h"
#include "cebus/core/error.h"
#include "cebus/type/char.h"
#include "cebus/type/integer.h"
#include "cebus/type/string.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum {
  ERR_INTERNAL,
  ERR_PARSE,
} ArgParseError;

bool args_c_shift(int *argc, const char ***argv) {
  if ((*argc) == 0) {
    return false;
  }
  (*argc)--;
  (*argv)++;
  return true;
}

Str args_shift(Args *args) {
  if (args->argc == 0) {
    return (Str){0};
  }
  args->argc--;
  return str_from_cstr(*args->argv++);
}

Args args_init(Arena *arena, int argc, const char **argv) {
  Args args = {.argc = argc, .argv = argv};

  da_init(&args.arguments, arena);
  args.hm = NULL;
  hm_create(arena);

  args.program = args_shift(&args);

  return args;
}

void args_print_usage(Args *args, FILE *file) {
  fprintf(file, STR_FMT, STR_ARG(args->program));
  for (u32 i = 0; i < args->positional; ++i) {
    Argument *argument = &args->arguments.items[i];
    fprintf(file, " <" STR_FMT ">", STR_ARG(argument->name));
  }
  if (args->arguments.len > args->positional) {
    fprintf(file, " [...options]");
  }
  fprintf(file, "\n");
}

void args_print_help(Args *args, FILE *file) {
  args_print_usage(args, file);

  for (u32 i = 0; i < args->arguments.len && i < args->positional; ++i) {
    Argument *argument = &args->arguments.items[i];
    const i32 padding = i32_max(0, 25 - (i32)argument->name.len);
    fprintf(file, " %2d. " STR_FMT " %*c " STR_FMT "\n", i + 1, STR_ARG(argument->name), padding,
            ' ', STR_ARG(argument->description));
  }

  for (usize i = args->positional; i < args->arguments.len; i++) {
    Argument *argument = &args->arguments.items[i];
    const i32 padding = i32_max(0, 28 - (i32)argument->name.len);
    fprintf(file, "  --" STR_FMT "%*c" STR_FMT "\n", STR_ARG(argument->name), padding, ' ',
            STR_ARG(argument->description));
  }
}

static void args_parse_argument(Argument *argument, Str arg, Error *error) {
  if (arg.data == NULL || str_startswith(arg, STR("--"))) {
    error_emit(error, ERR_PARSE, STR_REPR " needs parameter", STR_ARG(argument->name));
  }
  switch (argument->type) {
  case ARG_TYPE_NONE:
  case ARG_TYPE_FLAG:
  case ARG_TYPE_LIST: {
    error_emit(error, ERR_INTERNAL, "this type should never be parsed here: %d", argument->type);
    return;
  } break;
  case ARG_TYPE_I64: {
    bool minus_number = arg.data[0] == '-' && c_is_digit(arg.data[1]);
    if (!minus_number && !c_is_digit(arg.data[0])) {
      error_emit(error, ERR_PARSE, STR_REPR ": " STR_REPR " not a valid i64",
                 STR_ARG(argument->name), STR_ARG(arg));
      return;
    }
    argument->as.i64 = str_i64(arg);
  } break;
  case ARG_TYPE_U64: {
    bool minus_number = arg.data[0] == '-' && c_is_digit(arg.data[1]);
    if (minus_number) {
      error_emit(error, ERR_PARSE, STR_REPR ": " STR_REPR "minus numbers are not valid u64",
                 STR_ARG(argument->name), STR_ARG(arg));
      return;
    }
    if (!c_is_digit(arg.data[0])) {
      error_emit(error, ERR_PARSE, STR_REPR ": " STR_REPR " not a valid u64",
                 STR_ARG(argument->name), STR_ARG(arg));
      return;
    }
    argument->as.u64 = str_u64(arg);
  } break;
  case ARG_TYPE_STR: {
    argument->as.str = arg;
  } break;
  }

  argument->parsed = true;
}

bool args_parse(Args *args) {
  Error error = ErrNew;

  u32 argument_idx = 0;
  u32 positional_count = 0;
  for (Str arg = {0}; (arg = args_shift(args)).data;) {
    if (str_eq(arg, STR("-h"))) {
      args_print_usage(args, stdout);
      exit(0);
    }
    if (str_eq(arg, STR("--help"))) {
      args_print_help(args, stdout);
      exit(0);
    }

    bool arg_is_optional = arg.data[0] == '-' && (arg.data[1] == '-' || !c_is_digit(arg.data[1]));
    if (arg_is_optional) {
      if (str_eq(arg, STR("--"))) {
        return true;
      }
      Str argument_prefix = str_substring(arg, 0, 2);
      usize dash_count = str_count(argument_prefix, STR("-"));
      Str real_argument = str_substring(arg, usize_clamp(0, 2, dash_count), arg.len);
      const usize *idx = hm_get_usize(args->hm, str_hash(real_argument));
      if (idx == NULL) {
        error_emit(&error, ERR_PARSE, STR_REPR ": unknown argument", STR_ARG(arg));
        goto defer;
      }
      Argument *argument = &args->arguments.items[*idx];
      if (argument->parsed) {
        error_emit(&error, ERR_PARSE, STR_REPR ": argument was passed twice", STR_ARG(arg));
        goto defer;
      }
      if (argument->type == ARG_TYPE_FLAG) {
        argument->as.flag = true; // flag turned on
        continue;
      }
      if (argument->pos) {
        positional_count++;
      }
      args_parse_argument(argument, args_shift(args), &error);
      error_propagate(&error, { goto defer; });

      continue;
    }

    while (true) {
      if (argument_idx >= args->arguments.len) {
        error_emit(&error, ERR_PARSE,
                   STR_REPR ": too many positional arguments: got %d expected %" USIZE_FMT,
                   STR_ARG(arg), argument_idx, args->positional);
        goto defer;
      }
      Argument *argument = &args->arguments.items[argument_idx++];
      if (argument->parsed) {
        continue; // next
      }
      if (argument->type == ARG_TYPE_FLAG) {
        continue; // next
      }

      positional_count++;
      args_parse_argument(argument, arg, &error);
      error_propagate(&error, { goto defer; });

      break;
    }
  }

  if (positional_count < args->positional) {
    error_emit(&error, ERR_PARSE, "not enough positional arguments: got %d expected %" USIZE_FMT,
               positional_count, args->positional);
    goto defer;
  }

defer:
  error_context(&error, {
    if (error_code(ArgParseError) == ERR_PARSE) {
      fprintf(stderr, STR_FMT "\n", STR_ARG(error_msg()));
      args_print_usage(args, stderr);
      error_except();
      return false;
    }
  });

  return true;
}

#define ARGS_GENERIC_IMPLEMENTATION(NAME, T, T_ENUM)                                               \
  void args_add_##NAME(Args *args, const char *argument, const char *description) {                \
    cebus_assert(args->positional == args->arguments.len,                                          \
                 "an optional argument was passed before!");                                       \
    args->positional++;                                                                            \
    args->hm = args->hm ? args->hm : hm_create(args->arguments.arena);                             \
    Str name = str_from_cstr(argument);                                                            \
    hm_insert_usize(args->hm, str_hash(name), da_len(&args->arguments));                           \
    da_push(&args->arguments, (Argument){                                                          \
                                  .name = name,                                                    \
                                  .description = str_from_cstr(description),                       \
                                  .pos = true,                                                     \
                                  .type = T_ENUM,                                                  \
                                  .as = {0},                                                       \
                              });                                                                  \
  }                                                                                                \
  void args_add_opt_##NAME(Args *args, const char *argument, T def, const char *description) {     \
    args->hm = args->hm ? args->hm : hm_create(args->arguments.arena);                             \
    Str name = str_from_cstr(argument);                                                            \
    hm_insert_usize(args->hm, str_hash(name), da_len(&args->arguments));                           \
    da_push(&args->arguments, (Argument){                                                          \
                                  .name = name,                                                    \
                                  .description = str_from_cstr(description),                       \
                                  .type = T_ENUM,                                                  \
                                  .as.NAME = def,                                                  \
                              });                                                                  \
  }                                                                                                \
  T args_get_##NAME(Args *args, const char *argument) {                                            \
    cebus_assert(args->hm != NULL, "no arguments were given");                                     \
    const usize *idx = hm_get_usize(args->hm, str_hash(str_from_cstr(argument)));                  \
    cebus_assert(idx && args->arguments.items[*idx].type == T_ENUM, "");                           \
    return args->arguments.items[*idx].as.NAME;                                                    \
  }

ARGS_GENERIC_IMPLEMENTATION(i64, i64, ARG_TYPE_I64)
ARGS_GENERIC_IMPLEMENTATION(u64, u64, ARG_TYPE_U64)
ARGS_GENERIC_IMPLEMENTATION(str, Str, ARG_TYPE_STR)

void args_add_opt_flag(Args *args, const char *argument, const char *description) {
  args->hm = args->hm ? args->hm : hm_create(args->arguments.arena);
  usize idx = da_len(&args->arguments);
  Str name = str_from_cstr(argument);
  hm_insert_usize(args->hm, str_hash(name), idx);
  da_push(&args->arguments, (Argument){
                                .name = name,
                                .description = str_from_cstr(description),
                                .type = ARG_TYPE_FLAG,
                                .as.flag = false,
                            });
}

bool args_get_flag(Args *args, const char *argument) {
  cebus_assert(args->hm != NULL, "no arguments were given");
  const usize *idx = hm_get_usize(args->hm, str_hash(str_from_cstr(argument)));
  cebus_assert(idx && args->arguments.items[*idx].type == ARG_TYPE_FLAG, "");
  return args->arguments.items[*idx].as.flag;
}
