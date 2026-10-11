# std.image — Raster image decode/encode

```tauraro
from std.image import Image, decode, decode_bmp, encode_bmp,
                       decode_qoi, encode_qoi, decode_png, encode_png,
                       decode_jpeg, encode_jpeg
```

One shared in-memory representation (`Image`, RGBA8 — 4 bytes/pixel,
row-major, top row first, no row padding) that every codec decodes INTO
and encodes FROM, so converting between formats is just
`decode_bmp(bytes, len).encode_png()`.

Every codec treats `str` as a raw byte buffer with an explicit,
separately-tracked length — never via C `strlen` — since image file bytes
routinely contain embedded NUL bytes (black pixels, zero-valued header
fields, etc.).

---

## Image

| Member | Type | Description |
|---|---|---|
| `width` | `int` | Width in pixels. |
| `height` | `int` | Height in pixels. |
| `pixels` | `Vec[u8]` | `width * height * 4` bytes, RGBA order, row-major, top row first. |

| Method | Signature | Description |
|---|---|---|
| `Image.init` | `(w: int, h: int) -> Image` | A new image, all pixels `(0,0,0,0)`. |
| `.get_pixel` | `(x: int, y: int) -> (int, int, int, int)` | Read the `(r, g, b, a)` at `(x, y)`, each `0..255`. |
| `.set_pixel` | `(x: int, y: int, r: int, g: int, b: int, a: int)` | Write the pixel at `(x, y)` (each component masked to a byte). |
| `.free` | `(self)` | Free the pixel buffer. |

Every decoder returns an `Image` with `width == -1` (and `height == -1`)
on malformed/unsupported input, rather than raising — check `.width < 0`
to detect failure.

---

## std.image.bmp — BMP

Uncompressed Windows BMP (`BITMAPFILEHEADER` + 40-byte `BITMAPINFOHEADER`),
24-bit (BGR) and 32-bit (BGRA) variants, both bottom-up and top-down row
order. `encode_bmp` always writes 32-bit BGRA, top-down. RLE/BITFIELDS
with non-standard masks and JPEG/PNG-in-BMP are not supported.

| Function | Signature | Description |
|---|---|---|
| `decode_bmp` | `(data: str, len_: int) -> Image` | Decode a BMP buffer. |
| `encode_bmp` | `(img: Image) -> (str, int)` | Encode as 32-bit BGRA BMP bytes; returns `(bytes, length)`. |

---

## std.image.qoi — QOI ("Quite OK Image")

