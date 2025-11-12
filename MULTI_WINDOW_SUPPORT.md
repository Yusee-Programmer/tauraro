# Multi-Window Support in Tauraro WebViewTK

## Overview

Tauraro WebViewTK now supports running multiple windows and programs simultaneously! Each window runs in its own thread, allowing you to:

- Run multiple Tauraro programs at the same time
- Create multiple windows within a single program
- Each window operates independently
- Close windows individually without affecting others

## Technical Implementation

### Key Changes Made

#### 1. Thread-Based Window Management
Each window now spawns in a separate thread instead of blocking the main thread:

```rust
// src/modules/webviewtk/mod.rs (lines 880-960)
fn window_run(args: Vec<Value>) -> Result<Value> {
    // Spawn window in a separate thread
    std::thread::spawn(move || {
        // Platform-specific event loop
        #[cfg(target_os = "windows")]
        let event_loop = EventLoop::<()>::new_any_thread();

        #[cfg(not(target_os = "windows"))]
        let event_loop = EventLoop::<()>::new();

        // Create window and webview
        let window = WindowBuilder::new()
            .with_title(&title)
            .with_inner_size(tao::dpi::LogicalSize::new(width, height))
            .build(&event_loop)
            .expect("Failed to create window");

        let _webview = WebViewBuilder::new(window)
            .expect("Failed to create webview")
            .with_html(&html)
            .expect("Failed to set HTML")
            .build()
            .expect("Failed to build webview");

        // Run event loop in this thread
        event_loop.run(move |event, _, control_flow| {
            *control_flow = ControlFlow::Wait;

            match event {
                Event::WindowEvent {
                    event: WindowEvent::CloseRequested,
                    ..
                } => *control_flow = ControlFlow::Exit,
                _ => {}
            }
        });
    });

    // Small delay to ensure window thread starts
    std::thread::sleep(std::time::Duration::from_millis(100));

    Ok(Value::None)
}
```

#### 2. Platform-Specific Event Loop
On Windows, we use `EventLoop::new_any_thread()` to allow event loops in non-main threads:

```rust
#[cfg(all(feature = "webviewtk", target_os = "windows"))]
use wry::application::platform::windows::EventLoopExtWindows;
```

#### 3. Dict Method Call Fixes
Fixed VM to properly handle method calls on dict objects (Window class):

- **LoadAttr Handler** - Wraps callable dict values in BoundMethod
- **CallMethod Handler** - Checks dict keys for custom methods
- **call_method_slow_path** - Fixed RefCell borrowing with cloning

## Usage Examples

### Multiple Programs Running Simultaneously

```bash
# Terminal 1
./target/debug/tauraro.exe run ./examples/webviewtk_dashboard.py

# Terminal 2
./target/debug/tauraro.exe run ./examples/webviewtk_ecommerce.py

# Terminal 3
./target/debug/tauraro.exe run ./examples/webviewtk_social_media.py
```

All three programs run at the same time without interfering with each other!

### Multiple Windows in One Program

```python
import webviewtk as wv
import time

# Create Window 1
window1 = wv.Window("Window 1", 600, 400)
window1.set_html("<h1>Window 1</h1>")
window1.run()

# Create Window 2
window2 = wv.Window("Window 2", 600, 400)
window2.set_html("<h1>Window 2</h1>")
window2.run()

# Create Window 3
window3 = wv.Window("Window 3", 600, 400)
window3.set_html("<h1>Window 3</h1>")
window3.run()

# Keep program alive while windows run
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("\nClosing all windows...")
```

See `examples/test_multiple_windows.py` for a complete example with 4 windows!

## Benefits

### Before the Fix
- Only one Tauraro program could run at a time
- Running a second program required killing the first
- Single-threaded window management
- EventLoop.run() blocked the entire process

### After the Fix
- ✅ Multiple Tauraro programs run simultaneously
- ✅ Multiple windows within a single program
- ✅ Each window in its own thread
- ✅ Independent window lifecycle (close one, others stay open)
- ✅ Non-blocking window creation
- ✅ Platform-specific optimizations

## Testing

### Test Multiple Windows
```bash
./target/debug/tauraro.exe run ./examples/test_multiple_windows.py
```

