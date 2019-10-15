#ifndef STRSETCONST
#define STRSETCONST

#include <iostream>

#include "strset.h"
#include "strsetconst.h"

using namespace std;
using namespace jnp1;

namespace {

    auto &err() {
        static ios_base::Init init;
        static ostream &error = cerr;
        return error;
    }

    bool was_it_created = false;
    unsigned long id42;
    
#ifdef NDEBUG
    const bool debug = false;
#else
    const bool debug = true;
#endif
}

namespace jnp1 {

    unsigned long strset42() {
        if (!was_it_created) {
            if (debug)
                err() << "strsetconst init invoked" << endl;
            
            id42 = strset_new();
            was_it_created = true;
            strset_insert(id42, "42");
            
            if (debug)
                err() << "strsetconst init finished" << endl;
        }
        
        return id42;
    }
}
#endif
