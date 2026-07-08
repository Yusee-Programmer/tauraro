/* Minimal bare-metal platform: a bump allocator over a static arena + the result
 * sink. This is what a real target (or an RTOS/allocator crate) would provide. */
#include <stddef.h>
static unsigned char _arena[1u<<20]; static size_t _off = 0;
void* _fs_alloc(size_t n){ if(_off+n > sizeof(_arena)) return 0; void* p=&_arena[_off]; _off += (n+15u)&~15u; return p; }
void  _fs_free(void* p){ (void)p; }              /* bump: no per-object free */
void* _fs_calloc(size_t c, size_t s){ size_t n=c*s; unsigned char* p=_fs_alloc(n); if(p){ for(size_t i=0;i<n;i++) p[i]=0; } return p; }
void* _fs_realloc(void* p, size_t n){ unsigned char* q=_fs_alloc(n); if(p&&q){ unsigned char* a=p; for(size_t i=0;i<n;i++) q[i]=a[i]; } return q; }
volatile long g_fs_result = 0;
void _fs_result(long long s){ g_fs_result = (long)s; }   /* expect 4950 */
