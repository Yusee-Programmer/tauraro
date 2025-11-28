//! Built-in functions and values for Tauraro

use crate::value::Value;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use crate::modules::hplist::HPList;
use crate::base_object::{BaseObject, MRO};
use anyhow::anyhow;

pub fn init_builtins() -> HashMap<String, Value> {
    let mut builtins = HashMap::new();
    
    // None value
    builtins.insert("None".to_string(), Value::None);
    
    // Boolean values
    builtins.insert("True".to_string(), Value::Bool(true));
    builtins.insert("False".to_string(), Value::Bool(false));
    
    // Built-in functions
    builtins.insert("print".to_string(), Value::BuiltinFunction("print".to_string(), print_builtin));
    builtins.insert("len".to_string(), Value::BuiltinFunction("len".to_string(), len_builtin));
    builtins.insert("str".to_string(), Value::BuiltinFunction("str".to_string(), str_builtin));
    builtins.insert("int".to_string(), Value::BuiltinFunction("int".to_string(), int_builtin));
    builtins.insert("float".to_string(), Value::BuiltinFunction("float".to_string(), float_builtin));
    builtins.insert("bool".to_string(), Value::BuiltinFunction("bool".to_string(), bool_builtin));
    builtins.insert("list".to_string(), Value::BuiltinFunction("list".to_string(), list_builtin));
    builtins.insert("tuple".to_string(), Value::BuiltinFunction("tuple".to_string(), tuple_builtin));
    builtins.insert("dict".to_string(), Value::BuiltinFunction("dict".to_string(), dict_builtin));
    builtins.insert("range".to_string(), Value::BuiltinFunction("range".to_string(), range_builtin));
    builtins.insert("input".to_string(), Value::BuiltinFunction("input".to_string(), input_builtin));
    builtins.insert("abs".to_string(), Value::BuiltinFunction("abs".to_string(), abs_builtin));
    builtins.insert("min".to_string(), Value::BuiltinFunction("min".to_string(), min_builtin));
    builtins.insert("max".to_string(), Value::BuiltinFunction("max".to_string(), max_builtin));
    builtins.insert("sum".to_string(), Value::BuiltinFunction("sum".to_string(), sum_builtin));
    builtins.insert("round".to_string(), Value::BuiltinFunction("round".to_string(), round_builtin));
    builtins.insert("pow".to_string(), Value::BuiltinFunction("pow".to_string(), pow_builtin));
    builtins.insert("divmod".to_string(), Value::BuiltinFunction("divmod".to_string(), divmod_builtin));
    builtins.insert("enumerate".to_string(), Value::BuiltinFunction("enumerate".to_string(), enumerate_builtin));
    builtins.insert("zip".to_string(), Value::BuiltinFunction("zip".to_string(), zip_builtin));
    builtins.insert("map".to_string(), Value::BuiltinFunction("map".to_string(), map_builtin));
    builtins.insert("filter".to_string(), Value::BuiltinFunction("filter".to_string(), filter_builtin));
    builtins.insert("sorted".to_string(), Value::BuiltinFunction("sorted".to_string(), sorted_builtin));
    builtins.insert("reversed".to_string(), Value::BuiltinFunction("reversed".to_string(), reversed_builtin));
    builtins.insert("any".to_string(), Value::BuiltinFunction("any".to_string(), any_builtin));
    builtins.insert("all".to_string(), Value::BuiltinFunction("all".to_string(), all_builtin));
    builtins.insert("chr".to_string(), Value::BuiltinFunction("chr".to_string(), chr_builtin));
    builtins.insert("ord".to_string(), Value::BuiltinFunction("ord".to_string(), ord_builtin));
    builtins.insert("hex".to_string(), Value::BuiltinFunction("hex".to_string(), hex_builtin));
    builtins.insert("bin".to_string(), Value::BuiltinFunction("bin".to_string(), bin_builtin));
    builtins.insert("oct".to_string(), Value::BuiltinFunction("oct".to_string(), oct_builtin));
    builtins.insert("isinstance".to_string(), Value::BuiltinFunction("isinstance".to_string(), isinstance_builtin));
    builtins.insert("type".to_string(), Value::BuiltinFunction("type".to_string(), type_builtin));
    builtins.insert("callable".to_string(), Value::BuiltinFunction("callable".to_string(), callable_builtin));
    builtins.insert("hasattr".to_string(), Value::BuiltinFunction("hasattr".to_string(), hasattr_builtin));
    builtins.insert("getattr".to_string(), Value::BuiltinFunction("getattr".to_string(), getattr_builtin));
    builtins.insert("setattr".to_string(), Value::BuiltinFunction("setattr".to_string(), setattr_builtin));
    builtins.insert("delattr".to_string(), Value::BuiltinFunction("delattr".to_string(), delattr_builtin));
    builtins.insert("issubclass".to_string(), Value::BuiltinFunction("issubclass".to_string(), issubclass_builtin));
    builtins.insert("super".to_string(), Value::BuiltinFunction("super".to_string(), super_builtin));
    builtins.insert("staticmethod".to_string(), Value::BuiltinFunction("staticmethod".to_string(), staticmethod_builtin));
    builtins.insert("classmethod".to_string(), Value::BuiltinFunction("classmethod".to_string(), classmethod_builtin));
    builtins.insert("property".to_string(), Value::BuiltinFunction("property".to_string(), property_builtin));
    builtins.insert("dataclass".to_string(), Value::BuiltinFunction("dataclass".to_string(), dataclass_builtin));
    builtins.insert("Enum".to_string(), Value::BuiltinFunction("Enum".to_string(), enum_builtin));
    builtins.insert("open".to_string(), Value::BuiltinFunction("open".to_string(), open_builtin));
    builtins.insert("iter".to_string(), Value::BuiltinFunction("iter".to_string(), iter_builtin));
    builtins.insert("next".to_string(), Value::BuiltinFunction("next".to_string(), next_builtin));
    builtins.insert("id".to_string(), Value::BuiltinFunction("id".to_string(), id_builtin));
    builtins.insert("hash".to_string(), Value::BuiltinFunction("hash".to_string(), hash_builtin));
    builtins.insert("help".to_string(), Value::BuiltinFunction("help".to_string(), help_builtin));
    builtins.insert("dir".to_string(), Value::BuiltinFunction("dir".to_string(), dir_builtin));
    builtins.insert("vars".to_string(), Value::BuiltinFunction("vars".to_string(), vars_builtin));
    builtins.insert("locals".to_string(), Value::BuiltinFunction("locals".to_string(), locals_builtin));
    builtins.insert("globals".to_string(), Value::BuiltinFunction("globals".to_string(), globals_builtin));
    builtins.insert("eval".to_string(), Value::BuiltinFunction("eval".to_string(), eval_builtin));
    builtins.insert("exec".to_string(), Value::BuiltinFunction("exec".to_string(), exec_builtin));
    builtins.insert("compile".to_string(), Value::BuiltinFunction("compile".to_string(), compile_builtin));
    builtins.insert("repr".to_string(), Value::BuiltinFunction("repr".to_string(), repr_builtin));
    builtins.insert("ascii".to_string(), Value::BuiltinFunction("ascii".to_string(), ascii_builtin));
    builtins.insert("format".to_string(), Value::BuiltinFunction("format".to_string(), format_builtin));
    
    // Exception classes - Tier 1: Currently implemented (10 exceptions)
    // Phase 1: High priority (production-ready) (9 new exceptions = 19 total)
    builtins.insert("Exception".to_string(), Value::BuiltinFunction("Exception".to_string(), exception_builtin));
    builtins.insert("ValueError".to_string(), Value::BuiltinFunction("ValueError".to_string(), value_error_builtin));
    builtins.insert("TypeError".to_string(), Value::BuiltinFunction("TypeError".to_string(), type_error_builtin));
    builtins.insert("RuntimeError".to_string(), Value::BuiltinFunction("RuntimeError".to_string(), runtime_error_builtin));
    builtins.insert("ZeroDivisionError".to_string(), Value::BuiltinFunction("ZeroDivisionError".to_string(), zero_division_error_builtin));
    builtins.insert("IndexError".to_string(), Value::BuiltinFunction("IndexError".to_string(), index_error_builtin));
    builtins.insert("KeyError".to_string(), Value::BuiltinFunction("KeyError".to_string(), key_error_builtin));
    builtins.insert("NameError".to_string(), Value::BuiltinFunction("NameError".to_string(), name_error_builtin));
    builtins.insert("AttributeError".to_string(), Value::BuiltinFunction("AttributeError".to_string(), attribute_error_builtin));
    builtins.insert("AssertionError".to_string(), Value::BuiltinFunction("AssertionError".to_string(), assertion_error_builtin));

    // Phase 1: High-priority exceptions for production use (9 new)
    builtins.insert("ImportError".to_string(), Value::BuiltinFunction("ImportError".to_string(), import_error_builtin));
    builtins.insert("ModuleNotFoundError".to_string(), Value::BuiltinFunction("ModuleNotFoundError".to_string(), module_not_found_error_builtin));
    builtins.insert("FileNotFoundError".to_string(), Value::BuiltinFunction("FileNotFoundError".to_string(), file_not_found_error_builtin));
    builtins.insert("IOError".to_string(), Value::BuiltinFunction("IOError".to_string(), io_error_builtin));
    builtins.insert("OSError".to_string(), Value::BuiltinFunction("OSError".to_string(), os_error_builtin));
    builtins.insert("NotImplementedError".to_string(), Value::BuiltinFunction("NotImplementedError".to_string(), not_implemented_error_builtin));
    builtins.insert("RecursionError".to_string(), Value::BuiltinFunction("RecursionError".to_string(), recursion_error_builtin));
    builtins.insert("SyntaxError".to_string(), Value::BuiltinFunction("SyntaxError".to_string(), syntax_error_builtin));
    builtins.insert("IndentationError".to_string(), Value::BuiltinFunction("IndentationError".to_string(), indentation_error_builtin));

    // Phase 2: Medium-priority exceptions (8 exceptions = 27 total)
    builtins.insert("StopIteration".to_string(), Value::BuiltinFunction("StopIteration".to_string(), stop_iteration_builtin));
    builtins.insert("LookupError".to_string(), Value::BuiltinFunction("LookupError".to_string(), lookup_error_builtin));
    builtins.insert("EOFError".to_string(), Value::BuiltinFunction("EOFError".to_string(), eof_error_builtin));
    builtins.insert("PermissionError".to_string(), Value::BuiltinFunction("PermissionError".to_string(), permission_error_builtin));
    builtins.insert("TimeoutError".to_string(), Value::BuiltinFunction("TimeoutError".to_string(), timeout_error_builtin));
    builtins.insert("KeyboardInterrupt".to_string(), Value::BuiltinFunction("KeyboardInterrupt".to_string(), keyboard_interrupt_builtin));
    builtins.insert("SystemExit".to_string(), Value::BuiltinFunction("SystemExit".to_string(), system_exit_builtin));
    builtins.insert("OverflowError".to_string(), Value::BuiltinFunction("OverflowError".to_string(), overflow_error_builtin));

    // Phase 3: Unicode and Warning exceptions (11 exceptions = 38 total)
    builtins.insert("UnicodeError".to_string(), Value::BuiltinFunction("UnicodeError".to_string(), unicode_error_builtin));
    builtins.insert("UnicodeDecodeError".to_string(), Value::BuiltinFunction("UnicodeDecodeError".to_string(), unicode_decode_error_builtin));
    builtins.insert("UnicodeEncodeError".to_string(), Value::BuiltinFunction("UnicodeEncodeError".to_string(), unicode_encode_error_builtin));
    builtins.insert("Warning".to_string(), Value::BuiltinFunction("Warning".to_string(), warning_builtin));
    builtins.insert("DeprecationWarning".to_string(), Value::BuiltinFunction("DeprecationWarning".to_string(), deprecation_warning_builtin));
    builtins.insert("RuntimeWarning".to_string(), Value::BuiltinFunction("RuntimeWarning".to_string(), runtime_warning_builtin));
    builtins.insert("SyntaxWarning".to_string(), Value::BuiltinFunction("SyntaxWarning".to_string(), syntax_warning_builtin));
    builtins.insert("UserWarning".to_string(), Value::BuiltinFunction("UserWarning".to_string(), user_warning_builtin));
    builtins.insert("FutureWarning".to_string(), Value::BuiltinFunction("FutureWarning".to_string(), future_warning_builtin));
    builtins.insert("GeneratorExit".to_string(), Value::BuiltinFunction("GeneratorExit".to_string(), generator_exit_builtin));
    builtins.insert("MemoryError".to_string(), Value::BuiltinFunction("MemoryError".to_string(), memory_error_builtin));

    // Phase 4: Advanced exceptions (25+ exceptions = 63+ total)
    builtins.insert("SystemError".to_string(), Value::BuiltinFunction("SystemError".to_string(), system_error_builtin));
    builtins.insert("ReferenceError".to_string(), Value::BuiltinFunction("ReferenceError".to_string(), reference_error_builtin));
    builtins.insert("BufferError".to_string(), Value::BuiltinFunction("BufferError".to_string(), buffer_error_builtin));
    builtins.insert("ArithmeticError".to_string(), Value::BuiltinFunction("ArithmeticError".to_string(), arithmetic_error_builtin));
    builtins.insert("FloatingPointError".to_string(), Value::BuiltinFunction("FloatingPointError".to_string(), floating_point_error_builtin));
    builtins.insert("ConnectionError".to_string(), Value::BuiltinFunction("ConnectionError".to_string(), connection_error_builtin));
    builtins.insert("BrokenPipeError".to_string(), Value::BuiltinFunction("BrokenPipeError".to_string(), broken_pipe_error_builtin));
    builtins.insert("ChildProcessError".to_string(), Value::BuiltinFunction("ChildProcessError".to_string(), child_process_error_builtin));
    builtins.insert("ConnectionAbortedError".to_string(), Value::BuiltinFunction("ConnectionAbortedError".to_string(), connection_aborted_error_builtin));
    builtins.insert("ConnectionRefusedError".to_string(), Value::BuiltinFunction("ConnectionRefusedError".to_string(), connection_refused_error_builtin));
    builtins.insert("ConnectionResetError".to_string(), Value::BuiltinFunction("ConnectionResetError".to_string(), connection_reset_error_builtin));
    builtins.insert("FileExistsError".to_string(), Value::BuiltinFunction("FileExistsError".to_string(), file_exists_error_builtin));
    builtins.insert("InterruptedError".to_string(), Value::BuiltinFunction("InterruptedError".to_string(), interrupted_error_builtin));
    builtins.insert("IsADirectoryError".to_string(), Value::BuiltinFunction("IsADirectoryError".to_string(), is_a_directory_error_builtin));
    builtins.insert("NotADirectoryError".to_string(), Value::BuiltinFunction("NotADirectoryError".to_string(), not_a_directory_error_builtin));
    builtins.insert("ProcessLookupError".to_string(), Value::BuiltinFunction("ProcessLookupError".to_string(), process_lookup_error_builtin));
    builtins.insert("UnboundLocalError".to_string(), Value::BuiltinFunction("UnboundLocalError".to_string(), unbound_local_error_builtin));
    builtins.insert("UnicodeTranslateError".to_string(), Value::BuiltinFunction("UnicodeTranslateError".to_string(), unicode_translate_error_builtin));
    builtins.insert("TabError".to_string(), Value::BuiltinFunction("TabError".to_string(), tab_error_builtin));
    builtins.insert("ImportWarning".to_string(), Value::BuiltinFunction("ImportWarning".to_string(), import_warning_builtin));
    builtins.insert("UnicodeWarning".to_string(), Value::BuiltinFunction("UnicodeWarning".to_string(), unicode_warning_builtin));
    builtins.insert("BytesWarning".to_string(), Value::BuiltinFunction("BytesWarning".to_string(), bytes_warning_builtin));
    builtins.insert("ResourceWarning".to_string(), Value::BuiltinFunction("ResourceWarning".to_string(), resource_warning_builtin));
    builtins.insert("PendingDeprecationWarning".to_string(), Value::BuiltinFunction("PendingDeprecationWarning".to_string(), pending_deprecation_warning_builtin));
    builtins.insert("BlockingIOError".to_string(), Value::BuiltinFunction("BlockingIOError".to_string(), blocking_io_error_builtin));
    #[cfg(feature = "ffi")]
    {
        builtins.insert("load_library".to_string(), Value::BuiltinFunction("load_library".to_string(), ffi_load_library));
        builtins.insert("define_function".to_string(), Value::BuiltinFunction("define_function".to_string(), ffi_define_function));
        builtins.insert("call_function".to_string(), Value::BuiltinFunction("call_function".to_string(), ffi_call_function));
        builtins.insert("unload_library".to_string(), Value::BuiltinFunction("unload_library".to_string(), ffi_unload_library));
        builtins.insert("list_libraries".to_string(), Value::BuiltinFunction("list_libraries".to_string(), ffi_list_libraries));
        builtins.insert("library_info".to_string(), Value::BuiltinFunction("library_info".to_string(), ffi_library_info));
        builtins.insert("add_library_path".to_string(), Value::BuiltinFunction("add_library_path".to_string(), ffi_add_library_path));
        builtins.insert("allocate_buffer".to_string(), Value::BuiltinFunction("allocate_buffer".to_string(), ffi_allocate_buffer));
        builtins.insert("free_buffer".to_string(), Value::BuiltinFunction("free_buffer".to_string(), ffi_free_buffer));
    }

    // Memory management builtins
    builtins.insert("allocate".to_string(), Value::BuiltinFunction("allocate".to_string(), memory_allocate_builtin));
    builtins.insert("free".to_string(), Value::BuiltinFunction("free".to_string(), memory_free_builtin));
    builtins.insert("create_arena".to_string(), Value::BuiltinFunction("create_arena".to_string(), memory_create_arena_builtin));
    builtins.insert("destroy_arena".to_string(), Value::BuiltinFunction("destroy_arena".to_string(), memory_destroy_arena_builtin));
    builtins.insert("reset_arena".to_string(), Value::BuiltinFunction("reset_arena".to_string(), memory_reset_arena_builtin));
    builtins.insert("memory_stats".to_string(), Value::BuiltinFunction("memory_stats".to_string(), memory_stats_builtin));

    // System programming builtins
    builtins.insert("sizeof".to_string(), Value::BuiltinFunction("sizeof".to_string(), sizeof_builtin));
    builtins.insert("alignof".to_string(), Value::BuiltinFunction("alignof".to_string(), alignof_builtin));
    builtins.insert("memcpy".to_string(), Value::BuiltinFunction("memcpy".to_string(), memcpy_builtin));
    builtins.insert("memset".to_string(), Value::BuiltinFunction("memset".to_string(), memset_builtin));
    builtins.insert("memmove".to_string(), Value::BuiltinFunction("memmove".to_string(), memmove_builtin));
    builtins.insert("memcmp".to_string(), Value::BuiltinFunction("memcmp".to_string(), memcmp_builtin));
    builtins.insert("ptr_read".to_string(), Value::BuiltinFunction("ptr_read".to_string(), ptr_read_builtin));
    builtins.insert("ptr_write".to_string(), Value::BuiltinFunction("ptr_write".to_string(), ptr_write_builtin));
    builtins.insert("ptr_offset".to_string(), Value::BuiltinFunction("ptr_offset".to_string(), ptr_offset_builtin));
    builtins.insert("null_ptr".to_string(), Value::BuiltinFunction("null_ptr".to_string(), null_ptr_builtin));
    builtins.insert("is_null".to_string(), Value::BuiltinFunction("is_null".to_string(), is_null_builtin));

    // Memory management decorators (for VM compatibility, they're identity functions)
    builtins.insert("manual_memory".to_string(), Value::BuiltinFunction("manual_memory".to_string(), decorator_identity));
    builtins.insert("arena_memory".to_string(), Value::BuiltinFunction("arena_memory".to_string(), decorator_identity));
    builtins.insert("auto_memory".to_string(), Value::BuiltinFunction("auto_memory".to_string(), decorator_identity));

    // Base object class (implicit parent of all classes)
    let object_class = Value::Class {
        name: "object".to_string(),
        bases: vec![],
        methods: HashMap::new(),
        attributes: Rc::new(RefCell::new(HashMap::new())),
        metaclass: Some(Box::new(Value::Str("type".to_string()))),
        mro: MRO::from_linearization(vec!["object".to_string()]),
        base_object: BaseObject::new("object".to_string(), vec![]),
    };
    builtins.insert("object".to_string(), object_class);

    builtins
}

