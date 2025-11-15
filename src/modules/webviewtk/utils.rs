/// Utility Functions for WebViewTK
/// Helper functions for HTML generation and argument extraction

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;

/// Build an HTML element with optional attributes
pub fn build_element(
    tag: &str,
    content: &str,
    classes: Option<&str>,
    id: Option<&str>,
    style: Option<&str>,
    attrs: Option<&HashMap<String, String>>,
    self_closing: bool,
) -> String {
    let mut html = String::from("<");
    html.push_str(tag);

    if let Some(id_val) = id {
        if !id_val.is_empty() {
            html.push_str(&format!(" id=\"{}\"", escape_html(id_val)));
        }
    }

    if let Some(classes_val) = classes {
        if !classes_val.is_empty() {
            html.push_str(&format!(" class=\"{}\"", escape_html(classes_val)));
        }
    }

    if let Some(style_val) = style {
        if !style_val.is_empty() {
            html.push_str(&format!(" style=\"{}\"", escape_html(style_val)));
        }
    }

    if let Some(attrs_map) = attrs {
        for (key, value) in attrs_map {
            html.push_str(&format!(" {}=\"{}\"", escape_html(key), escape_html(value)));
        }
    }

    if self_closing {
        html.push_str(" />");
    } else {
        html.push('>');
        html.push_str(content);
        html.push_str(&format!("</{}>", tag));
    }

    html
}

/// Helper function to escape HTML
pub fn escape_html(text: &str) -> String {
    text.replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
        .replace('"', "&quot;")
        .replace('\'', "&#39;")
}

/// Helper to extract string argument from Value
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

/// Helper to extract dictionary argument from Value
pub fn extract_dict_arg(args: &[Value], index: usize) -> Option<HashMap<String, String>> {
    if index < args.len() {
        match &args[index] {
            Value::Dict(dict) => {
                let mut map = HashMap::new();
                for (key, val) in dict.borrow().iter() {
                    if let Value::Str(v) = val {
                        map.insert(key.clone(), v.clone());
                    }
                }
                Some(map)
            }
            _ => None,
        }
    } else {
        None
    }
}

/// Render HTML string (utility function)
pub fn render_html(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("render() requires at least one argument"));
    }

    let parts: Vec<String> = args.iter().map(|arg| {
        match arg {
            Value::Str(s) => s.clone(),
            _ => format!("{}", arg),
        }
    }).collect();

    Ok(Value::Str(parts.join("")))
}

/// Escape HTML utility function
pub fn escape_html_func(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("escape_html() requires a string argument"));
    }

    let text = extract_string_arg(&args, 0).unwrap_or_default();
    Ok(Value::Str(escape_html(&text)))
}
