#include "tauraro_types.h"

bool _expr_is_self_field(Expr* e);
bool _binop_is_float_name(TrStr n);
bool _hl_has_float(List_ptr* hl);
bool _block_mutates_self(Block* b);
bool _pblock_mutates_self(Block** pb);
bool _stmt_mutates_self(Stmt* s);

__attribute__((malloc,returns_nonnull,hot)) Symbol* Symbol_init(TrStr name, SymbolKind kind, AstType** ty) {
    /* pass */
    Symbol* s = ((Symbol*)_tr_obj_alloc(sizeof(Symbol)));
    /* pass */
    s->name = _tr_str_retain(name);
    /* pass */
    s->kind = kind;
    /* pass */
    s->ty = ty;
    /* pass */
    s->scope_depth = 0LL;
    /* pass */
    s->is_mut = false;
    /* pass */
    s->is_const = false;
    /* pass */
    s->is_shared = false;
    /* pass */
    s->is_moved = false;
    /* pass */
    s->active_borrows = 0LL;
    /* pass */
    s->borrowed_by = (void*)List_TrStr_new();
    /* pass */
    s->is_init = true;
    /* pass */
    s->is_param = false;
    /* pass */
    s->is_maybe_moved = false;
    /* pass */
    s->is_maybe_init = false;
    /* pass */
    s->ptr_region = 0LL;
    /* pass */
    s->is_freed = false;
    /* pass */
    s->decl_block_depth = 0LL;
    /* pass */
    s->decl_block_id = 0LL;
    /* pass */
    s->str_escaped = false;
    /* pass */
    s->coll_escaped = false;
    /* pass */
    s->borrows_region = _tr_str_lit_len("", 0LL);
    /* pass */
    return s;
}

__attribute__((malloc,returns_nonnull,hot)) Scope* Scope_init() {
    /* pass */
    Scope* s = ((Scope*)_tr_obj_alloc(sizeof(Scope)));
    /* pass */
    s->variables = _tr_dict_new(32LL);
    /* pass */
    s->decl_order = (void*)List_TrStr_new();
    /* pass */
    return s;
}

__attribute__((hot)) AstType** Sema_build_ast_type(Sema* self, Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (AstType**)(0LL);
    }
    /* pass */
    __auto_type _t332 = (*e);
    if (_t332.tag == Expr_EIdent) {
        __auto_type n = _t332.data.EIdent.name;
        /* pass */
        return box_asttype(AstType_init(n));
    } else if (_t332.tag == Expr_EIndex) {
        __auto_type obj = _t332.data.EIndex.obj;
__auto_type idx = _t332.data.EIndex._tr_v_index;
        /* pass */
        __auto_type _t333 = (*obj);
        if (_t333.tag == Expr_EIdent) {
            __auto_type on = _t333.data.EIdent.name;
            /* pass */
            AstType* at = AstType_init(on);
            /* pass */
            at->args = (void*)List_ptr_new();
            /* pass */
            __auto_type _t334 = (*idx);
            if (_t334.tag == Expr_ETuple) {
                __auto_type _bte = _t334.data.ETuple.items;
                /* pass */
                long long _bti = 0LL;
                /* pass */
                while ((_bti < _bte->len)) {
                    /* pass */
                    List_ptr_append(at->args, Sema_build_ast_type(self, ((Expr*)List_ptr_get(_bte, _bti))));
                    /* pass */
                    _bti = (_bti + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t334;
                /* pass */
                List_ptr_append(at->args, Sema_build_ast_type(self, idx));
            }
            /* pass */
            return box_asttype(at);
        } else if (1) {
            __auto_type _ = _t333;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t332;
        /* pass */
    }
    /* pass */
    return box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
}

__attribute__((hot)) AstType** Sema__targ_of(Sema* self, Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
    }
    /* pass */
    __auto_type _t335 = (*e);
    if (_t335.tag == Expr_EIdent) {
        __auto_type n = _t335.data.EIdent.name;
        return box_asttype(AstType_init(n));
    } else if (_t335.tag == Expr_EIndex) {
        return Sema_build_ast_type(self, e);
    } else if (1) {
        __auto_type _ = _t335;
        return box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
    }
}

__attribute__((malloc,returns_nonnull,hot)) Sema* Sema_init() {
    /* pass */
    Sema* s = ((Sema*)_tr_obj_alloc(sizeof(Sema)));
    /* pass */
    s->globals = _tr_dict_new(1024LL);
    /* pass */
    s->scopes = (void*)List_ptr_new();
    /* pass */
    s->errors = (void*)List_TrStr_new();
    /* pass */
    s->warnings = (void*)List_TrStr_new();
    /* pass */
    s->classes = _tr_dict_new(128LL);
    /* pass */
    s->enums = _tr_dict_new(64LL);
    /* pass */
    s->interfaces = _tr_dict_new(32LL);
    /* pass */
    s->type_aliases = _tr_dict_new(32LL);
    /* pass */
    s->type_alias_elem = _tr_dict_new(32LL);
    /* pass */
    s->current_file = _tr_str_lit_len("", 0LL);
    /* pass */
    s->current_func_name = _tr_str_lit_len("", 0LL);
    /* pass */
    s->current_class_name = _tr_str_lit_len("", 0LL);
    /* pass */
    s->current_scope_depth = 0LL;
    /* pass */
    s->in_async_fn = false;
    /* pass */
    s->assign_froms = _tr_dict_new(32LL);
    /* pass */
    s->fn_sigs = _tr_dict_new(64LL);
    /* pass */
    s->extern_names = _tr_dict_new(128LL);
    /* pass */
    s->nested_classes = (void*)List_ptr_new();
    /* pass */
    s->nested_functions = (void*)List_ptr_new();
    /* pass */
    s->nested_enums = (void*)List_ptr_new();
    /* pass */
    s->nested_interfaces = (void*)List_ptr_new();
    /* pass */
    s->current_line = 0LL;
    /* pass */
    s->current_func_generics = (void*)List_TrStr_new();
    /* pass */
    s->current_func_constraints = (void*)List_ptr_new();
    /* pass */
    s->closure_boundary = (-1LL);
    /* pass */
    s->closure_caps = (void*)List_ptr_new();
    /* pass */
    s->closure_cap_set = _tr_dict_new(0LL);
    /* pass */
    s->in_assign_target = false;
    /* pass */
    s->in_recv_pos = false;
    /* pass */
    s->container_borrows = _tr_dict_new(16LL);
    /* pass */
    s->capturing_moves = false;
    /* pass */
    s->branch_moved_buf = (void*)List_TrStr_new();
    /* pass */
    s->capturing_inits = false;
    /* pass */
    s->branch_init_buf = (void*)List_TrStr_new();
    /* pass */
    s->copy_classes = _tr_dict_new(32LL);
    /* pass */
    s->in_unsafe = false;
    /* pass */
    s->cur_fn_is_lib = false;
    /* pass */
    s->current_func_ret_from = _tr_str_lit_len("", 0LL);
    /* pass */
    s->current_func_ret_borrow_str = false;
    /* pass */
    s->current_func_ret_regions = (void*)List_TrStr_new();
    /* pass */
    s->current_func_outlives_a = (void*)List_TrStr_new();
    /* pass */
    s->current_func_outlives_b = (void*)List_TrStr_new();
    /* pass */
    s->current_region_params = (void*)List_TrStr_new();
    /* pass */
    s->cur_func_borrowers = (void*)List_TrStr_new();
    /* pass */
    s->cur_func_sources = (void*)List_TrStr_new();
    /* pass */
    s->strict_mode = false;
    /* pass */
    s->no_heap = false;
    /* pass */
    s->heap_boxes_tuples = false;
    /* pass */
    s->mutating_methods = _tr_dict_new(32LL);
    /* pass */
    s->fn_ret_owned = _tr_dict_new(64LL);
    /* pass */
    s->fn_param_consumes = _tr_dict_new(64LL);
    /* pass */
    s->ptr_aliased = _tr_dict_new(32LL);
    /* pass */
    s->decorator_names = _tr_dict_new(16LL);
    /* pass */
    s->variadic_fns = _tr_dict_new(8LL);
    /* pass */
    s->variadic_elem_ty = _tr_dict_new(8LL);
    /* pass */
    s->fn_defs = _tr_dict_new(32LL);
    /* pass */
    s->loop_scope_base = (void*)List_i64_new();
    /* pass */
    s->fn_scope_base = (void*)List_i64_new();
    /* pass */
    s->block_depth = 0LL;
    /* pass */
    s->block_depth_stack = (void*)List_i64_new();
    /* pass */
    s->next_block_id = 0LL;
    /* pass */
    s->block_stack = (void*)List_i64_new();
    /* pass */
    s->block_stack_base = (void*)List_i64_new();
    /* pass */
    s->do_temp_ctr = 0LL;
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("print", 5LL)), Symbol_init(_tr_str_lit_len("print", 5LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("input", 5LL)), Symbol_init(_tr_str_lit_len("input", 5LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("len", 3LL)), Symbol_init(_tr_str_lit_len("len", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("range", 5LL)), Symbol_init(_tr_str_lit_len("range", 5LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("type", 4LL)), Symbol_init(_tr_str_lit_len("type", 4LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("str", 3LL)), Symbol_init(_tr_str_lit_len("str", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("hex", 3LL)), Symbol_init(_tr_str_lit_len("hex", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("oct", 3LL)), Symbol_init(_tr_str_lit_len("oct", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("bin", 3LL)), Symbol_init(_tr_str_lit_len("bin", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("kernel_launch", 13LL)), Symbol_init(_tr_str_lit_len("kernel_launch", 13LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("int", 3LL)), Symbol_init(_tr_str_lit_len("int", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("float", 5LL)), Symbol_init(_tr_str_lit_len("float", 5LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("float", 5LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("bool", 4LL)), Symbol_init(_tr_str_lit_len("bool", 4LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ord", 3LL)), Symbol_init(_tr_str_lit_len("ord", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("chr", 3LL)), Symbol_init(_tr_str_lit_len("chr", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("char", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("abs", 3LL)), Symbol_init(_tr_str_lit_len("abs", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("max", 3LL)), Symbol_init(_tr_str_lit_len("max", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("min", 3LL)), Symbol_init(_tr_str_lit_len("min", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("sum", 3LL)), Symbol_init(_tr_str_lit_len("sum", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("round", 5LL)), Symbol_init(_tr_str_lit_len("round", 5LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("float", 5LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("sorted", 6LL)), Symbol_init(_tr_str_lit_len("sorted", 6LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("reversed", 8LL)), Symbol_init(_tr_str_lit_len("reversed", 8LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("iter", 4LL)), Symbol_init(_tr_str_lit_len("iter", 4LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("enumerate", 9LL)), Symbol_init(_tr_str_lit_len("enumerate", 9LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("zip", 3LL)), Symbol_init(_tr_str_lit_len("zip", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("map", 3LL)), Symbol_init(_tr_str_lit_len("map", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("filter", 6LL)), Symbol_init(_tr_str_lit_len("filter", 6LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("all", 3LL)), Symbol_init(_tr_str_lit_len("all", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("any", 3LL)), Symbol_init(_tr_str_lit_len("any", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert", 6LL)), Symbol_init(_tr_str_lit_len("assert", 6LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_eq", 9LL)), Symbol_init(_tr_str_lit_len("assert_eq", 9LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_ne", 9LL)), Symbol_init(_tr_str_lit_len("assert_ne", 9LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_lt", 9LL)), Symbol_init(_tr_str_lit_len("assert_lt", 9LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_le", 9LL)), Symbol_init(_tr_str_lit_len("assert_le", 9LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_gt", 9LL)), Symbol_init(_tr_str_lit_len("assert_gt", 9LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_ge", 9LL)), Symbol_init(_tr_str_lit_len("assert_ge", 9LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Some", 4LL)), Symbol_init(_tr_str_lit_len("Some", 4LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("Option", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Ok", 2LL)), Symbol_init(_tr_str_lit_len("Ok", 2LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("Result", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Err", 3LL)), Symbol_init(_tr_str_lit_len("Err", 3LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("Result", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("_tr_exit", 8LL)), Symbol_init(_tr_str_lit_len("_tr_exit", 8LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("_tr_strlen", 10LL)), Symbol_init(_tr_str_lit_len("_tr_strlen", 10LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("read_file", 9LL)), Symbol_init(_tr_str_lit_len("read_file", 9LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("write_file", 10LL)), Symbol_init(_tr_str_lit_len("write_file", 10LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("file_exists", 11LL)), Symbol_init(_tr_str_lit_len("file_exists", 11LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("_tr_system", 10LL)), Symbol_init(_tr_str_lit_len("_tr_system", 10LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("List", 4LL)), Symbol_init(_tr_str_lit_len("List", 4LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Pointer", 7LL)), Symbol_init(_tr_str_lit_len("Pointer", 7LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Pointer", 7LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("StringBuilder", 13LL)), Symbol_init(_tr_str_lit_len("StringBuilder", 13LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("StringBuilder", 13LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("StringObj", 9LL)), Symbol_init(_tr_str_lit_len("StringObj", 9LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("StringObj", 9LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Dict", 4LL)), Symbol_init(_tr_str_lit_len("Dict", 4LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Dict", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Set", 3LL)), Symbol_init(_tr_str_lit_len("Set", 3LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Set", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Box", 3LL)), Symbol_init(_tr_str_lit_len("Box", 3LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Box", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Option", 6LL)), Symbol_init(_tr_str_lit_len("Option", 6LL), SymbolKind_make_SEnum(), box_asttype(AstType_init(_tr_str_lit_len("Option", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Result", 6LL)), Symbol_init(_tr_str_lit_len("Result", 6LL), SymbolKind_make_SEnum(), box_asttype(AstType_init(_tr_str_lit_len("Result", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Exception", 9LL)), Symbol_init(_tr_str_lit_len("Exception", 9LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Exception", 9LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Error", 5LL)), Symbol_init(_tr_str_lit_len("Error", 5LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Error", 5LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ValueError", 10LL)), Symbol_init(_tr_str_lit_len("ValueError", 10LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("ValueError", 10LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("TypeError", 9LL)), Symbol_init(_tr_str_lit_len("TypeError", 9LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("TypeError", 9LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("IndexError", 10LL)), Symbol_init(_tr_str_lit_len("IndexError", 10LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("IndexError", 10LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("IOError", 7LL)), Symbol_init(_tr_str_lit_len("IOError", 7LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("IOError", 7LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("KeyError", 8LL)), Symbol_init(_tr_str_lit_len("KeyError", 8LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("KeyError", 8LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Task", 4LL)), Symbol_init(_tr_str_lit_len("Task", 4LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Task", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Future", 6LL)), Symbol_init(_tr_str_lit_len("Future", 6LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Future", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Channel", 7LL)), Symbol_init(_tr_str_lit_len("Channel", 7LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Channel", 7LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Chan", 4LL)), Symbol_init(_tr_str_lit_len("Chan", 4LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Chan", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Mutex", 5LL)), Symbol_init(_tr_str_lit_len("Mutex", 5LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Mutex", 5LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("RwLock", 6LL)), Symbol_init(_tr_str_lit_len("RwLock", 6LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("RwLock", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ThreadPool", 10LL)), Symbol_init(_tr_str_lit_len("ThreadPool", 10LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("ThreadPool", 10LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Thread", 6LL)), Symbol_init(_tr_str_lit_len("Thread", 6LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Thread", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("AsyncPool", 9LL)), Symbol_init(_tr_str_lit_len("AsyncPool", 9LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("AsyncPool", 9LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("AsyncTask", 9LL)), Symbol_init(_tr_str_lit_len("AsyncTask", 9LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("AsyncTask", 9LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Atomic", 6LL)), Symbol_init(_tr_str_lit_len("Atomic", 6LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Atomic", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ThreadLocal", 11LL)), Symbol_init(_tr_str_lit_len("ThreadLocal", 11LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("ThreadLocal", 11LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("await_all", 9LL)), Symbol_init(_tr_str_lit_len("await_all", 9LL), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Arc", 3LL)), Symbol_init(_tr_str_lit_len("Arc", 3LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Arc", 3LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Rc", 2LL)), Symbol_init(_tr_str_lit_len("Rc", 2LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Rc", 2LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("WaitGroup", 9LL)), Symbol_init(_tr_str_lit_len("WaitGroup", 9LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("WaitGroup", 9LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Shared", 6LL)), Symbol_init(_tr_str_lit_len("Shared", 6LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Shared", 6LL)))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Weak", 4LL)), Symbol_init(_tr_str_lit_len("Weak", 4LL), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit_len("Weak", 4LL)))));
    /* pass */
    return s;
}

__attribute__((hot)) TrStr Sema_io_ty_str(Sema* self, AstType* ty) {
    /* pass */
    TrStr s = _tr_str_retain(ty->name);
    /* pass */
    if (_tr_str_eqv((s), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        TrStr _strtmp_t336 = _tr_str_lit_len("void", 4LL);
        _tr_str_release(s);
        s = _strtmp_t336;
    }
    /* pass */
    if ((ty->args->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t337 = _tr_strx_concatv((s), (_tr_str_lit_len("[", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t337;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < ty->args->len)) {
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t338 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t338;
            }
            /* pass */
            TrStr _strtmp_t339 = ({ TrStr _cr = (Sema_io_ty_str(self, (*((AstType**)List_ptr_get(ty->args, i))))); TrStr _cres = _tr_strx_concatv((s), _cr); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t339;
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        TrStr _strtmp_t340 = _tr_strx_concatv((s), (_tr_str_lit_len("]", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t340;
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr Sema_io_doc_of(Sema* self, Block* body) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < body->stmts->len)) {
        /* pass */
        __auto_type _t341 = (*((Stmt*)List_ptr_get(body->stmts, i)));
        if (_t341.tag == Stmt_SLine) {
            __auto_type _ = _t341.data.SLine.n;
            /* pass */
        } else if (_t341.tag == Stmt_SExpr) {
            __auto_type e = _t341.data.SExpr.expr;
            /* pass */
            if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
                /* pass */
                __auto_type _t342 = (*e);
                if (_t342.tag == Expr_ELitStr) {
                    __auto_type s = _t342.data.ELitStr.val;
                    return _tr_str_retain(s);
                } else if (1) {
                    __auto_type _ = _t342;
                    /* pass */
                }
            }
            /* pass */
            return _tr_str_lit_len("", 0LL);
        } else if (1) {
            __auto_type _ = _t341;
            return _tr_str_lit_len("", 0LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Sema_io_func_sig(Sema* self, FunctionDef* f) {
    /* pass */
    TrStr s = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("def ", 4LL)), (f->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; });
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t343 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t343;
        }
        /* pass */
        Param* p = ((Param*)List_ptr_get(f->params, i));
        /* pass */
        TrStr _strtmp_t344 = _tr_strx_concatv((s), (p->name));
        _tr_str_release(s);
        s = _strtmp_t344;
        /* pass */
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            TrStr _strtmp_t345 = ({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _cr = (Sema_io_ty_str(self, (*p->ty))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t345;
        }
        /* pass */
        if (p->is_variadic) {
            /* pass */
            TrStr _strtmp_t346 = _tr_strx_concatv((s), (_tr_str_lit_len("...", 3LL)));
            _tr_str_release(s);
            s = _strtmp_t346;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr _strtmp_t347 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
    _tr_str_release(s);
    s = _strtmp_t347;
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        AstType* rt = (*f->ret_ty);
        /* pass */
        if ((((!_tr_str_eqv((rt->name), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((rt->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((rt->name), (_tr_str_lit_len("None", 4LL)))))) {
            /* pass */
            TrStr _strtmp_t348 = ({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _cr = (Sema_io_ty_str(self, rt)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t348;
        }
    }
    /* pass */
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        if ((!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            TrStr _strtmp_t349 = ({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" throws ", 8LL)))); TrStr _cr = (Sema_io_ty_str(self, (*f->throws_ty))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t349;
        }
    }
    /* pass */
    TrStr doc = Sema_io_doc_of(self, f->body);
    /* pass */
    if ((!_tr_str_eqv((doc), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        TrStr _strtmp_t350 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("\n        \"\"\"", 12LL)))); TrStr _cres = _tr_strx_concatv(_cl, (doc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\"\"\"", 3LL))); _tr_str_release(_cl); _cres; });
        _tr_str_release(s);
        s = _strtmp_t350;
    }
    /* pass */
    _tr_str_release(doc);
    return s;
}

__attribute__((hot)) TrStr Sema_build_inspect_str(Sema* self, TrStr name) {
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
        /* pass */
        ClassDef* c = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
        /* pass */
        TrStr s = _tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (c->name));
        /* pass */
        if ((c->base_classes->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t351 = _tr_strx_concatv((s), (_tr_str_lit_len("(extends ", 9LL)));
            _tr_str_release(s);
            s = _strtmp_t351;
            /* pass */
            long long bi = 0LL;
            /* pass */
            while ((bi < c->base_classes->len)) {
                /* pass */
                if ((bi > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t352 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                    _tr_str_release(s);
                    s = _strtmp_t352;
                }
                /* pass */
                TrStr _strtmp_t353 = ({ TrStr _cr = (List_TrStr_get(c->base_classes, bi)); TrStr _cres = _tr_strx_concatv((s), _cr); _tr_str_release(_cr); _cres; });
                _tr_str_release(s);
                s = _strtmp_t353;
                /* pass */
                bi = (bi + 1LL);
            }
            /* pass */
            TrStr _strtmp_t354 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
            _tr_str_release(s);
            s = _strtmp_t354;
        }
        /* pass */
        TrStr _strtmp_t355 = _tr_strx_concatv((s), (_tr_str_lit_len(":\n", 2LL)));
        _tr_str_release(s);
        s = _strtmp_t355;
        /* pass */
        if ((!_tr_str_eqv((c->docstring), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            TrStr _strtmp_t356 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("  doc: ", 7LL)))); TrStr _cres = _tr_strx_concatv(_cl, (c->docstring)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; });
            _tr_str_release(s);
            s = _strtmp_t356;
        }
        /* pass */
        if ((c->fields->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t357 = _tr_strx_concatv((s), (_tr_str_lit_len("  fields:\n", 10LL)));
            _tr_str_release(s);
            s = _strtmp_t357;
            /* pass */
            long long fi = 0LL;
            /* pass */
            while ((fi < c->fields->len)) {
                /* pass */
                FieldDef* fld = ((FieldDef*)List_ptr_get(c->fields, fi));
                /* pass */
                TrStr _strtmp_t358 = ({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _cres = _tr_strx_concatv(_cl, (fld->name)); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t358;
                /* pass */
                if ((((unsigned long long)(fld->ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    TrStr _strtmp_t359 = ({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _cr = (Sema_io_ty_str(self, (*fld->ty))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(s);
                    s = _strtmp_t359;
                }
                /* pass */
                TrStr _strtmp_t360 = _tr_strx_concatv((s), (_tr_str_lit_len("\n", 1LL)));
                _tr_str_release(s);
                s = _strtmp_t360;
                /* pass */
                fi = (fi + 1LL);
            }
        }
        /* pass */
        if ((c->methods->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t361 = _tr_strx_concatv((s), (_tr_str_lit_len("  methods:\n", 11LL)));
            _tr_str_release(s);
            s = _strtmp_t361;
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < c->methods->len)) {
                /* pass */
                TrStr _strtmp_t362 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(c->methods, mi)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t362;
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        return s;
    } else if (_tr_dict_contains(self->enums, _tr_strz(name))) {
        /* pass */
        EnumDef* e = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(name)));
        /* pass */
        TrStr s = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("enum ", 5LL)), (e->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_cl); _cres; });
        /* pass */
        if ((e->variants->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t363 = _tr_strx_concatv((s), (_tr_str_lit_len("  variants:\n", 12LL)));
            _tr_str_release(s);
            s = _strtmp_t363;
            /* pass */
            long long vi = 0LL;
            /* pass */
            while ((vi < e->variants->len)) {
                /* pass */
                VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, vi));
                /* pass */
                TrStr _strtmp_t364 = ({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _cres = _tr_strx_concatv(_cl, (v->name)); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t364;
                /* pass */
                if ((v->fields->len > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t365 = _tr_strx_concatv((s), (_tr_str_lit_len("(", 1LL)));
                    _tr_str_release(s);
                    s = _strtmp_t365;
                    /* pass */
                    long long vfi = 0LL;
                    /* pass */
                    while ((vfi < v->fields->len)) {
                        /* pass */
                        if ((vfi > 0LL)) {
                            /* pass */
                            TrStr _strtmp_t366 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                            _tr_str_release(s);
                            s = _strtmp_t366;
                        }
                        /* pass */
                        Param* vf = ((Param*)List_ptr_get(v->fields, vfi));
                        /* pass */
                        if ((((unsigned long long)(vf->ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            TrStr _strtmp_t367 = ({ TrStr _cr = (Sema_io_ty_str(self, (*vf->ty))); TrStr _cres = _tr_strx_concatv((s), _cr); _tr_str_release(_cr); _cres; });
                            _tr_str_release(s);
                            s = _strtmp_t367;
                        }
                        /* pass */
                        vfi = (vfi + 1LL);
                    }
                    /* pass */
                    TrStr _strtmp_t368 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
                    _tr_str_release(s);
                    s = _strtmp_t368;
                }
                /* pass */
                TrStr _strtmp_t369 = _tr_strx_concatv((s), (_tr_str_lit_len("\n", 1LL)));
                _tr_str_release(s);
                s = _strtmp_t369;
                /* pass */
                vi = (vi + 1LL);
            }
        }
        /* pass */
        if ((e->methods->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t370 = _tr_strx_concatv((s), (_tr_str_lit_len("  methods:\n", 11LL)));
            _tr_str_release(s);
            s = _strtmp_t370;
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < e->methods->len)) {
                /* pass */
                TrStr _strtmp_t371 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(e->methods, mi)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t371;
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        return s;
    } else if (_tr_dict_contains(self->interfaces, _tr_strz(name))) {
        /* pass */
        InterfaceDef* iface = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(name)));
        /* pass */
        TrStr s = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("interface ", 10LL)), (iface->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_cl); _cres; });
        /* pass */
        if ((iface->methods->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t372 = _tr_strx_concatv((s), (_tr_str_lit_len("  methods:\n", 11LL)));
            _tr_str_release(s);
            s = _strtmp_t372;
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < iface->methods->len)) {
                /* pass */
                TrStr _strtmp_t373 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(iface->methods, mi)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t373;
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        return s;
    } else if (_tr_dict_contains(self->fn_defs, _tr_strz(name))) {
        /* pass */
        return Sema_io_func_sig(self, ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(name))));
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("int", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("int: 64-bit signed integer (C long long).", 41LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("float", 5LL)))) {
        /* pass */
        return _tr_str_lit_len("float: 64-bit floating point number (C double).", 47LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("bool: true/false (C _Bool).", 27LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("str: immutable byte string (C char*).", 37LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("char", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("char: single byte character (C char).", 37LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("List", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("List[T]: growable, ordered, indexable sequence of T.", 52LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Dict", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("Dict[K, V]: hash map from K to V.", 33LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Set", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("Set[T]: unordered collection of unique T values.", 48LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Option", 6LL)))) {
        /* pass */
        return _tr_str_lit_len("Option[T]: either Some(T) or None.", 34LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Result", 6LL)))) {
        /* pass */
        return _tr_str_lit_len("Result[T, E]: either Ok(T) or Err(E).", 37LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Pointer", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("Pointer[T]: raw pointer to a T (C T*); use unsafe: for arithmetic.", 66LL);
    } else {
        /* pass */
        return ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("No info available for '", 23LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_cl); _cres; });
    }
}

__attribute__((hot)) void Sema_error(Sema* self, TrStr msg) {
    /* pass */
    TrStr loc = _tr_str_lit_len("", 0LL);
    /* pass */
    if ((_tr_str_lenv((self->current_file)) > 0LL)) {
        /* pass */
        TrStr _strtmp_t374 = _tr_strx_concatv((self->current_file), (_tr_str_lit_len(":", 1LL)));
        _tr_str_release(loc);
        loc = _strtmp_t374;
    }
    /* pass */
    if ((self->current_line > 0LL)) {
        /* pass */
        ({ TrStr _at_t375 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->current_line)))); TrStr _cres = _tr_strx_concatv((loc), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (msg)); _tr_str_release(_cl); _cres; })); List_TrStr_append(self->errors, _at_t375); _tr_str_release(_at_t375); });
    } else {
        /* pass */
        List_TrStr_append(self->errors, msg);
    }
    _tr_str_release(loc);
}

__attribute__((hot)) bool Sema_is_sendable_type(Sema* self, TrStr ty_name) {
    /* pass */
    if (((((_tr_str_eqv((ty_name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("str", 3LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((ty_name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("auto", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("isize", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((ty_name), (_tr_str_lit_len("Atomic", 6LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("RwLock", 6LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((ty_name), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("ThreadPool", 10LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((ty_name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((ty_name), (_tr_str_lit_len("AsyncPool", 9LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("AsyncTask", 9LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((ty_name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Weak", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((ty_name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Map", 3LL))))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(ty_name))) {
        /* pass */
        ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ty_name)));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(cls->iface_names))))) {
            /* pass */
            long long ii = 0LL;
            /* pass */
            while ((ii < cls->iface_names->len)) {
                /* pass */
                if (_tr_str_eqv((List_TrStr_get(cls->iface_names, ii)), (_tr_str_lit_len("Sendable", 8LL)))) {
                    /* pass */
                    return true;
                }
                /* pass */
                ii = (ii + 1LL);
            }
        }
        /* pass */
        return false;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_sendable_ty(Sema* self, AstType* ty) {
    /* pass */
    TrStr n = _tr_str_retain(ty->name);
    /* pass */
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL))))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            _tr_str_release(n);
            return Sema_is_sendable_ty(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
        }
        /* pass */
        _tr_str_release(n);
        return true;
    }
    /* pass */
    return Sema_is_sendable_type(self, n);
}

__attribute__((hot)) bool Sema_class_method_exists(Sema* self, TrStr cls_name, TrStr method) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
        /* pass */
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < cls->methods->len)) {
        /* pass */
        if (_tr_str_eqv((((FunctionDef*)List_ptr_get(cls->methods, mi))->name), (method))) {
            /* pass */
            return true;
        }
        /* pass */
        mi = (mi + 1LL);
    }
    /* pass */
    long long bi = 0LL;
    /* pass */
    while ((bi < cls->base_classes->len)) {
        /* pass */
        if (({ TrStr _at_t376 = (List_TrStr_get(cls->base_classes, bi)); __auto_type _wr = (Sema_class_method_exists(self, _at_t376, method)); _tr_str_release(_at_t376); _wr; })) {
            /* pass */
            return true;
        }
        /* pass */
        bi = (bi + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_universal_method(Sema* self, TrStr method) {
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("free", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("as_str", 6LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__len__", 7LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("__getitem__", 11LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__setitem__", 11LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("__eq__", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__ne__", 6LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("__lt__", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__gt__", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__le__", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__ge__", 6LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((method), (_tr_str_lit_len("__add__", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__sub__", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__mul__", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__div__", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__mod__", 7LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("__hash__", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__iter__", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__next__", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__contains__", 12LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("__str__", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__repr__", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__enter__", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__exit__", 8LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_expr_is_borrow(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return false;
    }
    /* pass */
    if (hir_expr_type(e)->is_borrow) {
        /* pass */
        return true;
    }
    /* pass */
    __auto_type _t377 = (*e);
    if (_t377.tag == HirExpr_EIdent) {
        __auto_type nm = _t377.data.EIdent.name;
        /* pass */
        Symbol* sym = Sema_resolve(self, nm);
        /* pass */
        if (((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL))))) {
            /* pass */
            if ((*sym->ty)->is_borrow) {
                /* pass */
                return true;
            }
        }
    } else if (1) {
        __auto_type _ = _t377;
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__expr_is_shared(Sema* self, HirExpr* e) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t378 = (*e);
    if (_t378.tag == HirExpr_EIdent) {
        __auto_type nm = _t378.data.EIdent.name;
        /* pass */
        Symbol* sym = Sema_resolve(self, nm);
        /* pass */
        return ((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && sym->is_shared);
    } else if (_t378.tag == HirExpr_EMethodCall) {
        __auto_type _tr_v_recv = _t378.data.EMethodCall.obj;
__auto_type meth = _t378.data.EMethodCall.method;
        /* pass */
        if (_tr_str_eqv((meth), (_tr_str_lit_len("clone", 5LL)))) {
            /* pass */
            return Sema__expr_is_shared(self, _tr_v_recv);
        }
    } else if (1) {
        __auto_type _ = _t378;
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema_check_spawn_sendable(Sema* self, HirExpr* e) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t379 = (*e);
    if (_t379.tag == HirExpr_ECall) {
        __auto_type cs_args = _t379.data.ECall.args;
        /* pass */
        long long csi = 0LL;
        /* pass */
        while ((csi < cs_args->len)) {
            /* pass */
            if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(cs_args, csi)))) {
                /* pass */
                Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot cross a thread boundary: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 252LL));
            }
            /* pass */
            AstType* arg_ty = hir_expr_type(((HirExpr*)List_ptr_get(cs_args, csi)));
            /* pass */
            if (((_tr_str_eqv((arg_ty->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((arg_ty->name), (_tr_str_lit_len("Weak", 4LL)))) && (arg_ty->args->len > 0LL))) {
                /* pass */
                TrStr inner_nm = _tr_str_retain((*((AstType**)List_ptr_get(arg_ty->args, 0LL)))->name);
                /* pass */
                if ((!Sema_is_sendable_type(self, inner_nm))) {
                    /* pass */
                    ({ TrStr _at_t380 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] 'Shared[", 14LL)), (inner_nm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' cannot safely cross thread boundaries because '", 50LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (inner_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not Sendable.\n      FIX: Add 'implements Sendable' to '", 60LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (inner_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' and ensure all mutable fields use Atomic[T] or Mutex[T].", 58LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t380); _tr_str_release(_at_t380); });
                }
            } else if ((((self->strict_mode && Sema__is_rc_class(self, arg_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(cs_args, csi))))) && (!Sema__is_unsafe_sendable(self, arg_ty->name)))) {
                /* pass */
                ({ TrStr _at_t381 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (arg_ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot cross a thread boundary (this is exactly why Rust's 'Rc' is '!Send').\n      FIX: share it as 'Shared[", 216LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arg_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc'), or 'Mutex[", 51LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arg_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' for exclusive access.", 24LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t381); _tr_str_release(_at_t381); });
            } else if ((!Sema_is_sendable_type(self, arg_ty->name))) {
                /* pass */
                ({ TrStr _at_t382 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (arg_ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex[", 87LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arg_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("] for exclusive access, or Atomic[T] for counters/flags.\n      Or add 'implements Sendable' to '", 96LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arg_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t382); _tr_str_release(_at_t382); });
            }
            /* pass */
            Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(cs_args, csi)));
            /* pass */
            csi = (csi + 1LL);
        }
    } else if (_t379.tag == HirExpr_EMethodCall) {
        __auto_type cs_margs = _t379.data.EMethodCall.args;
        /* pass */
        long long csmi = 0LL;
        /* pass */
        while ((csmi < cs_margs->len)) {
            /* pass */
            if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(cs_margs, csmi)))) {
                /* pass */
                Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot cross a thread boundary: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 252LL));
            }
            /* pass */
            AstType* arg_ty2 = hir_expr_type(((HirExpr*)List_ptr_get(cs_margs, csmi)));
            /* pass */
            if (((_tr_str_eqv((arg_ty2->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((arg_ty2->name), (_tr_str_lit_len("Weak", 4LL)))) && (arg_ty2->args->len > 0LL))) {
                /* pass */
                TrStr inner_nm2 = _tr_str_retain((*((AstType**)List_ptr_get(arg_ty2->args, 0LL)))->name);
                /* pass */
                if ((!Sema_is_sendable_type(self, inner_nm2))) {
                    /* pass */
                    ({ TrStr _at_t383 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] 'Shared[", 14LL)), (inner_nm2))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' cannot safely cross thread boundaries because '", 50LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (inner_nm2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not Sendable.\n      FIX: Add 'implements Sendable' to '", 60LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (inner_nm2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' and protect mutable fields with Atomic[T] or Mutex[T].", 56LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t383); _tr_str_release(_at_t383); });
                }
            } else if ((((self->strict_mode && Sema__is_rc_class(self, arg_ty2->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(cs_margs, csmi))))) && (!Sema__is_unsafe_sendable(self, arg_ty2->name)))) {
                /* pass */
                ({ TrStr _at_t384 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (arg_ty2->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot cross a thread boundary.\n      FIX: share it as 'Shared[", 171LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arg_ty2->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' (atomic refcount) or 'Mutex[", 31LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arg_ty2->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' for exclusive access.", 24LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t384); _tr_str_release(_at_t384); });
            } else if ((!Sema_is_sendable_type(self, arg_ty2->name))) {
                /* pass */
                ({ TrStr _at_t385 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (arg_ty2->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex[", 87LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arg_ty2->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("].", 2LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t385); _tr_str_release(_at_t385); });
            }
            /* pass */
            Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(cs_margs, csmi)));
            /* pass */
            csmi = (csmi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t379;
        /* pass */
    }
}

__attribute__((hot)) void Sema_check_class_sendable_fields(Sema* self, ClassDef* c) {
    /* pass */
    bool has_unsafe = false;
    /* pass */
    long long ui = 0LL;
    /* pass */
    while ((ui < c->iface_names->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(c->iface_names, ui)), (_tr_str_lit_len("UnsafeSendable", 14LL)))) {
            /* pass */
            has_unsafe = true;
        }
        /* pass */
        ui = (ui + 1LL);
    }
    /* pass */
    long long cfi = 0LL;
    /* pass */
    while ((cfi < c->fields->len)) {
        /* pass */
        FieldDef* fd = ((FieldDef*)List_ptr_get(c->fields, cfi));
        /* pass */
        if ((((unsigned long long)(fd->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            AstType* fty = (*fd->ty);
            /* pass */
            if (_tr_str_eqv((fty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
                /* pass */
                if ((!has_unsafe)) {
                    /* pass */
                    ({ TrStr _at_t386 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-2] Class '", 13LL)), (c->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' declares 'implements Sendable' but holds a raw 'Pointer' field '", 66LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fd->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("', which the compiler cannot prove thread-safe.\n      FIX: If '", 63LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (c->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' wraps an internally-synchronized handle, assert it explicitly: 'implements Sendable, UnsafeSendable'.\n      Otherwise wrap the data in Mutex[T]/Atomic[T], or drop 'implements Sendable'.", 187LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t386); _tr_str_release(_at_t386); });
                }
            } else if ((!Sema_is_sendable_ty(self, fty))) {
                /* pass */
                TrStr _t2_inner = _tr_str_retain(fty->name);
                /* pass */
                if (((((_tr_str_eqv((fty->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("Channel", 7LL)))) && (fty->args->len > 0LL))) {
                    /* pass */
                    TrStr _strtmp_t387 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((fty->name), (_tr_str_lit_len("[", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, ((*((AstType**)List_ptr_get(fty->args, 0LL)))->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(_t2_inner);
                    _t2_inner = _strtmp_t387;
                }
                /* pass */
                ({ TrStr _at_t388 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-2] Class '", 13LL)), (c->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' declares 'implements Sendable' but field '", 44LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fd->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_t2_inner)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not Sendable (non-thread-safe data is reachable across threads).\n      FIX: make the inner type Sendable, wrap it in Mutex[T]/RwLock[T] for guarded access or Atomic[T] for numerics, or drop 'implements Sendable'.", 217LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t388); _tr_str_release(_at_t388); });
                _tr_str_release(_t2_inner);
            } else if (((_tr_str_eqv((fty->name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("bool", 4LL))))) {
                /* pass */
                ({ TrStr _at_t389 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-3] Sendable class '", 22LL)), (c->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' has primitive field '", 23LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fd->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' that may cause data races if mutated from multiple threads.\n      FIX: Use 'Atomic[", 85LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' for safe concurrent mutation, or ensure this field is written only before the object is shared across threads.", 113LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(self->warnings, _at_t389); _tr_str_release(_at_t389); });
            }
        }
        /* pass */
        cfi = (cfi + 1LL);
    }
}

__attribute__((hot)) HirExpr* Sema__patch_empty_dict_hint(Sema* self, HirExpr* hv, AstType* hint_ty) {
    /* pass */
    if ((((unsigned long long)(hv)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return hv;
    }
    /* pass */
    if (((!_tr_str_eqv((hint_ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (!_tr_str_eqv((hint_ty->name), (_tr_str_lit_len("Map", 3LL)))))) {
        /* pass */
        return hv;
    }
    /* pass */
    if ((hint_ty->args->len < 2LL)) {
        /* pass */
        return hv;
    }
    /* pass */
    __auto_type _t390 = (*hv);
    if (_t390.tag == HirExpr_EDict) {
        __auto_type hk = _t390.data.EDict.keys;
__auto_type hvv = _t390.data.EDict.vals;
        /* pass */
        if ((hk->len == 0LL)) {
            /* pass */
            return box_hirexpr(HirExpr_ctor_EDict(hk, hvv, hint_ty));
        }
        /* pass */
        return hv;
    } else if (1) {
        __auto_type _ = _t390;
        return hv;
    }
}

__attribute__((hot)) void Sema_mark_moved(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            if ((!sym->is_moved)) {
                /* pass */
                sym->is_moved = true;
                /* pass */
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
                /* pass */
                if (self->capturing_moves) {
                    /* pass */
                    List_TrStr_append(self->branch_moved_buf, name);
                }
            }
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        if ((!sym->is_moved)) {
            /* pass */
            sym->is_moved = true;
            /* pass */
            _tr_dict_set(self->globals, _tr_strz(name), sym);
            /* pass */
            if (self->capturing_moves) {
                /* pass */
                List_TrStr_append(self->branch_moved_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_mark_freed(Sema* self, TrStr name) {
    /* pass */
    long long mf_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mf_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name)));
            /* pass */
            mf_sym->is_freed = true;
            /* pass */
            mf_sym->is_moved = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name), mf_sym);
            /* pass */
            if (self->capturing_moves) {
                /* pass */
                List_TrStr_append(self->branch_moved_buf, name);
            }
            /* pass */
            return;
        }
        /* pass */
        mf_i = (mf_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        mf_sym->is_freed = true;
        /* pass */
        mf_sym->is_moved = true;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), mf_sym);
        /* pass */
        if (self->capturing_moves) {
            /* pass */
            List_TrStr_append(self->branch_moved_buf, name);
        }
    }
}

__attribute__((hot)) void Sema_check_not_moved(Sema* self, TrStr name, TrStr ty_name) {
    /* pass */
    if (Sema_is_primitive_name(self, ty_name)) {
        /* pass */
        return;
    }
    /* pass */
    Symbol* sym = Sema_resolve(self, name);
    /* pass */
    if (((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && sym->is_moved)) {
        /* pass */
        ({ TrStr _at_t391 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving.", 123LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t391); _tr_str_release(_at_t391); });
    }
}

__attribute__((hot)) void Sema_mark_borrow(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->active_borrows = (sym->active_borrows + 1LL);
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        sym->active_borrows = (sym->active_borrows + 1LL);
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_unmark_borrow(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            if ((sym->active_borrows > 0LL)) {
                /* pass */
                sym->active_borrows = (sym->active_borrows - 1LL);
            }
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        if ((sym->active_borrows > 0LL)) {
            /* pass */
            sym->active_borrows = (sym->active_borrows - 1LL);
        }
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_check_no_active_borrows(Sema* self, TrStr name, TrStr ty_name) {
    /* pass */
    if (Sema_is_primitive_name(self, ty_name)) {
        /* pass */
        return;
    }
    /* pass */
    Symbol* sym = Sema_resolve(self, name);
    /* pass */
    if (((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && (sym->active_borrows > 0LL))) {
        /* pass */
        ({ TrStr _at_t392 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[M-2] Cannot move '", 19LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' while it is borrowed.\n      FIX: The borrow must end before '", 63LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' can be moved.", 15LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t392); _tr_str_release(_at_t392); });
    }
}

__attribute__((hot)) void Sema_mark_init(Sema* self, TrStr name) {
    /* pass */
    long long mi_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mi_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
            /* pass */
            if ((!mi_sym->is_init)) {
                /* pass */
                mi_sym->is_init = true;
                /* pass */
                mi_sym->is_maybe_init = false;
                /* pass */
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
                /* pass */
                if (self->capturing_inits) {
                    /* pass */
                    List_TrStr_append(self->branch_init_buf, name);
                }
            }
            /* pass */
            return;
        }
        /* pass */
        mi_i = (mi_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        if ((!mi_sym->is_init)) {
            /* pass */
            mi_sym->is_init = true;
            /* pass */
            mi_sym->is_maybe_init = false;
            /* pass */
            _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
            /* pass */
            if (self->capturing_inits) {
                /* pass */
                List_TrStr_append(self->branch_init_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_clear_container_borrow(Sema* self, TrStr var_name) {
    /* pass */
    if (_tr_dict_contains(self->container_borrows, _tr_strz(var_name))) {
        /* pass */
        _tr_dict_remove(self->container_borrows, _tr_strz(var_name));
    }
}

__attribute__((hot)) void Sema_unmark_moved(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->is_moved = false;
            /* pass */
            sym->is_freed = false;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        sym->is_moved = false;
        /* pass */
        sym->is_freed = false;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_moved(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->is_maybe_moved = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        sym->is_maybe_moved = true;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_unmark_init(Sema* self, TrStr name) {
    /* pass */
    long long mi_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mi_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
            /* pass */
            mi_sym->is_init = false;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
            /* pass */
            return;
        }
        /* pass */
        mi_i = (mi_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        mi_sym->is_init = false;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_init(Sema* self, TrStr name) {
    /* pass */
    long long mi_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mi_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
            /* pass */
            mi_sym->is_maybe_init = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
            /* pass */
            return;
        }
        /* pass */
        mi_i = (mi_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        mi_sym->is_maybe_init = true;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
    }
}

__attribute__((hot)) bool Sema_vec_str_contains(Sema* self, List_TrStr* v, TrStr s) {
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < v->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(v, vi)), (s))) {
            /* pass */
            return true;
        }
        /* pass */
        vi = (vi + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_has_copy_decorator(Sema* self, List_ptr* decs) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < decs->len)) {
        /* pass */
        if (_tr_str_eqv((((Decorator*)List_ptr_get(decs, i))->name), (_tr_str_lit_len("copy", 4LL)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_copy_class(Sema* self, TrStr name) {
    /* pass */
    if (Sema_is_primitive_name(self, name)) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->copy_classes, _tr_strz(name))) {
        /* pass */
        return ((bool)(uintptr_t)_tr_dict_get(self->copy_classes, _tr_strz(name)));
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(name)))) {
        /* pass */
        return false;
    }
    /* pass */
    _tr_dict_set(self->copy_classes, _tr_strz(name), false);
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
    /* pass */
    if ((cls->fields->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    bool all_copy = true;
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < cls->fields->len)) {
        /* pass */
        FieldDef* fd = ((FieldDef*)List_ptr_get(cls->fields, fi));
        /* pass */
        if ((((unsigned long long)(fd->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            AstType* fty = (*fd->ty);
            /* pass */
            if ((!fty->is_borrow)) {
                /* pass */
                if ((!Sema_is_copy_class(self, fty->name))) {
                    /* pass */
                    all_copy = false;
                }
            }
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    _tr_dict_set(self->copy_classes, _tr_strz(name), all_copy);
    /* pass */
    return all_copy;
}

__attribute__((hot)) void Sema_enter_scope(Sema* self) {
    /* pass */
    Scope* s_obj = Scope_init();
    /* pass */
    List_ptr_append(self->scopes, _tr_obj_retain(s_obj));
    /* pass */
    self->current_scope_depth = (self->current_scope_depth + 1LL);
    /* pass */
    List_i64_append(self->block_depth_stack, self->block_depth);
    /* pass */
    self->block_depth = 0LL;
    /* pass */
    List_i64_append(self->block_stack_base, self->block_stack->len);
    _tr_obj_release(s_obj, _trdrop_Scope);
}

__attribute__((hot)) void Sema_exit_scope(Sema* self) {
    /* pass */
    ((Scope*)List_ptr_pop(self->scopes));
    /* pass */
    self->current_scope_depth = (self->current_scope_depth - 1LL);
    /* pass */
    self->block_depth = List_i64_get(self->block_depth_stack, (self->block_depth_stack->len - 1LL));
    /* pass */
    List_i64_pop(self->block_depth_stack);
    /* pass */
    long long base = List_i64_get(self->block_stack_base, (self->block_stack_base->len - 1LL));
    /* pass */
    while ((self->block_stack->len > base)) {
        /* pass */
        List_i64_pop(self->block_stack);
    }
    /* pass */
    List_i64_pop(self->block_stack_base);
}

__attribute__((hot)) bool Sema__coll_elem_droppable(Sema* self, TrStr n) {
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
        /* pass */
        if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)))->is_class)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL))))) {
            /* pass */
            return false;
        }
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema__scan_ptr_aliased_ty(Sema* self, AstType* t) {
    /* pass */
    if ((_tr_str_eqv((t->name), (_tr_str_lit_len("Pointer", 7LL))) && (t->args->len > 0LL))) {
        /* pass */
        AstType* inner = (*((AstType**)List_ptr_get(t->args, 0LL)));
        /* pass */
        _tr_dict_set(self->ptr_aliased, _tr_strz(inner->name), true);
        /* pass */
        Sema__scan_ptr_aliased_ty(self, inner);
        /* pass */
        return;
    }
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < t->args->len)) {
        /* pass */
        Sema__scan_ptr_aliased_ty(self, (*((AstType**)List_ptr_get(t->args, ai))));
        /* pass */
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) bool Sema__is_rc_class(Sema* self, TrStr n) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(n)))) {
        /* pass */
        return false;
    }
    /* pass */
    if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)))->is_class)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Box", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL))))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_tr_dict_contains(self->ptr_aliased, _tr_strz(n))) {
        /* pass */
        return false;
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool Sema__ty_reaches_plain_rc(Sema* self, AstType* ft, long long depth) {
    /* pass */
    if ((depth > 24LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr n = _tr_str_retain(ft->name);
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL))))) {
        /* pass */
        _tr_str_release(n);
        return false;
    }
    /* pass */
    if ((((((((_tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Box", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL))))) {
        /* pass */
        if ((ft->args->len > 0LL)) {
            /* pass */
            _tr_str_release(n);
            return Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(ft->args, 0LL))), (depth + 1LL));
        }
        /* pass */
        _tr_str_release(n);
        return false;
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
        /* pass */
        if ((ft->args->len > 1LL)) {
            /* pass */
            _tr_str_release(n);
            return Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(ft->args, 1LL))), (depth + 1LL));
        }
        /* pass */
        _tr_str_release(n);
        return false;
    }
    /* pass */
    if (Sema__is_rc_class(self, n)) {
        /* pass */
        _tr_str_release(n);
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
        /* pass */
        return Sema__sendable_reaches_plain_rc(self, n, (depth + 1LL));
    }
    /* pass */
    _tr_str_release(n);
    return false;
}

__attribute__((hot)) bool Sema__sendable_reaches_plain_rc(Sema* self, TrStr tn, long long depth) {
    /* pass */
    if ((depth > 24LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
        /* pass */
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(tn)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < cls->fields->len)) {
        /* pass */
        FieldDef* f = ((FieldDef*)List_ptr_get(cls->fields, i));
        /* pass */
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if (Sema__ty_reaches_plain_rc(self, (*f->ty), (depth + 1LL))) {
                /* pass */
                return true;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__is_unsafe_sendable(Sema* self, TrStr n) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(n)))) {
        /* pass */
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)));
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(cls->iface_names)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < cls->iface_names->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(cls->iface_names, i)), (_tr_str_lit_len("UnsafeSendable", 14LL)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema__check_spawn_nested_rc(Sema* self, HirExpr* arg_expr) {
    /* pass */
    if ((!self->strict_mode)) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(arg_expr)))) {
        /* pass */
        return;
    }
    /* pass */
    AstType* at = hir_expr_type(arg_expr);
    /* pass */
    TrStr n = _tr_str_retain(at->name);
    /* pass */
    TrStr _cross_cls = _tr_str_retain(n);
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) && (at->args->len > 0LL))) {
        /* pass */
        TrStr _strtmp_t393 = _tr_str_retain((*((AstType**)List_ptr_get(at->args, 0LL)))->name);
        _tr_str_release(_cross_cls);
        _cross_cls = _strtmp_t393;
    }
    /* pass */
    if (Sema__is_unsafe_sendable(self, _cross_cls)) {
        /* pass */
        _tr_str_release(n);
        _tr_str_release(_cross_cls);
        return;
    }
    /* pass */
    bool reaches = false;
    /* pass */
    if (Sema__expr_is_shared(self, arg_expr)) {
        /* pass */
        TrStr inner = _tr_str_retain(n);
        /* pass */
        if (((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) && (at->args->len > 0LL))) {
            /* pass */
            TrStr _strtmp_t394 = _tr_str_retain((*((AstType**)List_ptr_get(at->args, 0LL)))->name);
            _tr_str_release(inner);
            inner = _strtmp_t394;
        }
        /* pass */
        reaches = Sema__sendable_reaches_plain_rc(self, inner, 0LL);
        _tr_str_release(inner);
    } else if (((((((((_tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Box", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) && (at->args->len > 0LL))) {
        /* pass */
        reaches = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(at->args, 0LL))), 0LL);
    } else if (((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) && (at->args->len > 1LL))) {
        /* pass */
        reaches = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(at->args, 1LL))), 0LL);
    }
    /* pass */
    if (reaches) {
        /* pass */
        ({ TrStr _at_t395 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] a value crossing the thread boundary transitively holds a plain (non-atomic) reference-counted class inside '", 115LL)), (n))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' (e.g. via a Mutex/Vec/field). Even wrapped in Shared/Sendable, that nested class's refcount is thread-local and would race across threads.\n      FIX: make the nested state 'Shared[T]' (atomic, like Rust's 'Arc'), or restructure so only one thread ever owns it.", 262LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t395); _tr_str_release(_at_t395); });
    }
    _tr_str_release(n);
    _tr_str_release(_cross_cls);
}

__attribute__((hot)) void Sema__collect_strong_edge(Sema* self, AstType* ft, List_TrStr* out) {
    /* pass */
    TrStr n = _tr_str_retain(ft->name);
    /* pass */
    if (ft->is_borrow) {
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL))))) {
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL))))) {
        /* pass */
        if ((ft->args->len > 0LL)) {
            /* pass */
            Sema__collect_strong_edge(self, (*((AstType**)List_ptr_get(ft->args, 0LL))), out);
        }
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
        /* pass */
        if ((ft->args->len > 1LL)) {
            /* pass */
            Sema__collect_strong_edge(self, (*((AstType**)List_ptr_get(ft->args, 1LL))), out);
        }
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) {
        /* pass */
        if ((ft->args->len > 0LL)) {
            /* pass */
            TrStr si = _tr_str_retain((*((AstType**)List_ptr_get(ft->args, 0LL)))->name);
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(si))) {
                /* pass */
                List_TrStr_append(out, si);
            }
        }
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if (Sema__is_rc_class(self, n)) {
        /* pass */
        List_TrStr_append(out, n);
    }
    _tr_str_release(n);
}

__attribute__((hot)) List_TrStr* Sema__strong_owned(Sema* self, ClassDef* cd) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < cd->fields->len)) {
        /* pass */
        FieldDef* f = ((FieldDef*)List_ptr_get(cd->fields, i));
        /* pass */
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema__collect_strong_edge(self, (*f->ty), out);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return out;
}

__attribute__((hot)) void Sema__cycle_dfs(Sema* self, TrStr cur, TrStr start, TrMap* on_path, TrMap* reported, long long depth) {
    /* pass */
    if ((depth > 200LL)) {
        /* pass */
        return;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cur)))) {
        /* pass */
        return;
    }
    /* pass */
    List_TrStr* edges = Sema__strong_owned(self, ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cur))));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < edges->len)) {
        /* pass */
        TrStr nxt = List_TrStr_get(edges, i);
        /* pass */
        if (_tr_str_eqv((nxt), (start))) {
            /* pass */
            if ((!_tr_dict_contains(reported, _tr_strz(start)))) {
                /* pass */
                _tr_dict_set(reported, _tr_strz(start), true);
                /* pass */
                ({ TrStr _at_t396 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[S-2] class '", 13LL)), (start))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is part of a strong reference CYCLE (via '", 44LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' -> '", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (nxt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'), which reference counting cannot reclaim — it would leak.\n      FIX: make one back-edge non-owning with 'Weak[", 115LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (start)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' (a weak reference) or 'Pointer[", 34LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (start)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' (raw, manual).", 17LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t396); _tr_str_release(_at_t396); });
            }
        } else if ((!_tr_dict_contains(on_path, _tr_strz(nxt)))) {
            /* pass */
            _tr_dict_set(on_path, _tr_strz(nxt), true);
            /* pass */
            Sema__cycle_dfs(self, nxt, start, on_path, reported, (depth + 1LL));
            /* pass */
            _tr_dict_set(on_path, _tr_strz(nxt), false);
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(nxt);
    }
    List_TrStr_free(edges);
}

__attribute__((hot)) void Sema_check_ownership_cycles(Sema* self, Program* prog) {
    /* pass */
    long long di = 0LL;
    /* pass */
    while ((di < prog->decls->len)) {
        /* pass */
        __auto_type _t397 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t397.tag == Decl_DClass) {
            __auto_type c = _t397.data.DClass.cls;
            Sema__scan_class_ptrs(self, c);
        } else if (_t397.tag == Decl_DActor) {
            __auto_type c = _t397.data.DActor.cls;
            Sema__scan_class_ptrs(self, c);
        } else if (_t397.tag == Decl_DExtend) {
            __auto_type ms = _t397.data.DExtend.methods;
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < ms->len)) {
                /* pass */
                Sema__scan_fn_ptrs(self, ((FunctionDef*)List_ptr_get(ms, mi)));
                /* pass */
                mi = (mi + 1LL);
            }
        } else if (_t397.tag == Decl_DFunction) {
            __auto_type f = _t397.data.DFunction.func;
            Sema__scan_fn_ptrs(self, f);
        } else if (1) {
            __auto_type _ = _t397;
            /* pass */
        }
        /* pass */
        di = (di + 1LL);
    }
    /* pass */
    TrMap* reported = _tr_dict_new(16LL);
    /* pass */
    di = 0LL;
    /* pass */
    while ((di < prog->decls->len)) {
        /* pass */
        __auto_type _t398 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t398.tag == Decl_DClass) {
            __auto_type c = _t398.data.DClass.cls;
            /* pass */
            if (Sema__is_rc_class(self, c->name)) {
                /* pass */
                TrMap* on_path = _tr_dict_new(16LL);
                /* pass */
                _tr_dict_set(on_path, _tr_strz(c->name), true);
                /* pass */
                Sema__cycle_dfs(self, c->name, c->name, on_path, reported, 0LL);
            }
        } else if (1) {
            __auto_type _ = _t398;
            /* pass */
        }
        /* pass */
        di = (di + 1LL);
    }
}

__attribute__((hot)) void Sema__scan_class_ptrs(Sema* self, ClassDef* c) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c->fields->len)) {
        /* pass */
        FieldDef* f = ((FieldDef*)List_ptr_get(c->fields, i));
        /* pass */
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema__scan_ptr_aliased_ty(self, (*f->ty));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < c->methods->len)) {
        /* pass */
        Sema__scan_fn_ptrs(self, ((FunctionDef*)List_ptr_get(c->methods, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema__scan_fn_ptrs(Sema* self, FunctionDef* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        Param* p = ((Param*)List_ptr_get(f->params, i));
        /* pass */
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema__scan_ptr_aliased_ty(self, (*p->ty));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        Sema__scan_ptr_aliased_ty(self, (*f->ret_ty));
    }
}

__attribute__((hot)) bool Sema_is_droppable_sym(Sema* self, Symbol* sym) {
    /* pass */
    if ((sym->kind.tag != SymbolKind_make_SVariable().tag)) {
        /* pass */
        return false;
    }
    /* pass */
    if (sym->is_param) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((unsigned long long)(sym->ty)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    if (sym->is_shared) {
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            return false;
        }
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_str_eqv(((*sym->ty)->name), (_tr_str_lit_len("Weak", 4LL)))) {
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            return false;
        }
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_str_eqv((sym->borrows_region), (_tr_str_lit_len("@borrowed", 9LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr tn = _tr_str_retain((*sym->ty)->name);
    /* pass */
    if (_tr_str_eqv((tn), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        if (sym->str_escaped) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if ((sym->active_borrows > 0LL)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        _tr_str_release(tn);
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
        /* pass */
        if (sym->coll_escaped) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if ((sym->active_borrows > 0LL)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        List_ptr* targs = (*sym->ty)->args;
        /* pass */
        if ((_tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL))))) {
            /* pass */
            if ((targs->len < 2LL)) {
                /* pass */
                _tr_str_release(tn);
                return false;
            }
            /* pass */
            TrStr kt = _tr_str_retain((*((AstType**)List_ptr_get(targs, 0LL)))->name);
            /* pass */
            TrStr vt = _tr_str_retain((*((AstType**)List_ptr_get(targs, 1LL)))->name);
            /* pass */
            if ((!Sema__coll_elem_droppable(self, kt))) {
                /* pass */
                _tr_str_release(tn);
                _tr_str_release(kt);
                _tr_str_release(vt);
                return false;
            }
            /* pass */
            if ((!Sema__coll_elem_droppable(self, vt))) {
                /* pass */
                _tr_str_release(tn);
                _tr_str_release(kt);
                _tr_str_release(vt);
                return false;
            }
            /* pass */
            _tr_str_release(tn);
            _tr_str_release(kt);
            _tr_str_release(vt);
            return true;
        } else {
            /* pass */
            if ((targs->len < 1LL)) {
                /* pass */
                _tr_str_release(tn);
                return false;
            }
            /* pass */
            TrStr et = _tr_str_retain((*((AstType**)List_ptr_get(targs, 0LL)))->name);
            /* pass */
            if ((!Sema__coll_elem_droppable(self, et))) {
                /* pass */
                _tr_str_release(tn);
                _tr_str_release(et);
                return false;
            }
            /* pass */
            _tr_str_release(tn);
            _tr_str_release(et);
            return true;
        }
    }
    /* pass */
    if (_tr_str_eqv((tn), (_tr_str_lit_len("Mutex", 5LL)))) {
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if ((sym->active_borrows > 0LL)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        _tr_str_release(tn);
        return true;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if (((((((((((_tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Chan", 4LL))))) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("StringBuilder", 13LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("StringObj", 9LL))))) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if ((sym->active_borrows > 0LL)) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if (((!sym->is_init) || sym->is_maybe_init)) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(tn)));
    /* pass */
    long long mi = 0LL;
    /* pass */
    bool has_dispose = false;
    /* pass */
    while ((mi < cls->methods->len)) {
        /* pass */
        TrStr mnm = _tr_str_retain(((FunctionDef*)List_ptr_get(cls->methods, mi))->name);
        /* pass */
        if (_tr_str_eqv((mnm), (_tr_str_lit_len("free", 4LL)))) {
            /* pass */
            if (sym->coll_escaped) {
                /* pass */
                _tr_str_release(tn);
                _tr_str_release(mnm);
                return false;
            }
            /* pass */
            _tr_str_release(tn);
            _tr_str_release(mnm);
            return true;
        }
        /* pass */
        if (_tr_str_eqv((mnm), (_tr_str_lit_len("dispose", 7LL)))) {
            /* pass */
            has_dispose = true;
        }
        /* pass */
        mi = (mi + 1LL);
        _tr_str_release(mnm);
    }
    /* pass */
    if (has_dispose) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    _tr_str_release(tn);
    return true;
}

__attribute__((hot)) void Sema_open_block(Sema* self) {
    /* pass */
    self->next_block_id = (self->next_block_id + 1LL);
    /* pass */
    List_i64_append(self->block_stack, self->next_block_id);
}

__attribute__((hot)) void Sema_close_block(Sema* self) {
    /* pass */
    List_i64_pop(self->block_stack);
}

__attribute__((hot)) bool Sema_block_stack_contains(Sema* self, long long id) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->block_stack->len)) {
        /* pass */
        if ((List_i64_get(self->block_stack, i) == id)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) List_ptr* Sema_compute_scope_drops(Sema* self, long long scope_idx, bool is_innermost) {
    /* pass */
    List_ptr* out = (void*)List_ptr_new();
    /* pass */
    if (((scope_idx < 0LL) || (scope_idx >= self->scopes->len))) {
        /* pass */
        return out;
    }
    /* pass */
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, scope_idx));
    /* pass */
    long long ki = 0LL;
    /* pass */
    while ((ki < scope->decl_order->len)) {
        /* pass */
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
        /* pass */
        if (_tr_dict_contains(scope->variables, _tr_strz(nm))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
            /* pass */
            if ((((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                bool visible = (sym->decl_block_id == 0LL);
                /* pass */
                if (((!visible) && is_innermost)) {
                    /* pass */
                    visible = Sema_block_stack_contains(self, sym->decl_block_id);
                }
                /* pass */
                if (visible) {
                    /* pass */
                    if (Sema_is_droppable_sym(self, sym)) {
                        /* pass */
                        List_ptr_append(out, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
                    }
                }
            }
        }
        /* pass */
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
    /* pass */
    return out;
}

__attribute__((hot)) void Sema_append_drops_from(Sema* self, HirBlock* hb, long long from_idx) {
    /* pass */
    Sema_append_drops_from_excl(self, hb, from_idx, _tr_str_lit_len("", 0LL));
}

__attribute__((hot)) void Sema_append_drops_from_excl(Sema* self, HirBlock* hb, long long from_idx, TrStr exclude) {
    /* pass */
    List_TrStr* excl_list = (void*)List_TrStr_new();
    /* pass */
    if ((!_tr_str_eqv((exclude), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        List_TrStr_append(excl_list, exclude);
    }
    /* pass */
    Sema_append_drops_from_excl_multi(self, hb, from_idx, excl_list);
}

__attribute__((hot)) void Sema_append_drops_from_excl_multi(Sema* self, HirBlock* hb, long long from_idx, List_TrStr* excludes) {
    /* pass */
    long long idx = (self->scopes->len - 1LL);
    /* pass */
    while ((idx >= from_idx)) {
        /* pass */
        bool is_innermost = (idx == (self->scopes->len - 1LL));
        /* pass */
        List_ptr* drops = Sema_compute_scope_drops(self, idx, is_innermost);
        /* pass */
        long long di = 0LL;
        /* pass */
        while ((di < drops->len)) {
            /* pass */
            HirStmt* d = ((HirStmt*)List_ptr_get(drops, di));
            /* pass */
            bool skip = false;
            /* pass */
            __auto_type _t399 = (*d);
            if (_t399.tag == HirStmt_SAutoDrop) {
                __auto_type dn = _t399.data.SAutoDrop.name;
                /* pass */
                long long ei = 0LL;
                /* pass */
                while ((ei < excludes->len)) {
                    /* pass */
                    if (_tr_str_eqv((List_TrStr_get(excludes, ei)), (dn))) {
                        /* pass */
                        skip = true;
                    }
                    /* pass */
                    ei = (ei + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t399;
                /* pass */
            }
            /* pass */
            if ((!skip)) {
                /* pass */
                HirBlock_push(hb, d);
            }
            /* pass */
            di = (di + 1LL);
        }
        /* pass */
        idx = (idx - 1LL);
    }
}

__attribute__((hot)) void Sema_collect_idents(Sema* self, HirExpr* e, List_TrStr* out) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t400 = (*e);
    if (_t400.tag == HirExpr_EIdent) {
        __auto_type nm = _t400.data.EIdent.name;
        List_TrStr_append(out, nm);
    } else if (_t400.tag == HirExpr_EBinOp) {
        __auto_type l = _t400.data.EBinOp.left;
__auto_type r = _t400.data.EBinOp.right;
        /* pass */
        Sema_collect_idents(self, l, out);
        /* pass */
        Sema_collect_idents(self, r, out);
    } else if (_t400.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t400.data.EUnaryOp.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t400.tag == HirExpr_ECall) {
        __auto_type callee = _t400.data.ECall.callee;
__auto_type args = _t400.data.ECall.args;
        /* pass */
        Sema_collect_idents(self, callee, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t400.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t400.data.EMethodCall.obj;
__auto_type args = _t400.data.EMethodCall.args;
        /* pass */
        Sema_collect_idents(self, obj, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t400.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t400.data.EPropAccess.obj;
        Sema_collect_idents(self, obj, out);
    } else if (_t400.tag == HirExpr_EIndex) {
        __auto_type obj = _t400.data.EIndex.obj;
__auto_type idx = _t400.data.EIndex._tr_v_index;
        /* pass */
        Sema_collect_idents(self, obj, out);
        /* pass */
        Sema_collect_idents(self, idx, out);
    } else if (_t400.tag == HirExpr_ECast) {
        __auto_type inner = _t400.data.ECast.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t400.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t400.data.ETryExpr.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t400.tag == HirExpr_EIfElse) {
        __auto_type c = _t400.data.EIfElse.cond;
__auto_type t = _t400.data.EIfElse.then_e;
__auto_type f = _t400.data.EIfElse.else_e;
        /* pass */
        Sema_collect_idents(self, c, out);
        /* pass */
        Sema_collect_idents(self, t, out);
        /* pass */
        Sema_collect_idents(self, f, out);
    } else if (_t400.tag == HirExpr_EList) {
        __auto_type items = _t400.data.EList.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t400.tag == HirExpr_ESet) {
        __auto_type items = _t400.data.ESet.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t400.tag == HirExpr_ETuple) {
        __auto_type items = _t400.data.ETuple.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t400.tag == HirExpr_EDict) {
        __auto_type keys = _t400.data.EDict.keys;
__auto_type vals = _t400.data.EDict.vals;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < keys->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(keys, i)), out);
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(vals, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t400.tag == HirExpr_ERange) {
        __auto_type s = _t400.data.ERange.start;
__auto_type en = _t400.data.ERange.end;
        /* pass */
        Sema_collect_idents(self, s, out);
        /* pass */
        Sema_collect_idents(self, en, out);
    } else if (_t400.tag == HirExpr_EAwait) {
        __auto_type inner = _t400.data.EAwait.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t400.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t400.data.EAwaitTimeout.expr;
__auto_type to = _t400.data.EAwaitTimeout.timeout_ms;
        /* pass */
        Sema_collect_idents(self, inner, out);
        /* pass */
        Sema_collect_idents(self, to, out);
    } else if (_t400.tag == HirExpr_EYield) {
        __auto_type inner = _t400.data.EYield.expr;
        Sema_collect_idents(self, inner, out);
    } else if (1) {
        __auto_type _ = _t400;
        /* pass */
    }
}

__attribute__((hot)) bool Sema_is_local_var(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema_mark_str_escaped(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->str_escaped = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
}

__attribute__((hot)) void Sema_set_borrows_region(Sema* self, TrStr name, TrStr region) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->borrows_region = _tr_str_retain(region);
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
}

__attribute__((hot)) TrStr Sema_compute_region(Sema* self, Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    __auto_type _t401 = (*e);
    if (_t401.tag == Expr_EBinOp) {
        __auto_type cr_op = _t401.data.EBinOp.op;
        /* pass */
        if (_tr_str_eqv((cr_op), (_tr_str_lit_len("+", 1LL)))) {
            /* pass */
            return _tr_str_lit_len("@owned", 6LL);
        }
        /* pass */
        return _tr_str_lit_len("", 0LL);
    } else if (_t401.tag == Expr_EIdent) {
        __auto_type cr_nm = _t401.data.EIdent.name;
        /* pass */
        Symbol* cr_sym = Sema_resolve(self, cr_nm);
        /* pass */
        if (_tr_str_eqv((cr_sym->name), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            return _tr_str_lit_len("", 0LL);
        }
        /* pass */
        if (cr_sym->is_param) {
            /* pass */
            return _tr_str_retain(cr_nm);
        }
        /* pass */
        return _tr_str_retain(cr_sym->borrows_region);
    } else if (_t401.tag == Expr_ECall) {
        __auto_type cr_callee = _t401.data.ECall.callee;
__auto_type cr_args = _t401.data.ECall.args;
        /* pass */
        if ((((unsigned long long)(cr_callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t402 = (*cr_callee);
            if (_t402.tag == Expr_EIdent) {
                __auto_type cr_fn = _t402.data.EIdent.name;
                /* pass */
                if (_tr_dict_contains(self->classes, _tr_strz(cr_fn))) {
                    /* pass */
                    return _tr_str_lit_len("@owned", 6LL);
                }
                /* pass */
                Symbol* cr_fsym = Sema_resolve(self, cr_fn);
                /* pass */
                if (((!_tr_str_eqv((cr_fsym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(cr_fsym->ty)) != ((unsigned long long)(0LL))))) {
                    /* pass */
                    AstType* cr_ret = (*cr_fsym->ty);
                    /* pass */
                    if (cr_ret->is_borrow) {
                        /* pass */
                        if (((cr_ret->from_index >= 0LL) && (cr_ret->from_index < cr_args->len))) {
                            /* pass */
                            return Sema_compute_region(self, ((Expr*)List_ptr_get(cr_args, cr_ret->from_index)));
                        }
                        /* pass */
                        return _tr_str_lit_len("", 0LL);
                    }
                    /* pass */
                    return _tr_str_lit_len("@owned", 6LL);
                }
            } else if (1) {
                __auto_type _ = _t402;
                /* pass */
            }
        }
        /* pass */
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t401;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool Sema_region_outlives(Sema* self, TrStr longer, TrStr shorter) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->current_func_outlives_a->len)) {
        /* pass */
        if ((_tr_str_eqv((List_TrStr_get(self->current_func_outlives_a, i)), (longer)) && _tr_str_eqv((List_TrStr_get(self->current_func_outlives_b, i)), (shorter)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < self->current_func_outlives_a->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(self->current_func_outlives_a, j)), (longer))) {
            /* pass */
            TrStr mid = List_TrStr_get(self->current_func_outlives_b, j);
            /* pass */
            long long kk = 0LL;
            /* pass */
            while ((kk < self->current_func_outlives_a->len)) {
                /* pass */
                if ((_tr_str_eqv((List_TrStr_get(self->current_func_outlives_a, kk)), (mid)) && _tr_str_eqv((List_TrStr_get(self->current_func_outlives_b, kk)), (shorter)))) {
                    /* pass */
                    _tr_str_release(mid);
                    return true;
                }
                /* pass */
                kk = (kk + 1LL);
            }
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_field_is_borrow(Sema* self, Expr* obj, TrStr field) {
    /* pass */
    if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr cls_name = _tr_str_lit_len("", 0LL);
    /* pass */
    __auto_type _t403 = (*obj);
    if (_t403.tag == Expr_EIdent) {
        __auto_type fb_obj = _t403.data.EIdent.name;
        /* pass */
        if (_tr_str_eqv((fb_obj), (_tr_str_lit_len("self", 4LL)))) {
            /* pass */
            TrStr _strtmp_t404 = _tr_str_retain(self->current_class_name);
            _tr_str_release(cls_name);
            cls_name = _strtmp_t404;
        } else {
            /* pass */
            Symbol* fb_sym = Sema_resolve(self, fb_obj);
            /* pass */
            if (((!_tr_str_eqv((fb_sym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(fb_sym->ty)) != ((unsigned long long)(0LL))))) {
                /* pass */
                TrStr _strtmp_t405 = _tr_str_retain((*fb_sym->ty)->name);
                _tr_str_release(cls_name);
                cls_name = _strtmp_t405;
            }
        }
    } else if (1) {
        __auto_type _ = _t403;
        /* pass */
    }
    /* pass */
    if (_tr_str_eqv((cls_name), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        _tr_str_release(cls_name);
        return false;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
        /* pass */
        _tr_str_release(cls_name);
        return false;
    }
    /* pass */
    ClassDef* fb_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
    /* pass */
    long long fb_i = 0LL;
    /* pass */
    while ((fb_i < fb_cls->fields->len)) {
        /* pass */
        FieldDef* fb_fld = ((FieldDef*)List_ptr_get(fb_cls->fields, fb_i));
        /* pass */
        if (_tr_str_eqv((fb_fld->name), (field))) {
            /* pass */
            if ((((unsigned long long)(fb_fld->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                _tr_str_release(cls_name);
                return (*fb_fld->ty)->is_borrow;
            }
        }
        /* pass */
        fb_i = (fb_i + 1LL);
    }
    /* pass */
    _tr_str_release(cls_name);
    return false;
}

__attribute__((hot)) void Sema_mark_str_arg(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t406 = (*e);
    if (_t406.tag == HirExpr_EIdent) {
        __auto_type nm = _t406.data.EIdent.name;
        /* pass */
        if (_tr_str_eqv((hir_expr_type(e)->name), (_tr_str_lit_len("str", 3LL)))) {
            /* pass */
            Sema_mark_str_escaped(self, nm);
        }
    } else if (1) {
        __auto_type _ = _t406;
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_escaped_str_args(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t407 = (*e);
    if (_t407.tag == HirExpr_EBinOp) {
        __auto_type l = _t407.data.EBinOp.left;
__auto_type r = _t407.data.EBinOp.right;
        /* pass */
        Sema_mark_escaped_str_args(self, l);
        /* pass */
        Sema_mark_escaped_str_args(self, r);
    } else if (_t407.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t407.data.EUnaryOp.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t407.tag == HirExpr_ECall) {
        __auto_type callee = _t407.data.ECall.callee;
__auto_type args = _t407.data.ECall.args;
        /* pass */
        Sema_mark_escaped_str_args(self, callee);
        /* pass */
        bool _call_is_c_free = false;
        /* pass */
        __auto_type _t408 = (*callee);
        if (_t408.tag == HirExpr_EIdent) {
            __auto_type _cnm = _t408.data.EIdent.name;
            /* pass */
            if (_tr_str_eqv((_cnm), (_tr_str_lit_len("_tr_c_free", 10LL)))) {
                /* pass */
                _call_is_c_free = true;
            }
        } else if (1) {
            __auto_type _ = _t408;
            /* pass */
        }
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            if (_call_is_c_free) {
                /* pass */
                __auto_type _t409 = (*((HirExpr*)List_ptr_get(args, i)));
                if (_t409.tag == HirExpr_ECast) {
                    __auto_type _cf_inner = _t409.data.ECast.expr;
                    /* pass */
                    Sema_mark_str_arg(self, _cf_inner);
                } else if (1) {
                    __auto_type _ = _t409;
                    /* pass */
                }
            }
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t407.data.EMethodCall.obj;
__auto_type mname = _t407.data.EMethodCall.method;
__auto_type args = _t407.data.EMethodCall.args;
        /* pass */
        Sema_mark_escaped_str_args(self, obj);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t407.data.EPropAccess.obj;
        Sema_mark_escaped_str_args(self, obj);
    } else if (_t407.tag == HirExpr_EIndex) {
        __auto_type obj = _t407.data.EIndex.obj;
__auto_type idx = _t407.data.EIndex._tr_v_index;
        /* pass */
        Sema_mark_escaped_str_args(self, obj);
        /* pass */
        Sema_mark_escaped_str_args(self, idx);
    } else if (_t407.tag == HirExpr_ECast) {
        __auto_type inner = _t407.data.ECast.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t407.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t407.data.ETryExpr.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t407.tag == HirExpr_EIfElse) {
        __auto_type c = _t407.data.EIfElse.cond;
__auto_type t = _t407.data.EIfElse.then_e;
__auto_type f = _t407.data.EIfElse.else_e;
        /* pass */
        Sema_mark_escaped_str_args(self, c);
        /* pass */
        Sema_mark_escaped_str_args(self, t);
        /* pass */
        Sema_mark_escaped_str_args(self, f);
    } else if (_t407.tag == HirExpr_EList) {
        __auto_type items = _t407.data.EList.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_ESet) {
        __auto_type items = _t407.data.ESet.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_ETuple) {
        __auto_type items = _t407.data.ETuple.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_EDict) {
        __auto_type keys = _t407.data.EDict.keys;
__auto_type vals = _t407.data.EDict.vals;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < keys->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(keys, i)));
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(vals, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_ERange) {
        __auto_type s = _t407.data.ERange.start;
__auto_type en = _t407.data.ERange.end;
        /* pass */
        Sema_mark_escaped_str_args(self, s);
        /* pass */
        Sema_mark_escaped_str_args(self, en);
    } else if (_t407.tag == HirExpr_EAwait) {
        __auto_type inner = _t407.data.EAwait.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t407.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t407.data.EAwaitTimeout.expr;
__auto_type to = _t407.data.EAwaitTimeout.timeout_ms;
        /* pass */
        Sema_mark_escaped_str_args(self, inner);
        /* pass */
        Sema_mark_escaped_str_args(self, to);
    } else if (_t407.tag == HirExpr_EYield) {
        __auto_type inner = _t407.data.EYield.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t407.tag == HirExpr_EFString) {
        __auto_type parts = _t407.data.EFString.parts;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < parts->len)) {
            /* pass */
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, i));
            /* pass */
            if (part->is_expr) {
                /* pass */
                Sema_mark_escaped_str_args(self, part->expr);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_EClosure) {
        __auto_type captures = _t407.data.EClosure.captures;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < captures->len)) {
            /* pass */
            HirParam* cap = ((HirParam*)List_ptr_get(captures, i));
            /* pass */
            if (_tr_str_eqv((cap->ty->name), (_tr_str_lit_len("str", 3LL)))) {
                /* pass */
                Sema_mark_str_escaped(self, cap->name);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t407.data.ESuperMethodCall.args;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_mark_str_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_ESuperPropAccess) {
        /* pass */
    } else if (_t407.tag == HirExpr_EListComp) {
        __auto_type element = _t407.data.EListComp.element;
__auto_type generators = _t407.data.EListComp.generators;
        /* pass */
        Sema_mark_str_arg(self, element);
        /* pass */
        Sema_mark_escaped_str_args(self, element);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < generators->len)) {
            /* pass */
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
            /* pass */
            Sema_mark_escaped_str_args(self, gen->iter);
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < gen->ifs->len)) {
                /* pass */
                Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_EGeneratorExpr) {
        __auto_type element = _t407.data.EGeneratorExpr.element;
__auto_type generators = _t407.data.EGeneratorExpr.generators;
        /* pass */
        Sema_mark_str_arg(self, element);
        /* pass */
        Sema_mark_escaped_str_args(self, element);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < generators->len)) {
            /* pass */
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
            /* pass */
            Sema_mark_escaped_str_args(self, gen->iter);
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < gen->ifs->len)) {
                /* pass */
                Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t407.tag == HirExpr_ESlice) {
        __auto_type start = _t407.data.ESlice.start;
__auto_type stop = _t407.data.ESlice.stop;
__auto_type step = _t407.data.ESlice.step;
        /* pass */
        Sema_mark_escaped_str_args(self, start);
        /* pass */
        Sema_mark_escaped_str_args(self, stop);
        /* pass */
        Sema_mark_escaped_str_args(self, step);
    } else if (_t407.tag == HirExpr_ETry) {
        /* pass */
    } else if (_t407.tag == HirExpr_EDo) {
        __auto_type _do_b = _t407.data.EDo.body;
        /* pass */
    } else if (_t407.tag == HirExpr_EMatchExpr) {
        /* pass */
    } else if (_t407.tag == HirExpr_ELoop) {
        /* pass */
    } else if (_t407.tag == HirExpr_EWhileExpr) {
        /* pass */
    } else if (_t407.tag == HirExpr_ELitInt) {
        /* pass */
    } else if (_t407.tag == HirExpr_ELitFloat) {
        /* pass */
    } else if (_t407.tag == HirExpr_ELitStr) {
        /* pass */
    } else if (_t407.tag == HirExpr_ELitBytes) {
        /* pass */
    } else if (_t407.tag == HirExpr_ERawStr) {
        /* pass */
    } else if (_t407.tag == HirExpr_ELitChar) {
        /* pass */
    } else if (_t407.tag == HirExpr_ELitBool) {
        /* pass */
    } else if (_t407.tag == HirExpr_ELitNone) {
        __auto_type _ = _t407.data.ELitNone.ty;
        /* pass */
    } else if (_t407.tag == HirExpr_ESizeOf) {
        /* pass */
    } else if (_t407.tag == HirExpr_EIdent) {
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_coll_escaped(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->coll_escaped = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
}

__attribute__((hot)) void Sema_mark_coll_arg(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t410 = (*e);
    if (_t410.tag == HirExpr_EIdent) {
        __auto_type nm = _t410.data.EIdent.name;
        /* pass */
        TrStr tn = _tr_str_retain(hir_expr_type(e)->name);
        /* pass */
        if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
            /* pass */
            Sema_mark_coll_escaped(self, nm);
        } else if (_tr_dict_contains(self->classes, _tr_strz(tn))) {
            /* pass */
            Sema_mark_coll_escaped(self, nm);
        }
        _tr_str_release(tn);
    } else if (1) {
        __auto_type _ = _t410;
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_escaped_coll_args(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t411 = (*e);
    if (_t411.tag == HirExpr_EBinOp) {
        __auto_type l = _t411.data.EBinOp.left;
__auto_type r = _t411.data.EBinOp.right;
        /* pass */
        Sema_mark_coll_arg(self, l);
        /* pass */
        Sema_mark_escaped_coll_args(self, l);
        /* pass */
        Sema_mark_coll_arg(self, r);
        /* pass */
        Sema_mark_escaped_coll_args(self, r);
    } else if (_t411.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t411.data.EUnaryOp.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t411.tag == HirExpr_ECall) {
        __auto_type callee = _t411.data.ECall.callee;
__auto_type args = _t411.data.ECall.args;
        /* pass */
        Sema_mark_escaped_coll_args(self, callee);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t411.data.EMethodCall.obj;
__auto_type method = _t411.data.EMethodCall.method;
__auto_type args = _t411.data.EMethodCall.args;
        /* pass */
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("values", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("items", 5LL))))) {
            /* pass */
            __auto_type _t412 = (*obj);
            if (_t412.tag == HirExpr_EIdent) {
                /* pass */
                AstType* obj_ty = hir_expr_type(obj);
                /* pass */
                if ((_tr_str_eqv((obj_ty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((obj_ty->name), (_tr_str_lit_len("Map", 3LL))))) {
                    /* pass */
                    if ((obj_ty->args->len > 1LL)) {
                        /* pass */
                        TrStr obj_vt = _tr_str_retain((*((AstType**)List_ptr_get(obj_ty->args, 1LL)))->name);
                        /* pass */
                        if ((_tr_str_eqv((obj_vt), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((obj_vt), (_tr_str_lit_len("String", 6LL))))) {
                            /* pass */
                            Sema_mark_coll_arg(self, obj);
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t412;
                /* pass */
            }
        }
        /* pass */
        bool _coll_static_call = false;
        /* pass */
        __auto_type _t413 = (*obj);
        if (_t413.tag == HirExpr_EIdent) {
            __auto_type _ocnm2 = _t413.data.EIdent.name;
            /* pass */
            if ((_tr_dict_contains(self->classes, _tr_strz(_ocnm2)) && (!Sema_is_local_var(self, _ocnm2)))) {
                /* pass */
                _coll_static_call = true;
            }
        } else if (1) {
            __auto_type _ = _t413;
            /* pass */
        }
        /* pass */
        Sema_mark_escaped_coll_args(self, obj);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            if ((!_coll_static_call)) {
                /* pass */
                Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            }
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t411.data.EPropAccess.obj;
        Sema_mark_escaped_coll_args(self, obj);
    } else if (_t411.tag == HirExpr_EIndex) {
        __auto_type obj = _t411.data.EIndex.obj;
__auto_type idx = _t411.data.EIndex._tr_v_index;
        /* pass */
        Sema_mark_escaped_coll_args(self, obj);
        /* pass */
        Sema_mark_coll_arg(self, idx);
        /* pass */
        Sema_mark_escaped_coll_args(self, idx);
    } else if (_t411.tag == HirExpr_ECast) {
        __auto_type inner = _t411.data.ECast.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t411.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t411.data.ETryExpr.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t411.tag == HirExpr_EIfElse) {
        __auto_type c = _t411.data.EIfElse.cond;
__auto_type t = _t411.data.EIfElse.then_e;
__auto_type f = _t411.data.EIfElse.else_e;
        /* pass */
        Sema_mark_escaped_coll_args(self, c);
        /* pass */
        Sema_mark_coll_arg(self, t);
        /* pass */
        Sema_mark_escaped_coll_args(self, t);
        /* pass */
        Sema_mark_coll_arg(self, f);
        /* pass */
        Sema_mark_escaped_coll_args(self, f);
    } else if (_t411.tag == HirExpr_EList) {
        __auto_type items = _t411.data.EList.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_ESet) {
        __auto_type items = _t411.data.ESet.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_ETuple) {
        __auto_type items = _t411.data.ETuple.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_EDict) {
        __auto_type keys = _t411.data.EDict.keys;
__auto_type vals = _t411.data.EDict.vals;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < keys->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(keys, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(keys, i)));
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(vals, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(vals, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_ERange) {
        __auto_type s = _t411.data.ERange.start;
__auto_type en = _t411.data.ERange.end;
        /* pass */
        Sema_mark_escaped_coll_args(self, s);
        /* pass */
        Sema_mark_escaped_coll_args(self, en);
    } else if (_t411.tag == HirExpr_EAwait) {
        __auto_type inner = _t411.data.EAwait.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t411.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t411.data.EAwaitTimeout.expr;
__auto_type to = _t411.data.EAwaitTimeout.timeout_ms;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, to);
    } else if (_t411.tag == HirExpr_EYield) {
        __auto_type inner = _t411.data.EYield.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t411.tag == HirExpr_EFString) {
        __auto_type parts = _t411.data.EFString.parts;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < parts->len)) {
            /* pass */
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, i));
            /* pass */
            if (part->is_expr) {
                /* pass */
                Sema_mark_escaped_coll_args(self, part->expr);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_EClosure) {
        __auto_type captures = _t411.data.EClosure.captures;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < captures->len)) {
            /* pass */
            HirParam* cap = ((HirParam*)List_ptr_get(captures, i));
            /* pass */
            TrStr ctn = _tr_str_retain(cap->ty->name);
            /* pass */
            if (((((_tr_str_eqv((ctn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Set", 3LL))))) {
                /* pass */
                Sema_mark_coll_escaped(self, cap->name);
            }
            /* pass */
            i = (i + 1LL);
            _tr_str_release(ctn);
        }
    } else if (_t411.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t411.data.ESuperMethodCall.args;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_ESuperPropAccess) {
        /* pass */
    } else if (_t411.tag == HirExpr_EListComp) {
        __auto_type element = _t411.data.EListComp.element;
__auto_type generators = _t411.data.EListComp.generators;
        /* pass */
        Sema_mark_coll_arg(self, element);
        /* pass */
        Sema_mark_escaped_coll_args(self, element);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < generators->len)) {
            /* pass */
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, gen->iter);
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < gen->ifs->len)) {
                /* pass */
                Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_EGeneratorExpr) {
        __auto_type element = _t411.data.EGeneratorExpr.element;
__auto_type generators = _t411.data.EGeneratorExpr.generators;
        /* pass */
        Sema_mark_coll_arg(self, element);
        /* pass */
        Sema_mark_escaped_coll_args(self, element);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < generators->len)) {
            /* pass */
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, gen->iter);
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < gen->ifs->len)) {
                /* pass */
                Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t411.tag == HirExpr_ESlice) {
        __auto_type start = _t411.data.ESlice.start;
__auto_type stop = _t411.data.ESlice.stop;
__auto_type step = _t411.data.ESlice.step;
        /* pass */
        Sema_mark_coll_arg(self, start);
        /* pass */
        Sema_mark_escaped_coll_args(self, start);
        /* pass */
        Sema_mark_coll_arg(self, stop);
        /* pass */
        Sema_mark_escaped_coll_args(self, stop);
        /* pass */
        Sema_mark_coll_arg(self, step);
        /* pass */
        Sema_mark_escaped_coll_args(self, step);
    } else if (_t411.tag == HirExpr_ETry) {
        /* pass */
    } else if (_t411.tag == HirExpr_EDo) {
        __auto_type _do_b = _t411.data.EDo.body;
        /* pass */
    } else if (_t411.tag == HirExpr_EMatchExpr) {
        /* pass */
    } else if (_t411.tag == HirExpr_ELoop) {
        /* pass */
    } else if (_t411.tag == HirExpr_EWhileExpr) {
        /* pass */
    } else if (_t411.tag == HirExpr_ELitInt) {
        /* pass */
    } else if (_t411.tag == HirExpr_ELitFloat) {
        /* pass */
    } else if (_t411.tag == HirExpr_ELitStr) {
        /* pass */
    } else if (_t411.tag == HirExpr_ELitBytes) {
        /* pass */
    } else if (_t411.tag == HirExpr_ERawStr) {
        /* pass */
    } else if (_t411.tag == HirExpr_ELitChar) {
        /* pass */
    } else if (_t411.tag == HirExpr_ELitBool) {
        /* pass */
    } else if (_t411.tag == HirExpr_ELitNone) {
        __auto_type _ = _t411.data.ELitNone.ty;
        /* pass */
    } else if (_t411.tag == HirExpr_ESizeOf) {
        /* pass */
    } else if (_t411.tag == HirExpr_EIdent) {
        /* pass */
    }
}

__attribute__((hot)) bool Sema_block_ends_in_jump(Sema* self, HirBlock* b) {
    /* pass */
    if ((b->stmts->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    HirStmt* last = ((HirStmt*)List_ptr_get(b->stmts, (b->stmts->len - 1LL)));
    /* pass */
    if ((((unsigned long long)(last)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t414 = (*last);
    if (_t414.tag == HirStmt_SReturn) {
        __auto_type _ = _t414.data.SReturn.val;
        return true;
    } else if (_t414.tag == HirStmt_SBreak) {
        __auto_type _ = _t414.data.SBreak.val;
        return true;
    } else if (_t414.tag == HirStmt_SContinue) {
        return true;
    } else if (1) {
        __auto_type _ = _t414;
        return false;
    }
}

__attribute__((hot)) void Sema_append_block_local_drops(Sema* self, HirBlock* hb, long long block_id) {
    /* pass */
    if (Sema_block_ends_in_jump(self, hb)) {
        /* pass */
        return;
    }
    /* pass */
    if ((self->scopes->len == 0LL)) {
        /* pass */
        return;
    }
    /* pass */
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
    /* pass */
    long long ki = 0LL;
    /* pass */
    while ((ki < scope->decl_order->len)) {
        /* pass */
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
        /* pass */
        if (((sym->decl_block_id == block_id) && Sema_is_droppable_sym(self, sym))) {
            /* pass */
            HirBlock_push(hb, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
        }
        /* pass */
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
}

__attribute__((hot)) void Sema_finalize_scope_drops(Sema* self, HirBlock* hb) {
    /* pass */
    if ((!Sema_block_ends_in_jump(self, hb))) {
        /* pass */
        Sema_append_drops_from(self, hb, (self->scopes->len - 1LL));
    }
}

__attribute__((hot)) bool Sema_block_str_decl(Sema* self, HirBlock* hb, TrStr nm) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hb->stmts->len)) {
        /* pass */
        __auto_type _t415 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t415.tag == HirStmt_SLet) {
            __auto_type sn = _t415.data.SLet.name;
__auto_type sty = _t415.data.SLet.ty;
            /* pass */
            if ((_tr_str_eqv((sn), (nm)) && _tr_str_eqv((sty->name), (_tr_str_lit_len("str", 3LL))))) {
                /* pass */
                return true;
            }
        } else if (1) {
            __auto_type _ = _t415;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_block_has_autodrop(Sema* self, HirBlock* hb, TrStr nm) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hb->stmts->len)) {
        /* pass */
        __auto_type _t416 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t416.tag == HirStmt_SAutoDrop) {
            __auto_type dn = _t416.data.SAutoDrop.name;
            /* pass */
            if (_tr_str_eqv((dn), (nm))) {
                /* pass */
                return true;
            }
        } else if (1) {
            __auto_type _ = _t416;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr Sema_block_coll_decl(Sema* self, HirBlock* hb, TrStr nm) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hb->stmts->len)) {
        /* pass */
        __auto_type _t417 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t417.tag == HirStmt_SLet) {
            __auto_type sn = _t417.data.SLet.name;
__auto_type sty = _t417.data.SLet.ty;
            /* pass */
            if (_tr_str_eqv((sn), (nm))) {
                /* pass */
                TrStr tn = _tr_str_retain(sty->name);
                /* pass */
                if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
                    /* pass */
                    return tn;
                }
            }
        } else if (1) {
            __auto_type _ = _t417;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool Sema_coll_droppable_by_sema(Sema* self, TrStr nm) {
    /* pass */
    Symbol* sym = Sema_resolve(self, nm);
    /* pass */
    if (_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    return Sema_is_droppable_sym(self, sym);
}

__attribute__((hot)) void Sema_apply_mir_if_drops(Sema* self, HirFunction* hf) {
    /* pass */
    List_ptr* plan = mir_if_drop_plan(hf);
    /* pass */
    long long psi = 0LL;
    /* pass */
    while ((psi < plan->len)) {
        /* pass */
        DropSite* site = ((DropSite*)List_ptr_get(plan, psi));
        /* pass */
        if ((!Sema_block_ends_in_jump(self, site->hir_block))) {
            /* pass */
            long long ppi = 0LL;
            /* pass */
            while ((ppi < site->places->len)) {
                /* pass */
                TrStr nm = List_TrStr_get(site->places, ppi);
                /* pass */
                if ((!Sema_block_has_autodrop(self, site->hir_block, nm))) {
                    /* pass */
                    if (Sema_block_str_decl(self, site->hir_block, nm)) {
                        /* pass */
                        HirBlock_push(site->hir_block, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, _tr_str_lit_len("str", 3LL))));
                    } else {
                        /* pass */
                        TrStr ctn = Sema_block_coll_decl(self, site->hir_block, nm);
                        /* pass */
                        if (((!_tr_str_eqv((ctn), (_tr_str_lit_len("", 0LL)))) && Sema_coll_droppable_by_sema(self, nm))) {
                            /* pass */
                            HirBlock_push(site->hir_block, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, ctn)));
                        }
                    }
                }
                /* pass */
                ppi = (ppi + 1LL);
                _tr_str_release(nm);
            }
        }
        /* pass */
        psi = (psi + 1LL);
    }
    List_ptr_free_obj(plan, _trdrop_DropSite);
}

__attribute__((hot)) void Sema_declare(Sema* self, TrStr name, SymbolKind kind, AstType** ty, bool is_mut) {
    /* pass */
    if ((((!_tr_str_eqv((name), (_tr_str_lit_len("self", 4LL)))) && (!_tr_str_eqv((name), (_tr_str_lit_len("_", 1LL))))) && (_tr_strlen(_tr_strz(name)) > 1LL))) {
        /* pass */
        TrStr cat = Sema_is_reserved_error(self, name);
        /* pass */
        if ((!_tr_str_eqv((cat), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            ({ TrStr _at_t418 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cat)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" and cannot be used as a name. Choose a different name (e.g. 'my_", 65LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("').", 3LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t418); _tr_str_release(_at_t418); });
        } else {
            /* pass */
            bool is_toplevel_decl = (_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))) && (kind.tag != SymbolKind_make_SVariable().tag));
            /* pass */
            if (is_toplevel_decl) {
                /* pass */
                TrStr kcat = Sema_is_reserved_keyword(self, name);
                /* pass */
                if ((!_tr_str_eqv((kcat), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    ({ TrStr _at_t419 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (kcat)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" and is reserved. Choose a different name (e.g. 'my_", 52LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("').", 3LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t419); _tr_str_release(_at_t419); });
                }
            }
        }
    }
    /* pass */
    Symbol* sym = Symbol_init(name, kind, ty);
    /* pass */
    sym->scope_depth = self->current_scope_depth;
    /* pass */
    sym->is_mut = is_mut;
    /* pass */
    sym->decl_block_depth = self->block_depth;
    /* pass */
    long long bsbase = 0LL;
    /* pass */
    if ((self->block_stack_base->len > 0LL)) {
        /* pass */
        bsbase = List_i64_get(self->block_stack_base, (self->block_stack_base->len - 1LL));
    }
    /* pass */
    if ((self->block_stack->len > bsbase)) {
        /* pass */
        sym->decl_block_id = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
    } else {
        /* pass */
        sym->decl_block_id = 0LL;
    }
    /* pass */
    if ((self->scopes->len > 0LL)) {
        /* pass */
        Scope* decl_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
        /* pass */
        if ((!_tr_dict_contains(decl_scope->variables, _tr_strz(name)))) {
            /* pass */
            List_TrStr_append(decl_scope->decl_order, name);
        }
        /* pass */
        _tr_dict_set(decl_scope->variables, _tr_strz(name), sym);
    } else {
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) Symbol* Sema_resolve(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        Scope* scope = ((Scope*)List_ptr_get(self->scopes, i));
        /* pass */
        if (_tr_dict_contains(scope->variables, _tr_strz(name))) {
            /* pass */
            return ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(name)));
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        return ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
    }
    /* pass */
    return Symbol_init(_tr_str_lit_len("", 0LL), SymbolKind_make_SVariable(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL))));
}

__attribute__((hot)) bool Sema_is_known_name(Sema* self, TrStr name) {
    /* pass */
    if (Sema__is_type_param_in_scope(self, name)) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->fn_sigs, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->extern_names, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->enums, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->interfaces, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->type_aliases, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (Sema_is_type_name(self, name)) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("dealloc", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("realloc", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("sizeof", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("repr", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("c_callback", 10LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_lenv((name)) >= 4LL) && _tr_str_eqv((_tr_str_slicev((name), 0LL, 4LL)), (_tr_str_lit_len("_tr_", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_type_name(Sema* self, TrStr nm) {
    /* pass */
    if (((_tr_dict_contains(self->classes, _tr_strz(nm)) || _tr_dict_contains(self->enums, _tr_strz(nm))) || _tr_dict_contains(self->interfaces, _tr_strz(nm)))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((nm), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("void", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((nm), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("isize", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((nm), (_tr_str_lit_len("u8", 2LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("usize", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((nm), (_tr_str_lit_len("f32", 3LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("f64", 3LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr Sema_type_ref_name(Sema* self, Expr* raw) {
    /* pass */
    if ((((unsigned long long)(raw)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    __auto_type _t420 = (*raw);
    if (_t420.tag == Expr_EIdent) {
        __auto_type nm = _t420.data.EIdent.name;
        /* pass */
        if ((!Sema_is_type_name(self, nm))) {
            /* pass */
            return _tr_str_lit_len("", 0LL);
        }
        /* pass */
        Symbol* s = Sema_resolve(self, nm);
        /* pass */
        if (((!_tr_str_eqv((s->name), (_tr_str_lit_len("", 0LL)))) && (s->kind.tag == SymbolKind_make_SVariable().tag))) {
            /* pass */
            return _tr_str_lit_len("", 0LL);
        }
        /* pass */
        return _tr_str_retain(nm);
    } else if (1) {
        __auto_type _ = _t420;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool Sema_is_global_not_local(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    return _tr_dict_contains(self->globals, _tr_strz(name));
}

__attribute__((hot)) HirProgram* Sema_analyze(Sema* self, Program* prog) {
    /* pass */
    HirProgram* hp = HirProgram_init();
    /* pass */
    long long ppi = 0LL;
    /* pass */
    while ((ppi < prog->decls->len)) {
        /* pass */
        __auto_type _t421 = (*((Decl*)List_ptr_get(prog->decls, ppi)));
        if (_t421.tag == Decl_DClass) {
            __auto_type c = _t421.data.DClass.cls;
            /* pass */
            if ((!_tr_dict_contains(self->classes, _tr_strz(c->name)))) {
                /* pass */
                _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
            }
        } else if (_t421.tag == Decl_DActor) {
            __auto_type c = _t421.data.DActor.cls;
            /* pass */
            if ((!_tr_dict_contains(self->classes, _tr_strz(c->name)))) {
                /* pass */
                _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
            }
        } else if (1) {
            __auto_type _ = _t421;
            /* pass */
        }
        /* pass */
        ppi = (ppi + 1LL);
    }
    /* pass */
    long long gpi = 0LL;
    /* pass */
    while ((gpi < prog->decls->len)) {
        /* pass */
        __auto_type _t422 = (*((Decl*)List_ptr_get(prog->decls, gpi)));
        if (_t422.tag == Decl_DTopLevelStmt) {
            __auto_type gs = _t422.data.DTopLevelStmt.stmt;
            /* pass */
            __auto_type _t423 = (*gs);
            if (_t423.tag == Stmt_SLet) {
                __auto_type g_name = _t423.data.SLet.name;
__auto_type g_is_mut = _t423.data.SLet.is_mut;
__auto_type g_ty_ptr = _t423.data.SLet.ty;
__auto_type g_val_ptr = _t423.data.SLet.val;
                /* pass */
                AstType** g_ty = ((AstType**)(0LL));
                /* pass */
                if ((((unsigned long long)(g_ty_ptr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    g_ty = g_ty_ptr;
                } else if ((((unsigned long long)(g_val_ptr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    __auto_type _t424 = (*g_val_ptr);
                    if (_t424.tag == Expr_ELitInt) {
                        __auto_type _ = _t424.data.ELitInt.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)));
                    } else if (_t424.tag == Expr_ELitFloat) {
                        __auto_type _ = _t424.data.ELitFloat.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("float", 5LL)));
                    } else if (_t424.tag == Expr_ELitStr) {
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)));
                    } else if (_t424.tag == Expr_ELitBool) {
                        __auto_type _ = _t424.data.ELitBool.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)));
                    } else if (1) {
                        __auto_type _ = _t424;
                        /* pass */
                    }
                }
                /* pass */
                if ((((unsigned long long)(g_ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    Sema_declare(self, g_name, SymbolKind_make_SVariable(), g_ty, g_is_mut);
                }
            } else if (1) {
                __auto_type _ = _t423;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t422;
            /* pass */
        }
        /* pass */
        gpi = (gpi + 1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        Sema_register_decl(self, ((Decl*)List_ptr_get(prog->decls, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (self->strict_mode) {
        /* pass */
        Sema_check_ownership_cycles(self, prog);
    }
    /* pass */
    if (self->strict_mode) {
        /* pass */
        i = 0LL;
        /* pass */
        while ((i < prog->decls->len)) {
            /* pass */
            __auto_type _t425 = (*((Decl*)List_ptr_get(prog->decls, i)));
            if (_t425.tag == Decl_DClass) {
                __auto_type mc = _t425.data.DClass.cls;
                /* pass */
                long long mmi = 0LL;
                /* pass */
                while ((mmi < mc->methods->len)) {
                    /* pass */
                    FunctionDef* mm = ((FunctionDef*)List_ptr_get(mc->methods, mmi));
                    /* pass */
                    if (_block_mutates_self(mm->body)) {
                        /* pass */
                        ({ TrStr _dkt_t426 = (({ TrStr _cl = (_tr_strx_concatv((mc->name), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (mm->name)); _tr_str_release(_cl); _cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t426), true); _tr_str_release(_dkt_t426); });
                    }
                    /* pass */
                    mmi = (mmi + 1LL);
                }
            } else if (_t425.tag == Decl_DActor) {
                __auto_type ac = _t425.data.DActor.cls;
                /* pass */
                long long ami = 0LL;
                /* pass */
                while ((ami < ac->methods->len)) {
                    /* pass */
                    FunctionDef* am = ((FunctionDef*)List_ptr_get(ac->methods, ami));
                    /* pass */
                    if (_block_mutates_self(am->body)) {
                        /* pass */
                        ({ TrStr _dkt_t427 = (({ TrStr _cl = (_tr_strx_concatv((ac->name), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (am->name)); _tr_str_release(_cl); _cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t427), true); _tr_str_release(_dkt_t427); });
                    }
                    /* pass */
                    ami = (ami + 1LL);
                }
            } else if (_t425.tag == Decl_DExtend) {
                __auto_type mtarget = _t425.data.DExtend.target;
__auto_type mmethods = _t425.data.DExtend.methods;
                /* pass */
                long long emi = 0LL;
                /* pass */
                while ((emi < mmethods->len)) {
                    /* pass */
                    FunctionDef* em = ((FunctionDef*)List_ptr_get(mmethods, emi));
                    /* pass */
                    if (_block_mutates_self(em->body)) {
                        /* pass */
                        ({ TrStr _dkt_t428 = (({ TrStr _cl = (_tr_strx_concatv((mtarget), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (em->name)); _tr_str_release(_cl); _cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t428), true); _tr_str_release(_dkt_t428); });
                    }
                    /* pass */
                    emi = (emi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t425;
                /* pass */
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        Decl* d = ((Decl*)List_ptr_get(prog->decls, i));
        /* pass */
        __auto_type _t429 = (*d);
        if (_t429.tag == Decl_DFunction) {
            __auto_type f = _t429.data.DFunction.func;
            /* pass */
            List_ptr_append(hp->functions, Sema_lower_func(self, f));
        } else if (_t429.tag == Decl_DClass) {
            __auto_type c = _t429.data.DClass.cls;
            /* pass */
            List_ptr_append(hp->classes, Sema_lower_class(self, c));
        } else if (_t429.tag == Decl_DActor) {
            __auto_type c = _t429.data.DActor.cls;
            /* pass */
            List_ptr_append(hp->classes, Sema_lower_class(self, c));
        } else if (_t429.tag == Decl_DEnum) {
            __auto_type e = _t429.data.DEnum.enm;
            /* pass */
            List_ptr_append(hp->enums, Sema_lower_enum(self, e));
        } else if (_t429.tag == Decl_DInterface) {
            __auto_type i_def = _t429.data.DInterface.iface;
            /* pass */
            List_ptr_append(hp->interfaces, Sema_lower_interface(self, i_def));
        } else if (_t429.tag == Decl_DExtend) {
            __auto_type target = _t429.data.DExtend.target;
__auto_type methods = _t429.data.DExtend.methods;
            /* pass */
            self->current_class_name = _tr_str_retain(target);
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(target))) {
                /* pass */
                self->current_region_params = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(target)))->region_params;
            }
            /* pass */
            long long hi = 0LL;
            /* pass */
            while ((hi < methods->len)) {
                /* pass */
                FunctionDef* f = ((FunctionDef*)List_ptr_get(methods, hi));
                /* pass */
                List_ptr_append(hp->functions, Sema_lower_func(self, f));
                /* pass */
                hi = (hi + 1LL);
            }
            /* pass */
            self->current_class_name = _tr_str_lit_len("", 0LL);
            /* pass */
            self->current_region_params = (void*)List_TrStr_new();
        } else if (_t429.tag == Decl_DTopLevelStmt) {
            __auto_type s = _t429.data.DTopLevelStmt.stmt;
            /* pass */
            List_ptr_append(hp->top_level_stmts, Sema_lower_stmt(self, s));
        } else if (_t429.tag == Decl_DExtern) {
            __auto_type functions = _t429.data.DExtern.functions;
            /* pass */
            long long ei = 0LL;
            /* pass */
            while ((ei < functions->len)) {
                /* pass */
                FunctionDef* ef = ((FunctionDef*)List_ptr_get(functions, ei));
                /* pass */
                HirFunction* hef = Sema_lower_func(self, ef);
                /* pass */
                hef->is_extern = true;
                /* pass */
                List_ptr_append(hp->extern_funcs, _tr_obj_retain(hef));
                /* pass */
                ei = (ei + 1LL);
                _tr_obj_release(hef, _trdrop_HirFunction);
            }
        } else if (_t429.tag == Decl_DDecoratorDef) {
            __auto_type f = _t429.data.DDecoratorDef.func;
            /* pass */
            HirFunction* hdf = Sema_lower_func(self, f);
            /* pass */
            hdf->is_decorator = true;
            /* pass */
            List_ptr_append(hp->decorator_defs, _tr_obj_retain(hdf));
            _tr_obj_release(hdf, _trdrop_HirFunction);
        } else if (_t429.tag == Decl_DTypeAlias) {
            __auto_type alias_name = _t429.data.DTypeAlias.name;
__auto_type target_ty = _t429.data.DTypeAlias.target;
            /* pass */
            List_TrStr_append(hp->type_alias_names, alias_name);
            /* pass */
            AstType** ta_ty_ptr = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
            /* pass */
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                ta_ty_ptr = target_ty;
            }
            /* pass */
            List_ptr_append(hp->type_alias_types, ta_ty_ptr);
        } else if (1) {
            __auto_type _ = _t429;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long nci = 0LL;
    /* pass */
    while ((nci < self->nested_classes->len)) {
        /* pass */
        List_ptr_append(hp->classes, _tr_obj_retain(((HirClass*)List_ptr_get(self->nested_classes, nci))));
        /* pass */
        nci = (nci + 1LL);
    }
    /* pass */
    long long nfi = 0LL;
    /* pass */
    while ((nfi < self->nested_functions->len)) {
        /* pass */
        List_ptr_append(hp->functions, _tr_obj_retain(((HirFunction*)List_ptr_get(self->nested_functions, nfi))));
        /* pass */
        nfi = (nfi + 1LL);
    }
    /* pass */
    long long nei = 0LL;
    /* pass */
    while ((nei < self->nested_enums->len)) {
        /* pass */
        List_ptr_append(hp->enums, _tr_obj_retain(((HirEnum*)List_ptr_get(self->nested_enums, nei))));
        /* pass */
        nei = (nei + 1LL);
    }
    /* pass */
    long long nii = 0LL;
    /* pass */
    while ((nii < self->nested_interfaces->len)) {
        /* pass */
        List_ptr_append(hp->interfaces, _tr_obj_retain(((HirInterface*)List_ptr_get(self->nested_interfaces, nii))));
        /* pass */
        nii = (nii + 1LL);
    }
    /* pass */
    Sema_compute_return_ownership(self, hp);
    /* pass */
    Sema_compute_param_ownership(self, hp);
    /* pass */
    Sema_apply_borrow_drops(self, hp);
    /* pass */
    Sema_check_free_fn_method_c_name_collisions(self, hp);
    /* pass */
    return hp;
}

__attribute__((hot)) void Sema_check_free_fn_method_c_name_collisions(Sema* self, HirProgram* hp) {
    /* pass */
    TrMap* method_c_names = _tr_dict_new(32LL);
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < hp->functions->len)) {
        /* pass */
        HirFunction* mf = ((HirFunction*)List_ptr_get(hp->functions, mi));
        /* pass */
        if (((!_tr_str_eqv((mf->class_name), (_tr_str_lit_len("", 0LL)))) && (!mf->is_extern))) {
            /* pass */
            TrStr mc_name = ({ TrStr _cl = (_tr_strx_concatv((mf->class_name), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (mf->name)); _tr_str_release(_cl); _cres; });
            /* pass */
            if ((!_tr_dict_contains(method_c_names, _tr_strz(mc_name)))) {
                /* pass */
                ({ TrStr _dvt_t430 = (({ TrStr _cl = (_tr_strx_concatv((mf->class_name), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (mf->name)); _tr_str_release(_cl); _cres; })); _tr_dict_set(method_c_names, _tr_strz(mc_name), _tr_str_box(_tr_str_retain(_dvt_t430))); _tr_str_release(_dvt_t430); });
            }
        }
        /* pass */
        mi = (mi + 1LL);
    }
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < hp->functions->len)) {
        /* pass */
        HirFunction* ff = ((HirFunction*)List_ptr_get(hp->functions, fi));
        /* pass */
        if ((((_tr_str_eqv((ff->class_name), (_tr_str_lit_len("", 0LL))) && (!ff->is_extern)) && (!_tr_str_eqv((ff->name), (_tr_str_lit_len("main", 4LL))))) && (ff->generics->len == 0LL))) {
            /* pass */
            TrStr fc_name = _tr_str_retain(ff->name);
            /* pass */
            if (_tr_dict_contains(method_c_names, _tr_strz(fc_name))) {
                /* pass */
                ({ TrStr _at_t431 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[C-1] free function '", 21LL)), (ff->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' generates the same C symbol name ('", 37LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fc_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("') as class/enum method '", 25LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(method_c_names, _tr_strz(fc_name))))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.\n      This compiles cleanly on its own but fails at LINK time ('multiple definition of \"", 91LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fc_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\"') the moment a build pulls in both definitions.\n      FIX: rename one of the two (the free function '", 103LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ff->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("', or the method '", 18LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(method_c_names, _tr_strz(fc_name))))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("').", 3LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t431); _tr_str_release(_at_t431); });
            }
        }
        /* pass */
        fi = (fi + 1LL);
    }
}

__attribute__((hot)) void Sema_compute_return_ownership(Sema* self, HirProgram* hp) {
    /* pass */
    List_TrStr* keys = (void*)List_TrStr_new();
    /* pass */
    List_ptr* fns = (void*)List_ptr_new();
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < hp->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(hp->functions, fi));
        /* pass */
        if (Sema__fn_ret_is_heap_class(self, f)) {
            /* pass */
            List_TrStr_append(keys, f->name);
            /* pass */
            List_ptr_append(fns, _tr_obj_retain(f));
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < hp->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < c->methods->len)) {
            /* pass */
            HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
            /* pass */
            if (Sema__fn_ret_is_heap_class(self, m)) {
                /* pass */
                ({ TrStr _at_t432 = (({ TrStr _cl = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (m->name)); _tr_str_release(_cl); _cres; })); List_TrStr_append(keys, _at_t432); _tr_str_release(_at_t432); });
                /* pass */
                List_ptr_append(fns, _tr_obj_retain(m));
            }
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    long long ei = 0LL;
    /* pass */
    while ((ei < hp->enums->len)) {
        /* pass */
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, ei));
        /* pass */
        long long emi = 0LL;
        /* pass */
        while ((emi < en->methods->len)) {
            /* pass */
            HirFunction* em = ((HirFunction*)List_ptr_get(en->methods, emi));
            /* pass */
            if (Sema__fn_ret_is_heap_class(self, em)) {
                /* pass */
                ({ TrStr _at_t433 = (({ TrStr _cl = (_tr_strx_concatv((en->name), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (em->name)); _tr_str_release(_cl); _cres; })); List_TrStr_append(keys, _at_t433); _tr_str_release(_at_t433); });
                /* pass */
                List_ptr_append(fns, _tr_obj_retain(em));
            }
            /* pass */
            emi = (emi + 1LL);
        }
        /* pass */
        ei = (ei + 1LL);
    }
    /* pass */
    long long ki = 0LL;
    /* pass */
    while ((ki < keys->len)) {
        /* pass */
        HirFunction* kf = ((HirFunction*)List_ptr_get(fns, ki));
        /* pass */
        bool kf_owned = true;
        /* pass */
        if ((((unsigned long long)(kf->ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if (((kf->ret_ty->is_borrow || (!_tr_str_eqv((kf->ret_ty->from_param), (_tr_str_lit_len("", 0LL))))) || (kf->ret_ty->from_regions->len > 0LL))) {
                /* pass */
                kf_owned = false;
            }
        }
        /* pass */
        ({ TrStr _dkt_t434 = (List_TrStr_get(keys, ki)); _tr_dict_set(self->fn_ret_owned, _tr_strz(_dkt_t434), kf_owned); _tr_str_release(_dkt_t434); });
        /* pass */
        ki = (ki + 1LL);
    }
    /* pass */
    bool changed = true;
    /* pass */
    while (changed) {
        /* pass */
        changed = false;
        /* pass */
        long long xi = 0LL;
        /* pass */
        while ((xi < fns->len)) {
            /* pass */
            TrStr k = List_TrStr_get(keys, xi);
            /* pass */
            if (((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(k)))) {
                /* pass */
                List_ptr* rets = (void*)List_ptr_new();
                /* pass */
                Sema__collect_returns(self, ((HirFunction*)List_ptr_get(fns, xi))->body, rets);
                /* pass */
                bool all_owned = true;
                /* pass */
                long long ri = 0LL;
                /* pass */
                while ((ri < rets->len)) {
                    /* pass */
                    if ((!Sema__ret_yields_owned(self, ((HirExpr*)List_ptr_get(rets, ri))))) {
                        /* pass */
                        all_owned = false;
                    }
                    /* pass */
                    ri = (ri + 1LL);
                }
                /* pass */
                if ((!all_owned)) {
                    /* pass */
                    _tr_dict_set(self->fn_ret_owned, _tr_strz(k), false);
                    /* pass */
                    changed = true;
                }
            }
            /* pass */
            xi = (xi + 1LL);
            _tr_str_release(k);
        }
    }
    /* pass */
    long long wi = 0LL;
    /* pass */
    while ((wi < fns->len)) {
        /* pass */
        HirFunction* hf2 = ((HirFunction*)List_ptr_get(fns, wi));
        /* pass */
        hf2->returns_owned = ({ TrStr _dkt_t435 = (List_TrStr_get(keys, wi)); __auto_type _wr = (((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(_dkt_t435)))); _tr_str_release(_dkt_t435); _wr; });
        /* pass */
        wi = (wi + 1LL);
    }
    List_TrStr_free(keys);
    List_ptr_free_obj(fns, _trdrop_HirFunction);
}

__attribute__((hot)) void Sema_compute_param_ownership(Sema* self, HirProgram* hp) {
    /* pass */
    List_TrStr* keys = (void*)List_TrStr_new();
    /* pass */
    List_ptr* fns = (void*)List_ptr_new();
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < hp->functions->len)) {
        /* pass */
        List_TrStr_append(keys, ((HirFunction*)List_ptr_get(hp->functions, fi))->name);
        /* pass */
        List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(hp->functions, fi))));
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < hp->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < c->methods->len)) {
            /* pass */
            ({ TrStr _at_t436 = (({ TrStr _cl = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (((HirFunction*)List_ptr_get(c->methods, mi))->name)); _tr_str_release(_cl); _cres; })); List_TrStr_append(keys, _at_t436); _tr_str_release(_at_t436); });
            /* pass */
            List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(c->methods, mi))));
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    long long eni = 0LL;
    /* pass */
    while ((eni < hp->enums->len)) {
        /* pass */
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, eni));
        /* pass */
        long long emi = 0LL;
        /* pass */
        while ((emi < en->methods->len)) {
            /* pass */
            ({ TrStr _at_t437 = (({ TrStr _cl = (_tr_strx_concatv((en->name), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (((HirFunction*)List_ptr_get(en->methods, emi))->name)); _tr_str_release(_cl); _cres; })); List_TrStr_append(keys, _at_t437); _tr_str_release(_at_t437); });
            /* pass */
            List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(en->methods, emi))));
            /* pass */
            emi = (emi + 1LL);
        }
        /* pass */
        eni = (eni + 1LL);
    }
    /* pass */
    long long ii = 0LL;
    /* pass */
    while ((ii < fns->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(fns, ii));
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < f->params->len)) {
            /* pass */
            if ((!_tr_str_eqv((((HirParam*)List_ptr_get(f->params, pi))->name), (_tr_str_lit_len("self", 4LL))))) {
                /* pass */
                ({ TrStr _dkt_t438 = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(keys, ii)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("#", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); _tr_dict_set(self->fn_param_consumes, _tr_strz(_dkt_t438), false); _tr_str_release(_dkt_t438); });
            }
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        ii = (ii + 1LL);
    }
    /* pass */
    bool changed = true;
    /* pass */
    while (changed) {
        /* pass */
        changed = false;
        /* pass */
        long long xi = 0LL;
        /* pass */
        while ((xi < fns->len)) {
            /* pass */
            HirFunction* f = ((HirFunction*)List_ptr_get(fns, xi));
            /* pass */
            long long pi = 0LL;
            /* pass */
            while ((pi < f->params->len)) {
                /* pass */
                TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pi))->name);
                /* pass */
                TrStr pk = ({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(keys, xi)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("#", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                /* pass */
                if (((!_tr_str_eqv((pn), (_tr_str_lit_len("self", 4LL)))) && _tr_dict_contains(self->fn_param_consumes, _tr_strz(pk)))) {
                    /* pass */
                    if ((!((bool)(uintptr_t)_tr_dict_get(self->fn_param_consumes, _tr_strz(pk))))) {
                        /* pass */
                        if (Sema__param_consumed_in_block(self, pn, f->body)) {
                            /* pass */
                            _tr_dict_set(self->fn_param_consumes, _tr_strz(pk), true);
                            /* pass */
                            changed = true;
                        }
                    }
                }
                /* pass */
                pi = (pi + 1LL);
                _tr_str_release(pn);
                _tr_str_release(pk);
            }
            /* pass */
            xi = (xi + 1LL);
        }
    }
    List_TrStr_free(keys);
    List_ptr_free_obj(fns, _trdrop_HirFunction);
}

__attribute__((hot)) bool Sema__is_pure_scalar(Sema* self, TrStr tn) {
    /* pass */
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("void", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((tn), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i64", 3LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("u8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("usize", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("f32", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("f64", 3LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__pc_owns_return(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return true;
    }
    /* pass */
    return (!Sema__is_pure_scalar(self, hir_expr_type(e)->name));
}

__attribute__((hot)) void Sema_apply_borrow_drops(Sema* self, HirProgram* hp) {
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < hp->functions->len)) {
        /* pass */
        Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(hp->functions, fi))->body);
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < hp->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < c->methods->len)) {
            /* pass */
            Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(c->methods, mi))->body);
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    long long eni = 0LL;
    /* pass */
    while ((eni < hp->enums->len)) {
        /* pass */
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, eni));
        /* pass */
        long long emi = 0LL;
        /* pass */
        while ((emi < en->methods->len)) {
            /* pass */
            Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(en->methods, emi))->body);
            /* pass */
            emi = (emi + 1LL);
        }
        /* pass */
        eni = (eni + 1LL);
    }
}

__attribute__((hot)) void Sema__borrow_drops_block(Sema* self, HirBlock* b) {
    /* pass */
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        Sema__borrow_drops_recurse_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr* add_names = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* add_cls = (void*)List_TrStr_new();
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        __auto_type _t439 = (*((HirStmt*)List_ptr_get(b->stmts, i)));
        if (_t439.tag == HirStmt_SLet) {
            __auto_type nm = _t439.data.SLet.name;
__auto_type _own = _t439.data.SLet.ownership;
__auto_type _mut2 = _t439.data.SLet.is_mut;
__auto_type _cst = _t439.data.SLet.is_const;
__auto_type _shr = _t439.data.SLet.is_shared;
__auto_type _ty = _t439.data.SLet.ty;
__auto_type val = _t439.data.SLet.val;
            /* pass */
            TrStr cn = _tr_str_retain(_ty->name);
            /* pass */
            if ((Sema__is_owned_droppable_class(self, cn) && Sema__is_fresh_owned_producer(self, val, cn))) {
                /* pass */
                if ((!Sema__block_has_drop_or_reassign(self, b, nm))) {
                    /* pass */
                    if ((!Sema__param_consumed_in_block(self, nm, b))) {
                        /* pass */
                        List_TrStr_append(add_names, nm);
                        /* pass */
                        List_TrStr_append(add_cls, cn);
                    }
                }
            }
            _tr_str_release(cn);
        } else if (1) {
            __auto_type _ = _t439;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < add_names->len)) {
        /* pass */
        ({ TrStr _at_t440 = (List_TrStr_get(add_names, ai)); TrStr _at_t441 = (List_TrStr_get(add_cls, ai)); Sema__insert_drop_before_terminator(self, b, _at_t440, _at_t441); _tr_str_release(_at_t440); _tr_str_release(_at_t441); });
        /* pass */
        ai = (ai + 1LL);
    }
    List_TrStr_free(add_names);
    List_TrStr_free(add_cls);
}

__attribute__((hot)) void Sema__borrow_drops_recurse_stmt(Sema* self, HirStmt* sp) {
    /* pass */
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t442 = (*sp);
    if (_t442.tag == HirStmt_SIf) {
        __auto_type tb = _t442.data.SIf.then_b;
__auto_type eb = _t442.data.SIf.else_b;
        /* pass */
        Sema__borrow_drops_block(self, tb);
        /* pass */
        Sema__borrow_drops_block(self, eb);
    } else if (_t442.tag == HirStmt_SWhile) {
        __auto_type wb = _t442.data.SWhile.body;
        Sema__borrow_drops_block(self, wb);
    } else if (_t442.tag == HirStmt_SFor) {
        __auto_type fb = _t442.data.SFor.body;
        Sema__borrow_drops_block(self, fb);
    } else if (_t442.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t442.data.SForUnpack.body;
        Sema__borrow_drops_block(self, fub);
    } else if (_t442.tag == HirStmt_SMatch) {
        __auto_type arms = _t442.data.SMatch.arms;
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < arms->len)) {
            /* pass */
            Sema__borrow_drops_block(self, ((HirMatchArm*)List_ptr_get(arms, ai))->body);
            /* pass */
            ai = (ai + 1LL);
        }
    } else if (_t442.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t442.data.SUnsafe.body;
        Sema__borrow_drops_block(self, ub);
    } else if (_t442.tag == HirStmt_SWith) {
        __auto_type wb = _t442.data.SWith.body;
        Sema__borrow_drops_block(self, wb);
    } else if (_t442.tag == HirStmt_STaskGroup) {
        __auto_type tb = _t442.data.STaskGroup.body;
        Sema__borrow_drops_block(self, tb);
    } else if (_t442.tag == HirStmt_STry) {
        __auto_type tb = _t442.data.STry.try_body;
__auto_type catches = _t442.data.STry.catches;
__auto_type fb = _t442.data.STry.finally_b;
        /* pass */
        Sema__borrow_drops_block(self, tb);
        /* pass */
        Sema__borrow_drops_block(self, fb);
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < catches->len)) {
            /* pass */
            Sema__borrow_drops_block(self, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body);
            /* pass */
            ci = (ci + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t442;
        /* pass */
    }
}

__attribute__((hot)) bool Sema__is_fresh_owned_producer(Sema* self, HirExpr* val, TrStr cn) {
    /* pass */
    if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(val)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t443 = (*val);
    if (_t443.tag == HirExpr_ECall) {
        __auto_type callee = _t443.data.ECall.callee;
        /* pass */
        __auto_type _t444 = (*callee);
        if (_t444.tag == HirExpr_EIdent) {
            __auto_type fnm = _t444.data.EIdent.name;
            /* pass */
            if (_tr_str_eqv((fnm), (cn))) {
                /* pass */
                return true;
            }
            /* pass */
            if ((_tr_dict_contains(self->fn_ret_owned, _tr_strz(fnm)) && ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(fnm))))) {
                /* pass */
                return true;
            }
        } else if (1) {
            __auto_type _ = _t444;
            /* pass */
        }
    } else if (_t443.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t443.data.EMethodCall.obj;
__auto_type m = _t443.data.EMethodCall.method;
        /* pass */
        if ((_tr_str_eqv((m), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("new", 3LL))))) {
            /* pass */
            return true;
        }
        /* pass */
        TrStr mkey = ({ TrStr _cl = (_tr_strx_concatv((hir_expr_type(obj)->name), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (m)); _tr_str_release(_cl); _cres; });
        /* pass */
        if ((_tr_dict_contains(self->fn_ret_owned, _tr_strz(mkey)) && ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(mkey))))) {
            /* pass */
            _tr_str_release(mkey);
            return true;
        }
        _tr_str_release(mkey);
    } else if (1) {
        __auto_type _ = _t443;
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__is_owned_droppable_class(Sema* self, TrStr cn) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cn)))) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cn)))->generics->len > 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if (Sema_class_method_exists(self, cn, _tr_str_lit_len("free", 4LL))) {
        /* pass */
        return true;
    }
    /* pass */
    return Sema__is_rc_class(self, cn);
}

__attribute__((hot)) bool Sema__block_has_drop_or_reassign(Sema* self, HirBlock* b, TrStr nm) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        __auto_type _t445 = (*((HirStmt*)List_ptr_get(b->stmts, i)));
        if (_t445.tag == HirStmt_SAutoDrop) {
            __auto_type dn = _t445.data.SAutoDrop.name;
            /* pass */
            if (_tr_str_eqv((dn), (nm))) {
                /* pass */
                return true;
            }
        } else if (_t445.tag == HirStmt_SFree) {
            __auto_type dn = _t445.data.SFree.name;
            /* pass */
            if (_tr_str_eqv((dn), (nm))) {
                /* pass */
                return true;
            }
        } else if (_t445.tag == HirStmt_SAssign) {
            __auto_type tgt = _t445.data.SAssign.target;
            /* pass */
            __auto_type _t446 = (*tgt);
            if (_t446.tag == HirExpr_EIdent) {
                __auto_type tn2 = _t446.data.EIdent.name;
                /* pass */
                if (_tr_str_eqv((tn2), (nm))) {
                    /* pass */
                    return true;
                }
            } else if (1) {
                __auto_type _ = _t446;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t445;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema__insert_drop_before_terminator(Sema* self, HirBlock* b, TrStr nm, TrStr cn) {
    /* pass */
    HirStmt* drop = box_hirstmt(HirStmt_ctor_SAutoDrop(nm, cn));
    /* pass */
    long long n = b->stmts->len;
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        List_ptr_append(b->stmts, drop);
        /* pass */
        return;
    }
    /* pass */
    HirStmt* last = ((HirStmt*)List_ptr_get(b->stmts, (n - 1LL)));
    /* pass */
    bool is_term = false;
    /* pass */
    HirExpr* term_val = (HirExpr*)(0LL);
    /* pass */
    __auto_type _t447 = (*last);
    if (_t447.tag == HirStmt_SReturn) {
        __auto_type rv = _t447.data.SReturn.val;
        /* pass */
        is_term = true;
        /* pass */
        term_val = rv;
    } else if (_t447.tag == HirStmt_SRaise) {
        __auto_type rv2 = _t447.data.SRaise.val;
        /* pass */
        is_term = true;
        /* pass */
        term_val = rv2;
    } else if (_t447.tag == HirStmt_SBreak) {
        __auto_type bv3 = _t447.data.SBreak.val;
        /* pass */
        is_term = true;
        /* pass */
        term_val = bv3;
    } else if (_t447.tag == HirStmt_SContinue) {
        is_term = true;
    } else if (1) {
        __auto_type _ = _t447;
        /* pass */
    }
    /* pass */
    if (is_term) {
        /* pass */
        List_TrStr* term_refs = (void*)List_TrStr_new();
        /* pass */
        if ((((unsigned long long)(term_val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema_collect_idents(self, term_val, term_refs);
        }
        /* pass */
        bool nm_referenced = false;
        /* pass */
        long long tri = 0LL;
        /* pass */
        while ((tri < term_refs->len)) {
            /* pass */
            if (_tr_str_eqv((List_TrStr_get(term_refs, tri)), (nm))) {
                /* pass */
                nm_referenced = true;
            }
            /* pass */
            tri = (tri + 1LL);
        }
        /* pass */
        if (nm_referenced) {
            /* pass */
            return;
        }
        /* pass */
        List_ptr_remove(b->stmts, (n - 1LL));
        /* pass */
        List_ptr_append(b->stmts, drop);
        /* pass */
        List_ptr_append(b->stmts, last);
    } else {
        /* pass */
        List_ptr_append(b->stmts, drop);
    }
}

__attribute__((hot)) bool Sema__pc_is_ident(Sema* self, HirExpr* ep, TrStr name) {
    /* pass */
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t448 = (*ep);
    if (_t448.tag == HirExpr_EIdent) {
        __auto_type n = _t448.data.EIdent.name;
        return _tr_str_eqv((n), (name));
    } else if (1) {
        __auto_type _ = _t448;
        return false;
    }
}

__attribute__((hot)) TrStr Sema__pc_callee_name(Sema* self, HirExpr* callee) {
    /* pass */
    if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    __auto_type _t449 = (*callee);
    if (_t449.tag == HirExpr_EIdent) {
        __auto_type n = _t449.data.EIdent.name;
        return _tr_str_retain(n);
    } else if (1) {
        __auto_type _ = _t449;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool Sema__pc_stores_method(Sema* self, TrStr m) {
    /* pass */
    return ((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL))));
}

__attribute__((hot)) bool Sema__pc_is_store_target(Sema* self, HirExpr* tgt) {
    /* pass */
    if ((((unsigned long long)(tgt)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t450 = (*tgt);
    if (_t450.tag == HirExpr_EPropAccess) {
        return true;
    } else if (_t450.tag == HirExpr_EIndex) {
        return true;
    } else if (1) {
        __auto_type _ = _t450;
        return false;
    }
}

__attribute__((hot)) bool Sema__pc_refs(Sema* self, HirExpr* ep, TrStr name) {
    /* pass */
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t451 = (*ep);
    if (_t451.tag == HirExpr_EIdent) {
        __auto_type n = _t451.data.EIdent.name;
        return _tr_str_eqv((n), (name));
    } else if (_t451.tag == HirExpr_EBinOp) {
        __auto_type l = _t451.data.EBinOp.left;
__auto_type r = _t451.data.EBinOp.right;
        return (Sema__pc_refs(self, l, name) || Sema__pc_refs(self, r, name));
    } else if (_t451.tag == HirExpr_EUnaryOp) {
        __auto_type e = _t451.data.EUnaryOp.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t451.tag == HirExpr_ECast) {
        __auto_type e = _t451.data.ECast.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t451.tag == HirExpr_EPropAccess) {
        __auto_type o = _t451.data.EPropAccess.obj;
        return Sema__pc_refs(self, o, name);
    } else if (_t451.tag == HirExpr_EIndex) {
        __auto_type o = _t451.data.EIndex.obj;
__auto_type ix = _t451.data.EIndex._tr_v_index;
        return (Sema__pc_refs(self, o, name) || Sema__pc_refs(self, ix, name));
    } else if (_t451.tag == HirExpr_ECall) {
        __auto_type cl = _t451.data.ECall.callee;
__auto_type args = _t451.data.ECall.args;
        return (Sema__pc_refs(self, cl, name) || Sema__pc_anyrefs(self, args, name));
    } else if (_t451.tag == HirExpr_EMethodCall) {
        __auto_type o = _t451.data.EMethodCall.obj;
__auto_type args = _t451.data.EMethodCall.args;
        return (Sema__pc_refs(self, o, name) || Sema__pc_anyrefs(self, args, name));
    } else if (_t451.tag == HirExpr_EList) {
        __auto_type items = _t451.data.EList.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t451.tag == HirExpr_ESet) {
        __auto_type items = _t451.data.ESet.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t451.tag == HirExpr_ETuple) {
        __auto_type items = _t451.data.ETuple.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t451.tag == HirExpr_EDict) {
        __auto_type dks = _t451.data.EDict.keys;
__auto_type dvs = _t451.data.EDict.vals;
        return (Sema__pc_anyrefs(self, dks, name) || Sema__pc_anyrefs(self, dvs, name));
    } else if (_t451.tag == HirExpr_ETryExpr) {
        __auto_type e = _t451.data.ETryExpr.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t451.tag == HirExpr_EAwait) {
        __auto_type e = _t451.data.EAwait.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t451.tag == HirExpr_EIfElse) {
        __auto_type cnd = _t451.data.EIfElse.cond;
__auto_type te = _t451.data.EIfElse.then_e;
__auto_type ee = _t451.data.EIfElse.else_e;
        return ((Sema__pc_refs(self, cnd, name) || Sema__pc_refs(self, te, name)) || Sema__pc_refs(self, ee, name));
    } else if (_t451.tag == HirExpr_ESlice) {
        __auto_type sa = _t451.data.ESlice.start;
__auto_type sb = _t451.data.ESlice.stop;
__auto_type sc = _t451.data.ESlice.step;
        return ((Sema__pc_refs(self, sa, name) || Sema__pc_refs(self, sb, name)) || Sema__pc_refs(self, sc, name));
    } else if (_t451.tag == HirExpr_ERange) {
        __auto_type ra = _t451.data.ERange.start;
__auto_type rb = _t451.data.ERange.end;
        return (Sema__pc_refs(self, ra, name) || Sema__pc_refs(self, rb, name));
    } else if (1) {
        __auto_type _ = _t451;
        return false;
    }
}

__attribute__((hot)) bool Sema__pc_anyrefs(Sema* self, List_ptr* args, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < args->len)) {
        /* pass */
        if (Sema__pc_refs(self, ((HirExpr*)List_ptr_get(args, i)), name)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__param_consumed_in_block(Sema* self, TrStr pname, HirBlock* b) {
    /* pass */
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        if (Sema__param_consumed_in_stmt(self, pname, ((HirStmt*)List_ptr_get(b->stmts, i)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__param_consumed_in_stmt(Sema* self, TrStr pname, HirStmt* sp) {
    /* pass */
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t452 = (*sp);
    if (_t452.tag == HirStmt_SReturn) {
        __auto_type e = _t452.data.SReturn.val;
        /* pass */
        if ((!Sema__pc_owns_return(self, e))) {
            /* pass */
            return false;
        }
        /* pass */
        return Sema__pc_refs(self, e, pname);
    } else if (_t452.tag == HirStmt_SRaise) {
        __auto_type e = _t452.data.SRaise.val;
        /* pass */
        if ((!Sema__pc_owns_return(self, e))) {
            /* pass */
            return false;
        }
        /* pass */
        return Sema__pc_refs(self, e, pname);
    } else if (_t452.tag == HirStmt_SFree) {
        __auto_type nm = _t452.data.SFree.name;
        return _tr_str_eqv((nm), (pname));
    } else if (_t452.tag == HirStmt_SSpawn) {
        __auto_type e = _t452.data.SSpawn.expr;
        return Sema__pc_refs(self, e, pname);
    } else if (_t452.tag == HirStmt_SAssign) {
        __auto_type tgt = _t452.data.SAssign.target;
__auto_type val = _t452.data.SAssign.val;
        /* pass */
        if ((Sema__pc_is_store_target(self, tgt) && Sema__pc_refs(self, val, pname))) {
            /* pass */
            return true;
        }
        /* pass */
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t452.tag == HirStmt_SLet) {
        __auto_type val = _t452.data.SLet.val;
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t452.tag == HirStmt_SMultiLet) {
        __auto_type val = _t452.data.SMultiLet.val;
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t452.tag == HirStmt_SExpr) {
        __auto_type e = _t452.data.SExpr.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t452.tag == HirStmt_SIf) {
        __auto_type tb = _t452.data.SIf.then_b;
__auto_type eb = _t452.data.SIf.else_b;
        return (Sema__param_consumed_in_block(self, pname, tb) || Sema__param_consumed_in_block(self, pname, eb));
    } else if (_t452.tag == HirStmt_SWhile) {
        __auto_type wb = _t452.data.SWhile.body;
        return Sema__param_consumed_in_block(self, pname, wb);
    } else if (_t452.tag == HirStmt_SFor) {
        __auto_type fb = _t452.data.SFor.body;
        return Sema__param_consumed_in_block(self, pname, fb);
    } else if (_t452.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t452.data.SForUnpack.body;
        return Sema__param_consumed_in_block(self, pname, fub);
    } else if (_t452.tag == HirStmt_SMatch) {
        __auto_type arms = _t452.data.SMatch.arms;
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < arms->len)) {
            /* pass */
            if (Sema__param_consumed_in_block(self, pname, ((HirMatchArm*)List_ptr_get(arms, ai))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        return false;
    } else if (_t452.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t452.data.SUnsafe.body;
        return Sema__param_consumed_in_block(self, pname, ub);
    } else if (_t452.tag == HirStmt_SWith) {
        __auto_type wb = _t452.data.SWith.body;
        return Sema__param_consumed_in_block(self, pname, wb);
    } else if (_t452.tag == HirStmt_STaskGroup) {
        __auto_type tb = _t452.data.STaskGroup.body;
        return Sema__param_consumed_in_block(self, pname, tb);
    } else if (_t452.tag == HirStmt_SDefer) {
        __auto_type ds = _t452.data.SDefer.stmt;
        return Sema__param_consumed_in_stmt(self, pname, ds);
    } else if (_t452.tag == HirStmt_STry) {
        __auto_type tb = _t452.data.STry.try_body;
__auto_type catches = _t452.data.STry.catches;
__auto_type fb = _t452.data.STry.finally_b;
        /* pass */
        if (Sema__param_consumed_in_block(self, pname, tb)) {
            /* pass */
            return true;
        }
        /* pass */
        if (Sema__param_consumed_in_block(self, pname, fb)) {
            /* pass */
            return true;
        }
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < catches->len)) {
            /* pass */
            if (Sema__param_consumed_in_block(self, pname, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            ci = (ci + 1LL);
        }
        /* pass */
        return false;
    } else if (1) {
        __auto_type _ = _t452;
        return false;
    }
}

__attribute__((hot)) bool Sema__param_consumed_in_expr(Sema* self, TrStr pname, HirExpr* ep) {
    /* pass */
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t453 = (*ep);
    if (_t453.tag == HirExpr_ECall) {
        __auto_type callee = _t453.data.ECall.callee;
__auto_type args = _t453.data.ECall.args;
        /* pass */
        TrStr gname = Sema__pc_callee_name(self, callee);
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < args->len)) {
            /* pass */
            if (Sema__pc_is_ident(self, ((HirExpr*)List_ptr_get(args, j)), pname)) {
                /* pass */
                if (_tr_str_eqv((gname), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    _tr_str_release(gname);
                    return true;
                }
                /* pass */
                TrStr ck = ({ TrStr _cl = (_tr_strx_concatv((gname), (_tr_str_lit_len("#", 1LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(j)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                /* pass */
                if ((!_tr_dict_contains(self->fn_param_consumes, _tr_strz(ck)))) {
                    /* pass */
                    _tr_str_release(gname);
                    _tr_str_release(ck);
                    return true;
                }
                /* pass */
                if (((bool)(uintptr_t)_tr_dict_get(self->fn_param_consumes, _tr_strz(ck)))) {
                    /* pass */
                    _tr_str_release(gname);
                    _tr_str_release(ck);
                    return true;
                }
            } else {
                /* pass */
                if (Sema__param_consumed_in_expr(self, pname, ((HirExpr*)List_ptr_get(args, j)))) {
                    /* pass */
                    _tr_str_release(gname);
                    return true;
                }
            }
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        _tr_str_release(gname);
        return Sema__param_consumed_in_expr(self, pname, callee);
    } else if (_t453.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t453.data.EMethodCall.obj;
__auto_type method = _t453.data.EMethodCall.method;
__auto_type args = _t453.data.EMethodCall.args;
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL))) && Sema__pc_is_ident(self, obj, pname))) {
            /* pass */
            return true;
        }
        /* pass */
        long long j2 = 0LL;
        /* pass */
        while ((j2 < args->len)) {
            /* pass */
            if (Sema__pc_is_ident(self, ((HirExpr*)List_ptr_get(args, j2)), pname)) {
                /* pass */
                return true;
            }
            /* pass */
            if (Sema__param_consumed_in_expr(self, pname, ((HirExpr*)List_ptr_get(args, j2)))) {
                /* pass */
                return true;
            }
            /* pass */
            j2 = (j2 + 1LL);
        }
        /* pass */
        return Sema__param_consumed_in_expr(self, pname, obj);
    } else if (_t453.tag == HirExpr_EList) {
        __auto_type items = _t453.data.EList.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t453.tag == HirExpr_ESet) {
        __auto_type items = _t453.data.ESet.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t453.tag == HirExpr_ETuple) {
        __auto_type items = _t453.data.ETuple.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t453.tag == HirExpr_EDict) {
        __auto_type dks = _t453.data.EDict.keys;
__auto_type dvs = _t453.data.EDict.vals;
        return (Sema__pc_anyrefs(self, dks, pname) || Sema__pc_anyrefs(self, dvs, pname));
    } else if (_t453.tag == HirExpr_EClosure) {
        __auto_type caps = _t453.data.EClosure.captures;
        /* pass */
        long long cj = 0LL;
        /* pass */
        while ((cj < caps->len)) {
            /* pass */
            if (_tr_str_eqv((((HirParam*)List_ptr_get(caps, cj))->name), (pname))) {
                /* pass */
                return true;
            }
            /* pass */
            cj = (cj + 1LL);
        }
        /* pass */
        return false;
    } else if (_t453.tag == HirExpr_EBinOp) {
        __auto_type l = _t453.data.EBinOp.left;
__auto_type r = _t453.data.EBinOp.right;
        return (Sema__param_consumed_in_expr(self, pname, l) || Sema__param_consumed_in_expr(self, pname, r));
    } else if (_t453.tag == HirExpr_EUnaryOp) {
        __auto_type e = _t453.data.EUnaryOp.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t453.tag == HirExpr_ECast) {
        __auto_type e = _t453.data.ECast.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t453.tag == HirExpr_EPropAccess) {
        __auto_type o = _t453.data.EPropAccess.obj;
        return Sema__param_consumed_in_expr(self, pname, o);
    } else if (_t453.tag == HirExpr_EIndex) {
        __auto_type o = _t453.data.EIndex.obj;
__auto_type ix = _t453.data.EIndex._tr_v_index;
        return (Sema__param_consumed_in_expr(self, pname, o) || Sema__param_consumed_in_expr(self, pname, ix));
    } else if (_t453.tag == HirExpr_EIfElse) {
        __auto_type cnd = _t453.data.EIfElse.cond;
__auto_type te = _t453.data.EIfElse.then_e;
__auto_type ee = _t453.data.EIfElse.else_e;
        return ((Sema__param_consumed_in_expr(self, pname, cnd) || Sema__param_consumed_in_expr(self, pname, te)) || Sema__param_consumed_in_expr(self, pname, ee));
    } else if (_t453.tag == HirExpr_EAwait) {
        __auto_type e = _t453.data.EAwait.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t453.tag == HirExpr_ETryExpr) {
        __auto_type e = _t453.data.ETryExpr.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (1) {
        __auto_type _ = _t453;
        return false;
    }
}

__attribute__((hot)) bool Sema__fn_ret_is_heap_class(Sema* self, HirFunction* f) {
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr rn = _tr_str_retain(f->ret_ty->name);
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(rn)))) {
        /* pass */
        _tr_str_release(rn);
        return false;
    }
    /* pass */
    if (((((((((((((((_tr_str_eqv((rn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((rn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Box", 3LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("StringObj", 9LL))))) {
        /* pass */
        _tr_str_release(rn);
        return false;
    }
    /* pass */
    ClassDef* cinfo = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(rn)));
    /* pass */
    if ((!cinfo->is_class)) {
        /* pass */
        _tr_str_release(rn);
        return false;
    }
    /* pass */
    _tr_str_release(rn);
    return true;
}

__attribute__((hot)) void Sema__collect_returns(Sema* self, HirBlock* b, List_ptr* out) {
    /* pass */
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        Sema__collect_returns_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)), out);
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema__collect_returns_stmt(Sema* self, HirStmt* sp, List_ptr* out) {
    /* pass */
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t454 = (*sp);
    if (_t454.tag == HirStmt_SReturn) {
        __auto_type re = _t454.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(re)) != ((unsigned long long)(0LL)))) {
            /* pass */
            List_ptr_append(out, re);
        }
    } else if (_t454.tag == HirStmt_SIf) {
        __auto_type tb = _t454.data.SIf.then_b;
__auto_type eb = _t454.data.SIf.else_b;
        /* pass */
        Sema__collect_returns(self, tb, out);
        /* pass */
        Sema__collect_returns(self, eb, out);
    } else if (_t454.tag == HirStmt_SWhile) {
        __auto_type wb = _t454.data.SWhile.body;
        Sema__collect_returns(self, wb, out);
    } else if (_t454.tag == HirStmt_SFor) {
        __auto_type fb = _t454.data.SFor.body;
        Sema__collect_returns(self, fb, out);
    } else if (_t454.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t454.data.SForUnpack.body;
        Sema__collect_returns(self, fub, out);
    } else if (_t454.tag == HirStmt_SMatch) {
        __auto_type marms = _t454.data.SMatch.arms;
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < marms->len)) {
            /* pass */
            Sema__collect_returns(self, ((HirMatchArm*)List_ptr_get(marms, ai))->body, out);
            /* pass */
            ai = (ai + 1LL);
        }
    } else if (_t454.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t454.data.SUnsafe.body;
        Sema__collect_returns(self, ub, out);
    } else if (_t454.tag == HirStmt_SWith) {
        __auto_type wb2 = _t454.data.SWith.body;
        Sema__collect_returns(self, wb2, out);
    } else if (_t454.tag == HirStmt_STaskGroup) {
        __auto_type tgb = _t454.data.STaskGroup.body;
        Sema__collect_returns(self, tgb, out);
    } else if (_t454.tag == HirStmt_SGpuBlock) {
        __auto_type gb = _t454.data.SGpuBlock.body;
        Sema__collect_returns(self, gb, out);
    } else if (_t454.tag == HirStmt_SDefer) {
        __auto_type ds = _t454.data.SDefer.stmt;
        Sema__collect_returns_stmt(self, ds, out);
    } else if (_t454.tag == HirStmt_STry) {
        __auto_type tb2 = _t454.data.STry.try_body;
__auto_type catches = _t454.data.STry.catches;
__auto_type fb2 = _t454.data.STry.finally_b;
        /* pass */
        Sema__collect_returns(self, tb2, out);
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < catches->len)) {
            /* pass */
            HirCatchClause** cc = ((HirCatchClause**)List_ptr_get(catches, ci));
            /* pass */
            if ((((unsigned long long)(cc)) != ((unsigned long long)(0LL)))) {
                /* pass */
                Sema__collect_returns(self, (*cc)->body, out);
            }
            /* pass */
            ci = (ci + 1LL);
        }
        /* pass */
        Sema__collect_returns(self, fb2, out);
    } else if (1) {
        __auto_type _ = _t454;
        /* pass */
    }
}

__attribute__((hot)) bool Sema__owned_of(Sema* self, TrStr key) {
    /* pass */
    if (_tr_dict_contains(self->fn_ret_owned, _tr_strz(key))) {
        /* pass */
        return ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(key)));
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__ret_yields_owned(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t455 = (*e);
    if (_t455.tag == HirExpr_EIdent) {
        return true;
    } else if (_t455.tag == HirExpr_EPropAccess) {
        return true;
    } else if (_t455.tag == HirExpr_ECall) {
        __auto_type callee = _t455.data.ECall.callee;
        /* pass */
        if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        __auto_type _t456 = (*callee);
        if (_t456.tag == HirExpr_EIdent) {
            __auto_type cn = _t456.data.EIdent.name;
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(cn))) {
                /* pass */
                return true;
            }
            /* pass */
            return Sema__owned_of(self, cn);
        } else if (1) {
            __auto_type _ = _t456;
            return false;
        }
    } else if (_t455.tag == HirExpr_EMethodCall) {
        __auto_type _tr_v_recv = _t455.data.EMethodCall.obj;
__auto_type m = _t455.data.EMethodCall.method;
        /* pass */
        TrStr rt = _tr_str_retain(hir_expr_type(_tr_v_recv)->name);
        /* pass */
        if ((((((_tr_str_eqv((rt), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((rt), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((rt), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((rt), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((rt), (_tr_str_lit_len("Set", 3LL)))) && ((_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("last", 4LL)))))) {
            /* pass */
            _tr_str_release(rt);
            return false;
        }
        /* pass */
        __auto_type _t457 = (*_tr_v_recv);
        if (_t457.tag == HirExpr_EIdent) {
            __auto_type rn = _t457.data.EIdent.name;
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(rn))) {
                /* pass */
                TrStr _strtmp_t458 = _tr_str_retain(rn);
                _tr_str_release(rt);
                rt = _strtmp_t458;
            }
        } else if (1) {
            __auto_type _ = _t457;
            /* pass */
        }
        /* pass */
        return ({ TrStr _at_t459 = (({ TrStr _cl = (_tr_strx_concatv((rt), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (m)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (Sema__owned_of(self, _at_t459)); _tr_str_release(_at_t459); _wr; });
    } else if (_t455.tag == HirExpr_EIndex) {
        return false;
    } else if (_t455.tag == HirExpr_EIfElse) {
        __auto_type a = _t455.data.EIfElse.then_e;
__auto_type b2 = _t455.data.EIfElse.else_e;
        /* pass */
        return (Sema__ret_yields_owned(self, a) && Sema__ret_yields_owned(self, b2));
    } else if (1) {
        __auto_type _ = _t455;
        return false;
    }
}

__attribute__((hot)) bool Sema__is_type_param_in_scope(Sema* self, TrStr name) {
    /* pass */
    if ((_tr_strlen(_tr_strz(name)) == 1LL)) {
        /* pass */
        return true;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->current_func_generics->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(self->current_func_generics, i)), (name))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (((!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->classes, _tr_strz(self->current_class_name)))) {
        /* pass */
        ClassDef* ccls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < ccls->generics->len)) {
            /* pass */
            if (_tr_str_eqv((List_TrStr_get(ccls->generics, ci)), (name))) {
                /* pass */
                return true;
            }
            /* pass */
            ci = (ci + 1LL);
        }
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__type_satisfies_bound(Sema* self, TrStr type_name, TrStr iface_name) {
    /* pass */
    if ((!_tr_dict_contains(self->interfaces, _tr_strz(iface_name)))) {
        /* pass */
        return true;
    }
    /* pass */
    if (Sema__is_type_param_in_scope(self, type_name)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(type_name)))) {
        /* pass */
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(type_name)));
    /* pass */
    long long ii = 0LL;
    /* pass */
    while ((ii < cls->iface_names->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(cls->iface_names, ii)), (iface_name))) {
            /* pass */
            return true;
        }
        /* pass */
        ii = (ii + 1LL);
    }
    /* pass */
    InterfaceDef* idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(iface_name)));
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < idef->methods->len)) {
        /* pass */
        TrStr imeth_nm = _tr_str_retain(((FunctionDef*)List_ptr_get(idef->methods, mi))->name);
        /* pass */
        bool found = false;
        /* pass */
        long long cmi = 0LL;
        /* pass */
        while ((cmi < cls->methods->len)) {
            /* pass */
            if (_tr_str_eqv((((FunctionDef*)List_ptr_get(cls->methods, cmi))->name), (imeth_nm))) {
                /* pass */
                found = true;
            }
            /* pass */
            cmi = (cmi + 1LL);
        }
        /* pass */
        if ((!found)) {
            /* pass */
            _tr_str_release(imeth_nm);
            return false;
        }
        /* pass */
        mi = (mi + 1LL);
        _tr_str_release(imeth_nm);
    }
    /* pass */
    return true;
}

__attribute__((hot)) AstType** Sema__resolve_generic_bound_method_ret(Sema* self, TrStr pname, TrStr method) {
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < self->current_func_constraints->len)) {
        /* pass */
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(self->current_func_constraints, ci));
        /* pass */
        if (_tr_str_eqv((gc->target), (pname))) {
            /* pass */
            long long bi = 0LL;
            /* pass */
            while ((bi < gc->bounds->len)) {
                /* pass */
                AstType* bound_ty = (*((AstType**)List_ptr_get(gc->bounds, bi)));
                /* pass */
                if (_tr_dict_contains(self->interfaces, _tr_strz(bound_ty->name))) {
                    /* pass */
                    InterfaceDef* idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(bound_ty->name)));
                    /* pass */
                    long long mi = 0LL;
                    /* pass */
                    while ((mi < idef->methods->len)) {
                        /* pass */
                        FunctionDef* imdef = ((FunctionDef*)List_ptr_get(idef->methods, mi));
                        /* pass */
                        if ((_tr_str_eqv((imdef->name), (method)) && (((unsigned long long)(imdef->ret_ty)) != ((unsigned long long)(0LL))))) {
                            /* pass */
                            AstType* found_ty = (*imdef->ret_ty);
                            /* pass */
                            if (((idef->generics->len > 0LL) && (bound_ty->args->len > 0LL))) {
                                /* pass */
                                long long gi = 0LL;
                                /* pass */
                                while ((gi < idef->generics->len)) {
                                    /* pass */
                                    if ((_tr_str_eqv((List_TrStr_get(idef->generics, gi)), (found_ty->name)) && (gi < bound_ty->args->len))) {
                                        /* pass */
                                        found_ty = (*((AstType**)List_ptr_get(bound_ty->args, gi)));
                                    }
                                    /* pass */
                                    gi = (gi + 1LL);
                                }
                            }
                            /* pass */
                            return box_asttype(found_ty);
                        }
                        /* pass */
                        mi = (mi + 1LL);
                    }
                }
                /* pass */
                bi = (bi + 1LL);
            }
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    return ((AstType**)(0LL));
}

__attribute__((hot)) void Sema_check_call_bounds(Sema* self, TrStr fname, List_ptr* hargs) {
    /* pass */
    if ((!_tr_dict_contains(self->fn_defs, _tr_strz(fname)))) {
        /* pass */
        return;
    }
    /* pass */
    FunctionDef* fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(fname)));
    /* pass */
    if ((fd->constraints->len == 0LL)) {
        /* pass */
        return;
    }
    /* pass */
    long long sk = 0LL;
    /* pass */
    if (((fd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(fd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
        /* pass */
        sk = 1LL;
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < fd->constraints->len)) {
        /* pass */
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(fd->constraints, ci));
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < fd->params->len)) {
            /* pass */
            AstType** _pty = ((Param*)List_ptr_get(fd->params, pi))->ty;
            /* pass */
            if (((((unsigned long long)(_pty)) != ((unsigned long long)(0LL))) && _tr_str_eqv(((*_pty)->name), (gc->target)))) {
                /* pass */
                long long aidx = (pi - sk);
                /* pass */
                if (((aidx >= 0LL) && (aidx < hargs->len))) {
                    /* pass */
                    TrStr concrete_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(hargs, aidx)))->name);
                    /* pass */
                    long long bi = 0LL;
                    /* pass */
                    while ((bi < gc->bounds->len)) {
                        /* pass */
                        TrStr iface_n = _tr_str_retain((*((AstType**)List_ptr_get(gc->bounds, bi)))->name);
                        /* pass */
                        if ((!Sema__type_satisfies_bound(self, concrete_n, iface_n))) {
                            /* pass */
                            ({ TrStr _at_t460 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[G-1] Type '", 12LL)), (concrete_n))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' does not satisfy the bound '", 30LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' required by type parameter '", 30LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (gc->target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.\n      FIX: implement all methods of interface '", 50LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' on '", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (concrete_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("', or add 'implements ", 22LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t460); _tr_str_release(_at_t460); });
                        }
                        /* pass */
                        bi = (bi + 1LL);
                        _tr_str_release(iface_n);
                    }
                }
            }
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) AstType* Sema__subst_ret_generics(Sema* self, AstType* ty, List_TrStr* generics, List_ptr* concrete) {
    /* pass */
    long long gi = 0LL;
    /* pass */
    while ((gi < generics->len)) {
        /* pass */
        if ((_tr_str_eqv((List_TrStr_get(generics, gi)), (ty->name)) && (gi < concrete->len))) {
            /* pass */
            return (*((AstType**)List_ptr_get(concrete, gi)));
        }
        /* pass */
        gi = (gi + 1LL);
    }
    /* pass */
    if ((ty->args->len == 0LL)) {
        /* pass */
        return ty;
    }
    /* pass */
    AstType* nt = AstType_init(ty->name);
    /* pass */
    nt->array_size = ty->array_size;
    /* pass */
    nt->args = (void*)List_ptr_new();
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < ty->args->len)) {
        /* pass */
        List_ptr_append(nt->args, box_asttype(Sema__subst_ret_generics(self, (*((AstType**)List_ptr_get(ty->args, ai))), generics, concrete)));
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    return nt;
}

__attribute__((hot)) void Sema_check_class_bounds(Sema* self, TrStr cls_name, List_ptr* arg_tys) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
        /* pass */
        return;
    }
    /* pass */
    ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
    /* pass */
    if ((cd->constraints->len == 0LL)) {
        /* pass */
        return;
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < cd->constraints->len)) {
        /* pass */
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(cd->constraints, ci));
        /* pass */
        long long gpi = 0LL;
        /* pass */
        while ((gpi < cd->generics->len)) {
            /* pass */
            if ((_tr_str_eqv((List_TrStr_get(cd->generics, gpi)), (gc->target)) && (gpi < arg_tys->len))) {
                /* pass */
                TrStr concrete_n = _tr_str_retain((*((AstType**)List_ptr_get(arg_tys, gpi)))->name);
                /* pass */
                long long bi = 0LL;
                /* pass */
                while ((bi < gc->bounds->len)) {
                    /* pass */
                    TrStr iface_n = _tr_str_retain((*((AstType**)List_ptr_get(gc->bounds, bi)))->name);
                    /* pass */
                    if ((!Sema__type_satisfies_bound(self, concrete_n, iface_n))) {
                        /* pass */
                        ({ TrStr _at_t461 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[G-1] Type '", 12LL)), (concrete_n))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' does not satisfy the bound '", 30LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' required by type parameter '", 30LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (gc->target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' of class '", 12LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cls_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.\n      FIX: implement all methods of interface '", 50LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' on '", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (concrete_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("', or add 'implements ", 22LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t461); _tr_str_release(_at_t461); });
                    }
                    /* pass */
                    bi = (bi + 1LL);
                    _tr_str_release(iface_n);
                }
            }
            /* pass */
            gpi = (gpi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) void Sema_register_decl(Sema* self, Decl* d) {
    /* pass */
    __auto_type _t462 = (*d);
    if (_t462.tag == Decl_DFunction) {
        __auto_type f = _t462.data.DFunction.func;
        /* pass */
        AstType** _f_ret = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
        /* pass */
        if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            _f_ret = f->ret_ty;
        }
        /* pass */
        if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            AstType* _frt = (*f->ret_ty);
            /* pass */
            if ((_frt->from_regions->len == 1LL)) {
                /* pass */
                long long _fi = 0LL;
                /* pass */
                while ((_fi < f->params->len)) {
                    /* pass */
                    if (_tr_str_eqv((((Param*)List_ptr_get(f->params, _fi))->name), (List_TrStr_get(_frt->from_regions, 0LL)))) {
                        /* pass */
                        _frt->from_index = _fi;
                    }
                    /* pass */
                    _fi = (_fi + 1LL);
                }
                /* pass */
                _f_ret = box_asttype(_frt);
            }
        }
        /* pass */
        AstType** _decl_ret = _f_ret;
        /* pass */
        if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if ((!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                AstType* _result_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
                /* pass */
                List_ptr_append(_result_ty->args, _f_ret);
                /* pass */
                List_ptr_append(_result_ty->args, f->throws_ty);
                /* pass */
                _decl_ret = box_asttype(_result_ty);
            }
        }
        /* pass */
        Sema_declare(self, f->name, SymbolKind_make_SFunction(), _decl_ret, false);
        /* pass */
        AstType* _fnty = AstType_init(_tr_str_lit_len("def", 3LL));
        /* pass */
        long long _pi = 0LL;
        /* pass */
        while ((_pi < f->params->len)) {
            /* pass */
            List_ptr_append(_fnty->args, ((Param*)List_ptr_get(f->params, _pi))->ty);
            /* pass */
            _pi = (_pi + 1LL);
        }
        /* pass */
        List_ptr_append(_fnty->args, _f_ret);
        /* pass */
        _tr_dict_set(self->fn_sigs, _tr_strz(f->name), _fnty);
        /* pass */
        _tr_dict_set(self->fn_defs, _tr_strz(f->name), _tr_obj_retain(f));
        /* pass */
        if ((f->params->len > 0LL)) {
            /* pass */
            Param* _vp = ((Param*)List_ptr_get(f->params, (f->params->len - 1LL)));
            /* pass */
            if (_vp->is_variadic) {
                /* pass */
                AstType* _velem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                /* pass */
                if ((((unsigned long long)(_vp->ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    _velem_ty = (*_vp->ty);
                }
                /* pass */
                ({ TrStr _dvt_t463 = (_tr_str_wrap(_tr_int_to_str((long long)((f->params->len - 1LL))))); _tr_dict_set(self->variadic_fns, _tr_strz(f->name), _tr_str_box(_tr_str_retain(_dvt_t463))); _tr_str_release(_dvt_t463); });
                /* pass */
                _tr_dict_set(self->variadic_elem_ty, _tr_strz(f->name), box_asttype(_velem_ty));
            }
        }
    } else if (_t462.tag == Decl_DClass) {
        __auto_type c = _t462.data.DClass.cls;
        /* pass */
        Sema_declare(self, c->name, SymbolKind_make_SClass(), box_asttype(AstType_init(c->name)), false);
        /* pass */
        _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
        /* pass */
        if (Sema_has_copy_decorator(self, c->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, _tr_strz(c->name), true);
        }
    } else if (_t462.tag == Decl_DActor) {
        __auto_type c = _t462.data.DActor.cls;
        /* pass */
        Sema_declare(self, c->name, SymbolKind_make_SClass(), box_asttype(AstType_init(c->name)), false);
        /* pass */
        _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
        /* pass */
        if (Sema_has_copy_decorator(self, c->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, _tr_strz(c->name), true);
        }
    } else if (_t462.tag == Decl_DEnum) {
        __auto_type e = _t462.data.DEnum.enm;
        /* pass */
        Sema_declare(self, e->name, SymbolKind_make_SEnum(), box_asttype(AstType_init(e->name)), false);
        /* pass */
        _tr_dict_set(self->enums, _tr_strz(e->name), _tr_obj_retain(e));
        /* pass */
        if (Sema_has_copy_decorator(self, e->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, _tr_strz(e->name), true);
        }
    } else if (_t462.tag == Decl_DInterface) {
        __auto_type i = _t462.data.DInterface.iface;
        /* pass */
        Sema_declare(self, i->name, SymbolKind_make_SInterface(), box_asttype(AstType_init(i->name)), false);
        /* pass */
        _tr_dict_set(self->interfaces, _tr_strz(i->name), _tr_obj_retain(i));
        /* pass */
        if (Sema_has_copy_decorator(self, i->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, _tr_strz(i->name), true);
        }
    } else if (_t462.tag == Decl_DExtend) {
        __auto_type target = _t462.data.DExtend.target;
__auto_type methods = _t462.data.DExtend.methods;
        /* pass */
        long long hi = 0LL;
        /* pass */
        while ((hi < methods->len)) {
            /* pass */
            FunctionDef* f = ((FunctionDef*)List_ptr_get(methods, hi));
            /* pass */
            AstType** _m_ret = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
            /* pass */
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                _m_ret = f->ret_ty;
            }
            /* pass */
            TrStr _decl_key = ({ TrStr _cl = (_tr_strx_concatv((target), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (f->name)); _tr_str_release(_cl); _cres; });
            /* pass */
            if (_tr_dict_contains(self->globals, _tr_strz(_decl_key))) {
                /* pass */
                long long _pc = 0LL;
                /* pass */
                long long _pci = 0LL;
                /* pass */
                while ((_pci < f->params->len)) {
                    /* pass */
                    if ((!_tr_str_eqv((((Param*)List_ptr_get(f->params, _pci))->name), (_tr_str_lit_len("self", 4LL))))) {
                        /* pass */
                        _pc = (_pc + 1LL);
                    }
                    /* pass */
                    _pci = (_pci + 1LL);
                }
                /* pass */
                TrStr _strtmp_t464 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_decl_key), (_tr_str_lit_len("_", 1LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(_pc)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("arg", 3LL))); _tr_str_release(_cl); _cres; });
                _tr_str_release(_decl_key);
                _decl_key = _strtmp_t464;
            }
            /* pass */
            Sema_declare(self, _decl_key, SymbolKind_make_SFunction(), _m_ret, false);
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(target))) {
                /* pass */
                List_ptr_append(((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(target)))->methods, _tr_obj_retain(f));
            }
            /* pass */
            hi = (hi + 1LL);
            _tr_str_release(_decl_key);
        }
    } else if (_t462.tag == Decl_DExtern) {
        __auto_type abi = _t462.data.DExtern.abi;
__auto_type functions = _t462.data.DExtern.functions;
        /* pass */
        long long ei = 0LL;
        /* pass */
        while ((ei < functions->len)) {
            /* pass */
            FunctionDef* f = ((FunctionDef*)List_ptr_get(functions, ei));
            /* pass */
            AstType** _e_ret = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
            /* pass */
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                _e_ret = f->ret_ty;
            }
            /* pass */
            Sema_declare(self, f->name, SymbolKind_make_SFunction(), _e_ret, false);
            /* pass */
            _tr_dict_set(self->extern_names, _tr_strz(f->name), true);
            /* pass */
            ei = (ei + 1LL);
        }
    } else if (_t462.tag == Decl_DDecoratorDef) {
        __auto_type f = _t462.data.DDecoratorDef.func;
        /* pass */
        _tr_dict_set(self->decorator_names, _tr_strz(f->name), true);
        /* pass */
        Sema_declare(self, f->name, SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit_len("void", 4LL))), false);
    } else if (_t462.tag == Decl_DTypeAlias) {
        __auto_type alias_name = _t462.data.DTypeAlias.name;
__auto_type target_ty = _t462.data.DTypeAlias.target;
        /* pass */
        AstType** alias_ty = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
        /* pass */
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            alias_ty = target_ty;
        }
        /* pass */
        Sema_declare(self, alias_name, SymbolKind_make_SClass(), alias_ty, false);
        /* pass */
        TrStr resolved_name = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            TrStr _strtmp_t465 = _tr_str_retain((*target_ty)->name);
            _tr_str_release(resolved_name);
            resolved_name = _strtmp_t465;
        }
        /* pass */
        if ((_tr_str_lenv((resolved_name)) > 0LL)) {
            /* pass */
            _tr_dict_set(self->type_aliases, _tr_strz(alias_name), _tr_str_box(_tr_str_retain(resolved_name)));
            /* pass */
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                AstType* ta_t = (*target_ty);
                /* pass */
                if ((ta_t->args->len > 0LL)) {
                    /* pass */
                    _tr_dict_set(self->type_alias_elem, _tr_strz(alias_name), _tr_str_box(_tr_str_retain((*((AstType**)List_ptr_get(ta_t->args, 0LL)))->name)));
                }
            }
        }
        _tr_str_release(resolved_name);
    } else if (1) {
        __auto_type _ = _t462;
        /* pass */
    }
}

__attribute__((hot)) HirFunction* Sema_lower_func(Sema* self, FunctionDef* f) {
    /* pass */
    self->current_line = f->line;
    /* pass */
    bool _saved_fn_is_lib = self->cur_fn_is_lib;
    /* pass */
    self->cur_fn_is_lib = f->is_lib;
    /* pass */
    if ((((!_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL)))) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("", 0LL))))) && (_tr_strlen(_tr_strz(f->name)) > 1LL))) {
        /* pass */
        TrStr fn_cat = Sema_is_reserved_error(self, f->name);
        /* pass */
        if ((!_tr_str_eqv((fn_cat), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            ({ TrStr _at_t466 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (f->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fn_cat)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" and cannot be used as a function name.", 39LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t466); _tr_str_release(_at_t466); });
        } else if (_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            TrStr fn_kcat = Sema_is_reserved_keyword(self, f->name);
            /* pass */
            if ((!_tr_str_eqv((fn_kcat), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                ({ TrStr _at_t467 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (f->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fn_kcat)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" and is reserved. Choose a different function name.", 51LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t467); _tr_str_release(_at_t467); });
            }
        }
    }
    /* pass */
    bool saved_async = self->in_async_fn;
    /* pass */
    self->in_async_fn = f->is_async;
    /* pass */
    self->current_func_name = _tr_str_retain(f->name);
    /* pass */
    List_TrStr* saved_func_generics = self->current_func_generics;
    /* pass */
    self->current_func_generics = f->generics;
    /* pass */
    List_ptr* saved_func_constraints = self->current_func_constraints;
    /* pass */
    self->current_func_constraints = f->constraints;
    /* pass */
    self->container_borrows = _tr_dict_new(16LL);
    /* pass */
    TrStr saved_ret_from = _tr_str_retain(self->current_func_ret_from);
    /* pass */
    bool saved_ret_borrow_str = self->current_func_ret_borrow_str;
    /* pass */
    List_TrStr* saved_ret_regions = self->current_func_ret_regions;
    /* pass */
    List_TrStr* saved_outlives_a = self->current_func_outlives_a;
    /* pass */
    List_TrStr* saved_outlives_b = self->current_func_outlives_b;
    /* pass */
    self->current_func_ret_from = _tr_str_lit_len("", 0LL);
    /* pass */
    self->current_func_ret_borrow_str = false;
    /* pass */
    self->current_func_ret_regions = (void*)List_TrStr_new();
    /* pass */
    self->current_func_outlives_a = f->outlives_a;
    /* pass */
    self->current_func_outlives_b = f->outlives_b;
    /* pass */
    if ((self->strict_mode && (f->outlives_a->len > 0LL))) {
        /* pass */
        long long wbi = 0LL;
        /* pass */
        while ((wbi < f->outlives_a->len)) {
            /* pass */
            bool w_ok_a = false;
            /* pass */
            bool w_ok_b = false;
            /* pass */
            long long wpi = 0LL;
            /* pass */
            while ((wpi < f->params->len)) {
                /* pass */
                if (_tr_str_eqv((((Param*)List_ptr_get(f->params, wpi))->name), (List_TrStr_get(f->outlives_a, wbi)))) {
                    /* pass */
                    w_ok_a = true;
                }
                /* pass */
                if (_tr_str_eqv((((Param*)List_ptr_get(f->params, wpi))->name), (List_TrStr_get(f->outlives_b, wbi)))) {
                    /* pass */
                    w_ok_b = true;
                }
                /* pass */
                wpi = (wpi + 1LL);
            }
            /* pass */
            if ((!w_ok_a)) {
                /* pass */
                ({ TrStr _at_t468 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(f->outlives_a, wbi)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("[L-2] region '", 14LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' in the `where ... outlives ...` clause is not a parameter of this function.", 77LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t468); _tr_str_release(_at_t468); });
            }
            /* pass */
            if ((!w_ok_b)) {
                /* pass */
                ({ TrStr _at_t469 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(f->outlives_b, wbi)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("[L-2] region '", 14LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' in the `where ... outlives ...` clause is not a parameter of this function.", 77LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t469); _tr_str_release(_at_t469); });
            }
            /* pass */
            wbi = (wbi + 1LL);
        }
    }
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        self->current_func_ret_from = _tr_str_retain((*f->ret_ty)->from_param);
        /* pass */
        self->current_func_ret_regions = (*f->ret_ty)->from_regions;
        /* pass */
        if (((*f->ret_ty)->is_borrow && _tr_str_eqv(((*f->ret_ty)->name), (_tr_str_lit_len("str", 3LL))))) {
            /* pass */
            self->current_func_ret_borrow_str = true;
        }
    }
    /* pass */
    if ((_tr_str_eqv((self->current_func_ret_from), (_tr_str_lit_len("", 0LL))) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
        /* pass */
        TrStr infer_ret_nm = _tr_str_retain((*f->ret_ty)->name);
        /* pass */
        bool infer_is_borrow = (*f->ret_ty)->is_borrow;
        /* pass */
        if ((((_tr_str_eqv((infer_ret_nm), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((infer_ret_nm), (_tr_str_lit_len("ref", 3LL)))) || _tr_str_eqv((infer_ret_nm), (_tr_str_lit_len("mut_ref", 7LL)))) || infer_is_borrow)) {
            /* pass */
            TrStr infer_from = _tr_str_lit_len("", 0LL);
            /* pass */
            long long infer_count = 0LL;
            /* pass */
            long long infer_i = 0LL;
            /* pass */
            while ((infer_i < f->params->len)) {
                /* pass */
                Param* infer_p = ((Param*)List_ptr_get(f->params, infer_i));
                /* pass */
                if (((!_tr_str_eqv((infer_p->name), (_tr_str_lit_len("self", 4LL)))) && (((unsigned long long)(infer_p->ty)) != ((unsigned long long)(0LL))))) {
                    /* pass */
                    AstType* infer_pty = (*infer_p->ty);
                    /* pass */
                    if ((!Sema_is_primitive_name(self, infer_pty->name))) {
                        /* pass */
                        TrStr _strtmp_t470 = _tr_str_retain(infer_p->name);
                        _tr_str_release(infer_from);
                        infer_from = _strtmp_t470;
                        /* pass */
                        infer_count = (infer_count + 1LL);
                    }
                }
                /* pass */
                infer_i = (infer_i + 1LL);
            }
            /* pass */
            if ((infer_count == 1LL)) {
                /* pass */
                self->current_func_ret_from = _tr_str_retain(infer_from);
            }
        }
        _tr_str_release(infer_ret_nm);
    }
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        List_TrStr* rregs = (*f->ret_ty)->from_regions;
        /* pass */
        long long rri = 0LL;
        /* pass */
        while ((rri < rregs->len)) {
            /* pass */
            TrStr rnm = List_TrStr_get(rregs, rri);
            /* pass */
            bool rok = false;
            /* pass */
            long long rpi = 0LL;
            /* pass */
            while ((rpi < f->params->len)) {
                /* pass */
                if (_tr_str_eqv((((Param*)List_ptr_get(f->params, rpi))->name), (rnm))) {
                    /* pass */
                    rok = true;
                }
                /* pass */
                rpi = (rpi + 1LL);
            }
            /* pass */
            if ((!rok)) {
                /* pass */
                long long rci = 0LL;
                /* pass */
                while ((rci < self->current_region_params->len)) {
                    /* pass */
                    if (_tr_str_eqv((List_TrStr_get(self->current_region_params, rci)), (rnm))) {
                        /* pass */
                        rok = true;
                    }
                    /* pass */
                    rci = (rci + 1LL);
                }
            }
            /* pass */
            if ((!rok)) {
                /* pass */
                ({ TrStr _at_t471 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[L-2] region source '", 21LL)), (rnm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' named in 'from' is not a parameter or a region parameter of the enclosing type.\n      FIX: name a parameter the borrow comes from, or declare 'class/enum/interface <T> from ", 175LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (rnm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":'.", 3LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t471); _tr_str_release(_at_t471); });
            }
            /* pass */
            rri = (rri + 1LL);
            _tr_str_release(rnm);
        }
    }
    /* pass */
    Sema_enter_scope(self);
    /* pass */
    List_ptr* hparams = (void*)List_ptr_new();
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < f->params->len)) {
        /* pass */
        Param* p = ((Param*)List_ptr_get(f->params, j));
        /* pass */
        AstType* p_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        /* pass */
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            p_ty = (*p->ty);
        } else if ((_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            p_ty = AstType_init(self->current_class_name);
        }
        /* pass */
        if (p->is_variadic) {
            /* pass */
            AstType* _vp_ty = AstType_init(_tr_str_lit_len("List", 4LL));
            /* pass */
            List_ptr_append(_vp_ty->args, box_asttype(p_ty));
            /* pass */
            p_ty = _vp_ty;
        }
        /* pass */
        Sema_declare(self, p->name, SymbolKind_make_SVariable(), box_asttype(p_ty), false);
        /* pass */
        if ((self->scopes->len > 0LL)) {
            /* pass */
            Scope* pb_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
            /* pass */
            if (_tr_dict_contains(pb_scope->variables, _tr_strz(p->name))) {
                /* pass */
                Symbol* pb_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pb_scope->variables, _tr_strz(p->name)));
                /* pass */
                pb_sym->is_param = true;
                /* pass */
                pb_sym->ptr_region = 2LL;
                /* pass */
                _tr_dict_set(pb_scope->variables, _tr_strz(p->name), pb_sym);
            }
        }
        /* pass */
        HirParam* hp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
        /* pass */
        hp->name = _tr_str_retain(p->name);
        /* pass */
        hp->ty = p_ty;
        /* pass */
        List_ptr_append(hparams, _tr_obj_retain(hp));
        /* pass */
        j = (j + 1LL);
        _tr_obj_release(hp, _trdrop_HirParam);
    }
    /* pass */
    HirFunction* hf = ((HirFunction*)_tr_obj_alloc(sizeof(HirFunction)));
    /* pass */
    hf->name = _tr_str_retain(f->name);
    /* pass */
    hf->class_name = _tr_str_retain(self->current_class_name);
    /* pass */
    hf->generics = f->generics;
    /* pass */
    hf->constraints = f->constraints;
    /* pass */
    hf->params = hparams;
    /* pass */
    hf->ret_ty = AstType_init(_tr_str_lit_len("None", 4LL));
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        hf->ret_ty = (*f->ret_ty);
    }
    /* pass */
    hf->throws_ty = AstType_init(_tr_str_lit_len("", 0LL));
    /* pass */
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        hf->throws_ty = (*f->throws_ty);
    }
    /* pass */
    self->cur_func_borrowers = (void*)List_TrStr_new();
    /* pass */
    self->cur_func_sources = (void*)List_TrStr_new();
    /* pass */
    HirBlock* _cltmp_t472 = Sema_lower_block(self, f->body);
    _tr_obj_release(hf->body, _trdrop_HirBlock);
    hf->body = _cltmp_t472;
    /* pass */
    hf->borrow_borrowers = self->cur_func_borrowers;
    /* pass */
    hf->borrow_sources = self->cur_func_sources;
    /* pass */
    hf->proven_borrows = mir_proven_borrows(hf);
    /* pass */
    if (self->strict_mode) {
        /* pass */
        List_TrStr* _bconf = mir_borrow_conflicts(hf, self->mutating_methods);
        /* pass */
        long long _bci = 0LL;
        /* pass */
        while ((_bci < _bconf->len)) {
            /* pass */
            ({ TrStr _at_t473 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(_bconf, _bci)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("[B-1] ", 6LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".\n      A place may have MANY shared `ref` borrows, OR exactly ONE exclusive `mut` borrow — never both. End one borrow's last use before the other begins.", 156LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t473); _tr_str_release(_at_t473); });
            /* pass */
            _bci = (_bci + 1LL);
        }
        /* pass */
        List_TrStr* _b3 = mir_shared_ref_param_violations(hf, self->mutating_methods);
        /* pass */
        long long _b3i = 0LL;
        /* pass */
        while ((_b3i < _b3->len)) {
            /* pass */
            ({ TrStr _at_t474 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(_b3, _b3i)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("[B-3] ", 6LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".\n      A shared `ref T` parameter cannot be mutated — declare it `mut ref T` for an exclusive (mutable) borrow.", 114LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t474); _tr_str_release(_at_t474); });
            /* pass */
            _b3i = (_b3i + 1LL);
        }
        List_TrStr_free(_bconf);
    }
    /* pass */
    hf->decorators = f->decorators;
    /* pass */
    hf->is_async = f->is_async;
    /* pass */
    hf->is_extern = f->is_extern;
    /* pass */
    hf->is_public = f->is_public;
    /* pass */
    hf->is_export = f->is_export;
    /* pass */
    bool _has_self = false;
    /* pass */
    long long _si = 0LL;
    /* pass */
    while ((_si < hparams->len)) {
        /* pass */
        if (_tr_str_eqv((((HirParam*)List_ptr_get(hparams, _si))->name), (_tr_str_lit_len("self", 4LL)))) {
            /* pass */
            _has_self = true;
        }
        /* pass */
        _si = (_si + 1LL);
    }
    /* pass */
    hf->is_static = ((!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))) && (!_has_self));
    /* pass */
    hf->is_variadic = f->is_variadic;
    /* pass */
    hf->is_decorator = false;
    /* pass */
    bool _is_entry_main = (_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL))) && _tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))));
    /* pass */
    if ((((!f->is_extern) && (f->body->stmts->len > 0LL)) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
        /* pass */
        TrStr ret_nm = _tr_str_retain((*f->ret_ty)->name);
        /* pass */
        if (((((((!_tr_str_eqv((ret_nm), (_tr_str_lit_len("void", 4LL)))) && (!_tr_str_eqv((ret_nm), (_tr_str_lit_len("None", 4LL))))) && (!_tr_str_eqv((ret_nm), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("init", 4LL))))) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("new", 3LL))))) && (!_is_entry_main))) {
            /* pass */
            if ((!Sema_block_returns(self, f->body))) {
                /* pass */
                self->current_line = f->line;
                /* pass */
                ({ TrStr _at_t475 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[F-3] Function '", 16LL)), (f->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' returns '", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ret_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' but is missing a return statement on at least one code path. FIX: Add a return at the end, or ensure all if/elif/else branches return.", 136LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t475); _tr_str_release(_at_t475); });
            }
        }
    }
    /* pass */
    Sema_finalize_scope_drops(self, hf->body);
    /* pass */
    Sema_apply_mir_if_drops(self, hf);
    /* pass */
    Sema_exit_scope(self);
    /* pass */
    self->in_async_fn = saved_async;
    /* pass */
    self->current_func_name = _tr_str_lit_len("", 0LL);
    /* pass */
    self->current_func_generics = saved_func_generics;
    /* pass */
    self->current_func_constraints = saved_func_constraints;
    /* pass */
    self->current_func_ret_from = _tr_str_retain(saved_ret_from);
    /* pass */
    self->current_func_ret_borrow_str = saved_ret_borrow_str;
    /* pass */
    self->current_func_ret_regions = saved_ret_regions;
    /* pass */
    self->current_func_outlives_a = saved_outlives_a;
    /* pass */
    self->current_func_outlives_b = saved_outlives_b;
    /* pass */
    self->cur_fn_is_lib = _saved_fn_is_lib;
    /* pass */
    _tr_str_release(saved_ret_from);
    return hf;
}

__attribute__((hot)) HirClass* Sema_lower_class(Sema* self, ClassDef* c) {
    /* pass */
    self->current_line = c->line;
    /* pass */
    self->current_class_name = _tr_str_retain(c->name);
    /* pass */
    self->current_region_params = c->region_params;
    /* pass */
    List_ptr* hfields = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c->fields->len)) {
        /* pass */
        FieldDef* f = ((FieldDef*)List_ptr_get(c->fields, i));
        /* pass */
        HirField* hf = ((HirField*)_tr_obj_alloc(sizeof(HirField)));
        /* pass */
        hf->name = _tr_str_retain(f->name);
        /* pass */
        AstType* f_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        /* pass */
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            f_ty = (*f->ty);
        }
        /* pass */
        hf->ty = f_ty;
        /* pass */
        List_ptr_append(hfields, _tr_obj_retain(hf));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hf, _trdrop_HirField);
    }
    /* pass */
    long long pf_i = 0LL;
    /* pass */
    while ((pf_i < c->fields->len)) {
        /* pass */
        FieldDef* pf_f = ((FieldDef*)List_ptr_get(c->fields, pf_i));
        /* pass */
        if ((((unsigned long long)(pf_f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            AstType* pf_ty = (*pf_f->ty);
            /* pass */
            if ((_tr_str_eqv((pf_ty->name), (_tr_str_lit_len("Shared", 6LL))) && (pf_ty->args->len > 0LL))) {
                /* pass */
                AstType* pf_inner = (*((AstType**)List_ptr_get(pf_ty->args, 0LL)));
                /* pass */
                if (_tr_str_eqv((pf_inner->name), (c->name))) {
                    /* pass */
                    ({ TrStr _at_t476 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[S-1] '", 7LL)), (c->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' has a 'Shared[", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (c->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' field '", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pf_f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' - this creates a reference cycle that leaks memory.\n      FIX: Use 'Weak[", 75LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (c->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' for back-references to break the cycle.", 42LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t476); _tr_str_release(_at_t476); });
                }
            }
        }
        /* pass */
        pf_i = (pf_i + 1LL);
    }
    /* pass */
    List_ptr* hmethods = (void*)List_ptr_new();
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < c->methods->len)) {
        /* pass */
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(c->methods, i)));
        /* pass */
        List_ptr_append(hmethods, _tr_obj_retain(hm));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
    /* pass */
    long long _ifc_i = 0LL;
    /* pass */
    while ((_ifc_i < c->iface_names->len)) {
        /* pass */
        TrStr _ifc_nm = List_TrStr_get(c->iface_names, _ifc_i);
        /* pass */
        if (_tr_str_eqv((_ifc_nm), (_tr_str_lit_len("Sendable", 8LL)))) {
            /* pass */
            if ((c->generics->len == 0LL)) {
                /* pass */
                Sema_check_class_sendable_fields(self, c);
            }
        } else if (_tr_str_eqv((_ifc_nm), (_tr_str_lit_len("UnsafeSendable", 14LL)))) {
            /* pass */
            /* pass */
        } else if ((!_tr_dict_contains(self->interfaces, _tr_strz(_ifc_nm)))) {
            /* pass */
            ({ TrStr _at_t477 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[I-1] Class '", 13LL)), (c->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' declares 'implements ", 23LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' but interface '", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not defined.\n      FIX: Define 'interface ", 47LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":' before this class, or check for typos.", 41LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t477); _tr_str_release(_at_t477); });
        } else {
            /* pass */
            InterfaceDef* _idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(_ifc_nm)));
            /* pass */
            long long _im_i = 0LL;
            /* pass */
            while ((_im_i < _idef->methods->len)) {
                /* pass */
                FunctionDef* _imeth = ((FunctionDef*)List_ptr_get(_idef->methods, _im_i));
                /* pass */
                bool _found = false;
                /* pass */
                long long _cm_i = 0LL;
                /* pass */
                while ((_cm_i < c->methods->len)) {
                    /* pass */
                    FunctionDef* _cmeth = ((FunctionDef*)List_ptr_get(c->methods, _cm_i));
                    /* pass */
                    if (_tr_str_eqv((_cmeth->name), (_imeth->name))) {
                        /* pass */
                        _found = true;
                        /* pass */
                        TrStr _iret = _tr_str_lit_len("void", 4LL);
                        /* pass */
                        if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            TrStr _strtmp_t478 = _tr_str_retain((*_imeth->ret_ty)->name);
                            _tr_str_release(_iret);
                            _iret = _strtmp_t478;
                        }
                        /* pass */
                        TrStr _cret = _tr_str_lit_len("void", 4LL);
                        /* pass */
                        if ((((unsigned long long)(_cmeth->ret_ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            TrStr _strtmp_t479 = _tr_str_retain((*_cmeth->ret_ty)->name);
                            _tr_str_release(_cret);
                            _cret = _strtmp_t479;
                        }
                        /* pass */
                        bool _iret_is_generic = ((((_ifc_i >= 0LL) && (_idef->generics->len > 0LL)) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("", 0LL)))));
                        /* pass */
                        long long _gi = 0LL;
                        /* pass */
                        while ((_gi < _idef->generics->len)) {
                            /* pass */
                            if (_tr_str_eqv((List_TrStr_get(_idef->generics, _gi)), (_iret))) {
                                /* pass */
                                _iret_is_generic = true;
                            }
                            /* pass */
                            _gi = (_gi + 1LL);
                        }
                        /* pass */
                        if (((((!_iret_is_generic) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((_cret), (_iret))))) {
                            /* pass */
                            ({ TrStr _at_t480 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[I-3] Class '", 13LL)), (c->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': method '", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_cmeth->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' returns '", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_cret)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' but interface '", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' declares '-> ", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_iret)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.\n      FIX: Change return type to '-> ", 40LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_iret)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t480); _tr_str_release(_at_t480); });
                        }
                        /* pass */
                        long long _ip_cnt = 0LL;
                        /* pass */
                        long long _ip_i = 0LL;
                        /* pass */
                        while ((_ip_i < _imeth->params->len)) {
                            /* pass */
                            if ((!_tr_str_eqv((((Param*)List_ptr_get(_imeth->params, _ip_i))->name), (_tr_str_lit_len("self", 4LL))))) {
                                /* pass */
                                _ip_cnt = (_ip_cnt + 1LL);
                            }
                            /* pass */
                            _ip_i = (_ip_i + 1LL);
                        }
                        /* pass */
                        long long _cp_cnt = 0LL;
                        /* pass */
                        long long _cp_i = 0LL;
                        /* pass */
                        while ((_cp_i < _cmeth->params->len)) {
                            /* pass */
                            if ((!_tr_str_eqv((((Param*)List_ptr_get(_cmeth->params, _cp_i))->name), (_tr_str_lit_len("self", 4LL))))) {
                                /* pass */
                                _cp_cnt = (_cp_cnt + 1LL);
                            }
                            /* pass */
                            _cp_i = (_cp_i + 1LL);
                        }
                        /* pass */
                        if ((_ip_cnt != _cp_cnt)) {
                            /* pass */
                            ({ TrStr _at_t481 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[I-3] Class '", 13LL)), (c->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': method '", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_cmeth->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' has ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(_cp_cnt)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" parameter(s) but interface '", 29LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' requires ", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(_ip_cnt)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".\n      FIX: Match the parameter list in '", 42LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' exactly.", 10LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t481); _tr_str_release(_at_t481); });
                        }
                    }
                    /* pass */
                    _cm_i = (_cm_i + 1LL);
                }
                /* pass */
                if ((!_found)) {
                    /* pass */
                    TrStr _sig = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("pub def ", 8LL)), (_imeth->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(self", 5LL))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    long long _pi = 0LL;
                    /* pass */
                    while ((_pi < _imeth->params->len)) {
                        /* pass */
                        Param* _p = ((Param*)List_ptr_get(_imeth->params, _pi));
                        /* pass */
                        if ((!_tr_str_eqv((_p->name), (_tr_str_lit_len("self", 4LL))))) {
                            /* pass */
                            TrStr _pty = _tr_str_lit_len("int", 3LL);
                            /* pass */
                            if ((((unsigned long long)(_p->ty)) != ((unsigned long long)(0LL)))) {
                                /* pass */
                                TrStr _strtmp_t482 = _tr_str_retain((*_p->ty)->name);
                                _tr_str_release(_pty);
                                _pty = _strtmp_t482;
                            }
                            /* pass */
                            TrStr _strtmp_t483 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_sig), (_tr_str_lit_len(", ", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (_p->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_pty)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(_sig);
                            _sig = _strtmp_t483;
                        }
                        /* pass */
                        _pi = (_pi + 1LL);
                    }
                    /* pass */
                    TrStr _iret2 = _tr_str_lit_len("void", 4LL);
                    /* pass */
                    if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        TrStr _strtmp_t484 = _tr_str_retain((*_imeth->ret_ty)->name);
                        _tr_str_release(_iret2);
                        _iret2 = _strtmp_t484;
                    }
                    /* pass */
                    TrStr _strtmp_t485 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_sig), (_tr_str_lit_len(") -> ", 5LL)))); TrStr _cres = _tr_strx_concatv(_cl, (_iret2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(_sig);
                    _sig = _strtmp_t485;
                    /* pass */
                    ({ TrStr _at_t486 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[I-2] Class '", 13LL)), (c->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' implements '", 14LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' but is missing method '", 25LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_imeth->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.\n      FIX: Add to 'extend ", 29LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (c->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":'  ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_sig)); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t486); _tr_str_release(_at_t486); });
                }
                /* pass */
                _im_i = (_im_i + 1LL);
            }
        }
        /* pass */
        _ifc_i = (_ifc_i + 1LL);
        _tr_str_release(_ifc_nm);
    }
    /* pass */
    HirClass* hc = ((HirClass*)_tr_obj_alloc(sizeof(HirClass)));
    /* pass */
    hc->name = _tr_str_retain(c->name);
    /* pass */
    hc->generics = c->generics;
    /* pass */
    hc->fields = hfields;
    /* pass */
    hc->methods = hmethods;
    /* pass */
    hc->base_classes = c->base_classes;
    /* pass */
    hc->iface_names = c->iface_names;
    /* pass */
    hc->iface_targs = c->iface_targs;
    /* pass */
    hc->decorators = c->decorators;
    /* pass */
    hc->is_public = c->is_public;
    /* pass */
    hc->is_class = c->is_class;
    /* pass */
    self->current_class_name = _tr_str_lit_len("", 0LL);
    /* pass */
    self->current_region_params = (void*)List_TrStr_new();
    /* pass */
    return hc;
}

__attribute__((hot)) HirEnum* Sema_lower_enum(Sema* self, EnumDef* e) {
    /* pass */
    self->current_class_name = _tr_str_retain(e->name);
    /* pass */
    self->current_region_params = e->region_params;
    /* pass */
    List_ptr* hvariants = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < e->variants->len)) {
        /* pass */
        VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, i));
        /* pass */
        List_ptr* hfields = (void*)List_ptr_new();
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < v->fields->len)) {
            /* pass */
            Param* p = ((Param*)List_ptr_get(v->fields, j));
            /* pass */
            if ((self->strict_mode && (((unsigned long long)(p->ty)) != ((unsigned long long)(0LL))))) {
                /* pass */
                AstType* _vty = (*p->ty);
                /* pass */
                long long _vri = 0LL;
                /* pass */
                while ((_vri < _vty->from_regions->len)) {
                    /* pass */
                    TrStr _vrn = List_TrStr_get(_vty->from_regions, _vri);
                    /* pass */
                    bool _vrok = false;
                    /* pass */
                    long long _vrk = 0LL;
                    /* pass */
                    while ((_vrk < e->region_params->len)) {
                        /* pass */
                        if (_tr_str_eqv((List_TrStr_get(e->region_params, _vrk)), (_vrn))) {
                            /* pass */
                            _vrok = true;
                        }
                        /* pass */
                        _vrk = (_vrk + 1LL);
                    }
                    /* pass */
                    if ((!_vrok)) {
                        /* pass */
                        self->current_line = e->line;
                        /* pass */
                        ({ TrStr _at_t487 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[L-2] region source '", 21LL)), (_vrn))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' in variant '", 14LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' of enum '", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (e->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not a region parameter.\n      FIX: declare 'enum ", 54LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (e->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" from ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_vrn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":'.", 3LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t487); _tr_str_release(_at_t487); });
                    }
                    /* pass */
                    _vri = (_vri + 1LL);
                    _tr_str_release(_vrn);
                }
            }
            /* pass */
            HirParam* hp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
            /* pass */
            hp->name = _tr_str_retain(p->name);
            /* pass */
            AstType* p_ty = AstType_init(p->name);
            /* pass */
            if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                p_ty = (*p->ty);
            }
            /* pass */
            hp->ty = p_ty;
            /* pass */
            List_ptr_append(hfields, _tr_obj_retain(hp));
            /* pass */
            j = (j + 1LL);
            _tr_obj_release(hp, _trdrop_HirParam);
        }
        /* pass */
        HirVariant* hv = ((HirVariant*)_tr_obj_alloc(sizeof(HirVariant)));
        /* pass */
        hv->name = _tr_str_retain(v->name);
        /* pass */
        hv->fields = hfields;
        /* pass */
        List_ptr_append(hvariants, _tr_obj_retain(hv));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hv, _trdrop_HirVariant);
    }
    /* pass */
    List_ptr* hmethods = (void*)List_ptr_new();
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < e->methods->len)) {
        /* pass */
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(e->methods, i)));
        /* pass */
        List_ptr_append(hmethods, _tr_obj_retain(hm));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
    /* pass */
    HirEnum* he = ((HirEnum*)_tr_obj_alloc(sizeof(HirEnum)));
    /* pass */
    he->name = _tr_str_retain(e->name);
    /* pass */
    he->generics = e->generics;
    /* pass */
    he->variants = hvariants;
    /* pass */
    he->methods = hmethods;
    /* pass */
    he->iface_names = e->iface_names;
    /* pass */
    he->decorators = e->decorators;
    /* pass */
    he->is_public = e->is_public;
    /* pass */
    self->current_class_name = _tr_str_lit_len("", 0LL);
    /* pass */
    self->current_region_params = (void*)List_TrStr_new();
    /* pass */
    return he;
}

__attribute__((hot)) HirInterface* Sema_lower_interface(Sema* self, InterfaceDef* i_def) {
    /* pass */
    self->current_region_params = i_def->region_params;
    /* pass */
    List_ptr* hmethods = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < i_def->methods->len)) {
        /* pass */
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(i_def->methods, i)));
        /* pass */
        List_ptr_append(hmethods, _tr_obj_retain(hm));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
    /* pass */
    HirInterface* hi = ((HirInterface*)_tr_obj_alloc(sizeof(HirInterface)));
    /* pass */
    hi->name = _tr_str_retain(i_def->name);
    /* pass */
    hi->generics = i_def->generics;
    /* pass */
    hi->methods = hmethods;
    /* pass */
    self->current_region_params = (void*)List_TrStr_new();
    /* pass */
    return hi;
}

__attribute__((hot)) void Sema_apply_escape_marks(Sema* self, HirStmt* _hs) {
    /* pass */
    __auto_type _t488 = (*_hs);
    if (_t488.tag == HirStmt_SExpr) {
        __auto_type _es_e = _t488.data.SExpr.expr;
        /* pass */
        Sema_mark_escaped_str_args(self, _es_e);
        /* pass */
        Sema_mark_escaped_coll_args(self, _es_e);
    } else if (_t488.tag == HirStmt_SLet) {
        __auto_type _sl_v = _t488.data.SLet.val;
        /* pass */
        Sema_mark_escaped_str_args(self, _sl_v);
        /* pass */
        Sema_mark_coll_arg(self, _sl_v);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sl_v);
    } else if (_t488.tag == HirStmt_SAssign) {
        __auto_type _sa_t = _t488.data.SAssign.target;
__auto_type _sa_v = _t488.data.SAssign.val;
        /* pass */
        Sema_mark_escaped_str_args(self, _sa_t);
        /* pass */
        Sema_mark_escaped_str_args(self, _sa_v);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sa_t);
        /* pass */
        Sema_mark_coll_arg(self, _sa_v);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sa_v);
    } else if (_t488.tag == HirStmt_SReturn) {
        __auto_type _sr_e = _t488.data.SReturn.val;
        /* pass */
        Sema_mark_escaped_str_args(self, _sr_e);
        /* pass */
        Sema_mark_coll_arg(self, _sr_e);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sr_e);
    } else if (_t488.tag == HirStmt_SIf) {
        __auto_type _si_c = _t488.data.SIf.cond;
        /* pass */
        Sema_mark_escaped_str_args(self, _si_c);
        /* pass */
        Sema_mark_escaped_coll_args(self, _si_c);
    } else if (_t488.tag == HirStmt_SWhile) {
        __auto_type _sw_c = _t488.data.SWhile.cond;
        /* pass */
        Sema_mark_escaped_str_args(self, _sw_c);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sw_c);
    } else if (_t488.tag == HirStmt_SFor) {
        __auto_type _sf_iter = _t488.data.SFor.iter;
        /* pass */
        Sema_mark_escaped_coll_args(self, _sf_iter);
    } else if (_t488.tag == HirStmt_SForUnpack) {
        __auto_type _sfu_iter = _t488.data.SForUnpack.iter;
        /* pass */
        Sema_mark_escaped_coll_args(self, _sfu_iter);
    } else if (1) {
        __auto_type _ = _t488;
        /* pass */
    }
}

__attribute__((hot)) HirBlock* Sema_lower_block(Sema* self, Block* b) {
    /* pass */
    HirBlock* hb = HirBlock_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        Stmt* orig_ptr = ((Stmt*)List_ptr_get(b->stmts, i));
        /* pass */
        HirStmt* _hs = Sema_lower_stmt(self, orig_ptr);
        /* pass */
        Sema_apply_escape_marks(self, _hs);
        /* pass */
        if ((((unsigned long long)(orig_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t489 = (*orig_ptr);
            if (_t489.tag == Stmt_SReturn) {
                __auto_type _ = _t489.data.SReturn.val;
                /* pass */
                List_TrStr* ret_excl = (void*)List_TrStr_new();
                /* pass */
                __auto_type _t490 = (*_hs);
                if (_t490.tag == HirStmt_SReturn) {
                    __auto_type lowered_ret = _t490.data.SReturn.val;
                    /* pass */
                    Sema_collect_idents(self, lowered_ret, ret_excl);
                } else if (1) {
                    __auto_type _ = _t490;
                    /* pass */
                }
                /* pass */
                long long ret_from = 0LL;
                /* pass */
                if ((self->fn_scope_base->len > 0LL)) {
                    /* pass */
                    ret_from = List_i64_get(self->fn_scope_base, (self->fn_scope_base->len - 1LL));
                }
                /* pass */
                Sema_append_drops_from_excl_multi(self, hb, ret_from, ret_excl);
            } else if (_t489.tag == Stmt_SBreak) {
                __auto_type _ = _t489.data.SBreak.val;
                /* pass */
                if ((self->loop_scope_base->len > 0LL)) {
                    /* pass */
                    Sema_append_drops_from(self, hb, List_i64_get(self->loop_scope_base, (self->loop_scope_base->len - 1LL)));
                }
            } else if (_t489.tag == Stmt_SContinue) {
                /* pass */
                if ((self->loop_scope_base->len > 0LL)) {
                    /* pass */
                    Sema_append_drops_from(self, hb, List_i64_get(self->loop_scope_base, (self->loop_scope_base->len - 1LL)));
                }
            } else if (1) {
                __auto_type _ = _t489;
                /* pass */
            }
        }
        /* pass */
        HirBlock_push(hb, _hs);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return hb;
}

__attribute__((hot)) HirStmt* Sema_lower_stmt(Sema* self, Stmt* s_ptr) {
    /* pass */
    if ((((unsigned long long)(s_ptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (HirStmt*)(0LL);
    }
    /* pass */
    __auto_type s = (*s_ptr);
    /* pass */
    __auto_type _t491 = s;
    if (_t491.tag == Stmt_SExpr) {
        __auto_type e = _t491.data.SExpr.expr;
        /* pass */
        HirStmt* h_s_expr = box_hirstmt(HirStmt_ctor_SExpr(Sema_lower_expr(self, e)));
        /* pass */
        if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t492 = (*e);
            if (_t492.tag == Expr_ECall) {
                __auto_type callee = _t492.data.ECall.callee;
__auto_type args = _t492.data.ECall.args;
                /* pass */
                if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    __auto_type _t493 = (*callee);
                    if (_t493.tag == Expr_EIdent) {
                        __auto_type fn_name = _t493.data.EIdent.name;
                        /* pass */
                        Symbol* fn_sym = Sema_resolve(self, fn_name);
                        /* pass */
                        if (((fn_sym->kind.tag == SymbolKind_make_SFunction().tag) && _tr_str_eqv(((*fn_sym->ty)->name), (_tr_str_lit_len("Result", 6LL))))) {
                            /* pass */
                            ({ TrStr _at_t494 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-4] '", 7LL)), (fn_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()' returns a Result and its error must be handled. FIX: Assign the result and match on it, use '?' to propagate, or '_ = ", 122LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fn_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(...)' to explicitly discard.", 29LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t494); _tr_str_release(_at_t494); });
                        }
                    } else if (1) {
                        __auto_type _ = _t493;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t492;
                /* pass */
            }
        }
        /* pass */
        return h_s_expr;
    } else if (_t491.tag == Stmt_SReturn) {
        __auto_type e = _t491.data.SReturn.val;
        /* pass */
        if (((self->strict_mode && self->current_func_ret_borrow_str) && (((unsigned long long)(e)) != ((unsigned long long)(0LL))))) {
            /* pass */
            __auto_type _t495 = (*e);
            if (_t495.tag == Expr_EBinOp) {
                __auto_type l3_op = _t495.data.EBinOp.op;
                /* pass */
                if (_tr_str_eqv((l3_op), (_tr_str_lit_len("+", 1LL)))) {
                    /* pass */
                    ({ TrStr _at_t496 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[L-3] borrow-returning function returns a freshly-built string (owned), not a borrow of region '", 96LL)), (self->current_func_ret_from))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.\n      FIX: return a slice/view of the region source, or change the return type to a plain owned 'str'.", 105LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t496); _tr_str_release(_at_t496); });
                }
            } else if (_t495.tag == Expr_EIdent) {
                __auto_type l3_nm = _t495.data.EIdent.name;
                /* pass */
                Symbol* l3_sym = Sema_resolve(self, l3_nm);
                /* pass */
                if ((!_tr_str_eqv((l3_sym->name), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    TrStr l3_eff = _tr_str_lit_len("", 0LL);
                    /* pass */
                    if (l3_sym->is_param) {
                        /* pass */
                        TrStr _strtmp_t497 = _tr_str_retain(l3_nm);
                        _tr_str_release(l3_eff);
                        l3_eff = _strtmp_t497;
                    } else {
                        /* pass */
                        TrStr _strtmp_t498 = _tr_str_retain(l3_sym->borrows_region);
                        _tr_str_release(l3_eff);
                        l3_eff = _strtmp_t498;
                    }
                    /* pass */
                    if (_tr_str_eqv((l3_eff), (_tr_str_lit_len("@owned", 6LL)))) {
                        /* pass */
                        ({ TrStr _at_t499 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[L-3] borrow-returning function returns '", 41LL)), (l3_nm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("', a freshly-built string (owned), not a borrow of region '", 59LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (self->current_func_ret_from)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.\n      FIX: return a slice/view of the region source, or change the return type to a plain owned 'str'.", 105LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t499); _tr_str_release(_at_t499); });
                    } else if (((!_tr_str_eqv((l3_eff), (_tr_str_lit_len("", 0LL)))) && (self->current_func_ret_regions->len > 0LL))) {
                        /* pass */
                        bool l3_in = false;
                        /* pass */
                        long long l3ri = 0LL;
                        /* pass */
                        while ((l3ri < self->current_func_ret_regions->len)) {
                            /* pass */
                            if (_tr_str_eqv((List_TrStr_get(self->current_func_ret_regions, l3ri)), (l3_eff))) {
                                /* pass */
                                l3_in = true;
                            }
                            /* pass */
                            l3ri = (l3ri + 1LL);
                        }
                        /* pass */
                        if ((!l3_in)) {
                            /* pass */
                            long long l3di = 0LL;
                            /* pass */
                            while ((l3di < self->current_func_ret_regions->len)) {
                                /* pass */
                                if (({ TrStr _at_t500 = (List_TrStr_get(self->current_func_ret_regions, l3di)); __auto_type _wr = (Sema_region_outlives(self, l3_eff, _at_t500)); _tr_str_release(_at_t500); _wr; })) {
                                    /* pass */
                                    l3_in = true;
                                }
                                /* pass */
                                l3di = (l3di + 1LL);
                            }
                        }
                        /* pass */
                        if ((!l3_in)) {
                            /* pass */
                            ({ TrStr _at_t501 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[L-4] borrow-returning function returns a borrow of region '", 60LL)), (l3_eff))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("', but its return is declared 'from ", 36LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (self->current_func_ret_from)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.\n      FIX: add '", 19LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (l3_eff)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' to the 'from' list, declare 'where ", 37LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (l3_eff)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" outlives ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (self->current_func_ret_from)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("', or return a borrow of a declared region.", 43LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t501); _tr_str_release(_at_t501); });
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t495;
                /* pass */
            }
        }
        /* pass */
        if ((((((unsigned long long)(e)) != ((unsigned long long)(0LL))) && (!self->in_unsafe)) && _tr_str_eqv((self->current_func_ret_from), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            __auto_type _t502 = (*e);
            if (_t502.tag == Expr_EIdent) {
                __auto_type ret_name = _t502.data.EIdent.name;
                /* pass */
                Symbol* ret_sym = Sema_resolve(self, ret_name);
                /* pass */
                if (((!_tr_str_eqv((ret_sym->name), (_tr_str_lit_len("", 0LL)))) && (ret_sym->ptr_region == 0LL))) {
                    /* pass */
                    if ((((unsigned long long)(ret_sym->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        if (_tr_str_eqv(((*ret_sym->ty)->name), (_tr_str_lit_len("Pointer", 7LL)))) {
                            /* pass */
                            ({ TrStr _at_t503 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[L-1] '", 7LL)), (ret_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a local Pointer that may not outlive this function call. Returning it is unsafe.\n      FIX: Annotate the return type with 'from <param>' if the pointer borrows from a parameter, or wrap the allocation in 'unsafe:' if it is heap-allocated.", 243LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t503); _tr_str_release(_at_t503); });
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t502;
                /* pass */
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SReturn(Sema_lower_expr(self, e)));
    } else if (_t491.tag == Stmt_SLet) {
        __auto_type name = _t491.data.SLet.name;
__auto_type ownership = _t491.data.SLet.ownership;
__auto_type is_mut = _t491.data.SLet.is_mut;
__auto_type is_const = _t491.data.SLet.is_const;
__auto_type is_shared = _t491.data.SLet.is_shared;
__auto_type ty_ptr = _t491.data.SLet.ty;
__auto_type val_ptr = _t491.data.SLet.val;
        /* pass */
        if (((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))))) {
            /* pass */
            __auto_type _t504 = (*val_ptr);
            if (_t504.tag == Expr_ELitNone) {
                /* pass */
                TrStr m7_ty_name = _tr_str_retain((*ty_ptr)->name);
                /* pass */
                if ((((((!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("Option", 6LL)))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("None", 4LL))))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("Pointer", 7LL)))))) {
                    /* pass */
                    ({ TrStr _at_t505 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[M-7] Cannot assign 'none' to '", 31LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' which has type '", 18LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (m7_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'. Only Option[T] can hold 'none'. FIX: Use 'Option[", 52LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (m7_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' as the type, or give '", 25LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' a real initial value.", 23LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t505); _tr_str_release(_at_t505); });
                }
                _tr_str_release(m7_ty_name);
            } else if (1) {
                __auto_type _ = _t504;
                /* pass */
            }
        }
        /* pass */
        AstType* ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            ty = (*ty_ptr);
        }
        /* pass */
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
        /* pass */
        if (((!_tr_str_eqv((ty->name), (_tr_str_lit_len("void", 4LL)))) && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("None", 4LL)))))) {
            /* pass */
            hval = Sema__patch_empty_dict_hint(self, hval, ty);
        }
        /* pass */
        if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("None", 4LL))))) {
            /* pass */
            ty = hir_expr_type(hval);
        }
        /* pass */
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && (!is_shared))) {
            /* pass */
            __auto_type _t506 = (*val_ptr);
            if (_t506.tag == Expr_EIdent) {
                __auto_type m1_src = _t506.data.EIdent.name;
                /* pass */
                Symbol* m1_sym = Sema_resolve(self, m1_src);
                /* pass */
                AstType* m1_ty = (*m1_sym->ty);
                /* pass */
                if (((((!Sema_is_primitive(self, m1_ty)) && (!Sema_is_copy_class(self, m1_ty->name))) && (!_tr_str_eqv((m1_sym->name), (_tr_str_lit_len("", 0LL))))) && (!m1_sym->is_shared))) {
                    /* pass */
                    Sema_check_not_moved(self, m1_src, m1_ty->name);
                    /* pass */
                    Sema_check_no_active_borrows(self, m1_src, m1_ty->name);
                    /* pass */
                    Sema_mark_moved(self, m1_src);
                }
            } else if (1) {
                __auto_type _ = _t506;
                /* pass */
            }
        }
        /* pass */
        Sema_declare(self, name, SymbolKind_make_SVariable(), box_asttype(ty), is_mut);
        /* pass */
        if ((is_shared && (self->scopes->len > 0LL))) {
            /* pass */
            Scope* _sh_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
            /* pass */
            if (_tr_dict_contains(_sh_scope->variables, _tr_strz(name))) {
                /* pass */
                Symbol* _sh_sym = ((Symbol*)(uintptr_t)_tr_dict_get(_sh_scope->variables, _tr_strz(name)));
                /* pass */
                _sh_sym->is_shared = true;
                /* pass */
                _tr_dict_set(_sh_scope->variables, _tr_strz(name), _sh_sym);
            }
        }
        /* pass */
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            ({ TrStr _at_t507 = (Sema_compute_region(self, val_ptr)); Sema_set_borrows_region(self, name, _at_t507); _tr_str_release(_at_t507); });
        }
        /* pass */
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t508 = (*val_ptr);
            if (_t508.tag == Expr_ECall) {
                __auto_type ce_callee = _t508.data.ECall.callee;
__auto_type ce_args = _t508.data.ECall.args;
                /* pass */
                if ((((unsigned long long)(ce_callee)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    __auto_type _t509 = (*ce_callee);
                    if (_t509.tag == Expr_EIdent) {
                        __auto_type ce_fn = _t509.data.EIdent.name;
                        /* pass */
                        Symbol* ce_sym = Sema_resolve(self, ce_fn);
                        /* pass */
                        if (((!_tr_str_eqv((ce_sym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(ce_sym->ty)) != ((unsigned long long)(0LL))))) {
                            /* pass */
                            AstType* ce_ret = (*ce_sym->ty);
                            /* pass */
                            if ((((ce_ret->is_borrow || (ce_ret->from_regions->len > 0LL)) && (ce_ret->from_index >= 0LL)) && (ce_ret->from_index < ce_args->len))) {
                                /* pass */
                                __auto_type _t510 = (*((Expr*)List_ptr_get(ce_args, ce_ret->from_index)));
                                if (_t510.tag == Expr_EIdent) {
                                    __auto_type ce_src = _t510.data.EIdent.name;
                                    /* pass */
                                    List_TrStr_append(self->cur_func_borrowers, name);
                                    /* pass */
                                    List_TrStr_append(self->cur_func_sources, ce_src);
                                } else if (1) {
                                    __auto_type _ = _t510;
                                    /* pass */
                                }
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t509;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t508;
                /* pass */
            }
        }
        /* pass */
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && _tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))))) {
            /* pass */
            bool heap_rhs = false;
            /* pass */
            __auto_type _t511 = (*val_ptr);
            if (_t511.tag == Expr_ECall) {
                heap_rhs = true;
            } else if (_t511.tag == Expr_EMethodCall) {
                heap_rhs = true;
            } else if (1) {
                __auto_type _ = _t511;
                /* pass */
            }
            /* pass */
            if (heap_rhs) {
                /* pass */
                if ((self->scopes->len > 0LL)) {
                    /* pass */
                    Scope* alloc_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
                    /* pass */
                    if (_tr_dict_contains(alloc_scope->variables, _tr_strz(name))) {
                        /* pass */
                        Symbol* alloc_sym = ((Symbol*)(uintptr_t)_tr_dict_get(alloc_scope->variables, _tr_strz(name)));
                        /* pass */
                        alloc_sym->ptr_region = 1LL;
                        /* pass */
                        _tr_dict_set(alloc_scope->variables, _tr_strz(name), alloc_sym);
                    }
                }
            }
        }
        /* pass */
        bool _decl_fixed_arr = false;
        /* pass */
        if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if ((_tr_str_eqv(((*ty_ptr)->name), (_tr_str_lit_len("Array", 5LL))) && ((*ty_ptr)->array_size > 0LL))) {
                /* pass */
                _decl_fixed_arr = true;
            }
        }
        /* pass */
        if (((((unsigned long long)(val_ptr)) == ((unsigned long long)(0LL))) && (!_decl_fixed_arr))) {
            /* pass */
            if ((self->scopes->len > 0LL)) {
                /* pass */
                Scope* pd_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
                /* pass */
                if (_tr_dict_contains(pd_scope->variables, _tr_strz(name))) {
                    /* pass */
                    Symbol* pd_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pd_scope->variables, _tr_strz(name)));
                    /* pass */
                    pd_sym->is_init = false;
                    /* pass */
                    _tr_dict_set(pd_scope->variables, _tr_strz(name), pd_sym);
                }
            }
        }
        /* pass */
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            TrStr pc_cont_nm = _tr_str_lit_len("", 0LL);
            /* pass */
            TrStr pc_meth = _tr_str_lit_len("", 0LL);
            /* pass */
            __auto_type _t512 = (*val_ptr);
            if (_t512.tag == Expr_EMethodCall) {
                __auto_type pc_obj = _t512.data.EMethodCall.obj;
__auto_type pc_m = _t512.data.EMethodCall.method;
                /* pass */
                TrStr _strtmp_t513 = _tr_str_retain(pc_m);
                _tr_str_release(pc_meth);
                pc_meth = _strtmp_t513;
                /* pass */
                __auto_type _t514 = (*pc_obj);
                if (_t514.tag == Expr_EIdent) {
                    __auto_type pc_src = _t514.data.EIdent.name;
                    TrStr _strtmp_t515 = _tr_str_retain(pc_src);
                    _tr_str_release(pc_cont_nm);
                    pc_cont_nm = _strtmp_t515;
                } else if (1) {
                    __auto_type _ = _t514;
                    /* pass */
                }
            } else if (_t512.tag == Expr_EIndex) {
                __auto_type pc_iobj = _t512.data.EIndex.obj;
                /* pass */
                TrStr _strtmp_t516 = _tr_str_lit_len("get", 3LL);
                _tr_str_release(pc_meth);
                pc_meth = _strtmp_t516;
                /* pass */
                __auto_type _t517 = (*pc_iobj);
                if (_t517.tag == Expr_EIdent) {
                    __auto_type pc_isrc = _t517.data.EIdent.name;
                    TrStr _strtmp_t518 = _tr_str_retain(pc_isrc);
                    _tr_str_release(pc_cont_nm);
                    pc_cont_nm = _strtmp_t518;
                } else if (1) {
                    __auto_type _ = _t517;
                    /* pass */
                }
            } else if (1) {
                __auto_type _ = _t512;
                /* pass */
            }
            /* pass */
            if (((!_tr_str_eqv((pc_cont_nm), (_tr_str_lit_len("", 0LL)))) && ((_tr_str_eqv((pc_meth), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("last", 4LL)))))) {
                /* pass */
                Symbol* pc_cont_sym = Sema_resolve(self, pc_cont_nm);
                /* pass */
                AstType* pc_cont_ty = (*pc_cont_sym->ty);
                /* pass */
                TrStr pc_ctn = _tr_str_retain(pc_cont_ty->name);
                /* pass */
                if ((_tr_str_eqv((pc_ctn), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("RwLock", 6LL))))) {
                    /* pass */
                    Sema_set_borrows_region(self, name, _tr_str_lit_len("@borrowed", 9LL));
                } else if (((((_tr_str_eqv((pc_ctn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("Set", 3LL))))) {
                    /* pass */
                    TrStr pc_elem_ty = _tr_str_retain(hir_expr_type(hval)->name);
                    /* pass */
                    bool pc_is_vt = (_tr_dict_contains(self->classes, _tr_strz(pc_elem_ty)) && (!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(pc_elem_ty)))->is_class));
                    /* pass */
                    if (((!Sema_is_primitive(self, hir_expr_type(hval))) && (!pc_is_vt))) {
                        /* pass */
                        _tr_dict_set(self->container_borrows, _tr_strz(pc_cont_nm), _tr_str_box(_tr_str_retain(name)));
                        /* pass */
                        if (((((((!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("str", 3LL)))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Vec", 3LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Dict", 4LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Map", 3LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Set", 3LL)))))) {
                            /* pass */
                            Sema_set_borrows_region(self, name, _tr_str_lit_len("@borrowed", 9LL));
                        }
                    }
                }
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SLet(name, ownership, is_mut, is_const, is_shared, ty, hval));
    } else if (_t491.tag == Stmt_SAssign) {
        __auto_type target = _t491.data.SAssign.target;
__auto_type val = _t491.data.SAssign.val;
        /* pass */
        self->in_assign_target = true;
        /* pass */
        HirExpr* htgt = Sema_lower_expr(self, target);
        /* pass */
        self->in_assign_target = false;
        /* pass */
        HirExpr* hv = Sema_lower_expr(self, val);
        /* pass */
        hv = Sema__patch_empty_dict_hint(self, hv, hir_expr_type(htgt));
        /* pass */
        if ((self->strict_mode && (((unsigned long long)(target)) != ((unsigned long long)(0LL))))) {
            /* pass */
            __auto_type _t519 = (*target);
            if (_t519.tag == Expr_EPropAccess) {
                __auto_type l5_obj = _t519.data.EPropAccess.obj;
__auto_type l5_field = _t519.data.EPropAccess.prop;
                /* pass */
                if ((_tr_str_eqv((Sema_compute_region(self, val)), (_tr_str_lit_len("@owned", 6LL))) && Sema_field_is_borrow(self, l5_obj, l5_field))) {
                    /* pass */
                    ({ TrStr _at_t520 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[L-5] storing a freshly-built (owned) string into the borrow field '", 68LL)), (l5_field))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' (declared 'ref').\n      FIX: store a borrow of the field's region, or make the field a plain owned 'str'.", 107LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t520); _tr_str_release(_at_t520); });
                }
            } else if (1) {
                __auto_type _ = _t519;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t521 = (*target);
            if (_t521.tag == Expr_EIdent) {
                __auto_type sa_decl_name = _t521.data.EIdent.name;
                /* pass */
                ({ TrStr _at_t522 = (Sema_compute_region(self, val)); Sema_set_borrows_region(self, sa_decl_name, _at_t522); _tr_str_release(_at_t522); });
                /* pass */
                Symbol* sa_existing = Sema_resolve(self, sa_decl_name);
                /* pass */
                if (_tr_str_eqv((sa_existing->name), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    Sema_declare(self, sa_decl_name, SymbolKind_make_SVariable(), box_asttype(hir_expr_type(hv)), false);
                } else if ((((self->scopes->len > 0LL) && Sema_is_global_not_local(self, sa_decl_name)) && (!sa_existing->is_mut))) {
                    /* pass */
                    Sema_declare(self, sa_decl_name, SymbolKind_make_SVariable(), box_asttype(hir_expr_type(hv)), true);
                    /* pass */
                    Sema_mark_init(self, sa_decl_name);
                    /* pass */
                    return box_hirstmt(HirStmt_ctor_SLet(sa_decl_name, Ownership_make_Own(), true, false, false, hir_expr_type(hv), hv));
                } else if (((sa_existing->kind.tag == SymbolKind_make_SVariable().tag) && (!sa_existing->is_mut))) {
                    /* pass */
                    ({ TrStr _at_t523 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[M-8] Cannot assign to '", 24LL)), (sa_decl_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' a second time because it is immutable.\n      FIX: Declare it as 'mut ", 71LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sa_decl_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = ...' if it needs to change.", 30LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t523); _tr_str_release(_at_t523); });
                }
            } else if (1) {
                __auto_type _ = _t521;
                /* pass */
            }
        }
        /* pass */
        bool sa_target_is_plain_ident = false;
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t524 = (*target);
            if (_t524.tag == Expr_EIdent) {
                __auto_type _ = _t524.data.EIdent.name;
                sa_target_is_plain_ident = true;
            } else if (1) {
                __auto_type _ = _t524;
                /* pass */
            }
        }
        /* pass */
        if ((sa_target_is_plain_ident && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
            /* pass */
            __auto_type _t525 = (*val);
            if (_t525.tag == Expr_EIdent) {
                __auto_type sa_src = _t525.data.EIdent.name;
                /* pass */
                Symbol* sa_sym = Sema_resolve(self, sa_src);
                /* pass */
                AstType* sa_ty = (*sa_sym->ty);
                /* pass */
                bool sa_is_known = (_tr_dict_contains(self->classes, _tr_strz(sa_ty->name)) || _tr_dict_contains(self->enums, _tr_strz(sa_ty->name)));
                /* pass */
                if (((((sa_is_known && (!Sema_is_primitive(self, sa_ty))) && (!Sema_is_copy_class(self, sa_ty->name))) && (!_tr_str_eqv((sa_sym->name), (_tr_str_lit_len("", 0LL))))) && (!sa_sym->is_shared))) {
                    /* pass */
                    Sema_check_no_active_borrows(self, sa_src, sa_ty->name);
                    /* pass */
                    Sema_mark_moved(self, sa_src);
                }
            } else if (1) {
                __auto_type _ = _t525;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t526 = (*target);
            if (_t526.tag == Expr_EIdent) {
                __auto_type pd_tgt = _t526.data.EIdent.name;
                Sema_mark_init(self, pd_tgt);
            } else if (1) {
                __auto_type _ = _t526;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t527 = (*target);
            if (_t527.tag == Expr_EIdent) {
                __auto_type pc_tgt = _t527.data.EIdent.name;
                Sema_clear_container_borrow(self, pc_tgt);
            } else if (1) {
                __auto_type _ = _t527;
                /* pass */
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SAssign(htgt, hv));
    } else if (_t491.tag == Stmt_SIf) {
        __auto_type cond = _t491.data.SIf.cond;
__auto_type then_b = _t491.data.SIf.then_b;
__auto_type elifs = _t491.data.SIf.elifs;
__auto_type else_b = _t491.data.SIf.else_b;
        /* pass */
        if ((((unsigned long long)(cond)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t528 = (*cond);
            if (_t528.tag == Expr_EIdent) {
                __auto_type t5_name = _t528.data.EIdent.name;
                /* pass */
                Symbol* t5_sym = Sema_resolve(self, t5_name);
                /* pass */
                TrStr t5_ty = _tr_str_retain((*t5_sym->ty)->name);
                /* pass */
                if (((((_tr_str_eqv((t5_ty), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("f64", 3LL))))) {
                    /* pass */
                    ({ TrStr _at_t529 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-5] '", 7LL)), (t5_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a number (", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (t5_ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(") and cannot be used as an 'if' condition. FIX: Write 'if ", 58LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (t5_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" != 0:' to explicitly check for non-zero.", 41LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t529); _tr_str_release(_at_t529); });
                }
                _tr_str_release(t5_ty);
            } else if (1) {
                __auto_type _ = _t528;
                /* pass */
            }
        }
        /* pass */
        bool si_outer_cap_m = self->capturing_moves;
        /* pass */
        List_TrStr* si_outer_buf_m = self->branch_moved_buf;
        /* pass */
        bool si_outer_cap_i = self->capturing_inits;
        /* pass */
        List_TrStr* si_outer_buf_i = self->branch_init_buf;
        /* pass */
        self->capturing_moves = true;
        /* pass */
        self->capturing_inits = true;
        /* pass */
        self->branch_moved_buf = (void*)List_TrStr_new();
        /* pass */
        self->branch_init_buf = (void*)List_TrStr_new();
        /* pass */
        HirExpr* hcond = Sema_lower_expr(self, cond);
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        HirBlock* hthen = Sema_lower_block(self, then_b);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        List_TrStr* si_then_moved = self->branch_moved_buf;
        /* pass */
        List_TrStr* si_then_inited = self->branch_init_buf;
        /* pass */
        long long si_uti = 0LL;
        /* pass */
        while ((si_uti < si_then_moved->len)) {
            /* pass */
            ({ TrStr _at_t530 = (List_TrStr_get(si_then_moved, si_uti)); Sema_unmark_moved(self, _at_t530); _tr_str_release(_at_t530); });
            /* pass */
            si_uti = (si_uti + 1LL);
        }
        /* pass */
        long long si_uii = 0LL;
        /* pass */
        while ((si_uii < si_then_inited->len)) {
            /* pass */
            ({ TrStr _at_t531 = (List_TrStr_get(si_then_inited, si_uii)); Sema_unmark_init(self, _at_t531); _tr_str_release(_at_t531); });
            /* pass */
            si_uii = (si_uii + 1LL);
        }
        /* pass */
        self->branch_moved_buf = (void*)List_TrStr_new();
        /* pass */
        self->branch_init_buf = (void*)List_TrStr_new();
        /* pass */
        HirBlock* helse = HirBlock_init();
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        if ((elifs->len > 0LL)) {
            /* pass */
            Sema_open_block(self);
            /* pass */
            HirBlock* base_else = Sema_lower_block(self, else_b);
            /* pass */
            Sema_close_block(self);
            /* pass */
            HirBlock* chain = _tr_obj_retain(base_else);
            /* pass */
            long long k = (elifs->len - 1LL);
            /* pass */
            while ((k >= 0LL)) {
                /* pass */
                ElifClause* elif_c = ((ElifClause*)List_ptr_get(elifs, k));
                /* pass */
                HirExpr* elif_cond = Sema_lower_expr(self, elif_c->cond);
                /* pass */
                Sema_open_block(self);
                /* pass */
                HirBlock* elif_body = Sema_lower_block(self, (*elif_c->body));
                /* pass */
                Sema_close_block(self);
                /* pass */
                HirBlock* nested = HirBlock_init();
                /* pass */
                HirBlock_push(nested, box_hirstmt(HirStmt_ctor_SIf(elif_cond, elif_body, chain)));
                /* pass */
                HirBlock* _cltmp_t532 = _tr_obj_retain(nested);
                _tr_obj_release(chain, _trdrop_HirBlock);
                chain = _cltmp_t532;
                /* pass */
                k = (k - 1LL);
                _tr_obj_release(elif_body, _trdrop_HirBlock);
                _tr_obj_release(nested, _trdrop_HirBlock);
            }
            /* pass */
            HirBlock* _cltmp_t533 = _tr_obj_retain(chain);
            _tr_obj_release(helse, _trdrop_HirBlock);
            helse = _cltmp_t533;
            _tr_obj_release(base_else, _trdrop_HirBlock);
        } else {
            /* pass */
            HirBlock* _cltmp_t534 = Sema_lower_block(self, else_b);
            _tr_obj_release(helse, _trdrop_HirBlock);
            helse = _cltmp_t534;
        }
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        List_TrStr* si_else_moved = self->branch_moved_buf;
        /* pass */
        List_TrStr* si_else_inited = self->branch_init_buf;
        /* pass */
        long long si_uei = 0LL;
        /* pass */
        while ((si_uei < si_else_moved->len)) {
            /* pass */
            ({ TrStr _at_t535 = (List_TrStr_get(si_else_moved, si_uei)); Sema_unmark_moved(self, _at_t535); _tr_str_release(_at_t535); });
            /* pass */
            si_uei = (si_uei + 1LL);
        }
        /* pass */
        long long si_uei2 = 0LL;
        /* pass */
        while ((si_uei2 < si_else_inited->len)) {
            /* pass */
            ({ TrStr _at_t536 = (List_TrStr_get(si_else_inited, si_uei2)); Sema_unmark_init(self, _at_t536); _tr_str_release(_at_t536); });
            /* pass */
            si_uei2 = (si_uei2 + 1LL);
        }
        /* pass */
        self->capturing_moves = si_outer_cap_m;
        /* pass */
        self->branch_moved_buf = si_outer_buf_m;
        /* pass */
        self->capturing_inits = si_outer_cap_i;
        /* pass */
        self->branch_init_buf = si_outer_buf_i;
        /* pass */
        bool si_then_jumps = Sema_block_ends_in_jump(self, hthen);
        /* pass */
        bool si_else_jumps = Sema_block_ends_in_jump(self, helse);
        /* pass */
        if ((si_then_jumps && (!si_else_jumps))) {
            /* pass */
            long long si_ei0 = 0LL;
            /* pass */
            while ((si_ei0 < si_else_moved->len)) {
                /* pass */
                ({ TrStr _at_t537 = (List_TrStr_get(si_else_moved, si_ei0)); Sema_mark_moved(self, _at_t537); _tr_str_release(_at_t537); });
                /* pass */
                si_ei0 = (si_ei0 + 1LL);
            }
            /* pass */
            long long si_ini0 = 0LL;
            /* pass */
            while ((si_ini0 < si_else_inited->len)) {
                /* pass */
                ({ TrStr _at_t538 = (List_TrStr_get(si_else_inited, si_ini0)); Sema_mark_init(self, _at_t538); _tr_str_release(_at_t538); });
                /* pass */
                si_ini0 = (si_ini0 + 1LL);
            }
        } else if ((si_else_jumps && (!si_then_jumps))) {
            /* pass */
            long long si_mi0 = 0LL;
            /* pass */
            while ((si_mi0 < si_then_moved->len)) {
                /* pass */
                ({ TrStr _at_t539 = (List_TrStr_get(si_then_moved, si_mi0)); Sema_mark_moved(self, _at_t539); _tr_str_release(_at_t539); });
                /* pass */
                si_mi0 = (si_mi0 + 1LL);
            }
            /* pass */
            long long si_ti0 = 0LL;
            /* pass */
            while ((si_ti0 < si_then_inited->len)) {
                /* pass */
                ({ TrStr _at_t540 = (List_TrStr_get(si_then_inited, si_ti0)); Sema_mark_init(self, _at_t540); _tr_str_release(_at_t540); });
                /* pass */
                si_ti0 = (si_ti0 + 1LL);
            }
        } else if ((si_then_jumps && si_else_jumps)) {
            /* pass */
            /* pass */
        } else {
            /* pass */
            long long si_mi = 0LL;
            /* pass */
            while ((si_mi < si_then_moved->len)) {
                /* pass */
                TrStr si_mn = List_TrStr_get(si_then_moved, si_mi);
                /* pass */
                if (Sema_vec_str_contains(self, si_else_moved, si_mn)) {
                    /* pass */
                    Sema_mark_moved(self, si_mn);
                } else {
                    /* pass */
                    Sema_mark_maybe_moved(self, si_mn);
                }
                /* pass */
                si_mi = (si_mi + 1LL);
                _tr_str_release(si_mn);
            }
            /* pass */
            long long si_ei = 0LL;
            /* pass */
            while ((si_ei < si_else_moved->len)) {
                /* pass */
                TrStr si_en = List_TrStr_get(si_else_moved, si_ei);
                /* pass */
                if ((!Sema_vec_str_contains(self, si_then_moved, si_en))) {
                    /* pass */
                    Sema_mark_maybe_moved(self, si_en);
                }
                /* pass */
                si_ei = (si_ei + 1LL);
                _tr_str_release(si_en);
            }
            /* pass */
            long long si_ini = 0LL;
            /* pass */
            while ((si_ini < si_then_inited->len)) {
                /* pass */
                TrStr si_inn = List_TrStr_get(si_then_inited, si_ini);
                /* pass */
                if (Sema_vec_str_contains(self, si_else_inited, si_inn)) {
                    /* pass */
                    Sema_mark_init(self, si_inn);
                } else {
                    /* pass */
                    Sema_mark_maybe_init(self, si_inn);
                }
                /* pass */
                si_ini = (si_ini + 1LL);
                _tr_str_release(si_inn);
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SIf(hcond, hthen, helse));
    } else if (_t491.tag == Stmt_SWhile) {
        __auto_type cond = _t491.data.SWhile.cond;
__auto_type body = _t491.data.SWhile.body;
__auto_type decorators = _t491.data.SWhile.decorators;
        /* pass */
        HirExpr* sw_cond = Sema_lower_expr(self, cond);
        /* pass */
        bool sw_outer_cap_m = self->capturing_moves;
        /* pass */
        List_TrStr* sw_outer_buf_m = self->branch_moved_buf;
        /* pass */
        bool sw_outer_cap_i = self->capturing_inits;
        /* pass */
        List_TrStr* sw_outer_buf_i = self->branch_init_buf;
        /* pass */
        self->capturing_moves = true;
        /* pass */
        self->capturing_inits = true;
        /* pass */
        self->branch_moved_buf = (void*)List_TrStr_new();
        /* pass */
        self->branch_init_buf = (void*)List_TrStr_new();
        /* pass */
        List_i64_append(self->loop_scope_base, self->scopes->len);
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        long long sw_block_id = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
        /* pass */
        HirBlock* sw_body = Sema_lower_block(self, body);
        /* pass */
        Sema_append_block_local_drops(self, sw_body, sw_block_id);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        List_i64_pop(self->loop_scope_base);
        /* pass */
        List_TrStr* sw_loop_moved = self->branch_moved_buf;
        /* pass */
        List_TrStr* sw_loop_inited = self->branch_init_buf;
        /* pass */
        self->capturing_moves = sw_outer_cap_m;
        /* pass */
        self->branch_moved_buf = sw_outer_buf_m;
        /* pass */
        self->capturing_inits = sw_outer_cap_i;
        /* pass */
        self->branch_init_buf = sw_outer_buf_i;
        /* pass */
        long long sw_mi = 0LL;
        /* pass */
        while ((sw_mi < sw_loop_moved->len)) {
            /* pass */
            ({ TrStr _at_t541 = (List_TrStr_get(sw_loop_moved, sw_mi)); Sema_unmark_moved(self, _at_t541); _tr_str_release(_at_t541); });
            /* pass */
            ({ TrStr _at_t542 = (List_TrStr_get(sw_loop_moved, sw_mi)); Sema_mark_maybe_moved(self, _at_t542); _tr_str_release(_at_t542); });
            /* pass */
            sw_mi = (sw_mi + 1LL);
        }
        /* pass */
        long long sw_ii = 0LL;
        /* pass */
        while ((sw_ii < sw_loop_inited->len)) {
            /* pass */
            ({ TrStr _at_t543 = (List_TrStr_get(sw_loop_inited, sw_ii)); Sema_unmark_init(self, _at_t543); _tr_str_release(_at_t543); });
            /* pass */
            ({ TrStr _at_t544 = (List_TrStr_get(sw_loop_inited, sw_ii)); Sema_mark_maybe_init(self, _at_t544); _tr_str_release(_at_t544); });
            /* pass */
            sw_ii = (sw_ii + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SWhile(sw_cond, sw_body));
    } else if (_t491.tag == Stmt_SFor) {
        __auto_type var = _t491.data.SFor.var;
__auto_type iter = _t491.data.SFor.iter;
__auto_type body = _t491.data.SFor.body;
__auto_type decorators = _t491.data.SFor.decorators;
__auto_type for_is_ref = _t491.data.SFor.is_ref;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        HirExpr* h_iter_for = Sema_lower_expr(self, iter);
        /* pass */
        AstType* var_ty_for = AstType_init(_tr_str_lit_len("int", 3LL));
        /* pass */
        TrStr iter_hn = _tr_str_retain(hir_expr_type(h_iter_for)->name);
        /* pass */
        long long iter_hal = hir_expr_type(h_iter_for)->args->len;
        /* pass */
        if (((_tr_str_eqv((iter_hn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((iter_hn), (_tr_str_lit_len("Vec", 3LL)))) && (iter_hal > 0LL))) {
            /* pass */
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if ((_tr_str_eqv((iter_hn), (_tr_str_lit_len("Chan", 4LL))) && (iter_hal > 0LL))) {
            /* pass */
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if (_tr_str_eqv((iter_hn), (_tr_str_lit_len("str", 3LL)))) {
            /* pass */
            var_ty_for = AstType_init(_tr_str_lit_len("char", 4LL));
        }
        /* pass */
        Sema_declare(self, var, SymbolKind_make_SVariable(), box_asttype(var_ty_for), false);
        /* pass */
        if (for_is_ref) {
            /* pass */
            __auto_type _t545 = (*iter);
            if (_t545.tag == Expr_EIdent) {
                __auto_type for_coll = _t545.data.EIdent.name;
                /* pass */
                List_TrStr_append(self->cur_func_borrowers, var);
                /* pass */
                List_TrStr_append(self->cur_func_sources, for_coll);
            } else if (1) {
                __auto_type _ = _t545;
                /* pass */
            }
        }
        /* pass */
        bool sf_outer_cap_m = self->capturing_moves;
        /* pass */
        List_TrStr* sf_outer_buf_m = self->branch_moved_buf;
        /* pass */
        bool sf_outer_cap_i = self->capturing_inits;
        /* pass */
        List_TrStr* sf_outer_buf_i = self->branch_init_buf;
        /* pass */
        self->capturing_moves = true;
        /* pass */
        self->capturing_inits = true;
        /* pass */
        self->branch_moved_buf = (void*)List_TrStr_new();
        /* pass */
        self->branch_init_buf = (void*)List_TrStr_new();
        /* pass */
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
        /* pass */
        HirBlock* sf_body = Sema_lower_block(self, body);
        /* pass */
        List_i64_pop(self->loop_scope_base);
        /* pass */
        List_TrStr* sf_loop_moved = self->branch_moved_buf;
        /* pass */
        List_TrStr* sf_loop_inited = self->branch_init_buf;
        /* pass */
        self->capturing_moves = sf_outer_cap_m;
        /* pass */
        self->branch_moved_buf = sf_outer_buf_m;
        /* pass */
        self->capturing_inits = sf_outer_cap_i;
        /* pass */
        self->branch_init_buf = sf_outer_buf_i;
        /* pass */
        long long sf_mi = 0LL;
        /* pass */
        while ((sf_mi < sf_loop_moved->len)) {
            /* pass */
            ({ TrStr _at_t546 = (List_TrStr_get(sf_loop_moved, sf_mi)); Sema_unmark_moved(self, _at_t546); _tr_str_release(_at_t546); });
            /* pass */
            ({ TrStr _at_t547 = (List_TrStr_get(sf_loop_moved, sf_mi)); Sema_mark_maybe_moved(self, _at_t547); _tr_str_release(_at_t547); });
            /* pass */
            sf_mi = (sf_mi + 1LL);
        }
        /* pass */
        long long sf_ii = 0LL;
        /* pass */
        while ((sf_ii < sf_loop_inited->len)) {
            /* pass */
            ({ TrStr _at_t548 = (List_TrStr_get(sf_loop_inited, sf_ii)); Sema_unmark_init(self, _at_t548); _tr_str_release(_at_t548); });
            /* pass */
            ({ TrStr _at_t549 = (List_TrStr_get(sf_loop_inited, sf_ii)); Sema_mark_maybe_init(self, _at_t549); _tr_str_release(_at_t549); });
            /* pass */
            sf_ii = (sf_ii + 1LL);
        }
        /* pass */
        Sema_finalize_scope_drops(self, sf_body);
        /* pass */
        HirStmt* hstmt = box_hirstmt(HirStmt_ctor_SFor(var, h_iter_for, sf_body));
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        _tr_str_release(iter_hn);
        _tr_obj_release(sf_body, _trdrop_HirBlock);
        return hstmt;
    } else if (_t491.tag == Stmt_SForUnpack) {
        __auto_type vars = _t491.data.SForUnpack.vars;
__auto_type iter = _t491.data.SForUnpack.iter;
__auto_type body = _t491.data.SForUnpack.body;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        HirExpr* h_iter_fu = Sema_lower_expr(self, iter);
        /* pass */
        List_ptr* fu_tys = (void*)List_ptr_new();
        /* pass */
        long long fu_ti = 0LL;
        /* pass */
        while ((fu_ti < vars->len)) {
            /* pass */
            List_ptr_append(fu_tys, AstType_init(_tr_str_lit_len("int", 3LL)));
            /* pass */
            fu_ti = (fu_ti + 1LL);
        }
        /* pass */
        __auto_type _t550 = (*h_iter_fu);
        if (_t550.tag == HirExpr_ECall) {
            __auto_type fu_callee = _t550.data.ECall.callee;
__auto_type fu_args = _t550.data.ECall.args;
            /* pass */
            __auto_type _t551 = (*fu_callee);
            if (_t551.tag == HirExpr_EIdent) {
                __auto_type fu_fn = _t551.data.EIdent.name;
                /* pass */
                if (((_tr_str_eqv((fu_fn), (_tr_str_lit_len("enumerate", 9LL))) && (fu_args->len == 1LL)) && (vars->len >= 2LL))) {
                    /* pass */
                    TrStr fu_col_ty_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name);
                    /* pass */
                    long long fu_col_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
                    /* pass */
                    if (((_tr_str_eqv((fu_col_ty_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fu_col_ty_n), (_tr_str_lit_len("Vec", 3LL)))) && (fu_col_al > 0LL))) {
                        /* pass */
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
                } else if (((_tr_str_eqv((fu_fn), (_tr_str_lit_len("zip", 3LL))) && (fu_args->len == 2LL)) && (vars->len >= 2LL))) {
                    /* pass */
                    TrStr fu_a_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name);
                    /* pass */
                    long long fu_a_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
                    /* pass */
                    TrStr fu_b_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->name);
                    /* pass */
                    long long fu_b_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args->len;
                    /* pass */
                    if (((_tr_str_eqv((fu_a_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fu_a_n), (_tr_str_lit_len("Vec", 3LL)))) && (fu_a_al > 0LL))) {
                        /* pass */
                        List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
                    /* pass */
                    if (((_tr_str_eqv((fu_b_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fu_b_n), (_tr_str_lit_len("Vec", 3LL)))) && (fu_b_al > 0LL))) {
                        /* pass */
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args, 0LL))));
                    }
                }
            } else if (1) {
                __auto_type _ = _t551;
                /* pass */
            }
        } else if (_t550.tag == HirExpr_EMethodCall) {
            __auto_type fu_obj = _t550.data.EMethodCall.obj;
__auto_type fu_meth = _t550.data.EMethodCall.method;
            /* pass */
            if ((_tr_str_eqv((fu_meth), (_tr_str_lit_len("items", 5LL))) && (vars->len >= 2LL))) {
                /* pass */
                TrStr fu_dty_n = _tr_str_retain(hir_expr_type(fu_obj)->name);
                /* pass */
                long long fu_dty_al = hir_expr_type(fu_obj)->args->len;
                /* pass */
                if (((_tr_str_eqv((fu_dty_n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((fu_dty_n), (_tr_str_lit_len("Map", 3LL)))) && (fu_dty_al >= 2LL))) {
                    /* pass */
                    List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 0LL))));
                    /* pass */
                    List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 1LL))));
                }
            }
        } else if (1) {
            __auto_type _ = _t550;
            /* pass */
        }
        /* pass */
        long long vi_fu = 0LL;
        /* pass */
        while ((vi_fu < vars->len)) {
            /* pass */
            ({ TrStr _at_t552 = (List_TrStr_get(vars, vi_fu)); Sema_declare(self, _at_t552, SymbolKind_make_SVariable(), box_asttype(((AstType*)List_ptr_get(fu_tys, vi_fu))), false); _tr_str_release(_at_t552); });
            /* pass */
            vi_fu = (vi_fu + 1LL);
        }
        /* pass */
        HirBlock* hblk_fu = Sema_lower_block(self, body);
        /* pass */
        Sema_finalize_scope_drops(self, hblk_fu);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        List_ptr_free(fu_tys);
        return box_hirstmt(HirStmt_ctor_SForUnpack(vars, h_iter_fu, hblk_fu));
    } else if (_t491.tag == Stmt_SMatch) {
        __auto_type subj = _t491.data.SMatch.expr;
__auto_type arms = _t491.data.SMatch.arms;
        /* pass */
        HirExpr* hsubj = Sema_lower_expr(self, subj);
        /* pass */
        TrStr ex_ty_name = _tr_str_retain(hir_expr_type(hsubj)->name);
        /* pass */
        List_ptr* h_arms = (void*)List_ptr_new();
        /* pass */
        bool ex_has_wild = false;
        /* pass */
        List_TrStr* ex_covered = (void*)List_TrStr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < arms->len)) {
            /* pass */
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, k));
            /* pass */
            __auto_type _t553 = arm->pat;
            if (_t553.tag == Pattern_PWild) {
                ex_has_wild = true;
            } else if (_t553.tag == Pattern_PBind) {
                __auto_type _ = _t553.data.PBind.name;
                ex_has_wild = true;
            } else if (_t553.tag == Pattern_PVariant) {
                __auto_type ex_vn = _t553.data.PVariant.variant;
                List_TrStr_append(ex_covered, ex_vn);
            } else if (_t553.tag == Pattern_PVariantBind) {
                __auto_type ex_vn2 = _t553.data.PVariantBind.variant;
                List_TrStr_append(ex_covered, ex_vn2);
            } else if (_t553.tag == Pattern_PVariantBindMany) {
                __auto_type ex_vn3 = _t553.data.PVariantBindMany.variant;
                List_TrStr_append(ex_covered, ex_vn3);
            } else if (_t553.tag == Pattern_POr) {
                __auto_type ex_orpats = _t553.data.POr.patterns;
                /* pass */
                long long ex_oi = 0LL;
                /* pass */
                while ((ex_oi < ex_orpats->len)) {
                    /* pass */
                    __auto_type _t554 = List_Pattern_get(ex_orpats, ex_oi);
                    if (_t554.tag == Pattern_PVariant) {
                        __auto_type ex_ovn = _t554.data.PVariant.variant;
                        List_TrStr_append(ex_covered, ex_ovn);
                    } else if (_t554.tag == Pattern_PVariantBind) {
                        __auto_type ex_bvn = _t554.data.PVariantBind.variant;
                        /* pass */
                        List_TrStr_append(ex_covered, ex_bvn);
                        /* pass */
                        ({ TrStr _at_t555 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[E-3] Payload binding '", 23LL)), (ex_ty_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ex_bvn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(..)' is not allowed inside an or-pattern ('|').\n      FIX: Split the alternatives into separate 'case' arms, or match the variant without binding: 'case ", 154LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ex_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ex_bvn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" | ...:'.", 9LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t555); _tr_str_release(_at_t555); });
                    } else if (_t554.tag == Pattern_PVariantBindMany) {
                        __auto_type ex_mvn = _t554.data.PVariantBindMany.variant;
                        /* pass */
                        List_TrStr_append(ex_covered, ex_mvn);
                        /* pass */
                        ({ TrStr _at_t556 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[E-3] Payload binding '", 23LL)), (ex_ty_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ex_mvn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(..)' is not allowed inside an or-pattern ('|').\n      FIX: Split the alternatives into separate 'case' arms, or match the variant without binding: 'case ", 154LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ex_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ex_mvn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" | ...:'.", 9LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t556); _tr_str_release(_at_t556); });
                    } else if (1) {
                        __auto_type _ = _t554;
                        /* pass */
                    }
                    /* pass */
                    ex_oi = (ex_oi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t553;
                /* pass */
            }
            /* pass */
            Sema_enter_scope(self);
            /* pass */
            AstType* _subj_ty = hir_expr_type(hsubj);
            /* pass */
            Sema_declare_pattern_binds_typed(self, arm->pat, _subj_ty);
            /* pass */
            HirMatchArm* h_arm = ({ HirBlock* _aot_t557 = (Sema_lower_block(self, (*arm->body))); __auto_type _wr = (HirMatchArm_init(arm->pat, _aot_t557)); _tr_obj_release(_aot_t557, _trdrop_HirBlock); _wr; });
            /* pass */
            if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
                /* pass */
                h_arm->guard = Sema_lower_expr(self, arm->guard);
            }
            /* pass */
            Sema_finalize_scope_drops(self, h_arm->body);
            /* pass */
            List_ptr_append(h_arms, _tr_obj_retain(h_arm));
            /* pass */
            Sema_exit_scope(self);
            /* pass */
            k = (k + 1LL);
            _tr_obj_release(h_arm, _trdrop_HirMatchArm);
        }
        /* pass */
        if (((!ex_has_wild) && _tr_dict_contains(self->enums, _tr_strz(ex_ty_name)))) {
            /* pass */
            EnumDef* ex_edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(ex_ty_name)));
            /* pass */
            List_TrStr* ex_missing = (void*)List_TrStr_new();
            /* pass */
            long long ex_vi = 0LL;
            /* pass */
            while ((ex_vi < ex_edef->variants->len)) {
                /* pass */
                TrStr ex_vname = _tr_str_retain(((VariantDef*)List_ptr_get(ex_edef->variants, ex_vi))->name);
                /* pass */
                if ((!List_TrStr_contains(ex_covered, ex_vname))) {
                    /* pass */
                    List_TrStr_append(ex_missing, ex_vname);
                }
                /* pass */
                ex_vi = (ex_vi + 1LL);
                _tr_str_release(ex_vname);
            }
            /* pass */
            if ((ex_missing->len > 0LL)) {
                /* pass */
                TrStr ex_msg = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] Non-exhaustive match on '", 31LL)), (ex_ty_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': missing variant", 18LL))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((ex_missing->len > 1LL)) {
                    /* pass */
                    TrStr _strtmp_t558 = _tr_strx_concatv((ex_msg), (_tr_str_lit_len("s", 1LL)));
                    _tr_str_release(ex_msg);
                    ex_msg = _strtmp_t558;
                }
                /* pass */
                TrStr _strtmp_t559 = _tr_strx_concatv((ex_msg), (_tr_str_lit_len(": ", 2LL)));
                _tr_str_release(ex_msg);
                ex_msg = _strtmp_t559;
                /* pass */
                long long ex_mi = 0LL;
                /* pass */
                while ((ex_mi < ex_missing->len)) {
                    /* pass */
                    if ((ex_mi > 0LL)) {
                        /* pass */
                        TrStr _strtmp_t560 = _tr_strx_concatv((ex_msg), (_tr_str_lit_len(", ", 2LL)));
                        _tr_str_release(ex_msg);
                        ex_msg = _strtmp_t560;
                    }
                    /* pass */
                    TrStr _strtmp_t561 = ({ TrStr _cr = (List_TrStr_get(ex_missing, ex_mi)); TrStr _cres = _tr_strx_concatv((ex_msg), _cr); _tr_str_release(_cr); _cres; });
                    _tr_str_release(ex_msg);
                    ex_msg = _strtmp_t561;
                    /* pass */
                    ex_mi = (ex_mi + 1LL);
                }
                /* pass */
                TrStr _strtmp_t562 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((ex_msg), (_tr_str_lit_len(".\n      FIX: Add a 'case ", 25LL)))); TrStr _cres = _tr_strx_concatv(_cl, (ex_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".VariantName:' arm for each missing variant, or add 'case _:' to handle all remaining cases.", 92LL))); _tr_str_release(_cl); _cres; });
                _tr_str_release(ex_msg);
                ex_msg = _strtmp_t562;
                /* pass */
                Sema_error(self, ex_msg);
            }
        }
        /* pass */
        _tr_str_release(ex_ty_name);
        List_TrStr_free(ex_covered);
        return box_hirstmt(HirStmt_ctor_SMatch(hsubj, h_arms));
    } else if (_t491.tag == Stmt_STry) {
        __auto_type try_body = _t491.data.STry.try_body;
__auto_type catches = _t491.data.STry.catches;
__auto_type finally_b = _t491.data.STry.finally_b;
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        HirBlock* h_try_body = Sema_lower_block(self, try_body);
        /* pass */
        List_ptr* h_catches = (void*)List_ptr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < catches->len)) {
            /* pass */
            CatchClause* cc = (*((CatchClause**)List_ptr_get(catches, k)));
            /* pass */
            HirCatchClause* hcc_val = ((HirCatchClause*)_tr_obj_alloc(sizeof(HirCatchClause)));
            /* pass */
            hcc_val->err_name = _tr_str_retain(cc->err_name);
            /* pass */
            hcc_val->err_type = AstType_init(_tr_str_lit_len("str", 3LL));
            /* pass */
            if ((((unsigned long long)(cc->err_type)) != ((unsigned long long)(0LL)))) {
                /* pass */
                hcc_val->err_type = (*cc->err_type);
            }
            /* pass */
            if ((!_tr_str_eqv((cc->err_name), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                Sema_declare(self, cc->err_name, SymbolKind_make_SVariable(), box_asttype(hcc_val->err_type), true);
            }
            /* pass */
            HirBlock* _cltmp_t563 = Sema_lower_block(self, (*cc->body));
            _tr_obj_release(hcc_val->body, _trdrop_HirBlock);
            hcc_val->body = _cltmp_t563;
            /* pass */
            /* unsafe block */
            /* pass */
            HirCatchClause** hcc = ((HirCatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(HirCatchClause*)));
            /* pass */
            (*hcc = hcc_val);
            /* pass */
            List_ptr_append(h_catches, hcc);
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        HirBlock* h_finally_b = Sema_lower_block(self, finally_b);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_STry(h_try_body, h_catches, h_finally_b));
    } else if (_t491.tag == Stmt_SRaise) {
        __auto_type e = _t491.data.SRaise.val;
        return box_hirstmt(HirStmt_ctor_SRaise(Sema_lower_expr(self, e)));
    } else if (_t491.tag == Stmt_SAssert) {
        __auto_type cond = _t491.data.SAssert.cond;
__auto_type msg = _t491.data.SAssert.msg;
        return box_hirstmt(HirStmt_ctor_SAssert(Sema_lower_expr(self, cond), Sema_lower_expr(self, msg)));
    } else if (_t491.tag == Stmt_SDefer) {
        __auto_type inner = _t491.data.SDefer.stmt;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SDefer(Sema_lower_stmt(self, inner)));
    } else if (_t491.tag == Stmt_SWith) {
        __auto_type items = _t491.data.SWith.items;
__auto_type aliases = _t491.data.SWith.aliases;
__auto_type body = _t491.data.SWith.body;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_ptr* h_items = (void*)List_ptr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < items->len)) {
            /* pass */
            HirExpr* h_wi = Sema_lower_expr(self, ((Expr*)List_ptr_get(items, k)));
            /* pass */
            List_ptr_append(h_items, h_wi);
            /* pass */
            if (((k < aliases->len) && (!_tr_str_eqv((List_TrStr_get(aliases, k)), (_tr_str_lit_len("", 0LL)))))) {
                /* pass */
                AstType* wi_ty = hir_expr_type(h_wi);
                /* pass */
                ({ TrStr _at_t564 = (List_TrStr_get(aliases, k)); Sema_declare(self, _at_t564, SymbolKind_make_SVariable(), box_asttype(wi_ty), true); _tr_str_release(_at_t564); });
            }
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        HirBlock* h_with_body = Sema_lower_block(self, body);
        /* pass */
        Sema_finalize_scope_drops(self, h_with_body);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_SWith(h_items, aliases, h_with_body));
    } else if (_t491.tag == Stmt_SAsm) {
        __auto_type code = _t491.data.SAsm.code;
__auto_type outputs = _t491.data.SAsm.outputs;
__auto_type inputs = _t491.data.SAsm.inputs;
__auto_type clobbers = _t491.data.SAsm.clobbers;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SAsm(code, outputs, inputs, clobbers));
    } else if (_t491.tag == Stmt_SSpawn) {
        __auto_type e = _t491.data.SSpawn.expr;
        /* pass */
        if ((!self->in_async_fn)) {
            /* pass */
            ({ TrStr _at_t565 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[C-5] 'spawn:' used outside an async function. FIX: Declare '", 61LL)), (self->current_func_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' as 'async def ", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (self->current_func_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(...)' to use spawn inside it.", 30LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t565); _tr_str_release(_at_t565); });
        }
        /* pass */
        HirExpr* spawn_lowered = Sema_lower_expr(self, e);
        /* pass */
        Sema_check_spawn_sendable(self, spawn_lowered);
        /* pass */
        return box_hirstmt(HirStmt_ctor_SSpawn(spawn_lowered));
    } else if (_t491.tag == Stmt_STaskGroup) {
        __auto_type body = _t491.data.STaskGroup.body;
        /* pass */
        if ((!self->in_async_fn)) {
            /* pass */
            ({ TrStr _at_t566 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[C-6] 'taskgroup:' used outside an async function. FIX: Declare '", 65LL)), (self->current_func_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' as 'async def ", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (self->current_func_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(...)' to use taskgroup inside it.", 34LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t566); _tr_str_release(_at_t566); });
        }
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        HirBlock* h_tg_body = Sema_lower_block(self, body);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_STaskGroup(h_tg_body));
    } else if (_t491.tag == Stmt_SChanSelect) {
        __auto_type cs_cases = _t491.data.SChanSelect.cases;
        /* pass */
        List_ptr* hcs_cases = (void*)List_ptr_new();
        /* pass */
        long long csi2 = 0LL;
        /* pass */
        while ((csi2 < cs_cases->len)) {
            /* pass */
            ChanSelectArm* arm = (*((ChanSelectArm**)List_ptr_get(cs_cases, csi2)));
            /* pass */
            HirChanSelectArm** harm = (HirChanSelectArm**)(0LL);
            /* pass */
            /* unsafe block */
            /* pass */
            harm = ((HirChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(HirChanSelectArm*)));
            /* pass */
            HirChanSelectArm* harm_v = ((HirChanSelectArm*)_tr_obj_alloc(sizeof(HirChanSelectArm)));
            /* pass */
            harm_v->kind = arm->kind;
            /* pass */
            harm_v->var_name = _tr_str_retain(arm->var_name);
            /* pass */
            if ((((unsigned long long)(arm->chan_expr)) != ((unsigned long long)(0LL)))) {
                /* pass */
                harm_v->chan_expr = Sema_lower_expr(self, arm->chan_expr);
            } else {
                /* pass */
                harm_v->chan_expr = (HirExpr*)(0LL);
            }
            /* pass */
            if ((((unsigned long long)(arm->val_expr)) != ((unsigned long long)(0LL)))) {
                /* pass */
                harm_v->val_expr = Sema_lower_expr(self, arm->val_expr);
            } else {
                /* pass */
                harm_v->val_expr = (HirExpr*)(0LL);
            }
            /* pass */
            if ((((unsigned long long)(arm->timeout_ms)) != ((unsigned long long)(0LL)))) {
                /* pass */
                harm_v->timeout_ms = Sema_lower_expr(self, arm->timeout_ms);
            } else {
                /* pass */
                harm_v->timeout_ms = (HirExpr*)(0LL);
            }
            /* pass */
            Sema_enter_scope(self);
            /* pass */
            if (((arm->kind == 0LL) && (!_tr_str_eqv((arm->var_name), (_tr_str_lit_len("", 0LL)))))) {
                /* pass */
                AstType* recv_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                /* pass */
                if ((((unsigned long long)(arm->chan_expr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    HirExpr* chan_hir = harm_v->chan_expr;
                    /* pass */
                    AstType* chan_ty = hir_expr_type(chan_hir);
                    /* pass */
                    if (((!_is_invalid_ptr(((unsigned long long)(chan_ty->args)))) && (chan_ty->args->len > 0LL))) {
                        /* pass */
                        recv_ty = (*((AstType**)List_ptr_get(chan_ty->args, 0LL)));
                    }
                }
                /* pass */
                Sema_declare(self, arm->var_name, SymbolKind_make_SVariable(), box_asttype(recv_ty), true);
            }
            /* pass */
            HirBlock* _cltmp_t567 = Sema_lower_block(self, arm->body);
            _tr_obj_release(harm_v->body, _trdrop_HirBlock);
            harm_v->body = _cltmp_t567;
            /* pass */
            Sema_finalize_scope_drops(self, harm_v->body);
            /* pass */
            Sema_exit_scope(self);
            /* pass */
            /* unsafe block */
            /* pass */
            (*harm = harm_v);
            /* pass */
            List_ptr_append(hcs_cases, harm);
            /* pass */
            csi2 = (csi2 + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SChanSelect(hcs_cases));
    } else if (_t491.tag == Stmt_SGpuBlock) {
        __auto_type body = _t491.data.SGpuBlock.body;
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        HirBlock* h_gpu_body = Sema_lower_block(self, body);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_SGpuBlock(h_gpu_body));
    } else if (_t491.tag == Stmt_SBreak) {
        __auto_type bv = _t491.data.SBreak.val;
        /* pass */
        HirExpr* hbv = (HirExpr*)(0LL);
        /* pass */
        if ((((unsigned long long)(bv)) != ((unsigned long long)(0LL)))) {
            /* pass */
            hbv = Sema_lower_expr(self, bv);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SBreak(hbv));
    } else if (_t491.tag == Stmt_SContinue) {
        return box_hirstmt(HirStmt_make_SContinue());
    } else if (_t491.tag == Stmt_SPass) {
        return box_hirstmt(HirStmt_make_SPass());
    } else if (_t491.tag == Stmt_SLocalDecl) {
        __auto_type ldecl = _t491.data.SLocalDecl.decl;
        /* pass */
        if ((!_tr_str_eqv((self->current_func_name), (_tr_str_lit_len("main", 4LL))))) {
            /* pass */
            Sema_error(self, _tr_str_lit_len("[E-2] Nested class/def/enum/interface declarations are only supported inside main().\n      FIX: Move this declaration to module (top-level) scope, or declare it inside main().", 175LL));
            /* pass */
            return box_hirstmt(HirStmt_make_SPass());
        }
        /* pass */
        Sema_register_decl(self, ldecl);
        /* pass */
        TrStr saved_ld_func_name = _tr_str_retain(self->current_func_name);
        /* pass */
        TrStr saved_ld_class_name = _tr_str_retain(self->current_class_name);
        /* pass */
        __auto_type _t568 = (*ldecl);
        if (_t568.tag == Decl_DFunction) {
            __auto_type ld_f = _t568.data.DFunction.func;
            /* pass */
            List_ptr_append(self->nested_functions, Sema_lower_func(self, ld_f));
        } else if (_t568.tag == Decl_DClass) {
            __auto_type ld_c = _t568.data.DClass.cls;
            /* pass */
            List_ptr_append(self->nested_classes, Sema_lower_class(self, ld_c));
        } else if (_t568.tag == Decl_DActor) {
            __auto_type ld_c = _t568.data.DActor.cls;
            /* pass */
            List_ptr_append(self->nested_classes, Sema_lower_class(self, ld_c));
        } else if (_t568.tag == Decl_DEnum) {
            __auto_type ld_e = _t568.data.DEnum.enm;
            /* pass */
            List_ptr_append(self->nested_enums, Sema_lower_enum(self, ld_e));
        } else if (_t568.tag == Decl_DInterface) {
            __auto_type ld_i = _t568.data.DInterface.iface;
            /* pass */
            List_ptr_append(self->nested_interfaces, Sema_lower_interface(self, ld_i));
        } else if (_t568.tag == Decl_DExtend) {
            __auto_type ld_target = _t568.data.DExtend.target;
__auto_type ld_methods = _t568.data.DExtend.methods;
            /* pass */
            self->current_class_name = _tr_str_retain(ld_target);
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(ld_target))) {
                /* pass */
                self->current_region_params = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ld_target)))->region_params;
            }
            /* pass */
            long long ld_ci = 0LL;
            /* pass */
            bool ld_found = false;
            /* pass */
            while ((ld_ci < self->nested_classes->len)) {
                /* pass */
                HirClass* ld_nc = ((HirClass*)List_ptr_get(self->nested_classes, ld_ci));
                /* pass */
                if (_tr_str_eqv((ld_nc->name), (ld_target))) {
                    /* pass */
                    long long ld_hi = 0LL;
                    /* pass */
                    while ((ld_hi < ld_methods->len)) {
                        /* pass */
                        List_ptr_append(ld_nc->methods, Sema_lower_func(self, ((FunctionDef*)List_ptr_get(ld_methods, ld_hi))));
                        /* pass */
                        ld_hi = (ld_hi + 1LL);
                    }
                    /* pass */
                    List_ptr_set(self->nested_classes, ld_ci, ld_nc);
                    /* pass */
                    ld_found = true;
                }
                /* pass */
                ld_ci = (ld_ci + 1LL);
            }
            /* pass */
            if ((!ld_found)) {
                /* pass */
                long long ld_hi2 = 0LL;
                /* pass */
                while ((ld_hi2 < ld_methods->len)) {
                    /* pass */
                    List_ptr_append(self->nested_functions, Sema_lower_func(self, ((FunctionDef*)List_ptr_get(ld_methods, ld_hi2))));
                    /* pass */
                    ld_hi2 = (ld_hi2 + 1LL);
                }
            }
        } else if (1) {
            __auto_type _ = _t568;
            /* pass */
        }
        /* pass */
        self->current_func_name = _tr_str_retain(saved_ld_func_name);
        /* pass */
        self->current_class_name = _tr_str_retain(saved_ld_class_name);
        /* pass */
        self->current_region_params = (void*)List_TrStr_new();
        /* pass */
        _tr_str_release(saved_ld_func_name);
        _tr_str_release(saved_ld_class_name);
        return box_hirstmt(HirStmt_make_SPass());
    } else if (_t491.tag == Stmt_SUnsafe) {
        __auto_type body = _t491.data.SUnsafe.body;
        /* pass */
        bool saved_unsafe = self->in_unsafe;
        /* pass */
        self->in_unsafe = true;
        /* pass */
        HirBlock* unsafe_hir = Sema_lower_block(self, body);
        /* pass */
        self->in_unsafe = saved_unsafe;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SUnsafe(unsafe_hir));
    } else if (_t491.tag == Stmt_SMultiLet) {
        __auto_type names = _t491.data.SMultiLet.names;
__auto_type is_mut = _t491.data.SMultiLet.is_mut;
__auto_type val_ptr = _t491.data.SMultiLet.val;
        /* pass */
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
        /* pass */
        AstType* val_ty = hir_expr_type(hval);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < names->len)) {
            /* pass */
            AstType* nty = AstType_init(_tr_str_lit_len("int", 3LL));
            /* pass */
            if ((val_ty->args->len > i)) {
                /* pass */
                nty = (*((AstType**)List_ptr_get(val_ty->args, i)));
            }
            /* pass */
            ({ TrStr _at_t569 = (List_TrStr_get(names, i)); Sema_declare(self, _at_t569, SymbolKind_make_SVariable(), box_asttype(nty), is_mut); _tr_str_release(_at_t569); });
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SMultiLet(names, is_mut, hval));
    } else if (_t491.tag == Stmt_SLine) {
        __auto_type n = _t491.data.SLine.n;
        /* pass */
        self->current_line = n;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SLineMarker(n));
    } else if (1) {
        __auto_type _ = _t491;
        return box_hirstmt(HirStmt_make_SPass());
    }
}

__attribute__((hot)) AstType* Sema_variant_field_ty(Sema* self, TrStr type_name, TrStr variant_name, long long field_idx) {
    /* pass */
    if (_tr_dict_contains(self->enums, _tr_strz(type_name))) {
        /* pass */
        EnumDef* enm = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(type_name)));
        /* pass */
        long long vi = 0LL;
        /* pass */
        while ((vi < enm->variants->len)) {
            /* pass */
            VariantDef* v = ((VariantDef*)List_ptr_get(enm->variants, vi));
            /* pass */
            if (_tr_str_eqv((v->name), (variant_name))) {
                /* pass */
                if ((field_idx < v->fields->len)) {
                    /* pass */
                    Param* fp = ((Param*)List_ptr_get(v->fields, field_idx));
                    /* pass */
                    if ((((unsigned long long)(fp->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        return (*fp->ty);
                    }
                }
                /* pass */
                return AstType_init(_tr_str_lit_len("void", 4LL));
            }
            /* pass */
            vi = (vi + 1LL);
        }
    }
    /* pass */
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) void Sema_declare_pattern_binds(Sema* self, Pattern pat) {
    /* pass */
    Sema_declare_pattern_binds_typed(self, pat, AstType_init(_tr_str_lit_len("int", 3LL)));
}

__attribute__((hot)) void Sema_declare_pattern_binds_typed(Sema* self, Pattern pat, AstType* subj_ty) {
    /* pass */
    __auto_type _t570 = pat;
    if (_t570.tag == Pattern_PBind) {
        __auto_type name = _t570.data.PBind.name;
        Sema_declare(self, name, SymbolKind_make_SVariable(), box_asttype(subj_ty), false);
    } else if (_t570.tag == Pattern_PVariantBind) {
        __auto_type type_name = _t570.data.PVariantBind.type_name;
__auto_type variant_name = _t570.data.PVariantBind.variant;
__auto_type field = _t570.data.PVariantBind.field;
        /* pass */
        AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, 0LL);
        /* pass */
        if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Option", 6LL))) && _tr_str_eqv((variant_name), (_tr_str_lit_len("Some", 4LL)))) && (subj_ty->args->len > 0LL))) {
            /* pass */
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 0LL)));
        } else if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((variant_name), (_tr_str_lit_len("Ok", 2LL)))) && (subj_ty->args->len > 0LL))) {
            /* pass */
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 0LL)));
        } else if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((variant_name), (_tr_str_lit_len("Err", 3LL)))) && (subj_ty->args->len > 1LL))) {
            /* pass */
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 1LL)));
        }
        /* pass */
        if (_tr_str_eqv((fty->name), (_tr_str_lit_len("void", 4LL)))) {
            /* pass */
            fty = AstType_init(_tr_str_lit_len("int", 3LL));
        }
        /* pass */
        Sema_declare(self, field, SymbolKind_make_SVariable(), box_asttype(fty), false);
    } else if (_t570.tag == Pattern_PVariantBindMany) {
        __auto_type type_name = _t570.data.PVariantBindMany.type_name;
__auto_type variant_name = _t570.data.PVariantBindMany.variant;
__auto_type fields = _t570.data.PVariantBindMany.fields;
        /* pass */
        long long _pi = 0LL;
        /* pass */
        while ((_pi < fields->len)) {
            /* pass */
            TrStr _pf = List_TrStr_get(fields, _pi);
            /* pass */
            if ((!_tr_str_eqv((_pf), (_tr_str_lit_len("_", 1LL))))) {
                /* pass */
                AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, _pi);
                /* pass */
                if (_tr_str_eqv((fty->name), (_tr_str_lit_len("void", 4LL)))) {
                    /* pass */
                    fty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
                /* pass */
                Sema_declare(self, _pf, SymbolKind_make_SVariable(), box_asttype(fty), false);
            }
            /* pass */
            _pi = (_pi + 1LL);
            _tr_str_release(_pf);
        }
    } else if (_t570.tag == Pattern_PTuple) {
        __auto_type first = _t570.data.PTuple.first;
__auto_type second = _t570.data.PTuple.second;
        /* pass */
        Sema_declare(self, first, SymbolKind_make_SVariable(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL))), false);
        /* pass */
        Sema_declare(self, second, SymbolKind_make_SVariable(), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL))), false);
    } else if (_t570.tag == Pattern_POr) {
        __auto_type pats = _t570.data.POr.patterns;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < pats->len)) {
            /* pass */
            Sema_declare_pattern_binds_typed(self, List_Pattern_get(pats, i), subj_ty);
            /* pass */
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t570;
        /* pass */
    }
}

__attribute__((hot)) AstType* Sema_str_method_ret_ty(Sema* self, TrStr method) {
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("split", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("split_to_vec", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("split_once", 10LL))))) {
        /* pass */
        return AstType_init_generic(_tr_str_lit_len("Vec", 3LL), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL))));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("strip", 5LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("trim", 4LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("trim_left", 9LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("trim_right", 10LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("to_upper", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("to_lower", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("upper", 5LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("lower", 5LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("lstrip", 6LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("rstrip", 6LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("center", 6LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("pad_left", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("pad_right", 9LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("zfill", 5LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("format", 6LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("chars", 5LL)))) {
        /* pass */
        return AstType_init_generic(_tr_str_lit_len("Vec", 3LL), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL))));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("capitalize", 10LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("title", 5LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("repeat", 6LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("replace", 7LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("replace_first", 13LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("strip_prefix", 12LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("strip_suffix", 12LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("remove_char", 11LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("last_index_of", 13LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("char_at", 7LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_int", 9LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_int", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_i64", 6LL))))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_f64", 6LL))))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("float", 5LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("starts_with", 11LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("ends_with", 9LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_bool", 10LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("contains_char", 13LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("eq", 2LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_digit", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_alpha", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_alnum", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_space", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_upper", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_lower", 8LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_bool", 10LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_float", 11LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("float", 5LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("lines", 5LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("Vec", 3LL));
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("words", 5LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit_len("Vec", 3LL));
    }
    /* pass */
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) void Sema_collect_block_refs(Sema* self, HirBlock* b, List_TrStr* out) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        Sema_collect_stmt_refs(self, ((HirStmt*)List_ptr_get(b->stmts, i)), out);
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema_collect_stmt_refs(Sema* self, HirStmt* s, List_TrStr* out) {
    /* pass */
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t571 = (*s);
    if (_t571.tag == HirStmt_SExpr) {
        __auto_type e = _t571.data.SExpr.expr;
        Sema_collect_idents(self, e, out);
    } else if (_t571.tag == HirStmt_SLet) {
        __auto_type v = _t571.data.SLet.val;
        Sema_collect_idents(self, v, out);
    } else if (_t571.tag == HirStmt_SAssign) {
        __auto_type t = _t571.data.SAssign.target;
__auto_type v = _t571.data.SAssign.val;
        /* pass */
        Sema_collect_idents(self, t, out);
        /* pass */
        Sema_collect_idents(self, v, out);
    } else if (_t571.tag == HirStmt_SReturn) {
        __auto_type v = _t571.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema_collect_idents(self, v, out);
        }
    } else if (_t571.tag == HirStmt_SBreak) {
        __auto_type v = _t571.data.SBreak.val;
        /* pass */
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema_collect_idents(self, v, out);
        }
    } else if (_t571.tag == HirStmt_SRaise) {
        __auto_type v = _t571.data.SRaise.val;
        Sema_collect_idents(self, v, out);
    } else if (_t571.tag == HirStmt_SIf) {
        __auto_type c = _t571.data.SIf.cond;
__auto_type tb = _t571.data.SIf.then_b;
__auto_type eb = _t571.data.SIf.else_b;
        /* pass */
        Sema_collect_idents(self, c, out);
        /* pass */
        Sema_collect_block_refs(self, tb, out);
        /* pass */
        Sema_collect_block_refs(self, eb, out);
    } else if (_t571.tag == HirStmt_SWhile) {
        __auto_type c = _t571.data.SWhile.cond;
__auto_type b2 = _t571.data.SWhile.body;
        /* pass */
        Sema_collect_idents(self, c, out);
        /* pass */
        Sema_collect_block_refs(self, b2, out);
    } else if (_t571.tag == HirStmt_SFor) {
        __auto_type it = _t571.data.SFor.iter;
__auto_type b2 = _t571.data.SFor.body;
        /* pass */
        Sema_collect_idents(self, it, out);
        /* pass */
        Sema_collect_block_refs(self, b2, out);
    } else if (_t571.tag == HirStmt_SForUnpack) {
        __auto_type it = _t571.data.SForUnpack.iter;
__auto_type b2 = _t571.data.SForUnpack.body;
        /* pass */
        Sema_collect_idents(self, it, out);
        /* pass */
        Sema_collect_block_refs(self, b2, out);
    } else if (_t571.tag == HirStmt_SMatch) {
        __auto_type subj = _t571.data.SMatch.expr;
__auto_type arms = _t571.data.SMatch.arms;
        /* pass */
        Sema_collect_idents(self, subj, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < arms->len)) {
            /* pass */
            Sema_collect_block_refs(self, ((HirMatchArm*)List_ptr_get(arms, i))->body, out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t571.tag == HirStmt_SUnsafe) {
        __auto_type b2 = _t571.data.SUnsafe.body;
        Sema_collect_block_refs(self, b2, out);
    } else if (1) {
        __auto_type _ = _t571;
        /* pass */
    }
}

__attribute__((hot)) HirExpr* Sema_lower_do_value(Sema* self, Block* do_body) {
    /* pass */
    Sema_enter_scope(self);
    /* pass */
    HirBlock* do_hb = HirBlock_init();
    /* pass */
    long long do_n = do_body->stmts->len;
    /* pass */
    long long do_i = 0LL;
    /* pass */
    HirExpr* do_val = (HirExpr*)(0LL);
    /* pass */
    AstType* do_ty = AstType_init(_tr_str_lit_len("void", 4LL));
    /* pass */
    while ((do_i < do_n)) {
        /* pass */
        Stmt* do_s = ((Stmt*)List_ptr_get(do_body->stmts, do_i));
        /* pass */
        if ((do_i == (do_n - 1LL))) {
            /* pass */
            __auto_type _t572 = (*do_s);
            if (_t572.tag == Stmt_SExpr) {
                __auto_type de = _t572.data.SExpr.expr;
                /* pass */
                do_val = Sema_lower_expr(self, de);
                /* pass */
                do_ty = hir_expr_type(do_val);
            } else if (1) {
                __auto_type _ = _t572;
                /* pass */
                HirStmt* do_hs = Sema_lower_stmt(self, do_s);
                /* pass */
                if ((((unsigned long long)(do_hs)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    Sema_apply_escape_marks(self, do_hs);
                    /* pass */
                    HirBlock_push(do_hb, do_hs);
                }
            }
        } else {
            /* pass */
            HirStmt* do_hs2 = Sema_lower_stmt(self, do_s);
            /* pass */
            if ((((unsigned long long)(do_hs2)) != ((unsigned long long)(0LL)))) {
                /* pass */
                Sema_apply_escape_marks(self, do_hs2);
                /* pass */
                HirBlock_push(do_hb, do_hs2);
            }
        }
        /* pass */
        do_i = (do_i + 1LL);
    }
    /* pass */
    List_TrStr* do_excl = (void*)List_TrStr_new();
    /* pass */
    if (((((unsigned long long)(do_val)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv((do_ty->name), (_tr_str_lit_len("str", 3LL)))))) {
        /* pass */
        Sema_collect_idents(self, do_val, do_excl);
    }
    /* pass */
    Sema_append_drops_from_excl_multi(self, do_hb, (self->scopes->len - 1LL), do_excl);
    /* pass */
    if ((((unsigned long long)(do_val)) != ((unsigned long long)(0LL)))) {
        /* pass */
        HirBlock_push(do_hb, box_hirstmt(HirStmt_ctor_SExpr(do_val)));
    }
    /* pass */
    Sema_exit_scope(self);
    /* pass */
    return box_hirexpr(HirExpr_ctor_EDo(do_hb, do_ty));
}

__attribute__((hot)) AstType* Sema_infer_break_type(Sema* self, HirBlock* hb) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hb->stmts->len)) {
        /* pass */
        AstType* t = Sema_infer_break_type_stmt(self, ((HirStmt*)List_ptr_get(hb->stmts, i)));
        /* pass */
        if ((!_tr_str_eqv((t->name), (_tr_str_lit_len("void", 4LL))))) {
            /* pass */
            return t;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) AstType* Sema_infer_break_type_stmt(Sema* self, HirStmt* s) {
    /* pass */
    __auto_type _t573 = (*s);
    if (_t573.tag == HirStmt_SBreak) {
        __auto_type bv = _t573.data.SBreak.val;
        /* pass */
        if ((((unsigned long long)(bv)) != ((unsigned long long)(0LL)))) {
            /* pass */
            return hir_expr_type(bv);
        }
        /* pass */
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (_t573.tag == HirStmt_SIf) {
        __auto_type then_b = _t573.data.SIf.then_b;
__auto_type else_b = _t573.data.SIf.else_b;
        /* pass */
        AstType* t = Sema_infer_break_type(self, then_b);
        /* pass */
        if ((!_tr_str_eqv((t->name), (_tr_str_lit_len("void", 4LL))))) {
            /* pass */
            return t;
        }
        /* pass */
        return Sema_infer_break_type(self, else_b);
    } else if (_t573.tag == HirStmt_SMatch) {
        __auto_type m_arms = _t573.data.SMatch.arms;
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < m_arms->len)) {
            /* pass */
            AstType* t2 = Sema_infer_break_type(self, ((HirMatchArm*)List_ptr_get(m_arms, ai))->body);
            /* pass */
            if ((!_tr_str_eqv((t2->name), (_tr_str_lit_len("void", 4LL))))) {
                /* pass */
                return t2;
            }
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (_t573.tag == HirStmt_STry) {
        __auto_type tb = _t573.data.STry.try_body;
__auto_type fb = _t573.data.STry.finally_b;
        /* pass */
        AstType* t3 = Sema_infer_break_type(self, tb);
        /* pass */
        if ((!_tr_str_eqv((t3->name), (_tr_str_lit_len("void", 4LL))))) {
            /* pass */
            return t3;
        }
        /* pass */
        return Sema_infer_break_type(self, fb);
    } else if (1) {
        __auto_type _ = _t573;
        return AstType_init(_tr_str_lit_len("void", 4LL));
    }
}

__attribute__((hot)) void Sema_check_no_heap(Sema* self, TrStr what, TrStr fix) {
    /* pass */
    if (self->no_heap) {
        /* pass */
        ({ TrStr _at_t574 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[H-1] ", 6LL)), (what))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" allocates on the heap, which --no-heap forbids (bare-metal zero-heap build).\n      FIX: ", 89LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fix)); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t574); _tr_str_release(_at_t574); });
    }
}

__attribute__((hot)) HirExpr* Sema_lower_expr(Sema* self, Expr* e_ptr) {
    /* pass */
    if ((((unsigned long long)(e_ptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (HirExpr*)(0LL);
    }
    /* pass */
    __auto_type e = (*e_ptr);
    /* pass */
    __auto_type _t575 = e;
    if (_t575.tag == Expr_ELitInt) {
        __auto_type v = _t575.data.ELitInt.val;
        return box_hirexpr(HirExpr_ctor_ELitInt(v, AstType_init(_tr_str_lit_len("int", 3LL))));
    } else if (_t575.tag == Expr_ELitFloat) {
        __auto_type v = _t575.data.ELitFloat.val;
        return box_hirexpr(HirExpr_ctor_ELitFloat(v, AstType_init(_tr_str_lit_len("float", 5LL))));
    } else if (_t575.tag == Expr_ELitStr) {
        __auto_type v = _t575.data.ELitStr.val;
__auto_type blen = _t575.data.ELitStr.blen;
        return box_hirexpr(HirExpr_ctor_ELitStr(v, blen, AstType_init(_tr_str_lit_len("str", 3LL))));
    } else if (_t575.tag == Expr_ERawStr) {
        __auto_type v = _t575.data.ERawStr.val;
__auto_type blen = _t575.data.ERawStr.blen;
        return box_hirexpr(HirExpr_ctor_ERawStr(v, blen, AstType_init(_tr_str_lit_len("str", 3LL))));
    } else if (_t575.tag == Expr_ELitBytes) {
        __auto_type v = _t575.data.ELitBytes.val;
__auto_type blen = _t575.data.ELitBytes.blen;
        return box_hirexpr(HirExpr_ctor_ELitBytes(v, blen, AstType_init(_tr_str_lit_len("Bytes", 5LL))));
    } else if (_t575.tag == Expr_ELitBool) {
        __auto_type v = _t575.data.ELitBool.val;
        return box_hirexpr(HirExpr_ctor_ELitBool(v, AstType_init(_tr_str_lit_len("bool", 4LL))));
    } else if (_t575.tag == Expr_ELitChar) {
        __auto_type v = _t575.data.ELitChar.val;
        return box_hirexpr(HirExpr_ctor_ELitChar(v, AstType_init(_tr_str_lit_len("char", 4LL))));
    } else if (_t575.tag == Expr_ELitNone) {
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init(_tr_str_lit_len("None", 4LL))));
    } else if (_t575.tag == Expr_EIdent) {
        __auto_type name = _t575.data.EIdent.name;
        /* pass */
        Symbol* sym = Sema_resolve(self, name);
        /* pass */
        AstType* ty = (*sym->ty);
        /* pass */
        if (((((_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))) && (!_tr_str_eqv((name), (_tr_str_lit_len("", 0LL))))) && (!self->in_assign_target)) && (!self->in_recv_pos)) && (!Sema_is_known_name(self, name)))) {
            /* pass */
            ({ TrStr _at_t576 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[N-3] name '", 12LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not defined.\n      FIX: check the spelling, declare it with 'mut ", 70LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = ...', or import it before use.", 33LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t576); _tr_str_release(_at_t576); });
        }
        /* pass */
        if ((_tr_dict_contains(self->fn_sigs, _tr_strz(name)) && (sym->kind.tag == SymbolKind_make_SFunction().tag))) {
            /* pass */
            return box_hirexpr(HirExpr_ctor_EIdent(name, ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(name))), false));
        }
        /* pass */
        if ((sym->is_freed && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            ({ TrStr _at_t577 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[M-6] '", 7LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' was freed by 'dealloc()' and can no longer be used.\n      FIX: Remove all uses of '", 85LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' after 'dealloc()', or restructure so the pointer is freed only when no longer needed.", 87LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t577); _tr_str_release(_at_t577); });
        } else if (((((sym->is_moved && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (!sym->is_shared))) {
            /* pass */
            ({ TrStr _at_t578 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving.", 123LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t578); _tr_str_release(_at_t578); });
        } else if (((((sym->is_maybe_moved && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (!sym->is_shared))) {
            /* pass */
            ({ TrStr _at_t579 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[M-5] '", 7LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' may have been moved on some code paths, making this use unsafe.\n      FIX: Ensure '", 85LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not moved before this point on any branch, or restructure so the use is inside the branch where it's still valid.", 118LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t579); _tr_str_release(_at_t579); });
        }
        /* pass */
        if ((((((!sym->is_init) && (!sym->is_maybe_init)) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
            /* pass */
            ({ TrStr _at_t580 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[I-1] Variable '", 16LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is used before being assigned a value.\n      FIX: Assign a value before use, e.g. 'mut ", 89LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = <default>'.", 14LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t580); _tr_str_release(_at_t580); });
        } else if (((((sym->is_maybe_init && (!sym->is_init)) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
            /* pass */
            ({ TrStr _at_t581 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[I-2] '", 7LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not initialized on all code paths before this use.\n      FIX: Initialize '", 79LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' before the if/loop, or ensure every branch assigns a value.", 61LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t581); _tr_str_release(_at_t581); });
        }
        /* pass */
        bool is_move = false;
        /* pass */
        if ((_tr_dict_contains(self->assign_froms, _tr_strz(name)) && (!Sema_is_primitive(self, ty)))) {
            /* pass */
            is_move = true;
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EIdent(name, ty, is_move));
    } else if (_t575.tag == Expr_EBinOp) {
        __auto_type op = _t575.data.EBinOp.op;
__auto_type left = _t575.data.EBinOp.left;
__auto_type right = _t575.data.EBinOp.right;
        /* pass */
        HirExpr* hleft = Sema_lower_expr(self, left);
        /* pass */
        HirExpr* hright = Sema_lower_expr(self, right);
        /* pass */
        if ((self->no_heap && (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))))) {
            /* pass */
            TrStr _nh_l = _tr_str_retain(hir_expr_type(hleft)->name);
            /* pass */
            TrStr _nh_r = _tr_str_retain(hir_expr_type(hright)->name);
            /* pass */
            if ((((_tr_str_eqv((_nh_l), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_nh_l), (_tr_str_lit_len("String", 6LL)))) || _tr_str_eqv((_nh_r), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((_nh_r), (_tr_str_lit_len("String", 6LL))))) {
                /* pass */
                TrStr _nh_what = _tr_str_lit_len("string concatenation (+)", 24LL);
                /* pass */
                if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
                    /* pass */
                    TrStr _strtmp_t582 = _tr_str_lit_len("string repetition (*)", 21LL);
                    _tr_str_release(_nh_what);
                    _nh_what = _strtmp_t582;
                }
                /* pass */
                Sema_check_no_heap(self, _nh_what, _tr_str_lit_len("keep string literals (they live in rodata) or use a StrView / fixed byte buffer; do not build new strings", 105LL));
            }
        }
        /* pass */
        if ((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL))))) {
            /* pass */
            TrStr lref = Sema_type_ref_name(self, left);
            /* pass */
            TrStr rref = Sema_type_ref_name(self, right);
            /* pass */
            if (((!_tr_str_eqv((lref), (_tr_str_lit_len("", 0LL)))) || (!_tr_str_eqv((rref), (_tr_str_lit_len("", 0LL)))))) {
                /* pass */
                TrStr lname = _tr_str_retain(lref);
                /* pass */
                if (_tr_str_eqv((lname), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    TrStr _strtmp_t583 = _tr_str_retain(hir_expr_type(hleft)->name);
                    _tr_str_release(lname);
                    lname = _strtmp_t583;
                }
                /* pass */
                TrStr rname = _tr_str_retain(rref);
                /* pass */
                if (_tr_str_eqv((rname), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    TrStr _strtmp_t584 = _tr_str_retain(hir_expr_type(hright)->name);
                    _tr_str_release(rname);
                    rname = _strtmp_t584;
                }
                /* pass */
                _tr_str_release(lref);
                _tr_str_release(rref);
                return box_hirexpr(HirExpr_ctor_EBinOp(op, box_hirexpr(HirExpr_ctor_ELitStr(lname, _tr_str_lenv((lname)), AstType_init(_tr_str_lit_len("str", 3LL)))), box_hirexpr(HirExpr_ctor_ELitStr(rname, _tr_str_lenv((rname)), AstType_init(_tr_str_lit_len("str", 3LL)))), AstType_init(_tr_str_lit_len("bool", 4LL))));
            }
        }
        /* pass */
        AstType* bin_ty = hir_expr_type(hleft);
        /* pass */
        if (_tr_str_eqv((bin_ty->name), (_tr_str_lit_len("void", 4LL)))) {
            /* pass */
            bin_ty = hir_expr_type(hright);
        }
        /* pass */
        if ((((((((((((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("or", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("&&", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("||", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("in", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("not in", 6LL))))) {
            /* pass */
            bin_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
        } else if ((_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL))) && (((_tr_str_eqv((hir_expr_type(hleft)->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((hir_expr_type(hleft)->name), (_tr_str_lit_len("String", 6LL)))) || _tr_str_eqv((hir_expr_type(hright)->name), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((hir_expr_type(hright)->name), (_tr_str_lit_len("String", 6LL)))))) {
            /* pass */
            bin_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if (((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) && (_binop_is_float_name(hir_expr_type(hleft)->name) || _binop_is_float_name(hir_expr_type(hright)->name)))) {
            /* pass */
            bin_ty = AstType_init(_tr_str_lit_len("float", 5LL));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EBinOp(op, hleft, hright, bin_ty));
    } else if (_t575.tag == Expr_EUnaryOp) {
        __auto_type op = _t575.data.EUnaryOp.op;
__auto_type expr = _t575.data.EUnaryOp.expr;
        /* pass */
        HirExpr* hexpr_inner = Sema_lower_expr(self, expr);
        /* pass */
        AstType* inner_ty = hir_expr_type(hexpr_inner);
        /* pass */
        AstType* un_ty = inner_ty;
        /* pass */
        if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
            /* pass */
            AstType* addr_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
            /* pass */
            List_ptr_append(addr_ty->args, box_asttype(inner_ty));
            /* pass */
            un_ty = addr_ty;
        } else if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
            /* pass */
            if ((_tr_str_eqv((inner_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (inner_ty->args->len > 0LL))) {
                /* pass */
                un_ty = (*((AstType**)List_ptr_get(inner_ty->args, 0LL)));
            }
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EUnaryOp(op, hexpr_inner, un_ty));
    } else if (_t575.tag == Expr_ECall) {
        __auto_type callee = _t575.data.ECall.callee;
__auto_type args = _t575.data.ECall.args;
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t585 = (*callee);
            if (_t585.tag == Expr_EIdent) {
                __auto_type ato_n = _t585.data.EIdent.name;
                /* pass */
                if ((self->no_heap && Sema__is_rc_class(self, ato_n))) {
                    /* pass */
                    ({ TrStr _at_t586 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("constructing heap class '", 25LL)), (ato_n))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_cl); _cres; })); Sema_check_no_heap(self, _at_t586, _tr_str_lit_len("mark the class '@value_type' (stack; all-scalar fields), or place the instance in caller-owned memory via a Pointer[T]", 118LL)); _tr_str_release(_at_t586); });
                }
                /* pass */
                if (_tr_str_eqv((ato_n), (_tr_str_lit_len("await_timeout", 13LL)))) {
                    /* pass */
                    if ((!self->in_async_fn)) {
                        /* pass */
                        Sema_error(self, _tr_str_lit_len("[C-4] 'await_timeout' used outside an async function.", 53LL));
                    }
                    /* pass */
                    if ((args->len < 2LL)) {
                        /* pass */
                        Sema_error(self, _tr_str_lit_len("await_timeout requires 2 arguments: await_timeout(expr, timeout_ms)", 67LL));
                        /* pass */
                        return box_hirexpr(HirExpr_ctor_ELitInt(0LL, AstType_init(_tr_str_lit_len("int", 3LL))));
                    }
                    /* pass */
                    HirExpr* ato_inner = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
                    /* pass */
                    HirExpr* ato_ms = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 1LL)));
                    /* pass */
                    AstType* ato_ty = hir_expr_type(ato_inner);
                    /* pass */
                    return box_hirexpr(HirExpr_ctor_EAwaitTimeout(ato_inner, ato_ms, ato_ty));
                }
                /* pass */
                if (_tr_str_eqv((ato_n), (_tr_str_lit_len("instanceOf", 10LL)))) {
                    /* pass */
                    if ((args->len < 2LL)) {
                        /* pass */
                        Sema_error(self, _tr_str_lit_len("instanceOf requires 2 arguments: instanceOf(obj, T)", 51LL));
                        /* pass */
                        return box_hirexpr(HirExpr_ctor_ELitBool(false, AstType_init(_tr_str_lit_len("bool", 4LL))));
                    }
                    /* pass */
                    HirExpr* io_obj = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
                    /* pass */
                    TrStr io_obj_ty_n = _tr_str_retain(hir_expr_type(io_obj)->name);
                    /* pass */
                    TrStr io_target_n = _tr_str_lit_len("", 0LL);
                    /* pass */
                    __auto_type _t587 = (*((Expr*)List_ptr_get(args, 1LL)));
                    if (_t587.tag == Expr_EIdent) {
                        __auto_type io_tn = _t587.data.EIdent.name;
                        TrStr _strtmp_t588 = _tr_str_retain(io_tn);
                        _tr_str_release(io_target_n);
                        io_target_n = _strtmp_t588;
                    } else if (_t587.tag == Expr_EIndex) {
                        __auto_type io_base = _t587.data.EIndex.obj;
                        /* pass */
                        __auto_type _t589 = (*io_base);
                        if (_t589.tag == Expr_EIdent) {
                            __auto_type io_tn2 = _t589.data.EIdent.name;
                            TrStr _strtmp_t590 = _tr_str_retain(io_tn2);
                            _tr_str_release(io_target_n);
                            io_target_n = _strtmp_t590;
                        } else if (1) {
                            __auto_type _ = _t589;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t587;
                        /* pass */
                    }
                    /* pass */
                    return box_hirexpr(HirExpr_ctor_ELitBool(_tr_str_eqv((io_obj_ty_n), (io_target_n)), AstType_init(_tr_str_lit_len("bool", 4LL))));
                }
                /* pass */
                if (_tr_str_eqv((ato_n), (_tr_str_lit_len("inspect", 7LL)))) {
                    /* pass */
                    if ((args->len < 1LL)) {
                        /* pass */
                        Sema_error(self, _tr_str_lit_len("inspect requires 1 argument: inspect(T)", 39LL));
                        /* pass */
                        return box_hirexpr(HirExpr_ctor_ELitStr(_tr_str_lit_len("", 0LL), 0LL, AstType_init(_tr_str_lit_len("str", 3LL))));
                    }
                    /* pass */
                    TrStr isp_target_n = _tr_str_lit_len("", 0LL);
                    /* pass */
                    __auto_type _t591 = (*((Expr*)List_ptr_get(args, 0LL)));
                    if (_t591.tag == Expr_EIdent) {
                        __auto_type isp_tn = _t591.data.EIdent.name;
                        TrStr _strtmp_t592 = _tr_str_retain(isp_tn);
                        _tr_str_release(isp_target_n);
                        isp_target_n = _strtmp_t592;
                    } else if (_t591.tag == Expr_EIndex) {
                        __auto_type isp_base = _t591.data.EIndex.obj;
                        /* pass */
                        __auto_type _t593 = (*isp_base);
                        if (_t593.tag == Expr_EIdent) {
                            __auto_type isp_tn2 = _t593.data.EIdent.name;
                            TrStr _strtmp_t594 = _tr_str_retain(isp_tn2);
                            _tr_str_release(isp_target_n);
                            isp_target_n = _strtmp_t594;
                        } else if (1) {
                            __auto_type _ = _t593;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t591;
                        /* pass */
                    }
                    /* pass */
                    if (_tr_str_eqv((isp_target_n), (_tr_str_lit_len("", 0LL)))) {
                        /* pass */
                        HirExpr* isp_obj = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
                        /* pass */
                        TrStr _strtmp_t595 = _tr_str_retain(hir_expr_type(isp_obj)->name);
                        _tr_str_release(isp_target_n);
                        isp_target_n = _strtmp_t595;
                    }
                    /* pass */
                    TrStr isp_str = Sema_build_inspect_str(self, isp_target_n);
                    /* pass */
                    _tr_str_release(isp_target_n);
                    return box_hirexpr(HirExpr_ctor_ELitStr(isp_str, _tr_str_lenv((isp_str)), AstType_init(_tr_str_lit_len("str", 3LL))));
                }
            } else if (1) {
                __auto_type _ = _t585;
                /* pass */
            }
        }
        /* pass */
        List_TrStr* p23_borrow_names = (void*)List_TrStr_new();
        /* pass */
        TrMap* p23_seen = _tr_dict_new(4LL);
        /* pass */
        long long p23_k = 0LL;
        /* pass */
        while ((p23_k < args->len)) {
            /* pass */
            Expr* p23_arg = ((Expr*)List_ptr_get(args, p23_k));
            /* pass */
            TrStr p23_nm = _tr_str_lit_len("", 0LL);
            /* pass */
            if ((((unsigned long long)(p23_arg)) != ((unsigned long long)(0LL)))) {
                /* pass */
                __auto_type _t596 = (*p23_arg);
                if (_t596.tag == Expr_EIdent) {
                    __auto_type p23_n = _t596.data.EIdent.name;
                    TrStr _strtmp_t597 = _tr_str_retain(p23_n);
                    _tr_str_release(p23_nm);
                    p23_nm = _strtmp_t597;
                } else if (1) {
                    __auto_type _ = _t596;
                    /* pass */
                }
            }
            /* pass */
            if ((!_tr_str_eqv((p23_nm), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                Symbol* p23_sym = Sema_resolve(self, p23_nm);
                /* pass */
                if (((!Sema_is_primitive(self, (*p23_sym->ty))) && (!_tr_str_eqv((p23_sym->name), (_tr_str_lit_len("", 0LL)))))) {
                    /* pass */
                    Sema_mark_borrow(self, p23_nm);
                    /* pass */
                    List_TrStr_append(p23_borrow_names, p23_nm);
                    /* pass */
                    if (_tr_dict_contains(p23_seen, _tr_strz(p23_nm))) {
                        /* pass */
                        ({ TrStr _at_t598 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[M-3] '", 7LL)), (p23_nm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' appears twice in the same call, creating aliased mutable access.\n      FIX: Clone one of the arguments: ", 106LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (p23_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".clone()", 8LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t598); _tr_str_release(_at_t598); });
                    }
                    /* pass */
                    _tr_dict_set(p23_seen, _tr_strz(p23_nm), true);
                }
            }
            /* pass */
            p23_k = (p23_k + 1LL);
            _tr_str_release(p23_nm);
        }
        /* pass */
        List_ptr* hl = (void*)List_ptr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < args->len)) {
            /* pass */
            List_ptr_append(hl, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k))));
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        long long p23_ui = 0LL;
        /* pass */
        while ((p23_ui < p23_borrow_names->len)) {
            /* pass */
            ({ TrStr _at_t599 = (List_TrStr_get(p23_borrow_names, p23_ui)); Sema_unmark_borrow(self, _at_t599); _tr_str_release(_at_t599); });
            /* pass */
            p23_ui = (p23_ui + 1LL);
        }
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t600 = (*callee);
            if (_t600.tag == Expr_EIdent) {
                __auto_type vfn_n = _t600.data.EIdent.name;
                /* pass */
                if (_tr_dict_contains(self->variadic_fns, _tr_strz(vfn_n))) {
                    /* pass */
                    long long vfixed = _tr_str_to_int(_tr_strz(_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->variadic_fns, _tr_strz(vfn_n))))));
                    /* pass */
                    AstType* velem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                    /* pass */
                    if (_tr_dict_contains(self->variadic_elem_ty, _tr_strz(vfn_n))) {
                        /* pass */
                        velem_ty = (*((AstType**)(uintptr_t)_tr_dict_get(self->variadic_elem_ty, _tr_strz(vfn_n))));
                    }
                    /* pass */
                    if ((hl->len >= vfixed)) {
                        /* pass */
                        List_ptr* vargs = (void*)List_ptr_new();
                        /* pass */
                        long long vi = vfixed;
                        /* pass */
                        while ((vi < hl->len)) {
                            /* pass */
                            List_ptr_append(vargs, ((HirExpr*)List_ptr_get(hl, vi)));
                            /* pass */
                            vi = (vi + 1LL);
                        }
                        /* pass */
                        AstType* vlist_ty = AstType_init(_tr_str_lit_len("List", 4LL));
                        /* pass */
                        List_ptr_append(vlist_ty->args, box_asttype(velem_ty));
                        /* pass */
                        List_ptr* vnew_hl = (void*)List_ptr_new();
                        /* pass */
                        long long vk = 0LL;
                        /* pass */
                        while ((vk < vfixed)) {
                            /* pass */
                            List_ptr_append(vnew_hl, ((HirExpr*)List_ptr_get(hl, vk)));
                            /* pass */
                            vk = (vk + 1LL);
                        }
                        /* pass */
                        List_ptr_append(vnew_hl, box_hirexpr(HirExpr_ctor_EList(vargs, vlist_ty)));
                        /* pass */
                        hl = vnew_hl;
                    }
                }
            } else if (1) {
                __auto_type _ = _t600;
                /* pass */
            }
        }
        /* pass */
        HirExpr* hcallee = Sema_lower_expr(self, callee);
        /* pass */
        AstType* ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
            /* pass */
            List_TrStr_free(p23_borrow_names);
            Dict_free(p23_seen);
            return box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
        }
        /* pass */
        __auto_type _t601 = (*callee);
        if (_t601.tag == Expr_EIdent) {
            __auto_type n = _t601.data.EIdent.name;
            /* pass */
            Sema_check_call_bounds(self, n, hl);
            /* pass */
            if (_tr_str_eqv((n), (_tr_str_lit_len("main", 4LL)))) {
                /* pass */
                Sema_error(self, _tr_str_lit_len("[E-1] Explicit call to 'main()' is forbidden. The compiler automatically invokes main() as the program entry point. Remove the 'main()' call from your source.", 158LL));
                /* pass */
                List_TrStr_free(p23_borrow_names);
                Dict_free(p23_seen);
                return box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
            }
            /* pass */
            if ((((((((_tr_dict_contains(self->classes, _tr_strz(n)) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))))) {
                /* pass */
                ret_ty = AstType_init(n);
            } else if (_tr_dict_contains(self->enums, _tr_strz(n))) {
                /* pass */
                ret_ty = AstType_init(n);
            } else if ((_tr_str_eqv((Sema_resolve(self, n)->name), (_tr_str_lit_len("", 0LL))) && Sema__is_type_param_in_scope(self, n))) {
                /* pass */
                ret_ty = AstType_init(n);
            } else if ((((_tr_str_eqv((n), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("max", 3LL)))) && _hl_has_float(hl))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("float", 5LL));
            } else if (_tr_str_eqv((n), (_tr_str_lit_len("c_callback", 10LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit_len("void", 4LL))));
            } else if ((_tr_str_eqv((n), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("dealloc", 7LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
                /* pass */
                if ((!self->in_unsafe)) {
                    /* pass */
                    ({ TrStr _at_t602 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[U-1] '", 7LL)), (n))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' used outside an 'unsafe:' block.\n      FIX: Wrap raw memory operations in 'unsafe:' to signal manual memory management, e.g.\n          unsafe:\n              p = ", 163LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("[T](n)", 6LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t602); _tr_str_release(_at_t602); });
                }
            } else {
                /* pass */
                Symbol* _fsym = Sema_resolve(self, n);
                /* pass */
                ret_ty = (*_fsym->ty);
                /* pass */
                if ((_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("def", 3LL))) && (ret_ty->args->len > 0LL))) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(ret_ty->args, (ret_ty->args->len - 1LL))));
                }
                /* pass */
                if (_tr_dict_contains(self->fn_defs, _tr_strz(n))) {
                    /* pass */
                    FunctionDef* _gfd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(n)));
                    /* pass */
                    if (((_gfd->generics->len > 0LL) && List_TrStr_contains(_gfd->generics, ret_ty->name))) {
                        /* pass */
                        long long _gsk = 0LL;
                        /* pass */
                        if (((_gfd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_gfd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
                            /* pass */
                            _gsk = 1LL;
                        }
                        /* pass */
                        long long _gpi = 0LL;
                        /* pass */
                        while ((_gpi < _gfd->params->len)) {
                            /* pass */
                            AstType** _gpty = ((Param*)List_ptr_get(_gfd->params, _gpi))->ty;
                            /* pass */
                            if (((((unsigned long long)(_gpty)) != ((unsigned long long)(0LL))) && _tr_str_eqv(((*_gpty)->name), (ret_ty->name)))) {
                                /* pass */
                                long long _gaidx = (_gpi - _gsk);
                                /* pass */
                                if (((_gaidx >= 0LL) && (_gaidx < hl->len))) {
                                    /* pass */
                                    ret_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _gaidx)));
                                    /* pass */
                                    break;
                                }
                            }
                            /* pass */
                            _gpi = (_gpi + 1LL);
                        }
                    }
                }
            }
        } else if (_t601.tag == Expr_EIndex) {
            __auto_type base = _t601.data.EIndex.obj;
__auto_type idx = _t601.data.EIndex._tr_v_index;
            /* pass */
            __auto_type _t603 = (*base);
            if (_t603.tag == Expr_EIdent) {
                __auto_type gn = _t603.data.EIdent.name;
                /* pass */
                if ((_tr_str_eqv((gn), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((gn), (_tr_str_lit_len("dealloc", 7LL))))) {
                    /* pass */
                    __auto_type _t604 = (*idx);
                    if (_t604.tag == Expr_EIdent) {
                        __auto_type tn = _t604.data.EIdent.name;
                        /* pass */
                        ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
                        /* pass */
                        List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn)));
                    } else if (1) {
                        __auto_type _ = _t604;
                        /* pass */
                        ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
                    }
                    /* pass */
                    if ((!self->in_unsafe)) {
                        /* pass */
                        ({ TrStr _at_t605 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[U-1] 'alloc'/'dealloc' used outside an 'unsafe:' block.\n      FIX: Wrap raw memory operations in 'unsafe:' to signal manual memory management, e.g.\n          unsafe:\n              p = ", 185LL)), (gn))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("[T](n)", 6LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t605); _tr_str_release(_at_t605); });
                    }
                } else if (((((_tr_str_eqv((gn), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((gn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((gn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((gn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((gn), (_tr_str_lit_len("Dict", 4LL))))) {
                    /* pass */
                    __auto_type _t606 = (*idx);
                    if (_t606.tag == Expr_EIdent) {
                        __auto_type tn = _t606.data.EIdent.name;
                        /* pass */
                        ret_ty = AstType_init(gn);
                        /* pass */
                        List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn)));
                    } else if (1) {
                        __auto_type _ = _t606;
                        /* pass */
                        ret_ty = AstType_init(gn);
                    }
                } else if (_tr_dict_contains(self->classes, _tr_strz(gn))) {
                    /* pass */
                    ret_ty = AstType_init(gn);
                    /* pass */
                    __auto_type _t607 = (*idx);
                    if (_t607.tag == Expr_EIdent) {
                        __auto_type targ_ca = _t607.data.EIdent.name;
                        List_ptr_append(ret_ty->args, box_asttype(AstType_init(targ_ca)));
                    } else if (1) {
                        __auto_type _ = _t607;
                        /* pass */
                    }
                } else if (_tr_dict_contains(self->enums, _tr_strz(gn))) {
                    /* pass */
                    ret_ty = AstType_init(gn);
                } else {
                    /* pass */
                    __auto_type _t608 = (*idx);
                    if (_t608.tag == Expr_EIdent) {
                        __auto_type farg_c = _t608.data.EIdent.name;
                        /* pass */
                        AstType* fret = AstType_init(farg_c);
                        /* pass */
                        if (_tr_dict_contains(self->fn_defs, _tr_strz(gn))) {
                            /* pass */
                            FunctionDef* _sf_fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(gn)));
                            /* pass */
                            if ((((unsigned long long)(_sf_fd->ret_ty)) != ((unsigned long long)(0LL)))) {
                                /* pass */
                                fret = (*_sf_fd->ret_ty);
                            }
                            /* pass */
                            if (((_sf_fd->generics->len > 0LL) && _tr_str_eqv((List_TrStr_get(_sf_fd->generics, 0LL)), (fret->name)))) {
                                /* pass */
                                fret = AstType_init(farg_c);
                            }
                            /* pass */
                            if ((((unsigned long long)(_sf_fd->throws_ty)) != ((unsigned long long)(0LL)))) {
                                /* pass */
                                if ((!_tr_str_eqv(((*_sf_fd->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
                                    /* pass */
                                    AstType* _sf_result = AstType_init(_tr_str_lit_len("Result", 6LL));
                                    /* pass */
                                    List_ptr_append(_sf_result->args, box_asttype(fret));
                                    /* pass */
                                    List_ptr_append(_sf_result->args, _sf_fd->throws_ty);
                                    /* pass */
                                    fret = _sf_result;
                                }
                            }
                        }
                        /* pass */
                        ret_ty = fret;
                        /* pass */
                        hcallee = ({ TrStr _at_t609 = (({ TrStr _cl = (_tr_strx_concatv((gn), (_tr_str_lit_len("__MONO_", 7LL)))); TrStr _cres = _tr_strx_concatv(_cl, (farg_c)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t609, fret, false))); _tr_str_release(_at_t609); _wr; });
                    } else if (_t608.tag == Expr_ETuple) {
                        __auto_type ftargs = _t608.data.ETuple.items;
                        /* pass */
                        List_TrStr* _mf_names = (void*)List_TrStr_new();
                        /* pass */
                        long long _mfi = 0LL;
                        /* pass */
                        while ((_mfi < ftargs->len)) {
                            /* pass */
                            TrStr _mf_nm = _tr_str_lit_len("", 0LL);
                            /* pass */
                            __auto_type _t610 = (*((Expr*)List_ptr_get(ftargs, _mfi)));
                            if (_t610.tag == Expr_EIdent) {
                                __auto_type _mfn = _t610.data.EIdent.name;
                                TrStr _strtmp_t611 = _tr_str_retain(_mfn);
                                _tr_str_release(_mf_nm);
                                _mf_nm = _strtmp_t611;
                            } else if (1) {
                                __auto_type _ = _t610;
                                /* pass */
                            }
                            /* pass */
                            List_TrStr_append(_mf_names, _mf_nm);
                            /* pass */
                            _mfi = (_mfi + 1LL);
                            _tr_str_release(_mf_nm);
                        }
                        /* pass */
                        TrStr _mf_mangled = _tr_str_retain(gn);
                        /* pass */
                        long long _mf_ji = 0LL;
                        /* pass */
                        while ((_mf_ji < _mf_names->len)) {
                            /* pass */
                            if ((_mf_ji == 0LL)) {
                                /* pass */
                                TrStr _strtmp_t612 = ({ TrStr _cl = (_tr_strx_concatv((_mf_mangled), (_tr_str_lit_len("__MONO_", 7LL)))); TrStr _cr = (List_TrStr_get(_mf_names, _mf_ji)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                                _tr_str_release(_mf_mangled);
                                _mf_mangled = _strtmp_t612;
                            } else {
                                /* pass */
                                TrStr _strtmp_t613 = ({ TrStr _cl = (_tr_strx_concatv((_mf_mangled), (_tr_str_lit_len("_", 1LL)))); TrStr _cr = (List_TrStr_get(_mf_names, _mf_ji)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                                _tr_str_release(_mf_mangled);
                                _mf_mangled = _strtmp_t613;
                            }
                            /* pass */
                            _mf_ji = (_mf_ji + 1LL);
                        }
                        /* pass */
                        AstType* fret2 = AstType_init(_tr_str_lit_len("void", 4LL));
                        /* pass */
                        if (_tr_dict_contains(self->fn_defs, _tr_strz(gn))) {
                            /* pass */
                            FunctionDef* _mf_fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(gn)));
                            /* pass */
                            if ((((unsigned long long)(_mf_fd->ret_ty)) != ((unsigned long long)(0LL)))) {
                                /* pass */
                                fret2 = (*_mf_fd->ret_ty);
                            }
                            /* pass */
                            long long _mf_gi = 0LL;
                            /* pass */
                            while ((_mf_gi < _mf_fd->generics->len)) {
                                /* pass */
                                if ((_tr_str_eqv((List_TrStr_get(_mf_fd->generics, _mf_gi)), (fret2->name)) && (_mf_gi < _mf_names->len))) {
                                    /* pass */
                                    fret2 = ({ TrStr _at_t614 = (List_TrStr_get(_mf_names, _mf_gi)); __auto_type _wr = (AstType_init(_at_t614)); _tr_str_release(_at_t614); _wr; });
                                }
                                /* pass */
                                _mf_gi = (_mf_gi + 1LL);
                            }
                            /* pass */
                            if ((((unsigned long long)(_mf_fd->throws_ty)) != ((unsigned long long)(0LL)))) {
                                /* pass */
                                if ((!_tr_str_eqv(((*_mf_fd->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
                                    /* pass */
                                    AstType* _mf_result = AstType_init(_tr_str_lit_len("Result", 6LL));
                                    /* pass */
                                    List_ptr_append(_mf_result->args, box_asttype(fret2));
                                    /* pass */
                                    List_ptr_append(_mf_result->args, _mf_fd->throws_ty);
                                    /* pass */
                                    fret2 = _mf_result;
                                }
                            }
                        }
                        /* pass */
                        ret_ty = fret2;
                        /* pass */
                        hcallee = box_hirexpr(HirExpr_ctor_EIdent(_mf_mangled, fret2, false));
                        List_TrStr_free(_mf_names);
                        _tr_str_release(_mf_mangled);
                    } else if (1) {
                        __auto_type _ = _t608;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t603;
                /* pass */
            }
        } else if (_t601.tag == Expr_EPropAccess) {
            __auto_type obj = _t601.data.EPropAccess.obj;
__auto_type variant = _t601.data.EPropAccess.prop;
            /* pass */
            __auto_type _t615 = (*obj);
            if (_t615.tag == Expr_EIdent) {
                __auto_type type_name = _t615.data.EIdent.name;
                /* pass */
                if (_tr_dict_contains(self->enums, _tr_strz(type_name))) {
                    /* pass */
                    ret_ty = AstType_init(type_name);
                } else if (((((_tr_str_eqv((type_name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((type_name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("ThreadLocal", 11LL)))) && ((_tr_str_eqv((variant), (_tr_str_lit_len("spawn", 5LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL)))))) {
                    /* pass */
                    ret_ty = AstType_init(type_name);
                } else if ((_tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL))))) {
                    /* pass */
                    if (((((_tr_str_eqv((type_name), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((type_name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Dict", 4LL))))) {
                        /* pass */
                        ret_ty = AstType_init(type_name);
                    } else if (_tr_dict_contains(self->classes, _tr_strz(type_name))) {
                        /* pass */
                        ret_ty = AstType_init(type_name);
                    }
                }
            } else if (_t615.tag == Expr_EIndex) {
                __auto_type base2 = _t615.data.EIndex.obj;
__auto_type idx2 = _t615.data.EIndex._tr_v_index;
                /* pass */
                if ((_tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL))))) {
                    /* pass */
                    __auto_type _t616 = (*base2);
                    if (_t616.tag == Expr_EIdent) {
                        __auto_type gn2 = _t616.data.EIdent.name;
                        /* pass */
                        if (((((_tr_str_eqv((gn2), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((gn2), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((gn2), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((gn2), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((gn2), (_tr_str_lit_len("Dict", 4LL))))) {
                            /* pass */
                            __auto_type _t617 = (*idx2);
                            if (_t617.tag == Expr_EIdent) {
                                __auto_type tn2 = _t617.data.EIdent.name;
                                /* pass */
                                ret_ty = AstType_init(gn2);
                                /* pass */
                                List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn2)));
                            } else if (1) {
                                __auto_type _ = _t617;
                                /* pass */
                                ret_ty = AstType_init(gn2);
                            }
                        } else if (_tr_dict_contains(self->classes, _tr_strz(gn2))) {
                            /* pass */
                            ret_ty = AstType_init(gn2);
                            /* pass */
                            __auto_type _t618 = (*idx2);
                            if (_t618.tag == Expr_EIdent) {
                                __auto_type targ_cb = _t618.data.EIdent.name;
                                List_ptr_append(ret_ty->args, box_asttype(AstType_init(targ_cb)));
                            } else if (1) {
                                __auto_type _ = _t618;
                                /* pass */
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t616;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t615;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t601;
            /* pass */
        }
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t619 = (*callee);
            if (_t619.tag == Expr_EIdent) {
                __auto_type aa_nm = _t619.data.EIdent.name;
                /* pass */
                if (_tr_str_eqv((aa_nm), (_tr_str_lit_len("await_all", 9LL)))) {
                    /* pass */
                    long long aa_i = 0LL;
                    /* pass */
                    while ((aa_i < hl->len)) {
                        /* pass */
                        Sema_check_spawn_sendable(self, ((HirExpr*)List_ptr_get(hl, aa_i)));
                        /* pass */
                        aa_i = (aa_i + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t619;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t620 = (*callee);
            if (_t620.tag == Expr_EIdent) {
                __auto_type df_nm = _t620.data.EIdent.name;
                /* pass */
                if ((_tr_str_eqv((df_nm), (_tr_str_lit_len("dealloc", 7LL))) && (args->len > 0LL))) {
                    /* pass */
                    Expr* df_arg0 = ((Expr*)List_ptr_get(args, 0LL));
                    /* pass */
                    if ((((unsigned long long)(df_arg0)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        __auto_type _t621 = (*df_arg0);
                        if (_t621.tag == Expr_EIdent) {
                            __auto_type df_ptr = _t621.data.EIdent.name;
                            /* pass */
                            Symbol* df_sym = Sema_resolve(self, df_ptr);
                            /* pass */
                            if ((!_tr_str_eqv((df_sym->name), (_tr_str_lit_len("", 0LL))))) {
                                /* pass */
                                Sema_mark_freed(self, df_ptr);
                            }
                        } else if (1) {
                            __auto_type _ = _t621;
                            /* pass */
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t620;
                /* pass */
            }
        }
        /* pass */
        List_TrStr_free(p23_borrow_names);
        Dict_free(p23_seen);
        return box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
    } else if (_t575.tag == Expr_EMethodCall) {
        __auto_type obj = _t575.data.EMethodCall.obj;
__auto_type method = _t575.data.EMethodCall.method;
__auto_type args = _t575.data.EMethodCall.args;
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("__index__", 9LL))) && (args->len > 0LL))) {
            /* pass */
            return Sema_lower_expr(self, box_expr(Expr_ctor_EIndex(obj, ((Expr*)List_ptr_get(args, 0LL)))));
        }
        /* pass */
        if (((self->no_heap && ((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("with_capacity", 13LL))))) && (((unsigned long long)(obj)) != ((unsigned long long)(0LL))))) {
            /* pass */
            TrStr _nh_rn = _tr_str_lit_len("", 0LL);
            /* pass */
            __auto_type _t622 = (*obj);
            if (_t622.tag == Expr_EIdent) {
                __auto_type _nhn = _t622.data.EIdent.name;
                TrStr _strtmp_t623 = _tr_str_retain(_nhn);
                _tr_str_release(_nh_rn);
                _nh_rn = _strtmp_t623;
            } else if (_t622.tag == Expr_EIndex) {
                __auto_type _nhbase = _t622.data.EIndex.obj;
                /* pass */
                if ((((unsigned long long)(_nhbase)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    __auto_type _t624 = (*_nhbase);
                    if (_t624.tag == Expr_EIdent) {
                        __auto_type _nhn2 = _t624.data.EIdent.name;
                        TrStr _strtmp_t625 = _tr_str_retain(_nhn2);
                        _tr_str_release(_nh_rn);
                        _nh_rn = _strtmp_t625;
                    } else if (1) {
                        __auto_type _ = _t624;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t622;
                /* pass */
            }
            /* pass */
            if (((((_tr_str_eqv((_nh_rn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Set", 3LL))))) {
                /* pass */
                ({ TrStr _at_t626 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_nh_rn), (_tr_str_lit_len(".", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (method)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("() (a growable heap collection)", 31LL))); _tr_str_release(_cl); _cres; })); Sema_check_no_heap(self, _at_t626, _tr_str_lit_len("use a fixed-size array [T; N] (stack), or parallel fixed arrays for a map", 73LL)); _tr_str_release(_at_t626); });
            }
        }
        /* pass */
        List_ptr* hl = (void*)List_ptr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < args->len)) {
            /* pass */
            List_ptr_append(hl, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k))));
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        bool _saved_recv_mc = self->in_recv_pos;
        /* pass */
        self->in_recv_pos = true;
        /* pass */
        HirExpr* hobj = Sema_lower_expr(self, obj);
        /* pass */
        self->in_recv_pos = _saved_recv_mc;
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
            /* pass */
            __auto_type _t627 = (*obj);
            if (_t627.tag == Expr_EIdent) {
                __auto_type free_nm = _t627.data.EIdent.name;
                /* pass */
                Symbol* free_sym = Sema_resolve(self, free_nm);
                /* pass */
                if ((!_tr_str_eqv((free_sym->name), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    Sema_mark_freed(self, free_nm);
                }
            } else if (1) {
                __auto_type _ = _t627;
                /* pass */
            }
        }
        /* pass */
        AstType* hobj_ty = hir_expr_type(hobj);
        /* pass */
        if (_tr_dict_contains(self->type_aliases, _tr_strz(hobj_ty->name))) {
            /* pass */
            TrStr _alias_base = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->type_aliases, _tr_strz(hobj_ty->name))));
            /* pass */
            AstType* _alias_ty = AstType_init(_alias_base);
            /* pass */
            if (_tr_dict_contains(self->type_alias_elem, _tr_strz(hobj_ty->name))) {
                /* pass */
                TrStr _elem_name = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->type_alias_elem, _tr_strz(hobj_ty->name))));
                /* pass */
                _alias_ty = AstType_init_generic(_alias_base, box_asttype(AstType_init(_elem_name)));
            }
            /* pass */
            hobj_ty = _alias_ty;
        }
        /* pass */
        TrStr _recv_name = _tr_str_lit_len("", 0LL);
        /* pass */
        bool _recv_is_shared = false;
        /* pass */
        __auto_type _t628 = (*obj);
        if (_t628.tag == Expr_EIdent) {
            __auto_type _rn = _t628.data.EIdent.name;
            /* pass */
            TrStr _strtmp_t629 = _tr_str_retain(_rn);
            _tr_str_release(_recv_name);
            _recv_name = _strtmp_t629;
            /* pass */
            Symbol* _rsym = Sema_resolve(self, _rn);
            /* pass */
            if (((!_tr_str_eqv((_rsym->name), (_tr_str_lit_len("", 0LL)))) && _rsym->is_shared)) {
                /* pass */
                _recv_is_shared = true;
            }
        } else if (1) {
            __auto_type _ = _t628;
            /* pass */
        }
        /* pass */
        if ((((!_tr_str_eqv((_recv_name), (_tr_str_lit_len("", 0LL)))) && (!Sema_is_local_var(self, _recv_name))) && (!Sema__is_known_type_name(self, _recv_name)))) {
            /* pass */
            if (_tr_str_eqv((Sema_resolve(self, _recv_name)->name), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                ({ TrStr _at_t630 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] '", 7LL)), (_recv_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not a defined type or value, so '", 38LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_recv_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (method)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(...)' cannot be resolved.\n      FIX: check the spelling, import the type, or define it — a call on an unknown name would otherwise emit invalid C.", 149LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t630); _tr_str_release(_at_t630); });
            }
        }
        /* pass */
        if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
            /* pass */
            ClassDef* _cf_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
            /* pass */
            long long _cf_i = 0LL;
            /* pass */
            AstType* _cf_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            /* pass */
            bool _cf_found = false;
            /* pass */
            while ((_cf_i < _cf_cls->fields->len)) {
                /* pass */
                FieldDef* _cf_fld = ((FieldDef*)List_ptr_get(_cf_cls->fields, _cf_i));
                /* pass */
                if ((_tr_str_eqv((_cf_fld->name), (method)) && (((unsigned long long)(_cf_fld->ty)) != ((unsigned long long)(0LL))))) {
                    /* pass */
                    _cf_ty = (*_cf_fld->ty);
                    /* pass */
                    _cf_found = true;
                }
                /* pass */
                _cf_i = (_cf_i + 1LL);
            }
            /* pass */
            if ((_cf_found && _tr_str_eqv((_cf_ty->name), (_tr_str_lit_len("def", 3LL))))) {
                /* pass */
                AstType* _cf_ret = AstType_init(_tr_str_lit_len("void", 4LL));
                /* pass */
                if ((_cf_ty->args->len > 0LL)) {
                    /* pass */
                    _cf_ret = (*((AstType**)List_ptr_get(_cf_ty->args, (_cf_ty->args->len - 1LL))));
                }
                /* pass */
                if (((_cf_cls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
                    /* pass */
                    _cf_ret = Sema__subst_ret_generics(self, _cf_ret, _cf_cls->generics, hobj_ty->args);
                }
                /* pass */
                HirExpr* _cf_prop = box_hirexpr(HirExpr_ctor_EPropAccess(hobj, method, _cf_ty));
                /* pass */
                _tr_str_release(_recv_name);
                return box_hirexpr(HirExpr_ctor_ECall(_cf_prop, hl, _cf_ret));
            }
        }
        /* pass */
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL))))) {
            /* pass */
            TrStr pc_obj_nm = _tr_str_lit_len("", 0LL);
            /* pass */
            __auto_type _t631 = (*obj);
            if (_t631.tag == Expr_EIdent) {
                __auto_type pc_src = _t631.data.EIdent.name;
                TrStr _strtmp_t632 = _tr_str_retain(pc_src);
                _tr_str_release(pc_obj_nm);
                pc_obj_nm = _strtmp_t632;
            } else if (1) {
                __auto_type _ = _t631;
                /* pass */
            }
            /* pass */
            if (((!_tr_str_eqv((pc_obj_nm), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->container_borrows, _tr_strz(pc_obj_nm)))) {
                /* pass */
                TrStr pc_borrow_var = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->container_borrows, _tr_strz(pc_obj_nm))));
                /* pass */
                ({ TrStr _at_t633 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[M-4] Cannot mutate '", 21LL)), (pc_obj_nm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' while '", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pc_borrow_var)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' holds a reference into it.\n      FIX: Finish using '", 54LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pc_borrow_var)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' before modifying '", 20LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pc_obj_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("', or copy it first: 'mut copy = ", 33LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pc_borrow_var)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t633); _tr_str_release(_at_t633); });
            }
        }
        /* pass */
        TrStr _bm_obj_nm = _tr_str_lit_len("", 0LL);
        /* pass */
        __auto_type _t634 = (*obj);
        if (_t634.tag == Expr_EIdent) {
            __auto_type _bm_n = _t634.data.EIdent.name;
            TrStr _strtmp_t635 = _tr_str_retain(_bm_n);
            _tr_str_release(_bm_obj_nm);
            _bm_obj_nm = _strtmp_t635;
        } else if (1) {
            __auto_type _ = _t634;
            /* pass */
        }
        /* pass */
        AstType* ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        AstType** _gpb_ret = ((AstType**)(0LL));
        /* pass */
        if (((self->current_func_constraints->len > 0LL) && Sema__is_type_param_in_scope(self, hobj_ty->name))) {
            /* pass */
            _gpb_ret = Sema__resolve_generic_bound_method_ret(self, hobj_ty->name, method);
        }
        /* pass */
        if ((((unsigned long long)(_gpb_ret)) != ((unsigned long long)(0LL)))) {
            /* pass */
            ret_ty = (*_gpb_ret);
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("OS", 2LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("OS", 2LL))))) {
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("cwd", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("platform", 8LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            } else if ((((_tr_str_eqv((method), (_tr_str_lit_len("is_windows", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_linux", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_darwin", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_macos", 8LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            }
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("Process", 7LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Process", 7LL))))) {
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("system", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("exit", 4LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("shell_output", 12LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("Env", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Env", 3LL))))) {
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("get_var", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("cwd", 3LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("Hash", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Hash", 4LL))))) {
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("sha256", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("md5", 3LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        }
        /* pass */
        if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && _tr_str_eqv((method), (_tr_str_lit_len("write", 5LL)))) && (!self->in_unsafe))) {
            /* pass */
            Sema_error(self, _tr_str_lit_len("[P-1] '.write()' on a Pointer mutates raw memory and must be inside an 'unsafe:' block.\n      FIX: Wrap this call in 'unsafe:', e.g.\n          unsafe:\n              ... .write(...) ...", 184LL));
        }
        /* pass */
        if (((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("auto", 4LL))))) {
            /* pass */
            if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                __auto_type _t636 = (*hobj);
                if (_t636.tag == HirExpr_EIdent) {
                    __auto_type recv_nm = _t636.data.EIdent.name;
                    ret_ty = AstType_init(recv_nm);
                } else if (_t636.tag == HirExpr_EIndex) {
                    __auto_type idx_base = _t636.data.EIndex.obj;
__auto_type idx_arg = _t636.data.EIndex._tr_v_index;
                    /* pass */
                    __auto_type _t637 = (*idx_base);
                    if (_t637.tag == HirExpr_EIdent) {
                        __auto_type gn = _t637.data.EIdent.name;
                        /* pass */
                        ret_ty = AstType_init(gn);
                        /* pass */
                        __auto_type _t638 = (*idx_arg);
                        if (_t638.tag == HirExpr_EIdent) {
                            __auto_type tn = _t638.data.EIdent.name;
                            List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn)));
                        } else if (1) {
                            __auto_type _ = _t638;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t637;
                        ret_ty = hobj_ty;
                    }
                } else if (1) {
                    __auto_type _ = _t636;
                    ret_ty = hobj_ty;
                }
            } else {
                /* pass */
                ret_ty = hobj_ty;
            }
        } else if (_tr_str_eqv((method), (_tr_str_lit_len("offset", 6LL)))) {
            /* pass */
            if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (!self->cur_fn_is_lib)) && (!self->in_unsafe))) {
                /* pass */
                Sema_error(self, _tr_str_lit_len("[P-2] '.offset()' does raw pointer arithmetic (can create an out-of-bounds pointer) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or use a safe slice / Vec[T] instead of pointer math.", 209LL));
            }
            /* pass */
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((((_tr_str_eqv((method), (_tr_str_lit_len("sum", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("min_val", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max_val", 7LL)))))) {
            /* pass */
            if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((_tr_str_eqv((method), (_tr_str_lit_len("any", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("all", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL)))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("first", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("last", 4LL)))))) {
            /* pass */
            if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("reversed", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("reversed_copy", 13LL)))))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL)))))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && _tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("last_index_of", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("count", 5LL)))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && _tr_str_eqv((method), (_tr_str_lit_len("join", 4LL))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) && _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Pointer", 7LL))))) {
            /* pass */
            if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                if (((!self->cur_fn_is_lib) && (!self->in_unsafe))) {
                    /* pass */
                    Sema_error(self, _tr_str_lit_len("[P-2] '.read()' dereferences a raw pointer (may read freed or invalid memory) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or hold the value in a safe Box[T] / Vec[T] / class reference instead of a raw pointer.", 237LL));
                }
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("as_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__len__", 7LL))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((method), (_tr_str_lit_len("checked_add", 11LL))) || _tr_str_eqv((method), (_tr_str_lit_len("checked_sub", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("checked_mul", 11LL))))) {
            /* pass */
            ret_ty = AstType_init_generic(_tr_str_lit_len("Option", 6LL), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL))));
        } else if (((((((((_tr_str_eqv((method), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sign", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clamp", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("gcd", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("lcm", 3LL)))) && ((((((((((((((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("isize", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("char", 4LL)))))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f32", 3LL))))) {
            /* pass */
            if (((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("floor", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("ceil", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("round", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sqrt", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("fabs", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("log", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("log2", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("log10", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("exp", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sin", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cos", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("tan", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("asin", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("acos", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("atan", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("atan2", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("float", 5LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_nan", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_inf", 6LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("str", 3LL)))) {
            /* pass */
            ret_ty = Sema_str_method_ret_ty(self, method);
        } else if (((((((((((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("char", 4LL))))) {
            /* pass */
            if (((((((_tr_str_eqv((method), (_tr_str_lit_len("to_hex", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_HEX", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_hex_upper", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_octal", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_oct", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_binary", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_bin", 6LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_f64", 6LL))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("float", 5LL));
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("to_int", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_i64", 6LL))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL)))) && (!_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("StringBuilder", 13LL)))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if (((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL))))) {
            /* pass */
            if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (hobj_ty->args->len > 1LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
            } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
                /* pass */
                ClassDef* _gcls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
                /* pass */
                long long _gmi = 0LL;
                /* pass */
                bool _gfound = false;
                /* pass */
                while ((_gmi < _gcls->methods->len)) {
                    /* pass */
                    FunctionDef* _gmdef = ((FunctionDef*)List_ptr_get(_gcls->methods, _gmi));
                    /* pass */
                    if (_tr_str_eqv((_gmdef->name), (method))) {
                        /* pass */
                        if ((((unsigned long long)(_gmdef->ret_ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            ret_ty = (*_gmdef->ret_ty);
                            /* pass */
                            if (((_gcls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
                                /* pass */
                                ret_ty = Sema__subst_ret_generics(self, ret_ty, _gcls->generics, hobj_ty->args);
                            }
                            /* pass */
                            _gfound = true;
                        }
                    }
                    /* pass */
                    _gmi = (_gmi + 1LL);
                }
                /* pass */
                if (((!_gfound) && (hobj_ty->args->len > 0LL))) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
            } else if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            }
        } else if (_tr_str_eqv((method), (_tr_str_lit_len("alloc", 5LL)))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if (_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Array", 5LL)))) {
            /* pass */
            if (_tr_str_eqv((method), (_tr_str_lit_len("as_ptr", 6LL)))) {
                /* pass */
                AstType* _arr_elem = AstType_init(_tr_str_lit_len("u8", 2LL));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    _arr_elem = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
                /* pass */
                ret_ty = AstType_init_generic(_tr_str_lit_len("Pointer", 7LL), box_asttype(_arr_elem));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Set", 3LL)))) {
            /* pass */
            if ((((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_subset", 9LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if (((_tr_str_eqv((method), (_tr_str_lit_len("add", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("to_list", 7LL)))) {
                /* pass */
                AstType* _set_elem_ty = AstType_init(_tr_str_lit_len("str", 3LL));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    _set_elem_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
                /* pass */
                ret_ty = AstType_init_generic(_tr_str_lit_len("List", 4LL), box_asttype(_set_elem_ty));
            } else if (((_tr_str_eqv((method), (_tr_str_lit_len("union", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("intersection", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("difference", 10LL))))) {
                /* pass */
                ret_ty = hobj_ty;
            }
        } else if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Dict", 4LL))))) {
            /* pass */
            if (_tr_str_eqv((method), (_tr_str_lit_len("keys", 4LL)))) {
                /* pass */
                AstType* _dict_key_ty = AstType_init(_tr_str_lit_len("str", 3LL));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    _dict_key_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
                /* pass */
                ret_ty = AstType_init_generic(_tr_str_lit_len("List", 4LL), box_asttype(_dict_key_ty));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("values", 6LL)))) {
                /* pass */
                AstType* _dict_val_ty = AstType_init(_tr_str_lit_len("ptr", 3LL));
                /* pass */
                if ((hobj_ty->args->len > 1LL)) {
                    /* pass */
                    _dict_val_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
                }
                /* pass */
                ret_ty = AstType_init_generic(_tr_str_lit_len("List", 4LL), box_asttype(_dict_val_ty));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Option", 6LL)))) {
            /* pass */
            if (((_tr_str_eqv((method), (_tr_str_lit_len("Some", 4LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("some", 4LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Option", 6LL))))) && (hl->len == 1LL))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("Option", 6LL));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("None", 4LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("none", 4LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Option", 6LL)))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("Option", 6LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_some", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_none", 7LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("unwrap", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("expect", 6LL))))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("unwrap_or", 9LL)))) {
                /* pass */
                if ((hl->len > 0LL)) {
                    /* pass */
                    ret_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)));
                } else if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Result", 6LL)))) {
            /* pass */
            if (((_tr_str_eqv((method), (_tr_str_lit_len("Ok", 2LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("ok", 2LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Result", 6LL))))) && (hl->len == 1LL))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit_len("void", 4LL))));
            } else if (((_tr_str_eqv((method), (_tr_str_lit_len("Err", 3LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("err", 3LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Result", 6LL))))) && (hl->len == 1LL))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit_len("void", 4LL))));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_ok", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_err", 6LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("unwrap", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("ok", 2LL))))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("unwrap_err", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("err", 3LL))))) {
                /* pass */
                if ((hobj_ty->args->len > 1LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
                }
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && (_tr_str_eqv((_recv_name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Thread", 6LL)))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("Thread", 6LL));
            /* pass */
            long long _tsi = 1LL;
            /* pass */
            while ((_tsi < hl->len)) {
                /* pass */
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _tsi)))) {
                    /* pass */
                    Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to Thread.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 254LL));
                }
                /* pass */
                AstType* _tsa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _tsi)));
                /* pass */
                if ((!Sema_is_sendable_ty(self, _tsa_ty))) {
                    /* pass */
                    ({ TrStr _at_t639 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (_tsa_ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not Sendable and cannot be passed to Thread.spawn.\n      FIX: Wrap in Mutex[", 81LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tsa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("] for exclusive access, or add 'implements Sendable' to '", 57LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tsa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t639); _tr_str_release(_at_t639); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _tsa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _tsi))))) && (!Sema__is_unsafe_sendable(self, _tsa_ty->name)))) {
                    /* pass */
                    ({ TrStr _at_t640 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (_tsa_ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot be passed to Thread.spawn (exactly why Rust's 'Rc' is '!Send'): retaining/releasing it from another thread races the count.\n      FIX: share it as 'Shared[", 270LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tsa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc'). A 'Sendable' class must itself hold only Sendable + atomically-shared state.", 117LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t640); _tr_str_release(_at_t640); });
                }
                /* pass */
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _tsi)));
                /* pass */
                _tsi = (_tsi + 1LL);
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("ThreadPool", 10LL))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            /* pass */
            long long _psi = 1LL;
            /* pass */
            while ((_psi < hl->len)) {
                /* pass */
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _psi)))) {
                    /* pass */
                    Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to ThreadPool.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 258LL));
                }
                /* pass */
                AstType* _psa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _psi)));
                /* pass */
                if ((!Sema_is_sendable_ty(self, _psa_ty))) {
                    /* pass */
                    ({ TrStr _at_t641 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (_psa_ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not Sendable and cannot be passed to ThreadPool.spawn.\n      FIX: Wrap in Mutex[", 85LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_psa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("] for exclusive access, or add 'implements Sendable' to '", 57LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_psa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t641); _tr_str_release(_at_t641); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _psa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _psi))))) && (!Sema__is_unsafe_sendable(self, _psa_ty->name)))) {
                    /* pass */
                    ({ TrStr _at_t642 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (_psa_ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it cannot be passed to ThreadPool.spawn (like Rust's 'Rc' being '!Send').\n      FIX: share it as 'Shared[", 192LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_psa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc').", 40LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t642); _tr_str_release(_at_t642); });
                }
                /* pass */
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _psi)));
                /* pass */
                _psi = (_psi + 1LL);
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("AsyncPool", 9LL))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("AsyncTask", 9LL));
            /* pass */
            long long _asi = 1LL;
            /* pass */
            while ((_asi < hl->len)) {
                /* pass */
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _asi)))) {
                    /* pass */
                    Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to AsyncPool.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 257LL));
                }
                /* pass */
                AstType* _asa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _asi)));
                /* pass */
                if ((!Sema_is_sendable_ty(self, _asa_ty))) {
                    /* pass */
                    ({ TrStr _at_t643 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (_asa_ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is not Sendable and cannot be passed to AsyncPool.spawn.\n      FIX: Wrap in Mutex[", 84LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_asa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("] for exclusive access, or add 'implements Sendable' to '", 57LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_asa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t643); _tr_str_release(_at_t643); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _asa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _asi))))) && (!Sema__is_unsafe_sendable(self, _asa_ty->name)))) {
                    /* pass */
                    ({ TrStr _at_t644 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (_asa_ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it cannot be passed to AsyncPool.spawn (like Rust's 'Rc' being '!Send'): the task may run on a different worker than the one that submitted it.\n      FIX: share it as 'Shared[", 262LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_asa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc').", 40LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t644); _tr_str_release(_at_t644); });
                }
                /* pass */
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _asi)));
                /* pass */
                _asi = (_asi + 1LL);
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("AsyncTask", 9LL)))) {
            /* pass */
            if (_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("done", 4LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Atomic", 6LL)))) {
            /* pass */
            if ((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("load", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("swap", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("exchange", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_add", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_sub", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("load_relaxed", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("load_acquire", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("load_seqcst", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add_relaxed", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add_release", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add_acqrel", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub_relaxed", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub_release", 11LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if ((((_tr_str_eqv((method), (_tr_str_lit_len("cas", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("compare_exchange", 16LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cas_weak", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cas_acqrel", 10LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (((((_tr_str_eqv((method), (_tr_str_lit_len("store", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("store_relaxed", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("store_release", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("store_seqcst", 12LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Chan", 4LL)))) {
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("recv", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("try_recv", 8LL))))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("cap", 3LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_closed", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("try_send", 8LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("send_timeout", 12LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("recv_timeout", 12LL)))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Mutex", 5LL)))) {
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("lock", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("RwLock", 6LL)))) {
            /* pass */
            if ((((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("read_lock", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("write_lock", 10LL))))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("StringBuilder", 13LL))) && _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("StringObj", 9LL));
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Shared", 6LL)))) {
            /* pass */
            if (_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL)))) {
                /* pass */
                ret_ty = hobj_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("downgrade", 9LL)))) {
                /* pass */
                AstType* weak_ty = AstType_init(_tr_str_lit_len("Weak", 4LL));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    weak_ty->args = (void*)List_ptr_new();
                    /* pass */
                    List_ptr_append(weak_ty->args, box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL)))));
                }
                /* pass */
                ret_ty = weak_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("drop", 4LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Weak", 4LL)))) {
            /* pass */
            if (_tr_str_eqv((method), (_tr_str_lit_len("upgrade", 7LL)))) {
                /* pass */
                AstType* opt_ty = AstType_init(_tr_str_lit_len("Option", 6LL));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    opt_ty->args = (void*)List_ptr_new();
                    /* pass */
                    List_ptr_append(opt_ty->args, box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL)))));
                }
                /* pass */
                ret_ty = opt_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("is_alive", 8LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            }
        } else if ((_recv_is_shared && (((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("downgrade", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("drop", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))))) {
            /* pass */
            if (_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL)))) {
                /* pass */
                ret_ty = hobj_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("drop", 4LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("downgrade", 9LL)))) {
                /* pass */
                AstType* _wk_ty = AstType_init(_tr_str_lit_len("Weak", 4LL));
                /* pass */
                _wk_ty->args = (void*)List_ptr_new();
                /* pass */
                List_ptr_append(_wk_ty->args, box_asttype(hobj_ty));
                /* pass */
                ret_ty = _wk_ty;
            }
        } else if (((((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))) && _tr_dict_contains(self->interfaces, _tr_strz(hobj_ty->name)))) {
            /* pass */
            InterfaceDef* _iface_def = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(hobj_ty->name)));
            /* pass */
            long long _imi = 0LL;
            /* pass */
            while ((_imi < _iface_def->methods->len)) {
                /* pass */
                FunctionDef* _imdef = ((FunctionDef*)List_ptr_get(_iface_def->methods, _imi));
                /* pass */
                if (_tr_str_eqv((_imdef->name), (method))) {
                    /* pass */
                    if ((((unsigned long long)(_imdef->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        ret_ty = (*_imdef->ret_ty);
                    }
                }
                /* pass */
                _imi = (_imi + 1LL);
            }
            /* pass */
            if (((_iface_def->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                long long _gi5 = 0LL;
                /* pass */
                while ((_gi5 < _iface_def->generics->len)) {
                    /* pass */
                    if (_tr_str_eqv((List_TrStr_get(_iface_def->generics, _gi5)), (ret_ty->name))) {
                        /* pass */
                        if ((_gi5 < hobj_ty->args->len)) {
                            /* pass */
                            ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, _gi5)));
                        }
                    }
                    /* pass */
                    _gi5 = (_gi5 + 1LL);
                }
            }
        } else if (_tr_dict_contains(self->enums, _tr_strz(hobj_ty->name))) {
            /* pass */
            ret_ty = AstType_init(hobj_ty->name);
            /* pass */
            if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Option", 6LL))) && _tr_str_eqv((method), (_tr_str_lit_len("Some", 4LL)))) && (hl->len == 1LL))) {
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((method), (_tr_str_lit_len("Ok", 2LL)))) && (hl->len == 1LL))) {
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit_len("void", 4LL))));
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((method), (_tr_str_lit_len("Err", 3LL)))) && (hl->len == 1LL))) {
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit_len("void", 4LL))));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
            }
        } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
            /* pass */
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
            /* pass */
            TrStr _hty_n = _tr_str_retain(hobj_ty->name);
            /* pass */
            bool _is_builtin_dispatch = (((((((((((((((((_tr_str_eqv((_hty_n), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("OS", 2LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Process", 7LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Env", 3LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Hash", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("File", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("AsyncTask", 9LL))));
            /* pass */
            if ((((!_is_builtin_dispatch) && (!Sema_class_method_exists(self, hobj_ty->name, method))) && (!Sema_is_universal_method(self, method)))) {
                /* pass */
                ({ TrStr _at_t645 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] No method '", 17LL)), (method))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' found on type '", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (hobj_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'.\n      FIX: Define 'pub def ", 30LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (method)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(self, ...)' in '", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (hobj_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' or its base class via 'extend ", 32LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (hobj_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":'.", 3LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t645); _tr_str_release(_at_t645); });
            }
            /* pass */
            long long _mi = 0LL;
            /* pass */
            AstType** _mdef_throws_ty = ((AstType**)(0LL));
            /* pass */
            while ((_mi < _cls->methods->len)) {
                /* pass */
                FunctionDef* _mdef = ((FunctionDef*)List_ptr_get(_cls->methods, _mi));
                /* pass */
                if (_tr_str_eqv((_mdef->name), (method))) {
                    /* pass */
                    if ((((unsigned long long)(_mdef->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        long long _mpc = 0LL;
                        /* pass */
                        long long _mpi = 0LL;
                        /* pass */
                        while ((_mpi < _mdef->params->len)) {
                            /* pass */
                            if ((!_tr_str_eqv((((Param*)List_ptr_get(_mdef->params, _mpi))->name), (_tr_str_lit_len("self", 4LL))))) {
                                /* pass */
                                _mpc = (_mpc + 1LL);
                            }
                            /* pass */
                            _mpi = (_mpi + 1LL);
                        }
                        /* pass */
                        if ((_mpc == hl->len)) {
                            /* pass */
                            ret_ty = (*_mdef->ret_ty);
                            /* pass */
                            _mdef_throws_ty = _mdef->throws_ty;
                        } else if (_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("void", 4LL)))) {
                            /* pass */
                            ret_ty = (*_mdef->ret_ty);
                            /* pass */
                            _mdef_throws_ty = _mdef->throws_ty;
                        }
                    }
                }
                /* pass */
                _mi = (_mi + 1LL);
            }
            /* pass */
            if ((((unsigned long long)(_mdef_throws_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                if ((!_tr_str_eqv(((*_mdef_throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    AstType* _mres_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
                    /* pass */
                    List_ptr_append(_mres_ty->args, box_asttype(ret_ty));
                    /* pass */
                    List_ptr_append(_mres_ty->args, _mdef_throws_ty);
                    /* pass */
                    ret_ty = _mres_ty;
                }
            }
            /* pass */
            if (((_cls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                ret_ty = Sema__subst_ret_generics(self, ret_ty, _cls->generics, hobj_ty->args);
            }
        }
        /* pass */
        _tr_str_release(_recv_name);
        _tr_str_release(_bm_obj_nm);
        return box_hirexpr(HirExpr_ctor_EMethodCall(hobj, method, hl, ret_ty));
    } else if (_t575.tag == Expr_EPropAccess) {
        __auto_type obj = _t575.data.EPropAccess.obj;
__auto_type prop = _t575.data.EPropAccess.prop;
        /* pass */
        __auto_type _t646 = (*obj);
        if (_t646.tag == Expr_EIdent) {
            __auto_type _pmv_cn = _t646.data.EIdent.name;
            /* pass */
            Symbol* _pmv_sym = Sema_resolve(self, _pmv_cn);
            /* pass */
            if ((((_tr_str_eqv((_pmv_sym->name), (_tr_str_lit_len("", 0LL))) || (_pmv_sym->kind.tag == SymbolKind_make_SClass().tag)) && _tr_dict_contains(self->classes, _tr_strz(_pmv_cn))) && Sema_class_method_exists(self, _pmv_cn, prop))) {
                /* pass */
                ClassDef* _pmv_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_pmv_cn)));
                /* pass */
                FunctionDef* _pmv_m = ((FunctionDef*)List_ptr_get(_pmv_cls->methods, 0LL));
                /* pass */
                bool _pmv_found = false;
                /* pass */
                long long _pmv_mi = 0LL;
                /* pass */
                while ((_pmv_mi < _pmv_cls->methods->len)) {
                    /* pass */
                    FunctionDef* _pmv_mm = ((FunctionDef*)List_ptr_get(_pmv_cls->methods, _pmv_mi));
                    /* pass */
                    if (_tr_str_eqv((_pmv_mm->name), (prop))) {
                        /* pass */
                        bool _pmv_is_static = true;
                        /* pass */
                        if (((_pmv_mm->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_pmv_mm->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
                            /* pass */
                            _pmv_is_static = false;
                        }
                        /* pass */
                        if (_pmv_is_static) {
                            /* pass */
                            _pmv_m = _tr_obj_retain(_pmv_mm);
                            /* pass */
                            _pmv_found = true;
                        }
                    }
                    /* pass */
                    _pmv_mi = (_pmv_mi + 1LL);
                }
                /* pass */
                if (_pmv_found) {
                    /* pass */
                    AstType* _pmv_fnty = AstType_init(_tr_str_lit_len("def", 3LL));
                    /* pass */
                    long long _pmv_pi = 0LL;
                    /* pass */
                    while ((_pmv_pi < _pmv_m->params->len)) {
                        /* pass */
                        List_ptr_append(_pmv_fnty->args, ((Param*)List_ptr_get(_pmv_m->params, _pmv_pi))->ty);
                        /* pass */
                        _pmv_pi = (_pmv_pi + 1LL);
                    }
                    /* pass */
                    if ((((unsigned long long)(_pmv_m->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        List_ptr_append(_pmv_fnty->args, _pmv_m->ret_ty);
                    } else {
                        /* pass */
                        List_ptr_append(_pmv_fnty->args, box_asttype(AstType_init(_tr_str_lit_len("void", 4LL))));
                    }
                    /* pass */
                    return ({ TrStr _at_t647 = (({ TrStr _cl = (_tr_strx_concatv((_pmv_cn), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (prop)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t647, _pmv_fnty, false))); _tr_str_release(_at_t647); _wr; });
                }
            }
        } else if (1) {
            __auto_type _ = _t646;
            /* pass */
        }
        /* pass */
        bool _saved_recv_pa = self->in_recv_pos;
        /* pass */
        self->in_recv_pos = true;
        /* pass */
        HirExpr* hobj = Sema_lower_expr(self, obj);
        /* pass */
        self->in_recv_pos = _saved_recv_pa;
        /* pass */
        TrStr hobj_ty_n = _tr_str_retain(hir_expr_type(hobj)->name);
        /* pass */
        AstType* hobj_ty_full = hir_expr_type(hobj);
        /* pass */
        AstType* ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Result", 6LL)))) {
            /* pass */
            if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_err", 6LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_ok", 5LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((prop), (_tr_str_lit_len("ok", 2LL))) && (hobj_ty_full->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            } else if ((_tr_str_eqv((prop), (_tr_str_lit_len("err", 3LL))) && (hobj_ty_full->args->len > 1LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 1LL)));
            }
        } else if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Option", 6LL)))) {
            /* pass */
            if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_some", 7LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_none", 7LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (((_tr_str_eqv((prop), (_tr_str_lit_len("value", 5LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("val", 3LL)))) && (hobj_ty_full->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            }
        } else if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("StringObj", 9LL)))) {
            /* pass */
            if (_tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
            } else if ((_tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("StringBuilder", 13LL)))) {
            /* pass */
            if (_tr_str_eqv((prop), (_tr_str_lit_len("buf", 3LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("StringObj", 9LL));
            }
        } else if ((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("List", 4LL))))) {
            /* pass */
            if (((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if (_tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
            }
        } else if (((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Set", 3LL))))) {
            /* pass */
            if (((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL))))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Shared", 6LL))) && (hobj_ty_full->args->len > 0LL)) && _tr_dict_contains(self->classes, _tr_strz((*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)))->name)))) {
            /* pass */
            ClassDef* _scls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz((*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)))->name)));
            /* pass */
            long long _sfi = 0LL;
            /* pass */
            while ((_sfi < _scls->fields->len)) {
                /* pass */
                FieldDef* _sfld = ((FieldDef*)List_ptr_get(_scls->fields, _sfi));
                /* pass */
                if (_tr_str_eqv((_sfld->name), (prop))) {
                    /* pass */
                    if ((((unsigned long long)(_sfld->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        ret_ty = (*_sfld->ty);
                    }
                }
                /* pass */
                _sfi = (_sfi + 1LL);
            }
        } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty_n))) {
            /* pass */
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty_n)));
            /* pass */
            long long _fi = 0LL;
            /* pass */
            while ((_fi < _cls->fields->len)) {
                /* pass */
                FieldDef* _fld = ((FieldDef*)List_ptr_get(_cls->fields, _fi));
                /* pass */
                if (_tr_str_eqv((_fld->name), (prop))) {
                    /* pass */
                    if ((((unsigned long long)(_fld->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        ret_ty = (*_fld->ty);
                    }
                }
                /* pass */
                _fi = (_fi + 1LL);
            }
            /* pass */
            if (((_cls->generics->len > 0LL) && (hobj_ty_full->args->len > 0LL))) {
                /* pass */
                ret_ty = Sema__subst_ret_generics(self, ret_ty, _cls->generics, hobj_ty_full->args);
            }
        } else if (_tr_dict_contains(self->enums, _tr_strz(hobj_ty_n))) {
            /* pass */
            ret_ty = AstType_init(hobj_ty_n);
        }
        /* pass */
        if ((_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("void", 4LL))) && ((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL)))))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        }
        /* pass */
        _tr_str_release(hobj_ty_n);
        return box_hirexpr(HirExpr_ctor_EPropAccess(hobj, prop, ret_ty));
    } else if (_t575.tag == Expr_EIndex) {
        __auto_type obj = _t575.data.EIndex.obj;
__auto_type idx_inner = _t575.data.EIndex._tr_v_index;
        /* pass */
        HirExpr* hexpr_obj = Sema_lower_expr(self, obj);
        /* pass */
        TrStr obj_ty_n = _tr_str_retain(hir_expr_type(hexpr_obj)->name);
        /* pass */
        TrStr obj_name = _tr_str_lit_len("", 0LL);
        /* pass */
        __auto_type _t648 = (*obj);
        if (_t648.tag == Expr_EIdent) {
            __auto_type n = _t648.data.EIdent.name;
            TrStr _strtmp_t649 = _tr_str_retain(n);
            _tr_str_release(obj_name);
            obj_name = _strtmp_t649;
        } else if (1) {
            __auto_type _ = _t648;
            /* pass */
        }
        /* pass */
        bool is_generic = false;
        /* pass */
        TrStr generic_arg_n = _tr_str_lit_len("", 0LL);
        /* pass */
        AstType** generic_arg_ty = ((AstType**)(0LL));
        /* pass */
        List_ptr* generic_args = (void*)List_ptr_new();
        /* pass */
        if ((((unsigned long long)(idx_inner)) == ((unsigned long long)(0LL)))) {
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return hexpr_obj;
        }
        /* pass */
        __auto_type _t650 = (*idx_inner);
        if (_t650.tag == Expr_ETuple) {
            __auto_type _tup_targs = _t650.data.ETuple.items;
            /* pass */
            is_generic = true;
            /* pass */
            long long _tti = 0LL;
            /* pass */
            while ((_tti < _tup_targs->len)) {
                /* pass */
                List_ptr_append(generic_args, Sema__targ_of(self, ((Expr*)List_ptr_get(_tup_targs, _tti))));
                /* pass */
                _tti = (_tti + 1LL);
            }
            /* pass */
            if ((generic_args->len > 0LL)) {
                /* pass */
                generic_arg_ty = ((AstType**)List_ptr_get(generic_args, 0LL));
                /* pass */
                TrStr _strtmp_t651 = _tr_str_retain((*((AstType**)List_ptr_get(generic_args, 0LL)))->name);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t651;
            }
        } else if (_t650.tag == Expr_ETypeArg) {
            __auto_type targ_ty = _t650.data.ETypeArg.ty;
            /* pass */
            is_generic = true;
            /* pass */
            generic_arg_ty = targ_ty;
            /* pass */
            TrStr _strtmp_t652 = _tr_str_retain((*targ_ty)->name);
            _tr_str_release(generic_arg_n);
            generic_arg_n = _strtmp_t652;
        } else if (_t650.tag == Expr_EIdent) {
            __auto_type iname = _t650.data.EIdent.name;
            /* pass */
            bool is_param = false;
            /* pass */
            if ((!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
                    /* pass */
                    ClassDef* cc = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
                    /* pass */
                    long long ci = 0LL;
                    /* pass */
                    while ((ci < cc->generics->len)) {
                        /* pass */
                        if (_tr_str_eqv((List_TrStr_get(cc->generics, ci)), (iname))) {
                            /* pass */
                            is_param = true;
                        }
                        /* pass */
                        ci = (ci + 1LL);
                    }
                }
            }
            /* pass */
            long long fi = 0LL;
            /* pass */
            while ((fi < self->current_func_generics->len)) {
                /* pass */
                if (_tr_str_eqv((List_TrStr_get(self->current_func_generics, fi)), (iname))) {
                    /* pass */
                    is_param = true;
                }
                /* pass */
                fi = (fi + 1LL);
            }
            /* pass */
            if ((((((((((((((Sema_is_primitive_name(self, iname) || _tr_str_eqv((iname), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("Str", 3LL)))) || is_param) || (_tr_strlen(_tr_strz(iname)) == 1LL)) || _tr_dict_contains(self->classes, _tr_strz(iname))) || _tr_str_eqv((iname), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_dict_contains(self->enums, _tr_strz(iname))) || _tr_str_eqv((iname), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("tuple", 5LL))))) {
                /* pass */
                is_generic = true;
                /* pass */
                TrStr _strtmp_t653 = _tr_str_retain(iname);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t653;
            }
        } else if (_t650.tag == Expr_EIndex) {
            /* pass */
            is_generic = true;
            /* pass */
            AstType** nested_ty = Sema_build_ast_type(self, idx_inner);
            /* pass */
            if ((((unsigned long long)(nested_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                TrStr _strtmp_t654 = _tr_str_retain((*nested_ty)->name);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t654;
                /* pass */
                generic_arg_ty = nested_ty;
            }
        } else if (1) {
            __auto_type _ = _t650;
            /* pass */
        }
        /* pass */
        bool obj_is_type = (((((((((((_tr_str_eqv((obj_name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Pointer", 7LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("alloc", 5LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("dealloc", 7LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("resize", 6LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("copy", 4LL)))) || _tr_dict_contains(self->classes, _tr_strz(obj_name))) || _tr_dict_contains(self->enums, _tr_strz(obj_name)));
        /* pass */
        if ((is_generic && obj_is_type)) {
            /* pass */
            TrStr eff_ty_n = _tr_str_retain(obj_ty_n);
            /* pass */
            if (((_tr_str_eqv((eff_ty_n), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("", 0LL)))) && (((((_tr_str_eqv((obj_name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Pointer", 7LL)))))) {
                /* pass */
                TrStr _strtmp_t655 = _tr_str_retain(obj_name);
                _tr_str_release(eff_ty_n);
                eff_ty_n = _strtmp_t655;
            }
            /* pass */
            if (((((((_tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Pointer", 7LL)))) && (!_tr_str_eqv((generic_arg_n), (_tr_str_lit_len("", 0LL)))))) {
                /* pass */
                AstType* container_ty = AstType_init(eff_ty_n);
                /* pass */
                container_ty->args = (void*)List_ptr_new();
                /* pass */
                if ((generic_args->len > 0LL)) {
                    /* pass */
                    long long _gci = 0LL;
                    /* pass */
                    while ((_gci < generic_args->len)) {
                        /* pass */
                        List_ptr_append(container_ty->args, ((AstType**)List_ptr_get(generic_args, _gci)));
                        /* pass */
                        _gci = (_gci + 1LL);
                    }
                } else if ((((unsigned long long)(generic_arg_ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    List_ptr_append(container_ty->args, generic_arg_ty);
                } else {
                    /* pass */
                    List_ptr_append(container_ty->args, box_asttype(AstType_init(generic_arg_n)));
                }
                /* pass */
                if ((((_tr_str_eqv((obj_name), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("dealloc", 7LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("resize", 6LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("copy", 4LL))))) {
                    /* pass */
                    _tr_str_release(obj_ty_n);
                    _tr_str_release(obj_name);
                    _tr_str_release(generic_arg_n);
                    _tr_str_release(eff_ty_n);
                    return box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_expr(self, idx_inner), container_ty));
                }
                /* pass */
                _tr_str_release(obj_ty_n);
                _tr_str_release(obj_name);
                _tr_str_release(generic_arg_n);
                return box_hirexpr(HirExpr_ctor_EIdent(eff_ty_n, container_ty, false));
            }
            /* pass */
            if (((_tr_dict_contains(self->classes, _tr_strz(obj_name)) || _tr_dict_contains(self->enums, _tr_strz(obj_name))) && (!_tr_str_eqv((generic_arg_n), (_tr_str_lit_len("", 0LL)))))) {
                /* pass */
                AstType* cls_ty = AstType_init(obj_name);
                /* pass */
                cls_ty->args = (void*)List_ptr_new();
                /* pass */
                if ((generic_args->len > 0LL)) {
                    /* pass */
                    long long _cgi = 0LL;
                    /* pass */
                    while ((_cgi < generic_args->len)) {
                        /* pass */
                        List_ptr_append(cls_ty->args, ((AstType**)List_ptr_get(generic_args, _cgi)));
                        /* pass */
                        _cgi = (_cgi + 1LL);
                    }
                } else if ((((unsigned long long)(generic_arg_ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    List_ptr_append(cls_ty->args, generic_arg_ty);
                } else {
                    /* pass */
                    List_ptr_append(cls_ty->args, box_asttype(AstType_init(generic_arg_n)));
                }
                /* pass */
                Sema_check_class_bounds(self, obj_name, cls_ty->args);
                /* pass */
                _tr_str_release(obj_ty_n);
                _tr_str_release(generic_arg_n);
                _tr_str_release(eff_ty_n);
                return box_hirexpr(HirExpr_ctor_EIdent(obj_name, cls_ty, false));
            }
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            _tr_str_release(eff_ty_n);
            return hexpr_obj;
        }
        /* pass */
        if ((_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Dict", 4LL))))) {
            /* pass */
            AstType* dval_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            /* pass */
            AstType* dobj_full = hir_expr_type(hexpr_obj);
            /* pass */
            if ((dobj_full->args->len > 1LL)) {
                /* pass */
                dval_ty = (*((AstType**)List_ptr_get(dobj_full->args, 1LL)));
            }
            /* pass */
            List_ptr* didx_args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(didx_args, Sema_lower_expr(self, idx_inner));
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit_len("get_index", 9LL), didx_args, dval_ty));
        }
        /* pass */
        if (_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Tuple", 5LL)))) {
            /* pass */
            AstType* tup_full = hir_expr_type(hexpr_obj);
            /* pass */
            AstType* telem_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            /* pass */
            __auto_type _t656 = (*idx_inner);
            if (_t656.tag == Expr_ELitInt) {
                __auto_type tup_ci = _t656.data.ELitInt.val;
                /* pass */
                if (((tup_ci >= 0LL) && (tup_ci < tup_full->args->len))) {
                    /* pass */
                    telem_ty = (*((AstType**)List_ptr_get(tup_full->args, tup_ci)));
                }
            } else if (1) {
                __auto_type _ = _t656;
                /* pass */
            }
            /* pass */
            List_ptr* tup_args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(tup_args, Sema_lower_expr(self, idx_inner));
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit_len("get_index", 9LL), tup_args, telem_ty));
        }
        /* pass */
        if ((((((!Sema_is_primitive_name(self, obj_ty_n)) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("str", 3LL))))) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Pointer", 7LL))))) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Vec", 3LL)))))) {
            /* pass */
            List_ptr* call_args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(call_args, Sema_lower_expr(self, idx_inner));
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit_len("get_index", 9LL), call_args, AstType_init(_tr_str_lit_len("void", 4LL))));
        }
        /* pass */
        AstType* elem_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        if ((_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Vec", 3LL))))) {
            /* pass */
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
                /* pass */
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if (_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Pointer", 7LL)))) {
            /* pass */
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
                /* pass */
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if (_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("str", 3LL)))) {
            /* pass */
            elem_ty = AstType_init(_tr_str_lit_len("char", 4LL));
        }
        /* pass */
        _tr_str_release(obj_ty_n);
        _tr_str_release(obj_name);
        _tr_str_release(generic_arg_n);
        return box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_expr(self, idx_inner), elem_ty));
    } else if (_t575.tag == Expr_ESizeOf) {
        __auto_type ty = _t575.data.ESizeOf.ty;
        /* pass */
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return box_hirexpr(HirExpr_ctor_ESizeOf(AstType_init(_tr_str_lit_len("void", 4LL)), AstType_init(_tr_str_lit_len("int", 3LL))));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ESizeOf((*ty), AstType_init(_tr_str_lit_len("int", 3LL))));
    } else if (_t575.tag == Expr_ECast) {
        __auto_type expr = _t575.data.ECast.expr;
__auto_type ty = _t575.data.ECast.ty;
        /* pass */
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return Sema_lower_expr(self, expr);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ECast(Sema_lower_expr(self, expr), (*ty)));
    } else if (_t575.tag == Expr_EFString) {
        __auto_type parts = _t575.data.EFString.parts;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit_len("an f-string", 11LL), _tr_str_lit_len("build output in a fixed byte buffer, or print the parts separately; plain string literals are fine (they live in rodata)", 120LL));
        /* pass */
        List_ptr* hparts = (void*)List_ptr_new();
        /* pass */
        long long m = 0LL;
        /* pass */
        while ((m < parts->len)) {
            /* pass */
            FStringPart* p_ast = ((FStringPart*)List_ptr_get(parts, m));
            /* pass */
            HirFStringPart* hp = HirFStringPart_init();
            /* pass */
            hp->is_expr = p_ast->is_expr;
            /* pass */
            hp->text = _tr_str_retain(p_ast->text);
            /* pass */
            hp->text_len = p_ast->text_len;
            /* pass */
            hp->fmt_spec = _tr_str_retain(p_ast->fmt_spec);
            /* pass */
            hp->expr = Sema_lower_expr(self, p_ast->expr);
            /* pass */
            List_ptr_append(hparts, _tr_obj_retain(hp));
            /* pass */
            m = (m + 1LL);
            _tr_obj_release(hp, _trdrop_HirFStringPart);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EFString(hparts, AstType_init(_tr_str_lit_len("str", 3LL))));
    } else if (_t575.tag == Expr_ETuple) {
        __auto_type items = _t575.data.ETuple.items;
        /* pass */
        if ((self->no_heap && self->heap_boxes_tuples)) {
            /* pass */
            Sema_check_no_heap(self, _tr_str_lit_len("a tuple literal (heap-boxed on the LLVM/native backend)", 55LL), _tr_str_lit_len("use the C backend (stack tuples) for --no-heap, or return values via out-params / a @value_type struct", 102LL));
        }
        /* pass */
        List_ptr* hitems = (void*)List_ptr_new();
        /* pass */
        long long n = 0LL;
        /* pass */
        while ((n < items->len)) {
            /* pass */
            List_ptr_append(hitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, n))));
            /* pass */
            n = (n + 1LL);
        }
        /* pass */
        AstType* tup_ty = AstType_init(_tr_str_lit_len("Tuple", 5LL));
        /* pass */
        long long m = 0LL;
        /* pass */
        while ((m < hitems->len)) {
            /* pass */
            List_ptr_append(tup_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hitems, m)))));
            /* pass */
            m = (m + 1LL);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ETuple(hitems, tup_ty));
    } else if (_t575.tag == Expr_EList) {
        __auto_type items = _t575.data.EList.items;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit_len("a list literal []", 17LL), _tr_str_lit_len("use a fixed-size array [T; N] (stack, zero-heap) instead", 56LL));
        /* pass */
        List_ptr* hitems = (void*)List_ptr_new();
        /* pass */
        long long n = 0LL;
        /* pass */
        while ((n < items->len)) {
            /* pass */
            List_ptr_append(hitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, n))));
            /* pass */
            n = (n + 1LL);
        }
        /* pass */
        AstType* list_ty = AstType_init(_tr_str_lit_len("List", 4LL));
        /* pass */
        if ((hitems->len > 0LL)) {
            /* pass */
            list_ty->args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(list_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hitems, 0LL)))));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EList(hitems, list_ty));
    } else if (_t575.tag == Expr_ESet) {
        __auto_type items = _t575.data.ESet.items;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit_len("a set literal", 13LL), _tr_str_lit_len("use a fixed-size array [T; N] + linear scan, or a bitset over a fixed word", 74LL));
        /* pass */
        List_ptr* sitems = (void*)List_ptr_new();
        /* pass */
        long long sn = 0LL;
        /* pass */
        while ((sn < items->len)) {
            /* pass */
            List_ptr_append(sitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, sn))));
            /* pass */
            sn = (sn + 1LL);
        }
        /* pass */
        AstType* set_ty = AstType_init(_tr_str_lit_len("Set", 3LL));
        /* pass */
        if ((sitems->len > 0LL)) {
            /* pass */
            set_ty->args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(set_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(sitems, 0LL)))));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ESet(sitems, set_ty));
    } else if (_t575.tag == Expr_EClosure) {
        __auto_type params = _t575.data.EClosure.params;
__auto_type ret_ty = _t575.data.EClosure.ret_ty;
__auto_type body = _t575.data.EClosure.body;
__auto_type is_async = _t575.data.EClosure.is_async;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        long long cap_idx = (self->scopes->len - 1LL);
        /* pass */
        List_ptr* hparams = (void*)List_ptr_new();
        /* pass */
        long long l = 0LL;
        /* pass */
        while ((l < params->len)) {
            /* pass */
            Param* pa = ((Param*)List_ptr_get(params, l));
            /* pass */
            AstType* pa_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            /* pass */
            if ((((unsigned long long)(pa->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                pa_ty = (*pa->ty);
            }
            /* pass */
            Sema_declare(self, pa->name, SymbolKind_make_SVariable(), box_asttype(pa_ty), false);
            /* pass */
            HirParam* hpa = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
            /* pass */
            hpa->name = _tr_str_retain(pa->name);
            /* pass */
            hpa->ty = pa_ty;
            /* pass */
            List_ptr_append(hparams, _tr_obj_retain(hpa));
            /* pass */
            l = (l + 1LL);
            _tr_obj_release(hpa, _trdrop_HirParam);
        }
        /* pass */
        AstType* r_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        if ((((unsigned long long)(ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            r_ty = (*ret_ty);
        }
        /* pass */
        List_i64_append(self->fn_scope_base, (self->scopes->len - 1LL));
        /* pass */
        HirBlock* clo_body = Sema_lower_block(self, body);
        /* pass */
        List_i64_pop(self->fn_scope_base);
        /* pass */
        Sema_finalize_scope_drops(self, clo_body);
        /* pass */
        List_TrStr* clo_refs = (void*)List_TrStr_new();
        /* pass */
        Sema_collect_block_refs(self, clo_body, clo_refs);
        /* pass */
        List_ptr* clo_caps = (void*)List_ptr_new();
        /* pass */
        TrMap* clo_seen = _tr_dict_new(8LL);
        /* pass */
        long long cri = 0LL;
        /* pass */
        while ((cri < clo_refs->len)) {
            /* pass */
            TrStr rn = List_TrStr_get(clo_refs, cri);
            /* pass */
            if ((!_tr_dict_contains(clo_seen, _tr_strz(rn)))) {
                /* pass */
                _tr_dict_set(clo_seen, _tr_strz(rn), true);
                /* pass */
                if ((!_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, cap_idx))->variables, _tr_strz(rn)))) {
                    /* pass */
                    long long si = (cap_idx - 1LL);
                    /* pass */
                    AstType** cty_p = (AstType**)(0LL);
                    /* pass */
                    while ((si >= 0LL)) {
                        /* pass */
                        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, si))->variables, _tr_strz(rn))) {
                            /* pass */
                            cty_p = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, si))->variables, _tr_strz(rn)))->ty;
                            /* pass */
                            si = (0LL - 1LL);
                        } else {
                            /* pass */
                            si = (si - 1LL);
                        }
                    }
                    /* pass */
                    if ((((unsigned long long)(cty_p)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        HirParam* ccp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
                        /* pass */
                        ccp->name = _tr_str_retain(rn);
                        /* pass */
                        ccp->ty = (*cty_p);
                        /* pass */
                        List_ptr_append(clo_caps, _tr_obj_retain(ccp));
                    }
                }
            }
            /* pass */
            cri = (cri + 1LL);
            _tr_str_release(rn);
        }
        /* pass */
        HirExpr hexpr = HirExpr_ctor_EClosure(hparams, r_ty, clo_body, is_async, clo_caps);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        _tr_obj_release(clo_body, _trdrop_HirBlock);
        Dict_free(clo_seen);
        return box_hirexpr(hexpr);
    } else if (_t575.tag == Expr_EIfElse) {
        __auto_type cond = _t575.data.EIfElse.cond;
__auto_type then_e = _t575.data.EIfElse.then_expr;
__auto_type else_e = _t575.data.EIfElse.else_expr;
        /* pass */
        HirExpr* hcond = Sema_lower_expr(self, cond);
        /* pass */
        HirExpr* hthen = Sema_lower_expr(self, then_e);
        /* pass */
        HirExpr* helse = Sema_lower_expr(self, else_e);
        /* pass */
        AstType* ite_ty = hir_expr_type(hthen);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EIfElse(hcond, hthen, helse, ite_ty));
    } else if (_t575.tag == Expr_EDo) {
        __auto_type do_body = _t575.data.EDo.body;
        /* pass */
        return Sema_lower_do_value(self, do_body);
    } else if (_t575.tag == Expr_EMatch) {
        __auto_type m_subj = _t575.data.EMatch.subj;
__auto_type m_arms = _t575.data.EMatch.arms;
        /* pass */
        HirExpr* hm_subj = Sema_lower_expr(self, m_subj);
        /* pass */
        AstType* hm_subj_ty = hir_expr_type(hm_subj);
        /* pass */
        List_ptr* hm_arms = (void*)List_ptr_new();
        /* pass */
        AstType* hm_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        long long hm_k = 0LL;
        /* pass */
        while ((hm_k < m_arms->len)) {
            /* pass */
            MatchArm* hm_arm = ((MatchArm*)List_ptr_get(m_arms, hm_k));
            /* pass */
            Sema_enter_scope(self);
            /* pass */
            Sema_declare_pattern_binds_typed(self, hm_arm->pat, hm_subj_ty);
            /* pass */
            HirExpr* hm_edo = Sema_lower_do_value(self, (*hm_arm->body));
            /* pass */
            if ((!_tr_str_eqv((hir_expr_type(hm_edo)->name), (_tr_str_lit_len("void", 4LL))))) {
                /* pass */
                hm_ty = hir_expr_type(hm_edo);
            }
            /* pass */
            HirBlock* hm_body = HirBlock_init();
            /* pass */
            HirBlock_push(hm_body, box_hirstmt(HirStmt_ctor_SExpr(hm_edo)));
            /* pass */
            HirMatchArm* h_marm = HirMatchArm_init(hm_arm->pat, hm_body);
            /* pass */
            if ((((unsigned long long)(hm_arm->guard)) != ((unsigned long long)(0LL)))) {
                /* pass */
                h_marm->guard = Sema_lower_expr(self, hm_arm->guard);
            }
            /* pass */
            List_ptr_append(hm_arms, _tr_obj_retain(h_marm));
            /* pass */
            Sema_exit_scope(self);
            /* pass */
            hm_k = (hm_k + 1LL);
            _tr_obj_release(hm_body, _trdrop_HirBlock);
            _tr_obj_release(h_marm, _trdrop_HirMatchArm);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EMatchExpr(hm_subj, hm_arms, hm_ty));
    } else if (_t575.tag == Expr_ELoop) {
        __auto_type loop_body = _t575.data.ELoop.body;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
        /* pass */
        HirBlock* h_loop_body = Sema_lower_block(self, loop_body);
        /* pass */
        List_i64_pop(self->loop_scope_base);
        /* pass */
        Sema_finalize_scope_drops(self, h_loop_body);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        AstType* loop_ty = Sema_infer_break_type(self, h_loop_body);
        /* pass */
        return box_hirexpr(HirExpr_ctor_ELoop(h_loop_body, loop_ty));
    } else if (_t575.tag == Expr_EWhileExpr) {
        __auto_type we_cond = _t575.data.EWhileExpr.cond;
__auto_type we_body = _t575.data.EWhileExpr.body;
__auto_type we_else = _t575.data.EWhileExpr.else_body;
        /* pass */
        HirExpr* h_we_cond = Sema_lower_expr(self, we_cond);
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
        /* pass */
        HirBlock* h_we_body = Sema_lower_block(self, we_body);
        /* pass */
        List_i64_pop(self->loop_scope_base);
        /* pass */
        Sema_finalize_scope_drops(self, h_we_body);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        HirExpr* h_we_else = Sema_lower_do_value(self, we_else);
        /* pass */
        AstType* we_ty = Sema_infer_break_type(self, h_we_body);
        /* pass */
        if (_tr_str_eqv((we_ty->name), (_tr_str_lit_len("void", 4LL)))) {
            /* pass */
            we_ty = hir_expr_type(h_we_else);
        }
        /* pass */
        HirBlock* h_we_else_b = HirBlock_init();
        /* pass */
        HirBlock_push(h_we_else_b, box_hirstmt(HirStmt_ctor_SExpr(h_we_else)));
        /* pass */
        return box_hirexpr(HirExpr_ctor_EWhileExpr(h_we_cond, h_we_body, h_we_else_b, we_ty));
    } else if (_t575.tag == Expr_ETryExpr) {
        __auto_type inner = _t575.data.ETryExpr.expr;
        /* pass */
        HirExpr* hinner = Sema_lower_expr(self, inner);
        /* pass */
        AstType* inner_ty = hir_expr_type(hinner);
        /* pass */
        AstType* ok_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        if ((_tr_str_eqv((inner_ty->name), (_tr_str_lit_len("Result", 6LL))) && (inner_ty->args->len > 0LL))) {
            /* pass */
            ok_ty = (*((AstType**)List_ptr_get(inner_ty->args, 0LL)));
        } else if ((!_tr_str_eqv((inner_ty->name), (_tr_str_lit_len("void", 4LL))))) {
            /* pass */
            ok_ty = inner_ty;
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ETryExpr(hinner, ok_ty));
    } else if (_t575.tag == Expr_EAwait) {
        __auto_type inner_await = _t575.data.EAwait.expr;
        /* pass */
        if ((!self->in_async_fn)) {
            /* pass */
            ({ TrStr _at_t657 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[C-4] 'await' used outside an async function. FIX: Declare '", 60LL)), (self->current_func_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' as 'async def ", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (self->current_func_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(...)' to use await inside it.", 30LL))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t657); _tr_str_release(_at_t657); });
        }
        /* pass */
        HirExpr* hinner_await = Sema_lower_expr(self, inner_await);
        /* pass */
        AstType* await_ty = hir_expr_type(hinner_await);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EAwait(hinner_await, await_ty));
    } else if (_t575.tag == Expr_EDict) {
        __auto_type keys = _t575.data.EDict.keys;
__auto_type vals = _t575.data.EDict.vals;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit_len("a dict/map literal", 18LL), _tr_str_lit_len("use parallel fixed-size arrays [K; N]/[V; N], or a static lookup table", 70LL));
        /* pass */
        List_ptr* h_keys = (void*)List_ptr_new();
        /* pass */
        List_ptr* h_vals = (void*)List_ptr_new();
        /* pass */
        long long di = 0LL;
        /* pass */
        while ((di < keys->len)) {
            /* pass */
            List_ptr_append(h_keys, Sema_lower_expr(self, ((Expr*)List_ptr_get(keys, di))));
            /* pass */
            List_ptr_append(h_vals, Sema_lower_expr(self, ((Expr*)List_ptr_get(vals, di))));
            /* pass */
            di = (di + 1LL);
        }
        /* pass */
        AstType* dict_ty = AstType_init(_tr_str_lit_len("Dict", 4LL));
        /* pass */
        if ((h_keys->len > 0LL)) {
            /* pass */
            List_ptr_append(dict_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(h_keys, 0LL)))));
            /* pass */
            List_ptr_append(dict_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(h_vals, 0LL)))));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EDict(h_keys, h_vals, dict_ty));
    } else if (_t575.tag == Expr_EListComp) {
        __auto_type element = _t575.data.EListComp.element;
__auto_type generators = _t575.data.EListComp.generators;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit_len("a list comprehension", 20LL), _tr_str_lit_len("loop over a fixed-size array [T; N] and write into another", 58LL));
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_ptr* hgens = (void*)List_ptr_new();
        /* pass */
        long long gc = 0LL;
        /* pass */
        while ((gc < generators->len)) {
            /* pass */
            Comprehension* gen_ast = (*((Comprehension**)List_ptr_get(generators, gc)));
            /* pass */
            HirComprehension** hgen_ptr = (HirComprehension**)(0LL);
            /* pass */
            /* unsafe block */
            /* pass */
            hgen_ptr = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
            /* pass */
            HirComprehension* hgen_val = ((HirComprehension*)_tr_obj_alloc(sizeof(HirComprehension)));
            /* pass */
            hgen_val->target = _tr_str_retain(gen_ast->target);
            /* pass */
            HirExpr* h_iter_lc = Sema_lower_expr(self, gen_ast->iter);
            /* pass */
            hgen_val->iter = h_iter_lc;
            /* pass */
            TrStr lc_itn = _tr_str_retain(hir_expr_type(h_iter_lc)->name);
            /* pass */
            long long lc_ial = hir_expr_type(h_iter_lc)->args->len;
            /* pass */
            AstType* lc_elem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            /* pass */
            if (((_tr_str_eqv((lc_itn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((lc_itn), (_tr_str_lit_len("Vec", 3LL)))) && (lc_ial > 0LL))) {
                /* pass */
                lc_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_lc)->args, 0LL)));
            } else if (_tr_str_eqv((lc_itn), (_tr_str_lit_len("str", 3LL)))) {
                /* pass */
                lc_elem_ty = AstType_init(_tr_str_lit_len("char", 4LL));
            }
            /* pass */
            Sema_declare(self, gen_ast->target, SymbolKind_make_SVariable(), box_asttype(lc_elem_ty), false);
            /* pass */
            List_ptr* hifs = (void*)List_ptr_new();
            /* pass */
            long long lc_fi = 0LL;
            /* pass */
            while ((lc_fi < gen_ast->ifs->len)) {
                /* pass */
                List_ptr_append(hifs, Sema_lower_expr(self, ((Expr*)List_ptr_get(gen_ast->ifs, lc_fi))));
                /* pass */
                lc_fi = (lc_fi + 1LL);
            }
            /* pass */
            hgen_val->ifs = hifs;
            /* pass */
            hgen_val->is_async = gen_ast->is_async;
            /* pass */
            /* unsafe block */
            /* pass */
            (*hgen_ptr = hgen_val);
            /* pass */
            List_ptr_append(hgens, hgen_ptr);
            /* pass */
            gc = (gc + 1LL);
            _tr_str_release(lc_itn);
        }
        /* pass */
        HirExpr* h_lc_elem = Sema_lower_expr(self, element);
        /* pass */
        AstType* lc_elem_hty = hir_expr_type(h_lc_elem);
        /* pass */
        AstType* comp_ty = AstType_init(_tr_str_lit_len("List", 4LL));
        /* pass */
        comp_ty->args = (void*)List_ptr_new();
        /* pass */
        List_ptr_append(comp_ty->args, box_asttype(lc_elem_hty));
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EListComp(h_lc_elem, hgens, comp_ty));
    } else if (_t575.tag == Expr_EGeneratorExpr) {
        __auto_type element = _t575.data.EGeneratorExpr.element;
__auto_type generators = _t575.data.EGeneratorExpr.generators;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_ptr* hgens2 = (void*)List_ptr_new();
        /* pass */
        long long gc2 = 0LL;
        /* pass */
        while ((gc2 < generators->len)) {
            /* pass */
            Comprehension* gen_ast2 = (*((Comprehension**)List_ptr_get(generators, gc2)));
            /* pass */
            HirComprehension** hgen_ptr2 = (HirComprehension**)(0LL);
            /* pass */
            /* unsafe block */
            /* pass */
            hgen_ptr2 = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
            /* pass */
            HirComprehension* hgen_val2 = ((HirComprehension*)_tr_obj_alloc(sizeof(HirComprehension)));
            /* pass */
            hgen_val2->target = _tr_str_retain(gen_ast2->target);
            /* pass */
            HirExpr* h_iter_ge = Sema_lower_expr(self, gen_ast2->iter);
            /* pass */
            hgen_val2->iter = h_iter_ge;
            /* pass */
            TrStr ge_itn = _tr_str_retain(hir_expr_type(h_iter_ge)->name);
            /* pass */
            long long ge_ial = hir_expr_type(h_iter_ge)->args->len;
            /* pass */
            AstType* ge_elem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            /* pass */
            if (((_tr_str_eqv((ge_itn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ge_itn), (_tr_str_lit_len("Vec", 3LL)))) && (ge_ial > 0LL))) {
                /* pass */
                ge_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_ge)->args, 0LL)));
            } else if (_tr_str_eqv((ge_itn), (_tr_str_lit_len("str", 3LL)))) {
                /* pass */
                ge_elem_ty = AstType_init(_tr_str_lit_len("char", 4LL));
            }
            /* pass */
            Sema_declare(self, gen_ast2->target, SymbolKind_make_SVariable(), box_asttype(ge_elem_ty), false);
            /* pass */
            List_ptr* hifs2 = (void*)List_ptr_new();
            /* pass */
            long long ge_fi = 0LL;
            /* pass */
            while ((ge_fi < gen_ast2->ifs->len)) {
                /* pass */
                List_ptr_append(hifs2, Sema_lower_expr(self, ((Expr*)List_ptr_get(gen_ast2->ifs, ge_fi))));
                /* pass */
                ge_fi = (ge_fi + 1LL);
            }
            /* pass */
            hgen_val2->ifs = hifs2;
            /* pass */
            hgen_val2->is_async = gen_ast2->is_async;
            /* pass */
            /* unsafe block */
            /* pass */
            (*hgen_ptr2 = hgen_val2);
            /* pass */
            List_ptr_append(hgens2, hgen_ptr2);
            /* pass */
            gc2 = (gc2 + 1LL);
            _tr_str_release(ge_itn);
        }
        /* pass */
        HirExpr* h_ge_elem = Sema_lower_expr(self, element);
        /* pass */
        AstType* ge_elem_hty = hir_expr_type(h_ge_elem);
        /* pass */
        AstType* gen_ty = AstType_init(_tr_str_lit_len("List", 4LL));
        /* pass */
        gen_ty->args = (void*)List_ptr_new();
        /* pass */
        List_ptr_append(gen_ty->args, box_asttype(ge_elem_hty));
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EGeneratorExpr(h_ge_elem, hgens2, gen_ty));
    } else if (_t575.tag == Expr_ESuperMethodCall) {
        __auto_type base_class = _t575.data.ESuperMethodCall.base_class;
__auto_type method = _t575.data.ESuperMethodCall.method;
__auto_type args = _t575.data.ESuperMethodCall.args;
        /* pass */
        List_ptr* h_super_args = (void*)List_ptr_new();
        /* pass */
        long long k_smc = 0LL;
        /* pass */
        while ((k_smc < args->len)) {
            /* pass */
            List_ptr_append(h_super_args, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k_smc))));
            /* pass */
            k_smc = (k_smc + 1LL);
        }
        /* pass */
        TrStr resolved_base = _tr_str_retain(base_class);
        /* pass */
        if ((_tr_str_eqv((resolved_base), (_tr_str_lit_len("", 0LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
                /* pass */
                ClassDef* cur_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
                /* pass */
                if ((cur_cls->base_classes->len > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t658 = List_TrStr_get(cur_cls->base_classes, 0LL);
                    _tr_str_release(resolved_base);
                    resolved_base = _strtmp_t658;
                }
            }
        }
        /* pass */
        AstType* super_ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        if (_tr_dict_contains(self->classes, _tr_strz(resolved_base))) {
            /* pass */
            ClassDef* bc_def = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(resolved_base)));
            /* pass */
            long long k_smc2 = 0LL;
            /* pass */
            while ((k_smc2 < bc_def->methods->len)) {
                /* pass */
                if (_tr_str_eqv((((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->name), (method))) {
                    /* pass */
                    if ((((unsigned long long)(((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        super_ret_ty = (*((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->ret_ty);
                    }
                }
                /* pass */
                k_smc2 = (k_smc2 + 1LL);
            }
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ESuperMethodCall(resolved_base, method, h_super_args, super_ret_ty));
    } else if (_t575.tag == Expr_ESuperPropAccess) {
        __auto_type base_class = _t575.data.ESuperPropAccess.base_class;
__auto_type prop = _t575.data.ESuperPropAccess.prop;
        /* pass */
        TrStr resolved_base2 = _tr_str_retain(base_class);
        /* pass */
        if ((_tr_str_eqv((resolved_base2), (_tr_str_lit_len("", 0LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
                /* pass */
                ClassDef* cur_cls2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
                /* pass */
                if ((cur_cls2->base_classes->len > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t659 = List_TrStr_get(cur_cls2->base_classes, 0LL);
                    _tr_str_release(resolved_base2);
                    resolved_base2 = _strtmp_t659;
                }
            }
        }
        /* pass */
        AstType* super_field_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        /* pass */
        if (_tr_dict_contains(self->classes, _tr_strz(resolved_base2))) {
            /* pass */
            ClassDef* bc_def2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(resolved_base2)));
            /* pass */
            long long k_spa = 0LL;
            /* pass */
            while ((k_spa < bc_def2->fields->len)) {
                /* pass */
                if (_tr_str_eqv((((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->name), (prop))) {
                    /* pass */
                    if ((((unsigned long long)(((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        super_field_ty = (*((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->ty);
                    }
                }
                /* pass */
                k_spa = (k_spa + 1LL);
            }
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ESuperPropAccess(resolved_base2, prop, super_field_ty));
    } else if (1) {
        __auto_type _ = _t575;
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init(_tr_str_lit_len("None", 4LL))));
    }
}

__attribute__((hot)) TrStr Sema_is_reserved_error(Sema* self, TrStr name) {
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("true", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("True", 4LL))))) {
        /* pass */
        return _tr_str_lit_len("built-in boolean constant 'true'", 32LL);
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("false", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("False", 5LL))))) {
        /* pass */
        return _tr_str_lit_len("built-in boolean constant 'false'", 33LL);
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("none", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("None", 4LL))))) {
        /* pass */
        return _tr_str_lit_len("built-in null constant 'none'", 29LL);
    }
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("Some", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("built-in Option constructor 'Some'", 34LL);
    }
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("Ok", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("built-in Result constructor 'Ok'", 32LL);
    }
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("Err", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("built-in Result constructor 'Err'", 33LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Sema_is_reserved_keyword(Sema* self, TrStr name) {
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("print", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("input", 5LL))))) {
        /* pass */
        return _tr_str_lit_len("built-in I/O function", 21LL);
    }
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("range", 5LL)))) {
        /* pass */
        return _tr_str_lit_len("built-in range function", 23LL);
    }
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("len", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("built-in len function", 21LL);
    }
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("type", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("built-in type function", 22LL);
    }
    /* pass */
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("sum", 3LL))))) {
        /* pass */
        return _tr_str_lit_len("built-in math function", 22LL);
    }
    /* pass */
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("int", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL))))) {
        /* pass */
        return _tr_str_lit_len("built-in primitive type", 23LL);
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Dict", 4LL))))) {
        /* pass */
        return _tr_str_lit_len("built-in container type", 23LL);
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("Option", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Result", 6LL))))) {
        /* pass */
        return _tr_str_lit_len("built-in enum type", 18LL);
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("Exception", 9LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Error", 5LL))))) {
        /* pass */
        return _tr_str_lit_len("built-in exception type", 23LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool Sema_block_returns(Sema* self, Block* b) {
    /* pass */
    if ((b->stmts->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long last_idx = (b->stmts->len - 1LL);
    /* pass */
    bool sl_going = true;
    /* pass */
    while ((last_idx > 0LL)) {
        /* pass */
        if ((!sl_going)) {
            /* pass */
            break;
        }
        /* pass */
        Stmt* sl_s = ((Stmt*)List_ptr_get(b->stmts, last_idx));
        /* pass */
        if ((((unsigned long long)(sl_s)) == ((unsigned long long)(0LL)))) {
            /* pass */
            last_idx = (last_idx - 1LL);
        } else {
            /* pass */
            bool is_sline = false;
            /* pass */
            __auto_type _t660 = (*sl_s);
            if (_t660.tag == Stmt_SLine) {
                __auto_type _ = _t660.data.SLine.n;
                is_sline = true;
            } else if (1) {
                __auto_type _ = _t660;
                /* pass */
            }
            /* pass */
            if (is_sline) {
                /* pass */
                last_idx = (last_idx - 1LL);
            } else {
                /* pass */
                sl_going = false;
            }
        }
    }
    /* pass */
    Stmt* last_s = ((Stmt*)List_ptr_get(b->stmts, last_idx));
    /* pass */
    if ((((unsigned long long)(last_s)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t661 = (*last_s);
    if (_t661.tag == Stmt_SReturn) {
        __auto_type _ = _t661.data.SReturn.val;
        return true;
    } else if (_t661.tag == Stmt_SRaise) {
        __auto_type _ = _t661.data.SRaise.val;
        return true;
    } else if (_t661.tag == Stmt_SUnsafe) {
        __auto_type body = _t661.data.SUnsafe.body;
        return Sema_block_returns(self, body);
    } else if (_t661.tag == Stmt_SIf) {
        __auto_type cond = _t661.data.SIf.cond;
__auto_type then_b = _t661.data.SIf.then_b;
__auto_type elifs = _t661.data.SIf.elifs;
__auto_type else_b = _t661.data.SIf.else_b;
        /* pass */
        if ((else_b->stmts->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        return (Sema_block_returns(self, then_b) && Sema_block_returns(self, else_b));
    } else if (_t661.tag == Stmt_SMatch) {
        __auto_type subj = _t661.data.SMatch.expr;
__auto_type arms = _t661.data.SMatch.arms;
        /* pass */
        bool has_wild = false;
        /* pass */
        bool all_ret = true;
        /* pass */
        List_TrStr* _fr_tnames = (void*)List_TrStr_new();
        /* pass */
        List_TrStr* _fr_vnames = (void*)List_TrStr_new();
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < arms->len)) {
            /* pass */
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, mi));
            /* pass */
            __auto_type _t662 = arm->pat;
            if (_t662.tag == Pattern_PWild) {
                has_wild = true;
            } else if (_t662.tag == Pattern_PBind) {
                __auto_type _ = _t662.data.PBind.name;
                has_wild = true;
            } else if (1) {
                __auto_type _ = _t662;
                Sema_pattern_covers(self, arm->pat, _fr_tnames, _fr_vnames);
            }
            /* pass */
            if ((((unsigned long long)(arm->body)) != ((unsigned long long)(0LL)))) {
                /* pass */
                if ((!Sema_block_returns(self, (*arm->body)))) {
                    /* pass */
                    all_ret = false;
                }
            }
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        if (has_wild) {
            /* pass */
            return all_ret;
        }
        /* pass */
        if ((!all_ret)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((_fr_vnames->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        TrStr _fr_ety = List_TrStr_get(_fr_tnames, 0LL);
        /* pass */
        bool _fr_same_ty = true;
        /* pass */
        long long _fr_ti = 1LL;
        /* pass */
        while ((_fr_ti < _fr_tnames->len)) {
            /* pass */
            if ((!_tr_str_eqv((List_TrStr_get(_fr_tnames, _fr_ti)), (_fr_ety)))) {
                /* pass */
                _fr_same_ty = false;
            }
            /* pass */
            _fr_ti = (_fr_ti + 1LL);
        }
        /* pass */
        if ((!_fr_same_ty)) {
            /* pass */
            _tr_str_release(_fr_ety);
            return false;
        }
        /* pass */
        if ((!_tr_dict_contains(self->enums, _tr_strz(_fr_ety)))) {
            /* pass */
            _tr_str_release(_fr_ety);
            return false;
        }
        /* pass */
        EnumDef* _fr_edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(_fr_ety)));
        /* pass */
        long long _fr_ei = 0LL;
        /* pass */
        while ((_fr_ei < _fr_edef->variants->len)) {
            /* pass */
            TrStr _fr_vn = _tr_str_retain(((VariantDef*)List_ptr_get(_fr_edef->variants, _fr_ei))->name);
            /* pass */
            bool _fr_found = false;
            /* pass */
            long long _fr_vi = 0LL;
            /* pass */
            while ((_fr_vi < _fr_vnames->len)) {
                /* pass */
                if (_tr_str_eqv((List_TrStr_get(_fr_vnames, _fr_vi)), (_fr_vn))) {
                    /* pass */
                    _fr_found = true;
                }
                /* pass */
                _fr_vi = (_fr_vi + 1LL);
            }
            /* pass */
            if ((!_fr_found)) {
                /* pass */
                _tr_str_release(_fr_ety);
                _tr_str_release(_fr_vn);
                return false;
            }
            /* pass */
            _fr_ei = (_fr_ei + 1LL);
            _tr_str_release(_fr_vn);
        }
        /* pass */
        _tr_str_release(_fr_ety);
        return true;
    } else if (1) {
        __auto_type _ = _t661;
        return false;
    }
}

__attribute__((hot)) void Sema_pattern_covers(Sema* self, Pattern p, List_TrStr* tnames, List_TrStr* vnames) {
    /* pass */
    __auto_type _t663 = p;
    if (_t663.tag == Pattern_PVariant) {
        __auto_type tn = _t663.data.PVariant.type_name;
__auto_type vn = _t663.data.PVariant.variant;
        /* pass */
        List_TrStr_append(tnames, tn);
        /* pass */
        List_TrStr_append(vnames, vn);
    } else if (_t663.tag == Pattern_PVariantBind) {
        __auto_type tn = _t663.data.PVariantBind.type_name;
__auto_type vn = _t663.data.PVariantBind.variant;
        /* pass */
        List_TrStr_append(tnames, tn);
        /* pass */
        List_TrStr_append(vnames, vn);
    } else if (_t663.tag == Pattern_PVariantBindMany) {
        __auto_type tn = _t663.data.PVariantBindMany.type_name;
__auto_type vn = _t663.data.PVariantBindMany.variant;
        /* pass */
        List_TrStr_append(tnames, tn);
        /* pass */
        List_TrStr_append(vnames, vn);
    } else if (_t663.tag == Pattern_POr) {
        __auto_type pats = _t663.data.POr.patterns;
        /* pass */
        long long oi = 0LL;
        /* pass */
        while ((oi < pats->len)) {
            /* pass */
            Sema_pattern_covers(self, List_Pattern_get(pats, oi), tnames, vnames);
            /* pass */
            oi = (oi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t663;
        /* pass */
    }
}

__attribute__((hot)) bool Sema_is_primitive(Sema* self, AstType* ty) {
    /* pass */
    return Sema_is_primitive_name(self, ty->name);
}

__attribute__((hot)) bool Sema__is_known_type_name(Sema* self, TrStr name) {
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    if (Sema_is_primitive_name(self, name)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Str", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("ptr", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("object", 6LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_strlen(_tr_strz(name)) == 1LL)) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->enums, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))) && _tr_dict_contains(self->interfaces, _tr_strz(name)))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((((_tr_str_eqv((name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Box", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("tuple", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Pointer", 7LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((((_tr_str_eqv((name), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Channel", 7LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Coro", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("AsyncPool", 9LL))) || _tr_str_eqv((name), (_tr_str_lit_len("AsyncTask", 9LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("OS", 2LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Process", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Env", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Hash", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("File", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("Math", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Clock", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Time", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Random", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Hmac", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("Json", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("JsonDoc", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("JsonWriter", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("JsonReader", 10LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_primitive_name(Sema* self, TrStr name) {
    /* pass */
    if ((((((_tr_str_eqv((name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("None", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("i64", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("i8", 2LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("u64", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("usize", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((name), (_tr_str_lit_len("f64", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("lambda", 6LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_starts_withv((name), (_tr_str_lit_len("c_", 2LL))) && (!_tr_str_eqv((name), (_tr_str_lit_len("c_void_ptr", 10LL))))) && (!_tr_str_eqv((name), (_tr_str_lit_len("c_FILE", 6LL)))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Str", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Bytes", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Channel", 7LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((name), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Atomic", 6LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("AsyncPool", 9LL))) || _tr_str_eqv((name), (_tr_str_lit_len("AsyncTask", 9LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("Pointer", 7LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("ref", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("mut_ref", 7LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _expr_is_self_field(Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t664 = (*e);
    if (_t664.tag == Expr_EPropAccess) {
        __auto_type obj = _t664.data.EPropAccess.obj;
        /* pass */
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        __auto_type _t665 = (*obj);
        if (_t665.tag == Expr_EIdent) {
            __auto_type nm = _t665.data.EIdent.name;
            return _tr_str_eqv((nm), (_tr_str_lit_len("self", 4LL)));
        } else if (1) {
            __auto_type _ = _t665;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t664;
        return false;
    }
}

__attribute__((hot)) bool _binop_is_float_name(TrStr n) {
    /* pass */
    return ((_tr_str_eqv((n), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL))));
}

__attribute__((hot)) bool _hl_has_float(List_ptr* hl) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hl->len)) {
        /* pass */
        if (_binop_is_float_name(hir_expr_type(((HirExpr*)List_ptr_get(hl, i)))->name)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _block_mutates_self(Block* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        if (_stmt_mutates_self(((Stmt*)List_ptr_get(b->stmts, i)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _pblock_mutates_self(Block** pb) {
    /* pass */
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    return _block_mutates_self((*pb));
}

__attribute__((hot)) bool _stmt_mutates_self(Stmt* s) {
    /* pass */
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t666 = (*s);
    if (_t666.tag == Stmt_SAssign) {
        __auto_type tgt = _t666.data.SAssign.target;
        return _expr_is_self_field(tgt);
    } else if (_t666.tag == Stmt_SIf) {
        __auto_type then_b = _t666.data.SIf.then_b;
__auto_type elifs = _t666.data.SIf.elifs;
__auto_type else_b = _t666.data.SIf.else_b;
        /* pass */
        if (_block_mutates_self(then_b)) {
            /* pass */
            return true;
        }
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < elifs->len)) {
            /* pass */
            if (_pblock_mutates_self(((ElifClause*)List_ptr_get(elifs, i))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return _block_mutates_self(else_b);
    } else if (_t666.tag == Stmt_SWhile) {
        __auto_type body = _t666.data.SWhile.body;
        return _block_mutates_self(body);
    } else if (_t666.tag == Stmt_SFor) {
        __auto_type body = _t666.data.SFor.body;
        return _block_mutates_self(body);
    } else if (_t666.tag == Stmt_SForUnpack) {
        __auto_type body = _t666.data.SForUnpack.body;
        return _block_mutates_self(body);
    } else if (_t666.tag == Stmt_SUnsafe) {
        __auto_type body = _t666.data.SUnsafe.body;
        return _block_mutates_self(body);
    } else if (_t666.tag == Stmt_SWith) {
        __auto_type body = _t666.data.SWith.body;
        return _block_mutates_self(body);
    } else if (_t666.tag == Stmt_STaskGroup) {
        __auto_type body = _t666.data.STaskGroup.body;
        return _block_mutates_self(body);
    } else if (_t666.tag == Stmt_SGpuBlock) {
        __auto_type body = _t666.data.SGpuBlock.body;
        return _block_mutates_self(body);
    } else if (_t666.tag == Stmt_SMatch) {
        __auto_type arms = _t666.data.SMatch.arms;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < arms->len)) {
            /* pass */
            if (_pblock_mutates_self(((MatchArm*)List_ptr_get(arms, i))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return false;
    } else if (_t666.tag == Stmt_STry) {
        __auto_type try_body = _t666.data.STry.try_body;
__auto_type catches = _t666.data.STry.catches;
__auto_type finally_b = _t666.data.STry.finally_b;
        /* pass */
        if (_block_mutates_self(try_body)) {
            /* pass */
            return true;
        }
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < catches->len)) {
            /* pass */
            if (_pblock_mutates_self((*((CatchClause**)List_ptr_get(catches, i)))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return _block_mutates_self(finally_b);
    } else if (_t666.tag == Stmt_SDefer) {
        __auto_type inner = _t666.data.SDefer.stmt;
        return _stmt_mutates_self(inner);
    } else if (1) {
        __auto_type _ = _t666;
        return false;
    }
}

__attribute__((hot)) Symbol** box_symbol(Symbol* s) {
    /* pass */
    /* unsafe block */
    /* pass */
    Symbol** p = ((Symbol**)_tr_c_calloc((size_t)(1LL), sizeof(Symbol*)));
    /* pass */
    (*p = s);
    /* pass */
    return p;
}

