// Window module - cross-platform window management

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use std::path::Path;
use std::fs;
use std::sync::{Arc, Mutex};
use std::thread;
use crossbeam::channel::{unbounded, Sender, Receiver};

#[cfg(feature = "webviewtk")]
use wry::{
    application::{
        event::{Event, WindowEvent},
        event_loop::{ControlFlow, EventLoop},
        window::{WindowBuilder, Window as WryWindow},
    },
    webview::WebViewBuilder,
};

pub struct Window {
    pub title: String,
    pub width: f64,
    pub height: f64,
    pub decorations: bool,
    pub resizable: bool,
    pub html_content: String,
}

#[cfg(feature = "webviewtk")]
pub fn create(args: Vec<Value>) -> Result<Value> {
    use crate::modules::webviewtk::utils::extract_kwargs;
    
    let mut title = "WebViewTK App".to_string();
    let mut width = 800.0;
    let mut height = 600.0;
    let mut decorations = true;
    let mut resizable = true;
    let mut native_titlebar = true;
    
    if let Some(kwargs) = extract_kwargs(&args) {
        if let Some(Value::Str(t)) = kwargs.get("title") {
            title = t.clone();
        }
        if let Some(val) = kwargs.get("width") {
            width = match val {
                Value::Float(f) => *f,
                Value::Int(i) => *i as f64,
                _ => 800.0,
            };
        }
        if let Some(val) = kwargs.get("height") {
            height = match val {
                Value::Float(f) => *f,
                Value::Int(i) => *i as f64,
                _ => 600.0,
            };
        }
        if let Some(Value::Bool(d)) = kwargs.get("decorations") {
            decorations = *d;
        }
        if let Some(Value::Bool(r)) = kwargs.get("resizable") {
            resizable = *r;
        }
        if let Some(Value::Bool(nt)) = kwargs.get("native_titlebar") {
            native_titlebar = *nt;
            // If native_titlebar is False, disable decorations
            if !native_titlebar {
                decorations = false;
            }
        }
    }
    
    let mut window_dict = HashMap::new();
    window_dict.insert("title".to_string(), Value::Str(title.clone()));
    window_dict.insert("width".to_string(), Value::Float(width));
    window_dict.insert("height".to_string(), Value::Float(height));
    window_dict.insert("decorations".to_string(), Value::Bool(decorations));
    window_dict.insert("resizable".to_string(), Value::Bool(resizable));
    window_dict.insert("native_titlebar".to_string(), Value::Bool(native_titlebar));
    window_dict.insert("_widget_type".to_string(), Value::Str("Window".to_string()));
    
    // Initialize resource collections using HPList
    use crate::modules::hplist::HPList;
    window_dict.insert("_cdns".to_string(), Value::List(HPList::new()));
    window_dict.insert("_custom_css".to_string(), Value::List(HPList::new()));
    window_dict.insert("_custom_js".to_string(), Value::List(HPList::new()));
    window_dict.insert("_custom_html".to_string(), Value::Str(String::new()));
    
    Ok(Value::Dict(Rc::new(RefCell::new(window_dict))))
}

#[cfg(not(feature = "webviewtk"))]
pub fn create(_args: Vec<Value>) -> Result<Value> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

// IPC message structures
#[cfg(feature = "webviewtk")]
#[derive(serde::Deserialize)]
struct IpcMessage {
    #[serde(rename = "type")]
    msg_type: Option<String>,
    id: Option<u32>,
    action: Option<String>,
    command: Option<String>,
    args: Option<serde_json::Value>,
    data: Option<serde_json::Value>,
    timestamp: Option<String>,
}

#[cfg(feature = "webviewtk")]
#[derive(serde::Serialize)]
struct IpcResponse {
    id: Option<u32>,
    action: Option<String>,
    command: Option<String>,
    success: bool,
    data: Option<serde_json::Value>,
    error: Option<String>,
}

/// Handle custom IPC messages from frontend
#[cfg(feature = "webviewtk")]
fn handle_ipc_message(tx: &Sender<String>, message: &str, commands: &super::command_registry::CommandRegistry) {
    // Try to parse as JSON
    match serde_json::from_str::<IpcMessage>(message) {
        Ok(ipc_msg) => {
            // Check message type
            let msg_type = ipc_msg.msg_type.as_deref().unwrap_or("action");
            
            match msg_type {
                "invoke" => {
                    // Command invocation (SPA API style)
                    if let Some(command) = &ipc_msg.command {
                        println!("🔧 Invoking command: {}, id={:?}", command, ipc_msg.id);
                        
                        let args = ipc_msg.args.clone().unwrap_or(serde_json::json!({}));
                        match commands.invoke(command, args) {
                            Ok(result) => send_command_response(tx, &ipc_msg, result),
                            Err(e) => send_command_error(tx, &ipc_msg, &e.to_string()),
                        }
                    } else {
                        send_command_error(tx, &ipc_msg, "Missing command name");
                    }
                }
                "action" | _ => {
                    // Legacy action-based IPC
                    if let Some(action) = &ipc_msg.action {
                        println!("📨 Received IPC action: {}, id={:?}", action, ipc_msg.id);
                        
                        match action.as_str() {
                            "system_info" => handle_system_info(tx, &ipc_msg),
                            "file_check" => handle_file_check(tx, &ipc_msg),
                            "calculate" => handle_calculate(tx, &ipc_msg),
                            "process_data" => handle_process_data(tx, &ipc_msg),
                            "save_settings" => handle_save_settings(tx, &ipc_msg),
                            _ => {
                                eprintln!("⚠️  Unknown IPC action: {}", action);
                                send_error_response(tx, &ipc_msg, "Unknown action");
                            }
                        }
                    } else {
                        send_error_response(tx, &ipc_msg, "Missing action or command");
                    }
                }
            }
        }
        Err(e) => {
            eprintln!("❌ Failed to parse IPC message as JSON: {}", e);
            eprintln!("   Raw message: {}", message);
        }
    }
}

