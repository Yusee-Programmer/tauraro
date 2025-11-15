# WebViewTK Menu & Title Bar Implementation - Current Limitations

## Overview
The `menu()`, `menu_item()`, and `titlebar()` functions have been added to the WebViewTK module, but they currently create data structures only. Native OS-level menu bar integration requires additional work.

## Current State

### What Works ✅
- `titlebar()` function creates titlebar configuration objects
- `menu()` function creates menu container objects  
- `menu_item()` function creates menu item objects
- `menu_separator()` function creates separator objects
- `Window.set_menu()`, `Window.set_titlebar()` methods store these configurations in the window object
- All functions compile and execute without errors
- Example file (`titlebar_menu_demo.py`) runs successfully

### What Doesn't Work Yet ❌
- Native OS menu bars are NOT created from the menu objects
- Window decorations/frameless mode settings are NOT applied
- Title bar customizations are NOT rendered as native OS controls
- Icon settings are NOT applied to the window
- Menus appear nowhere in the GUI

## Why?

The current Wry-based implementation in `window_run()` function:
1. Creates a `WindowBuilder` with basic title
2. Sets HTML content 
3. Runs the event loop

It does NOT:
- Extract and apply menu configurations
- Create native OS menus from `menu()` objects
- Apply titlebar customizations
- Handle custom window decorations

## Recommended Solution: HTML-Based Menus

Since this is a WebViewTK (Web-based GUI), the best approach is to create menus using HTML/CSS/JavaScript instead of native OS menus:

### Advantages:
- ✅ Works across all platforms identically
- ✅ Full control over appearance and behavior
- ✅ Can be customized with Tailwind CSS
- ✅ Integrates seamlessly with the web-based UI
- ✅ No platform-specific code needed

### Example Pattern:

```python
import webviewtk as wv

html = wv.render(
    "<html><head>",
    wv.cdn_tailwind(),
    """
    <style>
        .menubar { 
            background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
            padding: 0.75rem 1.25rem;
            display: flex;
            gap: 1rem;
        }
        .menu-btn {
            color: white;
            cursor: pointer;
            padding: 0.5rem 1rem;
            border-radius: 0.375rem;
        }
        .menu-btn:hover {
            background: rgba(255, 255, 255, 0.1);
        }
    </style>
    """,
    "</head><body>",
    "<div class='menubar'>",
    "<button class='menu-btn'>📁 File</button>",
    "<button class='menu-btn'>✏️ Edit</button>",
    "<button class='menu-btn'>👁️ View</button>",
    "</div>",
    "<div class='content p-8'>",
    "<h1>Your Application</h1>",
    "</div>",
    "</body></html>"
)

window = wv.Window("My App", 1000, 600)
window.set_html(html)
window.run()
```

## Future Enhancements

To implement native OS menu support:

1. **Extract menu objects** in `window_run()` function
2. **Use Wry's menu API** (when available) to create native menus
3. **Apply titlebar customizations** using platform-specific window builders
4. **Handle window decorations** via `WindowBuilder::with_decorations()`
5. **Support custom icons** via `WindowBuilder::with_window_icon()`

## Updated Example

See `examples/TITLEBAR_MENU_GUIDE.md` for HTML-based menu implementation patterns that work today.

## Status
- Module implementation: ✅ Complete
- API documentation: ✅ Complete  
- HTML-based menus: ✅ Working pattern available
- Native OS menus: ⏳ Pending (requires Wry API updates)
