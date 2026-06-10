#define _TR_MAIN
#include "tauraro_types.h"


__attribute__((hot)) void print_version() {
    /* pass */
    printf("%s\n", (char*)("tauraroc v0.0.5"));
}

__attribute__((hot)) void print_usage() {
    /* pass */
    printf("%s\n", (char*)("Usage: tauraroc <file.tr> [options]"));
    /* pass */
    printf("%s\n", (char*)("Options:"));
    /* pass */
    printf("%s\n", (char*)("  --version         Print version and exit"));
    /* pass */
    printf("%s\n", (char*)("  --emit c          Emit generated C code to build/"));
    /* pass */
    printf("%s\n", (char*)("  --emit ast        Emit AST representation and stop"));
    /* pass */
    printf("%s\n", (char*)("  --emit mir        Emit MIR basic blocks and stop"));
    /* pass */
    printf("%s\n", (char*)("  --run             Compile and immediately execute"));
    /* pass */
    printf("%s\n", (char*)("  --check           Run semantic analysis only (no codegen)"));
    /* pass */
    printf("%s\n", (char*)("  --verbose         Show all pipeline phases"));
    /* pass */
    printf("%s\n", (char*)("  --backend llvm    Use LLVM IR backend instead of C"));
    /* pass */
    printf("%s\n", (char*)("  -o <path>         Output executable name (temp .c files are deleted)"));
    /* pass */
    printf("%s\n", (char*)("  -O0/-O1/-O2/-O3  Optimization level (default: -O2)"));
    /* pass */
    printf("%s\n", (char*)("  -Os               Optimize for size"));
    /* pass */
    printf("%s\n", (char*)("  --link <path>     Link a file by path (.c .o .a .dll .lib .so)"));
    /* pass */
    printf("%s\n", (char*)("  -l<name>          Link a library by name (e.g. -luser32, -lgdi32)"));
    /* pass */
    printf("%s\n", (char*)("  -l <name>         Same as -l<name> with a space"));
    /* pass */
    printf("%s\n", (char*)("  --static          Statically link the output binary"));
    /* pass */
    printf("%s\n", (char*)("  --target <name>   Cross-compile for a target platform:"));
    /* pass */
    printf("%s\n", (char*)("                      android-arm64, android-arm32, android-x86_64, android-x86"));
    /* pass */
    printf("%s\n", (char*)("                      ios, ios-sim"));
    /* pass */
    printf("%s\n", (char*)("                      linux-arm64, linux-arm32, linux-x86_64, linux-riscv64"));
    /* pass */
    printf("%s\n", (char*)("                      windows-x64, windows-arm64"));
    /* pass */
    printf("%s\n", (char*)("                      macos-arm64, macos-x86_64"));
    /* pass */
    printf("%s\n", (char*)("                      embedded-arm, embedded-arm64, embedded-riscv32, embedded-riscv64"));
    /* pass */
    printf("%s\n", (char*)("                      wasm, wasm-wasi"));
    /* pass */
    printf("%s\n", (char*)("                    Or pass a raw LLVM triple (e.g. aarch64-linux-gnu)"));
    /* pass */
    printf("%s\n", (char*)("  --sysroot <path>  Override sysroot for the cross-compiler"));
    /* pass */
    printf("%s\n", (char*)("  --debug           Compile with ASAN and bounds-check assertions"));
    /* pass */
    printf("%s\n", (char*)("  --strict          Treat alloc/dealloc outside 'unsafe:' as a hard error [U-1]"));
    /* pass */
    printf("%s\n", (char*)("Bilingual: all English keywords have Hausa equivalents (aiki=def, aji=class, ...)"));
}

__attribute__((hot)) bool str_ends_with_dot_tr(char* path) {
    /* pass */
    char* p = ((char*)(path));
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

__attribute__((hot)) char* strip_extension(char* path) {
    /* pass */
    char* p = ((char*)(path));
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
            return _tr_str_slice(path, 0LL, (end - 1LL));
        }
        /* pass */
        end = (end - 1LL);
    }
    /* pass */
    return path;
}

__attribute__((hot)) bool str_starts_with(char* s, char* prefix) {
    /* pass */
    char* sp = ((char*)(s));
    /* pass */
    char* pp = ((char*)(prefix));
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

__attribute__((hot)) char* detect_c_compiler() {
    /* pass */
    char* null_dev = "/dev/null";
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        null_dev = "nul";
    }
    /* pass */
    if ((_tr_system(_tr_str_concat(_tr_str_concat("gcc --version >", null_dev), " 2>&1")) == 0LL)) {
        /* pass */
        return "gcc";
    }
    /* pass */
    if ((_tr_system(_tr_str_concat(_tr_str_concat("clang --version >", null_dev), " 2>&1")) == 0LL)) {
        /* pass */
        return "clang";
    }
    /* pass */
    if ((_tr_system(_tr_str_concat(_tr_str_concat("cc --version >", null_dev), " 2>&1")) == 0LL)) {
        /* pass */
        return "cc";
    }
    /* pass */
    return "gcc";
}

