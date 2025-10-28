//! FFI wrapper for websockets module - exports C-compatible functions
//! Uses #![no_std] for minimal dependencies and easy C linking

#![no_std]

use core::ffi::c_int;
use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

// Type definitions (must match C)
#[repr(C)]
pub enum TauraroType {
    Int = 0, Float = 1, Bool = 2, String = 3, List = 4,
    Dict = 5, Tuple = 6, Set = 7, None = 8, Object = 9,
    Function = 10, Bytes = 11, Complex = 12, Range = 13, Frozenset = 14,
}

#[repr(C)]
pub union TauraroData {
    pub int_val: i64,
    pub float_val: f64,
    pub bool_val: bool,
    pub str_val: *mut u8,
}

#[repr(C)]
pub struct TauraroValue {
    pub value_type: TauraroType,
    pub ref_count: c_int,
    pub data: TauraroData,
}

extern "C" {
    fn tauraro_value_new() -> *mut TauraroValue;
    fn tauraro_value_new_with_type(t: TauraroType) -> *mut TauraroValue;
}

// Helper function to create an integer value
unsafe fn create_int_value(val: i64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Int;
        (*result).data.int_val = val;
    }
    result
}

// Helper function to create a boolean value
unsafe fn create_bool_value(val: bool) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Bool;
        (*result).data.bool_val = val;
    }
    result
}

// Helper function to create a string value
unsafe fn create_string_value(s: &str) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        // Note: In a real implementation, we would need to allocate and copy the string
        // For now, we'll just set it to null
        (*result).data.str_val = core::ptr::null_mut();
    }
    result
}

// Helper function to create a tuple value
unsafe fn create_tuple_value(items: &[*mut TauraroValue]) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Tuple;
        // Note: In a real implementation, we would need to store the items
        // For now, we'll just create an empty tuple
    }
    result
}

// Helper function to create a dict value
unsafe fn create_dict_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Dict;
        // Note: In a real implementation, we would need to store key-value pairs
        // For now, we'll just create an empty dict
    }
    result
}

// Helper function to create an object value
unsafe fn create_object_value(class_name: &str) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Object;
        // Note: In a real implementation, we would need to store object data
        // For now, we'll just create a basic object
    }
    result
}

// Helper function to create bytes value
unsafe fn create_bytes_value(data: &[u8]) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Bytes;
        // Note: In a real implementation, we would need to store the bytes
        // For now, we'll just create a basic bytes object
    }
    result
}

// websockets.connect(uri, *, timeout=None, ssl=None) - Connect to WebSocket server
#[no_mangle]
pub extern "C" fn tauraro_websockets_connect(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 1 {
            return create_object_value("TypeError");
        }
        
        // Get URI argument
        let uri = *argv;
        
        // Connect to WebSocket server
        let websocket_obj = create_object_value("WebSocket");
        websocket_obj
    }
}

// websockets.serve(ws_handler, host=None, port=None, *, ssl=None) - Start WebSocket server
#[no_mangle]
pub extern "C" fn tauraro_websockets_serve(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 1 {
            return create_object_value("TypeError");
        }
        
        // Get handler argument
        let ws_handler = *argv;
        
        // Start WebSocket server
        let server_obj = create_object_value("WebSocketServer");
        server_obj
    }
}

// WebSocket.send(message) - Send message
#[no_mangle]
pub extern "C" fn tauraro_websockets_send(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 2 {
            return create_object_value("TypeError");
        }
        
        // Get WebSocket and message arguments
        let websocket = *argv;
        let message = *argv.add(1);
        
        // Send message (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// WebSocket.recv() - Receive message
#[no_mangle]
pub extern "C" fn tauraro_websockets_recv(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 1 {
            return create_object_value("TypeError");
        }
        
        // Get WebSocket argument
        let websocket = *argv;
        
        // Receive message (simplified implementation)
        create_string_value("message")
    }
}

// WebSocket.close(code=1000, reason='') - Close connection
#[no_mangle]
pub extern "C" fn tauraro_websockets_close(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 1 || argc > 3 {
            return create_object_value("TypeError");
        }
        
        // Get WebSocket, code, and reason arguments
        let websocket = *argv;
        let code = if argc >= 2 { *argv.add(1) } else { core::ptr::null_mut() };
        let reason = if argc >= 3 { *argv.add(2) } else { core::ptr::null_mut() };
        
        // Close connection (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// WebSocket.ping(data=None) - Send ping
#[no_mangle]
pub extern "C" fn tauraro_websockets_ping(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 1 || argc > 2 {
            return create_object_value("TypeError");
        }
        
        // Get WebSocket and data arguments
        let websocket = *argv;
        let data = if argc == 2 { *argv.add(1) } else { core::ptr::null_mut() };
        
        // Send ping (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// WebSocket.pong(data=None) - Send pong
#[no_mangle]
pub extern "C" fn tauraro_websockets_pong(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 1 || argc > 2 {
            return create_object_value("TypeError");
        }
        
        // Get WebSocket and data arguments
        let websocket = *argv;
        let data = if argc == 2 { *argv.add(1) } else { core::ptr::null_mut() };
        
        // Send pong (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// WebSocket.open() - Check if connection is open
#[no_mangle]
pub extern "C" fn tauraro_websockets_open(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 1 {
            return create_object_value("TypeError");
        }
        
        // Get WebSocket argument
        let websocket = *argv;
        
        // Check if connection is open (simplified implementation)
        create_bool_value(true)
    }
}

// WebSocket.closed() - Check if connection is closed
#[no_mangle]
pub extern "C" fn tauraro_websockets_closed(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 1 {
            return create_object_value("TypeError");
        }
        
        // Get WebSocket argument
        let websocket = *argv;
        
        // Check if connection is closed (simplified implementation)
        create_bool_value(false)
    }
}

// websockets.ConnectionClosed(code, reason) - Create connection closed exception
#[no_mangle]
pub extern "C" fn tauraro_websockets_connection_closed_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 2 {
            return create_object_value("TypeError");
        }
        
        // Get code and reason arguments
        let code = *argv;
        let reason = *argv.add(1);
        
        // Create connection closed exception
        let error_obj = create_object_value("ConnectionClosed");
        error_obj
    }
}

// websockets.InvalidHandshake(message) - Create invalid handshake exception
#[no_mangle]
pub extern "C" fn tauraro_websockets_invalid_handshake_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 1 {
            return create_object_value("TypeError");
        }
        
        // Get message argument
        let message = *argv;
        
        // Create invalid handshake exception
        let error_obj = create_object_value("InvalidHandshake");
        error_obj
    }
}