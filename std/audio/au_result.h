/*
 * std/audio/au_result.h -- the decoded-audio result shared by the std.audio
 * C shims (audio_native.c owns the trc_au_* accessors; ogg_native.c fills
 * the same struct so std.audio decodes Ogg through the same path).
 */
#ifndef TRC_AU_RESULT_H
#define TRC_AU_RESULT_H
#include <stdint.h>
typedef struct {
    double* data; int64_t frames; int channels; int rate; int bits; int is_float;
    char err[160];
} au_result;
#endif
