#ifndef __CLIB_DATATYPES_H__
#define __CLIB_DATATYPES_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
#define U8_MAX UINT8_MAX
#define U8_MIN 0
#define U8_BITS 8

typedef int8_t i8;
#define I8_MAX INT8_MAX
#define I8_MIN INT8_MIN
#define I8_BITS 8

typedef uint16_t u16;
#define U16_MAX UINT16_MAX
#define U16_MIN 0
#define U16_BITS 16

typedef int16_t i16;
#define I16_MAX INT16_MAX
#define I16_MIN INT16_MIN
#define I16_BITS 16

typedef uint32_t u32;
#define U32_MAX UINT32_MAX
#define U32_MIN 0
#define U32_BITS 32

typedef int32_t i32;
#define I32_MAX INT32_MAX
#define I32_MIN INT32_MIN
#define I32_BITS 32

typedef uint64_t u64;
#define U64_MAX UINT64_MAX
#define U64_MIN 0
#define U64_BITS 64

typedef int64_t i64;
#define I64_MAX INT64_MAX
#define I64_MIN INT64_MIN
#define I64_BITS 64

typedef size_t usize;
#define USIZE_MAX SIZE_MAX
#define USIZE_MIN 0
#define USIZE_BITS (sizeof(usize) * 8)

typedef float f32;
typedef double f64;

typedef struct {
  usize size;
  const u8 *data;
} Bytes;

typedef struct {
  usize len;
  const char *data;
} Str;

typedef struct {
  usize len;
  usize size;
  const char *data;
} Utf8;

#endif /* !__CLIB_DATATYPES_H__ */
