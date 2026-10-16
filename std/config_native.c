/*
 * std/config_native.c -- environment enumeration for std.config.
 *
 *   trc_cfg_env_count()            number of NAME=VALUE entries in environ
 *   trc_cfg_env_len(i)             byte length of entry i (-1 if out of range)
 *   trc_cfg_env_copy(i, buf, cap)  copy entry i into buf (no NUL needed by the
 *                                  caller; returns bytes copied, -1 on error)
 *
 * The runtime only exposes getenv(), which cannot list variables; layered
 * configuration needs every APP_* variable. Linked through
 * `# tauraro-cpp-shim: config_native.c` in std/config.tr.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if defined(TAURARO_BARE)
#  define TRC_CFG_ENVIRON ((char**)0)
#elif defined(_WIN32)
#  define TRC_CFG_ENVIRON _environ
#elif defined(__APPLE__)
#  include <crt_externs.h>
#  define TRC_CFG_ENVIRON (*_NSGetEnviron())
#else
extern char **environ;
#  define TRC_CFG_ENVIRON environ
#endif

int64_t trc_cfg_env_count(void) {
    char **e = TRC_CFG_ENVIRON;
    int64_t n = 0;
    if (!e) return 0;
    while (e[n]) n++;
    return n;
}

int64_t trc_cfg_env_len(int64_t i) {
    if (i < 0 || i >= trc_cfg_env_count()) return -1;
    return (int64_t)strlen(TRC_CFG_ENVIRON[i]);
}

int64_t trc_cfg_env_copy(int64_t i, char *buf, int64_t cap) {
    if (i < 0 || i >= trc_cfg_env_count() || cap <= 0) return -1;
    size_t n = strlen(TRC_CFG_ENVIRON[i]);
    if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(buf, TRC_CFG_ENVIRON[i], n);
    return (int64_t)n;
}
