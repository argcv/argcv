// Copyright 2021 Yu Jing
#ifndef ARGCV_IO_FILE_H_
#define ARGCV_IO_FILE_H_

#include <stdio.h>
#include <time.h>

#include <string>

#include "argcv/base/macros.h"
#include "argcv/base/types.h"

namespace argcv {

namespace io {

/// Defined a set of FileMode
///
/// See: https://golang.org/pkg/os/#FileMode
enum FileMode : uint32 {
  kModeDir = static_cast<uint32>(1) << (32 - 1),        //!< d: is a directory
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

/// \brief A FileInfo describes a file and is returned by Stat and Lstat.
class FileInfo {
 public:
  bool IsDir() const noexcept { return mode_ & kModeDir; }

  bool IsRegular() const noexcept { return 0 == (mode_ & kModeType); }

  FileMode Perm() const noexcept {
    return static_cast<FileMode>(mode_ & kModePerm);
  }

  SETTER_GETTER(FileInfo, string, name)
  SETTER_GETTER(FileInfo, size_t, size)
  SETTER_GETTER(FileInfo, FileMode, mode)
  SETTER_GETTER(FileInfo, time_t, mode_time)

 private:
  string name_;
  size_t size_;
  FileMode mode_;
  time_t mode_time_;
};

// TODO(yu): Not completed yet
class File {
 public:
  FILE* underlying_ref() noexcept { return f_; }

 private:
  FILE* f_;
};

}  // namespace io

}  // namespace argcv

#endif  // ARGCV_IO_FILE_H_
