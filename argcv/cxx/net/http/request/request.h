/* Copyright 2016 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================
Original Source: https://goo.gl/kPCbDH
  aka: tensorflow/core/platform/cloud/curl_http_request.h@tensorflow

Revised By: Yu Jing
*/
#ifndef ARGCV_CXX_NET_HTTP_REQUEST_REQUEST_H_
#define ARGCV_CXX_NET_HTTP_REQUEST_REQUEST_H_

#include <curl/curl.h>

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "argcv/cxx/env/env.h"
#include "argcv/cxx/status/errors.h"
#include "argcv/cxx/status/status.h"
#include "argcv/cxx/str/str_slice.h"
#include "glog/logging.h"

namespace argcv {

using ::argcv::Env;
using ::argcv::Status;
using ::argcv::StrSlice;

/**
 * Response Section
 **/
class Response {
 public:
  explicit Response(Env* env, Status status, uint16_t code) noexcept
      : env_(env), status_(status), code_(code) {
    response_buffer_.reserve(CURL_MAX_WRITE_SIZE);
  }

  explicit Response(Env* env) noexcept
      : Response(env, ::argcv::errors::Unavailable("Unavailable"), 0) {}

  void ClearBuffer() noexcept { response_buffer_.clear(); }

  std::string Body() noexcept {
    return std::string(&response_buffer_[0], response_buffer_.size());
  }

  bool GetHeader(const std::string& key, std::string* value) noexcept {
    if (response_headers_.find(key) == response_headers_.end()) {
      return false;
    } else {
      *value = response_headers_[key];
      return true;
    }
  }

  std::string ToString() const noexcept {
    std::string s(status_.ToString());
    s += "\n";
    s += std::string(&response_buffer_[0], response_buffer_.size());
    return s;
  }

  /// A write callback in the form which can be accepted by libcurl.
  static size_t WriteCallback(const void* ptr, size_t size, size_t nmemb,
                              void* userdata);

  /// A header callback in the form which can be accepted by libcurl.
  static size_t HeaderCallback(const void* ptr, size_t size, size_t nmemb,
                               void* this_object);
  /// A progress meter callback in the form which can be accepted by libcurl.
  static int ProgressCallback(void* this_object, curl_off_t dltotal,
                              curl_off_t dlnow, curl_off_t ultotal,
                              curl_off_t ulnow);

  Status& status() { return status_; }
  uint16_t& code() { return code_; }
  std::unordered_map<std::string, std::string>& headers() {
    return response_headers_;
  }

 private:
  Env* env_;
  Status status_;
  uint16_t code_;
  std::vector<char> response_buffer_;
  std::unordered_map<std::string, std::string> response_headers_;
  // The timestamp of the last activity related to the request execution, in
  // seconds since epoch.
  uint64_t last_progress_timestamp_ = 0;
  // The last progress in terms of bytes transmitted.
  curl_off_t last_progress_bytes_ = 0;
};

enum Method {
  kHead,  // https://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html
  kGet,   // using get in default
  kPost,
  kPut,
  kPatch,
  kDelete,
  kOptions,
  // kCopy,
  // kLink,
  // kUnlink,
  // kPurge,
  // kLock,
  // kUnlock,
  // kPropfind,
  // kView,
  // kConnect
};

class RequestBody {
 public:
  virtual void Reset() noexcept = 0;
  virtual size_t Copy(char* buf, size_t length) noexcept = 0;
  virtual size_t size() noexcept = 0;
};

// A String Based Instance
class PlainRequestBody : public RequestBody {
 public:
  // init
  PlainRequestBody() noexcept : PlainRequestBody("") {}
  explicit PlainRequestBody(const std::string& data) noexcept
      : RequestBody(), post_body_buffer_(data), post_body_read_(0) {}

  // copy
  PlainRequestBody(const PlainRequestBody& rhs) noexcept
      : post_body_buffer_(rhs.post_body_buffer_),
        post_body_read_(rhs.post_body_read_) {}

  // move
  PlainRequestBody(const PlainRequestBody&& rhs) noexcept
      : post_body_buffer_(std::move(rhs.post_body_buffer_)),
        post_body_read_(rhs.post_body_read_) {}

  virtual ~PlainRequestBody() {}

  void Reset() noexcept { post_body_read_ = 0; }

  size_t Copy(char* buf, size_t length) noexcept {
    // LOG(INFO) << "Copy::Check Size" << post_body_read_
    //           << " <= " << post_body_buffer_.size() << " ?";
    CHECK(post_body_read_ <= post_body_buffer_.size());
    const size_t bytes_to_copy =
        std::min(length, post_body_buffer_.size() - post_body_read_);
    memcpy(buf, post_body_buffer_.data() + post_body_read_, bytes_to_copy);
    // LOG(INFO) << "PlainRequestBody::Copy " << bytes_to_copy;
    post_body_read_ += bytes_to_copy;
    return bytes_to_copy;
  }

