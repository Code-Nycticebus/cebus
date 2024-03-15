# C Library 

This is an extension of the C standart library. 

### Quickstart

#### [pybuildc](https://github.com/Code-Nycticebus/pybuildc)

```console
pybuildc build
```

#### Gcc or Clang

Build the static library.

```console
gcc -I./src -c ./src/**/*.c && ar rcs libclib.a *.o && rm *.o
```

Set the ```CLIB_DIRECTORY``` variable and add ```-I"$CLIB DIRECTORY/src" -L"$CLIB_DIRECTORY" -lclib``` to your compiler flags.

```console
gcc -o info examples/info.c -I"$CLIB_DIRECTORY/src" -L"$CLIB_DIRECTORY" -lclib
```

## Documentation

<!-- DOCUMENTATION -->
- [Clib](#Clib)
   - [clib.h](#clibh)
- [Collection](#Collection)
   - [da.h](#dah)
   - [hashmap.h](#hashmaph)
   - [set.h](#seth)
- [Core](#Core)
   - [arena.h](#arenah)
   - [assert.h](#asserth)
   - [defines.h](#definesh)
   - [error.h](#errorh)
   - [logging.h](#loggingh)
   - [platform.h](#platformh)
   - [sorting.h](#sortingh)
- [Os](#Os)
   - [cmd.h](#cmdh)
   - [dll.h](#dllh)
   - [fs.h](#fsh)
   - [io.h](#ioh)
   - [os.h](#osh)
- [Type](#Type)
   - [byte.h](#byteh)
   - [char.h](#charh)
   - [float.h](#floath)
   - [integer.h](#integerh)
   - [string.h](#stringh)
   - [utf8.h](#utf8h)

## Clib

## [clib.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/clib.h)
Include this file to include all the header files.
```c
#include "clib/clib.h"
```

## Collection

## [da.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/collection/da.h)
## Initialization

To start using the dynamic array, you first need to create an instance of `Arena` for memory management and then initialize the dynamic array with the specific type.

```c
Arena arena = {0};
DA(int) vec = {0};
da_init(&vec, &arena);
```

## Adding Elements

Elements can be added to the dynamic array using `da_push`, which automatically resizes the array if additional space is required.

```c
da_push(&vec, 69);
da_push(&vec, 420);
```

## Accessing Elements

Access the first and last elements with `da_first` and `da_last`. Remove the last element with `da_pop`.

> :warning: `da_pop` does not perform any bounds checks.

```c
int first = da_first(&vec);
int last = da_last(&vec);
int popped = da_pop(&vec);
```

## Manipulating the Array

- `da_clear`: Reset the length of the array to zero.
- `da_empty`: Use to check if the array has no elements.
- `da_init_list`: Initialize dynamic array with elements from a static array.
- `da_copy`: Duplicate the contents of one dynamic array into another.

## Resizing and Reserving Space

- `da_resize`: Adjust the capacity of the array to a specified size. This is useful for preallocating space.
- `da_reserve`: Ensure there is enough space for additional elements, useful before adding multiple elements.

## Extending and Mapping

- `da_extend`: Add multiple elements from another array or list.
- `da_map`: Transform elements of the array into another form and store them in a destination array.
- `da_filter`: Filter dynamic array with a filter function and place it into a destination.
- `da_filter_ctx`: Filter dynamic array with a filter function, that takes a `void*` as a context, and place it into a destination.

## Sorting and Reversing

- `da_sort`: Sort the array using a comparison function.
- `da_sort_ctx`: Sort the array using a comparison function that also takes a `void*` as a context.
- `da_reverse`: Reverse the order of elements in the array.

## [hashmap.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/collection/hashmap.h)
## Initialization

Creating a new `HashMap` involves initializing an `Arena`, then calling `hm_create` or `hm_with_size` to initialize the hashmap with an optional initial size:

```c
Arena arena = {0};
HashMap* hm = hm_create(&arena);
```

## HashMap Operations

Basic hashmap management includes clearing, copying, resizing, reserving capacity, and updating from another hashmap:

- `hm_clear`: Clears the hashmap.
- `hm_copy`: Creates a copy of the hashmap.
- `hm_resize`: Resizes the hashmap.
- `hm_reserve`: Reserves space in the hashmap to optimize for upcoming insertions.
- `hm_update`: Merges another hashmap into the current one.

## Inserting Elements

Elements of various types can be inserted into the hashmap, including integers, floating-point numbers, and pointers:

- `hm_insert_f32`, `hm_insert_f64`: Insert floating-point values.
- `hm_insert_i32`, `hm_insert_u32`, `hm_insert_i64`, `hm_insert_u64`: Insert integer values.
- `hm_insert_mut_ptr`, `hm_insert_ptr`: Insert mutable or constant pointers.

## Querying Elements

Retrieve pointers to the values stored in the hashmap by their key hashes, allowing for mutable or immutable access:

> :warning: Avoid storing pointers from the hashmap for extended periods. 
> Keeping these pointers beyond the immediate scope can lead to undefined behavior, as the underlying storage may change.

- `hm_get_f32_mut`, `hm_get_f64_mut`, etc.: Retrieve mutable pointers to values.
- `hm_get_f32`, `hm_get_f64`, etc.: Retrieve immutable pointers to values.

## [set.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/collection/set.h)
## Initialization

To start using the set, you need to initialize an `Arena` and create a new set:

```c
Arena arena = {0};
Set set = set_create(&arena);
```

## Set Operations

Perform basic set operations such as adding, removing, and extending sets with multiple elements:

- `set_add`: Add an element to the set. Returns false if the element already exists.
- `set_remove`: Remove an element from the set.
- `set_extend`: Add multiple elements to the set at once.

Copy and resize sets for handling dynamic set sizes:

- `set_copy`: Create a duplicate of a set.
- `set_resize`: Adjust the internal capacity of the set.
- `set_reserve`: Ensure the set has enough capacity for a specified number of elements.

## Set Query Operations

Query the set for the presence of elements, equality with another set, or subset and disjoint relationships:

- `set_contains`: Check if an element is in the set.
- `set_eq`: Check if two sets are equal.
- `set_subset`: Determine if one set is a subset of another.
- `set_disjoint`: Determine if two sets have no elements in common.

## Set Algebraic Operations

Combine sets or find their differences using algebraic set operations:

- `set_intersection`: Find the intersection of two sets.
- `set_difference`: Find the difference between two sets.
- `set_union`: Combine two sets into a union.

## Core

## [arena.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/core/arena.h)
## Initialization

To start using the library, initialize an `Arena` struct:

```c
Arena arena = {0};
```

## Memory Allocation

Allocate memory from the arena using `arena_alloc` or `arena_calloc` for uninitialized or zero-initialized memory, respectively:

```c
int* i1 = arena_alloc(&arena, sizeof(int));
int* i2 = arena_alloc(&arena, sizeof(int));
int* i3 = arena_alloc(&arena, sizeof(int));
```

## Memory Deallocation

Deallocate all memory associated with an arena at once using `arena_free`. This operation frees all memory chunks allocated from the arena, providing a significant performance advantage over individual deallocation:

```c
arena_free(&arena);
```

## Chunk 

The library also provides functions for more granular control over memory chunks within an arena:

- `arena_alloc_chunk`: Allocate a new chunk of memory.
- `arena_calloc_chunk`: Allocates a new, zero initialized, chunk of memory.
- `arena_realloc_chunk`: Reallocate a previously allocated chunk to a new size.
- `arena_free_chunk`: Free a specific chunk of memory (advanced use cases).

## [assert.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/core/assert.h)
## Usage

Use the assertion macros to validate conditions and log failures:

```c
clib_assert(1 == 1, "It needs a message");
clib_assert(2 == 2, "It can even handle arguments: %d", 420);
```

## Available Macros

- `clib_assert(EXPR, FMT, ...)`: Asserts a condition and logs an error if the condition is false.
- `clib_assert_warn(EXPR, FMT, ...)`: Asserts a condition and logs a warning if the condition is false.
- `clib_assert_debug(EXPR, FMT, ...)`: Debug-only assert that logs a debug message if the condition is false.
- `clib_assert_return(EXPR, RETURN_VALUE)`: Asserts a condition and returns a value if the condition is false.

## Debug Break

A `debugbreak` is triggered on assertion failure to facilitate debugging, with behavior varying by platform:

- **Linux**: Uses `raise(SIGTRAP)`.
- **Windows**: Uses `__debugbreak()`.
- **Other Platforms**: Uses `abort()`.



## [defines.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/core/defines.h)
## Key Definitions and Macros

- **Data Types**: Defines essential types such as `u8`, `i8`, `u32`, `i32`, `usize`, `f32`, `f64`, `Bytes`, `Str`, and `Utf8`.
- **Mathematical Constants**: Defines `F32_PI` and `F32_E` for mathematical operations.
- **Memory Units**: Macros for `KILOBYTES`, `MEGABYTES`, and `GIGABYTES` to easily specify memory sizes.
- **Array Length**: `ARRAY_LEN(A)` calculates the number of elements in an array.
- **Comparison Ordering**: Enum `CmpOrdering` for less than, equal, and greater than comparisons.
- **Compiler Attributes**: Macros such as `EXPORT`, `NORETURN`, `UNUSED`, `PURE_FN`, `CONST_FN` for compiler-specific attributes.
- **Likely and Unlikely**: `LIKELY` and `UNLIKELY` macros to hint the compiler about branch prediction.
- **Format Attribute**: `FMT` macro to specify format strings for functions, enhancing type safety with `printf`-like functions.

## Usage Example

```c
u64 large_number = GIGABYTES(2);
printf("Large number in bytes: %" U64_FMT "\n", large_number);

Str example = STR("Example");
printf(STR_FMT "\n", STR_ARG(example));
```

## [error.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/core/error.h)
### Initialization Macros
- `ErrNew`: Initializes a new Error instance.
- `ErrPanic`: Initializes an Error that will trigger a panic on `error_emit()`.
- `ErrDefault`: Empty Error that will panic on `error_emit()`.

### Error Emitting and Context
- `error_emit()` initializes the passed in error.
```c
void function_that_fails(Error *error) {
  // ...
  if (failure_condition) {
    error_emit(error, error_code, "Error: %s", reason);
  }
}
```

- `error_context()` creates a context for you to handle the error. Panics if it
falls through
```c
Error error = ErrNew;
function_that_fails(&error);
error_context(&error, {
  // Error Handling
});
```

- `error_propagate()` creates a context. Does not panic if it falls through but
also does not reset the error.
```c
Error error = ErrNew;
function_that_fails(&error);
error_propagate(&error, {
  // Error Handling
});
```

### Error Handling
- `error_panic()`: Triggers a panic with the current error.
- `error_except()`: Resets the error state.
- `error_msg()`: Retrieves the error message.
- `error_code(T)`: Retrieves the error code and casts it to `T`.
- `error_set_code()`: Sets a new error code.
- `error_set_msg()`: Sets a new error message.
- `error_add_location()`: Adds current file and line location.
- `error_add_note()`: Adds a note to the error.


## [logging.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/core/logging.h)
## Usage

Call the function with the message format and arguments:

```c
clib_log_error("This is an error message: %s", error_details);
```

## Functions

- `clib_log(log_level, fmt, ...)`: Logs a message with the specified log level.
- `clib_log_fatal(fmt, ...)`: Logs a fatal message.
- `clib_log_error(fmt, ...)`: Logs an error message.
- `clib_log_warning(fmt, ...)`: Logs a warning message.
- `clib_log_info(fmt, ...)`: Logs an info message.
- `clib_log_debug(fmt, ...)`: Logs a debug message (only in debug builds).
- `clib_log_trace(fmt, ...)`: Logs a trace message (only in debug builds).

## [platform.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/core/platform.h)
## Features

- **Platform Detection**: Identifies the operating system, such as Linux or Windows.
- **Architecture Detection**: Determines the CPU architecture, such as x86_64 or ARM.
- **Compiler Detection**: Identifies the compiler used, such as GCC, Clang, or MSVC.
- **CPU Bitness**: Distinguishes between 32-bit and 64-bit environments.
- **Byte Order**: Defines the system's byte order (endianness).

## [sorting.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/core/sorting.h)
## Usage

Sort an array by providing the array, its size, the number of elements, and a comparison function:

```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```

For context-aware comparisons, use `quicksort_ctx` with a comparison function that takes an additional context parameter.


## Os

## [cmd.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/os/cmd.h)
## Functions

- **`cmd_exec(error, argc, argv)`**: Executes a system command.

## Error Handling

Defines `CmdError` enum for different command execution outcomes:
- `CMD_OK`: Command executed successfully.
- `CMD_FORK`: Error occurred while forking the process.
- `CMD_NOT_FOUND`: Command not found, typically returns `127`.

## Usage Example

```c
Error error = ErrNew;
Str args[] = {STR("/bin/echo"), STR("Hello, world!")};
cmd_exec(&error, 2, args);
error_context(&error, {
    // Handle error
});
```

## [dll.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/os/dll.h)
## Functions

- **`dll_load(path, error)`**: Loads a dynamic link library.
- **`dll_close(handle)`**: Closes an opened dynamic link library.
- **`dll_symbol(handle, symbol, error)`**: Retrieves a symbol from the dynamic link library.


## Usage Example

```c
Error error = ErrNew;
Dll *myLib = dll_load(STR("myLibrary.dll"), &error);
error_context(&error, {
	error_raise();
});
Function *myFunction = dll_symbol(myLib, "myFunctionName", &error);
// Use the function pointer as needed
dll_close(myLib);
```

## [fs.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/os/fs.h)
## Functions

- **Reading Files**:
  - `file_read_bytes(filename, arena, error)`: Reads the entire file into a byte array.
  - `file_read_str(filename, arena, error)`: Reads the entire file into a string.
  - `file_read_utf8(filename, arena, error)`: Reads the entire file into UTF-8 format.

- **Writing Files**:
  - `file_write(filename, bytes, error)`: Writes byte data to a file.
  - `file_write_str(filename, content, error)`: Writes a string to a file.
  - `file_write_utf8(filename, content, error)`: Writes UTF-8 formatted data to a file.

- **File Management**:
  - `file_open(filename, mode, error)`: Opens a file with the specified mode.
  - `file_close(file, error)`: Closes an open file.
  - `file_rename(old_name, new_name, error)`: Renames a file.
  - `file_remove(filename, error)`: Removes a file.
  - `file_exists(filename)`: Checks if a file exists.

## Usage Example

```c
Arena arena = {0};
Error error = ErrNew;
Str content = file_read_str(STR("filename.txt"), &arena, &error);
error_context(&error, {
    error_raise();
});
arena_free(&arena);
```

## [io.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/os/io.h)
## Functions

- **Output**:
  - `io_write(file, bytes, error)`: Writes byte data to a file or stream.
    - `file`: The file or stream to write to (e.g., `stdout`).
    - `bytes`: The byte data to write.
    - `error`: A pointer to an `Error` struct for error handling.

- **Input**:
  - `io_read(file, size, buffer, error)`: Reads a specified amount of byte data from a file or stream into a buffer.
  - `io_read_line(file, size, buffer, error)`: Reads a line of text from a file or stream into a buffer.
  - `input(prefix)`: Displays a prompt and reads a line of text from standard input.

## Usage Example

```c
Error e = ErrNew;
io_write(stdout, BYTES_STR("Hello, World"), &e);
error_context(&e, { error_raise(); });

Str ret = input(STR(":> "));
printf("input: '"STR_FMT"'\n", STR_ARG(ret));
```

Output when running the example:
```console
:> name
input: 'name'
```


## [os.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/os/os.h)
## Functions

- **Environment Variables**:
  - `os_getenv(env, error)`: Retrieves the value of an environment variable.
    - `env`: The name of the environment variable.
    - `error`: A pointer to an `Error` struct for error handling.

- **Current Working Directory**:
  - `os_getcwd(arena)`: Returns the current working directory, allocating memory from the specified `Arena`.
  - `os_chdir(path)`: Changes the current working directory to the specified path.

## Usage Example

```c
Error error = ErrNew;
Str cwd = os_getcwd(&arena);
Str home = os_getenv("HOME", &error);
error_context(&error, {
	error_raise();
});

printf("Current working directory: %s\n", cwd);
printf("Home directory: %s\n", home);
```

## Type

## [byte.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/type/byte.h)
## Features and Functions

- **Creating Byte Arrays**:
  - `BYTES`: Create a byte array from byte literals.
  - `BYTES_STR`: Create a byte array from a string literal.

- **Creating from Pointer and Size**:
  - `bytes_from_parts(size, data)`: Creates a byte array from given size and data pointer.

- **Copying Byte Arrays**:
  - `bytes_copy(bytes, arena)`: Performs a deep copy of a byte array, allocating memory from the specified arena.

- **Slicing and Taking Bytes**:
  - `bytes_slice(bytes, idx1, idx2)`: Returns a new byte array that is a slice from the original, from `idx1` to `idx2`.
  - `bytes_take(bytes, count)`: Modifies the original byte array to keep only the first `count` bytes, returning the removed portion.

- **Checking Equality**:
  - `bytes_eq(b1, b2)`: Checks if two byte arrays are equal.

- **Hexadecimal Conversion**:
  - `bytes_to_hex(bytes, arena)`: Converts a byte array into a hexadecimal string representation, using memory from the arena.
  - `bytes_from_hex(s, arena)`: Creates a byte array from a hexadecimal string, allocating memory from the arena.

## Usage
Create new Bytes from a literal.
```c
Bytes bytes = BYTES(0xff, 0x11);
Bytes bytes_str = BYTES_STR("Bytes from a string");
```

Or from a pointer with size.
```c
int a = 69;
Bytes bytes = bytes_from_parts(sizeof(a), &a);
```

if you need to take ownership of the bytes you can copy it.
```c
int a = 69;
Bytes bytes = bytes_from_parts(sizeof(a), &a);

Arena arena = {0};
Bytes owned_bytes = bytes_copy(bytes, &arena);
```

## [char.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/type/char.h)
## Character Classification Functions

- `c_is_alnum(c)`: Checks if a character is alphanumeric.
- `c_is_alpha(c)`: Checks if a character is alphabetic.
- `c_is_lower(c)`: Checks if a character is lowercase.
- `c_is_upper(c)`: Checks if a character is uppercase.
- `c_is_space(c)`: Checks if a character is a whitespace character.
- `c_is_cntrl(c)`: Checks if a character is a control character.
- `c_is_print(c)`: Checks if a character is printable.
- `c_is_graph(c)`: Checks if a character has a graphical representation.
- `c_is_blank(c)`: Checks if a character is blank (space or tab).
- `c_is_punct(c)`: Checks if a character is punctuation.
- `c_is_digit(c)`: Checks if a character is a digit.
- `c_is_xdigit(c)`: Checks if a character is a hexadecimal digit.

## Character Conversion Functions

- `c_to_lower(c)`: Converts a character to lowercase.
- `c_to_upper(c)`: Converts a character to uppercase.
- `c_to_u8(c)`: Converts a character to an unsigned 8-bit integer.
- `c_hex_to_u8(c)`: Converts a hexadecimal character to an unsigned 8-bit
integer.
- `c_u8_to_c(d)`: Converts an unsigned 8-bit integer to a character.
- `c_u8_to_hex(d)`: Converts an unsigned 8-bit integer to a hexadecimal
character (lowercase).
- `c_u8_to_HEX(d)`: Converts an unsigned 8-bit integer to a hexadecimal
character (uppercase).

## [float.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/type/float.h)
## Functions

These functions are available for `f32` and `f64`.

- `f32_eq(a, b)`: Tests if two `f32` are equal.
- `f32_isnan(value)`: Tests if `f32` is `nan`.
- `f32_abs(value)`: Returns the absolute value.
- `f32_min(a, b)`: Returns the min of `a` or `b`.
- `f32_max(a, b)` Returns the max of `a` or b.
- `f32_clamp(min, max, value)`: Clamps a `f32` between `min` and `max`.
- `f32_lerp(min, max, value)`: Linear interpolation between `min` and `max`.
- `f32_rad(deg)`: Converts degrees to radians.
- `f32_deg(rad)`: Converts radians to degrees.

## [integer.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/type/integer.h)
## Bit Operations

- `T_reverse_bits(T value)`: Reverses the bits in `value`.
- `T_leading_ones(T value)`: Counts the number of leading ones in `value`.
- `T_trailing_ones(T value)`: Counts the number of trailing ones in `value`.
- `T_leading_zeros(T value)`: Counts the number of leading zeros in `value`.
- `T_trailing_zeros(T value)`: Counts the number of trailing zeros in `value`.
- `T_count_zeros(T value)`: Counts the number of zeros in `value`.
- `T_count_ones(T value)`: Counts the number of ones in `value`.

## Byte Order Operations

- `T_swap_bytes(T value)`: Swaps the byte order of `value`.
- `T_to_be(T value)`: Converts `value` to big endian format.
- `T_from_be(T value)`: Converts `value` from big endian format.
- `T_to_le(T value)`: Converts `value` to little endian format.
- `T_from_le(T value)`: Converts `value` from little endian format.
- `T_to_ne_bytes(T value)`: Converts `value` to native endian format.

## Math Operations

Basic math operations including max, min, and clamp are provided.

- `T_max(T a, T b)`: Returns the maximum of `a` and `b`.
- `T_min(T a, T b)`: Returns the minimum of `a` and `b`.
- `T_clamp(T min, T max, T value)`: Clamps `value` between `min` and `max`.

## Utilities

- `T_hash(T value)`: Generates a hash for `value`.
- `T_swap(T *v1, T *v2)`: Swaps the values of `v1` and `v2`.
- `T_compare_lt(T a, T b)`: Compares `a` and `b` for less than.
- `T_compare_gt(T a, T b)`: Compares `a` and `b` for greater than.
- `T_compare_qsort(CmpOrdering ordering)`: Provides a comparison function
suitable for `qsort`.


## [string.h](https://github.com/Code-Nycticebus/clib/blob/main/src/clib/type/string.h)
## Features and Functions
- **String Creation and Printing**:
  - `STR("Hello World")`: Create a new string.
  - `printf(STR_FMT"\\n", STR_ARG(str))`: Print strings using macros.

- **String Manipulation**:
  - `str_lower(str, &arena)`, `str_upper(str, &arena)`: Convert to lower or upper case.
  - `str_append(str, suffix, &arena)`, `str_prepend(str, prefix, &arena)`: Append or prepend strings.
  - `str_wrap(str, wrap, &arena)`: Wrap a string.
  - `str_join(sep, count, strs, &arena)`: Join strings with a separator.

- **String Trimming and Slicing**:
  - `str_trim(str)`, `str_trim_left(str)`, `str_trim_right(str)`: Trim whitespace.
  - `str_chop_by_delim(str, delim)`, `str_try_chop_by_delim(str, delim, &chunk)`: Chop strings by delimiter.
  - `str_substring(str, start, end)`: Extract a substring.

- **String Comparison and Search**:
  - `str_eq(s1, s2)`, `str_eq_ignorecase(s1, s2)`: Check string equality.
  - `str_startswith(str, prefix)`, `str_endswith(str, suffix)`: Check prefixes/suffixes.
  - `str_contains(haystack, needle)`: Check if string contains a substring.

- **Conversion and Utility**:
  - `str_to_u64(str)`, `str_u64(n, &arena)`: Convert between strings and unsigned 64-bit integers.
  - `str_hash(str)`: Generate a hash value for a string.

## Usage Example

```c
Arena arena = {0};
Str greeting = STR("Hello World");
Str lower = str_lower(greeting, &arena);
printf(STR_FMT"\\n", STR_ARG(lower));
```

