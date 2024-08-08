#include "cebus/type/path.h"

#include "cebus/core/debug.h"
#include "cebus/type/string.h"

#define ASSERT(s, e)                                                                               \
  cebus_assert(str_eq(s, e), "'" STR_FMT "' : '" STR_FMT "'", STR_ARG(s), STR_ARG(e))

typedef struct {
  Path test;
  Path expected;
} TestCases;

#define test_case(cases, fn)                                                                       \
  for (u32 i = 0; i < ARRAY_LEN(cases); ++i) {                                                     \
    Str out = fn(cases[i].test);                                                                   \
    ASSERT(out, tests[i].expected);                                                                \
  }

static void test_name(void) {
  TestCases tests[] = {
      {PATH("."), STR("")},
      {PATH("./src"), STR("src")},
      {PATH("./src/config.txt"), STR("config.txt")},
      {PATH("./.conf/config.txt"), STR("config.txt")},
  };
  test_case(tests, path_name);
}

static void test_suffix(void) {
  TestCases tests[] = {
      {.test = PATH("."), .expected = PATH("")},
      {.test = PATH("./src/config.txt"), .expected = PATH(".txt")},
      {.test = PATH("./src"), .expected = PATH("")},
      {.test = PATH("./.conf/config.txt"), .expected = PATH(".txt")},
      {.test = PATH("./.conf/.config.txt"), .expected = PATH(".txt")},
  };
  test_case(tests, path_suffix);
}

static void test_stem(void) {
  TestCases tests[] = {
      {.test = PATH("."), .expected = PATH("")},
      {.test = PATH("./src"), .expected = PATH("src")},
      {.test = PATH("./src/config.txt"), .expected = PATH("config")},
      {.test = PATH("./.conf/config.txt"), .expected = PATH("config")},
      {.test = PATH("./.conf/.config.txt"), .expected = PATH(".config")},
  };
  test_case(tests, path_stem);
}

static void test_parent(void) {
  TestCases tests[] = {
      {.test = PATH("."), .expected = PATH("")},
      {.test = PATH("./src"), .expected = PATH(".")},
      {.test = PATH("./src/config.txt"), .expected = PATH("./src")},
      {.test = PATH("./.conf/config.txt"), .expected = PATH("./.conf")},
  };
  test_case(tests, path_parent);
}

static void test_file(void) {
  Arena arena = {0};

  Path expected_path = PATH("src/cebus/config.txt");
  Str expected_name = STR("config.txt");
  Path expected_parent = PATH("src/cebus");

  Path path = path_new(&arena, PATH("src"), PATH("cebus"), PATH("config.txt"));
  ASSERT(path, expected_path);
  Str name = path_name(path);
  ASSERT(name, expected_name);
  Path parent = path_parent(path);
  ASSERT(parent, expected_parent);

  arena_free(&arena);
}

static void test_directory(void) {
  Arena arena = {0};

  Path expected_path = PATH("src/cebus/core");
  Str expected_name = STR("core");
  Path expected_parent = PATH("src/cebus");

  Path path = path_new(&arena, PATH("src"), PATH("cebus"), PATH("core"));
  ASSERT(path, expected_path);
  Str name = path_name(path);
  ASSERT(name, expected_name);
  Path parent = path_parent(path);
  ASSERT(parent, expected_parent);

  arena_free(&arena);
}

int main(void) {
  test_name();
  test_suffix();
  test_stem();
  test_parent();

  test_file();
  test_directory();
}
