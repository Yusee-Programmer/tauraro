/*
 * std/sys/subprocess_native.c -- process spawning for std.sys.subprocess.
 *
 * POSIX: fork + execve. The program is looked up on PATH (the child's PATH
 * when the environment is changed) before forking, so "not found" is
 * reported cleanly; a CLOEXEC pipe carries chdir/exec failures from the
 * child back to the parent. Every descriptor except the child's stdio is
 * marked close-on-exec.
 * Windows: CreateProcessW with an MSVCRT-quoted command line converted from
 * UTF-8, a UTF-16 environment block, and PROC_THREAD_ATTRIBUTE_HANDLE_LIST
 * so only the child's three stdio handles are inherited.
 *
 * Linked through `# tauraro-cpp-shim: subprocess_native.c` in
 * std/sys/subprocess.tr. All strings cross the boundary as (pointer, length).
 */
#if !defined(_WIN32) && !defined(__APPLE__) && !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
#endif
#if defined(_WIN32) && (!defined(_WIN32_WINNT) || _WIN32_WINNT < 0x0600)
#  undef _WIN32_WINNT
#  define _WIN32_WINNT 0x0600
#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#if defined(_WIN32)
#  include <windows.h>
#  include <io.h>
#  include <fcntl.h>
#else
#  include <unistd.h>
#  include <fcntl.h>
#  include <signal.h>
#  include <poll.h>
#  include <time.h>
#  include <pthread.h>
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <sys/stat.h>
#  if defined(__APPLE__)
#    include <crt_externs.h>
#  endif
#  if defined(__linux__)
#    include <sys/syscall.h>
#  endif
#endif

/* ── Errors ──────────────────────────────────────────────────────────────── */

#if defined(_MSC_VER)
static __declspec(thread) char sp_err[600];
#else
static __thread char sp_err[600];
#endif

static void sp_set_err(const char* a, const char* b, const char* c) {
    snprintf(sp_err, sizeof sp_err, "%s%s%s", a ? a : "", b ? b : "", c ? c : "");
}

/* Copy the last error text into out (cap bytes); returns its length. */
int64_t trc_sp_last_error(char* out, int64_t cap) {
    size_t n = strlen(sp_err);
    if (cap <= 0) return 0;
    if (n > (size_t)cap - 1) n = (size_t)cap - 1;
    memcpy(out, sp_err, n);
    out[n] = 0;
    return (int64_t)n;
}

static char* sp_dup(const char* p, int64_t n) {
    char* s = (char*)malloc((size_t)n + 1);
    if (!s) return NULL;
    if (n) memcpy(s, p, (size_t)n);
    s[n] = 0;
    return s;
}

/* Growable list of malloc'd C strings. */
typedef struct { char** v; size_t n, cap; } sp_strs;

static int sp_strs_push(sp_strs* l, char* s) {
    if (!s) return -1;
    if (l->n + 1 >= l->cap) {
        size_t nc = l->cap ? l->cap * 2 : 8;
        char** nv = (char**)realloc(l->v, nc * sizeof(char*));
        if (!nv) { free(s); return -1; }
        l->v = nv; l->cap = nc;
    }
    l->v[l->n++] = s;
    l->v[l->n] = NULL;
    return 0;
}

static void sp_strs_free(sp_strs* l) {
    size_t i;
    for (i = 0; i < l->n; i++) free(l->v[i]);
    free(l->v);
    l->v = NULL; l->n = l->cap = 0;
}

/* ── Command builder ─────────────────────────────────────────────────────── */

enum { SP_INHERIT = 0, SP_PIPE = 1, SP_NULL = 2, SP_FILE = 3, SP_APPEND = 4, SP_MERGE = 5 };

typedef struct {
    sp_strs argv;
    sp_strs env_set;      /* "KEY=VALUE" */
    sp_strs env_remove;   /* "KEY" */
    int env_clear;
    char* cwd;
    char* raw_cmdline;    /* Windows: used verbatim instead of quoting argv */
    int mode[3];
    char* path[3];
} sp_cmd;

int64_t trc_sp_cmd_new(void) {
    sp_cmd* c = (sp_cmd*)calloc(1, sizeof(sp_cmd));
    return (int64_t)(intptr_t)c;
}

void trc_sp_cmd_free(int64_t h) {
    sp_cmd* c = (sp_cmd*)(intptr_t)h;
    int i;
    if (!c) return;
    sp_strs_free(&c->argv); sp_strs_free(&c->env_set); sp_strs_free(&c->env_remove);
    free(c->cwd); free(c->raw_cmdline);
    for (i = 0; i < 3; i++) free(c->path[i]);
    free(c);
}

int64_t trc_sp_cmd_arg(int64_t h, const char* p, int64_t n) {
    sp_cmd* c = (sp_cmd*)(intptr_t)h;
    if (memchr(p, 0, (size_t)n)) { sp_set_err("subprocess: argument contains a NUL byte", 0, 0); return -1; }
    if (sp_strs_push(&c->argv, sp_dup(p, n)) < 0) { sp_set_err("subprocess: out of memory", 0, 0); return -1; }
    return 0;
}

int64_t trc_sp_cmd_env(int64_t h, const char* k, int64_t kn, const char* v, int64_t vn) {
    sp_cmd* c = (sp_cmd*)(intptr_t)h;
    char* s;
    if (kn == 0 || memchr(k, '=', (size_t)kn) || memchr(k, 0, (size_t)kn) || memchr(v, 0, (size_t)vn)) {
        sp_set_err("subprocess: invalid environment variable name or value", 0, 0); return -1;
    }
    s = (char*)malloc((size_t)(kn + vn + 2));
    if (!s) { sp_set_err("subprocess: out of memory", 0, 0); return -1; }
    memcpy(s, k, (size_t)kn); s[kn] = '='; memcpy(s + kn + 1, v, (size_t)vn); s[kn + vn + 1] = 0;
    sp_strs_push(&c->env_set, s);
    return 0;
}

int64_t trc_sp_cmd_env_remove(int64_t h, const char* k, int64_t kn) {
    sp_cmd* c = (sp_cmd*)(intptr_t)h;
    sp_strs_push(&c->env_remove, sp_dup(k, kn));
    return 0;
}

void trc_sp_cmd_env_clear(int64_t h) { ((sp_cmd*)(intptr_t)h)->env_clear = 1; }

void trc_sp_cmd_cwd(int64_t h, const char* p, int64_t n) {
    sp_cmd* c = (sp_cmd*)(intptr_t)h;
    free(c->cwd);
    c->cwd = n > 0 ? sp_dup(p, n) : NULL;
}

