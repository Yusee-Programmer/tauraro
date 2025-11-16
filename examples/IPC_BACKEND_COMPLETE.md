# Tauraro IPC - Complete Backend Implementation ✅

## Overview

Tauraro WebViewTK now has **full bidirectional IPC communication** between frontend (JavaScript) and backend (Rust). Messages flow in both directions with JSON-based structured data.

## Architecture

```
┌─────────────────┐                    ┌──────────────────┐
│   Frontend      │                    │     Backend      │
│   JavaScript    │                    │   Rust/Tauraro   │
├─────────────────┤                    ├──────────────────┤
│                 │  ─── message ──>   │  IPC Handler     │
│  window.ipc     │                    │  Parse JSON      │
│  .postMessage() │                    │  Route Action    │
│                 │                    │  Execute Logic   │
│                 │  <── response ───  │  Send Response   │
│  handleBackend  │                    │  (via channel)   │
│  Response()     │                    │                  │
└─────────────────┘                    └──────────────────┘
```

## Message Format

### Request (Frontend → Backend)
```json
{
  "id": 1,
  "action": "system_info",
  "data": { "request": "details" },
  "timestamp": "2024-11-16T10:30:00Z"
}
```

### Response (Backend → Frontend)
```json
{
  "id": 1,
  "action": "system_info",
  "success": true,
  "data": {
    "os": "windows",
    "arch": "x86_64",
    "hostname": "mycomputer"
  },
  "error": null
}
```

## Built-in IPC Actions

The backend in `window.rs` currently handles these actions:

### 1. `system_info` - Get System Information
**Request:**
```javascript
sendToBackend('system_info', { request: 'details' });
```

**Response:**
```json
{
  "os": "windows",
  "arch": "x86_64",
  "family": "windows",
  "hostname": "DESKTOP-ABC123"
}
```

### 2. `file_check` - Check File Existence
**Request:**
```javascript
sendToBackend('file_check', { filename: 'README.md' });
```

**Response:**
```json
{
  "filename": "README.md",
  "exists": true,
  "size": 2048,
  "is_file": true,
  "is_dir": false
}
```

### 3. `calculate` - Backend Calculator
**Request:**
```javascript
sendToBackend('calculate', { 
  num1: 10, 
  num2: 5, 
  operation: '+' 
});
```

**Response:**
```json
{
  "result": 15,
  "operation": "+",
  "num1": 10,
  "num2": 5
}
```

Supported operations: `+`, `-`, `*`, `/`

### 4. `process_data` - Data Processing
**Request:**
```javascript
sendToBackend('process_data', { size: 100 });
```

**Response:**
```json
{
  "count": 100,
  "sum": 9900,
  "average": 99.0,
  "sample": [0, 2, 4, 6, 8, 10, 12, 14, 16, 18]
}
```

### 5. `save_settings` - Save Configuration
**Request:**
```javascript
sendToBackend('save_settings', {
  theme: 'dark',
  notifications: true,
  autoSave: true
});
```

**Response:**
```json
{
  "saved": true,
  "settings": {
    "theme": "dark",
    "notifications": true,
    "autoSave": true
  }
}
```

## Frontend Implementation

### Sending Messages
```javascript
// Helper function to send IPC messages
window.sendToBackend = function(action, data) {
    const message = JSON.stringify({
        id: messageCounter++,
        action: action,
        data: data,
        timestamp: new Date().toISOString()
    });
    
    window.ipc.postMessage(message);
};

// Usage
sendToBackend('system_info', {});
sendToBackend('file_check', { filename: 'data.txt' });
```

### Receiving Responses
```javascript
// Handler for backend responses
window.handleBackendResponse = function(response) {
    console.log('Response received:', response);
    
    if (response.success) {
        // Handle successful response
        console.log('Data:', response.data);
    } else {
        // Handle error
        console.error('Error:', response.error);
    }
};
```

## Backend Implementation (window.rs)

### IPC Handler Structure
```rust
.with_ipc_handler(move |window, message| {
    if message.starts_with("window:") {
        // Window control messages (minimize, maximize, close)
        handle_window_controls(window, message);
    } else {
        // Custom IPC messages
        handle_ipc_message(&tx, &message);
    }
})
```

