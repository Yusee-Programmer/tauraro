/// WebViewTK - Flutter-style UI framework for Tauraro
/// Complete rewrite with proper Flutter architecture

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

// Core modules
pub mod widgets;
pub mod window;
pub mod rendering;
pub mod utils;
pub mod cdn;
pub mod command_registry;

#[cfg(feature = "webviewtk")]
pub mod ipc;

// Re-exports
pub use widgets::*;
pub use window::Window;

/// Create the webviewtk module for Tauraro VM
pub fn create_webviewtk_module() -> Value {
    let mut namespace = HashMap::new();

    // Window class and functions
    namespace.insert("Window".to_string(), Value::NativeFunction(window::create));
    namespace.insert("mount_and_run".to_string(), Value::NativeFunction(window::mount_and_run_wrapper));

    #[cfg(feature = "webviewtk")]
    {
        // IPC functions
        namespace.insert("ipc_call".to_string(), Value::NativeFunction(ipc::ipc_call_handler));
        namespace.insert("ipc_register".to_string(), Value::NativeFunction(ipc::ipc_register_handler));

        // Basic widgets
        namespace.insert("Text".to_string(), Value::NativeFunction(widgets::text::create));
        namespace.insert("Container".to_string(), Value::NativeFunction(widgets::container::create));
        namespace.insert("Center".to_string(), Value::NativeFunction(widgets::center::create));
        namespace.insert("Padding".to_string(), Value::NativeFunction(widgets::padding::create));
        namespace.insert("SizedBox".to_string(), Value::NativeFunction(widgets::sized_box::create));
        
        // Layout widgets
        namespace.insert("Column".to_string(), Value::NativeFunction(widgets::column::create));
        namespace.insert("Row".to_string(), Value::NativeFunction(widgets::row::create));
        namespace.insert("Stack".to_string(), Value::NativeFunction(widgets::stack::create));
        namespace.insert("Expanded".to_string(), Value::NativeFunction(widgets::expanded::create));
        namespace.insert("Flexible".to_string(), Value::NativeFunction(widgets::flexible::create));
        namespace.insert("Spacer".to_string(), Value::NativeFunction(widgets::spacer::create));
        namespace.insert("Positioned".to_string(), Value::NativeFunction(widgets::positioned::create));
        
        // Material widgets
        namespace.insert("Button".to_string(), Value::NativeFunction(widgets::button::create));
        namespace.insert("TextField".to_string(), Value::NativeFunction(widgets::textfield::create));
        namespace.insert("Card".to_string(), Value::NativeFunction(widgets::card::create));
        namespace.insert("Scaffold".to_string(), Value::NativeFunction(widgets::scaffold::create));
        namespace.insert("AppBar".to_string(), Value::NativeFunction(widgets::appbar::create));
        namespace.insert("FloatingActionButton".to_string(), Value::NativeFunction(widgets::fab::create));
        namespace.insert("Divider".to_string(), Value::NativeFunction(widgets::divider::create));
        namespace.insert("ListTile".to_string(), Value::NativeFunction(widgets::list_tile::create));
        namespace.insert("CustomTitleBar".to_string(), Value::NativeFunction(widgets::custom_titlebar::create));
        
        // Gesture widgets
        namespace.insert("GestureDetector".to_string(), Value::NativeFunction(widgets::gesture_detector::create));
        namespace.insert("InkWell".to_string(), Value::NativeFunction(widgets::ink_well::create));
        
        // EdgeInsets helper
        let mut edge_insets = HashMap::new();
        edge_insets.insert("all".to_string(), Value::NativeFunction(utils::edgeinsets_all));
        edge_insets.insert("symmetric".to_string(), Value::NativeFunction(utils::edgeinsets_symmetric));
        edge_insets.insert("only".to_string(), Value::NativeFunction(utils::edgeinsets_only));
        edge_insets.insert("zero".to_string(), Value::NativeFunction(utils::edgeinsets_zero));
        namespace.insert("EdgeInsets".to_string(), Value::Dict(Rc::new(RefCell::new(edge_insets))));
        
        // CDN Management
        let cdns = cdn::CDN::all_cdns();
        let mut cdn_dict = HashMap::new();
        for (name, url) in cdns {
            cdn_dict.insert(name, Value::Str(url));
        }
        namespace.insert("CDN".to_string(), Value::Dict(Rc::new(RefCell::new(cdn_dict))));
        
        // Resource loading functions
        namespace.insert("include_cdn".to_string(), Value::NativeFunction(window::include_cdn_wrapper));
        namespace.insert("include_css_file".to_string(), Value::NativeFunction(window::include_css_file_wrapper));
        namespace.insert("include_js_file".to_string(), Value::NativeFunction(window::include_js_file_wrapper));
        namespace.insert("include_html_file".to_string(), Value::NativeFunction(window::include_html_file_wrapper));
        namespace.insert("add_custom_css".to_string(), Value::NativeFunction(window::add_custom_css_wrapper));
        namespace.insert("add_custom_js".to_string(), Value::NativeFunction(window::add_custom_js_wrapper));
        
        // Command registration
        namespace.insert("register_command".to_string(), Value::NativeFunction(window::register_command_wrapper));
    }

    Value::Module("webviewtk".to_string(), namespace)
}