void trc_sp_cmd_raw_cmdline(int64_t h, const char* p, int64_t n) {
    sp_cmd* c = (sp_cmd*)(intptr_t)h;
    free(c->raw_cmdline);
    c->raw_cmdline = sp_dup(p, n);
}

/* which: 0 stdin, 1 stdout, 2 stderr. */
void trc_sp_cmd_stdio(int64_t h, int64_t which, int64_t mode, const char* p, int64_t n) {
    sp_cmd* c = (sp_cmd*)(intptr_t)h;
    if (which < 0 || which > 2) return;
    c->mode[which] = (int)mode;
    free(c->path[which]);
    c->path[which] = (mode == SP_FILE || mode == SP_APPEND) ? sp_dup(p, n) : NULL;
}

#if !defined(_WIN32)
/* Does the env key of "KEY=VALUE" (or "KEY") equal key? */
static int sp_key_eq(const char* entry, const char* key, int icase) {
    size_t kl = strlen(key), i;
    for (i = 0; i < kl; i++) {
        char a = entry[i], b = key[i];
        if (!a) return 0;
        if (icase) {
            if (a >= 'a' && a <= 'z') a = (char)(a - 32);
            if (b >= 'a' && b <= 'z') b = (char)(b - 32);
        }
        if (a != b) return 0;
    }
    return entry[kl] == '=' || entry[kl] == 0;
}

/* Is variable `key` overridden (set or removed) by the command? */
static int sp_env_overridden(const sp_cmd* c, const char* entry, int icase) {
    size_t i;
    char key[512];
    const char* eq = strchr(entry + (entry[0] == '=' ? 1 : 0), '=');
    size_t kl = eq ? (size_t)(eq - entry) : strlen(entry);
    if (kl >= sizeof key) return 0;
    memcpy(key, entry, kl); key[kl] = 0;
    for (i = 0; i < c->env_remove.n; i++) if (sp_key_eq(c->env_remove.v[i], key, icase) && strlen(c->env_remove.v[i]) == kl) return 1;
    for (i = 0; i < c->env_set.n; i++) if (sp_key_eq(c->env_set.v[i], key, icase)) return 1;
    return 0;
}

/* Value of variable key in the env the child will see (NULL if unset). */
static const char* sp_child_getenv(const sp_cmd* c, const char* key, int icase) {
    size_t i, kl = strlen(key);
    for (i = c->env_set.n; i > 0; i--)
        if (sp_key_eq(c->env_set.v[i - 1], key, icase)) {
            /* a later remove of the same key wins only if no later set; keep it simple: sets win */
            return c->env_set.v[i - 1] + kl + 1;
        }
    for (i = 0; i < c->env_remove.n; i++) if (sp_key_eq(c->env_remove.v[i], key, icase)) return NULL;
    if (c->env_clear) return NULL;
    return getenv(key);
}
#endif

/* ── Child state ─────────────────────────────────────────────────────────── */

typedef struct {
#if defined(_WIN32)
    HANDLE proc;
    HANDLE io[3];          /* parent ends of pipes (INVALID_HANDLE_VALUE when none) */
#else
    int io[3];             /* parent ends of pipes (-1 when none) */
#endif
    int64_t pid;
    int exited;
    int64_t code, sig;
    /* communicate() results */
    char* buf[2]; size_t blen[2], bcap[2];
} sp_child;

static int sp_buf_add(sp_child* ch, int k, const char* d, size_t n) {
    if (ch->blen[k] + n + 1 > ch->bcap[k]) {
        size_t nc = ch->bcap[k] ? ch->bcap[k] : 65536;
        char* nb;
        while (nc < ch->blen[k] + n + 1) nc *= 2;
        nb = (char*)realloc(ch->buf[k], nc);
        if (!nb) return -1;
        ch->buf[k] = nb; ch->bcap[k] = nc;
    }
    memcpy(ch->buf[k] + ch->blen[k], d, n);
    ch->blen[k] += n;
    return 0;
}

int64_t trc_sp_pid(int64_t c) { return ((sp_child*)(intptr_t)c)->pid; }
int64_t trc_sp_has_pipe(int64_t c, int64_t which) {
    sp_child* ch = (sp_child*)(intptr_t)c;
#if defined(_WIN32)
    return ch->io[which] != INVALID_HANDLE_VALUE;
#else
    return ch->io[which] >= 0;
#endif
}
int64_t trc_sp_code(int64_t c) { return ((sp_child*)(intptr_t)c)->code; }
int64_t trc_sp_signal(int64_t c) { return ((sp_child*)(intptr_t)c)->sig; }
int64_t trc_sp_buf_len(int64_t c, int64_t k) { return (int64_t)((sp_child*)(intptr_t)c)->blen[k]; }

/* Copy communicate() buffer k (0 stdout, 1 stderr) into dst and release it. */
void trc_sp_buf_take(int64_t c, int64_t k, char* dst) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    if (ch->blen[k]) memcpy(dst, ch->buf[k], ch->blen[k]);
    free(ch->buf[k]); ch->buf[k] = NULL; ch->blen[k] = ch->bcap[k] = 0;
}

static int64_t sp_now_ms(void) {
#if defined(_WIN32)
    return (int64_t)GetTickCount64();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}