/// Send response back to frontend
#[cfg(feature = "webviewtk")]
fn send_response(tx: &Sender<String>, msg: &IpcMessage, data: serde_json::Value) {
    let response = IpcResponse {
        id: msg.id,
        action: msg.action.clone(),
        command: None,
        success: true,
        data: Some(data),
        error: None,
    };
    
    if let Ok(json) = serde_json::to_string(&response) {
        let script = format!("if (window.handleBackendResponse) {{ window.handleBackendResponse({}); }}", json);
        let _ = tx.send(script);
        if let Some(action) = &msg.action {
            println!("✅ Sent response for action: {}", action);
        }
    }
}

/// Send error response back to frontend
#[cfg(feature = "webviewtk")]
fn send_error_response(tx: &Sender<String>, msg: &IpcMessage, error: &str) {
    let response = IpcResponse {
        id: msg.id,
        action: msg.action.clone(),
        command: None,
        success: false,
        data: None,
        error: Some(error.to_string()),
    };
    
    if let Ok(json) = serde_json::to_string(&response) {
        let script = format!("if (window.handleBackendResponse) {{ window.handleBackendResponse({}); }}", json);
        let _ = tx.send(script);
        if let Some(action) = &msg.action {
            println!("❌ Sent error response for action: {} - {}", action, error);
        }
    }
}

/// Send command response back to frontend (for invoke API)
#[cfg(feature = "webviewtk")]
fn send_command_response(tx: &Sender<String>, msg: &IpcMessage, data: serde_json::Value) {
    let response = IpcResponse {
        id: msg.id,
        action: None,
        command: msg.command.clone(),
        success: true,
        data: Some(data),
        error: None,
    };
    
    if let Ok(json) = serde_json::to_string(&response) {
        let script = format!("if (window.handleInvokeResponse) {{ window.handleInvokeResponse({}); }}", json);
        let _ = tx.send(script);
        if let Some(command) = &msg.command {
            println!("✅ Sent response for command: {}", command);
        }
    }
}

/// Send command error back to frontend (for invoke API)
#[cfg(feature = "webviewtk")]
fn send_command_error(tx: &Sender<String>, msg: &IpcMessage, error: &str) {
    let response = IpcResponse {
        id: msg.id,
        action: None,
        command: msg.command.clone(),
        success: false,
        data: None,
        error: Some(error.to_string()),
    };
    
    if let Ok(json) = serde_json::to_string(&response) {
        let script = format!("if (window.handleInvokeResponse) {{ window.handleInvokeResponse({}); }}", json);
        let _ = tx.send(script);
        if let Some(command) = &msg.command {
            println!("❌ Sent error response for command: {} - {}", command, error);
        }
    }
}

/// Register built-in commands available to all applications
#[cfg(feature = "webviewtk")]
fn register_builtin_commands(commands: &super::command_registry::CommandRegistry) {
    // Math operations
    commands.register("add", |args| {
        let a = args.get("a").and_then(|v| v.as_f64()).unwrap_or(0.0);
        let b = args.get("b").and_then(|v| v.as_f64()).unwrap_or(0.0);
        Ok(serde_json::json!(a + b))
    });
    
    commands.register("multiply", |args| {
        let a = args.get("a").and_then(|v| v.as_f64()).unwrap_or(1.0);
        let b = args.get("b").and_then(|v| v.as_f64()).unwrap_or(1.0);
        Ok(serde_json::json!(a * b))
    });
    
    // String operations
    commands.register("toUpperCase", |args| {
        let text = args.get("text").and_then(|v| v.as_str()).unwrap_or("");
        Ok(serde_json::json!(text.to_uppercase()))
    });
    
    commands.register("toLowerCase", |args| {
        let text = args.get("text").and_then(|v| v.as_str()).unwrap_or("");
        Ok(serde_json::json!(text.to_lowercase()))
    });
    
    // Array operations
    commands.register("reverseArray", |args| {
        if let Some(arr) = args.get("array").and_then(|v| v.as_array()) {
            let mut reversed = arr.clone();
            reversed.reverse();
            Ok(serde_json::json!(reversed))
        } else {
            Err(anyhow::anyhow!("Missing or invalid array parameter"))
        }
    });
    
    // System info
    commands.register("getSystemInfo", |_args| {
        use std::env;
        Ok(serde_json::json!({
            "os": env::consts::OS,
            "arch": env::consts::ARCH,
            "family": env::consts::FAMILY,
        }))
    });
    
    // Echo command for testing
    commands.register("echo", |args| {
        Ok(args)
    });
}

