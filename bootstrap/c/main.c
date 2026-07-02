#define _TR_MAIN
#include "tauraro_types.h"


__attribute__((hot)) void print_version() {
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("tauraroc v0.0.7"))); printf("\n"); });
}

__attribute__((hot)) void print_usage() {
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("Usage: tauraroc <file.tr> [options]"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("       tauraroc fmt [-w] <file.tr>   Format source (stdout, or -w in place)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("       tauraroc lint <file.tr>       Analyze and report warnings/errors"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("Options:"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --version         Print version and exit"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --emit c          Emit generated C code to build/"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --emit ast        Emit AST representation and stop"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --emit mir        Emit MIR basic blocks and stop"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --run             Compile and immediately execute"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --check           Run semantic analysis only (no codegen)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --verbose         Show all pipeline phases"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --backend llvm    Use LLVM IR backend instead of C"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  -o <path>         Output executable name (temp .c files are deleted)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --lib             Build a shared library (.so/.dll) of `export def`s + a C header"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  -O0/-O1/-O2/-O3  Optimization level (default: -O2)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  -Os               Optimize for size"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --link <path>     Link a file by path (.c .o .a .dll .lib .so)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  -l<name>          Link a library by name (e.g. -luser32, -lgdi32)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  -l <name>         Same as -l<name> with a space"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --static          Statically link the output binary"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --target <name>   Cross-compile for a target platform:"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                      android-arm64, android-arm32, android-x86_64, android-x86"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                      ios, ios-sim"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                      linux-arm64, linux-arm32, linux-x86_64, linux-riscv64"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                      windows-x64, windows-arm64"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                      macos-arm64, macos-x86_64"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                      embedded-arm, embedded-arm64, embedded-riscv32, embedded-riscv64"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                      wasm, wasm-wasi"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                    Or pass a raw LLVM triple (e.g. aarch64-linux-gnu)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --sysroot <path>  Override sysroot for the cross-compiler"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --debug           Compile with ASAN and bounds-check assertions"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --strict          Treat alloc/dealloc outside 'unsafe:' as a hard error [U-1]"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("Bilingual: all English keywords have Hausa equivalents (aiki=def, aji=class, ...)"))); printf("\n"); });
}

__attribute__((hot)) bool str_ends_with_dot_tr(TrStr path) {
    /* pass */
    char* p = ((char*)(_tr_strz(path)));
    /* pass */
    long long len = 0LL;
    /* pass */
    while ((((long long)((*(p + len)))) != 0LL)) {
        /* pass */
        len = (len + 1LL);
    }
    /* pass */
    if ((len < 3LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((long long)((*(p + (len - 3LL))))) != 46LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((long long)((*(p + (len - 2LL))))) != 116LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((long long)((*(p + (len - 1LL))))) != 114LL)) {
        /* pass */
        return false;
    }
    /* pass */
    return true;
}

__attribute__((hot)) TrStr strip_extension(TrStr path) {
    /* pass */
    char* p = ((char*)(_tr_strz(path)));
    /* pass */
    long long len = 0LL;
    /* pass */
    while ((((long long)((*(p + len)))) != 0LL)) {
        /* pass */
        len = (len + 1LL);
    }
    /* pass */
    long long end = len;
    /* pass */
    while ((end > 0LL)) {
        /* pass */
        if ((((long long)((*(p + (end - 1LL))))) == 46LL)) {
            /* pass */
            return _tr_str_wrap(_tr_str_slice(_tr_strz(path), 0LL, (end - 1LL)));
        }
        /* pass */
        end = (end - 1LL);
    }
    /* pass */
    return _tr_str_retain(path);
}

__attribute__((hot)) bool str_starts_with(TrStr s, TrStr prefix) {
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    char* pp = ((char*)(_tr_strz(prefix)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long pc = ((long long)((*(pp + i))));
        /* pass */
        if ((pc == 0LL)) {
            /* pass */
            return true;
        }
        /* pass */
        long long sc = ((long long)((*(sp + i))));
        /* pass */
        if ((sc != pc)) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) TrStr detect_c_compiler() {
    /* pass */
    TrStr null_dev = _tr_str_lit("/dev/null");
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t2346 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t2346;
    }
    /* pass */
    if (({ TrStr _aet_t2347 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("gcc --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t2347.data) == 0LL)); _tr_str_release(_aet_t2347); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("gcc");
    }
    /* pass */
    if (({ TrStr _aet_t2348 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("clang --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t2348.data) == 0LL)); _tr_str_release(_aet_t2348); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("clang");
    }
    /* pass */
    if (({ TrStr _aet_t2349 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("cc --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t2349.data) == 0LL)); _tr_str_release(_aet_t2349); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("cc");
    }
    /* pass */
    _tr_str_release(null_dev);
    return _tr_str_lit("gcc");
}

__attribute__((hot)) bool is_clang_compiler(TrStr cc) {
    /* pass */
    TrStr null_dev = _tr_str_lit("/dev/null");
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t2350 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t2350;
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(cc), _tr_strz(_tr_str_lit("clang")))) {
        /* pass */
        _tr_str_release(null_dev);
        return true;
    }
    /* pass */
    if (_tr_str_eq(_tr_strz(cc), _tr_strz(_tr_str_lit("cc")))) {
        /* pass */
        _tr_str_release(null_dev);
        return ({ TrStr _aet_t2351 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" --version 2>&1 | grep -qi clang")))); __auto_type _wr = ((_tr_system(_aet_t2351.data) == 0LL)); _tr_str_release(_aet_t2351); _wr; });
    }
    /* pass */
    _tr_str_release(null_dev);
    return false;
}

__attribute__((hot)) TrStr resolve_target_triple(TrStr target) {
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("android-arm64"))) == 0)) {
        /* pass */
        return _tr_str_lit("aarch64-linux-android34");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("android-arm32"))) == 0)) {
        /* pass */
        return _tr_str_lit("armv7a-linux-androideabi34");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("android-x86_64"))) == 0)) {
        /* pass */
        return _tr_str_lit("x86_64-linux-android34");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("android-x86"))) == 0)) {
        /* pass */
        return _tr_str_lit("i686-linux-android34");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("ios"))) == 0)) {
        /* pass */
        return _tr_str_lit("aarch64-apple-ios");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("ios-sim"))) == 0)) {
        /* pass */
        return _tr_str_lit("aarch64-apple-ios-simulator");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("linux-arm64"))) == 0)) {
        /* pass */
        return _tr_str_lit("aarch64-linux-gnu");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("linux-arm32"))) == 0)) {
        /* pass */
        return _tr_str_lit("armv7-linux-gnueabihf");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("linux-x86_64"))) == 0)) {
        /* pass */
        return _tr_str_lit("x86_64-linux-gnu");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("linux-riscv64"))) == 0)) {
        /* pass */
        return _tr_str_lit("riscv64-linux-gnu");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("windows-x64"))) == 0)) {
        /* pass */
        return _tr_str_lit("x86_64-w64-mingw32");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("windows-arm64"))) == 0)) {
        /* pass */
        return _tr_str_lit("aarch64-w64-mingw32");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("macos-arm64"))) == 0)) {
        /* pass */
        return _tr_str_lit("aarch64-apple-macosx12.0");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("macos-x86_64"))) == 0)) {
        /* pass */
        return _tr_str_lit("x86_64-apple-macosx12.0");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("embedded-arm"))) == 0)) {
        /* pass */
        return _tr_str_lit("arm-none-eabi");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("embedded-arm64"))) == 0)) {
        /* pass */
        return _tr_str_lit("aarch64-none-elf");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("embedded-riscv32"))) == 0)) {
        /* pass */
        return _tr_str_lit("riscv32-unknown-elf");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("embedded-riscv64"))) == 0)) {
        /* pass */
        return _tr_str_lit("riscv64-unknown-elf");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("wasm"))) == 0)) {
        /* pass */
        return _tr_str_lit("wasm32-unknown-unknown");
    }
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("wasm-wasi"))) == 0)) {
        /* pass */
        return _tr_str_lit("wasm32-wasi");
    }
    /* pass */
    return _tr_str_retain(target);
}

