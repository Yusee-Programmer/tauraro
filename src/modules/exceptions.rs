/// Exception module - provides built-in exception classes with proper hierarchy
/// Similar to Python's built-in exceptions

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;

/// Create the exceptions module with all built-in exception classes
pub fn create_exceptions_module() -> Value {
    let mut namespace = HashMap::new();
    
    // BaseException - the root of all built-in exceptions
    namespace.insert("BaseException".to_string(), create_base_exception_class());
    
    // SystemExit - raised by sys.exit()
    namespace.insert("SystemExit".to_string(), create_system_exit_class());
    
    // KeyboardInterrupt - raised when user hits Ctrl+C
    namespace.insert("KeyboardInterrupt".to_string(), create_keyboard_interrupt_class());
    
    // GeneratorExit - raised when generator.close() is called
    namespace.insert("GeneratorExit".to_string(), create_generator_exit_class());
    
    // Exception - the base class for all built-in non-system-exiting exceptions
    namespace.insert("Exception".to_string(), create_exception_class());
    
    // StopIteration - raised by next() when no more items
    namespace.insert("StopIteration".to_string(), create_stop_iteration_class());
    
    // StopAsyncIteration - raised by async iterator's __anext__()
    namespace.insert("StopAsyncIteration".to_string(), create_stop_async_iteration_class());
    
    // ArithmeticError - base class for arithmetic errors
    namespace.insert("ArithmeticError".to_string(), create_arithmetic_error_class());
    
    // FloatingPointError - raised when floating point operation fails
    namespace.insert("FloatingPointError".to_string(), create_floating_point_error_class());
    
    // OverflowError - raised when result is too large
    namespace.insert("OverflowError".to_string(), create_overflow_error_class());
    
    // ZeroDivisionError - raised when dividing by zero
    namespace.insert("ZeroDivisionError".to_string(), create_zero_division_error_class());
    
    // AssertionError - raised by assert statement
    namespace.insert("AssertionError".to_string(), create_assertion_error_class());
    
    // AttributeError - raised when attribute reference fails
    namespace.insert("AttributeError".to_string(), create_attribute_error_class());
    
    // BufferError - raised when buffer operation fails
    namespace.insert("BufferError".to_string(), create_buffer_error_class());
    
    // EOFError - raised when input() hits EOF
    namespace.insert("EOFError".to_string(), create_eof_error_class());
    
    // ImportError - raised when import fails
    namespace.insert("ImportError".to_string(), create_import_error_class());
    
    // ModuleNotFoundError - raised when module not found
    namespace.insert("ModuleNotFoundError".to_string(), create_module_not_found_error_class());
    
    // LookupError - base class for lookup errors
    namespace.insert("LookupError".to_string(), create_lookup_error_class());
    
    // IndexError - raised when sequence index is out of range
    namespace.insert("IndexError".to_string(), create_index_error_class());
    
    // KeyError - raised when dict key is not found
    namespace.insert("KeyError".to_string(), create_key_error_class());
    
    // MemoryError - raised when operation runs out of memory
    namespace.insert("MemoryError".to_string(), create_memory_error_class());
    
    // NameError - raised when name is not found
    namespace.insert("NameError".to_string(), create_name_error_class());
    
    // UnboundLocalError - raised when local variable is referenced before assignment
    namespace.insert("UnboundLocalError".to_string(), create_unbound_local_error_class());
    
    // OSError - raised when system function returns system-related error
    namespace.insert("OSError".to_string(), create_os_error_class());
    
    // BlockingIOError - raised when operation would block on non-blocking object
    namespace.insert("BlockingIOError".to_string(), create_blocking_io_error_class());
    
    // ChildProcessError - raised when operation on child process fails
    namespace.insert("ChildProcessError".to_string(), create_child_process_error_class());
    
    // ConnectionError - base class for connection-related issues
    namespace.insert("ConnectionError".to_string(), create_connection_error_class());
    
    // BrokenPipeError - raised when trying to write on closed pipe
    namespace.insert("BrokenPipeError".to_string(), create_broken_pipe_error_class());
    
    // ConnectionAbortedError - raised when connection attempt is aborted
    namespace.insert("ConnectionAbortedError".to_string(), create_connection_aborted_error_class());
    
    // ConnectionRefusedError - raised when connection attempt is refused
    namespace.insert("ConnectionRefusedError".to_string(), create_connection_refused_error_class());
    
    // ConnectionResetError - raised when connection is reset
    namespace.insert("ConnectionResetError".to_string(), create_connection_reset_error_class());
    
    // FileExistsError - raised when trying to create existing file/directory
    namespace.insert("FileExistsError".to_string(), create_file_exists_error_class());
    
    // FileNotFoundError - raised when file/directory doesn't exist
    namespace.insert("FileNotFoundError".to_string(), create_file_not_found_error_class());
    
    // InterruptedError - raised when system call is interrupted by signal
    namespace.insert("InterruptedError".to_string(), create_interrupted_error_class());
    
    // IsADirectoryError - raised when file operation is requested on directory
    namespace.insert("IsADirectoryError".to_string(), create_is_a_directory_error_class());
    
    // NotADirectoryError - raised when directory operation is requested on non-directory
    namespace.insert("NotADirectoryError".to_string(), create_not_a_directory_error_class());
    
    // PermissionError - raised when operation lacks adequate access rights
    namespace.insert("PermissionError".to_string(), create_permission_error_class());
    
    // ProcessLookupError - raised when given process doesn't exist
    namespace.insert("ProcessLookupError".to_string(), create_process_lookup_error_class());
    
    // TimeoutError - raised when system function timed out
    namespace.insert("TimeoutError".to_string(), create_timeout_error_class());
    
    // ReferenceError - raised when weak reference proxy is used after referent is garbage collected
    namespace.insert("ReferenceError".to_string(), create_reference_error_class());
    
    // RuntimeError - raised when error is detected that doesn't fall in other categories
    namespace.insert("RuntimeError".to_string(), create_runtime_error_class());
    
    // NotImplementedError - raised when abstract method is not implemented
    namespace.insert("NotImplementedError".to_string(), create_not_implemented_error_class());
    
    // RecursionError - raised when maximum recursion depth is exceeded
    namespace.insert("RecursionError".to_string(), create_recursion_error_class());
    
    // SyntaxError - raised when parser encounters syntax error
    namespace.insert("SyntaxError".to_string(), create_syntax_error_class());
    
    // IndentationError - base class for indentation-related syntax errors
    namespace.insert("IndentationError".to_string(), create_indentation_error_class());
    
    // TabError - raised when indentation contains inconsistent use of tabs and spaces
    namespace.insert("TabError".to_string(), create_tab_error_class());
    
    // SystemError - raised when interpreter finds internal error
    namespace.insert("SystemError".to_string(), create_system_error_class());
    
    // TypeError - raised when operation is applied to object of inappropriate type
    namespace.insert("TypeError".to_string(), create_type_error_class());
    
    // ValueError - raised when operation receives argument of right type but inappropriate value
    namespace.insert("ValueError".to_string(), create_value_error_class());
    
    // UnicodeError - raised when Unicode-related encoding/decoding error occurs
    namespace.insert("UnicodeError".to_string(), create_unicode_error_class());
    
    // UnicodeDecodeError - raised when Unicode-related error occurs during decoding
    namespace.insert("UnicodeDecodeError".to_string(), create_unicode_decode_error_class());
    
    // UnicodeEncodeError - raised when Unicode-related error occurs during encoding
    namespace.insert("UnicodeEncodeError".to_string(), create_unicode_encode_error_class());
    
    // UnicodeTranslateError - raised when Unicode-related error occurs during translating
    namespace.insert("UnicodeTranslateError".to_string(), create_unicode_translate_error_class());
    
    // Warning - base class for all warning category classes
    namespace.insert("Warning".to_string(), create_warning_class());
    
    // DeprecationWarning - base category for warnings about deprecated features
    namespace.insert("DeprecationWarning".to_string(), create_deprecation_warning_class());
    
    // PendingDeprecationWarning - base category for warnings about features that will be deprecated
    namespace.insert("PendingDeprecationWarning".to_string(), create_pending_deprecation_warning_class());
    
    // RuntimeWarning - base category for warnings about dubious runtime behavior
    namespace.insert("RuntimeWarning".to_string(), create_runtime_warning_class());
    
    // SyntaxWarning - base category for warnings about dubious syntax
    namespace.insert("SyntaxWarning".to_string(), create_syntax_warning_class());
    
    // UserWarning - the default category for warn()
    namespace.insert("UserWarning".to_string(), create_user_warning_class());
    
    // FutureWarning - base category for warnings about constructs that will change semantically
    namespace.insert("FutureWarning".to_string(), create_future_warning_class());
    
    // ImportWarning - base category for warnings about probable mistakes in module imports
    namespace.insert("ImportWarning".to_string(), create_import_warning_class());
    
    // UnicodeWarning - base category for warnings related to Unicode
    namespace.insert("UnicodeWarning".to_string(), create_unicode_warning_class());
    
    // BytesWarning - base category for warnings related to bytes and bytearray
    namespace.insert("BytesWarning".to_string(), create_bytes_warning_class());
    
    // ResourceWarning - base category for warnings about resource usage
    namespace.insert("ResourceWarning".to_string(), create_resource_warning_class());
    
    Value::Module("exceptions".to_string(), namespace)
}

