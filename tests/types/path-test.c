#include "cebus/type/path.h"

#include "cebus/core/debug.h"
#include "cebus/type/string.h"

#define ASSERT(s, e)                                                           \
  cebus_assert(str_eq(s, e), STR_FMT ":" STR_FMT, STR_ARG(s), STR_ARG(e))

static void test_file(void) {
  Arena arena = {0};

  Path expected_path = PATH("src/cebus/config.txt");
  Str expected_name = STR("config");
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

static void test(void) {
  Arena arena = {0};

  arena_free(&arena);
}

int main(void) {
  test_file();
  test_directory();
  test();
}
