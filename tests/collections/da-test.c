#include "cebus/collection/da.h"

#include "cebus/core/arena.h"
#include "cebus/core/debug.h"
#include "cebus/type/integer.h"

static void test_vec(void) {
  Arena arena = {0};
  const usize n = 10;
  DA(usize) list = {0};
  da_init(&list, &arena);
  for (usize i = 0; i < n; ++i) {
    da_push(&list, i + 1);
  }

  for (usize i = 0; i < list.len; ++i) {
    cebus_assert(list.items[i] == i + 1, "Numbers were not pushed on the stack correctly");
  }
  da_clear(&list);
  cebus_assert(list.len == 0, "Clearing did not reset list.len");

  arena_free(&arena);
}

static void test_da_init(void) {
  Arena arena = {0};
  DA(usize) list = {0};
  da_init_static(&list, &arena, (usize[]){1, 2, 3, 4, 5});

  cebus_assert(list.len == 5, "Did not set len correctly");
  cebus_assert(list.cap == 5, "Did not set cap correctly");

  cebus_assert(list.items[0] == 1, "Did not init correctly");
  cebus_assert(list.items[2] == 3, "Did not init correctly");
  cebus_assert(list.items[4] == 5, "Did not init correctly");

  arena_free(&arena);
}

static usize times_two(usize v) { return v * 2; }

static void test_map(void) { // NOLINT
  Arena arena = {0};
  const usize n = 10;
  DA(usize) list = da_new(&arena);
  for (usize i = 0; i < n; ++i) {
    da_push(&list, i);
  }

  // Map inplace
  da_map(&list, &list, times_two);

  for (usize i = 0; i < list.len; ++i) {
    cebus_assert(list.items[i] == i * 2, "Mapping did not multiply by two");
  }
  arena_free(&arena);
}

static void test_sort(void) {
  Arena arena = {0};
  const usize n = 10;
  DA(usize) list = {0};
  da_init(&list, &arena);
  for (usize i = 0; i < n; ++i) {
    da_push(&list, n - i - 1);
  }

  da_sort(&list, usize_compare_qsort(CMP_LESS));

  for (usize i = 0; i < list.len; ++i) {
    cebus_assert(list.items[i] == i, "sorting did not work correctly");
  }

  arena_free(&arena);
}

static void test_last(void) {
  Arena arena = {0};

  DA(i32) list = {0};
  da_init(&list, &arena);
  cebus_assert(da_empty(&list), "List should be initialized empty");
  da_push(&list, 10);
  da_push(&list, 20);
  int last = da_last(&list);
  cebus_assert(last == 20, "Last is not the correct number");
  int first = da_first(&list);
  cebus_assert(first == 10, "First is not the correct number");

  arena_free(&arena);
}

static void test_extend(void) {
  Arena arena = {0};
  DA(i32) list = {0};
  da_init(&list, &arena);

  da_extend(&list, 3, ((int[]){1, 2, 3}));
  cebus_assert(list.len == 3, "List did not extend correctly");
  cebus_assert(list.items[0] == 1 && list.items[1] == 2 && list.items[2] == 3,
               "List did not extend correctly");

  i32 array[] = {1, 2, 3};
  da_extend(&list, 3, array);
  cebus_assert(list.items[3] == 1 && list.items[4] == 2 && list.items[5] == 3,
               "List did not extend correctly");

  da_extend_da(&list, &list);
  cebus_assert(list.items[6] == 1 && list.items[7] == 2 && list.items[8] == 3,
               "List did not extend correctly");

  arena_free(&arena);
}

static void test_reserve(void) {
  Arena arena = {0};
  DA(i32) list = {0};
  da_init(&list, &arena);
  da_resize(&list, 20);
  cebus_assert(list.cap == 20, "Capacity was not increased: %" USIZE_FMT, list.cap);
  da_reserve(&list, 50);
  cebus_assert(list.cap == 80, "Capacity was not increased: %" USIZE_FMT, list.cap);
  arena_free(&arena);
}

static void test_reverse(void) {
  Arena arena = {0};
  DA(usize) list = {0};
  da_init(&list, &arena);
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    da_push(&list, i + 1);
  }
  da_reverse(&list);
  for (usize i = 0; i < n; i++) {
    cebus_assert(list.items[i] == n - i, "List was not reversed correctly");
  }

  arena_free(&arena);
}

