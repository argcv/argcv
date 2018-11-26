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
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>  // for pid_t

#include "argcv/c/daemon.h"

int main(int argc, char const *argv[]) {
  openlog("hello-daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
  syslog(LOG_INFO, "daemon starting");
  pid_t pid = daemonize("/");
  if (pid > 0) {
    syslog(LOG_INFO, "starting...");
    FILE *fp = fopen("/tmp/hello-daemon.log", "w");
    fprintf(fp, "pid: %d\n", (int)pid);
    fflush(fp);
    int i;
    for (i = 0; i < 20; i++) {
      syslog(LOG_INFO, "heart beat..!");
      fprintf(fp, "sec: %i ...\n", i);
      fflush(fp);
      sleep(1);
    }
    fprintf(fp, "QUIT ..\n");
    fclose(fp);
  } else {
    FILE *fp = fopen("/tmp/hello-daemon.log", "w");
    fprintf(fp, "init failed, pid: (%d) ..\n", (int)pid);
    fclose(fp);
  }
  closelog();
  return 0;
}
