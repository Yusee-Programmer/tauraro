# std.image — Raster image decode/encode

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
