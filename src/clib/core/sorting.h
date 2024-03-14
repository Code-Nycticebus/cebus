/* DOCUMENTATION
## Usage

Sort an array by providing the array, its size, the number of elements, and a comparison function:

```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```

For context-aware comparisons, use `quicksort_ctx` with a comparison function that takes an additional context parameter.

*/

#ifndef __CLIB_SORTING_H__
#define __CLIB_SORTING_H__

#include "clib/core/defines.h"

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
