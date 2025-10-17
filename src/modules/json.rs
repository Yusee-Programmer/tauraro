/// JSON module - provides JSON encoding and decoding functionality
/// Similar to Python's json module

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
// Import HPList
use crate::modules::hplist::HPList;

/// Create the json module object with all its functions and constants
pub fn create_json_module() -> Value {
    let mut namespace = HashMap::new();
    
    // JSON functions
    namespace.insert("loads".to_string(), Value::NativeFunction(json_loads));
    namespace.insert("dumps".to_string(), Value::NativeFunction(json_dumps));
    namespace.insert("load".to_string(), Value::NativeFunction(json_load));
    namespace.insert("dump".to_string(), Value::NativeFunction(json_dump));
    
    // JSON encoder/decoder classes (simplified)
    namespace.insert("JSONEncoder".to_string(), Value::NativeFunction(create_json_encoder));
    namespace.insert("JSONDecoder".to_string(), Value::NativeFunction(create_json_decoder));
    
    // JSON exceptions (as strings for now)
    namespace.insert("JSONDecodeError".to_string(), Value::Str("JSONDecodeError".to_string()));
    
    Value::Module("json".to_string(), namespace)
}

/// Parse JSON string to Python object
fn json_loads(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 4 {
        return Err(anyhow::anyhow!("loads() takes 1 to 4 arguments"));
    }
    
    let json_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("JSON string must be a string")),
    };
    
    // Simple JSON parsing (placeholder implementation)
    parse_json_value(json_str.trim())
}

/// Serialize Python object to JSON string
fn json_dumps(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 10 {
        return Err(anyhow::anyhow!("dumps() takes 1 to 10 arguments"));
    }
    
    let obj = &args[0];
    
    // Extract optional parameters
    let indent = if args.len() > 1 {
        match &args[1] {
            Value::Int(n) => Some(*n as usize),
            Value::None => None,
            _ => None,
        }
    } else {
        None
    };
    
    serialize_to_json(obj, indent, 0)
}

/// Load JSON from file
fn json_load(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 4 {
        return Err(anyhow::anyhow!("load() takes 1 to 4 arguments"));
    }
    
    // Placeholder implementation - would need file I/O integration
    Err(anyhow::anyhow!("json.load() not yet implemented - requires file I/O integration"))
}

/// Dump JSON to file
fn json_dump(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 10 {
        return Err(anyhow::anyhow!("dump() takes 2 to 10 arguments"));
    }
    
    // Placeholder implementation - would need file I/O integration
    Err(anyhow::anyhow!("json.dump() not yet implemented - requires file I/O integration"))
}

/// Create JSON encoder object
fn create_json_encoder(_args: Vec<Value>) -> Result<Value> {
    let mut encoder = HashMap::new();
    encoder.insert("encode".to_string(), Value::NativeFunction(encoder_encode));
    encoder.insert("iterencode".to_string(), Value::NativeFunction(encoder_iterencode));
    
    Ok(Value::Object {
        class_name: "JSONEncoder".to_string(),
        fields: Rc::new(encoder),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("JSONEncoder".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["JSONEncoder".to_string(), "object".to_string()]),
    })
}

/// Create JSON decoder object
fn create_json_decoder(_args: Vec<Value>) -> Result<Value> {
    let mut decoder = HashMap::new();
    decoder.insert("decode".to_string(), Value::NativeFunction(decoder_decode));
    decoder.insert("raw_decode".to_string(), Value::NativeFunction(decoder_raw_decode));
    
    Ok(Value::Object {
        class_name: "JSONDecoder".to_string(),
        fields: Rc::new(decoder),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("JSONDecoder".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["JSONDecoder".to_string(), "object".to_string()]),
    })
}

// Encoder methods
fn encoder_encode(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("encode() takes exactly 1 argument"));
    }
    
    serialize_to_json(&args[0], None, 0)
}

fn encoder_iterencode(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::List(HPList::new()))
}

// Decoder methods
fn decoder_decode(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("decode() takes exactly 1 argument"));
    }
    
    let json_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("JSON string must be a string")),
    };
    
    parse_json_value(json_str.trim())
}

fn decoder_raw_decode(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::Tuple(vec![Value::None, Value::Int(0)]))
}

