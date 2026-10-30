#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) ModuleResolver* ModuleResolver_init() {
#line 80 "src/resolver.tr"
    ModuleResolver* r = ((ModuleResolver*)_tr_obj_alloc(sizeof(ModuleResolver)));
#line 81 "src/resolver.tr"
    r->visited = _tr_dict_new(128LL);
#line 82 "src/resolver.tr"
    r->all_decls = (void*)List_ptr_new();
#line 83 "src/resolver.tr"
    r->search_paths = (void*)List_TrStr_new();
#line 84 "src/resolver.tr"
    r->mod_dot_paths = (void*)List_TrStr_new();
#line 85 "src/resolver.tr"
    r->mod_file_paths = (void*)List_TrStr_new();
#line 86 "src/resolver.tr"
    r->all_decl_modules = (void*)List_TrStr_new();
#line 87 "src/resolver.tr"
    r->current_mod = _tr_str_lit_len("", 0LL);
#line 88 "src/resolver.tr"
    r->parse_errors = 0LL;
#line 89 "src/resolver.tr"
    r->fn_owner = _tr_dict_new(256LL);
#line 90 "src/resolver.tr"
    r->import_errors = (void*)List_TrStr_new();
#line 91 "src/resolver.tr"
    r->pending_syms = (void*)List_TrStr_new();
#line 92 "src/resolver.tr"
    r->last_found = true;
#line 93 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len(".", 1LL));
#line 94 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len("tauraro", 7LL));
#line 95 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len("..", 2LL));
#line 96 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len("../..", 5LL));
#line 97 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len("std", 3LL));
#line 98 "src/resolver.tr"
    return r;
}

