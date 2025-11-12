# WebViewTK Automatic Dependency Installation

## Overview

When you build Tauraro with the `webviewtk` feature, the build process automatically detects and manages platform-specific dependencies. This means you can compile and run WebViewTK applications immediately after building, without any manual dependency installation!

## How It Works

The `build.rs` script runs during compilation and:

1. **Detects if the webviewtk feature is enabled**
2. **Identifies your operating system** (Windows, Linux, or macOS)
3. **Checks for required dependencies**
4. **Automatically installs missing dependencies** (if needed)
5. **Provides clear feedback** via build warnings

## Platform-Specific Behavior

### Windows

**What it checks:**
- Installation directory: `C:\Program Files (x86)\Microsoft\EdgeWebView\Application`
- Alternative directory: `C:\Program Files\Microsoft\EdgeWebView\Application`
- Windows Registry key (fallback)

**What you'll see:**
```
warning: WebViewTK feature enabled - checking platform dependencies...
warning: Windows detected - checking for WebView2 Runtime...
warning: WebView2 Runtime is already installed at C:\Program Files (x86)\Microsoft\EdgeWebView\Application!
```

**Automatic installation:**
- If WebView2 is not found, the script:
  - Downloads the official Microsoft installer
  - Runs silent installation
  - Cleans up the installer file
  - Falls back to manual instructions if installation fails

**Note for Windows 11:**
- WebView2 Runtime comes pre-installed
- The build script will detect it automatically
- No installation needed!

### Linux

**Distribution detection:**
- Automatically identifies Debian/Ubuntu, Fedora, or Arch Linux
- Uses `pkg-config` to check for WebKitGTK

**Automatic installation commands:**
- **Debian/Ubuntu**: `sudo apt-get install -y libwebkit2gtk-4.0-dev`
- **Fedora**: `sudo dnf install -y webkit2gtk3-devel`
- **Arch Linux**: `sudo pacman -S --noconfirm webkit2gtk`

**What you'll see:**
```
warning: WebViewTK feature enabled - checking platform dependencies...
warning: Linux detected - checking for WebKitGTK...
warning: Detected Debian/Ubuntu-based system
warning: Installing libwebkit2gtk-4.0-dev...
warning: WebKitGTK installed successfully!
```

**Note**: You may be prompted for your sudo password during the build process.

### macOS

**No action needed:**
- WebKit is built into macOS
- The build script simply confirms the platform

**What you'll see:**
```
warning: WebViewTK feature enabled - checking platform dependencies...
warning: macOS detected - WebKit is built-in, no installation needed
```

## Usage

Simply build with the webviewtk feature:

```bash
# Release build (recommended)
cargo build --release --features webviewtk

# Debug build
cargo build --features webviewtk
```

The first time you build, you'll see the dependency check warnings. Subsequent builds will be faster as dependencies are already installed.

## Build Output Examples

### Successful Detection (Windows 11)
```
   Compiling tauraro v0.0.1
warning: tauraro@0.0.1: WebViewTK feature enabled - checking platform dependencies...
warning: tauraro@0.0.1: Windows detected - checking for WebView2 Runtime...
warning: tauraro@0.0.1: WebView2 Runtime is already installed at C:\Program Files (x86)\Microsoft\EdgeWebView\Application!
   Compiling...
```

### Installing Missing Dependency (Windows 10)
```
   Compiling tauraro v0.0.1
warning: tauraro@0.0.1: WebViewTK feature enabled - checking platform dependencies...
warning: tauraro@0.0.1: Windows detected - checking for WebView2 Runtime...
warning: tauraro@0.0.1: WebView2 Runtime not found. Attempting to download and install...
warning: tauraro@0.0.1: Downloading WebView2 Runtime from Microsoft...
warning: tauraro@0.0.1: Download complete. Installing WebView2 Runtime...
warning: tauraro@0.0.1: WebView2 Runtime installed successfully!
   Compiling...
```

### Linux Installation
```
   Compiling tauraro v0.0.1
warning: tauraro@0.0.1: WebViewTK feature enabled - checking platform dependencies...
warning: tauraro@0.0.1: Linux detected - checking for WebKitGTK...
warning: tauraro@0.0.1: Detected Debian/Ubuntu-based system
warning: tauraro@0.0.1: Installing libwebkit2gtk-4.0-dev...
[sudo] password for user:
warning: tauraro@0.0.1: WebKitGTK installed successfully!
   Compiling...
```

## Troubleshooting

### If Automatic Installation Fails

**Windows:**
```
warning: Automatic installation failed. Please install manually from:
warning: https://developer.microsoft.com/microsoft-edge/webview2/
```

**Linux:**
```
warning: Automatic installation failed. Please run manually:
warning: sudo apt-get install libwebkit2gtk-4.0-dev  # Ubuntu/Debian
warning: sudo dnf install webkit2gtk3-devel          # Fedora
warning: sudo pacman -S webkit2gtk                    # Arch
```

### Permission Issues (Linux)

If you get permission errors:
1. Run the suggested command manually with sudo
2. Rebuild: `cargo build --release --features webviewtk`

### Antivirus Blocking (Windows)

If your antivirus blocks the installation:
1. Temporarily disable antivirus
2. Rebuild to trigger installation
3. Re-enable antivirus
4. Or install WebView2 manually from Microsoft's official site

## Technical Details

### Build Script Location
- **File**: `build.rs` (in project root)
- **Language**: Rust
- **Runs**: During `cargo build` when `webviewtk` feature is enabled

### Feature Detection
```toml
# Cargo.toml
[features]
webviewtk = ["dep:wry", "dep:tao"]  # Cross-platform GUI framework
```

### Environment Variables
- `CARGO_FEATURE_WEBVIEWTK`: Set when feature is enabled
- `CARGO_CFG_TARGET_OS`: Contains target platform (windows/linux/macos)

## Benefits

1. **Zero Manual Setup**: Just build and run
2. **Cross-Platform**: Works consistently on Windows, Linux, and macOS
3. **Automatic Detection**: Skips installation if dependencies exist
4. **Clear Feedback**: Build warnings show exactly what's happening
5. **Fallback Options**: Manual installation instructions if automatic fails

## Verifying Installation

After building, test with a simple example:

```bash
# Run the simple window example
./target/release/tauraro.exe run examples/test_window_simple.py
```

If you see a window open, the automatic installation worked perfectly!

## See Also

- `docs/WEBVIEWTK_GUIDE.md` - Complete WebViewTK documentation
- `WEBVIEWTK_TROUBLESHOOTING.md` - Detailed troubleshooting guide
- `examples/README_WEBVIEWTK.md` - Example usage guide

## Credits

The automatic dependency installation uses:
- **WebView2** (Windows) - Microsoft Edge WebView2 Runtime
- **WebKitGTK** (Linux) - GTK port of WebKit
- **WebKit** (macOS) - Built into macOS

Powered by the **wry** crate (same as Tauri) for cross-platform webview support.