__attribute__((hot)) TrStr target_extra_flags(TrStr triple) {
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("none-eabi")))) {
        /* pass */
        return _tr_str_lit(" -nostdlib -freestanding -ffreestanding -DTAURARO_NO_OS=1");
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("none-elf")))) {
        /* pass */
        return _tr_str_lit(" -nostdlib -freestanding -ffreestanding -DTAURARO_NO_OS=1");
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("unknown-elf")))) {
        /* pass */
        return _tr_str_lit(" -nostdlib -freestanding -ffreestanding -DTAURARO_NO_OS=1");
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("wasm32-unknown-unknown")))) {
        /* pass */
        return _tr_str_lit(" -nostdlib --no-standard-libraries -DTAURARO_WASM=1 -DTAURARO_NO_OS=1");
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("wasm32-wasi")))) {
        /* pass */
        return _tr_str_lit(" -DTAURARO_WASM=1");
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("mingw")))) {
        /* pass */
        return _tr_str_lit(" -static");
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr detect_cross_compiler(TrStr triple) {
    /* pass */
    TrStr null_dev = _tr_str_lit("/dev/null");
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t2352 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t2352;
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("android")))) {
        /* pass */
        TrStr ndk = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("ANDROID_NDK_ROOT"))));
        /* pass */
        if ((strcmp(_tr_strz(ndk), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t2353 = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("ANDROID_NDK_HOME"))));
            _tr_str_release(ndk);
            ndk = _strtmp_t2353;
        }
        /* pass */
        if ((strcmp(_tr_strz(ndk), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t2354 = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("NDK_HOME"))));
            _tr_str_release(ndk);
            ndk = _strtmp_t2354;
        }
        /* pass */
        if ((strcmp(_tr_strz(ndk), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr wrapper = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(ndk), _tr_strz(_tr_str_lit("/toolchains/llvm/prebuilt/linux-x86_64/bin/")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(triple)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("-clang"))); _tr_str_release(_cl); _cres; });
            /* pass */
            if (({ TrStr _aet_t2355 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _tr_strz(wrapper))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" --version >"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(null_dev)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t2355.data) == 0LL)); _tr_str_release(_aet_t2355); _wr; })) {
                /* pass */
                _tr_str_release(null_dev);
                _tr_str_release(ndk);
                return wrapper;
            }
        }
    }
    /* pass */
    if (({ TrStr _aet_t2356 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("clang --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t2356.data) == 0LL)); _tr_str_release(_aet_t2356); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("clang");
    }
    /* pass */
    _tr_str_release(null_dev);
    return detect_c_compiler();
}

__attribute__((hot)) TrStr dir_of_path(TrStr path) {
    /* pass */
    char* p = ((char*)(_tr_strz(path)));
    /* pass */
    long long len = 0LL;
    /* pass */
    while ((((long long)((*(p + len)))) != 0LL)) {
        /* pass */
        len = (len + 1LL);
    }
    /* pass */
    long long end = len;
    /* pass */
    while ((end > 0LL)) {
        /* pass */
        long long c = ((long long)((*(p + (end - 1LL)))));
        /* pass */
        if (((c == 47LL) || (c == 92LL))) {
            /* pass */
            return _tr_str_wrap(_tr_str_slice(_tr_strz(path), 0LL, end));
        }
        /* pass */
        end = (end - 1LL);
    }
    /* pass */
    return _tr_str_lit("./");
}

__attribute__((hot)) TrStr strip_trailing_sep_inline(TrStr s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    long long last = ((long long)((*(p + (n - 1LL)))));
    /* pass */
    if (((last == 47LL) || (last == 92LL))) {
        /* pass */
        return _tr_str_wrap(_tr_str_slice(_tr_strz(s), 0LL, (n - 1LL)));
    }
    /* pass */
    return _tr_str_retain(s);
}

__attribute__((hot)) TrStr read_runtime_header(TrStr bin_path, TrStr input_path) {
    /* pass */
    if (file_exists(_tr_str_lit("tauraro/runtime/tauraro_rt.h"))) {
        /* pass */
        return read_file(_tr_str_lit("tauraro/runtime/tauraro_rt.h"));
    }
    /* pass */
    TrStr bin_dir = ({ TrStr _cl = (_tr_str_wrap(_tr_exe_dir())); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/"))); _tr_str_release(_cl); _cres; });
    /* pass */
    TrStr src1 = _tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")));
    /* pass */
    if (file_exists(src1)) {
        /* pass */
        _tr_str_release(bin_dir);
        return read_file(src1);
    }
    /* pass */
    TrStr src1b = _tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")));
    /* pass */
    if (file_exists(src1b)) {
        /* pass */
        _tr_str_release(bin_dir);
        _tr_str_release(src1);
        return read_file(src1b);
    }
    /* pass */
    if ((strcmp(_tr_strz(input_path), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr src_dir = dir_of_path(input_path);
        /* pass */
        if (({ TrStr _at_t2357 = (_tr_strx_concat(_tr_strz(src_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t2357)); _tr_str_release(_at_t2357); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            return ({ TrStr _at_t2358 = (_tr_strx_concat(_tr_strz(src_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t2358)); _tr_str_release(_at_t2358); _wr; });
        }
        /* pass */
        TrStr parent = ({ TrStr _at_t2359 = (strip_trailing_sep_inline(src_dir)); __auto_type _wr = (dir_of_path(_at_t2359)); _tr_str_release(_at_t2359); _wr; });
        /* pass */
        if (({ TrStr _at_t2360 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t2360)); _tr_str_release(_at_t2360); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            return ({ TrStr _at_t2361 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t2361)); _tr_str_release(_at_t2361); _wr; });
        }
        /* pass */
        if (({ TrStr _at_t2362 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t2362)); _tr_str_release(_at_t2362); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            return ({ TrStr _at_t2363 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t2363)); _tr_str_release(_at_t2363); _wr; });
        }
        /* pass */
        TrStr gp = ({ TrStr _at_t2364 = (strip_trailing_sep_inline(parent)); __auto_type _wr = (dir_of_path(_at_t2364)); _tr_str_release(_at_t2364); _wr; });
        /* pass */
        if (({ TrStr _at_t2365 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t2365)); _tr_str_release(_at_t2365); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            _tr_str_release(parent);
            return ({ TrStr _at_t2366 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t2366)); _tr_str_release(_at_t2366); _wr; });
        }
        /* pass */
        if (({ TrStr _at_t2367 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t2367)); _tr_str_release(_at_t2367); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            _tr_str_release(parent);
            return ({ TrStr _at_t2368 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t2368)); _tr_str_release(_at_t2368); _wr; });
        }
        _tr_str_release(src_dir);
        _tr_str_release(parent);
    }
    /* pass */
    if (file_exists(_tr_str_lit("runtime/tauraro_rt.h"))) {
        /* pass */
        _tr_str_release(bin_dir);
        _tr_str_release(src1);
        _tr_str_release(src1b);
        return read_file(_tr_str_lit("runtime/tauraro_rt.h"));
    }
    /* pass */
    _tr_str_release(bin_dir);
    _tr_str_release(src1);
    _tr_str_release(src1b);
    return _tr_str_lit("");
}

__attribute__((hot)) void ensure_runtime_header(TrStr out_dir, TrStr bin_path, TrStr input_path) {
    /* pass */
    TrStr dest = _tr_strx_concat(_tr_strz(out_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")));
    /* pass */
    if (file_exists(dest)) {
        /* pass */
        _tr_str_release(dest);
        return;
    }
    /* pass */
    TrStr content = read_runtime_header(bin_path, input_path);
    /* pass */
    if ((strcmp(_tr_strz(content), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        write_file(dest, content);
    }
    _tr_str_release(dest);
    _tr_str_release(content);
}

__attribute__((hot)) void sync_headers_to_runtime(TrStr rt_content, TrStr types_content) {
    /* pass */
    write_file(_tr_str_lit("tauraro/runtime/tauraro_rt.h"), rt_content);
}

__attribute__((hot)) TrStr strip_trailing_sep(TrStr s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    long long last = ((long long)((*(p + (n - 1LL)))));
    /* pass */
    if (((last == 47LL) || (last == 92LL))) {
        /* pass */
        return _tr_str_wrap(_tr_str_slice(_tr_strz(s), 0LL, (n - 1LL)));
    }
    /* pass */
    return _tr_str_retain(s);
}

__attribute__((hot)) long long count_path_env_entries(TrStr s) {
    /* pass */
    long long sep = 58LL;
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        sep = 59LL;
    }
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long n = 0LL;
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    long long _tr_v_count = 0LL;
    /* pass */
    long long start = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i <= n)) {
        /* pass */
        long long c = 0LL;
        /* pass */
        if ((i < n)) {
            /* pass */
            c = ((long long)((*(p + i))));
        }
        /* pass */
        if (((c == sep) || (i == n))) {
            /* pass */
            if ((i > start)) {
                /* pass */
                _tr_v_count = (_tr_v_count + 1LL);
            }
            /* pass */
            start = (i + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_v_count;
}

__attribute__((hot)) TrStr get_path_env_entry(TrStr s, long long idx) {
    /* pass */
    long long sep = 58LL;
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        sep = 59LL;
    }
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long n = 0LL;
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    long long cur = 0LL;
    /* pass */
    long long start = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i <= n)) {
        /* pass */
        long long c = 0LL;
        /* pass */
        if ((i < n)) {
            /* pass */
            c = ((long long)((*(p + i))));
        }
        /* pass */
        if (((c == sep) || (i == n))) {
            /* pass */
            if ((i > start)) {
                /* pass */
                if ((cur == idx)) {
                    /* pass */
                    return _tr_str_wrap(_tr_str_slice(_tr_strz(s), start, i));
                }
                /* pass */
                cur = (cur + 1LL);
            }
            /* pass */
            start = (i + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr to_runnable_path(TrStr s) {
    /* pass */
    if ((_tr_str_contains(_tr_strz(s), _tr_strz(_tr_str_lit("/"))) || _tr_str_contains(_tr_strz(s), _tr_strz(_tr_str_lit("\\"))))) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(_tr_str_lit("./")), _tr_strz(s));
}

__attribute__((hot)) TrStr path_to_native(TrStr s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    TrStr buf = _tr_str_wrap(_tr_str_slice(_tr_strz(s), 0LL, n));
    /* pass */
    char* bp = ((char*)(_tr_strz(buf)));
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        if ((((long long)((*(bp + j)))) == 47LL)) {
            /* pass */
            /* unsafe block */
            /* pass */
            (*(bp + j) = ((char)(92LL)));
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    return buf;
}

__attribute__((hot)) TrStr to_fwd_slashes(TrStr s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    TrStr buf = _tr_str_wrap(_tr_str_slice(_tr_strz(s), 0LL, n));
    /* pass */
    char* bp = ((char*)(_tr_strz(buf)));
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        if ((((long long)((*(bp + j)))) == 92LL)) {
            /* pass */
            /* unsafe block */
            /* pass */
            (*(bp + j) = ((char)(47LL)));
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    return buf;
}

__attribute__((hot)) TrStr dot_to_safe(TrStr s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    TrStr buf = _tr_str_wrap(_tr_str_slice(_tr_strz(s), 0LL, n));
    /* pass */
    char* bp = ((char*)(_tr_strz(buf)));
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        if ((((long long)((*(bp + j)))) == 46LL)) {
            /* pass */
            /* unsafe block */
            /* pass */
            (*(bp + j) = ((char)(95LL)));
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    return buf;
}

__attribute__((hot)) TrStr dot_last_seg(TrStr s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    long long last_dot = (-1LL);
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        if ((((long long)((*(p + j)))) == 46LL)) {
            /* pass */
            last_dot = j;
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    if ((last_dot < 0LL)) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(s), (last_dot + 1LL), n));
}

__attribute__((hot)) TrStr get_filename(TrStr path) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(path)));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    long long last_sep = (-1LL);
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        long long c = ((long long)((*(p + j))));
        /* pass */
        if (((c == 47LL) || (c == 92LL))) {
            /* pass */
            last_sep = j;
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    if ((last_sep < 0LL)) {
        /* pass */
        return _tr_str_retain(path);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(path), (last_sep + 1LL), n));
}

__attribute__((hot)) long long get_dot_depth(TrStr dot_path) {
    /* pass */
    char* p = ((char*)(_tr_strz(dot_path)));
    /* pass */
    long long dots = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        if ((c == 46LL)) {
            /* pass */
            dots = (dots + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (dots + 1LL);
}

__attribute__((hot)) TrStr ensure_builtin_dirs(TrStr build_dir, TrStr dot_path) {
    /* pass */
    TrStr current = _tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("include/")));
    /* pass */
    make_dir(current);
    /* pass */
    char* p = ((char*)(_tr_strz(dot_path)));
    /* pass */
    long long start = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 0LL)) {
            /* pass */
            TrStr last_seg = _tr_str_wrap(_tr_str_slice(_tr_strz(dot_path), start, i));
            /* pass */
            return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(current), _tr_strz(last_seg))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".c"))); _tr_str_release(_cl); _cres; });
        }
        /* pass */
        if ((c == 46LL)) {
            /* pass */
            TrStr seg = _tr_str_wrap(_tr_str_slice(_tr_strz(dot_path), start, i));
            /* pass */
            TrStr _strtmp_t2369 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(current), _tr_strz(seg))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(current);
            current = _strtmp_t2369;
            /* pass */
            make_dir(current);
            /* pass */
            start = (i + 1LL);
            _tr_str_release(seg);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    _tr_str_release(current);
    return _tr_str_lit("");
}

