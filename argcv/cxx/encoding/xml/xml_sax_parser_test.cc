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

#include "argcv/cxx/encoding/xml/xml_sax_parser.h"

#include <string>

#include "argcv/cxx/str/str_helper.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::StrPrintf;

using argcv::Trim;
using argcv::XmlAttrElems;
using argcv::XmlAttrPairs;
using argcv::XmlSAXParser;

TEST(XmlSAXParser, Basic) {
  std::string data(
      "<?xml version=\"1.0\"?>\n"
      "<gjob:Helping\n"
      "    xmlns:gjob=\"http://www.gnome.org/some-location\">\n"
      "    <gjob:Jobs>\n"
      "        <gjob:Job>\n"
      "            <gjob:Project ID=\"3\"/>\n"
      "            <gjob:Application>GBackup</gjob:Application>\n"
      "            <gjob:Category>Development</gjob:Category>\n"
      "            <gjob:Update>\n"
      "                <gjob:Status>Open</gjob:Status>\n"
      "                <gjob:Modified>Mon, 07 Jun 1999 20:27:45 -0400 MET "
      "DST</gjob:Modified>\n"
      "                <gjob:Salary>USD 0.00</gjob:Salary>\n"
      "            </gjob:Update>\n"
      "            <gjob:Developers>\n"
      "                <gjob:Developer></gjob:Developer>\n"
      "            </gjob:Developers>\n"
      "            <gjob:Contact>\n"
      "                <gjob:Person>Nathan Clemons</gjob:Person>\n"
      "                <gjob:Email>nathan@windsofstorm.net</gjob:Email>\n"
      "                <gjob:Company></gjob:Company>\n"
      "                <gjob:Organisation></gjob:Organisation>\n"
      "                <gjob:Webpage></gjob:Webpage>\n"
      "                <gjob:Snailmail></gjob:Snailmail>\n"
      "                <gjob:Phone></gjob:Phone>\n"
      "            </gjob:Contact>\n"
      "            <gjob:Requirements>\n"
      "              <![CDATA[\n"
      "                  Since this is a CDATA section\n"
      "                  I can use all sorts of reserved characters\n"
      "                  like > < \" and &\n"
      "                  or write things like\n"
      "                  <foo></bar>\n"
      "                  but my document is still well formed!\n"
      "              ]]>\n"
      "      The program should be released as free software, under the GPL.\n"
      "      </gjob:Requirements>\n"
      "            <gjob:Skills></gjob:Skills>\n"
      "            <gjob:Details>\n"
      "              <!--\n"
      "                  Since this is a comment\n"
      "                  I can use all sorts of reserved characters\n"
      "                  like > < \" and &\n"
      "                  or write things like\n"
      "                  <foo></bar>\n"
      "                  but my document is still well-formed!\n"
      "              -->\n"
      "      A GNOME based system that will allow a superuser to configure \n"
      "      compressed and uncompressed files and/or file systems to be "
      "backed \n"
      "      up with a supported media in the system.  This should be able to "
      "\n"
      "      perform via find commands generating a list of files that are "
      "passed \n"
      "      to tar, dd, cpio, cp, gzip, etc., to be directed to the tape "
      "machine \n"
      "      or via operations performed on the filesystem itself. Email \n"
      "      notification and GUI status display very important.\n"
      "      </gjob:Details>\n"
      "        </gjob:Job>\n"
      "    </gjob:Jobs>\n"
      "</gjob:Helping>\n"
      "\n"
      "");
  std::string out;
  XmlSAXParser handler;
  handler
      .OnStartElement([&](const std::string &name, const XmlAttrPairs &attrs) {
        out += argcv::StrPrintf("<%s ", name.c_str());
        for (auto attr : attrs) {
          out += argcv::StrPrintf("'%s'=<%s> ", attr.first.c_str(),
                                  attr.second.c_str());
        }
        out += argcv::StrPrintf(">\n");
        // LOG(INFO) << "OUT[#1]:" << out;
      })
      .OnEndElement([&](const std::string &name) {
        out += argcv::StrPrintf("</%s>\n", name.c_str());
        // LOG(INFO) << "OUT[#1]:" << out;
      })
      .OnCharacters([&](const std::string &buff) {
        std::string s(buff);
        s = Trim(s);
        if (s.length() > 0) {
          out += argcv::StrPrintf("${%s}\n", s.c_str());
        }
        // LOG(INFO) << "OUT[#2]:" << out;
      })
      .OnCdataBlock([&](const std::string &buff) {
        std::string s(buff);
        s = Trim(s);
        if (s.length() > 0) {
          out += argcv::StrPrintf("$[[%s]]\n", s.c_str());
        }
        // LOG(INFO) << "OUT[#3]:" << out;
      })
      .OnComment([&](const std::string &buff) {
        std::string s(buff);
        s = Trim(s);
        if (s.length() > 0) {
          out += argcv::StrPrintf("$<!-- %s -->\n", s.c_str());
        }
        // LOG(INFO) << "OUT[#4]:" << out;
      })
      .OnWarning([&](const std::string &msg) { LOG(WARNING) << msg; })
      .OnError([&](const std::string &msg) { LOG(ERROR) << msg; });

  int status = handler.ReadString(data, "abc.xml");
  if (status != 0) {
    LOG(ERROR) << "xml read error" << status;
  }

  LOG(INFO) << "output of xml: " << out;
  EXPECT_GT(out.length(), 0);
  //
  // EXPECT_GT(lhs, rhs_sm);
  // EXPECT_NE(lhs, rhs_sm);
  // EXPECT_GE(lhs, rhs_eq);
  // EXPECT_EQ(lhs, rhs_eq);
  // EXPECT_LE(lhs, rhs_eq);
  // EXPECT_LT(lhs, rhs_gt);
  // EXPECT_NE(lhs, rhs_gt);
  //
  // EXPECT_TRUE(lhs > rhs_sm);
  // EXPECT_TRUE(lhs != rhs_sm);
  // EXPECT_TRUE(lhs >= rhs_eq);
  // EXPECT_TRUE(lhs == rhs_eq);
  // EXPECT_TRUE(lhs <= rhs_eq);
  // EXPECT_TRUE(lhs < rhs_gt);
  // EXPECT_TRUE(lhs != rhs_gt);
  //
  // EXPECT_FALSE(lhs < rhs_sm);
  // EXPECT_FALSE(lhs <= rhs_sm);
  // EXPECT_FALSE(lhs != rhs_eq);
  // EXPECT_FALSE(lhs > rhs_gt);
}
