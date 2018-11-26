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
#include "argcv/c/dir/dir.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32  // for linux
#undef __STRICT_ANSI__
#define D_GNU_SOURCE
#define _GNU_SOURCE
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#else  // for windows
#include <io.h>
#endif  // WIN32

typedef int (*file_handle)(const char *file_name, int is_dir, void *user);

#ifndef WIN32  // for linux

int is_dir_check(const char *path) {
  struct stat st;
  lstat(path, &st);
  return S_ISDIR(st.st_mode);
}
//
int r_trav(const char *path, int recursive, file_handle f_handler, void *user) {
  DIR *pdir;
  struct dirent *pdirent;
  char tmp[1024];
  pdir = opendir(path);
  int stop_flag = 0;
  if (pdir) {
    while ((pdirent = readdir(pdir)) != 0) {
      // ignore "." && ".."
      if (!strcmp(pdirent->d_name, ".") || !strcmp(pdirent->d_name, ".."))
        continue;
      snprintf(tmp, sizeof(tmp), "%s/%s", path, pdirent->d_name);
      stop_flag = f_handler(tmp, is_dir_check(tmp), user);
      if (stop_flag) return 1;
      // if is Dir and recursive is true , into recursive
      if (is_dir_check(tmp) && recursive) {
        if (r_trav(tmp, recursive, f_handler, user)) {
          return 1;
        }
      }
    }
  } else {
    fprintf(stderr, "opendir error:%s\n", path);
  }
  closedir(pdir);
  return 0;
}

// interface
int dir_iter(const char *path, int recursive, file_handle f_handler,
             void *user) {
  int len;
  char tmp[256];
  len = strlen(path);
  snprintf(tmp, sizeof(tmp), "%s", path);
  if (tmp[len - 1] == '/') tmp[len - 1] = '\0';

  if (is_dir_check(tmp)) {
    r_trav(tmp, recursive, f_handler, user);
  } else {
    // printf("%s\n", path);
    f_handler(path, is_dir_check(path), user);
  }
  return 1;
}

#else  // for windows

// int dir_iter(const char *path, int recursive,file_handle f_handler)
int dir_iter(const char* path, int recursive, file_handle f_handler,
             void* user) {
  int len = strlen(path) + 3;
  intptr_t handle;
  char mypath[1024];
  char searchpath[1024];
  char tmppath[1024];
  char nxtpath[1024];
  char sp = '/';
  int i;
  struct _finddata_t fileinfo;
  snprintf(mypath, sizeof(mypath), "%s", path);
  switch (mypath[len - 1]) {
    case '\\':
      sp = '\\';
      len -= 1;
      mypath[len - 1] = '\0';
      break;
    case '/':
      len -= 1;
      mypath[len - 1] = '\0';
    case '.':
      sp = '/';
      break;
    default:
      for (i = 0; i < len; i++) {
        if (mypath[i] == '\\' || mypath[i] == '/') {
          sp = mypath[i];
          break;
        }
      }
  }
  snprintf(tmppath, sizeof(tmppath), "%s", mypath);
  snprintf(searchpath, sizeof(searchpath), "%s%c%s", mypath, sp, "*");

  for (handle = _findfirst(searchpath, &fileinfo);
       !_findnext(handle, &fileinfo);) {
    if (-1 == handle) return -1;
    //
    snprintf(nxtpath, sizeof(nxtpath), "%s%c%s", tmppath, sp, fileinfo.name);

    // call back
    if ((0 != strcmp(fileinfo.name, ".")) && (0 != strcmp(fileinfo.name, "..")))
      f_handler(nxtpath, ((fileinfo.attrib & _A_SUBDIR) != 0), user);

    if (((fileinfo.attrib & _A_SUBDIR) != 0) && recursive &&
        0 != strcmp(fileinfo.name, ".") && 0 != strcmp(fileinfo.name, ".."))
      dir_iter(nxtpath, recursive, f_handler, user);
  }
  _findclose(handle);
  return 1;
}

#endif  // end of linux/windows
