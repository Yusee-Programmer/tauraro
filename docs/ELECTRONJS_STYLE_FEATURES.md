# WebViewTK Electron.js-Style Features

## Summary

WebViewTK now supports Electron.js-like development patterns with simplified event handling, IPC communication, and pre-built UI helpers. This eliminates the need to write repetitive HTML/CSS/JavaScript for common patterns.

## ✨ Key New Features

### 1. Event Handlers on Widgets

Widgets like `button` and `input` now accept an **event handler parameter** that automatically generates IPC communication code.

**Before:**
```python
wv.button(
    "Click Me",
    "btn-class",
    "",  # id
    "",  # style
    {"onclick": "window.ipc.postMessage(JSON.stringify({cmd: 'my_event'}))"}
)
```

**After:**
```python
wv.button(
    "Click Me",
    "btn-class",
    "my_event"  # ← Event handler (auto-generates IPC call)
)
```

**How It Works:**
- If the event handler is a simple identifier (e.g., `my_event`), it automatically sends `{"cmd": "my_event"}` via IPC
- If it contains special characters or JavaScript syntax, it's treated as raw JavaScript code
- For inputs, it automatically includes the input value: `{"cmd": "my_event", "value": "...}"}`

### 2. UI Helper Functions

Pre-built components for common UI patterns - no more manual HTML/CSS!

#### `wv.window_controls(theme, classes="")`
Creates minimize, maximize, and close buttons.

```python
wv.window_controls("light")  # or "dark"
```

**Returns:** Styled window control buttons that automatically work with the IPC system.

#### `wv.menu_bar(menu_items, theme, classes="")`
Creates a menu bar with buttons.

```python
wv.menu_bar(["File", "Edit", "View", "Help"], "light")
```

Menu items can be:
- **Strings:** `"File"` → sends IPC `menu_file`
- **Dicts:** `{"label": "File", "event": "file_menu"}` → custom event name

#### `wv.search_bar(placeholder, event_handler, classes="")`
Creates a search input that sends IPC on input.

```python
wv.search_bar("Search...", "search_query")
```

Sends: `{"cmd": "search_query", "value": "user input"}`

#### `wv.titlebar_custom(title, show_menu, show_search, menu_items, theme)`
Creates a complete titlebar with all elements.

```python
wv.titlebar_custom(
    "My App",
    True,  # show_menu
    True,  # show_search
    ["File", "Edit", "View", "Help"],
    "light"
)
```

### 3. Backend Event Handlers (Coming Soon)

