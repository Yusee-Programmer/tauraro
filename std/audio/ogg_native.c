/*
 * std/audio/ogg_native.c -- Ogg container (RFC 3533), Ogg Vorbis and Ogg Opus
 * (RFC 7845) decoding for std.audio.ogg.
 *
 * Linked through `# tauraro-cpp-shim: ogg_native.c` in std/audio/ogg_native.tr.
 *
 *   Ogg     page sync + CRC-32 check, packet reassembly over page boundaries,
 *           multiplexed logical streams (the first Vorbis/Opus stream is
 *           decoded, the others are listed), granule positions for duration,
 *           exact end trimming and sample-accurate seeking
 *   Vorbis  vendor/stb_vorbis.c (public domain / MIT) in push mode, fed with
 *           the pages of the selected stream; comment header parsed here
 *   Opus    vendor/opus (libopus 1.5.2 decoder, BSD-3-Clause) compiled by
 *           opus_native.c; OpusHead (pre-skip, output gain, channel mapping
 *           families 0, 1, 2 and 255), OpusTags, 48 kHz output
 *
 * One engine (ogg_dec) serves the streaming decoder (feed bytes / pull
 * frames), the in-memory seekable reader and the one-shot decode.
 *
 * vendor revision: stb_vorbis 1.22, libopus 1.5.2
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "au_result.h"
#include "vendor/opus/opus_multistream.h"

#define STB_VORBIS_NO_STDIO
#define STB_VORBIS_NO_PULLDATA_API
#define STB_VORBIS_MAX_CHANNELS 16
#include "vendor/stb_vorbis.c"

/* ── Ogg pages ── */
static uint32_t ogg_crc_table[256];
static int ogg_crc_ready = 0;
static void ogg_crc_init(void) {
    uint32_t i, j, r;
    if (ogg_crc_ready) return;
    for (i = 0; i < 256; i++) {
        r = i << 24;
        for (j = 0; j < 8; j++) r = (r & 0x80000000u) ? (r << 1) ^ 0x04c11db7u : (r << 1);
        ogg_crc_table[i] = r;
    }
    ogg_crc_ready = 1;
}
static uint32_t ogg_crc(const uint8_t* p, size_t n) {
    uint32_t c = 0; size_t i;
    for (i = 0; i < n; i++) {
        uint8_t b = (i >= 22 && i < 26) ? 0 : p[i];     /* checksum field counts as zero */
        c = (c << 8) ^ ogg_crc_table[((c >> 24) & 0xff) ^ b];
    }
    return c;
}
static uint32_t rd32(const uint8_t* p) { return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24); }
static uint16_t rd16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }
static int64_t rd64(const uint8_t* p) { return (int64_t)((uint64_t)rd32(p) | ((uint64_t)rd32(p + 4) << 32)); }

typedef struct {
    const uint8_t* p;      /* page start ("OggS") */
    size_t len;            /* header + body */
    int flags;             /* 1 continued, 2 BOS, 4 EOS */
    int64_t granule;       /* -1: no packet ends on this page */
    uint32_t serial, seq;
    int nseg; const uint8_t* lacing; const uint8_t* body;
} ogg_page;

/* Look for a complete, CRC-valid page in buf[*pos..n). Returns 1 and advances
   *pos past it, 0 when more data is needed (*pos left at the candidate start),
   skipping garbage and corrupt pages (counted in *bad). */
static int ogg_next_page(const uint8_t* buf, size_t n, size_t* pos, ogg_page* pg, int64_t* bad) {
    size_t i = *pos;
    ogg_crc_init();
    for (;;) {
        size_t hl, bl = 0; int k;
        while (i + 4 <= n && memcmp(buf + i, "OggS", 4) != 0) i++;
        if (i + 4 > n) { *pos = i; return 0; }
        if (i + 27 > n) { *pos = i; return 0; }
        if (buf[i + 4] != 0) { i++; (*bad)++; continue; }
        hl = 27 + (size_t)buf[i + 26];
        if (i + hl > n) { *pos = i; return 0; }
        for (k = 0; k < buf[i + 26]; k++) bl += buf[i + 27 + k];
        if (i + hl + bl > n) { *pos = i; return 0; }
        if (ogg_crc(buf + i, hl + bl) != rd32(buf + i + 22)) { i++; (*bad)++; continue; }
        pg->p = buf + i; pg->len = hl + bl; pg->flags = buf[i + 5];
        pg->granule = rd64(buf + i + 6); pg->serial = rd32(buf + i + 14); pg->seq = rd32(buf + i + 18);
        pg->nseg = buf[i + 26]; pg->lacing = buf + i + 27; pg->body = buf + i + hl;
        *pos = i + hl + bl;
        return 1;
    }
}

/* ── engine ── */
enum { CODEC_NONE = 0, CODEC_VORBIS = 1, CODEC_OPUS = 2 };
#define MAX_STREAMS 64

typedef struct { uint32_t serial; char codec[16]; } ogg_stream_info;
typedef struct { size_t off, len; int64_t granule; int flags; } ogg_index_ent;

