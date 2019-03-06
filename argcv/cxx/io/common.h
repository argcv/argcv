// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_IO_COMMON_H_
#define ARGCV_CXX_IO_COMMON_H_

namespace argcv {
/**
 * \brief seek whence values
 */
enum SeekWhence {
  /// seek relative to the origin of the file
  kSeekStart = 0,
  /// seek relative to the current offset
  kSeekCurrent = 1,
  /// seek relative to the end
  kSeekEnd = 2
};

// TODO(yu): interfaces

// class Writer {
//  public:
//   virtual void Write() const = 0;
// };

}  // namespace argcv

#endif  // ARGCV_CXX_IO_COMMON_H_
