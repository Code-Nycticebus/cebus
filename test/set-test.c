
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

  arena_free(&arena);
}

static void test_subset(void) {
  Arena arena = {0};

  Set set1 = set_create(&arena, 10);
  for (usize i = 0; i < 10; i++) {
    set_add(&set1, i);
  }

  Set set2 = set_create(&arena, 10);
  for (usize i = 15; i < 25; i++) {
    set_add(&set2, i);
  }

  Set set3 = set_create(&arena, 10);
  for (usize i = 20; i < 30; i++) {
    set_add(&set3, i);
  }

  Set set4 = set_create(&arena, 20);
  for (usize i = 0; i < 20; i++) {
    set_add(&set4, i);
  }

  Set big_set = set_create(&arena, 20);
  for (usize i = 0; i < 20; i++) {
    set_add(&big_set, i);
  }

  clib_assert(set_is_subset(&set1, &big_set) == true,
              "set1 should be a subset");
  clib_assert(set_is_subset(&set2, &big_set) == false,
              "set2 should not be a subset");
  clib_assert(set_is_subset(&set3, &big_set) == false,
              "set3 should not be a subset");
  clib_assert(set_is_subset(&set4, &big_set) == false,
              "set4 should not be a subset");

  arena_free(&arena);
}

static void test_intersection(void) {
  Arena arena = {0};

  Set set1 = set_create(&arena, 10);
  set_add(&set1, 2);
  set_add(&set1, 3);
  set_add(&set1, 7);
  set_add(&set1, 8);

  Set big_set = set_create(&arena, 10);
  for (usize i = 0; i < 20; i++) {
    if (i % 2 == 0) {
      set_add(&big_set, i);
    }
  }

  Set inter = set_intersection(&set1, &big_set, &arena);

  clib_assert(set_is_subset(&inter, &big_set) == true,
              "inter should be a subset");

  clib_assert(set_contains(&inter, 2) == true, "inter should contain 2");
  clib_assert(set_contains(&inter, 3) == false, "inter should not contain 3");
  clib_assert(set_contains(&inter, 7) == false, "inter should not contain 7");
  clib_assert(set_contains(&inter, 8) == true, "inter should contain 8");

  arena_free(&arena);
}

int main(void) {
  test_set_insert();
  test_set_remove();
  test_subset();
  test_intersection();
}