// Helper function to create a class with proper inheritance
fn create_exception_class_with_inheritance(class_name: &str, base_classes: Vec<String>) -> Value {
    let mut fields = HashMap::new();
    fields.insert("__name__".to_string(), Value::Str(class_name.to_string()));
    
    // Clone base_classes for use in MRO computation since the original will be moved
    let base_classes_clone = base_classes.clone();
    
    Value::Object {
        class_name: class_name.to_string(),
        fields,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new(class_name.to_string(), base_classes),
        mro: crate::base_object::MRO::from_linearization(
            std::iter::once(class_name.to_string())
                .chain(base_classes_clone.into_iter())
                .chain(std::iter::once("object".to_string()))
                .collect()
        ),
    }
}

// BaseException class
fn create_base_exception_class() -> Value {
    create_exception_class_with_inheritance("BaseException", vec![])
}

// SystemExit class
fn create_system_exit_class() -> Value {
    create_exception_class_with_inheritance("SystemExit", vec!["BaseException".to_string()])
}

// KeyboardInterrupt class
fn create_keyboard_interrupt_class() -> Value {
    create_exception_class_with_inheritance("KeyboardInterrupt", vec!["BaseException".to_string()])
}

// GeneratorExit class
fn create_generator_exit_class() -> Value {
    create_exception_class_with_inheritance("GeneratorExit", vec!["BaseException".to_string()])
}

