#define _TR_MAIN
#include "tauraro_types.h"


__attribute__((hot)) void print_version() {
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("tauraroc v0.0.5")));
}

__attribute__((hot)) void print_usage() {
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("Usage: tauraroc <file.tr> [options]")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("Options:")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --version         Print version and exit")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --emit c          Emit generated C code to build/")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --emit ast        Emit AST representation and stop")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --emit mir        Emit MIR basic blocks and stop")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --run             Compile and immediately execute")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --check           Run semantic analysis only (no codegen)")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --verbose         Show all pipeline phases")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --backend llvm    Use LLVM IR backend instead of C")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  -o <path>         Output executable name (temp .c files are deleted)")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  -O0/-O1/-O2/-O3  Optimization level (default: -O2)")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  -Os               Optimize for size")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --link <path>     Link a file by path (.c .o .a .dll .lib .so)")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  -l<name>          Link a library by name (e.g. -luser32, -lgdi32)")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  -l <name>         Same as -l<name> with a space")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --static          Statically link the output binary")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --target <name>   Cross-compile for a target platform:")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("                      android-arm64, android-arm32, android-x86_64, android-x86")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("                      ios, ios-sim")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("                      linux-arm64, linux-arm32, linux-x86_64, linux-riscv64")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("                      windows-x64, windows-arm64")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("                      macos-arm64, macos-x86_64")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("                      embedded-arm, embedded-arm64, embedded-riscv32, embedded-riscv64")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("                      wasm, wasm-wasi")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("                    Or pass a raw LLVM triple (e.g. aarch64-linux-gnu)")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --sysroot <path>  Override sysroot for the cross-compiler")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --debug           Compile with ASAN and bounds-check assertions")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("  --strict          Treat alloc/dealloc outside 'unsafe:' as a hard error [U-1]")));
    /* pass */
    printf("%s\n", _tr_strz(_tr_str_lit("Bilingual: all English keywords have Hausa equivalents (aiki=def, aji=class, ...)")));
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
        TrStr _strtmp_t1534 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t1534;
    }
    /* pass */
    if ((_tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("gcc --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; }))) == 0LL)) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("gcc");
    }
    /* pass */
    if ((_tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("clang --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; }))) == 0LL)) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("clang");
    }
    /* pass */
    if ((_tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("cc --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; }))) == 0LL)) {
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
        TrStr _strtmp_t1535 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t1535;
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
        return (_tr_system(_tr_strz(_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" --version 2>&1 | grep -qi clang"))))) == 0LL);
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
        TrStr _strtmp_t1536 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t1536;
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("android")))) {
        /* pass */
        TrStr ndk = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("ANDROID_NDK_ROOT"))));
        /* pass */
        if ((strcmp(_tr_strz(ndk), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t1537 = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("ANDROID_NDK_HOME"))));
            _tr_str_release(ndk);
            ndk = _strtmp_t1537;
        }
        /* pass */
        if ((strcmp(_tr_strz(ndk), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t1538 = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("NDK_HOME"))));
            _tr_str_release(ndk);
            ndk = _strtmp_t1538;
        }
        /* pass */
        if ((strcmp(_tr_strz(ndk), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr wrapper = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(ndk), _tr_strz(_tr_str_lit("/toolchains/llvm/prebuilt/linux-x86_64/bin/")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(triple)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("-clang"))); _tr_str_release(_cl); _cres; });
            /* pass */
            if ((_tr_system(_tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _tr_strz(wrapper))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" --version >"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(null_dev)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; }))) == 0LL)) {
                /* pass */
                _tr_str_release(null_dev);
                _tr_str_release(ndk);
                return wrapper;
            }
        }
    }
    /* pass */
    if ((_tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("clang --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; }))) == 0LL)) {
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
        if (({ TrStr _at_t1539 = (_tr_strx_concat(_tr_strz(src_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t1539)); _tr_str_release(_at_t1539); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            return ({ TrStr _at_t1540 = (_tr_strx_concat(_tr_strz(src_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t1540)); _tr_str_release(_at_t1540); _wr; });
        }
        /* pass */
        TrStr parent = ({ TrStr _at_t1541 = (strip_trailing_sep_inline(src_dir)); __auto_type _wr = (dir_of_path(_at_t1541)); _tr_str_release(_at_t1541); _wr; });
        /* pass */
        if (({ TrStr _at_t1542 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t1542)); _tr_str_release(_at_t1542); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            return ({ TrStr _at_t1543 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t1543)); _tr_str_release(_at_t1543); _wr; });
        }
        /* pass */
        if (({ TrStr _at_t1544 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t1544)); _tr_str_release(_at_t1544); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            return ({ TrStr _at_t1545 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t1545)); _tr_str_release(_at_t1545); _wr; });
        }
        /* pass */
        TrStr gp = ({ TrStr _at_t1546 = (strip_trailing_sep_inline(parent)); __auto_type _wr = (dir_of_path(_at_t1546)); _tr_str_release(_at_t1546); _wr; });
        /* pass */
        if (({ TrStr _at_t1547 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t1547)); _tr_str_release(_at_t1547); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            _tr_str_release(parent);
            return ({ TrStr _at_t1548 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t1548)); _tr_str_release(_at_t1548); _wr; });
        }
        /* pass */
        if (({ TrStr _at_t1549 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t1549)); _tr_str_release(_at_t1549); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            _tr_str_release(parent);
            return ({ TrStr _at_t1550 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t1550)); _tr_str_release(_at_t1550); _wr; });
        }
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
            TrStr _strtmp_t1551 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(current), _tr_strz(seg))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(current);
            current = _strtmp_t1551;
            /* pass */
            make_dir(current);
            /* pass */
            start = (i + 1LL);
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
        _tr_system(_tr_strz(({ TrStr _cl = (({ TrStr _cr = (path_to_native(path)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("mkdir \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })));
    } else {
        /* pass */
        _tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("mkdir -p \"")), _tr_strz(path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>/dev/null"))); _tr_str_release(_cl); _cres; })));
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
        TrStr _strtmp_t1552 = resolve_target_triple(target);
        _tr_str_release(triple);
        triple = _strtmp_t1552;
        /* pass */
        TrStr _strtmp_t1553 = detect_cross_compiler(triple);
        _tr_str_release(cc);
        cc = _strtmp_t1553;
        /* pass */
        TrStr _strtmp_t1554 = _tr_strx_concat(_tr_strz(_tr_str_lit(" --target=")), _tr_strz(triple));
        _tr_str_release(cross_flags);
        cross_flags = _strtmp_t1554;
        /* pass */
        TrStr _strtmp_t1555 = ({ TrStr _cr = (target_extra_flags(triple)); TrStr _cres = _tr_strx_concat(_tr_strz(cross_flags), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(cross_flags);
        cross_flags = _strtmp_t1555;
        /* pass */
        if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t1556 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cross_flags), _tr_strz(_tr_str_lit(" --sysroot=\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
            _tr_str_release(cross_flags);
            cross_flags = _strtmp_t1556;
        }
    }
    /* pass */
    TrStr static_flag = _tr_str_lit("");
    /* pass */
    if (static_link) {
        /* pass */
        TrStr _strtmp_t1557 = _tr_str_lit(" -static");
        _tr_str_release(static_flag);
        static_flag = _strtmp_t1557;
    }
    /* pass */
    TrStr warn_flags = _tr_str_lit(" -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value");
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        TrStr _strtmp_t1558 = _tr_strx_concat(_tr_strz(warn_flags), _tr_strz(_tr_str_lit(" -Wno-unknown-attributes -Wno-parentheses-equality")));
        _tr_str_release(warn_flags);
        warn_flags = _strtmp_t1558;
    }
    /* pass */
    TrStr native_flags = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) == 0))) {
        /* pass */
        TrStr _strtmp_t1559 = _tr_str_lit(" -march=native -funroll-loops");
        _tr_str_release(native_flags);
        native_flags = _strtmp_t1559;
    }
    /* pass */
    TrStr overflow_flag = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) != 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("s"))) != 0))) {
        /* pass */
        TrStr _strtmp_t1560 = _tr_str_lit(" -ftrapv");
        _tr_str_release(overflow_flag);
        overflow_flag = _strtmp_t1560;
    }
    /* pass */
    TrStr cmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -O")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(opt_level)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(overflow_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(static_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(native_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cross_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(warn_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -DTAURARO_NO_RT_HELPERS \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(inc_dir)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c_files->len)) {
        /* pass */
        TrStr _strtmp_t1561 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(c_files, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t1561;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < link_paths->len)) {
        /* pass */
        TrStr _strtmp_t1562 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(link_paths, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t1562;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < lib_flags->len)) {
        /* pass */
        TrStr _strtmp_t1563 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (List_TrStr_get(lib_flags, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t1563;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr _strtmp_t1564 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lm")));
    _tr_str_release(cmd);
    cmd = _strtmp_t1564;
    /* pass */
    if (debug_mode) {
        /* pass */
        TrStr _strtmp_t1565 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -fsanitize=address,undefined -g")));
        _tr_str_release(cmd);
        cmd = _strtmp_t1565;
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
            TrStr _strtmp_t1566 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lws2_32 -mconsole")));
            _tr_str_release(cmd);
            cmd = _strtmp_t1566;
        }
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("  [CC] ")), _tr_strz(cmd))));
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

