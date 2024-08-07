#include "cebus/collection/hashmap.h"

#include "cebus/collection/da.h"
#include "cebus/core/debug.h"
#include "cebus/type/integer.h"
#include "cebus/type/string.h"

#include <stdio.h>

static void test_insert(void) {
  Arena arena = {0};
  HashMap *hm = hm_create(&arena);

  hm_insert_i64(hm, str_hash(STR("Hello")), 420);  // NOLINT
  hm_insert_i64(hm, str_hash(STR("Hello2")), -69); // NOLINT

  cebus_assert(*hm_get_i64(hm, str_hash(STR("Hello"))) == 420, "ht should get the value correctly");
  cebus_assert(*hm_get_i64(hm, str_hash(STR("Hello2"))) == -69,
               "ht should get the value correctly");

  arena_free(&arena);
}

static void test_hm(void) {
  const usize test_count = 10000;
  Arena arena = {0};

  HashMap *hm = hm_with_size(&arena, test_count * 2);
  for (usize i = 0; i < test_count; i++) {
    hm_insert_usize(hm, i, i * 4);
  }

  for (size_t i = 0; i < test_count; i++) {
    cebus_assert(*hm_get_usize(hm, i) == i * 4, "Hashing was wrong");
  }

  arena_free(&arena);
}

static void test_hm_ptr(void) {
  Arena arena = {0};
  HashMap *hm = hm_create(&arena);

  const int a = 32;
  hm_insert_ptr(hm, (u64)a, &a);

  const int b = 34;
  hm_insert_ptr(hm, (u64)b, &b);

  cebus_assert(hm_get_ptr(hm, (u64)a) == &a, "");
  cebus_assert(hm_get_ptr(hm, (u64)b) == &b, "");
  cebus_assert(hm_get_ptr(hm, (u64)15) == NULL, "");

  arena_free(&arena);
}

static CmpOrdering sort_by_occurence(const void *ctx, const void *a, const void *b) {
  return u64_compare_gt(*hm_get_u64(ctx, str_hash(*(const Str *)a)),
                        *hm_get_u64(ctx, str_hash(*(const Str *)b)));
}

static void test_example(void) {
  Arena arena = {0};
  const Str strings[6] = {
      STR("Apple"), STR("Banana"), STR("Apple"), STR("Strawberry"), STR("Apple"), STR("Banana"),
  };

  DA(Str) keys = {0};
  da_init(&keys, &arena);

  HashMap *hm = hm_create(&arena);
  for (usize i = 0; i < ARRAY_LEN(strings); i++) {
    u64 hash = str_hash(strings[i]);
    u64 *value = hm_get_u64_mut(hm, hash);
    if (value == NULL) {
      da_push(&keys, strings[i]);
      hm_insert_u64(hm, hash, 1);
    } else {
      (*value)++;
    }
  }

  da_sort_ctx(&keys, &keys, sort_by_occurence, hm);

  cebus_assert(str_eq(keys.items[0], STR("Apple")), "Apple does occure the most");
  cebus_assert(*hm_get_u64(hm, str_hash(keys.items[0])) == 3, "Apple should occure 3 times");

  cebus_assert(str_eq(keys.items[1], STR("Banana")), "Banana does occure the second most");
  cebus_assert(*hm_get_u64(hm, str_hash(keys.items[1])) == 2, "Banana should occure 2 times");

  cebus_assert(str_eq(keys.items[2], STR("Strawberry")), "Strawberry does occure the least");
  cebus_assert(*hm_get_u64(hm, str_hash(keys.items[2])) == 1, "Strawberry should occure 1 time");

  arena_free(&arena);
}

int main(void) {
  test_insert();
  test_hm();
  test_hm_ptr();
  test_example();
}