// Exception class
fn create_exception_class() -> Value {
    create_exception_class_with_inheritance("Exception", vec!["BaseException".to_string()])
}

// StopIteration class
fn create_stop_iteration_class() -> Value {
    create_exception_class_with_inheritance("StopIteration", vec!["Exception".to_string()])
}

// StopAsyncIteration class
fn create_stop_async_iteration_class() -> Value {
    create_exception_class_with_inheritance("StopAsyncIteration", vec!["Exception".to_string()])
}

// ArithmeticError class
fn create_arithmetic_error_class() -> Value {
    create_exception_class_with_inheritance("ArithmeticError", vec!["Exception".to_string()])
}

// FloatingPointError class
fn create_floating_point_error_class() -> Value {
    create_exception_class_with_inheritance("FloatingPointError", vec!["ArithmeticError".to_string()])
}

// OverflowError class
fn create_overflow_error_class() -> Value {
    create_exception_class_with_inheritance("OverflowError", vec!["ArithmeticError".to_string()])
}

// ZeroDivisionError class
fn create_zero_division_error_class() -> Value {
    create_exception_class_with_inheritance("ZeroDivisionError", vec!["ArithmeticError".to_string()])
}

// AssertionError class
fn create_assertion_error_class() -> Value {
    create_exception_class_with_inheritance("AssertionError", vec!["Exception".to_string()])
}

