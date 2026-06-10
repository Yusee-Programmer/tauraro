#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) ModuleResolver* ModuleResolver_init() {
    /* pass */
    ModuleResolver* r = ((ModuleResolver*)_tr_checked_alloc(sizeof(ModuleResolver)));
    /* pass */
    r->visited = _tr_dict_new(128LL);
    /* pass */
    r->all_decls = (void*)List_ptr_new();
    /* pass */
    r->search_paths = (void*)List_str_new();
    /* pass */
    r->mod_dot_paths = (void*)List_str_new();
    /* pass */
    r->mod_file_paths = (void*)List_str_new();
    /* pass */
    r->all_decl_modules = (void*)List_str_new();
    /* pass */
    r->current_mod = "";
    /* pass */
    r->parse_errors = 0LL;
    /* pass */
    List_str_append(r->search_paths, ".");
    /* pass */
    List_str_append(r->search_paths, "tauraro");
    /* pass */
    List_str_append(r->search_paths, "..");
    /* pass */
    List_str_append(r->search_paths, "../..");
    /* pass */
    List_str_append(r->search_paths, "std");
    /* pass */
    return r;
}

__attribute__((hot)) void ModuleResolver_add_search_path(ModuleResolver* self, char* p) {
    /* pass */
    List_str_append(self->search_paths, p);
}

