#ifndef __CLIB_DEFINES_H__
#define __CLIB_DEFINES_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define KILOBYTES(s) ((size_t)(s) * (size_t)1e+3)
#define MEGABYTES(s) ((size_t)(s) * (size_t)1e+6)
#define GIGABYTES(s) ((size_t)(s) * (size_t)1e+9)

typedef enum {
  CMP_LESS = -1,
  CMP_EQUAL = 0,
  CMP_GREATER = 1,
} CmpOrdering;

#endif /* !__CLIB_DEFINES_H__ */
