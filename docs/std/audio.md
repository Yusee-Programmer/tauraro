# std.audio — Audio buffers, codecs and DSP

```tauraro
from std.audio.mod import Audio, AudioBuffer, Wav, Mp3, Flac, Ogg, Vorbis, Opus
```

An `AudioBuffer` holds interleaved float samples in [-1, 1] (`samples[frame * channels + channel]`), the
`sample_rate`, `channels` and the decoded `source_bits`.

## Decoding and encoding

| Call | Description |
|---|---|
| `Audio.decode(data, len)?` | Sniffs WAV, FLAC, MP3, Ogg Vorbis or Ogg Opus. |
| `Wav.decode(data, len)?` | PCM 8/16/24/32-bit, IEEE float 32/64, A-law, μ-law, `WAVE_FORMAT_EXTENSIBLE`. |
| `Wav.encode(buf, bits, float_format)? -> str` | PCM 8/16/24/32-bit (TPDF dither at 8/16-bit) or float 32. |
| `Mp3.decode(data, len)?` | MPEG-1/2/2.5 Layer III, ID3v2 tags skipped (vendored minimp3, CC0). |
| `Flac.decode(data, len)?` | FLAC (vendored dr_flac, public domain / MIT-0). |

| `Ogg.decode(data, len)?` | Ogg Vorbis or Ogg Opus, whichever the first audio stream is. |
| `Vorbis.decode(data, len)?` / `Opus.decode(data, len)?` | Same, but an error if the first audio stream is the other codec. |

## Ogg, Vorbis and Opus — `std.audio.ogg`

```tauraro
from std.audio.ogg import OggFile, OggReader, OggDecoder, OggInfo
```

Container: RFC 3533 pages (CRC-32 verified, damaged pages skipped and counted in `bad_pages`), packet
reassembly across pages, multiplexed logical streams (the first Vorbis or Opus stream is decoded, all
streams are listed in `OggInfo.streams`), granule positions for exact length and end trimming.
Vorbis is decoded by vendored stb_vorbis 1.22 (public domain / MIT), Opus by the vendored libopus 1.5.2
decoder (BSD-3-Clause; SILK, CELT and hybrid; mapping families 0, 1, 2 and 255; RFC 7845 pre-skip, output
gain and end trimming). Opus is always decoded at 48 kHz. Channels are in WAV order (L R C LFE ...).

| Call | Description |
|---|---|
| `OggFile.info(data, len)? -> OggInfo` | `codec` ("vorbis"/"opus"), `channels`, `sample_rate`, exact `frames`, `duration`, `serial`, `pre_skip`, `output_gain_db`, `channel_family`, `nominal_bitrate`, `vendor`, `comments` (`KEY=value` list), `streams`, `bad_pages`, `lost_pages`, `bad_packets`; `info.tag("TITLE")` / `info.tags("ARTIST")` look tags up case-insensitively. |
| `OggFile.open(data, len)? -> OggReader` | Random access: `read(max)`, `read_all()`, `seek(frame)?`, `seek_seconds(s)?`, `tell()`, `total`, `info()`, `close()`. Seeks are sample-exact (the decoder pre-rolls from an earlier page). |
| `OggFile.pages(data, len)` / `OggFile.packets(data, len, serial)` | Raw structure: `OggPage` (offset, length, flags, granule, serial, sequence, packets) and `OggPacket` (length, granule, serial, first, last); `serial < 0` = every stream. |
| `OggDecoder.init(codec)` | Streaming decoder (`codec` "", "vorbis" or "opus"): `feed(bytes, n)?` any chunk size, `ready()`, `channels()`, `sample_rate()`, `available()`, `pull(max)` (negative = everything), `info()`, `finish()?` (applies the final end trim and reports truncated input), `close()`. Output is bit-identical to the one-shot decode. |
| `OggFile.opus_version()` | The vendored libopus version string. |

```tauraro
mut info = OggFile.info(bytes, n)?
print(info.tag("TITLE"), info.duration)
mut r = OggFile.open(bytes, n)?
r.seek(48000)?
mut chunk = r.read(4096)
```

Accuracy against libvorbis / libopus (GStreamer): Vorbis within 3e-7 of the float output (SNR 140 dB), Opus
within one 16-bit LSB of the reference's rounding (SNR 80 - 100 dB); frame counts are exact.

Limitations: Opus projection (mapping family 3, ambisonics) is not supported; Vorbis is limited to 16
channels; Vorbis bit-exactness is that of stb_vorbis (a few ulp from libvorbis); chained Ogg files decode
only their first logical stream per type; the reader keeps a copy of the file in memory.

## DSP (each returns a new buffer)

`to_mono()`, `to_channels(n)`, `resample(rate, quality)` (windowed sinc; `quality` = taps per side, 16–64),
`gain(factor)`, `gain_db(db)`, `normalize(peak)`, `clip()`, `fade_in(sec)`, `fade_out(sec)`, `trim(start, end)`,
`concat(other)`, `mix(other, gain)`, `reverse()`, `copy()`. Measurements: `frames()`, `duration()`, `peak()`, `peak_db()`,
`rms()`, `get(frame, ch)`, `set(...)`. Generators: `Audio.sine/square/sawtooth(freq, sec, rate, amp)`,
`Audio.noise(sec, rate, amp, seed)`, `Audio.silence(sec, rate, channels)`; `AudioBuffer.init(rate, ch, frames)`,
`AudioBuffer.from_samples(rate, ch, samples)?`.

```tauraro
mut buf = Audio.decode(bytes, n)?
mut speech = buf.to_mono().resample(16000, 32).normalize(0.9)
mut wav = Wav.encode(speech, 16, false)?
```

Tested by `tests/lang/115_audio.tr` (MP3 against ISO conformance vectors, FLAC bit-exact).

## Spectra and filters — `std.audio.dsp`

`AudioDsp` connects `AudioBuffer` to [`std.math.fft` / `std.math.dsp`](math.md#fft--stdmathfft). Analysis works on
the mono mix-down; filters run on every channel and return a new buffer.

```tauraro
from std.audio.dsp import AudioDsp
mut f = AudioDsp.dominant_frequency(buf)?               # Hz, parabolic-interpolated peak
mut sg = AudioDsp.spectrogram(buf, 1024, 256, "db")?    # Spectrogram (to_tensor() for a Tensor)
mut warm = AudioDsp.eq(AudioDsp.highpass(buf, 80.0, 0.7071), 3000.0, 1.0, -3.0)
```

`spectrum(buf)?` (Hann-windowed magnitudes, a full-scale sine reads its amplitude) → `Spectrum` (`freqs`,
`magnitudes`, `peak_freq()`), `dominant_frequency(buf)?`, `spectrogram(buf, nperseg, hop, mode)?`, `welch(buf,
nperseg)?`, `biquad(buf, bq)`, `lowpass/highpass(buf, cutoff, q)`, `eq(buf, freq, q, gain_db)`, `lfilter(buf, b, a)?`,
`filtfilt(buf, b, a)?`, `fir(buf, taps, compensate)?` (compensate removes a linear-phase FIR's delay),
`mono_samples(buf)`. Tested by `tests/lang/125_fft.tr`.
