# WebViewTK Responsive Design

## Overview

All WebViewTK applications are now **responsive by default** with modern web standards and Tailwind CSS 3.4.1 included automatically.

## What's Included

### 1. **Tailwind CSS CDN**
- **Version**: 3.4.1 (latest stable)
- **CDN**: `https://cdn.jsdelivr.net/npm/tailwindcss@3.4.1/dist/tailwind.min.css`
- **Usage**: All Tailwind utility classes are available in custom HTML widgets

### 2. **Responsive Viewport**
```html
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
```
- Prevents unwanted scaling
- Ensures consistent rendering across different displays
- Optimized for desktop applications

### 3. **Global Responsive Styles**

#### Box Sizing
```css
* { 
    margin: 0; 
    padding: 0; 
    box-sizing: border-box; 
}
```

#### Full-Height Layout
```css
html, body {
    width: 100%;
    height: 100%;
    overflow: hidden;
}
```

#### Responsive Root Container
```css
#root {
    width: 100%;
    height: 100%;
    display: flex;
    flex-direction: column;
}
```

### 4. **Typography**
- **System Font Stack**: `system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif`
- **Font Smoothing**: Optimized for crisp text rendering on all platforms
- **Responsive Text Classes**:
  - `.responsive-text`: `font-size: clamp(0.875rem, 2vw, 1rem)`
  - `.responsive-heading`: `font-size: clamp(1.5rem, 4vw, 2.5rem)`

### 5. **Smooth Animations**
```css
* {
    transition: all 0.2s ease;
}
```
All elements have smooth transitions for better UX.

### 6. **Custom Scrollbars** (Windows optimized)
- Width: 8px
- Rounded corners
- Hover effects
- Modern, minimal design

## Using Tailwind CSS

### Example: Applying Utility Classes

While WebViewTK provides widget-based styling, you can also use Tailwind classes in custom HTML:

```python
from webviewtk import Window, Container, Text, mount_and_run

# Using standard WebViewTK styling
ui = Container(
    child=Text("Hello World", font_size="24px", color="#1e40af"),
    padding="20px",
    background_color="#f3f4f6",
    border_radius="12px"
)

window = Window(title="My App", width=800, height=600)
mount_and_run(window, ui)
```

### Tailwind Utility Examples

If you create custom HTML widgets (future feature), you can use:

- **Colors**: `text-blue-600`, `bg-gray-100`, `border-red-500`
- **Spacing**: `p-4`, `m-2`, `mx-auto`, `space-y-4`
- **Flexbox**: `flex`, `flex-col`, `items-center`, `justify-between`
- **Grid**: `grid`, `grid-cols-3`, `gap-4`
- **Typography**: `text-xl`, `font-bold`, `text-center`
- **Borders**: `rounded-lg`, `border-2`, `shadow-lg`
- **Responsive**: `sm:text-base`, `md:text-lg`, `lg:text-xl`

## Responsive Window Behavior

### Resizable Windows
```python
window = Window(
    title="Responsive App",
    width=800,
    height=600,
    resizable=True  # Users can resize the window
)
```

### Content Adaptation
- All WebViewTK widgets automatically adapt to window size changes
- Flexbox-based layouts reflow naturally
- Text and containers scale appropriately

## Benefits

### 1. **Zero Configuration**
- No need to include CSS files manually
- Responsive features work out of the box
- Consistent across all applications

### 2. **Modern Design**
- Professional appearance by default
- Smooth animations and transitions
- Native-feeling scrollbars

### 3. **Developer Friendly**
- Familiar Tailwind CSS utilities
- Standard CSS properties in widgets
- Predictable behavior

### 4. **Performance**
- CDN delivers optimized CSS
- Minimal overhead
- Fast initial load

## Testing Responsive Design

### Example: Responsive Dashboard
```python
from webviewtk import Window, Container, Column, Row, Text, Center, mount_and_run

ui = Container(
    child=Column(
        children=[
            Text(
                "Dashboard",
                font_size="36px",
                color="#1e293b",
                font_weight="bold"
            ),
            Row(
                children=[
                    Container(
                        child=Text("Card 1", font_size="18px"),
                        padding="20px",
                        background_color="#3b82f6",
                        border_radius="12px",
                        flex="1"
                    ),
                    Container(
                        child=Text("Card 2", font_size="18px"),
                        padding="20px",
                        background_color="#10b981",
                        border_radius="12px",
                        flex="1"
                    ),
                ],
                gap="20px"
            )
        ],
        gap="20px"
    ),
    padding="40px"
)

window = Window(
    title="Responsive Dashboard",
    width=1000,
    height=700,
    resizable=True
)
mount_and_run(window, ui)
```

### Try It
1. Run any WebViewTK example
2. Resize the window
3. Notice how content adapts smoothly

## Future Enhancements

### Planned Features
- [ ] Custom CDN configuration (choose your own CSS framework)
- [ ] Dark mode support
- [ ] Theme customization
- [ ] Breakpoint-aware widget properties
- [ ] Custom CSS injection API

### Custom CDN (Future)
```python
window = Window(
    title="Custom CSS",
    width=800,
    height=600,
    cdn_links=[
        "https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css",
        "https://fonts.googleapis.com/css2?family=Inter:wght@400;700&display=swap"
    ]
)
```

## Browser Compatibility

WebViewTK uses the native webview on each platform:
- **Windows**: Edge WebView2 (Chromium-based)
- **macOS**: WKWebView (Safari/WebKit)
- **Linux**: WebKitGTK

All modern CSS features including Flexbox, Grid, and CSS custom properties are fully supported.

## Performance Tips

### 1. **Avoid Inline Styles for Large Lists**
Use widget properties instead of inline CSS for better performance.

### 2. **Leverage Tailwind Utilities**
When building custom HTML (future), Tailwind classes are more efficient than inline styles.

### 3. **Minimize DOM Depth**
Keep widget nesting reasonable for optimal rendering.

### 4. **Use Flexbox Wisely**
Column and Row widgets are optimized for layout—prefer them over custom CSS.

## Conclusion

WebViewTK now provides a **production-ready responsive foundation** for desktop applications with:
✅ Modern CSS framework (Tailwind CSS 3.4.1)  
✅ Responsive viewport and layouts  
✅ Smooth animations and transitions  
✅ Professional scrollbars and typography  
✅ Zero configuration required  

All applications are responsive by default—just focus on building great UIs!
