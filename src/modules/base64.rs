/// Base64 module - provides base64 encoding and decoding functionality
/// Similar to Python's base64 module

use crate::value::Value;
use std::collections::HashMap;

type Result<T> = anyhow::Result<T>;

/// Base64 alphabet
const STANDARD_ALPHABET: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const URL_SAFE_ALPHABET: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

/// Create the base64 module
pub fn create_base64_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Standard base64 functions
    namespace.insert("b64encode".to_string(), Value::NativeFunction(b64encode));
    namespace.insert("b64decode".to_string(), Value::NativeFunction(b64decode));
    namespace.insert("standard_b64encode".to_string(), Value::NativeFunction(b64encode));
    namespace.insert("standard_b64decode".to_string(), Value::NativeFunction(b64decode));
    
    // URL-safe base64 functions
    namespace.insert("urlsafe_b64encode".to_string(), Value::NativeFunction(urlsafe_b64encode));
    namespace.insert("urlsafe_b64decode".to_string(), Value::NativeFunction(urlsafe_b64decode));
    
    // Base32 functions
    namespace.insert("b32encode".to_string(), Value::NativeFunction(b32encode));
    namespace.insert("b32decode".to_string(), Value::NativeFunction(b32decode));
    
    // Base16 functions
    namespace.insert("b16encode".to_string(), Value::NativeFunction(b16encode));
    namespace.insert("b16decode".to_string(), Value::NativeFunction(b16decode));
    
    // Base85 functions
    namespace.insert("b85encode".to_string(), Value::NativeFunction(b85encode));
    namespace.insert("b85decode".to_string(), Value::NativeFunction(b85decode));
    
    // Legacy functions
    namespace.insert("encode".to_string(), Value::NativeFunction(encode));
    namespace.insert("decode".to_string(), Value::NativeFunction(decode));
    namespace.insert("encodebytes".to_string(), Value::NativeFunction(encodebytes));
    namespace.insert("decodebytes".to_string(), Value::NativeFunction(decodebytes));
    
    Value::Module("base64".to_string(), namespace)
}

/// Get a base64 module function by name
pub fn get_base64_function(name: &str) -> Option<fn(Vec<Value>) -> Result<Value>> {
    match name {
        "b64encode" => Some(b64encode),
        "b64decode" => Some(b64decode),
        "urlsafe_b64encode" => Some(urlsafe_b64encode),
        "urlsafe_b64decode" => Some(urlsafe_b64decode),
        "b32encode" => Some(b32encode),
        "b32decode" => Some(b32decode),
        "b16encode" => Some(b16encode),
        "b16decode" => Some(b16decode),
        "b85encode" => Some(b85encode),
        "b85decode" => Some(b85decode),
        "encode" => Some(encode),
        "decode" => Some(decode),
        "encodebytes" => Some(encodebytes),
        "decodebytes" => Some(decodebytes),
        _ => None,
    }
}

/// base64.b64encode(s, altchars=None) - Encode bytes using base64
fn b64encode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("b64encode() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s.as_bytes(),
        _ => return Err(anyhow::anyhow!("b64encode() argument must be bytes-like")),
    };
    
    let encoded = encode_base64(data, STANDARD_ALPHABET);
    Ok(Value::Str(encoded))
}

/// base64.b64decode(s, altchars=None, validate=False) - Decode base64 encoded bytes
fn b64decode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("b64decode() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("b64decode() argument must be string or bytes")),
    };
    
    let decoded = decode_base64(data, STANDARD_ALPHABET)?;
    Ok(Value::Str(String::from_utf8_lossy(&decoded).to_string()))
}

/// base64.urlsafe_b64encode(s) - Encode bytes using URL-safe base64
fn urlsafe_b64encode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("urlsafe_b64encode() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s.as_bytes(),
        _ => return Err(anyhow::anyhow!("urlsafe_b64encode() argument must be bytes-like")),
    };
    
    let encoded = encode_base64(data, URL_SAFE_ALPHABET);
    Ok(Value::Str(encoded))
}

/// base64.urlsafe_b64decode(s) - Decode URL-safe base64 encoded bytes
fn urlsafe_b64decode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("urlsafe_b64decode() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("urlsafe_b64decode() argument must be string or bytes")),
    };
    
    let decoded = decode_base64(data, URL_SAFE_ALPHABET)?;
    Ok(Value::Str(String::from_utf8_lossy(&decoded).to_string()))
}

