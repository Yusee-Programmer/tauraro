/* Leak probe for the native backend's ARC. Linked into a leak-check binary alongside the
 * program object and runtime.o (built with -DTAURARO_NMEM). A destructor runs after main()
 * returns and asserts no refcounted strings are still live — i.e. the generated retain/
 * release calls are balanced. Only valid for programs with no string globals (globals are
 * intentionally never dropped). */
#include <stdio.h>
#include <stdlib.h>

extern long long _tr_rt_str_live_count(void);

__attribute__((destructor)) static void _tr_native_leak_check(void) {
    long long live = _tr_rt_str_live_count();
    if (live < 0) return;                    /* counter disabled (no -DTAURARO_NMEM) */
    if (live != 0) {
        fprintf(stderr, "\nNATIVE ARC LEAK: %lld string(s) still live at exit\n", live);
        _exit(70);
    }
    fprintf(stderr, "native ARC: 0 strings leaked\n");
}
