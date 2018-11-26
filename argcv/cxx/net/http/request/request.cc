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
Original Source: https://goo.gl/utTaUX
  aka: tensorflow/core/platform/cloud/curl_http_request.cc@tensorflow

Revised By: Yu Jing
*/
#include "argcv/cxx/net/http/request/request.h"

#include <cstdint>
#include <memory>  // std::shared_ptr
#include <mutex>   // NOLINT(build/c++11)  std::once_flag
#include <string>

#include "argcv/cxx/env/env.h"
#include "argcv/cxx/env/macros.h"
#include "argcv/cxx/net/http/request/ccurl.h"
#include "argcv/cxx/status/errors.h"
#include "argcv/cxx/status/status.h"
#include "argcv/cxx/str/str_scan.h"
#include "argcv/cxx/str/str_slice.h"
#include "glog/logging.h"

namespace argcv {

using std::shared_ptr;

// Set to 1 to enable verbose debug output from curl.
constexpr uint64_t kVerboseOutput = 0;

// Timeout for the whole request. Set only to prevent hanging indefinitely.
constexpr uint32_t kRequestTimeoutSeconds = 3600;  // 1 hour

// Timeout for the connection phase.
constexpr uint32_t kConnectTimeoutSeconds = 120;  // 2 minutes

// The maximum period of request inactivity, after which the request
// is terminated.
constexpr uint64_t kInactivityTimeoutSeconds = 60;  // 1 minute

// AgentName
const char kAgentName[] = "ArgCV";

////////////////////////////////////////////////////////////////////////////////////////
//
// Request, Implementation of controller
//
////////////////////////////////////////////////////////////////////////////////////////

Response Request::Send() noexcept {
  // NOTE: CURL_CA_BUNDLE=/etc/ssl/certs/ca-certificates.crt is configured by
  //       default in //third_party:curl.BUILD and can be customized via an
  //       environment variable.
  Response ret(env_);
  shared_ptr<CCurl> cli = CCurl::Load();
  cli->SetOpt(CURLOPT_VERBOSE, kVerboseOutput);
  cli->SetOpt(CURLOPT_USERAGENT,
              StrSlice(kAgentName, "/", ARGCV_VERSION).ToString().c_str());

  // Do not use signals for timeouts - does not work in multi-threaded programs.
  cli->SetOpt(CURLOPT_NOSIGNAL, 1L);
  cli->SetOpt(CURLOPT_TIMEOUT, kRequestTimeoutSeconds);
  cli->SetOpt(CURLOPT_CONNECTTIMEOUT, kConnectTimeoutSeconds);
  cli->SetOpt(CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

  // Set up the progress meter.
  cli->SetOpt(CURLOPT_NOPROGRESS, (uint64_t)0L);
  cli->SetOpt(CURLOPT_XFERINFODATA, reinterpret_cast<void*>(&ret));
  cli->SetOpt(CURLOPT_XFERINFOFUNCTION, &Response::ProgressCallback);

  // LOG(INFO) << "url_:" << url_;
  StrSlice url = StrSlice(url_);
  if (params_.size() > 0) {
    url.Append("?");
    for (const auto& it : params_) {
      // LOG(INFO) << "key:" << it.first << " value:" << it.second;
      url.Append(cli->Escape(it.first, 0), "=", cli->Escape(it.second, 0), "&");
    }
  }
  // LOG(INFO) << "url:" << url.ToString();
  cli->SetOpt(CURLOPT_URL, url.ToString());

  bool with_body = false;
  // Method
  switch (method_) {
    case Method::kHead:
      cli->SetOpt(CURLOPT_CUSTOMREQUEST, "HEAD");
      break;
    case Method::kGet:
      cli->SetOpt(CURLOPT_CUSTOMREQUEST, "GET");
      break;
    case Method::kPost:
      cli->SetOpt(CURLOPT_CUSTOMREQUEST, "POST");
      with_body = true;
      break;
    case Method::kPut:
      cli->SetOpt(CURLOPT_CUSTOMREQUEST, "PUT");
      with_body = true;
      break;
    case Method::kPatch:
      cli->SetOpt(CURLOPT_CUSTOMREQUEST, "PATCH");
      with_body = true;
      break;
    case Method::kDelete:
      cli->SetOpt(CURLOPT_CUSTOMREQUEST, "DELETE");
      break;
    case Method::kOptions:
      cli->SetOpt(CURLOPT_CUSTOMREQUEST, "OPTIONS");
      break;
    default:
      ret.status() = errors::Unimplemented(
          StrSlice("This method is not implemented yet").ToString());
      return ret;
  }

  for (const auto& it : headers_) {
    // LOG(INFO) << "key:" << it.first << " value:" << it.second;
    cli->PushHeader(it.first, it.second);
  }

  if (range_start_ || range_end_) {
    StrSlice range_str;
    if (range_start_) {
      range_str.Append(std::to_string(*range_start_));
    }
    range_str.Append("-");
    if (range_end_) {
      range_str.Append(std::to_string(*range_end_));
    }
    cli->SetOpt(CURLOPT_RANGE, range_str.ToString());
  }

  if (proxy_) {
    cli->SetOpt(CURLOPT_PROXY, *proxy_);
  }

  if (with_body) {
    if (body_) {
      cli->SetOpt(CURLOPT_UPLOAD, (uint64_t)1L);
      cli->PushHeader("Content-Length", std::to_string(body_->size()).c_str());
      cli->SetOpt(CURLOPT_READDATA, reinterpret_cast<void*>(this));
      cli->SetOpt(CURLOPT_READFUNCTION, &Request::ReadCallback);
    } else {
      cli->SetOpt(CURLOPT_POSTFIELDS, "");
    }
  }

  // Set Header In Return
  cli->SetOpt(CURLOPT_HEADERDATA, reinterpret_cast<void*>(&ret));
  cli->SetOpt(CURLOPT_HEADERFUNCTION, &Response::HeaderCallback);

  // Set Body In Return
  cli->SetOpt(CURLOPT_WRITEDATA, reinterpret_cast<void*>(&ret));
  cli->SetOpt(CURLOPT_WRITEFUNCTION, &Response::WriteCallback);

  // Set Error Message In Return
  char error_buffer[CURL_ERROR_SIZE] = {0};
  cli->SetOpt(CURLOPT_ERRORBUFFER, error_buffer);

  const auto curl_result = cli->Perform();

  double written_size = 0;
  cli->GetInfo(CURLINFO_SIZE_DOWNLOAD, &written_size);
  uint64_t ret_code = 0;
  cli->GetInfo(CURLINFO_RESPONSE_CODE, &ret_code);

  const auto& error_message = StrSlice(
      "Error executing an HTTP request (HTTP response code ",
      std::to_string(ret_code), ", error code ", std::to_string(curl_result),
      ", error message '", error_buffer, "')");

  ret.code() = static_cast<uint16_t>(ret_code);
  switch (ret_code) {
    // The group of response codes indicating that the request achieved
    // the expected goal.
    case 200:  // OK
    case 201:  // Created
    case 204:  // No Content
    case 206:  // Partial Content
      if (curl_result != CURLE_OK) {
        // This means the server executed the request successfully, but then
        // something went wrong during the transmission of the response.
        ret.status() = errors::Unavailable(error_message);
      } else {
        ret.status() = Status::OK();
      }
      break;
    case 416:  // Requested Range Not Satisfiable
               // The requested range had no overlap with the available range.
               // This doesn't indicate an error, but this does mean an empty
               // response body.
      ret.ClearBuffer();
      ret.status() = Status::OK();
      break;

    // INVALID_ARGUMENT indicates a problem with how the request is constructed.
    case 400:  // Bad Request
    case 411:  // Length Required
      ret.status() = errors::InvalidArgument(error_message);
      break;

    // PERMISSION_DENIED indicates an authentication or an authorization issue.
    case 401:  // Unauthorized
    case 403:  // Forbidden
      ret.status() = errors::PermissionDenied(error_message);
      break;

    // NOT_FOUND indicates that the requested resource does not exist.
    case 404:  // Not found
    case 410:  // Gone
      ret.status() = errors::NotFound(error_message);
      break;

    // FAILED_PRECONDITION indicates that the request failed because some
    // of the underlying assumptions were not satisfied. The request
    // shouldn't be retried unless the external context has changed.
    case 302:  // Found
    case 303:  // See Other
    case 304:  // Not Modified
    case 307:  // Temporary Redirect
    case 308:  // Resume Incomplete
    case 412:  // Precondition Failed
    case 413:  // Payload Too Large
      ret.status() = errors::FailedPrecondition(error_message);
      break;

    // UNAVAILABLE indicates a problem that can go away if the request
    // is just retried without any modification.
    case 409:  // Conflict
    case 429:  // Too Many Requests
    case 500:  // Internal Server Error
    case 502:  // Bad Gateway
    case 503:  // Service Unavailable
    default:   // All other HTTP response codes also should be retried.
      ret.status() = errors::Unavailable(error_message);
      break;
  }
  return ret;
}

std::string Request::EscapeString(const std::string& str) noexcept {
  shared_ptr<CCurl> cli = CCurl::Load();
  return cli->Escape(str, str.length());
}

size_t Request::ReadCallback(char* buf, size_t size, size_t nmemb,
                             void* this_object) {
  // LOG(INFO) << "Request::ReadCallback, buf:" << buf << " ?";
  // https://curl.haxx.se/libcurl/c/CURLOPT_READFUNCTION.html
  CHECK(buf);
  auto that = reinterpret_cast<Request*>(this_object);
  // LOG(INFO) << "Request::ReadCallback, body:" << that->body_ << " ?";
  CHECK(that->body_);
  return that->body_->Copy(buf, size * nmemb);
}

size_t Response::WriteCallback(const void* ptr, size_t size, size_t nmemb,
                               void* this_object) {
  // LOG(INFO) << "Response::WriteCallback ING";
  CHECK(ptr);
  auto that = reinterpret_cast<Response*>(this_object);
  const size_t bytes_to_copy = size * nmemb;
  that->response_buffer_.insert(
      that->response_buffer_.end(), reinterpret_cast<const char*>(ptr),
      reinterpret_cast<const char*>(ptr) + bytes_to_copy);
  return bytes_to_copy;
}

size_t Response::HeaderCallback(const void* ptr, size_t size, size_t nmemb,
                                void* this_object) {
  CHECK(ptr);
  auto that = reinterpret_cast<Response*>(this_object);
  StrSlice header(
      std::string(reinterpret_cast<const char*>(ptr), size * nmemb));
  StrSlice name, value;

  // The supplied header has the form "<name>: <value>", parse it.
  StrScan sc(header);
  if (sc.ScanEscapedUntil(':').StopCapture().OneLiteral(": ").GetResult(
          &value, &name)) {
    std::string str_value = value.ToString();
    // StripTrailingWhitespace(&str_value);
    // LOG(INFO) << "Header: #FROM# K[" << name.ToString()  //
    //           << "] V[" << str_value << "]";
    // in this case, we are supposed to erase the '\n'
    // at the end of its value in case exists.
    TrimRightInPlace(&str_value);
    // LOG(INFO) << "Header: # TO # K[" << name.ToString()  //
    //           << "] V[" << str_value << "]";
    // LOG(INFO) << "Header: K[" << name.ToString() << "] V[" << str_value <<
    // "]";
    that->response_headers_[name.ToString()] = str_value;
  }
  return size * nmemb;
}

// Cancels the transmission if no progress has been made for too long.
int Response::ProgressCallback(void* this_object, curl_off_t dltotal,
                               curl_off_t dlnow, curl_off_t ultotal,
                               curl_off_t ulnow) {
  auto that = reinterpret_cast<Response*>(this_object);
  const auto now = that->env_->NowSeconds();
  const auto current_progress = dlnow + ulnow;
  if (that->last_progress_timestamp_ == 0 ||
      current_progress > that->last_progress_bytes_) {
    // This is the first time the callback is called or some progress
    // was made since the last tick.
    that->last_progress_timestamp_ = now;
    that->last_progress_bytes_ = current_progress;
    return 0;
  }

  if (now - that->last_progress_timestamp_ > kInactivityTimeoutSeconds) {
    LOG(ERROR) << "The transmission has been stuck at " << current_progress
               << " bytes for " << now - that->last_progress_timestamp_
               << " seconds and will be aborted.";
    return 1;  // Will abort the request.
  }

  // No progress was made since the last call, but we should wait a bit longer.
  return 0;
}

}  // namespace argcv
