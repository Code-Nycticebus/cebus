#ifndef __CLIB_SORTING_H__
#define __CLIB_SORTING_H__

#include "core/defines.h"

void quicksort(void *src, void *dest, usize size, usize nmem,
               CompareFn compare);
void quicksort_ctx(void *src, void *dest, usize size, usize nmem,
                   CompareCtxFn compare, const void *ctx);

#endif // !__CLIB_SORTING_H__