__attribute__((hot)) bool is_clang_compiler(char* cc) {
    /* pass */
    char* null_dev = "/dev/null";
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        null_dev = "nul";
    }
    /* pass */
    if (_tr_str_contains(cc, "clang")) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_str_eq(cc, "cc")) {
        /* pass */
        return (_tr_system(_tr_str_concat(cc, " --version 2>&1 | grep -qi clang")) == 0LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) char* resolve_target_triple(char* target) {
    /* pass */
    if ((strcmp((char*)target, (char*)"android-arm64") == 0)) {
        /* pass */
        return "aarch64-linux-android34";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"android-arm32") == 0)) {
        /* pass */
        return "armv7a-linux-androideabi34";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"android-x86_64") == 0)) {
        /* pass */
        return "x86_64-linux-android34";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"android-x86") == 0)) {
        /* pass */
        return "i686-linux-android34";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"ios") == 0)) {
        /* pass */
        return "aarch64-apple-ios";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"ios-sim") == 0)) {
        /* pass */
        return "aarch64-apple-ios-simulator";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"linux-arm64") == 0)) {
        /* pass */
        return "aarch64-linux-gnu";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"linux-arm32") == 0)) {
        /* pass */
        return "armv7-linux-gnueabihf";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"linux-x86_64") == 0)) {
        /* pass */
        return "x86_64-linux-gnu";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"linux-riscv64") == 0)) {
        /* pass */
        return "riscv64-linux-gnu";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"windows-x64") == 0)) {
        /* pass */
        return "x86_64-w64-mingw32";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"windows-arm64") == 0)) {
        /* pass */
        return "aarch64-w64-mingw32";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"macos-arm64") == 0)) {
        /* pass */
        return "aarch64-apple-macosx12.0";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"macos-x86_64") == 0)) {
        /* pass */
        return "x86_64-apple-macosx12.0";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"embedded-arm") == 0)) {
        /* pass */
        return "arm-none-eabi";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"embedded-arm64") == 0)) {
        /* pass */
        return "aarch64-none-elf";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"embedded-riscv32") == 0)) {
        /* pass */
        return "riscv32-unknown-elf";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"embedded-riscv64") == 0)) {
        /* pass */
        return "riscv64-unknown-elf";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"wasm") == 0)) {
        /* pass */
        return "wasm32-unknown-unknown";
    }
    /* pass */
    if ((strcmp((char*)target, (char*)"wasm-wasi") == 0)) {
        /* pass */
        return "wasm32-wasi";
    }
    /* pass */
    return target;
}

__attribute__((hot)) char* target_extra_flags(char* triple) {
    /* pass */
    if (_tr_str_contains(triple, "none-eabi")) {
        /* pass */
        return " -nostdlib -freestanding -ffreestanding -DTAURARO_NO_OS=1";
    }
    /* pass */
    if (_tr_str_contains(triple, "none-elf")) {
        /* pass */
        return " -nostdlib -freestanding -ffreestanding -DTAURARO_NO_OS=1";
    }
    /* pass */
    if (_tr_str_contains(triple, "unknown-elf")) {
        /* pass */
        return " -nostdlib -freestanding -ffreestanding -DTAURARO_NO_OS=1";
    }
    /* pass */
    if (_tr_str_contains(triple, "wasm32-unknown-unknown")) {
        /* pass */
        return " -nostdlib --no-standard-libraries -DTAURARO_WASM=1 -DTAURARO_NO_OS=1";
    }
    /* pass */
    if (_tr_str_contains(triple, "wasm32-wasi")) {
        /* pass */
        return " -DTAURARO_WASM=1";
    }
    /* pass */
    if (_tr_str_contains(triple, "mingw")) {
        /* pass */
        return " -static";
    }
    /* pass */
    return "";
}

__attribute__((hot)) char* detect_cross_compiler(char* triple) {
    /* pass */
    char* null_dev = "/dev/null";
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        null_dev = "nul";
    }
    /* pass */
    if (_tr_str_contains(triple, "android")) {
        /* pass */
        char* ndk = _tr_getenv("ANDROID_NDK_ROOT");
        /* pass */
        if ((strcmp((char*)ndk, (char*)"") == 0)) {
            /* pass */
            ndk = _tr_getenv("ANDROID_NDK_HOME");
        }
        /* pass */
        if ((strcmp((char*)ndk, (char*)"") == 0)) {
            /* pass */
            ndk = _tr_getenv("NDK_HOME");
        }
        /* pass */
        if ((strcmp((char*)ndk, (char*)"") != 0)) {
            /* pass */
            char* wrapper = _tr_str_concat(_tr_str_concat(_tr_str_concat(ndk, "/toolchains/llvm/prebuilt/linux-x86_64/bin/"), triple), "-clang");
            /* pass */
            if ((_tr_system(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("\"", wrapper), "\" --version >"), null_dev), " 2>&1")) == 0LL)) {
                /* pass */
                return wrapper;
            }
        }
    }
    /* pass */
    if ((_tr_system(_tr_str_concat(_tr_str_concat("clang --version >", null_dev), " 2>&1")) == 0LL)) {
        /* pass */
        return "clang";
    }
    /* pass */
    return detect_c_compiler();
}

__attribute__((hot)) char* dir_of_path(char* path) {
    /* pass */
    char* p = ((char*)(path));
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
            return _tr_str_slice(path, 0LL, end);
        }
        /* pass */
        end = (end - 1LL);
    }
    /* pass */
    return "./";
}

