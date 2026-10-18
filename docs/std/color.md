# std.color — Colours, colour spaces and CSS parsing

```tauraro
from std.color import Color, SPACE_OKLCH, delta_e2000_lab
mut c = Color.parse("#3366cc")?
mut d = Color.parse("oklch(70% 0.15 200 / 0.5)")?
mut m = c.mix(d, 0.5, SPACE_OKLCH)
print(m.to_hex_alpha())
print(c.contrast_ratio(Color.white()))     # WCAG
print(c.delta_e2000(d))
```

A `Color` holds non-linear sRGB `r`, `g`, `b` and `a` as floats nominally in 0..1. Out-of-gamut values coming from
Lab/OKLCH/wide-gamut input are kept so round trips are lossless; every byte/hex output clamps, and `clamped()` /
`gamut_mapped()` (OKLCH chroma reduction) bring a value back into sRGB. Not to be confused with `std.term`'s ANSI
`Color`.

Spaces (`SPACE_SRGB`, `SPACE_LINEAR`, `SPACE_HSL`, `SPACE_HSV`, `SPACE_HWB`, `SPACE_LAB`, `SPACE_LCH`, `SPACE_OKLAB`,
`SPACE_OKLCH`, `SPACE_XYZ`): exact sRGB transfer curve, D65 white. Hue is in degrees; saturation, lightness,
whiteness, blackness are 0..1; Lab `L` is 0..100. `to_lab()` / `Color.lab()` are D65-referenced; CSS `lab()`/`lch()`
are D50, available as `to_lab_d50()`, `Color.lab_d50()` (Bradford adaptation) and used by the parser.

| Group | API |
|---|---|
| Construct | `Color.rgb(r,g,b)`, `rgba`, `rgb8(r,g,b)`, `rgba8`, `black()`, `white()`, `transparent()`, `hsl(h,s,l,a)`, `hsv`, `hwb`, `linear`, `xyz`, `lab`, `lch`, `lab_d50`, `lch_d50`, `oklab`, `oklch`, `from_space(space,x,y,z,a)`, `from_argb32(v)` |
| Parse | `Color.parse(text)?` — `#rgb #rgba #rrggbb #rrggbbaa`, bare hex, the 148 CSS names, `transparent`, `rgb()/rgba()`, `hsl()/hsla()`, `hwb()`, `lab()`, `lch()`, `oklab()`, `oklch()`, `color(srgb \| srgb-linear \| xyz \| xyz-d65 \| xyz-d50 ...)`; commas or spaces, `/ alpha`, percentages, `deg/rad/grad/turn`, `none`. `from_hex(s)?`, `named(name)?` |
| Convert | `to_hsl() to_hsv() to_hwb() to_linear() to_xyz() to_lab() to_lch() to_lab_d50() to_lch_d50() to_oklab() to_oklch()` (tuples of three floats), `to_space(space)` |
| Output | `to_hex()`, `to_hex_alpha()`, `to_hex_short()`, `to_css()`, `to_css_hsl()`, `to_css_oklch()`, `to_css_lab()`, `r8() g8() b8() a8()`, `to_rgba8()`, `to_argb32()` |
| Alpha | `with_alpha(a)`, `over(bg)` (gamma-space source-over), `over_linear(bg)` (linear light) |
| Mix | `mix(other, t, space)` — shortest hue arc in polar spaces, achromatic colours borrow the other hue, premultiplied alpha (CSS Color 4); `scale_to(other, n, space)`, `tints(n)`, `shades(n)` |
| Accessibility | `luminance()`, `contrast_ratio(other)` (1..21), `wcag_level(other)` (`"AAA"`, `"AA"`, `"AA Large"`, `"Fail"`), `readable_text()` |
| Distance | `delta_e76`, `delta_e94`, `delta_e2000`, `delta_e_ok` (OKLab); free functions `delta_e76_lab`, `delta_e94_lab(.., textiles)`, `delta_e2000_lab` on Lab values |
| Palettes | `lighten(x)`, `darken(x)`, `saturate(x)`, `desaturate(x)` (HSL, fractions), `rotate_hue(deg)`, `complementary()`, `triadic()`, `tetradic()`, `split_complementary()`, `analogous(n, spread)`, `grayscale()`, `invert()` |

CIEDE2000 is verified against all 34 pairs of Sharma, Wu and Dalal's published test data (to 4 decimals); the
conversions against an independent Python implementation (colorsys, Lindbloom matrices, Ottosson's OKLab) and
published values for sRGB red; 120 random colours round-trip through nine spaces to the same byte values.

Integration: `Image.get_color/set_color/filled/fill_color`, `QrCode.to_image_colors/to_svg_colors`, and
`PdfPage.set_fill_color_c/set_stroke_color_c` accept `Color`. Existing integer-channel APIs are unchanged.
Tested by `tests/lang/137_qr_color.tr`.
