#include "../../../tauraro_types.h"

long long JsonDoc_cur(JsonDoc* self);
void JsonDoc_adv(JsonDoc* self);
void JsonDoc_skip_ws(JsonDoc* self);
long long JsonDoc__new_node(JsonDoc* self, long long tag);
long long JsonDoc__read_string(JsonDoc* self);
long long JsonDoc__parse_val(JsonDoc* self);
long long JsonDoc__parse_number(JsonDoc* self);
long long JsonDoc__parse_array(JsonDoc* self);
long long JsonDoc__parse_object(JsonDoc* self);
bool JsonDoc__key_eq(JsonDoc* self, long long child, TrStr key);
JsonRef JsonRef__wrap(JsonRef self, long long i);
void JsonWriter__sep(JsonWriter* self);

__attribute__((malloc,returns_nonnull,hot)) JsonDoc* JsonDoc_init(TrStr src) {
    /* pass */
    JsonDoc* d = ((JsonDoc*)_tr_obj_alloc(sizeof(JsonDoc)));
    /* pass */
    d->input = _tr_str_retain(src);
    /* pass */
    d->strs = _tr_str_lit("");
    /* pass */
    d->pos = 0LL;
    /* pass */
    d->len = _tr_strlen(_tr_strz(src));
    /* pass */
    d->soff = 0LL;
    /* pass */
    d->sb = StringBuilder_init(64LL);
    /* pass */
    d->tags = (void*)List_i64_new();
    /* pass */
    d->ivals = (void*)List_i64_new();
    /* pass */
    d->fvals = (void*)List_f64_new();
    /* pass */
    d->soffs = (void*)List_i64_new();
    /* pass */
    d->slens = (void*)List_i64_new();
    /* pass */
    d->koffs = (void*)List_i64_new();
    /* pass */
    d->klens = (void*)List_i64_new();
    /* pass */
    d->kids = (void*)List_i64_new();
    /* pass */
    d->sibs = (void*)List_i64_new();
    /* pass */
    return d;
}

