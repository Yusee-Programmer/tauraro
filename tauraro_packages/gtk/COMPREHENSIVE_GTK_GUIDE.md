# Tauraro GTK - Complete Cross-Platform GUI Framework

## 🎯 Framework Overview

Tauraro GTK is a complete, production-ready GUI framework built on GTK3, offering:
- **Cross-platform**: Windows, Linux, macOS with native rendering
- **Complete widget set**: 25+ widget types
- **Event handling**: Full callback/signal support  
- **Modern design**: Clean, Pythonic API
- **Production-ready**: Stable, well-tested, documented

## 📦 Complete Widget List

### Basic Widgets
- **Window**: Main application window
- **Button**: Click buttons
- **Label**: Text display
- **Entry**: Single-line text input
- **TextView**: Multi-line rich text (NEW!)
- **Image**: Display images (NEW!)

### Selection Widgets
- **CheckButton**: Toggle checkboxes
- **RadioButton**: Exclusive selection
- **ComboBox**: Dropdown selection
- **Switch**: Modern toggle switch (NEW!)
- **Scale**: Slider for numeric input (NEW!)

### Layout Containers
- **Box**: Linear layouts (H/V)
- **Grid**: Table/grid layouts
- **Frame**: Grouped containers with borders
- **ScrolledWindow**: Scrollable content (NEW!)
- **Notebook**: Tabbed interface (NEW!)

### Advanced Widgets
- **ProgressBar**: Progress indicators
- **Spinner**: Loading animations
- **Separator**: Visual dividers
- **Toolbar**: Application toolbars (NEW!)
- **StatusBar**: Status information (NEW!)
- **DrawingArea**: Custom drawing canvas (NEW!)

### Dialogs
- **MessageDialog**: Info/warning/error dialogs (NEW!)
- **FileChooserDialog**: File selection (NEW!)

## 🚀 Quick Start

```python
import gtk

# Initialize
gtk.init_gtk()

# Create window
window = gtk.Window("My App", 800, 600)

# Add widgets
label = gtk.Label("Hello, World!")
button = gtk.Button("Click Me!")

# Layout
vbox = gtk.Box(gtk.GTK_ORIENTATION_VERTICAL, 10)
vbox.pack_start(label, False, False, 10)
vbox.pack_start(button, False, False, 10)

window.add(vbox)
window.show_all()

# Run
app = gtk.Application("MyApp")
app.run()
```

## 📚 Feature Examples

### Dialogs
```python
# Message Dialog
def show_message():
    dialog = gtk.MessageDialog(
        window,
        gtk.GTK_MESSAGE_INFO,
        gtk.GTK_BUTTONS_OK,
        "Operation completed successfully!"
    )
    response = dialog.run()
    dialog.destroy()

# File Chooser
def choose_file():
    dialog = gtk.FileChooserDialog(
        "Open File",
        window,
        gtk.GTK_FILE_CHOOSER_ACTION_OPEN
    )
    response = dialog.run()
    if response == gtk.GTK_RESPONSE_OK:
        filename = dialog.get_filename()
        print("Selected:", filename)
    dialog.destroy()
```

### Tabs (Notebook)
```python
notebook = gtk.Notebook()

# Add tabs
page1 = gtk.Label("Content for tab 1")
page2 = gtk.Label("Content for tab 2")

notebook.append_page(page1, gtk.Label("Tab 1"))
notebook.append_page(page2, gtk.Label("Tab 2"))

window.add(notebook)
```

### Scrollable Content
```python
scrolled = gtk.ScrolledWindow()
scrolled.set_policy(gtk.GTK_POLICY_AUTOMATIC, gtk.GTK_POLICY_AUTOMATIC)

# Add large content
large_text = gtk.TextView()
scrolled.add(large_text)

window.add(scrolled)
```

### Image Display
```python
image = gtk.Image()
image.set_from_file("logo.png")

window.add(image)
```