__attribute__((hot)) char* strip_trailing_sep_inline(char* s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(s));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        return s;
    }
    /* pass */
    long long last = ((long long)((*(p + (n - 1LL)))));
    /* pass */
    if (((last == 47LL) || (last == 92LL))) {
        /* pass */
        return _tr_str_slice(s, 0LL, (n - 1LL));
    }
    /* pass */
    return s;
}

__attribute__((hot)) char* read_runtime_header(char* bin_path, char* input_path) {
    /* pass */
    if (file_exists("tauraro/runtime/tauraro_rt.h")) {
        /* pass */
        return read_file("tauraro/runtime/tauraro_rt.h");
    }
    /* pass */
    char* bin_dir = _tr_str_concat(_tr_exe_dir(), "/");
    /* pass */
    char* src1 = _tr_str_concat(bin_dir, "tauraro_rt.h");
    /* pass */
    if (file_exists(src1)) {
        /* pass */
        return read_file(src1);
    }
    /* pass */
    char* src1b = _tr_str_concat(bin_dir, "runtime/tauraro_rt.h");
    /* pass */
    if (file_exists(src1b)) {
        /* pass */
        return read_file(src1b);
    }
    /* pass */
    if ((strcmp((char*)input_path, (char*)"") != 0)) {
        /* pass */
        char* src_dir = dir_of_path(input_path);
        /* pass */
        if (file_exists(_tr_str_concat(src_dir, "tauraro_rt.h"))) {
            /* pass */
            return read_file(_tr_str_concat(src_dir, "tauraro_rt.h"));
        }
        /* pass */
        char* parent = dir_of_path(strip_trailing_sep_inline(src_dir));
        /* pass */
        if (file_exists(_tr_str_concat(parent, "runtime/tauraro_rt.h"))) {
            /* pass */
            return read_file(_tr_str_concat(parent, "runtime/tauraro_rt.h"));
        }
        /* pass */
        if (file_exists(_tr_str_concat(parent, "tauraro_rt.h"))) {
            /* pass */
            return read_file(_tr_str_concat(parent, "tauraro_rt.h"));
        }
        /* pass */
        char* gp = dir_of_path(strip_trailing_sep_inline(parent));
        /* pass */
        if (file_exists(_tr_str_concat(gp, "runtime/tauraro_rt.h"))) {
            /* pass */
            return read_file(_tr_str_concat(gp, "runtime/tauraro_rt.h"));
        }
        /* pass */
        if (file_exists(_tr_str_concat(gp, "tauraro_rt.h"))) {
            /* pass */
            return read_file(_tr_str_concat(gp, "tauraro_rt.h"));
        }
    }
    /* pass */
    if (file_exists("runtime/tauraro_rt.h")) {
        /* pass */
        return read_file("runtime/tauraro_rt.h");
    }
    /* pass */
    return "";
}

__attribute__((hot)) void ensure_runtime_header(char* out_dir, char* bin_path, char* input_path) {
    /* pass */
    char* dest = _tr_str_concat(out_dir, "tauraro_rt.h");
    /* pass */
    if (file_exists(dest)) {
        /* pass */
        return;
    }
    /* pass */
    char* content = read_runtime_header(bin_path, input_path);
    /* pass */
    if ((strcmp((char*)content, (char*)"") != 0)) {
        /* pass */
        write_file(dest, content);
    }
}

__attribute__((hot)) void sync_headers_to_runtime(char* rt_content, char* types_content) {
    /* pass */
    write_file("tauraro/runtime/tauraro_rt.h", rt_content);
}

__attribute__((hot)) char* strip_trailing_sep(char* s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(s));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        return s;
    }
    /* pass */
    long long last = ((long long)((*(p + (n - 1LL)))));
    /* pass */
    if (((last == 47LL) || (last == 92LL))) {
        /* pass */
        return _tr_str_slice(s, 0LL, (n - 1LL));
    }
    /* pass */
    return s;
}

__attribute__((hot)) long long count_path_env_entries(char* s) {
    /* pass */
    long long sep = 58LL;
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        sep = 59LL;
    }
    /* pass */
    char* p = ((char*)(s));
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

__attribute__((hot)) char* get_path_env_entry(char* s, long long idx) {
    /* pass */
    long long sep = 58LL;
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        sep = 59LL;
    }
    /* pass */
    char* p = ((char*)(s));
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
                    return _tr_str_slice(s, start, i);
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
    return "";
}

__attribute__((hot)) char* path_to_native(char* s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(s));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    __auto_type buf = _tr_str_slice(s, 0LL, n);
    /* pass */
    char* bp = ((char*)(buf));
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        if ((((long long)((*(bp + j)))) == 47LL)) {
            /* pass */
            (*(bp + j) = ((char)(92LL)));
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    return buf;
}

__attribute__((hot)) char* dot_to_safe(char* s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(s));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    __auto_type buf = _tr_str_slice(s, 0LL, n);
    /* pass */
    char* bp = ((char*)(buf));
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        if ((((long long)((*(bp + j)))) == 46LL)) {
            /* pass */
            (*(bp + j) = ((char)(95LL)));
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    return buf;
}

__attribute__((hot)) char* dot_last_seg(char* s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(s));
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
        return s;
    }
    /* pass */
    return _tr_str_slice(s, (last_dot + 1LL), n);
}

__attribute__((hot)) char* get_filename(char* path) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(path));
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
        return path;
    }
    /* pass */
    return _tr_str_slice(path, (last_sep + 1LL), n);
}