__attribute__((hot)) long long JsonDoc_cur(JsonDoc* self) {
    /* pass */
    if ((self->pos >= self->len)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    return ((long long)((*(((char*)(_tr_strz(self->input))) + self->pos))));
}

__attribute__((hot)) void JsonDoc_adv(JsonDoc* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
}

__attribute__((hot)) void JsonDoc_skip_ws(JsonDoc* self) {
    /* pass */
    while ((self->pos < self->len)) {
        /* pass */
        long long c = JsonDoc_cur(self);
        /* pass */
        if (((((c == 32LL) || (c == 9LL)) || (c == 10LL)) || (c == 13LL))) {
            /* pass */
            JsonDoc_adv(self);
        } else {
            /* pass */
            break;
        }
    }
}

__attribute__((hot)) long long JsonDoc__new_node(JsonDoc* self, long long tag) {
    /* pass */
    List_i64_append(self->tags, tag);
    /* pass */
    List_i64_append(self->ivals, 0LL);
    /* pass */
    List_f64_append(self->fvals, 0.0);
    /* pass */
    List_i64_append(self->soffs, 0LL);
    /* pass */
    List_i64_append(self->slens, 0LL);
    /* pass */
    List_i64_append(self->koffs, 0LL);
    /* pass */
    List_i64_append(self->klens, (0LL - 1LL));
    /* pass */
    List_i64_append(self->kids, (0LL - 1LL));
    /* pass */
    List_i64_append(self->sibs, (0LL - 1LL));
    /* pass */
    return (self->tags->len - 1LL);
}

__attribute__((hot)) long long JsonDoc__read_string(JsonDoc* self) {
    /* pass */
    long long off = self->soff;
    /* pass */
    JsonDoc_adv(self);
    /* pass */
    while ((self->pos < self->len)) {
        /* pass */
        long long c = JsonDoc_cur(self);
        /* pass */
        if ((c == 34LL)) {
            /* pass */
            JsonDoc_adv(self);
            /* pass */
            break;
        }
        /* pass */
        if ((c == 92LL)) {
            /* pass */
            JsonDoc_adv(self);
            /* pass */
            long long esc = JsonDoc_cur(self);
            /* pass */
            JsonDoc_adv(self);
            /* pass */
            if ((esc == 110LL)) {
                /* pass */
                StringBuilder_append_char(self->sb, 10LL);
            } else if ((esc == 116LL)) {
                /* pass */
                StringBuilder_append_char(self->sb, 9LL);
            } else if ((esc == 114LL)) {
                /* pass */
                StringBuilder_append_char(self->sb, 13LL);
            } else if ((esc == 92LL)) {
                /* pass */
                StringBuilder_append_char(self->sb, 92LL);
            } else if ((esc == 34LL)) {
                /* pass */
                StringBuilder_append_char(self->sb, 34LL);
            } else if ((esc == 47LL)) {
                /* pass */
                StringBuilder_append_char(self->sb, 47LL);
            } else if ((esc == 98LL)) {
                /* pass */
                StringBuilder_append_char(self->sb, 8LL);
            } else if ((esc == 102LL)) {
                /* pass */
                StringBuilder_append_char(self->sb, 12LL);
            } else {
                /* pass */
                StringBuilder_append_char(self->sb, esc);
            }
            /* pass */
            self->soff = (self->soff + 1LL);
        } else {
            /* pass */
            StringBuilder_append_char(self->sb, c);
            /* pass */
            self->soff = (self->soff + 1LL);
            /* pass */
            JsonDoc_adv(self);
        }
    }
    /* pass */
    return off;
}

__attribute__((hot)) long long JsonDoc__parse_val(JsonDoc* self) {
    /* pass */
    JsonDoc_skip_ws(self);
    /* pass */
    long long c = JsonDoc_cur(self);
    /* pass */
    if ((c == 34LL)) {
        /* pass */
        long long off = JsonDoc__read_string(self);
        /* pass */
        long long idx = JsonDoc__new_node(self, 4LL);
        /* pass */
        List_i64_set(self->soffs, idx, off);
        /* pass */
        List_i64_set(self->slens, idx, (self->soff - off));
        /* pass */
        return idx;
    }
    /* pass */
    if ((c == 116LL)) {
        /* pass */
        self->pos = (self->pos + 4LL);
        /* pass */
        long long idx = JsonDoc__new_node(self, 1LL);
        /* pass */
        List_i64_set(self->ivals, idx, 1LL);
        /* pass */
        return idx;
    }
    /* pass */
    if ((c == 102LL)) {
        /* pass */
        self->pos = (self->pos + 5LL);
        /* pass */
        return JsonDoc__new_node(self, 1LL);
    }
    /* pass */
    if ((c == 110LL)) {
        /* pass */
        self->pos = (self->pos + 4LL);
        /* pass */
        return JsonDoc__new_node(self, 0LL);
    }
    /* pass */
    if ((c == 91LL)) {
        /* pass */
        return JsonDoc__parse_array(self);
    }
    /* pass */
    if ((c == 123LL)) {
        /* pass */
        return JsonDoc__parse_object(self);
    }
    /* pass */
    if ((c == 0LL)) {
        /* pass */
        return JsonDoc__new_node(self, 0LL);
    }
    /* pass */
    return JsonDoc__parse_number(self);
}

__attribute__((hot)) long long JsonDoc__parse_number(JsonDoc* self) {
    /* pass */
    long long start = self->pos;
    /* pass */
    bool is_float = false;
    /* pass */
    if ((JsonDoc_cur(self) == 45LL)) {
        /* pass */
        JsonDoc_adv(self);
    }
    /* pass */
    while ((self->pos < self->len)) {
        /* pass */
        long long c = JsonDoc_cur(self);
        /* pass */
        if (((c >= 48LL) && (c <= 57LL))) {
            /* pass */
            JsonDoc_adv(self);
        } else if ((((c == 46LL) || (c == 101LL)) || (c == 69LL))) {
            /* pass */
            is_float = true;
            /* pass */
            JsonDoc_adv(self);
        } else if ((((c == 43LL) || (c == 45LL)) && (self->pos > (start + 1LL)))) {
            /* pass */
            JsonDoc_adv(self);
        } else {
            /* pass */
            break;
        }
    }
    /* pass */
    TrStr raw = _tr_str_lit("");
    /* pass */
    /* unsafe block */
    /* pass */
    TrStr _strtmp_t34 = _tr_str_wrap(_tr_str_slice(_tr_strz(self->input), start, self->pos));
    _tr_str_release(raw);
    raw = _strtmp_t34;
    /* pass */
    if (is_float) {
        /* pass */
        long long idx = JsonDoc__new_node(self, 3LL);
        /* pass */
        List_f64_set(self->fvals, idx, _tr_str_to_float(_tr_strz(raw)));
        /* pass */
        _tr_str_release(raw);
        return idx;
    }
    /* pass */
    long long idx = JsonDoc__new_node(self, 2LL);
    /* pass */
    List_i64_set(self->ivals, idx, _tr_str_to_int(_tr_strz(raw)));
    /* pass */
    _tr_str_release(raw);
    return idx;
}

__attribute__((hot)) long long JsonDoc__parse_array(JsonDoc* self) {
    /* pass */
    JsonDoc_adv(self);
    /* pass */
    long long arr = JsonDoc__new_node(self, 5LL);
    /* pass */
    JsonDoc_skip_ws(self);
    /* pass */
    if ((JsonDoc_cur(self) == 93LL)) {
        /* pass */
        JsonDoc_adv(self);
        /* pass */
        return arr;
    }
    /* pass */
    long long prev = (0LL - 1LL);
    /* pass */
    while (true) {
        /* pass */
        JsonDoc_skip_ws(self);
        /* pass */
        long long child = JsonDoc__parse_val(self);
        /* pass */
        if ((prev == (0LL - 1LL))) {
            /* pass */
            List_i64_set(self->kids, arr, child);
        } else {
            /* pass */
            List_i64_set(self->sibs, prev, child);
        }
        /* pass */
        prev = child;
        /* pass */
        JsonDoc_skip_ws(self);
        /* pass */
        long long ch = JsonDoc_cur(self);
        /* pass */
        if ((ch == 93LL)) {
            /* pass */
            JsonDoc_adv(self);
            /* pass */
            break;
        }
        /* pass */
        if ((ch == 44LL)) {
            /* pass */
            JsonDoc_adv(self);
        }
        /* pass */
        if ((ch == 0LL)) {
            /* pass */
            break;
        }
    }
    /* pass */
    return arr;
}

__attribute__((hot)) long long JsonDoc__parse_object(JsonDoc* self) {
    /* pass */
    JsonDoc_adv(self);
    /* pass */
    long long obj = JsonDoc__new_node(self, 6LL);
    /* pass */
    JsonDoc_skip_ws(self);
    /* pass */
    if ((JsonDoc_cur(self) == 125LL)) {
        /* pass */
        JsonDoc_adv(self);
        /* pass */
        return obj;
    }
    /* pass */
    long long prev = (0LL - 1LL);
    /* pass */
    while (true) {
        /* pass */
        JsonDoc_skip_ws(self);
        /* pass */
        if ((JsonDoc_cur(self) != 34LL)) {
            /* pass */
            break;
        }
        /* pass */
        long long koff = JsonDoc__read_string(self);
        /* pass */
        long long klen = (self->soff - koff);
        /* pass */
        JsonDoc_skip_ws(self);
        /* pass */
        if ((JsonDoc_cur(self) == 58LL)) {
            /* pass */
            JsonDoc_adv(self);
        }
        /* pass */
        JsonDoc_skip_ws(self);
        /* pass */
        long long child = JsonDoc__parse_val(self);
        /* pass */
        List_i64_set(self->koffs, child, koff);
        /* pass */
        List_i64_set(self->klens, child, klen);
        /* pass */
        if ((prev == (0LL - 1LL))) {
            /* pass */
            List_i64_set(self->kids, obj, child);
        } else {
            /* pass */
            List_i64_set(self->sibs, prev, child);
        }
        /* pass */
        prev = child;
        /* pass */
        JsonDoc_skip_ws(self);
        /* pass */
        long long ch = JsonDoc_cur(self);
        /* pass */
        if ((ch == 125LL)) {
            /* pass */
            JsonDoc_adv(self);
            /* pass */
            break;
        }
        /* pass */
        if ((ch == 44LL)) {
            /* pass */
            JsonDoc_adv(self);
        }
        /* pass */
        if ((ch == 0LL)) {
            /* pass */
            break;
        }
    }
    /* pass */
    return obj;
}

__attribute__((hot)) long long JsonDoc_parse_root(JsonDoc* self) {
    /* pass */
    long long r = JsonDoc__parse_val(self);
    /* pass */
    self->strs = StringBuilder_to_owned(self->sb);
    /* pass */
    return r;
}

__attribute__((hot)) long long JsonDoc_tag_at(JsonDoc* self, long long idx) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        return 0LL;
    }
    /* pass */
    return List_i64_get(self->tags, idx);
}

