#include "sorting.h"

#include <string.h>

////////////////////////////////////////////////////////////////////////////

static void swap(void *a, void *b, usize size) {
  u8 temp[size];
  memcpy(temp, a, size);
  memcpy(a, b, size);
  memcpy(b, temp, size);
}

////////////////////////////////////////////////////////////////////////////

static usize partition(u8 *base, usize size, usize low, usize high,
                       CompareFn compare) {
  u8 *pivot = &base[high * size];
  usize i = low - 1;
  for (usize j = low; j <= high - 1; j++) {
    if (compare(&base[j * size], pivot) < 0) {
      i++;
      swap(&base[i * size], &base[j * size], size);
    }
  }
  swap(&base[(i + 1) * size], pivot, size);
  return (i + 1);
}

static void _quicksort(void *base, size_t size, usize low, usize high,
                       CompareFn compare) {
  if (low < high) {
    usize pi = partition(base, size, low, high, compare);
    _quicksort(base, size, low, pi ? pi - 1 : 0, compare);
    _quicksort(base, size, pi + 1, high, compare);
  }
}

void quicksort(const void *src, void *dest, usize size, usize nelem,
               CompareFn compare) {
  if (dest != src) {
    memcpy(dest, src, size * nelem);
  }
  _quicksort(dest, size, 0, nelem - 1, compare);
}

////////////////////////////////////////////////////////////////////////////

static usize partition_ctx(u8 *base, usize size, usize low, usize high,
                           CompareCtxFn compare, const void *ctx) {
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

static void _quicksort_ctx(void *base, size_t size, usize low, usize high,
                           CompareCtxFn compare, const void *ctx) {
  if (low < high) {
    usize pi = partition_ctx(base, size, low, high, compare, ctx);
    _quicksort_ctx(base, size, low, pi ? pi - 1 : 0, compare, ctx);
    _quicksort_ctx(base, size, pi + 1, high, compare, ctx);
  }
}

void quicksort_ctx(const void *src, void *dest, usize size, usize nelem,
                   CompareCtxFn compare, const void *ctx) {
  if (dest != src) {
    memcpy(dest, src, size * nelem);
  }
  _quicksort_ctx(dest, size, 0, nelem - 1, compare, ctx);
}

////////////////////////////////////////////////////////////////////////////
