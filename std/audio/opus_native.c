/*
 * std/audio/opus_native.c -- the libopus 1.5.2 decoder, built as one translation unit.
 *
 * Linked through `# tauraro-cpp-shim: opus_native.c` in std/audio/opus_native.tr.
 * Only the decoder half of libopus is vendored (vendor/opus/, flattened from
 * celt/, silk/, src/ and include/ so that no -I flag is needed; see
 * vendor/README.md). Float build, generic C only: no SIMD, no assembly, no
 * run-time CPU detection, no DNN (deep PLC / DRED / OSCE) code.
 * std/audio/ogg_native.c calls the public opus_multistream_* API.
 *
 * vendor revision: libopus 1.5.2 decoder, 66 C files
 */
#define OPUS_BUILD 1
#define FLOATING_POINT 1
#define USE_ALLOCA 1
#define HAVE_LRINTF 1
#define HAVE_LRINT 1
#define PACKAGE_VERSION "1.5.2"
/* Unity build: opus_custom.h must declare the CELT decoder entry points for
   every file, not only for celt_decoder.c. */
#define CELT_DECODER_C

#include "vendor/opus/bands.c"
#include "vendor/opus/biquad_alt.c"
#include "vendor/opus/bwexpander_32.c"
#include "vendor/opus/bwexpander.c"
#include "vendor/opus/celt.c"
#include "vendor/opus/celt_decoder.c"
#include "vendor/opus/celt_lpc.c"
#include "vendor/opus/CNG.c"
#include "vendor/opus/code_signs.c"
#include "vendor/opus/cwrs.c"
#include "vendor/opus/dec_API.c"
#include "vendor/opus/decode_core.c"
#include "vendor/opus/decode_frame.c"
#include "vendor/opus/decode_indices.c"
#include "vendor/opus/decode_parameters.c"
#include "vendor/opus/decode_pitch.c"
#include "vendor/opus/decode_pulses.c"
#include "vendor/opus/decoder_set_fs.c"
#include "vendor/opus/entcode.c"
#include "vendor/opus/entdec.c"
#include "vendor/opus/entenc.c"
#include "vendor/opus/gain_quant.c"
#include "vendor/opus/init_decoder.c"
#include "vendor/opus/kiss_fft.c"
#include "vendor/opus/laplace.c"
#include "vendor/opus/lin2log.c"
#include "vendor/opus/log2lin.c"
#include "vendor/opus/LPC_analysis_filter.c"
#include "vendor/opus/LPC_fit.c"
#include "vendor/opus/LPC_inv_pred_gain.c"
#include "vendor/opus/mathops.c"
#include "vendor/opus/mdct.c"
#include "vendor/opus/modes.c"
#undef QA     /* LPC_inv_pred_gain.c and NLSF2A.c each define their own */
#include "vendor/opus/NLSF2A.c"
#include "vendor/opus/NLSF_decode.c"
#include "vendor/opus/NLSF_stabilize.c"
#include "vendor/opus/NLSF_unpack.c"
#include "vendor/opus/opus.c"
#include "vendor/opus/opus_decoder.c"
#include "vendor/opus/opus_multistream.c"
#include "vendor/opus/opus_multistream_decoder.c"
#include "vendor/opus/pitch.c"
#include "vendor/opus/pitch_est_tables.c"
#include "vendor/opus/PLC.c"
#include "vendor/opus/quant_bands.c"
#include "vendor/opus/rate.c"
#include "vendor/opus/resampler.c"
#include "vendor/opus/resampler_private_AR2.c"
#include "vendor/opus/resampler_private_down_FIR.c"
#include "vendor/opus/resampler_private_IIR_FIR.c"
#include "vendor/opus/resampler_private_up2_HQ.c"
#include "vendor/opus/resampler_rom.c"
#include "vendor/opus/shell_coder.c"
#include "vendor/opus/sort.c"
#include "vendor/opus/stereo_decode_pred.c"
#include "vendor/opus/stereo_MS_to_LR.c"
#include "vendor/opus/sum_sqr_shift.c"
#include "vendor/opus/table_LSF_cos.c"
#include "vendor/opus/tables_gain.c"
#include "vendor/opus/tables_LTP.c"
#include "vendor/opus/tables_NLSF_CB_NB_MB.c"
#include "vendor/opus/tables_NLSF_CB_WB.c"
#include "vendor/opus/tables_other.c"
#include "vendor/opus/tables_pitch_lag.c"
#include "vendor/opus/tables_pulses_per_block.c"
#include "vendor/opus/vq.c"
