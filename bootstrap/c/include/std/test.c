#include "../../tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) TestRunner* TestRunner_init(TrStr suite_name) {
    /* pass */
    TestRunner* r = ((TestRunner*)_tr_obj_alloc(sizeof(TestRunner)));
    /* pass */
    r->name = _tr_str_retain(suite_name);
    /* pass */
    r->passed = 0LL;
    /* pass */
    r->failed = 0LL;
    /* pass */
    return r;
}

__attribute__((hot)) void TestRunner__pass(TestRunner* self) {
    /* pass */
    self->passed = (self->passed + 1LL);
}

__attribute__((hot)) void TestRunner__fail(TestRunner* self, TrStr msg) {
    /* pass */
    self->failed = (self->failed + 1LL);
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_wrap(({ int _fz = snprintf(NULL,0,"  FAIL: %s", _tr_strz(msg)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"  FAIL: %s", _tr_strz(msg)); _fr; })))); printf("\n"); });
}

__attribute__((hot)) void TestRunner_assert_true(TestRunner* self, bool cond, TrStr msg) {
    /* pass */
    if (cond) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected true, got false", _tr_strz(msg)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected true, got false", _tr_strz(msg)); _fr; })));
    }
}

__attribute__((hot)) void TestRunner_assert_false(TestRunner* self, bool cond, TrStr msg) {
    /* pass */
    if ((!cond)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected false, got true", _tr_strz(msg)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected false, got true", _tr_strz(msg)); _fr; })));
    }
}

__attribute__((hot)) void TestRunner_assert_eq_bool(TestRunner* self, bool got, bool want, TrStr msg) {
    /* pass */
    if ((got == want)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        TrStr gs = _tr_str_lit("false");
        /* pass */
        TrStr ws = _tr_str_lit("false");
        /* pass */
        if (got) {
            /* pass */
            TrStr _strtmp_t1 = _tr_str_lit("true");
            _tr_str_release(gs);
            gs = _strtmp_t1;
        }
        /* pass */
        if (want) {
            /* pass */
            TrStr _strtmp_t2 = _tr_str_lit("true");
            _tr_str_release(ws);
            ws = _strtmp_t2;
        }
        /* pass */
        TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected %s, got %s", _tr_strz(msg), _tr_strz(ws), _tr_strz(gs)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected %s, got %s", _tr_strz(msg), _tr_strz(ws), _tr_strz(gs)); _fr; })));
        _tr_str_release(gs);
        _tr_str_release(ws);
    }
}

__attribute__((hot)) void TestRunner_assert_eq_int(TestRunner* self, long long got, long long want, TrStr msg) {
    /* pass */
    if ((got == want)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        ({ TrStr _wt_t3 = (_tr_str_wrap(_tr_int_to_str(want))); TrStr _wt_t4 = (_tr_str_wrap(_tr_int_to_str(got))); TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected %s, got %s", _tr_strz(msg), _wt_t3.data, _wt_t4.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected %s, got %s", _tr_strz(msg), _wt_t3.data, _wt_t4.data); _fr; }))); _tr_str_release(_wt_t3); _tr_str_release(_wt_t4); });
    }
}

__attribute__((hot)) void TestRunner_assert_ne_int(TestRunner* self, long long a, long long b, TrStr msg) {
    /* pass */
    if ((a != b)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        ({ TrStr _wt_t5 = (_tr_str_wrap(_tr_int_to_str(a))); TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected values to differ (both are %s)", _tr_strz(msg), _wt_t5.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected values to differ (both are %s)", _tr_strz(msg), _wt_t5.data); _fr; }))); _tr_str_release(_wt_t5); });
    }
}

