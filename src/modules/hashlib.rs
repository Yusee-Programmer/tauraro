/// Hashlib module - provides cryptographic hashing functionality
/// Similar to Python's hashlib module

use crate::value::Value;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use sha1::Sha1;
use sha2::{Sha224, Sha256, Sha384, Sha512, Digest};
use sha3::{Sha3_224, Sha3_256, Sha3_384, Sha3_512};
use md5::Md5;

type Result<T> = anyhow::Result<T>;

/// Create the hashlib module
pub fn create_hashlib_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Hash constructor functions
    namespace.insert("md5".to_string(), Value::BuiltinFunction("md5".to_string(), hashlib_md5));
    namespace.insert("sha1".to_string(), Value::BuiltinFunction("sha1".to_string(), hashlib_sha1));
    namespace.insert("sha224".to_string(), Value::BuiltinFunction("sha224".to_string(), hashlib_sha224));
    namespace.insert("sha256".to_string(), Value::BuiltinFunction("sha256".to_string(), hashlib_sha256));
    namespace.insert("sha384".to_string(), Value::BuiltinFunction("sha384".to_string(), hashlib_sha384));
    namespace.insert("sha512".to_string(), Value::BuiltinFunction("sha512".to_string(), hashlib_sha512));
    namespace.insert("sha3_224".to_string(), Value::BuiltinFunction("sha3_224".to_string(), hashlib_sha3_224));
    namespace.insert("sha3_256".to_string(), Value::BuiltinFunction("sha3_256".to_string(), hashlib_sha3_256));
    namespace.insert("sha3_384".to_string(), Value::BuiltinFunction("sha3_384".to_string(), hashlib_sha3_384));
    namespace.insert("sha3_512".to_string(), Value::BuiltinFunction("sha3_512".to_string(), hashlib_sha3_512));
    
    // General functions
    namespace.insert("new".to_string(), Value::BuiltinFunction("new".to_string(), hashlib_new));
    
    // PBKDF2 function
    namespace.insert("pbkdf2_hmac".to_string(), Value::BuiltinFunction("pbkdf2_hmac".to_string(), hashlib_pbkdf2_hmac));
    
    // Available algorithms
    let mut algorithms = Vec::new();
    algorithms.push(Value::Str("md5".to_string()));
        algorithms.push(Value::Str("sha1".to_string()));
        algorithms.push(Value::Str("sha224".to_string()));
        algorithms.push(Value::Str("sha256".to_string()));
        algorithms.push(Value::Str("sha384".to_string()));
        algorithms.push(Value::Str("sha512".to_string()));
        algorithms.push(Value::Str("sha3_224".to_string()));
        algorithms.push(Value::Str("sha3_256".to_string()));
        algorithms.push(Value::Str("sha3_384".to_string()));
        algorithms.push(Value::Str("sha3_512".to_string()));
    
    namespace.insert("algorithms_guaranteed".to_string(), Value::Tuple(algorithms.clone()));
    namespace.insert("algorithms_available".to_string(), Value::Tuple(algorithms));
    
    Value::Module("hashlib".to_string(), namespace)
}

/// Get a hashlib module function by name
pub fn get_hashlib_function(name: &str) -> Option<fn(Vec<Value>) -> Result<Value>> {
    match name {
        "md5" => Some(hashlib_md5),
        "sha1" => Some(hashlib_sha1),
        "sha224" => Some(hashlib_sha224),
        "sha256" => Some(hashlib_sha256),
        "sha384" => Some(hashlib_sha384),
        "sha512" => Some(hashlib_sha512),
        "sha3_224" => Some(hashlib_sha3_224),
        "sha3_256" => Some(hashlib_sha3_256),
        "sha3_384" => Some(hashlib_sha3_384),
        "sha3_512" => Some(hashlib_sha3_512),
        "new" => Some(hashlib_new),
        "pbkdf2_hmac" => Some(hashlib_pbkdf2_hmac),
        _ => None,
    }
}

