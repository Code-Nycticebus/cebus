#include "clib/da.h"

#include "clib/asserts.h"
#include "datatypes/integers.h"

static void test_da(void) {
  const usize n = 10;
  DA(usize) list = {0};
  da_init(&list, 2);
  for (usize i = 0; i < n; ++i) {
    da_push(&list, i + 1);
  }

  for (usize i = 0; i < list.len; ++i) {
    clib_assert(list.items[i] == i + 1,
                "Numbers were not pushed on the stack correctly");
  }
  da_clear(&list);
  clib_assert(list.len == 0, "Clearing did not reset list.len");
  da_free(&list);
}

usize times_two(usize v) { return v * 2; }

static void test_map(void) { // NOLINT
  const usize n = 10;
  DA(usize) list = {0};
  da_init(&list, 2);
  for (usize i = 0; i < n; ++i) {
    da_push(&list, i);
  }

  // Map inplace
  da_map(&list, &list, times_two);

  for (usize i = 0; i < list.len; ++i) {
    clib_assert(list.items[i] == i * 2, "Mapping did not multiply by two");
  }

  da_free(&list);
}

static void test_sort(void) {
  const usize n = 10;
  DA(usize) list = {0};
  for (usize i = 0; i < n; ++i) {
    da_push(&list, n - i - 1);
  }

  da_sort(&list, usize_compare_qsort(CMP_LESS));

  for (usize i = 0; i < list.len; ++i) {
    clib_assert(list.items[i] == i, "sorting did not work correctly");
  }

  da_free(&list);
}

static void test_last(void) {
  DA(i32) list = {0};
  da_init(&list, 1);
  clib_assert(da_empty(&list), "List should be initialized empty");
  da_push(&list, 10);
  int last = da_last(&list);
  clib_assert(last == 10, "Last is not the correct number");
  da_free(&list);
}

static void test_extend(void) {
  DA(i32) list = {0};

  da_extend(&list, 3, ((int[]){1, 2, 3}));
  clib_assert(list.items[0] == 1 && list.items[1] == 2 && list.items[2] == 3,
              "List did not extend correctly");
  da_free(&list);
}

static void test_reserve(void) {
  DA(i32) list = {0};
  da_reserve(&list, 5);
  clib_assert(list.cap == 5, "Capacity was not increased");
  da_free(&list);
}

static void test_reverse(void) {
  DA(usize) list = {0};
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    da_push(&list, i + 1);
  }
  da_reverse(&list);
  for (usize i = 0; i < n; i++) {
    clib_assert(list.items[i] == n - i, "List was not reversed correctly");
  }

  da_free(&list);
}

bool is_odd(i32 i) { return i % 2 == 0; }

static void test_filter(void) {
  DA(i32) list = {0};
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    da_push(&list, i);
  }

  da_filter(&list, is_odd);

  clib_assert(list.items[1] == 2, "list was not filtered correctly");
  clib_assert(list.items[2] == 4, "list was not filtered correctly");
  clib_assert(list.items[3] == 6, "list was not filtered correctly");

  da_free(&list);
}

static void test_copy(void) {
  DA(usize) l1 = {0};
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    da_push(&l1, i + 1);
  }

  DA(usize) l2 = {0};
  da_copy(&l1, &l2);
  clib_assert(l1.len == l2.len, "list was not copied correctly");
  for (usize i = 0; i < l2.len; i++) {
    clib_assert(l1.items[i] == l2.items[i], "list was not copied correctly");
  }

  da_free(&l1);
  da_free(&l2);
}

static void test_pop(void) {
  DA(usize) list = {0};
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    da_push(&list, i + 1);
  }

  for (usize i = list.len; 0 < i; i--) {
    clib_assert(da_pop(&list) == i, "Poping not correctly");
  }

  clib_assert(da_empty(&list) == true, "After all that not empty");

  da_free(&list);
}

int main(void) {
  test_da();
  test_map();
  test_extend();
  test_reserve();
  test_reverse();
  test_sort();
  test_last();
  test_filter();
  test_copy();
  test_pop();
}
