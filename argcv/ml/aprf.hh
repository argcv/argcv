#ifndef ARGCV_ML_APRF_HH
#define ARGCV_ML_APRF_HH

// accuracy,precision,recall and f1_measure

#include <cstdint>  // uint64_t

#include <string>
#include <vector>
#include <utility>  // std::pair, std::make_pair

#include "ml.hh"

namespace argcv {
namespace ml {
class aprf {
public:
    aprf() : tp(0), tn(0), fp(0), fn(0) {}
    ~aprf() {}

    void add(bool predicted, bool actual) {
        if (predicted && actual)
            tp++;
        else if (!predicted && actual)
            fn++;
        else if (predicted && !actual)
            fp++;
        else if (!predicted && !actual)
            tn++;
    }

    double accuracy() {
        size_t all = tp + fp + tn + fn;
        if (all == 0) return 0;
        return (double)(tp + tn) / all;
    }

    double precision() {
        if (tp + fp == 0)
            return 0;
        else
            return (double)tp / (tp + fp);
    }

    double recall() {
        if (tp + fn == 0)
            return 0;
        else
            return (double)tp / (tp + fn);
    }

    double f1() {
        //double p = precision();
        //double r = recall();
        //if (p + r == 0) return 0;
        //return 2 * p * r / (p + r);
        return f(1);
    }

    double f(size_t a) {
        if (a < 1) return 0;
        double p = precision();
        double r = recall();
        if (p + r == 0) return 0;
        double a2 = a * a;
        return (a2 + 1) * p * r / (a2 * (p + r));
    }

    void reset() { tp = tn = fp = fn = 0; }

private:
    size_t tp;
    size_t tn;
    size_t fp;
    size_t fn;
};
}
}  // namespace argcv::ml

#endif  //  ARGCV_ML_APRF_HH