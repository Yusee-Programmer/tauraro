#include "tauraro_types.h"

long long _indent_of(TrStr line);
TrStr _comment_text(TrStr line);
long long _bracket_depth(TrStr s);
TrStr _signature(TrStr decl);
TrStr _first_word_after(TrStr s, TrStr kw);
TrStr _join_doc(List_TrStr* lines);
DocItem* _new_item(TrStr kind, TrStr name, TrStr sig, TrStr doc, TrStr owner);
List_TrStr* _doc_lines(TrStr text);
bool _has_kind(DocModule* m, TrStr kind, TrStr owner);
TrStr _md_cell(TrStr s);
void _md_members(StringBuilder* sb, DocModule* m, TrStr owner);
TrStr _html_esc(TrStr s);
TrStr _html_doc(TrStr s);
TrStr _html_anchor(TrStr s);
TrStr _summary(TrStr doc);
List_TrStr* _doc_expand(TrStr path);

__attribute__((hot)) long long _indent_of(TrStr line) {
#line 31 "src/doc.tr"
    long long i = 0LL;
#line 32 "src/doc.tr"
    while (((i < _tr_str_lenv((line))) && ({ TrStr _cmpa_t2764 = (_tr_str_slicev((line), i, (i + 1LL))); TrStr _cmpb_t2765 = (_tr_str_lit_len(" ", 1LL)); _Bool _cmpr_t2766 = _tr_str_eqv(_cmpa_t2764, _cmpb_t2765); _tr_str_release(_cmpa_t2764); _cmpr_t2766; }))) {
#line 32 "src/doc.tr"
        i = (i + 1LL);
    }
#line 33 "src/doc.tr"
    return i;
}

__attribute__((hot)) TrStr _comment_text(TrStr line) {
#line 37 "src/doc.tr"
    TrStr t = _tr_str_stripv((line));
#line 38 "src/doc.tr"
    if (_tr_str_starts_withv((t), (_tr_str_lit_len("# ", 2LL)))) {
#line 38 "src/doc.tr"
        TrStr _trret592 = _tr_str_slicev((t), 2LL, _tr_str_lenv((t)));
        _tr_str_release(t);
        return _trret592;
    }
#line 39 "src/doc.tr"
    if (_tr_str_starts_withv((t), (_tr_str_lit_len("#", 1LL)))) {
#line 39 "src/doc.tr"
        TrStr _trret593 = _tr_str_slicev((t), 1LL, _tr_str_lenv((t)));
        _tr_str_release(t);
        return _trret593;
    }
#line 40 "src/doc.tr"
    TrStr _trret594 = _tr_str_retain(t);
    _tr_str_release(t);
    return _trret594;
}

__attribute__((hot)) long long _bracket_depth(TrStr s) {
#line 43 "src/doc.tr"
    long long d = 0LL;
#line 44 "src/doc.tr"
    long long i = 0LL;
#line 45 "src/doc.tr"
    bool in_str = false;
#line 46 "src/doc.tr"
    while ((i < _tr_str_lenv((s)))) {
#line 47 "src/doc.tr"
        TrStr c = _tr_str_slicev((s), i, (i + 1LL));
#line 48 "src/doc.tr"
        if (_tr_str_eqv((c), (_tr_str_lit_len("\"", 1LL)))) {
#line 48 "src/doc.tr"
            in_str = (!in_str);
        } else if ((!in_str)) {
#line 50 "src/doc.tr"
            if ((_tr_str_eqv((c), (_tr_str_lit_len("(", 1LL))) || _tr_str_eqv((c), (_tr_str_lit_len("[", 1LL))))) {
#line 50 "src/doc.tr"
                d = (d + 1LL);
            } else if ((_tr_str_eqv((c), (_tr_str_lit_len(")", 1LL))) || _tr_str_eqv((c), (_tr_str_lit_len("]", 1LL))))) {
#line 51 "src/doc.tr"
                d = (d - 1LL);
            } else if (_tr_str_eqv((c), (_tr_str_lit_len("#", 1LL)))) {
#line 52 "src/doc.tr"
                _tr_str_release(c);
                break;
            }
        }
#line 53 "src/doc.tr"
        i = (i + 1LL);
        _tr_str_release(c);
    }
#line 54 "src/doc.tr"
    return d;
}

__attribute__((hot)) TrStr _signature(TrStr decl) {
#line 58 "src/doc.tr"
    TrStr s = _tr_str_stripv((decl));
#line 59 "src/doc.tr"
    if (_tr_str_starts_withv((s), (_tr_str_lit_len("pub ", 4LL)))) {
#line 59 "src/doc.tr"
        TrStr _strtmp_t2767 = _tr_str_slicev((s), 4LL, _tr_str_lenv((s)));
        _tr_str_release(s);
        s = _strtmp_t2767;
    }
#line 60 "src/doc.tr"
    long long d = 0LL;
#line 61 "src/doc.tr"
    long long i = 0LL;
#line 62 "src/doc.tr"
    bool in_str = false;
#line 63 "src/doc.tr"
    while ((i < _tr_str_lenv((s)))) {
#line 64 "src/doc.tr"
        TrStr c = _tr_str_slicev((s), i, (i + 1LL));
#line 65 "src/doc.tr"
        if (_tr_str_eqv((c), (_tr_str_lit_len("\"", 1LL)))) {
#line 65 "src/doc.tr"
            in_str = (!in_str);
        } else if ((!in_str)) {
#line 67 "src/doc.tr"
            if ((_tr_str_eqv((c), (_tr_str_lit_len("(", 1LL))) || _tr_str_eqv((c), (_tr_str_lit_len("[", 1LL))))) {
#line 67 "src/doc.tr"
                d = (d + 1LL);
            } else if ((_tr_str_eqv((c), (_tr_str_lit_len(")", 1LL))) || _tr_str_eqv((c), (_tr_str_lit_len("]", 1LL))))) {
#line 68 "src/doc.tr"
                d = (d - 1LL);
            } else if ((_tr_str_eqv((c), (_tr_str_lit_len(":", 1LL))) && (d == 0LL))) {
#line 72 "src/doc.tr"
                TrStr _trret595 = _tr_str_stripv((_tr_str_slicev((s), 0LL, i)));
                _tr_str_release(s);
                _tr_str_release(c);
                return _trret595;
            }
        }
#line 73 "src/doc.tr"
        i = (i + 1LL);
        _tr_str_release(c);
    }
#line 74 "src/doc.tr"
    TrStr _trret596 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret596;
}

__attribute__((hot)) TrStr _first_word_after(TrStr s, TrStr kw) {
#line 77 "src/doc.tr"
    TrStr t = _tr_str_stripv((s));
#line 78 "src/doc.tr"
    if (_tr_str_starts_withv((t), (_tr_str_lit_len("pub ", 4LL)))) {
#line 78 "src/doc.tr"
        TrStr _strtmp_t2768 = _tr_str_slicev((t), 4LL, _tr_str_lenv((t)));
        _tr_str_release(t);
        t = _strtmp_t2768;
    }
#line 79 "src/doc.tr"
    if ((!_tr_str_starts_withv((t), (_tr_strx_concatv((kw), (_tr_str_lit_len(" ", 1LL))))))) {
#line 79 "src/doc.tr"
        _tr_str_release(t);
        return _tr_str_lit_len("", 0LL);
    }
#line 80 "src/doc.tr"
    TrStr _strtmp_t2771 = ({ __auto_type _ord_t2769 = ((_tr_str_lenv((kw)) + 1LL)); __auto_type _ord_t2770 = (_tr_str_lenv((t))); __auto_type _wr = (_tr_str_stripv((_tr_str_slicev((t), _ord_t2769, _ord_t2770)))); _wr; });
    _tr_str_release(t);
    t = _strtmp_t2771;
#line 81 "src/doc.tr"
    long long i = 0LL;
#line 82 "src/doc.tr"
    while ((i < _tr_str_lenv((t)))) {
#line 83 "src/doc.tr"
        TrStr c = _tr_str_slicev((t), i, (i + 1LL));
#line 84 "src/doc.tr"
        if (((((_tr_str_eqv((c), (_tr_str_lit_len("(", 1LL))) || _tr_str_eqv((c), (_tr_str_lit_len(":", 1LL)))) || _tr_str_eqv((c), (_tr_str_lit_len("[", 1LL)))) || _tr_str_eqv((c), (_tr_str_lit_len(" ", 1LL)))) || _tr_str_eqv((c), (_tr_str_lit_len("=", 1LL))))) {
#line 84 "src/doc.tr"
            _tr_str_release(c);
            break;
        }
#line 85 "src/doc.tr"
        i = (i + 1LL);
        _tr_str_release(c);
    }
#line 86 "src/doc.tr"
    TrStr _trret597 = _tr_str_slicev((t), 0LL, i);
    _tr_str_release(t);
    return _trret597;
}

