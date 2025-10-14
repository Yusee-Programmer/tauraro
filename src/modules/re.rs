/// Re module - provides regular expression support
/// Similar to Python's re module

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use regex::{Regex, RegexBuilder, Match};
use lazy_static::lazy_static;
use std::sync::Mutex;
// Import HPList
use crate::modules::hplist::HPList;

// Regex cache for compiled patterns
lazy_static! {
    static ref REGEX_CACHE: Mutex<HashMap<String, Regex>> = Mutex::new(HashMap::new());
}

/// Clear the regex cache
fn purge_cache() {
    let mut cache = REGEX_CACHE.lock().unwrap();
    cache.clear();
}

/// Compile a regex pattern with flags
fn compile_pattern(pattern: &str, flags: i64) -> Result<Regex> {
    let cache_key = format!("{}:{}", pattern, flags);
    
    // Check cache first
    {
        let cache = REGEX_CACHE.lock().unwrap();
        if let Some(cached_regex) = cache.get(&cache_key) {
            return Ok(cached_regex.clone());
        }
    }
    
    // Build regex with flags
    let mut builder = RegexBuilder::new(pattern);
    
    if flags & 2 != 0 { // IGNORECASE
        builder.case_insensitive(true);
    }
    if flags & 4 != 0 { // LOCALE
        // LOCALE flag is not directly supported in regex crate
    }
    if flags & 8 != 0 { // MULTILINE
        builder.multi_line(true);
    }
    if flags & 16 != 0 { // DOTALL
        builder.dot_matches_new_line(true);
    }
    if flags & 32 != 0 { // UNICODE
        builder.unicode(true);
    }
    if flags & 64 != 0 { // VERBOSE
        builder.ignore_whitespace(true);
    }
    if flags & 256 != 0 { // ASCII
        builder.unicode(false);
    }
    
    let regex = builder.build()
        .map_err(|e| anyhow!("Invalid regex pattern: {}", e))?;
    
    // Cache the compiled regex
    let mut cache = REGEX_CACHE.lock().unwrap();
    cache.insert(cache_key, regex.clone());
    
    Ok(regex)
}

/// Create the re module object with all its functions and constants
pub fn create_re_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Regular expression functions
    namespace.insert("compile".to_string(), Value::NativeFunction(re_compile));
    namespace.insert("search".to_string(), Value::NativeFunction(re_search));
    namespace.insert("match".to_string(), Value::NativeFunction(re_match));
    namespace.insert("findall".to_string(), Value::NativeFunction(re_findall));
    namespace.insert("finditer".to_string(), Value::NativeFunction(re_finditer));
    namespace.insert("sub".to_string(), Value::NativeFunction(re_sub));
    namespace.insert("subn".to_string(), Value::NativeFunction(re_subn));
    namespace.insert("split".to_string(), Value::NativeFunction(re_split));
    namespace.insert("escape".to_string(), Value::NativeFunction(re_escape));
    namespace.insert("purge".to_string(), Value::NativeFunction(re_purge));
    
    // Regular expression flags (constants)
    namespace.insert("IGNORECASE".to_string(), Value::Int(2));
    namespace.insert("I".to_string(), Value::Int(2));
    namespace.insert("LOCALE".to_string(), Value::Int(4));
    namespace.insert("L".to_string(), Value::Int(4));
    namespace.insert("MULTILINE".to_string(), Value::Int(8));
    namespace.insert("M".to_string(), Value::Int(8));
    namespace.insert("DOTALL".to_string(), Value::Int(16));
    namespace.insert("S".to_string(), Value::Int(16));
    namespace.insert("UNICODE".to_string(), Value::Int(32));
    namespace.insert("U".to_string(), Value::Int(32));
    namespace.insert("VERBOSE".to_string(), Value::Int(64));
    namespace.insert("X".to_string(), Value::Int(64));
    namespace.insert("DEBUG".to_string(), Value::Int(128));
    namespace.insert("ASCII".to_string(), Value::Int(256));
    namespace.insert("A".to_string(), Value::Int(256));
    
    Value::Module("re".to_string(), namespace)
}

