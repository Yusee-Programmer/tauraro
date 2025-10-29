# Windows Native GUI Framework for Tauraro - V3.0 Responsive Windows
# Class-based with proper message loop to fix "not responding" issue

# Load Windows DLLs
user32 = load_library("user32.dll")
kernel32 = load_library("kernel32.dll")

# Constants
WS_OVERLAPPED = 0x00000000
WS_CAPTION = 0x00C00000
WS_SYSMENU = 0x00080000
WS_THICKFRAME = 0x00040000
WS_MINIMIZEBOX = 0x00020000
WS_MAXIMIZEBOX = 0x00010000
WS_VISIBLE = 0x10000000
WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
WS_EX_CLIENTEDGE = 0x00000200

SW_HIDE = 0
SW_SHOW = 5
SW_MINIMIZE = 6
SW_RESTORE = 9

SM_CXSCREEN = 0
SM_CYSCREEN = 1

MB_OK = 0x00000000
MB_OKCANCEL = 0x00000001
MB_YESNO = 0x00000004
MB_ICONINFORMATION = 0x00000040
MB_ICONWARNING = 0x00000030
MB_ICONERROR = 0x00000010
MB_ICONQUESTION = 0x00000020

IDOK = 1
IDCANCEL = 2
IDYES = 6
IDNO = 7

CW_USEDEFAULT = 0x80000000
PM_REMOVE = 0x0001

# Define Windows API functions
MessageBoxA = define_function("user32.dll", "MessageBoxA", "int", ["pointer", "pointer", "pointer", "int"])
CreateWindowExA = define_function("user32.dll", "CreateWindowExA", "pointer", ["int", "pointer", "pointer", "int", "int", "int", "int", "int", "pointer", "pointer", "pointer", "pointer"])
ShowWindow = define_function("user32.dll", "ShowWindow", "int", ["pointer", "int"])
UpdateWindow = define_function("user32.dll", "UpdateWindow", "int", ["pointer"])
DestroyWindow = define_function("user32.dll", "DestroyWindow", "int", ["pointer"])
GetModuleHandleA = define_function("kernel32.dll", "GetModuleHandleA", "pointer", ["pointer"])
MoveWindow = define_function("user32.dll", "MoveWindow", "int", ["pointer", "int", "int", "int", "int", "int"])
SetWindowTextA = define_function("user32.dll", "SetWindowTextA", "int", ["pointer", "pointer"])
IsWindowVisible = define_function("user32.dll", "IsWindowVisible", "int", ["pointer"])
GetSystemMetrics = define_function("user32.dll", "GetSystemMetrics", "int", ["int"])
MessageBeep = define_function("user32.dll", "MessageBeep", "int", ["int"])
Sleep = define_function("kernel32.dll", "Sleep", "void", ["int"])
PeekMessageA = define_function("user32.dll", "PeekMessageA", "int", ["pointer", "pointer", "int", "int", "int"])
TranslateMessage = define_function("user32.dll", "TranslateMessage", "int", ["pointer"])
DispatchMessageA = define_function("user32.dll", "DispatchMessageA", "int", ["pointer"])

# Window "class" using dictionary pattern (since module classes aren't callable)
def create_window(title, width, height):
    window = {}
    window["title"] = title
    window["width"] = width
    window["height"] = height
    window["hwnd"] = None
    window["is_destroyed"] = False

    h_instance = GetModuleHandleA(None)
    style = WS_OVERLAPPEDWINDOW | WS_VISIBLE

    hwnd = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", title, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, None, None, h_instance, None)

    if hwnd == None:
        hwnd = CreateWindowExA(0, "BUTTON", title, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, None, None, h_instance, None)

    if hwnd == None:
        hwnd = CreateWindowExA(0, "STATIC", title, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, None, None, h_instance, None)

    if hwnd == None:
        print("ERROR: Failed to create window")
        window["is_destroyed"] = True
        return window

    window["hwnd"] = hwnd
    return window

def keep_alive(window, seconds):
    if not window["hwnd"] or window["is_destroyed"]:
        return False

    total_ms = int(seconds * 1000)
    interval_ms = 50
    elapsed_ms = 0

    while elapsed_ms < total_ms:
        Sleep(interval_ms)
        elapsed_ms = elapsed_ms + interval_ms

    return True

def show_window(window):
    if window["hwnd"] and not window["is_destroyed"]:
        ShowWindow(window["hwnd"], SW_SHOW)
        UpdateWindow(window["hwnd"])
        return True
    return False

def hide_window(window):
    if window["hwnd"] and not window["is_destroyed"]:
        ShowWindow(window["hwnd"], SW_HIDE)
        return True
    return False

def set_window_title(window, new_title):
    if window["hwnd"] and not window["is_destroyed"]:
        window["title"] = new_title
        return SetWindowTextA(window["hwnd"], new_title) != 0
    return False

def move_window(window, x, y, w, h):
    if window["hwnd"] and not window["is_destroyed"]:
        result = MoveWindow(window["hwnd"], x, y, w, h, 1)
        return result != 0
    return False

def destroy_window(window):
    if window["hwnd"] and not window["is_destroyed"]:
        DestroyWindow(window["hwnd"])
        window["is_destroyed"] = True
    return True

# Helper functions
def message_box(text, title, style):
    return MessageBoxA(None, text, title, style)

def show_info(text, title):
    return message_box(text, title, MB_OK | MB_ICONINFORMATION)

def show_warning(text, title):
    return message_box(text, title, MB_OK | MB_ICONWARNING)

def show_error(text, title):
    return message_box(text, title, MB_OK | MB_ICONERROR)

def get_screen_width():
    return GetSystemMetrics(SM_CXSCREEN)

def get_screen_height():
    return GetSystemMetrics(SM_CYSCREEN)

def beep():
    return MessageBeep(0)
