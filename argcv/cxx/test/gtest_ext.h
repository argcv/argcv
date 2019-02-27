// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_TEST_GTEST_EXT_H_
#define ARGCV_CXX_TEST_GTEST_EXT_H_

#include "gtest/gtest.h"

#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

static jmp_buf jmp_env;

static void catch_alarm(int sig) { longjmp(jmp_env, 1); }

// ref: https://github.com/google/googletest/issues/348#issuecomment-235674063
// by https://github.com/louis-langholtz
#define ASSERT_USECS(usecs, fn)                                     \
  {                                                                 \
    const auto val = setjmp(jmp_env);                               \
    if (val == 0) {                                                 \
      signal(SIGALRM, catch_alarm);                                 \
      ualarm((usecs), 0);                                           \
      { fn; };                                                      \
      ualarm(0, 0);                                                 \
    } else {                                                        \
      GTEST_FATAL_FAILURE_(#usecs " usecs timer tripped for " #fn); \
    }                                                               \
  }

#endif  // ARGCV_CXX_TEST_GTEST_EXT_H_
