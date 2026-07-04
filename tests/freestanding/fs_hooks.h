/* Freestanding platform hooks for the arm-none-eabi boundary check. Defines the
 * tier + a pluggable allocator so the runtime pulls NO libc allocator. */
#define TAURARO_KERNEL
#include <stddef.h>
void* _fs_alloc(size_t);   void  _fs_free(void*);
void* _fs_realloc(void*, size_t);   void* _fs_calloc(size_t, size_t);
#define TAURARO_ALLOC(sz)     _fs_alloc(sz)
#define TAURARO_FREE(p)       _fs_free(p)
#define TAURARO_REALLOC(p,sz) _fs_realloc(p,sz)
#define TAURARO_CALLOC(n,sz)  _fs_calloc(n,sz)
