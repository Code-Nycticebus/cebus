#include "clib/asserts.h"
#include "containers/ht.h"

#include "datatypes/str.h"

u64 hash(Str s) {
  u64 hash = 0x5432;
  for (usize i = 0; i < s.len; i++) {
    hash &= 0x7FFFFFFFFFFFFFF;
    hash = (hash << 5) + hash + s.data[i];
  }
  return hash;
}

static void test_insert(void) {
  HashTable ht = {0};

  ht_insert(&ht, hash(STR("Hello")), (HashValue){.u64 = 420});
  ht_insert(&ht, hash(STR("Hello2")), (HashValue){.u64 = 69});

  clib_assert(ht_get(&ht, hash(STR("Hello"))).u64 == 420,
              "ht should get the value correnctly");
  clib_assert(ht_get(&ht, hash(STR("Hello2"))).u64 == 69,
              "ht should get the value correnctly");

  HashValue value = {0};
  clib_assert(ht_try_get(&ht, hash(STR("NOT FOUND!")), &value) == false,
              "Did find something");

  ht_free(&ht);
}

int main(void) { test_insert(); }
