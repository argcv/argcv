// Copyright 2019 Yu Jing
#include "argcv/cxx/os/path.h"

#include <string>

#include "gtest/gtest.h"

using namespace argcv;  // NOLINT(build/namespaces)

TEST(PathBuff, Basic) {
  io::internal::PathBuff b0("hello");
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
  EXPECT_EQ(".", io::CleanPath(""));
  EXPECT_EQ("/", io::CleanPath("/"));
  EXPECT_EQ("/", io::CleanPath("/.."));
  EXPECT_EQ("/a/c", io::CleanPath("/a/b/../c"));
  EXPECT_EQ("/a/c", io::CleanPath("/a/b/../c/"));
  EXPECT_EQ("/a/c", io::CleanPath("/a/b/../c//"));
  EXPECT_EQ("/a/你好c", io::CleanPath("/a/b/../你好c//"));
}

TEST(Join, Basic) {
  const std::string path("你好");
  EXPECT_EQ("a/b/你好", io::JoinPath("a", "b", path));
  EXPECT_EQ("/a/b/你好/世界", io::JoinPath("/", "a", "b", path, "世界", "/"));
}

TEST(SplitFilename, Basic) {
  const std::string fullname("/usr/bin/man");
  std::string dir, file;
  io::SplitFilename(fullname, &dir, &file);
  EXPECT_EQ("/usr/bin", dir);
  EXPECT_EQ("man", file);
}

TEST(SplitFilename, NoSlash) {
  const std::string fullname("man");
  std::string dir, file;
  io::SplitFilename(fullname, &dir, &file);
  EXPECT_EQ("", dir);
  EXPECT_EQ("man", file);
}

TEST(BaseElem, Basic) {
  EXPECT_EQ("abc", io::Basename("/abc"));
  EXPECT_EQ(".", io::Basename(""));
  EXPECT_EQ("/", io::Basename("/"));
}
