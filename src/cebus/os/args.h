#ifndef __CEBUS_ARGS_H__
#define __CEBUS_ARGS_H__

#include "cebus/collection/da.h"
#include "cebus/collection/hashmap.h"
#include "cebus/core/defines.h"

typedef struct {
  Str name;
  Str description;
  enum {
    ARG_TYPE_NONE,
    ARG_TYPE_FLAG,
    ARG_TYPE_LIST,
    ARG_TYPE_I64,
    ARG_TYPE_U64,
    ARG_TYPE_STR,
  } type;
  union {
    bool flag;
    i64 i64;
    u64 u64;
    Str str;
  } as;
} Argument;

typedef struct {
  Str program;

  const char **argv;
  int argc;

  usize positional;
  DA(Argument) arguments;

  HashMap *hm;
} Args;

bool args_c_shift(int *argc, const char ***argv);

Args args_init(Arena *arena, int argc, const char **argv);
Str args_shift(Args *args);
void args_print_usage(Args *args, FILE *file);
void args_print_help(Args *args, FILE *file);
bool args_parse(Args *args);

void args_add_i64(Args *args, const char *argument, const char *description);
void args_add_u64(Args *args, const char *argument, const char *description);
void args_add_str(Args *args, const char *argument, const char *description);

void args_add_opt_i64(Args *args, const char *argument, i64 def, const char *description);
void args_add_opt_u64(Args *args, const char *argument, u64 def, const char *description);
void args_add_opt_str(Args *args, const char *argument, Str def, const char *description);
void args_add_opt_flag(Args *args, const char *argument, const char *description);

i64 args_get_i64(Args *args, const char *argument);
u64 args_get_u64(Args *args, const char *argument);
Str args_get_str(Args *args, const char *argument);
bool args_get_flag(Args *args, const char *argument);

#endif /* !__CEBUS_ARGS_H__ */
