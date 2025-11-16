# WebViewTK Component-Based Architecture

## Philosophy: Flutter-like Component System

WebViewTK follows a **component-based architecture** similar to Flutter, where the entire UI is built using component classes and functions. Components automatically generate HTML, CSS, and JavaScript behind the scenes.

## Core Principle

**Developers write Tauraro component code → Components generate HTML/CSS/JS → Beautiful UI renders**

No manual HTML/CSS/JS needed! (But you can add custom styling if you want)

## Component-Only Approach

### ✅ Ideal Way (Pure Components)

```python
import webviewtk

window = webviewtk.Window("My App", 800, 600)
window.add_cdn(webviewtk.CDN["TAILWIND"])

# Build UI with components only
title = webviewtk.Text({
    "content": "Welcome",
    "class_name": "text-4xl font-bold"
})

button = webviewtk.Button({
    "text": "Click Me",
    "on_click": "handleClick",
    "class_name": "bg-blue-500 text-white px-4 py-2"
})

# Compose layout - components generate HTML automatically
layout = '<div class="p-8">' + title + button + '</div>'

# Mount and run
window.mount({"html": layout, "css": "", "js": "..."})
window.run()
```

### ❌ Old Way (Manual HTML - Avoid This)

```python
# Don't do this - too much manual HTML!
html = """
<div class="p-8">
    <h1 class="text-4xl font-bold">Welcome</h1>
    <button class="bg-blue-500" onclick="...">Click Me</button>
</div>
"""
```

## Available Components

### Layout Components

#### VStack (Vertical Stack)
Arranges children vertically with consistent spacing.

```python
vstack = webviewtk.VStack({
    "spacing": 20,
    "padding": 40,
    "class_name": "bg-gray-100",
    "children": [child1, child2, child3]
})
```

#### HStack (Horizontal Stack)
Arranges children horizontally with consistent spacing.

```python
hstack = webviewtk.HStack({
    "spacing": 10,
    "class_name": "justify-center",
    "children": [btn1, btn2, btn3]
})
```

### UI Components

#### Text
Display text with styling.

```python
text = webviewtk.Text({
    "content": "Hello World",
    "class_name": "text-2xl font-bold text-blue-600"
})
```

#### Button
Interactive button with click handlers.

```python
button = webviewtk.Button({
    "text": "Submit",
    "on_click": "handleSubmit",
    "variant": "primary",  # primary, secondary, danger
    "class_name": "w-full"
})
```

#### Input
Text input fields.

```python
input_field = webviewtk.Input({
    "placeholder": "Enter text...",
    "type": "text",  # text, email, password, etc.
    "value": "",
    "class_name": "border rounded px-4 py-2"
})
```

#### Card
Container with elevation and optional title.

```python
card = webviewtk.Card({
    "title": "User Profile",
    "class_name": "shadow-lg",
    "children": [name, email, bio]
})
```

#### Div
Generic container for grouping.

```python
container = webviewtk.Div({
    "class_name": "flex items-center gap-4",
    "children": [icon, label]
})
```

#### RawHtml
Escape hatch for custom HTML (use sparingly).

```python
custom = webviewtk.RawHtml("<iframe src='...'></iframe>")
```

## How Components Work

### Behind the Scenes

1. **Developer writes component code:**
   ```python
   button = webviewtk.Button({"text": "Click", "on_click": "test"})
   ```

2. **Component generates HTML:**
   ```html
   <button class="btn btn-primary" onclick="window.handleClick('test')">
       Click
   </button>
   ```

3. **Component generates CSS (automatic):**
   ```css
   .btn {
       padding: 10px 20px;
       border: none;
       border-radius: 4px;
       cursor: pointer;
       transition: all 0.2s;
   }
   .btn-primary {
       background: #3b82f6;
       color: white;
   }
   ```

4. **Component generates JS (automatic):**
   ```javascript
   window.handleClick = function(action) {
       // Handle button clicks
   };
   ```

5. **window.mount() injects everything:**
   - CDN links
   - Component HTML
   - Component CSS
   - Component JS
   - IPC client

## Optional Custom Styling

While components handle everything, you can add custom styling if needed:

### Method 1: External CSS File

```python
window = webviewtk.Window("My App", 800, 600)
window.add_cdn(webviewtk.CDN["TAILWIND"])

# Build UI with components
layout = build_component_tree()

# Add custom CSS file
window.load_css("custom_styles.css")

window.mount({"html": layout, "css": "", "js": ""})
window.run()
```

### Method 2: Inline CSS

```python
custom_css = """
.my-custom-button {
    background: linear-gradient(45deg, #667eea, #764ba2);
    border-radius: 20px;
}
"""

window.mount({
    "html": layout,
    "css": custom_css,  # Custom CSS
    "js": app_js
})
```

### Method 3: RawHtml Component

