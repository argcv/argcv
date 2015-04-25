#ifndef ARGCV_ARGCV_HH
#define ARGCV_ARGCV_HH

#include <cstdio>

namespace argcv{
class argcv_info {
public:
    argcv_info(){
        printf("[argcv] Hello, World!\n");
    }
    
    virtual ~argcv_info();

private:

};

}

#endif // ARGCV_ARGCV_HH