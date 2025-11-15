/// Menu system for WebViewTK
/// Provides native menu creation and management functions

use crate::value::Value;
use crate::modules::hplist::HPList;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

/// Create a menu structure
/// Args: label="", items=[]
pub fn create_menu(args: Vec<Value>) -> Result<Value> {
    let label = extract_string_arg(&args, 0).unwrap_or_default();
    
    let mut menu_obj = HashMap::new();
    menu_obj.insert("label".to_string(), Value::Str(label));
    menu_obj.insert("items".to_string(), Value::List(HPList::new()));
    menu_obj.insert("type".to_string(), Value::Str("menu".to_string()));

    Ok(Value::Dict(Rc::new(RefCell::new(menu_obj))))
}

/// Create a menu item
/// Args: label="", action="", icon="", shortcut="", submenu=None
pub fn create_menu_item(args: Vec<Value>) -> Result<Value> {
    let label = extract_string_arg(&args, 0).unwrap_or_default();
    let action = extract_string_arg(&args, 1).unwrap_or_default();
    let icon = extract_string_arg(&args, 2);
    let shortcut = extract_string_arg(&args, 3);

    let mut item_obj = HashMap::new();
    item_obj.insert("label".to_string(), Value::Str(label));
    item_obj.insert("action".to_string(), Value::Str(action));
    item_obj.insert("type".to_string(), Value::Str("item".to_string()));

    if let Some(icon_url) = icon {
        item_obj.insert("icon".to_string(), Value::Str(icon_url));
    }
    if let Some(shortcut_key) = shortcut {
        item_obj.insert("shortcut".to_string(), Value::Str(shortcut_key));
    }

    Ok(Value::Dict(Rc::new(RefCell::new(item_obj))))
}

/// Create a menu separator
pub fn create_menu_separator(_args: Vec<Value>) -> Result<Value> {
    let mut sep_obj = HashMap::new();
    sep_obj.insert("type".to_string(), Value::Str("separator".to_string()));

    Ok(Value::Dict(Rc::new(RefCell::new(sep_obj))))
}

/// Add an item to a menu
/// Args: menu, item
pub fn menu_add_item(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("add_item() requires menu and item arguments"));
    }

    // Get the menu dict
    if let Value::Dict(menu_dict) = &args[0] {
        let mut menu_data = menu_dict.borrow_mut();
        
        // Get or create the items list
        let items_list = menu_data
            .entry("items".to_string())
            .or_insert_with(|| Value::List(HPList::new()));
        
        // Add the item to the list
        if let Value::List(items) = items_list {
            items.push(args[1].clone());
        }
    } else {
        return Err(anyhow::anyhow!("First argument must be a menu object"));
    }

    Ok(Value::None)
}

/// Extract a string argument from a Value vector
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

/// Extract a dictionary argument from a Value vector
pub fn extract_dict_arg(args: &[Value], index: usize) -> Option<HashMap<String, String>> {
    if index < args.len() {
        match &args[index] {
            Value::Dict(d) => {
                let borrowed = d.borrow();
                let mut result = HashMap::new();
                for (k, v) in borrowed.iter() {
                    if let Value::Str(s) = v {
                        result.insert(k.clone(), s.clone());
                    }
                }
                Some(result)
            }
            _ => None,
        }
    } else {
        None
    }
}

#[cfg(target_os = "windows")]
use winapi::um::winuser::{CreateMenu, CreatePopupMenu, AppendMenuW, MF_STRING, MF_SEPARATOR, MF_POPUP};
#[cfg(target_os = "windows")]
use winapi::shared::windef::HMENU;

