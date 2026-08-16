#ifndef RAYWIN_H
#define RAYWIN_H
/* A raylib-shaped mini 2D library backed by a REAL Win32 window (Windows only). */
#define RAYWIN_VERSION 200
typedef struct Vector2   { float x, y; } Vector2;
typedef struct Color     { unsigned char r, g, b, a; } Color;
typedef struct Rect { float x, y, width, height; } Rect;   /* collides with Win32 GDI -> auto-renamed by bindgen */

void GfxInit(int width, int height, const char* title);  /* open a real window */
int  GfxShouldClose(void);                                /* 1 when the window is closed / ESC */
void GfxBegin(void);
void GfxEnd(void);                                        /* present the frame + pump messages */
void GfxClose(void);
void GfxClear(Color color);
void GfxDrawRect(Rect rec, Color color);             /* by-value Rect + Color */
void GfxDrawLine(Vector2 a, Vector2 b, Color color);      /* two by-value Vector2 */
Color GfxGetPixel(int x, int y);                          /* by-value Color return */
#endif