fn print_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let output = args.iter().map(|arg| {
        // Call str() on each argument to support __str__ method
        let str_value = str_builtin(vec![arg.clone()]).unwrap_or_else(|_| Value::Str(format!("{}", arg)));
        // Extract the string from Value::Str
        match str_value {
            Value::Str(s) => s,
            _ => format!("{}", str_value)
        }
    }).collect::<Vec<_>>().join(" ");
    println!("{}", output);
    Ok(Value::None)
}

fn len_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("TypeError: len() takes exactly one argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Str(s) => Ok(Value::Int(s.len() as i64)),
        Value::List(items) => Ok(Value::Int(items.len() as i64)),
        Value::Tuple(items) => Ok(Value::Int(items.len() as i64)),
        Value::Dict(dict) => Ok(Value::Int(dict.borrow().len() as i64)),
        Value::Set(items) => Ok(Value::Int(items.len() as i64)),
        Value::FrozenSet(items) => Ok(Value::Int(items.len() as i64)),
        Value::Bytes(bytes) => Ok(Value::Int(bytes.len() as i64)),
        Value::ByteArray(bytes) => Ok(Value::Int(bytes.len() as i64)),
        _ => Err(anyhow::anyhow!("TypeError: object of type '{}' has no len()", args[0].type_name())),
    }
}

fn str_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Str(String::new()))
    } else if args.len() == 1 {
        // Check if the object has a __str__ method
        let value = &args[0];

        match value {
            Value::Object { class_methods, .. } => {
                // Try to get __str__ method
                if let Some(str_method) = class_methods.get("__str__") {
                    // Call the __str__ method with self as argument
                    match str_method {
                        Value::Closure { .. } => {
                            // For closures, we can't call them directly from builtins
                            // Fall back to default representation
                            // This would need VM support to call properly
                            Ok(Value::Str(format!("{}", value)))
                        }
                        Value::BuiltinFunction(_, func) => {
                            let result = func(vec![value.clone()])?;
                            // The result should be a string
                            if let Value::Str(s) = result {
                                Ok(Value::Str(s))
                            } else {
                                Ok(Value::Str(format!("{}", result)))
                            }
                        }
                        _ => Ok(Value::Str(format!("{}", value)))
                    }
                } else {
                    Ok(Value::Str(format!("{}", value)))
                }
            }
            _ => Ok(Value::Str(format!("{}", value)))
        }
    } else {
        Err(anyhow::anyhow!("str() takes at most 1 argument ({} given)", args.len()))
    }
}

fn int_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Int(0))
    } else if args.len() == 1 {
        args[0].to_int()
    } else {
        Err(anyhow::anyhow!("int() takes at most 1 argument ({} given)", args.len()))
    }
}

