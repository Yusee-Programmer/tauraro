# std.font — TrueType and OpenType fonts

```tauraro
from std.font import Font, draw_text, wrap_text, find_system_font, FontSubset
```

Reads TrueType (`glyf`), OpenType with CFF or CFF2 outlines, TrueType collections (`.ttc`) and
WOFF 1.0 fonts from bytes, measures and shapes text, rasterizes it into `std.image` images and
subsets fonts. The core is C (`std/font/font_native.c`, linked automatically) and every read of
font data is bounds checked: damaged fonts raise an error or yield empty glyphs, never crash
(fuzzed under ASan/UBSan).

| Module | Contents |
|---|---|
| `std.font` | `Font`, `GlyphMetrics`, `ShapedGlyph`, `GlyphOutline` and re-exports of everything below |
| `std.font.render` | `draw_text`, `draw_glyph`, `render_text`, `render_glyph` (`GlyphBitmap`) |
| `std.font.layout` | `wrap_text`, `text_lines_height` |
| `std.font.system` | `system_font_dirs`, `system_font_files`, `font_files_in`, `find_system_font` |
| `std.font.subset` | `FontSubset` |

```tauraro
mut f = Font.open("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")?
mut w = f.measure("Hello, world", 24.0)              # advance in pixels at 24 px/em
mut img = Image.init(400, 60)
draw_text(img, f, "Hello, wörld", 24.0, 10.0, 40.0, 20, 20, 20, 255)   # baseline at (10, 40)
mut lines = wrap_text(f, long_text, 12.0, 200.0)     # each line <= 200 px wide
f.free()
```

## Font

| Member | Description |
|---|---|
| `Font.load(bytes)?`, `Font.load_index(bytes, i)?` | Parse a font (face `i` of a collection). |
| `Font.open(path)?`, `Font.open_index(path, i)?` | Read and parse a file. |
| `Font.collection_count(bytes)` | Faces in a buffer (0 if not a font). |
| fields | `units_per_em`, `num_glyphs`, `ascent`, `descent`, `line_gap` (hhea, font units), `family`, `subfamily`, `full_name`, `postscript_name`, `kerning`, `ligatures` (shaping switches, default true). |
| `scale(size)`, `line_height(size)`, `ascent_at(size)`, `descent_at(size)` | Size-dependent metrics. |
| `bbox()`, `weight()`, `typo_metrics()`, `win_metrics()`, `x_height()`, `cap_height()`, `italic_angle()`, `underline_position()`, `underline_thickness()`, `is_fixed_pitch()`, `is_italic()`, `is_bold()`, `outline_format()` | Font-wide properties (OS/2, post, head). |
| `embeddable()`, `embedding_flags()` | OS/2 `fsType` licence bits. |
| `name(id)` | Name-table string. |
| `glyph_index(cp)`, `glyph_index_vs(cp, selector)`, `has_char(cp)`, `covers(text)` | cmap lookups (0 = .notdef). |
| `glyph_advance(gid)`, `glyph_lsb(gid)`, `glyph_metrics(gid)`, `kern(left, right)` | Per glyph, font units. Kerning uses GPOS `kern` pair adjustment, else the `kern` table. |
| `glyph_outline(gid)` | `GlyphOutline` (`cmds`, `xs`, `ys`) in font units, y up: `FONT_MOVE`, `FONT_LINE`, `FONT_QUAD`, `FONT_CUBIC`, `FONT_CLOSE`. |
| `shape(text)` | `ShapedGlyph` list: `gid`, byte range `cluster`..`cluster_end`, `advance` (kerned), pen `x`. |
| `measure(text, size)`, `measure_units(text)`, `text_bounds(text, size)` | Width and ink bounds of one line. |
| `free()` | Release the font. |

Shaping is simple left to right: cmap (with variation selectors), GSUB `liga` ligatures, kerning.

## Rendering

`draw_text(img, font, text, size, x, y, r, g, b, a) -> float` draws with the baseline starting at
(x, y) (pixels, y down) and returns the end pen x. Coverage is exact signed area (cubic and
quadratic curves flattened), composited source-over; no hinting, no pixel snapping.
`draw_glyph` draws one glyph by id. `render_text(font, text, size, r, g, b, a)` returns a new
transparent image fitted to one line; `render_glyph(font, gid, size)` returns a white bitmap whose
alpha is the coverage plus its `left` / `top` offsets from the pen position.

## Layout

`wrap_text(font, text, size, width)` breaks on UAX #29 word boundaries (so CJK wraps between
ideographs), starts a new line at each `\n`, drops trailing spaces and splits over-long words
between grapheme clusters.

## System fonts

`system_font_dirs()` lists the existing font directories (`/usr/share/fonts`, `~/.local/share/fonts`,
`~/.fonts`, `/System/Library/Fonts`, `/Library/Fonts`, `~/Library/Fonts`, `%WINDIR%\Fonts`, ...),
`system_font_files()` scans them recursively and `find_system_font("DejaVu Sans")` returns the
path of the best match by family or full name (regular styles first), or `""`.

## Subsetting

`FontSubset.init(font)`, `add(gid) -> new_id`, `original(new_id)`, `count()`, `build()? -> bytes`,
`free()`. New glyph ids are dense in the order added (0 is .notdef); composite components are
added automatically. TrueType glyphs are copied verbatim; CFF/CFF2 glyphs are converted to
quadratic TrueType contours. The result has head, hhea, maxp, hmtx, loca, glyf, post, OS/2 and a
stub cmap — meant for embedding (`std.encoding.pdf` uses it), not installing.

## Limitations

No complex-script shaping (Arabic joining, Indic reordering, mark positioning, GSUB beyond `liga`),
no hinting, no variable-font instancing (CFF2 and `glyf` use default masters), no color glyph
tables (COLR, SVG, bitmaps), WOFF2 is rejected, composite glyphs with point-matching anchors use
zero offsets.