__attribute__((hot)) void TestRunner_assert_gt_int(TestRunner* self, long long a, long long b, TrStr msg) {
    /* pass */
    if ((a > b)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        ({ TrStr _wt_t6 = (_tr_str_wrap(_tr_int_to_str(a))); TrStr _wt_t7 = (_tr_str_wrap(_tr_int_to_str(b))); TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected %s > %s", _tr_strz(msg), _wt_t6.data, _wt_t7.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected %s > %s", _tr_strz(msg), _wt_t6.data, _wt_t7.data); _fr; }))); _tr_str_release(_wt_t6); _tr_str_release(_wt_t7); });
    }
}

__attribute__((hot)) void TestRunner_assert_lt_int(TestRunner* self, long long a, long long b, TrStr msg) {
    /* pass */
    if ((a < b)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        ({ TrStr _wt_t8 = (_tr_str_wrap(_tr_int_to_str(a))); TrStr _wt_t9 = (_tr_str_wrap(_tr_int_to_str(b))); TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected %s < %s", _tr_strz(msg), _wt_t8.data, _wt_t9.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected %s < %s", _tr_strz(msg), _wt_t8.data, _wt_t9.data); _fr; }))); _tr_str_release(_wt_t8); _tr_str_release(_wt_t9); });
    }
}

__attribute__((hot)) void TestRunner_assert_ge_int(TestRunner* self, long long a, long long b, TrStr msg) {
    /* pass */
    if ((a >= b)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        ({ TrStr _wt_t10 = (_tr_str_wrap(_tr_int_to_str(a))); TrStr _wt_t11 = (_tr_str_wrap(_tr_int_to_str(b))); TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected %s >= %s", _tr_strz(msg), _wt_t10.data, _wt_t11.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected %s >= %s", _tr_strz(msg), _wt_t10.data, _wt_t11.data); _fr; }))); _tr_str_release(_wt_t10); _tr_str_release(_wt_t11); });
    }
}

__attribute__((hot)) void TestRunner_assert_le_int(TestRunner* self, long long a, long long b, TrStr msg) {
    /* pass */
    if ((a <= b)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        ({ TrStr _wt_t12 = (_tr_str_wrap(_tr_int_to_str(a))); TrStr _wt_t13 = (_tr_str_wrap(_tr_int_to_str(b))); TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected %s <= %s", _tr_strz(msg), _wt_t12.data, _wt_t13.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected %s <= %s", _tr_strz(msg), _wt_t12.data, _wt_t13.data); _fr; }))); _tr_str_release(_wt_t12); _tr_str_release(_wt_t13); });
    }
}

__attribute__((hot)) void TestRunner_assert_in_range(TestRunner* self, long long value, long long lo, long long hi, TrStr msg) {
    /* pass */
    if (((value >= lo) && (value <= hi))) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        ({ TrStr _wt_t14 = (_tr_str_wrap(_tr_int_to_str(value))); TrStr _wt_t15 = (_tr_str_wrap(_tr_int_to_str(lo))); TrStr _wt_t16 = (_tr_str_wrap(_tr_int_to_str(hi))); TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — %s not in [%s, %s]", _tr_strz(msg), _wt_t14.data, _wt_t15.data, _wt_t16.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — %s not in [%s, %s]", _tr_strz(msg), _wt_t14.data, _wt_t15.data, _wt_t16.data); _fr; }))); _tr_str_release(_wt_t14); _tr_str_release(_wt_t15); _tr_str_release(_wt_t16); });
    }
}

__attribute__((hot)) void TestRunner_assert_eq_str(TestRunner* self, TrStr got, TrStr want, TrStr msg) {
    /* pass */
    if ((strcmp(_tr_strz(got), _tr_strz(want)) == 0)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected \"%s\", got \"%s\"", _tr_strz(msg), _tr_strz(want), _tr_strz(got)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected \"%s\", got \"%s\"", _tr_strz(msg), _tr_strz(want), _tr_strz(got)); _fr; })));
    }
}

