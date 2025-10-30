# DUITK - Desktop UI Toolkit

A modern Windows GUI framework for Tauraro, built on top of the win32 package.

## Overview

DUITK (Desktop UI Toolkit) provides a high-level interface for creating native Windows applications using the win32 API through Tauraro. It simplifies Windows GUI development while maintaining access to the full power of the Windows API.

## Installation

DUITK is included as part of the Tauraro standard packages. No additional installation is required.

## Quick Start

``tauraro
# Import the DUITK package
import duitk

# Import individual modules as needed
import duitk.window

# Use DUITK functions
result = duitk.window.test_function()
print(result)  # Output: Hello, World!
```

## Available Modules

- `duitk.window` - Window management functions
- `duitk.controls` - Control creation functions (in development)
- `duitk.dialogs` - Dialog functions (in development)
- `duitk.menu` - Menu functions (in development)
- `duitk.system` - System functions (in development)
- `duitk.events` - Event handling functions (in development)

## Features

- Native Windows look and feel
- Access to full Windows API functionality
- Modular design for easy maintenance
- No circular dependencies with win32 package
- Deferred imports to avoid loading issues

## Usage Examples

### Simple Window Application

``tauraro
import duitk
import duitk.window

# Create a basic window application
result = duitk.window.test_function()
print("Window module working:", result)
```

## Development Status

DUITK is currently in development. The core framework is functional, with additional modules being refined for full compatibility.

## Contributing

Contributions to DUITK are welcome! Please submit issues and pull requests to the main Tauraro repository.

## License

DUITK is distributed as part of Tauraro under the same license terms.
