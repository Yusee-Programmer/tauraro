#include <windows.h>

// Simple window structure
typedef struct {
    HWND hwnd;
    int width;
    int height;
    char title[256];
} Window;

// Global window instance
static Window g_window = {0};

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Fill the window with white color
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
            
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Create a window
__declspec(dllexport) Window* create_window(const char* title, int width, int height) {
    // Register the window class
    const char CLASS_NAME[] = "Tauraro Window Class";
    
    WNDCLASS wc = {0};
    
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClass(&wc);
    
    // Create the window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );
    
    if (hwnd == NULL) {
        return NULL;
    }
    
    // Store window info
    g_window.hwnd = hwnd;
    g_window.width = width;
    g_window.height = height;
    strncpy(g_window.title, title, sizeof(g_window.title) - 1);
    
    return &g_window;
}

// Show the window
__declspec(dllexport) void show_window(Window* window) {
    if (window && window->hwnd) {
        ShowWindow(window->hwnd, SW_SHOW);
        UpdateWindow(window->hwnd);
    }
}

// Run the message loop
__declspec(dllexport) int run_message_loop() {
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

// Set window title
__declspec(dllexport) void set_window_title(Window* window, const char* title) {
    if (window && window->hwnd && title) {
        SetWindowText(window->hwnd, title);
        strncpy(window->title, title, sizeof(window->title) - 1);
    }
}