fn float_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Float(0.0))
    } else if args.len() == 1 {
        args[0].to_float()
    } else {
        Err(anyhow::anyhow!("float() takes at most 1 argument ({} given)", args.len()))
    }
}

fn bool_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Bool(false))
    } else if args.len() == 1 {
        Ok(Value::Bool(args[0].is_truthy()))
    } else {
        Err(anyhow::anyhow!("bool() takes at most 1 argument ({} given)", args.len()))
    }
}

fn list_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::List(HPList::new()))
    } else if args.len() == 1 {
        args[0].to_list()
    } else {
        Err(anyhow::anyhow!("list() takes at most 1 argument ({} given)", args.len()))
    }
}

fn tuple_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Tuple(Vec::new()))
    } else if args.len() == 1 {
        args[0].to_tuple()
    } else {
        Err(anyhow::anyhow!("tuple() takes at most 1 argument ({} given)", args.len()))
    }
}

fn dict_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Dict(Rc::new(RefCell::new(HashMap::new()))))
    } else if args.len() == 1 {
        match &args[0] {
            Value::Dict(dict) => Ok(Value::Dict(dict.clone())),
            Value::List(items) => {
                let mut dict = HashMap::new();
                for item in items.as_vec().iter() {
                    match item {
                        Value::Tuple(pair) if pair.len() == 2 => {
                            if let (Value::Str(key), value) = (&pair[0], &pair[1]) {
                                dict.insert(key.clone(), value.clone());
                            } else {
                                return Err(anyhow::anyhow!("dictionary update sequence element #0 has wrong type"));
                            }
                        }
                        _ => return Err(anyhow::anyhow!("dictionary update sequence element #0 has wrong type")),
                    }
                }
                Ok(Value::Dict(Rc::new(RefCell::new(dict))))
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", args[0].type_name())),
        }
    } else {
        Err(anyhow::anyhow!("dict() takes at most 1 argument ({} given)", args.len()))
    }
}

fn range_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    match args.len() {
        1 => {
            if let Value::Int(stop) = args[0] {
                Ok(Value::Range { start: 0, stop, step: 1 })
            } else {
                Err(anyhow::anyhow!("range() argument must be an integer"))
            }
        }
        2 => {
            if let (Value::Int(start), Value::Int(stop)) = (&args[0], &args[1]) {
                Ok(Value::Range { start: *start, stop: *stop, step: 1 })
            } else {
                Err(anyhow::anyhow!("range() arguments must be integers"))
            }
        }
        3 => {
            if let (Value::Int(start), Value::Int(stop), Value::Int(step)) = (&args[0], &args[1], &args[2]) {
                if *step == 0 {
                    Err(anyhow::anyhow!("range() step argument must not be zero"))
                } else {
                    Ok(Value::Range { start: *start, stop: *stop, step: *step })
                }
            } else {
                Err(anyhow::anyhow!("range() arguments must be integers"))
            }
        }
        _ => Err(anyhow::anyhow!("range() takes 1 to 3 arguments ({} given)", args.len())),
    }
}

fn input_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    use std::io::Write;

    if args.len() > 1 {
        return Err(anyhow::anyhow!("input() takes at most 1 argument ({} given)", args.len()));
    }

    // Print prompt and flush stdout to ensure it appears before reading input
    if !args.is_empty() {
        print!("{}", args[0]);
        std::io::stdout().flush()?;
    }

    let mut input = String::new();
    std::io::stdin().read_line(&mut input)?;
    // Remove trailing newline
    if input.ends_with('\n') {
        input.pop();
        if input.ends_with('\r') {
            input.pop();
        }
    }

    Ok(Value::Str(input))
}

fn abs_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("abs() takes exactly one argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Int(n.abs())),
        Value::Float(f) => Ok(Value::Float(f.abs())),
        Value::Complex { real, imag } => {
            Ok(Value::Float((real * real + imag * imag).sqrt()))
        }
        _ => Err(anyhow::anyhow!("bad operand type for abs(): '{}'", args[0].type_name())),
    }
}

fn min_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("TypeError: min() expected at least 1 argument, got 0"));
    }
    
    if args.len() == 1 {
        // Single iterable argument
        match &args[0] {
            Value::List(items) => {
                if items.is_empty() {
                    return Err(anyhow::anyhow!("ValueError: min() arg is an empty sequence"));
                }
                let mut min_val = items.get(0).unwrap().clone();
                for item in items.iter().skip(1) {
                    if item.partial_cmp(&min_val) == Some(std::cmp::Ordering::Less) {
                        min_val = item.clone();
                    }
                }
                Ok(min_val)
            }
            Value::Tuple(items) => {
                if items.is_empty() {
                    return Err(anyhow::anyhow!("ValueError: min() arg is an empty sequence"));
                }
                let mut min_val = items[0].clone();
                for item in items.iter().skip(1) {
                    if item.partial_cmp(&min_val) == Some(std::cmp::Ordering::Less) {
                        min_val = item.clone();
                    }
                }
                Ok(min_val)
            }
            _ => Err(anyhow::anyhow!("TypeError: '{}' object is not iterable", args[0].type_name())),
        }
    } else {
        // Multiple arguments
        let mut min_val = args[0].clone();
        for arg in args.iter().skip(1) {
            if arg.partial_cmp(&min_val) == Some(std::cmp::Ordering::Less) {
                min_val = arg.clone();
            }
        }
        Ok(min_val)
    }
}

fn max_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("max() expected at least 1 argument, got 0"));
    }
    
    if args.len() == 1 {
        // Single iterable argument
        match &args[0] {
            Value::List(items) => {
                if items.is_empty() {
                    return Err(anyhow::anyhow!("max() arg is an empty sequence"));
                }
                let mut max_val = items.get(0).unwrap().clone();
                for item in items.iter().skip(1) {
                    if item.partial_cmp(&max_val) == Some(std::cmp::Ordering::Greater) {
                        max_val = item.clone();
                    }
                }
                Ok(max_val)
            }
            Value::Tuple(items) => {
                if items.is_empty() {
                    return Err(anyhow::anyhow!("max() arg is an empty sequence"));
                }
                let mut max_val = items[0].clone();
                for item in items.iter().skip(1) {
                    if item.partial_cmp(&max_val) == Some(std::cmp::Ordering::Greater) {
                        max_val = item.clone();
                    }
                }
                Ok(max_val)
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", args[0].type_name())),
        }
    } else {
        // Multiple arguments
        let mut max_val = args[0].clone();
        for arg in args.iter().skip(1) {
            if arg.partial_cmp(&max_val) == Some(std::cmp::Ordering::Greater) {
                max_val = arg.clone();
            }
        }
        Ok(max_val)
    }
}

fn sum_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("sum() takes at most 2 arguments ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    let start = if args.len() == 2 { &args[1] } else { &Value::Int(0) };
    
    let mut sum = start.clone();
    
    match iterable {
        Value::List(items) => {
            for item in items.iter() {
                sum = match (&sum, &item) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Int(a), Value::Float(b)) => Value::Float(*a as f64 + b),
                    (Value::Float(a), Value::Int(b)) => Value::Float(a + (*b) as f64),
                    _ => return Err(anyhow::anyhow!("unsupported operand type(s) for +: '{}' and '{}'", sum.type_name(), item.type_name())),
                };
            }
            Ok(sum)
        }
        Value::Tuple(items) => {
            for item in items {
                sum = match (&sum, &item) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Int(a), Value::Float(b)) => Value::Float(*a as f64 + b),
                    (Value::Float(a), Value::Int(b)) => Value::Float(a + (*b) as f64),
                    _ => return Err(anyhow::anyhow!("unsupported operand type(s) for +: '{}' and '{}'", sum.type_name(), item.type_name())),
                };
            }
            Ok(sum)
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    }
}

fn round_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("round() takes at most 2 arguments ({} given)", args.len()));
    }
    
    let number = &args[0];
    let ndigits = if args.len() == 2 {
        match &args[1] {
            Value::Int(n) => *n,
            _ => return Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[1].type_name())),
        }
    } else {
        0
    };
    
    match number {
        Value::Int(n) => {
            if ndigits >= 0 {
                Ok(Value::Int(*n))
            } else {
                let factor = 10_i64.pow((-ndigits) as u32);
                let rounded = (*n + factor / 2) / factor * factor;
                Ok(Value::Int(rounded))
            }
        }
        Value::Float(f) => {
            if ndigits >= 0 {
                let factor = 10_f64.powi(ndigits as i32);
                Ok(Value::Float((f * factor).round() / factor))
            } else {
                let factor = 10_f64.powi((-ndigits) as i32);
                Ok(Value::Float((f / factor).round() * factor))
            }
        }
        _ => Err(anyhow::anyhow!("type {} doesn't define __round__ method", number.type_name())),
    }
}

fn pow_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("pow() takes 2 or 3 arguments ({} given)", args.len()));
    }
    
    let base = &args[0];
    let exp = &args[1];
    
    if args.len() == 3 {
        // Modular exponentiation
        let modulus = &args[2];
        match (base, exp, modulus) {
            (Value::Int(b), Value::Int(e), Value::Int(m)) => {
                if *e < 0 {
                    return Err(anyhow::anyhow!("pow() 2nd argument cannot be negative when 3rd argument specified"));
                }
                if *m == 0 {
                    return Err(anyhow::anyhow!("pow() 3rd argument cannot be 0"));
                }
                // Simple implementation for now
                Ok(Value::Int(b.pow(*e as u32) % m))
            }
            _ => Err(anyhow::anyhow!("pow() with 3 arguments requires integers")),
        }
    } else {
        // Regular exponentiation
        match (base, exp) {
            (Value::Int(b), Value::Int(e)) => {
                if *e >= 0 {
                    Ok(Value::Int(b.pow(*e as u32)))
                } else {
                    Ok(Value::Float((*b as f64).powf(*e as f64)))
                }
            }
            (Value::Float(b), Value::Float(e)) => Ok(Value::Float(b.powf(*e))),
            (Value::Int(b), Value::Float(e)) => Ok(Value::Float((*b as f64).powf(*e))),
            (Value::Float(b), Value::Int(e)) => Ok(Value::Float(b.powf(*e as f64))),
            _ => Err(anyhow::anyhow!("unsupported operand type(s) for pow()")),
        }
    }
}

fn divmod_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("divmod() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    let a = &args[0];
    let b = &args[1];
    
    match (a, b) {
        (Value::Int(x), Value::Int(y)) => {
            if *y == 0 {
                return Err(anyhow::anyhow!("division by zero"));
            }
            let div = x / y;
            let rem = x % y;
            Ok(Value::Tuple(vec![Value::Int(div), Value::Int(rem)]))
        }
        (Value::Float(x), Value::Float(y)) => {
            if *y == 0.0 {
                return Err(anyhow::anyhow!("division by zero"));
            }
            let div = (x / y).floor();
            let rem = x - div * y;
            Ok(Value::Tuple(vec![Value::Float(div), Value::Float(rem)]))
        }
        (Value::Int(x), Value::Float(y)) => {
            if *y == 0.0 {
                return Err(anyhow::anyhow!("division by zero"));
            }
            let xf = *x as f64;
            let div = (xf / y).floor();
            let rem = xf - div * y;
            Ok(Value::Tuple(vec![Value::Float(div), Value::Float(rem)]))
        }
        (Value::Float(x), Value::Int(y)) => {
            if *y == 0 {
                return Err(anyhow::anyhow!("division by zero"));
            }
            let yf = *y as f64;
            let div = (x / yf).floor();
            let rem = x - div * yf;
            Ok(Value::Tuple(vec![Value::Float(div), Value::Float(rem)]))
        }
        _ => Err(anyhow::anyhow!("unsupported operand type(s) for divmod()")),
    }
}