/// Create a hash object
fn create_hash_object(algorithm: &str, data: Option<&[u8]>) -> Result<Value> {
    let mut hash_obj = HashMap::new();
    
    hash_obj.insert("algorithm".to_string(), Value::Str(algorithm.to_string()));
    hash_obj.insert("digest_size".to_string(), Value::Int(get_digest_size(algorithm)));
    hash_obj.insert("block_size".to_string(), Value::Int(get_block_size(algorithm)));
    hash_obj.insert("name".to_string(), Value::Str(algorithm.to_string()));
    
    // Initialize with data if provided
    let initial_data = if let Some(d) = data {
        let hash_result = compute_hash(algorithm, d);
        hex_encode(&hash_result)
    } else {
        String::new()
    };
    hash_obj.insert("_hash_state".to_string(), Value::Str(initial_data));
    
    // Store the raw data for incremental updates
    let mut raw_data = Vec::new();
    if let Some(d) = data {
        raw_data.extend_from_slice(d);
    }
    hash_obj.insert("_raw_data".to_string(), Value::Str(String::from_utf8_lossy(&raw_data).to_string()));
    
    // Add methods
    hash_obj.insert("update".to_string(), Value::NativeFunction(hash_update));
    hash_obj.insert("digest".to_string(), Value::NativeFunction(hash_digest));
    hash_obj.insert("hexdigest".to_string(), Value::NativeFunction(hash_hexdigest));
    hash_obj.insert("copy".to_string(), Value::NativeFunction(hash_copy));
    
    Ok(Value::Object {
        class_name: format!("{}Hash", algorithm),
        fields: Rc::new(RefCell::new(hash_obj)),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new(format!("{}Hash", algorithm), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec![format!("{}Hash", algorithm), "object".to_string()]),
    })
}

/// Compute hash for given algorithm and data
fn compute_hash(algorithm: &str, data: &[u8]) -> Vec<u8> {
    match algorithm {
        "md5" => {
            let mut hasher = Md5::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        "sha1" => {
            let mut hasher = Sha1::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        "sha224" => {
            let mut hasher = Sha224::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        "sha256" => {
            let mut hasher = Sha256::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        "sha384" => {
            let mut hasher = Sha384::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        "sha512" => {
            let mut hasher = Sha512::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        "sha3_224" => {
            let mut hasher = Sha3_224::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        "sha3_256" => {
            let mut hasher = Sha3_256::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        "sha3_384" => {
            let mut hasher = Sha3_384::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        "sha3_512" => {
            let mut hasher = Sha3_512::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
        _ => {
            // Default to SHA256 for unknown algorithms
            let mut hasher = Sha256::new();
            hasher.update(data);
            hasher.finalize().to_vec()
        }
    }
}

/// Get digest size for algorithm
fn get_digest_size(algorithm: &str) -> i64 {
    match algorithm {
        "md5" => 16,
        "sha1" => 20,
        "sha224" => 28,
        "sha256" => 32,
        "sha384" => 48,
        "sha512" => 64,
        "sha3_224" => 28,
        "sha3_256" => 32,
        "sha3_384" => 48,
        "sha3_512" => 64,
        _ => 32, // default
    }
}

/// Get block size for algorithm
fn get_block_size(algorithm: &str) -> i64 {
    match algorithm {
        "md5" => 64,
        "sha1" => 64,
        "sha224" => 64,
        "sha256" => 64,
        "sha384" => 128,
        "sha512" => 128,
        "sha3_224" => 144,
        "sha3_256" => 136,
        "sha3_384" => 104,
        "sha3_512" => 72,
        _ => 64, // default
    }
}

/// hashlib.md5([data]) - Create MD5 hash object
fn hashlib_md5(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("md5() argument must be bytes-like")),
        }
    };
    
    create_hash_object("md5", data)
}

/// hashlib.sha1([data]) - Create SHA1 hash object
fn hashlib_sha1(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("sha1() argument must be bytes-like")),
        }
    };
    
    create_hash_object("sha1", data)
}

/// hashlib.sha224([data]) - Create SHA224 hash object
fn hashlib_sha224(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("sha224() argument must be bytes-like")),
        }
    };
    
    create_hash_object("sha224", data)
}

/// hashlib.sha256([data]) - Create SHA256 hash object
fn hashlib_sha256(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("sha256() argument must be bytes-like")),
        }
    };
    
    create_hash_object("sha256", data)
}

/// hashlib.sha384([data]) - Create SHA384 hash object
fn hashlib_sha384(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("sha384() argument must be bytes-like")),
        }
    };
    
    create_hash_object("sha384", data)
}