static void sp_sleep_ms(int ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec ts; ts.tv_sec = ms / 1000; ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

/* ── This process's own stdio, raw and binary-safe ───────────────────────── */

/* Write all n bytes to fd 1 or 2 (after flushing C stdio). 0 ok, -1 error. */
int64_t trc_sp_std_write(int64_t fd, const char* d, int64_t n) {
    int64_t off = 0;
    fflush(stdout); fflush(stderr);
#if defined(_WIN32)
    _setmode((int)fd, _O_BINARY);
    while (off < n) {
        unsigned chunk = (unsigned)((n - off) > 0x10000000 ? 0x10000000 : (n - off));
        int w = _write((int)fd, d + off, chunk);
        if (w < 0) return -1;
        off += w;
    }
#else
    while (off < n) {
        ssize_t w = write((int)fd, d + off, (size_t)(n - off));
        if (w < 0) { if (errno == EINTR) continue; return -1; }
        off += w;
    }
#endif
    return 0;
}

/* Read up to cap bytes from fd 0. Returns bytes, 0 at end, -1 error. */
int64_t trc_sp_std_read(int64_t fd, char* out, int64_t cap) {
#if defined(_WIN32)
    _setmode((int)fd, _O_BINARY);
    return (int64_t)_read((int)fd, out, (unsigned)(cap > 0x10000000 ? 0x10000000 : cap));
#else
    while (1) {
        ssize_t r = read((int)fd, out, (size_t)cap);
        if (r < 0 && errno == EINTR) continue;
        return (int64_t)r;
    }
#endif
}

#if !defined(_WIN32)
/* ════════════════════════════════════ POSIX ════════════════════════════════ */

static char** sp_environ(void) {
#if defined(__APPLE__)
    return *_NSGetEnviron();
#else
    extern char** environ;
    return environ;
#endif
}

#if !(defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
static int sp_set_cloexec(int fd) {
    int f = fcntl(fd, F_GETFD);
    if (f < 0) return -1;
    return fcntl(fd, F_SETFD, f | FD_CLOEXEC);
}
#endif

static int sp_pipe(int fds[2]) {
#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    return pipe2(fds, O_CLOEXEC);
#else
    if (pipe(fds) < 0) return -1;
    sp_set_cloexec(fds[0]); sp_set_cloexec(fds[1]);
    return 0;
#endif
}

static int sp_open_cloexec(const char* path, int flags, int mode) {
    int fd = open(path, flags | O_CLOEXEC, mode);
    return fd;
}

static int sp_is_exec_file(const char* p) {
    struct stat st;
    if (stat(p, &st) != 0) return 0;
    if (!S_ISREG(st.st_mode)) return 0;
    return access(p, X_OK) == 0;
}

/* Resolve the program like execvp, using the child's PATH. Returns malloc'd path or NULL. */
static char* sp_resolve(const sp_cmd* c, const char* prog) {
    const char* path;
    size_t pl = strlen(prog);
    if (strchr(prog, '/')) return sp_dup(prog, (int64_t)pl);
    path = sp_child_getenv(c, "PATH", 0);
    if (!path) path = "/usr/local/bin:/usr/bin:/bin";
    while (1) {
        const char* end = strchr(path, ':');
        size_t dl = end ? (size_t)(end - path) : strlen(path);
        char* cand = (char*)malloc(dl + pl + 2);
        if (!cand) return NULL;
        if (dl == 0) { memcpy(cand, prog, pl + 1); }
        else { memcpy(cand, path, dl); cand[dl] = '/'; memcpy(cand + dl + 1, prog, pl + 1); }
        if (sp_is_exec_file(cand)) {
            /* relative PATH entries are resolved against the child's cwd by execve anyway */
            return cand;
        }
        free(cand);
        if (!end) break;
        path = end + 1;
    }
    return NULL;
}

/* Write all bytes from the child; async-signal-safe. */
static void sp_child_report(int fd, int stage, int err) {
    int msg[2]; ssize_t r;
    msg[0] = stage; msg[1] = err;
    do { r = write(fd, msg, sizeof msg); } while (r < 0 && errno == EINTR);
    _exit(127);
}

static void sp_close_all_from3(int keep) {
#if defined(__linux__) && defined(SYS_close_range)
    /* CLOSE_RANGE_CLOEXEC = 4: mark instead of closing so `keep` survives until exec */
    if (syscall(SYS_close_range, 3u, ~0u, 4u) == 0) return;
#endif
    {
        long mx = sysconf(_SC_OPEN_MAX), fd;
        if (mx < 0 || mx > 65536) mx = 65536;
        for (fd = 3; fd < mx; fd++) if (fd != keep) { int f = fcntl((int)fd, F_GETFD); if (f >= 0 && !(f & FD_CLOEXEC)) fcntl((int)fd, F_SETFD, f | FD_CLOEXEC); }
    }
}

int64_t trc_sp_spawn(int64_t h) {
    sp_cmd* c = (sp_cmd*)(intptr_t)h;
    sp_child* ch;
    int child_fd[3] = { -1, -1, -1 };   /* fds the child gets as 0,1,2 (-1 = inherit) */
    int parent_fd[3] = { -1, -1, -1 };
    int errp[2] = { -1, -1 };
    char* exe = NULL;
    sp_strs envv = { 0, 0, 0 };
    char** envp;
    static char* empty_env[1] = { NULL };
    pid_t pid;
    int i, ok = 0;
    ssize_t r;
    int msg[2];

    if (c->argv.n == 0) { sp_set_err("subprocess: empty argv", 0, 0); return 0; }
    exe = sp_resolve(c, c->argv.v[0]);
    if (!exe) { sp_set_err("subprocess: program not found: ", c->argv.v[0], ""); return 0; }

    /* environment */
    if (!c->env_clear) {
        char** e = sp_environ();
        for (; e && *e; e++) if (!sp_env_overridden(c, *e, 0)) sp_strs_push(&envv, sp_dup(*e, (int64_t)strlen(*e)));
    }
    for (i = 0; i < (int)c->env_set.n; i++) {
        size_t j; int dup = 0;
        /* a later set of the same key wins */
        const char* eq = strchr(c->env_set.v[i], '=');
        for (j = (size_t)i + 1; j < c->env_set.n; j++)
            if (strncmp(c->env_set.v[j], c->env_set.v[i], (size_t)(eq - c->env_set.v[i]) + 1) == 0) dup = 1;
        if (!dup) sp_strs_push(&envv, sp_dup(c->env_set.v[i], (int64_t)strlen(c->env_set.v[i])));
    }
    envp = envv.v ? envv.v : empty_env;

    /* stdio */
    for (i = 0; i < 3; i++) {
        int m = c->mode[i];
        if (m == SP_PIPE) {
            int p[2];
            if (sp_pipe(p) < 0) { sp_set_err("subprocess: pipe: ", strerror(errno), ""); goto fail; }
            if (i == 0) { child_fd[0] = p[0]; parent_fd[0] = p[1]; }
            else { child_fd[i] = p[1]; parent_fd[i] = p[0]; }
        } else if (m == SP_NULL) {
            child_fd[i] = sp_open_cloexec("/dev/null", i == 0 ? O_RDONLY : O_WRONLY, 0);
            if (child_fd[i] < 0) { sp_set_err("subprocess: /dev/null: ", strerror(errno), ""); goto fail; }
        } else if (m == SP_FILE || m == SP_APPEND) {
            int fl = i == 0 ? O_RDONLY : (O_WRONLY | O_CREAT | (m == SP_APPEND ? O_APPEND : O_TRUNC));
            child_fd[i] = sp_open_cloexec(c->path[i], fl, 0666);
            if (child_fd[i] < 0) { sp_set_err("subprocess: cannot open ", c->path[i], ""); strncat(sp_err, ": ", sizeof sp_err - strlen(sp_err) - 1); strncat(sp_err, strerror(errno), sizeof sp_err - strlen(sp_err) - 1); goto fail; }
        }
    }
    if (sp_pipe(errp) < 0) { sp_set_err("subprocess: pipe: ", strerror(errno), ""); goto fail; }

    pid = fork();
    if (pid < 0) { sp_set_err("subprocess: fork: ", strerror(errno), ""); goto fail; }
    if (pid == 0) {
        sigset_t none_set;
        int src[3];
        /* move sources out of 0..2 so dup2 cannot clobber one another */
        for (i = 0; i < 3; i++) {
            src[i] = child_fd[i];
            if (src[i] >= 0 && src[i] < 3 && src[i] != i) {
                int nfd = fcntl(src[i], F_DUPFD, 3);
                if (nfd < 0) sp_child_report(errp[1], 1, errno);
                src[i] = nfd;
            }
        }
        for (i = 0; i < 3; i++) {
            if (i == 2 && c->mode[2] == SP_MERGE) {
                if (dup2(1, 2) < 0) sp_child_report(errp[1], 1, errno);
                continue;
            }
            if (src[i] < 0) continue;
            if (src[i] == i) { int f = fcntl(i, F_GETFD); if (f >= 0) fcntl(i, F_SETFD, f & ~FD_CLOEXEC); }
            else if (dup2(src[i], i) < 0) sp_child_report(errp[1], 1, errno);
        }
        sp_close_all_from3(errp[1]);
        if (c->cwd && chdir(c->cwd) < 0) sp_child_report(errp[1], 2, errno);
        signal(SIGPIPE, SIG_DFL);
        sigemptyset(&none_set);
        sigprocmask(SIG_SETMASK, &none_set, NULL);
        execve(exe, c->argv.v, envp);
        sp_child_report(errp[1], 3, errno);
    }

    /* parent */
    close(errp[1]); errp[1] = -1;
    do { r = read(errp[0], msg, sizeof msg); } while (r < 0 && errno == EINTR);
    close(errp[0]); errp[0] = -1;
    if (r == (ssize_t)sizeof msg) {
        int st;
        while (waitpid(pid, &st, 0) < 0 && errno == EINTR) {}
        if (msg[0] == 2) { sp_set_err("subprocess: cannot change directory to ", c->cwd, ": "); }
        else if (msg[0] == 3) { sp_set_err("subprocess: cannot execute ", c->argv.v[0], ": "); }
        else { sp_set_err("subprocess: cannot set up child stdio", ": ", ""); }
        strncat(sp_err, strerror(msg[1]), sizeof sp_err - strlen(sp_err) - 1);
        goto fail;
    }
    ch = (sp_child*)calloc(1, sizeof(sp_child));
    if (!ch) { sp_set_err("subprocess: out of memory", 0, 0); goto fail; }
    ch->pid = pid;
    for (i = 0; i < 3; i++) { ch->io[i] = parent_fd[i]; parent_fd[i] = -1; }
    ok = 1;

fail:
    for (i = 0; i < 3; i++) { if (child_fd[i] >= 0) close(child_fd[i]); if (parent_fd[i] >= 0) close(parent_fd[i]); }
    if (errp[0] >= 0) close(errp[0]);
    if (errp[1] >= 0) close(errp[1]);
    free(exe);
    sp_strs_free(&envv);
    return ok ? (int64_t)(intptr_t)ch : 0;
}

/* Run a write with SIGPIPE blocked; EPIPE is returned instead of killing us. */
typedef struct { sigset_t old; int was_pending; } sp_sigguard;

static void sp_sig_begin(sp_sigguard* g) {
    sigset_t s, pend;
    sigemptyset(&s); sigaddset(&s, SIGPIPE);
    sigpending(&pend);
    g->was_pending = sigismember(&pend, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &s, &g->old);
}

static void sp_sig_end(sp_sigguard* g, int got_epipe) {
    if (got_epipe && !g->was_pending) {
        sigset_t s, pend; int sig;
        sigemptyset(&s); sigaddset(&s, SIGPIPE);
        sigpending(&pend);
        if (sigismember(&pend, SIGPIPE)) sigwait(&s, &sig);
    }
    pthread_sigmask(SIG_SETMASK, &g->old, NULL);
}

/* Write all n bytes to the child's stdin. 0 ok, -1 error, -3 broken pipe (child closed stdin). */
int64_t trc_sp_write(int64_t c, const char* d, int64_t n) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    sp_sigguard g;
    int64_t off = 0, rc = 0;
    if (ch->io[0] < 0) { sp_set_err("subprocess: stdin is not piped (or already closed)", 0, 0); return -1; }
    sp_sig_begin(&g);
    while (off < n) {
        ssize_t w = write(ch->io[0], d + off, (size_t)(n - off));
        if (w < 0) {
            if (errno == EINTR) continue;
            if (errno == EPIPE) { rc = -3; sp_set_err("subprocess: broken pipe (child closed its stdin)", 0, 0); }
            else { rc = -1; sp_set_err("subprocess: write to stdin: ", strerror(errno), ""); }
            break;
        }
        off += w;
    }
    sp_sig_end(&g, rc == -3);
    return rc;
}

