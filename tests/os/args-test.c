#include "cebus.h"

static void test_basic(void) {
  Arena arena = {0};

  const char *argv[] = {"./a.out", "2", "bye", "--no-print"};
  int argc = ARRAY_LEN(argv);
  Args args = args_init(&arena, argc, argv);

  args_add_i64(&args, "number", "number of times the word will be used");
  args_add_opt_str(&args, "word", STR("hello"), "word that will be displayed");
  args_add_opt_flag(&args, "no-print", "if the message will be printed");

  cebus_assert(args_parse(&args), "parsing failed");

  i64 number = args_get_i64(&args, "number");
  cebus_assert(number == 2, "");
  cebus_assert(str_eq(args_get_str(&args, "word"), STR("bye")), "");
  cebus_assert(args_get_flag(&args, "no-print"), "");

  arena_free(&arena);
}

int main(void) { test_basic(); }
