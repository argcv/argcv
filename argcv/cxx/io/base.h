// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_IO_BASE_H_
#define ARGCV_CXX_IO_BASE_H_

#include <string>

#include "argcv/cxx/base/result.h"
#include "argcv/cxx/base/status.h"
#include "argcv/cxx/base/types.h"

namespace argcv {

namespace io {
/**
 * \brief seek whence values
 */
enum Whence {
  /// seek relative to the origin of the file
  kSeekStart = 0,
  /// seek relative to the current offset
  kSeekCurrent = 1,
  /// seek relative to the end
  kSeekEnd = 2
};

/// \brief seek to some offset
class Seeker {
 public:
  virtual ~Seeker() {}

  // return offset compare with start
  virtual Result<size_t> Seek(int64 offset, Whence whence) noexcept = 0;
};

/// \brief Write data.size() data into output stream to the end
class Writer {
 public:
  virtual ~Writer() {}

  virtual Result<size_t> Write(string_view data) noexcept = 0;
};

/// \brief Reads up to `n` bytes from the file starting at `offset`.
class Reader {
 public:
  virtual ~Reader() {}

  /// \brief Reads up to `n` bytes from the file starting at `offset`.
  ///
  /// scratch is used to store data, sizeof(scratch) should no less than
  /// n:size_t
  ///
  /// On OK returned status: `n` bytes have been stored in `*result`.
  /// On non-OK returned status: `[0..n]` bytes have been stored in `*result`.
  ///
  /// Returns `OUT_OF_RANGE` if fewer than n bytes were stored in `*result`
  /// because of EOF.
  ///
  virtual Result<size_t> Read(size_t n, string_view* result,
                              char* scratch) const noexcept = 0;
};

/// ReadSeeker is the interface that groups the basic Read and Seek methods.
class ReadSeeker : public Reader, public Seeker {
 public:
  virtual ~ReadSeeker() {}
};

/// WriteSeeker is the interface that groups the basic Write and Seek methods.
class WriteSeeker : public Writer, public Seeker {
 public:
  virtual ~WriteSeeker() {}
};

class Closer {
 public:
  virtual ~Closer() {}
  virtual Status Close() noexcept = 0;
};

/// ReadWriter is the interface that groups the basic Read and Write methods.
class ReadWriter : public Reader, public Writer {
 public:
  virtual ~ReadWriter() {}
};

/// ReadCloser is the interface that groups the basic Read and Close methods.
class ReadCloser : public Reader, public Closer {
 public:
  virtual ~ReadCloser() {}
};

/// WriteCloser is the interface that groups the basic Write and Close methods.
class WriteCloser : public Writer, public Closer {
 public:
  virtual ~WriteCloser() {}
};

/// ReadWriteCloser is the interface that groups the basic Read, Write and Close
/// methods.
class ReadWriteCloser : public Reader, public Writer, public Closer {
 public:
  virtual ~ReadWriteCloser() {}
};

/// ReadWriteSeeker is the interface that groups the basic Read, Write and Seek
/// methods.
class ReadWriteSeeker : public Reader, public Writer, public Seeker {
 public:
  virtual ~ReadWriteSeeker() {}
};

/// Write data.size() data into output stream from offset
class RandomWriter {
 public:
  virtual ~RandomWriter() {}

  virtual Result<size_t> WriteAt(uint64 offset, string_view data) const
      noexcept = 0;
};

class RandomReader {
 public:
  virtual ~RandomReader() {}
  /// \brief Reads up to `n` bytes from the file starting at `offset`.
  ///
  /// scratch is used to store data, sizeof(scratch) should no less than
  /// n:size_t
  ///
  /// On OK returned status: `n` bytes have been stored in `*result`.
  /// On non-OK returned status: `[0..n]` bytes have been stored in `*result`.
  ///
  /// Returns `OUT_OF_RANGE` if fewer than n bytes were stored in `*result`
  /// because of EOF.
  ///
  /// Safe for concurrent use by multiple threads.
  virtual Result<size_t> ReadAt(uint64 offset, size_t n, string_view* result,
                                char* scratch) const noexcept = 0;

  // sample implementation from tf
  // Result<size_t>  ReadAt(uint64 offset, size_t n, string_view* result,
  // char* scratch)  const noexcept override {
  //   Status s;
  //   char* dst = scratch;
  //   while (n > 0 && s.ok()) {
  //     ssize_t r = pread(fd_, dst, n, static_cast<off_t>(offset));
  //     if (r > 0) {
  //       dst += r;
  //       n -= r;
  //       offset += r;
  //     } else if (r == 0) {
  //       s = Status(error::OUT_OF_RANGE, "Read less bytes than requested");
  //     } else if (errno == EINTR || errno == EAGAIN) {
  //       // Retry
  //     } else {
  //       s = IOError(filename_, errno);
  //     }
  //   }
  //   *result = string_view(scratch, dst - scratch);
  //   return s;
  // }
};

/// RandomReadCloser
class RandomReadCloser : public RandomReader, public Closer {
 public:
  virtual ~RandomReadCloser() {}
};

/// RandomWriteCloser
class RandomWriteCloser : public RandomWriter, public Closer {
 public:
  virtual ~RandomWriteCloser() {}
};

/// RandomReadWriteCloser
class RandomReadWriteCloser : public RandomReader,
                              public RandomWriter,
                              public Closer {
 public:
  virtual ~RandomReadWriteCloser() {}
};

class NopCloser : public Closer {
 public:
  virtual ~NopCloser() {}

  /// Return success directly
  virtual Status Close() noexcept {
    // return ok directly
    return Status::OK();
  }
};

}  // namespace io

}  // namespace argcv

#endif  // ARGCV_CXX_IO_BASE_H_
