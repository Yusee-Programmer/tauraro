# std.audio — Audio buffers, WAV (and MP3 decode)

```tauraro
from std.audio import AudioBuffer, resample, mix, to_mono, slice, concat, gain, normalize
from std.audio.wav import decode_wav, encode_wav, encode_wav_ex, WavTags
```

Pure Tauraro, no external libraries. All codecs decode **into** one shared
`AudioBuffer`, so conversion and mixing don't depend on the file format.

## AudioBuffer (`std.audio`)

`AudioBuffer` = `rate` (Hz), `channels`, `frames`, and `data` — `frames * channels`
interleaved 32-bit floats nominally in [-1, 1]. A failed decode gives `rate == -1`
(`valid()` is false). Functions return **new** owned buffers unless marked in place;
call `free()` on buffers you are done with.

| Item | Description |
|---|---|
| `AudioBuffer.init(rate, channels, frames)` | silent buffer; `get(frame, ch)`, `set(frame, ch, v)`, `seconds()`, `duplicate()`, `valid()`, `free()` |
| `slice(b, start, count)`, `concat(a, b)` | cut / join (concat needs equal rate + channels) |
| `set_channels(b, n)`, `to_mono(b)`, `to_stereo(b)` | remix: mono→n duplicates, n→mono averages |
| `resample(b, new_rate, quality)` | quality 0 = linear, 1 = windowed-sinc (8 lobes, low-passed when downsampling) |
| `mix(a, b, ga, gb)` | new buffer `a*ga + b*gb`, as long as the longer; `b` is resampled/remixed to match `a` |
| `mix_into(dst, src, offset, g)` | add into `dst` in place (layouts must match) |
| `gain`, `normalize(b, target)`, `clip`, `fade_in(b, n)`, `fade_out(b, n)`, `reverse` | in place |
| `peak(b)`, `rms(b)` | level meters |

Note: the compiler rejects passing the same object twice in one call (`mix(m, m, ..)`);
pass `m.duplicate()` for the second argument.

## std.audio.wav

* **Decode** (`decode_wav(data, len_) -> WavFile`): integer PCM 8-bit unsigned and
  16/24/32-bit signed, IEEE float 32/64, A-law, µ-law, and `WAVE_FORMAT_EXTENSIBLE`
  wrapping PCM/float; any channel count up to 64; chunks in any order (unknown ones are
  skipped); a `data` chunk whose size runs past the file (streamed, size
  `0xFFFFFFFF`) is clamped; a trailing partial frame is dropped. `LIST`/`INFO` tags
  (`INAM`, `IART`, `ICMT`, `ICRD`, ...) are returned via `w.tag("INAM")`,
  `w.tag_count()`, `w.tag_id(i)`, `w.tag_value(i)`. `w.ok`, `w.error`, `w.buf`,
  `w.format`, `w.bits`.
* **Encode**: `encode_wav(buf, bits, is_float) -> (bytes, len)` with `bits` 8/16/24/32
  (PCM) or 32/64 (float); values are rounded and clamped. `encode_wav_ex(buf, bits,
  is_float, tags)` also writes a `LIST`/`INFO` chunk (`WavTags.init()`, `tags.add("INAM",
  "Title")`). More than 2 channels use `WAVE_FORMAT_EXTENSIBLE`. Unsupported
  parameters return `("", 0)`.
* **Verified**: the encoder's 8/16/24-bit output is byte-identical to Python's `wave`
  module; decoder fixtures cover every format above including G.711 reference
  vectors. 16/24-bit and 8-bit PCM survive a decode→encode round trip exactly.
* Limits: no RF64/`ds64` (files > 4 GiB), no ADPCM/other compressed WAV formats
  (`ok == false` with an error message), samples are held as f32 (32-bit PCM keeps ~24 bits).

## std.audio.opus — not implemented

Opus (SILK + CELT + range coder) is a very large codec and no stub is shipped: there is
**no** `std.audio.opus` module.
