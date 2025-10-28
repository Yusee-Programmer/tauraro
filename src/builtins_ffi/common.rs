//! Common types and helpers for FFI modules
//! This is meant to be included in each FFI module

#![no_std]

use core::ffi::{c_int, c_void};

#[repr(C)]
#[derive(PartialEq, Eq, Clone, Copy)]
pub enum TauraroType {
    Int = 0,
    Float = 1,
    Bool = 2,
    String = 3,
    List = 4,
    Dict = 5,
    Tuple = 6,
    Set = 7,
    None = 8,
    Object = 9,
    Function = 10,
    Bytes = 11,
    Complex = 12,
    Range = 13,
    Frozenset = 14,
}

#[repr(C)]
pub union TauraroData {
    pub int_val: i64,
    pub float_val: f64,
    pub bool_val: bool,
    pub str_val: *mut u8,
    pub ptr_val: *mut c_void,
}

#[repr(C)]
pub struct TauraroValue {
    pub value_type: TauraroType,
    pub ref_count: c_int,
    pub data: TauraroData,
}

// External functions provided by the main program
extern "C" {
    pub fn tauraro_value_new() -> *mut TauraroValue;
    pub fn malloc(size: usize) -> *mut u8;
    pub fn free(ptr: *mut u8);
}

// Helper functions
pub unsafe fn create_int_value(value: i64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Int;
        (*result).data.int_val = value;
    }
    result
}

pub unsafe fn create_float_value(value: f64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Float;
        (*result).data.float_val = value;
    }
    result
}

pub unsafe fn create_bool_value(value: bool) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Bool;
        (*result).data.bool_val = value;
    }
    result
}

pub unsafe fn create_none_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::None;
    }
    result
}

pub unsafe fn create_string_value(s: &str) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        let len = s.len();
        (*result).data.str_val = malloc(len + 1);
        if !(*result).data.str_val.is_null() {
            let src = s.as_ptr();
            for i in 0..len {
                *(*result).data.str_val.add(i) = *src.add(i);
            }
            *(*result).data.str_val.add(len) = 0;
        }
    }
    result
}

pub unsafe fn get_int_value(val: *mut TauraroValue) -> i64 {
    if val.is_null() {
        return 0;
    }
    match (*val).value_type {
        TauraroType::Int => (*val).data.int_val,
        TauraroType::Float => (*val).data.float_val as i64,
        TauraroType::Bool => if (*val).data.bool_val { 1 } else { 0 },
        _ => 0,
    }
}

pub unsafe fn get_float_value(val: *mut TauraroValue) -> f64 {
    if val.is_null() {
        return 0.0;
    }
    match (*val).value_type {
        TauraroType::Int => (*val).data.int_val as f64,
        TauraroType::Float => (*val).data.float_val,
        TauraroType::Bool => if (*val).data.bool_val { 1.0 } else { 0.0 },
        _ => 0.0,
    }
}

pub unsafe fn get_bool_value(val: *mut TauraroValue) -> bool {
    if val.is_null() {
        return false;
    }
    match (*val).value_type {
        TauraroType::Bool => (*val).data.bool_val,
        TauraroType::Int => (*val).data.int_val != 0,
        TauraroType::Float => (*val).data.float_val != 0.0,
        TauraroType::None => false,
        _ => true,
    }
}

pub unsafe fn is_valid_index(val: *mut TauraroValue) -> bool {
    if val.is_null() {
        return false;
    }
    matches!((*val).value_type, TauraroType::Int)
}

pub unsafe fn is_valid_number(val: *mut TauraroValue) -> bool {
    if val.is_null() {
        return false;
    }
    matches!((*val).value_type, TauraroType::Int | TauraroType::Float)
}
