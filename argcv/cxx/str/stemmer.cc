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
#include "argcv/cxx/str/stemmer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

#include "argcv/c/char/char_helper.h"
#include "argcv/cxx/str/str.h"
#include "third_party/porter2_stemmer/porter2_stemmer.h"

namespace argcv {

/**
 * stemm a string
 */
std::vector<std::string> StemmString(const std::string &str) {
  std::vector<std::string> rt;
  std::string buff;
  size_t sz = str.length();
  uint8_t m = 0;
  for (size_t i = 0; i < sz; i++) {
    char ch = str[i];
    uint8_t cm = 0;
    if (isupper(ch) || islower(ch)) {
      cm = 1;
      ch = tolower(ch);
    } else if (ch >= '0' && ch <= '9') {
      cm = 2;
    } else {
      switch (ch) {
        case '\r':
        case '\n':
        case '\t':
        case '\b':
        case ' ':
          cm = 0;
          break;
        case '\'':
        case '\"':
        case '/':
        case '[':
        case ']':
        case '%':
        case '&':
        case '_':
        case '(':
        case ')':
        case ',':
        case ':':
          cm = 3;
          break;
        default:
          cm = 4;
      }
    }

    if (m != cm) {
      if (buff.length() > 0) {
        if (utf8_char_length(buff[0]) == 1) {
          Porter2Stemmer::stem(buff);
        }
        rt.push_back(buff);
        buff.clear();
      }
      m = cm;
    }
    if (cm != 0) {
      buff += ch;
    }
  }
  if (buff.length() > 0) {
    if (utf8_char_length(buff[0]) == 1) {
      Porter2Stemmer::stem(buff);
    }
    rt.push_back(buff);
  }
  return rt;
}

/**
 * stemm a string, it will remove all Non ASCII words
 */
std::vector<std::string> StemmStringClear(const std::string &str) {
  std::vector<std::string> rt;
  std::string buff;
  size_t sz = str.length();
  for (size_t i = 0; i < sz; i++) {
    char ch = str[i];
    if (isupper(ch) || islower(ch)) {
      buff += tolower(ch);  // forces lower case
    } else {
      Porter2Stemmer::stem(buff);
      if (buff.length() > 0) {
        rt.push_back(buff);
        buff.clear();
      }
    }
  }
  Porter2Stemmer::stem(buff);
  if (buff.length() > 0) {
    rt.push_back(buff);
    buff.clear();
  }
  return rt;
}

/**
 * stemm a word
 */
std::string StemmWord(const std::string &word) {
  std::string buff = word;
  Porter2Stemmer::stem(buff);
  return buff;
}

}  // namespace argcv
