# Vendored decoders

| Path | Used by | Upstream | Version | Licence |
|---|---|---|---|---|
| `minimp3.h` | `audio_native.c` | https://github.com/lieff/minimp3 | as imported | CC0 1.0 (public domain dedication) |
| `dr_flac.h` | `audio_native.c` | https://github.com/mackron/dr_libs | as imported | Public domain (Unlicense) or MIT No Attribution, at your choice |
| `stb_vorbis.c` | `ogg_native.c` | https://github.com/nothings/stb (`stb_vorbis.c`) | 1.22 | Public domain (Unlicense) or MIT, at your choice (text at the end of the file) |
| `opus/` | `opus_native.c` (called by `ogg_native.c`) | https://downloads.xiph.org/releases/opus/opus-1.5.2.tar.gz (sha256 `65c1d2f78b9f2fb20082c38cbe47c951ad5839345876e46941612ee87f9a7ce1`) | 1.5.2 | BSD-3-Clause, see `opus/COPYING` |

`minimp3.h` and `dr_flac.h` are unmodified. The licence text of each is at the top / bottom of its file.

## stb_vorbis

Used as downloaded, with three small bounds checks in the comment-header parser of `start_decoder`
(marked `tauraro`): vendor / comment lengths above 16 MiB and comment counts above 2^20 are rejected,
the comment pointer table is zeroed, and `vorbis_deinit` tolerates a missing table. Without them a
corrupt comment header requested multi-gigabyte allocations or crashed in `vorbis_deinit` (found by
ASan fuzzing). `ogg_native.c` also gives it a fixed 48 MiB arena (`stb_vorbis_alloc`) so corrupt setup
headers cannot exhaust memory. It is built with `STB_VORBIS_NO_STDIO` and `STB_VORBIS_NO_PULLDATA_API`
and used through the push-data API, fed with whole Ogg pages. (UBSan reports a benign left shift of a
negative value at stb_vorbis.c:1586.)

## libopus

Only what the decoder needs, flattened into one directory (66 `.c` files and the headers they include)
so that no `-I` flag is required; the sources are unmodified. Taken from `celt/`, `silk/`, `src/` and
`include/`: the CELT decoder and its shared code (`bands celt celt_decoder celt_lpc cwrs entcode entdec
entenc kiss_fft laplace mathops mdct modes pitch quant_bands rate vq`), the SILK decoder and the
resampler it needs (`dec_API decode_* init_decoder PLC CNG NLSF* resampler* tables_* ...`),
`opus.c opus_decoder.c opus_multistream.c opus_multistream_decoder.c`. Not included: encoders, the
analysis/MLP code, projection (ambisonics), the DNN code (deep PLC, DRED, OSCE), SIMD and assembly
variants, CPU detection. `opus_native.c` builds the lot as one translation unit with `FLOATING_POINT`,
`USE_ALLOCA`, no custom modes (about 4 s to compile once; the object is cached).

When updating a codec, bump the `vendor revision` line in the owning shim's header comment: shims are
cached by their own text, so editing only files in this directory does not trigger a rebuild.
