/* DOCUMENTATION
## Features

- **Platform Detection**: Identifies the operating system, such as Linux or
Windows.
- **Architecture Detection**: Determines the CPU architecture, such as x86_64 or
ARM.
- **Compiler Detection**: Identifies the compiler used, such as GCC, Clang, or
MSVC.
- **CPU Bitness**: Distinguishes between 32-bit and 64-bit environments.
- **Byte Order**: Defines the system's byte order (endianness).
*/

#ifndef __CLIB_PLATFORM_H__
#define __CLIB_PLATFORM_H__

////////////////////////////////////////////////////////////////////////////
/* System */
#if defined(__linux__)
#define LINUX
#define CLIB_SYSTEM "Linux"
#elif defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#define CLIB_SYSTEM "Windows"
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#else
#error "Platform not supported!"
#endif
/* !System */

////////////////////////////////////////////////////////////////////////////
/* Architecture */
#if defined(__x86_64__) || defined(_M_X64)
#define x86_64
#define CLIB_ARCHITECTURE "x86_64"
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define x86_32
#define CLIB_ARCHITECTURE "x86_32"
#elif defined(__ARM_ARCH_2__)
#define ARM2
#define CLIB_ARCHITECTURE "ARM2"
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define ARM3
#define CLIB_ARCHITECTURE "ARM3"
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define ARM4T
#define CLIB_ARCHITECTURE "ARM4T"
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
#define ARM5
#define CLIB_ARCHITECTURE "ARM5"
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define ARM6T2
#define CLIB_ARCHITECTURE "ARM6T2"
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) ||                   \
    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) ||                    \
    defined(__ARM_ARCH_6ZK__)
#define ARM6
#define CLIB_ARCHITECTURE "ARM6"
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) ||                   \
    defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||                    \
    defined(__ARM_ARCH_7S__)
#define ARM7
#define CLIB_ARCHITECTURE "ARM7"
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||                  \
    defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARM7A
#define CLIB_ARCHITECTURE "ARM7A"
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||                  \
    defined(__ARM_ARCH_7S__)
#define ARM7R
#define CLIB_ARCHITECTURE "ARM7R"
#elif defined(__ARM_ARCH_7M__)
#define ARM7M
#define CLIB_ARCHITECTURE "ARM7M"
#elif defined(__ARM_ARCH_7S__)
#define ARM7S
#define CLIB_ARCHITECTURE "ARM7S"
#elif defined(__aarch64__) || defined(_M_ARM64)
#define ARM64
#define CLIB_ARCHITECTURE "ARM64"
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define MIPS
#define CLIB_ARCHITECTURE "MIPS"
#elif defined(__sh__)
#define SUPERH
#define CLIB_ARCHITECTURE "SUPERH"
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) ||  \
    defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) ||            \
    defined(_ARCH_PPC)
#define POWERPC
#define CLIB_ARCHITECTURE "POWERPC"
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define POWERPC64
#define CLIB_ARCHITECTURE "POWERPC64"
#elif defined(__sparc__) || defined(__sparc)
#define SPARC
#define CLIB_ARCHITECTURE "SPARC"
#elif defined(__m68k__)
#define M68K
#define CLIB_ARCHITECTURE "M68K"
#else
#define ARCHITECTURE_UNKNOWN 0
#define CLIB_ARCHITECTURE "ARCHITECTURE UNKNOWN"
#endif
/* !Architecture */

////////////////////////////////////////////////////////////////////////////
/* Compiler */
#if defined(__GNUC__) && !defined(__clang__)
#define GCC
#define CLIB_COMPILER "GCC"
#elif defined(__clang__)
#define CLANG
#define CLIB_COMPILER "Clang"
#elif defined(__TINYC__)
#define TINYC
#define CLIB_COMPILER "TinyC"
#elif defined(_MSC_VER)
#define MSVC
#define CLIB_COMPILER "MSVC"
#elif defined(__MINGW32__)
#define MINGW32
#define CLIB_COMPILER "MinGW32"
#elif defined(__MINGW64__)
#define MINGW64
#define CLIB_COMPILER "MinGW64"
#elif defined(__INTEL_COMPILER)
#define INTEL_COMPILER
#define CLIB_COMPILER "Intel Compiler"
#else
#define COMPILER_UNKOWN
#define CLIB_COMPILER "COMPILER UNKOWN"
#endif
/* !Compiler */

////////////////////////////////////////////////////////////////////////////
/* CPU */
#if defined(__LP64__) || defined(_LP64)
#define CLIB_64BIT
#else
#define CLIB_32BIT
#endif
/* !CPU */

////////////////////////////////////////////////////////////////////////////
/* Byte-Order */
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) &&                \
    defined(__ORDER_LITTLE_ENDIAN__)

#define ENDIAN_LITTLE __ORDER_LITTLE_ENDIAN__
#define ENDIAN_BIG __ORDER_BIG_ENDIAN__
#define CLIB_BYTE_ORDER __BYTE_ORDER__

#elif defined(_MSC_VER)
#define ENDIAN_LITTLE 0x00000001
#define ENDIAN_BIG 0x80000000
#define CLIB_BYTE_ORDER ENDIAN_LITTLE
#else
#error "No Byte Order detected!"
#endif
/* !Byte-Order */

////////////////////////////////////////////////////////////////////////////

#ifdef __cross__
#define CLIB_CROSS_COMPILATION
#endif

////////////////////////////////////////////////////////////////////////////

#endif /*  !__CLIB_PLATFORM_H__ */