__attribute__((hot)) void TestRunner_assert_ne_str(TestRunner* self, TrStr a, TrStr b, TrStr msg) {
    /* pass */
    if ((strcmp(_tr_strz(a), _tr_strz(b)) != 0)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected strings to differ, both are \"%s\"", _tr_strz(msg), _tr_strz(a)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected strings to differ, both are \"%s\"", _tr_strz(msg), _tr_strz(a)); _fr; })));
    }
}

__attribute__((hot)) void TestRunner_assert_contains(TestRunner* self, TrStr s, TrStr sub, TrStr msg) {
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    char* subp = ((char*)(_tr_strz(sub)));
    /* pass */
    long long slen = 0LL;
    /* pass */
    long long subl = 0LL;
    /* pass */
    while ((((long long)((*(sp + slen)))) != 0LL)) {
        /* pass */
        slen = (slen + 1LL);
    }
    /* pass */
    while ((((long long)((*(subp + subl)))) != 0LL)) {
        /* pass */
        subl = (subl + 1LL);
    }
    /* pass */
    bool found = false;
    /* pass */
    long long i = 0LL;
    /* pass */
    while (((i <= (slen - subl)) && (!found))) {
        /* pass */
        long long j = 0LL;
        /* pass */
        bool ok = true;
        /* pass */
        while (((j < subl) && ok)) {
            /* pass */
            if ((((long long)((*(sp + (i + j))))) != ((long long)((*(subp + j)))))) {
                /* pass */
                ok = false;
            }
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        if (ok) {
            /* pass */
            found = true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (found) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — \"%s\" does not contain \"%s\"", _tr_strz(msg), _tr_strz(s), _tr_strz(sub)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — \"%s\" does not contain \"%s\"", _tr_strz(msg), _tr_strz(s), _tr_strz(sub)); _fr; })));
    }
}

__attribute__((hot)) void TestRunner_assert_starts_with(TestRunner* self, TrStr s, TrStr prefix, TrStr msg) {
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    char* pp = ((char*)(_tr_strz(prefix)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long pc = ((long long)((*(pp + i))));
        /* pass */
        if ((pc == 0LL)) {
            /* pass */
            TestRunner__pass(self);
            /* pass */
            return;
        }
        /* pass */
        long long sc = ((long long)((*(sp + i))));
        /* pass */
        if (((sc == 0LL) || (sc != pc))) {
            /* pass */
            TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — \"%s\" does not start with \"%s\"", _tr_strz(msg), _tr_strz(s), _tr_strz(prefix)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — \"%s\" does not start with \"%s\"", _tr_strz(msg), _tr_strz(s), _tr_strz(prefix)); _fr; })));
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void TestRunner_assert_ends_with(TestRunner* self, TrStr s, TrStr suffix, TrStr msg) {
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    char* ep = ((char*)(_tr_strz(suffix)));
    /* pass */
    long long sl = 0LL;
    /* pass */
    long long el = 0LL;
    /* pass */
    while ((((long long)((*(sp + sl)))) != 0LL)) {
        /* pass */
        sl = (sl + 1LL);
    }
    /* pass */
    while ((((long long)((*(ep + el)))) != 0LL)) {
        /* pass */
        el = (el + 1LL);
    }
    /* pass */
    if ((el > sl)) {
        /* pass */
        TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — \"%s\" does not end with \"%s\"", _tr_strz(msg), _tr_strz(s), _tr_strz(suffix)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — \"%s\" does not end with \"%s\"", _tr_strz(msg), _tr_strz(s), _tr_strz(suffix)); _fr; })));
        /* pass */
        return;
    }
    /* pass */
    long long off = (sl - el);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < el)) {
        /* pass */
        if ((((long long)((*(sp + (off + i))))) != ((long long)((*(ep + i)))))) {
            /* pass */
            TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — \"%s\" does not end with \"%s\"", _tr_strz(msg), _tr_strz(s), _tr_strz(suffix)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — \"%s\" does not end with \"%s\"", _tr_strz(msg), _tr_strz(s), _tr_strz(suffix)); _fr; })));
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TestRunner__pass(self);
}

