/// Title bar system for WebViewTK
/// Provides native title bar customization functions

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

/// Create a title bar configuration
/// Args: visible=true, title="", icon="", custom_controls=false, dark_mode=false
pub fn create_titlebar(args: Vec<Value>) -> Result<Value> {
    let visible = if !args.is_empty() {
        match &args[0] {
            Value::Bool(b) => *b,
            _ => true,
        }
    } else {
        true
    };

    let title = extract_string_arg(&args, 1).unwrap_or_default();
    let icon = extract_string_arg(&args, 2);
    
    let custom_controls = if args.len() > 3 {
        match &args[3] {
            Value::Bool(b) => *b,
            _ => false,
        }
    } else {
        false
    };

    let dark_mode = if args.len() > 4 {
        match &args[4] {
            Value::Bool(b) => *b,
            _ => false,
        }
    } else {
        false
    };

    let mut titlebar_obj = HashMap::new();
    titlebar_obj.insert("visible".to_string(), Value::Bool(visible));
    titlebar_obj.insert("title".to_string(), Value::Str(title));
    titlebar_obj.insert("custom_controls".to_string(), Value::Bool(custom_controls));
    titlebar_obj.insert("dark_mode".to_string(), Value::Bool(dark_mode));
    
    if let Some(icon_path) = icon {
        titlebar_obj.insert("icon".to_string(), Value::Str(icon_path));
    }

    titlebar_obj.insert("type".to_string(), Value::Str("titlebar".to_string()));

    Ok(Value::Dict(Rc::new(RefCell::new(titlebar_obj))))
}

/// Extract a string argument from a Value vector
pub fn extract_string_arg(args: &[Value], index: usize) -> Option<String> {
    if index < args.len() {
        match &args[index] {
            Value::Str(s) => Some(s.clone()),
            _ => None,
        }
    } else {
        None
    }
}
