# std.image — Raster image decode/encode (BMP, QOI, PNG, JPEG, GIF, WebP, TIFF) and QR codes

```tauraro
from std.image import Image, decode, decode_bmp, encode_bmp,
                       decode_qoi, encode_qoi, decode_png, encode_png
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

## decode() — format sniffing

```tauraro
pub def decode(data: str, len_: int) -> Image
```

Inspects the magic bytes of `data` (`"BM"` for BMP, `"qoif"` for QOI, the
8-byte PNG signature for PNG) and dispatches to the matching decoder.
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

---

## JPEG — `std.image.jpeg`

`decode_jpeg(data, len) -> Image` (width -1 on failure) / `decode_jpeg_checked(data, len)? -> Image` read baseline and
progressive JPEGs (any subsampling, restart markers, grayscale/YCbCr/RGB/CMYK/YCCK); output is within a few levels of
libjpeg. Lossless, arithmetic-coded and 12-bit files are rejected. `jpeg_orientation(data, len)` returns the EXIF
orientation (1-8); apply it with `std.image.ops.apply_orientation`.
`encode_jpeg(img, quality) -> (str, int)` writes baseline JFIF (4:2:0 below quality 90, 4:4:4 from 90);
`encode_jpeg_opts(img, quality, subsample, gray)` chooses explicitly. Transparency is composited onto white.

## GIF — `std.image.gif`

`decode_gif(data, len)` (first frame), `decode_gif_animation(data, len)? -> GifAnimation` (`frames`, `delays` in ms,
`loop_count`, `width`, `height`; frames are full composited canvases). `encode_gif(img, dither) -> (str, int)`;
`GifEncoder.init(w, h, loop_count)` → `add_frame(img, delay_ms, dither)?` → `finish() -> (str, int)`. Images with
≤ 256 colours keep them exactly; otherwise median-cut quantisation with optional Floyd-Steinberg dithering. Alpha < 128
becomes transparent.

## WebP — `std.image.webp`

```tauraro
from std.image.webp import decode_webp, decode_webp_checked, decode_webp_animation, webp_info, encode_webp, WebpEncoder
```

`decode_webp(data, len) -> Image` (width -1 on failure) and `decode_webp_checked(data, len)? -> Image` read the first
frame; `decode_webp_animation(data, len)? -> WebpAnimation` (`frames`, `delays` in ms, `loop_count`, `width`,
`height`) returns every frame composited onto the canvas exactly as libwebp's `WebPAnimDecoder` does (blend and dispose
methods, sub-rectangle frames). `webp_info(data, len)? -> WebpInfo` gives `width`, `height`, `frames`, `has_alpha`,
`lossless`, `animated`.

Supported: simple and extended (VP8X) RIFF files; lossy VP8 key frames (segments, simple and normal loop filters, all
intra modes, multiple token partitions) with the ALPH alpha plane (raw or lossless-compressed, all four prediction
filters); lossless VP8L (all transforms, colour cache, meta prefix codes); ANIM/ANMF animation. Lossy output is
**bit-identical to libwebp** (and so to Pillow): same inverse transforms, loop filters, "fancy" chroma upsampling and
fixed-point YUV→RGB. Not supported: VP8 inter frames (never present in WebP), ICC/EXIF/XMP metadata (skipped).

`encode_webp(img) -> (str, int)` writes **lossless** VP8L: images with ≤ 256 colours use a palette with pixel
bundling; others use subtract-green plus a per-16×16-block predictor transform (14 modes, best by residual cost), then
LZ77 (hash chain, backward references with the 2-D distance codes) and length-limited Huffman codes. The decoded result
equals the input exactly. Lossy encoding is not provided. `WebpEncoder.init(w, h, loop_count)` → `add_frame(img,
duration_ms)?` → `finish() -> (str, int)` writes an animated file (each frame covers the canvas, no blending).
`std.image.decode` recognises `RIFF....WEBP`.

## TIFF — `std.image.tiff`

```tauraro
from std.image.tiff import decode_tiff, decode_tiff_page, decode_tiff_checked, tiff_page_count, tiff_info, encode_tiff, TiffEncoder, TIFF_NONE, TIFF_LZW
```

`decode_tiff(data, len) -> Image` reads page 0 (width -1 on failure); `decode_tiff_page(data, len, page)? -> Image`
reads any page, `tiff_page_count(data, len) -> int` counts them, `tiff_info(data, len, page)? -> TiffInfo` reports
`width`, `height`, `pages`, `bits_per_sample`, `samples_per_pixel`, `photometric`, `compression`.

Supported: classic little/big-endian TIFF; strips and tiles; chunky and planar layouts; no compression, PackBits, LZW
(TIFF 6.0 MSB-first with early change), Deflate (Adobe 8 and legacy 32946, own inflate); horizontal predictor (2) at
8 and 16 bits; 1/2/4/8/16-bit samples (16-bit reduced to 8 by taking the high byte); WhiteIsZero, BlackIsZero, RGB,
palette, 8-bit CMYK; extra samples as associated (premultiplied is undone) or unassociated alpha. Not supported:
BigTIFF, JPEG/CCITT compression, floating-point samples, mixed bit depths, old-style LZW, Lab/YCbCr photometrics,
orientation tag handling.

`encode_tiff(img, compression) -> (str, int)` writes 8-bit RGB (RGBA if any pixel is not opaque), strips of about
8 KiB, `TIFF_NONE` or `TIFF_LZW` (with the horizontal predictor). `TiffEncoder.init()` → `add_page(img,
compression)?` → `finish()` writes multi-page files. `std.image.decode` recognises `II*\0` / `MM\0*`.

## QR codes — `std.image.qr`

```tauraro
from std.image.qr import QrCode, QR_ECC_L, QR_ECC_M, QR_ECC_Q, QR_ECC_H
mut qr = QrCode.encode("https://tauraro.dev", QR_ECC_M)?
print(qr.to_text(2))                 # half-block art for the terminal
mut img = qr.to_image(8, 4)          # 8 px per module, 4-module quiet zone
mut svg = qr.to_svg(4)
```

ISO/IEC 18004 encoder: versions 1–40, levels L/M/Q/H, **automatic segmentation** into numeric / alphanumeric / byte
segments by a dynamic program that minimises the total bit length (so `"ABC1234567890"` switches modes inside the
message), smallest version that fits, error-correction **boosting** (the level is raised for free while the version
does not grow), mask selection by the four-rule penalty score, Reed–Solomon over GF(256). Text is UTF-8 bytes; Kanji
and ECI segments are not generated. Capacity: 7089 digits, 4296 alphanumerics, 2953 bytes (version 40-L).

| Member | Description |
|---|---|
| `QrCode.encode(text, ecc)?` | Encode UTF-8 text. `encode_bytes(data, len, ecc)?` is binary-safe. |
| `QrCode.encode_opts(data, len, ecc, min_version, max_version, mask, boost, force_mode)?` | Full control (`mask` -1 = best; `force_mode` `QR_MODE_AUTO/NUMERIC/ALNUM/BYTE`). |
| `size`, `version`, `ecc`, `mask`, `modules` | Result: `size = 17 + 4*version`; `modules` is `size*size` bytes, 1 = dark. |
| `get(x, y) -> bool` | Dark module test; false outside the symbol. |
| `to_image(scale, border)`, `to_image_colors(scale, border, fg, bg)` | RGBA `Image` (colours are `std.color.Color`). |
| `to_svg(border)`, `to_svg_colors(border, fg, bg)` | SVG document with one `<path>` (run-length rectangles). |
| `to_text(border)`, `to_text_opts(border, invert)` | UTF-8 half blocks, two rows per line; light modules are the filled cells (right for dark terminals), `invert` swaps. |

Verified by decoding with zxing-cpp for every version (1–40) at all four levels at its maximum capacity, plus mixed
data, UTF-8 and binary; the codewords of the ISO `01234567` example match the standard. Mask choice follows
the Nayuki-style penalty (a 1:1:3:1:1 pattern with light on both sides scores twice), so a few matrices differ from
other generators in mask only; all decode identically. Tested by `tests/lang/137_qr_color.tr`.

## Colours — `Color` interop

`Image.get_color(x, y)`, `set_color(x, y, c)`, `Image.filled(w, h, c)` and `fill_color(c)` take and return
[`std.color.Color`](color.md) values; the PDF writer has `set_fill_color_c(c)` / `set_stroke_color_c(c)`.

## Operations — `std.image.ops`

| Function | Description |
|---|---|
| `resize_image(img, w, h, filter)`, `thumbnail(img, max_w, max_h, filter)`, `scale(img, factor, filter)` | `FILTER_NEAREST`, `FILTER_BOX`, `FILTER_BILINEAR`, `FILTER_BICUBIC`, `FILTER_LANCZOS`; premultiplied alpha. (`resize` is a built-in name.) |
| `grayscale`, `invert`, `brightness(d)`, `contrast(f)`, `gamma(g)`, `saturation(f)`, `sepia`, `threshold(level)`, `opacity(f)`, `posterize(levels)` | Colour adjustments. |
| `gaussian_blur(sigma)`, `box_blur(radius)`, `unsharp_mask(sigma, amount, threshold)`, `sharpen`, `edges` (Sobel), `emboss`, `convolve(kernel, size, divisor, bias)?` | Filters. |
| `flip_horizontal`, `flip_vertical`, `rotate90/180/270` (clockwise), `transpose_image`, `crop(x, y, w, h)?`, `apply_orientation(img, exif)` | Geometry. |
| `solid(w, h, r, g, b, a)`, `fill(img, ...)`, `paste(base, top, x, y, opacity)`, `copy_image(img)` | Canvas helpers (`fill`/`paste` modify in place). |

`std.image.decode` also recognises JPEG, GIF, WebP and TIFF. Codecs and filters are C (`std/image/image_native.c`). Tested by
`tests/lang/114_image2.tr`; the codecs were cross-checked against Pillow on synthetic images and photos.

WebP and TIFF are C (`std/image/webp_native.c`, `std/image/tiff_native.c`); the WebP decoder carries the VP8 probability
and quantiser tables from libwebp (BSD-3-Clause, see `std/image/vendor/README.md`). Tested by
`tests/lang/136_webp_tiff.tr`; the decoders were compared byte-for-byte with Pillow on 56 WebP and 29 TIFF files
(every codec variant), the encoders' output decodes identically in Pillow, and both decoders were fuzzed
(corrupted/truncated inputs) under ASan/UBSan without findings.
