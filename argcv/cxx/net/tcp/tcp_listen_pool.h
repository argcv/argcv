/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2012-2015 codingnow.com
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
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * It comes from: https://github.com/cloudwu/mread
 * A corresponding discussion: https://blog.codingnow.com/2012/04/mread.html
 **/
#ifndef ARGCV_CXX_NET_TCP_TCP_LISTEN_POOL_H_
#define ARGCV_CXX_NET_TCP_TCP_LISTEN_POOL_H_

// for network io

#ifdef __linux__
#define __EPOLL__ 1
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || \
    defined(__NetBSD__)
#define __KQUEUE__ 1
#else  // ERROR/TODO use SELECT
#error "NEITHOR OF EPOLL AND KQUEUE ARE AVAILABLE"
#endif  // __linux__ or __APPLE__

#ifdef __EPOLL__
#include <sys/epoll.h>
#elif __KQUEUE__
#include <sys/event.h>
#endif  // __KQUEUE__

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>  // for fctl open

#include <string>

#include "glog/logging.h"

namespace argcv {

#define LISTENSOCKET (void *)((intptr_t)~0)
static const int kSzListenQ = 32;
static const int kSzPullMax = 102400;  // max pull size 10MB

enum class SockStatus {
  kSockInvalid,
  kSockClosed,
  kSockSuspend,
  // kSockRead,
  kSockPollin,
  kSockAlive = static_cast<int>(kSockSuspend)
};

const std::string SockStatusExplain(SockStatus status);

class TcpListenPool {
 public:
  class Conn {
   public:
    Conn() noexcept : status_(SockStatus::kSockInvalid) {}
    ~Conn() noexcept {
      LOG(INFO) << "[~conn] " << index_ << "#" << fd_;
      if (status_ >= SockStatus::kSockAlive) {
        close(fd_);
      }
    }

    bool assign(TcpListenPool *m_pool, int m_index, int m_fd) noexcept {
      pool_ = m_pool;
      index_ = m_index;
      fd_ = m_fd;
      // printf("init %d\n",pool_->_port());
      return true;
    }

    int fd() noexcept { return fd_; }
    void fd(int m_fd) noexcept { fd_ = m_fd; }

    int index() noexcept { return index_; }

    SockStatus status() noexcept { return status_; }
    void status(SockStatus m_status) noexcept { status_ = m_status; }

    void Reset(int m_fd, SockStatus m_status) noexcept {
      data_.clear();
      fd_ = m_fd;
      status_ = m_status;
    }

    void Push(const std::string &d) noexcept { data_ += d; }

    size_t size() noexcept { return data_.size(); }

    void Flush() noexcept {
      if (Closed()) {
        pool_->closed_size_dec();
        status_ = SockStatus::kSockInvalid;
        fd_ = pool_->UpdateFreeCoClue(index_);
        data_.clear();
      }
    }

    void Clear() noexcept {
      // cleanup the buffer
      data_.clear();
    }

    bool Closed() noexcept {
      return SockStatus::kSockClosed == status_ && data_.empty();
    }
    bool Empty() noexcept { return data_.empty(); }

    ssize_t Write(const std::string &data, size_t sz) noexcept {
      // return send(fd,data.c_str(),sz,MSG_DONTWAIT);
      for (;;) {
        ssize_t n_bytes = send(fd_, data.c_str(), sz,
                               MSG_DONTWAIT
#ifdef MSG_NOSIGNAL
                                   | MSG_NOSIGNAL
#endif      // MSG_NOSIGNAL
        );  // NOLINT(whitespace/parens)
        // int n_bytes = ::write(fd, data.c_str(), sz);//
        if (0 == n_bytes) {
          Deactive();
        } else if (-1 == n_bytes) {
          switch (errno) {  // #include <error.h>
            case EWOULDBLOCK:
              status_ = SockStatus::kSockSuspend;
              break;
            case EINTR:
              continue;
            default:
              Deactive();
              break;
          }
        }
        return n_bytes;
      }
    }

