/*
 * std/sys/sandbox_native.c -- process sandboxing for std.sys.sandbox.
 *
 *   Linux:   seccomp-BPF syscall filters built from pledge-style promises,
 *            custom allow/deny lists, and Landlock filesystem rules (unveil).
 *   OpenBSD: pledge(2) and unveil(2) directly.
 *   Other:   reported as unsupported.
 *
 * Linked through `# tauraro-cpp-shim: sandbox_native.c` in std/sys/sandbox.tr.
 */
#if defined(__linux__) && !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#if !defined(_WIN32)
#  include <unistd.h>
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <fcntl.h>
#  include <sys/stat.h>
#endif

#if defined(__linux__)
#  include <sys/syscall.h>
#  include <sys/prctl.h>
#  include <linux/filter.h>
#  include <linux/seccomp.h>
#  include <linux/audit.h>
#endif

static char sb_err[256];
/* Copy the last error message into dst (cap bytes); returns its length. */
int64_t trc_sb_error_into(char* dst, int64_t cap) {
    size_t n = strlen(sb_err);
    if ((int64_t)n > cap - 1) n = (size_t)(cap - 1);
    memcpy(dst, sb_err, n); dst[n] = 0;
    return (int64_t)n;
}
static void sb_set_err(const char* m) { strncpy(sb_err, m, sizeof sb_err - 1); sb_err[sizeof sb_err - 1] = 0; }
#if defined(__linux__) || defined(__OpenBSD__)
static void sb_set_errno(const char* m) {
    size_t n; strncpy(sb_err, m, sizeof sb_err - 1); sb_err[sizeof sb_err - 1] = 0;
    n = strlen(sb_err);
    if (n + 2 < sizeof sb_err) { sb_err[n++] = ':'; sb_err[n++] = ' '; sb_err[n] = 0; }
    strncat(sb_err, strerror(errno), sizeof sb_err - strlen(sb_err) - 1);
}
#endif

/* 1 seccomp + Landlock (Linux), 2 pledge/unveil (OpenBSD), 0 none. */
int64_t trc_sb_backend(void) {
#if defined(__OpenBSD__)
    return 2;
#elif defined(__linux__) && (defined(__x86_64__) || defined(__aarch64__) || defined(__i386__) || defined(__arm__) || (defined(__riscv) && __riscv_xlen == 64))
    return 1;
#else
    return 0;
#endif
}

/* ── fork / wait helpers (POSIX) ────────────────────────────────────────── */
int64_t trc_sb_fork(void) {
#if defined(_WIN32)
    return -1;
#else
    fflush(NULL);          /* the child must not re-emit the parent's buffered output */
    return (int64_t)fork();
#endif
}
/* Exit code of child `pid`, or 1000 + signal number when it was killed. */
int64_t trc_sb_wait(int64_t pid) {
#if defined(_WIN32)
    (void)pid; return -1;
#else
    int st = 0;
    while (waitpid((pid_t)pid, &st, 0) < 0) { if (errno != EINTR) return -1; }
    if (WIFEXITED(st)) return WEXITSTATUS(st);
    if (WIFSIGNALED(st)) return 1000 + WTERMSIG(st);
    return -1;
#endif
}
void trc_sb_exit(int64_t code) {
#if defined(_WIN32)
    exit((int)code);
#else
    fflush(stdout); fflush(stderr);
    _exit((int)code);
#endif
}