void trc_sp_close_pipe(int64_t c, int64_t which) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    if (ch->io[which] >= 0) { close(ch->io[which]); ch->io[which] = -1; }
}

/* Read up to cap bytes from stdout (1) or stderr (2). timeout_ms < 0 blocks.
 * Returns bytes read, 0 at end of stream, -1 error, -2 timeout. */
int64_t trc_sp_read(int64_t c, int64_t which, char* out, int64_t cap, int64_t timeout_ms) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    int fd = ch->io[which];
    if (fd < 0) { sp_set_err(which == 1 ? "subprocess: stdout is not piped" : "subprocess: stderr is not piped", 0, 0); return -1; }
    if (timeout_ms >= 0) {
        struct pollfd p; int r;
        p.fd = fd; p.events = POLLIN; p.revents = 0;
        do { r = poll(&p, 1, (int)timeout_ms); } while (r < 0 && errno == EINTR);
        if (r < 0) { sp_set_err("subprocess: poll: ", strerror(errno), ""); return -1; }
        if (r == 0) return -2;
    }
    while (1) {
        ssize_t r = read(fd, out, (size_t)cap);
        if (r < 0 && errno == EINTR) continue;
        if (r < 0) { sp_set_err("subprocess: read: ", strerror(errno), ""); return -1; }
        return (int64_t)r;
    }
}

static void sp_record_status(sp_child* ch, int st) {
    ch->exited = 1;
    if (WIFEXITED(st)) { ch->code = WEXITSTATUS(st); ch->sig = 0; }
    else if (WIFSIGNALED(st)) { ch->code = -1; ch->sig = WTERMSIG(st); }
    else { ch->code = -1; ch->sig = 0; }
}

