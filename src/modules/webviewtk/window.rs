/// Window management and event loop for WebViewTK
/// Handles native window creation, menu attachment (Windows), and event dispatching

use crate::value::Value;
use crate::modules::asyncio::runtime::AsyncRuntime;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use std::sync::{Arc, Mutex};
use once_cell::sync::Lazy;

#[cfg(feature = "webviewtk")]
use crossbeam::channel::{unbounded, Sender, Receiver};

#[cfg(feature = "webviewtk")]
use serde_json;

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

/// Helper: Serialize handler information to pass across thread boundary
/// We store function names that can be looked up and called later
#[cfg(feature = "webviewtk")]
fn serialize_handler_info(value: &Value) -> Option<String> {
    match value {
        Value::NativeFunction(_) => Some("native_function".to_string()),
        Value::Closure { .. } => Some("closure".to_string()),
        Value::Str(s) => Some(s.clone()),  // Function name as string
        _ => None,
    }
}

/// IPC event that gets sent from the window thread to main thread
#[derive(Debug, Clone)]
struct IpcEvent {
    event_name: String,
    data: String,  // JSON data as string
}

/// Global registry for IPC event channels
/// Each window gets a unique ID and its own channel
static IPC_CHANNELS: Lazy<Arc<Mutex<HashMap<u64, Sender<IpcEvent>>>>> = Lazy::new(|| {
    Arc::new(Mutex::new(HashMap::new()))
});

/// Counter for generating unique window IDs
static WINDOW_ID_COUNTER: Lazy<Arc<Mutex<u64>>> = Lazy::new(|| {
    Arc::new(Mutex::new(0))
});

/// Create a Window class instance
pub fn create_window_class(args: Vec<Value>) -> Result<Value> {
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

    // Generate unique window ID and create IPC event channel
    #[cfg(feature = "webviewtk")]
    let window_id = {
        let mut counter = WINDOW_ID_COUNTER.lock().unwrap();
        *counter += 1;
        *counter
    };
    
    #[cfg(feature = "webviewtk")]
    let (tx, rx) = unbounded::<IpcEvent>();
    
    #[cfg(feature = "webviewtk")]
    {
        // Register the sender in the global registry
        IPC_CHANNELS.lock().unwrap().insert(window_id, tx);
    }
    
    // Create window object
    let mut window_obj = HashMap::new();
    window_obj.insert("title".to_string(), Value::Str(title));
    window_obj.insert("width".to_string(), Value::Int(width as i64));
    window_obj.insert("height".to_string(), Value::Int(height as i64));
    window_obj.insert("html".to_string(), Value::Str(String::new()));
    window_obj.insert("menu".to_string(), Value::None);
    window_obj.insert("titlebar".to_string(), Value::None);
    window_obj.insert("icon".to_string(), Value::Str(String::new()));
    window_obj.insert("resizable".to_string(), Value::Bool(true));
    window_obj.insert("decorations".to_string(), Value::Bool(true));
    window_obj.insert("message_handlers".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));
    
    // Store window ID and receiver for process_events
    #[cfg(feature = "webviewtk")]
    window_obj.insert("_window_id".to_string(), Value::Int(window_id as i64));
    
    #[cfg(feature = "webviewtk")]
    window_obj.insert("_ipc_receiver".to_string(), Value::Str(format!("{:p}", Box::into_raw(Box::new(rx)) as *const _)));

    // Add methods
    window_obj.insert("set_html".to_string(), Value::NativeFunction(window_set_html));
    window_obj.insert("set_menu".to_string(), Value::NativeFunction(window_set_menu));
    window_obj.insert("set_titlebar".to_string(), Value::NativeFunction(window_set_titlebar));
    window_obj.insert("set_icon".to_string(), Value::NativeFunction(window_set_icon));
    window_obj.insert("set_resizable".to_string(), Value::NativeFunction(window_set_resizable));
    window_obj.insert("disable_decorations".to_string(), Value::NativeFunction(window_disable_decorations));
    window_obj.insert("on_message".to_string(), Value::NativeFunction(window_on_message));
    window_obj.insert("run".to_string(), Value::NativeFunction(window_run));
    window_obj.insert("run_async".to_string(), Value::NativeFunction(window_run_async));
    window_obj.insert("process_events".to_string(), Value::NativeFunction(window_process_events));

    Ok(Value::Dict(Rc::new(RefCell::new(window_obj))))
}