// AttributeError class
fn create_attribute_error_class() -> Value {
    create_exception_class_with_inheritance("AttributeError", vec!["Exception".to_string()])
}

// BufferError class
fn create_buffer_error_class() -> Value {
    create_exception_class_with_inheritance("BufferError", vec!["Exception".to_string()])
}

// EOFError class
fn create_eof_error_class() -> Value {
    create_exception_class_with_inheritance("EOFError", vec!["Exception".to_string()])
}

// ImportError class
fn create_import_error_class() -> Value {
    create_exception_class_with_inheritance("ImportError", vec!["Exception".to_string()])
}

// ModuleNotFoundError class
fn create_module_not_found_error_class() -> Value {
    create_exception_class_with_inheritance("ModuleNotFoundError", vec!["ImportError".to_string()])
}

// LookupError class
fn create_lookup_error_class() -> Value {
    create_exception_class_with_inheritance("LookupError", vec!["Exception".to_string()])
}

// IndexError class
fn create_index_error_class() -> Value {
    create_exception_class_with_inheritance("IndexError", vec!["LookupError".to_string()])
}

// KeyError class
fn create_key_error_class() -> Value {
    create_exception_class_with_inheritance("KeyError", vec!["LookupError".to_string()])
}

// MemoryError class
fn create_memory_error_class() -> Value {
    create_exception_class_with_inheritance("MemoryError", vec!["Exception".to_string()])
}

// NameError class
fn create_name_error_class() -> Value {
    create_exception_class_with_inheritance("NameError", vec!["Exception".to_string()])
}

// UnboundLocalError class
fn create_unbound_local_error_class() -> Value {
    create_exception_class_with_inheritance("UnboundLocalError", vec!["NameError".to_string()])
}

// OSError class
fn create_os_error_class() -> Value {
    create_exception_class_with_inheritance("OSError", vec!["Exception".to_string()])
}

// BlockingIOError class
fn create_blocking_io_error_class() -> Value {
    create_exception_class_with_inheritance("BlockingIOError", vec!["OSError".to_string()])
}

// ChildProcessError class
fn create_child_process_error_class() -> Value {
    create_exception_class_with_inheritance("ChildProcessError", vec!["OSError".to_string()])
}

// ConnectionError class
fn create_connection_error_class() -> Value {
    create_exception_class_with_inheritance("ConnectionError", vec!["OSError".to_string()])
}

// BrokenPipeError class
fn create_broken_pipe_error_class() -> Value {
    create_exception_class_with_inheritance("BrokenPipeError", vec!["ConnectionError".to_string()])
}

// ConnectionAbortedError class
fn create_connection_aborted_error_class() -> Value {
    create_exception_class_with_inheritance("ConnectionAbortedError", vec!["ConnectionError".to_string()])
}

// ConnectionRefusedError class
fn create_connection_refused_error_class() -> Value {
    create_exception_class_with_inheritance("ConnectionRefusedError", vec!["ConnectionError".to_string()])
}

// ConnectionResetError class
fn create_connection_reset_error_class() -> Value {
    create_exception_class_with_inheritance("ConnectionResetError", vec!["ConnectionError".to_string()])
}

// FileExistsError class
fn create_file_exists_error_class() -> Value {
    create_exception_class_with_inheritance("FileExistsError", vec!["OSError".to_string()])
}

