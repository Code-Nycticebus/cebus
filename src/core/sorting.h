#ifndef __CLIB_SORTING_H__
#define __CLIB_SORTING_H__

#include "core/defines.h"

void quicksort(void *src, void *dest, usize size, usize nmem,
               CompareFn compare);

#endif // !__CLIB_SORTING_H__
