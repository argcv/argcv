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
#include <cstdio>

#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

#include "./snappy.h"

#include "glog/logging.h"

using snappy::Compress;
using snappy::Uncompress;

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  std::string str_in =
      "Snappy is a compression/decompression library. "
      "It does not aim for maximum compression, or compatibility with any "
      "other compression library; instead, it aims for very high speeds and "
      "reasonable compression. For instance, compared to the fastest mode "
      "of zlib, Snappy is an order of magnitude faster for most inputs, "
      "but the resulting compressed files are anywhere from 20% to 100% "
      "bigger. (For more information, see \"Performance\", below.)";
  std::string str_co;

  LOG(INFO) << "raw data: [" << str_in << "] size: " << str_in.size();

  size_t sz = Compress(str_in.data(), str_in.size(), &str_co);

  LOG(INFO) << "compressed size : " << sz << " string size: " << str_co.size();

  std::string str_out;

  if (Uncompress(str_co.data(), sz, &str_out)) {
    LOG(INFO) << "Uncompressed, str: [" << str_out
              << "] size: " << str_out.size();
  } else {
    LOG(WARNING) << "Uncompress failed!!";
  }

  return 0;
}
