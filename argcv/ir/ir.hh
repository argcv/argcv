#ifndef ARGCV_IR_IR_HH
#define ARGCV_IR_IR_HH

// Information retrieval (IR) is the activity of obtaining information
// resources relevant to an information need from a collection of
// information resources. Searches can be based on metadata or
// on full-text (or other content-based) indexing.
// ref: http://en.wikipedia.org/wiki/Information_retrieval

#include <cmath>

namespace argcv {
namespace ir {

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
    return (static_cast<double>(stid) / atsid) * log(static_cast<double>(ads) / (dscct)) / 2.302585;
}
}
}  // namespace argcv::ir

#endif  //  ARGCV_IR_IR_HH
