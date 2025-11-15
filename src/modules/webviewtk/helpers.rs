/// Helper functions for common UI patterns
/// Simplifies creating standard components like window controls, menu bars, etc.

use crate::value::Value;
use crate::modules::hplist::HPList;
use anyhow::Result;
use std::rc::Rc;
use std::cell::RefCell;

/// Create window control buttons (minimize, maximize/restore, close)
/// Args: theme="light" (light or dark), classes=""
/// Returns HTML string with styled control buttons
pub fn create_window_controls(args: Vec<Value>) -> Result<Value> {
    let theme = if args.len() > 0 {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => "light".to_string(),
        }
    } else {
        "light".to_string()
    };
    
    let additional_classes = if args.len() > 1 {
        match &args[1] {
            Value::Str(s) => format!(" {}", s),
            _ => String::new(),
        }
    } else {
        String::new()
    };

    let bg_color = if theme == "dark" { "#1e1e1e" } else { "transparent" };
    let text_color = if theme == "dark" { "#ffffff" } else { "#000000" };
    let button_hover = if theme == "dark" { "#3a3a3a" } else { "#e0e0e0" };
    
    let html = format!(r#"<div class="window-controls titlebar-no-drag{}" style="display: flex; gap: 0;">
    <button onclick="window.tauraro.minimize()" class="titlebar-button" style="
        background: {};
        border: none;
        width: 46px;
        height: 32px;
        display: flex;
        align-items: center;
        justify-content: center;
        color: {};
        cursor: pointer;
        font-size: 14px;
        transition: background-color 0.2s;
    " onmouseover="this.style.backgroundColor='{}'" onmouseout="this.style.backgroundColor='{}'">
        <span style="display: inline-block; width: 10px; height: 1px; background: currentColor;"></span>
    </button>
    <button onclick="window.tauraro.maximize()" class="titlebar-button" style="
        background: {};
        border: none;
        width: 46px;
        height: 32px;
        display: flex;
        align-items: center;
        justify-content: center;
        color: {};
        cursor: pointer;
        font-size: 14px;
        transition: background-color 0.2s;
    " onmouseover="this.style.backgroundColor='{}'" onmouseout="this.style.backgroundColor='{}'">
        <span style="display: inline-block; width: 9px; height: 9px; border: 1px solid currentColor;"></span>
    </button>
    <button onclick="window.tauraro.close()" class="titlebar-button" style="
        background: {};
        border: none;
        width: 46px;
        height: 32px;
        display: flex;
        align-items: center;
        justify-content: center;
        color: {};
        cursor: pointer;
        font-size: 14px;
        transition: background-color 0.2s;
    " onmouseover="this.style.backgroundColor='#e81123'; this.style.color='white';" onmouseout="this.style.backgroundColor='{}'; this.style.color='{}';">
        <span style="display: inline-block; font-size: 12px;">✕</span>
    </button>
</div>"#, 
        additional_classes, bg_color, text_color, button_hover, bg_color,
        bg_color, text_color, button_hover, bg_color,
        bg_color, text_color, bg_color, text_color
    );

    Ok(Value::Str(html))
}

