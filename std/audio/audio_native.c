/*
 * std/audio/audio_native.c -- audio codecs and DSP kernels for std.audio.
 *
 * Linked through `# tauraro-cpp-shim: audio_native.c` in std/audio/native.tr.
 *
 *   WAV  read: PCM 8/16/24/32-bit, IEEE float 32/64, A-law, mu-law,
 *        WAVE_FORMAT_EXTENSIBLE; write: PCM 8/16/24/32-bit and float 32
 *   MP3  decode via vendor/minimp3.h (public domain, CC0)
 *   FLAC decode via vendor/dr_flac.h (public domain / MIT-0)
 *   resample: windowed-sinc (Blackman) with an anti-aliasing cutoff
 *
 * Samples cross to Tauraro as interleaved float64 in [-1, 1].
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#define MINIMP3_IMPLEMENTATION
#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_NO_SIMD
#include "vendor/minimp3.h"

#define DR_FLAC_IMPLEMENTATION
#define DR_FLAC_NO_STDIO
#define DR_FLAC_NO_OGG
#include "vendor/dr_flac.h"

#include "au_result.h"

void trc_au_free(void* h) { au_result* r = (au_result*)h; if (r) { free(r->data); free(r); } }
int64_t trc_au_ok(void* h) { au_result* r = (au_result*)h; return r && r->data && r->channels > 0; }
int64_t trc_au_frames(void* h) { return h ? ((au_result*)h)->frames : 0; }
int64_t trc_au_channels(void* h) { return h ? ((au_result*)h)->channels : 0; }
int64_t trc_au_rate(void* h) { return h ? ((au_result*)h)->rate : 0; }
int64_t trc_au_bits(void* h) { return h ? ((au_result*)h)->bits : 0; }
int64_t trc_au_is_float(void* h) { return h ? ((au_result*)h)->is_float : 0; }
int64_t trc_au_error(void* h, char* out, int64_t cap) {
    au_result* r = (au_result*)h; size_t n;
    if (!r) { memcpy(out, "out of memory", 13); return 13; }
    n = strlen(r->err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, r->err, n); return (int64_t)n;
}
/* Copy the samples (frames * channels doubles) into out. */
void trc_au_copy(void* h, double* out) {
    au_result* r = (au_result*)h;
    if (r && r->data) memcpy(out, r->data, sizeof(double) * (size_t)(r->frames * r->channels));
}

/* ── WAV ── */
static uint32_t le32(const uint8_t* p) { return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24); }
static uint16_t le16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }

static double alaw(uint8_t a) {
    int t, seg; a ^= 0x55;
    t = (a & 0x0f) << 4; seg = (a & 0x70) >> 4;
    if (seg == 0) t += 8; else if (seg == 1) t += 0x108; else { t += 0x108; t <<= seg - 1; }
    return ((a & 0x80) ? t : -t) / 32768.0;
}
static double ulaw(uint8_t u) {
    int t; u = (uint8_t)~u;
    t = ((u & 0x0f) << 3) + 0x84; t <<= (u & 0x70) >> 4;
    return ((u & 0x80) ? (0x84 - t) : (t - 0x84)) / 32768.0;
}

