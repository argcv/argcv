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
#ifndef ARGCV_CXX_NET_HTTP_REQUEST_CCURL_H_
#define ARGCV_CXX_NET_HTTP_REQUEST_CCURL_H_

#include <curl/curl.h>

#include <memory>  // std::shared_ptr
#include <mutex>   // NOLINT(build/c++11)  std::once_flag
#include <string>

#include "argcv/cxx/str/str.h"

namespace argcv {

static std::once_flag ccurl_init;

////////////////////////////////////////////////////////////////////////////////////////
//
// CCurl, a wrapper
//
////////////////////////////////////////////////////////////////////////////////////////
class CCurl {
 public:
  ~CCurl() {
    if (headers_) {
      ::curl_slist_free_all(headers_);
    }
    ::curl_easy_cleanup(hdr_);
  }

  static std::shared_ptr<CCurl> Load() {
    std::call_once(ccurl_init, []() {
      // If you did not already call curl_global_init,
      // curl_easy_init does it automatically. This may be lethal in
      // multi-threaded cases, since curl_global_init is not thread-safe, and it
      // may result in resource problems because there is no corresponding
      // cleanup
      curl_global_init(CURL_GLOBAL_ALL);
    });
    std::shared_ptr<CCurl> c(new CCurl);
    return c;
  }

  CURLcode SetOpt(CURLoption option, uint64_t param) {
    return ::curl_easy_setopt(hdr_, option, param);
  }

  CURLcode SetOpt(CURLoption option, const std::string& param) {
    return ::curl_easy_setopt(hdr_, option, param.c_str());
  }

  CURLcode SetOpt(CURLoption option, const char* param) {
    return ::curl_easy_setopt(hdr_, option, param);
  }

  CURLcode SetOpt(CURLoption option, void* param) {
    return ::curl_easy_setopt(hdr_, option, param);
  }

  CURLcode SetOpt(CURLoption option,
                  size_t (*param)(void*, size_t, size_t, void*)) {
    return ::curl_easy_setopt(hdr_, option, param);
  }

  CURLcode SetOpt(CURLoption option,
                  size_t (*param)(char*, size_t, size_t, void*)) {
    return ::curl_easy_setopt(hdr_, option, param);
  }

  CURLcode SetOpt(CURLoption option,
                  size_t (*param)(const void*, size_t, size_t, void*)) {
    return ::curl_easy_setopt(hdr_, option, param);
  }

  CURLcode SetOpt(CURLoption option,
                  int (*param)(void* clientp, curl_off_t dltotal,
                               curl_off_t dlnow, curl_off_t ultotal,
                               curl_off_t ulnow)) {
    return ::curl_easy_setopt(hdr_, option, param);
  }

  CURLcode Perform() {
    if (headers_) {
      ::curl_easy_setopt(hdr_, CURLOPT_HTTPHEADER, headers_);
    }
    return ::curl_easy_perform(hdr_);
  }

  CURLcode GetInfo(CURLINFO info, uint64_t* value) {
    return ::curl_easy_getinfo(hdr_, info, value);
  }

  CURLcode GetInfo(CURLINFO info, double* value) {
    return ::curl_easy_getinfo(hdr_, info, value);
  }

  char* Escape(const char* str, int length) {
    return ::curl_easy_escape(hdr_, str, length);
  }

  std::string Escape(const std::string& str, int length) {
    char* out_char_str = ::curl_easy_escape(hdr_, str.c_str(), length);
    std::string out_str(out_char_str);
    // curl_free reclaims memory that has been obtained through a libcurl call.
    // Use curl_free instead of free() to avoid anomalies that can result
    // from differences in memory management between your application and
    // libcurl.
    ::curl_free(out_char_str);
    return out_str;
  }

  void PushHeader(const std::string& name, const std::string& value) {
    headers_ = ::curl_slist_append(
        headers_, ::argcv::StrSlice(name, ": ", value).ToString().c_str());
  }

 private:
  CURL* hdr_;
  curl_slist* headers_ = nullptr;

  CCurl() noexcept : hdr_(::curl_easy_init()) {}

  curl_slist* curl_slist_append(curl_slist* list, const char* str) {
    return ::curl_slist_append(list, str);
  }

  void curl_slist_free_all(curl_slist* list) {
    return ::curl_slist_free_all(list);
  }
};

}  // namespace argcv

#endif  // ARGCV_CXX_NET_HTTP_REQUEST_CCURL_H_
