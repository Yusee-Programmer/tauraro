# WebViewTK Component System

## Overview

The WebViewTK component system provides a declarative, component-based approach to building desktop GUIs in Tauraro. Instead of manually writing HTML/CSS/JS, developers use high-level Tauraro component classes that automatically generate optimized frontend code.

## Architecture

### Core Modules

```
src/modules/webviewtk/
├── mod.rs              # Module entry point and API surface
├── window.rs           # Window management, CDN support, mount system
├── ipc.rs              # IPC Bridge for backend-frontend communication
├── component.rs        # Component architecture and HTML/CSS/JS generation
├── components/
│   └── mod.rs         # Component constructors exposed to Tauraro
└── utils.rs           # HTML escaping and rendering utilities
```

### Removed Files

The following legacy files have been removed as they're superseded by the component system:

- **widgets.rs** - Low-level HTML element builders (div, button, p, h1-h6, input, etc.)
- **helpers.rs** - UI pattern helpers (window_controls, menu_bar, search_bar)
- **titlebar.rs** - Custom titlebar creation
- **menu.rs** - Menu/menu_item system
- **drag.rs** - Drag region utilities
- **resources.rs** - CDN helper functions (moved to window.rs)

## Component Types

### 1. VStack (Vertical Stack)

Vertically arranges child components with consistent spacing.

```python
layout = webviewtk.VStack({
    "spacing": 20,
    "padding": 40,
    "class_name": "bg-gray-100",
    "children": []  # List of child components
})
```

**Properties:**
- `spacing` (int): Gap between children in pixels (default: 10)
- `padding` (int): Internal padding in pixels (default: 0)
- `class_name` (str): CSS classes for styling
- `children` (list): Array of child components

**Generated HTML:**
```html
<div class="vstack bg-gray-100" style="display: flex; flex-direction: column; gap: 20px; padding: 40px;">
    <!-- Children here -->
</div>
```

### 2. HStack (Horizontal Stack)

Horizontally arranges child components with consistent spacing.

```python
buttons = webviewtk.HStack({
    "spacing": 10,
    "class_name": "flex justify-center",
    "children": [btn1, btn2, btn3]
})
```

**Properties:**
- `spacing` (int): Gap between children in pixels (default: 10)
- `padding` (int): Internal padding in pixels (default: 0)
- `class_name` (str): CSS classes for styling
- `children` (list): Array of child components

**Generated HTML:**
```html
<div class="hstack flex justify-center" style="display: flex; flex-direction: row; gap: 10px;">
    <!-- Children here -->
</div>
```

### 3. Button

Interactive button with click handling.

```python
submit_btn = webviewtk.Button({
    "text": "Submit",
    "on_click": "handleSubmit",
    "variant": "primary",
    "class_name": "w-full"
})
```

**Properties:**
- `text` (str): Button label
- `on_click` (str): JavaScript function name to call (window.handleClick(action))
- `variant` (str): Style variant - "primary", "secondary", "danger" (default: "primary")
- `class_name` (str): Additional CSS classes

**Generated HTML:**
```html
<button class="btn btn-primary w-full" onclick="window.handleClick('handleSubmit')">
    Submit
</button>
```

**Generated CSS:**
```css
.btn {
    padding: 10px 20px;
    border: none;
    border-radius: 4px;
    cursor: pointer;
    transition: all 0.2s;
}
.btn-primary { background: #3b82f6; color: white; }
.btn-secondary { background: #6b7280; color: white; }
.btn-danger { background: #ef4444; color: white; }
```

### 4. Text

Text display with styling.

```python
heading = webviewtk.Text({
    "content": "Welcome to Tauraro",
    "class_name": "text-4xl font-bold text-blue-600",
    "style": {"color": "#1e40af"}
})
```

**Properties:**
- `content` (str): Text to display
- `class_name` (str): CSS classes
- `style` (dict): Inline styles as key-value pairs

**Generated HTML:**
```html
<span class="text-4xl font-bold text-blue-600" style="color: #1e40af;">
    Welcome to Tauraro
</span>
```

### 5. Input

Text input field with change handling.

```python
email_input = webviewtk.Input({
    "placeholder": "Enter email",
    "value": "",
    "type": "email",
    "on_change": "handleEmailChange",
    "class_name": "border rounded px-4 py-2"
})
```

