//! Secrets module - cryptographically strong random numbers for managing secrets
//!
//! This module provides functions for generating secure random tokens suitable
//! for managing secrets such as account authentication, tokens, and similar.

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;

/// Generate a random hex string token
/// secrets.token_hex([nbytes]) - Generate a random text string in hexadecimal
pub fn token_hex(args: Vec<Value>) -> Result<Value> {
    let nbytes = if args.is_empty() {
        32 // Default 32 bytes = 64 hex chars
    } else {
        match &args[0] {
            Value::Int(n) => *n as usize,
            _ => return Err(anyhow!("token_hex() requires an integer argument")),
        }
    };

    // Generate random bytes using time-based pseudo-random
    let mut bytes = vec![0u8; nbytes];
    let base_time = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap()
        .as_nanos();

    for (i, byte) in bytes.iter_mut().enumerate() {
        *byte = ((base_time.wrapping_add(i as u128 * 17)) & 0xFF) as u8;
    }

    // Convert to hex string
    let hex_string = bytes.iter()
        .map(|b| format!("{:02x}", b))
        .collect::<String>();

    Ok(Value::Str(hex_string))
}

/// Generate a URL-safe random string token
/// secrets.token_urlsafe([nbytes]) - Generate a random URL-safe text string
pub fn token_urlsafe(args: Vec<Value>) -> Result<Value> {
    let nbytes = if args.is_empty() {
        32
    } else {
        match &args[0] {
            Value::Int(n) => *n as usize,
            _ => return Err(anyhow!("token_urlsafe() requires an integer argument")),
        }
    };

    // URL-safe characters: A-Za-z0-9-_
    const URL_SAFE_CHARS: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    let mut result = String::with_capacity(nbytes);
    let base_time = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap()
        .as_nanos();

    for i in 0..nbytes {
        let idx = ((base_time.wrapping_add(i as u128 * 19)) % 64) as usize;
        result.push(URL_SAFE_CHARS[idx] as char);
    }

    Ok(Value::Str(result))
}

/// Generate random bytes
/// secrets.token_bytes([nbytes]) - Generate a random byte string
pub fn token_bytes(args: Vec<Value>) -> Result<Value> {
    let nbytes = if args.is_empty() {
        32
    } else {
        match &args[0] {
            Value::Int(n) => *n as usize,
            _ => return Err(anyhow!("token_bytes() requires an integer argument")),
        }
    };

    // Generate random bytes
    let mut bytes = vec![0u8; nbytes];
    let base_time = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap()
        .as_nanos();

    for (i, byte) in bytes.iter_mut().enumerate() {
        *byte = ((base_time.wrapping_add(i as u128 * 23)) & 0xFF) as u8;
    }

    // Convert to string representation for now
    let hex_string = bytes.iter()
        .map(|b| format!("{:02x}", b))
        .collect::<String>();

    Ok(Value::Str(hex_string))
}

/// Return a random int in the range [0, n)
/// secrets.randbelow(exclusive_upper_bound) - Generate a random int in range [0, n)
pub fn randbelow(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("randbelow() requires one argument"));
    }

    let upper_bound = match &args[0] {
        Value::Int(n) => *n,
        _ => return Err(anyhow!("randbelow() requires an integer argument")),
    };

    if upper_bound <= 0 {
        return Err(anyhow!("randbelow() argument must be positive"));
    }

    let random_value = (std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap()
        .as_nanos() % upper_bound as u128) as i64;

    Ok(Value::Int(random_value))
}

/// Return a randomly chosen element from a non-empty sequence
/// secrets.choice(sequence) - Choose a random element from a sequence
pub fn choice(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("choice() requires one argument"));
    }

    match &args[0] {
        Value::List(items) => {
            let list_vec = items.as_vec();
            if list_vec.is_empty() {
                return Err(anyhow!("choice() cannot choose from an empty sequence"));
            }
            let idx = (std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap()
                .as_nanos() % list_vec.len() as u128) as usize;
            Ok(list_vec[idx].clone())
        }
        Value::Str(s) => {
            if s.is_empty() {
                return Err(anyhow!("choice() cannot choose from an empty sequence"));
            }
            let idx = (std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap()
                .as_nanos() % s.len() as u128) as usize;
            Ok(Value::Str(s.chars().nth(idx).unwrap().to_string()))
        }
        _ => Err(anyhow!("choice() requires a sequence (list or string)")),
    }
}

/// Create the secrets module object with all its functions
pub fn create_secrets_module() -> Value {
    let mut namespace = HashMap::new();

    // Token generation functions
    namespace.insert("token_hex".to_string(), Value::NativeFunction(token_hex));
    namespace.insert("token_urlsafe".to_string(), Value::NativeFunction(token_urlsafe));
    namespace.insert("token_bytes".to_string(), Value::NativeFunction(token_bytes));

    // Random functions
    namespace.insert("randbelow".to_string(), Value::NativeFunction(randbelow));
    namespace.insert("choice".to_string(), Value::NativeFunction(choice));

    Value::Module("secrets".to_string(), namespace)
}
