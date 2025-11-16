// Rendering - Convert widget tree to HTML

use crate::value::Value;
use std::collections::HashMap;

/// RenderObject represents a node in the render tree
#[derive(Debug, Clone)]
pub struct RenderObject {
    pub tag: String,
    pub styles: Vec<(String, String)>,
    pub attributes: Vec<(String, String)>,
    pub text: Option<String>,
    pub raw_html: Option<String>,  // For embedding raw HTML (like child widgets)
    pub children: Vec<RenderObject>,
}

impl RenderObject {
    pub fn new(tag: &str) -> Self {
        RenderObject {
            tag: tag.to_string(),
            styles: Vec::new(),
            attributes: Vec::new(),
            text: None,
            raw_html: None,
            children: Vec::new(),
        }
    }

    pub fn with_style(mut self, key: &str, value: &str) -> Self {
        self.styles.push((key.to_string(), value.to_string()));
        self
    }

    pub fn with_attr(mut self, key: &str, value: &str) -> Self {
        self.attributes.push((key.to_string(), value.to_string()));
        self
    }

    pub fn with_text(mut self, text: &str) -> Self {
        self.text = Some(text.to_string());
        self
    }

    pub fn with_raw_html(mut self, html: &str) -> Self {
        self.raw_html = Some(html.to_string());
        self
    }

    pub fn with_child(mut self, child: RenderObject) -> Self {
        self.children.push(child);
        self
    }

    pub fn with_children(mut self, children: Vec<RenderObject>) -> Self {
        self.children = children;
        self
    }

    /// Convert to HTML string
    pub fn to_html(&self) -> String {
        let mut html = String::new();
        
        // Opening tag
        html.push_str(&format!("<{}", self.tag));
        
        // Attributes
        for (key, value) in &self.attributes {
            html.push_str(&format!(r#" {}="{}""#, key, escape_attr(value)));
        }
        
        // Styles
        if !self.styles.is_empty() {
            html.push_str(r#" style=""#);
            for (i, (key, value)) in self.styles.iter().enumerate() {
                if i > 0 {
                    html.push_str("; ");
                }
                html.push_str(&format!("{}: {}", key, value));
            }
            html.push('"');
        }
        
        html.push('>');
        
        // Content
        if let Some(text) = &self.text {
            html.push_str(&escape_html(text));
        }
        
        // Raw HTML (not escaped - for child widgets)
        if let Some(raw) = &self.raw_html {
            html.push_str(raw);
        }
        
        // Children
        for child in &self.children {
            html.push_str(&child.to_html());
        }
        
        // Closing tag
        html.push_str(&format!("</{}>", self.tag));
        
        html
    }
}

/// Extract widget data from Value
pub fn extract_widget_data(value: &Value) -> Option<HashMap<String, Value>> {
    match value {
        Value::Dict(dict) => Some(dict.borrow().clone()),
        _ => None,
    }
}

/// Get child widgets from a Value
pub fn get_children(value: &Value) -> Vec<Value> {
    match value {
        Value::List(list) => {
            let mut result = Vec::new();
            for item in list.iter() {
                result.push(item.clone());
            }
            result
        },
        _ => vec![],
    }
}

/// Get single child from Value
pub fn get_child(value: &Value) -> Option<Value> {
    Some(value.clone())
}

/// Get string from Value
pub fn get_string(value: &Value, default: &str) -> String {
    match value {
        Value::Str(s) => s.clone(),
        Value::Int(i) => i.to_string(),
        Value::Float(f) => f.to_string(),
        _ => default.to_string(),
    }
}

/// Get optional string from Value
pub fn get_optional_string(value: &Value) -> Option<String> {
    match value {
        Value::Str(s) => Some(s.clone()),
        Value::None => None,
        _ => Some(format!("{:?}", value)),
    }
}

/// Get float from Value
pub fn get_float(value: &Value, default: f64) -> f64 {
    match value {
        Value::Float(f) => *f,
        Value::Int(i) => *i as f64,
        _ => default,
    }
}

/// Get int from Value
pub fn get_int(value: &Value, default: i64) -> i64 {
    match value {
        Value::Int(i) => *i,
        Value::Float(f) => *f as i64,
        _ => default,
    }
}

/// Get bool from Value
pub fn get_bool(value: &Value, default: bool) -> bool {
    match value {
        Value::Bool(b) => *b,
        _ => default,
    }
}

fn escape_html(text: &str) -> String {
    text.replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
}

fn escape_attr(text: &str) -> String {
    text.replace('"', "&quot;")
        .replace('\'', "&#39;")
}