__attribute__((hot)) TrStr _join_doc(List_TrStr* lines) {
#line 89 "src/doc.tr"
    StringBuilder* sb = StringBuilder_init(256LL);
#line 90 "src/doc.tr"
    long long i = 0LL;
#line 91 "src/doc.tr"
    while ((i < lines->len)) {
#line 92 "src/doc.tr"
        if ((i > 0LL)) {
#line 92 "src/doc.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
        }
#line 93 "src/doc.tr"
        ({ TrStr _sbt_t2772 = (List_TrStr_get(lines, i)); StringBuilder_append(sb, _sbt_t2772); _tr_str_release(_sbt_t2772); });
#line 94 "src/doc.tr"
        i = (i + 1LL);
    }
#line 95 "src/doc.tr"
    return _tr_str_stripv((StringObj_as_str(StringBuilder_to_string(sb))));
}

__attribute__((hot)) DocItem* _new_item(TrStr kind, TrStr name, TrStr sig, TrStr doc, TrStr owner) {
#line 98 "src/doc.tr"
    DocItem* it = ((DocItem*)_tr_obj_alloc(sizeof(DocItem)));
#line 99 "src/doc.tr"
    it->kind = _tr_str_retain(kind);
#line 100 "src/doc.tr"
    it->name = _tr_str_retain(name);
#line 101 "src/doc.tr"
    it->sig = _tr_str_retain(sig);
#line 102 "src/doc.tr"
    it->doc = _tr_str_retain(doc);
#line 103 "src/doc.tr"
    it->owner = _tr_str_retain(owner);
#line 104 "src/doc.tr"
    return it;
}

__attribute__((hot)) TrStr doc_module_name(TrStr path) {
#line 108 "src/doc.tr"
    TrStr p = _tr_str_replacev((path), (_tr_str_lit_len("\\", 1LL)), (_tr_str_lit_len("/", 1LL)));
#line 109 "src/doc.tr"
    while (_tr_str_starts_withv((p), (_tr_str_lit_len("./", 2LL)))) {
#line 109 "src/doc.tr"
        TrStr _strtmp_t2773 = _tr_str_slicev((p), 2LL, _tr_str_lenv((p)));
        _tr_str_release(p);
        p = _strtmp_t2773;
    }
#line 111 "src/doc.tr"
    if ((_tr_str_starts_withv((p), (_tr_str_lit_len("/", 1LL))) || ((_tr_str_lenv((p)) > 1LL) && ({ TrStr _cmpa_t2774 = (_tr_str_slicev((p), 1LL, 2LL)); TrStr _cmpb_t2775 = (_tr_str_lit_len(":", 1LL)); _Bool _cmpr_t2776 = _tr_str_eqv(_cmpa_t2774, _cmpb_t2775); _tr_str_release(_cmpa_t2774); _cmpr_t2776; })))) {
#line 112 "src/doc.tr"
        long long at = _tr_str_index_ofv((p), (_tr_str_lit_len("/std/", 5LL)));
#line 113 "src/doc.tr"
        if ((at >= 0LL)) {
#line 113 "src/doc.tr"
            TrStr _strtmp_t2777 = _tr_str_slicev((p), (at + 1LL), _tr_str_lenv((p)));
            _tr_str_release(p);
            p = _strtmp_t2777;
        } else {
#line 115 "src/doc.tr"
            long long k = (_tr_str_lenv((p)) - 1LL);
#line 116 "src/doc.tr"
            while (((k >= 0LL) && ({ TrStr _cmpa_t2778 = (_tr_str_slicev((p), k, (k + 1LL))); TrStr _cmpb_t2779 = (_tr_str_lit_len("/", 1LL)); _Bool _cmpr_t2780 = !_tr_str_eqv(_cmpa_t2778, _cmpb_t2779); _tr_str_release(_cmpa_t2778); _cmpr_t2780; }))) {
#line 116 "src/doc.tr"
                k = (k - 1LL);
            }
#line 117 "src/doc.tr"
            TrStr _strtmp_t2781 = _tr_str_slicev((p), (k + 1LL), _tr_str_lenv((p)));
            _tr_str_release(p);
            p = _strtmp_t2781;
        }
    }
#line 118 "src/doc.tr"
    if (_tr_str_ends_withv((p), (_tr_str_lit_len(".tr", 3LL)))) {
#line 118 "src/doc.tr"
        TrStr _strtmp_t2782 = _tr_str_slicev((p), 0LL, (_tr_str_lenv((p)) - 3LL));
        _tr_str_release(p);
        p = _strtmp_t2782;
    }
#line 119 "src/doc.tr"
    if (_tr_str_ends_withv((p), (_tr_str_lit_len("/mod", 4LL)))) {
#line 119 "src/doc.tr"
        TrStr _strtmp_t2783 = _tr_str_slicev((p), 0LL, (_tr_str_lenv((p)) - 4LL));
        _tr_str_release(p);
        p = _strtmp_t2783;
    }
#line 120 "src/doc.tr"
    TrStr _trret598 = _tr_str_replacev((p), (_tr_str_lit_len("/", 1LL)), (_tr_str_lit_len(".", 1LL)));
    _tr_str_release(p);
    return _trret598;
}

__attribute__((hot)) List_TrStr* _doc_lines(TrStr text) {
#line 124 "src/doc.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 125 "src/doc.tr"
    long long start = 0LL;
#line 126 "src/doc.tr"
    long long i = 0LL;
#line 127 "src/doc.tr"
    long long n = _tr_str_lenv((text));
#line 128 "src/doc.tr"
    char* p = ((char*)(_tr_strz(text)));
#line 129 "src/doc.tr"
    while ((i < n)) {
#line 130 "src/doc.tr"
        long long c = 0LL;
#line 131 "src/doc.tr"
        /* unsafe block */
#line 131 "src/doc.tr"
        c = ((long long)((*(p + i))));
#line 132 "src/doc.tr"
        if ((c == 10LL)) {
#line 133 "src/doc.tr"
            ({ TrStr _at_t2784 = (_tr_str_slicev((text), start, i)); List_TrStr_append(out, _at_t2784); _tr_str_release(_at_t2784); });
#line 134 "src/doc.tr"
            start = (i + 1LL);
        }
#line 135 "src/doc.tr"
        i = (i + 1LL);
    }
#line 136 "src/doc.tr"
    if ((start < n)) {
#line 136 "src/doc.tr"
        ({ TrStr _at_t2785 = (_tr_str_slicev((text), start, n)); List_TrStr_append(out, _at_t2785); _tr_str_release(_at_t2785); });
    }
#line 137 "src/doc.tr"
    return out;
}