#if defined(__linux__)
/* name -> number for every syscall this file refers to; absent ones are skipped. */
typedef struct { const char* name; int nr; } sb_sysent;
static const sb_sysent sb_systab[] = {
#ifdef SYS__llseek
    {"_llseek", SYS__llseek},
#endif
#ifdef SYS__newselect
    {"_newselect", SYS__newselect},
#endif
#ifdef SYS_accept
    {"accept", SYS_accept},
#endif
#ifdef SYS_accept4
    {"accept4", SYS_accept4},
#endif
#ifdef SYS_access
    {"access", SYS_access},
#endif
#ifdef SYS_alarm
    {"alarm", SYS_alarm},
#endif
#ifdef SYS_arch_prctl
    {"arch_prctl", SYS_arch_prctl},
#endif
#ifdef SYS_arm_fadvise64_64
    {"arm_fadvise64_64", SYS_arm_fadvise64_64},
#endif
#ifdef SYS_bind
    {"bind", SYS_bind},
#endif
#ifdef SYS_brk
    {"brk", SYS_brk},
#endif
#ifdef SYS_cacheflush
    {"cacheflush", SYS_cacheflush},
#endif
#ifdef SYS_capget
    {"capget", SYS_capget},
#endif
#ifdef SYS_chdir
    {"chdir", SYS_chdir},
#endif
#ifdef SYS_chmod
    {"chmod", SYS_chmod},
#endif
#ifdef SYS_chown
    {"chown", SYS_chown},
#endif
#ifdef SYS_chown32
    {"chown32", SYS_chown32},
#endif
#ifdef SYS_clock_getres
    {"clock_getres", SYS_clock_getres},
#endif
#ifdef SYS_clock_gettime
    {"clock_gettime", SYS_clock_gettime},
#endif
#ifdef SYS_clock_gettime64
    {"clock_gettime64", SYS_clock_gettime64},
#endif
#ifdef SYS_clock_nanosleep
    {"clock_nanosleep", SYS_clock_nanosleep},
#endif
#ifdef SYS_clone
    {"clone", SYS_clone},
#endif
#ifdef SYS_clone3
    {"clone3", SYS_clone3},
#endif
#ifdef SYS_close
    {"close", SYS_close},
#endif
#ifdef SYS_close_range
    {"close_range", SYS_close_range},
#endif
#ifdef SYS_connect
    {"connect", SYS_connect},
#endif
#ifdef SYS_creat
    {"creat", SYS_creat},
#endif
#ifdef SYS_dup
    {"dup", SYS_dup},
#endif
#ifdef SYS_dup2
    {"dup2", SYS_dup2},
#endif
#ifdef SYS_dup3
    {"dup3", SYS_dup3},
#endif
#ifdef SYS_epoll_create
    {"epoll_create", SYS_epoll_create},
#endif
#ifdef SYS_epoll_create1
    {"epoll_create1", SYS_epoll_create1},
#endif
#ifdef SYS_epoll_ctl
    {"epoll_ctl", SYS_epoll_ctl},
#endif
#ifdef SYS_epoll_pwait
    {"epoll_pwait", SYS_epoll_pwait},
#endif
#ifdef SYS_epoll_pwait2
    {"epoll_pwait2", SYS_epoll_pwait2},
#endif
#ifdef SYS_epoll_wait
    {"epoll_wait", SYS_epoll_wait},
#endif
#ifdef SYS_eventfd
    {"eventfd", SYS_eventfd},
#endif
#ifdef SYS_eventfd2
    {"eventfd2", SYS_eventfd2},
#endif
#ifdef SYS_execve
    {"execve", SYS_execve},
#endif
#ifdef SYS_execveat
    {"execveat", SYS_execveat},
#endif
#ifdef SYS_exit
    {"exit", SYS_exit},
#endif
#ifdef SYS_exit_group
    {"exit_group", SYS_exit_group},
#endif
#ifdef SYS_faccessat
    {"faccessat", SYS_faccessat},
#endif
#ifdef SYS_faccessat2
    {"faccessat2", SYS_faccessat2},
#endif
#ifdef SYS_fadvise64
    {"fadvise64", SYS_fadvise64},
#endif
#ifdef SYS_fchdir
    {"fchdir", SYS_fchdir},
#endif
#ifdef SYS_fchmod
    {"fchmod", SYS_fchmod},
#endif
#ifdef SYS_fchmodat
    {"fchmodat", SYS_fchmodat},
#endif
#ifdef SYS_fchmodat2
    {"fchmodat2", SYS_fchmodat2},
#endif
#ifdef SYS_fchown
    {"fchown", SYS_fchown},
#endif
#ifdef SYS_fchown32
    {"fchown32", SYS_fchown32},
#endif
#ifdef SYS_fchownat
    {"fchownat", SYS_fchownat},
#endif
#ifdef SYS_fcntl
    {"fcntl", SYS_fcntl},
#endif
#ifdef SYS_fcntl64
    {"fcntl64", SYS_fcntl64},
#endif
#ifdef SYS_fdatasync
    {"fdatasync", SYS_fdatasync},
#endif
#ifdef SYS_flock
    {"flock", SYS_flock},
#endif
#ifdef SYS_fork
    {"fork", SYS_fork},
#endif
#ifdef SYS_fstat
    {"fstat", SYS_fstat},
#endif
#ifdef SYS_fstatat64
    {"fstatat64", SYS_fstatat64},
#endif
#ifdef SYS_fstatfs
    {"fstatfs", SYS_fstatfs},
#endif
#ifdef SYS_fstatfs64
    {"fstatfs64", SYS_fstatfs64},
#endif
#ifdef SYS_fsync
    {"fsync", SYS_fsync},
#endif
#ifdef SYS_ftruncate
    {"ftruncate", SYS_ftruncate},
#endif
#ifdef SYS_ftruncate64
    {"ftruncate64", SYS_ftruncate64},
#endif
#ifdef SYS_futex
    {"futex", SYS_futex},
#endif
#ifdef SYS_futex_waitv
    {"futex_waitv", SYS_futex_waitv},
#endif
#ifdef SYS_futimesat
    {"futimesat", SYS_futimesat},
#endif
#ifdef SYS_get_robust_list
    {"get_robust_list", SYS_get_robust_list},
#endif
#ifdef SYS_get_thread_area
    {"get_thread_area", SYS_get_thread_area},
#endif
#ifdef SYS_getcpu
    {"getcpu", SYS_getcpu},
#endif
#ifdef SYS_getcwd
    {"getcwd", SYS_getcwd},
#endif
#ifdef SYS_getdents
    {"getdents", SYS_getdents},
#endif
#ifdef SYS_getdents64
    {"getdents64", SYS_getdents64},
#endif
#ifdef SYS_getegid
    {"getegid", SYS_getegid},
#endif
#ifdef SYS_getegid32
    {"getegid32", SYS_getegid32},
#endif
#ifdef SYS_geteuid
    {"geteuid", SYS_geteuid},
#endif
#ifdef SYS_geteuid32
    {"geteuid32", SYS_geteuid32},
#endif
#ifdef SYS_getgid
    {"getgid", SYS_getgid},
#endif
#ifdef SYS_getgid32
    {"getgid32", SYS_getgid32},
#endif
#ifdef SYS_getgroups
    {"getgroups", SYS_getgroups},
#endif
#ifdef SYS_getitimer
    {"getitimer", SYS_getitimer},
#endif
#ifdef SYS_getpeername
    {"getpeername", SYS_getpeername},
#endif
#ifdef SYS_getpgid
    {"getpgid", SYS_getpgid},
#endif
#ifdef SYS_getpgrp
    {"getpgrp", SYS_getpgrp},
#endif
#ifdef SYS_getpid
    {"getpid", SYS_getpid},
#endif
#ifdef SYS_getppid
    {"getppid", SYS_getppid},
#endif
#ifdef SYS_getpriority
    {"getpriority", SYS_getpriority},
#endif
#ifdef SYS_getrandom
    {"getrandom", SYS_getrandom},
#endif
#ifdef SYS_getresgid
    {"getresgid", SYS_getresgid},
#endif
#ifdef SYS_getresuid
    {"getresuid", SYS_getresuid},
#endif
#ifdef SYS_getrlimit
    {"getrlimit", SYS_getrlimit},
#endif
#ifdef SYS_getrusage
    {"getrusage", SYS_getrusage},
#endif
#ifdef SYS_getsid
    {"getsid", SYS_getsid},
#endif
#ifdef SYS_getsockname
    {"getsockname", SYS_getsockname},
#endif
#ifdef SYS_getsockopt
    {"getsockopt", SYS_getsockopt},
#endif
#ifdef SYS_gettid
    {"gettid", SYS_gettid},
#endif
#ifdef SYS_gettimeofday
    {"gettimeofday", SYS_gettimeofday},
#endif
#ifdef SYS_getuid
    {"getuid", SYS_getuid},
#endif
#ifdef SYS_getuid32
    {"getuid32", SYS_getuid32},
#endif
#ifdef SYS_inotify_add_watch
    {"inotify_add_watch", SYS_inotify_add_watch},
#endif
#ifdef SYS_inotify_init
    {"inotify_init", SYS_inotify_init},
#endif
#ifdef SYS_inotify_init1
    {"inotify_init1", SYS_inotify_init1},
#endif
#ifdef SYS_inotify_rm_watch
    {"inotify_rm_watch", SYS_inotify_rm_watch},
#endif
#ifdef SYS_ioctl
    {"ioctl", SYS_ioctl},
#endif
#ifdef SYS_kill
    {"kill", SYS_kill},
#endif
#ifdef SYS_lchown
    {"lchown", SYS_lchown},
#endif
#ifdef SYS_lchown32
    {"lchown32", SYS_lchown32},
#endif
#ifdef SYS_link
    {"link", SYS_link},
#endif
#ifdef SYS_linkat
    {"linkat", SYS_linkat},
#endif
#ifdef SYS_listen
    {"listen", SYS_listen},
#endif
#ifdef SYS_lseek
    {"lseek", SYS_lseek},
#endif
#ifdef SYS_lstat
    {"lstat", SYS_lstat},
#endif
#ifdef SYS_lstat64
    {"lstat64", SYS_lstat64},
#endif
#ifdef SYS_madvise
    {"madvise", SYS_madvise},
#endif
#ifdef SYS_membarrier
    {"membarrier", SYS_membarrier},
#endif
#ifdef SYS_mkdir
    {"mkdir", SYS_mkdir},
#endif
#ifdef SYS_mkdirat
    {"mkdirat", SYS_mkdirat},
#endif
#ifdef SYS_mknod
    {"mknod", SYS_mknod},
#endif
#ifdef SYS_mknodat
    {"mknodat", SYS_mknodat},
#endif
#ifdef SYS_mlock
    {"mlock", SYS_mlock},
#endif
#ifdef SYS_mmap
    {"mmap", SYS_mmap},
#endif
#ifdef SYS_mmap2
    {"mmap2", SYS_mmap2},
#endif
#ifdef SYS_mprotect
    {"mprotect", SYS_mprotect},
#endif
#ifdef SYS_mremap
    {"mremap", SYS_mremap},
#endif
#ifdef SYS_msync
    {"msync", SYS_msync},
#endif
#ifdef SYS_munlock
    {"munlock", SYS_munlock},
#endif
#ifdef SYS_munmap
    {"munmap", SYS_munmap},
#endif
#ifdef SYS_nanosleep
    {"nanosleep", SYS_nanosleep},
#endif
#ifdef SYS_newfstatat
    {"newfstatat", SYS_newfstatat},
#endif
#ifdef SYS_open
    {"open", SYS_open},
#endif
#ifdef SYS_openat
    {"openat", SYS_openat},
#endif
#ifdef SYS_openat2
    {"openat2", SYS_openat2},
#endif
#ifdef SYS_pause
    {"pause", SYS_pause},
#endif
#ifdef SYS_pipe
    {"pipe", SYS_pipe},
#endif
#ifdef SYS_pipe2
    {"pipe2", SYS_pipe2},
#endif
#ifdef SYS_poll
    {"poll", SYS_poll},
#endif
#ifdef SYS_ppoll
    {"ppoll", SYS_ppoll},
#endif
#ifdef SYS_prctl
    {"prctl", SYS_prctl},
#endif
#ifdef SYS_pread64
    {"pread64", SYS_pread64},
#endif
#ifdef SYS_preadv
    {"preadv", SYS_preadv},
#endif
#ifdef SYS_preadv2
    {"preadv2", SYS_preadv2},
#endif
#ifdef SYS_prlimit64
    {"prlimit64", SYS_prlimit64},
#endif
#ifdef SYS_pselect6
    {"pselect6", SYS_pselect6},
#endif
#ifdef SYS_pwrite64
    {"pwrite64", SYS_pwrite64},
#endif
#ifdef SYS_pwritev
    {"pwritev", SYS_pwritev},
#endif
#ifdef SYS_pwritev2
    {"pwritev2", SYS_pwritev2},
#endif
#ifdef SYS_read
    {"read", SYS_read},
#endif
#ifdef SYS_readlink
    {"readlink", SYS_readlink},
#endif
#ifdef SYS_readlinkat
    {"readlinkat", SYS_readlinkat},
#endif
#ifdef SYS_readv
    {"readv", SYS_readv},
#endif
#ifdef SYS_recvfrom
    {"recvfrom", SYS_recvfrom},
#endif
#ifdef SYS_recvmmsg
    {"recvmmsg", SYS_recvmmsg},
#endif
#ifdef SYS_recvmsg
    {"recvmsg", SYS_recvmsg},
#endif
#ifdef SYS_rename
    {"rename", SYS_rename},
#endif
#ifdef SYS_renameat
    {"renameat", SYS_renameat},
#endif
#ifdef SYS_renameat2
    {"renameat2", SYS_renameat2},
#endif
#ifdef SYS_restart_syscall
    {"restart_syscall", SYS_restart_syscall},
#endif
#ifdef SYS_riscv_flush_icache
    {"riscv_flush_icache", SYS_riscv_flush_icache},
#endif
#ifdef SYS_rmdir
    {"rmdir", SYS_rmdir},
#endif
#ifdef SYS_rseq
    {"rseq", SYS_rseq},
#endif
#ifdef SYS_rt_sigaction
    {"rt_sigaction", SYS_rt_sigaction},
#endif
#ifdef SYS_rt_sigprocmask
    {"rt_sigprocmask", SYS_rt_sigprocmask},
#endif
#ifdef SYS_rt_sigreturn
    {"rt_sigreturn", SYS_rt_sigreturn},
#endif
#ifdef SYS_rt_sigsuspend
    {"rt_sigsuspend", SYS_rt_sigsuspend},
#endif
#ifdef SYS_rt_sigtimedwait
    {"rt_sigtimedwait", SYS_rt_sigtimedwait},
#endif
#ifdef SYS_sched_getaffinity
    {"sched_getaffinity", SYS_sched_getaffinity},
#endif
#ifdef SYS_sched_getparam
    {"sched_getparam", SYS_sched_getparam},
#endif
#ifdef SYS_sched_getscheduler
    {"sched_getscheduler", SYS_sched_getscheduler},
#endif
#ifdef SYS_sched_setaffinity
    {"sched_setaffinity", SYS_sched_setaffinity},
#endif
#ifdef SYS_sched_setparam
    {"sched_setparam", SYS_sched_setparam},
#endif
#ifdef SYS_sched_setscheduler
    {"sched_setscheduler", SYS_sched_setscheduler},
#endif
#ifdef SYS_sched_yield
    {"sched_yield", SYS_sched_yield},
#endif
#ifdef SYS_select
    {"select", SYS_select},
#endif
#ifdef SYS_sendfile
    {"sendfile", SYS_sendfile},
#endif
#ifdef SYS_sendfile64
    {"sendfile64", SYS_sendfile64},
#endif
#ifdef SYS_sendmmsg
    {"sendmmsg", SYS_sendmmsg},
#endif
#ifdef SYS_sendmsg
    {"sendmsg", SYS_sendmsg},
#endif
#ifdef SYS_sendto
    {"sendto", SYS_sendto},
#endif
#ifdef SYS_set_robust_list
    {"set_robust_list", SYS_set_robust_list},
#endif
#ifdef SYS_set_thread_area
    {"set_thread_area", SYS_set_thread_area},
#endif
#ifdef SYS_set_tid_address
    {"set_tid_address", SYS_set_tid_address},
#endif
#ifdef SYS_setfsgid
    {"setfsgid", SYS_setfsgid},
#endif
#ifdef SYS_setfsgid32
    {"setfsgid32", SYS_setfsgid32},
#endif
#ifdef SYS_setfsuid
    {"setfsuid", SYS_setfsuid},
#endif
#ifdef SYS_setfsuid32
    {"setfsuid32", SYS_setfsuid32},
#endif
#ifdef SYS_setgid
    {"setgid", SYS_setgid},
#endif
#ifdef SYS_setgid32
    {"setgid32", SYS_setgid32},
#endif
#ifdef SYS_setgroups
    {"setgroups", SYS_setgroups},
#endif
#ifdef SYS_setgroups32
    {"setgroups32", SYS_setgroups32},
#endif
#ifdef SYS_setitimer
    {"setitimer", SYS_setitimer},
#endif
#ifdef SYS_setpgid
    {"setpgid", SYS_setpgid},
#endif
#ifdef SYS_setpriority
    {"setpriority", SYS_setpriority},
#endif
#ifdef SYS_setregid
    {"setregid", SYS_setregid},
#endif
#ifdef SYS_setregid32
    {"setregid32", SYS_setregid32},
#endif
#ifdef SYS_setresgid
    {"setresgid", SYS_setresgid},
#endif
#ifdef SYS_setresgid32
    {"setresgid32", SYS_setresgid32},
#endif
#ifdef SYS_setresuid
    {"setresuid", SYS_setresuid},
#endif
#ifdef SYS_setresuid32
    {"setresuid32", SYS_setresuid32},
#endif
#ifdef SYS_setreuid
    {"setreuid", SYS_setreuid},
#endif
#ifdef SYS_setreuid32
    {"setreuid32", SYS_setreuid32},
#endif
#ifdef SYS_setrlimit
    {"setrlimit", SYS_setrlimit},
#endif
#ifdef SYS_setsid
    {"setsid", SYS_setsid},
#endif
#ifdef SYS_setsockopt
    {"setsockopt", SYS_setsockopt},
#endif
#ifdef SYS_setuid
    {"setuid", SYS_setuid},
#endif
#ifdef SYS_setuid32
    {"setuid32", SYS_setuid32},
#endif
#ifdef SYS_shutdown
    {"shutdown", SYS_shutdown},
#endif
#ifdef SYS_sigaltstack
    {"sigaltstack", SYS_sigaltstack},
#endif
#ifdef SYS_signalfd
    {"signalfd", SYS_signalfd},
#endif
#ifdef SYS_signalfd4
    {"signalfd4", SYS_signalfd4},
#endif
#ifdef SYS_sigreturn
    {"sigreturn", SYS_sigreturn},
#endif
#ifdef SYS_socket
    {"socket", SYS_socket},
#endif
#ifdef SYS_socketpair
    {"socketpair", SYS_socketpair},
#endif
#ifdef SYS_splice
    {"splice", SYS_splice},
#endif
#ifdef SYS_stat
    {"stat", SYS_stat},
#endif
#ifdef SYS_stat64
    {"stat64", SYS_stat64},
#endif
#ifdef SYS_statfs
    {"statfs", SYS_statfs},
#endif
#ifdef SYS_statfs64
    {"statfs64", SYS_statfs64},
#endif
#ifdef SYS_statx
    {"statx", SYS_statx},
#endif
#ifdef SYS_symlink
    {"symlink", SYS_symlink},
#endif
#ifdef SYS_symlinkat
    {"symlinkat", SYS_symlinkat},
#endif
#ifdef SYS_sysinfo
    {"sysinfo", SYS_sysinfo},
#endif
#ifdef SYS_tee
    {"tee", SYS_tee},
#endif
#ifdef SYS_tgkill
    {"tgkill", SYS_tgkill},
#endif
#ifdef SYS_time
    {"time", SYS_time},
#endif
#ifdef SYS_timerfd_create
    {"timerfd_create", SYS_timerfd_create},
#endif
#ifdef SYS_timerfd_gettime
    {"timerfd_gettime", SYS_timerfd_gettime},
#endif
#ifdef SYS_timerfd_settime
    {"timerfd_settime", SYS_timerfd_settime},
#endif
#ifdef SYS_times
    {"times", SYS_times},
#endif
#ifdef SYS_ugetrlimit
    {"ugetrlimit", SYS_ugetrlimit},
#endif
#ifdef SYS_umask
    {"umask", SYS_umask},
#endif
#ifdef SYS_uname
    {"uname", SYS_uname},
#endif
#ifdef SYS_unlink
    {"unlink", SYS_unlink},
#endif
#ifdef SYS_unlinkat
    {"unlinkat", SYS_unlinkat},
#endif
#ifdef SYS_utime
    {"utime", SYS_utime},
#endif
#ifdef SYS_utimensat
    {"utimensat", SYS_utimensat},
#endif
#ifdef SYS_utimes
    {"utimes", SYS_utimes},
#endif
#ifdef SYS_vfork
    {"vfork", SYS_vfork},
#endif
#ifdef SYS_wait4
    {"wait4", SYS_wait4},
#endif
#ifdef SYS_waitid
    {"waitid", SYS_waitid},
#endif
#ifdef SYS_write
    {"write", SYS_write},
#endif
#ifdef SYS_writev
    {"writev", SYS_writev},
#endif
    {0, -1}
};
static const char* const sb_p_stdio[] = {"read", "write", "readv", "writev", "pread64", "pwrite64", "preadv", "pwritev", "preadv2", "pwritev2", "close", "close_range", "fstat", "newfstatat", "fstatat64", "statx", "lseek", "_llseek", "mmap", "mmap2", "munmap", "mprotect", "brk", "mremap", "madvise", "msync", "mlock", "munlock", "rt_sigaction", "rt_sigprocmask", "rt_sigreturn", "sigreturn", "sigaltstack", "rt_sigsuspend", "rt_sigtimedwait", "exit", "exit_group", "futex", "futex_waitv", "clock_gettime", "clock_gettime64", "clock_getres", "clock_nanosleep", "nanosleep", "gettimeofday", "time", "times", "getrusage", "getpid", "gettid", "getppid", "getuid", "geteuid", "getgid", "getegid", "getuid32", "geteuid32", "getgid32", "getegid32", "getresuid", "getresgid", "getgroups", "getrandom", "sched_yield", "sched_getaffinity", "dup", "dup2", "dup3", "fcntl", "fcntl64", "ioctl", "pipe", "pipe2", "poll", "ppoll", "select", "_newselect", "pselect6", "epoll_create", "epoll_create1", "epoll_ctl", "epoll_wait", "epoll_pwait", "epoll_pwait2", "eventfd", "eventfd2", "timerfd_create", "timerfd_settime", "timerfd_gettime", "signalfd", "signalfd4", "set_robust_list", "get_robust_list", "set_tid_address", "rseq", "prlimit64", "getrlimit", "ugetrlimit", "uname", "sysinfo", "umask", "fstatfs", "fstatfs64", "ftruncate", "ftruncate64", "fsync", "fdatasync", "fadvise64", "arm_fadvise64_64", "sendfile", "sendfile64", "splice", "tee", "shutdown", "sendto", "recvfrom", "sendmsg", "recvmsg", "sendmmsg", "recvmmsg", "socketpair", "getsockname", "getpeername", "getsockopt", "setsockopt", "tgkill", "restart_syscall", "membarrier", "arch_prctl", "set_thread_area", "get_thread_area", "prctl", "capget", "sched_getparam", "sched_getscheduler", "getcpu", "riscv_flush_icache", "cacheflush", "alarm", "getitimer", "setitimer", "pause", 0};
static const char* const sb_p_rpath[] = {"stat", "lstat", "stat64", "lstat64", "access", "faccessat", "faccessat2", "readlink", "readlinkat", "getdents", "getdents64", "getcwd", "chdir", "fchdir", "statfs", "statfs64", "inotify_init", "inotify_init1", "inotify_add_watch", "inotify_rm_watch", 0};
static const char* const sb_p_cpath[] = {"mkdir", "mkdirat", "rmdir", "unlink", "unlinkat", "rename", "renameat", "renameat2", "link", "linkat", "symlink", "symlinkat", "creat", "mknod", "mknodat", 0};
static const char* const sb_p_fattr[] = {"chmod", "fchmod", "fchmodat", "fchmodat2", "chown", "fchown", "lchown", "fchownat", "chown32", "fchown32", "lchown32", "utime", "utimes", "utimensat", "futimesat", 0};
static const char* const sb_p_flock[] = {"flock", 0};
static const char* const sb_p_net[] = {"connect", "bind", "listen", "accept", "accept4", 0};
static const char* const sb_p_proc[] = {"fork", "vfork", "clone", "clone3", "wait4", "waitid", "kill", "setpgid", "getpgid", "getpgrp", "setsid", "getsid", "setpriority", "getpriority", "setrlimit", "sched_setaffinity", "sched_setscheduler", "sched_setparam", 0};
static const char* const sb_p_thread[] = {"clone", "clone3", 0};
static const char* const sb_p_exec[] = {"execve", "execveat", 0};
static const char* const sb_p_id[] = {"setuid", "setgid", "setreuid", "setregid", "setresuid", "setresgid", "setgroups", "setfsuid", "setfsgid", "setuid32", "setgid32", "setreuid32", "setregid32", "setresuid32", "setresgid32", "setgroups32", "setfsuid32", "setfsgid32", 0};