fn enumerate_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("enumerate() takes at most 2 arguments ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    let start = if args.len() == 2 {
        match &args[1] {
            Value::Int(n) => *n,
            _ => return Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[1].type_name())),
        }
    } else {
        0
    };
    
    match iterable {
        Value::List(items) => {
            let mut result = Vec::new();
            for (i, item) in items.iter().enumerate() {
                result.push(Value::Tuple(vec![Value::Int(start + i as i64), item.clone()]));
            }
            Ok(Value::List(HPList::from_values(result)))
        }
        Value::Tuple(items) => {
            let mut result = Vec::new();
            for (i, item) in items.iter().enumerate() {
                result.push(Value::Tuple(vec![Value::Int(start + i as i64), item.clone()]));
            }
            Ok(Value::List(HPList::from_values(result)))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    }
}

fn zip_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::List(HPList::new()));
    }
    
    // Convert all arguments to vectors
    let mut iterables = Vec::new();
    for arg in &args {
        match arg {
            Value::List(items) => iterables.push(items.as_vec().clone()),
            Value::Tuple(items) => iterables.push(items.clone()),
            _ => return Err(anyhow::anyhow!("'{}' object is not iterable", arg.type_name())),
        }
    }
    
    // Zip the iterables
    let min_len = iterables.iter().map(|v| v.len()).min().unwrap_or(0);
    let mut result = Vec::new();
    
    for i in 0..min_len {
        let mut tuple = Vec::new();
        for iterable in &iterables {
            tuple.push(iterable[i].clone());
        }
        result.push(Value::Tuple(tuple));
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

fn map_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("map() must have at least two arguments"));
    }
    
    let func = &args[0];
    let iterables: Vec<Value> = args.iter().skip(1).cloned().collect();
    
    // Convert iterables to vectors
    let mut iterable_vecs = Vec::new();
    for iterable in &iterables {
        match iterable {
            Value::List(items) => iterable_vecs.push(items.as_vec().clone()),
            Value::Tuple(items) => iterable_vecs.push(items.clone()),
            Value::Set(items) => iterable_vecs.push(items.clone()),
            Value::Str(s) => iterable_vecs.push(s.chars().map(|c| Value::Str(c.to_string())).collect()),
            Value::Range { start, stop, step } => {
                let mut range_items = Vec::new();
                let mut current = *start;
                if *step > 0 {
                    while current < *stop {
                        range_items.push(Value::Int(current));
                        current += *step;
                    }
                } else if *step < 0 {
                    while current > *stop {
                        range_items.push(Value::Int(current));
                        current += *step;
                    }
                }
                iterable_vecs.push(range_items);
            }
            _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
        }
    }
    
    // Map the function over the iterables
    let min_len = iterable_vecs.iter().map(|v| v.len()).min().unwrap_or(0);
    let mut result = Vec::new();
    
    for i in 0..min_len {
        let func_args: Vec<Value> = iterable_vecs.iter().map(|v| v[i].clone()).collect();
        
        // Call the function with the arguments
        let func_result = match func {
            Value::BuiltinFunction(_, f) => f(func_args)?,
            Value::NativeFunction(f) => f(func_args)?,
            Value::Closure { .. } | Value::Code(_) => {
                // For closures and user-defined functions, we can't evaluate from here
                // Return a helpful error message
                return Err(anyhow::anyhow!("map() with user-defined functions not yet supported - use list comprehension instead: [func(x) for x in iterable]"));
            }
            _ => return Err(anyhow::anyhow!("'{}' object is not callable", func.type_name())),
        };
        
        result.push(func_result);
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

fn filter_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("filter() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    let func = &args[0];
    let iterable = &args[1];
    
    // Convert iterable to vector
    let items = match iterable {
        Value::List(items) => items.as_vec().clone(),
        Value::Tuple(items) => items.clone(),
        _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    };
    
    // Filter the items
    let mut result = Vec::new();
    
    for item in items {
        // Call the function with the item
        let func_result = match func {
            Value::BuiltinFunction(_, f) => f(vec![item.clone()])?,
            Value::NativeFunction(f) => f(vec![item.clone()])?,
            Value::None => Value::Bool(item.is_truthy()), // None means use truthiness
            Value::Closure { .. } | Value::Code(_) => {
                return Err(anyhow::anyhow!("filter() with user-defined functions not yet supported - use list comprehension instead: [x for x in iterable if func(x)]"));
            }
            _ => return Err(anyhow::anyhow!("'{}' object is not callable", func.type_name())),
        };
        
        // If the result is truthy, include the item
        if func_result.is_truthy() {
            result.push(item);
        }
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

fn sorted_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("sorted() takes 1 to 3 arguments ({} given)", args.len()));
    }

    let iterable = &args[0];
    let key_fn = if args.len() > 1 { Some(&args[1]) } else { None };
    let reverse = if args.len() > 2 {
        match &args[2] {
            Value::Bool(b) => *b,
            _ => false,
        }
    } else {
        false
    };

    // Get the items to sort
    let items = match iterable {
        Value::List(items) => items.as_vec().clone(),
        Value::Tuple(items) => items.clone(),
        _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    };

    let mut sorted_items = items;

    // Sort with or without key function
    if let Some(key_func) = key_fn {
        // Sort using key function
        sorted_items.sort_by(|a, b| {
            // Call the key function on both values
            let key_a = match key_func {
                Value::BuiltinFunction(_, f) => match f(vec![a.clone()]) {
                    Ok(v) => v,
                    Err(_) => return std::cmp::Ordering::Equal,
                },
                Value::NativeFunction(f) => match f(vec![a.clone()]) {
                    Ok(v) => v,
                    Err(_) => return std::cmp::Ordering::Equal,
                },
                _ => return std::cmp::Ordering::Equal, // Can't handle closures in builtins yet
            };
            let key_b = match key_func {
                Value::BuiltinFunction(_, f) => match f(vec![b.clone()]) {
                    Ok(v) => v,
                    Err(_) => return std::cmp::Ordering::Equal,
                },
                Value::NativeFunction(f) => match f(vec![b.clone()]) {
                    Ok(v) => v,
                    Err(_) => return std::cmp::Ordering::Equal,
                },
                _ => return std::cmp::Ordering::Equal,
            };

            key_a.partial_cmp(&key_b).unwrap_or(std::cmp::Ordering::Equal)
        });
    } else {
        // Sort using natural ordering
        sorted_items.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
    }

    // Reverse if requested
    if reverse {
        sorted_items.reverse();
    }

    Ok(Value::List(HPList::from_values(sorted_items)))
}

fn reversed_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("reversed() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    
    match iterable {
        Value::List(items) => {
            let mut reversed_items = items.as_vec().clone();
            reversed_items.reverse();
            Ok(Value::List(HPList::from_values(reversed_items)))
        }
        Value::Tuple(items) => {
            let mut reversed_items = items.clone();
            reversed_items.reverse();
            Ok(Value::List(HPList::from_values(reversed_items)))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not reversible", iterable.type_name())),
    }
}

fn any_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("any() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    
    match iterable {
        Value::List(items) => {
            for item in items.iter() {
                if item.is_truthy() {
                    return Ok(Value::Bool(true));
                }
            }
            Ok(Value::Bool(false))
        }
        Value::Tuple(items) => {
            for item in items {
                if item.is_truthy() {
                    return Ok(Value::Bool(true));
                }
            }
            Ok(Value::Bool(false))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    }
}

fn all_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("all() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    
    match iterable {
        Value::List(items) => {
            for item in items.iter() {
                if !item.is_truthy() {
                    return Ok(Value::Bool(false));
                }
            }
            Ok(Value::Bool(true))
        }
        Value::Tuple(items) => {
            for item in items {
                if !item.is_truthy() {
                    return Ok(Value::Bool(false));
                }
            }
            Ok(Value::Bool(true))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    }
}

fn chr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("chr() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => {
            if *n < 0 || *n > 0x10FFFF {
                return Err(anyhow::anyhow!("chr() arg not in range(0x110000)"));
            }
            Ok(Value::Str(String::from_utf8(vec![*n as u8]).unwrap_or_else(|_| String::from("\u{FFFD}"))))
        }
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[0].type_name())),
    }
}

fn ord_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("ord() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Str(s) => {
            if s.chars().count() != 1 {
                return Err(anyhow::anyhow!("ord() expected a character, but string of length {} found", s.chars().count()));
            }
            Ok(Value::Int(s.chars().next().unwrap() as i64))
        }
        _ => Err(anyhow::anyhow!("ord() expected string of length 1, but {} found", args[0].type_name())),
    }
}

fn hex_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("hex() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Str(format!("0x{:x}", n))),
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[0].type_name())),
    }
}

fn bin_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("bin() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Str(format!("0b{:b}", n))),
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[0].type_name())),
    }
}

fn oct_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("oct() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Str(format!("0o{:o}", n))),
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[0].type_name())),
    }
}

fn isinstance_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("isinstance() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    let obj = &args[0];
    let type_info = &args[1];
    
    // Handle both string type names and actual class types
    let is_instance = match type_info {
        Value::Str(type_name) => {
            // String-based check
            obj.type_name() == type_name
        },
        Value::BuiltinFunction(name, _) => {
            // Check if object type matches builtin function name
            obj.type_name() == name.as_str()
        },
        Value::Class { name, .. } => {
            // Check if object is instance of this class
            match obj {
                Value::Object { class_name, .. } => {
                    class_name == name
                },
                _ => false
            }
        },
        Value::Tuple(items) => {
            // Check if object is instance of any type in the tuple
            items.iter().any(|item| {
                match item {
                    Value::Str(type_name) => obj.type_name() == type_name,
                    Value::BuiltinFunction(name, _) => obj.type_name() == name.as_str(),
                    Value::Class { name, .. } => {
                        match obj {
                            Value::Object { class_name, .. } => class_name == name,
                            _ => false
                        }
                    },
                    _ => false
                }
            })
        },
        _ => return Err(anyhow::anyhow!("isinstance() arg 2 must be a type, a tuple of types, or a string")),
    };
    
    Ok(Value::Bool(is_instance))
}

