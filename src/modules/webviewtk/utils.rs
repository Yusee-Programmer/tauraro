// Utility functions and helpers

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;

/// EdgeInsets for padding and margin
#[derive(Debug, Clone)]
pub struct EdgeInsets {
    pub top: f64,
    pub right: f64,
    pub bottom: f64,
    pub left: f64,
}

impl EdgeInsets {
    pub fn all(value: f64) -> Self {
        EdgeInsets {
            top: value,
            right: value,
            bottom: value,
            left: value,
        }
    }

    pub fn symmetric(vertical: f64, horizontal: f64) -> Self {
        EdgeInsets {
            top: vertical,
            right: horizontal,
            bottom: vertical,
            left: horizontal,
        }
    }

    pub fn only(top: f64, right: f64, bottom: f64, left: f64) -> Self {
        EdgeInsets { top, right, bottom, left }
    }

    pub fn zero() -> Self {
        EdgeInsets::all(0.0)
    }

    pub fn to_css(&self) -> String {
        format!("{}px {}px {}px {}px", self.top, self.right, self.bottom, self.left)
    }
}

// EdgeInsets constructors for Tauraro
pub fn edgeinsets_all(args: Vec<Value>) -> Result<Value> {
    let value = args.get(0)
        .and_then(|v| match v {
            Value::Float(f) => Some(*f),
            Value::Int(i) => Some(*i as f64),
            _ => None,
        })
        .unwrap_or(0.0);
    
    let insets = EdgeInsets::all(value);
    Ok(Value::Str(insets.to_css()))
}

pub fn edgeinsets_symmetric(args: Vec<Value>) -> Result<Value> {
    let mut vertical = 0.0;
    let mut horizontal = 0.0;
    
    if let Some(kwargs) = extract_kwargs(&args) {
        if let Some(v) = kwargs.get("vertical") {
            vertical = match v {
                Value::Float(f) => *f,
                Value::Int(i) => *i as f64,
                _ => 0.0,
            };
        }
        if let Some(h) = kwargs.get("horizontal") {
            horizontal = match h {
                Value::Float(f) => *f,
                Value::Int(i) => *i as f64,
                _ => 0.0,
            };
        }
    } else {
        // Fallback to positional arguments
        vertical = args.get(0)
            .and_then(|v| match v {
                Value::Float(f) => Some(*f),
                Value::Int(i) => Some(*i as f64),
                _ => None,
            })
            .unwrap_or(0.0);
        
        horizontal = args.get(1)
            .and_then(|v| match v {
                Value::Float(f) => Some(*f),
                Value::Int(i) => Some(*i as f64),
                _ => None,
            })
            .unwrap_or(0.0);
    }
    
    let insets = EdgeInsets::symmetric(vertical, horizontal);
    Ok(Value::Str(insets.to_css()))
}

pub fn edgeinsets_only(args: Vec<Value>) -> Result<Value> {
    let mut top = 0.0;
    let mut right = 0.0;
    let mut bottom = 0.0;
    let mut left = 0.0;
    
    if let Some(kwargs) = extract_kwargs(&args) {
        if let Some(v) = kwargs.get("top") {
            top = match v {
                Value::Float(f) => *f,
                Value::Int(i) => *i as f64,
                _ => 0.0,
            };
        }
        if let Some(v) = kwargs.get("right") {
            right = match v {
                Value::Float(f) => *f,
                Value::Int(i) => *i as f64,
                _ => 0.0,
            };
        }
        if let Some(v) = kwargs.get("bottom") {
            bottom = match v {
                Value::Float(f) => *f,
                Value::Int(i) => *i as f64,
                _ => 0.0,
            };
        }
        if let Some(v) = kwargs.get("left") {
            left = match v {
                Value::Float(f) => *f,
                Value::Int(i) => *i as f64,
                _ => 0.0,
            };
        }
    } else {
        // Fallback to positional arguments
        top = args.get(0)
            .and_then(|v| match v {
                Value::Float(f) => Some(*f),
                Value::Int(i) => Some(*i as f64),
                _ => None,
            })
            .unwrap_or(0.0);
        
        right = args.get(1)
            .and_then(|v| match v {
                Value::Float(f) => Some(*f),
                Value::Int(i) => Some(*i as f64),
                _ => None,
            })
            .unwrap_or(0.0);
        
        bottom = args.get(2)
            .and_then(|v| match v {
                Value::Float(f) => Some(*f),
                Value::Int(i) => Some(*i as f64),
                _ => None,
            })
            .unwrap_or(0.0);
        
        left = args.get(3)
            .and_then(|v| match v {
                Value::Float(f) => Some(*f),
                Value::Int(i) => Some(*i as f64),
                _ => None,
            })
            .unwrap_or(0.0);
    }
    
    let insets = EdgeInsets::only(top, right, bottom, left);
    Ok(Value::Str(insets.to_css()))
}

pub fn edgeinsets_zero(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("0px 0px 0px 0px".to_string()))
}

/// Extract kwargs from args (supports both dict and KwargsMarker)
pub fn extract_kwargs(args: &[Value]) -> Option<HashMap<String, Value>> {
    // Check for KwargsMarker (Flutter-style)
    if let Some(Value::KwargsMarker(kwargs)) = args.last() {
        return Some(kwargs.clone());
    }
    
    // Check for Dict (dictionary-style)
    if let Some(Value::Dict(dict)) = args.get(0) {
        return Some(dict.borrow().clone());
    }
    
    None
}

/// Generate unique widget ID
use std::sync::atomic::{AtomicUsize, Ordering};
static WIDGET_COUNTER: AtomicUsize = AtomicUsize::new(0);

pub fn generate_widget_id() -> String {
    let id = WIDGET_COUNTER.fetch_add(1, Ordering::SeqCst);
    format!("widget_{}", id)
}

/// Parse style string and merge with inline styles
pub fn merge_styles(base_styles: &[(String, String)], style_str: Option<&str>) -> Vec<(String, String)> {
    let mut styles = base_styles.to_vec();
    
    if let Some(style_str) = style_str {
        for part in style_str.split(';') {
            let part = part.trim();
            if part.is_empty() {
                continue;
            }
            if let Some((key, value)) = part.split_once(':') {
                styles.push((key.trim().to_string(), value.trim().to_string()));
            }
        }
    }
    
    styles
}
