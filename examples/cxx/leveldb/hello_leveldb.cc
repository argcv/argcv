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
#include <cstdio>

// #include <functional>
// #include <iostream>
// #include <iterator>
// #include <list>
// #include <map>
// #include <set>
// #include <vector>

// #include "leveldb/cache.h"
// #include "leveldb/db.h"
// #include "leveldb/options.h"
// #include "leveldb/write_batch.h"

#include "argcv/cxx/db/leveldb/helper.h"

#include "glog/logging.h"

using namespace argcv;

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  auto opt = LdbDefaultOpt();
  std::string ddir = "/tmp/ldb-test";
  size_t cache_size = 1000;
  bool create_if_missing = true;

  ::leveldb::DB* db = LdbInit(ddir, &opt, cache_size, create_if_missing);

  LdbPut(db, "alice::alice", "1");
  LdbPut(db, "alice::bob", "2");
  LdbPut(db, "alice::carol", "3");

  LdbBatchOperator* lbo = new LdbBatchOperator(db);
  lbo->Put("alice::della", "4");
  lbo->Put("alice::elizabeth", "5");
  lbo->Flush();
  lbo->Delete("alice::carol");
  delete lbo;

  LOG(INFO) << "check exists: alice::bob: "
            << (LdbExist(db, "alice::bob") ? "Found" : "Not Found");

  std::string val_ab;

  if (LdbGet(db, "alice::bob", &val_ab)) {
    LOG(INFO) << "get alice::bob: " << val_ab;
  } else {
    LOG(WARNING) << "get alice::bob: >>Not Exists<<";
  }

  LdbDelete(db, "alice::bob");

  int i = 0;
  LdbStartsWith(
      db, "alice::", [&](const std::string& k, const std::string& v) -> bool {
        LOG(INFO) << "key:" << k << ", v:" << v;
        i = i + 1;
        return true;
      });
  LOG(INFO) << "Size of StartsWith: I :" << i;
  LdbClose(db, &opt);
  LdbDestroy(ddir);
  return 0;
}
