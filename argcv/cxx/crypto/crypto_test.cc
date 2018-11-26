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
#include "argcv/cxx/crypto/crypto.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace argcv {
TEST(Crypto, MD4) {
  {
    MD4Helper md4g;
    md4g.Update("argcv");
    md4g.Finish();
    EXPECT_EQ(md4g.ToString(), "23e493d077bcb6115535c7ada7dc9704");
  }

  {
    MD4Helper md4g;
    md4g.Update("23e493d077bcb6115535c7ada7dc9704");
    md4g.Finish();
    EXPECT_EQ(md4g.ToString(), "bf67c3b8c2603192975170ec36c5fe65");
  }
}

TEST(Crypto, MD5) {
  {
    MD5Helper md5g;
    md5g.Update("argcv");
    md5g.Finish();
    EXPECT_EQ(md5g.ToString(), "be396a41f5f66da17c10ced61402df53");
  }

  {
    MD5Helper md5g;
    md5g.Update("be396a41f5f66da17c10ced61402df53");
    md5g.Finish();
    EXPECT_EQ(md5g.ToString(), "1ffbf7b9ab7476afc0fb13c11e91d117");
  }
}

TEST(Crypto, SHA1) {
  {
    SHA1Helper sha1g;
    sha1g.Update("argcv");
    sha1g.Finish();
    EXPECT_EQ(sha1g.ToString(), "cb517a54da3a5d5b07d78aa4e538b3b7d3e637cc");
  }

  {
    SHA1Helper sha1g;
    sha1g.Update("cb517a54da3a5d5b07d78aa4e538b3b7d3e637cc");
    sha1g.Finish();
    EXPECT_EQ(sha1g.ToString(), "3bbfd3842cfafc0fbb41b1978e3823d86e971c49");
  }
}

TEST(Crypto, SHA224) {
  {
    SHA224Helper sha224g;
    sha224g.Update("argcv");
    sha224g.Finish();
    EXPECT_EQ(sha224g.ToString(),
              "943fa32be6c6ad3d8264c3549fcd3f6da15d6cc904c390d4a321e4ee");
  }

  {
    SHA224Helper sha224g;
    sha224g.Update("943fa32be6c6ad3d8264c3549fcd3f6da15d6cc904c390d4a321e4ee");
    sha224g.Finish();
    EXPECT_EQ(sha224g.ToString(),
              "682b5840ac1f841d1c92cd7223c19887e507ed140b9276e2c5eb2165");
  }
}

TEST(Crypto, SHA256) {
  {
    SHA256Helper sha256g;
    sha256g.Update("argcv");
    sha256g.Finish();
    EXPECT_EQ(
        sha256g.ToString(),
        "80cde114e81d126de364f7813f2c26c21be35aeb6168039865387b5bbc33de4f");
  }

  {
    SHA256Helper sha256g;
    sha256g.Update(
        "80cde114e81d126de364f7813f2c26c21be35aeb6168039865387b5bbc33de4f");
    sha256g.Finish();
    EXPECT_EQ(
        sha256g.ToString(),
        "21b81c5010d5d5c684e4faa7f16a6f7509cdec66d1453a7e1bd229f91961a5a4");
  }
}

TEST(Crypto, SHA384) {
  {
    SHA384Helper sha384g;
    sha384g.Update("argcv");
    sha384g.Finish();
    EXPECT_EQ(sha384g.ToString(),
              "7ed66e861898cf7d4a9eb667260dc80f0db996bcef46a5feb774585b83abb84b"
              "b87f5dd536efd6a3e1c5c6b72ccef0d4");
  }

  {
    SHA384Helper sha384g;
    sha384g.Update(
        "7ed66e861898cf7d4a9eb667260dc80f0db996bcef46a5feb774585b83abb84bb87f5d"
        "d536efd6a3e1c5c6b72ccef0d4");
    sha384g.Finish();
    EXPECT_EQ(sha384g.ToString(),
              "132e11a3de747e1e810563f2d2ed1c0dd09e5d9e901414a65202b1f1954fbc6a"
              "173a7f38ad54bc8754375d1df049bb5a");
  }
}

TEST(Crypto, SHA512) {
  {
    SHA512Helper sha512g;
    sha512g.Update("argcv");
    sha512g.Finish();
    EXPECT_EQ(
        sha512g.ToString(),
        "a6aff7644f4a706feadfd1f875fa35f411c6cd55585e7f323bc6ec9f7807abce935226"
        "28efe0bf9fe2c9f27f06c58f1b617985c485ad6431f76579b67d6e8064");
  }

  {
    SHA512Helper sha512g;
    sha512g.Update(
        "a6aff7644f4a706feadfd1f875fa35f411c6cd55585e7f323bc6ec9f7807abce935226"
        "28efe0bf9fe2c9f27f06c58f1b617985c485ad6431f76579b67d6e8064");
    sha512g.Finish();
    EXPECT_EQ(
        sha512g.ToString(),
        "173d12dbf177c3a4fae57064a9694cba303f3c4dc39d16bf712ea67b7329a2125046da"
        "2e50df086a7221f18f965d915db7f448858cf8d56ab9e3ab1f16a3cd68");
  }
}

}  // namespace argcv
