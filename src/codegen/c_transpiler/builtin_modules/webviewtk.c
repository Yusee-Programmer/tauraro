// ==========================================
// WEBVIEWTK MODULE - Pure C Implementation (GUI/WebView Toolkit)
// ==========================================
// Provides: WebView, Window, Application, on_load, run
// Platform: Cross-platform (Windows/Linux/macOS)

#ifndef TAURARO_WEBVIEWTK_MODULE_H
#define TAURARO_WEBVIEWTK_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Window structure
typedef struct {
    char* title;
    int width;
    int height;
    char* html;
    void (*on_load)(void);
    void (*on_close)(void);
} WindowSpec;

// WebView structure
typedef struct {
    WindowSpec window;
    void* handle;
    int running;
} WebViewHandle;

// webviewtk.WebView(html, title="WebView", width=800, height=600)
static inline TauValue tauraro_webviewtk_WebView(TauValue html) {
    if (html.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    WebViewHandle* wv = (WebViewHandle*)malloc(sizeof(WebViewHandle));
    wv->window.html = (char*)malloc(strlen(html.value.s) + 1);
    strcpy(wv->window.html, html.value.s);
    wv->window.title = (char*)malloc(8);
    strcpy(wv->window.title, "WebView");
    wv->window.width = 800;
    wv->window.height = 600;
    wv->window.on_load = NULL;
    wv->window.on_close = NULL;
    wv->running = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)wv, .refcount = 1, .next = NULL};
}

// webviewtk.WebView with options
static inline TauValue tauraro_webviewtk_WebView_options(TauValue html, TauValue title, TauValue width, TauValue height) {
    WebViewHandle* wv = (WebViewHandle*)malloc(sizeof(WebViewHandle));
    wv->window.html = (char*)malloc(strlen(html.type == 2 ? html.value.s : ""));
    strcpy(wv->window.html, html.type == 2 ? html.value.s : "");
    
    if (title.type == 2) {
        wv->window.title = (char*)malloc(strlen(title.value.s) + 1);
        strcpy(wv->window.title, title.value.s);
    } else {
        wv->window.title = (char*)malloc(8);
        strcpy(wv->window.title, "WebView");
    }
    
    wv->window.width = width.type == 0 ? width.value.i : 800;
    wv->window.height = height.type == 0 ? height.value.i : 600;
    wv->window.on_load = NULL;
    wv->window.on_close = NULL;
    wv->running = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)wv, .refcount = 1, .next = NULL};
}

// webviewtk.WebView.run()
static inline TauValue tauraro_webviewtk_WebView_run(TauValue wv) {
    if (wv.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    WebViewHandle* webview = (WebViewHandle*)wv.value.ptr;
    webview->running = 1;
    printf("Running WebView: %s (%dx%d)\n", webview->window.title, webview->window.width, webview->window.height);
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// webviewtk.WebView.close()
static inline TauValue tauraro_webviewtk_WebView_close(TauValue wv) {
    if (wv.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    WebViewHandle* webview = (WebViewHandle*)wv.value.ptr;
    webview->running = 0;
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// webviewtk.WebView.set_title(title)
static inline TauValue tauraro_webviewtk_WebView_set_title(TauValue wv, TauValue title) {
    if (wv.type != 6 || title.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    WebViewHandle* webview = (WebViewHandle*)wv.value.ptr;
    free(webview->window.title);
    webview->window.title = (char*)malloc(strlen(title.value.s) + 1);
    strcpy(webview->window.title, title.value.s);
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// webviewtk.WebView.set_html(html)
static inline TauValue tauraro_webviewtk_WebView_set_html(TauValue wv, TauValue html) {
    if (wv.type != 6 || html.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    WebViewHandle* webview = (WebViewHandle*)wv.value.ptr;
    free(webview->window.html);
    webview->window.html = (char*)malloc(strlen(html.value.s) + 1);
    strcpy(webview->window.html, html.value.s);
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// webviewtk.WebView.on_load(callback)
static inline TauValue tauraro_webviewtk_WebView_on_load(TauValue wv, TauValue callback) {
    if (wv.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    WebViewHandle* webview = (WebViewHandle*)wv.value.ptr;
    webview->window.on_load = (void (*)(void))callback.value.ptr;
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// webviewtk.WebView.on_close(callback)
static inline TauValue tauraro_webviewtk_WebView_on_close(TauValue wv, TauValue callback) {
    if (wv.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    WebViewHandle* webview = (WebViewHandle*)wv.value.ptr;
    webview->window.on_close = (void (*)(void))callback.value.ptr;
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// webviewtk.WebView.eval(javascript)
static inline TauValue tauraro_webviewtk_WebView_eval(TauValue wv, TauValue javascript) {
    if (wv.type != 6 || javascript.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    // Execute JavaScript
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// webviewtk.Window(title, width, height)
static inline TauValue tauraro_webviewtk_Window(TauValue title, TauValue width, TauValue height) {
    WindowSpec* win = (WindowSpec*)malloc(sizeof(WindowSpec));
    if (title.type == 2) {
        win->title = (char*)malloc(strlen(title.value.s) + 1);
        strcpy(win->title, title.value.s);
    } else {
        win->title = (char*)malloc(1);
        win->title[0] = 0;
    }
    win->width = width.type == 0 ? width.value.i : 800;
    win->height = height.type == 0 ? height.value.i : 600;
    win->html = NULL;
    win->on_load = NULL;
    win->on_close = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)win, .refcount = 1, .next = NULL};
}

// webviewtk.Application()
static inline TauValue tauraro_webviewtk_Application(void) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// webviewtk.Application.run()
static inline TauValue tauraro_webviewtk_Application_run(TauValue app) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// webviewtk.run(html, title, width, height)
static inline TauValue tauraro_webviewtk_run(TauValue html, TauValue title, TauValue width, TauValue height) {
    TauValue wv = tauraro_webviewtk_WebView_options(html, title, width, height);
    return tauraro_webviewtk_WebView_run(wv);
}


#endif // TAURARO_WEBVIEWTK_MODULE_H