typedef struct {
    int want;                     /* CODEC_NONE = first Vorbis or Opus stream */
    int codec, selected, nhdr, ready, ended, fatal;
    uint32_t serial, next_seq; int have_seq;
    ogg_stream_info streams[MAX_STREAMS]; int nstreams;
    /* packet reassembly */
    uint8_t* pkt; size_t plen, pcap; int pkt_open, drop_cont;
    /* stream parameters */
    int channels, rate, pre_skip, gain_q8, family, nstreams_opus, ncoupled, bitrate_nominal;
    uint8_t mapping[255];
    char* vendor; char** cm; int64_t* cmlen; int ncm;
    /* Vorbis (stb_vorbis push mode) */
    stb_vorbis* v; stb_vorbis_alloc valloc; uint8_t* vb; size_t vblen, vbcap; uint8_t* hdr; size_t hdrlen, hdrcap;
    /* Opus */
    OpusMSDecoder* od; float* obuf;
    /* timeline */
    int64_t dec_total, gran_base, qhead_dec, skip, seek_target, last_granule;
    int base_known;
    /* output queue: interleaved float frames [qhead, qlen) */
    float* q; size_t qhead, qlen, qcap;
    /* input buffer for the streaming decoder */
    uint8_t* in; size_t inlen, incap;
    /* seekable reader: whole file + page index of the selected stream */
    uint8_t* data; size_t dlen;
    ogg_index_ent* idx; size_t nidx, idxcap, feed_next;
    int64_t total_frames, first_pcm, end_granule;
    int64_t bad_pages, gaps, bad_packets;
    char err[160];
} ogg_dec;

static int grow(void** p, size_t* cap, size_t need, size_t elem) {
    size_t nc; void* np;
    if (need <= *cap) return 1;
    nc = *cap ? *cap : 256;
    while (nc < need) { if (nc > ((size_t)1 << 40)) return 0; nc *= 2; }
    np = realloc(*p, nc * elem);
    if (!np) return 0;
    *p = np; *cap = nc; return 1;
}
static void fail(ogg_dec* e, const char* msg) { if (!e->fatal) { snprintf(e->err, sizeof e->err, "%s", msg); e->fatal = 1; } }

static void codec_free(ogg_dec* e) {
    if (e->v) { stb_vorbis_close(e->v); e->v = NULL; }
    if (e->od) { opus_multistream_decoder_destroy(e->od); e->od = NULL; }
}

/* frames the head of the queue may give up (the trimmed tail stays) */
static void q_drop_front(ogg_dec* e, int64_t k) {
    int64_t have = (int64_t)(e->qlen - e->qhead);
    if (k <= 0) return;
    if (k > have) { e->skip += k - have; k = have; }
    e->qhead += (size_t)k; e->qhead_dec += k;
}
/* Vorbis channel order (Vorbis I spec 4.3.9, also Opus mapping family 1) to the
   WAV / SMPTE order used by std.audio: out channel c = decoded channel perm[c]. */
static const uint8_t vorbis_to_wav[9][8] = {
    {0}, {0}, {0, 1}, {0, 2, 1}, {0, 1, 2, 3}, {0, 2, 1, 3, 4},
    {0, 2, 1, 5, 3, 4}, {0, 2, 1, 6, 5, 3, 4}, {0, 2, 1, 7, 5, 6, 3, 4},
};
static void q_push(ogg_dec* e, const float* interleaved, float** planar, int frames) {
    int ch = e->channels, i, c; int64_t start = 0;
    const uint8_t* perm = (ch <= 8 && (e->codec == CODEC_VORBIS || e->family == 1)) ? vorbis_to_wav[ch] : NULL;
    e->dec_total += frames;
    if (e->skip > 0) { start = e->skip < frames ? e->skip : frames; e->skip -= start; e->qhead_dec += start; }
    if (start >= frames) return;
    if (e->qhead > 0 && e->qhead == e->qlen) e->qhead = e->qlen = 0;
    if (e->qhead > (1u << 16) && e->qhead * 2 > e->qlen) {   /* compact */
        memmove(e->q, e->q + e->qhead * (size_t)ch, (e->qlen - e->qhead) * (size_t)ch * sizeof(float));
        e->qlen -= e->qhead; e->qhead = 0;
    }
    if (!grow((void**)&e->q, &e->qcap, (e->qlen + (size_t)frames) * (size_t)ch, sizeof(float))) { fail(e, "Ogg: out of memory"); return; }
    for (i = (int)start; i < frames; i++)
        for (c = 0; c < ch; c++)
        {
            int sc = perm ? perm[c] : c;
            e->q[(e->qlen + (size_t)(i - start)) * (size_t)ch + (size_t)c] = interleaved ? interleaved[(size_t)i * (size_t)ch + (size_t)sc] : planar[sc][i];
        }
    e->qlen += (size_t)(frames - start);
}
/* Remove decoded samples beyond granule g (end trimming). */
static void trim_tail(ogg_dec* e, int64_t g) {
    int64_t want = g - e->gran_base, extra, have;
    if (!e->base_known || g < 0) return;
    extra = e->dec_total - want;
    if (extra <= 0) return;
    have = (int64_t)(e->qlen - e->qhead);
    if (extra > have) extra = have;
    e->qlen -= (size_t)extra; e->dec_total -= extra;
}
static int64_t pre_skip_of(ogg_dec* e) { return e->codec == CODEC_OPUS ? e->pre_skip : 0; }

/* A page with a granule position finished: fix the timeline. */
static void page_done(ogg_dec* e, int64_t g, int last) {
    if (g < 0) return;
    e->last_granule = g;
    if (!e->base_known) {
        e->base_known = 1;
        e->gran_base = g - e->dec_total;
        /* Opus: a first page whose granule is below the decoded count is only
           valid as the last page and means "trim the end" (RFC 7845 4.5). */
        if (e->gran_base < 0 && e->codec == CODEC_OPUS) e->gran_base = 0;
        if (e->seek_target >= 0) {
            int64_t pos0 = e->gran_base - pre_skip_of(e);
            q_drop_front(e, e->seek_target - pos0 - e->qhead_dec);
            e->seek_target = -1;
        } else if (e->gran_base < 0) {
            q_drop_front(e, -e->gran_base);     /* Vorbis: leading samples before granule 0 are discarded */
        }
    }
    if (last) trim_tail(e, g);
}

