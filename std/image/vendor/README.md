# Vendored material in std.image

Nothing is compiled from third-party source files. The only third-party content is **data tables** inside
`std/image/webp_native.c`, copied from libwebp (https://github.com/webmproject/libwebp, `main` as of 2026-10):

| Tables | libwebp file | Licence |
|---|---|---|
| default coefficient probabilities (`CoeffsProba0`), coefficient update probabilities (`CoeffsUpdateProba`), key-frame sub-block mode probabilities (`kBModesProba`) | `src/dec/tree_dec.c` | BSD-3-Clause, Copyright (c) 2010 Google Inc. |
| DC / AC dequantisation tables (`kDcTable`, `kAcTable`) | `src/dec/quant_dec.c` | same |

These are the normative tables of RFC 6386 (VP8) and are required for bit-exact lossy decoding. The licence text is in
`LICENSE.libwebp` (the BSD-3-Clause licence of libwebp's COPYING file) and must travel with redistributions of
`webp_native.c`. All code in `webp_native.c`, `tiff_native.c` and `qr_native.c` is original, written from the WebP
container specification, RFC 9649 (VP8L), RFC 6386 (VP8), the TIFF 6.0 specification and ISO/IEC 18004; where libwebp's
integer arithmetic had to be reproduced for bit-identical output (inverse DCT/WHT, loop filters, fancy upsampling,
YUV→RGB fixed-point constants, animation blend) it was re-implemented from the published algorithm.
