# Core
## [asserts.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/asserts.h)
## Usage
You can assert if something is true with:
```c
clib_assert(1 == 1, "One should be one");
clib_assert(2 == 2, "It can even handle arguments: %d", 420);
```

Here are all the available macros
```c
clib_assert(EXPR, FMT, ...);
clib_assert_warn(EXPR, FMT, ...);
clib_assert_debug(EXPR, FMT, ...);
clib_assert_return(EXPR, RETURN_VALUE);
```
## [sorting.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/sorting.h)
## Example
```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```
void quicksort(const void *src, void *dest, usize size, usize nelem,
               CompareFn compare);

////////////////////////////////////////////////////////////////////////////

## [error.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/error.h)
## Usage
If a function can fail it should take an ```Error*``` as parameter. If an error
occurs, initialize the Error with ```error_emit()```.
```c
void function_that_can_fail(Error* error)
  int error_code = -1; // Function returns a bad value
  if (error_code < 0) {
    error_emit(error, error_code, "error: %d", error_code);
  }
}
```

Create new ```Error``` with ```ErrNew``` to except errors that occure
inside the function.
```c
Error error = ErrNew;
function_that_can_fail(&error);
```

Work inside of an error context with ```error_context()```
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_context(&error, {
  // Do error handling
});
```

Add notes to ```Error``` with ```error_add_note()``` or set a different error
code with ```error_set_code()```
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_context(&error, {
  error_add_note("Note added to error");
  error_set_code(69);
});
```

```error_panic()``` panics and aborts if it
encounters an error, ```error_warn()``` just prints a warning message and
```error_ignore()``` ignores error completely.
```c
Error error = ErrCreate;
function_that_can_fail(&error);
error_context(&error, {
  error_warn();
  error_ignore();
  error_panic();
});
```

Pass ```ErrPanic``` to that function to automatically call
```error_panic()``` if it encouters an error inside the function.
```c
function_that_can_fail(ErrPanic);
```

```ErrDefault``` is the same as ```ErrorPanic``` but the ```Error``` gets
created at the first occurence of an error.
```c
function_that_can_fail(ErrDefault);
```

Match your error types with ```error_match()```.
```c
  Error error = ErrNew;
  function_that_can_fail(true, &err2);
  error_context(&error, {
    error_match({
      case 69:
        error_panic();
      case 420: {
        error_ignore();
      } break;
    });
  });
```
## [platform.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/platform.h)
Here are various macros for figuring out what Platform and compiler is used.
#if defined(__linux__)
#define LINUX
#define CLIB_SYSTEM "Linux"
#elif defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#define CLIB_SYSTEM "Windows"
#else
#error "Platform currently not supported"
#endif
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
#if defined(__GNUC__) && !defined(__clang__)
#define GCC
#define CLIB_COMPILER "GCC"
#elif defined(__clang__)
#define CLANG
#define CLIB_COMPILER "Clang"
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
#if defined(__LP64__) || defined(_LP64)
#define CLIB_64BIT
#else
#define CLIB_32BIT
#endif
#define ENDIAN_UNKOWN 0x00
#define ENDIAN_LITTLE 0x01
#define ENDIAN_BIG 0x80

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) &&                \
    defined(__ORDER_LITTLE_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define CLIB_BYTE_ORDER ENDIAN_BIG
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define CLIB_BYTE_ORDER ENDIAN_LITTLE
#endif
#else
#define CLIB_BYTE_ORDER ENDIAN_UNKOWN
#endif
## [arena.h](https://github.com/Code-Nycticebus/clib/blob/main/src/core/arena.h)
## Usage
Create a new Arena with:
```c
Arena arena = {0};
```

Now you can allocate from this arena.
```c
int* i1 = arena_alloc(&arena, sizeof(int));
int* i2 = arena_alloc(&arena, sizeof(int));
int* i3 = arena_alloc(&arena, sizeof(int));
```

Don't forget to free the arena once you're done. This frees all allocated
integers at once.
```c
arena_free(&arena);
```