__attribute__((hot)) TrStr JsonDoc_str_at(JsonDoc* self, long long idx) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(self->strs), List_i64_get(self->soffs, idx), (List_i64_get(self->soffs, idx) + List_i64_get(self->slens, idx))));
}

__attribute__((hot)) StrView JsonDoc_strview_at(JsonDoc* self, long long idx) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        return StrView_of(self->strs, 0LL, 0LL);
    }
    /* pass */
    return StrView_of(self->strs, List_i64_get(self->soffs, idx), List_i64_get(self->slens, idx));
}

__attribute__((hot)) long long JsonDoc_int_at(JsonDoc* self, long long idx) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        return 0LL;
    }
    /* pass */
    return List_i64_get(self->ivals, idx);
}

__attribute__((hot)) double JsonDoc_float_at(JsonDoc* self, long long idx) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        return 0.0;
    }
    /* pass */
    return List_f64_get(self->fvals, idx);
}

__attribute__((hot)) bool JsonDoc_bool_at(JsonDoc* self, long long idx) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        return false;
    }
    /* pass */
    return (List_i64_get(self->ivals, idx) != 0LL);
}

__attribute__((hot)) bool JsonDoc__key_eq(JsonDoc* self, long long child, TrStr key) {
    /* pass */
    if ((List_i64_get(self->klens, child) < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr ks = _tr_str_wrap(_tr_str_slice(_tr_strz(self->strs), List_i64_get(self->koffs, child), (List_i64_get(self->koffs, child) + List_i64_get(self->klens, child))));
    /* pass */
    return (strcmp(_tr_strz(ks), _tr_strz(key)) == 0);
}

__attribute__((hot)) long long JsonDoc_obj_get_at(JsonDoc* self, long long idx, TrStr key) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        return (0LL - 1LL);
    }
    /* pass */
    long long child = List_i64_get(self->kids, idx);
    /* pass */
    while ((child != (0LL - 1LL))) {
        /* pass */
        if (JsonDoc__key_eq(self, child, key)) {
            /* pass */
            return child;
        }
        /* pass */
        child = List_i64_get(self->sibs, child);
    }
    /* pass */
    return (0LL - 1LL);
}

