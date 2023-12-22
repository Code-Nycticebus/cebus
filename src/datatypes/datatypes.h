#ifndef __CLIB_DATATYPES_H__
#define __CLIB_DATATYPES_H__

#include <clib/defines.h>

typedef struct {
  size_t size;
  const uint8_t *data;
} Bytes;

typedef struct {
  size_t len;
  const char *data;
} Str;

typedef struct {
  size_t len;
  size_t size;
  const char *data;
} Utf8;

#endif /* !__CLIB_DATATYPES_H__ */
