#include "clib/asserts.h"
#include "collections/set.h"
#include "collections/vec.h"
#include "core/defines.h"
#include "types/integers.h"
#include "types/str.h"

static void test_set_insert(void) {
  Arena arena = {0};
  Set set = set_create(&arena, 10); // NOLINT

  for (usize i = 0; i < 10; i++) { // NOLINT
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
  Set set = set_create(&arena, 10); // NOLINT

  for (usize i = 0; i < 10; i++) { // NOLINT
    set_add(&set, usize_hash(i));
  }

  set_remove(&set, usize_hash(2));
  set_remove(&set, usize_hash(4));
  set_remove(&set, usize_hash(5)); // NOLINT

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
  Set set1 = set_create(&arena, 10); // NOLINT
  set_add(&set1, usize_hash(1));
  set_add(&set1, usize_hash(2));
  set_add(&set1, usize_hash(3));

  Set set2 = set_create(&arena, 10); // NOLINT
  set_add(&set2, usize_hash(1));
  set_add(&set2, usize_hash(2));
  set_add(&set2, usize_hash(3));

  clib_assert(set_eq(&set1, &set2), "Sets should be equal");

  arena_free(&arena);
}

static void test_subset(void) {
  Arena arena = {0};

  Set set1 = set_create(&arena, 10); // NOLINT
  for (usize i = 0; i < 10; i++) {   // NOLINT
    set_add(&set1, usize_hash(i));
  }

  Set set2 = set_create(&arena, 10); // NOLINT
  for (usize i = 15; i < 25; i++) {  // NOLINT
    set_add(&set2, usize_hash(i));
  }
  Set set3 = set_create(&arena, 20); // NOLINT
  for (usize i = 0; i < 20; i++) {   // NOLINT
    set_add(&set3, usize_hash(i));
  }

  Set big_set = set_create(&arena, 20); // NOLINT
  for (usize i = 0; i < 20; i++) {      // NOLINT
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

  const usize test_numbers[] = {2, 3, 7, 8};
  const usize count = sizeof(test_numbers) / sizeof(test_numbers[0]);

  Set set1 = set_create(&arena, count * 2);
  for (usize i = 0; i < count; i++) {
    set_add(&set1, usize_hash(test_numbers[i]));
  }

  Set big_set = set_create(&arena, count * 4);
  for (usize i = 0; i < count * 4; i++) {
    if (i % 2 == 0) {
      set_add(&big_set, usize_hash(i));
    }
  }

  Set inter = set_intersection(&set1, &big_set, &arena);

  clib_assert(set_subset(&inter, &big_set) == true, "inter should be a subset");

  for (usize i = 0; i < count; i++) {
    clib_assert(set_contains(&inter, usize_hash(test_numbers[i])) ==
                    (test_numbers[i] % 2 == 0),
                "diff: %" USIZE_FMT, test_numbers[i]);
  }

  arena_free(&arena);
}

static bool filter_with_set(Set *intersection, Str s) {
  return set_contains(intersection, str_hash(s));
}

static void test_difference(void) {
  Arena arena = {0};

  const usize test_numbers[] = {2, 3, 7, 8};
  const usize count = sizeof(test_numbers) / sizeof(test_numbers[0]);

  Set set1 = set_create(&arena, count * 2);
  for (usize i = 0; i < count; i++) {
    set_add(&set1, usize_hash(test_numbers[i]));
  }

  Set big_set = set_create(&arena, count * 4);
  for (usize i = 0; i < count * 4; i++) {
    if (i % 2 == 0) {
      set_add(&big_set, usize_hash(i));
    }
  }

  Set diff = set_difference(&set1, &big_set, &arena);

  clib_assert(set_subset(&diff, &big_set) == false,
              "diff should not be a subset");

  for (usize i = 0; i < count; i++) {
    clib_assert(set_contains(&diff, usize_hash(test_numbers[i])) ==
                    (test_numbers[i] % 2 != 0),
                "diff: %" USIZE_FMT, test_numbers[i]);
  }

  arena_free(&arena);
}

static void test_example_duplicates(void) {
  Arena arena = {0};

  Str list[] = {
      STR("Apple"), STR("Banana"), STR("Apple"), STR("Cherry"), STR("Apple"),
  };
  const usize count = sizeof(list) / sizeof(list[0]);

  Set unique = set_create(&arena, count * 2);
  Set duplicates = set_create(&arena, count * 2);

  for (usize i = 0; i < count; i++) {
    u64 hash = str_hash(list[i]);
    if (set_contains(&unique, hash)) {
      set_add(&duplicates, hash);
    } else {
      set_add(&unique, hash);
    }
  }

  VEC(Str) unique_strings = {0};
  vec_init(&unique_strings, 5, &arena);
  vec_extend(&unique_strings, count, list);

  vec_filter_ctx(&unique_strings, &unique_strings, !filter_with_set,
                 &duplicates);

  clib_assert(unique_strings.len == 2,
              "Did not find the unique strings correctly");
  clib_assert(str_eq(unique_strings.items[0], STR("Banana")),
              "Did not filter out correctly");
  clib_assert(str_eq(unique_strings.items[1], STR("Cherry")),
              "Did not filter out correctly");

  arena_free(&arena);
}

static void test_example_intersection(void) {
  Arena arena = {0};

  Str strings1[] = {
      STR("Apple"), STR("Banana"), STR("Cherry"), STR("Pomegrade"),
      STR("Grape"), STR("Orange"), STR("Pear"),
  };
  const usize count = sizeof(strings1) / sizeof(strings1[0]);
  Set s1 = set_create(&arena, count * 2);
  for (size_t i = 0; i < count; i++) {
    set_add(&s1, str_hash(strings1[i]));
  }

  Str strings2[] = {
      STR("Apple"),  STR("Strawberry"), STR("Blueberry"), STR("Banana"),
      STR("Cherry"), STR("Lemon"),      STR("Pear"),
  };
  const usize count2 = sizeof(strings2) / sizeof(strings2[0]);

  Set s2 = set_create(&arena, count2 * 2);
  for (size_t i = 0; i < count2; i++) {
    set_add(&s2, str_hash(strings2[i]));
  }

  Set inter = set_intersection(&s1, &s2, &arena);
  VEC(Str) intersecting = {0};
  vec_init(&intersecting, 5, &arena);
  vec_extend(&intersecting, count, strings1);
  vec_filter_ctx(&intersecting, &intersecting, filter_with_set, &inter);

  clib_assert(intersecting.len == 4,
              "Did not find the intersection correctly: %" USIZE_FMT,
              intersecting.len);
  clib_assert(str_eq(intersecting.items[0], STR("Apple")),
              "Did not filter out correctly");
  clib_assert(str_eq(intersecting.items[1], STR("Banana")),
              "Did not filter out correctly");
  clib_assert(str_eq(intersecting.items[2], STR("Cherry")),
              "Did not filter out correctly");
  clib_assert(str_eq(intersecting.items[3], STR("Pear")),
              "Did not filter out correctly");

  arena_free(&arena);
}

int main(void) {
  test_set_insert();
  test_set_remove();
  test_eq();
  test_subset();
  test_intersection();
  test_difference();

  test_example_duplicates();
  test_example_intersection();
}
