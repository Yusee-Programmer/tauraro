/* Platform hooks for the mps2-an385 core-tier build (force-included before the
 * runtime). --freestanding already emits #define TAURARO_KERNEL into the C; here
 * we supply the pluggable allocator + the _TR_WRITE output sink it requires. */
#include <stddef.h>
void* _fs_alloc(size_t);  void  _fs_free(void*);
void* _fs_calloc(size_t, size_t);  void* _fs_realloc(void*, size_t);
void  _bare_write(const char*);
#define TAURARO_ALLOC(sz)     _fs_alloc(sz)
#define TAURARO_FREE(p)       _fs_free(p)
#define TAURARO_REALLOC(p,sz) _fs_realloc(p,sz)
#define TAURARO_CALLOC(n,sz)  _fs_calloc(n,sz)
#define _TR_WRITE(s)          _bare_write(s)
