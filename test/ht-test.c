#include "clib/asserts.h"
#include "containers/ht.h"

#include "datatypes/integers.h"
#include "datatypes/str.h"

u64 hash(Str s) {
  u64 hash = 0x5432;
  for (usize i = 0; i < s.len; i++) {
    hash &= ~(0x1fUL << 58);
    hash = (hash << 5) + hash + s.data[i];
  }
  return hash;
}

static void test_insert(void) {
  Arena arena = {0};
  HashTable ht = ht_create(&arena, 10);

  ht_insert(&ht, hash(STR("Hello")), (HashValue){.u64 = 420});
  ht_insert(&ht, hash(STR("Hello2")), (HashValue){.u64 = 69});

  clib_assert(ht_get(&ht, hash(STR("Hello"))).u64 == 420,
              "ht should get the value correnctly");
  clib_assert(ht_get(&ht, hash(STR("Hello2"))).u64 == 69,
              "ht should get the value correnctly");

  arena_free(&arena);
}

static void test_ht(void) {
  Arena arena = {0};
  HashTable ht = ht_create(&arena, 10000);

  for (size_t i = 0; i < 10000; i++) {
    if (i % 1000 == 0)
      printf("%zu\n", i);
    ht_insert(&ht, usize_hash(i), (HashValue){.u64 = i * 4});
  }
  printf("elements: %zu -> %zu kb\n", ht.count,
         ht.count * sizeof(HashNode) / 1000);

  printf("Fast math: 100*4 == %lu\n", ht_get(&ht, usize_hash(100)).u64);

  arena_free(&arena);
}

int main(void) {
  test_insert();
  test_ht();
}