/// Build a native Windows HMENU from Tauraro menu objects
/// Returns (HMENU, id_to_action_map) where the map tracks menu item IDs -> action strings
#[cfg(target_os = "windows")]
pub fn build_windows_menu(
    menu_val: &Value,
    next_id: &mut u16,
    id_map: &mut HashMap<u16, String>,
) -> Option<HMENU> {
    unsafe {
        if let Value::Dict(menu_dict) = menu_val {
            let menu_data = menu_dict.borrow();
            
            // Check if this is a menu type
            if let Some(Value::Str(menu_type)) = menu_data.get("type") {
                if menu_type != "menu" {
                    return None;
                }
            } else {
                return None;
            }
            
            // Create main menu bar
            let hmenu = CreateMenu();
            if hmenu.is_null() {
                return None;
            }
            
            // Get items list
            if let Some(Value::List(items)) = menu_data.get("items") {
                for item in items.iter() {
                    if let Value::Dict(item_dict) = item {
                        let item_data = item_dict.borrow();
                        
                        if let Some(Value::Str(item_type)) = item_data.get("type") {
                            match item_type.as_str() {
                                "separator" => {
                                    AppendMenuW(hmenu, MF_SEPARATOR, 0, std::ptr::null());
                                }
                                "item" => {
                                    // Regular menu item
                                    if let Some(Value::Str(label)) = item_data.get("label") {
                                        let id = *next_id;
                                        *next_id += 1;
                                        
                                        // Store action mapping if present
                                        if let Some(Value::Str(action)) = item_data.get("action") {
                                            if !action.is_empty() {
                                                id_map.insert(id, action.clone());
                                            }
                                        }
                                        
                                        let wide = to_wide(label);
                                        AppendMenuW(hmenu, MF_STRING, id as usize, wide.as_ptr());
                                    }
                                }
                                "menu" => {
                                    // Submenu (popup)
                                    if let Some(Value::Str(label)) = item_data.get("label") {
                                        let hpopup = CreatePopupMenu();
                                        if !hpopup.is_null() {
                                            // Recursively add submenu items
                                            if let Some(Value::List(subitems)) = item_data.get("items") {
                                                for subitem in subitems.iter() {
                                                    add_menu_item_to_popup(hpopup, &subitem, next_id, id_map);
                                                }
                                            }
                                            
                                            let wide = to_wide(label);
                                            AppendMenuW(hmenu, MF_POPUP, hpopup as usize, wide.as_ptr());
                                        }
                                    }
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
            
            Some(hmenu)
        } else {
            None
        }
    }
}

#[cfg(target_os = "windows")]
unsafe fn add_menu_item_to_popup(
    hpopup: HMENU,
    item_val: &Value,
    next_id: &mut u16,
    id_map: &mut HashMap<u16, String>,
) {
    if let Value::Dict(item_dict) = item_val {
        let item_data = item_dict.borrow();
        
        if let Some(Value::Str(item_type)) = item_data.get("type") {
            match item_type.as_str() {
                "separator" => {
                    AppendMenuW(hpopup, MF_SEPARATOR, 0, std::ptr::null());
                }
                "item" => {
                    if let Some(Value::Str(label)) = item_data.get("label") {
                        let id = *next_id;
                        *next_id += 1;
                        
                        // Store action mapping
                        if let Some(Value::Str(action)) = item_data.get("action") {
                            if !action.is_empty() {
                                id_map.insert(id, action.clone());
                            }
                        }
                        
                        let wide = to_wide(label);
                        AppendMenuW(hpopup, MF_STRING, id as usize, wide.as_ptr());
                    }
                }
                "menu" => {
                    // Nested submenu
                    if let Some(Value::Str(label)) = item_data.get("label") {
                        let hsubpopup = CreatePopupMenu();
                        if !hsubpopup.is_null() {
                            if let Some(Value::List(subitems)) = item_data.get("items") {
                                for subitem in subitems.iter() {
                                    add_menu_item_to_popup(hsubpopup, &subitem, next_id, id_map);
                                }
                            }
                            let wide = to_wide(label);
                            AppendMenuW(hpopup, MF_POPUP, hsubpopup as usize, wide.as_ptr());
                        }
                    }
                }
                _ => {}
            }
        }
    }
}

#[cfg(target_os = "windows")]
fn to_wide(s: &str) -> Vec<u16> {
    use std::os::windows::ffi::OsStrExt;
    use std::ffi::OsStr;
    
    OsStr::new(s)
        .encode_wide()
        .chain(std::iter::once(0))
        .collect()
}