// IPC Action Handlers

#[cfg(feature = "webviewtk")]
fn handle_system_info(tx: &Sender<String>, msg: &IpcMessage) {
    use std::env;
    
    let mut info = serde_json::Map::new();
    info.insert("os".to_string(), serde_json::json!(env::consts::OS));
    info.insert("arch".to_string(), serde_json::json!(env::consts::ARCH));
    info.insert("family".to_string(), serde_json::json!(env::consts::FAMILY));
    
    if let Ok(hostname) = hostname::get() {
        info.insert("hostname".to_string(), serde_json::json!(hostname.to_string_lossy()));
    }
    
    send_response(tx, msg, serde_json::Value::Object(info));
}

#[cfg(feature = "webviewtk")]
fn handle_file_check(tx: &Sender<String>, msg: &IpcMessage) {
    if let Some(data) = &msg.data {
        if let Some(filename) = data.get("filename").and_then(|v| v.as_str()) {
            let exists = std::path::Path::new(filename).exists();
            let mut result = serde_json::Map::new();
            result.insert("filename".to_string(), serde_json::json!(filename));
            result.insert("exists".to_string(), serde_json::json!(exists));
            
            if exists {
                if let Ok(metadata) = std::fs::metadata(filename) {
                    result.insert("size".to_string(), serde_json::json!(metadata.len()));
                    result.insert("is_file".to_string(), serde_json::json!(metadata.is_file()));
                    result.insert("is_dir".to_string(), serde_json::json!(metadata.is_dir()));
                }
            }
            
            send_response(tx, msg, serde_json::Value::Object(result));
        } else {
            send_error_response(tx, msg, "Missing 'filename' field");
        }
    } else {
        send_error_response(tx, msg, "Missing data payload");
    }
}

#[cfg(feature = "webviewtk")]
fn handle_calculate(tx: &Sender<String>, msg: &IpcMessage) {
    if let Some(data) = &msg.data {
        let num1 = data.get("num1").and_then(|v| v.as_f64()).unwrap_or(0.0);
        let num2 = data.get("num2").and_then(|v| v.as_f64()).unwrap_or(0.0);
        let operation = data.get("operation").and_then(|v| v.as_str()).unwrap_or("+");
        
        let result = match operation {
            "+" => num1 + num2,
            "-" => num1 - num2,
            "*" => num1 * num2,
            "/" => {
                if num2 != 0.0 {
                    num1 / num2
                } else {
                    send_error_response(tx, msg, "Division by zero");
                    return;
                }
            }
            _ => {
                send_error_response(tx, msg, "Unknown operation");
                return;
            }
        };
        
        let mut response_data = serde_json::Map::new();
        response_data.insert("result".to_string(), serde_json::json!(result));
        response_data.insert("operation".to_string(), serde_json::json!(operation));
        response_data.insert("num1".to_string(), serde_json::json!(num1));
        response_data.insert("num2".to_string(), serde_json::json!(num2));
        
        send_response(tx, msg, serde_json::Value::Object(response_data));
    } else {
        send_error_response(tx, msg, "Missing data payload");
    }
}

#[cfg(feature = "webviewtk")]
fn handle_process_data(tx: &Sender<String>, msg: &IpcMessage) {
    if let Some(data) = &msg.data {
        let size = data.get("size").and_then(|v| v.as_u64()).unwrap_or(100);
        
        // Simulate some data processing
        let processed: Vec<u64> = (0..size).map(|i| i * 2).collect();
        let sum: u64 = processed.iter().sum();
        let avg = sum as f64 / processed.len() as f64;
        
        let mut result = serde_json::Map::new();
        result.insert("count".to_string(), serde_json::json!(processed.len()));
        result.insert("sum".to_string(), serde_json::json!(sum));
        result.insert("average".to_string(), serde_json::json!(avg));
        result.insert("sample".to_string(), serde_json::json!(&processed[..std::cmp::min(10, processed.len())]));
        
        send_response(tx, msg, serde_json::Value::Object(result));
    } else {
        send_error_response(tx, msg, "Missing data payload");
    }
}

#[cfg(feature = "webviewtk")]
fn handle_save_settings(tx: &Sender<String>, msg: &IpcMessage) {
    if let Some(data) = &msg.data {
        // In a real app, you'd save to a config file or database
        println!("💾 Saving settings: {:?}", data);
        
        let mut result = serde_json::Map::new();
        result.insert("saved".to_string(), serde_json::json!(true));
        result.insert("settings".to_string(), data.clone());
        
        send_response(tx, msg, serde_json::Value::Object(result));
    } else {
        send_error_response(tx, msg, "Missing data payload");
    }
}