### Message Routing
```rust
fn handle_ipc_message(tx: &Sender<String>, message: &str) {
    match serde_json::from_str::<IpcMessage>(message) {
        Ok(ipc_msg) => {
            match ipc_msg.action.as_str() {
                "system_info" => handle_system_info(tx, &ipc_msg),
                "file_check" => handle_file_check(tx, &ipc_msg),
                "calculate" => handle_calculate(tx, &ipc_msg),
                "process_data" => handle_process_data(tx, &ipc_msg),
                "save_settings" => handle_save_settings(tx, &ipc_msg),
                _ => send_error_response(tx, &ipc_msg, "Unknown action")
            }
        }
        Err(e) => eprintln!("Failed to parse IPC message: {}", e)
    }
}
```

### Sending Responses
```rust
fn send_response(tx: &Sender<String>, msg: &IpcMessage, data: serde_json::Value) {
    let response = IpcResponse {
        id: msg.id,
        action: msg.action.clone(),
        success: true,
        data: Some(data),
        error: None,
    };
    
    if let Ok(json) = serde_json::to_string(&response) {
        let script = format!(
            "if (window.handleBackendResponse) {{ window.handleBackendResponse({}); }}", 
            json
        );
        let _ = tx.send(script);
    }
}
```

## Adding Custom IPC Actions

### 1. Add Handler Function
```rust
#[cfg(feature = "webviewtk")]
fn handle_custom_action(tx: &Sender<String>, msg: &IpcMessage) {
    if let Some(data) = &msg.data {
        // Your custom logic here
        let result = perform_custom_operation(data);
        
        // Send response
        send_response(tx, msg, serde_json::json!({
            "status": "success",
            "result": result
        }));
    } else {
        send_error_response(tx, msg, "Missing data payload");
    }
}
```

### 2. Add to Router
```rust
fn handle_ipc_message(tx: &Sender<String>, message: &str) {
    match serde_json::from_str::<IpcMessage>(message) {
        Ok(ipc_msg) => {
            match ipc_msg.action.as_str() {
                // ... existing actions ...
                "custom_action" => handle_custom_action(tx, &ipc_msg),
                _ => send_error_response(tx, &ipc_msg, "Unknown action")
            }
        }
        // ...
    }
}
```

### 3. Call from Frontend
```javascript
sendToBackend('custom_action', {
    param1: 'value1',
    param2: 'value2'
});
```

## Error Handling

### Backend Error Responses
```rust
send_error_response(tx, msg, "File not found");
```

Generates:
```json
{
  "id": 1,
  "action": "file_check",
  "success": false,
  "data": null,
  "error": "File not found"
}
```

### Frontend Error Display
```javascript
window.handleBackendResponse = function(response) {
    if (!response.success) {
        alert(`Error in ${response.action}: ${response.error}`);
    }
};
```

## Security Considerations

1. **Input Validation**: Always validate frontend input in backend handlers
2. **File System Access**: Limit file operations to safe directories
3. **Rate Limiting**: Consider rate limiting IPC messages
4. **Error Messages**: Don't expose sensitive system information in errors
5. **Path Traversal**: Sanitize file paths to prevent directory traversal attacks

## Example: Complete IPC Flow

### 1. User Action
User clicks "Check File" button

### 2. Frontend Sends Request
```javascript
sendToBackend('file_check', { filename: 'config.json' });
```

### 3. Backend Receives
```
📨 Received IPC message: action=file_check, id=Some(1)
```

### 4. Backend Processes
```rust
fn handle_file_check(tx: &Sender<String>, msg: &IpcMessage) {
    let filename = msg.data.get("filename").and_then(|v| v.as_str());
    let exists = std::path::Path::new(filename).exists();
    
    send_response(tx, msg, serde_json::json!({
        "filename": filename,
        "exists": exists
    }));
}
```

### 5. Backend Sends Response
```
✅ Sent response for action: file_check
```

### 6. Frontend Receives
```javascript
window.handleBackendResponse({
    id: 1,
    action: "file_check",
    success: true,
    data: { filename: "config.json", exists: true }
});
```

### 7. UI Updates
```javascript
document.getElementById('result').textContent = 
    "File config.json exists: true";
```

## Testing

Run the IPC demo:
```bash
cargo build --features webviewtk
./target/debug/tauraro run examples/ipc_advanced_demo.tr
```

Test all 5 actions and observe:
- ✅ Messages sent from frontend
- ✅ Backend processing in terminal
- ✅ Responses displayed in UI
- ✅ Activity log tracking all events

## Further Reading

- [WRY IPC Documentation](https://docs.rs/wry/latest/wry/)
- [Serde JSON Guide](https://docs.rs/serde_json/)
- [Examples: ipc_advanced_demo.tr](./ipc_advanced_demo.tr)
