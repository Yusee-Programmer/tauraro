# WebViewTK Flutter-Style Widget System

Complete widget-based UI framework for Tauraro - inspired by Flutter's architecture. Build UIs with pure widget classes, **NO HTML strings ever!**

## Architecture Overview

### Core Concept
```python
# Traditional approach (OLD):
window.set_html("<div>Hello</div>")  # Mixing HTML with code ❌

# Flutter-Style approach (NEW):
window.build(
    webviewtk.Text({"content": "Hello"})  # Pure widgets ✅
)
```

### Widget Registry Pattern
- All widgets stored globally in `WIDGET_REGISTRY`
- Widgets return dictionaries with `_widget_id` and `_widget_type`
- `window.build()` extracts widget from registry and generates HTML/CSS/JS automatically

## Complete Widget Reference

### Layout Widgets

#### Column
Arranges children vertically with spacing and alignment.

```python
webviewtk.Column({
    "children": [widget1, widget2, widget3],
    "spacing": 16,           # Vertical spacing between children
    "padding": 24,           # Uniform padding
    "alignment": "center",   # Main axis: "start", "center", "end", "space_between"
    "class_name": "my-column"
})
```

#### Row
Arranges children horizontally.

```python
webviewtk.Row({
    "children": [button1, button2],
    "spacing": 12,
    "padding": 16,
    "alignment": "space_between",
    "class_name": "my-row"
})
```

#### Container
Versatile box for styling and positioning.

```python
webviewtk.Container({
    "child": widget,
    "padding": 16,
    "margin": 8,
    "width": 300,
    "height": 200,
    "background_color": "#f0f0f0",
    "border_radius": 8,
    "border": "1px solid #ddd",
    "class_name": "my-container"
})
```

#### Center
Centers its child horizontally and vertically.

```python
webviewtk.Center({
    "child": widget
})
```

#### Padding
Adds padding around its child.

```python
webviewtk.Padding({
    "padding": 24,  # Uniform padding
    "child": widget
})
```

#### Expanded
Makes child fill available space in parent Column/Row.

```python
webviewtk.Row({
    "children": [
        webviewtk.Expanded({
            "flex": 2,
            "child": widget1
        }),
        webviewtk.Expanded({
            "flex": 1,
            "child": widget2
        })
    ]
})
```

#### Spacer
Creates flexible empty space.

```python
webviewtk.Spacer({
    "flex": 1
})
```

#### Stack
Positions children on top of each other (z-axis layering).

```python
webviewtk.Stack({
    "children": [background, overlay],
    "alignment": "center"
})
```

#### Positioned
Absolutely positions child within Stack.

```python
webviewtk.Positioned({
    "top": 10,
    "right": 10,
    "child": widget
})
```

#### SizedBox
Fixed size container.

```python
webviewtk.SizedBox({
    "width": 100,
    "height": 100,
    "child": widget  # Optional
})
```

#### Flexible
Gives child flexibility to expand/shrink.

```python
webviewtk.Flexible({
    "flex": 1,
    "fit": "tight",  # "tight" or "loose"
    "child": widget
})
```

#### Align
Aligns child within parent bounds.

```python
webviewtk.Align({
    "alignment": "top_left",  # or "center", "bottom_right", etc.
    "child": widget
})
```

#### Wrap
Wraps children to next line when space runs out.

```python
webviewtk.Wrap({
    "children": [chip1, chip2, chip3, chip4],
    "spacing": 8,        # Horizontal spacing
    "run_spacing": 4,    # Vertical spacing between lines
    "alignment": "start"
})
```

### Material Design Widgets

#### Scaffold
Main Material Design structure with AppBar, body, FAB.

```python
webviewtk.Scaffold({
    "app_bar": webviewtk.AppBar({...}),
    "body": webviewtk.Column({...}),
    "floating_action_button": webviewtk.FloatingActionButton({...}),
    "drawer": webviewtk.Drawer({...}),          # Optional
    "bottom_nav": webviewtk.BottomNav({...}),   # Optional
    "background_color": "white"
})
```

#### AppBar
Material Design app bar with title and actions.

```python
webviewtk.AppBar({
    "title": "My App",
    "leading": back_button,           # Optional
    "actions": [search_icon, menu],   # Optional
    "background_color": "#2196f3",
    "elevation": 4.0
})
```

#### FloatingActionButton
Circular floating action button.