#if defined(__x86_64__)
#  define SB_ARCH AUDIT_ARCH_X86_64
#elif defined(__aarch64__)
#  define SB_ARCH AUDIT_ARCH_AARCH64
#elif defined(__i386__)
#  define SB_ARCH AUDIT_ARCH_I386
#elif defined(__arm__)
#  define SB_ARCH AUDIT_ARCH_ARM
#elif defined(__riscv) && __riscv_xlen == 64
#  define SB_ARCH AUDIT_ARCH_RISCV64
#endif
#ifndef SECCOMP_RET_KILL_PROCESS
#  define SECCOMP_RET_KILL_PROCESS 0x80000000U
#endif
#ifndef SECCOMP_SET_MODE_FILTER
#  define SECCOMP_SET_MODE_FILTER 1
#endif
#ifndef SECCOMP_FILTER_FLAG_TSYNC
#  define SECCOMP_FILTER_FLAG_TSYNC 1
#endif

static int sb_nr(const char* name) {
    int i;
    for (i = 0; sb_systab[i].name; i++) if (strcmp(sb_systab[i].name, name) == 0) return sb_systab[i].nr;
    return -1;
}

enum { SB_ALWAYS = 0, SB_MASKZERO = 1, SB_VALUES = 2 };
typedef struct { int nr; int kind; int arg; uint32_t mask; uint32_t vals[4]; int nvals; } sb_rule;
typedef struct { sb_rule r[512]; int n; } sb_rules;