/// Execute a Tauraro closure with arguments
#[cfg(feature = "webviewtk")]
fn execute_tauraro_closure(closure: Value, args: Vec<Value>) -> Result<Value> {
    use crate::vm::VM;
    use std::collections::HashMap;
    
    // Create a new VM instance for execution
    let mut vm = VM::new();
    
    // Call the closure with args using VM's call_function
    vm.call_function(closure, args)
}

/// Mount UI to window and run
#[cfg(feature = "webviewtk")]
pub fn mount_and_run(window: &HashMap<String, Value>, ui: &HashMap<String, Value>) -> Result<()> {
    let title = match window.get("title") {
        Some(Value::Str(s)) => s.clone(),
        _ => "WebViewTK App".to_string(),
    };
    
    let width = match window.get("width") {
        Some(Value::Float(f)) => *f as u32,
        Some(Value::Int(i)) => *i as u32,
        _ => 800,
    };
    
    let height = match window.get("height") {
        Some(Value::Float(f)) => *f as u32,
        Some(Value::Int(i)) => *i as u32,
        _ => 600,
    };
    
    let decorations = match window.get("decorations") {
        Some(Value::Bool(b)) => *b,
        _ => true,
    };
    
    let resizable = match window.get("resizable") {
        Some(Value::Bool(b)) => *b,
        _ => true,
    };
    
    let html = match ui.get("_html") {
        Some(Value::Str(s)) => s.clone(),
        _ => "<div>No content</div>".to_string(),
    };
    
    // Collect CDN links
    let mut cdn_tags = String::new();
    if let Some(Value::List(cdns)) = window.get("_cdns") {
        for cdn in cdns.data.borrow().iter() {
            if let Value::Str(url) = cdn {
                if url.ends_with(".css") {
                    cdn_tags.push_str(&format!("    <link rel=\"stylesheet\" href=\"{}\">\n", url));
                } else if url.contains("cdn.tailwindcss.com") {
                    // Tailwind Play CDN must load without defer to process classes immediately
                    cdn_tags.push_str(&format!("    <script src=\"{}\"></script>\n", url));
                } else if url.ends_with(".js") {
                    // Other JS files can use defer
                    cdn_tags.push_str(&format!("    <script src=\"{}\" defer></script>\n", url));
                }
            }
        }
    }
    
    // Collect custom CSS
    let mut custom_css = String::new();
    if let Some(Value::List(css_list)) = window.get("_custom_css") {
        for css in css_list.data.borrow().iter() {
            if let Value::Str(content) = css {
                custom_css.push_str(content);
                custom_css.push('\n');
            }
        }
    }
    
    // Inject SPA API (window.invoke for calling Tauraro functions)
    let spa_api = r#"
// Tauraro SPA API - Bidirectional communication between frontend and backend
(function() {
    let messageId = 0;
    const pendingInvocations = {};
    
    // Main API: Invoke Tauraro backend functions
    window.invoke = function(command, args = {}) {
        return new Promise((resolve, reject) => {
            const id = ++messageId;
            const message = JSON.stringify({
                type: 'invoke',
                id: id,
                command: command,
                args: args
            });
            
            // Store promise callbacks
            pendingInvocations[id] = { resolve, reject, command };
            
            // Send to backend
            window.ipc.postMessage(message);
            
            // Timeout after 30 seconds
            setTimeout(() => {
                if (pendingInvocations[id]) {
                    delete pendingInvocations[id];
                    reject(new Error(`Command '${command}' timed out after 30s`));
                }
            }, 30000);
        });
    };
    
    // Handle command responses from backend
    window.handleInvokeResponse = function(response) {
        if (pendingInvocations[response.id]) {
            const { resolve, reject } = pendingInvocations[response.id];
            delete pendingInvocations[response.id];
            
            if (response.success) {
                resolve(response.data);
            } else {
                reject(new Error(response.error || 'Unknown error'));
            }
        }
    };
    
    // Console logging helper
    window.log = console.log.bind(console);
    window.error = console.error.bind(console);
    
    console.log('✅ Tauraro SPA API ready - use window.invoke(command, args)');
})();
"#;
    
    // Collect custom JS
    let mut custom_js = String::new();
    custom_js.push_str(spa_api);
    custom_js.push('\n');
    
    if let Some(Value::List(js_list)) = window.get("_custom_js") {
        for js in js_list.data.borrow().iter() {
            if let Value::Str(content) = js {
                custom_js.push_str(content);
                custom_js.push('\n');
            }
        }
    }
    
    // Get custom HTML
    let custom_html = match window.get("_custom_html") {
        Some(Value::Str(s)) => s.clone(),
        _ => String::new(),
    };
    
    let full_html = format!(r#"
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>{}</title>
{}    <style>
        * {{ 
            margin: 0; 
            padding: 0; 
            box-sizing: border-box; 
        }}
        html, body {{
            width: 100%;
            height: 100%;
            overflow: hidden;
        }}
        body {{ 
            font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
            -webkit-font-smoothing: antialiased;
            -moz-osx-font-smoothing: grayscale;
        }}
        #root {{
            width: 100%;
            height: 100%;
            display: flex;
            flex-direction: column;
        }}
        /* Responsive utilities */
        .responsive-text {{
            font-size: clamp(0.875rem, 2vw, 1rem);
        }}
        .responsive-heading {{
            font-size: clamp(1.5rem, 4vw, 2.5rem);
        }}
        /* Smooth transitions */
        * {{
            transition: all 0.2s ease;
        }}
        /* Better scrollbars on Windows */
        ::-webkit-scrollbar {{
            width: 8px;
            height: 8px;
        }}
        ::-webkit-scrollbar-track {{
            background: #f1f1f1;
        }}
        ::-webkit-scrollbar-thumb {{
            background: #888;
            border-radius: 4px;
        }}
        ::-webkit-scrollbar-thumb:hover {{
            background: #555;
        }}
        {}
    </style>
    <script>
        {}
    </script>