    ssize_t Pull(int32_t sz = 2048) noexcept {
      switch (status_) {
        // case SockStatus::kSockRead:
        //    _c->_status(SockStatus::kSockSuspend);
        case SockStatus::kSockClosed:
        case SockStatus::kSockSuspend:
          return -1;  // status: is closed
        default:
          if (SockStatus::kSockPollin != status_) {
            LOG(WARNING) << "unexpected status: " << SockStatusExplain(status_);
            return -2;  // unexpected status
          }
      }
      int32_t szPull = (int32_t)(sz - size());  // check pull size we still need
      if (szPull >= kSzPullMax)
        szPull = kSzPullMax - 1;  // pull size limited into kSzPullMax
      if (szPull > 0) {
        for (;;) {
          // char buffer[szPull + 1];
          char buffer[kSzPullMax];
          memset(buffer, 0, sizeof(char) * kSzPullMax);
          ssize_t n_bytes = recv(fd_, &buffer, (size_t)szPull, MSG_DONTWAIT);
          if (0 < n_bytes) {
            // _c->_status(SockStatus::kSockRead);
            Push(std::string(buffer, n_bytes));
            break;
          }
          if (0 == n_bytes) {
            Deactive();
            return 0;  // deactive
          }
          if (-1 == n_bytes) {  // some error founded
            switch (errno) {    // #include <error.h>
              case EWOULDBLOCK:
                status_ = SockStatus::kSockSuspend;
                break;
              case EINTR:
                continue;
              default:
                Deactive();
                break;
            }
            return -3;  // other error
          }
        }
      }
      return size();
    }

    // variables

    const std::string &str() const noexcept { return data_; }

   private:
    // conn(const conn &);  // Prevent copy-construction
    // conn & operator=(const conn &);  // Prevent assignment
    TcpListenPool *pool_;
    int index_;
    int fd_;
    std::string data_;
    SockStatus status_;

    void Deactive() noexcept {
      status_ = SockStatus::kSockClosed;
      data_.clear();
      close(fd_);
      LOG(INFO) << "[conn::deactive] close " << index_ << " (fd=" << fd_ << ")";
#ifdef __EPOLL__
      // int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
      epoll_ctl(pool_->ev_fd(), EPOLL_CTL_DEL, fd_, NULL);
#elif __KQUEUE__
      struct kevent ke;
      EV_SET(&ke, fd_, EVFILT_READ, EV_DELETE, 0, 0, NULL);
      kevent(pool_->ev_fd(), &ke, 1, NULL, 0, NULL);
#endif  // __KQUEUE__
      pool_->closed_size_inc();
    }
  };

  TcpListenPool(int m_port, size_t m_max_conn_size = 1024,
                size_t m_max_buff_size = 1024 * 1024 * 1024) noexcept
      : port_(m_port),
        max_conn_size_(m_max_conn_size),
        co_(new Conn[m_max_conn_size]),
        error_no_(0) {
    LOG(INFO) << "starting TcpListenPool.. :" << m_port
              << " max conn size:" << m_max_conn_size;
    if (!Init()) {
      LOG(WARNING) << "TcpListenPool start failed...";
      error_no_ = -1;  // start failed
    }
  }

  ~TcpListenPool() noexcept {
    delete[] co_;
    if (listen_fd_ >= 0) {
      close(listen_fd_);
    }
    if (ev_fd_ >= 0) {
      close(ev_fd_);
    }
    LOG(INFO) << "TcpListenPool closed";
  }

  int Poll(int timeout) noexcept {  // get a new id
    if (sz_closed_ > 0) {
      int closed_fd = ReportClosed();
      if (-1 != closed_fd) {
        return closed_fd;
      }
    }

    if (evq_head_ >= evq_len_) {
      if (-1 == evq_poll(timeout)) {
        return -1;
      }
    }

    int _1 __attribute__((unused)) = 1;

    for (;;) {
      void *evq_reply = EvqReadOne();
      if (nullptr == evq_reply) {
        // printf("EvqReadOne :: null ...\n");
        return -1;
      }
      if (LISTENSOCKET == evq_reply) {
        struct sockaddr_in remote_addr;
        socklen_t len = sizeof(struct sockaddr_in);
        int client_fd =
            accept(listen_fd_, (struct sockaddr *)&remote_addr, &len);
        if (client_fd >= 0) {
          LOG(INFO) << "TcpListenPool::poll connect "
                    << inet_ntoa(remote_addr.sin_addr) << ":"
                    << ntohs(remote_addr.sin_port) << " (fd=" << client_fd
                    << ")";
#ifdef SO_NOSIGPIPE
          // disaple SIGPIPE signalling for this socket on OSX
          if (setsockopt(client_fd, SOL_SOCKET, SO_NOSIGPIPE, &_1,
                         sizeof(_1)) == -1)
            LOG(WARNING) << "[SOCKET] Failed to set SO_NOSIGPIPE: "
                         << strerror(errno);
#endif  // SO_NOSIGPIPE
          Active(client_fd);
        }
      } else {
        // Conn *_c = (Conn *)evq_reply;
        Conn *c = reinterpret_cast<Conn *>(evq_reply);
        int idx = c->index();
        c->status(SockStatus::kSockPollin);
        return idx;
      }
    }
  }