__attribute__((hot)) long long get_dot_depth(char* dot_path) {
    /* pass */
    char* p = ((char*)(dot_path));
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

__attribute__((hot)) char* ensure_builtin_dirs(char* build_dir, char* dot_path) {
    /* pass */
    char* current = _tr_str_concat(build_dir, "include/");
    /* pass */
    make_dir(current);
    /* pass */
    char* p = ((char*)(dot_path));
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
            __auto_type last_seg = _tr_str_slice(dot_path, start, i);
            /* pass */
            return _tr_str_concat(_tr_str_concat(current, last_seg), ".c");
        }
        /* pass */
        if ((c == 46LL)) {
            /* pass */
            __auto_type seg = _tr_str_slice(dot_path, start, i);
            /* pass */
            current = _tr_str_concat(_tr_str_concat(current, seg), "/");
            /* pass */
            make_dir(current);
            /* pass */
            start = (i + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return "";
}

__attribute__((hot)) bool is_builtin_mod(char* dot_path) {
    /* pass */
    char* p = ((char*)(dot_path));
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

__attribute__((hot)) void make_dir(char* path) {
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("mkdir \"", path_to_native(path)), "\" 2>nul >nul"));
    } else {
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("mkdir -p \"", path), "\" 2>/dev/null"));
    }
}

__attribute__((hot)) long long compile_all_c(List_str* c_files, char* exe_path, char* inc_dir, List_str* link_paths, List_str* lib_flags, char* opt_level, bool verbose, bool static_link, char* target, char* sysroot, bool debug_mode) {
    /* pass */
    char* cc = detect_c_compiler();
    /* pass */
    char* triple = "";
    /* pass */
    char* cross_flags = "";
    /* pass */
    if ((strcmp((char*)target, (char*)"") != 0)) {
        /* pass */
        triple = resolve_target_triple(target);
        /* pass */
        cc = detect_cross_compiler(triple);
        /* pass */
        cross_flags = _tr_str_concat(" --target=", triple);
        /* pass */
        cross_flags = _tr_str_concat(cross_flags, target_extra_flags(triple));
        /* pass */
        if ((strcmp((char*)sysroot, (char*)"") != 0)) {
            /* pass */
            cross_flags = _tr_str_concat(_tr_str_concat(_tr_str_concat(cross_flags, " --sysroot=\""), sysroot), "\"");
        }
    }
    /* pass */
    char* static_flag = "";
    /* pass */
    if (static_link) {
        /* pass */
        static_flag = " -static";
    }
    /* pass */
    char* warn_flags = " -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value";
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        warn_flags = _tr_str_concat(warn_flags, " -Wno-unknown-attributes -Wno-parentheses-equality");
    }
    /* pass */
    char* native_flags = "";
    /* pass */
    if (((strcmp((char*)target, (char*)"") == 0) && (strcmp((char*)opt_level, (char*)"3") == 0))) {
        /* pass */
        native_flags = " -march=native -funroll-loops";
    }
    /* pass */
    char* overflow_flag = "";
    /* pass */
    if (((strcmp((char*)opt_level, (char*)"3") != 0) && (strcmp((char*)opt_level, (char*)"s") != 0))) {
        /* pass */
        overflow_flag = " -ftrapv";
    }
    /* pass */
    char* cmd = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(cc, " -O"), opt_level), overflow_flag), static_flag), native_flags), cross_flags), warn_flags), " -DTAURARO_NO_RT_HELPERS \"-I"), inc_dir), "\" -o \""), exe_path), "\"");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c_files->len)) {
        /* pass */
        cmd = _tr_str_concat(_tr_str_concat(_tr_str_concat(cmd, " \""), List_str_get(c_files, i)), "\"");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < link_paths->len)) {
        /* pass */
        cmd = _tr_str_concat(_tr_str_concat(_tr_str_concat(cmd, " \""), List_str_get(link_paths, i)), "\"");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < lib_flags->len)) {
        /* pass */
        cmd = _tr_str_concat(_tr_str_concat(cmd, " "), List_str_get(lib_flags, i));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    cmd = _tr_str_concat(cmd, " -lm");
    /* pass */
    if (debug_mode) {
        /* pass */
        cmd = _tr_str_concat(cmd, " -fsanitize=address,undefined -g");
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
            char* cf = List_str_get(c_files, wi);
            /* pass */
            if ((_tr_str_contains(cf, "tcp.c") || _tr_str_contains(cf, "/net/"))) {
                /* pass */
                need_wsa = true;
            }
            /* pass */
            wi = (wi + 1LL);
        }
        /* pass */
        if (need_wsa) {
            /* pass */
            cmd = _tr_str_concat(cmd, " -lws2_32 -mconsole");
        }
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat("  [CC] ", cmd)));
    }
    /* pass */
    return _tr_system(cmd);
}

__attribute__((hot)) long long compile_c_to_exe(char* c_path, char* exe_path, char* opt_level, bool verbose) {
    /* pass */
    char* cc = detect_c_compiler();
    /* pass */
    char* opt_flag = _tr_str_concat("-O", opt_level);
    /* pass */
    char* out_dir = strip_trailing_sep(dir_of_path(c_path));
    /* pass */
    char* warn_flags = " -Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value";
    /* pass */
    if (is_clang_compiler(cc)) {
        /* pass */
        warn_flags = _tr_str_concat(warn_flags, " -Wno-unknown-attributes -Wno-parentheses-equality");
    }
    /* pass */
    char* cmd = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(cc, " "), opt_flag), warn_flags), " -DTAURARO_NO_RT_HELPERS \"-I"), out_dir), "\" -o \""), exe_path), "\" \""), c_path), "\" -lm");
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat("  [CC] ", cmd)));
    }
    /* pass */
    return _tr_system(cmd);
}

