# CustomTitleBar Component - Known Limitations

## Window Control Buttons

### Current Status
The CustomTitleBar component displays window control buttons (minimize, maximize, close), but **minimize and maximize are not currently functional** due to WRY library limitations.

### What Works
- ✅ **Close button** - Fully functional (terminates the application)
- ✅ **Titlebar appearance** - Custom styling, colors, and layout
- ✅ **Drag region** - Window can be dragged by the titlebar (via `-webkit-app-region: drag`)
- ✅ **Native titlebar toggle** - `native_titlebar=False` removes OS titlebar

### What Doesn't Work
- ❌ **Minimize button** - Clicks are detected but window doesn't minimize
- ❌ **Maximize/Restore button** - Clicks are detected but window doesn't maximize/restore

### Technical Explanation

The WRY library's IPC handler (`with_ipc_handler`) receives messages from JavaScript but **does not provide access to the window instance** needed to call minimize/maximize methods. The handler signature is:

```rust
.with_ipc_handler(move |_window, message| {
    // _window parameter doesn't actually give us window control!
    match message.as_str() {
        "window:close" => std::process::exit(0),  // This works
        "window:minimize" => {
            // No way to access window.set_minimized(true)
        }
        "window:maximize" => {
            // No way to access window.set_maximized(true)
        }
        _ => {}
    }
})
```

### Workarounds

#### Option 1: Use Native Titlebar (Recommended)
For production apps where minimize/maximize is important:

```python
window = Window(
    title="My App",
    width=1200,
    height=800,
    native_titlebar=True  # Use OS titlebar
)
```

#### Option 2: Frameless Window Without Controls
For kiosk-style or full-screen apps:

```python
window = Window(
    title="My App",
    width=1200,
    height=800,
    native_titlebar=False
)

# Use CustomTitleBar with only close button
ui = Column(children=[
    CustomTitleBar(
        title="My App",
        show_minimize=False,  # Hide non-functional buttons
        show_maximize=False,
        show_close=True
    ),
    # ... content
])
```

#### Option 3: Custom Keyboard Shortcuts
Implement keyboard shortcuts for window management:

```javascript
document.addEventListener('keydown', (e) => {
    if (e.ctrlKey && e.key === 'm') {
        // Minimize - not currently possible
    }
    if (e.key === 'F11') {
        // Fullscreen toggle - not currently possible
    }
});
```

### Future Solutions

Possible approaches to enable full window control:

1. **WRY Enhancement**: Modify WRY to pass window reference to IPC handler
2. **Event System**: Implement a custom event bus that connects IPC to window methods
3. **External Process**: Use IPC to communicate with external process that has window handle
4. **Platform-Specific**: Use platform-specific APIs (Win32, X11, Cocoa) via FFI

### Usage Recommendations

**For Desktop Apps:**
- Use `native_titlebar=True` (default)
- Let the OS handle window controls
- Focus on app content and functionality

**For Branded/Custom UI:**
- Use `native_titlebar=False` with `CustomTitleBar`
- Hide minimize/maximize buttons (`show_minimize=False, show_maximize=False`)
- Keep only the close button functional
- Consider fullscreen mode for immersive experiences

**For Kiosk/Embedded:**
- Use frameless window (`native_titlebar=False`)
- No CustomTitleBar needed
- App controls exit/restart programmatically

### Example: Production-Ready CustomTitleBar

```python
from webviewtk import Window, Column, Text, CustomTitleBar, mount_and_run

window = Window(
    title="My App",
    width=1200,
    height=800,
    native_titlebar=False,
    resizable=True
)

ui = Column(children=[
    CustomTitleBar(
        title="My App - v1.0.0",
        height=40,
        background_color="#1e293b",
        text_color="#f1f5f9",
        show_minimize=False,  # Hide until functional
        show_maximize=False,  # Hide until functional
        show_close=True       # Keep close button
    ),
    # Your app content here
])

mount_and_run(window, ui)
```

### Testing

You can test the current behavior with:

```bash
tauraro run test_custom_titlebar_visibility.tr
```

**Expected behavior:**
- ✅ Custom titlebar appears at top
- ✅ Window can be dragged
- ✅ Close button (×) terminates app
- ⚠️ Minimize button (−) shows console log only
- ⚠️ Maximize button (□) shows console log only

### Contributing

If you'd like to help implement full window control functionality, see:
- WRY repository: https://github.com/tauri-apps/wry
- Tauraro issue tracker: (link to be added)
- Related: Tauri framework has solved this in their architecture

---

**Last Updated:** November 16, 2025  
**Status:** Known limitation, workarounds available