__attribute__((hot)) DocModule* doc_parse(TrStr path, TrStr text) {
#line 140 "src/doc.tr"
    DocModule* m = ((DocModule*)_tr_obj_alloc(sizeof(DocModule)));
#line 141 "src/doc.tr"
    m->path = _tr_str_retain(path);
#line 142 "src/doc.tr"
    m->name = doc_module_name(path);
#line 143 "src/doc.tr"
    m->items = (void*)List_ptr_new();
#line 144 "src/doc.tr"
    List_TrStr* lines = _doc_lines(text);
#line 145 "src/doc.tr"
    long long n = lines->len;
#line 147 "src/doc.tr"
    List_TrStr* head = (void*)List_TrStr_new();
#line 148 "src/doc.tr"
    long long i = 0LL;
#line 149 "src/doc.tr"
    bool ended_by_blank = false;
#line 150 "src/doc.tr"
    while ((i < n)) {
#line 151 "src/doc.tr"
        TrStr t = _tr_str_stripv((List_TrStr_get(lines, i)));
#line 152 "src/doc.tr"
        if (_tr_str_starts_withv((t), (_tr_str_lit_len("#", 1LL)))) {
#line 153 "src/doc.tr"
            TrStr ct = _comment_text(t);
#line 154 "src/doc.tr"
            if (((!_tr_str_starts_withv((ct), (_tr_str_lit_len("tauraro-", 8LL)))) && (!_tr_str_starts_withv((ct), (_tr_str_lit_len("!", 1LL)))))) {
#line 154 "src/doc.tr"
                List_TrStr_append(head, ct);
            }
#line 155 "src/doc.tr"
            i = (i + 1LL);
        } else if (_tr_str_eqv((t), (_tr_str_lit_len("", 0LL)))) {
#line 157 "src/doc.tr"
            if ((head->len > 0LL)) {
#line 158 "src/doc.tr"
                ended_by_blank = true;
#line 159 "src/doc.tr"
                _tr_str_release(t);
                break;
            }
#line 160 "src/doc.tr"
            i = (i + 1LL);
        } else {
#line 162 "src/doc.tr"
            _tr_str_release(t);
            break;
        }
        _tr_str_release(t);
    }
#line 164 "src/doc.tr"
    if ((!ended_by_blank)) {
#line 165 "src/doc.tr"
        List_TrStr_clear(head);
#line 166 "src/doc.tr"
        i = 0LL;
    }
#line 167 "src/doc.tr"
    m->doc = _join_doc(head);
#line 169 "src/doc.tr"
    long long skip_until = i;
#line 170 "src/doc.tr"
    List_TrStr* pending = (void*)List_TrStr_new();
#line 171 "src/doc.tr"
    long long pending_indent = (-1LL);
#line 172 "src/doc.tr"
    TrStr scope_kind = _tr_str_lit_len("", 0LL);
#line 173 "src/doc.tr"
    TrStr scope_name = _tr_str_lit_len("", 0LL);
#line 174 "src/doc.tr"
    long long scope_indent = (-1LL);
#line 175 "src/doc.tr"
    i = 0LL;
#line 176 "src/doc.tr"
    while ((i < n)) {
#line 177 "src/doc.tr"
        TrStr raw = _tr_str_replacev((List_TrStr_get(lines, i)), (_tr_str_lit_len("\r", 1LL)), (_tr_str_lit_len("", 0LL)));
#line 178 "src/doc.tr"
        TrStr t = _tr_str_stripv((raw));
#line 179 "src/doc.tr"
        long long ind = _indent_of(raw);
#line 180 "src/doc.tr"
        if (_tr_str_eqv((t), (_tr_str_lit_len("", 0LL)))) {
#line 181 "src/doc.tr"
            List_TrStr_clear(pending);
#line 182 "src/doc.tr"
            i = (i + 1LL);
#line 183 "src/doc.tr"
            _tr_str_release(t);
            _tr_str_release(raw);
            continue;
        }
#line 184 "src/doc.tr"
        if (_tr_str_starts_withv((t), (_tr_str_lit_len("#", 1LL)))) {
#line 185 "src/doc.tr"
            if ((i >= skip_until)) {
#line 186 "src/doc.tr"
                if (((pending->len == 0LL) || (pending_indent != ind))) {
#line 187 "src/doc.tr"
                    List_TrStr_clear(pending);
#line 188 "src/doc.tr"
                    pending_indent = ind;
                }
#line 189 "src/doc.tr"
                ({ TrStr _at_t2786 = (_comment_text(t)); List_TrStr_append(pending, _at_t2786); _tr_str_release(_at_t2786); });
            }
#line 190 "src/doc.tr"
            i = (i + 1LL);
#line 191 "src/doc.tr"
            _tr_str_release(t);
            _tr_str_release(raw);
            continue;
        }
#line 192 "src/doc.tr"
        if (((scope_indent >= 0LL) && (ind <= scope_indent))) {
#line 193 "src/doc.tr"
            TrStr _strtmp_t2787 = _tr_str_lit_len("", 0LL);
            _tr_str_release(scope_kind);
            scope_kind = _strtmp_t2787;
#line 194 "src/doc.tr"
            TrStr _strtmp_t2788 = _tr_str_lit_len("", 0LL);
            _tr_str_release(scope_name);
            scope_name = _strtmp_t2788;
#line 195 "src/doc.tr"
            scope_indent = (-1LL);
        }
#line 197 "src/doc.tr"
        TrStr decl = _tr_str_retain(t);
#line 198 "src/doc.tr"
        long long j = i;
#line 199 "src/doc.tr"
        while (((_bracket_depth(decl) > 0LL) && ((j + 1LL) < n))) {
#line 200 "src/doc.tr"
            j = (j + 1LL);
#line 201 "src/doc.tr"
            ({ TrStr _ap_t2789 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t2790 = (_tr_str_stripv((List_TrStr_get(lines, j)))); _tr_str_append(&decl, _ap_t2789); _tr_str_append(&decl, _ap_t2790); _tr_str_release(_ap_t2790); });
        }
#line 202 "src/doc.tr"
        TrStr doc = _join_doc(pending);
#line 203 "src/doc.tr"
        List_TrStr_clear(pending);
#line 204 "src/doc.tr"
        if ((ind == 0LL)) {
#line 205 "src/doc.tr"
            if (_tr_str_starts_withv((decl), (_tr_str_lit_len("pub def ", 8LL)))) {
#line 206 "src/doc.tr"
                ({ TrStr _at_t2791 = (_first_word_after(decl, _tr_str_lit_len("def", 3LL))); TrStr _at_t2792 = (_signature(decl)); DocItem* _ord_t2793 = (_new_item(_tr_str_lit_len("def", 3LL), _at_t2791, _at_t2792, doc, _tr_str_lit_len("", 0LL))); List_ptr_append(m->items, _tr_obj_retain(_ord_t2793)); _tr_str_release(_at_t2791); _tr_str_release(_at_t2792); _tr_obj_release(_ord_t2793, _trdrop_DocItem); });
            } else if (_tr_str_starts_withv((decl), (_tr_str_lit_len("pub const ", 10LL)))) {
#line 208 "src/doc.tr"
                TrStr cs = _tr_str_slicev((decl), 4LL, _tr_str_lenv((decl)));
#line 209 "src/doc.tr"
                long long hash = _tr_str_index_ofv((cs), (_tr_str_lit_len("  #", 3LL)));
#line 210 "src/doc.tr"
                if ((hash > 0LL)) {
#line 211 "src/doc.tr"
                    if (_tr_str_eqv((doc), (_tr_str_lit_len("", 0LL)))) {
#line 211 "src/doc.tr"
                        TrStr _strtmp_t2794 = _tr_str_stripv((_tr_str_slicev((cs), (hash + 3LL), _tr_str_lenv((cs)))));
                        _tr_str_release(doc);
                        doc = _strtmp_t2794;
                    }
#line 212 "src/doc.tr"
                    TrStr _strtmp_t2795 = _tr_str_stripv((_tr_str_slicev((cs), 0LL, hash)));
                    _tr_str_release(cs);
                    cs = _strtmp_t2795;
                }
#line 213 "src/doc.tr"
                ({ TrStr _at_t2796 = (_first_word_after(decl, _tr_str_lit_len("const", 5LL))); DocItem* _ord_t2797 = (_new_item(_tr_str_lit_len("const", 5LL), _at_t2796, cs, doc, _tr_str_lit_len("", 0LL))); List_ptr_append(m->items, _tr_obj_retain(_ord_t2797)); _tr_str_release(_at_t2796); _tr_obj_release(_ord_t2797, _trdrop_DocItem); });
                _tr_str_release(cs);
            } else if (_tr_str_starts_withv((decl), (_tr_str_lit_len("pub class ", 10LL)))) {
#line 215 "src/doc.tr"
                TrStr cn = _first_word_after(decl, _tr_str_lit_len("class", 5LL));
#line 216 "src/doc.tr"
                ({ TrStr _at_t2798 = (_signature(decl)); DocItem* _ord_t2799 = (_new_item(_tr_str_lit_len("class", 5LL), cn, _at_t2798, doc, _tr_str_lit_len("", 0LL))); List_ptr_append(m->items, _tr_obj_retain(_ord_t2799)); _tr_str_release(_at_t2798); _tr_obj_release(_ord_t2799, _trdrop_DocItem); });
#line 217 "src/doc.tr"
                TrStr _strtmp_t2800 = _tr_str_lit_len("class", 5LL);
                _tr_str_release(scope_kind);
                scope_kind = _strtmp_t2800;
#line 218 "src/doc.tr"
                TrStr _strtmp_t2801 = _tr_str_retain(cn);
                _tr_str_release(scope_name);
                scope_name = _strtmp_t2801;
#line 219 "src/doc.tr"
                scope_indent = 0LL;
                _tr_str_release(cn);
            } else if (_tr_str_starts_withv((decl), (_tr_str_lit_len("pub enum ", 9LL)))) {
#line 221 "src/doc.tr"
                TrStr en = _first_word_after(decl, _tr_str_lit_len("enum", 4LL));
#line 222 "src/doc.tr"
                ({ TrStr _at_t2802 = (_signature(decl)); DocItem* _ord_t2803 = (_new_item(_tr_str_lit_len("enum", 4LL), en, _at_t2802, doc, _tr_str_lit_len("", 0LL))); List_ptr_append(m->items, _tr_obj_retain(_ord_t2803)); _tr_str_release(_at_t2802); _tr_obj_release(_ord_t2803, _trdrop_DocItem); });
#line 223 "src/doc.tr"
                TrStr _strtmp_t2804 = _tr_str_lit_len("enum", 4LL);
                _tr_str_release(scope_kind);
                scope_kind = _strtmp_t2804;
#line 224 "src/doc.tr"
                TrStr _strtmp_t2805 = _tr_str_retain(en);
                _tr_str_release(scope_name);
                scope_name = _strtmp_t2805;
#line 225 "src/doc.tr"
                scope_indent = 0LL;
                _tr_str_release(en);
            } else if (_tr_str_starts_withv((decl), (_tr_str_lit_len("pub interface ", 14LL)))) {
#line 227 "src/doc.tr"
                TrStr inn = _first_word_after(decl, _tr_str_lit_len("interface", 9LL));
#line 228 "src/doc.tr"
                ({ TrStr _at_t2806 = (_signature(decl)); DocItem* _ord_t2807 = (_new_item(_tr_str_lit_len("interface", 9LL), inn, _at_t2806, doc, _tr_str_lit_len("", 0LL))); List_ptr_append(m->items, _tr_obj_retain(_ord_t2807)); _tr_str_release(_at_t2806); _tr_obj_release(_ord_t2807, _trdrop_DocItem); });
#line 229 "src/doc.tr"
                TrStr _strtmp_t2808 = _tr_str_lit_len("interface", 9LL);
                _tr_str_release(scope_kind);
                scope_kind = _strtmp_t2808;
#line 230 "src/doc.tr"
                TrStr _strtmp_t2809 = _tr_str_retain(inn);
                _tr_str_release(scope_name);
                scope_name = _strtmp_t2809;
#line 231 "src/doc.tr"
                scope_indent = 0LL;
                _tr_str_release(inn);
            } else if (_tr_str_starts_withv((decl), (_tr_str_lit_len("extend ", 7LL)))) {
#line 233 "src/doc.tr"
                TrStr xn = _first_word_after(decl, _tr_str_lit_len("extend", 6LL));
#line 234 "src/doc.tr"
                TrStr _strtmp_t2810 = _tr_str_lit_len("extend", 6LL);
                _tr_str_release(scope_kind);
                scope_kind = _strtmp_t2810;
#line 235 "src/doc.tr"
                TrStr _strtmp_t2811 = _tr_str_retain(xn);
                _tr_str_release(scope_name);
                scope_name = _strtmp_t2811;
#line 236 "src/doc.tr"
                scope_indent = 0LL;
                _tr_str_release(xn);
            }
        } else if ((!_tr_str_eqv((scope_kind), (_tr_str_lit_len("", 0LL))))) {
#line 238 "src/doc.tr"
            if (((_tr_str_eqv((scope_kind), (_tr_str_lit_len("extend", 6LL))) || _tr_str_eqv((scope_kind), (_tr_str_lit_len("interface", 9LL)))) && _tr_str_starts_withv((decl), (_tr_str_lit_len("pub def ", 8LL))))) {
#line 239 "src/doc.tr"
                TrStr sig = _signature(decl);
#line 240 "src/doc.tr"
                TrStr kind = _tr_str_lit_len("method", 6LL);
#line 241 "src/doc.tr"
                if ((_tr_str_index_ofv((sig), (_tr_str_lit_len("(self", 5LL))) < 0LL)) {
#line 241 "src/doc.tr"
                    TrStr _strtmp_t2812 = _tr_str_lit_len("static", 6LL);
                    _tr_str_release(kind);
                    kind = _strtmp_t2812;
                }
#line 242 "src/doc.tr"
                ({ TrStr _at_t2813 = (_first_word_after(decl, _tr_str_lit_len("def", 3LL))); DocItem* _ord_t2814 = (_new_item(kind, _at_t2813, sig, doc, scope_name)); List_ptr_append(m->items, _tr_obj_retain(_ord_t2814)); _tr_str_release(_at_t2813); _tr_obj_release(_ord_t2814, _trdrop_DocItem); });
                _tr_str_release(sig);
                _tr_str_release(kind);
            } else if ((_tr_str_eqv((scope_kind), (_tr_str_lit_len("interface", 9LL))) && _tr_str_starts_withv((decl), (_tr_str_lit_len("def ", 4LL))))) {
#line 244 "src/doc.tr"
                ({ TrStr _at_t2815 = (_first_word_after(decl, _tr_str_lit_len("def", 3LL))); TrStr _at_t2816 = (_signature(decl)); DocItem* _ord_t2817 = (_new_item(_tr_str_lit_len("method", 6LL), _at_t2815, _at_t2816, doc, scope_name)); List_ptr_append(m->items, _tr_obj_retain(_ord_t2817)); _tr_str_release(_at_t2815); _tr_str_release(_at_t2816); _tr_obj_release(_ord_t2817, _trdrop_DocItem); });
            } else if (((_tr_str_eqv((scope_kind), (_tr_str_lit_len("class", 5LL))) && _tr_str_starts_withv((decl), (_tr_str_lit_len("pub ", 4LL)))) && (!_tr_str_starts_withv((decl), (_tr_str_lit_len("pub def ", 8LL)))))) {
#line 246 "src/doc.tr"
                TrStr f = _tr_str_slicev((decl), 4LL, _tr_str_lenv((decl)));
#line 247 "src/doc.tr"
                TrStr fdoc = _tr_str_retain(doc);
#line 248 "src/doc.tr"
                long long hash = _tr_str_index_ofv((f), (_tr_str_lit_len("#", 1LL)));
#line 249 "src/doc.tr"
                if ((hash > 0LL)) {
#line 250 "src/doc.tr"
                    if (_tr_str_eqv((fdoc), (_tr_str_lit_len("", 0LL)))) {
#line 250 "src/doc.tr"
                        TrStr _strtmp_t2818 = _tr_str_stripv((_tr_str_slicev((f), (hash + 1LL), _tr_str_lenv((f)))));
                        _tr_str_release(fdoc);
                        fdoc = _strtmp_t2818;
                    }
#line 251 "src/doc.tr"
                    TrStr _strtmp_t2819 = _tr_str_stripv((_tr_str_slicev((f), 0LL, hash)));
                    _tr_str_release(f);
                    f = _strtmp_t2819;
                }
#line 252 "src/doc.tr"
                long long colon = _tr_str_index_ofv((f), (_tr_str_lit_len(":", 1LL)));
#line 253 "src/doc.tr"
                TrStr fname = _tr_str_retain(f);
#line 254 "src/doc.tr"
                if ((colon > 0LL)) {
#line 254 "src/doc.tr"
                    TrStr _strtmp_t2820 = _tr_str_stripv((_tr_str_slicev((f), 0LL, colon)));
                    _tr_str_release(fname);
                    fname = _strtmp_t2820;
                }
#line 255 "src/doc.tr"
                ({ DocItem* _ord_t2821 = (_new_item(_tr_str_lit_len("field", 5LL), fname, f, fdoc, scope_name)); List_ptr_append(m->items, _tr_obj_retain(_ord_t2821)); _tr_obj_release(_ord_t2821, _trdrop_DocItem); });
                _tr_str_release(f);
                _tr_str_release(fdoc);
                _tr_str_release(fname);
            } else if (((_tr_str_eqv((scope_kind), (_tr_str_lit_len("enum", 4LL))) && (!_tr_str_starts_withv((decl), (_tr_str_lit_len("def ", 4LL))))) && (!_tr_str_starts_withv((decl), (_tr_str_lit_len("pub def ", 8LL)))))) {
#line 257 "src/doc.tr"
                TrStr v = _tr_str_retain(decl);
#line 258 "src/doc.tr"
                TrStr vdoc = _tr_str_retain(doc);
#line 259 "src/doc.tr"
                long long hash2 = _tr_str_index_ofv((v), (_tr_str_lit_len("#", 1LL)));
#line 260 "src/doc.tr"
                if ((hash2 > 0LL)) {
#line 261 "src/doc.tr"
                    if (_tr_str_eqv((vdoc), (_tr_str_lit_len("", 0LL)))) {
#line 261 "src/doc.tr"
                        TrStr _strtmp_t2822 = _tr_str_stripv((_tr_str_slicev((v), (hash2 + 1LL), _tr_str_lenv((v)))));
                        _tr_str_release(vdoc);
                        vdoc = _strtmp_t2822;
                    }
#line 262 "src/doc.tr"
                    TrStr _strtmp_t2823 = _tr_str_stripv((_tr_str_slicev((v), 0LL, hash2)));
                    _tr_str_release(v);
                    v = _strtmp_t2823;
                }
#line 263 "src/doc.tr"
                TrStr vn = _tr_str_retain(v);
#line 264 "src/doc.tr"
                long long po = _tr_str_index_ofv((v), (_tr_str_lit_len("(", 1LL)));
#line 265 "src/doc.tr"
                if ((po > 0LL)) {
#line 265 "src/doc.tr"
                    TrStr _strtmp_t2824 = _tr_str_slicev((v), 0LL, po);
                    _tr_str_release(vn);
                    vn = _strtmp_t2824;
                }
#line 266 "src/doc.tr"
                ({ TrStr _at_t2825 = (_tr_str_stripv((vn))); DocItem* _ord_t2826 = (_new_item(_tr_str_lit_len("variant", 7LL), _at_t2825, v, vdoc, scope_name)); List_ptr_append(m->items, _tr_obj_retain(_ord_t2826)); _tr_str_release(_at_t2825); _tr_obj_release(_ord_t2826, _trdrop_DocItem); });
                _tr_str_release(v);
                _tr_str_release(vdoc);
                _tr_str_release(vn);
            }
        }
#line 267 "src/doc.tr"
        i = (j + 1LL);
        _tr_str_release(t);
        _tr_str_release(raw);
        _tr_str_release(decl);
        _tr_str_release(doc);
    }
#line 268 "src/doc.tr"
    List_TrStr_free(lines);
    _tr_str_release(scope_kind);
    _tr_str_release(scope_name);
    return m;
}

