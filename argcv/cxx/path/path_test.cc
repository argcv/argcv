// Copyright 2019 Yu Jing
#include "argcv/cxx/path/path.h"

#include <string>

#include "gtest/gtest.h"

using namespace argcv;  // NOLINT(build/namespaces)

TEST(IO, PathBuff) {
  path::internal::PathBuff b0("hello");
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

TEST(IO, CleanPath) {
  EXPECT_EQ(".", path::CleanPath(""));
  EXPECT_EQ("/", path::CleanPath("/"));
  EXPECT_EQ("/", path::CleanPath("/.."));
  EXPECT_EQ("/a/c", path::CleanPath("/a/b/../c"));
  EXPECT_EQ("/a/c", path::CleanPath("/a/b/../c/"));
  EXPECT_EQ("/a/c", path::CleanPath("/a/b/../c//"));
  EXPECT_EQ("/a/你好c", path::CleanPath("/a/b/../你好c//"));
  EXPECT_EQ("/a/你好c", path::CleanPath("/a/b/../你好c/"));
}

TEST(IO, JoinPath) {
  const std::string path("你好");
  EXPECT_EQ("a/b/你好", path::JoinPath("a", "b", path));
  EXPECT_EQ("/a/b/你好/世界", path::JoinPath("/", "a", "b", path, "世界", "/"));
}

TEST(IO, SplitFilename) {
  const std::string fullname("/usr/bin/man");
  string_view dir, file;
  path::SplitFilename(fullname, &dir, &file);
  EXPECT_EQ("/usr/bin/", dir);
  EXPECT_EQ("man", file);

  const std::string fullname2("man");
  path::SplitFilename(fullname2, &dir, &file);
  EXPECT_EQ("", dir);
  EXPECT_EQ("man", file);
}

TEST(IO, Basename) {
  EXPECT_EQ("abc", path::Basename("/abc"));
  EXPECT_EQ("abc", path::Basename("/abc/"));
  EXPECT_EQ(".", path::Basename(""));
  EXPECT_EQ("/", path::Basename("/"));
  EXPECT_EQ("def", path::Basename("/path/to/def"));
}

TEST(IO, Dirname) {
  EXPECT_EQ("/", path::Dirname("/abc"));
  EXPECT_EQ("/abc", path::Dirname("/abc/"));
  EXPECT_EQ(".", path::Dirname(""));
  EXPECT_EQ("/", path::Dirname("/"));
  EXPECT_EQ("/path/to", path::Dirname("/path/to/def"));
}
