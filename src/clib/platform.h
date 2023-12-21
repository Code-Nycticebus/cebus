#ifndef __CLIB_PLATFORM_H__
#define __CLIB_PLATFORM_H__

typedef enum {
  LINUX = 1,
  WINDOWS,
} ClibPlatform;

#if defined(__linux__)
#define CLIB_SYSTEM LINUX
#elif defined(_Win32) || defined(_Win64)
#define CLIB_SYSTEM WINDOWS
#else
#error "Platform currently not supported"
#endif

typedef enum {
  ARCHITECTURE_UNKNOWN,
  x86_64,
  x86_32,
  ARM2,
  ARM3,
  ARM4T,
  ARM5,
  ARM6T2,
  ARM6,
  ARM7,
  ARM7A,
  ARM7R,
  ARM7M,
  ARM7S,
  ARM64,
  MIPS,
  SUPERH,
  POWERPC,
  POWERPC64,
  SPARC,
  M68K,
} ClibArchitecture;

#if defined(__x86_64__) || defined(_M_X64)
#define CLIB_ARCHITECTURE x86_64
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define CLIB_ARCHITECTURE x86_32
#elif defined(__ARM_ARCH_2__)
#define CLIB_ARCHITECTURE ARM2
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define CLIB_ARCHITECTURE ARM3
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define CLIB_ARCHITECTURE ARM4T
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
#define CLIB_ARCHITECTURE ARM5
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define CLIB_ARCHITECTURE ARM6T2
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) ||                   \
    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) ||                    \
    defined(__ARM_ARCH_6ZK__)
#define CLIB_ARCHITECTURE ARM6
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) ||                   \
    defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||                    \
    defined(__ARM_ARCH_7S__)
#define CLIB_ARCHITECTURE ARM7
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||                  \
    defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define CLIB_ARCHITECTURE ARM7A
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||                  \
    defined(__ARM_ARCH_7S__)
#define CLIB_ARCHITECTURE ARM7R
#elif defined(__ARM_ARCH_7M__)
#define CLIB_ARCHITECTURE ARM7M
#elif defined(__ARM_ARCH_7S__)
#define CLIB_ARCHITECTURE ARM7S
#elif defined(__aarch64__) || defined(_M_ARM64)
#define CLIB_ARCHITECTURE ARM64
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define CLIB_ARCHITECTURE MIPS
#elif defined(__sh__)
#define CLIB_ARCHITECTURE SUPERH
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) ||  \
    defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) ||            \
    defined(_ARCH_PPC)
#define CLIB_ARCHITECTURE POWERPC
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define CLIB_ARCHITECTURE POWERPC64
#elif defined(__sparc__) || defined(__sparc)
#define CLIB_ARCHITECTURE SPARC
#elif defined(__m68k__)
#define CLIB_ARCHITECTURE M68K
#else
#define CLIB_ARCHITECTURE ARCHITECTURE_UNKNOWN
#endif

typedef enum {
  COMPILER_UNKOWN,
  GCC,
  CLANG,
  MSVC,
  MINGW32,
  MINGW64,
  INTEL_CC,
} ClibCompiler;

#if defined(__GNUC__) && !defined(__clang__)
#define CLIB_COMPILER GCC
#elif defined(__clang__)
#define CLIB_COMPILER CLANG
#elif defined(_MSC_VER)
#define CLIB_COMPILER MSVC
#elif defined(__MINGW32__)
#define CLIB_COMPILER MINGW32
#elif defined(__MINGW64__)
#define CLIB_COMPILER MINGW64
#elif defined(__INTEL_COMPILER)
#define CLIB_COMPILER INTEL_CC
#else
#define CLIB_COMPILER COMPILER_UNKOWN
#endif

#if defined(__LP64__) || defined(_LP64)
#define CLIB_64BIT
#else
#define CLIB_32BIT
#endif

typedef enum {
  ENDIAN_UNKOWN,
  ENDIAN_LITTLE,
  ENDIAN_BIG,
} ClibEndians;

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) &&                \
    defined(__ORDER_LITTLE_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define CLIB_ENDIANNESS ENDIAN_BIG
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define CLIB_ENDIANNESS ENDIAN_LITTLE
#else
#define CLIB_ENDIANNESS ENDIAN_UNKOWN
#endif
#else
#define CLIB_ENDIANNESS ENDIAN_UNKOWN
#endif

#ifdef __cross__
#define CLIB_CROSS_COMPILATION
#endif

#endif /*  !__CLIB_PLATFORM_H__ */
