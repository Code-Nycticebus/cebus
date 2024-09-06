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

#ifndef __CEBUS_PLATFORM_H__
#define __CEBUS_PLATFORM_H__

////////////////////////////////////////////////////////////////////////////
/* System */
#if defined(__linux__)
#define LINUX
#define CEBUS_SYSTEM "Linux"
#elif defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#define CEBUS_SYSTEM "Windows"
#define _CRT_SECURE_NO_WARNINGS
#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef near
#undef far
#else
#error "Platform not supported!"
#endif
/* !System */

////////////////////////////////////////////////////////////////////////////
/* Architecture */
#if defined(__x86_64__) || defined(_M_X64)
#define x86_64
#define CEBUS_ARCHITECTURE "x86_64"
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define x86_32
#define CEBUS_ARCHITECTURE "x86_32"
#elif defined(__ARM_ARCH_2__)
#define ARM2
#define CEBUS_ARCHITECTURE "ARM2"
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define ARM3
#define CEBUS_ARCHITECTURE "ARM3"
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define ARM4T
#define CEBUS_ARCHITECTURE "ARM4T"
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
#define ARM5
#define CEBUS_ARCHITECTURE "ARM5"
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define ARM6T2
#define CEBUS_ARCHITECTURE "ARM6T2"
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) ||           \
    defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
#define ARM6
#define CEBUS_ARCHITECTURE "ARM6"
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||           \
    defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARM7
#define CEBUS_ARCHITECTURE "ARM7"
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||          \
    defined(__ARM_ARCH_7S__)
#define ARM7A
#define CEBUS_ARCHITECTURE "ARM7A"
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARM7R
#define CEBUS_ARCHITECTURE "ARM7R"
#elif defined(__ARM_ARCH_7M__)
#define ARM7M
#define CEBUS_ARCHITECTURE "ARM7M"
#elif defined(__ARM_ARCH_7S__)
#define ARM7S
#define CEBUS_ARCHITECTURE "ARM7S"
#elif defined(__aarch64__) || defined(_M_ARM64)
#define ARM64
#define CEBUS_ARCHITECTURE "ARM64"
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define MIPS
#define CEBUS_ARCHITECTURE "MIPS"
#elif defined(__sh__)
#define SUPERH
#define CEBUS_ARCHITECTURE "SUPERH"
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) ||                      \
    defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
#define POWERPC
#define CEBUS_ARCHITECTURE "POWERPC"
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define POWERPC64
#define CEBUS_ARCHITECTURE "POWERPC64"
#elif defined(__sparc__) || defined(__sparc)
#define SPARC
#define CEBUS_ARCHITECTURE "SPARC"
#elif defined(__m68k__)
#define M68K
#define CEBUS_ARCHITECTURE "M68K"
#else
#define ARCHITECTURE_UNKNOWN 0
#define CEBUS_ARCHITECTURE "ARCHITECTURE UNKNOWN"
#endif
/* !Architecture */

////////////////////////////////////////////////////////////////////////////
/* Compiler */
#if defined(__GNUC__) && !defined(__clang__)
#define GCC
#define CEBUS_COMPILER "gcc"
#elif defined(__clang__)
#define CLANG
#define CEBUS_COMPILER "clang"
#elif defined(__TINYC__)
#define TINYC
#define CEBUS_COMPILER "tcc"
#elif defined(_MSC_VER)
#define MSVC
#define CEBUS_COMPILER "cl"
#elif defined(__MINGW32__)
#define MINGW32
#define CEBUS_COMPILER "mingw32"
#elif defined(__MINGW64__)
#define MINGW64
#define CEBUS_COMPILER "mingw64"
#else
#define COMPILER_UNKOWN
#define CEBUS_COMPILER "COMPILER UNKOWN"
#endif
/* !Compiler */

////////////////////////////////////////////////////////////////////////////
/* CPU */
#if defined(__LP64__) || defined(_LP64)
#define CEBUS_64BIT
#else
#define CEBUS_32BIT
#endif
/* !CPU */

////////////////////////////////////////////////////////////////////////////
/* Byte-Order */
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && defined(__ORDER_LITTLE_ENDIAN__)

#define ENDIAN_LITTLE __ORDER_LITTLE_ENDIAN__
#define ENDIAN_BIG __ORDER_BIG_ENDIAN__
#define CEBUS_BYTE_ORDER __BYTE_ORDER__

#elif defined(_MSC_VER)
#define ENDIAN_LITTLE 1234
#define ENDIAN_BIG 4321
#define CEBUS_BYTE_ORDER ENDIAN_LITTLE
#else
#error "No Byte Order detected!"
#endif
/* !Byte-Order */

////////////////////////////////////////////////////////////////////////////

#ifdef __cross__
#define CEBUS_CROSS_COMPILATION
#endif

////////////////////////////////////////////////////////////////////////////

#endif /*  !__CEBUS_PLATFORM_H__ */