__attribute__((hot)) void _print_diag(char* level, char* msg) {
    /* pass */
    char* fix_part = "";
    /* pass */
    char* main_part = msg;
    /* pass */
    long long fix_idx = _tr_str_index_of(msg, "FIX:");
    /* pass */
    if ((fix_idx >= 0LL)) {
        /* pass */
        char* _mp = _tr_str_slice(msg, 0LL, fix_idx);
        /* pass */
        main_part = _tr_str_strip(_mp);
        /* pass */
        char* _fp = _tr_str_slice(msg, fix_idx, _tr_strlen((char*)msg));
        /* pass */
        fix_part = _tr_str_strip(_fp);
    }
    /* pass */
    printf("%s\n", (char*)(_tr_str_concat(_tr_str_concat(level, ": "), main_part)));
    /* pass */
    if ((_tr_strlen((char*)fix_part) > 0LL)) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat("       ", fix_part)));
    }
}

__attribute__((hot)) void cleanup_build(char* build_dir, List_str* all_c_files) {
    /* pass */
    long long di = 0LL;
    /* pass */
    while ((di < all_c_files->len)) {
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            _tr_system(_tr_str_concat(_tr_str_concat("del /Q \"", path_to_native(List_str_get(all_c_files, di))), "\" 2>nul >nul"));
        } else {
            /* pass */
            _tr_system(_tr_str_concat(_tr_str_concat("rm -f \"", List_str_get(all_c_files, di)), "\""));
        }
        /* pass */
        di = (di + 1LL);
    }
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("del /Q \"", path_to_native(_tr_str_concat(build_dir, "tauraro_types.h"))), "\" 2>nul >nul"));
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("del /Q \"", path_to_native(_tr_str_concat(build_dir, "tauraro_rt.h"))), "\" 2>nul >nul"));
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("rmdir /S /Q \"", path_to_native(_tr_str_concat(build_dir, "include"))), "\" 2>nul >nul"));
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("rmdir \"", path_to_native(build_dir)), "\" 2>nul >nul"));
    } else {
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("rm -f \"", build_dir), "tauraro_types.h\""));
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("rm -f \"", build_dir), "tauraro_rt.h\""));
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("rm -rf \"", build_dir), "include\""));
        /* pass */
        _tr_system(_tr_str_concat(_tr_str_concat("rmdir \"", build_dir), "\" 2>/dev/null"));
    }
}