Register Tauraro functions to handle IPC messages from the frontend (like Electron's `ipcMain.on()`).

```python
# Register handler
window.on_message("my_event", lambda msg: print(f"Received: {msg}"))

# When frontend sends {"cmd": "my_event", "data": "hello"}
# Backend calls: lambda with msg = {"cmd": "my_event", "data": "hello"}
```

**Note:** The `window.on_message()` method is implemented but custom handler integration into the IPC system is not yet complete. Currently, you'll see `[DEBUG] Unknown IPC command` for custom events, but the built-in commands (drag_window, minimize_window, etc.) work perfectly.

## 📋 Examples

### Simple Event Handler Demo
**File:** `examples/simple_event_handler_demo.py`

Shows:
- Button with event handler
- Input with event handler (sends value on input)
- Real-time IPC communication

### UI Helpers Demo
**File:** `examples/ui_helpers_demo.py`

Shows:
- `wv.window_controls()` - minimize, maximize, close buttons
- `wv.menu_bar()` - menu with File, Edit, View, Help
- `wv.search_bar()` - search input component
- Custom titlebar with drag support

### Advanced Titlebar Demo
**File:** `examples/advanced_titlebar_demo.py`

Comprehensive demo with:
- Custom draggable titlebar
- Menu bar, search bar, action buttons
- Window controls
- Navigation sidebar
- Interactive content sections

## 🎯 Usage Guide

### Creating a Button with Event Handler

```python
# Simple event name (sends IPC automatically)
wv.button("Click Me", "btn-primary", "button_clicked")

# Raw JavaScript (for complex logic)
wv.button("Alert", "btn-secondary", "alert('Hello!')")
```

### Creating an Input with Event Handler

```python
# Sends input value on change
wv.input("text", "Type here...", "input_changed", "input-class")

# In terminal, you'll see:
# [DEBUG] IPC message received: {"cmd":"input_changed","value":"hello"}
```

### Creating a Custom Titlebar

```python
window.disable_decorations()  # Remove native titlebar

# Option 1: Use helper function
titlebar = wv.titlebar_custom("My App", True, True, ["File", "Edit"], "light")

# Option 2: Build manually
titlebar = wv.div(
    wv.render(
        wv.span("My App", "title-class"),
        wv.window_controls("light")
    ),
    "titlebar-drag",  # Makes it draggable
    "",
    "height: 40px; display: flex; justify-content: space-between; -webkit-app-region: drag;"
)
```

### Registering Backend Handlers (Planned)

```python
def handle_button_click(msg):
    print(f"Button clicked! Data: {msg}")
    # Process the event, update UI, call APIs, etc.

window.on_message("button_clicked", lambda msg: handle_button_click(msg))
```

## 🔧 Technical Details

### Widget Function Signatures (Updated)

**Button:**
```python
wv.button(text, classes, event_handler, id, style, attrs)
```

**Input:**
```python
wv.input(input_type, placeholder, event_handler, classes, id, style, attrs)
```

**Note:** The event_handler parameter was inserted at position 2 (after classes), shifting id and style to positions 3 and 4.

### Event Handler Processing

1. **Simple identifier** (e.g., `my_event`):
   - For buttons: `onclick="window.ipc.postMessage(JSON.stringify({cmd: 'my_event'}))"`
   - For inputs: `oninput="window.ipc.postMessage(JSON.stringify({cmd: 'my_event', value: this.value}))"`

2. **JavaScript code** (contains spaces, parens, etc.):
   - Used as-is: `onclick="alert('Hello')"`

### Helper Function Returns

All helper functions return `Value::Str(html_string)`, which can be used with `wv.render()` like any other widget.

## 🚀 Comparison: Before vs After

### Before (Manual IPC)

```python
wv.button(
    "Submit",
    "btn-primary",
    "",
    "",
    {"onclick": "window.ipc.postMessage(JSON.stringify({cmd: 'submit', data: {name: 'test'}}))"}
)
```

### After (Event Handler)

```python
wv.button("Submit", "btn-primary", "submit")
```

### Before (Manual Window Controls)

```html
<div>
    <button onclick="window.tauraro.minimize()" style="...50 lines of CSS...">−</button>
    <button onclick="window.tauraro.maximize()" style="...50 lines of CSS...">□</button>
    <button onclick="window.tauraro.close()" style="...50 lines of CSS...">✕</button>
</div>
```

### After (Helper Function)

```python
wv.window_controls("light")
```

## ⚠️ Known Limitations

1. **Custom handler integration incomplete:** The `window.on_message()` method is implemented, but the IPC handler doesn't yet call registered Tauraro functions. Currently, only built-in commands work (drag_window, minimize_window, maximize_window, close_window, etc.).

2. **Event handler position:** The event_handler parameter was added at position 2, which changes the function signature. Old code using positional arguments for id/style/attrs will need to be updated:
   ```python
   # Old (still works if using attrs dict):
   wv.button("Text", "class", "", "", {"onclick": "..."})
   
   # New:
   wv.button("Text", "class", "event_name", "", "", {})
   ```

3. **Theme options:** Helper functions support "light" and "dark" themes, but customization is limited. For full control, build components manually.

## 🔮 Future Enhancements

1. **Complete custom handler integration:** Make `window.on_message()` fully functional so backend Tauraro functions receive IPC messages
2. **More helper functions:** sidebar(), toolbar(), statusbar(), dialog(), etc.
3. **Theme customization:** Allow passing custom colors/styles to helper functions
4. **Event handler improvements:** Support passing data with events, custom JSON payloads
5. **Type safety:** Better validation of event handler parameters

## 📚 Related Files

- **Widget functions:** `src/modules/webviewtk/widgets.rs`
- **Helper functions:** `src/modules/webviewtk/helpers.rs`
- **Window/IPC handling:** `src/modules/webviewtk/window.rs`
- **Examples:** `examples/simple_event_handler_demo.py`, `examples/ui_helpers_demo.py`, `examples/advanced_titlebar_demo.py`

---

**Version:** 0.2.0  
**Last Updated:** November 15, 2024
