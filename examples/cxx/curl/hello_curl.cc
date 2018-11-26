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

#include "curl/curl.h"
#include "glog/logging.h"

// const std::string kUrl = "https://example.com";
const std::string kUrl = "https://argcv.com";
static bool kSkipPeerVerification = true;
static bool kSkipHostnameVerification = true;

class MyCurl {
 public:
  MyCurl() : curl(curl_easy_init()), is_ready_(false) {}

  ~MyCurl() {
    if (curl) curl_easy_cleanup(curl);
  }

  std::string Get(std::string url) {
    if (!curl) {
      return "";
    } else {
      curl_easy_setopt(curl, CURLOPT_URL, kUrl.c_str());

      struct curl_slist *headers = nullptr;
      headers = curl_slist_append(headers, "User-Agent: argcv-bot/0.0.1");
      headers = curl_slist_append(headers, "Accept:");  // remove "Accept:"

      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

      // long timeout = 100;

      // curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
      // curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, 0);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, MyCurl::OnWrite);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)this);

      if (kSkipPeerVerification) {
        /**
         * If you want to connect to a site who isn't using a certificate that
         * is signed by one of the certs in the CA bundle you have, you can skip
         * the verification of the server's certificate. This makes the
         * connection A LOT LESS SECURE.
         *
         * If you have a CA cert for the server stored someplace else than in
         * the default bundle, then the CURLOPT_CAPATH option might come handy
         * for you.
         */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      }

      if (kSkipHostnameVerification) {
        /**
         * If the site you're connecting to uses a different host name that what
         * they have mentioned in their server certificate's commonName (or
         * subjectAltName) fields, libcurl will refuse to connect. You can skip
         * this check, but this will make the connection less secure.
         */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
      }

      // Perform the request, res will get the return code
      code_ = curl_easy_perform(curl);

      // Check for errors
      if (code_ != CURLE_OK) {
        LOG(ERROR) << "curl_easy_perform() failed: "
                   << curl_easy_strerror(code_);
        return "";
      } else {
        is_ready_ = true;
        return data_;
      }
    }
  }

  std::string GetContentType() {
    if (is_ready_) {
      char *ct;
      CURLcode resp = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
      if (CURLE_OK == resp) {
        return std::string(ct);
      } else {
        return "";
      }
    } else {
      return "";
    }
  }

  static size_t OnWrite(void *ptr, size_t sz, size_t n, void *f) {
    return static_cast<MyCurl *>(f)->Handle(ptr, sz, n);
  }

  size_t Handle(void *ptr, size_t sz, size_t n) {
    fprintf(stderr, "size: %zu, num: %zu\n", sz, n);
    LOG(INFO) << "size:" << sz << " num:" << n;
    char *cr = (char *)ptr;
    for (size_t i = 0; i < n; i++) {
      data_.append(cr + (i * sz), sz);
    }
    // LOG(INFO) << "GET: \n[" << data_ << "\n]";
    return sz * n;
  }

 private:
  CURL *curl;
  CURLcode code_;
  std::string data_;
  bool is_ready_;
};

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  MyCurl c;
  std::string s = c.Get(kUrl);
  LOG(INFO) << "get response: \n[" << s << "\n]";
  LOG(INFO) << "content type:" << c.GetContentType();

  curl_global_cleanup();
  return 0;
}
