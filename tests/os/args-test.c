#include "cebus.h"
#include <string.h>

static void test_basic(void) {
  Arena arena = {0};

  const char *argv[] = {"./a.out", "-2", "2", "--str", "passed string", "-opt-flag"};
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

  const char *argv[] = {"./a.out", "--u64", "2", "-2", "positional string"};
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

static void test_positional_as_keyword(void) {
  Arena arena = {0};

  const char *argv[] = {"./a.out", "--u64", "2", "-2"};
  int argc = ARRAY_LEN(argv);
  Args args = args_init(&arena, argc, argv);

  args_add_i64(&args, "i64", "description of i64");
  args_add_u64(&args, "u64", "description of u64");

  cebus_assert(args_parse(&args), "parsing failed");

  i64 n1 = args_get_i64(&args, "i64");
  cebus_assert(n1 == -2, "");
  u64 n2 = args_get_u64(&args, "u64");
  cebus_assert(n2 == 2, "");

  arena_free(&arena);
}

static void test_iterating(void) {
  Arena arena = {0};

  const char *argv[] = {"./a.out", "-2", "37", "420"};
  int argc = ARRAY_LEN(argv);

  const Str expected[] = {
      STR_STATIC("./a.out"),
      STR_STATIC("-2"),
      STR_STATIC("37"),
      STR_STATIC("420"),
  };

  Args args = args_init(&arena, argc, argv);
  cebus_assert(str_eq(args.program, expected[0]), STR_REPR, STR_ARG(args.program));

  Str arg = {0};
  for (u32 i = 1; (arg = args_shift(&args)).data; ++i) {
    cebus_assert(str_eq(arg, expected[i]), STR_REPR, STR_ARG(arg));
  }

  arena_free(&arena);
}

static void test_iterating_traditional_way(void) {
  Arena arena = {0};

  const char *argv[] = {"./a.out", "-2", "37", "420"};
  int argc = ARRAY_LEN(argv);

  const char *expected[] = {"./a.out", "-2", "37", "420"};
  for (u32 i = 0; args_c_shift(&argc, (const char ***)&argv); ++i) {
    cebus_assert(strcmp(*argv, expected[i]), "");
  }

  arena_free(&arena);
}

static void test_getting_rest(void) {
  Arena arena = {0};

  const char *argv[] = {
      /* argv */
      "./a.out",           /* program name */
      "-32", "--opt-flag", /* positional and optional arguments */

      /* rest of arguments */
      "--",              /* delimiter */
      "-2", "37", "420", /* argv */
  };
  int argc = ARRAY_LEN(argv);

  Args args = args_init(&arena, argc, argv);

  args_add_i64(&args, "i64", "number description");
  args_add_opt_flag(&args, "opt-flag", "opt-flag description");

  cebus_assert(args_parse(&args), "parsing failed");

  i64 n1 = args_get_i64(&args, "i64");
  cebus_assert(n1 == -32, "");
  cebus_assert(args_get_flag(&args, "opt-flag"), "");

  const Str expected[] = {
      STR_STATIC("-2"),
      STR_STATIC("37"),
      STR_STATIC("420"),
      STR_STATIC("--opt-flag"),
  };
  u32 i = 0;
  for (Str arg = {0}; (arg = args_shift(&args)).data;) {
    cebus_assert(str_eq(arg, expected[i]), STR_REPR, STR_ARG(arg));
    ++i;
  }

  arena_free(&arena);
}

int main(void) {
  test_basic();
  test_advanced();
  test_positional_as_keyword();
  test_iterating();
  test_iterating_traditional_way();
  test_getting_rest();
}
