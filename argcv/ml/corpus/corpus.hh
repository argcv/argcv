#ifndef ARGCV_ML_CORPUS_CORPUS_HH
#define ARGCV_ML_CORPUS_CORPUS_HH

#include <cmath>
#include <cstdio>

namespace argcv { namespace ml { namespace corpus {

// tf-idf
// assume : k in document D
// stid : size of term k in  D
// atsid :  all term size in D
// ads : all document size
// dscct : document size contains current term
inline double tf_idf(size_t stid, size_t atsid, size_t ads, size_t dscct) {
    // #define MATH_LG_10 2.302585
    // tf * idf
    if (ads == 0 || atsid == 0 || dscct == 0) return 0;
    return (static_cast<double>(stid) / atsid) 
        * log(static_cast<double>(ads) / (dscct))/2.302585;
}



}}}  // namespace argcv::ml::corpus

#endif  //  ARGCV_ML_CORPUS_CORPUS_HH