  ssize_t Pull(size_t idx, int sz = 2048) noexcept {
    if (idx >= max_conn_size_) {
      return -3;  // error: idx not found
    }
    return co_[idx].Pull(sz);  //
  }

  ssize_t Write(size_t idx, const std::string &data, size_t sz) noexcept {
    if (idx >= max_conn_size_) {
      return -1;
    }
    return co_[idx].Write(data, sz);
  }

  // variables

  const int port() const noexcept { return port_; }
  const size_t max_conn_size() const noexcept { return max_conn_size_; }
  const int error_no() const noexcept { return error_no_; }

  Conn &get(size_t idx) noexcept { return co_[idx]; }
  Conn &operator[](size_t idx) noexcept { return co_[idx]; }

  int conn_id(size_t index_) noexcept { return co_[index_].fd(); }

  int &ev_fd() noexcept { return ev_fd_; }

 private:
  const int port_;
  const size_t max_conn_size_;  // MAX connection Size
  Conn *co_;

  int error_no_;  // -1 : init failed

  int listen_fd_;

  int free_co_clue_;
  // std::mutex _free_co_clue_mutex;

  size_t evq_head_;
  size_t evq_len_;

  size_t sz_closed_;
  // std::mutex closed_mutex;

  int ev_fd_;
#ifdef __EPOLL__
  struct epoll_event ev_[kSzListenQ];
#elif __KQUEUE__
  struct kevent ev_[kSzListenQ];
#endif  // __KQUEUE__
  bool Init() noexcept {
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == listen_fd_) {
      return false;
    }

    // set nonblocking
    int no_blocking_flag = fcntl(listen_fd_, F_GETFL, 0);
    if (-1 == no_blocking_flag) {
      return false;
    }
    if (-1 == fcntl(listen_fd_, F_SETFL, no_blocking_flag | O_NONBLOCK)) {
      return false;
    }

    int reuse = 1;
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    struct sockaddr_in self_addr;
    memset(&self_addr, 0, sizeof(struct sockaddr_in));
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = htons(port_);
    self_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_LOOPBACK

    // char * b_addr = inet_ntoa(self_addr.sin_addr);
    // uint16_t b_port = ntohs(self_addr.sin_port);
    // printf("TcpListenPool bind to %s:%u\n",b_addr,b_port);

    if (-1 == bind(listen_fd_, (struct sockaddr *)&self_addr,
                   sizeof(struct sockaddr))) {
      close(listen_fd_);
      return false;
    }
    if (-1 == listen(listen_fd_, kSzListenQ)) {
      close(listen_fd_);
      return false;
    }
#ifdef __EPOLL__
    ev_fd_ = epoll_create(max_conn_size_ + 1);
    if (-1 == ev_fd_) {
      close(listen_fd_);
      return false;
    }

    struct epoll_event ee;
    ee.events = EPOLLIN;
    ee.data.ptr = LISTENSOCKET;

    if (-1 == epoll_ctl(ev_fd_, EPOLL_CTL_ADD, listen_fd_, &ee)) {
      close(listen_fd_);
      close(ev_fd_);
      return false;
    }
#elif __KQUEUE__
    ev_fd_ = kqueue();
    if (-1 == ev_fd_) {
      close(listen_fd_);
      return false;
    }