void* trc_wav_decode(const char* data, int64_t n) {
    au_result* r = (au_result*)calloc(1, sizeof(au_result)); const uint8_t* p = (const uint8_t*)data;
    size_t pos = 12; int fmt = 0, ch = 0, bits = 0, block = 0; uint32_t rate = 0; const uint8_t* pcm = NULL; size_t pcmlen = 0;
    if (!r) return NULL;
    if (n < 12 || (memcmp(p, "RIFF", 4) != 0 && memcmp(p, "RIFX", 4) != 0) || memcmp(p + 8, "WAVE", 4) != 0) { snprintf(r->err, sizeof r->err, "WAV: not a RIFF/WAVE file"); return r; }
    if (memcmp(p, "RIFX", 4) == 0) { snprintf(r->err, sizeof r->err, "WAV: big-endian RIFX is not supported"); return r; }
    while (pos + 8 <= (size_t)n) {
        uint32_t clen = le32(p + pos + 4); const uint8_t* c = p + pos + 8;
        size_t avail = (size_t)n - pos - 8;
        if (clen > avail) clen = (uint32_t)avail;     /* tolerate a truncated final chunk */
        if (memcmp(p + pos, "fmt ", 4) == 0 && clen >= 16) {
            fmt = le16(c); ch = le16(c + 2); rate = le32(c + 4); block = le16(c + 12); bits = le16(c + 14);
            if (fmt == 0xFFFE && clen >= 26) fmt = le16(c + 24);     /* extensible: sub-format GUID */
        } else if (memcmp(p + pos, "data", 4) == 0) {
            pcm = c; pcmlen = clen;
        }
        pos += 8 + (size_t)clen + (clen & 1);
    }
    if (!fmt || !pcm) { snprintf(r->err, sizeof r->err, "WAV: missing fmt or data chunk"); return r; }
    if (ch < 1 || ch > 64 || rate == 0) { snprintf(r->err, sizeof r->err, "WAV: bad channel count or sample rate"); return r; }
    if (block <= 0) block = ch * ((bits + 7) / 8);
    {
        int bps = (bits + 7) / 8; int64_t frames = (int64_t)(pcmlen / (size_t)block), i; int c;
        if (!((fmt == 1 && (bits == 8 || bits == 16 || bits == 24 || bits == 32)) || (fmt == 3 && (bits == 32 || bits == 64)) || ((fmt == 6 || fmt == 7) && bits == 8))) {
            snprintf(r->err, sizeof r->err, "WAV: unsupported format %d with %d bits", fmt, bits); return r;
        }
        r->data = (double*)malloc(sizeof(double) * (size_t)(frames * ch + 1));
        if (!r->data) { snprintf(r->err, sizeof r->err, "WAV: out of memory"); return r; }
        for (i = 0; i < frames; i++)
            for (c = 0; c < ch; c++) {
                const uint8_t* s = pcm + (size_t)i * (size_t)block + (size_t)c * (size_t)bps; double v = 0;
                if (fmt == 1) {
                    if (bits == 8) v = (s[0] - 128) / 128.0;
                    else if (bits == 16) v = (int16_t)le16(s) / 32768.0;
                    else if (bits == 24) { int32_t x = (int32_t)(s[0] | (s[1] << 8) | (s[2] << 16)); if (x & 0x800000) x -= 0x1000000; v = x / 8388608.0; }
                    else v = (int32_t)le32(s) / 2147483648.0;
                } else if (fmt == 3) {
                    if (bits == 32) { float f; uint32_t u = le32(s); memcpy(&f, &u, 4); v = f; }
                    else { double d; uint64_t u = (uint64_t)le32(s) | ((uint64_t)le32(s + 4) << 32); memcpy(&d, &u, 8); v = d; }
                } else if (fmt == 6) v = alaw(s[0]);
                else v = ulaw(s[0]);
                r->data[i * ch + c] = v;
            }
        r->frames = frames; r->channels = ch; r->rate = (int)rate; r->bits = bits; r->is_float = fmt == 3;
    }
    return r;
}

static void put16(uint8_t* p, int v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); }
static void put32(uint8_t* p, uint32_t v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24); }
static double clip1(double v) { return v > 1 ? 1 : v < -1 ? -1 : v; }

/* bits: 8, 16, 24, 32 (PCM) or 32 with is_float. dither: TPDF for <= 16-bit. */
char* trc_wav_encode(const double* s, int64_t frames, int64_t ch, int64_t rate, int64_t bits, int64_t is_float, int64_t dither, int64_t* out_len) {
    int bps = (int)bits / 8; size_t dlen, total; uint8_t* o; int64_t i; uint32_t seed = 22222;
    if (ch < 1 || rate < 1 || !((!is_float && (bits == 8 || bits == 16 || bits == 24 || bits == 32)) || (is_float && bits == 32))) return NULL;
    dlen = (size_t)(frames * ch * bps); total = 44 + dlen + (dlen & 1);
    o = (uint8_t*)malloc(total);
    if (!o) return NULL;
    memcpy(o, "RIFF", 4); put32(o + 4, (uint32_t)(total - 8)); memcpy(o + 8, "WAVEfmt ", 8);
    put32(o + 16, 16); put16(o + 20, is_float ? 3 : 1); put16(o + 22, (int)ch); put32(o + 24, (uint32_t)rate);
    put32(o + 28, (uint32_t)(rate * ch * bps)); put16(o + 32, (int)(ch * bps)); put16(o + 34, (int)bits);
    memcpy(o + 36, "data", 4); put32(o + 40, (uint32_t)dlen);
    for (i = 0; i < frames * ch; i++) {
        uint8_t* d = o + 44 + (size_t)i * (size_t)bps; double v = clip1(s[i]);
        if (is_float) { float f = (float)v; uint32_t u; memcpy(&u, &f, 4); put32(d, u); continue; }
        if (dither && bits <= 16) {
            double r1, r2, lsb = bits == 8 ? 1.0 / 128 : 1.0 / 32768;
            seed = seed * 1664525u + 1013904223u; r1 = (seed >> 8) / 16777216.0;
            seed = seed * 1664525u + 1013904223u; r2 = (seed >> 8) / 16777216.0;
            v = clip1(v + (r1 - r2) * lsb);
        }
        if (bits == 8) d[0] = (uint8_t)lrint(v * 127.0 + 128.0);
        else if (bits == 16) { long x = lrint(v * 32767.0); put16(d, (int)x); }
        else if (bits == 24) { long x = lrint(v * 8388607.0); d[0] = (uint8_t)x; d[1] = (uint8_t)(x >> 8); d[2] = (uint8_t)(x >> 16); }
        else { long long x = llrint(v * 2147483647.0); put32(d, (uint32_t)x); }
    }
    if (dlen & 1) o[44 + dlen] = 0;
    *out_len = (int64_t)total;
    return (char*)o;
}
void trc_au_buf_free(char* p) { free(p); }