**Properties:**
- `value` (str): Initial value
- `placeholder` (str): Placeholder text
- `type` (str): Input type - "text", "email", "password", etc. (default: "text")
- `on_change` (str): JavaScript function name for change events
- `class_name` (str): CSS classes

**Generated HTML:**
```html
<input type="email" 
       value="" 
       placeholder="Enter email" 
       class="custom-input border rounded px-4 py-2"
       oninput="window.handleInput('handleEmailChange', this.value)">
```

### 6. Card

Container with optional title and elevated styling.

```python
user_card = webviewtk.Card({
    "title": "User Profile",
    "class_name": "shadow-lg",
    "children": [name_text, email_text, bio_text]
})
```

**Properties:**
- `title` (str, optional): Card header text
- `class_name` (str): CSS classes
- `children` (list): Array of child components

**Generated HTML:**
```html
<div class="card shadow-lg">
    <div class="card-header">User Profile</div>
    <div class="card-body">
        <!-- Children here -->
    </div>
</div>
```

**Generated CSS:**
```css
.card {
    background: white;
    border-radius: 8px;
    box-shadow: 0 2px 8px rgba(0,0,0,0.1);
    overflow: hidden;
}
.card-header {
    padding: 16px;
    font-weight: 600;
    border-bottom: 1px solid #e5e7eb;
}
.card-body { padding: 16px; }
```

### 7. Div

Generic container with styling.

```python
container = webviewtk.Div({
    "class_name": "container mx-auto",
    "style": {"max-width": "1200px"},
    "children": [header, content, footer]
})
```

**Properties:**
- `class_name` (str): CSS classes
- `style` (dict): Inline styles
- `children` (list): Array of child components

### 8. RawHtml

Escape hatch for custom HTML.

```python
custom = webviewtk.RawHtml("<iframe src='https://example.com'></iframe>")
```

**Properties:**
- First positional argument (str): Raw HTML string (NOT escaped)

**⚠️ Warning:** RawHtml bypasses HTML escaping - only use with trusted content!

## CDN Support

### Built-in CDN Constants

WebViewTK provides 10 pre-configured CDN constants:

```python
import webviewtk

window = webviewtk.Window("My App", 800, 600)

# Add single CDN
window.add_cdn(webviewtk.CDN["TAILWIND"])

# Add multiple CDNs
window.add_cdns([
    webviewtk.CDN["TAILWIND"],
    webviewtk.CDN["FONT_AWESOME"],
    webviewtk.CDN["ALPINE"]
])
```

**Available CDNs:**
- `TAILWIND` - Tailwind CSS 3.4.1
- `BOOTSTRAP` - Bootstrap 5.3.2 CSS
- `BOOTSTRAP_JS` - Bootstrap 5.3.2 JavaScript
- `BULMA` - Bulma 0.9.4
- `MATERIALIZE` - Materialize 1.0.0
- `ALPINE` - Alpine.js 3.13.3
- `HTMX` - HTMX 1.9.10
- `CHART_JS` - Chart.js 4.4.1
- `FONT_AWESOME` - Font Awesome 6.5.1
- `HEROICONS` - Heroicons 2.0.18

### Custom CDN URLs

```python
window.add_cdn("https://unpkg.com/my-library@1.0.0/dist/my-library.min.css")
window.add_cdn("https://cdn.jsdelivr.net/npm/custom-js@2.0.0/dist/custom.min.js")
```

The system automatically detects file type:
- `.css` extension → `<link rel="stylesheet">`
- `.js` extension → `<script src="">`
- Default → CSS link

## IPC Bridge

The IPC (Inter-Process Communication) Bridge enables bidirectional communication between the Rust backend and JavaScript frontend.

### Backend: Register Handlers

```python
import webviewtk

def get_user_data(args):
    user_id = args[0]
    # Fetch from database
    return {"name": "Alice", "email": "alice@example.com", "id": user_id}

# Register handler
webviewtk.ipc_register("getUserData", get_user_data)
```

### Frontend: Call Handlers

