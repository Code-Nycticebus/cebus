#include "sorting.h"

#include <string.h>

static void swap(void *a, void *b, usize size) {
  u8 temp[size];
  memcpy(temp, a, size);
  memcpy(a, b, size);
  memcpy(b, temp, size);
}

static usize partition(u8 *base, usize size, usize low, usize high,
                       CompareFn compare) {
  u8 *pivot = &base[high * size];
  usize i = low - 1;
  for (usize j = low; j < high; j++) {
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
    _quicksort(base, size, low, pi - 1, compare);
    _quicksort(base, size, pi + 1, high, compare);
  }
}

void quicksort(void *arr, void *dest, usize size, usize nelem,
               CompareFn compare) {
  if (dest != arr) {
    memcpy(dest, arr, size * nelem);
  }
  _quicksort(dest, size, 0, nelem - 1, compare);
}
