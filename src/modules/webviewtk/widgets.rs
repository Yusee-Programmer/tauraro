/// HTML Widget Creation Functions
/// Provides functions to create various HTML elements programmatically

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;

// Re-export helper functions
pub use super::{build_element, escape_html, extract_string_arg, extract_dict_arg};

/// Create a div element
/// Args: content="", classes="", id="", style="", attrs={}
pub fn create_div(args: Vec<Value>) -> Result<Value> {
    let content = extract_string_arg(&args, 0).unwrap_or_default();
    let classes = extract_string_arg(&args, 1);
    let id = extract_string_arg(&args, 2);
    let style = extract_string_arg(&args, 3);
    let attrs = extract_dict_arg(&args, 4);

    let html = build_element(
        "div",
        &content,
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        attrs.as_ref(),
        false,
    );

    Ok(Value::Str(html))
}

/// Create a button element
/// Args: text="", classes="", event_handler="", id="", style="", attrs={}
/// event_handler can be:
///   - IPC message name (string): sends {"cmd": "event_name", "data": {}} to backend
///   - JavaScript code: raw JS code to execute
pub fn create_button(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_else(|| "Button".to_string());
    let classes = extract_string_arg(&args, 1);
    let event_handler = extract_string_arg(&args, 2);
    let id = extract_string_arg(&args, 3);
    let style = extract_string_arg(&args, 4);
    let mut attrs = extract_dict_arg(&args, 5).unwrap_or_default();

    // Auto-generate onclick from event_handler if provided
    if let Some(handler) = event_handler {
        if !handler.is_empty() && !attrs.contains_key("onclick") {
            // If handler looks like a simple identifier (no spaces, parens, etc.), treat as IPC message
            if handler.chars().all(|c| c.is_alphanumeric() || c == '_') {
                attrs.insert("onclick".to_string(), 
                    format!("window.ipc.postMessage(JSON.stringify({{cmd: '{}'}}))", handler));
            } else {
                // Otherwise, treat as raw JavaScript
                attrs.insert("onclick".to_string(), handler);
            }
        }
    }

    let html = build_element(
        "button",
        &escape_html(&text),
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        Some(&attrs),
        false,
    );

    Ok(Value::Str(html))
}

/// Create a paragraph element
/// Args: text="", classes="", id="", style="", attrs={}
pub fn create_paragraph(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_default();
    let classes = extract_string_arg(&args, 1);
    let id = extract_string_arg(&args, 2);
    let style = extract_string_arg(&args, 3);
    let attrs = extract_dict_arg(&args, 4);

    let html = build_element(
        "p",
        &escape_html(&text),
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        attrs.as_ref(),
        false,
    );

    Ok(Value::Str(html))
}

/// Create a heading element
/// Args: text="", level=1, classes="", id="", style="", attrs={}
pub fn create_heading(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_default();
    let level = if args.len() > 1 {
        match &args[1] {
            Value::Int(n) => (*n as u8).clamp(1, 6),
            _ => 1,
        }
    } else {
        1
    };
    let classes = extract_string_arg(&args, 2);
    let id = extract_string_arg(&args, 3);
    let style = extract_string_arg(&args, 4);
    let attrs = extract_dict_arg(&args, 5);

    let tag = format!("h{}", level);
    let html = build_element(
        &tag,
        &escape_html(&text),
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        attrs.as_ref(),
        false,
    );

    Ok(Value::Str(html))
}

// Individual heading functions
macro_rules! create_heading_fn {
    ($fn_name:ident, $level:expr) => {
        pub fn $fn_name(args: Vec<Value>) -> Result<Value> {
            let text = extract_string_arg(&args, 0).unwrap_or_default();
            let classes = extract_string_arg(&args, 1);
            let id = extract_string_arg(&args, 2);
            let style = extract_string_arg(&args, 3);
            let attrs = extract_dict_arg(&args, 4);

            let html = build_element(
                concat!("h", $level),
                &escape_html(&text),
                classes.as_deref(),
                id.as_deref(),
                style.as_deref(),
                attrs.as_ref(),
                false,
            );

            Ok(Value::Str(html))
        }
    };
}

