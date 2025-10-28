//! C Transpiler for Tauraro
//!
//! This module transpiles Tauraro IR to C code and optionally compiles to executable.
//! It provides comprehensive support for all Python-compatible features including:
//! - Built-in types (int, float, str, bool, list, dict, tuple, set, etc.)
//! - Object-oriented programming (classes, inheritance, methods)
//! - Built-in functions (print, len, str, int, etc.)
//! - Operators and expressions
//! - Control flow statements

pub mod types;
pub mod builtins;
pub mod oop;
pub mod runtime;
pub mod functions;
pub mod expressions;
pub mod statements;
pub mod compiler;
pub mod imports;

use crate::codegen::{CodeGenerator, CodegenOptions, Target};
use crate::ir::{IRModule, IRFunction, IRInstruction, IRTypeInfo};
use crate::ast::Type;
use anyhow::Result;
use std::collections::HashSet;
use std::path::Path;

/// C Transpiler that converts Tauraro IR to C code
pub struct CTranspiler {
    target: Target,
}

impl CTranspiler {
    pub fn new() -> Self {
        Self {
            target: Target::C,
        }
    }

    /// Generate C header includes
    fn generate_headers(&self) -> String {
        let mut headers = String::new();
        headers.push_str("#include <stdio.h>\n");
        headers.push_str("#include <stdlib.h>\n");
        headers.push_str("#include <string.h>\n");
        headers.push_str("#include <stdbool.h>\n");
        headers.push_str("#include <stdint.h>\n");
        headers.push_str("#include <math.h>\n");
        headers.push_str("#include <ctype.h>\n");
        headers.push_str("\n");
        headers
    }

