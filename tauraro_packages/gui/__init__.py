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

# Helper class for creating Windows
class Window:
    def __init__(self, title, width, height):
        self.title = title
        self.width = width
        self.height = height
        self.x = CW_USEDEFAULT
        self.y = CW_USEDEFAULT
        self.hwnd = None
        self.class_name = "TauraroWindowClass"

    def create(self):
        h_instance = GetModuleHandleA(None)
        style = WS_OVERLAPPEDWINDOW | WS_VISIBLE
        args = [0, "STATIC", self.title, style, self.x, self.y, self.width, self.height, None, None, h_instance, None]
        self.hwnd = CreateWindowExA(*args)
        if self.hwnd == None:
            print("Failed to create window")
        return self.hwnd

    def show(self):
        if self.hwnd:
            ShowWindow(self.hwnd, SW_SHOW)
            UpdateWindow(self.hwnd)

    def hide(self):
        if self.hwnd:
            ShowWindow(self.hwnd, SW_HIDE)

    def destroy(self):
        if self.hwnd:
            DestroyWindow(self.hwnd)
            self.hwnd = None

def message_box(text, title, style):
    return MessageBoxA(None, text, title, style)