create_heading_fn!(create_h1, "1");
create_heading_fn!(create_h2, "2");
create_heading_fn!(create_h3, "3");
create_heading_fn!(create_h4, "4");
create_heading_fn!(create_h5, "5");
create_heading_fn!(create_h6, "6");

/// Create a span element
pub fn create_span(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_default();
    let classes = extract_string_arg(&args, 1);
    let id = extract_string_arg(&args, 2);
    let style = extract_string_arg(&args, 3);
    let attrs = extract_dict_arg(&args, 4);

    let html = build_element(
        "span",
        &escape_html(&text),
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        attrs.as_ref(),
        false,
    );

    Ok(Value::Str(html))
}

/// Create an input element
/// Args: input_type="text", placeholder="", event_handler="", classes="", id="", style="", attrs={}
pub fn create_input(args: Vec<Value>) -> Result<Value> {
    let input_type = extract_string_arg(&args, 0).unwrap_or_else(|| "text".to_string());
    let placeholder = extract_string_arg(&args, 1);
    let event_handler = extract_string_arg(&args, 2);
    let classes = extract_string_arg(&args, 3);
    let id = extract_string_arg(&args, 4);
    let style = extract_string_arg(&args, 5);
    let mut attrs = extract_dict_arg(&args, 6).unwrap_or_default();

    attrs.insert("type".to_string(), input_type);
    if let Some(ph) = placeholder {
        attrs.insert("placeholder".to_string(), ph);
    }

    // Auto-generate onchange/oninput from event_handler if provided
    if let Some(handler) = event_handler {
        if !handler.is_empty() && !attrs.contains_key("onchange") && !attrs.contains_key("oninput") {
            if handler.chars().all(|c| c.is_alphanumeric() || c == '_') {
                // Send IPC with input value
                attrs.insert("oninput".to_string(), 
                    format!("window.ipc.postMessage(JSON.stringify({{cmd: '{}', value: this.value}}))", handler));
            } else {
                attrs.insert("oninput".to_string(), handler);
            }
        }
    }

    let html = build_element(
        "input",
        "",
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        Some(&attrs),
        true,
    );

    Ok(Value::Str(html))
}

/// Create a textarea element
pub fn create_textarea(args: Vec<Value>) -> Result<Value> {
    let content = extract_string_arg(&args, 0).unwrap_or_default();
    let classes = extract_string_arg(&args, 1);
    let id = extract_string_arg(&args, 2);
    let style = extract_string_arg(&args, 3);
    let attrs = extract_dict_arg(&args, 4);

    let html = build_element(
        "textarea",
        &escape_html(&content),
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        attrs.as_ref(),
        false,
    );

    Ok(Value::Str(html))
}

/// Create a label element
pub fn create_label(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_default();
    let for_attr = extract_string_arg(&args, 1);
    let classes = extract_string_arg(&args, 2);
    let id = extract_string_arg(&args, 3);
    let style = extract_string_arg(&args, 4);
    let mut attrs = extract_dict_arg(&args, 5).unwrap_or_default();

    if let Some(for_val) = for_attr {
        attrs.insert("for".to_string(), for_val);
    }

    let html = build_element(
        "label",
        &escape_html(&text),
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        Some(&attrs),
        false,
    );

    Ok(Value::Str(html))
}

/// Create a form element
pub fn create_form(args: Vec<Value>) -> Result<Value> {
    let content = extract_string_arg(&args, 0).unwrap_or_default();
    let action = extract_string_arg(&args, 1);
    let method = extract_string_arg(&args, 2);
    let classes = extract_string_arg(&args, 3);
    let id = extract_string_arg(&args, 4);
    let style = extract_string_arg(&args, 5);
    let mut attrs = extract_dict_arg(&args, 6).unwrap_or_default();

    if let Some(act) = action {
        attrs.insert("action".to_string(), act);
    }
    if let Some(mth) = method {
        attrs.insert("method".to_string(), mth);
    }

    let html = build_element(
        "form",
        &content,
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        Some(&attrs),
        false,
    );

    Ok(Value::Str(html))
}

