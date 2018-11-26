/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Yu Jing
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
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
