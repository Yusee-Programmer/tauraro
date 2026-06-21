#!/usr/bin/env bash
# reactor_stress.sh — build + run the reactor keep-alive stress test, and (on
# Linux, if strace is present) count the epoll_ctl syscalls it makes.
#
# This is the safety gate + measurement harness for reactor changes (notably the
# persistent-epoll-registration optimization): correctness is asserted by the
# test's own OK line; the epoll_ctl count quantifies the syscall savings.
#
#   scripts/reactor_stress.sh              # correctness only
#   scripts/reactor_stress.sh --count      # + epoll_ctl syscall count (Linux)
#
# Exit 0 iff the test prints "REACTOR-STRESS OK" and no "FAILED".

set -uo pipefail

TAURAROC="${BOOTSTRAP_BIN:-./tauraroc}"
[ -x "$TAURAROC" ] || { [ -x ./tauraroc.exe ] && TAURAROC=./tauraroc.exe; }
if [ ! -x "$TAURAROC" ] && ! command -v "$TAURAROC" &>/dev/null; then
    echo "ERROR: tauraroc not found ($TAURAROC). Build it or set BOOTSTRAP_BIN." >&2
    exit 1
fi

TEST="tests/reactor/keepalive_stress.tr"
COUNT=0
[ "${1:-}" = "--count" ] && COUNT=1

# Portable timeout: GNU `timeout` (Linux / Git-Bash), `gtimeout` (macOS with
# coreutils), else none — macOS runners ship no `timeout`. Running without it is
# safe here: the test self-terminates (clients do a fixed number of round-trips,
# then main returns and the process exits, killing the detached server thread).
TO=""
if command -v timeout >/dev/null 2>&1; then TO="timeout 120"
elif command -v gtimeout >/dev/null 2>&1; then TO="gtimeout 120"
fi

run_once() {
    $TO "$TAURAROC" --run "$TEST" 2>&1
}

if [ "$COUNT" -eq 1 ] && [ "$(uname)" = "Linux" ] && command -v strace >/dev/null 2>&1; then
    echo "==> running under strace (counting epoll_ctl) ..."
    # -f follows the server + client threads; -e trace limits the count output.
    out="$(strace -f -e trace=epoll_ctl -c -- "$TAURAROC" --run "$TEST" 2>strace.out)"
    echo "$out"
    echo "----- epoll_ctl syscalls -----"
    grep -E "epoll_ctl|syscall" strace.out | tail -3 || true
    rm -f strace.out
else
    out="$(run_once)"
    echo "$out"
fi

if echo "$out" | grep -q "REACTOR-STRESS OK" && ! echo "$out" | grep -q "FAILED"; then
    echo "reactor_stress: PASS"
    exit 0
fi
echo "reactor_stress: FAIL"
exit 1
