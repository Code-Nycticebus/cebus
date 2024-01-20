#include "collections/set.h"

#include "collections/vec.h"
#include "core/asserts.h"
#include "core/defines.h"
#include "types/integers.h"
#include "types/str.h"

#define TEST_SET_DEFAULT_SIZE 10

static void test_set_insert(void) {
  Arena arena = {0};
  Set set = set_create(&arena);

  const u64 numbers[] = {
      usize_hash(1),
      usize_hash(2),
      usize_hash(3),
  };
  set_extend(&set, 3, numbers);

  set_reserve(&set, TEST_SET_DEFAULT_SIZE * 2);
  for (usize i = 4; i < TEST_SET_DEFAULT_SIZE * 2; i++) {
    set_add(&set, usize_hash(i));
  }

  clib_assert(set_contains(&set, usize_hash(1)) == true,
              "Set should contain this number!");
  clib_assert(set_contains(&set, usize_hash(2)) == true,
              "Set should contain this number!");
  clib_assert(set_contains(&set, usize_hash(3)) == true,
              "Set should contain this number!");
  clib_assert(set_contains(&set, usize_hash(4)) == true,
              "Set should contain this number!");
  clib_assert(set_contains(&set, usize_hash(5)) == true,
              "Set should contain this number!");

  arena_free(&arena);
}

static void test_eq(void) {
  Arena arena = {0};
  Set set1 = set_create(&arena);
  set_add(&set1, usize_hash(1));
  set_add(&set1, usize_hash(2));
  set_add(&set1, usize_hash(3));

  Set set2 = set_create(&arena);
  set_add(&set2, usize_hash(1));
  set_add(&set2, usize_hash(2));
  set_add(&set2, usize_hash(3));

  clib_assert(set_eq(&set1, &set2), "Sets should be equal");

  arena_free(&arena);
}

static void test_subset(void) {
  Arena arena = {0};

  Set set1 = set_create(&arena);
  for (usize i = 0; i < TEST_SET_DEFAULT_SIZE; i++) {
    set_add(&set1, usize_hash(i));
  }

  Set set2 = set_create(&arena);
  for (usize i = 15; i < 25; i++) { // NOLINT
    set_add(&set2, usize_hash(i));
  }
  Set set3 = set_create(&arena);
  for (usize i = 0; i < TEST_SET_DEFAULT_SIZE * 2; i++) {
    set_add(&set3, usize_hash(i));
  }

  Set big_set = set_create(&arena);
  for (usize i = 0; i < TEST_SET_DEFAULT_SIZE * 2; i++) {
    set_add(&big_set, usize_hash(i));
  }

  clib_assert(set_subset(&set1, &big_set) == true, "set1 should be a subset");
  clib_assert(set_subset(&set2, &big_set) == false,
              "set2 should not be a subset");
  clib_assert(set_subset(&set3, &big_set) == true,
              "set4 should not be a subset");

  arena_free(&arena);
}

static void test_intersection(void) {
  Arena arena = {0};

  const u64 test_numbers[] = {2, 3, 7, 8};
  const usize count = sizeof(test_numbers) / sizeof(test_numbers[0]);

  Set set1 = set_create(&arena);
  set_extend(&set1, count, test_numbers);

  const u64 test_numbers2[] = {2, 3, 7, 8, 9};
  const usize count2 = sizeof(test_numbers2) / sizeof(test_numbers2[0]);

  Set set2 = set_create(&arena);
  set_extend(&set2, count2, test_numbers2);

  Set inter = set_intersection(&set1, &set2, &arena);

  clib_assert(set_subset(&inter, &set2) == true, "inter should be a subset");

  arena_free(&arena);
}

static void test_difference(void) {
  Arena arena = {0};

  const u64 test_numbers[] = {1, 2, 3};
  const usize count = sizeof(test_numbers) / sizeof(test_numbers[0]);

  Set set1 = set_create(&arena);
  set_extend(&set1, count, test_numbers);

  const u64 test_numbers2[] = {3, 4, 5};
  const usize count2 = sizeof(test_numbers2) / sizeof(test_numbers2[0]);

  Set set2 = set_create(&arena);
  set_extend(&set2, count2, test_numbers2);

  Set diff = set_difference(&set1, &set2, &arena);

  clib_assert(set_contains(&diff, 1) == true, "should be different");
  clib_assert(set_contains(&diff, 2) == true, "should be different");
  clib_assert(set_contains(&diff, 3) == false, "should not be different");
  clib_assert(set_contains(&diff, 4) == false, "should not be symetrical");
  clib_assert(set_contains(&diff, 5) == false, "should not be symetrical");

  arena_free(&arena);
}

static bool filter_with_set(Set *intersection, Str s) {
  return set_contains(intersection, str_hash(s));
}

static void test_example_duplicates(void) {
  Arena arena = {0};

  Str strings[] = {
      STR("Apple"), STR("Banana"), STR("Apple"), STR("Cherry"), STR("Apple"),
  };
  VEC(Str) list = {0};
  vec_init_list(&list, &arena, ARRAY_SIZE(strings), strings);

  Set unique = set_create(&arena);
  Set duplicates = set_create(&arena);

  for (usize i = 0; i < list.len; i++) {
    u64 hash = str_hash(list.items[i]);
    if (set_add(&unique, hash) == false) {
      set_add(&duplicates, hash);
    }
  }

  VEC(Str) unique_strings = {0};
  vec_init(&unique_strings, &arena);
  vec_filter_ctx(&list, &unique_strings, !filter_with_set, &duplicates);

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

  const Str strings1[] = {
      STR("Apple"), STR("Banana"), STR("Cherry"), STR("Pomegrade"),
      STR("Grape"), STR("Orange"), STR("Pear"),
  };
  VEC(Str) first = {0};
  vec_init_list(&first, &arena, ARRAY_SIZE(strings1), strings1);

  Set s1 = set_create(&arena);
  for (size_t i = 0; i < first.len; i++) {
    set_add(&s1, str_hash(first.items[i]));
  }

  const Str strings2[] = {
      STR("Apple"),  STR("Strawberry"), STR("Blueberry"), STR("Banana"),
      STR("Cherry"), STR("Lemon"),      STR("Pear"),
  };
  const usize count2 = ARRAY_SIZE(strings2);
  Set s2 = set_create(&arena);
  for (size_t i = 0; i < count2; i++) {
    set_add(&s2, str_hash(strings2[i]));
  }

  Set intersection = set_intersection(&s1, &s2, &arena);
  VEC(Str) common = {0};
  vec_init(&common, &arena);
  vec_filter_ctx(&first, &common, filter_with_set, &intersection);

  clib_assert(common.len == 4,
              "Did not find the intersection correctly: %" USIZE_FMT,
              common.len);
  clib_assert(str_eq(common.items[0], STR("Apple")),
              "Did not filter out correctly");
  clib_assert(str_eq(common.items[1], STR("Banana")),
              "Did not filter out correctly");
  clib_assert(str_eq(common.items[2], STR("Cherry")),
              "Did not filter out correctly");
  clib_assert(str_eq(common.items[3], STR("Pear")),
              "Did not filter out correctly");

  arena_free(&arena);
}

int main(void) {
  test_set_insert();
  test_eq();
  test_subset();
  test_intersection();
  test_difference();

  test_example_duplicates();
  test_example_intersection();
}
