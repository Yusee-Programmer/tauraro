# Exposing Tauraro Functions to Frontend

This guide explains how to **expose Tauraro functions** to the JavaScript frontend and enable **bidirectional data exchange**.

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                      Frontend (JavaScript)                   │
│  ┌────────────────────────────────────────────────────────┐ │
│  │  window.invoke('myFunction', {param1: value1})        │ │
│  │         ↓                                               │ │
│  │  Returns Promise with result                           │ │
│  └────────────────────────────────────────────────────────┘ │
└──────────────────────────┬──────────────────────────────────┘
                           │ IPC Channel
                           ↓
┌─────────────────────────────────────────────────────────────┐
│                    Backend (Tauraro/Rust)                    │
│  ┌────────────────────────────────────────────────────────┐ │
│  │  CommandRegistry                                       │ │
│  │    ├─ "myFunction" → Tauraro function                 │ │
│  │    ├─ "calculate" → Math operations                   │ │
│  │    └─ "getData" → Database queries                    │ │
│  └────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

## Implementation

### 1. Command Registry System

The `CommandRegistry` allows you to:
- ✅ Register Tauraro functions with names
- ✅ Call them from frontend with parameters
- ✅ Get results back as promises
- ✅ Automatic type conversion (JSON ↔ Tauraro Values)

### 2. Integrating with Window.rs

```rust
// In src/modules/webviewtk/window.rs

use super::command_registry::CommandRegistry;

// Create command registry
let commands = CommandRegistry::new();

// Register your Tauraro functions
commands.register_tauraro_function("add", |args| {
    let a = match &args[0] {
        Value::Int(i) => *i as f64,
        Value::Float(f) => *f,
        _ => return Err(anyhow::anyhow!("Expected number")),
    };
    let b = match &args[1] {
        Value::Int(i) => *i as f64,
        Value::Float(f) => *f,
        _ => return Err(anyhow::anyhow!("Expected number")),
    };
    Ok(Value::Float(a + b))
});

// In IPC handler
if message.starts_with("invoke:") {
    let command_json: IpcCommand = serde_json::from_str(&message[7..])?;
    let result = commands.invoke(&command_json.command, command_json.args)?;
    send_response(tx, &command_json, result);
}
```

### 3. Frontend JavaScript API

```javascript
// Auto-injected API
window.invoke = async function(command, args = {}) {
    return new Promise((resolve, reject) => {
        const id = ++messageId;
        const message = JSON.stringify({
            type: 'invoke',
            id: id,
            command: command,
            args: args
        });
        
        // Store callback
        pendingInvocations[id] = { resolve, reject };
        
        // Send to backend
        window.ipc.postMessage(message);
        
        // Timeout after 30 seconds
        setTimeout(() => {
            if (pendingInvocations[id]) {
                delete pendingInvocations[id];
                reject(new Error('Command timeout'));
            }
        }, 30000);
    });
};

// Handle responses
window.handleInvokeResponse = function(response) {
    if (pendingInvocations[response.id]) {
        const { resolve, reject } = pendingInvocations[response.id];
        delete pendingInvocations[response.id];
        
        if (response.success) {
            resolve(response.data);
        } else {
            reject(new Error(response.error));
        }
    }
};
```

## Usage Examples

### Example 1: Simple Function Call

**Tauraro (Backend):**
```python
# Register function
def greet(name):
    return f"Hello, {name}!"

window.register_command("greet", greet)
```

**JavaScript (Frontend):**
```javascript
const result = await window.invoke('greet', {name: 'Alice'});
console.log(result); // "Hello, Alice!"
```

### Example 2: Database Query

**Tauraro (Backend):**
```python
def get_users(filters):
    # Query database
    users = db.query("SELECT * FROM users WHERE age > ?", [filters['min_age']])
    return [{'id': u.id, 'name': u.name} for u in users]

window.register_command("getUsers", get_users)
```

**JavaScript (Frontend):**
```javascript
const users = await window.invoke('getUsers', {min_age: 18});
users.forEach(user => console.log(user.name));
```

