#ifndef __CLIB_DEFINES_H__
#define __CLIB_DEFINES_H__

#include "datatypes/datatypes.h" // IWYU pragma: export

#define KILOBYTES(s) ((usize)(s) * (usize)1e+3)
#define MEGABYTES(s) ((usize)(s) * (usize)1e+6)
#define GIGABYTES(s) ((usize)(s) * (usize)1e+9)

typedef enum {
  CMP_LESS = -1,
  CMP_EQUAL = 0,
  CMP_GREATER = 1,
} CmpOrdering;

#endif /* !__CLIB_DEFINES_H__ */
