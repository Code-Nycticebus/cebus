#include "clib/da.h"

#include <assert.h>
#include <stddef.h>

void test_da(void) {
  const size_t n = 10;
  DA(size_t) list = {0};
  da_init(&list, 2);
  for (size_t i = 0; i < n; ++i) {
    da_push(&list, i + 1);
  }

  for (size_t i = 0; i < list.len; ++i) {
    assert(list.items[i] == i + 1);
  }

  da_free(&list);
}

size_t times_two(size_t idx, size_t v) {
  (void)idx;
  return v * 2;
}

void test_map(void) {
  const size_t n = 10;
  DA(size_t) list = {0};
  da_init(&list, 2);
  for (size_t i = 0; i < n; ++i) {
    da_push(&list, i);
  }

  da_map(&list, times_two);

  for (size_t i = 0; i < list.len; ++i) {
    assert(list.items[i] == i * 2);
  }

  da_free(&list);
}

int main(void) {
  test_da();
  test_map();
}