static void sb_add(sb_rules* rs, int nr, int kind, int arg, uint32_t mask, const uint32_t* vals, int nvals) {
    int i;
    if (nr < 0) return;
    for (i = 0; i < rs->n; i++) if (rs->r[i].nr == nr) {
        if (kind == SB_ALWAYS) rs->r[i].kind = SB_ALWAYS;
        return;
    }
    if (rs->n >= 512) return;
    rs->r[rs->n].nr = nr; rs->r[rs->n].kind = kind; rs->r[rs->n].arg = arg; rs->r[rs->n].mask = mask;
    rs->r[rs->n].nvals = nvals;
    for (i = 0; i < nvals && i < 4; i++) rs->r[rs->n].vals[i] = vals[i];
    rs->n++;
}
static void sb_add_list(sb_rules* rs, const char* const* names) {
    for (; *names; names++) sb_add(rs, sb_nr(*names), SB_ALWAYS, 0, 0, NULL, 0);
}

#define SB_ARG_LO(i) ((uint32_t)(offsetof(struct seccomp_data, args) + 8 * (i)))

/* Build and install a filter. listed_action applies to rules, other_action to the rest. */
static int sb_install(const sb_rules* rs, uint32_t listed_action, uint32_t other_action) {
#ifndef SB_ARCH
    (void)rs; (void)listed_action; (void)other_action;
    sb_set_err("seccomp: unsupported architecture"); return -1;
#else
    struct sock_filter* f; struct sock_fprog prog; int k = 0, i, j; long rc;
    f = (struct sock_filter*)calloc((size_t)rs->n * 8 + 16, sizeof *f);
    if (!f) { sb_set_err("seccomp: out of memory"); return -1; }
    f[k++] = (struct sock_filter)BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(struct seccomp_data, arch));
    f[k++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SB_ARCH, 1, 0);
    f[k++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS);
    f[k++] = (struct sock_filter)BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(struct seccomp_data, nr));