```javascript
// IPC client is automatically injected by window.mount()
try {
    const user = await window.IPC.call('getUserData', 123);
    console.log(user.name); // "Alice"
} catch (error) {
    console.error('IPC error:', error);
}
```

### IPC Client API

The JavaScript client provides a promise-based API:

```javascript
// Call with single argument
window.IPC.call('handlerName', arg)

// Call with multiple arguments (wrapped in array)
window.IPC.call('calculate', [10, 20, 'add'])

// Error handling
window.IPC.call('mightFail', data)
    .then(result => console.log(result))
    .catch(error => console.error(error))
```

## Window Mount System

The `window.mount()` method generates a complete HTML document with:
1. CDN link/script tags
2. Component HTML
3. Component CSS
4. IPC client injection
5. Custom JavaScript

### Basic Usage

```python
html_content = """
<div class="container">
    <h1>Hello World</h1>
</div>
"""

window.mount({
    "html": html_content,
    "css": ".container { padding: 20px; }",
    "js": "console.log('App loaded');"
})
```

### Component Usage (Current Approach)

Since components currently return HTML strings:

```python
# Build layout with HTML strings
layout_html = """
<div class="min-h-screen flex items-center justify-center">
    <div class="card">
        <h1>Counter: <span id="count">0</span></h1>
        <button onclick="window.handleClick('increment')">+</button>
        <button onclick="window.handleClick('decrement')">-</button>
    </div>
</div>
"""

counter_js = """
let count = 0;
window.handleClick = function(action) {
    if (action === 'increment') count++;
    if (action === 'decrement') count--;
    document.getElementById('count').textContent = count;
};
"""

window.mount({"html": layout_html, "css": "", "js": counter_js})
```

## Complete Example: Counter App

```python
"""
Counter Example - Component-Based WebViewTK
"""

import webviewtk

# Create window
window = webviewtk.Window("Counter App", 500, 400)

# Add Tailwind CSS
window.add_cdn(webviewtk.CDN["TAILWIND"])

# Build UI
html_content = """
<div class="min-h-screen bg-gradient-to-br from-purple-400 via-pink-500 to-red-500 
            flex items-center justify-center">
    <div class="bg-white rounded-2xl shadow-2xl p-12 text-center">
        <h1 class="text-6xl font-bold text-gray-800 mb-4">Counter App</h1>
        <p id="counter" class="text-8xl font-bold text-blue-600 mb-8">0</p>
        
        <div class="flex gap-4 justify-center mb-4">
            <button onclick="window.handleClick('decrement')" 
                    class="bg-red-500 hover:bg-red-600 text-white font-bold 
                           py-4 px-8 rounded-lg text-2xl transition">
                Decrease
            </button>
            <button onclick="window.handleClick('increment')" 
                    class="bg-green-500 hover:bg-green-600 text-white font-bold 
                           py-4 px-8 rounded-lg text-2xl transition">
                Increase
            </button>
        </div>
        
        <button onclick="window.handleClick('reset')" 
                class="bg-gray-500 hover:bg-gray-600 text-white font-bold 
                       py-2 px-6 rounded-lg transition">
            Reset
        </button>
    </div>
</div>
"""

# Counter logic
counter_js = """
let count = 0;
const counterElement = document.getElementById('counter');

window.handleClick = function(action) {
    if (action === 'increment') count++;
    else if (action === 'decrement') count--;
    else if (action === 'reset') count = 0;
    
    counterElement.textContent = count;
};
"""

# Mount and run
window.mount({"html": html_content, "css": "", "js": counter_js})
window.run()
```

## Running Examples

```powershell
# Build with WebViewTK feature
cargo build --features webviewtk

# Run counter example
cargo run --features webviewtk --bin tauraro -- run examples/counter/app.tr

# Run IPC demo
cargo run --features webviewtk --bin tauraro -- run examples/ipc_demo/app.tr
```

## Build Requirements

- **Feature Flag:** `--features webviewtk` required
- **Platform:** Windows (WebView2), Linux (WebKitGTK), macOS (WebKit)
- **Dependencies:** Managed by Cargo (wry, tao, serde_json, etc.)

## Future Enhancements

### Planned Features

