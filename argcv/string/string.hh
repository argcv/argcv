#ifndef ARGCV_STRING_STRING_HH
#define ARGCV_STRING_STRING_HH

#include <vector>
#include <string>
#include <algorithm>

namespace argcv { namespace string {

inline std::vector<std::string> &split(const std::string &s, const std::string &delim,
    std::vector<std::string> *_elems) {
    std::string item;
    if (delim.empty()) {
        _elems->push_back(s);
        return * _elems;
    }
    std::string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        std::string temp(substart, subend);
        // if (keep_empty || !temp.empty()) {
        if (!temp.empty()) {
            _elems->push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return * _elems;
}

inline std::vector<std::string> split(const std::string &s, const std::string &delim) {
    std::vector<std::string> elems;
    split(s, delim, & elems);
    return elems;
}


}}  // argcv::string

#endif  //  ARGCV_STRING_STRING_HH

