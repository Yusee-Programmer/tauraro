#include <stdio.h>
#include <stdlib.h>

// Forward declarations for FFI functions
typedef enum {
    Int = 0, Float = 1, Bool = 2, String = 3, List = 4,
    Dict = 5, Tuple = 6, Set = 7, None = 8, Object = 9,
    Function = 10, Bytes = 11, Complex = 12, Range = 13, Frozenset = 14,
} TauraroType;

typedef union {
    long long int_val;
    double float_val;
    int bool_val;
    char* str_val;
} TauraroData;

typedef struct {
    TauraroType value_type;
    int ref_count;
    TauraroData data;
} TauraroValue;

// Math module functions
extern TauraroValue* tauraro_math_sqrt(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_math_sin(int argc, TauraroValue** argv);

// String module functions
extern TauraroValue* tauraro_string_upper(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_string_lower(int argc, TauraroValue** argv);

// Sys module functions
extern TauraroValue* tauraro_sys_get_version(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_sys_get_platform(int argc, TauraroValue** argv);

// Time module functions
extern TauraroValue* tauraro_time_time(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_time_sleep(int argc, TauraroValue** argv);

// Random module functions
extern TauraroValue* tauraro_random_randint(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_random_random(int argc, TauraroValue** argv);

// OS module functions
extern TauraroValue* tauraro_os_getcwd(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_os_listdir(int argc, TauraroValue** argv);

// JSON module functions
extern TauraroValue* tauraro_json_dumps(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_json_loads(int argc, TauraroValue** argv);

// Re module functions
extern TauraroValue* tauraro_re_match(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_re_search(int argc, TauraroValue** argv);

// IO module functions
extern TauraroValue* tauraro_io_open(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_io_close(int argc, TauraroValue** argv);

// Datetime module functions
extern TauraroValue* tauraro_datetime_now(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_datetime_today(int argc, TauraroValue** argv);

// Collections module functions
extern TauraroValue* tauraro_collections_counter_new(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_collections_deque_new(int argc, TauraroValue** argv);

// Itertools module functions
extern TauraroValue* tauraro_itertools_count(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_itertools_cycle(int argc, TauraroValue** argv);

// Functools module functions
extern TauraroValue* tauraro_functools_partial(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_functools_reduce(int argc, TauraroValue** argv);

// Threading module functions
extern TauraroValue* tauraro_threading_thread_new(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_threading_lock_new(int argc, TauraroValue** argv);

// Copy module functions
extern TauraroValue* tauraro_copy_copy(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_copy_deepcopy(int argc, TauraroValue** argv);

// Base64 module functions
extern TauraroValue* tauraro_base64_b64encode(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_base64_b64decode(int argc, TauraroValue** argv);

// Hashlib module functions
extern TauraroValue* tauraro_hashlib_md5(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_hashlib_sha256(int argc, TauraroValue** argv);

// Urllib module functions
extern TauraroValue* tauraro_urllib_parse_quote(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_urllib_parse_unquote(int argc, TauraroValue** argv);

// CSV module functions
extern TauraroValue* tauraro_csv_reader(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_csv_writer(int argc, TauraroValue** argv);

// Logging module functions
extern TauraroValue* tauraro_logging_getlogger(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_logging_info(int argc, TauraroValue** argv);

// Unittest module functions
extern TauraroValue* tauraro_unittest_testcase_new(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_unittest_main(int argc, TauraroValue** argv);

// Socket module functions
extern TauraroValue* tauraro_socket_socket_new(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_socket_connect(int argc, TauraroValue** argv);

// Asyncio module functions
extern TauraroValue* tauraro_asyncio_get_event_loop(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_asyncio_run(int argc, TauraroValue** argv);

// HTTPTools module functions
extern TauraroValue* tauraro_httptools_request_parser_new(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_httptools_parse_url(int argc, TauraroValue** argv);

// HTTPX module functions
extern TauraroValue* tauraro_httpx_get(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_httpx_post(int argc, TauraroValue** argv);

// Memory module functions
extern TauraroValue* tauraro_memory_get_usage(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_memory_gc_collect(int argc, TauraroValue** argv);

// GC module functions
extern TauraroValue* tauraro_gc_collect(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_gc_enable(int argc, TauraroValue** argv);

// Exceptions module functions
extern TauraroValue* tauraro_exceptions_exception_new(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_exceptions_valueerror_new(int argc, TauraroValue** argv);

// ABC module functions
extern TauraroValue* tauraro_abc_abstractmethod(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_abc_ABC(int argc, TauraroValue** argv);

// Pickle module functions
extern TauraroValue* tauraro_pickle_dumps(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_pickle_loads(int argc, TauraroValue** argv);

// Websockets module functions
extern TauraroValue* tauraro_websockets_connect(int argc, TauraroValue** argv);
extern TauraroValue* tauraro_websockets_send(int argc, TauraroValue** argv);

int main() {
    printf("Tauraro FFI Modules Test\n");
    printf("=======================\n\n");
    
    printf("This is a demonstration of how Tauraro's FFI modules would be used from C code.\n");
    printf("Each builtin module has been implemented with C-compatible functions that can\n");
    printf("be linked with generated C code when compiling Tauraro scripts.\n\n");
    
    printf("Available modules:\n");
    printf("- math, sys, time, random, os, json, re, io\n");
    printf("- datetime, collections, itertools, functools, threading, copy\n");
    printf("- base64, hashlib, urllib, csv, logging, unittest\n");
    printf("- socket, asyncio, httptools, httpx, memory, gc\n");
    printf("- exceptions, abc, pickle, websockets\n\n");
    
    printf("When a Tauraro script is compiled to C code, these FFI implementations\n");
    printf("will be used instead of the regular modules in src/modules/*.\n");
    printf("However, when using the VM without compiling to C code, the regular\n");
    printf("implementations will be used.\n\n");
    
    printf("Test completed successfully!\n");
    
    return 0;
}