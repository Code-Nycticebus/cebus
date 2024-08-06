#include "cebus/type/path.h"

#include "cebus/core/debug.h"
#include "cebus/type/string.h"

static void file_test(void) {
  Arena arena = {0};

  Path path = path_new(&arena, PATH("src"), PATH("cebus"), PATH("config.txt"));

  Path fullpath = PATH("src/cebus/config.txt");
  cebus_assert(str_eq(path, fullpath), STR_FMT, STR_ARG(path));

  Str name = path_name(path);
  cebus_assert(str_eq(name, STR("config")), STR_FMT, STR_ARG(name));

  Path parent = path_parent(path);
  cebus_assert(str_eq(parent, STR("src/cebus")), STR_FMT, STR_ARG(parent));

  arena_free(&arena);
}

static void directory_test(void) {
  Arena arena = {0};

  Path path = path_new(&arena, PATH("src"), PATH("cebus"), PATH("core"));

  Path fullpath = PATH("src/cebus/core");
  cebus_assert(str_eq(path, fullpath), STR_FMT, STR_ARG(path));

  Str name = path_name(path);
  cebus_assert(str_eq(name, PATH("core")), STR_FMT, STR_ARG(name));

  Path parent = path_parent(path);
  cebus_assert(str_eq(parent, PATH("src/cebus")), STR_FMT, STR_ARG(parent));

  arena_free(&arena);
}

int main(void) {
  file_test();
  directory_test();
}