/// Set HTML content for the window
pub fn window_set_html(args: Vec<Value>) -> Result<Value> {
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

/// Set menu for the window
pub fn window_set_menu(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("set_menu() requires self and menu arguments"));
    }

    let menu = args[1].clone();

    // Update the window object's menu field
    if let Value::Dict(dict) = &args[0] {
        dict.borrow_mut().insert("menu".to_string(), menu);
    }

    Ok(Value::None)
}

/// Set title bar configuration for the window
pub fn window_set_titlebar(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("set_titlebar() requires self and titlebar arguments"));
    }

    let titlebar = args[1].clone();

    // Update the window object's titlebar field
    if let Value::Dict(dict) = &args[0] {
        dict.borrow_mut().insert("titlebar".to_string(), titlebar);
    }

    Ok(Value::None)
}

/// Set window icon
pub fn window_set_icon(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("set_icon() requires self and icon_path arguments"));
    }

    let icon_path = extract_string_arg(&args, 1).unwrap_or_default();

    // Update the window object's icon field
    if let Value::Dict(dict) = &args[0] {
        dict.borrow_mut().insert("icon".to_string(), Value::Str(icon_path));
    }

    Ok(Value::None)
}

/// Set whether window is resizable
pub fn window_set_resizable(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("set_resizable() requires self and resizable (bool) arguments"));
    }

    let resizable = match &args[1] {
        Value::Bool(b) => *b,
        _ => true,
    };

    // Update the window object's resizable field
    if let Value::Dict(dict) = &args[0] {
        dict.borrow_mut().insert("resizable".to_string(), Value::Bool(resizable));
    }

    Ok(Value::None)
}

/// Disable window decorations (title bar, borders, etc.)
pub fn window_disable_decorations(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("disable_decorations() requires self argument"));
    }

    // Update the window object's decorations field
    if let Value::Dict(dict) = &args[0] {
        dict.borrow_mut().insert("decorations".to_string(), Value::Bool(false));
    }

    Ok(Value::None)
}

/// Register a message handler for IPC communication from frontend
/// Usage: window.on_message("event_name", lambda msg: print(msg))
pub fn window_on_message(args: Vec<Value>) -> Result<Value> {
    if args.len() < 3 {
        return Err(anyhow::anyhow!("on_message() requires self, event_name, and handler arguments"));
    }

    let event_name = extract_string_arg(&args, 1)
        .ok_or_else(|| anyhow::anyhow!("on_message() requires event_name as string"))?;
    let handler = args[2].clone();

    // Store handler in window object's message_handlers dict
    if let Value::Dict(dict) = &args[0] {
        let mut borrowed = dict.borrow_mut();
        if let Some(Value::Dict(handlers)) = borrowed.get("message_handlers") {
            handlers.borrow_mut().insert(event_name, handler);
        }
    }

    Ok(Value::None)
}

