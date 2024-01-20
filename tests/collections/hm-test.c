#include "collections/hm.h"

#include "collections/vec.h"
#include "core/asserts.h"
#include "types/integers.h"
#include "types/str.h"

static void test_insert(void) {
  Arena arena = {0};
  HashMap hm = hm_create(&arena);

  hm_insert(&hm, str_hash(STR("Hello")), HashValue(u64, 420));  // NOLINT
  hm_insert(&hm, str_hash(STR("Hello2")), HashValue(i64, -69)); // NOLINT

  clib_assert(hm_get(&hm, str_hash(STR("Hello")))->as.u64 == 420,
              "ht should get the value correnctly");
  clib_assert(hm_get(&hm, str_hash(STR("Hello2")))->as.i64 == -69,
              "ht should get the value correnctly");

  arena_free(&arena);
}

static void test_hm(void) {
  const usize test_count = 10000;
  Arena arena = {0};

  HashMap hm = hm_with_size(&arena, test_count * 2);
  for (size_t i = 0; i < test_count; i++) {
    hm_insert(&hm, i, HashValue(u64, i * 4));
  }
  clib_assert(hm.count == test_count, "Hash table should be at this size");

  for (size_t i = 0; i < test_count; i++) {
    clib_assert(hm_get(&hm, i)->as.u64 == i * 4, "Hashing was wrong");
  }

  arena_free(&arena);
}

static CmpOrdering sort_by_occurence(const void *ctx, const void *a,
                                     const void *b) {
  return u64_compare_gt(hm_get(ctx, str_hash(*(const Str *)a))->as.u64,
                        hm_get(ctx, str_hash(*(const Str *)b))->as.u64);
}

static void test_example(void) {
  Arena arena = {0};
  Str strings[] = {
      STR("Apple"),      STR("Banana"), STR("Apple"),
      STR("Strawberry"), STR("Apple"),  STR("Banana"),
  };

  VEC(Str) keys = {0};
  vec_init(&keys, &arena);

  HashMap hm = hm_create(&arena);
  for (usize i = 0; i < ARRAY_SIZE(strings); i++) {
    u64 hash = str_hash(strings[i]);
    HashValue *value = hm_get(&hm, hash);
    if (value == NULL) {
      vec_push(&keys, strings[i]);
      hm_insert(&hm, hash, HashValue(u64, 1));
    } else {
      value->as.u64++;
    }
  }

  vec_sort_ctx(&keys, &keys, sort_by_occurence, &hm);

  clib_assert(str_eq(keys.items[0], STR("Apple")),
              "Apple does occure the most");
  clib_assert(hm_get(&hm, str_hash(keys.items[0]))->as.u64 == 3,
              "Apple should occure 3 times");

  clib_assert(str_eq(keys.items[1], STR("Banana")),
              "Banana does occure the second most");
  clib_assert(hm_get(&hm, str_hash(keys.items[1]))->as.u64 == 2,
              "Banana should occure 2 times");

  clib_assert(str_eq(keys.items[2], STR("Strawberry")),
              "Strawberry does occure the least");
  clib_assert(hm_get(&hm, str_hash(keys.items[2]))->as.u64 == 1,
              "Strawberry should occure 1 time");

  arena_free(&arena);
}

int main(void) {
  test_insert();
  test_hm();
  test_example();
}
