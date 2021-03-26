// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_PATH_PATH_H_
#define ARGCV_CXX_PATH_PATH_H_

#include <initializer_list>
#include <string>

#include "argcv/cxx/base/types.h"

// #include "glog/logging.h"

namespace argcv {
namespace path {

namespace internal {
class PathBuff {
 public:
  PathBuff() noexcept : PathBuff("", 0) {}

  explicit PathBuff(const string_view &data) noexcept
      : PathBuff(data, data.length()) {}

  explicit PathBuff(const string_view &data, size_t len) noexcept;

  virtual ~PathBuff() noexcept { delete[] buff_; }

  size_t W() noexcept { return sz_; }

  size_t Backspace() noexcept;

  // Warning: There is NO out_of_range check here
  char Index(int idx) noexcept { return buff_[idx]; }

  // Warning: Didn't Checked situation when sz_ == 0
  char Last() noexcept { return buff_[sz_ - 1]; }

  PathBuff &Append(char c) noexcept;

  string ToString() const noexcept { return string(buff_, sz_); }

  string_view ToView() const noexcept { return string_view(buff_, sz_); }

 private:
  size_t sz_;
  size_t sz_buff_;
  char *buff_;
};

string JoinPathImpl(std::initializer_list<const string_view> paths) noexcept;
}  // namespace internal

// Code Ref: https://golang.org/src/path/path.go
// See Also: https://9p.io/sys/doc/lexnames.html
//
// Clean returns the shortest path name equivalent to path
// by purely lexical processing. It applies the following rules
// iteratively until no further processing can be done:
//
//  1. Replace multiple slashes with a single slash.
//  2. Eliminate each . path name element (the current directory).
//  3. Eliminate each inner .. path name element (the parent directory)
//     along with the non-.. element that precedes it.
//  4. Eliminate .. elements that begin a rooted path:
//     that is, replace "/.." by "/" at the beginning of a path.
//
// The returned path ends in a slash only if it is the root "/".
//
// If the result of this process is an empty string, Clean
// returns the string ".".
inline string CleanPath(string_view in) noexcept {
  if (in == "") {
    return ".";
  }

  // Invariants:
  //  reading from path; r is index of next byte to process.
  //  writing to buf; w is index of next byte to write.
  //  dotdot is index in buf where .. must stop, either because
  //  it is the leading slash or it is a leading ../../.. prefix.
  bool rooted = in[0] == '/';
  internal::PathBuff out(rooted ? "/" : "");
  size_t r = 0;
  size_t dotdot = 0;
  size_t n = in.length();
  if (rooted) {
    r++;
    dotdot++;
  }
  while (r < n) {
    // current character
    char c = in[r];
    switch (c) {
      case '/':
        // empty path element
        r++;
        break;
      case '.':
        if (r + 1 <= n) {
          if (r + 1 == n || in[r + 1] == '/') {
            // . element
            r++;
            break;  // break switch(c)
          } else if (r + 2 <= n && in[r + 1] == '.' &&
                     (r + 2 == n || in[r + 2] == '/')) {
            // .. element: remove to last /
            r += 2;
            if (out.W() > dotdot) {
              out.Backspace();
              while (out.W() > dotdot && out.Index(out.W()) != '/') {
                out.Backspace();
              }
            } else if (!rooted) {
              if (out.W() > 0) {
                out.Append('/');
              }
              out.Append('.');
              out.Append('.');
              dotdot = out.W();
            }
            break;
          } else {
            // NO break, to default
          }
        } else {
          // NO break, to default
        }
        // if not eligible, no break, execute default
      default:
        // real path element.
        // add slash if needed
        if ((rooted && out.W() != 1) || (!rooted && out.W() != 0)) {
          out.Append('/');
        }
        // copy element
        while (r < n && in[r] != '/') {
          out.Append(in[r]);
          r++;
        }
    }
  }

  if (out.W() == 0) {
    return ".";
  }

  return out.ToString();
}

/// JoinPath joins any number of path elements into a single path, adding a
/// separating slash if necessary. The result is Cleaned; in particular,
/// all empty strings are ignored.
template <typename... T>
string JoinPath(const T &...args) noexcept {
  return internal::JoinPathImpl({args...});
}

/// SplitFilename splits path immediately following the final slash,
/// separating it into a directory and file name component.
///
/// If there is no slash in path, Split returns an empty dir and
/// file set to path.
/// The returned values have the property that in = dir+file.
inline void SplitFilename(string_view in, string_view *dir,
                          string_view *file) noexcept {
  // http://www.cplusplus.com/reference/string/string/find_last_of/
  std::size_t found = in.find_last_of("/");
  // LOG(INFO) << "SplitFilename...[" << in << "]=> [" << found << "]"
  //           << "check" << (found == string_view::npos);
  if (dir) {
    if (found == string_view::npos) {
      *dir = "";
    } else {
      // LOG(INFO) << "SplitFilename...[" << in << "]=> [" << found << "]"
      //           << "check" << (found == string_view::npos) << " ~~~ "
      //           << in.substr(0, found + 1);
      *dir = in.substr(0, found + 1);  //
    }
  }
  if (file) {
    *file = in.substr(found + 1);
  }
}

/// IsAbsPath detecteted whether to be a absolute path
inline bool IsAbsPath(string_view in) noexcept {
  return in.length() > 0 && in[0] == '/';
}

/// Dirname returns all but the last element of path, typically the path's
/// directory.
///
/// After dropping the final element using Split, the path is Cleaned
/// and trailing slashes are removed. If the path is empty, Dirname returns ".".
/// If the path consists entirely of slashes followed by non-slash bytes,
/// Dirname returns a single slash. In any other case, the returned path does
/// not end in a slash.
inline string Dirname(string_view in) noexcept {
  string_view dir;
  SplitFilename(in, &dir, nullptr);
  // LOG(INFO) << "Dirname... [" << in << "] => [" << dir << "]";
  return CleanPath(dir);
}

/// Basename returns the last element of path.
/// Trailing slashes are removed before extracting the last element.
/// If the path is empty, Base returns ".".
/// If the path consists entirely of slashes, Base returns "/".
inline string Basename(string_view in) noexcept {
  if (in == "") {
    return ".";
  }
  internal::PathBuff out(in);
  // Strip trailing slashes.
  while (out.W() > 0 && out.Last() == '/') {
    out.Backspace();
  }

  // string oin = out.ToString();
  string_view elem;
  // SplitFilename(out.ToString(), nullptr, &elem);
  SplitFilename(out.ToView(), nullptr, &elem);
  // If empty now, it had only slashes.
  if (elem == "") {
    return "/";
  }
  return string(elem);
}

}  // namespace path

}  // namespace argcv

#endif  // ARGCV_CXX_PATH_PATH_H_
