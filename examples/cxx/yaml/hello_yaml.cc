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
 * bazel run //examples/cxx/yaml:hello_yaml $(pwd)/examples/cxx/yaml/hello.yml
 **/

#include "glog/logging.h"
#include "yaml-cpp/yaml.h"

using namespace YAML;

void DoRead(const std::string& file) {
  YAML::Node root = YAML::LoadFile(file);
  YAML::Node list = root["list"];

  LOG(INFO) << "list[0]" << list[0].as<std::string>();  // "foo"
  LOG(INFO) << "list[0]" << list[0];                    // "foo", shorthand?
  LOG(INFO) << "list[1][primes][3]" << list[1]["primes"][3].as<int>();  // "7"
  try {
    LOG(INFO) << list[1]["odds"][3].as<int>();  // throws?
  } catch (std::exception& e) {
    LOG(WARNING) << "EXCEPTION";
  }
  // LOG(INFO)  << list[1]["odds"][2].as<int>();    // throws?

  list[2].push_back(5);
  // list[3] = "Hello, World";
  // list[0].reset();
  // list[0]["key"] = "value";
  for (auto r : list) {
    LOG(INFO) << "NEXT:" << r;
  }

  LOG(INFO) << "dump all:\n" << root;
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  for (int i = 1; i < argc; i++) {
    const std::string cname(argv[i]);
    LOG(INFO) << "processing file:" << cname;
    DoRead(cname);
  }
  return 0;
}
