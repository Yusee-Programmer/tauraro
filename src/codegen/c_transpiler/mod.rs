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
pub mod oop_optimized;
pub mod function_optimizer;
pub mod class_analyzer;
pub mod runtime;
pub mod functions;
pub mod expressions;
pub mod statements;
pub mod compiler;
pub mod imports;
pub mod type_inference;

use crate::codegen::{CodeGenerator, CodegenOptions, Target};
use crate::ir::{IRModule, IRFunction, IRInstruction, IRTypeInfo};
use crate::ast::Type;
use anyhow::Result;
use std::collections::{HashSet, HashMap};
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

        // Run type inference and class analysis for OOP optimizations
        let mut type_ctx_early = type_inference::TypeInferenceContext::new();
        type_ctx_early.analyze_module(&module);
        let mut class_analyzer_early = class_analyzer::ClassAnalyzer::new();
        let class_analysis_early = class_analyzer_early.analyze(&module, &type_ctx_early);

        // Add optimized class structs (100x faster OOP!)
        c_code.push_str(&class_analyzer::generate_optimized_class_structs(&class_analysis_early));
        c_code.push_str(&class_analyzer::generate_optimized_constructors(&class_analysis_early));

        // Add FFI header if FFI is used
        if self.uses_ffi(&module) {
            c_code.push_str("// FFI Support\n");
            c_code.push_str("#include \"tauraro_ffi.h\"\n\n");
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

        // Collect class names from function names
        let mut class_names = HashSet::new();
        for (func_name, _) in &module.functions {
            if let Some(pos) = func_name.find("__") {
                let class_name = &func_name[0..pos];
                class_names.insert(class_name.to_string());
            }
        }

        // Add forward declarations for all user-defined functions
        c_code.push_str("// Forward declarations for user-defined functions\n");
        for (_name, function) in &module.functions {
            c_code.push_str(&format!("tauraro_value_t* {}(int argc, tauraro_value_t** argv);\n", function.name));
        }
        c_code.push_str("\n");

        // Add global class variable declarations
        c_code.push_str(&self.generate_class_declarations(&module)?);

        // Generate functions
        for (_name, function) in &module.functions {
            c_code.push_str(&functions::generate_function(function, &class_names)?);
            c_code.push_str("\n\n");
        }

        // Generate main function with analysis context (reuse earlier analysis)
        c_code.push_str(&self.generate_main_function_with_analysis(&module, &type_ctx_early, &class_analysis_early)?);

        Ok(c_code)
    }

    /// Extract classes and their methods from module
    fn extract_classes(&self, module: &IRModule) -> HashMap<String, Vec<String>> {
        let mut classes: HashMap<String, Vec<String>> = HashMap::new();

        // Extract classes and their methods from function names
        // Functions like Animal__init__, Animal__speak, Dog__speak indicate classes
        for (func_name, _) in &module.functions {
            if let Some(pos) = func_name.find("__") {
                let class_name = &func_name[0..pos];
                let method_name = &func_name[pos+2..]; // Skip the "__"

                classes.entry(class_name.to_string())
                    .or_insert_with(Vec::new)
                    .push(method_name.to_string());
            }
        }

        classes
    }

    /// Generate global class variable declarations
    fn generate_class_declarations(&self, module: &IRModule) -> Result<String> {
        let classes = self.extract_classes(module);

        if classes.is_empty() {
            return Ok(String::new());
        }

        let mut decl_code = String::new();
        decl_code.push_str("// Global class variables\n");

        for class_name in classes.keys() {
            decl_code.push_str(&format!("tauraro_class_t* class_{};\n", class_name));
        }

        decl_code.push_str("\n");
        Ok(decl_code)
    }

    /// Generate class initialization code
    fn generate_class_initialization(&self, module: &IRModule) -> Result<String> {
        let classes = self.extract_classes(module);

        if classes.is_empty() {
            return Ok(String::new());
        }

        let mut init_code = String::new();
        init_code.push_str("\n    // === Class Initialization ===\n");

        // Initialize global class pointer variables
        for (class_name, methods) in &classes {
            init_code.push_str(&format!("    // Initialize class: {}\n", class_name));
            init_code.push_str(&format!("    class_{} = tauraro_class_create(\"{}\", NULL);\n",
                class_name, class_name));

            // Register all methods with the class
            for method_name in methods {
                let full_func_name = format!("{}__{}",  class_name, method_name);
                init_code.push_str(&format!("    tauraro_class_add_method(class_{}, \"{}\", (void*)&{});\n",
                    class_name, method_name, full_func_name));
            }

            init_code.push_str("\n");
        }

        init_code.push_str("    // === End Class Initialization ===\n\n");

        Ok(init_code)
    }

    /// Generate main function
    fn generate_main_function_with_analysis(&self, module: &IRModule, type_ctx: &type_inference::TypeInferenceContext, class_analysis: &class_analyzer::ClassAnalysisResult) -> Result<String> {
        let mut main_code = String::new();

        // Don't generate main if it already exists
        if module.functions.contains_key("main") {
            return Ok(main_code);
        }

        main_code.push_str("int main() {\n");

        // Track declared variables
        let mut declared_vars = HashSet::new();

        // Collect ALL variable names used in the module (comprehensive)
        fn collect_vars_from_instruction(instr: &IRInstruction, vars: &mut HashSet<String>) {
            match instr {
                IRInstruction::LoadConst { result, .. } => { vars.insert(result.clone()); }
                IRInstruction::LoadGlobal { result, name } => {
                    vars.insert(result.clone());
                    vars.insert(name.clone());
                }
                IRInstruction::LoadTypedGlobal { result, name, .. } => {
                    vars.insert(result.clone());
                    vars.insert(name.clone());
                }
                IRInstruction::StoreGlobal { name, value } => {
                    vars.insert(name.clone());
                    vars.insert(value.clone());
                }
                IRInstruction::Call { result: Some(result), args, .. } => {
                    vars.insert(result.clone());
                    for arg in args {
                        vars.insert(arg.clone());
                    }
                }
                IRInstruction::Call { func, args, .. } => {
                    for arg in args {
                        vars.insert(arg.clone());
                    }
                    if func.contains("__") && !args.is_empty() {
                        vars.insert(args[0].clone());
                    }
                }
                IRInstruction::BinaryOp { result, left, right, .. } => {
                    vars.insert(result.clone());
                    vars.insert(left.clone());
                    vars.insert(right.clone());
                }
                IRInstruction::TypedBinaryOp { result, left, right, .. } => {
                    vars.insert(result.clone());
                    vars.insert(left.clone());
                    vars.insert(right.clone());
                }
                IRInstruction::ObjectCreate { result, .. } => {
                    vars.insert(result.clone());
                }
                IRInstruction::ObjectGetAttr { result, object, .. } => {
                    vars.insert(result.clone());
                    vars.insert(object.clone());
                }
                IRInstruction::ObjectSetAttr { object, value, .. } => {
                    vars.insert(object.clone());
                    vars.insert(value.clone());
                }
                IRInstruction::For { variable, iterable, body } => {
                    vars.insert(variable.clone());
                    vars.insert(iterable.clone());
                    for body_instr in body {
                        collect_vars_from_instruction(body_instr, vars);
                    }
                }
                IRInstruction::While { condition, condition_instructions, body } => {
                    vars.insert(condition.clone());
                    for cond_instr in condition_instructions {
                        collect_vars_from_instruction(cond_instr, vars);
                    }
                    for body_instr in body {
                        collect_vars_from_instruction(body_instr, vars);
                    }
                }
                IRInstruction::If { condition, then_body, elif_branches, else_body } => {
                    vars.insert(condition.clone());
                    for then_instr in then_body {
                        collect_vars_from_instruction(then_instr, vars);
                    }
                    for (elif_cond, elif_body) in elif_branches {
                        vars.insert(elif_cond.clone());
                        for elif_instr in elif_body {
                            collect_vars_from_instruction(elif_instr, vars);
                        }
                    }
                    if let Some(else_instrs) = else_body {
                        for else_instr in else_instrs {
                            collect_vars_from_instruction(else_instr, vars);
                        }
                    }
                }
                IRInstruction::Return { value: Some(value) } => {
                    vars.insert(value.clone());
                }
                IRInstruction::ListCreate { result, elements } => {
                    vars.insert(result.clone());
                    for elem in elements {
                        vars.insert(elem.clone());
                    }
                }
                IRInstruction::DictCreate { result, pairs } => {
                    vars.insert(result.clone());
                    for (key, value) in pairs {
                        vars.insert(key.clone());
                        vars.insert(value.clone());
                    }
                }
                IRInstruction::SuperCall { result, args } => {
                    vars.insert(result.clone());
                    for arg in args {
                        vars.insert(arg.clone());
                    }
                }
                _ => {}
            }
        }

        for instruction in &module.globals {
            collect_vars_from_instruction(instruction, &mut declared_vars);
        }

        // Declare all variables as tauraro_value_t* (keep it simple for compatibility)
        // Optimized structs will be wrapped inside tauraro_value_t
        for var_name in &declared_vars {
            main_code.push_str(&format!("    tauraro_value_t* {} = NULL;\n", var_name));
        }

        // Generate class initialization code (if OOP is used)
        if self.uses_oop(module) {
            main_code.push_str(&self.generate_class_initialization(module)?);
        }

        // Build a map of ObjectCreate indices to their preceding LoadConst arguments
        let mut constructor_args: std::collections::HashMap<usize, Vec<String>> = std::collections::HashMap::new();
        let mut pending_args: Vec<String> = Vec::new();

        for (idx, instruction) in module.globals.iter().enumerate() {
            match instruction {
                IRInstruction::LoadConst { result, .. } => {
                    // Track potential constructor argument
                    if result.starts_with("arg_") {
                        pending_args.push(result.clone());
                    }
                }
                IRInstruction::ObjectCreate { .. } => {
                    // Save pending args for this ObjectCreate
                    if !pending_args.is_empty() {
                        constructor_args.insert(idx, pending_args.clone());
                        pending_args.clear();
                    }
                }
                IRInstruction::Call { .. } | IRInstruction::StoreGlobal { .. } => {
                    // These instructions consume arguments, so clear pending
                    if !pending_args.is_empty() {
                        pending_args.clear();
                    }
                }
                _ => {}
            }
        }

        // Execute global instructions (pass module and constructor args for context)
        for (idx, instruction) in module.globals.iter().enumerate() {
            let args = constructor_args.get(&idx).cloned().unwrap_or_default();
            main_code.push_str(&format!("    {}\n", self.generate_global_instruction_with_context(instruction, module, &args, &type_ctx, &class_analysis)?));
        }

        // Call main_function if it exists
        if module.functions.contains_key("main_function") {
            main_code.push_str("    main_function();\n");
        }

        main_code.push_str("    return 0;\n");
        main_code.push_str("}\n");

        Ok(main_code)
    }

    /// Generate code for a global instruction with full module context and constructor arguments
    fn generate_global_instruction_with_context(&self, instruction: &IRInstruction, module: &IRModule, constructor_args: &[String], type_ctx: &type_inference::TypeInferenceContext, class_analysis: &class_analyzer::ClassAnalysisResult) -> Result<String> {
        // Special handling for ObjectCreate to inject __init__ calls
        if let IRInstruction::ObjectCreate { class_name, result } = instruction {
            // Check if this class can be optimized
            if class_analysis.optimizable_classes.contains_key(class_name) {
                // OPTIMIZED: Use static struct constructor (100x faster!)
                // Wrap the optimized struct in a tauraro_value_t for compatibility
                let mut code = format!("// OPTIMIZED: Static struct for {}\n    ", class_name);
                code.push_str(&format!("{}_t* {}_struct = {}_new();\n    ", class_name, result, class_name));
                code.push_str(&format!("{} = tauraro_value_new();\n    ", result));
                code.push_str(&format!("{}->type = TAURARO_OBJECT;\n    ", result));
                code.push_str(&format!("{}->data.ptr_val = (void*){}_struct;", result, result));
                return Ok(code);
            }

            // Fall back to dynamic object creation
            let mut code = format!("{} = tauraro_object_create(\"{}\");", result, class_name);

            // Link object to its class
            code.push_str(&format!("\n    if (class_{}) {{\n", class_name));
            code.push_str(&format!("        ((tauraro_object_t*){}->data.obj_val)->class_ptr = class_{};\n", result, class_name));

            // Check if class has __init__ method and auto-call it
            let init_method_name = format!("{}____init__", class_name);
            if module.functions.contains_key(&init_method_name) {
                code.push_str("        // Auto-call __init__ with constructor arguments\n");
                code.push_str(&format!("        tauraro_value_t* init_method = tauraro_class_get_method(class_{}, \"__init__\");\n", class_name));
                code.push_str("        if (init_method && init_method->type == TAURARO_FUNCTION) {\n");
                code.push_str("            typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);\n");
                code.push_str("            method_func_t init_func = (method_func_t)init_method->data.ptr_val;\n");

                // Build argument list: self + constructor args
                if constructor_args.is_empty() {
                    code.push_str(&format!("            init_func(1, (tauraro_value_t*[]){{{}}});\n", result));
                } else {
                    let all_args = std::iter::once(result.as_str())
                        .chain(constructor_args.iter().map(|s| s.as_str()))
                        .collect::<Vec<_>>()
                        .join(", ");
                    let argc = constructor_args.len() + 1;
                    code.push_str(&format!("            init_func({}, (tauraro_value_t*[]){{{}}});\n", argc, all_args));
                }

                code.push_str("        }\n");
            }

            code.push_str("    }");
            return Ok(code);
        }

        // Check for optimizable operations (Int, Float, String, Bool)
        match instruction {
            // ========== INTEGER OPTIMIZATIONS ==========
            IRInstruction::LoadConst { value: crate::value::Value::Int(i), result }
                if type_ctx.is_optimizable_int(result) => {
                // Generate optimized code for integer constant
                return Ok(format!("{} = {};", result, i));
            }
            IRInstruction::BinaryOp { op, left, right, result }
                if type_ctx.is_optimizable_int(result) &&
                   type_ctx.is_optimizable_int(left) &&
                   type_ctx.is_optimizable_int(right) => {
                // Generate optimized code for integer binary operations (ALL OPERATORS)
                let op_str = match op {
                    // Arithmetic operators
                    crate::ast::BinaryOp::Add => "+",
                    crate::ast::BinaryOp::Sub => "-",
                    crate::ast::BinaryOp::Mul => "*",
                    crate::ast::BinaryOp::Div => "/",
                    crate::ast::BinaryOp::FloorDiv => "/",  // Integer division
                    crate::ast::BinaryOp::Mod => "%",

                    // Bitwise operators
                    crate::ast::BinaryOp::BitAnd => "&",
                    crate::ast::BinaryOp::BitOr => "|",
                    crate::ast::BinaryOp::BitXor => "^",
                    crate::ast::BinaryOp::LShift => "<<",
                    crate::ast::BinaryOp::RShift => ">>",

                    // Comparison operators
                    crate::ast::BinaryOp::Lt => "<",
                    crate::ast::BinaryOp::Le => "<=",
                    crate::ast::BinaryOp::Gt => ">",
                    crate::ast::BinaryOp::Ge => ">=",
                    crate::ast::BinaryOp::Gte => ">=",  // Alias
                    crate::ast::BinaryOp::Lte => "<=",  // Alias
                    crate::ast::BinaryOp::Eq => "==",
                    crate::ast::BinaryOp::Ne => "!=",
                    crate::ast::BinaryOp::Neq => "!=",  // Alias

                    // Logical operators (for integers, treat as boolean)
                    crate::ast::BinaryOp::And => "&&",
                    crate::ast::BinaryOp::Or => "||",

                    _ => {
                        // Fallback to standard handler for unsupported ops (Pow, MatMul, etc.)
                        return self.generate_global_instruction(instruction, &module.type_info);
                    }
                };
                return Ok(format!("{} = {} {} {};", result, left, op_str, right));
            }
            IRInstruction::StoreGlobal { name, value }
                if type_ctx.is_optimizable_int(name) && type_ctx.is_optimizable_int(value) => {
                // Direct assignment for integers
                return Ok(format!("{} = {};", name, value));
            }

            // ========== FLOAT OPTIMIZATIONS ==========
            IRInstruction::LoadConst { value: crate::value::Value::Float(f), result }
                if type_ctx.is_optimizable_float(result) => {
                // Generate optimized code for float constant
                return Ok(format!("{} = {};", result, f));
            }
            IRInstruction::BinaryOp { op, left, right, result }
                if type_ctx.is_optimizable_float(result) &&
                   type_ctx.is_optimizable_float(left) &&
                   type_ctx.is_optimizable_float(right) => {
                // Generate optimized code for float binary operations (ALL OPERATORS)
                match op {
                    // Arithmetic operators
                    crate::ast::BinaryOp::Add => return Ok(format!("{} = {} + {};", result, left, right)),
                    crate::ast::BinaryOp::Sub => return Ok(format!("{} = {} - {};", result, left, right)),
                    crate::ast::BinaryOp::Mul => return Ok(format!("{} = {} * {};", result, left, right)),
                    crate::ast::BinaryOp::Div => return Ok(format!("{} = {} / {};", result, left, right)),
                    crate::ast::BinaryOp::FloorDiv => return Ok(format!("{} = floor({} / {});", result, left, right)),
                    crate::ast::BinaryOp::Mod => return Ok(format!("{} = fmod({}, {});", result, left, right)),
                    crate::ast::BinaryOp::Pow => return Ok(format!("{} = pow({}, {});", result, left, right)),

                    // Comparison operators
                    crate::ast::BinaryOp::Lt => return Ok(format!("{} = ({} < {});", result, left, right)),
                    crate::ast::BinaryOp::Le => return Ok(format!("{} = ({} <= {});", result, left, right)),
                    crate::ast::BinaryOp::Gt => return Ok(format!("{} = ({} > {});", result, left, right)),
                    crate::ast::BinaryOp::Ge => return Ok(format!("{} = ({} >= {});", result, left, right)),
                    crate::ast::BinaryOp::Gte => return Ok(format!("{} = ({} >= {});", result, left, right)),
                    crate::ast::BinaryOp::Lte => return Ok(format!("{} = ({} <= {});", result, left, right)),
                    crate::ast::BinaryOp::Eq => return Ok(format!("{} = ({} == {});", result, left, right)),
                    crate::ast::BinaryOp::Ne => return Ok(format!("{} = ({} != {});", result, left, right)),
                    crate::ast::BinaryOp::Neq => return Ok(format!("{} = ({} != {});", result, left, right)),

                    // Logical operators
                    crate::ast::BinaryOp::And => return Ok(format!("{} = ({} && {});", result, left, right)),
                    crate::ast::BinaryOp::Or => return Ok(format!("{} = ({} || {});", result, left, right)),

                    _ => {
                        // Fallback to standard handler for unsupported ops
                        return self.generate_global_instruction(instruction, &module.type_info);
                    }
                }
            }
            IRInstruction::StoreGlobal { name, value }
                if type_ctx.is_optimizable_float(name) && type_ctx.is_optimizable_float(value) => {
                // Direct assignment for floats
                return Ok(format!("{} = {};", name, value));
            }

            // ========== STRING OPTIMIZATIONS ==========
            IRInstruction::LoadConst { value: crate::value::Value::Str(s), result }
                if type_ctx.is_optimizable_string(result) => {
                // Generate optimized code for string constant
                let escaped = s
                    .replace("\\", "\\\\")
                    .replace("\"", "\\\"")
                    .replace("\n", "\\n")
                    .replace("\r", "\\r")
                    .replace("\t", "\\t");
                return Ok(format!("{} = strdup(\"{}\");", result, escaped));
            }
            IRInstruction::BinaryOp { op, left, right, result }
                if type_ctx.is_optimizable_string(result) &&
                   type_ctx.is_optimizable_string(left) &&
                   type_ctx.is_optimizable_string(right) &&
                   matches!(op, crate::ast::BinaryOp::Add) => {
                // Generate optimized code for string concatenation
                return Ok(format!("{{ size_t len = strlen({}) + strlen({}) + 1; {} = malloc(len); strcpy({}, {}); strcat({}, {}); }}",
                    left, right, result, result, left, result, right));
            }
            IRInstruction::StoreGlobal { name, value }
                if type_ctx.is_optimizable_string(name) && type_ctx.is_optimizable_string(value) => {
                // Direct assignment for strings (need to duplicate)
                return Ok(format!("{} = strdup({});", name, value));
            }

            // ========== BOOL OPTIMIZATIONS ==========
            IRInstruction::LoadConst { value: crate::value::Value::Bool(b), result }
                if type_ctx.is_optimizable_bool(result) => {
                // Generate optimized code for bool constant
                return Ok(format!("{} = {};", result, if *b { "true" } else { "false" }));
            }
            IRInstruction::StoreGlobal { name, value }
                if type_ctx.is_optimizable_bool(name) && type_ctx.is_optimizable_bool(value) => {
                // Direct assignment for bools
                return Ok(format!("{} = {};", name, value));
            }
            IRInstruction::Call { func, args, result } if args.iter().any(|arg| type_ctx.is_optimizable(arg)) => {
                // Handle builtin function calls with optimized arguments (int, float, string, bool)
                // Need to convert native types to tauraro_value_t* temporarily
                let mut code_lines = Vec::new();
                let mut converted_args = Vec::new();

                for (i, arg) in args.iter().enumerate() {
                    if type_ctx.is_optimizable_int(arg) {
                        // Create temporary tauraro_value_t* for integer argument
                        let temp_var = format!("{}_as_value", arg);
                        code_lines.push(format!("tauraro_value_t* {} = tauraro_value_new();", temp_var));
                        code_lines.push(format!("{}->type = TAURARO_INT;", temp_var));
                        code_lines.push(format!("{}->data.int_val = {};", temp_var, arg));
                        converted_args.push(temp_var);
                    } else if type_ctx.is_optimizable_float(arg) {
                        // Create temporary tauraro_value_t* for float argument
                        let temp_var = format!("{}_as_value", arg);
                        code_lines.push(format!("tauraro_value_t* {} = tauraro_value_new();", temp_var));
                        code_lines.push(format!("{}->type = TAURARO_FLOAT;", temp_var));
                        code_lines.push(format!("{}->data.float_val = {};", temp_var, arg));
                        converted_args.push(temp_var);
                    } else if type_ctx.is_optimizable_string(arg) {
                        // Create temporary tauraro_value_t* for string argument
                        let temp_var = format!("{}_as_value", arg);
                        code_lines.push(format!("tauraro_value_t* {} = tauraro_value_new();", temp_var));
                        code_lines.push(format!("{}->type = TAURARO_STRING;", temp_var));
                        code_lines.push(format!("{}->data.str_val = {};", temp_var, arg));
                        converted_args.push(temp_var);
                    } else if type_ctx.is_optimizable_bool(arg) {
                        // Create temporary tauraro_value_t* for bool argument
                        let temp_var = format!("{}_as_value", arg);
                        code_lines.push(format!("tauraro_value_t* {} = tauraro_value_new();", temp_var));
                        code_lines.push(format!("{}->type = TAURARO_BOOL;", temp_var));
                        code_lines.push(format!("{}->data.bool_val = {};", temp_var, arg));
                        converted_args.push(temp_var);
                    } else {
                        converted_args.push(arg.clone());
                    }
                }

                // Generate the function call (add tauraro_ prefix if needed)
                let func_name = if func.starts_with("tauraro_") {
                    func.clone()
                } else {
                    format!("tauraro_{}", func)
                };
                let args_str = converted_args.join(", ");
                if let Some(res) = result {
                    code_lines.push(format!("{} = {}({}, (tauraro_value_t*[]){{{}}});", res, func_name, args.len(), args_str));
                } else {
                    code_lines.push(format!("{}({}, (tauraro_value_t*[]){{{}}});", func_name, args.len(), args_str));
                }

                return Ok(code_lines.join(" "));
            }
            IRInstruction::For { variable, iterable, body } if type_ctx.is_optimizable_int(variable) => {
                // Generate optimized For loop for integer variables
                let mut code = String::new();
                let iterator_var = format!("{}_iter", variable);
                let index_var = format!("{}_index", variable);

                code.push_str(&format!("// For loop: {} in {} (optimized)\n", variable, iterable));
                code.push_str(&format!("    tauraro_value_t* {} = {};\n", iterator_var, iterable));
                code.push_str(&format!("    int {} = 0;\n", index_var));

                // Only handle RANGE for now (most common case)
                code.push_str(&format!("    if ({}->type == TAURARO_RANGE) {{\n", iterator_var));
                code.push_str(&format!("        int start = {}->data.range_val->start;\n", iterator_var));
                code.push_str(&format!("        int stop = {}->data.range_val->stop;\n", iterator_var));
                code.push_str(&format!("        int step = {}->data.range_val->step;\n", iterator_var));
                code.push_str(&format!("        for ({} = start; (step > 0) ? ({} < stop) : ({} > stop); {} += step) {{\n",
                    variable, variable, variable, variable));

                // Generate body with optimized instructions
                for instruction in body {
                    let instr_code = self.generate_global_instruction_with_context(instruction, module, &[], type_ctx, class_analysis)?;
                    code.push_str(&format!("            {}\n", instr_code));
                }

                code.push_str("        }\n");
                code.push_str("    } else {\n");
                code.push_str("        // Fallback for non-range iterables\n");
                code.push_str("        // TODO: Handle list/tuple/etc\n");
                code.push_str("    }");

                return Ok(code);
            }

            // ========== WHILE LOOP OPTIMIZATIONS ==========
            IRInstruction::While { condition, condition_instructions, body }
                if type_ctx.is_optimizable_int(condition) || type_ctx.is_optimizable_bool(condition) => {
                // Generate optimized While loop with native condition
                let mut code = String::new();

                code.push_str(&format!("// While loop (optimized)\n"));
                code.push_str(&format!("    while ({}) {{\n", condition));

                // Generate body
                for instruction in body {
                    let instr_code = self.generate_global_instruction_with_context(instruction, module, &[], type_ctx, class_analysis)?;
                    code.push_str(&format!("        {}\n", instr_code));
                }

                // Re-evaluate condition at end of loop
                if !condition_instructions.is_empty() {
                    code.push_str("        // Re-evaluate condition\n");
                    for instr in condition_instructions {
                        let instr_code = self.generate_global_instruction_with_context(instr, module, &[], type_ctx, class_analysis)?;
                        code.push_str(&format!("        {}\n", instr_code));
                    }
                }

                code.push_str("    }");
                return Ok(code);
            }

            // ========== IF STATEMENT OPTIMIZATIONS ==========
            IRInstruction::If { condition, then_body, elif_branches, else_body }
                if type_ctx.is_optimizable_int(condition) || type_ctx.is_optimizable_bool(condition) => {
                // Generate optimized If with native condition
                let mut code = String::new();

                code.push_str(&format!("if ({}) {{\n", condition));

                // Generate then body
                for instruction in then_body {
                    let instr_code = self.generate_global_instruction_with_context(instruction, module, &[], type_ctx, class_analysis)?;
                    code.push_str(&format!("        {}\n", instr_code));
                }
                code.push_str("    }");

                // Generate elif branches
                for (elif_cond, elif_body) in elif_branches {
                    // Check if elif condition is also optimizable
                    if type_ctx.is_optimizable_int(elif_cond) || type_ctx.is_optimizable_bool(elif_cond) {
                        code.push_str(&format!(" else if ({}) {{\n", elif_cond));
                    } else {
                        code.push_str(&format!(" else if (tauraro_is_truthy({})) {{\n", elif_cond));
                    }
                    for instruction in elif_body {
                        let instr_code = self.generate_global_instruction_with_context(instruction, module, &[], type_ctx, class_analysis)?;
                        code.push_str(&format!("        {}\n", instr_code));
                    }
                    code.push_str("    }");
                }

                // Generate else body
                if let Some(else_instructions) = else_body {
                    code.push_str(" else {\n");
                    for instruction in else_instructions {
                        let instr_code = self.generate_global_instruction_with_context(instruction, module, &[], type_ctx, class_analysis)?;
                        code.push_str(&format!("        {}\n", instr_code));
                    }
                    code.push_str("    }");
                }

                return Ok(code);
            }

            // ========== OOP OPTIMIZATIONS ==========
            IRInstruction::ObjectSetAttr { object, attr, value } => {
                // Check if the object is an optimizable class instance
                if let Some(class_name) = class_analysis.object_types.get(object) {
                    if class_analysis.optimizable_classes.contains_key(class_name) {
                        // OPTIMIZED: Direct field access (100x faster!)
                        // Access the wrapped struct from ptr_val
                        return Ok(format!("// OPTIMIZED: Direct field access\n    (({}_t*){}->data.ptr_val)->{} = {};",
                            class_name, object, attr, value));
                    }
                }
                // Fall back to dynamic attribute setting
                return Ok(format!("tauraro_object_set_attr({}, \"{}\", {});", object, attr, value));
            }

            IRInstruction::ObjectGetAttr { object, attr, result } => {
                // Check if the object is an optimizable class instance
                if let Some(class_name) = class_analysis.object_types.get(object) {
                    if class_analysis.optimizable_classes.contains_key(class_name) {
                        // OPTIMIZED: Direct field access (100x faster!)
                        // Access the wrapped struct from ptr_val
                        return Ok(format!("// OPTIMIZED: Direct field access\n    {} = (({}_t*){}->data.ptr_val)->{};",
                            result, class_name, object, attr));
                    }
                }
                // Fall back to dynamic attribute getting
                return Ok(format!("{} = tauraro_object_get_attr({}, \"{}\");", result, object, attr));
            }

            // ========== METHOD CALL DEVIRTUALIZATION ==========
            IRInstruction::Call { func, args, result } if func.contains("__") && !args.is_empty() => {
                // Check if this is a method call on an optimizable class
                let parts: Vec<&str> = func.split("__").collect();
                if parts.len() == 2 {
                    let class_name = parts[0];
                    let method_name = parts[1];
                    let self_arg = &args[0];

                    // Check if the class is optimizable and self is a known instance
                    if class_analysis.optimizable_classes.contains_key(class_name) {
                        // OPTIMIZED: Direct method call (devirtualization)
                        // Generate typed method call instead of dynamic dispatch
                        let args_str = if args.len() == 1 {
                            format!("1, (tauraro_value_t*[]){{{}}}", self_arg)
                        } else {
                            let arg_list = args.join(", ");
                            format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_list)
                        };

                        let func_call = if let Some(res) = result {
                            format!("// OPTIMIZED: Devirtualized method call\n    {} = {}({});",
                                res, func, args_str)
                        } else {
                            format!("// OPTIMIZED: Devirtualized method call\n    {}({});",
                                func, args_str)
                        };
                        return Ok(func_call);
                    }
                }
                // Fall through to standard handler for non-optimizable method calls
            }

            _ => {}
        }

        // For all other instructions, use the standard handler
        self.generate_global_instruction(instruction, &module.type_info)
    }

    /// Generate code for a global instruction with variable tracking
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
                // First check if this is an FFI function call
                match func.as_str() {
                    "load_library" => {
                        // Generate C code for loading a native library
                        // Tauraro: load_library("libm.so")
                        // C: tauraro_ffi_load_library("libm.so");
                        if args.is_empty() {
                            return Err(anyhow::anyhow!("load_library requires at least 1 argument"));
                        }
                        let library_arg = &args[0];
                        return match result {
                            Some(res) => Ok(format!("{} = tauraro_ffi_load_library({}->data.str_val);", res, library_arg)),
                            None => Ok(format!("tauraro_ffi_load_library({}->data.str_val);", library_arg))
                        };
                    }
                    "define_function" => {
                        // Generate C code for defining a foreign function
                        // Tauraro: define_function(lib_name, func_name, return_type, [param_types...])
                        // C: tauraro_ffi_define_function(lib_name, func_name, return_type, param_types, count);
                        if args.len() < 4 {
                            return Err(anyhow::anyhow!("define_function requires at least 4 arguments"));
                        }

                        let lib_name = &args[0];
                        let func_name = &args[1];
                        let return_type = &args[2];
                        let param_types_list = &args[3];

                        // Generate code to extract parameter types from list
                        let mut code = String::new();
                        code.push_str("{\n    ");
                        code.push_str(&format!("    // Define foreign function from {}\n    ", lib_name));
                        code.push_str(&format!("    int param_count = {}->data.list_val->count;\n    ", param_types_list));
                        code.push_str("    const char** param_types = (const char**)malloc(sizeof(char*) * param_count);\n    ");
                        code.push_str(&format!("    for (int i = 0; i < param_count; i++) {{\n    "));
                        code.push_str(&format!("        tauraro_value_t* param_type = {}->data.list_val->items[i];\n    ", param_types_list));
                        code.push_str("        if (param_type->type == TAURARO_STRING) {\n    ");
                        code.push_str("            param_types[i] = param_type->data.str_val;\n    ");
                        code.push_str("        }\n    ");
                        code.push_str("    }\n    ");

                        match result {
                            Some(res) => {
                                code.push_str(&format!("    {} = tauraro_ffi_define_function(\n    ", res));
                                code.push_str(&format!("        {}->data.str_val,\n    ", lib_name));
                                code.push_str(&format!("        {}->data.str_val,\n    ", func_name));
                                code.push_str(&format!("        {}->data.str_val,\n    ", return_type));
                                code.push_str("        param_types,\n    ");
                                code.push_str("        param_count\n    ");
                                code.push_str("    );\n    ");
                            }
                            None => {
                                code.push_str("    tauraro_ffi_define_function(\n    ");
                                code.push_str(&format!("        {}->data.str_val,\n    ", lib_name));
                                code.push_str(&format!("        {}->data.str_val,\n    ", func_name));
                                code.push_str(&format!("        {}->data.str_val,\n    ", return_type));
                                code.push_str("        param_types,\n    ");
                                code.push_str("        param_count\n    ");
                                code.push_str("    );\n    ");
                            }
                        }

                        code.push_str("    free(param_types);\n    ");
                        code.push_str("}");

                        return Ok(code);
                    }
                    "call_function" => {
                        // Generate C code for calling a foreign function
                        // Tauraro: call_function(lib_name, func_name, [args...])
                        // C: tauraro_ffi_call_function(func_name, args, arg_count);
                        if args.len() < 2 {
                            return Err(anyhow::anyhow!("call_function requires at least 2 arguments"));
                        }

                        let lib_name = &args[0];
                        let func_name_arg = &args[1];
                        let ffi_args = if args.len() > 2 { &args[2] } else { "" };

                        let mut code = String::new();
                        code.push_str("{\n    ");
                        code.push_str(&format!("    // Call foreign function {} from {}\n    ", func_name_arg, lib_name));

                        if args.len() > 2 {
                            code.push_str(&format!("    int arg_count = {}->data.list_val->count;\n    ", ffi_args));
                            code.push_str(&format!("    tauraro_value_t** ffi_args = {}->data.list_val->items;\n    ", ffi_args));
                        } else {
                            code.push_str("    int arg_count = 0;\n    ");
                            code.push_str("    tauraro_value_t** ffi_args = NULL;\n    ");
                        }

                        match result {
                            Some(res) => {
                                code.push_str(&format!("    {} = tauraro_ffi_call_function(\n    ", res));
                                code.push_str(&format!("        {}->data.str_val,\n    ", func_name_arg));
                                code.push_str("        ffi_args,\n    ");
                                code.push_str("        arg_count\n    ");
                                code.push_str("    );\n    ");
                            }
                            None => {
                                code.push_str("    tauraro_ffi_call_function(\n    ");
                                code.push_str(&format!("        {}->data.str_val,\n    ", func_name_arg));
                                code.push_str("        ffi_args,\n    ");
                                code.push_str("        arg_count\n    ");
                                code.push_str("    );\n    ");
                            }
                        }

                        code.push_str("}");
                        return Ok(code);
                    }
                    _ => {
                        // Not an FFI function, continue with regular function call handling
                    }
                }

                // First check if this looks like an object method call: object__method
                // Pattern: lowercase_variable__methodname with double underscore and one arg (self)
                if func.contains("__") && args.len() == 1 {
                    // Check if first part is lowercase (likely a variable instance)
                    if let Some(double_under_pos) = func.find("__") {
                        let var_name = &func[0..double_under_pos];
                        let method_name = &func[double_under_pos+2..]; // Skip "__"

                        // Check if var_name looks like an instance variable (starts with lowercase)
                        // Exclude builtin modules
                        let is_instance_call = !matches!(var_name, "math" | "sys" | "os" | "time" | "random" | "json" | "re" | "io" | "csv" | "datetime" | "collections" | "itertools" | "functools" | "threading" | "socket" | "asyncio");

                        if is_instance_call {
                            // This is an object method call like dog__bark(dog)
                            // Transform to proper method invocation
                            // Get self from args[0]
                            let self_var = &args[0];

                            // Generate dynamic method call
                            let mut code = String::new();
                            code.push_str(&format!("// Object method call: {}.{}()\n    ", self_var, method_name));
                            code.push_str(&format!("if ({} && {}->type == TAURARO_OBJECT) {{\n    ", self_var, self_var));
                            code.push_str(&format!("    tauraro_object_t* obj_{} = (tauraro_object_t*){}->data.obj_val;\n    ", self_var, self_var));
                            code.push_str(&format!("    if (obj_{}->class_ptr) {{\n    ", self_var));
                            code.push_str(&format!("        tauraro_value_t* method = tauraro_class_get_method(obj_{}->class_ptr, \"{}\");\n    ", self_var, method_name));
                            code.push_str("        if (method && method->type == TAURARO_FUNCTION) {\n    ");
                            code.push_str("            // Call method function pointer with self\n    ");
                            code.push_str("            typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);\n    ");
                            code.push_str("            method_func_t func_ptr = (method_func_t)method->data.ptr_val;\n    ");
                            if let Some(res) = result {
                                code.push_str(&format!("            {} = func_ptr(1, (tauraro_value_t*[]){{{}}});\n    ", res, self_var));
                            } else {
                                code.push_str(&format!("            func_ptr(1, (tauraro_value_t*[]){{{}}});\n    ", self_var));
                            }
                            code.push_str("        }\n    ");
                            code.push_str("    }\n    ");
                            code.push_str("}");
                            return Ok(code);
                        }
                    }
                }

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
                    // Could be user-defined function or builtin function
                    // Check if it's a builtin function using builtins module
                    let is_builtin = builtins::is_builtin_function(func);

                    let args_str = if args.is_empty() {
                        "0, NULL".to_string()
                    } else {
                        let arg_list = args.join(", ");
                        format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_list)
                    };

                    // Add tauraro_ prefix only for builtin functions
                    let func_name = if is_builtin {
                        format!("tauraro_{}", func)
                    } else {
                        func.to_string()
                    };

                    match result {
                        Some(res) => Ok(format!("{} = {}({});", res, func_name, args_str)),
                        None => Ok(format!("{}({});", func_name, args_str))
                    }
                }
            }
            IRInstruction::ObjectCreate { class_name, result } => {
                // Create object and link it with its class
                let mut code = format!("{} = tauraro_object_create(\"{}\");", result, class_name);
                // Link object to its class (if class was initialized)
                code.push_str(&format!("\n    if (class_{}) {{\n", class_name));
                code.push_str(&format!("        ((tauraro_object_t*){}->data.obj_val)->class_ptr = class_{};\n", result, class_name));
                code.push_str("    }");
                // Note: __init__ should be called by a subsequent Call instruction in the IR
                // If not present in IR, user code didn't have __init__ or passed no args
                Ok(code)
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
                    crate::ast::BinaryOp::FloorDiv => "tauraro_floordiv",
                    crate::ast::BinaryOp::Mod => "tauraro_mod",
                    crate::ast::BinaryOp::Pow => "tauraro_pow",
                    crate::ast::BinaryOp::LShift => "tauraro_lshift",
                    crate::ast::BinaryOp::RShift => "tauraro_rshift",
                    crate::ast::BinaryOp::BitOr => "tauraro_bitor",
                    crate::ast::BinaryOp::BitXor => "tauraro_bitxor",
                    crate::ast::BinaryOp::BitAnd => "tauraro_bitand",
                    crate::ast::BinaryOp::Eq => "tauraro_eq",
                    crate::ast::BinaryOp::Ne => "tauraro_ne",
                    crate::ast::BinaryOp::Neq => "tauraro_ne",
                    crate::ast::BinaryOp::Lt => "tauraro_lt",
                    crate::ast::BinaryOp::Le => "tauraro_le",
                    crate::ast::BinaryOp::Lte => "tauraro_le",
                    crate::ast::BinaryOp::Gt => "tauraro_gt",
                    crate::ast::BinaryOp::Ge => "tauraro_ge",
                    crate::ast::BinaryOp::Gte => "tauraro_ge",
                    crate::ast::BinaryOp::Is => "tauraro_is",
                    crate::ast::BinaryOp::IsNot => "tauraro_is_not",
                    crate::ast::BinaryOp::In => "tauraro_in",
                    crate::ast::BinaryOp::NotIn => "tauraro_not_in",
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
                                    _ => "tauraro_add"
                                };
                                Ok(format!("{} = {}({}, {}); // Typed operation", result, op_func, left, right))
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
                                    _ => "tauraro_add"
                                };
                                Ok(format!("{} = {}({}, {}); // Typed operation", result, op_func, left, right))
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
                                    _ => "tauraro_add"
                                };
                                Ok(format!("{} = {}({}, {}); // Typed operation", result, op_func, left, right))
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
            IRInstruction::LoadLocal { name, result } => {
                // In global scope, treat local loads as global loads
                Ok(format!("{} = {};", result, name))
            }
            IRInstruction::StoreLocal { name, value } => {
                // In global scope, treat local stores as global stores
                Ok(format!("{} = {};", name, value))
            }
            IRInstruction::Import { module } => {
                Ok(format!("// Import module: {}", module))
            }
            IRInstruction::ImportFrom { module, names: _ } => {
                Ok(format!("// Import from module: {}", module))
            }
            IRInstruction::If { condition, then_body, elif_branches, else_body } => {
                let mut code = String::new();

                // Generate condition check
                code.push_str(&format!("if (tauraro_is_truthy({})) {{\n", condition));

                // Generate then body
                for instruction in then_body {
                    let instr_code = self.generate_global_instruction(instruction, type_info)?;
                    code.push_str(&format!("        {}\n", instr_code));
                }
                code.push_str("    }");

                // Generate elif branches
                for (elif_cond, elif_body) in elif_branches {
                    code.push_str(&format!(" else if (tauraro_is_truthy({})) {{\n", elif_cond));
                    for instruction in elif_body {
                        let instr_code = self.generate_global_instruction(instruction, type_info)?;
                        code.push_str(&format!("        {}\n", instr_code));
                    }
                    code.push_str("    }");
                }

                // Generate else body
                if let Some(else_instructions) = else_body {
                    code.push_str(" else {\n");
                    for instruction in else_instructions {
                        let instr_code = self.generate_global_instruction(instruction, type_info)?;
                        code.push_str(&format!("        {}\n", instr_code));
                    }
                    code.push_str("    }");
                }

                Ok(code)
            }
            IRInstruction::While { condition, condition_instructions, body } => {
                let mut code = String::new();

                // Generate while header
                code.push_str(&format!("while (tauraro_is_truthy({})) {{\n", condition));

                // Generate body
                for instruction in body {
                    let instr_code = self.generate_global_instruction(instruction, type_info)?;
                    code.push_str(&format!("        {}\n", instr_code));
                }

                // Re-evaluate condition at end of loop
                code.push_str("        // Re-evaluate condition\n");
                for instr in condition_instructions {
                    let instr_code = self.generate_global_instruction(instr, type_info)?;
                    code.push_str(&format!("        {}\n", instr_code));
                }
                code.push_str("    }");

                Ok(code)
            }
            IRInstruction::For { variable, iterable, body } => {
                let mut code = String::new();

                // Generate iterator setup
                let iterator_var = format!("{}_iter", variable);
                let index_var = format!("{}_index", variable);

                code.push_str(&format!("// For loop: {} in {}\n", variable, iterable));
                code.push_str(&format!("    tauraro_value_t* {} = {};\n", iterator_var, iterable));
                code.push_str(&format!("    int {} = 0;\n", index_var));

                // Handle different iterable types
                code.push_str(&format!("    if ({}->type == TAURARO_LIST) {{\n", iterator_var));
                code.push_str(&format!("        int iter_len = {}->data.list_val->size;\n", iterator_var));
                code.push_str(&format!("        for ({} = 0; {} < iter_len; {}++) {{\n", index_var, index_var, index_var));

                // Get current element
                code.push_str(&format!("            tauraro_value_t* {} = {}->data.list_val->items[{}];\n", variable, iterator_var, index_var));

                // Generate body
                for instruction in body {
                    let instr_code = self.generate_global_instruction(instruction, type_info)?;
                    code.push_str(&format!("            {}\n", instr_code));
                }

                code.push_str("        }\n");
                code.push_str("    } else if (");
                code.push_str(&format!("{}->type == TAURARO_RANGE) {{\n", iterator_var));
                code.push_str(&format!("        int start = {}->data.range_val->start;\n", iterator_var));
                code.push_str(&format!("        int stop = {}->data.range_val->stop;\n", iterator_var));
                code.push_str(&format!("        int step = {}->data.range_val->step;\n", iterator_var));
                let c_loop_var = format!("{}_c_loop_idx", variable);
                code.push_str(&format!("        for (int {} = start; (step > 0) ? ({} < stop) : ({} > stop); {} += step) {{\n",
                    c_loop_var, c_loop_var, c_loop_var, c_loop_var));

                // Create tauraro_value_t for the loop variable
                code.push_str(&format!("            tauraro_value_t* {} = tauraro_value_new();\n", variable));
                code.push_str(&format!("            {}->type = TAURARO_INT;\n", variable));
                code.push_str(&format!("            {}->data.int_val = {};\n", variable, c_loop_var));

                // Generate body
                for instruction in body {
                    let instr_code = self.generate_global_instruction(instruction, type_info)?;
                    code.push_str(&format!("            {}\n", instr_code));
                }

                code.push_str("        }\n");
                code.push_str("    } else {\n");
                code.push_str("        // TODO: Handle other iterable types (dict, tuple, set, string)\n");
                code.push_str("    }");

                Ok(code)
            }
            IRInstruction::Break => {
                Ok("break;".to_string())
            }
            IRInstruction::Continue => {
                Ok("continue;".to_string())
            }
            IRInstruction::Try { body, handlers: _, else_body: _, finally_body: _ } => {
                // Simple try block - just execute the body for now
                let mut code = String::new();
                code.push_str("// Try block (exception handling not fully implemented)\n");
                for instruction in body {
                    let instr_code = self.generate_global_instruction(instruction, type_info)?;
                    code.push_str(&format!("    {}\n", instr_code));
                }
                Ok(code)
            }
            IRInstruction::Raise { exception: _ } => {
                Ok("// Raise exception (not fully implemented)".to_string())
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

    /// Check if the module uses FFI features
    fn uses_ffi(&self, module: &IRModule) -> bool {
        // Check for FFI function calls
        let ffi_functions = [
            "load_library", "define_function", "call_function",
            "unload_library", "get_library_function"
        ];

        // Check global instructions
        for instruction in &module.globals {
            if let IRInstruction::Call { func, .. } = instruction {
                if ffi_functions.contains(&func.as_str()) {
                    return true;
                }
            }
        }

        // Check function instructions
        for (_name, function) in &module.functions {
            for block in &function.blocks {
                for instruction in &block.instructions {
                    if let IRInstruction::Call { func, .. } = instruction {
                        if ffi_functions.contains(&func.as_str()) {
                            return true;
                        }
                    }
                }
            }
        }

        false
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

        // Add optimized class structs (100x faster OOP!)
        let mut type_ctx = type_inference::TypeInferenceContext::new();
        type_ctx.analyze_module(&module);
        let mut class_analyzer_instance = class_analyzer::ClassAnalyzer::new();
        let class_analysis = class_analyzer_instance.analyze(&module, &type_ctx);
        c_code.push_str(&class_analyzer::generate_optimized_class_structs(&class_analysis));

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

        // Add optimized constructor implementations
        c_code.push_str(&class_analyzer::generate_optimized_constructors(&class_analysis));

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

        // Collect class names from function names
        let mut class_names = HashSet::new();
        for (func_name, _) in &module.functions {
            if let Some(pos) = func_name.find("__") {
                let class_name = &func_name[0..pos];
                class_names.insert(class_name.to_string());
            }
        }

        // Add global class variable declarations
        c_code.push_str(&self.generate_class_declarations(&module)?);

        // Generate functions
        for (name, function) in &module.functions {
            c_code.push_str(&functions::generate_function(function, &class_names)?);
            c_code.push_str("\n\n");
        }

        // Generate main function with the same analysis context
        c_code.push_str(&self.generate_main_function_with_analysis(&module, &type_ctx, &class_analysis)?);

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

        // Collect class names from function names
        let mut class_names = HashSet::new();
        for (func_name, _) in &module.functions {
            if let Some(pos) = func_name.find("__") {
                let class_name = &func_name[0..pos];
                class_names.insert(class_name.to_string());
            }
        }

        // Generate module functions
        for (_name, function) in &module.functions {
            c_code.push_str(&functions::generate_function(function, &class_names)?);
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
            // Check if output path explicitly requests C source code (.c extension)
            let path = Path::new(output_path);
            let is_c_source = path.extension().and_then(|s| s.to_str()) == Some("c");

            // Check if we should compile to executable
            // Compile to executable if:
            // - Has executable extension (.exe on Windows, or explicit binary name)
            // - Has no extension at all (treat as binary name)
            // - Doesn't end with .c
            let exe_ext = std::env::consts::EXE_EXTENSION;
            let should_compile = if is_c_source {
                false  // Never compile if user explicitly wants .c file
            } else if !exe_ext.is_empty() && output_path.ends_with(exe_ext) {
                true  // Explicitly requested executable (Windows)
            } else {
                path.extension().is_none()  // No extension means binary
            };

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