Full implementation of the [QOI specification](https://qoiformat.org/qoi-specification.pdf):
`QOI_OP_RGB`/`QOI_OP_RGBA` literals, `QOI_OP_INDEX` (64-entry running color
cache), `QOI_OP_DIFF`/`QOI_OP_LUMA` (small-delta encodings), and
`QOI_OP_RUN` (run-length for repeated pixels). No external dependency —
QOI's byte-oriented scheme is implemented directly.

| Function | Signature | Description |
|---|---|---|
| `decode_qoi` | `(data: str, len_: int) -> Image` | Decode a QOI buffer. |
| `encode_qoi` | `(img: Image) -> (str, int)` | Encode as QOI bytes; returns `(bytes, length)`. |

---

## std.image.png — PNG

Truecolor (`color type 2`/RGB and `6`/RGBA) and grayscale (`color type 0`)
PNG, 8 bits/channel, non-interlaced. Chunk parsing (`IHDR`/`IDAT`/`IEND`,
with CRC32 verification via the same polynomial `std.io.archive.Crc32`
uses) and the 5 PNG scanline filters (None/Sub/Up/Average/Paeth) are
implemented directly. Palette images (`color type 3`) and interlaced PNGs
are not supported — `decode_png` reports failure rather than guessing.

**DEFLATE/zlib is fully self-contained, not opt-in.** `std.compress.zlib`'s
`_tr_deflate`/`_tr_inflate`/`_tr_zlib_*` runtime primitives are stubs
unless the whole runtime is rebuilt with `-DTAURARO_COMPRESS_ZLIB -lz` —
a flag `tauraroc`'s own build pipeline never sets for ordinary compiled
programs, so depending on them would silently produce an empty/corrupt
`IDAT` chunk on every normal build. Instead, `std.image.png` implements
its own:

- **Decode**: a complete RFC 1951 DEFLATE inflater (stored, fixed-Huffman,
  AND dynamic-Huffman blocks) plus the RFC 1950 zlib wrapper (header +
  Adler-32 trailer) — so it reads PNGs from any real encoder (libpng,
  browsers, Python's `zlib`, etc.), not only ones this module wrote.
- **Encode**: DEFLATE "stored" blocks (RFC 1951 §3.2.4, verbatim bytes, no
  entropy coding) wrapped in a real zlib container. Larger than optimally
  compressed PNGs, but 100% spec-valid and decodes correctly in any real
  PNG viewer.

| Function | Signature | Description |
|---|---|---|
| `decode_png` | `(data: str, len_: int) -> Image` | Decode a PNG buffer. |
| `encode_png` | `(img: Image) -> (str, int)` | Encode as truecolor+alpha (color type 6) PNG bytes; returns `(bytes, length)`. |

---

## std.image.jpeg — JPEG

```tauraro
from std.image.jpeg import decode_jpeg, encode_jpeg, encode_jpeg_ex, jpeg_info
```

Pure Tauraro (no libjpeg), JFIF/Adobe files.

**Decoder** (`decode_jpeg`)

- SOF0 baseline, SOF1 extended-sequential (8-bit) and **SOF2 progressive**
  (DC/AC first and refinement scans, end-of-band runs, multiple scans).
- 1 (grayscale), 3 (YCbCr; RGB when the Adobe flag / `R,G,B` component ids say so)
  and 4 (CMYK / Adobe YCCK, converted to RGB) components, 8 bits per sample.
- Any sampling factors (4:4:4, 4:2:2, 4:2:0, 4:4:0, 4:1:1, ...), restart
  intervals (`DRI` / `RSTn`), non-interleaved scans.
- The IDCT is the IJG "islow" integer algorithm and the 2:1 chroma upsampling is
  libjpeg's "fancy" triangle filter, so for ordinary files the output is
  **bit-identical to libjpeg(-turbo)'s default decode**. This was checked
  against Pillow/libjpeg on baseline and progressive files at 4:4:4, 4:2:2 and
  4:2:0, qualities 10-100, grayscale, CMYK, odd sizes (1x1 ... 131x97) and an
  874x841 progressive photograph: 0 differing samples.
- Truncated entropy data decodes as far as it goes (the rest stays mid-grey).
  Arithmetic coding (SOF9+), lossless (SOF3), hierarchical and 12-bit JPEG are
  **not** supported: `decode_jpeg` returns an `Image` with `width == -1`.
  Images above 64 megapixels are rejected. ICC profiles and EXIF orientation
  are ignored (no automatic rotation).
- ~0.15 s for the 874x841 progressive photograph (-O2 C output).

**Encoder** (`encode_jpeg`, `encode_jpeg_ex`) — baseline sequential, standard
Annex-K Huffman tables, IJG quality scaling.

| Function | Signature | Description |
|---|---|---|
| `decode_jpeg` | `(data: str, len_: int) -> Image` | Decode to RGBA8 (alpha 255). `width == -1` on failure. |
| `jpeg_info` | `(data: str, len_: int) -> JpegInfo` | Header only: `ok`, `width`, `height`, `components`, `progressive`, `adobe` (APP14 transform, -1 if absent). |
| `encode_jpeg` | `(img: Image, quality: int) -> (str, int)` | 4:2:0 baseline JPEG, `quality` 1..100. Returns `(bytes, length)`. |
| `encode_jpeg_ex` | `(img, quality, subsampling, gray, restart) -> (str, int)` | `subsampling` 0 = 4:4:4, 1 = 4:2:2, 2 = 4:2:0; `gray` writes a 1-component file; `restart` = restart interval in MCUs (0 = none). |

Alpha is composited over white (JPEG has no alpha channel). The encoder's
output was validated by decoding it with Pillow/libjpeg (including restart
intervals and grayscale); at equal quality its PSNR matches libjpeg's within
~0.1 dB. It does not do progressive output or optimised Huffman tables.

---

## std.image.gif — GIF

```tauraro
from std.image.gif import decode_gif, decode_gif_anim, encode_gif, encode_gif_ex, GifWriter, GifAnim
```

Pure Tauraro, GIF87a and GIF89a.

**Decoder** — LZW (2..12-bit codes, clear/EOI, deferred clear), interlaced and
sequential frames, global and local palettes, Graphic Control Extensions
(delay, disposal 0/1/2/3, transparent index), Netscape loop count. Comment,
plain-text and unknown extensions are skipped. Every frame is **composited onto
the logical-screen canvas**, so `anim.frame(i)` is what a viewer shows at step
`i`. The canvas starts fully transparent and the background colour index is not
painted (as browsers do). A file whose later frames are damaged yields the
frames decoded so far. Compositing was compared with Pillow frame by frame
(disposal 0-3, local palettes, interlace, transparency, noise that forces LZW
table resets, and a 184-frame real-world animation): 0 differing pixels.

| Function / method | Description |
|---|---|
| `decode_gif(data, len_) -> Image` | First frame as RGBA8; `width == -1` on failure. |
| `decode_gif_anim(data, len_) -> GifAnim` | `ok`, `width`, `height`, `frame_count`, `loop_count` (0 = forever, N = N plays, -1 = no loop extension). |
| `anim.frame(i) -> Image` | Owned copy of composited frame `i`. |
| `anim.delay_cs(i)`, `anim.disposal(i)`, `anim.frame_rect(i) -> (x, y, w, h)`, `anim.transparent_index(i)`, `anim.has_local_palette(i)` | Per-frame metadata. |
| `anim.free()` | Release the animation (call it when done). |

**Encoder** — exact palette when the image has at most 256 colours, otherwise
median-cut quantisation (5 bits/channel histogram) with optional Floyd-Steinberg
dithering. `alpha < 128` becomes the transparent index, `alpha >= 128` is opaque.
LZW uses a hash dictionary and proper clear codes.

| Function / method | Description |
|---|---|
| `encode_gif(img) -> (str, int)` | Single-frame GIF89a. |
| `encode_gif_ex(img, dither: bool) -> (str, int)` | Same, optional dithering. |
| `GifWriter.init(w, h, loop_count)` | Animation writer (`loop_count` 0 = forever, -1 = no loop extension). |
| `w.add_frame(img, delay_cs)` / `w.add_frame_ex(img, delay_cs, disposal)` | Append a frame (same size as the canvas). |
| `w.set_dither(bool)`, `w.set_optimize(bool)` | Optimise stores only the changed rectangle of each frame, using transparency. |
| `w.finish() -> (str, int)` | Write the trailer and return the bytes. |

Limits: every frame is quantised to its own 256-colour local palette (there is
no global palette optimisation), frames must be as large as the canvas, and
encoding is slower than decoding (median cut + hash LZW). Results of the
encoder were verified by decoding with this decoder and with Pillow/libgif.

---

## std.image.transform — resize, crop, rotate, filters, compositing

```tauraro
from std.image.transform import resize_image, crop, flip_h, rotate90, rotate, gaussian_blur,
                                sharpen, grayscale, composite, FILTER_LANCZOS
```

Pure Tauraro, operating on the shared RGBA8 `Image`. Functions return a **new**
owned image (the argument is untouched) except the in-place drawing helpers
(`composite`, `composite_ex`, `paste`, `fill_rect`). Invalid sizes give an `Image`
with `width == -1`. (`resize_image` is not called `resize` because the compiler
treats a function named `resize` as the realloc intrinsic — bug M1-3.)

| Group | Functions |
|---|---|
| Resize | `resize_image(img, w, h, filter)` with `FILTER_NEAREST` / `FILTER_BILINEAR` / `FILTER_BICUBIC` / `FILTER_LANCZOS`; `resize_nearest/bilinear/bicubic/lanczos`; `resize_fit(img, max_w, max_h, filter, shrink_only)` |
| Geometry | `crop(img, x, y, w, h)` (out-of-range area transparent), `flip_h`, `flip_v`, `rotate90(img, quarter_turns)` (clockwise), `rotate180`, `transpose`, `rotate(img, degrees, expand, smooth)`, `copy_image` |
| Convolution | `Kernel.init(n)` + `set/get`, `divisor`, `offset`; `convolve(img, kernel)`; `box_blur(img, radius)`, `gaussian_blur(img, sigma)`, `sharpen(img, amount)`, `edge_detect(img)` (Laplacian), `sobel(img)`, `emboss(img)` |
| Colour | `grayscale`, `invert`, `brightness(img, add)`, `contrast(img, factor)`, `gamma(img, g)`, `saturation(img, factor)`, `threshold(img, t)`, `sepia`, `with_opacity(img, 0..1)` |
| Compositing | `composite(dst, src, x, y)` (Porter-Duff *over*, straight alpha, clipped), `composite_ex(..., opacity 0..255)`, `paste` (no blend), `fill_rect`, `flatten(img, r, g, b)` |

**Accuracy.** Resizing uses Pillow's separable algorithm (the filter support
grows with the down-scale factor, so shrinking is anti-aliased). Compared with
Pillow on opaque images nearest, bicubic and lanczos are byte-identical and
bilinear is within 1 level; images with alpha are resampled premultiplied and
agree within a few levels. `rotate` matches Pillow's canvas size for `expand`
and its pixels for nearest sampling (a handful of exact-tie pixels may differ).
Convolutions clamp at the borders (Pillow leaves a 1-pixel border unfiltered);
alpha passes through unchanged. `gaussian_blur` is a true Gaussian (Pillow
approximates one with box blurs), `contrast` pivots at 128 (Pillow pivots at the
mean luma). Everything is per-pixel integer/float code with no SIMD: a 1000x1000
Lanczos resize takes a few hundred ms.

---

## decode() — format sniffing

```tauraro
pub def decode(data: str, len_: int) -> Image
```

Inspects the magic bytes of `data` (`"BM"` for BMP, `"qoif"` for QOI, the
8-byte PNG signature for PNG, `FF D8 FF` for JPEG, `"GIF8"` for GIF — first frame) and dispatches to the matching decoder.
Returns an `Image` with `width == -1` for unrecognized or too-short input.

---

## Example

```tauraro
from std.image import Image, decode_bmp, encode_png

mut img = Image.init(2, 2)
img.set_pixel(0, 0, 255, 0, 0, 255)
img.set_pixel(1, 0, 0, 255, 0, 255)
img.set_pixel(0, 1, 0, 0, 255, 255)
img.set_pixel(1, 1, 255, 255, 0, 128)

# Convert BMP bytes to PNG bytes:
mut bmp_bytes, bmp_len = encode_bmp(img)
mut from_bmp = decode_bmp(bmp_bytes, bmp_len)
mut png_bytes, png_len = encode_png(from_bmp)
```