__attribute__((hot)) bool is_builtin_mod(TrStr dot_path) {
    /* pass */
    char* p = ((char*)(_tr_strz(dot_path)));
    /* pass */
    bool is_core = true;
    /* pass */
    if ((((long long)((*(p + 0LL)))) != 99LL)) {
        /* pass */
        is_core = false;
    }
    /* pass */
    if ((((long long)((*(p + 1LL)))) != 111LL)) {
        /* pass */
        is_core = false;
    }
    /* pass */
    if ((((long long)((*(p + 2LL)))) != 114LL)) {
        /* pass */
        is_core = false;
    }
    /* pass */
    if ((((long long)((*(p + 3LL)))) != 101LL)) {
        /* pass */
        is_core = false;
    }
    /* pass */
    if (is_core) {
        /* pass */
        long long c5 = ((long long)((*(p + 4LL))));
        /* pass */
        if (((c5 == 0LL) || (c5 == 46LL))) {
            /* pass */
            return true;
        }
    }
    /* pass */
    bool is_std = true;
    /* pass */
    if ((((long long)((*(p + 0LL)))) != 115LL)) {
        /* pass */
        is_std = false;
    }
    /* pass */
    if ((((long long)((*(p + 1LL)))) != 116LL)) {
        /* pass */
        is_std = false;
    }
    /* pass */
    if ((((long long)((*(p + 2LL)))) != 100LL)) {
        /* pass */
        is_std = false;
    }
    /* pass */
    if (is_std) {
        /* pass */
        long long c4 = ((long long)((*(p + 3LL))));
        /* pass */
        if (((c4 == 0LL) || (c4 == 46LL))) {
            /* pass */
            return true;
        }
    }
    /* pass */
    return false;
}

__attribute__((hot)) void make_dir(TrStr path) {
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        ({ TrStr _aet_t2370 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(path)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("mkdir \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2370.data); _tr_str_release(_aet_t2370); });
    } else {
        /* pass */
        ({ TrStr _aet_t2371 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("mkdir -p \"")), _tr_strz(path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>/dev/null"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2371.data); _tr_str_release(_aet_t2371); });
    }
}

__attribute__((hot)) long long compile_all_c(List_TrStr* c_files, TrStr exe_path, TrStr inc_dir, List_TrStr* link_paths, List_TrStr* lib_flags, TrStr opt_level, bool verbose, bool static_link, TrStr target, TrStr sysroot, bool debug_mode) {
    /* pass */
    TrStr cc = detect_c_compiler();
    /* pass */
    TrStr triple = _tr_str_lit("");
    /* pass */
    TrStr cross_flags = _tr_str_lit("");
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t2372 = resolve_target_triple(target);
        _tr_str_release(triple);
        triple = _strtmp_t2372;
        /* pass */
        TrStr _strtmp_t2373 = detect_cross_compiler(triple);
        _tr_str_release(cc);
        cc = _strtmp_t2373;
        /* pass */
        TrStr _strtmp_t2374 = _tr_strx_concat(_tr_strz(_tr_str_lit(" --target=")), _tr_strz(triple));
        _tr_str_release(cross_flags);
        cross_flags = _strtmp_t2374;
        /* pass */
        TrStr _strtmp_t2375 = ({ TrStr _cr = (target_extra_flags(triple)); TrStr _cres = _tr_strx_concat(_tr_strz(cross_flags), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(cross_flags);
        cross_flags = _strtmp_t2375;
        /* pass */
        if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t2376 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cross_flags), _tr_strz(_tr_str_lit(" --sysroot=\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
            _tr_str_release(cross_flags);
            cross_flags = _strtmp_t2376;
        }
    }
    /* pass */
    TrStr static_flag = _tr_str_lit("");
    /* pass */
    if (static_link) {
        /* pass */
        TrStr _strtmp_t2377 = _tr_str_lit(" -static");
        _tr_str_release(static_flag);
        static_flag = _strtmp_t2377;
    }
    /* pass */
    TrStr warn_flags = _tr_str_lit(" -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value");
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        TrStr _strtmp_t2378 = _tr_strx_concat(_tr_strz(warn_flags), _tr_strz(_tr_str_lit(" -Wno-unknown-attributes -Wno-parentheses-equality")));
        _tr_str_release(warn_flags);
        warn_flags = _strtmp_t2378;
    }
    /* pass */
    TrStr native_flags = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) == 0))) {
        /* pass */
        TrStr _strtmp_t2379 = _tr_str_lit(" -march=native -funroll-loops");
        _tr_str_release(native_flags);
        native_flags = _strtmp_t2379;
    }
    /* pass */
    TrStr overflow_flag = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) != 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("s"))) != 0))) {
        /* pass */
        TrStr _strtmp_t2380 = _tr_str_lit(" -ftrapv");
        _tr_str_release(overflow_flag);
        overflow_flag = _strtmp_t2380;
    }
    /* pass */
    TrStr cmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -O")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(opt_level)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(overflow_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(static_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(native_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cross_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(warn_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -DTAURARO_NO_RT_HELPERS \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(inc_dir)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c_files->len)) {
        /* pass */
        TrStr _strtmp_t2381 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(c_files, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t2381;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < link_paths->len)) {
        /* pass */
        TrStr _strtmp_t2382 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(link_paths, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t2382;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < lib_flags->len)) {
        /* pass */
        TrStr _strtmp_t2383 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (List_TrStr_get(lib_flags, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t2383;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr _strtmp_t2384 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lm")));
    _tr_str_release(cmd);
    cmd = _strtmp_t2384;
    /* pass */
    if (debug_mode) {
        /* pass */
        TrStr _strtmp_t2385 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -fsanitize=address,undefined -g")));
        _tr_str_release(cmd);
        cmd = _strtmp_t2385;
    }
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        bool need_wsa = false;
        /* pass */
        long long wi = 0LL;
        /* pass */
        while ((wi < c_files->len)) {
            /* pass */
            TrStr cf = List_TrStr_get(c_files, wi);
            /* pass */
            if ((_tr_str_contains(_tr_strz(cf), _tr_strz(_tr_str_lit("tcp.c"))) || _tr_str_contains(_tr_strz(cf), _tr_strz(_tr_str_lit("/net/"))))) {
                /* pass */
                need_wsa = true;
            }
            /* pass */
            wi = (wi + 1LL);
            _tr_str_release(cf);
        }
        /* pass */
        if (need_wsa) {
            /* pass */
            TrStr _strtmp_t2386 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lws2_32 -mconsole")));
            _tr_str_release(cmd);
            cmd = _strtmp_t2386;
        }
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("  [CC] ")), _tr_strz(cmd)))); printf("\n"); });
    }
    /* pass */
    _tr_str_release(cc);
    _tr_str_release(triple);
    _tr_str_release(cross_flags);
    _tr_str_release(static_flag);
    _tr_str_release(warn_flags);
    _tr_str_release(native_flags);
    _tr_str_release(overflow_flag);
    return _tr_system(_tr_strz(cmd));
}

__attribute__((hot)) TrStr obj_path_for(TrStr c_path) {
    /* pass */
    long long n = _tr_strlen(_tr_strz(c_path));
    /* pass */
    if ((n > 2LL)) {
        /* pass */
        TrStr tail = _tr_str_wrap(_tr_str_slice(_tr_strz(c_path), (n - 2LL), n));
        /* pass */
        if ((strcmp(_tr_strz(tail), _tr_strz(_tr_str_lit(".c"))) == 0)) {
            /* pass */
            _tr_str_release(tail);
            return ({ TrStr _cl = (_tr_str_wrap(_tr_str_slice(_tr_strz(c_path), 0LL, (n - 2LL)))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".o"))); _tr_str_release(_cl); _cres; });
        }
        _tr_str_release(tail);
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(c_path), _tr_strz(_tr_str_lit(".o")));
}

