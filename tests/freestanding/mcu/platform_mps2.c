/* Bare-metal platform for QEMU mps2-an385: a bump allocator over a static arena
 * (the pluggable allocator the core runtime calls) + a byte sink over the ARM
 * CMSDK UART0 (what _TR_WRITE routes print() through). No libc, no OS. */
#include <stddef.h>
#include <stdint.h>
#define UART0_DATA (*(volatile uint32_t*)0x40004000u)   /* CMSDK UART0 TX/RX data  */
#define UART0_CTRL (*(volatile uint32_t*)0x40004008u)   /* bit0 = TX enable        */
static int _uart_up = 0;
void _bare_write(const char* s) {
    if (!_uart_up) { UART0_CTRL = 1u; _uart_up = 1; }
    if (s) while (*s) UART0_DATA = (uint32_t)(unsigned char)*s++;
}
static unsigned char _arena[512u*1024u];
static size_t _off = 0;
void* _fs_alloc(size_t n)            { if (_off+n > sizeof(_arena)) return 0; void* p=&_arena[_off]; _off += (n+7u)&~7u; return p; }
void  _fs_free(void* p)              { (void)p; }
void* _fs_calloc(size_t c, size_t s) { size_t n=c*s; unsigned char* p=(unsigned char*)_fs_alloc(n); if(p) for(size_t i=0;i<n;i++) p[i]=0; return p; }
void* _fs_realloc(void* p, size_t n) { unsigned char* q=(unsigned char*)_fs_alloc(n); if(p&&q) for(size_t i=0;i<n;i++) q[i]=((unsigned char*)p)[i]; return q; }