fn type_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    match args.len() {
        1 => {
            // type(obj) - get the type of an object
            let obj = &args[0];
            
            // Return the type name as a string
            match obj {
                Value::Object { class_name, .. } => {
                    Ok(Value::Str(class_name.clone()))
                },
                Value::Class { name, .. } => {
                    Ok(Value::Str(name.clone()))
                },
                _ => {
                    Ok(Value::Str(obj.type_name().to_string()))
                }
            }
        },
        3 => {
            // type(name, bases, dict) - create a new class (metaclass form)
            let name = match &args[0] {
                Value::Str(s) => s.clone(),
                _ => return Err(anyhow::anyhow!("type() name argument must be a string")),
            };
            
            let bases = match &args[1] {
                Value::Tuple(bases_vec) => {
                    // Convert tuple of base classes to Vec<String>
                    let mut base_names = Vec::new();
                    for base in bases_vec {
                        match base {
                            Value::Class { name: base_name, .. } => {
                                base_names.push(base_name.clone());
                            },
                            Value::Str(base_name) => {
                                base_names.push(base_name.clone());
                            },
                            _ => return Err(anyhow::anyhow!("bases must be a tuple of classes")),
                        }
                    }
                    base_names
                },
                Value::List(bases_list) => {
                    // Also support list of bases
                    let mut base_names = Vec::new();
                    for i in 0..bases_list.len() {
                        if let Some(base) = bases_list.get(i as isize) {
                            match base {
                                Value::Class { name: base_name, .. } => {
                                    base_names.push(base_name.clone());
                                },
                                Value::Str(base_name) => {
                                    base_names.push(base_name.clone());
                                },
                                _ => return Err(anyhow::anyhow!("bases must be a tuple of classes")),
                            }
                        }
                    }
                    base_names
                },
                _ => return Err(anyhow::anyhow!("type() bases argument must be a tuple or list")),
            };
            
            let dict = match &args[2] {
                Value::Dict(dict_rc) => {
                    // Extract methods and attributes from the dict
                    let dict_borrow = dict_rc.borrow();
                    let mut methods = HashMap::new();
                    let mut attributes = HashMap::new();
                    
                    for (key, value) in dict_borrow.iter() {
                        if key.starts_with("__") && key.ends_with("__") {
                            // Dunder methods/attributes
                            if matches!(value, Value::Closure { .. } | Value::NativeFunction(_) | Value::BuiltinFunction(..)) {
                                methods.insert(key.clone(), value.clone());
                            } else {
                                attributes.insert(key.clone(), value.clone());
                            }
                        } else {
                            // Regular methods or class attributes
                            match value {
                                Value::Closure { .. } | Value::NativeFunction(_) | Value::BuiltinFunction(..) => {
                                    methods.insert(key.clone(), value.clone());
                                },
                                _ => {
                                    attributes.insert(key.clone(), value.clone());
                                }
                            }
                        }
                    }
                    (methods, attributes)
                },
                _ => return Err(anyhow::anyhow!("type() dict argument must be a dictionary")),
            };
            
            // If no bases provided, inherit from object
            let actual_bases = if bases.is_empty() {
                vec!["object".to_string()]
            } else {
                bases
            };
            
            // Create a simple MRO: [ClassName, BaseName1, BaseName2, ..., object]
            // For now, we use a simplified linearization without full C3
            let mut linearization = vec![name.clone()];
            linearization.extend(actual_bases.clone());
            if !linearization.contains(&"object".to_string()) {
                linearization.push("object".to_string());
            }
            
            // Create base object with object base class inheritance
            let base_object = BaseObject::new(name.clone(), actual_bases.clone());
            
            // Create the new class
            Ok(Value::Class {
                name,
                bases: actual_bases,
                methods: dict.0,
                attributes: Rc::new(RefCell::new(dict.1)),
                metaclass: None, // Could be enhanced to support custom metaclasses
                mro: MRO::from_linearization(linearization),
                base_object,
            })
        },
        _ => {
            Err(anyhow::anyhow!("type() takes 1 or 3 arguments ({} given)", args.len()))
        }
    }
}

fn callable_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("callable() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let obj = &args[0];
    let is_callable = match obj {
        Value::BuiltinFunction(_, _) | Value::NativeFunction(_) | Value::Closure { .. } => true,
        _ => false,
    };
    
    Ok(Value::Bool(is_callable))
}

fn hasattr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("hasattr() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    let obj = &args[0];
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("hasattr() attribute name must be string")),
    };
    
    // Try to check if the attribute exists on the object
    let has_attr = match obj {
        Value::Object { fields, class_methods, .. } => {
            // First check fields
            fields.borrow().contains_key(attr_name) || class_methods.contains_key(attr_name)
        },
        Value::Class { methods, attributes, .. } => {
            methods.contains_key(attr_name) || attributes.borrow().contains_key(attr_name)
        },
        Value::Module(_, namespace) => {
            namespace.contains_key(attr_name)
        },
        Value::Dict(dict) => {
            dict.borrow().contains_key(attr_name)
        },
        _ => {
            // For other objects, check if they have a method with this name
            obj.get_method(attr_name).is_some()
        }
    };
    Ok(Value::Bool(has_attr))
}

fn getattr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("getattr() takes 2 or 3 arguments ({} given)", args.len()));
    }
    
    let obj = &args[0];
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("getattr() attribute name must be string")),
    };
    
    // Try to get the attribute from the object
    match obj {
        Value::Object { fields, class_methods, .. } => {
            // First check fields
            if let Some(value) = fields.borrow().get(attr_name) {
                Ok(value.clone())
            }
            // Then check methods
            else if let Some(method) = class_methods.get(attr_name) {
                Ok(method.clone())
            }
            // If not found and default provided, return default
            else if args.len() == 3 {
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        Value::Class { methods, attributes, .. } => {
            // Check class attributes first, then methods
            if let Some(value) = attributes.borrow().get(attr_name) {
                Ok(value.clone())
            }
            else if let Some(method) = methods.get(attr_name) {
                Ok(method.clone())
            }
            // If not found and default provided, return default
            else if args.len() == 3 {
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        Value::Module(_, namespace) => {
            // Check module attributes
            if let Some(value) = namespace.get(attr_name) {
                Ok(value.clone())
            }
            // If not found and default provided, return default
            else if args.len() == 3 {
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        Value::Dict(dict) => {
            // For dictionaries, treat keys as attributes
            if let Some(value) = dict.borrow().get(attr_name) {
                Ok(value.clone())
            }
            // If not found and default provided, return default
            else if args.len() == 3 {
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        _ => {
            // For other objects, try to get method
            if let Some(method) = obj.get_method(attr_name) {
                Ok(method)
            } else if args.len() == 3 {
                // Return default value
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        }
    }
}

fn setattr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 3 {
        return Err(anyhow::anyhow!("setattr() takes exactly 3 arguments ({} given)", args.len()));
    }
    
    let obj = &mut args[0].clone(); // We'll need to make this mutable in a full implementation
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("setattr() attribute name must be string")),
    };
    let value = &args[2];
    
    // For now, we'll just return None as setting attributes requires mutable references
    // In a full implementation, we would modify the object's fields
    Ok(Value::None)
}

fn delattr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("delattr() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as deleting attributes is complex
    Ok(Value::None)
}

fn issubclass_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("issubclass() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    let (subclass, superclass) = (&args[0], &args[1]);
    
    // Extract class names from both arguments
    let sub_name = match subclass {
        Value::Class { name, .. } => name.clone(),
        _ => return Err(anyhow::anyhow!("issubclass() arg 1 must be a class")),
    };
    
    let super_name = match superclass {
        Value::Class { name, .. } => name.clone(),
        Value::Str(s) => s.clone(),  // Allow string class names for convenience
        _ => return Err(anyhow::anyhow!("issubclass() arg 2 must be a class or string")),
    };
    
    // Check if subclass name equals superclass name
    if sub_name == super_name {
        return Ok(Value::Bool(true));
    }
    
    // Check if superclass is in the subclass's MRO (Method Resolution Order)
    if let Value::Class { mro, .. } = subclass {
        let linearization = mro.get_linearization();
        let is_subclass = linearization.contains(&super_name);
        Ok(Value::Bool(is_subclass))
    } else {
        Ok(Value::Bool(false))
    }
}

pub fn super_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    // super() can be called with 0, 1, or 2 arguments
    match args.len() {
        0 => {
            // Zero-argument super() - needs to be handled by the compiler/VM to get context
            // We'll return a special marker value that the VM will handle
            Ok(Value::Super("".to_string(), "".to_string(), None, None))
        }
        1 => {
            // One-argument super(cls)
            if let Value::Class { name, mro, .. } = &args[0] {
                // Create an unbound super object
                Ok(Value::Super(name.clone(), mro.get_linearization().get(1).cloned().unwrap_or_else(|| "object".to_string()), None, None))
            } else {
                Err(anyhow!("super() argument 1 must be type, not {}", args[0].type_name()))
            }
        }
        2 => {
            // Two-argument super(cls, obj)
            if let (Value::Class { name: class_name, mro, .. }, Value::Object { .. }) = (&args[0], &args[1]) {
                // Determine the parent class from the MRO
                let parent_class = if let Some(second_class) = mro.get_linearization().get(1) {
                    second_class.clone()
                } else {
                    "object".to_string()
                };
                
                // Get parent class methods
                let parent_methods = if let Some(parent_class_value) = args[0].get_method(&parent_class) {
                    if let Value::Class { methods, .. } = parent_class_value {
                        Some(methods.clone())
                    } else {
                        None
                    }
                } else {
                    None
                };
                
                // Create the super object with the current class, parent class, instance, and parent methods
                let super_obj = Value::Super(class_name.clone(), parent_class, Some(Box::new(args[1].clone())), parent_methods);
                
                Ok(super_obj)
            } else {
                Err(anyhow!("super() arguments must be type, object"))
            }
        }
        _ => Err(anyhow!("super() takes 0, 1, or 2 arguments, not {}", args.len())),
    }
}

pub fn staticmethod_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("staticmethod() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // Wrap the function in a StaticMethod descriptor
    Ok(Value::StaticMethod {
        method: Box::new(args[0].clone()),
    })
}

pub fn classmethod_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("classmethod() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // We wrap the function in a special marker. The class will be added when accessed.
    // For now, we create a placeholder class - this will be set when accessed from a class
    Ok(Value::ClassMethod {
        method: Box::new(args[0].clone()),
        class: Box::new(Value::None), // Placeholder, will be set during access
    })
}

fn dataclass_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("dataclass() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return the class as-is
    // In a full implementation, we would add __init__, __repr__, __eq__, etc. methods
    Ok(args[0].clone())
}

fn enum_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("Enum() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return the class as-is
    // In a full implementation, we would process the class to create enum members
    Ok(args[0].clone())
}

pub fn property_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 4 {
        return Err(anyhow::anyhow!("property() takes at most 4 arguments ({} given)", args.len()));
    }
    
    // Create a property object that stores the getter, setter, and deleter functions
    let getter = if !args.is_empty() { Some(Box::new(args[0].clone())) } else { None };
    let setter = if args.len() > 1 { Some(Box::new(args[1].clone())) } else { None };
    let deleter = if args.len() > 2 { Some(Box::new(args[2].clone())) } else { None };
    let doc = if args.len() > 3 { 
        match &args[3] {
            Value::Str(s) => Some(s.clone()),
            _ => None
        }
    } else { 
        None 
    };
    
    // Create fields for the property object
    let mut fields = HashMap::new();
    if let Some(getter_fn) = getter {
        fields.insert("fget".to_string(), *getter_fn);
    }
    if let Some(setter_fn) = setter {
        fields.insert("fset".to_string(), *setter_fn);
    }
    if let Some(deleter_fn) = deleter {
        fields.insert("fdel".to_string(), *deleter_fn);
    }
    if let Some(doc_str) = doc {
        fields.insert("__doc__".to_string(), Value::Str(doc_str));
    }
    
    // For now, we'll just return a special Property value that contains the functions
    // In a full implementation, this would be a proper property object
    Ok(Value::Object {
        class_name: "property".to_string(),
        fields: Rc::new(RefCell::new(fields)),
        class_methods: HashMap::new(),
        base_object: BaseObject::new("property".to_string(), vec!["object".to_string()]),
        mro: MRO::from_linearization(vec!["property".to_string(), "object".to_string()]),
    })
}

fn open_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("open() takes at most 3 arguments ({} given)", args.len()));
    }

    // Get filename
    let filename = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow::anyhow!("open() filename must be a string")),
    };

    // Get mode (default is 'r')
    let mode = if args.len() > 1 {
        match &args[1] {
            Value::Str(s) => s.clone(),
            _ => "r".to_string(),
        }
    } else {
        "r".to_string()
    };

    // Get encoding (default is 'utf-8')
    let encoding = if args.len() > 2 {
        match &args[2] {
            Value::Str(s) => s.clone(),
            _ => "utf-8".to_string(),
        }
    } else {
        "utf-8".to_string()
    };

    // Create a file object with context manager support
    let mut file_fields = HashMap::new();
    file_fields.insert("filename".to_string(), Value::Str(filename.clone()));
    file_fields.insert("mode".to_string(), Value::Str(mode.clone()));
    file_fields.insert("encoding".to_string(), Value::Str(encoding));
    file_fields.insert("closed".to_string(), Value::Bool(false));

    // Add file methods
    file_fields.insert("read".to_string(), Value::NativeFunction(file_read));
    file_fields.insert("write".to_string(), Value::NativeFunction(file_write));
    file_fields.insert("close".to_string(), Value::NativeFunction(file_close));
    file_fields.insert("__enter__".to_string(), Value::NativeFunction(file_enter));
    file_fields.insert("__exit__".to_string(), Value::NativeFunction(file_exit));
    file_fields.insert("readline".to_string(), Value::NativeFunction(file_readline));
    file_fields.insert("readlines".to_string(), Value::NativeFunction(file_readlines));
    file_fields.insert("writelines".to_string(), Value::NativeFunction(file_writelines));

    Ok(Value::Object {
        class_name: "file".to_string(),
        fields: Rc::new(RefCell::new(file_fields)),
        class_methods: HashMap::new(),
        base_object: BaseObject::new("file".to_string(), vec!["object".to_string()]),
        mro: MRO::from_linearization(vec!["file".to_string(), "object".to_string()]),
    })
}

