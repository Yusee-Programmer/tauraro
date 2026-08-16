# Graphics demo — the full bindgen → graphics pipeline

A complete, runnable proof that Tauraro can build graphics applications through
**auto-generated FFI bindings** (Phase 2). `raygfx` is a small, raylib-shaped 2D library that
renders to a software framebuffer (so the result is verifiable headlessly — no display needed).
Its API uses exactly the shapes a real graphics library uses: **by-value `Vector2`/`Color`/`Rect`
structs** as arguments *and* return values, an integer color-packing function, and constants.

## Files
- `raygfx.h` / `raygfx.c` — the mini graphics library (raylib-shaped).
- `game.tr` — a Tauraro program that draws a scene and reads pixels back.

## Run it

```sh
# 1. build the library
cc -c raygfx.c -o raygfx.o

# 2. generate Tauraro bindings from the header (the whole point)
tauraroc ../../bindgen.tr -o tauraro-bindgen
./tauraro-bindgen raygfx.h -o raygfx.tr

# 3. compile the Tauraro program and link it with the library
tauraroc game.tr --emit c
cc build/main.c raygfx.o -Ibuild -o game        # (+ -lws2_32 on Windows)

# 4. run — draws the scene, verifies pixels, and writes scene.ppm
./game
```

`game.tr` never touches C directly — it just `from raygfx import Vector2, Color, Rect,
DrawRectangleRec, DrawLineV, GetImageColor, …` and calls them. The generated `raygfx.tr` maps
`typedef struct { float x, y; } Vector2;` → `@value_type class Vector2`, and the by-value struct
arguments/returns cross the FFI correctly (verified: a red rectangle read back as `230,41,55`).

## Output

The program prints the pixels it reads back (proving the draw calls worked) and saves
`scene.ppm`, which contains a red rectangle, a green rectangle, and a yellow diagonal line over a
blue background — i.e. a real rendered scene.

## A real window (`raywin.*` + `game_win.tr`)

`raywin` is the same idea backed by a **real Win32 window** (Windows only). `game_win.tr` opens a
320×240 window and runs a raylib-style game loop that animates a moving box over a scene:

```sh
cc -c raywin.c -o raywin.o
./tauraro-bindgen raywin.h -o raywin.tr
tauraroc game_win.tr --emit c
cc build/main.c raywin.o -Ibuild -o game_win -lgdi32 -luser32 -lws2_32
./game_win            # a real window appears; close it or press ESC to exit
```

The game loop is pure Tauraro: `while GfxShouldClose() == 0: GfxBegin(); GfxClear(...);
GfxDrawRect(...); GfxDrawLine(...); GfxEnd()`.

## Collisions

`raygfx`/`raywin` use the struct name `Rect` because their C **implementation** includes
`<windows.h>` for GDI, whose `Rectangle` function would clash. The bindgen itself handles this
automatically for the **bindings**: a type whose name collides with a runtime/Win32 symbol (e.g.
`Rectangle`) is renamed with a trailing `_` (`class Rectangle_:`) consistently across the output
— the struct layout (ABI) is unchanged, so it still passes correctly to the C function. A
**function** whose name the runtime already declares (`printf`, `CloseWindow`) is skipped. So the
same three steps work on a real library — replace the header with `raylib.h` / `SDL.h` and link
`-lraylib` / `-lSDL2`.