/// Create an img element
pub fn create_img(args: Vec<Value>) -> Result<Value> {
    let src = extract_string_arg(&args, 0).unwrap_or_default();
    let alt = extract_string_arg(&args, 1);
    let classes = extract_string_arg(&args, 2);
    let id = extract_string_arg(&args, 3);
    let style = extract_string_arg(&args, 4);
    let mut attrs = extract_dict_arg(&args, 5).unwrap_or_default();

    attrs.insert("src".to_string(), src);
    if let Some(alt_text) = alt {
        attrs.insert("alt".to_string(), alt_text);
    }

    let html = build_element(
        "img",
        "",
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        Some(&attrs),
        true,
    );

    Ok(Value::Str(html))
}

/// Create an anchor (link) element
pub fn create_link(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_else(|| "Link".to_string());
    let href = extract_string_arg(&args, 1).unwrap_or_else(|| "#".to_string());
    let classes = extract_string_arg(&args, 2);
    let id = extract_string_arg(&args, 3);
    let style = extract_string_arg(&args, 4);
    let mut attrs = extract_dict_arg(&args, 5).unwrap_or_default();

    attrs.insert("href".to_string(), href);

    let html = build_element(
        "a",
        &escape_html(&text),
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        Some(&attrs),
        false,
    );

    Ok(Value::Str(html))
}

// List elements
macro_rules! create_list_element {
    ($fn_name:ident, $tag:expr) => {
        pub fn $fn_name(args: Vec<Value>) -> Result<Value> {
            let content = extract_string_arg(&args, 0).unwrap_or_default();
            let classes = extract_string_arg(&args, 1);
            let id = extract_string_arg(&args, 2);
            let style = extract_string_arg(&args, 3);
            let attrs = extract_dict_arg(&args, 4);

            let html = build_element(
                $tag,
                &content,
                classes.as_deref(),
                id.as_deref(),
                style.as_deref(),
                attrs.as_ref(),
                false,
            );

            Ok(Value::Str(html))
        }
    };
}

create_list_element!(create_ul, "ul");
create_list_element!(create_ol, "ol");
create_list_element!(create_li, "li");

// Table elements
create_list_element!(create_table, "table");
create_list_element!(create_tr, "tr");
create_list_element!(create_td, "td");
create_list_element!(create_th, "th");

// Semantic HTML5 elements
create_list_element!(create_nav, "nav");
create_list_element!(create_header, "header");
create_list_element!(create_footer, "footer");
create_list_element!(create_section, "section");
create_list_element!(create_article, "article");
create_list_element!(create_aside, "aside");
create_list_element!(create_main, "main");

/// Create a select dropdown
pub fn create_select(args: Vec<Value>) -> Result<Value> {
    let content = extract_string_arg(&args, 0).unwrap_or_default();
    let classes = extract_string_arg(&args, 1);
    let id = extract_string_arg(&args, 2);
    let style = extract_string_arg(&args, 3);
    let attrs = extract_dict_arg(&args, 4);

    let html = build_element(
        "select",
        &content,
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        attrs.as_ref(),
        false,
    );

    Ok(Value::Str(html))
}

/// Create an option element
pub fn create_option(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_default();
    let value = extract_string_arg(&args, 1);
    let classes = extract_string_arg(&args, 2);
    let id = extract_string_arg(&args, 3);
    let style = extract_string_arg(&args, 4);
    let mut attrs = extract_dict_arg(&args, 5).unwrap_or_default();

    if let Some(val) = value {
        attrs.insert("value".to_string(), val);
    }

    let html = build_element(
        "option",
        &escape_html(&text),
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        Some(&attrs),
        false,
    );

    Ok(Value::Str(html))
}

