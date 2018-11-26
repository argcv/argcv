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
 * bazel run //examples/cxx/libxml2:gjobread $(pwd)/examples/c/libxml2/gjobs.xml
 **/
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "argcv/cxx/encoding/xml/xml_sax_parser.h"

using argcv::XmlAttrElems;
using argcv::XmlAttrPairs;
using argcv::XmlSAXParser;

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](int ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  for (int i = 1; i < argc; i++) {
    const std::string cname(argv[i]);
    FILE *f = fopen(cname.c_str(), "r");
    if (!f) {
      LOG(ERROR) << "file open error.";
      continue;
    }

    int xi = 0;
    auto mg_left = [](int n) {
      for (int i = 0; i < n; i++) {
        putchar('\t');
      }
    };
    XmlSAXParser handler;
    handler
        .AddDTDSystemURIMapping("dblp-2017-08-29.dtd",
                                "http://dblp.uni-trier.de/xml/dblp.dtd")
        .OnStartElement(
            [&](const std::string &name, const XmlAttrPairs &attrs) {
              mg_left(xi);
              printf("<%s ", name.c_str());
              for (auto attr : attrs) {
                printf("'%s'=<%s> ", attr.first.c_str(), attr.second.c_str());
              }
              printf(">\n");
              xi++;
            })
        .OnEndElement([&](const std::string &name) {
          xi--;
          mg_left(xi);
          printf("</%s>\n", name.c_str());
        })
        .OnCharacters([&](const std::string &buff) {
          std::string s(buff);
          trim(s);
          if (s.length() > 0) {
            mg_left(xi);
            printf("${%s}\n", s.c_str());
          }
        })
        .OnCdataBlock([&](const std::string &buff) {
          std::string s(buff);
          trim(s);
          if (s.length() > 0) {
            mg_left(xi);
            printf("$[[%s]]\n", s.c_str());
          }
        })
        .OnComment([&](const std::string &buff) {
          std::string s(buff);
          trim(s);
          if (s.length() > 0) {
            mg_left(xi);
            printf("$<!-- %s -->\n", s.c_str());
          }
        })
        .OnWarning([&](const std::string &msg) { LOG(WARNING) << msg; })
        .OnError([&](const std::string &msg) { LOG(ERROR) << msg; });

    int status = handler.ReadFile(f, cname.c_str());
    if (status != 0) {
      LOG(ERROR) << "xml read error" << status;
    }

    fclose(f);
  }

  return 0;
}