__attribute__((hot)) int main(int argc, char** argv) {
    _tr_argc = argc; _tr_argv = argv;
    _tr_init_console();
#ifndef TAURARO_BARE
    _tr_global_async_pool = _tr_threadpool_auto();
#endif
    List_str* args = List_str_new();
    for (int _ai = 0; _ai < argc; _ai++) { List_str_append(args, argv[_ai]); }
    /* pass */
    if ((args->len < 2LL)) {
        /* pass */
        print_usage();
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (((args->len == 2LL) && (strcmp((char*)List_str_get(args, 1LL), (char*)"--version") == 0))) {
        /* pass */
        print_version();
        /* pass */
        exit((int)(0LL));
    }
    /* pass */
    char* input_path = "";
    /* pass */
    char* output_path = "";
    /* pass */
    char* backend = "c";
    /* pass */
    char* emit_mode = "exe";
    /* pass */
    bool run_after = false;
    /* pass */
    bool check_only = false;
    /* pass */
    bool verbose = false;
    /* pass */
    char* opt_level = "2";
    /* pass */
    List_str* link_paths = (void*)List_str_new();
    /* pass */
    List_str* lib_flags = (void*)List_str_new();
    /* pass */
    bool static_link = false;
    /* pass */
    char* target = "";
    /* pass */
    char* sysroot = "";
    /* pass */
    bool debug_mode = false;
    /* pass */
    bool strict_mode = false;
    /* pass */
    long long i = 1LL;
    /* pass */
    while ((i < args->len)) {
        /* pass */
        char* arg = List_str_get(args, i);
        /* pass */
        if ((strcmp((char*)arg, (char*)"--version") == 0)) {
            /* pass */
            print_version();
            /* pass */
            exit((int)(0LL));
        } else if ((strcmp((char*)arg, (char*)"--run") == 0)) {
            /* pass */
            run_after = true;
        } else if ((strcmp((char*)arg, (char*)"--check") == 0)) {
            /* pass */
            check_only = true;
        } else if ((strcmp((char*)arg, (char*)"--verbose") == 0)) {
            /* pass */
            verbose = true;
        } else if (((strcmp((char*)arg, (char*)"--emit") == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            emit_mode = List_str_get(args, i);
        } else if (((strcmp((char*)arg, (char*)"--backend") == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            backend = List_str_get(args, i);
        } else if (((strcmp((char*)arg, (char*)"-o") == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            output_path = List_str_get(args, i);
        } else if (((strcmp((char*)arg, (char*)"--link") == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            List_str_append(link_paths, List_str_get(args, i));
        } else if (((strcmp((char*)arg, (char*)"-l") == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            List_str_append(lib_flags, StringObj_as_str(StringObj_init(_tr_str_concat("-l", List_str_get(args, i)))));
        } else if ((str_starts_with(arg, "-l") && (strcmp((char*)arg, (char*)"-l") != 0))) {
            /* pass */
            List_str_append(lib_flags, arg);
        } else if ((strcmp((char*)arg, (char*)"-O0") == 0)) {
            /* pass */
            opt_level = "0";
        } else if ((strcmp((char*)arg, (char*)"-O1") == 0)) {
            /* pass */
            opt_level = "1";
        } else if ((strcmp((char*)arg, (char*)"-O2") == 0)) {
            /* pass */
            opt_level = "2";
        } else if ((strcmp((char*)arg, (char*)"-O3") == 0)) {
            /* pass */
            opt_level = "3";
        } else if ((strcmp((char*)arg, (char*)"-Os") == 0)) {
            /* pass */
            opt_level = "s";
        } else if ((strcmp((char*)arg, (char*)"--static") == 0)) {
            /* pass */
            static_link = true;
        } else if (((strcmp((char*)arg, (char*)"--target") == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            target = List_str_get(args, i);
        } else if (((strcmp((char*)arg, (char*)"--sysroot") == 0) && ((i + 1LL) < args->len))) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            sysroot = List_str_get(args, i);
        } else if ((strcmp((char*)arg, (char*)"--debug") == 0)) {
            /* pass */
            debug_mode = true;
        } else if ((strcmp((char*)arg, (char*)"--strict") == 0)) {
            /* pass */
            strict_mode = true;
        } else if ((!str_starts_with(arg, "-"))) {
            /* pass */
            if ((strcmp((char*)input_path, (char*)"") == 0)) {
                /* pass */
                input_path = arg;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((strcmp((char*)input_path, (char*)"") == 0)) {
        /* pass */
        printf("%s\n", (char*)("error: no input file specified"));
        /* pass */
        print_usage();
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (((!file_exists(input_path)) && (!str_ends_with_dot_tr(input_path)))) {
        /* pass */
        input_path = _tr_str_concat(input_path, ".tr");
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat("[1/5] Resolving modules: ", input_path)));
    }
    /* pass */
    ModuleResolver* resolver = ModuleResolver_init();
    /* pass */
    char* bin_dir = strip_trailing_sep_inline(_tr_exe_dir());
    /* pass */
    ModuleResolver_add_search_path(resolver, bin_dir);
    /* pass */
    ModuleResolver_add_search_path(resolver, _tr_str_concat(bin_dir, "/std"));
    /* pass */
    ModuleResolver_add_search_path(resolver, _tr_str_concat(bin_dir, "/packages"));
    /* pass */
    ModuleResolver_add_search_path(resolver, _tr_str_concat(bin_dir, "/packages/sites"));
    /* pass */
    ModuleResolver_add_search_path(resolver, "packages");
    /* pass */
    ModuleResolver_add_search_path(resolver, "packages/sites");
    /* pass */
    char* tauraro_path_env = _tr_getenv("TAURARO_PATH");
    /* pass */
    if ((strcmp((char*)tauraro_path_env, (char*)"") != 0)) {
        /* pass */
        long long ep_count = count_path_env_entries(tauraro_path_env);
        /* pass */
        long long epi = 0LL;
        /* pass */
        while ((epi < ep_count)) {
            /* pass */
            ModuleResolver_add_search_path(resolver, get_path_env_entry(tauraro_path_env, epi));
            /* pass */
            epi = (epi + 1LL);
        }
    }
    /* pass */
    Program* prog = ModuleResolver_resolve_main(resolver, input_path);
    /* pass */
    if ((resolver->parse_errors > 0LL)) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat(_tr_str_concat("error: ", _tr_int_to_str((long long)(resolver->parse_errors))), " parse error(s); aborting compilation.")));
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if ((strcmp((char*)emit_mode, (char*)"ast") == 0)) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat("[AST] Declarations found: ", _tr_int_to_str((long long)(prog->decls->len)))));
        /* pass */
        return 0;
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", (char*)("[2/5] Semantic analysis..."));
    }
    /* pass */
    Sema* sema = Sema_init();
    /* pass */
    sema->strict_mode = strict_mode;
    /* pass */
    sema->current_file = input_path;
    /* pass */
    HirProgram* hir = Sema_analyze(sema, prog);
    /* pass */
    if ((sema->warnings->len > 0LL)) {
        /* pass */
        long long wk = 0LL;
        /* pass */
        while ((wk < sema->warnings->len)) {
            /* pass */
            _print_diag("warning", List_str_get(sema->warnings, wk));
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
            _print_diag("error", List_str_get(sema->errors, k));
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        exit((int)(1LL));
    }
    /* pass */
    if (check_only) {
        /* pass */
        printf("%s\n", (char*)("Check passed: no errors found."));
        /* pass */
        return 0;
    }
    /* pass */
    if ((strcmp((char*)emit_mode, (char*)"mir") == 0)) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat(_tr_str_concat("[MIR] ", _tr_int_to_str((long long)(hir->functions->len))), " functions lowered")));
        /* pass */
        return 0;
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat(_tr_str_concat("[3/5] Code generation (backend=", backend), ")...")));
    }
    /* pass */
    if ((strcmp((char*)backend, (char*)"llvm") == 0)) {
        /* pass */
        LlvmGenerator* llvm_gen = LlvmGenerator_init();
        /* pass */
        char* llvm_ir = LlvmGenerator_generate(llvm_gen, hir);
        /* pass */
        if ((strcmp((char*)output_path, (char*)"") == 0)) {
            /* pass */
            output_path = _tr_str_concat(strip_extension(input_path), ".ll");
        }
        /* pass */
        write_file(output_path, llvm_ir);
        /* pass */
        if (verbose) {
            /* pass */
            printf("%s\n", (char*)(_tr_str_concat("[4/5] LLVM IR written to ", output_path)));
        }
        /* pass */
        return 0;
    }
    /* pass */
    CGenerator* c_gen = CGenerator_init();
    /* pass */
    char* rt_h = read_runtime_header(List_str_get(args, 0LL), input_path);
    /* pass */
    CGenerator_register_program(c_gen, hir);
    /* pass */
    CGenerator_scan_mono_prog(c_gen, hir);
    /* pass */
    char* build_dir = "build/";
    /* pass */
    make_dir(build_dir);
    /* pass */
    if ((strcmp((char*)rt_h, (char*)"") != 0)) {
        /* pass */
        write_file(_tr_str_concat(build_dir, "tauraro_rt.h"), rt_h);
    }
    /* pass */
    char* types_h = CGenerator_generate_types_header(c_gen, hir);
    /* pass */
    types_h = _tr_str_concat(types_h, CGenerator_generate_module_compat(c_gen, resolver->all_decl_modules, resolver->all_decls));
    /* pass */
    write_file(_tr_str_concat(build_dir, "tauraro_types.h"), types_h);
    /* pass */
    if ((strcmp((char*)rt_h, (char*)"") != 0)) {
        /* pass */
        sync_headers_to_runtime(rt_h, types_h);
    }
    /* pass */
    List_str* all_c_files = (void*)List_str_new();
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < resolver->mod_dot_paths->len)) {
        /* pass */
        char* dot_path = List_str_get(resolver->mod_dot_paths, mi);
        /* pass */
        TrMap* class_set = _tr_dict_new(16LL);
        /* pass */
        TrMap* fn_set = _tr_dict_new(32LL);
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < resolver->all_decl_modules->len)) {
            /* pass */
            if ((strcmp((char*)List_str_get(resolver->all_decl_modules, k), (char*)dot_path) == 0)) {
                /* pass */
                __auto_type _t267 = (*((Decl*)List_ptr_get(resolver->all_decls, k)));
                if (_t267.tag == Decl_DClass) {
                    __auto_type c = _t267.data.DClass.cls;
                    /* pass */
                    _tr_dict_set(class_set, c->name, true);
                } else if (_t267.tag == Decl_DFunction) {
                    __auto_type f = _t267.data.DFunction.func;
                    /* pass */
                    _tr_dict_set(fn_set, f->name, true);
                } else if (_t267.tag == Decl_DEnum) {
                    __auto_type e = _t267.data.DEnum.enm;
                    /* pass */
                    _tr_dict_set(class_set, e->name, true);
                } else if (_t267.tag == Decl_DInterface) {
                    __auto_type iface = _t267.data.DInterface.iface;
                    /* pass */
                    _tr_dict_set(class_set, iface->name, true);
                } else if (_t267.tag == Decl_DExtend) {
                    __auto_type target = _t267.data.DExtend.target;
__auto_type methods = _t267.data.DExtend.methods;
                    /* pass */
                    _tr_dict_set(class_set, target, true);
                } else if (1) {
                    __auto_type _ = _t267;
                    /* pass */
                }
            }
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        char* c_path = "";
        /* pass */
        long long depth = 0LL;
        /* pass */
        if (is_builtin_mod(dot_path)) {
            /* pass */
            c_path = ensure_builtin_dirs(build_dir, dot_path);
            /* pass */
            depth = get_dot_depth(dot_path);
        } else {
            /* pass */
            c_path = _tr_str_concat(_tr_str_concat(_tr_str_concat(build_dir, "module_"), dot_to_safe(dot_path)), ".c");
            /* pass */
            depth = 0LL;
        }
        /* pass */
        char* mod_c = CGenerator_generate_module_c(c_gen, hir, class_set, fn_set, depth);
        /* pass */
        write_file(c_path, mod_c);
        /* pass */
        List_str_append(all_c_files, StringObj_as_str(StringObj_init(c_path)));
        /* pass */
        mi = (mi + 1LL);
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
        if ((strcmp((char*)List_str_get(resolver->all_decl_modules, k2), (char*)"") == 0)) {
            /* pass */
            __auto_type _t268 = (*((Decl*)List_ptr_get(resolver->all_decls, k2)));
            if (_t268.tag == Decl_DClass) {
                __auto_type c = _t268.data.DClass.cls;
                /* pass */
                _tr_dict_set(main_class_set, c->name, true);
            } else if (_t268.tag == Decl_DFunction) {
                __auto_type f = _t268.data.DFunction.func;
                /* pass */
                _tr_dict_set(main_fn_set, f->name, true);
            } else if (_t268.tag == Decl_DEnum) {
                __auto_type e = _t268.data.DEnum.enm;
                /* pass */
                _tr_dict_set(main_class_set, e->name, true);
            } else if (_t268.tag == Decl_DInterface) {
                __auto_type iface = _t268.data.DInterface.iface;
                /* pass */
                _tr_dict_set(main_class_set, iface->name, true);
            } else if (_t268.tag == Decl_DExtend) {
                __auto_type target = _t268.data.DExtend.target;
__auto_type methods = _t268.data.DExtend.methods;
                /* pass */
                _tr_dict_set(main_class_set, target, true);
            } else if (1) {
                __auto_type _ = _t268;
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
        _tr_dict_set(main_class_set, ((HirClass*)List_ptr_get(sema->nested_classes, nci))->name, true);
        /* pass */
        nci = (nci + 1LL);
    }
    /* pass */
    long long nfi = 0LL;
    /* pass */
    while ((nfi < sema->nested_functions->len)) {
        /* pass */
        _tr_dict_set(main_fn_set, ((HirFunction*)List_ptr_get(sema->nested_functions, nfi))->name, true);
        /* pass */
        nfi = (nfi + 1LL);
    }
    /* pass */
    long long nei = 0LL;
    /* pass */
    while ((nei < sema->nested_enums->len)) {
        /* pass */
        _tr_dict_set(main_class_set, ((HirEnum*)List_ptr_get(sema->nested_enums, nei))->name, true);
        /* pass */
        nei = (nei + 1LL);
    }
    /* pass */
    long long nii = 0LL;
    /* pass */
    while ((nii < sema->nested_interfaces->len)) {
        /* pass */
        _tr_dict_set(main_class_set, ((HirInterface*)List_ptr_get(sema->nested_interfaces, nii))->name, true);
        /* pass */
        nii = (nii + 1LL);
    }
    /* pass */
    char* main_c = CGenerator_generate_main_c(c_gen, hir, main_class_set, main_fn_set);
    /* pass */
    char* main_c_path = _tr_str_concat(build_dir, "main.c");
    /* pass */
    write_file(main_c_path, main_c);
    /* pass */
    List_str_append(all_c_files, StringObj_as_str(StringObj_init(main_c_path)));
    /* pass */
    if ((strcmp((char*)emit_mode, (char*)"c") == 0)) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat("Modular C output written to: ", build_dir)));
        /* pass */
        printf("%s\n", (char*)("  tauraro_types.h  - shared type definitions + all function prototypes"));
        /* pass */
        printf("%s\n", (char*)("  tauraro_rt.h     - runtime header"));
        /* pass */
        printf("%s\n", (char*)("  main.c           - program entry"));
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < (all_c_files->len - 1LL))) {
            /* pass */
            printf("%s\n", (char*)(_tr_str_concat("  ", get_filename(List_str_get(all_c_files, pi)))));
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        return 0;
    }
    /* pass */
    char* exe_name = "";
    /* pass */
    if ((strcmp((char*)output_path, (char*)"") != 0)) {
        /* pass */
        exe_name = output_path;
    } else {
        /* pass */
        exe_name = strip_extension(get_filename(input_path));
    }
    /* pass */
    if ((strcmp((char*)exe_name, (char*)"") == 0)) {
        /* pass */
        exe_name = "a";
    }
    /* pass */
    long long en_len = 0LL;
    /* pass */
    char* en_p = ((char*)(exe_name));
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
                        exe_name = _tr_str_slice(exe_name, 0LL, (en_len - 4LL));
                    }
                }
            }
        }
    }
    /* pass */
    char* exe_ext = ".exe";
    /* pass */
    if ((!_tr_is_windows())) {
        /* pass */
        exe_ext = "";
    }
    /* pass */
    char* exe_path = "";
    /* pass */
    if ((strcmp((char*)output_path, (char*)"") != 0)) {
        /* pass */
        exe_path = _tr_str_concat(exe_name, exe_ext);
    } else {
        /* pass */
        exe_path = _tr_str_concat(_tr_str_concat(build_dir, exe_name), exe_ext);
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat(_tr_str_concat(_tr_str_concat("[5/5] Linking ", _tr_int_to_str((long long)(all_c_files->len))), " modules -> "), exe_path)));
    }
    /* pass */
    long long rc = compile_all_c(all_c_files, exe_path, build_dir, link_paths, lib_flags, opt_level, verbose, static_link, target, sysroot, debug_mode);
    /* pass */
    if ((rc != 0LL)) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat(_tr_str_concat("error: compilation failed (exit code ", _tr_int_to_str((long long)(rc))), ")")));
        /* pass */
        exit((int)(rc));
    }
    /* pass */
    if ((strcmp((char*)output_path, (char*)"") != 0)) {
        /* pass */
        cleanup_build(build_dir, all_c_files);
    }
    /* pass */
    if (verbose) {
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat("Done: ", exe_path)));
    }
    /* pass */
    if (run_after) {
        /* pass */
        char* run_path = exe_path;
        /* pass */
        if (_tr_is_windows()) {
            /* pass */
            run_path = path_to_native(exe_path);
        }
        /* pass */
        long long run_rc = _tr_system(_tr_str_concat(_tr_str_concat("\"", run_path), "\""));
        /* pass */
        exit((int)(run_rc));
    }
#ifndef TAURARO_BARE
    if (_tr_global_async_pool) { _tr_threadpool_free(_tr_global_async_pool); _tr_global_async_pool = NULL; }
#endif
    return 0;
}
