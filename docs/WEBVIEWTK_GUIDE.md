# WebViewTK Framework Guide

WebViewTK is a cross-platform GUI framework for Tauraro that uses HTML, CSS, and JavaScript for UI rendering while Tauraro handles the application logic. It's similar to Tauri and Electron.js.

## Table of Contents

1. [Installation](#installation)
2. [Quick Start](#quick-start)
3. [Widget Functions](#widget-functions)
4. [CDN Integration](#cdn-integration)
5. [Window Management](#window-management)
6. [Complete Examples](#complete-examples)

## Installation

### Building with WebViewTK

To use WebViewTK, simply compile Tauraro with the `webviewtk` feature:

```bash
# Debug build
cargo build --features webviewtk

# Release build (recommended for production)
cargo build --release --features webviewtk
```

**Automated Dependency Installation**: When you build with the `webviewtk` feature, Tauraro automatically detects and installs platform-specific dependencies:

- **Windows**: Checks for WebView2 Runtime and installs it automatically if missing (Windows 11 has it pre-installed)
- **Linux**: Detects your distribution and installs WebKitGTK using the appropriate package manager
- **macOS**: No action needed - WebKit is built-in

You'll see build warnings showing the dependency check status during compilation.

### Dependencies

WebViewTK uses these Rust crates:
- **wry** - Cross-platform webview library (same as Tauri)
- **tao** - Cross-platform window creation

Platform-specific dependencies are automatically managed by the build script.

## Quick Start

### Hello World Example

```python
import webviewtk as wv

# Create a simple heading
heading = wv.h1("Hello, World!", "text-4xl font-bold")

# Build HTML page
html = wv.html(
    wv.render(
        wv.head(wv.cdn_tailwind()),
        wv.body(heading)
    )
)

# Create and run window
window = wv.Window("My App", 800, 600)
window.set_html("<!DOCTYPE html>" + html)
window.run()
```

## Widget Functions

### Basic Elements

#### Headings
```python
wv.h1("Title", classes="", id="", style="", attrs={})
wv.h2("Subtitle", "text-2xl font-bold")
wv.h3("Section")
wv.h4("Subsection")
wv.h5("Minor heading")
wv.h6("Smallest heading")

# Or use the generic heading function
wv.heading("Title", level=1, classes="")
```

#### Text Elements
```python
# Paragraph
wv.p("This is a paragraph", "text-gray-700")

# Span
wv.span("Inline text", "font-bold text-red-500")

# Div container
wv.div("Content here", "container mx-auto p-4")
```

### Form Elements

#### Input Fields
```python
# Text input
wv.input("text", placeholder="Enter name", classes="form-control")

# Email input
wv.input("email", "your@email.com", "input-field")

# Password input
wv.input("password", "", "password-input")

# Textarea
wv.textarea("Default content", "form-control")
```

#### Buttons
```python
# Basic button
wv.button("Click Me", "btn btn-primary")

# Button with ID and style
wv.button("Submit", classes="btn", id="submitBtn", style="margin: 10px")
```

#### Checkboxes and Radio Buttons
```python
# Checkbox
wv.checkbox("Remember me", name="remember", classes="form-check")

# Radio button
wv.radio("Option 1", name="choice", value="opt1", classes="radio")
```

#### Select Dropdowns
```python
# Create select with options
options = wv.render(
    wv.option("Choice 1", "1"),
    wv.option("Choice 2", "2"),
    wv.option("Choice 3", "3")
)
wv.select(options, "form-select")
```

#### Forms
```python
form_content = wv.render(
    wv.label("Email:", "email"),
    wv.input("email", "", "form-control", "email"),
    wv.button("Submit", "btn")
)

wv.form(form_content, action="/submit", method="POST", classes="my-form")
```

### Images and Links

```python
# Image
wv.img("image.jpg", alt="Description", classes="img-fluid")

# Link
wv.link("Click here", href="https://example.com", classes="text-blue-500")
# Or use the alias
wv.a("Link text", "https://example.com")
```

### Lists

```python
# Unordered list
items = wv.render(
    wv.li("Item 1"),
    wv.li("Item 2"),
    wv.li("Item 3")
)
wv.ul(items, "list-disc ml-4")

# Ordered list
wv.ol(items, "list-decimal ml-4")
```

### Tables

```python
rows = wv.render(
    wv.tr(wv.render(
        wv.th("Name"),
        wv.th("Age")
    )),
    wv.tr(wv.render(
        wv.td("John"),
        wv.td("30")
    ))
)

wv.table(rows, "table table-striped")
```

### Semantic HTML5 Elements

```python
# Navigation
nav_content = wv.render(
    wv.link("Home", "/"),
    wv.link("About", "/about")
)
wv.nav(nav_content, "navbar")

# Header
wv.header(wv.h1("Site Title"), "site-header")

# Footer
wv.footer("© 2025 My Site", "site-footer")

# Main content
wv.main(content, "main-content")

# Section
wv.section(content, "content-section")

# Article
wv.article(post_content, "blog-post")

# Aside
wv.aside(sidebar_content, "sidebar")
```

### HTML Structure

```python
# Complete HTML document structure
wv.html(
    wv.render(
        wv.head(head_content, lang="en"),
        wv.body(body_content)
    )
)

# Head elements
wv.title("Page Title")
wv.meta({"charset": "utf-8"})
wv.meta({"name": "viewport", "content": "width=device-width, initial-scale=1"})

# Style and script tags
wv.style("body { margin: 0; }")
wv.script("console.log('Hello');")
```

## CDN Integration

WebViewTK provides easy integration with popular CSS and JavaScript frameworks:

### Tailwind CSS
```python
wv.cdn_tailwind()  # Latest version
wv.cdn_tailwind("3.3.0")  # Specific version
```

### Bootstrap
```python
wv.cdn_bootstrap()  # Includes both CSS and JS
wv.cdn_bootstrap("5.3.0")
```

### jQuery
```python
wv.cdn_jquery()
wv.cdn_jquery("3.7.0")
```

### Vue.js
```python
wv.cdn_vue()
wv.cdn_vue("3.3.4")
```

### React
```python
wv.cdn_react()  # Includes React and ReactDOM
wv.cdn_react("18.2.0")
```

### Alpine.js
```python
wv.cdn_alpine()
wv.cdn_alpine("3.x.x")
```

### Custom CDN
```python
# CSS file
wv.cdn_custom("https://example.com/style.css", "style")

# JavaScript file
wv.cdn_custom("https://example.com/script.js", "script")

# Or use specific functions
wv.style_link("https://example.com/style.css")
wv.script_link("https://example.com/script.js")
```

## Window Management

### Creating a Window

```python
# Create window with title, width, height
window = wv.Window("My Application", 1024, 768)

# Set HTML content
window.set_html(html_string)

# Run the window (blocks until closed)
window.run()
```

### Window Properties

```python
# Access window properties (read-only after creation)
window = wv.Window("App", 800, 600)

# These are stored in the window dict:
# - title: Window title
# - width: Window width in pixels
# - height: Window height in pixels
# - html: HTML content string
```

## Complete Examples

### Example 1: Simple Button App

```python
import webviewtk as wv

# Create UI
html_content = wv.div(
    wv.render(
        wv.h1("Click Counter", "text-3xl font-bold mb-4"),
        wv.button("Click Me", "bg-blue-500 text-white px-4 py-2 rounded")
    ),
    "container mx-auto p-8"
)

# Build full page
full_html = "<!DOCTYPE html>" + wv.html(
    wv.render(
        wv.head(wv.render(
            wv.title("Counter App"),
            wv.cdn_tailwind()
        )),
        wv.body(html_content)
    )
)

# Create and run window
window = wv.Window("Counter App", 400, 300)
window.set_html(full_html)
window.run()
```

### Example 2: Form with Bootstrap

```python
import webviewtk as wv

# Build form
form_content = wv.render(
    wv.div(
        wv.render(
            wv.label("Name", "name", "form-label"),
            wv.input("text", "Your name", "form-control", "name")
        ),
        "mb-3"
    ),
    wv.div(
        wv.render(
            wv.label("Email", "email", "form-label"),
            wv.input("email", "your@email.com", "form-control", "email")
        ),
        "mb-3"
    ),
    wv.button("Submit", "btn btn-primary")
)

form = wv.form(form_content, "", "POST", "container mt-5")

# Build page
full_html = "<!DOCTYPE html>" + wv.html(
    wv.render(
        wv.head(wv.render(
            wv.title("Form Example"),
            wv.meta({"charset": "utf-8"}),
            wv.cdn_bootstrap()
        )),
        wv.body(wv.render(
            wv.h1("Contact Form", "text-center my-4"),
            form
        ))
    )
)

# Run window
window = wv.Window("Form App", 600, 500)
window.set_html(full_html)
window.run()
```

### Example 3: Dashboard with Alpine.js

```python
import webviewtk as wv

# Dashboard HTML with Alpine.js
dashboard = """
<div x-data="{ users: 1250, revenue: 45680, orders: 892 }">
    <div class="grid grid-cols-3 gap-6 p-8">
        <div class="bg-white p-6 rounded-lg shadow">
            <h3 class="text-lg font-semibold">Users</h3>
            <p class="text-3xl font-bold text-blue-600" x-text="users"></p>
        </div>
        <div class="bg-white p-6 rounded-lg shadow">
            <h3 class="text-lg font-semibold">Revenue</h3>
            <p class="text-3xl font-bold text-green-600" x-text="'$' + revenue"></p>
        </div>
        <div class="bg-white p-6 rounded-lg shadow">
            <h3 class="text-lg font-semibold">Orders</h3>
            <p class="text-3xl font-bold text-purple-600" x-text="orders"></p>
        </div>
    </div>
</div>
"""

# Build page
full_html = "<!DOCTYPE html>" + wv.html(
    wv.render(
        wv.head(wv.render(
            wv.title("Dashboard"),
            wv.cdn_tailwind(),
            wv.cdn_alpine()
        )),
        wv.body(
            wv.render(
                wv.h1("Analytics Dashboard", "text-4xl font-bold text-center my-8"),
                dashboard
            ),
            "bg-gray-100 min-h-screen"
        )
    )
)

# Run window
window = wv.Window("Dashboard", 1200, 800)
window.set_html(full_html)
window.run()
```

## Utility Functions

### render()
Combines multiple HTML strings into one:
```python
combined = wv.render(
    wv.h1("Title"),
    wv.p("Paragraph 1"),
    wv.p("Paragraph 2")
)
```

### escape_html()
Escapes HTML special characters for security:
```python
safe_text = wv.escape_html("<script>alert('xss')</script>")
```

## Widget Function Parameters

All widget functions follow this pattern:
```python
widget(
    content="",      # Main content (text or HTML)
    classes="",      # CSS classes
    id="",           # Element ID
    style="",        # Inline CSS styles
    attrs={}         # Dictionary of additional HTML attributes
)
```

Example with all parameters:
```python
wv.div(
    "Content here",
    classes="container mx-auto",
    id="main-div",
    style="padding: 20px;",
    attrs={"data-value": "123", "role": "main"}
)
```

## Security

All text content is automatically HTML-escaped to prevent XSS attacks. If you need to include raw HTML, use the `render()` function with pre-built HTML from other widget functions.

## Performance Tips

1. **Build HTML once**: Create your HTML structure before calling `window.run()`
2. **Use CDNs**: CDN-hosted libraries load faster than bundled files
3. **Minimize inline styles**: Use CSS frameworks like Tailwind or Bootstrap
4. **Release builds**: Always use `--release` flag for production builds

## Troubleshooting

### Window doesn't display
- Make sure you compiled with `--features webviewtk`
- Check that HTML content is not empty
- Verify you called `window.set_html()` before `window.run()`

### Missing dependencies on Windows
- Install WebView2 Runtime: https://developer.microsoft.com/microsoft-edge/webview2/

### Build errors
- Make sure all dependencies are updated: `cargo update`
- Clean build directory: `cargo clean`

## Platform Support

WebViewTK works on:
- ✅ Windows (WebView2)
- ✅ macOS (WebKit)
- ✅ Linux (WebKitGTK)

## Examples

Find more examples in the `examples/` directory:
- `test_webviewtk.py` - Comprehensive examples
- `test_webviewtk_simple.py` - HTML generation test
- `test_window_simple.py` - Simple window example

## License

WebViewTK is part of the Tauraro project and is licensed under MIT OR Apache-2.0.
