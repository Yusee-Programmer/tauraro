// Row widget - horizontal layout
use crate::value::Value;
use crate::modules::webviewtk::{rendering::*, utils::*};
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

pub struct Row;

pub fn create(args: Vec<Value>) -> Result<Value> {
    let mut children_html = Vec::new();
    let mut spacing = 0.0;
    let mut style = None;
    
    if let Some(kwargs) = extract_kwargs(&args) {
        if let Some(val) = kwargs.get("children") {
            if let Value::List(list) = val {
                for child in list.iter() {
                    if let Value::Dict(dict) = child {
                        if let Some(Value::Str(html)) = dict.borrow().get("_html") {
                            children_html.push(html.clone());
                        }
                    }
                }
            }
        }
        if let Some(val) = kwargs.get("spacing") {
            spacing = get_float(val, 0.0);
        }
        if let Some(val) = kwargs.get("style") {
            style = get_optional_string(val);
        }
    }
    
    let mut obj = RenderObject::new("div");
    obj = obj.with_style("display", "flex")
             .with_style("flex-direction", "row");
    
    if spacing > 0.0 {
        obj = obj.with_style("gap", &format!("{}px", spacing));
    }
    
    if let Some(ref style_str) = style {
        for part in style_str.split(';') {
            let part = part.trim();
            if part.is_empty() { continue; }
            if let Some((key, value)) = part.split_once(':') {
                obj = obj.with_style(key.trim(), value.trim());
            }
        }
    }
    
    let combined_children = children_html.join("");
    if !combined_children.is_empty() {
        obj = obj.with_raw_html(&combined_children);
    }
    
    let html = obj.to_html();
    
    let mut widget_dict = HashMap::new();
    widget_dict.insert("_widget_id".to_string(), Value::Str(generate_widget_id()));
    widget_dict.insert("_widget_type".to_string(), Value::Str("Row".to_string()));
    widget_dict.insert("_html".to_string(), Value::Str(html));
    
    Ok(Value::Dict(Rc::new(RefCell::new(widget_dict))))
}
