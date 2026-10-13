# std.audio — Audio buffers, codecs and DSP

```tauraro
from std.audio.mod import Audio, AudioBuffer, Wav, Mp3, Flac
```

An `AudioBuffer` holds interleaved float samples in [-1, 1] (`samples[frame * channels + channel]`), the
`sample_rate`, `channels` and the decoded `source_bits`.

## Decoding and encoding

| Call | Description |
|---|---|
| `Audio.decode(data, len)?` | Sniffs WAV, FLAC or MP3. |
| `Wav.decode(data, len)?` | PCM 8/16/24/32-bit, IEEE float 32/64, A-law, μ-law, `WAVE_FORMAT_EXTENSIBLE`. |
| `Wav.encode(buf, bits, float_format)? -> str` | PCM 8/16/24/32-bit (TPDF dither at 8/16-bit) or float 32. |
| `Mp3.decode(data, len)?` | MPEG-1/2/2.5 Layer III, ID3v2 tags skipped (vendored minimp3, CC0). |
| `Flac.decode(data, len)?` | FLAC (vendored dr_flac, public domain / MIT-0). |

Opus and Ogg Vorbis are not supported.

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
