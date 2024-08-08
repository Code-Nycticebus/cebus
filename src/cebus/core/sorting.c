#include "sorting.h"

#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

void quicksort(const void *src, void *dest, usize size, usize nelem, CompareFn compare) {
  if (dest != src) {
    memcpy(dest, src, size * nelem);
  }
  qsort(dest, nelem, size, compare);
}

////////////////////////////////////////////////////////////////////////////

static inline void swap(void *a, void *b, usize size) {
  u8 *pa = a;
  u8 *pb = b;
  for (usize i = 0; i < size; ++i) {
    u8 temp = pa[i];
    pa[i] = pb[i];
    pb[i] = temp;
  }
}

////////////////////////////////////////////////////////////////////////////

static inline usize partition_ctx(u8 *base, usize size, usize low, usize high, CompareCtxFn compare,
                                  const void *ctx) {
  u8 *pivot = &base[high * size];
  usize i = low - 1;
  for (usize j = low; j <= high - 1; j++) {
    if (compare(ctx, &base[j * size], pivot) < 0) {
      i++;
      swap(&base[i * size], &base[j * size], size);
    }
  }
  swap(&base[(i + 1) * size], pivot, size);
  return (i + 1);
}

static void _quicksort_ctx(void *base, size_t size, usize low, usize high, CompareCtxFn compare,
                           const void *ctx) {
  if (low < high) {
    usize pi = partition_ctx(base, size, low, high, compare, ctx);
    _quicksort_ctx(base, size, low, pi ? pi - 1 : 0, compare, ctx);
    _quicksort_ctx(base, size, pi + 1, high, compare, ctx);
  }
}

void quicksort_ctx(const void *src, void *dest, usize size, usize nelem, CompareCtxFn compare,
                   const void *ctx) {
  if (dest != src) {
    memcpy(dest, src, size * nelem);
  }
  _quicksort_ctx(dest, size, 0, nelem - 1, compare, ctx);
}

////////////////////////////////////////////////////////////////////////////
