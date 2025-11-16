# WebViewTK CDN Management Guide

## Overview

WebViewTK now includes comprehensive CDN management allowing you to easily include popular CSS/JS frameworks, custom files, and inline code in your applications.

## Available CDN Constants

Access popular CDNs through the `CDN` dictionary:

### CSS Frameworks
- `CDN["TAILWIND_CSS"]` - Tailwind CSS 3.4.1
- `CDN["BOOTSTRAP_CSS"]` - Bootstrap 5.3.2
- `CDN["BULMA_CSS"]` - Bulma 0.9.4
- `CDN["FOUNDATION_CSS"]` - Foundation 6.8.1
- `CDN["MATERIAL_CSS"]` - Material Web 1.0.0
- `CDN["SEMANTIC_UI_CSS"]` - Semantic UI 2.5.0

### JavaScript Frameworks
- `CDN["ALPINE_JS"]` - Alpine.js 3.13.3
- `CDN["HTMX"]` - HTMX 1.9.10
- `CDN["VUE_JS"]` - Vue.js 3.4.15
- `CDN["REACT_JS"]` - React 18.2.0
- `CDN["JQUERY"]` - jQuery 3.7.1
- `CDN["BOOTSTRAP_JS"]` - Bootstrap JS 5.3.2

### Icon Libraries
- `CDN["FONT_AWESOME"]` - Font Awesome 6.5.1
- `CDN["MATERIAL_ICONS"]` - Material Icons
- `CDN["BOOTSTRAP_ICONS"]` - Bootstrap Icons 1.11.3

### Google Fonts
- `CDN["GOOGLE_FONTS_INTER"]` - Inter font family
- `CDN["GOOGLE_FONTS_ROBOTO"]` - Roboto font family
- `CDN["GOOGLE_FONTS_POPPINS"]` - Poppins font family

### Animation Libraries
- `CDN["ANIMATE_CSS"]` - Animate.css 4.1.1
- `CDN["GSAP"]` - GSAP 3.12.5

### Utility Libraries
- `CDN["LODASH"]` - Lodash 4.17.21
- `CDN["MOMENT_JS"]` - Moment.js 2.30.1
- `CDN["DAY_JS"]` - Day.js 1.11.10
- `CDN["AXIOS"]` - Axios 1.6.5

### Chart Libraries
- `CDN["CHART_JS"]` - Chart.js 4.4.1
- `CDN["APEXCHARTS"]` - ApexCharts 3.45.2

## Functions

### `include_cdn(window, cdn_url)`
Include a CDN link in your window. Accepts single URL or list of URLs.

**Parameters:**
- `window` - The Window object
- `cdn_url` - CDN URL string or CDN constant

**Example:**
```python
from webviewtk import Window, include_cdn, CDN

window = Window(title="My App", width=1200, height=800)

# Single CDN
include_cdn(window, CDN["BOOTSTRAP_CSS"])

# Multiple CDNs
include_cdn(window, CDN["BOOTSTRAP_JS"])
include_cdn(window, CDN["BOOTSTRAP_ICONS"])

# Custom CDN URL
include_cdn(window, "https://cdn.example.com/my-library.css")
```

### `include_css_file(window, file_path)`
Include a CSS file from your local filesystem.

**Parameters:**
- `window` - The Window object
- `file_path` - Path to CSS file (relative or absolute)

**Example:**
```python
from webviewtk import Window, include_css_file

window = Window(title="My App", width=1200, height=800)

# Relative path
include_css_file(window, "styles/custom.css")

# Absolute path
include_css_file(window, "C:/projects/myapp/styles/theme.css")
```

### `include_js_file(window, file_path)`
Include a JavaScript file from your local filesystem.

**Parameters:**
- `window` - The Window object
- `file_path` - Path to JS file (relative or absolute)

**Example:**
```python
from webviewtk import Window, include_js_file

window = Window(title="My App", width=1200, height=800)

include_js_file(window, "scripts/app.js")
include_js_file(window, "scripts/utils.js")
```

### `include_html_file(window, file_path)`
Include HTML content from a file (inserted at top of body).

**Parameters:**
- `window` - The Window object
- `file_path` - Path to HTML file

**Example:**
```python
from webviewtk import Window, include_html_file

window = Window(title="My App", width=1200, height=800)

# Include custom HTML snippet
include_html_file(window, "templates/header.html")
```

### `add_custom_css(window, css_code)`
Add inline CSS code to your window.

**Parameters:**
- `window` - The Window object
- `css_code` - CSS code as string

**Example:**
```python
from webviewtk import Window, add_custom_css

window = Window(title="My App", width=1200, height=800)

add_custom_css(window, """
.custom-button {
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    border: none;
    color: white;
    padding: 12px 24px;
    border-radius: 8px;
    cursor: pointer;
}
.custom-button:hover {
    transform: translateY(-2px);
    box-shadow: 0 10px 20px rgba(0,0,0,0.2);
}
""")
```

### `add_custom_js(window, js_code)`
Add inline JavaScript code to your window.

**Parameters:**
- `window` - The Window object
- `js_code` - JavaScript code as string

**Example:**
```python
from webviewtk import Window, add_custom_js

window = Window(title="My App", width=1200, height=800)

add_custom_js(window, """
function initializeApp() {
    console.log('App initialized!');
    // Your JavaScript code here
}

document.addEventListener('DOMContentLoaded', initializeApp);
""")
```

