#include "core/asserts.h"
#include "core/sorting.h"

static CmpOrdering cmp(const void *a, const void *b) {
  return *(const i32 *)a - *(const i32 *)b;
}

// Example usage
int main(void) {
  i32 array[] = {10, 9, 8, 7, 6, 4, 3, 5, 1};
  usize n = sizeof(array) / sizeof(array[0]);
  quicksort(array, array, sizeof(i32), n, cmp);
  clib_assert(array[0] == 1, "Array was not sorted correctly");

  return 0;
}