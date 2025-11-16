# WebViewTK Framework - Flutter-Style Rebuild Complete

## Overview
Complete from-scratch rebuild of WebViewTK with clean Flutter-style architecture.

## Architecture

### Core Modules
- **mod.rs** - Main module with VM integration
- **rendering.rs** - HTML generation engine with RenderObject pattern
- **utils.rs** - Utilities (EdgeInsets, extract_kwargs, widget IDs)
- **window.rs** - Cross-platform window management with WRY
- **ipc.rs** - Inter-process communication (basic structure)

### Rendering Pipeline
```
Widget → RenderObject → HTML String → Widget Dict
```

Each widget:
1. Accepts keyword arguments (Flutter-style)
2. Builds a RenderObject tree
3. Converts to HTML
4. Returns dict with `_widget_id`, `_widget_type`, `_html`

### Window System
- WRY-based cross-platform webview
- Event loop integration
- Tailwind CSS auto-included
- HTML template generation

## Widget Status

### ✅ Fully Implemented (7 widgets)

#### Basic Widgets
- **Text** - Display text with styling
  ```python
  Text("Hello", font_size=24.0, color="#0000ff")
  ```
  
- **Container** - Box model container
  ```python
  Container(
      child=Text("Content"),
      padding=EdgeInsets.all(16.0),
      background_color="#ffffff",
      border_radius="8px"
  )
  ```

- **Center** - Center child widget
  ```python
  Center(child=Text("Centered"))
  ```

- **Padding** - Add padding around child
  ```python
  Padding(
      child=Text("Padded"),
      padding=EdgeInsets.symmetric(horizontal=20.0, vertical=10.0)
  )
  ```

- **SizedBox** - Fixed-size container
  ```python
  SizedBox(width=100.0, height=50.0, child=Text("Fixed size"))
  ```

#### Layout Widgets
- **Column** - Vertical layout with flexbox
  ```python
  Column(
      children=[widget1, widget2, widget3],
      spacing=10.0,
      main_axis_alignment="center",
      cross_axis_alignment="stretch"
  )
  ```

- **Row** - Horizontal layout with flexbox
  ```python
  Row(
      children=[widget1, widget2, widget3],
      spacing=10.0
  )
  ```

### ⏳ Templated (15 widgets - need implementation)

#### Layout Widgets
- Stack
- Expanded
- Flexible
- Spacer
- Positioned

#### Material Widgets
- Button
- TextField
- Card
- Scaffold
- AppBar
- FloatingActionButton (FAB)
- Divider
- ListTile

#### Gesture Widgets
- GestureDetector
- InkWell

## Features

### Keyword Arguments Support
All widgets support Flutter-style keyword arguments:
```python
Text(
    "Hello",
    font_size=24.0,
    font_weight="bold",
    color="#2563eb"
)
```

Also supports dictionary-style:
```python
Text({"text": "Hello", "font_size": 24.0})
```

### EdgeInsets Helper
```python
EdgeInsets.all(16.0)
EdgeInsets.symmetric(horizontal=20.0, vertical=10.0)
EdgeInsets.only(top=10.0, bottom=20.0, left=5.0, right=5.0)
EdgeInsets.zero()
```

### Flexbox Alignment

**Main Axis Alignment** (Column: vertical, Row: horizontal)
- `"start"` - Align to start
- `"center"` - Center items
- `"end"` - Align to end
- `"space_between"` - Space between items
- `"space_around"` - Space around items
- `"space_evenly"` - Even spacing

**Cross Axis Alignment** (Column: horizontal, Row: vertical)
- `"start"` - Align to start
- `"center"` - Center items
- `"end"` - Align to end
- `"stretch"` - Stretch to fill

## Example Usage

### Hello World
```python
from webviewtk import Window, Text, Center

ui = Center(
    child=Text("Hello, World!", font_size=32.0, color="#0000ff")
)

window = Window(title="Hello", width=800, height=600)
window.mount_and_run(ui)
```

### Complex Layout
```python
from webviewtk import Window, Column, Row, Container, Text, Center

ui = Center(
    child=Column(
        children=[
            Text("Title", font_size=32.0, font_weight="bold"),
            Container(height=20.0),
            Row(
                children=[
                    Container(
                        child=Text("Box 1", color="#ffffff"),
                        padding=EdgeInsets.all(16.0),
                        background_color="#ef4444",
                        border_radius="8px",
                        width=150.0
                    ),
                    Container(
                        child=Text("Box 2", color="#ffffff"),
                        padding=EdgeInsets.all(16.0),
                        background_color="#3b82f6",
                        border_radius="8px",
                        width=150.0
                    )
                ],
                spacing=20.0,
                main_axis_alignment="center"
            )
        ],
        spacing=10.0,
        main_axis_alignment="center",
        cross_axis_alignment="center"
    )
)

window = Window(title="Layout Demo", width=800, height=600)
window.mount_and_run(ui)
```

## Implementation Details

### RenderObject Pattern
```rust
pub struct RenderObject {
    pub tag: String,
    pub styles: Vec<String>,
    pub attributes: Vec<(String, String)>,
    pub text: Option<String>,
    pub children: Vec<RenderObject>,
}

impl RenderObject {
    pub fn new(tag: &str) -> Self { ... }
    pub fn with_style(self, style: &str) -> Self { ... }
    pub fn with_attr(self, key: &str, value: &str) -> Self { ... }
    pub fn with_text(self, text: &str) -> Self { ... }
    pub fn with_child(self, child: RenderObject) -> Self { ... }
    pub fn to_html(&self) -> String { ... }
}
```

