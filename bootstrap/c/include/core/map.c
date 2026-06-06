#include "../../tauraro_types.h"


__attribute__((hot)) long long _map_hash(void* key, long long cap) {
    /* pass */
    unsigned long long h = ((unsigned long long)(-3750763034362895579LL));
    /* pass */
    char* p = ((char*)(key));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        unsigned long long c = ((unsigned long long)((*(p + i))));
        /* pass */
        if ((c == ((unsigned long long)(0LL)))) {
            /* pass */
            break;
        }
        /* pass */
        h = ((h ^ c) * ((unsigned long long)(1099511628211LL)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (((long long)(h)) % cap);
}

