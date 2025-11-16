// divider widget
use crate::value::Value;
use crate::modules::webviewtk::utils::*;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

pub struct Divider;

pub fn create(args: Vec<Value>) -> Result<Value> {
    let mut widget_dict = HashMap::new();
    widget_dict.insert("_widget_id".to_string(), Value::Str(generate_widget_id()));
    widget_dict.insert("_widget_type".to_string(), Value::Str("Divider".to_string()));
    widget_dict.insert("_html".to_string(), Value::Str("<div>divider</div>".to_string()));
    Ok(Value::Dict(Rc::new(RefCell::new(widget_dict))))
}
