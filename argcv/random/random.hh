#ifndef ARGCV_RANDOM_RANDOM_HH
#define ARGCV_RANDOM_RANDOM_HH

#include <cstdlib>
#include <ctime>

#include <string>

namespace argcv {
namespace random {

inline int random_int() {
    static unsigned int seed = (unsigned int)time(NULL);
    return rand_r(&seed);
}

inline std::string random_str(const int len) {
    static const char alphanum[] = "0123456789"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz";
    std::string s;
    for (int i = 0; i < len; ++i) {
        s += alphanum[random_int() % (sizeof(alphanum) - 1)];
    }
    return s;
}
}
}  //  namespace argcv::random

#endif  //  ARGCV_RANDOM_RANDOM_HH