#ifndef RAYGFX_H
#define RAYGFX_H
/* A raylib-shaped mini 2D library rendering to a software framebuffer (headless-verifiable). */
#define RAYGFX_VERSION 100

typedef struct Vector2   { float x, y; } Vector2;
typedef struct Color     { unsigned char r, g, b, a; } Color;
typedef struct Rect { float x, y, width, height; } Rect;

void InitCanvas(int width, int height);
void CloseCanvas(void);
int  GetCanvasWidth(void);
void ClearBackground(Color color);                          /* by-value Color arg          */
void DrawRectRec(Rect rec, Color color);          /* by-value Rect + Color  */
void DrawLineV(Vector2 startPos, Vector2 endPos, Color color); /* two by-value Vector2      */
Color GetImageColor(int x, int y);                          /* by-value Color RETURN       */
unsigned int ColorToInt(Color color);                       /* by-value arg -> int         */
int  SavePPM(const char* path);                             /* dump framebuffer to verify  */
#endif
