# Windows Native GUI Library for Tauraro
# Provides simple access to Windows API for creating native GUI applications

# Load Windows user32.dll and kernel32.dll
user32 = load_library("user32.dll")
kernel32 = load_library("kernel32.dll")

# Define Windows constants
WS_OVERLAPPEDWINDOW = 0x00CF0000
WS_VISIBLE = 0x10000000
WS_CHILD = 0x40000000
WS_BORDER = 0x00800000
CW_USEDEFAULT = 0x80000000
SW_SHOW = 5
SW_HIDE = 0
WM_DESTROY = 0x0002
WM_CLOSE = 0x0010
WM_COMMAND = 0x0111
WM_LBUTTONDOWN = 0x0201
WM_PAINT = 0x000F
CS_HREDRAW = 0x0002
CS_VREDRAW = 0x0001
IDC_ARROW = 32512
COLOR_WINDOW = 5
MB_OK = 0x00000000
MB_OKCANCEL = 0x00000001
MB_ICONINFORMATION = 0x00000040
MB_ICONWARNING = 0x00000030
MB_ICONERROR = 0x00000010

# Extended window styles
WS_EX_CLIENTEDGE = 0x00000200

# Message constants
PM_REMOVE = 0x0001

# Define Windows API functions and get callable function objects
MessageBoxA = define_function("user32.dll", "MessageBoxA", "int", ["pointer", "pointer", "pointer", "int"])
CreateWindowExA = define_function("user32.dll", "CreateWindowExA", "pointer", ["int", "pointer", "pointer", "int", "int", "int", "int", "int", "pointer", "pointer", "pointer", "pointer"])
ShowWindow = define_function("user32.dll", "ShowWindow", "int", ["pointer", "int"])
UpdateWindow = define_function("user32.dll", "UpdateWindow", "int", ["pointer"])
DestroyWindow = define_function("user32.dll", "DestroyWindow", "int", ["pointer"])
GetModuleHandleA = define_function("kernel32.dll", "GetModuleHandleA", "pointer", ["pointer"])

# Additional Windows API functions for extended GUI features
MoveWindow = define_function("user32.dll", "MoveWindow", "int", ["pointer", "int", "int", "int", "int", "int"])
SetWindowTextA = define_function("user32.dll", "SetWindowTextA", "int", ["pointer", "pointer"])
GetWindowTextA = define_function("user32.dll", "GetWindowTextA", "int", ["pointer", "pointer", "int"])
GetWindowTextLengthA = define_function("user32.dll", "GetWindowTextLengthA", "int", ["pointer"])
GetWindowRect = define_function("user32.dll", "GetWindowRect", "int", ["pointer", "pointer"])
IsWindowVisible = define_function("user32.dll", "IsWindowVisible", "int", ["pointer"])
SetWindowPos = define_function("user32.dll", "SetWindowPos", "int", ["pointer", "pointer", "int", "int", "int", "int", "int"])
ClientToScreen = define_function("user32.dll", "ClientToScreen", "int", ["pointer", "pointer"])
GetSystemMetrics = define_function("user32.dll", "GetSystemMetrics", "int", ["int"])
MessageBeep = define_function("user32.dll", "MessageBeep", "int", ["int"])

# Message loop functions
GetMessageA = define_function("user32.dll", "GetMessageA", "int", ["pointer", "pointer", "int", "int"])
TranslateMessage = define_function("user32.dll", "TranslateMessage", "int", ["pointer"])
DispatchMessageA = define_function("user32.dll", "DispatchMessageA", "int", ["pointer"])
PostQuitMessage = define_function("user32.dll", "PostQuitMessage", "void", ["int"])

# Timing function
Sleep = define_function("kernel32.dll", "Sleep", "void", ["int"])

# Helper functions for window management
def create_window(title, width, height):
    # Create a native Windows window and return its handle
    h_instance = GetModuleHandleA(None)
    style = WS_OVERLAPPEDWINDOW | WS_VISIBLE
    x = CW_USEDEFAULT
    y = CW_USEDEFAULT

    # Try different window classes to see which works better
    # First try "EDIT" class which might be more responsive
    hwnd = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", title, style, x, y, width, height, None, None, h_instance, None)
    
    # If EDIT fails, try "BUTTON" class
    if hwnd == None:
        hwnd = CreateWindowExA(0, "BUTTON", title, style, x, y, width, height, None, None, h_instance, None)
    
    # If BUTTON fails, fall back to STATIC
    if hwnd == None:
        hwnd = CreateWindowExA(0, "STATIC", title, style, x, y, width, height, None, None, h_instance, None)

    if hwnd == None:
        print("Failed to create window")
        return None

    return hwnd

def create_window_centered(title, width, height):
    # Create a window centered on the screen
    screen_width = get_screen_width()
    screen_height = get_screen_height()

    x = (screen_width - width) // 2
    y = (screen_height - height) // 2

    h_instance = GetModuleHandleA(None)
    style = WS_OVERLAPPEDWINDOW | WS_VISIBLE

    # Try different window classes
    hwnd = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", title, style, x, y, width, height, None, None, h_instance, None)

    if hwnd == None:
        hwnd = CreateWindowExA(0, "BUTTON", title, style, x, y, width, height, None, None, h_instance, None)

    if hwnd == None:
        hwnd = CreateWindowExA(0, "STATIC", title, style, x, y, width, height, None, None, h_instance, None)

    if hwnd == None:
        print("Failed to create centered window")
        return None

    return hwnd

