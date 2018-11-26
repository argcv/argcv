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

// #include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>

#include "argcv/cxx/net/helper/helper.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::IsPortAvailable;
using argcv::PickUnusedPortOrZero;

int HoldTcpPort(const char *serv, const char *host) {
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
    LOG(ERROR) << "getaddrinfo() failed: " << ::gai_strerror(n);
    return 0;
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
  } while ((res = res->ai_next) != NULL);
  if (res == NULL) {
    LOG(ERROR) << "bind() failed";
    return 0;
  }
  // listen LISTENQ is customed
  ::listen(listenfd, 1024);
  ::freeaddrinfo(ressave);
  return listenfd;
}

TEST(NetHelper, PickUnusedPortOrZero) {
  int listenfd = HoldTcpPort("27017", "0.0.0.0");
  int port0 = PickUnusedPortOrZero(27017, 27030, "0.0.0.0");
  int port1 = PickUnusedPortOrZero(27017, 27030, "0.0.0.0");
  LOG(INFO) << "port0:" << port0;
  LOG(INFO) << "port1:" << port1;
  CHECK_GE(port0, 0);
  CHECK_LT(port0, 65536);
  CHECK_GE(port1, 0);
  CHECK_LT(port1, 65536);
  CHECK_NE(port0, port1);
  if (listenfd > 0) ::close(listenfd);
}