__attribute__((hot)) long long JsonDoc_array_len_at(JsonDoc* self, long long idx) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        return 0LL;
    }
    /* pass */
    long long cnt = 0LL;
    /* pass */
    long long child = List_i64_get(self->kids, idx);
    /* pass */
    while ((child != (0LL - 1LL))) {
        /* pass */
        cnt = (cnt + 1LL);
        /* pass */
        child = List_i64_get(self->sibs, child);
    }
    /* pass */
    return cnt;
}

__attribute__((hot)) long long JsonDoc_array_get_at(JsonDoc* self, long long idx, long long i) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        return (0LL - 1LL);
    }
    /* pass */
    long long child = List_i64_get(self->kids, idx);
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((child != (0LL - 1LL))) {
        /* pass */
        if ((k == i)) {
            /* pass */
            return child;
        }
        /* pass */
        k = (k + 1LL);
        /* pass */
        child = List_i64_get(self->sibs, child);
    }
    /* pass */
    return (0LL - 1LL);
}

__attribute__((hot)) void JsonDoc_write_at(JsonDoc* self, long long idx, StringBuilder* sb) {
    /* pass */
    if (((idx < 0LL) || (idx >= self->tags->len))) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("null"));
        /* pass */
        return;
    }
    /* pass */
    long long t = List_i64_get(self->tags, idx);
    /* pass */
    if ((t == 0LL)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("null"));
    } else if ((t == 1LL)) {
        /* pass */
        if ((List_i64_get(self->ivals, idx) != 0LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("true"));
        } else {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("false"));
        }
    } else if ((t == 2LL)) {
        /* pass */
        ({ TrStr _sbt_t35 = (_tr_str_wrap(_tr_int_to_str(List_i64_get(self->ivals, idx)))); StringBuilder_append(sb, _sbt_t35); _tr_str_release(_sbt_t35); });
    } else if ((t == 3LL)) {
        /* pass */
        ({ TrStr _sbt_t36 = (_tr_str_wrap(_tr_float_to_str(List_f64_get(self->fvals, idx)))); StringBuilder_append(sb, _sbt_t36); _tr_str_release(_sbt_t36); });
    } else if ((t == 4LL)) {
        /* pass */
        StringBuilder_append_char(sb, 34LL);
        /* pass */
        ({ TrStr _at_t37 = (JsonDoc_str_at(self, idx)); _json_escape(_at_t37, sb); _tr_str_release(_at_t37); });
        /* pass */
        StringBuilder_append_char(sb, 34LL);
    } else if ((t == 5LL)) {
        /* pass */
        StringBuilder_append_char(sb, 91LL);
        /* pass */
        long long child = List_i64_get(self->kids, idx);
        /* pass */
        bool first = true;
        /* pass */
        while ((child != (0LL - 1LL))) {
            /* pass */
            if ((!first)) {
                /* pass */
                StringBuilder_append_char(sb, 44LL);
            }
            /* pass */
            first = false;
            /* pass */
            JsonDoc_write_at(self, child, sb);
            /* pass */
            child = List_i64_get(self->sibs, child);
        }
        /* pass */
        StringBuilder_append_char(sb, 93LL);
    } else if ((t == 6LL)) {
        /* pass */
        StringBuilder_append_char(sb, 123LL);
        /* pass */
        long long child = List_i64_get(self->kids, idx);
        /* pass */
        bool first = true;
        /* pass */
        while ((child != (0LL - 1LL))) {
            /* pass */
            if ((!first)) {
                /* pass */
                StringBuilder_append_char(sb, 44LL);
            }
            /* pass */
            first = false;
            /* pass */
            StringBuilder_append_char(sb, 34LL);
            /* pass */
            ({ TrStr _at_t38 = (_tr_str_wrap(_tr_str_slice(_tr_strz(self->strs), List_i64_get(self->koffs, child), (List_i64_get(self->koffs, child) + List_i64_get(self->klens, child))))); _json_escape(_at_t38, sb); _tr_str_release(_at_t38); });
            /* pass */
            StringBuilder_append_char(sb, 34LL);
            /* pass */
            StringBuilder_append_char(sb, 58LL);
            /* pass */
            JsonDoc_write_at(self, child, sb);
            /* pass */
            child = List_i64_get(self->sibs, child);
        }
        /* pass */
        StringBuilder_append_char(sb, 125LL);
    }
}