// FileNotFoundError class
fn create_file_not_found_error_class() -> Value {
    create_exception_class_with_inheritance("FileNotFoundError", vec!["OSError".to_string()])
}

// InterruptedError class
fn create_interrupted_error_class() -> Value {
    create_exception_class_with_inheritance("InterruptedError", vec!["OSError".to_string()])
}

// IsADirectoryError class
fn create_is_a_directory_error_class() -> Value {
    create_exception_class_with_inheritance("IsADirectoryError", vec!["OSError".to_string()])
}

// NotADirectoryError class
fn create_not_a_directory_error_class() -> Value {
    create_exception_class_with_inheritance("NotADirectoryError", vec!["OSError".to_string()])
}

// PermissionError class
fn create_permission_error_class() -> Value {
    create_exception_class_with_inheritance("PermissionError", vec!["OSError".to_string()])
}

// ProcessLookupError class
fn create_process_lookup_error_class() -> Value {
    create_exception_class_with_inheritance("ProcessLookupError", vec!["OSError".to_string()])
}

// TimeoutError class
fn create_timeout_error_class() -> Value {
    create_exception_class_with_inheritance("TimeoutError", vec!["OSError".to_string()])
}

// ReferenceError class
fn create_reference_error_class() -> Value {
    create_exception_class_with_inheritance("ReferenceError", vec!["Exception".to_string()])
}

// RuntimeError class
fn create_runtime_error_class() -> Value {
    create_exception_class_with_inheritance("RuntimeError", vec!["Exception".to_string()])
}

// NotImplementedError class
fn create_not_implemented_error_class() -> Value {
    create_exception_class_with_inheritance("NotImplementedError", vec!["RuntimeError".to_string()])
}

// RecursionError class
fn create_recursion_error_class() -> Value {
    create_exception_class_with_inheritance("RecursionError", vec!["RuntimeError".to_string()])
}

// SyntaxError class
fn create_syntax_error_class() -> Value {
    create_exception_class_with_inheritance("SyntaxError", vec!["Exception".to_string()])
}

// IndentationError class
fn create_indentation_error_class() -> Value {
    create_exception_class_with_inheritance("IndentationError", vec!["SyntaxError".to_string()])
}

// TabError class
fn create_tab_error_class() -> Value {
    create_exception_class_with_inheritance("TabError", vec!["IndentationError".to_string()])
}

// SystemError class
fn create_system_error_class() -> Value {
    create_exception_class_with_inheritance("SystemError", vec!["Exception".to_string()])
}

// TypeError class
fn create_type_error_class() -> Value {
    create_exception_class_with_inheritance("TypeError", vec!["Exception".to_string()])
}

// ValueError class
fn create_value_error_class() -> Value {
    create_exception_class_with_inheritance("ValueError", vec!["Exception".to_string()])
}

// UnicodeError class
fn create_unicode_error_class() -> Value {
    create_exception_class_with_inheritance("UnicodeError", vec!["ValueError".to_string()])
}

// UnicodeDecodeError class
fn create_unicode_decode_error_class() -> Value {
    create_exception_class_with_inheritance("UnicodeDecodeError", vec!["UnicodeError".to_string()])
}

// UnicodeEncodeError class
fn create_unicode_encode_error_class() -> Value {
    create_exception_class_with_inheritance("UnicodeEncodeError", vec!["UnicodeError".to_string()])
}

// UnicodeTranslateError class
fn create_unicode_translate_error_class() -> Value {
    create_exception_class_with_inheritance("UnicodeTranslateError", vec!["UnicodeError".to_string()])
}

// Warning class
fn create_warning_class() -> Value {
    create_exception_class_with_inheritance("Warning", vec!["Exception".to_string()])
}

// DeprecationWarning class
fn create_deprecation_warning_class() -> Value {
    create_exception_class_with_inheritance("DeprecationWarning", vec!["Warning".to_string()])
}

