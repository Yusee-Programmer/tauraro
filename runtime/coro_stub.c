/* No-op coroutine-scheduling entry points for the LLVM/native backend's SYNCHRONOUS
 * async model: `await f()` is lowered to a direct call (yielding exactly the value the
 * C backend's coroutine+join produces), so cooperative yield points (sleep/yield) don't
 * affect any deterministic result. A SEPARATE translation unit (does NOT include
 * tauraro_rt.h) so its EXPORTED symbols provide the linker definitions the emitted code
 * needs, without colliding with the header's file-local `static` versions. Real
 * multi-task concurrency (Coro.spawn / scheduler) is not modeled here. */
void _tr_co_yield_h(void) { }
void _tr_co_sleep_h(long long ms) { (void)ms; }
void _tr_co_run_h(void) { }
void _tr_co_spawn_h(void* fn, void* arg) { (void)fn; (void)arg; }
long long _tr_co_await_fd_h(long long fd, long long ev) { (void)fd; (void)ev; return 0; }
long long _tr_co_await_h(char* c) { (void)c; return 0; }
char* _tr_co_go_h(void* fn, void* arg) { (void)fn; (void)arg; return (char*)0; }
void _tr_co_free_h(char* c) { (void)c; }
int _tr_co_done_h(char* c) { (void)c; return 1; }
