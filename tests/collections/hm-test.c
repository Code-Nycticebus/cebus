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

int main(void) {
  test_insert();
  test_hm();
  test_hm_ptr();
}
