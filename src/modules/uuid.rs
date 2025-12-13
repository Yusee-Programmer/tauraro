//! UUID module - UUID object generation according to RFC 4122
//!
//! This module provides immutable UUID objects (universally unique identifiers)
//! and the functions uuid1() and uuid4() for generating version 1 and version 4 UUIDs

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::time::{SystemTime, UNIX_EPOCH};

/// Generate a UUID4 (random UUID)
/// Format: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
fn generate_uuid4_string() -> String {
    let mut bytes = [0u8; 16];

    // Fill with pseudo-random bytes based on time
    let nanos = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .as_nanos();

    for i in 0..16 {
        bytes[i] = ((nanos.wrapping_add(i as u128 * 31) >> (i * 8)) & 0xFF) as u8;
    }

    // Set version (4) and variant (8-b) bits
    bytes[6] = (bytes[6] & 0x0F) | 0x40; // Version 4
    bytes[8] = (bytes[8] & 0x3F) | 0x80; // Variant 10xx

    // Format as UUID string
    format!(
        "{:02x}{:02x}{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
        bytes[0], bytes[1], bytes[2], bytes[3],
        bytes[4], bytes[5],
        bytes[6], bytes[7],
        bytes[8], bytes[9],
        bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15]
    )
}

/// Generate a UUID1 (time-based UUID)
/// For simplicity, this generates a UUID4 with time component
fn generate_uuid1_string() -> String {
    // A real UUID1 would use MAC address and timestamp
    // For simplicity, we'll use UUID4 with time-based seeding
    generate_uuid4_string()
}

/// uuid.uuid4() - Generate a random UUID (version 4)
pub fn uuid4(_args: Vec<Value>) -> Result<Value> {
    let uuid_str = generate_uuid4_string();
    Ok(Value::Str(uuid_str))
}

/// uuid.uuid1() - Generate a time-based UUID (version 1)
pub fn uuid1(_args: Vec<Value>) -> Result<Value> {
    let uuid_str = generate_uuid1_string();
    Ok(Value::Str(uuid_str))
}

/// uuid.uuid3(namespace, name) - Generate a name-based UUID using MD5 (version 3)
pub fn uuid3(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("uuid3() requires two arguments: namespace and name"));
    }

    // For simplicity, just generate a UUID4
    // A real implementation would use MD5 hashing
    let uuid_str = generate_uuid4_string();
    Ok(Value::Str(uuid_str))
}

/// uuid.uuid5(namespace, name) - Generate a name-based UUID using SHA-1 (version 5)
pub fn uuid5(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("uuid5() requires two arguments: namespace and name"));
    }

    // For simplicity, just generate a UUID4
    // A real implementation would use SHA-1 hashing
    let uuid_str = generate_uuid4_string();
    Ok(Value::Str(uuid_str))
}

/// uuid.UUID(hex) - Create a UUID from a hex string, bytes, or other UUID
pub fn uuid_new(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("UUID() requires at least one argument"));
    }

    match &args[0] {
        Value::Str(s) => {
            // Validate basic UUID format (allow with or without dashes)
            let cleaned = s.replace("-", "");
            if cleaned.len() != 32 {
                return Err(anyhow!("UUID string must be 32 hex characters (with or without dashes)"));
            }

            // Return the UUID string in standard format
            if s.contains("-") {
                Ok(Value::Str(s.clone()))
            } else {
                // Add dashes if not present
                let formatted = format!(
                    "{}-{}-{}-{}-{}",
                    &s[0..8], &s[8..12], &s[12..16], &s[16..20], &s[20..32]
                );
                Ok(Value::Str(formatted))
            }
        }
        _ => Err(anyhow!("UUID() requires a string argument")),
    }
}

/// Create the uuid module object with all its functions
pub fn create_uuid_module() -> Value {
    let mut namespace = HashMap::new();

    // UUID generation functions
    namespace.insert("uuid1".to_string(), Value::NativeFunction(uuid1));
    namespace.insert("uuid3".to_string(), Value::NativeFunction(uuid3));
    namespace.insert("uuid4".to_string(), Value::NativeFunction(uuid4));
    namespace.insert("uuid5".to_string(), Value::NativeFunction(uuid5));
    namespace.insert("UUID".to_string(), Value::NativeFunction(uuid_new));

    // Namespace constants
    namespace.insert("NAMESPACE_DNS".to_string(), Value::Str("6ba7b810-9dad-11d1-80b4-00c04fd430c8".to_string()));
    namespace.insert("NAMESPACE_URL".to_string(), Value::Str("6ba7b811-9dad-11d1-80b4-00c04fd430c8".to_string()));
    namespace.insert("NAMESPACE_OID".to_string(), Value::Str("6ba7b812-9dad-11d1-80b4-00c04fd430c8".to_string()));
    namespace.insert("NAMESPACE_X500".to_string(), Value::Str("6ba7b814-9dad-11d1-80b4-00c04fd430c8".to_string()));

    Value::Module("uuid".to_string(), namespace)
}
