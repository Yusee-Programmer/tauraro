/// CDN and External Resource Functions
/// Provides functions to include external libraries and resources

use crate::value::Value;
use anyhow::Result;

// Re-export helper functions
pub use super::{escape_html, extract_string_arg};

/// Include Tailwind CSS via CDN
pub fn cdn_tailwind(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "3.3.0".to_string());
    let html = format!(
        "<script src=\"https://cdn.tailwindcss.com?v={}\"></script>",
        escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include Bootstrap via CDN
pub fn cdn_bootstrap(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "5.3.0".to_string());
    let html = format!(
        "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@{}/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n\
         <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@{}/dist/js/bootstrap.bundle.min.js\"></script>",
        escape_html(&version), escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include jQuery via CDN
pub fn cdn_jquery(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "3.7.0".to_string());
    let html = format!(
        "<script src=\"https://code.jquery.com/jquery-{}.min.js\"></script>",
        escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include Vue.js via CDN
pub fn cdn_vue(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "3.3.4".to_string());
    let html = format!(
        "<script src=\"https://unpkg.com/vue@{}/dist/vue.global.js\"></script>",
        escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include React via CDN
pub fn cdn_react(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "18.2.0".to_string());
    let html = format!(
        "<script crossorigin src=\"https://unpkg.com/react@{}/umd/react.production.min.js\"></script>\n\
         <script crossorigin src=\"https://unpkg.com/react-dom@{}/umd/react-dom.production.min.js\"></script>",
        escape_html(&version), escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include Alpine.js via CDN
pub fn cdn_alpine(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "3.x.x".to_string());
    let html = format!(
        "<script defer src=\"https://cdn.jsdelivr.net/npm/alpinejs@{}/dist/cdn.min.js\"></script>",
        escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include custom CDN link
/// Args: url, type="script" (can be "script" or "style")
pub fn cdn_custom(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("cdn_custom() requires at least a URL argument"));
    }

    let url = extract_string_arg(&args, 0).unwrap_or_default();
    let link_type = extract_string_arg(&args, 1).unwrap_or_else(|| "script".to_string());

    let html = match link_type.as_str() {
        "style" | "css" => format!("<link rel=\"stylesheet\" href=\"{}\" />", escape_html(&url)),
        _ => format!("<script src=\"{}\"></script>", escape_html(&url)),
    };

    Ok(Value::Str(html))
}

/// Create a stylesheet link
pub fn style_link(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("style_link() requires a URL argument"));
    }

    let url = extract_string_arg(&args, 0).unwrap_or_default();
    let html = format!("<link rel=\"stylesheet\" href=\"{}\" />", escape_html(&url));
    Ok(Value::Str(html))
}

/// Create a script link
pub fn script_link(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("script_link() requires a URL argument"));
    }

    let url = extract_string_arg(&args, 0).unwrap_or_default();
    let html = format!("<script src=\"{}\"></script>", escape_html(&url));
    Ok(Value::Str(html))
}
