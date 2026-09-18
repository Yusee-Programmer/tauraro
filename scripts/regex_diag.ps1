# TEMPORARY diagnostic script -- not part of the normal build. Prints exactly what
# this runner's MinGW provides for POSIX regex: which libs exist, and whether
# regcomp/regexec actually WORK (compile, link, AND produce a correct match),
# both with and without -lsystre -ltre. Remove once the real fix is confirmed.
$ErrorActionPreference = "Continue"

Write-Host "=== gcc version ==="
gcc --version

Write-Host "=== gcc -print-search-dirs (libraries line) ==="
$dirs = gcc -print-search-dirs 2>&1 | Select-String "^libraries:"
Write-Host $dirs

Write-Host "=== searching for regex-related lib files ==="
$libLine = ($dirs -replace "^libraries: =", "")
$paths = $libLine -split ";" | Where-Object { $_ -ne "" }
foreach ($p in $paths) {
    if (Test-Path $p) {
        Get-ChildItem -Path $p -Filter "*regex*" -ErrorAction SilentlyContinue | ForEach-Object { Write-Host "  FOUND: $($_.FullName)" }
        Get-ChildItem -Path $p -Filter "*systre*" -ErrorAction SilentlyContinue | ForEach-Object { Write-Host "  FOUND: $($_.FullName)" }
        Get-ChildItem -Path $p -Filter "*tre*"    -ErrorAction SilentlyContinue | ForEach-Object { Write-Host "  FOUND: $($_.FullName)" }
    }
}

Write-Host "=== does <regex.h> exist, and where ==="
"#include <regex.h>`nint main(void){return 0;}" | Out-File -Encoding ascii _rdiag_hdr.c
gcc -E -v _rdiag_hdr.c 2>&1 | Select-String "regex.h"

$probeSrc = @'
#include <regex.h>
#include <stdio.h>
int main(void) {
    regex_t re;
    int rc = regcomp(&re, "(a+)b", REG_EXTENDED);
    if (rc != 0) {
        char buf[256];
        regerror(rc, &re, buf, sizeof(buf));
        printf("REGCOMP_FAILED: %s\n", buf);
        return 1;
    }
    regmatch_t m[2];
    rc = regexec(&re, "xxaaabxx", 2, m, 0);
    if (rc != 0) {
        printf("REGEXEC_NO_MATCH\n");
        regfree(&re);
        return 2;
    }
    printf("MATCH whole=[%d,%d) group1=[%d,%d) nsub=%d\n", (int)m[0].rm_so, (int)m[0].rm_eo, (int)m[1].rm_so, (int)m[1].rm_eo, (int)re.re_nsub);
    regfree(&re);
    return 0;
}
'@
$probeSrc | Out-File -Encoding ascii _rdiag_probe.c

Write-Host "=== probe: compile+link WITHOUT extra libs ==="
gcc _rdiag_probe.c -o _rdiag_probe_noextra.exe 2>&1
if (Test-Path _rdiag_probe_noextra.exe) {
    Write-Host "  link: OK -- running..."
    & .\_rdiag_probe_noextra.exe
    Write-Host "  exit code: $LASTEXITCODE"
} else {
    Write-Host "  link: FAILED"
}

Write-Host "=== probe: compile+link WITH -lsystre -ltre ==="
gcc _rdiag_probe.c -lsystre -ltre -o _rdiag_probe_systre.exe 2>&1
if (Test-Path _rdiag_probe_systre.exe) {
    Write-Host "  link: OK -- running..."
    & .\_rdiag_probe_systre.exe
    Write-Host "  exit code: $LASTEXITCODE"
} else {
    Write-Host "  link: FAILED"
}

Write-Host "=== probe: compile+link WITH -lregex ==="
gcc _rdiag_probe.c -lregex -o _rdiag_probe_regex.exe 2>&1
if (Test-Path _rdiag_probe_regex.exe) {
    Write-Host "  link: OK -- running..."
    & .\_rdiag_probe_regex.exe
    Write-Host "  exit code: $LASTEXITCODE"
} else {
    Write-Host "  link: FAILED"
}

Remove-Item -Force _rdiag_hdr.c,_rdiag_probe.c,_rdiag_probe_noextra.exe,_rdiag_probe_systre.exe,_rdiag_probe_regex.exe -ErrorAction SilentlyContinue
Write-Host "=== diag done ==="