```python
webviewtk.FloatingActionButton({
    "child": plus_icon,
    "on_pressed": "add_handler",
    "background_color": "#ff5722"
})
```

#### Drawer
Side navigation drawer.

```python
webviewtk.Drawer({
    "header": header_widget,  # Optional
    "children": [item1, item2, item3]
})
```

#### ListTile
Standard list item with title, subtitle, icons.

```python
webviewtk.ListTile({
    "title": "Settings",
    "subtitle": "App preferences",   # Optional
    "leading": settings_icon,        # Optional
    "trailing": arrow_icon,          # Optional
    "on_tap": "settings_handler"
})
```

### Basic Widgets

#### Text
Displays styled text.

```python
webviewtk.Text({
    "content": "Hello World",
    "font_size": 24,
    "font_weight": "bold",  # "normal", "bold", "lighter", "100"-"900"
    "color": "#333333",
    "text_align": "center", # "left", "center", "right", "justify"
    "class_name": "my-text"
})
```

#### Icon
Material Icons (requires CDN or icon font).

```python
webviewtk.Icon({
    "icon_name": "home",    # Material icon name
    "size": 24,
    "color": "#666"
})
```

#### Image
Displays images from URLs or paths.

```python
webviewtk.Image({
    "src": "https://example.com/image.jpg",
    "width": 300,
    "height": 200,
    "fit": "cover"  # "fill", "contain", "cover", "scale_down", etc.
})
```

#### Divider
Horizontal line separator.

```python
webviewtk.Divider({
    "height": 1,
    "thickness": 1,
    "color": "#e0e0e0"
})
```

### Interactive Widgets

#### Button
Styled button with variants.

```python
webviewtk.Button({
    "text": "Click Me",
    "on_click": "handler_id",
    "variant": "primary",    # "primary", "secondary", "danger", "success", 
                            # "warning", "info", "text", "outlined", "elevated"
    "padding": 16,
    "class_name": "my-button"
})
```

**Button Variants:**
- `primary` - Blue, main action
- `secondary` - Gray, secondary action
- `danger` - Red, destructive action
- `success` - Green, positive action
- `warning` - Orange, warning action
- `info` - Light blue, informational
- `text` - No background, text only
- `outlined` - Border only
- `elevated` - Raised with shadow

#### IconButton
Button with just an icon.

```python
webviewtk.IconButton({
    "icon": webviewtk.Icon({"icon_name": "delete"}),
    "on_pressed": "delete_handler",
    "tooltip": "Delete item"
})
```

#### TextField
Text input field.

```python
webviewtk.TextField({
    "value": "",
    "placeholder": "Enter your name",
    "type": "text",            # "text", "password", "email", "number", etc.
    "on_change": "input_handler",
    "padding": 12,
    "decoration": None,        # Future: InputDecoration support
    "class_name": "my-input"
})
```

#### Checkbox
Checkbox with optional label.

```python
webviewtk.Checkbox({
    "value": True,
    "on_changed": "checkbox_handler",
    "label": "I agree to terms"
})
```

#### Radio
Radio button in a group.

```python
webviewtk.Radio({
    "value": "option1",
    "group_value": "option1",  # Currently selected value
    "on_changed": "radio_handler",
    "label": "Option 1"
})
```

#### Switch
Toggle switch.

```python
webviewtk.Switch({
    "value": False,
    "on_changed": "switch_handler",
    "label": "Enable notifications"
})
```

#### Slider
Range slider.

```python
webviewtk.Slider({
    "value": 50,
    "min": 0,
    "max": 100,
    "on_changed": "slider_handler"
})
```

#### InkWell
Adds ink ripple effect to child on tap.

```python
webviewtk.InkWell({
    "child": card,
    "on_tap": "tap_handler"
})
```

#### GestureDetector
Detects various gestures.

```python
webviewtk.GestureDetector({
    "child": widget,
    "on_tap": "tap_handler",
    "on_double_tap": "double_tap_handler",
    "on_long_press": "long_press_handler"
})
```

### Scrollable Widgets

#### ListView
Vertical scrollable list.

```python
webviewtk.ListView({
    "children": [item1, item2, item3, item4, item5],
    "scrollable": True,
    "padding": 16
})
```

#### GridView
Grid layout with scrolling.

```python
webviewtk.GridView({
    "children": [card1, card2, card3, card4, card5, card6],
    "cross_axis_count": 3,  # Number of columns
    "spacing": 12,
    "padding": 16
})
```

#### SingleChildScrollView
Makes single child scrollable.