static const char* sniff_codec(const uint8_t* b, size_t n) {
    if (n >= 7 && memcmp(b, "\x01vorbis", 7) == 0) return "vorbis";
    if (n >= 8 && memcmp(b, "OpusHead", 8) == 0) return "opus";
    if (n >= 7 && memcmp(b, "\x80theora", 7) == 0) return "theora";
    if (n >= 5 && memcmp(b, "\x7f" "FLAC", 5) == 0) return "flac";
    if (n >= 8 && memcmp(b, "Speex   ", 8) == 0) return "speex";
    if (n >= 8 && memcmp(b, "fishead\0", 8) == 0) return "skeleton";
    if (n >= 8 && memcmp(b, "OVP80\x01\x01", 7) == 0) return "vp8";
    if (n >= 5 && memcmp(b, "\x80kate", 5) == 0) return "kate";
    if (n >= 8 && memcmp(b, "PCM     ", 8) == 0) return "pcm";
    return "unknown";
}

/* Vorbis / Opus comment block: vendor, then n "KEY=value" strings. */
static int parse_comments(ogg_dec* e, const uint8_t* p, size_t n) {
    uint32_t vl, cnt, i; size_t pos;
    if (n < 4) return 0;
    vl = rd32(p);
    if (vl > n - 4) return 0;
    e->vendor = (char*)malloc((size_t)vl + 1);
    if (!e->vendor) return 0;
    memcpy(e->vendor, p + 4, vl); e->vendor[vl] = 0;
    pos = 4 + (size_t)vl;
    if (n - pos < 4) return 1;                  /* tolerate a missing count */
    cnt = rd32(p + pos); pos += 4;
    if (cnt > (n - pos) / 4) cnt = (uint32_t)((n - pos) / 4);
    e->cm = (char**)calloc(cnt + 1, sizeof(char*)); e->cmlen = (int64_t*)calloc(cnt + 1, sizeof(int64_t));
    if (!e->cm || !e->cmlen) return 0;
    for (i = 0; i < cnt; i++) {
        uint32_t l;
        if (n - pos < 4) break;
        l = rd32(p + pos); pos += 4;
        if (l > n - pos) break;
        e->cm[e->ncm] = (char*)malloc((size_t)l + 1);
        if (!e->cm[e->ncm]) return 0;
        memcpy(e->cm[e->ncm], p + pos, l); e->cm[e->ncm][l] = 0; e->cmlen[e->ncm] = l;
        e->ncm++; pos += l;
    }
    return 1;
}

static void opus_header(ogg_dec* e, const uint8_t* p, size_t n) {
    int ch, err = 0, i;
    if (n < 19 || (p[8] & 0xf0) != 0) { fail(e, "Opus: bad OpusHead (unsupported version)"); return; }
    ch = p[9];
    if (ch < 1) { fail(e, "Opus: OpusHead has zero channels"); return; }
    e->channels = ch; e->rate = 48000;
    e->pre_skip = rd16(p + 10); e->gain_q8 = (int16_t)rd16(p + 16); e->family = p[18];
    if (e->family == 0) {
        if (ch > 2) { fail(e, "Opus: mapping family 0 allows at most 2 channels"); return; }
        e->nstreams_opus = 1; e->ncoupled = ch - 1; e->mapping[0] = 0; e->mapping[1] = 1;
    } else if (e->family == 1 || e->family == 2 || e->family == 255) {
        if (n < 21 + (size_t)ch) { fail(e, "Opus: OpusHead channel mapping table is truncated"); return; }
        e->nstreams_opus = p[19]; e->ncoupled = p[20];
        if (e->nstreams_opus < 1 || e->ncoupled > e->nstreams_opus || e->nstreams_opus + e->ncoupled > 255) { fail(e, "Opus: bad stream counts in OpusHead"); return; }
        if (e->family == 1 && ch > 8) { fail(e, "Opus: mapping family 1 allows at most 8 channels"); return; }
        for (i = 0; i < ch; i++) {
            e->mapping[i] = p[21 + i];
            if (e->mapping[i] != 255 && e->mapping[i] >= e->nstreams_opus + e->ncoupled) { fail(e, "Opus: bad channel mapping"); return; }
        }
    } else { fail(e, "Opus: unsupported channel mapping family (3 = projection is not supported)"); return; }
    e->od = opus_multistream_decoder_create(48000, ch, e->nstreams_opus, e->ncoupled, e->mapping, &err);
    if (!e->od || err != OPUS_OK) { e->od = NULL; fail(e, "Opus: cannot create the decoder for this OpusHead"); return; }
    if (e->gain_q8) opus_multistream_decoder_ctl(e->od, OPUS_SET_GAIN(e->gain_q8));
    e->obuf = (float*)malloc(sizeof(float) * 5760 * (size_t)ch);
    if (!e->obuf) fail(e, "Opus: out of memory");
    e->skip = e->pre_skip;
}

static void opus_packet(ogg_dec* e, const uint8_t* p, size_t n) {
    int got;
    if (!e->od) return;
    got = opus_multistream_decode_float(e->od, p, (opus_int32)n, e->obuf, 5760, 0);
    if (got < 0) {      /* corrupt packet: conceal with the packet's duration */
        int dur = opus_packet_get_nb_samples(p, (opus_int32)n, 48000);
        e->bad_packets++;
        if (dur <= 0 || dur > 5760) return;
        got = opus_multistream_decode_float(e->od, NULL, 0, e->obuf, dur, 0);
        if (got < 0) return;
    }
    if (got > 0) q_push(e, e->obuf, NULL, got);
}

