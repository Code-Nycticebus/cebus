#include "core/asserts.h"
#include "core/sorting.h"

static CmpOrdering cmp(const void *a, const void *b) {
  return *(const i32 *)a - *(const i32 *)b;
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

  return 0;
}
