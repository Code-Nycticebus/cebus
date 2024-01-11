#include "clib/asserts.h"
#include "collections/ht.h"

#include "types/integers.h"
#include "types/str.h"

#define TEST_HT_DEFAULT_SIZE 10

static void test_insert(void) {
  Arena arena = {0};
  HashTable ht = ht_create(&arena, TEST_HT_DEFAULT_SIZE);

  ht_insert(&ht, str_hash(STR("Hello")), (HashValue){.u64 = 420});  // NOLINT
  ht_insert(&ht, str_hash(STR("Hello2")), (HashValue){.i64 = -69}); // NOLINT

  clib_assert(ht_get(&ht, str_hash(STR("Hello")))->value.u64 == 420,
              "ht should get the value correnctly");
  clib_assert(ht_get(&ht, str_hash(STR("Hello2")))->value.i64 == -69,
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

  clib_assert(ht_get(&ht, usize_hash(10))->value.u64 == 40,
              "Hashing was wrong");
  clib_assert(ht_get(&ht, usize_hash(20))->value.u64 == 80,
              "Hashing was wrong");
  clib_assert(ht_get(&ht, usize_hash(30))->value.u64 == 120,
              "Hashing was wrong");

  arena_free(&arena);
}

int main(void) {
  test_insert();
  test_ht();
}