/// base64.b32encode(s) - Encode bytes using base32
fn b32encode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("b32encode() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s.as_bytes(),
        _ => return Err(anyhow::anyhow!("b32encode() argument must be bytes-like")),
    };
    
    let encoded = encode_base32(data);
    Ok(Value::Str(encoded))
}

/// base64.b32decode(s, casefold=False, map01=None) - Decode base32 encoded bytes
fn b32decode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("b32decode() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("b32decode() argument must be string or bytes")),
    };
    
    let decoded = decode_base32(data)?;
    Ok(Value::Str(String::from_utf8_lossy(&decoded).to_string()))
}

/// base64.b16encode(s) - Encode bytes using base16 (hex)
fn b16encode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("b16encode() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s.as_bytes(),
        _ => return Err(anyhow::anyhow!("b16encode() argument must be bytes-like")),
    };
    
    let encoded = hex::encode_upper(data);
    Ok(Value::Str(encoded))
}

/// base64.b16decode(s, casefold=False) - Decode base16 (hex) encoded bytes
fn b16decode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("b16decode() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("b16decode() argument must be string or bytes")),
    };
    
    match hex::decode(data) {
        Ok(decoded) => Ok(Value::Str(String::from_utf8_lossy(&decoded).to_string())),
        Err(_) => Err(anyhow::anyhow!("Invalid base16 data")),
    }
}

/// base64.b85encode(b, pad=False) - Encode bytes using base85
fn b85encode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("b85encode() missing required argument: 'b'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s.as_bytes(),
        _ => return Err(anyhow::anyhow!("b85encode() argument must be bytes-like")),
    };
    
    // Simplified base85 encoding (placeholder implementation)
    let encoded = format!("b85:{}", hex::encode(data));
    Ok(Value::Str(encoded))
}

/// base64.b85decode(b) - Decode base85 encoded bytes
fn b85decode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("b85decode() missing required argument: 'b'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("b85decode() argument must be string or bytes")),
    };
    
    // Simplified base85 decoding (placeholder implementation)
    if data.starts_with("b85:") {
        let hex_data = &data[4..];
        match hex::decode(hex_data) {
            Ok(decoded) => Ok(Value::Str(String::from_utf8_lossy(&decoded).to_string())),
            Err(_) => Err(anyhow::anyhow!("Invalid base85 data")),
        }
    } else {
        Err(anyhow::anyhow!("Invalid base85 data format"))
    }
}

/// Legacy encode function
fn encode(args: Vec<Value>) -> Result<Value> {
    encodebytes(args)
}

/// Legacy decode function
fn decode(args: Vec<Value>) -> Result<Value> {
    decodebytes(args)
}

/// base64.encodebytes(s) - Encode bytes with base64 and add newlines
fn encodebytes(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("encodebytes() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s.as_bytes(),
        _ => return Err(anyhow::anyhow!("encodebytes() argument must be bytes-like")),
    };
    
    let encoded = encode_base64(data, STANDARD_ALPHABET);
    // Add newlines every 76 characters
    let mut result = String::new();
    for (i, c) in encoded.chars().enumerate() {
        if i > 0 && i % 76 == 0 {
            result.push('\n');
        }
        result.push(c);
    }
    result.push('\n');
    
    Ok(Value::Str(result))
}

/// base64.decodebytes(s) - Decode base64 encoded bytes
fn decodebytes(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("decodebytes() missing required argument: 's'"));
    }
    
    let data = match &args[0] {
        Value::Str(s) => s.replace('\n', "").replace('\r', ""),
        _ => return Err(anyhow::anyhow!("decodebytes() argument must be string or bytes")),
    };
    
    let decoded = decode_base64(&data, STANDARD_ALPHABET)?;
    Ok(Value::Str(String::from_utf8_lossy(&decoded).to_string()))
}

