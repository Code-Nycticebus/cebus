#ifndef __CLIB_OPTION_H__
#define __CLIB_OPTION_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* clang-format off */
#define OPTION_X_DO(DO)          \
  DO(OPTION_PTR, ptr, void *)    \
  DO(OPTION_U64, u64, uint64_t)  \
  DO(OPTION_I64, i64, int64_t)

/* clang-format on */

#define OPTION_X_NONE
#define OPTION_DEFINE_ENUM(enum, _id, _T) enum,
#define OPTION_DEFINE_TYPE(_enum, id, T) T id;
#define OPTION_DECL_SOME(_enum, id, T) Option option_some_##id(T value);
#define OPTION_DECL_UNWRAP(_enum, id, T)                                       \
  T _option_unwrap_##id(Option o, const char *file, int line);
#define OPTION_DECL_EXPECT(_enum, id, T)                                       \
  T _option_expect_##id(Option o, const char *msg, const char *file, int line);
#define OPTION_DECL_MAP(_enum, id, T)                                          \
  Option option_map_##id(Option o, T (*map)(T));
#define OPTION_DECL_DEFAULT(_enum, id, T)                                      \
  T option_default_##id(Option o, T def);

typedef enum { OPTION_NONE, OPTION_X_DO(OPTION_DEFINE_ENUM) } OptionTypes;

typedef struct {
  OptionTypes type;
  union {
    OPTION_X_DO(OPTION_DEFINE_TYPE)
  } val;
} Option;

#define option_propagate(o)                                                    \
  do {                                                                         \
    if (option_is_none(o)) {                                                   \
      return o;                                                                \
    }                                                                          \
  } while (0)

bool option_is_some(Option o);
bool option_is_none(Option o);

Option option_none(void);
OPTION_X_DO(OPTION_DECL_SOME)
OPTION_X_DO(OPTION_DECL_UNWRAP)
OPTION_X_DO(OPTION_DECL_EXPECT)
OPTION_X_DO(OPTION_DECL_MAP)
OPTION_X_DO(OPTION_DECL_DEFAULT)

#define option_unwrap_i64(o) _option_unwrap_i64(o, __FILE__, __LINE__);
#define option_unwrap_u64(o) _option_unwrap_u64(o, __FILE__, __LINE__);
#define option_unwrap_ptr(o) _option_unwrap_ptr(o, __FILE__, __LINE__);

#define option_expect_i64(o, msg)                                              \
  _option_expect_i64(o, msg, __FILE__, __LINE__);
#define option_expect_u64(o, msg)                                              \
  _option_expect_u64(o, msg, __FILE__, __LINE__);
#define option_expect_ptr(o, msg)                                              \
  _option_expect_ptr(o, msg, __FILE__, __LINE__);

#endif // !__CLIB_OPTION_H__