__attribute__((hot)) long long compile_all_c_incremental(List_TrStr* c_files, List_bool* needs, TrStr exe_path, TrStr inc_dir, List_TrStr* link_paths, List_TrStr* lib_flags, TrStr opt_level, bool verbose, bool static_link, TrStr target, TrStr sysroot, bool debug_mode, bool build_shared) {
    /* pass */
    TrStr cc = detect_c_compiler();
    /* pass */
    TrStr triple = _tr_str_lit("");
    /* pass */
    TrStr cross_flags = _tr_str_lit("");
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t2387 = resolve_target_triple(target);
        _tr_str_release(triple);
        triple = _strtmp_t2387;
        /* pass */
        TrStr _strtmp_t2388 = detect_cross_compiler(triple);
        _tr_str_release(cc);
        cc = _strtmp_t2388;
        /* pass */
        TrStr _strtmp_t2389 = _tr_strx_concat(_tr_strz(_tr_str_lit(" --target=")), _tr_strz(triple));
        _tr_str_release(cross_flags);
        cross_flags = _strtmp_t2389;
        /* pass */
        TrStr _strtmp_t2390 = ({ TrStr _cr = (target_extra_flags(triple)); TrStr _cres = _tr_strx_concat(_tr_strz(cross_flags), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(cross_flags);
        cross_flags = _strtmp_t2390;
        /* pass */
        if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t2391 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cross_flags), _tr_strz(_tr_str_lit(" --sysroot=\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
            _tr_str_release(cross_flags);
            cross_flags = _strtmp_t2391;
        }
    }
    /* pass */
    TrStr static_flag = _tr_str_lit("");
    /* pass */
    if (static_link) {
        /* pass */
        TrStr _strtmp_t2392 = _tr_str_lit(" -static");
        _tr_str_release(static_flag);
        static_flag = _strtmp_t2392;
    }
    /* pass */
    TrStr warn_flags = _tr_str_lit(" -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value");
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        TrStr _strtmp_t2393 = _tr_strx_concat(_tr_strz(warn_flags), _tr_strz(_tr_str_lit(" -Wno-unknown-attributes -Wno-parentheses-equality")));
        _tr_str_release(warn_flags);
        warn_flags = _strtmp_t2393;
    }
    /* pass */
    TrStr native_flags = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) == 0))) {
        /* pass */
        TrStr _strtmp_t2394 = _tr_str_lit(" -march=native -funroll-loops");
        _tr_str_release(native_flags);
        native_flags = _strtmp_t2394;
    }
    /* pass */
    TrStr overflow_flag = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) != 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("s"))) != 0))) {
        /* pass */
        TrStr _strtmp_t2395 = _tr_str_lit(" -ftrapv");
        _tr_str_release(overflow_flag);
        overflow_flag = _strtmp_t2395;
    }
    /* pass */
    TrStr dbg = _tr_str_lit("");
    /* pass */
    if (debug_mode) {
        /* pass */
        TrStr _strtmp_t2396 = _tr_str_lit(" -fsanitize=address,undefined -g");
        _tr_str_release(dbg);
        dbg = _strtmp_t2396;
    }
    /* pass */
    TrStr pic = _tr_str_lit("");
    /* pass */
    if (build_shared) {
        /* pass */
        TrStr _strtmp_t2397 = _tr_str_lit(" -fPIC");
        _tr_str_release(pic);
        pic = _strtmp_t2397;
    }
    /* pass */
    TrStr common = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit(" -O")), _tr_strz(opt_level))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(overflow_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(static_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(native_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cross_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(warn_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(dbg)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pic)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -DTAURARO_NO_RT_HELPERS \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(inc_dir)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    /* pass */
    List_TrStr* o_files = (void*)List_TrStr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    long long compiled = 0LL;
    /* pass */
    while ((i < c_files->len)) {
        /* pass */
        TrStr cpath = List_TrStr_get(c_files, i);
        /* pass */
        TrStr opath = obj_path_for(cpath);
        /* pass */
        List_TrStr_append(o_files, opath);
        /* pass */
        if ((List_bool_get(needs, i) || (!file_exists(opath)))) {
            /* pass */
            TrStr ccmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(common))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -c \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cpath)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(opath)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
            /* pass */
            if (verbose) {
                /* pass */
                ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("  [CC -c] ")), _tr_strz(cpath)))); printf("\n"); });
            }
            /* pass */
            long long crc = _tr_system(_tr_strz(ccmd));
            /* pass */
            if ((crc != 0LL)) {
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": compiling "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cpath)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" failed (exit code "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(crc)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                /* pass */
                _tr_str_release(cc);
                _tr_str_release(triple);
                _tr_str_release(cross_flags);
                _tr_str_release(static_flag);
                _tr_str_release(warn_flags);
                _tr_str_release(native_flags);
                _tr_str_release(overflow_flag);
                _tr_str_release(dbg);
                _tr_str_release(pic);
                _tr_str_release(common);
                List_TrStr_free(o_files);
                _tr_str_release(cpath);
                _tr_str_release(opath);
                _tr_str_release(ccmd);
                return crc;
            }
            _tr_str_release(ccmd);
        } else {
            /* pass */
            compiled = (compiled + 1LL);
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(cpath);
        _tr_str_release(opath);
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(compiled)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  [incremental] reused ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" of "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(c_files->len)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" cached object(s)"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    }
    /* pass */
    TrStr shared_flag = _tr_str_lit("");
    /* pass */
    if (build_shared) {
        /* pass */
        TrStr _strtmp_t2398 = _tr_str_lit(" -shared");
        _tr_str_release(shared_flag);
        shared_flag = _strtmp_t2398;
    }
    /* pass */
    TrStr cmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(common))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shared_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < o_files->len)) {
        /* pass */
        TrStr _strtmp_t2399 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(o_files, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t2399;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < link_paths->len)) {
        /* pass */
        TrStr _strtmp_t2400 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(link_paths, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t2400;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < lib_flags->len)) {
        /* pass */
        TrStr _strtmp_t2401 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (List_TrStr_get(lib_flags, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t2401;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr _strtmp_t2402 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lm")));
    _tr_str_release(cmd);
    cmd = _strtmp_t2402;
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t2403 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lws2_32 -mconsole")));
        _tr_str_release(cmd);
        cmd = _strtmp_t2403;
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("  [LINK] ")), _tr_strz(cmd)))); printf("\n"); });
    }
    /* pass */
    _tr_str_release(cc);
    _tr_str_release(triple);
    _tr_str_release(cross_flags);
    _tr_str_release(static_flag);
    _tr_str_release(warn_flags);
    _tr_str_release(native_flags);
    _tr_str_release(overflow_flag);
    _tr_str_release(dbg);
    _tr_str_release(pic);
    _tr_str_release(common);
    List_TrStr_free(o_files);
    _tr_str_release(shared_flag);
    return _tr_system(_tr_strz(cmd));
}

__attribute__((hot)) long long compile_c_to_exe(TrStr c_path, TrStr exe_path, TrStr opt_level, bool verbose) {
    /* pass */
    TrStr cc = detect_c_compiler();
    /* pass */
    TrStr opt_flag = _tr_strx_concat(_tr_strz(_tr_str_lit("-O")), _tr_strz(opt_level));
    /* pass */
    TrStr out_dir = ({ TrStr _at_t2404 = (dir_of_path(c_path)); __auto_type _wr = (strip_trailing_sep(_at_t2404)); _tr_str_release(_at_t2404); _wr; });
    /* pass */
    TrStr warn_flags = _tr_str_lit(" -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value");
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        TrStr _strtmp_t2405 = _tr_strx_concat(_tr_strz(warn_flags), _tr_strz(_tr_str_lit(" -Wno-unknown-attributes -Wno-parentheses-equality")));
        _tr_str_release(warn_flags);
        warn_flags = _strtmp_t2405;
    }
    /* pass */
    TrStr cmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(opt_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(warn_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -DTAURARO_NO_RT_HELPERS \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(out_dir)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -lm"))); _tr_str_release(_cl); _cres; });
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("  [CC] ")), _tr_strz(cmd)))); printf("\n"); });
    }
    /* pass */
    _tr_str_release(cc);
    _tr_str_release(opt_flag);
    _tr_str_release(out_dir);
    _tr_str_release(warn_flags);
    return _tr_system(_tr_strz(cmd));
}