static void vorbis_drain(ogg_dec* e) {
    size_t pos = 0; int guard = 0;
    while (e->v && pos < e->vblen && !e->fatal) {
        int ch = 0, samples = 0; float** out = NULL;
        int used = stb_vorbis_decode_frame_pushdata(e->v, e->vb + pos, (int)(e->vblen - pos), &ch, &out, &samples);
        if (used <= 0) break;
        pos += (size_t)used;
        if (samples > 0 && out) {
            if (ch != e->channels) { e->bad_packets++; continue; }
            q_push(e, NULL, out, samples);
        }
        if (++guard > 1000000) break;
    }
    if (pos > 0) { memmove(e->vb, e->vb + pos, e->vblen - pos); e->vblen -= pos; }
    if (e->vblen > (1u << 20)) {      /* a packet that never completes: resynchronise */
        e->vblen = 0; e->bad_packets++;
        if (e->v) stb_vorbis_flush_pushdata(e->v);
    }
}

static void vorbis_open(ogg_dec* e) {
    int used = 0, err = 0; stb_vorbis_info vi;
    /* All decoder memory comes from one fixed arena: a corrupt setup header
       cannot request an absurd allocation (stb_vorbis returns NULL instead). */
    if (!e->valloc.alloc_buffer) {
        e->valloc.alloc_buffer_length_in_bytes = 48 << 20;
        e->valloc.alloc_buffer = (char*)malloc((size_t)e->valloc.alloc_buffer_length_in_bytes);
        if (!e->valloc.alloc_buffer) { fail(e, "Vorbis: out of memory"); return; }
    }
    e->v = stb_vorbis_open_pushdata(e->hdr, (int)e->hdrlen, &used, &err, &e->valloc);
    if (!e->v) { fail(e, err == VORBIS_too_many_channels ? "Vorbis: too many channels (max 16)" : "Vorbis: cannot parse the header packets"); return; }
    vi = stb_vorbis_get_info(e->v);
    if ((int)vi.channels != e->channels || (int)vi.sample_rate != e->rate) { fail(e, "Vorbis: inconsistent header"); return; }
}

/* A complete packet of the selected stream. */
static void on_packet(ogg_dec* e, const uint8_t* p, size_t n) {
    if (e->fatal) return;
    if (e->codec == CODEC_OPUS) {
        if (e->nhdr == 0) { opus_header(e, p, n); e->nhdr = 1; return; }
        if (e->nhdr == 1) {
            if (n < 8 || memcmp(p, "OpusTags", 8) != 0) { fail(e, "Opus: missing OpusTags header"); return; }
            if (!parse_comments(e, p + 8, n - 8)) e->ncm = 0;
            e->nhdr = 2; e->ready = 1; return;
        }
        opus_packet(e, p, n);
        return;
    }
    /* Vorbis headers only; audio goes through stb_vorbis as whole pages */
    if (e->nhdr == 0) {
        if (n < 30 || memcmp(p, "\x01vorbis", 7) != 0 || rd32(p + 7) != 0) { fail(e, "Vorbis: bad identification header"); return; }
        e->channels = p[11]; e->rate = (int)rd32(p + 12); e->bitrate_nominal = (int)rd32(p + 20);
        if (e->channels < 1 || e->rate < 1) { fail(e, "Vorbis: bad channel count or sample rate"); return; }
        if (e->channels > 16) { fail(e, "Vorbis: too many channels (max 16)"); return; }
        e->nhdr = 1; return;
    }
    if (e->nhdr == 1) {
        if (n < 7 || memcmp(p, "\x03vorbis", 7) != 0) { fail(e, "Vorbis: missing comment header"); return; }
        if (!parse_comments(e, p + 7, n - 7)) e->ncm = 0;
        e->nhdr = 2; return;
    }
    if (e->nhdr == 2) {
        if (n < 7 || memcmp(p, "\x05vorbis", 7) != 0) { fail(e, "Vorbis: missing setup header"); return; }
        e->nhdr = 3;
    }
}

/* Split a page body into packets (reassembling across pages). */
static void page_packets(ogg_dec* e, const ogg_page* pg) {
    const uint8_t* b = pg->body; int s; size_t off = 0, start = 0;
    if (!(pg->flags & 1)) { e->plen = 0; e->pkt_open = 0; }           /* fresh packet starts here */
    else if (!e->pkt_open) e->drop_cont = 1;                           /* continuation of something we don't have */
    for (s = 0; s < pg->nseg && !e->fatal; s++) {
        off += pg->lacing[s];
        if (pg->lacing[s] < 255) {
            size_t seg = off - start;
            if (e->drop_cont) { e->drop_cont = 0; start = off; e->plen = 0; e->pkt_open = 0; continue; }
            if (e->plen == 0) on_packet(e, b + start, seg);        /* whole packet on this page: no copy */
            else {
                if (!grow((void**)&e->pkt, &e->pcap, e->plen + seg, 1)) { fail(e, "Ogg: out of memory"); return; }
                memcpy(e->pkt + e->plen, b + start, seg); e->plen += seg;
                on_packet(e, e->pkt, e->plen);
            }
            e->plen = 0; e->pkt_open = 0; start = off;
            if (e->codec == CODEC_VORBIS && e->nhdr >= 3) break;      /* the rest is audio (stb_vorbis) */
        }
    }
    if (s == pg->nseg && start < off && !e->drop_cont) {        /* packet continues on the next page */
        size_t seg = off - start;
        if (e->plen + seg > ((size_t)1 << 26)) { e->plen = 0; e->drop_cont = 1; e->bad_packets++; return; }
        if (!grow((void**)&e->pkt, &e->pcap, e->plen + seg, 1)) { fail(e, "Ogg: out of memory"); return; }
        memcpy(e->pkt + e->plen, b + start, seg); e->plen += seg; e->pkt_open = 1;
    } else if (s == pg->nseg && start < off) e->pkt_open = 0;
}

