#include "tauraro_types.h"

bool _expr_is_self_field(Expr* e);
bool _binop_is_float_name(TrStr n);
bool _hl_has_float(List_ptr* hl);
bool _block_mutates_self(Block* b);
bool _pblock_mutates_self(Block** pb);
bool _stmt_mutates_self(Stmt* s);
bool _rhs_is_alias(Expr* e);
void _block_alias_assigns(Block* b, TrMap* out);
void _pblock_alias_assigns(Block** pb, TrMap* out);
void _stmt_alias_assigns(Stmt* s, TrMap* out);

__attribute__((malloc,returns_nonnull,hot)) Symbol* Symbol_init(TrStr name, SymbolKind kind, AstType** ty) {
#line 54 "src/sema.tr"
    Symbol* s = ((Symbol*)_tr_obj_alloc(sizeof(Symbol)));
#line 55 "src/sema.tr"
    s->name = _tr_str_retain(name);
#line 56 "src/sema.tr"
    s->kind = kind;
#line 57 "src/sema.tr"
    s->ty = ty;
#line 58 "src/sema.tr"
    s->scope_depth = 0LL;
#line 59 "src/sema.tr"
    s->is_mut = false;
#line 60 "src/sema.tr"
    s->is_const = false;
#line 61 "src/sema.tr"
    s->is_shared = false;
#line 62 "src/sema.tr"
    s->is_moved = false;
#line 63 "src/sema.tr"
    s->active_borrows = 0LL;
#line 64 "src/sema.tr"
    s->borrowed_by = (void*)List_TrStr_new();
#line 65 "src/sema.tr"
    s->is_init = true;
#line 66 "src/sema.tr"
    s->is_param = false;
#line 67 "src/sema.tr"
    s->is_maybe_moved = false;
#line 68 "src/sema.tr"
    s->is_maybe_init = false;
#line 69 "src/sema.tr"
    s->ptr_region = 0LL;
#line 70 "src/sema.tr"
    s->is_freed = false;
#line 71 "src/sema.tr"
    s->decl_block_depth = 0LL;
#line 72 "src/sema.tr"
    s->decl_block_id = 0LL;
#line 73 "src/sema.tr"
    s->str_escaped = false;
#line 74 "src/sema.tr"
    s->coll_escaped = false;
#line 75 "src/sema.tr"
    s->borrows_region = _tr_str_lit_len("", 0LL);
#line 76 "src/sema.tr"
    s->firm = _tr_str_lit_len("", 0LL);
#line 77 "src/sema.tr"
    s->firm_cls = _tr_str_lit_len("", 0LL);
#line 78 "src/sema.tr"
    return s;
}

__attribute__((malloc,returns_nonnull,hot)) Scope* Scope_init() {
#line 86 "src/sema.tr"
    Scope* s = ((Scope*)_tr_obj_alloc(sizeof(Scope)));
#line 87 "src/sema.tr"
    s->variables = _tr_dict_new(32LL);
#line 88 "src/sema.tr"
    s->decl_order = (void*)List_TrStr_new();
#line 89 "src/sema.tr"
    return s;
}

__attribute__((hot)) AstType** Sema_build_ast_type(Sema* self, Expr* e) {
#line 319 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 319 "src/sema.tr"
        return (AstType**)(0LL);
    }
#line 320 "src/sema.tr"
    __auto_type _t686 = (*e);
    if (_t686.tag == Expr_EIdent) {
        __auto_type n = _t686.data.EIdent.name;
#line 322 "src/sema.tr"
        AstType** _trret175 = box_asttype(AstType_init(n));
        return _trret175;
    } else if (_t686.tag == Expr_EIndex) {
        __auto_type obj = _t686.data.EIndex.obj;
__auto_type idx = _t686.data.EIndex._tr_v_index;
#line 324 "src/sema.tr"
        __auto_type _t687 = (*obj);
        if (_t687.tag == Expr_EIdent) {
            __auto_type on = _t687.data.EIdent.name;
#line 326 "src/sema.tr"
            AstType* at = AstType_init(on);
#line 327 "src/sema.tr"
            at->args = (void*)List_ptr_new();
#line 331 "src/sema.tr"
            __auto_type _t688 = (*idx);
            if (_t688.tag == Expr_ETuple) {
                __auto_type _bte = _t688.data.ETuple.items;
#line 333 "src/sema.tr"
                long long _bti = 0LL;
#line 334 "src/sema.tr"
                while ((_bti < _bte->len)) {
#line 335 "src/sema.tr"
                    ({ __auto_type _ord_t689 = (Sema_build_ast_type(self, ((Expr*)List_ptr_get(_bte, _bti)))); List_ptr_append(at->args, _ord_t689); });
#line 336 "src/sema.tr"
                    _bti = (_bti + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t688;
#line 338 "src/sema.tr"
                ({ __auto_type _ord_t690 = (Sema_build_ast_type(self, idx)); List_ptr_append(at->args, _ord_t690); });
            }
#line 339 "src/sema.tr"
            AstType** _trret176 = box_asttype(at);
            return _trret176;
        } else if (1) {
            __auto_type _ = _t687;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t686;
        /* pass */
    }
#line 342 "src/sema.tr"
    return box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
}

__attribute__((hot)) AstType** Sema__targ_of(Sema* self, Expr* e) {
#line 347 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 347 "src/sema.tr"
        return box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
    }
#line 348 "src/sema.tr"
    __auto_type _t691 = (*e);
    if (_t691.tag == Expr_EIdent) {
        __auto_type n = _t691.data.EIdent.name;
        AstType** _trret177 = box_asttype(AstType_init(n));
        return _trret177;
    } else if (_t691.tag == Expr_EIndex) {
        return Sema_build_ast_type(self, e);
    } else if (1) {
        __auto_type _ = _t691;
        return box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
    }
}

__attribute__((malloc,returns_nonnull,hot)) Sema* Sema_init() {
#line 354 "src/sema.tr"
    Sema* s = ((Sema*)_tr_obj_alloc(sizeof(Sema)));
#line 355 "src/sema.tr"
    s->globals = _tr_dict_new(1024LL);
#line 356 "src/sema.tr"
    s->scopes = (void*)List_ptr_new();
#line 357 "src/sema.tr"
    s->errors = (void*)List_TrStr_new();
#line 358 "src/sema.tr"
    s->warnings = (void*)List_TrStr_new();
#line 359 "src/sema.tr"
    s->classes = _tr_dict_new(128LL);
#line 360 "src/sema.tr"
    s->enums = _tr_dict_new(64LL);
#line 361 "src/sema.tr"
    s->interfaces = _tr_dict_new(32LL);
#line 362 "src/sema.tr"
    s->type_aliases = _tr_dict_new(32LL);
#line 363 "src/sema.tr"
    s->type_alias_elem = _tr_dict_new(32LL);
#line 364 "src/sema.tr"
    s->current_file = _tr_str_lit_len("", 0LL);
#line 365 "src/sema.tr"
    s->current_func_name = _tr_str_lit_len("", 0LL);
#line 366 "src/sema.tr"
    s->current_class_name = _tr_str_lit_len("", 0LL);
#line 367 "src/sema.tr"
    s->current_scope_depth = 0LL;
#line 368 "src/sema.tr"
    s->in_async_fn = false;
#line 369 "src/sema.tr"
    s->fn_try_ok = true;
#line 370 "src/sema.tr"
    s->closure_depth = 0LL;
#line 371 "src/sema.tr"
    s->assign_froms = _tr_dict_new(32LL);
#line 372 "src/sema.tr"
    s->fn_sigs = _tr_dict_new(64LL);
#line 373 "src/sema.tr"
    s->closure_hint = (AstType**)(0LL);
#line 374 "src/sema.tr"
    s->list_hint = (AstType**)(0LL);
#line 375 "src/sema.tr"
    s->extern_names = _tr_dict_new(128LL);
#line 376 "src/sema.tr"
    s->nested_classes = (void*)List_ptr_new();
#line 377 "src/sema.tr"
    s->nested_functions = (void*)List_ptr_new();
#line 378 "src/sema.tr"
    s->nested_enums = (void*)List_ptr_new();
#line 379 "src/sema.tr"
    s->nested_interfaces = (void*)List_ptr_new();
#line 380 "src/sema.tr"
    s->current_line = 0LL;
#line 381 "src/sema.tr"
    s->current_func_generics = (void*)List_TrStr_new();
#line 382 "src/sema.tr"
    s->current_func_constraints = (void*)List_ptr_new();
#line 383 "src/sema.tr"
    s->closure_boundary = (-1LL);
#line 384 "src/sema.tr"
    s->closure_caps = (void*)List_ptr_new();
#line 385 "src/sema.tr"
    s->closure_cap_set = _tr_dict_new(0LL);
#line 386 "src/sema.tr"
    s->in_assign_target = false;
#line 387 "src/sema.tr"
    s->in_assign_target_plain = false;
#line 388 "src/sema.tr"
    s->in_recv_pos = false;
#line 389 "src/sema.tr"
    s->container_borrows = _tr_dict_new(16LL);
#line 390 "src/sema.tr"
    s->alias_assigned = _tr_dict_new(8LL);
#line 391 "src/sema.tr"
    s->sendable_tparams = (void*)List_TrStr_new();
#line 392 "src/sema.tr"
    s->capturing_moves = false;
#line 393 "src/sema.tr"
    s->branch_moved_buf = (void*)List_TrStr_new();
#line 394 "src/sema.tr"
    s->capturing_inits = false;
#line 395 "src/sema.tr"
    s->branch_init_buf = (void*)List_TrStr_new();
#line 396 "src/sema.tr"
    s->copy_classes = _tr_dict_new(32LL);
#line 397 "src/sema.tr"
    s->in_unsafe = false;
#line 398 "src/sema.tr"
    s->cur_fn_is_lib = false;
#line 399 "src/sema.tr"
    s->current_func_ret_from = _tr_str_lit_len("", 0LL);
#line 400 "src/sema.tr"
    s->current_func_ret_borrow_str = false;
#line 401 "src/sema.tr"
    s->current_func_ret_family = _tr_str_lit_len("", 0LL);
#line 402 "src/sema.tr"
    s->ambiguous_classes = _tr_dict_new(8LL);
#line 403 "src/sema.tr"
    s->extern_defs = _tr_dict_new(16LL);
#line 404 "src/sema.tr"
    s->global_firm = _tr_dict_new(16LL);
#line 405 "src/sema.tr"
    s->current_func_ret_regions = (void*)List_TrStr_new();
#line 406 "src/sema.tr"
    s->current_func_outlives_a = (void*)List_TrStr_new();
#line 407 "src/sema.tr"
    s->current_func_outlives_b = (void*)List_TrStr_new();
#line 408 "src/sema.tr"
    s->current_region_params = (void*)List_TrStr_new();
#line 409 "src/sema.tr"
    s->cur_func_borrowers = (void*)List_TrStr_new();
#line 410 "src/sema.tr"
    s->cur_func_sources = (void*)List_TrStr_new();
#line 411 "src/sema.tr"
    s->strict_mode = false;
#line 412 "src/sema.tr"
    s->no_heap = false;
#line 413 "src/sema.tr"
    s->heap_boxes_tuples = false;
#line 414 "src/sema.tr"
    s->mutating_methods = _tr_dict_new(32LL);
#line 415 "src/sema.tr"
    s->fn_ret_owned = _tr_dict_new(64LL);
#line 416 "src/sema.tr"
    s->fn_param_consumes = _tr_dict_new(64LL);
#line 417 "src/sema.tr"
    s->ptr_aliased = _tr_dict_new(32LL);
#line 418 "src/sema.tr"
    s->decorator_names = _tr_dict_new(16LL);
#line 419 "src/sema.tr"
    s->variadic_fns = _tr_dict_new(8LL);
#line 420 "src/sema.tr"
    s->variadic_elem_ty = _tr_dict_new(8LL);
#line 421 "src/sema.tr"
    s->fn_defs = _tr_dict_new(32LL);
#line 422 "src/sema.tr"
    s->loop_scope_base = (void*)List_i64_new();
#line 423 "src/sema.tr"
    s->loop_blk_floor = (void*)List_i64_new();
#line 424 "src/sema.tr"
    s->clo_let_names = (void*)List_TrStr_new();
#line 425 "src/sema.tr"
    s->clo_let_ptrs = (void*)List_ptr_new();
#line 426 "src/sema.tr"
    s->clo_let_scopes = (void*)List_i64_new();
#line 427 "src/sema.tr"
    s->loop_blk_scope = (void*)List_i64_new();
#line 428 "src/sema.tr"
    s->fn_scope_base = (void*)List_i64_new();
#line 429 "src/sema.tr"
    s->block_depth = 0LL;
#line 430 "src/sema.tr"
    s->block_depth_stack = (void*)List_i64_new();
#line 431 "src/sema.tr"
    s->next_block_id = 0LL;
#line 432 "src/sema.tr"
    s->block_stack = (void*)List_i64_new();
#line 433 "src/sema.tr"
    s->shadow_names = (void*)List_TrStr_new();
#line 434 "src/sema.tr"
    s->shadow_prevs = (void*)List_ptr_new();
#line 435 "src/sema.tr"
    s->shadow_blocks = (void*)List_i64_new();
#line 436 "src/sema.tr"
    s->shadow_scopes = (void*)List_i64_new();
#line 437 "src/sema.tr"
    s->block_stack_base = (void*)List_i64_new();
#line 438 "src/sema.tr"
    s->do_temp_ctr = 0LL;
#line 439 "src/sema.tr"
    s->pc_alias_consumes = false;
#line 442 "src/sema.tr"
    ({ __auto_type _ord_t692 = (SymbolKind_make_SFunction()); __auto_type _ord_t693 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t694 = (Symbol_init(_tr_str_lit_len("print", 5LL), _ord_t692, _ord_t693)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("print", 5LL)), _ord_t694); });
#line 443 "src/sema.tr"
    ({ __auto_type _ord_t695 = (SymbolKind_make_SFunction()); __auto_type _ord_t696 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t697 = (Symbol_init(_tr_str_lit_len("input", 5LL), _ord_t695, _ord_t696)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("input", 5LL)), _ord_t697); });
#line 444 "src/sema.tr"
    ({ __auto_type _ord_t698 = (SymbolKind_make_SFunction()); __auto_type _ord_t699 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t700 = (Symbol_init(_tr_str_lit_len("len", 3LL), _ord_t698, _ord_t699)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("len", 3LL)), _ord_t700); });
#line 445 "src/sema.tr"
    AstType* _range_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 446 "src/sema.tr"
    ({ __auto_type _ord_t701 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); List_ptr_append(_range_ty->args, _ord_t701); });
#line 447 "src/sema.tr"
    ({ __auto_type _ord_t702 = (SymbolKind_make_SFunction()); __auto_type _ord_t703 = (box_asttype(_range_ty)); __auto_type _ord_t704 = (Symbol_init(_tr_str_lit_len("range", 5LL), _ord_t702, _ord_t703)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("range", 5LL)), _ord_t704); });
#line 448 "src/sema.tr"
    ({ __auto_type _ord_t705 = (SymbolKind_make_SFunction()); __auto_type _ord_t706 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t707 = (Symbol_init(_tr_str_lit_len("type", 4LL), _ord_t705, _ord_t706)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("type", 4LL)), _ord_t707); });
#line 449 "src/sema.tr"
    ({ __auto_type _ord_t708 = (SymbolKind_make_SFunction()); __auto_type _ord_t709 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t710 = (Symbol_init(_tr_str_lit_len("str", 3LL), _ord_t708, _ord_t709)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("str", 3LL)), _ord_t710); });
#line 450 "src/sema.tr"
    ({ __auto_type _ord_t711 = (SymbolKind_make_SFunction()); __auto_type _ord_t712 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t713 = (Symbol_init(_tr_str_lit_len("hex", 3LL), _ord_t711, _ord_t712)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("hex", 3LL)), _ord_t713); });
#line 451 "src/sema.tr"
    ({ __auto_type _ord_t714 = (SymbolKind_make_SFunction()); __auto_type _ord_t715 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t716 = (Symbol_init(_tr_str_lit_len("oct", 3LL), _ord_t714, _ord_t715)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("oct", 3LL)), _ord_t716); });
#line 452 "src/sema.tr"
    ({ __auto_type _ord_t717 = (SymbolKind_make_SFunction()); __auto_type _ord_t718 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t719 = (Symbol_init(_tr_str_lit_len("bin", 3LL), _ord_t717, _ord_t718)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("bin", 3LL)), _ord_t719); });
#line 456 "src/sema.tr"
    ({ __auto_type _ord_t720 = (SymbolKind_make_SFunction()); __auto_type _ord_t721 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t722 = (Symbol_init(_tr_str_lit_len("kernel_launch", 13LL), _ord_t720, _ord_t721)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("kernel_launch", 13LL)), _ord_t722); });
#line 457 "src/sema.tr"
    ({ __auto_type _ord_t723 = (SymbolKind_make_SFunction()); __auto_type _ord_t724 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t725 = (Symbol_init(_tr_str_lit_len("int", 3LL), _ord_t723, _ord_t724)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("int", 3LL)), _ord_t725); });
#line 458 "src/sema.tr"
    ({ __auto_type _ord_t726 = (SymbolKind_make_SFunction()); __auto_type _ord_t727 = (box_asttype(AstType_init(_tr_str_lit_len("float", 5LL)))); __auto_type _ord_t728 = (Symbol_init(_tr_str_lit_len("float", 5LL), _ord_t726, _ord_t727)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("float", 5LL)), _ord_t728); });
#line 459 "src/sema.tr"
    ({ __auto_type _ord_t729 = (SymbolKind_make_SFunction()); __auto_type _ord_t730 = (box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))); __auto_type _ord_t731 = (Symbol_init(_tr_str_lit_len("bool", 4LL), _ord_t729, _ord_t730)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("bool", 4LL)), _ord_t731); });
#line 460 "src/sema.tr"
    ({ __auto_type _ord_t732 = (SymbolKind_make_SFunction()); __auto_type _ord_t733 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t734 = (Symbol_init(_tr_str_lit_len("ord", 3LL), _ord_t732, _ord_t733)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ord", 3LL)), _ord_t734); });
#line 461 "src/sema.tr"
    ({ __auto_type _ord_t735 = (SymbolKind_make_SFunction()); __auto_type _ord_t736 = (box_asttype(AstType_init(_tr_str_lit_len("char", 4LL)))); __auto_type _ord_t737 = (Symbol_init(_tr_str_lit_len("chr", 3LL), _ord_t735, _ord_t736)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("chr", 3LL)), _ord_t737); });
#line 462 "src/sema.tr"
    ({ __auto_type _ord_t738 = (SymbolKind_make_SFunction()); __auto_type _ord_t739 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t740 = (Symbol_init(_tr_str_lit_len("abs", 3LL), _ord_t738, _ord_t739)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("abs", 3LL)), _ord_t740); });
#line 463 "src/sema.tr"
    ({ __auto_type _ord_t741 = (SymbolKind_make_SFunction()); __auto_type _ord_t742 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t743 = (Symbol_init(_tr_str_lit_len("max", 3LL), _ord_t741, _ord_t742)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("max", 3LL)), _ord_t743); });
#line 464 "src/sema.tr"
    ({ __auto_type _ord_t744 = (SymbolKind_make_SFunction()); __auto_type _ord_t745 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t746 = (Symbol_init(_tr_str_lit_len("min", 3LL), _ord_t744, _ord_t745)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("min", 3LL)), _ord_t746); });
#line 465 "src/sema.tr"
    ({ __auto_type _ord_t747 = (SymbolKind_make_SFunction()); __auto_type _ord_t748 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t749 = (Symbol_init(_tr_str_lit_len("sum", 3LL), _ord_t747, _ord_t748)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("sum", 3LL)), _ord_t749); });
#line 466 "src/sema.tr"
    ({ __auto_type _ord_t750 = (SymbolKind_make_SFunction()); __auto_type _ord_t751 = (box_asttype(AstType_init(_tr_str_lit_len("float", 5LL)))); __auto_type _ord_t752 = (Symbol_init(_tr_str_lit_len("round", 5LL), _ord_t750, _ord_t751)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("round", 5LL)), _ord_t752); });
#line 467 "src/sema.tr"
    ({ __auto_type _ord_t753 = (SymbolKind_make_SFunction()); __auto_type _ord_t754 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t755 = (Symbol_init(_tr_str_lit_len("sorted", 6LL), _ord_t753, _ord_t754)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("sorted", 6LL)), _ord_t755); });
#line 468 "src/sema.tr"
    ({ __auto_type _ord_t756 = (SymbolKind_make_SFunction()); __auto_type _ord_t757 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t758 = (Symbol_init(_tr_str_lit_len("reversed", 8LL), _ord_t756, _ord_t757)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("reversed", 8LL)), _ord_t758); });
#line 469 "src/sema.tr"
    ({ __auto_type _ord_t759 = (SymbolKind_make_SFunction()); __auto_type _ord_t760 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t761 = (Symbol_init(_tr_str_lit_len("iter", 4LL), _ord_t759, _ord_t760)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("iter", 4LL)), _ord_t761); });
#line 470 "src/sema.tr"
    ({ __auto_type _ord_t762 = (SymbolKind_make_SFunction()); __auto_type _ord_t763 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t764 = (Symbol_init(_tr_str_lit_len("enumerate", 9LL), _ord_t762, _ord_t763)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("enumerate", 9LL)), _ord_t764); });
#line 471 "src/sema.tr"
    ({ __auto_type _ord_t765 = (SymbolKind_make_SFunction()); __auto_type _ord_t766 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t767 = (Symbol_init(_tr_str_lit_len("zip", 3LL), _ord_t765, _ord_t766)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("zip", 3LL)), _ord_t767); });
#line 472 "src/sema.tr"
    ({ __auto_type _ord_t768 = (SymbolKind_make_SFunction()); __auto_type _ord_t769 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t770 = (Symbol_init(_tr_str_lit_len("map", 3LL), _ord_t768, _ord_t769)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("map", 3LL)), _ord_t770); });
#line 473 "src/sema.tr"
    ({ __auto_type _ord_t771 = (SymbolKind_make_SFunction()); __auto_type _ord_t772 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t773 = (Symbol_init(_tr_str_lit_len("filter", 6LL), _ord_t771, _ord_t772)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("filter", 6LL)), _ord_t773); });
#line 474 "src/sema.tr"
    ({ __auto_type _ord_t774 = (SymbolKind_make_SFunction()); __auto_type _ord_t775 = (box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))); __auto_type _ord_t776 = (Symbol_init(_tr_str_lit_len("all", 3LL), _ord_t774, _ord_t775)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("all", 3LL)), _ord_t776); });
#line 475 "src/sema.tr"
    ({ __auto_type _ord_t777 = (SymbolKind_make_SFunction()); __auto_type _ord_t778 = (box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))); __auto_type _ord_t779 = (Symbol_init(_tr_str_lit_len("any", 3LL), _ord_t777, _ord_t778)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("any", 3LL)), _ord_t779); });
#line 476 "src/sema.tr"
    ({ __auto_type _ord_t780 = (SymbolKind_make_SFunction()); __auto_type _ord_t781 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t782 = (Symbol_init(_tr_str_lit_len("assert", 6LL), _ord_t780, _ord_t781)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert", 6LL)), _ord_t782); });
#line 477 "src/sema.tr"
    ({ __auto_type _ord_t783 = (SymbolKind_make_SFunction()); __auto_type _ord_t784 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t785 = (Symbol_init(_tr_str_lit_len("assert_eq", 9LL), _ord_t783, _ord_t784)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_eq", 9LL)), _ord_t785); });
#line 478 "src/sema.tr"
    ({ __auto_type _ord_t786 = (SymbolKind_make_SFunction()); __auto_type _ord_t787 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t788 = (Symbol_init(_tr_str_lit_len("assert_ne", 9LL), _ord_t786, _ord_t787)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_ne", 9LL)), _ord_t788); });
#line 479 "src/sema.tr"
    ({ __auto_type _ord_t789 = (SymbolKind_make_SFunction()); __auto_type _ord_t790 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t791 = (Symbol_init(_tr_str_lit_len("assert_lt", 9LL), _ord_t789, _ord_t790)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_lt", 9LL)), _ord_t791); });
#line 480 "src/sema.tr"
    ({ __auto_type _ord_t792 = (SymbolKind_make_SFunction()); __auto_type _ord_t793 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t794 = (Symbol_init(_tr_str_lit_len("assert_le", 9LL), _ord_t792, _ord_t793)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_le", 9LL)), _ord_t794); });
#line 481 "src/sema.tr"
    ({ __auto_type _ord_t795 = (SymbolKind_make_SFunction()); __auto_type _ord_t796 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t797 = (Symbol_init(_tr_str_lit_len("assert_gt", 9LL), _ord_t795, _ord_t796)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_gt", 9LL)), _ord_t797); });
#line 482 "src/sema.tr"
    ({ __auto_type _ord_t798 = (SymbolKind_make_SFunction()); __auto_type _ord_t799 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t800 = (Symbol_init(_tr_str_lit_len("assert_ge", 9LL), _ord_t798, _ord_t799)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_ge", 9LL)), _ord_t800); });
#line 483 "src/sema.tr"
    ({ __auto_type _ord_t801 = (SymbolKind_make_SFunction()); __auto_type _ord_t802 = (box_asttype(AstType_init(_tr_str_lit_len("Option", 6LL)))); __auto_type _ord_t803 = (Symbol_init(_tr_str_lit_len("Some", 4LL), _ord_t801, _ord_t802)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Some", 4LL)), _ord_t803); });
#line 484 "src/sema.tr"
    ({ __auto_type _ord_t804 = (SymbolKind_make_SFunction()); __auto_type _ord_t805 = (box_asttype(AstType_init(_tr_str_lit_len("Result", 6LL)))); __auto_type _ord_t806 = (Symbol_init(_tr_str_lit_len("Ok", 2LL), _ord_t804, _ord_t805)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Ok", 2LL)), _ord_t806); });
#line 485 "src/sema.tr"
    ({ __auto_type _ord_t807 = (SymbolKind_make_SFunction()); __auto_type _ord_t808 = (box_asttype(AstType_init(_tr_str_lit_len("Result", 6LL)))); __auto_type _ord_t809 = (Symbol_init(_tr_str_lit_len("Err", 3LL), _ord_t807, _ord_t808)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Err", 3LL)), _ord_t809); });
#line 486 "src/sema.tr"
    ({ __auto_type _ord_t810 = (SymbolKind_make_SFunction()); __auto_type _ord_t811 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t812 = (Symbol_init(_tr_str_lit_len("_tr_exit", 8LL), _ord_t810, _ord_t811)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("_tr_exit", 8LL)), _ord_t812); });
#line 487 "src/sema.tr"
    ({ __auto_type _ord_t813 = (SymbolKind_make_SFunction()); __auto_type _ord_t814 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t815 = (Symbol_init(_tr_str_lit_len("_tr_strlen", 10LL), _ord_t813, _ord_t814)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("_tr_strlen", 10LL)), _ord_t815); });
#line 488 "src/sema.tr"
    ({ __auto_type _ord_t816 = (SymbolKind_make_SFunction()); __auto_type _ord_t817 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t818 = (Symbol_init(_tr_str_lit_len("read_file", 9LL), _ord_t816, _ord_t817)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("read_file", 9LL)), _ord_t818); });
#line 489 "src/sema.tr"
    ({ __auto_type _ord_t819 = (SymbolKind_make_SFunction()); __auto_type _ord_t820 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t821 = (Symbol_init(_tr_str_lit_len("write_file", 10LL), _ord_t819, _ord_t820)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("write_file", 10LL)), _ord_t821); });
#line 490 "src/sema.tr"
    ({ __auto_type _ord_t822 = (SymbolKind_make_SFunction()); __auto_type _ord_t823 = (box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))); __auto_type _ord_t824 = (Symbol_init(_tr_str_lit_len("file_exists", 11LL), _ord_t822, _ord_t823)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("file_exists", 11LL)), _ord_t824); });
#line 491 "src/sema.tr"
    ({ __auto_type _ord_t825 = (SymbolKind_make_SFunction()); __auto_type _ord_t826 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t827 = (Symbol_init(_tr_str_lit_len("_tr_system", 10LL), _ord_t825, _ord_t826)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("_tr_system", 10LL)), _ord_t827); });
#line 496 "src/sema.tr"
    ({ __auto_type _ord_t828 = (SymbolKind_make_SClass()); __auto_type _ord_t829 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t830 = (Symbol_init(_tr_str_lit_len("List", 4LL), _ord_t828, _ord_t829)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("List", 4LL)), _ord_t830); });
#line 497 "src/sema.tr"
    ({ __auto_type _ord_t831 = (SymbolKind_make_SClass()); __auto_type _ord_t832 = (box_asttype(AstType_init(_tr_str_lit_len("Pointer", 7LL)))); __auto_type _ord_t833 = (Symbol_init(_tr_str_lit_len("Pointer", 7LL), _ord_t831, _ord_t832)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Pointer", 7LL)), _ord_t833); });
#line 498 "src/sema.tr"
    ({ __auto_type _ord_t834 = (SymbolKind_make_SClass()); __auto_type _ord_t835 = (box_asttype(AstType_init(_tr_str_lit_len("StringBuilder", 13LL)))); __auto_type _ord_t836 = (Symbol_init(_tr_str_lit_len("StringBuilder", 13LL), _ord_t834, _ord_t835)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("StringBuilder", 13LL)), _ord_t836); });
#line 499 "src/sema.tr"
    ({ __auto_type _ord_t837 = (SymbolKind_make_SClass()); __auto_type _ord_t838 = (box_asttype(AstType_init(_tr_str_lit_len("StringObj", 9LL)))); __auto_type _ord_t839 = (Symbol_init(_tr_str_lit_len("StringObj", 9LL), _ord_t837, _ord_t838)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("StringObj", 9LL)), _ord_t839); });
#line 501 "src/sema.tr"
    ({ __auto_type _ord_t840 = (SymbolKind_make_SClass()); __auto_type _ord_t841 = (box_asttype(AstType_init(_tr_str_lit_len("Dict", 4LL)))); __auto_type _ord_t842 = (Symbol_init(_tr_str_lit_len("Dict", 4LL), _ord_t840, _ord_t841)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Dict", 4LL)), _ord_t842); });
#line 502 "src/sema.tr"
    ({ __auto_type _ord_t843 = (SymbolKind_make_SClass()); __auto_type _ord_t844 = (box_asttype(AstType_init(_tr_str_lit_len("Set", 3LL)))); __auto_type _ord_t845 = (Symbol_init(_tr_str_lit_len("Set", 3LL), _ord_t843, _ord_t844)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Set", 3LL)), _ord_t845); });
#line 503 "src/sema.tr"
    ({ __auto_type _ord_t846 = (SymbolKind_make_SClass()); __auto_type _ord_t847 = (box_asttype(AstType_init(_tr_str_lit_len("Box", 3LL)))); __auto_type _ord_t848 = (Symbol_init(_tr_str_lit_len("Box", 3LL), _ord_t846, _ord_t847)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Box", 3LL)), _ord_t848); });
#line 506 "src/sema.tr"
    ({ __auto_type _ord_t849 = (SymbolKind_make_SEnum()); __auto_type _ord_t850 = (box_asttype(AstType_init(_tr_str_lit_len("Option", 6LL)))); __auto_type _ord_t851 = (Symbol_init(_tr_str_lit_len("Option", 6LL), _ord_t849, _ord_t850)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Option", 6LL)), _ord_t851); });
#line 507 "src/sema.tr"
    ({ __auto_type _ord_t852 = (SymbolKind_make_SEnum()); __auto_type _ord_t853 = (box_asttype(AstType_init(_tr_str_lit_len("Result", 6LL)))); __auto_type _ord_t854 = (Symbol_init(_tr_str_lit_len("Result", 6LL), _ord_t852, _ord_t853)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Result", 6LL)), _ord_t854); });
#line 510 "src/sema.tr"
    ({ __auto_type _ord_t855 = (SymbolKind_make_SClass()); __auto_type _ord_t856 = (box_asttype(AstType_init(_tr_str_lit_len("Exception", 9LL)))); __auto_type _ord_t857 = (Symbol_init(_tr_str_lit_len("Exception", 9LL), _ord_t855, _ord_t856)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Exception", 9LL)), _ord_t857); });
#line 511 "src/sema.tr"
    ({ __auto_type _ord_t858 = (SymbolKind_make_SClass()); __auto_type _ord_t859 = (box_asttype(AstType_init(_tr_str_lit_len("Error", 5LL)))); __auto_type _ord_t860 = (Symbol_init(_tr_str_lit_len("Error", 5LL), _ord_t858, _ord_t859)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Error", 5LL)), _ord_t860); });
#line 512 "src/sema.tr"
    ({ __auto_type _ord_t861 = (SymbolKind_make_SClass()); __auto_type _ord_t862 = (box_asttype(AstType_init(_tr_str_lit_len("ValueError", 10LL)))); __auto_type _ord_t863 = (Symbol_init(_tr_str_lit_len("ValueError", 10LL), _ord_t861, _ord_t862)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ValueError", 10LL)), _ord_t863); });
#line 513 "src/sema.tr"
    ({ __auto_type _ord_t864 = (SymbolKind_make_SClass()); __auto_type _ord_t865 = (box_asttype(AstType_init(_tr_str_lit_len("TypeError", 9LL)))); __auto_type _ord_t866 = (Symbol_init(_tr_str_lit_len("TypeError", 9LL), _ord_t864, _ord_t865)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("TypeError", 9LL)), _ord_t866); });
#line 514 "src/sema.tr"
    ({ __auto_type _ord_t867 = (SymbolKind_make_SClass()); __auto_type _ord_t868 = (box_asttype(AstType_init(_tr_str_lit_len("IndexError", 10LL)))); __auto_type _ord_t869 = (Symbol_init(_tr_str_lit_len("IndexError", 10LL), _ord_t867, _ord_t868)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("IndexError", 10LL)), _ord_t869); });
#line 515 "src/sema.tr"
    ({ __auto_type _ord_t870 = (SymbolKind_make_SClass()); __auto_type _ord_t871 = (box_asttype(AstType_init(_tr_str_lit_len("IOError", 7LL)))); __auto_type _ord_t872 = (Symbol_init(_tr_str_lit_len("IOError", 7LL), _ord_t870, _ord_t871)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("IOError", 7LL)), _ord_t872); });
#line 516 "src/sema.tr"
    ({ __auto_type _ord_t873 = (SymbolKind_make_SClass()); __auto_type _ord_t874 = (box_asttype(AstType_init(_tr_str_lit_len("KeyError", 8LL)))); __auto_type _ord_t875 = (Symbol_init(_tr_str_lit_len("KeyError", 8LL), _ord_t873, _ord_t874)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("KeyError", 8LL)), _ord_t875); });
#line 519 "src/sema.tr"
    ({ __auto_type _ord_t876 = (SymbolKind_make_SClass()); __auto_type _ord_t877 = (box_asttype(AstType_init(_tr_str_lit_len("Task", 4LL)))); __auto_type _ord_t878 = (Symbol_init(_tr_str_lit_len("Task", 4LL), _ord_t876, _ord_t877)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Task", 4LL)), _ord_t878); });
#line 520 "src/sema.tr"
    ({ __auto_type _ord_t879 = (SymbolKind_make_SClass()); __auto_type _ord_t880 = (box_asttype(AstType_init(_tr_str_lit_len("Future", 6LL)))); __auto_type _ord_t881 = (Symbol_init(_tr_str_lit_len("Future", 6LL), _ord_t879, _ord_t880)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Future", 6LL)), _ord_t881); });
#line 521 "src/sema.tr"
    ({ __auto_type _ord_t882 = (SymbolKind_make_SClass()); __auto_type _ord_t883 = (box_asttype(AstType_init(_tr_str_lit_len("Channel", 7LL)))); __auto_type _ord_t884 = (Symbol_init(_tr_str_lit_len("Channel", 7LL), _ord_t882, _ord_t883)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Channel", 7LL)), _ord_t884); });
#line 522 "src/sema.tr"
    ({ __auto_type _ord_t885 = (SymbolKind_make_SClass()); __auto_type _ord_t886 = (box_asttype(AstType_init(_tr_str_lit_len("Chan", 4LL)))); __auto_type _ord_t887 = (Symbol_init(_tr_str_lit_len("Chan", 4LL), _ord_t885, _ord_t886)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Chan", 4LL)), _ord_t887); });
#line 523 "src/sema.tr"
    ({ __auto_type _ord_t888 = (SymbolKind_make_SClass()); __auto_type _ord_t889 = (box_asttype(AstType_init(_tr_str_lit_len("Mutex", 5LL)))); __auto_type _ord_t890 = (Symbol_init(_tr_str_lit_len("Mutex", 5LL), _ord_t888, _ord_t889)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Mutex", 5LL)), _ord_t890); });
#line 524 "src/sema.tr"
    ({ __auto_type _ord_t891 = (SymbolKind_make_SClass()); __auto_type _ord_t892 = (box_asttype(AstType_init(_tr_str_lit_len("RwLock", 6LL)))); __auto_type _ord_t893 = (Symbol_init(_tr_str_lit_len("RwLock", 6LL), _ord_t891, _ord_t892)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("RwLock", 6LL)), _ord_t893); });
#line 525 "src/sema.tr"
    ({ __auto_type _ord_t894 = (SymbolKind_make_SClass()); __auto_type _ord_t895 = (box_asttype(AstType_init(_tr_str_lit_len("ThreadPool", 10LL)))); __auto_type _ord_t896 = (Symbol_init(_tr_str_lit_len("ThreadPool", 10LL), _ord_t894, _ord_t895)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ThreadPool", 10LL)), _ord_t896); });
#line 526 "src/sema.tr"
    ({ __auto_type _ord_t897 = (SymbolKind_make_SClass()); __auto_type _ord_t898 = (box_asttype(AstType_init(_tr_str_lit_len("Thread", 6LL)))); __auto_type _ord_t899 = (Symbol_init(_tr_str_lit_len("Thread", 6LL), _ord_t897, _ord_t898)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Thread", 6LL)), _ord_t899); });
#line 527 "src/sema.tr"
    ({ __auto_type _ord_t900 = (SymbolKind_make_SClass()); __auto_type _ord_t901 = (box_asttype(AstType_init(_tr_str_lit_len("AsyncPool", 9LL)))); __auto_type _ord_t902 = (Symbol_init(_tr_str_lit_len("AsyncPool", 9LL), _ord_t900, _ord_t901)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("AsyncPool", 9LL)), _ord_t902); });
#line 528 "src/sema.tr"
    ({ __auto_type _ord_t903 = (SymbolKind_make_SClass()); __auto_type _ord_t904 = (box_asttype(AstType_init(_tr_str_lit_len("AsyncTask", 9LL)))); __auto_type _ord_t905 = (Symbol_init(_tr_str_lit_len("AsyncTask", 9LL), _ord_t903, _ord_t904)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("AsyncTask", 9LL)), _ord_t905); });
#line 529 "src/sema.tr"
    ({ __auto_type _ord_t906 = (SymbolKind_make_SClass()); __auto_type _ord_t907 = (box_asttype(AstType_init(_tr_str_lit_len("Atomic", 6LL)))); __auto_type _ord_t908 = (Symbol_init(_tr_str_lit_len("Atomic", 6LL), _ord_t906, _ord_t907)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Atomic", 6LL)), _ord_t908); });
#line 530 "src/sema.tr"
    ({ __auto_type _ord_t909 = (SymbolKind_make_SClass()); __auto_type _ord_t910 = (box_asttype(AstType_init(_tr_str_lit_len("ThreadLocal", 11LL)))); __auto_type _ord_t911 = (Symbol_init(_tr_str_lit_len("ThreadLocal", 11LL), _ord_t909, _ord_t910)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ThreadLocal", 11LL)), _ord_t911); });
#line 531 "src/sema.tr"
    ({ __auto_type _ord_t912 = (SymbolKind_make_SFunction()); __auto_type _ord_t913 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t914 = (Symbol_init(_tr_str_lit_len("await_all", 9LL), _ord_t912, _ord_t913)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("await_all", 9LL)), _ord_t914); });
#line 532 "src/sema.tr"
    ({ __auto_type _ord_t915 = (SymbolKind_make_SClass()); __auto_type _ord_t916 = (box_asttype(AstType_init(_tr_str_lit_len("Arc", 3LL)))); __auto_type _ord_t917 = (Symbol_init(_tr_str_lit_len("Arc", 3LL), _ord_t915, _ord_t916)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Arc", 3LL)), _ord_t917); });
#line 533 "src/sema.tr"
    ({ __auto_type _ord_t918 = (SymbolKind_make_SClass()); __auto_type _ord_t919 = (box_asttype(AstType_init(_tr_str_lit_len("Rc", 2LL)))); __auto_type _ord_t920 = (Symbol_init(_tr_str_lit_len("Rc", 2LL), _ord_t918, _ord_t919)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Rc", 2LL)), _ord_t920); });
#line 534 "src/sema.tr"
    ({ __auto_type _ord_t921 = (SymbolKind_make_SClass()); __auto_type _ord_t922 = (box_asttype(AstType_init(_tr_str_lit_len("WaitGroup", 9LL)))); __auto_type _ord_t923 = (Symbol_init(_tr_str_lit_len("WaitGroup", 9LL), _ord_t921, _ord_t922)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("WaitGroup", 9LL)), _ord_t923); });
#line 537 "src/sema.tr"
    ({ __auto_type _ord_t924 = (SymbolKind_make_SClass()); __auto_type _ord_t925 = (box_asttype(AstType_init(_tr_str_lit_len("Shared", 6LL)))); __auto_type _ord_t926 = (Symbol_init(_tr_str_lit_len("Shared", 6LL), _ord_t924, _ord_t925)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Shared", 6LL)), _ord_t926); });
#line 538 "src/sema.tr"
    ({ __auto_type _ord_t927 = (SymbolKind_make_SClass()); __auto_type _ord_t928 = (box_asttype(AstType_init(_tr_str_lit_len("Weak", 4LL)))); __auto_type _ord_t929 = (Symbol_init(_tr_str_lit_len("Weak", 4LL), _ord_t927, _ord_t928)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Weak", 4LL)), _ord_t929); });
#line 540 "src/sema.tr"
    return s;
}

__attribute__((hot)) TrStr Sema_io_ty_str(Sema* self, AstType* ty) {
#line 545 "src/sema.tr"
    TrStr s = _tr_str_retain(ty->name);
#line 546 "src/sema.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("", 0LL)))) {
#line 546 "src/sema.tr"
        TrStr _strtmp_t930 = _tr_str_lit_len("void", 4LL);
        _tr_str_release(s);
        s = _strtmp_t930;
    }
#line 547 "src/sema.tr"
    if ((ty->args->len > 0LL)) {
#line 548 "src/sema.tr"
        TrStr _strtmp_t932 = _tr_strx_concatv((s), (_tr_str_lit_len("[", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t932;
#line 549 "src/sema.tr"
        long long i = 0LL;
#line 550 "src/sema.tr"
        while ((i < ty->args->len)) {
#line 551 "src/sema.tr"
            if ((i > 0LL)) {
#line 551 "src/sema.tr"
                TrStr _strtmp_t934 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t934;
            }
#line 552 "src/sema.tr"
            TrStr _strtmp_t936 = ({ TrStr _tr_cr = (Sema_io_ty_str(self, (*((AstType**)List_ptr_get(ty->args, i))))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t936;
#line 553 "src/sema.tr"
            i = (i + 1LL);
        }
#line 554 "src/sema.tr"
        TrStr _strtmp_t938 = _tr_strx_concatv((s), (_tr_str_lit_len("]", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t938;
    }
#line 555 "src/sema.tr"
    TrStr _trret178 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret178;
}

__attribute__((hot)) TrStr Sema_io_doc_of(Sema* self, Block* body) {
#line 559 "src/sema.tr"
    long long i = 0LL;
#line 560 "src/sema.tr"
    while ((i < body->stmts->len)) {
#line 561 "src/sema.tr"
        __auto_type _t939 = (*((Stmt*)List_ptr_get(body->stmts, i)));
        if (_t939.tag == Stmt_SLine) {
            __auto_type _ = _t939.data.SLine.n;
            /* pass */
        } else if (_t939.tag == Stmt_SExpr) {
            __auto_type e = _t939.data.SExpr.expr;
#line 564 "src/sema.tr"
            if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
#line 565 "src/sema.tr"
                __auto_type _t940 = (*e);
                if (_t940.tag == Expr_ELitStr) {
                    __auto_type s = _t940.data.ELitStr.val;
                    TrStr _trret179 = _tr_str_retain(s);
                    return _trret179;
                } else if (1) {
                    __auto_type _ = _t940;
                    /* pass */
                }
            }
#line 568 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        } else if (1) {
            __auto_type _ = _t939;
            return _tr_str_lit_len("", 0LL);
        }
#line 570 "src/sema.tr"
        i = (i + 1LL);
    }
#line 571 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Sema_io_func_sig(Sema* self, FunctionDef* f) {
#line 574 "src/sema.tr"
    TrStr s = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("def ", 4LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 575 "src/sema.tr"
    long long i = 0LL;
#line 576 "src/sema.tr"
    while ((i < f->params->len)) {
#line 577 "src/sema.tr"
        if ((i > 0LL)) {
#line 577 "src/sema.tr"
            TrStr _strtmp_t942 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t942;
        }
#line 578 "src/sema.tr"
        Param* p = ((Param*)List_ptr_get(f->params, i));
#line 579 "src/sema.tr"
        ({ TrStr _ap_t943 = (p->name); _tr_str_append(&s, _ap_t943); });
#line 580 "src/sema.tr"
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
#line 581 "src/sema.tr"
            TrStr _strtmp_t946 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cr = (Sema_io_ty_str(self, (*p->ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t946;
        }
#line 582 "src/sema.tr"
        if (p->is_variadic) {
#line 582 "src/sema.tr"
            TrStr _strtmp_t948 = _tr_strx_concatv((s), (_tr_str_lit_len("...", 3LL)));
            _tr_str_release(s);
            s = _strtmp_t948;
        }
#line 583 "src/sema.tr"
        i = (i + 1LL);
    }
#line 584 "src/sema.tr"
    TrStr _strtmp_t950 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
    _tr_str_release(s);
    s = _strtmp_t950;
#line 585 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 586 "src/sema.tr"
        AstType* rt = (*f->ret_ty);
#line 587 "src/sema.tr"
        if ((((!_tr_str_eqv((rt->name), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((rt->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((rt->name), (_tr_str_lit_len("None", 4LL)))))) {
#line 588 "src/sema.tr"
            TrStr _strtmp_t953 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _tr_cr = (Sema_io_ty_str(self, rt)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t953;
        }
    }
#line 589 "src/sema.tr"
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 590 "src/sema.tr"
        if ((!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 591 "src/sema.tr"
            TrStr _strtmp_t956 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" throws ", 8LL)))); TrStr _tr_cr = (Sema_io_ty_str(self, (*f->throws_ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t956;
        }
    }
#line 592 "src/sema.tr"
    TrStr doc = Sema_io_doc_of(self, f->body);
#line 593 "src/sema.tr"
    if ((!_tr_str_eqv((doc), (_tr_str_lit_len("", 0LL))))) {
#line 594 "src/sema.tr"
        TrStr _strtmp_t960 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("\n        \"\"\"", 12LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (doc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"\"\"", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t960;
    }
#line 595 "src/sema.tr"
    TrStr _trret180 = _tr_str_retain(s);
    _tr_str_release(s);
    _tr_str_release(doc);
    return _trret180;
}

__attribute__((hot)) TrStr Sema_build_inspect_str(Sema* self, TrStr name) {
#line 600 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
#line 601 "src/sema.tr"
        ClassDef* c = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
#line 602 "src/sema.tr"
        TrStr s = _tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (c->name));
#line 603 "src/sema.tr"
        if ((c->base_classes->len > 0LL)) {
#line 604 "src/sema.tr"
            TrStr _strtmp_t962 = _tr_strx_concatv((s), (_tr_str_lit_len("(extends ", 9LL)));
            _tr_str_release(s);
            s = _strtmp_t962;
#line 605 "src/sema.tr"
            long long bi = 0LL;
#line 606 "src/sema.tr"
            while ((bi < c->base_classes->len)) {
#line 607 "src/sema.tr"
                if ((bi > 0LL)) {
#line 607 "src/sema.tr"
                    TrStr _strtmp_t964 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                    _tr_str_release(s);
                    s = _strtmp_t964;
                }
#line 608 "src/sema.tr"
                TrStr _strtmp_t966 = ({ TrStr _tr_cr = (List_TrStr_get(c->base_classes, bi)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t966;
#line 609 "src/sema.tr"
                bi = (bi + 1LL);
            }
#line 610 "src/sema.tr"
            TrStr _strtmp_t968 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
            _tr_str_release(s);
            s = _strtmp_t968;
        }
#line 611 "src/sema.tr"
        TrStr _strtmp_t970 = _tr_strx_concatv((s), (_tr_str_lit_len(":\n", 2LL)));
        _tr_str_release(s);
        s = _strtmp_t970;
#line 612 "src/sema.tr"
        if ((!_tr_str_eqv((c->docstring), (_tr_str_lit_len("", 0LL))))) {
#line 613 "src/sema.tr"
            TrStr _strtmp_t974 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("  doc: ", 7LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->docstring)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t974;
        }
#line 614 "src/sema.tr"
        if ((c->fields->len > 0LL)) {
#line 615 "src/sema.tr"
            TrStr _strtmp_t976 = _tr_strx_concatv((s), (_tr_str_lit_len("  fields:\n", 10LL)));
            _tr_str_release(s);
            s = _strtmp_t976;
#line 616 "src/sema.tr"
            long long fi = 0LL;
#line 617 "src/sema.tr"
            while ((fi < c->fields->len)) {
#line 618 "src/sema.tr"
                FieldDef* fld = ((FieldDef*)List_ptr_get(c->fields, fi));
#line 619 "src/sema.tr"
                TrStr _strtmp_t979 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fld->name)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t979;
#line 620 "src/sema.tr"
                if ((((unsigned long long)(fld->ty)) != ((unsigned long long)(0LL)))) {
#line 621 "src/sema.tr"
                    TrStr _strtmp_t982 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cr = (Sema_io_ty_str(self, (*fld->ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
                    _tr_str_release(s);
                    s = _strtmp_t982;
                }
#line 622 "src/sema.tr"
                TrStr _strtmp_t984 = _tr_strx_concatv((s), (_tr_str_lit_len("\n", 1LL)));
                _tr_str_release(s);
                s = _strtmp_t984;
#line 623 "src/sema.tr"
                fi = (fi + 1LL);
            }
        }
#line 624 "src/sema.tr"
        if ((c->methods->len > 0LL)) {
#line 625 "src/sema.tr"
            TrStr _strtmp_t986 = _tr_strx_concatv((s), (_tr_str_lit_len("  methods:\n", 11LL)));
            _tr_str_release(s);
            s = _strtmp_t986;
#line 626 "src/sema.tr"
            long long mi = 0LL;
#line 627 "src/sema.tr"
            while ((mi < c->methods->len)) {
#line 628 "src/sema.tr"
                TrStr _strtmp_t990 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(c->methods, mi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t990;
#line 629 "src/sema.tr"
                mi = (mi + 1LL);
            }
        }
#line 630 "src/sema.tr"
        TrStr _trret181 = _tr_str_retain(s);
        _tr_str_release(s);
        return _trret181;
    } else if (_tr_dict_contains(self->enums, _tr_strz(name))) {
#line 632 "src/sema.tr"
        EnumDef* e = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(name)));
#line 633 "src/sema.tr"
        TrStr s = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("enum ", 5LL)), (e->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 634 "src/sema.tr"
        if ((e->variants->len > 0LL)) {
#line 635 "src/sema.tr"
            TrStr _strtmp_t992 = _tr_strx_concatv((s), (_tr_str_lit_len("  variants:\n", 12LL)));
            _tr_str_release(s);
            s = _strtmp_t992;
#line 636 "src/sema.tr"
            long long vi = 0LL;
#line 637 "src/sema.tr"
            while ((vi < e->variants->len)) {
#line 638 "src/sema.tr"
                VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, vi));
#line 639 "src/sema.tr"
                TrStr _strtmp_t995 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->name)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t995;
#line 640 "src/sema.tr"
                if ((v->fields->len > 0LL)) {
#line 641 "src/sema.tr"
                    TrStr _strtmp_t997 = _tr_strx_concatv((s), (_tr_str_lit_len("(", 1LL)));
                    _tr_str_release(s);
                    s = _strtmp_t997;
#line 642 "src/sema.tr"
                    long long vfi = 0LL;
#line 643 "src/sema.tr"
                    while ((vfi < v->fields->len)) {
#line 644 "src/sema.tr"
                        if ((vfi > 0LL)) {
#line 644 "src/sema.tr"
                            TrStr _strtmp_t999 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                            _tr_str_release(s);
                            s = _strtmp_t999;
                        }
#line 645 "src/sema.tr"
                        Param* vf = ((Param*)List_ptr_get(v->fields, vfi));
#line 646 "src/sema.tr"
                        if ((((unsigned long long)(vf->ty)) != ((unsigned long long)(0LL)))) {
#line 647 "src/sema.tr"
                            TrStr _strtmp_t1001 = ({ TrStr _tr_cr = (Sema_io_ty_str(self, (*vf->ty))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
                            _tr_str_release(s);
                            s = _strtmp_t1001;
                        }
#line 648 "src/sema.tr"
                        vfi = (vfi + 1LL);
                    }
#line 649 "src/sema.tr"
                    TrStr _strtmp_t1003 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
                    _tr_str_release(s);
                    s = _strtmp_t1003;
                }
#line 650 "src/sema.tr"
                TrStr _strtmp_t1005 = _tr_strx_concatv((s), (_tr_str_lit_len("\n", 1LL)));
                _tr_str_release(s);
                s = _strtmp_t1005;
#line 651 "src/sema.tr"
                vi = (vi + 1LL);
            }
        }
#line 652 "src/sema.tr"
        if ((e->methods->len > 0LL)) {
#line 653 "src/sema.tr"
            TrStr _strtmp_t1007 = _tr_strx_concatv((s), (_tr_str_lit_len("  methods:\n", 11LL)));
            _tr_str_release(s);
            s = _strtmp_t1007;
#line 654 "src/sema.tr"
            long long mi = 0LL;
#line 655 "src/sema.tr"
            while ((mi < e->methods->len)) {
#line 656 "src/sema.tr"
                TrStr _strtmp_t1011 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(e->methods, mi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t1011;
#line 657 "src/sema.tr"
                mi = (mi + 1LL);
            }
        }
#line 658 "src/sema.tr"
        TrStr _trret183 = _tr_str_retain(s);
        _tr_str_release(s);
        return _trret183;
    } else if (_tr_dict_contains(self->interfaces, _tr_strz(name))) {
#line 660 "src/sema.tr"
        InterfaceDef* iface = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(name)));
#line 661 "src/sema.tr"
        TrStr s = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("interface ", 10LL)), (iface->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 662 "src/sema.tr"
        if ((iface->methods->len > 0LL)) {
#line 663 "src/sema.tr"
            TrStr _strtmp_t1013 = _tr_strx_concatv((s), (_tr_str_lit_len("  methods:\n", 11LL)));
            _tr_str_release(s);
            s = _strtmp_t1013;
#line 664 "src/sema.tr"
            long long mi = 0LL;
#line 665 "src/sema.tr"
            while ((mi < iface->methods->len)) {
#line 666 "src/sema.tr"
                TrStr _strtmp_t1017 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(iface->methods, mi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t1017;
#line 667 "src/sema.tr"
                mi = (mi + 1LL);
            }
        }
#line 668 "src/sema.tr"
        TrStr _trret182 = _tr_str_retain(s);
        _tr_str_release(s);
        return _trret182;
    } else if (_tr_dict_contains(self->fn_defs, _tr_strz(name))) {
#line 670 "src/sema.tr"
        return Sema_io_func_sig(self, ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(name))));
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("int", 3LL)))) {
#line 672 "src/sema.tr"
        return _tr_str_lit_len("int: 64-bit signed integer (C long long).", 41LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("float", 5LL)))) {
#line 674 "src/sema.tr"
        return _tr_str_lit_len("float: 64-bit floating point number (C double).", 47LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL)))) {
#line 676 "src/sema.tr"
        return _tr_str_lit_len("bool: true/false (C _Bool).", 27LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL)))) {
#line 678 "src/sema.tr"
        return _tr_str_lit_len("str: immutable byte string (C char*).", 37LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("char", 4LL)))) {
#line 680 "src/sema.tr"
        return _tr_str_lit_len("char: single byte character (C char).", 37LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("List", 4LL)))) {
#line 682 "src/sema.tr"
        return _tr_str_lit_len("List[T]: growable, ordered, indexable sequence of T.", 52LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Dict", 4LL)))) {
#line 684 "src/sema.tr"
        return _tr_str_lit_len("Dict[K, V]: hash map from K to V.", 33LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Set", 3LL)))) {
#line 686 "src/sema.tr"
        return _tr_str_lit_len("Set[T]: unordered collection of unique T values.", 48LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Option", 6LL)))) {
#line 688 "src/sema.tr"
        return _tr_str_lit_len("Option[T]: either Some(T) or None.", 34LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Result", 6LL)))) {
#line 690 "src/sema.tr"
        return _tr_str_lit_len("Result[T, E]: either Ok(T) or Err(E).", 37LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 692 "src/sema.tr"
        return _tr_str_lit_len("Pointer[T]: raw pointer to a T (C T*); use unsafe: for arithmetic.", 66LL);
    } else {
#line 694 "src/sema.tr"
        return ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("No info available for '", 23LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
    }
}

__attribute__((hot)) void Sema_error(Sema* self, TrStr msg) {
#line 697 "src/sema.tr"
    TrStr loc = _tr_str_lit_len("", 0LL);
#line 698 "src/sema.tr"
    if ((_tr_str_lenv((self->current_file)) > 0LL)) {
#line 699 "src/sema.tr"
        TrStr _strtmp_t1018 = _tr_strx_concatv((self->current_file), (_tr_str_lit_len(":", 1LL)));
        _tr_str_release(loc);
        loc = _strtmp_t1018;
    }
#line 700 "src/sema.tr"
    if ((self->current_line > 0LL)) {
#line 701 "src/sema.tr"
        ({ TrStr _ord_t1019 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->current_line)))); TrStr _tr_cres = _tr_strx_concatv((loc), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (msg)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->errors, _ord_t1019); _tr_str_release(_ord_t1019); });
    } else {
#line 703 "src/sema.tr"
        List_TrStr_append(self->errors, msg);
    }
    _tr_str_release(loc);
}

__attribute__((hot)) bool Sema_is_sendable_type(Sema* self, TrStr ty_name) {
#line 708 "src/sema.tr"
    long long tpi = 0LL;
#line 709 "src/sema.tr"
    while ((tpi < self->sendable_tparams->len)) {
#line 710 "src/sema.tr"
        if (({ TrStr _cmpa_t1020 = (List_TrStr_get(self->sendable_tparams, tpi)); TrStr _cmpb_t1021 = (ty_name); _Bool _cmpr_t1022 = _tr_str_eqv(_cmpa_t1020, _cmpb_t1021); _tr_str_release(_cmpa_t1020); _cmpr_t1022; })) {
#line 710 "src/sema.tr"
            return true;
        }
#line 711 "src/sema.tr"
        tpi = (tpi + 1LL);
    }
#line 712 "src/sema.tr"
    if (((((_tr_str_eqv((ty_name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("str", 3LL))))) {
#line 712 "src/sema.tr"
        return true;
    }
#line 713 "src/sema.tr"
    if (((((_tr_str_eqv((ty_name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("auto", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("isize", 5LL))))) {
#line 713 "src/sema.tr"
        return true;
    }
#line 714 "src/sema.tr"
    if (((_tr_str_eqv((ty_name), (_tr_str_lit_len("Atomic", 6LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("RwLock", 6LL))))) {
#line 714 "src/sema.tr"
        return true;
    }
#line 715 "src/sema.tr"
    if (((_tr_str_eqv((ty_name), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("ThreadPool", 10LL))))) {
#line 715 "src/sema.tr"
        return true;
    }
#line 716 "src/sema.tr"
    if ((_tr_str_eqv((ty_name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
#line 716 "src/sema.tr"
        return true;
    }
#line 717 "src/sema.tr"
    if ((_tr_str_eqv((ty_name), (_tr_str_lit_len("AsyncPool", 9LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("AsyncTask", 9LL))))) {
#line 717 "src/sema.tr"
        return true;
    }
#line 718 "src/sema.tr"
    if ((_tr_str_eqv((ty_name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Weak", 4LL))))) {
#line 718 "src/sema.tr"
        return true;
    }
#line 720 "src/sema.tr"
    if ((((_tr_str_eqv((ty_name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Map", 3LL))))) {
#line 720 "src/sema.tr"
        return false;
    }
#line 722 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(ty_name))) {
#line 723 "src/sema.tr"
        ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ty_name)));
#line 724 "src/sema.tr"
        if ((!_is_invalid_ptr(((unsigned long long)(cls->iface_names))))) {
#line 725 "src/sema.tr"
            long long ii = 0LL;
#line 726 "src/sema.tr"
            while ((ii < cls->iface_names->len)) {
#line 727 "src/sema.tr"
                if (({ TrStr _cmpa_t1023 = (List_TrStr_get(cls->iface_names, ii)); TrStr _cmpb_t1024 = (_tr_str_lit_len("Sendable", 8LL)); _Bool _cmpr_t1025 = _tr_str_eqv(_cmpa_t1023, _cmpb_t1024); _tr_str_release(_cmpa_t1023); _cmpr_t1025; })) {
#line 727 "src/sema.tr"
                    return true;
                }
#line 728 "src/sema.tr"
                ii = (ii + 1LL);
            }
        }
#line 730 "src/sema.tr"
        return false;
    }
#line 732 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_sendable_ty(Sema* self, AstType* ty) {
#line 741 "src/sema.tr"
    TrStr n = _tr_str_retain(ty->name);
#line 742 "src/sema.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL))))) {
#line 743 "src/sema.tr"
        if ((ty->args->len > 0LL)) {
#line 744 "src/sema.tr"
            bool _trret184 = Sema_is_sendable_ty(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
            _tr_str_release(n);
            return _trret184;
        }
#line 745 "src/sema.tr"
        _tr_str_release(n);
        return true;
    }
#line 746 "src/sema.tr"
    bool _trret185 = Sema_is_sendable_type(self, n);
    _tr_str_release(n);
    return _trret185;
}

__attribute__((hot)) TrStr Sema_decl_family(Sema* self, AstType* ty) {
#line 753 "src/sema.tr"
    TrStr n = _tr_str_retain(ty->name);
#line 754 "src/sema.tr"
    if (((((((((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("isize", 5LL))))) {
#line 754 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("int", 3LL);
    }
#line 755 "src/sema.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))))) {
#line 755 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("float", 5LL);
    }
#line 756 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) {
#line 756 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("bool", 4LL);
    }
#line 757 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL)))) {
#line 757 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("str", 3LL);
    }
#line 758 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))))) {
#line 758 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("List", 4LL);
    }
#line 759 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
#line 759 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("Dict", 4LL);
    }
#line 760 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("tuple", 5LL))))) {
#line 760 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("Tuple", 5LL);
    }
#line 761 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) {
#line 761 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("Option", 6LL);
    }
#line 762 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 762 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("Pointer", 7LL);
    }
#line 763 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))) {
#line 763 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("Result", 6LL);
    }
#line 764 "src/sema.tr"
    long long gi = 0LL;
#line 765 "src/sema.tr"
    while ((gi < self->current_func_generics->len)) {
#line 766 "src/sema.tr"
        if (({ TrStr _cmpa_t1026 = (List_TrStr_get(self->current_func_generics, gi)); TrStr _cmpb_t1027 = (n); _Bool _cmpr_t1028 = _tr_str_eqv(_cmpa_t1026, _cmpb_t1027); _tr_str_release(_cmpa_t1026); _cmpr_t1028; })) {
#line 766 "src/sema.tr"
            _tr_str_release(n);
            return _tr_str_lit_len("", 0LL);
        }
#line 767 "src/sema.tr"
        gi = (gi + 1LL);
    }
#line 768 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
#line 769 "src/sema.tr"
        ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)));
#line 770 "src/sema.tr"
        if ((((cd->generics->len == 0LL) && (cd->iface_names->len == 0LL)) && (cd->base_classes->len == 0LL))) {
#line 770 "src/sema.tr"
            _tr_str_release(n);
            return _tr_str_lit_len("class", 5LL);
        }
#line 771 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("", 0LL);
    }
#line 772 "src/sema.tr"
    if (((_tr_dict_contains(self->enums, _tr_strz(n)) && (!_tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL))))) && (!_tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))))) {
#line 772 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("class", 5LL);
    }
#line 773 "src/sema.tr"
    if (((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
#line 773 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("class", 5LL);
    }
#line 774 "src/sema.tr"
    _tr_str_release(n);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void Sema_set_symbol_firm(Sema* self, TrStr name, TrStr fam, TrStr cls) {
#line 778 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("", 0LL)))) {
#line 778 "src/sema.tr"
        return;
    }
#line 779 "src/sema.tr"
    long long si = (self->scopes->len - 1LL);
#line 780 "src/sema.tr"
    while ((si >= 0LL)) {
#line 781 "src/sema.tr"
        Scope* sc = ((Scope*)List_ptr_get(self->scopes, si));
#line 782 "src/sema.tr"
        if (_tr_dict_contains(sc->variables, _tr_strz(name))) {
#line 783 "src/sema.tr"
            Symbol* sy = ((Symbol*)(uintptr_t)_tr_dict_get(sc->variables, _tr_strz(name)));
#line 784 "src/sema.tr"
            sy->firm = _tr_str_retain(fam);
#line 785 "src/sema.tr"
            sy->firm_cls = _tr_str_retain(cls);
#line 786 "src/sema.tr"
            _tr_dict_set(sc->variables, _tr_strz(name), sy);
#line 787 "src/sema.tr"
            return;
        }
#line 788 "src/sema.tr"
        si = (si - 1LL);
    }
#line 789 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 790 "src/sema.tr"
        Symbol* gs = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 791 "src/sema.tr"
        gs->firm = _tr_str_retain(fam);
#line 792 "src/sema.tr"
        gs->firm_cls = _tr_str_retain(cls);
#line 793 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), gs);
    }
}

__attribute__((hot)) bool Sema_is_global_symbol(Sema* self, TrStr name) {
#line 797 "src/sema.tr"
    long long si = (self->scopes->len - 1LL);
#line 798 "src/sema.tr"
    while ((si >= 1LL)) {
#line 799 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, si))->variables, _tr_strz(name))) {
#line 799 "src/sema.tr"
            return false;
        }
#line 800 "src/sema.tr"
        si = (si - 1LL);
    }
#line 801 "src/sema.tr"
    return true;
}

__attribute__((hot)) FunctionDef* Sema_find_unique_method(Sema* self, TrStr cls, TrStr m) {
#line 806 "src/sema.tr"
    FunctionDef* nothing = FunctionDef_init(_tr_str_lit_len("", 0LL));
#line 807 "src/sema.tr"
    if ((((!_tr_dict_contains(self->classes, _tr_strz(cls))) || _tr_dict_contains(self->ambiguous_classes, _tr_strz(cls))) || Sema_is_rt_builtin_class(self, cls))) {
#line 807 "src/sema.tr"
        return nothing;
    }
#line 808 "src/sema.tr"
    ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls)));
#line 809 "src/sema.tr"
    if ((cd->generics->len > 0LL)) {
#line 809 "src/sema.tr"
        return nothing;
    }
#line 810 "src/sema.tr"
    FunctionDef* found = _tr_obj_retain(nothing);
#line 811 "src/sema.tr"
    long long cnt = 0LL;
#line 812 "src/sema.tr"
    long long i = 0LL;
#line 813 "src/sema.tr"
    while ((i < cd->methods->len)) {
#line 814 "src/sema.tr"
        if (_tr_str_eqv((((FunctionDef*)List_ptr_get(cd->methods, i))->name), (m))) {
#line 815 "src/sema.tr"
            FunctionDef* _cltmp_t1029 = _tr_obj_retain(((FunctionDef*)List_ptr_get(cd->methods, i)));
            _tr_obj_release(found, _trdrop_FunctionDef);
            found = _cltmp_t1029;
#line 816 "src/sema.tr"
            cnt = (cnt + 1LL);
        }
#line 817 "src/sema.tr"
        i = (i + 1LL);
    }
#line 818 "src/sema.tr"
    if (((cnt == 1LL) && (found->generics->len == 0LL))) {
#line 818 "src/sema.tr"
        _tr_obj_release(nothing, _trdrop_FunctionDef);
        return found;
    }
#line 819 "src/sema.tr"
    if ((cnt > 1LL)) {
#line 819 "src/sema.tr"
        _tr_obj_release(found, _trdrop_FunctionDef);
        return nothing;
    }
#line 820 "src/sema.tr"
    long long bi = 0LL;
#line 821 "src/sema.tr"
    while ((bi < cd->base_classes->len)) {
#line 822 "src/sema.tr"
        FunctionDef* r = ({ TrStr _at_t1030 = (List_TrStr_get(cd->base_classes, bi)); __auto_type _wr = (Sema_find_unique_method(self, _at_t1030, m)); _tr_str_release(_at_t1030); _wr; });
#line 823 "src/sema.tr"
        if ((!_tr_str_eqv((r->name), (_tr_str_lit_len("", 0LL))))) {
#line 823 "src/sema.tr"
            _tr_obj_release(nothing, _trdrop_FunctionDef);
            _tr_obj_release(found, _trdrop_FunctionDef);
            return r;
        }
#line 824 "src/sema.tr"
        bi = (bi + 1LL);
        _tr_obj_release(r, _trdrop_FunctionDef);
    }
#line 825 "src/sema.tr"
    _tr_obj_release(found, _trdrop_FunctionDef);
    return nothing;
}

__attribute__((hot)) AstType* Sema_find_field_type(Sema* self, TrStr cls, TrStr f) {
#line 829 "src/sema.tr"
    AstType* nothing = AstType_init(_tr_str_lit_len("", 0LL));
#line 830 "src/sema.tr"
    if ((((!_tr_dict_contains(self->classes, _tr_strz(cls))) || _tr_dict_contains(self->ambiguous_classes, _tr_strz(cls))) || Sema_is_rt_builtin_class(self, cls))) {
#line 830 "src/sema.tr"
        return nothing;
    }
#line 831 "src/sema.tr"
    ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls)));
#line 832 "src/sema.tr"
    if ((cd->generics->len > 0LL)) {
#line 832 "src/sema.tr"
        return nothing;
    }
#line 833 "src/sema.tr"
    long long i = 0LL;
#line 834 "src/sema.tr"
    while ((i < cd->fields->len)) {
#line 835 "src/sema.tr"
        if ((_tr_str_eqv((((FieldDef*)List_ptr_get(cd->fields, i))->name), (f)) && (((unsigned long long)(((FieldDef*)List_ptr_get(cd->fields, i))->ty)) != ((unsigned long long)(0LL))))) {
#line 835 "src/sema.tr"
            return (*((FieldDef*)List_ptr_get(cd->fields, i))->ty);
        }
#line 836 "src/sema.tr"
        i = (i + 1LL);
    }
#line 837 "src/sema.tr"
    long long bi = 0LL;
#line 838 "src/sema.tr"
    while ((bi < cd->base_classes->len)) {
#line 839 "src/sema.tr"
        AstType* r = ({ TrStr _at_t1031 = (List_TrStr_get(cd->base_classes, bi)); __auto_type _wr = (Sema_find_field_type(self, _at_t1031, f)); _tr_str_release(_at_t1031); _wr; });
#line 840 "src/sema.tr"
        if ((!_tr_str_eqv((r->name), (_tr_str_lit_len("", 0LL))))) {
#line 840 "src/sema.tr"
            return r;
        }
#line 841 "src/sema.tr"
        bi = (bi + 1LL);
    }
#line 842 "src/sema.tr"
    return nothing;
}

__attribute__((hot)) TrStr Sema_firm_class(Sema* self, HirExpr* e) {
#line 846 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 846 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 847 "src/sema.tr"
    __auto_type _t1032 = (*e);
    if (_t1032.tag == HirExpr_EIdent) {
        __auto_type nm = _t1032.data.EIdent.name;
#line 849 "src/sema.tr"
        Symbol* sy = Sema_resolve(self, nm);
#line 850 "src/sema.tr"
        if (((!_tr_str_eqv((sy->name), (_tr_str_lit_len("", 0LL)))) && (sy->kind.tag == SymbolKind_make_SVariable().tag))) {
#line 850 "src/sema.tr"
            TrStr _trret186 = _tr_str_retain(sy->firm_cls);
            return _trret186;
        }
#line 851 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t1032.tag == HirExpr_ECall) {
        __auto_type callee = _t1032.data.ECall.callee;
#line 853 "src/sema.tr"
        __auto_type _t1033 = (*callee);
        if (_t1033.tag == HirExpr_EIdent) {
            __auto_type cn = _t1033.data.EIdent.name;
#line 855 "src/sema.tr"
            if (((_tr_dict_contains(self->classes, _tr_strz(cn)) && ({ __auto_type _ord_t1034 = (Sema_resolve(self, cn)->kind); __auto_type _wr = ((_ord_t1034.tag != SymbolKind_make_SVariable().tag)); _wr; })) && ({ TrStr _cmpa_t1035 = (Sema_decl_family(self, AstType_init(cn))); TrStr _cmpb_t1036 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t1037 = _tr_str_eqv(_cmpa_t1035, _cmpb_t1036); _tr_str_release(_cmpa_t1035); _cmpr_t1037; }))) {
#line 855 "src/sema.tr"
                TrStr _trret187 = _tr_str_retain(cn);
                return _trret187;
            }
#line 856 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        } else if (1) {
            __auto_type _ = _t1033;
            return _tr_str_lit_len("", 0LL);
        }
    } else if (_t1032.tag == HirExpr_EPropAccess) {
        __auto_type po = _t1032.data.EPropAccess.obj;
__auto_type pf = _t1032.data.EPropAccess.prop;
#line 859 "src/sema.tr"
        TrStr pc = Sema_firm_class(self, po);
#line 860 "src/sema.tr"
        if (_tr_str_eqv((pc), (_tr_str_lit_len("", 0LL)))) {
#line 860 "src/sema.tr"
            _tr_str_release(pc);
            return _tr_str_lit_len("", 0LL);
        }
#line 861 "src/sema.tr"
        AstType* ft = Sema_find_field_type(self, pc, pf);
#line 862 "src/sema.tr"
        if ((((!_tr_str_eqv((ft->name), (_tr_str_lit_len("", 0LL)))) && ({ TrStr _cmpa_t1038 = (Sema_decl_family(self, ft)); TrStr _cmpb_t1039 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t1040 = _tr_str_eqv(_cmpa_t1038, _cmpb_t1039); _tr_str_release(_cmpa_t1038); _cmpr_t1040; })) && _tr_dict_contains(self->classes, _tr_strz(ft->name)))) {
#line 862 "src/sema.tr"
            TrStr _trret188 = _tr_str_retain(ft->name);
            _tr_str_release(pc);
            return _trret188;
        }
#line 863 "src/sema.tr"
        _tr_str_release(pc);
        return _tr_str_lit_len("", 0LL);
    } else if (_t1032.tag == HirExpr_EMethodCall) {
        __auto_type mo = _t1032.data.EMethodCall.obj;
__auto_type mm = _t1032.data.EMethodCall.method;
__auto_type margs = _t1032.data.EMethodCall.args;
#line 865 "src/sema.tr"
        TrStr rc = Sema_method_receiver_class(self, mo);
#line 866 "src/sema.tr"
        if (_tr_str_eqv((rc), (_tr_str_lit_len("", 0LL)))) {
#line 866 "src/sema.tr"
            _tr_str_release(rc);
            return _tr_str_lit_len("", 0LL);
        }
#line 867 "src/sema.tr"
        FunctionDef* md = Sema_find_unique_method(self, rc, mm);
#line 868 "src/sema.tr"
        if ((_tr_str_eqv((md->name), (_tr_str_lit_len("", 0LL))) || (((unsigned long long)(md->ret_ty)) == ((unsigned long long)(0LL))))) {
#line 868 "src/sema.tr"
            _tr_str_release(rc);
            _tr_obj_release(md, _trdrop_FunctionDef);
            return _tr_str_lit_len("", 0LL);
        }
#line 869 "src/sema.tr"
        if (((((unsigned long long)(md->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*md->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 869 "src/sema.tr"
            _tr_str_release(rc);
            _tr_obj_release(md, _trdrop_FunctionDef);
            return _tr_str_lit_len("", 0LL);
        }
#line 870 "src/sema.tr"
        AstType* rt = (*md->ret_ty);
#line 871 "src/sema.tr"
        if ((({ TrStr _cmpa_t1041 = (Sema_decl_family(self, rt)); TrStr _cmpb_t1042 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t1043 = _tr_str_eqv(_cmpa_t1041, _cmpb_t1042); _tr_str_release(_cmpa_t1041); _cmpr_t1043; }) && _tr_dict_contains(self->classes, _tr_strz(rt->name)))) {
#line 871 "src/sema.tr"
            TrStr _trret189 = _tr_str_retain(rt->name);
            _tr_str_release(rc);
            _tr_obj_release(md, _trdrop_FunctionDef);
            return _trret189;
        }
#line 872 "src/sema.tr"
        _tr_str_release(rc);
        _tr_obj_release(md, _trdrop_FunctionDef);
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t1032;
        return _tr_str_lit_len("", 0LL);
    }
#line 874 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Sema_method_receiver_class(Sema* self, HirExpr* mo) {
#line 879 "src/sema.tr"
    __auto_type _t1044 = (*mo);
    if (_t1044.tag == HirExpr_EIdent) {
        __auto_type rn = _t1044.data.EIdent.name;
#line 881 "src/sema.tr"
        Symbol* sy = Sema_resolve(self, rn);
#line 882 "src/sema.tr"
        if (((!_tr_str_eqv((sy->name), (_tr_str_lit_len("", 0LL)))) && (sy->kind.tag == SymbolKind_make_SVariable().tag))) {
#line 882 "src/sema.tr"
            TrStr _trret190 = _tr_str_retain(sy->firm_cls);
            return _trret190;
        }
#line 883 "src/sema.tr"
        if ((_tr_dict_contains(self->classes, _tr_strz(rn)) && ({ TrStr _cmpa_t1045 = (Sema_decl_family(self, AstType_init(rn))); TrStr _cmpb_t1046 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t1047 = _tr_str_eqv(_cmpa_t1045, _cmpb_t1046); _tr_str_release(_cmpa_t1045); _cmpr_t1047; }))) {
#line 883 "src/sema.tr"
            TrStr _trret191 = _tr_str_retain(rn);
            return _trret191;
        }
#line 884 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t1044;
        return Sema_firm_class(self, mo);
    }
#line 886 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Sema_firm_family(Sema* self, HirExpr* e) {
#line 889 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 889 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 890 "src/sema.tr"
    __auto_type _t1048 = (*e);
    if (_t1048.tag == HirExpr_ELitInt) {
        return _tr_str_lit_len("int", 3LL);
    } else if (_t1048.tag == HirExpr_ELitFloat) {
        return _tr_str_lit_len("float", 5LL);
    } else if (_t1048.tag == HirExpr_ELitStr) {
        return _tr_str_lit_len("str", 3LL);
    } else if (_t1048.tag == HirExpr_ELitBool) {
        return _tr_str_lit_len("bool", 4LL);
    } else if (_t1048.tag == HirExpr_ELitNone) {
        __auto_type _ = _t1048.data.ELitNone.ty;
        return _tr_str_lit_len("None", 4LL);
    } else if (_t1048.tag == HirExpr_EFString) {
        return _tr_str_lit_len("str", 3LL);
    } else if (_t1048.tag == HirExpr_EList) {
        return _tr_str_lit_len("List", 4LL);
    } else if (_t1048.tag == HirExpr_EDict) {
        return _tr_str_lit_len("Dict", 4LL);
    } else if (_t1048.tag == HirExpr_ETuple) {
        return _tr_str_lit_len("Tuple", 5LL);
    } else if (_t1048.tag == HirExpr_EIdent) {
        __auto_type nm = _t1048.data.EIdent.name;
#line 901 "src/sema.tr"
        Symbol* sy = Sema_resolve(self, nm);
#line 902 "src/sema.tr"
        if (((!_tr_str_eqv((sy->name), (_tr_str_lit_len("", 0LL)))) && (sy->kind.tag == SymbolKind_make_SVariable().tag))) {
#line 903 "src/sema.tr"
            if (((_tr_str_eqv((sy->firm), (_tr_str_lit_len("", 0LL))) && _tr_dict_contains(self->global_firm, _tr_strz(nm))) && Sema_is_global_symbol(self, nm))) {
#line 903 "src/sema.tr"
                TrStr _trret192 = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->global_firm, _tr_strz(nm))));
                return _trret192;
            }
#line 904 "src/sema.tr"
            TrStr _trret193 = _tr_str_retain(sy->firm);
            return _trret193;
        }
#line 905 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t1048.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t1048.data.EUnaryOp.op;
__auto_type x = _t1048.data.EUnaryOp.expr;
#line 907 "src/sema.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("not", 3LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!", 1LL))))) {
#line 907 "src/sema.tr"
            return _tr_str_lit_len("bool", 4LL);
        }
#line 908 "src/sema.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))))) {
#line 909 "src/sema.tr"
            TrStr xf = Sema_firm_family(self, x);
#line 910 "src/sema.tr"
            if ((_tr_str_eqv((xf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((xf), (_tr_str_lit_len("float", 5LL))))) {
#line 910 "src/sema.tr"
                TrStr _trret194 = _tr_str_retain(xf);
                _tr_str_release(xf);
                return _trret194;
            }
        }
#line 911 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t1048.tag == HirExpr_EBinOp) {
        __auto_type op = _t1048.data.EBinOp.op;
__auto_type l = _t1048.data.EBinOp.left;
__auto_type r = _t1048.data.EBinOp.right;
#line 913 "src/sema.tr"
        if ((((((((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("or", 2LL))))) {
#line 913 "src/sema.tr"
            return _tr_str_lit_len("bool", 4LL);
        }
#line 914 "src/sema.tr"
        TrStr lf = Sema_firm_family(self, l);
#line 915 "src/sema.tr"
        TrStr rf = Sema_firm_family(self, r);
#line 916 "src/sema.tr"
        if (((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) && _tr_str_eqv((lf), (_tr_str_lit_len("str", 3LL)))) && _tr_str_eqv((rf), (_tr_str_lit_len("str", 3LL))))) {
#line 916 "src/sema.tr"
            _tr_str_release(lf);
            _tr_str_release(rf);
            return _tr_str_lit_len("str", 3LL);
        }
#line 917 "src/sema.tr"
        if ((((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL))))) {
#line 918 "src/sema.tr"
            if (((_tr_str_eqv((lf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((lf), (_tr_str_lit_len("float", 5LL)))) && (_tr_str_eqv((rf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((rf), (_tr_str_lit_len("float", 5LL)))))) {
#line 919 "src/sema.tr"
                if ((_tr_str_eqv((lf), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((rf), (_tr_str_lit_len("float", 5LL))))) {
#line 919 "src/sema.tr"
                    _tr_str_release(lf);
                    _tr_str_release(rf);
                    return _tr_str_lit_len("float", 5LL);
                }
#line 920 "src/sema.tr"
                _tr_str_release(lf);
                _tr_str_release(rf);
                return _tr_str_lit_len("int", 3LL);
            }
        }
#line 921 "src/sema.tr"
        _tr_str_release(lf);
        _tr_str_release(rf);
        return _tr_str_lit_len("", 0LL);
    } else if (_t1048.tag == HirExpr_EMethodCall) {
        __auto_type mo = _t1048.data.EMethodCall.obj;
__auto_type mm = _t1048.data.EMethodCall.method;
__auto_type margs = _t1048.data.EMethodCall.args;
#line 923 "src/sema.tr"
        if ((_tr_str_eqv((mm), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((mm), (_tr_str_lit_len("new", 3LL))))) {
#line 924 "src/sema.tr"
            __auto_type _t1049 = (*mo);
            if (_t1049.tag == HirExpr_EIndex) {
                __auto_type cb = _t1049.data.EIndex.obj;
#line 926 "src/sema.tr"
                __auto_type _t1050 = (*cb);
                if (_t1050.tag == HirExpr_EIdent) {
                    __auto_type cbn = _t1050.data.EIdent.name;
#line 928 "src/sema.tr"
                    if (((_tr_str_eqv((cbn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((cbn), (_tr_str_lit_len("Vec", 3LL)))) && (!_tr_dict_contains(self->classes, _tr_strz(cbn))))) {
#line 928 "src/sema.tr"
                        return _tr_str_lit_len("List", 4LL);
                    }
#line 929 "src/sema.tr"
                    if (((_tr_str_eqv((cbn), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((cbn), (_tr_str_lit_len("Map", 3LL)))) && (!_tr_dict_contains(self->classes, _tr_strz(cbn))))) {
#line 929 "src/sema.tr"
                        return _tr_str_lit_len("Dict", 4LL);
                    }
                } else if (1) {
                    __auto_type _ = _t1050;
                    /* pass */
                }
            } else if (1) {
                __auto_type _ = _t1049;
                /* pass */
            }
        }
#line 932 "src/sema.tr"
        TrStr mrc = Sema_method_receiver_class(self, mo);
#line 933 "src/sema.tr"
        if ((!_tr_str_eqv((mrc), (_tr_str_lit_len("", 0LL))))) {
#line 934 "src/sema.tr"
            FunctionDef* mmd = Sema_find_unique_method(self, mrc, mm);
#line 935 "src/sema.tr"
            if ((((!_tr_str_eqv((mmd->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(mmd->ret_ty)) != ((unsigned long long)(0LL)))) && (!((((unsigned long long)(mmd->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*mmd->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))))) {
#line 936 "src/sema.tr"
                TrStr _trret195 = Sema_decl_family(self, (*mmd->ret_ty));
                _tr_str_release(mrc);
                _tr_obj_release(mmd, _trdrop_FunctionDef);
                return _trret195;
            }
            _tr_obj_release(mmd, _trdrop_FunctionDef);
        }
#line 937 "src/sema.tr"
        if (((_tr_str_eqv((mm), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((mm), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 0LL))) {
#line 938 "src/sema.tr"
            TrStr lrf = Sema_firm_family(self, mo);
#line 939 "src/sema.tr"
            if (((_tr_str_eqv((lrf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((lrf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((lrf), (_tr_str_lit_len("Dict", 4LL))))) {
#line 939 "src/sema.tr"
                _tr_str_release(mrc);
                _tr_str_release(lrf);
                return _tr_str_lit_len("int", 3LL);
            }
        }
#line 940 "src/sema.tr"
        if (((_tr_str_eqv((mm), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((mm), (_tr_str_lit_len("to_string", 9LL)))) && (margs->len == 0LL))) {
#line 941 "src/sema.tr"
            TrStr mty = _tr_str_retain(hir_expr_type(mo)->name);
#line 942 "src/sema.tr"
            if ((!(_tr_dict_contains(self->classes, _tr_strz(mty)) && Sema_class_method_exists(self, mty, mm)))) {
#line 942 "src/sema.tr"
                _tr_str_release(mrc);
                _tr_str_release(mty);
                return _tr_str_lit_len("str", 3LL);
            }
        }
#line 943 "src/sema.tr"
        _tr_str_release(mrc);
        return _tr_str_lit_len("", 0LL);
    } else if (_t1048.tag == HirExpr_EPropAccess) {
        __auto_type po = _t1048.data.EPropAccess.obj;
__auto_type pf = _t1048.data.EPropAccess.prop;
#line 945 "src/sema.tr"
        TrStr pc = Sema_firm_class(self, po);
#line 946 "src/sema.tr"
        if ((!_tr_str_eqv((pc), (_tr_str_lit_len("", 0LL))))) {
#line 947 "src/sema.tr"
            AstType* ft = Sema_find_field_type(self, pc, pf);
#line 948 "src/sema.tr"
            if ((!_tr_str_eqv((ft->name), (_tr_str_lit_len("", 0LL))))) {
#line 948 "src/sema.tr"
                TrStr _trret196 = Sema_decl_family(self, ft);
                _tr_str_release(pc);
                return _trret196;
            }
        }
#line 949 "src/sema.tr"
        _tr_str_release(pc);
        return _tr_str_lit_len("", 0LL);
    } else if (_t1048.tag == HirExpr_ECall) {
        __auto_type callee = _t1048.data.ECall.callee;
#line 951 "src/sema.tr"
        __auto_type _t1051 = (*callee);
        if (_t1051.tag == HirExpr_EIdent) {
            __auto_type fn = _t1051.data.EIdent.name;
#line 953 "src/sema.tr"
            if (((_tr_dict_contains(self->classes, _tr_strz(fn)) && ({ __auto_type _ord_t1052 = (Sema_resolve(self, fn)->kind); __auto_type _wr = ((_ord_t1052.tag != SymbolKind_make_SVariable().tag)); _wr; })) && ({ TrStr _cmpa_t1053 = (Sema_decl_family(self, AstType_init(fn))); TrStr _cmpb_t1054 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t1055 = _tr_str_eqv(_cmpa_t1053, _cmpb_t1054); _tr_str_release(_cmpa_t1053); _cmpr_t1055; }))) {
#line 953 "src/sema.tr"
                return _tr_str_lit_len("class", 5LL);
            }
#line 954 "src/sema.tr"
            if (_tr_dict_contains(self->fn_defs, _tr_strz(fn))) {
#line 955 "src/sema.tr"
                FunctionDef* fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(fn)));
#line 956 "src/sema.tr"
                if ((((fd->generics->len > 0LL) || fd->is_extern) || (((unsigned long long)(fd->ret_ty)) == ((unsigned long long)(0LL))))) {
#line 956 "src/sema.tr"
                    return _tr_str_lit_len("", 0LL);
                }
#line 957 "src/sema.tr"
                if (((((unsigned long long)(fd->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*fd->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 957 "src/sema.tr"
                    return _tr_str_lit_len("", 0LL);
                }
#line 958 "src/sema.tr"
                TrStr _trret197 = Sema_decl_family(self, (*fd->ret_ty));
                return _trret197;
            }
#line 959 "src/sema.tr"
            if (_tr_str_eqv((Sema_resolve(self, fn)->name), (_tr_str_lit_len("", 0LL)))) {
#line 960 "src/sema.tr"
                if (_tr_str_eqv((fn), (_tr_str_lit_len("str", 3LL)))) {
#line 960 "src/sema.tr"
                    return _tr_str_lit_len("str", 3LL);
                }
#line 961 "src/sema.tr"
                if ((_tr_str_eqv((fn), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("len", 3LL))))) {
#line 961 "src/sema.tr"
                    return _tr_str_lit_len("int", 3LL);
                }
#line 962 "src/sema.tr"
                if (_tr_str_eqv((fn), (_tr_str_lit_len("float", 5LL)))) {
#line 962 "src/sema.tr"
                    return _tr_str_lit_len("float", 5LL);
                }
            }
#line 963 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        } else if (1) {
            __auto_type _ = _t1051;
            return _tr_str_lit_len("", 0LL);
        }
    } else if (1) {
        __auto_type _ = _t1048;
        return _tr_str_lit_len("", 0LL);
    }
#line 966 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool Sema_family_mismatch(Sema* self, TrStr exp, TrStr act) {
#line 969 "src/sema.tr"
    if ((_tr_str_eqv((exp), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((act), (_tr_str_lit_len("", 0LL))))) {
#line 969 "src/sema.tr"
        return false;
    }
#line 970 "src/sema.tr"
    bool exp_scalar = (((_tr_str_eqv((exp), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((exp), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("str", 3LL))));
#line 971 "src/sema.tr"
    bool act_scalar = (((_tr_str_eqv((act), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((act), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((act), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((act), (_tr_str_lit_len("str", 3LL))));
#line 972 "src/sema.tr"
    if ((exp_scalar && act_scalar)) {
#line 972 "src/sema.tr"
        return (_tr_str_eqv((exp), (_tr_str_lit_len("str", 3LL))) != _tr_str_eqv((act), (_tr_str_lit_len("str", 3LL))));
    }
#line 973 "src/sema.tr"
    if ((exp_scalar && (_tr_str_eqv((act), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((act), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 973 "src/sema.tr"
        return true;
    }
#line 974 "src/sema.tr"
    if ((act_scalar && ((_tr_str_eqv((exp), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((exp), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("class", 5LL)))))) {
#line 974 "src/sema.tr"
        return true;
    }
#line 975 "src/sema.tr"
    if (((_tr_str_eqv((exp), (_tr_str_lit_len("List", 4LL))) && _tr_str_eqv((act), (_tr_str_lit_len("Dict", 4LL)))) || (_tr_str_eqv((exp), (_tr_str_lit_len("Dict", 4LL))) && _tr_str_eqv((act), (_tr_str_lit_len("List", 4LL)))))) {
#line 975 "src/sema.tr"
        return true;
    }
#line 976 "src/sema.tr"
    if ((_tr_str_eqv((exp), (_tr_str_lit_len("class", 5LL))) && (_tr_str_eqv((act), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((act), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 976 "src/sema.tr"
        return true;
    }
#line 977 "src/sema.tr"
    if (((_tr_str_eqv((exp), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((exp), (_tr_str_lit_len("Result", 6LL)))) && (((((_tr_str_eqv((act), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((act), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((act), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((act), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((act), (_tr_str_lit_len("bool", 4LL)))) || (_tr_str_eqv((exp), (_tr_str_lit_len("Result", 6LL))) && (_tr_str_eqv((act), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((act), (_tr_str_lit_len("str", 3LL)))))))) {
#line 977 "src/sema.tr"
        return true;
    }
#line 978 "src/sema.tr"
    if ((_tr_str_eqv((act), (_tr_str_lit_len("None", 4LL))) && (((exp_scalar || _tr_str_eqv((exp), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("Tuple", 5LL)))))) {
#line 978 "src/sema.tr"
        return true;
    }
#line 979 "src/sema.tr"
    if ((_tr_str_eqv((act), (_tr_str_lit_len("class", 5LL))) && (((exp_scalar || _tr_str_eqv((exp), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("Tuple", 5LL)))))) {
#line 979 "src/sema.tr"
        return true;
    }
#line 980 "src/sema.tr"
    if ((_tr_str_eqv((act), (_tr_str_lit_len("Option", 6LL))) && (((exp_scalar || _tr_str_eqv((exp), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("class", 5LL)))))) {
#line 980 "src/sema.tr"
        return true;
    }
#line 981 "src/sema.tr"
    if ((_tr_str_eqv((exp), (_tr_str_lit_len("Tuple", 5LL))) && (!_tr_str_eqv((act), (_tr_str_lit_len("Tuple", 5LL)))))) {
#line 981 "src/sema.tr"
        return ((act_scalar || _tr_str_eqv((act), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((act), (_tr_str_lit_len("Dict", 4LL))));
    }
#line 982 "src/sema.tr"
    if ((_tr_str_eqv((act), (_tr_str_lit_len("Tuple", 5LL))) && (!_tr_str_eqv((exp), (_tr_str_lit_len("Tuple", 5LL)))))) {
#line 982 "src/sema.tr"
        return (((exp_scalar || _tr_str_eqv((exp), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("class", 5LL))));
    }
#line 983 "src/sema.tr"
    return false;
}

__attribute__((hot)) TrStr Sema_family_word(Sema* self, TrStr fam) {
#line 986 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("class", 5LL)))) {
#line 986 "src/sema.tr"
        return _tr_str_lit_len("a class/enum value", 18LL);
    }
#line 987 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("Option", 6LL)))) {
#line 987 "src/sema.tr"
        return _tr_str_lit_len("an Option (maybe-empty) value", 29LL);
    }
#line 988 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("None", 4LL)))) {
#line 988 "src/sema.tr"
        return _tr_str_lit_len("None", 4LL);
    }
#line 989 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 989 "src/sema.tr"
        return _tr_str_lit_len("a Pointer", 9LL);
    }
#line 990 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("Result", 6LL)))) {
#line 990 "src/sema.tr"
        return _tr_str_lit_len("a Result", 8LL);
    }
#line 991 "src/sema.tr"
    return _tr_strx_concatv((_tr_str_lit_len("a ", 2LL)), (fam));
}

__attribute__((hot)) void Sema_check_expr_assignable(Sema* self, AstType* exp_ty, HirExpr* e, TrStr what) {
#line 995 "src/sema.tr"
    if (((((_tr_str_eqv((exp_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((exp_ty->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((exp_ty->name), (_tr_str_lit_len("Set", 3LL)))) && (exp_ty->args->len > 0LL)) && (!_is_invalid_ptr(((unsigned long long)(e)))))) {
#line 996 "src/sema.tr"
        AstType* elt = (*((AstType**)List_ptr_get(exp_ty->args, 0LL)));
#line 997 "src/sema.tr"
        __auto_type _t1056 = (*e);
        if (_t1056.tag == HirExpr_EList) {
            __auto_type items = _t1056.data.EList.items;
#line 999 "src/sema.tr"
            long long li = 0LL;
#line 1000 "src/sema.tr"
            while ((li < items->len)) {
#line 1001 "src/sema.tr"
                if (({ TrStr _ord_t1057 = (Sema_decl_family(self, elt)); TrStr _ord_t1058 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(items, li)))); __auto_type _wr = (Sema_family_mismatch(self, _ord_t1057, _ord_t1058)); _tr_str_release(_ord_t1057); _tr_str_release(_ord_t1058); _wr; })) {
#line 1002 "src/sema.tr"
                    ({ __auto_type _ord_t1059 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: ", 21LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is a ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (exp_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (elt->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] but its element ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1061 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1059); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((li + 1LL))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1060 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(items, li)))); __auto_type _ord_t1062 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1061); TrStr _tr_cr = (Sema_family_word(self, _at_t1060)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: make every element a '", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1063 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1062); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (elt->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1063); _tr_str_release(_at_t1060); _tr_str_release(_at_t1063); });
#line 1003 "src/sema.tr"
                    break;
                }
#line 1004 "src/sema.tr"
                li = (li + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t1056;
            /* pass */
        }
    }
#line 1007 "src/sema.tr"
    TrStr ef = Sema_decl_family(self, exp_ty);
#line 1008 "src/sema.tr"
    TrStr af = Sema_firm_family(self, e);
#line 1009 "src/sema.tr"
    if (Sema_family_mismatch(self, ef, af)) {
#line 1010 "src/sema.tr"
        ({ __auto_type _ord_t1064 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: ", 21LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" expects '", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (exp_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but is given ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1065 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1064); TrStr _tr_cr = (Sema_family_word(self, af)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass a value of type '", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1066 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1065); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (exp_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (convert it explicitly, e.g. str(x) / int(x) / float(x), if that is what you mean).", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1066); _tr_str_release(_at_t1066); });
    }
    _tr_str_release(ef);
    _tr_str_release(af);
}

__attribute__((hot)) void Sema_check_call_args(Sema* self, FunctionDef* fd, List_ptr* args, long long sk, TrStr what) {
#line 1016 "src/sema.tr"
    long long pi = sk;
#line 1017 "src/sema.tr"
    while ((pi < fd->params->len)) {
#line 1018 "src/sema.tr"
        if (((Param*)List_ptr_get(fd->params, pi))->is_variadic) {
#line 1018 "src/sema.tr"
            return;
        }
#line 1019 "src/sema.tr"
        pi = (pi + 1LL);
    }
#line 1020 "src/sema.tr"
    long long want = (fd->params->len - sk);
#line 1021 "src/sema.tr"
    if ((want != args->len)) {
#line 1022 "src/sema.tr"
        TrStr plural = _tr_str_lit_len("s", 1LL);
#line 1023 "src/sema.tr"
        if ((want == 1LL)) {
#line 1023 "src/sema.tr"
            TrStr _strtmp_t1067 = _tr_str_lit_len("", 0LL);
            _tr_str_release(plural);
            plural = _strtmp_t1067;
        }
#line 1024 "src/sema.tr"
        TrStr verb = _tr_str_lit_len("were", 4LL);
#line 1025 "src/sema.tr"
        if ((args->len == 1LL)) {
#line 1025 "src/sema.tr"
            TrStr _strtmp_t1068 = _tr_str_lit_len("was", 3LL);
            _tr_str_release(verb);
            verb = _strtmp_t1068;
        }
#line 1026 "src/sema.tr"
        ({ __auto_type _ord_t1069 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" takes ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (plural)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1070 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1069); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(args->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (verb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1071 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1070); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (plural)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (see its definition).", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1071); _tr_str_release(_at_t1071); });
#line 1027 "src/sema.tr"
        _tr_str_release(plural);
        _tr_str_release(verb);
        return;
    }
#line 1028 "src/sema.tr"
    if ((fd->generics->len > 0LL)) {
#line 1028 "src/sema.tr"
        return;
    }
#line 1029 "src/sema.tr"
    long long ai = 0LL;
#line 1030 "src/sema.tr"
    while ((ai < args->len)) {
#line 1031 "src/sema.tr"
        AstType** pty = ((Param*)List_ptr_get(fd->params, (sk + ai)))->ty;
#line 1032 "src/sema.tr"
        if ((((unsigned long long)(pty)) != ((unsigned long long)(0LL)))) {
#line 1033 "src/sema.tr"
            ({ __auto_type _ord_t1072 = ((*pty)); __auto_type _ord_t1073 = (((HirExpr*)List_ptr_get(args, ai))); TrStr _ord_t1074 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((ai + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (what)); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1072, _ord_t1073, _ord_t1074); _tr_str_release(_ord_t1074); });
        }
#line 1034 "src/sema.tr"
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) long long Sema_builtin_method_range(Sema* self, TrStr fam, TrStr method) {
#line 1039 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("str", 3LL)))) {
#line 1040 "src/sema.tr"
        if ((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("upper", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("lower", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_upper", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_lower", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("strip", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("trim", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("trim_left", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("trim_right", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("capitalize", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("title", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL))))) {
#line 1040 "src/sema.tr"
            return 0LL;
        }
#line 1041 "src/sema.tr"
        if ((((((((((_tr_str_eqv((method), (_tr_str_lit_len("find", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("starts_with", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("ends_with", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("split", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("split_exact", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("repeat", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("strip_prefix", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("strip_suffix", 12LL))))) {
#line 1041 "src/sema.tr"
            return 101LL;
        }
#line 1042 "src/sema.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("replace", 7LL)))) {
#line 1042 "src/sema.tr"
            return 202LL;
        }
    } else if (_tr_str_eqv((fam), (_tr_str_lit_len("List", 4LL)))) {
#line 1044 "src/sema.tr"
        if ((((((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("last", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max_val", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("min_val", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sum", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reversed", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reversed_copy", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_asc", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_desc", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_list", 7LL))))) {
#line 1044 "src/sema.tr"
            return 0LL;
        }
#line 1045 "src/sema.tr"
        if (((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("append", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("push", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reserve", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_by", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("union", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("intersection", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("difference", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_subset", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("all", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("any", 3LL))))) {
#line 1045 "src/sema.tr"
            return 101LL;
        }
#line 1046 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("swap", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL))))) {
#line 1046 "src/sema.tr"
            return 202LL;
        }
#line 1047 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("sort", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("count", 5LL))))) {
#line 1047 "src/sema.tr"
            return 1LL;
        }
    } else if (_tr_str_eqv((fam), (_tr_str_lit_len("Dict", 4LL)))) {
#line 1049 "src/sema.tr"
        if ((((((((_tr_str_eqv((method), (_tr_str_lit_len("keys", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("values", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("items", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL))))) {
#line 1049 "src/sema.tr"
            return 0LL;
        }
#line 1050 "src/sema.tr"
        if (((((_tr_str_eqv((method), (_tr_str_lit_len("has", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("update", 6LL))))) {
#line 1050 "src/sema.tr"
            return 101LL;
        }
#line 1051 "src/sema.tr"
        if (((_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL))))) {
#line 1051 "src/sema.tr"
            return 202LL;
        }
#line 1052 "src/sema.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) {
#line 1052 "src/sema.tr"
            return 102LL;
        }
    }
#line 1053 "src/sema.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) void Sema_check_builtin_method_arity(Sema* self, TrStr fam, TrStr method, long long nargs) {
#line 1056 "src/sema.tr"
    if ((((!_tr_str_eqv((fam), (_tr_str_lit_len("str", 3LL)))) && (!_tr_str_eqv((fam), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((fam), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 1056 "src/sema.tr"
        return;
    }
#line 1057 "src/sema.tr"
    long long r = Sema_builtin_method_range(self, fam, method);
#line 1058 "src/sema.tr"
    if ((r < 0LL)) {
#line 1058 "src/sema.tr"
        return;
    }
#line 1059 "src/sema.tr"
    long long lo = _tr_idiv((long long)(r), (long long)(100LL));
#line 1060 "src/sema.tr"
    long long hi = _tr_imod((long long)(r), (long long)(100LL));
#line 1061 "src/sema.tr"
    if (((nargs >= lo) && (nargs <= hi))) {
#line 1061 "src/sema.tr"
        return;
    }
#line 1062 "src/sema.tr"
    TrStr pl = _tr_str_lit_len("s", 1LL);
#line 1063 "src/sema.tr"
    TrStr want_s = _tr_str_wrap(_tr_int_to_str((long long)(lo)));
#line 1064 "src/sema.tr"
    if ((lo != hi)) {
#line 1064 "src/sema.tr"
        TrStr _strtmp_t1076 = ({ __auto_type _ord_t1075 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(lo)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t1075); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        _tr_str_release(want_s);
        want_s = _strtmp_t1076;
    }
#line 1065 "src/sema.tr"
    if (((lo == 1LL) && (hi == 1LL))) {
#line 1065 "src/sema.tr"
        TrStr _strtmp_t1077 = _tr_str_lit_len("", 0LL);
        _tr_str_release(pl);
        pl = _strtmp_t1077;
    }
#line 1066 "src/sema.tr"
    TrStr vb = _tr_str_lit_len("were", 4LL);
#line 1067 "src/sema.tr"
    if ((nargs == 1LL)) {
#line 1067 "src/sema.tr"
        TrStr _strtmp_t1078 = _tr_str_lit_len("was", 3LL);
        _tr_str_release(vb);
        vb = _strtmp_t1078;
    }
#line 1068 "src/sema.tr"
    ({ TrStr _at_t1079 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (fam))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (want_s)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(nargs)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: see the built-in method list in the language docs.", 69LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1079); _tr_str_release(_at_t1079); });
    _tr_str_release(pl);
    _tr_str_release(want_s);
    _tr_str_release(vb);
}

__attribute__((hot)) bool Sema_class_has_field(Sema* self, TrStr cls_name, TrStr name) {
#line 1072 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
#line 1072 "src/sema.tr"
        return false;
    }
#line 1073 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
#line 1074 "src/sema.tr"
    long long fi = 0LL;
#line 1075 "src/sema.tr"
    while ((fi < cls->fields->len)) {
#line 1076 "src/sema.tr"
        if (_tr_str_eqv((((FieldDef*)List_ptr_get(cls->fields, fi))->name), (name))) {
#line 1076 "src/sema.tr"
            return true;
        }
#line 1077 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 1078 "src/sema.tr"
    long long bi = 0LL;
#line 1079 "src/sema.tr"
    while ((bi < cls->base_classes->len)) {
#line 1080 "src/sema.tr"
        if (({ TrStr _at_t1080 = (List_TrStr_get(cls->base_classes, bi)); __auto_type _wr = (Sema_class_has_field(self, _at_t1080, name)); _tr_str_release(_at_t1080); _wr; })) {
#line 1080 "src/sema.tr"
            return true;
        }
#line 1081 "src/sema.tr"
        bi = (bi + 1LL);
    }
#line 1082 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_rt_builtin_class(Sema* self, TrStr n) {
#line 1086 "src/sema.tr"
    return (((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncTask", 9LL))));
}

__attribute__((hot)) bool Sema_is_builtin_dispatch_class(Sema* self, TrStr n) {
#line 1089 "src/sema.tr"
    return (((((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("OS", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Process", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Env", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Hash", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("File", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncTask", 9LL))));
}

__attribute__((hot)) bool Sema_type_name_known(Sema* self, TrStr n) {
#line 1093 "src/sema.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((n), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("None", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Self", 4LL))))) {
#line 1093 "src/sema.tr"
        return true;
    }
#line 1094 "src/sema.tr"
    if (_is_builtin_type_word(n)) {
#line 1094 "src/sema.tr"
        return true;
    }
#line 1095 "src/sema.tr"
    if (Sema_is_known_name(self, n)) {
#line 1095 "src/sema.tr"
        return true;
    }
#line 1096 "src/sema.tr"
    if ((!_tr_str_eqv((Sema_resolve(self, n)->name), (_tr_str_lit_len("", 0LL))))) {
#line 1096 "src/sema.tr"
        return true;
    }
#line 1097 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema_check_type_known(Sema* self, AstType** tp, TrStr where_) {
#line 1100 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(tp)))) {
#line 1100 "src/sema.tr"
        return;
    }
#line 1101 "src/sema.tr"
    AstType* t = (*tp);
#line 1102 "src/sema.tr"
    if ((!Sema_type_name_known(self, t->name))) {
#line 1103 "src/sema.tr"
        ({ TrStr _at_t1081 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-4] Unknown type '", 20LL)), (t->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (in ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (where_)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").\n      FIX: check the spelling, or import it (e.g. `from std.x import ", 72LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("`).", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1081); _tr_str_release(_at_t1081); });
#line 1104 "src/sema.tr"
        return;
    }
#line 1105 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(t->args)))) {
#line 1105 "src/sema.tr"
        return;
    }
#line 1106 "src/sema.tr"
    long long ai = 0LL;
#line 1107 "src/sema.tr"
    while ((ai < t->args->len)) {
#line 1108 "src/sema.tr"
        if ((_tr_str_eqv(((*((AstType**)List_ptr_get(t->args, ai)))->name), (_tr_str_lit_len("void", 4LL))) && (((((_tr_str_eqv((t->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Option", 6LL)))))) {
#line 1109 "src/sema.tr"
            ({ TrStr _at_t1082 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-4] 'void' is not a valid element type for ", 45LL)), (t->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (in ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (where_)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").\n      FIX: name the element type, e.g. ", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[int].", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1082); _tr_str_release(_at_t1082); });
        }
#line 1110 "src/sema.tr"
        Sema_check_type_known(self, ((AstType**)List_ptr_get(t->args, ai)), where_);
#line 1111 "src/sema.tr"
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) void Sema_check_scalar_method(Sema* self, TrStr fam, TrStr method, TrStr ty_name) {
#line 1115 "src/sema.tr"
    if (Sema_class_method_exists(self, ty_name, method)) {
#line 1115 "src/sema.tr"
        return;
    }
#line 1116 "src/sema.tr"
    if ((_tr_str_eqv((fam), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fam), (_tr_str_lit_len("Dict", 4LL))))) {
#line 1117 "src/sema.tr"
        if (Sema_is_universal_method(self, method)) {
#line 1117 "src/sema.tr"
            return;
        }
#line 1118 "src/sema.tr"
        if (Sema_known_collection_method(self, fam, method)) {
#line 1118 "src/sema.tr"
            return;
        }
#line 1119 "src/sema.tr"
        ({ TrStr _at_t1083 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, fam)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-6] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" has no method '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'.\n      FIX: check the spelling, or that the receiver is the collection you meant.", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1083); _tr_str_release(_at_t1083); });
#line 1120 "src/sema.tr"
        return;
    }
#line 1121 "src/sema.tr"
    if ((_tr_str_eqv((fam), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((fam), (_tr_str_lit_len("float", 5LL))))) {
#line 1122 "src/sema.tr"
        if ((Sema_is_universal_method(self, method) || Sema_known_number_method(self, method))) {
#line 1122 "src/sema.tr"
            return;
        }
#line 1123 "src/sema.tr"
        ({ TrStr _at_t1084 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, fam)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-6] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" has no method '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'.\n      FIX: check that the receiver is the value you meant; numbers only have methods such as abs(), pow() and to_str().", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1084); _tr_str_release(_at_t1084); });
#line 1124 "src/sema.tr"
        return;
    }
#line 1125 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("None", 4LL)))) {
#line 1126 "src/sema.tr"
        ({ TrStr _at_t1085 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-6] None has no method '", 26LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'.\n      FIX: the variable holds None here; give it a real value, or use an Option[T] and check .is_some first.", 113LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1085); _tr_str_release(_at_t1085); });
#line 1127 "src/sema.tr"
        return;
    }
#line 1128 "src/sema.tr"
    if (((!_tr_str_eqv((fam), (_tr_str_lit_len("bool", 4LL)))) && (!_tr_str_eqv((fam), (_tr_str_lit_len("Tuple", 5LL)))))) {
#line 1128 "src/sema.tr"
        return;
    }
#line 1129 "src/sema.tr"
    if (Sema_is_universal_method(self, method)) {
#line 1129 "src/sema.tr"
        return;
    }
#line 1130 "src/sema.tr"
    if ((_tr_str_eqv((fam), (_tr_str_lit_len("Tuple", 5LL))) && (((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("last", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("count", 5LL)))))) {
#line 1130 "src/sema.tr"
        return;
    }
#line 1131 "src/sema.tr"
    ({ __auto_type _ord_t1086 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, fam)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-6] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" has no method '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'.\n      FIX: check that the receiver is the object you meant (this one is ", 77LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1087 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1086); TrStr _tr_cr = (Sema_family_word(self, fam)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1087); _tr_str_release(_at_t1087); });
}

__attribute__((hot)) void Sema_check_static_call(Sema* self, TrStr type_name, TrStr variant, List_ptr* hl) {
#line 1135 "src/sema.tr"
    if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Thread", 6LL))) && _tr_str_eqv((variant), (_tr_str_lit_len("spawn", 5LL)))) && (hl->len >= 1LL))) {
#line 1136 "src/sema.tr"
        __auto_type _t1088 = (*((HirExpr*)List_ptr_get(hl, 0LL)));
        if (_t1088.tag == HirExpr_EIdent) {
            __auto_type sfn = _t1088.data.EIdent.name;
#line 1138 "src/sema.tr"
            if ((_tr_dict_contains(self->fn_defs, _tr_strz(sfn)) && ({ __auto_type _ord_t1089 = (Sema_resolve(self, sfn)->kind); __auto_type _wr = ((_ord_t1089.tag == SymbolKind_make_SFunction().tag)); _wr; }))) {
#line 1139 "src/sema.tr"
                FunctionDef* sfd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(sfn)));
#line 1140 "src/sema.tr"
                long long sk = 0LL;
#line 1141 "src/sema.tr"
                while ((sk < sfd->params->len)) {
#line 1142 "src/sema.tr"
                    if (((Param*)List_ptr_get(sfd->params, sk))->is_variadic) {
#line 1142 "src/sema.tr"
                        return;
                    }
#line 1143 "src/sema.tr"
                    sk = (sk + 1LL);
                }
#line 1144 "src/sema.tr"
                if ((sfd->params->len != (hl->len - 1LL))) {
#line 1145 "src/sema.tr"
                    ({ __auto_type _ord_t1090 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] 'Thread.spawn(", 20LL)), (sfn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ...)' passes ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((hl->len - 1LL))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument(s) to '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sfn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()', which takes ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1091 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1090); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(sfd->params->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: Thread.spawn(f, arg1, arg2, ...) passes exactly the arguments f declares.", 86LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1091); _tr_str_release(_at_t1091); });
                }
            }
        } else if (1) {
            __auto_type _ = _t1088;
            /* pass */
        }
    }
#line 1147 "src/sema.tr"
    if ((_tr_str_eqv((type_name), (_tr_str_lit_len("Atomic", 6LL))) && (_tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL)))))) {
#line 1148 "src/sema.tr"
        if ((hl->len != 1LL)) {
#line 1149 "src/sema.tr"
            ({ TrStr _at_t1092 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] 'Atomic.", 14LL)), (variant))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes 1 argument but ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" were given.\n      FIX: Atomic.new(initial_value).", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1092); _tr_str_release(_at_t1092); });
        } else {
#line 1151 "src/sema.tr"
            TrStr af = Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)));
#line 1152 "src/sema.tr"
            if (((_tr_str_eqv((af), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((af), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((af), (_tr_str_lit_len("Dict", 4LL))))) {
#line 1153 "src/sema.tr"
                ({ TrStr _at_t1093 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, af)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: Atomic needs a number as its initial value but is given ", 77LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: Atomic.new(0).", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1093); _tr_str_release(_at_t1093); });
            }
        }
    }
#line 1154 "src/sema.tr"
    if ((_tr_dict_contains(self->ambiguous_classes, _tr_strz(type_name)) || Sema_is_rt_builtin_class(self, type_name))) {
#line 1154 "src/sema.tr"
        return;
    }
#line 1155 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(type_name)))) {
#line 1155 "src/sema.tr"
        return;
    }
#line 1156 "src/sema.tr"
    ClassDef* cdef = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(type_name)));
#line 1157 "src/sema.tr"
    long long named = 0LL;
#line 1158 "src/sema.tr"
    bool ok = false;
#line 1159 "src/sema.tr"
    long long want = 0LL;
#line 1160 "src/sema.tr"
    bool variadic = false;
#line 1161 "src/sema.tr"
    FunctionDef* one = FunctionDef_init(_tr_str_lit_len("", 0LL));
#line 1162 "src/sema.tr"
    long long i = 0LL;
#line 1163 "src/sema.tr"
    while ((i < cdef->methods->len)) {
#line 1164 "src/sema.tr"
        FunctionDef* m = ((FunctionDef*)List_ptr_get(cdef->methods, i));
#line 1165 "src/sema.tr"
        if (_tr_str_eqv((m->name), (variant))) {
#line 1166 "src/sema.tr"
            bool has_self = ((m->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(m->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))));
#line 1167 "src/sema.tr"
            long long cnt = m->params->len;
#line 1168 "src/sema.tr"
            if (has_self) {
#line 1168 "src/sema.tr"
                cnt = (cnt - 1LL);
            }
#line 1169 "src/sema.tr"
            long long pj = 0LL;
#line 1170 "src/sema.tr"
            while ((pj < m->params->len)) {
#line 1171 "src/sema.tr"
                if (((Param*)List_ptr_get(m->params, pj))->is_variadic) {
#line 1171 "src/sema.tr"
                    variadic = true;
                }
#line 1172 "src/sema.tr"
                pj = (pj + 1LL);
            }
#line 1173 "src/sema.tr"
            if ((named == 0LL)) {
#line 1173 "src/sema.tr"
                want = cnt;
            }
#line 1174 "src/sema.tr"
            named = (named + 1LL);
#line 1175 "src/sema.tr"
            if (((cnt == hl->len) || (has_self && ((cnt + 1LL) == hl->len)))) {
#line 1175 "src/sema.tr"
                ok = true;
            }
#line 1176 "src/sema.tr"
            FunctionDef* _cltmp_t1094 = _tr_obj_retain(m);
            _tr_obj_release(one, _trdrop_FunctionDef);
            one = _cltmp_t1094;
        }
#line 1177 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1178 "src/sema.tr"
    if (((named == 0LL) || variadic)) {
#line 1178 "src/sema.tr"
        _tr_obj_release(one, _trdrop_FunctionDef);
        return;
    }
#line 1179 "src/sema.tr"
    if ((!ok)) {
#line 1180 "src/sema.tr"
        TrStr pl = _tr_str_lit_len("s", 1LL);
#line 1181 "src/sema.tr"
        if ((want == 1LL)) {
#line 1181 "src/sema.tr"
            TrStr _strtmp_t1095 = _tr_str_lit_len("", 0LL);
            _tr_str_release(pl);
            pl = _strtmp_t1095;
        }
#line 1182 "src/sema.tr"
        TrStr vb = _tr_str_lit_len("were", 4LL);
#line 1183 "src/sema.tr"
        if ((hl->len == 1LL)) {
#line 1183 "src/sema.tr"
            TrStr _strtmp_t1096 = _tr_str_lit_len("was", 3LL);
            _tr_str_release(vb);
            vb = _strtmp_t1096;
        }
#line 1184 "src/sema.tr"
        ({ __auto_type _ord_t1097 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (type_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (variant)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1098 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1097); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1099 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1098); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (see its definition).", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1099); _tr_str_release(_at_t1099); });
    } else if (((((named == 1LL) && (cdef->generics->len == 0LL)) && (one->generics->len == 0LL)) && (one->params->len == hl->len))) {
#line 1186 "src/sema.tr"
        long long ti = 0LL;
#line 1187 "src/sema.tr"
        while ((ti < hl->len)) {
#line 1188 "src/sema.tr"
            if ((((unsigned long long)(((Param*)List_ptr_get(one->params, ti))->ty)) != ((unsigned long long)(0LL)))) {
#line 1189 "src/sema.tr"
                ({ __auto_type _ord_t1100 = ((*((Param*)List_ptr_get(one->params, ti))->ty)); __auto_type _ord_t1101 = (((HirExpr*)List_ptr_get(hl, ti))); TrStr _ord_t1102 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((ti + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (type_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (variant)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1100, _ord_t1101, _ord_t1102); _tr_str_release(_ord_t1102); });
            }
#line 1190 "src/sema.tr"
            ti = (ti + 1LL);
        }
    }
    _tr_obj_release(one, _trdrop_FunctionDef);
}

__attribute__((hot)) HirExpr* Sema_lower_index_operand(Sema* self, Expr* idx, HirExpr* base) {
#line 1194 "src/sema.tr"
    bool sv_a = self->in_assign_target;
#line 1195 "src/sema.tr"
    bool sv_p = self->in_assign_target_plain;
#line 1196 "src/sema.tr"
    self->in_assign_target = false;
#line 1197 "src/sema.tr"
    self->in_assign_target_plain = false;
#line 1198 "src/sema.tr"
    HirExpr* r = Sema_lower_expr(self, idx);
#line 1199 "src/sema.tr"
    self->in_assign_target = sv_a;
#line 1200 "src/sema.tr"
    self->in_assign_target_plain = sv_p;
#line 1201 "src/sema.tr"
    if (true) {
#line 1202 "src/sema.tr"
        TrStr bf = Sema_firm_family(self, base);
#line 1203 "src/sema.tr"
        TrStr xf = Sema_firm_family(self, r);
#line 1204 "src/sema.tr"
        AstType* bty = hir_expr_type(base);
#line 1205 "src/sema.tr"
        if ((((_tr_str_eqv((bty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((bty->name), (_tr_str_lit_len("Map", 3LL)))) && (bty->args->len > 0LL)) && ({ TrStr _at_t1103 = (Sema_decl_family(self, (*((AstType**)List_ptr_get(bty->args, 0LL))))); __auto_type _wr = (Sema_family_mismatch(self, _at_t1103, xf)); _tr_str_release(_at_t1103); _wr; }))) {
#line 1206 "src/sema.tr"
            ({ __auto_type _ord_t1104 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: a ", 23LL)), (bty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1105 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1104); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(bty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ...] is indexed with ", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1106 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1105); TrStr _tr_cr = (Sema_family_word(self, xf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: use a '", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1107 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1106); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(bty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' key.", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1107); _tr_str_release(_at_t1107); });
        }
#line 1207 "src/sema.tr"
        if ((((_tr_str_eqv((bf), (_tr_str_lit_len("None", 4LL))) || _tr_str_eqv((bf), (_tr_str_lit_len("int", 3LL)))) || _tr_str_eqv((bf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((bf), (_tr_str_lit_len("bool", 4LL))))) {
#line 1208 "src/sema.tr"
            ({ __auto_type _ord_t1108 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, bf)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: ", 21LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" cannot be indexed.\n      FIX: check that the variable holds the list / string / dict you meant (it holds ", 106LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1109 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1108); TrStr _tr_cr = (Sema_family_word(self, bf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" here).", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1109); _tr_str_release(_at_t1109); });
        }
#line 1209 "src/sema.tr"
        if (((_tr_str_eqv((bf), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((bf), (_tr_str_lit_len("str", 3LL)))) && _tr_str_eqv((xf), (_tr_str_lit_len("float", 5LL))))) {
#line 1210 "src/sema.tr"
            ({ TrStr _at_t1110 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: a ", 23LL)), (bf))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is indexed with a float, but the index must be an int.\n      FIX: convert it: int(x).", 86LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1110); _tr_str_release(_at_t1110); });
        }
#line 1211 "src/sema.tr"
        if (((_tr_str_eqv((bf), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((bf), (_tr_str_lit_len("str", 3LL)))) && ((_tr_str_eqv((xf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((xf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((xf), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 1212 "src/sema.tr"
            ({ TrStr _at_t1111 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: a ", 23LL)), (bf))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is indexed with ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, xf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", but the index must be an int.\n      FIX: use an integer index (for a dict, declare it as a Dict).", 99LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1111); _tr_str_release(_at_t1111); });
        }
    }
#line 1213 "src/sema.tr"
    return r;
}

__attribute__((hot)) bool Sema_known_collection_method(Sema* self, TrStr fam, TrStr m) {
#line 1218 "src/sema.tr"
    if (((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("add", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("all", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("any", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clone", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("copy", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("count", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("difference", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))))) {
#line 1218 "src/sema.tr"
        return true;
    }
#line 1219 "src/sema.tr"
    if ((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("has", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("index_of", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("init", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("intersection", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("is_empty", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("is_subset", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("join", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("last", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("len", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("length", 6LL))))) {
#line 1219 "src/sema.tr"
        return true;
    }
#line 1220 "src/sema.tr"
    if ((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("max", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("max_val", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("min_val", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reserve", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reverse", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reversed", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reversed_copy", 13LL))))) {
#line 1220 "src/sema.tr"
        return true;
    }
#line 1221 "src/sema.tr"
    if ((((((((((_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("slice", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort_asc", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort_by", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort_desc", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sum", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("swap", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_list", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("union", 5LL))))) {
#line 1221 "src/sema.tr"
        return true;
    }
#line 1222 "src/sema.tr"
    if ((((((_tr_str_eqv((m), (_tr_str_lit_len("get_index", 9LL))) || _tr_str_eqv((m), (_tr_str_lit_len("get_or", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("items", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("keys", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("values", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("update", 6LL))))) {
#line 1222 "src/sema.tr"
        return true;
    }
#line 1223 "src/sema.tr"
    if (((((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("map", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("filter", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("each", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("for_each", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fold", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reduce", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("find", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sorted", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("enumerate", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("zip", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("take", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("skip", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("flat_map", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("find_index", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("position", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("partition", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("group_by", 8LL))))) {
#line 1223 "src/sema.tr"
        return true;
    }
#line 1224 "src/sema.tr"
    return false;
}

__attribute__((hot)) long long Sema_builtin_class_method_arity(Sema* self, TrStr cls, TrStr m) {
#line 1229 "src/sema.tr"
    if (_tr_str_eqv((cls), (_tr_str_lit_len("Chan", 4LL)))) {
#line 1230 "src/sema.tr"
        if (((((((_tr_str_eqv((m), (_tr_str_lit_len("cap", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("close", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("is_closed", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("len", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("recv", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("try_recv", 8LL))))) {
#line 1230 "src/sema.tr"
            return 0LL;
        }
#line 1231 "src/sema.tr"
        if (((_tr_str_eqv((m), (_tr_str_lit_len("recv_timeout", 12LL))) || _tr_str_eqv((m), (_tr_str_lit_len("send", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("try_send", 8LL))))) {
#line 1231 "src/sema.tr"
            return 1LL;
        }
#line 1232 "src/sema.tr"
        if (_tr_str_eqv((m), (_tr_str_lit_len("send_timeout", 12LL)))) {
#line 1232 "src/sema.tr"
            return 2LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 1234 "src/sema.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("get", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("lock", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("unlock", 6LL))))) {
#line 1234 "src/sema.tr"
            return 0LL;
        }
#line 1235 "src/sema.tr"
        if (_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) {
#line 1235 "src/sema.tr"
            return 1LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 1237 "src/sema.tr"
        if (((((((_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("read", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("read_lock", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("read_unlock", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write_lock", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write_unlock", 12LL))))) {
#line 1237 "src/sema.tr"
            return 0LL;
        }
#line 1238 "src/sema.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL))))) {
#line 1238 "src/sema.tr"
            return 1LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("ThreadLocal", 11LL)))) {
#line 1240 "src/sema.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("get", 3LL))))) {
#line 1240 "src/sema.tr"
            return 0LL;
        }
#line 1241 "src/sema.tr"
        if (_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) {
#line 1241 "src/sema.tr"
            return 1LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("Weak", 4LL)))) {
#line 1243 "src/sema.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("drop", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("is_alive", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("upgrade", 7LL))))) {
#line 1243 "src/sema.tr"
            return 0LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("Thread", 6LL)))) {
#line 1245 "src/sema.tr"
        if (((((_tr_str_eqv((m), (_tr_str_lit_len("join", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("detach", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("id", 2LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("current_id", 10LL))))) {
#line 1245 "src/sema.tr"
            return 0LL;
        }
#line 1246 "src/sema.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("sleep", 5LL))) || _tr_str_eqv((m), (_tr_str_lit_len("sleep_ms", 8LL))))) {
#line 1246 "src/sema.tr"
            return 1LL;
        }
#line 1247 "src/sema.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL))) || _tr_str_eqv((m), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("init", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("auto", 4LL))))) {
#line 1247 "src/sema.tr"
            return 99LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("Atomic", 6LL)))) {
#line 1249 "src/sema.tr"
        if ((((((((_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("get", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("increment", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("decrement", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("load", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("load_acquire", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("load_relaxed", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("load_seqcst", 11LL))))) {
#line 1249 "src/sema.tr"
            return 0LL;
        }
#line 1250 "src/sema.tr"
        if ((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("add", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("add_acqrel", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add_relaxed", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add_release", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("exchange", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_add", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_add_acqrel", 16LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_add_relaxed", 17LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_add_release", 17LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_sub", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_sub_relaxed", 17LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_sub_release", 17LL))))) {
#line 1250 "src/sema.tr"
            return 1LL;
        }
#line 1251 "src/sema.tr"
        if (((((((((_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("store", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("store_relaxed", 13LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("store_release", 13LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("store_seqcst", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub_relaxed", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub_release", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("swap", 4LL))))) {
#line 1251 "src/sema.tr"
            return 1LL;
        }
#line 1252 "src/sema.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("cas", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("cas_acqrel", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("cas_weak", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("compare_exchange", 16LL))))) {
#line 1252 "src/sema.tr"
            return 2LL;
        }
    }
#line 1253 "src/sema.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) void Sema_check_builtin_class_method(Sema* self, TrStr cls, TrStr method, List_ptr* hl) {
#line 1257 "src/sema.tr"
    if (_tr_str_eqv((cls), (_tr_str_lit_len("StringBuilder", 13LL)))) {
#line 1258 "src/sema.tr"
        if ((!(((((((((Sema_class_method_exists(self, cls, method) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("append_char", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("init", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("as_str", 6LL)))) || Sema_is_universal_method(self, method)))) {
#line 1259 "src/sema.tr"
            ({ TrStr _at_t1112 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] No method '", 17LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' found on type 'StringBuilder'.\n      FIX: check the spelling; see the StringBuilder methods in docs/std/string.md.", 116LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1112); _tr_str_release(_at_t1112); });
        }
#line 1260 "src/sema.tr"
        return;
    }
#line 1261 "src/sema.tr"
    if (_tr_str_eqv((cls), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 1262 "src/sema.tr"
        if ((!(((((((((((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("offset", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cast", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_int", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("addr", 4LL)))) || Sema_is_universal_method(self, method)) || Sema_class_method_exists(self, _tr_str_lit_len("Pointer", 7LL), method)))) {
#line 1263 "src/sema.tr"
            ({ TrStr _at_t1113 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] No method '", 17LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' found on type 'Pointer'.\n      FIX: check the spelling; pointers support .read(), .write(v) and .offset(n) (inside unsafe:).", 126LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1113); _tr_str_release(_at_t1113); });
        }
#line 1264 "src/sema.tr"
        return;
    }
#line 1265 "src/sema.tr"
    if ((((((((!_tr_str_eqv((cls), (_tr_str_lit_len("Chan", 4LL)))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("Mutex", 5LL))))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("RwLock", 6LL))))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("ThreadLocal", 11LL))))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("Weak", 4LL))))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("Atomic", 6LL))))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("Thread", 6LL)))))) {
#line 1265 "src/sema.tr"
        return;
    }
#line 1266 "src/sema.tr"
    if ((_tr_dict_contains(self->classes, _tr_strz(cls)) && Sema_class_method_exists(self, cls, method))) {
#line 1266 "src/sema.tr"
        return;
    }
#line 1267 "src/sema.tr"
    if ((Sema_is_universal_method(self, method) && (!_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))))) {
#line 1267 "src/sema.tr"
        return;
    }
#line 1268 "src/sema.tr"
    long long want = Sema_builtin_class_method_arity(self, cls, method);
#line 1269 "src/sema.tr"
    if ((want < 0LL)) {
#line 1270 "src/sema.tr"
        ({ TrStr _at_t1114 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] No method '", 17LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' found on type '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cls)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: check the spelling; see the ", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cls)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" method list in docs/lang/16_concurrency.md.", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1114); _tr_str_release(_at_t1114); });
#line 1271 "src/sema.tr"
        return;
    }
#line 1272 "src/sema.tr"
    if ((want == 99LL)) {
#line 1272 "src/sema.tr"
        return;
    }
#line 1273 "src/sema.tr"
    if ((want != hl->len)) {
#line 1274 "src/sema.tr"
        TrStr pl = _tr_str_lit_len("s", 1LL);
#line 1275 "src/sema.tr"
        if ((want == 1LL)) {
#line 1275 "src/sema.tr"
            TrStr _strtmp_t1115 = _tr_str_lit_len("", 0LL);
            _tr_str_release(pl);
            pl = _strtmp_t1115;
        }
#line 1276 "src/sema.tr"
        TrStr vb = _tr_str_lit_len("were", 4LL);
#line 1277 "src/sema.tr"
        if ((hl->len == 1LL)) {
#line 1277 "src/sema.tr"
            TrStr _strtmp_t1116 = _tr_str_lit_len("was", 3LL);
            _tr_str_release(vb);
            vb = _strtmp_t1116;
        }
#line 1278 "src/sema.tr"
        ({ __auto_type _ord_t1117 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (cls))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1118 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1117); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1119 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1118); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1119); _tr_str_release(_at_t1119); });
#line 1279 "src/sema.tr"
        _tr_str_release(pl);
        _tr_str_release(vb);
        return;
    }
#line 1280 "src/sema.tr"
    if (_tr_str_eqv((cls), (_tr_str_lit_len("Atomic", 6LL)))) {
#line 1281 "src/sema.tr"
        long long ai = 0LL;
#line 1282 "src/sema.tr"
        while ((ai < hl->len)) {
#line 1283 "src/sema.tr"
            if (((({ TrStr _cmpa_t1120 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, ai)))); TrStr _cmpb_t1121 = (_tr_str_lit_len("str", 3LL)); _Bool _cmpr_t1122 = _tr_str_eqv(_cmpa_t1120, _cmpb_t1121); _tr_str_release(_cmpa_t1120); _cmpr_t1122; }) || ({ TrStr _cmpa_t1123 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, ai)))); TrStr _cmpb_t1124 = (_tr_str_lit_len("List", 4LL)); _Bool _cmpr_t1125 = _tr_str_eqv(_cmpa_t1123, _cmpb_t1124); _tr_str_release(_cmpa_t1123); _cmpr_t1125; })) || ({ TrStr _cmpa_t1126 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, ai)))); TrStr _cmpb_t1127 = (_tr_str_lit_len("Dict", 4LL)); _Bool _cmpr_t1128 = _tr_str_eqv(_cmpa_t1126, _cmpb_t1127); _tr_str_release(_cmpa_t1126); _cmpr_t1128; }))) {
#line 1284 "src/sema.tr"
                ({ __auto_type _ord_t1130 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((ai + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: argument ", 30LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of 'Atomic.", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' must be a number but is ", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1129 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, ai)))); TrStr _at_t1131 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1130); TrStr _tr_cr = (Sema_family_word(self, _at_t1129)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass an int.", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1131); _tr_str_release(_at_t1129); _tr_str_release(_at_t1131); });
            }
#line 1285 "src/sema.tr"
            ai = (ai + 1LL);
        }
    }
}

__attribute__((hot)) void Sema_check_user_method_call(Sema* self, TrStr cls, Expr* obj, TrStr method, List_ptr* hl) {
#line 1290 "src/sema.tr"
    if ((((!_tr_dict_contains(self->classes, _tr_strz(cls))) || _tr_dict_contains(self->ambiguous_classes, _tr_strz(cls))) || Sema_is_builtin_dispatch_class(self, cls))) {
#line 1290 "src/sema.tr"
        return;
    }
#line 1291 "src/sema.tr"
    if (((((_tr_str_eqv((cls), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((cls), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((cls), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((cls), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((cls), (_tr_str_lit_len("Set", 3LL))))) {
#line 1291 "src/sema.tr"
        return;
    }
#line 1292 "src/sema.tr"
    __auto_type _t1132 = (*obj);
    if (_t1132.tag == Expr_EIdent) {
        __auto_type _ru_n = _t1132.data.EIdent.name;
#line 1294 "src/sema.tr"
        if ((_tr_dict_contains(self->classes, _tr_strz(_ru_n)) && ({ __auto_type _ord_t1133 = (Sema_resolve(self, _ru_n)->kind); __auto_type _wr = ((_ord_t1133.tag != SymbolKind_make_SVariable().tag)); _wr; }))) {
#line 1294 "src/sema.tr"
            return;
        }
    } else if (1) {
        __auto_type _ = _t1132;
        /* pass */
    }
#line 1296 "src/sema.tr"
    ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls)));
#line 1297 "src/sema.tr"
    long long named = 0LL;
#line 1298 "src/sema.tr"
    bool ok = false;
#line 1299 "src/sema.tr"
    long long want = 0LL;
#line 1300 "src/sema.tr"
    FunctionDef* one = FunctionDef_init(_tr_str_lit_len("", 0LL));
#line 1301 "src/sema.tr"
    long long i = 0LL;
#line 1302 "src/sema.tr"
    while ((i < cd->methods->len)) {
#line 1303 "src/sema.tr"
        FunctionDef* m = ((FunctionDef*)List_ptr_get(cd->methods, i));
#line 1304 "src/sema.tr"
        if (_tr_str_eqv((m->name), (method))) {
#line 1305 "src/sema.tr"
            long long cnt = 0LL;
#line 1306 "src/sema.tr"
            long long pj = 0LL;
#line 1307 "src/sema.tr"
            while ((pj < m->params->len)) {
#line 1308 "src/sema.tr"
                if ((!_tr_str_eqv((((Param*)List_ptr_get(m->params, pj))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 1308 "src/sema.tr"
                    cnt = (cnt + 1LL);
                }
#line 1309 "src/sema.tr"
                if (((Param*)List_ptr_get(m->params, pj))->is_variadic) {
#line 1309 "src/sema.tr"
                    _tr_obj_release(one, _trdrop_FunctionDef);
                    return;
                }
#line 1310 "src/sema.tr"
                pj = (pj + 1LL);
            }
#line 1311 "src/sema.tr"
            if ((named == 0LL)) {
#line 1311 "src/sema.tr"
                want = cnt;
            }
#line 1312 "src/sema.tr"
            named = (named + 1LL);
#line 1313 "src/sema.tr"
            if ((cnt == hl->len)) {
#line 1313 "src/sema.tr"
                ok = true;
            }
#line 1314 "src/sema.tr"
            FunctionDef* _cltmp_t1134 = _tr_obj_retain(m);
            _tr_obj_release(one, _trdrop_FunctionDef);
            one = _cltmp_t1134;
        }
#line 1315 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1316 "src/sema.tr"
    if ((named == 0LL)) {
#line 1316 "src/sema.tr"
        _tr_obj_release(one, _trdrop_FunctionDef);
        return;
    }
#line 1317 "src/sema.tr"
    if ((!ok)) {
#line 1318 "src/sema.tr"
        TrStr pl = _tr_str_lit_len("s", 1LL);
#line 1319 "src/sema.tr"
        if ((want == 1LL)) {
#line 1319 "src/sema.tr"
            TrStr _strtmp_t1135 = _tr_str_lit_len("", 0LL);
            _tr_str_release(pl);
            pl = _strtmp_t1135;
        }
#line 1320 "src/sema.tr"
        TrStr vb = _tr_str_lit_len("were", 4LL);
#line 1321 "src/sema.tr"
        if ((hl->len == 1LL)) {
#line 1321 "src/sema.tr"
            TrStr _strtmp_t1136 = _tr_str_lit_len("was", 3LL);
            _tr_str_release(vb);
            vb = _strtmp_t1136;
        }
#line 1322 "src/sema.tr"
        ({ __auto_type _ord_t1137 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (cls))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1138 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1137); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1139 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1138); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (see its definition).", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1139); _tr_str_release(_at_t1139); });
    } else if ((((named == 1LL) && (cd->generics->len == 0LL)) && (one->generics->len == 0LL))) {
#line 1324 "src/sema.tr"
        long long sk = 0LL;
#line 1325 "src/sema.tr"
        if (((one->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(one->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 1325 "src/sema.tr"
            sk = 1LL;
        }
#line 1326 "src/sema.tr"
        long long ti = 0LL;
#line 1327 "src/sema.tr"
        while (((ti < hl->len) && ((sk + ti) < one->params->len))) {
#line 1328 "src/sema.tr"
            if ((((unsigned long long)(((Param*)List_ptr_get(one->params, (sk + ti)))->ty)) != ((unsigned long long)(0LL)))) {
#line 1329 "src/sema.tr"
                ({ __auto_type _ord_t1140 = ((*((Param*)List_ptr_get(one->params, (sk + ti)))->ty)); __auto_type _ord_t1141 = (((HirExpr*)List_ptr_get(hl, ti))); TrStr _ord_t1142 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((ti + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cls)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1140, _ord_t1141, _ord_t1142); _tr_str_release(_ord_t1142); });
            }
#line 1330 "src/sema.tr"
            ti = (ti + 1LL);
        }
    }
    _tr_obj_release(one, _trdrop_FunctionDef);
}

__attribute__((hot)) bool Sema_known_number_method(Sema* self, TrStr m) {
#line 1334 "src/sema.tr"
    if ((((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("acos", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("as_str", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("asin", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("atan", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("atan2", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("ceil", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("checked_add", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("checked_mul", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("checked_sub", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clamp", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("cos", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("exp", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fabs", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("floor", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("gcd", 3LL))))) {
#line 1334 "src/sema.tr"
        return true;
    }
#line 1335 "src/sema.tr"
    if ((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("is_inf", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("is_nan", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("lcm", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("log", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("log10", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("log2", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pow", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("round", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sign", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sin", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sqrt", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("tan", 3LL))))) {
#line 1335 "src/sema.tr"
        return true;
    }
#line 1336 "src/sema.tr"
    if (((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("to_HEX", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("to_bin", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_binary", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_f64", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_float", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_hex", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_hex_upper", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_i64", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_int", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_oct", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_octal", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_str", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_string", 9LL))))) {
#line 1336 "src/sema.tr"
        return true;
    }
#line 1337 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema_check_collection_arg_types(Sema* self, AstType* ty, TrStr method, List_ptr* hl) {
#line 1342 "src/sema.tr"
    if (((_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))) && _tr_str_eqv((method), (_tr_str_lit_len("offset", 6LL)))) && (hl->len == 1LL))) {
#line 1343 "src/sema.tr"
        TrStr pf = Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)));
#line 1344 "src/sema.tr"
        if (((_tr_str_eqv((pf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((pf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((pf), (_tr_str_lit_len("Dict", 4LL))))) {
#line 1345 "src/sema.tr"
            ({ TrStr _at_t1143 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, pf)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: Pointer.offset() needs an int offset but is given ", 71LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass an integer element count.", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1143); _tr_str_release(_at_t1143); });
        }
#line 1346 "src/sema.tr"
        _tr_str_release(pf);
        return;
    }
#line 1347 "src/sema.tr"
    if ((((_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Set", 3LL)))) && (ty->args->len > 0LL))) {
#line 1348 "src/sema.tr"
        TrStr ef = Sema_decl_family(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
#line 1349 "src/sema.tr"
        long long vi = (0LL - 1LL);
#line 1350 "src/sema.tr"
        if (((((_tr_str_eqv((method), (_tr_str_lit_len("append", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("push", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))))) {
#line 1350 "src/sema.tr"
            vi = 0LL;
        }
#line 1351 "src/sema.tr"
        if (((_tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) && (hl->len == 2LL))) {
#line 1352 "src/sema.tr"
            vi = 1LL;
#line 1353 "src/sema.tr"
            TrStr xf = Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)));
#line 1354 "src/sema.tr"
            if (((_tr_str_eqv((xf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((xf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((xf), (_tr_str_lit_len("Dict", 4LL))))) {
#line 1355 "src/sema.tr"
                ({ __auto_type _ord_t1144 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the index of ", 34LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() must be an int but is ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1145 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1144); TrStr _tr_cr = (Sema_family_word(self, xf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass an integer position.", 38LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1145); _tr_str_release(_at_t1145); });
            }
        }
#line 1356 "src/sema.tr"
        if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("Set", 3LL))) && _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL))))) {
#line 1356 "src/sema.tr"
            vi = 0LL;
        }
#line 1357 "src/sema.tr"
        if ((((vi >= 0LL) && (vi < hl->len)) && ({ TrStr _at_t1146 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, vi)))); __auto_type _wr = (Sema_family_mismatch(self, ef, _at_t1146)); _tr_str_release(_at_t1146); _wr; }))) {
#line 1358 "src/sema.tr"
            ({ __auto_type _ord_t1147 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: ", 21LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1149 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1147); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("].", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() is given ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1148 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, vi)))); __auto_type _ord_t1150 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1149); TrStr _tr_cr = (Sema_family_word(self, _at_t1148)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass a '", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1151 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1150); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (convert it first if needed).", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1151); _tr_str_release(_at_t1148); _tr_str_release(_at_t1151); });
        }
    } else if (((_tr_str_eqv((ty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Map", 3LL)))) && (ty->args->len > 1LL))) {
#line 1360 "src/sema.tr"
        TrStr kf = Sema_decl_family(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
#line 1361 "src/sema.tr"
        TrStr vf = Sema_decl_family(self, (*((AstType**)List_ptr_get(ty->args, 1LL))));
#line 1362 "src/sema.tr"
        if ((((((((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) && (hl->len >= 1LL))) {
#line 1363 "src/sema.tr"
            if (({ TrStr _at_t1152 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)))); __auto_type _wr = (Sema_family_mismatch(self, kf, _at_t1152)); _tr_str_release(_at_t1152); _wr; })) {
#line 1364 "src/sema.tr"
                ({ __auto_type _ord_t1153 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the key of ", 32LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1154 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1153); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1156 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1154); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 1LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("].", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() is ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1155 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)))); __auto_type _ord_t1157 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1156); TrStr _tr_cr = (Sema_family_word(self, _at_t1155)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: use a '", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1158 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1157); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' key.", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1158); _tr_str_release(_at_t1155); _tr_str_release(_at_t1158); });
            }
        }
#line 1365 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL)))) && (hl->len == 2LL))) {
#line 1366 "src/sema.tr"
            if (({ TrStr _at_t1159 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 1LL)))); __auto_type _wr = (Sema_family_mismatch(self, vf, _at_t1159)); _tr_str_release(_at_t1159); _wr; })) {
#line 1367 "src/sema.tr"
                ({ __auto_type _ord_t1160 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the value of ", 34LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1161 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1160); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1163 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1161); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 1LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("].", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() is ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1162 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 1LL)))); __auto_type _ord_t1164 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1163); TrStr _tr_cr = (Sema_family_word(self, _at_t1162)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: use a '", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1165 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1164); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 1LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' value.", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1165); _tr_str_release(_at_t1162); _tr_str_release(_at_t1165); });
            }
        }
    }
}

__attribute__((hot)) bool Sema_class_method_exists(Sema* self, TrStr cls_name, TrStr method) {
#line 1371 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
#line 1371 "src/sema.tr"
        return false;
    }
#line 1372 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
#line 1373 "src/sema.tr"
    long long mi = 0LL;
#line 1374 "src/sema.tr"
    while ((mi < cls->methods->len)) {
#line 1375 "src/sema.tr"
        if (_tr_str_eqv((((FunctionDef*)List_ptr_get(cls->methods, mi))->name), (method))) {
#line 1375 "src/sema.tr"
            return true;
        }
#line 1376 "src/sema.tr"
        mi = (mi + 1LL);
    }
#line 1377 "src/sema.tr"
    long long bi = 0LL;
#line 1378 "src/sema.tr"
    while ((bi < cls->base_classes->len)) {
#line 1379 "src/sema.tr"
        if (({ TrStr _at_t1166 = (List_TrStr_get(cls->base_classes, bi)); __auto_type _wr = (Sema_class_method_exists(self, _at_t1166, method)); _tr_str_release(_at_t1166); _wr; })) {
#line 1379 "src/sema.tr"
            return true;
        }
#line 1380 "src/sema.tr"
        bi = (bi + 1LL);
    }
#line 1381 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_universal_method(Sema* self, TrStr method) {
#line 1386 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("free", 4LL))))) {
#line 1386 "src/sema.tr"
        return true;
    }
#line 1387 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("as_str", 6LL))))) {
#line 1387 "src/sema.tr"
        return true;
    }
#line 1388 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__len__", 7LL))))) {
#line 1388 "src/sema.tr"
        return true;
    }
#line 1389 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL))))) {
#line 1389 "src/sema.tr"
        return true;
    }
#line 1390 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("__getitem__", 11LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__setitem__", 11LL))))) {
#line 1390 "src/sema.tr"
        return true;
    }
#line 1391 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("__eq__", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__ne__", 6LL))))) {
#line 1391 "src/sema.tr"
        return true;
    }
#line 1392 "src/sema.tr"
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("__lt__", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__gt__", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__le__", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__ge__", 6LL))))) {
#line 1392 "src/sema.tr"
        return true;
    }
#line 1393 "src/sema.tr"
    if (((((_tr_str_eqv((method), (_tr_str_lit_len("__add__", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__sub__", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__mul__", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__div__", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__mod__", 7LL))))) {
#line 1393 "src/sema.tr"
        return true;
    }
#line 1394 "src/sema.tr"
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("__hash__", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__iter__", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__next__", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__contains__", 12LL))))) {
#line 1394 "src/sema.tr"
        return true;
    }
#line 1395 "src/sema.tr"
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("__str__", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__repr__", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__enter__", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__exit__", 8LL))))) {
#line 1395 "src/sema.tr"
        return true;
    }
#line 1396 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_expr_is_borrow(Sema* self, HirExpr* e) {
#line 1402 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1402 "src/sema.tr"
        return false;
    }
#line 1403 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 1403 "src/sema.tr"
        return false;
    }
#line 1404 "src/sema.tr"
    if (hir_expr_type(e)->is_borrow) {
#line 1404 "src/sema.tr"
        return true;
    }
#line 1405 "src/sema.tr"
    __auto_type _t1167 = (*e);
    if (_t1167.tag == HirExpr_EIdent) {
        __auto_type nm = _t1167.data.EIdent.name;
#line 1407 "src/sema.tr"
        Symbol* sym = Sema_resolve(self, nm);
#line 1408 "src/sema.tr"
        if (((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL))))) {
#line 1409 "src/sema.tr"
            if ((*sym->ty)->is_borrow) {
#line 1409 "src/sema.tr"
                return true;
            }
        }
    } else if (1) {
        __auto_type _ = _t1167;
        /* pass */
    }
#line 1411 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__expr_is_shared(Sema* self, HirExpr* e) {
#line 1419 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 1419 "src/sema.tr"
        return false;
    }
#line 1420 "src/sema.tr"
    __auto_type _t1168 = (*e);
    if (_t1168.tag == HirExpr_EIdent) {
        __auto_type nm = _t1168.data.EIdent.name;
#line 1422 "src/sema.tr"
        Symbol* sym = Sema_resolve(self, nm);
#line 1423 "src/sema.tr"
        bool _trret198 = ((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && sym->is_shared);
        return _trret198;
    } else if (_t1168.tag == HirExpr_EMethodCall) {
        __auto_type _tr_v_recv = _t1168.data.EMethodCall.obj;
__auto_type meth = _t1168.data.EMethodCall.method;
#line 1427 "src/sema.tr"
        if (_tr_str_eqv((meth), (_tr_str_lit_len("clone", 5LL)))) {
#line 1427 "src/sema.tr"
            bool _trret199 = Sema__expr_is_shared(self, _tr_v_recv);
            return _trret199;
        }
    } else if (1) {
        __auto_type _ = _t1168;
        /* pass */
    }
#line 1429 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema_check_spawn_sendable(Sema* self, HirExpr* e) {
#line 1432 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 1432 "src/sema.tr"
        return;
    }
#line 1433 "src/sema.tr"
    __auto_type _t1169 = (*e);
    if (_t1169.tag == HirExpr_ECall) {
        __auto_type cs_args = _t1169.data.ECall.args;
#line 1435 "src/sema.tr"
        long long csi = 0LL;
#line 1436 "src/sema.tr"
        while ((csi < cs_args->len)) {
#line 1441 "src/sema.tr"
            if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(cs_args, csi)))) {
#line 1442 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot cross a thread boundary: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 252LL));
            }
#line 1443 "src/sema.tr"
            AstType* arg_ty = hir_expr_type(((HirExpr*)List_ptr_get(cs_args, csi)));
#line 1444 "src/sema.tr"
            if (((_tr_str_eqv((arg_ty->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((arg_ty->name), (_tr_str_lit_len("Weak", 4LL)))) && (arg_ty->args->len > 0LL))) {
#line 1445 "src/sema.tr"
                TrStr inner_nm = _tr_str_retain((*((AstType**)List_ptr_get(arg_ty->args, 0LL)))->name);
#line 1446 "src/sema.tr"
                if ((!Sema_is_sendable_type(self, inner_nm))) {
#line 1447 "src/sema.tr"
                    ({ TrStr _at_t1170 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] 'Shared[", 14LL)), (inner_nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' cannot safely cross thread boundaries because '", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (inner_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable.\n      FIX: Add 'implements Sendable' to '", 60LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (inner_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' and ensure all mutable fields use Atomic[T] or Mutex[T].", 58LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1170); _tr_str_release(_at_t1170); });
                }
            } else if ((((self->strict_mode && Sema__is_rc_class(self, arg_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(cs_args, csi))))) && (!Sema__is_unsafe_sendable(self, arg_ty->name)))) {
#line 1449 "src/sema.tr"
                ({ TrStr _at_t1171 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (arg_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot cross a thread boundary (this is exactly why Rust's 'Rc' is '!Send').\n      FIX: share it as 'Shared[", 216LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc'), or 'Mutex[", 51LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' for exclusive access.", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1171); _tr_str_release(_at_t1171); });
            } else if ((!Sema_is_sendable_type(self, arg_ty->name))) {
#line 1451 "src/sema.tr"
                ({ TrStr _at_t1172 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (arg_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex[", 87LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] for exclusive access, or Atomic[T] for counters/flags.\n      Or add 'implements Sendable' to '", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1172); _tr_str_release(_at_t1172); });
            }
#line 1452 "src/sema.tr"
            Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(cs_args, csi)));
#line 1453 "src/sema.tr"
            csi = (csi + 1LL);
        }
    } else if (_t1169.tag == HirExpr_EMethodCall) {
        __auto_type cs_margs = _t1169.data.EMethodCall.args;
#line 1455 "src/sema.tr"
        long long csmi = 0LL;
#line 1456 "src/sema.tr"
        while ((csmi < cs_margs->len)) {
#line 1457 "src/sema.tr"
            if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(cs_margs, csmi)))) {
#line 1458 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot cross a thread boundary: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 252LL));
            }
#line 1459 "src/sema.tr"
            AstType* arg_ty2 = hir_expr_type(((HirExpr*)List_ptr_get(cs_margs, csmi)));
#line 1460 "src/sema.tr"
            if (((_tr_str_eqv((arg_ty2->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((arg_ty2->name), (_tr_str_lit_len("Weak", 4LL)))) && (arg_ty2->args->len > 0LL))) {
#line 1461 "src/sema.tr"
                TrStr inner_nm2 = _tr_str_retain((*((AstType**)List_ptr_get(arg_ty2->args, 0LL)))->name);
#line 1462 "src/sema.tr"
                if ((!Sema_is_sendable_type(self, inner_nm2))) {
#line 1463 "src/sema.tr"
                    ({ TrStr _at_t1173 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] 'Shared[", 14LL)), (inner_nm2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' cannot safely cross thread boundaries because '", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (inner_nm2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable.\n      FIX: Add 'implements Sendable' to '", 60LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (inner_nm2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' and protect mutable fields with Atomic[T] or Mutex[T].", 56LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1173); _tr_str_release(_at_t1173); });
                }
            } else if ((((self->strict_mode && Sema__is_rc_class(self, arg_ty2->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(cs_margs, csmi))))) && (!Sema__is_unsafe_sendable(self, arg_ty2->name)))) {
#line 1465 "src/sema.tr"
                ({ TrStr _at_t1174 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (arg_ty2->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot cross a thread boundary.\n      FIX: share it as 'Shared[", 171LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty2->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount) or 'Mutex[", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty2->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' for exclusive access.", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1174); _tr_str_release(_at_t1174); });
            } else if ((!Sema_is_sendable_type(self, arg_ty2->name))) {
#line 1467 "src/sema.tr"
                ({ TrStr _at_t1175 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (arg_ty2->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex[", 87LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty2->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("].", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1175); _tr_str_release(_at_t1175); });
            }
#line 1468 "src/sema.tr"
            Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(cs_margs, csmi)));
#line 1469 "src/sema.tr"
            csmi = (csmi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1169;
        /* pass */
    }
}

__attribute__((hot)) void Sema_check_class_sendable_fields(Sema* self, ClassDef* c) {
#line 1478 "src/sema.tr"
    bool has_unsafe = false;
#line 1479 "src/sema.tr"
    long long ui = 0LL;
#line 1480 "src/sema.tr"
    while ((ui < c->iface_names->len)) {
#line 1481 "src/sema.tr"
        if (({ TrStr _cmpa_t1176 = (List_TrStr_get(c->iface_names, ui)); TrStr _cmpb_t1177 = (_tr_str_lit_len("UnsafeSendable", 14LL)); _Bool _cmpr_t1178 = _tr_str_eqv(_cmpa_t1176, _cmpb_t1177); _tr_str_release(_cmpa_t1176); _cmpr_t1178; })) {
#line 1481 "src/sema.tr"
            has_unsafe = true;
        }
#line 1482 "src/sema.tr"
        ui = (ui + 1LL);
    }
#line 1487 "src/sema.tr"
    if (has_unsafe) {
#line 1487 "src/sema.tr"
        return;
    }
#line 1488 "src/sema.tr"
    long long cfi = 0LL;
#line 1489 "src/sema.tr"
    while ((cfi < c->fields->len)) {
#line 1490 "src/sema.tr"
        FieldDef* fd = ((FieldDef*)List_ptr_get(c->fields, cfi));
#line 1491 "src/sema.tr"
        if ((((unsigned long long)(fd->ty)) != ((unsigned long long)(0LL)))) {
#line 1492 "src/sema.tr"
            AstType* fty = (*fd->ty);
#line 1493 "src/sema.tr"
            if (_tr_str_eqv((fty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 1496 "src/sema.tr"
                if ((!has_unsafe)) {
#line 1497 "src/sema.tr"
                    ({ TrStr _at_t1179 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-2] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares 'implements Sendable' but holds a raw 'Pointer' field '", 66LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fd->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which the compiler cannot prove thread-safe.\n      FIX: If '", 63LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' wraps an internally-synchronized handle, assert it explicitly: 'implements Sendable, UnsafeSendable'.\n      Otherwise wrap the data in Mutex[T]/Atomic[T], or drop 'implements Sendable'.", 187LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1179); _tr_str_release(_at_t1179); });
                }
            } else if ((!Sema_is_sendable_ty(self, fty))) {
#line 1502 "src/sema.tr"
                TrStr _t2_inner = _tr_str_retain(fty->name);
#line 1503 "src/sema.tr"
                if (((((_tr_str_eqv((fty->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("Channel", 7LL)))) && (fty->args->len > 0LL))) {
#line 1504 "src/sema.tr"
                    TrStr _strtmp_t1181 = ({ __auto_type _ord_t1180 = (_tr_strx_concatv((fty->name), (_tr_str_lit_len("[", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1180); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(fty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
                    _tr_str_release(_t2_inner);
                    _t2_inner = _strtmp_t1181;
                }
#line 1505 "src/sema.tr"
                ({ TrStr _at_t1182 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-2] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares 'implements Sendable' but field '", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fd->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_t2_inner)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable (non-thread-safe data is reachable across threads).\n      FIX: make the inner type Sendable, wrap it in Mutex[T]/RwLock[T] for guarded access or Atomic[T] for numerics, or drop 'implements Sendable'.", 217LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1182); _tr_str_release(_at_t1182); });
                _tr_str_release(_t2_inner);
            } else if (((_tr_str_eqv((fty->name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("bool", 4LL))))) {
#line 1507 "src/sema.tr"
                ({ TrStr _at_t1183 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-3] Sendable class '", 22LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has primitive field '", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fd->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' that may cause data races if mutated from multiple threads.\n      FIX: Use 'Atomic[", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' for safe concurrent mutation, or ensure this field is written only before the object is shared across threads.", 113LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->warnings, _at_t1183); _tr_str_release(_at_t1183); });
            }
        }
#line 1508 "src/sema.tr"
        cfi = (cfi + 1LL);
    }
}

__attribute__((hot)) HirExpr* Sema__patch_empty_dict_hint(Sema* self, HirExpr* hv, AstType* hint_ty) {
#line 1521 "src/sema.tr"
    if ((((unsigned long long)(hv)) == ((unsigned long long)(0LL)))) {
#line 1521 "src/sema.tr"
        return hv;
    }
#line 1522 "src/sema.tr"
    if (((!_tr_str_eqv((hint_ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (!_tr_str_eqv((hint_ty->name), (_tr_str_lit_len("Map", 3LL)))))) {
#line 1522 "src/sema.tr"
        return hv;
    }
#line 1523 "src/sema.tr"
    if ((hint_ty->args->len < 2LL)) {
#line 1523 "src/sema.tr"
        return hv;
    }
#line 1524 "src/sema.tr"
    __auto_type _t1184 = (*hv);
    if (_t1184.tag == HirExpr_EDict) {
        __auto_type hk = _t1184.data.EDict.keys;
__auto_type hvv = _t1184.data.EDict.vals;
#line 1526 "src/sema.tr"
        if ((hk->len == 0LL)) {
#line 1526 "src/sema.tr"
            return box_hirexpr(HirExpr_ctor_EDict(hk, hvv, hint_ty));
        }
#line 1527 "src/sema.tr"
        return hv;
    } else if (1) {
        __auto_type _ = _t1184;
        return hv;
    }
}

__attribute__((hot)) void Sema_mark_moved(Sema* self, TrStr name) {
#line 1533 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1534 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1535 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1536 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1537 "src/sema.tr"
            if ((!sym->is_moved)) {
#line 1538 "src/sema.tr"
                sym->is_moved = true;
#line 1539 "src/sema.tr"
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1540 "src/sema.tr"
                if (self->capturing_moves) {
#line 1541 "src/sema.tr"
                    List_TrStr_append(self->branch_moved_buf, name);
                }
            }
#line 1542 "src/sema.tr"
            return;
        }
#line 1543 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1544 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1545 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1546 "src/sema.tr"
        if ((!sym->is_moved)) {
#line 1547 "src/sema.tr"
            sym->is_moved = true;
#line 1548 "src/sema.tr"
            _tr_dict_set(self->globals, _tr_strz(name), sym);
#line 1549 "src/sema.tr"
            if (self->capturing_moves) {
#line 1550 "src/sema.tr"
                List_TrStr_append(self->branch_moved_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_mark_freed(Sema* self, TrStr name) {
#line 1553 "src/sema.tr"
    long long mf_i = (self->scopes->len - 1LL);
#line 1554 "src/sema.tr"
    while ((mf_i >= 0LL)) {
#line 1555 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name))) {
#line 1556 "src/sema.tr"
            Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name)));
#line 1557 "src/sema.tr"
            mf_sym->is_freed = true;
#line 1558 "src/sema.tr"
            mf_sym->is_moved = true;
#line 1559 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name), mf_sym);
#line 1560 "src/sema.tr"
            if (self->capturing_moves) {
#line 1561 "src/sema.tr"
                List_TrStr_append(self->branch_moved_buf, name);
            }
#line 1562 "src/sema.tr"
            return;
        }
#line 1563 "src/sema.tr"
        mf_i = (mf_i - 1LL);
    }
#line 1564 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1565 "src/sema.tr"
        Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1566 "src/sema.tr"
        mf_sym->is_freed = true;
#line 1567 "src/sema.tr"
        mf_sym->is_moved = true;
#line 1568 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), mf_sym);
#line 1569 "src/sema.tr"
        if (self->capturing_moves) {
#line 1570 "src/sema.tr"
            List_TrStr_append(self->branch_moved_buf, name);
        }
    }
}

__attribute__((hot)) void Sema_check_not_moved(Sema* self, TrStr name, TrStr ty_name) {
#line 1573 "src/sema.tr"
    if (Sema_is_primitive_name(self, ty_name)) {
#line 1573 "src/sema.tr"
        return;
    }
#line 1574 "src/sema.tr"
    Symbol* sym = Sema_resolve(self, name);
#line 1575 "src/sema.tr"
    if (((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && sym->is_moved)) {
#line 1576 "src/sema.tr"
        ({ TrStr _at_t1185 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving.", 123LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1185); _tr_str_release(_at_t1185); });
    }
}

__attribute__((hot)) void Sema_mark_borrow(Sema* self, TrStr name) {
#line 1579 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1580 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1581 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1582 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1583 "src/sema.tr"
            sym->active_borrows = (sym->active_borrows + 1LL);
#line 1584 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1585 "src/sema.tr"
            return;
        }
#line 1586 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1587 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1588 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1589 "src/sema.tr"
        sym->active_borrows = (sym->active_borrows + 1LL);
#line 1590 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_unmark_borrow(Sema* self, TrStr name) {
#line 1593 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1594 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1595 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1596 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1597 "src/sema.tr"
            if ((sym->active_borrows > 0LL)) {
#line 1598 "src/sema.tr"
                sym->active_borrows = (sym->active_borrows - 1LL);
            }
#line 1599 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1600 "src/sema.tr"
            return;
        }
#line 1601 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1602 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1603 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1604 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 1605 "src/sema.tr"
            sym->active_borrows = (sym->active_borrows - 1LL);
        }
#line 1606 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_check_no_active_borrows(Sema* self, TrStr name, TrStr ty_name) {
#line 1609 "src/sema.tr"
    if (Sema_is_primitive_name(self, ty_name)) {
#line 1609 "src/sema.tr"
        return;
    }
#line 1610 "src/sema.tr"
    Symbol* sym = Sema_resolve(self, name);
#line 1611 "src/sema.tr"
    if (((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && (sym->active_borrows > 0LL))) {
#line 1612 "src/sema.tr"
        ({ TrStr _at_t1186 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-2] Cannot move '", 19LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while it is borrowed.\n      FIX: The borrow must end before '", 63LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' can be moved.", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1186); _tr_str_release(_at_t1186); });
    }
}

__attribute__((hot)) void Sema_mark_init(Sema* self, TrStr name) {
#line 1616 "src/sema.tr"
    long long mi_i = (self->scopes->len - 1LL);
#line 1617 "src/sema.tr"
    while ((mi_i >= 0LL)) {
#line 1618 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
#line 1619 "src/sema.tr"
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
#line 1620 "src/sema.tr"
            if ((!mi_sym->is_init)) {
#line 1622 "src/sema.tr"
                mi_sym->is_init = true;
#line 1623 "src/sema.tr"
                mi_sym->is_maybe_init = false;
#line 1624 "src/sema.tr"
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
#line 1625 "src/sema.tr"
                if (self->capturing_inits) {
#line 1626 "src/sema.tr"
                    List_TrStr_append(self->branch_init_buf, name);
                }
            }
#line 1627 "src/sema.tr"
            return;
        }
#line 1628 "src/sema.tr"
        mi_i = (mi_i - 1LL);
    }
#line 1629 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1630 "src/sema.tr"
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1631 "src/sema.tr"
        if ((!mi_sym->is_init)) {
#line 1632 "src/sema.tr"
            mi_sym->is_init = true;
#line 1633 "src/sema.tr"
            mi_sym->is_maybe_init = false;
#line 1634 "src/sema.tr"
            _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
#line 1635 "src/sema.tr"
            if (self->capturing_inits) {
#line 1636 "src/sema.tr"
                List_TrStr_append(self->branch_init_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_clear_container_borrow(Sema* self, TrStr var_name) {
#line 1640 "src/sema.tr"
    if (_tr_dict_contains(self->container_borrows, _tr_strz(var_name))) {
#line 1641 "src/sema.tr"
        _tr_dict_remove(self->container_borrows, _tr_strz(var_name));
    }
}

__attribute__((hot)) void Sema_unmark_moved(Sema* self, TrStr name) {
#line 1645 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1646 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1647 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1648 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1649 "src/sema.tr"
            sym->is_moved = false;
#line 1655 "src/sema.tr"
            sym->is_freed = false;
#line 1656 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1657 "src/sema.tr"
            return;
        }
#line 1658 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1659 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1660 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1661 "src/sema.tr"
        sym->is_moved = false;
#line 1662 "src/sema.tr"
        sym->is_freed = false;
#line 1663 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_moved(Sema* self, TrStr name) {
#line 1667 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1668 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1669 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1670 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1671 "src/sema.tr"
            sym->is_maybe_moved = true;
#line 1672 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1673 "src/sema.tr"
            return;
        }
#line 1674 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1675 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1676 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1677 "src/sema.tr"
        sym->is_maybe_moved = true;
#line 1678 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_revive_moved(Sema* self, TrStr name) {
#line 1686 "src/sema.tr"
    bool rv_found = false;
#line 1687 "src/sema.tr"
    long long rv_i = (self->scopes->len - 1LL);
#line 1688 "src/sema.tr"
    while (((rv_i >= 0LL) && (!rv_found))) {
#line 1689 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, rv_i))->variables, _tr_strz(name))) {
#line 1690 "src/sema.tr"
            Symbol* rv_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, rv_i))->variables, _tr_strz(name)));
#line 1691 "src/sema.tr"
            rv_sym->is_moved = false;
#line 1692 "src/sema.tr"
            rv_sym->is_maybe_moved = false;
#line 1693 "src/sema.tr"
            rv_sym->is_freed = false;
#line 1694 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, rv_i))->variables, _tr_strz(name), rv_sym);
#line 1695 "src/sema.tr"
            rv_found = true;
        }
#line 1696 "src/sema.tr"
        rv_i = (rv_i - 1LL);
    }
#line 1697 "src/sema.tr"
    if (((!rv_found) && _tr_dict_contains(self->globals, _tr_strz(name)))) {
#line 1698 "src/sema.tr"
        Symbol* rv_g = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1699 "src/sema.tr"
        rv_g->is_moved = false;
#line 1700 "src/sema.tr"
        rv_g->is_maybe_moved = false;
#line 1701 "src/sema.tr"
        rv_g->is_freed = false;
#line 1702 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), rv_g);
    }
#line 1705 "src/sema.tr"
    if (((rv_found && self->capturing_moves) && (((unsigned long long)(self->branch_moved_buf)) != 0LL))) {
#line 1706 "src/sema.tr"
        List_TrStr* rv_nb = (void*)List_TrStr_new();
#line 1707 "src/sema.tr"
        long long rv_k = 0LL;
#line 1708 "src/sema.tr"
        while ((rv_k < self->branch_moved_buf->len)) {
#line 1709 "src/sema.tr"
            if (({ TrStr _cmpa_t1187 = (List_TrStr_get(self->branch_moved_buf, rv_k)); TrStr _cmpb_t1188 = (name); _Bool _cmpr_t1189 = !_tr_str_eqv(_cmpa_t1187, _cmpb_t1188); _tr_str_release(_cmpa_t1187); _cmpr_t1189; })) {
#line 1709 "src/sema.tr"
                ({ TrStr _at_t1190 = (List_TrStr_get(self->branch_moved_buf, rv_k)); List_TrStr_append(rv_nb, _at_t1190); _tr_str_release(_at_t1190); });
            }
#line 1710 "src/sema.tr"
            rv_k = (rv_k + 1LL);
        }
#line 1711 "src/sema.tr"
        self->branch_moved_buf = rv_nb;
    }
}

__attribute__((hot)) void Sema_unmark_init(Sema* self, TrStr name) {
#line 1715 "src/sema.tr"
    long long mi_i = (self->scopes->len - 1LL);
#line 1716 "src/sema.tr"
    while ((mi_i >= 0LL)) {
#line 1717 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
#line 1718 "src/sema.tr"
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
#line 1719 "src/sema.tr"
            mi_sym->is_init = false;
#line 1720 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
#line 1721 "src/sema.tr"
            return;
        }
#line 1722 "src/sema.tr"
        mi_i = (mi_i - 1LL);
    }
#line 1723 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1724 "src/sema.tr"
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1725 "src/sema.tr"
        mi_sym->is_init = false;
#line 1726 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_init(Sema* self, TrStr name) {
#line 1730 "src/sema.tr"
    long long mi_i = (self->scopes->len - 1LL);
#line 1731 "src/sema.tr"
    while ((mi_i >= 0LL)) {
#line 1732 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
#line 1733 "src/sema.tr"
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
#line 1734 "src/sema.tr"
            mi_sym->is_maybe_init = true;
#line 1735 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
#line 1736 "src/sema.tr"
            return;
        }
#line 1737 "src/sema.tr"
        mi_i = (mi_i - 1LL);
    }
#line 1738 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1739 "src/sema.tr"
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1740 "src/sema.tr"
        mi_sym->is_maybe_init = true;
#line 1741 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
    }
}

__attribute__((hot)) bool Sema_vec_str_contains(Sema* self, List_TrStr* v, TrStr s) {
#line 1745 "src/sema.tr"
    long long vi = 0LL;
#line 1746 "src/sema.tr"
    while ((vi < v->len)) {
#line 1747 "src/sema.tr"
        if (({ TrStr _cmpa_t1191 = (List_TrStr_get(v, vi)); TrStr _cmpb_t1192 = (s); _Bool _cmpr_t1193 = _tr_str_eqv(_cmpa_t1191, _cmpb_t1192); _tr_str_release(_cmpa_t1191); _cmpr_t1193; })) {
#line 1747 "src/sema.tr"
            return true;
        }
#line 1748 "src/sema.tr"
        vi = (vi + 1LL);
    }
#line 1749 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_has_copy_decorator(Sema* self, List_ptr* decs) {
#line 1755 "src/sema.tr"
    long long i = 0LL;
#line 1756 "src/sema.tr"
    while ((i < decs->len)) {
#line 1757 "src/sema.tr"
        if (_tr_str_eqv((((Decorator*)List_ptr_get(decs, i))->name), (_tr_str_lit_len("copy", 4LL)))) {
#line 1757 "src/sema.tr"
            return true;
        }
#line 1758 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1759 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_class_has_free(Sema* self, TrStr name) {
#line 1764 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(name)))) {
#line 1764 "src/sema.tr"
        return false;
    }
#line 1765 "src/sema.tr"
    ClassDef* hf_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
#line 1766 "src/sema.tr"
    long long hf_i = 0LL;
#line 1767 "src/sema.tr"
    while ((hf_i < hf_cls->methods->len)) {
#line 1768 "src/sema.tr"
        TrStr hf_n = _tr_str_retain(((FunctionDef*)List_ptr_get(hf_cls->methods, hf_i))->name);
#line 1769 "src/sema.tr"
        if ((_tr_str_eqv((hf_n), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((hf_n), (_tr_str_lit_len("dispose", 7LL))))) {
#line 1769 "src/sema.tr"
            _tr_str_release(hf_n);
            return true;
        }
#line 1770 "src/sema.tr"
        hf_i = (hf_i + 1LL);
        _tr_str_release(hf_n);
    }
#line 1771 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_copy_class(Sema* self, TrStr name) {
#line 1774 "src/sema.tr"
    if (Sema_is_primitive_name(self, name)) {
#line 1774 "src/sema.tr"
        return true;
    }
#line 1775 "src/sema.tr"
    if (_tr_dict_contains(self->copy_classes, _tr_strz(name))) {
#line 1775 "src/sema.tr"
        return ((bool)(uintptr_t)_tr_dict_get(self->copy_classes, _tr_strz(name)));
    }
#line 1776 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(name)))) {
#line 1776 "src/sema.tr"
        return false;
    }
#line 1778 "src/sema.tr"
    _tr_dict_set(self->copy_classes, _tr_strz(name), false);
#line 1779 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
#line 1780 "src/sema.tr"
    if ((cls->fields->len == 0LL)) {
#line 1780 "src/sema.tr"
        return false;
    }
#line 1781 "src/sema.tr"
    bool all_copy = true;
#line 1782 "src/sema.tr"
    long long fi = 0LL;
#line 1783 "src/sema.tr"
    while ((fi < cls->fields->len)) {
#line 1784 "src/sema.tr"
        FieldDef* fd = ((FieldDef*)List_ptr_get(cls->fields, fi));
#line 1785 "src/sema.tr"
        if ((((unsigned long long)(fd->ty)) != ((unsigned long long)(0LL)))) {
#line 1786 "src/sema.tr"
            AstType* fty = (*fd->ty);
#line 1792 "src/sema.tr"
            if ((!fty->is_borrow)) {
#line 1793 "src/sema.tr"
                if ((!Sema_is_copy_class(self, fty->name))) {
#line 1794 "src/sema.tr"
                    all_copy = false;
                }
            }
        }
#line 1795 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 1796 "src/sema.tr"
    _tr_dict_set(self->copy_classes, _tr_strz(name), all_copy);
#line 1797 "src/sema.tr"
    bool _trret200 = all_copy;
    return _trret200;
}

__attribute__((hot)) void Sema_enter_scope(Sema* self) {
#line 1800 "src/sema.tr"
    Scope* s_obj = Scope_init();
#line 1801 "src/sema.tr"
    List_ptr_append(self->scopes, _tr_obj_retain(s_obj));
#line 1802 "src/sema.tr"
    self->current_scope_depth = (self->current_scope_depth + 1LL);
#line 1805 "src/sema.tr"
    List_i64_append(self->block_depth_stack, self->block_depth);
#line 1806 "src/sema.tr"
    self->block_depth = 0LL;
#line 1807 "src/sema.tr"
    List_i64_append(self->block_stack_base, self->block_stack->len);
    _tr_obj_release(s_obj, _trdrop_Scope);
}

__attribute__((hot)) void Sema_exit_scope(Sema* self) {
#line 1813 "src/sema.tr"
    long long es_top = (self->scopes->len - 1LL);
#line 1814 "src/sema.tr"
    if ((es_top >= 0LL)) {
#line 1815 "src/sema.tr"
        Scope* es_scope = ((Scope*)List_ptr_get(self->scopes, es_top));
#line 1816 "src/sema.tr"
        long long es_k = (self->clo_let_names->len - 1LL);
#line 1817 "src/sema.tr"
        while ((es_k >= 0LL)) {
#line 1818 "src/sema.tr"
            if ((List_i64_get(self->clo_let_scopes, es_k) == es_top)) {
#line 1819 "src/sema.tr"
                TrStr es_nm = List_TrStr_get(self->clo_let_names, es_k);
#line 1820 "src/sema.tr"
                if (_tr_dict_contains(es_scope->variables, _tr_strz(es_nm))) {
#line 1821 "src/sema.tr"
                    Symbol* es_sym = ((Symbol*)(uintptr_t)_tr_dict_get(es_scope->variables, _tr_strz(es_nm)));
#line 1822 "src/sema.tr"
                    if (es_sym->coll_escaped) {
#line 1823 "src/sema.tr"
                        HirExpr* es_p = ((HirExpr*)List_ptr_get(self->clo_let_ptrs, es_k));
#line 1824 "src/sema.tr"
                        __auto_type _t1194 = (*es_p);
                        if (_t1194.tag == HirExpr_EClosure) {
                            __auto_type c_ps = _t1194.data.EClosure.params;
__auto_type c_rt = _t1194.data.EClosure.ret_ty;
__auto_type c_bd = _t1194.data.EClosure.body;
__auto_type c_as = _t1194.data.EClosure.is_async;
__auto_type c_caps = _t1194.data.EClosure.captures;
#line 1826 "src/sema.tr"
                            HirParam* mk = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 1827 "src/sema.tr"
                            mk->name = _tr_str_lit_len("@heap", 5LL);
#line 1828 "src/sema.tr"
                            mk->ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 1829 "src/sema.tr"
                            List_ptr* ncaps = (void*)List_ptr_new();
#line 1830 "src/sema.tr"
                            long long mi = 0LL;
#line 1831 "src/sema.tr"
                            while ((mi < c_caps->len)) {
#line 1832 "src/sema.tr"
                                List_ptr_append(ncaps, _tr_obj_retain(((HirParam*)List_ptr_get(c_caps, mi))));
#line 1833 "src/sema.tr"
                                mi = (mi + 1LL);
                            }
#line 1834 "src/sema.tr"
                            List_ptr_append(ncaps, _tr_obj_retain(mk));
#line 1835 "src/sema.tr"
                            /* unsafe block */
#line 1836 "src/sema.tr"
                            (*es_p = HirExpr_ctor_EClosure(c_ps, c_rt, c_bd, c_as, ncaps));
                            _tr_obj_release(mk, _trdrop_HirParam);
                        } else if (1) {
                            __auto_type _ = _t1194;
                            /* pass */
                        }
                    }
                }
#line 1838 "src/sema.tr"
                List_TrStr_pop(self->clo_let_names);
#line 1839 "src/sema.tr"
                List_ptr_pop(self->clo_let_ptrs);
#line 1840 "src/sema.tr"
                List_i64_pop(self->clo_let_scopes);
            }
#line 1841 "src/sema.tr"
            es_k = (es_k - 1LL);
        }
    }
#line 1842 "src/sema.tr"
    ((Scope*)List_ptr_pop(self->scopes));
#line 1843 "src/sema.tr"
    self->current_scope_depth = (self->current_scope_depth - 1LL);
#line 1844 "src/sema.tr"
    self->block_depth = List_i64_get(self->block_depth_stack, (self->block_depth_stack->len - 1LL));
#line 1845 "src/sema.tr"
    List_i64_pop(self->block_depth_stack);
#line 1846 "src/sema.tr"
    long long base = List_i64_get(self->block_stack_base, (self->block_stack_base->len - 1LL));
#line 1847 "src/sema.tr"
    while ((self->block_stack->len > base)) {
#line 1848 "src/sema.tr"
        List_i64_pop(self->block_stack);
    }
#line 1849 "src/sema.tr"
    List_i64_pop(self->block_stack_base);
}

__attribute__((hot)) bool Sema__coll_elem_droppable(Sema* self, TrStr n) {
#line 1858 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL)))) {
#line 1858 "src/sema.tr"
        return true;
    }
#line 1859 "src/sema.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL))))) {
#line 1859 "src/sema.tr"
        return true;
    }
#line 1860 "src/sema.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL))))) {
#line 1860 "src/sema.tr"
        return true;
    }
#line 1861 "src/sema.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL))))) {
#line 1861 "src/sema.tr"
        return true;
    }
#line 1862 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))))) {
#line 1862 "src/sema.tr"
        return true;
    }
#line 1868 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
#line 1869 "src/sema.tr"
        if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)))->is_class)) {
#line 1869 "src/sema.tr"
            return false;
        }
#line 1873 "src/sema.tr"
        if ((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 1873 "src/sema.tr"
            return false;
        }
#line 1874 "src/sema.tr"
        return true;
    }
#line 1875 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema__scan_ptr_aliased_ty(Sema* self, AstType* t) {
#line 1882 "src/sema.tr"
    if ((_tr_str_eqv((t->name), (_tr_str_lit_len("Pointer", 7LL))) && (t->args->len > 0LL))) {
#line 1883 "src/sema.tr"
        AstType* inner = (*((AstType**)List_ptr_get(t->args, 0LL)));
#line 1884 "src/sema.tr"
        _tr_dict_set(self->ptr_aliased, _tr_strz(inner->name), true);
#line 1885 "src/sema.tr"
        Sema__scan_ptr_aliased_ty(self, inner);
#line 1886 "src/sema.tr"
        return;
    }
#line 1887 "src/sema.tr"
    long long ai = 0LL;
#line 1888 "src/sema.tr"
    while ((ai < t->args->len)) {
#line 1889 "src/sema.tr"
        Sema__scan_ptr_aliased_ty(self, (*((AstType**)List_ptr_get(t->args, ai))));
#line 1890 "src/sema.tr"
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) bool Sema__is_rc_class(Sema* self, TrStr n) {
#line 1895 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(n)))) {
#line 1895 "src/sema.tr"
        return false;
    }
#line 1896 "src/sema.tr"
    if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)))->is_class)) {
#line 1896 "src/sema.tr"
        return false;
    }
#line 1897 "src/sema.tr"
    if ((((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || (_tr_str_eqv((n), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 1897 "src/sema.tr"
        return false;
    }
#line 1898 "src/sema.tr"
    if (_tr_dict_contains(self->ptr_aliased, _tr_strz(n))) {
#line 1898 "src/sema.tr"
        return false;
    }
#line 1899 "src/sema.tr"
    return true;
}

__attribute__((hot)) bool Sema__ty_reaches_plain_rc(Sema* self, AstType* ft, long long depth) {
#line 1909 "src/sema.tr"
    if ((depth > 24LL)) {
#line 1909 "src/sema.tr"
        return false;
    }
#line 1910 "src/sema.tr"
    TrStr n = _tr_str_retain(ft->name);
#line 1911 "src/sema.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 1911 "src/sema.tr"
        _tr_str_release(n);
        return false;
    }
#line 1912 "src/sema.tr"
    if ((((((((_tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || (_tr_str_eqv((n), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL))))) {
#line 1913 "src/sema.tr"
        if ((ft->args->len > 0LL)) {
#line 1913 "src/sema.tr"
            bool _trret201 = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(ft->args, 0LL))), (depth + 1LL));
            _tr_str_release(n);
            return _trret201;
        }
#line 1914 "src/sema.tr"
        _tr_str_release(n);
        return false;
    }
#line 1915 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
#line 1916 "src/sema.tr"
        if ((ft->args->len > 1LL)) {
#line 1916 "src/sema.tr"
            bool _trret202 = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(ft->args, 1LL))), (depth + 1LL));
            _tr_str_release(n);
            return _trret202;
        }
#line 1917 "src/sema.tr"
        _tr_str_release(n);
        return false;
    }
#line 1918 "src/sema.tr"
    if (Sema__is_rc_class(self, n)) {
#line 1918 "src/sema.tr"
        _tr_str_release(n);
        return true;
    }
#line 1919 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
#line 1919 "src/sema.tr"
        bool _trret203 = Sema__sendable_reaches_plain_rc(self, n, (depth + 1LL));
        _tr_str_release(n);
        return _trret203;
    }
#line 1920 "src/sema.tr"
    _tr_str_release(n);
    return false;
}

__attribute__((hot)) bool Sema__sendable_reaches_plain_rc(Sema* self, TrStr tn, long long depth) {
#line 1923 "src/sema.tr"
    if ((depth > 24LL)) {
#line 1923 "src/sema.tr"
        return false;
    }
#line 1924 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
#line 1924 "src/sema.tr"
        return false;
    }
#line 1925 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(tn)));
#line 1926 "src/sema.tr"
    long long i = 0LL;
#line 1927 "src/sema.tr"
    while ((i < cls->fields->len)) {
#line 1928 "src/sema.tr"
        FieldDef* f = ((FieldDef*)List_ptr_get(cls->fields, i));
#line 1929 "src/sema.tr"
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
#line 1930 "src/sema.tr"
            if (Sema__ty_reaches_plain_rc(self, (*f->ty), (depth + 1LL))) {
#line 1930 "src/sema.tr"
                return true;
            }
        }
#line 1931 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1932 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__is_unsafe_sendable(Sema* self, TrStr n) {
#line 1945 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(n)))) {
#line 1945 "src/sema.tr"
        return false;
    }
#line 1946 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)));
#line 1947 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(cls->iface_names)))) {
#line 1947 "src/sema.tr"
        return false;
    }
#line 1948 "src/sema.tr"
    long long i = 0LL;
#line 1949 "src/sema.tr"
    while ((i < cls->iface_names->len)) {
#line 1950 "src/sema.tr"
        if (({ TrStr _cmpa_t1195 = (List_TrStr_get(cls->iface_names, i)); TrStr _cmpb_t1196 = (_tr_str_lit_len("UnsafeSendable", 14LL)); _Bool _cmpr_t1197 = _tr_str_eqv(_cmpa_t1195, _cmpb_t1196); _tr_str_release(_cmpa_t1195); _cmpr_t1197; })) {
#line 1950 "src/sema.tr"
            return true;
        }
#line 1951 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1952 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema__check_spawn_nested_rc(Sema* self, HirExpr* arg_expr) {
#line 1955 "src/sema.tr"
    if ((!self->strict_mode)) {
#line 1955 "src/sema.tr"
        return;
    }
#line 1956 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(arg_expr)))) {
#line 1956 "src/sema.tr"
        return;
    }
#line 1957 "src/sema.tr"
    AstType* at = hir_expr_type(arg_expr);
#line 1958 "src/sema.tr"
    TrStr n = _tr_str_retain(at->name);
#line 1960 "src/sema.tr"
    TrStr _cross_cls = _tr_str_retain(n);
#line 1961 "src/sema.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) && (at->args->len > 0LL))) {
#line 1961 "src/sema.tr"
        TrStr _strtmp_t1198 = _tr_str_retain((*((AstType**)List_ptr_get(at->args, 0LL)))->name);
        _tr_str_release(_cross_cls);
        _cross_cls = _strtmp_t1198;
    }
#line 1962 "src/sema.tr"
    if (Sema__is_unsafe_sendable(self, _cross_cls)) {
#line 1962 "src/sema.tr"
        _tr_str_release(n);
        _tr_str_release(_cross_cls);
        return;
    }
#line 1963 "src/sema.tr"
    bool reaches = false;
#line 1964 "src/sema.tr"
    if (Sema__expr_is_shared(self, arg_expr)) {
#line 1965 "src/sema.tr"
        TrStr inner = _tr_str_retain(n);
#line 1966 "src/sema.tr"
        if (((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) && (at->args->len > 0LL))) {
#line 1966 "src/sema.tr"
            TrStr _strtmp_t1199 = _tr_str_retain((*((AstType**)List_ptr_get(at->args, 0LL)))->name);
            _tr_str_release(inner);
            inner = _strtmp_t1199;
        }
#line 1967 "src/sema.tr"
        reaches = Sema__sendable_reaches_plain_rc(self, inner, 0LL);
        _tr_str_release(inner);
    } else if (((((((((_tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || (_tr_str_eqv((n), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) && (at->args->len > 0LL))) {
#line 1969 "src/sema.tr"
        reaches = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(at->args, 0LL))), 0LL);
    } else if (((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) && (at->args->len > 1LL))) {
#line 1971 "src/sema.tr"
        reaches = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(at->args, 1LL))), 0LL);
    }
#line 1972 "src/sema.tr"
    if (reaches) {
#line 1973 "src/sema.tr"
        ({ TrStr _at_t1200 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] a value crossing the thread boundary transitively holds a plain (non-atomic) reference-counted class inside '", 115LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (e.g. via a Mutex/Vec/field). Even wrapped in Shared/Sendable, that nested class's refcount is thread-local and would race across threads.\n      FIX: make the nested state 'Shared[T]' (atomic, like Rust's 'Arc'), or restructure so only one thread ever owns it.", 262LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1200); _tr_str_release(_at_t1200); });
    }
    _tr_str_release(n);
    _tr_str_release(_cross_cls);
}

__attribute__((hot)) void Sema__collect_strong_edge(Sema* self, AstType* ft, List_TrStr* out) {
#line 1979 "src/sema.tr"
    TrStr n = _tr_str_retain(ft->name);
#line 1984 "src/sema.tr"
    if (ft->is_borrow) {
#line 1984 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1985 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL))))) {
#line 1985 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1986 "src/sema.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL))))) {
#line 1987 "src/sema.tr"
        if ((ft->args->len > 0LL)) {
#line 1987 "src/sema.tr"
            Sema__collect_strong_edge(self, (*((AstType**)List_ptr_get(ft->args, 0LL))), out);
        }
#line 1988 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1989 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
#line 1990 "src/sema.tr"
        if ((ft->args->len > 1LL)) {
#line 1990 "src/sema.tr"
            Sema__collect_strong_edge(self, (*((AstType**)List_ptr_get(ft->args, 1LL))), out);
        }
#line 1991 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1992 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) {
#line 1993 "src/sema.tr"
        if ((ft->args->len > 0LL)) {
#line 1994 "src/sema.tr"
            TrStr si = _tr_str_retain((*((AstType**)List_ptr_get(ft->args, 0LL)))->name);
#line 1995 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(si))) {
#line 1995 "src/sema.tr"
                List_TrStr_append(out, si);
            }
        }
#line 1996 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1997 "src/sema.tr"
    if (Sema__is_rc_class(self, n)) {
#line 1997 "src/sema.tr"
        List_TrStr_append(out, n);
    }
    _tr_str_release(n);
}

__attribute__((hot)) List_TrStr* Sema__strong_owned(Sema* self, ClassDef* cd) {
#line 2000 "src/sema.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 2001 "src/sema.tr"
    long long i = 0LL;
#line 2002 "src/sema.tr"
    while ((i < cd->fields->len)) {
#line 2003 "src/sema.tr"
        FieldDef* f = ((FieldDef*)List_ptr_get(cd->fields, i));
#line 2004 "src/sema.tr"
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
#line 2005 "src/sema.tr"
            Sema__collect_strong_edge(self, (*f->ty), out);
        }
#line 2006 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2007 "src/sema.tr"
    return out;
}

__attribute__((hot)) void Sema__cycle_dfs(Sema* self, TrStr cur, TrStr start, TrMap* on_path, TrMap* reported, long long depth) {
#line 2012 "src/sema.tr"
    if ((depth > 200LL)) {
#line 2012 "src/sema.tr"
        return;
    }
#line 2013 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cur)))) {
#line 2013 "src/sema.tr"
        return;
    }
#line 2014 "src/sema.tr"
    List_TrStr* edges = Sema__strong_owned(self, ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cur))));
#line 2015 "src/sema.tr"
    long long i = 0LL;
#line 2016 "src/sema.tr"
    while ((i < edges->len)) {
#line 2017 "src/sema.tr"
        TrStr nxt = List_TrStr_get(edges, i);
#line 2018 "src/sema.tr"
        if (_tr_str_eqv((nxt), (start))) {
#line 2020 "src/sema.tr"
            if ((!_tr_dict_contains(reported, _tr_strz(start)))) {
#line 2021 "src/sema.tr"
                _tr_dict_set(reported, _tr_strz(start), true);
#line 2022 "src/sema.tr"
                ({ TrStr _at_t1201 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[S-2] class '", 13LL)), (start))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is part of a strong reference CYCLE (via '", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' -> '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nxt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'), which reference counting cannot reclaim — it would leak.\n      FIX: make one back-edge non-owning with 'Weak[", 115LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (start)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (a weak reference) or 'Pointer[", 34LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (start)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (raw, manual).", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1201); _tr_str_release(_at_t1201); });
            }
        } else if ((!_tr_dict_contains(on_path, _tr_strz(nxt)))) {
#line 2024 "src/sema.tr"
            _tr_dict_set(on_path, _tr_strz(nxt), true);
#line 2025 "src/sema.tr"
            Sema__cycle_dfs(self, nxt, start, on_path, reported, (depth + 1LL));
#line 2026 "src/sema.tr"
            _tr_dict_set(on_path, _tr_strz(nxt), false);
        }
#line 2027 "src/sema.tr"
        i = (i + 1LL);
        _tr_str_release(nxt);
    }
}

__attribute__((hot)) void Sema_check_ownership_cycles(Sema* self, Program* prog) {
#line 2031 "src/sema.tr"
    long long di = 0LL;
#line 2032 "src/sema.tr"
    while ((di < prog->decls->len)) {
#line 2033 "src/sema.tr"
        __auto_type _t1202 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t1202.tag == Decl_DClass) {
            __auto_type c = _t1202.data.DClass.cls;
            Sema__scan_class_ptrs(self, c);
        } else if (_t1202.tag == Decl_DActor) {
            __auto_type c = _t1202.data.DActor.cls;
            Sema__scan_class_ptrs(self, c);
        } else if (_t1202.tag == Decl_DExtend) {
            __auto_type ms = _t1202.data.DExtend.methods;
#line 2037 "src/sema.tr"
            long long mi = 0LL;
#line 2038 "src/sema.tr"
            while ((mi < ms->len)) {
#line 2039 "src/sema.tr"
                Sema__scan_fn_ptrs(self, ((FunctionDef*)List_ptr_get(ms, mi)));
#line 2040 "src/sema.tr"
                mi = (mi + 1LL);
            }
        } else if (_t1202.tag == Decl_DFunction) {
            __auto_type f = _t1202.data.DFunction.func;
            Sema__scan_fn_ptrs(self, f);
        } else if (1) {
            __auto_type _ = _t1202;
            /* pass */
        }
#line 2043 "src/sema.tr"
        di = (di + 1LL);
    }
#line 2045 "src/sema.tr"
    TrMap* reported = _tr_dict_new(16LL);
#line 2046 "src/sema.tr"
    di = 0LL;
#line 2047 "src/sema.tr"
    while ((di < prog->decls->len)) {
#line 2048 "src/sema.tr"
        __auto_type _t1203 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t1203.tag == Decl_DClass) {
            __auto_type c = _t1203.data.DClass.cls;
#line 2050 "src/sema.tr"
            if (Sema__is_rc_class(self, c->name)) {
#line 2051 "src/sema.tr"
                TrMap* on_path = _tr_dict_new(16LL);
#line 2052 "src/sema.tr"
                _tr_dict_set(on_path, _tr_strz(c->name), true);
#line 2053 "src/sema.tr"
                Sema__cycle_dfs(self, c->name, c->name, on_path, reported, 0LL);
            }
        } else if (1) {
            __auto_type _ = _t1203;
            /* pass */
        }
#line 2055 "src/sema.tr"
        di = (di + 1LL);
    }
}

__attribute__((hot)) void Sema__scan_class_ptrs(Sema* self, ClassDef* c) {
#line 2058 "src/sema.tr"
    long long i = 0LL;
#line 2059 "src/sema.tr"
    while ((i < c->fields->len)) {
#line 2060 "src/sema.tr"
        FieldDef* f = ((FieldDef*)List_ptr_get(c->fields, i));
#line 2061 "src/sema.tr"
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
#line 2061 "src/sema.tr"
            Sema__scan_ptr_aliased_ty(self, (*f->ty));
        }
#line 2062 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2063 "src/sema.tr"
    i = 0LL;
#line 2064 "src/sema.tr"
    while ((i < c->methods->len)) {
#line 2065 "src/sema.tr"
        Sema__scan_fn_ptrs(self, ((FunctionDef*)List_ptr_get(c->methods, i)));
#line 2066 "src/sema.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema__scan_fn_ptrs(Sema* self, FunctionDef* f) {
#line 2069 "src/sema.tr"
    long long i = 0LL;
#line 2070 "src/sema.tr"
    while ((i < f->params->len)) {
#line 2071 "src/sema.tr"
        Param* p = ((Param*)List_ptr_get(f->params, i));
#line 2072 "src/sema.tr"
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
#line 2072 "src/sema.tr"
            Sema__scan_ptr_aliased_ty(self, (*p->ty));
        }
#line 2073 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2074 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 2074 "src/sema.tr"
        Sema__scan_ptr_aliased_ty(self, (*f->ret_ty));
    }
}

__attribute__((hot)) bool Sema_is_droppable_sym(Sema* self, Symbol* sym) {
#line 2080 "src/sema.tr"
    if ((sym->kind.tag != SymbolKind_make_SVariable().tag)) {
#line 2080 "src/sema.tr"
        return false;
    }
#line 2081 "src/sema.tr"
    if (sym->is_param) {
#line 2081 "src/sema.tr"
        return false;
    }
#line 2082 "src/sema.tr"
    if ((((unsigned long long)(sym->ty)) == ((unsigned long long)(0LL)))) {
#line 2082 "src/sema.tr"
        return false;
    }
#line 2088 "src/sema.tr"
    if (sym->is_shared) {
#line 2089 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 2089 "src/sema.tr"
            return false;
        }
#line 2090 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 2090 "src/sema.tr"
            return false;
        }
#line 2091 "src/sema.tr"
        return true;
    }
#line 2092 "src/sema.tr"
    if (_tr_str_eqv(((*sym->ty)->name), (_tr_str_lit_len("Weak", 4LL)))) {
#line 2093 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 2093 "src/sema.tr"
            return false;
        }
#line 2094 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 2094 "src/sema.tr"
            return false;
        }
#line 2095 "src/sema.tr"
        return true;
    }
#line 2100 "src/sema.tr"
    if (_tr_str_eqv((sym->borrows_region), (_tr_str_lit_len("@borrowed", 9LL)))) {
#line 2100 "src/sema.tr"
        return false;
    }
#line 2101 "src/sema.tr"
    TrStr tn = _tr_str_retain((*sym->ty)->name);
#line 2108 "src/sema.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("str", 3LL)))) {
#line 2109 "src/sema.tr"
        if (sym->str_escaped) {
#line 2109 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2110 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 2110 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2111 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 2111 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2112 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 2112 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2113 "src/sema.tr"
        _tr_str_release(tn);
        return true;
    }
#line 2121 "src/sema.tr"
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 2122 "src/sema.tr"
        if (sym->coll_escaped) {
#line 2122 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2123 "src/sema.tr"
        if (_tr_dict_contains(self->alias_assigned, _tr_strz(sym->name))) {
#line 2123 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2124 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 2124 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2125 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 2125 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2126 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 2126 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2127 "src/sema.tr"
        List_ptr* targs = (*sym->ty)->args;
#line 2128 "src/sema.tr"
        if ((_tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL))))) {
#line 2129 "src/sema.tr"
            if ((targs->len < 2LL)) {
#line 2129 "src/sema.tr"
                _tr_str_release(tn);
                return false;
            }
#line 2130 "src/sema.tr"
            TrStr kt = _tr_str_retain((*((AstType**)List_ptr_get(targs, 0LL)))->name);
#line 2131 "src/sema.tr"
            TrStr vt = _tr_str_retain((*((AstType**)List_ptr_get(targs, 1LL)))->name);
#line 2132 "src/sema.tr"
            if ((!Sema__coll_elem_droppable(self, kt))) {
#line 2132 "src/sema.tr"
                _tr_str_release(tn);
                _tr_str_release(kt);
                _tr_str_release(vt);
                return false;
            }
#line 2136 "src/sema.tr"
            if ((!Sema__coll_elem_droppable(self, vt))) {
#line 2136 "src/sema.tr"
                _tr_str_release(tn);
                _tr_str_release(kt);
                _tr_str_release(vt);
                return false;
            }
#line 2137 "src/sema.tr"
            _tr_str_release(tn);
            _tr_str_release(kt);
            _tr_str_release(vt);
            return true;
        } else {
#line 2139 "src/sema.tr"
            if ((targs->len < 1LL)) {
#line 2139 "src/sema.tr"
                _tr_str_release(tn);
                return false;
            }
#line 2140 "src/sema.tr"
            TrStr et = _tr_str_retain((*((AstType**)List_ptr_get(targs, 0LL)))->name);
#line 2141 "src/sema.tr"
            if ((!Sema__coll_elem_droppable(self, et))) {
#line 2141 "src/sema.tr"
                _tr_str_release(tn);
                _tr_str_release(et);
                return false;
            }
#line 2142 "src/sema.tr"
            _tr_str_release(tn);
            _tr_str_release(et);
            return true;
        }
    }
#line 2148 "src/sema.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 2149 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 2149 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2150 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 2150 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2151 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 2151 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2152 "src/sema.tr"
        _tr_str_release(tn);
        return true;
    }
#line 2156 "src/sema.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL)))) {
#line 2157 "src/sema.tr"
        if (sym->coll_escaped) {
#line 2157 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2158 "src/sema.tr"
        if (_tr_dict_contains(self->alias_assigned, _tr_strz(sym->name))) {
#line 2158 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2159 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 2159 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2160 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 2160 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2161 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 2161 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2162 "src/sema.tr"
        _tr_str_release(tn);
        return true;
    }
#line 2163 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
#line 2163 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 2166 "src/sema.tr"
    if (((((((((((_tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Chan", 4LL))))) {
#line 2166 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 2170 "src/sema.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("StringBuilder", 13LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 2170 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 2171 "src/sema.tr"
    if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 2171 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 2172 "src/sema.tr"
    if ((sym->active_borrows > 0LL)) {
#line 2172 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 2173 "src/sema.tr"
    if (((!sym->is_init) || sym->is_maybe_init)) {
#line 2173 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 2174 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(tn)));
#line 2175 "src/sema.tr"
    long long mi = 0LL;
#line 2176 "src/sema.tr"
    bool has_dispose = false;
#line 2177 "src/sema.tr"
    while ((mi < cls->methods->len)) {
#line 2178 "src/sema.tr"
        TrStr mnm = _tr_str_retain(((FunctionDef*)List_ptr_get(cls->methods, mi))->name);
#line 2179 "src/sema.tr"
        if (_tr_str_eqv((mnm), (_tr_str_lit_len("free", 4LL)))) {
#line 2188 "src/sema.tr"
            if (sym->coll_escaped) {
#line 2188 "src/sema.tr"
                _tr_str_release(tn);
                _tr_str_release(mnm);
                return false;
            }
#line 2189 "src/sema.tr"
            _tr_str_release(tn);
            _tr_str_release(mnm);
            return true;
        }
#line 2190 "src/sema.tr"
        if (_tr_str_eqv((mnm), (_tr_str_lit_len("dispose", 7LL)))) {
#line 2190 "src/sema.tr"
            has_dispose = true;
        }
#line 2191 "src/sema.tr"
        mi = (mi + 1LL);
        _tr_str_release(mnm);
    }
#line 2193 "src/sema.tr"
    if (has_dispose) {
#line 2193 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 2199 "src/sema.tr"
    _tr_str_release(tn);
    return true;
}

__attribute__((hot)) void Sema_open_block(Sema* self) {
#line 2203 "src/sema.tr"
    self->next_block_id = (self->next_block_id + 1LL);
#line 2204 "src/sema.tr"
    List_i64_append(self->block_stack, self->next_block_id);
}

__attribute__((hot)) void Sema_close_block(Sema* self) {
#line 2208 "src/sema.tr"
    long long closing = 0LL;
#line 2209 "src/sema.tr"
    if ((self->block_stack->len > 0LL)) {
#line 2209 "src/sema.tr"
        closing = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
    }
#line 2211 "src/sema.tr"
    while (((self->shadow_blocks->len > 0LL) && (List_i64_get(self->shadow_blocks, (self->shadow_blocks->len - 1LL)) == closing))) {
#line 2212 "src/sema.tr"
        long long li = (self->shadow_blocks->len - 1LL);
#line 2213 "src/sema.tr"
        long long si = List_i64_get(self->shadow_scopes, li);
#line 2214 "src/sema.tr"
        if (((si >= 0LL) && (si < self->scopes->len))) {
#line 2215 "src/sema.tr"
            Scope* sc = ((Scope*)List_ptr_get(self->scopes, si));
#line 2216 "src/sema.tr"
            ({ TrStr _ord_t1204 = (List_TrStr_get(self->shadow_names, li)); __auto_type _ord_t1205 = (((Symbol*)List_ptr_get(self->shadow_prevs, li))); _tr_dict_set(sc->variables, _tr_strz(_ord_t1204), _ord_t1205); _tr_str_release(_ord_t1204); });
        }
#line 2217 "src/sema.tr"
        List_TrStr_pop(self->shadow_names);
#line 2218 "src/sema.tr"
        ((Symbol*)List_ptr_pop(self->shadow_prevs));
#line 2219 "src/sema.tr"
        List_i64_pop(self->shadow_blocks);
#line 2220 "src/sema.tr"
        List_i64_pop(self->shadow_scopes);
    }
#line 2221 "src/sema.tr"
    List_i64_pop(self->block_stack);
}

__attribute__((hot)) bool Sema_block_stack_contains(Sema* self, long long id) {
#line 2225 "src/sema.tr"
    long long i = 0LL;
#line 2226 "src/sema.tr"
    while ((i < self->block_stack->len)) {
#line 2227 "src/sema.tr"
        if ((List_i64_get(self->block_stack, i) == id)) {
#line 2227 "src/sema.tr"
            return true;
        }
#line 2228 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2229 "src/sema.tr"
    return false;
}

__attribute__((hot)) List_ptr* Sema_compute_scope_drops(Sema* self, long long scope_idx, bool is_innermost) {
#line 2236 "src/sema.tr"
    List_ptr* out = (void*)List_ptr_new();
#line 2237 "src/sema.tr"
    if (((scope_idx < 0LL) || (scope_idx >= self->scopes->len))) {
#line 2237 "src/sema.tr"
        return out;
    }
#line 2238 "src/sema.tr"
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, scope_idx));
#line 2239 "src/sema.tr"
    long long ki = 0LL;
#line 2240 "src/sema.tr"
    while ((ki < scope->decl_order->len)) {
#line 2241 "src/sema.tr"
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
#line 2247 "src/sema.tr"
        if (_tr_dict_contains(scope->variables, _tr_strz(nm))) {
#line 2248 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
#line 2249 "src/sema.tr"
            if ((((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL)))) {
#line 2250 "src/sema.tr"
                bool visible = (sym->decl_block_id == 0LL);
#line 2251 "src/sema.tr"
                if (((!visible) && is_innermost)) {
#line 2252 "src/sema.tr"
                    visible = Sema_block_stack_contains(self, sym->decl_block_id);
                }
#line 2253 "src/sema.tr"
                if (visible) {
#line 2254 "src/sema.tr"
                    if (Sema_is_droppable_sym(self, sym)) {
#line 2255 "src/sema.tr"
                        List_ptr_append(out, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
                    }
                }
            }
        }
#line 2256 "src/sema.tr"
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
#line 2257 "src/sema.tr"
    return out;
}

__attribute__((hot)) void Sema_append_drops_from(Sema* self, HirBlock* hb, long long from_idx) {
#line 2262 "src/sema.tr"
    Sema_append_drops_from_excl(self, hb, from_idx, _tr_str_lit_len("", 0LL));
}

__attribute__((hot)) void Sema_append_drops_from_excl(Sema* self, HirBlock* hb, long long from_idx, TrStr exclude) {
#line 2268 "src/sema.tr"
    List_TrStr* excl_list = (void*)List_TrStr_new();
#line 2269 "src/sema.tr"
    if ((!_tr_str_eqv((exclude), (_tr_str_lit_len("", 0LL))))) {
#line 2269 "src/sema.tr"
        List_TrStr_append(excl_list, exclude);
    }
#line 2270 "src/sema.tr"
    Sema_append_drops_from_excl_multi(self, hb, from_idx, excl_list);
}

__attribute__((hot)) void Sema_append_drops_from_excl_multi(Sema* self, HirBlock* hb, long long from_idx, List_TrStr* excludes) {
#line 2279 "src/sema.tr"
    long long idx = (self->scopes->len - 1LL);
#line 2280 "src/sema.tr"
    while ((idx >= from_idx)) {
#line 2281 "src/sema.tr"
        bool is_innermost = (idx == (self->scopes->len - 1LL));
#line 2282 "src/sema.tr"
        List_ptr* drops = Sema_compute_scope_drops(self, idx, is_innermost);
#line 2283 "src/sema.tr"
        long long di = 0LL;
#line 2284 "src/sema.tr"
        while ((di < drops->len)) {
#line 2285 "src/sema.tr"
            HirStmt* d = ((HirStmt*)List_ptr_get(drops, di));
#line 2286 "src/sema.tr"
            bool skip = false;
#line 2287 "src/sema.tr"
            __auto_type _t1206 = (*d);
            if (_t1206.tag == HirStmt_SAutoDrop) {
                __auto_type dn = _t1206.data.SAutoDrop.name;
#line 2289 "src/sema.tr"
                long long ei = 0LL;
#line 2290 "src/sema.tr"
                while ((ei < excludes->len)) {
#line 2291 "src/sema.tr"
                    if (({ TrStr _cmpa_t1207 = (List_TrStr_get(excludes, ei)); TrStr _cmpb_t1208 = (dn); _Bool _cmpr_t1209 = _tr_str_eqv(_cmpa_t1207, _cmpb_t1208); _tr_str_release(_cmpa_t1207); _cmpr_t1209; })) {
#line 2291 "src/sema.tr"
                        skip = true;
                    }
#line 2292 "src/sema.tr"
                    ei = (ei + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1206;
                /* pass */
            }
#line 2294 "src/sema.tr"
            if ((!skip)) {
#line 2294 "src/sema.tr"
                HirBlock_push(hb, d);
            }
#line 2295 "src/sema.tr"
            di = (di + 1LL);
        }
#line 2296 "src/sema.tr"
        idx = (idx - 1LL);
    }
}

__attribute__((hot)) AstType** Sema_closure_hint_for_fn(Sema* self, TrStr fname, long long k) {
#line 2305 "src/sema.tr"
    AstType** nohint = (AstType**)(0LL);
#line 2306 "src/sema.tr"
    if ((!_tr_dict_contains(self->fn_sigs, _tr_strz(fname)))) {
#line 2306 "src/sema.tr"
        return nohint;
    }
#line 2307 "src/sema.tr"
    AstType* sig = ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(fname)));
#line 2308 "src/sema.tr"
    if ((k >= (sig->args->len - 1LL))) {
#line 2308 "src/sema.tr"
        AstType** _trret204 = nohint;
        return _trret204;
    }
#line 2309 "src/sema.tr"
    AstType** pt = ((AstType**)List_ptr_get(sig->args, k));
#line 2310 "src/sema.tr"
    if ((((unsigned long long)(pt)) == ((unsigned long long)(0LL)))) {
#line 2310 "src/sema.tr"
        AstType** _trret205 = nohint;
        return _trret205;
    }
#line 2311 "src/sema.tr"
    if (_tr_str_eqv(((*pt)->name), (_tr_str_lit_len("def", 3LL)))) {
#line 2311 "src/sema.tr"
        AstType** _trret206 = pt;
        return _trret206;
    }
#line 2312 "src/sema.tr"
    AstType** _trret207 = nohint;
    return _trret207;
}

__attribute__((hot)) AstType** Sema_closure_hint_for_method(Sema* self, TrStr method, long long k) {
#line 2318 "src/sema.tr"
    AstType** nohint = (AstType**)(0LL);
#line 2319 "src/sema.tr"
    AstType** found = (AstType**)(0LL);
#line 2320 "src/sema.tr"
    bool ambiguous = false;
#line 2321 "src/sema.tr"
    List_TrStr* keys = _tr_dict_keys(self->classes);
#line 2322 "src/sema.tr"
    long long ci = 0LL;
#line 2323 "src/sema.tr"
    while ((ci < keys->len)) {
#line 2324 "src/sema.tr"
        ClassDef* cd = ({ TrStr _ord_t1210 = (List_TrStr_get(keys, ci)); __auto_type _wr = (((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_ord_t1210)))); _tr_str_release(_ord_t1210); _wr; });
#line 2325 "src/sema.tr"
        long long mi = 0LL;
#line 2326 "src/sema.tr"
        while ((mi < cd->methods->len)) {
#line 2327 "src/sema.tr"
            FunctionDef* md = ((FunctionDef*)List_ptr_get(cd->methods, mi));
#line 2328 "src/sema.tr"
            if (_tr_str_eqv((md->name), (method))) {
#line 2329 "src/sema.tr"
                long long idx = 0LL;
#line 2330 "src/sema.tr"
                long long pi = 0LL;
#line 2331 "src/sema.tr"
                while ((pi < md->params->len)) {
#line 2332 "src/sema.tr"
                    Param* prm = ((Param*)List_ptr_get(md->params, pi));
#line 2333 "src/sema.tr"
                    if ((!_tr_str_eqv((prm->name), (_tr_str_lit_len("self", 4LL))))) {
#line 2334 "src/sema.tr"
                        if ((((idx == k) && (((unsigned long long)(prm->ty)) != ((unsigned long long)(0LL)))) && _tr_str_eqv(((*prm->ty)->name), (_tr_str_lit_len("def", 3LL))))) {
#line 2335 "src/sema.tr"
                            if ((((unsigned long long)(found)) == ((unsigned long long)(0LL)))) {
#line 2336 "src/sema.tr"
                                found = prm->ty;
                            } else if (({ __auto_type _ord_t1211 = ((*found)->args->len); __auto_type _wr = ((_ord_t1211 != (*prm->ty)->args->len)); _wr; })) {
#line 2338 "src/sema.tr"
                                ambiguous = true;
                            }
                        }
#line 2339 "src/sema.tr"
                        idx = (idx + 1LL);
                    }
#line 2340 "src/sema.tr"
                    pi = (pi + 1LL);
                }
            }
#line 2341 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 2342 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 2343 "src/sema.tr"
    if (ambiguous) {
#line 2343 "src/sema.tr"
        AstType** _trret208 = nohint;
        List_TrStr_free(keys);
        return _trret208;
    }
#line 2344 "src/sema.tr"
    AstType** _trret209 = found;
    List_TrStr_free(keys);
    return _trret209;
}

__attribute__((hot)) AstType** Sema_list_hint_for_fn(Sema* self, TrStr fname, long long k) {
#line 2349 "src/sema.tr"
    AstType** nohint = (AstType**)(0LL);
#line 2350 "src/sema.tr"
    if ((!_tr_dict_contains(self->fn_sigs, _tr_strz(fname)))) {
#line 2350 "src/sema.tr"
        return nohint;
    }
#line 2351 "src/sema.tr"
    AstType* sig = ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(fname)));
#line 2352 "src/sema.tr"
    if ((k >= (sig->args->len - 1LL))) {
#line 2352 "src/sema.tr"
        AstType** _trret210 = nohint;
        return _trret210;
    }
#line 2353 "src/sema.tr"
    AstType** pt = ((AstType**)List_ptr_get(sig->args, k));
#line 2354 "src/sema.tr"
    if ((((unsigned long long)(pt)) == ((unsigned long long)(0LL)))) {
#line 2354 "src/sema.tr"
        AstType** _trret211 = nohint;
        return _trret211;
    }
#line 2355 "src/sema.tr"
    TrStr pn = _tr_str_retain((*pt)->name);
#line 2356 "src/sema.tr"
    if (((_tr_str_eqv((pn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((pn), (_tr_str_lit_len("Vec", 3LL)))) && ((*pt)->args->len > 0LL))) {
#line 2356 "src/sema.tr"
        AstType** _trret212 = pt;
        _tr_str_release(pn);
        return _trret212;
    }
#line 2357 "src/sema.tr"
    AstType** _trret213 = nohint;
    _tr_str_release(pn);
    return _trret213;
}

__attribute__((hot)) AstType** Sema_list_hint_for_method(Sema* self, TrStr method, long long k) {
#line 2362 "src/sema.tr"
    AstType** nohint = (AstType**)(0LL);
#line 2363 "src/sema.tr"
    AstType** found = (AstType**)(0LL);
#line 2364 "src/sema.tr"
    List_TrStr* keys = _tr_dict_keys(self->classes);
#line 2365 "src/sema.tr"
    long long ci = 0LL;
#line 2366 "src/sema.tr"
    while ((ci < keys->len)) {
#line 2367 "src/sema.tr"
        ClassDef* cd = ({ TrStr _ord_t1212 = (List_TrStr_get(keys, ci)); __auto_type _wr = (((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_ord_t1212)))); _tr_str_release(_ord_t1212); _wr; });
#line 2368 "src/sema.tr"
        long long mi = 0LL;
#line 2369 "src/sema.tr"
        while ((mi < cd->methods->len)) {
#line 2370 "src/sema.tr"
            FunctionDef* md = ((FunctionDef*)List_ptr_get(cd->methods, mi));
#line 2371 "src/sema.tr"
            if (_tr_str_eqv((md->name), (method))) {
#line 2372 "src/sema.tr"
                long long idx = 0LL;
#line 2373 "src/sema.tr"
                long long pi = 0LL;
#line 2374 "src/sema.tr"
                while ((pi < md->params->len)) {
#line 2375 "src/sema.tr"
                    Param* prm = ((Param*)List_ptr_get(md->params, pi));
#line 2376 "src/sema.tr"
                    if ((!_tr_str_eqv((prm->name), (_tr_str_lit_len("self", 4LL))))) {
#line 2377 "src/sema.tr"
                        if (((idx == k) && (((unsigned long long)(prm->ty)) != ((unsigned long long)(0LL))))) {
#line 2378 "src/sema.tr"
                            TrStr pn = _tr_str_retain((*prm->ty)->name);
#line 2379 "src/sema.tr"
                            if (((_tr_str_eqv((pn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((pn), (_tr_str_lit_len("Vec", 3LL)))) && ((*prm->ty)->args->len > 0LL))) {
#line 2380 "src/sema.tr"
                                TrStr en = _tr_str_retain((*((AstType**)List_ptr_get((*prm->ty)->args, 0LL)))->name);
#line 2381 "src/sema.tr"
                                if ((((unsigned long long)(found)) == ((unsigned long long)(0LL)))) {
#line 2382 "src/sema.tr"
                                    found = prm->ty;
                                } else if (((!_tr_str_eqv(((*found)->name), (pn))) || (!_tr_str_eqv(((*((AstType**)List_ptr_get((*found)->args, 0LL)))->name), (en))))) {
#line 2384 "src/sema.tr"
                                    AstType** _trret214 = nohint;
                                    List_TrStr_free(keys);
                                    _tr_str_release(pn);
                                    _tr_str_release(en);
                                    return _trret214;
                                }
                            } else {
#line 2386 "src/sema.tr"
                                AstType** _trret215 = nohint;
                                List_TrStr_free(keys);
                                _tr_str_release(pn);
                                return _trret215;
                            }
                        }
#line 2387 "src/sema.tr"
                        idx = (idx + 1LL);
                    }
#line 2388 "src/sema.tr"
                    pi = (pi + 1LL);
                }
            }
#line 2389 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 2390 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 2391 "src/sema.tr"
    AstType** _trret216 = found;
    List_TrStr_free(keys);
    return _trret216;
}

__attribute__((hot)) void Sema_collect_idents(Sema* self, HirExpr* e, List_TrStr* out) {
#line 2394 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2394 "src/sema.tr"
        return;
    }
#line 2395 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2395 "src/sema.tr"
        return;
    }
#line 2396 "src/sema.tr"
    __auto_type _t1213 = (*e);
    if (_t1213.tag == HirExpr_EIdent) {
        __auto_type nm = _t1213.data.EIdent.name;
        List_TrStr_append(out, nm);
    } else if (_t1213.tag == HirExpr_EBinOp) {
        __auto_type l = _t1213.data.EBinOp.left;
__auto_type r = _t1213.data.EBinOp.right;
#line 2399 "src/sema.tr"
        Sema_collect_idents(self, l, out);
#line 2400 "src/sema.tr"
        Sema_collect_idents(self, r, out);
    } else if (_t1213.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t1213.data.EUnaryOp.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1213.tag == HirExpr_ECall) {
        __auto_type callee = _t1213.data.ECall.callee;
__auto_type args = _t1213.data.ECall.args;
#line 2403 "src/sema.tr"
        Sema_collect_idents(self, callee, out);
#line 2404 "src/sema.tr"
        long long i = 0LL;
#line 2405 "src/sema.tr"
        while ((i < args->len)) {
#line 2406 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(args, i)), out);
#line 2407 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1213.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1213.data.EMethodCall.obj;
__auto_type args = _t1213.data.EMethodCall.args;
#line 2409 "src/sema.tr"
        Sema_collect_idents(self, obj, out);
#line 2410 "src/sema.tr"
        long long i = 0LL;
#line 2411 "src/sema.tr"
        while ((i < args->len)) {
#line 2412 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(args, i)), out);
#line 2413 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1213.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t1213.data.EPropAccess.obj;
        Sema_collect_idents(self, obj, out);
    } else if (_t1213.tag == HirExpr_EIndex) {
        __auto_type obj = _t1213.data.EIndex.obj;
__auto_type idx = _t1213.data.EIndex._tr_v_index;
#line 2416 "src/sema.tr"
        Sema_collect_idents(self, obj, out);
#line 2417 "src/sema.tr"
        Sema_collect_idents(self, idx, out);
    } else if (_t1213.tag == HirExpr_ECast) {
        __auto_type inner = _t1213.data.ECast.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1213.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t1213.data.ETryExpr.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1213.tag == HirExpr_EIfElse) {
        __auto_type c = _t1213.data.EIfElse.cond;
__auto_type t = _t1213.data.EIfElse.then_e;
__auto_type f = _t1213.data.EIfElse.else_e;
#line 2421 "src/sema.tr"
        Sema_collect_idents(self, c, out);
#line 2422 "src/sema.tr"
        Sema_collect_idents(self, t, out);
#line 2423 "src/sema.tr"
        Sema_collect_idents(self, f, out);
    } else if (_t1213.tag == HirExpr_EList) {
        __auto_type items = _t1213.data.EList.items;
#line 2425 "src/sema.tr"
        long long i = 0LL;
#line 2426 "src/sema.tr"
        while ((i < items->len)) {
#line 2427 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
#line 2428 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1213.tag == HirExpr_ESet) {
        __auto_type items = _t1213.data.ESet.items;
#line 2430 "src/sema.tr"
        long long i = 0LL;
#line 2431 "src/sema.tr"
        while ((i < items->len)) {
#line 2432 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
#line 2433 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1213.tag == HirExpr_ETuple) {
        __auto_type items = _t1213.data.ETuple.items;
#line 2435 "src/sema.tr"
        long long i = 0LL;
#line 2436 "src/sema.tr"
        while ((i < items->len)) {
#line 2437 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
#line 2438 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1213.tag == HirExpr_EDict) {
        __auto_type keys = _t1213.data.EDict.keys;
__auto_type vals = _t1213.data.EDict.vals;
#line 2440 "src/sema.tr"
        long long i = 0LL;
#line 2441 "src/sema.tr"
        while ((i < keys->len)) {
#line 2442 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(keys, i)), out);
#line 2443 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(vals, i)), out);
#line 2444 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1213.tag == HirExpr_ERange) {
        __auto_type s = _t1213.data.ERange.start;
__auto_type en = _t1213.data.ERange.end;
#line 2446 "src/sema.tr"
        Sema_collect_idents(self, s, out);
#line 2447 "src/sema.tr"
        Sema_collect_idents(self, en, out);
    } else if (_t1213.tag == HirExpr_EAwait) {
        __auto_type inner = _t1213.data.EAwait.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1213.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t1213.data.EAwaitTimeout.expr;
__auto_type to = _t1213.data.EAwaitTimeout.timeout_ms;
#line 2450 "src/sema.tr"
        Sema_collect_idents(self, inner, out);
#line 2451 "src/sema.tr"
        Sema_collect_idents(self, to, out);
    } else if (_t1213.tag == HirExpr_EYield) {
        __auto_type inner = _t1213.data.EYield.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1213.tag == HirExpr_EFString) {
        __auto_type fparts = _t1213.data.EFString.parts;
#line 2456 "src/sema.tr"
        long long fpi = 0LL;
#line 2457 "src/sema.tr"
        while ((fpi < fparts->len)) {
#line 2458 "src/sema.tr"
            HirFStringPart* fpart = ((HirFStringPart*)List_ptr_get(fparts, fpi));
#line 2459 "src/sema.tr"
            if ((fpart->is_expr && (((unsigned long long)(fpart->expr)) != ((unsigned long long)(0LL))))) {
#line 2460 "src/sema.tr"
                Sema_collect_idents(self, fpart->expr, out);
            }
#line 2461 "src/sema.tr"
            long long fsa = 0LL;
#line 2462 "src/sema.tr"
            while ((fsa < fpart->spec_args->len)) {
#line 2463 "src/sema.tr"
                Sema_collect_idents(self, ((HirExpr*)List_ptr_get(fpart->spec_args, fsa)), out);
#line 2464 "src/sema.tr"
                fsa = (fsa + 1LL);
            }
#line 2465 "src/sema.tr"
            fpi = (fpi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1213;
        /* pass */
    }
}

__attribute__((hot)) bool Sema_is_local_var(Sema* self, TrStr name) {
#line 2470 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 2471 "src/sema.tr"
    while ((i >= 0LL)) {
#line 2472 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 2472 "src/sema.tr"
            return true;
        }
#line 2473 "src/sema.tr"
        i = (i - 1LL);
    }
#line 2474 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema_mark_str_escaped(Sema* self, TrStr name) {
#line 2477 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 2478 "src/sema.tr"
    while ((i >= 0LL)) {
#line 2479 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 2480 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 2481 "src/sema.tr"
            sym->str_escaped = true;
#line 2482 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 2483 "src/sema.tr"
            return;
        }
#line 2484 "src/sema.tr"
        i = (i - 1LL);
    }
}

__attribute__((hot)) void Sema_set_borrows_region(Sema* self, TrStr name, TrStr region) {
#line 2489 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 2490 "src/sema.tr"
    while ((i >= 0LL)) {
#line 2491 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 2492 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 2493 "src/sema.tr"
            sym->borrows_region = _tr_str_retain(region);
#line 2494 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 2495 "src/sema.tr"
            return;
        }
#line 2496 "src/sema.tr"
        i = (i - 1LL);
    }
}

__attribute__((hot)) TrStr Sema_compute_region(Sema* self, Expr* e) {
#line 2504 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2504 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 2505 "src/sema.tr"
    __auto_type _t1214 = (*e);
    if (_t1214.tag == Expr_EBinOp) {
        __auto_type cr_op = _t1214.data.EBinOp.op;
#line 2507 "src/sema.tr"
        if (_tr_str_eqv((cr_op), (_tr_str_lit_len("+", 1LL)))) {
#line 2507 "src/sema.tr"
            return _tr_str_lit_len("@owned", 6LL);
        }
#line 2508 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t1214.tag == Expr_EIdent) {
        __auto_type cr_nm = _t1214.data.EIdent.name;
#line 2510 "src/sema.tr"
        Symbol* cr_sym = Sema_resolve(self, cr_nm);
#line 2511 "src/sema.tr"
        if (_tr_str_eqv((cr_sym->name), (_tr_str_lit_len("", 0LL)))) {
#line 2511 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 2512 "src/sema.tr"
        if (cr_sym->is_param) {
#line 2512 "src/sema.tr"
            TrStr _trret217 = _tr_str_retain(cr_nm);
            return _trret217;
        }
#line 2513 "src/sema.tr"
        TrStr _trret218 = _tr_str_retain(cr_sym->borrows_region);
        return _trret218;
    } else if (_t1214.tag == Expr_ECall) {
        __auto_type cr_callee = _t1214.data.ECall.callee;
__auto_type cr_args = _t1214.data.ECall.args;
#line 2515 "src/sema.tr"
        if ((((unsigned long long)(cr_callee)) != ((unsigned long long)(0LL)))) {
#line 2516 "src/sema.tr"
            __auto_type _t1215 = (*cr_callee);
            if (_t1215.tag == Expr_EIdent) {
                __auto_type cr_fn = _t1215.data.EIdent.name;
#line 2519 "src/sema.tr"
                if (_tr_dict_contains(self->classes, _tr_strz(cr_fn))) {
#line 2519 "src/sema.tr"
                    return _tr_str_lit_len("@owned", 6LL);
                }
#line 2520 "src/sema.tr"
                Symbol* cr_fsym = Sema_resolve(self, cr_fn);
#line 2521 "src/sema.tr"
                if (((!_tr_str_eqv((cr_fsym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(cr_fsym->ty)) != ((unsigned long long)(0LL))))) {
#line 2522 "src/sema.tr"
                    AstType* cr_ret = (*cr_fsym->ty);
#line 2523 "src/sema.tr"
                    if (cr_ret->is_borrow) {
#line 2525 "src/sema.tr"
                        if (((cr_ret->from_index >= 0LL) && (cr_ret->from_index < cr_args->len))) {
#line 2526 "src/sema.tr"
                            TrStr _trret219 = Sema_compute_region(self, ((Expr*)List_ptr_get(cr_args, cr_ret->from_index)));
                            return _trret219;
                        }
#line 2527 "src/sema.tr"
                        return _tr_str_lit_len("", 0LL);
                    }
#line 2529 "src/sema.tr"
                    return _tr_str_lit_len("@owned", 6LL);
                }
            } else if (1) {
                __auto_type _ = _t1215;
                /* pass */
            }
        }
#line 2531 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t1214;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool Sema_region_outlives(Sema* self, TrStr longer, TrStr shorter) {
#line 2538 "src/sema.tr"
    long long i = 0LL;
#line 2539 "src/sema.tr"
    while ((i < self->current_func_outlives_a->len)) {
#line 2540 "src/sema.tr"
        if ((({ TrStr _cmpa_t1216 = (List_TrStr_get(self->current_func_outlives_a, i)); TrStr _cmpb_t1217 = (longer); _Bool _cmpr_t1218 = _tr_str_eqv(_cmpa_t1216, _cmpb_t1217); _tr_str_release(_cmpa_t1216); _cmpr_t1218; }) && ({ TrStr _cmpa_t1219 = (List_TrStr_get(self->current_func_outlives_b, i)); TrStr _cmpb_t1220 = (shorter); _Bool _cmpr_t1221 = _tr_str_eqv(_cmpa_t1219, _cmpb_t1220); _tr_str_release(_cmpa_t1219); _cmpr_t1221; }))) {
#line 2541 "src/sema.tr"
            return true;
        }
#line 2542 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2543 "src/sema.tr"
    long long j = 0LL;
#line 2544 "src/sema.tr"
    while ((j < self->current_func_outlives_a->len)) {
#line 2545 "src/sema.tr"
        if (({ TrStr _cmpa_t1222 = (List_TrStr_get(self->current_func_outlives_a, j)); TrStr _cmpb_t1223 = (longer); _Bool _cmpr_t1224 = _tr_str_eqv(_cmpa_t1222, _cmpb_t1223); _tr_str_release(_cmpa_t1222); _cmpr_t1224; })) {
#line 2546 "src/sema.tr"
            TrStr mid = List_TrStr_get(self->current_func_outlives_b, j);
#line 2547 "src/sema.tr"
            long long kk = 0LL;
#line 2548 "src/sema.tr"
            while ((kk < self->current_func_outlives_a->len)) {
#line 2549 "src/sema.tr"
                if ((({ TrStr _cmpa_t1225 = (List_TrStr_get(self->current_func_outlives_a, kk)); TrStr _cmpb_t1226 = (mid); _Bool _cmpr_t1227 = _tr_str_eqv(_cmpa_t1225, _cmpb_t1226); _tr_str_release(_cmpa_t1225); _cmpr_t1227; }) && ({ TrStr _cmpa_t1228 = (List_TrStr_get(self->current_func_outlives_b, kk)); TrStr _cmpb_t1229 = (shorter); _Bool _cmpr_t1230 = _tr_str_eqv(_cmpa_t1228, _cmpb_t1229); _tr_str_release(_cmpa_t1228); _cmpr_t1230; }))) {
#line 2550 "src/sema.tr"
                    _tr_str_release(mid);
                    return true;
                }
#line 2551 "src/sema.tr"
                kk = (kk + 1LL);
            }
        }
#line 2552 "src/sema.tr"
        j = (j + 1LL);
    }
#line 2553 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_field_is_borrow(Sema* self, Expr* obj, TrStr field) {
#line 2558 "src/sema.tr"
    if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
#line 2558 "src/sema.tr"
        return false;
    }
#line 2559 "src/sema.tr"
    TrStr cls_name = _tr_str_lit_len("", 0LL);
#line 2560 "src/sema.tr"
    __auto_type _t1231 = (*obj);
    if (_t1231.tag == Expr_EIdent) {
        __auto_type fb_obj = _t1231.data.EIdent.name;
#line 2562 "src/sema.tr"
        if (_tr_str_eqv((fb_obj), (_tr_str_lit_len("self", 4LL)))) {
#line 2563 "src/sema.tr"
            TrStr _strtmp_t1232 = _tr_str_retain(self->current_class_name);
            _tr_str_release(cls_name);
            cls_name = _strtmp_t1232;
        } else {
#line 2565 "src/sema.tr"
            Symbol* fb_sym = Sema_resolve(self, fb_obj);
#line 2566 "src/sema.tr"
            if (((!_tr_str_eqv((fb_sym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(fb_sym->ty)) != ((unsigned long long)(0LL))))) {
#line 2567 "src/sema.tr"
                TrStr _strtmp_t1233 = _tr_str_retain((*fb_sym->ty)->name);
                _tr_str_release(cls_name);
                cls_name = _strtmp_t1233;
            }
        }
    } else if (1) {
        __auto_type _ = _t1231;
        /* pass */
    }
#line 2569 "src/sema.tr"
    if (_tr_str_eqv((cls_name), (_tr_str_lit_len("", 0LL)))) {
#line 2569 "src/sema.tr"
        _tr_str_release(cls_name);
        return false;
    }
#line 2570 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
#line 2570 "src/sema.tr"
        _tr_str_release(cls_name);
        return false;
    }
#line 2571 "src/sema.tr"
    ClassDef* fb_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
#line 2572 "src/sema.tr"
    long long fb_i = 0LL;
#line 2573 "src/sema.tr"
    while ((fb_i < fb_cls->fields->len)) {
#line 2574 "src/sema.tr"
        FieldDef* fb_fld = ((FieldDef*)List_ptr_get(fb_cls->fields, fb_i));
#line 2575 "src/sema.tr"
        if (_tr_str_eqv((fb_fld->name), (field))) {
#line 2576 "src/sema.tr"
            if ((((unsigned long long)(fb_fld->ty)) != ((unsigned long long)(0LL)))) {
#line 2577 "src/sema.tr"
                bool _trret220 = (*fb_fld->ty)->is_borrow;
                _tr_str_release(cls_name);
                return _trret220;
            }
        }
#line 2578 "src/sema.tr"
        fb_i = (fb_i + 1LL);
    }
#line 2579 "src/sema.tr"
    _tr_str_release(cls_name);
    return false;
}

__attribute__((hot)) void Sema_mark_str_arg(Sema* self, HirExpr* e) {
#line 2587 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2587 "src/sema.tr"
        return;
    }
#line 2588 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2588 "src/sema.tr"
        return;
    }
#line 2589 "src/sema.tr"
    __auto_type _t1234 = (*e);
    if (_t1234.tag == HirExpr_EIdent) {
        __auto_type nm = _t1234.data.EIdent.name;
#line 2591 "src/sema.tr"
        if (_tr_str_eqv((hir_expr_type(e)->name), (_tr_str_lit_len("str", 3LL)))) {
#line 2591 "src/sema.tr"
            Sema_mark_str_escaped(self, nm);
        }
    } else if (1) {
        __auto_type _ = _t1234;
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_escaped_str_args(Sema* self, HirExpr* e) {
#line 2597 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2597 "src/sema.tr"
        return;
    }
#line 2598 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2598 "src/sema.tr"
        return;
    }
#line 2599 "src/sema.tr"
    __auto_type _t1235 = (*e);
    if (_t1235.tag == HirExpr_EBinOp) {
        __auto_type l = _t1235.data.EBinOp.left;
__auto_type r = _t1235.data.EBinOp.right;
#line 2601 "src/sema.tr"
        Sema_mark_escaped_str_args(self, l);
#line 2602 "src/sema.tr"
        Sema_mark_escaped_str_args(self, r);
    } else if (_t1235.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t1235.data.EUnaryOp.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1235.tag == HirExpr_ECall) {
        __auto_type callee = _t1235.data.ECall.callee;
__auto_type args = _t1235.data.ECall.args;
#line 2605 "src/sema.tr"
        Sema_mark_escaped_str_args(self, callee);
#line 2616 "src/sema.tr"
        bool _call_is_c_free = false;
#line 2617 "src/sema.tr"
        __auto_type _t1236 = (*callee);
        if (_t1236.tag == HirExpr_EIdent) {
            __auto_type _cnm = _t1236.data.EIdent.name;
#line 2619 "src/sema.tr"
            if (_tr_str_eqv((_cnm), (_tr_str_lit_len("_tr_c_free", 10LL)))) {
#line 2619 "src/sema.tr"
                _call_is_c_free = true;
            }
        } else if (1) {
            __auto_type _ = _t1236;
            /* pass */
        }
#line 2621 "src/sema.tr"
        long long i = 0LL;
#line 2622 "src/sema.tr"
        while ((i < args->len)) {
#line 2623 "src/sema.tr"
            if (_call_is_c_free) {
#line 2631 "src/sema.tr"
                __auto_type _t1237 = (*((HirExpr*)List_ptr_get(args, i)));
                if (_t1237.tag == HirExpr_ECast) {
                    __auto_type _cf_inner = _t1237.data.ECast.expr;
#line 2633 "src/sema.tr"
                    Sema_mark_str_arg(self, _cf_inner);
                } else if (1) {
                    __auto_type _ = _t1237;
                    /* pass */
                }
            }
#line 2635 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2636 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1235.data.EMethodCall.obj;
__auto_type mname = _t1235.data.EMethodCall.method;
__auto_type args = _t1235.data.EMethodCall.args;
#line 2638 "src/sema.tr"
        Sema_mark_escaped_str_args(self, obj);
#line 2657 "src/sema.tr"
        long long i = 0LL;
#line 2658 "src/sema.tr"
        while ((i < args->len)) {
#line 2659 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2660 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t1235.data.EPropAccess.obj;
        Sema_mark_escaped_str_args(self, obj);
    } else if (_t1235.tag == HirExpr_EIndex) {
        __auto_type obj = _t1235.data.EIndex.obj;
__auto_type idx = _t1235.data.EIndex._tr_v_index;
#line 2663 "src/sema.tr"
        Sema_mark_escaped_str_args(self, obj);
#line 2664 "src/sema.tr"
        Sema_mark_escaped_str_args(self, idx);
    } else if (_t1235.tag == HirExpr_ECast) {
        __auto_type inner = _t1235.data.ECast.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1235.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t1235.data.ETryExpr.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1235.tag == HirExpr_EIfElse) {
        __auto_type c = _t1235.data.EIfElse.cond;
__auto_type t = _t1235.data.EIfElse.then_e;
__auto_type f = _t1235.data.EIfElse.else_e;
#line 2668 "src/sema.tr"
        Sema_mark_escaped_str_args(self, c);
#line 2669 "src/sema.tr"
        Sema_mark_escaped_str_args(self, t);
#line 2670 "src/sema.tr"
        Sema_mark_escaped_str_args(self, f);
    } else if (_t1235.tag == HirExpr_EList) {
        __auto_type items = _t1235.data.EList.items;
#line 2672 "src/sema.tr"
        long long i = 0LL;
#line 2673 "src/sema.tr"
        while ((i < items->len)) {
#line 2674 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2675 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_ESet) {
        __auto_type items = _t1235.data.ESet.items;
#line 2677 "src/sema.tr"
        long long i = 0LL;
#line 2678 "src/sema.tr"
        while ((i < items->len)) {
#line 2679 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2680 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_ETuple) {
        __auto_type items = _t1235.data.ETuple.items;
#line 2682 "src/sema.tr"
        long long i = 0LL;
#line 2683 "src/sema.tr"
        while ((i < items->len)) {
#line 2684 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2685 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_EDict) {
        __auto_type keys = _t1235.data.EDict.keys;
__auto_type vals = _t1235.data.EDict.vals;
#line 2687 "src/sema.tr"
        long long i = 0LL;
#line 2688 "src/sema.tr"
        while ((i < keys->len)) {
#line 2689 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(keys, i)));
#line 2690 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(vals, i)));
#line 2691 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_ERange) {
        __auto_type s = _t1235.data.ERange.start;
__auto_type en = _t1235.data.ERange.end;
#line 2693 "src/sema.tr"
        Sema_mark_escaped_str_args(self, s);
#line 2694 "src/sema.tr"
        Sema_mark_escaped_str_args(self, en);
    } else if (_t1235.tag == HirExpr_EAwait) {
        __auto_type inner = _t1235.data.EAwait.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1235.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t1235.data.EAwaitTimeout.expr;
__auto_type to = _t1235.data.EAwaitTimeout.timeout_ms;
#line 2697 "src/sema.tr"
        Sema_mark_escaped_str_args(self, inner);
#line 2698 "src/sema.tr"
        Sema_mark_escaped_str_args(self, to);
    } else if (_t1235.tag == HirExpr_EYield) {
        __auto_type inner = _t1235.data.EYield.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1235.tag == HirExpr_EFString) {
        __auto_type parts = _t1235.data.EFString.parts;
#line 2703 "src/sema.tr"
        long long i = 0LL;
#line 2704 "src/sema.tr"
        while ((i < parts->len)) {
#line 2705 "src/sema.tr"
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, i));
#line 2706 "src/sema.tr"
            if (part->is_expr) {
#line 2706 "src/sema.tr"
                Sema_mark_escaped_str_args(self, part->expr);
            }
#line 2707 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_EClosure) {
        __auto_type captures = _t1235.data.EClosure.captures;
#line 2714 "src/sema.tr"
        long long i = 0LL;
#line 2715 "src/sema.tr"
        while ((i < captures->len)) {
#line 2716 "src/sema.tr"
            HirParam* cap = ((HirParam*)List_ptr_get(captures, i));
#line 2717 "src/sema.tr"
            if (_tr_str_eqv((cap->ty->name), (_tr_str_lit_len("str", 3LL)))) {
#line 2717 "src/sema.tr"
                Sema_mark_str_escaped(self, cap->name);
            }
#line 2718 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t1235.data.ESuperMethodCall.args;
#line 2722 "src/sema.tr"
        long long i = 0LL;
#line 2723 "src/sema.tr"
        while ((i < args->len)) {
#line 2724 "src/sema.tr"
            Sema_mark_str_arg(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2725 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2726 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_ESuperPropAccess) {
        /* pass */
    } else if (_t1235.tag == HirExpr_EListComp) {
        __auto_type element = _t1235.data.EListComp.element;
__auto_type generators = _t1235.data.EListComp.generators;
#line 2732 "src/sema.tr"
        Sema_mark_str_arg(self, element);
#line 2733 "src/sema.tr"
        Sema_mark_escaped_str_args(self, element);
#line 2734 "src/sema.tr"
        long long i = 0LL;
#line 2735 "src/sema.tr"
        while ((i < generators->len)) {
#line 2736 "src/sema.tr"
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
#line 2737 "src/sema.tr"
            Sema_mark_escaped_str_args(self, gen->iter);
#line 2738 "src/sema.tr"
            long long j = 0LL;
#line 2739 "src/sema.tr"
            while ((j < gen->ifs->len)) {
#line 2740 "src/sema.tr"
                Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
#line 2741 "src/sema.tr"
                j = (j + 1LL);
            }
#line 2742 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_EGeneratorExpr) {
        __auto_type element = _t1235.data.EGeneratorExpr.element;
__auto_type generators = _t1235.data.EGeneratorExpr.generators;
#line 2744 "src/sema.tr"
        Sema_mark_str_arg(self, element);
#line 2745 "src/sema.tr"
        Sema_mark_escaped_str_args(self, element);
#line 2746 "src/sema.tr"
        long long i = 0LL;
#line 2747 "src/sema.tr"
        while ((i < generators->len)) {
#line 2748 "src/sema.tr"
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
#line 2749 "src/sema.tr"
            Sema_mark_escaped_str_args(self, gen->iter);
#line 2750 "src/sema.tr"
            long long j = 0LL;
#line 2751 "src/sema.tr"
            while ((j < gen->ifs->len)) {
#line 2752 "src/sema.tr"
                Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
#line 2753 "src/sema.tr"
                j = (j + 1LL);
            }
#line 2754 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1235.tag == HirExpr_ESlice) {
        __auto_type start = _t1235.data.ESlice.start;
__auto_type stop = _t1235.data.ESlice.stop;
__auto_type step = _t1235.data.ESlice.step;
#line 2756 "src/sema.tr"
        Sema_mark_escaped_str_args(self, start);
#line 2757 "src/sema.tr"
        Sema_mark_escaped_str_args(self, stop);
#line 2758 "src/sema.tr"
        Sema_mark_escaped_str_args(self, step);
    } else if (_t1235.tag == HirExpr_ETry) {
        /* pass */
    } else if (_t1235.tag == HirExpr_EDo) {
        __auto_type _do_b = _t1235.data.EDo.body;
        /* pass */
    } else if (_t1235.tag == HirExpr_EMatchExpr) {
        /* pass */
    } else if (_t1235.tag == HirExpr_ELoop) {
        /* pass */
    } else if (_t1235.tag == HirExpr_EWhileExpr) {
        /* pass */
    } else if (_t1235.tag == HirExpr_ELitInt) {
        /* pass */
    } else if (_t1235.tag == HirExpr_ELitFloat) {
        /* pass */
    } else if (_t1235.tag == HirExpr_ELitStr) {
        /* pass */
    } else if (_t1235.tag == HirExpr_ELitBytes) {
        /* pass */
    } else if (_t1235.tag == HirExpr_ERawStr) {
        /* pass */
    } else if (_t1235.tag == HirExpr_ELitChar) {
        /* pass */
    } else if (_t1235.tag == HirExpr_ELitBool) {
        /* pass */
    } else if (_t1235.tag == HirExpr_ELitNone) {
        __auto_type _ = _t1235.data.ELitNone.ty;
        /* pass */
    } else if (_t1235.tag == HirExpr_ESizeOf) {
        /* pass */
    } else if (_t1235.tag == HirExpr_EIdent) {
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_coll_escaped(Sema* self, TrStr name) {
#line 2787 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 2788 "src/sema.tr"
    while ((i >= 0LL)) {
#line 2789 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 2790 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 2791 "src/sema.tr"
            sym->coll_escaped = true;
#line 2792 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 2793 "src/sema.tr"
            return;
        }
#line 2794 "src/sema.tr"
        i = (i - 1LL);
    }
}

__attribute__((hot)) void Sema_mark_coll_arg(Sema* self, HirExpr* e) {
#line 2803 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2803 "src/sema.tr"
        return;
    }
#line 2804 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2804 "src/sema.tr"
        return;
    }
#line 2805 "src/sema.tr"
    __auto_type _t1238 = (*e);
    if (_t1238.tag == HirExpr_EIdent) {
        __auto_type nm = _t1238.data.EIdent.name;
#line 2807 "src/sema.tr"
        TrStr tn = _tr_str_retain(hir_expr_type(e)->name);
#line 2808 "src/sema.tr"
        if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 2809 "src/sema.tr"
            Sema_mark_coll_escaped(self, nm);
        } else if ((_tr_str_eqv((tn), (_tr_str_lit_len("def", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("lambda", 6LL))))) {
#line 2814 "src/sema.tr"
            Sema_mark_coll_escaped(self, nm);
        } else if (_tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL)))) {
#line 2818 "src/sema.tr"
            Sema_mark_coll_escaped(self, nm);
        } else if (_tr_dict_contains(self->classes, _tr_strz(tn))) {
#line 2825 "src/sema.tr"
            Sema_mark_coll_escaped(self, nm);
        }
        _tr_str_release(tn);
    } else if (1) {
        __auto_type _ = _t1238;
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_escaped_coll_args(Sema* self, HirExpr* e) {
#line 2834 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2834 "src/sema.tr"
        return;
    }
#line 2835 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2835 "src/sema.tr"
        return;
    }
#line 2836 "src/sema.tr"
    __auto_type _t1239 = (*e);
    if (_t1239.tag == HirExpr_EBinOp) {
        __auto_type l = _t1239.data.EBinOp.left;
__auto_type r = _t1239.data.EBinOp.right;
#line 2838 "src/sema.tr"
        Sema_mark_coll_arg(self, l);
#line 2839 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, l);
#line 2840 "src/sema.tr"
        Sema_mark_coll_arg(self, r);
#line 2841 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, r);
    } else if (_t1239.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t1239.data.EUnaryOp.expr;
#line 2843 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2844 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1239.tag == HirExpr_ECall) {
        __auto_type callee = _t1239.data.ECall.callee;
__auto_type args = _t1239.data.ECall.args;
#line 2846 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, callee);
#line 2847 "src/sema.tr"
        long long i = 0LL;
#line 2848 "src/sema.tr"
        while ((i < args->len)) {
#line 2849 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2850 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2851 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1239.data.EMethodCall.obj;
__auto_type method = _t1239.data.EMethodCall.method;
__auto_type args = _t1239.data.EMethodCall.args;
#line 2867 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("values", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("items", 5LL))))) {
#line 2868 "src/sema.tr"
            __auto_type _t1240 = (*obj);
            if (_t1240.tag == HirExpr_EIdent) {
#line 2870 "src/sema.tr"
                AstType* obj_ty = hir_expr_type(obj);
#line 2871 "src/sema.tr"
                if ((_tr_str_eqv((obj_ty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((obj_ty->name), (_tr_str_lit_len("Map", 3LL))))) {
#line 2872 "src/sema.tr"
                    if ((obj_ty->args->len > 1LL)) {
#line 2873 "src/sema.tr"
                        TrStr obj_vt = _tr_str_retain((*((AstType**)List_ptr_get(obj_ty->args, 1LL)))->name);
#line 2874 "src/sema.tr"
                        if ((_tr_str_eqv((obj_vt), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((obj_vt), (_tr_str_lit_len("String", 6LL))))) {
#line 2875 "src/sema.tr"
                            Sema_mark_coll_arg(self, obj);
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1240;
                /* pass */
            }
        }
#line 2884 "src/sema.tr"
        bool _coll_static_call = false;
#line 2885 "src/sema.tr"
        __auto_type _t1241 = (*obj);
        if (_t1241.tag == HirExpr_EIdent) {
            __auto_type _ocnm2 = _t1241.data.EIdent.name;
#line 2887 "src/sema.tr"
            if ((_tr_dict_contains(self->classes, _tr_strz(_ocnm2)) && (!Sema_is_local_var(self, _ocnm2)))) {
#line 2887 "src/sema.tr"
                _coll_static_call = true;
            }
        } else if (1) {
            __auto_type _ = _t1241;
            /* pass */
        }
#line 2889 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, obj);
#line 2890 "src/sema.tr"
        long long i = 0LL;
#line 2891 "src/sema.tr"
        while ((i < args->len)) {
#line 2892 "src/sema.tr"
            if ((!_coll_static_call)) {
#line 2892 "src/sema.tr"
                Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            }
#line 2893 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2894 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t1239.data.EPropAccess.obj;
        Sema_mark_escaped_coll_args(self, obj);
    } else if (_t1239.tag == HirExpr_EIndex) {
        __auto_type obj = _t1239.data.EIndex.obj;
__auto_type idx = _t1239.data.EIndex._tr_v_index;
#line 2897 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, obj);
#line 2898 "src/sema.tr"
        Sema_mark_coll_arg(self, idx);
#line 2899 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, idx);
    } else if (_t1239.tag == HirExpr_ECast) {
        __auto_type inner = _t1239.data.ECast.expr;
#line 2901 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2902 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1239.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t1239.data.ETryExpr.expr;
#line 2904 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2905 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1239.tag == HirExpr_EIfElse) {
        __auto_type c = _t1239.data.EIfElse.cond;
__auto_type t = _t1239.data.EIfElse.then_e;
__auto_type f = _t1239.data.EIfElse.else_e;
#line 2907 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, c);
#line 2908 "src/sema.tr"
        Sema_mark_coll_arg(self, t);
#line 2909 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, t);
#line 2910 "src/sema.tr"
        Sema_mark_coll_arg(self, f);
#line 2911 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, f);
    } else if (_t1239.tag == HirExpr_EList) {
        __auto_type items = _t1239.data.EList.items;
#line 2913 "src/sema.tr"
        long long i = 0LL;
#line 2914 "src/sema.tr"
        while ((i < items->len)) {
#line 2915 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2916 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2917 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_ESet) {
        __auto_type items = _t1239.data.ESet.items;
#line 2919 "src/sema.tr"
        long long i = 0LL;
#line 2920 "src/sema.tr"
        while ((i < items->len)) {
#line 2921 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2922 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2923 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_ETuple) {
        __auto_type items = _t1239.data.ETuple.items;
#line 2925 "src/sema.tr"
        long long i = 0LL;
#line 2926 "src/sema.tr"
        while ((i < items->len)) {
#line 2927 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2928 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2929 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_EDict) {
        __auto_type keys = _t1239.data.EDict.keys;
__auto_type vals = _t1239.data.EDict.vals;
#line 2931 "src/sema.tr"
        long long i = 0LL;
#line 2932 "src/sema.tr"
        while ((i < keys->len)) {
#line 2933 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(keys, i)));
#line 2934 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(keys, i)));
#line 2935 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(vals, i)));
#line 2936 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(vals, i)));
#line 2937 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_ERange) {
        __auto_type s = _t1239.data.ERange.start;
__auto_type en = _t1239.data.ERange.end;
#line 2939 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, s);
#line 2940 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, en);
    } else if (_t1239.tag == HirExpr_EAwait) {
        __auto_type inner = _t1239.data.EAwait.expr;
#line 2942 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2943 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1239.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t1239.data.EAwaitTimeout.expr;
__auto_type to = _t1239.data.EAwaitTimeout.timeout_ms;
#line 2945 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2946 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
#line 2947 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, to);
    } else if (_t1239.tag == HirExpr_EYield) {
        __auto_type inner = _t1239.data.EYield.expr;
#line 2949 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2950 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1239.tag == HirExpr_EFString) {
        __auto_type parts = _t1239.data.EFString.parts;
#line 2954 "src/sema.tr"
        long long i = 0LL;
#line 2955 "src/sema.tr"
        while ((i < parts->len)) {
#line 2956 "src/sema.tr"
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, i));
#line 2957 "src/sema.tr"
            if (part->is_expr) {
#line 2957 "src/sema.tr"
                Sema_mark_escaped_coll_args(self, part->expr);
            }
#line 2958 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_EClosure) {
        __auto_type captures = _t1239.data.EClosure.captures;
#line 2965 "src/sema.tr"
        long long i = 0LL;
#line 2966 "src/sema.tr"
        while ((i < captures->len)) {
#line 2967 "src/sema.tr"
            HirParam* cap = ((HirParam*)List_ptr_get(captures, i));
#line 2968 "src/sema.tr"
            TrStr ctn = _tr_str_retain(cap->ty->name);
#line 2969 "src/sema.tr"
            if (((((_tr_str_eqv((ctn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Set", 3LL))))) {
#line 2970 "src/sema.tr"
                Sema_mark_coll_escaped(self, cap->name);
            }
#line 2971 "src/sema.tr"
            i = (i + 1LL);
            _tr_str_release(ctn);
        }
    } else if (_t1239.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t1239.data.ESuperMethodCall.args;
#line 2975 "src/sema.tr"
        long long i = 0LL;
#line 2976 "src/sema.tr"
        while ((i < args->len)) {
#line 2977 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2978 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2979 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_ESuperPropAccess) {
        /* pass */
    } else if (_t1239.tag == HirExpr_EListComp) {
        __auto_type element = _t1239.data.EListComp.element;
__auto_type generators = _t1239.data.EListComp.generators;
#line 2986 "src/sema.tr"
        Sema_mark_coll_arg(self, element);
#line 2987 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, element);
#line 2988 "src/sema.tr"
        long long i = 0LL;
#line 2989 "src/sema.tr"
        while ((i < generators->len)) {
#line 2990 "src/sema.tr"
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
#line 2991 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, gen->iter);
#line 2992 "src/sema.tr"
            long long j = 0LL;
#line 2993 "src/sema.tr"
            while ((j < gen->ifs->len)) {
#line 2994 "src/sema.tr"
                Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
#line 2995 "src/sema.tr"
                j = (j + 1LL);
            }
#line 2996 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_EGeneratorExpr) {
        __auto_type element = _t1239.data.EGeneratorExpr.element;
__auto_type generators = _t1239.data.EGeneratorExpr.generators;
#line 2998 "src/sema.tr"
        Sema_mark_coll_arg(self, element);
#line 2999 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, element);
#line 3000 "src/sema.tr"
        long long i = 0LL;
#line 3001 "src/sema.tr"
        while ((i < generators->len)) {
#line 3002 "src/sema.tr"
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
#line 3003 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, gen->iter);
#line 3004 "src/sema.tr"
            long long j = 0LL;
#line 3005 "src/sema.tr"
            while ((j < gen->ifs->len)) {
#line 3006 "src/sema.tr"
                Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
#line 3007 "src/sema.tr"
                j = (j + 1LL);
            }
#line 3008 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1239.tag == HirExpr_ESlice) {
        __auto_type start = _t1239.data.ESlice.start;
__auto_type stop = _t1239.data.ESlice.stop;
__auto_type step = _t1239.data.ESlice.step;
#line 3010 "src/sema.tr"
        Sema_mark_coll_arg(self, start);
#line 3011 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, start);
#line 3012 "src/sema.tr"
        Sema_mark_coll_arg(self, stop);
#line 3013 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, stop);
#line 3014 "src/sema.tr"
        Sema_mark_coll_arg(self, step);
#line 3015 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, step);
    } else if (_t1239.tag == HirExpr_ETry) {
        /* pass */
    } else if (_t1239.tag == HirExpr_EDo) {
        __auto_type _do_b = _t1239.data.EDo.body;
        /* pass */
    } else if (_t1239.tag == HirExpr_EMatchExpr) {
        /* pass */
    } else if (_t1239.tag == HirExpr_ELoop) {
        /* pass */
    } else if (_t1239.tag == HirExpr_EWhileExpr) {
        /* pass */
    } else if (_t1239.tag == HirExpr_ELitInt) {
        /* pass */
    } else if (_t1239.tag == HirExpr_ELitFloat) {
        /* pass */
    } else if (_t1239.tag == HirExpr_ELitStr) {
        /* pass */
    } else if (_t1239.tag == HirExpr_ELitBytes) {
        /* pass */
    } else if (_t1239.tag == HirExpr_ERawStr) {
        /* pass */
    } else if (_t1239.tag == HirExpr_ELitChar) {
        /* pass */
    } else if (_t1239.tag == HirExpr_ELitBool) {
        /* pass */
    } else if (_t1239.tag == HirExpr_ELitNone) {
        __auto_type _ = _t1239.data.ELitNone.ty;
        /* pass */
    } else if (_t1239.tag == HirExpr_ESizeOf) {
        /* pass */
    } else if (_t1239.tag == HirExpr_EIdent) {
        /* pass */
    }
}

__attribute__((hot)) bool Sema_block_ends_in_jump(Sema* self, HirBlock* b) {
#line 3046 "src/sema.tr"
    if ((b->stmts->len == 0LL)) {
#line 3046 "src/sema.tr"
        return false;
    }
#line 3047 "src/sema.tr"
    HirStmt* last = ((HirStmt*)List_ptr_get(b->stmts, (b->stmts->len - 1LL)));
#line 3048 "src/sema.tr"
    if ((((unsigned long long)(last)) == ((unsigned long long)(0LL)))) {
#line 3048 "src/sema.tr"
        return false;
    }
#line 3049 "src/sema.tr"
    __auto_type _t1242 = (*last);
    if (_t1242.tag == HirStmt_SReturn) {
        __auto_type _ = _t1242.data.SReturn.val;
        return true;
    } else if (_t1242.tag == HirStmt_SBreak) {
        __auto_type _ = _t1242.data.SBreak.val;
        return true;
    } else if (_t1242.tag == HirStmt_SContinue) {
        return true;
    } else if (1) {
        __auto_type _ = _t1242;
        return false;
    }
}

__attribute__((hot)) void Sema_append_block_local_drops(Sema* self, HirBlock* hb, long long block_id) {
#line 3061 "src/sema.tr"
    if (Sema_block_ends_in_jump(self, hb)) {
#line 3061 "src/sema.tr"
        return;
    }
#line 3062 "src/sema.tr"
    if ((self->scopes->len == 0LL)) {
#line 3062 "src/sema.tr"
        return;
    }
#line 3063 "src/sema.tr"
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 3064 "src/sema.tr"
    long long ki = 0LL;
#line 3065 "src/sema.tr"
    while ((ki < scope->decl_order->len)) {
#line 3066 "src/sema.tr"
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
#line 3067 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
#line 3068 "src/sema.tr"
        if (((sym->decl_block_id == block_id) && Sema_is_droppable_sym(self, sym))) {
#line 3069 "src/sema.tr"
            HirBlock_push(hb, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
        }
#line 3070 "src/sema.tr"
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
}

__attribute__((hot)) bool Sema__early_exit_drop_ok(Sema* self, TrStr tn) {
#line 3079 "src/sema.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("str", 3LL)))) {
#line 3079 "src/sema.tr"
        return true;
    }
#line 3080 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
#line 3080 "src/sema.tr"
        return false;
    }
#line 3081 "src/sema.tr"
    if ((((((((((((((((_tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL)))) || (_tr_str_eqv((tn), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((tn), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 3081 "src/sema.tr"
        return false;
    }
#line 3082 "src/sema.tr"
    if (Sema_class_has_free(self, tn)) {
#line 3082 "src/sema.tr"
        return false;
    }
#line 3083 "src/sema.tr"
    return true;
}

__attribute__((hot)) void Sema_append_while_exit_drops(Sema* self, HirBlock* hb) {
#line 3092 "src/sema.tr"
    long long top = (self->loop_blk_floor->len - 1LL);
#line 3093 "src/sema.tr"
    if ((top < 0LL)) {
#line 3093 "src/sema.tr"
        return;
    }
#line 3094 "src/sema.tr"
    long long floor = List_i64_get(self->loop_blk_floor, top);
#line 3095 "src/sema.tr"
    long long si = List_i64_get(self->loop_blk_scope, top);
#line 3096 "src/sema.tr"
    if ((((floor < 0LL) || (si < 0LL)) || (si >= self->scopes->len))) {
#line 3096 "src/sema.tr"
        return;
    }
#line 3097 "src/sema.tr"
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, si));
#line 3098 "src/sema.tr"
    long long ki = 0LL;
#line 3099 "src/sema.tr"
    while ((ki < scope->decl_order->len)) {
#line 3100 "src/sema.tr"
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
#line 3101 "src/sema.tr"
        if (_tr_dict_contains(scope->variables, _tr_strz(nm))) {
#line 3102 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
#line 3103 "src/sema.tr"
            if (((((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL))) && (sym->decl_block_id != 0LL))) {
#line 3104 "src/sema.tr"
                long long pos = floor;
#line 3105 "src/sema.tr"
                bool inside = false;
#line 3106 "src/sema.tr"
                while ((pos < self->block_stack->len)) {
#line 3107 "src/sema.tr"
                    if (({ __auto_type _ord_t1243 = (List_i64_get(self->block_stack, pos)); __auto_type _wr = ((_ord_t1243 == sym->decl_block_id)); _wr; })) {
#line 3107 "src/sema.tr"
                        inside = true;
                    }
#line 3108 "src/sema.tr"
                    pos = (pos + 1LL);
                }
#line 3109 "src/sema.tr"
                if (((inside && Sema_is_droppable_sym(self, sym)) && Sema__early_exit_drop_ok(self, (*sym->ty)->name))) {
#line 3110 "src/sema.tr"
                    HirBlock_push(hb, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
                }
            }
        }
#line 3111 "src/sema.tr"
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
}

__attribute__((hot)) void Sema_finalize_scope_drops(Sema* self, HirBlock* hb) {
#line 3116 "src/sema.tr"
    if ((!Sema_block_ends_in_jump(self, hb))) {
#line 3117 "src/sema.tr"
        Sema_append_drops_from(self, hb, (self->scopes->len - 1LL));
    }
}

__attribute__((hot)) bool Sema_block_str_decl(Sema* self, HirBlock* hb, TrStr nm) {
#line 3121 "src/sema.tr"
    long long i = 0LL;
#line 3122 "src/sema.tr"
    while ((i < hb->stmts->len)) {
#line 3123 "src/sema.tr"
        __auto_type _t1244 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t1244.tag == HirStmt_SLet) {
            __auto_type sn = _t1244.data.SLet.name;
__auto_type sty = _t1244.data.SLet.ty;
#line 3125 "src/sema.tr"
            if ((_tr_str_eqv((sn), (nm)) && _tr_str_eqv((sty->name), (_tr_str_lit_len("str", 3LL))))) {
#line 3125 "src/sema.tr"
                return true;
            }
        } else if (1) {
            __auto_type _ = _t1244;
            /* pass */
        }
#line 3127 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3128 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_block_has_autodrop(Sema* self, HirBlock* hb, TrStr nm) {
#line 3132 "src/sema.tr"
    long long i = 0LL;
#line 3133 "src/sema.tr"
    while ((i < hb->stmts->len)) {
#line 3134 "src/sema.tr"
        __auto_type _t1245 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t1245.tag == HirStmt_SAutoDrop) {
            __auto_type dn = _t1245.data.SAutoDrop.name;
#line 3136 "src/sema.tr"
            if (_tr_str_eqv((dn), (nm))) {
#line 3136 "src/sema.tr"
                return true;
            }
        } else if (1) {
            __auto_type _ = _t1245;
            /* pass */
        }
#line 3138 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3139 "src/sema.tr"
    return false;
}

__attribute__((hot)) TrStr Sema_block_coll_decl(Sema* self, HirBlock* hb, TrStr nm) {
#line 3144 "src/sema.tr"
    long long i = 0LL;
#line 3145 "src/sema.tr"
    while ((i < hb->stmts->len)) {
#line 3146 "src/sema.tr"
        __auto_type _t1246 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t1246.tag == HirStmt_SLet) {
            __auto_type sn = _t1246.data.SLet.name;
__auto_type sty = _t1246.data.SLet.ty;
#line 3148 "src/sema.tr"
            if (_tr_str_eqv((sn), (nm))) {
#line 3149 "src/sema.tr"
                TrStr tn = _tr_str_retain(sty->name);
#line 3150 "src/sema.tr"
                if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 3151 "src/sema.tr"
                    TrStr _trret221 = _tr_str_retain(tn);
                    _tr_str_release(tn);
                    return _trret221;
                }
            }
        } else if (1) {
            __auto_type _ = _t1246;
            /* pass */
        }
#line 3153 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3154 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool Sema_coll_droppable_by_sema(Sema* self, TrStr nm) {
#line 3161 "src/sema.tr"
    Symbol* sym = Sema_resolve(self, nm);
#line 3162 "src/sema.tr"
    if (_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) {
#line 3162 "src/sema.tr"
        return false;
    }
#line 3163 "src/sema.tr"
    bool _trret222 = Sema_is_droppable_sym(self, sym);
    return _trret222;
}

__attribute__((hot)) void Sema_apply_mir_if_drops(Sema* self, HirFunction* hf) {
#line 3174 "src/sema.tr"
    List_ptr* plan = mir_if_drop_plan(hf);
#line 3175 "src/sema.tr"
    long long psi = 0LL;
#line 3176 "src/sema.tr"
    while ((psi < plan->len)) {
#line 3177 "src/sema.tr"
        DropSite* site = ((DropSite*)List_ptr_get(plan, psi));
#line 3180 "src/sema.tr"
        if ((!Sema_block_ends_in_jump(self, site->hir_block))) {
#line 3181 "src/sema.tr"
            long long ppi = 0LL;
#line 3182 "src/sema.tr"
            while ((ppi < site->places->len)) {
#line 3183 "src/sema.tr"
                TrStr nm = List_TrStr_get(site->places, ppi);
#line 3184 "src/sema.tr"
                if ((!Sema_block_has_autodrop(self, site->hir_block, nm))) {
#line 3185 "src/sema.tr"
                    if (Sema_block_str_decl(self, site->hir_block, nm)) {
#line 3186 "src/sema.tr"
                        ({ __auto_type _ord_t1247 = (box_hirstmt(HirStmt_ctor_SAutoDrop(nm, _tr_str_lit_len("str", 3LL)))); HirBlock_push(site->hir_block, _ord_t1247); });
                    } else {
#line 3188 "src/sema.tr"
                        TrStr ctn = Sema_block_coll_decl(self, site->hir_block, nm);
#line 3189 "src/sema.tr"
                        if (((!_tr_str_eqv((ctn), (_tr_str_lit_len("", 0LL)))) && Sema_coll_droppable_by_sema(self, nm))) {
#line 3190 "src/sema.tr"
                            ({ __auto_type _ord_t1248 = (box_hirstmt(HirStmt_ctor_SAutoDrop(nm, ctn))); HirBlock_push(site->hir_block, _ord_t1248); });
                        }
                    }
                }
#line 3191 "src/sema.tr"
                ppi = (ppi + 1LL);
                _tr_str_release(nm);
            }
        }
#line 3192 "src/sema.tr"
        psi = (psi + 1LL);
    }
    List_ptr_free_obj(plan, _trdrop_DropSite);
}

__attribute__((hot)) void Sema_declare(Sema* self, TrStr name, SymbolKind kind, AstType** ty, bool is_mut) {
#line 3196 "src/sema.tr"
    if ((((!_tr_str_eqv((name), (_tr_str_lit_len("self", 4LL)))) && (!_tr_str_eqv((name), (_tr_str_lit_len("_", 1LL))))) && (_tr_strlen(_tr_strz(name)) > 1LL))) {
#line 3197 "src/sema.tr"
        TrStr cat = Sema_is_reserved_error(self, name);
#line 3198 "src/sema.tr"
        if ((!_tr_str_eqv((cat), (_tr_str_lit_len("", 0LL))))) {
#line 3199 "src/sema.tr"
            ({ TrStr _at_t1249 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and cannot be used as a name. Choose a different name (e.g. 'my_", 65LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("').", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1249); _tr_str_release(_at_t1249); });
        } else {
#line 3204 "src/sema.tr"
            bool is_toplevel_decl = (_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))) && (kind.tag != SymbolKind_make_SVariable().tag));
#line 3205 "src/sema.tr"
            if (is_toplevel_decl) {
#line 3206 "src/sema.tr"
                TrStr kcat = Sema_is_reserved_keyword(self, name);
#line 3207 "src/sema.tr"
                if ((!_tr_str_eqv((kcat), (_tr_str_lit_len("", 0LL))))) {
#line 3208 "src/sema.tr"
                    ({ TrStr _at_t1250 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kcat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and is reserved. Choose a different name (e.g. 'my_", 52LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("').", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1250); _tr_str_release(_at_t1250); });
                }
            }
        }
    }
#line 3209 "src/sema.tr"
    Symbol* sym = Symbol_init(name, kind, ty);
#line 3210 "src/sema.tr"
    sym->scope_depth = self->current_scope_depth;
#line 3211 "src/sema.tr"
    sym->is_mut = is_mut;
#line 3212 "src/sema.tr"
    sym->decl_block_depth = self->block_depth;
#line 3213 "src/sema.tr"
    long long bsbase = 0LL;
#line 3214 "src/sema.tr"
    if ((self->block_stack_base->len > 0LL)) {
#line 3214 "src/sema.tr"
        bsbase = List_i64_get(self->block_stack_base, (self->block_stack_base->len - 1LL));
    }
#line 3215 "src/sema.tr"
    if ((self->block_stack->len > bsbase)) {
#line 3216 "src/sema.tr"
        sym->decl_block_id = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
    } else {
#line 3218 "src/sema.tr"
        sym->decl_block_id = 0LL;
    }
#line 3220 "src/sema.tr"
    if ((self->scopes->len > 0LL)) {
#line 3221 "src/sema.tr"
        Scope* decl_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 3225 "src/sema.tr"
        if ((((kind.tag == SymbolKind_make_SVariable().tag) && _tr_dict_contains(decl_scope->variables, _tr_strz(name))) && (!_tr_str_starts_withv((name), (_tr_str_lit_len("_", 1LL)))))) {
#line 3226 "src/sema.tr"
            Symbol* prev = ((Symbol*)(uintptr_t)_tr_dict_get(decl_scope->variables, _tr_strz(name)));
#line 3227 "src/sema.tr"
            if ((((prev->kind.tag == SymbolKind_make_SVariable().tag) && (prev->decl_block_id == sym->decl_block_id)) && (!prev->is_param))) {
#line 3228 "src/sema.tr"
                ({ TrStr _at_t1251 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-2] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is already declared in this block. Assign to it without `mut` (`", 66LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ...`), or pick a different name.", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1251); _tr_str_release(_at_t1251); });
            }
        }
#line 3229 "src/sema.tr"
        if ((!_tr_dict_contains(decl_scope->variables, _tr_strz(name)))) {
#line 3229 "src/sema.tr"
            List_TrStr_append(decl_scope->decl_order, name);
        } else if ((((kind.tag == SymbolKind_make_SVariable().tag) && (!_tr_str_starts_withv((name), (_tr_str_lit_len("_", 1LL))))) && (sym->decl_block_id != 0LL))) {
#line 3232 "src/sema.tr"
            Symbol* shadowed = ((Symbol*)(uintptr_t)_tr_dict_get(decl_scope->variables, _tr_strz(name)));
#line 3233 "src/sema.tr"
            if (((shadowed->kind.tag == SymbolKind_make_SVariable().tag) && (shadowed->decl_block_id != sym->decl_block_id))) {
#line 3234 "src/sema.tr"
                List_TrStr_append(self->shadow_names, name);
#line 3235 "src/sema.tr"
                List_ptr_append(self->shadow_prevs, shadowed);
#line 3236 "src/sema.tr"
                List_i64_append(self->shadow_blocks, sym->decl_block_id);
#line 3237 "src/sema.tr"
                List_i64_append(self->shadow_scopes, (self->scopes->len - 1LL));
            }
        }
#line 3238 "src/sema.tr"
        _tr_dict_set(decl_scope->variables, _tr_strz(name), sym);
    } else {
#line 3240 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) Symbol* Sema_resolve(Sema* self, TrStr name) {
#line 3243 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 3244 "src/sema.tr"
    while ((i >= 0LL)) {
#line 3245 "src/sema.tr"
        Scope* scope = ((Scope*)List_ptr_get(self->scopes, i));
#line 3246 "src/sema.tr"
        if (_tr_dict_contains(scope->variables, _tr_strz(name))) {
#line 3247 "src/sema.tr"
            return ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(name)));
        }
#line 3248 "src/sema.tr"
        i = (i - 1LL);
    }
#line 3250 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 3251 "src/sema.tr"
        return ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
    }
#line 3253 "src/sema.tr"
    return ({ __auto_type _ord_t1252 = (SymbolKind_make_SVariable()); __auto_type _ord_t1253 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _wr = (Symbol_init(_tr_str_lit_len("", 0LL), _ord_t1252, _ord_t1253)); _wr; });
}

__attribute__((hot)) bool Sema_is_known_name(Sema* self, TrStr name) {
#line 3268 "src/sema.tr"
    if (Sema__is_type_param_in_scope(self, name)) {
#line 3268 "src/sema.tr"
        return true;
    }
#line 3269 "src/sema.tr"
    if (_tr_dict_contains(self->fn_sigs, _tr_strz(name))) {
#line 3269 "src/sema.tr"
        return true;
    }
#line 3270 "src/sema.tr"
    if (_tr_dict_contains(self->extern_names, _tr_strz(name))) {
#line 3270 "src/sema.tr"
        return true;
    }
#line 3271 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
#line 3271 "src/sema.tr"
        return true;
    }
#line 3272 "src/sema.tr"
    if (_tr_dict_contains(self->enums, _tr_strz(name))) {
#line 3272 "src/sema.tr"
        return true;
    }
#line 3273 "src/sema.tr"
    if (_tr_dict_contains(self->interfaces, _tr_strz(name))) {
#line 3273 "src/sema.tr"
        return true;
    }
#line 3274 "src/sema.tr"
    if (_tr_dict_contains(self->type_aliases, _tr_strz(name))) {
#line 3274 "src/sema.tr"
        return true;
    }
#line 3275 "src/sema.tr"
    if (Sema_is_type_name(self, name)) {
#line 3275 "src/sema.tr"
        return true;
    }
#line 3277 "src/sema.tr"
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("dealloc", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("realloc", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("sizeof", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("repr", 4LL))))) {
#line 3277 "src/sema.tr"
        return true;
    }
#line 3278 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("c_callback", 10LL)))) {
#line 3278 "src/sema.tr"
        return true;
    }
#line 3280 "src/sema.tr"
    if (((_tr_str_lenv((name)) >= 4LL) && ({ TrStr _cmpa_t1254 = (_tr_str_slicev((name), 0LL, 4LL)); TrStr _cmpb_t1255 = (_tr_str_lit_len("_tr_", 4LL)); _Bool _cmpr_t1256 = _tr_str_eqv(_cmpa_t1254, _cmpb_t1255); _tr_str_release(_cmpa_t1254); _cmpr_t1256; }))) {
#line 3280 "src/sema.tr"
        return true;
    }
#line 3281 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_type_name(Sema* self, TrStr nm) {
#line 3285 "src/sema.tr"
    if (((_tr_dict_contains(self->classes, _tr_strz(nm)) || _tr_dict_contains(self->enums, _tr_strz(nm))) || _tr_dict_contains(self->interfaces, _tr_strz(nm)))) {
#line 3285 "src/sema.tr"
        return true;
    }
#line 3286 "src/sema.tr"
    if ((((((_tr_str_eqv((nm), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("void", 4LL))))) {
#line 3286 "src/sema.tr"
        return true;
    }
#line 3287 "src/sema.tr"
    if (((((_tr_str_eqv((nm), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("isize", 5LL))))) {
#line 3287 "src/sema.tr"
        return true;
    }
#line 3288 "src/sema.tr"
    if (((((_tr_str_eqv((nm), (_tr_str_lit_len("u8", 2LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("usize", 5LL))))) {
#line 3288 "src/sema.tr"
        return true;
    }
#line 3289 "src/sema.tr"
    if ((_tr_str_eqv((nm), (_tr_str_lit_len("f32", 3LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("f64", 3LL))))) {
#line 3289 "src/sema.tr"
        return true;
    }
#line 3290 "src/sema.tr"
    return false;
}

__attribute__((hot)) TrStr Sema_type_ref_name(Sema* self, Expr* raw) {
#line 3295 "src/sema.tr"
    if ((((unsigned long long)(raw)) == ((unsigned long long)(0LL)))) {
#line 3295 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 3296 "src/sema.tr"
    __auto_type _t1257 = (*raw);
    if (_t1257.tag == Expr_EIdent) {
        __auto_type nm = _t1257.data.EIdent.name;
#line 3298 "src/sema.tr"
        if ((!Sema_is_type_name(self, nm))) {
#line 3298 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 3303 "src/sema.tr"
        Symbol* s = Sema_resolve(self, nm);
#line 3304 "src/sema.tr"
        if (((!_tr_str_eqv((s->name), (_tr_str_lit_len("", 0LL)))) && (s->kind.tag == SymbolKind_make_SVariable().tag))) {
#line 3304 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 3305 "src/sema.tr"
        TrStr _trret223 = _tr_str_retain(nm);
        return _trret223;
    } else if (1) {
        __auto_type _ = _t1257;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool Sema_is_global_not_local(Sema* self, TrStr name) {
#line 3313 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 3314 "src/sema.tr"
    while ((i >= 0LL)) {
#line 3315 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 3315 "src/sema.tr"
            return false;
        }
#line 3316 "src/sema.tr"
        i = (i - 1LL);
    }
#line 3317 "src/sema.tr"
    return _tr_dict_contains(self->globals, _tr_strz(name));
}

__attribute__((hot)) HirProgram* Sema_analyze(Sema* self, Program* prog) {
#line 3320 "src/sema.tr"
    HirProgram* hp = HirProgram_init();
#line 3329 "src/sema.tr"
    long long ppi = 0LL;
#line 3330 "src/sema.tr"
    while ((ppi < prog->decls->len)) {
#line 3331 "src/sema.tr"
        __auto_type _t1258 = (*((Decl*)List_ptr_get(prog->decls, ppi)));
        if (_t1258.tag == Decl_DClass) {
            __auto_type c = _t1258.data.DClass.cls;
#line 3333 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(c->name))) {
#line 3333 "src/sema.tr"
                _tr_dict_set(self->ambiguous_classes, _tr_strz(c->name), true);
            } else {
#line 3334 "src/sema.tr"
                _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
            }
        } else if (_t1258.tag == Decl_DActor) {
            __auto_type c = _t1258.data.DActor.cls;
#line 3336 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(c->name))) {
#line 3336 "src/sema.tr"
                _tr_dict_set(self->ambiguous_classes, _tr_strz(c->name), true);
            } else {
#line 3337 "src/sema.tr"
                _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
            }
        } else if (1) {
            __auto_type _ = _t1258;
            /* pass */
        }
#line 3339 "src/sema.tr"
        ppi = (ppi + 1LL);
    }
#line 3354 "src/sema.tr"
    long long gpi = 0LL;
#line 3355 "src/sema.tr"
    while ((gpi < prog->decls->len)) {
#line 3356 "src/sema.tr"
        __auto_type _t1259 = (*((Decl*)List_ptr_get(prog->decls, gpi)));
        if (_t1259.tag == Decl_DTopLevelStmt) {
            __auto_type gs = _t1259.data.DTopLevelStmt.stmt;
#line 3358 "src/sema.tr"
            __auto_type _t1260 = (*gs);
            if (_t1260.tag == Stmt_SLet) {
                __auto_type g_name = _t1260.data.SLet.name;
__auto_type g_is_mut = _t1260.data.SLet.is_mut;
__auto_type g_ty_ptr = _t1260.data.SLet.ty;
__auto_type g_val_ptr = _t1260.data.SLet.val;
#line 3360 "src/sema.tr"
                AstType** g_ty = ((AstType**)(0LL));
#line 3361 "src/sema.tr"
                if ((((unsigned long long)(g_ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 3362 "src/sema.tr"
                    g_ty = g_ty_ptr;
                } else if ((((unsigned long long)(g_val_ptr)) != ((unsigned long long)(0LL)))) {
#line 3364 "src/sema.tr"
                    __auto_type _t1261 = (*g_val_ptr);
                    if (_t1261.tag == Expr_ELitInt) {
                        __auto_type _ = _t1261.data.ELitInt.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)));
                    } else if (_t1261.tag == Expr_ELitFloat) {
                        __auto_type _ = _t1261.data.ELitFloat.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("float", 5LL)));
                    } else if (_t1261.tag == Expr_ELitStr) {
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)));
                    } else if (_t1261.tag == Expr_ELitBool) {
                        __auto_type _ = _t1261.data.ELitBool.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)));
                    } else if (1) {
                        __auto_type _ = _t1261;
                        /* pass */
                    }
                }
#line 3370 "src/sema.tr"
                if ((((unsigned long long)(g_ty)) != ((unsigned long long)(0LL)))) {
#line 3371 "src/sema.tr"
                    Sema_declare(self, g_name, SymbolKind_make_SVariable(), g_ty, g_is_mut);
#line 3372 "src/sema.tr"
                    TrStr g_fam = Sema_decl_family(self, (*g_ty));
#line 3373 "src/sema.tr"
                    if ((((((_tr_str_eqv((g_fam), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((g_fam), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((g_fam), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((g_fam), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((g_fam), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((g_fam), (_tr_str_lit_len("Dict", 4LL))))) {
#line 3373 "src/sema.tr"
                        _tr_dict_set(self->global_firm, _tr_strz(g_name), _tr_str_box(_tr_str_retain(g_fam)));
                    }
                }
            } else if (1) {
                __auto_type _ = _t1260;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t1259;
            /* pass */
        }
#line 3376 "src/sema.tr"
        gpi = (gpi + 1LL);
    }
#line 3378 "src/sema.tr"
    long long i = 0LL;
#line 3379 "src/sema.tr"
    while ((i < prog->decls->len)) {
#line 3380 "src/sema.tr"
        Sema_register_decl(self, ((Decl*)List_ptr_get(prog->decls, i)));
#line 3381 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3386 "src/sema.tr"
    Sema_check_type_arities(self, prog);
#line 3393 "src/sema.tr"
    if (self->strict_mode) {
#line 3394 "src/sema.tr"
        Sema_check_ownership_cycles(self, prog);
    }
#line 3399 "src/sema.tr"
    if (self->strict_mode) {
#line 3400 "src/sema.tr"
        i = 0LL;
#line 3401 "src/sema.tr"
        while ((i < prog->decls->len)) {
#line 3402 "src/sema.tr"
            __auto_type _t1262 = (*((Decl*)List_ptr_get(prog->decls, i)));
            if (_t1262.tag == Decl_DClass) {
                __auto_type mc = _t1262.data.DClass.cls;
#line 3404 "src/sema.tr"
                long long mmi = 0LL;
#line 3405 "src/sema.tr"
                while ((mmi < mc->methods->len)) {
#line 3406 "src/sema.tr"
                    FunctionDef* mm = ((FunctionDef*)List_ptr_get(mc->methods, mmi));
#line 3407 "src/sema.tr"
                    if (_block_mutates_self(mm->body)) {
#line 3407 "src/sema.tr"
                        ({ TrStr _dkt_t1263 = (({ TrStr _tr_cl = (_tr_strx_concatv((mc->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mm->name)); _tr_str_release(_tr_cl); _tr_cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t1263), true); _tr_str_release(_dkt_t1263); });
                    }
#line 3408 "src/sema.tr"
                    mmi = (mmi + 1LL);
                }
            } else if (_t1262.tag == Decl_DActor) {
                __auto_type ac = _t1262.data.DActor.cls;
#line 3410 "src/sema.tr"
                long long ami = 0LL;
#line 3411 "src/sema.tr"
                while ((ami < ac->methods->len)) {
#line 3412 "src/sema.tr"
                    FunctionDef* am = ((FunctionDef*)List_ptr_get(ac->methods, ami));
#line 3413 "src/sema.tr"
                    if (_block_mutates_self(am->body)) {
#line 3413 "src/sema.tr"
                        ({ TrStr _dkt_t1264 = (({ TrStr _tr_cl = (_tr_strx_concatv((ac->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (am->name)); _tr_str_release(_tr_cl); _tr_cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t1264), true); _tr_str_release(_dkt_t1264); });
                    }
#line 3414 "src/sema.tr"
                    ami = (ami + 1LL);
                }
            } else if (_t1262.tag == Decl_DExtend) {
                __auto_type mtarget = _t1262.data.DExtend.target;
__auto_type mmethods = _t1262.data.DExtend.methods;
#line 3416 "src/sema.tr"
                long long emi = 0LL;
#line 3417 "src/sema.tr"
                while ((emi < mmethods->len)) {
#line 3418 "src/sema.tr"
                    FunctionDef* em = ((FunctionDef*)List_ptr_get(mmethods, emi));
#line 3419 "src/sema.tr"
                    if (_block_mutates_self(em->body)) {
#line 3419 "src/sema.tr"
                        ({ TrStr _dkt_t1265 = (({ TrStr _tr_cl = (_tr_strx_concatv((mtarget), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (em->name)); _tr_str_release(_tr_cl); _tr_cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t1265), true); _tr_str_release(_dkt_t1265); });
                    }
#line 3420 "src/sema.tr"
                    emi = (emi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1262;
                /* pass */
            }
#line 3422 "src/sema.tr"
            i = (i + 1LL);
        }
    }
#line 3429 "src/sema.tr"
    TrIDict* pre_global_hir = _tr_idict_new(4LL);
#line 3430 "src/sema.tr"
    long long pgi = 0LL;
#line 3431 "src/sema.tr"
    while ((pgi < prog->decls->len)) {
#line 3432 "src/sema.tr"
        __auto_type _t1266 = (*((Decl*)List_ptr_get(prog->decls, pgi)));
        if (_t1266.tag == Decl_DTopLevelStmt) {
            __auto_type pgs = _t1266.data.DTopLevelStmt.stmt;
#line 3434 "src/sema.tr"
            __auto_type _t1267 = (*pgs);
            if (_t1267.tag == Stmt_SLet) {
                __auto_type pg_name = _t1267.data.SLet.name;
__auto_type pg_val = _t1267.data.SLet.val;
#line 3436 "src/sema.tr"
                if (((((unsigned long long)(pg_val)) != ((unsigned long long)(0LL))) && (!_tr_dict_contains(self->globals, _tr_strz(pg_name))))) {
#line 3437 "src/sema.tr"
                    _tr_idict_set(pre_global_hir, pgi, Sema_lower_stmt(self, pgs));
                }
            } else if (1) {
                __auto_type _ = _t1267;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t1266;
            /* pass */
        }
#line 3440 "src/sema.tr"
        pgi = (pgi + 1LL);
    }
#line 3442 "src/sema.tr"
    i = 0LL;
#line 3443 "src/sema.tr"
    while ((i < prog->decls->len)) {
#line 3444 "src/sema.tr"
        Decl* d = ((Decl*)List_ptr_get(prog->decls, i));
#line 3445 "src/sema.tr"
        __auto_type _t1268 = (*d);
        if (_t1268.tag == Decl_DFunction) {
            __auto_type f = _t1268.data.DFunction.func;
#line 3447 "src/sema.tr"
            ({ HirFunction* _ord_t1269 = (Sema_lower_func(self, f)); List_ptr_append(hp->functions, _tr_obj_retain(_ord_t1269)); _tr_obj_release(_ord_t1269, _trdrop_HirFunction); });
        } else if (_t1268.tag == Decl_DClass) {
            __auto_type c = _t1268.data.DClass.cls;
#line 3449 "src/sema.tr"
            ({ HirClass* _ord_t1270 = (Sema_lower_class(self, c)); List_ptr_append(hp->classes, _tr_obj_retain(_ord_t1270)); _tr_obj_release(_ord_t1270, _trdrop_HirClass); });
        } else if (_t1268.tag == Decl_DActor) {
            __auto_type c = _t1268.data.DActor.cls;
#line 3451 "src/sema.tr"
            ({ HirClass* _ord_t1271 = (Sema_lower_class(self, c)); List_ptr_append(hp->classes, _tr_obj_retain(_ord_t1271)); _tr_obj_release(_ord_t1271, _trdrop_HirClass); });
        } else if (_t1268.tag == Decl_DEnum) {
            __auto_type e = _t1268.data.DEnum.enm;
#line 3453 "src/sema.tr"
            ({ HirEnum* _ord_t1272 = (Sema_lower_enum(self, e)); List_ptr_append(hp->enums, _tr_obj_retain(_ord_t1272)); _tr_obj_release(_ord_t1272, _trdrop_HirEnum); });
        } else if (_t1268.tag == Decl_DInterface) {
            __auto_type i_def = _t1268.data.DInterface.iface;
#line 3455 "src/sema.tr"
            ({ HirInterface* _ord_t1273 = (Sema_lower_interface(self, i_def)); List_ptr_append(hp->interfaces, _tr_obj_retain(_ord_t1273)); _tr_obj_release(_ord_t1273, _trdrop_HirInterface); });
        } else if (_t1268.tag == Decl_DExtend) {
            __auto_type target = _t1268.data.DExtend.target;
__auto_type methods = _t1268.data.DExtend.methods;
#line 3457 "src/sema.tr"
            self->current_class_name = _tr_str_retain(target);
#line 3458 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(target))) {
#line 3459 "src/sema.tr"
                self->current_region_params = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(target)))->region_params;
            }
#line 3460 "src/sema.tr"
            long long hi = 0LL;
#line 3461 "src/sema.tr"
            while ((hi < methods->len)) {
#line 3462 "src/sema.tr"
                FunctionDef* f = ((FunctionDef*)List_ptr_get(methods, hi));
#line 3463 "src/sema.tr"
                ({ HirFunction* _ord_t1274 = (Sema_lower_func(self, f)); List_ptr_append(hp->functions, _tr_obj_retain(_ord_t1274)); _tr_obj_release(_ord_t1274, _trdrop_HirFunction); });
#line 3464 "src/sema.tr"
                hi = (hi + 1LL);
            }
#line 3465 "src/sema.tr"
            self->current_class_name = _tr_str_lit_len("", 0LL);
#line 3466 "src/sema.tr"
            self->current_region_params = (void*)List_TrStr_new();
        } else if (_t1268.tag == Decl_DTopLevelStmt) {
            __auto_type s = _t1268.data.DTopLevelStmt.stmt;
#line 3468 "src/sema.tr"
            if (_tr_idict_contains(pre_global_hir, i)) {
#line 3469 "src/sema.tr"
                ({ __auto_type _ord_t1275 = (((HirStmt*)(uintptr_t)_tr_idict_get(pre_global_hir, i))); List_ptr_append(hp->top_level_stmts, _ord_t1275); });
            } else {
#line 3471 "src/sema.tr"
                ({ __auto_type _ord_t1276 = (Sema_lower_stmt(self, s)); List_ptr_append(hp->top_level_stmts, _ord_t1276); });
            }
        } else if (_t1268.tag == Decl_DExtern) {
            __auto_type functions = _t1268.data.DExtern.functions;
#line 3473 "src/sema.tr"
            long long ei = 0LL;
#line 3474 "src/sema.tr"
            while ((ei < functions->len)) {
#line 3475 "src/sema.tr"
                FunctionDef* ef = ((FunctionDef*)List_ptr_get(functions, ei));
#line 3476 "src/sema.tr"
                HirFunction* hef = Sema_lower_func(self, ef);
#line 3477 "src/sema.tr"
                hef->is_extern = true;
#line 3478 "src/sema.tr"
                List_ptr_append(hp->extern_funcs, _tr_obj_retain(hef));
#line 3479 "src/sema.tr"
                ei = (ei + 1LL);
                _tr_obj_release(hef, _trdrop_HirFunction);
            }
        } else if (_t1268.tag == Decl_DDecoratorDef) {
            __auto_type f = _t1268.data.DDecoratorDef.func;
#line 3481 "src/sema.tr"
            HirFunction* hdf = Sema_lower_func(self, f);
#line 3482 "src/sema.tr"
            hdf->is_decorator = true;
#line 3483 "src/sema.tr"
            List_ptr_append(hp->decorator_defs, _tr_obj_retain(hdf));
            _tr_obj_release(hdf, _trdrop_HirFunction);
        } else if (_t1268.tag == Decl_DTypeAlias) {
            __auto_type alias_name = _t1268.data.DTypeAlias.name;
__auto_type target_ty = _t1268.data.DTypeAlias.target;
#line 3486 "src/sema.tr"
            List_TrStr_append(hp->type_alias_names, alias_name);
#line 3487 "src/sema.tr"
            AstType** ta_ty_ptr = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 3488 "src/sema.tr"
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
#line 3488 "src/sema.tr"
                ta_ty_ptr = target_ty;
            }
#line 3489 "src/sema.tr"
            List_ptr_append(hp->type_alias_types, ta_ty_ptr);
        } else if (1) {
            __auto_type _ = _t1268;
            /* pass */
        }
#line 3491 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3495 "src/sema.tr"
    long long nci = 0LL;
#line 3496 "src/sema.tr"
    while ((nci < self->nested_classes->len)) {
#line 3497 "src/sema.tr"
        ({ __auto_type _ord_t1277 = (((HirClass*)List_ptr_get(self->nested_classes, nci))); List_ptr_append(hp->classes, _tr_obj_retain(_ord_t1277)); });
#line 3498 "src/sema.tr"
        nci = (nci + 1LL);
    }
#line 3499 "src/sema.tr"
    long long nfi = 0LL;
#line 3500 "src/sema.tr"
    while ((nfi < self->nested_functions->len)) {
#line 3501 "src/sema.tr"
        ({ __auto_type _ord_t1278 = (((HirFunction*)List_ptr_get(self->nested_functions, nfi))); List_ptr_append(hp->functions, _tr_obj_retain(_ord_t1278)); });
#line 3502 "src/sema.tr"
        nfi = (nfi + 1LL);
    }
#line 3503 "src/sema.tr"
    long long nei = 0LL;
#line 3504 "src/sema.tr"
    while ((nei < self->nested_enums->len)) {
#line 3505 "src/sema.tr"
        ({ __auto_type _ord_t1279 = (((HirEnum*)List_ptr_get(self->nested_enums, nei))); List_ptr_append(hp->enums, _tr_obj_retain(_ord_t1279)); });
#line 3506 "src/sema.tr"
        nei = (nei + 1LL);
    }
#line 3507 "src/sema.tr"
    long long nii = 0LL;
#line 3508 "src/sema.tr"
    while ((nii < self->nested_interfaces->len)) {
#line 3509 "src/sema.tr"
        ({ __auto_type _ord_t1280 = (((HirInterface*)List_ptr_get(self->nested_interfaces, nii))); List_ptr_append(hp->interfaces, _tr_obj_retain(_ord_t1280)); });
#line 3510 "src/sema.tr"
        nii = (nii + 1LL);
    }
#line 3517 "src/sema.tr"
    Sema_compute_return_ownership(self, hp);
#line 3520 "src/sema.tr"
    Sema_compute_param_ownership(self, hp);
#line 3523 "src/sema.tr"
    Sema_apply_borrow_drops(self, hp);
#line 3524 "src/sema.tr"
    Sema_check_free_fn_method_c_name_collisions(self, hp);
#line 3525 "src/sema.tr"
    return hp;
}

__attribute__((hot)) void Sema_check_free_fn_method_c_name_collisions(Sema* self, HirProgram* hp) {
#line 3546 "src/sema.tr"
    TrMap* method_c_names = _tr_dict_new(32LL);
#line 3547 "src/sema.tr"
    long long mi = 0LL;
#line 3548 "src/sema.tr"
    while ((mi < hp->functions->len)) {
#line 3549 "src/sema.tr"
        HirFunction* mf = ((HirFunction*)List_ptr_get(hp->functions, mi));
#line 3550 "src/sema.tr"
        if (((!_tr_str_eqv((mf->class_name), (_tr_str_lit_len("", 0LL)))) && (!mf->is_extern))) {
#line 3551 "src/sema.tr"
            TrStr mc_name = ({ TrStr _tr_cl = (_tr_strx_concatv((mf->class_name), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mf->name)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3552 "src/sema.tr"
            if ((!_tr_dict_contains(method_c_names, _tr_strz(mc_name)))) {
#line 3553 "src/sema.tr"
                ({ TrStr _dvt_t1281 = (({ TrStr _tr_cl = (_tr_strx_concatv((mf->class_name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mf->name)); _tr_str_release(_tr_cl); _tr_cres; })); _tr_dict_set(method_c_names, _tr_strz(mc_name), _tr_str_box(_tr_str_retain(_dvt_t1281))); _tr_str_release(_dvt_t1281); });
            }
        }
#line 3554 "src/sema.tr"
        mi = (mi + 1LL);
    }
#line 3555 "src/sema.tr"
    long long fi = 0LL;
#line 3556 "src/sema.tr"
    while ((fi < hp->functions->len)) {
#line 3557 "src/sema.tr"
        HirFunction* ff = ((HirFunction*)List_ptr_get(hp->functions, fi));
#line 3558 "src/sema.tr"
        if ((((_tr_str_eqv((ff->class_name), (_tr_str_lit_len("", 0LL))) && (!ff->is_extern)) && (!_tr_str_eqv((ff->name), (_tr_str_lit_len("main", 4LL))))) && (ff->generics->len == 0LL))) {
#line 3559 "src/sema.tr"
            TrStr fc_name = _tr_str_retain(ff->name);
#line 3560 "src/sema.tr"
            if (_tr_dict_contains(method_c_names, _tr_strz(fc_name))) {
#line 3561 "src/sema.tr"
                ({ __auto_type _ord_t1282 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[C-1] free function '", 21LL)), (ff->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' generates the same C symbol name ('", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fc_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("') as class/enum method '", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1283 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1282); TrStr _tr_cr = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(method_c_names, _tr_strz(fc_name))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      This compiles cleanly on its own but fails at LINK time ('multiple definition of \"", 91LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fc_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"') the moment a build pulls in both definitions.\n      FIX: rename one of the two (the free function '", 103LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1284 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1283); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ff->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or the method '", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1285 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1284); TrStr _tr_cr = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(method_c_names, _tr_strz(fc_name))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("').", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1285); _tr_str_release(_at_t1285); });
            }
        }
#line 3562 "src/sema.tr"
        fi = (fi + 1LL);
    }
}

__attribute__((hot)) void Sema_compute_return_ownership(Sema* self, HirProgram* hp) {
#line 3576 "src/sema.tr"
    List_TrStr* keys = (void*)List_TrStr_new();
#line 3577 "src/sema.tr"
    List_ptr* fns = (void*)List_ptr_new();
#line 3578 "src/sema.tr"
    long long fi = 0LL;
#line 3579 "src/sema.tr"
    while ((fi < hp->functions->len)) {
#line 3580 "src/sema.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(hp->functions, fi));
#line 3581 "src/sema.tr"
        if (Sema__fn_ret_is_heap_class(self, f)) {
#line 3582 "src/sema.tr"
            List_TrStr_append(keys, f->name);
#line 3583 "src/sema.tr"
            List_ptr_append(fns, _tr_obj_retain(f));
        }
#line 3584 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 3585 "src/sema.tr"
    long long ci = 0LL;
#line 3586 "src/sema.tr"
    while ((ci < hp->classes->len)) {
#line 3587 "src/sema.tr"
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
#line 3588 "src/sema.tr"
        long long mi = 0LL;
#line 3589 "src/sema.tr"
        while ((mi < c->methods->len)) {
#line 3590 "src/sema.tr"
            HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
#line 3591 "src/sema.tr"
            if (Sema__fn_ret_is_heap_class(self, m)) {
#line 3592 "src/sema.tr"
                ({ TrStr _at_t1286 = (({ TrStr _tr_cl = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(keys, _at_t1286); _tr_str_release(_at_t1286); });
#line 3593 "src/sema.tr"
                List_ptr_append(fns, _tr_obj_retain(m));
            }
#line 3594 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 3595 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 3596 "src/sema.tr"
    long long ei = 0LL;
#line 3597 "src/sema.tr"
    while ((ei < hp->enums->len)) {
#line 3598 "src/sema.tr"
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, ei));
#line 3599 "src/sema.tr"
        long long emi = 0LL;
#line 3600 "src/sema.tr"
        while ((emi < en->methods->len)) {
#line 3601 "src/sema.tr"
            HirFunction* em = ((HirFunction*)List_ptr_get(en->methods, emi));
#line 3602 "src/sema.tr"
            if (Sema__fn_ret_is_heap_class(self, em)) {
#line 3603 "src/sema.tr"
                ({ TrStr _at_t1287 = (({ TrStr _tr_cl = (_tr_strx_concatv((en->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (em->name)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(keys, _at_t1287); _tr_str_release(_at_t1287); });
#line 3604 "src/sema.tr"
                List_ptr_append(fns, _tr_obj_retain(em));
            }
#line 3605 "src/sema.tr"
            emi = (emi + 1LL);
        }
#line 3606 "src/sema.tr"
        ei = (ei + 1LL);
    }
#line 3612 "src/sema.tr"
    long long ki = 0LL;
#line 3613 "src/sema.tr"
    while ((ki < keys->len)) {
#line 3614 "src/sema.tr"
        HirFunction* kf = ((HirFunction*)List_ptr_get(fns, ki));
#line 3615 "src/sema.tr"
        bool kf_owned = true;
#line 3616 "src/sema.tr"
        if ((((unsigned long long)(kf->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 3617 "src/sema.tr"
            if (((kf->ret_ty->is_borrow || (!_tr_str_eqv((kf->ret_ty->from_param), (_tr_str_lit_len("", 0LL))))) || (kf->ret_ty->from_regions->len > 0LL))) {
#line 3618 "src/sema.tr"
                kf_owned = false;
            }
        }
#line 3619 "src/sema.tr"
        ({ TrStr _ord_t1288 = (List_TrStr_get(keys, ki)); _tr_dict_set(self->fn_ret_owned, _tr_strz(_ord_t1288), kf_owned); _tr_str_release(_ord_t1288); });
#line 3620 "src/sema.tr"
        ki = (ki + 1LL);
    }
#line 3622 "src/sema.tr"
    bool changed = true;
#line 3623 "src/sema.tr"
    while (changed) {
#line 3624 "src/sema.tr"
        changed = false;
#line 3625 "src/sema.tr"
        long long xi = 0LL;
#line 3626 "src/sema.tr"
        while ((xi < fns->len)) {
#line 3627 "src/sema.tr"
            TrStr k = List_TrStr_get(keys, xi);
#line 3628 "src/sema.tr"
            if (((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(k)))) {
#line 3629 "src/sema.tr"
                List_ptr* rets = (void*)List_ptr_new();
#line 3630 "src/sema.tr"
                Sema__collect_returns(self, ((HirFunction*)List_ptr_get(fns, xi))->body, rets);
#line 3631 "src/sema.tr"
                bool all_owned = true;
#line 3632 "src/sema.tr"
                long long ri = 0LL;
#line 3633 "src/sema.tr"
                while ((ri < rets->len)) {
#line 3634 "src/sema.tr"
                    if ((!Sema__ret_yields_owned(self, ((HirExpr*)List_ptr_get(rets, ri))))) {
#line 3635 "src/sema.tr"
                        all_owned = false;
                    }
#line 3636 "src/sema.tr"
                    ri = (ri + 1LL);
                }
#line 3637 "src/sema.tr"
                if ((!all_owned)) {
#line 3638 "src/sema.tr"
                    _tr_dict_set(self->fn_ret_owned, _tr_strz(k), false);
#line 3639 "src/sema.tr"
                    changed = true;
                }
            }
#line 3640 "src/sema.tr"
            xi = (xi + 1LL);
            _tr_str_release(k);
        }
    }
#line 3642 "src/sema.tr"
    long long wi = 0LL;
#line 3643 "src/sema.tr"
    while ((wi < fns->len)) {
#line 3644 "src/sema.tr"
        HirFunction* hf2 = ((HirFunction*)List_ptr_get(fns, wi));
#line 3645 "src/sema.tr"
        hf2->returns_owned = ({ TrStr _ord_t1289 = (List_TrStr_get(keys, wi)); __auto_type _wr = (((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(_ord_t1289)))); _tr_str_release(_ord_t1289); _wr; });
#line 3646 "src/sema.tr"
        wi = (wi + 1LL);
    }
    List_TrStr_free(keys);
    List_ptr_free_obj(fns, _trdrop_HirFunction);
}

__attribute__((hot)) void Sema_compute_param_ownership(Sema* self, HirProgram* hp) {
#line 3656 "src/sema.tr"
    List_TrStr* keys = (void*)List_TrStr_new();
#line 3657 "src/sema.tr"
    List_ptr* fns = (void*)List_ptr_new();
#line 3658 "src/sema.tr"
    long long fi = 0LL;
#line 3659 "src/sema.tr"
    while ((fi < hp->functions->len)) {
#line 3660 "src/sema.tr"
        List_TrStr_append(keys, ((HirFunction*)List_ptr_get(hp->functions, fi))->name);
#line 3661 "src/sema.tr"
        List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(hp->functions, fi))));
#line 3662 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 3663 "src/sema.tr"
    long long ci = 0LL;
#line 3664 "src/sema.tr"
    while ((ci < hp->classes->len)) {
#line 3665 "src/sema.tr"
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
#line 3666 "src/sema.tr"
        long long mi = 0LL;
#line 3667 "src/sema.tr"
        while ((mi < c->methods->len)) {
#line 3668 "src/sema.tr"
            ({ __auto_type _ord_t1290 = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _at_t1291 = (({ TrStr _tr_cl = (_ord_t1290); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((HirFunction*)List_ptr_get(c->methods, mi))->name)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(keys, _at_t1291); _tr_str_release(_at_t1291); });
#line 3669 "src/sema.tr"
            List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(c->methods, mi))));
#line 3670 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 3671 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 3672 "src/sema.tr"
    long long eni = 0LL;
#line 3673 "src/sema.tr"
    while ((eni < hp->enums->len)) {
#line 3674 "src/sema.tr"
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, eni));
#line 3675 "src/sema.tr"
        long long emi = 0LL;
#line 3676 "src/sema.tr"
        while ((emi < en->methods->len)) {
#line 3677 "src/sema.tr"
            ({ __auto_type _ord_t1292 = (_tr_strx_concatv((en->name), (_tr_str_lit_len(".", 1LL)))); TrStr _at_t1293 = (({ TrStr _tr_cl = (_ord_t1292); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((HirFunction*)List_ptr_get(en->methods, emi))->name)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(keys, _at_t1293); _tr_str_release(_at_t1293); });
#line 3678 "src/sema.tr"
            List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(en->methods, emi))));
#line 3679 "src/sema.tr"
            emi = (emi + 1LL);
        }
#line 3680 "src/sema.tr"
        eni = (eni + 1LL);
    }
#line 3682 "src/sema.tr"
    long long ii = 0LL;
#line 3683 "src/sema.tr"
    while ((ii < fns->len)) {
#line 3684 "src/sema.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(fns, ii));
#line 3685 "src/sema.tr"
        long long pi = 0LL;
#line 3686 "src/sema.tr"
        while ((pi < f->params->len)) {
#line 3687 "src/sema.tr"
            if ((!_tr_str_eqv((((HirParam*)List_ptr_get(f->params, pi))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 3688 "src/sema.tr"
                ({ __auto_type _ord_t1294 = (({ TrStr _tr_cl = (List_TrStr_get(keys, ii)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("#", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1295 = (({ TrStr _tr_cl = (_ord_t1294); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _tr_dict_set(self->fn_param_consumes, _tr_strz(_ord_t1295), false); _tr_str_release(_ord_t1295); });
            }
#line 3689 "src/sema.tr"
            pi = (pi + 1LL);
        }
#line 3690 "src/sema.tr"
        ii = (ii + 1LL);
    }
#line 3692 "src/sema.tr"
    bool changed = true;
#line 3693 "src/sema.tr"
    while (changed) {
#line 3694 "src/sema.tr"
        changed = false;
#line 3695 "src/sema.tr"
        long long xi = 0LL;
#line 3696 "src/sema.tr"
        while ((xi < fns->len)) {
#line 3697 "src/sema.tr"
            HirFunction* f = ((HirFunction*)List_ptr_get(fns, xi));
#line 3698 "src/sema.tr"
            long long pi = 0LL;
#line 3699 "src/sema.tr"
            while ((pi < f->params->len)) {
#line 3700 "src/sema.tr"
                TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pi))->name);
#line 3701 "src/sema.tr"
                TrStr pk = ({ __auto_type _ord_t1296 = (({ TrStr _tr_cl = (List_TrStr_get(keys, xi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("#", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t1296); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
#line 3702 "src/sema.tr"
                if (((!_tr_str_eqv((pn), (_tr_str_lit_len("self", 4LL)))) && _tr_dict_contains(self->fn_param_consumes, _tr_strz(pk)))) {
#line 3703 "src/sema.tr"
                    if ((!((bool)(uintptr_t)_tr_dict_get(self->fn_param_consumes, _tr_strz(pk))))) {
#line 3704 "src/sema.tr"
                        if (Sema__param_consumed_in_block(self, pn, f->body)) {
#line 3705 "src/sema.tr"
                            _tr_dict_set(self->fn_param_consumes, _tr_strz(pk), true);
#line 3706 "src/sema.tr"
                            changed = true;
                        }
                    }
                }
#line 3707 "src/sema.tr"
                pi = (pi + 1LL);
                _tr_str_release(pn);
                _tr_str_release(pk);
            }
#line 3708 "src/sema.tr"
            xi = (xi + 1LL);
        }
    }
    List_TrStr_free(keys);
    List_ptr_free_obj(fns, _trdrop_HirFunction);
}

__attribute__((hot)) bool Sema__is_pure_scalar(Sema* self, TrStr tn) {
#line 3714 "src/sema.tr"
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("void", 4LL))))) {
#line 3714 "src/sema.tr"
        return true;
    }
#line 3715 "src/sema.tr"
    if ((((_tr_str_eqv((tn), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i64", 3LL))))) {
#line 3715 "src/sema.tr"
        return true;
    }
#line 3716 "src/sema.tr"
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("u8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("usize", 5LL))))) {
#line 3716 "src/sema.tr"
        return true;
    }
#line 3717 "src/sema.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("f32", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("f64", 3LL))))) {
#line 3717 "src/sema.tr"
        return true;
    }
#line 3718 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__pc_owns_return(Sema* self, HirExpr* e) {
#line 3723 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 3723 "src/sema.tr"
        return true;
    }
#line 3724 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 3724 "src/sema.tr"
        return true;
    }
#line 3725 "src/sema.tr"
    return (!Sema__is_pure_scalar(self, hir_expr_type(e)->name));
}

__attribute__((hot)) void Sema_apply_borrow_drops(Sema* self, HirProgram* hp) {
#line 3742 "src/sema.tr"
    long long fi = 0LL;
#line 3743 "src/sema.tr"
    while ((fi < hp->functions->len)) {
#line 3744 "src/sema.tr"
        Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(hp->functions, fi))->body);
#line 3745 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 3746 "src/sema.tr"
    long long ci = 0LL;
#line 3747 "src/sema.tr"
    while ((ci < hp->classes->len)) {
#line 3748 "src/sema.tr"
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
#line 3749 "src/sema.tr"
        long long mi = 0LL;
#line 3750 "src/sema.tr"
        while ((mi < c->methods->len)) {
#line 3751 "src/sema.tr"
            Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(c->methods, mi))->body);
#line 3752 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 3753 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 3754 "src/sema.tr"
    long long eni = 0LL;
#line 3755 "src/sema.tr"
    while ((eni < hp->enums->len)) {
#line 3756 "src/sema.tr"
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, eni));
#line 3757 "src/sema.tr"
        long long emi = 0LL;
#line 3758 "src/sema.tr"
        while ((emi < en->methods->len)) {
#line 3759 "src/sema.tr"
            Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(en->methods, emi))->body);
#line 3760 "src/sema.tr"
            emi = (emi + 1LL);
        }
#line 3761 "src/sema.tr"
        eni = (eni + 1LL);
    }
}

__attribute__((hot)) void Sema__borrow_drops_block(Sema* self, HirBlock* b) {
#line 3764 "src/sema.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 3764 "src/sema.tr"
        return;
    }
#line 3765 "src/sema.tr"
    long long i = 0LL;
#line 3766 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 3767 "src/sema.tr"
        Sema__borrow_drops_recurse_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)));
#line 3768 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3769 "src/sema.tr"
    List_TrStr* add_names = (void*)List_TrStr_new();
#line 3770 "src/sema.tr"
    List_TrStr* add_cls = (void*)List_TrStr_new();
#line 3771 "src/sema.tr"
    i = 0LL;
#line 3772 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 3773 "src/sema.tr"
        __auto_type _t1297 = (*((HirStmt*)List_ptr_get(b->stmts, i)));
        if (_t1297.tag == HirStmt_SLet) {
            __auto_type nm = _t1297.data.SLet.name;
__auto_type _own = _t1297.data.SLet.ownership;
__auto_type _mut2 = _t1297.data.SLet.is_mut;
__auto_type _cst = _t1297.data.SLet.is_const;
__auto_type _shr = _t1297.data.SLet.is_shared;
__auto_type _ty = _t1297.data.SLet.ty;
__auto_type val = _t1297.data.SLet.val;
#line 3775 "src/sema.tr"
            TrStr cn = _tr_str_retain(_ty->name);
#line 3776 "src/sema.tr"
            if ((Sema__is_owned_droppable_class(self, cn) && Sema__is_fresh_owned_producer(self, val, cn))) {
#line 3777 "src/sema.tr"
                if ((!Sema__block_has_drop_or_reassign(self, b, nm))) {
#line 3781 "src/sema.tr"
                    self->pc_alias_consumes = Sema_class_method_exists(self, cn, _tr_str_lit_len("free", 4LL));
#line 3782 "src/sema.tr"
                    bool _bd_consumed = Sema__param_consumed_in_block(self, nm, b);
#line 3783 "src/sema.tr"
                    self->pc_alias_consumes = false;
#line 3784 "src/sema.tr"
                    if ((!_bd_consumed)) {
#line 3785 "src/sema.tr"
                        List_TrStr_append(add_names, nm);
#line 3786 "src/sema.tr"
                        List_TrStr_append(add_cls, cn);
                    }
                }
            }
            _tr_str_release(cn);
        } else if (1) {
            __auto_type _ = _t1297;
            /* pass */
        }
#line 3788 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3789 "src/sema.tr"
    long long ai = 0LL;
#line 3790 "src/sema.tr"
    while ((ai < add_names->len)) {
#line 3791 "src/sema.tr"
        ({ TrStr _ord_t1298 = (List_TrStr_get(add_names, ai)); TrStr _ord_t1299 = (List_TrStr_get(add_cls, ai)); Sema__insert_drop_before_terminator(self, b, _ord_t1298, _ord_t1299); _tr_str_release(_ord_t1298); _tr_str_release(_ord_t1299); });
#line 3792 "src/sema.tr"
        ai = (ai + 1LL);
    }
    List_TrStr_free(add_names);
    List_TrStr_free(add_cls);
}

__attribute__((hot)) void Sema__borrow_drops_recurse_stmt(Sema* self, HirStmt* sp) {
#line 3795 "src/sema.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 3795 "src/sema.tr"
        return;
    }
#line 3796 "src/sema.tr"
    __auto_type _t1300 = (*sp);
    if (_t1300.tag == HirStmt_SIf) {
        __auto_type tb = _t1300.data.SIf.then_b;
__auto_type eb = _t1300.data.SIf.else_b;
#line 3798 "src/sema.tr"
        Sema__borrow_drops_block(self, tb);
#line 3799 "src/sema.tr"
        Sema__borrow_drops_block(self, eb);
    } else if (_t1300.tag == HirStmt_SWhile) {
        __auto_type wb = _t1300.data.SWhile.body;
        Sema__borrow_drops_block(self, wb);
    } else if (_t1300.tag == HirStmt_SFor) {
        __auto_type fb = _t1300.data.SFor.body;
        Sema__borrow_drops_block(self, fb);
    } else if (_t1300.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t1300.data.SForUnpack.body;
        Sema__borrow_drops_block(self, fub);
    } else if (_t1300.tag == HirStmt_SMatch) {
        __auto_type arms = _t1300.data.SMatch.arms;
#line 3804 "src/sema.tr"
        long long ai = 0LL;
#line 3805 "src/sema.tr"
        while ((ai < arms->len)) {
#line 3806 "src/sema.tr"
            Sema__borrow_drops_block(self, ((HirMatchArm*)List_ptr_get(arms, ai))->body);
#line 3807 "src/sema.tr"
            ai = (ai + 1LL);
        }
    } else if (_t1300.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t1300.data.SUnsafe.body;
        Sema__borrow_drops_block(self, ub);
    } else if (_t1300.tag == HirStmt_SWith) {
        __auto_type wb = _t1300.data.SWith.body;
        Sema__borrow_drops_block(self, wb);
    } else if (_t1300.tag == HirStmt_STaskGroup) {
        __auto_type tb = _t1300.data.STaskGroup.body;
        Sema__borrow_drops_block(self, tb);
    } else if (_t1300.tag == HirStmt_STry) {
        __auto_type tb = _t1300.data.STry.try_body;
__auto_type catches = _t1300.data.STry.catches;
__auto_type fb = _t1300.data.STry.finally_b;
#line 3812 "src/sema.tr"
        Sema__borrow_drops_block(self, tb);
#line 3813 "src/sema.tr"
        Sema__borrow_drops_block(self, fb);
#line 3814 "src/sema.tr"
        long long ci = 0LL;
#line 3815 "src/sema.tr"
        while ((ci < catches->len)) {
#line 3816 "src/sema.tr"
            Sema__borrow_drops_block(self, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body);
#line 3817 "src/sema.tr"
            ci = (ci + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1300;
        /* pass */
    }
}

__attribute__((hot)) bool Sema__is_fresh_owned_producer(Sema* self, HirExpr* val, TrStr cn) {
#line 3824 "src/sema.tr"
    if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
#line 3824 "src/sema.tr"
        return false;
    }
#line 3825 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(val)))) {
#line 3825 "src/sema.tr"
        return false;
    }
#line 3826 "src/sema.tr"
    __auto_type _t1301 = (*val);
    if (_t1301.tag == HirExpr_ECall) {
        __auto_type callee = _t1301.data.ECall.callee;
#line 3828 "src/sema.tr"
        __auto_type _t1302 = (*callee);
        if (_t1302.tag == HirExpr_EIdent) {
            __auto_type fnm = _t1302.data.EIdent.name;
#line 3830 "src/sema.tr"
            if (_tr_str_eqv((fnm), (cn))) {
#line 3830 "src/sema.tr"
                return true;
            }
#line 3831 "src/sema.tr"
            if ((_tr_dict_contains(self->fn_ret_owned, _tr_strz(fnm)) && ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(fnm))))) {
#line 3831 "src/sema.tr"
                return true;
            }
        } else if (1) {
            __auto_type _ = _t1302;
            /* pass */
        }
    } else if (_t1301.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1301.data.EMethodCall.obj;
__auto_type m = _t1301.data.EMethodCall.method;
#line 3834 "src/sema.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("new", 3LL))))) {
#line 3834 "src/sema.tr"
            return true;
        }
#line 3835 "src/sema.tr"
        TrStr mkey = ({ TrStr _tr_cl = (_tr_strx_concatv((hir_expr_type(obj)->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3836 "src/sema.tr"
        if ((_tr_dict_contains(self->fn_ret_owned, _tr_strz(mkey)) && ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(mkey))))) {
#line 3836 "src/sema.tr"
            _tr_str_release(mkey);
            return true;
        }
        _tr_str_release(mkey);
    } else if (1) {
        __auto_type _ = _t1301;
        /* pass */
    }
#line 3838 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__is_owned_droppable_class(Sema* self, TrStr cn) {
#line 3841 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cn)))) {
#line 3841 "src/sema.tr"
        return false;
    }
#line 3842 "src/sema.tr"
    if ((((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cn)))->generics->len > 0LL)) {
#line 3842 "src/sema.tr"
        return false;
    }
#line 3843 "src/sema.tr"
    if (Sema_class_method_exists(self, cn, _tr_str_lit_len("free", 4LL))) {
#line 3843 "src/sema.tr"
        return true;
    }
#line 3844 "src/sema.tr"
    return Sema__is_rc_class(self, cn);
}

__attribute__((hot)) bool Sema__block_has_drop_or_reassign(Sema* self, HirBlock* b, TrStr nm) {
#line 3847 "src/sema.tr"
    long long i = 0LL;
#line 3848 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 3849 "src/sema.tr"
        __auto_type _t1303 = (*((HirStmt*)List_ptr_get(b->stmts, i)));
        if (_t1303.tag == HirStmt_SAutoDrop) {
            __auto_type dn = _t1303.data.SAutoDrop.name;
#line 3851 "src/sema.tr"
            if (_tr_str_eqv((dn), (nm))) {
#line 3851 "src/sema.tr"
                return true;
            }
        } else if (_t1303.tag == HirStmt_SFree) {
            __auto_type dn = _t1303.data.SFree.name;
#line 3853 "src/sema.tr"
            if (_tr_str_eqv((dn), (nm))) {
#line 3853 "src/sema.tr"
                return true;
            }
        } else if (_t1303.tag == HirStmt_SAssign) {
            __auto_type tgt = _t1303.data.SAssign.target;
#line 3855 "src/sema.tr"
            __auto_type _t1304 = (*tgt);
            if (_t1304.tag == HirExpr_EIdent) {
                __auto_type tn2 = _t1304.data.EIdent.name;
#line 3857 "src/sema.tr"
                if (_tr_str_eqv((tn2), (nm))) {
#line 3857 "src/sema.tr"
                    return true;
                }
            } else if (1) {
                __auto_type _ = _t1304;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t1303;
            /* pass */
        }
#line 3860 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3861 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema__insert_drop_before_terminator(Sema* self, HirBlock* b, TrStr nm, TrStr cn) {
#line 3864 "src/sema.tr"
    HirStmt* drop = box_hirstmt(HirStmt_ctor_SAutoDrop(nm, cn));
#line 3865 "src/sema.tr"
    long long n = b->stmts->len;
#line 3866 "src/sema.tr"
    if ((n == 0LL)) {
#line 3867 "src/sema.tr"
        List_ptr_append(b->stmts, drop);
#line 3868 "src/sema.tr"
        return;
    }
#line 3869 "src/sema.tr"
    HirStmt* last = ((HirStmt*)List_ptr_get(b->stmts, (n - 1LL)));
#line 3870 "src/sema.tr"
    bool is_term = false;
#line 3871 "src/sema.tr"
    HirExpr* term_val = (HirExpr*)(0LL);
#line 3872 "src/sema.tr"
    __auto_type _t1305 = (*last);
    if (_t1305.tag == HirStmt_SReturn) {
        __auto_type rv = _t1305.data.SReturn.val;
#line 3874 "src/sema.tr"
        is_term = true;
#line 3875 "src/sema.tr"
        term_val = rv;
    } else if (_t1305.tag == HirStmt_SRaise) {
        __auto_type rv2 = _t1305.data.SRaise.val;
#line 3877 "src/sema.tr"
        is_term = true;
#line 3878 "src/sema.tr"
        term_val = rv2;
    } else if (_t1305.tag == HirStmt_SBreak) {
        __auto_type bv3 = _t1305.data.SBreak.val;
#line 3880 "src/sema.tr"
        is_term = true;
#line 3881 "src/sema.tr"
        term_val = bv3;
    } else if (_t1305.tag == HirStmt_SContinue) {
        is_term = true;
    } else if (1) {
        __auto_type _ = _t1305;
        /* pass */
    }
#line 3884 "src/sema.tr"
    if (is_term) {
#line 3897 "src/sema.tr"
        List_TrStr* term_refs = (void*)List_TrStr_new();
#line 3898 "src/sema.tr"
        if ((((unsigned long long)(term_val)) != ((unsigned long long)(0LL)))) {
#line 3898 "src/sema.tr"
            Sema_collect_idents(self, term_val, term_refs);
        }
#line 3899 "src/sema.tr"
        bool nm_referenced = false;
#line 3900 "src/sema.tr"
        long long tri = 0LL;
#line 3901 "src/sema.tr"
        while ((tri < term_refs->len)) {
#line 3902 "src/sema.tr"
            if (({ TrStr _cmpa_t1306 = (List_TrStr_get(term_refs, tri)); TrStr _cmpb_t1307 = (nm); _Bool _cmpr_t1308 = _tr_str_eqv(_cmpa_t1306, _cmpb_t1307); _tr_str_release(_cmpa_t1306); _cmpr_t1308; })) {
#line 3902 "src/sema.tr"
                nm_referenced = true;
            }
#line 3903 "src/sema.tr"
            tri = (tri + 1LL);
        }
#line 3904 "src/sema.tr"
        if (nm_referenced) {
#line 3904 "src/sema.tr"
            return;
        }
#line 3905 "src/sema.tr"
        List_ptr_remove(b->stmts, (n - 1LL));
#line 3906 "src/sema.tr"
        List_ptr_append(b->stmts, drop);
#line 3907 "src/sema.tr"
        List_ptr_append(b->stmts, last);
    } else {
#line 3909 "src/sema.tr"
        List_ptr_append(b->stmts, drop);
    }
}

__attribute__((hot)) bool Sema__pc_is_ident(Sema* self, HirExpr* ep, TrStr name) {
#line 3912 "src/sema.tr"
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
#line 3912 "src/sema.tr"
        return false;
    }
#line 3913 "src/sema.tr"
    __auto_type _t1309 = (*ep);
    if (_t1309.tag == HirExpr_EIdent) {
        __auto_type n = _t1309.data.EIdent.name;
        bool _trret224 = _tr_str_eqv((n), (name));
        return _trret224;
    } else if (1) {
        __auto_type _ = _t1309;
        return false;
    }
}

__attribute__((hot)) TrStr Sema__pc_callee_name(Sema* self, HirExpr* callee) {
#line 3918 "src/sema.tr"
    if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
#line 3918 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 3919 "src/sema.tr"
    __auto_type _t1310 = (*callee);
    if (_t1310.tag == HirExpr_EIdent) {
        __auto_type n = _t1310.data.EIdent.name;
        TrStr _trret225 = _tr_str_retain(n);
        return _trret225;
    } else if (1) {
        __auto_type _ = _t1310;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool Sema__pc_stores_method(Sema* self, TrStr m) {
#line 3924 "src/sema.tr"
    return ((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL))));
}

__attribute__((hot)) bool Sema__pc_is_store_target(Sema* self, HirExpr* tgt) {
#line 3927 "src/sema.tr"
    if ((((unsigned long long)(tgt)) == ((unsigned long long)(0LL)))) {
#line 3927 "src/sema.tr"
        return false;
    }
#line 3928 "src/sema.tr"
    __auto_type _t1311 = (*tgt);
    if (_t1311.tag == HirExpr_EPropAccess) {
        return true;
    } else if (_t1311.tag == HirExpr_EIndex) {
        return true;
    } else if (1) {
        __auto_type _ = _t1311;
        return false;
    }
}

__attribute__((hot)) bool Sema__pc_refs(Sema* self, HirExpr* ep, TrStr name) {
#line 3935 "src/sema.tr"
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
#line 3935 "src/sema.tr"
        return false;
    }
#line 3936 "src/sema.tr"
    __auto_type _t1312 = (*ep);
    if (_t1312.tag == HirExpr_EIdent) {
        __auto_type n = _t1312.data.EIdent.name;
        bool _trret226 = _tr_str_eqv((n), (name));
        return _trret226;
    } else if (_t1312.tag == HirExpr_EBinOp) {
        __auto_type l = _t1312.data.EBinOp.left;
__auto_type r = _t1312.data.EBinOp.right;
        return (Sema__pc_refs(self, l, name) || Sema__pc_refs(self, r, name));
    } else if (_t1312.tag == HirExpr_EUnaryOp) {
        __auto_type e = _t1312.data.EUnaryOp.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t1312.tag == HirExpr_ECast) {
        __auto_type e = _t1312.data.ECast.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t1312.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1312.data.EPropAccess.obj;
        return Sema__pc_refs(self, o, name);
    } else if (_t1312.tag == HirExpr_EIndex) {
        __auto_type o = _t1312.data.EIndex.obj;
__auto_type ix = _t1312.data.EIndex._tr_v_index;
        return (Sema__pc_refs(self, o, name) || Sema__pc_refs(self, ix, name));
    } else if (_t1312.tag == HirExpr_ECall) {
        __auto_type cl = _t1312.data.ECall.callee;
__auto_type args = _t1312.data.ECall.args;
        return (Sema__pc_refs(self, cl, name) || Sema__pc_anyrefs(self, args, name));
    } else if (_t1312.tag == HirExpr_EMethodCall) {
        __auto_type o = _t1312.data.EMethodCall.obj;
__auto_type args = _t1312.data.EMethodCall.args;
        return (Sema__pc_refs(self, o, name) || Sema__pc_anyrefs(self, args, name));
    } else if (_t1312.tag == HirExpr_EList) {
        __auto_type items = _t1312.data.EList.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t1312.tag == HirExpr_ESet) {
        __auto_type items = _t1312.data.ESet.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t1312.tag == HirExpr_ETuple) {
        __auto_type items = _t1312.data.ETuple.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t1312.tag == HirExpr_EDict) {
        __auto_type dks = _t1312.data.EDict.keys;
__auto_type dvs = _t1312.data.EDict.vals;
        return (Sema__pc_anyrefs(self, dks, name) || Sema__pc_anyrefs(self, dvs, name));
    } else if (_t1312.tag == HirExpr_ETryExpr) {
        __auto_type e = _t1312.data.ETryExpr.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t1312.tag == HirExpr_EAwait) {
        __auto_type e = _t1312.data.EAwait.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t1312.tag == HirExpr_EIfElse) {
        __auto_type cnd = _t1312.data.EIfElse.cond;
__auto_type te = _t1312.data.EIfElse.then_e;
__auto_type ee = _t1312.data.EIfElse.else_e;
        return ((Sema__pc_refs(self, cnd, name) || Sema__pc_refs(self, te, name)) || Sema__pc_refs(self, ee, name));
    } else if (_t1312.tag == HirExpr_ESlice) {
        __auto_type sa = _t1312.data.ESlice.start;
__auto_type sb = _t1312.data.ESlice.stop;
__auto_type sc = _t1312.data.ESlice.step;
        return ((Sema__pc_refs(self, sa, name) || Sema__pc_refs(self, sb, name)) || Sema__pc_refs(self, sc, name));
    } else if (_t1312.tag == HirExpr_ERange) {
        __auto_type ra = _t1312.data.ERange.start;
__auto_type rb = _t1312.data.ERange.end;
        return (Sema__pc_refs(self, ra, name) || Sema__pc_refs(self, rb, name));
    } else if (1) {
        __auto_type _ = _t1312;
        return false;
    }
}

__attribute__((hot)) bool Sema__pc_anyrefs(Sema* self, List_ptr* args, TrStr name) {
#line 3957 "src/sema.tr"
    long long i = 0LL;
#line 3958 "src/sema.tr"
    while ((i < args->len)) {
#line 3959 "src/sema.tr"
        if (Sema__pc_refs(self, ((HirExpr*)List_ptr_get(args, i)), name)) {
#line 3959 "src/sema.tr"
            return true;
        }
#line 3960 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3961 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__param_consumed_in_block(Sema* self, TrStr pname, HirBlock* b) {
#line 3964 "src/sema.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 3964 "src/sema.tr"
        return false;
    }
#line 3965 "src/sema.tr"
    long long i = 0LL;
#line 3966 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 3967 "src/sema.tr"
        if (Sema__param_consumed_in_stmt(self, pname, ((HirStmt*)List_ptr_get(b->stmts, i)))) {
#line 3967 "src/sema.tr"
            return true;
        }
#line 3968 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3969 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__param_consumed_in_stmt(Sema* self, TrStr pname, HirStmt* sp) {
#line 3972 "src/sema.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 3972 "src/sema.tr"
        return false;
    }
#line 3973 "src/sema.tr"
    __auto_type _t1313 = (*sp);
    if (_t1313.tag == HirStmt_SReturn) {
        __auto_type e = _t1313.data.SReturn.val;
#line 3981 "src/sema.tr"
        if ((!Sema__pc_owns_return(self, e))) {
#line 3981 "src/sema.tr"
            return false;
        }
#line 3982 "src/sema.tr"
        return Sema__pc_refs(self, e, pname);
    } else if (_t1313.tag == HirStmt_SRaise) {
        __auto_type e = _t1313.data.SRaise.val;
#line 3984 "src/sema.tr"
        if ((!Sema__pc_owns_return(self, e))) {
#line 3984 "src/sema.tr"
            return false;
        }
#line 3985 "src/sema.tr"
        return Sema__pc_refs(self, e, pname);
    } else if (_t1313.tag == HirStmt_SFree) {
        __auto_type nm = _t1313.data.SFree.name;
        bool _trret227 = _tr_str_eqv((nm), (pname));
        return _trret227;
    } else if (_t1313.tag == HirStmt_SSpawn) {
        __auto_type e = _t1313.data.SSpawn.expr;
        return Sema__pc_refs(self, e, pname);
    } else if (_t1313.tag == HirStmt_SAssign) {
        __auto_type tgt = _t1313.data.SAssign.target;
__auto_type val = _t1313.data.SAssign.val;
#line 3989 "src/sema.tr"
        if ((Sema__pc_is_store_target(self, tgt) && Sema__pc_refs(self, val, pname))) {
#line 3989 "src/sema.tr"
            return true;
        }
#line 3990 "src/sema.tr"
        if ((self->pc_alias_consumes && Sema__pc_is_ident(self, val, pname))) {
#line 3990 "src/sema.tr"
            return true;
        }
#line 3991 "src/sema.tr"
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t1313.tag == HirStmt_SLet) {
        __auto_type val = _t1313.data.SLet.val;
#line 3993 "src/sema.tr"
        if ((self->pc_alias_consumes && Sema__pc_is_ident(self, val, pname))) {
#line 3993 "src/sema.tr"
            return true;
        }
#line 3994 "src/sema.tr"
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t1313.tag == HirStmt_SMultiLet) {
        __auto_type val = _t1313.data.SMultiLet.val;
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t1313.tag == HirStmt_SExpr) {
        __auto_type e = _t1313.data.SExpr.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t1313.tag == HirStmt_SIf) {
        __auto_type tb = _t1313.data.SIf.then_b;
__auto_type eb = _t1313.data.SIf.else_b;
        bool _trret228 = (Sema__param_consumed_in_block(self, pname, tb) || Sema__param_consumed_in_block(self, pname, eb));
        return _trret228;
    } else if (_t1313.tag == HirStmt_SWhile) {
        __auto_type wb = _t1313.data.SWhile.body;
        bool _trret229 = Sema__param_consumed_in_block(self, pname, wb);
        return _trret229;
    } else if (_t1313.tag == HirStmt_SFor) {
        __auto_type fb = _t1313.data.SFor.body;
        bool _trret230 = Sema__param_consumed_in_block(self, pname, fb);
        return _trret230;
    } else if (_t1313.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t1313.data.SForUnpack.body;
        bool _trret231 = Sema__param_consumed_in_block(self, pname, fub);
        return _trret231;
    } else if (_t1313.tag == HirStmt_SMatch) {
        __auto_type arms = _t1313.data.SMatch.arms;
#line 4002 "src/sema.tr"
        long long ai = 0LL;
#line 4003 "src/sema.tr"
        while ((ai < arms->len)) {
#line 4004 "src/sema.tr"
            if (Sema__param_consumed_in_block(self, pname, ((HirMatchArm*)List_ptr_get(arms, ai))->body)) {
#line 4004 "src/sema.tr"
                return true;
            }
#line 4005 "src/sema.tr"
            ai = (ai + 1LL);
        }
#line 4006 "src/sema.tr"
        return false;
    } else if (_t1313.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t1313.data.SUnsafe.body;
        bool _trret232 = Sema__param_consumed_in_block(self, pname, ub);
        return _trret232;
    } else if (_t1313.tag == HirStmt_SWith) {
        __auto_type wb = _t1313.data.SWith.body;
        bool _trret233 = Sema__param_consumed_in_block(self, pname, wb);
        return _trret233;
    } else if (_t1313.tag == HirStmt_STaskGroup) {
        __auto_type tb = _t1313.data.STaskGroup.body;
        bool _trret234 = Sema__param_consumed_in_block(self, pname, tb);
        return _trret234;
    } else if (_t1313.tag == HirStmt_SDefer) {
        __auto_type ds = _t1313.data.SDefer.stmt;
        return Sema__param_consumed_in_stmt(self, pname, ds);
    } else if (_t1313.tag == HirStmt_STry) {
        __auto_type tb = _t1313.data.STry.try_body;
__auto_type catches = _t1313.data.STry.catches;
__auto_type fb = _t1313.data.STry.finally_b;
#line 4012 "src/sema.tr"
        if (Sema__param_consumed_in_block(self, pname, tb)) {
#line 4012 "src/sema.tr"
            return true;
        }
#line 4013 "src/sema.tr"
        if (Sema__param_consumed_in_block(self, pname, fb)) {
#line 4013 "src/sema.tr"
            return true;
        }
#line 4014 "src/sema.tr"
        long long ci = 0LL;
#line 4015 "src/sema.tr"
        while ((ci < catches->len)) {
#line 4016 "src/sema.tr"
            if (Sema__param_consumed_in_block(self, pname, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body)) {
#line 4016 "src/sema.tr"
                return true;
            }
#line 4017 "src/sema.tr"
            ci = (ci + 1LL);
        }
#line 4018 "src/sema.tr"
        return false;
    } else if (1) {
        __auto_type _ = _t1313;
        return false;
    }
}

__attribute__((hot)) bool Sema__param_consumed_in_expr(Sema* self, TrStr pname, HirExpr* ep) {
#line 4024 "src/sema.tr"
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
#line 4024 "src/sema.tr"
        return false;
    }
#line 4025 "src/sema.tr"
    __auto_type _t1314 = (*ep);
    if (_t1314.tag == HirExpr_ECall) {
        __auto_type callee = _t1314.data.ECall.callee;
__auto_type args = _t1314.data.ECall.args;
#line 4027 "src/sema.tr"
        TrStr gname = Sema__pc_callee_name(self, callee);
#line 4028 "src/sema.tr"
        long long j = 0LL;
#line 4029 "src/sema.tr"
        while ((j < args->len)) {
#line 4030 "src/sema.tr"
            if (Sema__pc_is_ident(self, ((HirExpr*)List_ptr_get(args, j)), pname)) {
#line 4031 "src/sema.tr"
                if (_tr_str_eqv((gname), (_tr_str_lit_len("", 0LL)))) {
#line 4031 "src/sema.tr"
                    _tr_str_release(gname);
                    return true;
                }
#line 4032 "src/sema.tr"
                TrStr ck = ({ TrStr _tr_cl = (_tr_strx_concatv((gname), (_tr_str_lit_len("#", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(j)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 4033 "src/sema.tr"
                if ((!_tr_dict_contains(self->fn_param_consumes, _tr_strz(ck)))) {
#line 4033 "src/sema.tr"
                    _tr_str_release(gname);
                    _tr_str_release(ck);
                    return true;
                }
#line 4034 "src/sema.tr"
                if (((bool)(uintptr_t)_tr_dict_get(self->fn_param_consumes, _tr_strz(ck)))) {
#line 4034 "src/sema.tr"
                    _tr_str_release(gname);
                    _tr_str_release(ck);
                    return true;
                }
            } else {
#line 4036 "src/sema.tr"
                if (Sema__param_consumed_in_expr(self, pname, ((HirExpr*)List_ptr_get(args, j)))) {
#line 4036 "src/sema.tr"
                    _tr_str_release(gname);
                    return true;
                }
            }
#line 4037 "src/sema.tr"
            j = (j + 1LL);
        }
#line 4038 "src/sema.tr"
        bool _trret235 = Sema__param_consumed_in_expr(self, pname, callee);
        _tr_str_release(gname);
        return _trret235;
    } else if (_t1314.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1314.data.EMethodCall.obj;
__auto_type method = _t1314.data.EMethodCall.method;
__auto_type args = _t1314.data.EMethodCall.args;
#line 4044 "src/sema.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL))) && Sema__pc_is_ident(self, obj, pname))) {
#line 4044 "src/sema.tr"
            return true;
        }
#line 4045 "src/sema.tr"
        long long j2 = 0LL;
#line 4046 "src/sema.tr"
        while ((j2 < args->len)) {
#line 4047 "src/sema.tr"
            if (Sema__pc_is_ident(self, ((HirExpr*)List_ptr_get(args, j2)), pname)) {
#line 4047 "src/sema.tr"
                return true;
            }
#line 4048 "src/sema.tr"
            if (Sema__param_consumed_in_expr(self, pname, ((HirExpr*)List_ptr_get(args, j2)))) {
#line 4048 "src/sema.tr"
                return true;
            }
#line 4049 "src/sema.tr"
            j2 = (j2 + 1LL);
        }
#line 4050 "src/sema.tr"
        bool _trret236 = Sema__param_consumed_in_expr(self, pname, obj);
        return _trret236;
    } else if (_t1314.tag == HirExpr_EList) {
        __auto_type items = _t1314.data.EList.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t1314.tag == HirExpr_ESet) {
        __auto_type items = _t1314.data.ESet.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t1314.tag == HirExpr_ETuple) {
        __auto_type items = _t1314.data.ETuple.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t1314.tag == HirExpr_EDict) {
        __auto_type dks = _t1314.data.EDict.keys;
__auto_type dvs = _t1314.data.EDict.vals;
        return (Sema__pc_anyrefs(self, dks, pname) || Sema__pc_anyrefs(self, dvs, pname));
    } else if (_t1314.tag == HirExpr_EClosure) {
        __auto_type caps = _t1314.data.EClosure.captures;
#line 4056 "src/sema.tr"
        long long cj = 0LL;
#line 4057 "src/sema.tr"
        while ((cj < caps->len)) {
#line 4058 "src/sema.tr"
            if (_tr_str_eqv((((HirParam*)List_ptr_get(caps, cj))->name), (pname))) {
#line 4058 "src/sema.tr"
                return true;
            }
#line 4059 "src/sema.tr"
            cj = (cj + 1LL);
        }
#line 4060 "src/sema.tr"
        return false;
    } else if (_t1314.tag == HirExpr_EBinOp) {
        __auto_type l = _t1314.data.EBinOp.left;
__auto_type r = _t1314.data.EBinOp.right;
        return (Sema__param_consumed_in_expr(self, pname, l) || Sema__param_consumed_in_expr(self, pname, r));
    } else if (_t1314.tag == HirExpr_EUnaryOp) {
        __auto_type e = _t1314.data.EUnaryOp.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t1314.tag == HirExpr_ECast) {
        __auto_type e = _t1314.data.ECast.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t1314.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1314.data.EPropAccess.obj;
        return Sema__param_consumed_in_expr(self, pname, o);
    } else if (_t1314.tag == HirExpr_EIndex) {
        __auto_type o = _t1314.data.EIndex.obj;
__auto_type ix = _t1314.data.EIndex._tr_v_index;
        return (Sema__param_consumed_in_expr(self, pname, o) || Sema__param_consumed_in_expr(self, pname, ix));
    } else if (_t1314.tag == HirExpr_EIfElse) {
        __auto_type cnd = _t1314.data.EIfElse.cond;
__auto_type te = _t1314.data.EIfElse.then_e;
__auto_type ee = _t1314.data.EIfElse.else_e;
        return ((Sema__param_consumed_in_expr(self, pname, cnd) || Sema__param_consumed_in_expr(self, pname, te)) || Sema__param_consumed_in_expr(self, pname, ee));
    } else if (_t1314.tag == HirExpr_EAwait) {
        __auto_type e = _t1314.data.EAwait.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t1314.tag == HirExpr_ETryExpr) {
        __auto_type e = _t1314.data.ETryExpr.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (1) {
        __auto_type _ = _t1314;
        return false;
    }
}

__attribute__((hot)) bool Sema__fn_ret_is_heap_class(Sema* self, HirFunction* f) {
#line 4072 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) == ((unsigned long long)(0LL)))) {
#line 4072 "src/sema.tr"
        return false;
    }
#line 4073 "src/sema.tr"
    TrStr rn = _tr_str_retain(f->ret_ty->name);
#line 4074 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(rn)))) {
#line 4074 "src/sema.tr"
        _tr_str_release(rn);
        return false;
    }
#line 4075 "src/sema.tr"
    if (((((((((((((((_tr_str_eqv((rn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((rn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Set", 3LL)))) || (_tr_str_eqv((rn), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((rn), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 4075 "src/sema.tr"
        _tr_str_release(rn);
        return false;
    }
#line 4077 "src/sema.tr"
    ClassDef* cinfo = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(rn)));
#line 4078 "src/sema.tr"
    if ((!cinfo->is_class)) {
#line 4078 "src/sema.tr"
        _tr_str_release(rn);
        return false;
    }
#line 4079 "src/sema.tr"
    _tr_str_release(rn);
    return true;
}

__attribute__((hot)) void Sema__collect_returns(Sema* self, HirBlock* b, List_ptr* out) {
#line 4085 "src/sema.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 4085 "src/sema.tr"
        return;
    }
#line 4086 "src/sema.tr"
    long long i = 0LL;
#line 4087 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 4088 "src/sema.tr"
        Sema__collect_returns_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)), out);
#line 4089 "src/sema.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema__collect_returns_stmt(Sema* self, HirStmt* sp, List_ptr* out) {
#line 4092 "src/sema.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 4092 "src/sema.tr"
        return;
    }
#line 4093 "src/sema.tr"
    __auto_type _t1315 = (*sp);
    if (_t1315.tag == HirStmt_SReturn) {
        __auto_type re = _t1315.data.SReturn.val;
#line 4095 "src/sema.tr"
        if ((((unsigned long long)(re)) != ((unsigned long long)(0LL)))) {
#line 4095 "src/sema.tr"
            List_ptr_append(out, re);
        }
    } else if (_t1315.tag == HirStmt_SIf) {
        __auto_type tb = _t1315.data.SIf.then_b;
__auto_type eb = _t1315.data.SIf.else_b;
#line 4097 "src/sema.tr"
        Sema__collect_returns(self, tb, out);
#line 4098 "src/sema.tr"
        Sema__collect_returns(self, eb, out);
    } else if (_t1315.tag == HirStmt_SWhile) {
        __auto_type wb = _t1315.data.SWhile.body;
        Sema__collect_returns(self, wb, out);
    } else if (_t1315.tag == HirStmt_SFor) {
        __auto_type fb = _t1315.data.SFor.body;
        Sema__collect_returns(self, fb, out);
    } else if (_t1315.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t1315.data.SForUnpack.body;
        Sema__collect_returns(self, fub, out);
    } else if (_t1315.tag == HirStmt_SMatch) {
        __auto_type marms = _t1315.data.SMatch.arms;
#line 4103 "src/sema.tr"
        long long ai = 0LL;
#line 4104 "src/sema.tr"
        while ((ai < marms->len)) {
#line 4105 "src/sema.tr"
            Sema__collect_returns(self, ((HirMatchArm*)List_ptr_get(marms, ai))->body, out);
#line 4106 "src/sema.tr"
            ai = (ai + 1LL);
        }
    } else if (_t1315.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t1315.data.SUnsafe.body;
        Sema__collect_returns(self, ub, out);
    } else if (_t1315.tag == HirStmt_SWith) {
        __auto_type wb2 = _t1315.data.SWith.body;
        Sema__collect_returns(self, wb2, out);
    } else if (_t1315.tag == HirStmt_STaskGroup) {
        __auto_type tgb = _t1315.data.STaskGroup.body;
        Sema__collect_returns(self, tgb, out);
    } else if (_t1315.tag == HirStmt_SGpuBlock) {
        __auto_type gb = _t1315.data.SGpuBlock.body;
        Sema__collect_returns(self, gb, out);
    } else if (_t1315.tag == HirStmt_SDefer) {
        __auto_type ds = _t1315.data.SDefer.stmt;
        Sema__collect_returns_stmt(self, ds, out);
    } else if (_t1315.tag == HirStmt_STry) {
        __auto_type tb2 = _t1315.data.STry.try_body;
__auto_type catches = _t1315.data.STry.catches;
__auto_type fb2 = _t1315.data.STry.finally_b;
#line 4113 "src/sema.tr"
        Sema__collect_returns(self, tb2, out);
#line 4114 "src/sema.tr"
        long long ci = 0LL;
#line 4115 "src/sema.tr"
        while ((ci < catches->len)) {
#line 4116 "src/sema.tr"
            HirCatchClause** cc = ((HirCatchClause**)List_ptr_get(catches, ci));
#line 4117 "src/sema.tr"
            if ((((unsigned long long)(cc)) != ((unsigned long long)(0LL)))) {
#line 4117 "src/sema.tr"
                Sema__collect_returns(self, (*cc)->body, out);
            }
#line 4118 "src/sema.tr"
            ci = (ci + 1LL);
        }
#line 4119 "src/sema.tr"
        Sema__collect_returns(self, fb2, out);
    } else if (1) {
        __auto_type _ = _t1315;
        /* pass */
    }
}

__attribute__((hot)) bool Sema__owned_of(Sema* self, TrStr key) {
#line 4124 "src/sema.tr"
    if (_tr_dict_contains(self->fn_ret_owned, _tr_strz(key))) {
#line 4124 "src/sema.tr"
        return ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(key)));
    }
#line 4125 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__ret_yields_owned(Sema* self, HirExpr* e) {
#line 4128 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 4128 "src/sema.tr"
        return false;
    }
#line 4129 "src/sema.tr"
    __auto_type _t1316 = (*e);
    if (_t1316.tag == HirExpr_EIdent) {
        return true;
    } else if (_t1316.tag == HirExpr_EPropAccess) {
        return true;
    } else if (_t1316.tag == HirExpr_ECall) {
        __auto_type callee = _t1316.data.ECall.callee;
#line 4133 "src/sema.tr"
        if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
#line 4133 "src/sema.tr"
            return false;
        }
#line 4134 "src/sema.tr"
        __auto_type _t1317 = (*callee);
        if (_t1317.tag == HirExpr_EIdent) {
            __auto_type cn = _t1317.data.EIdent.name;
#line 4136 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(cn))) {
#line 4136 "src/sema.tr"
                return true;
            }
#line 4137 "src/sema.tr"
            bool _trret237 = Sema__owned_of(self, cn);
            return _trret237;
        } else if (1) {
            __auto_type _ = _t1317;
            return false;
        }
    } else if (_t1316.tag == HirExpr_EMethodCall) {
        __auto_type _tr_v_recv = _t1316.data.EMethodCall.obj;
__auto_type m = _t1316.data.EMethodCall.method;
#line 4140 "src/sema.tr"
        TrStr rt = _tr_str_retain(hir_expr_type(_tr_v_recv)->name);
#line 4142 "src/sema.tr"
        if ((((((_tr_str_eqv((rt), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((rt), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((rt), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((rt), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((rt), (_tr_str_lit_len("Set", 3LL)))) && ((_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("last", 4LL)))))) {
#line 4143 "src/sema.tr"
            _tr_str_release(rt);
            return false;
        }
#line 4145 "src/sema.tr"
        __auto_type _t1318 = (*_tr_v_recv);
        if (_t1318.tag == HirExpr_EIdent) {
            __auto_type rn = _t1318.data.EIdent.name;
#line 4147 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(rn))) {
#line 4147 "src/sema.tr"
                TrStr _strtmp_t1319 = _tr_str_retain(rn);
                _tr_str_release(rt);
                rt = _strtmp_t1319;
            }
        } else if (1) {
            __auto_type _ = _t1318;
            /* pass */
        }
#line 4149 "src/sema.tr"
        bool _trret238 = ({ TrStr _at_t1320 = (({ TrStr _tr_cl = (_tr_strx_concatv((rt), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (Sema__owned_of(self, _at_t1320)); _tr_str_release(_at_t1320); _wr; });
        _tr_str_release(rt);
        return _trret238;
    } else if (_t1316.tag == HirExpr_EIndex) {
        return false;
    } else if (_t1316.tag == HirExpr_EIfElse) {
        __auto_type a = _t1316.data.EIfElse.then_e;
__auto_type b2 = _t1316.data.EIfElse.else_e;
#line 4152 "src/sema.tr"
        return (Sema__ret_yields_owned(self, a) && Sema__ret_yields_owned(self, b2));
    } else if (1) {
        __auto_type _ = _t1316;
        return false;
    }
}

__attribute__((hot)) bool Sema__is_type_param_in_scope(Sema* self, TrStr name) {
#line 4164 "src/sema.tr"
    if ((_tr_strlen(_tr_strz(name)) == 1LL)) {
#line 4164 "src/sema.tr"
        return true;
    }
#line 4165 "src/sema.tr"
    long long i = 0LL;
#line 4166 "src/sema.tr"
    while ((i < self->current_func_generics->len)) {
#line 4167 "src/sema.tr"
        if (({ TrStr _cmpa_t1321 = (List_TrStr_get(self->current_func_generics, i)); TrStr _cmpb_t1322 = (name); _Bool _cmpr_t1323 = _tr_str_eqv(_cmpa_t1321, _cmpb_t1322); _tr_str_release(_cmpa_t1321); _cmpr_t1323; })) {
#line 4167 "src/sema.tr"
            return true;
        }
#line 4168 "src/sema.tr"
        i = (i + 1LL);
    }
#line 4184 "src/sema.tr"
    if (((!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->classes, _tr_strz(self->current_class_name)))) {
#line 4185 "src/sema.tr"
        ClassDef* ccls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
#line 4186 "src/sema.tr"
        long long ci = 0LL;
#line 4187 "src/sema.tr"
        while ((ci < ccls->generics->len)) {
#line 4188 "src/sema.tr"
            if (({ TrStr _cmpa_t1324 = (List_TrStr_get(ccls->generics, ci)); TrStr _cmpb_t1325 = (name); _Bool _cmpr_t1326 = _tr_str_eqv(_cmpa_t1324, _cmpb_t1325); _tr_str_release(_cmpa_t1324); _cmpr_t1326; })) {
#line 4188 "src/sema.tr"
                return true;
            }
#line 4189 "src/sema.tr"
            ci = (ci + 1LL);
        }
    }
#line 4190 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__type_satisfies_bound(Sema* self, TrStr type_name, TrStr iface_name) {
#line 4193 "src/sema.tr"
    if ((!_tr_dict_contains(self->interfaces, _tr_strz(iface_name)))) {
#line 4193 "src/sema.tr"
        return true;
    }
#line 4196 "src/sema.tr"
    if (Sema__is_type_param_in_scope(self, type_name)) {
#line 4196 "src/sema.tr"
        return true;
    }
#line 4197 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(type_name)))) {
#line 4197 "src/sema.tr"
        return false;
    }
#line 4198 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(type_name)));
#line 4199 "src/sema.tr"
    long long ii = 0LL;
#line 4200 "src/sema.tr"
    while ((ii < cls->iface_names->len)) {
#line 4201 "src/sema.tr"
        if (({ TrStr _cmpa_t1327 = (List_TrStr_get(cls->iface_names, ii)); TrStr _cmpb_t1328 = (iface_name); _Bool _cmpr_t1329 = _tr_str_eqv(_cmpa_t1327, _cmpb_t1328); _tr_str_release(_cmpa_t1327); _cmpr_t1329; })) {
#line 4201 "src/sema.tr"
            return true;
        }
#line 4202 "src/sema.tr"
        ii = (ii + 1LL);
    }
#line 4203 "src/sema.tr"
    InterfaceDef* idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(iface_name)));
#line 4204 "src/sema.tr"
    long long mi = 0LL;
#line 4205 "src/sema.tr"
    while ((mi < idef->methods->len)) {
#line 4206 "src/sema.tr"
        TrStr imeth_nm = _tr_str_retain(((FunctionDef*)List_ptr_get(idef->methods, mi))->name);
#line 4207 "src/sema.tr"
        bool found = false;
#line 4208 "src/sema.tr"
        long long cmi = 0LL;
#line 4209 "src/sema.tr"
        while ((cmi < cls->methods->len)) {
#line 4210 "src/sema.tr"
            if (_tr_str_eqv((((FunctionDef*)List_ptr_get(cls->methods, cmi))->name), (imeth_nm))) {
#line 4210 "src/sema.tr"
                found = true;
            }
#line 4211 "src/sema.tr"
            cmi = (cmi + 1LL);
        }
#line 4212 "src/sema.tr"
        if ((!found)) {
#line 4212 "src/sema.tr"
            _tr_str_release(imeth_nm);
            return false;
        }
#line 4213 "src/sema.tr"
        mi = (mi + 1LL);
        _tr_str_release(imeth_nm);
    }
#line 4214 "src/sema.tr"
    return true;
}

__attribute__((hot)) AstType** Sema__resolve_generic_bound_method_ret(Sema* self, TrStr pname, TrStr method) {
#line 4225 "src/sema.tr"
    long long ci = 0LL;
#line 4226 "src/sema.tr"
    while ((ci < self->current_func_constraints->len)) {
#line 4227 "src/sema.tr"
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(self->current_func_constraints, ci));
#line 4228 "src/sema.tr"
        if (_tr_str_eqv((gc->target), (pname))) {
#line 4229 "src/sema.tr"
            long long bi = 0LL;
#line 4230 "src/sema.tr"
            while ((bi < gc->bounds->len)) {
#line 4231 "src/sema.tr"
                AstType* bound_ty = (*((AstType**)List_ptr_get(gc->bounds, bi)));
#line 4232 "src/sema.tr"
                if (_tr_dict_contains(self->interfaces, _tr_strz(bound_ty->name))) {
#line 4233 "src/sema.tr"
                    InterfaceDef* idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(bound_ty->name)));
#line 4234 "src/sema.tr"
                    long long mi = 0LL;
#line 4235 "src/sema.tr"
                    while ((mi < idef->methods->len)) {
#line 4236 "src/sema.tr"
                        FunctionDef* imdef = ((FunctionDef*)List_ptr_get(idef->methods, mi));
#line 4237 "src/sema.tr"
                        if ((_tr_str_eqv((imdef->name), (method)) && (((unsigned long long)(imdef->ret_ty)) != ((unsigned long long)(0LL))))) {
#line 4238 "src/sema.tr"
                            AstType* found_ty = (*imdef->ret_ty);
#line 4242 "src/sema.tr"
                            if (((idef->generics->len > 0LL) && (bound_ty->args->len > 0LL))) {
#line 4243 "src/sema.tr"
                                long long gi = 0LL;
#line 4244 "src/sema.tr"
                                while ((gi < idef->generics->len)) {
#line 4245 "src/sema.tr"
                                    if ((({ __auto_type _ord_t1330 = (List_TrStr_get(idef->generics, gi)); __auto_type _wr = (({ TrStr _cmpa_t1331 = (_ord_t1330); TrStr _cmpb_t1332 = (found_ty->name); _Bool _cmpr_t1333 = _tr_str_eqv(_cmpa_t1331, _cmpb_t1332); _tr_str_release(_cmpa_t1331); _cmpr_t1333; })); _wr; }) && (gi < bound_ty->args->len))) {
#line 4246 "src/sema.tr"
                                        found_ty = (*((AstType**)List_ptr_get(bound_ty->args, gi)));
                                    }
#line 4247 "src/sema.tr"
                                    gi = (gi + 1LL);
                                }
                            }
#line 4248 "src/sema.tr"
                            AstType** _trret239 = box_asttype(found_ty);
                            return _trret239;
                        }
#line 4249 "src/sema.tr"
                        mi = (mi + 1LL);
                    }
                }
#line 4250 "src/sema.tr"
                bi = (bi + 1LL);
            }
        }
#line 4251 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 4252 "src/sema.tr"
    return ((AstType**)(0LL));
}

__attribute__((hot)) long long Sema_fn_value_throws_kind(Sema* self, AstType* ty) {
#line 4263 "src/sema.tr"
    if (((!_tr_str_eqv((ty->name), (_tr_str_lit_len("def", 3LL)))) || (ty->args->len == 0LL))) {
#line 4263 "src/sema.tr"
        return (0LL - 1LL);
    }
#line 4264 "src/sema.tr"
    if ((*((AstType**)List_ptr_get(ty->args, (ty->args->len - 1LL))))->is_throws) {
#line 4264 "src/sema.tr"
        return 1LL;
    }
#line 4265 "src/sema.tr"
    return 0LL;
}

__attribute__((hot)) void Sema_check_fn_value_throws(Sema* self, AstType** expected_p, AstType* actual, TrStr what) {
#line 4268 "src/sema.tr"
    if ((((unsigned long long)(expected_p)) == ((unsigned long long)(0LL)))) {
#line 4268 "src/sema.tr"
        return;
    }
#line 4269 "src/sema.tr"
    AstType* expected = (*expected_p);
#line 4273 "src/sema.tr"
    if (((_tr_str_eqv((expected->name), (_tr_str_lit_len("lambda", 6LL))) && _tr_str_eqv((actual->name), (_tr_str_lit_len("def", 3LL)))) && (actual->args->len > 0LL))) {
#line 4274 "src/sema.tr"
        AstType* lam_ret = (*((AstType**)List_ptr_get(actual->args, (actual->args->len - 1LL))));
#line 4275 "src/sema.tr"
        if (((((!_tr_str_eqv((lam_ret->name), (_tr_str_lit_len("int", 3LL)))) && (!_tr_str_eqv((lam_ret->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((lam_ret->name), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((lam_ret->name), (_tr_str_lit_len("None", 4LL)))))) {
#line 4276 "src/sema.tr"
            ({ TrStr _at_t1334 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-5] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is a `lambda` slot, which is called as returning `int`, but the function/closure passed returns '", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lam_ret->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: give the parameter a full function type (`def(...) -> ", 68LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lam_ret->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("`) so the result type is known.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1334); _tr_str_release(_at_t1334); });
#line 4277 "src/sema.tr"
            return;
        }
    }
#line 4278 "src/sema.tr"
    long long ek = Sema_fn_value_throws_kind(self, expected);
#line 4279 "src/sema.tr"
    long long ak = Sema_fn_value_throws_kind(self, actual);
#line 4280 "src/sema.tr"
    if ((((ek < 0LL) || (ak < 0LL)) || (ek == ak))) {
#line 4280 "src/sema.tr"
        return;
    }
#line 4281 "src/sema.tr"
    if ((ek == 1LL)) {
#line 4282 "src/sema.tr"
        ({ TrStr _at_t1335 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-4] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" expects a `throws` function value (`def(...) throws E -> R`) but a plain function/closure was passed. The two have different return types (a throws function returns a Result), so they cannot be exchanged.\n      FIX: declare the argument `throws` too (`def (...) throws E -> R:` / `def f(...) throws E -> R`), or change the parameter type to a plain `def(...) -> R`.", 366LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1335); _tr_str_release(_at_t1335); });
    } else {
#line 4284 "src/sema.tr"
        ({ TrStr _at_t1336 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-4] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" expects a plain function value (`def(...) -> R`) but a `throws` function/closure was passed. The two have different return types (a throws function returns a Result), so they cannot be exchanged.\n      FIX: handle the error inside a plain wrapper (`def (...) -> R:` that checks `.is_err`), or change the parameter type to `def(...) throws E -> R`.", 348LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1336); _tr_str_release(_at_t1336); });
    }
}

__attribute__((hot)) void Sema_check_call_fn_args(Sema* self, TrStr fname, List_ptr* hargs) {
#line 4288 "src/sema.tr"
    if ((!_tr_dict_contains(self->fn_defs, _tr_strz(fname)))) {
#line 4288 "src/sema.tr"
        return;
    }
#line 4289 "src/sema.tr"
    FunctionDef* fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(fname)));
#line 4290 "src/sema.tr"
    if ((fd->generics->len > 0LL)) {
#line 4290 "src/sema.tr"
        return;
    }
#line 4291 "src/sema.tr"
    long long sk = 0LL;
#line 4292 "src/sema.tr"
    if (((fd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(fd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4292 "src/sema.tr"
        sk = 1LL;
    }
#line 4293 "src/sema.tr"
    long long pi = sk;
#line 4294 "src/sema.tr"
    while ((pi < fd->params->len)) {
#line 4295 "src/sema.tr"
        long long aidx = (pi - sk);
#line 4296 "src/sema.tr"
        if ((aidx < hargs->len)) {
#line 4297 "src/sema.tr"
            ({ __auto_type _ord_t1337 = (((Param*)List_ptr_get(fd->params, pi))->ty); __auto_type _ord_t1338 = (hir_expr_type(((HirExpr*)List_ptr_get(hargs, aidx)))); TrStr _ord_t1339 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((aidx + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_fn_value_throws(self, _ord_t1337, _ord_t1338, _ord_t1339); _tr_str_release(_ord_t1339); });
        }
#line 4298 "src/sema.tr"
        pi = (pi + 1LL);
    }
}

__attribute__((hot)) AstType* Sema_closure_return_type(Sema* self, HirBlock* block) {
#line 4302 "src/sema.tr"
    AstType* nothing = AstType_init(_tr_str_lit_len("", 0LL));
#line 4303 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(block)))) {
#line 4303 "src/sema.tr"
        return nothing;
    }
#line 4304 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(block->stmts)))) {
#line 4304 "src/sema.tr"
        return nothing;
    }
#line 4305 "src/sema.tr"
    long long i = 0LL;
#line 4306 "src/sema.tr"
    while ((i < block->stmts->len)) {
#line 4307 "src/sema.tr"
        HirStmt* st = ((HirStmt*)List_ptr_get(block->stmts, i));
#line 4308 "src/sema.tr"
        if ((!_is_invalid_ptr(((unsigned long long)(st))))) {
#line 4309 "src/sema.tr"
            __auto_type _t1340 = (*st);
            if (_t1340.tag == HirStmt_SReturn) {
                __auto_type v = _t1340.data.SReturn.val;
#line 4311 "src/sema.tr"
                if ((!_is_invalid_ptr(((unsigned long long)(v))))) {
#line 4311 "src/sema.tr"
                    return hir_expr_type(v);
                }
            } else if (_t1340.tag == HirStmt_SIf) {
                __auto_type t = _t1340.data.SIf.then_b;
__auto_type e = _t1340.data.SIf.else_b;
#line 4313 "src/sema.tr"
                AstType* tt = Sema_closure_return_type(self, t);
#line 4314 "src/sema.tr"
                if ((!_tr_str_eqv((tt->name), (_tr_str_lit_len("", 0LL))))) {
#line 4314 "src/sema.tr"
                    return tt;
                }
#line 4315 "src/sema.tr"
                if ((!_is_invalid_ptr(((unsigned long long)(e))))) {
#line 4316 "src/sema.tr"
                    AstType* et = Sema_closure_return_type(self, e);
#line 4317 "src/sema.tr"
                    if ((!_tr_str_eqv((et->name), (_tr_str_lit_len("", 0LL))))) {
#line 4317 "src/sema.tr"
                        return et;
                    }
                }
            } else if (_t1340.tag == HirStmt_SWhile) {
                __auto_type wb = _t1340.data.SWhile.body;
#line 4319 "src/sema.tr"
                AstType* wt = Sema_closure_return_type(self, wb);
#line 4320 "src/sema.tr"
                if ((!_tr_str_eqv((wt->name), (_tr_str_lit_len("", 0LL))))) {
#line 4320 "src/sema.tr"
                    return wt;
                }
            } else if (_t1340.tag == HirStmt_SFor) {
                __auto_type fb = _t1340.data.SFor.body;
#line 4322 "src/sema.tr"
                AstType* ft = Sema_closure_return_type(self, fb);
#line 4323 "src/sema.tr"
                if ((!_tr_str_eqv((ft->name), (_tr_str_lit_len("", 0LL))))) {
#line 4323 "src/sema.tr"
                    return ft;
                }
            } else if (_t1340.tag == HirStmt_SUnsafe) {
                __auto_type ub = _t1340.data.SUnsafe.body;
#line 4325 "src/sema.tr"
                AstType* ut = Sema_closure_return_type(self, ub);
#line 4326 "src/sema.tr"
                if ((!_tr_str_eqv((ut->name), (_tr_str_lit_len("", 0LL))))) {
#line 4326 "src/sema.tr"
                    return ut;
                }
            } else if (1) {
                __auto_type _ = _t1340;
                /* pass */
            }
        }
#line 4328 "src/sema.tr"
        i = (i + 1LL);
    }
#line 4329 "src/sema.tr"
    return nothing;
}

__attribute__((hot)) void Sema_check_call_bounds(Sema* self, TrStr fname, List_ptr* hargs) {
#line 4332 "src/sema.tr"
    if ((!_tr_dict_contains(self->fn_defs, _tr_strz(fname)))) {
#line 4332 "src/sema.tr"
        return;
    }
#line 4333 "src/sema.tr"
    FunctionDef* fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(fname)));
#line 4334 "src/sema.tr"
    if ((fd->constraints->len == 0LL)) {
#line 4334 "src/sema.tr"
        return;
    }
#line 4335 "src/sema.tr"
    long long sk = 0LL;
#line 4336 "src/sema.tr"
    if (((fd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(fd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4336 "src/sema.tr"
        sk = 1LL;
    }
#line 4337 "src/sema.tr"
    long long ci = 0LL;
#line 4338 "src/sema.tr"
    while ((ci < fd->constraints->len)) {
#line 4339 "src/sema.tr"
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(fd->constraints, ci));
#line 4340 "src/sema.tr"
        long long pi = 0LL;
#line 4341 "src/sema.tr"
        while ((pi < fd->params->len)) {
#line 4342 "src/sema.tr"
            AstType** _pty = ((Param*)List_ptr_get(fd->params, pi))->ty;
#line 4343 "src/sema.tr"
            if (((((unsigned long long)(_pty)) != ((unsigned long long)(0LL))) && ({ __auto_type _ord_t1341 = ((*_pty)->name); __auto_type _wr = (_tr_str_eqv((_ord_t1341), (gc->target))); _wr; }))) {
#line 4344 "src/sema.tr"
                long long aidx = (pi - sk);
#line 4345 "src/sema.tr"
                if (((aidx >= 0LL) && (aidx < hargs->len))) {
#line 4346 "src/sema.tr"
                    TrStr concrete_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(hargs, aidx)))->name);
#line 4347 "src/sema.tr"
                    long long bi = 0LL;
#line 4348 "src/sema.tr"
                    while ((bi < gc->bounds->len)) {
#line 4349 "src/sema.tr"
                        TrStr iface_n = _tr_str_retain((*((AstType**)List_ptr_get(gc->bounds, bi)))->name);
#line 4350 "src/sema.tr"
                        if ((!Sema__type_satisfies_bound(self, concrete_n, iface_n))) {
#line 4351 "src/sema.tr"
                            ({ TrStr _at_t1342 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[G-1] Type '", 12LL)), (concrete_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' does not satisfy the bound '", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' required by type parameter '", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gc->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: implement all methods of interface '", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' on '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (concrete_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or add 'implements ", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1342); _tr_str_release(_at_t1342); });
                        }
#line 4352 "src/sema.tr"
                        bi = (bi + 1LL);
                        _tr_str_release(iface_n);
                    }
                }
            }
#line 4353 "src/sema.tr"
            pi = (pi + 1LL);
        }
#line 4354 "src/sema.tr"
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) AstType* Sema__subst_ret_generics(Sema* self, AstType* ty, List_TrStr* generics, List_ptr* concrete) {
#line 4361 "src/sema.tr"
    long long gi = 0LL;
#line 4362 "src/sema.tr"
    while ((gi < generics->len)) {
#line 4363 "src/sema.tr"
        if ((({ __auto_type _ord_t1343 = (List_TrStr_get(generics, gi)); __auto_type _wr = (({ TrStr _cmpa_t1344 = (_ord_t1343); TrStr _cmpb_t1345 = (ty->name); _Bool _cmpr_t1346 = _tr_str_eqv(_cmpa_t1344, _cmpb_t1345); _tr_str_release(_cmpa_t1344); _cmpr_t1346; })); _wr; }) && (gi < concrete->len))) {
#line 4364 "src/sema.tr"
            return (*((AstType**)List_ptr_get(concrete, gi)));
        }
#line 4365 "src/sema.tr"
        gi = (gi + 1LL);
    }
#line 4366 "src/sema.tr"
    if ((ty->args->len == 0LL)) {
#line 4366 "src/sema.tr"
        return ty;
    }
#line 4367 "src/sema.tr"
    AstType* nt = AstType_init(ty->name);
#line 4368 "src/sema.tr"
    nt->array_size = ty->array_size;
#line 4369 "src/sema.tr"
    nt->args = (void*)List_ptr_new();
#line 4370 "src/sema.tr"
    long long ai = 0LL;
#line 4371 "src/sema.tr"
    while ((ai < ty->args->len)) {
#line 4372 "src/sema.tr"
        ({ __auto_type _ord_t1347 = (box_asttype(Sema__subst_ret_generics(self, (*((AstType**)List_ptr_get(ty->args, ai))), generics, concrete))); List_ptr_append(nt->args, _ord_t1347); });
#line 4373 "src/sema.tr"
        ai = (ai + 1LL);
    }
#line 4374 "src/sema.tr"
    return nt;
}

__attribute__((hot)) void Sema_check_class_bounds(Sema* self, TrStr cls_name, List_ptr* arg_tys) {
#line 4380 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
#line 4380 "src/sema.tr"
        return;
    }
#line 4381 "src/sema.tr"
    ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
#line 4382 "src/sema.tr"
    if ((cd->constraints->len == 0LL)) {
#line 4382 "src/sema.tr"
        return;
    }
#line 4383 "src/sema.tr"
    long long ci = 0LL;
#line 4384 "src/sema.tr"
    while ((ci < cd->constraints->len)) {
#line 4385 "src/sema.tr"
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(cd->constraints, ci));
#line 4386 "src/sema.tr"
        long long gpi = 0LL;
#line 4387 "src/sema.tr"
        while ((gpi < cd->generics->len)) {
#line 4388 "src/sema.tr"
            if ((({ __auto_type _ord_t1348 = (List_TrStr_get(cd->generics, gpi)); __auto_type _wr = (({ TrStr _cmpa_t1349 = (_ord_t1348); TrStr _cmpb_t1350 = (gc->target); _Bool _cmpr_t1351 = _tr_str_eqv(_cmpa_t1349, _cmpb_t1350); _tr_str_release(_cmpa_t1349); _cmpr_t1351; })); _wr; }) && (gpi < arg_tys->len))) {
#line 4389 "src/sema.tr"
                TrStr concrete_n = _tr_str_retain((*((AstType**)List_ptr_get(arg_tys, gpi)))->name);
#line 4390 "src/sema.tr"
                long long bi = 0LL;
#line 4391 "src/sema.tr"
                while ((bi < gc->bounds->len)) {
#line 4392 "src/sema.tr"
                    TrStr iface_n = _tr_str_retain((*((AstType**)List_ptr_get(gc->bounds, bi)))->name);
#line 4393 "src/sema.tr"
                    if ((!Sema__type_satisfies_bound(self, concrete_n, iface_n))) {
#line 4394 "src/sema.tr"
                        ({ TrStr _at_t1352 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[G-1] Type '", 12LL)), (concrete_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' does not satisfy the bound '", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' required by type parameter '", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gc->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of class '", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cls_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: implement all methods of interface '", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' on '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (concrete_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or add 'implements ", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1352); _tr_str_release(_at_t1352); });
                    }
#line 4395 "src/sema.tr"
                    bi = (bi + 1LL);
                    _tr_str_release(iface_n);
                }
            }
#line 4396 "src/sema.tr"
            gpi = (gpi + 1LL);
        }
#line 4397 "src/sema.tr"
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) void Sema__check_ty_arity(Sema* self, AstType** tp, TrStr where_, long long line) {
#line 4404 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(tp)))) {
#line 4404 "src/sema.tr"
        return;
    }
#line 4405 "src/sema.tr"
    AstType* t = (*tp);
#line 4406 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(t->args)))) {
#line 4406 "src/sema.tr"
        return;
    }
#line 4407 "src/sema.tr"
    if ((t->args->len == 0LL)) {
#line 4407 "src/sema.tr"
        return;
    }
#line 4408 "src/sema.tr"
    long long expected = (-1LL);
#line 4413 "src/sema.tr"
    if (((((_tr_str_eqv((t->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Set", 3LL))))) {
#line 4414 "src/sema.tr"
        expected = (-1LL);
    } else if (_tr_dict_contains(self->classes, _tr_strz(t->name))) {
#line 4416 "src/sema.tr"
        expected = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(t->name)))->generics->len;
    } else if (_tr_dict_contains(self->enums, _tr_strz(t->name))) {
#line 4418 "src/sema.tr"
        expected = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(t->name)))->generics->len;
    }
#line 4419 "src/sema.tr"
    if (((expected > 0LL) && (t->args->len != expected))) {
#line 4420 "src/sema.tr"
        long long saved_line = self->current_line;
#line 4421 "src/sema.tr"
        if ((line > 0LL)) {
#line 4421 "src/sema.tr"
            self->current_line = line;
        }
#line 4422 "src/sema.tr"
        ({ __auto_type _ord_t1353 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[G-2] wrong number of type arguments for '", 42LL)), (t->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': expected ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1354 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1353); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(expected)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", found ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1355 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1354); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(t->args->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (in ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (where_)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").\n      FIX: write '", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1356 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1355); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' with exactly ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1357 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1356); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(expected)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" type argument(s).", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1357); _tr_str_release(_at_t1357); });
#line 4423 "src/sema.tr"
        self->current_line = saved_line;
    }
#line 4424 "src/sema.tr"
    long long ai = 0LL;
#line 4425 "src/sema.tr"
    while ((ai < t->args->len)) {
#line 4426 "src/sema.tr"
        Sema__check_ty_arity(self, ((AstType**)List_ptr_get(t->args, ai)), where_, line);
#line 4427 "src/sema.tr"
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) void Sema__check_fn_arities(Sema* self, FunctionDef* f, TrStr owner) {
#line 4430 "src/sema.tr"
    TrStr fn_where = _tr_strx_concatv((owner), (f->name));
#line 4431 "src/sema.tr"
    long long pi = 0LL;
#line 4432 "src/sema.tr"
    while ((pi < f->params->len)) {
#line 4433 "src/sema.tr"
        ({ __auto_type _ord_t1358 = (((Param*)List_ptr_get(f->params, pi))->ty); __auto_type _ord_t1359 = (f->line); Sema__check_ty_arity(self, _ord_t1358, fn_where, _ord_t1359); });
#line 4434 "src/sema.tr"
        pi = (pi + 1LL);
    }
#line 4435 "src/sema.tr"
    Sema__check_ty_arity(self, f->ret_ty, fn_where, f->line);
    _tr_str_release(fn_where);
}

__attribute__((hot)) void Sema_check_type_arities(Sema* self, Program* prog) {
#line 4438 "src/sema.tr"
    long long di = 0LL;
#line 4439 "src/sema.tr"
    while ((di < prog->decls->len)) {
#line 4440 "src/sema.tr"
        __auto_type _t1360 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t1360.tag == Decl_DFunction) {
            __auto_type f = _t1360.data.DFunction.func;
#line 4442 "src/sema.tr"
            Sema__check_fn_arities(self, f, _tr_str_lit_len("", 0LL));
        } else if (_t1360.tag == Decl_DClass) {
            __auto_type c = _t1360.data.DClass.cls;
#line 4444 "src/sema.tr"
            long long cfi = 0LL;
#line 4445 "src/sema.tr"
            while ((cfi < c->fields->len)) {
#line 4446 "src/sema.tr"
                ({ __auto_type _ord_t1361 = (((FieldDef*)List_ptr_get(c->fields, cfi))->ty); __auto_type _ord_t1362 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("field ", 6LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1363 = (({ TrStr _tr_cl = (_ord_t1362); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((FieldDef*)List_ptr_get(c->fields, cfi))->name)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1364 = (c->line); Sema__check_ty_arity(self, _ord_t1361, _ord_t1363, _ord_t1364); _tr_str_release(_ord_t1363); });
#line 4447 "src/sema.tr"
                cfi = (cfi + 1LL);
            }
#line 4448 "src/sema.tr"
            long long cmi = 0LL;
#line 4449 "src/sema.tr"
            while ((cmi < c->methods->len)) {
#line 4450 "src/sema.tr"
                ({ __auto_type _ord_t1365 = (((FunctionDef*)List_ptr_get(c->methods, cmi))); TrStr _ord_t1366 = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); Sema__check_fn_arities(self, _ord_t1365, _ord_t1366); _tr_str_release(_ord_t1366); });
#line 4451 "src/sema.tr"
                cmi = (cmi + 1LL);
            }
        } else if (_t1360.tag == Decl_DExtend) {
            __auto_type target = _t1360.data.DExtend.target;
__auto_type methods = _t1360.data.DExtend.methods;
#line 4453 "src/sema.tr"
            long long emi = 0LL;
#line 4454 "src/sema.tr"
            while ((emi < methods->len)) {
#line 4455 "src/sema.tr"
                ({ __auto_type _ord_t1368 = (((FunctionDef*)List_ptr_get(methods, emi))); TrStr _at_t1367 = (_tr_strx_concatv((target), (_tr_str_lit_len(".", 1LL)))); Sema__check_fn_arities(self, _ord_t1368, _at_t1367); _tr_str_release(_at_t1367); });
#line 4456 "src/sema.tr"
                emi = (emi + 1LL);
            }
        } else if (_t1360.tag == Decl_DEnum) {
            __auto_type e = _t1360.data.DEnum.enm;
#line 4458 "src/sema.tr"
            long long vi = 0LL;
#line 4459 "src/sema.tr"
            while ((vi < e->variants->len)) {
#line 4460 "src/sema.tr"
                long long vfi = 0LL;
#line 4461 "src/sema.tr"
                while ((vfi < ((VariantDef*)List_ptr_get(e->variants, vi))->fields->len)) {
#line 4462 "src/sema.tr"
                    ({ __auto_type _ord_t1369 = (((Param*)List_ptr_get(((VariantDef*)List_ptr_get(e->variants, vi))->fields, vfi))->ty); __auto_type _ord_t1370 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("enum ", 5LL)), (e->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1371 = (({ TrStr _tr_cl = (_ord_t1370); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((VariantDef*)List_ptr_get(e->variants, vi))->name)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1372 = (e->line); Sema__check_ty_arity(self, _ord_t1369, _ord_t1371, _ord_t1372); _tr_str_release(_ord_t1371); });
#line 4463 "src/sema.tr"
                    vfi = (vfi + 1LL);
                }
#line 4464 "src/sema.tr"
                vi = (vi + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t1360;
            /* pass */
        }
#line 4466 "src/sema.tr"
        di = (di + 1LL);
    }
}

__attribute__((hot)) void Sema_register_decl(Sema* self, Decl* d) {
#line 4469 "src/sema.tr"
    __auto_type _t1373 = (*d);
    if (_t1373.tag == Decl_DFunction) {
        __auto_type f = _t1373.data.DFunction.func;
#line 4471 "src/sema.tr"
        AstType** _f_ret = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 4472 "src/sema.tr"
        if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4472 "src/sema.tr"
            _f_ret = f->ret_ty;
        }
#line 4476 "src/sema.tr"
        if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4477 "src/sema.tr"
            AstType* _frt = (*f->ret_ty);
#line 4482 "src/sema.tr"
            if ((_frt->from_regions->len == 1LL)) {
#line 4483 "src/sema.tr"
                long long _fi = 0LL;
#line 4484 "src/sema.tr"
                while ((_fi < f->params->len)) {
#line 4485 "src/sema.tr"
                    if (({ __auto_type _ord_t1374 = (((Param*)List_ptr_get(f->params, _fi))->name); __auto_type _wr = (({ TrStr _cmpa_t1375 = (_ord_t1374); TrStr _cmpb_t1376 = (List_TrStr_get(_frt->from_regions, 0LL)); _Bool _cmpr_t1377 = _tr_str_eqv(_cmpa_t1375, _cmpb_t1376); _tr_str_release(_cmpb_t1376); _cmpr_t1377; })); _wr; })) {
#line 4486 "src/sema.tr"
                        _frt->from_index = _fi;
                    }
#line 4487 "src/sema.tr"
                    _fi = (_fi + 1LL);
                }
#line 4488 "src/sema.tr"
                _f_ret = box_asttype(_frt);
            }
        }
#line 4492 "src/sema.tr"
        AstType** _decl_ret = _f_ret;
#line 4493 "src/sema.tr"
        if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 4494 "src/sema.tr"
            if ((!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 4495 "src/sema.tr"
                AstType* _result_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 4496 "src/sema.tr"
                List_ptr_append(_result_ty->args, _f_ret);
#line 4497 "src/sema.tr"
                List_ptr_append(_result_ty->args, f->throws_ty);
#line 4498 "src/sema.tr"
                _decl_ret = box_asttype(_result_ty);
            }
        }
#line 4499 "src/sema.tr"
        Sema_declare(self, f->name, SymbolKind_make_SFunction(), _decl_ret, false);
#line 4503 "src/sema.tr"
        AstType* _fnty = AstType_init(_tr_str_lit_len("def", 3LL));
#line 4504 "src/sema.tr"
        long long _pi = 0LL;
#line 4505 "src/sema.tr"
        while ((_pi < f->params->len)) {
#line 4506 "src/sema.tr"
            ({ __auto_type _ord_t1378 = (((Param*)List_ptr_get(f->params, _pi))->ty); List_ptr_append(_fnty->args, _ord_t1378); });
#line 4507 "src/sema.tr"
            _pi = (_pi + 1LL);
        }
#line 4510 "src/sema.tr"
        if (((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 4511 "src/sema.tr"
            ({ __auto_type _ord_t1379 = (box_asttype(throws_result_type((*_f_ret), f->throws_ty))); List_ptr_append(_fnty->args, _ord_t1379); });
        } else {
#line 4513 "src/sema.tr"
            List_ptr_append(_fnty->args, _f_ret);
        }
#line 4514 "src/sema.tr"
        _tr_dict_set(self->fn_sigs, _tr_strz(f->name), _fnty);
#line 4515 "src/sema.tr"
        _tr_dict_set(self->fn_defs, _tr_strz(f->name), _tr_obj_retain(f));
#line 4518 "src/sema.tr"
        if ((f->params->len > 0LL)) {
#line 4519 "src/sema.tr"
            Param* _vp = ((Param*)List_ptr_get(f->params, (f->params->len - 1LL)));
#line 4520 "src/sema.tr"
            if (_vp->is_variadic) {
#line 4521 "src/sema.tr"
                AstType* _velem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 4522 "src/sema.tr"
                if ((((unsigned long long)(_vp->ty)) != ((unsigned long long)(0LL)))) {
#line 4522 "src/sema.tr"
                    _velem_ty = (*_vp->ty);
                }
#line 4523 "src/sema.tr"
                ({ __auto_type _ord_t1380 = (f->name); TrStr _ord_t1381 = (_tr_str_wrap(_tr_int_to_str((long long)((f->params->len - 1LL))))); _tr_dict_set(self->variadic_fns, _tr_strz(_ord_t1380), _tr_str_box(_tr_str_retain(_ord_t1381))); _tr_str_release(_ord_t1381); });
#line 4524 "src/sema.tr"
                ({ __auto_type _ord_t1382 = (f->name); __auto_type _ord_t1383 = (box_asttype(_velem_ty)); _tr_dict_set(self->variadic_elem_ty, _tr_strz(_ord_t1382), _ord_t1383); });
            }
        }
    } else if (_t1373.tag == Decl_DClass) {
        __auto_type c = _t1373.data.DClass.cls;
#line 4526 "src/sema.tr"
        ({ __auto_type _ord_t1384 = (c->name); __auto_type _ord_t1385 = (SymbolKind_make_SClass()); __auto_type _ord_t1386 = (box_asttype(AstType_init(c->name))); Sema_declare(self, _ord_t1384, _ord_t1385, _ord_t1386, false); });
#line 4527 "src/sema.tr"
        if ((_tr_dict_contains(self->classes, _tr_strz(c->name)) && (((unsigned long long)(((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(c->name))))) != ((unsigned long long)(c))))) {
#line 4527 "src/sema.tr"
            _tr_dict_set(self->ambiguous_classes, _tr_strz(c->name), true);
        }
#line 4528 "src/sema.tr"
        _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
#line 4530 "src/sema.tr"
        if (Sema_has_copy_decorator(self, c->decorators)) {
#line 4530 "src/sema.tr"
            _tr_dict_set(self->copy_classes, _tr_strz(c->name), true);
        }
    } else if (_t1373.tag == Decl_DActor) {
        __auto_type c = _t1373.data.DActor.cls;
#line 4532 "src/sema.tr"
        ({ __auto_type _ord_t1387 = (c->name); __auto_type _ord_t1388 = (SymbolKind_make_SClass()); __auto_type _ord_t1389 = (box_asttype(AstType_init(c->name))); Sema_declare(self, _ord_t1387, _ord_t1388, _ord_t1389, false); });
#line 4533 "src/sema.tr"
        _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
#line 4534 "src/sema.tr"
        if (Sema_has_copy_decorator(self, c->decorators)) {
#line 4534 "src/sema.tr"
            _tr_dict_set(self->copy_classes, _tr_strz(c->name), true);
        }
    } else if (_t1373.tag == Decl_DEnum) {
        __auto_type e = _t1373.data.DEnum.enm;
#line 4536 "src/sema.tr"
        ({ __auto_type _ord_t1390 = (e->name); __auto_type _ord_t1391 = (SymbolKind_make_SEnum()); __auto_type _ord_t1392 = (box_asttype(AstType_init(e->name))); Sema_declare(self, _ord_t1390, _ord_t1391, _ord_t1392, false); });
#line 4537 "src/sema.tr"
        _tr_dict_set(self->enums, _tr_strz(e->name), _tr_obj_retain(e));
#line 4538 "src/sema.tr"
        if (Sema_has_copy_decorator(self, e->decorators)) {
#line 4538 "src/sema.tr"
            _tr_dict_set(self->copy_classes, _tr_strz(e->name), true);
        }
    } else if (_t1373.tag == Decl_DInterface) {
        __auto_type i = _t1373.data.DInterface.iface;
#line 4540 "src/sema.tr"
        ({ __auto_type _ord_t1393 = (i->name); __auto_type _ord_t1394 = (SymbolKind_make_SInterface()); __auto_type _ord_t1395 = (box_asttype(AstType_init(i->name))); Sema_declare(self, _ord_t1393, _ord_t1394, _ord_t1395, false); });
#line 4541 "src/sema.tr"
        _tr_dict_set(self->interfaces, _tr_strz(i->name), _tr_obj_retain(i));
#line 4542 "src/sema.tr"
        if (Sema_has_copy_decorator(self, i->decorators)) {
#line 4542 "src/sema.tr"
            _tr_dict_set(self->copy_classes, _tr_strz(i->name), true);
        }
    } else if (_t1373.tag == Decl_DExtend) {
        __auto_type target = _t1373.data.DExtend.target;
__auto_type methods = _t1373.data.DExtend.methods;
#line 4544 "src/sema.tr"
        long long hi = 0LL;
#line 4545 "src/sema.tr"
        while ((hi < methods->len)) {
#line 4546 "src/sema.tr"
            FunctionDef* f = ((FunctionDef*)List_ptr_get(methods, hi));
#line 4547 "src/sema.tr"
            AstType** _m_ret = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 4548 "src/sema.tr"
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4548 "src/sema.tr"
                _m_ret = f->ret_ty;
            }
#line 4551 "src/sema.tr"
            TrStr _decl_key = ({ TrStr _tr_cl = (_tr_strx_concatv((target), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; });
#line 4552 "src/sema.tr"
            if (_tr_dict_contains(self->globals, _tr_strz(_decl_key))) {
#line 4554 "src/sema.tr"
                long long _pc = 0LL;
#line 4555 "src/sema.tr"
                long long _pci = 0LL;
#line 4556 "src/sema.tr"
                while ((_pci < f->params->len)) {
#line 4557 "src/sema.tr"
                    if ((!_tr_str_eqv((((Param*)List_ptr_get(f->params, _pci))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4557 "src/sema.tr"
                        _pc = (_pc + 1LL);
                    }
#line 4558 "src/sema.tr"
                    _pci = (_pci + 1LL);
                }
#line 4559 "src/sema.tr"
                ({ TrStr _ap_t1396 = (_tr_str_lit_len("_", 1LL)); TrStr _ap_t1397 = (_tr_str_wrap(_tr_int_to_str((long long)(_pc)))); TrStr _ap_t1398 = (_tr_str_lit_len("arg", 3LL)); _tr_str_append(&_decl_key, _ap_t1396); _tr_str_append(&_decl_key, _ap_t1397); _tr_str_append(&_decl_key, _ap_t1398); _tr_str_release(_ap_t1397); });
            }
#line 4560 "src/sema.tr"
            Sema_declare(self, _decl_key, SymbolKind_make_SFunction(), _m_ret, false);
#line 4561 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(target))) {
#line 4562 "src/sema.tr"
                List_ptr_append(((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(target)))->methods, _tr_obj_retain(f));
            }
#line 4563 "src/sema.tr"
            hi = (hi + 1LL);
            _tr_str_release(_decl_key);
        }
    } else if (_t1373.tag == Decl_DExtern) {
        __auto_type abi = _t1373.data.DExtern.abi;
__auto_type functions = _t1373.data.DExtern.functions;
#line 4565 "src/sema.tr"
        long long ei = 0LL;
#line 4566 "src/sema.tr"
        while ((ei < functions->len)) {
#line 4567 "src/sema.tr"
            FunctionDef* f = ((FunctionDef*)List_ptr_get(functions, ei));
#line 4568 "src/sema.tr"
            AstType** _e_ret = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 4569 "src/sema.tr"
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4569 "src/sema.tr"
                _e_ret = f->ret_ty;
            }
#line 4570 "src/sema.tr"
            Sema_declare(self, f->name, SymbolKind_make_SFunction(), _e_ret, false);
#line 4571 "src/sema.tr"
            _tr_dict_set(self->extern_names, _tr_strz(f->name), true);
#line 4572 "src/sema.tr"
            _tr_dict_set(self->extern_defs, _tr_strz(f->name), _tr_obj_retain(f));
#line 4573 "src/sema.tr"
            ei = (ei + 1LL);
        }
    } else if (_t1373.tag == Decl_DDecoratorDef) {
        __auto_type f = _t1373.data.DDecoratorDef.func;
#line 4575 "src/sema.tr"
        _tr_dict_set(self->decorator_names, _tr_strz(f->name), true);
#line 4576 "src/sema.tr"
        ({ __auto_type _ord_t1399 = (f->name); __auto_type _ord_t1400 = (SymbolKind_make_SFunction()); __auto_type _ord_t1401 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); Sema_declare(self, _ord_t1399, _ord_t1400, _ord_t1401, false); });
    } else if (_t1373.tag == Decl_DTypeAlias) {
        __auto_type alias_name = _t1373.data.DTypeAlias.name;
__auto_type target_ty = _t1373.data.DTypeAlias.target;
#line 4580 "src/sema.tr"
        AstType** alias_ty = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 4581 "src/sema.tr"
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
#line 4581 "src/sema.tr"
            alias_ty = target_ty;
        }
#line 4582 "src/sema.tr"
        Sema_declare(self, alias_name, SymbolKind_make_SClass(), alias_ty, false);
#line 4584 "src/sema.tr"
        TrStr resolved_name = _tr_str_lit_len("", 0LL);
#line 4585 "src/sema.tr"
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
#line 4585 "src/sema.tr"
            TrStr _strtmp_t1402 = _tr_str_retain((*target_ty)->name);
            _tr_str_release(resolved_name);
            resolved_name = _strtmp_t1402;
        }
#line 4586 "src/sema.tr"
        if ((_tr_str_lenv((resolved_name)) > 0LL)) {
#line 4587 "src/sema.tr"
            _tr_dict_set(self->type_aliases, _tr_strz(alias_name), _tr_str_box(_tr_str_retain(resolved_name)));
#line 4589 "src/sema.tr"
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
#line 4590 "src/sema.tr"
                AstType* ta_t = (*target_ty);
#line 4591 "src/sema.tr"
                if ((ta_t->args->len > 0LL)) {
#line 4592 "src/sema.tr"
                    ({ __auto_type _ord_t1403 = ((*((AstType**)List_ptr_get(ta_t->args, 0LL)))->name); _tr_dict_set(self->type_alias_elem, _tr_strz(alias_name), _tr_str_box(_tr_str_retain(_ord_t1403))); });
                }
            }
        }
        _tr_str_release(resolved_name);
    } else if (1) {
        __auto_type _ = _t1373;
        /* pass */
    }
}

__attribute__((hot)) HirFunction* Sema_lower_func(Sema* self, FunctionDef* f) {
#line 4596 "src/sema.tr"
    self->current_line = f->line;
#line 4597 "src/sema.tr"
    bool _saved_fn_is_lib = self->cur_fn_is_lib;
#line 4598 "src/sema.tr"
    self->cur_fn_is_lib = f->is_lib;
#line 4602 "src/sema.tr"
    if ((((!_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL)))) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("", 0LL))))) && (_tr_strlen(_tr_strz(f->name)) > 1LL))) {
#line 4603 "src/sema.tr"
        TrStr fn_cat = Sema_is_reserved_error(self, f->name);
#line 4604 "src/sema.tr"
        if ((!_tr_str_eqv((fn_cat), (_tr_str_lit_len("", 0LL))))) {
#line 4605 "src/sema.tr"
            ({ TrStr _at_t1404 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fn_cat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and cannot be used as a function name.", 39LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1404); _tr_str_release(_at_t1404); });
        } else if (_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))) {
#line 4607 "src/sema.tr"
            TrStr fn_kcat = Sema_is_reserved_keyword(self, f->name);
#line 4608 "src/sema.tr"
            if ((!_tr_str_eqv((fn_kcat), (_tr_str_lit_len("", 0LL))))) {
#line 4609 "src/sema.tr"
                ({ TrStr _at_t1405 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fn_kcat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and is reserved. Choose a different function name.", 51LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1405); _tr_str_release(_at_t1405); });
            }
        }
    }
#line 4611 "src/sema.tr"
    bool saved_async = self->in_async_fn;
#line 4612 "src/sema.tr"
    self->in_async_fn = f->is_async;
#line 4613 "src/sema.tr"
    bool saved_try_ok = self->fn_try_ok;
#line 4614 "src/sema.tr"
    self->fn_try_ok = false;
#line 4615 "src/sema.tr"
    if (((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 4615 "src/sema.tr"
        self->fn_try_ok = true;
    }
#line 4616 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4617 "src/sema.tr"
        TrStr _rtn = _tr_str_retain((*f->ret_ty)->name);
#line 4618 "src/sema.tr"
        if ((_tr_str_eqv((_rtn), (_tr_str_lit_len("Result", 6LL))) || _tr_str_eqv((_rtn), (_tr_str_lit_len("Option", 6LL))))) {
#line 4618 "src/sema.tr"
            self->fn_try_ok = true;
        }
        _tr_str_release(_rtn);
    }
#line 4619 "src/sema.tr"
    self->current_func_name = _tr_str_retain(f->name);
#line 4620 "src/sema.tr"
    List_TrStr* saved_func_generics = self->current_func_generics;
#line 4621 "src/sema.tr"
    self->current_func_generics = f->generics;
#line 4622 "src/sema.tr"
    List_ptr* saved_func_constraints = self->current_func_constraints;
#line 4623 "src/sema.tr"
    self->current_func_constraints = f->constraints;
#line 4625 "src/sema.tr"
    self->container_borrows = _tr_dict_new(16LL);
#line 4627 "src/sema.tr"
    TrStr saved_ret_from = _tr_str_retain(self->current_func_ret_from);
#line 4628 "src/sema.tr"
    bool saved_ret_borrow_str = self->current_func_ret_borrow_str;
#line 4629 "src/sema.tr"
    TrStr saved_ret_family = _tr_str_retain(self->current_func_ret_family);
#line 4630 "src/sema.tr"
    self->current_func_ret_family = _tr_str_lit_len("", 0LL);
#line 4631 "src/sema.tr"
    if (((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))) && (!f->is_extern))) {
#line 4631 "src/sema.tr"
        ({ __auto_type _ord_t1407 = (f->ret_ty); TrStr _at_t1406 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the return type of '", 20LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_type_known(self, _ord_t1407, _at_t1406); _tr_str_release(_at_t1406); });
    }
#line 4632 "src/sema.tr"
    if (((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))) && (f->generics->len == 0LL))) {
#line 4633 "src/sema.tr"
        self->current_func_ret_family = Sema_decl_family(self, (*f->ret_ty));
    }
#line 4634 "src/sema.tr"
    List_TrStr* saved_ret_regions = self->current_func_ret_regions;
#line 4635 "src/sema.tr"
    List_TrStr* saved_outlives_a = self->current_func_outlives_a;
#line 4636 "src/sema.tr"
    List_TrStr* saved_outlives_b = self->current_func_outlives_b;
#line 4637 "src/sema.tr"
    self->current_func_ret_from = _tr_str_lit_len("", 0LL);
#line 4638 "src/sema.tr"
    self->current_func_ret_borrow_str = false;
#line 4639 "src/sema.tr"
    self->current_func_ret_regions = (void*)List_TrStr_new();
#line 4640 "src/sema.tr"
    self->current_func_outlives_a = f->outlives_a;
#line 4641 "src/sema.tr"
    self->current_func_outlives_b = f->outlives_b;
#line 4643 "src/sema.tr"
    if ((self->strict_mode && (f->outlives_a->len > 0LL))) {
#line 4644 "src/sema.tr"
        long long wbi = 0LL;
#line 4645 "src/sema.tr"
        while ((wbi < f->outlives_a->len)) {
#line 4646 "src/sema.tr"
            bool w_ok_a = false;
#line 4647 "src/sema.tr"
            bool w_ok_b = false;
#line 4648 "src/sema.tr"
            long long wpi = 0LL;
#line 4649 "src/sema.tr"
            while ((wpi < f->params->len)) {
#line 4650 "src/sema.tr"
                if (({ __auto_type _ord_t1408 = (((Param*)List_ptr_get(f->params, wpi))->name); __auto_type _wr = (({ TrStr _cmpa_t1409 = (_ord_t1408); TrStr _cmpb_t1410 = (List_TrStr_get(f->outlives_a, wbi)); _Bool _cmpr_t1411 = _tr_str_eqv(_cmpa_t1409, _cmpb_t1410); _tr_str_release(_cmpb_t1410); _cmpr_t1411; })); _wr; })) {
#line 4650 "src/sema.tr"
                    w_ok_a = true;
                }
#line 4651 "src/sema.tr"
                if (({ __auto_type _ord_t1412 = (((Param*)List_ptr_get(f->params, wpi))->name); __auto_type _wr = (({ TrStr _cmpa_t1413 = (_ord_t1412); TrStr _cmpb_t1414 = (List_TrStr_get(f->outlives_b, wbi)); _Bool _cmpr_t1415 = _tr_str_eqv(_cmpa_t1413, _cmpb_t1414); _tr_str_release(_cmpb_t1414); _cmpr_t1415; })); _wr; })) {
#line 4651 "src/sema.tr"
                    w_ok_b = true;
                }
#line 4652 "src/sema.tr"
                wpi = (wpi + 1LL);
            }
#line 4653 "src/sema.tr"
            if ((!w_ok_a)) {
#line 4654 "src/sema.tr"
                ({ TrStr _at_t1416 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(f->outlives_a, wbi)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[L-2] region '", 14LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in the `where ... outlives ...` clause is not a parameter of this function.", 77LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1416); _tr_str_release(_at_t1416); });
            }
#line 4655 "src/sema.tr"
            if ((!w_ok_b)) {
#line 4656 "src/sema.tr"
                ({ TrStr _at_t1417 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(f->outlives_b, wbi)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[L-2] region '", 14LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in the `where ... outlives ...` clause is not a parameter of this function.", 77LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1417); _tr_str_release(_at_t1417); });
            }
#line 4657 "src/sema.tr"
            wbi = (wbi + 1LL);
        }
    }
#line 4658 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4659 "src/sema.tr"
        self->current_func_ret_from = _tr_str_retain((*f->ret_ty)->from_param);
#line 4660 "src/sema.tr"
        self->current_func_ret_regions = (*f->ret_ty)->from_regions;
#line 4661 "src/sema.tr"
        if (((*f->ret_ty)->is_borrow && _tr_str_eqv(((*f->ret_ty)->name), (_tr_str_lit_len("str", 3LL))))) {
#line 4662 "src/sema.tr"
            self->current_func_ret_borrow_str = true;
        }
    }
#line 4666 "src/sema.tr"
    if ((_tr_str_eqv((self->current_func_ret_from), (_tr_str_lit_len("", 0LL))) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
#line 4667 "src/sema.tr"
        TrStr infer_ret_nm = _tr_str_retain((*f->ret_ty)->name);
#line 4668 "src/sema.tr"
        bool infer_is_borrow = (*f->ret_ty)->is_borrow;
#line 4669 "src/sema.tr"
        if ((((_tr_str_eqv((infer_ret_nm), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((infer_ret_nm), (_tr_str_lit_len("ref", 3LL)))) || _tr_str_eqv((infer_ret_nm), (_tr_str_lit_len("mut_ref", 7LL)))) || infer_is_borrow)) {
#line 4670 "src/sema.tr"
            TrStr infer_from = _tr_str_lit_len("", 0LL);
#line 4671 "src/sema.tr"
            long long infer_count = 0LL;
#line 4672 "src/sema.tr"
            long long infer_i = 0LL;
#line 4673 "src/sema.tr"
            while ((infer_i < f->params->len)) {
#line 4674 "src/sema.tr"
                Param* infer_p = ((Param*)List_ptr_get(f->params, infer_i));
#line 4675 "src/sema.tr"
                if (((!_tr_str_eqv((infer_p->name), (_tr_str_lit_len("self", 4LL)))) && (((unsigned long long)(infer_p->ty)) != ((unsigned long long)(0LL))))) {
#line 4676 "src/sema.tr"
                    AstType* infer_pty = (*infer_p->ty);
#line 4677 "src/sema.tr"
                    if ((!Sema_is_primitive_name(self, infer_pty->name))) {
#line 4678 "src/sema.tr"
                        TrStr _strtmp_t1418 = _tr_str_retain(infer_p->name);
                        _tr_str_release(infer_from);
                        infer_from = _strtmp_t1418;
#line 4679 "src/sema.tr"
                        infer_count = (infer_count + 1LL);
                    }
                }
#line 4680 "src/sema.tr"
                infer_i = (infer_i + 1LL);
            }
#line 4681 "src/sema.tr"
            if ((infer_count == 1LL)) {
#line 4682 "src/sema.tr"
                self->current_func_ret_from = _tr_str_retain(infer_from);
            }
        }
        _tr_str_release(infer_ret_nm);
    }
#line 4687 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4688 "src/sema.tr"
        List_TrStr* rregs = (*f->ret_ty)->from_regions;
#line 4689 "src/sema.tr"
        long long rri = 0LL;
#line 4690 "src/sema.tr"
        while ((rri < rregs->len)) {
#line 4691 "src/sema.tr"
            TrStr rnm = List_TrStr_get(rregs, rri);
#line 4692 "src/sema.tr"
            bool rok = false;
#line 4693 "src/sema.tr"
            long long rpi = 0LL;
#line 4694 "src/sema.tr"
            while ((rpi < f->params->len)) {
#line 4695 "src/sema.tr"
                if (_tr_str_eqv((((Param*)List_ptr_get(f->params, rpi))->name), (rnm))) {
#line 4696 "src/sema.tr"
                    rok = true;
                }
#line 4697 "src/sema.tr"
                rpi = (rpi + 1LL);
            }
#line 4698 "src/sema.tr"
            if ((!rok)) {
#line 4700 "src/sema.tr"
                long long rci = 0LL;
#line 4701 "src/sema.tr"
                while ((rci < self->current_region_params->len)) {
#line 4702 "src/sema.tr"
                    if (({ TrStr _cmpa_t1419 = (List_TrStr_get(self->current_region_params, rci)); TrStr _cmpb_t1420 = (rnm); _Bool _cmpr_t1421 = _tr_str_eqv(_cmpa_t1419, _cmpb_t1420); _tr_str_release(_cmpa_t1419); _cmpr_t1421; })) {
#line 4703 "src/sema.tr"
                        rok = true;
                    }
#line 4704 "src/sema.tr"
                    rci = (rci + 1LL);
                }
            }
#line 4705 "src/sema.tr"
            if ((!rok)) {
#line 4706 "src/sema.tr"
                ({ TrStr _at_t1422 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-2] region source '", 21LL)), (rnm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' named in 'from' is not a parameter or a region parameter of the enclosing type.\n      FIX: name a parameter the borrow comes from, or declare 'class/enum/interface <T> from ", 175LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rnm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":'.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1422); _tr_str_release(_at_t1422); });
            }
#line 4707 "src/sema.tr"
            rri = (rri + 1LL);
            _tr_str_release(rnm);
        }
    }
#line 4708 "src/sema.tr"
    Sema_enter_scope(self);
#line 4710 "src/sema.tr"
    List_ptr* hparams = (void*)List_ptr_new();
#line 4711 "src/sema.tr"
    long long j = 0LL;
#line 4712 "src/sema.tr"
    while ((j < f->params->len)) {
#line 4713 "src/sema.tr"
        Param* p = ((Param*)List_ptr_get(f->params, j));
#line 4714 "src/sema.tr"
        AstType* p_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 4715 "src/sema.tr"
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
#line 4716 "src/sema.tr"
            p_ty = (*p->ty);
#line 4717 "src/sema.tr"
            if ((!f->is_extern)) {
#line 4717 "src/sema.tr"
                ({ __auto_type _ord_t1424 = (p->ty); TrStr _at_t1423 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("parameter '", 11LL)), (p->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_type_known(self, _ord_t1424, _at_t1423); _tr_str_release(_at_t1423); });
            }
        } else if ((_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))))) {
#line 4719 "src/sema.tr"
            p_ty = AstType_init(self->current_class_name);
        }
#line 4720 "src/sema.tr"
        if (p->is_variadic) {
#line 4723 "src/sema.tr"
            AstType* _vp_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 4724 "src/sema.tr"
            ({ __auto_type _ord_t1425 = (box_asttype(p_ty)); List_ptr_append(_vp_ty->args, _ord_t1425); });
#line 4725 "src/sema.tr"
            p_ty = _vp_ty;
        }
#line 4727 "src/sema.tr"
        ({ __auto_type _ord_t1426 = (p->name); __auto_type _ord_t1427 = (SymbolKind_make_SVariable()); __auto_type _ord_t1428 = (box_asttype(p_ty)); Sema_declare(self, _ord_t1426, _ord_t1427, _ord_t1428, false); });
#line 4729 "src/sema.tr"
        if ((self->scopes->len > 0LL)) {
#line 4730 "src/sema.tr"
            Scope* pb_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 4731 "src/sema.tr"
            if (_tr_dict_contains(pb_scope->variables, _tr_strz(p->name))) {
#line 4732 "src/sema.tr"
                Symbol* pb_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pb_scope->variables, _tr_strz(p->name)));
#line 4733 "src/sema.tr"
                pb_sym->is_param = true;
#line 4734 "src/sema.tr"
                pb_sym->ptr_region = 2LL;
#line 4735 "src/sema.tr"
                if ((((_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))))) && _tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) && ({ TrStr _cmpa_t1429 = (Sema_decl_family(self, AstType_init(self->current_class_name))); TrStr _cmpb_t1430 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t1431 = _tr_str_eqv(_cmpa_t1429, _cmpb_t1430); _tr_str_release(_cmpa_t1429); _cmpr_t1431; }))) {
#line 4736 "src/sema.tr"
                    pb_sym->firm = _tr_str_lit_len("class", 5LL);
#line 4737 "src/sema.tr"
                    pb_sym->firm_cls = _tr_str_retain(self->current_class_name);
                }
#line 4738 "src/sema.tr"
                if ((((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL))) && (!p->is_variadic)) && (f->generics->len == 0LL))) {
#line 4739 "src/sema.tr"
                    pb_sym->firm = Sema_decl_family(self, p_ty);
#line 4740 "src/sema.tr"
                    if ((_tr_str_eqv((pb_sym->firm), (_tr_str_lit_len("class", 5LL))) && _tr_dict_contains(self->classes, _tr_strz(p_ty->name)))) {
#line 4740 "src/sema.tr"
                        pb_sym->firm_cls = _tr_str_retain(p_ty->name);
                    }
                }
#line 4741 "src/sema.tr"
                _tr_dict_set(pb_scope->variables, _tr_strz(p->name), pb_sym);
            }
        }
#line 4743 "src/sema.tr"
        HirParam* hp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 4744 "src/sema.tr"
        hp->name = _tr_str_retain(p->name);
#line 4745 "src/sema.tr"
        hp->ty = p_ty;
#line 4746 "src/sema.tr"
        List_ptr_append(hparams, _tr_obj_retain(hp));
#line 4747 "src/sema.tr"
        j = (j + 1LL);
        _tr_obj_release(hp, _trdrop_HirParam);
    }
#line 4749 "src/sema.tr"
    HirFunction* hf = ((HirFunction*)_tr_obj_alloc(sizeof(HirFunction)));
#line 4750 "src/sema.tr"
    hf->name = _tr_str_retain(f->name);
#line 4751 "src/sema.tr"
    hf->class_name = _tr_str_retain(self->current_class_name);
#line 4752 "src/sema.tr"
    hf->generics = f->generics;
#line 4753 "src/sema.tr"
    hf->constraints = f->constraints;
#line 4754 "src/sema.tr"
    hf->params = hparams;
#line 4755 "src/sema.tr"
    hf->ret_ty = AstType_init(_tr_str_lit_len("None", 4LL));
#line 4756 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4756 "src/sema.tr"
        hf->ret_ty = (*f->ret_ty);
    }
#line 4757 "src/sema.tr"
    hf->throws_ty = AstType_init(_tr_str_lit_len("", 0LL));
#line 4758 "src/sema.tr"
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 4758 "src/sema.tr"
        hf->throws_ty = (*f->throws_ty);
    }
#line 4760 "src/sema.tr"
    self->cur_func_borrowers = (void*)List_TrStr_new();
#line 4761 "src/sema.tr"
    self->cur_func_sources = (void*)List_TrStr_new();
#line 4762 "src/sema.tr"
    TrMap* saved_alias_assigned = self->alias_assigned;
#line 4763 "src/sema.tr"
    self->alias_assigned = _tr_dict_new(8LL);
#line 4764 "src/sema.tr"
    _block_alias_assigns(f->body, self->alias_assigned);
#line 4765 "src/sema.tr"
    HirBlock* _cltmp_t1432 = Sema_lower_block(self, f->body);
    _tr_obj_release(hf->body, _trdrop_HirBlock);
    hf->body = _cltmp_t1432;
#line 4766 "src/sema.tr"
    self->alias_assigned = saved_alias_assigned;
#line 4767 "src/sema.tr"
    hf->borrow_borrowers = self->cur_func_borrowers;
#line 4768 "src/sema.tr"
    hf->borrow_sources = self->cur_func_sources;
#line 4770 "src/sema.tr"
    hf->proven_borrows = mir_proven_borrows(hf);
#line 4774 "src/sema.tr"
    if (self->strict_mode) {
#line 4775 "src/sema.tr"
        List_TrStr* _bconf = mir_borrow_conflicts(hf, self->mutating_methods);
#line 4776 "src/sema.tr"
        long long _bci = 0LL;
#line 4777 "src/sema.tr"
        while ((_bci < _bconf->len)) {
#line 4778 "src/sema.tr"
            ({ TrStr _at_t1433 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(_bconf, _bci)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[B-1] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      A place may have MANY shared `ref` borrows, OR exactly ONE exclusive `mut` borrow — never both. End one borrow's last use before the other begins.", 156LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1433); _tr_str_release(_at_t1433); });
#line 4779 "src/sema.tr"
            _bci = (_bci + 1LL);
        }
#line 4781 "src/sema.tr"
        List_TrStr* _b3 = mir_shared_ref_param_violations(hf, self->mutating_methods);
#line 4782 "src/sema.tr"
        long long _b3i = 0LL;
#line 4783 "src/sema.tr"
        while ((_b3i < _b3->len)) {
#line 4784 "src/sema.tr"
            ({ TrStr _at_t1434 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(_b3, _b3i)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[B-3] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      A shared `ref T` parameter cannot be mutated — declare it `mut ref T` for an exclusive (mutable) borrow.", 114LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1434); _tr_str_release(_at_t1434); });
#line 4785 "src/sema.tr"
            _b3i = (_b3i + 1LL);
        }
        List_TrStr_free(_bconf);
    }
#line 4786 "src/sema.tr"
    hf->decorators = f->decorators;
#line 4787 "src/sema.tr"
    hf->is_async = f->is_async;
#line 4788 "src/sema.tr"
    hf->is_extern = f->is_extern;
#line 4789 "src/sema.tr"
    hf->is_public = f->is_public;
#line 4790 "src/sema.tr"
    hf->is_export = f->is_export;
#line 4791 "src/sema.tr"
    bool _has_self = false;
#line 4792 "src/sema.tr"
    long long _si = 0LL;
#line 4793 "src/sema.tr"
    while ((_si < hparams->len)) {
#line 4794 "src/sema.tr"
        if (_tr_str_eqv((((HirParam*)List_ptr_get(hparams, _si))->name), (_tr_str_lit_len("self", 4LL)))) {
#line 4794 "src/sema.tr"
            _has_self = true;
        }
#line 4795 "src/sema.tr"
        _si = (_si + 1LL);
    }
#line 4796 "src/sema.tr"
    hf->is_static = ((!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))) && (!_has_self));
#line 4797 "src/sema.tr"
    hf->is_variadic = f->is_variadic;
#line 4798 "src/sema.tr"
    hf->is_decorator = false;
#line 4805 "src/sema.tr"
    bool _is_entry_main = (_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL))) && _tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))));
#line 4806 "src/sema.tr"
    if ((((!f->is_extern) && (f->body->stmts->len > 0LL)) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
#line 4807 "src/sema.tr"
        TrStr ret_nm = _tr_str_retain((*f->ret_ty)->name);
#line 4808 "src/sema.tr"
        if (((((((!_tr_str_eqv((ret_nm), (_tr_str_lit_len("void", 4LL)))) && (!_tr_str_eqv((ret_nm), (_tr_str_lit_len("None", 4LL))))) && (!_tr_str_eqv((ret_nm), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("init", 4LL))))) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("new", 3LL))))) && (!_is_entry_main))) {
#line 4809 "src/sema.tr"
            if ((!Sema_block_returns(self, f->body))) {
#line 4810 "src/sema.tr"
                self->current_line = f->line;
#line 4811 "src/sema.tr"
                ({ TrStr _at_t1435 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-3] Function '", 16LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' returns '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ret_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but is missing a return statement on at least one code path. FIX: Add a return at the end, or ensure all if/elif/else branches return.", 136LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1435); _tr_str_release(_at_t1435); });
            }
        }
    }
#line 4813 "src/sema.tr"
    Sema_finalize_scope_drops(self, hf->body);
#line 4818 "src/sema.tr"
    Sema_apply_mir_if_drops(self, hf);
#line 4819 "src/sema.tr"
    Sema_exit_scope(self);
#line 4821 "src/sema.tr"
    self->in_async_fn = saved_async;
#line 4822 "src/sema.tr"
    self->fn_try_ok = saved_try_ok;
#line 4823 "src/sema.tr"
    self->current_func_name = _tr_str_lit_len("", 0LL);
#line 4824 "src/sema.tr"
    self->current_func_generics = saved_func_generics;
#line 4825 "src/sema.tr"
    self->current_func_constraints = saved_func_constraints;
#line 4826 "src/sema.tr"
    self->current_func_ret_from = _tr_str_retain(saved_ret_from);
#line 4827 "src/sema.tr"
    self->current_func_ret_borrow_str = saved_ret_borrow_str;
#line 4828 "src/sema.tr"
    self->current_func_ret_family = _tr_str_retain(saved_ret_family);
#line 4829 "src/sema.tr"
    self->current_func_ret_regions = saved_ret_regions;
#line 4830 "src/sema.tr"
    self->current_func_outlives_a = saved_outlives_a;
#line 4831 "src/sema.tr"
    self->current_func_outlives_b = saved_outlives_b;
#line 4832 "src/sema.tr"
    self->cur_fn_is_lib = _saved_fn_is_lib;
#line 4833 "src/sema.tr"
    _tr_str_release(saved_ret_from);
    _tr_str_release(saved_ret_family);
    return hf;
}

__attribute__((hot)) HirClass* Sema_lower_class(Sema* self, ClassDef* c) {
#line 4836 "src/sema.tr"
    self->current_line = c->line;
#line 4837 "src/sema.tr"
    self->current_class_name = _tr_str_retain(c->name);
#line 4838 "src/sema.tr"
    self->current_region_params = c->region_params;
#line 4839 "src/sema.tr"
    List_ptr* hfields = (void*)List_ptr_new();
#line 4840 "src/sema.tr"
    long long i = 0LL;
#line 4841 "src/sema.tr"
    while ((i < c->fields->len)) {
#line 4842 "src/sema.tr"
        FieldDef* f = ((FieldDef*)List_ptr_get(c->fields, i));
#line 4843 "src/sema.tr"
        HirField* hf = ((HirField*)_tr_obj_alloc(sizeof(HirField)));
#line 4844 "src/sema.tr"
        hf->name = _tr_str_retain(f->name);
#line 4845 "src/sema.tr"
        AstType* f_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 4846 "src/sema.tr"
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
#line 4846 "src/sema.tr"
            f_ty = (*f->ty);
        }
#line 4847 "src/sema.tr"
        hf->ty = f_ty;
#line 4848 "src/sema.tr"
        List_ptr_append(hfields, _tr_obj_retain(hf));
#line 4849 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hf, _trdrop_HirField);
    }
#line 4852 "src/sema.tr"
    long long pf_i = 0LL;
#line 4853 "src/sema.tr"
    while ((pf_i < c->fields->len)) {
#line 4854 "src/sema.tr"
        FieldDef* pf_f = ((FieldDef*)List_ptr_get(c->fields, pf_i));
#line 4855 "src/sema.tr"
        if ((((unsigned long long)(pf_f->ty)) != ((unsigned long long)(0LL)))) {
#line 4856 "src/sema.tr"
            AstType* pf_ty = (*pf_f->ty);
#line 4857 "src/sema.tr"
            if ((_tr_str_eqv((pf_ty->name), (_tr_str_lit_len("Shared", 6LL))) && (pf_ty->args->len > 0LL))) {
#line 4858 "src/sema.tr"
                AstType* pf_inner = (*((AstType**)List_ptr_get(pf_ty->args, 0LL)));
#line 4859 "src/sema.tr"
                if (_tr_str_eqv((pf_inner->name), (c->name))) {
#line 4860 "src/sema.tr"
                    ({ TrStr _at_t1436 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[S-1] '", 7LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has a 'Shared[", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' field '", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pf_f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' - this creates a reference cycle that leaks memory.\n      FIX: Use 'Weak[", 75LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' for back-references to break the cycle.", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1436); _tr_str_release(_at_t1436); });
                }
            }
        }
#line 4861 "src/sema.tr"
        pf_i = (pf_i + 1LL);
    }
#line 4863 "src/sema.tr"
    List_ptr* hmethods = (void*)List_ptr_new();
#line 4864 "src/sema.tr"
    i = 0LL;
#line 4865 "src/sema.tr"
    while ((i < c->methods->len)) {
#line 4866 "src/sema.tr"
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(c->methods, i)));
#line 4867 "src/sema.tr"
        List_ptr_append(hmethods, _tr_obj_retain(hm));
#line 4868 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
#line 4871 "src/sema.tr"
    long long _ifc_i = 0LL;
#line 4872 "src/sema.tr"
    while ((_ifc_i < c->iface_names->len)) {
#line 4873 "src/sema.tr"
        TrStr _ifc_nm = List_TrStr_get(c->iface_names, _ifc_i);
#line 4874 "src/sema.tr"
        if (_tr_str_eqv((_ifc_nm), (_tr_str_lit_len("Sendable", 8LL)))) {
#line 4878 "src/sema.tr"
            self->sendable_tparams = c->generics;
#line 4879 "src/sema.tr"
            Sema_check_class_sendable_fields(self, c);
#line 4880 "src/sema.tr"
            self->sendable_tparams = (void*)List_TrStr_new();
        } else if (_tr_str_eqv((_ifc_nm), (_tr_str_lit_len("UnsafeSendable", 14LL)))) {
#line 4885 "src/sema.tr"
            /* pass */
        } else if ((!_tr_dict_contains(self->interfaces, _tr_strz(_ifc_nm)))) {
#line 4887 "src/sema.tr"
            ({ TrStr _at_t1437 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-1] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares 'implements ", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but interface '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not defined.\n      FIX: Define 'interface ", 47LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":' before this class, or check for typos.", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1437); _tr_str_release(_at_t1437); });
        } else {
#line 4889 "src/sema.tr"
            InterfaceDef* _idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(_ifc_nm)));
#line 4890 "src/sema.tr"
            long long _im_i = 0LL;
#line 4891 "src/sema.tr"
            while ((_im_i < _idef->methods->len)) {
#line 4892 "src/sema.tr"
                FunctionDef* _imeth = ((FunctionDef*)List_ptr_get(_idef->methods, _im_i));
#line 4893 "src/sema.tr"
                bool _found = false;
#line 4894 "src/sema.tr"
                long long _cm_i = 0LL;
#line 4895 "src/sema.tr"
                while ((_cm_i < c->methods->len)) {
#line 4896 "src/sema.tr"
                    FunctionDef* _cmeth = ((FunctionDef*)List_ptr_get(c->methods, _cm_i));
#line 4897 "src/sema.tr"
                    if (_tr_str_eqv((_cmeth->name), (_imeth->name))) {
#line 4898 "src/sema.tr"
                        _found = true;
#line 4900 "src/sema.tr"
                        TrStr _iret = _tr_str_lit_len("void", 4LL);
#line 4901 "src/sema.tr"
                        if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4902 "src/sema.tr"
                            TrStr _strtmp_t1438 = _tr_str_retain((*_imeth->ret_ty)->name);
                            _tr_str_release(_iret);
                            _iret = _strtmp_t1438;
                        }
#line 4903 "src/sema.tr"
                        TrStr _cret = _tr_str_lit_len("void", 4LL);
#line 4904 "src/sema.tr"
                        if ((((unsigned long long)(_cmeth->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4905 "src/sema.tr"
                            TrStr _strtmp_t1439 = _tr_str_retain((*_cmeth->ret_ty)->name);
                            _tr_str_release(_cret);
                            _cret = _strtmp_t1439;
                        }
#line 4907 "src/sema.tr"
                        bool _iret_is_generic = ((((_ifc_i >= 0LL) && (_idef->generics->len > 0LL)) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("", 0LL)))));
#line 4908 "src/sema.tr"
                        long long _gi = 0LL;
#line 4909 "src/sema.tr"
                        while ((_gi < _idef->generics->len)) {
#line 4910 "src/sema.tr"
                            if (({ TrStr _cmpa_t1440 = (List_TrStr_get(_idef->generics, _gi)); TrStr _cmpb_t1441 = (_iret); _Bool _cmpr_t1442 = _tr_str_eqv(_cmpa_t1440, _cmpb_t1441); _tr_str_release(_cmpa_t1440); _cmpr_t1442; })) {
#line 4910 "src/sema.tr"
                                _iret_is_generic = true;
                            }
#line 4911 "src/sema.tr"
                            _gi = (_gi + 1LL);
                        }
#line 4912 "src/sema.tr"
                        if (((((!_iret_is_generic) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((_cret), (_iret))))) {
#line 4913 "src/sema.tr"
                            ({ TrStr _at_t1443 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-3] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': method '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_cmeth->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' returns '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_cret)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but interface '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares '-> ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_iret)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: Change return type to '-> ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_iret)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1443); _tr_str_release(_at_t1443); });
                        }
#line 4915 "src/sema.tr"
                        long long _ip_cnt = 0LL;
#line 4916 "src/sema.tr"
                        long long _ip_i = 0LL;
#line 4917 "src/sema.tr"
                        while ((_ip_i < _imeth->params->len)) {
#line 4918 "src/sema.tr"
                            if ((!_tr_str_eqv((((Param*)List_ptr_get(_imeth->params, _ip_i))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4918 "src/sema.tr"
                                _ip_cnt = (_ip_cnt + 1LL);
                            }
#line 4919 "src/sema.tr"
                            _ip_i = (_ip_i + 1LL);
                        }
#line 4920 "src/sema.tr"
                        long long _cp_cnt = 0LL;
#line 4921 "src/sema.tr"
                        long long _cp_i = 0LL;
#line 4922 "src/sema.tr"
                        while ((_cp_i < _cmeth->params->len)) {
#line 4923 "src/sema.tr"
                            if ((!_tr_str_eqv((((Param*)List_ptr_get(_cmeth->params, _cp_i))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4923 "src/sema.tr"
                                _cp_cnt = (_cp_cnt + 1LL);
                            }
#line 4924 "src/sema.tr"
                            _cp_i = (_cp_i + 1LL);
                        }
#line 4925 "src/sema.tr"
                        if ((_ip_cnt != _cp_cnt)) {
#line 4926 "src/sema.tr"
                            ({ __auto_type _ord_t1444 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-3] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': method '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_cmeth->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1445 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1444); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_cp_cnt)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" parameter(s) but interface '", 29LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' requires ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1446 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1445); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_ip_cnt)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: Match the parameter list in '", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' exactly.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1446); _tr_str_release(_at_t1446); });
                        }
                    }
#line 4927 "src/sema.tr"
                    _cm_i = (_cm_i + 1LL);
                }
#line 4928 "src/sema.tr"
                if ((!_found)) {
#line 4929 "src/sema.tr"
                    TrStr _sig = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("pub def ", 8LL)), (_imeth->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(self", 5LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 4930 "src/sema.tr"
                    long long _pi = 0LL;
#line 4931 "src/sema.tr"
                    while ((_pi < _imeth->params->len)) {
#line 4932 "src/sema.tr"
                        Param* _p = ((Param*)List_ptr_get(_imeth->params, _pi));
#line 4933 "src/sema.tr"
                        if ((!_tr_str_eqv((_p->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4934 "src/sema.tr"
                            TrStr _pty = _tr_str_lit_len("int", 3LL);
#line 4935 "src/sema.tr"
                            if ((((unsigned long long)(_p->ty)) != ((unsigned long long)(0LL)))) {
#line 4935 "src/sema.tr"
                                TrStr _strtmp_t1447 = _tr_str_retain((*_p->ty)->name);
                                _tr_str_release(_pty);
                                _pty = _strtmp_t1447;
                            }
#line 4936 "src/sema.tr"
                            ({ TrStr _ap_t1448 = (_tr_str_lit_len(", ", 2LL)); TrStr _ap_t1449 = (_p->name); TrStr _ap_t1450 = (_tr_str_lit_len(": ", 2LL)); TrStr _ap_t1451 = (_pty); _tr_str_append(&_sig, _ap_t1448); _tr_str_append(&_sig, _ap_t1449); _tr_str_append(&_sig, _ap_t1450); _tr_str_append(&_sig, _ap_t1451); });
                        }
#line 4937 "src/sema.tr"
                        _pi = (_pi + 1LL);
                    }
#line 4938 "src/sema.tr"
                    TrStr _iret2 = _tr_str_lit_len("void", 4LL);
#line 4939 "src/sema.tr"
                    if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4939 "src/sema.tr"
                        TrStr _strtmp_t1452 = _tr_str_retain((*_imeth->ret_ty)->name);
                        _tr_str_release(_iret2);
                        _iret2 = _strtmp_t1452;
                    }
#line 4940 "src/sema.tr"
                    ({ TrStr _ap_t1453 = (_tr_str_lit_len(") -> ", 5LL)); TrStr _ap_t1454 = (_iret2); TrStr _ap_t1455 = (_tr_str_lit_len(":", 1LL)); _tr_str_append(&_sig, _ap_t1453); _tr_str_append(&_sig, _ap_t1454); _tr_str_append(&_sig, _ap_t1455); });
#line 4941 "src/sema.tr"
                    ({ TrStr _at_t1456 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-2] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' implements '", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but is missing method '", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_imeth->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: Add to 'extend ", 29LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":'  ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_sig)); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1456); _tr_str_release(_at_t1456); });
                }
#line 4942 "src/sema.tr"
                _im_i = (_im_i + 1LL);
            }
        }
#line 4943 "src/sema.tr"
        _ifc_i = (_ifc_i + 1LL);
        _tr_str_release(_ifc_nm);
    }
#line 4945 "src/sema.tr"
    HirClass* hc = ((HirClass*)_tr_obj_alloc(sizeof(HirClass)));
#line 4946 "src/sema.tr"
    hc->name = _tr_str_retain(c->name);
#line 4947 "src/sema.tr"
    hc->generics = c->generics;
#line 4948 "src/sema.tr"
    hc->fields = hfields;
#line 4949 "src/sema.tr"
    hc->methods = hmethods;
#line 4950 "src/sema.tr"
    hc->base_classes = c->base_classes;
#line 4951 "src/sema.tr"
    hc->iface_names = c->iface_names;
#line 4952 "src/sema.tr"
    hc->iface_targs = c->iface_targs;
#line 4953 "src/sema.tr"
    hc->decorators = c->decorators;
#line 4954 "src/sema.tr"
    hc->is_public = c->is_public;
#line 4955 "src/sema.tr"
    hc->is_class = c->is_class;
#line 4957 "src/sema.tr"
    self->current_class_name = _tr_str_lit_len("", 0LL);
#line 4958 "src/sema.tr"
    self->current_region_params = (void*)List_TrStr_new();
#line 4959 "src/sema.tr"
    return hc;
}

__attribute__((hot)) HirEnum* Sema_lower_enum(Sema* self, EnumDef* e) {
#line 4962 "src/sema.tr"
    self->current_class_name = _tr_str_retain(e->name);
#line 4963 "src/sema.tr"
    self->current_region_params = e->region_params;
#line 4964 "src/sema.tr"
    List_ptr* hvariants = (void*)List_ptr_new();
#line 4965 "src/sema.tr"
    long long i = 0LL;
#line 4966 "src/sema.tr"
    while ((i < e->variants->len)) {
#line 4967 "src/sema.tr"
        VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, i));
#line 4968 "src/sema.tr"
        List_ptr* hfields = (void*)List_ptr_new();
#line 4969 "src/sema.tr"
        long long j = 0LL;
#line 4970 "src/sema.tr"
        while ((j < v->fields->len)) {
#line 4971 "src/sema.tr"
            Param* p = ((Param*)List_ptr_get(v->fields, j));
#line 4975 "src/sema.tr"
            if ((self->strict_mode && (((unsigned long long)(p->ty)) != ((unsigned long long)(0LL))))) {
#line 4976 "src/sema.tr"
                AstType* _vty = (*p->ty);
#line 4977 "src/sema.tr"
                long long _vri = 0LL;
#line 4978 "src/sema.tr"
                while ((_vri < _vty->from_regions->len)) {
#line 4979 "src/sema.tr"
                    TrStr _vrn = List_TrStr_get(_vty->from_regions, _vri);
#line 4980 "src/sema.tr"
                    bool _vrok = false;
#line 4981 "src/sema.tr"
                    long long _vrk = 0LL;
#line 4982 "src/sema.tr"
                    while ((_vrk < e->region_params->len)) {
#line 4983 "src/sema.tr"
                        if (({ TrStr _cmpa_t1457 = (List_TrStr_get(e->region_params, _vrk)); TrStr _cmpb_t1458 = (_vrn); _Bool _cmpr_t1459 = _tr_str_eqv(_cmpa_t1457, _cmpb_t1458); _tr_str_release(_cmpa_t1457); _cmpr_t1459; })) {
#line 4983 "src/sema.tr"
                            _vrok = true;
                        }
#line 4984 "src/sema.tr"
                        _vrk = (_vrk + 1LL);
                    }
#line 4985 "src/sema.tr"
                    if ((!_vrok)) {
#line 4986 "src/sema.tr"
                        self->current_line = e->line;
#line 4987 "src/sema.tr"
                        ({ TrStr _at_t1460 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-2] region source '", 21LL)), (_vrn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in variant '", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of enum '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (e->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not a region parameter.\n      FIX: declare 'enum ", 54LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (e->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" from ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_vrn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":'.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1460); _tr_str_release(_at_t1460); });
                    }
#line 4988 "src/sema.tr"
                    _vri = (_vri + 1LL);
                    _tr_str_release(_vrn);
                }
            }
#line 4989 "src/sema.tr"
            HirParam* hp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 4990 "src/sema.tr"
            hp->name = _tr_str_retain(p->name);
#line 4995 "src/sema.tr"
            AstType* p_ty = AstType_init(p->name);
#line 4996 "src/sema.tr"
            if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
#line 4996 "src/sema.tr"
                p_ty = (*p->ty);
            }
#line 4997 "src/sema.tr"
            hp->ty = p_ty;
#line 4998 "src/sema.tr"
            List_ptr_append(hfields, _tr_obj_retain(hp));
#line 4999 "src/sema.tr"
            j = (j + 1LL);
            _tr_obj_release(hp, _trdrop_HirParam);
        }
#line 5001 "src/sema.tr"
        HirVariant* hv = ((HirVariant*)_tr_obj_alloc(sizeof(HirVariant)));
#line 5002 "src/sema.tr"
        hv->name = _tr_str_retain(v->name);
#line 5003 "src/sema.tr"
        hv->fields = hfields;
#line 5004 "src/sema.tr"
        List_ptr_append(hvariants, _tr_obj_retain(hv));
#line 5005 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hv, _trdrop_HirVariant);
    }
#line 5007 "src/sema.tr"
    List_ptr* hmethods = (void*)List_ptr_new();
#line 5008 "src/sema.tr"
    i = 0LL;
#line 5009 "src/sema.tr"
    while ((i < e->methods->len)) {
#line 5010 "src/sema.tr"
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(e->methods, i)));
#line 5011 "src/sema.tr"
        List_ptr_append(hmethods, _tr_obj_retain(hm));
#line 5012 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
#line 5014 "src/sema.tr"
    HirEnum* he = ((HirEnum*)_tr_obj_alloc(sizeof(HirEnum)));
#line 5015 "src/sema.tr"
    he->name = _tr_str_retain(e->name);
#line 5016 "src/sema.tr"
    he->generics = e->generics;
#line 5017 "src/sema.tr"
    he->variants = hvariants;
#line 5018 "src/sema.tr"
    he->methods = hmethods;
#line 5019 "src/sema.tr"
    he->iface_names = e->iface_names;
#line 5020 "src/sema.tr"
    he->decorators = e->decorators;
#line 5021 "src/sema.tr"
    he->is_public = e->is_public;
#line 5023 "src/sema.tr"
    self->current_class_name = _tr_str_lit_len("", 0LL);
#line 5024 "src/sema.tr"
    self->current_region_params = (void*)List_TrStr_new();
#line 5025 "src/sema.tr"
    return he;
}

__attribute__((hot)) HirInterface* Sema_lower_interface(Sema* self, InterfaceDef* i_def) {
#line 5028 "src/sema.tr"
    self->current_region_params = i_def->region_params;
#line 5029 "src/sema.tr"
    List_ptr* hmethods = (void*)List_ptr_new();
#line 5030 "src/sema.tr"
    long long i = 0LL;
#line 5031 "src/sema.tr"
    while ((i < i_def->methods->len)) {
#line 5032 "src/sema.tr"
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(i_def->methods, i)));
#line 5033 "src/sema.tr"
        List_ptr_append(hmethods, _tr_obj_retain(hm));
#line 5034 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
#line 5036 "src/sema.tr"
    HirInterface* hi = ((HirInterface*)_tr_obj_alloc(sizeof(HirInterface)));
#line 5037 "src/sema.tr"
    hi->name = _tr_str_retain(i_def->name);
#line 5038 "src/sema.tr"
    hi->generics = i_def->generics;
#line 5039 "src/sema.tr"
    hi->methods = hmethods;
#line 5040 "src/sema.tr"
    self->current_region_params = (void*)List_TrStr_new();
#line 5041 "src/sema.tr"
    return hi;
}

__attribute__((hot)) void Sema_apply_escape_marks(Sema* self, HirStmt* _hs) {
#line 5047 "src/sema.tr"
    __auto_type _t1461 = (*_hs);
    if (_t1461.tag == HirStmt_SExpr) {
        __auto_type _es_e = _t1461.data.SExpr.expr;
#line 5049 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _es_e);
#line 5050 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _es_e);
    } else if (_t1461.tag == HirStmt_SLet) {
        __auto_type _sl_v = _t1461.data.SLet.val;
#line 5052 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sl_v);
#line 5053 "src/sema.tr"
        Sema_mark_coll_arg(self, _sl_v);
#line 5054 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sl_v);
    } else if (_t1461.tag == HirStmt_SAssign) {
        __auto_type _sa_t = _t1461.data.SAssign.target;
__auto_type _sa_v = _t1461.data.SAssign.val;
#line 5056 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sa_t);
#line 5057 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sa_v);
#line 5058 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sa_t);
#line 5059 "src/sema.tr"
        Sema_mark_coll_arg(self, _sa_v);
#line 5060 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sa_v);
    } else if (_t1461.tag == HirStmt_SReturn) {
        __auto_type _sr_e = _t1461.data.SReturn.val;
#line 5062 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sr_e);
#line 5063 "src/sema.tr"
        Sema_mark_coll_arg(self, _sr_e);
#line 5064 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sr_e);
    } else if (_t1461.tag == HirStmt_SIf) {
        __auto_type _si_c = _t1461.data.SIf.cond;
#line 5066 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _si_c);
#line 5067 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _si_c);
    } else if (_t1461.tag == HirStmt_SWhile) {
        __auto_type _sw_c = _t1461.data.SWhile.cond;
#line 5069 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sw_c);
#line 5070 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sw_c);
    } else if (_t1461.tag == HirStmt_SFor) {
        __auto_type _sf_iter = _t1461.data.SFor.iter;
#line 5072 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sf_iter);
    } else if (_t1461.tag == HirStmt_SForUnpack) {
        __auto_type _sfu_iter = _t1461.data.SForUnpack.iter;
#line 5074 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sfu_iter);
    } else if (1) {
        __auto_type _ = _t1461;
        /* pass */
    }
}

__attribute__((hot)) HirBlock* Sema_lower_block(Sema* self, Block* b) {
#line 5078 "src/sema.tr"
    HirBlock* hb = HirBlock_init();
#line 5079 "src/sema.tr"
    long long i = 0LL;
#line 5080 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 5081 "src/sema.tr"
        Stmt* orig_ptr = ((Stmt*)List_ptr_get(b->stmts, i));
#line 5082 "src/sema.tr"
        HirStmt* _hs = Sema_lower_stmt(self, orig_ptr);
#line 5083 "src/sema.tr"
        Sema_apply_escape_marks(self, _hs);
#line 5087 "src/sema.tr"
        if ((((unsigned long long)(orig_ptr)) != ((unsigned long long)(0LL)))) {
#line 5088 "src/sema.tr"
            __auto_type _t1462 = (*orig_ptr);
            if (_t1462.tag == Stmt_SReturn) {
                __auto_type _ = _t1462.data.SReturn.val;
#line 5097 "src/sema.tr"
                List_TrStr* ret_excl = (void*)List_TrStr_new();
#line 5098 "src/sema.tr"
                __auto_type _t1463 = (*_hs);
                if (_t1463.tag == HirStmt_SReturn) {
                    __auto_type lowered_ret = _t1463.data.SReturn.val;
#line 5100 "src/sema.tr"
                    Sema_collect_idents(self, lowered_ret, ret_excl);
                } else if (1) {
                    __auto_type _ = _t1463;
                    /* pass */
                }
#line 5105 "src/sema.tr"
                long long ret_from = 0LL;
#line 5106 "src/sema.tr"
                if ((self->fn_scope_base->len > 0LL)) {
#line 5107 "src/sema.tr"
                    ret_from = List_i64_get(self->fn_scope_base, (self->fn_scope_base->len - 1LL));
                }
#line 5113 "src/sema.tr"
                bool ret_pre_done = false;
#line 5114 "src/sema.tr"
                if ((ret_excl->len > 0LL)) {
#line 5115 "src/sema.tr"
                    __auto_type _t1464 = (*_hs);
                    if (_t1464.tag == HirStmt_SReturn) {
                        __auto_type prim_ret = _t1464.data.SReturn.val;
#line 5117 "src/sema.tr"
                        if ((((unsigned long long)(prim_ret)) != ((unsigned long long)(0LL)))) {
#line 5118 "src/sema.tr"
                            AstType* prim_ty = hir_expr_type(prim_ret);
#line 5119 "src/sema.tr"
                            if ((((Sema_is_primitive_name(self, prim_ty->name) && (!_tr_str_eqv((prim_ty->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((prim_ty->name), (_tr_str_lit_len("None", 4LL))))) && (!_tr_str_eqv((prim_ty->name), (_tr_str_lit_len("lambda", 6LL)))))) {
#line 5120 "src/sema.tr"
                                HirBlock* all_drops = HirBlock_init();
#line 5121 "src/sema.tr"
                                Sema_append_drops_from_excl_multi(self, all_drops, ret_from, (void*)List_TrStr_new());
#line 5127 "src/sema.tr"
                                HirBlock* pre_drops = HirBlock_init();
#line 5128 "src/sema.tr"
                                long long adi = 0LL;
#line 5129 "src/sema.tr"
                                while ((adi < all_drops->stmts->len)) {
#line 5130 "src/sema.tr"
                                    HirStmt* ad = ((HirStmt*)List_ptr_get(all_drops->stmts, adi));
#line 5131 "src/sema.tr"
                                    bool keep_d = true;
#line 5132 "src/sema.tr"
                                    __auto_type _t1465 = (*ad);
                                    if (_t1465.tag == HirStmt_SAutoDrop) {
                                        __auto_type adn = _t1465.data.SAutoDrop.name;
__auto_type adt = _t1465.data.SAutoDrop.class_name;
#line 5134 "src/sema.tr"
                                        bool was_excl = false;
#line 5135 "src/sema.tr"
                                        long long aei = 0LL;
#line 5136 "src/sema.tr"
                                        while ((aei < ret_excl->len)) {
#line 5137 "src/sema.tr"
                                            if (({ TrStr _cmpa_t1466 = (List_TrStr_get(ret_excl, aei)); TrStr _cmpb_t1467 = (adn); _Bool _cmpr_t1468 = _tr_str_eqv(_cmpa_t1466, _cmpb_t1467); _tr_str_release(_cmpa_t1466); _cmpr_t1468; })) {
#line 5137 "src/sema.tr"
                                                was_excl = true;
                                            }
#line 5138 "src/sema.tr"
                                            aei = (aei + 1LL);
                                        }
#line 5139 "src/sema.tr"
                                        if ((was_excl && (!Sema__early_exit_drop_ok(self, adt)))) {
#line 5139 "src/sema.tr"
                                            keep_d = false;
                                        }
                                    } else if (1) {
                                        __auto_type _ = _t1465;
                                        /* pass */
                                    }
#line 5141 "src/sema.tr"
                                    if (keep_d) {
#line 5141 "src/sema.tr"
                                        HirBlock_push(pre_drops, ad);
                                    }
#line 5142 "src/sema.tr"
                                    adi = (adi + 1LL);
                                }
#line 5143 "src/sema.tr"
                                if ((pre_drops->stmts->len > 0LL)) {
#line 5144 "src/sema.tr"
                                    self->do_temp_ctr = (self->do_temp_ctr + 1LL);
#line 5145 "src/sema.tr"
                                    TrStr rt_name = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->do_temp_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_trret", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 5146 "src/sema.tr"
                                    HirBlock_push(hb, box_hirstmt(HirStmt_ctor_SLet(rt_name, Ownership_make_Own(), false, false, false, prim_ty, prim_ret)));
#line 5147 "src/sema.tr"
                                    long long pdi = 0LL;
#line 5148 "src/sema.tr"
                                    while ((pdi < pre_drops->stmts->len)) {
#line 5149 "src/sema.tr"
                                        HirBlock_push(hb, ((HirStmt*)List_ptr_get(pre_drops->stmts, pdi)));
#line 5150 "src/sema.tr"
                                        pdi = (pdi + 1LL);
                                    }
#line 5151 "src/sema.tr"
                                    _hs = box_hirstmt(HirStmt_ctor_SReturn(box_hirexpr(HirExpr_ctor_EIdent(rt_name, prim_ty, false))));
#line 5152 "src/sema.tr"
                                    ret_pre_done = true;
                                }
                                _tr_obj_release(pre_drops, _trdrop_HirBlock);
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t1464;
                        /* pass */
                    }
                }
#line 5154 "src/sema.tr"
                if ((!ret_pre_done)) {
#line 5155 "src/sema.tr"
                    Sema_append_drops_from_excl_multi(self, hb, ret_from, ret_excl);
                }
            } else if (_t1462.tag == Stmt_SBreak) {
                __auto_type _ = _t1462.data.SBreak.val;
#line 5157 "src/sema.tr"
                if ((self->loop_scope_base->len > 0LL)) {
#line 5158 "src/sema.tr"
                    Sema_append_drops_from(self, hb, List_i64_get(self->loop_scope_base, (self->loop_scope_base->len - 1LL)));
#line 5159 "src/sema.tr"
                    Sema_append_while_exit_drops(self, hb);
                }
            } else if (_t1462.tag == Stmt_SContinue) {
#line 5161 "src/sema.tr"
                if ((self->loop_scope_base->len > 0LL)) {
#line 5162 "src/sema.tr"
                    Sema_append_drops_from(self, hb, List_i64_get(self->loop_scope_base, (self->loop_scope_base->len - 1LL)));
#line 5163 "src/sema.tr"
                    Sema_append_while_exit_drops(self, hb);
                }
            } else if (1) {
                __auto_type _ = _t1462;
                /* pass */
            }
        }
#line 5165 "src/sema.tr"
        HirBlock_push(hb, _hs);
#line 5166 "src/sema.tr"
        i = (i + 1LL);
    }
#line 5167 "src/sema.tr"
    return hb;
}

__attribute__((hot)) HirStmt* Sema_lower_stmt(Sema* self, Stmt* s_ptr) {
#line 5170 "src/sema.tr"
    if ((((unsigned long long)(s_ptr)) == ((unsigned long long)(0LL)))) {
#line 5170 "src/sema.tr"
        return (HirStmt*)(0LL);
    }
#line 5171 "src/sema.tr"
    __auto_type s = (*s_ptr);
#line 5172 "src/sema.tr"
    __auto_type _t1469 = s;
    if (_t1469.tag == Stmt_SExpr) {
        __auto_type e = _t1469.data.SExpr.expr;
#line 5174 "src/sema.tr"
        HirStmt* h_s_expr = box_hirstmt(HirStmt_ctor_SExpr(Sema_lower_expr(self, e)));
#line 5176 "src/sema.tr"
        if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
#line 5177 "src/sema.tr"
            __auto_type _t1470 = (*e);
            if (_t1470.tag == Expr_ECall) {
                __auto_type callee = _t1470.data.ECall.callee;
__auto_type args = _t1470.data.ECall.args;
#line 5179 "src/sema.tr"
                if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 5180 "src/sema.tr"
                    __auto_type _t1471 = (*callee);
                    if (_t1471.tag == Expr_EIdent) {
                        __auto_type fn_name = _t1471.data.EIdent.name;
#line 5182 "src/sema.tr"
                        Symbol* fn_sym = Sema_resolve(self, fn_name);
#line 5183 "src/sema.tr"
                        if (((fn_sym->kind.tag == SymbolKind_make_SFunction().tag) && _tr_str_eqv(((*fn_sym->ty)->name), (_tr_str_lit_len("Result", 6LL))))) {
#line 5184 "src/sema.tr"
                            ({ TrStr _at_t1472 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-4] '", 7LL)), (fn_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' returns a Result and its error must be handled. FIX: Assign the result and match on it, use '?' to propagate, or '_ = ", 122LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fn_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' to explicitly discard.", 29LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1472); _tr_str_release(_at_t1472); });
                        }
                    } else if (1) {
                        __auto_type _ = _t1471;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1470;
                /* pass */
            }
        }
#line 5187 "src/sema.tr"
        return h_s_expr;
    } else if (_t1469.tag == Stmt_SReturn) {
        __auto_type e = _t1469.data.SReturn.val;
#line 5195 "src/sema.tr"
        if (((self->strict_mode && self->current_func_ret_borrow_str) && (((unsigned long long)(e)) != ((unsigned long long)(0LL))))) {
#line 5196 "src/sema.tr"
            __auto_type _t1473 = (*e);
            if (_t1473.tag == Expr_EBinOp) {
                __auto_type l3_op = _t1473.data.EBinOp.op;
#line 5198 "src/sema.tr"
                if (_tr_str_eqv((l3_op), (_tr_str_lit_len("+", 1LL)))) {
#line 5199 "src/sema.tr"
                    ({ TrStr _at_t1474 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-3] borrow-returning function returns a freshly-built string (owned), not a borrow of region '", 96LL)), (self->current_func_ret_from))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: return a slice/view of the region source, or change the return type to a plain owned 'str'.", 105LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1474); _tr_str_release(_at_t1474); });
                }
            } else if (_t1473.tag == Expr_EIdent) {
                __auto_type l3_nm = _t1473.data.EIdent.name;
#line 5201 "src/sema.tr"
                Symbol* l3_sym = Sema_resolve(self, l3_nm);
#line 5202 "src/sema.tr"
                if ((!_tr_str_eqv((l3_sym->name), (_tr_str_lit_len("", 0LL))))) {
#line 5205 "src/sema.tr"
                    TrStr l3_eff = _tr_str_lit_len("", 0LL);
#line 5206 "src/sema.tr"
                    if (l3_sym->is_param) {
#line 5207 "src/sema.tr"
                        TrStr _strtmp_t1475 = _tr_str_retain(l3_nm);
                        _tr_str_release(l3_eff);
                        l3_eff = _strtmp_t1475;
                    } else {
#line 5209 "src/sema.tr"
                        TrStr _strtmp_t1476 = _tr_str_retain(l3_sym->borrows_region);
                        _tr_str_release(l3_eff);
                        l3_eff = _strtmp_t1476;
                    }
#line 5210 "src/sema.tr"
                    if (_tr_str_eqv((l3_eff), (_tr_str_lit_len("@owned", 6LL)))) {
#line 5211 "src/sema.tr"
                        ({ TrStr _at_t1477 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-3] borrow-returning function returns '", 41LL)), (l3_nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', a freshly-built string (owned), not a borrow of region '", 59LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_ret_from)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: return a slice/view of the region source, or change the return type to a plain owned 'str'.", 105LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1477); _tr_str_release(_at_t1477); });
                    } else if (((!_tr_str_eqv((l3_eff), (_tr_str_lit_len("", 0LL)))) && (self->current_func_ret_regions->len > 0LL))) {
#line 5216 "src/sema.tr"
                        bool l3_in = false;
#line 5217 "src/sema.tr"
                        long long l3ri = 0LL;
#line 5218 "src/sema.tr"
                        while ((l3ri < self->current_func_ret_regions->len)) {
#line 5219 "src/sema.tr"
                            if (({ TrStr _cmpa_t1478 = (List_TrStr_get(self->current_func_ret_regions, l3ri)); TrStr _cmpb_t1479 = (l3_eff); _Bool _cmpr_t1480 = _tr_str_eqv(_cmpa_t1478, _cmpb_t1479); _tr_str_release(_cmpa_t1478); _cmpr_t1480; })) {
#line 5220 "src/sema.tr"
                                l3_in = true;
                            }
#line 5221 "src/sema.tr"
                            l3ri = (l3ri + 1LL);
                        }
#line 5225 "src/sema.tr"
                        if ((!l3_in)) {
#line 5226 "src/sema.tr"
                            long long l3di = 0LL;
#line 5227 "src/sema.tr"
                            while ((l3di < self->current_func_ret_regions->len)) {
#line 5228 "src/sema.tr"
                                if (({ TrStr _at_t1481 = (List_TrStr_get(self->current_func_ret_regions, l3di)); __auto_type _wr = (Sema_region_outlives(self, l3_eff, _at_t1481)); _tr_str_release(_at_t1481); _wr; })) {
#line 5229 "src/sema.tr"
                                    l3_in = true;
                                }
#line 5230 "src/sema.tr"
                                l3di = (l3di + 1LL);
                            }
                        }
#line 5231 "src/sema.tr"
                        if ((!l3_in)) {
#line 5232 "src/sema.tr"
                            ({ TrStr _at_t1482 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-4] borrow-returning function returns a borrow of region '", 60LL)), (l3_eff))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but its return is declared 'from ", 36LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_ret_from)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: add '", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (l3_eff)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to the 'from' list, declare 'where ", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (l3_eff)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" outlives ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_ret_from)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or return a borrow of a declared region.", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1482); _tr_str_release(_at_t1482); });
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1473;
                /* pass */
            }
        }
#line 5235 "src/sema.tr"
        if ((((((unsigned long long)(e)) != ((unsigned long long)(0LL))) && (!self->in_unsafe)) && _tr_str_eqv((self->current_func_ret_from), (_tr_str_lit_len("", 0LL))))) {
#line 5236 "src/sema.tr"
            __auto_type _t1483 = (*e);
            if (_t1483.tag == Expr_EIdent) {
                __auto_type ret_name = _t1483.data.EIdent.name;
#line 5238 "src/sema.tr"
                Symbol* ret_sym = Sema_resolve(self, ret_name);
#line 5239 "src/sema.tr"
                if (((!_tr_str_eqv((ret_sym->name), (_tr_str_lit_len("", 0LL)))) && (ret_sym->ptr_region == 0LL))) {
#line 5240 "src/sema.tr"
                    if ((((unsigned long long)(ret_sym->ty)) != ((unsigned long long)(0LL)))) {
#line 5241 "src/sema.tr"
                        if (_tr_str_eqv(((*ret_sym->ty)->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 5242 "src/sema.tr"
                            ({ TrStr _at_t1484 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-1] '", 7LL)), (ret_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a local Pointer that may not outlive this function call. Returning it is unsafe.\n      FIX: Annotate the return type with 'from <param>' if the pointer borrows from a parameter, or wrap the allocation in 'unsafe:' if it is heap-allocated.", 243LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1484); _tr_str_release(_at_t1484); });
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1483;
                /* pass */
            }
        }
#line 5244 "src/sema.tr"
        HirExpr* _ret_h = Sema_lower_expr(self, e);
#line 5245 "src/sema.tr"
        if ((((((unsigned long long)(e)) != ((unsigned long long)(0LL))) && (self->closure_depth == 0LL)) && (!_tr_str_eqv((self->current_func_ret_family), (_tr_str_lit_len("", 0LL)))))) {
#line 5246 "src/sema.tr"
            TrStr _rf = Sema_firm_family(self, _ret_h);
#line 5247 "src/sema.tr"
            if (Sema_family_mismatch(self, self->current_func_ret_family, _rf)) {
#line 5248 "src/sema.tr"
                ({ __auto_type _ord_t1485 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is declared to return ", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1486 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1485); TrStr _tr_cr = (Sema_family_word(self, self->current_func_ret_family)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but this returns ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1487 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1486); TrStr _tr_cr = (Sema_family_word(self, _rf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: return a value of the declared type, or change the function's return type.", 87LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1487); _tr_str_release(_at_t1487); });
            }
        }
#line 5249 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SReturn(_ret_h));
    } else if (_t1469.tag == Stmt_SLet) {
        __auto_type name = _t1469.data.SLet.name;
__auto_type ownership = _t1469.data.SLet.ownership;
__auto_type is_mut = _t1469.data.SLet.is_mut;
__auto_type is_const = _t1469.data.SLet.is_const;
__auto_type is_shared = _t1469.data.SLet.is_shared;
__auto_type ty_ptr = _t1469.data.SLet.ty;
__auto_type val_ptr = _t1469.data.SLet.val;
#line 5252 "src/sema.tr"
        if (((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))))) {
#line 5253 "src/sema.tr"
            __auto_type _t1488 = (*val_ptr);
            if (_t1488.tag == Expr_ELitNone) {
#line 5255 "src/sema.tr"
                TrStr m7_ty_name = _tr_str_retain((*ty_ptr)->name);
#line 5256 "src/sema.tr"
                if ((((((!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("Option", 6LL)))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("None", 4LL))))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("Pointer", 7LL)))))) {
#line 5257 "src/sema.tr"
                    ({ TrStr _at_t1489 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-7] Cannot assign 'none' to '", 31LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' which has type '", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m7_ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'. Only Option[T] can hold 'none'. FIX: Use 'Option[", 52LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m7_ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' as the type, or give '", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' a real initial value.", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1489); _tr_str_release(_at_t1489); });
                }
                _tr_str_release(m7_ty_name);
            } else if (1) {
                __auto_type _ = _t1488;
                /* pass */
            }
        }
#line 5259 "src/sema.tr"
        AstType* ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 5260 "src/sema.tr"
        if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 5261 "src/sema.tr"
            ty = (*ty_ptr);
#line 5262 "src/sema.tr"
            ({ TrStr _at_t1490 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declaration", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema__check_ty_arity(self, ty_ptr, _at_t1490, 0LL); _tr_str_release(_at_t1490); });
#line 5263 "src/sema.tr"
            ({ TrStr _at_t1491 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the declaration of '", 20LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_type_known(self, ty_ptr, _at_t1491); _tr_str_release(_at_t1491); });
        }
#line 5264 "src/sema.tr"
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
#line 5265 "src/sema.tr"
        if (((!_tr_str_eqv((ty->name), (_tr_str_lit_len("void", 4LL)))) && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("None", 4LL)))))) {
#line 5265 "src/sema.tr"
            hval = Sema__patch_empty_dict_hint(self, hval, ty);
        }
#line 5266 "src/sema.tr"
        if (((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))))) {
#line 5267 "src/sema.tr"
            ({ __auto_type _ord_t1493 = (hir_expr_type(hval)); TrStr _at_t1492 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the initializer of '", 20LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_fn_value_throws(self, ty_ptr, _ord_t1493, _at_t1492); _tr_str_release(_at_t1492); });
        }
#line 5268 "src/sema.tr"
        if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("None", 4LL))))) {
#line 5268 "src/sema.tr"
            ty = hir_expr_type(hval);
        }
#line 5273 "src/sema.tr"
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && (!is_shared))) {
#line 5274 "src/sema.tr"
            __auto_type _t1494 = (*val_ptr);
            if (_t1494.tag == Expr_EIdent) {
                __auto_type m1_src = _t1494.data.EIdent.name;
#line 5276 "src/sema.tr"
                Symbol* m1_sym = Sema_resolve(self, m1_src);
#line 5277 "src/sema.tr"
                AstType* m1_ty = (*m1_sym->ty);
#line 5279 "src/sema.tr"
                if (((((!Sema_is_primitive(self, m1_ty)) && (!Sema_is_copy_class(self, m1_ty->name))) && (!_tr_str_eqv((m1_sym->name), (_tr_str_lit_len("", 0LL))))) && (!m1_sym->is_shared))) {
#line 5280 "src/sema.tr"
                    Sema_check_not_moved(self, m1_src, m1_ty->name);
#line 5281 "src/sema.tr"
                    Sema_check_no_active_borrows(self, m1_src, m1_ty->name);
#line 5282 "src/sema.tr"
                    Sema_mark_moved(self, m1_src);
                } else if (((Sema_class_has_free(self, m1_ty->name) && (!_tr_str_eqv((m1_sym->name), (_tr_str_lit_len("", 0LL))))) && (!m1_sym->is_shared))) {
#line 5289 "src/sema.tr"
                    Sema_mark_moved(self, m1_src);
                }
            } else if (1) {
                __auto_type _ = _t1494;
                /* pass */
            }
        }
#line 5293 "src/sema.tr"
        if (((is_shared && _tr_str_eqv((ty->name), (_tr_str_lit_len("Shared", 6LL)))) && (ty->args->len > 0LL))) {
#line 5293 "src/sema.tr"
            ty = (*((AstType**)List_ptr_get(ty->args, 0LL)));
        }
#line 5294 "src/sema.tr"
        if ((((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) && (!is_shared))) {
#line 5295 "src/sema.tr"
            ({ __auto_type _ord_t1496 = ((*ty_ptr)); TrStr _at_t1495 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the initializer of '", 20LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1496, hval, _at_t1495); _tr_str_release(_at_t1495); });
        }
#line 5296 "src/sema.tr"
        TrStr _let_firm = _tr_str_lit_len("", 0LL);
#line 5297 "src/sema.tr"
        TrStr _let_cls = _tr_str_lit_len("", 0LL);
#line 5298 "src/sema.tr"
        if ((!is_shared)) {
#line 5299 "src/sema.tr"
            if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 5300 "src/sema.tr"
                TrStr _strtmp_t1497 = Sema_decl_family(self, (*ty_ptr));
                _tr_str_release(_let_firm);
                _let_firm = _strtmp_t1497;
#line 5301 "src/sema.tr"
                if ((_tr_str_eqv((_let_firm), (_tr_str_lit_len("class", 5LL))) && ({ __auto_type _ord_t1498 = ((*ty_ptr)->name); __auto_type _wr = (_tr_dict_contains(self->classes, _tr_strz(_ord_t1498))); _wr; }))) {
#line 5301 "src/sema.tr"
                    TrStr _strtmp_t1499 = _tr_str_retain((*ty_ptr)->name);
                    _tr_str_release(_let_cls);
                    _let_cls = _strtmp_t1499;
                }
            } else if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
#line 5303 "src/sema.tr"
                TrStr _strtmp_t1500 = Sema_firm_family(self, hval);
                _tr_str_release(_let_firm);
                _let_firm = _strtmp_t1500;
#line 5304 "src/sema.tr"
                if (_tr_str_eqv((_let_firm), (_tr_str_lit_len("class", 5LL)))) {
#line 5304 "src/sema.tr"
                    TrStr _strtmp_t1501 = Sema_firm_class(self, hval);
                    _tr_str_release(_let_cls);
                    _let_cls = _strtmp_t1501;
                }
            }
        }
#line 5305 "src/sema.tr"
        ({ __auto_type _ord_t1502 = (SymbolKind_make_SVariable()); __auto_type _ord_t1503 = (box_asttype(ty)); Sema_declare(self, name, _ord_t1502, _ord_t1503, is_mut); });
#line 5306 "src/sema.tr"
        if (((!_tr_str_eqv((_let_firm), (_tr_str_lit_len("", 0LL)))) && (self->scopes->len > 0LL))) {
#line 5307 "src/sema.tr"
            Scope* _fm_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 5308 "src/sema.tr"
            if (_tr_dict_contains(_fm_scope->variables, _tr_strz(name))) {
#line 5309 "src/sema.tr"
                Symbol* _fm_sym = ((Symbol*)(uintptr_t)_tr_dict_get(_fm_scope->variables, _tr_strz(name)));
#line 5310 "src/sema.tr"
                _fm_sym->firm = _tr_str_retain(_let_firm);
#line 5311 "src/sema.tr"
                _fm_sym->firm_cls = _tr_str_retain(_let_cls);
#line 5312 "src/sema.tr"
                _tr_dict_set(_fm_scope->variables, _tr_strz(name), _fm_sym);
            }
        }
#line 5316 "src/sema.tr"
        if ((is_shared && (self->scopes->len > 0LL))) {
#line 5317 "src/sema.tr"
            Scope* _sh_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 5318 "src/sema.tr"
            if (_tr_dict_contains(_sh_scope->variables, _tr_strz(name))) {
#line 5319 "src/sema.tr"
                Symbol* _sh_sym = ((Symbol*)(uintptr_t)_tr_dict_get(_sh_scope->variables, _tr_strz(name)));
#line 5320 "src/sema.tr"
                _sh_sym->is_shared = true;
#line 5321 "src/sema.tr"
                _tr_dict_set(_sh_scope->variables, _tr_strz(name), _sh_sym);
            }
        }
#line 5324 "src/sema.tr"
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
#line 5325 "src/sema.tr"
            ({ TrStr _at_t1504 = (Sema_compute_region(self, val_ptr)); Sema_set_borrows_region(self, name, _at_t1504); _tr_str_release(_at_t1504); });
        }
#line 5331 "src/sema.tr"
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
#line 5332 "src/sema.tr"
            __auto_type _t1505 = (*val_ptr);
            if (_t1505.tag == Expr_ECall) {
                __auto_type ce_callee = _t1505.data.ECall.callee;
__auto_type ce_args = _t1505.data.ECall.args;
#line 5334 "src/sema.tr"
                if ((((unsigned long long)(ce_callee)) != ((unsigned long long)(0LL)))) {
#line 5335 "src/sema.tr"
                    __auto_type _t1506 = (*ce_callee);
                    if (_t1506.tag == Expr_EIdent) {
                        __auto_type ce_fn = _t1506.data.EIdent.name;
#line 5337 "src/sema.tr"
                        Symbol* ce_sym = Sema_resolve(self, ce_fn);
#line 5338 "src/sema.tr"
                        if (((!_tr_str_eqv((ce_sym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(ce_sym->ty)) != ((unsigned long long)(0LL))))) {
#line 5339 "src/sema.tr"
                            AstType* ce_ret = (*ce_sym->ty);
#line 5340 "src/sema.tr"
                            if ((((ce_ret->is_borrow || (ce_ret->from_regions->len > 0LL)) && (ce_ret->from_index >= 0LL)) && (ce_ret->from_index < ce_args->len))) {
#line 5341 "src/sema.tr"
                                __auto_type _t1507 = (*((Expr*)List_ptr_get(ce_args, ce_ret->from_index)));
                                if (_t1507.tag == Expr_EIdent) {
                                    __auto_type ce_src = _t1507.data.EIdent.name;
#line 5343 "src/sema.tr"
                                    List_TrStr_append(self->cur_func_borrowers, name);
#line 5344 "src/sema.tr"
                                    List_TrStr_append(self->cur_func_sources, ce_src);
                                } else if (1) {
                                    __auto_type _ = _t1507;
                                    /* pass */
                                }
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t1506;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1505;
                /* pass */
            }
        }
#line 5351 "src/sema.tr"
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && _tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 5352 "src/sema.tr"
            bool heap_rhs = false;
#line 5353 "src/sema.tr"
            __auto_type _t1508 = (*val_ptr);
            if (_t1508.tag == Expr_ECall) {
                heap_rhs = true;
            } else if (_t1508.tag == Expr_EMethodCall) {
                heap_rhs = true;
            } else if (1) {
                __auto_type _ = _t1508;
                /* pass */
            }
#line 5357 "src/sema.tr"
            if (heap_rhs) {
#line 5358 "src/sema.tr"
                if ((self->scopes->len > 0LL)) {
#line 5359 "src/sema.tr"
                    Scope* alloc_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 5360 "src/sema.tr"
                    if (_tr_dict_contains(alloc_scope->variables, _tr_strz(name))) {
#line 5361 "src/sema.tr"
                        Symbol* alloc_sym = ((Symbol*)(uintptr_t)_tr_dict_get(alloc_scope->variables, _tr_strz(name)));
#line 5362 "src/sema.tr"
                        alloc_sym->ptr_region = 1LL;
#line 5363 "src/sema.tr"
                        _tr_dict_set(alloc_scope->variables, _tr_strz(name), alloc_sym);
                    }
                }
            }
        }
#line 5366 "src/sema.tr"
        bool _decl_fixed_arr = false;
#line 5367 "src/sema.tr"
        if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 5368 "src/sema.tr"
            if ((_tr_str_eqv(((*ty_ptr)->name), (_tr_str_lit_len("Array", 5LL))) && ((*ty_ptr)->array_size > 0LL))) {
#line 5368 "src/sema.tr"
                _decl_fixed_arr = true;
            }
        }
#line 5369 "src/sema.tr"
        if (((((unsigned long long)(val_ptr)) == ((unsigned long long)(0LL))) && (!_decl_fixed_arr))) {
#line 5370 "src/sema.tr"
            if ((self->scopes->len > 0LL)) {
#line 5371 "src/sema.tr"
                Scope* pd_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 5372 "src/sema.tr"
                if (_tr_dict_contains(pd_scope->variables, _tr_strz(name))) {
#line 5373 "src/sema.tr"
                    Symbol* pd_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pd_scope->variables, _tr_strz(name)));
#line 5374 "src/sema.tr"
                    pd_sym->is_init = false;
#line 5375 "src/sema.tr"
                    _tr_dict_set(pd_scope->variables, _tr_strz(name), pd_sym);
                }
            }
        }
#line 5382 "src/sema.tr"
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
#line 5383 "src/sema.tr"
            TrStr pc_cont_nm = _tr_str_lit_len("", 0LL);
#line 5384 "src/sema.tr"
            TrStr pc_meth = _tr_str_lit_len("", 0LL);
#line 5385 "src/sema.tr"
            __auto_type _t1509 = (*val_ptr);
            if (_t1509.tag == Expr_EMethodCall) {
                __auto_type pc_obj = _t1509.data.EMethodCall.obj;
__auto_type pc_m = _t1509.data.EMethodCall.method;
#line 5387 "src/sema.tr"
                TrStr _strtmp_t1510 = _tr_str_retain(pc_m);
                _tr_str_release(pc_meth);
                pc_meth = _strtmp_t1510;
#line 5388 "src/sema.tr"
                __auto_type _t1511 = (*pc_obj);
                if (_t1511.tag == Expr_EIdent) {
                    __auto_type pc_src = _t1511.data.EIdent.name;
                    TrStr _strtmp_t1512 = _tr_str_retain(pc_src);
                    _tr_str_release(pc_cont_nm);
                    pc_cont_nm = _strtmp_t1512;
                } else if (1) {
                    __auto_type _ = _t1511;
                    /* pass */
                }
            } else if (_t1509.tag == Expr_EIndex) {
                __auto_type pc_iobj = _t1509.data.EIndex.obj;
#line 5392 "src/sema.tr"
                TrStr _strtmp_t1513 = _tr_str_lit_len("get", 3LL);
                _tr_str_release(pc_meth);
                pc_meth = _strtmp_t1513;
#line 5393 "src/sema.tr"
                __auto_type _t1514 = (*pc_iobj);
                if (_t1514.tag == Expr_EIdent) {
                    __auto_type pc_isrc = _t1514.data.EIdent.name;
                    TrStr _strtmp_t1515 = _tr_str_retain(pc_isrc);
                    _tr_str_release(pc_cont_nm);
                    pc_cont_nm = _strtmp_t1515;
                } else if (1) {
                    __auto_type _ = _t1514;
                    /* pass */
                }
            } else if (1) {
                __auto_type _ = _t1509;
                /* pass */
            }
#line 5403 "src/sema.tr"
            if ((_tr_str_eqv((pc_cont_nm), (_tr_str_lit_len("", 0LL))) && ((_tr_str_eqv((pc_meth), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("last", 4LL)))))) {
#line 5404 "src/sema.tr"
                __auto_type _t1516 = (*hval);
                if (_t1516.tag == HirExpr_EMethodCall) {
                    __auto_type pc_ho = _t1516.data.EMethodCall.obj;
#line 5406 "src/sema.tr"
                    TrStr pc_hn = _tr_str_retain(hir_expr_type(pc_ho)->name);
#line 5407 "src/sema.tr"
                    if (((((_tr_str_eqv((pc_hn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((pc_hn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((pc_hn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pc_hn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((pc_hn), (_tr_str_lit_len("Set", 3LL))))) {
#line 5408 "src/sema.tr"
                        TrStr pc_fe = _tr_str_retain(hir_expr_type(hval)->name);
#line 5409 "src/sema.tr"
                        if ((((((_tr_str_eqv((pc_fe), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((pc_fe), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((pc_fe), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pc_fe), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((pc_fe), (_tr_str_lit_len("Set", 3LL)))) || Sema_class_has_free(self, pc_fe))) {
#line 5410 "src/sema.tr"
                            Sema_set_borrows_region(self, name, _tr_str_lit_len("@borrowed", 9LL));
                        }
                    }
                    _tr_str_release(pc_hn);
                } else if (1) {
                    __auto_type _ = _t1516;
                    /* pass */
                }
            }
#line 5412 "src/sema.tr"
            if (((!_tr_str_eqv((pc_cont_nm), (_tr_str_lit_len("", 0LL)))) && ((_tr_str_eqv((pc_meth), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("last", 4LL)))))) {
#line 5413 "src/sema.tr"
                Symbol* pc_cont_sym = Sema_resolve(self, pc_cont_nm);
#line 5414 "src/sema.tr"
                AstType* pc_cont_ty = (*pc_cont_sym->ty);
#line 5415 "src/sema.tr"
                TrStr pc_ctn = _tr_str_retain(pc_cont_ty->name);
#line 5416 "src/sema.tr"
                if ((_tr_str_eqv((pc_ctn), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("RwLock", 6LL))))) {
#line 5422 "src/sema.tr"
                    Sema_set_borrows_region(self, name, _tr_str_lit_len("@borrowed", 9LL));
                } else if (((((_tr_str_eqv((pc_ctn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("Set", 3LL))))) {
#line 5424 "src/sema.tr"
                    TrStr pc_elem_ty = _tr_str_retain(hir_expr_type(hval)->name);
#line 5427 "src/sema.tr"
                    bool pc_is_vt = (_tr_dict_contains(self->classes, _tr_strz(pc_elem_ty)) && (!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(pc_elem_ty)))->is_class));
#line 5428 "src/sema.tr"
                    if (((!Sema_is_primitive(self, hir_expr_type(hval))) && (!pc_is_vt))) {
#line 5429 "src/sema.tr"
                        _tr_dict_set(self->container_borrows, _tr_strz(pc_cont_nm), _tr_str_box(_tr_str_retain(name)));
#line 5433 "src/sema.tr"
                        if (((((((!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("str", 3LL)))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Vec", 3LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Dict", 4LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Map", 3LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Set", 3LL)))))) {
#line 5434 "src/sema.tr"
                            Sema_set_borrows_region(self, name, _tr_str_lit_len("@borrowed", 9LL));
                        }
                    }
                }
            }
        }
#line 5435 "src/sema.tr"
        if (((((unsigned long long)(hval)) != ((unsigned long long)(0LL))) && (!_is_invalid_ptr(((unsigned long long)(hval)))))) {
#line 5436 "src/sema.tr"
            __auto_type _t1517 = (*hval);
            if (_t1517.tag == HirExpr_EClosure) {
#line 5438 "src/sema.tr"
                List_TrStr_append(self->clo_let_names, name);
#line 5439 "src/sema.tr"
                List_ptr_append(self->clo_let_ptrs, hval);
#line 5440 "src/sema.tr"
                List_i64_append(self->clo_let_scopes, (self->scopes->len - 1LL));
            } else if (1) {
                __auto_type _ = _t1517;
                /* pass */
            }
        }
#line 5442 "src/sema.tr"
        hval = Sema_retype_none(self, hval, ty);
#line 5443 "src/sema.tr"
        HirStmt* _trret240 = box_hirstmt(HirStmt_ctor_SLet(name, ownership, is_mut, is_const, is_shared, ty, hval));
        _tr_str_release(_let_firm);
        _tr_str_release(_let_cls);
        return _trret240;
    } else if (_t1469.tag == Stmt_SAssign) {
        __auto_type target = _t1469.data.SAssign.target;
__auto_type val = _t1469.data.SAssign.val;
#line 5449 "src/sema.tr"
        if (((((unsigned long long)(target)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
#line 5450 "src/sema.tr"
            __auto_type _t1518 = (*target);
            if (_t1518.tag == Expr_EIdent) {
                __auto_type cl_decl_name = _t1518.data.EIdent.name;
#line 5452 "src/sema.tr"
                if (_tr_str_eqv((Sema_resolve(self, cl_decl_name)->name), (_tr_str_lit_len("", 0LL)))) {
#line 5453 "src/sema.tr"
                    __auto_type _t1519 = (*val);
                    if (_t1519.tag == Expr_EClosure) {
#line 5455 "src/sema.tr"
                        HirStmt* _trret241 = ({ __auto_type _ord_t1520 = (Ownership_make_Own()); __auto_type _ord_t1521 = ((AstType**)(0LL)); __auto_type _wr = (Sema_lower_stmt(self, box_astestmt(Stmt_ctor_SLet(cl_decl_name, _ord_t1520, false, false, false, _ord_t1521, val)))); _wr; });
                        return _trret241;
                    } else if (1) {
                        __auto_type _ = _t1519;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1518;
                /* pass */
            }
        }
#line 5458 "src/sema.tr"
        self->in_assign_target = true;
#line 5459 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5460 "src/sema.tr"
            __auto_type _t1522 = (*target);
            if (_t1522.tag == Expr_EIdent) {
                __auto_type _ = _t1522.data.EIdent.name;
                self->in_assign_target_plain = true;
            } else if (1) {
                __auto_type _ = _t1522;
                /* pass */
            }
        }
#line 5463 "src/sema.tr"
        HirExpr* htgt = Sema_lower_expr(self, target);
#line 5464 "src/sema.tr"
        self->in_assign_target = false;
#line 5465 "src/sema.tr"
        self->in_assign_target_plain = false;
#line 5466 "src/sema.tr"
        HirExpr* hv = Sema_lower_expr(self, val);
#line 5467 "src/sema.tr"
        hv = Sema__patch_empty_dict_hint(self, hv, hir_expr_type(htgt));
#line 5468 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5469 "src/sema.tr"
            __auto_type _t1523 = (*htgt);
            if (_t1523.tag == HirExpr_EPropAccess) {
                __auto_type _fo = _t1523.data.EPropAccess.obj;
__auto_type _fname = _t1523.data.EPropAccess.prop;
#line 5471 "src/sema.tr"
                AstType* _fo_ty = hir_expr_type(_fo);
#line 5472 "src/sema.tr"
                if (((_tr_dict_contains(self->classes, _tr_strz(_fo_ty->name)) && (!_tr_dict_contains(self->ambiguous_classes, _tr_strz(_fo_ty->name)))) && (!Sema_is_builtin_dispatch_class(self, _fo_ty->name)))) {
#line 5473 "src/sema.tr"
                    ClassDef* _fo_cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_fo_ty->name)));
#line 5474 "src/sema.tr"
                    if ((_fo_cd->generics->len == 0LL)) {
#line 5475 "src/sema.tr"
                        long long _fo_i = 0LL;
#line 5476 "src/sema.tr"
                        while ((_fo_i < _fo_cd->fields->len)) {
#line 5477 "src/sema.tr"
                            if ((_tr_str_eqv((((FieldDef*)List_ptr_get(_fo_cd->fields, _fo_i))->name), (_fname)) && (((unsigned long long)(((FieldDef*)List_ptr_get(_fo_cd->fields, _fo_i))->ty)) != ((unsigned long long)(0LL))))) {
#line 5478 "src/sema.tr"
                                ({ __auto_type _ord_t1524 = ((*((FieldDef*)List_ptr_get(_fo_cd->fields, _fo_i))->ty)); TrStr _ord_t1525 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the field '", 11LL)), (_fname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_fo_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1524, hv, _ord_t1525); _tr_str_release(_ord_t1525); });
                            }
#line 5479 "src/sema.tr"
                            _fo_i = (_fo_i + 1LL);
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1523;
                /* pass */
            }
#line 5481 "src/sema.tr"
            __auto_type _t1526 = (*target);
            if (_t1526.tag == Expr_EIdent) {
                __auto_type _as_n = _t1526.data.EIdent.name;
#line 5483 "src/sema.tr"
                Symbol* _as_sym = Sema_resolve(self, _as_n);
#line 5484 "src/sema.tr"
                if ((((!_tr_str_eqv((_as_sym->name), (_tr_str_lit_len("", 0LL)))) && (_as_sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!_tr_str_eqv((_as_sym->firm), (_tr_str_lit_len("", 0LL)))))) {
#line 5485 "src/sema.tr"
                    TrStr _as_af = Sema_firm_family(self, hv);
#line 5486 "src/sema.tr"
                    if (Sema_family_mismatch(self, _as_sym->firm, _as_af)) {
#line 5487 "src/sema.tr"
                        ({ __auto_type _ord_t1527 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (_as_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' holds ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _as_sym->firm)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but is assigned ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1528 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1527); TrStr _tr_cr = (Sema_family_word(self, _as_af)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: assign a value of the same type, or declare a new variable.", 72LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1528); _tr_str_release(_at_t1528); });
                    }
                }
            } else if (1) {
                __auto_type _ = _t1526;
                /* pass */
            }
        }
#line 5491 "src/sema.tr"
        if ((self->strict_mode && (((unsigned long long)(target)) != ((unsigned long long)(0LL))))) {
#line 5492 "src/sema.tr"
            __auto_type _t1529 = (*target);
            if (_t1529.tag == Expr_EPropAccess) {
                __auto_type l5_obj = _t1529.data.EPropAccess.obj;
__auto_type l5_field = _t1529.data.EPropAccess.prop;
#line 5494 "src/sema.tr"
                if ((({ TrStr _cmpa_t1530 = (Sema_compute_region(self, val)); TrStr _cmpb_t1531 = (_tr_str_lit_len("@owned", 6LL)); _Bool _cmpr_t1532 = _tr_str_eqv(_cmpa_t1530, _cmpb_t1531); _tr_str_release(_cmpa_t1530); _cmpr_t1532; }) && Sema_field_is_borrow(self, l5_obj, l5_field))) {
#line 5495 "src/sema.tr"
                    ({ TrStr _at_t1533 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-5] storing a freshly-built (owned) string into the borrow field '", 68LL)), (l5_field))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (declared 'ref').\n      FIX: store a borrow of the field's region, or make the field a plain owned 'str'.", 107LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1533); _tr_str_release(_at_t1533); });
                }
            } else if (1) {
                __auto_type _ = _t1529;
                /* pass */
            }
        }
#line 5500 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5501 "src/sema.tr"
            __auto_type _t1534 = (*target);
            if (_t1534.tag == Expr_EIdent) {
                __auto_type sa_decl_name = _t1534.data.EIdent.name;
#line 5506 "src/sema.tr"
                ({ TrStr _at_t1535 = (Sema_compute_region(self, val)); Sema_set_borrows_region(self, sa_decl_name, _at_t1535); _tr_str_release(_at_t1535); });
#line 5507 "src/sema.tr"
                Symbol* sa_existing = Sema_resolve(self, sa_decl_name);
#line 5508 "src/sema.tr"
                if (_tr_str_eqv((sa_existing->name), (_tr_str_lit_len("", 0LL)))) {
#line 5511 "src/sema.tr"
                    ({ __auto_type _ord_t1536 = (SymbolKind_make_SVariable()); __auto_type _ord_t1537 = (box_asttype(hir_expr_type(hv))); Sema_declare(self, sa_decl_name, _ord_t1536, _ord_t1537, false); });
#line 5515 "src/sema.tr"
                    if ((_tr_str_eqv((hir_expr_type(htgt)->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((hir_expr_type(htgt)->name), (_tr_str_lit_len("", 0LL))))) {
#line 5516 "src/sema.tr"
                        htgt = box_hirexpr(HirExpr_ctor_EIdent(sa_decl_name, hir_expr_type(hv), false));
                    }
                } else if ((((self->scopes->len > 0LL) && Sema_is_global_not_local(self, sa_decl_name)) && (!sa_existing->is_mut))) {
#line 5525 "src/sema.tr"
                    ({ __auto_type _ord_t1538 = (SymbolKind_make_SVariable()); __auto_type _ord_t1539 = (box_asttype(hir_expr_type(hv))); Sema_declare(self, sa_decl_name, _ord_t1538, _ord_t1539, true); });
#line 5526 "src/sema.tr"
                    Sema_mark_init(self, sa_decl_name);
#line 5527 "src/sema.tr"
                    HirStmt* _trret242 = ({ __auto_type _ord_t1540 = (Ownership_make_Own()); __auto_type _ord_t1541 = (hir_expr_type(hv)); __auto_type _wr = (box_hirstmt(HirStmt_ctor_SLet(sa_decl_name, _ord_t1540, true, false, false, _ord_t1541, hv))); _wr; });
                    return _trret242;
                } else if (((sa_existing->kind.tag == SymbolKind_make_SVariable().tag) && (!sa_existing->is_mut))) {
#line 5529 "src/sema.tr"
                    ({ TrStr _at_t1542 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-8] Cannot assign to '", 24LL)), (sa_decl_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' a second time because it is immutable.\n      FIX: Declare it as 'mut ", 71LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sa_decl_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ...' if it needs to change.", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1542); _tr_str_release(_at_t1542); });
                }
            } else if (1) {
                __auto_type _ = _t1534;
                /* pass */
            }
        }
#line 5544 "src/sema.tr"
        bool sa_target_is_plain_ident = false;
#line 5545 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5546 "src/sema.tr"
            __auto_type _t1543 = (*target);
            if (_t1543.tag == Expr_EIdent) {
                __auto_type _ = _t1543.data.EIdent.name;
                sa_target_is_plain_ident = true;
            } else if (1) {
                __auto_type _ = _t1543;
                /* pass */
            }
        }
#line 5549 "src/sema.tr"
        if ((sa_target_is_plain_ident && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
#line 5550 "src/sema.tr"
            __auto_type _t1544 = (*val);
            if (_t1544.tag == Expr_EIdent) {
                __auto_type sa_src = _t1544.data.EIdent.name;
#line 5552 "src/sema.tr"
                Symbol* sa_sym = Sema_resolve(self, sa_src);
#line 5553 "src/sema.tr"
                AstType* sa_ty = (*sa_sym->ty);
#line 5554 "src/sema.tr"
                bool sa_is_known = (_tr_dict_contains(self->classes, _tr_strz(sa_ty->name)) || _tr_dict_contains(self->enums, _tr_strz(sa_ty->name)));
#line 5555 "src/sema.tr"
                if (((((sa_is_known && (!Sema_is_primitive(self, sa_ty))) && (!Sema_is_copy_class(self, sa_ty->name))) && (!_tr_str_eqv((sa_sym->name), (_tr_str_lit_len("", 0LL))))) && (!sa_sym->is_shared))) {
#line 5556 "src/sema.tr"
                    Sema_check_no_active_borrows(self, sa_src, sa_ty->name);
#line 5557 "src/sema.tr"
                    Sema_mark_moved(self, sa_src);
                } else if ((((sa_is_known && Sema_class_has_free(self, sa_ty->name)) && (!_tr_str_eqv((sa_sym->name), (_tr_str_lit_len("", 0LL))))) && (!sa_sym->is_shared))) {
#line 5559 "src/sema.tr"
                    Sema_mark_moved(self, sa_src);
                }
            } else if (1) {
                __auto_type _ = _t1544;
                /* pass */
            }
        }
#line 5562 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5563 "src/sema.tr"
            __auto_type _t1545 = (*target);
            if (_t1545.tag == Expr_EIdent) {
                __auto_type pd_tgt = _t1545.data.EIdent.name;
#line 5565 "src/sema.tr"
                Sema_mark_init(self, pd_tgt);
#line 5566 "src/sema.tr"
                Sema_revive_moved(self, pd_tgt);
            } else if (1) {
                __auto_type _ = _t1545;
                /* pass */
            }
        }
#line 5569 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5570 "src/sema.tr"
            __auto_type _t1546 = (*target);
            if (_t1546.tag == Expr_EIdent) {
                __auto_type pc_tgt = _t1546.data.EIdent.name;
                Sema_clear_container_borrow(self, pc_tgt);
            } else if (1) {
                __auto_type _ = _t1546;
                /* pass */
            }
        }
#line 5573 "src/sema.tr"
        hv = Sema_retype_none(self, hv, hir_expr_type(htgt));
#line 5574 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SAssign(htgt, hv));
    } else if (_t1469.tag == Stmt_SIf) {
        __auto_type cond = _t1469.data.SIf.cond;
__auto_type then_b = _t1469.data.SIf.then_b;
__auto_type elifs = _t1469.data.SIf.elifs;
__auto_type else_b = _t1469.data.SIf.else_b;
#line 5577 "src/sema.tr"
        if ((((unsigned long long)(cond)) != ((unsigned long long)(0LL)))) {
#line 5578 "src/sema.tr"
            __auto_type _t1547 = (*cond);
            if (_t1547.tag == Expr_EIdent) {
                __auto_type t5_name = _t1547.data.EIdent.name;
#line 5580 "src/sema.tr"
                Symbol* t5_sym = Sema_resolve(self, t5_name);
#line 5581 "src/sema.tr"
                TrStr t5_ty = _tr_str_retain((*t5_sym->ty)->name);
#line 5582 "src/sema.tr"
                if (((((_tr_str_eqv((t5_ty), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("f64", 3LL))))) {
#line 5583 "src/sema.tr"
                    ({ TrStr _at_t1548 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-5] '", 7LL)), (t5_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a number (", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t5_ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") and cannot be used as an 'if' condition. FIX: Write 'if ", 58LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t5_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" != 0:' to explicitly check for non-zero.", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1548); _tr_str_release(_at_t1548); });
                }
                _tr_str_release(t5_ty);
            } else if (1) {
                __auto_type _ = _t1547;
                /* pass */
            }
        }
#line 5586 "src/sema.tr"
        bool si_outer_cap_m = self->capturing_moves;
#line 5587 "src/sema.tr"
        List_TrStr* si_outer_buf_m = self->branch_moved_buf;
#line 5588 "src/sema.tr"
        bool si_outer_cap_i = self->capturing_inits;
#line 5589 "src/sema.tr"
        List_TrStr* si_outer_buf_i = self->branch_init_buf;
#line 5591 "src/sema.tr"
        self->capturing_moves = true;
#line 5592 "src/sema.tr"
        self->capturing_inits = true;
#line 5593 "src/sema.tr"
        self->branch_moved_buf = (void*)List_TrStr_new();
#line 5594 "src/sema.tr"
        self->branch_init_buf = (void*)List_TrStr_new();
#line 5595 "src/sema.tr"
        HirExpr* hcond = Sema_lower_expr(self, cond);
#line 5596 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5597 "src/sema.tr"
        Sema_open_block(self);
#line 5598 "src/sema.tr"
        HirBlock* hthen = Sema_lower_block(self, then_b);
#line 5599 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5600 "src/sema.tr"
        Sema_close_block(self);
#line 5601 "src/sema.tr"
        List_TrStr* si_then_moved = self->branch_moved_buf;
#line 5602 "src/sema.tr"
        List_TrStr* si_then_inited = self->branch_init_buf;
#line 5604 "src/sema.tr"
        long long si_uti = 0LL;
#line 5605 "src/sema.tr"
        while ((si_uti < si_then_moved->len)) {
#line 5606 "src/sema.tr"
            ({ TrStr _at_t1549 = (List_TrStr_get(si_then_moved, si_uti)); Sema_unmark_moved(self, _at_t1549); _tr_str_release(_at_t1549); });
#line 5607 "src/sema.tr"
            si_uti = (si_uti + 1LL);
        }
#line 5608 "src/sema.tr"
        long long si_uii = 0LL;
#line 5609 "src/sema.tr"
        while ((si_uii < si_then_inited->len)) {
#line 5610 "src/sema.tr"
            ({ TrStr _at_t1550 = (List_TrStr_get(si_then_inited, si_uii)); Sema_unmark_init(self, _at_t1550); _tr_str_release(_at_t1550); });
#line 5611 "src/sema.tr"
            si_uii = (si_uii + 1LL);
        }
#line 5613 "src/sema.tr"
        self->branch_moved_buf = (void*)List_TrStr_new();
#line 5614 "src/sema.tr"
        self->branch_init_buf = (void*)List_TrStr_new();
#line 5615 "src/sema.tr"
        HirBlock* helse = HirBlock_init();
#line 5616 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5617 "src/sema.tr"
        Sema_open_block(self);
#line 5618 "src/sema.tr"
        if ((elifs->len > 0LL)) {
#line 5628 "src/sema.tr"
            Sema_open_block(self);
#line 5629 "src/sema.tr"
            HirBlock* base_else = Sema_lower_block(self, else_b);
#line 5630 "src/sema.tr"
            Sema_close_block(self);
#line 5631 "src/sema.tr"
            HirBlock* chain = _tr_obj_retain(base_else);
#line 5632 "src/sema.tr"
            long long k = (elifs->len - 1LL);
#line 5633 "src/sema.tr"
            while ((k >= 0LL)) {
#line 5634 "src/sema.tr"
                ElifClause* elif_c = ((ElifClause*)List_ptr_get(elifs, k));
#line 5635 "src/sema.tr"
                HirExpr* elif_cond = Sema_lower_expr(self, elif_c->cond);
#line 5636 "src/sema.tr"
                Sema_open_block(self);
#line 5637 "src/sema.tr"
                HirBlock* elif_body = Sema_lower_block(self, (*elif_c->body));
#line 5638 "src/sema.tr"
                Sema_close_block(self);
#line 5639 "src/sema.tr"
                HirBlock* nested = HirBlock_init();
#line 5640 "src/sema.tr"
                HirBlock_push(nested, box_hirstmt(HirStmt_ctor_SIf(elif_cond, elif_body, chain)));
#line 5641 "src/sema.tr"
                HirBlock* _cltmp_t1551 = _tr_obj_retain(nested);
                _tr_obj_release(chain, _trdrop_HirBlock);
                chain = _cltmp_t1551;
#line 5642 "src/sema.tr"
                k = (k - 1LL);
                _tr_obj_release(elif_body, _trdrop_HirBlock);
                _tr_obj_release(nested, _trdrop_HirBlock);
            }
#line 5643 "src/sema.tr"
            HirBlock* _cltmp_t1552 = _tr_obj_retain(chain);
            _tr_obj_release(helse, _trdrop_HirBlock);
            helse = _cltmp_t1552;
            _tr_obj_release(base_else, _trdrop_HirBlock);
        } else {
#line 5645 "src/sema.tr"
            HirBlock* _cltmp_t1553 = Sema_lower_block(self, else_b);
            _tr_obj_release(helse, _trdrop_HirBlock);
            helse = _cltmp_t1553;
        }
#line 5646 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5647 "src/sema.tr"
        Sema_close_block(self);
#line 5648 "src/sema.tr"
        List_TrStr* si_else_moved = self->branch_moved_buf;
#line 5649 "src/sema.tr"
        List_TrStr* si_else_inited = self->branch_init_buf;
#line 5651 "src/sema.tr"
        long long si_uei = 0LL;
#line 5652 "src/sema.tr"
        while ((si_uei < si_else_moved->len)) {
#line 5653 "src/sema.tr"
            ({ TrStr _at_t1554 = (List_TrStr_get(si_else_moved, si_uei)); Sema_unmark_moved(self, _at_t1554); _tr_str_release(_at_t1554); });
#line 5654 "src/sema.tr"
            si_uei = (si_uei + 1LL);
        }
#line 5655 "src/sema.tr"
        long long si_uei2 = 0LL;
#line 5656 "src/sema.tr"
        while ((si_uei2 < si_else_inited->len)) {
#line 5657 "src/sema.tr"
            ({ TrStr _at_t1555 = (List_TrStr_get(si_else_inited, si_uei2)); Sema_unmark_init(self, _at_t1555); _tr_str_release(_at_t1555); });
#line 5658 "src/sema.tr"
            si_uei2 = (si_uei2 + 1LL);
        }
#line 5660 "src/sema.tr"
        self->capturing_moves = si_outer_cap_m;
#line 5661 "src/sema.tr"
        self->branch_moved_buf = si_outer_buf_m;
#line 5662 "src/sema.tr"
        self->capturing_inits = si_outer_cap_i;
#line 5663 "src/sema.tr"
        self->branch_init_buf = si_outer_buf_i;
#line 5669 "src/sema.tr"
        bool si_then_jumps = Sema_block_ends_in_jump(self, hthen);
#line 5670 "src/sema.tr"
        bool si_else_jumps = Sema_block_ends_in_jump(self, helse);
#line 5671 "src/sema.tr"
        if ((si_then_jumps && (!si_else_jumps))) {
#line 5672 "src/sema.tr"
            long long si_ei0 = 0LL;
#line 5673 "src/sema.tr"
            while ((si_ei0 < si_else_moved->len)) {
#line 5674 "src/sema.tr"
                ({ TrStr _at_t1556 = (List_TrStr_get(si_else_moved, si_ei0)); Sema_mark_moved(self, _at_t1556); _tr_str_release(_at_t1556); });
#line 5675 "src/sema.tr"
                si_ei0 = (si_ei0 + 1LL);
            }
#line 5676 "src/sema.tr"
            long long si_ini0 = 0LL;
#line 5677 "src/sema.tr"
            while ((si_ini0 < si_else_inited->len)) {
#line 5678 "src/sema.tr"
                ({ TrStr _at_t1557 = (List_TrStr_get(si_else_inited, si_ini0)); Sema_mark_init(self, _at_t1557); _tr_str_release(_at_t1557); });
#line 5679 "src/sema.tr"
                si_ini0 = (si_ini0 + 1LL);
            }
        } else if ((si_else_jumps && (!si_then_jumps))) {
#line 5681 "src/sema.tr"
            long long si_mi0 = 0LL;
#line 5682 "src/sema.tr"
            while ((si_mi0 < si_then_moved->len)) {
#line 5683 "src/sema.tr"
                ({ TrStr _at_t1558 = (List_TrStr_get(si_then_moved, si_mi0)); Sema_mark_moved(self, _at_t1558); _tr_str_release(_at_t1558); });
#line 5684 "src/sema.tr"
                si_mi0 = (si_mi0 + 1LL);
            }
#line 5685 "src/sema.tr"
            long long si_ti0 = 0LL;
#line 5686 "src/sema.tr"
            while ((si_ti0 < si_then_inited->len)) {
#line 5687 "src/sema.tr"
                ({ TrStr _at_t1559 = (List_TrStr_get(si_then_inited, si_ti0)); Sema_mark_init(self, _at_t1559); _tr_str_release(_at_t1559); });
#line 5688 "src/sema.tr"
                si_ti0 = (si_ti0 + 1LL);
            }
        } else if ((si_then_jumps && si_else_jumps)) {
#line 5690 "src/sema.tr"
            /* pass */
        } else {
#line 5693 "src/sema.tr"
            long long si_mi = 0LL;
#line 5694 "src/sema.tr"
            while ((si_mi < si_then_moved->len)) {
#line 5695 "src/sema.tr"
                TrStr si_mn = List_TrStr_get(si_then_moved, si_mi);
#line 5696 "src/sema.tr"
                if (Sema_vec_str_contains(self, si_else_moved, si_mn)) {
#line 5697 "src/sema.tr"
                    Sema_mark_moved(self, si_mn);
                } else {
#line 5699 "src/sema.tr"
                    Sema_mark_maybe_moved(self, si_mn);
                }
#line 5700 "src/sema.tr"
                si_mi = (si_mi + 1LL);
                _tr_str_release(si_mn);
            }
#line 5701 "src/sema.tr"
            long long si_ei = 0LL;
#line 5702 "src/sema.tr"
            while ((si_ei < si_else_moved->len)) {
#line 5703 "src/sema.tr"
                TrStr si_en = List_TrStr_get(si_else_moved, si_ei);
#line 5704 "src/sema.tr"
                if ((!Sema_vec_str_contains(self, si_then_moved, si_en))) {
#line 5705 "src/sema.tr"
                    Sema_mark_maybe_moved(self, si_en);
                }
#line 5706 "src/sema.tr"
                si_ei = (si_ei + 1LL);
                _tr_str_release(si_en);
            }
#line 5708 "src/sema.tr"
            long long si_ini = 0LL;
#line 5709 "src/sema.tr"
            while ((si_ini < si_then_inited->len)) {
#line 5710 "src/sema.tr"
                TrStr si_inn = List_TrStr_get(si_then_inited, si_ini);
#line 5711 "src/sema.tr"
                if (Sema_vec_str_contains(self, si_else_inited, si_inn)) {
#line 5712 "src/sema.tr"
                    Sema_mark_init(self, si_inn);
                } else {
#line 5714 "src/sema.tr"
                    Sema_mark_maybe_init(self, si_inn);
                }
#line 5715 "src/sema.tr"
                si_ini = (si_ini + 1LL);
                _tr_str_release(si_inn);
            }
        }
#line 5716 "src/sema.tr"
        HirStmt* _trret243 = box_hirstmt(HirStmt_ctor_SIf(hcond, hthen, helse));
        _tr_obj_release(hthen, _trdrop_HirBlock);
        _tr_obj_release(helse, _trdrop_HirBlock);
        return _trret243;
    } else if (_t1469.tag == Stmt_SWhile) {
        __auto_type cond = _t1469.data.SWhile.cond;
__auto_type body = _t1469.data.SWhile.body;
__auto_type decorators = _t1469.data.SWhile.decorators;
#line 5718 "src/sema.tr"
        HirExpr* sw_cond = Sema_lower_expr(self, cond);
#line 5720 "src/sema.tr"
        bool sw_outer_cap_m = self->capturing_moves;
#line 5721 "src/sema.tr"
        List_TrStr* sw_outer_buf_m = self->branch_moved_buf;
#line 5722 "src/sema.tr"
        bool sw_outer_cap_i = self->capturing_inits;
#line 5723 "src/sema.tr"
        List_TrStr* sw_outer_buf_i = self->branch_init_buf;
#line 5724 "src/sema.tr"
        self->capturing_moves = true;
#line 5725 "src/sema.tr"
        self->capturing_inits = true;
#line 5726 "src/sema.tr"
        self->branch_moved_buf = (void*)List_TrStr_new();
#line 5727 "src/sema.tr"
        self->branch_init_buf = (void*)List_TrStr_new();
#line 5731 "src/sema.tr"
        List_i64_append(self->loop_scope_base, self->scopes->len);
#line 5732 "src/sema.tr"
        List_i64_append(self->loop_blk_floor, self->block_stack->len);
#line 5733 "src/sema.tr"
        List_i64_append(self->loop_blk_scope, (self->scopes->len - 1LL));
#line 5734 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5735 "src/sema.tr"
        Sema_open_block(self);
#line 5736 "src/sema.tr"
        long long sw_block_id = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
#line 5737 "src/sema.tr"
        HirBlock* sw_body = Sema_lower_block(self, body);
#line 5738 "src/sema.tr"
        Sema_append_block_local_drops(self, sw_body, sw_block_id);
#line 5739 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5740 "src/sema.tr"
        Sema_close_block(self);
#line 5741 "src/sema.tr"
        List_i64_pop(self->loop_scope_base);
#line 5742 "src/sema.tr"
        List_i64_pop(self->loop_blk_floor);
#line 5743 "src/sema.tr"
        List_i64_pop(self->loop_blk_scope);
#line 5744 "src/sema.tr"
        List_TrStr* sw_loop_moved = self->branch_moved_buf;
#line 5745 "src/sema.tr"
        List_TrStr* sw_loop_inited = self->branch_init_buf;
#line 5746 "src/sema.tr"
        self->capturing_moves = sw_outer_cap_m;
#line 5747 "src/sema.tr"
        self->branch_moved_buf = sw_outer_buf_m;
#line 5748 "src/sema.tr"
        self->capturing_inits = sw_outer_cap_i;
#line 5749 "src/sema.tr"
        self->branch_init_buf = sw_outer_buf_i;
#line 5750 "src/sema.tr"
        long long sw_mi = 0LL;
#line 5751 "src/sema.tr"
        while ((sw_mi < sw_loop_moved->len)) {
#line 5752 "src/sema.tr"
            ({ TrStr _at_t1560 = (List_TrStr_get(sw_loop_moved, sw_mi)); Sema_unmark_moved(self, _at_t1560); _tr_str_release(_at_t1560); });
#line 5753 "src/sema.tr"
            ({ TrStr _at_t1561 = (List_TrStr_get(sw_loop_moved, sw_mi)); Sema_mark_maybe_moved(self, _at_t1561); _tr_str_release(_at_t1561); });
#line 5754 "src/sema.tr"
            sw_mi = (sw_mi + 1LL);
        }
#line 5755 "src/sema.tr"
        long long sw_ii = 0LL;
#line 5756 "src/sema.tr"
        while ((sw_ii < sw_loop_inited->len)) {
#line 5757 "src/sema.tr"
            ({ TrStr _at_t1562 = (List_TrStr_get(sw_loop_inited, sw_ii)); Sema_unmark_init(self, _at_t1562); _tr_str_release(_at_t1562); });
#line 5758 "src/sema.tr"
            ({ TrStr _at_t1563 = (List_TrStr_get(sw_loop_inited, sw_ii)); Sema_mark_maybe_init(self, _at_t1563); _tr_str_release(_at_t1563); });
#line 5759 "src/sema.tr"
            sw_ii = (sw_ii + 1LL);
        }
#line 5760 "src/sema.tr"
        HirStmt* _trret244 = box_hirstmt(HirStmt_ctor_SWhile(sw_cond, sw_body));
        _tr_obj_release(sw_body, _trdrop_HirBlock);
        return _trret244;
    } else if (_t1469.tag == Stmt_SFor) {
        __auto_type var = _t1469.data.SFor.var;
__auto_type iter = _t1469.data.SFor.iter;
__auto_type body = _t1469.data.SFor.body;
__auto_type decorators = _t1469.data.SFor.decorators;
__auto_type for_is_ref = _t1469.data.SFor.is_ref;
#line 5762 "src/sema.tr"
        Sema_enter_scope(self);
#line 5763 "src/sema.tr"
        HirExpr* h_iter_for = Sema_lower_expr(self, iter);
#line 5764 "src/sema.tr"
        AstType* var_ty_for = AstType_init(_tr_str_lit_len("int", 3LL));
#line 5765 "src/sema.tr"
        TrStr iter_hn = _tr_str_retain(hir_expr_type(h_iter_for)->name);
#line 5766 "src/sema.tr"
        long long iter_hal = hir_expr_type(h_iter_for)->args->len;
#line 5767 "src/sema.tr"
        if (((_tr_str_eqv((iter_hn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((iter_hn), (_tr_str_lit_len("Vec", 3LL)))) && (iter_hal > 0LL))) {
#line 5768 "src/sema.tr"
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if ((_tr_str_eqv((iter_hn), (_tr_str_lit_len("Chan", 4LL))) && (iter_hal > 0LL))) {
#line 5770 "src/sema.tr"
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if (_tr_str_eqv((iter_hn), (_tr_str_lit_len("str", 3LL)))) {
#line 5772 "src/sema.tr"
            var_ty_for = AstType_init(_tr_str_lit_len("char", 4LL));
        }
#line 5773 "src/sema.tr"
        ({ __auto_type _ord_t1564 = (SymbolKind_make_SVariable()); __auto_type _ord_t1565 = (box_asttype(var_ty_for)); Sema_declare(self, var, _ord_t1564, _ord_t1565, false); });
#line 5778 "src/sema.tr"
        if (((_tr_str_eqv((iter_hn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((iter_hn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((iter_hn), (_tr_str_lit_len("Set", 3LL))))) {
#line 5779 "src/sema.tr"
            TrStr lv_n = _tr_str_retain(var_ty_for->name);
#line 5780 "src/sema.tr"
            if (((((_tr_str_eqv((lv_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((lv_n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((lv_n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((lv_n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((lv_n), (_tr_str_lit_len("Set", 3LL))))) {
#line 5781 "src/sema.tr"
                Sema_set_borrows_region(self, var, _tr_str_lit_len("@borrowed", 9LL));
            }
        }
#line 5785 "src/sema.tr"
        if (for_is_ref) {
#line 5786 "src/sema.tr"
            __auto_type _t1566 = (*iter);
            if (_t1566.tag == Expr_EIdent) {
                __auto_type for_coll = _t1566.data.EIdent.name;
#line 5788 "src/sema.tr"
                List_TrStr_append(self->cur_func_borrowers, var);
#line 5789 "src/sema.tr"
                List_TrStr_append(self->cur_func_sources, for_coll);
            } else if (1) {
                __auto_type _ = _t1566;
                /* pass */
            }
        }
#line 5792 "src/sema.tr"
        bool sf_outer_cap_m = self->capturing_moves;
#line 5793 "src/sema.tr"
        List_TrStr* sf_outer_buf_m = self->branch_moved_buf;
#line 5794 "src/sema.tr"
        bool sf_outer_cap_i = self->capturing_inits;
#line 5795 "src/sema.tr"
        List_TrStr* sf_outer_buf_i = self->branch_init_buf;
#line 5796 "src/sema.tr"
        self->capturing_moves = true;
#line 5797 "src/sema.tr"
        self->capturing_inits = true;
#line 5798 "src/sema.tr"
        self->branch_moved_buf = (void*)List_TrStr_new();
#line 5799 "src/sema.tr"
        self->branch_init_buf = (void*)List_TrStr_new();
#line 5800 "src/sema.tr"
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
#line 5801 "src/sema.tr"
        List_i64_append(self->loop_blk_floor, (-1LL));
#line 5802 "src/sema.tr"
        List_i64_append(self->loop_blk_scope, (-1LL));
#line 5803 "src/sema.tr"
        HirBlock* sf_body = Sema_lower_block(self, body);
#line 5804 "src/sema.tr"
        List_i64_pop(self->loop_scope_base);
#line 5805 "src/sema.tr"
        List_i64_pop(self->loop_blk_floor);
#line 5806 "src/sema.tr"
        List_i64_pop(self->loop_blk_scope);
#line 5807 "src/sema.tr"
        List_TrStr* sf_loop_moved = self->branch_moved_buf;
#line 5808 "src/sema.tr"
        List_TrStr* sf_loop_inited = self->branch_init_buf;
#line 5809 "src/sema.tr"
        self->capturing_moves = sf_outer_cap_m;
#line 5810 "src/sema.tr"
        self->branch_moved_buf = sf_outer_buf_m;
#line 5811 "src/sema.tr"
        self->capturing_inits = sf_outer_cap_i;
#line 5812 "src/sema.tr"
        self->branch_init_buf = sf_outer_buf_i;
#line 5813 "src/sema.tr"
        long long sf_mi = 0LL;
#line 5814 "src/sema.tr"
        while ((sf_mi < sf_loop_moved->len)) {
#line 5815 "src/sema.tr"
            ({ TrStr _at_t1567 = (List_TrStr_get(sf_loop_moved, sf_mi)); Sema_unmark_moved(self, _at_t1567); _tr_str_release(_at_t1567); });
#line 5816 "src/sema.tr"
            ({ TrStr _at_t1568 = (List_TrStr_get(sf_loop_moved, sf_mi)); Sema_mark_maybe_moved(self, _at_t1568); _tr_str_release(_at_t1568); });
#line 5817 "src/sema.tr"
            sf_mi = (sf_mi + 1LL);
        }
#line 5818 "src/sema.tr"
        long long sf_ii = 0LL;
#line 5819 "src/sema.tr"
        while ((sf_ii < sf_loop_inited->len)) {
#line 5820 "src/sema.tr"
            ({ TrStr _at_t1569 = (List_TrStr_get(sf_loop_inited, sf_ii)); Sema_unmark_init(self, _at_t1569); _tr_str_release(_at_t1569); });
#line 5821 "src/sema.tr"
            ({ TrStr _at_t1570 = (List_TrStr_get(sf_loop_inited, sf_ii)); Sema_mark_maybe_init(self, _at_t1570); _tr_str_release(_at_t1570); });
#line 5822 "src/sema.tr"
            sf_ii = (sf_ii + 1LL);
        }
#line 5823 "src/sema.tr"
        Sema_finalize_scope_drops(self, sf_body);
#line 5824 "src/sema.tr"
        HirStmt* hstmt = box_hirstmt(HirStmt_ctor_SFor(var, h_iter_for, sf_body));
#line 5825 "src/sema.tr"
        Sema_exit_scope(self);
#line 5826 "src/sema.tr"
        HirStmt* _trret245 = hstmt;
        _tr_str_release(iter_hn);
        _tr_obj_release(sf_body, _trdrop_HirBlock);
        return _trret245;
    } else if (_t1469.tag == Stmt_SForUnpack) {
        __auto_type vars = _t1469.data.SForUnpack.vars;
__auto_type iter = _t1469.data.SForUnpack.iter;
__auto_type body = _t1469.data.SForUnpack.body;
#line 5828 "src/sema.tr"
        Sema_enter_scope(self);
#line 5829 "src/sema.tr"
        HirExpr* h_iter_fu = Sema_lower_expr(self, iter);
#line 5831 "src/sema.tr"
        List_ptr* fu_tys = (void*)List_ptr_new();
#line 5832 "src/sema.tr"
        long long fu_ti = 0LL;
#line 5833 "src/sema.tr"
        while ((fu_ti < vars->len)) {
#line 5834 "src/sema.tr"
            List_ptr_append(fu_tys, AstType_init(_tr_str_lit_len("int", 3LL)));
#line 5835 "src/sema.tr"
            fu_ti = (fu_ti + 1LL);
        }
#line 5836 "src/sema.tr"
        __auto_type _t1571 = (*h_iter_fu);
        if (_t1571.tag == HirExpr_ECall) {
            __auto_type fu_callee = _t1571.data.ECall.callee;
__auto_type fu_args = _t1571.data.ECall.args;
#line 5838 "src/sema.tr"
            __auto_type _t1572 = (*fu_callee);
            if (_t1572.tag == HirExpr_EIdent) {
                __auto_type fu_fn = _t1572.data.EIdent.name;
#line 5840 "src/sema.tr"
                if (((_tr_str_eqv((fu_fn), (_tr_str_lit_len("enumerate", 9LL))) && (fu_args->len == 1LL)) && (vars->len >= 2LL))) {
#line 5842 "src/sema.tr"
                    TrStr fu_col_ty_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name);
#line 5843 "src/sema.tr"
                    long long fu_col_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
#line 5844 "src/sema.tr"
                    if (((_tr_str_eqv((fu_col_ty_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fu_col_ty_n), (_tr_str_lit_len("Vec", 3LL)))) && (fu_col_al > 0LL))) {
#line 5845 "src/sema.tr"
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
                } else if (((_tr_str_eqv((fu_fn), (_tr_str_lit_len("zip", 3LL))) && (fu_args->len == 2LL)) && (vars->len >= 2LL))) {
#line 5848 "src/sema.tr"
                    TrStr fu_a_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name);
#line 5849 "src/sema.tr"
                    long long fu_a_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
#line 5850 "src/sema.tr"
                    TrStr fu_b_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->name);
#line 5851 "src/sema.tr"
                    long long fu_b_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args->len;
#line 5852 "src/sema.tr"
                    if (((_tr_str_eqv((fu_a_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fu_a_n), (_tr_str_lit_len("Vec", 3LL)))) && (fu_a_al > 0LL))) {
#line 5853 "src/sema.tr"
                        List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
#line 5854 "src/sema.tr"
                    if (((_tr_str_eqv((fu_b_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fu_b_n), (_tr_str_lit_len("Vec", 3LL)))) && (fu_b_al > 0LL))) {
#line 5855 "src/sema.tr"
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args, 0LL))));
                    }
                }
            } else if (1) {
                __auto_type _ = _t1572;
                /* pass */
            }
        } else if (_t1571.tag == HirExpr_EMethodCall) {
            __auto_type fu_obj = _t1571.data.EMethodCall.obj;
__auto_type fu_meth = _t1571.data.EMethodCall.method;
#line 5858 "src/sema.tr"
            if ((_tr_str_eqv((fu_meth), (_tr_str_lit_len("items", 5LL))) && (vars->len >= 2LL))) {
#line 5859 "src/sema.tr"
                TrStr fu_dty_n = _tr_str_retain(hir_expr_type(fu_obj)->name);
#line 5860 "src/sema.tr"
                long long fu_dty_al = hir_expr_type(fu_obj)->args->len;
#line 5861 "src/sema.tr"
                if (((_tr_str_eqv((fu_dty_n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((fu_dty_n), (_tr_str_lit_len("Map", 3LL)))) && (fu_dty_al >= 2LL))) {
#line 5862 "src/sema.tr"
                    List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 0LL))));
#line 5863 "src/sema.tr"
                    List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 1LL))));
                }
            }
        } else if (1) {
            __auto_type _ = _t1571;
            /* pass */
        }
#line 5865 "src/sema.tr"
        long long vi_fu = 0LL;
#line 5866 "src/sema.tr"
        while ((vi_fu < vars->len)) {
#line 5867 "src/sema.tr"
            ({ TrStr _ord_t1573 = (List_TrStr_get(vars, vi_fu)); __auto_type _ord_t1574 = (SymbolKind_make_SVariable()); __auto_type _ord_t1575 = (box_asttype(((AstType*)List_ptr_get(fu_tys, vi_fu)))); Sema_declare(self, _ord_t1573, _ord_t1574, _ord_t1575, false); _tr_str_release(_ord_t1573); });
#line 5868 "src/sema.tr"
            vi_fu = (vi_fu + 1LL);
        }
#line 5869 "src/sema.tr"
        HirBlock* hblk_fu = Sema_lower_block(self, body);
#line 5870 "src/sema.tr"
        Sema_finalize_scope_drops(self, hblk_fu);
#line 5871 "src/sema.tr"
        Sema_exit_scope(self);
#line 5872 "src/sema.tr"
        HirStmt* _trret246 = box_hirstmt(HirStmt_ctor_SForUnpack(vars, h_iter_fu, hblk_fu));
        List_ptr_free(fu_tys);
        _tr_obj_release(hblk_fu, _trdrop_HirBlock);
        return _trret246;
    } else if (_t1469.tag == Stmt_SMatch) {
        __auto_type subj = _t1469.data.SMatch.expr;
__auto_type arms = _t1469.data.SMatch.arms;
#line 5874 "src/sema.tr"
        HirExpr* hsubj = Sema_lower_expr(self, subj);
#line 5875 "src/sema.tr"
        TrStr ex_ty_name = _tr_str_retain(hir_expr_type(hsubj)->name);
#line 5876 "src/sema.tr"
        List_ptr* h_arms = (void*)List_ptr_new();
#line 5877 "src/sema.tr"
        bool ex_has_wild = false;
#line 5878 "src/sema.tr"
        List_TrStr* ex_covered = (void*)List_TrStr_new();
#line 5879 "src/sema.tr"
        long long k = 0LL;
#line 5880 "src/sema.tr"
        while ((k < arms->len)) {
#line 5881 "src/sema.tr"
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, k));
#line 5884 "src/sema.tr"
            bool ex_guarded = (((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)));
#line 5885 "src/sema.tr"
            __auto_type _t1576 = arm->pat;
            if (_t1576.tag == Pattern_PWild) {
#line 5887 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5887 "src/sema.tr"
                    ex_has_wild = true;
                }
            } else if (_t1576.tag == Pattern_PBind) {
                __auto_type _ = _t1576.data.PBind.name;
#line 5889 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5889 "src/sema.tr"
                    ex_has_wild = true;
                }
            } else if (_t1576.tag == Pattern_PVariant) {
                __auto_type ex_vn = _t1576.data.PVariant.variant;
#line 5891 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5891 "src/sema.tr"
                    List_TrStr_append(ex_covered, ex_vn);
                }
            } else if (_t1576.tag == Pattern_PVariantBind) {
                __auto_type ex_vn2 = _t1576.data.PVariantBind.variant;
#line 5893 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5893 "src/sema.tr"
                    List_TrStr_append(ex_covered, ex_vn2);
                }
            } else if (_t1576.tag == Pattern_PVariantBindMany) {
                __auto_type ex_vn3 = _t1576.data.PVariantBindMany.variant;
#line 5895 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5895 "src/sema.tr"
                    List_TrStr_append(ex_covered, ex_vn3);
                }
            } else if (_t1576.tag == Pattern_POr) {
                __auto_type ex_orpats = _t1576.data.POr.patterns;
#line 5899 "src/sema.tr"
                long long ex_oi = 0LL;
#line 5900 "src/sema.tr"
                while ((ex_oi < ex_orpats->len)) {
#line 5901 "src/sema.tr"
                    __auto_type _t1577 = List_Pattern_get(ex_orpats, ex_oi);
                    if (_t1577.tag == Pattern_PVariant) {
                        __auto_type ex_ovn = _t1577.data.PVariant.variant;
#line 5903 "src/sema.tr"
                        if ((!ex_guarded)) {
#line 5903 "src/sema.tr"
                            List_TrStr_append(ex_covered, ex_ovn);
                        }
                    } else if (_t1577.tag == Pattern_PVariantBind) {
                        __auto_type ex_bvn = _t1577.data.PVariantBind.variant;
#line 5905 "src/sema.tr"
                        if ((!ex_guarded)) {
#line 5905 "src/sema.tr"
                            List_TrStr_append(ex_covered, ex_bvn);
                        }
#line 5906 "src/sema.tr"
                        ({ TrStr _at_t1578 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-3] Payload binding '", 23LL)), (ex_ty_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_bvn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(..)' is not allowed inside an or-pattern ('|').\n      FIX: Split the alternatives into separate 'case' arms, or match the variant without binding: 'case ", 154LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_bvn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ...:'.", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1578); _tr_str_release(_at_t1578); });
                    } else if (_t1577.tag == Pattern_PVariantBindMany) {
                        __auto_type ex_mvn = _t1577.data.PVariantBindMany.variant;
#line 5908 "src/sema.tr"
                        if ((!ex_guarded)) {
#line 5908 "src/sema.tr"
                            List_TrStr_append(ex_covered, ex_mvn);
                        }
#line 5909 "src/sema.tr"
                        ({ TrStr _at_t1579 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-3] Payload binding '", 23LL)), (ex_ty_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_mvn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(..)' is not allowed inside an or-pattern ('|').\n      FIX: Split the alternatives into separate 'case' arms, or match the variant without binding: 'case ", 154LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_mvn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ...:'.", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1579); _tr_str_release(_at_t1579); });
                    } else if (1) {
                        __auto_type _ = _t1577;
                        /* pass */
                    }
#line 5911 "src/sema.tr"
                    ex_oi = (ex_oi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1576;
                /* pass */
            }
#line 5913 "src/sema.tr"
            Sema_enter_scope(self);
#line 5915 "src/sema.tr"
            AstType* _subj_ty = hir_expr_type(hsubj);
#line 5916 "src/sema.tr"
            Sema_declare_pattern_binds_typed(self, arm->pat, _subj_ty);
#line 5917 "src/sema.tr"
            HirMatchArm* h_arm = ({ __auto_type _ord_t1580 = (arm->pat); HirBlock* _ord_t1581 = (Sema_lower_block(self, (*arm->body))); __auto_type _wr = (HirMatchArm_init(_ord_t1580, _ord_t1581)); _tr_obj_release(_ord_t1581, _trdrop_HirBlock); _wr; });
#line 5919 "src/sema.tr"
            if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
#line 5920 "src/sema.tr"
                h_arm->guard = Sema_lower_expr(self, arm->guard);
            }
#line 5921 "src/sema.tr"
            Sema_finalize_scope_drops(self, h_arm->body);
#line 5922 "src/sema.tr"
            List_ptr_append(h_arms, _tr_obj_retain(h_arm));
#line 5923 "src/sema.tr"
            Sema_exit_scope(self);
#line 5924 "src/sema.tr"
            k = (k + 1LL);
            _tr_obj_release(h_arm, _trdrop_HirMatchArm);
        }
#line 5926 "src/sema.tr"
        if (((!ex_has_wild) && _tr_dict_contains(self->enums, _tr_strz(ex_ty_name)))) {
#line 5927 "src/sema.tr"
            EnumDef* ex_edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(ex_ty_name)));
#line 5928 "src/sema.tr"
            List_TrStr* ex_missing = (void*)List_TrStr_new();
#line 5929 "src/sema.tr"
            long long ex_vi = 0LL;
#line 5930 "src/sema.tr"
            while ((ex_vi < ex_edef->variants->len)) {
#line 5931 "src/sema.tr"
                TrStr ex_vname = _tr_str_retain(((VariantDef*)List_ptr_get(ex_edef->variants, ex_vi))->name);
#line 5932 "src/sema.tr"
                if ((!List_TrStr_contains(ex_covered, ex_vname))) {
#line 5933 "src/sema.tr"
                    List_TrStr_append(ex_missing, ex_vname);
                }
#line 5934 "src/sema.tr"
                ex_vi = (ex_vi + 1LL);
                _tr_str_release(ex_vname);
            }
#line 5935 "src/sema.tr"
            if ((ex_missing->len > 0LL)) {
#line 5936 "src/sema.tr"
                TrStr ex_msg = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] Non-exhaustive match on '", 31LL)), (ex_ty_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': missing variant", 18LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 5937 "src/sema.tr"
                if ((ex_missing->len > 1LL)) {
#line 5937 "src/sema.tr"
                    ({ TrStr _ap_t1582 = (_tr_str_lit_len("s", 1LL)); _tr_str_append(&ex_msg, _ap_t1582); });
                }
#line 5938 "src/sema.tr"
                ({ TrStr _ap_t1583 = (_tr_str_lit_len(": ", 2LL)); _tr_str_append(&ex_msg, _ap_t1583); });
#line 5939 "src/sema.tr"
                long long ex_mi = 0LL;
#line 5940 "src/sema.tr"
                while ((ex_mi < ex_missing->len)) {
#line 5941 "src/sema.tr"
                    if ((ex_mi > 0LL)) {
#line 5941 "src/sema.tr"
                        ({ TrStr _ap_t1584 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ex_msg, _ap_t1584); });
                    }
#line 5942 "src/sema.tr"
                    ({ TrStr _ap_t1585 = (List_TrStr_get(ex_missing, ex_mi)); _tr_str_append(&ex_msg, _ap_t1585); _tr_str_release(_ap_t1585); });
#line 5943 "src/sema.tr"
                    ex_mi = (ex_mi + 1LL);
                }
#line 5944 "src/sema.tr"
                ({ TrStr _ap_t1586 = (_tr_str_lit_len(".\n      FIX: Add a 'case ", 25LL)); TrStr _ap_t1587 = (ex_ty_name); TrStr _ap_t1588 = (_tr_str_lit_len(".VariantName:' arm for each missing variant, or add 'case _:' to handle all remaining cases.", 92LL)); _tr_str_append(&ex_msg, _ap_t1586); _tr_str_append(&ex_msg, _ap_t1587); _tr_str_append(&ex_msg, _ap_t1588); });
#line 5945 "src/sema.tr"
                Sema_error(self, ex_msg);
            }
        }
#line 5946 "src/sema.tr"
        HirStmt* _trret247 = box_hirstmt(HirStmt_ctor_SMatch(hsubj, h_arms));
        _tr_str_release(ex_ty_name);
        List_TrStr_free(ex_covered);
        return _trret247;
    } else if (_t1469.tag == Stmt_STry) {
        __auto_type try_body = _t1469.data.STry.try_body;
__auto_type catches = _t1469.data.STry.catches;
__auto_type finally_b = _t1469.data.STry.finally_b;
#line 5948 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5949 "src/sema.tr"
        Sema_open_block(self);
#line 5950 "src/sema.tr"
        HirBlock* h_try_body = Sema_lower_block(self, try_body);
#line 5951 "src/sema.tr"
        List_ptr* h_catches = (void*)List_ptr_new();
#line 5952 "src/sema.tr"
        long long k = 0LL;
#line 5953 "src/sema.tr"
        while ((k < catches->len)) {
#line 5954 "src/sema.tr"
            CatchClause* cc = (*((CatchClause**)List_ptr_get(catches, k)));
#line 5955 "src/sema.tr"
            HirCatchClause* hcc_val = ((HirCatchClause*)_tr_obj_alloc(sizeof(HirCatchClause)));
#line 5956 "src/sema.tr"
            hcc_val->err_name = _tr_str_retain(cc->err_name);
#line 5957 "src/sema.tr"
            hcc_val->err_type = AstType_init(_tr_str_lit_len("str", 3LL));
#line 5958 "src/sema.tr"
            if ((((unsigned long long)(cc->err_type)) != ((unsigned long long)(0LL)))) {
#line 5958 "src/sema.tr"
                hcc_val->err_type = (*cc->err_type);
            }
#line 5961 "src/sema.tr"
            if ((!_tr_str_eqv((cc->err_name), (_tr_str_lit_len("", 0LL))))) {
#line 5962 "src/sema.tr"
                ({ __auto_type _ord_t1589 = (cc->err_name); __auto_type _ord_t1590 = (SymbolKind_make_SVariable()); __auto_type _ord_t1591 = (box_asttype(hcc_val->err_type)); Sema_declare(self, _ord_t1589, _ord_t1590, _ord_t1591, true); });
            }
#line 5963 "src/sema.tr"
            HirBlock* _cltmp_t1592 = Sema_lower_block(self, (*cc->body));
            _tr_obj_release(hcc_val->body, _trdrop_HirBlock);
            hcc_val->body = _cltmp_t1592;
#line 5964 "src/sema.tr"
            /* unsafe block */
#line 5965 "src/sema.tr"
            HirCatchClause** hcc = ((HirCatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(HirCatchClause*)));
#line 5966 "src/sema.tr"
            (*hcc = hcc_val);
#line 5967 "src/sema.tr"
            List_ptr_append(h_catches, hcc);
#line 5968 "src/sema.tr"
            k = (k + 1LL);
        }
#line 5969 "src/sema.tr"
        HirBlock* h_finally_b = Sema_lower_block(self, finally_b);
#line 5970 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5971 "src/sema.tr"
        Sema_close_block(self);
#line 5972 "src/sema.tr"
        HirStmt* _trret248 = box_hirstmt(HirStmt_ctor_STry(h_try_body, h_catches, h_finally_b));
        _tr_obj_release(h_try_body, _trdrop_HirBlock);
        _tr_obj_release(h_finally_b, _trdrop_HirBlock);
        return _trret248;
    } else if (_t1469.tag == Stmt_SRaise) {
        __auto_type e = _t1469.data.SRaise.val;
        return box_hirstmt(HirStmt_ctor_SRaise(Sema_lower_expr(self, e)));
    } else if (_t1469.tag == Stmt_SAssert) {
        __auto_type cond = _t1469.data.SAssert.cond;
__auto_type msg = _t1469.data.SAssert.msg;
        return ({ __auto_type _ord_t1593 = (Sema_lower_expr(self, cond)); __auto_type _ord_t1594 = (Sema_lower_expr(self, msg)); __auto_type _wr = (box_hirstmt(HirStmt_ctor_SAssert(_ord_t1593, _ord_t1594))); _wr; });
    } else if (_t1469.tag == Stmt_SDefer) {
        __auto_type inner = _t1469.data.SDefer.stmt;
#line 5976 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SDefer(Sema_lower_stmt(self, inner)));
    } else if (_t1469.tag == Stmt_SWith) {
        __auto_type items = _t1469.data.SWith.items;
__auto_type aliases = _t1469.data.SWith.aliases;
__auto_type body = _t1469.data.SWith.body;
#line 5978 "src/sema.tr"
        Sema_enter_scope(self);
#line 5979 "src/sema.tr"
        List_ptr* h_items = (void*)List_ptr_new();
#line 5980 "src/sema.tr"
        long long k = 0LL;
#line 5981 "src/sema.tr"
        while ((k < items->len)) {
#line 5982 "src/sema.tr"
            HirExpr* h_wi = Sema_lower_expr(self, ((Expr*)List_ptr_get(items, k)));
#line 5983 "src/sema.tr"
            List_ptr_append(h_items, h_wi);
#line 5985 "src/sema.tr"
            if (((k < aliases->len) && ({ TrStr _cmpa_t1595 = (List_TrStr_get(aliases, k)); TrStr _cmpb_t1596 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t1597 = !_tr_str_eqv(_cmpa_t1595, _cmpb_t1596); _tr_str_release(_cmpa_t1595); _cmpr_t1597; }))) {
#line 5986 "src/sema.tr"
                AstType* wi_ty = hir_expr_type(h_wi);
#line 5987 "src/sema.tr"
                ({ TrStr _ord_t1598 = (List_TrStr_get(aliases, k)); __auto_type _ord_t1599 = (SymbolKind_make_SVariable()); __auto_type _ord_t1600 = (box_asttype(wi_ty)); Sema_declare(self, _ord_t1598, _ord_t1599, _ord_t1600, true); _tr_str_release(_ord_t1598); });
            }
#line 5988 "src/sema.tr"
            k = (k + 1LL);
        }
#line 5989 "src/sema.tr"
        HirBlock* h_with_body = Sema_lower_block(self, body);
#line 5990 "src/sema.tr"
        Sema_finalize_scope_drops(self, h_with_body);
#line 5991 "src/sema.tr"
        Sema_exit_scope(self);
#line 5992 "src/sema.tr"
        HirStmt* _trret249 = box_hirstmt(HirStmt_ctor_SWith(h_items, aliases, h_with_body));
        _tr_obj_release(h_with_body, _trdrop_HirBlock);
        return _trret249;
    } else if (_t1469.tag == Stmt_SAsm) {
        __auto_type code = _t1469.data.SAsm.code;
__auto_type outputs = _t1469.data.SAsm.outputs;
__auto_type inputs = _t1469.data.SAsm.inputs;
__auto_type clobbers = _t1469.data.SAsm.clobbers;
#line 5994 "src/sema.tr"
        HirStmt* _trret250 = box_hirstmt(HirStmt_ctor_SAsm(code, outputs, inputs, clobbers));
        return _trret250;
    } else if (_t1469.tag == Stmt_SSpawn) {
        __auto_type e = _t1469.data.SSpawn.expr;
#line 5996 "src/sema.tr"
        if ((!self->in_async_fn)) {
#line 5997 "src/sema.tr"
            ({ TrStr _at_t1601 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[C-5] 'spawn:' used outside an async function. FIX: Declare '", 61LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as 'async def ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' to use spawn inside it.", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1601); _tr_str_release(_at_t1601); });
        }
#line 5998 "src/sema.tr"
        HirExpr* spawn_lowered = Sema_lower_expr(self, e);
#line 5999 "src/sema.tr"
        Sema_check_spawn_sendable(self, spawn_lowered);
#line 6000 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SSpawn(spawn_lowered));
    } else if (_t1469.tag == Stmt_STaskGroup) {
        __auto_type body = _t1469.data.STaskGroup.body;
#line 6002 "src/sema.tr"
        if ((!self->in_async_fn)) {
#line 6003 "src/sema.tr"
            ({ TrStr _at_t1602 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[C-6] 'taskgroup:' used outside an async function. FIX: Declare '", 65LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as 'async def ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' to use taskgroup inside it.", 34LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1602); _tr_str_release(_at_t1602); });
        }
#line 6004 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 6005 "src/sema.tr"
        Sema_open_block(self);
#line 6006 "src/sema.tr"
        HirBlock* h_tg_body = Sema_lower_block(self, body);
#line 6007 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 6008 "src/sema.tr"
        Sema_close_block(self);
#line 6009 "src/sema.tr"
        HirStmt* _trret251 = box_hirstmt(HirStmt_ctor_STaskGroup(h_tg_body));
        _tr_obj_release(h_tg_body, _trdrop_HirBlock);
        return _trret251;
    } else if (_t1469.tag == Stmt_SChanSelect) {
        __auto_type cs_cases = _t1469.data.SChanSelect.cases;
#line 6011 "src/sema.tr"
        List_ptr* hcs_cases = (void*)List_ptr_new();
#line 6012 "src/sema.tr"
        long long csi2 = 0LL;
#line 6013 "src/sema.tr"
        while ((csi2 < cs_cases->len)) {
#line 6014 "src/sema.tr"
            ChanSelectArm* arm = (*((ChanSelectArm**)List_ptr_get(cs_cases, csi2)));
#line 6015 "src/sema.tr"
            HirChanSelectArm** harm = (HirChanSelectArm**)(0LL);
#line 6016 "src/sema.tr"
            /* unsafe block */
#line 6016 "src/sema.tr"
            harm = ((HirChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(HirChanSelectArm*)));
#line 6017 "src/sema.tr"
            HirChanSelectArm* harm_v = ((HirChanSelectArm*)_tr_obj_alloc(sizeof(HirChanSelectArm)));
#line 6018 "src/sema.tr"
            harm_v->kind = arm->kind;
#line 6019 "src/sema.tr"
            harm_v->var_name = _tr_str_retain(arm->var_name);
#line 6020 "src/sema.tr"
            if ((((unsigned long long)(arm->chan_expr)) != ((unsigned long long)(0LL)))) {
#line 6021 "src/sema.tr"
                harm_v->chan_expr = Sema_lower_expr(self, arm->chan_expr);
            } else {
#line 6023 "src/sema.tr"
                harm_v->chan_expr = (HirExpr*)(0LL);
            }
#line 6024 "src/sema.tr"
            if ((((unsigned long long)(arm->val_expr)) != ((unsigned long long)(0LL)))) {
#line 6025 "src/sema.tr"
                harm_v->val_expr = Sema_lower_expr(self, arm->val_expr);
            } else {
#line 6027 "src/sema.tr"
                harm_v->val_expr = (HirExpr*)(0LL);
            }
#line 6028 "src/sema.tr"
            if ((((unsigned long long)(arm->timeout_ms)) != ((unsigned long long)(0LL)))) {
#line 6029 "src/sema.tr"
                harm_v->timeout_ms = Sema_lower_expr(self, arm->timeout_ms);
            } else {
#line 6031 "src/sema.tr"
                harm_v->timeout_ms = (HirExpr*)(0LL);
            }
#line 6033 "src/sema.tr"
            Sema_enter_scope(self);
#line 6034 "src/sema.tr"
            if (((arm->kind == 0LL) && (!_tr_str_eqv((arm->var_name), (_tr_str_lit_len("", 0LL)))))) {
#line 6035 "src/sema.tr"
                AstType* recv_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 6036 "src/sema.tr"
                if ((((unsigned long long)(arm->chan_expr)) != ((unsigned long long)(0LL)))) {
#line 6037 "src/sema.tr"
                    HirExpr* chan_hir = harm_v->chan_expr;
#line 6038 "src/sema.tr"
                    AstType* chan_ty = hir_expr_type(chan_hir);
#line 6039 "src/sema.tr"
                    if (((!_is_invalid_ptr(((unsigned long long)(chan_ty->args)))) && (chan_ty->args->len > 0LL))) {
#line 6040 "src/sema.tr"
                        recv_ty = (*((AstType**)List_ptr_get(chan_ty->args, 0LL)));
                    }
                }
#line 6041 "src/sema.tr"
                ({ __auto_type _ord_t1603 = (arm->var_name); __auto_type _ord_t1604 = (SymbolKind_make_SVariable()); __auto_type _ord_t1605 = (box_asttype(recv_ty)); Sema_declare(self, _ord_t1603, _ord_t1604, _ord_t1605, true); });
            }
#line 6042 "src/sema.tr"
            HirBlock* _cltmp_t1606 = Sema_lower_block(self, arm->body);
            _tr_obj_release(harm_v->body, _trdrop_HirBlock);
            harm_v->body = _cltmp_t1606;
#line 6043 "src/sema.tr"
            Sema_finalize_scope_drops(self, harm_v->body);
#line 6044 "src/sema.tr"
            Sema_exit_scope(self);
#line 6045 "src/sema.tr"
            /* unsafe block */
#line 6045 "src/sema.tr"
            (*harm = harm_v);
#line 6046 "src/sema.tr"
            List_ptr_append(hcs_cases, harm);
#line 6047 "src/sema.tr"
            csi2 = (csi2 + 1LL);
        }
#line 6048 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SChanSelect(hcs_cases));
    } else if (_t1469.tag == Stmt_SGpuBlock) {
        __auto_type body = _t1469.data.SGpuBlock.body;
#line 6050 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 6051 "src/sema.tr"
        Sema_open_block(self);
#line 6052 "src/sema.tr"
        HirBlock* h_gpu_body = Sema_lower_block(self, body);
#line 6053 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 6054 "src/sema.tr"
        Sema_close_block(self);
#line 6055 "src/sema.tr"
        HirStmt* _trret252 = box_hirstmt(HirStmt_ctor_SGpuBlock(h_gpu_body));
        _tr_obj_release(h_gpu_body, _trdrop_HirBlock);
        return _trret252;
    } else if (_t1469.tag == Stmt_SBreak) {
        __auto_type bv = _t1469.data.SBreak.val;
#line 6057 "src/sema.tr"
        HirExpr* hbv = (HirExpr*)(0LL);
#line 6058 "src/sema.tr"
        if ((((unsigned long long)(bv)) != ((unsigned long long)(0LL)))) {
#line 6058 "src/sema.tr"
            hbv = Sema_lower_expr(self, bv);
        }
#line 6059 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SBreak(hbv));
    } else if (_t1469.tag == Stmt_SContinue) {
        return box_hirstmt(HirStmt_make_SContinue());
    } else if (_t1469.tag == Stmt_SPass) {
        return box_hirstmt(HirStmt_make_SPass());
    } else if (_t1469.tag == Stmt_SLocalDecl) {
        __auto_type ldecl = _t1469.data.SLocalDecl.decl;
#line 6063 "src/sema.tr"
        if ((!_tr_str_eqv((self->current_func_name), (_tr_str_lit_len("main", 4LL))))) {
#line 6064 "src/sema.tr"
            Sema_error(self, _tr_str_lit_len("[E-2] Nested class/def/enum/interface declarations are only supported inside main().\n      FIX: Move this declaration to module (top-level) scope, or declare it inside main().", 175LL));
#line 6065 "src/sema.tr"
            return box_hirstmt(HirStmt_make_SPass());
        }
#line 6066 "src/sema.tr"
        Sema_register_decl(self, ldecl);
#line 6070 "src/sema.tr"
        TrStr saved_ld_func_name = _tr_str_retain(self->current_func_name);
#line 6071 "src/sema.tr"
        TrStr saved_ld_class_name = _tr_str_retain(self->current_class_name);
#line 6072 "src/sema.tr"
        __auto_type _t1607 = (*ldecl);
        if (_t1607.tag == Decl_DFunction) {
            __auto_type ld_f = _t1607.data.DFunction.func;
#line 6074 "src/sema.tr"
            ({ HirFunction* _ord_t1608 = (Sema_lower_func(self, ld_f)); List_ptr_append(self->nested_functions, _tr_obj_retain(_ord_t1608)); _tr_obj_release(_ord_t1608, _trdrop_HirFunction); });
        } else if (_t1607.tag == Decl_DClass) {
            __auto_type ld_c = _t1607.data.DClass.cls;
#line 6076 "src/sema.tr"
            ({ HirClass* _ord_t1609 = (Sema_lower_class(self, ld_c)); List_ptr_append(self->nested_classes, _tr_obj_retain(_ord_t1609)); _tr_obj_release(_ord_t1609, _trdrop_HirClass); });
        } else if (_t1607.tag == Decl_DActor) {
            __auto_type ld_c = _t1607.data.DActor.cls;
#line 6078 "src/sema.tr"
            ({ HirClass* _ord_t1610 = (Sema_lower_class(self, ld_c)); List_ptr_append(self->nested_classes, _tr_obj_retain(_ord_t1610)); _tr_obj_release(_ord_t1610, _trdrop_HirClass); });
        } else if (_t1607.tag == Decl_DEnum) {
            __auto_type ld_e = _t1607.data.DEnum.enm;
#line 6080 "src/sema.tr"
            ({ HirEnum* _ord_t1611 = (Sema_lower_enum(self, ld_e)); List_ptr_append(self->nested_enums, _tr_obj_retain(_ord_t1611)); _tr_obj_release(_ord_t1611, _trdrop_HirEnum); });
        } else if (_t1607.tag == Decl_DInterface) {
            __auto_type ld_i = _t1607.data.DInterface.iface;
#line 6082 "src/sema.tr"
            ({ HirInterface* _ord_t1612 = (Sema_lower_interface(self, ld_i)); List_ptr_append(self->nested_interfaces, _tr_obj_retain(_ord_t1612)); _tr_obj_release(_ord_t1612, _trdrop_HirInterface); });
        } else if (_t1607.tag == Decl_DExtend) {
            __auto_type ld_target = _t1607.data.DExtend.target;
__auto_type ld_methods = _t1607.data.DExtend.methods;
#line 6088 "src/sema.tr"
            self->current_class_name = _tr_str_retain(ld_target);
#line 6089 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(ld_target))) {
#line 6090 "src/sema.tr"
                self->current_region_params = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ld_target)))->region_params;
            }
#line 6091 "src/sema.tr"
            long long ld_ci = 0LL;
#line 6092 "src/sema.tr"
            bool ld_found = false;
#line 6093 "src/sema.tr"
            while ((ld_ci < self->nested_classes->len)) {
#line 6094 "src/sema.tr"
                HirClass* ld_nc = ((HirClass*)List_ptr_get(self->nested_classes, ld_ci));
#line 6095 "src/sema.tr"
                if (_tr_str_eqv((ld_nc->name), (ld_target))) {
#line 6096 "src/sema.tr"
                    long long ld_hi = 0LL;
#line 6097 "src/sema.tr"
                    while ((ld_hi < ld_methods->len)) {
#line 6098 "src/sema.tr"
                        ({ HirFunction* _ord_t1613 = (Sema_lower_func(self, ((FunctionDef*)List_ptr_get(ld_methods, ld_hi)))); List_ptr_append(ld_nc->methods, _tr_obj_retain(_ord_t1613)); _tr_obj_release(_ord_t1613, _trdrop_HirFunction); });
#line 6099 "src/sema.tr"
                        ld_hi = (ld_hi + 1LL);
                    }
#line 6100 "src/sema.tr"
                    ({ List_ptr* _sl = self->nested_classes; long long _si = (long long)(ld_ci); void* _sn = (void*)(_tr_obj_retain(ld_nc)); void* _so = (_sl && (size_t)_si < _sl->len) ? (void*)List_ptr_get(_sl, _si) : (void*)0; List_ptr_set(_sl, _si, _sn); if (_so) _tr_obj_release((HirClass*)_so, _trdrop_HirClass); });
#line 6101 "src/sema.tr"
                    ld_found = true;
                }
#line 6102 "src/sema.tr"
                ld_ci = (ld_ci + 1LL);
            }
#line 6103 "src/sema.tr"
            if ((!ld_found)) {
#line 6104 "src/sema.tr"
                long long ld_hi2 = 0LL;
#line 6105 "src/sema.tr"
                while ((ld_hi2 < ld_methods->len)) {
#line 6106 "src/sema.tr"
                    ({ HirFunction* _ord_t1614 = (Sema_lower_func(self, ((FunctionDef*)List_ptr_get(ld_methods, ld_hi2)))); List_ptr_append(self->nested_functions, _tr_obj_retain(_ord_t1614)); _tr_obj_release(_ord_t1614, _trdrop_HirFunction); });
#line 6107 "src/sema.tr"
                    ld_hi2 = (ld_hi2 + 1LL);
                }
            }
        } else if (1) {
            __auto_type _ = _t1607;
            /* pass */
        }
#line 6109 "src/sema.tr"
        self->current_func_name = _tr_str_retain(saved_ld_func_name);
#line 6110 "src/sema.tr"
        self->current_class_name = _tr_str_retain(saved_ld_class_name);
#line 6111 "src/sema.tr"
        self->current_region_params = (void*)List_TrStr_new();
#line 6112 "src/sema.tr"
        HirStmt* _trret253 = box_hirstmt(HirStmt_make_SPass());
        _tr_str_release(saved_ld_func_name);
        _tr_str_release(saved_ld_class_name);
        return _trret253;
    } else if (_t1469.tag == Stmt_SUnsafe) {
        __auto_type body = _t1469.data.SUnsafe.body;
#line 6115 "src/sema.tr"
        bool saved_unsafe = self->in_unsafe;
#line 6116 "src/sema.tr"
        self->in_unsafe = true;
#line 6117 "src/sema.tr"
        HirBlock* unsafe_hir = Sema_lower_block(self, body);
#line 6118 "src/sema.tr"
        self->in_unsafe = saved_unsafe;
#line 6119 "src/sema.tr"
        HirStmt* _trret254 = box_hirstmt(HirStmt_ctor_SUnsafe(unsafe_hir));
        _tr_obj_release(unsafe_hir, _trdrop_HirBlock);
        return _trret254;
    } else if (_t1469.tag == Stmt_SMultiLet) {
        __auto_type names = _t1469.data.SMultiLet.names;
__auto_type is_mut = _t1469.data.SMultiLet.is_mut;
__auto_type val_ptr = _t1469.data.SMultiLet.val;
#line 6121 "src/sema.tr"
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
#line 6122 "src/sema.tr"
        AstType* val_ty = hir_expr_type(hval);
#line 6123 "src/sema.tr"
        long long i = 0LL;
#line 6124 "src/sema.tr"
        while ((i < names->len)) {
#line 6125 "src/sema.tr"
            AstType* nty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 6126 "src/sema.tr"
            if ((val_ty->args->len > i)) {
#line 6126 "src/sema.tr"
                nty = (*((AstType**)List_ptr_get(val_ty->args, i)));
            }
#line 6127 "src/sema.tr"
            ({ TrStr _ord_t1615 = (List_TrStr_get(names, i)); __auto_type _ord_t1616 = (SymbolKind_make_SVariable()); __auto_type _ord_t1617 = (box_asttype(nty)); Sema_declare(self, _ord_t1615, _ord_t1616, _ord_t1617, is_mut); _tr_str_release(_ord_t1615); });
#line 6128 "src/sema.tr"
            i = (i + 1LL);
        }
#line 6129 "src/sema.tr"
        HirStmt* _trret255 = box_hirstmt(HirStmt_ctor_SMultiLet(names, is_mut, hval));
        return _trret255;
    } else if (_t1469.tag == Stmt_SLine) {
        __auto_type n = _t1469.data.SLine.n;
#line 6131 "src/sema.tr"
        self->current_line = n;
#line 6136 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SLineMarker(n));
    } else if (1) {
        __auto_type _ = _t1469;
        return box_hirstmt(HirStmt_make_SPass());
    }
}

__attribute__((hot)) AstType* Sema_variant_field_ty(Sema* self, TrStr type_name, TrStr variant_name, long long field_idx) {
#line 6140 "src/sema.tr"
    if (_tr_dict_contains(self->enums, _tr_strz(type_name))) {
#line 6141 "src/sema.tr"
        EnumDef* enm = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(type_name)));
#line 6142 "src/sema.tr"
        long long vi = 0LL;
#line 6143 "src/sema.tr"
        while ((vi < enm->variants->len)) {
#line 6144 "src/sema.tr"
            VariantDef* v = ((VariantDef*)List_ptr_get(enm->variants, vi));
#line 6145 "src/sema.tr"
            if (_tr_str_eqv((v->name), (variant_name))) {
#line 6146 "src/sema.tr"
                if ((field_idx < v->fields->len)) {
#line 6147 "src/sema.tr"
                    Param* fp = ((Param*)List_ptr_get(v->fields, field_idx));
#line 6148 "src/sema.tr"
                    if ((((unsigned long long)(fp->ty)) != ((unsigned long long)(0LL)))) {
#line 6148 "src/sema.tr"
                        return (*fp->ty);
                    }
                }
#line 6149 "src/sema.tr"
                return AstType_init(_tr_str_lit_len("void", 4LL));
            }
#line 6150 "src/sema.tr"
            vi = (vi + 1LL);
        }
    }
#line 6151 "src/sema.tr"
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) void Sema_declare_pattern_binds(Sema* self, Pattern pat) {
#line 6154 "src/sema.tr"
    Sema_declare_pattern_binds_typed(self, pat, AstType_init(_tr_str_lit_len("int", 3LL)));
}

__attribute__((hot)) void Sema_declare_pattern_binds_typed(Sema* self, Pattern pat, AstType* subj_ty) {
#line 6158 "src/sema.tr"
    __auto_type _t1618 = pat;
    if (_t1618.tag == Pattern_PBind) {
        __auto_type name = _t1618.data.PBind.name;
        ({ __auto_type _ord_t1619 = (SymbolKind_make_SVariable()); __auto_type _ord_t1620 = (box_asttype(subj_ty)); Sema_declare(self, name, _ord_t1619, _ord_t1620, false); });
    } else if (_t1618.tag == Pattern_PVariantBind) {
        __auto_type type_name = _t1618.data.PVariantBind.type_name;
__auto_type variant_name = _t1618.data.PVariantBind.variant;
__auto_type field = _t1618.data.PVariantBind.field;
#line 6161 "src/sema.tr"
        AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, 0LL);
#line 6164 "src/sema.tr"
        if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Option", 6LL))) && _tr_str_eqv((variant_name), (_tr_str_lit_len("Some", 4LL)))) && (subj_ty->args->len > 0LL))) {
#line 6165 "src/sema.tr"
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 0LL)));
        } else if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((variant_name), (_tr_str_lit_len("Ok", 2LL)))) && (subj_ty->args->len > 0LL))) {
#line 6167 "src/sema.tr"
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 0LL)));
        } else if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((variant_name), (_tr_str_lit_len("Err", 3LL)))) && (subj_ty->args->len > 1LL))) {
#line 6169 "src/sema.tr"
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 1LL)));
        }
#line 6170 "src/sema.tr"
        if (_tr_str_eqv((fty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 6170 "src/sema.tr"
            fty = AstType_init(_tr_str_lit_len("int", 3LL));
        }
#line 6171 "src/sema.tr"
        ({ __auto_type _ord_t1621 = (SymbolKind_make_SVariable()); __auto_type _ord_t1622 = (box_asttype(fty)); Sema_declare(self, field, _ord_t1621, _ord_t1622, false); });
    } else if (_t1618.tag == Pattern_PVariantBindMany) {
        __auto_type type_name = _t1618.data.PVariantBindMany.type_name;
__auto_type variant_name = _t1618.data.PVariantBindMany.variant;
__auto_type fields = _t1618.data.PVariantBindMany.fields;
#line 6173 "src/sema.tr"
        long long _pi = 0LL;
#line 6174 "src/sema.tr"
        while ((_pi < fields->len)) {
#line 6175 "src/sema.tr"
            TrStr _pf = List_TrStr_get(fields, _pi);
#line 6176 "src/sema.tr"
            if ((!_tr_str_eqv((_pf), (_tr_str_lit_len("_", 1LL))))) {
#line 6177 "src/sema.tr"
                AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, _pi);
#line 6178 "src/sema.tr"
                if (_tr_str_eqv((fty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 6178 "src/sema.tr"
                    fty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
#line 6179 "src/sema.tr"
                ({ __auto_type _ord_t1623 = (SymbolKind_make_SVariable()); __auto_type _ord_t1624 = (box_asttype(fty)); Sema_declare(self, _pf, _ord_t1623, _ord_t1624, false); });
            }
#line 6180 "src/sema.tr"
            _pi = (_pi + 1LL);
            _tr_str_release(_pf);
        }
    } else if (_t1618.tag == Pattern_PTuple) {
        __auto_type first = _t1618.data.PTuple.first;
__auto_type second = _t1618.data.PTuple.second;
#line 6182 "src/sema.tr"
        ({ __auto_type _ord_t1625 = (SymbolKind_make_SVariable()); __auto_type _ord_t1626 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); Sema_declare(self, first, _ord_t1625, _ord_t1626, false); });
#line 6183 "src/sema.tr"
        ({ __auto_type _ord_t1627 = (SymbolKind_make_SVariable()); __auto_type _ord_t1628 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); Sema_declare(self, second, _ord_t1627, _ord_t1628, false); });
    } else if (_t1618.tag == Pattern_POr) {
        __auto_type pats = _t1618.data.POr.patterns;
#line 6185 "src/sema.tr"
        long long i = 0LL;
#line 6186 "src/sema.tr"
        while ((i < pats->len)) {
#line 6187 "src/sema.tr"
            Sema_declare_pattern_binds_typed(self, List_Pattern_get(pats, i), subj_ty);
#line 6188 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1618;
        /* pass */
    }
}

__attribute__((hot)) void Sema_check_match_expr_exhaustive(Sema* self, TrStr ty_name, List_ptr* arms) {
#line 6194 "src/sema.tr"
    if ((!_tr_dict_contains(self->enums, _tr_strz(ty_name)))) {
#line 6194 "src/sema.tr"
        return;
    }
#line 6195 "src/sema.tr"
    List_TrStr* covered = (void*)List_TrStr_new();
#line 6196 "src/sema.tr"
    bool has_wild = false;
#line 6197 "src/sema.tr"
    long long i = 0LL;
#line 6198 "src/sema.tr"
    while ((i < arms->len)) {
#line 6199 "src/sema.tr"
        MatchArm* arm = ((MatchArm*)List_ptr_get(arms, i));
#line 6200 "src/sema.tr"
        bool guarded = (((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)));
#line 6201 "src/sema.tr"
        if ((!guarded)) {
#line 6202 "src/sema.tr"
            __auto_type _t1629 = arm->pat;
            if (_t1629.tag == Pattern_PWild) {
                has_wild = true;
            } else if (_t1629.tag == Pattern_PBind) {
                __auto_type _ = _t1629.data.PBind.name;
                has_wild = true;
            } else if (_t1629.tag == Pattern_PVariant) {
                __auto_type v1 = _t1629.data.PVariant.variant;
                List_TrStr_append(covered, v1);
            } else if (_t1629.tag == Pattern_PVariantBind) {
                __auto_type v2 = _t1629.data.PVariantBind.variant;
                List_TrStr_append(covered, v2);
            } else if (_t1629.tag == Pattern_PVariantBindMany) {
                __auto_type v3 = _t1629.data.PVariantBindMany.variant;
                List_TrStr_append(covered, v3);
            } else if (_t1629.tag == Pattern_POr) {
                __auto_type ors = _t1629.data.POr.patterns;
#line 6209 "src/sema.tr"
                long long oi = 0LL;
#line 6210 "src/sema.tr"
                while ((oi < ors->len)) {
#line 6211 "src/sema.tr"
                    __auto_type _t1630 = List_Pattern_get(ors, oi);
                    if (_t1630.tag == Pattern_PVariant) {
                        __auto_type ov1 = _t1630.data.PVariant.variant;
                        List_TrStr_append(covered, ov1);
                    } else if (_t1630.tag == Pattern_PVariantBind) {
                        __auto_type ov2 = _t1630.data.PVariantBind.variant;
                        List_TrStr_append(covered, ov2);
                    } else if (_t1630.tag == Pattern_PVariantBindMany) {
                        __auto_type ov3 = _t1630.data.PVariantBindMany.variant;
                        List_TrStr_append(covered, ov3);
                    } else if (1) {
                        __auto_type _ = _t1630;
                        /* pass */
                    }
#line 6216 "src/sema.tr"
                    oi = (oi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1629;
                /* pass */
            }
        }
#line 6218 "src/sema.tr"
        i = (i + 1LL);
    }
#line 6219 "src/sema.tr"
    if (has_wild) {
#line 6219 "src/sema.tr"
        List_TrStr_free(covered);
        return;
    }
#line 6220 "src/sema.tr"
    EnumDef* edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(ty_name)));
#line 6221 "src/sema.tr"
    TrStr missing = _tr_str_lit_len("", 0LL);
#line 6222 "src/sema.tr"
    long long nmiss = 0LL;
#line 6223 "src/sema.tr"
    long long vi = 0LL;
#line 6224 "src/sema.tr"
    while ((vi < edef->variants->len)) {
#line 6225 "src/sema.tr"
        TrStr vn = _tr_str_retain(((VariantDef*)List_ptr_get(edef->variants, vi))->name);
#line 6226 "src/sema.tr"
        if ((!List_TrStr_contains(covered, vn))) {
#line 6227 "src/sema.tr"
            if ((nmiss > 0LL)) {
#line 6227 "src/sema.tr"
                ({ TrStr _ap_t1631 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&missing, _ap_t1631); });
            }
#line 6228 "src/sema.tr"
            ({ TrStr _ap_t1632 = (vn); _tr_str_append(&missing, _ap_t1632); });
#line 6229 "src/sema.tr"
            nmiss = (nmiss + 1LL);
        }
#line 6230 "src/sema.tr"
        vi = (vi + 1LL);
        _tr_str_release(vn);
    }
#line 6231 "src/sema.tr"
    if ((nmiss > 0LL)) {
#line 6232 "src/sema.tr"
        TrStr plural = _tr_str_lit_len("", 0LL);
#line 6233 "src/sema.tr"
        if ((nmiss > 1LL)) {
#line 6233 "src/sema.tr"
            TrStr _strtmp_t1633 = _tr_str_lit_len("s", 1LL);
            _tr_str_release(plural);
            plural = _strtmp_t1633;
        }
#line 6234 "src/sema.tr"
        ({ TrStr _at_t1634 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] Non-exhaustive match expression on '", 42LL)), (ty_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': missing variant", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (plural)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (missing)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: Add a 'case ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".VariantName:' arm for each missing variant, or add 'case _:' to give the remaining cases a value.", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1634); _tr_str_release(_at_t1634); });
    }
    List_TrStr_free(covered);
    _tr_str_release(missing);
}

__attribute__((hot)) HirExpr* Sema_retype_none(Sema* self, HirExpr* e, AstType* expected) {
#line 6239 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 6239 "src/sema.tr"
        return e;
    }
#line 6240 "src/sema.tr"
    if ((!_tr_str_eqv((expected->name), (_tr_str_lit_len("Option", 6LL))))) {
#line 6240 "src/sema.tr"
        return e;
    }
#line 6241 "src/sema.tr"
    __auto_type _t1635 = (*e);
    if (_t1635.tag == HirExpr_ELitNone) {
        __auto_type _ = _t1635.data.ELitNone.ty;
        HirExpr* _trret256 = box_hirexpr(HirExpr_ctor_ELitNone(expected));
        return _trret256;
    } else if (1) {
        __auto_type _ = _t1635;
        return e;
    }
}

__attribute__((hot)) AstType* Sema_str_method_ret_ty(Sema* self, TrStr method) {
#line 6246 "src/sema.tr"
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("split", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("split_to_vec", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("split_once", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("split_exact", 11LL))))) {
#line 6247 "src/sema.tr"
        return AstType_init_generic(_tr_str_lit_len("Vec", 3LL), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL))));
    }
#line 6248 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("strip", 5LL)))) {
#line 6248 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6249 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("trim", 4LL)))) {
#line 6249 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6250 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("trim_left", 9LL)))) {
#line 6250 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6251 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("trim_right", 10LL)))) {
#line 6251 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6252 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("to_upper", 8LL)))) {
#line 6252 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6253 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("to_lower", 8LL)))) {
#line 6253 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6254 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("upper", 5LL)))) {
#line 6254 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6255 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("lower", 5LL)))) {
#line 6255 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6256 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("lstrip", 6LL)))) {
#line 6256 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6257 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("rstrip", 6LL)))) {
#line 6257 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6258 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("center", 6LL)))) {
#line 6258 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6259 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("pad_left", 8LL)))) {
#line 6259 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6260 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("pad_right", 9LL)))) {
#line 6260 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6261 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("zfill", 5LL)))) {
#line 6261 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6262 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("format", 6LL)))) {
#line 6262 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6263 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("chars", 5LL)))) {
#line 6263 "src/sema.tr"
        return AstType_init_generic(_tr_str_lit_len("Vec", 3LL), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL))));
    }
#line 6264 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("capitalize", 10LL)))) {
#line 6264 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6265 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("title", 5LL)))) {
#line 6265 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6266 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL)))) {
#line 6266 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6267 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("repeat", 6LL)))) {
#line 6267 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6268 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("replace", 7LL)))) {
#line 6268 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6269 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("replace_first", 13LL)))) {
#line 6269 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6270 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL)))) {
#line 6270 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6271 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("strip_prefix", 12LL)))) {
#line 6271 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6272 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("strip_suffix", 12LL)))) {
#line 6272 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6273 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("remove_char", 11LL)))) {
#line 6273 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6274 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) {
#line 6274 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6275 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))) {
#line 6275 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6276 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("find", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
#line 6276 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6277 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("md5", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sha256", 6LL))))) {
#line 6277 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6278 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("last_index_of", 13LL))) || _tr_str_eqv((method), (_tr_str_lit_len("rfind", 5LL))))) {
#line 6278 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6279 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL)))) {
#line 6279 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6280 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("char_at", 7LL)))) {
#line 6280 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6281 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_int", 9LL)))) {
#line 6281 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6282 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_int", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_i64", 6LL))))) {
#line 6282 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6283 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_f64", 6LL))))) {
#line 6283 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("float", 5LL));
    }
#line 6284 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("starts_with", 11LL)))) {
#line 6284 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6285 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("ends_with", 9LL)))) {
#line 6285 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6286 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) {
#line 6286 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6287 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_bool", 10LL)))) {
#line 6287 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6288 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) {
#line 6288 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6289 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("contains_char", 13LL)))) {
#line 6289 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6290 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("eq", 2LL)))) {
#line 6290 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6291 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_digit", 8LL)))) {
#line 6291 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6292 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_alpha", 8LL)))) {
#line 6292 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6293 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_alnum", 8LL)))) {
#line 6293 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6294 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_space", 8LL)))) {
#line 6294 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6295 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_upper", 8LL)))) {
#line 6295 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6296 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_lower", 8LL)))) {
#line 6296 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6297 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_bool", 10LL)))) {
#line 6297 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6298 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_float", 11LL)))) {
#line 6298 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("float", 5LL));
    }
#line 6299 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("lines", 5LL)))) {
#line 6299 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("Vec", 3LL));
    }
#line 6300 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("words", 5LL)))) {
#line 6300 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("Vec", 3LL));
    }
#line 6301 "src/sema.tr"
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) void Sema_collect_block_refs(Sema* self, HirBlock* b, List_TrStr* out) {
#line 6306 "src/sema.tr"
    long long i = 0LL;
#line 6307 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 6308 "src/sema.tr"
        Sema_collect_stmt_refs(self, ((HirStmt*)List_ptr_get(b->stmts, i)), out);
#line 6309 "src/sema.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema_collect_stmt_refs(Sema* self, HirStmt* s, List_TrStr* out) {
#line 6312 "src/sema.tr"
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
#line 6312 "src/sema.tr"
        return;
    }
#line 6313 "src/sema.tr"
    __auto_type _t1636 = (*s);
    if (_t1636.tag == HirStmt_SExpr) {
        __auto_type e = _t1636.data.SExpr.expr;
        Sema_collect_idents(self, e, out);
    } else if (_t1636.tag == HirStmt_SLet) {
        __auto_type v = _t1636.data.SLet.val;
        Sema_collect_idents(self, v, out);
    } else if (_t1636.tag == HirStmt_SAssign) {
        __auto_type t = _t1636.data.SAssign.target;
__auto_type v = _t1636.data.SAssign.val;
#line 6317 "src/sema.tr"
        Sema_collect_idents(self, t, out);
#line 6318 "src/sema.tr"
        Sema_collect_idents(self, v, out);
    } else if (_t1636.tag == HirStmt_SReturn) {
        __auto_type v = _t1636.data.SReturn.val;
#line 6320 "src/sema.tr"
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
#line 6320 "src/sema.tr"
            Sema_collect_idents(self, v, out);
        }
    } else if (_t1636.tag == HirStmt_SBreak) {
        __auto_type v = _t1636.data.SBreak.val;
#line 6322 "src/sema.tr"
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
#line 6322 "src/sema.tr"
            Sema_collect_idents(self, v, out);
        }
    } else if (_t1636.tag == HirStmt_SRaise) {
        __auto_type v = _t1636.data.SRaise.val;
        Sema_collect_idents(self, v, out);
    } else if (_t1636.tag == HirStmt_SIf) {
        __auto_type c = _t1636.data.SIf.cond;
__auto_type tb = _t1636.data.SIf.then_b;
__auto_type eb = _t1636.data.SIf.else_b;
#line 6325 "src/sema.tr"
        Sema_collect_idents(self, c, out);
#line 6326 "src/sema.tr"
        Sema_collect_block_refs(self, tb, out);
#line 6327 "src/sema.tr"
        Sema_collect_block_refs(self, eb, out);
    } else if (_t1636.tag == HirStmt_SWhile) {
        __auto_type c = _t1636.data.SWhile.cond;
__auto_type b2 = _t1636.data.SWhile.body;
#line 6329 "src/sema.tr"
        Sema_collect_idents(self, c, out);
#line 6330 "src/sema.tr"
        Sema_collect_block_refs(self, b2, out);
    } else if (_t1636.tag == HirStmt_SFor) {
        __auto_type it = _t1636.data.SFor.iter;
__auto_type b2 = _t1636.data.SFor.body;
#line 6332 "src/sema.tr"
        Sema_collect_idents(self, it, out);
#line 6333 "src/sema.tr"
        Sema_collect_block_refs(self, b2, out);
    } else if (_t1636.tag == HirStmt_SForUnpack) {
        __auto_type it = _t1636.data.SForUnpack.iter;
__auto_type b2 = _t1636.data.SForUnpack.body;
#line 6335 "src/sema.tr"
        Sema_collect_idents(self, it, out);
#line 6336 "src/sema.tr"
        Sema_collect_block_refs(self, b2, out);
    } else if (_t1636.tag == HirStmt_SMatch) {
        __auto_type subj = _t1636.data.SMatch.expr;
__auto_type arms = _t1636.data.SMatch.arms;
#line 6338 "src/sema.tr"
        Sema_collect_idents(self, subj, out);
#line 6339 "src/sema.tr"
        long long i = 0LL;
#line 6340 "src/sema.tr"
        while ((i < arms->len)) {
#line 6341 "src/sema.tr"
            Sema_collect_block_refs(self, ((HirMatchArm*)List_ptr_get(arms, i))->body, out);
#line 6342 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1636.tag == HirStmt_SUnsafe) {
        __auto_type b2 = _t1636.data.SUnsafe.body;
        Sema_collect_block_refs(self, b2, out);
    } else if (1) {
        __auto_type _ = _t1636;
        /* pass */
    }
}

__attribute__((hot)) HirExpr* Sema_lower_do_value(Sema* self, Block* do_body) {
#line 6354 "src/sema.tr"
    Sema_enter_scope(self);
#line 6355 "src/sema.tr"
    HirBlock* do_hb = HirBlock_init();
#line 6356 "src/sema.tr"
    long long do_n = do_body->stmts->len;
#line 6357 "src/sema.tr"
    long long do_i = 0LL;
#line 6358 "src/sema.tr"
    HirExpr* do_val = (HirExpr*)(0LL);
#line 6359 "src/sema.tr"
    AstType* do_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 6360 "src/sema.tr"
    while ((do_i < do_n)) {
#line 6361 "src/sema.tr"
        Stmt* do_s = ((Stmt*)List_ptr_get(do_body->stmts, do_i));
#line 6362 "src/sema.tr"
        if ((do_i == (do_n - 1LL))) {
#line 6363 "src/sema.tr"
            __auto_type _t1637 = (*do_s);
            if (_t1637.tag == Stmt_SExpr) {
                __auto_type de = _t1637.data.SExpr.expr;
#line 6365 "src/sema.tr"
                do_val = Sema_lower_expr(self, de);
#line 6366 "src/sema.tr"
                do_ty = hir_expr_type(do_val);
            } else if (1) {
                __auto_type _ = _t1637;
#line 6368 "src/sema.tr"
                HirStmt* do_hs = Sema_lower_stmt(self, do_s);
#line 6369 "src/sema.tr"
                if ((((unsigned long long)(do_hs)) != ((unsigned long long)(0LL)))) {
#line 6370 "src/sema.tr"
                    Sema_apply_escape_marks(self, do_hs);
#line 6371 "src/sema.tr"
                    HirBlock_push(do_hb, do_hs);
                }
            }
        } else {
#line 6373 "src/sema.tr"
            HirStmt* do_hs2 = Sema_lower_stmt(self, do_s);
#line 6374 "src/sema.tr"
            if ((((unsigned long long)(do_hs2)) != ((unsigned long long)(0LL)))) {
#line 6375 "src/sema.tr"
                Sema_apply_escape_marks(self, do_hs2);
#line 6376 "src/sema.tr"
                HirBlock_push(do_hb, do_hs2);
            }
        }
#line 6377 "src/sema.tr"
        do_i = (do_i + 1LL);
    }
#line 6378 "src/sema.tr"
    List_TrStr* do_excl = (void*)List_TrStr_new();
#line 6379 "src/sema.tr"
    if (((((unsigned long long)(do_val)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv((do_ty->name), (_tr_str_lit_len("str", 3LL)))))) {
#line 6380 "src/sema.tr"
        Sema_collect_idents(self, do_val, do_excl);
    }
#line 6381 "src/sema.tr"
    Sema_append_drops_from_excl_multi(self, do_hb, (self->scopes->len - 1LL), do_excl);
#line 6382 "src/sema.tr"
    if ((((unsigned long long)(do_val)) != ((unsigned long long)(0LL)))) {
#line 6382 "src/sema.tr"
        HirBlock_push(do_hb, box_hirstmt(HirStmt_ctor_SExpr(do_val)));
    }
#line 6383 "src/sema.tr"
    Sema_exit_scope(self);
#line 6384 "src/sema.tr"
    HirExpr* _trret257 = box_hirexpr(HirExpr_ctor_EDo(do_hb, do_ty));
    _tr_obj_release(do_hb, _trdrop_HirBlock);
    return _trret257;
}

__attribute__((hot)) AstType* Sema_infer_break_type(Sema* self, HirBlock* hb) {
#line 6390 "src/sema.tr"
    long long i = 0LL;
#line 6391 "src/sema.tr"
    while ((i < hb->stmts->len)) {
#line 6392 "src/sema.tr"
        AstType* t = Sema_infer_break_type_stmt(self, ((HirStmt*)List_ptr_get(hb->stmts, i)));
#line 6393 "src/sema.tr"
        if ((!_tr_str_eqv((t->name), (_tr_str_lit_len("void", 4LL))))) {
#line 6393 "src/sema.tr"
            return t;
        }
#line 6394 "src/sema.tr"
        i = (i + 1LL);
    }
#line 6395 "src/sema.tr"
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) AstType* Sema_infer_break_type_stmt(Sema* self, HirStmt* s) {
#line 6398 "src/sema.tr"
    __auto_type _t1638 = (*s);
    if (_t1638.tag == HirStmt_SBreak) {
        __auto_type bv = _t1638.data.SBreak.val;
#line 6400 "src/sema.tr"
        if ((((unsigned long long)(bv)) != ((unsigned long long)(0LL)))) {
#line 6400 "src/sema.tr"
            return hir_expr_type(bv);
        }
#line 6401 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (_t1638.tag == HirStmt_SIf) {
        __auto_type then_b = _t1638.data.SIf.then_b;
__auto_type else_b = _t1638.data.SIf.else_b;
#line 6403 "src/sema.tr"
        AstType* t = Sema_infer_break_type(self, then_b);
#line 6404 "src/sema.tr"
        if ((!_tr_str_eqv((t->name), (_tr_str_lit_len("void", 4LL))))) {
#line 6404 "src/sema.tr"
            return t;
        }
#line 6405 "src/sema.tr"
        return Sema_infer_break_type(self, else_b);
    } else if (_t1638.tag == HirStmt_SMatch) {
        __auto_type m_arms = _t1638.data.SMatch.arms;
#line 6407 "src/sema.tr"
        long long ai = 0LL;
#line 6408 "src/sema.tr"
        while ((ai < m_arms->len)) {
#line 6409 "src/sema.tr"
            AstType* t2 = Sema_infer_break_type(self, ((HirMatchArm*)List_ptr_get(m_arms, ai))->body);
#line 6410 "src/sema.tr"
            if ((!_tr_str_eqv((t2->name), (_tr_str_lit_len("void", 4LL))))) {
#line 6410 "src/sema.tr"
                return t2;
            }
#line 6411 "src/sema.tr"
            ai = (ai + 1LL);
        }
#line 6412 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (_t1638.tag == HirStmt_STry) {
        __auto_type tb = _t1638.data.STry.try_body;
__auto_type fb = _t1638.data.STry.finally_b;
#line 6414 "src/sema.tr"
        AstType* t3 = Sema_infer_break_type(self, tb);
#line 6415 "src/sema.tr"
        if ((!_tr_str_eqv((t3->name), (_tr_str_lit_len("void", 4LL))))) {
#line 6415 "src/sema.tr"
            return t3;
        }
#line 6416 "src/sema.tr"
        return Sema_infer_break_type(self, fb);
    } else if (1) {
        __auto_type _ = _t1638;
        return AstType_init(_tr_str_lit_len("void", 4LL));
    }
}

__attribute__((hot)) void Sema_check_no_heap(Sema* self, TrStr what, TrStr fix) {
#line 6425 "src/sema.tr"
    if (self->no_heap) {
#line 6426 "src/sema.tr"
        ({ TrStr _at_t1639 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[H-1] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" allocates on the heap, which --no-heap forbids (bare-metal zero-heap build).\n      FIX: ", 89LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fix)); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1639); _tr_str_release(_at_t1639); });
    }
}

__attribute__((hot)) HirExpr* Sema_lower_expr(Sema* self, Expr* e_ptr) {
#line 6429 "src/sema.tr"
    if ((((unsigned long long)(e_ptr)) == ((unsigned long long)(0LL)))) {
#line 6429 "src/sema.tr"
        return (HirExpr*)(0LL);
    }
#line 6430 "src/sema.tr"
    __auto_type e = (*e_ptr);
#line 6431 "src/sema.tr"
    __auto_type _t1640 = e;
    if (_t1640.tag == Expr_ELitInt) {
        __auto_type v = _t1640.data.ELitInt.val;
        return box_hirexpr(HirExpr_ctor_ELitInt(v, AstType_init(_tr_str_lit_len("int", 3LL))));
    } else if (_t1640.tag == Expr_ELitFloat) {
        __auto_type v = _t1640.data.ELitFloat.val;
        return box_hirexpr(HirExpr_ctor_ELitFloat(v, AstType_init(_tr_str_lit_len("float", 5LL))));
    } else if (_t1640.tag == Expr_ELitStr) {
        __auto_type v = _t1640.data.ELitStr.val;
__auto_type blen = _t1640.data.ELitStr.blen;
        HirExpr* _trret258 = box_hirexpr(HirExpr_ctor_ELitStr(v, blen, AstType_init(_tr_str_lit_len("str", 3LL))));
        return _trret258;
    } else if (_t1640.tag == Expr_ERawStr) {
        __auto_type v = _t1640.data.ERawStr.val;
__auto_type blen = _t1640.data.ERawStr.blen;
        HirExpr* _trret259 = box_hirexpr(HirExpr_ctor_ERawStr(v, blen, AstType_init(_tr_str_lit_len("str", 3LL))));
        return _trret259;
    } else if (_t1640.tag == Expr_ELitBytes) {
        __auto_type v = _t1640.data.ELitBytes.val;
__auto_type blen = _t1640.data.ELitBytes.blen;
        HirExpr* _trret260 = box_hirexpr(HirExpr_ctor_ELitBytes(v, blen, AstType_init(_tr_str_lit_len("Bytes", 5LL))));
        return _trret260;
    } else if (_t1640.tag == Expr_ELitBool) {
        __auto_type v = _t1640.data.ELitBool.val;
        return box_hirexpr(HirExpr_ctor_ELitBool(v, AstType_init(_tr_str_lit_len("bool", 4LL))));
    } else if (_t1640.tag == Expr_ELitChar) {
        __auto_type v = _t1640.data.ELitChar.val;
        return box_hirexpr(HirExpr_ctor_ELitChar(v, AstType_init(_tr_str_lit_len("char", 4LL))));
    } else if (_t1640.tag == Expr_ELitNone) {
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init(_tr_str_lit_len("None", 4LL))));
    } else if (_t1640.tag == Expr_EIdent) {
        __auto_type name = _t1640.data.EIdent.name;
#line 6441 "src/sema.tr"
        Symbol* sym = Sema_resolve(self, name);
#line 6442 "src/sema.tr"
        AstType* ty = (*sym->ty);
#line 6447 "src/sema.tr"
        bool _n3_recv_ok = self->in_recv_pos;
#line 6448 "src/sema.tr"
        if (_n3_recv_ok) {
#line 6451 "src/sema.tr"
            TrStr _n3_c = _tr_str_slicev((name), 0LL, 1LL);
#line 6452 "src/sema.tr"
            if (((strcmp(_tr_strz(_n3_c), _tr_strz(_tr_str_lit_len("a", 1LL))) >= 0) && (strcmp(_tr_strz(_n3_c), _tr_strz(_tr_str_lit_len("z", 1LL))) <= 0))) {
#line 6452 "src/sema.tr"
                _n3_recv_ok = false;
            }
        }
#line 6453 "src/sema.tr"
        bool _n3_known = Sema_is_known_name(self, name);
#line 6454 "src/sema.tr"
        if (((((((_n3_known && (_tr_str_lenv((name)) == 1LL)) && (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit_len("a", 1LL))) >= 0)) && (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit_len("z", 1LL))) <= 0)) && (!_tr_dict_contains(self->fn_sigs, _tr_strz(name)))) && (!_tr_dict_contains(self->extern_names, _tr_strz(name)))) && (!_tr_dict_contains(self->classes, _tr_strz(name))))) {
#line 6455 "src/sema.tr"
            _n3_known = false;
        }
#line 6456 "src/sema.tr"
        if (((((_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))) && (!_tr_str_eqv((name), (_tr_str_lit_len("", 0LL))))) && (!self->in_assign_target)) && (!_n3_recv_ok)) && (!_n3_known))) {
#line 6457 "src/sema.tr"
            ({ TrStr _at_t1641 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-3] name '", 12LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not defined.\n      FIX: check the spelling, declare it with 'mut ", 70LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ...', or import it before use.", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1641); _tr_str_release(_at_t1641); });
        }
#line 6460 "src/sema.tr"
        if ((_tr_dict_contains(self->fn_sigs, _tr_strz(name)) && (sym->kind.tag == SymbolKind_make_SFunction().tag))) {
#line 6461 "src/sema.tr"
            HirExpr* _trret261 = box_hirexpr(HirExpr_ctor_EIdent(name, ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(name))), false));
            return _trret261;
        }
#line 6463 "src/sema.tr"
        if ((sym->is_freed && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))))) {
#line 6464 "src/sema.tr"
            ({ TrStr _at_t1642 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-6] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was freed by 'dealloc()' and can no longer be used.\n      FIX: Remove all uses of '", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' after 'dealloc()', or restructure so the pointer is freed only when no longer needed.", 87LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1642); _tr_str_release(_at_t1642); });
        } else if ((((((sym->is_moved && (!self->in_assign_target_plain)) && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (!sym->is_shared))) {
#line 6467 "src/sema.tr"
            ({ TrStr _at_t1643 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving.", 123LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1643); _tr_str_release(_at_t1643); });
        } else if ((((((sym->is_maybe_moved && (!self->in_assign_target_plain)) && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (!sym->is_shared))) {
#line 6470 "src/sema.tr"
            ({ TrStr _at_t1644 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-5] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' may have been moved on some code paths, making this use unsafe.\n      FIX: Ensure '", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not moved before this point on any branch, or restructure so the use is inside the branch where it's still valid.", 118LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1644); _tr_str_release(_at_t1644); });
        }
#line 6472 "src/sema.tr"
        if ((((((!sym->is_init) && (!sym->is_maybe_init)) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
#line 6473 "src/sema.tr"
            ({ TrStr _at_t1645 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-1] Variable '", 16LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is used before being assigned a value.\n      FIX: Assign a value before use, e.g. 'mut ", 89LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = <default>'.", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1645); _tr_str_release(_at_t1645); });
        } else if (((((sym->is_maybe_init && (!sym->is_init)) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
#line 6476 "src/sema.tr"
            ({ TrStr _at_t1646 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-2] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not initialized on all code paths before this use.\n      FIX: Initialize '", 79LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' before the if/loop, or ensure every branch assigns a value.", 61LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1646); _tr_str_release(_at_t1646); });
        }
#line 6477 "src/sema.tr"
        bool is_move = false;
#line 6478 "src/sema.tr"
        if ((_tr_dict_contains(self->assign_froms, _tr_strz(name)) && (!Sema_is_primitive(self, ty)))) {
#line 6478 "src/sema.tr"
            is_move = true;
        }
#line 6479 "src/sema.tr"
        HirExpr* _trret262 = box_hirexpr(HirExpr_ctor_EIdent(name, ty, is_move));
        return _trret262;
    } else if (_t1640.tag == Expr_EBinOp) {
        __auto_type op = _t1640.data.EBinOp.op;
__auto_type left = _t1640.data.EBinOp.left;
__auto_type right = _t1640.data.EBinOp.right;
#line 6481 "src/sema.tr"
        HirExpr* hleft = Sema_lower_expr(self, left);
#line 6482 "src/sema.tr"
        HirExpr* hright = Sema_lower_expr(self, right);
#line 6483 "src/sema.tr"
        if (true) {
#line 6484 "src/sema.tr"
            TrStr _bo_lf = Sema_firm_family(self, hleft);
#line 6485 "src/sema.tr"
            TrStr _bo_rf = Sema_firm_family(self, hright);
#line 6486 "src/sema.tr"
            TrStr _bo_bad = _tr_str_retain(_bo_rf);
#line 6487 "src/sema.tr"
            if (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6487 "src/sema.tr"
                TrStr _strtmp_t1647 = _tr_str_retain(_bo_lf);
                _tr_str_release(_bo_bad);
                _bo_bad = _strtmp_t1647;
            }
#line 6488 "src/sema.tr"
            if ((((((_tr_str_eqv((op), (_tr_str_lit_len("|", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("^", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<<", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">>", 2LL)))) && (((((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 6489 "src/sema.tr"
                ({ TrStr _at_t1648 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' needs integer operands, but one side is ", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _bo_bad)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: convert it to an int first.", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1648); _tr_str_release(_at_t1648); });
            }
#line 6490 "src/sema.tr"
            if ((((((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("Dict", 4LL)))) && ((((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("**", 2LL))))) && (!((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("Tuple", 5LL))) && _tr_str_eqv((_bo_rf), (_tr_str_lit_len("Tuple", 5LL)))) && _tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))))))) {
#line 6491 "src/sema.tr"
                TrStr _bo_t = _tr_str_retain(_bo_lf);
#line 6492 "src/sema.tr"
                if ((!(_tr_str_eqv((_bo_lf), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 6492 "src/sema.tr"
                    TrStr _strtmp_t1649 = _tr_str_retain(_bo_rf);
                    _tr_str_release(_bo_t);
                    _bo_t = _strtmp_t1649;
                }
#line 6493 "src/sema.tr"
                ({ __auto_type _ord_t1650 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' cannot be applied to ", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _bo_t)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: check that the variable holds the number you meant (it holds ", 74LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1651 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1650); TrStr _tr_cr = (Sema_family_word(self, _bo_t)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" here).", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1651); _tr_str_release(_at_t1651); });
            }
#line 6494 "src/sema.tr"
            if (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("None", 4LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("None", 4LL)))) && ((((((((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))))) {
#line 6495 "src/sema.tr"
                ({ TrStr _at_t1652 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' cannot be applied to None.\n      FIX: the value here is None; give the variable a real value before using it in arithmetic or comparisons.", 140LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1652); _tr_str_release(_at_t1652); });
            }
#line 6496 "src/sema.tr"
            if (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("Option", 6LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("Option", 6LL)))) && ((((((((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))))) {
#line 6497 "src/sema.tr"
                ({ TrStr _at_t1653 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' cannot be applied to an Option value directly.\n      FIX: unwrap it first: `x.unwrap()`, `x.unwrap_or(default)`, or check `x.is_some()`.", 138LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1653); _tr_str_release(_at_t1653); });
            }
#line 6498 "src/sema.tr"
            if (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL)))) && ((((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))))) {
#line 6499 "src/sema.tr"
                ({ TrStr _at_t1654 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not defined for strings.\n      FIX: only '+' (concatenate), comparisons, 'str * int' (repeat) and 'in' work on strings; build text with f-strings or str(x).", 161LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1654); _tr_str_release(_at_t1654); });
            }
#line 6500 "src/sema.tr"
            if (((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("not in", 6LL)))) && (!_tr_str_eqv((_bo_lf), (_tr_str_lit_len("", 0LL)))))) {
#line 6501 "src/sema.tr"
                AstType* _in_t = hir_expr_type(hright);
#line 6502 "src/sema.tr"
                if ((((((_tr_str_eqv((_in_t->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((_in_t->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((_in_t->name), (_tr_str_lit_len("Set", 3LL)))) && (_in_t->args->len > 0LL)) && (_tr_str_eqv((_bo_rf), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("", 0LL))))) && ({ TrStr _at_t1655 = (Sema_decl_family(self, (*((AstType**)List_ptr_get(_in_t->args, 0LL))))); __auto_type _wr = (Sema_family_mismatch(self, _at_t1655, _bo_lf)); _tr_str_release(_at_t1655); _wr; }))) {
#line 6503 "src/sema.tr"
                    ({ __auto_type _ord_t1656 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, _bo_lf)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: testing ", 29LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" for membership in a ", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1657 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1656); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_in_t->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1658 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1657); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(_in_t->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("].\n      FIX: look for a '", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1659 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1658); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(_in_t->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' value.", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1659); _tr_str_release(_at_t1659); });
                }
            }
#line 6504 "src/sema.tr"
            if ((_tr_str_eqv((op), (_tr_str_lit_len("is", 2LL))) && (!_tr_str_eqv((_bo_rf), (_tr_str_lit_len("", 0LL)))))) {
#line 6505 "src/sema.tr"
                ({ TrStr _at_t1660 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, _bo_rf)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the right-hand side of 'is' must be an enum variant or a type, not ", 88LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: compare values with '==' (e.g. `x == 3`); use `is` only as `shape is Shape.Circle`.", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1660); _tr_str_release(_at_t1660); });
            }
#line 6506 "src/sema.tr"
            if ((((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("not in", 6LL)))) && ((_tr_str_eqv((_bo_rf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("bool", 4LL))))) && (!(((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))))))) {
#line 6507 "src/sema.tr"
                ({ TrStr _at_t1661 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the right-hand side of '", 45LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' must be a list, dict, set or string, but it is ", 49LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _bo_rf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: put the collection (or the string to search in) on the right: `item in collection`.", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1661); _tr_str_release(_at_t1661); });
            }
#line 6508 "src/sema.tr"
            bool _bo_ls = (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))));
#line 6509 "src/sema.tr"
            bool _bo_rs = (((_tr_str_eqv((_bo_rf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL))));
#line 6510 "src/sema.tr"
            if ((_bo_ls && _bo_rs)) {
#line 6511 "src/sema.tr"
                bool _bo_mixed = (_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) != _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL))));
#line 6512 "src/sema.tr"
                bool _bo_cmp = (((((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL))));
#line 6513 "src/sema.tr"
                bool _bo_arith = ((((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("**", 2LL))));
#line 6514 "src/sema.tr"
                if ((_bo_mixed && (_bo_cmp || _bo_arith))) {
#line 6515 "src/sema.tr"
                    if ((!(_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL))) && (_tr_str_eqv((_bo_lf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("int", 3LL))))))) {
#line 6516 "src/sema.tr"
                        ({ __auto_type _ord_t1662 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: cannot apply '", 35LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _bo_lf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1663 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1662); TrStr _tr_cr = (Sema_family_word(self, _bo_rf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: convert one side first, e.g. str(x) or int(s), so both operands have the same type.", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1663); _tr_str_release(_at_t1663); });
                    }
                } else if (((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("not in", 6LL)))) && (((_tr_str_eqv((_bo_rf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("bool", 4LL)))) || (_tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL))) && (!_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL)))))))) {
#line 6518 "src/sema.tr"
                    ({ TrStr _at_t1664 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the right-hand side of '", 45LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' must be a list, dict, set or string, but it is ", 49LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _bo_rf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: put the collection (or the string to search in) on the right: `item in collection`.", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1664); _tr_str_release(_at_t1664); });
                } else if (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) && _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL)))) && (((((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))))) {
#line 6520 "src/sema.tr"
                    ({ TrStr _at_t1665 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not defined for two strings.\n      FIX: only '+' (concatenate), comparisons, and 'str * int' (repeat) work on strings.", 123LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1665); _tr_str_release(_at_t1665); });
                }
            }
        }
#line 6522 "src/sema.tr"
        if ((self->no_heap && (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))))) {
#line 6523 "src/sema.tr"
            TrStr _nh_l = _tr_str_retain(hir_expr_type(hleft)->name);
#line 6524 "src/sema.tr"
            TrStr _nh_r = _tr_str_retain(hir_expr_type(hright)->name);
#line 6525 "src/sema.tr"
            if ((((_tr_str_eqv((_nh_l), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_nh_l), (_tr_str_lit_len("String", 6LL)))) || _tr_str_eqv((_nh_r), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((_nh_r), (_tr_str_lit_len("String", 6LL))))) {
#line 6526 "src/sema.tr"
                TrStr _nh_what = _tr_str_lit_len("string concatenation (+)", 24LL);
#line 6527 "src/sema.tr"
                if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 6527 "src/sema.tr"
                    TrStr _strtmp_t1666 = _tr_str_lit_len("string repetition (*)", 21LL);
                    _tr_str_release(_nh_what);
                    _nh_what = _strtmp_t1666;
                }
#line 6528 "src/sema.tr"
                Sema_check_no_heap(self, _nh_what, _tr_str_lit_len("keep string literals (they live in rodata) or use a StrView / fixed byte buffer; do not build new strings", 105LL));
            }
        }
#line 6535 "src/sema.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL))))) {
#line 6536 "src/sema.tr"
            TrStr lref = Sema_type_ref_name(self, left);
#line 6537 "src/sema.tr"
            TrStr rref = Sema_type_ref_name(self, right);
#line 6538 "src/sema.tr"
            if (((!_tr_str_eqv((lref), (_tr_str_lit_len("", 0LL)))) || (!_tr_str_eqv((rref), (_tr_str_lit_len("", 0LL)))))) {
#line 6539 "src/sema.tr"
                TrStr lname = _tr_str_retain(lref);
#line 6540 "src/sema.tr"
                if (_tr_str_eqv((lname), (_tr_str_lit_len("", 0LL)))) {
#line 6540 "src/sema.tr"
                    TrStr _strtmp_t1667 = _tr_str_retain(hir_expr_type(hleft)->name);
                    _tr_str_release(lname);
                    lname = _strtmp_t1667;
                }
#line 6541 "src/sema.tr"
                TrStr rname = _tr_str_retain(rref);
#line 6542 "src/sema.tr"
                if (_tr_str_eqv((rname), (_tr_str_lit_len("", 0LL)))) {
#line 6542 "src/sema.tr"
                    TrStr _strtmp_t1668 = _tr_str_retain(hir_expr_type(hright)->name);
                    _tr_str_release(rname);
                    rname = _strtmp_t1668;
                }
#line 6543 "src/sema.tr"
                HirExpr* _trret263 = ({ __auto_type _ord_t1669 = (_tr_str_lenv((lname))); __auto_type _ord_t1670 = (AstType_init(_tr_str_lit_len("str", 3LL))); __auto_type _ord_t1671 = (box_hirexpr(HirExpr_ctor_ELitStr(lname, _ord_t1669, _ord_t1670))); __auto_type _ord_t1672 = (_tr_str_lenv((rname))); __auto_type _ord_t1673 = (AstType_init(_tr_str_lit_len("str", 3LL))); __auto_type _ord_t1674 = (box_hirexpr(HirExpr_ctor_ELitStr(rname, _ord_t1672, _ord_t1673))); __auto_type _ord_t1675 = (AstType_init(_tr_str_lit_len("bool", 4LL))); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EBinOp(op, _ord_t1671, _ord_t1674, _ord_t1675))); _wr; });
                _tr_str_release(lref);
                _tr_str_release(rref);
                _tr_str_release(lname);
                _tr_str_release(rname);
                return _trret263;
            }
        }
#line 6544 "src/sema.tr"
        AstType* bin_ty = hir_expr_type(hleft);
#line 6545 "src/sema.tr"
        if (_tr_str_eqv((bin_ty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 6545 "src/sema.tr"
            bin_ty = hir_expr_type(hright);
        }
#line 6546 "src/sema.tr"
        if ((((((((((((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("or", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("&&", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("||", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("in", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("not in", 6LL))))) {
#line 6547 "src/sema.tr"
            bin_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
        } else if ((_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL))) && (((_tr_str_eqv((hir_expr_type(hleft)->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((hir_expr_type(hleft)->name), (_tr_str_lit_len("String", 6LL)))) || _tr_str_eqv((hir_expr_type(hright)->name), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((hir_expr_type(hright)->name), (_tr_str_lit_len("String", 6LL)))))) {
#line 6549 "src/sema.tr"
            bin_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if (((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) && (_binop_is_float_name(hir_expr_type(hleft)->name) || _binop_is_float_name(hir_expr_type(hright)->name)))) {
#line 6551 "src/sema.tr"
            bin_ty = AstType_init(_tr_str_lit_len("float", 5LL));
        }
#line 6552 "src/sema.tr"
        HirExpr* _trret264 = box_hirexpr(HirExpr_ctor_EBinOp(op, hleft, hright, bin_ty));
        return _trret264;
    } else if (_t1640.tag == Expr_EUnaryOp) {
        __auto_type op = _t1640.data.EUnaryOp.op;
__auto_type expr = _t1640.data.EUnaryOp.expr;
#line 6554 "src/sema.tr"
        HirExpr* hexpr_inner = Sema_lower_expr(self, expr);
#line 6555 "src/sema.tr"
        if (((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) && ({ TrStr _cmpa_t1676 = (Sema_firm_family(self, hexpr_inner)); TrStr _cmpb_t1677 = (_tr_str_lit_len("str", 3LL)); _Bool _cmpr_t1678 = _tr_str_eqv(_cmpa_t1676, _cmpb_t1677); _tr_str_release(_cmpa_t1676); _cmpr_t1678; }))) {
#line 6556 "src/sema.tr"
            ({ TrStr _at_t1679 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: unary '", 28LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' cannot be applied to a str.\n      FIX: convert it first, e.g. -int(s).", 72LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1679); _tr_str_release(_at_t1679); });
        }
#line 6557 "src/sema.tr"
        AstType* inner_ty = hir_expr_type(hexpr_inner);
#line 6558 "src/sema.tr"
        AstType* un_ty = inner_ty;
#line 6559 "src/sema.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
#line 6560 "src/sema.tr"
            AstType* addr_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
#line 6561 "src/sema.tr"
            ({ __auto_type _ord_t1680 = (box_asttype(inner_ty)); List_ptr_append(addr_ty->args, _ord_t1680); });
#line 6562 "src/sema.tr"
            un_ty = addr_ty;
        } else if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 6564 "src/sema.tr"
            if ((_tr_str_eqv((inner_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (inner_ty->args->len > 0LL))) {
#line 6565 "src/sema.tr"
                un_ty = (*((AstType**)List_ptr_get(inner_ty->args, 0LL)));
            }
        }
#line 6566 "src/sema.tr"
        HirExpr* _trret265 = box_hirexpr(HirExpr_ctor_EUnaryOp(op, hexpr_inner, un_ty));
        return _trret265;
    } else if (_t1640.tag == Expr_ECall) {
        __auto_type callee = _t1640.data.ECall.callee;
__auto_type args = _t1640.data.ECall.args;
#line 6569 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6570 "src/sema.tr"
            __auto_type _t1681 = (*callee);
            if (_t1681.tag == Expr_EIdent) {
                __auto_type ato_n = _t1681.data.EIdent.name;
#line 6573 "src/sema.tr"
                if ((self->no_heap && Sema__is_rc_class(self, ato_n))) {
#line 6574 "src/sema.tr"
                    ({ TrStr _at_t1682 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("constructing heap class '", 25LL)), (ato_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_no_heap(self, _at_t1682, _tr_str_lit_len("mark the class '@value_type' (stack; all-scalar fields), or place the instance in caller-owned memory via a Pointer[T]", 118LL)); _tr_str_release(_at_t1682); });
                }
#line 6575 "src/sema.tr"
                if (_tr_str_eqv((ato_n), (_tr_str_lit_len("await_timeout", 13LL)))) {
#line 6576 "src/sema.tr"
                    if ((!self->in_async_fn)) {
#line 6577 "src/sema.tr"
                        Sema_error(self, _tr_str_lit_len("[C-4] 'await_timeout' used outside an async function.", 53LL));
                    }
#line 6578 "src/sema.tr"
                    if ((args->len < 2LL)) {
#line 6579 "src/sema.tr"
                        Sema_error(self, _tr_str_lit_len("await_timeout requires 2 arguments: await_timeout(expr, timeout_ms)", 67LL));
#line 6580 "src/sema.tr"
                        HirExpr* _trret266 = box_hirexpr(HirExpr_ctor_ELitInt(0LL, AstType_init(_tr_str_lit_len("int", 3LL))));
                        return _trret266;
                    }
#line 6581 "src/sema.tr"
                    HirExpr* ato_inner = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
#line 6582 "src/sema.tr"
                    HirExpr* ato_ms = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 1LL)));
#line 6583 "src/sema.tr"
                    AstType* ato_ty = hir_expr_type(ato_inner);
#line 6584 "src/sema.tr"
                    HirExpr* _trret267 = box_hirexpr(HirExpr_ctor_EAwaitTimeout(ato_inner, ato_ms, ato_ty));
                    return _trret267;
                }
#line 6588 "src/sema.tr"
                if (_tr_str_eqv((ato_n), (_tr_str_lit_len("instanceOf", 10LL)))) {
#line 6589 "src/sema.tr"
                    if ((args->len < 2LL)) {
#line 6590 "src/sema.tr"
                        Sema_error(self, _tr_str_lit_len("instanceOf requires 2 arguments: instanceOf(obj, T)", 51LL));
#line 6591 "src/sema.tr"
                        HirExpr* _trret268 = box_hirexpr(HirExpr_ctor_ELitBool(false, AstType_init(_tr_str_lit_len("bool", 4LL))));
                        return _trret268;
                    }
#line 6592 "src/sema.tr"
                    HirExpr* io_obj = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
#line 6593 "src/sema.tr"
                    TrStr io_obj_ty_n = _tr_str_retain(hir_expr_type(io_obj)->name);
#line 6594 "src/sema.tr"
                    TrStr io_target_n = _tr_str_lit_len("", 0LL);
#line 6595 "src/sema.tr"
                    __auto_type _t1683 = (*((Expr*)List_ptr_get(args, 1LL)));
                    if (_t1683.tag == Expr_EIdent) {
                        __auto_type io_tn = _t1683.data.EIdent.name;
                        TrStr _strtmp_t1684 = _tr_str_retain(io_tn);
                        _tr_str_release(io_target_n);
                        io_target_n = _strtmp_t1684;
                    } else if (_t1683.tag == Expr_EIndex) {
                        __auto_type io_base = _t1683.data.EIndex.obj;
#line 6598 "src/sema.tr"
                        __auto_type _t1685 = (*io_base);
                        if (_t1685.tag == Expr_EIdent) {
                            __auto_type io_tn2 = _t1685.data.EIdent.name;
                            TrStr _strtmp_t1686 = _tr_str_retain(io_tn2);
                            _tr_str_release(io_target_n);
                            io_target_n = _strtmp_t1686;
                        } else if (1) {
                            __auto_type _ = _t1685;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t1683;
                        /* pass */
                    }
#line 6602 "src/sema.tr"
                    HirExpr* _trret269 = box_hirexpr(HirExpr_ctor_ELitBool(_tr_str_eqv((io_obj_ty_n), (io_target_n)), AstType_init(_tr_str_lit_len("bool", 4LL))));
                    _tr_str_release(io_obj_ty_n);
                    _tr_str_release(io_target_n);
                    return _trret269;
                }
#line 6606 "src/sema.tr"
                if (_tr_str_eqv((ato_n), (_tr_str_lit_len("inspect", 7LL)))) {
#line 6607 "src/sema.tr"
                    if ((args->len < 1LL)) {
#line 6608 "src/sema.tr"
                        Sema_error(self, _tr_str_lit_len("inspect requires 1 argument: inspect(T)", 39LL));
#line 6609 "src/sema.tr"
                        HirExpr* _trret270 = box_hirexpr(HirExpr_ctor_ELitStr(_tr_str_lit_len("", 0LL), 0LL, AstType_init(_tr_str_lit_len("str", 3LL))));
                        return _trret270;
                    }
#line 6610 "src/sema.tr"
                    TrStr isp_target_n = _tr_str_lit_len("", 0LL);
#line 6611 "src/sema.tr"
                    __auto_type _t1687 = (*((Expr*)List_ptr_get(args, 0LL)));
                    if (_t1687.tag == Expr_EIdent) {
                        __auto_type isp_tn = _t1687.data.EIdent.name;
                        TrStr _strtmp_t1688 = _tr_str_retain(isp_tn);
                        _tr_str_release(isp_target_n);
                        isp_target_n = _strtmp_t1688;
                    } else if (_t1687.tag == Expr_EIndex) {
                        __auto_type isp_base = _t1687.data.EIndex.obj;
#line 6614 "src/sema.tr"
                        __auto_type _t1689 = (*isp_base);
                        if (_t1689.tag == Expr_EIdent) {
                            __auto_type isp_tn2 = _t1689.data.EIdent.name;
                            TrStr _strtmp_t1690 = _tr_str_retain(isp_tn2);
                            _tr_str_release(isp_target_n);
                            isp_target_n = _strtmp_t1690;
                        } else if (1) {
                            __auto_type _ = _t1689;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t1687;
                        /* pass */
                    }
#line 6618 "src/sema.tr"
                    if (_tr_str_eqv((isp_target_n), (_tr_str_lit_len("", 0LL)))) {
#line 6619 "src/sema.tr"
                        HirExpr* isp_obj = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
#line 6620 "src/sema.tr"
                        TrStr _strtmp_t1691 = _tr_str_retain(hir_expr_type(isp_obj)->name);
                        _tr_str_release(isp_target_n);
                        isp_target_n = _strtmp_t1691;
                    }
#line 6621 "src/sema.tr"
                    TrStr isp_str = Sema_build_inspect_str(self, isp_target_n);
#line 6622 "src/sema.tr"
                    HirExpr* _trret271 = ({ __auto_type _ord_t1692 = (_tr_str_lenv((isp_str))); __auto_type _ord_t1693 = (AstType_init(_tr_str_lit_len("str", 3LL))); __auto_type _wr = (box_hirexpr(HirExpr_ctor_ELitStr(isp_str, _ord_t1692, _ord_t1693))); _wr; });
                    _tr_str_release(isp_target_n);
                    _tr_str_release(isp_str);
                    return _trret271;
                }
            } else if (1) {
                __auto_type _ = _t1681;
                /* pass */
            }
        }
#line 6625 "src/sema.tr"
        List_TrStr* p23_borrow_names = (void*)List_TrStr_new();
#line 6626 "src/sema.tr"
        TrMap* p23_seen = _tr_dict_new(4LL);
#line 6627 "src/sema.tr"
        long long p23_k = 0LL;
#line 6628 "src/sema.tr"
        while ((p23_k < args->len)) {
#line 6629 "src/sema.tr"
            Expr* p23_arg = ((Expr*)List_ptr_get(args, p23_k));
#line 6630 "src/sema.tr"
            TrStr p23_nm = _tr_str_lit_len("", 0LL);
#line 6631 "src/sema.tr"
            if ((((unsigned long long)(p23_arg)) != ((unsigned long long)(0LL)))) {
#line 6632 "src/sema.tr"
                __auto_type _t1694 = (*p23_arg);
                if (_t1694.tag == Expr_EIdent) {
                    __auto_type p23_n = _t1694.data.EIdent.name;
                    TrStr _strtmp_t1695 = _tr_str_retain(p23_n);
                    _tr_str_release(p23_nm);
                    p23_nm = _strtmp_t1695;
                } else if (1) {
                    __auto_type _ = _t1694;
                    /* pass */
                }
            }
#line 6635 "src/sema.tr"
            if ((!_tr_str_eqv((p23_nm), (_tr_str_lit_len("", 0LL))))) {
#line 6636 "src/sema.tr"
                Symbol* p23_sym = Sema_resolve(self, p23_nm);
#line 6637 "src/sema.tr"
                if (((!Sema_is_primitive(self, (*p23_sym->ty))) && (!_tr_str_eqv((p23_sym->name), (_tr_str_lit_len("", 0LL)))))) {
#line 6638 "src/sema.tr"
                    Sema_mark_borrow(self, p23_nm);
#line 6639 "src/sema.tr"
                    List_TrStr_append(p23_borrow_names, p23_nm);
#line 6640 "src/sema.tr"
                    if (_tr_dict_contains(p23_seen, _tr_strz(p23_nm))) {
#line 6641 "src/sema.tr"
                        ({ TrStr _at_t1696 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-3] '", 7LL)), (p23_nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' appears twice in the same call, creating aliased mutable access.\n      FIX: Clone one of the arguments: ", 106LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p23_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".clone()", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1696); _tr_str_release(_at_t1696); });
                    }
#line 6642 "src/sema.tr"
                    _tr_dict_set(p23_seen, _tr_strz(p23_nm), true);
                }
            }
#line 6643 "src/sema.tr"
            p23_k = (p23_k + 1LL);
            _tr_str_release(p23_nm);
        }
#line 6644 "src/sema.tr"
        List_ptr* hl = (void*)List_ptr_new();
#line 6645 "src/sema.tr"
        TrStr _ch_fname = _tr_str_lit_len("", 0LL);
#line 6646 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6647 "src/sema.tr"
            __auto_type _t1697 = (*callee);
            if (_t1697.tag == Expr_EIdent) {
                __auto_type _ch_n = _t1697.data.EIdent.name;
                TrStr _strtmp_t1698 = _tr_str_retain(_ch_n);
                _tr_str_release(_ch_fname);
                _ch_fname = _strtmp_t1698;
            } else if (1) {
                __auto_type _ = _t1697;
                /* pass */
            }
        }
#line 6650 "src/sema.tr"
        long long k = 0LL;
#line 6651 "src/sema.tr"
        while ((k < args->len)) {
#line 6652 "src/sema.tr"
            self->closure_hint = (AstType**)(0LL);
#line 6653 "src/sema.tr"
            if ((!_tr_str_eqv((_ch_fname), (_tr_str_lit_len("", 0LL))))) {
#line 6654 "src/sema.tr"
                __auto_type _t1699 = (*((Expr*)List_ptr_get(args, k)));
                if (_t1699.tag == Expr_EClosure) {
                    self->closure_hint = Sema_closure_hint_for_fn(self, _ch_fname, k);
                } else if (_t1699.tag == Expr_EList) {
                    __auto_type _lh_items = _t1699.data.EList.items;
#line 6657 "src/sema.tr"
                    if ((_lh_items->len == 0LL)) {
#line 6657 "src/sema.tr"
                        self->list_hint = Sema_list_hint_for_fn(self, _ch_fname, k);
                    }
                } else if (1) {
                    __auto_type _ = _t1699;
                    /* pass */
                }
            }
#line 6659 "src/sema.tr"
            HirExpr* harg = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k)));
#line 6661 "src/sema.tr"
            if (((!_tr_str_eqv((_ch_fname), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->fn_sigs, _tr_strz(_ch_fname)))) {
#line 6662 "src/sema.tr"
                AstType* sig_n = ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(_ch_fname)));
#line 6663 "src/sema.tr"
                if (((k < (sig_n->args->len - 1LL)) && (((unsigned long long)(((AstType**)List_ptr_get(sig_n->args, k)))) != ((unsigned long long)(0LL))))) {
#line 6664 "src/sema.tr"
                    harg = Sema_retype_none(self, harg, (*((AstType**)List_ptr_get(sig_n->args, k))));
                }
            }
#line 6665 "src/sema.tr"
            List_ptr_append(hl, harg);
#line 6666 "src/sema.tr"
            self->closure_hint = (AstType**)(0LL);
#line 6667 "src/sema.tr"
            self->list_hint = (AstType**)(0LL);
#line 6668 "src/sema.tr"
            k = (k + 1LL);
        }
#line 6670 "src/sema.tr"
        long long p23_ui = 0LL;
#line 6671 "src/sema.tr"
        while ((p23_ui < p23_borrow_names->len)) {
#line 6672 "src/sema.tr"
            ({ TrStr _at_t1700 = (List_TrStr_get(p23_borrow_names, p23_ui)); Sema_unmark_borrow(self, _at_t1700); _tr_str_release(_at_t1700); });
#line 6673 "src/sema.tr"
            p23_ui = (p23_ui + 1LL);
        }
#line 6676 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6677 "src/sema.tr"
            __auto_type _t1701 = (*callee);
            if (_t1701.tag == Expr_EIdent) {
                __auto_type vfn_n = _t1701.data.EIdent.name;
#line 6679 "src/sema.tr"
                if (_tr_dict_contains(self->variadic_fns, _tr_strz(vfn_n))) {
#line 6680 "src/sema.tr"
                    long long vfixed = _tr_str_to_int(_tr_strz(_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->variadic_fns, _tr_strz(vfn_n))))));
#line 6681 "src/sema.tr"
                    AstType* velem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 6682 "src/sema.tr"
                    if (_tr_dict_contains(self->variadic_elem_ty, _tr_strz(vfn_n))) {
#line 6683 "src/sema.tr"
                        velem_ty = (*((AstType**)(uintptr_t)_tr_dict_get(self->variadic_elem_ty, _tr_strz(vfn_n))));
                    }
#line 6684 "src/sema.tr"
                    if ((hl->len >= vfixed)) {
#line 6685 "src/sema.tr"
                        List_ptr* vargs = (void*)List_ptr_new();
#line 6686 "src/sema.tr"
                        long long vi = vfixed;
#line 6687 "src/sema.tr"
                        while ((vi < hl->len)) {
#line 6688 "src/sema.tr"
                            List_ptr_append(vargs, ((HirExpr*)List_ptr_get(hl, vi)));
#line 6689 "src/sema.tr"
                            vi = (vi + 1LL);
                        }
#line 6690 "src/sema.tr"
                        AstType* vlist_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 6691 "src/sema.tr"
                        ({ __auto_type _ord_t1702 = (box_asttype(velem_ty)); List_ptr_append(vlist_ty->args, _ord_t1702); });
#line 6692 "src/sema.tr"
                        List_ptr* vnew_hl = (void*)List_ptr_new();
#line 6693 "src/sema.tr"
                        long long vk = 0LL;
#line 6694 "src/sema.tr"
                        while ((vk < vfixed)) {
#line 6695 "src/sema.tr"
                            List_ptr_append(vnew_hl, ((HirExpr*)List_ptr_get(hl, vk)));
#line 6696 "src/sema.tr"
                            vk = (vk + 1LL);
                        }
#line 6697 "src/sema.tr"
                        List_ptr_append(vnew_hl, box_hirexpr(HirExpr_ctor_EList(vargs, vlist_ty)));
#line 6698 "src/sema.tr"
                        hl = vnew_hl;
                    }
                }
            } else if (1) {
                __auto_type _ = _t1701;
                /* pass */
            }
        }
#line 6700 "src/sema.tr"
        HirExpr* hcallee = Sema_lower_expr(self, callee);
#line 6701 "src/sema.tr"
        AstType* ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 6702 "src/sema.tr"
        if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
#line 6702 "src/sema.tr"
            HirExpr* _trret272 = box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
            List_TrStr_free(p23_borrow_names);
            Dict_free(p23_seen);
            _tr_str_release(_ch_fname);
            return _trret272;
        }
#line 6703 "src/sema.tr"
        __auto_type _t1703 = (*callee);
        if (_t1703.tag == Expr_EIdent) {
            __auto_type n = _t1703.data.EIdent.name;
#line 6706 "src/sema.tr"
            Sema_check_call_bounds(self, n, hl);
#line 6707 "src/sema.tr"
            Sema_check_call_fn_args(self, n, hl);
#line 6708 "src/sema.tr"
            if (_tr_str_eqv((n), (_tr_str_lit_len("main", 4LL)))) {
#line 6709 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[E-1] Explicit call to 'main()' is forbidden. The compiler automatically invokes main() as the program entry point. Remove the 'main()' call from your source.", 158LL));
#line 6710 "src/sema.tr"
                HirExpr* _trret273 = box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
                List_TrStr_free(p23_borrow_names);
                Dict_free(p23_seen);
                _tr_str_release(_ch_fname);
                return _trret273;
            }
#line 6711 "src/sema.tr"
            if ((((((((_tr_dict_contains(self->classes, _tr_strz(n)) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6712 "src/sema.tr"
                ret_ty = AstType_init(n);
            } else if (_tr_dict_contains(self->enums, _tr_strz(n))) {
#line 6714 "src/sema.tr"
                ret_ty = AstType_init(n);
            } else if ((_tr_str_eqv((Sema_resolve(self, n)->name), (_tr_str_lit_len("", 0LL))) && Sema__is_type_param_in_scope(self, n))) {
#line 6723 "src/sema.tr"
                ret_ty = AstType_init(n);
            } else if ((((((_tr_str_eqv((n), (_tr_str_lit_len("min", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("sum", 3LL)))) && (hl->len == 1LL)) && (hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))->args->len > 0LL)) && (_tr_str_eqv((hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))->name), (_tr_str_lit_len("Vec", 3LL)))))) {
#line 6725 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))->args, 0LL)));
            } else if ((((_tr_str_eqv((n), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("max", 3LL)))) && _hl_has_float(hl))) {
#line 6727 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("float", 5LL));
            } else if (_tr_str_eqv((n), (_tr_str_lit_len("c_callback", 10LL)))) {
#line 6730 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
#line 6731 "src/sema.tr"
                ({ __auto_type _ord_t1704 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1704); });
            } else if ((_tr_str_eqv((n), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("dealloc", 7LL))))) {
#line 6733 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
#line 6734 "src/sema.tr"
                if ((!self->in_unsafe)) {
#line 6735 "src/sema.tr"
                    ({ TrStr _at_t1705 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[U-1] '", 7LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' used outside an 'unsafe:' block.\n      FIX: Wrap raw memory operations in 'unsafe:' to signal manual memory management, e.g.\n          unsafe:\n              p = ", 163LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T](n)", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1705); _tr_str_release(_at_t1705); });
                }
            } else {
#line 6737 "src/sema.tr"
                Symbol* _fsym = Sema_resolve(self, n);
#line 6738 "src/sema.tr"
                ret_ty = (*_fsym->ty);
#line 6741 "src/sema.tr"
                if ((_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("def", 3LL))) && (ret_ty->args->len > 0LL))) {
#line 6742 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(ret_ty->args, (ret_ty->args->len - 1LL))));
                } else if ((_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("lambda", 6LL))) && (_fsym->kind.tag == SymbolKind_make_SVariable().tag))) {
#line 6747 "src/sema.tr"
                    if ((ret_ty->args->len > 0LL)) {
#line 6748 "src/sema.tr"
                        ret_ty = (*((AstType**)List_ptr_get(ret_ty->args, (ret_ty->args->len - 1LL))));
                    } else {
#line 6750 "src/sema.tr"
                        ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                    }
                }
#line 6754 "src/sema.tr"
                if (((_tr_dict_contains(self->extern_defs, _tr_strz(n)) && (!_tr_dict_contains(self->fn_defs, _tr_strz(n)))) && ({ __auto_type _ord_t1706 = (Sema_resolve(self, n)->kind); __auto_type _wr = ((_ord_t1706.tag != SymbolKind_make_SVariable().tag)); _wr; }))) {
#line 6755 "src/sema.tr"
                    ({ __auto_type _ord_t1708 = (((FunctionDef*)(uintptr_t)_tr_dict_get(self->extern_defs, _tr_strz(n)))); TrStr _at_t1707 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_call_args(self, _ord_t1708, hl, 0LL, _at_t1707); _tr_str_release(_at_t1707); });
                }
#line 6756 "src/sema.tr"
                if (_tr_dict_contains(self->fn_defs, _tr_strz(n))) {
#line 6757 "src/sema.tr"
                    FunctionDef* _gfd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(n)));
#line 6760 "src/sema.tr"
                    if ((((_gfd->generics->len > 0LL) && (_gfd->constraints->len == 0LL)) && (self->current_func_generics->len == 0LL))) {
#line 6761 "src/sema.tr"
                        long long _ngi = 0LL;
#line 6762 "src/sema.tr"
                        while ((_ngi < _gfd->generics->len)) {
#line 6763 "src/sema.tr"
                            TrStr _ngp = List_TrStr_get(_gfd->generics, _ngi);
#line 6764 "src/sema.tr"
                            bool _nfound = false;
#line 6765 "src/sema.tr"
                            long long _nsk = 0LL;
#line 6766 "src/sema.tr"
                            if (((_gfd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_gfd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 6766 "src/sema.tr"
                                _nsk = 1LL;
                            }
#line 6767 "src/sema.tr"
                            long long _npi = _nsk;
#line 6768 "src/sema.tr"
                            while ((_npi < _gfd->params->len)) {
#line 6769 "src/sema.tr"
                                AstType** _npty = ((Param*)List_ptr_get(_gfd->params, _npi))->ty;
#line 6770 "src/sema.tr"
                                if (((((unsigned long long)(_npty)) != ((unsigned long long)(0LL))) && type_mentions((*_npty), _ngp))) {
#line 6770 "src/sema.tr"
                                    _nfound = true;
                                }
#line 6771 "src/sema.tr"
                                _npi = (_npi + 1LL);
                            }
#line 6772 "src/sema.tr"
                            if (((!_nfound) && (_gfd->generics->len == 1LL))) {
#line 6773 "src/sema.tr"
                                ({ TrStr _at_t1709 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[G-2] Cannot infer type parameter '", 35LL)), (_ngp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of generic function '", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from its arguments.\n      FIX: give it explicitly: ", 53LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[Type](...)", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1709); _tr_str_release(_at_t1709); });
                            }
#line 6774 "src/sema.tr"
                            _ngi = (_ngi + 1LL);
                            _tr_str_release(_ngp);
                        }
                    }
#line 6775 "src/sema.tr"
                    if (({ __auto_type _ord_t1710 = (Sema_resolve(self, n)->kind); __auto_type _wr = ((_ord_t1710.tag == SymbolKind_make_SFunction().tag)); _wr; })) {
#line 6776 "src/sema.tr"
                        ({ TrStr _at_t1711 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_call_args(self, _gfd, hl, 0LL, _at_t1711); _tr_str_release(_at_t1711); });
                    }
#line 6777 "src/sema.tr"
                    if (((_gfd->generics->len > 0LL) && List_TrStr_contains(_gfd->generics, ret_ty->name))) {
#line 6778 "src/sema.tr"
                        long long _gsk = 0LL;
#line 6779 "src/sema.tr"
                        if (((_gfd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_gfd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 6779 "src/sema.tr"
                            _gsk = 1LL;
                        }
#line 6780 "src/sema.tr"
                        long long _gpi = 0LL;
#line 6781 "src/sema.tr"
                        while ((_gpi < _gfd->params->len)) {
#line 6782 "src/sema.tr"
                            AstType** _gpty = ((Param*)List_ptr_get(_gfd->params, _gpi))->ty;
#line 6783 "src/sema.tr"
                            if (((((unsigned long long)(_gpty)) != ((unsigned long long)(0LL))) && ({ __auto_type _ord_t1712 = ((*_gpty)->name); __auto_type _wr = (_tr_str_eqv((_ord_t1712), (ret_ty->name))); _wr; }))) {
#line 6784 "src/sema.tr"
                                long long _gaidx = (_gpi - _gsk);
#line 6785 "src/sema.tr"
                                if (((_gaidx >= 0LL) && (_gaidx < hl->len))) {
#line 6786 "src/sema.tr"
                                    ret_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _gaidx)));
#line 6787 "src/sema.tr"
                                    break;
                                }
                            } else if (((((unsigned long long)(_gpty)) != ((unsigned long long)(0LL))) && ((*_gpty)->args->len > 0LL))) {
#line 6789 "src/sema.tr"
                                long long _gaidx2 = (_gpi - _gsk);
#line 6790 "src/sema.tr"
                                if (((_gaidx2 >= 0LL) && (_gaidx2 < hl->len))) {
#line 6791 "src/sema.tr"
                                    TrStr _gut = ({ __auto_type _ord_t1713 = ((*_gpty)); __auto_type _wr = (unify_generic(_ord_t1713, hir_expr_type(((HirExpr*)List_ptr_get(hl, _gaidx2))), ret_ty->name)); _wr; });
#line 6792 "src/sema.tr"
                                    if ((!_tr_str_eqv((_gut), (_tr_str_lit_len("", 0LL))))) {
#line 6793 "src/sema.tr"
                                        ret_ty = AstType_init(_gut);
#line 6794 "src/sema.tr"
                                        _tr_str_release(_gut);
                                        break;
                                    }
                                }
                            }
#line 6795 "src/sema.tr"
                            _gpi = (_gpi + 1LL);
                        }
                    }
                }
            }
        } else if (_t1703.tag == Expr_EIndex) {
            __auto_type base = _t1703.data.EIndex.obj;
__auto_type idx = _t1703.data.EIndex._tr_v_index;
#line 6797 "src/sema.tr"
            __auto_type _t1714 = (*base);
            if (_t1714.tag == Expr_EIdent) {
                __auto_type gn = _t1714.data.EIdent.name;
#line 6799 "src/sema.tr"
                if ((_tr_str_eqv((gn), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((gn), (_tr_str_lit_len("dealloc", 7LL))))) {
#line 6800 "src/sema.tr"
                    __auto_type _t1715 = (*idx);
                    if (_t1715.tag == Expr_EIdent) {
                        __auto_type tn = _t1715.data.EIdent.name;
#line 6802 "src/sema.tr"
                        ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
#line 6803 "src/sema.tr"
                        ({ __auto_type _ord_t1716 = (box_asttype(AstType_init(tn))); List_ptr_append(ret_ty->args, _ord_t1716); });
                    } else if (1) {
                        __auto_type _ = _t1715;
#line 6805 "src/sema.tr"
                        ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
                    }
#line 6806 "src/sema.tr"
                    if ((!self->in_unsafe)) {
#line 6807 "src/sema.tr"
                        ({ TrStr _at_t1717 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[U-1] 'alloc'/'dealloc' used outside an 'unsafe:' block.\n      FIX: Wrap raw memory operations in 'unsafe:' to signal manual memory management, e.g.\n          unsafe:\n              p = ", 185LL)), (gn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T](n)", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1717); _tr_str_release(_at_t1717); });
                    }
                } else if (((((_tr_str_eqv((gn), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((gn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((gn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((gn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((gn), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6809 "src/sema.tr"
                    __auto_type _t1718 = (*idx);
                    if (_t1718.tag == Expr_EIdent) {
                        __auto_type tn = _t1718.data.EIdent.name;
#line 6811 "src/sema.tr"
                        ret_ty = AstType_init(gn);
#line 6812 "src/sema.tr"
                        ({ __auto_type _ord_t1719 = (box_asttype(AstType_init(tn))); List_ptr_append(ret_ty->args, _ord_t1719); });
                    } else if (1) {
                        __auto_type _ = _t1718;
#line 6814 "src/sema.tr"
                        ret_ty = AstType_init(gn);
                    }
                } else if (_tr_dict_contains(self->classes, _tr_strz(gn))) {
#line 6816 "src/sema.tr"
                    ret_ty = AstType_init(gn);
#line 6817 "src/sema.tr"
                    __auto_type _t1720 = (*idx);
                    if (_t1720.tag == Expr_EIdent) {
                        __auto_type targ_ca = _t1720.data.EIdent.name;
                        ({ __auto_type _ord_t1721 = (box_asttype(AstType_init(targ_ca))); List_ptr_append(ret_ty->args, _ord_t1721); });
                    } else if (1) {
                        __auto_type _ = _t1720;
                        /* pass */
                    }
                } else if (_tr_dict_contains(self->enums, _tr_strz(gn))) {
#line 6821 "src/sema.tr"
                    ret_ty = AstType_init(gn);
                } else {
#line 6828 "src/sema.tr"
                    __auto_type _t1722 = (*idx);
                    if (_t1722.tag == Expr_EIdent) {
                        __auto_type farg_c = _t1722.data.EIdent.name;
#line 6848 "src/sema.tr"
                        AstType* fret = AstType_init(farg_c);
#line 6849 "src/sema.tr"
                        if (_tr_dict_contains(self->fn_defs, _tr_strz(gn))) {
#line 6850 "src/sema.tr"
                            FunctionDef* _sf_fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(gn)));
#line 6851 "src/sema.tr"
                            if ((((unsigned long long)(_sf_fd->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 6851 "src/sema.tr"
                                fret = (*_sf_fd->ret_ty);
                            }
#line 6852 "src/sema.tr"
                            if (((_sf_fd->generics->len > 0LL) && ({ __auto_type _ord_t1723 = (List_TrStr_get(_sf_fd->generics, 0LL)); __auto_type _wr = (({ TrStr _cmpa_t1724 = (_ord_t1723); TrStr _cmpb_t1725 = (fret->name); _Bool _cmpr_t1726 = _tr_str_eqv(_cmpa_t1724, _cmpb_t1725); _tr_str_release(_cmpa_t1724); _cmpr_t1726; })); _wr; }))) {
#line 6853 "src/sema.tr"
                                fret = AstType_init(farg_c);
                            }
#line 6867 "src/sema.tr"
                            if ((((unsigned long long)(_sf_fd->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 6868 "src/sema.tr"
                                if ((!_tr_str_eqv(((*_sf_fd->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 6869 "src/sema.tr"
                                    AstType* _sf_result = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 6870 "src/sema.tr"
                                    ({ __auto_type _ord_t1727 = (box_asttype(fret)); List_ptr_append(_sf_result->args, _ord_t1727); });
#line 6871 "src/sema.tr"
                                    List_ptr_append(_sf_result->args, _sf_fd->throws_ty);
#line 6872 "src/sema.tr"
                                    fret = _sf_result;
                                }
                            }
                        }
#line 6873 "src/sema.tr"
                        ret_ty = fret;
#line 6874 "src/sema.tr"
                        hcallee = ({ TrStr _at_t1728 = (({ TrStr _tr_cl = (_tr_strx_concatv((gn), (_tr_str_lit_len("__MONO_", 7LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (farg_c)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t1728, fret, false))); _tr_str_release(_at_t1728); _wr; });
                    } else if (_t1722.tag == Expr_ETuple) {
                        __auto_type ftargs = _t1722.data.ETuple.items;
#line 6881 "src/sema.tr"
                        List_TrStr* _mf_names = (void*)List_TrStr_new();
#line 6882 "src/sema.tr"
                        long long _mfi = 0LL;
#line 6883 "src/sema.tr"
                        while ((_mfi < ftargs->len)) {
#line 6884 "src/sema.tr"
                            TrStr _mf_nm = _tr_str_lit_len("", 0LL);
#line 6885 "src/sema.tr"
                            __auto_type _t1729 = (*((Expr*)List_ptr_get(ftargs, _mfi)));
                            if (_t1729.tag == Expr_EIdent) {
                                __auto_type _mfn = _t1729.data.EIdent.name;
                                TrStr _strtmp_t1730 = _tr_str_retain(_mfn);
                                _tr_str_release(_mf_nm);
                                _mf_nm = _strtmp_t1730;
                            } else if (1) {
                                __auto_type _ = _t1729;
                                /* pass */
                            }
#line 6888 "src/sema.tr"
                            List_TrStr_append(_mf_names, _mf_nm);
#line 6889 "src/sema.tr"
                            _mfi = (_mfi + 1LL);
                            _tr_str_release(_mf_nm);
                        }
#line 6890 "src/sema.tr"
                        TrStr _mf_mangled = _tr_str_retain(gn);
#line 6891 "src/sema.tr"
                        long long _mf_ji = 0LL;
#line 6892 "src/sema.tr"
                        while ((_mf_ji < _mf_names->len)) {
#line 6893 "src/sema.tr"
                            if ((_mf_ji == 0LL)) {
#line 6893 "src/sema.tr"
                                ({ TrStr _ap_t1731 = (_tr_str_lit_len("__MONO_", 7LL)); TrStr _ap_t1732 = (List_TrStr_get(_mf_names, _mf_ji)); _tr_str_append(&_mf_mangled, _ap_t1731); _tr_str_append(&_mf_mangled, _ap_t1732); _tr_str_release(_ap_t1732); });
                            } else {
#line 6894 "src/sema.tr"
                                ({ TrStr _ap_t1733 = (_tr_str_lit_len("_", 1LL)); TrStr _ap_t1734 = (List_TrStr_get(_mf_names, _mf_ji)); _tr_str_append(&_mf_mangled, _ap_t1733); _tr_str_append(&_mf_mangled, _ap_t1734); _tr_str_release(_ap_t1734); });
                            }
#line 6895 "src/sema.tr"
                            _mf_ji = (_mf_ji + 1LL);
                        }
#line 6896 "src/sema.tr"
                        AstType* fret2 = AstType_init(_tr_str_lit_len("void", 4LL));
#line 6897 "src/sema.tr"
                        if (_tr_dict_contains(self->fn_defs, _tr_strz(gn))) {
#line 6898 "src/sema.tr"
                            FunctionDef* _mf_fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(gn)));
#line 6899 "src/sema.tr"
                            if ((((unsigned long long)(_mf_fd->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 6899 "src/sema.tr"
                                fret2 = (*_mf_fd->ret_ty);
                            }
#line 6900 "src/sema.tr"
                            long long _mf_gi = 0LL;
#line 6901 "src/sema.tr"
                            while ((_mf_gi < _mf_fd->generics->len)) {
#line 6902 "src/sema.tr"
                                if ((({ __auto_type _ord_t1735 = (List_TrStr_get(_mf_fd->generics, _mf_gi)); __auto_type _wr = (({ TrStr _cmpa_t1736 = (_ord_t1735); TrStr _cmpb_t1737 = (fret2->name); _Bool _cmpr_t1738 = _tr_str_eqv(_cmpa_t1736, _cmpb_t1737); _tr_str_release(_cmpa_t1736); _cmpr_t1738; })); _wr; }) && (_mf_gi < _mf_names->len))) {
#line 6903 "src/sema.tr"
                                    fret2 = ({ TrStr _at_t1739 = (List_TrStr_get(_mf_names, _mf_gi)); __auto_type _wr = (AstType_init(_at_t1739)); _tr_str_release(_at_t1739); _wr; });
                                }
#line 6904 "src/sema.tr"
                                _mf_gi = (_mf_gi + 1LL);
                            }
#line 6908 "src/sema.tr"
                            if ((((unsigned long long)(_mf_fd->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 6909 "src/sema.tr"
                                if ((!_tr_str_eqv(((*_mf_fd->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 6910 "src/sema.tr"
                                    AstType* _mf_result = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 6911 "src/sema.tr"
                                    ({ __auto_type _ord_t1740 = (box_asttype(fret2)); List_ptr_append(_mf_result->args, _ord_t1740); });
#line 6912 "src/sema.tr"
                                    List_ptr_append(_mf_result->args, _mf_fd->throws_ty);
#line 6913 "src/sema.tr"
                                    fret2 = _mf_result;
                                }
                            }
                        }
#line 6914 "src/sema.tr"
                        ret_ty = fret2;
#line 6915 "src/sema.tr"
                        hcallee = box_hirexpr(HirExpr_ctor_EIdent(_mf_mangled, fret2, false));
                        List_TrStr_free(_mf_names);
                        _tr_str_release(_mf_mangled);
                    } else if (1) {
                        __auto_type _ = _t1722;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1714;
                /* pass */
            }
        } else if (_t1703.tag == Expr_EPropAccess) {
            __auto_type obj = _t1703.data.EPropAccess.obj;
__auto_type variant = _t1703.data.EPropAccess.prop;
#line 6919 "src/sema.tr"
            __auto_type _t1741 = (*obj);
            if (_t1741.tag == Expr_EIdent) {
                __auto_type type_name = _t1741.data.EIdent.name;
#line 6921 "src/sema.tr"
                if (({ __auto_type _ord_t1742 = (Sema_resolve(self, type_name)->kind); __auto_type _wr = ((_ord_t1742.tag != SymbolKind_make_SVariable().tag)); _wr; })) {
#line 6921 "src/sema.tr"
                    Sema_check_static_call(self, type_name, variant, hl);
                }
#line 6922 "src/sema.tr"
                if (_tr_dict_contains(self->enums, _tr_strz(type_name))) {
#line 6923 "src/sema.tr"
                    ret_ty = AstType_init(type_name);
                } else if (((((_tr_str_eqv((type_name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((type_name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("ThreadLocal", 11LL)))) && ((_tr_str_eqv((variant), (_tr_str_lit_len("spawn", 5LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL)))))) {
#line 6926 "src/sema.tr"
                    ret_ty = AstType_init(type_name);
                } else if ((_tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL))))) {
#line 6928 "src/sema.tr"
                    if (((((_tr_str_eqv((type_name), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((type_name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6929 "src/sema.tr"
                        ret_ty = AstType_init(type_name);
                    } else if (_tr_dict_contains(self->classes, _tr_strz(type_name))) {
#line 6931 "src/sema.tr"
                        ret_ty = AstType_init(type_name);
                    }
                } else if (_tr_dict_contains(self->classes, _tr_strz(type_name))) {
#line 6936 "src/sema.tr"
                    ClassDef* _scdef = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(type_name)));
#line 6937 "src/sema.tr"
                    long long _smi = 0LL;
#line 6938 "src/sema.tr"
                    bool _sfound = false;
#line 6939 "src/sema.tr"
                    while (((_smi < _scdef->methods->len) && (!_sfound))) {
#line 6940 "src/sema.tr"
                        FunctionDef* _smdef = ((FunctionDef*)List_ptr_get(_scdef->methods, _smi));
#line 6941 "src/sema.tr"
                        if (_tr_str_eqv((_smdef->name), (variant))) {
#line 6942 "src/sema.tr"
                            long long _nself = _smdef->params->len;
#line 6943 "src/sema.tr"
                            if (((_smdef->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_smdef->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 6944 "src/sema.tr"
                                _nself = (_smdef->params->len - 1LL);
                            }
#line 6945 "src/sema.tr"
                            if ((_nself == hl->len)) {
#line 6946 "src/sema.tr"
                                if ((((unsigned long long)(_smdef->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 6947 "src/sema.tr"
                                    ret_ty = (*_smdef->ret_ty);
                                }
#line 6948 "src/sema.tr"
                                _sfound = true;
                            }
                        }
#line 6949 "src/sema.tr"
                        _smi = (_smi + 1LL);
                    }
#line 6950 "src/sema.tr"
                    if ((!_sfound)) {
#line 6951 "src/sema.tr"
                        _smi = 0LL;
#line 6952 "src/sema.tr"
                        while (((_smi < _scdef->methods->len) && (!_sfound))) {
#line 6953 "src/sema.tr"
                            FunctionDef* _smdef2 = ((FunctionDef*)List_ptr_get(_scdef->methods, _smi));
#line 6954 "src/sema.tr"
                            if (_tr_str_eqv((_smdef2->name), (variant))) {
#line 6955 "src/sema.tr"
                                if ((((unsigned long long)(_smdef2->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 6956 "src/sema.tr"
                                    ret_ty = (*_smdef2->ret_ty);
                                }
#line 6957 "src/sema.tr"
                                _sfound = true;
                            }
#line 6958 "src/sema.tr"
                            _smi = (_smi + 1LL);
                        }
                    }
                }
            } else if (_t1741.tag == Expr_EIndex) {
                __auto_type base2 = _t1741.data.EIndex.obj;
__auto_type idx2 = _t1741.data.EIndex._tr_v_index;
#line 6960 "src/sema.tr"
                if ((_tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL))))) {
#line 6961 "src/sema.tr"
                    __auto_type _t1743 = (*base2);
                    if (_t1743.tag == Expr_EIdent) {
                        __auto_type gn2 = _t1743.data.EIdent.name;
#line 6963 "src/sema.tr"
                        if (((((_tr_str_eqv((gn2), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((gn2), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((gn2), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((gn2), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((gn2), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6964 "src/sema.tr"
                            __auto_type _t1744 = (*idx2);
                            if (_t1744.tag == Expr_EIdent) {
                                __auto_type tn2 = _t1744.data.EIdent.name;
#line 6966 "src/sema.tr"
                                ret_ty = AstType_init(gn2);
#line 6967 "src/sema.tr"
                                ({ __auto_type _ord_t1745 = (box_asttype(AstType_init(tn2))); List_ptr_append(ret_ty->args, _ord_t1745); });
                            } else if (1) {
                                __auto_type _ = _t1744;
#line 6969 "src/sema.tr"
                                ret_ty = AstType_init(gn2);
                            }
                        } else if (_tr_dict_contains(self->classes, _tr_strz(gn2))) {
#line 6971 "src/sema.tr"
                            ret_ty = AstType_init(gn2);
#line 6972 "src/sema.tr"
                            __auto_type _t1746 = (*idx2);
                            if (_t1746.tag == Expr_EIdent) {
                                __auto_type targ_cb = _t1746.data.EIdent.name;
                                ({ __auto_type _ord_t1747 = (box_asttype(AstType_init(targ_cb))); List_ptr_append(ret_ty->args, _ord_t1747); });
                            } else if (1) {
                                __auto_type _ = _t1746;
                                /* pass */
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t1743;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1741;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t1703;
            /* pass */
        }
#line 6979 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6980 "src/sema.tr"
            __auto_type _t1748 = (*callee);
            if (_t1748.tag == Expr_EIdent) {
                __auto_type aa_nm = _t1748.data.EIdent.name;
#line 6982 "src/sema.tr"
                if (_tr_str_eqv((aa_nm), (_tr_str_lit_len("await_all", 9LL)))) {
#line 6983 "src/sema.tr"
                    long long aa_i = 0LL;
#line 6984 "src/sema.tr"
                    while ((aa_i < hl->len)) {
#line 6985 "src/sema.tr"
                        Sema_check_spawn_sendable(self, ((HirExpr*)List_ptr_get(hl, aa_i)));
#line 6986 "src/sema.tr"
                        aa_i = (aa_i + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t1748;
                /* pass */
            }
        }
#line 6989 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6990 "src/sema.tr"
            __auto_type _t1749 = (*callee);
            if (_t1749.tag == Expr_EIdent) {
                __auto_type df_nm = _t1749.data.EIdent.name;
#line 6992 "src/sema.tr"
                if ((_tr_str_eqv((df_nm), (_tr_str_lit_len("dealloc", 7LL))) && (args->len > 0LL))) {
#line 6993 "src/sema.tr"
                    Expr* df_arg0 = ((Expr*)List_ptr_get(args, 0LL));
#line 6994 "src/sema.tr"
                    if ((((unsigned long long)(df_arg0)) != ((unsigned long long)(0LL)))) {
#line 6995 "src/sema.tr"
                        __auto_type _t1750 = (*df_arg0);
                        if (_t1750.tag == Expr_EIdent) {
                            __auto_type df_ptr = _t1750.data.EIdent.name;
#line 6997 "src/sema.tr"
                            Symbol* df_sym = Sema_resolve(self, df_ptr);
#line 6998 "src/sema.tr"
                            if ((!_tr_str_eqv((df_sym->name), (_tr_str_lit_len("", 0LL))))) {
#line 6999 "src/sema.tr"
                                Sema_mark_freed(self, df_ptr);
                            }
                        } else if (1) {
                            __auto_type _ = _t1750;
                            /* pass */
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1749;
                /* pass */
            }
        }
#line 7002 "src/sema.tr"
        HirExpr* _trret274 = box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
        List_TrStr_free(p23_borrow_names);
        Dict_free(p23_seen);
        _tr_str_release(_ch_fname);
        return _trret274;
    } else if (_t1640.tag == Expr_EMethodCall) {
        __auto_type obj = _t1640.data.EMethodCall.obj;
__auto_type method = _t1640.data.EMethodCall.method;
__auto_type args = _t1640.data.EMethodCall.args;
#line 7004 "src/sema.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("__index__", 9LL))) && (args->len > 0LL))) {
#line 7005 "src/sema.tr"
            HirExpr* _trret275 = Sema_lower_expr(self, box_expr(Expr_ctor_EIndex(obj, ((Expr*)List_ptr_get(args, 0LL)))));
            return _trret275;
        }
#line 7008 "src/sema.tr"
        if (((self->no_heap && ((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("with_capacity", 13LL))))) && (((unsigned long long)(obj)) != ((unsigned long long)(0LL))))) {
#line 7009 "src/sema.tr"
            TrStr _nh_rn = _tr_str_lit_len("", 0LL);
#line 7010 "src/sema.tr"
            __auto_type _t1751 = (*obj);
            if (_t1751.tag == Expr_EIdent) {
                __auto_type _nhn = _t1751.data.EIdent.name;
                TrStr _strtmp_t1752 = _tr_str_retain(_nhn);
                _tr_str_release(_nh_rn);
                _nh_rn = _strtmp_t1752;
            } else if (_t1751.tag == Expr_EIndex) {
                __auto_type _nhbase = _t1751.data.EIndex.obj;
#line 7013 "src/sema.tr"
                if ((((unsigned long long)(_nhbase)) != ((unsigned long long)(0LL)))) {
#line 7014 "src/sema.tr"
                    __auto_type _t1753 = (*_nhbase);
                    if (_t1753.tag == Expr_EIdent) {
                        __auto_type _nhn2 = _t1753.data.EIdent.name;
                        TrStr _strtmp_t1754 = _tr_str_retain(_nhn2);
                        _tr_str_release(_nh_rn);
                        _nh_rn = _strtmp_t1754;
                    } else if (1) {
                        __auto_type _ = _t1753;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1751;
                /* pass */
            }
#line 7018 "src/sema.tr"
            if (((((_tr_str_eqv((_nh_rn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Set", 3LL))))) {
#line 7019 "src/sema.tr"
                ({ TrStr _at_t1755 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_nh_rn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() (a growable heap collection)", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_no_heap(self, _at_t1755, _tr_str_lit_len("use a fixed-size array [T; N] (stack), or parallel fixed arrays for a map", 73LL)); _tr_str_release(_at_t1755); });
            }
        }
#line 7020 "src/sema.tr"
        List_ptr* hl = (void*)List_ptr_new();
#line 7021 "src/sema.tr"
        long long k = 0LL;
#line 7022 "src/sema.tr"
        while ((k < args->len)) {
#line 7023 "src/sema.tr"
            self->closure_hint = (AstType**)(0LL);
#line 7024 "src/sema.tr"
            __auto_type _t1756 = (*((Expr*)List_ptr_get(args, k)));
            if (_t1756.tag == Expr_EClosure) {
                self->closure_hint = Sema_closure_hint_for_method(self, method, k);
            } else if (_t1756.tag == Expr_EList) {
                __auto_type _lhm_items = _t1756.data.EList.items;
#line 7027 "src/sema.tr"
                if ((_lhm_items->len == 0LL)) {
#line 7027 "src/sema.tr"
                    self->list_hint = Sema_list_hint_for_method(self, method, k);
                }
            } else if (1) {
                __auto_type _ = _t1756;
                /* pass */
            }
#line 7029 "src/sema.tr"
            List_ptr_append(hl, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k))));
#line 7030 "src/sema.tr"
            self->closure_hint = (AstType**)(0LL);
#line 7031 "src/sema.tr"
            self->list_hint = (AstType**)(0LL);
#line 7032 "src/sema.tr"
            k = (k + 1LL);
        }
#line 7033 "src/sema.tr"
        bool _saved_recv_mc = self->in_recv_pos;
#line 7034 "src/sema.tr"
        self->in_recv_pos = true;
#line 7035 "src/sema.tr"
        HirExpr* hobj = Sema_lower_expr(self, obj);
#line 7036 "src/sema.tr"
        self->in_recv_pos = _saved_recv_mc;
#line 7041 "src/sema.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
#line 7042 "src/sema.tr"
            __auto_type _t1757 = (*obj);
            if (_t1757.tag == Expr_EIdent) {
                __auto_type free_nm = _t1757.data.EIdent.name;
#line 7044 "src/sema.tr"
                Symbol* free_sym = Sema_resolve(self, free_nm);
#line 7045 "src/sema.tr"
                if ((!_tr_str_eqv((free_sym->name), (_tr_str_lit_len("", 0LL))))) {
#line 7045 "src/sema.tr"
                    Sema_mark_freed(self, free_nm);
                }
            } else if (1) {
                __auto_type _ = _t1757;
                /* pass */
            }
        }
#line 7047 "src/sema.tr"
        AstType* hobj_ty = hir_expr_type(hobj);
#line 7048 "src/sema.tr"
        if (true) {
#line 7049 "src/sema.tr"
            TrStr _ba_fam = Sema_firm_family(self, hobj);
#line 7050 "src/sema.tr"
            if (_tr_str_eqv((_ba_fam), (_tr_str_lit_len("", 0LL)))) {
#line 7051 "src/sema.tr"
                TrStr _ba_n = _tr_str_retain(hir_expr_type(hobj)->name);
#line 7052 "src/sema.tr"
                if ((((!_tr_dict_contains(self->classes, _tr_strz(_ba_n))) || _tr_str_eqv((_ba_n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((_ba_n), (_tr_str_lit_len("Vec", 3LL))))) {
#line 7053 "src/sema.tr"
                    if ((_tr_str_eqv((_ba_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((_ba_n), (_tr_str_lit_len("Vec", 3LL))))) {
#line 7053 "src/sema.tr"
                        TrStr _strtmp_t1758 = _tr_str_lit_len("List", 4LL);
                        _tr_str_release(_ba_fam);
                        _ba_fam = _strtmp_t1758;
                    } else if ((_tr_str_eqv((_ba_n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((_ba_n), (_tr_str_lit_len("Map", 3LL))))) {
#line 7054 "src/sema.tr"
                        TrStr _strtmp_t1759 = _tr_str_lit_len("Dict", 4LL);
                        _tr_str_release(_ba_fam);
                        _ba_fam = _strtmp_t1759;
                    } else if (_tr_str_eqv((_ba_n), (_tr_str_lit_len("str", 3LL)))) {
#line 7055 "src/sema.tr"
                        TrStr _strtmp_t1760 = _tr_str_lit_len("str", 3LL);
                        _tr_str_release(_ba_fam);
                        _ba_fam = _strtmp_t1760;
                    }
                }
            }
#line 7056 "src/sema.tr"
            Sema_check_builtin_method_arity(self, _ba_fam, method, hl->len);
#line 7057 "src/sema.tr"
            ({ TrStr _ord_t1761 = (Sema_firm_family(self, hobj)); __auto_type _ord_t1762 = (hir_expr_type(hobj)->name); Sema_check_scalar_method(self, _ord_t1761, method, _ord_t1762); _tr_str_release(_ord_t1761); });
#line 7058 "src/sema.tr"
            Sema_check_builtin_class_method(self, hir_expr_type(hobj)->name, method, hl);
#line 7059 "src/sema.tr"
            Sema_check_collection_arg_types(self, hir_expr_type(hobj), method, hl);
#line 7060 "src/sema.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL))))) {
#line 7060 "src/sema.tr"
                Sema_check_user_method_call(self, hir_expr_type(hobj)->name, obj, method, hl);
            }
#line 7061 "src/sema.tr"
            if ((_tr_str_eqv((hir_expr_type(hobj)->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((hir_expr_type(hobj)->name), (_tr_str_lit_len("", 0LL))))) {
#line 7062 "src/sema.tr"
                __auto_type _t1763 = (*obj);
                if (_t1763.tag == Expr_EIdent) {
                    __auto_type _sb_t = _t1763.data.EIdent.name;
#line 7064 "src/sema.tr"
                    if (((((_tr_dict_contains(self->classes, _tr_strz(_sb_t)) && ({ __auto_type _ord_t1764 = (Sema_resolve(self, _sb_t)->kind); __auto_type _wr = ((_ord_t1764.tag != SymbolKind_make_SVariable().tag)); _wr; })) && (!_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))))) && (!_tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) && (!_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL)))))) {
#line 7064 "src/sema.tr"
                        Sema_check_static_call(self, _sb_t, method, hl);
                    }
                } else if (1) {
                    __auto_type _ = _t1763;
                    /* pass */
                }
            }
#line 7066 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL)))) {
#line 7067 "src/sema.tr"
                __auto_type _t1765 = (*obj);
                if (_t1765.tag == Expr_EIdent) {
                    __auto_type _sp_t = _t1765.data.EIdent.name;
#line 7069 "src/sema.tr"
                    if (_tr_str_eqv((_sp_t), (_tr_str_lit_len("Thread", 6LL)))) {
#line 7069 "src/sema.tr"
                        Sema_check_static_call(self, _tr_str_lit_len("Thread", 6LL), _tr_str_lit_len("spawn", 5LL), hl);
                    }
                } else if (1) {
                    __auto_type _ = _t1765;
                    /* pass */
                }
            }
        }
#line 7072 "src/sema.tr"
        if (_tr_dict_contains(self->type_aliases, _tr_strz(hobj_ty->name))) {
#line 7073 "src/sema.tr"
            TrStr _alias_base = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->type_aliases, _tr_strz(hobj_ty->name))));
#line 7074 "src/sema.tr"
            AstType* _alias_ty = AstType_init(_alias_base);
#line 7075 "src/sema.tr"
            if (_tr_dict_contains(self->type_alias_elem, _tr_strz(hobj_ty->name))) {
#line 7076 "src/sema.tr"
                TrStr _elem_name = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->type_alias_elem, _tr_strz(hobj_ty->name))));
#line 7077 "src/sema.tr"
                _alias_ty = AstType_init_generic(_alias_base, box_asttype(AstType_init(_elem_name)));
            }
#line 7078 "src/sema.tr"
            hobj_ty = _alias_ty;
        }
#line 7084 "src/sema.tr"
        TrStr _recv_name = _tr_str_lit_len("", 0LL);
#line 7085 "src/sema.tr"
        bool _recv_is_shared = false;
#line 7086 "src/sema.tr"
        __auto_type _t1766 = (*obj);
        if (_t1766.tag == Expr_EIdent) {
            __auto_type _rn = _t1766.data.EIdent.name;
#line 7088 "src/sema.tr"
            TrStr _strtmp_t1767 = _tr_str_retain(_rn);
            _tr_str_release(_recv_name);
            _recv_name = _strtmp_t1767;
#line 7089 "src/sema.tr"
            Symbol* _rsym = Sema_resolve(self, _rn);
#line 7090 "src/sema.tr"
            if (((!_tr_str_eqv((_rsym->name), (_tr_str_lit_len("", 0LL)))) && _rsym->is_shared)) {
#line 7090 "src/sema.tr"
                _recv_is_shared = true;
            }
        } else if (1) {
            __auto_type _ = _t1766;
            /* pass */
        }
#line 7102 "src/sema.tr"
        if ((((!_tr_str_eqv((_recv_name), (_tr_str_lit_len("", 0LL)))) && (!Sema_is_local_var(self, _recv_name))) && (!Sema__is_known_type_name(self, _recv_name)))) {
#line 7103 "src/sema.tr"
            if (_tr_str_eqv((Sema_resolve(self, _recv_name)->name), (_tr_str_lit_len("", 0LL)))) {
#line 7104 "src/sema.tr"
                ({ TrStr _at_t1768 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] '", 7LL)), (_recv_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not a defined type or value, so '", 38LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_recv_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' cannot be resolved.\n      FIX: check the spelling, import the type, or define it — a call on an unknown name would otherwise emit invalid C.", 149LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1768); _tr_str_release(_at_t1768); });
            }
        }
#line 7107 "src/sema.tr"
        if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
#line 7108 "src/sema.tr"
            ClassDef* _cf_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
#line 7109 "src/sema.tr"
            long long _cf_i = 0LL;
#line 7110 "src/sema.tr"
            AstType* _cf_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7111 "src/sema.tr"
            bool _cf_found = false;
#line 7112 "src/sema.tr"
            while ((_cf_i < _cf_cls->fields->len)) {
#line 7113 "src/sema.tr"
                FieldDef* _cf_fld = ((FieldDef*)List_ptr_get(_cf_cls->fields, _cf_i));
#line 7114 "src/sema.tr"
                if ((_tr_str_eqv((_cf_fld->name), (method)) && (((unsigned long long)(_cf_fld->ty)) != ((unsigned long long)(0LL))))) {
#line 7115 "src/sema.tr"
                    _cf_ty = (*_cf_fld->ty);
#line 7116 "src/sema.tr"
                    _cf_found = true;
                }
#line 7117 "src/sema.tr"
                _cf_i = (_cf_i + 1LL);
            }
#line 7118 "src/sema.tr"
            if ((_cf_found && _tr_str_eqv((_cf_ty->name), (_tr_str_lit_len("def", 3LL))))) {
#line 7119 "src/sema.tr"
                AstType* _cf_ret = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7120 "src/sema.tr"
                if ((_cf_ty->args->len > 0LL)) {
#line 7120 "src/sema.tr"
                    _cf_ret = (*((AstType**)List_ptr_get(_cf_ty->args, (_cf_ty->args->len - 1LL))));
                }
#line 7131 "src/sema.tr"
                if (((_cf_cls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
#line 7132 "src/sema.tr"
                    _cf_ret = Sema__subst_ret_generics(self, _cf_ret, _cf_cls->generics, hobj_ty->args);
                }
#line 7133 "src/sema.tr"
                HirExpr* _cf_prop = box_hirexpr(HirExpr_ctor_EPropAccess(hobj, method, _cf_ty));
#line 7134 "src/sema.tr"
                HirExpr* _trret276 = box_hirexpr(HirExpr_ctor_ECall(_cf_prop, hl, _cf_ret));
                _tr_str_release(_recv_name);
                return _trret276;
            }
        }
#line 7136 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL))))) {
#line 7137 "src/sema.tr"
            TrStr pc_obj_nm = _tr_str_lit_len("", 0LL);
#line 7138 "src/sema.tr"
            __auto_type _t1769 = (*obj);
            if (_t1769.tag == Expr_EIdent) {
                __auto_type pc_src = _t1769.data.EIdent.name;
                TrStr _strtmp_t1770 = _tr_str_retain(pc_src);
                _tr_str_release(pc_obj_nm);
                pc_obj_nm = _strtmp_t1770;
            } else if (1) {
                __auto_type _ = _t1769;
                /* pass */
            }
#line 7141 "src/sema.tr"
            if (((!_tr_str_eqv((pc_obj_nm), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->container_borrows, _tr_strz(pc_obj_nm)))) {
#line 7142 "src/sema.tr"
                TrStr pc_borrow_var = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->container_borrows, _tr_strz(pc_obj_nm))));
#line 7143 "src/sema.tr"
                ({ TrStr _at_t1771 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-4] Cannot mutate '", 21LL)), (pc_obj_nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while '", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pc_borrow_var)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' holds a reference into it.\n      FIX: Finish using '", 54LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pc_borrow_var)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' before modifying '", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pc_obj_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or copy it first: 'mut copy = ", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pc_borrow_var)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1771); _tr_str_release(_at_t1771); });
            }
        }
#line 7145 "src/sema.tr"
        TrStr _bm_obj_nm = _tr_str_lit_len("", 0LL);
#line 7146 "src/sema.tr"
        __auto_type _t1772 = (*obj);
        if (_t1772.tag == Expr_EIdent) {
            __auto_type _bm_n = _t1772.data.EIdent.name;
            TrStr _strtmp_t1773 = _tr_str_retain(_bm_n);
            _tr_str_release(_bm_obj_nm);
            _bm_obj_nm = _strtmp_t1773;
        } else if (1) {
            __auto_type _ = _t1772;
            /* pass */
        }
#line 7149 "src/sema.tr"
        AstType* ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7153 "src/sema.tr"
        AstType** _gpb_ret = ((AstType**)(0LL));
#line 7154 "src/sema.tr"
        if (((self->current_func_constraints->len > 0LL) && Sema__is_type_param_in_scope(self, hobj_ty->name))) {
#line 7155 "src/sema.tr"
            _gpb_ret = Sema__resolve_generic_bound_method_ret(self, hobj_ty->name, method);
        }
#line 7157 "src/sema.tr"
        if ((((unsigned long long)(_gpb_ret)) != ((unsigned long long)(0LL)))) {
#line 7158 "src/sema.tr"
            ret_ty = (*_gpb_ret);
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("OS", 2LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("OS", 2LL))))) {
#line 7160 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("cwd", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("platform", 8LL))))) {
#line 7160 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            } else if ((((_tr_str_eqv((method), (_tr_str_lit_len("is_windows", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_linux", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_darwin", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_macos", 8LL))))) {
#line 7161 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            }
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("Process", 7LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Process", 7LL))))) {
#line 7163 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("system", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("exit", 4LL))))) {
#line 7163 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("shell_output", 12LL)))) {
#line 7164 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("Env", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Env", 3LL))))) {
#line 7166 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("get_var", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("cwd", 3LL))))) {
#line 7166 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("Hash", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Hash", 4LL))))) {
#line 7168 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("sha256", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("md5", 3LL))))) {
#line 7168 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        }
#line 7174 "src/sema.tr"
        if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && _tr_str_eqv((method), (_tr_str_lit_len("write", 5LL)))) && (!self->in_unsafe))) {
#line 7175 "src/sema.tr"
            Sema_error(self, _tr_str_lit_len("[P-1] '.write()' on a Pointer mutates raw memory and must be inside an 'unsafe:' block.\n      FIX: Wrap this call in 'unsafe:', e.g.\n          unsafe:\n              ... .write(...) ...", 184LL));
        }
#line 7188 "src/sema.tr"
        bool _auto_is_self_factory = true;
#line 7189 "src/sema.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("auto", 4LL))) && _tr_dict_contains(self->classes, _tr_strz(hobj_ty->name)))) {
#line 7190 "src/sema.tr"
            ClassDef* _af_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
#line 7191 "src/sema.tr"
            long long _af_i = 0LL;
#line 7192 "src/sema.tr"
            while ((_af_i < _af_cls->methods->len)) {
#line 7193 "src/sema.tr"
                FunctionDef* _af_m = ((FunctionDef*)List_ptr_get(_af_cls->methods, _af_i));
#line 7194 "src/sema.tr"
                if ((_tr_str_eqv((_af_m->name), (_tr_str_lit_len("auto", 4LL))) && (((unsigned long long)(_af_m->ret_ty)) != ((unsigned long long)(0LL))))) {
#line 7195 "src/sema.tr"
                    if (({ __auto_type _ord_t1774 = ((*_af_m->ret_ty)->name); __auto_type _wr = ((!_tr_str_eqv((_ord_t1774), (hobj_ty->name)))); _wr; })) {
#line 7195 "src/sema.tr"
                        _auto_is_self_factory = false;
                    }
                }
#line 7196 "src/sema.tr"
                _af_i = (_af_i + 1LL);
            }
        }
#line 7197 "src/sema.tr"
        if (((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))) || (_tr_str_eqv((method), (_tr_str_lit_len("auto", 4LL))) && _auto_is_self_factory))) {
#line 7198 "src/sema.tr"
            __auto_type _t1775 = (*obj);
            if (_t1775.tag == Expr_EIdent) {
                __auto_type _ic_n = _t1775.data.EIdent.name;
#line 7200 "src/sema.tr"
                if ((_tr_dict_contains(self->classes, _tr_strz(_ic_n)) && ({ __auto_type _ord_t1776 = (Sema_resolve(self, _ic_n)->kind); __auto_type _wr = ((_ord_t1776.tag != SymbolKind_make_SVariable().tag)); _wr; }))) {
#line 7200 "src/sema.tr"
                    Sema_check_static_call(self, _ic_n, method, hl);
                }
            } else if (1) {
                __auto_type _ = _t1775;
                /* pass */
            }
#line 7203 "src/sema.tr"
            if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("", 0LL))))) {
#line 7206 "src/sema.tr"
                __auto_type _t1777 = (*hobj);
                if (_t1777.tag == HirExpr_EIdent) {
                    __auto_type recv_nm = _t1777.data.EIdent.name;
                    ret_ty = AstType_init(recv_nm);
                } else if (_t1777.tag == HirExpr_EIndex) {
                    __auto_type idx_base = _t1777.data.EIndex.obj;
__auto_type idx_arg = _t1777.data.EIndex._tr_v_index;
#line 7210 "src/sema.tr"
                    __auto_type _t1778 = (*idx_base);
                    if (_t1778.tag == HirExpr_EIdent) {
                        __auto_type gn = _t1778.data.EIdent.name;
#line 7212 "src/sema.tr"
                        ret_ty = AstType_init(gn);
#line 7213 "src/sema.tr"
                        __auto_type _t1779 = (*idx_arg);
                        if (_t1779.tag == HirExpr_EIdent) {
                            __auto_type tn = _t1779.data.EIdent.name;
                            ({ __auto_type _ord_t1780 = (box_asttype(AstType_init(tn))); List_ptr_append(ret_ty->args, _ord_t1780); });
                        } else if (1) {
                            __auto_type _ = _t1779;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t1778;
                        ret_ty = hobj_ty;
                    }
                } else if (1) {
                    __auto_type _ = _t1777;
                    ret_ty = hobj_ty;
                }
            } else {
#line 7219 "src/sema.tr"
                ret_ty = hobj_ty;
            }
        } else if (_tr_str_eqv((method), (_tr_str_lit_len("offset", 6LL)))) {
#line 7224 "src/sema.tr"
            if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (!self->cur_fn_is_lib)) && (!self->in_unsafe))) {
#line 7225 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[P-2] '.offset()' does raw pointer arithmetic (can create an out-of-bounds pointer) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or use a safe slice / Vec[T] instead of pointer math.", 209LL));
            }
#line 7226 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((((_tr_str_eqv((method), (_tr_str_lit_len("sum", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("min_val", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max_val", 7LL)))))) {
#line 7229 "src/sema.tr"
            if ((hobj_ty->args->len > 0LL)) {
#line 7229 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
#line 7230 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((_tr_str_eqv((method), (_tr_str_lit_len("any", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("all", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))))) {
#line 7231 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL)))))) {
#line 7232 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("first", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("last", 4LL)))))) {
#line 7234 "src/sema.tr"
            if ((hobj_ty->args->len > 0LL)) {
#line 7234 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
#line 7235 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("reversed", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("reversed_copy", 13LL)))))) {
#line 7236 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL)))))) {
#line 7237 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && _tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL))))) {
#line 7238 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("last_index_of", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("count", 5LL)))))) {
#line 7239 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && _tr_str_eqv((method), (_tr_str_lit_len("join", 4LL))))) {
#line 7240 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) && _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 7242 "src/sema.tr"
            if ((hobj_ty->args->len > 0LL)) {
#line 7246 "src/sema.tr"
                if (((!self->cur_fn_is_lib) && (!self->in_unsafe))) {
#line 7247 "src/sema.tr"
                    Sema_error(self, _tr_str_lit_len("[P-2] '.read()' dereferences a raw pointer (may read freed or invalid memory) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or hold the value in a safe Box[T] / Vec[T] / class reference instead of a raw pointer.", 237LL));
                }
#line 7248 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
#line 7249 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("as_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))))) {
#line 7262 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__len__", 7LL))))) {
#line 7263 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((method), (_tr_str_lit_len("checked_add", 11LL))) || _tr_str_eqv((method), (_tr_str_lit_len("checked_sub", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("checked_mul", 11LL))))) {
#line 7266 "src/sema.tr"
            ret_ty = AstType_init_generic(_tr_str_lit_len("Option", 6LL), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL))));
        } else if (((((((((_tr_str_eqv((method), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sign", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clamp", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("gcd", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("lcm", 3LL)))) && ((((((((((((((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("isize", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("char", 4LL)))))) {
#line 7271 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f32", 3LL))))) {
#line 7274 "src/sema.tr"
            if (((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("floor", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("ceil", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("round", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sqrt", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("fabs", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("log", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("log2", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("log10", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("exp", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sin", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cos", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("tan", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("asin", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("acos", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("atan", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("atan2", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL))))) {
#line 7275 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("float", 5LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_nan", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_inf", 6LL))))) {
#line 7276 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("str", 3LL)))) {
#line 7279 "src/sema.tr"
            ret_ty = Sema_str_method_ret_ty(self, method);
#line 7280 "src/sema.tr"
            if ((((!_str_method_known(method)) && (!Sema_class_method_exists(self, _tr_str_lit_len("str", 3LL), method))) && (!Sema_is_universal_method(self, method)))) {
#line 7283 "src/sema.tr"
                ({ TrStr _at_t1781 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] No method '", 17LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' found on type 'str'.\n      FIX: see docs/std/string.md for the str methods, or add one with 'extend str:'.", 108LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1781); _tr_str_release(_at_t1781); });
            }
        } else if (((((((((((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("char", 4LL))))) {
#line 7286 "src/sema.tr"
            if (((((((_tr_str_eqv((method), (_tr_str_lit_len("to_hex", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_HEX", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_hex_upper", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_octal", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_oct", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_binary", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_bin", 6LL))))) {
#line 7287 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_f64", 6LL))))) {
#line 7288 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("float", 5LL));
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("to_int", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_i64", 6LL))))) {
#line 7289 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL)))) && (!_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("StringBuilder", 13LL)))))) {
#line 7293 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if (((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL))))) {
#line 7295 "src/sema.tr"
            if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (hobj_ty->args->len > 0LL))) {
#line 7296 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (hobj_ty->args->len > 1LL))) {
#line 7298 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (hobj_ty->args->len > 0LL))) {
#line 7314 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
#line 7326 "src/sema.tr"
                ClassDef* _gcls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
#line 7327 "src/sema.tr"
                long long _gmi = 0LL;
#line 7328 "src/sema.tr"
                bool _gfound = false;
#line 7329 "src/sema.tr"
                while ((_gmi < _gcls->methods->len)) {
#line 7330 "src/sema.tr"
                    FunctionDef* _gmdef = ((FunctionDef*)List_ptr_get(_gcls->methods, _gmi));
#line 7331 "src/sema.tr"
                    if (_tr_str_eqv((_gmdef->name), (method))) {
#line 7332 "src/sema.tr"
                        if ((((unsigned long long)(_gmdef->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 7333 "src/sema.tr"
                            ret_ty = (*_gmdef->ret_ty);
#line 7334 "src/sema.tr"
                            if (((_gcls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
#line 7335 "src/sema.tr"
                                ret_ty = Sema__subst_ret_generics(self, ret_ty, _gcls->generics, hobj_ty->args);
                            }
#line 7336 "src/sema.tr"
                            _gfound = true;
                        }
                    }
#line 7337 "src/sema.tr"
                    _gmi = (_gmi + 1LL);
                }
#line 7338 "src/sema.tr"
                if (((!_gfound) && (hobj_ty->args->len > 0LL))) {
#line 7339 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
            } else if ((hobj_ty->args->len > 0LL)) {
#line 7341 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            }
        } else if (_tr_str_eqv((method), (_tr_str_lit_len("alloc", 5LL)))) {
#line 7342 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) {
#line 7343 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Array", 5LL)))) {
#line 7346 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("as_ptr", 6LL)))) {
#line 7348 "src/sema.tr"
                AstType* _arr_elem = AstType_init(_tr_str_lit_len("u8", 2LL));
#line 7349 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7349 "src/sema.tr"
                    _arr_elem = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
#line 7350 "src/sema.tr"
                ret_ty = AstType_init_generic(_tr_str_lit_len("Pointer", 7LL), box_asttype(_arr_elem));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
#line 7351 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Set", 3LL)))) {
#line 7354 "src/sema.tr"
            if ((((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_subset", 9LL))))) {
#line 7354 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
#line 7355 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if (((_tr_str_eqv((method), (_tr_str_lit_len("add", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL))))) {
#line 7356 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("to_list", 7LL)))) {
#line 7359 "src/sema.tr"
                AstType* _set_elem_ty = AstType_init(_tr_str_lit_len("str", 3LL));
#line 7360 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7361 "src/sema.tr"
                    _set_elem_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
#line 7362 "src/sema.tr"
                ret_ty = AstType_init_generic(_tr_str_lit_len("List", 4LL), box_asttype(_set_elem_ty));
            } else if (((_tr_str_eqv((method), (_tr_str_lit_len("union", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("intersection", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("difference", 10LL))))) {
#line 7363 "src/sema.tr"
                ret_ty = hobj_ty;
            }
        } else if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Dict", 4LL))))) {
#line 7366 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("keys", 4LL)))) {
#line 7368 "src/sema.tr"
                AstType* _dict_key_ty = AstType_init(_tr_str_lit_len("str", 3LL));
#line 7369 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7370 "src/sema.tr"
                    _dict_key_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
#line 7371 "src/sema.tr"
                ret_ty = AstType_init_generic(_tr_str_lit_len("List", 4LL), box_asttype(_dict_key_ty));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("values", 6LL)))) {
#line 7374 "src/sema.tr"
                AstType* _dict_val_ty = AstType_init(_tr_str_lit_len("ptr", 3LL));
#line 7375 "src/sema.tr"
                if ((hobj_ty->args->len > 1LL)) {
#line 7376 "src/sema.tr"
                    _dict_val_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
                }
#line 7377 "src/sema.tr"
                ret_ty = AstType_init_generic(_tr_str_lit_len("List", 4LL), box_asttype(_dict_val_ty));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Option", 6LL)))) {
#line 7385 "src/sema.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("Some", 4LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("some", 4LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Option", 6LL))))) && (hl->len == 1LL))) {
#line 7386 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Option", 6LL));
#line 7387 "src/sema.tr"
                ({ __auto_type _ord_t1782 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1782); });
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("None", 4LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("none", 4LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Option", 6LL)))))) {
#line 7388 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Option", 6LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_some", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_none", 7LL))))) {
#line 7389 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("unwrap", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("expect", 6LL))))) {
#line 7391 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7391 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7392 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("unwrap_or", 9LL)))) {
#line 7394 "src/sema.tr"
                if ((hl->len > 0LL)) {
#line 7394 "src/sema.tr"
                    ret_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)));
                } else if ((hobj_ty->args->len > 0LL)) {
#line 7395 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7396 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Result", 6LL)))) {
#line 7404 "src/sema.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("Ok", 2LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("ok", 2LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Result", 6LL))))) && (hl->len == 1LL))) {
#line 7405 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 7406 "src/sema.tr"
                ({ __auto_type _ord_t1783 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1783); });
#line 7407 "src/sema.tr"
                ({ __auto_type _ord_t1784 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1784); });
            } else if (((_tr_str_eqv((method), (_tr_str_lit_len("Err", 3LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("err", 3LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Result", 6LL))))) && (hl->len == 1LL))) {
#line 7409 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 7410 "src/sema.tr"
                ({ __auto_type _ord_t1785 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1785); });
#line 7411 "src/sema.tr"
                ({ __auto_type _ord_t1786 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1786); });
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_ok", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_err", 6LL))))) {
#line 7412 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("unwrap", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("ok", 2LL))))) {
#line 7414 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7414 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7415 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("unwrap_err", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("err", 3LL))))) {
#line 7417 "src/sema.tr"
                if ((hobj_ty->args->len > 1LL)) {
#line 7417 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
                } else {
#line 7418 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
                }
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && (_tr_str_eqv((_recv_name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Thread", 6LL)))))) {
#line 7424 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("Thread", 6LL));
#line 7426 "src/sema.tr"
            long long _tsi = 1LL;
#line 7427 "src/sema.tr"
            while ((_tsi < hl->len)) {
#line 7428 "src/sema.tr"
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _tsi)))) {
#line 7429 "src/sema.tr"
                    Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to Thread.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 254LL));
                }
#line 7430 "src/sema.tr"
                AstType* _tsa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _tsi)));
#line 7431 "src/sema.tr"
                if ((!Sema_is_sendable_ty(self, _tsa_ty))) {
#line 7432 "src/sema.tr"
                    ({ TrStr _at_t1787 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (_tsa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be passed to Thread.spawn.\n      FIX: Wrap in Mutex[", 81LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tsa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] for exclusive access, or add 'implements Sendable' to '", 57LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tsa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1787); _tr_str_release(_at_t1787); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _tsa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _tsi))))) && (!Sema__is_unsafe_sendable(self, _tsa_ty->name)))) {
#line 7434 "src/sema.tr"
                    ({ TrStr _at_t1788 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (_tsa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot be passed to Thread.spawn (exactly why Rust's 'Rc' is '!Send'): retaining/releasing it from another thread races the count.\n      FIX: share it as 'Shared[", 270LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tsa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc'). A 'Sendable' class must itself hold only Sendable + atomically-shared state.", 117LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1788); _tr_str_release(_at_t1788); });
                }
#line 7435 "src/sema.tr"
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _tsi)));
#line 7436 "src/sema.tr"
                _tsi = (_tsi + 1LL);
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("ThreadPool", 10LL))))) {
#line 7439 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7441 "src/sema.tr"
            long long _psi = 1LL;
#line 7442 "src/sema.tr"
            while ((_psi < hl->len)) {
#line 7443 "src/sema.tr"
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _psi)))) {
#line 7444 "src/sema.tr"
                    Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to ThreadPool.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 258LL));
                }
#line 7445 "src/sema.tr"
                AstType* _psa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _psi)));
#line 7446 "src/sema.tr"
                if ((!Sema_is_sendable_ty(self, _psa_ty))) {
#line 7447 "src/sema.tr"
                    ({ TrStr _at_t1789 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (_psa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be passed to ThreadPool.spawn.\n      FIX: Wrap in Mutex[", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_psa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] for exclusive access, or add 'implements Sendable' to '", 57LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_psa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1789); _tr_str_release(_at_t1789); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _psa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _psi))))) && (!Sema__is_unsafe_sendable(self, _psa_ty->name)))) {
#line 7449 "src/sema.tr"
                    ({ TrStr _at_t1790 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (_psa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it cannot be passed to ThreadPool.spawn (like Rust's 'Rc' being '!Send').\n      FIX: share it as 'Shared[", 192LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_psa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc').", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1790); _tr_str_release(_at_t1790); });
                }
#line 7450 "src/sema.tr"
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _psi)));
#line 7451 "src/sema.tr"
                _psi = (_psi + 1LL);
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("AsyncPool", 9LL))))) {
#line 7461 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("AsyncTask", 9LL));
#line 7462 "src/sema.tr"
            long long _asi = 1LL;
#line 7463 "src/sema.tr"
            while ((_asi < hl->len)) {
#line 7464 "src/sema.tr"
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _asi)))) {
#line 7465 "src/sema.tr"
                    Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to AsyncPool.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 257LL));
                }
#line 7466 "src/sema.tr"
                AstType* _asa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _asi)));
#line 7467 "src/sema.tr"
                if ((!Sema_is_sendable_ty(self, _asa_ty))) {
#line 7468 "src/sema.tr"
                    ({ TrStr _at_t1791 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (_asa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be passed to AsyncPool.spawn.\n      FIX: Wrap in Mutex[", 84LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_asa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] for exclusive access, or add 'implements Sendable' to '", 57LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_asa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1791); _tr_str_release(_at_t1791); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _asa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _asi))))) && (!Sema__is_unsafe_sendable(self, _asa_ty->name)))) {
#line 7470 "src/sema.tr"
                    ({ TrStr _at_t1792 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (_asa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it cannot be passed to AsyncPool.spawn (like Rust's 'Rc' being '!Send'): the task may run on a different worker than the one that submitted it.\n      FIX: share it as 'Shared[", 262LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_asa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc').", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1792); _tr_str_release(_at_t1792); });
                }
#line 7471 "src/sema.tr"
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _asi)));
#line 7472 "src/sema.tr"
                _asi = (_asi + 1LL);
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("AsyncTask", 9LL)))) {
#line 7475 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) {
#line 7475 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("done", 4LL)))) {
#line 7476 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
#line 7477 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Atomic", 6LL)))) {
#line 7480 "src/sema.tr"
            if ((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("load", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("swap", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("exchange", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_add", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_sub", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("load_relaxed", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("load_acquire", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("load_seqcst", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add_relaxed", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add_release", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add_acqrel", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub_relaxed", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub_release", 11LL))))) {
#line 7481 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if ((((_tr_str_eqv((method), (_tr_str_lit_len("cas", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("compare_exchange", 16LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cas_weak", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cas_acqrel", 10LL))))) {
#line 7483 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (((((_tr_str_eqv((method), (_tr_str_lit_len("store", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("store_relaxed", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("store_release", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("store_seqcst", 12LL))))) {
#line 7485 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Chan", 4LL)))) {
#line 7488 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("recv", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("try_recv", 8LL))))) {
#line 7489 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7489 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7490 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("cap", 3LL))))) {
#line 7491 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_closed", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("try_send", 8LL))))) {
#line 7492 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("send_timeout", 12LL)))) {
#line 7493 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("recv_timeout", 12LL)))) {
#line 7495 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7495 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7496 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 7499 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("lock", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))))) {
#line 7500 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7500 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7501 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 7504 "src/sema.tr"
            if ((((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("read_lock", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("write_lock", 10LL))))) {
#line 7505 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7505 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7506 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("StringBuilder", 13LL))) && _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))))) {
#line 7507 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("StringObj", 9LL));
        } else if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("StringBuilder", 13LL))) && _tr_str_eqv((method), (_tr_str_lit_len("to_owned", 8LL))))) {
#line 7508 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Shared", 6LL)))) {
#line 7513 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL)))) {
#line 7513 "src/sema.tr"
                ret_ty = hobj_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))) {
#line 7514 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("downgrade", 9LL)))) {
#line 7516 "src/sema.tr"
                AstType* weak_ty = AstType_init(_tr_str_lit_len("Weak", 4LL));
#line 7517 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7518 "src/sema.tr"
                    weak_ty->args = (void*)List_ptr_new();
#line 7519 "src/sema.tr"
                    ({ __auto_type _ord_t1793 = (box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL))))); List_ptr_append(weak_ty->args, _ord_t1793); });
                }
#line 7520 "src/sema.tr"
                ret_ty = weak_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("drop", 4LL)))) {
#line 7521 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Weak", 4LL)))) {
#line 7524 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("upgrade", 7LL)))) {
#line 7525 "src/sema.tr"
                AstType* opt_ty = AstType_init(_tr_str_lit_len("Option", 6LL));
#line 7526 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7527 "src/sema.tr"
                    opt_ty->args = (void*)List_ptr_new();
#line 7528 "src/sema.tr"
                    ({ __auto_type _ord_t1794 = (box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL))))); List_ptr_append(opt_ty->args, _ord_t1794); });
                }
#line 7529 "src/sema.tr"
                ret_ty = opt_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("is_alive", 8LL)))) {
#line 7530 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            }
        } else if ((_recv_is_shared && (((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("downgrade", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("drop", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))))) {
#line 7535 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL)))) {
#line 7535 "src/sema.tr"
                ret_ty = hobj_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))) {
#line 7536 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("drop", 4LL)))) {
#line 7537 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("downgrade", 9LL)))) {
#line 7539 "src/sema.tr"
                AstType* _wk_ty = AstType_init(_tr_str_lit_len("Weak", 4LL));
#line 7540 "src/sema.tr"
                _wk_ty->args = (void*)List_ptr_new();
#line 7541 "src/sema.tr"
                ({ __auto_type _ord_t1795 = (box_asttype(hobj_ty)); List_ptr_append(_wk_ty->args, _ord_t1795); });
#line 7542 "src/sema.tr"
                ret_ty = _wk_ty;
            }
        } else if (((((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))) && _tr_dict_contains(self->interfaces, _tr_strz(hobj_ty->name)))) {
#line 7544 "src/sema.tr"
            InterfaceDef* _iface_def = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(hobj_ty->name)));
#line 7545 "src/sema.tr"
            long long _imi = 0LL;
#line 7546 "src/sema.tr"
            while ((_imi < _iface_def->methods->len)) {
#line 7547 "src/sema.tr"
                FunctionDef* _imdef = ((FunctionDef*)List_ptr_get(_iface_def->methods, _imi));
#line 7548 "src/sema.tr"
                if (_tr_str_eqv((_imdef->name), (method))) {
#line 7549 "src/sema.tr"
                    if ((((unsigned long long)(_imdef->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 7550 "src/sema.tr"
                        ret_ty = (*_imdef->ret_ty);
                    }
                }
#line 7551 "src/sema.tr"
                _imi = (_imi + 1LL);
            }
#line 7553 "src/sema.tr"
            if (((_iface_def->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
#line 7554 "src/sema.tr"
                long long _gi5 = 0LL;
#line 7555 "src/sema.tr"
                while ((_gi5 < _iface_def->generics->len)) {
#line 7556 "src/sema.tr"
                    if (({ __auto_type _ord_t1796 = (List_TrStr_get(_iface_def->generics, _gi5)); __auto_type _wr = (({ TrStr _cmpa_t1797 = (_ord_t1796); TrStr _cmpb_t1798 = (ret_ty->name); _Bool _cmpr_t1799 = _tr_str_eqv(_cmpa_t1797, _cmpb_t1798); _tr_str_release(_cmpa_t1797); _cmpr_t1799; })); _wr; })) {
#line 7557 "src/sema.tr"
                        if ((_gi5 < hobj_ty->args->len)) {
#line 7558 "src/sema.tr"
                            ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, _gi5)));
                        }
                    }
#line 7559 "src/sema.tr"
                    _gi5 = (_gi5 + 1LL);
                }
            }
        } else if (_tr_dict_contains(self->enums, _tr_strz(hobj_ty->name))) {
#line 7561 "src/sema.tr"
            ret_ty = AstType_init(hobj_ty->name);
#line 7565 "src/sema.tr"
            if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Option", 6LL))) && _tr_str_eqv((method), (_tr_str_lit_len("Some", 4LL)))) && (hl->len == 1LL))) {
#line 7566 "src/sema.tr"
                ({ __auto_type _ord_t1800 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1800); });
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((method), (_tr_str_lit_len("Ok", 2LL)))) && (hl->len == 1LL))) {
#line 7568 "src/sema.tr"
                ({ __auto_type _ord_t1801 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1801); });
#line 7569 "src/sema.tr"
                ({ __auto_type _ord_t1802 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1802); });
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((method), (_tr_str_lit_len("Err", 3LL)))) && (hl->len == 1LL))) {
#line 7571 "src/sema.tr"
                ({ __auto_type _ord_t1803 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1803); });
#line 7572 "src/sema.tr"
                ({ __auto_type _ord_t1804 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1804); });
            }
        } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
#line 7574 "src/sema.tr"
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
#line 7575 "src/sema.tr"
            TrStr _hty_n = _tr_str_retain(hobj_ty->name);
#line 7576 "src/sema.tr"
            bool _is_builtin_dispatch = (((((((((((((((((_tr_str_eqv((_hty_n), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("OS", 2LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Process", 7LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Env", 3LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Hash", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("File", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("AsyncTask", 9LL))));
#line 7577 "src/sema.tr"
            if ((((!_is_builtin_dispatch) && (!Sema_class_method_exists(self, hobj_ty->name, method))) && (!Sema_is_universal_method(self, method)))) {
#line 7578 "src/sema.tr"
                ({ TrStr _at_t1805 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] No method '", 17LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' found on type '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: Define 'pub def ", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(self, ...)' in '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' or its base class via 'extend ", 32LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":'.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1805); _tr_str_release(_at_t1805); });
            }
#line 7579 "src/sema.tr"
            long long _mi = 0LL;
#line 7580 "src/sema.tr"
            AstType** _mdef_throws_ty = ((AstType**)(0LL));
#line 7581 "src/sema.tr"
            long long _m_named = 0LL;
#line 7582 "src/sema.tr"
            bool _m_recv_is_type = false;
#line 7583 "src/sema.tr"
            __auto_type _t1806 = (*obj);
            if (_t1806.tag == Expr_EIdent) {
                __auto_type _m_rn = _t1806.data.EIdent.name;
#line 7585 "src/sema.tr"
                _m_recv_is_type = (({ __auto_type _ord_t1807 = (Sema_resolve(self, _m_rn)->kind); __auto_type _wr = ((_ord_t1807.tag == SymbolKind_make_SClass().tag)); _wr; }) || _tr_dict_contains(self->classes, _tr_strz(_m_rn)));
            } else if (1) {
                __auto_type _ = _t1806;
                /* pass */
            }
#line 7587 "src/sema.tr"
            bool _m_arity_ok = false;
#line 7588 "src/sema.tr"
            FunctionDef* _m_one = FunctionDef_init(_tr_str_lit_len("", 0LL));
#line 7589 "src/sema.tr"
            long long _m_first_want = 0LL;
#line 7590 "src/sema.tr"
            bool _m_variadic = false;
#line 7591 "src/sema.tr"
            while ((_mi < _cls->methods->len)) {
#line 7592 "src/sema.tr"
                FunctionDef* _mdef = ((FunctionDef*)List_ptr_get(_cls->methods, _mi));
#line 7593 "src/sema.tr"
                if (_tr_str_eqv((_mdef->name), (method))) {
#line 7594 "src/sema.tr"
                    long long _m_cnt = 0LL;
#line 7595 "src/sema.tr"
                    long long _m_ci = 0LL;
#line 7596 "src/sema.tr"
                    while ((_m_ci < _mdef->params->len)) {
#line 7597 "src/sema.tr"
                        if ((!_tr_str_eqv((((Param*)List_ptr_get(_mdef->params, _m_ci))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7597 "src/sema.tr"
                            _m_cnt = (_m_cnt + 1LL);
                        }
#line 7598 "src/sema.tr"
                        if (((Param*)List_ptr_get(_mdef->params, _m_ci))->is_variadic) {
#line 7598 "src/sema.tr"
                            _m_variadic = true;
                        }
#line 7599 "src/sema.tr"
                        _m_ci = (_m_ci + 1LL);
                    }
#line 7600 "src/sema.tr"
                    if ((_m_named == 0LL)) {
#line 7600 "src/sema.tr"
                        _m_first_want = _m_cnt;
                    }
#line 7601 "src/sema.tr"
                    _m_named = (_m_named + 1LL);
#line 7602 "src/sema.tr"
                    if ((_m_cnt == hl->len)) {
#line 7602 "src/sema.tr"
                        _m_arity_ok = true;
                    }
#line 7603 "src/sema.tr"
                    FunctionDef* _cltmp_t1808 = _tr_obj_retain(_mdef);
                    _tr_obj_release(_m_one, _trdrop_FunctionDef);
                    _m_one = _cltmp_t1808;
#line 7604 "src/sema.tr"
                    if ((((((_m_cnt + 1LL) == hl->len) && (_mdef->params->len > 0LL)) && _tr_str_eqv((((Param*)List_ptr_get(_mdef->params, 0LL))->name), (_tr_str_lit_len("self", 4LL)))) && _m_recv_is_type)) {
#line 7604 "src/sema.tr"
                        _m_arity_ok = true;
                    }
#line 7605 "src/sema.tr"
                    if ((((unsigned long long)(_mdef->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 7608 "src/sema.tr"
                        long long _mpc = 0LL;
#line 7609 "src/sema.tr"
                        long long _mpi = 0LL;
#line 7610 "src/sema.tr"
                        while ((_mpi < _mdef->params->len)) {
#line 7611 "src/sema.tr"
                            if ((!_tr_str_eqv((((Param*)List_ptr_get(_mdef->params, _mpi))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7611 "src/sema.tr"
                                _mpc = (_mpc + 1LL);
                            }
#line 7612 "src/sema.tr"
                            _mpi = (_mpi + 1LL);
                        }
#line 7613 "src/sema.tr"
                        if ((_mpc == hl->len)) {
#line 7614 "src/sema.tr"
                            ret_ty = (*_mdef->ret_ty);
#line 7615 "src/sema.tr"
                            _mdef_throws_ty = _mdef->throws_ty;
#line 7616 "src/sema.tr"
                            if (((_cls->generics->len == 0LL) && (_mdef->generics->len == 0LL))) {
#line 7617 "src/sema.tr"
                                long long _fa_sk = 0LL;
#line 7618 "src/sema.tr"
                                if (((_mdef->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_mdef->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7618 "src/sema.tr"
                                    _fa_sk = 1LL;
                                }
#line 7619 "src/sema.tr"
                                long long _fa_i = _fa_sk;
#line 7620 "src/sema.tr"
                                while ((_fa_i < _mdef->params->len)) {
#line 7621 "src/sema.tr"
                                    if (((_fa_i - _fa_sk) < hl->len)) {
#line 7622 "src/sema.tr"
                                        ({ __auto_type _ord_t1809 = (((Param*)List_ptr_get(_mdef->params, _fa_i))->ty); __auto_type _ord_t1810 = (hir_expr_type(((HirExpr*)List_ptr_get(hl, (_fa_i - _fa_sk))))); __auto_type _ord_t1811 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(((_fa_i - _fa_sk) + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1812 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1811); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_fn_value_throws(self, _ord_t1809, _ord_t1810, _ord_t1812); _tr_str_release(_ord_t1812); });
                                    }
#line 7623 "src/sema.tr"
                                    _fa_i = (_fa_i + 1LL);
                                }
                            }
                        } else if (_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 7625 "src/sema.tr"
                            ret_ty = (*_mdef->ret_ty);
#line 7626 "src/sema.tr"
                            _mdef_throws_ty = _mdef->throws_ty;
                        }
                    }
                }
#line 7627 "src/sema.tr"
                _mi = (_mi + 1LL);
            }
#line 7628 "src/sema.tr"
            if (((((((_m_named == 1LL) && _m_arity_ok) && (!_m_variadic)) && (!Sema_is_rt_builtin_class(self, hobj_ty->name))) && (_cls->generics->len == 0LL)) && (_m_one->generics->len == 0LL))) {
#line 7629 "src/sema.tr"
                long long _m1_sk = 0LL;
#line 7630 "src/sema.tr"
                if (((_m_one->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_m_one->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7630 "src/sema.tr"
                    _m1_sk = 1LL;
                }
#line 7631 "src/sema.tr"
                if (((_m_one->params->len - _m1_sk) == hl->len)) {
#line 7632 "src/sema.tr"
                    long long _m1_i = 0LL;
#line 7633 "src/sema.tr"
                    while ((_m1_i < hl->len)) {
#line 7634 "src/sema.tr"
                        AstType** _m1_pt = ((Param*)List_ptr_get(_m_one->params, (_m1_sk + _m1_i)))->ty;
#line 7635 "src/sema.tr"
                        if ((((unsigned long long)(_m1_pt)) != ((unsigned long long)(0LL)))) {
#line 7636 "src/sema.tr"
                            ({ __auto_type _ord_t1813 = ((*_m1_pt)); __auto_type _ord_t1814 = (((HirExpr*)List_ptr_get(hl, _m1_i))); __auto_type _ord_t1815 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((_m1_i + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1816 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1815); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1813, _ord_t1814, _ord_t1816); _tr_str_release(_ord_t1816); });
                        }
#line 7637 "src/sema.tr"
                        _m1_i = (_m1_i + 1LL);
                    }
                }
            }
#line 7638 "src/sema.tr"
            if (((((_m_named > 0LL) && (!_m_arity_ok)) && (!_m_variadic)) && (!Sema_is_rt_builtin_class(self, hobj_ty->name)))) {
#line 7639 "src/sema.tr"
                TrStr _m_pl = _tr_str_lit_len("s", 1LL);
#line 7640 "src/sema.tr"
                if ((_m_first_want == 1LL)) {
#line 7640 "src/sema.tr"
                    TrStr _strtmp_t1817 = _tr_str_lit_len("", 0LL);
                    _tr_str_release(_m_pl);
                    _m_pl = _strtmp_t1817;
                }
#line 7641 "src/sema.tr"
                TrStr _m_vb = _tr_str_lit_len("were", 4LL);
#line 7642 "src/sema.tr"
                if ((hl->len == 1LL)) {
#line 7642 "src/sema.tr"
                    TrStr _strtmp_t1818 = _tr_str_lit_len("was", 3LL);
                    _tr_str_release(_m_vb);
                    _m_vb = _strtmp_t1818;
                }
#line 7643 "src/sema.tr"
                ({ __auto_type _ord_t1819 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (hobj_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1820 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1819); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_m_first_want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_m_pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1821 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1820); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_m_vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1822 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1821); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_m_first_want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_m_pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (see its definition).", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1822); _tr_str_release(_at_t1822); });
            }
#line 7655 "src/sema.tr"
            if ((((unsigned long long)(_mdef_throws_ty)) != ((unsigned long long)(0LL)))) {
#line 7656 "src/sema.tr"
                if ((!_tr_str_eqv(((*_mdef_throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 7657 "src/sema.tr"
                    AstType* _mres_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 7658 "src/sema.tr"
                    ({ __auto_type _ord_t1823 = (box_asttype(ret_ty)); List_ptr_append(_mres_ty->args, _ord_t1823); });
#line 7659 "src/sema.tr"
                    List_ptr_append(_mres_ty->args, _mdef_throws_ty);
#line 7660 "src/sema.tr"
                    ret_ty = _mres_ty;
                }
            }
#line 7663 "src/sema.tr"
            if (((_cls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
#line 7664 "src/sema.tr"
                ret_ty = Sema__subst_ret_generics(self, ret_ty, _cls->generics, hobj_ty->args);
            }
            _tr_obj_release(_m_one, _trdrop_FunctionDef);
        }
#line 7665 "src/sema.tr"
        HirExpr* _trret277 = box_hirexpr(HirExpr_ctor_EMethodCall(hobj, method, hl, ret_ty));
        _tr_str_release(_recv_name);
        _tr_str_release(_bm_obj_nm);
        return _trret277;
    } else if (_t1640.tag == Expr_EPropAccess) {
        __auto_type obj = _t1640.data.EPropAccess.obj;
__auto_type prop = _t1640.data.EPropAccess.prop;
#line 7682 "src/sema.tr"
        __auto_type _t1824 = (*obj);
        if (_t1824.tag == Expr_EIdent) {
            __auto_type _pmv_cn = _t1824.data.EIdent.name;
#line 7684 "src/sema.tr"
            Symbol* _pmv_sym = Sema_resolve(self, _pmv_cn);
#line 7686 "src/sema.tr"
            if (((((_tr_dict_contains(self->enums, _tr_strz(_pmv_cn)) && (!_tr_str_eqv((_pmv_cn), (_tr_str_lit_len("Option", 6LL))))) && (!_tr_str_eqv((_pmv_cn), (_tr_str_lit_len("Result", 6LL))))) && (_pmv_sym->kind.tag != SymbolKind_make_SVariable().tag)) && (!_tr_dict_contains(self->ambiguous_classes, _tr_strz(_pmv_cn))))) {
#line 7687 "src/sema.tr"
                EnumDef* _ev_def = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(_pmv_cn)));
#line 7688 "src/sema.tr"
                bool _ev_found = false;
#line 7689 "src/sema.tr"
                long long _ev_i = 0LL;
#line 7690 "src/sema.tr"
                while ((_ev_i < _ev_def->variants->len)) {
#line 7691 "src/sema.tr"
                    if (_tr_str_eqv((((VariantDef*)List_ptr_get(_ev_def->variants, _ev_i))->name), (prop))) {
#line 7691 "src/sema.tr"
                        _ev_found = true;
                    }
#line 7692 "src/sema.tr"
                    _ev_i = (_ev_i + 1LL);
                }
#line 7693 "src/sema.tr"
                _ev_i = 0LL;
#line 7694 "src/sema.tr"
                while ((_ev_i < _ev_def->methods->len)) {
#line 7695 "src/sema.tr"
                    if (_tr_str_eqv((((FunctionDef*)List_ptr_get(_ev_def->methods, _ev_i))->name), (prop))) {
#line 7695 "src/sema.tr"
                        _ev_found = true;
                    }
#line 7696 "src/sema.tr"
                    _ev_i = (_ev_i + 1LL);
                }
#line 7697 "src/sema.tr"
                if ((((((!_ev_found) && (!Sema_is_universal_method(self, prop))) && (!_tr_str_eqv((prop), (_tr_str_lit_len("tag", 3LL))))) && (!_tr_str_eqv((prop), (_tr_str_lit_len("name", 4LL))))) && (!_tr_str_eqv((prop), (_tr_str_lit_len("values", 6LL)))))) {
#line 7698 "src/sema.tr"
                    ({ TrStr _at_t1825 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-7] Enum '", 12LL)), (_pmv_cn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has no variant or method '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: check the spelling of the variant name.", 53LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1825); _tr_str_release(_at_t1825); });
                }
            }
#line 7706 "src/sema.tr"
            if ((((_tr_str_eqv((_pmv_sym->name), (_tr_str_lit_len("", 0LL))) || (_pmv_sym->kind.tag == SymbolKind_make_SClass().tag)) && _tr_dict_contains(self->classes, _tr_strz(_pmv_cn))) && Sema_class_method_exists(self, _pmv_cn, prop))) {
#line 7707 "src/sema.tr"
                ClassDef* _pmv_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_pmv_cn)));
#line 7708 "src/sema.tr"
                FunctionDef* _pmv_m = ((FunctionDef*)List_ptr_get(_pmv_cls->methods, 0LL));
#line 7709 "src/sema.tr"
                bool _pmv_found = false;
#line 7710 "src/sema.tr"
                long long _pmv_mi = 0LL;
#line 7711 "src/sema.tr"
                while ((_pmv_mi < _pmv_cls->methods->len)) {
#line 7712 "src/sema.tr"
                    FunctionDef* _pmv_mm = ((FunctionDef*)List_ptr_get(_pmv_cls->methods, _pmv_mi));
#line 7713 "src/sema.tr"
                    if (_tr_str_eqv((_pmv_mm->name), (prop))) {
#line 7714 "src/sema.tr"
                        bool _pmv_is_static = true;
#line 7715 "src/sema.tr"
                        if (((_pmv_mm->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_pmv_mm->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7716 "src/sema.tr"
                            _pmv_is_static = false;
                        }
#line 7717 "src/sema.tr"
                        if (_pmv_is_static) {
#line 7718 "src/sema.tr"
                            _pmv_m = _tr_obj_retain(_pmv_mm);
#line 7719 "src/sema.tr"
                            _pmv_found = true;
                        }
                    }
#line 7720 "src/sema.tr"
                    _pmv_mi = (_pmv_mi + 1LL);
                }
#line 7721 "src/sema.tr"
                if (_pmv_found) {
#line 7722 "src/sema.tr"
                    AstType* _pmv_fnty = AstType_init(_tr_str_lit_len("def", 3LL));
#line 7723 "src/sema.tr"
                    long long _pmv_pi = 0LL;
#line 7724 "src/sema.tr"
                    while ((_pmv_pi < _pmv_m->params->len)) {
#line 7725 "src/sema.tr"
                        ({ __auto_type _ord_t1826 = (((Param*)List_ptr_get(_pmv_m->params, _pmv_pi))->ty); List_ptr_append(_pmv_fnty->args, _ord_t1826); });
#line 7726 "src/sema.tr"
                        _pmv_pi = (_pmv_pi + 1LL);
                    }
#line 7727 "src/sema.tr"
                    if ((((unsigned long long)(_pmv_m->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 7728 "src/sema.tr"
                        if (((((unsigned long long)(_pmv_m->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*_pmv_m->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 7729 "src/sema.tr"
                            ({ __auto_type _ord_t1827 = (box_asttype(throws_result_type((*_pmv_m->ret_ty), _pmv_m->throws_ty))); List_ptr_append(_pmv_fnty->args, _ord_t1827); });
                        } else {
#line 7731 "src/sema.tr"
                            List_ptr_append(_pmv_fnty->args, _pmv_m->ret_ty);
                        }
                    } else {
#line 7733 "src/sema.tr"
                        ({ __auto_type _ord_t1828 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(_pmv_fnty->args, _ord_t1828); });
                    }
#line 7734 "src/sema.tr"
                    HirExpr* _trret278 = ({ TrStr _at_t1829 = (({ TrStr _tr_cl = (_tr_strx_concatv((_pmv_cn), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t1829, _pmv_fnty, false))); _tr_str_release(_at_t1829); _wr; });
                    return _trret278;
                }
            }
        } else if (1) {
            __auto_type _ = _t1824;
            /* pass */
        }
#line 7736 "src/sema.tr"
        bool _saved_recv_pa = self->in_recv_pos;
#line 7737 "src/sema.tr"
        self->in_recv_pos = true;
#line 7738 "src/sema.tr"
        HirExpr* hobj = Sema_lower_expr(self, obj);
#line 7739 "src/sema.tr"
        self->in_recv_pos = _saved_recv_pa;
#line 7740 "src/sema.tr"
        TrStr hobj_ty_n = _tr_str_retain(hir_expr_type(hobj)->name);
#line 7741 "src/sema.tr"
        AstType* hobj_ty_full = hir_expr_type(hobj);
#line 7742 "src/sema.tr"
        AstType* ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7743 "src/sema.tr"
        if (true) {
#line 7744 "src/sema.tr"
            TrStr _pa_ff = Sema_firm_family(self, hobj);
#line 7745 "src/sema.tr"
            if (((_tr_str_eqv((_pa_ff), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_pa_ff), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_pa_ff), (_tr_str_lit_len("bool", 4LL))))) {
#line 7746 "src/sema.tr"
                ({ TrStr _at_t1830 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-6] Cannot read '.", 20LL)), (prop))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _pa_ff)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": a ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_pa_ff)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" has no fields.\n      FIX: check that the receiver is the object you meant; numbers and booleans only have methods such as abs() or to_str().", 141LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1830); _tr_str_release(_at_t1830); });
            }
        }
#line 7747 "src/sema.tr"
        if (((((_tr_dict_contains(self->classes, _tr_strz(hobj_ty_n)) && (!_tr_dict_contains(self->ambiguous_classes, _tr_strz(hobj_ty_n)))) && (!_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Result", 6LL))))) && (!_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Option", 6LL))))) && (!Sema_is_builtin_dispatch_class(self, hobj_ty_n)))) {
#line 7748 "src/sema.tr"
            if ((((!Sema_class_has_field(self, hobj_ty_n, prop)) && (!Sema_class_method_exists(self, hobj_ty_n, prop))) && (!Sema_is_universal_method(self, prop)))) {
#line 7749 "src/sema.tr"
                ({ TrStr _at_t1831 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-7] Type '", 12LL)), (hobj_ty_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has no field '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: check the spelling, or declare 'pub ", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": <type>' in the class.", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1831); _tr_str_release(_at_t1831); });
            }
        }
#line 7750 "src/sema.tr"
        TrStr _pc_f = Sema_firm_family(self, hobj);
#line 7751 "src/sema.tr"
        if ((((_tr_str_eqv((_pc_f), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((_pc_f), (_tr_str_lit_len("Dict", 4LL)))) && (!Sema_class_method_exists(self, hobj_ty_n, prop))) && (!Sema_class_has_field(self, hobj_ty_n, prop)))) {
#line 7752 "src/sema.tr"
            if ((!((((((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("keys", 4LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("values", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("items", 5LL)))))) {
#line 7753 "src/sema.tr"
                ({ TrStr _at_t1832 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-7] A ", 8LL)), (_pc_f))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" has no field '", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: collections expose .len (and methods such as .append()); check the spelling.", 90LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1832); _tr_str_release(_at_t1832); });
            }
        } else if ((_tr_str_eqv((_pc_f), (_tr_str_lit_len("str", 3LL))) && (!((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL))))))) {
#line 7755 "src/sema.tr"
            ({ TrStr _at_t1833 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-7] A str has no field '", 26LL)), (prop))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: strings expose .len and methods such as .upper(); check the spelling.", 83LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1833); _tr_str_release(_at_t1833); });
        }
#line 7756 "src/sema.tr"
        if ((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Result", 6LL))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Option", 6LL))))) {
#line 7757 "src/sema.tr"
            if ((!((((((((((((_tr_str_eqv((prop), (_tr_str_lit_len("err", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("ok", 2LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_err", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_ok", 5LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_some", 7LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_none", 7LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("val", 3LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("value", 5LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("some", 4LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("none", 4LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("tag", 3LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL)))) || (((_tr_str_lenv((prop)) > 0LL) && (strcmp(_tr_strz(_tr_str_slicev((prop), 0LL, 1LL)), _tr_strz(_tr_str_lit_len("A", 1LL))) >= 0)) && (strcmp(_tr_strz(_tr_str_slicev((prop), 0LL, 1LL)), _tr_strz(_tr_str_lit_len("Z", 1LL))) <= 0))))) {
#line 7758 "src/sema.tr"
                ({ TrStr _at_t1834 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-7] '", 7LL)), (hobj_ty_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has no field '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: use .is_ok / .is_err / .ok / .err (Result) or .is_some / .is_none / .value (Option), or the unwrap() family of methods.", 133LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1834); _tr_str_release(_at_t1834); });
            }
        }
#line 7759 "src/sema.tr"
        if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Result", 6LL)))) {
#line 7760 "src/sema.tr"
            if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_err", 6LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_ok", 5LL))))) {
#line 7760 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((prop), (_tr_str_lit_len("ok", 2LL))) && (hobj_ty_full->args->len > 0LL))) {
#line 7761 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            } else if ((_tr_str_eqv((prop), (_tr_str_lit_len("err", 3LL))) && (hobj_ty_full->args->len > 1LL))) {
#line 7762 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 1LL)));
            }
        } else if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Option", 6LL)))) {
#line 7764 "src/sema.tr"
            if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_some", 7LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_none", 7LL))))) {
#line 7764 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (((_tr_str_eqv((prop), (_tr_str_lit_len("value", 5LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("val", 3LL)))) && (hobj_ty_full->args->len > 0LL))) {
#line 7765 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            }
        } else if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("StringObj", 9LL)))) {
#line 7767 "src/sema.tr"
            if (_tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL)))) {
#line 7767 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
            } else if ((_tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL))))) {
#line 7768 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("StringBuilder", 13LL)))) {
#line 7770 "src/sema.tr"
            if (_tr_str_eqv((prop), (_tr_str_lit_len("buf", 3LL)))) {
#line 7770 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("StringObj", 9LL));
            }
        } else if ((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("List", 4LL))))) {
#line 7772 "src/sema.tr"
            if (((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL))))) {
#line 7772 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if (_tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL)))) {
#line 7773 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
            }
        } else if (((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Set", 3LL))))) {
#line 7775 "src/sema.tr"
            if (((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL))))) {
#line 7775 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Shared", 6LL))) && (hobj_ty_full->args->len > 0LL)) && ({ __auto_type _ord_t1835 = ((*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)))->name); __auto_type _wr = (_tr_dict_contains(self->classes, _tr_strz(_ord_t1835))); _wr; }))) {
#line 7780 "src/sema.tr"
            ClassDef* _scls = ({ __auto_type _ord_t1836 = ((*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)))->name); __auto_type _wr = (((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_ord_t1836)))); _wr; });
#line 7781 "src/sema.tr"
            long long _sfi = 0LL;
#line 7782 "src/sema.tr"
            while ((_sfi < _scls->fields->len)) {
#line 7783 "src/sema.tr"
                FieldDef* _sfld = ((FieldDef*)List_ptr_get(_scls->fields, _sfi));
#line 7784 "src/sema.tr"
                if (_tr_str_eqv((_sfld->name), (prop))) {
#line 7785 "src/sema.tr"
                    if ((((unsigned long long)(_sfld->ty)) != ((unsigned long long)(0LL)))) {
#line 7785 "src/sema.tr"
                        ret_ty = (*_sfld->ty);
                    }
                }
#line 7786 "src/sema.tr"
                _sfi = (_sfi + 1LL);
            }
        } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty_n))) {
#line 7788 "src/sema.tr"
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty_n)));
#line 7789 "src/sema.tr"
            long long _fi = 0LL;
#line 7790 "src/sema.tr"
            while ((_fi < _cls->fields->len)) {
#line 7791 "src/sema.tr"
                FieldDef* _fld = ((FieldDef*)List_ptr_get(_cls->fields, _fi));
#line 7792 "src/sema.tr"
                if (_tr_str_eqv((_fld->name), (prop))) {
#line 7793 "src/sema.tr"
                    if ((((unsigned long long)(_fld->ty)) != ((unsigned long long)(0LL)))) {
#line 7793 "src/sema.tr"
                        ret_ty = (*_fld->ty);
                    }
                }
#line 7794 "src/sema.tr"
                _fi = (_fi + 1LL);
            }
#line 7806 "src/sema.tr"
            if (((_cls->generics->len > 0LL) && (hobj_ty_full->args->len > 0LL))) {
#line 7807 "src/sema.tr"
                ret_ty = Sema__subst_ret_generics(self, ret_ty, _cls->generics, hobj_ty_full->args);
            }
        } else if (_tr_dict_contains(self->enums, _tr_strz(hobj_ty_n))) {
#line 7810 "src/sema.tr"
            ret_ty = AstType_init(hobj_ty_n);
        }
#line 7811 "src/sema.tr"
        if ((_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("void", 4LL))) && ((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL)))))) {
#line 7812 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        }
#line 7813 "src/sema.tr"
        HirExpr* _trret279 = box_hirexpr(HirExpr_ctor_EPropAccess(hobj, prop, ret_ty));
        _tr_str_release(hobj_ty_n);
        _tr_str_release(_pc_f);
        return _trret279;
    } else if (_t1640.tag == Expr_EIndex) {
        __auto_type obj = _t1640.data.EIndex.obj;
__auto_type idx_inner = _t1640.data.EIndex._tr_v_index;
#line 7815 "src/sema.tr"
        HirExpr* hexpr_obj = Sema_lower_expr(self, obj);
#line 7816 "src/sema.tr"
        TrStr obj_ty_n = _tr_str_retain(hir_expr_type(hexpr_obj)->name);
#line 7817 "src/sema.tr"
        TrStr obj_name = _tr_str_lit_len("", 0LL);
#line 7818 "src/sema.tr"
        __auto_type _t1837 = (*obj);
        if (_t1837.tag == Expr_EIdent) {
            __auto_type n = _t1837.data.EIdent.name;
            TrStr _strtmp_t1838 = _tr_str_retain(n);
            _tr_str_release(obj_name);
            obj_name = _strtmp_t1838;
        } else if (1) {
            __auto_type _ = _t1837;
            /* pass */
        }
#line 7821 "src/sema.tr"
        bool is_generic = false;
#line 7822 "src/sema.tr"
        TrStr generic_arg_n = _tr_str_lit_len("", 0LL);
#line 7823 "src/sema.tr"
        AstType** generic_arg_ty = ((AstType**)(0LL));
#line 7827 "src/sema.tr"
        List_ptr* generic_args = (void*)List_ptr_new();
#line 7828 "src/sema.tr"
        if ((((unsigned long long)(idx_inner)) == ((unsigned long long)(0LL)))) {
#line 7829 "src/sema.tr"
            HirExpr* _trret280 = hexpr_obj;
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return _trret280;
        }
#line 7830 "src/sema.tr"
        __auto_type _t1839 = (*idx_inner);
        if (_t1839.tag == Expr_ETuple) {
            __auto_type _tup_targs = _t1839.data.ETuple.items;
#line 7833 "src/sema.tr"
            is_generic = true;
#line 7834 "src/sema.tr"
            long long _tti = 0LL;
#line 7835 "src/sema.tr"
            while ((_tti < _tup_targs->len)) {
#line 7836 "src/sema.tr"
                List_ptr_append(generic_args, Sema__targ_of(self, ((Expr*)List_ptr_get(_tup_targs, _tti))));
#line 7837 "src/sema.tr"
                _tti = (_tti + 1LL);
            }
#line 7838 "src/sema.tr"
            if ((generic_args->len > 0LL)) {
#line 7839 "src/sema.tr"
                generic_arg_ty = ((AstType**)List_ptr_get(generic_args, 0LL));
#line 7840 "src/sema.tr"
                TrStr _strtmp_t1840 = _tr_str_retain((*((AstType**)List_ptr_get(generic_args, 0LL)))->name);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t1840;
            }
        } else if (_t1839.tag == Expr_ETypeArg) {
            __auto_type targ_ty = _t1839.data.ETypeArg.ty;
#line 7842 "src/sema.tr"
            is_generic = true;
#line 7843 "src/sema.tr"
            generic_arg_ty = targ_ty;
#line 7844 "src/sema.tr"
            TrStr _strtmp_t1841 = _tr_str_retain((*targ_ty)->name);
            _tr_str_release(generic_arg_n);
            generic_arg_n = _strtmp_t1841;
        } else if (_t1839.tag == Expr_EIdent) {
            __auto_type iname = _t1839.data.EIdent.name;
#line 7846 "src/sema.tr"
            bool is_param = false;
#line 7847 "src/sema.tr"
            if ((!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))))) {
#line 7848 "src/sema.tr"
                if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
#line 7849 "src/sema.tr"
                    ClassDef* cc = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
#line 7850 "src/sema.tr"
                    long long ci = 0LL;
#line 7851 "src/sema.tr"
                    while ((ci < cc->generics->len)) {
#line 7852 "src/sema.tr"
                        if (({ TrStr _cmpa_t1842 = (List_TrStr_get(cc->generics, ci)); TrStr _cmpb_t1843 = (iname); _Bool _cmpr_t1844 = _tr_str_eqv(_cmpa_t1842, _cmpb_t1843); _tr_str_release(_cmpa_t1842); _cmpr_t1844; })) {
#line 7852 "src/sema.tr"
                            is_param = true;
                        }
#line 7853 "src/sema.tr"
                        ci = (ci + 1LL);
                    }
                }
            }
#line 7854 "src/sema.tr"
            long long fi = 0LL;
#line 7855 "src/sema.tr"
            while ((fi < self->current_func_generics->len)) {
#line 7856 "src/sema.tr"
                if (({ TrStr _cmpa_t1845 = (List_TrStr_get(self->current_func_generics, fi)); TrStr _cmpb_t1846 = (iname); _Bool _cmpr_t1847 = _tr_str_eqv(_cmpa_t1845, _cmpb_t1846); _tr_str_release(_cmpa_t1845); _cmpr_t1847; })) {
#line 7856 "src/sema.tr"
                    is_param = true;
                }
#line 7857 "src/sema.tr"
                fi = (fi + 1LL);
            }
#line 7859 "src/sema.tr"
            if ((((((((((((((Sema_is_primitive_name(self, iname) || _tr_str_eqv((iname), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("Str", 3LL)))) || is_param) || (_tr_strlen(_tr_strz(iname)) == 1LL)) || _tr_dict_contains(self->classes, _tr_strz(iname))) || _tr_str_eqv((iname), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_dict_contains(self->enums, _tr_strz(iname))) || _tr_str_eqv((iname), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("tuple", 5LL))))) {
#line 7860 "src/sema.tr"
                is_generic = true;
#line 7861 "src/sema.tr"
                TrStr _strtmp_t1848 = _tr_str_retain(iname);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t1848;
            }
        } else if (_t1839.tag == Expr_EIndex) {
#line 7867 "src/sema.tr"
            is_generic = true;
#line 7868 "src/sema.tr"
            AstType** nested_ty = Sema_build_ast_type(self, idx_inner);
#line 7869 "src/sema.tr"
            if ((((unsigned long long)(nested_ty)) != ((unsigned long long)(0LL)))) {
#line 7870 "src/sema.tr"
                TrStr _strtmp_t1849 = _tr_str_retain((*nested_ty)->name);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t1849;
#line 7871 "src/sema.tr"
                generic_arg_ty = nested_ty;
            }
        } else if (1) {
            __auto_type _ = _t1839;
            /* pass */
        }
#line 7879 "src/sema.tr"
        bool obj_is_type = (((((((((((_tr_str_eqv((obj_name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Pointer", 7LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("alloc", 5LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("dealloc", 7LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("resize", 6LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("copy", 4LL)))) || _tr_dict_contains(self->classes, _tr_strz(obj_name))) || _tr_dict_contains(self->enums, _tr_strz(obj_name)));
#line 7880 "src/sema.tr"
        if ((is_generic && obj_is_type)) {
#line 7884 "src/sema.tr"
            TrStr eff_ty_n = _tr_str_retain(obj_ty_n);
#line 7885 "src/sema.tr"
            if (((_tr_str_eqv((eff_ty_n), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("", 0LL)))) && (((((_tr_str_eqv((obj_name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Pointer", 7LL)))))) {
#line 7886 "src/sema.tr"
                TrStr _strtmp_t1850 = _tr_str_retain(obj_name);
                _tr_str_release(eff_ty_n);
                eff_ty_n = _strtmp_t1850;
            }
#line 7887 "src/sema.tr"
            if (((((((_tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Pointer", 7LL)))) && (!_tr_str_eqv((generic_arg_n), (_tr_str_lit_len("", 0LL)))))) {
#line 7888 "src/sema.tr"
                AstType* container_ty = AstType_init(eff_ty_n);
#line 7889 "src/sema.tr"
                container_ty->args = (void*)List_ptr_new();
#line 7893 "src/sema.tr"
                if ((generic_args->len > 0LL)) {
#line 7894 "src/sema.tr"
                    long long _gci = 0LL;
#line 7895 "src/sema.tr"
                    while ((_gci < generic_args->len)) {
#line 7896 "src/sema.tr"
                        ({ __auto_type _ord_t1851 = (((AstType**)List_ptr_get(generic_args, _gci))); List_ptr_append(container_ty->args, _ord_t1851); });
#line 7897 "src/sema.tr"
                        _gci = (_gci + 1LL);
                    }
                } else if ((((unsigned long long)(generic_arg_ty)) != ((unsigned long long)(0LL)))) {
#line 7899 "src/sema.tr"
                    List_ptr_append(container_ty->args, generic_arg_ty);
                } else {
#line 7901 "src/sema.tr"
                    ({ __auto_type _ord_t1852 = (box_asttype(AstType_init(generic_arg_n))); List_ptr_append(container_ty->args, _ord_t1852); });
                }
#line 7902 "src/sema.tr"
                if ((((_tr_str_eqv((obj_name), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("dealloc", 7LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("resize", 6LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("copy", 4LL))))) {
#line 7903 "src/sema.tr"
                    HirExpr* _trret281 = box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_index_operand(self, idx_inner, hexpr_obj), container_ty));
                    _tr_str_release(obj_ty_n);
                    _tr_str_release(obj_name);
                    _tr_str_release(generic_arg_n);
                    _tr_str_release(eff_ty_n);
                    return _trret281;
                }
#line 7904 "src/sema.tr"
                HirExpr* _trret282 = box_hirexpr(HirExpr_ctor_EIdent(eff_ty_n, container_ty, false));
                _tr_str_release(obj_ty_n);
                _tr_str_release(obj_name);
                _tr_str_release(generic_arg_n);
                _tr_str_release(eff_ty_n);
                return _trret282;
            }
#line 7906 "src/sema.tr"
            if (((_tr_dict_contains(self->classes, _tr_strz(obj_name)) || _tr_dict_contains(self->enums, _tr_strz(obj_name))) && (!_tr_str_eqv((generic_arg_n), (_tr_str_lit_len("", 0LL)))))) {
#line 7907 "src/sema.tr"
                AstType* cls_ty = AstType_init(obj_name);
#line 7908 "src/sema.tr"
                cls_ty->args = (void*)List_ptr_new();
#line 7909 "src/sema.tr"
                if ((generic_args->len > 0LL)) {
#line 7910 "src/sema.tr"
                    long long _cgi = 0LL;
#line 7911 "src/sema.tr"
                    while ((_cgi < generic_args->len)) {
#line 7912 "src/sema.tr"
                        ({ __auto_type _ord_t1853 = (((AstType**)List_ptr_get(generic_args, _cgi))); List_ptr_append(cls_ty->args, _ord_t1853); });
#line 7913 "src/sema.tr"
                        _cgi = (_cgi + 1LL);
                    }
                } else if ((((unsigned long long)(generic_arg_ty)) != ((unsigned long long)(0LL)))) {
#line 7916 "src/sema.tr"
                    List_ptr_append(cls_ty->args, generic_arg_ty);
                } else {
#line 7918 "src/sema.tr"
                    ({ __auto_type _ord_t1854 = (box_asttype(AstType_init(generic_arg_n))); List_ptr_append(cls_ty->args, _ord_t1854); });
                }
#line 7920 "src/sema.tr"
                Sema_check_class_bounds(self, obj_name, cls_ty->args);
#line 7921 "src/sema.tr"
                HirExpr* _trret283 = box_hirexpr(HirExpr_ctor_EIdent(obj_name, cls_ty, false));
                _tr_str_release(obj_ty_n);
                _tr_str_release(obj_name);
                _tr_str_release(generic_arg_n);
                _tr_str_release(eff_ty_n);
                return _trret283;
            }
#line 7922 "src/sema.tr"
            HirExpr* _trret284 = hexpr_obj;
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            _tr_str_release(eff_ty_n);
            return _trret284;
        }
#line 7927 "src/sema.tr"
        if ((_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Dict", 4LL))))) {
#line 7928 "src/sema.tr"
            AstType* dval_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7929 "src/sema.tr"
            AstType* dobj_full = hir_expr_type(hexpr_obj);
#line 7930 "src/sema.tr"
            if ((dobj_full->args->len > 1LL)) {
#line 7930 "src/sema.tr"
                dval_ty = (*((AstType**)List_ptr_get(dobj_full->args, 1LL)));
            }
#line 7931 "src/sema.tr"
            List_ptr* didx_args = (void*)List_ptr_new();
#line 7932 "src/sema.tr"
            List_ptr_append(didx_args, Sema_lower_index_operand(self, idx_inner, hexpr_obj));
#line 7933 "src/sema.tr"
            HirExpr* _trret285 = box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit_len("get_index", 9LL), didx_args, dval_ty));
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return _trret285;
        }
#line 7936 "src/sema.tr"
        if (_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Tuple", 5LL)))) {
#line 7937 "src/sema.tr"
            AstType* tup_full = hir_expr_type(hexpr_obj);
#line 7938 "src/sema.tr"
            AstType* telem_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7939 "src/sema.tr"
            __auto_type _t1855 = (*idx_inner);
            if (_t1855.tag == Expr_ELitInt) {
                __auto_type tup_ci = _t1855.data.ELitInt.val;
#line 7941 "src/sema.tr"
                if (((tup_ci >= 0LL) && (tup_ci < tup_full->args->len))) {
#line 7942 "src/sema.tr"
                    telem_ty = (*((AstType**)List_ptr_get(tup_full->args, tup_ci)));
                }
            } else if (1) {
                __auto_type _ = _t1855;
                /* pass */
            }
#line 7944 "src/sema.tr"
            List_ptr* tup_args = (void*)List_ptr_new();
#line 7945 "src/sema.tr"
            List_ptr_append(tup_args, Sema_lower_index_operand(self, idx_inner, hexpr_obj));
#line 7946 "src/sema.tr"
            HirExpr* _trret286 = box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit_len("get_index", 9LL), tup_args, telem_ty));
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return _trret286;
        }
#line 7947 "src/sema.tr"
        if ((((((!Sema_is_primitive_name(self, obj_ty_n)) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("str", 3LL))))) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Pointer", 7LL))))) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Vec", 3LL)))))) {
#line 7948 "src/sema.tr"
            List_ptr* call_args = (void*)List_ptr_new();
#line 7949 "src/sema.tr"
            List_ptr_append(call_args, Sema_lower_index_operand(self, idx_inner, hexpr_obj));
#line 7950 "src/sema.tr"
            HirExpr* _trret287 = box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit_len("get_index", 9LL), call_args, AstType_init(_tr_str_lit_len("void", 4LL))));
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return _trret287;
        }
#line 7952 "src/sema.tr"
        AstType* elem_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7953 "src/sema.tr"
        if ((_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Vec", 3LL))))) {
#line 7954 "src/sema.tr"
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
#line 7955 "src/sema.tr"
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if (_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 7957 "src/sema.tr"
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
#line 7958 "src/sema.tr"
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if (_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("str", 3LL)))) {
#line 7959 "src/sema.tr"
            elem_ty = AstType_init(_tr_str_lit_len("char", 4LL));
        }
#line 7960 "src/sema.tr"
        HirExpr* _trret288 = box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_index_operand(self, idx_inner, hexpr_obj), elem_ty));
        _tr_str_release(obj_ty_n);
        _tr_str_release(obj_name);
        _tr_str_release(generic_arg_n);
        return _trret288;
    } else if (_t1640.tag == Expr_ESizeOf) {
        __auto_type ty = _t1640.data.ESizeOf.ty;
#line 7962 "src/sema.tr"
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
#line 7962 "src/sema.tr"
            return ({ __auto_type _ord_t1856 = (AstType_init(_tr_str_lit_len("void", 4LL))); __auto_type _ord_t1857 = (AstType_init(_tr_str_lit_len("int", 3LL))); __auto_type _wr = (box_hirexpr(HirExpr_ctor_ESizeOf(_ord_t1856, _ord_t1857))); _wr; });
        }
#line 7963 "src/sema.tr"
        return ({ __auto_type _ord_t1858 = ((*ty)); __auto_type _ord_t1859 = (AstType_init(_tr_str_lit_len("int", 3LL))); __auto_type _wr = (box_hirexpr(HirExpr_ctor_ESizeOf(_ord_t1858, _ord_t1859))); _wr; });
    } else if (_t1640.tag == Expr_ECast) {
        __auto_type expr = _t1640.data.ECast.expr;
__auto_type ty = _t1640.data.ECast.ty;
#line 7965 "src/sema.tr"
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
#line 7965 "src/sema.tr"
            return Sema_lower_expr(self, expr);
        }
#line 7966 "src/sema.tr"
        return ({ __auto_type _ord_t1860 = (Sema_lower_expr(self, expr)); __auto_type _ord_t1861 = ((*ty)); __auto_type _wr = (box_hirexpr(HirExpr_ctor_ECast(_ord_t1860, _ord_t1861))); _wr; });
    } else if (_t1640.tag == Expr_EFString) {
        __auto_type parts = _t1640.data.EFString.parts;
#line 7968 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("an f-string", 11LL), _tr_str_lit_len("build output in a fixed byte buffer, or print the parts separately; plain string literals are fine (they live in rodata)", 120LL));
#line 7969 "src/sema.tr"
        List_ptr* hparts = (void*)List_ptr_new();
#line 7970 "src/sema.tr"
        long long m = 0LL;
#line 7971 "src/sema.tr"
        while ((m < parts->len)) {
#line 7972 "src/sema.tr"
            FStringPart* p_ast = ((FStringPart*)List_ptr_get(parts, m));
#line 7973 "src/sema.tr"
            HirFStringPart* hp = HirFStringPart_init();
#line 7974 "src/sema.tr"
            hp->is_expr = p_ast->is_expr;
#line 7975 "src/sema.tr"
            hp->text = _tr_str_retain(p_ast->text);
#line 7976 "src/sema.tr"
            hp->text_len = p_ast->text_len;
#line 7977 "src/sema.tr"
            hp->fmt_spec = _tr_str_retain(p_ast->fmt_spec);
#line 7978 "src/sema.tr"
            hp->expr = Sema_lower_expr(self, p_ast->expr);
#line 7979 "src/sema.tr"
            long long sai = 0LL;
#line 7980 "src/sema.tr"
            while ((sai < p_ast->spec_args->len)) {
#line 7981 "src/sema.tr"
                ({ __auto_type _ord_t1862 = (Sema_lower_expr(self, ((Expr*)List_ptr_get(p_ast->spec_args, sai)))); List_ptr_append(hp->spec_args, _ord_t1862); });
#line 7982 "src/sema.tr"
                sai = (sai + 1LL);
            }
#line 7983 "src/sema.tr"
            List_ptr_append(hparts, _tr_obj_retain(hp));
#line 7984 "src/sema.tr"
            m = (m + 1LL);
            _tr_obj_release(hp, _trdrop_HirFStringPart);
        }
#line 7985 "src/sema.tr"
        HirExpr* _trret289 = box_hirexpr(HirExpr_ctor_EFString(hparts, AstType_init(_tr_str_lit_len("str", 3LL))));
        return _trret289;
    } else if (_t1640.tag == Expr_ETuple) {
        __auto_type items = _t1640.data.ETuple.items;
#line 7990 "src/sema.tr"
        if ((self->no_heap && self->heap_boxes_tuples)) {
#line 7991 "src/sema.tr"
            Sema_check_no_heap(self, _tr_str_lit_len("a tuple literal (heap-boxed on the LLVM/native backend)", 55LL), _tr_str_lit_len("use the C backend (stack tuples) for --no-heap, or return values via out-params / a @value_type struct", 102LL));
        }
#line 7992 "src/sema.tr"
        List_ptr* hitems = (void*)List_ptr_new();
#line 7993 "src/sema.tr"
        long long n = 0LL;
#line 7994 "src/sema.tr"
        while ((n < items->len)) {
#line 7995 "src/sema.tr"
            List_ptr_append(hitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, n))));
#line 7996 "src/sema.tr"
            n = (n + 1LL);
        }
#line 8000 "src/sema.tr"
        AstType* tup_ty = AstType_init(_tr_str_lit_len("Tuple", 5LL));
#line 8001 "src/sema.tr"
        long long m = 0LL;
#line 8002 "src/sema.tr"
        while ((m < hitems->len)) {
#line 8003 "src/sema.tr"
            ({ __auto_type _ord_t1863 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hitems, m))))); List_ptr_append(tup_ty->args, _ord_t1863); });
#line 8004 "src/sema.tr"
            m = (m + 1LL);
        }
#line 8005 "src/sema.tr"
        HirExpr* _trret290 = box_hirexpr(HirExpr_ctor_ETuple(hitems, tup_ty));
        return _trret290;
    } else if (_t1640.tag == Expr_EList) {
        __auto_type items = _t1640.data.EList.items;
#line 8007 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("a list literal []", 17LL), _tr_str_lit_len("use a fixed-size array [T; N] (stack, zero-heap) instead", 56LL));
#line 8008 "src/sema.tr"
        List_ptr* hitems = (void*)List_ptr_new();
#line 8009 "src/sema.tr"
        long long n = 0LL;
#line 8010 "src/sema.tr"
        while ((n < items->len)) {
#line 8011 "src/sema.tr"
            List_ptr_append(hitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, n))));
#line 8012 "src/sema.tr"
            n = (n + 1LL);
        }
#line 8013 "src/sema.tr"
        AstType* list_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 8014 "src/sema.tr"
        if ((hitems->len > 1LL)) {
#line 8015 "src/sema.tr"
            bool _hl_str = false;
#line 8016 "src/sema.tr"
            bool _hl_num = false;
#line 8017 "src/sema.tr"
            long long _hl_i = 0LL;
#line 8018 "src/sema.tr"
            while ((_hl_i < hitems->len)) {
#line 8019 "src/sema.tr"
                TrStr _hl_f = Sema_firm_family(self, ((HirExpr*)List_ptr_get(hitems, _hl_i)));
#line 8020 "src/sema.tr"
                if (_tr_str_eqv((_hl_f), (_tr_str_lit_len("str", 3LL)))) {
#line 8020 "src/sema.tr"
                    _hl_str = true;
                }
#line 8021 "src/sema.tr"
                if ((_tr_str_eqv((_hl_f), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_hl_f), (_tr_str_lit_len("float", 5LL))))) {
#line 8021 "src/sema.tr"
                    _hl_num = true;
                }
#line 8022 "src/sema.tr"
                _hl_i = (_hl_i + 1LL);
                _tr_str_release(_hl_f);
            }
#line 8023 "src/sema.tr"
            if ((_hl_str && _hl_num)) {
#line 8024 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[E-5] Type mismatch: a list literal mixes strings and numbers.\n      FIX: a list holds one element type; convert the items (str(x) / int(s)) or use separate lists.", 163LL));
            }
        }
#line 8025 "src/sema.tr"
        if ((hitems->len > 0LL)) {
#line 8026 "src/sema.tr"
            list_ty->args = (void*)List_ptr_new();
#line 8027 "src/sema.tr"
            ({ __auto_type _ord_t1864 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hitems, 0LL))))); List_ptr_append(list_ty->args, _ord_t1864); });
        } else if ((((unsigned long long)(self->list_hint)) != ((unsigned long long)(0LL)))) {
#line 8030 "src/sema.tr"
            list_ty = (*self->list_hint);
        }
#line 8031 "src/sema.tr"
        self->list_hint = (AstType**)(0LL);
#line 8032 "src/sema.tr"
        HirExpr* _trret291 = box_hirexpr(HirExpr_ctor_EList(hitems, list_ty));
        return _trret291;
    } else if (_t1640.tag == Expr_ESet) {
        __auto_type items = _t1640.data.ESet.items;
#line 8034 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("a set literal", 13LL), _tr_str_lit_len("use a fixed-size array [T; N] + linear scan, or a bitset over a fixed word", 74LL));
#line 8035 "src/sema.tr"
        List_ptr* sitems = (void*)List_ptr_new();
#line 8036 "src/sema.tr"
        long long sn = 0LL;
#line 8037 "src/sema.tr"
        while ((sn < items->len)) {
#line 8038 "src/sema.tr"
            List_ptr_append(sitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, sn))));
#line 8039 "src/sema.tr"
            sn = (sn + 1LL);
        }
#line 8042 "src/sema.tr"
        AstType* set_ty = AstType_init(_tr_str_lit_len("Set", 3LL));
#line 8043 "src/sema.tr"
        if ((sitems->len > 0LL)) {
#line 8044 "src/sema.tr"
            set_ty->args = (void*)List_ptr_new();
#line 8045 "src/sema.tr"
            ({ __auto_type _ord_t1865 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(sitems, 0LL))))); List_ptr_append(set_ty->args, _ord_t1865); });
        }
#line 8046 "src/sema.tr"
        HirExpr* _trret292 = box_hirexpr(HirExpr_ctor_ESet(sitems, set_ty));
        return _trret292;
    } else if (_t1640.tag == Expr_EClosure) {
        __auto_type params = _t1640.data.EClosure.params;
__auto_type ret_ty = _t1640.data.EClosure.ret_ty;
__auto_type body = _t1640.data.EClosure.body;
__auto_type is_async = _t1640.data.EClosure.is_async;
#line 8048 "src/sema.tr"
        self->closure_depth = (self->closure_depth + 1LL);
#line 8051 "src/sema.tr"
        AstType** _cl_hint = self->closure_hint;
#line 8052 "src/sema.tr"
        self->closure_hint = (AstType**)(0LL);
#line 8053 "src/sema.tr"
        bool _cl_hint_ok = ((((unsigned long long)(_cl_hint)) != ((unsigned long long)(0LL))) && ({ __auto_type _ord_t1866 = ((*_cl_hint)->args->len); __auto_type _wr = ((_ord_t1866 == (params->len + 1LL))); _wr; }));
#line 8054 "src/sema.tr"
        Sema_enter_scope(self);
#line 8055 "src/sema.tr"
        long long cap_idx = (self->scopes->len - 1LL);
#line 8056 "src/sema.tr"
        List_ptr* hparams = (void*)List_ptr_new();
#line 8057 "src/sema.tr"
        long long l = 0LL;
#line 8058 "src/sema.tr"
        while ((l < params->len)) {
#line 8059 "src/sema.tr"
            Param* pa = ((Param*)List_ptr_get(params, l));
#line 8060 "src/sema.tr"
            AstType* pa_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 8061 "src/sema.tr"
            if ((((unsigned long long)(pa->ty)) != ((unsigned long long)(0LL)))) {
#line 8062 "src/sema.tr"
                pa_ty = (*pa->ty);
            } else if ((_cl_hint_ok && (((unsigned long long)(((AstType**)List_ptr_get((*_cl_hint)->args, l)))) != ((unsigned long long)(0LL))))) {
#line 8064 "src/sema.tr"
                pa_ty = (*((AstType**)List_ptr_get((*_cl_hint)->args, l)));
            }
#line 8065 "src/sema.tr"
            ({ __auto_type _ord_t1867 = (pa->name); __auto_type _ord_t1868 = (SymbolKind_make_SVariable()); __auto_type _ord_t1869 = (box_asttype(pa_ty)); Sema_declare(self, _ord_t1867, _ord_t1868, _ord_t1869, false); });
#line 8066 "src/sema.tr"
            HirParam* hpa = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 8067 "src/sema.tr"
            hpa->name = _tr_str_retain(pa->name);
#line 8068 "src/sema.tr"
            hpa->ty = pa_ty;
#line 8069 "src/sema.tr"
            List_ptr_append(hparams, _tr_obj_retain(hpa));
#line 8070 "src/sema.tr"
            l = (l + 1LL);
            _tr_obj_release(hpa, _trdrop_HirParam);
        }
#line 8071 "src/sema.tr"
        AstType* r_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 8072 "src/sema.tr"
        if ((((unsigned long long)(ret_ty)) != ((unsigned long long)(0LL)))) {
#line 8072 "src/sema.tr"
            r_ty = (*ret_ty);
        }
#line 8075 "src/sema.tr"
        List_i64_append(self->fn_scope_base, (self->scopes->len - 1LL));
#line 8076 "src/sema.tr"
        HirBlock* clo_body = Sema_lower_block(self, body);
#line 8077 "src/sema.tr"
        List_i64_pop(self->fn_scope_base);
#line 8078 "src/sema.tr"
        Sema_finalize_scope_drops(self, clo_body);
#line 8084 "src/sema.tr"
        List_TrStr* clo_refs = (void*)List_TrStr_new();
#line 8085 "src/sema.tr"
        Sema_collect_block_refs(self, clo_body, clo_refs);
#line 8086 "src/sema.tr"
        List_ptr* clo_caps = (void*)List_ptr_new();
#line 8087 "src/sema.tr"
        TrMap* clo_seen = _tr_dict_new(8LL);
#line 8088 "src/sema.tr"
        long long cri = 0LL;
#line 8089 "src/sema.tr"
        while ((cri < clo_refs->len)) {
#line 8090 "src/sema.tr"
            TrStr rn = List_TrStr_get(clo_refs, cri);
#line 8091 "src/sema.tr"
            if ((!_tr_dict_contains(clo_seen, _tr_strz(rn)))) {
#line 8092 "src/sema.tr"
                _tr_dict_set(clo_seen, _tr_strz(rn), true);
#line 8093 "src/sema.tr"
                if ((!_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, cap_idx))->variables, _tr_strz(rn)))) {
#line 8094 "src/sema.tr"
                    long long si = (cap_idx - 1LL);
#line 8095 "src/sema.tr"
                    AstType** cty_p = (AstType**)(0LL);
#line 8096 "src/sema.tr"
                    bool cap_mut = true;
#line 8097 "src/sema.tr"
                    while ((si >= 0LL)) {
#line 8098 "src/sema.tr"
                        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, si))->variables, _tr_strz(rn))) {
#line 8099 "src/sema.tr"
                            Symbol* cap_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, si))->variables, _tr_strz(rn)));
#line 8100 "src/sema.tr"
                            cty_p = cap_sym->ty;
#line 8101 "src/sema.tr"
                            cap_mut = (cap_sym->is_mut || (cap_sym->kind.tag != SymbolKind_make_SVariable().tag));
#line 8102 "src/sema.tr"
                            si = (0LL - 1LL);
                        } else {
#line 8104 "src/sema.tr"
                            si = (si - 1LL);
                        }
                    }
#line 8105 "src/sema.tr"
                    if ((((unsigned long long)(cty_p)) != ((unsigned long long)(0LL)))) {
#line 8106 "src/sema.tr"
                        HirParam* ccp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 8107 "src/sema.tr"
                        ccp->name = _tr_str_retain(rn);
#line 8108 "src/sema.tr"
                        ccp->ty = (*cty_p);
#line 8113 "src/sema.tr"
                        ccp->by_val = (!cap_mut);
#line 8114 "src/sema.tr"
                        if ((ccp->by_val && _tr_dict_contains(self->classes, _tr_strz(ccp->ty->name)))) {
#line 8115 "src/sema.tr"
                            if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ccp->ty->name)))->is_class)) {
#line 8115 "src/sema.tr"
                                ccp->by_val = false;
                            }
                        }
#line 8116 "src/sema.tr"
                        List_ptr_append(clo_caps, _tr_obj_retain(ccp));
                    }
                }
            }
#line 8117 "src/sema.tr"
            cri = (cri + 1LL);
            _tr_str_release(rn);
        }
#line 8121 "src/sema.tr"
        if ((((unsigned long long)(ret_ty)) == ((unsigned long long)(0LL)))) {
#line 8122 "src/sema.tr"
            AstType* inf_ty = Sema_closure_return_type(self, clo_body);
#line 8123 "src/sema.tr"
            if ((((!_tr_str_eqv((inf_ty->name), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((inf_ty->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((inf_ty->name), (_tr_str_lit_len("None", 4LL)))))) {
#line 8124 "src/sema.tr"
                r_ty = inf_ty;
            }
        }
#line 8125 "src/sema.tr"
        HirExpr hexpr = HirExpr_ctor_EClosure(hparams, r_ty, clo_body, is_async, clo_caps);
#line 8126 "src/sema.tr"
        self->closure_depth = (self->closure_depth - 1LL);
#line 8127 "src/sema.tr"
        Sema_exit_scope(self);
#line 8128 "src/sema.tr"
        HirExpr* _trret293 = box_hirexpr(hexpr);
        _tr_obj_release(clo_body, _trdrop_HirBlock);
        Dict_free(clo_seen);
        return _trret293;
    } else if (_t1640.tag == Expr_EIfElse) {
        __auto_type cond = _t1640.data.EIfElse.cond;
__auto_type then_e = _t1640.data.EIfElse.then_expr;
__auto_type else_e = _t1640.data.EIfElse.else_expr;
#line 8130 "src/sema.tr"
        HirExpr* hcond = Sema_lower_expr(self, cond);
#line 8131 "src/sema.tr"
        HirExpr* hthen = Sema_lower_expr(self, then_e);
#line 8132 "src/sema.tr"
        HirExpr* helse = Sema_lower_expr(self, else_e);
#line 8133 "src/sema.tr"
        AstType* ite_ty = hir_expr_type(hthen);
#line 8134 "src/sema.tr"
        HirExpr* _trret294 = box_hirexpr(HirExpr_ctor_EIfElse(hcond, hthen, helse, ite_ty));
        return _trret294;
    } else if (_t1640.tag == Expr_EDo) {
        __auto_type do_body = _t1640.data.EDo.body;
#line 8145 "src/sema.tr"
        HirExpr* _trret295 = Sema_lower_do_value(self, do_body);
        return _trret295;
    } else if (_t1640.tag == Expr_EMatch) {
        __auto_type m_subj = _t1640.data.EMatch.subj;
__auto_type m_arms = _t1640.data.EMatch.arms;
#line 8151 "src/sema.tr"
        HirExpr* hm_subj = Sema_lower_expr(self, m_subj);
#line 8152 "src/sema.tr"
        AstType* hm_subj_ty = hir_expr_type(hm_subj);
#line 8153 "src/sema.tr"
        List_ptr* hm_arms = (void*)List_ptr_new();
#line 8154 "src/sema.tr"
        AstType* hm_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 8155 "src/sema.tr"
        long long hm_k = 0LL;
#line 8156 "src/sema.tr"
        while ((hm_k < m_arms->len)) {
#line 8157 "src/sema.tr"
            MatchArm* hm_arm = ((MatchArm*)List_ptr_get(m_arms, hm_k));
#line 8158 "src/sema.tr"
            Sema_enter_scope(self);
#line 8159 "src/sema.tr"
            Sema_declare_pattern_binds_typed(self, hm_arm->pat, hm_subj_ty);
#line 8160 "src/sema.tr"
            HirExpr* hm_edo = Sema_lower_do_value(self, (*hm_arm->body));
#line 8161 "src/sema.tr"
            if ((!_tr_str_eqv((hir_expr_type(hm_edo)->name), (_tr_str_lit_len("void", 4LL))))) {
#line 8161 "src/sema.tr"
                hm_ty = hir_expr_type(hm_edo);
            }
#line 8162 "src/sema.tr"
            HirBlock* hm_body = HirBlock_init();
#line 8163 "src/sema.tr"
            HirBlock_push(hm_body, box_hirstmt(HirStmt_ctor_SExpr(hm_edo)));
#line 8164 "src/sema.tr"
            HirMatchArm* h_marm = HirMatchArm_init(hm_arm->pat, hm_body);
#line 8165 "src/sema.tr"
            if ((((unsigned long long)(hm_arm->guard)) != ((unsigned long long)(0LL)))) {
#line 8166 "src/sema.tr"
                h_marm->guard = Sema_lower_expr(self, hm_arm->guard);
            }
#line 8167 "src/sema.tr"
            List_ptr_append(hm_arms, _tr_obj_retain(h_marm));
#line 8168 "src/sema.tr"
            Sema_exit_scope(self);
#line 8169 "src/sema.tr"
            hm_k = (hm_k + 1LL);
            _tr_obj_release(hm_body, _trdrop_HirBlock);
            _tr_obj_release(h_marm, _trdrop_HirMatchArm);
        }
#line 8170 "src/sema.tr"
        Sema_check_match_expr_exhaustive(self, hm_subj_ty->name, m_arms);
#line 8171 "src/sema.tr"
        HirExpr* _trret296 = box_hirexpr(HirExpr_ctor_EMatchExpr(hm_subj, hm_arms, hm_ty));
        return _trret296;
    } else if (_t1640.tag == Expr_ELoop) {
        __auto_type loop_body = _t1640.data.ELoop.body;
#line 8174 "src/sema.tr"
        Sema_enter_scope(self);
#line 8175 "src/sema.tr"
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
#line 8176 "src/sema.tr"
        List_i64_append(self->loop_blk_floor, (-1LL));
#line 8177 "src/sema.tr"
        List_i64_append(self->loop_blk_scope, (-1LL));
#line 8178 "src/sema.tr"
        HirBlock* h_loop_body = Sema_lower_block(self, loop_body);
#line 8179 "src/sema.tr"
        List_i64_pop(self->loop_scope_base);
#line 8180 "src/sema.tr"
        List_i64_pop(self->loop_blk_floor);
#line 8181 "src/sema.tr"
        List_i64_pop(self->loop_blk_scope);
#line 8182 "src/sema.tr"
        Sema_finalize_scope_drops(self, h_loop_body);
#line 8183 "src/sema.tr"
        Sema_exit_scope(self);
#line 8184 "src/sema.tr"
        AstType* loop_ty = Sema_infer_break_type(self, h_loop_body);
#line 8185 "src/sema.tr"
        HirExpr* _trret297 = box_hirexpr(HirExpr_ctor_ELoop(h_loop_body, loop_ty));
        _tr_obj_release(h_loop_body, _trdrop_HirBlock);
        return _trret297;
    } else if (_t1640.tag == Expr_EWhileExpr) {
        __auto_type we_cond = _t1640.data.EWhileExpr.cond;
__auto_type we_body = _t1640.data.EWhileExpr.body;
__auto_type we_else = _t1640.data.EWhileExpr.else_body;
#line 8189 "src/sema.tr"
        HirExpr* h_we_cond = Sema_lower_expr(self, we_cond);
#line 8190 "src/sema.tr"
        Sema_enter_scope(self);
#line 8191 "src/sema.tr"
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
#line 8192 "src/sema.tr"
        List_i64_append(self->loop_blk_floor, (-1LL));
#line 8193 "src/sema.tr"
        List_i64_append(self->loop_blk_scope, (-1LL));
#line 8194 "src/sema.tr"
        HirBlock* h_we_body = Sema_lower_block(self, we_body);
#line 8195 "src/sema.tr"
        List_i64_pop(self->loop_scope_base);
#line 8196 "src/sema.tr"
        List_i64_pop(self->loop_blk_floor);
#line 8197 "src/sema.tr"
        List_i64_pop(self->loop_blk_scope);
#line 8198 "src/sema.tr"
        Sema_finalize_scope_drops(self, h_we_body);
#line 8199 "src/sema.tr"
        Sema_exit_scope(self);
#line 8201 "src/sema.tr"
        HirExpr* h_we_else = Sema_lower_do_value(self, we_else);
#line 8202 "src/sema.tr"
        AstType* we_ty = Sema_infer_break_type(self, h_we_body);
#line 8203 "src/sema.tr"
        if (_tr_str_eqv((we_ty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 8203 "src/sema.tr"
            we_ty = hir_expr_type(h_we_else);
        }
#line 8205 "src/sema.tr"
        HirBlock* h_we_else_b = HirBlock_init();
#line 8206 "src/sema.tr"
        HirBlock_push(h_we_else_b, box_hirstmt(HirStmt_ctor_SExpr(h_we_else)));
#line 8207 "src/sema.tr"
        HirExpr* _trret298 = box_hirexpr(HirExpr_ctor_EWhileExpr(h_we_cond, h_we_body, h_we_else_b, we_ty));
        _tr_obj_release(h_we_body, _trdrop_HirBlock);
        _tr_obj_release(h_we_else_b, _trdrop_HirBlock);
        return _trret298;
    } else if (_t1640.tag == Expr_ETryExpr) {
        __auto_type inner = _t1640.data.ETryExpr.expr;
#line 8209 "src/sema.tr"
        if ((((!self->fn_try_ok) && (self->closure_depth == 0LL)) && (!_tr_str_eqv((self->current_func_name), (_tr_str_lit_len("", 0LL)))))) {
#line 8210 "src/sema.tr"
            ({ TrStr _at_t1870 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-4] '?' used in '", 19LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which neither throws nor returns a Result/Option. FIX: declare it 'def ", 74LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...) throws str -> T' (or return Result[T, E]), or handle the error with 'if r.is_err:' instead of '?'.", 104LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1870); _tr_str_release(_at_t1870); });
        }
#line 8211 "src/sema.tr"
        HirExpr* hinner = Sema_lower_expr(self, inner);
#line 8212 "src/sema.tr"
        AstType* inner_ty = hir_expr_type(hinner);
#line 8214 "src/sema.tr"
        AstType* ok_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 8215 "src/sema.tr"
        if ((_tr_str_eqv((inner_ty->name), (_tr_str_lit_len("Result", 6LL))) && (inner_ty->args->len > 0LL))) {
#line 8216 "src/sema.tr"
            ok_ty = (*((AstType**)List_ptr_get(inner_ty->args, 0LL)));
        } else if ((!_tr_str_eqv((inner_ty->name), (_tr_str_lit_len("void", 4LL))))) {
#line 8218 "src/sema.tr"
            ok_ty = inner_ty;
        }
#line 8219 "src/sema.tr"
        HirExpr* _trret299 = box_hirexpr(HirExpr_ctor_ETryExpr(hinner, ok_ty));
        return _trret299;
    } else if (_t1640.tag == Expr_EAwait) {
        __auto_type inner_await = _t1640.data.EAwait.expr;
#line 8222 "src/sema.tr"
        if ((!self->in_async_fn)) {
#line 8223 "src/sema.tr"
            ({ TrStr _at_t1871 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[C-4] 'await' used outside an async function. FIX: Declare '", 60LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as 'async def ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' to use await inside it.", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1871); _tr_str_release(_at_t1871); });
        }
#line 8224 "src/sema.tr"
        HirExpr* hinner_await = Sema_lower_expr(self, inner_await);
#line 8225 "src/sema.tr"
        AstType* await_ty = hir_expr_type(hinner_await);
#line 8226 "src/sema.tr"
        HirExpr* _trret300 = box_hirexpr(HirExpr_ctor_EAwait(hinner_await, await_ty));
        return _trret300;
    } else if (_t1640.tag == Expr_EDict) {
        __auto_type keys = _t1640.data.EDict.keys;
__auto_type vals = _t1640.data.EDict.vals;
#line 8228 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("a dict/map literal", 18LL), _tr_str_lit_len("use parallel fixed-size arrays [K; N]/[V; N], or a static lookup table", 70LL));
#line 8229 "src/sema.tr"
        List_ptr* h_keys = (void*)List_ptr_new();
#line 8230 "src/sema.tr"
        List_ptr* h_vals = (void*)List_ptr_new();
#line 8231 "src/sema.tr"
        long long di = 0LL;
#line 8232 "src/sema.tr"
        while ((di < keys->len)) {
#line 8233 "src/sema.tr"
            List_ptr_append(h_keys, Sema_lower_expr(self, ((Expr*)List_ptr_get(keys, di))));
#line 8234 "src/sema.tr"
            List_ptr_append(h_vals, Sema_lower_expr(self, ((Expr*)List_ptr_get(vals, di))));
#line 8235 "src/sema.tr"
            di = (di + 1LL);
        }
#line 8239 "src/sema.tr"
        AstType* dict_ty = AstType_init(_tr_str_lit_len("Dict", 4LL));
#line 8240 "src/sema.tr"
        if ((h_keys->len > 0LL)) {
#line 8241 "src/sema.tr"
            ({ __auto_type _ord_t1872 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(h_keys, 0LL))))); List_ptr_append(dict_ty->args, _ord_t1872); });
#line 8242 "src/sema.tr"
            ({ __auto_type _ord_t1873 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(h_vals, 0LL))))); List_ptr_append(dict_ty->args, _ord_t1873); });
        }
#line 8243 "src/sema.tr"
        HirExpr* _trret301 = box_hirexpr(HirExpr_ctor_EDict(h_keys, h_vals, dict_ty));
        return _trret301;
    } else if (_t1640.tag == Expr_EListComp) {
        __auto_type element = _t1640.data.EListComp.element;
__auto_type generators = _t1640.data.EListComp.generators;
#line 8245 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("a list comprehension", 20LL), _tr_str_lit_len("loop over a fixed-size array [T; N] and write into another", 58LL));
#line 8246 "src/sema.tr"
        Sema_enter_scope(self);
#line 8247 "src/sema.tr"
        List_ptr* hgens = (void*)List_ptr_new();
#line 8248 "src/sema.tr"
        long long gc = 0LL;
#line 8249 "src/sema.tr"
        while ((gc < generators->len)) {
#line 8250 "src/sema.tr"
            Comprehension* gen_ast = (*((Comprehension**)List_ptr_get(generators, gc)));
#line 8251 "src/sema.tr"
            HirComprehension** hgen_ptr = (HirComprehension**)(0LL);
#line 8252 "src/sema.tr"
            /* unsafe block */
#line 8252 "src/sema.tr"
            hgen_ptr = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
#line 8253 "src/sema.tr"
            HirComprehension* hgen_val = ((HirComprehension*)_tr_obj_alloc(sizeof(HirComprehension)));
#line 8254 "src/sema.tr"
            hgen_val->target = _tr_str_retain(gen_ast->target);
#line 8255 "src/sema.tr"
            HirExpr* h_iter_lc = Sema_lower_expr(self, gen_ast->iter);
#line 8256 "src/sema.tr"
            hgen_val->iter = h_iter_lc;
#line 8257 "src/sema.tr"
            TrStr lc_itn = _tr_str_retain(hir_expr_type(h_iter_lc)->name);
#line 8258 "src/sema.tr"
            long long lc_ial = hir_expr_type(h_iter_lc)->args->len;
#line 8259 "src/sema.tr"
            AstType* lc_elem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 8260 "src/sema.tr"
            if (((_tr_str_eqv((lc_itn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((lc_itn), (_tr_str_lit_len("Vec", 3LL)))) && (lc_ial > 0LL))) {
#line 8261 "src/sema.tr"
                lc_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_lc)->args, 0LL)));
            } else if (_tr_str_eqv((lc_itn), (_tr_str_lit_len("str", 3LL)))) {
#line 8263 "src/sema.tr"
                lc_elem_ty = AstType_init(_tr_str_lit_len("char", 4LL));
            }
#line 8264 "src/sema.tr"
            ({ __auto_type _ord_t1874 = (gen_ast->target); __auto_type _ord_t1875 = (SymbolKind_make_SVariable()); __auto_type _ord_t1876 = (box_asttype(lc_elem_ty)); Sema_declare(self, _ord_t1874, _ord_t1875, _ord_t1876, false); });
#line 8265 "src/sema.tr"
            List_ptr* hifs = (void*)List_ptr_new();
#line 8266 "src/sema.tr"
            long long lc_fi = 0LL;
#line 8267 "src/sema.tr"
            while ((lc_fi < gen_ast->ifs->len)) {
#line 8268 "src/sema.tr"
                List_ptr_append(hifs, Sema_lower_expr(self, ((Expr*)List_ptr_get(gen_ast->ifs, lc_fi))));
#line 8269 "src/sema.tr"
                lc_fi = (lc_fi + 1LL);
            }
#line 8270 "src/sema.tr"
            hgen_val->ifs = hifs;
#line 8271 "src/sema.tr"
            hgen_val->is_async = gen_ast->is_async;
#line 8272 "src/sema.tr"
            /* unsafe block */
#line 8272 "src/sema.tr"
            (*hgen_ptr = hgen_val);
#line 8273 "src/sema.tr"
            List_ptr_append(hgens, hgen_ptr);
#line 8274 "src/sema.tr"
            gc = (gc + 1LL);
            _tr_str_release(lc_itn);
        }
#line 8275 "src/sema.tr"
        HirExpr* h_lc_elem = Sema_lower_expr(self, element);
#line 8276 "src/sema.tr"
        AstType* lc_elem_hty = hir_expr_type(h_lc_elem);
#line 8277 "src/sema.tr"
        AstType* comp_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 8278 "src/sema.tr"
        comp_ty->args = (void*)List_ptr_new();
#line 8279 "src/sema.tr"
        ({ __auto_type _ord_t1877 = (box_asttype(lc_elem_hty)); List_ptr_append(comp_ty->args, _ord_t1877); });
#line 8280 "src/sema.tr"
        Sema_exit_scope(self);
#line 8281 "src/sema.tr"
        HirExpr* _trret302 = box_hirexpr(HirExpr_ctor_EListComp(h_lc_elem, hgens, comp_ty));
        return _trret302;
    } else if (_t1640.tag == Expr_EGeneratorExpr) {
        __auto_type element = _t1640.data.EGeneratorExpr.element;
__auto_type generators = _t1640.data.EGeneratorExpr.generators;
#line 8283 "src/sema.tr"
        Sema_enter_scope(self);
#line 8284 "src/sema.tr"
        List_ptr* hgens2 = (void*)List_ptr_new();
#line 8285 "src/sema.tr"
        long long gc2 = 0LL;
#line 8286 "src/sema.tr"
        while ((gc2 < generators->len)) {
#line 8287 "src/sema.tr"
            Comprehension* gen_ast2 = (*((Comprehension**)List_ptr_get(generators, gc2)));
#line 8288 "src/sema.tr"
            HirComprehension** hgen_ptr2 = (HirComprehension**)(0LL);
#line 8289 "src/sema.tr"
            /* unsafe block */
#line 8289 "src/sema.tr"
            hgen_ptr2 = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
#line 8290 "src/sema.tr"
            HirComprehension* hgen_val2 = ((HirComprehension*)_tr_obj_alloc(sizeof(HirComprehension)));
#line 8291 "src/sema.tr"
            hgen_val2->target = _tr_str_retain(gen_ast2->target);
#line 8292 "src/sema.tr"
            HirExpr* h_iter_ge = Sema_lower_expr(self, gen_ast2->iter);
#line 8293 "src/sema.tr"
            hgen_val2->iter = h_iter_ge;
#line 8294 "src/sema.tr"
            TrStr ge_itn = _tr_str_retain(hir_expr_type(h_iter_ge)->name);
#line 8295 "src/sema.tr"
            long long ge_ial = hir_expr_type(h_iter_ge)->args->len;
#line 8296 "src/sema.tr"
            AstType* ge_elem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 8297 "src/sema.tr"
            if (((_tr_str_eqv((ge_itn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ge_itn), (_tr_str_lit_len("Vec", 3LL)))) && (ge_ial > 0LL))) {
#line 8298 "src/sema.tr"
                ge_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_ge)->args, 0LL)));
            } else if (_tr_str_eqv((ge_itn), (_tr_str_lit_len("str", 3LL)))) {
#line 8300 "src/sema.tr"
                ge_elem_ty = AstType_init(_tr_str_lit_len("char", 4LL));
            }
#line 8301 "src/sema.tr"
            ({ __auto_type _ord_t1878 = (gen_ast2->target); __auto_type _ord_t1879 = (SymbolKind_make_SVariable()); __auto_type _ord_t1880 = (box_asttype(ge_elem_ty)); Sema_declare(self, _ord_t1878, _ord_t1879, _ord_t1880, false); });
#line 8302 "src/sema.tr"
            List_ptr* hifs2 = (void*)List_ptr_new();
#line 8303 "src/sema.tr"
            long long ge_fi = 0LL;
#line 8304 "src/sema.tr"
            while ((ge_fi < gen_ast2->ifs->len)) {
#line 8305 "src/sema.tr"
                List_ptr_append(hifs2, Sema_lower_expr(self, ((Expr*)List_ptr_get(gen_ast2->ifs, ge_fi))));
#line 8306 "src/sema.tr"
                ge_fi = (ge_fi + 1LL);
            }
#line 8307 "src/sema.tr"
            hgen_val2->ifs = hifs2;
#line 8308 "src/sema.tr"
            hgen_val2->is_async = gen_ast2->is_async;
#line 8309 "src/sema.tr"
            /* unsafe block */
#line 8309 "src/sema.tr"
            (*hgen_ptr2 = hgen_val2);
#line 8310 "src/sema.tr"
            List_ptr_append(hgens2, hgen_ptr2);
#line 8311 "src/sema.tr"
            gc2 = (gc2 + 1LL);
            _tr_str_release(ge_itn);
        }
#line 8312 "src/sema.tr"
        HirExpr* h_ge_elem = Sema_lower_expr(self, element);
#line 8313 "src/sema.tr"
        AstType* ge_elem_hty = hir_expr_type(h_ge_elem);
#line 8314 "src/sema.tr"
        AstType* gen_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 8315 "src/sema.tr"
        gen_ty->args = (void*)List_ptr_new();
#line 8316 "src/sema.tr"
        ({ __auto_type _ord_t1881 = (box_asttype(ge_elem_hty)); List_ptr_append(gen_ty->args, _ord_t1881); });
#line 8317 "src/sema.tr"
        Sema_exit_scope(self);
#line 8318 "src/sema.tr"
        HirExpr* _trret303 = box_hirexpr(HirExpr_ctor_EGeneratorExpr(h_ge_elem, hgens2, gen_ty));
        return _trret303;
    } else if (_t1640.tag == Expr_ESuperMethodCall) {
        __auto_type base_class = _t1640.data.ESuperMethodCall.base_class;
__auto_type method = _t1640.data.ESuperMethodCall.method;
__auto_type args = _t1640.data.ESuperMethodCall.args;
#line 8320 "src/sema.tr"
        List_ptr* h_super_args = (void*)List_ptr_new();
#line 8321 "src/sema.tr"
        long long k_smc = 0LL;
#line 8322 "src/sema.tr"
        while ((k_smc < args->len)) {
#line 8323 "src/sema.tr"
            List_ptr_append(h_super_args, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k_smc))));
#line 8324 "src/sema.tr"
            k_smc = (k_smc + 1LL);
        }
#line 8326 "src/sema.tr"
        TrStr resolved_base = _tr_str_retain(base_class);
#line 8327 "src/sema.tr"
        if ((_tr_str_eqv((resolved_base), (_tr_str_lit_len("", 0LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))))) {
#line 8328 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
#line 8329 "src/sema.tr"
                ClassDef* cur_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
#line 8330 "src/sema.tr"
                if ((cur_cls->base_classes->len > 0LL)) {
#line 8331 "src/sema.tr"
                    TrStr _strtmp_t1882 = List_TrStr_get(cur_cls->base_classes, 0LL);
                    _tr_str_release(resolved_base);
                    resolved_base = _strtmp_t1882;
                }
            }
        }
#line 8332 "src/sema.tr"
        AstType* super_ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 8333 "src/sema.tr"
        if (_tr_dict_contains(self->classes, _tr_strz(resolved_base))) {
#line 8334 "src/sema.tr"
            ClassDef* bc_def = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(resolved_base)));
#line 8335 "src/sema.tr"
            long long k_smc2 = 0LL;
#line 8336 "src/sema.tr"
            while ((k_smc2 < bc_def->methods->len)) {
#line 8337 "src/sema.tr"
                if (_tr_str_eqv((((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->name), (method))) {
#line 8338 "src/sema.tr"
                    if ((((unsigned long long)(((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 8339 "src/sema.tr"
                        super_ret_ty = (*((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->ret_ty);
                    }
                }
#line 8340 "src/sema.tr"
                k_smc2 = (k_smc2 + 1LL);
            }
        }
#line 8341 "src/sema.tr"
        HirExpr* _trret304 = box_hirexpr(HirExpr_ctor_ESuperMethodCall(resolved_base, method, h_super_args, super_ret_ty));
        _tr_str_release(resolved_base);
        return _trret304;
    } else if (_t1640.tag == Expr_ESuperPropAccess) {
        __auto_type base_class = _t1640.data.ESuperPropAccess.base_class;
__auto_type prop = _t1640.data.ESuperPropAccess.prop;
#line 8343 "src/sema.tr"
        TrStr resolved_base2 = _tr_str_retain(base_class);
#line 8344 "src/sema.tr"
        if ((_tr_str_eqv((resolved_base2), (_tr_str_lit_len("", 0LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))))) {
#line 8345 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
#line 8346 "src/sema.tr"
                ClassDef* cur_cls2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
#line 8347 "src/sema.tr"
                if ((cur_cls2->base_classes->len > 0LL)) {
#line 8348 "src/sema.tr"
                    TrStr _strtmp_t1883 = List_TrStr_get(cur_cls2->base_classes, 0LL);
                    _tr_str_release(resolved_base2);
                    resolved_base2 = _strtmp_t1883;
                }
            }
        }
#line 8349 "src/sema.tr"
        AstType* super_field_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 8350 "src/sema.tr"
        if (_tr_dict_contains(self->classes, _tr_strz(resolved_base2))) {
#line 8351 "src/sema.tr"
            ClassDef* bc_def2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(resolved_base2)));
#line 8352 "src/sema.tr"
            long long k_spa = 0LL;
#line 8353 "src/sema.tr"
            while ((k_spa < bc_def2->fields->len)) {
#line 8354 "src/sema.tr"
                if (_tr_str_eqv((((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->name), (prop))) {
#line 8355 "src/sema.tr"
                    if ((((unsigned long long)(((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->ty)) != ((unsigned long long)(0LL)))) {
#line 8356 "src/sema.tr"
                        super_field_ty = (*((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->ty);
                    }
                }
#line 8357 "src/sema.tr"
                k_spa = (k_spa + 1LL);
            }
        }
#line 8358 "src/sema.tr"
        HirExpr* _trret305 = box_hirexpr(HirExpr_ctor_ESuperPropAccess(resolved_base2, prop, super_field_ty));
        _tr_str_release(resolved_base2);
        return _trret305;
    } else if (1) {
        __auto_type _ = _t1640;
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init(_tr_str_lit_len("None", 4LL))));
    }
}

__attribute__((hot)) TrStr Sema_is_reserved_error(Sema* self, TrStr name) {
#line 8363 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("true", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("True", 4LL))))) {
#line 8363 "src/sema.tr"
        return _tr_str_lit_len("built-in boolean constant 'true'", 32LL);
    }
#line 8364 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("false", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("False", 5LL))))) {
#line 8364 "src/sema.tr"
        return _tr_str_lit_len("built-in boolean constant 'false'", 33LL);
    }
#line 8365 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("none", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("None", 4LL))))) {
#line 8365 "src/sema.tr"
        return _tr_str_lit_len("built-in null constant 'none'", 29LL);
    }
#line 8366 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("Some", 4LL)))) {
#line 8366 "src/sema.tr"
        return _tr_str_lit_len("built-in Option constructor 'Some'", 34LL);
    }
#line 8367 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("Ok", 2LL)))) {
#line 8367 "src/sema.tr"
        return _tr_str_lit_len("built-in Result constructor 'Ok'", 32LL);
    }
#line 8368 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("Err", 3LL)))) {
#line 8368 "src/sema.tr"
        return _tr_str_lit_len("built-in Result constructor 'Err'", 33LL);
    }
#line 8369 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Sema_is_reserved_keyword(Sema* self, TrStr name) {
#line 8376 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("print", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("input", 5LL))))) {
#line 8376 "src/sema.tr"
        return _tr_str_lit_len("built-in I/O function", 21LL);
    }
#line 8377 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("range", 5LL)))) {
#line 8377 "src/sema.tr"
        return _tr_str_lit_len("built-in range function", 23LL);
    }
#line 8378 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("len", 3LL)))) {
#line 8378 "src/sema.tr"
        return _tr_str_lit_len("built-in len function", 21LL);
    }
#line 8379 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("type", 4LL)))) {
#line 8379 "src/sema.tr"
        return _tr_str_lit_len("built-in type function", 22LL);
    }
#line 8380 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("sum", 3LL))))) {
#line 8380 "src/sema.tr"
        return _tr_str_lit_len("built-in math function", 22LL);
    }
#line 8381 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("int", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL))))) {
#line 8381 "src/sema.tr"
        return _tr_str_lit_len("built-in primitive type", 23LL);
    }
#line 8382 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Dict", 4LL))))) {
#line 8382 "src/sema.tr"
        return _tr_str_lit_len("built-in container type", 23LL);
    }
#line 8383 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("Option", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Result", 6LL))))) {
#line 8383 "src/sema.tr"
        return _tr_str_lit_len("built-in enum type", 18LL);
    }
#line 8384 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("Exception", 9LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Error", 5LL))))) {
#line 8384 "src/sema.tr"
        return _tr_str_lit_len("built-in exception type", 23LL);
    }
#line 8387 "src/sema.tr"
    if ((((((((((_tr_str_eqv((name), (_tr_str_lit_len("_Complex", 8LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_Imaginary", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Bool", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Atomic", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Generic", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Noreturn", 9LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Alignas", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Alignof", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Static_assert", 14LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Thread_local", 13LL))))) {
#line 8387 "src/sema.tr"
        return _tr_str_lit_len("C99/C11 keyword", 15LL);
    }
#line 8388 "src/sema.tr"
    if ((((((((_tr_str_eqv((name), (_tr_str_lit_len("DIR", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("FILE", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("EOF", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("NULL", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("errno", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("stdin", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("stdout", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("stderr", 6LL))))) {
#line 8388 "src/sema.tr"
        return _tr_str_lit_len("C library identifier", 20LL);
    }
#line 8389 "src/sema.tr"
    if ((((((((((((((_tr_str_eqv((name), (_tr_str_lit_len("ERROR", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("DELETE", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("IN", 2LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("OUT", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("OPTIONAL", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("NEAR", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("FAR", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("VOID", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("CONST", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("TRUE", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("FALSE", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("INFINITE", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("SEVERITY_ERROR", 14LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("SEVERITY_SUCCESS", 16LL))))) {
#line 8389 "src/sema.tr"
        return _tr_str_lit_len("Windows SDK macro", 17LL);
    }
#line 8390 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool Sema_block_returns(Sema* self, Block* b) {
#line 8394 "src/sema.tr"
    if ((b->stmts->len == 0LL)) {
#line 8394 "src/sema.tr"
        return false;
    }
#line 8397 "src/sema.tr"
    long long last_idx = (b->stmts->len - 1LL);
#line 8398 "src/sema.tr"
    bool sl_going = true;
#line 8399 "src/sema.tr"
    while ((last_idx > 0LL)) {
#line 8400 "src/sema.tr"
        if ((!sl_going)) {
#line 8400 "src/sema.tr"
            break;
        }
#line 8401 "src/sema.tr"
        Stmt* sl_s = ((Stmt*)List_ptr_get(b->stmts, last_idx));
#line 8402 "src/sema.tr"
        if ((((unsigned long long)(sl_s)) == ((unsigned long long)(0LL)))) {
#line 8403 "src/sema.tr"
            last_idx = (last_idx - 1LL);
        } else {
#line 8405 "src/sema.tr"
            bool is_sline = false;
#line 8406 "src/sema.tr"
            __auto_type _t1884 = (*sl_s);
            if (_t1884.tag == Stmt_SLine) {
                __auto_type _ = _t1884.data.SLine.n;
                is_sline = true;
            } else if (1) {
                __auto_type _ = _t1884;
                /* pass */
            }
#line 8409 "src/sema.tr"
            if (is_sline) {
#line 8410 "src/sema.tr"
                last_idx = (last_idx - 1LL);
            } else {
#line 8412 "src/sema.tr"
                sl_going = false;
            }
        }
    }
#line 8413 "src/sema.tr"
    Stmt* last_s = ((Stmt*)List_ptr_get(b->stmts, last_idx));
#line 8414 "src/sema.tr"
    if ((((unsigned long long)(last_s)) == ((unsigned long long)(0LL)))) {
#line 8414 "src/sema.tr"
        return false;
    }
#line 8415 "src/sema.tr"
    __auto_type _t1885 = (*last_s);
    if (_t1885.tag == Stmt_SReturn) {
        __auto_type _ = _t1885.data.SReturn.val;
        return true;
    } else if (_t1885.tag == Stmt_SRaise) {
        __auto_type _ = _t1885.data.SRaise.val;
        return true;
    } else if (_t1885.tag == Stmt_SUnsafe) {
        __auto_type body = _t1885.data.SUnsafe.body;
        bool _trret306 = Sema_block_returns(self, body);
        return _trret306;
    } else if (_t1885.tag == Stmt_SIf) {
        __auto_type cond = _t1885.data.SIf.cond;
__auto_type then_b = _t1885.data.SIf.then_b;
__auto_type elifs = _t1885.data.SIf.elifs;
__auto_type else_b = _t1885.data.SIf.else_b;
#line 8420 "src/sema.tr"
        if ((else_b->stmts->len == 0LL)) {
#line 8420 "src/sema.tr"
            return false;
        }
#line 8421 "src/sema.tr"
        bool _trret307 = (Sema_block_returns(self, then_b) && Sema_block_returns(self, else_b));
        return _trret307;
    } else if (_t1885.tag == Stmt_SMatch) {
        __auto_type subj = _t1885.data.SMatch.expr;
__auto_type arms = _t1885.data.SMatch.arms;
#line 8428 "src/sema.tr"
        bool has_wild = false;
#line 8429 "src/sema.tr"
        bool all_ret = true;
#line 8430 "src/sema.tr"
        List_TrStr* _fr_tnames = (void*)List_TrStr_new();
#line 8431 "src/sema.tr"
        List_TrStr* _fr_vnames = (void*)List_TrStr_new();
#line 8432 "src/sema.tr"
        long long mi = 0LL;
#line 8433 "src/sema.tr"
        while ((mi < arms->len)) {
#line 8434 "src/sema.tr"
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, mi));
#line 8435 "src/sema.tr"
            __auto_type _t1886 = arm->pat;
            if (_t1886.tag == Pattern_PWild) {
                has_wild = true;
            } else if (_t1886.tag == Pattern_PBind) {
                __auto_type _ = _t1886.data.PBind.name;
                has_wild = true;
            } else if (1) {
                __auto_type _ = _t1886;
                Sema_pattern_covers(self, arm->pat, _fr_tnames, _fr_vnames);
            }
#line 8439 "src/sema.tr"
            if ((((unsigned long long)(arm->body)) != ((unsigned long long)(0LL)))) {
#line 8440 "src/sema.tr"
                if ((!Sema_block_returns(self, (*arm->body)))) {
#line 8441 "src/sema.tr"
                    all_ret = false;
                }
            }
#line 8442 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 8443 "src/sema.tr"
        if (has_wild) {
#line 8443 "src/sema.tr"
            bool _trret308 = all_ret;
            return _trret308;
        }
#line 8444 "src/sema.tr"
        if ((!all_ret)) {
#line 8444 "src/sema.tr"
            return false;
        }
#line 8445 "src/sema.tr"
        if ((_fr_vnames->len == 0LL)) {
#line 8445 "src/sema.tr"
            return false;
        }
#line 8446 "src/sema.tr"
        TrStr _fr_ety = List_TrStr_get(_fr_tnames, 0LL);
#line 8447 "src/sema.tr"
        bool _fr_same_ty = true;
#line 8448 "src/sema.tr"
        long long _fr_ti = 1LL;
#line 8449 "src/sema.tr"
        while ((_fr_ti < _fr_tnames->len)) {
#line 8450 "src/sema.tr"
            if (({ TrStr _cmpa_t1887 = (List_TrStr_get(_fr_tnames, _fr_ti)); TrStr _cmpb_t1888 = (_fr_ety); _Bool _cmpr_t1889 = !_tr_str_eqv(_cmpa_t1887, _cmpb_t1888); _tr_str_release(_cmpa_t1887); _cmpr_t1889; })) {
#line 8450 "src/sema.tr"
                _fr_same_ty = false;
            }
#line 8451 "src/sema.tr"
            _fr_ti = (_fr_ti + 1LL);
        }
#line 8452 "src/sema.tr"
        if ((!_fr_same_ty)) {
#line 8452 "src/sema.tr"
            _tr_str_release(_fr_ety);
            return false;
        }
#line 8453 "src/sema.tr"
        if ((!_tr_dict_contains(self->enums, _tr_strz(_fr_ety)))) {
#line 8453 "src/sema.tr"
            _tr_str_release(_fr_ety);
            return false;
        }
#line 8454 "src/sema.tr"
        EnumDef* _fr_edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(_fr_ety)));
#line 8455 "src/sema.tr"
        long long _fr_ei = 0LL;
#line 8456 "src/sema.tr"
        while ((_fr_ei < _fr_edef->variants->len)) {
#line 8457 "src/sema.tr"
            TrStr _fr_vn = _tr_str_retain(((VariantDef*)List_ptr_get(_fr_edef->variants, _fr_ei))->name);
#line 8458 "src/sema.tr"
            bool _fr_found = false;
#line 8459 "src/sema.tr"
            long long _fr_vi = 0LL;
#line 8460 "src/sema.tr"
            while ((_fr_vi < _fr_vnames->len)) {
#line 8461 "src/sema.tr"
                if (({ TrStr _cmpa_t1890 = (List_TrStr_get(_fr_vnames, _fr_vi)); TrStr _cmpb_t1891 = (_fr_vn); _Bool _cmpr_t1892 = _tr_str_eqv(_cmpa_t1890, _cmpb_t1891); _tr_str_release(_cmpa_t1890); _cmpr_t1892; })) {
#line 8461 "src/sema.tr"
                    _fr_found = true;
                }
#line 8462 "src/sema.tr"
                _fr_vi = (_fr_vi + 1LL);
            }
#line 8463 "src/sema.tr"
            if ((!_fr_found)) {
#line 8463 "src/sema.tr"
                _tr_str_release(_fr_ety);
                _tr_str_release(_fr_vn);
                return false;
            }
#line 8464 "src/sema.tr"
            _fr_ei = (_fr_ei + 1LL);
            _tr_str_release(_fr_vn);
        }
#line 8465 "src/sema.tr"
        _tr_str_release(_fr_ety);
        return true;
    } else if (1) {
        __auto_type _ = _t1885;
        return false;
    }
}

__attribute__((hot)) void Sema_pattern_covers(Sema* self, Pattern p, List_TrStr* tnames, List_TrStr* vnames) {
#line 8474 "src/sema.tr"
    __auto_type _t1893 = p;
    if (_t1893.tag == Pattern_PVariant) {
        __auto_type tn = _t1893.data.PVariant.type_name;
__auto_type vn = _t1893.data.PVariant.variant;
#line 8476 "src/sema.tr"
        List_TrStr_append(tnames, tn);
#line 8477 "src/sema.tr"
        List_TrStr_append(vnames, vn);
    } else if (_t1893.tag == Pattern_PVariantBind) {
        __auto_type tn = _t1893.data.PVariantBind.type_name;
__auto_type vn = _t1893.data.PVariantBind.variant;
#line 8479 "src/sema.tr"
        List_TrStr_append(tnames, tn);
#line 8480 "src/sema.tr"
        List_TrStr_append(vnames, vn);
    } else if (_t1893.tag == Pattern_PVariantBindMany) {
        __auto_type tn = _t1893.data.PVariantBindMany.type_name;
__auto_type vn = _t1893.data.PVariantBindMany.variant;
#line 8482 "src/sema.tr"
        List_TrStr_append(tnames, tn);
#line 8483 "src/sema.tr"
        List_TrStr_append(vnames, vn);
    } else if (_t1893.tag == Pattern_POr) {
        __auto_type pats = _t1893.data.POr.patterns;
#line 8485 "src/sema.tr"
        long long oi = 0LL;
#line 8486 "src/sema.tr"
        while ((oi < pats->len)) {
#line 8487 "src/sema.tr"
            Sema_pattern_covers(self, List_Pattern_get(pats, oi), tnames, vnames);
#line 8488 "src/sema.tr"
            oi = (oi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1893;
        /* pass */
    }
}

__attribute__((hot)) bool Sema_is_primitive(Sema* self, AstType* ty) {
#line 8491 "src/sema.tr"
    return Sema_is_primitive_name(self, ty->name);
}

__attribute__((hot)) bool Sema__is_known_type_name(Sema* self, TrStr name) {
#line 8498 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) {
#line 8498 "src/sema.tr"
        return true;
    }
#line 8499 "src/sema.tr"
    if (Sema_is_primitive_name(self, name)) {
#line 8499 "src/sema.tr"
        return true;
    }
#line 8500 "src/sema.tr"
    if ((((((_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Str", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("ptr", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("object", 6LL))))) {
#line 8500 "src/sema.tr"
        return true;
    }
#line 8501 "src/sema.tr"
    if ((_tr_strlen(_tr_strz(name)) == 1LL)) {
#line 8501 "src/sema.tr"
        return true;
    }
#line 8502 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
#line 8502 "src/sema.tr"
        return true;
    }
#line 8503 "src/sema.tr"
    if (_tr_dict_contains(self->enums, _tr_strz(name))) {
#line 8503 "src/sema.tr"
        return true;
    }
#line 8504 "src/sema.tr"
    if (((((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))) && _tr_dict_contains(self->interfaces, _tr_strz(name)))) {
#line 8504 "src/sema.tr"
        return true;
    }
#line 8505 "src/sema.tr"
    if (((((((((_tr_str_eqv((name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Set", 3LL)))) || (_tr_str_eqv((name), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((name), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("tuple", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 8505 "src/sema.tr"
        return true;
    }
#line 8506 "src/sema.tr"
    if (((((((((_tr_str_eqv((name), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Channel", 7LL))))) {
#line 8506 "src/sema.tr"
        return true;
    }
#line 8507 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Coro", 4LL))))) {
#line 8507 "src/sema.tr"
        return true;
    }
#line 8508 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("AsyncPool", 9LL))) || _tr_str_eqv((name), (_tr_str_lit_len("AsyncTask", 9LL))))) {
#line 8508 "src/sema.tr"
        return true;
    }
#line 8514 "src/sema.tr"
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("OS", 2LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Process", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Env", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Hash", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("File", 4LL))))) {
#line 8514 "src/sema.tr"
        return true;
    }
#line 8515 "src/sema.tr"
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("Math", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Clock", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Time", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Random", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Hmac", 4LL))))) {
#line 8515 "src/sema.tr"
        return true;
    }
#line 8516 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("Json", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("JsonDoc", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("JsonWriter", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("JsonReader", 10LL))))) {
#line 8516 "src/sema.tr"
        return true;
    }
#line 8517 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_primitive_name(Sema* self, TrStr name) {
#line 8520 "src/sema.tr"
    if ((((((_tr_str_eqv((name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("None", 4LL))))) {
#line 8520 "src/sema.tr"
        return true;
    }
#line 8521 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("i64", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("i8", 2LL))))) {
#line 8521 "src/sema.tr"
        return true;
    }
#line 8522 "src/sema.tr"
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("u64", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("usize", 5LL))))) {
#line 8522 "src/sema.tr"
        return true;
    }
#line 8523 "src/sema.tr"
    if (((_tr_str_eqv((name), (_tr_str_lit_len("f64", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("lambda", 6LL))))) {
#line 8523 "src/sema.tr"
        return true;
    }
#line 8526 "src/sema.tr"
    if (((_tr_str_starts_withv((name), (_tr_str_lit_len("c_", 2LL))) && (!_tr_str_eqv((name), (_tr_str_lit_len("c_void_ptr", 10LL))))) && (!_tr_str_eqv((name), (_tr_str_lit_len("c_FILE", 6LL)))))) {
#line 8526 "src/sema.tr"
        return true;
    }
#line 8530 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Str", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Bytes", 5LL))))) {
#line 8530 "src/sema.tr"
        return true;
    }
#line 8531 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Channel", 7LL))))) {
#line 8531 "src/sema.tr"
        return true;
    }
#line 8532 "src/sema.tr"
    if (((_tr_str_eqv((name), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Atomic", 6LL))))) {
#line 8532 "src/sema.tr"
        return true;
    }
#line 8533 "src/sema.tr"
    if (((_tr_str_eqv((name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
#line 8533 "src/sema.tr"
        return true;
    }
#line 8534 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("AsyncPool", 9LL))) || _tr_str_eqv((name), (_tr_str_lit_len("AsyncTask", 9LL))))) {
#line 8534 "src/sema.tr"
        return true;
    }
#line 8535 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 8535 "src/sema.tr"
        return true;
    }
#line 8537 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("ref", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("mut_ref", 7LL))))) {
#line 8537 "src/sema.tr"
        return true;
    }
#line 8538 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool _is_builtin_type_word(TrStr n) {
#line 21 "src/sema.tr"
    if (((_tr_str_lenv((n)) > 2LL) && ({ TrStr _cmpa_t1894 = (_tr_str_slicev((n), 0LL, 2LL)); TrStr _cmpb_t1895 = (_tr_str_lit_len("c_", 2LL)); _Bool _cmpr_t1896 = _tr_str_eqv(_cmpa_t1894, _cmpb_t1895); _tr_str_release(_cmpa_t1894); _cmpr_t1896; }))) {
#line 21 "src/sema.tr"
        return true;
    }
#line 22 "src/sema.tr"
    if (((((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("isize", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))))) {
#line 22 "src/sema.tr"
        return true;
    }
#line 23 "src/sema.tr"
    if (((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("tuple", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Array", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ref", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("mut_ref", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("def", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("lambda", 6LL))))) {
#line 23 "src/sema.tr"
        return true;
    }
#line 24 "src/sema.tr"
    if (((((((((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Bytes", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("String", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Any", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("any", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("object", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Future", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Task", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncTask", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL))))) {
#line 24 "src/sema.tr"
        return true;
    }
#line 25 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool _expr_is_self_field(Expr* e) {
#line 97 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 97 "src/sema.tr"
        return false;
    }
#line 98 "src/sema.tr"
    __auto_type _t1897 = (*e);
    if (_t1897.tag == Expr_EPropAccess) {
        __auto_type obj = _t1897.data.EPropAccess.obj;
#line 100 "src/sema.tr"
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
#line 100 "src/sema.tr"
            return false;
        }
#line 101 "src/sema.tr"
        __auto_type _t1898 = (*obj);
        if (_t1898.tag == Expr_EIdent) {
            __auto_type nm = _t1898.data.EIdent.name;
            bool _trret164 = _tr_str_eqv((nm), (_tr_str_lit_len("self", 4LL)));
            return _trret164;
        } else if (1) {
            __auto_type _ = _t1898;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t1897;
        return false;
    }
}

__attribute__((hot)) bool _binop_is_float_name(TrStr n) {
#line 107 "src/sema.tr"
    return ((_tr_str_eqv((n), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL))));
}

__attribute__((hot)) bool _hl_has_float(List_ptr* hl) {
#line 111 "src/sema.tr"
    long long i = 0LL;
#line 112 "src/sema.tr"
    while ((i < hl->len)) {
#line 113 "src/sema.tr"
        if (_binop_is_float_name(hir_expr_type(((HirExpr*)List_ptr_get(hl, i)))->name)) {
#line 113 "src/sema.tr"
            return true;
        }
#line 114 "src/sema.tr"
        i = (i + 1LL);
    }
#line 115 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool _block_mutates_self(Block* b) {
#line 118 "src/sema.tr"
    long long i = 0LL;
#line 119 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 120 "src/sema.tr"
        if (_stmt_mutates_self(((Stmt*)List_ptr_get(b->stmts, i)))) {
#line 120 "src/sema.tr"
            return true;
        }
#line 121 "src/sema.tr"
        i = (i + 1LL);
    }
#line 122 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool _pblock_mutates_self(Block** pb) {
#line 125 "src/sema.tr"
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
#line 125 "src/sema.tr"
        return false;
    }
#line 126 "src/sema.tr"
    return _block_mutates_self((*pb));
}

__attribute__((hot)) bool _stmt_mutates_self(Stmt* s) {
#line 129 "src/sema.tr"
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
#line 129 "src/sema.tr"
        return false;
    }
#line 130 "src/sema.tr"
    __auto_type _t1899 = (*s);
    if (_t1899.tag == Stmt_SAssign) {
        __auto_type tgt = _t1899.data.SAssign.target;
        return _expr_is_self_field(tgt);
    } else if (_t1899.tag == Stmt_SIf) {
        __auto_type then_b = _t1899.data.SIf.then_b;
__auto_type elifs = _t1899.data.SIf.elifs;
__auto_type else_b = _t1899.data.SIf.else_b;
#line 133 "src/sema.tr"
        if (_block_mutates_self(then_b)) {
#line 133 "src/sema.tr"
            return true;
        }
#line 134 "src/sema.tr"
        long long i = 0LL;
#line 135 "src/sema.tr"
        while ((i < elifs->len)) {
#line 136 "src/sema.tr"
            if (_pblock_mutates_self(((ElifClause*)List_ptr_get(elifs, i))->body)) {
#line 136 "src/sema.tr"
                return true;
            }
#line 137 "src/sema.tr"
            i = (i + 1LL);
        }
#line 138 "src/sema.tr"
        bool _trret165 = _block_mutates_self(else_b);
        return _trret165;
    } else if (_t1899.tag == Stmt_SWhile) {
        __auto_type body = _t1899.data.SWhile.body;
        bool _trret166 = _block_mutates_self(body);
        return _trret166;
    } else if (_t1899.tag == Stmt_SFor) {
        __auto_type body = _t1899.data.SFor.body;
        bool _trret167 = _block_mutates_self(body);
        return _trret167;
    } else if (_t1899.tag == Stmt_SForUnpack) {
        __auto_type body = _t1899.data.SForUnpack.body;
        bool _trret168 = _block_mutates_self(body);
        return _trret168;
    } else if (_t1899.tag == Stmt_SUnsafe) {
        __auto_type body = _t1899.data.SUnsafe.body;
        bool _trret169 = _block_mutates_self(body);
        return _trret169;
    } else if (_t1899.tag == Stmt_SWith) {
        __auto_type body = _t1899.data.SWith.body;
        bool _trret170 = _block_mutates_self(body);
        return _trret170;
    } else if (_t1899.tag == Stmt_STaskGroup) {
        __auto_type body = _t1899.data.STaskGroup.body;
        bool _trret171 = _block_mutates_self(body);
        return _trret171;
    } else if (_t1899.tag == Stmt_SGpuBlock) {
        __auto_type body = _t1899.data.SGpuBlock.body;
        bool _trret172 = _block_mutates_self(body);
        return _trret172;
    } else if (_t1899.tag == Stmt_SMatch) {
        __auto_type arms = _t1899.data.SMatch.arms;
#line 147 "src/sema.tr"
        long long i = 0LL;
#line 148 "src/sema.tr"
        while ((i < arms->len)) {
#line 149 "src/sema.tr"
            if (_pblock_mutates_self(((MatchArm*)List_ptr_get(arms, i))->body)) {
#line 149 "src/sema.tr"
                return true;
            }
#line 150 "src/sema.tr"
            i = (i + 1LL);
        }
#line 151 "src/sema.tr"
        return false;
    } else if (_t1899.tag == Stmt_STry) {
        __auto_type try_body = _t1899.data.STry.try_body;
__auto_type catches = _t1899.data.STry.catches;
__auto_type finally_b = _t1899.data.STry.finally_b;
#line 153 "src/sema.tr"
        if (_block_mutates_self(try_body)) {
#line 153 "src/sema.tr"
            return true;
        }
#line 154 "src/sema.tr"
        long long i = 0LL;
#line 155 "src/sema.tr"
        while ((i < catches->len)) {
#line 156 "src/sema.tr"
            if (_pblock_mutates_self((*((CatchClause**)List_ptr_get(catches, i)))->body)) {
#line 156 "src/sema.tr"
                return true;
            }
#line 157 "src/sema.tr"
            i = (i + 1LL);
        }
#line 158 "src/sema.tr"
        bool _trret173 = _block_mutates_self(finally_b);
        return _trret173;
    } else if (_t1899.tag == Stmt_SDefer) {
        __auto_type inner = _t1899.data.SDefer.stmt;
        return _stmt_mutates_self(inner);
    } else if (1) {
        __auto_type _ = _t1899;
        return false;
    }
}

__attribute__((hot)) bool _rhs_is_alias(Expr* e) {
#line 169 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 169 "src/sema.tr"
        return false;
    }
#line 170 "src/sema.tr"
    __auto_type _t1900 = (*e);
    if (_t1900.tag == Expr_EIdent) {
        __auto_type _ = _t1900.data.EIdent.name;
        return true;
    } else if (_t1900.tag == Expr_EPropAccess) {
        return true;
    } else if (_t1900.tag == Expr_EIndex) {
        return true;
    } else if (_t1900.tag == Expr_EMethodCall) {
        __auto_type m = _t1900.data.EMethodCall.method;
        bool _trret174 = ((_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("last", 4LL))));
        return _trret174;
    } else if (1) {
        __auto_type _ = _t1900;
        return false;
    }
}

__attribute__((hot)) void _block_alias_assigns(Block* b, TrMap* out) {
#line 178 "src/sema.tr"
    long long i = 0LL;
#line 179 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 180 "src/sema.tr"
        _stmt_alias_assigns(((Stmt*)List_ptr_get(b->stmts, i)), out);
#line 181 "src/sema.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void _pblock_alias_assigns(Block** pb, TrMap* out) {
#line 184 "src/sema.tr"
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
#line 184 "src/sema.tr"
        return;
    }
#line 185 "src/sema.tr"
    _block_alias_assigns((*pb), out);
}

__attribute__((hot)) void _stmt_alias_assigns(Stmt* s, TrMap* out) {
#line 188 "src/sema.tr"
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
#line 188 "src/sema.tr"
        return;
    }
#line 189 "src/sema.tr"
    __auto_type _t1901 = (*s);
    if (_t1901.tag == Stmt_SAssign) {
        __auto_type tgt = _t1901.data.SAssign.target;
__auto_type val = _t1901.data.SAssign.val;
#line 191 "src/sema.tr"
        if ((((unsigned long long)(tgt)) != ((unsigned long long)(0LL)))) {
#line 192 "src/sema.tr"
            __auto_type _t1902 = (*tgt);
            if (_t1902.tag == Expr_EIdent) {
                __auto_type tn = _t1902.data.EIdent.name;
#line 194 "src/sema.tr"
                if (_rhs_is_alias(val)) {
#line 194 "src/sema.tr"
                    _tr_dict_set(out, _tr_strz(tn), true);
                }
            } else if (1) {
                __auto_type _ = _t1902;
                /* pass */
            }
        }
    } else if (_t1901.tag == Stmt_SIf) {
        __auto_type then_b = _t1901.data.SIf.then_b;
__auto_type elifs = _t1901.data.SIf.elifs;
__auto_type else_b = _t1901.data.SIf.else_b;
#line 197 "src/sema.tr"
        _block_alias_assigns(then_b, out);
#line 198 "src/sema.tr"
        long long i = 0LL;
#line 199 "src/sema.tr"
        while ((i < elifs->len)) {
#line 200 "src/sema.tr"
            _pblock_alias_assigns(((ElifClause*)List_ptr_get(elifs, i))->body, out);
#line 201 "src/sema.tr"
            i = (i + 1LL);
        }
#line 202 "src/sema.tr"
        _block_alias_assigns(else_b, out);
    } else if (_t1901.tag == Stmt_SWhile) {
        __auto_type body = _t1901.data.SWhile.body;
        _block_alias_assigns(body, out);
    } else if (_t1901.tag == Stmt_SFor) {
        __auto_type body = _t1901.data.SFor.body;
        _block_alias_assigns(body, out);
    } else if (_t1901.tag == Stmt_SForUnpack) {
        __auto_type body = _t1901.data.SForUnpack.body;
        _block_alias_assigns(body, out);
    } else if (_t1901.tag == Stmt_SUnsafe) {
        __auto_type body = _t1901.data.SUnsafe.body;
        _block_alias_assigns(body, out);
    } else if (_t1901.tag == Stmt_SWith) {
        __auto_type body = _t1901.data.SWith.body;
        _block_alias_assigns(body, out);
    } else if (_t1901.tag == Stmt_STaskGroup) {
        __auto_type body = _t1901.data.STaskGroup.body;
        _block_alias_assigns(body, out);
    } else if (_t1901.tag == Stmt_SGpuBlock) {
        __auto_type body = _t1901.data.SGpuBlock.body;
        _block_alias_assigns(body, out);
    } else if (_t1901.tag == Stmt_SMatch) {
        __auto_type arms = _t1901.data.SMatch.arms;
#line 211 "src/sema.tr"
        long long i = 0LL;
#line 212 "src/sema.tr"
        while ((i < arms->len)) {
#line 213 "src/sema.tr"
            _pblock_alias_assigns(((MatchArm*)List_ptr_get(arms, i))->body, out);
#line 214 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1901.tag == Stmt_STry) {
        __auto_type try_body = _t1901.data.STry.try_body;
__auto_type catches = _t1901.data.STry.catches;
__auto_type finally_b = _t1901.data.STry.finally_b;
#line 216 "src/sema.tr"
        _block_alias_assigns(try_body, out);
#line 217 "src/sema.tr"
        long long i = 0LL;
#line 218 "src/sema.tr"
        while ((i < catches->len)) {
#line 219 "src/sema.tr"
            _pblock_alias_assigns((*((CatchClause**)List_ptr_get(catches, i)))->body, out);
#line 220 "src/sema.tr"
            i = (i + 1LL);
        }
#line 221 "src/sema.tr"
        _block_alias_assigns(finally_b, out);
    } else if (_t1901.tag == Stmt_SDefer) {
        __auto_type inner = _t1901.data.SDefer.stmt;
        _stmt_alias_assigns(inner, out);
    } else if (1) {
        __auto_type _ = _t1901;
        /* pass */
    }
}

__attribute__((hot)) Stmt* box_astestmt(Stmt t) {
#line 8550 "src/sema.tr"
    /* unsafe block */
#line 8551 "src/sema.tr"
    Stmt* p = ((Stmt*)_tr_c_calloc((size_t)(1LL), sizeof(Stmt)));
#line 8552 "src/sema.tr"
    (*p = t);
#line 8553 "src/sema.tr"
    return p;
}

__attribute__((hot)) Symbol** box_symbol(Symbol* s) {
#line 8562 "src/sema.tr"
    /* unsafe block */
#line 8563 "src/sema.tr"
    Symbol** p = ((Symbol**)_tr_c_calloc((size_t)(1LL), sizeof(Symbol*)));
#line 8564 "src/sema.tr"
    (*p = s);
#line 8565 "src/sema.tr"
    return p;
}

__attribute__((hot)) bool _str_method_known(TrStr m) {
#line 8570 "src/sema.tr"
    return _tr_str_containsv((_tr_str_lit_len(" as_ptr as_str capitalize ceil center char_at chars contains contains_char count ends_with eq find floor format free get_index index_of is_alnum is_alpha is_digit is_empty is_lower is_space is_upper join last_index_of rfind __len__ len length lines lower lpad offset pad_left pad_right parse_bool parse_float parse_int pow read remove_char repeat replace replace_first reverse rpad slice split split_exact split_once split_to_vec starts_with strip strip_prefix strip_suffix title to_float to_int to_lower to_str to_string to_upper trim trim_left trim_right upper words write zfill clone hash bytes ", 598LL)), (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len(" ", 1LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })));
}

