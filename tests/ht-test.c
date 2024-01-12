#include "collections/ht.h"
#include "collections/vec.h"
#include "core/asserts.h"
#include "core/logging.h"

#include "core/defines.h"
#include "types/integers.h"
#include "types/str.h"

#define TEST_HT_DEFAULT_SIZE 10

static void test_insert(void) {
  Arena arena = {0};
  HashTable ht = ht_create(&arena, TEST_HT_DEFAULT_SIZE);

  ht_insert(&ht, str_hash(STR("Hello")), (HashValue){.u64 = 420});  // NOLINT
  ht_insert(&ht, str_hash(STR("Hello2")), (HashValue){.i64 = -69}); // NOLINT

  clib_assert(ht_get(&ht, str_hash(STR("Hello")))->u64 == 420,
              "ht should get the value correnctly");
  clib_assert(ht_get(&ht, str_hash(STR("Hello2")))->i64 == -69,
              "ht should get the value correnctly");

  arena_free(&arena);
}

static void test_ht(void) {
  const usize test_count = 10000;
  Arena arena = {0};
  HashTable ht = ht_create(&arena, test_count);

  for (size_t i = 0; i < test_count; i++) {
    ht_insert(&ht, usize_hash(i), (HashValue){.u64 = i * 4});
  }
  clib_assert(ht.count == test_count, "Hash table should be at this size");

  clib_assert(ht_get(&ht, usize_hash(10))->u64 == 40, "Hashing was wrong");
  clib_assert(ht_get(&ht, usize_hash(20))->u64 == 80, "Hashing was wrong");
  clib_assert(ht_get(&ht, usize_hash(30))->u64 == 120, "Hashing was wrong");

  arena_free(&arena);
}

static CmpOrdering sort_by_occurence(const void *ctx, const void *a,
                                     const void *b) {
  return u64_compare_gt(ht_get(ctx, str_hash(*(const Str *)a))->u64,
                        ht_get(ctx, str_hash(*(const Str *)b))->u64);
}

static void test_example(void) {
  Arena arena = {0};
  Str strings[] = {
      STR("Apple"),      STR("Banana"), STR("Apple"),
      STR("Strawberry"), STR("Apple"),  STR("Banana"),
  };
  VEC(Str) list = {0};
  vec_init_list(&list, &arena, ARRAY_SIZE(strings), strings);

  VEC(Str) text = {0};
  vec_init(&text, 4, &arena);

  HashTable ht = ht_create(&arena, 10);
  for (usize i = 0; i < list.len; i++) {
    u64 hash = str_hash(list.items[i]);
    HashValue *value = ht_get(&ht, hash);
    if (value == NULL) {
      vec_push(&text, list.items[i]);
      ht_insert(&ht, hash, (HashValue){.u64 = 1});
    } else {
      value->u64++;
    }
  }

  vec_sort_ctx(&text, &text, sort_by_occurence, &ht);

  clib_assert(str_eq(text.items[0], STR("Apple")),
              "Apple does occure the most");
  clib_assert(str_eq(text.items[1], STR("Banana")),
              "Banana does occure the second most");
  clib_assert(str_eq(text.items[2], STR("Strawberry")),
              "Strawberry does occure the least");

  arena_free(&arena);
}

int main(void) {
  test_insert();
  test_ht();
  test_example();
}