/* Wait for exit. timeout_ms < 0 blocks, 0 polls. Returns 1 exited, 0 still running, -1 error. */
int64_t trc_sp_wait(int64_t c, int64_t timeout_ms) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    int64_t deadline = timeout_ms > 0 ? sp_now_ms() + timeout_ms : 0;
    int nap = 1;
    if (ch->exited) return 1;
    while (1) {
        int st; pid_t r;
        r = waitpid((pid_t)ch->pid, &st, timeout_ms < 0 ? 0 : WNOHANG);
        if (r < 0) {
            if (errno == EINTR) continue;
            sp_set_err("subprocess: waitpid: ", strerror(errno), "");
            return -1;
        }
        if (r == (pid_t)ch->pid) { sp_record_status(ch, st); return 1; }
        if (timeout_ms == 0) return 0;
        if (sp_now_ms() >= deadline) return 0;
        sp_sleep_ms(nap);
        if (nap < 20) nap *= 2;
    }
}

/* Send a signal (POSIX); force=1 from kill() means SIGKILL. */
int64_t trc_sp_signal_send(int64_t c, int64_t sig) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    if (ch->exited) return 0;
    if (kill((pid_t)ch->pid, (int)sig) < 0) {
        if (errno == ESRCH) return 0;
        sp_set_err("subprocess: kill: ", strerror(errno), "");
        return -1;
    }
    return 0;
}

int64_t trc_sp_kill(int64_t c) { return trc_sp_signal_send(c, SIGKILL); }

/* Feed `in` to stdin (if piped) while draining stdout and stderr into the
 * internal buffers until both reach end of stream. Returns 0, -1 error, -2 timeout. */
int64_t trc_sp_communicate(int64_t c, const char* in, int64_t in_len, int64_t timeout_ms) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    int64_t off = 0, deadline = timeout_ms >= 0 ? sp_now_ms() + timeout_ms : 0, rc = 0;
    char buf[65536];
    sp_sigguard g;
    int got_epipe = 0;
    if (ch->io[0] >= 0) {
        if (in_len == 0) trc_sp_close_pipe(c, 0);
        else { int f = fcntl(ch->io[0], F_GETFL); fcntl(ch->io[0], F_SETFL, f | O_NONBLOCK); }
    }
    sp_sig_begin(&g);
    while (ch->io[0] >= 0 || ch->io[1] >= 0 || ch->io[2] >= 0) {
        struct pollfd p[3]; int idx[3]; int np = 0, k, r, wait_ms = -1;
        if (ch->io[0] >= 0) { p[np].fd = ch->io[0]; p[np].events = POLLOUT; p[np].revents = 0; idx[np++] = 0; }
        for (k = 1; k < 3; k++) if (ch->io[k] >= 0) { p[np].fd = ch->io[k]; p[np].events = POLLIN; p[np].revents = 0; idx[np++] = k; }
        if (timeout_ms >= 0) {
            int64_t left = deadline - sp_now_ms();
            if (left <= 0) { rc = -2; sp_set_err("subprocess: timed out", 0, 0); break; }
            wait_ms = left > 2000000000 ? 2000000000 : (int)left;
        }
        r = poll(p, (nfds_t)np, wait_ms);
        if (r < 0) { if (errno == EINTR) continue; rc = -1; sp_set_err("subprocess: poll: ", strerror(errno), ""); break; }
        if (r == 0) continue;
        for (k = 0; k < np; k++) {
            int w = idx[k];
            if (!p[k].revents) continue;
            if (w == 0) {
                if (p[k].revents & (POLLERR | POLLHUP | POLLNVAL)) { trc_sp_close_pipe(c, 0); continue; }
                {
                    size_t chunk = (size_t)(in_len - off); ssize_t n;
                    if (chunk > 65536) chunk = 65536;
                    n = write(ch->io[0], in + off, chunk);
                    if (n < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) continue;
                        if (errno == EPIPE) got_epipe = 1;
                        trc_sp_close_pipe(c, 0);   /* child stopped reading: not an error */
                        continue;
                    }
                    off += n;
                    if (off >= in_len) trc_sp_close_pipe(c, 0);
                }
            } else {
                ssize_t n = read(ch->io[w], buf, sizeof buf);
                if (n < 0) { if (errno == EINTR || errno == EAGAIN) continue; rc = -1; sp_set_err("subprocess: read: ", strerror(errno), ""); goto done; }
                if (n == 0) { trc_sp_close_pipe(c, w); continue; }
                if (sp_buf_add(ch, w - 1, buf, (size_t)n) < 0) { rc = -1; sp_set_err("subprocess: out of memory", 0, 0); goto done; }
            }
        }
    }
done:
    sp_sig_end(&g, got_epipe);
    if (rc == 0 && timeout_ms >= 0) {
        /* pipes are closed but the process may still be running */
        int64_t left = deadline - sp_now_ms();
        int64_t w = trc_sp_wait(c, left > 0 ? left : 0);
        if (w < 0) rc = -1;
        else if (w == 0) { rc = -2; sp_set_err("subprocess: timed out", 0, 0); }
    }
    return rc;
}

void trc_sp_free(int64_t c) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    int i;
    if (!ch) return;
    for (i = 0; i < 3; i++) if (ch->io[i] >= 0) close(ch->io[i]);
    free(ch->buf[0]); free(ch->buf[1]);
    free(ch);
}

int64_t trc_sp_platform(void) {
#if defined(__APPLE__)
    return 2;
#else
    return 1;
#endif
}

#else
/* ═══════════════════════════════════ Windows ═══════════════════════════════ */

static void sp_win_err(const char* what, const char* detail) {
    char buf[300]; DWORD e = GetLastError(); DWORD n;
    n = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e, 0, buf, sizeof buf, NULL);
    while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r' || buf[n - 1] == '.')) buf[--n] = 0;
    if (n == 0) snprintf(buf, sizeof buf, "error %lu", (unsigned long)e);
    snprintf(sp_err, sizeof sp_err, "%s%s%s%s", what, detail ? detail : "", detail && detail[0] ? ": " : "", buf);
}

/* UTF-8 -> malloc'd UTF-16 (NULL on failure). */
static wchar_t* sp_wide(const char* s) {
    int n = MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
    wchar_t* w;
    if (n <= 0) return NULL;
    w = (wchar_t*)malloc((size_t)n * sizeof(wchar_t));
    if (!w) return NULL;
    MultiByteToWideChar(CP_UTF8, 0, s, -1, w, n);
    return w;
}

