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
  args.hm = hm_create(arena);

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
  case ARG_TYPE_FLAG:
  case ARG_TYPE_NONE: {
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
}

bool args_parse(Args *args) {
  Error error = ErrNew;

  u32 positional = 0;
  for (Str arg; (arg = args_shift(args)).data;) {
    if (str_eq(arg, STR("-h"))) {
      args_print_usage(args, stdout);
      exit(0);
    }
    if (str_eq(arg, STR("--help"))) {
      args_print_help(args, stdout);
      exit(0);
    }

    bool is_optional = (arg.data[0] == '-') && (arg.data[1] == '-' || !c_is_digit(arg.data[1]));
    if (is_optional) {
      Str s = str_substring(arg, usize_min(str_count(arg, STR("-")), 2), arg.len);
      const usize *idx = hm_get_usize(args->hm, str_hash(s));
      if (idx == NULL) {
        error_emit(&error, ERR_PARSE, STR_REPR ": unknown argument", STR_ARG(arg));
        goto defer;
      }
      Argument *argument = &args->arguments.items[*idx];
      if (argument->type == ARG_TYPE_FLAG) {
        argument->as.flag = true; // flag turned on
      } else {
        args_parse_argument(argument, args_shift(args), &error);
        error_propagate(&error, { goto defer; });
      }
      continue;
    }

    while (true) {
      if (positional >= args->arguments.len) {
        error_emit(&error, ERR_PARSE, STR_REPR ": too many positional arguments", STR_ARG(arg));
        goto defer;
      }
      Argument *argument = &args->arguments.items[positional++];
      if (argument->type == ARG_TYPE_FLAG) {
        continue;
      }

      args_parse_argument(argument, arg, &error);
      error_propagate(&error, { goto defer; });
      break;
    }
  }

  if (positional < args->positional) {
    error_emit(&error, ERR_PARSE, "not enough positional arguments");
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
  void args_add_opt_##NAME(Args *args, const char *argument, T def, const char *description) {     \
    Str name = str_from_cstr(argument);                                                            \
    hm_insert_usize(args->hm, str_hash(name), da_len(&args->arguments));                           \
    da_push(&args->arguments, (Argument){                                                          \
                                  .name = name,                                                    \
                                  .description = str_from_cstr(description),                       \
                                  .type = T_ENUM,                                                  \
                                  .as.NAME = def,                                                  \
                              });                                                                  \
  }                                                                                                \
  void args_add_##NAME(Args *args, const char *argument, const char *description) {                \
    cebus_assert(args->positional == args->arguments.len,                                          \
                 "an optional argument was passed before!");                                       \
    args->positional++;                                                                            \
    args_add_opt_##NAME(args, argument, (T){0}, description);                                      \
  }                                                                                                \
  T args_get_##NAME(Args *args, const char *argument) {                                            \
    const usize *idx = hm_get_usize(args->hm, str_hash(str_from_cstr(argument)));                  \
    cebus_assert(idx && args->arguments.items[*idx].type == T_ENUM, "");                           \
    return args->arguments.items[*idx].as.NAME;                                                    \
  }

ARGS_GENERIC_IMPLEMENTATION(i64, i64, ARG_TYPE_I64)
ARGS_GENERIC_IMPLEMENTATION(u64, u64, ARG_TYPE_U64)
ARGS_GENERIC_IMPLEMENTATION(str, Str, ARG_TYPE_STR)

void args_add_opt_flag(Args *args, const char *argument, const char *description) {
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
  const usize *idx = hm_get_usize(args->hm, str_hash(str_from_cstr(argument)));
  cebus_assert(idx && args->arguments.items[*idx].type == ARG_TYPE_FLAG, "");
  return args->arguments.items[*idx].as.flag;
}
