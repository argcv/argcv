// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_BASE_TYPES_H_
#define ARGCV_CXX_BASE_TYPES_H_

#include <cstdint>

#include <atomic>
#include <deque>
#include <map>
#include <queue>  // queue & priority_queue
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "absl/strings/string_view.h"

/*! \brief Type definitions

    Some predefined types to make code shorter & friendly
*/
namespace argcv {

typedef int64_t i64t;
typedef int32_t i32t;
typedef int16_t i16t;
typedef int8_t i8t;
typedef char i4t;
typedef uint64_t u64t;
typedef uint32_t u32t;
typedef uint16_t u16t;
typedef uint8_t u8t;
typedef unsigned char u4t;
typedef long double f128t;
typedef double f64t;
typedef float f32t;

// typedef std::atomic<T> as atomic<T>
// template <typename T>
// using atomic = std::atomic<T>;
using std::atomic;
using std::deque;
using std::map;
using std::priority_queue;
using std::queue;
using std::set;
using std::stack;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// This `absl::string_view` abstraction is designed to be a drop-in
// replacement for the C++17 `std::string_view` abstraction.
using string_view = absl::string_view;

}  // namespace argcv

#endif  // ARGCV_CXX_BASE_TYPES_H_
