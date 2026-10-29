#include "tauraro_types.h"

TrStr pj_line(TrStr blob, long long idx);
void pj_worker(_TrSharedBox* job);
long long pj_parse_int(TrStr s);

__attribute__((malloc,returns_nonnull,hot)) PendingShims* PendingShims_init() {
#line 111 "src/pjobs.tr"
    PendingShims* p = ((PendingShims*)_tr_obj_alloc(sizeof(PendingShims)));
#line 112 "src/pjobs.tr"
    p->cmds = (void*)List_TrStr_new();
#line 113 "src/pjobs.tr"
    p->names = (void*)List_TrStr_new();
#line 114 "src/pjobs.tr"
    p->logs = (void*)List_TrStr_new();
#line 115 "src/pjobs.tr"
    p->sidecars = (void*)List_TrStr_new();
#line 116 "src/pjobs.tr"
    p->keys = (void*)List_TrStr_new();
#line 117 "src/pjobs.tr"
    return p;
}

__attribute__((hot)) TrStr pj_line(TrStr blob, long long idx) {
#line 29 "src/pjobs.tr"
    List_TrStr* lines = _tr_str_splitv((blob), (_tr_str_lit_len("\n", 1LL)));
#line 30 "src/pjobs.tr"
    return List_TrStr_get(lines, idx);
}