static void note_stream(ogg_dec* e, const ogg_page* pg) {
    int i; size_t first = 0;
    for (i = 0; i < e->nstreams; i++) if (e->streams[i].serial == pg->serial) return;
    if (e->nstreams >= MAX_STREAMS) return;
    if (pg->nseg > 0) {
        for (i = 0; i < pg->nseg; i++) { first += pg->lacing[i]; if (pg->lacing[i] < 255) break; }
    }
    e->streams[e->nstreams].serial = pg->serial;
    snprintf(e->streams[e->nstreams].codec, sizeof e->streams[0].codec, "%s", sniff_codec(pg->body, first));
    e->nstreams++;
}

/* Process one CRC-valid page. last: treat as the final page (end trim). */
static void eng_page(ogg_dec* e, const ogg_page* pg, int last) {
    if (e->fatal) return;
    if (pg->flags & 2) note_stream(e, pg);
    if (e->ended) return;
    if (!e->selected) {
        const char* c;
        if (!(pg->flags & 2)) return;
        c = e->streams[e->nstreams - 1].serial == pg->serial ? e->streams[e->nstreams - 1].codec : "";
        if ((strcmp(c, "vorbis") == 0 && e->want != CODEC_OPUS) || (strcmp(c, "opus") == 0 && e->want != CODEC_VORBIS)) {
            e->selected = 1; e->serial = pg->serial; e->codec = c[0] == 'v' ? CODEC_VORBIS : CODEC_OPUS;
        } else return;
    }
    if (pg->serial != e->serial) return;
    if (e->have_seq && pg->seq != e->next_seq) {        /* lost pages */
        e->gaps++; e->plen = 0; e->pkt_open = 0;
    }
    e->have_seq = 1; e->next_seq = pg->seq + 1;
    if (e->codec == CODEC_VORBIS && e->nhdr < 3) {
        if (!grow((void**)&e->hdr, &e->hdrcap, e->hdrlen + pg->len, 1)) { fail(e, "Ogg: out of memory"); return; }
        memcpy(e->hdr + e->hdrlen, pg->p, pg->len); e->hdrlen += pg->len;
        page_packets(e, pg);
        if (e->nhdr >= 3 && !e->fatal) {
            vorbis_open(e);
            e->ready = !e->fatal;
        }
    } else if (e->codec == CODEC_VORBIS) {
        if (!grow((void**)&e->vb, &e->vbcap, e->vblen + pg->len, 1)) { fail(e, "Ogg: out of memory"); return; }
        memcpy(e->vb + e->vblen, pg->p, pg->len); e->vblen += pg->len;
        vorbis_drain(e);
        page_done(e, pg->granule, last || (pg->flags & 4));
    } else {
        int audio = e->nhdr >= 2;
        page_packets(e, pg);
        if (audio) page_done(e, pg->granule, last || (pg->flags & 4));
    }
    if (pg->flags & 4) e->ended = 1;
}

static ogg_dec* eng_new(int want) {
    ogg_dec* e = (ogg_dec*)calloc(1, sizeof(ogg_dec));
    if (!e) return NULL;
    e->want = want; e->seek_target = -1; e->last_granule = -1; e->end_granule = -1;
    return e;
}

void trc_ogg_free(void* h) {
    ogg_dec* e = (ogg_dec*)h; int i;
    if (!e) return;
    codec_free(e);
    free(e->pkt); free(e->vendor);
    if (e->cm) for (i = 0; i < e->ncm; i++) free(e->cm[i]);
    free(e->cm); free(e->cmlen); free(e->valloc.alloc_buffer); free(e->vb); free(e->hdr); free(e->obuf); free(e->q); free(e->in);
    free(e->data); free(e->idx); free(e);
}

/* ── streaming decoder ── */
void* trc_ogg_new(int64_t want) { return eng_new((int)want); }

/* Append bytes; decode every complete page. 0 ok, -1 fatal (see error). */
int64_t trc_ogg_feed(void* h, const char* data, int64_t n) {
    ogg_dec* e = (ogg_dec*)h; size_t pos = 0; ogg_page pg;
    if (!e) return -1;
    if (e->fatal) return -1;
    if (n > 0) {
        if (!grow((void**)&e->in, &e->incap, e->inlen + (size_t)n, 1)) { fail(e, "Ogg: out of memory"); return -1; }
        memcpy(e->in + e->inlen, data, (size_t)n); e->inlen += (size_t)n;
    }
    while (!e->fatal && ogg_next_page(e->in, e->inlen, &pos, &pg, &e->bad_pages)) eng_page(e, &pg, 0);
    if (pos > 0) { memmove(e->in, e->in + pos, e->inlen - pos); e->inlen -= pos; }
    return e->fatal ? -1 : 0;
}
/* End of input: trim to the last granule position seen. */
int64_t trc_ogg_finish(void* h) {
    ogg_dec* e = (ogg_dec*)h;
    if (!e) return -1;
    if (e->fatal) return -1;
    if (!e->ended && e->last_granule >= 0) trim_tail(e, e->last_granule);
    e->ended = 1;
    if (!e->selected) { fail(e, e->nstreams ? "Ogg: no Vorbis or Opus stream found" : "Ogg: not an Ogg stream"); return -1; }
    if (!e->ready) { fail(e, "Ogg: stream ended before its headers were complete"); return -1; }
    return 0;
}
int64_t trc_ogg_available(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? (int64_t)(e->qlen - e->qhead) : 0; }
/* Copy up to max frames as interleaved doubles; returns frames copied. */
int64_t trc_ogg_pull(void* h, double* out, int64_t max) {
    ogg_dec* e = (ogg_dec*)h; int64_t n, i; size_t ch;
    if (!e || max <= 0) return 0;
    n = (int64_t)(e->qlen - e->qhead);
    if (n > max) n = max;
    ch = (size_t)e->channels;
    for (i = 0; i < n * (int64_t)ch; i++) out[i] = e->q[e->qhead * ch + (size_t)i];
    e->qhead += (size_t)n; e->qhead_dec += n;
    if (e->qhead == e->qlen) e->qhead = e->qlen = 0;
    return n;
}