__attribute__((hot)) long long compile_c_to_exe(TrStr c_path, TrStr exe_path, TrStr opt_level, bool verbose) {
    /* pass */
    TrStr cc = detect_c_compiler();
    /* pass */
    TrStr opt_flag = _tr_strx_concat(_tr_strz(_tr_str_lit("-O")), _tr_strz(opt_level));
    /* pass */
    TrStr out_dir = ({ TrStr _at_t1567 = (dir_of_path(c_path)); __auto_type _wr = (strip_trailing_sep(_at_t1567)); _tr_str_release(_at_t1567); _wr; });
    /* pass */
    TrStr warn_flags = _tr_str_lit(" -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value");
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        TrStr _strtmp_t1568 = _tr_strx_concat(_tr_strz(warn_flags), _tr_strz(_tr_str_lit(" -Wno-unknown-attributes -Wno-parentheses-equality")));
        _tr_str_release(warn_flags);
        warn_flags = _strtmp_t1568;
    }
    /* pass */
    TrStr cmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(opt_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(warn_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -DTAURARO_NO_RT_HELPERS \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(out_dir)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -lm"))); _tr_str_release(_cl); _cres; });
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("  [CC] ")), _tr_strz(cmd))));
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
        TrStr _mp = _tr_str_wrap(_tr_str_slice(_tr_strz(msg), 0LL, fix_idx));
        /* pass */
        TrStr _strtmp_t1569 = _tr_str_wrap(_tr_str_strip(_tr_strz(_mp)));
        _tr_str_release(main_part);
        main_part = _strtmp_t1569;
        /* pass */
        TrStr _fp = _tr_str_wrap(_tr_str_slice(_tr_strz(msg), fix_idx, _tr_strlen(_tr_strz(msg))));
        /* pass */
        TrStr _strtmp_t1570 = _tr_str_wrap(_tr_str_strip(_tr_strz(_fp)));
        _tr_str_release(fix_part);
        fix_part = _strtmp_t1570;
    }
    /* pass */
    printf("%s\n", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(level), _tr_strz(_tr_str_lit(": ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(main_part)); _tr_str_release(_cl); _cres; })));
    /* pass */
    if ((_tr_strlen(_tr_strz(fix_part)) > 0LL)) {
        /* pass */
        printf("%s\n", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("       ")), _tr_strz(fix_part))));
    }
    _tr_str_release(fix_part);
    _tr_str_release(main_part);
}

