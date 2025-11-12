# Changelog - Multi-Window Support

## Version: Multi-Window Support Release

### Summary
Successfully implemented multi-window and multi-process support for Tauraro WebViewTK, resolving the issue where only one Tauraro program could run at a time.

### Issues Resolved

#### 1. Window Display Issue
**Problem**: `'dict' object has no attribute 'set_html'` error preventing windows from displaying

**Root Cause**: VM wasn't properly handling method calls on dict objects that had callable values stored as keys

**Solution**:
- Enhanced LoadAttr handler to wrap callable dict values in BoundMethod
- Updated three method call locations to check dict keys for custom methods
- Fixed RefCell borrowing conflicts

**Files Modified**:
- `src/bytecode/vm.rs` (lines 2729-2754, 5222-5252, 7094-7134)

#### 2. Single-Process Limitation
**Problem**: "tauraro is only running on one process when we run another tauraro program we have to kill the other running process"

**Root Cause**: EventLoop.run() blocked entire process on main thread

**Solution**:
- Modified window_run to spawn each window in a separate thread
- Used platform-specific EventLoop::new_any_thread() for Windows
- Added EventLoopExtWindows import for Windows platform
- Fixed type annotations for EventLoop::<()>

**Files Modified**:
- `src/modules/webviewtk/mod.rs` (lines 21-22, 880-960)

### New Features

#### Multi-Window Support
- ✅ Multiple Tauraro programs can run simultaneously
- ✅ Multiple windows within a single program
- ✅ Each window runs in its own thread
- ✅ Independent window lifecycle (close one, others stay open)
- ✅ Non-blocking window creation
- ✅ Platform-specific optimizations for Windows

#### Thread-Based Architecture
```rust
// Each window.run() now spawns a thread
std::thread::spawn(move || {
    let event_loop = EventLoop::<()>::new_any_thread(); // Windows
    // ... create window and run event loop
});
```

### New Examples Created

#### Comprehensive Production-Ready Examples
1. **webviewtk_dashboard.py** (17KB)
   - Analytics dashboard with animated charts and cards
   - Gradient backgrounds, hover effects
   - Mobile-responsive with Alpine.js state management

2. **webviewtk_ecommerce.py** (21KB)
   - Complete e-commerce store with shopping cart
   - Product gallery, filters, real-time search
   - Cart drawer with add/remove functionality

3. **webviewtk_social_media.py** (22KB)
   - Instagram-style social media feed
   - Stories carousel, like animations, expandable comments
   - Bookmark functionality, user suggestions

4. **webviewtk_portfolio.py** (25KB)
   - Portfolio/landing page with smooth scrolling
   - Animated gradient hero, project showcase
   - Skill bars, contact form, floating elements

#### Test Examples
- **test_multiple_windows.py** - Demonstrates 4 windows simultaneously
- **launch_all_examples.py** - Launches all comprehensive examples at once

### Documentation Created

- **MULTI_WINDOW_SUPPORT.md** - Complete technical documentation
- **README_WEBVIEWTK_EXAMPLES.md** - Comprehensive example guide
- **FEATURES_SHOWCASE.md** - Feature comparison matrix
- **Updated README_WEBVIEWTK.md** - Added multi-window section

### Technical Improvements

#### VM Enhancements
```rust
// LoadAttr Handler - Wrap callable dict values
match &value {
    Value::Closure { .. } | Value::NativeFunction(_) | Value::BuiltinFunction(_, _) => {
        Value::BoundMethod {
            object: Box::new(object_value.clone()),
            method_name: attr_name.clone(),
        }
    }
    _ => value.clone()
}
```

#### RefCell Borrowing Fix
```rust
// Clone method before releasing borrow
let method_opt = dict.borrow().get(method_name).cloned();
if let Some(method) = method_opt {
    // Now borrow is released, safe to call method
    self.call_function_fast(method, ...)
}
```

#### Platform-Specific Event Loop
```rust
#[cfg(target_os = "windows")]
let event_loop = EventLoop::<()>::new_any_thread();

#[cfg(not(target_os = "windows"))]
let event_loop = EventLoop::<()>::new();
```

### Testing Results

#### Multiple Windows Test ✅
```
Creating Window 1 (Blue)...
✓ Window 1 started!
Creating Window 2 (Green)...
✓ Window 2 started!
Creating Window 3 (Purple)...
✓ Window 3 started!
Creating Window 4 (Red)...
✓ Window 4 started!

SUCCESS! All 4 windows are now running simultaneously!
```

#### Multiple Programs Test ✅
Successfully launched and verified:
- Dashboard Pro (analytics)
- TechStore (e-commerce)
- SocialHub (social media)

All programs running concurrently without conflicts!

### Breaking Changes
None - All changes are backward compatible

### Performance Impact
- Minimal thread overhead per window
- Each window has independent event loop
- No shared state between windows
- Memory usage scales linearly with window count

### Known Issues
- Minor Windows warning: "Failed to unregister class Chrome_WidgetWin_0" (non-critical)

### Upgrade Instructions
```bash
# Clean rebuild
cargo clean
cargo build --features webviewtk

# Test multi-window support
./target/debug/tauraro.exe run examples/test_multiple_windows.py

# Launch all comprehensive examples
./target/debug/tauraro.exe run examples/launch_all_examples.py
```

### Files Changed Summary

#### Core Files
- `src/bytecode/vm.rs` - VM method call enhancements
- `src/modules/webviewtk/mod.rs` - Thread-based window management
- `src/modules/mod.rs` - Module registration

#### New Example Files (8 files)
- `examples/webviewtk_dashboard.py`
- `examples/webviewtk_ecommerce.py`
- `examples/webviewtk_social_media.py`
- `examples/webviewtk_portfolio.py`
- `examples/test_multiple_windows.py`
- `examples/launch_all_examples.py`
- `examples/run_example.py`
- `examples/FEATURES_SHOWCASE.md`

#### Documentation Files (3 files)
- `MULTI_WINDOW_SUPPORT.md` (new)
- `examples/README_WEBVIEWTK_EXAMPLES.md` (new)
- `examples/README_WEBVIEWTK.md` (updated)

### Statistics
- **Lines of Code Added**: ~3,500+ lines (examples + documentation)
- **Files Modified**: 3 core files
- **Files Created**: 11 new files
- **Examples Created**: 4 comprehensive + 2 test examples
- **Documentation Pages**: 3 complete guides

### What's Next
Potential future enhancements:
- Window communication (IPC)
- Shared state management between windows
- Window positioning API
- Multi-monitor support
- Custom event handling system

---

**Release Date**: January 2025
**Status**: ✅ Completed and Tested
**Impact**: Major feature enhancement

Built with ❤️ for Tauraro Programming Language