### Widget Pattern
```rust
pub fn create(args: Vec<Value>) -> Result<Value> {
    // 1. Extract kwargs
    let kwargs = extract_kwargs(&args)?;
    
    // 2. Build RenderObject
    let render = RenderObject::new("div")
        .with_style("display: flex")
        .with_child(child_render);
    
    // 3. Generate HTML
    let html = render.to_html();
    
    // 4. Return widget dict
    Ok(Value::Dict(Rc::new(RefCell::new(HashMap::from([
        ("_widget_id".to_string(), Value::Str(generate_widget_id())),
        ("_widget_type".to_string(), Value::Str("WidgetName".to_string())),
        ("_html".to_string(), Value::Str(html))
    ])))))
}
```

## Compilation Status

✅ **Successfully compiles** with `cargo build --features webviewtk`

- 0 errors
- 480 warnings (mostly unused variables in unrelated modules)
- All WebViewTK modules compile cleanly

## Next Steps

### Priority 1: Implement Core Material Widgets
1. **Button** - Click handling with events
2. **TextField** - Text input with validation
3. **Scaffold** - Page structure (AppBar + Body + FAB)
4. **AppBar** - Navigation bar with title and actions

### Priority 2: Implement Layout Widgets
1. **Stack** - Absolute positioning
2. **Expanded** - Flex child
3. **Positioned** - Positioned in Stack

### Priority 3: Event Handling
1. Complete IPC module for JavaScript ↔ Tauraro communication
2. Event propagation system
3. GestureDetector implementation
4. InkWell with Material ripples

### Priority 4: State Management
1. StatefulWidget system
2. State class with setState()
3. BuildContext tree
4. Widget rebuild on state change

### Priority 5: Advanced Features
1. Navigation and routing
2. Animations
3. Themes and styling
4. Hot reload
5. Mobile platform support

## File Structure

```
src/modules/webviewtk/
├── mod.rs                    # Main module (77 lines)
├── rendering.rs              # HTML generation (171 lines)
├── utils.rs                  # Utilities (102 lines)
├── window.rs                 # Window management (170 lines)
├── ipc.rs                    # IPC (basic, 13 lines)
└── widgets/
    ├── mod.rs                # Widget organization (56 lines)
    ├── text.rs               # ✅ Complete (59 lines)
    ├── container.rs          # ✅ Complete (93 lines)
    ├── center.rs             # ✅ Complete (37 lines)
    ├── padding.rs            # ✅ Complete (42 lines)
    ├── sized_box.rs          # ✅ Complete (42 lines)
    ├── column.rs             # ✅ Complete (92 lines)
    ├── row.rs                # ✅ Complete (69 lines)
    ├── stack.rs              # ⏳ Template
    ├── expanded.rs           # ⏳ Template
    ├── flexible.rs           # ⏳ Template
    ├── spacer.rs             # ⏳ Template
    ├── positioned.rs         # ⏳ Template
    ├── button.rs             # ⏳ Template
    ├── textfield.rs          # ⏳ Template
    ├── card.rs               # ⏳ Template
    ├── scaffold.rs           # ⏳ Template
    ├── appbar.rs             # ⏳ Template
    ├── fab.rs                # ⏳ Template
    ├── divider.rs            # ⏳ Template
    ├── list_tile.rs          # ⏳ Template
    ├── gesture_detector.rs   # ⏳ Template
    └── ink_well.rs           # ⏳ Template
```

**Total:** 27 files, ~1020 lines of WebViewTK code (excluding templates)

## Technical Notes

### HPList Iteration
HPList requires `.iter()` not `.borrow().iter()`:
```rust
for child in list.iter() {  // ✅ Correct
    // ...
}

for child in list.borrow().iter() {  // ❌ Wrong
    // ...
}
```

### Widget Dict Format
All widgets return this dict structure:
```rust
{
    "_widget_id": "widget_123",
    "_widget_type": "Text",
    "_html": "<span style='font-size: 24px;'>Hello</span>"
}
```

### Window Integration
```rust
// window.rs - mount_and_run()
1. Extract HTML from widget dict
2. Generate full HTML document with Tailwind CSS
3. Create WRY EventLoop and WindowBuilder
4. Create WebView with HTML content
5. Run event loop
6. Handle WindowEvent::CloseRequested
```

## Dependencies

### Cargo.toml
```toml
[features]
webviewtk = ["wry"]

[dependencies]
wry = { version = "0.43", optional = true }
```

### WRY Version
- wry = 0.43 (cross-platform webview)
- Windows: WebView2 (automatically detected)
- macOS: WKWebView
- Linux: WebKitGTK

## Acknowledgments

Complete from-scratch rebuild based on Flutter's widget architecture:
- ✅ Keyword arguments (Flutter-style API)
- ✅ Widget composition pattern
- ✅ RenderObject tree
- ✅ Flexbox layouts (Column, Row)
- ✅ Material Design ready
- ✅ Cross-platform with WRY
- ⏳ StatefulWidget (planned)
- ⏳ BuildContext (planned)
- ⏳ Hot reload (planned)