```python
# For truly custom HTML needs
custom_widget = webviewtk.RawHtml("""
<div class="custom-widget">
    <canvas id="myCanvas"></canvas>
</div>
""")

layout = container + custom_widget + footer
```

## CDN Support

Add CSS frameworks and libraries via CDN:

```python
window.add_cdn(webviewtk.CDN["TAILWIND"])      # Tailwind CSS
window.add_cdn(webviewtk.CDN["BOOTSTRAP"])     # Bootstrap
window.add_cdn(webviewtk.CDN["FONT_AWESOME"])  # Icons
window.add_cdn(webviewtk.CDN["ALPINE"])        # Alpine.js
window.add_cdn(webviewtk.CDN["CHART_JS"])      # Charts

# Or custom CDN
window.add_cdn("https://unpkg.com/my-library@1.0.0/dist/style.css")
```

## Future Vision

### Current (v0.2.0)
```python
# Components return HTML strings
button = webviewtk.Button({...})  # Returns: "<button>...</button>"

# Manual composition with string concatenation
layout = '<div>' + button + '</div>'

# Manual event handlers
window.mount({"html": layout, "css": "", "js": "window.handleClick = ..."})
```

### Future (v1.0.0)
```python
# Components return objects
button = webviewtk.Button(
    text="Click",
    on_click=lambda: print("Clicked!")  # Python function, not JS string!
)

# Nested composition (Flutter-style)
layout = webviewtk.VStack(
    children=[
        webviewtk.Text("Header"),
        webviewtk.HStack(
            children=[button1, button2]
        ),
        webviewtk.Card(
            title="Content",
            children=[body_text]
        )
    ]
)

# Direct mounting - generates everything
window.mount(layout)  # That's it! HTML/CSS/JS auto-generated

# State management
state = webviewtk.State({"count": 0})

button = webviewtk.Button(
    text=lambda: f"Count: {state['count']}",
    on_click=lambda: state.update({"count": state['count'] + 1})
)

# Auto re-render on state changes!
```

## Examples

### 1. Counter App (Pure Components)
Location: `examples/counter/app.tr`

Demonstrates:
- Text and Button components
- Tailwind CSS via CDN
- Component composition
- Event handling

### 2. Todo App (Component-Based)
Location: `examples/component_todo/app.tr`

Demonstrates:
- Input, Button, Text components
- Dynamic rendering
- State management
- Filters and stats

### 3. IPC Demo (Components + Backend)
Location: `examples/ipc_demo/app.tr`

Demonstrates:
- Component-based UI
- IPC Bridge communication
- Backend data fetching
- Real-time updates

### 4. Profile Manager (Advanced Components)
Location: `examples/profile_manager.tr`

Demonstrates:
- Card components
- Form inputs
- Grid layouts
- Complex component trees

## Best Practices

### ✅ DO

- Use components for all UI elements
- Leverage Tailwind utility classes for styling
- Compose complex UIs from simple components
- Add custom CSS only when truly needed
- Use CDN for common libraries

### ❌ DON'T

- Write raw HTML strings manually
- Mix component and manual HTML approaches
- Bypass component system unless necessary
- Forget to add CDNs before mounting
- Hardcode styles when Tailwind classes work

## Component Development Guidelines

When building apps:

1. **Start with layout components** (VStack, HStack, Card)
2. **Add UI components** (Text, Button, Input)
3. **Style with Tailwind classes**
4. **Compose into tree structure**
5. **Add custom CSS only if needed**
6. **Mount and run**

Example workflow:

```python
# 1. Layout structure
main_container = VStack(...)

# 2. Add UI elements
title = Text(...)
form = VStack([Input(...), Input(...), Button(...)])

# 3. Compose
layout = main_container + title + form

# 4. Mount (components generated HTML/CSS)
window.mount({"html": layout, "css": "", "js": ""})
```

## Migration Guide

### From Manual HTML to Components

**Before:**
```python
html = """
<div class="card">
    <h2>Title</h2>
    <button onclick="handleClick()">Click</button>
</div>
"""
```

**After:**
```python
title = webviewtk.Text({"content": "Title", "class_name": "text-2xl"})
button = webviewtk.Button({"text": "Click", "on_click": "handleClick"})
card = '<div class="card">' + title + button + '</div>'
```

### From External Files to Components

**Before:**
```python
window.load_html("ui.html")
window.load_css("styles.css")
window.load_script("app.js")
```

**After:**
```python
# Build with components
layout = build_ui_with_components()

# Optional: Add custom CSS if needed
window.load_css("custom_extras.css")

window.mount({"html": layout, "css": "", "js": ""})
```

## Support

For questions or issues:
- Check documentation: `docs/WEBVIEWTK_COMPONENT_SYSTEM.md`
- See examples: `examples/` directory
- GitHub Issues: Report bugs or request features

## License

Part of Tauraro project. See LICENSE file.
