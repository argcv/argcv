#include <cassert>
#include <cstdio>
#include <string>

#include "glog/logging.h"
#include "re2/re2.h"

// Please refer to https://github.com/google/re2/wiki/Syntax for the syntax

void example_matching() {
  // https://github.com/google/re2/wiki/CplusplusAPI#matching-interface
  LOG(INFO) << "[FullMatch] hello, h.*o: " << std::boolalpha
            << RE2::FullMatch("hello", "h.*o");
  LOG(INFO) << "[FullMatch] hello, e: " << std::boolalpha
            << RE2::FullMatch("hello", "e");

  LOG(INFO) << "[PartialMatch] hello, h.*o: " << std::boolalpha
            << RE2::PartialMatch("hello", "h.*o");
  LOG(INFO) << "[PartialMatch] hello, e: " << std::boolalpha
            << RE2::PartialMatch("hello", "e");
}

void example_submatch_extraction() {
  // https://github.com/google/re2/wiki/CplusplusAPI#submatch-extraction
  // Successful parsing.
  int i;
  std::string s;

  LOG(INFO) << "[FullMatch] ruby:1234, (\\w+):(\\d+): " << std::boolalpha
            << RE2::FullMatch("ruby:1234", "(\\w+):(\\d+)", &s, &i);
  LOG(INFO) << "[Extraction Result] s: " << s << ", i: " << i;

  LOG(INFO) << "[FullMatch] ruby:1234, (.+):" << std::boolalpha
            << RE2::FullMatch("ruby:1234", "(.+)", &i);
  // The above line returns 'false'
  // Can not be parsed as an integer
  // LOG(INFO) << "[Extraction Result] s: " << s << ", i: " << i;

  LOG(INFO) << "[PartialMatch] ruby:1234, (\\d+):" << std::boolalpha
            << RE2::PartialMatch("ruby:1234", "(\\d+)", &i);
  LOG(INFO) << "[Extraction Result] i: " << i;

  LOG(INFO) << "[FullMatch] ruby:1234, (\\w+):(\\d+):" << std::boolalpha
            << RE2::FullMatch("ruby:1234", "(\\w+):(\\d+)", &s);
  // Success; does not extract the number.
  LOG(INFO) << "[Extraction Result] s: " << s;

  LOG(INFO) << "[FullMatch] ruby:1234, (\\w+):(\\d+):" << std::boolalpha
            << RE2::FullMatch("ruby:1234", "(\\w+):(\\d+)", (void *)nullptr,
                              &i);
  // Success; skips NULL argument.
  LOG(INFO) << "[Extraction Result] i: " << i;

  // Fails: integer overflow keeps value from being stored in i.
  LOG(INFO) << "[FullMatch] ruby:123456789123, (\\w+):(\\d+):" << std::boolalpha
            << RE2::FullMatch("ruby:123456789123", "(\\w+):(\\d+)", &s, &i);
}

void example_pre_compiled_regex() {
  // https://github.com/google/re2/wiki/CplusplusAPI#pre-compiled-regular-expressions
  RE2 re("(\\w+):(\\d+)");
  LOG(INFO) << "check status: " << re.ok();

  int i;
  std::string s;

  LOG(INFO) << "[FullMatch] ruby:1234, re: " << std::boolalpha
            << RE2::FullMatch("ruby:1234", re, &s, &i);
  LOG(INFO) << "[Extraction Result] s: " << s << ", i: " << i;

  LOG(INFO) << "[FullMatch] ruby:1234, re:" << std::boolalpha
            << RE2::FullMatch("ruby:1234", re, &s);
  // Success; does not extract the number.
  LOG(INFO) << "[Extraction Result] s: " << s;

  LOG(INFO) << "[FullMatch] ruby:1234, re:" << std::boolalpha
            << RE2::FullMatch("ruby:1234", re, (void *)nullptr, &i);
  // Success; skips NULL argument.
  LOG(INFO) << "[Extraction Result] i: " << i;

  // Fails: integer overflow keeps value from being stored in i.
  LOG(INFO) << "[FullMatch] ruby:123456789123, re:" << std::boolalpha
            << RE2::FullMatch("ruby:123456789123", re, &s, &i);
}

void example_options() {
  // https://github.com/google/re2/wiki/CplusplusAPI#options
  // // don't write to stderr for parser failure

  {
    RE2 re("(ab", RE2::Quiet);
    LOG(INFO) << "check status: " << re.ok();
    LOG(INFO) << "retrieve error: " << re.error();
  }

  {
    // no quiet
    RE2 re("(ab");
    LOG(INFO) << "check status: " << re.ok();
  }
}

int main(int argc, char *argv[]) {
  LOG(INFO) << "Starting...";
  example_matching();
  example_submatch_extraction();
  example_pre_compiled_regex();
  example_options();
  return 0;
}