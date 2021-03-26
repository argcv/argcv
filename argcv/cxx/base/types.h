// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_BASE_TYPES_H_
#define ARGCV_CXX_BASE_TYPES_H_

#include <atomic>
#include <cstdint>
#include <deque>
#include <functional>  // std::equal_to<>
#include <map>
#include <memory>  // std::allocator<>
#include <queue>   // queue & priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "absl/strings/string_view.h"

/*! \brief Type definitions

    Some predefined types to make code shorter & friendly
*/
namespace argcv {

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef char int4;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
typedef unsigned char uint4;
typedef long double float128;
typedef double float64;
typedef float float32;

// typedef std::atomic<T> as atomic<T>
template <typename T>
using atomic = std::atomic<T>;

template <class T, class Allocator = std::allocator<T>>
using deque = std::deque<T, Allocator>;

template <class Key, class T, class Compare = std::less<Key>>
using map = std::map<Key, T, Compare>;

template <class T, class Container = deque<T>>
using queue = std::queue<T, Container>;

template <class Value, class Compare = std::less<Value>>
using set = std::set<Value, Compare>;

template <class Value, class Compare = std::less<Value>>
using multiset = std::multiset<Value, Compare>;

template <class T, class Container = deque<T>>
using stack = std::stack<T, Container>;

template <class Key, class T, class Hash = std::hash<Key>,
          class Pred = std::equal_to<Key>>
using unordered_map = std::unordered_map<Key, T, Hash, Pred>;

template <class T, class Hash = std::hash<T>, class Pred = std::equal_to<T>>
using unordered_set = std::unordered_set<T, Hash, Pred>;

template <class T, class Hash = std::hash<T>, class Pred = std::equal_to<T>>
using unordered_multiset = std::unordered_multiset<T, Hash, Pred>;

template <class T>
using vector = std::vector<T>;

template <class T, class Container = vector<T>,
          class Compare = std::less<typename Container::value_type>>
using priority_queue = std::priority_queue<T, Container, Compare>;

// This `absl::string_view` abstraction is designed to be a drop-in
// replacement for the C++17 `std::string_view` abstraction.
using string_view = absl::string_view;

using string = std::basic_string<char>;

}  // namespace argcv

#endif  // ARGCV_CXX_BASE_TYPES_H_