    /// Generate extern declarations for builtin modules
    fn generate_builtin_extern_declarations(&self, module_name: &str) -> String {
        let mut decls = String::new();

        match module_name {
            "math" => {
                decls.push_str("// Math module - extern declarations\n");
                decls.push_str("extern double tauraro_math_pi;\n");
                decls.push_str("extern double tauraro_math_e;\n");
                decls.push_str("extern tauraro_value_t* tauraro_math_sqrt(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_math_pow(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_math_sin(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_math_cos(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_math_tan(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_math_log(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_math_exp(int argc, tauraro_value_t** argv);\n");
            }
            "sys" => {
                decls.push_str("// Sys module - extern declarations\n");
                decls.push_str("extern const char* tauraro_sys_platform;\n");
                decls.push_str("extern const char* tauraro_sys_version;\n");
                decls.push_str("extern void tauraro_sys_exit(int argc, tauraro_value_t** argv) __attribute__((noreturn));\n");
            }
            "os" => {
                decls.push_str("// OS module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_os_getcwd(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_os_listdir(int argc, tauraro_value_t** argv);\n");
            }
            "time" => {
                decls.push_str("// Time module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_time_time(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_time_sleep(int argc, tauraro_value_t** argv);\n");
            }
            "random" => {
                decls.push_str("// Random module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_random_random(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_random_randint(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_random_seed(int argc, tauraro_value_t** argv);\n");
            }
            "json" => {
                decls.push_str("// JSON module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_json_loads(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_json_dumps(int argc, tauraro_value_t** argv);\n");
            }
            "re" => {
                decls.push_str("// Regex module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_re_search(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_re_match(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_re_findall(int argc, tauraro_value_t** argv);\n");
            }
            "io" => {
                decls.push_str("// IO module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_io_open(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_io_read(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_io_write(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_io_close(int argc, tauraro_value_t** argv);\n");
            }
            "datetime" => {
                decls.push_str("// Datetime module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_datetime_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_datetime_now(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_datetime_utcnow(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_date_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_date_today(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_time_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_timedelta_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern long long tauraro_datetime_MINYEAR;\n");
                decls.push_str("extern long long tauraro_datetime_MAXYEAR;\n");
            }
            "collections" => {
                decls.push_str("// Collections module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_collections_deque(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_collections_counter(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_collections_defaultdict(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_collections_highperflist(int argc, tauraro_value_t** argv);\n");
            }
            "itertools" => {
                decls.push_str("// Itertools module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_chain(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_combinations(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_permutations(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_product(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_repeat(int argc, tauraro_value_t** argv);\n");
            }
            "functools" => {
                decls.push_str("// Functools module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_functools_partial(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_functools_reduce(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_functools_lru_cache(int argc, tauraro_value_t** argv);\n");
            }
            "threading" => {
                decls.push_str("// Threading module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_threading_thread_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_threading_lock_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_threading_rlock_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_threading_semaphore_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_threading_event_new(int argc, tauraro_value_t** argv);\n");
            }
            "copy" => {
                decls.push_str("// Copy module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_copy_copy(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_copy_deepcopy(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_copy_error_new(int argc, tauraro_value_t** argv);\n");
            }
            "base64" => {
                decls.push_str("// Base64 module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_b64encode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_b64decode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_standard_b64encode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_standard_b64decode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_urlsafe_b64encode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_urlsafe_b64decode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_b32encode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_b32decode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_b16encode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_b16decode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_b85encode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_b85decode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_encode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_decode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_encodebytes(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_base64_decodebytes(int argc, tauraro_value_t** argv);\n");
            }
            "hashlib" => {
                decls.push_str("// Hashlib module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_hashlib_md5(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_hashlib_sha1(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_hashlib_sha256(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_hashlib_sha512(int argc, tauraro_value_t** argv);\n");
            }
            "urllib" => {
                decls.push_str("// Urllib module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_urllib_parse_quote(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_urllib_parse_unquote(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_urllib_parse_urlencode(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_urllib_request_urlopen(int argc, tauraro_value_t** argv);\n");
            }
            "csv" => {
                decls.push_str("// CSV module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_reader(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_writer(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_dictreader(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_dictwriter(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_sniffer_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_register_dialect(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_unregister_dialect(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_get_dialect(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_list_dialects(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_field_size_limit(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_csv_error_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern long long tauraro_csv_QUOTE_ALL;\n");
                decls.push_str("extern long long tauraro_csv_QUOTE_MINIMAL;\n");
                decls.push_str("extern long long tauraro_csv_QUOTE_NONNUMERIC;\n");
                decls.push_str("extern long long tauraro_csv_QUOTE_NONE;\n");
            }
            "logging" => {
                decls.push_str("// Logging module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_logging_getlogger(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_logging_basicconfig(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_logging_debug(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_logging_info(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_logging_warning(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_logging_error(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_logging_critical(int argc, tauraro_value_t** argv);\n");
            }
            "unittest" => {
                decls.push_str("// Unittest module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_unittest_testcase_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_unittest_testsuite_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_unittest_testloader_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_unittest_texttestrunner_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_unittest_main(int argc, tauraro_value_t** argv);\n");
            }
            "socket" => {
                decls.push_str("// Socket module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_socket_socket_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_socket_gethostname(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_socket_gethostbyname(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_socket_inet_aton(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_socket_inet_ntoa(int argc, tauraro_value_t** argv);\n");
            }
            "asyncio" => {
                decls.push_str("// Asyncio module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_get_event_loop(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_new_event_loop(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_set_event_loop(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_run(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_run_until_complete(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_create_task(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_gather(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_wait_for(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_shield(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_wait(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_sleep(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_iscoroutine(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_iscoroutinefunction(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_isfuture(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_lock_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_event_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_semaphore_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_queue_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_cancelled_error_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_timeout_error_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_asyncio_invalid_state_error_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern const char* tauraro_asyncio_FIRST_COMPLETED;\n");
                decls.push_str("extern const char* tauraro_asyncio_FIRST_EXCEPTION;\n");
                decls.push_str("extern const char* tauraro_asyncio_ALL_COMPLETED;\n");
            }
            "httptools" => {
                decls.push_str("// Httptools module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_httptools_request_parser_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_httptools_response_parser_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_httptools_parse_url(int argc, tauraro_value_t** argv);\n");
            }
            "websockets" => {
                decls.push_str("// Websockets module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_websockets_connect(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_websockets_serve(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_websockets_send(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_websockets_recv(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_websockets_close(int argc, tauraro_value_t** argv);\n");
            }
            "httpx" => {
                decls.push_str("// Httpx module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_httpx_get(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_httpx_post(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_httpx_put(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_httpx_delete(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_httpx_client_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_httpx_asyncclient_new(int argc, tauraro_value_t** argv);\n");
            }
            "memory" => {
                decls.push_str("// Memory module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_memory_get_usage(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_memory_get_peak(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_memory_gc_collect(int argc, tauraro_value_t** argv);\n");
            }
            "gc" => {
                decls.push_str("// GC module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_gc_collect(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_gc_enable(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_gc_disable(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_gc_isenabled(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_gc_get_stats(int argc, tauraro_value_t** argv);\n");
            }
            "exceptions" => {
                decls.push_str("// Exceptions module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_exception_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_valueerror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_typeerror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_runtimeerror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_importerror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_keyerror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_indexerror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_attributeerror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_nameerror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_ioerror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_oserror_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_exceptions_zerodivisionerror_new(int argc, tauraro_value_t** argv);\n");
            }
            "abc" => {
                decls.push_str("// ABC module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_abc_abcmeta_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_abc_abc_new(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_abc_abstractmethod(int argc, tauraro_value_t** argv);\n");
            }
            "pickle" => {
                decls.push_str("// Pickle module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_pickle_dumps(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_pickle_loads(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_pickle_dump(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_pickle_load(int argc, tauraro_value_t** argv);\n");
            }
            "itertools" => {
                decls.push_str("// Itertools module - extern declarations\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_count(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_cycle(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_repeat(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_accumulate(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_chain(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_compress(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_dropwhile(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_filterfalse(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_groupby(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_islice(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_starmap(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_takewhile(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_tee(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_zip_longest(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_product(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_permutations(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_combinations(int argc, tauraro_value_t** argv);\n");
                decls.push_str("extern tauraro_value_t* tauraro_itertools_combinations_with_replacement(int argc, tauraro_value_t** argv);\n");
            }
            _ => {
                decls.push_str(&format!("// {} module - extern declarations\n", module_name));
                decls.push_str(&format!("// Note: Builtin module '{}' extern declarations not yet implemented\n", module_name));
            }
        }

        decls.push_str("\n");
        decls
    }

