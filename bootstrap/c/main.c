#define _TR_MAIN
#include "tauraro_types.h"


__attribute__((hot)) void print_version() {
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("tauraroc v0.0.8"))); printf("\n"); });
}

__attribute__((hot)) void print_usage() {
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("Usage: tauraroc <file.tr> [options]"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("       tauraroc fmt [-w] <file.tr>   Format source (stdout, or -w in place)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("       tauraroc lint <file.tr>       Analyze and report warnings/errors"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("       tauraroc bindgen <hdr.h>      Generate FFI bindings from a C header (-o out.tr)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("       tauraroc bindgen <hdr.hpp> -h cpp  Bind a C++ header (auto C++->C shim via libclang)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("       tauraroc bindgen <hdr> --pkg <name>  Auto-discover include/link flags via pkg-config"))); printf("\n"); });
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
    ({ printf("%s", _tr_strz(_tr_str_lit("  --backend <b>     Code generator: c (default, C->gcc/clang), llvm (LLVM IR),"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                      native (x86-64->ELF, under construction)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  -o <path>         Exact output path (its directory is honored; overrides -d)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  -d <dir>          Output directory for the executable (default: current dir)"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --lib             Build a shared library (.so/.dll) of `export def`s + a C header"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("  --export-cpp      Also emit an ergonomic C++ header to call Tauraro from C++ (implies --lib)"))); printf("\n"); });
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
    ({ printf("%s", _tr_strz(_tr_str_lit("  --target <name>   Cross-compile for a target platform (C and LLVM backends):"))); printf("\n"); });
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
    ({ printf("%s", _tr_strz(_tr_str_lit("  --no-heap         Reject heap types (List/Dict/Set/heap class/dynamic str) [H-1] — structurally"))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("                    zero-heap for bare metal; value-types/fixed-arrays/pointers/StrView only"))); printf("\n"); });
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
        TrStr _strtmp_t3270 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t3270;
    }
    /* pass */
    if (({ TrStr _aet_t3271 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("gcc --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3271.data) == 0LL)); _tr_str_release(_aet_t3271); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("gcc");
    }
    /* pass */
    if (({ TrStr _aet_t3272 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("clang --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3272.data) == 0LL)); _tr_str_release(_aet_t3272); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("clang");
    }
    /* pass */
    if (({ TrStr _aet_t3273 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("cc --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3273.data) == 0LL)); _tr_str_release(_aet_t3273); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("cc");
    }
    /* pass */
    if (({ TrStr _aet_t3274 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("zig version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3274.data) == 0LL)); _tr_str_release(_aet_t3274); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("zig cc");
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
        TrStr _strtmp_t3275 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t3275;
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(cc), _tr_strz(_tr_str_lit("clang")))) {
        /* pass */
        _tr_str_release(null_dev);
        return true;
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(cc), _tr_strz(_tr_str_lit("zig")))) {
        /* pass */
        _tr_str_release(null_dev);
        return true;
    }
    /* pass */
    if (_tr_str_eq(_tr_strz(cc), _tr_strz(_tr_str_lit("cc")))) {
        /* pass */
        _tr_str_release(null_dev);
        return ({ TrStr _aet_t3276 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" --version 2>&1 | grep -qi clang")))); __auto_type _wr = ((_tr_system(_aet_t3276.data) == 0LL)); _tr_str_release(_aet_t3276); _wr; });
    }
    /* pass */
    _tr_str_release(null_dev);
    return false;
}

__attribute__((hot)) TrStr pkg_config_query(TrStr name, TrStr kind) {
    /* pass */
    TrStr null_dev = _tr_str_lit("/dev/null");
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t3277 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t3277;
    }
    /* pass */
    TrStr tmp = _tr_str_lit("_tr_pkgcfg.txt");
    /* pass */
    long long rc = ({ TrStr _aet_t3278 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("pkg-config ")), _tr_strz(kind))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" > \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(null_dev)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_tr_system(_aet_t3278.data)); _tr_str_release(_aet_t3278); _wr; });
    /* pass */
    if ((rc != 0LL)) {
        /* pass */
        _tr_str_release(null_dev);
        _tr_str_release(tmp);
        return _tr_str_lit("");
    }
    /* pass */
    if ((!file_exists(tmp))) {
        /* pass */
        _tr_str_release(null_dev);
        _tr_str_release(tmp);
        return _tr_str_lit("");
    }
    /* pass */
    TrStr out = _tr_str_wrap(_tr_str_strip(_tr_strz(read_file(tmp))));
    /* pass */
    _tr_str_release(null_dev);
    _tr_str_release(tmp);
    return out;
}

__attribute__((hot)) TrStr cxx_of(TrStr cc) {
    /* pass */
    if (_tr_str_eq(_tr_strz(cc), _tr_strz(_tr_str_lit("gcc")))) {
        /* pass */
        return _tr_str_lit("g++");
    }
    /* pass */
    if (_tr_str_eq(_tr_strz(cc), _tr_strz(_tr_str_lit("clang")))) {
        /* pass */
        return _tr_str_lit("clang++");
    }
    /* pass */
    if (_tr_str_eq(_tr_strz(cc), _tr_strz(_tr_str_lit("zig cc")))) {
        /* pass */
        return _tr_str_lit("zig c++");
    }
    /* pass */
    return _tr_str_lit("c++");
}

__attribute__((hot)) bool cpp_lto_ok(TrStr cc, TrStr target) {
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        return false;
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(cc), _tr_strz(_tr_str_lit("gcc")))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(cc), _tr_strz(_tr_str_lit("zig")))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool auto_link_cpp_shims(List_TrStr* mod_file_paths, TrStr build_dir, TrStr cc, TrStr opt_level, List_TrStr* link_paths, List_TrStr* lib_flags, bool verbose, bool lto) {
    /* pass */
    bool any = false;
    /* pass */
    TrMap* seen = _tr_dict_new(8LL);
    /* pass */
    TrStr cxx = cxx_of(cc);
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < mod_file_paths->len)) {
        /* pass */
        TrStr trpath = List_TrStr_get(mod_file_paths, mi);
        /* pass */
        mi = (mi + 1LL);
        /* pass */
        if ((!file_exists(trpath))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr src = read_file(trpath);
        /* pass */
        List_TrStr* lines = _tr_str_split(_tr_strz(src), _tr_strz(_tr_str_lit("\n")));
        /* pass */
        TrStr shim = _tr_str_lit("");
        /* pass */
        TrStr cflags = _tr_str_lit("");
        /* pass */
        TrStr linkflags = _tr_str_lit("");
        /* pass */
        List_TrStr* libs = (void*)List_TrStr_new();
        /* pass */
        long long li = 0LL;
        /* pass */
        while ((li < lines->len)) {
            /* pass */
            TrStr ln = List_TrStr_get(lines, li);
            /* pass */
            li = (li + 1LL);
            /* pass */
            if (str_starts_with(ln, _tr_str_lit("# tauraro-cpp-shim:"))) {
                /* pass */
                TrStr _strtmp_t3280 = ({ TrStr _wt_t3279 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 19LL, _tr_strlen(_tr_strz(ln))))); __auto_type _wr = (_tr_str_wrap(_tr_str_strip(_wt_t3279.data))); _tr_str_release(_wt_t3279); _wr; });
                _tr_str_release(shim);
                shim = _strtmp_t3280;
            } else if (str_starts_with(ln, _tr_str_lit("# tauraro-cpp-cflags:"))) {
                /* pass */
                TrStr _strtmp_t3282 = ({ TrStr _wt_t3281 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 21LL, _tr_strlen(_tr_strz(ln))))); __auto_type _wr = (_tr_str_wrap(_tr_str_strip(_wt_t3281.data))); _tr_str_release(_wt_t3281); _wr; });
                _tr_str_release(cflags);
                cflags = _strtmp_t3282;
            } else if (str_starts_with(ln, _tr_str_lit("# tauraro-cpp-linkflags:"))) {
                /* pass */
                TrStr _strtmp_t3284 = ({ TrStr _wt_t3283 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 24LL, _tr_strlen(_tr_strz(ln))))); __auto_type _wr = (_tr_str_wrap(_tr_str_strip(_wt_t3283.data))); _tr_str_release(_wt_t3283); _wr; });
                _tr_str_release(linkflags);
                linkflags = _strtmp_t3284;
            } else if (str_starts_with(ln, _tr_str_lit("# tauraro-cpp-lib:"))) {
                /* pass */
                ({ TrStr _wt_t3285 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 18LL, _tr_strlen(_tr_strz(ln))))); TrStr _wt_t3286 = (_tr_str_wrap(_tr_str_strip(_wt_t3285.data))); TrStr _wt_t3287 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 18LL, _tr_strlen(_tr_strz(ln))))); TrStr _at_t3288 = (_tr_str_wrap(_tr_str_strip(_wt_t3287.data))); List_TrStr_append(libs, _at_t3288); _tr_str_release(_wt_t3285); _tr_str_release(_wt_t3286); _tr_str_release(_wt_t3287); _tr_str_release(_at_t3288); });
            } else if (((_tr_strlen(_tr_strz(ln)) > 0LL) && (!str_starts_with(ln, _tr_str_lit("#"))))) {
                /* pass */
                break;
            }
            _tr_str_release(ln);
        }
        /* pass */
        if (((strcmp(_tr_strz(shim), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(linkflags), _tr_strz(_tr_str_lit(""))) == 0))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(shim), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr shim_src = _tr_str_retain(shim);
            /* pass */
            if ((!file_exists(shim_src))) {
                /* pass */
                TrStr cand = ({ TrStr _cl = (dir_of_path(trpath)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shim)); _tr_str_release(_cl); _cres; });
                /* pass */
                if (file_exists(cand)) {
                    /* pass */
                    TrStr _strtmp_t3289 = _tr_str_retain(cand);
                    _tr_str_release(shim_src);
                    shim_src = _strtmp_t3289;
                } else {
                    /* pass */
                    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_yellow(_tr_str_lit("warning"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": C++ shim '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shim)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' referenced by "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(trpath)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" not found; link it manually with --link"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                    /* pass */
                    continue;
                }
            }
            /* pass */
            if ((!_tr_dict_contains(seen, _tr_strz(shim_src)))) {
                /* pass */
                _tr_dict_set(seen, _tr_strz(shim_src), true);
                /* pass */
                TrStr oname = ({ TrStr _cl = (({ TrStr _cr = (get_filename(shim_src)); TrStr _cres = _tr_strx_concat(_tr_strz(build_dir), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".o"))); _tr_str_release(_cl); _cres; });
                /* pass */
                TrStr sidecar = _tr_strx_concat(_tr_strz(oname), _tr_strz(_tr_str_lit(".src")));
                /* pass */
                TrStr shim_txt = read_file(shim_src);
                /* pass */
                bool need = true;
                /* pass */
                if ((file_exists(oname) && file_exists(sidecar))) {
                    /* pass */
                    if ((strcmp(_tr_strz(read_file(sidecar)), _tr_strz(shim_txt)) == 0)) {
                        /* pass */
                        need = false;
                    }
                }
                /* pass */
                if (need) {
                    /* pass */
                    TrStr stdflag = _tr_str_lit("");
                    /* pass */
                    if ((!_tr_str_contains(_tr_strz(cflags), _tr_strz(_tr_str_lit("-std"))))) {
                        /* pass */
                        TrStr _strtmp_t3290 = _tr_str_lit(" -std=c++17");
                        _tr_str_release(stdflag);
                        stdflag = _strtmp_t3290;
                    }
                    /* pass */
                    TrStr ltof = _tr_str_lit("");
                    /* pass */
                    if (lto) {
                        /* pass */
                        TrStr _strtmp_t3291 = _tr_str_lit(" -flto");
                        _tr_str_release(ltof);
                        ltof = _strtmp_t3291;
                    }
                    /* pass */
                    TrStr ccmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cxx), _tr_strz(_tr_str_lit(" -O")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(opt_level)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ltof)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(stdflag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -c \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shim_src)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cflags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(oname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    if (verbose) {
                        /* pass */
                        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("  [CXX shim] ")), _tr_strz(shim_src)))); printf("\n"); });
                    }
                    /* pass */
                    long long rc = _tr_system(_tr_strz(ccmd));
                    /* pass */
                    if ((rc != 0LL)) {
                        /* pass */
                        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": compiling C++ shim "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shim_src)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" failed (exit code "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(rc)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                        /* pass */
                        exit((int)(rc));
                    }
                    /* pass */
                    write_file(sidecar, shim_txt);
                    _tr_str_release(stdflag);
                    _tr_str_release(ltof);
                    _tr_str_release(ccmd);
                }
                /* pass */
                List_TrStr_append(link_paths, oname);
                /* pass */
                long long bi = 0LL;
                /* pass */
                while ((bi < libs->len)) {
                    /* pass */
                    ({ TrStr _at_t3292 = (({ TrStr _cr = (List_TrStr_get(libs, bi)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("-l")), _cr.data); _tr_str_release(_cr); _cres; })); List_TrStr_append(lib_flags, _at_t3292); _tr_str_release(_at_t3292); });
                    /* pass */
                    bi = (bi + 1LL);
                }
                _tr_str_release(oname);
            }
        }
        /* pass */
        if ((strcmp(_tr_strz(linkflags), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            List_TrStr_append(lib_flags, linkflags);
        }
        /* pass */
        any = true;
        _tr_str_release(trpath);
        _tr_str_release(src);
        List_TrStr_free(lines);
        _tr_str_release(shim);
        _tr_str_release(cflags);
        _tr_str_release(linkflags);
        List_TrStr_free(libs);
    }
    /* pass */
    Dict_free(seen);
    _tr_str_release(cxx);
    return any;
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

__attribute__((hot)) TrStr linker_script_cortex_m() {
    /* pass */
    TrStr s = _tr_str_lit("/* Generated by tauraroc --emit-ld : Cortex-M (mps2-an385) memory map. */\n");
    /* pass */
    TrStr _strtmp_t3293 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("ENTRY(_tr_reset)\n")));
    _tr_str_release(s);
    s = _strtmp_t3293;
    /* pass */
    TrStr _strtmp_t3294 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("MEMORY {\n")));
    _tr_str_release(s);
    s = _strtmp_t3294;
    /* pass */
    TrStr _strtmp_t3295 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 4M\n")));
    _tr_str_release(s);
    s = _strtmp_t3295;
    /* pass */
    TrStr _strtmp_t3296 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 4M\n")));
    _tr_str_release(s);
    s = _strtmp_t3296;
    /* pass */
    TrStr _strtmp_t3297 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("}\n")));
    _tr_str_release(s);
    s = _strtmp_t3297;
    /* pass */
    TrStr _strtmp_t3298 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("SECTIONS {\n")));
    _tr_str_release(s);
    s = _strtmp_t3298;
    /* pass */
    TrStr _strtmp_t3299 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  .isr_vector : { KEEP(*(.isr_vector)) } > FLASH\n")));
    _tr_str_release(s);
    s = _strtmp_t3299;
    /* pass */
    TrStr _strtmp_t3300 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  .text : { *(.text*) *(.rodata*) } > FLASH\n")));
    _tr_str_release(s);
    s = _strtmp_t3300;
    /* pass */
    TrStr _strtmp_t3301 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  _sidata = LOADADDR(.data);\n")));
    _tr_str_release(s);
    s = _strtmp_t3301;
    /* pass */
    TrStr _strtmp_t3302 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  .data : { _sdata = .; *(.data*) _edata = .; } > RAM AT > FLASH\n")));
    _tr_str_release(s);
    s = _strtmp_t3302;
    /* pass */
    TrStr _strtmp_t3303 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  .bss  : { __bss_start__ = .; *(.bss* COMMON) __bss_end__ = .; } > RAM\n")));
    _tr_str_release(s);
    s = _strtmp_t3303;
    /* pass */
    TrStr _strtmp_t3304 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  . = ALIGN(8);\n")));
    _tr_str_release(s);
    s = _strtmp_t3304;
    /* pass */
    TrStr _strtmp_t3305 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  . = . + 0x8000;\n")));
    _tr_str_release(s);
    s = _strtmp_t3305;
    /* pass */
    TrStr _strtmp_t3306 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  _stack_top = ORIGIN(RAM) + LENGTH(RAM);\n")));
    _tr_str_release(s);
    s = _strtmp_t3306;
    /* pass */
    TrStr _strtmp_t3307 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("}\n")));
    _tr_str_release(s);
    s = _strtmp_t3307;
    /* pass */
    return s;
}