This creates 4 windows simultaneously (Blue, Green, Purple, Red), each running in its own thread.

### Test Multiple Programs
Run these in separate terminals:

```bash
# Dashboard with animations
./target/debug/tauraro.exe run ./examples/webviewtk_dashboard.py

# E-commerce store
./target/debug/tauraro.exe run ./examples/webviewtk_ecommerce.py

# Social media feed
./target/debug/tauraro.exe run ./examples/webviewtk_social_media.py

# Portfolio page
./target/debug/tauraro.exe run ./examples/webviewtk_portfolio.py
```

All programs run concurrently without conflicts!

## Technical Notes

### Windows Platform
On Windows, we use `EventLoop::new_any_thread()` from the `EventLoopExtWindows` trait to allow event loops in non-main threads. This is a Windows-specific API that solves the "event loop outside main thread" panic.

### Event Loop Per Window
Each window gets its own event loop that runs in a dedicated thread:

- Independent event processing
- Window-specific control flow
- Clean shutdown on window close
- No interference between windows

### Thread Safety
- Each window's state is isolated to its thread
- No shared mutable state between windows
- Safe concurrent operation

### Memory Management
- Windows are properly cleaned up when closed
- Event loops exit cleanly
- Threads terminate when windows close

## Troubleshooting

### Error: "RefCell already borrowed"
**Fixed!** This was caused by nested borrows when calling dict methods. The fix clones the method before the borrow is released.

### Error: "Event loop outside main thread"
**Fixed!** We now use platform-specific `EventLoop::new_any_thread()` on Windows.

### Windows Don't Display
Make sure you've built with the webviewtk feature:
```bash
cargo build --features webviewtk
```

### Only One Window Shows
This was the original issue - now fixed! All windows should display independently.

## Performance Considerations

### Thread Overhead
- Each window spawns a new thread
- Minimal overhead for typical use cases
- Threads are lightweight on modern systems

### Memory Usage
- Each window has its own webview instance
- HTML/CSS/JavaScript loaded per window
- Reasonable for desktop applications

### Recommended Limits
- Up to 10-20 windows should work fine
- More windows may impact performance
- Test on target hardware

## Future Enhancements

Potential improvements:
- Window communication (IPC between windows)
- Shared state management
- Window pooling for performance
- Custom event handling
- Window positioning API
- Multi-monitor support

## Files Modified

### Core VM Files
- `src/bytecode/vm.rs` (lines 2729-2754, 5222-5252, 7094-7134)
  - LoadAttr handler for dict methods
  - CallMethod handler for dict objects
  - call_method_slow_path RefCell fix

### WebViewTK Module
- `src/modules/webviewtk/mod.rs` (lines 21-22, 880-960)
  - Thread-based window management
  - Platform-specific event loop
  - Window creation and lifecycle

### Module Registration
- `src/modules/mod.rs` (lines 97-98, 142)
  - Always available webviewtk module

## Examples Created

### Test Files
- `examples/test_multiple_windows.py` - 4 windows simultaneously
- `examples/test_webviewtk.py` - Basic WebViewTK demo
- `examples/test_window_display.py` - Simple window test

### Comprehensive Examples
- `examples/webviewtk_dashboard.py` - Analytics dashboard
- `examples/webviewtk_ecommerce.py` - E-commerce store
- `examples/webviewtk_social_media.py` - Social media feed
- `examples/webviewtk_portfolio.py` - Portfolio page

### Documentation
- `examples/README_WEBVIEWTK_EXAMPLES.md` - Comprehensive guide
- `examples/FEATURES_SHOWCASE.md` - Feature matrix
- `examples/run_example.py` - Interactive launcher

## Summary

The multi-window support enhancement makes Tauraro WebViewTK significantly more powerful and flexible. You can now:

1. **Run multiple Tauraro programs** simultaneously
2. **Create multiple windows** in a single program
3. **Each window operates independently** in its own thread
4. **Close windows individually** without affecting others
5. **Non-blocking window creation** for better UX

This brings Tauraro WebViewTK closer to production-ready desktop application capabilities!

---

**Built with ❤️ for the Tauraro Programming Language**