    struct kevent ke;
    EV_SET(&ke, listen_fd_, EVFILT_READ, EV_ADD, 0, 0, LISTENSOCKET);
    if (-1 == kevent(ev_fd_, &ke, 1, NULL, 0, NULL)) {
      close(listen_fd_);
      close(ev_fd_);
      return false;
    }
#endif  // __KQUEUE__
    for (size_t i = 0; i < max_conn_size_ - 1; i++) {
      co_[i].assign(this, static_cast<int>(i), static_cast<int>(i + 1));
    }
    co_[max_conn_size_ - 1].assign(this, static_cast<int>(max_conn_size_ - 1),
                                   -1);
    free_co_clue_ = 0;
    evq_head_ = 0;
    evq_len_ = 0;
    return true;
  }

  int evq_poll(int timeout) noexcept {
#ifdef __EPOLL__
    int n = epoll_wait(ev_fd_, ev_, kSzListenQ, timeout);
#elif __KQUEUE__
    struct timespec timeoutspec;
    timeoutspec.tv_sec = timeout / 1000;
    timeoutspec.tv_nsec = (timeout % 1000) * 1000000;
    int n = kevent(ev_fd_, NULL, 0, ev_, kSzListenQ, &timeoutspec);
#endif  // __KQUEUE__

    evq_head_ = 0;
    if (n == -1) {
      evq_len_ = 0;
      return -1;
    } else {
      evq_len_ = n;
      return n;
    }
  }

  void *EvqReadOne() noexcept {
    if (evq_head_ >= evq_len_) {
      return nullptr;
    }
#ifdef __EPOLL__
    return ev_[evq_head_++].data.ptr;
// int n = epoll_wait(ev_fd , ev, kSzListenQ, timeout);
#elif __KQUEUE__
    return ev_[evq_head_++].udata;
#endif  // __KQUEUE__
  }

  int CoAlloc() noexcept {
    // std::lock_guard<std::mutex> l(_free_co_clue_mutex);
    if (-1 == free_co_clue_) {
      return -1;
    }
    int s = free_co_clue_;
    free_co_clue_ = co_[s].fd();
    // printf("CoAlloc::%d\n",s);
    return s;
  }

  void Active(int fd) noexcept {
    int co_offset = CoAlloc();
    if (-1 == co_offset) {
      LOG(WARNING) << "TcpListenPool::active for FD: " << fd
                   << " **FAILED** , because assign connection failed";
      return;
    }
#ifdef __EPOLL__
    struct epoll_event ee;
    ee.events = EPOLLIN;
    ee.data.ptr = &(co_[co_offset]);
    if (-1 == epoll_ctl(ev_fd_, EPOLL_CTL_ADD, fd, &ee)) {
      close(fd);
      LOG(WARNING) << "TcpListenPool::active for FD: " << fd
                   << " **FAILED** , establish epoll failed";
      return;
    }
#elif __KQUEUE__
    struct kevent ke;
    EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, &(co_[co_offset]));
    if (-1 == kevent(ev_fd_, &ke, 1, NULL, 0, NULL)) {
      close(fd);
      LOG(WARNING) << "TcpListenPool::active for FD: " << fd
                   << " **FAILED** , establish kevent failed";
      return;
    }
#endif  // __KQUEUE__
    co_[co_offset].Reset(fd, SockStatus::kSockSuspend);
    LOG(INFO) << "TcpListenPool::active for FD: " << fd << ", new connection";
  }

  int ReportClosed() noexcept {
    for (size_t i = 0; i < max_conn_size_; i++) {
      if (SockStatus::kSockClosed == co_[i].status()) {
        return static_cast<int>(i);
      }
    }
    return -1;
  }

  int UpdateFreeCoClue(int offset) noexcept {
    int rt_offset = free_co_clue_;
    free_co_clue_ = offset;
    return rt_offset;
  }

  void closed_size_inc() noexcept {  // increase
    // std::lock_guard<std::mutex> l(closed_mutex);
    sz_closed_++;
  }

  void closed_size_dec() noexcept {  // decrease
    // std::lock_guard<std::mutex> l(closed_mutex);
    sz_closed_--;
  }
};

typedef TcpListenPool::Conn Conn;

}  // namespace argcv

#endif  //  ARGCV_CXX_NET_TCP_TCP_LISTEN_POOL_H_