/// Compile a regular expression pattern
fn re_compile(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("compile() takes 1 or 2 arguments"));
    }
    
    let pattern = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow::anyhow!("pattern must be a string")),
    };
    
    let flags = if args.len() > 1 {
        match &args[1] {
            Value::Int(f) => *f,
            _ => return Err(anyhow::anyhow!("flags must be an integer")),
        }
    } else {
        0
    };
    
    // Create a compiled pattern object
    let mut pattern_obj = HashMap::new();
    pattern_obj.insert("pattern".to_string(), Value::Str(pattern));
    pattern_obj.insert("flags".to_string(), Value::Int(flags));
    pattern_obj.insert("search".to_string(), Value::NativeFunction(pattern_search));
    pattern_obj.insert("match".to_string(), Value::NativeFunction(pattern_match));
    pattern_obj.insert("findall".to_string(), Value::NativeFunction(pattern_findall));
    pattern_obj.insert("finditer".to_string(), Value::NativeFunction(pattern_finditer));
    pattern_obj.insert("sub".to_string(), Value::NativeFunction(pattern_sub));
    pattern_obj.insert("subn".to_string(), Value::NativeFunction(pattern_subn));
    pattern_obj.insert("split".to_string(), Value::NativeFunction(pattern_split));
    
    Ok(Value::Object {
        class_name: "Pattern".to_string(),
        fields: pattern_obj,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Pattern".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Pattern".to_string(), "object".to_string()]),
    })
}

/// Search for pattern in string
fn re_search(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 4 {
        return Err(anyhow::anyhow!("search() takes 2 to 4 arguments"));
    }
    
    let pattern = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("pattern must be a string")),
    };
    
    let string = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("string must be a string")),
    };
    
    let flags = if args.len() > 2 {
        match &args[2] {
            Value::Int(f) => *f,
            _ => return Err(anyhow::anyhow!("flags must be an integer")),
        }
    } else {
        0
    };
    
    // Compile and search
    let regex = compile_pattern(pattern, flags)?;
    
    if let Some(mat) = regex.find(string) {
        create_match_object_from_match(&regex, string, mat)
    } else {
        Ok(Value::None)
    }
}

/// Match pattern at beginning of string
fn re_match(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 4 {
        return Err(anyhow::anyhow!("match() takes 2 to 4 arguments"));
    }
    
    let pattern = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("pattern must be a string")),
    };
    
    let string = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("string must be a string")),
    };
    
    let flags = if args.len() > 2 {
        match &args[2] {
            Value::Int(f) => *f,
            _ => return Err(anyhow::anyhow!("flags must be an integer")),
        }
    } else {
        0
    };
    
    // Compile and match at beginning
    let regex = compile_pattern(pattern, flags)?;
    
    if let Some(mat) = regex.find_at(string, 0) {
        if mat.start() == 0 {
            create_match_object_from_match(&regex, string, mat)
        } else {
            Ok(Value::None)
        }
    } else {
        Ok(Value::None)
    }
}

/// Find all non-overlapping matches
fn re_findall(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 4 {
        return Err(anyhow::anyhow!("findall() takes 2 to 4 arguments"));
    }
    
    let pattern = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("pattern must be a string")),
    };
    
    let string = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("string must be a string")),
    };
    
    let flags = if args.len() > 2 {
        match &args[2] {
            Value::Int(f) => *f,
            _ => return Err(anyhow::anyhow!("flags must be an integer")),
        }
    } else {
        0
    };
    
    // Compile and find all matches
    let regex = compile_pattern(pattern, flags)?;
    let matches: Vec<Value> = regex.find_iter(string)
        .map(|mat| Value::Str(mat.as_str().to_string()))
        .collect();
    
    Ok(Value::List(HPList::from_values(matches)))
}