/* ── accessors (streaming decoder and reader) ── */
int64_t trc_ogg_ready(void* h) { ogg_dec* e = (ogg_dec*)h; return e && e->ready && !e->fatal; }
int64_t trc_ogg_ended(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->ended : 1; }
int64_t trc_ogg_failed(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->fatal : 1; }
int64_t trc_ogg_codec(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->codec : 0; }
int64_t trc_ogg_channels(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->channels : 0; }
int64_t trc_ogg_rate(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->rate : 0; }
int64_t trc_ogg_serial(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? (int64_t)e->serial : 0; }
int64_t trc_ogg_pre_skip(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->pre_skip : 0; }
int64_t trc_ogg_gain_q8(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->gain_q8 : 0; }
int64_t trc_ogg_family(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->family : 0; }
int64_t trc_ogg_bitrate(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->bitrate_nominal : 0; }
int64_t trc_ogg_bad_pages(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->bad_pages : 0; }
int64_t trc_ogg_gaps(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->gaps : 0; }
int64_t trc_ogg_bad_packets(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->bad_packets : 0; }
int64_t trc_ogg_comment_count(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->ncm : 0; }
/* Copies at most cap bytes of comment i (i = -1: the vendor string); returns its full length. */
int64_t trc_ogg_comment(void* h, int64_t i, char* out, int64_t cap) {
    ogg_dec* e = (ogg_dec*)h; const char* s; int64_t l;
    if (!e) return 0;
    if (i == -1) { s = e->vendor ? e->vendor : ""; l = (int64_t)strlen(s); }
    else if (i >= 0 && i < e->ncm) { s = e->cm[i]; l = e->cmlen[i]; }
    else return 0;
    if (out && cap > 0) memcpy(out, s, (size_t)(l < cap ? l : cap));
    return l;
}
int64_t trc_ogg_comment_len(void* h, int64_t i) { return trc_ogg_comment(h, i, NULL, 0); }
int64_t trc_ogg_stream_count(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->nstreams : 0; }
int64_t trc_ogg_stream_serial(void* h, int64_t i) { ogg_dec* e = (ogg_dec*)h; return e && i >= 0 && i < e->nstreams ? (int64_t)e->streams[i].serial : 0; }
int64_t trc_ogg_stream_codec(void* h, int64_t i, char* out, int64_t cap) {
    ogg_dec* e = (ogg_dec*)h; int64_t l;
    if (!e || i < 0 || i >= e->nstreams) return 0;
    l = (int64_t)strlen(e->streams[i].codec);
    if (l > cap) l = cap;
    memcpy(out, e->streams[i].codec, (size_t)l);
    return l;
}
int64_t trc_ogg_error(void* h, char* out, int64_t cap) {
    ogg_dec* e = (ogg_dec*)h; size_t n;
    if (!e) { memcpy(out, "out of memory", 13); return 13; }
    n = strlen(e->err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, e->err, n); return (int64_t)n;
}

/* ── seekable in-memory reader ── */
static void reader_feed_page(ogg_dec* e, size_t k) {
    ogg_page pg; size_t pos = e->idx[k].off;
    int64_t dummy = 0;
    if (!ogg_next_page(e->data, e->dlen, &pos, &pg, &dummy)) return;
    eng_page(e, &pg, k + 1 == e->nidx);
}
/* Decode pages until at least `frames` are queued (or the stream ends). */
static void reader_fill(ogg_dec* e, int64_t frames) {
    while (!e->fatal && e->feed_next < e->nidx && !e->ended &&
           ((int64_t)(e->qlen - e->qhead) < frames || !e->base_known || e->seek_target >= 0)) {
        reader_feed_page(e, e->feed_next++);
    }
}

