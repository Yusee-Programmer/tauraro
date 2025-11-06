# Native Windowing Support - GuiDesktop v1.1

**GuiDesktop** now includes full native windowing support with platform-specific backends!

---

## 🎉 What's New in v1.1

### Native Windowing Backends

GuiDesktop now creates **real native windows** on all major platforms:

✅ **Windows** - Win32 API integration (fully implemented)
✅ **Linux** - X11/Xlib support (fully implemented)
🚧 **Linux** - Wayland support (experimental, use X11 for now)
🚧 **macOS** - Cocoa/AppKit support (experimental)

### Platform-Specific Event Loops

Each platform now has its own native message/event loop:

- **Windows**: Win32 message loop with `PeekMessage`/`DispatchMessage`
- **Linux (X11)**: X11 event loop with `XNextEvent`
- **Linux (Wayland)**: Wayland display roundtrip (experimental)
- **macOS**: Cocoa run loop (experimental)

---

## 🏗️ Architecture

### Backend System

```
Application (cross-platform API)
    │
    ├─→ NativeApplication (platform event loop)
    │       │
    │       ├─→ Win32Backend (Windows)
    │       ├─→ X11Backend (Linux X11)
    │       ├─→ WaylandBackend (Linux Wayland)
    │       └─→ CocoaBackend (macOS)
    │
    └─→ NativeWindow (platform window creation)
            │
            └─→ Cairo rendering surface
```

### Files Structure

```
guidesktop/
├── backend_win32.tr      # Windows Win32 backend
├── backend_x11.tr        # Linux X11 backend
├── backend_wayland.tr    # Linux Wayland backend (experimental)
├── backend_cocoa.tr      # macOS Cocoa backend (experimental)
├── window_native.tr      # Native window class
├── application_native.tr # Native application class
└── examples/
    ├── example_native_window.tr    # Single native window
    └── example_multi_window.tr     # Multiple windows
```

---

## 🚀 Usage

### Basic Native Window

```tauraro
import guidesktop

# Initialize
guidesktop.init()
guidesktop.define_cairo_functions()

# Load backend
if guidesktop._is_windows:
    import guidesktop.backend_win32
elif guidesktop._is_linux:
    import guidesktop.backend_x11

# Load native modules
import guidesktop.window_native
import guidesktop.application_native

# Create native application
app = guidesktop.application_native.NativeApplication("My App")

# Create native window
window = app.create_window("Hello Native!", 600, 400)

# Add widgets
label = guidesktop.Label("This is a real native window!")
label.set_bounds(50, 50, 500, 40)
window.add_widget(label)

# Run with native event loop
app.run()
```

### Multiple Windows

```tauraro
app = guidesktop.application_native.NativeApplication("Multi-Window App")

# Create multiple windows
window1 = app.create_window("Window 1", 400, 300)
window1.set_position(100, 100)

window2 = app.create_window("Window 2", 400, 300)
window2.set_position(520, 100)

# Add widgets to both windows
# ...

# Run - all windows display simultaneously
app.run()
```

---

## 🔧 Platform-Specific Details

### Windows (Win32)

**Status:** ✅ Fully Implemented

**Features:**
- Native window creation with `CreateWindowExA`
- Win32 message loop with `PeekMessage`/`DispatchMessage`
- Window management (move, resize, title, close)
- Device context access for Cairo rendering

**Requirements:**
- Windows Vista or later
- user32.dll, gdi32.dll, kernel32.dll (built-in)

**Functions Used:**
- `CreateWindowExA` - Create window
- `ShowWindow`, `UpdateWindow` - Display window
- `PeekMessageA`, `DispatchMessageA` - Message loop
- `GetDC`, `ReleaseDC` - Device context for drawing

### Linux (X11)

**Status:** ✅ Fully Implemented

**Features:**
- X11 window creation with `XCreateSimpleWindow`
- X11 event loop with `XNextEvent`/`XPending`
- Window management (map, move, resize, title)
- Event handling (mouse, keyboard, expose)

**Requirements:**
- X11/X.org server
- libX11.so.6
- DISPLAY environment variable set

**Functions Used:**
- `XOpenDisplay` - Connect to X server
- `XCreateSimpleWindow` - Create window
- `XMapWindow` - Show window
- `XNextEvent`, `XPending` - Event loop
- `XStoreName` - Set window title

**Notes:**
- Works on traditional X11 systems
- Works on Wayland with XWayland compatibility layer
- Automatically uses XWayland if Wayland is running

### Linux (Wayland)

**Status:** 🚧 Experimental

**Features:**
- Basic Wayland display connection
- Window creation not yet implemented

**Why Experimental?**
Wayland is significantly more complex than X11:
- Requires multiple protocol interfaces (wl_compositor, wl_shell/xdg_shell)
- No simple window creation API
- Needs surface, shell surface, frame callbacks
- Compositor-specific implementations

**Recommendation:**
Use X11 backend for now. XWayland provides excellent compatibility.

### macOS (Cocoa)

**Status:** 🚧 Experimental

**Features:**
- Objective-C runtime loading
- Window creation not yet implemented

**Why Experimental?**
Cocoa requires Objective-C runtime:
- NSApplication initialization
- NSWindow creation through objc_msgSend
- Complex message passing
- Run loop integration