// File method implementations
fn file_read(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("read() requires file object as first argument"));
    }

    // Extract filename from file object
    let filename = if let Value::Object { fields, .. } = &args[0] {
        if let Some(Value::Str(name)) = fields.borrow().get("filename") {
            name.clone()
        } else {
            return Err(anyhow::anyhow!("File object missing filename"));
        }
    } else {
        return Err(anyhow::anyhow!("read() requires file object"));
    };

    // Read the file
    let contents = std::fs::read_to_string(&filename)
        .map_err(|e| anyhow::anyhow!("Failed to read file '{}': {}", filename, e))?;

    Ok(Value::Str(contents))
}

fn file_write(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("write() requires file object and content"));
    }

    // Extract filename from file object
    let filename = if let Value::Object { fields, .. } = &args[0] {
        if let Some(Value::Str(name)) = fields.borrow().get("filename") {
            name.clone()
        } else {
            return Err(anyhow::anyhow!("File object missing filename"));
        }
    } else {
        return Err(anyhow::anyhow!("write() requires file object"));
    };

    // Get content to write
    let content = match &args[1] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow::anyhow!("write() content must be a string")),
    };

    // Write to file
    std::fs::write(&filename, &content)
        .map_err(|e| anyhow::anyhow!("Failed to write file '{}': {}", filename, e))?;

    Ok(Value::Int(content.len() as i64))
}

fn file_close(_args: Vec<Value>) -> anyhow::Result<Value> {
    // Mark file as closed (in a real implementation, this would close file handles)
    Ok(Value::None)
}

fn file_enter(args: Vec<Value>) -> anyhow::Result<Value> {
    // __enter__ returns self for context manager protocol
    if args.is_empty() {
        return Err(anyhow::anyhow!("__enter__() requires file object"));
    }
    Ok(args[0].clone())
}

fn file_exit(args: Vec<Value>) -> anyhow::Result<Value> {
    // __exit__ closes the file
    // args: self, exc_type, exc_value, traceback
    if args.is_empty() {
        return Err(anyhow::anyhow!("__exit__() requires file object"));
    }

    // Close the file (currently a no-op)
    file_close(vec![args[0].clone()])?;

    // Return False to propagate exceptions
    Ok(Value::Bool(false))
}

fn file_readline(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("readline() requires file object"));
    }

    // Extract filename
    let filename = if let Value::Object { fields, .. } = &args[0] {
        if let Some(Value::Str(name)) = fields.borrow().get("filename") {
            name.clone()
        } else {
            return Err(anyhow::anyhow!("File object missing filename"));
        }
    } else {
        return Err(anyhow::anyhow!("readline() requires file object"));
    };

    // Read first line (simplified implementation)
    let contents = std::fs::read_to_string(&filename)
        .map_err(|e| anyhow::anyhow!("Failed to read file '{}': {}", filename, e))?;

    let first_line = contents.lines().next().unwrap_or("").to_string();
    Ok(Value::Str(first_line))
}

fn file_readlines(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("readlines() requires file object"));
    }

    // Extract filename
    let filename = if let Value::Object { fields, .. } = &args[0] {
        if let Some(Value::Str(name)) = fields.borrow().get("filename") {
            name.clone()
        } else {
            return Err(anyhow::anyhow!("File object missing filename"));
        }
    } else {
        return Err(anyhow::anyhow!("readlines() requires file object"));
    };

    // Read all lines
    let contents = std::fs::read_to_string(&filename)
        .map_err(|e| anyhow::anyhow!("Failed to read file '{}': {}", filename, e))?;

    let lines: Vec<Value> = contents.lines().map(|line| Value::Str(line.to_string() + "\n")).collect();
    Ok(Value::List(HPList::from_values(lines)))
}

fn file_writelines(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("writelines() requires file object and lines"));
    }

    // Extract filename
    let filename = if let Value::Object { fields, .. } = &args[0] {
        if let Some(Value::Str(name)) = fields.borrow().get("filename") {
            name.clone()
        } else {
            return Err(anyhow::anyhow!("File object missing filename"));
        }
    } else {
        return Err(anyhow::anyhow!("writelines() requires file object"));
    };

    // Get lines to write
    let lines = match &args[1] {
        Value::List(items) => {
            let mut result = String::new();
            for item in items.iter() {
                if let Value::Str(s) = item {
                    result.push_str(&s);
                } else {
                    return Err(anyhow::anyhow!("writelines() requires list of strings"));
                }
            }
            result
        }
        _ => return Err(anyhow::anyhow!("writelines() requires a list")),
    };

    // Write to file
    std::fs::write(&filename, lines)
        .map_err(|e| anyhow::anyhow!("Failed to write file '{}': {}", filename, e))?;

    Ok(Value::None)
}

fn iter_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("iter() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let obj = &args[0];
    
    match obj {
        Value::List(items) => {
            // Create an Iterator object for the list
            Ok(Value::Iterator {
                items: items.as_vec().clone(),
                current_index: 0,
            })
        },
        Value::Tuple(items) => {
            // Create an Iterator object for the tuple
            Ok(Value::Iterator {
                items: items.clone(),
                current_index: 0,
            })
        },
        Value::Str(s) => {
            // Create an Iterator object for the string (character by character)
            let items: Vec<Value> = s.chars().map(|c| Value::Str(c.to_string())).collect();
            Ok(Value::Iterator {
                items,
                current_index: 0,
            })
        },
        Value::Dict(dict) => {
            // Create an Iterator object for the dict (keys)
            let items: Vec<Value> = dict.borrow().keys().map(|k| Value::Str(k.clone())).collect();
            Ok(Value::Iterator {
                items,
                current_index: 0,
            })
        },
        Value::Set(items) => {
            // Create an Iterator object for the set
            Ok(Value::Iterator {
                items: items.clone(),
                current_index: 0,
            })
        },
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", obj.type_name())),
    }
}

fn next_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 1 || args.len() > 2 {
        return Err(anyhow::anyhow!("next() takes at least 1 and at most 2 arguments ({} given)", args.len()));
    }
    
    // Get the iterator
    let iterator = &args[0];
    
    // For generators, we would resume execution and get the next value
    match iterator {
        Value::Generator { code, frame, finished } => {
            // For generators, we need to resume execution
            if *finished {
                // Generator is finished, raise StopIteration
                return Err(anyhow::anyhow!("StopIteration"));
            }
            
            // Create a VM instance to execute the generator
            // For now, we'll just return None as proper generator implementation is complex
            // In a full implementation, we would resume the generator execution
            Ok(Value::None)
        },
        Value::Iterator { ref items, ref current_index } => {
            // For Iterator objects, check if we've reached the end
            if *current_index < items.len() {
                // Return the current item
                Ok(items[*current_index].clone())
            } else {
                // Iterator exhausted, raise StopIteration
                Err(anyhow::anyhow!("StopIteration"))
            }
        },
        Value::ClassMethod { .. } | Value::StaticMethod { .. } => {
            // These are descriptors, not iterators
            Err(anyhow::anyhow!("ClassMethod and StaticMethod objects are not iterators"))
        },
        _ => {
            // For other iterators, return None for now
            Ok(Value::None)
        }
    }
}

fn id_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("id() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // Return a unique ID based on the object's memory address or hash
    // For now, we'll use a simple hash-based approach
    let obj = &args[0];
    use std::collections::hash_map::DefaultHasher;
    use std::hash::{Hash, Hasher};
    
    let mut hasher = DefaultHasher::new();
    obj.hash(&mut hasher);
    let id = hasher.finish() as i64;
    
    Ok(Value::Int(id))
}

fn hash_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("hash() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return a dummy hash
    Ok(Value::Int(0))
}

fn help_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() > 1 {
        return Err(anyhow::anyhow!("help() takes at most 1 argument ({} given)", args.len()));
    }
    
    println!("Tauraro help system - for more information, visit the documentation");
    Ok(Value::None)
}

fn dir_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() > 1 {
        return Err(anyhow::anyhow!("dir() takes at most 1 argument ({} given)", args.len()));
    }
    
    if args.is_empty() {
        // dir() without arguments - return names in current local scope
        // For now, we'll just return an empty list
        Ok(Value::List(HPList::new()))
    } else {
        // dir(obj) - return attributes of the object
        let obj = &args[0];
        let mut attrs = Vec::new();
        
        // Add built-in attributes based on object type
        match obj {
            Value::Object { fields, class_methods, .. } => {
                // Add field names
                for field_name in fields.borrow().keys() {
                    attrs.push(Value::Str(field_name.clone()));
                }
                // Add method names
                for method_name in class_methods.keys() {
                    attrs.push(Value::Str(method_name.clone()));
                }
            },
            Value::Class { methods, .. } => {
                // Add method names
                for method_name in methods.keys() {
                    attrs.push(Value::Str(method_name.clone()));
                }
            },
            Value::Module(_, namespace) => {
                // Add module attributes
                for attr_name in namespace.keys() {
                    attrs.push(Value::Str(attr_name.clone()));
                }
            },
            Value::Dict(dict) => {
                // Add dictionary keys
                for key in dict.borrow().keys() {
                    attrs.push(Value::Str(key.clone()));
                }
            },
            _ => {
                // For other types, we could add dunder methods
                // For now, we'll just return an empty list
            }
        }
        
        // Sort the attributes
        attrs.sort_by(|a, b| {
            if let (Value::Str(a_str), Value::Str(b_str)) = (a, b) {
                a_str.cmp(b_str)
            } else {
                std::cmp::Ordering::Equal
            }
        });
        
        Ok(Value::List(HPList::from_values(attrs)))
    }
}

fn vars_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() > 1 {
        return Err(anyhow::anyhow!("vars() takes at most 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return an empty dict
    Ok(Value::Dict(Rc::new(RefCell::new(HashMap::new()))))
}

fn locals_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if !args.is_empty() {
        return Err(anyhow::anyhow!("locals() takes no arguments ({} given)", args.len()));
    }

    // For now, we'll just return an empty dict
    Ok(Value::Dict(Rc::new(RefCell::new(HashMap::new()))))
}

fn globals_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if !args.is_empty() {
        return Err(anyhow::anyhow!("globals() takes no arguments ({} given)", args.len()));
    }

    // For now, we'll just return an empty dict
    Ok(Value::Dict(Rc::new(RefCell::new(HashMap::new()))))
}

fn eval_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("eval() takes at most 3 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as eval is complex
    Ok(Value::None)
}

fn exec_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("exec() takes at most 3 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as exec is complex
    Ok(Value::None)
}

