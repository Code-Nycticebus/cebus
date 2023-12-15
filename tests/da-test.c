#include "clib/da.h"

#include <assert.h>
#include <stddef.h>

typedef struct {
  DA(size_t);
} ListSize;

void test_da(void) {
  const size_t n = 10;
  ListSize list = {0};
  da_init(&list, 2);
  for (size_t i = 0; i < n; ++i) {
    da_push(&list, i + 1);
  }

  for (size_t i = 0; i < list.len; ++i) {
    assert(list.items[i] == i + 1);
  }

  da_free(&list);
}

size_t map(size_t v) { return v * 2; }

void test_map(void) {
  const size_t n = 10;
  ListSize list = {0};
  da_init(&list, 2);
  for (size_t i = 0; i < n; ++i) {
    da_push(&list, i);
  }

  da_map(&list, map);

  for (size_t i = 0; i < list.len; ++i) {
    assert(list.items[i] == i * 2);
  }

  da_free(&list);
}

int sort(const void *a, const void *b) { return *(size_t *)a - *(size_t *)b; }

void test_sort(void) {
  const size_t n = 10;
  ListSize list = {0};
  da_init(&list, 2);
  for (size_t i = 0; i < n; ++i) {
    da_push(&list, n - i - 1);
  }

  da_sort(&list, sort);

  for (size_t i = 0; i < list.len; ++i) {
    assert(list.items[i] == i);
  }

  da_free(&list);
}

int main(void) {
  test_da();
  test_map();
}
