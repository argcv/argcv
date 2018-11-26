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
#include "argcv/cxx/fs/path.h"

#include <string>

#include "gtest/gtest.h"

using namespace argcv;  // NOLINT(build/namespaces)

TEST(PathBuff, Basic) {
  PathBuff b0("hello");
  EXPECT_EQ("hello", b0.ToString());
  EXPECT_EQ(5, b0.W());
  b0.Backspace();
  EXPECT_EQ(4, b0.W());
  EXPECT_EQ("hell", b0.ToString());
  b0.Append('_').Append('X');
  EXPECT_EQ("hell_X", b0.ToString());
  std::string cn_str("你好");
  for (auto c : cn_str) {
    b0.Append(c);
  }
  EXPECT_EQ("hell_X你好", b0.ToString());
  EXPECT_EQ('e', b0.Index(1));
}

TEST(Path, Basic) {
  EXPECT_EQ(".", Clean(""));
  EXPECT_EQ("/", Clean("/"));
  EXPECT_EQ("/", Clean("/.."));
  EXPECT_EQ("/a/c", Clean("/a/b/../c"));
  EXPECT_EQ("/a/c", Clean("/a/b/../c/"));
  EXPECT_EQ("/a/c", Clean("/a/b/../c//"));
  EXPECT_EQ("/a/你好c", Clean("/a/b/../你好c//"));
}

TEST(Join, Basic) {
  const std::string path("你好");
  EXPECT_EQ("a/b/你好", Join({"a", "b", path}));
  EXPECT_EQ("/a/b/你好/世界", Join({"/", "a", "b", path, "世界", "/"}));
}

TEST(SplitFilename, Basic) {
  const std::string fullname("/usr/bin/man");
  std::string dir, file;
  SplitFilename(fullname, &dir, &file);
  EXPECT_EQ("/usr/bin", dir);
  EXPECT_EQ("man", file);
}

TEST(SplitFilename, NoSlash) {
  const std::string fullname("man");
  std::string dir, file;
  SplitFilename(fullname, &dir, &file);
  EXPECT_EQ("", dir);
  EXPECT_EQ("man", file);
}

TEST(BaseElem, Basic) {
  EXPECT_EQ("abc", BaseElem("/abc"));
  EXPECT_EQ(".", BaseElem(""));
  EXPECT_EQ("/", BaseElem("/"));
}
