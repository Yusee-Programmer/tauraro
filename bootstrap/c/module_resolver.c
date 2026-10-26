#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) ModuleResolver* ModuleResolver_init() {
#line 74 "src/resolver.tr"
    ModuleResolver* r = ((ModuleResolver*)_tr_obj_alloc(sizeof(ModuleResolver)));
#line 75 "src/resolver.tr"
    r->visited = _tr_dict_new(128LL);
#line 76 "src/resolver.tr"
    r->all_decls = (void*)List_ptr_new();
#line 77 "src/resolver.tr"
    r->search_paths = (void*)List_TrStr_new();
#line 78 "src/resolver.tr"
    r->mod_dot_paths = (void*)List_TrStr_new();
#line 79 "src/resolver.tr"
    r->mod_file_paths = (void*)List_TrStr_new();
#line 80 "src/resolver.tr"
    r->all_decl_modules = (void*)List_TrStr_new();
#line 81 "src/resolver.tr"
    r->current_mod = _tr_str_lit_len("", 0LL);
#line 82 "src/resolver.tr"
    r->parse_errors = 0LL;
#line 83 "src/resolver.tr"
    r->fn_owner = _tr_dict_new(256LL);
#line 84 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len(".", 1LL));
#line 85 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len("tauraro", 7LL));
#line 86 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len("..", 2LL));
#line 87 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len("../..", 5LL));
#line 88 "src/resolver.tr"
    List_TrStr_append(r->search_paths, _tr_str_lit_len("std", 3LL));
#line 89 "src/resolver.tr"
    return r;
}

