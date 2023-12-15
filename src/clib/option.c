#include "option.h"
#include <stdio.h>
#include <stdlib.h>

#define OPTION_TO_STR(enum, _id, T)                                            \
  case enum:                                                                   \
    return #T;

#define OPTION_IMPL_SOME(enum, id, T)                                          \
  Option option_some_##id(T value) {                                           \
    return (Option){.type = enum, .val.id = value};                            \
  }

#define OPTION_IMPL_UNWRAP(enum, id, T)                                        \
  T _option_unwrap_##id(Option o, const char *file, int line) {                \
    if (o.type == OPTION_NONE) {                                               \
      option_none_error("Unwrapping failed", file, line);                      \
    }                                                                          \
    if (o.type != enum) {                                                      \
      option_type_error(enum, o.type, file, line);                             \
    }                                                                          \
    return o.val.id;                                                           \
  }

#define OPTION_IMPL_EXPECT(enum, id, T)                                        \
  T _option_expect_##id(Option o, const char *msg, const char *file,           \
                        int line) {                                            \
    if (o.type == OPTION_NONE) {                                               \
      option_none_error(msg, file, line);                                      \
    }                                                                          \
    if (o.type != enum) {                                                      \
      option_type_error(enum, o.type, file, line);                             \
    }                                                                          \
    return o.val.id;                                                           \
  }

#define OPTION_IMPL_MAP(enum, id, T)                                           \
  Option option_map_##id(Option o, T (*map)(T)) {                              \
    option_propagate(o);                                                       \
    if (o.type != enum) {                                                      \
      option_type_error(enum, o.type, __FILE__, __LINE__);                     \
    }                                                                          \
    return option_some_##id(map(o.val.id));                                    \
  }

#define OPTION_IMPL_DEFAULT(enum, id, T)                                       \
  T option_default_##id(Option o, T def) {                                     \
    if (o.type == OPTION_NONE) {                                               \
      return def;                                                              \
    }                                                                          \
    if (o.type != enum) {                                                      \
      option_type_error(enum, o.type, __FILE__, __LINE__);                     \
    }                                                                          \
    return o.val.id;                                                           \
  }

static const char *option_type_to_str(OptionTypes type) {
  switch (type) {
    OPTION_X_DO(OPTION_TO_STR)
  case OPTION_NONE:
    return "None";
  }
  fprintf(stderr, "Option Type: Unreachable %d\n", type);
  abort();
}

static void option_none_error(const char *msg, const char *file, int line) {
  fprintf(stderr, "%s:%d: Option: %s\n", file, line, msg);
  abort();
}

static void option_type_error(OptionTypes expected, OptionTypes got,
                              const char *file, int line) {
  fprintf(stderr,
          " %s:%d: Unwrapping failed: types do not match: expected: '%s' got: "
          "'%s'\n",
          file, line, option_type_to_str(expected), option_type_to_str(got));
  abort();
}

bool option_is_some(Option o) { return o.type != OPTION_NONE; }
bool option_is_none(Option o) { return o.type == OPTION_NONE; }

Option option_none(void) { return (Option){0}; }

OPTION_X_DO(OPTION_IMPL_SOME)
OPTION_X_DO(OPTION_IMPL_UNWRAP)
OPTION_X_DO(OPTION_IMPL_EXPECT)
OPTION_X_DO(OPTION_IMPL_MAP)
OPTION_X_DO(OPTION_IMPL_DEFAULT)
