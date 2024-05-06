#include "clib/core/sorting.h"

#include "clib/core/debug.h"

static CmpOrdering cmp(const void *a, const void *b) {
  return *(const i32 *)a - *(const i32 *)b;
}

static CmpOrdering cmp_smallest(const void *ctx, const void *a, const void *b) {
  return *(const i32 *)ctx == *(const i32 *)a
             ? CMP_LESS
             : *(const i32 *)a - *(const i32 *)b;
}

// Example usage
int main(void) {
  const i32 array[] = {10, 9, 8, 7, 6, 4, 3, 5, 1};
  usize n = sizeof(array) / sizeof(array[0]);

  i32 sorted[n];
  quicksort(array, sorted, sizeof(i32), n, cmp);
  clib_assert(sorted[0] == 1, "Array was not sorted correctly");
  clib_assert(sorted[1] == 3, "Array was not sorted correctly");
  clib_assert(sorted[2] == 4, "Array was not sorted correctly");
  clib_assert(sorted[3] == 5, "Array was not sorted correctly");

  i32 sorted_smallest[n];
  i32 smallest = 4;
  quicksort_ctx(array, sorted_smallest, sizeof(i32), n, cmp_smallest,
                &smallest);
  clib_assert(sorted_smallest[0] == 4, "Array was not sorted correctly");
  clib_assert(sorted_smallest[1] == 1, "Array was not sorted correctly");
  clib_assert(sorted_smallest[2] == 3, "Array was not sorted correctly");
  clib_assert(sorted_smallest[3] == 5, "Array was not sorted correctly");

  return 0;
}