    /// Generate complete C code from IR module
    fn generate_c_code(&self, module: IRModule, output_dir: Option<&str>) -> Result<String> {
        use crate::codegen::c_transpiler::imports::{ImportAnalyzer, ModuleCompiler};
        use std::path::PathBuf;

        // Analyze imports in the IR module
        let mut analyzer = ImportAnalyzer::new();
        analyzer.analyze_ir(&module)?;

        // Check if there are any imports
        let has_imports = !analyzer.modules.is_empty();

        // Determine build directory based on imports
        let build_dir = if has_imports {
            // If there are imports, use build directory
            let dir = if let Some(d) = output_dir {
                PathBuf::from(d)
            } else {
                PathBuf::from("build")
            };
            std::fs::create_dir_all(&dir)?;

            // Create builtin subdirectory for builtin modules
            let builtin_dir = dir.join("builtin");
            std::fs::create_dir_all(&builtin_dir)?;

            Some(dir)
        } else {
            // No imports, output to current directory
            None
        };

        // Compile user-defined modules
        let mut user_module_headers = Vec::new();
        if let Some(ref dir) = build_dir {
            let mut module_compiler = ModuleCompiler::new(dir.clone());
            for user_module in analyzer.get_user_modules() {
                let (_c_path, h_path) = module_compiler.compile_module(user_module)?;
                user_module_headers.push(h_path);
            }
        }

        // Get builtin modules and generate extern declarations
        let builtin_modules = analyzer.get_builtin_modules();

        let mut c_code = String::new();

        // Add standard headers
        c_code.push_str(&self.generate_headers());

        // Add type definitions (only if not already defined in headers)
        if user_module_headers.is_empty() {
            c_code.push_str(&types::generate_type_definitions());
        } else {
            // If headers are included, types are already defined there
            c_code.push_str("#ifndef TAURARO_TYPES_DEFINED\n");
            c_code.push_str(&types::generate_type_definitions());
            c_code.push_str("#endif // TAURARO_TYPES_DEFINED\n\n");
        }

        // Add OOP structures (always included since builtins may reference them)
        if user_module_headers.is_empty() {
            c_code.push_str(&oop::generate_oop_structures());
        } else {
            c_code.push_str("#ifndef TAURARO_OOP_DEFINED\n");
            c_code.push_str(&oop::generate_oop_structures());
            c_code.push_str("#endif // TAURARO_OOP_DEFINED\n\n");
        }

        // Add user module headers (after types so they can use tauraro_value_t)
        for header_path in &user_module_headers {
            if let Some(header_name) = header_path.file_name() {
                c_code.push_str(&format!("#include \"{}\"\n", header_name.to_string_lossy()));
            }
        }
        c_code.push_str("\n");

        // Add extern declarations for builtin modules (after types are defined)
        if !builtin_modules.is_empty() {
            c_code.push_str("// Extern declarations for builtin modules (implemented in Rust)\n");
            for builtin in builtin_modules {
                c_code.push_str(&self.generate_builtin_extern_declarations(&builtin.name));
            }
            c_code.push_str("\n");
        }

        // Add type function declarations
        c_code.push_str(&types::generate_type_function_declarations());

        // Add OOP function declarations (always included for compatibility)
        c_code.push_str(&oop::generate_oop_declarations());

        // Analyze which builtins are used
        let used_builtins = self.analyze_used_builtins(&module);

        // Add builtin function declarations
        c_code.push_str(&builtins::generate_builtin_declarations(&used_builtins));

        // Add runtime function declarations
        c_code.push_str("// Runtime operators\n");
        c_code.push_str("tauraro_value_t* tauraro_add(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_sub(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_mul(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_div(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_mod(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_eq(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_ne(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_lt(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_le(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_gt(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_ge(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("\n");

        // Add optimized function declarations for typed operations
        c_code.push_str("// Optimized typed operations\n");
        c_code.push_str("int64_t tauraro_add_int(int64_t left, int64_t right);\n");
        c_code.push_str("double tauraro_add_float(double left, double right);\n");
        c_code.push_str("char* tauraro_add_string(char* left, char* right);\n");
        c_code.push_str("\n");

        // Add type utility implementations
        c_code.push_str(&types::generate_type_utility_functions());

        // Add OOP implementations (always included for compatibility)
        c_code.push_str(&oop::generate_oop_implementations());

        // Add builtin implementations
        if !used_builtins.is_empty() {
            c_code.push_str("// Builtin function implementations\n");
            for builtin in &used_builtins {
                c_code.push_str(&builtins::generate_builtin_implementation(builtin));
                c_code.push_str("\n");
            }
        }

        // Add runtime support implementations
        c_code.push_str(&runtime::generate_runtime_support());

        // Add optimized function implementations for typed operations
        c_code.push_str("// Optimized typed operation implementations\n");
        c_code.push_str("int64_t tauraro_add_int(int64_t left, int64_t right) {\n");
        c_code.push_str("    return left + right;\n");
        c_code.push_str("}\n\n");
        c_code.push_str("double tauraro_add_float(double left, double right) {\n");
        c_code.push_str("    return left + right;\n");
        c_code.push_str("}\n\n");
        c_code.push_str("char* tauraro_add_string(char* left, char* right) {\n");
        c_code.push_str("    size_t left_len = strlen(left);\n");
        c_code.push_str("    size_t right_len = strlen(right);\n");
        c_code.push_str("    char* result = malloc(left_len + right_len + 1);\n");
        c_code.push_str("    strcpy(result, left);\n");
        c_code.push_str("    strcat(result, right);\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        // Add global variables
        c_code.push_str("// Global variables\n");
        for instruction in &module.globals {
            match instruction {
                IRInstruction::StoreGlobal { name, value: _ } => {
                    c_code.push_str(&format!("tauraro_value_t* {};\n", name));
                }
                IRInstruction::StoreTypedGlobal { name, value: _, type_info: _ } => {
                    c_code.push_str(&format!("tauraro_value_t* {};\n", name));
                }
                _ => {}
            }
        }
        c_code.push_str("\n");

        // Generate functions
        for (_name, function) in &module.functions {
            c_code.push_str(&functions::generate_function(function)?);
            c_code.push_str("\n\n");
        }

        // Generate main function
        c_code.push_str(&self.generate_main_function(&module)?);

        Ok(c_code)
    }

    /// Generate main function
    fn generate_main_function(&self, module: &IRModule) -> Result<String> {
        let mut main_code = String::new();

        // Don't generate main if it already exists
        if module.functions.contains_key("main") {
            return Ok(main_code);
        }

        main_code.push_str("int main() {\n");

        // Track declared variables
        let mut declared_vars = HashSet::new();

        // Collect all variable names
        for instruction in &module.globals {
            match instruction {
                IRInstruction::LoadConst { result, .. } => {
                    declared_vars.insert(result.clone());
                }
                IRInstruction::LoadGlobal { result, .. } => {
                    declared_vars.insert(result.clone());
                }
                IRInstruction::LoadTypedGlobal { result, .. } => {
                    declared_vars.insert(result.clone());
                }
                IRInstruction::Call { result: Some(result), .. } => {
                    declared_vars.insert(result.clone());
                }
                IRInstruction::Call { func, args, result: None } => {
                    // For method calls with no result, we still need to track them
                    if func.contains("__") && !args.is_empty() {
                        declared_vars.insert(args[0].clone()); // First arg is self
                    }
                }
                IRInstruction::BinaryOp { result, .. } => {
                    declared_vars.insert(result.clone());
                }
                IRInstruction::TypedBinaryOp { result, .. } => {
                    declared_vars.insert(result.clone());
                }
                IRInstruction::ObjectCreate { result, .. } => {
                    declared_vars.insert(result.clone());
                }
                IRInstruction::ObjectGetAttr { result, .. } => {
                    declared_vars.insert(result.clone());
                }
                _ => {}
            }
        }

        // Declare all variables
        for var_name in &declared_vars {
            main_code.push_str(&format!("    tauraro_value_t* {} = NULL;\n", var_name));
        }

        // Execute global instructions
        for instruction in &module.globals {
            main_code.push_str(&format!("    {}\n", self.generate_global_instruction(instruction, &module.type_info)?));
        }

        // Call main_function if it exists
        if module.functions.contains_key("main_function") {
            main_code.push_str("    main_function();\n");
        }

        main_code.push_str("    return 0;\n");
        main_code.push_str("}\n");

        Ok(main_code)
    }

    /// Generate code for a global instruction
    fn generate_global_instruction(&self, instruction: &IRInstruction, type_info: &IRTypeInfo) -> Result<String> {
        use crate::value::Value;

        match instruction {
            IRInstruction::Comment(text) => {
                // Generate C comment
                Ok(format!("// {}", text))
            },
            IRInstruction::LoadConst { value, result } => {
                match value {
                    Value::Int(i) => {
                        Ok(format!("{} = tauraro_value_new(); {}->type = TAURARO_INT; {}->data.int_val = {};",
                            result, result, result, i))
                    }
                    Value::Float(f) => {
                        Ok(format!("{} = tauraro_value_new(); {}->type = TAURARO_FLOAT; {}->data.float_val = {};",
                            result, result, result, f))
                    }
                    Value::Str(s) => {
                        // Escape special characters in strings
                        let escaped = s
                            .replace("\\", "\\\\")
                            .replace("\"", "\\\"")
                            .replace("\n", "\\n")
                            .replace("\r", "\\r")
                            .replace("\t", "\\t");
                        Ok(format!("{} = tauraro_value_new(); {}->type = TAURARO_STRING; {}->data.str_val = strdup(\"{}\");",
                            result, result, result, escaped))
                    }
                    Value::Bool(b) => {
                        Ok(format!("{} = tauraro_value_new(); {}->type = TAURARO_BOOL; {}->data.bool_val = {};",
                            result, result, result, if *b { "true" } else { "false" }))
                    }
                    Value::None => {
                        Ok(format!("{} = tauraro_value_new(); {}->type = TAURARO_NONE;", result, result))
                    }
                    _ => {
                        Ok(format!("{} = tauraro_value_new(); {}->type = TAURARO_NONE; // Unsupported constant type",
                            result, result))
                    }
                }
            }
            IRInstruction::StoreGlobal { name, value } => {
                Ok(format!("{} = {};", name, value))
            }
            IRInstruction::StoreTypedGlobal { name, value, type_info: var_type } => {
                // For typed globals, we can potentially optimize based on the type
                match var_type {
                    Type::Simple(type_name) if type_name == "int" => {
                        Ok(format!("{} = {}; // Typed as int", name, value))
                    }
                    Type::Simple(type_name) if type_name == "float" => {
                        Ok(format!("{} = {}; // Typed as float", name, value))
                    }
                    Type::Simple(type_name) if type_name == "str" => {
                        Ok(format!("{} = {}; // Typed as str", name, value))
                    }
                    _ => {
                        Ok(format!("{} = {}; // Typed variable", name, value))
                    }
                }
            }
            IRInstruction::Call { func, args, result } => {
                // Check what kind of function this is
                if func.contains("__") {
                    // Could be a method call (class__method) or module function (module__function)
                    // Module functions from imports should use single underscore and skip first arg
                    // Check if this looks like a module function call
                    if !args.is_empty() && args[0].chars().all(|c| c.is_ascii_lowercase() || c == '_') {
                        // Likely a module function call: module__function(module, args...)
                        // Check if it's a builtin module
                        let parts: Vec<&str> = func.split("__").collect();
                        let is_builtin = parts.len() == 2 && matches!(parts[0],
                            "math" | "sys" | "os" | "time" | "random" | "json" | "re" |
                            "datetime" | "collections" | "itertools" | "functools");

                        let fixed_func = if is_builtin {
                            // Builtin module: convert math__sqrt to tauraro_math_sqrt
                            format!("tauraro_{}_{}", parts[0], parts[1])
                        } else {
                            // User module: convert mymath__square to mymath_square
                            func.replace("__", "_")
                        };

                        let actual_args = &args[1..]; // Skip the module argument

                        if actual_args.is_empty() {
                            match result {
                                Some(res) => Ok(format!("{} = {}(0, NULL);", res, fixed_func)),
                                None => Ok(format!("{}(0, NULL);", fixed_func))
                            }
                        } else {
                            let arg_list = actual_args.join(", ");
                            let args_str = format!("{}, (tauraro_value_t*[]){{{}}}", actual_args.len(), arg_list);
                            match result {
                                Some(res) => Ok(format!("{} = {}({});", res, fixed_func, args_str)),
                                None => Ok(format!("{}({});", fixed_func, args_str))
                            }
                        }
                    } else {
                        // Regular method call (class__method) - uses argc/argv convention
                        let args_str = if args.is_empty() {
                            "0, NULL".to_string()
                        } else {
                            let arg_list = args.join(", ");
                            format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_list)
                        };

                        match result {
                            Some(res) => {
                                if !args.is_empty() {
                                    Ok(format!("{} = {}({});", res, func, args_str))
                                } else {
                                    Ok(format!("{} = {}(0, NULL);", res, func))
                                }
                            },
                            None => {
                                if !args.is_empty() {
                                    Ok(format!("{}({});", func, args_str))
                                } else {
                                    Ok(format!("{}(0, NULL);", func))
                                }
                            }
                        }
                    }
                } else if func == "tauraro_super_call" {
                    // Handle super() call - uses argc/argv convention
                    let args_str = if args.is_empty() {
                        "0, NULL".to_string()
                    } else {
                        let arg_list = args.join(", ");
                        format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_list)
                    };

                    match result {
                        Some(res) => {
                            Ok(format!("{} = tauraro_super_call({});", res, args_str))
                        },
                        None => {
                            Ok(format!("tauraro_super_call({});", args_str))
                        }
                    }
                } else if func.contains("_") {
                    // Module function (module_function) - uses argc/argv convention
                    let args_str = if args.is_empty() {
                        "0, NULL".to_string()
                    } else {
                        let arg_list = args.join(", ");
                        format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_list)
                    };

                    match result {
                        Some(res) => Ok(format!("{} = {}({});", res, func, args_str)),
                        None => Ok(format!("{}({});", func, args_str))
                    }
                } else {
                    // Regular builtin function - uses argc/argv convention, add tauraro_ prefix
                    let args_str = if args.is_empty() {
                        "0, NULL".to_string()
                    } else {
                        let arg_list = args.join(", ");
                        format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_list)
                    };

                    match result {
                        Some(res) => Ok(format!("{} = tauraro_{}({});", res, func, args_str)),
                        None => Ok(format!("tauraro_{}({});", func, args_str))
                    }
                }
            }
            IRInstruction::ObjectCreate { class_name, result } => {
                Ok(format!("{} = tauraro_object_create(\"{}\");", result, class_name))
            }
            IRInstruction::ObjectSetAttr { object, attr, value } => {
                Ok(format!("tauraro_object_set_attr({}, \"{}\", {});", object, attr, value))
            }
            IRInstruction::ObjectGetAttr { object, attr, result } => {
                Ok(format!("{} = tauraro_object_get_attr({}, \"{}\");", result, object, attr))
            }
            IRInstruction::SuperCall { args, result } => {
                let args_str = if args.is_empty() {
                    "0, NULL".to_string()
                } else {
                    let arg_list = args.join(", ");
                    format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_list)
                };
                Ok(format!("{} = tauraro_super_call({});", result, args_str))
            }
            IRInstruction::BinaryOp { op, left, right, result } => {
                let op_func = match op {
                    crate::ast::BinaryOp::Add => "tauraro_add",
                    crate::ast::BinaryOp::Sub => "tauraro_sub",
                    crate::ast::BinaryOp::Mul => "tauraro_mul",
                    crate::ast::BinaryOp::Div => "tauraro_div",
                    crate::ast::BinaryOp::Mod => "tauraro_mod",
                    crate::ast::BinaryOp::Eq => "tauraro_eq",
                    crate::ast::BinaryOp::Ne => "tauraro_ne",
                    crate::ast::BinaryOp::Lt => "tauraro_lt",
                    crate::ast::BinaryOp::Le => "tauraro_le",
                    crate::ast::BinaryOp::Gt => "tauraro_gt",
                    crate::ast::BinaryOp::Ge => "tauraro_ge",
                    _ => "tauraro_add"  // Fallback
                };
                Ok(format!("{} = {}({}, {});", result, op_func, left, right))
            }
            IRInstruction::TypedBinaryOp { op, left, right, result, type_info: expr_type } => {
                // Generate optimized code based on type information
                match expr_type {
                    Type::Simple(type_name) if type_name == "int" => {
                        match op {
                            crate::ast::BinaryOp::Add => {
                                Ok(format!("{} = tauraro_value_new(); {}->type = TAURARO_INT; {}->data.int_val = tauraro_add_int({}->data.int_val, {}->data.int_val);", 
                                    result, result, result, left, right))
                            }
                            _ => {
                                // Fall back to generic operation for other operators
                                Ok(format!("{} = tauraro_add({}, {}); // Typed operation", result, left, right))
                            }
                        }
                    }
                    Type::Simple(type_name) if type_name == "float" => {
                        match op {
                            crate::ast::BinaryOp::Add => {
                                Ok(format!("{} = tauraro_value_new(); {}->type = TAURARO_FLOAT; {}->data.float_val = tauraro_add_float({}->data.float_val, {}->data.float_val);", 
                                    result, result, result, left, right))
                            }
                            _ => {
                                // Fall back to generic operation for other operators
                                Ok(format!("{} = tauraro_add({}, {}); // Typed operation", result, left, right))
                            }
                        }
                    }
                    Type::Simple(type_name) if type_name == "str" => {
                        match op {
                            crate::ast::BinaryOp::Add => {
                                Ok(format!("{} = tauraro_value_new(); {}->type = TAURARO_STRING; {}->data.str_val = tauraro_add_string({}->data.str_val, {}->data.str_val);", 
                                    result, result, result, left, right))
                            }
                            _ => {
                                // Fall back to generic operation for other operators
                                Ok(format!("{} = tauraro_add({}, {}); // Typed operation", result, left, right))
                            }
                        }
                    }
                    _ => {
                        // Fall back to generic operation for other types
                        let op_func = match op {
                            crate::ast::BinaryOp::Add => "tauraro_add",
                            crate::ast::BinaryOp::Sub => "tauraro_sub",
                            crate::ast::BinaryOp::Mul => "tauraro_mul",
                            crate::ast::BinaryOp::Div => "tauraro_div",
                            crate::ast::BinaryOp::Mod => "tauraro_mod",
                            crate::ast::BinaryOp::Eq => "tauraro_eq",
                            crate::ast::BinaryOp::Ne => "tauraro_ne",
                            crate::ast::BinaryOp::Lt => "tauraro_lt",
                            crate::ast::BinaryOp::Le => "tauraro_le",
                            crate::ast::BinaryOp::Gt => "tauraro_gt",
                            crate::ast::BinaryOp::Ge => "tauraro_ge",
                            _ => "tauraro_add"  // Fallback
                        };
                        Ok(format!("{} = {}({}, {}); // Typed operation", result, op_func, left, right))
                    }
                }
            }
            IRInstruction::LoadGlobal { name, result } => {
                // Special handling for class names
                // If there are functions with this name as a prefix, it's likely a class
                // In that case, we should create a string value with the class name
                Ok(format!("{} = {};", result, name))
            }
            IRInstruction::LoadTypedGlobal { name, result, type_info: _ } => {
                // For typed globals, we can potentially optimize based on the type
                Ok(format!("{} = {}; // Typed load", result, name))
            }
            IRInstruction::Import { module } => {
                Ok(format!("// Import module: {}", module))
            }
            IRInstruction::ImportFrom { module, names: _ } => {
                Ok(format!("// Import from module: {}", module))
            }
            _ => {
                Ok(format!("// Global instruction: {:?}", instruction))
            }
        }
    }

    /// Analyze which builtin functions are used in the module
    fn analyze_used_builtins(&self, module: &IRModule) -> HashSet<String> {
        let mut used = HashSet::new();

        // Always include essential builtins
        used.insert("print".to_string());
        used.insert("isinstance".to_string());

        // Check global instructions
        for instruction in &module.globals {
            if let IRInstruction::Call { func, .. } = instruction {
                if builtins::is_builtin_function(func) {
                    used.insert(func.clone());
                }
            }
        }

        // Check function instructions
        for (_name, function) in &module.functions {
            for block in &function.blocks {
                for instruction in &block.instructions {
                    if let IRInstruction::Call { func, .. } = instruction {
                        if builtins::is_builtin_function(func) {
                            used.insert(func.clone());
                        }
                    }
                }
            }
        }

        used
    }

    /// Check if the module uses OOP features
    fn uses_oop(&self, module: &IRModule) -> bool {
        // Check for class methods (functions with __ in their name)
        for (name, _function) in &module.functions {
            if name.contains("__") {
                return true;
            }
        }

        // Check global instructions
        for instruction in &module.globals {
            if matches!(instruction,
                IRInstruction::ObjectCreate { .. } |
                IRInstruction::ObjectSetAttr { .. } |
                IRInstruction::ObjectGetAttr { .. })
            {
                return true;
            }
        }

        // Check function instructions
        for (_name, function) in &module.functions {
            for block in &function.blocks {
                for instruction in &block.instructions {
                    if matches!(instruction,
                        IRInstruction::ObjectCreate { .. } |
                        IRInstruction::ObjectSetAttr { .. } |
                        IRInstruction::ObjectGetAttr { .. })
                    {
                        return true;
                    }
                }
            }
        }

        false
    }

    /// Generate C code with import system support
    pub fn generate_with_imports(&self, program: &crate::ast::Program, module: IRModule, output_dir: &str) -> Result<String> {
        use imports::{ImportAnalyzer, ModuleCompiler};
        use std::path::PathBuf;

        // Analyze imports in the program
        let mut analyzer = ImportAnalyzer::new();
        analyzer.analyze(program)?;

        let output_path = PathBuf::from(output_dir);
        std::fs::create_dir_all(&output_path)?;

        // Compile user-defined modules
        let mut module_compiler = ModuleCompiler::new(output_path.clone());
        let mut user_module_headers = Vec::new();

        for user_module in analyzer.get_user_modules() {
            let (_c_path, h_path) = module_compiler.compile_module(user_module)?;
            user_module_headers.push(h_path);
        }

        // Get builtin modules (will be linked later)
        let _builtin_modules = analyzer.get_builtin_modules();

        // Generate main C code with includes
        let mut c_code = String::new();

        // Add standard headers
        c_code.push_str(&self.generate_headers());

        // Add user module headers
        for header_path in &user_module_headers {
            if let Some(header_name) = header_path.file_name() {
                c_code.push_str(&format!("#include \"{}\"\n", header_name.to_string_lossy()));
            }
        }
        c_code.push_str("\n");

        // Add type definitions
        c_code.push_str(&types::generate_type_definitions());

        // Add OOP structures
        c_code.push_str(&oop::generate_oop_structures());

        // Add type function declarations
        c_code.push_str(&types::generate_type_function_declarations());

        // Add OOP function declarations
        c_code.push_str(&oop::generate_oop_declarations());

        // Analyze which builtins are used
        let used_builtins = self.analyze_used_builtins(&module);

        // Add builtin function declarations
        c_code.push_str(&builtins::generate_builtin_declarations(&used_builtins));

        // Add runtime function declarations
        c_code.push_str("// Runtime operators\n");
        c_code.push_str("tauraro_value_t* tauraro_add(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_sub(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_mul(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_div(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_mod(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_eq(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_ne(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_lt(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_le(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_gt(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("tauraro_value_t* tauraro_ge(tauraro_value_t* left, tauraro_value_t* right);\n");
        c_code.push_str("\n");

        // Add type utility implementations
        c_code.push_str(&types::generate_type_utility_functions());

        // Add OOP implementations
        c_code.push_str(&oop::generate_oop_implementations());

        // Add builtin implementations
        if !used_builtins.is_empty() {
            c_code.push_str("// Builtin function implementations\n");
            for builtin in &used_builtins {
                c_code.push_str(&builtins::generate_builtin_implementation(builtin));
                c_code.push_str("\n");
            }
        }

        // Add runtime operator implementations
        c_code.push_str(&runtime::generate_runtime_support());

        // Generate functions
        for (name, function) in &module.functions {
            c_code.push_str(&functions::generate_function(function)?);
            c_code.push_str("\n\n");
        }

        // Generate main function
        c_code.push_str(&self.generate_main_function(&module)?);

        Ok(c_code)
    }

    /// Transpile a module (for recursive compilation)
    pub fn transpile_module(&self, module: &IRModule, module_name: &str) -> Result<String> {
        let mut c_code = String::new();

        // Add includes
        c_code.push_str("#include <stdio.h>\n");
        c_code.push_str("#include <stdlib.h>\n");
        c_code.push_str("#include <string.h>\n");
        c_code.push_str("#include <stdbool.h>\n");
        c_code.push_str("#include <stdint.h>\n");
        c_code.push_str("#include <math.h>\n\n");

        // Add header include
        c_code.push_str(&format!("#include \"{}.h\"\n\n", module_name));

        // Add type definitions
        c_code.push_str(&types::generate_type_definitions());

        // Add OOP if needed
        if self.uses_oop(module) {
            c_code.push_str(&oop::generate_oop_structures());
            c_code.push_str(&oop::generate_oop_implementations());
        }

        // Generate module functions
        for (_name, function) in &module.functions {
            c_code.push_str(&functions::generate_function(function)?);
            c_code.push_str("\n\n");
        }

        // Module globals would be generated here
        c_code.push_str("// Module global variables\n");

        Ok(c_code)
    }
}