</head>
<body>
{}    <div id="root">{}</div>
</body>
</html>
"#, title, cdn_tags, custom_css, custom_js, custom_html, html);
    
    // Extract user commands BEFORE creating WRY window (which will shadow the window parameter)
    let user_commands_opt = window.get("_commands").cloned();
    
    let event_loop = EventLoop::new();
    let wry_window = WindowBuilder::new()
        .with_title(&title)
        .with_inner_size(wry::application::dpi::LogicalSize::new(width, height))
        .with_decorations(decorations)
        .with_resizable(resizable)
        .build(&event_loop)?;
    
    let window_id = wry_window.id();
    
    // Create a shared reference to track window state
    let is_maximized = Arc::new(Mutex::new(false));
    let is_maximized_clone = is_maximized.clone();
    
    // Create channel for sending responses from IPC handlers to webview (using crossbeam for better performance)
    let (tx, rx) = unbounded::<String>();
    
    // Create command registry for exposing Tauraro functions to frontend
    let commands = super::command_registry::CommandRegistry::new();
    
    // Register built-in commands
    register_builtin_commands(&commands);
    
    // Create a crossbeam channel for executing Tauraro closures
    // Commands run in IPC thread, but closures need to run in main thread with VM access
    // crossbeam is thread-safe and doesn't require Arc<Mutex<>>
    let (closure_tx, closure_rx) = unbounded::<(String, serde_json::Value, Sender<Result<serde_json::Value, String>>)>();
    
    // Clone user commands for closure storage
    let user_commands_for_executor = user_commands_opt.clone();
    
    // Register user-defined Tauraro commands
    if let Some(Value::Dict(user_commands)) = user_commands_opt {
        for (name, func) in user_commands.borrow().iter() {
            match func {
                Value::NativeFunction(f) => {
                    let func_ptr = *f; // Copy the function pointer (which is thread-safe)
                    
                    commands.register(&name, move |args| {
                        // Convert JSON args to Tauraro Values
                        use super::command_registry::{json_to_tauraro_values, tauraro_value_to_json};
                        
                        let tauraro_args = json_to_tauraro_values(&args)?;
                        
                        // Call the Tauraro native function
                        let result = func_ptr(tauraro_args)?;
                        
                        // Convert result back to JSON
                        tauraro_value_to_json(&result)
                    });
                    
                    println!("📡 Mounted native function command: {}", name);
                }
                Value::Closure { .. } => {
                    // For closures, register a handler that sends execution request to main thread
                    let name_clone = name.clone();
                    let closure_tx_clone = closure_tx.clone(); // crossbeam Sender is Clone!
                    
                    commands.register(&name, move |args| {
                        println!("🔵 IPC thread: Received invoke for closure '{}'", name_clone);
                        
                        // Send execution request to main thread
                        let (result_tx, result_rx) = unbounded();
                        
                        println!("🔵 IPC thread: Attempting to send execution request via channel...");
                        match closure_tx_clone.send((name_clone.clone(), args.clone(), result_tx)) {
                            Ok(_) => println!("✅ IPC thread: Successfully sent execution request to event loop"),
                            Err(e) => {
                                println!("❌ IPC thread: Failed to send: {}", e);
                                return Err(anyhow::anyhow!("Failed to send closure execution request: {}", e));
                            }
                        }
                        
                        // Return immediately - don't block!
                        // The event loop will send the result back asynchronously via JavaScript
                        use std::time::Duration;
                        use std::thread;
                        
                        // Try to get result quickly (non-blocking attempt)
                        for _ in 0..50 {
                            match result_rx.try_recv() {
                                Ok(Ok(result)) => return Ok(result),
                                Ok(Err(e)) => return Err(anyhow::anyhow!("Closure execution error: {}", e)),
                                Err(crossbeam::channel::TryRecvError::Disconnected) => {
                                    return Err(anyhow::anyhow!("Channel disconnected"));
                                }
                                Err(crossbeam::channel::TryRecvError::Empty) => {
                                    thread::sleep(Duration::from_millis(1));
                                }
                            }
                        }
                        
                        // If not ready after 50ms, return pending status
                        // JavaScript will need to poll or we need to implement callbacks
                        Err(anyhow::anyhow!("Closure execution pending (check console for completion)"))
                    });
                    
                    println!("📡 Mounted Tauraro closure command: {}", name);
                }
                _ => {
                    println!("⚠️  Warning: '{}' is not a function, skipping registration", name);
                }
            }
        }
    }
    
    let tx_clone = tx.clone();
    let webview = WebViewBuilder::new(wry_window)?
        .with_html(&full_html)?
        .with_ipc_handler(move |window, message| {
            // Handle custom titlebar button clicks and drag
            if message.starts_with("window:drag_start") {
                // Initiate window drag
                let _ = window.drag_window();
            } else if message.starts_with("window:drag_move") || message.starts_with("window:drag_end") {
                // These are handled by the drag_window() call
            } else if message.starts_with("window:") {
                // Window control messages
                match message.as_str() {
                    "window:close" => {
                        std::process::exit(0);
                    }
                    "window:minimize" => {
                        window.set_minimized(true);
                    }
                    "window:maximize" => {
                        let mut maximized = is_maximized_clone.lock().unwrap();
                        *maximized = !*maximized;
                        window.set_maximized(*maximized);
                    }
                    _ => {}
                }
            } else {
                // Custom IPC messages - parse JSON and handle
                handle_ipc_message(&tx_clone, &message, &commands);
            }
        })
        .build()?;
    
    // Debug: Add a flag to log only once
    use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering};
    let logged_event_loop_start = Arc::new(AtomicBool::new(false));
    let logged_event_loop_start_clone = Arc::clone(&logged_event_loop_start);
    let loop_iteration_count = Arc::new(AtomicUsize::new(0));
    let loop_iteration_count_clone = Arc::clone(&loop_iteration_count);
    
    println!("🔁 Starting event loop...");
    println!("🔁 closure_rx channel ready for receiving");
    
    let iteration_count = Arc::new(AtomicUsize::new(0));
    let iteration_count_clone = iteration_count.clone();
    
    event_loop.run(move |event, _, control_flow| {
        // Count iterations
        let count = iteration_count_clone.fetch_add(1, Ordering::Relaxed);
        
        // Log closure_rx status once
        static mut LOGGED_RX: bool = false;
        unsafe {
            if !LOGGED_RX {
                println!("🔍 Event loop has closure_rx: checking if disconnected...");
                // Try to peek at the channel status
                match closure_rx.try_recv() {
                    Err(crossbeam::channel::TryRecvError::Disconnected) => {
                        println!("❌ CRITICAL: closure_rx is DISCONNECTED at event loop start!");
                    }
                    Err(crossbeam::channel::TryRecvError::Empty) => {
                        println!("✅ closure_rx is connected and empty (normal)");
                    }
                    Ok(_) => {
                        println!("✅ closure_rx already has messages");
                    }
                }
                LOGGED_RX = true;
            }
        }
        
        // Log iterations periodically to debug timing
        if count % 10000 == 0 && count > 0 {
            println!("🔁 Event loop at iteration: {}", count);
        }
        
        // Use Poll for immediate message processing (responsive backend at cost of CPU)
        *control_flow = ControlFlow::Poll;
        
        // Check for pending scripts to evaluate
        if let Ok(script) = rx.try_recv() {
            let _ = webview.evaluate_script(&script);
        }
        
        // Check closure channel multiple times per iteration for better responsiveness
        // This compensates for slow event loop iterations
        for _ in 0..10 {
            if let Ok((command_name, args, result_tx)) = closure_rx.try_recv() {
                println!("🔄 Event loop (iteration {}): Received closure execution request: {}", count, command_name);
            
                // Execute the closure and send result back
            let result = if let Some(Value::Dict(ref cmds)) = user_commands_for_executor {
                if let Some(func) = cmds.borrow().get(&command_name) {
                    println!("✓ Found closure function: {}", command_name);
                    
                    // Execute the closure
                    use super::command_registry::{json_to_tauraro_values, tauraro_value_to_json};
                    
                    match json_to_tauraro_values(&args) {
                        Ok(tauraro_args) => {
                            println!("✓ Converted args, executing closure with VM...");
                            
                            // Call the closure
                            match func {
                                Value::Closure { .. } => {
                                    // Create a VM to execute the closure
                                    match execute_tauraro_closure(func.clone(), tauraro_args) {
                                        Ok(result_value) => {
                                            println!("✓ Closure executed successfully: {:?}", result_value);
                                            
                                            match tauraro_value_to_json(&result_value) {
                                                Ok(json) => {
                                                    println!("✓ Converted result to JSON");
                                                    Ok(json)
                                                }
                                                Err(e) => {
                                                    println!("✗ Failed to convert result to JSON: {}", e);
                                                    Err(format!("Failed to convert result to JSON: {}", e))
                                                }
                                            }
                                        }
                                        Err(e) => {
                                            println!("✗ Closure execution failed: {}", e);
                                            Err(format!("Closure execution failed: {}", e))
                                        }
                                    }
                                }
                                _ => {
                                    println!("✗ Not a closure");
                                    Err(format!("'{}' is not a closure", command_name))
                                }
                            }
                        }
                        Err(e) => {
                            println!("✗ Failed to convert args: {}", e);
                            Err(format!("Failed to convert args: {}", e))
                        }
                    }
                } else {
                    println!("✗ Command not found: {}", command_name);
                    Err(format!("Command '{}' not found", command_name))
                }
            } else {
                println!("✗ No commands registered");
                Err("No commands registered".to_string())
            };
            
                println!("📤 Sending result back to IPC thread...");
                // Send result back (ignore send errors if receiver dropped)
                let _ = result_tx.send(result);
            } else {
                // No message available, break out of check loop
                break;
            }
        }
        
        match event {
            Event::WindowEvent {
                event: WindowEvent::CloseRequested,
                window_id: id,
                ..
            } if id == window_id => *control_flow = ControlFlow::Exit,
            _ => {}
        }
    });
}

