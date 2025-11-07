// Runtime helper functions for JIT compiler
// These functions are called from JIT-compiled code to perform complex operations

#![allow(dead_code)]

use crate::value::Value;
use crate::bytecode::objects::RcValue;
use crate::modules::hplist::HPList;

/// Runtime helper: Load item from list by index
/// Args: registers_ptr, list_reg, index_reg, result_reg
/// Returns: 0 on success, -1 on error
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_subscr_load_list(
    registers_ptr: *mut RcValue,
    list_reg: u32,
    index_reg: u32,
    result_reg: u32,
) -> i32 {
    // Safety: We trust that the VM has set up valid pointers
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let list_val = &registers[list_reg as usize];
    let index_val = &registers[index_reg as usize];

    match (&list_val.value, &index_val.value) {
        (Value::List(items), Value::Int(index)) => {
            let normalized_index = if *index < 0 {
                (items.len() as i64 + *index) as usize
            } else {
                *index as usize
            };

            if normalized_index < items.len() {
                if let Some(item) = items.get(normalized_index as isize) {
                    registers[result_reg as usize] = RcValue::new(item.clone());
                    return 0;
                }
            }
            -1  // Index out of bounds
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: Store item to list by index
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_subscr_store_list(
    registers_ptr: *mut RcValue,
    list_reg: u32,
    index_reg: u32,
    value_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    // Extract values first to avoid borrow checker issues
    let index = match &registers[index_reg as usize].value {
        Value::Int(i) => *i,
        _ => return -1,  // Type error
    };
    let value = registers[value_reg as usize].value.clone();

    let list_val = &mut registers[list_reg as usize];

    match &mut list_val.value {
        Value::List(items) => {
            let normalized_index = if index < 0 {
                (items.len() as i64 + index) as usize
            } else {
                index as usize
            };

            if normalized_index < items.len() {
                let _ = items.set(normalized_index as isize, value);
                return 0;
            }
            -1  // Index out of bounds
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: Append item to list
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_list_append(
    registers_ptr: *mut RcValue,
    list_reg: u32,
    value_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    // Extract value first to avoid borrow checker issues
    let value = registers[value_reg as usize].value.clone();
    let list_val = &mut registers[list_reg as usize];

    match &mut list_val.value {
        Value::List(items) => {
            items.append(value);
            0
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: Get length of collection
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_len(
    registers_ptr: *mut RcValue,
    obj_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let obj = &registers[obj_reg as usize];

    let len = match &obj.value {
        Value::List(items) => items.len() as i64,
        Value::Str(s) => s.len() as i64,
        Value::Tuple(items) => items.len() as i64,
        Value::Dict(dict) => dict.borrow().len() as i64,
        _ => return -1,  // Type error
    };

    registers[result_reg as usize] = RcValue::new(Value::Int(len));
    0
}

/// Runtime helper: Build a list from N values
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_build_list(
    registers_ptr: *mut RcValue,
    start_reg: u32,
    count: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let mut items = HPList::new();
    for i in 0..count {
        let val = registers[(start_reg + i) as usize].value.clone();
        items.append(val);
    }

    registers[result_reg as usize] = RcValue::new(Value::List(items));
    0
}

/// Runtime helper: Get iterator from range
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_get_range_iter(
    registers_ptr: *mut RcValue,
    range_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let range_val = &registers[range_reg as usize];

    match &range_val.value {
        Value::Range { start, stop, step } => {
            registers[result_reg as usize] = RcValue::new(Value::RangeIterator {
                start: *start,
                stop: *stop,
                step: *step,
                current: *start,
            });
            0
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: Advance iterator and get next value
/// Returns: 1 if has next, 0 if exhausted, -1 on error
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_iter_next(
    registers_ptr: *mut RcValue,
    iter_reg: u32,
    value_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    // Extract values to avoid borrow checker issues
    let (has_next, next_value) = {
        let iter_val = &mut registers[iter_reg as usize];
        match &mut iter_val.value {
            Value::RangeIterator { start: _, stop, step, current } => {
                if (*step > 0 && *current < *stop) || (*step < 0 && *current > *stop) {
                    let val = *current;
                    *current += *step;
                    (true, Some(val))
                } else {
                    (false, None)
                }
            }
            _ => (false, None),
        }
    };

    if let Some(val) = next_value {
        registers[value_reg as usize] = RcValue::new(Value::Int(val));
        1  // Has next
    } else if has_next {
        -1  // Type error
    } else {
        0  // Exhausted
    }
}

// ============================================================================
// FUNCTION OPERATIONS
// ============================================================================

/// Runtime helper: Call a function with arguments
/// Args: registers_ptr, func_reg, args_start_reg, args_count, result_reg
/// Returns: 0 on success, -1 on error
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_call_function(
    registers_ptr: *mut RcValue,
    func_reg: u32,
    args_start_reg: u32,
    args_count: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    // Extract function
    let func_val = &registers[func_reg as usize];

    // Collect arguments
    let mut args = Vec::new();
    for i in 0..args_count {
        args.push(registers[(args_start_reg + i) as usize].value.clone());
    }

    // Call based on function type
    match &func_val.value {
        Value::BuiltinFunction(_name, _func) => {
            // Call builtin function
            // This would require access to the builtin function registry
            // For now, return error to indicate interpreter fallback needed
            -1
        }
        Value::Closure { .. } | Value::NativeFunction(_) | Value::Code(_) => {
            // User-defined function or closure
            // Would need to set up new stack frame, etc.
            // For now, return error to indicate interpreter fallback needed
            -1
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: Return from function
/// This is mainly a marker for control flow
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_return_value(
    registers_ptr: *mut RcValue,
    value_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);
    registers[result_reg as usize] = registers[value_reg as usize].clone();
    0
}

// ============================================================================
// CLASS & OBJECT OPERATIONS
// ============================================================================

/// Runtime helper: Load attribute from object
/// Args: registers_ptr, obj_reg, attr_name_reg, result_reg
/// Returns: 0 on success, -1 on error
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_load_attr(
    registers_ptr: *mut RcValue,
    obj_reg: u32,
    attr_name_idx: u32,  // Index into constants table
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    // This would need access to the constants table to get the attribute name
    // For now, return error to indicate interpreter fallback needed
    -1
}

/// Runtime helper: Store attribute to object
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_store_attr(
    registers_ptr: *mut RcValue,
    obj_reg: u32,
    attr_name_idx: u32,
    value_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);
    // Would need access to constants table and object mutation
    -1
}

/// Runtime helper: Call method on object
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_call_method(
    registers_ptr: *mut RcValue,
    obj_reg: u32,
    method_name_idx: u32,
    args_start_reg: u32,
    args_count: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);
    // Would need method lookup and invocation
    -1
}

/// Runtime helper: Create new instance of class
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_make_instance(
    registers_ptr: *mut RcValue,
    class_reg: u32,
    args_start_reg: u32,
    args_count: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);
    // Would need class instantiation logic
    -1
}

// ============================================================================
// STRING OPERATIONS
// ============================================================================

/// Runtime helper: String concatenation
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_string_concat(
    registers_ptr: *mut RcValue,
    left_reg: u32,
    right_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let left_val = &registers[left_reg as usize];
    let right_val = &registers[right_reg as usize];

    match (&left_val.value, &right_val.value) {
        (Value::Str(s1), Value::Str(s2)) => {
            let result = format!("{}{}", s1, s2);
            registers[result_reg as usize] = RcValue::new(Value::Str(result));
            0
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: String indexing
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_string_index(
    registers_ptr: *mut RcValue,
    str_reg: u32,
    index_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let str_val = &registers[str_reg as usize];
    let index_val = &registers[index_reg as usize];

    match (&str_val.value, &index_val.value) {
        (Value::Str(s), Value::Int(index)) => {
            let normalized_index = if *index < 0 {
                (s.len() as i64 + *index) as usize
            } else {
                *index as usize
            };

            if let Some(ch) = s.chars().nth(normalized_index) {
                registers[result_reg as usize] = RcValue::new(Value::Str(ch.to_string()));
                0
            } else {
                -1  // Index out of bounds
            }
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: String slicing
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_string_slice(
    registers_ptr: *mut RcValue,
    str_reg: u32,
    start_reg: u32,
    stop_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let str_val = &registers[str_reg as usize];
    let start_val = &registers[start_reg as usize];
    let stop_val = &registers[stop_reg as usize];

    match (&str_val.value, &start_val.value, &stop_val.value) {
        (Value::Str(s), Value::Int(start), Value::Int(stop)) => {
            let len = s.len() as i64;
            let start_idx = if *start < 0 { 0 } else { *start as usize };
            let stop_idx = if *stop > len { len as usize } else { *stop as usize };

            if start_idx <= stop_idx && stop_idx <= s.len() {
                let result: String = s.chars().skip(start_idx).take(stop_idx - start_idx).collect();
                registers[result_reg as usize] = RcValue::new(Value::Str(result));
                0
            } else {
                -1  // Invalid slice
            }
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: String length (optimized version of len() for strings)
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_string_len(
    registers_ptr: *mut RcValue,
    str_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    match &registers[str_reg as usize].value {
        Value::Str(s) => {
            registers[result_reg as usize] = RcValue::new(Value::Int(s.len() as i64));
            0
        }
        _ => -1,  // Type error
    }
}

// ============================================================================
// TUPLE OPERATIONS
// ============================================================================

/// Runtime helper: Build tuple from values
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_build_tuple(
    registers_ptr: *mut RcValue,
    start_reg: u32,
    count: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let mut items = Vec::new();
    for i in 0..count {
        let val = registers[(start_reg + i) as usize].value.clone();
        items.push(val);
    }

    registers[result_reg as usize] = RcValue::new(Value::Tuple(items));
    0
}

/// Runtime helper: Tuple indexing
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_tuple_index(
    registers_ptr: *mut RcValue,
    tuple_reg: u32,
    index_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let tuple_val = &registers[tuple_reg as usize];
    let index_val = &registers[index_reg as usize];

    match (&tuple_val.value, &index_val.value) {
        (Value::Tuple(items), Value::Int(index)) => {
            let normalized_index = if *index < 0 {
                (items.len() as i64 + *index) as usize
            } else {
                *index as usize
            };

            if normalized_index < items.len() {
                registers[result_reg as usize] = RcValue::new(items[normalized_index].clone());
                0
            } else {
                -1  // Index out of bounds
            }
        }
        _ => -1,  // Type error
    }
}

// ============================================================================
// DICTIONARY OPERATIONS
// ============================================================================

/// Runtime helper: Dictionary get with index notation
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_dict_get(
    registers_ptr: *mut RcValue,
    dict_reg: u32,
    key_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let dict_val = &registers[dict_reg as usize];
    let key_val = &registers[key_reg as usize];

    match &dict_val.value {
        Value::Dict(dict_ref) => {
            // Convert key to string
            let key_str = match &key_val.value {
                Value::Str(s) => s.clone(),
                Value::Int(n) => n.to_string(),
                _ => format!("{:?}", key_val.value),
            };

            // Extract value before borrow ends to avoid borrow checker issues
            let dict = dict_ref.borrow();
            let value_opt = dict.get(&key_str).cloned();
            drop(dict);  // Explicitly drop borrow

            if let Some(value) = value_opt {
                registers[result_reg as usize] = RcValue::new(value);
                0
            } else {
                -1  // Key not found
            }
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: Dictionary set
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_dict_set(
    registers_ptr: *mut RcValue,
    dict_reg: u32,
    key_reg: u32,
    value_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    // Extract values to avoid borrow issues
    let key_str = match &registers[key_reg as usize].value {
        Value::Str(s) => s.clone(),
        Value::Int(n) => n.to_string(),
        _ => format!("{:?}", registers[key_reg as usize].value),
    };
    let value = registers[value_reg as usize].value.clone();

    let dict_val = &mut registers[dict_reg as usize];

    match &mut dict_val.value {
        Value::Dict(dict_ref) => {
            dict_ref.borrow_mut().insert(key_str, value);
            0
        }
        _ => -1,  // Type error
    }
}

/// Runtime helper: Build dictionary from key-value pairs
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_build_dict(
    registers_ptr: *mut RcValue,
    pairs_start_reg: u32,
    pair_count: u32,
    result_reg: u32,
) -> i32 {
    use std::cell::RefCell;
    use std::rc::Rc;
    use std::collections::HashMap;

    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let mut dict = HashMap::new();
    for i in 0..pair_count {
        let key_reg = (pairs_start_reg + i * 2) as usize;
        let val_reg = (pairs_start_reg + i * 2 + 1) as usize;

        let key_str = match &registers[key_reg].value {
            Value::Str(s) => s.clone(),
            Value::Int(n) => n.to_string(),
            _ => format!("{:?}", registers[key_reg].value),
        };

        dict.insert(key_str, registers[val_reg].value.clone());
    }

    registers[result_reg as usize] = RcValue::new(Value::Dict(Rc::new(RefCell::new(dict))));
    0
}

// ============================================================================
// SET OPERATIONS
// ============================================================================

/// Runtime helper: Build set from values
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_build_set(
    registers_ptr: *mut RcValue,
    start_reg: u32,
    count: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let mut items = Vec::new();
    for i in 0..count {
        let val = registers[(start_reg + i) as usize].value.clone();
        // Note: Real set would need deduplication
        items.push(val);
    }

    registers[result_reg as usize] = RcValue::new(Value::Set(items));
    0
}

/// Runtime helper: Set add operation
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_set_add(
    registers_ptr: *mut RcValue,
    set_reg: u32,
    value_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let value = registers[value_reg as usize].value.clone();
    let set_val = &mut registers[set_reg as usize];

    match &mut set_val.value {
        Value::Set(items) => {
            // Check if value already exists (simple linear search for now)
            let value_str = format!("{:?}", value);
            let exists = items.iter().any(|v| format!("{:?}", v) == value_str);
            if !exists {
                items.push(value);
            }
            0
        }
        _ => -1,  // Type error
    }
}

// ============================================================================
// TYPE CHECKING AND CONVERSION
// ============================================================================

/// Runtime helper: Check if value is of specific type
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_isinstance(
    registers_ptr: *mut RcValue,
    value_reg: u32,
    type_tag: u32,  // 0=int, 1=float, 2=str, 3=list, 4=dict, etc.
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let val = &registers[value_reg as usize];
    let is_instance = match (type_tag, &val.value) {
        (0, Value::Int(_)) => true,
        (1, Value::Float(_)) => true,
        (2, Value::Str(_)) => true,
        (3, Value::List(_)) => true,
        (4, Value::Dict(_)) => true,
        (5, Value::Tuple(_)) => true,
        (6, Value::Set(_)) => true,
        (7, Value::Bool(_)) => true,
        _ => false,
    };

    registers[result_reg as usize] = RcValue::new(Value::Bool(is_instance));
    0
}

/// Runtime helper: Convert to string
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_to_string(
    registers_ptr: *mut RcValue,
    value_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let val = &registers[value_reg as usize];
    let string = match &val.value {
        Value::Int(i) => i.to_string(),
        Value::Float(f) => f.to_string(),
        Value::Bool(b) => b.to_string(),
        Value::Str(s) => s.clone(),
        _ => format!("{:?}", val.value),
    };

    registers[result_reg as usize] = RcValue::new(Value::Str(string));
    0
}

/// Runtime helper: Convert to boolean
#[no_mangle]
pub unsafe extern "C" fn tauraro_jit_to_bool(
    registers_ptr: *mut RcValue,
    value_reg: u32,
    result_reg: u32,
) -> i32 {
    let registers = std::slice::from_raw_parts_mut(registers_ptr, 256);

    let val = &registers[value_reg as usize];
    let bool_val = match &val.value {
        Value::Bool(b) => *b,
        Value::Int(i) => *i != 0,
        Value::Float(f) => *f != 0.0,
        Value::Str(s) => !s.is_empty(),
        Value::List(l) => l.len() > 0,
        Value::Dict(d) => d.borrow().len() > 0,
        Value::Tuple(t) => !t.is_empty(),
        Value::None => false,
        _ => true,
    };

    registers[result_reg as usize] = RcValue::new(Value::Bool(bool_val));
    0
}