impl CodeGenerator for CTranspiler {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        // Extract output directory from output path for header file generation
        let output_dir = options.output_path.as_ref().and_then(|path| {
            std::path::Path::new(path).parent().map(|p| p.to_str().unwrap_or("."))
        });

        let c_code = self.generate_c_code(module, output_dir)?;

        // If output path is specified and we want to compile to executable
        if let Some(output_path) = &options.output_path {
            // Check if we should compile to executable
            let should_compile = output_path.ends_with(std::env::consts::EXE_EXTENSION)
                || !output_path.contains(".")
                || Path::new(output_path).extension().is_none();

            if should_compile {
                // Compile to executable
                compiler::compile_to_executable(&c_code, output_path, options.opt_level)?;
                // Return empty bytes since executable is created separately
                return Ok(vec![]);
            }
        }

        // Return C code as bytes
        Ok(c_code.into_bytes())
    }

    fn get_target(&self) -> Target {
        Target::C
    }

    fn supports_optimization(&self) -> bool {
        true
    }

    fn get_supported_features(&self) -> Vec<&'static str> {
        vec![
            "basic_types",
            "functions",
            "control_flow",
            "data_structures",
            "builtin_functions",
            "collections",
            "objects",
            "inheritance",
            "operators",
            "memory_management",
            "static_typing", // Added to indicate support for static typing
        ]
    }
}

impl Default for CTranspiler {
    fn default() -> Self {
        Self::new()
    }
}