/* Growable byte buffer for the command line. */
typedef struct { char* d; size_t n, cap; } sp_sb;
static void sp_sb_add(sp_sb* b, const char* s, size_t n) {
    if (b->n + n + 1 > b->cap) {
        size_t nc = b->cap ? b->cap * 2 : 256;
        while (nc < b->n + n + 1) nc *= 2;
        b->d = (char*)realloc(b->d, nc); b->cap = nc;
    }
    memcpy(b->d + b->n, s, n); b->n += n; b->d[b->n] = 0;
}
static void sp_sb_ch(sp_sb* b, char c, size_t times) { while (times--) sp_sb_add(b, &c, 1); }

/* Append one argument quoted by the MSVCRT / CommandLineToArgvW rules. */
static void sp_quote_arg(sp_sb* b, const char* a) {
    const char* p;
    if (a[0] && !strpbrk(a, " \t\n\v\"")) { sp_sb_add(b, a, strlen(a)); return; }
    sp_sb_ch(b, '"', 1);
    for (p = a; ; p++) {
        size_t bs = 0;
        while (*p == '\\') { bs++; p++; }
        if (*p == 0) { sp_sb_ch(b, '\\', bs * 2); break; }
        if (*p == '"') { sp_sb_ch(b, '\\', bs * 2 + 1); sp_sb_ch(b, '"', 1); }
        else { sp_sb_ch(b, '\\', bs); sp_sb_ch(b, *p, 1); }
    }
    sp_sb_ch(b, '"', 1);
}

static int sp_wkey_eq(const wchar_t* entry, const wchar_t* key, size_t kl) {
    return _wcsnicmp(entry, key, kl) == 0 && (entry[kl] == L'=' || entry[kl] == 0);
}

/* Build a UTF-16 environment block (malloc'd) or NULL with sp_err set. */
static wchar_t* sp_env_block(const sp_cmd* c) {
    size_t cap = 4096, n = 0, i;
    wchar_t* blk = (wchar_t*)malloc(cap * sizeof(wchar_t));
    wchar_t* sys = NULL;
    if (!blk) return NULL;
#define SP_ADDW(w) do { size_t L_ = wcslen(w) + 1; if (n + L_ + 2 > cap) { while (n + L_ + 2 > cap) cap *= 2; blk = (wchar_t*)realloc(blk, cap * sizeof(wchar_t)); } memcpy(blk + n, (w), L_ * sizeof(wchar_t)); n += L_; } while (0)
    if (!c->env_clear) {
        wchar_t* e;
        sys = GetEnvironmentStringsW();
        for (e = sys; e && *e; e += wcslen(e) + 1) {
            const wchar_t* eq = wcschr(e + (e[0] == L'=' ? 1 : 0), L'=');
            size_t kl = eq ? (size_t)(eq - e) : wcslen(e);
            int skip = 0;
            for (i = 0; i < c->env_remove.n && !skip; i++) { wchar_t* k = sp_wide(c->env_remove.v[i]); if (k && wcslen(k) == kl && sp_wkey_eq(e, k, kl)) skip = 1; free(k); }
            for (i = 0; i < c->env_set.n && !skip; i++) { wchar_t* k = sp_wide(c->env_set.v[i]); if (k) { wchar_t* q = wcschr(k, L'='); if (q && (size_t)(q - k) == kl && _wcsnicmp(e, k, kl) == 0) skip = 1; } free(k); }
            if (!skip) SP_ADDW(e);
        }
        if (sys) FreeEnvironmentStringsW(sys);
    }
    for (i = 0; i < c->env_set.n; i++) {
        size_t j; int dup = 0;
        const char* eq = strchr(c->env_set.v[i], '=');
        for (j = i + 1; j < c->env_set.n; j++) if (_strnicmp(c->env_set.v[j], c->env_set.v[i], (size_t)(eq - c->env_set.v[i]) + 1) == 0) dup = 1;
        if (!dup) { wchar_t* w = sp_wide(c->env_set.v[i]); if (w) { SP_ADDW(w); free(w); } }
    }
#undef SP_ADDW
    if (n == 0) blk[n++] = 0;
    blk[n++] = 0;
    return blk;
}

static HANDLE sp_inheritable_dup(HANDLE h) {
    HANDLE d = NULL;
    if (h == NULL || h == INVALID_HANDLE_VALUE) return NULL;
    if (!DuplicateHandle(GetCurrentProcess(), h, GetCurrentProcess(), &d, 0, TRUE, DUPLICATE_SAME_ACCESS)) return NULL;
    return d;
}