#[cfg(not(feature = "webviewtk"))]
pub fn mount_and_run(_window: &HashMap<String, Value>, _ui: &HashMap<String, Value>) -> Result<()> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

/// Wrapper function for calling from Tauraro
#[cfg(feature = "webviewtk")]
pub fn mount_and_run_wrapper(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("mount_and_run requires 2 arguments: window and ui"));
    }
    
    let window_dict = match &args[0] {
        Value::Dict(d) => d.borrow().clone(),
        _ => return Err(anyhow::anyhow!("First argument must be a Window dict")),
    };
    
    let ui_dict = match &args[1] {
        Value::Dict(d) => d.borrow().clone(),
        _ => return Err(anyhow::anyhow!("Second argument must be a UI dict")),
    };
    
    mount_and_run(&window_dict, &ui_dict)?;
    Ok(Value::None)
}

#[cfg(not(feature = "webviewtk"))]
pub fn mount_and_run_wrapper(_args: Vec<Value>) -> Result<Value> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

/// Add CDN links to window
#[cfg(feature = "webviewtk")]
pub fn include_cdn_wrapper(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("include_cdn requires 2 arguments: window and cdn_url(s)"));
    }
    
    let window_dict = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow::anyhow!("First argument must be a Window dict")),
    };
    
    let mut dict = window_dict.borrow_mut();
    let cdns = match dict.get_mut("_cdns") {
        Some(Value::List(l)) => l.data.clone(),
        _ => return Err(anyhow::anyhow!("Window dict missing _cdns")),
    };
    
    // Support single CDN or list of CDNs
    if let Value::List(cdn_list) = &args[1] {
        for cdn in cdn_list.data.borrow().iter() {
            if let Value::Str(url) = cdn {
                cdns.borrow_mut().push(Value::Str(url.clone()));
            }
        }
    } else if let Value::Str(url) = &args[1] {
        cdns.borrow_mut().push(Value::Str(url.clone()));
    }
    
    Ok(Value::None)
}

