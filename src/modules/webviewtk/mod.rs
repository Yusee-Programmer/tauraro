/// WebViewTK - Cross-platform GUI framework for Tauraro
/// Uses HTML, CSS, and JavaScript for UI rendering with Tauraro handling the logic
/// Similar to Tauri and Electron.js

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

#[cfg(feature = "webviewtk")]
use wry::{
    application::{
        event::{Event, WindowEvent},
        event_loop::{ControlFlow, EventLoop},
        window::WindowBuilder,
    },
    webview::WebViewBuilder,
};

#[cfg(all(feature = "webviewtk", target_os = "windows"))]
use wry::application::platform::windows::EventLoopExtWindows;

/// Create the webviewtk module object with all its functions and classes
pub fn create_webviewtk_module() -> Value {
    let mut namespace = HashMap::new();

    // Widget creation functions
    namespace.insert("div".to_string(), Value::NativeFunction(create_div));
    namespace.insert("button".to_string(), Value::NativeFunction(create_button));
    namespace.insert("paragraph".to_string(), Value::NativeFunction(create_paragraph));
    namespace.insert("p".to_string(), Value::NativeFunction(create_paragraph)); // Alias
    namespace.insert("heading".to_string(), Value::NativeFunction(create_heading));
    namespace.insert("h1".to_string(), Value::NativeFunction(create_h1));
    namespace.insert("h2".to_string(), Value::NativeFunction(create_h2));
    namespace.insert("h3".to_string(), Value::NativeFunction(create_h3));
    namespace.insert("h4".to_string(), Value::NativeFunction(create_h4));
    namespace.insert("h5".to_string(), Value::NativeFunction(create_h5));
    namespace.insert("h6".to_string(), Value::NativeFunction(create_h6));
    namespace.insert("span".to_string(), Value::NativeFunction(create_span));
    namespace.insert("input".to_string(), Value::NativeFunction(create_input));
    namespace.insert("textarea".to_string(), Value::NativeFunction(create_textarea));
    namespace.insert("label".to_string(), Value::NativeFunction(create_label));
    namespace.insert("form".to_string(), Value::NativeFunction(create_form));
    namespace.insert("img".to_string(), Value::NativeFunction(create_img));
    namespace.insert("link".to_string(), Value::NativeFunction(create_link));
    namespace.insert("a".to_string(), Value::NativeFunction(create_link)); // Alias
    namespace.insert("ul".to_string(), Value::NativeFunction(create_ul));
    namespace.insert("ol".to_string(), Value::NativeFunction(create_ol));
    namespace.insert("li".to_string(), Value::NativeFunction(create_li));
    namespace.insert("table".to_string(), Value::NativeFunction(create_table));
    namespace.insert("tr".to_string(), Value::NativeFunction(create_tr));
    namespace.insert("td".to_string(), Value::NativeFunction(create_td));
    namespace.insert("th".to_string(), Value::NativeFunction(create_th));
    namespace.insert("nav".to_string(), Value::NativeFunction(create_nav));
    namespace.insert("header".to_string(), Value::NativeFunction(create_header));
    namespace.insert("footer".to_string(), Value::NativeFunction(create_footer));
    namespace.insert("section".to_string(), Value::NativeFunction(create_section));
    namespace.insert("article".to_string(), Value::NativeFunction(create_article));
    namespace.insert("aside".to_string(), Value::NativeFunction(create_aside));
    namespace.insert("main".to_string(), Value::NativeFunction(create_main));
    namespace.insert("select".to_string(), Value::NativeFunction(create_select));
    namespace.insert("option".to_string(), Value::NativeFunction(create_option));
    namespace.insert("checkbox".to_string(), Value::NativeFunction(create_checkbox));
    namespace.insert("radio".to_string(), Value::NativeFunction(create_radio));

    // CDN and external resource functions
    namespace.insert("cdn_tailwind".to_string(), Value::NativeFunction(cdn_tailwind));
    namespace.insert("cdn_bootstrap".to_string(), Value::NativeFunction(cdn_bootstrap));
    namespace.insert("cdn_jquery".to_string(), Value::NativeFunction(cdn_jquery));
    namespace.insert("cdn_vue".to_string(), Value::NativeFunction(cdn_vue));
    namespace.insert("cdn_react".to_string(), Value::NativeFunction(cdn_react));
    namespace.insert("cdn_alpine".to_string(), Value::NativeFunction(cdn_alpine));
    namespace.insert("cdn_custom".to_string(), Value::NativeFunction(cdn_custom));
    namespace.insert("style_link".to_string(), Value::NativeFunction(style_link));
    namespace.insert("script_link".to_string(), Value::NativeFunction(script_link));

    // HTML structure functions
    namespace.insert("html".to_string(), Value::NativeFunction(create_html));
    namespace.insert("head".to_string(), Value::NativeFunction(create_head));
    namespace.insert("body".to_string(), Value::NativeFunction(create_body));
    namespace.insert("title".to_string(), Value::NativeFunction(create_title));
    namespace.insert("meta".to_string(), Value::NativeFunction(create_meta));
    namespace.insert("style".to_string(), Value::NativeFunction(create_style));
    namespace.insert("script".to_string(), Value::NativeFunction(create_script));

    // Window class
    namespace.insert("Window".to_string(), Value::NativeFunction(create_window_class));

    // Utility functions
    namespace.insert("render".to_string(), Value::NativeFunction(render_html));
    namespace.insert("escape_html".to_string(), Value::NativeFunction(escape_html_func));

    Value::Module("webviewtk".to_string(), namespace)
}

