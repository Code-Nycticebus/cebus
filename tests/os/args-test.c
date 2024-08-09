#include "cebus.h"

static void test_basic(void) {
  Arena arena = {0};

  const char *argv[] = {"./a.out", "-2", "2", "--str", "passed string", "--opt-flag"};
  int argc = ARRAY_LEN(argv);
  Args args = args_init(&arena, argc, argv);

  args_add_i64(&args, "i64", "description of i64");
  args_add_u64(&args, "u64", "description of u64");
  args_add_opt_str(&args, "str", STR("optional string"), "description of optional string");
  args_add_opt_flag(&args, "opt-flag", "description of flag");

  cebus_assert(args_parse(&args), "parsing failed");

  i64 n1 = args_get_i64(&args, "i64");
  cebus_assert(n1 == -2, "");
  u64 n2 = args_get_u64(&args, "u64");
  cebus_assert(n2 == 2, "");
  Str s1 = args_get_str(&args, "str");
  cebus_assert(str_eq(s1, STR("passed string")), "");
  cebus_assert(args_get_flag(&args, "opt-flag"), "");

  arena_free(&arena);
}

static void test_advanced(void) {
  Arena arena = {0};

  const char *argv[] = {"./a.out", "-2", "2", "positional string"};
  int argc = ARRAY_LEN(argv);
  Args args = args_init(&arena, argc, argv);

  args_add_i64(&args, "i64", "description of i64");
  args_add_u64(&args, "u64", "description of u64");
  args_add_opt_str(&args, "str", STR("optional string"), "description of optional string");
  args_add_opt_flag(&args, "opt-flag", "description of flag");

  cebus_assert(args_parse(&args), "parsing failed");

  i64 n1 = args_get_i64(&args, "i64");
  cebus_assert(n1 == -2, "");
  u64 n2 = args_get_u64(&args, "u64");
  cebus_assert(n2 == 2, "");
  Str s1 = args_get_str(&args, "str");
  cebus_assert(str_eq(s1, STR("positional string")), "");
  cebus_assert(!args_get_flag(&args, "opt-flag"), "");

  arena_free(&arena);
}

int main(void) {
  test_basic();
  test_advanced();
}
