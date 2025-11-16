# WebViewTK Test Suite

Comprehensive test examples for WebViewTK framework.

## Running Tests

```bash
# Using debug build
cargo build --features webviewtk
.\target\debug\tauraro.exe run examples\webviewtk_tests\01_hello_world.tr

# Using release build
cargo build --release --features webviewtk
.\target\release\tauraro.exe run examples\webviewtk_tests\01_hello_world.tr
```

## Test Overview

### Basic Tests

| Test | Description | Widgets Used |
|------|-------------|--------------|
| 01_hello_world | Simple text display | Text, Center |
| 02_container_styles | Container with full styling | Container, Text, EdgeInsets |
| 03_column_layout | Vertical layout with spacing | Column, Text |
| 04_row_layout | Horizontal layout with colored boxes | Row, Container, Text |
| 05_nested_layouts | Column inside Row and vice versa | Column, Row, Container |

### Styling Tests

| Test | Description | Widgets Used |
|------|-------------|--------------|
| 06_edge_insets | All EdgeInsets variations | EdgeInsets, Container, Text |
| 07_alignment_test | Main and cross axis alignment | Column, Row, Container |
| 08_text_styles | Text widget styling options | Text, Container |
| 09_sized_box | Fixed dimensions and spacing | SizedBox, Container |
| 10_padding_widget | Padding widget variations | Padding, Container, Text |

### Complex Tests

| Test | Description | Widgets Used |
|------|-------------|--------------|
| 11_complex_card | User profile cards | All basic widgets |
| 12_dashboard_layout | Full dashboard UI | All widgets + functions |

## Widget Coverage

### ✅ Fully Tested
- Text
- Container
- Center
- Padding
- SizedBox
- Column
- Row
- EdgeInsets (all variations)

### Widget Features Tested

#### Text
- [x] Basic text display
- [x] font_size
- [x] font_weight
- [x] color
- [x] text_align
- [x] custom style

#### Container
- [x] child widget
- [x] width and height
- [x] padding (EdgeInsets)
- [x] margin (EdgeInsets)
- [x] background_color
- [x] border_radius
- [x] border
- [x] custom style

#### Column
- [x] children array
- [x] spacing
- [x] main_axis_alignment (start, center, end, space_between, space_around, space_evenly)
- [x] cross_axis_alignment (start, center, end, stretch)

#### Row
- [x] children array
- [x] spacing
- [x] custom style

#### SizedBox
- [x] width
- [x] height
- [x] child widget
- [x] empty (for spacing)

#### Padding
- [x] child widget
- [x] padding (EdgeInsets)

#### EdgeInsets
- [x] all()
- [x] symmetric(horizontal, vertical)
- [x] only(top, right, bottom, left)
- [x] zero()

## Test Results

Run all tests to verify:

```bash
# Run all tests (PowerShell)
Get-ChildItem examples\webviewtk_tests\*.tr | ForEach-Object {
    Write-Host "Testing: $($_.Name)" -ForegroundColor Cyan
    .\target\debug\tauraro.exe run $_.FullName
    Start-Sleep -Seconds 2
}
```

## Expected Visual Output

### 01_hello_world
- Blue "Hello, WebViewTK!" text centered on screen

### 02_container_styles
- Blue container with white text, rounded corners, border

### 03_column_layout
- 5 colored text items stacked vertically with spacing

### 04_row_layout
- 3 colored boxes arranged horizontally

### 05_nested_layouts
- Title with 2x2 grid of colored boxes below

### 06_edge_insets
- 4 containers demonstrating different padding styles

### 07_alignment_test
- 5 rows showing different alignment options

### 08_text_styles
- Various text styling demonstrations

### 09_sized_box
- Fixed-size colored boxes with spacing

### 10_padding_widget
- 4 containers with different padding configurations

### 11_complex_card
- 3 user profile cards with avatar, name, role, description

### 12_dashboard_layout
- Full dashboard with header, stats, activity feed, sidebar

## Creating New Tests

Template for new test:

```python
"""
WebViewTK Test XX: Test Name
Description of what this test demonstrates
"""

from webviewtk import Window, Text, Center, Container, Column, Row, EdgeInsets, SizedBox, Padding

# Build your UI
ui = Center(
    child=Text("Your test content")
)

# Create window
window = Window(
    title="Test XX: Test Name",
    width=800,
    height=600
)

# Run
window.mount_and_run(ui)
```

## Troubleshooting

### Window doesn't appear
- Ensure WebView2 is installed (Windows)
- Check webviewtk feature is enabled: `--features webviewtk`

### Layout issues
- Check alignment settings (main_axis_alignment, cross_axis_alignment)
- Verify spacing values
- Inspect EdgeInsets values

### Styling not applied
- Check CSS syntax in style attribute
- Verify color format (#RRGGBB)
- Check border-radius and border syntax

## Next Steps

Additional tests to create:
- [ ] Stack widget tests (when implemented)
- [ ] Button widget tests (when implemented)
- [ ] TextField widget tests (when implemented)
- [ ] Scaffold layout tests (when implemented)
- [ ] Event handling tests (when implemented)
- [ ] IPC communication tests (when implemented)
