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
#include <unistd.h>

#include "argcv/cxx/par/timer.h"
#include "glog/logging.h"

using argcv::Timer;

int main(int argc, char *argv[]) {
  LOG(INFO) << "start...";
  Timer t;
  t.Label("a");
  ::sleep(1);
  LOG(INFO) << "all  :" << t.All().Ms() << " ms, or " << t.All().Sec()
            << " sec";
  t.Label("b");
  ::sleep(2);
  t.Label("c");
  LOG(INFO) << "b->c :" << t.Between("b", "c").Ms() << " ms, or "
            << t.Between("b", "c").Nanoseconds() << " ns";
  LOG(INFO) << "b->? :" << t.From("b").Ms() << " ms, or "
            << t.From("b").Microseconds() << " micro sec";
  LOG(INFO) << "?->b :" << t.To("b").Ms() << " ms, or " << t.Until("b").Sec()
            << " sec";
  LOG(INFO) << "all  :" << t.All().Ms() << " ms, or " << t.All().Sec()
            << " sec";
  return 0;
}
