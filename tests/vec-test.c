#include "collections/vec.h"

#include "clib/asserts.h"
#include "datatypes/integers.h"

static void test_vec(void) {
  const usize n = 10;
  VEC(usize) list = {0};
  for (usize i = 0; i < n; ++i) {
    vec_push(&list, i + 1);
  }

  for (usize i = 0; i < list.len; ++i) {
    clib_assert(list.items[i] == i + 1,
                "Numbers were not pushed on the stack correctly");
  }
  vec_clear(&list);
  clib_assert(list.len == 0, "Clearing did not reset list.len");
  vec_free(&list);
}

static usize times_two(usize v) { return v * 2; }

static void test_map(void) { // NOLINT
  const usize n = 10;
  VEC(usize) list = {0};
  for (usize i = 0; i < n; ++i) {
    vec_push(&list, i);
  }

  // Map inplace
  vec_map(&list, &list, times_two);

  for (usize i = 0; i < list.len; ++i) {
    clib_assert(list.items[i] == i * 2, "Mapping did not multiply by two");
  }

  vec_free(&list);
}

static void test_sort(void) {
  const usize n = 10;
  VEC(usize) list = {0};
  for (usize i = 0; i < n; ++i) {
    vec_push(&list, n - i - 1);
  }

  vec_sort(&list, usize_compare_qsort(CMP_LESS));

  for (usize i = 0; i < list.len; ++i) {
    clib_assert(list.items[i] == i, "sorting did not work correctly");
  }

  vec_free(&list);
}

static void test_last(void) {
  VEC(i32) list = {0};
  clib_assert(vec_empty(&list), "List should be initialized empty");
  vec_push(&list, 10);
  vec_push(&list, 20);
  int last = vec_last(&list);
  clib_assert(last == 20, "Last is not the correct number");
  int first = vec_first(&list);
  clib_assert(first == 10, "First is not the correct number");
  vec_free(&list);
}

static void test_extend(void) {
  VEC(i32) list = {0};

  vec_extend(&list, 3, ((int[]){1, 2, 3}));
  clib_assert(list.items[0] == 1 && list.items[1] == 2 && list.items[2] == 3,
              "List did not extend correctly");
  vec_free(&list);
}

static void test_reserve(void) {
  VEC(i32) list = {0};
  vec_reserve(&list, 50);
  clib_assert(list.cap == 50, "Capacity was not increased");
  vec_free(&list);
}

static void test_reverse(void) {
  VEC(usize) list = {0};
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    vec_push(&list, i + 1);
  }
  vec_reverse(&list);
  for (usize i = 0; i < n; i++) {
    clib_assert(list.items[i] == n - i, "List was not reversed correctly");
  }

  vec_free(&list);
}

static bool is_odd(usize i) { return i % 2 == 0; }

static void test_filter(void) {
  VEC(usize) list = {0};
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    vec_push(&list, i);
  }

  vec_filter(&list, &list, is_odd);

  clib_assert(list.items[1] == 2, "list was not filtered correctly");
  clib_assert(list.items[2] == 4, "list was not filtered correctly");
  clib_assert(list.items[3] == 6, "list was not filtered correctly");

  vec_free(&list);
}

static void test_copy(void) {
  VEC(usize) l1 = {0};
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    vec_push(&l1, i + 1);
  }

  VEC(usize) l2 = {0};
  vec_copy(&l1, &l2);
  clib_assert(l1.len == l2.len, "list was not copied correctly");
  for (usize i = 0; i < l2.len; i++) {
    clib_assert(l1.items[i] == l2.items[i], "list was not copied correctly");
  }

  vec_free(&l1);
  vec_free(&l2);
}

static void test_pop(void) {
  VEC(usize) list = {0};
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    vec_push(&list, i + 1);
  }

  for (usize i = list.len; 0 < i; i--) {
    clib_assert(vec_pop(&list) == i, "Poping not correctly");
  }

  clib_assert(vec_empty(&list) == true, "After all that not empty");

  vec_free(&list);
}

int main(void) {
  test_vec();
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
