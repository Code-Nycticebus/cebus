#ifndef __CLIB_SORTING_H__
#define __CLIB_SORTING_H__

/* DOCUMENTATION
### Usage
```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```
*/

#include "core/defines.h"

////////////////////////////////////////////////////////////////////////////

/* Sort an array with a function */
void quicksort(const void *src, void *dest, usize size, usize nelem,
               CompareFn compare);

////////////////////////////////////////////////////////////////////////////

/* Sort an array with a function that also takes a context */
void quicksort_ctx(const void *src, void *dest, usize size, usize nelem,
                   CompareCtxFn compare, const void *ctx);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_SORTING_H__ */
