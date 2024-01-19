#include "collections/hm.h"

#include "collections/vec.h"
#include "core/asserts.h"
#include "core/defines.h"
#include "core/logging.h"
#include "types/integers.h"
#include "types/str.h"

static void test_insert(void) {
  Arena arena = {0};
  HashMap hm = hm_create(&arena);

  hm_insert(&hm, str_hash(STR("Hello")), (HashValue){.as.u64 = 420});  // NOLINT
  hm_insert(&hm, str_hash(STR("Hello2")), (HashValue){.as.i64 = -69}); // NOLINT

  clib_assert(hm_get(&hm, str_hash(STR("Hello")))->as.u64 == 420,
              "ht should get the value correnctly");
  clib_assert(hm_get(&hm, str_hash(STR("Hello2")))->as.i64 == -69,
              "ht should get the value correnctly");

  arena_free(&arena);
}

static void test_hm(void) {
  const usize test_count = 10000;
  Arena arena = {0};
  HashMap hm = hm_create(&arena);
  hm_reserve(&hm, test_count * 2);

  for (size_t i = 0; i < test_count; i++) {
    hm_insert(&hm, usize_hash(i), (HashValue){.as.u64 = i * 4});
  }
  clib_assert(hm.count == test_count, "Hash table should be at this size");

  clib_assert(hm_get(&hm, usize_hash(10))->as.u64 == 40, "Hashing was wrong");
  clib_assert(hm_get(&hm, usize_hash(20))->as.u64 == 80, "Hashing was wrong");
  clib_assert(hm_get(&hm, usize_hash(30))->as.u64 == 120, "Hashing was wrong");

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
  VEC(Str) list = {0};
  vec_init_list(&list, &arena, ARRAY_SIZE(strings), strings);

  VEC(Str) text = {0};
  vec_init(&text, &arena);

  HashMap hm = hm_create(&arena);
  for (usize i = 0; i < list.len; i++) {
    u64 hash = str_hash(list.items[i]);
    HashValue *value = hm_get(&hm, hash);
    if (value == NULL) {
      vec_push(&text, list.items[i]);
      hm_insert(&hm, hash, (HashValue){.as.u64 = 1});
    } else {
      value->as.u64++;
    }
  }

  vec_sort_ctx(&text, &text, sort_by_occurence, &hm);

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
  test_hm();
  test_example();
}
