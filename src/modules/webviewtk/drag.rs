/// Draggable Custom Titlebar Support
/// Provides CSS and JavaScript injection to make custom HTML titlebars draggable
/// Similar to Electron's -webkit-app-region CSS property

use crate::value::Value;
use anyhow::Result;

/// Get the CSS for making elements draggable (app-region support)
/// This mimics Electron's -webkit-app-region CSS property
pub fn get_drag_region_css() -> String {
    r#"
        /* Draggable titlebar support */
        [data-tauri-drag-region], .titlebar-drag {
            -webkit-app-region: drag;
            app-region: drag;
            user-select: none;
            -webkit-user-select: none;
            cursor: default !important;
        }
        
        [data-tauri-drag-region] button,
        [data-tauri-drag-region] a,
        [data-tauri-drag-region] input,
        [data-tauri-drag-region] select,
        .titlebar-drag button,
        .titlebar-drag a,
        .titlebar-drag input,
        .titlebar-drag select,
        .titlebar-no-drag {
            -webkit-app-region: no-drag;
            app-region: no-drag;
            cursor: default;
        }
    "#.to_string()
}

/// Get the JavaScript for window dragging support
/// This enables dragging windows via custom titlebars using mousedown/mousemove events
pub fn get_drag_region_js() -> String {
    r#"
        (function() {
            console.log('[Tauraro Drag] Initializing drag regions...');
            
            // Find all draggable elements
            function initDraggableRegions() {
                const draggableElements = document.querySelectorAll('[data-tauri-drag-region], .titlebar-drag');
                console.log('[Tauraro Drag] Found ' + draggableElements.length + ' draggable regions');
                
                draggableElements.forEach(element => {
                    element.style.cursor = 'default';
                    element.style.userSelect = 'none';
                    element.style.webkitUserSelect = 'none';
                    element.style.webkitAppRegion = 'drag';
                    element.style.appRegion = 'drag';
                    
                    // Prevent dragging on interactive elements
                    const noDragElements = element.querySelectorAll('button, a, input, select, textarea, [data-tauri-drag-no-region], .titlebar-no-drag');
                    noDragElements.forEach(child => {
                        child.style.cursor = 'default';
                        child.style.webkitAppRegion = 'no-drag';
                        child.style.appRegion = 'no-drag';
                    });
                });
                
                console.log('[Tauraro Drag] Drag regions configured with -webkit-app-region CSS');
            }
            }
            
            // Initialize on load
            if (document.readyState === 'loading') {
                document.addEventListener('DOMContentLoaded', initDraggableRegions);
            } else {
                initDraggableRegions();
            }
            
            // Re-initialize when content changes (for dynamic content)
            const observer = new MutationObserver(initDraggableRegions);
            observer.observe(document.body, { childList: true, subtree: true });
            
            console.log('[Tauraro Drag] Drag regions initialized');
        })();
    "#.to_string()
}

