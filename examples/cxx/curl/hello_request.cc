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

#include "argcv/cxx/net/http/request/request.h"
#include "glog/logging.h"

// const std::string kUrl = "https://example.com";
const std::string kUrl = "https://argcv.com";

using argcv::Request;

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  Request req(kUrl);
  auto ret = req.SetRangeStart(0).SetRangeEnd(20).Get();
  LOG(INFO) << "[CODE][" << std::to_string(ret.code()) << "]";
  LOG(INFO) << "[STATUS][" << ret.status() << "]";
  LOG(INFO) << "[BODY][" << ret.Body() << "]";
  LOG(INFO) << "[BODY-LENGTH][" << ret.Body().size() << "]";
  for (auto& header : ret.headers()) {
    LOG(INFO) << "[HEADER][" << header.first  //
              << "] => [" << header.second << "]";
  }

  // server: nginx/1.14.0
  return 0;
}