static bool is_odd(usize i) { return i % 2 == 0; }

static void test_filter(void) {
  Arena arena = {0};
  DA(usize) list = {0};
  da_init(&list, &arena);
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    da_push(&list, i);
  }

  da_filter(&list, &list, is_odd);

  cebus_assert(list.items[1] == 2, "list was not filtered correctly");
  cebus_assert(list.items[2] == 4, "list was not filtered correctly");
  cebus_assert(list.items[3] == 6, "list was not filtered correctly");

  arena_free(&arena);
}

typedef struct {
  usize a;
} Ctx;
static bool filter_with_context(Ctx *ctx, usize a) { return ctx->a < a; }

static void test_filter_ctx(void) {
  Arena arena = {0};
  DA(usize) list = {0};
  da_init(&list, &arena);
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    da_push(&list, i);
  }

  Ctx ctx = {.a = 4};
  da_filter_ctx(&list, &list, filter_with_context, &ctx);

  cebus_assert(list.items[0] == 5, "list was not filtered correctly");
  cebus_assert(list.items[1] == 6, "list was not filtered correctly");
  cebus_assert(list.items[2] == 7, "list was not filtered correctly");

  arena_free(&arena);
}

static void test_copy(void) {
  Arena arena = {0};
  DA(usize) l1 = {0};
  da_init(&l1, &arena);
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    da_push(&l1, i + 1);
  }

  DA(usize) l2 = {0};
  da_init(&l2, &arena);
  da_copy(&l1, &l2);
  cebus_assert(l1.len == l2.len, "list was not copied correctly");
  for (usize i = 0; i < l2.len; i++) {
    cebus_assert(l1.items[i] == l2.items[i], "list was not copied correctly");
  }

  arena_free(&arena);
}

static void test_pop(void) {
  Arena arena = {0};
  DA(usize) list = {0};
  da_init(&list, &arena);
  const usize n = 10;
  for (usize i = 0; i < n; i++) {
    da_push(&list, i + 1);
  }

  for (usize i = list.len; 0 < i; i--) {
    cebus_assert(da_pop(&list) == i, "Poping not correctly");
  }

  cebus_assert(da_empty(&list) == true, "After all that not empty");

  arena_free(&arena);
}

static void test_insert(void) {
  Arena arena = {0};
  DA(usize) list = {0};
  da_init(&list, &arena);

  da_push(&list, 1);
  da_push(&list, 4);

  da_insert(&list, 2, 1);
  da_insert(&list, 3, 2);

  cebus_assert(list.len == 4, "");
  cebus_assert(da_get(&list, 0) == 1, "");
  cebus_assert(da_get(&list, 1) == 2, "");
  cebus_assert(da_get(&list, 2) == 3, "%" USIZE_FMT, da_get(&list, 2));
  cebus_assert(da_get(&list, 3) == 4, "%" USIZE_FMT, da_get(&list, 3));

  arena_free(&arena);
}

static void test_remove(void) {
  Arena arena = {0};
  DA(usize) list = {0};
  da_init(&list, &arena);

  da_push(&list, 1);
  da_push(&list, 2);
  da_push(&list, 3);
  da_push(&list, 4);

  da_remove(&list, 1);
  da_remove(&list, 1);

  cebus_assert(list.len == 2, "");
  cebus_assert(da_get(&list, 0) == 1, "");
  cebus_assert(da_get(&list, 1) == 4, "");

  arena_free(&arena);
}

static void test_for_each(void) {
  Arena arena = {0};
  DA(usize) list = {0};

  usize el[] = {1, 2, 3, 4};
  da_init_static(&list, &arena, el);

  usize i = 0;
  da_for_each(usize *, element, &list) {
    cebus_assert(*element == el[i], "");
    i++;
  }
  cebus_assert(i == list.len, "");
}

int main(void) {
  test_vec();
  test_da_init();
  test_map();
  test_extend();
  test_reserve();
  test_reverse();
  test_sort();
  test_last();
  test_filter();
  test_filter_ctx();
  test_copy();
  test_pop();
  test_insert();
  test_remove();
  test_for_each();
}