int64_t trc_sp_spawn(int64_t h) {
    sp_cmd* c = (sp_cmd*)(intptr_t)h;
    sp_child* ch = NULL;
    HANDLE child_h[3] = { NULL, NULL, NULL };
    HANDLE parent_h[3] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };
    SECURITY_ATTRIBUTES sa;
    STARTUPINFOEXW si;
    PROCESS_INFORMATION pi;
    LPPROC_THREAD_ATTRIBUTE_LIST attrs = NULL;
    SIZE_T asz = 0;
    HANDLE list[3]; DWORD nlist = 0;
    sp_sb cl = { 0, 0, 0 };
    wchar_t *wcl = NULL, *wcwd = NULL, *wenv = NULL;
    int i, ok = 0;
    size_t k;
    static const DWORD std_ids[3] = { STD_INPUT_HANDLE, STD_OUTPUT_HANDLE, STD_ERROR_HANDLE };

    if (c->argv.n == 0) { sp_set_err("subprocess: empty argv", 0, 0); return 0; }
    sa.nLength = sizeof sa; sa.lpSecurityDescriptor = NULL; sa.bInheritHandle = TRUE;

    for (i = 0; i < 3; i++) {
        int m = c->mode[i];
        if (m == SP_PIPE) {
            HANDLE r, w;
            if (!CreatePipe(&r, &w, &sa, 65536)) { sp_win_err("subprocess: CreatePipe: ", ""); goto fail; }
            if (i == 0) { child_h[0] = r; parent_h[0] = w; } else { child_h[i] = w; parent_h[i] = r; }
            SetHandleInformation(parent_h[i], HANDLE_FLAG_INHERIT, 0);
        } else if (m == SP_NULL) {
            child_h[i] = CreateFileW(L"NUL", i == 0 ? GENERIC_READ : GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, NULL);
            if (child_h[i] == INVALID_HANDLE_VALUE) { child_h[i] = NULL; sp_win_err("subprocess: NUL: ", ""); goto fail; }
        } else if (m == SP_FILE || m == SP_APPEND) {
            wchar_t* wp = sp_wide(c->path[i]);
            DWORD acc = i == 0 ? GENERIC_READ : (m == SP_APPEND ? FILE_APPEND_DATA | SYNCHRONIZE : GENERIC_WRITE);
            DWORD disp = i == 0 ? OPEN_EXISTING : (m == SP_APPEND ? OPEN_ALWAYS : CREATE_ALWAYS);
            child_h[i] = wp ? CreateFileW(wp, acc, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, &sa, disp, FILE_ATTRIBUTE_NORMAL, NULL) : INVALID_HANDLE_VALUE;
            free(wp);
            if (child_h[i] == INVALID_HANDLE_VALUE) { child_h[i] = NULL; sp_win_err("subprocess: cannot open ", c->path[i]); goto fail; }
        } else if (m == SP_MERGE) {
            /* filled below from stdout */
        } else {
            child_h[i] = sp_inheritable_dup(GetStdHandle(std_ids[i]));
        }
    }
    if (c->mode[2] == SP_MERGE) child_h[2] = sp_inheritable_dup(child_h[1] ? child_h[1] : GetStdHandle(STD_OUTPUT_HANDLE));

    /* command line */
    if (c->raw_cmdline) sp_sb_add(&cl, c->raw_cmdline, strlen(c->raw_cmdline));
    else for (k = 0; k < c->argv.n; k++) { if (k) sp_sb_ch(&cl, ' ', 1); sp_quote_arg(&cl, c->argv.v[k]); }
    wcl = sp_wide(cl.d ? cl.d : "");
    if (!wcl) { sp_set_err("subprocess: invalid UTF-8 in command line", 0, 0); goto fail; }
    if (c->cwd) { wcwd = sp_wide(c->cwd); if (!wcwd) { sp_set_err("subprocess: invalid UTF-8 in directory", 0, 0); goto fail; } }
    if (c->env_clear || c->env_set.n || c->env_remove.n) { wenv = sp_env_block(c); if (!wenv) { sp_set_err("subprocess: out of memory", 0, 0); goto fail; } }

    memset(&si, 0, sizeof si);
    si.StartupInfo.cb = sizeof si;
    si.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
    si.StartupInfo.hStdInput = child_h[0];
    si.StartupInfo.hStdOutput = child_h[1];
    si.StartupInfo.hStdError = child_h[2];
    for (i = 0; i < 3; i++) {
        DWORD j; int dup = 0;
        if (!child_h[i]) continue;
        for (j = 0; j < nlist; j++) if (list[j] == child_h[i]) dup = 1;
        if (!dup) list[nlist++] = child_h[i];
    }
    if (nlist) {
        InitializeProcThreadAttributeList(NULL, 1, 0, &asz);
        attrs = (LPPROC_THREAD_ATTRIBUTE_LIST)malloc(asz);
        if (!attrs || !InitializeProcThreadAttributeList(attrs, 1, 0, &asz)) { free(attrs); attrs = NULL; sp_win_err("subprocess: attribute list: ", ""); goto fail; }
        if (!UpdateProcThreadAttribute(attrs, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, list, nlist * sizeof(HANDLE), NULL, NULL)) { sp_win_err("subprocess: attribute list: ", ""); goto fail; }
        si.lpAttributeList = attrs;
    }
    memset(&pi, 0, sizeof pi);
    if (!CreateProcessW(NULL, wcl, NULL, NULL, nlist ? TRUE : FALSE,
                        CREATE_UNICODE_ENVIRONMENT | (attrs ? EXTENDED_STARTUPINFO_PRESENT : 0),
                        wenv, wcwd, &si.StartupInfo, &pi)) {
        DWORD e = GetLastError();
        if (e == ERROR_DIRECTORY) {
            sp_set_err("subprocess: cannot change directory to ", c->cwd ? c->cwd : "", ": not a directory");
        } else if (e == ERROR_FILE_NOT_FOUND || e == ERROR_PATH_NOT_FOUND) {
            if (wcwd && GetFileAttributesW(wcwd) == INVALID_FILE_ATTRIBUTES) sp_set_err("subprocess: cannot change directory to ", c->cwd, ": not found");
            else sp_set_err("subprocess: program not found: ", c->argv.v[0], "");
        } else { SetLastError(e); sp_win_err("subprocess: cannot execute ", c->argv.v[0]); }
        goto fail;
    }
    CloseHandle(pi.hThread);
    ch = (sp_child*)calloc(1, sizeof(sp_child));
    if (!ch) { TerminateProcess(pi.hProcess, 1); CloseHandle(pi.hProcess); sp_set_err("subprocess: out of memory", 0, 0); goto fail; }
    ch->proc = pi.hProcess;
    ch->pid = (int64_t)pi.dwProcessId;
    for (i = 0; i < 3; i++) { ch->io[i] = parent_h[i]; parent_h[i] = INVALID_HANDLE_VALUE; }
    ok = 1;

fail:
    for (i = 0; i < 3; i++) {
        if (child_h[i]) CloseHandle(child_h[i]);
        if (parent_h[i] != INVALID_HANDLE_VALUE) CloseHandle(parent_h[i]);
    }
    if (attrs) { DeleteProcThreadAttributeList(attrs); free(attrs); }
    free(cl.d); free(wcl); free(wcwd); free(wenv);
    return ok ? (int64_t)(intptr_t)ch : 0;
}

int64_t trc_sp_write(int64_t c, const char* d, int64_t n) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    int64_t off = 0;
    if (ch->io[0] == INVALID_HANDLE_VALUE) { sp_set_err("subprocess: stdin is not piped (or already closed)", 0, 0); return -1; }
    while (off < n) {
        DWORD w = 0, want = (DWORD)((n - off) > 0x10000000 ? 0x10000000 : (n - off));
        if (!WriteFile(ch->io[0], d + off, want, &w, NULL)) {
            DWORD e = GetLastError();
            if (e == ERROR_BROKEN_PIPE || e == ERROR_NO_DATA) { sp_set_err("subprocess: broken pipe (child closed its stdin)", 0, 0); return -3; }
            sp_win_err("subprocess: write to stdin: ", ""); return -1;
        }
        off += w;
    }
    return 0;
}

void trc_sp_close_pipe(int64_t c, int64_t which) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    if (ch->io[which] != INVALID_HANDLE_VALUE) { CloseHandle(ch->io[which]); ch->io[which] = INVALID_HANDLE_VALUE; }
}