__attribute__((hot)) JsonRef JsonDoc_root(JsonDoc* self) {
    /* pass */
    JsonRef r = (JsonRef){0};
    /* pass */
    r.doc = self;
    /* pass */
    r.idx = 0LL;
    /* pass */
    return r;
}

__attribute__((hot)) JsonRef JsonRef__wrap(JsonRef self, long long i) {
    /* pass */
    JsonRef r = (JsonRef){0};
    /* pass */
    r.doc = self.doc;
    /* pass */
    r.idx = i;
    /* pass */
    return r;
}

__attribute__((hot)) bool JsonRef_exists(JsonRef self) {
    /* pass */
    return (self.idx >= 0LL);
}

__attribute__((hot)) long long JsonRef_tag(JsonRef self) {
    /* pass */
    return JsonDoc_tag_at(self.doc, self.idx);
}

__attribute__((hot)) bool JsonRef_is_null(JsonRef self) {
    /* pass */
    return (JsonDoc_tag_at(self.doc, self.idx) == 0LL);
}

__attribute__((hot)) bool JsonRef_is_bool(JsonRef self) {
    /* pass */
    return (JsonDoc_tag_at(self.doc, self.idx) == 1LL);
}

__attribute__((hot)) bool JsonRef_is_int(JsonRef self) {
    /* pass */
    return (JsonDoc_tag_at(self.doc, self.idx) == 2LL);
}

__attribute__((hot)) bool JsonRef_is_float(JsonRef self) {
    /* pass */
    return (JsonDoc_tag_at(self.doc, self.idx) == 3LL);
}

__attribute__((hot)) bool JsonRef_is_str(JsonRef self) {
    /* pass */
    return (JsonDoc_tag_at(self.doc, self.idx) == 4LL);
}

__attribute__((hot)) bool JsonRef_is_array(JsonRef self) {
    /* pass */
    return (JsonDoc_tag_at(self.doc, self.idx) == 5LL);
}

__attribute__((hot)) bool JsonRef_is_object(JsonRef self) {
    /* pass */
    return (JsonDoc_tag_at(self.doc, self.idx) == 6LL);
}