def show_window(hwnd):
    # Show a window
    if hwnd:
        ShowWindow(hwnd, SW_SHOW)
        UpdateWindow(hwnd)
        return True
    return False

def hide_window(hwnd):
    # Hide a window
    if hwnd:
        ShowWindow(hwnd, SW_HIDE)
        return True
    return False

def destroy_window(hwnd):
    # Destroy a window
    if hwnd:
        DestroyWindow(hwnd)
        return True
    return False

def message_box(text, title, style):
    # Display a Windows message box
    return MessageBoxA(None, text, title, style)

def create_button(parent_hwnd, text, x, y, width, height, id):
    # Create a button control
    h_instance = GetModuleHandleA(None)
    hwnd = CreateWindowExA(0, "BUTTON", text, WS_CHILD | WS_VISIBLE, x, y, width, height, parent_hwnd, None, h_instance, None)
    
    if hwnd == None:
        print("Failed to create button")
        return None
    
    return hwnd

def create_textbox(parent_hwnd, x, y, width, height, id):
    # Create a text input control
    h_instance = GetModuleHandleA(None)
    hwnd = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, x, y, width, height, parent_hwnd, None, h_instance, None)
    
    if hwnd == None:
        print("Failed to create textbox")
        return None
    
    return hwnd

# Extended GUI functions
def get_screen_width():
    # Get screen width
    return GetSystemMetrics(0)  # SM_CXSCREEN

def get_screen_height():
    # Get screen height
    return GetSystemMetrics(1)  # SM_CYSCREEN

def get_screen_size():
    # Get screen size as tuple (width, height)
    width = GetSystemMetrics(0)  # SM_CXSCREEN
    height = GetSystemMetrics(1)  # SM_CYSCREEN
    return (width, height)

def move_window(hwnd, x, y, width, height, repaint=True):
    # Move and resize window
    if hwnd:
        if repaint:
            repaint_flag = 1
        else:
            repaint_flag = 0
        return MoveWindow(hwnd, x, y, width, height, repaint_flag)
    return False

def set_window_title(hwnd, title):
    # Set window title
    if hwnd and title:
        return SetWindowTextA(hwnd, title)
    return False

def get_window_title(hwnd):
    # Get window title
    if hwnd:
        # Get the length of the text
        length = GetWindowTextLengthA(hwnd)
        if length > 0:
            # For now, return a placeholder - a full implementation would need
            # proper memory management for the text buffer
            return "Window Title"
    return ""

def is_window_visible(hwnd):
    # Check if window is visible
    if hwnd:
        return IsWindowVisible(hwnd) != 0
    return False

def beep(sound_type=0):
    # Play system sound
    MessageBeep(sound_type)
    return 1

def beep_info():
    # Play information sound
    MessageBeep(0x00000040)  # MB_ICONINFORMATION
    return 1

def beep_warning():
    # Play warning sound
    MessageBeep(0x00000030)  # MB_ICONWARNING
    return 1

def beep_error():
    # Play error sound
    MessageBeep(0x00000010)  # MB_ICONERROR
    return 1

def beep_question():
    # Play question sound
    MessageBeep(0x00000020)  # MB_ICONQUESTION
    return 1

def set_window_position(hwnd, x, y, width=None, height=None):
    # Set window position
    if hwnd and x != None and y != None:
        if width != None and height != None:
            # Move and resize
            return MoveWindow(hwnd, x, y, width, height, 1)
        else:
            # Just move
            # For now, we'll use a default size
            return MoveWindow(hwnd, x, y, 300, 200, 1)
    return False

def get_window_rect(hwnd):
    # Get window rectangle coordinates
    if hwnd:
        # This is a simplified implementation - a full implementation would need
        # proper handling of the RECT structure
        # Return a placeholder tuple (left, top, right, bottom)
        return (0, 0, 300, 200)
    return None

def keep_window_alive(seconds):
    # Keep a window alive for specified seconds using Sleep
    # This prevents windows from disappearing or becoming unresponsive
    # Sleep takes milliseconds, so multiply seconds by 1000
    milliseconds = seconds * 1000
    Sleep(milliseconds)
    return True

def show_window_for(hwnd, seconds):
    # Show a window for specified number of seconds
    # This is useful for demonstrations or temporary windows
    if hwnd:
        show_window(hwnd)
        keep_window_alive(seconds)
        return True
    return False

def show_window_with_message(hwnd, message_title="Window", message_text="Close this dialog to continue"):
    # Show a window and display a message box to keep it visible
    # The window will remain visible until the user closes the message box
    if hwnd:
        show_window(hwnd)
        message_box(message_text, message_title, MB_OK | MB_ICONINFORMATION)
        return True
    return False