/// Run the window event loop (blocking - waits until window closes)
pub fn window_run(args: Vec<Value>) -> Result<Value> {
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

        // Extract window data, window_id, and message handlers before thread spawn
        let (title, width, height, html, decorations, handlers_map, win_id) = if let Value::Dict(dict) = &args[0] {
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
            let decorations = match d.get("decorations") {
                Some(Value::Bool(b)) => *b,
                _ => true,  // Default to showing decorations
            };
            
            // Extract window ID
            let win_id = match d.get("_window_id") {
                Some(Value::Int(id)) => *id as u64,
                _ => 0,  // Fallback (should not happen)
            };
            
            // Extract message handlers (serialize to strings for thread safety)
            let mut handlers = HashMap::new();
            if let Some(Value::Dict(handlers_dict)) = d.get("message_handlers") {
                let h = handlers_dict.borrow();
                for (key, value) in h.iter() {
                    if let Some(handler_info) = serialize_handler_info(value) {
                        handlers.insert(key.clone(), handler_info);
                    }
                }
            }
            
            (title, width, height, html, decorations, handlers, win_id)
        } else {
            return Err(anyhow::anyhow!("run() requires a Window object"));
        };

        // Validate HTML is not empty
        if html.is_empty() {
            return Err(anyhow::anyhow!("Window HTML content is empty. Call set_html() first."));
        }

        // Spawn window in a separate thread to allow multiple processes
        let handle = std::thread::spawn(move || {
            // Create event loop (use new_any_thread on Windows for thread safety)
            #[cfg(target_os = "windows")]
            let event_loop = EventLoop::<()>::new_any_thread();

            #[cfg(not(target_os = "windows"))]
            let event_loop = EventLoop::<()>::new();

            let window = WindowBuilder::new()
                .with_title(&title)
                .with_inner_size(tao::dpi::LogicalSize::new(width, height))
                .with_decorations(decorations)  // Apply decorations setting (cross-platform)
                .build(&event_loop)
                .expect("Failed to create window");
            
            // Store window ID for event loop access (tao window ID, not our tracking ID)
            let window_id = window.id();
            
            // Create webview with IPC handler
            let webview = WebViewBuilder::new(window)
                .expect("Failed to create webview")
                .with_html(&html)
                .expect("Failed to set HTML")
                .with_initialization_script(r#"
                    // Polyfill for -webkit-app-region drag support and window controls
                    (function() {
                        // Initialize window.tauraro API
                        window.tauraro = window.tauraro || {};
                        
                        window.tauraro.minimize = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'minimize_window' }));
                            }
                        };
                        
                        window.tauraro.maximize = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'maximize_window' }));
                            }
                        };
                        
                        window.tauraro.restore = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'restore_window' }));
                            }
                        };
                        
                        window.tauraro.close = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'close_window' }));
                            }
                        };
                        
                        window.tauraro.fullscreen = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'fullscreen_window' }));
                            }
                        };
                        
                        window.tauraro.dragWindow = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'drag_window' }));
                            }
                        };
                        
                        let isDragging = false;
                        document.addEventListener('mousedown', function(e) {
                            const element = e.target.closest('[data-tauri-drag-region], .titlebar-drag, [style*="webkit-app-region: drag"]');
                            if (element) {
                                const computedStyle = window.getComputedStyle(element);
                                if (computedStyle.webkitAppRegion === 'drag' || element.style.webkitAppRegion === 'drag' || 
                                    element.hasAttribute('data-tauri-drag-region') || element.classList.contains('titlebar-drag')) {
                                    // Check if clicking on no-drag child
                                    const noDragChild = e.target.closest('[data-tauri-drag-no-region], .titlebar-no-drag, button, a, input, select, textarea');
                                    if (noDragChild) {
                                        const noDragStyle = window.getComputedStyle(noDragChild);
                                        if (noDragStyle.webkitAppRegion === 'no-drag' || noDragChild.style.webkitAppRegion === 'no-drag' ||
                                            noDragChild.hasAttribute('data-tauri-drag-no-region') || noDragChild.classList.contains('titlebar-no-drag') ||
                                            ['BUTTON', 'A', 'INPUT', 'SELECT', 'TEXTAREA'].includes(noDragChild.tagName)) {
                                            return;
                                        }
                                    }
                                    isDragging = true;
                                    if (window.ipc && window.ipc.postMessage) {
                                        window.ipc.postMessage(JSON.stringify({ cmd: 'drag_window' }));
                                    }
                                    e.preventDefault();
                                }
                            }
                        }, true);
                        document.addEventListener('mouseup', function() {
                            isDragging = false;
                        }, true);
                    })();
                "#)
                .with_ipc_handler(move |webview, msg| {
                    // Parse JSON message
                    if let Ok(json) = serde_json::from_str::<serde_json::Value>(&msg) {
                        if let Some(cmd) = json.get("cmd").and_then(|v| v.as_str()) {
                            // Check registered custom handlers first
                            if let Some(handler_info) = handlers_map.get(cmd) {
                                eprintln!("[IPC] Custom event '{}' registered (handler: {})", cmd, handler_info);
                                
                                // Extract event data
                                let data = json.get("value")
                                    .map(|v| v.to_string())
                                    .unwrap_or_else(|| "null".to_string());
                                
                                eprintln!("[IPC] Event data: {}", data);
                                
                                // Send event to main thread via channel
                                let event = IpcEvent {
                                    event_name: cmd.to_string(),
                                    data,
                                };
                                
                                if let Ok(channels) = IPC_CHANNELS.lock() {
                                    if let Some(sender) = channels.get(&win_id) {
                                        if let Err(e) = sender.send(event) {
                                            eprintln!("[IPC] Failed to send event to main thread: {}", e);
                                        } else {
                                            eprintln!("[IPC] Event sent to main thread for execution");
                                        }
                                    } else {
                                        eprintln!("[IPC] No channel found for window ID {}", win_id);
                                    }
                                } else {
                                    eprintln!("[IPC] Failed to lock IPC channels");
                                }
                                return;
                            }
                            
                            // Built-in window commands
                            match cmd {
                                "drag_window" => {
                                    #[cfg(target_os = "windows")]
                                    {
                                        use wry::application::platform::windows::WindowExtWindows;
                                        let _ = webview.drag_window();
                                    }
                                    #[cfg(target_os = "macos")]
                                    {
                                        use wry::application::platform::macos::WindowExtMacOS;
                                        let _ = webview.drag_window();
                                    }
                                    #[cfg(target_os = "linux")]
                                    {
                                        use wry::application::platform::unix::WindowExtUnix;
                                        let _ = webview.drag_window();
                                    }
                                }
                                "minimize_window" => {
                                    webview.set_minimized(true);
                                }
                                "maximize_window" => {
                                    let is_maximized = webview.is_maximized();
                                    webview.set_maximized(!is_maximized);
                                }
                                "restore_window" => {
                                    webview.set_maximized(false);
                                }
                                "fullscreen_window" => {
                                    if webview.fullscreen().is_some() {
                                        webview.set_fullscreen(None);
                                    } else {
                                        use wry::application::window::Fullscreen;
                                        webview.set_fullscreen(Some(Fullscreen::Borderless(None)));
                                    }
                                }
                                "close_window" => {
                                    std::process::exit(0);
                                }
                                // Menu commands
                                cmd if cmd.starts_with("menu_") => {
                                    eprintln!("[IPC] Menu action: {}", cmd);
                                }
                                // Search commands
                                "search" | "search_query" => {
                                    if let Some(value) = json.get("value") {
                                        eprintln!("[IPC] Search query: {}", value);
                                    }
                                }
                                _ => {
                                    eprintln!("[IPC] Unhandled event: {}", cmd);
                                }
                            }
                        }
                    }
                })
                .build()
                .expect("Failed to build webview");

            eprintln!("[DEBUG] Window created with IPC handler, starting event loop...");

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

        // Block until window closes
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
pub fn window_run_async(args: Vec<Value>) -> Result<Value> {
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

        let (title, width, height, html, decorations, handlers_map, win_id) = if let Value::Dict(dict) = &args[0] {
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
            let decorations = match d.get("decorations") {
                Some(Value::Bool(b)) => *b,
                _ => true,  // Default to showing decorations
            };
            
            // Extract window ID
            let win_id = match d.get("_window_id") {
                Some(Value::Int(id)) => *id as u64,
                _ => 0,  // Fallback (should not happen)
            };
            
            // Extract message handlers (serialize to strings for thread safety)
            let mut handlers = HashMap::new();
            if let Some(Value::Dict(handlers_dict)) = d.get("message_handlers") {
                let h = handlers_dict.borrow();
                for (key, value) in h.iter() {
                    if let Some(handler_info) = serialize_handler_info(value) {
                        handlers.insert(key.clone(), handler_info);
                    }
                }
            }
            
            (title, width, height, html, decorations, handlers, win_id)
        } else {
            return Err(anyhow::anyhow!("run_async() requires a Window object"));
        };

        // Validate HTML is not empty
        if html.is_empty() {
            return Err(anyhow::anyhow!("Window HTML content is empty. Call set_html() first."));
        }

        // Spawn window in background thread (don't wait for it)
        std::thread::spawn(move || {
            #[cfg(target_os = "windows")]
            let event_loop = EventLoop::<()>::new_any_thread();

            #[cfg(not(target_os = "windows"))]
            let event_loop = EventLoop::<()>::new();

            let window = WindowBuilder::new()
                .with_title(&title)
                .with_inner_size(tao::dpi::LogicalSize::new(width, height))
                .with_decorations(decorations)  // Apply decorations setting
                .build(&event_loop)
                .expect("Failed to create window");

            let window_id = window.id();

            let webview = WebViewBuilder::new(window)
                .expect("Failed to create webview")
                .with_html(&html)
                .expect("Failed to set HTML")
                .with_initialization_script(r#"
                    // Polyfill for -webkit-app-region drag support and window controls
                    (function() {
                        // Initialize window.tauraro API
                        window.tauraro = window.tauraro || {};
                        
                        window.tauraro.minimize = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'minimize_window' }));
                            }
                        };
                        
                        window.tauraro.maximize = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'maximize_window' }));
                            }
                        };
                        
                        window.tauraro.restore = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'restore_window' }));
                            }
                        };
                        
                        window.tauraro.close = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'close_window' }));
                            }
                        };
                        
                        window.tauraro.fullscreen = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'fullscreen_window' }));
                            }
                        };
                        
                        window.tauraro.dragWindow = function() {
                            if (window.ipc && window.ipc.postMessage) {
                                window.ipc.postMessage(JSON.stringify({ cmd: 'drag_window' }));
                            }
                        };
                        
                        let isDragging = false;
                        document.addEventListener('mousedown', function(e) {
                            const element = e.target.closest('[data-tauri-drag-region], .titlebar-drag, [style*="webkit-app-region: drag"]');
                            if (element) {
                                const computedStyle = window.getComputedStyle(element);
                                if (computedStyle.webkitAppRegion === 'drag' || element.style.webkitAppRegion === 'drag' || 
                                    element.hasAttribute('data-tauri-drag-region') || element.classList.contains('titlebar-drag')) {
                                    // Check if clicking on no-drag child
                                    const noDragChild = e.target.closest('[data-tauri-drag-no-region], .titlebar-no-drag, button, a, input, select, textarea');
                                    if (noDragChild) {
                                        const noDragStyle = window.getComputedStyle(noDragChild);
                                        if (noDragStyle.webkitAppRegion === 'no-drag' || noDragChild.style.webkitAppRegion === 'no-drag' ||
                                            noDragChild.hasAttribute('data-tauri-drag-no-region') || noDragChild.classList.contains('titlebar-no-drag') ||
                                            ['BUTTON', 'A', 'INPUT', 'SELECT', 'TEXTAREA'].includes(noDragChild.tagName)) {
                                            return;
                                        }
                                    }
                                    isDragging = true;
                                    if (window.ipc && window.ipc.postMessage) {
                                        window.ipc.postMessage(JSON.stringify({ cmd: 'drag_window' }));
                                    }
                                    e.preventDefault();
                                }
                            }
                        }, true);
                        document.addEventListener('mouseup', function() {
                            isDragging = false;
                        }, true);
                    })();
                "#)
                .with_ipc_handler(move |webview, msg| {
                    // Parse JSON message
                    if let Ok(json) = serde_json::from_str::<serde_json::Value>(&msg) {
                        if let Some(cmd) = json.get("cmd").and_then(|v| v.as_str()) {
                            // Check registered custom handlers first
                            if let Some(handler_info) = handlers_map.get(cmd) {
                                eprintln!("[IPC] Custom event '{}' registered (handler: {})", cmd, handler_info);
                                
                                // Extract event data
                                let data = json.get("value")
                                    .map(|v| v.to_string())
                                    .unwrap_or_else(|| "null".to_string());
                                
                                eprintln!("[IPC] Event data: {}", data);
                                
                                // Send event to main thread via channel
                                let event = IpcEvent {
                                    event_name: cmd.to_string(),
                                    data,
                                };
                                
                                if let Ok(channels) = IPC_CHANNELS.lock() {
                                    if let Some(sender) = channels.get(&win_id) {
                                        if let Err(e) = sender.send(event) {
                                            eprintln!("[IPC] Failed to send event to main thread: {}", e);
                                        } else {
                                            eprintln!("[IPC] Event sent to main thread for execution");
                                        }
                                    } else {
                                        eprintln!("[IPC] No channel found for window ID {}", win_id);
                                    }
                                } else {
                                    eprintln!("[IPC] Failed to lock IPC channels");
                                }
                                return;
                            }
                            
                            // Built-in window commands
                            match cmd {
                                "drag_window" => {
                                    #[cfg(target_os = "windows")]
                                    {
                                        use wry::application::platform::windows::WindowExtWindows;
                                        let _ = webview.drag_window();
                                    }
                                    #[cfg(target_os = "macos")]
                                    {
                                        use wry::application::platform::macos::WindowExtMacOS;
                                        let _ = webview.drag_window();
                                    }
                                    #[cfg(target_os = "linux")]
                                    {
                                        use wry::application::platform::unix::WindowExtUnix;
                                        let _ = webview.drag_window();
                                    }
                                }
                                "minimize_window" => {
                                    webview.set_minimized(true);
                                }
                                "maximize_window" => {
                                    let is_maximized = webview.is_maximized();
                                    webview.set_maximized(!is_maximized);
                                }
                                "restore_window" => {
                                    webview.set_maximized(false);
                                }
                                "fullscreen_window" => {
                                    if webview.fullscreen().is_some() {
                                        webview.set_fullscreen(None);
                                    } else {
                                        use wry::application::window::Fullscreen;
                                        webview.set_fullscreen(Some(Fullscreen::Borderless(None)));
                                    }
                                }
                                "close_window" => {
                                    std::process::exit(0);
                                }
                                // Menu commands
                                cmd if cmd.starts_with("menu_") => {
                                    eprintln!("[IPC] Menu action: {}", cmd);
                                }
                                // Search commands
                                "search" | "search_query" => {
                                    if let Some(value) = json.get("value") {
                                        eprintln!("[IPC] Search query: {}", value);
                                    }
                                }
                                _ => {
                                    eprintln!("[IPC] Unhandled event: {}", cmd);
                                }
                            }
                        }
                    }
                })
                .build()
                .expect("Failed to build webview");

            eprintln!("[DEBUG] Async window created with IPC handler, starting event loop...");

            event_loop.run(move |event, _, control_flow| {
                *control_flow = ControlFlow::Wait;

                match event {
                    Event::WindowEvent {
                        event: WindowEvent::CloseRequested,
                        ..
                    } => {
                        *control_flow = ControlFlow::Exit;
                    }
                    _ => {}
                }
            });
        });

        Ok(Value::None)
    }
}