/// Parse a JSON value from string (simplified implementation)
fn parse_json_value(s: &str) -> Result<Value> {
    let s = s.trim();
    
    if s.is_empty() {
        return Err(anyhow::anyhow!("Expecting value"));
    }
    
    // Handle different JSON types
    if s == "null" {
        return Ok(Value::None);
    }
    
    if s == "true" {
        return Ok(Value::Bool(true));
    }
    
    if s == "false" {
        return Ok(Value::Bool(false));
    }
    
    // String
    if s.starts_with('"') && s.ends_with('"') && s.len() >= 2 {
        let content = &s[1..s.len()-1];
        // Simple unescape (placeholder)
        let unescaped = content
            .replace("\\\"", "\"")
            .replace("\\\\", "\\")
            .replace("\\n", "\n")
            .replace("\\r", "\r")
            .replace("\\t", "\t");
        return Ok(Value::Str(unescaped));
    }
    
    // Number
    if let Ok(int_val) = s.parse::<i64>() {
        return Ok(Value::Int(int_val));
    }
    
    if let Ok(float_val) = s.parse::<f64>() {
        return Ok(Value::Float(float_val));
    }
    
    // Array
    if s.starts_with('[') && s.ends_with(']') {
        return parse_json_array(&s[1..s.len()-1]);
    }
    
    // Object
    if s.starts_with('{') && s.ends_with('}') {
        return parse_json_object(&s[1..s.len()-1]);
    }
    
    Err(anyhow::anyhow!("Invalid JSON value: {}", s))
}

/// Parse JSON array (simplified implementation)
fn parse_json_array(s: &str) -> Result<Value> {
    let s = s.trim();
    if s.is_empty() {
        return Ok(Value::List(HPList::new()));
    }
    
    // Simple comma splitting (doesn't handle nested structures properly)
    let mut items = Vec::new();
    let mut current = String::new();
    let mut depth = 0;
    let mut in_string = false;
    let mut escape_next = false;
    
    for ch in s.chars() {
        if escape_next {
            current.push(ch);
            escape_next = false;
            continue;
        }
        
        match ch {
            '\\' if in_string => {
                current.push(ch);
                escape_next = true;
            }
            '"' => {
                current.push(ch);
                in_string = !in_string;
            }
            '[' | '{' if !in_string => {
                current.push(ch);
                depth += 1;
            }
            ']' | '}' if !in_string => {
                current.push(ch);
                depth -= 1;
            }
            ',' if !in_string && depth == 0 => {
                if !current.trim().is_empty() {
                    items.push(parse_json_value(current.trim())?);
                }
                current.clear();
            }
            _ => {
                current.push(ch);
            }
        }
    }
    
    if !current.trim().is_empty() {
        items.push(parse_json_value(current.trim())?);
    }
    
    Ok(Value::List(HPList::from_values(items)))
}

/// Parse JSON object (simplified implementation)
fn parse_json_object(s: &str) -> Result<Value> {
    let s = s.trim();
    if s.is_empty() {
        return Ok(Value::Dict(HashMap::new()));
    }
    
    let mut obj = HashMap::new();
    let mut current = String::new();
    let mut depth = 0;
    let mut in_string = false;
    let mut escape_next = false;
    
    for ch in s.chars() {
        if escape_next {
            current.push(ch);
            escape_next = false;
            continue;
        }
        
        match ch {
            '\\' if in_string => {
                current.push(ch);
                escape_next = true;
            }
            '"' => {
                current.push(ch);
                in_string = !in_string;
            }
            '[' | '{' if !in_string => {
                current.push(ch);
                depth += 1;
            }
            ']' | '}' if !in_string => {
                current.push(ch);
                depth -= 1;
            }
            ',' if !in_string && depth == 0 => {
                if !current.trim().is_empty() {
                    parse_key_value_pair(current.trim(), &mut obj)?;
                }
                current.clear();
            }
            _ => {
                current.push(ch);
            }
        }
    }
    
    if !current.trim().is_empty() {
        parse_key_value_pair(current.trim(), &mut obj)?;
    }
    
    Ok(Value::Dict(obj))
}

