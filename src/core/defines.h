#ifndef __CLIB_DEFINES_H__
#define __CLIB_DEFINES_H__

#include "platform.h"

#include <float.h>
#include <stdbool.h>
#include <stddef.h> // IWYU pragma: export
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////

#define KILOBYTES(s) ((usize)(s) * (usize)1e+3)
#define MEGABYTES(s) ((usize)(s) * (usize)1e+6)
#define GIGABYTES(s) ((usize)(s) * (usize)1e+9)

#define ARRAY_LEN(A) (sizeof((A)) / sizeof((A)[0]))

////////////////////////////////////////////////////////////////////////////

typedef enum {
  CMP_LESS = -1,
  CMP_EQUAL = 0,
  CMP_GREATER = 1,
} CmpOrdering;

typedef CmpOrdering (*CompareFn)(const void *, const void *);
typedef CmpOrdering (*CompareCtxFn)(const void *, const void *, const void *);

////////////////////////////////////////////////////////////////////////////

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
#define F32_MAX FLT_MAX
#define F32_MIN FLT_MIN
#define F32_EPSILON FLT_EPSILON

typedef double f64;
#define F64_MAX DBL_MAX
#define F64_MIN DBL_MIN
#define F64_EPSILON DBL_EPSILON

#define F32_INF (*(const f32 *)(const u32[]){0x7F800000})
#define F64_INF (*(const f64 *)(const u64[]){0x7FF0000000000000})
#define NAN (0.0 / 0.0)
#define PI 3.14159265358979323846
#define E 2.71828182845904523536

////////////////////////////////////////////////////////////////////////////

#define BYTES(...)                                                             \
  (Bytes) {                                                                    \
    sizeof((const u8[]){__VA_ARGS__}), (const u8[]) { __VA_ARGS__ }            \
  }

#define BYTES_STR(s)                                                           \
  (Bytes) { sizeof(s) - 1, (const u8 *)(s) }

typedef struct {
  usize size;
  const u8 *data;
} Bytes;

////////////////////////////////////////////////////////////////////////////

#define STR(str) ((Str){.len = sizeof(str) - 1, .data = (str)})
#define STR_FMT "%.*s"
#define STR_ARG(str) (i32)(str).len, (str).data

typedef struct {
  usize len;
  const char *data;
} Str;

////////////////////////////////////////////////////////////////////////////

#define UTF8(s) utf8_decode(BYTES_STR(s), ErrDefault)
#define UTF8_FMT "%.*s"
#define UTF8_ARG(s) (i32)(s).size, (s).data

typedef struct {
  usize size;
  const char *data;
  usize len;
} Utf8;

////////////////////////////////////////////////////////////////////////////

#if defined(LINUX)
#include <bits/types/FILE.h>
#elif defined(WINDOWS)
#define _FILE_DEFINED
typedef struct _iobuf FILE;
#endif

////////////////////////////////////////////////////////////////////////////

#if defined(GCC) || defined(CLANG) || defined(MINGW32) || defined(MINGW64)

#include <sys/cdefs.h>
#define export
#define no_return __attribute__((noreturn))
#define pure __attribute__((pure))
#define const_fn __attribute__((const))
#define unused __attribute__((unused))
#define used __attribute__((used))
#define likely(exp) __builtin_expect(((exp) != 0), 1)
#define unlikely(exp) __builtin_expect(((exp) != 0), 0)
#if __has_attribute(format)
#define fmt_args(__fmt_arg)                                                    \
  __attribute__((format(printf, __fmt_arg, __fmt_arg + 1)))
#else
#define noreturn
#endif

#elif defined(MSVC)

#define export __declspec(dllexport)
#define no_return __declspec(noreturn)
#define pure
#define unused __pragma(warning(suppress : 4100))
#define used __pragma(warning(suppress : 4100))
#define likely(x) (x)
#define unlikely(x) (x)
#define fmt_args(__fmt_arg) _Printf_format_string_ __fmt_arg

#else

#define export
#define no_return
#define pure
#define unused
#define used
#define likely(...) (__VA_ARGS__)
#define unlikely(...) (__VA_ARGS__)
#define fmt_args(...)

#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_DEFINES_H__ */