fn compile_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("compile() takes at most 3 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as compile is complex
    Ok(Value::None)
}

fn repr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("repr() takes exactly 1 argument ({} given)", args.len()));
    }
    
    Ok(Value::Str(format!("{:?}", args[0])))
}

fn ascii_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("ascii() takes exactly 1 argument ({} given)", args.len()));
    }
    
    Ok(Value::Str(format!("{:?}", args[0])))
}

fn format_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("format() takes at most 2 arguments ({} given)", args.len()));
    }
    
    let obj = &args[0];
    Ok(Value::Str(format!("{}", obj)))
}

fn exception_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::new_exception(
            "Exception".to_string(),
            "".to_string(),
            None,
        ))
    } else {
        let message = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        };
        Ok(Value::new_exception(
            "Exception".to_string(),
            message,
            None,
        ))
    }
}

fn value_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::new_exception(
            "ValueError".to_string(),
            "".to_string(),
            None,
        ))
    } else {
        let message = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        };
        Ok(Value::new_exception(
            "ValueError".to_string(),
            message,
            None,
        ))
    }
}

fn type_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::new_exception(
            "TypeError".to_string(),
            "".to_string(),
            None,
        ))
    } else {
        let message = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        };
        Ok(Value::new_exception(
            "TypeError".to_string(),
            message,
            None,
        ))
    }
}

fn runtime_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::new_exception(
            "RuntimeError".to_string(),
            "".to_string(),
            None,
        ))
    } else {
        let message = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        };
        Ok(Value::new_exception(
            "RuntimeError".to_string(),
            message,
            None,
        ))
    }
}

fn zero_division_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("ZeroDivisionError".to_string(), message, None))
}

fn index_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("IndexError".to_string(), message, None))
}

fn key_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("KeyError".to_string(), message, None))
}

fn name_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("NameError".to_string(), message, None))
}

fn attribute_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("AttributeError".to_string(), message, None))
}

fn assertion_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("AssertionError".to_string(), message, None))
}

// ============================================================================
// FFI Builtin Functions
// ============================================================================

// Re-export GLOBAL_FFI_MANAGER from ffi_builtins to maintain single instance
#[cfg(feature = "ffi")]
pub use crate::ffi_builtins::GLOBAL_FFI_MANAGER;

#[cfg(feature = "ffi")]
fn ffi_load_library(args: Vec<Value>) -> anyhow::Result<Value> {
    crate::ffi_builtins::load_library_builtin(args)
}

#[cfg(not(feature = "ffi"))]
fn ffi_load_library(_args: Vec<Value>) -> anyhow::Result<Value> {
    Err(anyhow!("FFI feature is not enabled"))
}

#[cfg(feature = "ffi")]
fn ffi_define_function(args: Vec<Value>) -> anyhow::Result<Value> {
    crate::ffi_builtins::define_function_builtin(args)
}

#[cfg(not(feature = "ffi"))]
fn ffi_define_function(_args: Vec<Value>) -> anyhow::Result<Value> {
    Err(anyhow!("FFI feature is not enabled"))
}

#[cfg(feature = "ffi")]
fn ffi_call_function(args: Vec<Value>) -> anyhow::Result<Value> {
    crate::ffi_builtins::call_function_builtin(args)
}

#[cfg(not(feature = "ffi"))]
fn ffi_call_function(_args: Vec<Value>) -> anyhow::Result<Value> {
    Err(anyhow!("FFI feature is not enabled"))
}

#[cfg(feature = "ffi")]
fn ffi_unload_library(args: Vec<Value>) -> anyhow::Result<Value> {
    crate::ffi_builtins::unload_library_builtin(args)
}

#[cfg(not(feature = "ffi"))]
fn ffi_unload_library(_args: Vec<Value>) -> anyhow::Result<Value> {
    Err(anyhow!("FFI feature is not enabled"))
}

#[cfg(feature = "ffi")]
fn ffi_list_libraries(args: Vec<Value>) -> anyhow::Result<Value> {
    crate::ffi_builtins::list_libraries_builtin(args)
}

#[cfg(not(feature = "ffi"))]
fn ffi_list_libraries(_args: Vec<Value>) -> anyhow::Result<Value> {
    Err(anyhow!("FFI feature is not enabled"))
}

#[cfg(feature = "ffi")]
fn ffi_library_info(args: Vec<Value>) -> anyhow::Result<Value> {
    crate::ffi_builtins::library_info_builtin(args)
}

#[cfg(not(feature = "ffi"))]
fn ffi_library_info(_args: Vec<Value>) -> anyhow::Result<Value> {
    Err(anyhow!("FFI feature is not enabled"))
}

#[cfg(feature = "ffi")]
fn ffi_add_library_path(args: Vec<Value>) -> anyhow::Result<Value> {
    crate::ffi_builtins::add_library_path_builtin(args)
}

#[cfg(not(feature = "ffi"))]
fn ffi_add_library_path(_args: Vec<Value>) -> anyhow::Result<Value> {
    Err(anyhow!("FFI feature is not enabled"))
}

#[cfg(feature = "ffi")]
fn ffi_allocate_buffer(args: Vec<Value>) -> anyhow::Result<Value> {
    crate::ffi_builtins::allocate_buffer_builtin(args)
}

#[cfg(not(feature = "ffi"))]
fn ffi_allocate_buffer(_args: Vec<Value>) -> anyhow::Result<Value> {
    Err(anyhow!("FFI feature is not enabled"))
}

#[cfg(feature = "ffi")]
fn ffi_free_buffer(args: Vec<Value>) -> anyhow::Result<Value> {
    crate::ffi_builtins::free_buffer_builtin(args)
}

#[cfg(not(feature = "ffi"))]
fn ffi_free_buffer(_args: Vec<Value>) -> anyhow::Result<Value> {
    Err(anyhow!("FFI feature is not enabled"))
}

// ============================================================================
// Memory Management Builtin Functions
// ============================================================================

use crate::vm::memory_management::VMMemoryContext;

// Thread-local memory context for the VM
thread_local! {
    static MEMORY_CONTEXT: RefCell<VMMemoryContext> = RefCell::new(VMMemoryContext::default());
}

/// Allocate a buffer of the given size
/// Usage: buffer = allocate(size)
fn memory_allocate_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("allocate() takes exactly 1 argument (size)"));
    }

    let size = match &args[0] {
        Value::Int(n) if *n > 0 => *n as usize,
        Value::Int(_) => return Err(anyhow!("allocate() size must be positive")),
        _ => return Err(anyhow!("allocate() size must be an integer")),
    };

    MEMORY_CONTEXT.with(|ctx| {
        let mut ctx = ctx.borrow_mut();
        let (id, _buffer) = ctx.allocate(size);
        // Return a ByteArray containing the buffer ID
        Ok(Value::ByteArray(vec![id as u8]))
    })
}

/// Free a manually allocated buffer
/// Usage: free(buffer)
fn memory_free_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("free() takes exactly 1 argument (buffer)"));
    }

    let buffer_id = match &args[0] {
        Value::ByteArray(bytes) if !bytes.is_empty() => bytes[0] as usize,
        _ => return Err(anyhow!("free() requires a buffer allocated with allocate()")),
    };

    MEMORY_CONTEXT.with(|ctx| {
        let mut ctx = ctx.borrow_mut();
        ctx.free(buffer_id)?;
        Ok(Value::None)
    })
}

/// Create a new arena
/// Usage: create_arena(name)
fn memory_create_arena_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("create_arena() takes exactly 1 argument (name)"));
    }

    let name = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("create_arena() name must be a string")),
    };

    MEMORY_CONTEXT.with(|ctx| {
        let mut ctx = ctx.borrow_mut();
        ctx.create_arena(name.clone());
        ctx.set_current_arena(name);
        Ok(Value::None)
    })
}

/// Destroy an arena and free all its allocations
/// Usage: destroy_arena(name)
fn memory_destroy_arena_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("destroy_arena() takes exactly 1 argument (name)"));
    }

    let name = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("destroy_arena() name must be a string")),
    };

    MEMORY_CONTEXT.with(|ctx| {
        let mut ctx = ctx.borrow_mut();
        ctx.destroy_arena(&name)?;
        Ok(Value::None)
    })
}

/// Reset an arena (free all allocations but keep the arena)
/// Usage: reset_arena(name)
fn memory_reset_arena_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("reset_arena() takes exactly 1 argument (name)"));
    }

    let name = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("reset_arena() name must be a string")),
    };

    MEMORY_CONTEXT.with(|ctx| {
        let mut ctx = ctx.borrow_mut();
        ctx.reset_arena(&name)?;
        Ok(Value::None)
    })
}

/// Get memory statistics
/// Usage: stats = memory_stats()
fn memory_stats_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if !args.is_empty() {
        return Err(anyhow!("memory_stats() takes no arguments"));
    }

    MEMORY_CONTEXT.with(|ctx| {
        let ctx = ctx.borrow();
        let stats = ctx.get_stats();
        Ok(Value::Str(stats.to_string()))
    })
}

// ============================================================================
// System Programming Builtins
// ============================================================================

/// Get the size of a value in bytes
/// Usage: size = sizeof(value)
fn sizeof_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("sizeof() takes exactly 1 argument"));
    }

    let size = match &args[0] {
        Value::Int(_) => std::mem::size_of::<i64>(),
        Value::Float(_) => std::mem::size_of::<f64>(),
        Value::Bool(_) => std::mem::size_of::<bool>(),
        Value::Str(s) => s.len() + 1, // Including null terminator
        Value::ByteArray(b) => b.len(),
        _ => std::mem::size_of::<Value>(),
    };

    Ok(Value::Int(size as i64))
}

/// Get the alignment of a value type
/// Usage: align = alignof(value)
fn alignof_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("alignof() takes exactly 1 argument"));
    }

    let align = match &args[0] {
        Value::Int(_) => std::mem::align_of::<i64>(),
        Value::Float(_) => std::mem::align_of::<f64>(),
        Value::Bool(_) => std::mem::align_of::<bool>(),
        _ => std::mem::align_of::<usize>(), // Pointer alignment
    };

    Ok(Value::Int(align as i64))
}

/// Copy memory from source to destination
/// Usage: memcpy(dest_ptr, src_ptr, size)
fn memcpy_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 3 {
        return Err(anyhow!("memcpy() takes exactly 3 arguments (dest, src, size)"));
    }

    let dest = match &args[0] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("memcpy() dest must be a pointer (integer)")),
    };
    let src = match &args[1] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("memcpy() src must be a pointer (integer)")),
    };
    let size = match &args[2] {
        Value::Int(n) if *n >= 0 => *n as usize,
        _ => return Err(anyhow!("memcpy() size must be a non-negative integer")),
    };

    if dest != 0 && src != 0 && size > 0 {
        unsafe {
            std::ptr::copy_nonoverlapping(src as *const u8, dest as *mut u8, size);
        }
    }

    Ok(Value::Int(dest as i64))
}

/// Set memory to a value
/// Usage: memset(ptr, value, size)
fn memset_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 3 {
        return Err(anyhow!("memset() takes exactly 3 arguments (ptr, value, size)"));
    }

    let ptr = match &args[0] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("memset() ptr must be a pointer (integer)")),
    };
    let value = match &args[1] {
        Value::Int(n) => *n as u8,
        _ => return Err(anyhow!("memset() value must be an integer")),
    };
    let size = match &args[2] {
        Value::Int(n) if *n >= 0 => *n as usize,
        _ => return Err(anyhow!("memset() size must be a non-negative integer")),
    };

    if ptr != 0 && size > 0 {
        unsafe {
            std::ptr::write_bytes(ptr as *mut u8, value, size);
        }
    }

    Ok(Value::Int(ptr as i64))
}

