// Container widget - box model with styling

use crate::value::Value;
use crate::modules::webviewtk::{rendering::*, utils::*};
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

pub struct Container {
    pub child: Option<String>,
    pub width: Option<f64>,
    pub height: Option<f64>,
    pub padding: Option<String>,
    pub margin: Option<String>,
    pub background_color: Option<String>,
    pub border_radius: Option<f64>,
    pub border: Option<String>,
    pub style: Option<String>,
}

impl Container {
    pub fn render(&self, child_html: &str) -> RenderObject {
        let mut obj = RenderObject::new("div");
        
        if let Some(w) = self.width {
            obj = obj.with_style("width", &format!("{}px", w));
        }
        if let Some(h) = self.height {
            obj = obj.with_style("height", &format!("{}px", h));
        }
        if let Some(ref p) = self.padding {
            obj = obj.with_style("padding", p);
        }
        if let Some(ref m) = self.margin {
            obj = obj.with_style("margin", m);
        }
        if let Some(ref bg) = self.background_color {
            obj = obj.with_style("background-color", bg);
        }
        if let Some(r) = self.border_radius {
            obj = obj.with_style("border-radius", &format!("{}px", r));
        }
        if let Some(ref b) = self.border {
            obj = obj.with_style("border", b);
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
        
        if !child_html.is_empty() {
            obj = obj.with_raw_html(child_html);
        }
        
        obj
    }
}

pub fn create(args: Vec<Value>) -> Result<Value> {
    let mut child = None;
    let mut width = None;
    let mut height = None;
    let mut padding = None;
    let mut margin = None;
    let mut background_color = None;
    let mut border_radius = None;
    let mut border = None;
    let mut style = None;
    
    if let Some(kwargs) = extract_kwargs(&args) {
        if let Some(val) = kwargs.get("child") {
            if let Value::Dict(dict) = val {
                if let Some(Value::Str(html)) = dict.borrow().get("_html") {
                    child = Some(html.clone());
                }
            }
        }
        if let Some(val) = kwargs.get("width") {
            width = Some(get_float(val, 0.0));
        }
        if let Some(val) = kwargs.get("height") {
            height = Some(get_float(val, 0.0));
        }
        if let Some(val) = kwargs.get("padding") {
            padding = get_optional_string(val);
        }
        if let Some(val) = kwargs.get("margin") {
            margin = get_optional_string(val);
        }
        if let Some(val) = kwargs.get("background_color") {
            background_color = get_optional_string(val);
        }
        if let Some(val) = kwargs.get("border_radius") {
            border_radius = Some(get_float(val, 0.0));
        }
        if let Some(val) = kwargs.get("border") {
            border = get_optional_string(val);
        }
        if let Some(val) = kwargs.get("style") {
            style = get_optional_string(val);
        }
    }
    
    let container = Container {
        child: child.clone(),
        width,
        height,
        padding,
        margin,
        background_color,
        border_radius,
        border,
        style,
    };
    
    let html = container.render(&child.unwrap_or_default()).to_html();
    
    let widget_id = generate_widget_id();
    let mut widget_dict = HashMap::new();
    widget_dict.insert("_widget_id".to_string(), Value::Str(widget_id));
    widget_dict.insert("_widget_type".to_string(), Value::Str("Container".to_string()));
    widget_dict.insert("_html".to_string(), Value::Str(html));
    
    Ok(Value::Dict(Rc::new(RefCell::new(widget_dict))))
}