#if defined(__x86_64__)
    /* x32 ABI syscalls share the arch value; refuse them outright */
    f[k++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 0x40000000U, 0, 1);
    f[k++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS);
#endif
    for (i = 0; i < rs->n; i++) {
        const sb_rule* r = &rs->r[i];
        if (r->kind == SB_ALWAYS || listed_action != SECCOMP_RET_ALLOW) {
            f[k++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, (uint32_t)r->nr, 0, 1);
            f[k++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, listed_action);
        } else if (r->kind == SB_MASKZERO) {
            f[k++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, (uint32_t)r->nr, 0, 4);
            f[k++] = (struct sock_filter)BPF_STMT(BPF_LD | BPF_W | BPF_ABS, SB_ARG_LO(r->arg));
            f[k++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, r->mask, 1, 0);
            f[k++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW);
            f[k++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, other_action);
        } else {
            int n = r->nvals;
            f[k++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, (uint32_t)r->nr, 0, (unsigned char)(n + 3));
            f[k++] = (struct sock_filter)BPF_STMT(BPF_LD | BPF_W | BPF_ABS, SB_ARG_LO(r->arg));
            for (j = 0; j < n; j++)
                f[k++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, r->vals[j], (unsigned char)(n - j), 0);
            f[k++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, other_action);
            f[k++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW);
        }
    }
    f[k++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, other_action);
    prog.len = (unsigned short)k; prog.filter = f;
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) != 0) { sb_set_errno("seccomp: PR_SET_NO_NEW_PRIVS"); free(f); return -1; }
#if defined(SYS_seccomp)
    rc = syscall(SYS_seccomp, SECCOMP_SET_MODE_FILTER, SECCOMP_FILTER_FLAG_TSYNC, &prog);
    if (rc != 0 && errno == EINVAL) rc = syscall(SYS_seccomp, SECCOMP_SET_MODE_FILTER, 0, &prog);
    if (rc != 0 && errno == ENOSYS) rc = prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog, 0, 0);
