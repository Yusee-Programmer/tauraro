# GuiDesktop - Custom Cross-Platform GUI Framework

A modern, custom-built desktop GUI framework for the Tauraro programming language, built from scratch using **Cairo** for 2D rendering.

**Version:** 1.0.0
**License:** Part of the Tauraro programming language project

---

## 🎯 Overview

GuiDesktop is a **completely custom GUI framework** designed specifically for Tauraro. Unlike wrapper frameworks that use GTK or Qt, GuiDesktop is built from the ground up with:

- ✨ **Cairo 2D Rendering** - Professional vector graphics across all platforms
- 🏗️ **Custom Widget System** - Purpose-built widget hierarchy
- 📐 **Flexible Layout Engine** - VBox, HBox, and Grid layouts
- 🎨 **Built-in Theming** - Customizable colors and styles
- ⚡ **FFI-Based** - Uses Tauraro's FFI for native library integration
- 🖥️ **Cross-Platform Ready** - Architecture supports Windows, Linux, and macOS

---

## 🏛️ Architecture

### Rendering Pipeline

GuiDesktop follows a similar architecture to GTK, using different backends per platform:

| Platform | Window Backend | Rendering Backend |
|----------|---------------|-------------------|
| **Windows** | Win32 | Cairo + GDI/Direct2D |
| **Linux (X11)** | Xlib/XCB | Cairo + X11 |
| **Linux (Wayland)** | Wayland protocol | Cairo + Wayland compositor |
| **macOS** | Cocoa | Cairo + Quartz |

### Component Stack

```
┌─────────────────────────────────────┐
│      Tauraro Application Code       │
├─────────────────────────────────────┤
│  GuiDesktop High-Level API          │
│  (Widgets, Layouts, Events)         │
├─────────────────────────────────────┤
│     Cairo 2D Rendering Engine       │
├─────────────────────────────────────┤
│   Platform-Specific Window Backend  │
│   (Win32, X11, Wayland, Cocoa)      │
├─────────────────────────────────────┤
│       Tauraro FFI Layer             │
├─────────────────────────────────────┤
│    Native OS APIs & Libraries       │
└─────────────────────────────────────┘
```

---

## 📦 Installation

### Prerequisites

**Cairo** must be installed on your system:

#### Windows
```bash
# Using MSYS2
pacman -S mingw-w64-x86_64-cairo

# Or download from: https://www.cairographics.org/
```

#### Linux
```bash
# Debian/Ubuntu
sudo apt-get install libcairo2 libcairo2-dev

# Fedora
sudo dnf install cairo cairo-devel

# Arch Linux
sudo pacman -S cairo
```

#### macOS
```bash
brew install cairo
```

### Using GuiDesktop

Simply import the package in your Tauraro programs:

```tauraro
import guidesktop
```

---

## 🚀 Quick Start

### Basic Window Example

```tauraro
import guidesktop

# Initialize framework
guidesktop.init()
guidesktop.define_cairo_functions()

# Create application
app = guidesktop.Application("My App")

# Create window
window = app.create_window("Hello World", 400, 300)

# Add a label
label = guidesktop.Label("Hello, GuiDesktop!")
label.set_bounds(50, 50, 300, 40)
label.set_font("Sans", 18)
window.add_widget(label)

# Render to image (native windowing in development)
app.run_mock_render("output.png")
```

### With Layout Managers

```tauraro
import guidesktop

guidesktop.init()
guidesktop.define_cairo_functions()

app = guidesktop.Application("Layout Demo")
window = app.create_window("Layouts", 400, 300)

# Create container with VBox layout
panel = guidesktop.Panel()
panel.set_bounds(10, 10, 380, 280)
window.add_widget(panel)

vbox = guidesktop.VBoxLayout(spacing=10, margin=10)
panel.set_layout(vbox)

# Add widgets
title = guidesktop.Label("Title")
title.height = 40
panel.add_child(title)

button = guidesktop.Button("Click Me")
button.height = 35
panel.add_child(button)

app.run_mock_render("layout.png")
```

---

## 🧩 Core Components

### 1. Color System

```tauraro
# Built-in colors
guidesktop.Colors.WHITE
guidesktop.Colors.BLACK
guidesktop.Colors.RED
guidesktop.Colors.BLUE
guidesktop.Colors.ACCENT
guidesktop.Colors.BACKGROUND

# Custom colors (RGBA, 0.0 to 1.0)
custom_color = guidesktop.Color(0.2, 0.6, 1.0, 1.0)
```

### 2. Geometry Classes

```tauraro
# Rectangle
rect = guidesktop.Rect(x, y, width, height)
if rect.contains(mouse_x, mouse_y):
    print("Point inside!")

# Point
point = guidesktop.Point(x, y)

# Size
size = guidesktop.Size(width, height)
```

### 3. Cairo Renderer

