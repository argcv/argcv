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
#ifndef ARGCV_CXX_NET_TCP_TCP_SERVICE_H_
#define ARGCV_CXX_NET_TCP_TCP_SERVICE_H_

#include <assert.h>

#include <functional>
#include <string>

#include "argcv/cxx/net/tcp/tcp_listen_pool.h"
#include "argcv/cxx/par/lock.h"
#include "glog/logging.h"

namespace argcv {
// TODO(yu): Working In Progress..

using ::argcv::Spinlock;

class TcpContext {
 public:
  void OnAccess(const std::function<void()>& func) noexcept {}
  void OnSend(const std::function<void()>& func) noexcept {}
  void OnClose(const std::function<void()>& func) noexcept {}

 private:
};

class TcpService {
 public:
  TcpService(int m_port, size_t n_threads = 4, size_t m_max_conn_size = 1024,
             size_t m_max_buff_size = 1024 * 1024 * 1024) noexcept
      : lock_(Spinlock()),
        stop_(true),
        stop_attempt_(false),
        n_threads_(n_threads),
        port_(m_port),
        max_conn_size_(m_max_conn_size),
        max_buff_size_(m_max_buff_size) {
    assert(n_threads > 0);
    LOG(INFO) << "starting TcpService.. :" << m_port
              << " max conn size:" << m_max_conn_size;
  }

  void Stop() {
    auto g = lock_.Guard();
    stop_attempt_ = true;
  }

  void Listen() {
    {
      auto g = lock_.Guard();
      if (!stop_) {
        LOG(INFO) << "Listen Failed: It is Serving";
        return;
      }
      stop_ = false;
      stop_attempt_ = false;
    }

    TcpListenPool pool(port_, max_conn_size_, max_buff_size_);
    assert(0 == pool.error_no());
    LOG(INFO) << "Start Listening...";
    for (; !stop_attempt_;) {
      int id = pool.Poll(0);
      if (id != -1) {
        LOG(INFO) << "ACCEPT, ID:" << id;
        Conn& c = pool[id];
        ssize_t st = pool.Pull(id, 1024);
        LOG(INFO) << "ACCEPT, STATUS:" << st;
        if (st > 0) {
          std::string s = c.str();
          LOG(INFO) << "LENGTH:" << c.str().length() << " DATA[" << c.str()
                    << "]";
          c.Write(c.str(), c.str().length());
          c.Clear();
        } else {
          if (c.Closed()) {
            LOG(INFO) << "is closed";
            // break;
          } else {
            LOG(WARNING) << "unknown error:" << SockStatusExplain(c.status());
            // break;
          }
        }
        c.Flush();
      }
      std::this_thread::yield();
    }
    LOG(INFO) << "Stopped";
    stop_attempt_ = false;
    stop_ = true;
  }

  // this is just used to cleanup warning
  size_t NThreads() const noexcept { return n_threads_; }

 private:
  Spinlock lock_;
  bool stop_;
  bool stop_attempt_;
  size_t n_threads_;
  int port_;
  size_t max_conn_size_;
  size_t max_buff_size_;
};

}  // namespace argcv

#endif  //  ARGCV_CXX_NET_TCP_TCP_SERVICE_H_