__attribute__((hot)) bool _has_kind(DocModule* m, TrStr kind, TrStr owner) {
#line 273 "src/doc.tr"
    long long i = 0LL;
#line 274 "src/doc.tr"
    while ((i < m->items->len)) {
#line 275 "src/doc.tr"
        DocItem* it = ((DocItem*)List_ptr_get(m->items, i));
#line 276 "src/doc.tr"
        if ((_tr_str_eqv((it->kind), (kind)) && _tr_str_eqv((it->owner), (owner)))) {
#line 276 "src/doc.tr"
            return true;
        }
#line 277 "src/doc.tr"
        i = (i + 1LL);
    }
#line 278 "src/doc.tr"
    return false;
}

__attribute__((hot)) TrStr _md_cell(TrStr s) {
#line 281 "src/doc.tr"
    return _tr_str_replacev((_tr_str_replacev((s), (_tr_str_lit_len("|", 1LL)), (_tr_str_lit_len("\\|", 2LL)))), (_tr_str_lit_len("\n", 1LL)), (_tr_str_lit_len(" ", 1LL)));
}

__attribute__((hot)) void _md_members(StringBuilder* sb, DocModule* m, TrStr owner) {
#line 284 "src/doc.tr"
    if ((_has_kind(m, _tr_str_lit_len("field", 5LL), owner) || _has_kind(m, _tr_str_lit_len("variant", 7LL), owner))) {
#line 285 "src/doc.tr"
        StringBuilder_append(sb, _tr_str_lit_len("\n| Member | Description |\n|---|---|\n", 36LL));
#line 286 "src/doc.tr"
        long long i = 0LL;
#line 287 "src/doc.tr"
        while ((i < m->items->len)) {
#line 288 "src/doc.tr"
            DocItem* it = ((DocItem*)List_ptr_get(m->items, i));
#line 289 "src/doc.tr"
            if ((_tr_str_eqv((it->owner), (owner)) && (_tr_str_eqv((it->kind), (_tr_str_lit_len("field", 5LL))) || _tr_str_eqv((it->kind), (_tr_str_lit_len("variant", 7LL)))))) {
#line 290 "src/doc.tr"
                ({ __auto_type _ord_t2827 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_md_cell(it->sig)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("| `", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("` | ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2828 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2827); TrStr _tr_cr = (_md_cell(it->doc)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" |\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2828); _tr_str_release(_sbt_t2828); });
            }
#line 291 "src/doc.tr"
            i = (i + 1LL);
        }
    }
#line 292 "src/doc.tr"
    long long j = 0LL;
#line 293 "src/doc.tr"
    while ((j < m->items->len)) {
#line 294 "src/doc.tr"
        DocItem* mt = ((DocItem*)List_ptr_get(m->items, j));
#line 295 "src/doc.tr"
        if ((_tr_str_eqv((mt->owner), (owner)) && (_tr_str_eqv((mt->kind), (_tr_str_lit_len("method", 6LL))) || _tr_str_eqv((mt->kind), (_tr_str_lit_len("static", 6LL)))))) {
#line 296 "src/doc.tr"
            ({ TrStr _sbt_t2829 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("\n#### `", 7LL)), (owner))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mt->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("`\n\n```tauraro\n", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mt->sig)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n```\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2829); _tr_str_release(_sbt_t2829); });
#line 297 "src/doc.tr"
            if ((!_tr_str_eqv((mt->doc), (_tr_str_lit_len("", 0LL))))) {
#line 297 "src/doc.tr"
                ({ TrStr _sbt_t2830 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("\n", 1LL)), (mt->doc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2830); _tr_str_release(_sbt_t2830); });
            }
        }
#line 298 "src/doc.tr"
        j = (j + 1LL);
    }
}

__attribute__((hot)) TrStr doc_markdown(DocModule* m) {
#line 301 "src/doc.tr"
    StringBuilder* sb = StringBuilder_init(4096LL);
#line 302 "src/doc.tr"
    ({ TrStr _sbt_t2831 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("# ", 2LL)), (m->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2831); _tr_str_release(_sbt_t2831); });
#line 303 "src/doc.tr"
    ({ TrStr _sbt_t2832 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("`", 1LL)), (m->path))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("`\n\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2832); _tr_str_release(_sbt_t2832); });
#line 304 "src/doc.tr"
    if ((!_tr_str_eqv((m->doc), (_tr_str_lit_len("", 0LL))))) {
#line 304 "src/doc.tr"
        ({ TrStr _sbt_t2833 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("```text\n", 8LL)), (m->doc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n```\n\n", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2833); _tr_str_release(_sbt_t2833); });
    }