/* ── MP3 ── */
void* trc_mp3_decode(const char* data, int64_t n) {
    au_result* r = (au_result*)calloc(1, sizeof(au_result)); mp3dec_t dec; mp3dec_frame_info_t info;
    float pcm[MINIMP3_MAX_SAMPLES_PER_FRAME]; const uint8_t* p = (const uint8_t*)data; size_t left = (size_t)n, cap = 0;
    if (!r) return NULL;
    /* skip an ID3v2 tag */
    if (left >= 10 && memcmp(p, "ID3", 3) == 0) {
        size_t sz = ((size_t)(p[6] & 0x7f) << 21) | ((size_t)(p[7] & 0x7f) << 14) | ((size_t)(p[8] & 0x7f) << 7) | (p[9] & 0x7f);
        sz += 10 + ((p[5] & 0x10) ? 10 : 0);
        if (sz < left) { p += sz; left -= sz; }
    }
    mp3dec_init(&dec);
    for (;;) {
        int samples = mp3dec_decode_frame(&dec, p, (int)(left > 0x7fffffff ? 0x7fffffff : left), pcm, &info), i;
        if (info.frame_bytes == 0) break;
        p += info.frame_bytes; left -= (size_t)info.frame_bytes;
        if (samples == 0) continue;
        if (!r->channels) { r->channels = info.channels; r->rate = info.hz; }
        if (info.channels != r->channels) continue;    /* channel layout change mid-stream: skip */
        if ((size_t)((r->frames + samples) * r->channels) > cap) {
            size_t nc = cap ? cap * 2 : 1 << 20; double* nd;
            while (nc < (size_t)((r->frames + samples) * r->channels)) nc *= 2;
            nd = (double*)realloc(r->data, sizeof(double) * nc);
            if (!nd) { snprintf(r->err, sizeof r->err, "MP3: out of memory"); free(r->data); r->data = NULL; return r; }
            r->data = nd; cap = nc;
        }
        for (i = 0; i < samples * r->channels; i++) r->data[r->frames * r->channels + i] = pcm[i];
        r->frames += samples;
    }
    if (!r->data) snprintf(r->err, sizeof r->err, "MP3: no decodable frames");
    r->bits = 16; r->is_float = 0;
    return r;
}

/* ── FLAC ── */
void* trc_flac_decode(const char* data, int64_t n) {
    au_result* r = (au_result*)calloc(1, sizeof(au_result)); unsigned int ch = 0, rate = 0; drflac_uint64 frames = 0; float* f; int64_t i;
    drflac* fl;
    if (!r) return NULL;
    fl = drflac_open_memory(data, (size_t)n, NULL);
    if (!fl) { snprintf(r->err, sizeof r->err, "FLAC: not a FLAC stream"); return r; }
    ch = fl->channels; rate = fl->sampleRate; r->bits = fl->bitsPerSample;
    frames = fl->totalPCMFrameCount;
    f = (float*)malloc(sizeof(float) * (size_t)(frames * ch + 1));
    if (!f) { drflac_close(fl); snprintf(r->err, sizeof r->err, "FLAC: out of memory"); return r; }
    frames = drflac_read_pcm_frames_f32(fl, frames, f);
    drflac_close(fl);
    r->data = (double*)malloc(sizeof(double) * (size_t)(frames * ch + 1));
    if (!r->data) { free(f); snprintf(r->err, sizeof r->err, "FLAC: out of memory"); return r; }
    for (i = 0; i < (int64_t)(frames * ch); i++) r->data[i] = f[i];
    free(f);
    r->frames = (int64_t)frames; r->channels = (int)ch; r->rate = (int)rate;
    return r;
}

/* ── resampling ── windowed sinc, `quality` taps per side (8..64). */
int64_t trc_au_resample(const double* in, int64_t frames, int64_t ch, int64_t from_rate, int64_t to_rate, double* out, int64_t out_frames, int64_t quality) {
    double ratio = (double)to_rate / (double)from_rate, cutoff = ratio < 1 ? ratio : 1.0; int64_t o; int taps = (int)quality, c;
    if (taps < 4) taps = 4;
    if (taps > 128) taps = 128;
    for (o = 0; o < out_frames; o++) {
        double pos = o / ratio; int64_t center = (int64_t)floor(pos), k;
        for (c = 0; c < ch; c++) {
            double acc = 0, wsum = 0;
            for (k = center - taps + 1; k <= center + taps; k++) {
                double x = (pos - k) * cutoff, sinc, win, t, wgt;
                if (k < 0 || k >= frames) continue;
                sinc = x == 0 ? 1.0 : sin(M_PI * x) / (M_PI * x);
                t = (pos - k) / (taps / cutoff);
                if (t <= -1 || t >= 1) continue;
                win = 0.42 + 0.5 * cos(M_PI * t) + 0.08 * cos(2 * M_PI * t);   /* Blackman */
                wgt = sinc * win * cutoff;
                acc += in[k * ch + c] * wgt; wsum += wgt;
            }
            out[o * ch + c] = wsum != 0 ? acc / wsum : 0;
        }
    }
    return out_frames;
}