__attribute__((hot)) void cleanup_build(TrStr build_dir, List_TrStr* all_c_files) {
    /* pass */
    long long di = 0LL;
    /* pass */
    while ((di < all_c_files->len)) {
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            ({ TrStr _at_t1571 = (List_TrStr_get(all_c_files, di)); _tr_system(_tr_strz(({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t1571)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("del /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; }))); _tr_str_release(_at_t1571); });
        } else {
            /* pass */
            _tr_system(_tr_strz(({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(all_c_files, di)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("rm -f \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; })));
        }
        /* pass */
        di = (di + 1LL);
    }
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        ({ TrStr _at_t1572 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_types.h")))); _tr_system(_tr_strz(({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t1572)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("del /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; }))); _tr_str_release(_at_t1572); });
        /* pass */
        ({ TrStr _at_t1573 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); _tr_system(_tr_strz(({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t1573)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("del /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; }))); _tr_str_release(_at_t1573); });
        /* pass */
        ({ TrStr _at_t1574 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("include")))); _tr_system(_tr_strz(({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t1574)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("rmdir /S /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; }))); _tr_str_release(_at_t1574); });
        /* pass */
        _tr_system(_tr_strz(({ TrStr _cl = (({ TrStr _cr = (path_to_native(build_dir)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("rmdir \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })));
    } else {
        /* pass */
        _tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -f \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("tauraro_types.h\""))); _tr_str_release(_cl); _cres; })));
        /* pass */
        _tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -f \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("tauraro_rt.h\""))); _tr_str_release(_cl); _cres; })));
        /* pass */
        _tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -rf \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("include\""))); _tr_str_release(_cl); _cres; })));
        /* pass */
        _tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rmdir \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>/dev/null"))); _tr_str_release(_cl); _cres; })));
    }
}

__attribute__((hot)) int main(int argc, char** argv) {
    _tr_argc = argc; _tr_argv = argv;
    _tr_init_console();
#ifndef TAURARO_BARE
    _tr_global_async_pool = _tr_threadpool_auto();
#endif
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
    long long i = 1LL;
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
            TrStr _strtmp_t1575 = List_TrStr_get(args, i);
            _tr_str_release(emit_mode);
            emit_mode = _strtmp_t1575;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--backend"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t1576 = List_TrStr_get(args, i);
            _tr_str_release(backend);
            backend = _strtmp_t1576;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-o"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t1577 = List_TrStr_get(args, i);
            _tr_str_release(output_path);
            output_path = _strtmp_t1577;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--link"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t1578 = (List_TrStr_get(args, i)); List_TrStr_append(link_paths, _at_t1578); _tr_str_release(_at_t1578); });
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-l"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t1579 = (({ TrStr _cr = (List_TrStr_get(args, i)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("-l")), _cr.data); _tr_str_release(_cr); _cres; })); List_TrStr_append(lib_flags, _at_t1579); _tr_str_release(_at_t1579); });
        } else if ((str_starts_with(arg, _tr_str_lit("-l")) && (strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-l"))) != 0))) {
            /* pass */
            List_TrStr_append(lib_flags, arg);
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O0"))) == 0)) {
            /* pass */
            TrStr _strtmp_t1580 = _tr_str_lit("0");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t1580;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O1"))) == 0)) {
            /* pass */
            TrStr _strtmp_t1581 = _tr_str_lit("1");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t1581;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O2"))) == 0)) {
            /* pass */
            TrStr _strtmp_t1582 = _tr_str_lit("2");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t1582;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O3"))) == 0)) {
            /* pass */
            TrStr _strtmp_t1583 = _tr_str_lit("3");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t1583;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-Os"))) == 0)) {
            /* pass */
            TrStr _strtmp_t1584 = _tr_str_lit("s");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t1584;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--static"))) == 0)) {
            /* pass */
            static_link = true;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--target"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t1585 = List_TrStr_get(args, i);
            _tr_str_release(target);
            target = _strtmp_t1585;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--sysroot"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t1586 = List_TrStr_get(args, i);
            _tr_str_release(sysroot);
            sysroot = _strtmp_t1586;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--debug"))) == 0)) {
            /* pass */
            debug_mode = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--strict"))) == 0)) {
            /* pass */
            strict_mode = true;
        } else if ((!str_starts_with(arg, _tr_str_lit("-")))) {
            /* pass */
            if ((strcmp(_tr_strz(input_path), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t1587 = _tr_str_retain(arg);
                _tr_str_release(input_path);
                input_path = _strtmp_t1587;
            }
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(arg);
    }
    /* pass */
    if ((strcmp(_tr_strz(input_path), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        printf("%s\n", _tr_strz(_tr_str_lit("error: no input file specified")));
        /* pass */
        print_usage();
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (((!file_exists(input_path)) && (!str_ends_with_dot_tr(input_path)))) {
        /* pass */
        TrStr _strtmp_t1588 = _tr_strx_concat(_tr_strz(input_path), _tr_strz(_tr_str_lit(".tr")));
        _tr_str_release(input_path);
        input_path = _strtmp_t1588;
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[1/5] Resolving modules: ")), _tr_strz(input_path))));
    }
    /* pass */
    ModuleResolver* resolver = ModuleResolver_init();
    /* pass */
    TrStr bin_dir = ({ TrStr _at_t1589 = (_tr_str_wrap(_tr_exe_dir())); __auto_type _wr = (strip_trailing_sep_inline(_at_t1589)); _tr_str_release(_at_t1589); _wr; });
    /* pass */
    ModuleResolver_add_search_path(resolver, bin_dir);
    /* pass */
    ({ TrStr _at_t1590 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("/std")))); ModuleResolver_add_search_path(resolver, _at_t1590); _tr_str_release(_at_t1590); });
    /* pass */
    ({ TrStr _at_t1591 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("/packages")))); ModuleResolver_add_search_path(resolver, _at_t1591); _tr_str_release(_at_t1591); });
    /* pass */
    ({ TrStr _at_t1592 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("/packages/sites")))); ModuleResolver_add_search_path(resolver, _at_t1592); _tr_str_release(_at_t1592); });
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
            ({ TrStr _at_t1593 = (get_path_env_entry(tauraro_path_env, epi)); ModuleResolver_add_search_path(resolver, _at_t1593); _tr_str_release(_at_t1593); });
            /* pass */
            epi = (epi + 1LL);
        }
    }
    /* pass */
    Program* prog = ModuleResolver_resolve_main(resolver, input_path);
    /* pass */
    if ((resolver->parse_errors > 0LL)) {
        /* pass */
        printf("%s\n", _tr_strz(({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(resolver->parse_errors)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("error: ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" parse error(s); aborting compilation."))); _tr_str_release(_cl); _cres; })));
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if ((strcmp(_tr_strz(emit_mode), _tr_strz(_tr_str_lit("ast"))) == 0)) {
        /* pass */
        printf("%s\n", _tr_strz(({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(prog->decls->len)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[AST] Declarations found: ")), _cr.data); _tr_str_release(_cr); _cres; })));
        /* pass */
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tauraro_path_env);
        return 0;
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", _tr_strz(_tr_str_lit("[2/5] Semantic analysis...")));
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
            ({ TrStr _at_t1594 = (List_TrStr_get(sema->warnings, wk)); _print_diag(_tr_str_lit("warning"), _at_t1594); _tr_str_release(_at_t1594); });
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
            ({ TrStr _at_t1595 = (List_TrStr_get(sema->errors, k)); _print_diag(_tr_str_lit("error"), _at_t1595); _tr_str_release(_at_t1595); });
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (check_only) {
        /* pass */
        printf("%s\n", _tr_strz(_tr_str_lit("Check passed: no errors found.")));
        /* pass */
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tauraro_path_env);
        return 0;
    }
    /* pass */
    if ((strcmp(_tr_strz(emit_mode), _tr_strz(_tr_str_lit("mir"))) == 0)) {
        /* pass */
        printf("%s\n", _tr_strz(({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(hir->functions->len)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[MIR] ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" functions lowered"))); _tr_str_release(_cl); _cres; })));
        /* pass */
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tauraro_path_env);
        return 0;
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[3/5] Code generation (backend=")), _tr_strz(backend))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")..."))); _tr_str_release(_cl); _cres; })));
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
            TrStr _strtmp_t1596 = ({ TrStr _cl = (strip_extension(input_path)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".ll"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(output_path);
            output_path = _strtmp_t1596;
        }
        /* pass */
        write_file(output_path, llvm_ir);
        /* pass */
        if (verbose) {
            /* pass */
            printf("%s\n", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[4/5] LLVM IR written to ")), _tr_strz(output_path))));
        }
        /* pass */
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tauraro_path_env);
        _tr_str_release(llvm_ir);
        return 0;
    }
    /* pass */
    CGenerator* c_gen = CGenerator_init();
    /* pass */
    TrStr rt_h = ({ TrStr _at_t1597 = (List_TrStr_get(args, 0LL)); __auto_type _wr = (read_runtime_header(_at_t1597, input_path)); _tr_str_release(_at_t1597); _wr; });
    /* pass */
    CGenerator_register_program(c_gen, hir);
    /* pass */
    CGenerator_scan_mono_prog(c_gen, hir);
    /* pass */
    TrStr build_dir = _tr_str_lit("build/");
    /* pass */
    make_dir(build_dir);
    /* pass */
    if ((strcmp(_tr_strz(rt_h), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _at_t1598 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); write_file(_at_t1598, rt_h); _tr_str_release(_at_t1598); });
    }
    /* pass */
    TrStr types_h = CGenerator_generate_types_header(c_gen, hir);
    /* pass */
    TrStr _strtmp_t1599 = ({ TrStr _cr = (CGenerator_generate_module_compat(c_gen, resolver->all_decl_modules, resolver->all_decls)); TrStr _cres = _tr_strx_concat(_tr_strz(types_h), _cr.data); _tr_str_release(_cr); _cres; });
    _tr_str_release(types_h);
    types_h = _strtmp_t1599;
    /* pass */
    ({ TrStr _at_t1600 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_types.h")))); write_file(_at_t1600, types_h); _tr_str_release(_at_t1600); });
    /* pass */
    if ((strcmp(_tr_strz(rt_h), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        sync_headers_to_runtime(rt_h, types_h);
    }
    /* pass */
    List_TrStr* all_c_files = (void*)List_TrStr_new();
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
                __auto_type _t1601 = (*((Decl*)List_ptr_get(resolver->all_decls, k)));
                if (_t1601.tag == Decl_DClass) {
                    __auto_type c = _t1601.data.DClass.cls;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(c->name), true);
                } else if (_t1601.tag == Decl_DFunction) {
                    __auto_type f = _t1601.data.DFunction.func;
                    /* pass */
                    _tr_dict_set(fn_set, _tr_strz(f->name), true);
                } else if (_t1601.tag == Decl_DEnum) {
                    __auto_type e = _t1601.data.DEnum.enm;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(e->name), true);
                } else if (_t1601.tag == Decl_DInterface) {
                    __auto_type iface = _t1601.data.DInterface.iface;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(iface->name), true);
                } else if (_t1601.tag == Decl_DExtend) {
                    __auto_type target = _t1601.data.DExtend.target;
__auto_type methods = _t1601.data.DExtend.methods;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(target), true);
                    _tr_str_release(target);
                } else if (1) {
                    __auto_type _ = _t1601;
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
            TrStr _strtmp_t1602 = ensure_builtin_dirs(build_dir, dot_path);
            _tr_str_release(c_path);
            c_path = _strtmp_t1602;
            /* pass */
            depth = get_dot_depth(dot_path);
        } else {
            /* pass */
            TrStr _strtmp_t1603 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("module_")))); TrStr _cr = (dot_to_safe(dot_path)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".c"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(c_path);
            c_path = _strtmp_t1603;
            /* pass */
            depth = 0LL;
        }
        /* pass */
        TrStr mod_c = CGenerator_generate_module_c(c_gen, hir, class_set, fn_set, depth);
        /* pass */
        write_file(c_path, mod_c);
        /* pass */
        List_TrStr_append(all_c_files, c_path);
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
            __auto_type _t1604 = (*((Decl*)List_ptr_get(resolver->all_decls, k2)));
            if (_t1604.tag == Decl_DClass) {
                __auto_type c = _t1604.data.DClass.cls;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(c->name), true);
            } else if (_t1604.tag == Decl_DFunction) {
                __auto_type f = _t1604.data.DFunction.func;
                /* pass */
                _tr_dict_set(main_fn_set, _tr_strz(f->name), true);
            } else if (_t1604.tag == Decl_DEnum) {
                __auto_type e = _t1604.data.DEnum.enm;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(e->name), true);
            } else if (_t1604.tag == Decl_DInterface) {
                __auto_type iface = _t1604.data.DInterface.iface;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(iface->name), true);
            } else if (_t1604.tag == Decl_DExtend) {
                __auto_type target = _t1604.data.DExtend.target;
__auto_type methods = _t1604.data.DExtend.methods;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(target), true);
                _tr_str_release(target);
            } else if (1) {
                __auto_type _ = _t1604;
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
    TrStr main_c = CGenerator_generate_main_c(c_gen, hir, main_class_set, main_fn_set);
    /* pass */
    TrStr main_c_path = _tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("main.c")));
    /* pass */
    write_file(main_c_path, main_c);
    /* pass */
    List_TrStr_append(all_c_files, main_c_path);
    /* pass */
    if ((strcmp(_tr_strz(emit_mode), _tr_strz(_tr_str_lit("c"))) == 0)) {
        /* pass */
        printf("%s\n", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("Modular C output written to: ")), _tr_strz(build_dir))));
        /* pass */
        printf("%s\n", _tr_strz(_tr_str_lit("  tauraro_types.h  - shared type definitions + all function prototypes")));
        /* pass */
        printf("%s\n", _tr_strz(_tr_str_lit("  tauraro_rt.h     - runtime header")));
        /* pass */
        printf("%s\n", _tr_strz(_tr_str_lit("  main.c           - program entry")));
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < (all_c_files->len - 1LL))) {
            /* pass */
            ({ TrStr _at_t1605 = (List_TrStr_get(all_c_files, pi)); printf("%s\n", _tr_strz(({ TrStr _cr = (get_filename(_at_t1605)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _cr.data); _tr_str_release(_cr); _cres; }))); _tr_str_release(_at_t1605); });
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        _tr_str_release(output_path);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tauraro_path_env);
        _tr_str_release(rt_h);
        _tr_str_release(build_dir);
        _tr_str_release(types_h);
        List_TrStr_free(all_c_files);
        _tr_str_release(main_c);
        _tr_str_release(main_c_path);
        return 0;
    }
    /* pass */
    TrStr exe_name = _tr_str_lit("");
    /* pass */
    if ((strcmp(_tr_strz(output_path), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t1606 = _tr_str_retain(output_path);
        _tr_str_release(exe_name);
        exe_name = _strtmp_t1606;
    } else {
        /* pass */
        TrStr _strtmp_t1608 = ({ TrStr _at_t1607 = (get_filename(input_path)); __auto_type _wr = (strip_extension(_at_t1607)); _tr_str_release(_at_t1607); _wr; });
        _tr_str_release(exe_name);
        exe_name = _strtmp_t1608;
    }
    /* pass */
    if ((strcmp(_tr_strz(exe_name), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        TrStr _strtmp_t1609 = _tr_str_lit("a");
        _tr_str_release(exe_name);
        exe_name = _strtmp_t1609;
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
                        TrStr _strtmp_t1610 = _tr_str_wrap(_tr_str_slice(_tr_strz(exe_name), 0LL, (en_len - 4LL)));
                        _tr_str_release(exe_name);
                        exe_name = _strtmp_t1610;
                    }
                }
            }
        }
    }
    /* pass */
    TrStr exe_ext = _tr_str_lit(".exe");
    /* pass */
    if ((!_tr_is_windows())) {
        /* pass */
        TrStr _strtmp_t1611 = _tr_str_lit("");
        _tr_str_release(exe_ext);
        exe_ext = _strtmp_t1611;
    }
    /* pass */
    TrStr exe_path = _tr_str_lit("");
    /* pass */
    if ((strcmp(_tr_strz(output_path), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t1612 = _tr_strx_concat(_tr_strz(exe_name), _tr_strz(exe_ext));
        _tr_str_release(exe_path);
        exe_path = _strtmp_t1612;
    } else {
        /* pass */
        TrStr _strtmp_t1613 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(exe_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_ext)); _tr_str_release(_cl); _cres; });
        _tr_str_release(exe_path);
        exe_path = _strtmp_t1613;
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(all_c_files->len)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[5/5] Linking ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" modules -> "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; })));
    }
    /* pass */
    long long rc = compile_all_c(all_c_files, exe_path, build_dir, link_paths, lib_flags, opt_level, verbose, static_link, target, sysroot, debug_mode);
    /* pass */
    if ((rc != 0LL)) {
        /* pass */
        printf("%s\n", _tr_strz(({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(rc)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("error: compilation failed (exit code ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; })));
        /* pass */
        exit((int)(rc));
    }
    /* pass */
    if ((strcmp(_tr_strz(output_path), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        cleanup_build(build_dir, all_c_files);
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("Done: ")), _tr_strz(exe_path))));
    }
    /* pass */
    if (run_after) {
        /* pass */
        TrStr run_path = to_runnable_path(exe_path);
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            TrStr _strtmp_t1614 = path_to_native(run_path);
            _tr_str_release(run_path);
            run_path = _strtmp_t1614;
        }
        /* pass */
        long long run_rc = _tr_system(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _tr_strz(run_path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; })));
        /* pass */
        exit((int)(run_rc));
    }
    _tr_str_release(output_path);
    _tr_str_release(backend);
    _tr_str_release(emit_mode);
    _tr_str_release(opt_level);
    _tr_str_release(target);
    _tr_str_release(sysroot);
    _tr_str_release(tauraro_path_env);
    _tr_str_release(rt_h);
    _tr_str_release(build_dir);
    _tr_str_release(types_h);
    _tr_str_release(main_c);
    _tr_str_release(main_c_path);
    _tr_str_release(exe_name);
    _tr_str_release(exe_ext);
    _tr_str_release(exe_path);
#ifndef TAURARO_BARE
    if (_tr_global_async_pool) { _tr_threadpool_free(_tr_global_async_pool); _tr_global_async_pool = NULL; }
#endif
    return 0;
}