### Example 3: File Operations

**Tauraro (Backend):**
```python
import os

def list_files(directory):
    files = os.listdir(directory)
    return {
        'count': len(files),
        'files': files
    }

window.register_command("listFiles", list_files)
```

**JavaScript (Frontend):**
```javascript
try {
    const result = await window.invoke('listFiles', {directory: './docs'});
    document.getElementById('fileCount').textContent = result.count;
    result.files.forEach(file => {
        // Display files
    });
} catch (error) {
    console.error('Failed to list files:', error);
}
```

### Example 4: Complex Data Processing

**Tauraro (Backend):**
```python
def process_data(data, options):
    result = {
        'processed': [],
        'stats': {}
    }
    
    for item in data:
        # Process each item
        processed_item = transform(item, options)
        result['processed'].append(processed_item)
    
    result['stats'] = {
        'count': len(result['processed']),
        'average': calculate_average(result['processed'])
    }
    
    return result

window.register_command("processData", process_data)
```

**JavaScript (Frontend):**
```javascript
const data = [1, 2, 3, 4, 5];
const options = {method: 'square'};

const result = await window.invoke('processData', {data, options});
console.log('Processed:', result.processed);
console.log('Stats:', result.stats);
```

## Type Conversion

### Tauraro → JSON
- `None` → `null`
- `Bool` → `boolean`
- `Int` → `number`
- `Float` → `number`
- `Str` → `string`
- `List` → `Array`
- `Dict` → `Object`

### JSON → Tauraro
- `null` → `None`
- `boolean` → `Bool`
- `number` (int) → `Int`
- `number` (float) → `Float`
- `string` → `Str`
- `Array` → `List`
- `Object` → `Dict`

## Alternative: Using HTTP Server

If you need more control or want REST API-style access, you can run an embedded HTTP server:

### Recommended Crate: **`tiny_http`** or **`warp`**

```toml
[dependencies]
tiny_http = "0.12"
# or
warp = "0.3"
```

**Example with tiny_http:**
```rust
use tiny_http::{Server, Response};

let server = Server::http("127.0.0.1:8080").unwrap();

for request in server.incoming_requests() {
    let response = Response::from_string("Hello World");
    request.respond(response).ok();
}
```

**Frontend:**
```javascript
const response = await fetch('http://localhost:8080/api/function', {
    method: 'POST',
    body: JSON.stringify({param: value})
});
const data = await response.json();
```

## Best Practices

1. **Validate Input**: Always validate frontend data before processing
2. **Error Handling**: Return structured errors with helpful messages
3. **Async Operations**: Use async/await for long-running operations
4. **Security**: Never trust frontend input, sanitize everything
5. **Rate Limiting**: Implement rate limits for expensive operations
6. **Logging**: Log all command invocations for debugging

## Security Considerations

```rust
// Validate permissions
fn check_file_access(path: &str) -> Result<()> {
    let canonical = std::fs::canonicalize(path)?;
    if !canonical.starts_with("/allowed/directory") {
        return Err(anyhow::anyhow!("Access denied"));
    }
    Ok(())
}

// Rate limiting
use std::time::{Duration, Instant};
static LAST_CALL: Mutex<Option<Instant>> = Mutex::new(None);

fn rate_limited_function(args: Vec<Value>) -> Result<Value> {
    let mut last = LAST_CALL.lock().unwrap();
    if let Some(time) = *last {
        if time.elapsed() < Duration::from_secs(1) {
            return Err(anyhow::anyhow!("Rate limit exceeded"));
        }
    }
    *last = Some(Instant::now());
    
    // Process...
}
```

## Next Steps

The command registry system is now implemented. To use it:

1. **Integrate into window.rs** - Add command registration before event loop
2. **Update IPC handler** - Handle "invoke:" message type
3. **Inject JavaScript API** - Add `window.invoke()` to custom_js
4. **Register your functions** - Use `register_tauraro_function()`
5. **Call from frontend** - Use `await window.invoke()`

This provides a clean, type-safe way to expose Tauraro functions to your frontend!