#[cfg(not(feature = "webviewtk"))]
pub fn include_cdn_wrapper(_args: Vec<Value>) -> Result<Value> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

/// Include CSS file
#[cfg(feature = "webviewtk")]
pub fn include_css_file_wrapper(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("include_css_file requires 2 arguments: window and file_path"));
    }
    
    let window_dict = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow::anyhow!("First argument must be a Window dict")),
    };
    
    let file_path = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("Second argument must be a file path string")),
    };
    
    // Read file content
    let path = Path::new(file_path);
    let content = fs::read_to_string(path)
        .map_err(|e| anyhow::anyhow!("Failed to read CSS file '{}': {}", file_path, e))?;
    
    let mut dict = window_dict.borrow_mut();
    let css_list = match dict.get_mut("_custom_css") {
        Some(Value::List(l)) => l.data.clone(),
        _ => return Err(anyhow::anyhow!("Window dict missing _custom_css")),
    };
    
    css_list.borrow_mut().push(Value::Str(content));
    
    Ok(Value::None)
}

#[cfg(not(feature = "webviewtk"))]
pub fn include_css_file_wrapper(_args: Vec<Value>) -> Result<Value> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

/// Include JavaScript file
#[cfg(feature = "webviewtk")]
pub fn include_js_file_wrapper(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("include_js_file requires 2 arguments: window and file_path"));
    }
    
    let window_dict = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow::anyhow!("First argument must be a Window dict")),
    };
    
    let file_path = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("Second argument must be a file path string")),
    };
    
    // Read file content
    let path = Path::new(file_path);
    let content = fs::read_to_string(path)
        .map_err(|e| anyhow::anyhow!("Failed to read JS file '{}': {}", file_path, e))?;
    
    let mut dict = window_dict.borrow_mut();
    let js_list = match dict.get_mut("_custom_js") {
        Some(Value::List(l)) => l.data.clone(),
        _ => return Err(anyhow::anyhow!("Window dict missing _custom_js")),
    };
    
    js_list.borrow_mut().push(Value::Str(content));
    
    Ok(Value::None)
}