#else
    rc = prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog, 0, 0);
#endif
    free(f);
    if (rc != 0) { sb_set_errno("seccomp: cannot install filter"); return -1; }
    return 0;
#endif
}

/* Promise bits (must match std/sys/sandbox.tr). */
#define SBP_STDIO  (1 << 0)
#define SBP_RPATH  (1 << 1)
#define SBP_WPATH  (1 << 2)
#define SBP_CPATH  (1 << 3)
#define SBP_FATTR  (1 << 4)
#define SBP_FLOCK  (1 << 5)
#define SBP_INET   (1 << 6)
#define SBP_UNIX   (1 << 7)
#define SBP_DNS    (1 << 8)
#define SBP_PROC   (1 << 9)
#define SBP_THREAD (1 << 10)
#define SBP_EXEC   (1 << 11)
#define SBP_ID     (1 << 12)

int64_t trc_sb_seccomp_promises(int64_t bits, int64_t deny_errno) {
    static sb_rules rs;
    uint32_t deny = deny_errno > 0 ? (SECCOMP_RET_ERRNO | ((uint32_t)deny_errno & SECCOMP_RET_DATA)) : SECCOMP_RET_KILL_PROCESS;
    uint32_t doms[4]; int nd = 0;
    rs.n = 0;
    if (bits & SBP_STDIO) sb_add_list(&rs, sb_p_stdio);
    if (bits & SBP_RPATH) sb_add_list(&rs, sb_p_rpath);
    if (bits & SBP_CPATH) sb_add_list(&rs, sb_p_cpath);
    if (bits & SBP_FATTR) sb_add_list(&rs, sb_p_fattr);
    if (bits & SBP_FLOCK) sb_add_list(&rs, sb_p_flock);
    if (bits & (SBP_INET | SBP_UNIX | SBP_DNS)) sb_add_list(&rs, sb_p_net);
    if (bits & SBP_PROC) sb_add_list(&rs, sb_p_proc);
    if (bits & SBP_THREAD) sb_add_list(&rs, sb_p_thread);
    if (bits & SBP_EXEC) sb_add_list(&rs, sb_p_exec);
    if (bits & SBP_ID) sb_add_list(&rs, sb_p_id);
    /* open/openat by flags: rpath = read-only, wpath = no O_CREAT, cpath = anything */
    {
        int kind = -1; uint32_t mask = 0;
        if (bits & SBP_CPATH) kind = SB_ALWAYS;
        else if (bits & SBP_WPATH) { kind = SB_MASKZERO; mask = O_CREAT; }
        else if (bits & SBP_RPATH) { kind = SB_MASKZERO; mask = O_ACCMODE | O_CREAT | O_TRUNC; }
        if (kind >= 0) {
            sb_add(&rs, sb_nr("openat"), kind, 2, mask, NULL, 0);
            sb_add(&rs, sb_nr("open"), kind, 1, mask, NULL, 0);
            if (kind == SB_ALWAYS) sb_add(&rs, sb_nr("openat2"), SB_ALWAYS, 0, 0, NULL, 0);
        }
        if (bits & (SBP_RPATH | SBP_WPATH | SBP_CPATH)) {
            /* the path-based stat/access family comes with any path promise */
            sb_add_list(&rs, sb_p_rpath);
        }
    }
    if (bits & (SBP_INET | SBP_DNS)) { doms[nd++] = 2; doms[nd++] = 10; }   /* AF_INET, AF_INET6 */
    if (bits & SBP_UNIX) doms[nd++] = 1;                                    /* AF_UNIX */
    if (bits & SBP_DNS) doms[nd++] = 16;                                    /* AF_NETLINK (getaddrinfo) */
    if (nd > 0) sb_add(&rs, sb_nr("socket"), SB_VALUES, 0, 0, doms, nd);
    return sb_install(&rs, SECCOMP_RET_ALLOW, deny) == 0 ? 0 : -1;
}