```tauraro
# Get Cairo context from window
ctx = window.cairo_ctx

# Drawing primitives
ctx.set_color(guidesktop.Colors.RED)
ctx.rectangle(10, 10, 100, 50)
ctx.fill()

ctx.draw_rect(10, 70, 100, 50,
              fill_color=guidesktop.Colors.BLUE,
              stroke_color=guidesktop.Colors.BLACK,
              line_width=2.0)

ctx.draw_rounded_rect(10, 130, 100, 50, radius=10,
                      fill_color=guidesktop.Colors.GREEN)

ctx.draw_text(10, 200, "Hello", guidesktop.Colors.BLACK,
              font_family="Sans", font_size=16)
```

---

## 🎨 Widgets

### Widget Hierarchy

```
Widget (base)
├── Container
│   ├── Panel
│   ├── Frame
│   └── Window
├── Label
├── Button
└── TextBox
```

### Label

```tauraro
label = guidesktop.Label("Text here")
label.set_bounds(x, y, width, height)
label.set_font("Sans", 16)
label.text_color = guidesktop.Colors.FOREGROUND
label.set_alignment(0.5, 0.5)  # Center aligned
label.background_color = guidesktop.Colors.WHITE
label.border_width = 1
```

### Button

```tauraro
button = guidesktop.Button("Click Me")
button.set_bounds(x, y, width, height)
button.normal_color = guidesktop.Colors.BUTTON_BG
button.hover_color = guidesktop.Colors.BUTTON_HOVER
button.text_color = guidesktop.Colors.FOREGROUND

# Event handling (when implemented)
button.on_click(lambda event: print("Clicked!"))
```

### TextBox

```tauraro
textbox = guidesktop.TextBox("Initial text")
textbox.set_bounds(x, y, width, height)
textbox.set_placeholder("Enter text...")
textbox.editable = True
textbox.set_font("Sans", 14)

text = textbox.text  # Get current text
textbox.set_text("New text")  # Set text
```

### Panel & Frame

```tauraro
# Simple panel
panel = guidesktop.Panel()
panel.background_color = guidesktop.Colors.BACKGROUND
panel.border_width = 1

# Frame with title
frame = guidesktop.Frame("Frame Title")
frame.set_bounds(x, y, width, height)
```

---

## 📐 Layout Managers

### VBoxLayout (Vertical)

```tauraro
panel = guidesktop.Panel()
vbox = guidesktop.VBoxLayout(spacing=10, margin=5)
panel.set_layout(vbox)

# Add children - they stack vertically
panel.add_child(widget1)
panel.add_child(widget2)
panel.add_child(widget3)
```

### HBoxLayout (Horizontal)

```tauraro
panel = guidesktop.Panel()
hbox = guidesktop.HBoxLayout(spacing=5, margin=0)
panel.set_layout(hbox)

# Add children - they arrange horizontally
panel.add_child(btn1)
panel.add_child(btn2)
panel.add_child(btn3)
```

### GridLayout

```tauraro
panel = guidesktop.Panel()
grid = guidesktop.GridLayout(rows=3, columns=4, spacing=5, margin=5)
panel.set_layout(grid)

# Add children - they fill grid left-to-right, top-to-bottom
for i in range(12):
    panel.add_child(create_widget(i))
```

### AbsoluteLayout

```tauraro
panel = guidesktop.Panel()
absolute = guidesktop.AbsoluteLayout()
panel.set_layout(absolute)

# Widgets keep their manual positions
widget.set_bounds(x, y, width, height)
```

---

## 🎯 Event System

### Event Types

- `EVENT_MOUSE_MOVE` - Mouse movement
- `EVENT_MOUSE_DOWN` - Mouse button press
- `EVENT_MOUSE_UP` - Mouse button release
- `EVENT_MOUSE_ENTER` - Mouse enters widget
- `EVENT_MOUSE_LEAVE` - Mouse leaves widget
- `EVENT_KEY_DOWN` - Key press
- `EVENT_KEY_UP` - Key release
- `EVENT_RESIZE` - Widget resized
- `EVENT_PAINT` - Redraw requested
- `EVENT_FOCUS_IN` - Widget gained focus
- `EVENT_FOCUS_OUT` - Widget lost focus

### Event Handling

```tauraro
# Connect event handler
widget.on_click(my_callback)
widget.on_mouse_move(mouse_handler)
widget.on_key_press(key_handler)

# Custom event handler
def my_callback(event):
    print("Event: " + str(event.type))
    event.mark_handled()  # Stop propagation

widget.connect(EVENT_MOUSE_DOWN, my_callback)
```

---

## 🪟 Window Management

### Window Class

```tauraro
window = guidesktop.Window("Title", width, height)

# Position and size
window.set_position(x, y)
window.set_size(width, height)
window.center()

# State
window.show()
window.hide()
window.maximize()
window.minimize()
window.set_fullscreen(True)

# Add widgets
window.add_widget(widget)
window.remove_widget(widget)

# Rendering
window.render_frame()
window.save_screenshot("output.png")
```

