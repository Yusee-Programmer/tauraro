# Comprehensive WebViewTK Examples with Custom Title Bars

This directory contains professional, production-ready examples of WebViewTK applications with custom title bars. All examples use `win.disable_decorations()` to remove the default window frame and implement beautiful custom title bars with draggable regions.

## 📋 Examples Overview

### 1. **01_modern_todo_app.tr** - Task Management
A beautiful todo application with custom window controls.

**Features:**
- ✅ Custom draggable title bar
- ✅ Add, complete, and delete tasks
- ✅ Task persistence and state management
- ✅ Gradient UI design
- ✅ Empty state handling

**IPC Events:**
- `add_task` - Add new task
- `toggle_task` - Mark task complete/incomplete
- `delete_task` - Remove task
- `minimize`, `maximize`, `close` - Window controls

**Run:**
```bash
.\target\debug\tauraro.exe run .\examples\01_modern_todo_app.tr
```

---

### 2. **02_music_player_ui.tr** - Media Player Interface
Modern music player UI inspired by Spotify.

**Features:**
- 🎵 Custom title bar with app branding
- 🎵 Album art display
- 🎵 Playback controls (play, pause, next, previous)
- 🎵 Volume control slider
- 🎵 Progress bar with seek functionality
- 🎵 Sidebar navigation

**IPC Events:**
- `play`, `pause` - Playback control
- `next`, `previous` - Track navigation
- `volume` - Volume adjustment
- `seek` - Seek to position

**Run:**
```bash
.\target\debug\tauraro.exe run .\examples\02_music_player_ui.tr
```

---

### 3. **03_code_editor_ui.tr** - Code Editor
VS Code-inspired code editor interface.

**Features:**
- ⚡ Dark theme IDE layout
- ⚡ File explorer sidebar
- ⚡ Tab management
- ⚡ Syntax-ready code area
- ⚡ Toolbar with quick actions
- ⚡ Status bar
- ⚡ Menu bar in title bar

**IPC Events:**
- `file_new`, `file_open`, `file_save` - File operations
- `code_change` - Editor content changed
- `run` - Execute code

**Run:**
```bash
.\target\debug\tauraro.exe run .\examples\03_code_editor_ui.tr
```

---

### 4. **04_chat_app_ui.tr** - Messaging Application
Modern chat interface with contacts and messages.

**Features:**
- 💬 Custom title bar with app icon
- 💬 Contacts sidebar with search
- 💬 Message bubbles (sent/received)
- 💬 Voice and video call buttons
- 💬 Auto-resizing message input
- 💬 Online status indicators

**IPC Events:**
- `send_message` - Send chat message
- `typing` - User typing indicator
- `call_voice`, `call_video` - Initiate calls

**Run:**
```bash
.\target\debug\tauraro.exe run .\examples\04_chat_app_ui.tr
```

---

### 5. **05_dashboard_app.tr** - Analytics Dashboard
Business intelligence dashboard with stats and charts.

**Features:**
- 📊 Light theme professional design
- 📊 KPI stat cards
- 📊 Chart placeholders
- 📊 Data tables with status badges
- 📊 Navigation in title bar
- 📊 Export functionality

**IPC Events:**
- `refresh` - Refresh dashboard data
- `export` - Export data to CSV
- `settings` - Open settings
- `notifications` - View notifications

**Run:**
```bash
.\target\debug\tauraro.exe run .\examples\05_dashboard_app.tr
```

---

### 6. **06_settings_app.tr** - Settings Interface
Modern settings panel with multiple configuration options.

**Features:**
- ⚙️ Dark theme settings UI
- ⚙️ Sidebar navigation
- ⚙️ Toggle switches
- ⚙️ Dropdown selectors
- ⚙️ Input fields
- ⚙️ Save/reset functionality

**IPC Events:**
- `theme_change` - Change app theme
- `notification_toggle` - Toggle notifications
- `auto_update_toggle` - Toggle auto-updates
- `save`, `reset` - Save or reset settings

**Run:**
```bash
.\target\debug\tauraro.exe run .\examples\06_settings_app.tr
```

---

## 🎨 Custom Title Bar Implementation

All examples use the following pattern:

### 1. Disable Default Decorations
```python
win = Window("App Name", 1000, 700)
win.disable_decorations()  # Remove default title bar
```

### 2. HTML Title Bar Structure
```html
<div class="titlebar">
    <div class="titlebar-title">
        <span>🔥</span>
        <span>App Name</span>
    </div>
    <div class="titlebar-controls">
        <button onclick="minimize()">−</button>
        <button onclick="maximize()">□</button>
        <button class="close" onclick="closeApp()">×</button>
    </div>
</div>
```

### 3. CSS for Draggable Title Bar
```css
.titlebar {
    -webkit-app-region: drag;  /* Make entire bar draggable */
    background: #2a2a2a;
    height: 40px;
    /* ... other styles ... */
}

.titlebar-controls {
    -webkit-app-region: no-drag;  /* Make buttons clickable */
    /* ... button styles ... */
}
```