__attribute__((hot)) void ModuleResolver_add_search_path(ModuleResolver* self, TrStr p) {
#line 92 "src/resolver.tr"
    if (_tr_str_eqv((p), (_tr_str_lit_len("", 0LL)))) {
#line 92 "src/resolver.tr"
        return;
    }
#line 93 "src/resolver.tr"
    long long i = 0LL;
#line 94 "src/resolver.tr"
    while ((i < self->search_paths->len)) {
#line 95 "src/resolver.tr"
        if (({ TrStr _cmpa_t512 = (List_TrStr_get(self->search_paths, i)); TrStr _cmpb_t513 = (p); _Bool _cmpr_t514 = _tr_str_eqv(_cmpa_t512, _cmpb_t513); _tr_str_release(_cmpa_t512); _cmpr_t514; })) {
#line 95 "src/resolver.tr"
            return;
        }
#line 96 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 97 "src/resolver.tr"
    List_TrStr_append(self->search_paths, p);
}

__attribute__((hot)) TrStr ModuleResolver_dir_of_path(ModuleResolver* self, TrStr path) {
#line 101 "src/resolver.tr"
    char* p = ((char*)(_tr_strz(path)));
#line 102 "src/resolver.tr"
    long long n = 0LL;
#line 103 "src/resolver.tr"
    while ((((long long)((*(p + n)))) != 0LL)) {
#line 103 "src/resolver.tr"
        n = (n + 1LL);
    }
#line 104 "src/resolver.tr"
    long long last = (0LL - 1LL);
#line 105 "src/resolver.tr"
    long long i = 0LL;
#line 106 "src/resolver.tr"
    while ((i < n)) {
#line 107 "src/resolver.tr"
        long long c = ((long long)((*(p + i))));
#line 108 "src/resolver.tr"
        if (((c == 47LL) || (c == 92LL))) {
#line 108 "src/resolver.tr"
            last = i;
        }
#line 109 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 110 "src/resolver.tr"
    if ((last < 0LL)) {
#line 110 "src/resolver.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 111 "src/resolver.tr"
    return _tr_str_slicev((path), 0LL, last);
}

__attribute__((hot)) TrStr ModuleResolver_base_of_path(ModuleResolver* self, TrStr path) {
#line 115 "src/resolver.tr"
    char* p = ((char*)(_tr_strz(path)));
#line 116 "src/resolver.tr"
    long long n = 0LL;
#line 117 "src/resolver.tr"
    while ((((long long)((*(p + n)))) != 0LL)) {
#line 117 "src/resolver.tr"
        n = (n + 1LL);
    }
#line 118 "src/resolver.tr"
    long long last = (0LL - 1LL);
#line 119 "src/resolver.tr"
    long long i = 0LL;
#line 120 "src/resolver.tr"
    while ((i < n)) {
#line 121 "src/resolver.tr"
        long long c = ((long long)((*(p + i))));
#line 122 "src/resolver.tr"
        if (((c == 47LL) || (c == 92LL))) {
#line 122 "src/resolver.tr"
            last = i;
        }
#line 123 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 124 "src/resolver.tr"
    return _tr_str_slicev((path), (last + 1LL), n);
}

__attribute__((hot)) Program* ModuleResolver_resolve_main(ModuleResolver* self, TrStr main_path) {
#line 134 "src/resolver.tr"
    long long dir_end = 0LL;
#line 135 "src/resolver.tr"
    long long ci = 0LL;
#line 136 "src/resolver.tr"
    char* mp = ((char*)(_tr_strz(main_path)));
#line 137 "src/resolver.tr"
    while ((((long long)((*(mp + ci)))) != 0LL)) {
#line 138 "src/resolver.tr"
        long long ch = ((long long)((*(mp + ci))));
#line 139 "src/resolver.tr"
        if (((ch == 47LL) || (ch == 92LL))) {
#line 140 "src/resolver.tr"
            dir_end = ci;
        }
#line 141 "src/resolver.tr"
        ci = (ci + 1LL);
    }
#line 142 "src/resolver.tr"
    if ((dir_end > 0LL)) {
#line 144 "src/resolver.tr"
        TrStr dir_str = _tr_str_lit_len("", 0LL);
#line 145 "src/resolver.tr"
        /* unsafe block */
#line 146 "src/resolver.tr"
        TrStr _strtmp_t515 = _tr_str_slicev((main_path), 0LL, dir_end);
        _tr_str_release(dir_str);
        dir_str = _strtmp_t515;
#line 147 "src/resolver.tr"
        List_TrStr_append(self->search_paths, dir_str);
#line 151 "src/resolver.tr"
        long long par_end = 0LL;
#line 152 "src/resolver.tr"
        long long pj = 0LL;
#line 153 "src/resolver.tr"
        char* dp = ((char*)(_tr_strz(dir_str)));
#line 154 "src/resolver.tr"
        while ((((long long)((*(dp + pj)))) != 0LL)) {
#line 155 "src/resolver.tr"
            long long dch = ((long long)((*(dp + pj))));
#line 156 "src/resolver.tr"
            if (((dch == 47LL) || (dch == 92LL))) {
#line 156 "src/resolver.tr"
                par_end = pj;
            }
#line 157 "src/resolver.tr"
            pj = (pj + 1LL);
        }
#line 163 "src/resolver.tr"
        TrStr parent_dir = _tr_str_lit_len(".", 1LL);
#line 164 "src/resolver.tr"
        if ((par_end > 0LL)) {
#line 165 "src/resolver.tr"
            /* unsafe block */
#line 166 "src/resolver.tr"
            TrStr _strtmp_t516 = _tr_str_slicev((dir_str), 0LL, par_end);
            _tr_str_release(parent_dir);
            parent_dir = _strtmp_t516;
#line 167 "src/resolver.tr"
            List_TrStr_append(self->search_paths, parent_dir);
#line 168 "src/resolver.tr"
            long long gp_end = 0LL;
#line 169 "src/resolver.tr"
            long long gj = 0LL;
#line 170 "src/resolver.tr"
            char* gp_p = ((char*)(_tr_strz(parent_dir)));
#line 171 "src/resolver.tr"
            while ((((long long)((*(gp_p + gj)))) != 0LL)) {
#line 172 "src/resolver.tr"
                long long gch = ((long long)((*(gp_p + gj))));
#line 173 "src/resolver.tr"
                if (((gch == 47LL) || (gch == 92LL))) {
#line 173 "src/resolver.tr"
                    gp_end = gj;
                }
#line 174 "src/resolver.tr"
                gj = (gj + 1LL);
            }
#line 175 "src/resolver.tr"
            if ((gp_end > 0LL)) {
#line 176 "src/resolver.tr"
                TrStr gp_dir = _tr_str_lit_len("", 0LL);
#line 177 "src/resolver.tr"
                /* unsafe block */
#line 178 "src/resolver.tr"
                TrStr _strtmp_t517 = _tr_str_slicev((parent_dir), 0LL, gp_end);
                _tr_str_release(gp_dir);
                gp_dir = _strtmp_t517;
#line 179 "src/resolver.tr"
                List_TrStr_append(self->search_paths, gp_dir);
                _tr_str_release(gp_dir);
            }
        }
#line 187 "src/resolver.tr"
        void* vh = _tr_opendir(_tr_strz(parent_dir));
#line 188 "src/resolver.tr"
        if ((((unsigned long long)(vh)) != ((unsigned long long)(0LL)))) {
#line 189 "src/resolver.tr"
            TrStr vname = _tr_str_wrap(_tr_readdir(vh));
#line 190 "src/resolver.tr"
            while ((!_tr_str_eqv((vname), (_tr_str_lit_len("", 0LL))))) {
#line 191 "src/resolver.tr"
                if (((!_tr_str_eqv((vname), (_tr_str_lit_len(".", 1LL)))) && (!_tr_str_eqv((vname), (_tr_str_lit_len("..", 2LL)))))) {
#line 192 "src/resolver.tr"
                    TrStr vpkg_src = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((parent_dir), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/src", 4LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 193 "src/resolver.tr"
                    if (_tr_dir_exists(_tr_strz(vpkg_src))) {
#line 194 "src/resolver.tr"
                        List_TrStr_append(self->search_paths, vpkg_src);
                    }
                }
#line 195 "src/resolver.tr"
                TrStr _strtmp_t518 = _tr_str_wrap(_tr_readdir(vh));
                _tr_str_release(vname);
                vname = _strtmp_t518;
            }
#line 196 "src/resolver.tr"
            _tr_closedir(vh);
            _tr_str_release(vname);
        }
    }
#line 197 "src/resolver.tr"
    ModuleResolver_resolve_file(self, main_path, true);
#line 198 "src/resolver.tr"
    Program* p = Program_init();
#line 199 "src/resolver.tr"
    long long i = 0LL;
#line 200 "src/resolver.tr"
    while ((i < self->all_decls->len)) {
#line 201 "src/resolver.tr"
        Program_push(p, ((Decl*)List_ptr_get(self->all_decls, i)));
#line 202 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 203 "src/resolver.tr"
    return p;
}

__attribute__((hot)) void ModuleResolver_resolve_file(ModuleResolver* self, TrStr path, bool is_root) {
#line 209 "src/resolver.tr"
    if (_tr_dict_contains(self->visited, _tr_strz(path))) {
#line 209 "src/resolver.tr"
        return;
    }
#line 210 "src/resolver.tr"
    _tr_dict_set(self->visited, _tr_strz(path), true);
#line 212 "src/resolver.tr"
    TrStr source = read_file(path);
#line 213 "src/resolver.tr"
    if (_tr_str_eqv((source), (_tr_str_lit_len("", 0LL)))) {
#line 215 "src/resolver.tr"
        char* p_ptr = ((char*)(_tr_strz(path)));
#line 216 "src/resolver.tr"
        long long slen = 0LL;
#line 217 "src/resolver.tr"
        while ((((long long)((*(p_ptr + slen)))) != 0LL)) {
#line 218 "src/resolver.tr"
            slen = (slen + 1LL);
        }
#line 219 "src/resolver.tr"
        bool has_tr = false;
#line 220 "src/resolver.tr"
        if ((slen >= 3LL)) {
#line 221 "src/resolver.tr"
            if ((((long long)((*(p_ptr + (slen - 3LL))))) == 46LL)) {
#line 222 "src/resolver.tr"
                if ((((long long)((*(p_ptr + (slen - 2LL))))) == 116LL)) {
#line 223 "src/resolver.tr"
                    if ((((long long)((*(p_ptr + (slen - 1LL))))) == 114LL)) {
#line 224 "src/resolver.tr"
                        has_tr = true;
                    }
                }
            }
        }
#line 225 "src/resolver.tr"
        if ((!has_tr)) {
#line 226 "src/resolver.tr"
            TrStr _strtmp_t520 = ({ TrStr _at_t519 = (_tr_strx_concatv((path), (_tr_str_lit_len(".tr", 3LL)))); __auto_type _wr = (read_file(_at_t519)); _tr_str_release(_at_t519); _wr; });
            _tr_str_release(source);
            source = _strtmp_t520;
        }
#line 227 "src/resolver.tr"
        if (_tr_str_eqv((source), (_tr_str_lit_len("", 0LL)))) {
#line 227 "src/resolver.tr"
            _tr_str_release(source);
            return;
        }
    }
#line 234 "src/resolver.tr"
    bool file_trusted = (ModuleResolver__path_is_lib(self, path) || ModuleResolver__source_is_trusted(self, source));
#line 236 "src/resolver.tr"
    ({ TrStr _aet_t521 = (_tr_strx_concatv((_tr_str_lit_len("parsing ", 8LL)), (path))); _tr_set_ice_ctx(_aet_t521.data); _tr_str_release(_aet_t521); });
#line 237 "src/resolver.tr"
    Lexer* lexer = Lexer_init(source);
#line 238 "src/resolver.tr"
    List_Token* tokens = Lexer_tokenize(lexer);
#line 239 "src/resolver.tr"
    ModuleResolver__dedupe_private_fns(self, tokens, path, is_root, ModuleResolver__source_is_trusted(self, source));
#line 240 "src/resolver.tr"
    Parser* parser = Parser_init(tokens, lexer->token_lines);
#line 241 "src/resolver.tr"
    parser->current_file = _tr_str_retain(path);
#line 242 "src/resolver.tr"
    parser->cols = lexer->token_cols;
#line 243 "src/resolver.tr"
    parser->src_text = _tr_str_retain(source);
#line 244 "src/resolver.tr"
    Program* prog = Parser_parse_program(parser);
#line 245 "src/resolver.tr"
    self->parse_errors = (self->parse_errors + parser->error_count);
#line 248 "src/resolver.tr"
    long long i = 0LL;
#line 249 "src/resolver.tr"
    while ((i < Program_len(prog))) {
#line 250 "src/resolver.tr"
        Decl* decl_ptr = Program_get(prog, i);
#line 251 "src/resolver.tr"
        __auto_type _t522 = (*decl_ptr);
        if (_t522.tag == Decl_DImport) {
            __auto_type mod_path = _t522.data.DImport.path;
__auto_type alias = _t522.data.DImport.alias;
#line 253 "src/resolver.tr"
            ModuleResolver_resolve_module_path(self, mod_path);
        } else if (_t522.tag == Decl_DFromImport) {
            __auto_type mod_path = _t522.data.DFromImport.path;
__auto_type items = _t522.data.DFromImport.items;
#line 255 "src/resolver.tr"
            ModuleResolver_resolve_module_path(self, mod_path);
        } else if (1) {
            __auto_type _ = _t522;
#line 265 "src/resolver.tr"
            if (file_trusted) {
#line 266 "src/resolver.tr"
                ModuleResolver__mark_decl_lib(self, decl_ptr);
            }
#line 267 "src/resolver.tr"
            List_ptr_append(self->all_decls, decl_ptr);
#line 268 "src/resolver.tr"
            List_TrStr_append(self->all_decl_modules, self->current_mod);
        }
#line 269 "src/resolver.tr"
        i = (i + 1LL);
    }
    _tr_str_release(source);
    _tr_obj_release(lexer, _trdrop_Lexer);
    _tr_obj_release(parser, _trdrop_Parser);
    _tr_obj_release(prog, _trdrop_Program);
}

__attribute__((hot)) void ModuleResolver__dedupe_private_fns(ModuleResolver* self, List_Token* tokens, TrStr path, bool is_root, bool trusted) {
#line 278 "src/resolver.tr"
    bool exempt = (((is_root || trusted) || ModuleResolver__contains(self, path, _tr_str_lit_len("core/", 5LL))) || ModuleResolver__contains(self, path, _tr_str_lit_len("core\\", 5LL)));
#line 279 "src/resolver.tr"
    TrMap* renames = _tr_dict_new(8LL);
#line 280 "src/resolver.tr"
    long long depth = 0LL;
#line 281 "src/resolver.tr"
    long long i = 0LL;
#line 282 "src/resolver.tr"
    while ((i < tokens->len)) {
#line 283 "src/resolver.tr"
        long long nxt = 0LL;
#line 284 "src/resolver.tr"
        __auto_type _t523 = List_Token_get(tokens, i);
        if (_t523.tag == Token_Indent) {
            depth = (depth + 1LL);
        } else if (_t523.tag == Token_Dedent) {
            depth = (depth - 1LL);
        } else if (_t523.tag == Token_KwDef) {
#line 288 "src/resolver.tr"
            if (((depth == 0LL) && ((i + 1LL) < tokens->len))) {
#line 289 "src/resolver.tr"
                bool is_pub = false;
#line 290 "src/resolver.tr"
                if ((i > 0LL)) {
#line 291 "src/resolver.tr"
                    __auto_type _t524 = List_Token_get(tokens, (i - 1LL));
                    if (_t524.tag == Token_KwPub) {
                        is_pub = true;
                    } else if (1) {
                        __auto_type _ = _t524;
                        /* pass */
                    }
                }
#line 294 "src/resolver.tr"
                if ((!is_pub)) {
#line 295 "src/resolver.tr"
                    __auto_type _t525 = List_Token_get(tokens, (i + 1LL));
                    if (_t525.tag == Token_Ident) {
                        __auto_type nm = _t525.data.Ident.name;
#line 297 "src/resolver.tr"
                        if (((!_tr_str_eqv((nm), (_tr_str_lit_len("main", 4LL)))) && (!_tr_str_starts_withv((nm), (_tr_str_lit_len("_tr", 3LL)))))) {
#line 298 "src/resolver.tr"
                            if (_tr_dict_contains(self->fn_owner, _tr_strz(nm))) {
#line 299 "src/resolver.tr"
                                if ((({ TrStr _cmpa_t526 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->fn_owner, _tr_strz(nm))))); TrStr _cmpb_t527 = (path); _Bool _cmpr_t528 = !_tr_str_eqv(_cmpa_t526, _cmpb_t527); _tr_str_release(_cmpa_t526); _cmpr_t528; }) && (!exempt))) {
#line 300 "src/resolver.tr"
                                    ({ TrStr _dvt_t529 = (({ TrStr _tr_cl = (_tr_strx_concatv((nm), (_tr_str_lit_len("__", 2LL)))); TrStr _tr_cr = (ModuleResolver__path_tag(self, path)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _tr_dict_set(renames, _tr_strz(nm), _tr_str_box(_tr_str_retain(_dvt_t529))); _tr_str_release(_dvt_t529); });
                                }
                            } else {
#line 302 "src/resolver.tr"
                                _tr_dict_set(self->fn_owner, _tr_strz(nm), _tr_str_box(_tr_str_retain(path)));
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t525;
                        /* pass */
                    }
                }
            }
        } else if (1) {
            __auto_type _ = _t523;
            /* pass */
        }
#line 305 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 306 "src/resolver.tr"
    if ((((long long)renames->len) == 0LL)) {
#line 306 "src/resolver.tr"
        Dict_free_strval(renames);
        return;
    }
#line 307 "src/resolver.tr"
    depth = 0LL;
#line 308 "src/resolver.tr"
    i = 0LL;
#line 309 "src/resolver.tr"
    while ((i < tokens->len)) {
#line 310 "src/resolver.tr"
        Token tk = List_Token_get(tokens, i);
#line 311 "src/resolver.tr"
        __auto_type _t530 = tk;
        if (_t530.tag == Token_Indent) {
            depth = (depth + 1LL);
        } else if (_t530.tag == Token_Dedent) {
            depth = (depth - 1LL);
        } else if (_t530.tag == Token_Ident) {
            __auto_type nm = _t530.data.Ident.name;
#line 315 "src/resolver.tr"
            if (_tr_dict_contains(renames, _tr_strz(nm))) {
#line 316 "src/resolver.tr"
                bool prev_dot = false;
#line 317 "src/resolver.tr"
                bool prev_def = false;
#line 318 "src/resolver.tr"
                bool prev_open = false;
#line 319 "src/resolver.tr"
                if ((i > 0LL)) {
#line 320 "src/resolver.tr"
                    __auto_type _t531 = List_Token_get(tokens, (i - 1LL));
                    if (_t531.tag == Token_Dot) {
                        prev_dot = true;
                    } else if (_t531.tag == Token_KwDef) {
                        prev_def = true;
                    } else if (_t531.tag == Token_LParen) {
                        prev_open = true;
                    } else if (_t531.tag == Token_Comma) {
                        prev_open = true;
                    } else if (1) {
                        __auto_type _ = _t531;
                        /* pass */
                    }
                }
#line 326 "src/resolver.tr"
                bool next_call = false;
#line 327 "src/resolver.tr"
                bool next_arg_end = false;
#line 328 "src/resolver.tr"
                if (((i + 1LL) < tokens->len)) {
#line 329 "src/resolver.tr"
                    __auto_type _t532 = List_Token_get(tokens, (i + 1LL));
                    if (_t532.tag == Token_LParen) {
                        next_call = true;
                    } else if (_t532.tag == Token_Comma) {
                        next_arg_end = true;
                    } else if (_t532.tag == Token_RParen) {
                        next_arg_end = true;
                    } else if (1) {
                        __auto_type _ = _t532;
                        /* pass */
                    }
                }
#line 334 "src/resolver.tr"
                bool hit = false;
#line 335 "src/resolver.tr"
                if ((!prev_dot)) {
#line 336 "src/resolver.tr"
                    if (prev_def) {
#line 337 "src/resolver.tr"
                        hit = ((depth == 0LL) && next_call);
                    } else if (next_call) {
#line 339 "src/resolver.tr"
                        hit = true;
                    } else if ((prev_open && next_arg_end)) {
#line 341 "src/resolver.tr"
                        hit = true;
                    }
                }
#line 342 "src/resolver.tr"
                if (hit) {
#line 342 "src/resolver.tr"
                    tokens->data[(_tr_bounds_check(i, tokens->len), i)] = ({ TrStr _at_t533 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(renames, _tr_strz(nm))))); __auto_type _wr = (Token_ctor_Ident(_at_t533)); _tr_str_release(_at_t533); _wr; });
                }
            }
        } else if (1) {
            __auto_type _ = _t530;
            /* pass */
        }
#line 344 "src/resolver.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) TrStr ModuleResolver__path_tag(ModuleResolver* self, TrStr path) {
#line 348 "src/resolver.tr"
    StringBuilder* sb = StringBuilder_init((_tr_str_lenv((path)) + 1LL));
#line 349 "src/resolver.tr"
    long long i = 0LL;
#line 350 "src/resolver.tr"
    while ((i < _tr_str_lenv((path)))) {
#line 351 "src/resolver.tr"
        long long c = _tr_str_char_at_codev((path), i);
#line 352 "src/resolver.tr"
        if (((((c >= 48LL) && (c <= 57LL)) || ((c >= 65LL) && (c <= 90LL))) || ((c >= 97LL) && (c <= 122LL)))) {
#line 353 "src/resolver.tr"
            StringBuilder_append_char(sb, c);
        } else {
#line 355 "src/resolver.tr"
            StringBuilder_append_char(sb, 95LL);
        }
#line 356 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 357 "src/resolver.tr"
    TrStr out = StringBuilder_to_owned(sb);
#line 358 "src/resolver.tr"
    StringBuilder__tr_fn_free(sb);
#line 359 "src/resolver.tr"
    TrStr _trret154 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret154;
}

__attribute__((hot)) bool ModuleResolver__path_is_lib(ModuleResolver* self, TrStr path) {
#line 364 "src/resolver.tr"
    return (((ModuleResolver__contains(self, path, _tr_str_lit_len("std/", 4LL)) || ModuleResolver__contains(self, path, _tr_str_lit_len("std\\", 4LL))) || ModuleResolver__contains(self, path, _tr_str_lit_len("core/", 5LL))) || ModuleResolver__contains(self, path, _tr_str_lit_len("core\\", 5LL)));
}

__attribute__((hot)) bool ModuleResolver__source_is_trusted(ModuleResolver* self, TrStr source) {
#line 368 "src/resolver.tr"
    long long n = _tr_str_lenv((source));
#line 369 "src/resolver.tr"
    long long lim = 400LL;
#line 370 "src/resolver.tr"
    if ((n < lim)) {
#line 370 "src/resolver.tr"
        lim = n;
    }
#line 371 "src/resolver.tr"
    if ((lim <= 0LL)) {
#line 371 "src/resolver.tr"
        return false;
    }
#line 372 "src/resolver.tr"
    return ({ TrStr _at_t534 = (_tr_str_slicev((source), 0LL, lim)); __auto_type _wr = (ModuleResolver__contains(self, _at_t534, _tr_str_lit_len("@trusted", 8LL))); _tr_str_release(_at_t534); _wr; });
}

__attribute__((hot)) bool ModuleResolver__contains(ModuleResolver* self, TrStr hay, TrStr needle) {
#line 375 "src/resolver.tr"
    long long hl = _tr_str_lenv((hay));
#line 376 "src/resolver.tr"
    long long nl = _tr_str_lenv((needle));
#line 377 "src/resolver.tr"
    if (((nl == 0LL) || (nl > hl))) {
#line 377 "src/resolver.tr"
        return false;
    }
#line 378 "src/resolver.tr"
    long long i = 0LL;
#line 379 "src/resolver.tr"
    while (((i + nl) <= hl)) {
#line 380 "src/resolver.tr"
        if (({ TrStr _cmpa_t535 = (_tr_str_slicev((hay), i, (i + nl))); TrStr _cmpb_t536 = (needle); _Bool _cmpr_t537 = _tr_str_eqv(_cmpa_t535, _cmpb_t536); _tr_str_release(_cmpa_t535); _cmpr_t537; })) {
#line 380 "src/resolver.tr"
            return true;
        }
#line 381 "src/resolver.tr"
        i = (i + 1LL);
    }
#line 382 "src/resolver.tr"
    return false;
}

__attribute__((hot)) void ModuleResolver__mark_decl_lib(ModuleResolver* self, Decl* dp) {
#line 386 "src/resolver.tr"
    if ((((unsigned long long)(dp)) == ((unsigned long long)(0LL)))) {
#line 386 "src/resolver.tr"
        return;
    }
#line 387 "src/resolver.tr"
    __auto_type _t538 = (*dp);
    if (_t538.tag == Decl_DFunction) {
        __auto_type f = _t538.data.DFunction.func;
        f->is_lib = true;
    } else if (_t538.tag == Decl_DClass) {
        __auto_type c = _t538.data.DClass.cls;
        ModuleResolver__mark_methods_lib(self, c);
    } else if (_t538.tag == Decl_DActor) {
        __auto_type c = _t538.data.DActor.cls;
        ModuleResolver__mark_methods_lib(self, c);
    } else if (_t538.tag == Decl_DExtend) {
        __auto_type ms = _t538.data.DExtend.methods;
#line 392 "src/resolver.tr"
        long long i = 0LL;
#line 393 "src/resolver.tr"
        while ((i < ms->len)) {
#line 394 "src/resolver.tr"
            ((FunctionDef*)List_ptr_get(ms, i))->is_lib = true;
#line 395 "src/resolver.tr"
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t538;
        /* pass */
    }
}

__attribute__((hot)) void ModuleResolver__mark_methods_lib(ModuleResolver* self, ClassDef* c) {
#line 399 "src/resolver.tr"
    long long i = 0LL;
#line 400 "src/resolver.tr"
    while ((i < c->methods->len)) {
#line 401 "src/resolver.tr"
        ((FunctionDef*)List_ptr_get(c->methods, i))->is_lib = true;
#line 402 "src/resolver.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void ModuleResolver_resolve_recursive(ModuleResolver* self, TrStr path) {
#line 406 "src/resolver.tr"
    ModuleResolver_resolve_file(self, path, false);
}

__attribute__((hot)) void ModuleResolver_resolve_module_path(ModuleResolver* self, TrStr mod_path) {
#line 416 "src/resolver.tr"
    long long n = 0LL;
#line 417 "src/resolver.tr"
    char* p = ((char*)(_tr_strz(mod_path)));
#line 418 "src/resolver.tr"
    while ((((long long)((*(p + n)))) != 0LL)) {
#line 418 "src/resolver.tr"
        n = (n + 1LL);
    }
#line 419 "src/resolver.tr"
    TrStr dir_path = _tr_str_slicev((mod_path), 0LL, n);
#line 420 "src/resolver.tr"
    char* bp = ((char*)(_tr_strz(dir_path)));
#line 421 "src/resolver.tr"
    long long j = 0LL;
#line 422 "src/resolver.tr"
    while ((j < n)) {
#line 423 "src/resolver.tr"
        if ((((long long)((*(bp + j)))) == 46LL)) {
#line 424 "src/resolver.tr"
            /* unsafe block */
#line 424 "src/resolver.tr"
            (*(bp + j) = ((char)(47LL)));
        }
#line 425 "src/resolver.tr"
        j = (j + 1LL);
    }
#line 428 "src/resolver.tr"
    TrStr saved_mod = _tr_str_retain(self->current_mod);
#line 429 "src/resolver.tr"
    self->current_mod = _tr_str_retain(mod_path);
#line 432 "src/resolver.tr"
    long long sidx = 0LL;
#line 433 "src/resolver.tr"
    while ((sidx < self->search_paths->len)) {
#line 434 "src/resolver.tr"
        TrStr base = List_TrStr_get(self->search_paths, sidx);
#line 437 "src/resolver.tr"
        TrStr pkg_path = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/mod.tr", 7LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 438 "src/resolver.tr"
        if (file_exists(pkg_path)) {
#line 445 "src/resolver.tr"
            ({ TrStr _at_t539 = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); ModuleResolver_add_search_path(self, _at_t539); _tr_str_release(_at_t539); });
#line 446 "src/resolver.tr"
            ({ TrStr _at_t540 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/src", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); ModuleResolver_add_search_path(self, _at_t540); _tr_str_release(_at_t540); });
#line 447 "src/resolver.tr"
            bool already = _tr_dict_contains(self->visited, _tr_strz(pkg_path));
#line 448 "src/resolver.tr"
            ModuleResolver_resolve_file(self, pkg_path, false);
#line 449 "src/resolver.tr"
            if ((!already)) {
#line 450 "src/resolver.tr"
                List_TrStr_append(self->mod_dot_paths, mod_path);
#line 451 "src/resolver.tr"
                List_TrStr_append(self->mod_file_paths, pkg_path);
            }
#line 452 "src/resolver.tr"
            self->current_mod = _tr_str_retain(saved_mod);
#line 453 "src/resolver.tr"
            _tr_str_release(dir_path);
            _tr_str_release(saved_mod);
            _tr_str_release(base);
            _tr_str_release(pkg_path);
            return;
        }
#line 456 "src/resolver.tr"
        TrStr flat_path = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".tr", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 457 "src/resolver.tr"
        if (file_exists(flat_path)) {
#line 458 "src/resolver.tr"
            bool already = _tr_dict_contains(self->visited, _tr_strz(flat_path));
#line 459 "src/resolver.tr"
            ModuleResolver_resolve_file(self, flat_path, false);
#line 460 "src/resolver.tr"
            if ((!already)) {
#line 461 "src/resolver.tr"
                List_TrStr_append(self->mod_dot_paths, mod_path);
#line 462 "src/resolver.tr"
                List_TrStr_append(self->mod_file_paths, flat_path);
            }
#line 463 "src/resolver.tr"
            self->current_mod = _tr_str_retain(saved_mod);
#line 464 "src/resolver.tr"
            _tr_str_release(dir_path);
            _tr_str_release(saved_mod);
            _tr_str_release(base);
            _tr_str_release(pkg_path);
            _tr_str_release(flat_path);
            return;
        }
#line 472 "src/resolver.tr"
        TrStr pkg_src_path = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/src/", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (ModuleResolver_base_of_path(self, dir_path)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".tr", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 473 "src/resolver.tr"
        if (file_exists(pkg_src_path)) {
#line 479 "src/resolver.tr"
            ({ TrStr _at_t541 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("/", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dir_path)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("/src", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); ModuleResolver_add_search_path(self, _at_t541); _tr_str_release(_at_t541); });
#line 480 "src/resolver.tr"
            bool already3 = _tr_dict_contains(self->visited, _tr_strz(pkg_src_path));
#line 481 "src/resolver.tr"
            ModuleResolver_resolve_file(self, pkg_src_path, false);
#line 482 "src/resolver.tr"
            if ((!already3)) {
#line 483 "src/resolver.tr"
                List_TrStr_append(self->mod_dot_paths, mod_path);
#line 484 "src/resolver.tr"
                List_TrStr_append(self->mod_file_paths, pkg_src_path);
            }
#line 485 "src/resolver.tr"
            self->current_mod = _tr_str_retain(saved_mod);
#line 486 "src/resolver.tr"
            _tr_str_release(dir_path);
            _tr_str_release(saved_mod);
            _tr_str_release(base);
            _tr_str_release(pkg_path);
            _tr_str_release(flat_path);
            _tr_str_release(pkg_src_path);
            return;
        }
#line 488 "src/resolver.tr"
        sidx = (sidx + 1LL);
        _tr_str_release(base);
        _tr_str_release(pkg_path);
        _tr_str_release(flat_path);
        _tr_str_release(pkg_src_path);
    }
#line 490 "src/resolver.tr"
    self->current_mod = _tr_str_retain(saved_mod);
    _tr_str_release(dir_path);
    _tr_str_release(saved_mod);
}

__attribute__((hot)) bool decl_is_pub(Decl d) {
#line 35 "src/resolver.tr"
    __auto_type _t542 = d;
    if (_t542.tag == Decl_DFunction) {
        __auto_type f = _t542.data.DFunction.func;
#line 37 "src/resolver.tr"
        bool _trret148 = f->is_public;
        return _trret148;
    } else if (_t542.tag == Decl_DClass) {
        __auto_type c = _t542.data.DClass.cls;
#line 39 "src/resolver.tr"
        bool _trret149 = c->is_public;
        return _trret149;
    } else if (_t542.tag == Decl_DEnum) {
        __auto_type e = _t542.data.DEnum.enm;
#line 41 "src/resolver.tr"
        bool _trret150 = e->is_public;
        return _trret150;
    } else if (_t542.tag == Decl_DInterface) {
        __auto_type i = _t542.data.DInterface.iface;
#line 43 "src/resolver.tr"
        bool _trret151 = i->is_public;
        return _trret151;
    } else if (_t542.tag == Decl_DActor) {
        __auto_type c = _t542.data.DActor.cls;
#line 45 "src/resolver.tr"
        bool _trret152 = c->is_public;
        return _trret152;
    } else if (_t542.tag == Decl_DExtern) {
        __auto_type abi = _t542.data.DExtern.abi;
__auto_type funcs = _t542.data.DExtern.functions;
#line 47 "src/resolver.tr"
        return true;
    } else if (_t542.tag == Decl_DExtend) {
        __auto_type target = _t542.data.DExtend.target;
__auto_type methods = _t542.data.DExtend.methods;
#line 49 "src/resolver.tr"
        return true;
    } else if (_t542.tag == Decl_DDecoratorDef) {
        __auto_type f = _t542.data.DDecoratorDef.func;
#line 51 "src/resolver.tr"
        bool _trret153 = f->is_public;
        return _trret153;
    } else if (1) {
        __auto_type _ = _t542;
#line 53 "src/resolver.tr"
        return false;
    }
}

