// Copyright 2019 Yu Jing
#ifndef ARGCV_C_MACROS_H_
#define ARGCV_C_MACROS_H_

// Compiler attributes
#if (defined(__GNUC__) || defined(__APPLE__)) && !defined(SWIG)
// Compiler supports GCC-style attributes
#define ATTRIBUTE_NORETURN __attribute__((noreturn))
#define ATTRIBUTE_NOINLINE __attribute__((noinline))
#define ATTRIBUTE_ALWAYS_INLINE __inline__ __attribute__((always_inline))
#define ATTRIBUTE_UNUSED __attribute__((unused))
#define ATTRIBUTE_COLD __attribute__((cold))
#define PACKED __attribute__((packed))
#define MUST_USE_RESULT __attribute__((warn_unused_result))
#define PRINTF_ATTRIBUTE(string_index, first_to_check) \
  __attribute__((__format__(__printf__, string_index, first_to_check)))
#define SCANF_ATTRIBUTE(string_index, first_to_check) \
  __attribute__((__format__(__scanf__, string_index, first_to_check)))

#else
// Non-GCC equivalents
#define ATTRIBUTE_NORETURN
#define ATTRIBUTE_NOINLINE
#define ATTRIBUTE_ALWAYS_INLINE
#define ATTRIBUTE_UNUSED
#define ATTRIBUTE_COLD
#define MUST_USE_RESULT
#define PACKED
#define PRINTF_ATTRIBUTE(string_index, first_to_check)
#define SCANF_ATTRIBUTE(string_index, first_to_check)
#endif  // (defined(__GNUC__) || defined(__APPLE__)) && !defined(SWIG)

#endif  //  ARGCV_C_MACROS_H_