/* names: '\n'-separated syscall names. allowlist: listed are allowed and the
 * rest denied; otherwise listed are denied and the rest allowed. deny_errno 0
 * kills the process. Returns the number of unknown names, or -1. */
int64_t trc_sb_seccomp_list(const char* names, int64_t allowlist, int64_t deny_errno) {
    static sb_rules rs;
    uint32_t deny = deny_errno > 0 ? (SECCOMP_RET_ERRNO | ((uint32_t)deny_errno & SECCOMP_RET_DATA)) : SECCOMP_RET_KILL_PROCESS;
    char nm[64]; const char* p = names; int unknown = 0;
    rs.n = 0;
    while (*p) {
        size_t n = 0;
        while (p[n] && p[n] != '\n') n++;
        if (n > 0 && n < sizeof nm) {
            int nr; memcpy(nm, p, n); nm[n] = 0;
            nr = sb_nr(nm);
#if defined(__linux__)
            if (nr < 0) {
                /* names outside the built-in table */
                if (strcmp(nm, "getpid") == 0) nr = SYS_getpid;
            }
#endif
            if (nr < 0) unknown++; else sb_add(&rs, nr, SB_ALWAYS, 0, 0, NULL, 0);
        }
        p += n;
        if (*p == '\n') p++;
    }
    if (allowlist) { if (sb_install(&rs, SECCOMP_RET_ALLOW, deny) != 0) return -1; }
    else { if (sb_install(&rs, deny, SECCOMP_RET_ALLOW) != 0) return -1; }
    return unknown;
}

/* Syscall number for a name on this architecture, or -1. */
int64_t trc_sb_syscall_nr(const char* name) { return sb_nr(name); }

/* ── Landlock ─────────────────────────────────────────────────────────────── */
#ifndef SYS_landlock_create_ruleset
#  define SYS_landlock_create_ruleset 444
#  define SYS_landlock_add_rule 445
#  define SYS_landlock_restrict_self 446
#endif
#define LL_CREATE_RULESET_VERSION (1U << 0)
#define LL_RULE_PATH_BENEATH 1
#define LL_EXECUTE     (1ULL << 0)
#define LL_WRITE_FILE  (1ULL << 1)
#define LL_READ_FILE   (1ULL << 2)
#define LL_READ_DIR    (1ULL << 3)
#define LL_REMOVE_DIR  (1ULL << 4)
#define LL_REMOVE_FILE (1ULL << 5)
#define LL_MAKE_CHAR   (1ULL << 6)
#define LL_MAKE_DIR    (1ULL << 7)
#define LL_MAKE_REG    (1ULL << 8)
#define LL_MAKE_SOCK   (1ULL << 9)
#define LL_MAKE_FIFO   (1ULL << 10)
#define LL_MAKE_BLOCK  (1ULL << 11)
#define LL_MAKE_SYM    (1ULL << 12)
#define LL_REFER       (1ULL << 13)
#define LL_TRUNCATE    (1ULL << 14)

