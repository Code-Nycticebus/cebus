#ifndef __CLIB_DATATYPES_H__
#define __CLIB_DATATYPES_H__

#include "clib/platform.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
#define U8_MAX UINT8_MAX
#define U8_MIN 0
#define U8_BITS 8
#define U8_FMT "hhu"
#define U8_HEX "hhx"

typedef int8_t i8;
#define I8_MAX INT8_MAX
#define I8_MIN INT8_MIN
#define I8_BITS 8
#define I8_FMT "hhd"
#define I8_HEX "hhx"

typedef uint16_t u16;
#define U16_MAX UINT16_MAX
#define U16_MIN 0
#define U16_BITS 16
#define U16_FMT "hd"
#define U16_HEX "hx"

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
#if defined(LINUX)
#define U64_FMT "lu"
#define U64_HEX "lx"
#elif defined(WINDOWS)
#define U64_FMT "llu"
#define U64_HEX "llx"
#endif

typedef int64_t i64;
#define I64_MAX INT64_MAX
#define I64_MIN INT64_MIN
#define I64_BITS 64
#if defined(LINUX)
#define I64_FMT "lu"
#define I64_HEX "lx"
#elif defined(WINDOWS)
#define I64_FMT "llu"
#define I64_HEX "llx"
#endif

typedef size_t usize;
#define USIZE_MAX SIZE_MAX
#define USIZE_MIN 0
#define USIZE_BITS (sizeof(usize) * 8)
#if defined(LINUX)
#define USIZE_FMT "zu"
#elif defined(WINDOWS)
#define USIZE_FMT "llu"
#endif

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
  usize size;
  const char *data;
  usize len;
} Utf8;

#endif /* !__CLIB_DATATYPES_H__ */