/// Create a menu bar with buttons
/// Args: menu_items=["File", "Edit", "View", "Help"], theme="light", classes=""
/// Menu items can be strings or dicts with {label: "File", event: "file_menu"}
pub fn create_menu_bar(args: Vec<Value>) -> Result<Value> {
    let menu_items = if args.len() > 0 {
        match &args[0] {
            Value::List(items) => items.clone(),
            _ => HPList { data: Rc::new(RefCell::new(vec![])) },
        }
    } else {
        HPList { data: Rc::new(RefCell::new(vec![])) }
    };

    let theme = if args.len() > 1 {
        match &args[1] {
            Value::Str(s) => s.clone(),
            _ => "light".to_string(),
        }
    } else {
        "light".to_string()
    };

    let text_color = if theme == "dark" { "#ffffff" } else { "#333333" };
    let button_hover = if theme == "dark" { "#3a3a3a" } else { "#e0e0e0" };

    let mut buttons_html = String::new();
    for item in menu_items.data.borrow().iter() {
        let (label, event) = match item {
            Value::Str(s) => (s.clone(), format!("menu_{}", s.to_lowercase())),
            Value::Dict(d) => {
                let borrowed = d.borrow();
                let label = borrowed.get("label")
                    .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
                    .unwrap_or_else(|| "Menu".to_string());
                let event = borrowed.get("event")
                    .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
                    .unwrap_or_else(|| format!("menu_{}", label.to_lowercase()));
                (label, event)
            },
            _ => continue,
        };

        buttons_html.push_str(&format!(r#"
        <button onclick="window.ipc.postMessage(JSON.stringify({{cmd: '{}'}}))" class="menu-btn" style="
            background: transparent;
            border: none;
            padding: 4px 12px;
            color: {};
            font-size: 12px;
            cursor: pointer;
            border-radius: 3px;
            transition: background-color 0.2s;
        " onmouseover="this.style.backgroundColor='{}'" onmouseout="this.style.backgroundColor='transparent'">
            {}
        </button>"#, event, text_color, button_hover, label));
    }

    let html = format!(r#"<div class="menu-bar titlebar-no-drag" style="display: flex; gap: 4px; margin-right: 20px; -webkit-app-region: no-drag;">
    {}
</div>"#, buttons_html);

    Ok(Value::Str(html))
}

/// Create a search bar
/// Args: placeholder="Search...", event_handler="search", classes=""
pub fn create_search_bar(args: Vec<Value>) -> Result<Value> {
    let placeholder = if args.len() > 0 {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => "Search...".to_string(),
        }
    } else {
        "Search...".to_string()
    };

    let event_handler = if args.len() > 1 {
        match &args[1] {
            Value::Str(s) => s.clone(),
            _ => "search".to_string(),
        }
    } else {
        "search".to_string()
    };

    let html = format!(r#"<div class="search-container titlebar-no-drag" style="
    display: flex;
    align-items: center;
    background: rgba(0,0,0,0.1);
    border-radius: 4px;
    padding: 4px 8px;
    margin-right: 20px;
    -webkit-app-region: no-drag;
">
    <span style="margin-right: 6px; opacity: 0.7;">🔍</span>
    <input type="search" placeholder="{}" 
        oninput="window.ipc.postMessage(JSON.stringify({{cmd: '{}', value: this.value}}))"
        style="
            background: transparent;
            border: none;
            outline: none;
            color: inherit;
            font-size: 12px;
            width: 150px;
        ">
</div>"#, placeholder, event_handler);

    Ok(Value::Str(html))
}

/// Create a complete custom titlebar with all common elements
/// Args: title="App", show_menu=true, show_search=true, menu_items=["File","Edit","View","Help"], theme="light"
pub fn create_titlebar(args: Vec<Value>) -> Result<Value> {
    let title = if args.len() > 0 {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => "App".to_string(),
        }
    } else {
        "App".to_string()
    };

    let show_menu = if args.len() > 1 {
        match &args[1] {
            Value::Bool(b) => *b,
            _ => true,
        }
    } else {
        true
    };

    let show_search = if args.len() > 2 {
        match &args[2] {
            Value::Bool(b) => *b,
            _ => true,
        }
    } else {
        true
    };

    let menu_items = if args.len() > 3 {
        match &args[3] {
            Value::List(items) => items.clone(),
            _ => HPList { data: Rc::new(RefCell::new(vec![
                Value::Str("File".to_string()), Value::Str("Edit".to_string()), 
                Value::Str("View".to_string()), Value::Str("Help".to_string())
            ])) },
        }
    } else {
        HPList { data: Rc::new(RefCell::new(vec![
            Value::Str("File".to_string()), Value::Str("Edit".to_string()), 
            Value::Str("View".to_string()), Value::Str("Help".to_string())
        ])) }
    };

    let theme = if args.len() > 4 {
        match &args[4] {
            Value::Str(s) => s.clone(),
            _ => "light".to_string(),
        }
    } else {
        "light".to_string()
    };

    let bg_color = if theme == "dark" { "#1e1e1e" } else { "#f0f0f0" };
    let text_color = if theme == "dark" { "#ffffff" } else { "#000000" };

    let menu_html = if show_menu {
        create_menu_bar(vec![Value::List(menu_items), Value::Str(theme.clone())])?
    } else {
        Value::Str(String::new())
    };

    let search_html = if show_search {
        create_search_bar(vec![])?
    } else {
        Value::Str(String::new())
    };

    let controls_html = create_window_controls(vec![Value::Str(theme.clone())])?;

    let menu_str = if let Value::Str(s) = menu_html { s } else { String::new() };
    let search_str = if let Value::Str(s) = search_html { s } else { String::new() };
    let controls_str = if let Value::Str(s) = controls_html { s } else { String::new() };

    let html = format!(r#"<div class="titlebar-drag" style="
    display: flex;
    justify-content: space-between;
    align-items: center;
    height: 40px;
    background-color: {};
    color: {};
    padding: 0 10px;
    user-select: none;
    -webkit-user-select: none;
    -webkit-app-region: drag;
    app-region: drag;
">
    <div style="display: flex; align-items: center; flex-grow: 1;">
        <span style="font-size: 12px; font-weight: 600; margin-right: 20px;">{}</span>
        {}
        {}
    </div>
    {}
</div>"#, bg_color, text_color, title, menu_str, search_str, controls_str);

    Ok(Value::Str(html))
}
