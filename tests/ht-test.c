#include "clib/asserts.h"
#include "collections/ht.h"

#include "datatypes/integers.h"
#include "datatypes/str.h"

static void test_insert(void) {
  Arena arena = {0};
  HashTable ht = ht_create(&arena, 10);

  ht_insert(&ht, str_hash(STR("Hello")), (HashValue){.u64 = 420});
  ht_insert(&ht, str_hash(STR("Hello2")), (HashValue){.i64 = -69});

  clib_assert(ht_get(&ht, str_hash(STR("Hello")))->value.u64 == 420,
              "ht should get the value correnctly");
  clib_assert(ht_get(&ht, str_hash(STR("Hello2")))->value.i64 == -69,
              "ht should get the value correnctly");

  arena_free(&arena);
}

static void test_ht(void) {
  Arena arena = {0};
  HashTable ht = ht_create(&arena, 10000);

  for (size_t i = 0; i < 10000; i++) {
    ht_insert(&ht, usize_hash(i), (HashValue){.u64 = i * 4});
  }
  clib_assert(ht.count == 10000, "Hash table should be at this size");

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
