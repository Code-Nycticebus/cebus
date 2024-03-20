/* DOCUMENTATION
## Key Definitions and Macros

- **Data Types**: Defines types such as `u8`, `i8`, `u32`, `i32`,
`usize`, `f32`, `f64`, `Bytes`, `Str`, and `Utf8`.
- **Mathematical Constants**: Defines `F64_PI` and `F64_E` for mathematical
operations.
- **Memory Units**: Macros for `KILOBYTES`, `MEGABYTES`, and `GIGABYTES` to
easily specify memory sizes.
- **Array Length**: `ARRAY_LEN(A)` calculates the number of elements in an
array.
- **Comparison Ordering**: Enum `CmpOrdering` for less than, equal, and greater
than comparisons.
- **Compiler Attributes**: Macros such as `EXPORT`, `NORETURN`, `UNUSED`,
`PURE_FN`, `CONST_FN` for compiler-specific attributes.
- **Likely and Unlikely**: `LIKELY` and `UNLIKELY` macros to hint the compiler
about branch prediction.
- **Format Attribute**: `FMT` macro to specify format strings for functions,
enhancing type safety with `printf`-like functions.

*/

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
#else
#define U64_FMT "lu"
#define U64_HEX "lx"
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
#else
#define I64_FMT "lu"
#define I32_FMT "lx"
#endif

typedef size_t usize;
#define USIZE_MAX SIZE_MAX
#define USIZE_MIN 0
#define USIZE_BITS (sizeof(usize) * 8)
#if defined(WINDOWS) && defined(GCC)
#define USIZE_FMT "llu"
#else
#define USIZE_FMT "zu"
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
#define F32_NAN (0.0f / 0.0f)
#define F64_NAN (0.0 / 0.0)
#define F64_PI 3.14159265358979323846
#define F32_PI 3.1415926535f
#define F64_E 2.71828182845904523536
#define F32_E 2.7182818284f

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

#define EXPORT __attribute__((used))
#define NORETURN __attribute__((noreturn))
#define UNUSED __attribute__((unused))
#define PURE_FN __attribute__((pure)) __attribute__((warn_unused_result))
#define CONST_FN __attribute__((const)) __attribute__((warn_unused_result))
#define LIKELY(exp) __builtin_expect(((exp) != 0), 1)
#define UNLIKELY(exp) __builtin_expect(((exp) != 0), 0)
#define FMT(__fmt_arg) __attribute__((format(printf, __fmt_arg, __fmt_arg + 1)))

#elif defined(MSVC)

#include <sal.h>
#define EXPORT __declspec(dllexport)
#define NORETURN __declspec(noreturn)
#define UNUSED __pragma(warning(suppress : 4100))
#define PURE_FN _Check_return
#define CONST_FN _Check_return

#endif

#ifndef EXPORT
#define EXPORT
#endif

#ifndef NORETURN
#define NORETURN
#endif

#ifndef UNUSED
#define UNUSED
#endif

#ifndef PURE_FN
#define PURE_FN
#endif

#ifndef CONST_FN
#define CONST_FN
#endif

#ifndef LIKELY
#define LIKELY(...) (__VA_ARGS__)
#endif

#ifndef UNLIKELY
#define UNLIKELY(...) (__VA_ARGS__)
#endif

#ifndef FMT
#define FMT(...)
#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_DEFINES_H__ */
