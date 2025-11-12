# WebViewTK Window Display Troubleshooting

## Issue: Window Not Displaying

If the window isn't appearing when you run your WebViewTK application, follow these steps:

### 1. Verify Build with webviewtk Feature

Make sure you built Tauraro with the `webviewtk` feature:

```bash
cargo build --release --features webviewtk
```

**Check if it's compiled:**
```bash
# On Windows
ls target/release/tauraro.exe

# The file should exist and be recent
```

### 2. Windows: WebView2 Runtime

**Automatic Installation**: When you build with `--features webviewtk`, the build script automatically checks for and installs WebView2 Runtime if needed. You'll see build warnings like:

```
warning: WebViewTK feature enabled - checking platform dependencies...
warning: Windows detected - checking for WebView2 Runtime...
warning: WebView2 Runtime is already installed at C:\Program Files (x86)\Microsoft\EdgeWebView\Application!
```

**Note**: Windows 11 comes with WebView2 Runtime pre-installed!

If automatic installation fails, you can install manually:
- Direct download: https://go.microsoft.com/fwlink/p/?LinkId=2124703
- Or visit: https://developer.microsoft.com/microsoft-edge/webview2/

After manual installation, restart your terminal and rebuild.

### 3. Check Your Code

Make sure you're calling the methods in the correct order:

```python
import webviewtk as wv

# 1. Build HTML
html = "<!DOCTYPE html>" + wv.html(...)

# 2. Create window
window = wv.Window("Title", 800, 600)

# 3. Set HTML (IMPORTANT!)
window.set_html(html)

# 4. Run window
window.run()  # This MUST be the last line
```

**Common mistakes:**
- Forgetting to call `window.set_html()`
- HTML string is empty
- Not adding `<!DOCTYPE html>` prefix
- Calling `window.run()` before `window.set_html()`

### 4. Test with Minimal Example

Save this as `test_minimal.py`:

```python
import webviewtk as wv

html = """<!DOCTYPE html>
<html>
<head><title>Test</title></head>
<body style="background: lightblue; padding: 50px;">
    <h1 style="color: darkblue;">Hello from WebViewTK!</h1>
    <p>If you see this, it works!</p>
</body>
</html>"""

window = wv.Window("Minimal Test", 600, 400)
window.set_html(html)
print("Opening window...")
window.run()
```

Run it:
```bash
./target/release/tauraro.exe run test_minimal.py
```

**Expected behavior:**
- You should see "Opening window..." printed
- A window should appear with blue background
- The program will block (not return to prompt) until you close the window

### 5. Check for Errors

Run with verbose output to see any errors:

```bash
# Enable Rust backtrace
set RUST_BACKTRACE=1
./target/release/tauraro.exe run test_minimal.py
```

Look for error messages like:
- "WebViewTK feature is not enabled" - You didn't build with `--features webviewtk`
- "Window HTML content is empty" - You forgot to call `set_html()`
- "Failed to create window" - System/permission issue
- "Failed to create webview" - WebView2 not installed (Windows)

### 6. Platform-Specific Issues

#### Windows
- **Must have WebView2 Runtime installed** (see step 2)
- Antivirus might block - temporarily disable and try
- Check Windows Defender hasn't quarantined the executable

#### Linux
- Install WebKitGTK:
  ```bash
  # Ubuntu/Debian
  sudo apt install libwebkit2gtk-4.0-dev

  # Fedora
  sudo dnf install webkit2gtk3-devel

  # Arch
  sudo pacman -S webkit2gtk
  ```

#### macOS
- WebKit is built-in, should work out of the box
- Check Security & Privacy settings if blocked

### 7. Debug Mode

Try running in debug mode to get more information:

```bash
cargo build --features webviewtk  # Note: without --release
./target/debug/tauraro.exe run test_minimal.py
```

Debug builds provide more detailed error messages.

### 8. Verify Imports

Check that webviewtk module is available:

```python
# test_import.py
try:
    import webviewtk as wv
    print("✓ webviewtk imported successfully")
    print(f"✓ Window class available: {hasattr(wv, 'Window')}")
    print(f"✓ div function available: {hasattr(wv, 'div')}")
except Exception as e:
    print(f"✗ Error importing webviewtk: {e}")
```

Run:
```bash
./target/release/tauraro.exe run test_import.py
```

### 9. Known Limitations

- `window.run()` is a **blocking call** - code after it won't execute until window closes
- Only one window at a time is currently supported
- Must be run from main thread
- HTML/CSS/JS errors won't be visible (add JavaScript console logging if needed)

### 10. Alternative: Test HTML Generation

If windows still won't show, test HTML generation without GUI:

```python
import webviewtk as wv

# Generate HTML
html = wv.html(
    wv.render(
        wv.head(wv.title("Test")),
        wv.body(wv.h1("Test"))
    )
)

# Save to file
with open("test_output.html", "w") as f:
    f.write("<!DOCTYPE html>" + html)

print("HTML saved to test_output.html")
print("Open it in your browser to verify HTML generation works")
```

This confirms the widget functions work even if windowing doesn't.

## Still Not Working?

### Collect Debug Information

Run these commands and share the output:

```bash
# 1. Check build succeeded
ls -lh target/release/tauraro.exe

# 2. Check webviewtk feature was compiled
strings target/release/tauraro.exe | grep -i webview | head -10

# 3. Run with backtrace
set RUST_BACKTRACE=full
./target/release/tauraro.exe run test_minimal.py 2>&1 | tee error_log.txt
```

### Common Solutions

| Problem | Solution |
|---------|----------|
| "WebViewTK feature not enabled" | Rebuild with `--features webviewtk` |
| "Window HTML content is empty" | Call `window.set_html(html)` before `window.run()` |
| "Failed to create webview" (Windows) | Install WebView2 Runtime |
| Window flashes and closes | You have code after `window.run()` that causes exit |
| Import error | Module not registered - check Cargo.toml has feature |
| Permission denied | Run with elevated privileges or check antivirus |

## Contact/Report Issues

If you've tried all steps above and it still doesn't work, please provide:
1. Operating system and version
2. Output of `cargo --version` and `rustc --version`
3. Complete error messages
4. The minimal test code you're running
5. Output from debug information commands above
