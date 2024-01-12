#ifndef __CLIB_SORTING_H__
#define __CLIB_SORTING_H__

#include "core/defines.h"

void quicksort(const void *src, void *dest, usize size, usize nmem,
               CompareFn compare);
void quicksort_ctx(const void *src, void *dest, usize size, usize nmem,
                   CompareCtxFn compare, const void *ctx);

#endif // !__CLIB_SORTING_H__