__attribute__((hot)) void _print_diag(TrStr level, TrStr msg) {
    /* pass */
    TrStr fix_part = _tr_str_lit("");
    /* pass */
    TrStr main_part = _tr_str_retain(msg);
    /* pass */
    long long fix_idx = _tr_str_index_of(_tr_strz(msg), _tr_strz(_tr_str_lit("FIX:")));
    /* pass */
    if ((fix_idx >= 0LL)) {
        /* pass */
        TrStr _strtmp_t2407 = ({ TrStr _wt_t2406 = (_tr_str_wrap(_tr_str_slice(_tr_strz(msg), 0LL, fix_idx))); __auto_type _wr = (_tr_str_wrap(_tr_str_strip(_wt_t2406.data))); _tr_str_release(_wt_t2406); _wr; });
        _tr_str_release(main_part);
        main_part = _strtmp_t2407;
        /* pass */
        TrStr _strtmp_t2409 = ({ TrStr _wt_t2408 = (_tr_str_wrap(_tr_str_slice(_tr_strz(msg), (fix_idx + 4LL), _tr_strlen(_tr_strz(msg))))); __auto_type _wr = (_tr_str_wrap(_tr_str_strip(_wt_t2408.data))); _tr_str_release(_wt_t2408); _wr; });
        _tr_str_release(fix_part);
        fix_part = _strtmp_t2409;
    }
    /* pass */
    TrStr loc_str = _tr_str_lit("");
    /* pass */
    TrStr file_path = _tr_str_lit("");
    /* pass */
    long long line_no = 0LL;
    /* pass */
    TrStr body = _tr_str_retain(main_part);
    /* pass */
    long long pe = _tr_str_index_of(_tr_strz(main_part), _tr_strz(_tr_str_lit(": ")));
    /* pass */
    if ((pe > 0LL)) {
        /* pass */
        TrStr head = _tr_str_wrap(_tr_str_slice(_tr_strz(main_part), 0LL, pe));
        /* pass */
        long long ln = loc_line(head);
        /* pass */
        if ((ln > 0LL)) {
            /* pass */
            TrStr _strtmp_t2410 = _tr_str_retain(head);
            _tr_str_release(loc_str);
            loc_str = _strtmp_t2410;
            /* pass */
            TrStr _strtmp_t2411 = loc_file(head);
            _tr_str_release(file_path);
            file_path = _strtmp_t2411;
            /* pass */
            line_no = ln;
            /* pass */
            TrStr _strtmp_t2412 = _tr_str_wrap(_tr_str_slice(_tr_strz(main_part), (pe + 2LL), _tr_strlen(_tr_strz(main_part))));
            _tr_str_release(body);
            body = _strtmp_t2412;
        }
    }
    /* pass */
    TrStr lvl = c_red(level);
    /* pass */
    if ((strcmp(_tr_strz(level), _tr_strz(_tr_str_lit("warning"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2413 = c_yellow(level);
        _tr_str_release(lvl);
        lvl = _strtmp_t2413;
    }
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(lvl), _tr_strz(_tr_str_lit(": ")))); TrStr _cr = (c_bold(body)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; }))); printf("\n"); });
    /* pass */
    if ((_tr_strlen(_tr_strz(loc_str)) > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (c_cyan(_tr_str_lit("-->"))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(loc_str)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        TrStr src = _tr_str_lit("");
        /* pass */
        if ((_tr_strlen(_tr_strz(file_path)) > 0LL)) {
            /* pass */
            TrStr _strtmp_t2414 = read_file(file_path);
            _tr_str_release(src);
            src = _strtmp_t2414;
        }
        /* pass */
        if ((_tr_strlen(_tr_strz(src)) > 0LL)) {
            /* pass */
            TrStr srcline = _nth_source_line(src, line_no);
            /* pass */
            if ((_tr_strlen(_tr_strz(srcline)) > 0LL)) {
                /* pass */
                TrStr gnum = ({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(line_no)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("   ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" | "))); _tr_str_release(_cl); _cres; });
                /* pass */
                TrStr gbar = ({ TrStr _cl = (spaces((_tr_strlen(_tr_strz(gnum)) - 2LL))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("| "))); _tr_str_release(_cl); _cres; });
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (c_dim(gnum)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(srcline)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                /* pass */
                TrStr name = first_quoted(msg);
                /* pass */
                long long col = col_of(srcline, name);
                /* pass */
                if ((col > 0LL)) {
                    /* pass */
                    TrStr underline = repeat_char(_tr_str_lit("^"), _tr_strlen(_tr_strz(name)));
                    /* pass */
                    TrStr caret = c_red(underline);
                    /* pass */
                    if ((strcmp(_tr_strz(level), _tr_strz(_tr_str_lit("warning"))) == 0)) {
                        /* pass */
                        TrStr _strtmp_t2415 = c_yellow(underline);
                        _tr_str_release(caret);
                        caret = _strtmp_t2415;
                    }
                    /* pass */
                    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (c_dim(gbar)); TrStr _cr = (spaces((col - 1LL))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(caret)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                    _tr_str_release(caret);
                }
                _tr_str_release(gnum);
            }
        }
    }
    /* pass */
    if ((_tr_strlen(_tr_strz(fix_part)) > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (c_green(_tr_str_lit("= help"))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fix_part)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    }
    _tr_str_release(fix_part);
    _tr_str_release(main_part);
    _tr_str_release(loc_str);
    _tr_str_release(file_path);
    _tr_str_release(body);
    _tr_str_release(lvl);
}

__attribute__((hot)) void cleanup_build(TrStr build_dir, List_TrStr* all_c_files) {
    /* pass */
    long long di = 0LL;
    /* pass */
    while ((di < all_c_files->len)) {
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            ({ TrStr _at_t2416 = (List_TrStr_get(all_c_files, di)); TrStr _aet_t2417 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t2416)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("del /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2417.data); _tr_str_release(_at_t2416); _tr_str_release(_aet_t2417); });
        } else {
            /* pass */
            ({ TrStr _aet_t2418 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(all_c_files, di)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("rm -f \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2418.data); _tr_str_release(_aet_t2418); });
        }
        /* pass */
        di = (di + 1LL);
    }
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        ({ TrStr _at_t2419 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_types.h")))); TrStr _aet_t2420 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t2419)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("del /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2420.data); _tr_str_release(_at_t2419); _tr_str_release(_aet_t2420); });
        /* pass */
        ({ TrStr _at_t2421 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); TrStr _aet_t2422 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t2421)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("del /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2422.data); _tr_str_release(_at_t2421); _tr_str_release(_aet_t2422); });
        /* pass */
        ({ TrStr _at_t2423 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("include")))); TrStr _aet_t2424 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t2423)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("rmdir /S /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2424.data); _tr_str_release(_at_t2423); _tr_str_release(_aet_t2424); });
        /* pass */
        ({ TrStr _aet_t2425 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(build_dir)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("rmdir \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2425.data); _tr_str_release(_aet_t2425); });
    } else {
        /* pass */
        ({ TrStr _aet_t2426 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -f \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("tauraro_types.h\""))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2426.data); _tr_str_release(_aet_t2426); });
        /* pass */
        ({ TrStr _aet_t2427 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -f \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("tauraro_rt.h\""))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2427.data); _tr_str_release(_aet_t2427); });
        /* pass */
        ({ TrStr _aet_t2428 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -rf \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("include\""))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2428.data); _tr_str_release(_aet_t2428); });
        /* pass */
        ({ TrStr _aet_t2429 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rmdir \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>/dev/null"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t2429.data); _tr_str_release(_aet_t2429); });
    }
}

__attribute__((hot)) void run_fmt(TrStr path0, bool write_in_place) {
    /* pass */
    TrStr path = _tr_str_retain(path0);
    /* pass */
    if (((!file_exists(path)) && (!str_ends_with_dot_tr(path)))) {
        /* pass */
        TrStr _strtmp_t2430 = _tr_strx_concat(_tr_strz(path), _tr_strz(_tr_str_lit(".tr")));
        _tr_str_release(path);
        path = _strtmp_t2430;
    }
    /* pass */
    TrStr source = read_file(path);
    /* pass */
    if ((strcmp(_tr_strz(source), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("error: cannot read ")), _tr_strz(path)))); printf("\n"); });
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    Lexer* lexer = Lexer_init(source);
    /* pass */
    lexer->record_comments = true;
    /* pass */
    List_Token* tokens = Lexer_tokenize(lexer);
    /* pass */
    Parser* parser = Parser_init(tokens, lexer->token_lines);
    /* pass */
    parser->current_file = _tr_str_retain(path);
    /* pass */
    parser->cols = lexer->token_cols;
    /* pass */
    parser->src_text = _tr_str_retain(source);
    /* pass */
    Program* prog = Parser_parse_program(parser);
    /* pass */
    if ((parser->error_count > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(parser->error_count)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("error: ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" parse error(s); not formatting "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(path)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    Formatter* f = Formatter_init(lexer->comment_lines, lexer->comment_texts, lexer->comment_trailing);
    /* pass */
    TrStr formatted = Formatter_format_program(f, prog);
    /* pass */
    if (f->unsupported) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("error: ")), _tr_strz(path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" uses a construct the formatter does not yet support; leaving it unchanged"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (write_in_place) {
        /* pass */
        write_file(path, formatted);
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("formatted ")), _tr_strz(path)))); printf("\n"); });
    } else {
        /* pass */
        _tr_print_raw(_tr_strz(formatted));
    }
    _tr_str_release(path);
    _tr_str_release(source);
    _tr_obj_release(lexer, _trdrop_Lexer);
    _tr_obj_release(parser, _trdrop_Parser);
    _tr_obj_release(f, _trdrop_Formatter);
    _tr_str_release(formatted);
}

__attribute__((hot)) int main(int argc, char** argv) {
    _tr_argc = argc; _tr_argv = argv;
    _tr_init_console();
    List_TrStr* args = List_TrStr_new();
    for (int _ai = 0; _ai < argc; _ai++) { List_TrStr_append(args, _tr_str_lit(argv[_ai])); }
    /* pass */
    if ((args->len < 2LL)) {
        /* pass */
        print_usage();
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (((args->len == 2LL) && (strcmp(_tr_strz(List_TrStr_get(args, 1LL)), _tr_strz(_tr_str_lit("--version"))) == 0))) {
        /* pass */
        print_version();
        /* pass */
        exit((int)(0LL));
    }
    /* pass */
    TrStr subcmd = _tr_str_lit("");
    /* pass */
    if ((args->len >= 2LL)) {
        /* pass */
        TrStr a1 = List_TrStr_get(args, 1LL);
        /* pass */
        if (((strcmp(_tr_strz(a1), _tr_strz(_tr_str_lit("fmt"))) == 0) || (strcmp(_tr_strz(a1), _tr_strz(_tr_str_lit("lint"))) == 0))) {
            /* pass */
            TrStr _strtmp_t2431 = _tr_str_retain(a1);
            _tr_str_release(subcmd);
            subcmd = _strtmp_t2431;
        }
    }
    /* pass */
    if ((strcmp(_tr_strz(subcmd), _tr_strz(_tr_str_lit("fmt"))) == 0)) {
        /* pass */
        bool write_in_place = false;
        /* pass */
        TrStr fpath = _tr_str_lit("");
        /* pass */
        long long fk = 2LL;
        /* pass */
        while ((fk < args->len)) {
            /* pass */
            TrStr fa = List_TrStr_get(args, fk);
            /* pass */
            if (((strcmp(_tr_strz(fa), _tr_strz(_tr_str_lit("-w"))) == 0) || (strcmp(_tr_strz(fa), _tr_strz(_tr_str_lit("--write"))) == 0))) {
                /* pass */
                write_in_place = true;
            } else if ((!str_starts_with(fa, _tr_str_lit("-")))) {
                /* pass */
                if ((strcmp(_tr_strz(fpath), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t2432 = _tr_str_retain(fa);
                    _tr_str_release(fpath);
                    fpath = _strtmp_t2432;
                }
            }
            /* pass */
            fk = (fk + 1LL);
            _tr_str_release(fa);
        }
        /* pass */
        if ((strcmp(_tr_strz(fpath), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("error: fmt requires a file: tauraroc fmt [-w] <file.tr>"))); printf("\n"); });
            /* pass */
            exit((int)(1LL));
        }
        /* pass */
        run_fmt(fpath, write_in_place);
        /* pass */
        exit((int)(0LL));
    }
    /* pass */
    TrStr input_path = _tr_str_lit("");
    /* pass */
    TrStr output_path = _tr_str_lit("");
    /* pass */
    TrStr backend = _tr_str_lit("c");
    /* pass */
    TrStr emit_mode = _tr_str_lit("exe");
    /* pass */
    bool run_after = false;
    /* pass */
    bool check_only = false;
    /* pass */
    bool verbose = false;
    /* pass */
    TrStr opt_level = _tr_str_lit("2");
    /* pass */
    List_TrStr* link_paths = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* lib_flags = (void*)List_TrStr_new();
    /* pass */
    bool static_link = false;
    /* pass */
    TrStr target = _tr_str_lit("");
    /* pass */
    TrStr sysroot = _tr_str_lit("");
    /* pass */
    bool debug_mode = false;
    /* pass */
    bool strict_mode = false;
    /* pass */
    bool no_elide = false;
    /* pass */
    bool lib_mode = false;
    /* pass */
    long long i = 1LL;
    /* pass */
    if ((strcmp(_tr_strz(subcmd), _tr_strz(_tr_str_lit("lint"))) == 0)) {
        /* pass */
        check_only = true;
        /* pass */
        i = 2LL;
    }
    /* pass */
    while ((i < args->len)) {
        /* pass */
        TrStr arg = List_TrStr_get(args, i);
        /* pass */
        if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--version"))) == 0)) {
            /* pass */
            print_version();
            /* pass */
            exit((int)(0LL));
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--run"))) == 0)) {
            /* pass */
            run_after = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--check"))) == 0)) {
            /* pass */
            check_only = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--verbose"))) == 0)) {
            /* pass */
            verbose = true;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--emit"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t2433 = List_TrStr_get(args, i);
            _tr_str_release(emit_mode);
            emit_mode = _strtmp_t2433;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--backend"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t2434 = List_TrStr_get(args, i);
            _tr_str_release(backend);
            backend = _strtmp_t2434;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-o"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t2435 = List_TrStr_get(args, i);
            _tr_str_release(output_path);
            output_path = _strtmp_t2435;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--link"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t2436 = (List_TrStr_get(args, i)); List_TrStr_append(link_paths, _at_t2436); _tr_str_release(_at_t2436); });
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-l"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t2437 = (({ TrStr _cr = (List_TrStr_get(args, i)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("-l")), _cr.data); _tr_str_release(_cr); _cres; })); List_TrStr_append(lib_flags, _at_t2437); _tr_str_release(_at_t2437); });
        } else if ((str_starts_with(arg, _tr_str_lit("-l")) && (strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-l"))) != 0))) {
            /* pass */
            List_TrStr_append(lib_flags, arg);
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O0"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2438 = _tr_str_lit("0");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t2438;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O1"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2439 = _tr_str_lit("1");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t2439;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O2"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2440 = _tr_str_lit("2");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t2440;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O3"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2441 = _tr_str_lit("3");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t2441;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-Os"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2442 = _tr_str_lit("s");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t2442;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--static"))) == 0)) {
            /* pass */
            static_link = true;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--target"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t2443 = List_TrStr_get(args, i);
            _tr_str_release(target);
            target = _strtmp_t2443;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--sysroot"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t2444 = List_TrStr_get(args, i);
            _tr_str_release(sysroot);
            sysroot = _strtmp_t2444;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--debug"))) == 0)) {
            /* pass */
            debug_mode = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--strict"))) == 0)) {
            /* pass */
            strict_mode = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--no-elide"))) == 0)) {
            /* pass */
            no_elide = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--lib"))) == 0)) {
            /* pass */
            lib_mode = true;
        } else if ((!str_starts_with(arg, _tr_str_lit("-")))) {
            /* pass */
            if ((strcmp(_tr_strz(input_path), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t2445 = _tr_str_retain(arg);
                _tr_str_release(input_path);
                input_path = _strtmp_t2445;
            }
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(arg);
    }
    /* pass */
    if ((strcmp(_tr_strz(input_path), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": no input file specified"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        print_usage();
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (((!file_exists(input_path)) && (!str_ends_with_dot_tr(input_path)))) {
        /* pass */
        TrStr _strtmp_t2446 = _tr_strx_concat(_tr_strz(input_path), _tr_strz(_tr_str_lit(".tr")));
        _tr_str_release(input_path);
        input_path = _strtmp_t2446;
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[1/5] Resolving modules: ")), _tr_strz(input_path)))); printf("\n"); });
    }
    /* pass */
    ModuleResolver* resolver = ModuleResolver_init();
    /* pass */
    TrStr bin_dir = ({ TrStr _at_t2447 = (_tr_str_wrap(_tr_exe_dir())); __auto_type _wr = (strip_trailing_sep_inline(_at_t2447)); _tr_str_release(_at_t2447); _wr; });
    /* pass */
    ModuleResolver_add_search_path(resolver, bin_dir);
    /* pass */
    ({ TrStr _at_t2448 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("/std")))); ModuleResolver_add_search_path(resolver, _at_t2448); _tr_str_release(_at_t2448); });
    /* pass */
    ({ TrStr _at_t2449 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("/packages")))); ModuleResolver_add_search_path(resolver, _at_t2449); _tr_str_release(_at_t2449); });
    /* pass */
    ({ TrStr _at_t2450 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("/packages/sites")))); ModuleResolver_add_search_path(resolver, _at_t2450); _tr_str_release(_at_t2450); });
    /* pass */
    ModuleResolver_add_search_path(resolver, _tr_str_lit("packages"));
    /* pass */
    ModuleResolver_add_search_path(resolver, _tr_str_lit("packages/sites"));
    /* pass */
    TrStr tauraro_path_env = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("TAURARO_PATH"))));
    /* pass */
    if ((strcmp(_tr_strz(tauraro_path_env), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        long long ep_count = count_path_env_entries(tauraro_path_env);
        /* pass */
        long long epi = 0LL;
        /* pass */
        while ((epi < ep_count)) {
            /* pass */
            ({ TrStr _at_t2451 = (get_path_env_entry(tauraro_path_env, epi)); ModuleResolver_add_search_path(resolver, _at_t2451); _tr_str_release(_at_t2451); });
            /* pass */
            epi = (epi + 1LL);
        }
    }
    /* pass */
    Program* prog = ModuleResolver_resolve_main(resolver, input_path);
    /* pass */
    if ((resolver->parse_errors > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(resolver->parse_errors)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" parse error(s); aborting compilation."))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    long long macro_errs = expand_macros(prog);
    /* pass */
    if ((macro_errs > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(macro_errs)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" macro error(s); aborting compilation."))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if ((strcmp(_tr_strz(emit_mode), _tr_strz(_tr_str_lit("ast"))) == 0)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(prog->decls->len)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[AST] Declarations found: ")), _cr.data); _tr_str_release(_cr); _cres; }))); printf("\n"); });
        /* pass */
        _tr_str_release(subcmd);
        _tr_str_release(input_path);
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        return 0;
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("[2/5] Semantic analysis..."))); printf("\n"); });
    }
    /* pass */
    Sema* sema = Sema_init();
    /* pass */
    sema->strict_mode = strict_mode;
    /* pass */
    sema->current_file = _tr_str_retain(input_path);
    /* pass */
    HirProgram* hir = Sema_analyze(sema, prog);
    /* pass */
    if ((sema->warnings->len > 0LL)) {
        /* pass */
        long long wk = 0LL;
        /* pass */
        while ((wk < sema->warnings->len)) {
            /* pass */
            ({ TrStr _at_t2452 = (List_TrStr_get(sema->warnings, wk)); _print_diag(_tr_str_lit("warning"), _at_t2452); _tr_str_release(_at_t2452); });
            /* pass */
            wk = (wk + 1LL);
        }
    }
    /* pass */
    if ((sema->errors->len > 0LL)) {
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < sema->errors->len)) {
            /* pass */
            ({ TrStr _at_t2453 = (List_TrStr_get(sema->errors, k)); _print_diag(_tr_str_lit("error"), _at_t2453); _tr_str_release(_at_t2453); });
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (check_only) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("Check passed: no errors found."))); printf("\n"); });
        /* pass */
        _tr_str_release(subcmd);
        _tr_str_release(input_path);
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(sema, _trdrop_Sema);
        return 0;
    }
    /* pass */
    if ((strcmp(_tr_strz(emit_mode), _tr_strz(_tr_str_lit("mir"))) == 0)) {
        /* pass */
        MirProgram* mir_prog = lower_program(hir);
        /* pass */
        ({ printf("%s", _tr_strz(dump_mir(mir_prog))); printf("\n"); });
        /* pass */
        _tr_str_release(subcmd);
        _tr_str_release(input_path);
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(sema, _trdrop_Sema);
        return 0;
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[3/5] Code generation (backend=")), _tr_strz(backend))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")..."))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    }
    /* pass */
    if ((strcmp(_tr_strz(backend), _tr_strz(_tr_str_lit("llvm"))) == 0)) {
        /* pass */
        LlvmGenerator* llvm_gen = LlvmGenerator_init();
        /* pass */
        TrStr llvm_ir = LlvmGenerator_generate(llvm_gen, hir);
        /* pass */
        if ((strcmp(_tr_strz(output_path), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t2454 = ({ TrStr _cl = (strip_extension(input_path)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".ll"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(output_path);
            output_path = _strtmp_t2454;
        }
        /* pass */
        write_file(output_path, llvm_ir);
        /* pass */
        if (verbose) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[4/5] LLVM IR written to ")), _tr_strz(output_path)))); printf("\n"); });
        }
        /* pass */
        _tr_str_release(subcmd);
        _tr_str_release(input_path);
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(sema, _trdrop_Sema);
        _tr_obj_release(llvm_gen, _trdrop_LlvmGenerator);
        _tr_str_release(llvm_ir);
        return 0;
    }
    /* pass */
    CGenerator* c_gen = CGenerator_init();
    /* pass */
    c_gen->emit_line_info = debug_mode;
    /* pass */
    c_gen->no_elide = no_elide;
    /* pass */
    TrStr rt_h = ({ TrStr _at_t2455 = (List_TrStr_get(args, 0LL)); __auto_type _wr = (read_runtime_header(_at_t2455, input_path)); _tr_str_release(_at_t2455); _wr; });
    /* pass */
    CGenerator_register_program(c_gen, hir);
    /* pass */
    CGenerator_scan_mono_prog(c_gen, hir);
    /* pass */
    TrStr build_dir = _tr_str_lit("build/");
    /* pass */
    make_dir(build_dir);
    /* pass */
    TrStr types_h = CGenerator_generate_types_header(c_gen, hir);
    /* pass */
    TrStr _strtmp_t2456 = ({ TrStr _cr = (CGenerator_generate_module_compat(c_gen, resolver->all_decl_modules, resolver->all_decls)); TrStr _cres = _tr_strx_concat(_tr_strz(types_h), _cr.data); _tr_str_release(_cr); _cres; });
    _tr_str_release(types_h);
    types_h = _strtmp_t2456;
    /* pass */
    bool force_all = false;
    /* pass */
    TrStr types_path = _tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_types.h")));
    /* pass */
    if (file_exists(types_path)) {
        /* pass */
        if ((strcmp(_tr_strz(read_file(types_path)), _tr_strz(types_h)) != 0)) {
            /* pass */
            force_all = true;
        }
    } else {
        /* pass */
        force_all = true;
    }
    /* pass */
    if ((strcmp(_tr_strz(rt_h), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr rt_path = _tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")));
        /* pass */
        if (file_exists(rt_path)) {
            /* pass */
            if ((strcmp(_tr_strz(read_file(rt_path)), _tr_strz(rt_h)) != 0)) {
                /* pass */
                force_all = true;
            }
        } else {
            /* pass */
            force_all = true;
        }
    }
    /* pass */
    TrStr flags_sig = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("opt=")), _tr_strz(opt_level))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";tgt="))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(target)); _tr_str_release(_cl); _cres; });
    /* pass */
    if (static_link) {
        /* pass */
        TrStr _strtmp_t2457 = _tr_strx_concat(_tr_strz(flags_sig), _tr_strz(_tr_str_lit(";static")));
        _tr_str_release(flags_sig);
        flags_sig = _strtmp_t2457;
    }
    /* pass */
    if (debug_mode) {
        /* pass */
        TrStr _strtmp_t2458 = _tr_strx_concat(_tr_strz(flags_sig), _tr_strz(_tr_str_lit(";debug")));
        _tr_str_release(flags_sig);
        flags_sig = _strtmp_t2458;
    }
    /* pass */
    if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t2459 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(flags_sig), _tr_strz(_tr_str_lit(";sysroot=")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; });
        _tr_str_release(flags_sig);
        flags_sig = _strtmp_t2459;
    }
    /* pass */
    TrStr flags_path = _tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit(".build_flags")));
    /* pass */
    if (file_exists(flags_path)) {
        /* pass */
        if ((strcmp(_tr_strz(read_file(flags_path)), _tr_strz(flags_sig)) != 0)) {
            /* pass */
            force_all = true;
        }
    } else {
        /* pass */
        force_all = true;
    }
    /* pass */
    write_file(flags_path, flags_sig);
    /* pass */
    if ((strcmp(_tr_strz(rt_h), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _at_t2460 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); write_file(_at_t2460, rt_h); _tr_str_release(_at_t2460); });
    }
    /* pass */
    write_file(types_path, types_h);
    /* pass */
    if ((strcmp(_tr_strz(rt_h), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        sync_headers_to_runtime(rt_h, types_h);
    }
    /* pass */
    List_TrStr* all_c_files = (void*)List_TrStr_new();
    /* pass */
    List_bool* needs_recompile = (void*)List_bool_new();
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < resolver->mod_dot_paths->len)) {
        /* pass */
        TrStr dot_path = List_TrStr_get(resolver->mod_dot_paths, mi);
        /* pass */
        TrMap* class_set = _tr_dict_new(16LL);
        /* pass */
        TrMap* fn_set = _tr_dict_new(32LL);
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < resolver->all_decl_modules->len)) {
            /* pass */
            if ((strcmp(_tr_strz(List_TrStr_get(resolver->all_decl_modules, k)), _tr_strz(dot_path)) == 0)) {
                /* pass */
                __auto_type _t2461 = (*((Decl*)List_ptr_get(resolver->all_decls, k)));
                if (_t2461.tag == Decl_DClass) {
                    __auto_type c = _t2461.data.DClass.cls;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(c->name), true);
                } else if (_t2461.tag == Decl_DFunction) {
                    __auto_type f = _t2461.data.DFunction.func;
                    /* pass */
                    _tr_dict_set(fn_set, _tr_strz(f->name), true);
                    _tr_obj_release(f, _trdrop_FunctionDef);
                } else if (_t2461.tag == Decl_DEnum) {
                    __auto_type e = _t2461.data.DEnum.enm;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(e->name), true);
                } else if (_t2461.tag == Decl_DInterface) {
                    __auto_type iface = _t2461.data.DInterface.iface;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(iface->name), true);
                } else if (_t2461.tag == Decl_DExtend) {
                    __auto_type target = _t2461.data.DExtend.target;
__auto_type methods = _t2461.data.DExtend.methods;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(target), true);
                    _tr_str_release(target);
                } else if (1) {
                    __auto_type _ = _t2461;
                    /* pass */
                }
            }
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        TrStr c_path = _tr_str_lit("");
        /* pass */
        long long depth = 0LL;
        /* pass */
        if (is_builtin_mod(dot_path)) {
            /* pass */
            TrStr _strtmp_t2462 = ensure_builtin_dirs(build_dir, dot_path);
            _tr_str_release(c_path);
            c_path = _strtmp_t2462;
            /* pass */
            depth = get_dot_depth(dot_path);
        } else {
            /* pass */
            TrStr _strtmp_t2463 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("module_")))); TrStr _cr = (dot_to_safe(dot_path)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".c"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(c_path);
            c_path = _strtmp_t2463;
            /* pass */
            depth = 0LL;
        }
        /* pass */
        if ((c_gen->emit_line_info && (mi < resolver->mod_file_paths->len))) {
            /* pass */
            c_gen->cur_src_file = ({ TrStr _at_t2464 = (List_TrStr_get(resolver->mod_file_paths, mi)); __auto_type _wr = (to_fwd_slashes(_at_t2464)); _tr_str_release(_at_t2464); _wr; });
        }
        /* pass */
        TrStr mod_c = CGenerator_generate_module_c(c_gen, hir, class_set, fn_set, depth);
        /* pass */
        bool mod_changed = true;
        /* pass */
        if ((!force_all)) {
            /* pass */
            if (file_exists(c_path)) {
                /* pass */
                if ((strcmp(_tr_strz(read_file(c_path)), _tr_strz(mod_c)) == 0)) {
                    /* pass */
                    mod_changed = false;
                }
            }
        }
        /* pass */
        if (mod_changed) {
            /* pass */
            write_file(c_path, mod_c);
        }
        /* pass */
        List_TrStr_append(all_c_files, c_path);
        /* pass */
        List_bool_append(needs_recompile, mod_changed);
        /* pass */
        mi = (mi + 1LL);
        _tr_str_release(dot_path);
        _tr_str_release(c_path);
        _tr_str_release(mod_c);
    }
    /* pass */
    TrMap* main_class_set = _tr_dict_new(32LL);
    /* pass */
    TrMap* main_fn_set = _tr_dict_new(64LL);
    /* pass */
    long long k2 = 0LL;
    /* pass */
    while ((k2 < resolver->all_decl_modules->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(resolver->all_decl_modules, k2)), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            __auto_type _t2465 = (*((Decl*)List_ptr_get(resolver->all_decls, k2)));
            if (_t2465.tag == Decl_DClass) {
                __auto_type c = _t2465.data.DClass.cls;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(c->name), true);
            } else if (_t2465.tag == Decl_DFunction) {
                __auto_type f = _t2465.data.DFunction.func;
                /* pass */
                _tr_dict_set(main_fn_set, _tr_strz(f->name), true);
                _tr_obj_release(f, _trdrop_FunctionDef);
            } else if (_t2465.tag == Decl_DEnum) {
                __auto_type e = _t2465.data.DEnum.enm;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(e->name), true);
            } else if (_t2465.tag == Decl_DInterface) {
                __auto_type iface = _t2465.data.DInterface.iface;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(iface->name), true);
            } else if (_t2465.tag == Decl_DExtend) {
                __auto_type target = _t2465.data.DExtend.target;
__auto_type methods = _t2465.data.DExtend.methods;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(target), true);
                _tr_str_release(target);
            } else if (1) {
                __auto_type _ = _t2465;
                /* pass */
            }
        }
        /* pass */
        k2 = (k2 + 1LL);
    }
    /* pass */
    long long nci = 0LL;
    /* pass */
    while ((nci < sema->nested_classes->len)) {
        /* pass */
        _tr_dict_set(main_class_set, _tr_strz(((HirClass*)List_ptr_get(sema->nested_classes, nci))->name), true);
        /* pass */
        nci = (nci + 1LL);
    }
    /* pass */
    long long nfi = 0LL;
    /* pass */
    while ((nfi < sema->nested_functions->len)) {
        /* pass */
        _tr_dict_set(main_fn_set, _tr_strz(((HirFunction*)List_ptr_get(sema->nested_functions, nfi))->name), true);
        /* pass */
        nfi = (nfi + 1LL);
    }
    /* pass */
    long long nei = 0LL;
    /* pass */
    while ((nei < sema->nested_enums->len)) {
        /* pass */
        _tr_dict_set(main_class_set, _tr_strz(((HirEnum*)List_ptr_get(sema->nested_enums, nei))->name), true);
        /* pass */
        nei = (nei + 1LL);
    }
    /* pass */
    long long nii = 0LL;
    /* pass */
    while ((nii < sema->nested_interfaces->len)) {
        /* pass */
        _tr_dict_set(main_class_set, _tr_strz(((HirInterface*)List_ptr_get(sema->nested_interfaces, nii))->name), true);
        /* pass */
        nii = (nii + 1LL);
    }
    /* pass */
    if (c_gen->emit_line_info) {
        /* pass */
        c_gen->cur_src_file = to_fwd_slashes(input_path);
    }
    /* pass */
    TrStr main_c = CGenerator_generate_main_c(c_gen, hir, main_class_set, main_fn_set);
    /* pass */
    TrStr main_c_path = _tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("main.c")));
    /* pass */
    bool main_changed = true;
    /* pass */
    if ((!force_all)) {
        /* pass */
        if (file_exists(main_c_path)) {
            /* pass */
            if ((strcmp(_tr_strz(read_file(main_c_path)), _tr_strz(main_c)) == 0)) {
                /* pass */
                main_changed = false;
            }
        }
    }
    /* pass */
    if (main_changed) {
        /* pass */
        write_file(main_c_path, main_c);
    }
    /* pass */
    List_TrStr_append(all_c_files, main_c_path);
    /* pass */
    List_bool_append(needs_recompile, main_changed);
    /* pass */
    if ((strcmp(_tr_strz(emit_mode), _tr_strz(_tr_str_lit("c"))) == 0)) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("Modular C output written to: ")), _tr_strz(build_dir)))); printf("\n"); });
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("  tauraro_types.h  - shared type definitions + all function prototypes"))); printf("\n"); });
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("  tauraro_rt.h     - runtime header"))); printf("\n"); });
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("  main.c           - program entry"))); printf("\n"); });
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < (all_c_files->len - 1LL))) {
            /* pass */
            ({ TrStr _at_t2466 = (List_TrStr_get(all_c_files, pi)); ({ printf("%s", _tr_strz(({ TrStr _cr = (get_filename(_at_t2466)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _cr.data); _tr_str_release(_cr); _cres; }))); printf("\n"); }); _tr_str_release(_at_t2466); });
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        _tr_str_release(subcmd);
        _tr_str_release(input_path);
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(sema, _trdrop_Sema);
        _tr_obj_release(c_gen, _trdrop_CGenerator);
        _tr_str_release(rt_h);
        _tr_str_release(build_dir);
        _tr_str_release(types_h);
        _tr_str_release(types_path);
        _tr_str_release(flags_sig);
        _tr_str_release(flags_path);
        List_TrStr_free(all_c_files);
        List_bool_free(needs_recompile);
        _tr_str_release(main_c);
        _tr_str_release(main_c_path);
        return 0;
    }
    /* pass */
    TrStr exe_name = _tr_str_lit("");
    /* pass */
    if ((strcmp(_tr_strz(output_path), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t2467 = _tr_str_retain(output_path);
        _tr_str_release(exe_name);
        exe_name = _strtmp_t2467;
    } else {
        /* pass */
        TrStr _strtmp_t2469 = ({ TrStr _at_t2468 = (get_filename(input_path)); __auto_type _wr = (strip_extension(_at_t2468)); _tr_str_release(_at_t2468); _wr; });
        _tr_str_release(exe_name);
        exe_name = _strtmp_t2469;
    }
    /* pass */
    if ((strcmp(_tr_strz(exe_name), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        TrStr _strtmp_t2470 = _tr_str_lit("a");
        _tr_str_release(exe_name);
        exe_name = _strtmp_t2470;
    }
    /* pass */
    long long en_len = 0LL;
    /* pass */
    char* en_p = ((char*)(_tr_strz(exe_name)));
    /* pass */
    while ((((long long)((*(en_p + en_len)))) != 0LL)) {
        /* pass */
        en_len = (en_len + 1LL);
    }
    /* pass */
    if ((en_len > 4LL)) {
        /* pass */
        if ((((long long)((*(en_p + (en_len - 4LL))))) == 46LL)) {
            /* pass */
            if ((((long long)((*(en_p + (en_len - 3LL))))) == 101LL)) {
                /* pass */
                if ((((long long)((*(en_p + (en_len - 2LL))))) == 120LL)) {
                    /* pass */
                    if ((((long long)((*(en_p + (en_len - 1LL))))) == 101LL)) {
                        /* pass */
                        TrStr _strtmp_t2471 = _tr_str_wrap(_tr_str_slice(_tr_strz(exe_name), 0LL, (en_len - 4LL)));
                        _tr_str_release(exe_name);
                        exe_name = _strtmp_t2471;
                    }
                }
            }
        }
    }
    /* pass */
    TrStr exe_ext = _tr_str_lit(".exe");
    /* pass */
    if (lib_mode) {
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            TrStr _strtmp_t2472 = _tr_str_lit(".dll");
            _tr_str_release(exe_ext);
            exe_ext = _strtmp_t2472;
        } else {
            /* pass */
            TrStr _strtmp_t2473 = _tr_str_lit(".so");
            _tr_str_release(exe_ext);
            exe_ext = _strtmp_t2473;
        }
    } else if ((!_tr_is_windows())) {
        /* pass */
        TrStr _strtmp_t2474 = _tr_str_lit("");
        _tr_str_release(exe_ext);
        exe_ext = _strtmp_t2474;
    }
    /* pass */
    TrStr exe_path = _tr_str_lit("");
    /* pass */
    if ((strcmp(_tr_strz(output_path), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t2475 = _tr_strx_concat(_tr_strz(exe_name), _tr_strz(exe_ext));
        _tr_str_release(exe_path);
        exe_path = _strtmp_t2475;
    } else {
        /* pass */
        TrStr _strtmp_t2476 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(exe_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_ext)); _tr_str_release(_cl); _cres; });
        _tr_str_release(exe_path);
        exe_path = _strtmp_t2476;
    }
    /* pass */
    if (lib_mode) {
        /* pass */
        TrStr hdr = CGenerator_generate_export_header(c_gen, hir);
        /* pass */
        TrStr hdr_path = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(exe_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".h"))); _tr_str_release(_cl); _cres; });
        /* pass */
        if ((strcmp(_tr_strz(output_path), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t2477 = _tr_strx_concat(_tr_strz(exe_name), _tr_strz(_tr_str_lit(".h")));
            _tr_str_release(hdr_path);
            hdr_path = _strtmp_t2477;
        }
        /* pass */
        write_file(hdr_path, hdr);
        /* pass */
        if (verbose) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[lib] header: ")), _tr_strz(hdr_path)))); printf("\n"); });
        }
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(all_c_files->len)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[5/5] Compiling + linking ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" modules -> "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    }
    /* pass */
    long long rc = compile_all_c_incremental(all_c_files, needs_recompile, exe_path, build_dir, link_paths, lib_flags, opt_level, verbose, static_link, target, sysroot, debug_mode, lib_mode);
    /* pass */
    if ((rc != 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": compilation failed (exit code "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(rc)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        exit((int)(rc));
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("Done: ")), _tr_strz(exe_path)))); printf("\n"); });
    }
    /* pass */
    if (run_after) {
        /* pass */
        TrStr run_path = to_runnable_path(exe_path);
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            TrStr _strtmp_t2478 = path_to_native(run_path);
            _tr_str_release(run_path);
            run_path = _strtmp_t2478;
        }
        /* pass */
        long long run_rc = ({ TrStr _aet_t2479 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _tr_strz(run_path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_tr_system(_aet_t2479.data)); _tr_str_release(_aet_t2479); _wr; });
        /* pass */
        exit((int)(run_rc));
    }
    _tr_str_release(subcmd);
    _tr_str_release(input_path);
    _tr_str_release(output_path);
    _tr_str_release(backend);
    _tr_str_release(emit_mode);
    _tr_str_release(opt_level);
    _tr_str_release(target);
    _tr_str_release(sysroot);
    _tr_obj_release(resolver, _trdrop_ModuleResolver);
    _tr_str_release(bin_dir);
    _tr_str_release(tauraro_path_env);
    _tr_obj_release(sema, _trdrop_Sema);
    _tr_obj_release(c_gen, _trdrop_CGenerator);
    _tr_str_release(rt_h);
    _tr_str_release(build_dir);
    _tr_str_release(types_h);
    _tr_str_release(types_path);
    _tr_str_release(flags_sig);
    _tr_str_release(flags_path);
    _tr_str_release(main_c);
    _tr_str_release(main_c_path);
    _tr_str_release(exe_name);
    _tr_str_release(exe_ext);
    _tr_str_release(exe_path);
#ifndef TAURARO_BARE
    _tr_async_pool_shutdown();
#endif
    return 0;
}
