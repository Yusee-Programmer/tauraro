#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) GpuGenerator* GpuGenerator_init() {
    /* pass */
    GpuGenerator* g = ((GpuGenerator*)_tr_obj_alloc(sizeof(GpuGenerator)));
    /* pass */
    g->ok = true;
    /* pass */
    g->fail_note = _tr_str_lit_len("", 0LL);
    /* pass */
    g->n_kernels = 0LL;
    /* pass */
    return g;
}

__attribute__((hot)) TrStr GpuGenerator_emit(GpuGenerator* self, HirProgram* prog, TrStr target) {
    /* pass */
    TrStr tgt = _tr_str_retain(target);
    /* pass */
    if (_tr_str_eqv((tgt), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        TrStr _strtmp_t3527 = _tr_str_lit_len("spirv", 5LL);
        _tr_str_release(tgt);
        tgt = _strtmp_t3527;
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init(2048LL);
    /* pass */
    if (_tr_str_eqv((tgt), (_tr_str_lit_len("nvptx", 5LL)))) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("; Tauraro GPU kernels -> NVPTX (CUDA PTX)\n", 42LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("target triple = \"nvptx64-nvidia-cuda\"\n", 38LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("target datalayout = \"e-i64:64-i128:128-v16:16-v32:32-n16:32:64\"\n\n", 65LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.tid.x()\n", 45LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.tid.y()\n", 45LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.tid.z()\n", 45LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ntid.x()\n", 46LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ntid.y()\n", 46LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ntid.z()\n", 46LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.x()\n", 47LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.y()\n", 47LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.z()\n", 47LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.nctaid.x()\n", 48LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare void @llvm.nvvm.barrier0()\n\n", 36LL));
    } else {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("; Tauraro GPU kernels -> SPIR-V (OpenCL)\n", 41LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("target triple = \"spirv64-unknown-unknown\"\n\n", 43LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z13get_global_idj(i32)\n", 47LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z12get_local_idj(i32)\n", 46LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z12get_group_idj(i32)\n", 46LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z14get_local_sizej(i32)\n", 48LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z15get_global_sizej(i32)\n", 49LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z14get_num_groupsj(i32)\n", 48LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func void @_Z7barrierj(i32)\n\n", 42LL));
    }
    /* pass */
    List_ptr* dev_fns = (void*)List_ptr_new();
    /* pass */
    long long df = 0LL;
    /* pass */
    while ((df < prog->functions->len)) {
        /* pass */
        HirFunction* fd = ((HirFunction*)List_ptr_get(prog->functions, df));
        /* pass */
        if (fn_is_device(fd)) {
            /* pass */
            List_ptr_append(dev_fns, _tr_obj_retain(fd));
        }
        /* pass */
        df = (df + 1LL);
    }
    /* pass */
    long long de = 0LL;
    /* pass */
    while ((de < dev_fns->len)) {
        /* pass */
        GpuEmitter* e0 = GpuEmitter_init(tgt);
        /* pass */
        e0->dev_fns = dev_fns;
        /* pass */
        ({ TrStr _sbt_t3528 = (GpuEmitter_emit_device_fn(e0, ((HirFunction*)List_ptr_get(dev_fns, de)))); StringBuilder_append(sb, _sbt_t3528); _tr_str_release(_sbt_t3528); });
        /* pass */
        if ((!e0->ok)) {
            /* pass */
            self->ok = false;
            /* pass */
            if (_tr_str_eqv((self->fail_note), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                self->fail_note = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("device fn '", 11LL)), (((HirFunction*)List_ptr_get(dev_fns, de))->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (e0->fail_note)); _tr_str_release(_cl); _cres; });
            }
        }
        /* pass */
        de = (de + 1LL);
        _tr_obj_release(e0, _trdrop_GpuEmitter);
    }
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, fi));
        /* pass */
        if (fn_is_kernel(f)) {
            /* pass */
            self->n_kernels = (self->n_kernels + 1LL);
            /* pass */
            GpuEmitter* e = GpuEmitter_init(tgt);
            /* pass */
            e->dev_fns = dev_fns;
            /* pass */
            ({ TrStr _sbt_t3529 = (GpuEmitter_emit_kernel(e, f)); StringBuilder_append(sb, _sbt_t3529); _tr_str_release(_sbt_t3529); });
            /* pass */
            if ((!e->ok)) {
                /* pass */
                self->ok = false;
                /* pass */
                if (_tr_str_eqv((self->fail_note), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    self->fail_note = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("kernel '", 8LL)), (f->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (e->fail_note)); _tr_str_release(_cl); _cres; });
                }
            }
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    if ((self->n_kernels == 0LL)) {
        /* pass */
        self->ok = false;
        /* pass */
        if (_tr_str_eqv((self->fail_note), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            self->fail_note = _tr_str_lit_len("no @kernel functions found", 26LL);
        }
    }
    /* pass */
    if ((_tr_str_eqv((tgt), (_tr_str_lit_len("nvptx", 5LL))) && (self->n_kernels > 0LL))) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("\n!nvvm.annotations = !{", 23LL));
        /* pass */
        long long fi2 = 0LL;
        /* pass */
        long long ki = 0LL;
        /* pass */
        while ((fi2 < prog->functions->len)) {
            /* pass */
            HirFunction* f2 = ((HirFunction*)List_ptr_get(prog->functions, fi2));
            /* pass */
            if (fn_is_kernel(f2)) {
                /* pass */
                if ((ki > 0LL)) {
                    /* pass */
                    StringBuilder_append(sb, _tr_str_lit_len(", ", 2LL));
                }
                /* pass */
                ({ TrStr _sbt_t3530 = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ki)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("!", 1LL)), _cr); _tr_str_release(_cr); _cres; })); StringBuilder_append(sb, _sbt_t3530); _tr_str_release(_sbt_t3530); });
                /* pass */
                ki = (ki + 1LL);
            }
            /* pass */
            fi2 = (fi2 + 1LL);
        }
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
        /* pass */
        long long fi3 = 0LL;
        /* pass */
        long long ki2 = 0LL;
        /* pass */
        while ((fi3 < prog->functions->len)) {
            /* pass */
            HirFunction* f3 = ((HirFunction*)List_ptr_get(prog->functions, fi3));
            /* pass */
            if (fn_is_kernel(f3)) {
                /* pass */
                ({ TrStr _sbt_t3531 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ki2)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("!", 1LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = !{ptr @", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (f3->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", !\"kernel\", i32 1}\n", 20LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t3531); _tr_str_release(_sbt_t3531); });
                /* pass */
                ki2 = (ki2 + 1LL);
            }
            /* pass */
            fi3 = (fi3 + 1LL);
        }
    }
    /* pass */
    _tr_str_release(tgt);
    return StringObj_as_str(StringBuilder_to_string(sb));
}