#[cfg(not(feature = "webviewtk"))]
pub fn include_js_file_wrapper(_args: Vec<Value>) -> Result<Value> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

/// Include HTML file (inserted at top of body)
#[cfg(feature = "webviewtk")]
pub fn include_html_file_wrapper(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("include_html_file requires 2 arguments: window and file_path"));
    }
    
    let window_dict = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow::anyhow!("First argument must be a Window dict")),
    };
    
    let file_path = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("Second argument must be a file path string")),
    };
    
    // Read file content
    let path = Path::new(file_path);
    let content = fs::read_to_string(path)
        .map_err(|e| anyhow::anyhow!("Failed to read HTML file '{}': {}", file_path, e))?;
    
    let mut dict = window_dict.borrow_mut();
    if let Some(Value::Str(existing)) = dict.get_mut("_custom_html") {
        existing.push_str(&content);
    }
    
    Ok(Value::None)
}

#[cfg(not(feature = "webviewtk"))]
pub fn include_html_file_wrapper(_args: Vec<Value>) -> Result<Value> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

/// Add custom CSS code
#[cfg(feature = "webviewtk")]
pub fn add_custom_css_wrapper(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("add_custom_css requires 2 arguments: window and css_code"));
    }
    
    let window_dict = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow::anyhow!("First argument must be a Window dict")),
    };
    
    let css_code = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("Second argument must be a CSS code string")),
    };
    
    let mut dict = window_dict.borrow_mut();
    let css_list = match dict.get_mut("_custom_css") {
        Some(Value::List(l)) => l.data.clone(),
        _ => return Err(anyhow::anyhow!("Window dict missing _custom_css")),
    };
    
    css_list.borrow_mut().push(Value::Str(css_code.clone()));
    
    Ok(Value::None)
}

#[cfg(not(feature = "webviewtk"))]
pub fn add_custom_css_wrapper(_args: Vec<Value>) -> Result<Value> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

/// Add custom JavaScript code
#[cfg(feature = "webviewtk")]
pub fn add_custom_js_wrapper(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("add_custom_js requires 2 arguments: window and js_code"));
    }
    
    let window_dict = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow::anyhow!("First argument must be a Window dict")),
    };
    
    let js_code = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("Second argument must be a JS code string")),
    };
    
    let mut dict = window_dict.borrow_mut();
    let js_list = match dict.get_mut("_custom_js") {
        Some(Value::List(l)) => l.data.clone(),
        _ => return Err(anyhow::anyhow!("Window dict missing _custom_js")),
    };
    
    js_list.borrow_mut().push(Value::Str(js_code.clone()));
    
    Ok(Value::None)
}

#[cfg(not(feature = "webviewtk"))]
pub fn add_custom_js_wrapper(_args: Vec<Value>) -> Result<Value> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

/// Register a Tauraro function as a backend command
/// Usage: register_command(window, "command_name", tauraro_function)
#[cfg(feature = "webviewtk")]
pub fn register_command_wrapper(args: Vec<Value>) -> Result<Value> {
    if args.len() < 3 {
        return Err(anyhow::anyhow!("register_command requires 3 arguments: window, command_name, and function"));
    }
    
    let window_dict = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow::anyhow!("First argument must be a Window dict")),
    };
    
    let command_name = match &args[1] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow::anyhow!("Second argument must be a command name string")),
    };
    
    let function = args[2].clone();
    
    // Validate that it's a callable
    match &function {
        Value::Closure { .. } | Value::NativeFunction(_) => {},
        _ => return Err(anyhow::anyhow!("Third argument must be a function or closure")),
    }
    
    // Store the command in the window's command registry
    let mut dict = window_dict.borrow_mut();
    let commands_map = match dict.get_mut("_commands") {
        Some(Value::Dict(d)) => d.clone(),
        _ => {
            // Create commands map if it doesn't exist
            let new_map = Rc::new(RefCell::new(HashMap::new()));
            dict.insert("_commands".to_string(), Value::Dict(new_map.clone()));
            new_map
        }
    };
    
    commands_map.insert(command_name.clone(), function);
    
    println!("✅ Registered Tauraro command: {}", command_name);
    
    Ok(Value::None)
}

#[cfg(not(feature = "webviewtk"))]
pub fn register_command_wrapper(_args: Vec<Value>) -> Result<Value> {
    Err(anyhow::anyhow!("webviewtk feature not enabled"))
}