```python
webviewtk.SingleChildScrollView({
    "child": long_content,
    "direction": "vertical"  # "vertical" or "horizontal"
})
```

### Container Widgets

#### Card
Material Design card with elevation.

```python
webviewtk.Card({
    "child": content,
    "elevation": 2,
    "border_radius": 8
})
```

#### Dialog
Modal dialog box.

```python
webviewtk.Dialog({
    "title": "Confirm Action",
    "content": webviewtk.Text({"content": "Are you sure?"}),
    "actions": [cancel_button, ok_button]
})
```

#### BottomSheet
Sheet that slides up from bottom.

```python
webviewtk.BottomSheet({
    "child": content,
    "background_color": "white"
})
```

## Helper Classes

### EdgeInsets
Define padding/margin for widgets.

```python
# Uniform padding on all sides
webviewtk.EdgeInsets.all(16)

# Different horizontal and vertical
webviewtk.EdgeInsets.symmetric({
    "vertical": 12,
    "horizontal": 24
})

# Individual sides
webviewtk.EdgeInsets.only({
    "top": 8,
    "right": 16,
    "bottom": 8,
    "left": 16
})

# No padding/margin
webviewtk.EdgeInsets.zero()
```

## Complete Example

```python
import webviewtk

def main():
    window = webviewtk.Window({
        "title": "Flutter-Style App",
        "width": 800,
        "height": 600
    })
    
    # Material Design structure
    app = webviewtk.Scaffold({
        "app_bar": webviewtk.AppBar({
            "title": "My App",
            "background_color": "#2196f3"
        }),
        
        "body": webviewtk.SingleChildScrollView({
            "child": webviewtk.Column({
                "padding": 24,
                "spacing": 20,
                "children": [
                    # Header
                    webviewtk.Text({
                        "content": "Welcome",
                        "font_size": 32,
                        "font_weight": "bold"
                    }),
                    
                    # Interactive section
                    webviewtk.Card({
                        "child": webviewtk.Container({
                            "padding": 16,
                            "child": webviewtk.Column({
                                "spacing": 12,
                                "children": [
                                    webviewtk.TextField({
                                        "placeholder": "Enter text",
                                        "value": ""
                                    }),
                                    
                                    webviewtk.Row({
                                        "spacing": 8,
                                        "children": [
                                            webviewtk.Button({
                                                "text": "Submit",
                                                "variant": "primary"
                                            }),
                                            webviewtk.Button({
                                                "text": "Cancel",
                                                "variant": "outlined"
                                            })
                                        ]
                                    })
                                ]
                            })
                        })
                    }),
                    
                    # Grid of items
                    webviewtk.GridView({
                        "cross_axis_count": 3,
                        "spacing": 12,
                        "children": [
                            webviewtk.Card({"child": item1}),
                            webviewtk.Card({"child": item2}),
                            webviewtk.Card({"child": item3}),
                            # ... more items
                        ]
                    })
                ]
            })
        }),
        
        "floating_action_button": webviewtk.FloatingActionButton({
            "child": webviewtk.Text({
                "content": "+",
                "font_size": 24,
                "color": "white"
            }),
            "background_color": "#ff5722"
        })
    })
    
    window.build(app)
    window.run()

if __name__ == "__main__":
    main()
```

## Architecture Benefits

1. **Type Safety**: Widgets are structured data, not string templates
2. **Composability**: Widgets nest naturally like Flutter
3. **Separation of Concerns**: UI structure separate from rendering
4. **No HTML Knowledge Required**: Developers work with high-level widgets
5. **Automatic Optimization**: HTML/CSS/JS generated efficiently
6. **Flutter-like DX**: Familiar API for Flutter developers

## Future Enhancements

- **StatefulWidget/StatelessWidget**: Reactive state management
- **setState()**: Trigger UI updates
- **Navigator**: Route management and navigation
- **Animations**: AnimatedContainer, Tween, etc.
- **Themes**: Global styling system
- **InheritedWidget**: Data propagation down widget tree
- **Custom Widgets**: User-defined reusable components

## Generated Output

When you call `window.build(widget)`:

1. Widget tree traversed recursively
2. HTML structure generated from widget hierarchy
3. CSS styles generated from widget properties
4. JavaScript event handlers registered for interactions
5. All injected into WebView automatically

**You never write HTML/CSS/JS manually!**

---

**Tauraro WebViewTK** - Flutter-style UI framework for desktop apps 🚀