__attribute__((hot)) TrStr JsonRef_get_str(JsonRef self) {
    /* pass */
    return JsonDoc_str_at(self.doc, self.idx);
}

__attribute__((hot)) StrView JsonRef_str_view(JsonRef self) {
    /* pass */
    return JsonDoc_strview_at(self.doc, self.idx);
}

__attribute__((hot)) bool JsonRef_str_eq(JsonRef self, TrStr other) {
    /* pass */
    return StrView_eq(JsonDoc_strview_at(self.doc, self.idx), other);
}

__attribute__((hot)) long long JsonRef_get_int(JsonRef self) {
    /* pass */
    return JsonDoc_int_at(self.doc, self.idx);
}

__attribute__((hot)) double JsonRef_get_float(JsonRef self) {
    /* pass */
    return JsonDoc_float_at(self.doc, self.idx);
}

__attribute__((hot)) bool JsonRef_get_bool(JsonRef self) {
    /* pass */
    return JsonDoc_bool_at(self.doc, self.idx);
}

__attribute__((hot)) double JsonRef_as_float(JsonRef self) {
    /* pass */
    if ((JsonDoc_tag_at(self.doc, self.idx) == 2LL)) {
        /* pass */
        return ((double)(JsonDoc_int_at(self.doc, self.idx)));
    }
    /* pass */
    return JsonDoc_float_at(self.doc, self.idx);
}

__attribute__((hot)) JsonRef JsonRef_obj_get(JsonRef self, TrStr key) {
    /* pass */
    return JsonRef__wrap(self, JsonDoc_obj_get_at(self.doc, self.idx, key));
}

__attribute__((hot)) bool JsonRef_obj_has(JsonRef self, TrStr key) {
    /* pass */
    return (JsonDoc_obj_get_at(self.doc, self.idx, key) >= 0LL);
}

__attribute__((hot)) long long JsonRef_array_len(JsonRef self) {
    /* pass */
    return JsonDoc_array_len_at(self.doc, self.idx);
}

__attribute__((hot)) JsonRef JsonRef_array_get(JsonRef self, long long i) {
    /* pass */
    return JsonRef__wrap(self, JsonDoc_array_get_at(self.doc, self.idx, i));
}

__attribute__((hot)) TrStr JsonRef_to_str(JsonRef self) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    JsonDoc_write_at(self.doc, self.idx, sb);
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) JsonDoc* Json_parse(TrStr src) {
    /* pass */
    JsonDoc* d = JsonDoc_init(src);
    /* pass */
    long long r = JsonDoc_parse_root(d);
    /* pass */
    return d;
}

__attribute__((malloc,returns_nonnull,hot)) JsonWriter* JsonWriter_init(long long capacity) {
    /* pass */
    JsonWriter* w = ((JsonWriter*)_tr_obj_alloc(sizeof(JsonWriter)));
    /* pass */
    w->sb = StringBuilder_init(capacity);
    /* pass */
    w->_first = (void*)List_bool_new();
    /* pass */
    w->_pend = false;
    /* pass */
    return w;
}

__attribute__((hot)) void JsonWriter__sep(JsonWriter* self) {
    /* pass */
    if (self->_pend) {
        /* pass */
        self->_pend = false;
        /* pass */
        return;
    }
    /* pass */
    if ((self->_first->len > 0LL)) {
        /* pass */
        long long idx = (self->_first->len - 1LL);
        /* pass */
        if (List_bool_get(self->_first, idx)) {
            /* pass */
            List_bool_set(self->_first, idx, false);
        } else {
            /* pass */
            StringBuilder_append_char(self->sb, 44LL);
        }
    }
}

__attribute__((hot)) void JsonWriter_begin_object(JsonWriter* self) {
    /* pass */
    JsonWriter__sep(self);
    /* pass */
    StringBuilder_append_char(self->sb, 123LL);
    /* pass */
    List_bool_append(self->_first, true);
}

__attribute__((hot)) void JsonWriter_end_object(JsonWriter* self) {
    /* pass */
    StringBuilder_append_char(self->sb, 125LL);
    /* pass */
    List_bool_pop(self->_first);
}

__attribute__((hot)) void JsonWriter_begin_array(JsonWriter* self) {
    /* pass */
    JsonWriter__sep(self);
    /* pass */
    StringBuilder_append_char(self->sb, 91LL);
    /* pass */
    List_bool_append(self->_first, true);
}