/// hashlib.sha512([data]) - Create SHA512 hash object
fn hashlib_sha512(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("sha512() argument must be bytes-like")),
        }
    };
    
    create_hash_object("sha512", data)
}

/// hashlib.sha3_224([data]) - Create SHA3-224 hash object
fn hashlib_sha3_224(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("sha3_224() argument must be bytes-like")),
        }
    };
    
    create_hash_object("sha3_224", data)
}

/// hashlib.sha3_256([data]) - Create SHA3-256 hash object
fn hashlib_sha3_256(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("sha3_256() argument must be bytes-like")),
        }
    };
    
    create_hash_object("sha3_256", data)
}

/// hashlib.sha3_384([data]) - Create SHA3-384 hash object
fn hashlib_sha3_384(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("sha3_384() argument must be bytes-like")),
        }
    };
    
    create_hash_object("sha3_384", data)
}

/// hashlib.sha3_512([data]) - Create SHA3-512 hash object
fn hashlib_sha3_512(args: Vec<Value>) -> Result<Value> {
    let data = if args.is_empty() {
        None
    } else {
        match &args[0] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("sha3_512() argument must be bytes-like")),
        }
    };
    
    create_hash_object("sha3_512", data)
}

/// hashlib.new(name[, data], *, usedforsecurity=True) - Create hash object by name
fn hashlib_new(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("new() missing required argument: 'name'"));
    }
    
    let algorithm = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("new() algorithm name must be string")),
    };
    
    let data = if args.len() > 1 {
        match &args[1] {
            Value::Str(s) => Some(s.as_bytes()),
            _ => return Err(anyhow::anyhow!("new() data argument must be bytes-like")),
        }
    } else {
        None
    };
    
    create_hash_object(algorithm, data)
}

/// hashlib.pbkdf2_hmac(hash_name, password, salt, iterations, dklen=None)
fn hashlib_pbkdf2_hmac(args: Vec<Value>) -> Result<Value> {
    if args.len() < 4 {
        return Err(anyhow::anyhow!("pbkdf2_hmac() missing required arguments"));
    }
    
    let hash_name = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("pbkdf2_hmac() hash_name must be string")),
    };
    
    let password = match &args[1] {
        Value::Str(s) => s.as_bytes(),
        _ => return Err(anyhow::anyhow!("pbkdf2_hmac() password must be bytes-like")),
    };
    
    let salt = match &args[2] {
        Value::Str(s) => s.as_bytes(),
        _ => return Err(anyhow::anyhow!("pbkdf2_hmac() salt must be bytes-like")),
    };
    
    let iterations = match &args[3] {
        Value::Int(i) => *i as usize,
        _ => return Err(anyhow::anyhow!("pbkdf2_hmac() iterations must be integer")),
    };
    
    let dklen = if args.len() > 4 {
        match &args[4] {
            Value::Int(i) => Some(*i as usize),
            Value::None => None,
            _ => return Err(anyhow::anyhow!("pbkdf2_hmac() dklen must be integer or None")),
        }
    } else {
        None
    };
    
    // Simplified PBKDF2 implementation (placeholder)
    let key_length = dklen.unwrap_or(get_digest_size(hash_name) as usize);
    let derived_key = pbkdf2_simple(password, salt, iterations, key_length);
    
    Ok(Value::Str(hex_encode(&derived_key)))
}

/// Simplified PBKDF2 implementation
fn pbkdf2_simple(password: &[u8], salt: &[u8], iterations: usize, key_length: usize) -> Vec<u8> {
    let mut result = Vec::with_capacity(key_length);
    let mut current = Vec::new();
    current.extend_from_slice(password);
    current.extend_from_slice(salt);
    
    for _ in 0..iterations {
        current = simple_hash(&current);
    }
    
    // Extend or truncate to desired length
    while result.len() < key_length {
        result.extend_from_slice(&current);
        if result.len() >= key_length {
            result.truncate(key_length);
            break;
        }
        current = simple_hash(&current);
    }
    
    result
}

/// Simple hash function (placeholder implementation)
fn simple_hash(data: &[u8]) -> Vec<u8> {
    let mut hash = Vec::new();
    let mut sum: u64 = 0;
    
    for &byte in data {
        sum = sum.wrapping_add(byte as u64);
        sum = sum.wrapping_mul(31);
    }
    
    // Convert to bytes
    for i in 0..8 {
        hash.push((sum >> (i * 8)) as u8);
    }
    
    hash
}

