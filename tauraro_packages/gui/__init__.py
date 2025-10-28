# Windows Native GUI Library for Tauraro
# Provides simple access to Windows API for creating native GUI applications

# Load Windows user32.dll and kernel32.dll
user32 = load_library("user32.dll")
kernel32 = load_library("kernel32.dll")

# Define Windows constants
WS_OVERLAPPEDWINDOW = 0x00CF0000
WS_VISIBLE = 0x10000000
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

# Define Windows API functions and get callable function objects
MessageBoxA = define_function("user32.dll", "MessageBoxA", "int", ["pointer", "pointer", "pointer", "int"])
CreateWindowExA = define_function("user32.dll", "CreateWindowExA", "pointer", ["int", "pointer", "pointer", "int", "int", "int", "int", "int", "pointer", "pointer", "pointer", "pointer"])
ShowWindow = define_function("user32.dll", "ShowWindow", "int", ["pointer", "int"])
UpdateWindow = define_function("user32.dll", "UpdateWindow", "int", ["pointer"])
DestroyWindow = define_function("user32.dll", "DestroyWindow", "int", ["pointer"])
GetModuleHandleA = define_function("kernel32.dll", "GetModuleHandleA", "pointer", ["pointer"])

# Helper functions for window management
def create_window(title, width, height):
    # Create a native Windows window and return its handle
    h_instance = GetModuleHandleA(None)
    style = WS_OVERLAPPEDWINDOW | WS_VISIBLE
    x = CW_USEDEFAULT
    y = CW_USEDEFAULT

    hwnd = CreateWindowExA(0, "STATIC", title, style, x, y, width, height, None, None, h_instance, None)

    if hwnd == None:
        print("Failed to create window")
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
