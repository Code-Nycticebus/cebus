
#include "clib/asserts.h"
#include "containers/set.h"
#include "datatypes/integers.h"

static void test_set_insert(void) {
  Arena arena = {0};
  Set set = set_create(&arena, 10);

  for (usize i = 0; i < 10; i++) {
    set_add(&set, usize_hash(i));
  }

  clib_assert(set_contains(&set, usize_hash(2)) == true,
              "Set should contain this number!");
  clib_assert(set_contains(&set, usize_hash(3)) == true,
              "Set should contain this number!");
  clib_assert(set_contains(&set, usize_hash(5)) == true,
              "Set should contain this number!");

  arena_free(&arena);
}

static void test_set_remove(void) {
  Arena arena = {0};
  Set set = set_create(&arena, 10);

  for (usize i = 0; i < 10; i++) {
    set_add(&set, usize_hash(i));
  }

  set_remove(&set, usize_hash(2));
  set_remove(&set, usize_hash(4));
  set_remove(&set, usize_hash(5));

  clib_assert(set_contains(&set, usize_hash(2)) == false,
              "Set should not contain this number!");
  clib_assert(set_contains(&set, usize_hash(3)) == true,
              "Set should still contain this number!");
  clib_assert(set_contains(&set, usize_hash(4)) == false,
              "Set should not contain this number!");
  clib_assert(set_contains(&set, usize_hash(5)) == false,
              "Set should not contain this number!");

  arena_free(&arena);
}

static void test_eq(void) {
  Arena arena = {0};
  Set set1 = set_create(&arena, 10);
  set_add(&set1, usize_hash(1));
  set_add(&set1, usize_hash(2));
  set_add(&set1, usize_hash(3));

  Set set2 = set_create(&arena, 10);
  set_add(&set2, usize_hash(1));
  set_add(&set2, usize_hash(2));
  set_add(&set2, usize_hash(3));

  clib_assert(set_eq(&set1, &set2), "Sets should be equal");

  arena_free(&arena);
}

static void test_subset(void) {
  Arena arena = {0};

  Set set1 = set_create(&arena, 10);
  for (usize i = 0; i < 10; i++) {
    set_add(&set1, usize_hash(i));
  }

  Set set2 = set_create(&arena, 10);
  for (usize i = 15; i < 25; i++) {
    set_add(&set2, usize_hash(i));
  }
  Set set3 = set_create(&arena, 20);
  for (usize i = 0; i < 20; i++) {
    set_add(&set3, usize_hash(i));
  }

  Set big_set = set_create(&arena, 20);
  for (usize i = 0; i < 20; i++) {
    set_add(&big_set, usize_hash(i));
  }

  clib_assert(set_subset(&set1, &big_set) == true, "set1 should be a subset");
  clib_assert(set_subset(&set2, &big_set) == false,
              "set2 should not be a subset");
  clib_assert(set_subset(&set3, &big_set) == false,
              "set4 should not be a subset");

  arena_free(&arena);
}

static void test_intersection(void) {
  Arena arena = {0};

  Set set1 = set_create(&arena, 10);
  set_add(&set1, usize_hash(2));
  set_add(&set1, usize_hash(3));
  set_add(&set1, usize_hash(7));
  set_add(&set1, usize_hash(8));

  Set big_set = set_create(&arena, 10);
  for (usize i = 0; i < 20; i++) {
    if (i % 2 == 0) {
      set_add(&big_set, usize_hash(i));
    }
  }

  Set inter = set_intersection(&set1, &big_set, &arena);

  clib_assert(set_subset(&inter, &big_set) == true, "inter should be a subset");

  clib_assert(set_contains(&inter, usize_hash(2)) == true,
              "inter should contain 2");
  clib_assert(set_contains(&inter, usize_hash(3)) == false,
              "inter should not contain 3");
  clib_assert(set_contains(&inter, usize_hash(4)) == false,
              "inter should not contain 4");
  clib_assert(set_contains(&inter, usize_hash(7)) == false,
              "inter should not contain 7");
  clib_assert(set_contains(&inter, usize_hash(8)) == true,
              "inter should contain 8");

  arena_free(&arena);
}

static void test_difference(void) {
  Arena arena = {0};

  Set set1 = set_create(&arena, 10);
  set_add(&set1, usize_hash(2));
  set_add(&set1, usize_hash(3));
  set_add(&set1, usize_hash(7));
  set_add(&set1, usize_hash(8));

  Set big_set = set_create(&arena, 10);
  for (usize i = 0; i < 20; i++) {
    if (i % 2 == 0) {
      set_add(&big_set, usize_hash(i));
    }
  }

  Set diff = set_difference(&set1, &big_set, &arena);

  clib_assert(set_subset(&diff, &big_set) == false,
              "inter should not be a subset");

  clib_assert(set_contains(&diff, usize_hash(2)) == false,
              "diff should not contain 2");
  clib_assert(set_contains(&diff, usize_hash(3)) == true,
              "diff should contain 3");
  clib_assert(set_contains(&diff, usize_hash(4)) == false,
              "diff should not contain 4");
  clib_assert(set_contains(&diff, usize_hash(7)) == true,
              "diff should contain 7");
  clib_assert(set_contains(&diff, usize_hash(8)) == false,
              "diff should not contain 8");

  arena_free(&arena);
}

int main(void) {
  test_set_insert();
  test_set_remove();
  test_eq();
  test_subset();
  test_intersection();
  test_difference();
}