__attribute__((hot)) void JsonWriter_end_array(JsonWriter* self) {
    /* pass */
    StringBuilder_append_char(self->sb, 93LL);
    /* pass */
    List_bool_pop(self->_first);
}

__attribute__((hot)) void JsonWriter_key(JsonWriter* self, TrStr name) {
    /* pass */
    JsonWriter__sep(self);
    /* pass */
    StringBuilder_append_char(self->sb, 34LL);
    /* pass */
    _json_escape(name, self->sb);
    /* pass */
    StringBuilder_append_char(self->sb, 34LL);
    /* pass */
    StringBuilder_append_char(self->sb, 58LL);
    /* pass */
    self->_pend = true;
}

__attribute__((hot)) void JsonWriter_int_val(JsonWriter* self, long long n) {
    /* pass */
    JsonWriter__sep(self);
    /* pass */
    StringBuilder_append_int(self->sb, n);
}

__attribute__((hot)) void JsonWriter_str_val(JsonWriter* self, TrStr s) {
    /* pass */
    JsonWriter__sep(self);
    /* pass */
    StringBuilder_append_char(self->sb, 34LL);
    /* pass */
    _json_escape(s, self->sb);
    /* pass */
    StringBuilder_append_char(self->sb, 34LL);
}

__attribute__((hot)) void JsonWriter_bool_val(JsonWriter* self, bool b) {
    /* pass */
    JsonWriter__sep(self);
    /* pass */
    if (b) {
        /* pass */
        StringBuilder_append(self->sb, _tr_str_lit("true"));
    } else {
        /* pass */
        StringBuilder_append(self->sb, _tr_str_lit("false"));
    }
}

__attribute__((hot)) void JsonWriter_null_val(JsonWriter* self) {
    /* pass */
    JsonWriter__sep(self);
    /* pass */
    StringBuilder_append(self->sb, _tr_str_lit("null"));
}

__attribute__((hot)) void JsonWriter_field_int(JsonWriter* self, TrStr name, long long n) {
    /* pass */
    JsonWriter_key(self, name);
    /* pass */
    JsonWriter_int_val(self, n);
}

__attribute__((hot)) void JsonWriter_field_str(JsonWriter* self, TrStr name, TrStr s) {
    /* pass */
    JsonWriter_key(self, name);
    /* pass */
    JsonWriter_str_val(self, s);
}

__attribute__((hot)) void JsonWriter_field_bool(JsonWriter* self, TrStr name, bool b) {
    /* pass */
    JsonWriter_key(self, name);
    /* pass */
    JsonWriter_bool_val(self, b);
}

__attribute__((hot)) TrStr JsonWriter_view(JsonWriter* self) {
    /* pass */
    return StringBuilder_as_str(self->sb);
}

__attribute__((hot)) TrStr JsonWriter_finish(JsonWriter* self) {
    /* pass */
    return StringBuilder_to_owned(self->sb);
}

__attribute__((hot)) void JsonWriter__tr_fn_free(JsonWriter* self) {
    /* pass */
    StringBuilder__tr_fn_free(self->sb);
    /* pass */
    List_bool_free(self->_first);
    /* pass */
    /* unsafe block */
    /* pass */
    _tr_c_free(((char*)(self)));
}

__attribute__((hot)) long long JSON_NULL() {
    /* pass */
    return 0LL;
}

__attribute__((hot)) long long JSON_BOOL() {
    /* pass */
    return 1LL;
}

__attribute__((hot)) long long JSON_INT() {
    /* pass */
    return 2LL;
}

__attribute__((hot)) long long JSON_FLOAT() {
    /* pass */
    return 3LL;
}

__attribute__((hot)) long long JSON_STR() {
    /* pass */
    return 4LL;
}

__attribute__((hot)) long long JSON_ARRAY() {
    /* pass */
    return 5LL;
}

__attribute__((hot)) long long JSON_OBJ() {
    /* pass */
    return 6LL;
}

__attribute__((hot)) void _json_escape(TrStr s, StringBuilder* sb) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long n = _tr_strlen(_tr_strz(s));
    /* pass */
    while ((i < n)) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 34LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\\""));
        } else if ((c == 92LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\\\"));
        } else if ((c == 10LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\n"));
        } else if ((c == 13LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\r"));
        } else if ((c == 9LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\t"));
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
        }
        /* pass */
        i = (i + 1LL);
    }
}