struct sb_ll_ruleset_attr { uint64_t handled_access_fs; };
struct sb_ll_path_beneath { uint64_t allowed_access; int32_t parent_fd; } __attribute__((packed));

int64_t trc_sb_landlock_abi(void) {
    long v = syscall(SYS_landlock_create_ruleset, NULL, 0, LL_CREATE_RULESET_VERSION);
    return v < 0 ? 0 : v;
}

static uint64_t sb_ll_handled(int64_t abi) {
    uint64_t h = (LL_MAKE_SYM << 1) - 1;          /* ABI 1: bits 0..12 */
    if (abi >= 2) h |= LL_REFER;
    if (abi >= 3) h |= LL_TRUNCATE;
    return h;
}

/* New ruleset handling every filesystem right this kernel knows; returns its fd or -1. */
int64_t trc_sb_landlock_create(void) {
    struct sb_ll_ruleset_attr a; long fd; int64_t abi = trc_sb_landlock_abi();
    if (abi <= 0) { sb_set_err("landlock: not supported by this kernel"); return -1; }
    a.handled_access_fs = sb_ll_handled(abi);
    fd = syscall(SYS_landlock_create_ruleset, &a, sizeof a, 0);
    if (fd < 0) { sb_set_errno("landlock: create_ruleset"); return -1; }
    return fd;
}

/* perms: any of r (read), w (write), x (execute), c (create/remove). */
int64_t trc_sb_landlock_add(int64_t rfd, const char* path, const char* perms) {
    struct sb_ll_path_beneath pb; struct stat st; int fd; uint64_t acc = 0; int64_t abi = trc_sb_landlock_abi();
    const char* q;
    for (q = perms; *q; q++) {
        if (*q == 'r') acc |= LL_READ_FILE | LL_READ_DIR;
        else if (*q == 'w') acc |= LL_WRITE_FILE | (abi >= 3 ? LL_TRUNCATE : 0);
        else if (*q == 'x') acc |= LL_EXECUTE;
        else if (*q == 'c') acc |= LL_REMOVE_DIR | LL_REMOVE_FILE | LL_MAKE_CHAR | LL_MAKE_DIR | LL_MAKE_REG
                                 | LL_MAKE_SOCK | LL_MAKE_FIFO | LL_MAKE_BLOCK | LL_MAKE_SYM | (abi >= 2 ? LL_REFER : 0);
        else { sb_set_err("landlock: permissions are r, w, x and c"); return -1; }
    }
#ifdef O_PATH
    fd = open(path, O_PATH | O_CLOEXEC);
#else
    fd = open(path, O_RDONLY | O_CLOEXEC);
#endif
    if (fd < 0) { sb_set_errno("landlock: cannot open path"); return -1; }
    if (fstat(fd, &st) == 0 && !S_ISDIR(st.st_mode))
        acc &= LL_EXECUTE | LL_WRITE_FILE | LL_READ_FILE | LL_TRUNCATE;   /* file rules take file rights only */
    pb.allowed_access = acc; pb.parent_fd = fd;
    if (acc != 0 && syscall(SYS_landlock_add_rule, (int)rfd, LL_RULE_PATH_BENEATH, &pb, 0) != 0) {
        sb_set_errno("landlock: add_rule"); close(fd); return -1;
    }
    close(fd);
    return 0;
}

int64_t trc_sb_landlock_restrict(int64_t rfd) {
    long rc;
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) != 0) { sb_set_errno("landlock: PR_SET_NO_NEW_PRIVS"); return -1; }
    rc = syscall(SYS_landlock_restrict_self, (int)rfd, 0);
    close((int)rfd);
    if (rc != 0) { sb_set_errno("landlock: restrict_self"); return -1; }
    return 0;
}

int64_t trc_sb_no_new_privs(void) { return prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == 0 ? 0 : -1; }

#else  /* !__linux__ */

int64_t trc_sb_seccomp_promises(int64_t bits, int64_t deny_errno) { (void)bits; (void)deny_errno; sb_set_err("seccomp: Linux only"); return -1; }
int64_t trc_sb_seccomp_list(const char* names, int64_t allowlist, int64_t deny_errno) { (void)names; (void)allowlist; (void)deny_errno; sb_set_err("seccomp: Linux only"); return -1; }
int64_t trc_sb_syscall_nr(const char* name) { (void)name; return -1; }
int64_t trc_sb_landlock_abi(void) { return 0; }
int64_t trc_sb_landlock_create(void) { sb_set_err("landlock: Linux only"); return -1; }
int64_t trc_sb_landlock_add(int64_t rfd, const char* path, const char* perms) { (void)rfd; (void)path; (void)perms; return -1; }
int64_t trc_sb_landlock_restrict(int64_t rfd) { (void)rfd; return -1; }
int64_t trc_sb_no_new_privs(void) { return -1; }

#endif

/* ── OpenBSD pledge / unveil ─────────────────────────────────────────────── */
int64_t trc_sb_pledge(const char* promises) {
#if defined(__OpenBSD__)
    if (pledge(promises, NULL) != 0) { sb_set_errno("pledge"); return -1; }
    return 0;
#else
    (void)promises; sb_set_err("pledge: OpenBSD only"); return -1;
#endif
}
int64_t trc_sb_unveil(const char* path, const char* perms) {
#if defined(__OpenBSD__)
    if (unveil(path[0] ? path : NULL, perms[0] || path[0] ? perms : NULL) != 0) { sb_set_errno("unveil"); return -1; }
    return 0;
#else
    (void)path; (void)perms; sb_set_err("unveil: OpenBSD only"); return -1;
#endif
}