/// Parse a key-value pair for JSON object
fn parse_key_value_pair(s: &str, obj: &mut HashMap<String, Value>) -> Result<()> {
    if let Some(colon_pos) = find_colon_separator(s) {
        let key_part = s[..colon_pos].trim();
        let value_part = s[colon_pos + 1..].trim();
        
        // Parse key (must be a string)
        let key = if key_part.starts_with('"') && key_part.ends_with('"') && key_part.len() >= 2 {
            key_part[1..key_part.len()-1].to_string()
        } else {
            return Err(anyhow::anyhow!("JSON object keys must be strings"));
        };
        
        let value = parse_json_value(value_part)?;
        obj.insert(key, value);
    } else {
        return Err(anyhow::anyhow!("Invalid key-value pair: {}", s));
    }
    
    Ok(())
}

/// Find the colon separator in a key-value pair
fn find_colon_separator(s: &str) -> Option<usize> {
    let mut in_string = false;
    let mut escape_next = false;
    
    for (i, ch) in s.char_indices() {
        if escape_next {
            escape_next = false;
            continue;
        }
        
        match ch {
            '\\' if in_string => escape_next = true,
            '"' => in_string = !in_string,
            ':' if !in_string => return Some(i),
            _ => {}
        }
    }
    
    None
}

/// Serialize a Value to JSON string
fn serialize_to_json(value: &Value, indent: Option<usize>, current_depth: usize) -> Result<Value> {
    let json_str = match value {
        Value::None => "null".to_string(),
        Value::Bool(b) => b.to_string(),
        Value::Int(n) => n.to_string(),
        Value::Float(f) => {
            if f.is_finite() {
                f.to_string()
            } else if f.is_infinite() {
                return Err(anyhow::anyhow!("Out of range float values are not JSON compliant"));
            } else {
                return Err(anyhow::anyhow!("NaN values are not JSON compliant"));
            }
        }
        Value::Str(s) => {
            // Simple escape (placeholder)
            let escaped = s
                .replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r")
                .replace("\t", "\\t");
            format!("\"{}\"", escaped)
        }
        Value::List(items) => {
            if items.is_empty() {
                "[]".to_string()
            } else {
                let mut result = String::from("[");
                if indent.is_some() {
                    result.push('\n');
                }
                
                for (i, item) in items.iter().enumerate() {
                    if let Some(indent_size) = indent {
                        result.push_str(&" ".repeat((current_depth + 1) * indent_size));
                    }
                    
                    let item_json = serialize_to_json(item, indent, current_depth + 1)?;
                    if let Value::Str(s) = item_json {
                        result.push_str(&s);
                    }
                    
                    if i < items.len() - 1 {
                        result.push(',');
                    }
                    
                    if let Some(indent_size) = indent {
                        result.push('\n');
                    }
                }
                
                if let Some(indent_size) = indent {
                    result.push_str(&" ".repeat(current_depth * indent_size));
                }
                result.push(']');
                result
            }
        }
        Value::Dict(map) => {
            if map.is_empty() {
                "{}".to_string()
            } else {
                let mut result = String::from("{");
                if indent.is_some() {
                    result.push('\n');
                }
                
                let items: Vec<_> = map.iter().collect();
                for (i, (key, value)) in items.iter().enumerate() {
                    if let Some(indent_size) = indent {
                        result.push_str(&" ".repeat((current_depth + 1) * indent_size));
                    }
                    
                    // Key must be a string
                    result.push_str(&format!("\"{}\":", key));
                    if let Some(indent_size) = indent {
                        result.push(' ');
                    }
                    
                    let value_json = serialize_to_json(value, indent, current_depth + 1)?;
                    if let Value::Str(s) = value_json {
                        result.push_str(&s);
                    }
                    
                    if i < items.len() - 1 {
                        result.push(',');
                    }
                    
                    if let Some(indent_size) = indent {
                        result.push('\n');
                    }
                }
                
                if let Some(indent_size) = indent {
                    result.push_str(&" ".repeat(current_depth * indent_size));
                }
                result.push('}');
                result
            }
        }
        Value::Tuple(items) => {
            // Serialize tuples as arrays
            let list_value = Value::List(HPList::from_values(items.clone()));
            return serialize_to_json(&list_value, indent, current_depth);
        }
        _ => {
            return Err(anyhow::anyhow!("Object of type '{}' is not JSON serializable", value.type_name()));
        }
    };
    
    Ok(Value::Str(json_str))
}