## Raw HTML in Text Widget

Use `raw_html=True` to render HTML directly without escaping:

**Example:**
```python
from webviewtk import Text

# Regular text (HTML escaped)
text1 = Text("<h1>Hello</h1>")  # Displays: <h1>Hello</h1>

# Raw HTML (rendered as HTML)
text2 = Text("<h1>Hello</h1>", raw_html=True)  # Displays: Hello (as H1 heading)

# Bootstrap components
text3 = Text(
    '<button class="btn btn-primary">Click Me</button>',
    raw_html=True
)
```

## Complete Example: Bootstrap Dashboard

```python
from webviewtk import (
    Window, Container, Column, Text, mount_and_run,
    include_cdn, add_custom_css, CDN
)

# Create window
window = Window(
    title="Bootstrap Dashboard",
    width=1400,
    height=900,
    resizable=True
)

# Include Bootstrap framework
include_cdn(window, CDN["BOOTSTRAP_CSS"])
include_cdn(window, CDN["BOOTSTRAP_JS"])
include_cdn(window, CDN["BOOTSTRAP_ICONS"])
include_cdn(window, CDN["GOOGLE_FONTS_INTER"])

# Add custom styles
add_custom_css(window, """
body {
    font-family: 'Inter', sans-serif;
}
.card {
    transition: transform 0.2s;
}
.card:hover {
    transform: translateY(-5px);
    box-shadow: 0 8px 16px rgba(0,0,0,0.1);
}
""")

# Build UI with Bootstrap components
ui = Container(
    child=Column(
        children=[
            # Header
            Text(
                '<div class="container-fluid">'
                '<div class="row bg-primary text-white py-4">'
                '<div class="col">'
                '<h2><i class="bi bi-speedometer2 me-2"></i>Dashboard</h2>'
                '<p class="mb-0">Welcome back!</p>'
                '</div></div></div>',
                raw_html=True
            ),
            
            # Stats cards
            Text(
                '<div class="container-fluid mt-4">'
                '<div class="row g-4">'
                '<div class="col-md-3">'
                '<div class="card border-primary">'
                '<div class="card-body text-center">'
                '<i class="bi bi-currency-dollar" style="font-size: 3rem;"></i>'
                '<h3 class="mt-3">$54,239</h3>'
                '<p class="text-muted">Revenue</p>'
                '</div></div></div>'
                # ... more cards
                '</div></div>',
                raw_html=True
            )
        ],
        cross_axis_alignment="stretch"
    )
)

mount_and_run(window, ui)
```

## Complete Example: Custom Files

```python
from webviewtk import (
    Window, Container, Text, mount_and_run,
    include_css_file, include_js_file, add_custom_css
)

window = Window(title="Custom Theme App", width=1200, height=800)

# Include local theme files
include_css_file(window, "assets/theme.css")
include_css_file(window, "assets/components.css")
include_js_file(window, "assets/app.js")

# Add inline customizations
add_custom_css(window, """
:root {
    --primary-color: #6366f1;
    --secondary-color: #8b5cf6;
}
""")

ui = Container(
    child=Text('<div class="custom-layout">Content</div>', raw_html=True)
)

mount_and_run(window, ui)
```

## Tips & Best Practices

### 1. **Order Matters**
Include CDNs before custom CSS to allow overrides:
```python
include_cdn(window, CDN["BOOTSTRAP_CSS"])  # First
add_custom_css(window, ".btn { ... }")     # Then override
```

### 2. **Combine Multiple Frameworks Carefully**
Some frameworks may conflict. Test thoroughly:
```python
# Safe: Bootstrap + Font Awesome
include_cdn(window, CDN["BOOTSTRAP_CSS"])
include_cdn(window, CDN["FONT_AWESOME"])

# Potential conflict: Bootstrap + Tailwind
# Use only one major CSS framework
```

### 3. **Use Raw HTML for Complex Layouts**
For Bootstrap/complex HTML, use `raw_html=True`:
```python
Text('<div class="card">...</div>', raw_html=True)
```

### 4. **Organize Custom Files**
Keep assets organized:
```
myapp/
├── app.tr
├── styles/
│   ├── theme.css
│   └── components.css
└── scripts/
    ├── app.js
    └── utils.js
```

### 5. **Leverage Browser DevTools**
Use browser debugging to inspect the rendered HTML and styles.

## Supported Browsers

WebViewTK uses the native webview on each platform:
- **Windows**: Edge WebView2 (Chromium)
- **macOS**: WKWebView (Safari/WebKit)
- **Linux**: WebKitGTK

All modern CSS/JS features are supported!

## Troubleshooting

### CDN not loading
- Check internet connection
- Verify CDN URL is correct
- Check browser console for errors

### Custom file not found
- Use absolute paths for reliability
- Check file permissions
- Verify file encoding (UTF-8 recommended)

### Styles not applying
- Check CSS specificity
- Verify CDN loaded before custom styles
- Inspect element in browser DevTools

### Raw HTML not rendering
- Ensure `raw_html=True` is set
- Check HTML syntax
- Verify closing tags

## Next Steps

- Explore more CDN libraries
- Create reusable component templates
- Build complex dashboard applications
- Integrate with external APIs

Happy coding! 🚀