void* trc_oggr_open(const char* data, int64_t n, int64_t want) {
    ogg_dec* e = eng_new((int)want); size_t pos = 0; ogg_page pg; int64_t bad = 0;
    if (!e) return NULL;
    if (n < 0) n = 0;
    e->data = (uint8_t*)malloc((size_t)n + 1);
    if (!e->data) { fail(e, "Ogg: out of memory"); return e; }
    memcpy(e->data, data, (size_t)n); e->dlen = (size_t)n;
    /* pass 1: headers and stream selection */
    while (!e->fatal && !e->ready && ogg_next_page(e->data, e->dlen, &pos, &pg, &e->bad_pages)) {
        eng_page(e, &pg, 0);
    }
    if (e->fatal) return e;
    if (!e->selected) { fail(e, e->nstreams ? "Ogg: no Vorbis or Opus stream found" : "Ogg: not an Ogg stream"); return e; }
    if (!e->ready) { fail(e, "Ogg: stream ended before its headers were complete"); return e; }
    /* pass 2: index the selected stream's pages after the headers */
    while (ogg_next_page(e->data, e->dlen, &pos, &pg, &bad)) {
        if (pg.flags & 2) note_stream(e, &pg);
        if (pg.serial != e->serial) continue;
        if (!grow((void**)&e->idx, &e->idxcap, e->nidx + 1, sizeof(ogg_index_ent))) { fail(e, "Ogg: out of memory"); return e; }
        e->idx[e->nidx].off = (size_t)(pg.p - e->data); e->idx[e->nidx].len = pg.len;
        e->idx[e->nidx].granule = pg.granule; e->idx[e->nidx].flags = pg.flags;
        e->nidx++;
        if (pg.granule >= 0) e->end_granule = pg.granule;
        if (pg.flags & 4) break;
    }
    e->bad_pages += bad;
    e->feed_next = 0;
    /* establish the timeline (decodes up to the first page with a granule) */
    reader_fill(e, 1);
    if (e->fatal) return e;
    if (!e->base_known) { e->gran_base = 0; e->base_known = 1; }
    {
        int64_t first = e->gran_base, end = e->end_granule - pre_skip_of(e);
        if (e->codec == CODEC_VORBIS && first < 0) first = 0;
        e->first_pcm = first;
        e->total_frames = e->end_granule >= 0 ? end - first : (int64_t)(e->qlen - e->qhead);
        if (e->total_frames < 0) e->total_frames = 0;
    }
    return e;
}
int64_t trc_oggr_total(void* h) { ogg_dec* e = (ogg_dec*)h; return e ? e->total_frames : 0; }
/* Current frame (0 = first output frame). */
int64_t trc_oggr_tell(void* h) {
    ogg_dec* e = (ogg_dec*)h;
    if (!e || !e->base_known) return 0;
    return e->gran_base + e->qhead_dec - pre_skip_of(e) - e->first_pcm;
}
int64_t trc_oggr_read(void* h, double* out, int64_t max) {
    ogg_dec* e = (ogg_dec*)h;
    if (!e || e->fatal) return 0;
    reader_fill(e, max);
    return trc_ogg_pull(h, out, max);
}
/* Position so the next read starts at `frame`. 0 ok, -1 failure. */
int64_t trc_oggr_seek(void* h, int64_t frame) {
    ogg_dec* e = (ogg_dec*)h; int64_t target, preroll; size_t k, start = 0, last_mid = 0; int found = 0;
    if (!e || e->fatal || !e->ready) return -1;
    if (frame < 0) frame = 0;
    if (frame > e->total_frames) frame = e->total_frames;
    target = e->first_pcm + frame;
    preroll = e->codec == CODEC_OPUS ? 3840 + 5760 : 8192;
    /* The timeline after a restart is anchored on the first page with a
       granule; the final page cannot serve (its granule is end-trimmed), so a
       restart point must leave a non-final granule page ahead of it. */
    for (k = 0; k + 1 < e->nidx; k++) if (e->idx[k].granule >= 0) last_mid = k;
    for (k = 0; k < last_mid; k++) {
        int64_t g = e->idx[k].granule;
        if (g < 0) continue;
        if (g - pre_skip_of(e) <= target - preroll) { start = k + 1; found = 1; }
        else break;
    }
    /* reset the decoder */
    e->qhead = e->qlen = 0; e->plen = 0; e->pkt_open = 0; e->drop_cont = 0; e->vblen = 0;
    e->dec_total = 0; e->qhead_dec = 0; e->base_known = 0; e->ended = 0; e->have_seq = 0;
    e->skip = 0; e->seek_target = target;
    if (e->codec == CODEC_OPUS) opus_multistream_decoder_ctl(e->od, OPUS_RESET_STATE);
    else {
        if (e->v) { stb_vorbis_close(e->v); e->v = NULL; }
        vorbis_open(e);
        if (e->fatal) return -1;
    }
    if (!found) start = 0;      /* a leading continuation is dropped by page_packets / stb_vorbis */
    e->feed_next = start;
    reader_fill(e, 1);
    if (!e->base_known) {   /* nothing decoded after the seek point */
        e->base_known = 1; e->seek_target = -1; e->gran_base = e->end_granule >= 0 ? e->end_granule : 0;
        e->qhead_dec = 0;
    }
    return e->fatal ? -1 : 0;
}

/* ── one-shot ── */
void* trc_ogg_decode(const char* data, int64_t n, int64_t want) {
    au_result* r = (au_result*)calloc(1, sizeof(au_result)); ogg_dec* e; int64_t frames;
    if (!r) return NULL;
    e = eng_new((int)want);
    if (!e) { snprintf(r->err, sizeof r->err, "Ogg: out of memory"); return r; }
    if (trc_ogg_feed(e, data, n) != 0 || trc_ogg_finish(e) != 0) {
        snprintf(r->err, sizeof r->err, "%s", e->err); trc_ogg_free(e); return r;
    }
    frames = trc_ogg_available(e);
    r->data = (double*)malloc(sizeof(double) * (size_t)(frames * e->channels + 1));
    if (!r->data) { snprintf(r->err, sizeof r->err, "Ogg: out of memory"); trc_ogg_free(e); return r; }
    trc_ogg_pull(e, r->data, frames);
    r->frames = frames; r->channels = e->channels; r->rate = e->rate;
    r->bits = e->codec == CODEC_OPUS ? 16 : 0; r->is_float = 0;
    trc_ogg_free(e);
    return r;
}

/* ── raw container access: pages and packets ── */
typedef struct {
    ogg_page* pages; size_t npages, pcap;
    uint8_t** pk; size_t* pklen; int64_t* pkgran; uint32_t* pkserial; int* pkflags; size_t npk, pkcap;
    uint8_t* data; int64_t bad;
} ogg_raw;

void trc_ogg_raw_free(void* h) {
    ogg_raw* r = (ogg_raw*)h; size_t i;
    if (!r) return;
    for (i = 0; i < r->npk; i++) free(r->pk[i]);
    free(r->pk); free(r->pklen); free(r->pkgran); free(r->pkserial); free(r->pkflags); free(r->pages); free(r->data); free(r);
}

/* Parse every page; packets of `serial` (all streams if serial < 0) are reassembled.
   Packet flags: 2 first packet of its stream, 4 last packet of an EOS page. */
