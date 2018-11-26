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
#ifndef ARGCV_C_DAEMON_H_
#define ARGCV_C_DAEMON_H_

// #include <stdio.h>

#include <fcntl.h>     // open
#include <signal.h>    // signal
#include <sys/stat.h>  // open O_RDONL
// #include <syslog.h>    // syslog
#include <unistd.h>  // setsid

#ifdef __cplusplus
extern "C" {
#endif  //

static int daemon_max_fd = 64;

// switch to daemon mode
// param:
// work_dir: working directory in daemon mode
// return pid
// if pid > 0, success full
// if pid < 0, switch failed
// pid will never equal to 0
pid_t daemonize(const char* work_dir) {
  pid_t pid;

  if ((pid = fork()) < 0) {
    // called 'fork' but failed
    return -1;
  } else if (pid) {
    // called 'fork', we will get 2 process
    // this process is for parent process
    _exit(0);  // parent terminates
  }

  // fprintf(stderr, "continue %s:%d\n", __FILE__, __LINE__);

  // child 1 continues ...
  if (setsid() < 0) {  // become session leader
    // fprintf(stderr, "setsid() < 0?? %s:%d\n", __FILE__, __LINE__);
    return -1;
  }

  // fprintf(stderr, "continue %s:%d\n", __FILE__, __LINE__);

  // ignore hup signal
  signal(SIGHUP, SIG_IGN);

  // fprintf(stderr, "ignore hup %s:%d\n", __FILE__, __LINE__);

  if ((pid = fork()) < 0) {
    // fprintf(stderr, "pid2 < 0 %s:%d\n", __FILE__, __LINE__);
    return -1;
  } else if (pid) {
    _exit(0);
  }
  // child 2 continues ...
  // here is daemon mode now

  int status_chdir = chdir(work_dir);
  // fprintf(stderr, "chdir %d !! %s:%d\n", status_chdir, __FILE__, __LINE__);
  // assert(status_chdir == 0);
  if (0 != status_chdir) {
    if (status_chdir > 0) {
      status_chdir = -status_chdir;
    }
    return status_chdir;
  }

  int i;
  // close off file descriptors
  for (i = 0; i < daemon_max_fd; i++) {
    close(i);
  }

  open("/dev/null", O_RDONLY);
  open("/dev/null", O_RDWR);
  open("/dev/null", O_RDWR);

  pid = getpid();

  return pid;
}

#ifdef __cplusplus
}
#endif  //

#endif  //  ARGCV_C_DAEMON_H_