/// Find all matches as iterator
fn re_finditer(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 4 {
        return Err(anyhow::anyhow!("finditer() takes 2 to 4 arguments"));
    }
    
    let pattern = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("pattern must be a string")),
    };
    
    let string = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("string must be a string")),
    };
    
    let flags = if args.len() > 2 {
        match &args[2] {
            Value::Int(f) => *f,
            _ => return Err(anyhow::anyhow!("flags must be an integer")),
        }
    } else {
        0
    };
    
    // Compile and find all matches as iterator objects
    let regex = compile_pattern(pattern, flags)?;
    let matches: Vec<Value> = regex.find_iter(string)
        .map(|mat| {
            create_match_object_from_match(&regex, string, mat)
                .unwrap_or(Value::None)
        })
        .collect();
    
    Ok(Value::List(HPList::from_values(matches)))
}

/// Substitute occurrences of pattern
fn re_sub(args: Vec<Value>) -> Result<Value> {
    if args.len() < 3 || args.len() > 5 {
        return Err(anyhow::anyhow!("sub() takes 3 to 5 arguments"));
    }
    
    let pattern = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("pattern must be a string")),
    };
    
    let repl = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("replacement must be a string")),
    };
    
    let string = match &args[2] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("string must be a string")),
    };
    
    let flags = if args.len() > 3 {
        match &args[3] {
            Value::Int(f) => *f,
            _ => return Err(anyhow::anyhow!("flags must be an integer")),
        }
    } else {
        0
    };
    
    let count = if args.len() > 4 {
        match &args[4] {
            Value::Int(c) => *c,
            _ => return Err(anyhow::anyhow!("count must be an integer")),
        }
    } else {
        0
    };
    
    // Compile and substitute
    let regex = compile_pattern(pattern, flags)?;
    let result = if count > 0 {
        regex.replacen(string, count as usize, repl)
    } else {
        regex.replace_all(string, repl)
    };
    
    Ok(Value::Str(result.to_string()))
}

/// Substitute occurrences and return count
fn re_subn(args: Vec<Value>) -> Result<Value> {
    if args.len() < 3 || args.len() > 5 {
        return Err(anyhow::anyhow!("subn() takes 3 to 5 arguments"));
    }
    
    let pattern = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("pattern must be a string")),
    };
    
    let repl = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("replacement must be a string")),
    };
    
    let string = match &args[2] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("string must be a string")),
    };
    
    let flags = if args.len() > 3 {
        match &args[3] {
            Value::Int(f) => *f,
            _ => return Err(anyhow::anyhow!("flags must be an integer")),
        }
    } else {
        0
    };
    
    let count = if args.len() > 4 {
        match &args[4] {
            Value::Int(c) => *c,
            _ => return Err(anyhow::anyhow!("count must be an integer")),
        }
    } else {
        0
    };
    
    // Compile and substitute with count
    let regex = compile_pattern(pattern, flags)?;
    let (result, actual_count) = if count > 0 {
        let result = regex.replacen(string, count as usize, repl);
        (result.to_string(), count as i64)
    } else {
        let result = regex.replace_all(string, repl);
        let actual_count = regex.find_iter(string).count() as i64;
        (result.to_string(), actual_count)
    };
    
    Ok(Value::Tuple(vec![Value::Str(result), Value::Int(actual_count)]))
}

/// Split string by pattern
fn re_split(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 4 {
        return Err(anyhow::anyhow!("split() takes 2 to 4 arguments"));
    }
    
    let pattern = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("pattern must be a string")),
    };
    
    let string = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("string must be a string")),
    };
    
    let flags = if args.len() > 2 {
        match &args[2] {
            Value::Int(f) => *f,
            _ => return Err(anyhow::anyhow!("flags must be an integer")),
        }
    } else {
        0
    };
    
    let maxsplit = if args.len() > 3 {
        match &args[3] {
            Value::Int(m) => *m,
            _ => return Err(anyhow::anyhow!("maxsplit must be an integer")),
        }
    } else {
        0
    };
    
    // Compile and split
    let regex = compile_pattern(pattern, flags)?;
    let parts: Vec<Value> = if maxsplit > 0 {
        regex.splitn(string, maxsplit as usize + 1)
            .map(|s| Value::Str(s.to_string()))
            .collect()
    } else {
        regex.split(string)
            .map(|s| Value::Str(s.to_string()))
            .collect()
    };
    
    Ok(Value::List(HPList::from_values(parts)))
}