void* trc_ogg_raw(const char* data, int64_t n, int64_t serial) {
    ogg_raw* r = (ogg_raw*)calloc(1, sizeof(ogg_raw)); size_t pos = 0; ogg_page pg;
    struct { uint32_t serial; uint8_t* buf; size_t len, cap; int open, seen; } st[MAX_STREAMS]; int nst = 0, i;
    if (!r) return NULL;
    memset(st, 0, sizeof st);
    if (n < 0) n = 0;
    r->data = (uint8_t*)malloc((size_t)n + 1);
    if (!r->data) { free(r); return NULL; }
    memcpy(r->data, data, (size_t)n);
    while (ogg_next_page(r->data, (size_t)n, &pos, &pg, &r->bad)) {
        int s, k = -1; size_t off = 0, start = 0;
        if (grow((void**)&r->pages, &r->pcap, r->npages + 1, sizeof(ogg_page))) r->pages[r->npages++] = pg;
        if (serial >= 0 && pg.serial != (uint32_t)serial) continue;
        for (i = 0; i < nst; i++) if (st[i].serial == pg.serial) k = i;
        if (k < 0) { if (nst >= MAX_STREAMS) continue; k = nst++; st[k].serial = pg.serial; }
        if (!(pg.flags & 1)) st[k].len = 0, st[k].open = 0;
        else if (!st[k].open) st[k].len = (size_t)-1;          /* drop the orphan continuation */
        for (s = 0; s < pg.nseg; s++) {
            off += pg.lacing[s];
            if (pg.lacing[s] < 255 || s == pg.nseg - 1) {
                size_t seg = off - start;
                if (st[k].len != (size_t)-1) {
                    if (!grow((void**)&st[k].buf, &st[k].cap, st[k].len + seg + 1, 1)) break;
                    memcpy(st[k].buf + st[k].len, pg.body + start, seg); st[k].len += seg;
                }
                start = off;
                if (pg.lacing[s] < 255) {
                    if (st[k].len != (size_t)-1) {
                        size_t m = r->npk + 1;
                        if (!grow((void**)&r->pk, &r->pkcap, m, sizeof(uint8_t*))) break;
                        r->pklen = (size_t*)realloc(r->pklen, r->pkcap * sizeof(size_t));
                        r->pkgran = (int64_t*)realloc(r->pkgran, r->pkcap * sizeof(int64_t));
                        r->pkserial = (uint32_t*)realloc(r->pkserial, r->pkcap * sizeof(uint32_t));
                        r->pkflags = (int*)realloc(r->pkflags, r->pkcap * sizeof(int));
                        if (!r->pklen || !r->pkgran || !r->pkserial || !r->pkflags) break;
                        r->pk[r->npk] = (uint8_t*)malloc(st[k].len + 1);
                        if (!r->pk[r->npk]) break;
                        memcpy(r->pk[r->npk], st[k].buf, st[k].len);
                        r->pklen[r->npk] = st[k].len; r->pkserial[r->npk] = pg.serial;
                        {
                            int last_on_page = 1, t;
                            for (t = s + 1; t < pg.nseg; t++) if (pg.lacing[t] < 255) { last_on_page = 0; break; }
                            r->pkgran[r->npk] = last_on_page ? pg.granule : -1;
                            r->pkflags[r->npk] = (st[k].seen ? 0 : 2) | ((last_on_page && (pg.flags & 4)) ? 4 : 0);
                        }
                        st[k].seen = 1;
                        r->npk++;
                    }
                    st[k].len = 0;
                }
            }
        }
        st[k].open = pg.nseg > 0 && pg.lacing[pg.nseg - 1] == 255;
        if (!st[k].open) st[k].len = 0;
    }
    for (i = 0; i < nst; i++) free(st[i].buf);
    return r;
}
int64_t trc_ogg_raw_bad(void* h) { return h ? ((ogg_raw*)h)->bad : 0; }
int64_t trc_ogg_raw_pages(void* h) { return h ? (int64_t)((ogg_raw*)h)->npages : 0; }
/* field: 0 offset, 1 length, 2 flags, 3 granule, 4 serial, 5 sequence, 6 packets ending on it */
int64_t trc_ogg_raw_page(void* h, int64_t i, int64_t field) {
    ogg_raw* r = (ogg_raw*)h; ogg_page* p; int s, c = 0;
    if (!r || i < 0 || (size_t)i >= r->npages) return 0;
    p = &r->pages[i];
    switch (field) {
        case 0: return (int64_t)(p->p - r->data);
        case 1: return (int64_t)p->len;
        case 2: return p->flags;
        case 3: return p->granule;
        case 4: return (int64_t)p->serial;
        case 5: return (int64_t)p->seq;
        case 6: for (s = 0; s < p->nseg; s++) if (p->lacing[s] < 255) c++; return c;
    }
    return 0;
}
int64_t trc_ogg_raw_packets(void* h) { return h ? (int64_t)((ogg_raw*)h)->npk : 0; }
/* field: 0 length, 1 granule (-1 unless last on its page), 2 serial, 3 flags */
int64_t trc_ogg_raw_packet(void* h, int64_t i, int64_t field) {
    ogg_raw* r = (ogg_raw*)h;
    if (!r || i < 0 || (size_t)i >= r->npk) return 0;
    switch (field) {
        case 0: return (int64_t)r->pklen[i];
        case 1: return r->pkgran[i];
        case 2: return (int64_t)r->pkserial[i];
        case 3: return r->pkflags[i];
    }
    return 0;
}
void trc_ogg_raw_packet_copy(void* h, int64_t i, char* out) {
    ogg_raw* r = (ogg_raw*)h;
    if (r && i >= 0 && (size_t)i < r->npk) memcpy(out, r->pk[i], r->pklen[i]);
}