__attribute__((hot)) void ModuleResolver_add_search_path(ModuleResolver* self, TrStr p) {
#line 101 "src/resolver.tr"
    if (_tr_str_eqv((p), (_tr_str_lit_len("", 0LL)))) {
#line 101 "src/resolver.tr"
        return;
    }
#line 102 "src/resolver.tr"
    long long i = 0LL;
#line 103 "src/resolver.tr"
    while ((i < self->search_paths->len)) {
#line 104 "src/resolver.tr"
        if (({ TrStr _cmpa_t561 = (List_TrStr_get(self->search_paths, i)); TrStr _cmpb_t562 = (p); _Bool _cmpr_t563 = _tr_str_eqv(_cmpa_t561, _cmpb_t562); _tr_str_release(_cmpa_t561); _cmpr_t563; })) {
#line 104 "src/resolver.tr"
            return;
        }
#line 105 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 106 "src/resolver.tr"
    List_TrStr_append(self->search_paths, p);
}

__attribute__((hot)) TrStr ModuleResolver_dir_of_path(ModuleResolver* self, TrStr path) {
#line 110 "src/resolver.tr"
    char* p = ((char*)(_tr_strz(path)));
#line 111 "src/resolver.tr"
    long long n = 0LL;
#line 112 "src/resolver.tr"
    while ((((long long)((*(p + n)))) != 0LL)) {
#line 112 "src/resolver.tr"
        n = (n + 1LL);
    }
#line 113 "src/resolver.tr"
    long long last = (0LL - 1LL);
#line 114 "src/resolver.tr"
    long long i = 0LL;
#line 115 "src/resolver.tr"
    while ((i < n)) {
#line 116 "src/resolver.tr"
        long long c = ((long long)((*(p + i))));
#line 117 "src/resolver.tr"
        if (((c == 47LL) || (c == 92LL))) {
#line 117 "src/resolver.tr"
            last = i;
        }
#line 118 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 119 "src/resolver.tr"
    if ((last < 0LL)) {
#line 119 "src/resolver.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 120 "src/resolver.tr"
    return _tr_str_slicev((path), 0LL, last);
}

__attribute__((hot)) TrStr ModuleResolver_base_of_path(ModuleResolver* self, TrStr path) {
#line 124 "src/resolver.tr"
    char* p = ((char*)(_tr_strz(path)));
#line 125 "src/resolver.tr"
    long long n = 0LL;
#line 126 "src/resolver.tr"
    while ((((long long)((*(p + n)))) != 0LL)) {
#line 126 "src/resolver.tr"
        n = (n + 1LL);
    }
#line 127 "src/resolver.tr"
    long long last = (0LL - 1LL);
#line 128 "src/resolver.tr"
    long long i = 0LL;
#line 129 "src/resolver.tr"
    while ((i < n)) {
#line 130 "src/resolver.tr"
        long long c = ((long long)((*(p + i))));
#line 131 "src/resolver.tr"
        if (((c == 47LL) || (c == 92LL))) {
#line 131 "src/resolver.tr"
            last = i;
        }
#line 132 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 133 "src/resolver.tr"
    return _tr_str_slicev((path), (last + 1LL), n);
}

__attribute__((hot)) Program* ModuleResolver_resolve_main(ModuleResolver* self, TrStr main_path) {
#line 143 "src/resolver.tr"
    long long dir_end = 0LL;
#line 144 "src/resolver.tr"
    long long ci = 0LL;
#line 145 "src/resolver.tr"
    char* mp = ((char*)(_tr_strz(main_path)));
#line 146 "src/resolver.tr"
    while ((((long long)((*(mp + ci)))) != 0LL)) {
#line 147 "src/resolver.tr"
        long long ch = ((long long)((*(mp + ci))));
#line 148 "src/resolver.tr"
        if (((ch == 47LL) || (ch == 92LL))) {
#line 149 "src/resolver.tr"
            dir_end = ci;
        }
#line 150 "src/resolver.tr"
        ci = (ci + 1LL);
    }
#line 151 "src/resolver.tr"
    if ((dir_end > 0LL)) {
#line 153 "src/resolver.tr"
        TrStr dir_str = _tr_str_lit_len("", 0LL);
#line 154 "src/resolver.tr"
        /* unsafe block */
#line 155 "src/resolver.tr"
        TrStr _strtmp_t564 = _tr_str_slicev((main_path), 0LL, dir_end);
        _tr_str_release(dir_str);
        dir_str = _strtmp_t564;
#line 156 "src/resolver.tr"
        List_TrStr_append(self->search_paths, dir_str);
#line 160 "src/resolver.tr"
        long long par_end = 0LL;
#line 161 "src/resolver.tr"
        long long pj = 0LL;
#line 162 "src/resolver.tr"
        char* dp = ((char*)(_tr_strz(dir_str)));
#line 163 "src/resolver.tr"
        while ((((long long)((*(dp + pj)))) != 0LL)) {
#line 164 "src/resolver.tr"
            long long dch = ((long long)((*(dp + pj))));
#line 165 "src/resolver.tr"
            if (((dch == 47LL) || (dch == 92LL))) {
#line 165 "src/resolver.tr"
                par_end = pj;
            }
#line 166 "src/resolver.tr"
            pj = (pj + 1LL);
        }
#line 172 "src/resolver.tr"
        TrStr parent_dir = _tr_str_lit_len(".", 1LL);
#line 173 "src/resolver.tr"
        if ((par_end > 0LL)) {
#line 174 "src/resolver.tr"
            /* unsafe block */
#line 175 "src/resolver.tr"
            TrStr _strtmp_t565 = _tr_str_slicev((dir_str), 0LL, par_end);
            _tr_str_release(parent_dir);
            parent_dir = _strtmp_t565;
#line 176 "src/resolver.tr"
            List_TrStr_append(self->search_paths, parent_dir);
#line 177 "src/resolver.tr"
            long long gp_end = 0LL;
#line 178 "src/resolver.tr"
            long long gj = 0LL;
#line 179 "src/resolver.tr"
            char* gp_p = ((char*)(_tr_strz(parent_dir)));
#line 180 "src/resolver.tr"
            while ((((long long)((*(gp_p + gj)))) != 0LL)) {
#line 181 "src/resolver.tr"
                long long gch = ((long long)((*(gp_p + gj))));
#line 182 "src/resolver.tr"
                if (((gch == 47LL) || (gch == 92LL))) {
#line 182 "src/resolver.tr"
                    gp_end = gj;
                }
#line 183 "src/resolver.tr"
                gj = (gj + 1LL);
            }
#line 184 "src/resolver.tr"
            if ((gp_end > 0LL)) {
#line 185 "src/resolver.tr"
                TrStr gp_dir = _tr_str_lit_len("", 0LL);
#line 186 "src/resolver.tr"
                /* unsafe block */
#line 187 "src/resolver.tr"
                TrStr _strtmp_t566 = _tr_str_slicev((parent_dir), 0LL, gp_end);
                _tr_str_release(gp_dir);
                gp_dir = _strtmp_t566;
#line 188 "src/resolver.tr"
                List_TrStr_append(self->search_paths, gp_dir);
                _tr_str_release(gp_dir);
            }
        }
#line 196 "src/resolver.tr"
        void* vh = _tr_opendir(_tr_strz(parent_dir));
#line 197 "src/resolver.tr"
        if ((((unsigned long long)(vh)) != ((unsigned long long)(0LL)))) {
#line 198 "src/resolver.tr"
            TrStr vname = _tr_str_wrap(_tr_readdir(vh));
#line 199 "src/resolver.tr"
            while ((!_tr_str_eqv((vname), (_tr_str_lit_len("", 0LL))))) {
#line 200 "src/resolver.tr"
                if (((!_tr_str_eqv((vname), (_tr_str_lit_len(".", 1LL)))) && (!_tr_str_eqv((vname), (_tr_str_lit_len("..", 2LL)))))) {
#line 201 "src/resolver.tr"
                    TrStr vpkg_src = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((parent_dir), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/src", 4LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 202 "src/resolver.tr"
                    if (_tr_dir_exists(_tr_strz(vpkg_src))) {
#line 203 "src/resolver.tr"
                        List_TrStr_append(self->search_paths, vpkg_src);
                    }
                }
#line 204 "src/resolver.tr"
                TrStr _strtmp_t567 = _tr_str_wrap(_tr_readdir(vh));
                _tr_str_release(vname);
                vname = _strtmp_t567;
            }
#line 205 "src/resolver.tr"
            _tr_closedir(vh);
            _tr_str_release(vname);
        }
    }
#line 206 "src/resolver.tr"
    ModuleResolver_resolve_file(self, main_path, true);
#line 207 "src/resolver.tr"
    Program* p = Program_init();
#line 208 "src/resolver.tr"
    long long i = 0LL;
#line 209 "src/resolver.tr"
    while ((i < self->all_decls->len)) {
#line 210 "src/resolver.tr"
        Program_push(p, ((Decl*)List_ptr_get(self->all_decls, i)));
#line 211 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 212 "src/resolver.tr"
    return p;
}

__attribute__((hot)) long long ModuleResolver__import_line(ModuleResolver* self, TrStr source, TrStr mod_path) {
#line 216 "src/resolver.tr"
    long long n = _tr_str_lenv((source));
#line 217 "src/resolver.tr"
    long long start = 0LL;
#line 218 "src/resolver.tr"
    long long line_no = 1LL;
#line 219 "src/resolver.tr"
    long long i = 0LL;
#line 220 "src/resolver.tr"
    while ((i <= n)) {
#line 221 "src/resolver.tr"
        if (((i == n) || ({ TrStr _cmpa_t568 = (_tr_str_slicev((source), i, (i + 1LL))); TrStr _cmpb_t569 = (_tr_str_lit_len("\n", 1LL)); _Bool _cmpr_t570 = _tr_str_eqv(_cmpa_t568, _cmpb_t569); _tr_str_release(_cmpa_t568); _cmpr_t570; }))) {
#line 222 "src/resolver.tr"
            TrStr l = _tr_str_stripv((_tr_str_slicev((source), start, i)));
#line 223 "src/resolver.tr"
            if (((_tr_str_starts_withv((l), (_tr_str_lit_len("import ", 7LL))) || _tr_str_starts_withv((l), (_tr_str_lit_len("from ", 5LL)))) && (_tr_str_index_ofv((l), (mod_path)) >= 0LL))) {
#line 223 "src/resolver.tr"
                long long _trret159 = line_no;
                _tr_str_release(l);
                return _trret159;
            }
#line 224 "src/resolver.tr"
            start = (i + 1LL);
#line 225 "src/resolver.tr"
            line_no = (line_no + 1LL);
            _tr_str_release(l);
        }
#line 226 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 227 "src/resolver.tr"
    return 0LL;
}

__attribute__((hot)) bool ModuleResolver__program_declares(ModuleResolver* self, TrStr name) {
#line 231 "src/resolver.tr"
    long long k = 0LL;
#line 232 "src/resolver.tr"
    while ((k < self->all_decls->len)) {
#line 233 "src/resolver.tr"
        __auto_type _t571 = (*((Decl*)List_ptr_get(self->all_decls, k)));
        if (_t571.tag == Decl_DFunction) {
            __auto_type f = _t571.data.DFunction.func;
#line 235 "src/resolver.tr"
            if (_tr_str_eqv((f->name), (name))) {
#line 235 "src/resolver.tr"
                return true;
            }
        } else if (_t571.tag == Decl_DClass) {
            __auto_type c = _t571.data.DClass.cls;
#line 237 "src/resolver.tr"
            if (_tr_str_eqv((c->name), (name))) {
#line 237 "src/resolver.tr"
                return true;
            }
        } else if (_t571.tag == Decl_DActor) {
            __auto_type c = _t571.data.DActor.cls;
#line 239 "src/resolver.tr"
            if (_tr_str_eqv((c->name), (name))) {
#line 239 "src/resolver.tr"
                return true;
            }
        } else if (_t571.tag == Decl_DEnum) {
            __auto_type e = _t571.data.DEnum.enm;
#line 241 "src/resolver.tr"
            if (_tr_str_eqv((e->name), (name))) {
#line 241 "src/resolver.tr"
                return true;
            }
        } else if (_t571.tag == Decl_DInterface) {
            __auto_type i = _t571.data.DInterface.iface;
#line 243 "src/resolver.tr"
            if (_tr_str_eqv((i->name), (name))) {
#line 243 "src/resolver.tr"
                return true;
            }
        } else if (_t571.tag == Decl_DTypeAlias) {
            __auto_type an = _t571.data.DTypeAlias.name;
#line 245 "src/resolver.tr"
            if (_tr_str_eqv((an), (name))) {
#line 245 "src/resolver.tr"
                return true;
            }
        } else if (_t571.tag == Decl_DDecoratorDef) {
            __auto_type f = _t571.data.DDecoratorDef.func;
#line 247 "src/resolver.tr"
            if (_tr_str_eqv((f->name), (name))) {
#line 247 "src/resolver.tr"
                return true;
            }
        } else if (_t571.tag == Decl_DExtern) {
            __auto_type fns = _t571.data.DExtern.functions;
#line 249 "src/resolver.tr"
            long long ei = 0LL;
#line 250 "src/resolver.tr"
            while ((ei < fns->len)) {
#line 251 "src/resolver.tr"
                if (_tr_str_eqv((((FunctionDef*)List_ptr_get(fns, ei))->name), (name))) {
#line 251 "src/resolver.tr"
                    return true;
                }
#line 252 "src/resolver.tr"
                ei = (ei + 1LL);
            }
        } else if (_t571.tag == Decl_DTopLevelStmt) {
            __auto_type gs = _t571.data.DTopLevelStmt.stmt;
#line 254 "src/resolver.tr"
            __auto_type _t572 = (*gs);
            if (_t572.tag == Stmt_SLet) {
                __auto_type gn = _t572.data.SLet.name;
#line 256 "src/resolver.tr"
                if (_tr_str_eqv((gn), (name))) {
#line 256 "src/resolver.tr"
                    return true;
                }
            } else if (1) {
                __auto_type _ = _t572;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t571;
            /* pass */
        }
#line 259 "src/resolver.tr"
        k = (k + 1LL);
    }
#line 260 "src/resolver.tr"
    return false;
}

__attribute__((hot)) bool ModuleResolver__import_builtin_word(ModuleResolver* self, TrStr n) {
#line 264 "src/resolver.tr"
    if (((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Array", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 264 "src/resolver.tr"
        return true;
    }
#line 265 "src/resolver.tr"
    if (((((((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Bytes", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("String", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Any", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Future", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Task", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncTask", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL))))) {
#line 265 "src/resolver.tr"
        return true;
    }
#line 266 "src/resolver.tr"
    return false;
}

__attribute__((hot)) void ModuleResolver_check_imported_symbols(ModuleResolver* self) {
#line 270 "src/resolver.tr"
    long long k = 0LL;
#line 271 "src/resolver.tr"
    while ((k < self->pending_syms->len)) {
#line 272 "src/resolver.tr"
        List_TrStr* parts = _tr_str_splitv((List_TrStr_get(self->pending_syms, k)), (_tr_str_lit_len("|", 1LL)));
#line 273 "src/resolver.tr"
        if ((parts->len == 4LL)) {
#line 274 "src/resolver.tr"
            TrStr nm = List_TrStr_get(parts, 3LL);
#line 275 "src/resolver.tr"
            if (((((!_tr_str_eqv((nm), (_tr_str_lit_len("*", 1LL)))) && (!_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL))))) && (!ModuleResolver__import_builtin_word(self, nm))) && (!ModuleResolver__program_declares(self, nm)))) {
#line 276 "src/resolver.tr"
                TrStr sub = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_str_replacev((List_TrStr_get(parts, 2LL)), (_tr_str_lit_len(".", 1LL)), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; });
#line 277 "src/resolver.tr"
                bool found_sub = false;
#line 278 "src/resolver.tr"
                long long si = 0LL;
#line 279 "src/resolver.tr"
                while ((si < self->search_paths->len)) {
#line 280 "src/resolver.tr"
                    if ((({ TrStr _at_t573 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (List_TrStr_get(self->search_paths, si)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sub)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".tr", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (file_exists(_at_t573)); _tr_str_release(_at_t573); _wr; }) || ({ TrStr _at_t574 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (List_TrStr_get(self->search_paths, si)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sub)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/mod.tr", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (file_exists(_at_t574)); _tr_str_release(_at_t574); _wr; }))) {
#line 280 "src/resolver.tr"
                        found_sub = true;
                    }
#line 281 "src/resolver.tr"
                    si = (si + 1LL);
                }
#line 282 "src/resolver.tr"
                if ((!found_sub)) {
#line 283 "src/resolver.tr"
                    ({ __auto_type _ord_t575 = (({ TrStr _tr_cl = (List_TrStr_get(parts, 0LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t576 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t575); TrStr _tr_cr = (List_TrStr_get(parts, 1LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [N-7] Module '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t577 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t576); TrStr _tr_cr = (List_TrStr_get(parts, 2LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has no symbol '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: check the spelling of '", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or the module it is imported from.", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->import_errors, _ord_t577); _tr_str_release(_ord_t577); });
                }
            }
        }
#line 284 "src/resolver.tr"
        k = (k + 1LL);
        List_TrStr_free(parts);
    }
}

__attribute__((hot)) void ModuleResolver_resolve_file(ModuleResolver* self, TrStr path, bool is_root) {
#line 290 "src/resolver.tr"
    if (_tr_dict_contains(self->visited, _tr_strz(path))) {
#line 290 "src/resolver.tr"
        return;
    }
#line 291 "src/resolver.tr"
    _tr_dict_set(self->visited, _tr_strz(path), true);
#line 293 "src/resolver.tr"
    TrStr source = read_file(path);
#line 294 "src/resolver.tr"
    if (_tr_str_eqv((source), (_tr_str_lit_len("", 0LL)))) {
#line 296 "src/resolver.tr"
        char* p_ptr = ((char*)(_tr_strz(path)));
#line 297 "src/resolver.tr"
        long long slen = 0LL;
#line 298 "src/resolver.tr"
        while ((((long long)((*(p_ptr + slen)))) != 0LL)) {
#line 299 "src/resolver.tr"
            slen = (slen + 1LL);
        }
#line 300 "src/resolver.tr"
        bool has_tr = false;
#line 301 "src/resolver.tr"
        if ((slen >= 3LL)) {
#line 302 "src/resolver.tr"
            if ((((long long)((*(p_ptr + (slen - 3LL))))) == 46LL)) {
#line 303 "src/resolver.tr"
                if ((((long long)((*(p_ptr + (slen - 2LL))))) == 116LL)) {
#line 304 "src/resolver.tr"
                    if ((((long long)((*(p_ptr + (slen - 1LL))))) == 114LL)) {
#line 305 "src/resolver.tr"
                        has_tr = true;
                    }
                }
            }
        }
#line 306 "src/resolver.tr"
        if ((!has_tr)) {
#line 307 "src/resolver.tr"
            TrStr _strtmp_t579 = ({ TrStr _at_t578 = (_tr_strx_concatv((path), (_tr_str_lit_len(".tr", 3LL)))); __auto_type _wr = (read_file(_at_t578)); _tr_str_release(_at_t578); _wr; });
            _tr_str_release(source);
            source = _strtmp_t579;
        }
#line 308 "src/resolver.tr"
        if (_tr_str_eqv((source), (_tr_str_lit_len("", 0LL)))) {
#line 308 "src/resolver.tr"
            _tr_str_release(source);
            return;
        }
    }
#line 311 "src/resolver.tr"
    char* nul_p = ((char*)(_tr_strz(source)));
#line 312 "src/resolver.tr"
    long long nul_n = _tr_str_lenv((source));
#line 313 "src/resolver.tr"
    long long nul_at = (-1LL);
#line 314 "src/resolver.tr"
    long long nul_i = 0LL;
#line 315 "src/resolver.tr"
    while ((nul_i < nul_n)) {
#line 316 "src/resolver.tr"
        if ((((long long)((*(nul_p + nul_i)))) == 0LL)) {
#line 317 "src/resolver.tr"
            nul_at = nul_i;
#line 318 "src/resolver.tr"
            break;
        }
#line 319 "src/resolver.tr"
        nul_i = (nul_i + 1LL);
    }
#line 320 "src/resolver.tr"
    if (((((nul_at < 0LL) && (_tr_file_size(_tr_strz(path)) > (nul_n + 0LL))) && (_tr_file_size(_tr_strz(path)) > 0LL)) && (nul_n < _tr_file_size(_tr_strz(path))))) {
#line 321 "src/resolver.tr"
        nul_at = nul_n;
    }
#line 322 "src/resolver.tr"
    if ((nul_at >= 0LL)) {
#line 323 "src/resolver.tr"
        self->parse_errors = (self->parse_errors + 1LL);
#line 324 "src/resolver.tr"
        ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("error: [LEX-1] ", 15LL)), (path))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" contains a NUL byte (offset ", 29LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(nul_at)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); source files must be text.", 29LL))); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 325 "src/resolver.tr"
        ({ printf("%s", _tr_strz(_tr_str_lit_len("  = help: remove the stray NUL (0x00) byte - it would silently truncate the rest of the file.", 93LL))); printf("\n"); });
#line 326 "src/resolver.tr"
        _tr_str_release(source);
        return;
    }
#line 333 "src/resolver.tr"
    bool file_trusted = (ModuleResolver__path_is_lib(self, path) || ModuleResolver__source_is_trusted(self, source));
#line 335 "src/resolver.tr"
    ({ TrStr _aet_t580 = (_tr_strx_concatv((_tr_str_lit_len("parsing ", 8LL)), (path))); _tr_set_ice_ctx(_aet_t580.data); _tr_str_release(_aet_t580); });
#line 336 "src/resolver.tr"
    Lexer* lexer = Lexer_init(source);
#line 337 "src/resolver.tr"
    List_Token* tokens = Lexer_tokenize(lexer);
#line 338 "src/resolver.tr"
    ModuleResolver__dedupe_private_fns(self, tokens, path, is_root, ModuleResolver__source_is_trusted(self, source));
#line 339 "src/resolver.tr"
    Parser* parser = Parser_init(tokens, lexer->token_lines);
#line 340 "src/resolver.tr"
    parser->current_file = _tr_str_retain(path);
#line 341 "src/resolver.tr"
    parser->cols = lexer->token_cols;
#line 342 "src/resolver.tr"
    parser->src_text = _tr_str_retain(source);
#line 343 "src/resolver.tr"
    Program* prog = Parser_parse_program(parser);
#line 344 "src/resolver.tr"
    self->parse_errors = (self->parse_errors + parser->error_count);
#line 347 "src/resolver.tr"
    long long i = 0LL;
#line 348 "src/resolver.tr"
    while ((i < Program_len(prog))) {
#line 349 "src/resolver.tr"
        Decl* decl_ptr = Program_get(prog, i);
#line 350 "src/resolver.tr"
        __auto_type _t581 = (*decl_ptr);
        if (_t581.tag == Decl_DImport) {
            __auto_type mod_path = _t581.data.DImport.path;
__auto_type alias = _t581.data.DImport.alias;
#line 352 "src/resolver.tr"
            ModuleResolver_resolve_module_path(self, mod_path);
#line 353 "src/resolver.tr"
            if ((!self->last_found)) {
#line 354 "src/resolver.tr"
                ({ TrStr _ord_t582 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((path), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ModuleResolver__import_line(self, source, mod_path))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [N-7] Module '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mod_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was not found.\n      FIX: check the module path (dots for directories), or add its directory to TAURARO_PATH.", 111LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->import_errors, _ord_t582); _tr_str_release(_ord_t582); });
            }
        } else if (_t581.tag == Decl_DFromImport) {
            __auto_type mod_path = _t581.data.DFromImport.path;
__auto_type items = _t581.data.DFromImport.items;
#line 356 "src/resolver.tr"
            ModuleResolver_resolve_module_path(self, mod_path);
#line 357 "src/resolver.tr"
            if ((!self->last_found)) {
#line 358 "src/resolver.tr"
                ({ TrStr _ord_t583 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((path), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ModuleResolver__import_line(self, source, mod_path))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [N-7] Module '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mod_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was not found.\n      FIX: check the module path (dots for directories), or add its directory to TAURARO_PATH.", 111LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->import_errors, _ord_t583); _tr_str_release(_ord_t583); });
            } else {
#line 360 "src/resolver.tr"
                long long ii = 0LL;
#line 361 "src/resolver.tr"
                while ((ii < items->len)) {
#line 362 "src/resolver.tr"
                    ({ __auto_type _ord_t584 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((path), (_tr_str_lit_len("|", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ModuleResolver__import_line(self, source, mod_path))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mod_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t585 = (({ TrStr _tr_cl = (_ord_t584); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((ImportItem*)List_ptr_get(items, ii))->name)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->pending_syms, _ord_t585); _tr_str_release(_ord_t585); });
#line 363 "src/resolver.tr"
                    ii = (ii + 1LL);
                }
            }
        } else if (1) {
            __auto_type _ = _t581;
#line 373 "src/resolver.tr"
            if (file_trusted) {
#line 374 "src/resolver.tr"
                ModuleResolver__mark_decl_lib(self, decl_ptr);
            }
#line 375 "src/resolver.tr"
            List_ptr_append(self->all_decls, decl_ptr);
#line 376 "src/resolver.tr"
            List_TrStr_append(self->all_decl_modules, self->current_mod);
        }
#line 377 "src/resolver.tr"
        i = (i + 1LL);
    }
    _tr_str_release(source);
    _tr_obj_release(lexer, _trdrop_Lexer);
    _tr_obj_release(parser, _trdrop_Parser);
    _tr_obj_release(prog, _trdrop_Program);
}

__attribute__((hot)) void ModuleResolver__dedupe_private_fns(ModuleResolver* self, List_Token* tokens, TrStr path, bool is_root, bool trusted) {
#line 386 "src/resolver.tr"
    bool exempt = (((is_root || trusted) || ModuleResolver__contains(self, path, _tr_str_lit_len("core/", 5LL))) || ModuleResolver__contains(self, path, _tr_str_lit_len("core\\", 5LL)));
#line 387 "src/resolver.tr"
    TrMap* renames = _tr_dict_new(8LL);
#line 388 "src/resolver.tr"
    long long depth = 0LL;
#line 389 "src/resolver.tr"
    long long i = 0LL;
#line 390 "src/resolver.tr"
    while ((i < tokens->len)) {
#line 391 "src/resolver.tr"
        long long nxt = 0LL;
#line 392 "src/resolver.tr"
        __auto_type _t586 = List_Token_get(tokens, i);
        if (_t586.tag == Token_Indent) {
            depth = (depth + 1LL);
        } else if (_t586.tag == Token_Dedent) {
            depth = (depth - 1LL);
        } else if (_t586.tag == Token_KwDef) {
#line 396 "src/resolver.tr"
            if (((depth == 0LL) && ((i + 1LL) < tokens->len))) {
#line 397 "src/resolver.tr"
                bool is_pub = false;
#line 398 "src/resolver.tr"
                if ((i > 0LL)) {
#line 399 "src/resolver.tr"
                    __auto_type _t587 = List_Token_get(tokens, (i - 1LL));
                    if (_t587.tag == Token_KwPub) {
                        is_pub = true;
                    } else if (1) {
                        __auto_type _ = _t587;
                        /* pass */
                    }
                }
#line 402 "src/resolver.tr"
                if ((!is_pub)) {
#line 403 "src/resolver.tr"
                    __auto_type _t588 = List_Token_get(tokens, (i + 1LL));
                    if (_t588.tag == Token_Ident) {
                        __auto_type nm = _t588.data.Ident.name;
#line 405 "src/resolver.tr"
                        if (((!_tr_str_eqv((nm), (_tr_str_lit_len("main", 4LL)))) && (!_tr_str_starts_withv((nm), (_tr_str_lit_len("_tr", 3LL)))))) {
#line 406 "src/resolver.tr"
                            if (_tr_dict_contains(self->fn_owner, _tr_strz(nm))) {
#line 407 "src/resolver.tr"
                                if ((({ TrStr _cmpa_t589 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->fn_owner, _tr_strz(nm))))); TrStr _cmpb_t590 = (path); _Bool _cmpr_t591 = !_tr_str_eqv(_cmpa_t589, _cmpb_t590); _tr_str_release(_cmpa_t589); _cmpr_t591; }) && (!exempt))) {
#line 408 "src/resolver.tr"
                                    ({ TrStr _dvt_t592 = (({ TrStr _tr_cl = (_tr_strx_concatv((nm), (_tr_str_lit_len("__", 2LL)))); TrStr _tr_cr = (ModuleResolver__path_tag(self, path)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _tr_dict_set(renames, _tr_strz(nm), _tr_str_box(_tr_str_retain(_dvt_t592))); _tr_str_release(_dvt_t592); });
                                }
                            } else {
#line 410 "src/resolver.tr"
                                _tr_dict_set(self->fn_owner, _tr_strz(nm), _tr_str_box(_tr_str_retain(path)));
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t588;
                        /* pass */
                    }
                }
            }
        } else if (1) {
            __auto_type _ = _t586;
            /* pass */
        }
#line 413 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 414 "src/resolver.tr"
    if ((((long long)renames->len) == 0LL)) {
#line 414 "src/resolver.tr"
        Dict_free_strval(renames);
        return;
    }
#line 415 "src/resolver.tr"
    depth = 0LL;
#line 416 "src/resolver.tr"
    i = 0LL;
#line 417 "src/resolver.tr"
    while ((i < tokens->len)) {
#line 418 "src/resolver.tr"
        Token tk = List_Token_get(tokens, i);
#line 419 "src/resolver.tr"
        __auto_type _t593 = tk;
        if (_t593.tag == Token_Indent) {
            depth = (depth + 1LL);
        } else if (_t593.tag == Token_Dedent) {
            depth = (depth - 1LL);
        } else if (_t593.tag == Token_Ident) {
            __auto_type nm = _t593.data.Ident.name;
#line 423 "src/resolver.tr"
            if (_tr_dict_contains(renames, _tr_strz(nm))) {
#line 424 "src/resolver.tr"
                bool prev_dot = false;
#line 425 "src/resolver.tr"
                bool prev_def = false;
#line 426 "src/resolver.tr"
                bool prev_open = false;
#line 427 "src/resolver.tr"
                if ((i > 0LL)) {
#line 428 "src/resolver.tr"
                    __auto_type _t594 = List_Token_get(tokens, (i - 1LL));
                    if (_t594.tag == Token_Dot) {
                        prev_dot = true;
                    } else if (_t594.tag == Token_KwDef) {
                        prev_def = true;
                    } else if (_t594.tag == Token_LParen) {
                        prev_open = true;
                    } else if (_t594.tag == Token_Comma) {
                        prev_open = true;
                    } else if (1) {
                        __auto_type _ = _t594;
                        /* pass */
                    }
                }
#line 434 "src/resolver.tr"
                bool next_call = false;
#line 435 "src/resolver.tr"
                bool next_arg_end = false;
#line 436 "src/resolver.tr"
                if (((i + 1LL) < tokens->len)) {
#line 437 "src/resolver.tr"
                    __auto_type _t595 = List_Token_get(tokens, (i + 1LL));
                    if (_t595.tag == Token_LParen) {
                        next_call = true;
                    } else if (_t595.tag == Token_Comma) {
                        next_arg_end = true;
                    } else if (_t595.tag == Token_RParen) {
                        next_arg_end = true;
                    } else if (1) {
                        __auto_type _ = _t595;
                        /* pass */
                    }
                }
#line 442 "src/resolver.tr"
                bool hit = false;
#line 443 "src/resolver.tr"
                if ((!prev_dot)) {
#line 444 "src/resolver.tr"
                    if (prev_def) {
#line 445 "src/resolver.tr"
                        hit = ((depth == 0LL) && next_call);
                    } else if (next_call) {
#line 447 "src/resolver.tr"
                        hit = true;
                    } else if ((prev_open && next_arg_end)) {
#line 449 "src/resolver.tr"
                        hit = true;
                    }
                }
#line 450 "src/resolver.tr"
                if (hit) {
#line 450 "src/resolver.tr"
                    tokens->data[(_tr_bounds_check(i, tokens->len), i)] = ({ TrStr _at_t596 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(renames, _tr_strz(nm))))); __auto_type _wr = (Token_ctor_Ident(_at_t596)); _tr_str_release(_at_t596); _wr; });
                }
            }
        } else if (1) {
            __auto_type _ = _t593;
            /* pass */
        }
#line 452 "src/resolver.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) TrStr ModuleResolver__path_tag(ModuleResolver* self, TrStr path) {
#line 456 "src/resolver.tr"
    StringBuilder* sb = StringBuilder_init((_tr_str_lenv((path)) + 1LL));
#line 457 "src/resolver.tr"
    long long i = 0LL;
#line 458 "src/resolver.tr"
    while ((i < _tr_str_lenv((path)))) {
#line 459 "src/resolver.tr"
        long long c = _tr_str_char_at_codev((path), i);
#line 460 "src/resolver.tr"
        if (((((c >= 48LL) && (c <= 57LL)) || ((c >= 65LL) && (c <= 90LL))) || ((c >= 97LL) && (c <= 122LL)))) {
#line 461 "src/resolver.tr"
            StringBuilder_append_char(sb, c);
        } else {
#line 463 "src/resolver.tr"
            StringBuilder_append_char(sb, 95LL);
        }
#line 464 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 465 "src/resolver.tr"
    TrStr out = StringBuilder_to_owned(sb);
#line 466 "src/resolver.tr"
    StringBuilder__tr_fn_free(sb);
#line 467 "src/resolver.tr"
    TrStr _trret160 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret160;
}

__attribute__((hot)) bool ModuleResolver__path_is_lib(ModuleResolver* self, TrStr path) {
#line 472 "src/resolver.tr"
    return (((ModuleResolver__contains(self, path, _tr_str_lit_len("std/", 4LL)) || ModuleResolver__contains(self, path, _tr_str_lit_len("std\\", 4LL))) || ModuleResolver__contains(self, path, _tr_str_lit_len("core/", 5LL))) || ModuleResolver__contains(self, path, _tr_str_lit_len("core\\", 5LL)));
}

__attribute__((hot)) bool ModuleResolver__source_is_trusted(ModuleResolver* self, TrStr source) {
#line 476 "src/resolver.tr"
    long long n = _tr_str_lenv((source));
#line 477 "src/resolver.tr"
    long long lim = 400LL;
#line 478 "src/resolver.tr"
    if ((n < lim)) {
#line 478 "src/resolver.tr"
        lim = n;
    }
#line 479 "src/resolver.tr"
    if ((lim <= 0LL)) {
#line 479 "src/resolver.tr"
        return false;
    }
#line 480 "src/resolver.tr"
    return ({ TrStr _at_t597 = (_tr_str_slicev((source), 0LL, lim)); __auto_type _wr = (ModuleResolver__contains(self, _at_t597, _tr_str_lit_len("@trusted", 8LL))); _tr_str_release(_at_t597); _wr; });
}

__attribute__((hot)) bool ModuleResolver__contains(ModuleResolver* self, TrStr hay, TrStr needle) {
#line 483 "src/resolver.tr"
    long long hl = _tr_str_lenv((hay));
#line 484 "src/resolver.tr"
    long long nl = _tr_str_lenv((needle));
#line 485 "src/resolver.tr"
    if (((nl == 0LL) || (nl > hl))) {
#line 485 "src/resolver.tr"
        return false;
    }
#line 486 "src/resolver.tr"
    long long i = 0LL;
#line 487 "src/resolver.tr"
    while (((i + nl) <= hl)) {
#line 488 "src/resolver.tr"
        if (({ TrStr _cmpa_t598 = (_tr_str_slicev((hay), i, (i + nl))); TrStr _cmpb_t599 = (needle); _Bool _cmpr_t600 = _tr_str_eqv(_cmpa_t598, _cmpb_t599); _tr_str_release(_cmpa_t598); _cmpr_t600; })) {
#line 488 "src/resolver.tr"
            return true;
        }
#line 489 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 490 "src/resolver.tr"
    return false;
}

__attribute__((hot)) void ModuleResolver__mark_decl_lib(ModuleResolver* self, Decl* dp) {
#line 494 "src/resolver.tr"
    if ((((unsigned long long)(dp)) == ((unsigned long long)(0LL)))) {
#line 494 "src/resolver.tr"
        return;
    }
#line 495 "src/resolver.tr"
    __auto_type _t601 = (*dp);
    if (_t601.tag == Decl_DFunction) {
        __auto_type f = _t601.data.DFunction.func;
        f->is_lib = true;
    } else if (_t601.tag == Decl_DClass) {
        __auto_type c = _t601.data.DClass.cls;
        ModuleResolver__mark_methods_lib(self, c);
    } else if (_t601.tag == Decl_DActor) {
        __auto_type c = _t601.data.DActor.cls;
        ModuleResolver__mark_methods_lib(self, c);
    } else if (_t601.tag == Decl_DExtend) {
        __auto_type ms = _t601.data.DExtend.methods;
#line 500 "src/resolver.tr"
        long long i = 0LL;
#line 501 "src/resolver.tr"
        while ((i < ms->len)) {
#line 502 "src/resolver.tr"
            ((FunctionDef*)List_ptr_get(ms, i))->is_lib = true;
#line 503 "src/resolver.tr"
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t601;
        /* pass */
    }
}

__attribute__((hot)) void ModuleResolver__mark_methods_lib(ModuleResolver* self, ClassDef* c) {
#line 507 "src/resolver.tr"
    long long i = 0LL;
#line 508 "src/resolver.tr"
    while ((i < c->methods->len)) {
#line 509 "src/resolver.tr"
        ((FunctionDef*)List_ptr_get(c->methods, i))->is_lib = true;
#line 510 "src/resolver.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void ModuleResolver_resolve_recursive(ModuleResolver* self, TrStr path) {
#line 514 "src/resolver.tr"
    ModuleResolver_resolve_file(self, path, false);
}

__attribute__((hot)) void ModuleResolver_resolve_module_path(ModuleResolver* self, TrStr mod_path) {
#line 524 "src/resolver.tr"
    long long n = 0LL;
#line 525 "src/resolver.tr"
    char* p = ((char*)(_tr_strz(mod_path)));
#line 526 "src/resolver.tr"
    while ((((long long)((*(p + n)))) != 0LL)) {
#line 526 "src/resolver.tr"
        n = (n + 1LL);
    }
#line 527 "src/resolver.tr"
    TrStr dir_path = _tr_str_slicev((mod_path), 0LL, n);
#line 528 "src/resolver.tr"
    char* bp = ((char*)(_tr_strz(dir_path)));
#line 529 "src/resolver.tr"
    long long j = 0LL;
#line 530 "src/resolver.tr"
    while ((j < n)) {
#line 531 "src/resolver.tr"
        if ((((long long)((*(bp + j)))) == 46LL)) {
#line 532 "src/resolver.tr"
            /* unsafe block */
#line 532 "src/resolver.tr"
            (*(bp + j) = ((char)(47LL)));
        }
#line 533 "src/resolver.tr"
        j = (j + 1LL);
    }
#line 536 "src/resolver.tr"
    TrStr saved_mod = _tr_str_retain(self->current_mod);
#line 537 "src/resolver.tr"
    self->current_mod = _tr_str_retain(mod_path);
#line 540 "src/resolver.tr"
    long long sidx = 0LL;
#line 541 "src/resolver.tr"
    while ((sidx < self->search_paths->len)) {
#line 542 "src/resolver.tr"
        TrStr base = List_TrStr_get(self->search_paths, sidx);
#line 545 "src/resolver.tr"
        TrStr pkg_path = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/mod.tr", 7LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 546 "src/resolver.tr"
        if (file_exists(pkg_path)) {
#line 553 "src/resolver.tr"
            ({ TrStr _at_t602 = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); ModuleResolver_add_search_path(self, _at_t602); _tr_str_release(_at_t602); });
#line 554 "src/resolver.tr"
            ({ TrStr _at_t603 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/src", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); ModuleResolver_add_search_path(self, _at_t603); _tr_str_release(_at_t603); });
#line 555 "src/resolver.tr"
            bool already = _tr_dict_contains(self->visited, _tr_strz(pkg_path));
#line 556 "src/resolver.tr"
            ModuleResolver_resolve_file(self, pkg_path, false);
#line 557 "src/resolver.tr"
            if ((!already)) {
#line 558 "src/resolver.tr"
                List_TrStr_append(self->mod_dot_paths, mod_path);
#line 559 "src/resolver.tr"
                List_TrStr_append(self->mod_file_paths, pkg_path);
            }
#line 560 "src/resolver.tr"
            self->current_mod = _tr_str_retain(saved_mod);
#line 561 "src/resolver.tr"
            self->last_found = true;
#line 562 "src/resolver.tr"
            _tr_str_release(dir_path);
            _tr_str_release(saved_mod);
            _tr_str_release(base);
            _tr_str_release(pkg_path);
            return;
        }
#line 565 "src/resolver.tr"
        TrStr flat_path = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".tr", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 566 "src/resolver.tr"
        if (file_exists(flat_path)) {
#line 567 "src/resolver.tr"
            bool already = _tr_dict_contains(self->visited, _tr_strz(flat_path));
#line 568 "src/resolver.tr"
            ModuleResolver_resolve_file(self, flat_path, false);
#line 569 "src/resolver.tr"
            if ((!already)) {
#line 570 "src/resolver.tr"
                List_TrStr_append(self->mod_dot_paths, mod_path);
#line 571 "src/resolver.tr"
                List_TrStr_append(self->mod_file_paths, flat_path);
            }
#line 572 "src/resolver.tr"
            self->current_mod = _tr_str_retain(saved_mod);
#line 573 "src/resolver.tr"
            self->last_found = true;
#line 574 "src/resolver.tr"
            _tr_str_release(dir_path);
            _tr_str_release(saved_mod);
            _tr_str_release(base);
            _tr_str_release(pkg_path);
            _tr_str_release(flat_path);
            return;
        }
#line 582 "src/resolver.tr"
        TrStr pkg_src_path = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/src/", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (ModuleResolver_base_of_path(self, dir_path)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".tr", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 583 "src/resolver.tr"
        if (file_exists(pkg_src_path)) {
#line 589 "src/resolver.tr"
            ({ TrStr _at_t604 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/src", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); ModuleResolver_add_search_path(self, _at_t604); _tr_str_release(_at_t604); });
#line 590 "src/resolver.tr"
            bool already3 = _tr_dict_contains(self->visited, _tr_strz(pkg_src_path));
#line 591 "src/resolver.tr"
            ModuleResolver_resolve_file(self, pkg_src_path, false);
#line 592 "src/resolver.tr"
            if ((!already3)) {
#line 593 "src/resolver.tr"
                List_TrStr_append(self->mod_dot_paths, mod_path);
#line 594 "src/resolver.tr"
                List_TrStr_append(self->mod_file_paths, pkg_src_path);
            }
#line 595 "src/resolver.tr"
            self->current_mod = _tr_str_retain(saved_mod);
#line 596 "src/resolver.tr"
            self->last_found = true;
#line 597 "src/resolver.tr"
            _tr_str_release(dir_path);
            _tr_str_release(saved_mod);
            _tr_str_release(base);
            _tr_str_release(pkg_path);
            _tr_str_release(flat_path);
            _tr_str_release(pkg_src_path);
            return;
        }
#line 599 "src/resolver.tr"
        sidx = (sidx + 1LL);
        _tr_str_release(base);
        _tr_str_release(pkg_path);
        _tr_str_release(flat_path);
        _tr_str_release(pkg_src_path);
    }
#line 601 "src/resolver.tr"
    self->current_mod = _tr_str_retain(saved_mod);
#line 602 "src/resolver.tr"
    self->last_found = false;
    _tr_str_release(dir_path);
    _tr_str_release(saved_mod);
}

__attribute__((hot)) bool decl_is_pub(Decl d) {
#line 38 "src/resolver.tr"
    __auto_type _t605 = d;
    if (_t605.tag == Decl_DFunction) {
        __auto_type f = _t605.data.DFunction.func;
#line 40 "src/resolver.tr"
        bool _trret153 = f->is_public;
        return _trret153;
    } else if (_t605.tag == Decl_DClass) {
        __auto_type c = _t605.data.DClass.cls;
#line 42 "src/resolver.tr"
        bool _trret154 = c->is_public;
        return _trret154;
    } else if (_t605.tag == Decl_DEnum) {
        __auto_type e = _t605.data.DEnum.enm;
#line 44 "src/resolver.tr"
        bool _trret155 = e->is_public;
        return _trret155;
    } else if (_t605.tag == Decl_DInterface) {
        __auto_type i = _t605.data.DInterface.iface;
#line 46 "src/resolver.tr"
        bool _trret156 = i->is_public;
        return _trret156;
    } else if (_t605.tag == Decl_DActor) {
        __auto_type c = _t605.data.DActor.cls;
#line 48 "src/resolver.tr"
        bool _trret157 = c->is_public;
        return _trret157;
    } else if (_t605.tag == Decl_DExtern) {
        __auto_type abi = _t605.data.DExtern.abi;
__auto_type funcs = _t605.data.DExtern.functions;
#line 50 "src/resolver.tr"
        return true;
    } else if (_t605.tag == Decl_DExtend) {
        __auto_type target = _t605.data.DExtend.target;
__auto_type methods = _t605.data.DExtend.methods;
#line 52 "src/resolver.tr"
        return true;
    } else if (_t605.tag == Decl_DDecoratorDef) {
        __auto_type f = _t605.data.DDecoratorDef.func;
#line 54 "src/resolver.tr"
        bool _trret158 = f->is_public;
        return _trret158;
    } else if (1) {
        __auto_type _ = _t605;
#line 56 "src/resolver.tr"
        return false;
    }
}

