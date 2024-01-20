#ifndef __CLIB_SORTING_H__
#define __CLIB_SORTING_H__

#include "core/defines.h"

/* Sort an array with a function */
void quicksort(const void *src, void *dest, usize size, usize nelem,
               CompareFn compare);
/* Sort an array with a function that also takes a context */
void quicksort_ctx(const void *src, void *dest, usize size, usize nelem,
                   CompareCtxFn compare, const void *ctx);

#endif // !__CLIB_SORTING_H__
