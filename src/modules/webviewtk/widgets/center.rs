// Center widget

use crate::value::Value;
use crate::modules::webviewtk::{rendering::*, utils::*};
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

pub struct Center;

pub fn create(args: Vec<Value>) -> Result<Value> {
    let mut child_html = String::new();
    
    if let Some(kwargs) = extract_kwargs(&args) {
        if let Some(val) = kwargs.get("child") {
            if let Value::Dict(dict) = val {
                if let Some(Value::Str(html)) = dict.borrow().get("_html") {
                    child_html = html.clone();
                }
            }
        }
    }
    
    let mut obj = RenderObject::new("div");
    obj = obj.with_style("display", "flex")
             .with_style("justify-content", "center")
             .with_style("align-items", "center");
    
    if !child_html.is_empty() {
        obj = obj.with_raw_html(&child_html);
    }
    
    let html = obj.to_html();
    
    let mut widget_dict = HashMap::new();
    widget_dict.insert("_widget_id".to_string(), Value::Str(generate_widget_id()));
    widget_dict.insert("_widget_type".to_string(), Value::Str("Center".to_string()));
    widget_dict.insert("_html".to_string(), Value::Str(html));
    
    Ok(Value::Dict(Rc::new(RefCell::new(widget_dict))))
}
