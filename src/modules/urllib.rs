use crate::value::Value;
use std::collections::HashMap;
use url::Url;
use anyhow::{Result, anyhow};

pub fn create_urllib_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Create urllib.parse submodule
    let mut parse_namespace = HashMap::new();
    
    // Add urlparse function
    parse_namespace.insert(
        "urlparse".to_string(),
        Value::NativeFunction(urlparse),
    );
    
    // Add urlencode function
    parse_namespace.insert(
        "urlencode".to_string(),
        Value::NativeFunction(urlencode),
    );
    
    // Add quote function
    parse_namespace.insert(
        "quote".to_string(),
        Value::NativeFunction(quote),
    );
    
    // Add unquote function
    parse_namespace.insert(
        "unquote".to_string(),
        Value::NativeFunction(unquote),
    );
    
    // Create the parse submodule
    namespace.insert(
        "parse".to_string(),
        Value::Module("parse".to_string(), parse_namespace),
    );
    
    Value::Module("urllib".to_string(), namespace)
}

fn urlparse(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("urlparse() missing required argument: 'url'"));
    }
    
    let url_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("urlparse() argument must be a string")),
    };
    
    match Url::parse(url_str) {
        Ok(url) => {
            let mut result = HashMap::new();
            
            result.insert("scheme".to_string(), Value::Str(url.scheme().to_string()));
            result.insert("netloc".to_string(), Value::Str(url.host_str().unwrap_or("").to_string()));
            result.insert("path".to_string(), Value::Str(url.path().to_string()));
            result.insert("params".to_string(), Value::Str("".to_string())); // URL crate doesn't separate params
            result.insert("query".to_string(), Value::Str(url.query().unwrap_or("").to_string()));
            result.insert("fragment".to_string(), Value::Str(url.fragment().unwrap_or("").to_string()));
            
            Ok(Value::Dict(result))
        }
        Err(_) => {
            // If URL parsing fails, return a basic structure with empty values
            let mut result = HashMap::new();
            result.insert("scheme".to_string(), Value::Str("".to_string()));
            result.insert("netloc".to_string(), Value::Str("".to_string()));
            result.insert("path".to_string(), Value::Str(url_str.to_string()));
            result.insert("params".to_string(), Value::Str("".to_string()));
            result.insert("query".to_string(), Value::Str("".to_string()));
            result.insert("fragment".to_string(), Value::Str("".to_string()));
            
            Ok(Value::Dict(result))
        }
    }
}

fn urlencode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("urlencode() missing required argument: 'query'"));
    }
    
    let query_dict = match &args[0] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("urlencode() argument must be a dictionary")),
    };
    
    let mut encoded_pairs = Vec::new();
    
    for (key, value) in query_dict {
        let key_str = match key {
            k => k.clone(),
        };
        
        let value_str = match value {
            Value::Str(s) => s.clone(),
            Value::Int(i) => i.to_string(),
            Value::Float(f) => f.to_string(),
            _ => return Err(anyhow!("urlencode() values must be strings or numbers")),
        };
        
        // URL encode the key and value
        let encoded_key = percent_encode(&key_str);
        let encoded_value = percent_encode(&value_str);
        
        encoded_pairs.push(format!("{}={}", encoded_key, encoded_value));
    }
    
    Ok(Value::Str(encoded_pairs.join("&")))
}

fn quote(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("quote() missing required argument: 'string'"));
    }
    
    let input_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("quote() argument must be a string")),
    };
    
    // URL encode the string
    let encoded = percent_encode(input_str);
    Ok(Value::Str(encoded))
}

fn unquote(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("unquote() missing required argument: 'string'"));
    }
    
    let input_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("unquote() argument must be a string")),
    };
    
    // URL decode the string
    let decoded = percent_decode(input_str);
    Ok(Value::Str(decoded))
}

fn percent_encode(input: &str) -> String {
    let mut result = String::new();
    
    for byte in input.bytes() {
        match byte {
            // Unreserved characters (don't encode)
            b'A'..=b'Z' | b'a'..=b'z' | b'0'..=b'9' | b'-' | b'.' | b'_' | b'~' => {
                result.push(byte as char);
            }
            // Space becomes %20 (not + for quote function)
            b' ' => {
                result.push_str("%20");
            }
            // Everything else gets percent-encoded
            _ => {
                result.push_str(&format!("%{:02X}", byte));
            }
        }
    }
    
    result
}

fn percent_decode(input: &str) -> String {
    let mut result = String::new();
    let mut chars = input.chars().peekable();
    
    while let Some(ch) = chars.next() {
        if ch == '%' {
            // Try to decode the next two characters as hex
            let hex1 = chars.next();
            let hex2 = chars.next();
            
            if let (Some(h1), Some(h2)) = (hex1, hex2) {
                let hex_str = format!("{}{}", h1, h2);
                if let Ok(byte_val) = u8::from_str_radix(&hex_str, 16) {
                    result.push(byte_val as char);
                    continue;
                }
            }
            
            // If decoding fails, just add the % character
            result.push(ch);
            if let Some(h1) = hex1 {
                result.push(h1);
            }
            if let Some(h2) = hex2 {
                result.push(h2);
            }
        } else {
            result.push(ch);
        }
    }
    
    result
}