### Toolbar
```python
toolbar = gtk.Toolbar()

# Add buttons
btn_new = gtk.ToolButton(None, "New")
btn_open = gtk.ToolButton(None, "Open")
btn_save = gtk.ToolButton(None, "Save")

toolbar.insert(btn_new, 0)
toolbar.insert(btn_open, 1)
toolbar.insert(gtk.SeparatorToolItem(), 2)
toolbar.insert(btn_save, 3)

vbox.pack_start(toolbar, False, False, 0)
```

### StatusBar
```python
statusbar = gtk.StatusBar()
context_id = statusbar.get_context_id("main")
statusbar.push(context_id, "Ready")

vbox.pack_end(statusbar, False, False, 0)
```

## 🎨 Complete Application Template

```python
import gtk

class MyApplication:
    def __init__(self):
        # Initialize GTK
        gtk.init_gtk()
        
        # Create window
        self.window = gtk.Window("Complete App", 900, 700)
        
        # Main layout
        main_vbox = gtk.Box(gtk.GTK_ORIENTATION_VERTICAL, 0)
        
        # Toolbar
        self.create_toolbar(main_vbox)
        
        # Content area with tabs
        self.create_content(main_vbox)
        
        # StatusBar
        self.create_statusbar(main_vbox)
        
        self.window.add(main_vbox)
        self.window.show_all()
    
    def create_toolbar(self, vbox):
        toolbar = gtk.Toolbar()
        # Add toolbar items...
        vbox.pack_start(toolbar, False, False, 0)
    
    def create_content(self, vbox):
        notebook = gtk.Notebook()
        # Add tabs...
        vbox.pack_start(notebook, True, True, 0)
    
    def create_statusbar(self, vbox):
        statusbar = gtk.StatusBar()
        vbox.pack_end(statusbar, False, False, 0)
    
    def run(self):
        app = gtk.Application("MyApp")
        app.run()

# Run application
if __name__ == "__main__":
    app = MyApplication()
    app.run()
```

## 🔧 Constants Reference

### Window Types
- `GTK_WINDOW_TOPLEVEL`: Regular window
- `GTK_WINDOW_POPUP`: Popup window

### Orientations
- `GTK_ORIENTATION_HORIZONTAL`: Horizontal layout
- `GTK_ORIENTATION_VERTICAL`: Vertical layout

### Message Types
- `GTK_MESSAGE_INFO`: Information
- `GTK_MESSAGE_WARNING`: Warning
- `GTK_MESSAGE_QUESTION`: Question
- `GTK_MESSAGE_ERROR`: Error

### Button Types
- `GTK_BUTTONS_NONE`: No buttons
- `GTK_BUTTONS_OK`: OK button
- `GTK_BUTTONS_YES_NO`: Yes/No buttons
- `GTK_BUTTONS_OK_CANCEL`: OK/Cancel buttons

### Dialog Responses
- `GTK_RESPONSE_OK`: -5
- `GTK_RESPONSE_CANCEL`: -6
- `GTK_RESPONSE_YES`: -8
- `GTK_RESPONSE_NO`: -9

### Scroll Policies
- `GTK_POLICY_AUTOMATIC`: Show scrollbar when needed
- `GTK_POLICY_ALWAYS`: Always show scrollbar
- `GTK_POLICY_NEVER`: Never show scrollbar

## 🌟 Best Practices

1. **Always initialize GTK**: Call `gtk.init_gtk()` first
2. **Use layouts**: Never set absolute positions
3. **Show all widgets**: Call `window.show_all()` before running
4. **Run main loop**: Use `Application().run()` to keep window open
5. **Clean up dialogs**: Always `destroy()` dialogs after use

## 📖 Additional Resources

- See `examples/` directory for complete working examples
- Check `widgets.tr` for all widget class implementations  
- Visit GTK3 documentation for detailed API reference

## ✅ Platform Support

- ✅ Windows (GTK3 runtime required)
- ✅ Linux (GTK3 via package manager)
- ✅ macOS (GTK3 via Homebrew)

Install GTK3:
- **Windows**: Download from gtk.org
- **Linux**: `sudo apt install libgtk-3-0`
- **macOS**: `brew install gtk+3`

---

**Version**: 1.0.0 | **License**: MIT | **Language**: Tauraro