### Application Class

```tauraro
app = guidesktop.Application("My App")

# Create windows
window1 = app.create_window("Window 1", 400, 300)
window2 = app.create_window("Window 2", 600, 400)

# Run application (mock render for v1.0)
app.run_mock_render("output.png")

# Or run event loop (when implemented)
app.run()
app.quit()
```

---

## 📚 Examples

The package includes comprehensive examples:

1. **example_basic.tr** - Simple window with label and button
2. **example_layout.tr** - Demonstrates VBox, HBox, and Grid layouts
3. **example_widgets.tr** - Showcase of all widget types
4. **example_calculator.tr** - Calculator UI with grid layout

Run examples:

```bash
tauraro tauraro_packages/guidesktop/example_basic.tr
```

This will generate a PNG image showing the rendered GUI.

---

## 🔧 Current Version: 1.0.0

### ✅ Implemented Features

- ✅ Cairo 2D rendering engine integration
- ✅ Widget system (Label, Button, TextBox, Panel, Frame)
- ✅ Layout managers (VBox, HBox, Grid, Absolute)
- ✅ Event system architecture
- ✅ Color and theming system
- ✅ Window management classes
- ✅ Application framework
- ✅ Render-to-image functionality
- ✅ Comprehensive documentation
- ✅ Example applications

### 🚧 In Development

- ⏳ Native windowing integration (Win32, X11, Wayland, Cocoa)
- ⏳ Event loop implementation
- ⏳ Mouse and keyboard event handling
- ⏳ Real-time window updates
- ⏳ More widgets (ListView, TreeView, MenuBar, etc.)
- ⏳ Advanced Cairo drawing features
- ⏳ Animation system
- ⏳ Drag and drop

---

## 🎨 Design Philosophy

GuiDesktop follows these principles:

1. **Custom-Built**: Not a wrapper - built specifically for Tauraro
2. **Cairo-Powered**: Professional 2D rendering across platforms
3. **Modern API**: Clean, object-oriented design
4. **Cross-Platform**: Same code works everywhere
5. **Flexible**: Easy to extend and customize
6. **FFI-First**: Leverages Tauraro's FFI capabilities

---

## 🏗️ Architecture Details

### Why Cairo?

Cairo is the perfect choice for a custom GUI framework:

- ✅ Cross-platform (Windows, Linux, macOS)
- ✅ Vector graphics (resolution-independent)
- ✅ Professional rendering quality
- ✅ Extensive API for 2D drawing
- ✅ Used by GTK, Firefox, Chrome
- ✅ Well-documented and stable

### Rendering Workflow

```
1. Application creates widgets
2. Widgets added to window
3. Layout manager positions widgets
4. Cairo renders to surface
5. Surface displayed via platform backend
```

---

## 🤝 Contributing

GuiDesktop is in active development. Contributions welcome for:

- Native windowing backends
- Additional widgets
- Event handling implementation
- Platform testing
- Documentation improvements
- Example applications

---

## 📖 API Reference

### Module Structure

```
guidesktop/
├── __init__.tr          # Core framework, platform detection, colors
├── cairo_renderer.tr    # Cairo 2D rendering bindings
├── events.tr            # Event system
├── widget.tr            # Base widget classes
├── widgets.tr           # Standard widgets
├── layout.tr            # Layout managers
├── window.tr            # Window management
├── application.tr       # Application class
└── examples/            # Example programs
```

### Import Usage

```tauraro
import guidesktop

# Access components
guidesktop.init()
guidesktop.Colors.RED
guidesktop.Label("text")
guidesktop.VBoxLayout()
```

---

## 🐛 Known Limitations (v1.0)

1. **Native Windowing**: Not yet implemented - renders to PNG instead
2. **Event Loop**: Mock event loop - real platform integration coming
3. **Text Metrics**: Simple text rendering without proper metrics
4. **Widget Set**: Limited to basic widgets (more coming)
5. **Animations**: Not yet supported

These will be addressed in upcoming versions!

---

## 🔮 Roadmap

### Version 1.1 (Next Release)
- [ ] Windows native windowing (Win32)
- [ ] Linux X11 windowing
- [ ] Basic event handling
- [ ] Mouse input

### Version 1.2
- [ ] Wayland support
- [ ] macOS Cocoa backend
- [ ] Keyboard input
- [ ] More widgets

### Version 2.0
- [ ] Complete widget set
- [ ] Animation system
- [ ] Advanced theming
- [ ] Accessibility features

---

## 📄 License

Part of the Tauraro programming language project.

## 🔗 Links

- **Tauraro**: https://github.com/Yusee-Programmer/tauraro
- **Cairo Graphics**: https://www.cairographics.org/
- **Documentation**: See this README

---

**GuiDesktop v1.0** - A custom GUI framework built from scratch for Tauraro!
