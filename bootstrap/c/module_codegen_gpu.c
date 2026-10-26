#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) GpuGenerator* GpuGenerator_init() {
#line 32 "src/codegen/gpu/mod.tr"
    GpuGenerator* g = ((GpuGenerator*)_tr_obj_alloc(sizeof(GpuGenerator)));
#line 33 "src/codegen/gpu/mod.tr"
    g->ok = true;
#line 34 "src/codegen/gpu/mod.tr"
    g->fail_note = _tr_str_lit_len("", 0LL);
#line 35 "src/codegen/gpu/mod.tr"
    g->n_kernels = 0LL;
#line 36 "src/codegen/gpu/mod.tr"
    return g;
}

__attribute__((hot)) TrStr GpuGenerator_emit(GpuGenerator* self, HirProgram* prog, TrStr target) {
#line 41 "src/codegen/gpu/mod.tr"
    TrStr tgt = _tr_str_retain(target);
#line 42 "src/codegen/gpu/mod.tr"
    if (_tr_str_eqv((tgt), (_tr_str_lit_len("", 0LL)))) {
#line 42 "src/codegen/gpu/mod.tr"
        TrStr _strtmp_t6657 = _tr_str_lit_len("spirv", 5LL);
        _tr_str_release(tgt);
        tgt = _strtmp_t6657;
    }
#line 43 "src/codegen/gpu/mod.tr"
    StringBuilder* sb = StringBuilder_init(2048LL);
#line 45 "src/codegen/gpu/mod.tr"
    if (_tr_str_eqv((tgt), (_tr_str_lit_len("nvptx", 5LL)))) {
#line 46 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("; Tauraro GPU kernels -> NVPTX (CUDA PTX)\n", 42LL));
#line 47 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("target triple = \"nvptx64-nvidia-cuda\"\n", 38LL));
#line 48 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("target datalayout = \"e-i64:64-i128:128-v16:16-v32:32-n16:32:64\"\n\n", 65LL));
#line 49 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.tid.x()\n", 45LL));
#line 50 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.tid.y()\n", 45LL));
#line 51 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.tid.z()\n", 45LL));
#line 52 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ntid.x()\n", 46LL));
#line 53 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ntid.y()\n", 46LL));
#line 54 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ntid.z()\n", 46LL));
#line 55 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.x()\n", 47LL));
#line 56 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.y()\n", 47LL));
#line 57 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.z()\n", 47LL));
#line 58 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare i32 @llvm.nvvm.read.ptx.sreg.nctaid.x()\n", 48LL));
#line 59 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare void @llvm.nvvm.barrier0()\n\n", 36LL));
    } else {
#line 61 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("; Tauraro GPU kernels -> SPIR-V (OpenCL)\n", 41LL));
#line 62 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("target triple = \"spirv64-unknown-unknown\"\n\n", 43LL));
#line 63 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z13get_global_idj(i32)\n", 47LL));
#line 64 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z12get_local_idj(i32)\n", 46LL));
#line 65 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z12get_group_idj(i32)\n", 46LL));
#line 66 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z14get_local_sizej(i32)\n", 48LL));
#line 67 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z15get_global_sizej(i32)\n", 49LL));
#line 68 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func i64 @_Z14get_num_groupsj(i32)\n", 48LL));
#line 69 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("declare spir_func void @_Z7barrierj(i32)\n\n", 42LL));
    }
#line 72 "src/codegen/gpu/mod.tr"
    List_ptr* dev_fns = (void*)List_ptr_new();
#line 73 "src/codegen/gpu/mod.tr"
    long long df = 0LL;
#line 74 "src/codegen/gpu/mod.tr"
    while ((df < prog->functions->len)) {
#line 75 "src/codegen/gpu/mod.tr"
        HirFunction* fd = ((HirFunction*)List_ptr_get(prog->functions, df));
#line 76 "src/codegen/gpu/mod.tr"
        if (fn_is_device(fd)) {
#line 76 "src/codegen/gpu/mod.tr"
            List_ptr_append(dev_fns, _tr_obj_retain(fd));
        }
#line 77 "src/codegen/gpu/mod.tr"
        df = (df + 1LL);
    }
#line 80 "src/codegen/gpu/mod.tr"
    long long de = 0LL;
#line 81 "src/codegen/gpu/mod.tr"
    while ((de < dev_fns->len)) {
#line 82 "src/codegen/gpu/mod.tr"
        GpuEmitter* e0 = GpuEmitter_init(tgt);
#line 83 "src/codegen/gpu/mod.tr"
        e0->dev_fns = dev_fns;
#line 84 "src/codegen/gpu/mod.tr"
        ({ TrStr _sbt_t6658 = (GpuEmitter_emit_device_fn(e0, ((HirFunction*)List_ptr_get(dev_fns, de)))); StringBuilder_append(sb, _sbt_t6658); _tr_str_release(_sbt_t6658); });
#line 85 "src/codegen/gpu/mod.tr"
        if ((!e0->ok)) {
#line 86 "src/codegen/gpu/mod.tr"
            self->ok = false;
#line 87 "src/codegen/gpu/mod.tr"
            if (_tr_str_eqv((self->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 87 "src/codegen/gpu/mod.tr"
                self->fail_note = ({ __auto_type _ord_t6659 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("device fn '", 11LL)), (((HirFunction*)List_ptr_get(dev_fns, de))->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t6659); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (e0->fail_note)); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
            }
        }
#line 88 "src/codegen/gpu/mod.tr"
        de = (de + 1LL);
        _tr_obj_release(e0, _trdrop_GpuEmitter);
    }
#line 90 "src/codegen/gpu/mod.tr"
    long long fi = 0LL;
#line 91 "src/codegen/gpu/mod.tr"
    while ((fi < prog->functions->len)) {
#line 92 "src/codegen/gpu/mod.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, fi));
#line 93 "src/codegen/gpu/mod.tr"
        if (fn_is_kernel(f)) {
#line 94 "src/codegen/gpu/mod.tr"
            self->n_kernels = (self->n_kernels + 1LL);
#line 95 "src/codegen/gpu/mod.tr"
            GpuEmitter* e = GpuEmitter_init(tgt);
#line 96 "src/codegen/gpu/mod.tr"
            e->dev_fns = dev_fns;
#line 97 "src/codegen/gpu/mod.tr"
            ({ TrStr _sbt_t6660 = (GpuEmitter_emit_kernel(e, f)); StringBuilder_append(sb, _sbt_t6660); _tr_str_release(_sbt_t6660); });
#line 98 "src/codegen/gpu/mod.tr"
            if ((!e->ok)) {
#line 99 "src/codegen/gpu/mod.tr"
                self->ok = false;
#line 100 "src/codegen/gpu/mod.tr"
                if (_tr_str_eqv((self->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 100 "src/codegen/gpu/mod.tr"
                    self->fail_note = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("kernel '", 8LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (e->fail_note)); _tr_str_release(_tr_cl); _tr_cres; });
                }
            }
        }
#line 101 "src/codegen/gpu/mod.tr"
        fi = (fi + 1LL);
    }
#line 103 "src/codegen/gpu/mod.tr"
    if ((self->n_kernels == 0LL)) {
#line 104 "src/codegen/gpu/mod.tr"
        self->ok = false;
#line 105 "src/codegen/gpu/mod.tr"
        if (_tr_str_eqv((self->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 105 "src/codegen/gpu/mod.tr"
            self->fail_note = _tr_str_lit_len("no @kernel functions found", 26LL);
        }
    }
#line 108 "src/codegen/gpu/mod.tr"
    if ((_tr_str_eqv((tgt), (_tr_str_lit_len("nvptx", 5LL))) && (self->n_kernels > 0LL))) {
#line 109 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("\n!nvvm.annotations = !{", 23LL));
#line 110 "src/codegen/gpu/mod.tr"
        long long fi2 = 0LL;
#line 111 "src/codegen/gpu/mod.tr"
        long long ki = 0LL;
#line 112 "src/codegen/gpu/mod.tr"
        while ((fi2 < prog->functions->len)) {
#line 113 "src/codegen/gpu/mod.tr"
            HirFunction* f2 = ((HirFunction*)List_ptr_get(prog->functions, fi2));
#line 114 "src/codegen/gpu/mod.tr"
            if (fn_is_kernel(f2)) {
#line 115 "src/codegen/gpu/mod.tr"
                if ((ki > 0LL)) {
#line 115 "src/codegen/gpu/mod.tr"
                    StringBuilder_append(sb, _tr_str_lit_len(", ", 2LL));
                }
#line 116 "src/codegen/gpu/mod.tr"
                ({ TrStr _sbt_t6661 = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ki)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("!", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(sb, _sbt_t6661); _tr_str_release(_sbt_t6661); });
#line 117 "src/codegen/gpu/mod.tr"
                ki = (ki + 1LL);
            }
#line 118 "src/codegen/gpu/mod.tr"
            fi2 = (fi2 + 1LL);
        }
#line 119 "src/codegen/gpu/mod.tr"
        StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 120 "src/codegen/gpu/mod.tr"
        long long fi3 = 0LL;
#line 121 "src/codegen/gpu/mod.tr"
        long long ki2 = 0LL;
#line 122 "src/codegen/gpu/mod.tr"
        while ((fi3 < prog->functions->len)) {
#line 123 "src/codegen/gpu/mod.tr"
            HirFunction* f3 = ((HirFunction*)List_ptr_get(prog->functions, fi3));
#line 124 "src/codegen/gpu/mod.tr"
            if (fn_is_kernel(f3)) {
#line 125 "src/codegen/gpu/mod.tr"
                ({ __auto_type _ord_t6662 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ki2)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("!", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = !{ptr @", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6663 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6662); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f3->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", !\"kernel\", i32 1}\n", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t6663); _tr_str_release(_sbt_t6663); });
#line 126 "src/codegen/gpu/mod.tr"
                ki2 = (ki2 + 1LL);
            }
#line 127 "src/codegen/gpu/mod.tr"
            fi3 = (fi3 + 1LL);
        }
    }
#line 129 "src/codegen/gpu/mod.tr"
    TrStr _trret1830 = StringObj_as_str(StringBuilder_to_string(sb));
    _tr_str_release(tgt);
    StringBuilder__tr_fn_free(sb);
    return _trret1830;
}

