// Text widget - displays text content

use crate::value::Value;
use crate::modules::webviewtk::{rendering::*, utils::*};
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

pub struct Text {
    pub content: String,
    pub font_size: Option<f64>,
    pub font_weight: Option<String>,
    pub color: Option<String>,
    pub text_align: Option<String>,
    pub style: Option<String>,
    pub raw_html: bool,
}

impl Text {
    pub fn render(&self) -> RenderObject {
        let mut obj = RenderObject::new("span");
        
        // Apply inline styles
        if let Some(size) = self.font_size {
            obj = obj.with_style("font-size", &format!("{}px", size));
        }
        if let Some(ref weight) = self.font_weight {
            obj = obj.with_style("font-weight", weight);
        }
        if let Some(ref color) = self.color {
            obj = obj.with_style("color", color);
        }
        if let Some(ref align) = self.text_align {
            obj = obj.with_style("text-align", align);
        }
        
        // Custom styles
        if let Some(ref style_str) = self.style {
            for part in style_str.split(';') {
                let part = part.trim();
                if part.is_empty() { continue; }
                if let Some((key, value)) = part.split_once(':') {
                    obj = obj.with_style(key.trim(), value.trim());
                }
            }
        }
        
        obj.with_text(&self.content)
    }
}

pub fn create(args: Vec<Value>) -> Result<Value> {
    let mut content = String::new();
    let mut font_size = None;
    let mut font_weight = None;
    let mut color = None;
    let mut text_align = None;
    let mut style = None;
    let mut raw_html = false;
    
    // Support positional first argument
    if let Some(Value::Str(text)) = args.get(0) {
        content = text.clone();
    }
    
    // Extract keyword arguments
    if let Some(kwargs) = extract_kwargs(&args) {
        if let Some(val) = kwargs.get("text") {
            content = get_string(val, "");
        }
        if let Some(val) = kwargs.get("content") {
            content = get_string(val, "");
        }
        if let Some(val) = kwargs.get("font_size") {
            font_size = Some(get_float(val, 14.0));
        }
        if let Some(val) = kwargs.get("font_weight") {
            font_weight = get_optional_string(val);
        }
        if let Some(val) = kwargs.get("color") {
            color = get_optional_string(val);
        }
        if let Some(val) = kwargs.get("text_align") {
            text_align = get_optional_string(val);
        }
        if let Some(val) = kwargs.get("style") {
            style = get_optional_string(val);
        }
        if let Some(Value::Bool(b)) = kwargs.get("raw_html") {
            raw_html = *b;
        }
    }
    
    let text = Text {
        content,
        font_size,
        font_weight,
        color,
        text_align,
        style,
        raw_html,
    };
    
    let html = if raw_html {
        // Return raw HTML directly without wrapping in span
        text.content.clone()
    } else {
        text.render().to_html()
    };
    
    let widget_id = generate_widget_id();
    let mut widget_dict = HashMap::new();
    widget_dict.insert("_widget_id".to_string(), Value::Str(widget_id));
    widget_dict.insert("_widget_type".to_string(), Value::Str("Text".to_string()));
    widget_dict.insert("_html".to_string(), Value::Str(html));
    
    Ok(Value::Dict(Rc::new(RefCell::new(widget_dict))))
}
