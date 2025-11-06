//! Central module system entry point

use crate::value::Value;
use std::collections::HashMap;

// Re-export all submodules
pub mod abc;
pub mod asyncio;
pub mod base64;
pub mod collections;
pub mod copy;
pub mod csv;
pub mod datetime;
pub mod exceptions;
pub mod functools;
pub mod gc;
pub mod hashlib;
pub mod hplist;
pub mod httptools;
pub mod httpx;
pub mod io;
pub mod itertools;
pub mod json;
pub mod logging;
pub mod math;
pub mod memory;
pub mod multiprocessing;
pub mod orm;
pub mod os;
pub mod pickle;
pub mod random;
pub mod re;
pub mod serveit;
pub mod socket;
pub mod subprocess;
pub mod sys;
pub mod templa;
pub mod threading;
pub mod time;
pub mod unittest;
pub mod urllib;
pub mod websockets;

// Re-export commonly used items
pub use crate::modules::hplist::HPList;

/// Initialize all built-in modules and return them as a HashMap
pub fn init_builtin_modules() -> HashMap<String, Value> {
    let mut modules = HashMap::new();

    // Add built-in modules
    modules.insert("abc".to_string(), abc::create_abc_module());
    modules.insert("os".to_string(), os::create_os_module());
    modules.insert("sys".to_string(), sys::create_sys_module());
    modules.insert("threading".to_string(), threading::create_threading_module());
    modules.insert("time".to_string(), time::create_time_module());
    modules.insert("datetime".to_string(), datetime::create_datetime_module());
    modules.insert("io".to_string(), io::create_io_module());
    modules.insert("math".to_string(), math::create_math_module());
    modules.insert("random".to_string(), random::create_random_module());
    modules.insert("re".to_string(), re::create_re_module());
    modules.insert("json".to_string(), json::create_json_module());
    modules.insert("functools".to_string(), functools::create_functools_module());
    modules.insert("itertools".to_string(), itertools::create_itertools_module());
    modules.insert("collections".to_string(), collections::create_collections_module());
    modules.insert("copy".to_string(), copy::create_copy_module());
    modules.insert("pickle".to_string(), pickle::create_pickle_module());
    modules.insert("base64".to_string(), base64::create_base64_module());
    modules.insert("hashlib".to_string(), hashlib::create_hashlib_module());
    modules.insert("urllib".to_string(), urllib::create_urllib_module());
    modules.insert("csv".to_string(), csv::create_csv_module());
    modules.insert("logging".to_string(), logging::create_logging_module());
    modules.insert("unittest".to_string(), unittest::create_unittest_module());
    modules.insert("socket".to_string(), socket::create_socket_module());
    modules.insert("asyncio".to_string(), asyncio::create_asyncio_module());
    modules.insert("httptools".to_string(), httptools::create_httptools_module());
    modules.insert("websockets".to_string(), websockets::create_websockets_module());
    modules.insert("httpx".to_string(), httpx::create_httpx_module());
    modules.insert("serveit".to_string(), serveit::create_serveit_module());
    modules.insert("templa".to_string(), templa::create_templa_module());
    modules.insert("orm".to_string(), orm::create_orm_module());

    // Add process management modules
    modules.insert("subprocess".to_string(), subprocess::create_subprocess_module());
    modules.insert("multiprocessing".to_string(), multiprocessing::create_multiprocessing_module());

    // Add memory management modules
    modules.insert("memory".to_string(), memory::create_memory_module());
    modules.insert("gc".to_string(), gc::create_gc_module());

    // Add exceptions module
    modules.insert("exceptions".to_string(), exceptions::create_exceptions_module());
    
    modules
}

/// Get a specific built-in module by name
pub fn get_builtin_module(name: &str) -> Option<Value> {
    match name {
        "abc" => Some(abc::create_abc_module()),
        "os" => Some(os::create_os_module()),
        "sys" => Some(sys::create_sys_module()),
        "threading" => Some(threading::create_threading_module()),
        "time" => Some(time::create_time_module()),
        "datetime" => Some(datetime::create_datetime_module()),
        "io" => Some(io::create_io_module()),
        "math" => Some(math::create_math_module()),
        "random" => Some(random::create_random_module()),
        "re" => Some(re::create_re_module()),
        "json" => Some(json::create_json_module()),
        "functools" => Some(functools::create_functools_module()),
        "itertools" => Some(itertools::create_itertools_module()),
        "collections" => Some(collections::create_collections_module()),
        "copy" => Some(copy::create_copy_module()),
        "pickle" => Some(pickle::create_pickle_module()),
        "base64" => Some(base64::create_base64_module()),
        "hashlib" => Some(hashlib::create_hashlib_module()),
        "urllib" => Some(urllib::create_urllib_module()),
        "csv" => Some(csv::create_csv_module()),
        "logging" => Some(logging::create_logging_module()),
        "unittest" => Some(unittest::create_unittest_module()),
        "socket" => Some(socket::create_socket_module()),
        "asyncio" => Some(asyncio::create_asyncio_module()),
        "httptools" => Some(httptools::create_httptools_module()),
        "websockets" => Some(websockets::create_websockets_module()),
        "httpx" => Some(httpx::create_httpx_module()),
        "serveit" => Some(serveit::create_serveit_module()),
        "templa" => Some(templa::create_templa_module()),
        "orm" => Some(orm::create_orm_module()),
        "subprocess" => Some(subprocess::create_subprocess_module()),
        "multiprocessing" => Some(multiprocessing::create_multiprocessing_module()),
        "memory" => Some(memory::create_memory_module()),
        "gc" => Some(gc::create_gc_module()),
        "exceptions" => Some(exceptions::create_exceptions_module()),
        _ => None,
    }
}

/// Check if a module name is a built-in module
pub fn is_builtin_module(name: &str) -> bool {
    matches!(name, "abc" | "os" | "sys" | "threading" | "time" | "datetime" | "io" | "math" | "random" | "re" | "json" | "functools" | "itertools" | "collections" | "copy" | "pickle" | "base64" | "hashlib" | "urllib" | "csv" | "logging" | "unittest" | "socket" | "asyncio" | "httptools" | "websockets" | "httpx" | "serveit" | "templa" | "orm" | "subprocess" | "multiprocessing" | "memory" | "gc" | "exceptions")
}

/// Get list of all built-in module names
pub fn get_builtin_module_names() -> Vec<String> {
    vec![
        "abc".to_string(),
        "os".to_string(),
        "sys".to_string(),
        "threading".to_string(),
        "time".to_string(),
        "datetime".to_string(),
        "io".to_string(),
        "math".to_string(),
        "random".to_string(),
        "re".to_string(),
        "json".to_string(),
        "functools".to_string(),
        "itertools".to_string(),
        "collections".to_string(),
        "copy".to_string(),
        "pickle".to_string(),
        "base64".to_string(),
        "hashlib".to_string(),
        "urllib".to_string(),
        "csv".to_string(),
        "logging".to_string(),
        "unittest".to_string(),
        "socket".to_string(),
        "asyncio".to_string(),
        "httptools".to_string(),
        "websockets".to_string(),
        "httpx".to_string(),
        "serveit".to_string(),
        "templa".to_string(),
        "orm".to_string(),
        "subprocess".to_string(),
        "multiprocessing".to_string(),
        "memory".to_string(),
        "gc".to_string(),
        "exceptions".to_string(),
    ]
}