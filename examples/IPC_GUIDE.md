# IPC (Inter-Process Communication) in Tauraro WebViewTK

This guide explains how to use IPC to communicate between the frontend (JavaScript) and backend (Tauraro/Rust).

## Overview

IPC allows your JavaScript frontend to send messages to the Tauraro backend, enabling:
- Backend data processing
- File system operations
- System information access
- Database queries
- Any server-side logic

## How It Works

### 1. Frontend → Backend (Currently Supported)

**JavaScript sends messages:**
```javascript
window.ipc.postMessage(JSON.stringify({
    action: "calculate",
    data: { num1: 10, num2: 5, operation: "+" }
}));
```

**Backend receives in `src/modules/webviewtk/window.rs`:**
```rust
.with_ipc_handler(move |window, message| {
    // message contains the JSON string
    println!("Received IPC message: {}", message);
    
    // Parse and handle the message
    // TODO: Add your custom logic here
})
```

### 2. Backend → Frontend (Future Enhancement)

To send responses back to the frontend, you can use WebView evaluation:

```rust
// In the IPC handler:
window.evaluate_script(&format!(
    "window.handleBackendResponse({})", 
    response_json
)).ok();
```

## Examples

### Example 1: Simple Message

**Frontend:**
```javascript
window.ipc.postMessage(JSON.stringify({
    action: "hello",
    data: { name: "World" }
}));
```

**Backend (Terminal Output):**
```
Received IPC message: {"action":"hello","data":{"name":"World"}}
```

### Example 2: File Check

**Frontend:**
```javascript
window.ipc.postMessage(JSON.stringify({
    action: "file_check",
    data: { filename: "README.md" }
}));
```

**Backend Handler (Add to window.rs):**
```rust
use std::fs;

.with_ipc_handler(move |window, message| {
    if let Ok(json) = serde_json::from_str::<serde_json::Value>(&message) {
        if json["action"] == "file_check" {
            let filename = json["data"]["filename"].as_str().unwrap_or("");
            let exists = fs::metadata(filename).is_ok();
            
            // Send response back
            let response = format!(r#"{{
                "action": "file_check_result",
                "exists": {},
                "filename": "{}"
            }}"#, exists, filename);
            
            window.evaluate_script(&format!(
                "window.handleBackendResponse({})", 
                response
            )).ok();
        }
    }
})
```

### Example 3: Calculator

**Frontend:**
```javascript
window.ipc.postMessage(JSON.stringify({
    action: "calculate",
    data: { num1: 10, num2: 5, operation: "+" }
}));
```

**Backend Handler:**
```rust
if json["action"] == "calculate" {
    let num1 = json["data"]["num1"].as_f64().unwrap_or(0.0);
    let num2 = json["data"]["num2"].as_f64().unwrap_or(0.0);
    let op = json["data"]["operation"].as_str().unwrap_or("+");
    
    let result = match op {
        "+" => num1 + num2,
        "-" => num1 - num2,
        "*" => num1 * num2,
        "/" => if num2 != 0.0 { num1 / num2 } else { 0.0 },
        _ => 0.0
    };
    
    let response = format!(r#"{{
        "action": "calculate_result",
        "result": {}
    }}"#, result);
    
    window.evaluate_script(&format!(
        "window.handleBackendResponse({})", 
        response
    )).ok();
}
```

## Running the Demo

```bash
# Make sure you've built with webviewtk feature
cargo build --features webviewtk

# Run the IPC demo
./target/debug/tauraro run examples/ipc_advanced_demo.tr
```

## Demo Features

The `ipc_advanced_demo.tr` example includes:

1. **System Information** - Request OS/platform details
2. **File Check** - Check if a file exists
3. **Calculator** - Perform backend calculations
4. **Data Processing** - Process arrays/data structures
5. **Settings** - Save user preferences

## Current Limitations

- ✅ Frontend → Backend: **Fully working**
- ⚠️ Backend → Frontend: **Requires manual implementation** (see examples above)
- ⚠️ IPC messages are currently only logged to terminal
- ⚠️ No built-in JSON parsing in IPC handler

## Extending the IPC System

### Step 1: Add JSON Parsing

Add to `Cargo.toml`:
```toml
[dependencies]
serde = { version = "1.0", features = ["derive"] }
serde_json = "1.0"
```

### Step 2: Create Message Types

```rust
use serde::{Deserialize, Serialize};

#[derive(Deserialize)]
struct IpcMessage {
    action: String,
    data: serde_json::Value,
}

#[derive(Serialize)]
struct IpcResponse {
    action: String,
    data: serde_json::Value,
}
```

### Step 3: Implement Handler Router

```rust
.with_ipc_handler(move |window, message| {
    if message.starts_with("window:") {
        // Handle window control messages
        // ... existing code ...
    } else {
        // Handle custom IPC messages
        if let Ok(ipc_msg) = serde_json::from_str::<IpcMessage>(&message) {
            match ipc_msg.action.as_str() {
                "file_check" => handle_file_check(window, ipc_msg.data),
                "calculate" => handle_calculate(window, ipc_msg.data),
                "system_info" => handle_system_info(window),
                _ => eprintln!("Unknown IPC action: {}", ipc_msg.action)
            }
        }
    }
})
```

### Step 4: Add Helper Functions

```rust
fn handle_file_check(window: &wry::application::window::Window, data: serde_json::Value) {
    let filename = data["filename"].as_str().unwrap_or("");
    let exists = std::fs::metadata(filename).is_ok();
    
    send_response(window, "file_check_result", json!({
        "exists": exists,
        "filename": filename
    }));
}

fn send_response(window: &wry::application::window::Window, action: &str, data: serde_json::Value) {
    let response = IpcResponse {
        action: action.to_string(),
        data,
    };
    
    if let Ok(json) = serde_json::to_string(&response) {
        let script = format!("if(window.handleBackendResponse) {{ window.handleBackendResponse({}); }}", json);
        window.evaluate_script(&script).ok();
    }
}
```

## Best Practices

1. **Always validate input** - Check message structure before processing
2. **Use structured data** - Prefer JSON over plain strings
3. **Handle errors gracefully** - Don't crash on invalid messages
4. **Keep handlers async-friendly** - Long operations should not block
5. **Log for debugging** - Print messages during development

## Security Considerations

- **Never trust frontend input** - Always validate and sanitize
- **Limit file system access** - Use allowed paths/directories
- **Rate limit messages** - Prevent flooding from malicious frontends
- **Sanitize responses** - Don't expose sensitive system information

## Further Reading

- [WRY IPC Documentation](https://docs.rs/wry/latest/wry/)
- [Tauri IPC Patterns](https://tauri.app/v1/guides/features/command)
- [WebView2 Message Passing](https://docs.microsoft.com/en-us/microsoft-edge/webview2/)

---

**Note**: The current IPC implementation is basic and logs messages to the terminal. Extending it to handle responses and complex workflows requires modifications to `src/modules/webviewtk/window.rs`.