/// Process pending IPC events from the window thread
/// This should be called periodically (e.g., in an event loop or after operations)
/// to execute handlers for events sent from the JavaScript side
/// 
/// The function executes handlers directly if they are native functions.
/// For Tauraro closures (including async functions), it calls them with the event data.
/// For async handlers, the returned coroutine should be awaited by the caller.
pub fn window_process_events(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "webviewtk"))]
    {
        return Err(anyhow::anyhow!(
            "WebViewTK feature is not enabled. Please compile with --features webviewtk"
        ));
    }

    #[cfg(feature = "webviewtk")]
    {
        use crate::vm::VM;
        
        if args.is_empty() {
            return Err(anyhow::anyhow!("process_events() requires self argument"));
        }

        // Get window object
        let window_dict = match &args[0] {
            Value::Dict(dict) => dict,
            _ => return Err(anyhow::anyhow!("process_events() requires a Window object")),
        };

        // Extract receiver pointer
        let receiver_ptr = {
            let d = window_dict.borrow();
            match d.get("_ipc_receiver") {
                Some(Value::Str(s)) => {
                    // Parse pointer from string
                    let ptr_str = s.trim_start_matches("0x");
                    usize::from_str_radix(ptr_str, 16)
                        .map_err(|e| anyhow::anyhow!("Failed to parse receiver pointer: {}", e))?
                }
                _ => return Err(anyhow::anyhow!("Window object missing _ipc_receiver")),
            }
        };

        // SAFETY: The receiver was allocated with Box::into_raw in create_window_class
        // and is valid for the lifetime of the window object
        let receiver = unsafe { &*(receiver_ptr as *const Receiver<IpcEvent>) };

        // Get message handlers
        let handlers_dict = {
            let d = window_dict.borrow();
            match d.get("message_handlers") {
                Some(Value::Dict(dict)) => dict.clone(),
                _ => return Ok(Value::Int(0)),  // No handlers registered
            }
        };

        // Process all pending events (non-blocking)
        let mut processed = 0;
        while let Ok(event) = receiver.try_recv() {
            eprintln!("[process_events] Processing event: {}", event.event_name);
            
            // Look up handler
            let handler = {
                let h = handlers_dict.borrow();
                h.get(&event.event_name).cloned()
            };

            if let Some(handler_value) = handler {
                // Parse JSON data to Value
                let event_data = parse_json_to_value(&event.data)?;
                
                // Execute handler with event data
                match &handler_value {
                    Value::Closure { .. } => {
                        // Create a VM instance to call the closure
                        let mut vm = VM::new();
                        
                        // Call the closure with event data as argument
                        match vm.call_function(handler_value.clone(), vec![event_data]) {
                            Ok(result) => {
                                // Check if result is a coroutine (async handler)
                                if let Value::Coroutine { .. } = result {
                                    eprintln!("[process_events] Handler returned coroutine - running async");
                                    // For async handlers, run the coroutine using AsyncRuntime
                                    let runtime = crate::modules::asyncio::runtime::AsyncRuntime::global();
                                    match runtime.run_until_complete(result) {
                                        Ok(async_result) => {
                                            eprintln!("[process_events] Async handler completed: {:?}", async_result);
                                        }
                                        Err(e) => {
                                            eprintln!("[process_events] Async handler failed: {}", e);
                                        }
                                    }
                                } else {
                                    eprintln!("[process_events] Sync handler returned: {:?}", result);
                                }
                            }
                            Err(e) => {
                                eprintln!("[process_events] Handler error: {}", e);
                            }
                        }
                    }
                    Value::NativeFunction(func) => {
                        // Call native function directly
                        eprintln!("[process_events] Calling native function handler");
                        match func(vec![event_data]) {
                            Ok(result) => {
                                eprintln!("[process_events] Native handler returned: {:?}", result);
                            }
                            Err(e) => {
                                eprintln!("[process_events] Native handler error: {}", e);
                            }
                        }
                    }
                    _ => {
                        eprintln!("[process_events] Handler is not callable: {:?}", handler_value);
                    }
                }
                
                processed += 1;
            } else {
                eprintln!("[process_events] No handler found for event: {}", event.event_name);
            }
        }

        Ok(Value::Int(processed))
    }
}

