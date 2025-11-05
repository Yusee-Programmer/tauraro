# Installation Guide

This guide will help you install Tauraro on your system.

## Prerequisites

Tauraro requires:
- Rust 1.70 or later
- Cargo (comes with Rust)
- A C compiler (for native compilation)
  - GCC on Linux
  - Clang on macOS
  - MSVC or MinGW on Windows

## Installing Rust

If you don't have Rust installed:

```bash
# Linux/macOS
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

# Windows
# Download and run rustup-init.exe from https://rustup.rs/
```

## Building Tauraro

### From Source

```bash
# Clone the repository
git clone https://github.com/Yusee-Programmer/tauraro.git
cd tauraro

# Build in release mode (optimized)
cargo build --release

# The binary will be at: target/release/tauraro
```

### Building with All Features

```bash
# Build with all features enabled
cargo build --release --all-features

# Build with specific features
cargo build --release --features "jit,ffi"
```

## Installation

### System-wide Installation

```bash
# After building, install system-wide
cargo install --path .

# Or copy the binary to your PATH
sudo cp target/release/tauraro /usr/local/bin/
```

### Verify Installation

```bash
# Check version
tauraro --version

# Should output: Tauraro 1.0.0 (or current version)
```

## Platform-Specific Notes

### Linux

Tauraro works out of the box on most Linux distributions. Ensure you have build-essential:

```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# Fedora
sudo dnf install gcc make

# Arch
sudo pacman -S base-devel
```

### macOS

Install Xcode Command Line Tools:

```bash
xcode-select --install
```

### Windows

Option 1: Use MSVC (recommended)
```bash
# Install Visual Studio Build Tools
# Download from: https://visualstudio.microsoft.com/downloads/
```

Option 2: Use MinGW
```bash
# Install MinGW-w64
# Download from: https://www.mingw-w64.org/
```

## Running Tauraro

After installation, you can run Tauraro in three modes:

### 1. Script Execution (VM)
```bash
tauraro run script.py
```

### 2. Interactive REPL
```bash
tauraro repl
```

### 3. Compile to Native Binary
```bash
tauraro compile script.py -o output
./output  # Run the compiled binary
```

## Troubleshooting

### "Command not found"

If you get "command not found", add Cargo's bin directory to your PATH:

```bash
# Linux/macOS - Add to ~/.bashrc or ~/.zshrc
export PATH="$HOME/.cargo/bin:$PATH"

# Windows - Add to system PATH
%USERPROFILE%\.cargo\bin
```

### Build Errors

If you encounter build errors:

```bash
# Update Rust
rustup update

# Clean and rebuild
cargo clean
cargo build --release
```

### Permission Denied

If you get permission errors:

```bash
# Make binary executable (Linux/macOS)
chmod +x target/release/tauraro
```

## Next Steps

- [Quick Start Tutorial](quick-start.md)
- [Write Your First Program](first-program.md)
- [Explore Language Features](../language/syntax.md)
