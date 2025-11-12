# WebViewTK Examples

This directory contains examples demonstrating the WebViewTK framework.

## Prerequisites

Build Tauraro with the webviewtk feature:

```bash
# Release build (recommended)
cargo build --release --features webviewtk

# Debug build
cargo build --features webviewtk
```

**Automatic Dependency Installation**: The build process automatically detects and installs platform-specific dependencies:
- Windows: WebView2 Runtime (Windows 11 has it pre-installed)
- Linux: WebKitGTK
- macOS: WebKit (built-in)

You'll see build warnings showing the dependency status during compilation.

## Running Examples

```bash
# Using release build
./target/release/tauraro.exe run examples/test_window_simple.py

# Using debug build
./target/debug/tauraro.exe run examples/test_window_simple.py
```

## Available Examples

### Quick Launchers

#### launch_all_examples.py
**Launch all comprehensive examples at once**

Demonstrates Tauraro's multi-process capability by launching all 4 comprehensive examples simultaneously!

```bash
./target/release/tauraro.exe run examples/launch_all_examples.py
```

This will open:
- Dashboard Pro (analytics dashboard)
- TechStore (e-commerce store)
- SocialHub (social media feed)
- Portfolio (landing page)

All 4 programs run independently - you can close any window without affecting the others!

#### run_example.py
**Interactive menu to choose examples**

```bash
./target/release/tauraro.exe run examples/run_example.py
```

### Comprehensive Examples (Production-Ready)

#### 1. webviewtk_dashboard.py
**Analytics Dashboard with Animations**

```bash
./target/release/tauraro.exe run examples/webviewtk_dashboard.py
```

Features:
- 📊 Animated statistics cards with gradients
- 📈 Interactive bar chart with animations
- 📝 Real-time activity feed
- 🎨 Quick action buttons with hover effects
- 📱 Fully responsive (mobile & desktop)
- ✨ Smooth fade-in and slide-in animations

#### 2. webviewtk_ecommerce.py
**E-Commerce Store with Shopping Cart**

```bash
./target/release/tauraro.exe run examples/webviewtk_ecommerce.py
```

Features:
- 🛍️ Product gallery with category filters
- 🛒 Animated shopping cart drawer
- 🔍 Real-time search functionality
- ⭐ Product ratings and reviews
- 📦 Add/remove items with quantity controls
- 💳 Cart total calculation

#### 3. webviewtk_social_media.py
**Social Media Feed with Interactive Posts**

```bash
./target/release/tauraro.exe run examples/webviewtk_social_media.py
```

Features:
- 📱 Instagram-style interface
- 📸 Stories carousel with view tracking
- ❤️ Like button with heart animation
- 💬 Expandable comments section
- 🔖 Bookmark functionality
- ✍️ Real-time comment posting

#### 4. webviewtk_portfolio.py
**Modern Portfolio/Landing Page**

```bash
./target/release/tauraro.exe run examples/webviewtk_portfolio.py
```

Features:
- 🎨 Animated gradient hero section
- 📜 Smooth scroll navigation
- 🚀 Project showcase grid
- 📊 Animated skill progress bars
- 📧 Contact form with validation
- ✨ Staggered entrance animations

See `README_WEBVIEWTK_EXAMPLES.md` for detailed documentation of all comprehensive examples.

### Basic Examples

#### test_window_simple.py
**Simple window with Tailwind CSS**

Opens a window with a clean, modern interface using Tailwind CSS.

```bash
./target/release/tauraro.exe run examples/test_window_simple.py
```

Features:
- Tailwind CSS styling
- Gradient background
- Responsive layout
- Button elements

#### test_webviewtk_simple.py
**HTML generation test (no GUI)**

Tests the HTML generation functions without opening a window.

```bash
./target/release/tauraro.exe run examples/test_webviewtk_simple.py
```

Features:
- Tests all widget functions
- CDN link generation
- Form elements
- Complex structures
- Output shows generated HTML

#### test_webviewtk.py
**Comprehensive examples**

Shows all WebViewTK features with multiple examples.

```bash
./target/release/tauraro.exe run examples/test_webviewtk.py
```

Features:
- Simple Hello World
- Tailwind CSS integration
- Bootstrap form
- Complex layout with semantic HTML5
- Interactive dashboard with Alpine.js
- Window creation (commented out by default)

To actually open a window, uncomment the `window.run()` line at the end of `example_window()` function.

#### test_multiple_windows.py
**Multiple Windows Test**

Demonstrates running 4 windows simultaneously within a single program!

```bash
./target/release/tauraro.exe run examples/test_multiple_windows.py
```

Features:
- Creates 4 independent windows (Blue, Green, Purple, Red)
- Each window runs in its own thread
- Close any window individually
- Demonstrates multi-window support

## Quick Start

Here's a minimal example to get started:

```python
import webviewtk as wv

# Create UI
html = "<!DOCTYPE html>" + wv.html(
    wv.render(
        wv.head(wv.cdn_tailwind()),
        wv.body(
            wv.h1("Hello, Tauraro!", "text-4xl font-bold text-center mt-20")
        )
    )
)

# Create and run window
window = wv.Window("My App", 800, 600)
window.set_html(html)
window.run()
```

Save this as `my_app.py` and run:
```bash
./target/release/tauraro.exe run my_app.py
```

## Multi-Window Support 🎉

Tauraro WebViewTK now supports running multiple windows and programs simultaneously!

### Multiple Programs at Once

You can now run multiple Tauraro programs at the same time without having to kill existing processes:

```bash
# Terminal 1
./target/release/tauraro.exe run examples/webviewtk_dashboard.py

# Terminal 2 (at the same time!)
./target/release/tauraro.exe run examples/webviewtk_ecommerce.py

# Terminal 3 (even more!)
./target/release/tauraro.exe run examples/webviewtk_social_media.py
```

All three programs run independently! Close any window, and the others keep running.

### Multiple Windows in One Program

You can also create multiple windows within a single program:

```python
import webviewtk as wv
import time

# Create Window 1
window1 = wv.Window("Window 1", 600, 400)
window1.set_html("<!DOCTYPE html><html><body><h1>Window 1</h1></body></html>")
window1.run()

# Create Window 2
window2 = wv.Window("Window 2", 600, 400)
window2.set_html("<!DOCTYPE html><html><body><h1>Window 2</h1></body></html>")
window2.run()

# Keep program alive
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Closing...")
```

See `test_multiple_windows.py` for a complete example with 4 windows!

### How It Works

Each window runs in its own thread with an independent event loop:
- **Thread-based**: Each `window.run()` spawns a new thread
- **Non-blocking**: You can create multiple windows sequentially
- **Independent lifecycle**: Close any window without affecting others
- **Platform-optimized**: Uses Windows-specific APIs for best performance

See `MULTI_WINDOW_SUPPORT.md` for technical details.

## Common Issues

### Window doesn't appear
- Make sure you built with `--features webviewtk`
- Check that you called `window.set_html()` before `window.run()`
- Verify HTML content is not empty

### Missing dependencies (Windows)
Install Microsoft Edge WebView2 Runtime:
https://developer.microsoft.com/microsoft-edge/webview2/

### Build errors
```bash
# Clean and rebuild
cargo clean
cargo build --release --features webviewtk
```

## Documentation

See `docs/WEBVIEWTK_GUIDE.md` for complete documentation.

## Platform Support

- ✅ Windows (WebView2)
- ✅ macOS (WebKit)
- ✅ Linux (WebKitGTK)
