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
  da_clear(&list);
  assert(list.len == 0);
  da_free(&list);
}

size_t times_two(size_t v) { return v * 2; }

void test_map(void) { // NOLINT
  const size_t n = 10;
  DA(size_t) list = {0};
  da_init(&list, 2);
  for (size_t i = 0; i < n; ++i) {
    da_push(&list, i);
  }

  // Map inplace
  da_map(&list, &list, times_two);

  for (size_t i = 0; i < list.len; ++i) {
    assert(list.items[i] == i * 2);
  }

  da_free(&list);
}

int sort(const void *a, const void *b) { return *(size_t *)a - *(size_t *)b; }

void test_sort(void) {
  const size_t n = 10;
  DA(size_t) list = {0};
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

void test_last(void) {
  DA(int) list = {0};
  da_init(&list, 1);
  assert(da_empty(&list));
  da_push(&list, 10);
  int last = da_last(&list);
  assert(last == 10);
}

void test_extend(void) {
  DA(int) list = {0};

  da_extend(&list, 3, ((int[]){1, 2, 3}));
  assert(list.items[0] == 1);
  assert(list.items[1] == 2);
  assert(list.items[2] == 3);
  da_free(&list);
}

void test_reserve(void) {
  DA(int) list = {0};
  da_reserve(&list, 5);
  assert(list.cap == 5);
  da_free(&list);
}

void test_reverse(void) { // NOLINT
  DA(size_t) list = {0};
  const size_t n = 10;
  for (size_t i = 0; i < n; i++) {
    da_push(&list, i + 1);
  }
  da_reverse(&list);
  for (size_t i = 0; i < n; i++) {
    assert(list.items[i] == n - i);
  }

  da_free(&list);
}

int is_odd(int i) { return i % 2 == 0; }

void test_filter(void) { // NOLINT
  DA(int) list = {0};
  const size_t n = 10;
  for (size_t i = 0; i < n; i++) {
    da_push(&list, i);
  }

  da_filter(&list, is_odd);

  assert(list.items[1] == 2);
  assert(list.items[2] == 4);
  assert(list.items[3] == 6);

  da_free(&list);
}

void test_copy(void) { // NOLINT
  DA(size_t) l1 = {0};
  const size_t n = 10;
  for (size_t i = 0; i < n; i++) {
    da_push(&l1, i + 1);
  }

  DA(size_t) l2 = {0};
  da_copy(&l1, &l2);
  assert(l1.len == l2.len);
  for (size_t i = 0; i < l2.len; i++) {
    assert(l1.items[i] == l2.items[i]);
  }

  da_free(&l1);
  da_free(&l2);
}

int main(void) {
  test_da();
  test_map();
  test_extend();
  test_reserve();
  test_reverse();
  test_filter();
  test_copy();
}