int64_t trc_sp_read(int64_t c, int64_t which, char* out, int64_t cap, int64_t timeout_ms) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    HANDLE hd = ch->io[which];
    DWORD got = 0;
    if (hd == INVALID_HANDLE_VALUE) { sp_set_err(which == 1 ? "subprocess: stdout is not piped" : "subprocess: stderr is not piped", 0, 0); return -1; }
    if (timeout_ms >= 0) {
        int64_t deadline = sp_now_ms() + timeout_ms;
        int nap = 1;
        while (1) {
            DWORD avail = 0;
            if (!PeekNamedPipe(hd, NULL, 0, NULL, &avail, NULL)) {
                if (GetLastError() == ERROR_BROKEN_PIPE) return 0;
                sp_win_err("subprocess: read: ", ""); return -1;
            }
            if (avail > 0) { if ((int64_t)avail < cap) cap = (int64_t)avail; break; }
            if (sp_now_ms() >= deadline) return -2;
            sp_sleep_ms(nap); if (nap < 10) nap *= 2;
        }
    }
    if (cap > 0x10000000) cap = 0x10000000;
    if (!ReadFile(hd, out, (DWORD)cap, &got, NULL)) {
        if (GetLastError() == ERROR_BROKEN_PIPE) return 0;
        sp_win_err("subprocess: read: ", ""); return -1;
    }
    return (int64_t)got;
}

static void sp_record_status(sp_child* ch) {
    DWORD code = 0;
    GetExitCodeProcess(ch->proc, &code);
    ch->exited = 1; ch->code = (int64_t)code; ch->sig = 0;
}

int64_t trc_sp_wait(int64_t c, int64_t timeout_ms) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    DWORD r;
    if (ch->exited) return 1;
    r = WaitForSingleObject(ch->proc, timeout_ms < 0 ? INFINITE : (DWORD)(timeout_ms > 0x7ffffff0 ? 0x7ffffff0 : timeout_ms));
    if (r == WAIT_OBJECT_0) { sp_record_status(ch); return 1; }
    if (r == WAIT_TIMEOUT) return 0;
    sp_win_err("subprocess: wait: ", "");
    return -1;
}

int64_t trc_sp_signal_send(int64_t c, int64_t sig) {
    (void)c; (void)sig;
    sp_set_err("subprocess: signals are not available on Windows", 0, 0);
    return -1;
}

int64_t trc_sp_kill(int64_t c) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    if (ch->exited) return 0;
    if (!TerminateProcess(ch->proc, 1)) {
        if (WaitForSingleObject(ch->proc, 0) == WAIT_OBJECT_0) return 0;
        sp_win_err("subprocess: kill: ", ""); return -1;
    }
    return 0;
}

/* Windows communicate: a thread writes stdin (WriteFile on an anonymous pipe
 * blocks); the calling thread drains stdout/stderr with PeekNamedPipe. */
typedef struct { HANDLE h; const char* d; int64_t n; } sp_wjob;

static DWORD WINAPI sp_writer_thread(LPVOID arg) {
    sp_wjob* j = (sp_wjob*)arg;
    int64_t off = 0;
    while (off < j->n) {
        DWORD w = 0, want = (DWORD)((j->n - off) > 65536 ? 65536 : (j->n - off));
        if (!WriteFile(j->h, j->d + off, want, &w, NULL)) break;
        off += w;
    }
    CloseHandle(j->h);
    return 0;
}

int64_t trc_sp_communicate(int64_t c, const char* in, int64_t in_len, int64_t timeout_ms) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    int64_t deadline = timeout_ms >= 0 ? sp_now_ms() + timeout_ms : 0, rc = 0;
    HANDLE wt = NULL;
    sp_wjob job;
    char* buf = (char*)malloc(65536);
    int nap = 1;
    if (!buf) { sp_set_err("subprocess: out of memory", 0, 0); return -1; }
    if (ch->io[0] != INVALID_HANDLE_VALUE) {
        if (in_len == 0) trc_sp_close_pipe(c, 0);
        else {
            job.h = ch->io[0]; job.d = in; job.n = in_len;
            ch->io[0] = INVALID_HANDLE_VALUE;   /* owned by the writer thread now */
            wt = CreateThread(NULL, 0, sp_writer_thread, &job, 0, NULL);
            if (!wt) { CloseHandle(job.h); sp_win_err("subprocess: CreateThread: ", ""); free(buf); return -1; }
        }
    }
    while (ch->io[1] != INVALID_HANDLE_VALUE || ch->io[2] != INVALID_HANDLE_VALUE) {
        int k, progress = 0;
        for (k = 1; k < 3; k++) {
            DWORD avail = 0, got = 0;
            if (ch->io[k] == INVALID_HANDLE_VALUE) continue;
            if (!PeekNamedPipe(ch->io[k], NULL, 0, NULL, &avail, NULL)) {
                if (GetLastError() == ERROR_BROKEN_PIPE) { trc_sp_close_pipe(c, k); progress = 1; continue; }
                sp_win_err("subprocess: read: ", ""); rc = -1; goto done;
            }
            if (avail == 0) continue;
            if (avail > 65536) avail = 65536;
            if (!ReadFile(ch->io[k], buf, avail, &got, NULL)) {
                if (GetLastError() == ERROR_BROKEN_PIPE) { trc_sp_close_pipe(c, k); progress = 1; continue; }
                sp_win_err("subprocess: read: ", ""); rc = -1; goto done;
            }
            if (sp_buf_add(ch, k - 1, buf, got) < 0) { sp_set_err("subprocess: out of memory", 0, 0); rc = -1; goto done; }
            progress = 1;
        }
        if (progress) { nap = 1; continue; }
        if (timeout_ms >= 0 && sp_now_ms() >= deadline) { rc = -2; sp_set_err("subprocess: timed out", 0, 0); goto done; }
        sp_sleep_ms(nap); if (nap < 8) nap *= 2;
    }
done:
    if (wt) {
        if (rc != 0) TerminateProcess(ch->proc, 1);   /* unblocks the writer */
        WaitForSingleObject(wt, INFINITE);
        CloseHandle(wt);
    }
    free(buf);
    if (rc == 0 && timeout_ms >= 0) {
        int64_t left = deadline - sp_now_ms();
        int64_t w = trc_sp_wait(c, left > 0 ? left : 0);
        if (w < 0) rc = -1;
        else if (w == 0) { rc = -2; sp_set_err("subprocess: timed out", 0, 0); }
    }
    return rc;
}

void trc_sp_free(int64_t c) {
    sp_child* ch = (sp_child*)(intptr_t)c;
    int i;
    if (!ch) return;
    for (i = 0; i < 3; i++) if (ch->io[i] != INVALID_HANDLE_VALUE) CloseHandle(ch->io[i]);
    if (ch->proc) CloseHandle(ch->proc);
    free(ch->buf[0]); free(ch->buf[1]);
    free(ch);
}

int64_t trc_sp_platform(void) { return 3; }

#endif