// PendingDeprecationWarning class
fn create_pending_deprecation_warning_class() -> Value {
    create_exception_class_with_inheritance("PendingDeprecationWarning", vec!["Warning".to_string()])
}

// RuntimeWarning class
fn create_runtime_warning_class() -> Value {
    create_exception_class_with_inheritance("RuntimeWarning", vec!["Warning".to_string()])
}

// SyntaxWarning class
fn create_syntax_warning_class() -> Value {
    create_exception_class_with_inheritance("SyntaxWarning", vec!["Warning".to_string()])
}

// UserWarning class
fn create_user_warning_class() -> Value {
    create_exception_class_with_inheritance("UserWarning", vec!["Warning".to_string()])
}

// FutureWarning class
fn create_future_warning_class() -> Value {
    create_exception_class_with_inheritance("FutureWarning", vec!["Warning".to_string()])
}

// ImportWarning class
fn create_import_warning_class() -> Value {
    create_exception_class_with_inheritance("ImportWarning", vec!["Warning".to_string()])
}

// UnicodeWarning class
fn create_unicode_warning_class() -> Value {
    create_exception_class_with_inheritance("UnicodeWarning", vec!["Warning".to_string()])
}

// BytesWarning class
fn create_bytes_warning_class() -> Value {
    create_exception_class_with_inheritance("BytesWarning", vec!["Warning".to_string()])
}

// ResourceWarning class
fn create_resource_warning_class() -> Value {
    create_exception_class_with_inheritance("ResourceWarning", vec!["Warning".to_string()])
}