1. **True Component Objects:** Components return Component instances, not HTML strings
2. **State Management:** Reactive state with auto re-render
3. **Component Composition:** Nested components with proper parent-child relationships
4. **Event System:** Type-safe event handlers with automatic binding
5. **IPC Decorators:** `@IPC.handler` for cleaner handler registration
6. **More Components:** Image, Link, List, Table, Form, Modal, etc.
7. **Custom Components:** User-defined component classes
8. **Hot Reload:** Live reload during development
9. **Dev Tools:** Component inspector and debugger

### Architecture Evolution

Current:
```python
button = webviewtk.Button({...})  # Returns HTML string
window.mount({"html": button, ...})
```

Future:
```python
# Components are objects
button = webviewtk.Button(text="Click", on_click=handle_click)
layout = webviewtk.VStack(children=[button])

# Direct mounting
window.mount(layout)  # Generates HTML/CSS/JS automatically
```

## Implementation Details

### Component HTML Generation

Components implement `to_html()` which:
1. Escapes user content (except RawHtml)
2. Generates semantic HTML with accessibility attributes
3. Applies CSS classes and inline styles
4. Sets up event handlers

### Component CSS Generation

Components implement `to_css()` which:
1. Generates scoped styles for component types
2. Includes modern CSS (flexbox, transitions, shadows)
3. Provides variant styles (button variants, etc.)
4. Ensures consistent spacing and typography

### Component JS Generation

Components implement `to_js()` which:
1. Registers event handlers (handleClick, handleInput)
2. Integrates with IPC Bridge
3. Provides component-specific behavior
4. Ensures proper event delegation

### Value Type Handling

All component constructors accept Tauraro `Value` types:
- `Value::Dict` for keyword arguments
- `Value::Str` for strings
- `Value::Int` for integers
- `Value::List` for arrays
- `Value::Bool` for booleans

Example:
```rust
pub fn create_button_component(args: Vec<Value>) -> anyhow::Result<Value> {
    if let Some(Value::Dict(kwargs)) = args.get(0) {
        let kwargs_ref = kwargs.borrow();  // RefCell borrow
        if let Some(Value::Str(text)) = kwargs_ref.get("text") {
            // Use text
        }
    }
    Ok(Value::Str(component.to_html()))
}
```

## API Reference

### Window Methods

```python
# Window creation
window = webviewtk.Window(title: str, width: int, height: int)

# CDN management
window.add_cdn(url: str)
window.add_cdns(urls: list[str])

# Content loading
window.set_html(html: str)
window.load_html(path: str)
window.load_css(path: str)
window.load_script(path: str)

# Component mounting
window.mount(config: dict)  # {"html": str, "css": str, "js": str}

# Event handling
window.on_message(callback: function)

# Lifecycle
window.run()  # Blocking
```

### IPC Functions

```python
# Register handler (backend)
webviewtk.ipc_register(name: str, handler: function)

# Call handler (frontend JavaScript)
window.IPC.call(name: string, ...args: any): Promise<any>
```

### Component Constructors

All constructors accept a dict with component-specific properties:

```python
webviewtk.VStack(props: dict) -> str
webviewtk.HStack(props: dict) -> str
webviewtk.Button(props: dict) -> str
webviewtk.Text(props: dict) -> str
webviewtk.Input(props: dict) -> str
webviewtk.Card(props: dict) -> str
webviewtk.Div(props: dict) -> str
webviewtk.RawHtml(html: str) -> str
```

## Troubleshooting

### Common Issues

**1. "mount() requires self and component arguments"**
- Ensure you're calling `window.mount()` not `webviewtk.mount()`
- Pass a dict with "html", "css", "js" keys

**2. Component not rendering**
- Check that component returns HTML string
- Verify mount() receives correct structure
- Inspect browser console for JS errors

**3. IPC call fails**
- Ensure handler is registered before window.run()
- Check handler name matches exactly
- Verify handler returns serializable data

**4. CDN not loading**
- Check network connectivity
- Verify CDN URL is correct
- Try adding CDN before mount()

### Debug Mode

```python
# Enable window debugging
window.on_message(lambda msg: print(f"[WEBVIEW] {msg}"))

# Check IPC registration
webviewtk.ipc_register("test", lambda args: print("IPC works!"))
```

## License

WebViewTK is part of the Tauraro project. See LICENSE file for details.
