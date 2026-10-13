# Vendored codecs

Third-party C codecs compiled into the `std.compress` shims. Each is built as
part of one translation unit (`#include`d by the shim next to the module), so
there is no build system and no external library.

| Directory | Used by | Upstream | Version | Licence |
|---|---|---|---|---|
| `bzip2/` | `std/compress/bzip2_native.c` | https://sourceware.org/bzip2/ (https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz) | 1.0.8 | bzip2 licence (BSD-style), see `bzip2/LICENSE` |
| `xz/` | `std/compress/xz_native.c` | XZ Embedded, https://github.com/tukaani-project/xz-embedded | tag `v2024-12-30` | 0BSD (public-domain equivalent), see `xz/COPYING` |
| `brotli/` | `std/compress/brotli_native.c` | https://github.com/google/brotli (decoder: `c/common`, `c/dec`, `c/include/brotli`) | v1.2.0 | MIT, see `brotli/LICENSE` |

Files taken:

- **bzip2**: `blocksort.c huffman.c crctable.c randtable.c compress.c
  decompress.c bzlib.c bzlib.h bzlib_private.h LICENSE`, unmodified. Built
  with `BZ_NO_STDIO`; the shim supplies `bz_internal_error`.
- **xz**: `linux/include/linux/xz.h`, `userspace/xz_config.h`,
  `linux/lib/xz/{xz_private.h,xz_stream.h,xz_lzma2.h,xz_crc32.c,xz_crc64.c,
  xz_sha256.c,xz_dec_stream.c,xz_dec_lzma2.c,xz_dec_bcj.c}`, `COPYING`,
  unmodified. The shim enables the options by `#define` before including
  them (concatenated streams, CRC64, SHA-256, all BCJ filters) and sets
  `XZ_EXTERN` to `static`.
- **brotli**: the decoder half only (`c/common/*` without the encoder-only
  files, `c/dec/*`, `c/include/brotli/{decode.h,port.h,shared_dictionary.h,types.h}`)
  and `LICENSE`. One mechanical change so that no `-I` flag is needed:
  every `#include <brotli/X.h>` was rewritten to a relative
  `#include "../include/brotli/X.h"` (in `common/` and `dec/`) or
  `#include "X.h"` (inside `include/brotli/`).

The `.lzma` ("LZMA alone") decoder in `xz_native.c` is not vendored; it was
written for Tauraro after the LZMA specification in the public-domain LZMA SDK.

When updating a codec, bump the `vendor revision` line in its shim's header
comment as well: shims are cached by their own text, so editing only files in
this directory does not trigger a rebuild of already-built programs.