// Exception creation functions that can be used by other modules
pub fn create_exception(class_name: &str, message: &str, args: Vec<Value>) -> Value {
    let mut fields = HashMap::new();
    fields.insert("message".to_string(), Value::Str(message.to_string()));
    fields.insert("args".to_string(), Value::Tuple(args));
    
    // Default to Exception if class not found in hierarchy
    let base_classes = match class_name {
        "SystemExit" => vec!["BaseException".to_string()],
        "KeyboardInterrupt" => vec!["BaseException".to_string()],
        "GeneratorExit" => vec!["BaseException".to_string()],
        "StopIteration" => vec!["Exception".to_string()],
        "StopAsyncIteration" => vec!["Exception".to_string()],
        "ArithmeticError" => vec!["Exception".to_string()],
        "FloatingPointError" => vec!["ArithmeticError".to_string()],
        "OverflowError" => vec!["ArithmeticError".to_string()],
        "ZeroDivisionError" => vec!["ArithmeticError".to_string()],
        "AssertionError" => vec!["Exception".to_string()],
        "AttributeError" => vec!["Exception".to_string()],
        "BufferError" => vec!["Exception".to_string()],
        "EOFError" => vec!["Exception".to_string()],
        "ImportError" => vec!["Exception".to_string()],
        "ModuleNotFoundError" => vec!["ImportError".to_string()],
        "LookupError" => vec!["Exception".to_string()],
        "IndexError" => vec!["LookupError".to_string()],
        "KeyError" => vec!["LookupError".to_string()],
        "MemoryError" => vec!["Exception".to_string()],
        "NameError" => vec!["Exception".to_string()],
        "UnboundLocalError" => vec!["NameError".to_string()],
        "OSError" => vec!["Exception".to_string()],
        "BlockingIOError" => vec!["OSError".to_string()],
        "ChildProcessError" => vec!["OSError".to_string()],
        "ConnectionError" => vec!["OSError".to_string()],
        "BrokenPipeError" => vec!["ConnectionError".to_string()],
        "ConnectionAbortedError" => vec!["ConnectionError".to_string()],
        "ConnectionRefusedError" => vec!["ConnectionError".to_string()],
        "ConnectionResetError" => vec!["ConnectionError".to_string()],
        "FileExistsError" => vec!["OSError".to_string()],
        "FileNotFoundError" => vec!["OSError".to_string()],
        "InterruptedError" => vec!["OSError".to_string()],
        "IsADirectoryError" => vec!["OSError".to_string()],
        "NotADirectoryError" => vec!["OSError".to_string()],
        "PermissionError" => vec!["OSError".to_string()],
        "ProcessLookupError" => vec!["OSError".to_string()],
        "TimeoutError" => vec!["OSError".to_string()],
        "ReferenceError" => vec!["Exception".to_string()],
        "RuntimeError" => vec!["Exception".to_string()],
        "NotImplementedError" => vec!["RuntimeError".to_string()],
        "RecursionError" => vec!["RuntimeError".to_string()],
        "SyntaxError" => vec!["Exception".to_string()],
        "IndentationError" => vec!["SyntaxError".to_string()],
        "TabError" => vec!["IndentationError".to_string()],
        "SystemError" => vec!["Exception".to_string()],
        "TypeError" => vec!["Exception".to_string()],
        "ValueError" => vec!["Exception".to_string()],
        "UnicodeError" => vec!["ValueError".to_string()],
        "UnicodeDecodeError" => vec!["UnicodeError".to_string()],
        "UnicodeEncodeError" => vec!["UnicodeError".to_string()],
        "UnicodeTranslateError" => vec!["UnicodeError".to_string()],
        "Warning" => vec!["Exception".to_string()],
        "DeprecationWarning" => vec!["Warning".to_string()],
        "PendingDeprecationWarning" => vec!["Warning".to_string()],
        "RuntimeWarning" => vec!["Warning".to_string()],
        "SyntaxWarning" => vec!["Warning".to_string()],
        "UserWarning" => vec!["Warning".to_string()],
        "FutureWarning" => vec!["Warning".to_string()],
        "ImportWarning" => vec!["Warning".to_string()],
        "UnicodeWarning" => vec!["Warning".to_string()],
        "BytesWarning" => vec!["Warning".to_string()],
        "ResourceWarning" => vec!["Warning".to_string()],
        _ => vec!["Exception".to_string()], // Default to Exception
    };
    
    // Clone base_classes for use in MRO computation since the original will be moved
    let base_classes_clone = base_classes.clone();
    
    Value::Object {
        class_name: class_name.to_string(),
        fields,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new(class_name.to_string(), base_classes),
        mro: crate::base_object::MRO::from_linearization(
            std::iter::once(class_name.to_string())
                .chain(base_classes_clone.into_iter())
                .chain(std::iter::once("object".to_string()))
                .collect()
        ),
    }
}

// Specific exception creation functions
pub fn create_value_error(message: &str) -> Value {
    create_exception("ValueError", message, vec![Value::Str(message.to_string())])
}

pub fn create_type_error(message: &str) -> Value {
    create_exception("TypeError", message, vec![Value::Str(message.to_string())])
}

pub fn create_index_error(message: &str) -> Value {
    create_exception("IndexError", message, vec![Value::Str(message.to_string())])
}

pub fn create_key_error(message: &str) -> Value {
    create_exception("KeyError", message, vec![Value::Str(message.to_string())])
}

pub fn create_attribute_error(message: &str) -> Value {
    create_exception("AttributeError", message, vec![Value::Str(message.to_string())])
}

pub fn create_name_error(message: &str) -> Value {
    create_exception("NameError", message, vec![Value::Str(message.to_string())])
}

pub fn create_import_error(message: &str) -> Value {
    create_exception("ImportError", message, vec![Value::Str(message.to_string())])
}

pub fn create_syntax_error(message: &str) -> Value {
    create_exception("SyntaxError", message, vec![Value::Str(message.to_string())])
}

pub fn create_os_error(message: &str) -> Value {
    create_exception("OSError", message, vec![Value::Str(message.to_string())])
}

pub fn create_runtime_error(message: &str) -> Value {
    create_exception("RuntimeError", message, vec![Value::Str(message.to_string())])
}