### 4. IPC Handlers for Window Controls
```python
func handleMinimize(data):
    print("Minimize window")

func handleMaximize(data):
    print("Maximize window")

func handleClose(data):
    print("Close window")

win.on_message("minimize", handleMinimize)
win.on_message("maximize", handleMaximize)
win.on_message("close", handleClose)
```

---

## 🚀 Key Features Across All Examples

### ✨ Custom Title Bars
- **Draggable regions** using `-webkit-app-region: drag`
- **Custom window controls** (minimize, maximize, close)
- **App branding** with icons and titles
- **Menu integration** (some examples)

### 🎯 Modern UI/UX
- **Professional design** with gradients and shadows
- **Responsive layouts** using flexbox/grid
- **Smooth animations** and transitions
- **Consistent styling** patterns

### 🔗 IPC Communication
- **Event-driven architecture** using `window.ipc.postMessage()`
- **Backend handlers** in Tauraro
- **State management** with global variables
- **Real-time updates** via event processing

### 📱 Responsive Design
- **Flexible layouts** that adapt to window size
- **Scrollable content** areas
- **Proper overflow** handling
- **Mobile-first** approach

---

## 🎓 Learning Path

**Beginners:**
1. Start with `01_modern_todo_app.tr` - simplest example
2. Understand custom title bar pattern
3. Learn basic IPC communication

**Intermediate:**
1. Explore `02_music_player_ui.tr` or `04_chat_app_ui.tr`
2. Study complex layouts (sidebar + content)
3. Implement sliders and interactive controls

**Advanced:**
1. Analyze `03_code_editor_ui.tr` or `05_dashboard_app.tr`
2. Multi-panel interfaces
3. Data visualization patterns
4. Complex state management

---

## 🛠️ Technical Patterns

### Window Creation Pattern
```python
from webviewtk import Window
import time

win = Window("Title", width, height)
win.disable_decorations()

# Register handlers
win.on_message("event", handler)

# Set HTML
win.set_html(html)

# Run async
win.run_async()

# Event loop
for i in range(600):
    win.process_events()
    time.sleep(0.1)
```

### State Management Pattern
```python
# Global state
counter = 0

# Handler with state modification
func handleIncrement(data):
    global counter
    counter = counter + 1
    print("Counter:", counter)
```

### CSS Draggable Region Pattern
```css
/* Draggable area */
.titlebar {
    -webkit-app-region: drag;
}

/* Non-draggable interactive elements */
.titlebar-controls,
.titlebar-menu {
    -webkit-app-region: no-drag;
}
```

---

## 🎨 Design Guidelines

### Title Bar Design
- **Height**: 35-45px recommended
- **Colors**: Match your app theme
- **Icons**: Use emoji or icon fonts
- **Controls**: Right-aligned minimize, maximize, close
- **Hover effects**: Visual feedback on hover

### Color Schemes
- **Dark themes**: `#1a1a1a`, `#2a2a2a`, `#3a3a3a`
- **Light themes**: `#f5f7fa`, `#ffffff`, `#e5e7eb`
- **Accents**: `#667eea`, `#764ba2`, `#1da1f2`

### Typography
- **Title**: 13-14px, weight 500-600
- **Body**: 14-15px, weight 400
- **Small**: 11-13px for metadata

---

## 📦 Dependencies

All examples use:
- **webviewtk** - Window creation and IPC
- **time** - Event loop timing

No external dependencies required!

---

## 🐛 Troubleshooting

### Window not draggable?
Ensure `-webkit-app-region: drag` is set on the title bar container.

### Buttons not clickable?
Add `-webkit-app-region: no-drag` to button containers.

### IPC not working?
Check that handlers are registered before calling `win.set_html()`.

### Window controls not working?
Implement actual window control logic in handlers (minimize/maximize/close).

---

## 💡 Tips & Best Practices

1. **Always disable decorations first** before setting HTML
2. **Register all handlers** before calling `set_html()`
3. **Use semantic HTML** for better structure
4. **Keep CSS modular** with clear class names
5. **Test draggable regions** thoroughly
6. **Handle window resize** gracefully
7. **Provide visual feedback** for all interactions
8. **Use consistent spacing** (8px, 16px, 24px grid)

---

## 🎯 Next Steps

**Extend these examples:**
- Add file system operations
- Implement persistent storage
- Connect to APIs
- Add more complex animations
- Create multi-window apps
- Build native menu integration

**Create your own:**
- Use these as templates
- Mix and match UI components
- Implement your own IPC events
- Design custom themes

---

## 📝 License

These examples are part of the Tauraro project and follow the same license.

## 🤝 Contributing

Feel free to create more examples and submit PRs!

---

**Happy Building! 🚀**