__attribute__((hot)) TrStr linker_script_riscv() {
    /* pass */
    TrStr s = _tr_str_lit("/* Generated by tauraroc --emit-ld : RISC-V (qemu virt) memory map. */\n");
    /* pass */
    TrStr _strtmp_t3308 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("ENTRY(_start)\n")));
    _tr_str_release(s);
    s = _strtmp_t3308;
    /* pass */
    TrStr _strtmp_t3309 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("MEMORY {\n")));
    _tr_str_release(s);
    s = _strtmp_t3309;
    /* pass */
    TrStr _strtmp_t3310 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M\n")));
    _tr_str_release(s);
    s = _strtmp_t3310;
    /* pass */
    TrStr _strtmp_t3311 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("}\n")));
    _tr_str_release(s);
    s = _strtmp_t3311;
    /* pass */
    TrStr _strtmp_t3312 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("SECTIONS {\n")));
    _tr_str_release(s);
    s = _strtmp_t3312;
    /* pass */
    TrStr _strtmp_t3313 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  .text : { KEEP(*(.text.init)) *(.text*) *(.rodata*) } > RAM\n")));
    _tr_str_release(s);
    s = _strtmp_t3313;
    /* pass */
    TrStr _strtmp_t3314 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  .data : { *(.data*) *(.sdata*) } > RAM\n")));
    _tr_str_release(s);
    s = _strtmp_t3314;
    /* pass */
    TrStr _strtmp_t3315 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  .bss  : { __bss_start__ = .; *(.bss* .sbss* COMMON) __bss_end__ = .; } > RAM\n")));
    _tr_str_release(s);
    s = _strtmp_t3315;
    /* pass */
    TrStr _strtmp_t3316 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  . = ALIGN(16);\n")));
    _tr_str_release(s);
    s = _strtmp_t3316;
    /* pass */
    TrStr _strtmp_t3317 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  . = . + 0x8000;\n")));
    _tr_str_release(s);
    s = _strtmp_t3317;
    /* pass */
    TrStr _strtmp_t3318 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  _stack_top = .;\n")));
    _tr_str_release(s);
    s = _strtmp_t3318;
    /* pass */
    TrStr _strtmp_t3319 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("}\n")));
    _tr_str_release(s);
    s = _strtmp_t3319;
    /* pass */
    return s;
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
        TrStr _strtmp_t3320 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t3320;
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("android")))) {
        /* pass */
        TrStr ndk = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("ANDROID_NDK_ROOT"))));
        /* pass */
        if ((strcmp(_tr_strz(ndk), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t3321 = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("ANDROID_NDK_HOME"))));
            _tr_str_release(ndk);
            ndk = _strtmp_t3321;
        }
        /* pass */
        if ((strcmp(_tr_strz(ndk), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t3322 = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("NDK_HOME"))));
            _tr_str_release(ndk);
            ndk = _strtmp_t3322;
        }
        /* pass */
        if ((strcmp(_tr_strz(ndk), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr wrapper = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(ndk), _tr_strz(_tr_str_lit("/toolchains/llvm/prebuilt/linux-x86_64/bin/")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(triple)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("-clang"))); _tr_str_release(_cl); _cres; });
            /* pass */
            if (({ TrStr _aet_t3323 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _tr_strz(wrapper))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" --version >"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(null_dev)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3323.data) == 0LL)); _tr_str_release(_aet_t3323); _wr; })) {
                /* pass */
                _tr_str_release(null_dev);
                _tr_str_release(ndk);
                return wrapper;
            }
        }
    }
    /* pass */
    if (({ TrStr _aet_t3324 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("clang --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3324.data) == 0LL)); _tr_str_release(_aet_t3324); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        return _tr_str_lit("clang");
    }
    /* pass */
    _tr_str_release(null_dev);
    return detect_c_compiler();
}

__attribute__((hot)) TrStr detect_bundled_zig() {
    /* pass */
    TrStr ext = _tr_str_lit("");
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t3325 = _tr_str_lit(".exe");
        _tr_str_release(ext);
        ext = _strtmp_t3325;
    }
    /* pass */
    TrStr z = ({ TrStr _cl = (({ TrStr _cl = (_tr_str_wrap(_tr_exe_dir())); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/zig/zig"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ext)); _tr_str_release(_cl); _cres; });
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t3326 = path_to_native(z);
        _tr_str_release(z);
        z = _strtmp_t3326;
    }
    /* pass */
    if (file_exists(z)) {
        /* pass */
        _tr_str_release(ext);
        return z;
    }
    /* pass */
    _tr_str_release(ext);
    _tr_str_release(z);
    return _tr_str_lit("");
}

__attribute__((hot)) bool activate_bundled_zig() {
    /* pass */
    TrStr zbin = detect_bundled_zig();
    /* pass */
    if ((strcmp(_tr_strz(zbin), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        _tr_str_release(zbin);
        return false;
    }
    /* pass */
    TrStr zdir = ({ TrStr _cl = (_tr_str_wrap(_tr_exe_dir())); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/zig"))); _tr_str_release(_cl); _cres; });
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t3327 = path_to_native(zdir);
        _tr_str_release(zdir);
        zdir = _strtmp_t3327;
    }
    /* pass */
    TrStr sep = _tr_str_lit(":");
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t3328 = _tr_str_lit(";");
        _tr_str_release(sep);
        sep = _strtmp_t3328;
    }
    /* pass */
    TrStr cur = _tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit("PATH"))));
    /* pass */
    ({ TrStr _aet_t3329 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(zdir), _tr_strz(sep))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur)); _tr_str_release(_cl); _cres; })); _tr_setenv(_tr_strz(_tr_str_lit("PATH")), _aet_t3329.data); _tr_str_release(_aet_t3329); });
    /* pass */
    _tr_str_release(zbin);
    _tr_str_release(zdir);
    _tr_str_release(sep);
    _tr_str_release(cur);
    return true;
}

__attribute__((hot)) TrStr detect_lld_flag() {
    /* pass */
    TrStr null_dev = _tr_str_lit("/dev/null");
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t3330 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t3330;
    }
    /* pass */
    TrStr ext = _tr_str_lit("");
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t3331 = _tr_str_lit(".exe");
        _tr_str_release(ext);
        ext = _strtmp_t3331;
    }
    /* pass */
    TrStr bundled = ({ TrStr _cl = (({ TrStr _cl = (_tr_str_wrap(_tr_exe_dir())); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/llvm/ld.lld"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ext)); _tr_str_release(_cl); _cres; });
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t3332 = path_to_native(bundled);
        _tr_str_release(bundled);
        bundled = _strtmp_t3332;
    }
    /* pass */
    if (({ TrStr _aet_t3333 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _tr_strz(bundled))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" --version >"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(null_dev)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3333.data) == 0LL)); _tr_str_release(_aet_t3333); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        _tr_str_release(ext);
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit(" -fuse-ld=\"")), _tr_strz(bundled))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (({ TrStr _aet_t3334 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("ld.lld --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3334.data) == 0LL)); _tr_str_release(_aet_t3334); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        _tr_str_release(ext);
        _tr_str_release(bundled);
        return _tr_str_lit(" -fuse-ld=lld");
    }
    /* pass */
    if (({ TrStr _aet_t3335 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("lld --version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3335.data) == 0LL)); _tr_str_release(_aet_t3335); _wr; })) {
        /* pass */
        _tr_str_release(null_dev);
        _tr_str_release(ext);
        _tr_str_release(bundled);
        return _tr_str_lit(" -fuse-ld=lld");
    }
    /* pass */
    _tr_str_release(null_dev);
    _tr_str_release(ext);
    _tr_str_release(bundled);
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr zig_target_of(TrStr triple) {
    /* pass */
    if ((_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("mingw"))) || _tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("windows-gnu"))))) {
        /* pass */
        if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("aarch64")))) {
            /* pass */
            return _tr_str_lit("aarch64-windows-gnu");
        }
        /* pass */
        return _tr_str_lit("x86_64-windows-gnu");
    }
    /* pass */
    if ((_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("wasm32-wasi"))) || _tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("wasm32-wasip"))))) {
        /* pass */
        return _tr_str_lit("wasm32-wasi");
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("wasm")))) {
        /* pass */
        return _tr_str_lit("wasm32-freestanding");
    }
    /* pass */
    if (((_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("apple"))) || _tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("macos")))) || _tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("darwin"))))) {
        /* pass */
        if ((_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("aarch64"))) || _tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("arm64"))))) {
            /* pass */
            return _tr_str_lit("aarch64-macos-none");
        }
        /* pass */
        return _tr_str_lit("x86_64-macos-none");
    }
    /* pass */
    if (((_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("none-eabi"))) || _tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("none-elf")))) || _tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("unknown-elf"))))) {
        /* pass */
        if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("aarch64")))) {
            /* pass */
            return _tr_str_lit("aarch64-freestanding-none");
        }
        /* pass */
        if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("riscv64")))) {
            /* pass */
            return _tr_str_lit("riscv64-freestanding-none");
        }
        /* pass */
        if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("riscv32")))) {
            /* pass */
            return _tr_str_lit("riscv32-freestanding-none");
        }
        /* pass */
        if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("arm")))) {
            /* pass */
            return _tr_str_lit("arm-freestanding-eabi");
        }
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("android")))) {
        /* pass */
        return _tr_str_retain(triple);
    }
    /* pass */
    if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("linux")))) {
        /* pass */
        if (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("armv7")))) {
            /* pass */
            return ({ TrStr _cr = (slice_after_arch(triple)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("arm")), _cr.data); _tr_str_release(_cr); _cres; });
        }
        /* pass */
        return _tr_str_retain(triple);
    }
    /* pass */
    return _tr_str_retain(triple);
}

__attribute__((hot)) TrStr slice_after_arch(TrStr triple) {
    /* pass */
    char* p = ((char*)(_tr_strz(triple)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 0LL)) {
            /* pass */
            return _tr_str_lit("");
        }
        /* pass */
        if ((c == 45LL)) {
            /* pass */
            return _tr_str_wrap(_tr_str_slice(_tr_strz(triple), i, str_len_of(triple)));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) bool out_is_windows(TrStr target, TrStr triple) {
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        return _tr_is_windows();
    }
    /* pass */
    return (_tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("windows"))) || _tr_str_contains(_tr_strz(triple), _tr_strz(_tr_str_lit("mingw"))));
}

