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
#include "argcv/cxx/net/http/request/request.h"

#include <memory>

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace argcv {
TEST(Request, Get) {
  Request req("https://jsonplaceholder.typicode.com/posts");
  auto ret = req.SetParam("id", "1").Get();
  LOG(INFO) << "[CODE][" << std::to_string(ret.code()) << "]";
  LOG(INFO) << "[STATUS][" << ret.status() << "]";
  LOG(INFO) << "[BODY][" << ret.Body() << "]";
  for (auto& header : ret.headers()) {
    LOG(INFO) << "[HEADER][K:" << header.first << "] [V:" << header.second
              << "]";
  }
}

TEST(Request, Head) {
  Request req("https://jsonplaceholder.typicode.com/posts");
  // prevent "Connection", "keep-alive"
  auto ret = req.SetParam("id", "1").SetHeader("Connection", "close").Head();
  LOG(INFO) << "[CODE][" << std::to_string(ret.code()) << "]";
  LOG(INFO) << "[STATUS][" << ret.status() << "]";
  LOG(INFO) << "[BODY][" << ret.Body() << "]";
  for (auto& header : ret.headers()) {
    LOG(INFO) << "[HEADER][K:" << header.first << "] [V:" << header.second
              << "]";
  }
}

TEST(Request, Patch) {
  Request req("https://jsonplaceholder.typicode.com/posts/1");
  req.SetHeader("Content-Type", "application/json");
  req.SetBodyString("{\"title\":\"foo\",\"body\":\"bar\"}");
  auto ret = req.Patch();
  LOG(INFO) << "[CODE][" << std::to_string(ret.code()) << "]";
  LOG(INFO) << "[STATUS][" << ret.status() << "]";
  LOG(INFO) << "[BODY][" << ret.Body() << "]";
  std::string date;
  bool hasHeader = ret.GetHeader("Date", &date);
  if (hasHeader) {
    LOG(INFO) << "found header Date:[" << date << "]";
  } else {
    LOG(INFO) << "header Date not found";
  }
  for (auto& header : ret.headers()) {
    LOG(INFO) << "[HEADER][K:" << header.first << "] [V:" << header.second
              << "]";
  }
}

TEST(Request, Options) {
  Request req("https://jsonplaceholder.typicode.com/posts/1");
  auto ret = req.Options();
  LOG(INFO) << "[CODE][" << std::to_string(ret.code()) << "]";
  LOG(INFO) << "[STATUS][" << ret.status() << "]";
  LOG(INFO) << "[BODY][" << ret.Body() << "]";
  for (auto& header : ret.headers()) {
    LOG(INFO) << "[HEADER][K:" << header.first << "] [V:" << header.second
              << "]";
  }
}

TEST(Request, Range) {
  Request req("https://example.com");
  auto ret = req.SetRangeStart(0).SetRangeEnd(20).Get();
  LOG(INFO) << "[CODE][" << std::to_string(ret.code()) << "]";
  LOG(INFO) << "[STATUS][" << ret.status() << "]";
  LOG(INFO) << "[BODY][" << ret.Body() << "]";
  for (auto& header : ret.headers()) {
    LOG(INFO) << "[HEADER][K:" << header.first << "] [V:" << header.second
              << "]";
  }
}

// TEST(Request, Proxy) {
//   Request req("http://i.argcv.com");
//   auto ret = req.SetProxy("socks5://localhost:1080").Get();
//   LOG(INFO) << "[CODE][" << std::to_string(ret.code()) << "]";
//   LOG(INFO) << "[STATUS][" << ret.status() << "]";
//   LOG(INFO) << "[BODY][" << ret.Body() << "]";
//   for (auto& header : ret.headers()) {
//     LOG(INFO) << "[HEADER][K:" << header.first << "] [V:" << header.second
//               << "]";
//   }
// }

}  // namespace argcv