#line 305 "src/doc.tr"
    if (_has_kind(m, _tr_str_lit_len("const", 5LL), _tr_str_lit_len("", 0LL))) {
#line 306 "src/doc.tr"
        StringBuilder_append(sb, _tr_str_lit_len("## Constants\n\n| Constant | Description |\n|---|---|\n", 51LL));
#line 307 "src/doc.tr"
        long long i = 0LL;
#line 308 "src/doc.tr"
        while ((i < m->items->len)) {
#line 309 "src/doc.tr"
            DocItem* it = ((DocItem*)List_ptr_get(m->items, i));
#line 310 "src/doc.tr"
            if (_tr_str_eqv((it->kind), (_tr_str_lit_len("const", 5LL)))) {
#line 310 "src/doc.tr"
                ({ __auto_type _ord_t2834 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_md_cell(it->sig)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("| `", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("` | ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2835 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2834); TrStr _tr_cr = (_md_cell(it->doc)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" |\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2835); _tr_str_release(_sbt_t2835); });
            }
#line 311 "src/doc.tr"
            i = (i + 1LL);
        }
#line 312 "src/doc.tr"
        StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
    }
#line 313 "src/doc.tr"
    if (_has_kind(m, _tr_str_lit_len("def", 3LL), _tr_str_lit_len("", 0LL))) {
#line 314 "src/doc.tr"
        StringBuilder_append(sb, _tr_str_lit_len("## Functions\n", 13LL));
#line 315 "src/doc.tr"
        long long i2 = 0LL;
#line 316 "src/doc.tr"
        while ((i2 < m->items->len)) {
#line 317 "src/doc.tr"
            DocItem* it2 = ((DocItem*)List_ptr_get(m->items, i2));
#line 318 "src/doc.tr"
            if (_tr_str_eqv((it2->kind), (_tr_str_lit_len("def", 3LL)))) {
#line 319 "src/doc.tr"
                ({ TrStr _sbt_t2836 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("\n### `", 6LL)), (it2->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("`\n\n```tauraro\n", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (it2->sig)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n```\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2836); _tr_str_release(_sbt_t2836); });
#line 320 "src/doc.tr"
                if ((!_tr_str_eqv((it2->doc), (_tr_str_lit_len("", 0LL))))) {
#line 320 "src/doc.tr"
                    ({ TrStr _sbt_t2837 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("\n", 1LL)), (it2->doc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2837); _tr_str_release(_sbt_t2837); });
                }
            }
#line 321 "src/doc.tr"
            i2 = (i2 + 1LL);
        }
#line 322 "src/doc.tr"
        StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
    }
#line 323 "src/doc.tr"
    long long k = 0LL;
#line 324 "src/doc.tr"
    while ((k < m->items->len)) {
#line 325 "src/doc.tr"
        DocItem* c = ((DocItem*)List_ptr_get(m->items, k));
#line 326 "src/doc.tr"
        if (((_tr_str_eqv((c->kind), (_tr_str_lit_len("class", 5LL))) || _tr_str_eqv((c->kind), (_tr_str_lit_len("enum", 4LL)))) || _tr_str_eqv((c->kind), (_tr_str_lit_len("interface", 9LL))))) {
#line 327 "src/doc.tr"
            ({ TrStr _sbt_t2838 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("## ", 3LL)), (c->kind))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" `", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("`\n\n```tauraro\n", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->sig)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n```\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2838); _tr_str_release(_sbt_t2838); });
#line 328 "src/doc.tr"
            if ((!_tr_str_eqv((c->doc), (_tr_str_lit_len("", 0LL))))) {
#line 328 "src/doc.tr"
                ({ TrStr _sbt_t2839 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("\n", 1LL)), (c->doc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2839); _tr_str_release(_sbt_t2839); });
            }
#line 329 "src/doc.tr"
            _md_members(sb, m, c->name);
#line 330 "src/doc.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
        }
#line 331 "src/doc.tr"
        k = (k + 1LL);
    }
#line 332 "src/doc.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _html_esc(TrStr s) {
#line 335 "src/doc.tr"
    return _tr_str_replacev((_tr_str_replacev((_tr_str_replacev((_tr_str_replacev((s), (_tr_str_lit_len("&", 1LL)), (_tr_str_lit_len("&amp;", 5LL)))), (_tr_str_lit_len("<", 1LL)), (_tr_str_lit_len("&lt;", 4LL)))), (_tr_str_lit_len(">", 1LL)), (_tr_str_lit_len("&gt;", 4LL)))), (_tr_str_lit_len("\"", 1LL)), (_tr_str_lit_len("&quot;", 6LL)));
}

__attribute__((hot)) TrStr _html_doc(TrStr s) {
#line 338 "src/doc.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("", 0LL)))) {
#line 338 "src/doc.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 339 "src/doc.tr"
    return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_html_esc(s)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("<pre class=\"doc\">", 17LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</pre>\n", 7LL))); _tr_str_release(_tr_cl); _tr_cres; });
}

__attribute__((hot)) TrStr _html_anchor(TrStr s) {
#line 341 "src/doc.tr"
    return _tr_str_replacev((s), (_tr_str_lit_len(".", 1LL)), (_tr_str_lit_len("-", 1LL)));
}

__attribute__((hot)) TrStr doc_html(DocModule* m) {
#line 344 "src/doc.tr"
    StringBuilder* sb = StringBuilder_init(8192LL);
#line 345 "src/doc.tr"
    StringBuilder_append(sb, _tr_str_lit_len("<!doctype html>\n<html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n", 129LL));
#line 346 "src/doc.tr"
    ({ __auto_type _ord_t2840 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_html_esc(m->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("<title>", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</title>\n<style>", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2841 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2840); TrStr _tr_cr = (doc_css()); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</style></head><body><main>\n", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2841); _tr_str_release(_sbt_t2841); });
#line 347 "src/doc.tr"
    ({ __auto_type _ord_t2842 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_html_esc(m->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("<p><a href=\"index.html\">Index</a></p>\n<h1>", 42LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</h1>\n<p class=\"path\">", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2843 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2842); TrStr _tr_cr = (_html_esc(m->path)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</p>\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2843); _tr_str_release(_sbt_t2843); });
#line 348 "src/doc.tr"
    ({ TrStr _sbt_t2844 = (_html_doc(m->doc)); StringBuilder_append(sb, _sbt_t2844); _tr_str_release(_sbt_t2844); });
#line 349 "src/doc.tr"
    if (_has_kind(m, _tr_str_lit_len("const", 5LL), _tr_str_lit_len("", 0LL))) {
#line 350 "src/doc.tr"
        StringBuilder_append(sb, _tr_str_lit_len("<h2>Constants</h2>\n<table>\n", 27LL));
#line 351 "src/doc.tr"
        long long i = 0LL;
#line 352 "src/doc.tr"
        while ((i < m->items->len)) {
#line 353 "src/doc.tr"
            DocItem* it = ((DocItem*)List_ptr_get(m->items, i));
#line 354 "src/doc.tr"
            if (_tr_str_eqv((it->kind), (_tr_str_lit_len("const", 5LL)))) {
#line 354 "src/doc.tr"
                ({ __auto_type _ord_t2845 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_html_esc(it->sig)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("<tr><td><code>", 14LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</code></td><td>", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2846 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2845); TrStr _tr_cr = (_html_esc(it->doc)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</td></tr>\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2846); _tr_str_release(_sbt_t2846); });
            }
#line 355 "src/doc.tr"
            i = (i + 1LL);
        }
#line 356 "src/doc.tr"
        StringBuilder_append(sb, _tr_str_lit_len("</table>\n", 9LL));
    }
#line 357 "src/doc.tr"
    if (_has_kind(m, _tr_str_lit_len("def", 3LL), _tr_str_lit_len("", 0LL))) {
#line 358 "src/doc.tr"
        StringBuilder_append(sb, _tr_str_lit_len("<h2>Functions</h2>\n", 19LL));
#line 359 "src/doc.tr"
        long long i2 = 0LL;
#line 360 "src/doc.tr"
        while ((i2 < m->items->len)) {
#line 361 "src/doc.tr"
            DocItem* it2 = ((DocItem*)List_ptr_get(m->items, i2));
#line 362 "src/doc.tr"
            if (_tr_str_eqv((it2->kind), (_tr_str_lit_len("def", 3LL)))) {
#line 363 "src/doc.tr"
                ({ __auto_type _ord_t2847 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_html_anchor(it2->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("<h3 id=\"", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\">", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2848 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2847); TrStr _tr_cr = (_html_esc(it2->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</h3>\n<pre class=\"sig\">", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2849 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2848); TrStr _tr_cr = (_html_esc(it2->sig)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</pre>\n", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2850 = (({ TrStr _tr_cl = (_ord_t2849); TrStr _tr_cr = (_html_doc(it2->doc)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(sb, _sbt_t2850); _tr_str_release(_sbt_t2850); });
            }
#line 364 "src/doc.tr"
            i2 = (i2 + 1LL);
        }
    }
#line 365 "src/doc.tr"
    long long k = 0LL;
#line 366 "src/doc.tr"
    while ((k < m->items->len)) {
#line 367 "src/doc.tr"
        DocItem* c = ((DocItem*)List_ptr_get(m->items, k));
#line 368 "src/doc.tr"
        if (((_tr_str_eqv((c->kind), (_tr_str_lit_len("class", 5LL))) || _tr_str_eqv((c->kind), (_tr_str_lit_len("enum", 4LL)))) || _tr_str_eqv((c->kind), (_tr_str_lit_len("interface", 9LL))))) {
#line 369 "src/doc.tr"
            ({ __auto_type _ord_t2851 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_html_anchor(c->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("<h2 id=\"", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\">", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2852 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2851); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->kind)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2853 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2852); TrStr _tr_cr = (_html_esc(c->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</h2>\n<pre class=\"sig\">", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2854 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2853); TrStr _tr_cr = (_html_esc(c->sig)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</pre>\n", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2855 = (({ TrStr _tr_cl = (_ord_t2854); TrStr _tr_cr = (_html_doc(c->doc)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(sb, _sbt_t2855); _tr_str_release(_sbt_t2855); });
#line 370 "src/doc.tr"
            if ((_has_kind(m, _tr_str_lit_len("field", 5LL), c->name) || _has_kind(m, _tr_str_lit_len("variant", 7LL), c->name))) {
#line 371 "src/doc.tr"
                StringBuilder_append(sb, _tr_str_lit_len("<table>\n", 8LL));
#line 372 "src/doc.tr"
                long long f = 0LL;
#line 373 "src/doc.tr"
                while ((f < m->items->len)) {
#line 374 "src/doc.tr"
                    DocItem* fi = ((DocItem*)List_ptr_get(m->items, f));
#line 375 "src/doc.tr"
                    if ((_tr_str_eqv((fi->owner), (c->name)) && (_tr_str_eqv((fi->kind), (_tr_str_lit_len("field", 5LL))) || _tr_str_eqv((fi->kind), (_tr_str_lit_len("variant", 7LL)))))) {
#line 376 "src/doc.tr"
                        ({ __auto_type _ord_t2856 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_html_esc(fi->sig)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("<tr><td><code>", 14LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</code></td><td>", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2857 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2856); TrStr _tr_cr = (_html_esc(fi->doc)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</td></tr>\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2857); _tr_str_release(_sbt_t2857); });
                    }
#line 377 "src/doc.tr"
                    f = (f + 1LL);
                }
#line 378 "src/doc.tr"
                StringBuilder_append(sb, _tr_str_lit_len("</table>\n", 9LL));
            }
#line 379 "src/doc.tr"
            long long q = 0LL;
#line 380 "src/doc.tr"
            while ((q < m->items->len)) {
#line 381 "src/doc.tr"
                DocItem* mt = ((DocItem*)List_ptr_get(m->items, q));
#line 382 "src/doc.tr"
                if ((_tr_str_eqv((mt->owner), (c->name)) && (_tr_str_eqv((mt->kind), (_tr_str_lit_len("method", 6LL))) || _tr_str_eqv((mt->kind), (_tr_str_lit_len("static", 6LL)))))) {
#line 383 "src/doc.tr"
                    ({ TrStr _at_t2858 = (({ TrStr _tr_cl = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mt->name)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2860 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_html_anchor(_at_t2858)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("<h4 id=\"", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\">", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2859 = (({ TrStr _tr_cl = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mt->name)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2861 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2860); TrStr _tr_cr = (_html_esc(_at_t2859)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</h4>\n<pre class=\"sig\">", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2862 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2861); TrStr _tr_cr = (_html_esc(mt->sig)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</pre>\n", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2863 = (({ TrStr _tr_cl = (_ord_t2862); TrStr _tr_cr = (_html_doc(mt->doc)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(sb, _sbt_t2863); _tr_str_release(_at_t2858); _tr_str_release(_at_t2859); _tr_str_release(_sbt_t2863); });
                }
#line 384 "src/doc.tr"
                q = (q + 1LL);
            }
        }
#line 385 "src/doc.tr"
        k = (k + 1LL);
    }
#line 386 "src/doc.tr"
    StringBuilder_append(sb, _tr_str_lit_len("</main></body></html>\n", 22LL));
#line 387 "src/doc.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr doc_css() {
#line 390 "src/doc.tr"
    return _tr_str_lit_len(":root{--bg:#fff;--fg:#1b1f24;--muted:#57606a;--code:#f3f4f6;--line:#d8dee4;--link:#0b62c4}@media (prefers-color-scheme:dark){:root{--bg:#0f1115;--fg:#e6e8eb;--muted:#9aa3ad;--code:#1a1e24;--line:#2b313a;--link:#6cb0ff}}body{background:var(--bg);color:var(--fg);font:16px/1.55 system-ui,sans-serif;margin:0}main{max-width:900px;margin:0 auto;padding:24px 16px}a{color:var(--link)}pre,code{font:14px/1.45 ui-monospace,Menlo,Consolas,monospace}pre{background:var(--code);padding:10px 12px;border-radius:6px;overflow-x:auto}pre.doc{background:none;padding:0 0 0 2px;white-space:pre-wrap}.path{color:var(--muted)}table{border-collapse:collapse;width:100%}td{border-top:1px solid var(--line);padding:6px 8px;vertical-align:top}h2{border-bottom:1px solid var(--line);padding-bottom:4px;margin-top:2em}", 794LL);
}

__attribute__((hot)) TrStr doc_index_markdown(List_ptr* mods) {
#line 393 "src/doc.tr"
    StringBuilder* sb = StringBuilder_init(2048LL);
#line 394 "src/doc.tr"
    StringBuilder_append(sb, _tr_str_lit_len("# API reference\n\n| Module | Summary |\n|---|---|\n", 48LL));
#line 395 "src/doc.tr"
    long long i = 0LL;
#line 396 "src/doc.tr"
    while ((i < mods->len)) {
#line 397 "src/doc.tr"
        DocModule* m = ((DocModule*)List_ptr_get(mods, i));
#line 398 "src/doc.tr"
        ({ __auto_type _ord_t2865 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("| [`", 4LL)), (m->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("`](", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".md) | ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2864 = (_summary(m->doc)); TrStr _sbt_t2866 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2865); TrStr _tr_cr = (_md_cell(_at_t2864)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" |\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2866); _tr_str_release(_at_t2864); _tr_str_release(_sbt_t2866); });
#line 399 "src/doc.tr"
        i = (i + 1LL);
    }
#line 400 "src/doc.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr doc_index_html(List_ptr* mods) {
#line 403 "src/doc.tr"
    StringBuilder* sb = StringBuilder_init(4096LL);
#line 404 "src/doc.tr"
    ({ TrStr _sbt_t2867 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (doc_css()); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("<!doctype html>\n<html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<title>API reference</title>\n<style>", 165LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</style></head><body><main>\n<h1>API reference</h1>\n<table>\n", 59LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2867); _tr_str_release(_sbt_t2867); });
#line 405 "src/doc.tr"
    long long i = 0LL;
#line 406 "src/doc.tr"
    while ((i < mods->len)) {
#line 407 "src/doc.tr"
        DocModule* m = ((DocModule*)List_ptr_get(mods, i));
#line 408 "src/doc.tr"
        ({ __auto_type _ord_t2868 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("<tr><td><a href=\"", 17LL)), (m->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".html\"><code>", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2870 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2868); TrStr _tr_cr = (_html_esc(m->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</code></a></td><td>", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2869 = (_summary(m->doc)); TrStr _sbt_t2871 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2870); TrStr _tr_cr = (_html_esc(_at_t2869)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("</td></tr>\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t2871); _tr_str_release(_at_t2869); _tr_str_release(_sbt_t2871); });
#line 409 "src/doc.tr"
        i = (i + 1LL);
    }
#line 410 "src/doc.tr"
    StringBuilder_append(sb, _tr_str_lit_len("</table>\n</main></body></html>\n", 31LL));
#line 411 "src/doc.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _summary(TrStr doc) {
#line 415 "src/doc.tr"
    TrStr first = _tr_str_retain(doc);
#line 416 "src/doc.tr"
    long long nl = _tr_str_index_ofv((doc), (_tr_str_lit_len("\n", 1LL)));
#line 417 "src/doc.tr"
    if ((nl >= 0LL)) {
#line 417 "src/doc.tr"
        TrStr _strtmp_t2872 = _tr_str_slicev((doc), 0LL, nl);
        _tr_str_release(first);
        first = _strtmp_t2872;
    }
#line 418 "src/doc.tr"
    long long dash = _tr_str_index_ofv((first), (_tr_str_lit_len(" -- ", 4LL)));
#line 419 "src/doc.tr"
    if ((dash >= 0LL)) {
#line 419 "src/doc.tr"
        TrStr _strtmp_t2873 = _tr_str_slicev((first), (dash + 4LL), _tr_str_lenv((first)));
        _tr_str_release(first);
        first = _strtmp_t2873;
    } else {
#line 421 "src/doc.tr"
        long long em = _tr_str_index_ofv((first), (_tr_str_lit_len(" — ", 5LL)));
#line 422 "src/doc.tr"
        if ((em >= 0LL)) {
#line 422 "src/doc.tr"
            TrStr _strtmp_t2874 = _tr_str_slicev((first), (em + 5LL), _tr_str_lenv((first)));
            _tr_str_release(first);
            first = _strtmp_t2874;
        }
    }
#line 423 "src/doc.tr"
    TrStr _trret599 = _tr_str_stripv((first));
    _tr_str_release(first);
    return _trret599;
}

__attribute__((hot)) List_TrStr* _doc_expand(TrStr path) {
#line 427 "src/doc.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 428 "src/doc.tr"
    if (_tr_str_ends_withv((path), (_tr_str_lit_len(".tr", 3LL)))) {
#line 429 "src/doc.tr"
        List_TrStr_append(out, path);
#line 430 "src/doc.tr"
        return out;
    }
#line 431 "src/doc.tr"
    TrStr tmp = _tr_str_lit_len("_tr_doc_list.txt", 16LL);
#line 432 "src/doc.tr"
    if (_tr_is_windows()) {
#line 433 "src/doc.tr"
        ({ TrStr _aet_t2875 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_replacev((path), (_tr_str_lit_len("/", 1LL)), (_tr_str_lit_len("\\", 1LL)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("dir /b /s /o:n \"", 16LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\\*.tr\" > ", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tmp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 2>nul", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t2875.data); _tr_str_release(_aet_t2875); });
    } else {
#line 435 "src/doc.tr"
        ({ TrStr _aet_t2876 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("find \"", 6LL)), (path))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" -name '*.tr' -type f | LC_ALL=C sort > ", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tmp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 2>/dev/null", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t2876.data); _tr_str_release(_aet_t2876); });
    }
#line 436 "src/doc.tr"
    if (file_exists(tmp)) {
#line 437 "src/doc.tr"
        List_TrStr* lines = _tr_str_splitv((read_file(tmp)), (_tr_str_lit_len("\n", 1LL)));
#line 438 "src/doc.tr"
        long long i = 0LL;
#line 439 "src/doc.tr"
        while ((i < lines->len)) {
#line 440 "src/doc.tr"
            TrStr l = _tr_str_replacev((_tr_str_stripv((List_TrStr_get(lines, i)))), (_tr_str_lit_len("\\", 1LL)), (_tr_str_lit_len("/", 1LL)));
#line 441 "src/doc.tr"
            if (_tr_str_ends_withv((l), (_tr_str_lit_len(".tr", 3LL)))) {
#line 441 "src/doc.tr"
                List_TrStr_append(out, l);
            }
#line 442 "src/doc.tr"
            i = (i + 1LL);
            _tr_str_release(l);
        }
#line 443 "src/doc.tr"
        if (_tr_is_windows()) {
#line 443 "src/doc.tr"
            ({ TrStr _aet_t2877 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("del /Q ", 7LL)), (tmp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 2>nul >nul", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t2877.data); _tr_str_release(_aet_t2877); });
        } else {
#line 444 "src/doc.tr"
            ({ TrStr _aet_t2878 = (_tr_strx_concatv((_tr_str_lit_len("rm -f ", 6LL)), (tmp))); _tr_system(_aet_t2878.data); _tr_str_release(_aet_t2878); });
        }
    }
#line 445 "src/doc.tr"
    _tr_str_release(tmp);
    return out;
}

__attribute__((hot)) long long run_doc(List_TrStr* inputs, TrStr out_dir, bool html) {
#line 450 "src/doc.tr"
    List_ptr* mods = (void*)List_ptr_new();
#line 451 "src/doc.tr"
    long long i = 0LL;
#line 452 "src/doc.tr"
    while ((i < inputs->len)) {
#line 453 "src/doc.tr"
        List_TrStr* files = ({ TrStr _at_t2879 = (List_TrStr_get(inputs, i)); __auto_type _wr = (_doc_expand(_at_t2879)); _tr_str_release(_at_t2879); _wr; });
#line 454 "src/doc.tr"
        if ((files->len == 0LL)) {
#line 455 "src/doc.tr"
            ({ printf("%s", _tr_strz(({ TrStr _tr_cr = (List_TrStr_get(inputs, i)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("error: no .tr files in ", 23LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); });
#line 456 "src/doc.tr"
            List_ptr_free_obj(mods, _trdrop_DocModule);
            List_TrStr_free(files);
            return 1LL;
        }
#line 457 "src/doc.tr"
        long long k = 0LL;
#line 458 "src/doc.tr"
        while ((k < files->len)) {
#line 459 "src/doc.tr"
            TrStr f = List_TrStr_get(files, k);
#line 460 "src/doc.tr"
            if ((!file_exists(f))) {
#line 461 "src/doc.tr"
                ({ printf("%s", _tr_strz(_tr_strx_concatv((_tr_str_lit_len("error: cannot read ", 19LL)), (f)))); printf("\n"); });
#line 462 "src/doc.tr"
                List_ptr_free_obj(mods, _trdrop_DocModule);
                List_TrStr_free(files);
                _tr_str_release(f);
                return 1LL;
            }
#line 463 "src/doc.tr"
            ({ TrStr _at_t2880 = (read_file(f)); List_ptr_append(mods, doc_parse(f, _at_t2880)); _tr_str_release(_at_t2880); });
#line 464 "src/doc.tr"
            k = (k + 1LL);
            _tr_str_release(f);
        }
#line 465 "src/doc.tr"
        i = (i + 1LL);
        List_TrStr_free(files);
    }
#line 466 "src/doc.tr"
    if (_tr_str_eqv((out_dir), (_tr_str_lit_len("", 0LL)))) {
#line 467 "src/doc.tr"
        long long j = 0LL;
#line 468 "src/doc.tr"
        while ((j < mods->len)) {
#line 469 "src/doc.tr"
            if ((j > 0LL)) {
#line 469 "src/doc.tr"
                ({ printf("%s", _tr_strz(_tr_str_lit_len("\n---\n", 5LL))); printf("\n"); });
            }
#line 470 "src/doc.tr"
            ({ printf("%s", _tr_strz(doc_markdown(((DocModule*)List_ptr_get(mods, j))))); printf("\n"); });
#line 471 "src/doc.tr"
            j = (j + 1LL);
        }
#line 472 "src/doc.tr"
        List_ptr_free_obj(mods, _trdrop_DocModule);
        return 0LL;
    }
#line 473 "src/doc.tr"
    if (_tr_is_windows()) {
#line 473 "src/doc.tr"
        ({ TrStr _aet_t2881 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_replacev((out_dir), (_tr_str_lit_len("/", 1LL)), (_tr_str_lit_len("\\", 1LL)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("mkdir \"", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" 2>nul", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t2881.data); _tr_str_release(_aet_t2881); });
    } else {
#line 474 "src/doc.tr"
        ({ TrStr _aet_t2882 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("mkdir -p \"", 10LL)), (out_dir))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t2882.data); _tr_str_release(_aet_t2882); });
    }
#line 475 "src/doc.tr"
    long long j2 = 0LL;
#line 476 "src/doc.tr"
    while ((j2 < mods->len)) {
#line 477 "src/doc.tr"
        DocModule* m = ((DocModule*)List_ptr_get(mods, j2));
#line 478 "src/doc.tr"
        if (html) {
#line 478 "src/doc.tr"
            ({ TrStr _at_t2883 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((out_dir), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".html", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2884 = (doc_html(m)); write_file(_at_t2883, _at_t2884); _tr_str_release(_at_t2883); _tr_str_release(_at_t2884); });
        } else {
#line 479 "src/doc.tr"
            ({ TrStr _at_t2885 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((out_dir), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".md", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2886 = (doc_markdown(m)); write_file(_at_t2885, _at_t2886); _tr_str_release(_at_t2885); _tr_str_release(_at_t2886); });
        }
#line 480 "src/doc.tr"
        j2 = (j2 + 1LL);
    }
#line 481 "src/doc.tr"
    if (html) {
#line 481 "src/doc.tr"
        ({ TrStr _at_t2887 = (_tr_strx_concatv((out_dir), (_tr_str_lit_len("/index.html", 11LL)))); TrStr _at_t2888 = (doc_index_html(mods)); write_file(_at_t2887, _at_t2888); _tr_str_release(_at_t2887); _tr_str_release(_at_t2888); });
    } else {
#line 482 "src/doc.tr"
        ({ TrStr _at_t2889 = (_tr_strx_concatv((out_dir), (_tr_str_lit_len("/README.md", 10LL)))); TrStr _at_t2890 = (doc_index_markdown(mods)); write_file(_at_t2889, _at_t2890); _tr_str_release(_at_t2889); _tr_str_release(_at_t2890); });
    }
#line 483 "src/doc.tr"
    ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(mods->len)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("doc: ", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" module(s) written to ", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (out_dir)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 484 "src/doc.tr"
    return 0LL;
}