__attribute__((hot)) long long str_len_of(TrStr s) {
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
    return n;
}

__attribute__((hot)) TrStr resolve_cross_driver(TrStr triple) {
    /* pass */
    TrStr null_dev = _tr_str_lit("/dev/null");
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        TrStr _strtmp_t3336 = _tr_str_lit("nul");
        _tr_str_release(null_dev);
        null_dev = _strtmp_t3336;
    }
    /* pass */
    if (({ TrStr _aet_t3337 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("zig version >")), _tr_strz(null_dev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3337.data) == 0LL)); _tr_str_release(_aet_t3337); _wr; })) {
        /* pass */
        TrStr zt = zig_target_of(triple);
        /* pass */
        if ((strcmp(_tr_strz(zt), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            _tr_str_release(null_dev);
            return _tr_strx_concat(_tr_strz(_tr_str_lit("zig cc -target ")), _tr_strz(zt));
        }
    }
    /* pass */
    _tr_str_release(null_dev);
    return ({ TrStr _cl = (({ TrStr _cl = (detect_cross_compiler(triple)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" --target="))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(triple)); _tr_str_release(_cl); _cres; });
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

__attribute__((hot)) TrStr find_native_abi_c(TrStr input_path) {
    /* pass */
    if (file_exists(_tr_str_lit("tauraro/runtime/native_abi.c"))) {
        /* pass */
        return _tr_str_lit("tauraro/runtime/native_abi.c");
    }
    /* pass */
    TrStr bin_dir = ({ TrStr _cl = (_tr_str_wrap(_tr_exe_dir())); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/"))); _tr_str_release(_cl); _cres; });
    /* pass */
    if (({ TrStr _at_t3338 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("native_abi.c")))); __auto_type _wr = (file_exists(_at_t3338)); _tr_str_release(_at_t3338); _wr; })) {
        /* pass */
        return _tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("native_abi.c")));
    }
    /* pass */
    if (({ TrStr _at_t3339 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("runtime/native_abi.c")))); __auto_type _wr = (file_exists(_at_t3339)); _tr_str_release(_at_t3339); _wr; })) {
        /* pass */
        return _tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("runtime/native_abi.c")));
    }
    /* pass */
    if ((strcmp(_tr_strz(input_path), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr src_dir = dir_of_path(input_path);
        /* pass */
        if (({ TrStr _at_t3340 = (_tr_strx_concat(_tr_strz(src_dir), _tr_strz(_tr_str_lit("native_abi.c")))); __auto_type _wr = (file_exists(_at_t3340)); _tr_str_release(_at_t3340); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            return _tr_strx_concat(_tr_strz(src_dir), _tr_strz(_tr_str_lit("native_abi.c")));
        }
        /* pass */
        TrStr parent = ({ TrStr _at_t3341 = (strip_trailing_sep_inline(src_dir)); __auto_type _wr = (dir_of_path(_at_t3341)); _tr_str_release(_at_t3341); _wr; });
        /* pass */
        if (({ TrStr _at_t3342 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("runtime/native_abi.c")))); __auto_type _wr = (file_exists(_at_t3342)); _tr_str_release(_at_t3342); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src_dir);
            return _tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("runtime/native_abi.c")));
        }
        /* pass */
        TrStr gp = ({ TrStr _at_t3343 = (strip_trailing_sep_inline(parent)); __auto_type _wr = (dir_of_path(_at_t3343)); _tr_str_release(_at_t3343); _wr; });
        /* pass */
        if (({ TrStr _at_t3344 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("runtime/native_abi.c")))); __auto_type _wr = (file_exists(_at_t3344)); _tr_str_release(_at_t3344); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src_dir);
            _tr_str_release(parent);
            return _tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("runtime/native_abi.c")));
        }
        _tr_str_release(src_dir);
        _tr_str_release(parent);
    }
    /* pass */
    if (file_exists(_tr_str_lit("runtime/native_abi.c"))) {
        /* pass */
        _tr_str_release(bin_dir);
        return _tr_str_lit("runtime/native_abi.c");
    }
    /* pass */
    _tr_str_release(bin_dir);
    return _tr_str_lit("");
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
        if (({ TrStr _at_t3345 = (_tr_strx_concat(_tr_strz(src_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t3345)); _tr_str_release(_at_t3345); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            return ({ TrStr _at_t3346 = (_tr_strx_concat(_tr_strz(src_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t3346)); _tr_str_release(_at_t3346); _wr; });
        }
        /* pass */
        TrStr parent = ({ TrStr _at_t3347 = (strip_trailing_sep_inline(src_dir)); __auto_type _wr = (dir_of_path(_at_t3347)); _tr_str_release(_at_t3347); _wr; });
        /* pass */
        if (({ TrStr _at_t3348 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t3348)); _tr_str_release(_at_t3348); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            return ({ TrStr _at_t3349 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t3349)); _tr_str_release(_at_t3349); _wr; });
        }
        /* pass */
        if (({ TrStr _at_t3350 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t3350)); _tr_str_release(_at_t3350); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            return ({ TrStr _at_t3351 = (_tr_strx_concat(_tr_strz(parent), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t3351)); _tr_str_release(_at_t3351); _wr; });
        }
        /* pass */
        TrStr gp = ({ TrStr _at_t3352 = (strip_trailing_sep_inline(parent)); __auto_type _wr = (dir_of_path(_at_t3352)); _tr_str_release(_at_t3352); _wr; });
        /* pass */
        if (({ TrStr _at_t3353 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t3353)); _tr_str_release(_at_t3353); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            _tr_str_release(parent);
            return ({ TrStr _at_t3354 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("runtime/tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t3354)); _tr_str_release(_at_t3354); _wr; });
        }
        /* pass */
        if (({ TrStr _at_t3355 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (file_exists(_at_t3355)); _tr_str_release(_at_t3355); _wr; })) {
            /* pass */
            _tr_str_release(bin_dir);
            _tr_str_release(src1);
            _tr_str_release(src1b);
            _tr_str_release(src_dir);
            _tr_str_release(parent);
            return ({ TrStr _at_t3356 = (_tr_strx_concat(_tr_strz(gp), _tr_strz(_tr_str_lit("tauraro_rt.h")))); __auto_type _wr = (read_file(_at_t3356)); _tr_str_release(_at_t3356); _wr; });
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

__attribute__((hot)) bool has_path_sep(TrStr path) {
    /* pass */
    char* p = ((char*)(_tr_strz(path)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if (((c == 47LL) || (c == 92LL))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr out_dir_prefix(TrStr out_dir) {
    /* pass */
    if ((strcmp(_tr_strz(out_dir), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    TrStr d = _tr_str_retain(out_dir);
    /* pass */
    if ((!(_tr_str_ends_with(_tr_strz(d), _tr_strz(_tr_str_lit("/"))) || _tr_str_ends_with(_tr_strz(d), _tr_strz(_tr_str_lit("\\")))))) {
        /* pass */
        TrStr _strtmp_t3357 = _tr_strx_concat(_tr_strz(d), _tr_strz(_tr_str_lit("/")));
        _tr_str_release(d);
        d = _strtmp_t3357;
    }
    /* pass */
    make_dir(d);
    /* pass */
    return d;
}

__attribute__((hot)) TrStr resolve_output_stem(TrStr input_path, TrStr output_path, TrStr out_dir) {
    /* pass */
    if (((strcmp(_tr_strz(output_path), _tr_strz(_tr_str_lit(""))) != 0) && has_path_sep(output_path))) {
        /* pass */
        ({ TrStr _at_t3358 = (dir_of_path(output_path)); make_dir(_at_t3358); _tr_str_release(_at_t3358); });
        /* pass */
        return _tr_str_retain(output_path);
    }
    /* pass */
    TrStr pref = out_dir_prefix(out_dir);
    /* pass */
    if ((strcmp(_tr_strz(output_path), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        return _tr_strx_concat(_tr_strz(pref), _tr_strz(output_path));
    }
    /* pass */
    return ({ TrStr _at_t3359 = (get_filename(input_path)); __auto_type _wr = (({ TrStr _cr = (strip_extension(_at_t3359)); TrStr _cres = _tr_strx_concat(_tr_strz(pref), _cr.data); _tr_str_release(_cr); _cres; })); _tr_str_release(_at_t3359); _wr; });
}

__attribute__((hot)) TrStr ensure_ext(TrStr path, TrStr ext) {
    /* pass */
    if ((strcmp(_tr_strz(ext), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        return _tr_str_retain(path);
    }
    /* pass */
    if (_tr_str_ends_with(_tr_strz(path), _tr_strz(ext))) {
        /* pass */
        return _tr_str_retain(path);
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(path), _tr_strz(ext));
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
            TrStr _strtmp_t3360 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(current), _tr_strz(seg))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(current);
            current = _strtmp_t3360;
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
        ({ TrStr _aet_t3361 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(path)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("mkdir \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3361.data); _tr_str_release(_aet_t3361); });
    } else {
        /* pass */
        ({ TrStr _aet_t3362 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("mkdir -p \"")), _tr_strz(path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>/dev/null"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3362.data); _tr_str_release(_aet_t3362); });
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
        TrStr _strtmp_t3363 = resolve_target_triple(target);
        _tr_str_release(triple);
        triple = _strtmp_t3363;
        /* pass */
        TrStr _strtmp_t3364 = resolve_cross_driver(triple);
        _tr_str_release(cc);
        cc = _strtmp_t3364;
        /* pass */
        TrStr _strtmp_t3365 = target_extra_flags(triple);
        _tr_str_release(cross_flags);
        cross_flags = _strtmp_t3365;
        /* pass */
        if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t3366 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cross_flags), _tr_strz(_tr_str_lit(" --sysroot=\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
            _tr_str_release(cross_flags);
            cross_flags = _strtmp_t3366;
        }
    }
    /* pass */
    TrStr static_flag = _tr_str_lit("");
    /* pass */
    if (static_link) {
        /* pass */
        TrStr _strtmp_t3367 = _tr_str_lit(" -static");
        _tr_str_release(static_flag);
        static_flag = _strtmp_t3367;
    }
    /* pass */
    TrStr warn_flags = _tr_str_lit(" -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value");
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        TrStr _strtmp_t3368 = _tr_strx_concat(_tr_strz(warn_flags), _tr_strz(_tr_str_lit(" -Wno-unknown-attributes -Wno-parentheses-equality")));
        _tr_str_release(warn_flags);
        warn_flags = _strtmp_t3368;
    }
    /* pass */
    TrStr native_flags = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) == 0))) {
        /* pass */
        TrStr _strtmp_t3369 = _tr_str_lit(" -march=native -funroll-loops");
        _tr_str_release(native_flags);
        native_flags = _strtmp_t3369;
    }
    /* pass */
    TrStr overflow_flag = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) != 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("s"))) != 0))) {
        /* pass */
        TrStr _strtmp_t3370 = _tr_str_lit(" -ftrapv");
        _tr_str_release(overflow_flag);
        overflow_flag = _strtmp_t3370;
    }
    /* pass */
    TrStr cmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -O")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(opt_level)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(overflow_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(static_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(native_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cross_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(warn_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -DTAURARO_NO_RT_HELPERS \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(inc_dir)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c_files->len)) {
        /* pass */
        TrStr _strtmp_t3371 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(c_files, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t3371;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < link_paths->len)) {
        /* pass */
        TrStr _strtmp_t3372 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(link_paths, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t3372;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < lib_flags->len)) {
        /* pass */
        TrStr _strtmp_t3373 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (List_TrStr_get(lib_flags, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t3373;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr _strtmp_t3374 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lm")));
    _tr_str_release(cmd);
    cmd = _strtmp_t3374;
    /* pass */
    if (debug_mode) {
        /* pass */
        TrStr _strtmp_t3375 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -fsanitize=address,undefined -g")));
        _tr_str_release(cmd);
        cmd = _strtmp_t3375;
    }
    /* pass */
    if (out_is_windows(target, triple)) {
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
            TrStr _strtmp_t3376 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lws2_32 -mconsole")));
            _tr_str_release(cmd);
            cmd = _strtmp_t3376;
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

__attribute__((hot)) long long compile_all_c_incremental(List_TrStr* c_files, List_bool* needs, TrStr exe_path, TrStr inc_dir, List_TrStr* link_paths, List_TrStr* lib_flags, TrStr opt_level, bool verbose, bool static_link, TrStr target, TrStr sysroot, bool debug_mode, bool build_shared, bool link_cxx, bool use_lto) {
    /* pass */
    TrStr cc = detect_c_compiler();
    /* pass */
    TrStr triple = _tr_str_lit("");
    /* pass */
    TrStr cross_flags = _tr_str_lit("");
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t3377 = resolve_target_triple(target);
        _tr_str_release(triple);
        triple = _strtmp_t3377;
        /* pass */
        TrStr _strtmp_t3378 = resolve_cross_driver(triple);
        _tr_str_release(cc);
        cc = _strtmp_t3378;
        /* pass */
        TrStr _strtmp_t3379 = target_extra_flags(triple);
        _tr_str_release(cross_flags);
        cross_flags = _strtmp_t3379;
        /* pass */
        if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t3380 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cross_flags), _tr_strz(_tr_str_lit(" --sysroot=\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
            _tr_str_release(cross_flags);
            cross_flags = _strtmp_t3380;
        }
    }
    /* pass */
    TrStr static_flag = _tr_str_lit("");
    /* pass */
    if (static_link) {
        /* pass */
        TrStr _strtmp_t3381 = _tr_str_lit(" -static");
        _tr_str_release(static_flag);
        static_flag = _strtmp_t3381;
    }
    /* pass */
    TrStr warn_flags = _tr_str_lit(" -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value");
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        TrStr _strtmp_t3382 = _tr_strx_concat(_tr_strz(warn_flags), _tr_strz(_tr_str_lit(" -Wno-unknown-attributes -Wno-parentheses-equality")));
        _tr_str_release(warn_flags);
        warn_flags = _strtmp_t3382;
    }
    /* pass */
    TrStr native_flags = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) == 0))) {
        /* pass */
        TrStr _strtmp_t3383 = _tr_str_lit(" -march=native -funroll-loops");
        _tr_str_release(native_flags);
        native_flags = _strtmp_t3383;
    }
    /* pass */
    TrStr overflow_flag = _tr_str_lit("");
    /* pass */
    if (((strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("3"))) != 0) && (strcmp(_tr_strz(opt_level), _tr_strz(_tr_str_lit("s"))) != 0))) {
        /* pass */
        TrStr _strtmp_t3384 = _tr_str_lit(" -ftrapv");
        _tr_str_release(overflow_flag);
        overflow_flag = _strtmp_t3384;
    }
    /* pass */
    TrStr dbg = _tr_str_lit("");
    /* pass */
    if (debug_mode) {
        /* pass */
        TrStr _strtmp_t3385 = _tr_str_lit(" -fsanitize=address,undefined -g");
        _tr_str_release(dbg);
        dbg = _strtmp_t3385;
    }
    /* pass */
    TrStr pic = _tr_str_lit("");
    /* pass */
    if (build_shared) {
        /* pass */
        TrStr _strtmp_t3386 = _tr_str_lit(" -fPIC");
        _tr_str_release(pic);
        pic = _strtmp_t3386;
    }
    /* pass */
    TrStr lto_flag = _tr_str_lit("");
    /* pass */
    if (use_lto) {
        /* pass */
        TrStr _strtmp_t3387 = _tr_str_lit(" -flto");
        _tr_str_release(lto_flag);
        lto_flag = _strtmp_t3387;
    }
    /* pass */
    TrStr common = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit(" -O")), _tr_strz(opt_level))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(overflow_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(static_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(native_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cross_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(warn_flags)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(dbg)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pic)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lto_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -DTAURARO_NO_RT_HELPERS \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(inc_dir)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
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
                _tr_str_release(lto_flag);
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
        TrStr _strtmp_t3388 = _tr_str_lit(" -shared");
        _tr_str_release(shared_flag);
        shared_flag = _strtmp_t3388;
    }
    /* pass */
    TrStr link_driver = _tr_str_retain(cc);
    /* pass */
    if (link_cxx) {
        /* pass */
        TrStr _strtmp_t3389 = cxx_of(cc);
        _tr_str_release(link_driver);
        link_driver = _strtmp_t3389;
    }
    /* pass */
    TrStr cmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(link_driver), _tr_strz(common))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shared_flag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < o_files->len)) {
        /* pass */
        TrStr _strtmp_t3390 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(o_files, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t3390;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < link_paths->len)) {
        /* pass */
        TrStr _strtmp_t3391 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(link_paths, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t3391;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < lib_flags->len)) {
        /* pass */
        TrStr _strtmp_t3392 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (List_TrStr_get(lib_flags, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(cmd);
        cmd = _strtmp_t3392;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr _strtmp_t3393 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lm")));
    _tr_str_release(cmd);
    cmd = _strtmp_t3393;
    /* pass */
    if (out_is_windows(target, triple)) {
        /* pass */
        TrStr _strtmp_t3394 = _tr_strx_concat(_tr_strz(cmd), _tr_strz(_tr_str_lit(" -lws2_32 -mconsole")));
        _tr_str_release(cmd);
        cmd = _strtmp_t3394;
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
    _tr_str_release(lto_flag);
    _tr_str_release(common);
    List_TrStr_free(o_files);
    _tr_str_release(shared_flag);
    _tr_str_release(link_driver);
    return _tr_system(_tr_strz(cmd));
}

__attribute__((hot)) long long compile_c_to_exe(TrStr c_path, TrStr exe_path, TrStr opt_level, bool verbose) {
    /* pass */
    TrStr cc = detect_c_compiler();
    /* pass */
    TrStr opt_flag = _tr_strx_concat(_tr_strz(_tr_str_lit("-O")), _tr_strz(opt_level));
    /* pass */
    TrStr out_dir = ({ TrStr _at_t3395 = (dir_of_path(c_path)); __auto_type _wr = (strip_trailing_sep(_at_t3395)); _tr_str_release(_at_t3395); _wr; });
    /* pass */
    TrStr warn_flags = _tr_str_lit(" -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value");
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        TrStr _strtmp_t3396 = _tr_strx_concat(_tr_strz(warn_flags), _tr_strz(_tr_str_lit(" -Wno-unknown-attributes -Wno-parentheses-equality")));
        _tr_str_release(warn_flags);
        warn_flags = _strtmp_t3396;
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
        TrStr _strtmp_t3398 = ({ TrStr _wt_t3397 = (_tr_str_wrap(_tr_str_slice(_tr_strz(msg), 0LL, fix_idx))); __auto_type _wr = (_tr_str_wrap(_tr_str_strip(_wt_t3397.data))); _tr_str_release(_wt_t3397); _wr; });
        _tr_str_release(main_part);
        main_part = _strtmp_t3398;
        /* pass */
        TrStr _strtmp_t3400 = ({ TrStr _wt_t3399 = (_tr_str_wrap(_tr_str_slice(_tr_strz(msg), (fix_idx + 4LL), _tr_strlen(_tr_strz(msg))))); __auto_type _wr = (_tr_str_wrap(_tr_str_strip(_wt_t3399.data))); _tr_str_release(_wt_t3399); _wr; });
        _tr_str_release(fix_part);
        fix_part = _strtmp_t3400;
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
            TrStr _strtmp_t3401 = _tr_str_retain(head);
            _tr_str_release(loc_str);
            loc_str = _strtmp_t3401;
            /* pass */
            TrStr _strtmp_t3402 = loc_file(head);
            _tr_str_release(file_path);
            file_path = _strtmp_t3402;
            /* pass */
            line_no = ln;
            /* pass */
            TrStr _strtmp_t3403 = _tr_str_wrap(_tr_str_slice(_tr_strz(main_part), (pe + 2LL), _tr_strlen(_tr_strz(main_part))));
            _tr_str_release(body);
            body = _strtmp_t3403;
        }
    }
    /* pass */
    TrStr lvl = c_red(level);
    /* pass */
    if ((strcmp(_tr_strz(level), _tr_strz(_tr_str_lit("warning"))) == 0)) {
        /* pass */
        TrStr _strtmp_t3404 = c_yellow(level);
        _tr_str_release(lvl);
        lvl = _strtmp_t3404;
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
            TrStr _strtmp_t3405 = read_file(file_path);
            _tr_str_release(src);
            src = _strtmp_t3405;
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
                        TrStr _strtmp_t3406 = c_yellow(underline);
                        _tr_str_release(caret);
                        caret = _strtmp_t3406;
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
            ({ TrStr _at_t3407 = (List_TrStr_get(all_c_files, di)); TrStr _aet_t3408 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t3407)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("del /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3408.data); _tr_str_release(_at_t3407); _tr_str_release(_aet_t3408); });
        } else {
            /* pass */
            ({ TrStr _aet_t3409 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(all_c_files, di)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("rm -f \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3409.data); _tr_str_release(_aet_t3409); });
        }
        /* pass */
        di = (di + 1LL);
    }
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        ({ TrStr _at_t3410 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_types.h")))); TrStr _aet_t3411 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t3410)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("del /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3411.data); _tr_str_release(_at_t3410); _tr_str_release(_aet_t3411); });
        /* pass */
        ({ TrStr _at_t3412 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); TrStr _aet_t3413 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t3412)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("del /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3413.data); _tr_str_release(_at_t3412); _tr_str_release(_aet_t3413); });
        /* pass */
        ({ TrStr _at_t3414 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("include")))); TrStr _aet_t3415 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(_at_t3414)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("rmdir /S /Q \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3415.data); _tr_str_release(_at_t3414); _tr_str_release(_aet_t3415); });
        /* pass */
        ({ TrStr _aet_t3416 = (({ TrStr _cl = (({ TrStr _cr = (path_to_native(build_dir)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("rmdir \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>nul >nul"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3416.data); _tr_str_release(_aet_t3416); });
    } else {
        /* pass */
        ({ TrStr _aet_t3417 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -f \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("tauraro_types.h\""))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3417.data); _tr_str_release(_aet_t3417); });
        /* pass */
        ({ TrStr _aet_t3418 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -f \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("tauraro_rt.h\""))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3418.data); _tr_str_release(_aet_t3418); });
        /* pass */
        ({ TrStr _aet_t3419 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -rf \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("include\""))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3419.data); _tr_str_release(_aet_t3419); });
        /* pass */
        ({ TrStr _aet_t3420 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rmdir \"")), _tr_strz(build_dir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>/dev/null"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t3420.data); _tr_str_release(_aet_t3420); });
    }
}

__attribute__((hot)) void run_fmt(TrStr path0, bool write_in_place) {
    /* pass */
    TrStr path = _tr_str_retain(path0);
    /* pass */
    if (((!file_exists(path)) && (!str_ends_with_dot_tr(path)))) {
        /* pass */
        TrStr _strtmp_t3421 = _tr_strx_concat(_tr_strz(path), _tr_strz(_tr_str_lit(".tr")));
        _tr_str_release(path);
        path = _strtmp_t3421;
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
    _tr_obj_release(prog, _trdrop_Program);
    _tr_obj_release(f, _trdrop_Formatter);
    _tr_str_release(formatted);
}

__attribute__((hot)) int main(int argc, char** argv) {
    _tr_argc = argc; _tr_argv = argv;
    _tr_init_console();
    List_TrStr* args = List_TrStr_new();
    for (int _ai = 0; _ai < argc; _ai++) { List_TrStr_append(args, _tr_str_lit(argv[_ai])); }
    /* pass */
    activate_bundled_zig();
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
        if ((((strcmp(_tr_strz(a1), _tr_strz(_tr_str_lit("fmt"))) == 0) || (strcmp(_tr_strz(a1), _tr_strz(_tr_str_lit("lint"))) == 0)) || (strcmp(_tr_strz(a1), _tr_strz(_tr_str_lit("bindgen"))) == 0))) {
            /* pass */
            TrStr _strtmp_t3422 = _tr_str_retain(a1);
            _tr_str_release(subcmd);
            subcmd = _strtmp_t3422;
        }
    }
    /* pass */
    if ((strcmp(_tr_strz(subcmd), _tr_strz(_tr_str_lit("bindgen"))) == 0)) {
        /* pass */
        TrStr bg_header = _tr_str_lit("");
        /* pass */
        TrStr bg_out = _tr_str_lit("bindings.tr");
        /* pass */
        TrStr bg_cc = detect_c_compiler();
        /* pass */
        bool bg_cpp = false;
        /* pass */
        TrStr bg_extra = _tr_str_lit("");
        /* pass */
        TrStr bg_pkglibs = _tr_str_lit("");
        /* pass */
        long long bk = 2LL;
        /* pass */
        while ((bk < args->len)) {
            /* pass */
            TrStr ba = List_TrStr_get(args, bk);
            /* pass */
            if (((strcmp(_tr_strz(ba), _tr_strz(_tr_str_lit("--pkg"))) == 0) && ((bk + 1LL) < args->len))) {
                /* pass */
                TrStr pkgname = List_TrStr_get(args, (bk + 1LL));
                /* pass */
                bk = (bk + 1LL);
                /* pass */
                TrStr pkgcf = pkg_config_query(pkgname, _tr_str_lit("--cflags"));
                /* pass */
                TrStr pkglf = pkg_config_query(pkgname, _tr_str_lit("--libs"));
                /* pass */
                if (((strcmp(_tr_strz(pkgcf), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(pkglf), _tr_strz(_tr_str_lit(""))) == 0))) {
                    /* pass */
                    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_yellow(_tr_str_lit("warning"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": pkg-config found no package '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pkgname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' — pass -I/-D/-l manually"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                }
                /* pass */
                if ((strcmp(_tr_strz(pkgcf), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t3423 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(bg_extra), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pkgcf)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(bg_extra);
                    bg_extra = _strtmp_t3423;
                }
                /* pass */
                if ((strcmp(_tr_strz(pkglf), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t3424 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(bg_pkglibs), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pkglf)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(bg_pkglibs);
                    bg_pkglibs = _strtmp_t3424;
                }
            } else if (((strcmp(_tr_strz(ba), _tr_strz(_tr_str_lit("-o"))) == 0) && ((bk + 1LL) < args->len))) {
                /* pass */
                TrStr _strtmp_t3425 = List_TrStr_get(args, (bk + 1LL));
                _tr_str_release(bg_out);
                bg_out = _strtmp_t3425;
                /* pass */
                bk = (bk + 1LL);
            } else if (((strcmp(_tr_strz(ba), _tr_strz(_tr_str_lit("--cc"))) == 0) && ((bk + 1LL) < args->len))) {
                /* pass */
                TrStr _strtmp_t3426 = List_TrStr_get(args, (bk + 1LL));
                _tr_str_release(bg_cc);
                bg_cc = _strtmp_t3426;
                /* pass */
                bk = (bk + 1LL);
            } else if (((strcmp(_tr_strz(ba), _tr_strz(_tr_str_lit("-h"))) == 0) && ((bk + 1LL) < args->len))) {
                /* pass */
                if ((strcmp(_tr_strz(List_TrStr_get(args, (bk + 1LL))), _tr_strz(_tr_str_lit("cpp"))) == 0)) {
                    /* pass */
                    bg_cpp = true;
                }
                /* pass */
                bk = (bk + 1LL);
            } else if ((strcmp(_tr_strz(ba), _tr_strz(_tr_str_lit("--cpp"))) == 0)) {
                /* pass */
                bg_cpp = true;
            } else if (((strcmp(_tr_strz(ba), _tr_strz(_tr_str_lit("-I"))) == 0) && ((bk + 1LL) < args->len))) {
                /* pass */
                TrStr _strtmp_t3427 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(bg_extra), _tr_strz(_tr_str_lit(" -I\"")))); TrStr _cr = (List_TrStr_get(args, (bk + 1LL))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
                _tr_str_release(bg_extra);
                bg_extra = _strtmp_t3427;
                /* pass */
                bk = (bk + 1LL);
            } else if (((strcmp(_tr_strz(ba), _tr_strz(_tr_str_lit("-D"))) == 0) && ((bk + 1LL) < args->len))) {
                /* pass */
                TrStr _strtmp_t3428 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(bg_extra), _tr_strz(_tr_str_lit(" -D")))); TrStr _cr = (List_TrStr_get(args, (bk + 1LL))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(bg_extra);
                bg_extra = _strtmp_t3428;
                /* pass */
                bk = (bk + 1LL);
            } else if (((strcmp(_tr_strz(ba), _tr_strz(_tr_str_lit("-isystem"))) == 0) && ((bk + 1LL) < args->len))) {
                /* pass */
                TrStr _strtmp_t3429 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(bg_extra), _tr_strz(_tr_str_lit(" -isystem \"")))); TrStr _cr = (List_TrStr_get(args, (bk + 1LL))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
                _tr_str_release(bg_extra);
                bg_extra = _strtmp_t3429;
                /* pass */
                bk = (bk + 1LL);
            } else if ((((str_starts_with(ba, _tr_str_lit("-I")) || str_starts_with(ba, _tr_str_lit("-D"))) || str_starts_with(ba, _tr_str_lit("-std="))) || str_starts_with(ba, _tr_str_lit("-f")))) {
                /* pass */
                TrStr _strtmp_t3430 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(bg_extra), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ba)); _tr_str_release(_cl); _cres; });
                _tr_str_release(bg_extra);
                bg_extra = _strtmp_t3430;
            } else if ((!str_starts_with(ba, _tr_str_lit("-")))) {
                /* pass */
                if ((strcmp(_tr_strz(bg_header), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t3431 = _tr_str_retain(ba);
                    _tr_str_release(bg_header);
                    bg_header = _strtmp_t3431;
                }
            }
            /* pass */
            bk = (bk + 1LL);
            _tr_str_release(ba);
        }
        /* pass */
        if ((strcmp(_tr_strz(bg_header), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("error: bindgen requires a header: tauraroc bindgen <header.h> [-o out.tr] [--cc <compiler>] [-h cpp] [-I<dir>] [-D<macro>]"))); printf("\n"); });
            /* pass */
            exit((int)(1LL));
        }
        /* pass */
        if (bg_cpp) {
            /* pass */
            run_bindgen_cpp(bg_header, bg_out, bg_cc, bg_extra, bg_pkglibs);
        } else {
            /* pass */
            run_bindgen(bg_header, bg_out, bg_cc, bg_extra, bg_pkglibs);
        }
        /* pass */
        exit((int)(0LL));
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
                    TrStr _strtmp_t3432 = _tr_str_retain(fa);
                    _tr_str_release(fpath);
                    fpath = _strtmp_t3432;
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
    TrStr out_dir = _tr_str_lit("");
    /* pass */
    TrStr backend = _tr_str_lit("c");
    /* pass */
    TrStr emit_mode = _tr_str_lit("exe");
    /* pass */
    TrStr emit_ld = _tr_str_lit("");
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
    bool no_heap = false;
    /* pass */
    TrStr tier_define = _tr_str_lit("");
    /* pass */
    bool lib_mode = false;
    /* pass */
    bool no_auto_cpp = false;
    /* pass */
    bool export_cpp = false;
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
            TrStr _strtmp_t3433 = List_TrStr_get(args, i);
            _tr_str_release(emit_mode);
            emit_mode = _strtmp_t3433;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--emit-ld"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t3434 = List_TrStr_get(args, i);
            _tr_str_release(emit_ld);
            emit_ld = _strtmp_t3434;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--backend"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t3435 = List_TrStr_get(args, i);
            _tr_str_release(backend);
            backend = _strtmp_t3435;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-o"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t3436 = List_TrStr_get(args, i);
            _tr_str_release(output_path);
            output_path = _strtmp_t3436;
        } else if ((((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-d"))) == 0) || (strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--dir"))) == 0)) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t3437 = List_TrStr_get(args, i);
            _tr_str_release(out_dir);
            out_dir = _strtmp_t3437;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--link"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t3438 = (List_TrStr_get(args, i)); List_TrStr_append(link_paths, _at_t3438); _tr_str_release(_at_t3438); });
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-l"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t3439 = (({ TrStr _cr = (List_TrStr_get(args, i)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("-l")), _cr.data); _tr_str_release(_cr); _cres; })); List_TrStr_append(lib_flags, _at_t3439); _tr_str_release(_at_t3439); });
        } else if ((str_starts_with(arg, _tr_str_lit("-l")) && (strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-l"))) != 0))) {
            /* pass */
            List_TrStr_append(lib_flags, arg);
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O0"))) == 0)) {
            /* pass */
            TrStr _strtmp_t3440 = _tr_str_lit("0");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t3440;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O1"))) == 0)) {
            /* pass */
            TrStr _strtmp_t3441 = _tr_str_lit("1");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t3441;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O2"))) == 0)) {
            /* pass */
            TrStr _strtmp_t3442 = _tr_str_lit("2");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t3442;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-O3"))) == 0)) {
            /* pass */
            TrStr _strtmp_t3443 = _tr_str_lit("3");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t3443;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("-Os"))) == 0)) {
            /* pass */
            TrStr _strtmp_t3444 = _tr_str_lit("s");
            _tr_str_release(opt_level);
            opt_level = _strtmp_t3444;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--static"))) == 0)) {
            /* pass */
            static_link = true;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--target"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t3445 = List_TrStr_get(args, i);
            _tr_str_release(target);
            target = _strtmp_t3445;
        } else if (((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--sysroot"))) == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            TrStr _strtmp_t3446 = List_TrStr_get(args, i);
            _tr_str_release(sysroot);
            sysroot = _strtmp_t3446;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--debug"))) == 0)) {
            /* pass */
            debug_mode = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--strict"))) == 0)) {
            /* pass */
            strict_mode = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--no-elide"))) == 0)) {
            /* pass */
            no_elide = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--no-auto-cpp"))) == 0)) {
            /* pass */
            no_auto_cpp = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--export-cpp"))) == 0)) {
            /* pass */
            export_cpp = true;
            /* pass */
            lib_mode = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--no-heap"))) == 0)) {
            /* pass */
            no_heap = true;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--freestanding"))) == 0)) {
            /* pass */
            TrStr _strtmp_t3447 = _tr_str_lit("TAURARO_KERNEL");
            _tr_str_release(tier_define);
            tier_define = _strtmp_t3447;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--no-std"))) == 0)) {
            /* pass */
            TrStr _strtmp_t3448 = _tr_str_lit("TAURARO_NO_OS");
            _tr_str_release(tier_define);
            tier_define = _strtmp_t3448;
        } else if ((strcmp(_tr_strz(arg), _tr_strz(_tr_str_lit("--lib"))) == 0)) {
            /* pass */
            lib_mode = true;
        } else if ((!str_starts_with(arg, _tr_str_lit("-")))) {
            /* pass */
            if ((strcmp(_tr_strz(input_path), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t3449 = _tr_str_retain(arg);
                _tr_str_release(input_path);
                input_path = _strtmp_t3449;
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
        TrStr _strtmp_t3450 = _tr_strx_concat(_tr_strz(input_path), _tr_strz(_tr_str_lit(".tr")));
        _tr_str_release(input_path);
        input_path = _strtmp_t3450;
    }
    /* pass */
    if ((!file_exists(input_path))) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": cannot read "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(input_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": no such file"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[1/5] Resolving modules: ")), _tr_strz(input_path)))); printf("\n"); });
    }
    /* pass */
    ModuleResolver* resolver = ModuleResolver_init();
    /* pass */
    TrStr bin_dir = ({ TrStr _at_t3451 = (_tr_str_wrap(_tr_exe_dir())); __auto_type _wr = (strip_trailing_sep_inline(_at_t3451)); _tr_str_release(_at_t3451); _wr; });
    /* pass */
    ModuleResolver_add_search_path(resolver, bin_dir);
    /* pass */
    ({ TrStr _at_t3452 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("/std")))); ModuleResolver_add_search_path(resolver, _at_t3452); _tr_str_release(_at_t3452); });
    /* pass */
    ({ TrStr _at_t3453 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("/packages")))); ModuleResolver_add_search_path(resolver, _at_t3453); _tr_str_release(_at_t3453); });
    /* pass */
    ({ TrStr _at_t3454 = (_tr_strx_concat(_tr_strz(bin_dir), _tr_strz(_tr_str_lit("/packages/sites")))); ModuleResolver_add_search_path(resolver, _at_t3454); _tr_str_release(_at_t3454); });
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
            ({ TrStr _at_t3455 = (get_path_env_entry(tauraro_path_env, epi)); ModuleResolver_add_search_path(resolver, _at_t3455); _tr_str_release(_at_t3455); });
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
        _tr_str_release(out_dir);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(emit_ld);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tier_define);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(prog, _trdrop_Program);
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
    sema->no_heap = no_heap;
    /* pass */
    sema->heap_boxes_tuples = (strcmp(_tr_strz(backend), _tr_strz(_tr_str_lit("c"))) != 0);
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
            ({ TrStr _at_t3456 = (List_TrStr_get(sema->warnings, wk)); _print_diag(_tr_str_lit("warning"), _at_t3456); _tr_str_release(_at_t3456); });
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
            ({ TrStr _at_t3457 = (List_TrStr_get(sema->errors, k)); _print_diag(_tr_str_lit("error"), _at_t3457); _tr_str_release(_at_t3457); });
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
        _tr_str_release(out_dir);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(emit_ld);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tier_define);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(prog, _trdrop_Program);
        _tr_obj_release(sema, _trdrop_Sema);
        _tr_obj_release(hir, _trdrop_HirProgram);
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
        _tr_str_release(out_dir);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(emit_ld);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tier_define);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(prog, _trdrop_Program);
        _tr_obj_release(sema, _trdrop_Sema);
        _tr_obj_release(hir, _trdrop_HirProgram);
        _tr_obj_release(mir_prog, _trdrop_MirProgram);
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
        TrStr llvm_ir = LlvmGenerator_generate_nh(llvm_gen, hir, no_heap);
        /* pass */
        if ((!llvm_gen->ok)) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": the LLVM backend can't lower this program yet"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            /* pass */
            if ((strcmp(_tr_strz(llvm_gen->fail_note), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("       reason: ")), _tr_strz(llvm_gen->fail_note)))); printf("\n"); });
            }
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("       (it shares the native backend's feature subset). Use --backend c (default)."))); printf("\n"); });
            /* pass */
            exit((int)(2LL));
        }
        /* pass */
        if (_tr_str_ends_with(_tr_strz(output_path), _tr_strz(_tr_str_lit(".ll")))) {
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
            _tr_str_release(out_dir);
            _tr_str_release(backend);
            _tr_str_release(emit_mode);
            _tr_str_release(emit_ld);
            _tr_str_release(opt_level);
            List_TrStr_free(link_paths);
            List_TrStr_free(lib_flags);
            _tr_str_release(target);
            _tr_str_release(sysroot);
            _tr_str_release(tier_define);
            _tr_obj_release(resolver, _trdrop_ModuleResolver);
            _tr_str_release(bin_dir);
            _tr_str_release(tauraro_path_env);
            _tr_obj_release(prog, _trdrop_Program);
            _tr_obj_release(sema, _trdrop_Sema);
            _tr_obj_release(hir, _trdrop_HirProgram);
            _tr_obj_release(llvm_gen, _trdrop_LlvmGenerator);
            _tr_str_release(llvm_ir);
            return 0;
        }
        /* pass */
        TrStr lx_triple = _tr_str_lit("");
        /* pass */
        bool lx_is_cross = false;
        /* pass */
        if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t3458 = resolve_target_triple(target);
            _tr_str_release(lx_triple);
            lx_triple = _strtmp_t3458;
            /* pass */
            lx_is_cross = true;
        }
        /* pass */
        TrStr lx_tflags = _tr_str_lit("");
        /* pass */
        if (lx_is_cross) {
            /* pass */
            TrStr _strtmp_t3459 = target_extra_flags(lx_triple);
            _tr_str_release(lx_tflags);
            lx_tflags = _strtmp_t3459;
        }
        /* pass */
        TrStr lx_xcc = _tr_str_lit("");
        /* pass */
        bool lx_via_zig = false;
        /* pass */
        if (lx_is_cross) {
            /* pass */
            TrStr lx_xnull = _tr_str_lit("/dev/null");
            /* pass */
            if (_tr_is_windows()) {
                /* pass */
                TrStr _strtmp_t3460 = _tr_str_lit("nul");
                _tr_str_release(lx_xnull);
                lx_xnull = _strtmp_t3460;
            }
            /* pass */
            if (({ TrStr _aet_t3461 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("zig version >")), _tr_strz(lx_xnull))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3461.data) == 0LL)); _tr_str_release(_aet_t3461); _wr; })) {
                /* pass */
                TrStr lx_zt = zig_target_of(lx_triple);
                /* pass */
                if ((strcmp(_tr_strz(lx_zt), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t3462 = _tr_strx_concat(_tr_strz(_tr_str_lit("zig cc -target ")), _tr_strz(lx_zt));
                    _tr_str_release(lx_xcc);
                    lx_xcc = _strtmp_t3462;
                    /* pass */
                    lx_via_zig = true;
                }
            }
            /* pass */
            if (({ TrStr _aet_t3463 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("clang --version >")), _tr_strz(lx_xnull))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = (((strcmp(_tr_strz(lx_xcc), _tr_strz(_tr_str_lit(""))) == 0) && (_tr_system(_aet_t3463.data) == 0LL))); _tr_str_release(_aet_t3463); _wr; })) {
                /* pass */
                TrStr _strtmp_t3464 = _tr_strx_concat(_tr_strz(_tr_str_lit("clang --target=")), _tr_strz(lx_triple));
                _tr_str_release(lx_xcc);
                lx_xcc = _strtmp_t3464;
            }
            /* pass */
            if ((strcmp(_tr_strz(lx_xcc), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": cross-compiling --backend llvm to "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_triple)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" needs a clang-family driver"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                /* pass */
                ({ printf("%s", _tr_strz(_tr_str_lit("       install 'zig' (recommended — zig cc bundles libc, no sysroot) or clang >= 15."))); printf("\n"); });
                /* pass */
                exit((int)(1LL));
            }
            /* pass */
            if (verbose) {
                /* pass */
                ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[llvm] cross driver: ")), _tr_strz(lx_xcc)))); printf("\n"); });
            }
        }
        /* pass */
        TrStr lx_ext = _tr_str_lit("");
        /* pass */
        if (lx_is_cross) {
            /* pass */
            if (_tr_str_contains(_tr_strz(lx_triple), _tr_strz(_tr_str_lit("wasm")))) {
                /* pass */
                TrStr _strtmp_t3465 = _tr_str_lit(".wasm");
                _tr_str_release(lx_ext);
                lx_ext = _strtmp_t3465;
            } else if ((_tr_str_contains(_tr_strz(lx_triple), _tr_strz(_tr_str_lit("windows"))) || _tr_str_contains(_tr_strz(lx_triple), _tr_strz(_tr_str_lit("mingw"))))) {
                /* pass */
                TrStr _strtmp_t3466 = _tr_str_lit(".exe");
                _tr_str_release(lx_ext);
                lx_ext = _strtmp_t3466;
            }
        } else if (_tr_is_windows()) {
            /* pass */
            TrStr _strtmp_t3467 = _tr_str_lit(".exe");
            _tr_str_release(lx_ext);
            lx_ext = _strtmp_t3467;
        }
        /* pass */
        TrStr lx_exe = ({ TrStr _at_t3468 = (resolve_output_stem(input_path, output_path, out_dir)); __auto_type _wr = (ensure_ext(_at_t3468, lx_ext)); _tr_str_release(_at_t3468); _wr; });
        /* pass */
        make_dir(_tr_str_lit("build"));
        /* pass */
        TrStr lx_ll = _tr_str_lit("build/llvm_out.ll");
        /* pass */
        if (((lx_is_cross && _tr_str_contains(_tr_strz(lx_triple), _tr_strz(_tr_str_lit("wasm")))) && _tr_str_contains(_tr_strz(llvm_ir), _tr_strz(_tr_str_lit("define i32 @main()"))))) {
            /* pass */
            TrStr _strtmp_t3469 = _tr_strx_concat(_tr_strz(llvm_ir), _tr_strz(_tr_str_lit("\n@__main_void = hidden alias i32 (), ptr @main\n")));
            _tr_str_release(llvm_ir);
            llvm_ir = _strtmp_t3469;
        }
        /* pass */
        write_file(lx_ll, llvm_ir);
        /* pass */
        if (verbose) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[4/5] LLVM IR written to ")), _tr_strz(lx_ll)))); printf("\n"); });
        }
        /* pass */
        TrStr lx_abi = find_native_abi_c(input_path);
        /* pass */
        if ((strcmp(_tr_strz(lx_abi), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": runtime/native_abi.c not found (needed to link --backend llvm output)"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("       looked beside the compiler binary, the input file, and ./runtime/"))); printf("\n"); });
            /* pass */
            exit((int)(1LL));
        }
        /* pass */
        TrStr lx_cc = detect_c_compiler();
        /* pass */
        TrStr lx_xflags = _tr_str_lit("");
        /* pass */
        if (lx_is_cross) {
            /* pass */
            TrStr _strtmp_t3470 = _tr_str_retain(lx_xcc);
            _tr_str_release(lx_cc);
            lx_cc = _strtmp_t3470;
            /* pass */
            TrStr _strtmp_t3471 = _tr_str_retain(lx_tflags);
            _tr_str_release(lx_xflags);
            lx_xflags = _strtmp_t3471;
            /* pass */
            if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                TrStr _strtmp_t3472 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(lx_xflags), _tr_strz(_tr_str_lit(" --sysroot=\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
                _tr_str_release(lx_xflags);
                lx_xflags = _strtmp_t3472;
            }
        }
        /* pass */
        TrStr lx_rto = _tr_str_lit("build/llvm_runtime.o");
        /* pass */
        long long lx_rc = ({ TrStr _aet_t3473 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(lx_cc), _tr_strz(lx_xflags))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -O2 -w -c \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (dir_of_path(lx_abi)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_abi)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_rto)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_tr_system(_aet_t3473.data)); _tr_str_release(_aet_t3473); _wr; });
        /* pass */
        if ((lx_rc != 0LL)) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": failed to compile the native runtime ("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_abi)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            /* pass */
            if (lx_is_cross) {
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("       target ")), _tr_strz(lx_triple))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" needs its libc headers/sysroot."))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                /* pass */
                ({ printf("%s", _tr_strz(_tr_str_lit("       fix: pass --sysroot <path>, or install 'zig' (zig cc bundles libc); freestanding"))); printf("\n"); });
                /* pass */
                ({ printf("%s", _tr_strz(_tr_str_lit("       targets (embedded-*, wasm) need no sysroot once the bare runtime lands (Phase 2/3)."))); printf("\n"); });
            }
            /* pass */
            exit((int)(1LL));
        }
        /* pass */
        TrStr lx_null = _tr_str_lit("/dev/null");
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            TrStr _strtmp_t3474 = _tr_str_lit("nul");
            _tr_str_release(lx_null);
            lx_null = _strtmp_t3474;
        }
        /* pass */
        TrStr lx_libs = _tr_str_lit(" -lm");
        /* pass */
        TrStr lx_libs_gcc = _tr_str_lit(" -lm");
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            TrStr _strtmp_t3475 = _tr_strx_concat(_tr_strz(lx_libs), _tr_strz(_tr_str_lit(" -lws2_32 -lucrtbase -mconsole")));
            _tr_str_release(lx_libs);
            lx_libs = _strtmp_t3475;
            /* pass */
            TrStr _strtmp_t3476 = _tr_strx_concat(_tr_strz(lx_libs_gcc), _tr_strz(_tr_str_lit(" -lws2_32")));
            _tr_str_release(lx_libs_gcc);
            lx_libs_gcc = _strtmp_t3476;
        }
        /* pass */
        bool lx_built = false;
        /* pass */
        if (lx_is_cross) {
            /* pass */
            TrStr lx_xobj = _tr_str_lit("build/llvm_out.o");
            /* pass */
            TrStr lx_cg = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(lx_xcc), _tr_strz(_tr_str_lit(" -O")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(opt_level)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_tflags)); _tr_str_release(_cl); _cres; });
            /* pass */
            if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                TrStr _strtmp_t3477 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(lx_cg), _tr_strz(_tr_str_lit(" --sysroot=\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
                _tr_str_release(lx_cg);
                lx_cg = _strtmp_t3477;
            }
            /* pass */
            if (({ TrStr _aet_t3478 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(lx_cg), _tr_strz(_tr_str_lit(" -c \"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_ll)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_xobj)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>build/llvm_link.log"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3478.data) != 0LL)); _tr_str_release(_aet_t3478); _wr; })) {
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": LLVM cross code generation failed for "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_triple)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" (see build/llvm_link.log)"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                /* pass */
                exit((int)(1LL));
            }
            /* pass */
            if (verbose) {
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[4/5] target object -> ")), _tr_strz(lx_xobj))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" ("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_triple)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            }
            /* pass */
            TrStr lx_xlibs = _tr_str_lit(" -lm");
            /* pass */
            if ((_tr_str_contains(_tr_strz(lx_triple), _tr_strz(_tr_str_lit("wasm"))) || _tr_str_contains(_tr_strz(lx_tflags), _tr_strz(_tr_str_lit("nostdlib"))))) {
                /* pass */
                TrStr _strtmp_t3479 = _tr_str_lit("");
                _tr_str_release(lx_xlibs);
                lx_xlibs = _strtmp_t3479;
            } else if ((_tr_str_contains(_tr_strz(lx_triple), _tr_strz(_tr_str_lit("windows"))) || _tr_str_contains(_tr_strz(lx_triple), _tr_strz(_tr_str_lit("mingw"))))) {
                /* pass */
                TrStr _strtmp_t3480 = _tr_str_lit(" -lm -lws2_32 -mconsole");
                _tr_str_release(lx_xlibs);
                lx_xlibs = _strtmp_t3480;
            }
            /* pass */
            TrStr lx_xld = _tr_str_lit("");
            /* pass */
            if (((!_tr_str_contains(_tr_strz(lx_triple), _tr_strz(_tr_str_lit("wasm")))) && (!lx_via_zig))) {
                /* pass */
                TrStr _strtmp_t3481 = detect_lld_flag();
                _tr_str_release(lx_xld);
                lx_xld = _strtmp_t3481;
            }
            /* pass */
            TrStr lx_xlink = _tr_strx_concat(_tr_strz(lx_xcc), _tr_strz(lx_tflags));
            /* pass */
            if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                TrStr _strtmp_t3482 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(lx_xlink), _tr_strz(_tr_str_lit(" --sysroot=\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
                _tr_str_release(lx_xlink);
                lx_xlink = _strtmp_t3482;
            }
            /* pass */
            TrStr _strtmp_t3483 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(lx_xlink), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_xobj)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_rto)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_xlibs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_xld)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_exe)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>>build/llvm_link.log"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(lx_xlink);
            lx_xlink = _strtmp_t3483;
            /* pass */
            if ((_tr_system(_tr_strz(lx_xlink)) == 0LL)) {
                /* pass */
                lx_built = true;
            }
            /* pass */
            if ((!lx_built)) {
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": cross-link to "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_triple)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" failed (see build/llvm_link.log)"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("       install 'zig' (bundles libc+lld, no sysroot), or LLD + a ")), _tr_strz(lx_triple))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" sysroot (--sysroot)."))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                /* pass */
                exit((int)(1LL));
            }
            /* pass */
            if (verbose) {
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[5/5] cross LLVM executable -> ")), _tr_strz(lx_exe))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" ("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_triple)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            }
            /* pass */
            _tr_str_release(subcmd);
            _tr_str_release(input_path);
            _tr_str_release(output_path);
            _tr_str_release(out_dir);
            _tr_str_release(backend);
            _tr_str_release(emit_mode);
            _tr_str_release(emit_ld);
            _tr_str_release(opt_level);
            List_TrStr_free(link_paths);
            List_TrStr_free(lib_flags);
            _tr_str_release(target);
            _tr_str_release(sysroot);
            _tr_str_release(tier_define);
            _tr_obj_release(resolver, _trdrop_ModuleResolver);
            _tr_str_release(bin_dir);
            _tr_str_release(tauraro_path_env);
            _tr_obj_release(prog, _trdrop_Program);
            _tr_obj_release(sema, _trdrop_Sema);
            _tr_obj_release(hir, _trdrop_HirProgram);
            _tr_obj_release(llvm_gen, _trdrop_LlvmGenerator);
            _tr_str_release(llvm_ir);
            _tr_str_release(lx_triple);
            _tr_str_release(lx_tflags);
            _tr_str_release(lx_xcc);
            _tr_str_release(lx_ext);
            _tr_str_release(lx_exe);
            _tr_str_release(lx_ll);
            _tr_str_release(lx_abi);
            _tr_str_release(lx_cc);
            _tr_str_release(lx_xflags);
            _tr_str_release(lx_rto);
            _tr_str_release(lx_null);
            _tr_str_release(lx_libs);
            _tr_str_release(lx_libs_gcc);
            _tr_str_release(lx_xobj);
            _tr_str_release(lx_cg);
            _tr_str_release(lx_xlibs);
            _tr_str_release(lx_xld);
            _tr_str_release(lx_xlink);
            return 0;
        }
        /* pass */
        TrStr lx_tri = _tr_str_lit("");
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            TrStr _strtmp_t3484 = _tr_str_lit("x86_64-pc-windows-gnu");
            _tr_str_release(lx_tri);
            lx_tri = _strtmp_t3484;
        }
        /* pass */
        long long lx_inproc = _tr_llvm_emit_object(_tr_strz(lx_ll), _tr_strz(_tr_str_lit("build/llvm_out.o")), _tr_strz(lx_tri));
        /* pass */
        if ((lx_inproc == 0LL)) {
            /* pass */
            if (({ TrStr _aet_t3485 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(lx_cc), _tr_strz(_tr_str_lit(" build/llvm_out.o ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_rto)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_libs_gcc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_exe)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>build/llvm_link.log"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3485.data) == 0LL)); _tr_str_release(_aet_t3485); _wr; })) {
                /* pass */
                lx_built = true;
            }
        }
        /* pass */
        if (({ TrStr _aet_t3486 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("clang --version >")), _tr_strz(lx_null))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = (((!lx_built) && (_tr_system(_aet_t3486.data) == 0LL))); _tr_str_release(_aet_t3486); _wr; })) {
            /* pass */
            TrStr lx_cmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("clang -O")), _tr_strz(opt_level))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_ll)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_rto)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_libs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_exe)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>build/llvm_link.log"))); _tr_str_release(_cl); _cres; });
            /* pass */
            if ((_tr_system(_tr_strz(lx_cmd)) == 0LL)) {
                /* pass */
                lx_built = true;
            }
        }
        /* pass */
        if (({ TrStr _aet_t3487 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("llc --version >")), _tr_strz(lx_null))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = (((!lx_built) && (_tr_system(_aet_t3487.data) == 0LL))); _tr_str_release(_aet_t3487); _wr; })) {
            /* pass */
            TrStr lx_llc_tri = _tr_str_lit("");
            /* pass */
            if (_tr_is_windows()) {
                /* pass */
                TrStr _strtmp_t3488 = _tr_str_lit(" -mtriple=x86_64-pc-windows-gnu");
                _tr_str_release(lx_llc_tri);
                lx_llc_tri = _strtmp_t3488;
            }
            /* pass */
            if (({ TrStr _aet_t3489 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("llc -O2 -filetype=obj")), _tr_strz(lx_llc_tri))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_ll)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o build/llvm_out.o 2>build/llvm_link.log"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3489.data) == 0LL)); _tr_str_release(_aet_t3489); _wr; })) {
                /* pass */
                if (({ TrStr _aet_t3490 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(lx_cc), _tr_strz(_tr_str_lit(" build/llvm_out.o ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_rto)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_libs_gcc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_exe)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>>build/llvm_link.log"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3490.data) == 0LL)); _tr_str_release(_aet_t3490); _wr; })) {
                    /* pass */
                    lx_built = true;
                }
            }
        }
        /* pass */
        if (({ TrStr _aet_t3491 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("zig version >")), _tr_strz(lx_null))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>&1"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = (((!lx_built) && (_tr_system(_aet_t3491.data) == 0LL))); _tr_str_release(_aet_t3491); _wr; })) {
            /* pass */
            TrStr lx_zrto = _tr_str_lit("build/llvm_runtime_zig.o");
            /* pass */
            if (({ TrStr _aet_t3492 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (dir_of_path(lx_abi)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("zig cc -O2 -w -c \"-I")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_abi)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" -o "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_zrto)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>build/llvm_link.log"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3492.data) == 0LL)); _tr_str_release(_aet_t3492); _wr; })) {
                /* pass */
                if (({ TrStr _aet_t3493 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("zig cc -O")), _tr_strz(opt_level))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_ll)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_zrto)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_libs_gcc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -o \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lx_exe)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>>build/llvm_link.log"))); _tr_str_release(_cl); _cres; })); __auto_type _wr = ((_tr_system(_aet_t3493.data) == 0LL)); _tr_str_release(_aet_t3493); _wr; })) {
                    /* pass */
                    lx_built = true;
                }
            }
        }
        /* pass */
        if ((!lx_built)) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": could not compile the LLVM IR to an executable"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("       need clang (LLVM >= 15) / llc on PATH, or a bundled/installed zig;"))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("       see build/llvm_link.log"))); printf("\n"); });
            /* pass */
            exit((int)(1LL));
        }
        /* pass */
        if (verbose) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[5/5] executable written to ")), _tr_strz(lx_exe)))); printf("\n"); });
        }
        /* pass */
        if (run_after) {
            /* pass */
            TrStr lx_run = to_runnable_path(lx_exe);
            /* pass */
            if (_tr_is_windows()) {
                /* pass */
                TrStr _strtmp_t3494 = path_to_native(lx_run);
                _tr_str_release(lx_run);
                lx_run = _strtmp_t3494;
            }
            /* pass */
            ({ TrStr _aet_t3495 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _tr_strz(lx_run))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; })); exit((int)(_tr_system(_aet_t3495.data))); _tr_str_release(_aet_t3495); });
        }
        /* pass */
        _tr_str_release(subcmd);
        _tr_str_release(input_path);
        _tr_str_release(output_path);
        _tr_str_release(out_dir);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(emit_ld);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tier_define);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(prog, _trdrop_Program);
        _tr_obj_release(sema, _trdrop_Sema);
        _tr_obj_release(hir, _trdrop_HirProgram);
        _tr_obj_release(llvm_gen, _trdrop_LlvmGenerator);
        _tr_str_release(llvm_ir);
        _tr_str_release(lx_triple);
        _tr_str_release(lx_tflags);
        _tr_str_release(lx_xcc);
        _tr_str_release(lx_ext);
        _tr_str_release(lx_exe);
        _tr_str_release(lx_ll);
        _tr_str_release(lx_abi);
        _tr_str_release(lx_cc);
        _tr_str_release(lx_xflags);
        _tr_str_release(lx_rto);
        _tr_str_release(lx_null);
        _tr_str_release(lx_libs);
        _tr_str_release(lx_libs_gcc);
        _tr_str_release(lx_tri);
        return 0;
    }
    /* pass */
    if ((strcmp(_tr_strz(backend), _tr_strz(_tr_str_lit("native"))) == 0)) {
        /* pass */
        NativeGenerator* nat_gen = NativeGenerator_init();
        /* pass */
        TrStr nat_out = ({ TrStr _at_t3496 = (resolve_output_stem(input_path, output_path, out_dir)); __auto_type _wr = (ensure_ext(_at_t3496, _tr_str_lit(".o"))); _tr_str_release(_at_t3496); _wr; });
        /* pass */
        if ((!NativeGenerator_emit_object(nat_gen, hir, nat_out))) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": the native backend (--backend native) is not implemented yet"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            /* pass */
            if ((strcmp(_tr_strz(nat_gen->fail_note), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("       reason: ")), _tr_strz(nat_gen->fail_note)))); printf("\n"); });
            }
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("       it is under construction (x86-64/ELF). Use --backend c (default) or --backend llvm for now."))); printf("\n"); });
            /* pass */
            exit((int)(2LL));
        }
        /* pass */
        if (verbose) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[4/5] native object written to ")), _tr_strz(nat_out)))); printf("\n"); });
        }
        /* pass */
        _tr_str_release(subcmd);
        _tr_str_release(input_path);
        _tr_str_release(output_path);
        _tr_str_release(out_dir);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(emit_ld);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tier_define);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(prog, _trdrop_Program);
        _tr_obj_release(sema, _trdrop_Sema);
        _tr_obj_release(hir, _trdrop_HirProgram);
        _tr_obj_release(nat_gen, _trdrop_NativeGenerator);
        _tr_str_release(nat_out);
        return 0;
    }
    /* pass */
    CGenerator* c_gen = CGenerator_init();
    /* pass */
    c_gen->emit_line_info = debug_mode;
    /* pass */
    c_gen->no_elide = no_elide;
    /* pass */
    c_gen->tier_define = _tr_str_retain(tier_define);
    /* pass */
    if (_tr_str_contains(_tr_strz(target), _tr_strz(_tr_str_lit("riscv")))) {
        /* pass */
        c_gen->bare_arch = _tr_str_lit("riscv");
    }
    /* pass */
    TrStr rt_h = ({ TrStr _at_t3497 = (List_TrStr_get(args, 0LL)); __auto_type _wr = (read_runtime_header(_at_t3497, input_path)); _tr_str_release(_at_t3497); _wr; });
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
    TrStr _strtmp_t3498 = ({ TrStr _cr = (CGenerator_generate_module_compat(c_gen, resolver->all_decl_modules, resolver->all_decls)); TrStr _cres = _tr_strx_concat(_tr_strz(types_h), _cr.data); _tr_str_release(_cr); _cres; });
    _tr_str_release(types_h);
    types_h = _strtmp_t3498;
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
        TrStr _strtmp_t3499 = _tr_strx_concat(_tr_strz(flags_sig), _tr_strz(_tr_str_lit(";static")));
        _tr_str_release(flags_sig);
        flags_sig = _strtmp_t3499;
    }
    /* pass */
    if (debug_mode) {
        /* pass */
        TrStr _strtmp_t3500 = _tr_strx_concat(_tr_strz(flags_sig), _tr_strz(_tr_str_lit(";debug")));
        _tr_str_release(flags_sig);
        flags_sig = _strtmp_t3500;
    }
    /* pass */
    if ((strcmp(_tr_strz(sysroot), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t3501 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(flags_sig), _tr_strz(_tr_str_lit(";sysroot=")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sysroot)); _tr_str_release(_cl); _cres; });
        _tr_str_release(flags_sig);
        flags_sig = _strtmp_t3501;
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
        ({ TrStr _at_t3502 = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("tauraro_rt.h")))); write_file(_at_t3502, rt_h); _tr_str_release(_at_t3502); });
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
                __auto_type _t3503 = (*((Decl*)List_ptr_get(resolver->all_decls, k)));
                if (_t3503.tag == Decl_DClass) {
                    __auto_type c = _t3503.data.DClass.cls;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(c->name), true);
                } else if (_t3503.tag == Decl_DFunction) {
                    __auto_type f = _t3503.data.DFunction.func;
                    /* pass */
                    _tr_dict_set(fn_set, _tr_strz(f->name), true);
                    _tr_obj_release(f, _trdrop_FunctionDef);
                } else if (_t3503.tag == Decl_DEnum) {
                    __auto_type e = _t3503.data.DEnum.enm;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(e->name), true);
                } else if (_t3503.tag == Decl_DInterface) {
                    __auto_type iface = _t3503.data.DInterface.iface;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(iface->name), true);
                } else if (_t3503.tag == Decl_DExtend) {
                    __auto_type target = _t3503.data.DExtend.target;
__auto_type methods = _t3503.data.DExtend.methods;
                    /* pass */
                    _tr_dict_set(class_set, _tr_strz(target), true);
                    _tr_str_release(target);
                } else if (1) {
                    __auto_type _ = _t3503;
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
            TrStr _strtmp_t3504 = ensure_builtin_dirs(build_dir, dot_path);
            _tr_str_release(c_path);
            c_path = _strtmp_t3504;
            /* pass */
            depth = get_dot_depth(dot_path);
        } else {
            /* pass */
            TrStr _strtmp_t3505 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(build_dir), _tr_strz(_tr_str_lit("module_")))); TrStr _cr = (dot_to_safe(dot_path)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".c"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(c_path);
            c_path = _strtmp_t3505;
            /* pass */
            depth = 0LL;
        }
        /* pass */
        if ((c_gen->emit_line_info && (mi < resolver->mod_file_paths->len))) {
            /* pass */
            c_gen->cur_src_file = ({ TrStr _at_t3506 = (List_TrStr_get(resolver->mod_file_paths, mi)); __auto_type _wr = (to_fwd_slashes(_at_t3506)); _tr_str_release(_at_t3506); _wr; });
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
            __auto_type _t3507 = (*((Decl*)List_ptr_get(resolver->all_decls, k2)));
            if (_t3507.tag == Decl_DClass) {
                __auto_type c = _t3507.data.DClass.cls;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(c->name), true);
            } else if (_t3507.tag == Decl_DFunction) {
                __auto_type f = _t3507.data.DFunction.func;
                /* pass */
                _tr_dict_set(main_fn_set, _tr_strz(f->name), true);
                _tr_obj_release(f, _trdrop_FunctionDef);
            } else if (_t3507.tag == Decl_DEnum) {
                __auto_type e = _t3507.data.DEnum.enm;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(e->name), true);
            } else if (_t3507.tag == Decl_DInterface) {
                __auto_type iface = _t3507.data.DInterface.iface;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(iface->name), true);
            } else if (_t3507.tag == Decl_DExtend) {
                __auto_type target = _t3507.data.DExtend.target;
__auto_type methods = _t3507.data.DExtend.methods;
                /* pass */
                _tr_dict_set(main_class_set, _tr_strz(target), true);
                _tr_str_release(target);
            } else if (1) {
                __auto_type _ = _t3507;
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
    CppExporter* cpp_exporter = CppExporter_init(c_gen);
    /* pass */
    if (export_cpp) {
        /* pass */
        TrStr _strtmp_t3508 = ({ TrStr _cr = (CppExporter_generate_cpp_class_bridges(cpp_exporter, hir)); TrStr _cres = _tr_strx_concat(_tr_strz(main_c), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(main_c);
        main_c = _strtmp_t3508;
    }
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
    if ((strcmp(_tr_strz(emit_ld), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        if (_tr_str_contains(_tr_strz(target), _tr_strz(_tr_str_lit("riscv")))) {
            /* pass */
            ({ TrStr _at_t3509 = (linker_script_riscv()); write_file(emit_ld, _at_t3509); _tr_str_release(_at_t3509); });
        } else {
            /* pass */
            ({ TrStr _at_t3510 = (linker_script_cortex_m()); write_file(emit_ld, _at_t3510); _tr_str_release(_at_t3510); });
        }
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("Linker script written to: ")), _tr_strz(emit_ld)))); printf("\n"); });
    }
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
            ({ TrStr _at_t3511 = (List_TrStr_get(all_c_files, pi)); ({ printf("%s", _tr_strz(({ TrStr _cr = (get_filename(_at_t3511)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _cr.data); _tr_str_release(_cr); _cres; }))); printf("\n"); }); _tr_str_release(_at_t3511); });
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        _tr_str_release(subcmd);
        _tr_str_release(input_path);
        _tr_str_release(output_path);
        _tr_str_release(out_dir);
        _tr_str_release(backend);
        _tr_str_release(emit_mode);
        _tr_str_release(emit_ld);
        _tr_str_release(opt_level);
        List_TrStr_free(link_paths);
        List_TrStr_free(lib_flags);
        _tr_str_release(target);
        _tr_str_release(sysroot);
        _tr_str_release(tier_define);
        _tr_obj_release(resolver, _trdrop_ModuleResolver);
        _tr_str_release(bin_dir);
        _tr_str_release(tauraro_path_env);
        _tr_obj_release(prog, _trdrop_Program);
        _tr_obj_release(sema, _trdrop_Sema);
        _tr_obj_release(hir, _trdrop_HirProgram);
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
        _tr_obj_release(cpp_exporter, _trdrop_CppExporter);
        _tr_str_release(main_c_path);
        return 0;
    }
    /* pass */
    TrStr exe_name = resolve_output_stem(input_path, output_path, out_dir);
    /* pass */
    if ((strcmp(_tr_strz(exe_name), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        TrStr _strtmp_t3512 = _tr_str_lit("a");
        _tr_str_release(exe_name);
        exe_name = _strtmp_t3512;
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
                        TrStr _strtmp_t3513 = _tr_str_wrap(_tr_str_slice(_tr_strz(exe_name), 0LL, (en_len - 4LL)));
                        _tr_str_release(exe_name);
                        exe_name = _strtmp_t3513;
                    }
                }
            }
        }
    }
    /* pass */
    TrStr tgt_triple = _tr_str_lit("");
    /* pass */
    if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t3514 = resolve_target_triple(target);
        _tr_str_release(tgt_triple);
        tgt_triple = _strtmp_t3514;
    }
    /* pass */
    bool out_win = out_is_windows(target, tgt_triple);
    /* pass */
    TrStr exe_ext = _tr_str_lit("");
    /* pass */
    if (lib_mode) {
        /* pass */
        if (out_win) {
            /* pass */
            TrStr _strtmp_t3515 = _tr_str_lit(".dll");
            _tr_str_release(exe_ext);
            exe_ext = _strtmp_t3515;
        } else {
            /* pass */
            TrStr _strtmp_t3516 = _tr_str_lit(".so");
            _tr_str_release(exe_ext);
            exe_ext = _strtmp_t3516;
        }
    } else if (out_win) {
        /* pass */
        TrStr _strtmp_t3517 = _tr_str_lit(".exe");
        _tr_str_release(exe_ext);
        exe_ext = _strtmp_t3517;
    } else if (((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) != 0) && _tr_str_contains(_tr_strz(tgt_triple), _tr_strz(_tr_str_lit("wasm"))))) {
        /* pass */
        TrStr _strtmp_t3518 = _tr_str_lit(".wasm");
        _tr_str_release(exe_ext);
        exe_ext = _strtmp_t3518;
    }
    /* pass */
    TrStr exe_path = _tr_strx_concat(_tr_strz(exe_name), _tr_strz(exe_ext));
    /* pass */
    if (lib_mode) {
        /* pass */
        TrStr hdr = CGenerator_generate_export_header(c_gen, hir);
        /* pass */
        TrStr hdr_path = _tr_strx_concat(_tr_strz(exe_name), _tr_strz(_tr_str_lit(".h")));
        /* pass */
        write_file(hdr_path, hdr);
        /* pass */
        if (verbose) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("[lib] header: ")), _tr_strz(hdr_path)))); printf("\n"); });
        }
        /* pass */
        if (export_cpp) {
            /* pass */
            TrStr ns = get_filename(exe_name);
            /* pass */
            if ((strcmp(_tr_strz(ns), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t3519 = _tr_str_lit("tauraro");
                _tr_str_release(ns);
                ns = _strtmp_t3519;
            }
            /* pass */
            TrStr cpp_hdr = CppExporter_generate_export_cpp_header(cpp_exporter, hir, ns);
            /* pass */
            TrStr cpp_hdr_path = _tr_strx_concat(_tr_strz(exe_name), _tr_strz(_tr_str_lit(".hpp")));
            /* pass */
            write_file(cpp_hdr_path, cpp_hdr);
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[export-cpp] C++ header: ")), _tr_strz(cpp_hdr_path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("  (namespace "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ns)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        }
    }
    /* pass */
    if (((strcmp(_tr_strz(emit_ld), _tr_strz(_tr_str_lit(""))) != 0) && (!lib_mode))) {
        /* pass */
        TrStr bm_elf = _tr_strx_concat(_tr_strz(exe_name), _tr_strz(_tr_str_lit(".elf")));
        /* pass */
        TrStr bm_cc = detect_c_compiler();
        /* pass */
        if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t3521 = ({ TrStr _at_t3520 = (resolve_target_triple(target)); __auto_type _wr = (resolve_cross_driver(_at_t3520)); _tr_str_release(_at_t3520); _wr; });
            _tr_str_release(bm_cc);
            bm_cc = _strtmp_t3521;
        }
        /* pass */
        TrStr bm_arch = _tr_str_lit(" -mcpu=cortex-m3 -mthumb");
        /* pass */
        if (_tr_str_contains(_tr_strz(target), _tr_strz(_tr_str_lit("riscv")))) {
            /* pass */
            TrStr _strtmp_t3522 = _tr_str_lit(" -march=rv64imac -mabi=lp64 -mcmodel=medany");
            _tr_str_release(bm_arch);
            bm_arch = _strtmp_t3522;
        }
        /* pass */
        TrStr bm_lld = _tr_str_lit("");
        /* pass */
        if ((!_tr_str_contains(_tr_strz(bm_cc), _tr_strz(_tr_str_lit("zig"))))) {
            /* pass */
            TrStr _strtmp_t3523 = detect_lld_flag();
            _tr_str_release(bm_lld);
            bm_lld = _strtmp_t3523;
        }
        /* pass */
        TrStr bm_cmd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(bm_cc), _tr_strz(_tr_str_lit(" -O")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(opt_level)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bm_arch)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -ffreestanding -nostdlib -fno-builtin"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bm_lld)); _tr_str_release(_cl); _cres; });
        /* pass */
        TrStr _strtmp_t3524 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(bm_cmd), _tr_strz(_tr_str_lit(" -T \"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(emit_ld)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(build_dir)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("include\" \"-I"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(build_dir)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(bm_cmd);
        bm_cmd = _strtmp_t3524;
        /* pass */
        long long bmi = 0LL;
        /* pass */
        while ((bmi < all_c_files->len)) {
            /* pass */
            TrStr _strtmp_t3525 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(bm_cmd), _tr_strz(_tr_str_lit(" \"")))); TrStr _cr = (List_TrStr_get(all_c_files, bmi)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
            _tr_str_release(bm_cmd);
            bm_cmd = _strtmp_t3525;
            /* pass */
            bmi = (bmi + 1LL);
        }
        /* pass */
        TrStr _strtmp_t3526 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(bm_cmd), _tr_strz(_tr_str_lit(" -o \"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bm_elf)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>build/bare_link.log"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(bm_cmd);
        bm_cmd = _strtmp_t3526;
        /* pass */
        if (verbose) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[5/5] bare-metal link -> ")), _tr_strz(bm_elf))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("  ("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bm_cc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        }
        /* pass */
        if ((_tr_system(_tr_strz(bm_cmd)) == 0LL)) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("Bare-metal ELF written to: ")), _tr_strz(bm_elf)))); printf("\n"); });
            /* pass */
            _tr_str_release(subcmd);
            _tr_str_release(input_path);
            _tr_str_release(output_path);
            _tr_str_release(out_dir);
            _tr_str_release(backend);
            _tr_str_release(emit_mode);
            _tr_str_release(emit_ld);
            _tr_str_release(opt_level);
            List_TrStr_free(link_paths);
            List_TrStr_free(lib_flags);
            _tr_str_release(target);
            _tr_str_release(sysroot);
            _tr_str_release(tier_define);
            _tr_obj_release(resolver, _trdrop_ModuleResolver);
            _tr_str_release(bin_dir);
            _tr_str_release(tauraro_path_env);
            _tr_obj_release(prog, _trdrop_Program);
            _tr_obj_release(sema, _trdrop_Sema);
            _tr_obj_release(hir, _trdrop_HirProgram);
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
            _tr_obj_release(cpp_exporter, _trdrop_CppExporter);
            _tr_str_release(main_c_path);
            _tr_str_release(exe_name);
            _tr_str_release(tgt_triple);
            _tr_str_release(exe_ext);
            _tr_str_release(exe_path);
            _tr_str_release(bm_elf);
            _tr_str_release(bm_cc);
            _tr_str_release(bm_arch);
            _tr_str_release(bm_lld);
            _tr_str_release(bm_cmd);
            return 0;
        }
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": bare-metal link failed (see build/bare_link.log)"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("       need a freestanding toolchain — the bundled zig, or arm-none-eabi / riscv64 gcc."))); printf("\n"); });
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    bool has_cpp = false;
    /* pass */
    bool use_lto = false;
    /* pass */
    if ((!no_auto_cpp)) {
        /* pass */
        TrStr cc0 = detect_c_compiler();
        /* pass */
        if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t3528 = ({ TrStr _at_t3527 = (resolve_target_triple(target)); __auto_type _wr = (resolve_cross_driver(_at_t3527)); _tr_str_release(_at_t3527); _wr; });
            _tr_str_release(cc0);
            cc0 = _strtmp_t3528;
        }
        /* pass */
        use_lto = cpp_lto_ok(cc0, target);
        /* pass */
        has_cpp = auto_link_cpp_shims(resolver->mod_file_paths, build_dir, cc0, opt_level, link_paths, lib_flags, verbose, use_lto);
    }
    /* pass */
    if ((!has_cpp)) {
        /* pass */
        use_lto = false;
    }
    /* pass */
    if (verbose) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(all_c_files->len)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[5/5] Compiling + linking ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" modules -> "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(exe_path)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    }
    /* pass */
    long long rc = compile_all_c_incremental(all_c_files, needs_recompile, exe_path, build_dir, link_paths, lib_flags, opt_level, verbose, static_link, target, sysroot, debug_mode, lib_mode, has_cpp, use_lto);
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
            TrStr _strtmp_t3529 = path_to_native(run_path);
            _tr_str_release(run_path);
            run_path = _strtmp_t3529;
        }
        /* pass */
        long long run_rc = ({ TrStr _aet_t3530 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _tr_strz(run_path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_tr_system(_aet_t3530.data)); _tr_str_release(_aet_t3530); _wr; });
        /* pass */
        exit((int)(run_rc));
    }
    _tr_str_release(subcmd);
    _tr_str_release(input_path);
    _tr_str_release(output_path);
    _tr_str_release(out_dir);
    _tr_str_release(backend);
    _tr_str_release(emit_mode);
    _tr_str_release(emit_ld);
    _tr_str_release(opt_level);
    _tr_str_release(target);
    _tr_str_release(sysroot);
    _tr_str_release(tier_define);
    _tr_obj_release(resolver, _trdrop_ModuleResolver);
    _tr_str_release(bin_dir);
    _tr_str_release(tauraro_path_env);
    _tr_obj_release(prog, _trdrop_Program);
    _tr_obj_release(sema, _trdrop_Sema);
    _tr_obj_release(hir, _trdrop_HirProgram);
    _tr_obj_release(c_gen, _trdrop_CGenerator);
    _tr_str_release(rt_h);
    _tr_str_release(build_dir);
    _tr_str_release(types_h);
    _tr_str_release(types_path);
    _tr_str_release(flags_sig);
    _tr_str_release(flags_path);
    _tr_str_release(main_c);
    _tr_obj_release(cpp_exporter, _trdrop_CppExporter);
    _tr_str_release(main_c_path);
    _tr_str_release(exe_name);
    _tr_str_release(tgt_triple);
    _tr_str_release(exe_ext);
    _tr_str_release(exe_path);
#ifndef TAURARO_BARE
    _tr_async_pool_shutdown();
#endif
    return 0;
}
