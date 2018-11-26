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
#include "argcv/cxx/type/byte.h"

#include <iostream>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace argcv {

TEST(Bytes, ToHexString) {
  uint64_t d = 0x1223344556677889;
  std::string hex = ToBytes(d).ToHexString();
  EXPECT_EQ("1223344556677889", hex);
}

TEST(Bytes, FromHexString) {
  uint64_t d = 0x1223344556677889;
  std::string hex = ToBytes(d).ToHexString();
  Bytes ba;
  EXPECT_TRUE(ba.FromHexString(hex));
  EXPECT_EQ("1223344556677889", ba.ToHexString());
  uint64_t d2;
  EXPECT_EQ(true, ba.Write(&d2, 0));
  EXPECT_EQ(d, d2);
}

TEST(Bytes, ToBytes) {
  {
    // 1 uint64_t
    uint64_t d = 0x1223344556677889;
    Bytes ba = ToBytes(d);
    EXPECT_EQ(0x12, static_cast<int64_t>(ba[0]));
    EXPECT_EQ(0x23, static_cast<Byte>(ba[1]));
    EXPECT_EQ(0x34, static_cast<Byte>(ba[2]));
    EXPECT_EQ(0x45, static_cast<Byte>(ba[3]));
    EXPECT_EQ(0x56, static_cast<Byte>(ba[4]));
    EXPECT_EQ(0x67, static_cast<Byte>(ba[5]));
    EXPECT_EQ(0x78, static_cast<Byte>(ba[6]));
    EXPECT_EQ(0x89, static_cast<Byte>(ba[7]));
  }

  {
    // 2 uint32_t
    uint32_t d0 = 0x12233445;
    uint32_t d1 = 0x56677889;
    Bytes ba = ToBytes({d0, d1});
    // std::vector<uint32_t> dv;
    // dv.push_back(d0);
    // dv.push_back(d1);
    // Bytes ba = ToBytes<uint32_t>(dv);
    // std::cerr << "0: 0x" << std::hex << static_cast<uint16_t>(ba[0]) <<
    // std::endl; std::cerr << "1: 0x" << std::hex <<
    // static_cast<uint16_t>(ba[1]) << std::endl; std::cerr << "2: 0x" <<
    // std::hex << static_cast<uint16_t>(ba[2]) << std::endl; std::cerr << "3:
    // 0x" << std::hex << static_cast<uint16_t>(ba[3]) << std::endl; std::cerr
    // << "4: 0x" << std::hex << static_cast<uint16_t>(ba[4]) << std::endl;
    // std::cerr << "5: 0x" << std::hex << static_cast<uint16_t>(ba[5]) <<
    // std::endl; std::cerr << "6: 0x" << std::hex <<
    // static_cast<uint16_t>(ba[6]) << std::endl; std::cerr << "7: 0x" <<
    // std::hex << static_cast<uint16_t>(ba[7]) << std::endl;
    EXPECT_EQ(0x12, static_cast<Byte>(ba[0]));
    EXPECT_EQ(0x23, static_cast<Byte>(ba[1]));
    EXPECT_EQ(0x34, static_cast<Byte>(ba[2]));
    EXPECT_EQ(0x45, static_cast<Byte>(ba[3]));
    EXPECT_EQ(0x56, static_cast<Byte>(ba[4]));
    EXPECT_EQ(0x67, static_cast<Byte>(ba[5]));
    EXPECT_EQ(0x78, static_cast<Byte>(ba[6]));
    EXPECT_EQ(0x89, static_cast<Byte>(ba[7]));
  }
}

TEST(Bytes, Write) {
  {
    // 1 load one elem
    uint64_t d = 0x1223344556677889;
    Bytes ba = ToBytes(d);
    uint16_t o;
    ba.Write(&o, 0);
    EXPECT_EQ(0x1223, o);
    ba.Write(&o, 2);
    EXPECT_EQ(0x3445, o);
  }

  {
    // 2 load as vector
    uint64_t d = 0x122334455667aaff;
    Bytes ba = ToBytes(d);
    std::vector<uint16_t> ov;
    ba.Write(&ov);
    EXPECT_EQ(sizeof(uint64_t) / sizeof(uint16_t), ov.size());
    EXPECT_EQ(0x1223, ov[0]);
    EXPECT_EQ(0x3445, ov[1]);
    EXPECT_EQ(0x5667, ov[2]);
    EXPECT_EQ(0xaaff, ov[3]);
  }
}

TEST(Bytes, IOStream) {
  uint64_t d = 0x122334455667aaff;
  Bytes ba = ToBytes(d);
  LOG(INFO) << "#LOG# HEX[" << ba << "]";
  std::cout << "#OUT# HEX[" << ba << "]" << std::endl;
  std::cerr << "#ERR# HEX[" << ba << "]" << std::endl;
}

}  // namespace argcv
