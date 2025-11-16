# WebViewTK Modularization & Custom CDN/IPC Guide

## Overview

WebViewTK has been fully modularized and enhanced with:
1. **Custom CDN Support** - Add any CSS/JS library via URL
2. **IPC Backend Integration** - Connect widget events to Tauraro backend handlers
3. **Modular Architecture** - Components organized by category in separate files

---

## 1. Custom CDN Support

### Three Ways to Add CDN Libraries

#### Method 1: Predefined CDN Constants
```python
import webviewtk

window = webviewtk.Window({"title": "App", "width": 800, "height": 600})

# Use built-in CDN constants
window.add_cdn(webviewtk.CDN["TAILWIND"])
window.add_cdn(webviewtk.CDN["BOOTSTRAP"])
window.add_cdn(webviewtk.CDN["FONT_AWESOME"])
```

**Available CDN Constants:**
- `TAILWIND` - Tailwind CSS
- `BOOTSTRAP` - Bootstrap 5
- `BOOTSTRAP_JS` - Bootstrap JavaScript
- `BULMA` - Bulma CSS
- `MATERIALIZE` - Materialize CSS
- `ALPINE` - Alpine.js
- `HTMX` - HTMX
- `CHART_JS` - Chart.js
- `FONT_AWESOME` - Font Awesome icons
- `HEROICONS` - Heroicons

#### Method 2: Direct URL
```python
# Add any CSS/JS library by URL
window.add_cdn("https://cdn.jsdelivr.net/npm/tailwindcss@3.4.1/dist/tailwind.min.css")
window.add_cdn("https://fonts.googleapis.com/css2?family=Inter:wght@400;700&display=swap")
window.add_cdn("https://cdnjs.cloudflare.com/ajax/libs/animate.css/4.1.1/animate.min.css")
```

#### Method 3: Custom Named CDN
```python
# Add with metadata (useful for managing multiple custom libraries)
window.add_custom_cdn({
    "name": "my_animations",
    "url": "https://example.com/my-library.css"
})
```

### Multiple CDN at Once
```python
# Add multiple CDN URLs in one call
window.add_cdns([
    "https://fonts.googleapis.com/icon?family=Material+Icons",
    "https://unpkg.com/lucide@latest/dist/umd/lucide.js"
])
```

---

## 2. IPC Backend Integration

### How IPC Works

Widget events (button clicks, input changes) can trigger **Tauraro backend functions** automatically.

### Step-by-Step IPC Setup

#### 1. Define Backend Handler Functions
```python
import webviewtk

def handle_increment():
    print("Increment button clicked!")
    # Update state, database, etc.
    return {"status": "success", "value": 1}

def handle_save(data):
    print(f"Saving: {data}")
    # Save to file/database
    return {"status": "saved"}
```

#### 2. Register Handlers with IPC
```python
# Register handlers BEFORE window.run()
webviewtk.ipc_register("handle_increment", handle_increment)
webviewtk.ipc_register("handle_save", handle_save)
```

#### 3. Connect Widgets to Handlers
```python
# Button with IPC handler
increment_btn = webviewtk.Button({
    "text": "Increment",
    "on_click": "handle_increment"  # Calls backend function!
})

# TextField with IPC handler
text_field = webviewtk.TextField({
    "placeholder": "Type something",
    "on_change": "handle_save"  # Calls backend on input change
})
```

#### 4. Build and Run
```python
window.build(app)
window.run()  # IPC handlers now active!
```

### Complete IPC Example

```python
import webviewtk

counter = {"value": 0}

def increment():
    counter["value"] += 1
    print(f"Counter: {counter['value']}")
    return counter

def decrement():
    counter["value"] -= 1
    print(f"Counter: {counter['value']}")
    return counter

def reset():
    counter["value"] = 0
    print("Counter reset")
    return counter

def main():
    window = webviewtk.Window({"title": "IPC Counter", "width": 400, "height": 300})
    
    # Register handlers
    webviewtk.ipc_register("increment", increment)
    webviewtk.ipc_register("decrement", decrement)
    webviewtk.ipc_register("reset", reset)
    
    # UI with IPC-connected buttons
    app = webviewtk.Column({
        "spacing": 16,
        "padding": 24,
        "children": [
            webviewtk.Text({
                "content": "IPC Counter",
                "font_size": 24,
                "font_weight": "bold"
            }),
            webviewtk.Row({
                "spacing": 8,
                "children": [
                    webviewtk.Button({
                        "text": "−",
                        "on_click": "decrement"
                    }),
                    webviewtk.Button({
                        "text": "Reset",
                        "on_click": "reset"
                    }),
                    webviewtk.Button({
                        "text": "+",
                        "on_click": "increment"
                    })
                ]
            })
        ]
    })
    
    window.build(app)
    window.run()

if __name__ == "__main__":
    main()
```

### IPC Handler Signatures

```python
# No parameters
def my_handler():
    return {"result": "ok"}

# With single parameter (from input fields)
def handle_input(value):
    print(f"Input: {value}")
    return {"processed": value}

# With multiple parameters (from complex widgets)
def handle_form(name, email, age):
    return {"saved": True, "name": name}
```

---

## 3. Modular Architecture

### Component Organization

WebViewTK components are now organized into logical modules:

```
src/modules/webviewtk/components/
├── mod.rs              # Main module - re-exports all widgets
├── helpers.rs          # Widget registry & helper functions
├── layout.rs           # Layout widgets (Column, Row, Container, etc.)
├── basic.rs            # Basic widgets (Button, Text, TextField, Card)
├── material.rs         # Material Design (Scaffold, AppBar, FAB, etc.)
└── utils_widgets.rs    # EdgeInsets helpers
```

### Module Structure

#### `helpers.rs`
- Widget Registry (`WIDGET_REGISTRY`)
- `register_widget()` - Store widget in registry
- `get_widget()` - Retrieve widget by ID
- Helper functions: `get_string()`, `get_float()`, `get_edge_insets()`, etc.

#### `layout.rs`
- `create_column()` - Vertical layout
- `create_row()` - Horizontal layout
- `create_container()` - Styled box
- `create_center()` - Center child
- `create_padding()` - Add padding
- `create_expanded()` - Fill available space
- `create_spacer()` - Flexible empty space

#### `basic.rs`
- `create_button_component()` - Interactive button
- `create_text_component()` - Styled text
- `create_textfield()` - Input field
- `create_card_component()` - Card container

#### `material.rs`
- `create_scaffold()` - Material Design layout
- `create_appbar()` - Top app bar
- `create_floating_action_button()` - FAB

#### `utils_widgets.rs`
- `edgeinsets_all()` - Uniform padding
- `edgeinsets_symmetric()` - Horizontal/vertical
- `edgeinsets_only()` - Individual sides
- `edgeinsets_zero()` - No padding

### Benefits of Modularization

1. **Maintainability** - Each widget category in its own file
2. **Scalability** - Easy to add new widgets without bloating single file
3. **Clarity** - Clear organization by functionality
4. **Performance** - Faster compilation (only rebuild changed modules)
5. **Team Collaboration** - Multiple developers can work on different widget files

### Adding New Widgets

To add a new widget:

1. **Choose appropriate module** (or create new one)
2. **Add constructor function** in that module file
3. **Export function** in `mod.rs`
4. **Register in main** `mod.rs` namespace

Example - Adding a new `Checkbox` widget:

```rust
// In basic.rs
pub fn create_checkbox(args: Vec<Value>) -> anyhow::Result<Value> {
    // Extract properties
    let mut checked = false;
    let mut on_changed = None;
    
    // ... property extraction ...
    
    // Create component
    let component = Component::new(ComponentType::Checkbox {
        value: checked,
        on_changed,
        label: None
    });
    
    let widget_id = register_widget(component);
    
    // Return widget dict
    let mut widget_dict = HashMap::new();
    widget_dict.insert("_widget_id".to_string(), Value::Str(widget_id));
    widget_dict.insert("_widget_type".to_string(), Value::Str("Checkbox".to_string()));
    
    Ok(Value::Dict(Rc::new(RefCell::new(widget_dict))))
}
```

```rust
// In components/mod.rs
pub use basic::{
    create_button_component, create_text_component,
    create_textfield, create_card_component, create_checkbox  // Add here
};
```

```rust
// In webviewtk/mod.rs
namespace.insert("Checkbox".to_string(), Value::NativeFunction(components::create_checkbox));
```

---

## Complete Example: Custom CDN + IPC + Modular Widgets

See `examples/cdn_ipc_demo/app.tr` for a comprehensive demonstration combining all three features.

---

## API Reference

### Window Methods

#### CDN Management
```python
window.add_cdn(url: str)                        # Add single CDN
window.add_cdns(urls: list)                     # Add multiple CDN
window.add_custom_cdn({"name": str, "url": str})  # Add named CDN
```

#### IPC Management
```python
webviewtk.ipc_register(name: str, handler: function)  # Register handler
webviewtk.ipc_call(name: str, *args)                  # Call handler (from JS)
```

#### Widget Building
```python
window.build(root_widget)  # Generate HTML/CSS/JS from widget tree
window.run()               # Show window and start event loop
```

---

## Best Practices

### CDN
- ✅ Add CDN **before** `window.build()`
- ✅ Use predefined constants when available (version-controlled)
- ✅ Group related CDN (e.g., CSS + JS together)
- ❌ Don't add duplicate CDN
- ❌ Don't add after `window.run()`

### IPC
- ✅ Register handlers **before** `window.run()`
- ✅ Use descriptive handler names (`handle_save`, not `h1`)
- ✅ Return serializable data (dicts, lists, strings)
- ✅ Handle errors gracefully
- ❌ Don't use blocking operations in handlers (use async if needed)
- ❌ Don't modify global state unsafely

### Modular Architecture
- ✅ Keep related widgets in same module file
- ✅ Use helper functions to reduce duplication
- ✅ Follow naming conventions (`create_*` for constructors)
- ✅ Document widget properties
- ❌ Don't create circular dependencies between modules
- ❌ Don't expose internal helpers globally

---

## Troubleshooting

### CDN not loading
- Check URL is correct and accessible
- Verify CDN supports CORS
- Check browser console for errors

### IPC handler not found
- Ensure `ipc_register()` called before `window.run()`
- Check handler name matches exactly (case-sensitive)
- Verify handler is registered in same script

### Widget not rendering
- Confirm widget is registered in registry
- Check widget dict has `_widget_id` and `_widget_type`
- Verify component type exists in `ComponentType` enum

---

**Tauraro WebViewTK** - Modular, Extensible, Production-Ready GUI Framework 🚀