__attribute__((hot)) void TestRunner_assert_eq_float(TestRunner* self, double got, double want, double eps, TrStr msg) {
    /* pass */
    double diff = (got - want);
    /* pass */
    if ((diff < 0.0)) {
        /* pass */
        diff = (0.0 - diff);
    }
    /* pass */
    if ((diff <= eps)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        ({ TrStr _wt_t17 = (_tr_str_wrap(_tr_float_to_str(eps))); TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — floats not equal within epsilon %s", _tr_strz(msg), _wt_t17.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — floats not equal within epsilon %s", _tr_strz(msg), _wt_t17.data); _fr; }))); _tr_str_release(_wt_t17); });
    }
}

__attribute__((hot)) void TestRunner_assert_ne_float(TestRunner* self, double a, double b, double eps, TrStr msg) {
    /* pass */
    double diff = (a - b);
    /* pass */
    if ((diff < 0.0)) {
        /* pass */
        diff = (0.0 - diff);
    }
    /* pass */
    if ((diff > eps)) {
        /* pass */
        TestRunner__pass(self);
    } else {
        /* pass */
        ({ TrStr _wt_t18 = (_tr_str_wrap(_tr_float_to_str(eps))); TestRunner__fail(self, _tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s — expected floats to differ by more than %s", _tr_strz(msg), _wt_t18.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s — expected floats to differ by more than %s", _tr_strz(msg), _wt_t18.data); _fr; }))); _tr_str_release(_wt_t18); });
    }
}

__attribute__((hot)) void TestRunner_fail(TestRunner* self, TrStr msg) {
    /* pass */
    TestRunner__fail(self, msg);
}

__attribute__((hot)) void TestRunner_skip(TestRunner* self, TrStr msg) {
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_wrap(({ int _fz = snprintf(NULL,0,"  SKIP: %s", _tr_strz(msg)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"  SKIP: %s", _tr_strz(msg)); _fr; })))); printf("\n"); });
}

__attribute__((hot)) void TestRunner_section(TestRunner* self, TrStr label) {
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_wrap(({ int _fz = snprintf(NULL,0,"  [%s]", _tr_strz(label)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"  [%s]", _tr_strz(label)); _fr; })))); printf("\n"); });
}

__attribute__((hot)) bool TestRunner_ok(TestRunner* self) {
    /* pass */
    return (self->failed == 0LL);
}

__attribute__((hot)) void TestRunner_reset(TestRunner* self) {
    /* pass */
    self->passed = 0LL;
    /* pass */
    self->failed = 0LL;
}

__attribute__((hot)) bool TestRunner_summary(TestRunner* self) {
    /* pass */
    long long total = (self->passed + self->failed);
    /* pass */
    if ((self->failed == 0LL)) {
        /* pass */
        ({ TrStr _wt_t19 = (_tr_str_wrap(_tr_int_to_str(total))); ({ printf("%s", _tr_strz(_tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s: all %s tests passed.", _tr_strz(self->name), _wt_t19.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s: all %s tests passed.", _tr_strz(self->name), _wt_t19.data); _fr; })))); printf("\n"); }); _tr_str_release(_wt_t19); });
        /* pass */
        return true;
    } else {
        /* pass */
        ({ TrStr _wt_t20 = (_tr_str_wrap(_tr_int_to_str(self->failed))); TrStr _wt_t21 = (_tr_str_wrap(_tr_int_to_str(total))); ({ printf("%s", _tr_strz(_tr_str_wrap(({ int _fz = snprintf(NULL,0,"%s: %s/%s tests FAILED.", _tr_strz(self->name), _wt_t20.data, _wt_t21.data); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"%s: %s/%s tests FAILED.", _tr_strz(self->name), _wt_t20.data, _wt_t21.data); _fr; })))); printf("\n"); }); _tr_str_release(_wt_t20); _tr_str_release(_wt_t21); });
        /* pass */
        return false;
    }
}