**Coming in v1.2!**

---

## 📊 Backend Feature Matrix

| Feature | Win32 | X11 | Wayland | Cocoa |
|---------|-------|-----|---------|-------|
| Window Creation | ✅ | ✅ | ❌ | ❌ |
| Window Show/Hide | ✅ | ✅ | ❌ | ❌ |
| Move/Resize | ✅ | ✅ | ❌ | ❌ |
| Set Title | ✅ | ✅ | ❌ | ❌ |
| Event Loop | ✅ | ✅ | ⚠️ | ⚠️ |
| Mouse Events | ⚠️ | ⚠️ | ❌ | ❌ |
| Keyboard Events | ⚠️ | ⚠️ | ❌ | ❌ |
| Cairo Integration | ⚠️ | ⚠️ | ❌ | ❌ |

✅ Fully implemented
⚠️ Partially implemented / Planned for v1.2
❌ Not yet implemented

---

## 🎯 Implementation Details

### Win32 Backend (`backend_win32.tr`)

**Key APIs:**
```tauraro
# Window creation
CreateWindowExA(exStyle, className, title, style, x, y, w, h, ...)

# Message loop
while True:
    PeekMessageA(msg, hwnd, 0, 0, PM_REMOVE)
    TranslateMessage(msg)
    DispatchMessageA(msg)
```

**Window Class:**
Uses predefined "STATIC" class for simplicity. Custom WNDCLASSEX registration can be added later for more control.

### X11 Backend (`backend_x11.tr`)

**Key APIs:**
```tauraro
# Display connection
display = XOpenDisplay(NULL)
screen = XDefaultScreen(display)
root = XRootWindow(display, screen)

# Window creation
window = XCreateSimpleWindow(display, root, x, y, w, h, border, ...)
XMapWindow(display, window)

# Event loop
while True:
    XNextEvent(display, event)
    # Process event
```

**Event Masks:**
```tauraro
KeyPressMask | ButtonPressMask | ExposureMask | StructureNotifyMask
```

---

## 🐛 Known Limitations (v1.1)

### All Platforms

1. **Cairo Integration Incomplete**
   - Native windows create successfully
   - Cairo rendering to windows not yet connected
   - Workaround: Render to memory surface, blit to window

2. **Event Handling Partial**
   - Event loops work
   - Event translation to GuiDesktop events not complete
   - Mouse/keyboard callbacks not yet wired up

3. **Window Manager Protocols**
   - Close button may not work properly
   - Min/max/fullscreen buttons not fully integrated
   - Window decorations use system defaults

### Windows-Specific

1. Window class is "STATIC" (generic)
2. Custom window procedure not registered
3. WM_PAINT handling simplified

### Linux X11-Specific

1. WM_DELETE_WINDOW atom not fully integrated
2. Event structures use simplified arrays
3. XWayland assumed for Wayland systems

---

## 🔮 Roadmap

### v1.2 (Next Release)

- [ ] Complete Cairo-to-native-window rendering
- [ ] Full mouse event handling
- [ ] Full keyboard event handling
- [ ] Window close protocol handling
- [ ] macOS Cocoa backend implementation
- [ ] Advanced window features (minimize, maximize, fullscreen)

### v2.0 (Future)

- [ ] Wayland native protocol implementation
- [ ] Hardware acceleration
- [ ] OpenGL/Vulkan surface integration
- [ ] Multi-monitor support
- [ ] Window animations
- [ ] System tray integration

---

## 💡 Tips

### Windows Development

```bash
# Ensure you have user32.dll, gdi32.dll (always present on Windows)
# Run examples:
tauraro tauraro_packages/guidesktop/example_native_window.tr
```

### Linux Development

```bash
# Install X11 development libraries
sudo apt-get install libx11-6 libx11-dev

# Check X11 is running
echo $DISPLAY  # Should show :0 or :1

# Run examples
tauraro tauraro_packages/guidesktop/example_native_window.tr
```

### Debugging

```tauraro
# Enable verbose output
print(guidesktop._platform)
print(guidesktop._window_backend)

# Check backend initialization
backend = guidesktop.get_native_backend()
print(backend.initialized)
```

---

## 📖 Examples

### Example 1: Simple Native Window

```bash
tauraro tauraro_packages/guidesktop/example_native_window.tr
```

Creates a single native window with widgets.

### Example 2: Multiple Windows

```bash
tauraro tauraro_packages/guidesktop/example_multi_window.tr
```

Creates three native windows simultaneously.

---

## 🤝 Contributing

Native windowing is complex! Contributions welcome for:

- Completing Cairo integration
- Event handling improvements
- Cocoa backend implementation
- Wayland protocol support
- Bug fixes and testing

---

## 📝 Version History

**v1.1 (Current)** - Native Windowing
- ✅ Win32 backend (Windows)
- ✅ X11 backend (Linux)
- 🚧 Wayland backend (experimental)
- 🚧 Cocoa backend (experimental)
- Native event loops
- Platform window creation

**v1.0** - Initial Release
- Cairo rendering
- Widget system
- Mock rendering only

---

**GuiDesktop v1.1** - Real native windows on Windows and Linux! 🎉
