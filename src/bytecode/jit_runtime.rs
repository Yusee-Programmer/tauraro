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