/// Create a draggable titlebar div with window controls
/// Args: title="App", show_controls=true, theme="light"
pub fn create_draggable_titlebar(args: Vec<Value>) -> Result<Value> {
    let title = if args.is_empty() {
        "Tauraro App".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => "Tauraro App".to_string(),
        }
    };
    
    let show_controls = if args.len() > 1 {
        match &args[1] {
            Value::Bool(b) => *b,
            _ => true,
        }
    } else {
        true
    };
    
    let theme = if args.len() > 2 {
        match &args[2] {
            Value::Str(s) => s.clone(),
            _ => "light".to_string(),
        }
    } else {
        "light".to_string()
    };
    
    let bg_color = if theme == "dark" { "#1e1e1e" } else { "#f0f0f0" };
    let text_color = if theme == "dark" { "#ffffff" } else { "#000000" };
    let button_hover = if theme == "dark" { "#3a3a3a" } else { "#e0e0e0" };
    
    let controls_html = if show_controls {
        format!(r#"
            <div class="titlebar-controls titlebar-no-drag" style="display: flex; gap: 0;">
                <button onclick="window.tauraro.minimize()" class="titlebar-button" style="
                    background: transparent;
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
                " onmouseover="this.style.backgroundColor='{}'" onmouseout="this.style.backgroundColor='transparent'">
                    <span style="display: inline-block; width: 10px; height: 1px; background: currentColor;"></span>
                </button>
                <button onclick="window.tauraro.maximize()" class="titlebar-button" style="
                    background: transparent;
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
                " onmouseover="this.style.backgroundColor='{}'" onmouseout="this.style.backgroundColor='transparent'">
                    <span style="display: inline-block; width: 9px; height: 9px; border: 1px solid currentColor;"></span>
                </button>
                <button onclick="window.tauraro.close()" class="titlebar-button" style="
                    background: transparent;
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
                " onmouseover="this.style.backgroundColor='#e81123'; this.style.color='white';" onmouseout="this.style.backgroundColor='transparent'; this.style.color='{}';">
                    <span style="display: inline-block; font-size: 12px;">✕</span>
                </button>
            </div>
        "#, text_color, button_hover, text_color, button_hover, text_color, text_color)
    } else {
        String::new()
    };
    
    let html = format!(r#"
        <div data-tauri-drag-region class="titlebar-drag" style="
            display: flex;
            justify-content: space-between;
            align-items: center;
            height: 32px;
            background-color: {};
            color: {};
            padding: 0 10px;
            user-select: none;
            -webkit-user-select: none;
            -webkit-app-region: drag;
            app-region: drag;
        ">
            <div class="titlebar-title" style="
                font-size: 12px;
                flex-grow: 1;
            ">{}</div>
            {}
        </div>
    "#, bg_color, text_color, title, controls_html);
    
    Ok(Value::Str(html))
}

/// Inject draggable titlebar CSS into head
pub fn inject_drag_css(args: Vec<Value>) -> Result<Value> {
    let css = get_drag_region_css();
    let html = format!("<style>{}</style>", css);
    Ok(Value::Str(html))
}

/// Inject draggable titlebar JavaScript into body
pub fn inject_drag_js(args: Vec<Value>) -> Result<Value> {
    let js = get_drag_region_js();
    let html = format!("<script>{}</script>", js);
    Ok(Value::Str(html))
}

/// Get window control API JavaScript
/// This provides window.tauraro.minimize(), maximize(), close(), dragWindow(), etc.
pub fn get_window_control_api_js() -> String {
    r#"
        window.tauraro = window.tauraro || {};
        
        console.log('[Tauraro API] Initializing window control API...');
        
        // Window dragging function
        window.tauraro.dragWindow = function() {
            console.log('[Tauraro API] Drag window requested');
            if (window.ipc && window.ipc.postMessage) {
                window.ipc.postMessage(JSON.stringify({
                    cmd: 'drag_window'
                }));
            } else if (window.external && window.external.invoke) {
                window.external.invoke(JSON.stringify({
                    cmd: 'drag_window'
                }));
            }
        };
        
        // Window control functions
        window.tauraro.minimize = function() {
            console.log('[Tauraro API] Minimize window requested');
            if (window.ipc && window.ipc.postMessage) {
                window.ipc.postMessage(JSON.stringify({
                    cmd: 'minimize_window'
                }));
            } else if (window.external && window.external.invoke) {
                window.external.invoke(JSON.stringify({
                    cmd: 'minimize_window'
                }));
            }
        };
        
        window.tauraro.maximize = function() {
            console.log('[Tauraro API] Maximize window requested');
            if (window.ipc && window.ipc.postMessage) {
                window.ipc.postMessage(JSON.stringify({
                    cmd: 'maximize_window'
                }));
            } else if (window.external && window.external.invoke) {
                window.external.invoke(JSON.stringify({
                    cmd: 'maximize_window'
                }));
            }
        };
        
        window.tauraro.restore = function() {
            console.log('[Tauraro API] Restore window requested');
            if (window.ipc && window.ipc.postMessage) {
                window.ipc.postMessage(JSON.stringify({
                    cmd: 'restore_window'
                }));
            } else if (window.external && window.external.invoke) {
                window.external.invoke(JSON.stringify({
                    cmd: 'restore_window'
                }));
            }
        };
        
        window.tauraro.close = function() {
            console.log('[Tauraro API] Close window requested');
            if (window.ipc && window.ipc.postMessage) {
                window.ipc.postMessage(JSON.stringify({
                    cmd: 'close_window'
                }));
            } else if (window.external && window.external.invoke) {
                window.external.invoke(JSON.stringify({
                    cmd: 'close_window'
                }));
            } else {
                window.close();
            }
        };
        
        window.tauraro.fullscreen = function() {
            console.log('[Tauraro API] Toggle fullscreen requested');
            if (window.ipc && window.ipc.postMessage) {
                window.ipc.postMessage(JSON.stringify({
                    cmd: 'fullscreen_window'
                }));
            } else if (window.external && window.external.invoke) {
                window.external.invoke(JSON.stringify({
                    cmd: 'fullscreen_window'
                }));
            }
        };
        
        window.tauraro.isMaximized = false;
        
        // Toggle maximize/restore
        window.tauraro.toggleMaximize = function() {
            if (window.tauraro.isMaximized) {
                window.tauraro.restore();
            } else {
                window.tauraro.maximize();
            }
            window.tauraro.isMaximized = !window.tauraro.isMaximized;
        };
        
        console.log('[Tauraro API] Window control API ready');
    "#.to_string()
}

/// Inject window control API JavaScript
pub fn inject_window_control_api(args: Vec<Value>) -> Result<Value> {
    let js = get_window_control_api_js();
    let html = format!("<script>{}</script>", js);
    Ok(Value::Str(html))
}