// Helper function to build HTML element with attributes
fn build_element(
    tag: &str,
    content: &str,
    classes: Option<&str>,
    id: Option<&str>,
    style: Option<&str>,
    attrs: Option<&HashMap<String, String>>,
    self_closing: bool,
) -> String {
    let mut html = format!("<{}", tag);

    // Add id attribute
    if let Some(id_val) = id {
        if !id_val.is_empty() {
            html.push_str(&format!(" id=\"{}\"", escape_html(id_val)));
        }
    }

    // Add class attribute
    if let Some(class_val) = classes {
        if !class_val.is_empty() {
            html.push_str(&format!(" class=\"{}\"", escape_html(class_val)));
        }
    }

    // Add style attribute
    if let Some(style_val) = style {
        if !style_val.is_empty() {
            html.push_str(&format!(" style=\"{}\"", escape_html(style_val)));
        }
    }

    // Add custom attributes
    if let Some(attrs_map) = attrs {
        for (key, val) in attrs_map {
            html.push_str(&format!(" {}=\"{}\"", escape_html(key), escape_html(val)));
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

// Helper function to escape HTML
fn escape_html(text: &str) -> String {
    text.replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
        .replace('"', "&quot;")
        .replace('\'', "&#39;")
}

// Helper to extract arguments from Value
fn extract_string_arg(args: &[Value], index: usize) -> Option<String> {
    if index < args.len() {
        match &args[index] {
            Value::Str(s) => Some(s.clone()),
            _ => None,
        }
    } else {
        None
    }
}

fn extract_dict_arg(args: &[Value], index: usize) -> Option<HashMap<String, String>> {
    if index < args.len() {
        match &args[index] {
            Value::Dict(dict) => {
                let mut map = HashMap::new();
                for (key, val) in dict.borrow().iter() {
                    // Keys are already Strings, values are Value enums
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

// Widget creation functions

/// Create a div element
/// Args: content="", classes="", id="", style="", attrs={}
fn create_div(args: Vec<Value>) -> Result<Value> {
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
/// Args: text="", classes="", id="", style="", attrs={}
fn create_button(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_else(|| "Button".to_string());
    let classes = extract_string_arg(&args, 1);
    let id = extract_string_arg(&args, 2);
    let style = extract_string_arg(&args, 3);
    let attrs = extract_dict_arg(&args, 4);

    let html = build_element(
        "button",
        &escape_html(&text),
        classes.as_deref(),
        id.as_deref(),
        style.as_deref(),
        attrs.as_ref(),
        false,
    );

    Ok(Value::Str(html))
}

/// Create a paragraph element
/// Args: text="", classes="", id="", style="", attrs={}
fn create_paragraph(args: Vec<Value>) -> Result<Value> {
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
fn create_heading(args: Vec<Value>) -> Result<Value> {
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
        fn $fn_name(args: Vec<Value>) -> Result<Value> {
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
fn create_span(args: Vec<Value>) -> Result<Value> {
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
/// Args: input_type="text", placeholder="", classes="", id="", style="", attrs={}
fn create_input(args: Vec<Value>) -> Result<Value> {
    let input_type = extract_string_arg(&args, 0).unwrap_or_else(|| "text".to_string());
    let placeholder = extract_string_arg(&args, 1);
    let classes = extract_string_arg(&args, 2);
    let id = extract_string_arg(&args, 3);
    let style = extract_string_arg(&args, 4);
    let mut attrs = extract_dict_arg(&args, 5).unwrap_or_default();

    attrs.insert("type".to_string(), input_type);
    if let Some(ph) = placeholder {
        attrs.insert("placeholder".to_string(), ph);
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
fn create_textarea(args: Vec<Value>) -> Result<Value> {
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
fn create_label(args: Vec<Value>) -> Result<Value> {
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
fn create_form(args: Vec<Value>) -> Result<Value> {
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
fn create_img(args: Vec<Value>) -> Result<Value> {
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
fn create_link(args: Vec<Value>) -> Result<Value> {
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
        fn $fn_name(args: Vec<Value>) -> Result<Value> {
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
fn create_select(args: Vec<Value>) -> Result<Value> {
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
fn create_option(args: Vec<Value>) -> Result<Value> {
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
fn create_checkbox(args: Vec<Value>) -> Result<Value> {
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
fn create_radio(args: Vec<Value>) -> Result<Value> {
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

// CDN Functions

/// Include Tailwind CSS via CDN
fn cdn_tailwind(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "3.3.0".to_string());
    let html = format!(
        "<script src=\"https://cdn.tailwindcss.com?v={}\"></script>",
        escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include Bootstrap via CDN
fn cdn_bootstrap(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "5.3.0".to_string());
    let html = format!(
        "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@{}/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n\
         <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@{}/dist/js/bootstrap.bundle.min.js\"></script>",
        escape_html(&version), escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include jQuery via CDN
fn cdn_jquery(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "3.7.0".to_string());
    let html = format!(
        "<script src=\"https://code.jquery.com/jquery-{}.min.js\"></script>",
        escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include Vue.js via CDN
fn cdn_vue(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "3.3.4".to_string());
    let html = format!(
        "<script src=\"https://unpkg.com/vue@{}/dist/vue.global.js\"></script>",
        escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include React via CDN
fn cdn_react(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "18.2.0".to_string());
    let html = format!(
        "<script crossorigin src=\"https://unpkg.com/react@{}/umd/react.production.min.js\"></script>\n\
         <script crossorigin src=\"https://unpkg.com/react-dom@{}/umd/react-dom.production.min.js\"></script>",
        escape_html(&version), escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include Alpine.js via CDN
fn cdn_alpine(args: Vec<Value>) -> Result<Value> {
    let version = extract_string_arg(&args, 0).unwrap_or_else(|| "3.x.x".to_string());
    let html = format!(
        "<script defer src=\"https://cdn.jsdelivr.net/npm/alpinejs@{}/dist/cdn.min.js\"></script>",
        escape_html(&version)
    );
    Ok(Value::Str(html))
}

/// Include custom CDN link
/// Args: url, type="script" (can be "script" or "style")
fn cdn_custom(args: Vec<Value>) -> Result<Value> {
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
fn style_link(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("style_link() requires a URL argument"));
    }

    let url = extract_string_arg(&args, 0).unwrap_or_default();
    let html = format!("<link rel=\"stylesheet\" href=\"{}\" />", escape_html(&url));
    Ok(Value::Str(html))
}

/// Create a script link
fn script_link(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("script_link() requires a URL argument"));
    }

    let url = extract_string_arg(&args, 0).unwrap_or_default();
    let html = format!("<script src=\"{}\"></script>", escape_html(&url));
    Ok(Value::Str(html))
}

// HTML structure functions

/// Create an html element
fn create_html(args: Vec<Value>) -> Result<Value> {
    let content = extract_string_arg(&args, 0).unwrap_or_default();
    let lang = extract_string_arg(&args, 1).unwrap_or_else(|| "en".to_string());
    let mut attrs = HashMap::new();
    attrs.insert("lang".to_string(), lang);

    let html = build_element("html", &content, None, None, None, Some(&attrs), false);
    Ok(Value::Str(html))
}

/// Create a head element
fn create_head(args: Vec<Value>) -> Result<Value> {
    let content = extract_string_arg(&args, 0).unwrap_or_default();
    let html = build_element("head", &content, None, None, None, None, false);
    Ok(Value::Str(html))
}

/// Create a body element
fn create_body(args: Vec<Value>) -> Result<Value> {
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
fn create_title(args: Vec<Value>) -> Result<Value> {
    let text = extract_string_arg(&args, 0).unwrap_or_else(|| "Tauraro App".to_string());
    let html = format!("<title>{}</title>", escape_html(&text));
    Ok(Value::Str(html))
}

/// Create a meta element
fn create_meta(args: Vec<Value>) -> Result<Value> {
    let attrs = extract_dict_arg(&args, 0).unwrap_or_default();
    let html = build_element("meta", "", None, None, None, Some(&attrs), true);
    Ok(Value::Str(html))
}

/// Create a style element
fn create_style(args: Vec<Value>) -> Result<Value> {
    let css = extract_string_arg(&args, 0).unwrap_or_default();
    let html = format!("<style>{}</style>", css);
    Ok(Value::Str(html))
}

/// Create a script element
fn create_script(args: Vec<Value>) -> Result<Value> {
    let js = extract_string_arg(&args, 0).unwrap_or_default();
    let html = format!("<script>{}</script>", js);
    Ok(Value::Str(html))
}

// Window class

/// Create a Window class instance
fn create_window_class(args: Vec<Value>) -> Result<Value> {
    let title = extract_string_arg(&args, 0).unwrap_or_else(|| "Tauraro App".to_string());
    let width = if args.len() > 1 {
        match &args[1] {
            Value::Int(n) => *n as u32,
            _ => 800,
        }
    } else {
        800
    };
    let height = if args.len() > 2 {
        match &args[2] {
            Value::Int(n) => *n as u32,
            _ => 600,
        }
    } else {
        600
    };

    // Create window object
    let mut window_obj = HashMap::new();
    window_obj.insert("title".to_string(), Value::Str(title));
    window_obj.insert("width".to_string(), Value::Int(width as i64));
    window_obj.insert("height".to_string(), Value::Int(height as i64));
    window_obj.insert("html".to_string(), Value::Str(String::new()));

    // Add methods
    window_obj.insert("set_html".to_string(), Value::NativeFunction(window_set_html));
    window_obj.insert("run".to_string(), Value::NativeFunction(window_run));
    window_obj.insert("run_async".to_string(), Value::NativeFunction(window_run_async));

    Ok(Value::Dict(Rc::new(RefCell::new(window_obj))))
}

/// Set HTML content for the window
fn window_set_html(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("set_html() requires self and html arguments"));
    }

    let html = extract_string_arg(&args, 1).unwrap_or_default();

    // Update the window object's html field
    if let Value::Dict(dict) = &args[0] {
        dict.borrow_mut().insert("html".to_string(), Value::Str(html));
    }

    Ok(Value::None)
}

/// Run the window event loop (blocking - waits until window closes)
fn window_run(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "webviewtk"))]
    {
        return Err(anyhow::anyhow!(
            "WebViewTK feature is not enabled. Please compile with --features webviewtk"
        ));
    }

    #[cfg(feature = "webviewtk")]
    {
        if args.is_empty() {
            return Err(anyhow::anyhow!("run() requires self argument"));
        }

        let (title, width, height, html) = if let Value::Dict(dict) = &args[0] {
            let d = dict.borrow();
            let title = match d.get("title") {
                Some(Value::Str(s)) => s.clone(),
                _ => "Tauraro App".to_string(),
            };
            let width = match d.get("width") {
                Some(Value::Int(n)) => *n as u32,
                _ => 800,
            };
            let height = match d.get("height") {
                Some(Value::Int(n)) => *n as u32,
                _ => 600,
            };
            let html = match d.get("html") {
                Some(Value::Str(s)) => s.clone(),
                _ => String::new(),
            };
            (title, width, height, html)
        } else {
            return Err(anyhow::anyhow!("run() requires a Window object"));
        };

        // Validate HTML is not empty
        if html.is_empty() {
            return Err(anyhow::anyhow!("Window HTML content is empty. Call set_html() first."));
        }

        // Spawn window in a separate thread to allow multiple processes
        let handle = std::thread::spawn(move || {
            // Create event loop - use platform-specific API for non-main threads
            #[cfg(target_os = "windows")]
            let event_loop = EventLoop::<()>::new_any_thread();

            #[cfg(not(target_os = "windows"))]
            let event_loop = EventLoop::<()>::new();

            let window = WindowBuilder::new()
                .with_title(&title)
                .with_inner_size(tao::dpi::LogicalSize::new(width, height))
                .build(&event_loop)
                .expect("Failed to create window");

            // Create webview
            let _webview = WebViewBuilder::new(window)
                .expect("Failed to create webview")
                .with_html(&html)
                .expect("Failed to set HTML")
                .build()
                .expect("Failed to build webview");

            eprintln!("[DEBUG] Window created, starting event loop...");

            // Run event loop - this blocks until window is closed
            event_loop.run(move |event, _, control_flow| {
                *control_flow = ControlFlow::Wait;

                match event {
                    Event::WindowEvent {
                        event: WindowEvent::CloseRequested,
                        ..
                    } => {
                        eprintln!("[DEBUG] Window close requested");
                        *control_flow = ControlFlow::Exit;
                    }
                    _ => {}
                }
            });

            eprintln!("[DEBUG] Event loop exited");
        });

        // Block until window closes - this allows multiple processes to run simultaneously
        // Each process has its own thread for the window event loop
        match handle.join() {
            Ok(_) => {
                // Window closed normally
            }
            Err(e) => {
                eprintln!("Error: Window thread panicked: {:?}", e);
                return Err(anyhow::anyhow!("Window thread panicked"));
            }
        }

        Ok(Value::None)
    }
}

/// Run the window event loop (non-blocking - returns immediately)
/// Use this for multiple windows in one program
fn window_run_async(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "webviewtk"))]
    {
        return Err(anyhow::anyhow!(
            "WebViewTK feature is not enabled. Please compile with --features webviewtk"
        ));
    }

    #[cfg(feature = "webviewtk")]
    {
        if args.is_empty() {
            return Err(anyhow::anyhow!("run_async() requires self argument"));
        }

        let (title, width, height, html) = if let Value::Dict(dict) = &args[0] {
            let d = dict.borrow();
            let title = match d.get("title") {
                Some(Value::Str(s)) => s.clone(),
                _ => "Tauraro App".to_string(),
            };
            let width = match d.get("width") {
                Some(Value::Int(n)) => *n as u32,
                _ => 800,
            };
            let height = match d.get("height") {
                Some(Value::Int(n)) => *n as u32,
                _ => 600,
            };
            let html = match d.get("html") {
                Some(Value::Str(s)) => s.clone(),
                _ => String::new(),
            };
            (title, width, height, html)
        } else {
            return Err(anyhow::anyhow!("run_async() requires a Window object"));
        };

        // Validate HTML is not empty
        if html.is_empty() {
            return Err(anyhow::anyhow!("Window HTML content is empty. Call set_html() first."));
        }

        // Spawn window in a separate thread - don't wait for it (non-blocking)
        std::thread::spawn(move || {
            // Create event loop - use platform-specific API for non-main threads
            #[cfg(target_os = "windows")]
            let event_loop = EventLoop::<()>::new_any_thread();

            #[cfg(not(target_os = "windows"))]
            let event_loop = EventLoop::<()>::new();

            let window = WindowBuilder::new()
                .with_title(&title)
                .with_inner_size(tao::dpi::LogicalSize::new(width, height))
                .build(&event_loop)
                .expect("Failed to create window");

            // Create webview
            let _webview = WebViewBuilder::new(window)
                .expect("Failed to create webview")
                .with_html(&html)
                .expect("Failed to set HTML")
                .build()
                .expect("Failed to build webview");

            // Run event loop - this blocks until window is closed
            event_loop.run(move |event, _, control_flow| {
                *control_flow = ControlFlow::Wait;

                match event {
                    Event::WindowEvent {
                        event: WindowEvent::CloseRequested,
                        ..
                    } => *control_flow = ControlFlow::Exit,
                    _ => {}
                }
            });
        });

        // Small delay to ensure window thread starts
        std::thread::sleep(std::time::Duration::from_millis(100));

        Ok(Value::None)
    }
}

/// Render HTML string (utility function)
fn render_html(args: Vec<Value>) -> Result<Value> {
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
fn escape_html_func(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("escape_html() requires a string argument"));
    }

    let text = extract_string_arg(&args, 0).unwrap_or_default();
    Ok(Value::Str(escape_html(&text)))
}