/// Helper function to encode data using base64
fn encode_base64(data: &[u8], alphabet: &[u8]) -> String {
    let mut result = String::new();
    let mut i = 0;
    
    while i < data.len() {
        let b1 = data[i];
        let b2 = if i + 1 < data.len() { data[i + 1] } else { 0 };
        let b3 = if i + 2 < data.len() { data[i + 2] } else { 0 };
        
        let n = ((b1 as u32) << 16) | ((b2 as u32) << 8) | (b3 as u32);
        
        result.push(alphabet[((n >> 18) & 63) as usize] as char);
        result.push(alphabet[((n >> 12) & 63) as usize] as char);
        result.push(if i + 1 < data.len() { alphabet[((n >> 6) & 63) as usize] as char } else { '=' });
        result.push(if i + 2 < data.len() { alphabet[(n & 63) as usize] as char } else { '=' });
        
        i += 3;
    }
    
    result
}

/// Helper function to decode base64 data
fn decode_base64(data: &str, alphabet: &[u8]) -> Result<Vec<u8>> {
    let mut result = Vec::new();
    let chars: Vec<char> = data.chars().filter(|&c| c != '=' && !c.is_whitespace()).collect();
    
    let mut i = 0;
    while i + 3 < chars.len() {
        let c1 = find_char_index(chars[i], alphabet)?;
        let c2 = find_char_index(chars[i + 1], alphabet)?;
        let c3 = find_char_index(chars[i + 2], alphabet)?;
        let c4 = find_char_index(chars[i + 3], alphabet)?;
        
        let n = (c1 << 18) | (c2 << 12) | (c3 << 6) | c4;
        
        result.push((n >> 16) as u8);
        result.push((n >> 8) as u8);
        result.push(n as u8);
        
        i += 4;
    }
    
    Ok(result)
}

/// Helper function to find character index in alphabet
fn find_char_index(c: char, alphabet: &[u8]) -> Result<u32> {
    for (i, &ch) in alphabet.iter().enumerate() {
        if ch as char == c {
            return Ok(i as u32);
        }
    }
    Err(anyhow::anyhow!("Invalid character in base64 data"))
}

/// Helper function to encode data using base32
fn encode_base32(data: &[u8]) -> String {
    const BASE32_ALPHABET: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    
    let mut result = String::new();
    let mut i = 0;
    
    while i < data.len() {
        let mut n = 0u64;
        let mut count = 0;
        
        for j in 0..5 {
            if i + j < data.len() {
                n = (n << 8) | (data[i + j] as u64);
                count += 1;
            } else {
                n <<= 8;
            }
        }
        
        for j in 0..8 {
            if j < (count * 8 + 4) / 5 {
                let index = ((n >> (35 - j * 5)) & 31) as usize;
                result.push(BASE32_ALPHABET[index] as char);
            } else {
                result.push('=');
            }
        }
        
        i += 5;
    }
    
    result
}

/// Helper function to decode base32 data
fn decode_base32(data: &str) -> Result<Vec<u8>> {
    const BASE32_ALPHABET: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    
    let mut result = Vec::new();
    let chars: Vec<char> = data.chars().filter(|&c| c != '=' && !c.is_whitespace()).collect();
    
    let mut i = 0;
    while i + 7 < chars.len() {
        let mut n = 0u64;
        
        for j in 0..8 {
            if i + j < chars.len() {
                let c = chars[i + j];
                let index = find_char_index(c, BASE32_ALPHABET)?;
                n = (n << 5) | (index as u64);
            }
        }
        
        for j in 0..5 {
            result.push((n >> (32 - j * 8)) as u8);
        }
        
        i += 8;
    }
    
    Ok(result)
}

/// Simple hex encoding module
mod hex {
    pub fn encode_upper(data: &[u8]) -> String {
        data.iter().map(|b| format!("{:02X}", b)).collect()
    }
    
    pub fn encode(data: &[u8]) -> String {
        data.iter().map(|b| format!("{:02x}", b)).collect()
    }
    
    pub fn decode(s: &str) -> Result<Vec<u8>, &'static str> {
        if s.len() % 2 != 0 {
            return Err("Invalid hex string length");
        }
        
        let mut result = Vec::new();
        for chunk in s.as_bytes().chunks(2) {
            let hex_str = std::str::from_utf8(chunk).map_err(|_| "Invalid UTF-8")?;
            let byte = u8::from_str_radix(hex_str, 16).map_err(|_| "Invalid hex character")?;
            result.push(byte);
        }
        
        Ok(result)
    }
}
