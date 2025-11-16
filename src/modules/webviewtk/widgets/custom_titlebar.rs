use crate::value::Value;
use anyhow::Result;
use crate::modules::webviewtk::rendering::RenderObject;
use crate::modules::webviewtk::rendering::{get_string, get_float, get_bool};
use crate::modules::webviewtk::utils::extract_kwargs;
use std::rc::Rc;
use std::cell::RefCell;
use std::collections::HashMap;

/// CustomTitleBar widget - creates a custom window titlebar with drag region and window controls
#[derive(Clone, Debug)]
pub struct CustomTitleBar {
    pub title: String,
    pub height: f64,
    pub background_color: String,
    pub text_color: String,
    pub show_minimize: bool,
    pub show_maximize: bool,
    pub show_close: bool,
    pub child: Option<Box<Value>>,
}

impl CustomTitleBar {
    pub fn render(&self) -> RenderObject {
        let mut obj = RenderObject::new("div");
        
        // Create titlebar HTML with drag region and controls
        let minimize_btn = if self.show_minimize {
            r#"<button class="titlebar-btn minimize-btn" onclick="window.ipc.postMessage('window:minimize')">−</button>"#
        } else {
            ""
        };
        
        let maximize_btn = if self.show_maximize {
            r#"<button class="titlebar-btn maximize-btn" onclick="window.ipc.postMessage('window:maximize')">□</button>"#
        } else {
            ""
        };
        
        let close_btn = if self.show_close {
            r#"<button class="titlebar-btn close-btn" onclick="window.ipc.postMessage('window:close')">×</button>"#
        } else {
            ""
        };
        
        let titlebar_html = format!(
            r#"<div class="titlebar-content titlebar-drag-region" style="height: {}px !important; background-color: {} !important; color: {} !important; display: flex !important; align-items: center !important; justify-content: space-between !important; padding: 0 16px !important; -webkit-app-region: drag; user-select: none; position: fixed; top: 0; left: 0; right: 0; z-index: 9999; cursor: default;">
                <div class="titlebar-left" style="display: flex; align-items: center; gap: 12px; -webkit-app-region: drag; cursor: default;">
                    <span class="titlebar-title" style="font-weight: 500; color: {} !important; cursor: default;">{}</span>
                </div>
                <div class="titlebar-controls" style="-webkit-app-region: no-drag; display: flex; gap: 0px; align-items: center; height: 100%; cursor: default;">
                    {}{}{}
                </div>
            </div>"#,
            self.height,
            self.background_color,
            self.text_color,
            self.text_color,
            self.title,
            minimize_btn,
            maximize_btn,
            close_btn
        );
        
        // Add inline CSS for titlebar buttons with proper color inheritance and drag script
        let titlebar_css = format!(r#"
<style>
.webviewtk-custom-titlebar {{
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    z-index: 9999;
}}

.titlebar-content {{
    background-color: {} !important;
    color: {} !important;
}}

.titlebar-btn {{
    width: 46px;
    height: {}px;
    border: none;
    background: transparent;
    color: {} !important;
    font-size: 20px;
    font-weight: 300;
    cursor: pointer;
    transition: background-color 0.2s;
    -webkit-app-region: no-drag;
    display: flex;
    align-items: center;
    justify-content: center;
}}

.titlebar-btn:hover {{
    background-color: rgba(255, 255, 255, 0.1) !important;
}}

.close-btn:hover {{
    background-color: #e81123 !important;
    color: white !important;
}}

.titlebar-btn:active {{
    background-color: rgba(255, 255, 255, 0.2) !important;
}}
</style>
<script>
(function() {{
    // Make titlebar draggable via IPC
    document.addEventListener('DOMContentLoaded', function() {{
        const dragRegion = document.querySelector('.titlebar-drag-region');
        if (!dragRegion) return;
        
        let isDragging = false;
        let startX, startY;
        
        dragRegion.addEventListener('mousedown', function(e) {{
            // Don't drag if clicking on buttons
            if (e.target.closest('.titlebar-btn')) return;
            
            isDragging = true;
            startX = e.clientX;
            startY = e.clientY;
            
            // Send drag start message
            if (window.ipc) {{
                window.ipc.postMessage('window:drag_start:' + startX + ':' + startY);
            }}
        }});
        
        document.addEventListener('mousemove', function(e) {{
            if (!isDragging) return;
            
            const deltaX = e.clientX - startX;
            const deltaY = e.clientY - startY;
            
            // Send drag move message
            if (window.ipc) {{
                window.ipc.postMessage('window:drag_move:' + deltaX + ':' + deltaY);
            }}
        }});
        
        document.addEventListener('mouseup', function() {{
            if (isDragging) {{
                isDragging = false;
                // Send drag end message
                if (window.ipc) {{
                    window.ipc.postMessage('window:drag_end');
                }}
            }}
        }});
    }});
}})();
</script>
"#, self.background_color, self.text_color, self.height, self.text_color);
        
        obj.raw_html = Some(format!("{}{}", titlebar_css, titlebar_html));
        
        obj
    }
}

pub fn create(args: Vec<Value>) -> Result<Value> {
    fn extract_kwargs(args: &[Value]) -> Option<HashMap<String, Value>> {
        for arg in args {
            match arg {
                Value::Dict(dict) => return Some(dict.borrow().clone()),
                Value::KwargsMarker(map) => return Some(map.clone()),
                _ => {}
            }
        }
        None
    }
    
    fn get_string(value: &Value, default: &str) -> String {
        match value {
            Value::Str(s) => s.clone(),
            _ => default.to_string(),
        }
    }
    
    fn get_float(value: &Value, default: f64) -> f64 {
        match value {
            Value::Float(f) => *f,
            Value::Int(i) => *i as f64,
            _ => default,
        }
    }
    
    fn get_bool(value: &Value, default: bool) -> bool {
        match value {
            Value::Bool(b) => *b,
            _ => default,
        }
    }
    
    let mut title = String::new();
    let mut height = 40.0;
    let mut background_color = "#1e1e1e".to_string();
    let mut text_color = "#ffffff".to_string();
    let mut show_minimize = true;
    let mut show_maximize = true;
    let mut show_close = true;
    let mut child: Option<Box<Value>> = None;
    
    if let Some(kwargs) = extract_kwargs(&args) {
        if let Some(val) = kwargs.get("title") {
            title = get_string(val, "");
        }
        if let Some(val) = kwargs.get("height") {
            height = get_float(val, 40.0);
        }
        if let Some(val) = kwargs.get("background_color") {
            background_color = get_string(val, "#1e1e1e");
        }
        if let Some(val) = kwargs.get("text_color") {
            text_color = get_string(val, "#ffffff");
        }
        if let Some(val) = kwargs.get("show_minimize") {
            show_minimize = get_bool(val, true);
        }
        if let Some(val) = kwargs.get("show_maximize") {
            show_maximize = get_bool(val, true);
        }
        if let Some(val) = kwargs.get("show_close") {
            show_close = get_bool(val, true);
        }
        if let Some(val) = kwargs.get("child") {
            child = Some(Box::new(val.clone()));
        }
    }
    
    let titlebar = CustomTitleBar {
        title,
        height,
        background_color,
        text_color,
        show_minimize,
        show_maximize,
        show_close,
        child,
    };
    
    // Render the titlebar to HTML
    let render_obj = titlebar.render();
    let html = render_obj.to_html();
    
    let mut dict = HashMap::new();
    dict.insert("_widget_type".to_string(), Value::Str("CustomTitleBar".to_string()));
    dict.insert("_widget_id".to_string(), Value::Str(crate::modules::webviewtk::utils::generate_widget_id()));
    dict.insert("_html".to_string(), Value::Str(html));
    
    Ok(Value::Dict(Rc::new(RefCell::new(dict))))
}

pub fn render_value(value: &Value) -> Result<RenderObject> {
    // This is a placeholder - actual rendering will be done in the mount_and_run flow
    Ok(RenderObject::new("div"))
}