/// Escape special characters in pattern
fn re_escape(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("escape() takes exactly 1 argument"));
    }
    
    let pattern = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("pattern must be a string")),
    };
    
    // Use regex crate's escape function for proper escaping
    let escaped = regex::escape(pattern);
    
    Ok(Value::Str(escaped))
}

/// Clear the regular expression cache
fn re_purge(_args: Vec<Value>) -> Result<Value> {
    let mut cache = REGEX_CACHE.lock().unwrap();
    cache.clear();
    Ok(Value::None)
}

// Pattern object methods
fn pattern_search(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::None)
}

fn pattern_match(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::None)
}

fn pattern_findall(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::List(HPList::new()))
}

fn pattern_finditer(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::List(HPList::new()))
}

fn pattern_sub(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::Str("".to_string()))
}

fn pattern_subn(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::Tuple(vec![Value::Str("".to_string()), Value::Int(0)]))
}

fn pattern_split(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::List(HPList::new()))
}

/// Create a match object from a regex Match
fn create_match_object_from_match(regex: &Regex, string: &str, mat: Match) -> Result<Value> {
    let mut match_obj = HashMap::new();
    match_obj.insert("string".to_string(), Value::Str(string.to_string()));
    match_obj.insert("re".to_string(), Value::Str(regex.as_str().to_string()));
    match_obj.insert("pos".to_string(), Value::Int(mat.start() as i64));
    match_obj.insert("endpos".to_string(), Value::Int(mat.end() as i64));
    match_obj.insert("group".to_string(), Value::NativeFunction(match_group));
    match_obj.insert("groups".to_string(), Value::NativeFunction(match_groups));
    match_obj.insert("groupdict".to_string(), Value::NativeFunction(match_groupdict));
    match_obj.insert("start".to_string(), Value::NativeFunction(match_start));
    match_obj.insert("end".to_string(), Value::NativeFunction(match_end));
    match_obj.insert("span".to_string(), Value::NativeFunction(match_span));
    
    // Create a simple object structure (simplified for this implementation)
    Ok(Value::Object {
        class_name: "Match".to_string(),
        fields: match_obj,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Match".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Match".to_string(), "object".to_string()]),
    })
}

/// Create a match object (legacy function for compatibility)
fn create_match_object(pattern: &str, string: &str, start: usize) -> Result<Value> {
    let mut match_obj = HashMap::new();
    match_obj.insert("string".to_string(), Value::Str(string.to_string()));
    match_obj.insert("re".to_string(), Value::Str(pattern.to_string()));
    match_obj.insert("pos".to_string(), Value::Int(start as i64));
    match_obj.insert("endpos".to_string(), Value::Int(string.len() as i64));
    match_obj.insert("group".to_string(), Value::NativeFunction(match_group));
    match_obj.insert("groups".to_string(), Value::NativeFunction(match_groups));
    match_obj.insert("groupdict".to_string(), Value::NativeFunction(match_groupdict));
    match_obj.insert("start".to_string(), Value::NativeFunction(match_start));
    match_obj.insert("end".to_string(), Value::NativeFunction(match_end));
    match_obj.insert("span".to_string(), Value::NativeFunction(match_span));
    
    Ok(Value::Object {
        class_name: "Match".to_string(),
        fields: match_obj,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Match".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Match".to_string(), "object".to_string()]),
    })
}

// Match object methods
fn match_group(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::Str("".to_string()))
}

fn match_groups(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::Tuple(vec![]))
}

fn match_groupdict(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::Dict(HashMap::new()))
}

fn match_start(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::Int(0))
}

fn match_end(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::Int(0))
}

fn match_span(_args: Vec<Value>) -> Result<Value> {
    // Placeholder implementation
    Ok(Value::Tuple(vec![Value::Int(0), Value::Int(0)]))
}