__attribute__((hot)) void pj_worker(_TrSharedBox* job) {
#line 33 "src/pjobs.tr"
    while (true) {
#line 34 "src/pjobs.tr"
        long long i = _tr_atomic_add(((PJob*)(job->data))->next, (long long)(1LL));
#line 35 "src/pjobs.tr"
        if ((i >= _tr_atomic_load(((PJob*)(job->data))->total))) {
#line 35 "src/pjobs.tr"
            break;
        }
#line 36 "src/pjobs.tr"
        long long rc = ({ TrStr _aet_t602 = (pj_line(((PJob*)(job->data))->blob, i)); __auto_type _wr = (_tr_system(_aet_t602.data)); _tr_str_release(_aet_t602); _wr; });
#line 37 "src/pjobs.tr"
        ({ __auto_type _ord_t603 = (((PJob*)(job->data))->base); TrStr _at_t604 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv((_ord_t603), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".rc", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t605 = (_tr_str_wrap(_tr_int_to_str((long long)(rc)))); write_file(_at_t604, _at_t605); _tr_str_release(_at_t604); _tr_str_release(_at_t605); });
    }
#line 38 "src/pjobs.tr"
    _tr_atomic_add(((PJob*)(job->data))->done, (long long)(1LL));
}

__attribute__((hot)) long long pj_parse_int(TrStr s) {
#line 41 "src/pjobs.tr"
    long long n = 0LL;
#line 42 "src/pjobs.tr"
    long long i = 0LL;
#line 43 "src/pjobs.tr"
    if ((_tr_str_lenv((s)) == 0LL)) {
#line 43 "src/pjobs.tr"
        return (0LL - 1LL);
    }
#line 44 "src/pjobs.tr"
    while ((i < _tr_str_lenv((s)))) {
#line 45 "src/pjobs.tr"
        long long c = _tr_str_char_at_codev((s), i);
#line 46 "src/pjobs.tr"
        if (((c < 48LL) || (c > 57LL))) {
#line 46 "src/pjobs.tr"
            return (0LL - 1LL);
        }
#line 47 "src/pjobs.tr"
        n = ((n * 10LL) + (c - 48LL));
#line 48 "src/pjobs.tr"
        i = (i + 1LL);
    }
#line 49 "src/pjobs.tr"
    return n;
}

__attribute__((hot)) long long pjobs_workers(long long njobs) {
#line 53 "src/pjobs.tr"
    long long w = _tr_cpu_count();
#line 54 "src/pjobs.tr"
    long long env = ({ TrStr _at_t606 = (_tr_str_lit(_tr_getenv(_tr_strz(_tr_str_lit_len("TAURARO_JOBS", 12LL))))); __auto_type _wr = (pj_parse_int(_at_t606)); _tr_str_release(_at_t606); _wr; });
#line 55 "src/pjobs.tr"
    if ((env > 0LL)) {
#line 55 "src/pjobs.tr"
        w = env;
    }
#line 56 "src/pjobs.tr"
    if ((w > 16LL)) {
#line 56 "src/pjobs.tr"
        w = 16LL;
    }
#line 57 "src/pjobs.tr"
    if ((w < 1LL)) {
#line 57 "src/pjobs.tr"
        w = 1LL;
    }
#line 58 "src/pjobs.tr"
    if ((w > njobs)) {
#line 58 "src/pjobs.tr"
        w = njobs;
    }
#line 59 "src/pjobs.tr"
    return w;
}

__attribute__((hot)) List_i64* pjobs_run(List_TrStr* cmds, long long workers, TrStr rcbase) {
#line 64 "src/pjobs.tr"
    List_i64* rcs = (void*)List_i64_new();
#line 65 "src/pjobs.tr"
    long long k = 0LL;
#line 66 "src/pjobs.tr"
    if (((workers <= 1LL) || (cmds->len <= 1LL))) {
#line 67 "src/pjobs.tr"
        while ((k < cmds->len)) {
#line 68 "src/pjobs.tr"
            ({ TrStr _aet_t607 = (List_TrStr_get(cmds, k)); List_i64_append(rcs, _tr_system(_aet_t607.data)); _tr_str_release(_aet_t607); });
#line 69 "src/pjobs.tr"
            k = (k + 1LL);
        }
#line 70 "src/pjobs.tr"
        return rcs;
    }
#line 71 "src/pjobs.tr"
    TrStr blob = _tr_str_lit_len("", 0LL);
#line 72 "src/pjobs.tr"
    k = 0LL;
#line 73 "src/pjobs.tr"
    while ((k < cmds->len)) {
#line 74 "src/pjobs.tr"
        if ((k > 0LL)) {
#line 74 "src/pjobs.tr"
            ({ TrStr _ap_t608 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&blob, _ap_t608); });
        }
#line 75 "src/pjobs.tr"
        ({ TrStr _ap_t609 = (List_TrStr_get(cmds, k)); _tr_str_append(&blob, _ap_t609); _tr_str_release(_ap_t609); });
#line 76 "src/pjobs.tr"
        k = (k + 1LL);
    }
#line 77 "src/pjobs.tr"
    _TrSharedBox* job = _tr_shared_new(((PJob*)_tr_obj_alloc(sizeof(PJob))), _trdrop_PJob);
#line 78 "src/pjobs.tr"
    ((PJob*)(job->data))->next = _tr_atomic_new((long long)(0LL));
#line 79 "src/pjobs.tr"
    ((PJob*)(job->data))->total = _tr_atomic_new((long long)(cmds->len));
#line 80 "src/pjobs.tr"
    ((PJob*)(job->data))->blob = _tr_str_retain(blob);
#line 81 "src/pjobs.tr"
    ((PJob*)(job->data))->base = _tr_str_retain(rcbase);
#line 82 "src/pjobs.tr"
    ((PJob*)(job->data))->done = _tr_atomic_new((long long)(0LL));
#line 83 "src/pjobs.tr"
    k = 0LL;
#line 84 "src/pjobs.tr"
    while ((k < workers)) {
#line 85 "src/pjobs.tr"
        _TrThreadObj* t = _tr_threadobj_spawn(_tr_spawn_wrap_pj_worker, (void*)(job));
#line 86 "src/pjobs.tr"
        k = (k + 1LL);
    }
#line 87 "src/pjobs.tr"
    while ((_tr_atomic_load(((PJob*)(job->data))->done) < workers)) {
#line 88 "src/pjobs.tr"
        _tr_thread_sleep_ms((long long)(1LL));
    }
#line 89 "src/pjobs.tr"
    k = 0LL;
#line 90 "src/pjobs.tr"
    while ((k < cmds->len)) {
#line 91 "src/pjobs.tr"
        TrStr rf = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(k)))); TrStr _tr_cres = _tr_strx_concatv((rcbase), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".rc", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 92 "src/pjobs.tr"
        if (file_exists(rf)) {
#line 93 "src/pjobs.tr"
            ({ TrStr _at_t610 = (read_file(rf)); List_i64_append(rcs, pj_parse_int(_at_t610)); _tr_str_release(_at_t610); });
#line 94 "src/pjobs.tr"
            _tr_file_delete(_tr_strz(rf));
        } else {
#line 96 "src/pjobs.tr"
            List_i64_append(rcs, (0LL - 1LL));
        }
#line 97 "src/pjobs.tr"
        k = (k + 1LL);
        _tr_str_release(rf);
    }
#line 98 "src/pjobs.tr"
    _tr_str_release(blob);
    _tr_shared_drop(job);
    return rcs;
}