__attribute__((hot)) Program* ModuleResolver_resolve_main(ModuleResolver* self, char* main_path) {
    /* pass */
    long long dir_end = 0LL;
    /* pass */
    long long ci = 0LL;
    /* pass */
    char* mp = ((char*)(main_path));
    /* pass */
    while ((((long long)((*(mp + ci)))) != 0LL)) {
        /* pass */
        long long ch = ((long long)((*(mp + ci))));
        /* pass */
        if (((ch == 47LL) || (ch == 92LL))) {
            /* pass */
            dir_end = ci;
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    if ((dir_end > 0LL)) {
        /* pass */
        char* dir_str = "";
        /* pass */
        /* unsafe block */
        /* pass */
        dir_str = _tr_str_slice(main_path, 0LL, dir_end);
        /* pass */
        List_str_append(self->search_paths, dir_str);
        /* pass */
        long long par_end = 0LL;
        /* pass */
        long long pj = 0LL;
        /* pass */
        char* dp = ((char*)(dir_str));
        /* pass */
        while ((((long long)((*(dp + pj)))) != 0LL)) {
            /* pass */
            long long dch = ((long long)((*(dp + pj))));
            /* pass */
            if (((dch == 47LL) || (dch == 92LL))) {
                /* pass */
                par_end = pj;
            }
            /* pass */
            pj = (pj + 1LL);
        }
        /* pass */
        if ((par_end > 0LL)) {
            /* pass */
            char* parent_dir = "";
            /* pass */
            /* unsafe block */
            /* pass */
            parent_dir = _tr_str_slice(dir_str, 0LL, par_end);
            /* pass */
            List_str_append(self->search_paths, parent_dir);
            /* pass */
            long long gp_end = 0LL;
            /* pass */
            long long gj = 0LL;
            /* pass */
            char* gp_p = ((char*)(parent_dir));
            /* pass */
            while ((((long long)((*(gp_p + gj)))) != 0LL)) {
                /* pass */
                long long gch = ((long long)((*(gp_p + gj))));
                /* pass */
                if (((gch == 47LL) || (gch == 92LL))) {
                    /* pass */
                    gp_end = gj;
                }
                /* pass */
                gj = (gj + 1LL);
            }
            /* pass */
            if ((gp_end > 0LL)) {
                /* pass */
                char* gp_dir = "";
                /* pass */
                /* unsafe block */
                /* pass */
                gp_dir = _tr_str_slice(parent_dir, 0LL, gp_end);
                /* pass */
                List_str_append(self->search_paths, gp_dir);
            }
        }
    }
    /* pass */
    ModuleResolver_resolve_file(self, main_path, true);
    /* pass */
    Program* p = Program_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->all_decls->len)) {
        /* pass */
        Program_push(p, ((Decl*)List_ptr_get(self->all_decls, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return p;
}

__attribute__((hot)) void ModuleResolver_resolve_file(ModuleResolver* self, char* path, bool is_root) {
    /* pass */
    if (_tr_dict_contains(self->visited, path)) {
        /* pass */
        return;
    }
    /* pass */
    _tr_dict_set(self->visited, path, true);
    /* pass */
    char* source = read_file(path);
    /* pass */
    if ((strcmp((char*)source, (char*)"") == 0)) {
        /* pass */
        char* p_ptr = ((char*)(path));
        /* pass */
        long long slen = 0LL;
        /* pass */
        while ((((long long)((*(p_ptr + slen)))) != 0LL)) {
            /* pass */
            slen = (slen + 1LL);
        }
        /* pass */
        bool has_tr = false;
        /* pass */
        if ((slen >= 3LL)) {
            /* pass */
            if ((((long long)((*(p_ptr + (slen - 3LL))))) == 46LL)) {
                /* pass */
                if ((((long long)((*(p_ptr + (slen - 2LL))))) == 116LL)) {
                    /* pass */
                    if ((((long long)((*(p_ptr + (slen - 1LL))))) == 114LL)) {
                        /* pass */
                        has_tr = true;
                    }
                }
            }
        }
        /* pass */
        if ((!has_tr)) {
            /* pass */
            source = read_file(_tr_str_concat(path, ".tr"));
        }
        /* pass */
        if ((strcmp((char*)source, (char*)"") == 0)) {
            /* pass */
            return;
        }
    }
    /* pass */
    Lexer* lexer = Lexer_init(source);
    /* pass */
    List_Token* tokens = Lexer_tokenize(lexer);
    /* pass */
    Parser* parser = Parser_init(tokens, lexer->token_lines);
    /* pass */
    Program* prog = Parser_parse_program(parser);
    /* pass */
    self->parse_errors = (self->parse_errors + parser->error_count);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < Program_len(prog))) {
        /* pass */
        Decl* decl_ptr = Program_get(prog, i);
        /* pass */
        __auto_type _t126 = (*decl_ptr);
        if (_t126.tag == Decl_DImport) {
            __auto_type mod_path = _t126.data.DImport.path;
__auto_type alias = _t126.data.DImport.alias;
            /* pass */
            ModuleResolver_resolve_module_path(self, mod_path);
        } else if (_t126.tag == Decl_DFromImport) {
            __auto_type mod_path = _t126.data.DFromImport.path;
__auto_type items = _t126.data.DFromImport.items;
            /* pass */
            ModuleResolver_resolve_module_path(self, mod_path);
        } else if (1) {
            __auto_type _ = _t126;
            /* pass */
            List_ptr_append(self->all_decls, decl_ptr);
            /* pass */
            List_str_append(self->all_decl_modules, self->current_mod);
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void ModuleResolver_resolve_recursive(ModuleResolver* self, char* path) {
    /* pass */
    ModuleResolver_resolve_file(self, path, false);
}

__attribute__((hot)) void ModuleResolver_resolve_module_path(ModuleResolver* self, char* mod_path) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(mod_path));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    __auto_type dir_path = _tr_str_slice(mod_path, 0LL, n);
    /* pass */
    char* bp = ((char*)(dir_path));
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        if ((((long long)((*(bp + j)))) == 46LL)) {
            /* pass */
            (*(bp + j) = ((char)(47LL)));
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    char* saved_mod = self->current_mod;
    /* pass */
    self->current_mod = mod_path;
    /* pass */
    long long sidx = 0LL;
    /* pass */
    while ((sidx < self->search_paths->len)) {
        /* pass */
        char* base = List_str_get(self->search_paths, sidx);
        /* pass */
        char* pkg_path = _tr_str_concat(_tr_str_concat(_tr_str_concat(base, "/"), dir_path), "/mod.tr");
        /* pass */
        if (file_exists(pkg_path)) {
            /* pass */
            bool already = _tr_dict_contains(self->visited, pkg_path);
            /* pass */
            ModuleResolver_resolve_file(self, pkg_path, false);
            /* pass */
            if ((!already)) {
                /* pass */
                List_str_append(self->mod_dot_paths, mod_path);
                /* pass */
                List_str_append(self->mod_file_paths, StringObj_as_str(StringObj_init(pkg_path)));
            }
            /* pass */
            self->current_mod = saved_mod;
            /* pass */
            return;
        }
        /* pass */
        char* flat_path = _tr_str_concat(_tr_str_concat(_tr_str_concat(base, "/"), dir_path), ".tr");
        /* pass */
        if (file_exists(flat_path)) {
            /* pass */
            bool already = _tr_dict_contains(self->visited, flat_path);
            /* pass */
            ModuleResolver_resolve_file(self, flat_path, false);
            /* pass */
            if ((!already)) {
                /* pass */
                List_str_append(self->mod_dot_paths, mod_path);
                /* pass */
                List_str_append(self->mod_file_paths, StringObj_as_str(StringObj_init(flat_path)));
            }
            /* pass */
            self->current_mod = saved_mod;
            /* pass */
            return;
        }
        /* pass */
        sidx = (sidx + 1LL);
    }
    /* pass */
    self->current_mod = saved_mod;
}

__attribute__((hot)) bool decl_is_pub(Decl d) {
    /* pass */
    __auto_type _t127 = d;
    if (_t127.tag == Decl_DFunction) {
        __auto_type f = _t127.data.DFunction.func;
        /* pass */
        return f->is_public;
    } else if (_t127.tag == Decl_DClass) {
        __auto_type c = _t127.data.DClass.cls;
        /* pass */
        return c->is_public;
    } else if (_t127.tag == Decl_DEnum) {
        __auto_type e = _t127.data.DEnum.enm;
        /* pass */
        return e->is_public;
    } else if (_t127.tag == Decl_DInterface) {
        __auto_type i = _t127.data.DInterface.iface;
        /* pass */
        return i->is_public;
    } else if (_t127.tag == Decl_DActor) {
        __auto_type c = _t127.data.DActor.cls;
        /* pass */
        return c->is_public;
    } else if (_t127.tag == Decl_DExtern) {
        __auto_type abi = _t127.data.DExtern.abi;
__auto_type funcs = _t127.data.DExtern.functions;
        /* pass */
        return true;
    } else if (_t127.tag == Decl_DExtend) {
        __auto_type target = _t127.data.DExtend.target;
__auto_type methods = _t127.data.DExtend.methods;
        /* pass */
        return true;
    } else if (_t127.tag == Decl_DDecoratorDef) {
        __auto_type f = _t127.data.DDecoratorDef.func;
        /* pass */
        return f->is_public;
    } else if (1) {
        __auto_type _ = _t127;
        /* pass */
        return false;
    }
}

