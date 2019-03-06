// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_OS_COMMOM_H_
#define ARGCV_CXX_OS_COMMOM_H_

#include <stdio.h>
#include <time.h>

#include <string>

#include "argcv/cxx/base/macros.h"
#include "argcv/cxx/base/types.h"

namespace argcv {

/// Defined a set of FileMode
///
/// See: https://golang.org/pkg/os/#FileMode
enum FileMode : uint32 {
  kModeDir = static_cast<uint32>(0x1) << (32 - 1),      //!< d: is a directory
  kModeAppend = static_cast<uint32>(1) << (32 - 2),     //!< a: append-only
  kModeExclusive = static_cast<uint32>(1) << (32 - 3),  //!< l: exclusive use

  /// T: temporary file; Plan 9 only
  kModeTemporary = static_cast<uint32>(1) << (32 - 4),
  kModeSymlink = static_cast<uint32>(1) << (32 - 5),  //!< L: symbolic link
  kModeDevice = static_cast<uint32>(1) << (32 - 6),   //!< D: device file

  /// p: named pipe (FIFO)
  kModeNamedPipe = static_cast<uint32>(1) << (32 - 7),
  kModeSocket = static_cast<uint32>(1) << (32 - 8),   //!< S: Unix domain socket
  kModeSetuid = static_cast<uint32>(1) << (32 - 9),   //!< u: setuid
  kModeSetgid = static_cast<uint32>(1) << (32 - 10),  //!< g: setgid

  /// c: Unix character device, when ModeDevice is set
  kModeCharDevice = static_cast<uint32>(1) << (32 - 11),
  kModeSticky = static_cast<uint32>(1) << (32 - 12),  //!< t: sticky
  /// ?: non-regular file; nothing else is known about this file
  kModeIrregular = static_cast<uint32>(1) << (32 - 13),

  /// Mask for the type bits. For regular files, none will be set.
  kModeType = kModeDir | kModeSymlink | kModeNamedPipe | kModeSocket |
              kModeDevice | kModeCharDevice | kModeIrregular,

  kModePerm = 0777  ///< Unix permission bits
};

/// A FileInfo describes a file and is returned by Stat and Lstat.
class FileInfo {
 public:
  bool IsDir() const noexcept { return kModeDir & mode_; }

  SETTER_GETTER(FileInfo, string, name)
  SETTER_GETTER(FileInfo, size_t, size)
  SETTER_GETTER(FileInfo, FileMode, mode)
  SETTER_GETTER(FileInfo, time_t, mode_time)

 protected:
  string name_;
  size_t size_;
  FileMode mode_;
  time_t mode_time_;
};

// TODO
class File {
 public:
  FILE* underlying_ref() noexcept { return f_; }

 private:
  FILE* f_;
};

}  // namespace argcv

#endif  // ARGCV_CXX_OS_COMMOM_H_