/// Move memory (handles overlapping regions)
/// Usage: memmove(dest_ptr, src_ptr, size)
fn memmove_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 3 {
        return Err(anyhow!("memmove() takes exactly 3 arguments (dest, src, size)"));
    }

    let dest = match &args[0] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("memmove() dest must be a pointer (integer)")),
    };
    let src = match &args[1] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("memmove() src must be a pointer (integer)")),
    };
    let size = match &args[2] {
        Value::Int(n) if *n >= 0 => *n as usize,
        _ => return Err(anyhow!("memmove() size must be a non-negative integer")),
    };

    if dest != 0 && src != 0 && size > 0 {
        unsafe {
            std::ptr::copy(src as *const u8, dest as *mut u8, size);
        }
    }

    Ok(Value::Int(dest as i64))
}

/// Compare memory regions
/// Usage: result = memcmp(ptr1, ptr2, size)
fn memcmp_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 3 {
        return Err(anyhow!("memcmp() takes exactly 3 arguments (ptr1, ptr2, size)"));
    }

    let ptr1 = match &args[0] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("memcmp() ptr1 must be a pointer (integer)")),
    };
    let ptr2 = match &args[1] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("memcmp() ptr2 must be a pointer (integer)")),
    };
    let size = match &args[2] {
        Value::Int(n) if *n >= 0 => *n as usize,
        _ => return Err(anyhow!("memcmp() size must be a non-negative integer")),
    };

    if ptr1 == 0 || ptr2 == 0 || size == 0 {
        return Ok(Value::Int(0));
    }

    let result = unsafe {
        let s1 = std::slice::from_raw_parts(ptr1 as *const u8, size);
        let s2 = std::slice::from_raw_parts(ptr2 as *const u8, size);
        s1.cmp(s2) as i64
    };

    Ok(Value::Int(result))
}

/// Read a value from a pointer
/// Usage: value = ptr_read(ptr, byte_size=8)
fn ptr_read_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow!("ptr_read() takes 1 or 2 arguments (ptr, byte_size=8)"));
    }

    let ptr = match &args[0] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("ptr_read() ptr must be a pointer (integer)")),
    };
    let byte_size = if args.len() > 1 {
        match &args[1] {
            Value::Int(n) => *n as usize,
            _ => return Err(anyhow!("ptr_read() byte_size must be an integer")),
        }
    } else {
        8
    };

    if ptr == 0 {
        return Ok(Value::Int(0));
    }

    let value = unsafe {
        match byte_size {
            1 => *(ptr as *const i8) as i64,
            2 => *(ptr as *const i16) as i64,
            4 => *(ptr as *const i32) as i64,
            8 => *(ptr as *const i64),
            _ => *(ptr as *const i64),
        }
    };

    Ok(Value::Int(value))
}

/// Write a value to a pointer
/// Usage: ptr_write(ptr, value, byte_size=8)
fn ptr_write_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow!("ptr_write() takes 2 or 3 arguments (ptr, value, byte_size=8)"));
    }

    let ptr = match &args[0] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("ptr_write() ptr must be a pointer (integer)")),
    };
    let value = match &args[1] {
        Value::Int(n) => *n,
        _ => return Err(anyhow!("ptr_write() value must be an integer")),
    };
    let byte_size = if args.len() > 2 {
        match &args[2] {
            Value::Int(n) => *n as usize,
            _ => return Err(anyhow!("ptr_write() byte_size must be an integer")),
        }
    } else {
        8
    };

    if ptr != 0 {
        unsafe {
            match byte_size {
                1 => *(ptr as *mut i8) = value as i8,
                2 => *(ptr as *mut i16) = value as i16,
                4 => *(ptr as *mut i32) = value as i32,
                8 => *(ptr as *mut i64) = value,
                _ => *(ptr as *mut i64) = value,
            }
        }
    }

    Ok(Value::None)
}

/// Offset a pointer by a number of bytes
/// Usage: new_ptr = ptr_offset(ptr, offset)
fn ptr_offset_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("ptr_offset() takes exactly 2 arguments (ptr, offset)"));
    }

    let ptr = match &args[0] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow!("ptr_offset() ptr must be a pointer (integer)")),
    };
    let offset = match &args[1] {
        Value::Int(n) => *n,
        _ => return Err(anyhow!("ptr_offset() offset must be an integer")),
    };

    let new_ptr = if offset >= 0 {
        ptr.wrapping_add(offset as usize)
    } else {
        ptr.wrapping_sub((-offset) as usize)
    };

    Ok(Value::Int(new_ptr as i64))
}

/// Get a null pointer
/// Usage: ptr = null_ptr()
fn null_ptr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if !args.is_empty() {
        return Err(anyhow!("null_ptr() takes no arguments"));
    }
    Ok(Value::Int(0))
}

/// Check if a pointer is null
/// Usage: result = is_null(ptr)
fn is_null_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("is_null() takes exactly 1 argument"));
    }

    let is_null = match &args[0] {
        Value::Int(n) => *n == 0,
        Value::None => true,
        _ => false,
    };

    Ok(Value::Bool(is_null))
}

/// Identity decorator - returns the decorated function unchanged
/// Used for memory management decorators in VM mode
fn decorator_identity(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("Decorator requires exactly 1 argument (the function)"));
    }
    // Just return the function unchanged
    Ok(args[0].clone())
}

// ============================================================================
// Phase 1: High-Priority Exception Constructors (9 new exceptions)
// ============================================================================

fn import_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "cannot import module".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("ImportError".to_string(), message, None))
}

fn module_not_found_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "No module named 'unknown'".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => format!("No module named '{}'", s),
            _ => format!("No module named '{}'", args[0]),
        }
    };
    Ok(Value::new_exception("ModuleNotFoundError".to_string(), message, None))
}

fn file_not_found_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "[Errno 2] No such file or directory".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => format!("[Errno 2] No such file or directory: '{}'", s),
            _ => format!("[Errno 2] No such file or directory: '{}'", args[0]),
        }
    };
    Ok(Value::new_exception("FileNotFoundError".to_string(), message, None))
}

fn io_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "I/O error".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("IOError".to_string(), message, None))
}

fn os_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "Operating system error".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("OSError".to_string(), message, None))
}

fn not_implemented_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("NotImplementedError".to_string(), message, None))
}

fn recursion_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "maximum recursion depth exceeded".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("RecursionError".to_string(), message, None))
}

fn syntax_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "invalid syntax".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("SyntaxError".to_string(), message, None))
}

fn indentation_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() {
        "indentation error".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        }
    };
    Ok(Value::new_exception("IndentationError".to_string(), message, None))
}

// ============================================================================
// Phase 2: Medium-Priority Exception Constructors (8 new)
// ============================================================================

fn stop_iteration_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("StopIteration".to_string(), message, None))
}

fn lookup_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "lookup error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("LookupError".to_string(), message, None))
}

fn eof_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "EOF error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("EOFError".to_string(), message, None))
}

fn permission_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 13] Permission denied".to_string() } else { match &args[0] { Value::Str(s) => format!("[Errno 13] Permission denied: '{}'", s), _ => format!("[Errno 13] Permission denied: '{}'", args[0]) } };
    Ok(Value::new_exception("PermissionError".to_string(), message, None))
}

fn timeout_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "timeout".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("TimeoutError".to_string(), message, None))
}

fn keyboard_interrupt_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("KeyboardInterrupt".to_string(), message, None))
}

fn system_exit_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("SystemExit".to_string(), message, None))
}

fn overflow_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "integer overflow".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("OverflowError".to_string(), message, None))
}

// ============================================================================
// Phase 3: Unicode and Warning Exception Constructors (11 new)
// ============================================================================

fn unicode_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "unicode error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("UnicodeError".to_string(), message, None))
}

fn unicode_decode_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "unicode decode error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("UnicodeDecodeError".to_string(), message, None))
}

fn unicode_encode_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "unicode encode error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("UnicodeEncodeError".to_string(), message, None))
}

fn warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("Warning".to_string(), message, None))
}

fn deprecation_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("DeprecationWarning".to_string(), message, None))
}

fn runtime_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("RuntimeWarning".to_string(), message, None))
}

fn syntax_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("SyntaxWarning".to_string(), message, None))
}

fn user_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("UserWarning".to_string(), message, None))
}

fn future_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("FutureWarning".to_string(), message, None))
}

fn generator_exit_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("GeneratorExit".to_string(), message, None))
}

fn memory_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "out of memory".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("MemoryError".to_string(), message, None))
}

// ============================================================================
// Phase 4: Advanced Exception Constructors (25+ new)
// ============================================================================

fn system_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "internal error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("SystemError".to_string(), message, None))
}

fn reference_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "weak reference object no longer exists".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ReferenceError".to_string(), message, None))
}

fn buffer_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "buffer error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("BufferError".to_string(), message, None))
}

fn arithmetic_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "arithmetic error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ArithmeticError".to_string(), message, None))
}

fn floating_point_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "floating point error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("FloatingPointError".to_string(), message, None))
}

fn connection_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "connection error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ConnectionError".to_string(), message, None))
}

fn broken_pipe_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 32] Broken pipe".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("BrokenPipeError".to_string(), message, None))
}

fn child_process_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "child process error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ChildProcessError".to_string(), message, None))
}

fn connection_aborted_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 103] Software caused connection abort".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ConnectionAbortedError".to_string(), message, None))
}

fn connection_refused_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 111] Connection refused".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ConnectionRefusedError".to_string(), message, None))
}

fn connection_reset_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 104] Connection reset by peer".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ConnectionResetError".to_string(), message, None))
}

fn file_exists_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 17] File exists".to_string() } else { match &args[0] { Value::Str(s) => format!("[Errno 17] File exists: '{}'", s), _ => format!("[Errno 17] File exists: '{}'", args[0]) } };
    Ok(Value::new_exception("FileExistsError".to_string(), message, None))
}

fn interrupted_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 4] Interrupted system call".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("InterruptedError".to_string(), message, None))
}

fn is_a_directory_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 21] Is a directory".to_string() } else { match &args[0] { Value::Str(s) => format!("[Errno 21] Is a directory: '{}'", s), _ => format!("[Errno 21] Is a directory: '{}'", args[0]) } };
    Ok(Value::new_exception("IsADirectoryError".to_string(), message, None))
}

fn not_a_directory_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 20] Not a directory".to_string() } else { match &args[0] { Value::Str(s) => format!("[Errno 20] Not a directory: '{}'", s), _ => format!("[Errno 20] Not a directory: '{}'", args[0]) } };
    Ok(Value::new_exception("NotADirectoryError".to_string(), message, None))
}

fn process_lookup_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 3] No such process".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ProcessLookupError".to_string(), message, None))
}

fn unbound_local_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "local variable referenced before assignment".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("UnboundLocalError".to_string(), message, None))
}

fn unicode_translate_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "unicode translate error".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("UnicodeTranslateError".to_string(), message, None))
}

fn tab_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "inconsistent use of tabs and spaces in indentation".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("TabError".to_string(), message, None))
}

fn import_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ImportWarning".to_string(), message, None))
}

fn unicode_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("UnicodeWarning".to_string(), message, None))
}

fn bytes_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("BytesWarning".to_string(), message, None))
}

fn resource_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("ResourceWarning".to_string(), message, None))
}

fn pending_deprecation_warning_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("PendingDeprecationWarning".to_string(), message, None))
}

fn blocking_io_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let message = if args.is_empty() { "[Errno 11] Resource temporarily unavailable".to_string() } else { match &args[0] { Value::Str(s) => s.clone(), _ => format!("{}", args[0]) } };
    Ok(Value::new_exception("BlockingIOError".to_string(), message, None))
}