/// Helper: Parse JSON string to Tauraro Value
#[cfg(feature = "webviewtk")]
fn parse_json_to_value(json_str: &str) -> Result<Value> {
    let json: serde_json::Value = serde_json::from_str(json_str)
        .map_err(|e| anyhow::anyhow!("Failed to parse JSON: {}", e))?;
    
    Ok(json_to_tauraro_value(&json))
}

/// Helper: Convert serde_json::Value to Tauraro Value
#[cfg(feature = "webviewtk")]
fn json_to_tauraro_value(json: &serde_json::Value) -> Value {
    match json {
        serde_json::Value::Null => Value::None,
        serde_json::Value::Bool(b) => Value::Bool(*b),
        serde_json::Value::Number(n) => {
            if let Some(i) = n.as_i64() {
                Value::Int(i)
            } else if let Some(f) = n.as_f64() {
                Value::Float(f)
            } else {
                Value::None
            }
        }
        serde_json::Value::String(s) => Value::Str(s.clone()),
        serde_json::Value::Array(arr) => {
            let values: Vec<Value> = arr.iter().map(json_to_tauraro_value).collect();
            Value::new_list(values)
        }
        serde_json::Value::Object(obj) => {
            let mut map = HashMap::new();
            for (k, v) in obj.iter() {
                map.insert(k.clone(), json_to_tauraro_value(v));
            }
            Value::Dict(Rc::new(RefCell::new(map)))
        }
    }
}

// Helper function to convert String to wide (UTF-16) for Windows API
#[cfg(target_os = "windows")]
fn to_wide(s: &str) -> Vec<u16> {
    use std::os::windows::ffi::OsStrExt;
    use std::ffi::OsStr;
    
    let wide: Vec<u16> = OsStr::new(s)
        .encode_wide()
        .collect();
    wide
}

/// Extract a string argument from a Value vector
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