/// Create a checkbox input
pub fn create_checkbox(args: Vec<Value>) -> Result<Value> {
    let label = extract_string_arg(&args, 0).unwrap_or_default();
    let name = extract_string_arg(&args, 1);
    let classes = extract_string_arg(&args, 2);
    let id = extract_string_arg(&args, 3);
    let style = extract_string_arg(&args, 4);
    let mut attrs = extract_dict_arg(&args, 5).unwrap_or_default();

    attrs.insert("type".to_string(), "checkbox".to_string());
    if let Some(n) = name {
        attrs.insert("name".to_string(), n);
    }

    let input_html = build_element(
        "input",
        "",
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        Some(&attrs),
        true,
    );

    let html = if !label.is_empty() {
        format!("<label>{} {}</label>", input_html, escape_html(&label))
    } else {
        input_html
    };

    Ok(Value::Str(html))
}

/// Create a radio button input
pub fn create_radio(args: Vec<Value>) -> Result<Value> {
    let label = extract_string_arg(&args, 0).unwrap_or_default();
    let name = extract_string_arg(&args, 1);
    let value = extract_string_arg(&args, 2);
    let classes = extract_string_arg(&args, 3);
    let id = extract_string_arg(&args, 4);
    let style = extract_string_arg(&args, 5);
    let mut attrs = extract_dict_arg(&args, 6).unwrap_or_default();

    attrs.insert("type".to_string(), "radio".to_string());
    if let Some(n) = name {
        attrs.insert("name".to_string(), n);
    }
    if let Some(v) = value {
        attrs.insert("value".to_string(), v);
    }

    let input_html = build_element(
        "input",
        "",
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        Some(&attrs),
        true,
    );

    let html = if !label.is_empty() {
        format!("<label>{} {}</label>", input_html, escape_html(&label))
    } else {
        input_html
    };

    Ok(Value::Str(html))
}

// HTML structure functions

/// Create an html element
pub fn create_html(args: Vec<Value>) -> Result<Value> {
    let content = extract_string_arg(&args, 0).unwrap_or_default();
    let lang = extract_string_arg(&args, 1).unwrap_or_else(|| "en".to_string());
    let mut attrs = HashMap::new();
    attrs.insert("lang".to_string(), lang);

    let html = build_element("html", &content, None, None, None, Some(&attrs), false);
    Ok(Value::Str(html))
}

/// Create a head element
pub fn create_head(args: Vec<Value>) -> Result<Value> {
    let content = extract_string_arg(&args, 0).unwrap_or_default();
    let html = build_element("head", &content, None, None, None, None, false);
    Ok(Value::Str(html))
}

/// Create a body element
pub fn create_body(args: Vec<Value>) -> Result<Value> {
    let content = extract_string_arg(&args, 0).unwrap_or_default();
    let classes = extract_string_arg(&args, 1);
    let id = extract_string_arg(&args, 2);
    let style = extract_string_arg(&args, 3);
    let attrs = extract_dict_arg(&args, 4);

    let html = build_element(
        "body",
        &content,
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        attrs.as_ref(),
        false,
    );

    Ok(Value::Str(html))
}

/// Create a title element
pub fn create_title(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_else(|| "Tauraro App".to_string());
    let html = format!("<title>{}</title>", escape_html(&text));
    Ok(Value::Str(html))
}

/// Create a meta element
pub fn create_meta(args: Vec<Value>) -> Result<Value> {
    let attrs = extract_dict_arg(&args, 0).unwrap_or_default();
    let html = build_element("meta", "", None, None, None, Some(&attrs), true);
    Ok(Value::Str(html))
}

/// Create a style element
pub fn create_style(args: Vec<Value>) -> Result<Value> {
    let css = extract_string_arg(&args, 0).unwrap_or_default();
    let html = format!("<style>{}</style>", css);
    Ok(Value::Str(html))
}

/// Create a script element
pub fn create_script(args: Vec<Value>) -> Result<Value> {
    let js = extract_string_arg(&args, 0).unwrap_or_default();
    let html = format!("<script>{}</script>", js);
    Ok(Value::Str(html))
}