/// Convert bytes to hex string
fn hex_encode(data: &[u8]) -> String {
    data.iter().map(|b| format!("{:02x}", b)).collect()
}

/// Hash object method implementations
pub fn get_hash_method(method_name: &str) -> Option<fn(Vec<Value>) -> Result<Value>> {
    match method_name {
        "update" => Some(hash_update),
        "digest" => Some(hash_digest),
        "hexdigest" => Some(hash_hexdigest),
        "copy" => Some(hash_copy),
        _ => None,
    }
}

/// Hash.update(data) - Update hash with new data
fn hash_update(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("update() missing required argument: 'data'"));
    }
    
    let hash_obj = &args[0];
    let new_data = match &args[1] {
        Value::Str(s) => s.as_bytes(),
        _ => return Err(anyhow::anyhow!("update() data argument must be bytes-like")),
    };
    
    // Get current raw data from hash object
    let mut current_data = Vec::new();
    if let Value::Object { fields, .. } = hash_obj {
        if let Some(Value::Str(existing_data)) = fields.borrow().get("_raw_data") {
            current_data.extend_from_slice(existing_data.as_bytes());
        }
    }
    
    // Append new data
    current_data.extend_from_slice(new_data);
    
    // Update the hash object with new data
    if let Value::Object { fields, .. } = hash_obj {
        let algorithm = match fields.borrow().get("algorithm") {
            Some(Value::Str(algo)) => algo,
            _ => return Err(anyhow::anyhow!("Hash object missing algorithm")),
        };
        
        // Compute new hash
        let hash_result = compute_hash(algorithm, &current_data);
        let hex_hash = hex_encode(&hash_result);
        
        // Update the object fields
        let mut updated_fields = (**fields).clone();
        updated_fields.borrow_mut().insert("_hash_state".to_string(), Value::Str(hex_hash));
        updated_fields.borrow_mut().insert("_raw_data".to_string(), Value::Str(String::from_utf8_lossy(&current_data).to_string()));
        
        // Create new hash object with updated fields
        return Ok(Value::Object {
            class_name: algorithm.to_string() + "Hash",
            fields: std::rc::Rc::new(updated_fields),
            class_methods: HashMap::new(),
            base_object: crate::base_object::BaseObject::new(algorithm.to_string() + "Hash", vec!["object".to_string()]),
            mro: crate::base_object::MRO::from_linearization(vec![algorithm.to_string() + "Hash", "object".to_string()]),
        });
    }
    
    Err(anyhow::anyhow!("Invalid hash object"))
}

/// Hash.digest() - Return digest as bytes
fn hash_digest(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("digest() missing self argument"));
    }
    
    let hash_obj = &args[0];
    
    if let Value::Object { fields, .. } = hash_obj {
        if let Some(Value::Str(hash_state)) = fields.borrow().get("_hash_state") {
            // Convert hex string back to bytes
            let bytes = hex_decode(hash_state)?;
            return Ok(Value::Str(String::from_utf8_lossy(&bytes).to_string()));
        }
    }
    
    Err(anyhow::anyhow!("Hash object missing hash state"))
}

/// Hash.hexdigest() - Return digest as hex string
fn hash_hexdigest(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("hexdigest() missing self argument"));
    }
    
    let hash_obj = &args[0];
    
    if let Value::Object { fields, .. } = hash_obj {
        if let Some(Value::Str(hash_state)) = fields.borrow().get("_hash_state") {
            return Ok(Value::Str(hash_state.clone()));
        }
    }
    
    Err(anyhow::anyhow!("Hash object missing hash state"))
}

/// Hash.copy() - Return copy of hash object
fn hash_copy(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("copy() missing self argument"));
    }
    
    // Return a copy of the hash object
    Ok(args[0].clone())
}

/// Convert hex string to bytes
fn hex_decode(hex_str: &str) -> Result<Vec<u8>> {
    let mut bytes = Vec::new();
    for i in (0..hex_str.len()).step_by(2) {
        if i + 1 < hex_str.len() {
            let byte_str = &hex_str[i..i+2];
            let byte = u8::from_str_radix(byte_str, 16)
                .map_err(|_| anyhow::anyhow!("Invalid hex string"))?;
            bytes.push(byte);
        }
    }
    Ok(bytes)
}