  size_t size() noexcept { return post_body_buffer_.size(); }

 private:
  StrSlice post_body_buffer_;
  size_t post_body_read_ = 0;
};

/**
 * Request
 */
class Request {
 public:
  // init
  Request() noexcept : Request(Env::Default(), "") {}
  explicit Request(const std::string& url) noexcept
      : Request(Env::Default(), url) {}
  explicit Request(Env* env, const std::string& url) noexcept
      : env_(env), url_(url), method_(Method::kGet) {}
  // copy
  Request(const Request& rhs) noexcept
      : env_(rhs.env_),
        url_(rhs.url_),
        method_(rhs.method_),
        params_(rhs.params_),
        headers_(rhs.headers_),
        body_(rhs.body_) {}
  Request(const Request&& rhs) noexcept
      : env_(rhs.env_),
        url_(std::move(rhs.url_)),
        method_(rhs.method_),
        params_(std::move(rhs.params_)),
        headers_(std::move(rhs.headers_)),
        body_(std::move(rhs.body_)) {}

  Request& Url(const std::string& url) noexcept {
    url_ = url;
    return *this;
  }

  Request& SetMethod(Method method) noexcept {
    method_ = method;
    return *this;
  }

  Request& SetHeader(const std::string& key,
                     const std::string& value) noexcept {
    headers_[key] = value;
    return *this;
  }

  Request& SetParam(const std::string& key, const std::string& value) noexcept {
    params_[key] = value;
    return *this;
  }

  Request& SetBody(std::shared_ptr<RequestBody> body) noexcept {
    body_ = body;
    return *this;
  }

  Request& SetBodyString(const std::string& str) noexcept {
    body_ = std::shared_ptr<RequestBody>(new PlainRequestBody(str));
    return *this;
  }

  Request& SetRange(uint64_t start, uint64_t end) {
    return SetRangeStart(start).SetRangeEnd(end);
  }

  Request& SetRangeStart(uint64_t value) {
    std::shared_ptr<uint64_t> svalue(new uint64_t);
    *svalue = value;
    range_start_ = svalue;
    return *this;
  }

  Request& SetRangeEnd(uint64_t value) {
    std::shared_ptr<uint64_t> svalue(new uint64_t);
    *svalue = value;
    range_end_ = svalue;
    return *this;
  }

  Request& SetAuthBearerHeader(const std::string& auth_token) noexcept {
    return auth_token.empty()
               ? *this
               : SetHeader("Authorization",
                           StrSlice("Bearer ", auth_token).ToString());
  }

  Request& SetProxy(const std::string& proxy) noexcept {
    std::shared_ptr<std::string> sproxy(new std::string(proxy));
    proxy_ = sproxy;
    return *this;
  }

  Response Send() noexcept;

  Response Head() noexcept { return SetMethod(Method::kHead).Send(); }

  Response Get() noexcept { return SetMethod(Method::kGet).Send(); }

  Response Post() noexcept { return SetMethod(Method::kPost).Send(); }

  Response Put() noexcept { return SetMethod(Method::kPut).Send(); }

  Response Patch() noexcept { return SetMethod(Method::kPatch).Send(); }

  Response Delete() noexcept { return SetMethod(Method::kDelete).Send(); }

  Response Options() noexcept { return SetMethod(Method::kOptions).Send(); }

  std::string EscapeString(const std::string& str) noexcept;

  // Request& Range(uint64_t start, uint64_t end) {
  //   libcurl_->curl_easy_setopt(
  //       curl_, CURLOPT_RANGE,
  //       StrSlice(std::to_string(start), "-", std::to_string(end))
  //           .ToString()
  //           .c_str());
  //   return Status::OK();
  // }

 private:
  Env* env_;
  std::string url_;
  Method method_;
  std::map<std::string, std::string> params_;
  std::map<std::string, std::string> headers_;
  std::shared_ptr<RequestBody> body_;
  std::shared_ptr<uint64_t> range_start_;
  std::shared_ptr<uint64_t> range_end_;
  // https://curl.haxx.se/libcurl/c/CURLOPT_PROXY.html
  std::shared_ptr<std::string> proxy_;

  static size_t ReadCallback(char* buf, size_t size, size_t nmemb,
                             void* userdata);
};

}  // namespace argcv

#endif  // ARGCV_CXX_NET_HTTP_REQUEST_REQUEST_H_
