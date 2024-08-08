#include "cebus.h"

int main(void) {
  Arena arena = {0};

  const char *argv[] = {"./a.out", "2", "--word", "bye", "--no-print"};
  int argc = ARRAY_LEN(argv);
  Args args = args_init(&arena, argc, argv);

  args_add_i64(&args, "number", "number of times the word will be used");
  args_add_opt_str(&args, "word", STR("hello"), "word that will be displayed");
  args_add_opt_flag(&args, "no-print", "if the message will be printed");

  if (!args_parse(&args)) {
    return 1;
  }

  cebus_assert(args_get_i64(&args, "number") == 2, "");
  cebus_assert(str_eq(args_get_str(&args, "word"), STR("bye")), "");
  cebus_assert(args_get_flag(&args, "no-print"), "");

  arena_free(&arena);
}
