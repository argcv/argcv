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

#include "argcv/cxx/net/helper/helper.h"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <unordered_set>
//
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <unistd.h>

#include "glog/logging.h"

namespace argcv {

bool TryBindTcp(const char *serv, const char *host) {
  int listenfd;
  int n;
  const int on = 1;
  struct addrinfo hints;
  struct addrinfo *res;
  struct addrinfo *ressave;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((n = ::getaddrinfo(host, serv, &hints, &res)) != 0) {
    LOG(ERROR) << "getaddrinfo() failed: " << ::gai_strerror(n) << " "
               << (host ? host : "nullptr") << ":" << serv;
    return false;
  }
  ressave = res;
  do {
    //
    listenfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // error , try next one
    if (listenfd < 0) continue;
    //
    ::setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    // try bind , if equals zero ,
    // it will be judged as success and break the loop
    if (::bind(listenfd, res->ai_addr, res->ai_addrlen) == 0) break;
    // bind error , close & try next one
    ::close(listenfd);
  } while ((res = res->ai_next) != nullptr);
  if (res == nullptr) {
    LOG(ERROR) << "bind() failed " << (host ? host : "nullptr") << ":" << serv;
    return false;
  }
  // listen LISTENQ is customed
  ::listen(listenfd, 1024);
  ::freeaddrinfo(ressave);
  ::close(listenfd);
  return true;
}

int TryBindUdp(const char *serv, const char *host) {
  int listenfd;
  int n;
  struct addrinfo hints;
  struct addrinfo *res;
  struct addrinfo *ressave;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((n = ::getaddrinfo(host, serv, &hints, &res)) != 0) {
    LOG(ERROR) << "getaddrinfo() failed: " << ::gai_strerror(n) << " "
               << (host ? host : "nullptr") << ":" << serv;
    return false;
  }

  ressave = res;

  do {
    listenfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listenfd < 0) continue;
    if (::bind(listenfd, res->ai_addr, res->ai_addrlen) == 0) break;
    ::close(listenfd);
  } while ((res = res->ai_next) != nullptr);

  if (res == nullptr) {
    LOG(ERROR) << "bind() failed " << (host ? host : "nullptr") << ":" << serv;
    return false;
  }
  ::freeaddrinfo(ressave);
  ::close(listenfd);
  return listenfd;
}

bool IsPortAvailable(int *port, const char *host, bool is_tcp) {
  // LOG(INFO) << "TESTING::" << (*port) << "is_tcp" << is_tcp;
  char serv[20];
  memset(serv, 0, sizeof(char) * 20);
  snprintf(serv, sizeof(serv), "%d", *port);
  bool status = is_tcp ? TryBindTcp(serv, host) : TryBindUdp(serv, host);
  // LOG(INFO) << "TESTING::" << (*port) << "is_tcp" << is_tcp << "pass..?"
  //           << status;
  return status;
}

int PickUnusedPortOrZero(int port_lo, int port_hi, const char *host) {
  static std::unordered_set<int> chosen_ports;
  // Type of port to first pick in the next iteration.
  bool is_tcp = true;
  for (auto port = port_lo; port < port_hi; port++) {
    if (chosen_ports.find(port) != chosen_ports.end()) {
      continue;
    }
    if (!IsPortAvailable(&port, host, is_tcp)) {
      chosen_ports.insert(port);
      continue;
    }

    if (!IsPortAvailable(&port, host, !is_tcp)) {
      chosen_ports.insert(port);
      continue;
    }

    chosen_ports.insert(port);
    return port;
  }
  return 0;
}

}  // namespace argcv
