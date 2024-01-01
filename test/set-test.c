
#include "clib/asserts.h"
#include "containers/set.h"

static void test_set_insert(void) {
  Set set = {0};

  for (usize i = 0; i < 10; i++) {
    set_insert(&set, i);
  }

  clib_assert(set_contains(&set, 2) == true, "Set should contain this number!");
  clib_assert(set_contains(&set, 4) == true, "Set should contain this number!");
  clib_assert(set_contains(&set, 5) == true, "Set should contain this number!");

  set_free(&set);
}

static void test_set_remove(void) {
  Set set = {0};

  for (usize i = 0; i < 10; i++) {
    set_insert(&set, i);
  }

  set_remove(&set, 2);
  set_remove(&set, 4);
  set_remove(&set, 5);

  clib_assert(set_contains(&set, 2) == false,
              "Set should not contain this number!");
  clib_assert(set_contains(&set, 4) == false,
              "Set should not contain this number!");
  clib_assert(set_contains(&set, 5) == false,
              "Set should not contain this number!");

  set_free(&set);
}

int main(void) {
  test_set_insert();
  test_set_remove();
}
