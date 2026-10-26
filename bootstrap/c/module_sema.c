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
#line 53 "src/sema.tr"
    Symbol* s = ((Symbol*)_tr_obj_alloc(sizeof(Symbol)));
#line 54 "src/sema.tr"
    s->name = _tr_str_retain(name);
#line 55 "src/sema.tr"
    s->kind = kind;
#line 56 "src/sema.tr"
    s->ty = ty;
#line 57 "src/sema.tr"
    s->scope_depth = 0LL;
#line 58 "src/sema.tr"
    s->is_mut = false;
#line 59 "src/sema.tr"
    s->is_const = false;
#line 60 "src/sema.tr"
    s->is_shared = false;
#line 61 "src/sema.tr"
    s->is_moved = false;
#line 62 "src/sema.tr"
    s->active_borrows = 0LL;
#line 63 "src/sema.tr"
    s->borrowed_by = (void*)List_TrStr_new();
#line 64 "src/sema.tr"
    s->is_init = true;
#line 65 "src/sema.tr"
    s->is_param = false;
#line 66 "src/sema.tr"
    s->is_maybe_moved = false;
#line 67 "src/sema.tr"
    s->is_maybe_init = false;
#line 68 "src/sema.tr"
    s->ptr_region = 0LL;
#line 69 "src/sema.tr"
    s->is_freed = false;
#line 70 "src/sema.tr"
    s->decl_block_depth = 0LL;
#line 71 "src/sema.tr"
    s->decl_block_id = 0LL;
#line 72 "src/sema.tr"
    s->str_escaped = false;
#line 73 "src/sema.tr"
    s->coll_escaped = false;
#line 74 "src/sema.tr"
    s->borrows_region = _tr_str_lit_len("", 0LL);
#line 75 "src/sema.tr"
    s->firm = _tr_str_lit_len("", 0LL);
#line 76 "src/sema.tr"
    return s;
}

__attribute__((malloc,returns_nonnull,hot)) Scope* Scope_init() {
#line 84 "src/sema.tr"
    Scope* s = ((Scope*)_tr_obj_alloc(sizeof(Scope)));
#line 85 "src/sema.tr"
    s->variables = _tr_dict_new(32LL);
#line 86 "src/sema.tr"
    s->decl_order = (void*)List_TrStr_new();
#line 87 "src/sema.tr"
    return s;
}

__attribute__((hot)) AstType** Sema_build_ast_type(Sema* self, Expr* e) {
#line 315 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 315 "src/sema.tr"
        return (AstType**)(0LL);
    }
#line 316 "src/sema.tr"
    __auto_type _t686 = (*e);
    if (_t686.tag == Expr_EIdent) {
        __auto_type n = _t686.data.EIdent.name;
#line 318 "src/sema.tr"
        AstType** _trret175 = box_asttype(AstType_init(n));
        return _trret175;
    } else if (_t686.tag == Expr_EIndex) {
        __auto_type obj = _t686.data.EIndex.obj;
__auto_type idx = _t686.data.EIndex._tr_v_index;
#line 320 "src/sema.tr"
        __auto_type _t687 = (*obj);
        if (_t687.tag == Expr_EIdent) {
            __auto_type on = _t687.data.EIdent.name;
#line 322 "src/sema.tr"
            AstType* at = AstType_init(on);
#line 323 "src/sema.tr"
            at->args = (void*)List_ptr_new();
#line 327 "src/sema.tr"
            __auto_type _t688 = (*idx);
            if (_t688.tag == Expr_ETuple) {
                __auto_type _bte = _t688.data.ETuple.items;
#line 329 "src/sema.tr"
                long long _bti = 0LL;
#line 330 "src/sema.tr"
                while ((_bti < _bte->len)) {
#line 331 "src/sema.tr"
                    ({ __auto_type _ord_t689 = (Sema_build_ast_type(self, ((Expr*)List_ptr_get(_bte, _bti)))); List_ptr_append(at->args, _ord_t689); });
#line 332 "src/sema.tr"
                    _bti = (_bti + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t688;
#line 334 "src/sema.tr"
                ({ __auto_type _ord_t690 = (Sema_build_ast_type(self, idx)); List_ptr_append(at->args, _ord_t690); });
            }
#line 335 "src/sema.tr"
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
#line 338 "src/sema.tr"
    return box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
}

__attribute__((hot)) AstType** Sema__targ_of(Sema* self, Expr* e) {
#line 343 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 343 "src/sema.tr"
        return box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
    }
#line 344 "src/sema.tr"
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
#line 350 "src/sema.tr"
    Sema* s = ((Sema*)_tr_obj_alloc(sizeof(Sema)));
#line 351 "src/sema.tr"
    s->globals = _tr_dict_new(1024LL);
#line 352 "src/sema.tr"
    s->scopes = (void*)List_ptr_new();
#line 353 "src/sema.tr"
    s->errors = (void*)List_TrStr_new();
#line 354 "src/sema.tr"
    s->warnings = (void*)List_TrStr_new();
#line 355 "src/sema.tr"
    s->classes = _tr_dict_new(128LL);
#line 356 "src/sema.tr"
    s->enums = _tr_dict_new(64LL);
#line 357 "src/sema.tr"
    s->interfaces = _tr_dict_new(32LL);
#line 358 "src/sema.tr"
    s->type_aliases = _tr_dict_new(32LL);
#line 359 "src/sema.tr"
    s->type_alias_elem = _tr_dict_new(32LL);
#line 360 "src/sema.tr"
    s->current_file = _tr_str_lit_len("", 0LL);
#line 361 "src/sema.tr"
    s->current_func_name = _tr_str_lit_len("", 0LL);
#line 362 "src/sema.tr"
    s->current_class_name = _tr_str_lit_len("", 0LL);
#line 363 "src/sema.tr"
    s->current_scope_depth = 0LL;
#line 364 "src/sema.tr"
    s->in_async_fn = false;
#line 365 "src/sema.tr"
    s->fn_try_ok = true;
#line 366 "src/sema.tr"
    s->closure_depth = 0LL;
#line 367 "src/sema.tr"
    s->assign_froms = _tr_dict_new(32LL);
#line 368 "src/sema.tr"
    s->fn_sigs = _tr_dict_new(64LL);
#line 369 "src/sema.tr"
    s->closure_hint = (AstType**)(0LL);
#line 370 "src/sema.tr"
    s->list_hint = (AstType**)(0LL);
#line 371 "src/sema.tr"
    s->extern_names = _tr_dict_new(128LL);
#line 372 "src/sema.tr"
    s->nested_classes = (void*)List_ptr_new();
#line 373 "src/sema.tr"
    s->nested_functions = (void*)List_ptr_new();
#line 374 "src/sema.tr"
    s->nested_enums = (void*)List_ptr_new();
#line 375 "src/sema.tr"
    s->nested_interfaces = (void*)List_ptr_new();
#line 376 "src/sema.tr"
    s->current_line = 0LL;
#line 377 "src/sema.tr"
    s->current_func_generics = (void*)List_TrStr_new();
#line 378 "src/sema.tr"
    s->current_func_constraints = (void*)List_ptr_new();
#line 379 "src/sema.tr"
    s->closure_boundary = (-1LL);
#line 380 "src/sema.tr"
    s->closure_caps = (void*)List_ptr_new();
#line 381 "src/sema.tr"
    s->closure_cap_set = _tr_dict_new(0LL);
#line 382 "src/sema.tr"
    s->in_assign_target = false;
#line 383 "src/sema.tr"
    s->in_assign_target_plain = false;
#line 384 "src/sema.tr"
    s->in_recv_pos = false;
#line 385 "src/sema.tr"
    s->container_borrows = _tr_dict_new(16LL);
#line 386 "src/sema.tr"
    s->alias_assigned = _tr_dict_new(8LL);
#line 387 "src/sema.tr"
    s->sendable_tparams = (void*)List_TrStr_new();
#line 388 "src/sema.tr"
    s->capturing_moves = false;
#line 389 "src/sema.tr"
    s->branch_moved_buf = (void*)List_TrStr_new();
#line 390 "src/sema.tr"
    s->capturing_inits = false;
#line 391 "src/sema.tr"
    s->branch_init_buf = (void*)List_TrStr_new();
#line 392 "src/sema.tr"
    s->copy_classes = _tr_dict_new(32LL);
#line 393 "src/sema.tr"
    s->in_unsafe = false;
#line 394 "src/sema.tr"
    s->cur_fn_is_lib = false;
#line 395 "src/sema.tr"
    s->current_func_ret_from = _tr_str_lit_len("", 0LL);
#line 396 "src/sema.tr"
    s->current_func_ret_borrow_str = false;
#line 397 "src/sema.tr"
    s->current_func_ret_family = _tr_str_lit_len("", 0LL);
#line 398 "src/sema.tr"
    s->ambiguous_classes = _tr_dict_new(8LL);
#line 399 "src/sema.tr"
    s->current_func_ret_regions = (void*)List_TrStr_new();
#line 400 "src/sema.tr"
    s->current_func_outlives_a = (void*)List_TrStr_new();
#line 401 "src/sema.tr"
    s->current_func_outlives_b = (void*)List_TrStr_new();
#line 402 "src/sema.tr"
    s->current_region_params = (void*)List_TrStr_new();
#line 403 "src/sema.tr"
    s->cur_func_borrowers = (void*)List_TrStr_new();
#line 404 "src/sema.tr"
    s->cur_func_sources = (void*)List_TrStr_new();
#line 405 "src/sema.tr"
    s->strict_mode = false;
#line 406 "src/sema.tr"
    s->no_heap = false;
#line 407 "src/sema.tr"
    s->heap_boxes_tuples = false;
#line 408 "src/sema.tr"
    s->mutating_methods = _tr_dict_new(32LL);
#line 409 "src/sema.tr"
    s->fn_ret_owned = _tr_dict_new(64LL);
#line 410 "src/sema.tr"
    s->fn_param_consumes = _tr_dict_new(64LL);
#line 411 "src/sema.tr"
    s->ptr_aliased = _tr_dict_new(32LL);
#line 412 "src/sema.tr"
    s->decorator_names = _tr_dict_new(16LL);
#line 413 "src/sema.tr"
    s->variadic_fns = _tr_dict_new(8LL);
#line 414 "src/sema.tr"
    s->variadic_elem_ty = _tr_dict_new(8LL);
#line 415 "src/sema.tr"
    s->fn_defs = _tr_dict_new(32LL);
#line 416 "src/sema.tr"
    s->loop_scope_base = (void*)List_i64_new();
#line 417 "src/sema.tr"
    s->loop_blk_floor = (void*)List_i64_new();
#line 418 "src/sema.tr"
    s->clo_let_names = (void*)List_TrStr_new();
#line 419 "src/sema.tr"
    s->clo_let_ptrs = (void*)List_ptr_new();
#line 420 "src/sema.tr"
    s->clo_let_scopes = (void*)List_i64_new();
#line 421 "src/sema.tr"
    s->loop_blk_scope = (void*)List_i64_new();
#line 422 "src/sema.tr"
    s->fn_scope_base = (void*)List_i64_new();
#line 423 "src/sema.tr"
    s->block_depth = 0LL;
#line 424 "src/sema.tr"
    s->block_depth_stack = (void*)List_i64_new();
#line 425 "src/sema.tr"
    s->next_block_id = 0LL;
#line 426 "src/sema.tr"
    s->block_stack = (void*)List_i64_new();
#line 427 "src/sema.tr"
    s->shadow_names = (void*)List_TrStr_new();
#line 428 "src/sema.tr"
    s->shadow_prevs = (void*)List_ptr_new();
#line 429 "src/sema.tr"
    s->shadow_blocks = (void*)List_i64_new();
#line 430 "src/sema.tr"
    s->shadow_scopes = (void*)List_i64_new();
#line 431 "src/sema.tr"
    s->block_stack_base = (void*)List_i64_new();
#line 432 "src/sema.tr"
    s->do_temp_ctr = 0LL;
#line 433 "src/sema.tr"
    s->pc_alias_consumes = false;
#line 436 "src/sema.tr"
    ({ __auto_type _ord_t692 = (SymbolKind_make_SFunction()); __auto_type _ord_t693 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t694 = (Symbol_init(_tr_str_lit_len("print", 5LL), _ord_t692, _ord_t693)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("print", 5LL)), _ord_t694); });
#line 437 "src/sema.tr"
    ({ __auto_type _ord_t695 = (SymbolKind_make_SFunction()); __auto_type _ord_t696 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t697 = (Symbol_init(_tr_str_lit_len("input", 5LL), _ord_t695, _ord_t696)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("input", 5LL)), _ord_t697); });
#line 438 "src/sema.tr"
    ({ __auto_type _ord_t698 = (SymbolKind_make_SFunction()); __auto_type _ord_t699 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t700 = (Symbol_init(_tr_str_lit_len("len", 3LL), _ord_t698, _ord_t699)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("len", 3LL)), _ord_t700); });
#line 439 "src/sema.tr"
    AstType* _range_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 440 "src/sema.tr"
    ({ __auto_type _ord_t701 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); List_ptr_append(_range_ty->args, _ord_t701); });
#line 441 "src/sema.tr"
    ({ __auto_type _ord_t702 = (SymbolKind_make_SFunction()); __auto_type _ord_t703 = (box_asttype(_range_ty)); __auto_type _ord_t704 = (Symbol_init(_tr_str_lit_len("range", 5LL), _ord_t702, _ord_t703)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("range", 5LL)), _ord_t704); });
#line 442 "src/sema.tr"
    ({ __auto_type _ord_t705 = (SymbolKind_make_SFunction()); __auto_type _ord_t706 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t707 = (Symbol_init(_tr_str_lit_len("type", 4LL), _ord_t705, _ord_t706)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("type", 4LL)), _ord_t707); });
#line 443 "src/sema.tr"
    ({ __auto_type _ord_t708 = (SymbolKind_make_SFunction()); __auto_type _ord_t709 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t710 = (Symbol_init(_tr_str_lit_len("str", 3LL), _ord_t708, _ord_t709)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("str", 3LL)), _ord_t710); });
#line 444 "src/sema.tr"
    ({ __auto_type _ord_t711 = (SymbolKind_make_SFunction()); __auto_type _ord_t712 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t713 = (Symbol_init(_tr_str_lit_len("hex", 3LL), _ord_t711, _ord_t712)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("hex", 3LL)), _ord_t713); });
#line 445 "src/sema.tr"
    ({ __auto_type _ord_t714 = (SymbolKind_make_SFunction()); __auto_type _ord_t715 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t716 = (Symbol_init(_tr_str_lit_len("oct", 3LL), _ord_t714, _ord_t715)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("oct", 3LL)), _ord_t716); });
#line 446 "src/sema.tr"
    ({ __auto_type _ord_t717 = (SymbolKind_make_SFunction()); __auto_type _ord_t718 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t719 = (Symbol_init(_tr_str_lit_len("bin", 3LL), _ord_t717, _ord_t718)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("bin", 3LL)), _ord_t719); });
#line 450 "src/sema.tr"
    ({ __auto_type _ord_t720 = (SymbolKind_make_SFunction()); __auto_type _ord_t721 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t722 = (Symbol_init(_tr_str_lit_len("kernel_launch", 13LL), _ord_t720, _ord_t721)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("kernel_launch", 13LL)), _ord_t722); });
#line 451 "src/sema.tr"
    ({ __auto_type _ord_t723 = (SymbolKind_make_SFunction()); __auto_type _ord_t724 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t725 = (Symbol_init(_tr_str_lit_len("int", 3LL), _ord_t723, _ord_t724)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("int", 3LL)), _ord_t725); });
#line 452 "src/sema.tr"
    ({ __auto_type _ord_t726 = (SymbolKind_make_SFunction()); __auto_type _ord_t727 = (box_asttype(AstType_init(_tr_str_lit_len("float", 5LL)))); __auto_type _ord_t728 = (Symbol_init(_tr_str_lit_len("float", 5LL), _ord_t726, _ord_t727)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("float", 5LL)), _ord_t728); });
#line 453 "src/sema.tr"
    ({ __auto_type _ord_t729 = (SymbolKind_make_SFunction()); __auto_type _ord_t730 = (box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))); __auto_type _ord_t731 = (Symbol_init(_tr_str_lit_len("bool", 4LL), _ord_t729, _ord_t730)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("bool", 4LL)), _ord_t731); });
#line 454 "src/sema.tr"
    ({ __auto_type _ord_t732 = (SymbolKind_make_SFunction()); __auto_type _ord_t733 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t734 = (Symbol_init(_tr_str_lit_len("ord", 3LL), _ord_t732, _ord_t733)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ord", 3LL)), _ord_t734); });
#line 455 "src/sema.tr"
    ({ __auto_type _ord_t735 = (SymbolKind_make_SFunction()); __auto_type _ord_t736 = (box_asttype(AstType_init(_tr_str_lit_len("char", 4LL)))); __auto_type _ord_t737 = (Symbol_init(_tr_str_lit_len("chr", 3LL), _ord_t735, _ord_t736)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("chr", 3LL)), _ord_t737); });
#line 456 "src/sema.tr"
    ({ __auto_type _ord_t738 = (SymbolKind_make_SFunction()); __auto_type _ord_t739 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t740 = (Symbol_init(_tr_str_lit_len("abs", 3LL), _ord_t738, _ord_t739)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("abs", 3LL)), _ord_t740); });
#line 457 "src/sema.tr"
    ({ __auto_type _ord_t741 = (SymbolKind_make_SFunction()); __auto_type _ord_t742 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t743 = (Symbol_init(_tr_str_lit_len("max", 3LL), _ord_t741, _ord_t742)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("max", 3LL)), _ord_t743); });
#line 458 "src/sema.tr"
    ({ __auto_type _ord_t744 = (SymbolKind_make_SFunction()); __auto_type _ord_t745 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t746 = (Symbol_init(_tr_str_lit_len("min", 3LL), _ord_t744, _ord_t745)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("min", 3LL)), _ord_t746); });
#line 459 "src/sema.tr"
    ({ __auto_type _ord_t747 = (SymbolKind_make_SFunction()); __auto_type _ord_t748 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t749 = (Symbol_init(_tr_str_lit_len("sum", 3LL), _ord_t747, _ord_t748)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("sum", 3LL)), _ord_t749); });
#line 460 "src/sema.tr"
    ({ __auto_type _ord_t750 = (SymbolKind_make_SFunction()); __auto_type _ord_t751 = (box_asttype(AstType_init(_tr_str_lit_len("float", 5LL)))); __auto_type _ord_t752 = (Symbol_init(_tr_str_lit_len("round", 5LL), _ord_t750, _ord_t751)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("round", 5LL)), _ord_t752); });
#line 461 "src/sema.tr"
    ({ __auto_type _ord_t753 = (SymbolKind_make_SFunction()); __auto_type _ord_t754 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t755 = (Symbol_init(_tr_str_lit_len("sorted", 6LL), _ord_t753, _ord_t754)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("sorted", 6LL)), _ord_t755); });
#line 462 "src/sema.tr"
    ({ __auto_type _ord_t756 = (SymbolKind_make_SFunction()); __auto_type _ord_t757 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t758 = (Symbol_init(_tr_str_lit_len("reversed", 8LL), _ord_t756, _ord_t757)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("reversed", 8LL)), _ord_t758); });
#line 463 "src/sema.tr"
    ({ __auto_type _ord_t759 = (SymbolKind_make_SFunction()); __auto_type _ord_t760 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t761 = (Symbol_init(_tr_str_lit_len("iter", 4LL), _ord_t759, _ord_t760)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("iter", 4LL)), _ord_t761); });
#line 464 "src/sema.tr"
    ({ __auto_type _ord_t762 = (SymbolKind_make_SFunction()); __auto_type _ord_t763 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t764 = (Symbol_init(_tr_str_lit_len("enumerate", 9LL), _ord_t762, _ord_t763)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("enumerate", 9LL)), _ord_t764); });
#line 465 "src/sema.tr"
    ({ __auto_type _ord_t765 = (SymbolKind_make_SFunction()); __auto_type _ord_t766 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t767 = (Symbol_init(_tr_str_lit_len("zip", 3LL), _ord_t765, _ord_t766)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("zip", 3LL)), _ord_t767); });
#line 466 "src/sema.tr"
    ({ __auto_type _ord_t768 = (SymbolKind_make_SFunction()); __auto_type _ord_t769 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t770 = (Symbol_init(_tr_str_lit_len("map", 3LL), _ord_t768, _ord_t769)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("map", 3LL)), _ord_t770); });
#line 467 "src/sema.tr"
    ({ __auto_type _ord_t771 = (SymbolKind_make_SFunction()); __auto_type _ord_t772 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t773 = (Symbol_init(_tr_str_lit_len("filter", 6LL), _ord_t771, _ord_t772)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("filter", 6LL)), _ord_t773); });
#line 468 "src/sema.tr"
    ({ __auto_type _ord_t774 = (SymbolKind_make_SFunction()); __auto_type _ord_t775 = (box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))); __auto_type _ord_t776 = (Symbol_init(_tr_str_lit_len("all", 3LL), _ord_t774, _ord_t775)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("all", 3LL)), _ord_t776); });
#line 469 "src/sema.tr"
    ({ __auto_type _ord_t777 = (SymbolKind_make_SFunction()); __auto_type _ord_t778 = (box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))); __auto_type _ord_t779 = (Symbol_init(_tr_str_lit_len("any", 3LL), _ord_t777, _ord_t778)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("any", 3LL)), _ord_t779); });
#line 470 "src/sema.tr"
    ({ __auto_type _ord_t780 = (SymbolKind_make_SFunction()); __auto_type _ord_t781 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t782 = (Symbol_init(_tr_str_lit_len("assert", 6LL), _ord_t780, _ord_t781)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert", 6LL)), _ord_t782); });
#line 471 "src/sema.tr"
    ({ __auto_type _ord_t783 = (SymbolKind_make_SFunction()); __auto_type _ord_t784 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t785 = (Symbol_init(_tr_str_lit_len("assert_eq", 9LL), _ord_t783, _ord_t784)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_eq", 9LL)), _ord_t785); });
#line 472 "src/sema.tr"
    ({ __auto_type _ord_t786 = (SymbolKind_make_SFunction()); __auto_type _ord_t787 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t788 = (Symbol_init(_tr_str_lit_len("assert_ne", 9LL), _ord_t786, _ord_t787)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_ne", 9LL)), _ord_t788); });
#line 473 "src/sema.tr"
    ({ __auto_type _ord_t789 = (SymbolKind_make_SFunction()); __auto_type _ord_t790 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t791 = (Symbol_init(_tr_str_lit_len("assert_lt", 9LL), _ord_t789, _ord_t790)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_lt", 9LL)), _ord_t791); });
#line 474 "src/sema.tr"
    ({ __auto_type _ord_t792 = (SymbolKind_make_SFunction()); __auto_type _ord_t793 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t794 = (Symbol_init(_tr_str_lit_len("assert_le", 9LL), _ord_t792, _ord_t793)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_le", 9LL)), _ord_t794); });
#line 475 "src/sema.tr"
    ({ __auto_type _ord_t795 = (SymbolKind_make_SFunction()); __auto_type _ord_t796 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t797 = (Symbol_init(_tr_str_lit_len("assert_gt", 9LL), _ord_t795, _ord_t796)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_gt", 9LL)), _ord_t797); });
#line 476 "src/sema.tr"
    ({ __auto_type _ord_t798 = (SymbolKind_make_SFunction()); __auto_type _ord_t799 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t800 = (Symbol_init(_tr_str_lit_len("assert_ge", 9LL), _ord_t798, _ord_t799)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("assert_ge", 9LL)), _ord_t800); });
#line 477 "src/sema.tr"
    ({ __auto_type _ord_t801 = (SymbolKind_make_SFunction()); __auto_type _ord_t802 = (box_asttype(AstType_init(_tr_str_lit_len("Option", 6LL)))); __auto_type _ord_t803 = (Symbol_init(_tr_str_lit_len("Some", 4LL), _ord_t801, _ord_t802)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Some", 4LL)), _ord_t803); });
#line 478 "src/sema.tr"
    ({ __auto_type _ord_t804 = (SymbolKind_make_SFunction()); __auto_type _ord_t805 = (box_asttype(AstType_init(_tr_str_lit_len("Result", 6LL)))); __auto_type _ord_t806 = (Symbol_init(_tr_str_lit_len("Ok", 2LL), _ord_t804, _ord_t805)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Ok", 2LL)), _ord_t806); });
#line 479 "src/sema.tr"
    ({ __auto_type _ord_t807 = (SymbolKind_make_SFunction()); __auto_type _ord_t808 = (box_asttype(AstType_init(_tr_str_lit_len("Result", 6LL)))); __auto_type _ord_t809 = (Symbol_init(_tr_str_lit_len("Err", 3LL), _ord_t807, _ord_t808)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Err", 3LL)), _ord_t809); });
#line 480 "src/sema.tr"
    ({ __auto_type _ord_t810 = (SymbolKind_make_SFunction()); __auto_type _ord_t811 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t812 = (Symbol_init(_tr_str_lit_len("_tr_exit", 8LL), _ord_t810, _ord_t811)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("_tr_exit", 8LL)), _ord_t812); });
#line 481 "src/sema.tr"
    ({ __auto_type _ord_t813 = (SymbolKind_make_SFunction()); __auto_type _ord_t814 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t815 = (Symbol_init(_tr_str_lit_len("_tr_strlen", 10LL), _ord_t813, _ord_t814)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("_tr_strlen", 10LL)), _ord_t815); });
#line 482 "src/sema.tr"
    ({ __auto_type _ord_t816 = (SymbolKind_make_SFunction()); __auto_type _ord_t817 = (box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)))); __auto_type _ord_t818 = (Symbol_init(_tr_str_lit_len("read_file", 9LL), _ord_t816, _ord_t817)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("read_file", 9LL)), _ord_t818); });
#line 483 "src/sema.tr"
    ({ __auto_type _ord_t819 = (SymbolKind_make_SFunction()); __auto_type _ord_t820 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t821 = (Symbol_init(_tr_str_lit_len("write_file", 10LL), _ord_t819, _ord_t820)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("write_file", 10LL)), _ord_t821); });
#line 484 "src/sema.tr"
    ({ __auto_type _ord_t822 = (SymbolKind_make_SFunction()); __auto_type _ord_t823 = (box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)))); __auto_type _ord_t824 = (Symbol_init(_tr_str_lit_len("file_exists", 11LL), _ord_t822, _ord_t823)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("file_exists", 11LL)), _ord_t824); });
#line 485 "src/sema.tr"
    ({ __auto_type _ord_t825 = (SymbolKind_make_SFunction()); __auto_type _ord_t826 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); __auto_type _ord_t827 = (Symbol_init(_tr_str_lit_len("_tr_system", 10LL), _ord_t825, _ord_t826)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("_tr_system", 10LL)), _ord_t827); });
#line 490 "src/sema.tr"
    ({ __auto_type _ord_t828 = (SymbolKind_make_SClass()); __auto_type _ord_t829 = (box_asttype(AstType_init(_tr_str_lit_len("List", 4LL)))); __auto_type _ord_t830 = (Symbol_init(_tr_str_lit_len("List", 4LL), _ord_t828, _ord_t829)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("List", 4LL)), _ord_t830); });
#line 491 "src/sema.tr"
    ({ __auto_type _ord_t831 = (SymbolKind_make_SClass()); __auto_type _ord_t832 = (box_asttype(AstType_init(_tr_str_lit_len("Pointer", 7LL)))); __auto_type _ord_t833 = (Symbol_init(_tr_str_lit_len("Pointer", 7LL), _ord_t831, _ord_t832)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Pointer", 7LL)), _ord_t833); });
#line 492 "src/sema.tr"
    ({ __auto_type _ord_t834 = (SymbolKind_make_SClass()); __auto_type _ord_t835 = (box_asttype(AstType_init(_tr_str_lit_len("StringBuilder", 13LL)))); __auto_type _ord_t836 = (Symbol_init(_tr_str_lit_len("StringBuilder", 13LL), _ord_t834, _ord_t835)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("StringBuilder", 13LL)), _ord_t836); });
#line 493 "src/sema.tr"
    ({ __auto_type _ord_t837 = (SymbolKind_make_SClass()); __auto_type _ord_t838 = (box_asttype(AstType_init(_tr_str_lit_len("StringObj", 9LL)))); __auto_type _ord_t839 = (Symbol_init(_tr_str_lit_len("StringObj", 9LL), _ord_t837, _ord_t838)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("StringObj", 9LL)), _ord_t839); });
#line 495 "src/sema.tr"
    ({ __auto_type _ord_t840 = (SymbolKind_make_SClass()); __auto_type _ord_t841 = (box_asttype(AstType_init(_tr_str_lit_len("Dict", 4LL)))); __auto_type _ord_t842 = (Symbol_init(_tr_str_lit_len("Dict", 4LL), _ord_t840, _ord_t841)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Dict", 4LL)), _ord_t842); });
#line 496 "src/sema.tr"
    ({ __auto_type _ord_t843 = (SymbolKind_make_SClass()); __auto_type _ord_t844 = (box_asttype(AstType_init(_tr_str_lit_len("Set", 3LL)))); __auto_type _ord_t845 = (Symbol_init(_tr_str_lit_len("Set", 3LL), _ord_t843, _ord_t844)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Set", 3LL)), _ord_t845); });
#line 497 "src/sema.tr"
    ({ __auto_type _ord_t846 = (SymbolKind_make_SClass()); __auto_type _ord_t847 = (box_asttype(AstType_init(_tr_str_lit_len("Box", 3LL)))); __auto_type _ord_t848 = (Symbol_init(_tr_str_lit_len("Box", 3LL), _ord_t846, _ord_t847)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Box", 3LL)), _ord_t848); });
#line 500 "src/sema.tr"
    ({ __auto_type _ord_t849 = (SymbolKind_make_SEnum()); __auto_type _ord_t850 = (box_asttype(AstType_init(_tr_str_lit_len("Option", 6LL)))); __auto_type _ord_t851 = (Symbol_init(_tr_str_lit_len("Option", 6LL), _ord_t849, _ord_t850)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Option", 6LL)), _ord_t851); });
#line 501 "src/sema.tr"
    ({ __auto_type _ord_t852 = (SymbolKind_make_SEnum()); __auto_type _ord_t853 = (box_asttype(AstType_init(_tr_str_lit_len("Result", 6LL)))); __auto_type _ord_t854 = (Symbol_init(_tr_str_lit_len("Result", 6LL), _ord_t852, _ord_t853)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Result", 6LL)), _ord_t854); });
#line 504 "src/sema.tr"
    ({ __auto_type _ord_t855 = (SymbolKind_make_SClass()); __auto_type _ord_t856 = (box_asttype(AstType_init(_tr_str_lit_len("Exception", 9LL)))); __auto_type _ord_t857 = (Symbol_init(_tr_str_lit_len("Exception", 9LL), _ord_t855, _ord_t856)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Exception", 9LL)), _ord_t857); });
#line 505 "src/sema.tr"
    ({ __auto_type _ord_t858 = (SymbolKind_make_SClass()); __auto_type _ord_t859 = (box_asttype(AstType_init(_tr_str_lit_len("Error", 5LL)))); __auto_type _ord_t860 = (Symbol_init(_tr_str_lit_len("Error", 5LL), _ord_t858, _ord_t859)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Error", 5LL)), _ord_t860); });
#line 506 "src/sema.tr"
    ({ __auto_type _ord_t861 = (SymbolKind_make_SClass()); __auto_type _ord_t862 = (box_asttype(AstType_init(_tr_str_lit_len("ValueError", 10LL)))); __auto_type _ord_t863 = (Symbol_init(_tr_str_lit_len("ValueError", 10LL), _ord_t861, _ord_t862)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ValueError", 10LL)), _ord_t863); });
#line 507 "src/sema.tr"
    ({ __auto_type _ord_t864 = (SymbolKind_make_SClass()); __auto_type _ord_t865 = (box_asttype(AstType_init(_tr_str_lit_len("TypeError", 9LL)))); __auto_type _ord_t866 = (Symbol_init(_tr_str_lit_len("TypeError", 9LL), _ord_t864, _ord_t865)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("TypeError", 9LL)), _ord_t866); });
#line 508 "src/sema.tr"
    ({ __auto_type _ord_t867 = (SymbolKind_make_SClass()); __auto_type _ord_t868 = (box_asttype(AstType_init(_tr_str_lit_len("IndexError", 10LL)))); __auto_type _ord_t869 = (Symbol_init(_tr_str_lit_len("IndexError", 10LL), _ord_t867, _ord_t868)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("IndexError", 10LL)), _ord_t869); });
#line 509 "src/sema.tr"
    ({ __auto_type _ord_t870 = (SymbolKind_make_SClass()); __auto_type _ord_t871 = (box_asttype(AstType_init(_tr_str_lit_len("IOError", 7LL)))); __auto_type _ord_t872 = (Symbol_init(_tr_str_lit_len("IOError", 7LL), _ord_t870, _ord_t871)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("IOError", 7LL)), _ord_t872); });
#line 510 "src/sema.tr"
    ({ __auto_type _ord_t873 = (SymbolKind_make_SClass()); __auto_type _ord_t874 = (box_asttype(AstType_init(_tr_str_lit_len("KeyError", 8LL)))); __auto_type _ord_t875 = (Symbol_init(_tr_str_lit_len("KeyError", 8LL), _ord_t873, _ord_t874)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("KeyError", 8LL)), _ord_t875); });
#line 513 "src/sema.tr"
    ({ __auto_type _ord_t876 = (SymbolKind_make_SClass()); __auto_type _ord_t877 = (box_asttype(AstType_init(_tr_str_lit_len("Task", 4LL)))); __auto_type _ord_t878 = (Symbol_init(_tr_str_lit_len("Task", 4LL), _ord_t876, _ord_t877)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Task", 4LL)), _ord_t878); });
#line 514 "src/sema.tr"
    ({ __auto_type _ord_t879 = (SymbolKind_make_SClass()); __auto_type _ord_t880 = (box_asttype(AstType_init(_tr_str_lit_len("Future", 6LL)))); __auto_type _ord_t881 = (Symbol_init(_tr_str_lit_len("Future", 6LL), _ord_t879, _ord_t880)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Future", 6LL)), _ord_t881); });
#line 515 "src/sema.tr"
    ({ __auto_type _ord_t882 = (SymbolKind_make_SClass()); __auto_type _ord_t883 = (box_asttype(AstType_init(_tr_str_lit_len("Channel", 7LL)))); __auto_type _ord_t884 = (Symbol_init(_tr_str_lit_len("Channel", 7LL), _ord_t882, _ord_t883)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Channel", 7LL)), _ord_t884); });
#line 516 "src/sema.tr"
    ({ __auto_type _ord_t885 = (SymbolKind_make_SClass()); __auto_type _ord_t886 = (box_asttype(AstType_init(_tr_str_lit_len("Chan", 4LL)))); __auto_type _ord_t887 = (Symbol_init(_tr_str_lit_len("Chan", 4LL), _ord_t885, _ord_t886)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Chan", 4LL)), _ord_t887); });
#line 517 "src/sema.tr"
    ({ __auto_type _ord_t888 = (SymbolKind_make_SClass()); __auto_type _ord_t889 = (box_asttype(AstType_init(_tr_str_lit_len("Mutex", 5LL)))); __auto_type _ord_t890 = (Symbol_init(_tr_str_lit_len("Mutex", 5LL), _ord_t888, _ord_t889)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Mutex", 5LL)), _ord_t890); });
#line 518 "src/sema.tr"
    ({ __auto_type _ord_t891 = (SymbolKind_make_SClass()); __auto_type _ord_t892 = (box_asttype(AstType_init(_tr_str_lit_len("RwLock", 6LL)))); __auto_type _ord_t893 = (Symbol_init(_tr_str_lit_len("RwLock", 6LL), _ord_t891, _ord_t892)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("RwLock", 6LL)), _ord_t893); });
#line 519 "src/sema.tr"
    ({ __auto_type _ord_t894 = (SymbolKind_make_SClass()); __auto_type _ord_t895 = (box_asttype(AstType_init(_tr_str_lit_len("ThreadPool", 10LL)))); __auto_type _ord_t896 = (Symbol_init(_tr_str_lit_len("ThreadPool", 10LL), _ord_t894, _ord_t895)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ThreadPool", 10LL)), _ord_t896); });
#line 520 "src/sema.tr"
    ({ __auto_type _ord_t897 = (SymbolKind_make_SClass()); __auto_type _ord_t898 = (box_asttype(AstType_init(_tr_str_lit_len("Thread", 6LL)))); __auto_type _ord_t899 = (Symbol_init(_tr_str_lit_len("Thread", 6LL), _ord_t897, _ord_t898)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Thread", 6LL)), _ord_t899); });
#line 521 "src/sema.tr"
    ({ __auto_type _ord_t900 = (SymbolKind_make_SClass()); __auto_type _ord_t901 = (box_asttype(AstType_init(_tr_str_lit_len("AsyncPool", 9LL)))); __auto_type _ord_t902 = (Symbol_init(_tr_str_lit_len("AsyncPool", 9LL), _ord_t900, _ord_t901)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("AsyncPool", 9LL)), _ord_t902); });
#line 522 "src/sema.tr"
    ({ __auto_type _ord_t903 = (SymbolKind_make_SClass()); __auto_type _ord_t904 = (box_asttype(AstType_init(_tr_str_lit_len("AsyncTask", 9LL)))); __auto_type _ord_t905 = (Symbol_init(_tr_str_lit_len("AsyncTask", 9LL), _ord_t903, _ord_t904)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("AsyncTask", 9LL)), _ord_t905); });
#line 523 "src/sema.tr"
    ({ __auto_type _ord_t906 = (SymbolKind_make_SClass()); __auto_type _ord_t907 = (box_asttype(AstType_init(_tr_str_lit_len("Atomic", 6LL)))); __auto_type _ord_t908 = (Symbol_init(_tr_str_lit_len("Atomic", 6LL), _ord_t906, _ord_t907)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Atomic", 6LL)), _ord_t908); });
#line 524 "src/sema.tr"
    ({ __auto_type _ord_t909 = (SymbolKind_make_SClass()); __auto_type _ord_t910 = (box_asttype(AstType_init(_tr_str_lit_len("ThreadLocal", 11LL)))); __auto_type _ord_t911 = (Symbol_init(_tr_str_lit_len("ThreadLocal", 11LL), _ord_t909, _ord_t910)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("ThreadLocal", 11LL)), _ord_t911); });
#line 525 "src/sema.tr"
    ({ __auto_type _ord_t912 = (SymbolKind_make_SFunction()); __auto_type _ord_t913 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _ord_t914 = (Symbol_init(_tr_str_lit_len("await_all", 9LL), _ord_t912, _ord_t913)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("await_all", 9LL)), _ord_t914); });
#line 526 "src/sema.tr"
    ({ __auto_type _ord_t915 = (SymbolKind_make_SClass()); __auto_type _ord_t916 = (box_asttype(AstType_init(_tr_str_lit_len("Arc", 3LL)))); __auto_type _ord_t917 = (Symbol_init(_tr_str_lit_len("Arc", 3LL), _ord_t915, _ord_t916)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Arc", 3LL)), _ord_t917); });
#line 527 "src/sema.tr"
    ({ __auto_type _ord_t918 = (SymbolKind_make_SClass()); __auto_type _ord_t919 = (box_asttype(AstType_init(_tr_str_lit_len("Rc", 2LL)))); __auto_type _ord_t920 = (Symbol_init(_tr_str_lit_len("Rc", 2LL), _ord_t918, _ord_t919)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Rc", 2LL)), _ord_t920); });
#line 528 "src/sema.tr"
    ({ __auto_type _ord_t921 = (SymbolKind_make_SClass()); __auto_type _ord_t922 = (box_asttype(AstType_init(_tr_str_lit_len("WaitGroup", 9LL)))); __auto_type _ord_t923 = (Symbol_init(_tr_str_lit_len("WaitGroup", 9LL), _ord_t921, _ord_t922)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("WaitGroup", 9LL)), _ord_t923); });
#line 531 "src/sema.tr"
    ({ __auto_type _ord_t924 = (SymbolKind_make_SClass()); __auto_type _ord_t925 = (box_asttype(AstType_init(_tr_str_lit_len("Shared", 6LL)))); __auto_type _ord_t926 = (Symbol_init(_tr_str_lit_len("Shared", 6LL), _ord_t924, _ord_t925)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Shared", 6LL)), _ord_t926); });
#line 532 "src/sema.tr"
    ({ __auto_type _ord_t927 = (SymbolKind_make_SClass()); __auto_type _ord_t928 = (box_asttype(AstType_init(_tr_str_lit_len("Weak", 4LL)))); __auto_type _ord_t929 = (Symbol_init(_tr_str_lit_len("Weak", 4LL), _ord_t927, _ord_t928)); _tr_dict_set(s->globals, _tr_strz(_tr_str_lit_len("Weak", 4LL)), _ord_t929); });
#line 534 "src/sema.tr"
    return s;
}

__attribute__((hot)) TrStr Sema_io_ty_str(Sema* self, AstType* ty) {
#line 539 "src/sema.tr"
    TrStr s = _tr_str_retain(ty->name);
#line 540 "src/sema.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("", 0LL)))) {
#line 540 "src/sema.tr"
        TrStr _strtmp_t930 = _tr_str_lit_len("void", 4LL);
        _tr_str_release(s);
        s = _strtmp_t930;
    }
#line 541 "src/sema.tr"
    if ((ty->args->len > 0LL)) {
#line 542 "src/sema.tr"
        TrStr _strtmp_t932 = _tr_strx_concatv((s), (_tr_str_lit_len("[", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t932;
#line 543 "src/sema.tr"
        long long i = 0LL;
#line 544 "src/sema.tr"
        while ((i < ty->args->len)) {
#line 545 "src/sema.tr"
            if ((i > 0LL)) {
#line 545 "src/sema.tr"
                TrStr _strtmp_t934 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t934;
            }
#line 546 "src/sema.tr"
            TrStr _strtmp_t936 = ({ TrStr _tr_cr = (Sema_io_ty_str(self, (*((AstType**)List_ptr_get(ty->args, i))))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t936;
#line 547 "src/sema.tr"
            i = (i + 1LL);
        }
#line 548 "src/sema.tr"
        TrStr _strtmp_t938 = _tr_strx_concatv((s), (_tr_str_lit_len("]", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t938;
    }
#line 549 "src/sema.tr"
    TrStr _trret178 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret178;
}

__attribute__((hot)) TrStr Sema_io_doc_of(Sema* self, Block* body) {
#line 553 "src/sema.tr"
    long long i = 0LL;
#line 554 "src/sema.tr"
    while ((i < body->stmts->len)) {
#line 555 "src/sema.tr"
        __auto_type _t939 = (*((Stmt*)List_ptr_get(body->stmts, i)));
        if (_t939.tag == Stmt_SLine) {
            __auto_type _ = _t939.data.SLine.n;
            /* pass */
        } else if (_t939.tag == Stmt_SExpr) {
            __auto_type e = _t939.data.SExpr.expr;
#line 558 "src/sema.tr"
            if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
#line 559 "src/sema.tr"
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
#line 562 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        } else if (1) {
            __auto_type _ = _t939;
            return _tr_str_lit_len("", 0LL);
        }
#line 564 "src/sema.tr"
        i = (i + 1LL);
    }
#line 565 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Sema_io_func_sig(Sema* self, FunctionDef* f) {
#line 568 "src/sema.tr"
    TrStr s = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("def ", 4LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 569 "src/sema.tr"
    long long i = 0LL;
#line 570 "src/sema.tr"
    while ((i < f->params->len)) {
#line 571 "src/sema.tr"
        if ((i > 0LL)) {
#line 571 "src/sema.tr"
            TrStr _strtmp_t942 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t942;
        }
#line 572 "src/sema.tr"
        Param* p = ((Param*)List_ptr_get(f->params, i));
#line 573 "src/sema.tr"
        ({ TrStr _ap_t943 = (p->name); _tr_str_append(&s, _ap_t943); });
#line 574 "src/sema.tr"
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
#line 575 "src/sema.tr"
            TrStr _strtmp_t946 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cr = (Sema_io_ty_str(self, (*p->ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t946;
        }
#line 576 "src/sema.tr"
        if (p->is_variadic) {
#line 576 "src/sema.tr"
            TrStr _strtmp_t948 = _tr_strx_concatv((s), (_tr_str_lit_len("...", 3LL)));
            _tr_str_release(s);
            s = _strtmp_t948;
        }
#line 577 "src/sema.tr"
        i = (i + 1LL);
    }
#line 578 "src/sema.tr"
    TrStr _strtmp_t950 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
    _tr_str_release(s);
    s = _strtmp_t950;
#line 579 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 580 "src/sema.tr"
        AstType* rt = (*f->ret_ty);
#line 581 "src/sema.tr"
        if ((((!_tr_str_eqv((rt->name), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((rt->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((rt->name), (_tr_str_lit_len("None", 4LL)))))) {
#line 582 "src/sema.tr"
            TrStr _strtmp_t953 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _tr_cr = (Sema_io_ty_str(self, rt)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t953;
        }
    }
#line 583 "src/sema.tr"
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 584 "src/sema.tr"
        if ((!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 585 "src/sema.tr"
            TrStr _strtmp_t956 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" throws ", 8LL)))); TrStr _tr_cr = (Sema_io_ty_str(self, (*f->throws_ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t956;
        }
    }
#line 586 "src/sema.tr"
    TrStr doc = Sema_io_doc_of(self, f->body);
#line 587 "src/sema.tr"
    if ((!_tr_str_eqv((doc), (_tr_str_lit_len("", 0LL))))) {
#line 588 "src/sema.tr"
        TrStr _strtmp_t960 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("\n        \"\"\"", 12LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (doc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"\"\"", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t960;
    }
#line 589 "src/sema.tr"
    TrStr _trret180 = _tr_str_retain(s);
    _tr_str_release(s);
    _tr_str_release(doc);
    return _trret180;
}

__attribute__((hot)) TrStr Sema_build_inspect_str(Sema* self, TrStr name) {
#line 594 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
#line 595 "src/sema.tr"
        ClassDef* c = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
#line 596 "src/sema.tr"
        TrStr s = _tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (c->name));
#line 597 "src/sema.tr"
        if ((c->base_classes->len > 0LL)) {
#line 598 "src/sema.tr"
            TrStr _strtmp_t962 = _tr_strx_concatv((s), (_tr_str_lit_len("(extends ", 9LL)));
            _tr_str_release(s);
            s = _strtmp_t962;
#line 599 "src/sema.tr"
            long long bi = 0LL;
#line 600 "src/sema.tr"
            while ((bi < c->base_classes->len)) {
#line 601 "src/sema.tr"
                if ((bi > 0LL)) {
#line 601 "src/sema.tr"
                    TrStr _strtmp_t964 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                    _tr_str_release(s);
                    s = _strtmp_t964;
                }
#line 602 "src/sema.tr"
                TrStr _strtmp_t966 = ({ TrStr _tr_cr = (List_TrStr_get(c->base_classes, bi)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t966;
#line 603 "src/sema.tr"
                bi = (bi + 1LL);
            }
#line 604 "src/sema.tr"
            TrStr _strtmp_t968 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
            _tr_str_release(s);
            s = _strtmp_t968;
        }
#line 605 "src/sema.tr"
        TrStr _strtmp_t970 = _tr_strx_concatv((s), (_tr_str_lit_len(":\n", 2LL)));
        _tr_str_release(s);
        s = _strtmp_t970;
#line 606 "src/sema.tr"
        if ((!_tr_str_eqv((c->docstring), (_tr_str_lit_len("", 0LL))))) {
#line 607 "src/sema.tr"
            TrStr _strtmp_t974 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("  doc: ", 7LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->docstring)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t974;
        }
#line 608 "src/sema.tr"
        if ((c->fields->len > 0LL)) {
#line 609 "src/sema.tr"
            TrStr _strtmp_t976 = _tr_strx_concatv((s), (_tr_str_lit_len("  fields:\n", 10LL)));
            _tr_str_release(s);
            s = _strtmp_t976;
#line 610 "src/sema.tr"
            long long fi = 0LL;
#line 611 "src/sema.tr"
            while ((fi < c->fields->len)) {
#line 612 "src/sema.tr"
                FieldDef* fld = ((FieldDef*)List_ptr_get(c->fields, fi));
#line 613 "src/sema.tr"
                TrStr _strtmp_t979 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fld->name)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t979;
#line 614 "src/sema.tr"
                if ((((unsigned long long)(fld->ty)) != ((unsigned long long)(0LL)))) {
#line 615 "src/sema.tr"
                    TrStr _strtmp_t982 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cr = (Sema_io_ty_str(self, (*fld->ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
                    _tr_str_release(s);
                    s = _strtmp_t982;
                }
#line 616 "src/sema.tr"
                TrStr _strtmp_t984 = _tr_strx_concatv((s), (_tr_str_lit_len("\n", 1LL)));
                _tr_str_release(s);
                s = _strtmp_t984;
#line 617 "src/sema.tr"
                fi = (fi + 1LL);
            }
        }
#line 618 "src/sema.tr"
        if ((c->methods->len > 0LL)) {
#line 619 "src/sema.tr"
            TrStr _strtmp_t986 = _tr_strx_concatv((s), (_tr_str_lit_len("  methods:\n", 11LL)));
            _tr_str_release(s);
            s = _strtmp_t986;
#line 620 "src/sema.tr"
            long long mi = 0LL;
#line 621 "src/sema.tr"
            while ((mi < c->methods->len)) {
#line 622 "src/sema.tr"
                TrStr _strtmp_t990 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(c->methods, mi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t990;
#line 623 "src/sema.tr"
                mi = (mi + 1LL);
            }
        }
#line 624 "src/sema.tr"
        TrStr _trret181 = _tr_str_retain(s);
        _tr_str_release(s);
        return _trret181;
    } else if (_tr_dict_contains(self->enums, _tr_strz(name))) {
#line 626 "src/sema.tr"
        EnumDef* e = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(name)));
#line 627 "src/sema.tr"
        TrStr s = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("enum ", 5LL)), (e->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 628 "src/sema.tr"
        if ((e->variants->len > 0LL)) {
#line 629 "src/sema.tr"
            TrStr _strtmp_t992 = _tr_strx_concatv((s), (_tr_str_lit_len("  variants:\n", 12LL)));
            _tr_str_release(s);
            s = _strtmp_t992;
#line 630 "src/sema.tr"
            long long vi = 0LL;
#line 631 "src/sema.tr"
            while ((vi < e->variants->len)) {
#line 632 "src/sema.tr"
                VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, vi));
#line 633 "src/sema.tr"
                TrStr _strtmp_t995 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->name)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t995;
#line 634 "src/sema.tr"
                if ((v->fields->len > 0LL)) {
#line 635 "src/sema.tr"
                    TrStr _strtmp_t997 = _tr_strx_concatv((s), (_tr_str_lit_len("(", 1LL)));
                    _tr_str_release(s);
                    s = _strtmp_t997;
#line 636 "src/sema.tr"
                    long long vfi = 0LL;
#line 637 "src/sema.tr"
                    while ((vfi < v->fields->len)) {
#line 638 "src/sema.tr"
                        if ((vfi > 0LL)) {
#line 638 "src/sema.tr"
                            TrStr _strtmp_t999 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                            _tr_str_release(s);
                            s = _strtmp_t999;
                        }
#line 639 "src/sema.tr"
                        Param* vf = ((Param*)List_ptr_get(v->fields, vfi));
#line 640 "src/sema.tr"
                        if ((((unsigned long long)(vf->ty)) != ((unsigned long long)(0LL)))) {
#line 641 "src/sema.tr"
                            TrStr _strtmp_t1001 = ({ TrStr _tr_cr = (Sema_io_ty_str(self, (*vf->ty))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
                            _tr_str_release(s);
                            s = _strtmp_t1001;
                        }
#line 642 "src/sema.tr"
                        vfi = (vfi + 1LL);
                    }
#line 643 "src/sema.tr"
                    TrStr _strtmp_t1003 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
                    _tr_str_release(s);
                    s = _strtmp_t1003;
                }
#line 644 "src/sema.tr"
                TrStr _strtmp_t1005 = _tr_strx_concatv((s), (_tr_str_lit_len("\n", 1LL)));
                _tr_str_release(s);
                s = _strtmp_t1005;
#line 645 "src/sema.tr"
                vi = (vi + 1LL);
            }
        }
#line 646 "src/sema.tr"
        if ((e->methods->len > 0LL)) {
#line 647 "src/sema.tr"
            TrStr _strtmp_t1007 = _tr_strx_concatv((s), (_tr_str_lit_len("  methods:\n", 11LL)));
            _tr_str_release(s);
            s = _strtmp_t1007;
#line 648 "src/sema.tr"
            long long mi = 0LL;
#line 649 "src/sema.tr"
            while ((mi < e->methods->len)) {
#line 650 "src/sema.tr"
                TrStr _strtmp_t1011 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(e->methods, mi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t1011;
#line 651 "src/sema.tr"
                mi = (mi + 1LL);
            }
        }
#line 652 "src/sema.tr"
        TrStr _trret183 = _tr_str_retain(s);
        _tr_str_release(s);
        return _trret183;
    } else if (_tr_dict_contains(self->interfaces, _tr_strz(name))) {
#line 654 "src/sema.tr"
        InterfaceDef* iface = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(name)));
#line 655 "src/sema.tr"
        TrStr s = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("interface ", 10LL)), (iface->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 656 "src/sema.tr"
        if ((iface->methods->len > 0LL)) {
#line 657 "src/sema.tr"
            TrStr _strtmp_t1013 = _tr_strx_concatv((s), (_tr_str_lit_len("  methods:\n", 11LL)));
            _tr_str_release(s);
            s = _strtmp_t1013;
#line 658 "src/sema.tr"
            long long mi = 0LL;
#line 659 "src/sema.tr"
            while ((mi < iface->methods->len)) {
#line 660 "src/sema.tr"
                TrStr _strtmp_t1017 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("    ", 4LL)))); TrStr _tr_cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(iface->methods, mi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t1017;
#line 661 "src/sema.tr"
                mi = (mi + 1LL);
            }
        }
#line 662 "src/sema.tr"
        TrStr _trret182 = _tr_str_retain(s);
        _tr_str_release(s);
        return _trret182;
    } else if (_tr_dict_contains(self->fn_defs, _tr_strz(name))) {
#line 664 "src/sema.tr"
        return Sema_io_func_sig(self, ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(name))));
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("int", 3LL)))) {
#line 666 "src/sema.tr"
        return _tr_str_lit_len("int: 64-bit signed integer (C long long).", 41LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("float", 5LL)))) {
#line 668 "src/sema.tr"
        return _tr_str_lit_len("float: 64-bit floating point number (C double).", 47LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL)))) {
#line 670 "src/sema.tr"
        return _tr_str_lit_len("bool: true/false (C _Bool).", 27LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL)))) {
#line 672 "src/sema.tr"
        return _tr_str_lit_len("str: immutable byte string (C char*).", 37LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("char", 4LL)))) {
#line 674 "src/sema.tr"
        return _tr_str_lit_len("char: single byte character (C char).", 37LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("List", 4LL)))) {
#line 676 "src/sema.tr"
        return _tr_str_lit_len("List[T]: growable, ordered, indexable sequence of T.", 52LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Dict", 4LL)))) {
#line 678 "src/sema.tr"
        return _tr_str_lit_len("Dict[K, V]: hash map from K to V.", 33LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Set", 3LL)))) {
#line 680 "src/sema.tr"
        return _tr_str_lit_len("Set[T]: unordered collection of unique T values.", 48LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Option", 6LL)))) {
#line 682 "src/sema.tr"
        return _tr_str_lit_len("Option[T]: either Some(T) or None.", 34LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Result", 6LL)))) {
#line 684 "src/sema.tr"
        return _tr_str_lit_len("Result[T, E]: either Ok(T) or Err(E).", 37LL);
    } else if (_tr_str_eqv((name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 686 "src/sema.tr"
        return _tr_str_lit_len("Pointer[T]: raw pointer to a T (C T*); use unsafe: for arithmetic.", 66LL);
    } else {
#line 688 "src/sema.tr"
        return ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("No info available for '", 23LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
    }
}

__attribute__((hot)) void Sema_error(Sema* self, TrStr msg) {
#line 691 "src/sema.tr"
    TrStr loc = _tr_str_lit_len("", 0LL);
#line 692 "src/sema.tr"
    if ((_tr_str_lenv((self->current_file)) > 0LL)) {
#line 693 "src/sema.tr"
        TrStr _strtmp_t1018 = _tr_strx_concatv((self->current_file), (_tr_str_lit_len(":", 1LL)));
        _tr_str_release(loc);
        loc = _strtmp_t1018;
    }
#line 694 "src/sema.tr"
    if ((self->current_line > 0LL)) {
#line 695 "src/sema.tr"
        ({ TrStr _ord_t1019 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->current_line)))); TrStr _tr_cres = _tr_strx_concatv((loc), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (msg)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->errors, _ord_t1019); _tr_str_release(_ord_t1019); });
    } else {
#line 697 "src/sema.tr"
        List_TrStr_append(self->errors, msg);
    }
    _tr_str_release(loc);
}

__attribute__((hot)) bool Sema_is_sendable_type(Sema* self, TrStr ty_name) {
#line 702 "src/sema.tr"
    long long tpi = 0LL;
#line 703 "src/sema.tr"
    while ((tpi < self->sendable_tparams->len)) {
#line 704 "src/sema.tr"
        if (({ TrStr _cmpa_t1020 = (List_TrStr_get(self->sendable_tparams, tpi)); TrStr _cmpb_t1021 = (ty_name); _Bool _cmpr_t1022 = _tr_str_eqv(_cmpa_t1020, _cmpb_t1021); _tr_str_release(_cmpa_t1020); _cmpr_t1022; })) {
#line 704 "src/sema.tr"
            return true;
        }
#line 705 "src/sema.tr"
        tpi = (tpi + 1LL);
    }
#line 706 "src/sema.tr"
    if (((((_tr_str_eqv((ty_name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("str", 3LL))))) {
#line 706 "src/sema.tr"
        return true;
    }
#line 707 "src/sema.tr"
    if (((((_tr_str_eqv((ty_name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("auto", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("isize", 5LL))))) {
#line 707 "src/sema.tr"
        return true;
    }
#line 708 "src/sema.tr"
    if (((_tr_str_eqv((ty_name), (_tr_str_lit_len("Atomic", 6LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("RwLock", 6LL))))) {
#line 708 "src/sema.tr"
        return true;
    }
#line 709 "src/sema.tr"
    if (((_tr_str_eqv((ty_name), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("ThreadPool", 10LL))))) {
#line 709 "src/sema.tr"
        return true;
    }
#line 710 "src/sema.tr"
    if ((_tr_str_eqv((ty_name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
#line 710 "src/sema.tr"
        return true;
    }
#line 711 "src/sema.tr"
    if ((_tr_str_eqv((ty_name), (_tr_str_lit_len("AsyncPool", 9LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("AsyncTask", 9LL))))) {
#line 711 "src/sema.tr"
        return true;
    }
#line 712 "src/sema.tr"
    if ((_tr_str_eqv((ty_name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Weak", 4LL))))) {
#line 712 "src/sema.tr"
        return true;
    }
#line 714 "src/sema.tr"
    if ((((_tr_str_eqv((ty_name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((ty_name), (_tr_str_lit_len("Map", 3LL))))) {
#line 714 "src/sema.tr"
        return false;
    }
#line 716 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(ty_name))) {
#line 717 "src/sema.tr"
        ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ty_name)));
#line 718 "src/sema.tr"
        if ((!_is_invalid_ptr(((unsigned long long)(cls->iface_names))))) {
#line 719 "src/sema.tr"
            long long ii = 0LL;
#line 720 "src/sema.tr"
            while ((ii < cls->iface_names->len)) {
#line 721 "src/sema.tr"
                if (({ TrStr _cmpa_t1023 = (List_TrStr_get(cls->iface_names, ii)); TrStr _cmpb_t1024 = (_tr_str_lit_len("Sendable", 8LL)); _Bool _cmpr_t1025 = _tr_str_eqv(_cmpa_t1023, _cmpb_t1024); _tr_str_release(_cmpa_t1023); _cmpr_t1025; })) {
#line 721 "src/sema.tr"
                    return true;
                }
#line 722 "src/sema.tr"
                ii = (ii + 1LL);
            }
        }
#line 724 "src/sema.tr"
        return false;
    }
#line 726 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_sendable_ty(Sema* self, AstType* ty) {
#line 735 "src/sema.tr"
    TrStr n = _tr_str_retain(ty->name);
#line 736 "src/sema.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL))))) {
#line 737 "src/sema.tr"
        if ((ty->args->len > 0LL)) {
#line 738 "src/sema.tr"
            bool _trret184 = Sema_is_sendable_ty(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
            _tr_str_release(n);
            return _trret184;
        }
#line 739 "src/sema.tr"
        _tr_str_release(n);
        return true;
    }
#line 740 "src/sema.tr"
    bool _trret185 = Sema_is_sendable_type(self, n);
    _tr_str_release(n);
    return _trret185;
}

__attribute__((hot)) TrStr Sema_decl_family(Sema* self, AstType* ty) {
#line 747 "src/sema.tr"
    TrStr n = _tr_str_retain(ty->name);
#line 748 "src/sema.tr"
    if (((((((((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("isize", 5LL))))) {
#line 748 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("int", 3LL);
    }
#line 749 "src/sema.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))))) {
#line 749 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("float", 5LL);
    }
#line 750 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) {
#line 750 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("bool", 4LL);
    }
#line 751 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL)))) {
#line 751 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("str", 3LL);
    }
#line 752 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))))) {
#line 752 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("List", 4LL);
    }
#line 753 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
#line 753 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("Dict", 4LL);
    }
#line 754 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("tuple", 5LL))))) {
#line 754 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("Tuple", 5LL);
    }
#line 755 "src/sema.tr"
    long long gi = 0LL;
#line 756 "src/sema.tr"
    while ((gi < self->current_func_generics->len)) {
#line 757 "src/sema.tr"
        if (({ TrStr _cmpa_t1026 = (List_TrStr_get(self->current_func_generics, gi)); TrStr _cmpb_t1027 = (n); _Bool _cmpr_t1028 = _tr_str_eqv(_cmpa_t1026, _cmpb_t1027); _tr_str_release(_cmpa_t1026); _cmpr_t1028; })) {
#line 757 "src/sema.tr"
            _tr_str_release(n);
            return _tr_str_lit_len("", 0LL);
        }
#line 758 "src/sema.tr"
        gi = (gi + 1LL);
    }
#line 759 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
#line 760 "src/sema.tr"
        ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)));
#line 761 "src/sema.tr"
        if ((((cd->generics->len == 0LL) && (cd->iface_names->len == 0LL)) && (cd->base_classes->len == 0LL))) {
#line 761 "src/sema.tr"
            _tr_str_release(n);
            return _tr_str_lit_len("class", 5LL);
        }
#line 762 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("", 0LL);
    }
#line 763 "src/sema.tr"
    if (((_tr_dict_contains(self->enums, _tr_strz(n)) && (!_tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL))))) && (!_tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))))) {
#line 763 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("class", 5LL);
    }
#line 764 "src/sema.tr"
    if (((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
#line 764 "src/sema.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("class", 5LL);
    }
#line 765 "src/sema.tr"
    _tr_str_release(n);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Sema_firm_family(Sema* self, HirExpr* e) {
#line 768 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 768 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 769 "src/sema.tr"
    __auto_type _t1029 = (*e);
    if (_t1029.tag == HirExpr_ELitInt) {
        return _tr_str_lit_len("int", 3LL);
    } else if (_t1029.tag == HirExpr_ELitFloat) {
        return _tr_str_lit_len("float", 5LL);
    } else if (_t1029.tag == HirExpr_ELitStr) {
        return _tr_str_lit_len("str", 3LL);
    } else if (_t1029.tag == HirExpr_ELitBool) {
        return _tr_str_lit_len("bool", 4LL);
    } else if (_t1029.tag == HirExpr_ELitNone) {
        __auto_type _ = _t1029.data.ELitNone.ty;
        return _tr_str_lit_len("None", 4LL);
    } else if (_t1029.tag == HirExpr_EFString) {
        return _tr_str_lit_len("str", 3LL);
    } else if (_t1029.tag == HirExpr_EList) {
        return _tr_str_lit_len("List", 4LL);
    } else if (_t1029.tag == HirExpr_EDict) {
        return _tr_str_lit_len("Dict", 4LL);
    } else if (_t1029.tag == HirExpr_ETuple) {
        return _tr_str_lit_len("Tuple", 5LL);
    } else if (_t1029.tag == HirExpr_EIdent) {
        __auto_type nm = _t1029.data.EIdent.name;
#line 780 "src/sema.tr"
        Symbol* sy = Sema_resolve(self, nm);
#line 781 "src/sema.tr"
        if (((!_tr_str_eqv((sy->name), (_tr_str_lit_len("", 0LL)))) && (sy->kind.tag == SymbolKind_make_SVariable().tag))) {
#line 781 "src/sema.tr"
            TrStr _trret186 = _tr_str_retain(sy->firm);
            return _trret186;
        }
#line 782 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t1029.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t1029.data.EUnaryOp.op;
__auto_type x = _t1029.data.EUnaryOp.expr;
#line 784 "src/sema.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("not", 3LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!", 1LL))))) {
#line 784 "src/sema.tr"
            return _tr_str_lit_len("bool", 4LL);
        }
#line 785 "src/sema.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))))) {
#line 786 "src/sema.tr"
            TrStr xf = Sema_firm_family(self, x);
#line 787 "src/sema.tr"
            if ((_tr_str_eqv((xf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((xf), (_tr_str_lit_len("float", 5LL))))) {
#line 787 "src/sema.tr"
                TrStr _trret187 = _tr_str_retain(xf);
                _tr_str_release(xf);
                return _trret187;
            }
        }
#line 788 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t1029.tag == HirExpr_EBinOp) {
        __auto_type op = _t1029.data.EBinOp.op;
__auto_type l = _t1029.data.EBinOp.left;
__auto_type r = _t1029.data.EBinOp.right;
#line 790 "src/sema.tr"
        if ((((((((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("or", 2LL))))) {
#line 790 "src/sema.tr"
            return _tr_str_lit_len("bool", 4LL);
        }
#line 791 "src/sema.tr"
        TrStr lf = Sema_firm_family(self, l);
#line 792 "src/sema.tr"
        TrStr rf = Sema_firm_family(self, r);
#line 793 "src/sema.tr"
        if (((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) && _tr_str_eqv((lf), (_tr_str_lit_len("str", 3LL)))) && _tr_str_eqv((rf), (_tr_str_lit_len("str", 3LL))))) {
#line 793 "src/sema.tr"
            _tr_str_release(lf);
            _tr_str_release(rf);
            return _tr_str_lit_len("str", 3LL);
        }
#line 794 "src/sema.tr"
        if ((((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL))))) {
#line 795 "src/sema.tr"
            if (((_tr_str_eqv((lf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((lf), (_tr_str_lit_len("float", 5LL)))) && (_tr_str_eqv((rf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((rf), (_tr_str_lit_len("float", 5LL)))))) {
#line 796 "src/sema.tr"
                if ((_tr_str_eqv((lf), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((rf), (_tr_str_lit_len("float", 5LL))))) {
#line 796 "src/sema.tr"
                    _tr_str_release(lf);
                    _tr_str_release(rf);
                    return _tr_str_lit_len("float", 5LL);
                }
#line 797 "src/sema.tr"
                _tr_str_release(lf);
                _tr_str_release(rf);
                return _tr_str_lit_len("int", 3LL);
            }
        }
#line 798 "src/sema.tr"
        _tr_str_release(lf);
        _tr_str_release(rf);
        return _tr_str_lit_len("", 0LL);
    } else if (_t1029.tag == HirExpr_EMethodCall) {
        __auto_type mo = _t1029.data.EMethodCall.obj;
__auto_type mm = _t1029.data.EMethodCall.method;
__auto_type margs = _t1029.data.EMethodCall.args;
#line 800 "src/sema.tr"
        if (((_tr_str_eqv((mm), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((mm), (_tr_str_lit_len("to_string", 9LL)))) && (margs->len == 0LL))) {
#line 801 "src/sema.tr"
            TrStr mty = _tr_str_retain(hir_expr_type(mo)->name);
#line 802 "src/sema.tr"
            if ((!(_tr_dict_contains(self->classes, _tr_strz(mty)) && Sema_class_method_exists(self, mty, mm)))) {
#line 802 "src/sema.tr"
                _tr_str_release(mty);
                return _tr_str_lit_len("str", 3LL);
            }
            _tr_str_release(mty);
        }
#line 803 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t1029.tag == HirExpr_ECall) {
        __auto_type callee = _t1029.data.ECall.callee;
#line 805 "src/sema.tr"
        __auto_type _t1030 = (*callee);
        if (_t1030.tag == HirExpr_EIdent) {
            __auto_type fn = _t1030.data.EIdent.name;
#line 807 "src/sema.tr"
            if (_tr_dict_contains(self->fn_defs, _tr_strz(fn))) {
#line 808 "src/sema.tr"
                FunctionDef* fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(fn)));
#line 809 "src/sema.tr"
                if ((((fd->generics->len > 0LL) || fd->is_extern) || (((unsigned long long)(fd->ret_ty)) == ((unsigned long long)(0LL))))) {
#line 809 "src/sema.tr"
                    return _tr_str_lit_len("", 0LL);
                }
#line 810 "src/sema.tr"
                if (((((unsigned long long)(fd->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*fd->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 810 "src/sema.tr"
                    return _tr_str_lit_len("", 0LL);
                }
#line 811 "src/sema.tr"
                TrStr fam = Sema_decl_family(self, (*fd->ret_ty));
#line 812 "src/sema.tr"
                if (_tr_str_eqv((fam), (_tr_str_lit_len("class", 5LL)))) {
#line 812 "src/sema.tr"
                    _tr_str_release(fam);
                    return _tr_str_lit_len("", 0LL);
                }
#line 813 "src/sema.tr"
                TrStr _trret188 = _tr_str_retain(fam);
                _tr_str_release(fam);
                return _trret188;
            }
#line 814 "src/sema.tr"
            if (_tr_str_eqv((Sema_resolve(self, fn)->name), (_tr_str_lit_len("", 0LL)))) {
#line 815 "src/sema.tr"
                if (_tr_str_eqv((fn), (_tr_str_lit_len("str", 3LL)))) {
#line 815 "src/sema.tr"
                    return _tr_str_lit_len("str", 3LL);
                }
#line 816 "src/sema.tr"
                if ((_tr_str_eqv((fn), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("len", 3LL))))) {
#line 816 "src/sema.tr"
                    return _tr_str_lit_len("int", 3LL);
                }
#line 817 "src/sema.tr"
                if (_tr_str_eqv((fn), (_tr_str_lit_len("float", 5LL)))) {
#line 817 "src/sema.tr"
                    return _tr_str_lit_len("float", 5LL);
                }
            }
#line 818 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        } else if (1) {
            __auto_type _ = _t1030;
            return _tr_str_lit_len("", 0LL);
        }
    } else if (1) {
        __auto_type _ = _t1029;
        return _tr_str_lit_len("", 0LL);
    }
#line 821 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool Sema_family_mismatch(Sema* self, TrStr exp, TrStr act) {
#line 824 "src/sema.tr"
    if ((_tr_str_eqv((exp), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((act), (_tr_str_lit_len("", 0LL))))) {
#line 824 "src/sema.tr"
        return false;
    }
#line 825 "src/sema.tr"
    bool exp_scalar = (((_tr_str_eqv((exp), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((exp), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("str", 3LL))));
#line 826 "src/sema.tr"
    bool act_scalar = (((_tr_str_eqv((act), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((act), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((act), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((act), (_tr_str_lit_len("str", 3LL))));
#line 827 "src/sema.tr"
    if ((exp_scalar && act_scalar)) {
#line 827 "src/sema.tr"
        return (_tr_str_eqv((exp), (_tr_str_lit_len("str", 3LL))) != _tr_str_eqv((act), (_tr_str_lit_len("str", 3LL))));
    }
#line 828 "src/sema.tr"
    if ((exp_scalar && (_tr_str_eqv((act), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((act), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 828 "src/sema.tr"
        return true;
    }
#line 829 "src/sema.tr"
    if ((act_scalar && ((_tr_str_eqv((exp), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((exp), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("class", 5LL)))))) {
#line 829 "src/sema.tr"
        return true;
    }
#line 830 "src/sema.tr"
    if (((_tr_str_eqv((exp), (_tr_str_lit_len("List", 4LL))) && _tr_str_eqv((act), (_tr_str_lit_len("Dict", 4LL)))) || (_tr_str_eqv((exp), (_tr_str_lit_len("Dict", 4LL))) && _tr_str_eqv((act), (_tr_str_lit_len("List", 4LL)))))) {
#line 830 "src/sema.tr"
        return true;
    }
#line 831 "src/sema.tr"
    if ((_tr_str_eqv((exp), (_tr_str_lit_len("class", 5LL))) && (_tr_str_eqv((act), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((act), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 831 "src/sema.tr"
        return true;
    }
#line 832 "src/sema.tr"
    if ((_tr_str_eqv((exp), (_tr_str_lit_len("Tuple", 5LL))) && (!_tr_str_eqv((act), (_tr_str_lit_len("Tuple", 5LL)))))) {
#line 832 "src/sema.tr"
        return ((act_scalar || _tr_str_eqv((act), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((act), (_tr_str_lit_len("Dict", 4LL))));
    }
#line 833 "src/sema.tr"
    if ((_tr_str_eqv((act), (_tr_str_lit_len("Tuple", 5LL))) && (!_tr_str_eqv((exp), (_tr_str_lit_len("Tuple", 5LL)))))) {
#line 833 "src/sema.tr"
        return (((exp_scalar || _tr_str_eqv((exp), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((exp), (_tr_str_lit_len("class", 5LL))));
    }
#line 834 "src/sema.tr"
    return false;
}

__attribute__((hot)) TrStr Sema_family_word(Sema* self, TrStr fam) {
#line 837 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("class", 5LL)))) {
#line 837 "src/sema.tr"
        return _tr_str_lit_len("a class/enum value", 18LL);
    }
#line 838 "src/sema.tr"
    return _tr_strx_concatv((_tr_str_lit_len("a ", 2LL)), (fam));
}

__attribute__((hot)) void Sema_check_expr_assignable(Sema* self, AstType* exp_ty, HirExpr* e, TrStr what) {
#line 842 "src/sema.tr"
    if (((((_tr_str_eqv((exp_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((exp_ty->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((exp_ty->name), (_tr_str_lit_len("Set", 3LL)))) && (exp_ty->args->len > 0LL)) && (!_is_invalid_ptr(((unsigned long long)(e)))))) {
#line 843 "src/sema.tr"
        AstType* elt = (*((AstType**)List_ptr_get(exp_ty->args, 0LL)));
#line 844 "src/sema.tr"
        __auto_type _t1031 = (*e);
        if (_t1031.tag == HirExpr_EList) {
            __auto_type items = _t1031.data.EList.items;
#line 846 "src/sema.tr"
            long long li = 0LL;
#line 847 "src/sema.tr"
            while ((li < items->len)) {
#line 848 "src/sema.tr"
                if (({ TrStr _ord_t1032 = (Sema_decl_family(self, elt)); TrStr _ord_t1033 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(items, li)))); __auto_type _wr = (Sema_family_mismatch(self, _ord_t1032, _ord_t1033)); _tr_str_release(_ord_t1032); _tr_str_release(_ord_t1033); _wr; })) {
#line 849 "src/sema.tr"
                    ({ __auto_type _ord_t1034 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: ", 21LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is a ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (exp_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (elt->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] but its element ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1036 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1034); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((li + 1LL))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1035 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(items, li)))); __auto_type _ord_t1037 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1036); TrStr _tr_cr = (Sema_family_word(self, _at_t1035)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: make every element a '", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1038 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1037); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (elt->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1038); _tr_str_release(_at_t1035); _tr_str_release(_at_t1038); });
#line 850 "src/sema.tr"
                    break;
                }
#line 851 "src/sema.tr"
                li = (li + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t1031;
            /* pass */
        }
    }
#line 854 "src/sema.tr"
    TrStr ef = Sema_decl_family(self, exp_ty);
#line 855 "src/sema.tr"
    TrStr af = Sema_firm_family(self, e);
#line 856 "src/sema.tr"
    if (Sema_family_mismatch(self, ef, af)) {
#line 857 "src/sema.tr"
        ({ __auto_type _ord_t1039 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: ", 21LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" expects '", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (exp_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but is given ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1040 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1039); TrStr _tr_cr = (Sema_family_word(self, af)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass a value of type '", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1041 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1040); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (exp_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (convert it explicitly, e.g. str(x) / int(x) / float(x), if that is what you mean).", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1041); _tr_str_release(_at_t1041); });
    }
    _tr_str_release(ef);
    _tr_str_release(af);
}

__attribute__((hot)) void Sema_check_call_args(Sema* self, FunctionDef* fd, List_ptr* args, long long sk, TrStr what) {
#line 863 "src/sema.tr"
    long long pi = sk;
#line 864 "src/sema.tr"
    while ((pi < fd->params->len)) {
#line 865 "src/sema.tr"
        if (((Param*)List_ptr_get(fd->params, pi))->is_variadic) {
#line 865 "src/sema.tr"
            return;
        }
#line 866 "src/sema.tr"
        pi = (pi + 1LL);
    }
#line 867 "src/sema.tr"
    long long want = (fd->params->len - sk);
#line 868 "src/sema.tr"
    if ((want != args->len)) {
#line 869 "src/sema.tr"
        TrStr plural = _tr_str_lit_len("s", 1LL);
#line 870 "src/sema.tr"
        if ((want == 1LL)) {
#line 870 "src/sema.tr"
            TrStr _strtmp_t1042 = _tr_str_lit_len("", 0LL);
            _tr_str_release(plural);
            plural = _strtmp_t1042;
        }
#line 871 "src/sema.tr"
        TrStr verb = _tr_str_lit_len("were", 4LL);
#line 872 "src/sema.tr"
        if ((args->len == 1LL)) {
#line 872 "src/sema.tr"
            TrStr _strtmp_t1043 = _tr_str_lit_len("was", 3LL);
            _tr_str_release(verb);
            verb = _strtmp_t1043;
        }
#line 873 "src/sema.tr"
        ({ __auto_type _ord_t1044 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" takes ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (plural)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1045 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1044); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(args->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (verb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1046 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1045); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (plural)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (see its definition).", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1046); _tr_str_release(_at_t1046); });
#line 874 "src/sema.tr"
        _tr_str_release(plural);
        _tr_str_release(verb);
        return;
    }
#line 875 "src/sema.tr"
    if ((fd->generics->len > 0LL)) {
#line 875 "src/sema.tr"
        return;
    }
#line 876 "src/sema.tr"
    long long ai = 0LL;
#line 877 "src/sema.tr"
    while ((ai < args->len)) {
#line 878 "src/sema.tr"
        AstType** pty = ((Param*)List_ptr_get(fd->params, (sk + ai)))->ty;
#line 879 "src/sema.tr"
        if ((((unsigned long long)(pty)) != ((unsigned long long)(0LL)))) {
#line 880 "src/sema.tr"
            ({ __auto_type _ord_t1047 = ((*pty)); __auto_type _ord_t1048 = (((HirExpr*)List_ptr_get(args, ai))); TrStr _ord_t1049 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((ai + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (what)); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1047, _ord_t1048, _ord_t1049); _tr_str_release(_ord_t1049); });
        }
#line 881 "src/sema.tr"
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) long long Sema_builtin_method_range(Sema* self, TrStr fam, TrStr method) {
#line 886 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("str", 3LL)))) {
#line 887 "src/sema.tr"
        if ((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("upper", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("lower", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_upper", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_lower", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("strip", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("trim", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("trim_left", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("trim_right", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("capitalize", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("title", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL))))) {
#line 887 "src/sema.tr"
            return 0LL;
        }
#line 888 "src/sema.tr"
        if ((((((((((_tr_str_eqv((method), (_tr_str_lit_len("find", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("starts_with", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("ends_with", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("split", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("split_exact", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("repeat", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("strip_prefix", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("strip_suffix", 12LL))))) {
#line 888 "src/sema.tr"
            return 101LL;
        }
#line 889 "src/sema.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("replace", 7LL)))) {
#line 889 "src/sema.tr"
            return 202LL;
        }
    } else if (_tr_str_eqv((fam), (_tr_str_lit_len("List", 4LL)))) {
#line 891 "src/sema.tr"
        if ((((((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("last", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max_val", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("min_val", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sum", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reversed", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reversed_copy", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_asc", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_desc", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_list", 7LL))))) {
#line 891 "src/sema.tr"
            return 0LL;
        }
#line 892 "src/sema.tr"
        if (((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("append", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("push", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("reserve", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_by", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("union", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("intersection", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("difference", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_subset", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("all", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("any", 3LL))))) {
#line 892 "src/sema.tr"
            return 101LL;
        }
#line 893 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("swap", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL))))) {
#line 893 "src/sema.tr"
            return 202LL;
        }
#line 894 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("sort", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("count", 5LL))))) {
#line 894 "src/sema.tr"
            return 1LL;
        }
    } else if (_tr_str_eqv((fam), (_tr_str_lit_len("Dict", 4LL)))) {
#line 896 "src/sema.tr"
        if ((((((((_tr_str_eqv((method), (_tr_str_lit_len("keys", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("values", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("items", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL))))) {
#line 896 "src/sema.tr"
            return 0LL;
        }
#line 897 "src/sema.tr"
        if (((((_tr_str_eqv((method), (_tr_str_lit_len("has", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("update", 6LL))))) {
#line 897 "src/sema.tr"
            return 101LL;
        }
#line 898 "src/sema.tr"
        if (((_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL))))) {
#line 898 "src/sema.tr"
            return 202LL;
        }
#line 899 "src/sema.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) {
#line 899 "src/sema.tr"
            return 102LL;
        }
    }
#line 900 "src/sema.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) void Sema_check_builtin_method_arity(Sema* self, TrStr fam, TrStr method, long long nargs) {
#line 903 "src/sema.tr"
    if ((((!_tr_str_eqv((fam), (_tr_str_lit_len("str", 3LL)))) && (!_tr_str_eqv((fam), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((fam), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 903 "src/sema.tr"
        return;
    }
#line 904 "src/sema.tr"
    long long r = Sema_builtin_method_range(self, fam, method);
#line 905 "src/sema.tr"
    if ((r < 0LL)) {
#line 905 "src/sema.tr"
        return;
    }
#line 906 "src/sema.tr"
    long long lo = _tr_idiv((long long)(r), (long long)(100LL));
#line 907 "src/sema.tr"
    long long hi = _tr_imod((long long)(r), (long long)(100LL));
#line 908 "src/sema.tr"
    if (((nargs >= lo) && (nargs <= hi))) {
#line 908 "src/sema.tr"
        return;
    }
#line 909 "src/sema.tr"
    TrStr pl = _tr_str_lit_len("s", 1LL);
#line 910 "src/sema.tr"
    TrStr want_s = _tr_str_wrap(_tr_int_to_str((long long)(lo)));
#line 911 "src/sema.tr"
    if ((lo != hi)) {
#line 911 "src/sema.tr"
        TrStr _strtmp_t1051 = ({ __auto_type _ord_t1050 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(lo)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t1050); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        _tr_str_release(want_s);
        want_s = _strtmp_t1051;
    }
#line 912 "src/sema.tr"
    if (((lo == 1LL) && (hi == 1LL))) {
#line 912 "src/sema.tr"
        TrStr _strtmp_t1052 = _tr_str_lit_len("", 0LL);
        _tr_str_release(pl);
        pl = _strtmp_t1052;
    }
#line 913 "src/sema.tr"
    TrStr vb = _tr_str_lit_len("were", 4LL);
#line 914 "src/sema.tr"
    if ((nargs == 1LL)) {
#line 914 "src/sema.tr"
        TrStr _strtmp_t1053 = _tr_str_lit_len("was", 3LL);
        _tr_str_release(vb);
        vb = _strtmp_t1053;
    }
#line 915 "src/sema.tr"
    ({ TrStr _at_t1054 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (fam))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (want_s)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(nargs)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: see the built-in method list in the language docs.", 69LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1054); _tr_str_release(_at_t1054); });
    _tr_str_release(pl);
    _tr_str_release(want_s);
    _tr_str_release(vb);
}

__attribute__((hot)) bool Sema_class_has_field(Sema* self, TrStr cls_name, TrStr name) {
#line 919 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
#line 919 "src/sema.tr"
        return false;
    }
#line 920 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
#line 921 "src/sema.tr"
    long long fi = 0LL;
#line 922 "src/sema.tr"
    while ((fi < cls->fields->len)) {
#line 923 "src/sema.tr"
        if (_tr_str_eqv((((FieldDef*)List_ptr_get(cls->fields, fi))->name), (name))) {
#line 923 "src/sema.tr"
            return true;
        }
#line 924 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 925 "src/sema.tr"
    long long bi = 0LL;
#line 926 "src/sema.tr"
    while ((bi < cls->base_classes->len)) {
#line 927 "src/sema.tr"
        if (({ TrStr _at_t1055 = (List_TrStr_get(cls->base_classes, bi)); __auto_type _wr = (Sema_class_has_field(self, _at_t1055, name)); _tr_str_release(_at_t1055); _wr; })) {
#line 927 "src/sema.tr"
            return true;
        }
#line 928 "src/sema.tr"
        bi = (bi + 1LL);
    }
#line 929 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_rt_builtin_class(Sema* self, TrStr n) {
#line 933 "src/sema.tr"
    return ((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncTask", 9LL))));
}

__attribute__((hot)) bool Sema_is_builtin_dispatch_class(Sema* self, TrStr n) {
#line 936 "src/sema.tr"
    return (((((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("OS", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Process", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Env", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Hash", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("File", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncTask", 9LL))));
}

__attribute__((hot)) bool Sema_type_name_known(Sema* self, TrStr n) {
#line 940 "src/sema.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((n), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("None", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Self", 4LL))))) {
#line 940 "src/sema.tr"
        return true;
    }
#line 941 "src/sema.tr"
    if (_is_builtin_type_word(n)) {
#line 941 "src/sema.tr"
        return true;
    }
#line 942 "src/sema.tr"
    if (Sema_is_known_name(self, n)) {
#line 942 "src/sema.tr"
        return true;
    }
#line 943 "src/sema.tr"
    if ((!_tr_str_eqv((Sema_resolve(self, n)->name), (_tr_str_lit_len("", 0LL))))) {
#line 943 "src/sema.tr"
        return true;
    }
#line 944 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema_check_type_known(Sema* self, AstType** tp, TrStr where_) {
#line 947 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(tp)))) {
#line 947 "src/sema.tr"
        return;
    }
#line 948 "src/sema.tr"
    AstType* t = (*tp);
#line 949 "src/sema.tr"
    if ((!Sema_type_name_known(self, t->name))) {
#line 950 "src/sema.tr"
        ({ TrStr _at_t1056 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-4] Unknown type '", 20LL)), (t->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (in ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (where_)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").\n      FIX: check the spelling, or import it (e.g. `from std.x import ", 72LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("`).", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1056); _tr_str_release(_at_t1056); });
#line 951 "src/sema.tr"
        return;
    }
#line 952 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(t->args)))) {
#line 952 "src/sema.tr"
        return;
    }
#line 953 "src/sema.tr"
    long long ai = 0LL;
#line 954 "src/sema.tr"
    while ((ai < t->args->len)) {
#line 955 "src/sema.tr"
        if ((_tr_str_eqv(((*((AstType**)List_ptr_get(t->args, ai)))->name), (_tr_str_lit_len("void", 4LL))) && (((((_tr_str_eqv((t->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Option", 6LL)))))) {
#line 956 "src/sema.tr"
            ({ TrStr _at_t1057 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-4] 'void' is not a valid element type for ", 45LL)), (t->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (in ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (where_)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").\n      FIX: name the element type, e.g. ", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[int].", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1057); _tr_str_release(_at_t1057); });
        }
#line 957 "src/sema.tr"
        Sema_check_type_known(self, ((AstType**)List_ptr_get(t->args, ai)), where_);
#line 958 "src/sema.tr"
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) void Sema_check_scalar_method(Sema* self, TrStr fam, TrStr method, TrStr ty_name) {
#line 962 "src/sema.tr"
    if (Sema_class_method_exists(self, ty_name, method)) {
#line 962 "src/sema.tr"
        return;
    }
#line 963 "src/sema.tr"
    if ((_tr_str_eqv((fam), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fam), (_tr_str_lit_len("Dict", 4LL))))) {
#line 964 "src/sema.tr"
        if (Sema_is_universal_method(self, method)) {
#line 964 "src/sema.tr"
            return;
        }
#line 965 "src/sema.tr"
        if (Sema_known_collection_method(self, fam, method)) {
#line 965 "src/sema.tr"
            return;
        }
#line 966 "src/sema.tr"
        ({ TrStr _at_t1058 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, fam)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-6] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" has no method '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'.\n      FIX: check the spelling, or that the receiver is the collection you meant.", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1058); _tr_str_release(_at_t1058); });
#line 967 "src/sema.tr"
        return;
    }
#line 968 "src/sema.tr"
    if ((_tr_str_eqv((fam), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((fam), (_tr_str_lit_len("float", 5LL))))) {
#line 969 "src/sema.tr"
        if ((Sema_is_universal_method(self, method) || Sema_known_number_method(self, method))) {
#line 969 "src/sema.tr"
            return;
        }
#line 970 "src/sema.tr"
        ({ TrStr _at_t1059 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, fam)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-6] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" has no method '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'.\n      FIX: check that the receiver is the value you meant; numbers only have methods such as abs(), pow() and to_str().", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1059); _tr_str_release(_at_t1059); });
#line 971 "src/sema.tr"
        return;
    }
#line 972 "src/sema.tr"
    if (_tr_str_eqv((fam), (_tr_str_lit_len("None", 4LL)))) {
#line 973 "src/sema.tr"
        ({ TrStr _at_t1060 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-6] None has no method '", 26LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'.\n      FIX: the variable holds None here; give it a real value, or use an Option[T] and check .is_some first.", 113LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1060); _tr_str_release(_at_t1060); });
#line 974 "src/sema.tr"
        return;
    }
#line 975 "src/sema.tr"
    if (((!_tr_str_eqv((fam), (_tr_str_lit_len("bool", 4LL)))) && (!_tr_str_eqv((fam), (_tr_str_lit_len("Tuple", 5LL)))))) {
#line 975 "src/sema.tr"
        return;
    }
#line 976 "src/sema.tr"
    if (Sema_is_universal_method(self, method)) {
#line 976 "src/sema.tr"
        return;
    }
#line 977 "src/sema.tr"
    if ((_tr_str_eqv((fam), (_tr_str_lit_len("Tuple", 5LL))) && (((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("last", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("count", 5LL)))))) {
#line 977 "src/sema.tr"
        return;
    }
#line 978 "src/sema.tr"
    ({ __auto_type _ord_t1061 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, fam)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-6] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" has no method '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'.\n      FIX: check that the receiver is the object you meant (this one is ", 77LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1062 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1061); TrStr _tr_cr = (Sema_family_word(self, fam)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1062); _tr_str_release(_at_t1062); });
}

__attribute__((hot)) void Sema_check_static_call(Sema* self, TrStr type_name, TrStr variant, List_ptr* hl) {
#line 982 "src/sema.tr"
    if ((_tr_str_eqv((type_name), (_tr_str_lit_len("Atomic", 6LL))) && (_tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL)))))) {
#line 983 "src/sema.tr"
        if ((hl->len != 1LL)) {
#line 984 "src/sema.tr"
            ({ TrStr _at_t1063 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] 'Atomic.", 14LL)), (variant))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes 1 argument but ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" were given.\n      FIX: Atomic.new(initial_value).", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1063); _tr_str_release(_at_t1063); });
        } else {
#line 986 "src/sema.tr"
            TrStr af = Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)));
#line 987 "src/sema.tr"
            if (((_tr_str_eqv((af), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((af), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((af), (_tr_str_lit_len("Dict", 4LL))))) {
#line 988 "src/sema.tr"
                ({ TrStr _at_t1064 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, af)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: Atomic needs a number as its initial value but is given ", 77LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: Atomic.new(0).", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1064); _tr_str_release(_at_t1064); });
            }
        }
    }
#line 989 "src/sema.tr"
    if ((_tr_dict_contains(self->ambiguous_classes, _tr_strz(type_name)) || Sema_is_rt_builtin_class(self, type_name))) {
#line 989 "src/sema.tr"
        return;
    }
#line 990 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(type_name)))) {
#line 990 "src/sema.tr"
        return;
    }
#line 991 "src/sema.tr"
    ClassDef* cdef = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(type_name)));
#line 992 "src/sema.tr"
    long long named = 0LL;
#line 993 "src/sema.tr"
    bool ok = false;
#line 994 "src/sema.tr"
    long long want = 0LL;
#line 995 "src/sema.tr"
    bool variadic = false;
#line 996 "src/sema.tr"
    FunctionDef* one = FunctionDef_init(_tr_str_lit_len("", 0LL));
#line 997 "src/sema.tr"
    long long i = 0LL;
#line 998 "src/sema.tr"
    while ((i < cdef->methods->len)) {
#line 999 "src/sema.tr"
        FunctionDef* m = ((FunctionDef*)List_ptr_get(cdef->methods, i));
#line 1000 "src/sema.tr"
        if (_tr_str_eqv((m->name), (variant))) {
#line 1001 "src/sema.tr"
            bool has_self = ((m->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(m->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))));
#line 1002 "src/sema.tr"
            long long cnt = m->params->len;
#line 1003 "src/sema.tr"
            if (has_self) {
#line 1003 "src/sema.tr"
                cnt = (cnt - 1LL);
            }
#line 1004 "src/sema.tr"
            long long pj = 0LL;
#line 1005 "src/sema.tr"
            while ((pj < m->params->len)) {
#line 1006 "src/sema.tr"
                if (((Param*)List_ptr_get(m->params, pj))->is_variadic) {
#line 1006 "src/sema.tr"
                    variadic = true;
                }
#line 1007 "src/sema.tr"
                pj = (pj + 1LL);
            }
#line 1008 "src/sema.tr"
            if ((named == 0LL)) {
#line 1008 "src/sema.tr"
                want = cnt;
            }
#line 1009 "src/sema.tr"
            named = (named + 1LL);
#line 1010 "src/sema.tr"
            if (((cnt == hl->len) || (has_self && ((cnt + 1LL) == hl->len)))) {
#line 1010 "src/sema.tr"
                ok = true;
            }
#line 1011 "src/sema.tr"
            FunctionDef* _cltmp_t1065 = _tr_obj_retain(m);
            _tr_obj_release(one, _trdrop_FunctionDef);
            one = _cltmp_t1065;
        }
#line 1012 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1013 "src/sema.tr"
    if (((named == 0LL) || variadic)) {
#line 1013 "src/sema.tr"
        _tr_obj_release(one, _trdrop_FunctionDef);
        return;
    }
#line 1014 "src/sema.tr"
    if ((!ok)) {
#line 1015 "src/sema.tr"
        TrStr pl = _tr_str_lit_len("s", 1LL);
#line 1016 "src/sema.tr"
        if ((want == 1LL)) {
#line 1016 "src/sema.tr"
            TrStr _strtmp_t1066 = _tr_str_lit_len("", 0LL);
            _tr_str_release(pl);
            pl = _strtmp_t1066;
        }
#line 1017 "src/sema.tr"
        TrStr vb = _tr_str_lit_len("were", 4LL);
#line 1018 "src/sema.tr"
        if ((hl->len == 1LL)) {
#line 1018 "src/sema.tr"
            TrStr _strtmp_t1067 = _tr_str_lit_len("was", 3LL);
            _tr_str_release(vb);
            vb = _strtmp_t1067;
        }
#line 1019 "src/sema.tr"
        ({ __auto_type _ord_t1068 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (type_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (variant)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1069 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1068); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1070 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1069); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (see its definition).", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1070); _tr_str_release(_at_t1070); });
        _tr_str_release(pl);
        _tr_str_release(vb);
    } else if (((((named == 1LL) && (cdef->generics->len == 0LL)) && (one->generics->len == 0LL)) && (one->params->len == hl->len))) {
#line 1021 "src/sema.tr"
        long long ti = 0LL;
#line 1022 "src/sema.tr"
        while ((ti < hl->len)) {
#line 1023 "src/sema.tr"
            if ((((unsigned long long)(((Param*)List_ptr_get(one->params, ti))->ty)) != ((unsigned long long)(0LL)))) {
#line 1024 "src/sema.tr"
                ({ __auto_type _ord_t1071 = ((*((Param*)List_ptr_get(one->params, ti))->ty)); __auto_type _ord_t1072 = (((HirExpr*)List_ptr_get(hl, ti))); TrStr _ord_t1073 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((ti + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (type_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (variant)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1071, _ord_t1072, _ord_t1073); _tr_str_release(_ord_t1073); });
            }
#line 1025 "src/sema.tr"
            ti = (ti + 1LL);
        }
    }
    _tr_obj_release(one, _trdrop_FunctionDef);
}

__attribute__((hot)) HirExpr* Sema_lower_index_operand(Sema* self, Expr* idx, HirExpr* base) {
#line 1029 "src/sema.tr"
    bool sv_a = self->in_assign_target;
#line 1030 "src/sema.tr"
    bool sv_p = self->in_assign_target_plain;
#line 1031 "src/sema.tr"
    self->in_assign_target = false;
#line 1032 "src/sema.tr"
    self->in_assign_target_plain = false;
#line 1033 "src/sema.tr"
    HirExpr* r = Sema_lower_expr(self, idx);
#line 1034 "src/sema.tr"
    self->in_assign_target = sv_a;
#line 1035 "src/sema.tr"
    self->in_assign_target_plain = sv_p;
#line 1036 "src/sema.tr"
    if (true) {
#line 1037 "src/sema.tr"
        TrStr bf = Sema_firm_family(self, base);
#line 1038 "src/sema.tr"
        TrStr xf = Sema_firm_family(self, r);
#line 1039 "src/sema.tr"
        AstType* bty = hir_expr_type(base);
#line 1040 "src/sema.tr"
        if ((((_tr_str_eqv((bty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((bty->name), (_tr_str_lit_len("Map", 3LL)))) && (bty->args->len > 0LL)) && ({ TrStr _at_t1074 = (Sema_decl_family(self, (*((AstType**)List_ptr_get(bty->args, 0LL))))); __auto_type _wr = (Sema_family_mismatch(self, _at_t1074, xf)); _tr_str_release(_at_t1074); _wr; }))) {
#line 1041 "src/sema.tr"
            ({ __auto_type _ord_t1075 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: a ", 23LL)), (bty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1076 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1075); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(bty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ...] is indexed with ", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1077 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1076); TrStr _tr_cr = (Sema_family_word(self, xf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: use a '", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1078 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1077); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(bty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' key.", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1078); _tr_str_release(_at_t1078); });
        }
#line 1042 "src/sema.tr"
        if (((_tr_str_eqv((bf), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((bf), (_tr_str_lit_len("str", 3LL)))) && ((_tr_str_eqv((xf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((xf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((xf), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 1043 "src/sema.tr"
            ({ TrStr _at_t1079 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: a ", 23LL)), (bf))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is indexed with ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, xf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", but the index must be an int.\n      FIX: use an integer index (for a dict, declare it as a Dict).", 99LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1079); _tr_str_release(_at_t1079); });
        }
    }
#line 1044 "src/sema.tr"
    return r;
}

__attribute__((hot)) bool Sema_known_collection_method(Sema* self, TrStr fam, TrStr m) {
#line 1049 "src/sema.tr"
    if (((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("add", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("all", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("any", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clone", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("copy", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("count", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("difference", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))))) {
#line 1049 "src/sema.tr"
        return true;
    }
#line 1050 "src/sema.tr"
    if ((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("has", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("index_of", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("init", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("intersection", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("is_empty", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("is_subset", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("join", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("last", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("len", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("length", 6LL))))) {
#line 1050 "src/sema.tr"
        return true;
    }
#line 1051 "src/sema.tr"
    if ((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("max", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("max_val", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("min_val", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reserve", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reverse", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reversed", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reversed_copy", 13LL))))) {
#line 1051 "src/sema.tr"
        return true;
    }
#line 1052 "src/sema.tr"
    if ((((((((((_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("slice", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort_asc", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort_by", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort_desc", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sum", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("swap", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_list", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("union", 5LL))))) {
#line 1052 "src/sema.tr"
        return true;
    }
#line 1053 "src/sema.tr"
    if ((((((_tr_str_eqv((m), (_tr_str_lit_len("get_index", 9LL))) || _tr_str_eqv((m), (_tr_str_lit_len("get_or", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("items", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("keys", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("values", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("update", 6LL))))) {
#line 1053 "src/sema.tr"
        return true;
    }
#line 1054 "src/sema.tr"
    if (((((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("map", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("filter", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("each", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("for_each", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fold", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reduce", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("find", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sorted", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("enumerate", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("zip", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("take", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("skip", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("flat_map", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("find_index", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("position", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("partition", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("group_by", 8LL))))) {
#line 1054 "src/sema.tr"
        return true;
    }
#line 1055 "src/sema.tr"
    return false;
}

__attribute__((hot)) long long Sema_builtin_class_method_arity(Sema* self, TrStr cls, TrStr m) {
#line 1060 "src/sema.tr"
    if (_tr_str_eqv((cls), (_tr_str_lit_len("Chan", 4LL)))) {
#line 1061 "src/sema.tr"
        if (((((((_tr_str_eqv((m), (_tr_str_lit_len("cap", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("close", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("is_closed", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("len", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("recv", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("try_recv", 8LL))))) {
#line 1061 "src/sema.tr"
            return 0LL;
        }
#line 1062 "src/sema.tr"
        if (((_tr_str_eqv((m), (_tr_str_lit_len("recv_timeout", 12LL))) || _tr_str_eqv((m), (_tr_str_lit_len("send", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("try_send", 8LL))))) {
#line 1062 "src/sema.tr"
            return 1LL;
        }
#line 1063 "src/sema.tr"
        if (_tr_str_eqv((m), (_tr_str_lit_len("send_timeout", 12LL)))) {
#line 1063 "src/sema.tr"
            return 2LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 1065 "src/sema.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("get", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("lock", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("unlock", 6LL))))) {
#line 1065 "src/sema.tr"
            return 0LL;
        }
#line 1066 "src/sema.tr"
        if (_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) {
#line 1066 "src/sema.tr"
            return 1LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 1068 "src/sema.tr"
        if (((((((_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("read", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("read_lock", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("read_unlock", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write_lock", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write_unlock", 12LL))))) {
#line 1068 "src/sema.tr"
            return 0LL;
        }
#line 1069 "src/sema.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL))))) {
#line 1069 "src/sema.tr"
            return 1LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("ThreadLocal", 11LL)))) {
#line 1071 "src/sema.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("get", 3LL))))) {
#line 1071 "src/sema.tr"
            return 0LL;
        }
#line 1072 "src/sema.tr"
        if (_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) {
#line 1072 "src/sema.tr"
            return 1LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("Weak", 4LL)))) {
#line 1074 "src/sema.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("drop", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("is_alive", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("upgrade", 7LL))))) {
#line 1074 "src/sema.tr"
            return 0LL;
        }
    } else if (_tr_str_eqv((cls), (_tr_str_lit_len("Atomic", 6LL)))) {
#line 1076 "src/sema.tr"
        if ((((((((_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("get", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("increment", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("decrement", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("load", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("load_acquire", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("load_relaxed", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("load_seqcst", 11LL))))) {
#line 1076 "src/sema.tr"
            return 0LL;
        }
#line 1077 "src/sema.tr"
        if ((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("add", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("add_acqrel", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add_relaxed", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add_release", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("exchange", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_add", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_add_acqrel", 16LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_add_relaxed", 17LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_add_release", 17LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_sub", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_sub_relaxed", 17LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fetch_sub_release", 17LL))))) {
#line 1077 "src/sema.tr"
            return 1LL;
        }
#line 1078 "src/sema.tr"
        if (((((((((_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("store", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("store_relaxed", 13LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("store_release", 13LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("store_seqcst", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub_relaxed", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub_release", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("swap", 4LL))))) {
#line 1078 "src/sema.tr"
            return 1LL;
        }
#line 1079 "src/sema.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("cas", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("cas_acqrel", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("cas_weak", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("compare_exchange", 16LL))))) {
#line 1079 "src/sema.tr"
            return 2LL;
        }
    }
#line 1080 "src/sema.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) void Sema_check_builtin_class_method(Sema* self, TrStr cls, TrStr method, List_ptr* hl) {
#line 1084 "src/sema.tr"
    if (((((((!_tr_str_eqv((cls), (_tr_str_lit_len("Chan", 4LL)))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("Mutex", 5LL))))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("RwLock", 6LL))))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("ThreadLocal", 11LL))))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("Weak", 4LL))))) && (!_tr_str_eqv((cls), (_tr_str_lit_len("Atomic", 6LL)))))) {
#line 1084 "src/sema.tr"
        return;
    }
#line 1085 "src/sema.tr"
    if ((_tr_dict_contains(self->classes, _tr_strz(cls)) && Sema_class_method_exists(self, cls, method))) {
#line 1085 "src/sema.tr"
        return;
    }
#line 1086 "src/sema.tr"
    if ((Sema_is_universal_method(self, method) && (!_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))))) {
#line 1086 "src/sema.tr"
        return;
    }
#line 1087 "src/sema.tr"
    long long want = Sema_builtin_class_method_arity(self, cls, method);
#line 1088 "src/sema.tr"
    if ((want < 0LL)) {
#line 1089 "src/sema.tr"
        ({ TrStr _at_t1080 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] No method '", 17LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' found on type '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cls)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: check the spelling; see the ", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cls)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" method list in docs/lang/16_concurrency.md.", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1080); _tr_str_release(_at_t1080); });
#line 1090 "src/sema.tr"
        return;
    }
#line 1091 "src/sema.tr"
    if ((want != hl->len)) {
#line 1092 "src/sema.tr"
        TrStr pl = _tr_str_lit_len("s", 1LL);
#line 1093 "src/sema.tr"
        if ((want == 1LL)) {
#line 1093 "src/sema.tr"
            TrStr _strtmp_t1081 = _tr_str_lit_len("", 0LL);
            _tr_str_release(pl);
            pl = _strtmp_t1081;
        }
#line 1094 "src/sema.tr"
        TrStr vb = _tr_str_lit_len("were", 4LL);
#line 1095 "src/sema.tr"
        if ((hl->len == 1LL)) {
#line 1095 "src/sema.tr"
            TrStr _strtmp_t1082 = _tr_str_lit_len("was", 3LL);
            _tr_str_release(vb);
            vb = _strtmp_t1082;
        }
#line 1096 "src/sema.tr"
        ({ __auto_type _ord_t1083 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (cls))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1084 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1083); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1085 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1084); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1085); _tr_str_release(_at_t1085); });
#line 1097 "src/sema.tr"
        _tr_str_release(pl);
        _tr_str_release(vb);
        return;
    }
#line 1098 "src/sema.tr"
    if (_tr_str_eqv((cls), (_tr_str_lit_len("Atomic", 6LL)))) {
#line 1099 "src/sema.tr"
        long long ai = 0LL;
#line 1100 "src/sema.tr"
        while ((ai < hl->len)) {
#line 1101 "src/sema.tr"
            if (((({ TrStr _cmpa_t1086 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, ai)))); TrStr _cmpb_t1087 = (_tr_str_lit_len("str", 3LL)); _Bool _cmpr_t1088 = _tr_str_eqv(_cmpa_t1086, _cmpb_t1087); _tr_str_release(_cmpa_t1086); _cmpr_t1088; }) || ({ TrStr _cmpa_t1089 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, ai)))); TrStr _cmpb_t1090 = (_tr_str_lit_len("List", 4LL)); _Bool _cmpr_t1091 = _tr_str_eqv(_cmpa_t1089, _cmpb_t1090); _tr_str_release(_cmpa_t1089); _cmpr_t1091; })) || ({ TrStr _cmpa_t1092 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, ai)))); TrStr _cmpb_t1093 = (_tr_str_lit_len("Dict", 4LL)); _Bool _cmpr_t1094 = _tr_str_eqv(_cmpa_t1092, _cmpb_t1093); _tr_str_release(_cmpa_t1092); _cmpr_t1094; }))) {
#line 1102 "src/sema.tr"
                ({ __auto_type _ord_t1096 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((ai + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: argument ", 30LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of 'Atomic.", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' must be a number but is ", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1095 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, ai)))); TrStr _at_t1097 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1096); TrStr _tr_cr = (Sema_family_word(self, _at_t1095)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass an int.", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1097); _tr_str_release(_at_t1095); _tr_str_release(_at_t1097); });
            }
#line 1103 "src/sema.tr"
            ai = (ai + 1LL);
        }
    }
}

__attribute__((hot)) void Sema_check_user_method_call(Sema* self, TrStr cls, Expr* obj, TrStr method, List_ptr* hl) {
#line 1108 "src/sema.tr"
    if ((((!_tr_dict_contains(self->classes, _tr_strz(cls))) || _tr_dict_contains(self->ambiguous_classes, _tr_strz(cls))) || Sema_is_builtin_dispatch_class(self, cls))) {
#line 1108 "src/sema.tr"
        return;
    }
#line 1109 "src/sema.tr"
    if (((((_tr_str_eqv((cls), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((cls), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((cls), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((cls), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((cls), (_tr_str_lit_len("Set", 3LL))))) {
#line 1109 "src/sema.tr"
        return;
    }
#line 1110 "src/sema.tr"
    __auto_type _t1098 = (*obj);
    if (_t1098.tag == Expr_EIdent) {
        __auto_type _ru_n = _t1098.data.EIdent.name;
#line 1112 "src/sema.tr"
        if ((_tr_dict_contains(self->classes, _tr_strz(_ru_n)) && ({ __auto_type _ord_t1099 = (Sema_resolve(self, _ru_n)->kind); __auto_type _wr = ((_ord_t1099.tag != SymbolKind_make_SVariable().tag)); _wr; }))) {
#line 1112 "src/sema.tr"
            return;
        }
    } else if (1) {
        __auto_type _ = _t1098;
        /* pass */
    }
#line 1114 "src/sema.tr"
    ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls)));
#line 1115 "src/sema.tr"
    long long named = 0LL;
#line 1116 "src/sema.tr"
    bool ok = false;
#line 1117 "src/sema.tr"
    long long want = 0LL;
#line 1118 "src/sema.tr"
    FunctionDef* one = FunctionDef_init(_tr_str_lit_len("", 0LL));
#line 1119 "src/sema.tr"
    long long i = 0LL;
#line 1120 "src/sema.tr"
    while ((i < cd->methods->len)) {
#line 1121 "src/sema.tr"
        FunctionDef* m = ((FunctionDef*)List_ptr_get(cd->methods, i));
#line 1122 "src/sema.tr"
        if (_tr_str_eqv((m->name), (method))) {
#line 1123 "src/sema.tr"
            long long cnt = 0LL;
#line 1124 "src/sema.tr"
            long long pj = 0LL;
#line 1125 "src/sema.tr"
            while ((pj < m->params->len)) {
#line 1126 "src/sema.tr"
                if ((!_tr_str_eqv((((Param*)List_ptr_get(m->params, pj))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 1126 "src/sema.tr"
                    cnt = (cnt + 1LL);
                }
#line 1127 "src/sema.tr"
                if (((Param*)List_ptr_get(m->params, pj))->is_variadic) {
#line 1127 "src/sema.tr"
                    _tr_obj_release(one, _trdrop_FunctionDef);
                    return;
                }
#line 1128 "src/sema.tr"
                pj = (pj + 1LL);
            }
#line 1129 "src/sema.tr"
            if ((named == 0LL)) {
#line 1129 "src/sema.tr"
                want = cnt;
            }
#line 1130 "src/sema.tr"
            named = (named + 1LL);
#line 1131 "src/sema.tr"
            if ((cnt == hl->len)) {
#line 1131 "src/sema.tr"
                ok = true;
            }
#line 1132 "src/sema.tr"
            FunctionDef* _cltmp_t1100 = _tr_obj_retain(m);
            _tr_obj_release(one, _trdrop_FunctionDef);
            one = _cltmp_t1100;
        }
#line 1133 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1134 "src/sema.tr"
    if ((named == 0LL)) {
#line 1134 "src/sema.tr"
        _tr_obj_release(one, _trdrop_FunctionDef);
        return;
    }
#line 1135 "src/sema.tr"
    if ((!ok)) {
#line 1136 "src/sema.tr"
        TrStr pl = _tr_str_lit_len("s", 1LL);
#line 1137 "src/sema.tr"
        if ((want == 1LL)) {
#line 1137 "src/sema.tr"
            TrStr _strtmp_t1101 = _tr_str_lit_len("", 0LL);
            _tr_str_release(pl);
            pl = _strtmp_t1101;
        }
#line 1138 "src/sema.tr"
        TrStr vb = _tr_str_lit_len("were", 4LL);
#line 1139 "src/sema.tr"
        if ((hl->len == 1LL)) {
#line 1139 "src/sema.tr"
            TrStr _strtmp_t1102 = _tr_str_lit_len("was", 3LL);
            _tr_str_release(vb);
            vb = _strtmp_t1102;
        }
#line 1140 "src/sema.tr"
        ({ __auto_type _ord_t1103 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (cls))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1104 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1103); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1105 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1104); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (see its definition).", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1105); _tr_str_release(_at_t1105); });
    } else if ((((named == 1LL) && (cd->generics->len == 0LL)) && (one->generics->len == 0LL))) {
#line 1142 "src/sema.tr"
        long long sk = 0LL;
#line 1143 "src/sema.tr"
        if (((one->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(one->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 1143 "src/sema.tr"
            sk = 1LL;
        }
#line 1144 "src/sema.tr"
        long long ti = 0LL;
#line 1145 "src/sema.tr"
        while (((ti < hl->len) && ((sk + ti) < one->params->len))) {
#line 1146 "src/sema.tr"
            if ((((unsigned long long)(((Param*)List_ptr_get(one->params, (sk + ti)))->ty)) != ((unsigned long long)(0LL)))) {
#line 1147 "src/sema.tr"
                ({ __auto_type _ord_t1106 = ((*((Param*)List_ptr_get(one->params, (sk + ti)))->ty)); __auto_type _ord_t1107 = (((HirExpr*)List_ptr_get(hl, ti))); TrStr _ord_t1108 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((ti + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cls)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1106, _ord_t1107, _ord_t1108); _tr_str_release(_ord_t1108); });
            }
#line 1148 "src/sema.tr"
            ti = (ti + 1LL);
        }
    }
    _tr_obj_release(one, _trdrop_FunctionDef);
}

__attribute__((hot)) bool Sema_known_number_method(Sema* self, TrStr m) {
#line 1152 "src/sema.tr"
    if ((((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("acos", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("as_str", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("asin", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("atan", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("atan2", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("ceil", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("checked_add", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("checked_mul", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("checked_sub", 11LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clamp", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("cos", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("exp", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("fabs", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("floor", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("gcd", 3LL))))) {
#line 1152 "src/sema.tr"
        return true;
    }
#line 1153 "src/sema.tr"
    if ((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("is_inf", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("is_nan", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("lcm", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("log", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("log10", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("log2", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pow", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("round", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sign", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sin", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sqrt", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("tan", 3LL))))) {
#line 1153 "src/sema.tr"
        return true;
    }
#line 1154 "src/sema.tr"
    if (((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("to_HEX", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("to_bin", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_binary", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_f64", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_float", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_hex", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_hex_upper", 12LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_i64", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_int", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_oct", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_octal", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_str", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("to_string", 9LL))))) {
#line 1154 "src/sema.tr"
        return true;
    }
#line 1155 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema_check_collection_arg_types(Sema* self, AstType* ty, TrStr method, List_ptr* hl) {
#line 1160 "src/sema.tr"
    if (((_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))) && _tr_str_eqv((method), (_tr_str_lit_len("offset", 6LL)))) && (hl->len == 1LL))) {
#line 1161 "src/sema.tr"
        TrStr pf = Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)));
#line 1162 "src/sema.tr"
        if (((_tr_str_eqv((pf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((pf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((pf), (_tr_str_lit_len("Dict", 4LL))))) {
#line 1163 "src/sema.tr"
            ({ TrStr _at_t1109 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, pf)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: Pointer.offset() needs an int offset but is given ", 71LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass an integer element count.", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1109); _tr_str_release(_at_t1109); });
        }
#line 1164 "src/sema.tr"
        _tr_str_release(pf);
        return;
    }
#line 1165 "src/sema.tr"
    if ((((_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Set", 3LL)))) && (ty->args->len > 0LL))) {
#line 1166 "src/sema.tr"
        TrStr ef = Sema_decl_family(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
#line 1167 "src/sema.tr"
        long long vi = (0LL - 1LL);
#line 1168 "src/sema.tr"
        if (((((_tr_str_eqv((method), (_tr_str_lit_len("append", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("push", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))))) {
#line 1168 "src/sema.tr"
            vi = 0LL;
        }
#line 1169 "src/sema.tr"
        if (((_tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) && (hl->len == 2LL))) {
#line 1170 "src/sema.tr"
            vi = 1LL;
#line 1171 "src/sema.tr"
            TrStr xf = Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)));
#line 1172 "src/sema.tr"
            if (((_tr_str_eqv((xf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((xf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((xf), (_tr_str_lit_len("Dict", 4LL))))) {
#line 1173 "src/sema.tr"
                ({ __auto_type _ord_t1110 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the index of ", 34LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() must be an int but is ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1111 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1110); TrStr _tr_cr = (Sema_family_word(self, xf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass an integer position.", 38LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1111); _tr_str_release(_at_t1111); });
            }
        }
#line 1174 "src/sema.tr"
        if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("Set", 3LL))) && _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL))))) {
#line 1174 "src/sema.tr"
            vi = 0LL;
        }
#line 1175 "src/sema.tr"
        if ((((vi >= 0LL) && (vi < hl->len)) && ({ TrStr _at_t1112 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, vi)))); __auto_type _wr = (Sema_family_mismatch(self, ef, _at_t1112)); _tr_str_release(_at_t1112); _wr; }))) {
#line 1176 "src/sema.tr"
            ({ __auto_type _ord_t1113 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: ", 21LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1115 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1113); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("].", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() is given ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1114 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, vi)))); __auto_type _ord_t1116 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1115); TrStr _tr_cr = (Sema_family_word(self, _at_t1114)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: pass a '", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1117 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1116); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (convert it first if needed).", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1117); _tr_str_release(_at_t1114); _tr_str_release(_at_t1117); });
        }
    } else if (((_tr_str_eqv((ty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Map", 3LL)))) && (ty->args->len > 1LL))) {
#line 1178 "src/sema.tr"
        TrStr kf = Sema_decl_family(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
#line 1179 "src/sema.tr"
        TrStr vf = Sema_decl_family(self, (*((AstType**)List_ptr_get(ty->args, 1LL))));
#line 1180 "src/sema.tr"
        if ((((((((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) && (hl->len >= 1LL))) {
#line 1181 "src/sema.tr"
            if (({ TrStr _at_t1118 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)))); __auto_type _wr = (Sema_family_mismatch(self, kf, _at_t1118)); _tr_str_release(_at_t1118); _wr; })) {
#line 1182 "src/sema.tr"
                ({ __auto_type _ord_t1119 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the key of ", 32LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1120 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1119); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1122 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1120); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 1LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("].", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() is ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1121 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 0LL)))); __auto_type _ord_t1123 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1122); TrStr _tr_cr = (Sema_family_word(self, _at_t1121)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: use a '", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1124 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1123); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' key.", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1124); _tr_str_release(_at_t1121); _tr_str_release(_at_t1124); });
            }
        }
#line 1183 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL)))) && (hl->len == 2LL))) {
#line 1184 "src/sema.tr"
            if (({ TrStr _at_t1125 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 1LL)))); __auto_type _wr = (Sema_family_mismatch(self, vf, _at_t1125)); _tr_str_release(_at_t1125); _wr; })) {
#line 1185 "src/sema.tr"
                ({ __auto_type _ord_t1126 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the value of ", 34LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1127 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1126); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1129 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1127); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 1LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("].", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() is ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1128 = (Sema_firm_family(self, ((HirExpr*)List_ptr_get(hl, 1LL)))); __auto_type _ord_t1130 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1129); TrStr _tr_cr = (Sema_family_word(self, _at_t1128)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: use a '", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1131 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1130); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(ty->args, 1LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' value.", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1131); _tr_str_release(_at_t1128); _tr_str_release(_at_t1131); });
            }
        }
    }
}

__attribute__((hot)) bool Sema_class_method_exists(Sema* self, TrStr cls_name, TrStr method) {
#line 1189 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
#line 1189 "src/sema.tr"
        return false;
    }
#line 1190 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
#line 1191 "src/sema.tr"
    long long mi = 0LL;
#line 1192 "src/sema.tr"
    while ((mi < cls->methods->len)) {
#line 1193 "src/sema.tr"
        if (_tr_str_eqv((((FunctionDef*)List_ptr_get(cls->methods, mi))->name), (method))) {
#line 1193 "src/sema.tr"
            return true;
        }
#line 1194 "src/sema.tr"
        mi = (mi + 1LL);
    }
#line 1195 "src/sema.tr"
    long long bi = 0LL;
#line 1196 "src/sema.tr"
    while ((bi < cls->base_classes->len)) {
#line 1197 "src/sema.tr"
        if (({ TrStr _at_t1132 = (List_TrStr_get(cls->base_classes, bi)); __auto_type _wr = (Sema_class_method_exists(self, _at_t1132, method)); _tr_str_release(_at_t1132); _wr; })) {
#line 1197 "src/sema.tr"
            return true;
        }
#line 1198 "src/sema.tr"
        bi = (bi + 1LL);
    }
#line 1199 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_universal_method(Sema* self, TrStr method) {
#line 1204 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("free", 4LL))))) {
#line 1204 "src/sema.tr"
        return true;
    }
#line 1205 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("as_str", 6LL))))) {
#line 1205 "src/sema.tr"
        return true;
    }
#line 1206 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__len__", 7LL))))) {
#line 1206 "src/sema.tr"
        return true;
    }
#line 1207 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL))))) {
#line 1207 "src/sema.tr"
        return true;
    }
#line 1208 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("__getitem__", 11LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__setitem__", 11LL))))) {
#line 1208 "src/sema.tr"
        return true;
    }
#line 1209 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("__eq__", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__ne__", 6LL))))) {
#line 1209 "src/sema.tr"
        return true;
    }
#line 1210 "src/sema.tr"
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("__lt__", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__gt__", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__le__", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__ge__", 6LL))))) {
#line 1210 "src/sema.tr"
        return true;
    }
#line 1211 "src/sema.tr"
    if (((((_tr_str_eqv((method), (_tr_str_lit_len("__add__", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__sub__", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__mul__", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__div__", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__mod__", 7LL))))) {
#line 1211 "src/sema.tr"
        return true;
    }
#line 1212 "src/sema.tr"
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("__hash__", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__iter__", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__next__", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__contains__", 12LL))))) {
#line 1212 "src/sema.tr"
        return true;
    }
#line 1213 "src/sema.tr"
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("__str__", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__repr__", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__enter__", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("__exit__", 8LL))))) {
#line 1213 "src/sema.tr"
        return true;
    }
#line 1214 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_expr_is_borrow(Sema* self, HirExpr* e) {
#line 1220 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1220 "src/sema.tr"
        return false;
    }
#line 1221 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 1221 "src/sema.tr"
        return false;
    }
#line 1222 "src/sema.tr"
    if (hir_expr_type(e)->is_borrow) {
#line 1222 "src/sema.tr"
        return true;
    }
#line 1223 "src/sema.tr"
    __auto_type _t1133 = (*e);
    if (_t1133.tag == HirExpr_EIdent) {
        __auto_type nm = _t1133.data.EIdent.name;
#line 1225 "src/sema.tr"
        Symbol* sym = Sema_resolve(self, nm);
#line 1226 "src/sema.tr"
        if (((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL))))) {
#line 1227 "src/sema.tr"
            if ((*sym->ty)->is_borrow) {
#line 1227 "src/sema.tr"
                return true;
            }
        }
    } else if (1) {
        __auto_type _ = _t1133;
        /* pass */
    }
#line 1229 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__expr_is_shared(Sema* self, HirExpr* e) {
#line 1237 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 1237 "src/sema.tr"
        return false;
    }
#line 1238 "src/sema.tr"
    __auto_type _t1134 = (*e);
    if (_t1134.tag == HirExpr_EIdent) {
        __auto_type nm = _t1134.data.EIdent.name;
#line 1240 "src/sema.tr"
        Symbol* sym = Sema_resolve(self, nm);
#line 1241 "src/sema.tr"
        bool _trret189 = ((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && sym->is_shared);
        return _trret189;
    } else if (_t1134.tag == HirExpr_EMethodCall) {
        __auto_type _tr_v_recv = _t1134.data.EMethodCall.obj;
__auto_type meth = _t1134.data.EMethodCall.method;
#line 1245 "src/sema.tr"
        if (_tr_str_eqv((meth), (_tr_str_lit_len("clone", 5LL)))) {
#line 1245 "src/sema.tr"
            bool _trret190 = Sema__expr_is_shared(self, _tr_v_recv);
            return _trret190;
        }
    } else if (1) {
        __auto_type _ = _t1134;
        /* pass */
    }
#line 1247 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema_check_spawn_sendable(Sema* self, HirExpr* e) {
#line 1250 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 1250 "src/sema.tr"
        return;
    }
#line 1251 "src/sema.tr"
    __auto_type _t1135 = (*e);
    if (_t1135.tag == HirExpr_ECall) {
        __auto_type cs_args = _t1135.data.ECall.args;
#line 1253 "src/sema.tr"
        long long csi = 0LL;
#line 1254 "src/sema.tr"
        while ((csi < cs_args->len)) {
#line 1259 "src/sema.tr"
            if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(cs_args, csi)))) {
#line 1260 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot cross a thread boundary: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 252LL));
            }
#line 1261 "src/sema.tr"
            AstType* arg_ty = hir_expr_type(((HirExpr*)List_ptr_get(cs_args, csi)));
#line 1262 "src/sema.tr"
            if (((_tr_str_eqv((arg_ty->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((arg_ty->name), (_tr_str_lit_len("Weak", 4LL)))) && (arg_ty->args->len > 0LL))) {
#line 1263 "src/sema.tr"
                TrStr inner_nm = _tr_str_retain((*((AstType**)List_ptr_get(arg_ty->args, 0LL)))->name);
#line 1264 "src/sema.tr"
                if ((!Sema_is_sendable_type(self, inner_nm))) {
#line 1265 "src/sema.tr"
                    ({ TrStr _at_t1136 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] 'Shared[", 14LL)), (inner_nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' cannot safely cross thread boundaries because '", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (inner_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable.\n      FIX: Add 'implements Sendable' to '", 60LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (inner_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' and ensure all mutable fields use Atomic[T] or Mutex[T].", 58LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1136); _tr_str_release(_at_t1136); });
                }
            } else if ((((self->strict_mode && Sema__is_rc_class(self, arg_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(cs_args, csi))))) && (!Sema__is_unsafe_sendable(self, arg_ty->name)))) {
#line 1267 "src/sema.tr"
                ({ TrStr _at_t1137 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (arg_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot cross a thread boundary (this is exactly why Rust's 'Rc' is '!Send').\n      FIX: share it as 'Shared[", 216LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc'), or 'Mutex[", 51LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' for exclusive access.", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1137); _tr_str_release(_at_t1137); });
            } else if ((!Sema_is_sendable_type(self, arg_ty->name))) {
#line 1269 "src/sema.tr"
                ({ TrStr _at_t1138 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (arg_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex[", 87LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] for exclusive access, or Atomic[T] for counters/flags.\n      Or add 'implements Sendable' to '", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1138); _tr_str_release(_at_t1138); });
            }
#line 1270 "src/sema.tr"
            Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(cs_args, csi)));
#line 1271 "src/sema.tr"
            csi = (csi + 1LL);
        }
    } else if (_t1135.tag == HirExpr_EMethodCall) {
        __auto_type cs_margs = _t1135.data.EMethodCall.args;
#line 1273 "src/sema.tr"
        long long csmi = 0LL;
#line 1274 "src/sema.tr"
        while ((csmi < cs_margs->len)) {
#line 1275 "src/sema.tr"
            if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(cs_margs, csmi)))) {
#line 1276 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot cross a thread boundary: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 252LL));
            }
#line 1277 "src/sema.tr"
            AstType* arg_ty2 = hir_expr_type(((HirExpr*)List_ptr_get(cs_margs, csmi)));
#line 1278 "src/sema.tr"
            if (((_tr_str_eqv((arg_ty2->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((arg_ty2->name), (_tr_str_lit_len("Weak", 4LL)))) && (arg_ty2->args->len > 0LL))) {
#line 1279 "src/sema.tr"
                TrStr inner_nm2 = _tr_str_retain((*((AstType**)List_ptr_get(arg_ty2->args, 0LL)))->name);
#line 1280 "src/sema.tr"
                if ((!Sema_is_sendable_type(self, inner_nm2))) {
#line 1281 "src/sema.tr"
                    ({ TrStr _at_t1139 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] 'Shared[", 14LL)), (inner_nm2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' cannot safely cross thread boundaries because '", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (inner_nm2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable.\n      FIX: Add 'implements Sendable' to '", 60LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (inner_nm2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' and protect mutable fields with Atomic[T] or Mutex[T].", 56LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1139); _tr_str_release(_at_t1139); });
                }
            } else if ((((self->strict_mode && Sema__is_rc_class(self, arg_ty2->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(cs_margs, csmi))))) && (!Sema__is_unsafe_sendable(self, arg_ty2->name)))) {
#line 1283 "src/sema.tr"
                ({ TrStr _at_t1140 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (arg_ty2->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot cross a thread boundary.\n      FIX: share it as 'Shared[", 171LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty2->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount) or 'Mutex[", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty2->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' for exclusive access.", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1140); _tr_str_release(_at_t1140); });
            } else if ((!Sema_is_sendable_type(self, arg_ty2->name))) {
#line 1285 "src/sema.tr"
                ({ TrStr _at_t1141 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (arg_ty2->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex[", 87LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arg_ty2->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("].", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1141); _tr_str_release(_at_t1141); });
            }
#line 1286 "src/sema.tr"
            Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(cs_margs, csmi)));
#line 1287 "src/sema.tr"
            csmi = (csmi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1135;
        /* pass */
    }
}

__attribute__((hot)) void Sema_check_class_sendable_fields(Sema* self, ClassDef* c) {
#line 1296 "src/sema.tr"
    bool has_unsafe = false;
#line 1297 "src/sema.tr"
    long long ui = 0LL;
#line 1298 "src/sema.tr"
    while ((ui < c->iface_names->len)) {
#line 1299 "src/sema.tr"
        if (({ TrStr _cmpa_t1142 = (List_TrStr_get(c->iface_names, ui)); TrStr _cmpb_t1143 = (_tr_str_lit_len("UnsafeSendable", 14LL)); _Bool _cmpr_t1144 = _tr_str_eqv(_cmpa_t1142, _cmpb_t1143); _tr_str_release(_cmpa_t1142); _cmpr_t1144; })) {
#line 1299 "src/sema.tr"
            has_unsafe = true;
        }
#line 1300 "src/sema.tr"
        ui = (ui + 1LL);
    }
#line 1305 "src/sema.tr"
    if (has_unsafe) {
#line 1305 "src/sema.tr"
        return;
    }
#line 1306 "src/sema.tr"
    long long cfi = 0LL;
#line 1307 "src/sema.tr"
    while ((cfi < c->fields->len)) {
#line 1308 "src/sema.tr"
        FieldDef* fd = ((FieldDef*)List_ptr_get(c->fields, cfi));
#line 1309 "src/sema.tr"
        if ((((unsigned long long)(fd->ty)) != ((unsigned long long)(0LL)))) {
#line 1310 "src/sema.tr"
            AstType* fty = (*fd->ty);
#line 1311 "src/sema.tr"
            if (_tr_str_eqv((fty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 1314 "src/sema.tr"
                if ((!has_unsafe)) {
#line 1315 "src/sema.tr"
                    ({ TrStr _at_t1145 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-2] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares 'implements Sendable' but holds a raw 'Pointer' field '", 66LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fd->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which the compiler cannot prove thread-safe.\n      FIX: If '", 63LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' wraps an internally-synchronized handle, assert it explicitly: 'implements Sendable, UnsafeSendable'.\n      Otherwise wrap the data in Mutex[T]/Atomic[T], or drop 'implements Sendable'.", 187LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1145); _tr_str_release(_at_t1145); });
                }
            } else if ((!Sema_is_sendable_ty(self, fty))) {
#line 1320 "src/sema.tr"
                TrStr _t2_inner = _tr_str_retain(fty->name);
#line 1321 "src/sema.tr"
                if (((((_tr_str_eqv((fty->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("Channel", 7LL)))) && (fty->args->len > 0LL))) {
#line 1322 "src/sema.tr"
                    TrStr _strtmp_t1147 = ({ __auto_type _ord_t1146 = (_tr_strx_concatv((fty->name), (_tr_str_lit_len("[", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1146); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*((AstType**)List_ptr_get(fty->args, 0LL)))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
                    _tr_str_release(_t2_inner);
                    _t2_inner = _strtmp_t1147;
                }
#line 1323 "src/sema.tr"
                ({ TrStr _at_t1148 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-2] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares 'implements Sendable' but field '", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fd->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_t2_inner)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable (non-thread-safe data is reachable across threads).\n      FIX: make the inner type Sendable, wrap it in Mutex[T]/RwLock[T] for guarded access or Atomic[T] for numerics, or drop 'implements Sendable'.", 217LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1148); _tr_str_release(_at_t1148); });
                _tr_str_release(_t2_inner);
            } else if (((_tr_str_eqv((fty->name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((fty->name), (_tr_str_lit_len("bool", 4LL))))) {
#line 1325 "src/sema.tr"
                ({ TrStr _at_t1149 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-3] Sendable class '", 22LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has primitive field '", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fd->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' that may cause data races if mutated from multiple threads.\n      FIX: Use 'Atomic[", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' for safe concurrent mutation, or ensure this field is written only before the object is shared across threads.", 113LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->warnings, _at_t1149); _tr_str_release(_at_t1149); });
            }
        }
#line 1326 "src/sema.tr"
        cfi = (cfi + 1LL);
    }
}

__attribute__((hot)) HirExpr* Sema__patch_empty_dict_hint(Sema* self, HirExpr* hv, AstType* hint_ty) {
#line 1339 "src/sema.tr"
    if ((((unsigned long long)(hv)) == ((unsigned long long)(0LL)))) {
#line 1339 "src/sema.tr"
        return hv;
    }
#line 1340 "src/sema.tr"
    if (((!_tr_str_eqv((hint_ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (!_tr_str_eqv((hint_ty->name), (_tr_str_lit_len("Map", 3LL)))))) {
#line 1340 "src/sema.tr"
        return hv;
    }
#line 1341 "src/sema.tr"
    if ((hint_ty->args->len < 2LL)) {
#line 1341 "src/sema.tr"
        return hv;
    }
#line 1342 "src/sema.tr"
    __auto_type _t1150 = (*hv);
    if (_t1150.tag == HirExpr_EDict) {
        __auto_type hk = _t1150.data.EDict.keys;
__auto_type hvv = _t1150.data.EDict.vals;
#line 1344 "src/sema.tr"
        if ((hk->len == 0LL)) {
#line 1344 "src/sema.tr"
            return box_hirexpr(HirExpr_ctor_EDict(hk, hvv, hint_ty));
        }
#line 1345 "src/sema.tr"
        return hv;
    } else if (1) {
        __auto_type _ = _t1150;
        return hv;
    }
}

__attribute__((hot)) void Sema_mark_moved(Sema* self, TrStr name) {
#line 1351 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1352 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1353 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1354 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1355 "src/sema.tr"
            if ((!sym->is_moved)) {
#line 1356 "src/sema.tr"
                sym->is_moved = true;
#line 1357 "src/sema.tr"
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1358 "src/sema.tr"
                if (self->capturing_moves) {
#line 1359 "src/sema.tr"
                    List_TrStr_append(self->branch_moved_buf, name);
                }
            }
#line 1360 "src/sema.tr"
            return;
        }
#line 1361 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1362 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1363 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1364 "src/sema.tr"
        if ((!sym->is_moved)) {
#line 1365 "src/sema.tr"
            sym->is_moved = true;
#line 1366 "src/sema.tr"
            _tr_dict_set(self->globals, _tr_strz(name), sym);
#line 1367 "src/sema.tr"
            if (self->capturing_moves) {
#line 1368 "src/sema.tr"
                List_TrStr_append(self->branch_moved_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_mark_freed(Sema* self, TrStr name) {
#line 1371 "src/sema.tr"
    long long mf_i = (self->scopes->len - 1LL);
#line 1372 "src/sema.tr"
    while ((mf_i >= 0LL)) {
#line 1373 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name))) {
#line 1374 "src/sema.tr"
            Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name)));
#line 1375 "src/sema.tr"
            mf_sym->is_freed = true;
#line 1376 "src/sema.tr"
            mf_sym->is_moved = true;
#line 1377 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name), mf_sym);
#line 1378 "src/sema.tr"
            if (self->capturing_moves) {
#line 1379 "src/sema.tr"
                List_TrStr_append(self->branch_moved_buf, name);
            }
#line 1380 "src/sema.tr"
            return;
        }
#line 1381 "src/sema.tr"
        mf_i = (mf_i - 1LL);
    }
#line 1382 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1383 "src/sema.tr"
        Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1384 "src/sema.tr"
        mf_sym->is_freed = true;
#line 1385 "src/sema.tr"
        mf_sym->is_moved = true;
#line 1386 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), mf_sym);
#line 1387 "src/sema.tr"
        if (self->capturing_moves) {
#line 1388 "src/sema.tr"
            List_TrStr_append(self->branch_moved_buf, name);
        }
    }
}

__attribute__((hot)) void Sema_check_not_moved(Sema* self, TrStr name, TrStr ty_name) {
#line 1391 "src/sema.tr"
    if (Sema_is_primitive_name(self, ty_name)) {
#line 1391 "src/sema.tr"
        return;
    }
#line 1392 "src/sema.tr"
    Symbol* sym = Sema_resolve(self, name);
#line 1393 "src/sema.tr"
    if (((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && sym->is_moved)) {
#line 1394 "src/sema.tr"
        ({ TrStr _at_t1151 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving.", 123LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1151); _tr_str_release(_at_t1151); });
    }
}

__attribute__((hot)) void Sema_mark_borrow(Sema* self, TrStr name) {
#line 1397 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1398 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1399 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1400 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1401 "src/sema.tr"
            sym->active_borrows = (sym->active_borrows + 1LL);
#line 1402 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1403 "src/sema.tr"
            return;
        }
#line 1404 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1405 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1406 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1407 "src/sema.tr"
        sym->active_borrows = (sym->active_borrows + 1LL);
#line 1408 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_unmark_borrow(Sema* self, TrStr name) {
#line 1411 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1412 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1413 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1414 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1415 "src/sema.tr"
            if ((sym->active_borrows > 0LL)) {
#line 1416 "src/sema.tr"
                sym->active_borrows = (sym->active_borrows - 1LL);
            }
#line 1417 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1418 "src/sema.tr"
            return;
        }
#line 1419 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1420 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1421 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1422 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 1423 "src/sema.tr"
            sym->active_borrows = (sym->active_borrows - 1LL);
        }
#line 1424 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_check_no_active_borrows(Sema* self, TrStr name, TrStr ty_name) {
#line 1427 "src/sema.tr"
    if (Sema_is_primitive_name(self, ty_name)) {
#line 1427 "src/sema.tr"
        return;
    }
#line 1428 "src/sema.tr"
    Symbol* sym = Sema_resolve(self, name);
#line 1429 "src/sema.tr"
    if (((!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) && (sym->active_borrows > 0LL))) {
#line 1430 "src/sema.tr"
        ({ TrStr _at_t1152 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-2] Cannot move '", 19LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while it is borrowed.\n      FIX: The borrow must end before '", 63LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' can be moved.", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1152); _tr_str_release(_at_t1152); });
    }
}

__attribute__((hot)) void Sema_mark_init(Sema* self, TrStr name) {
#line 1434 "src/sema.tr"
    long long mi_i = (self->scopes->len - 1LL);
#line 1435 "src/sema.tr"
    while ((mi_i >= 0LL)) {
#line 1436 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
#line 1437 "src/sema.tr"
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
#line 1438 "src/sema.tr"
            if ((!mi_sym->is_init)) {
#line 1440 "src/sema.tr"
                mi_sym->is_init = true;
#line 1441 "src/sema.tr"
                mi_sym->is_maybe_init = false;
#line 1442 "src/sema.tr"
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
#line 1443 "src/sema.tr"
                if (self->capturing_inits) {
#line 1444 "src/sema.tr"
                    List_TrStr_append(self->branch_init_buf, name);
                }
            }
#line 1445 "src/sema.tr"
            return;
        }
#line 1446 "src/sema.tr"
        mi_i = (mi_i - 1LL);
    }
#line 1447 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1448 "src/sema.tr"
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1449 "src/sema.tr"
        if ((!mi_sym->is_init)) {
#line 1450 "src/sema.tr"
            mi_sym->is_init = true;
#line 1451 "src/sema.tr"
            mi_sym->is_maybe_init = false;
#line 1452 "src/sema.tr"
            _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
#line 1453 "src/sema.tr"
            if (self->capturing_inits) {
#line 1454 "src/sema.tr"
                List_TrStr_append(self->branch_init_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_clear_container_borrow(Sema* self, TrStr var_name) {
#line 1458 "src/sema.tr"
    if (_tr_dict_contains(self->container_borrows, _tr_strz(var_name))) {
#line 1459 "src/sema.tr"
        _tr_dict_remove(self->container_borrows, _tr_strz(var_name));
    }
}

__attribute__((hot)) void Sema_unmark_moved(Sema* self, TrStr name) {
#line 1463 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1464 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1465 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1466 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1467 "src/sema.tr"
            sym->is_moved = false;
#line 1473 "src/sema.tr"
            sym->is_freed = false;
#line 1474 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1475 "src/sema.tr"
            return;
        }
#line 1476 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1477 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1478 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1479 "src/sema.tr"
        sym->is_moved = false;
#line 1480 "src/sema.tr"
        sym->is_freed = false;
#line 1481 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_moved(Sema* self, TrStr name) {
#line 1485 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 1486 "src/sema.tr"
    while ((i >= 0LL)) {
#line 1487 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 1488 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 1489 "src/sema.tr"
            sym->is_maybe_moved = true;
#line 1490 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 1491 "src/sema.tr"
            return;
        }
#line 1492 "src/sema.tr"
        i = (i - 1LL);
    }
#line 1493 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1494 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1495 "src/sema.tr"
        sym->is_maybe_moved = true;
#line 1496 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_revive_moved(Sema* self, TrStr name) {
#line 1504 "src/sema.tr"
    bool rv_found = false;
#line 1505 "src/sema.tr"
    long long rv_i = (self->scopes->len - 1LL);
#line 1506 "src/sema.tr"
    while (((rv_i >= 0LL) && (!rv_found))) {
#line 1507 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, rv_i))->variables, _tr_strz(name))) {
#line 1508 "src/sema.tr"
            Symbol* rv_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, rv_i))->variables, _tr_strz(name)));
#line 1509 "src/sema.tr"
            rv_sym->is_moved = false;
#line 1510 "src/sema.tr"
            rv_sym->is_maybe_moved = false;
#line 1511 "src/sema.tr"
            rv_sym->is_freed = false;
#line 1512 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, rv_i))->variables, _tr_strz(name), rv_sym);
#line 1513 "src/sema.tr"
            rv_found = true;
        }
#line 1514 "src/sema.tr"
        rv_i = (rv_i - 1LL);
    }
#line 1515 "src/sema.tr"
    if (((!rv_found) && _tr_dict_contains(self->globals, _tr_strz(name)))) {
#line 1516 "src/sema.tr"
        Symbol* rv_g = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1517 "src/sema.tr"
        rv_g->is_moved = false;
#line 1518 "src/sema.tr"
        rv_g->is_maybe_moved = false;
#line 1519 "src/sema.tr"
        rv_g->is_freed = false;
#line 1520 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), rv_g);
    }
#line 1523 "src/sema.tr"
    if (((rv_found && self->capturing_moves) && (((unsigned long long)(self->branch_moved_buf)) != 0LL))) {
#line 1524 "src/sema.tr"
        List_TrStr* rv_nb = (void*)List_TrStr_new();
#line 1525 "src/sema.tr"
        long long rv_k = 0LL;
#line 1526 "src/sema.tr"
        while ((rv_k < self->branch_moved_buf->len)) {
#line 1527 "src/sema.tr"
            if (({ TrStr _cmpa_t1153 = (List_TrStr_get(self->branch_moved_buf, rv_k)); TrStr _cmpb_t1154 = (name); _Bool _cmpr_t1155 = !_tr_str_eqv(_cmpa_t1153, _cmpb_t1154); _tr_str_release(_cmpa_t1153); _cmpr_t1155; })) {
#line 1527 "src/sema.tr"
                ({ TrStr _at_t1156 = (List_TrStr_get(self->branch_moved_buf, rv_k)); List_TrStr_append(rv_nb, _at_t1156); _tr_str_release(_at_t1156); });
            }
#line 1528 "src/sema.tr"
            rv_k = (rv_k + 1LL);
        }
#line 1529 "src/sema.tr"
        self->branch_moved_buf = rv_nb;
    }
}

__attribute__((hot)) void Sema_unmark_init(Sema* self, TrStr name) {
#line 1533 "src/sema.tr"
    long long mi_i = (self->scopes->len - 1LL);
#line 1534 "src/sema.tr"
    while ((mi_i >= 0LL)) {
#line 1535 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
#line 1536 "src/sema.tr"
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
#line 1537 "src/sema.tr"
            mi_sym->is_init = false;
#line 1538 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
#line 1539 "src/sema.tr"
            return;
        }
#line 1540 "src/sema.tr"
        mi_i = (mi_i - 1LL);
    }
#line 1541 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1542 "src/sema.tr"
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1543 "src/sema.tr"
        mi_sym->is_init = false;
#line 1544 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_init(Sema* self, TrStr name) {
#line 1548 "src/sema.tr"
    long long mi_i = (self->scopes->len - 1LL);
#line 1549 "src/sema.tr"
    while ((mi_i >= 0LL)) {
#line 1550 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
#line 1551 "src/sema.tr"
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
#line 1552 "src/sema.tr"
            mi_sym->is_maybe_init = true;
#line 1553 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
#line 1554 "src/sema.tr"
            return;
        }
#line 1555 "src/sema.tr"
        mi_i = (mi_i - 1LL);
    }
#line 1556 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 1557 "src/sema.tr"
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
#line 1558 "src/sema.tr"
        mi_sym->is_maybe_init = true;
#line 1559 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
    }
}

__attribute__((hot)) bool Sema_vec_str_contains(Sema* self, List_TrStr* v, TrStr s) {
#line 1563 "src/sema.tr"
    long long vi = 0LL;
#line 1564 "src/sema.tr"
    while ((vi < v->len)) {
#line 1565 "src/sema.tr"
        if (({ TrStr _cmpa_t1157 = (List_TrStr_get(v, vi)); TrStr _cmpb_t1158 = (s); _Bool _cmpr_t1159 = _tr_str_eqv(_cmpa_t1157, _cmpb_t1158); _tr_str_release(_cmpa_t1157); _cmpr_t1159; })) {
#line 1565 "src/sema.tr"
            return true;
        }
#line 1566 "src/sema.tr"
        vi = (vi + 1LL);
    }
#line 1567 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_has_copy_decorator(Sema* self, List_ptr* decs) {
#line 1573 "src/sema.tr"
    long long i = 0LL;
#line 1574 "src/sema.tr"
    while ((i < decs->len)) {
#line 1575 "src/sema.tr"
        if (_tr_str_eqv((((Decorator*)List_ptr_get(decs, i))->name), (_tr_str_lit_len("copy", 4LL)))) {
#line 1575 "src/sema.tr"
            return true;
        }
#line 1576 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1577 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_class_has_free(Sema* self, TrStr name) {
#line 1582 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(name)))) {
#line 1582 "src/sema.tr"
        return false;
    }
#line 1583 "src/sema.tr"
    ClassDef* hf_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
#line 1584 "src/sema.tr"
    long long hf_i = 0LL;
#line 1585 "src/sema.tr"
    while ((hf_i < hf_cls->methods->len)) {
#line 1586 "src/sema.tr"
        TrStr hf_n = _tr_str_retain(((FunctionDef*)List_ptr_get(hf_cls->methods, hf_i))->name);
#line 1587 "src/sema.tr"
        if ((_tr_str_eqv((hf_n), (_tr_str_lit_len("free", 4LL))) || _tr_str_eqv((hf_n), (_tr_str_lit_len("dispose", 7LL))))) {
#line 1587 "src/sema.tr"
            _tr_str_release(hf_n);
            return true;
        }
#line 1588 "src/sema.tr"
        hf_i = (hf_i + 1LL);
        _tr_str_release(hf_n);
    }
#line 1589 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_copy_class(Sema* self, TrStr name) {
#line 1592 "src/sema.tr"
    if (Sema_is_primitive_name(self, name)) {
#line 1592 "src/sema.tr"
        return true;
    }
#line 1593 "src/sema.tr"
    if (_tr_dict_contains(self->copy_classes, _tr_strz(name))) {
#line 1593 "src/sema.tr"
        return ((bool)(uintptr_t)_tr_dict_get(self->copy_classes, _tr_strz(name)));
    }
#line 1594 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(name)))) {
#line 1594 "src/sema.tr"
        return false;
    }
#line 1596 "src/sema.tr"
    _tr_dict_set(self->copy_classes, _tr_strz(name), false);
#line 1597 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
#line 1598 "src/sema.tr"
    if ((cls->fields->len == 0LL)) {
#line 1598 "src/sema.tr"
        return false;
    }
#line 1599 "src/sema.tr"
    bool all_copy = true;
#line 1600 "src/sema.tr"
    long long fi = 0LL;
#line 1601 "src/sema.tr"
    while ((fi < cls->fields->len)) {
#line 1602 "src/sema.tr"
        FieldDef* fd = ((FieldDef*)List_ptr_get(cls->fields, fi));
#line 1603 "src/sema.tr"
        if ((((unsigned long long)(fd->ty)) != ((unsigned long long)(0LL)))) {
#line 1604 "src/sema.tr"
            AstType* fty = (*fd->ty);
#line 1610 "src/sema.tr"
            if ((!fty->is_borrow)) {
#line 1611 "src/sema.tr"
                if ((!Sema_is_copy_class(self, fty->name))) {
#line 1612 "src/sema.tr"
                    all_copy = false;
                }
            }
        }
#line 1613 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 1614 "src/sema.tr"
    _tr_dict_set(self->copy_classes, _tr_strz(name), all_copy);
#line 1615 "src/sema.tr"
    bool _trret191 = all_copy;
    return _trret191;
}

__attribute__((hot)) void Sema_enter_scope(Sema* self) {
#line 1618 "src/sema.tr"
    Scope* s_obj = Scope_init();
#line 1619 "src/sema.tr"
    List_ptr_append(self->scopes, _tr_obj_retain(s_obj));
#line 1620 "src/sema.tr"
    self->current_scope_depth = (self->current_scope_depth + 1LL);
#line 1623 "src/sema.tr"
    List_i64_append(self->block_depth_stack, self->block_depth);
#line 1624 "src/sema.tr"
    self->block_depth = 0LL;
#line 1625 "src/sema.tr"
    List_i64_append(self->block_stack_base, self->block_stack->len);
    _tr_obj_release(s_obj, _trdrop_Scope);
}

__attribute__((hot)) void Sema_exit_scope(Sema* self) {
#line 1631 "src/sema.tr"
    long long es_top = (self->scopes->len - 1LL);
#line 1632 "src/sema.tr"
    if ((es_top >= 0LL)) {
#line 1633 "src/sema.tr"
        Scope* es_scope = ((Scope*)List_ptr_get(self->scopes, es_top));
#line 1634 "src/sema.tr"
        long long es_k = (self->clo_let_names->len - 1LL);
#line 1635 "src/sema.tr"
        while ((es_k >= 0LL)) {
#line 1636 "src/sema.tr"
            if ((List_i64_get(self->clo_let_scopes, es_k) == es_top)) {
#line 1637 "src/sema.tr"
                TrStr es_nm = List_TrStr_get(self->clo_let_names, es_k);
#line 1638 "src/sema.tr"
                if (_tr_dict_contains(es_scope->variables, _tr_strz(es_nm))) {
#line 1639 "src/sema.tr"
                    Symbol* es_sym = ((Symbol*)(uintptr_t)_tr_dict_get(es_scope->variables, _tr_strz(es_nm)));
#line 1640 "src/sema.tr"
                    if (es_sym->coll_escaped) {
#line 1641 "src/sema.tr"
                        HirExpr* es_p = ((HirExpr*)List_ptr_get(self->clo_let_ptrs, es_k));
#line 1642 "src/sema.tr"
                        __auto_type _t1160 = (*es_p);
                        if (_t1160.tag == HirExpr_EClosure) {
                            __auto_type c_ps = _t1160.data.EClosure.params;
__auto_type c_rt = _t1160.data.EClosure.ret_ty;
__auto_type c_bd = _t1160.data.EClosure.body;
__auto_type c_as = _t1160.data.EClosure.is_async;
__auto_type c_caps = _t1160.data.EClosure.captures;
#line 1644 "src/sema.tr"
                            HirParam* mk = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 1645 "src/sema.tr"
                            mk->name = _tr_str_lit_len("@heap", 5LL);
#line 1646 "src/sema.tr"
                            mk->ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 1647 "src/sema.tr"
                            List_ptr* ncaps = (void*)List_ptr_new();
#line 1648 "src/sema.tr"
                            long long mi = 0LL;
#line 1649 "src/sema.tr"
                            while ((mi < c_caps->len)) {
#line 1650 "src/sema.tr"
                                List_ptr_append(ncaps, _tr_obj_retain(((HirParam*)List_ptr_get(c_caps, mi))));
#line 1651 "src/sema.tr"
                                mi = (mi + 1LL);
                            }
#line 1652 "src/sema.tr"
                            List_ptr_append(ncaps, _tr_obj_retain(mk));
#line 1653 "src/sema.tr"
                            /* unsafe block */
#line 1654 "src/sema.tr"
                            (*es_p = HirExpr_ctor_EClosure(c_ps, c_rt, c_bd, c_as, ncaps));
                            _tr_obj_release(mk, _trdrop_HirParam);
                        } else if (1) {
                            __auto_type _ = _t1160;
                            /* pass */
                        }
                    }
                }
#line 1656 "src/sema.tr"
                List_TrStr_pop(self->clo_let_names);
#line 1657 "src/sema.tr"
                List_ptr_pop(self->clo_let_ptrs);
#line 1658 "src/sema.tr"
                List_i64_pop(self->clo_let_scopes);
            }
#line 1659 "src/sema.tr"
            es_k = (es_k - 1LL);
        }
    }
#line 1660 "src/sema.tr"
    ((Scope*)List_ptr_pop(self->scopes));
#line 1661 "src/sema.tr"
    self->current_scope_depth = (self->current_scope_depth - 1LL);
#line 1662 "src/sema.tr"
    self->block_depth = List_i64_get(self->block_depth_stack, (self->block_depth_stack->len - 1LL));
#line 1663 "src/sema.tr"
    List_i64_pop(self->block_depth_stack);
#line 1664 "src/sema.tr"
    long long base = List_i64_get(self->block_stack_base, (self->block_stack_base->len - 1LL));
#line 1665 "src/sema.tr"
    while ((self->block_stack->len > base)) {
#line 1666 "src/sema.tr"
        List_i64_pop(self->block_stack);
    }
#line 1667 "src/sema.tr"
    List_i64_pop(self->block_stack_base);
}

__attribute__((hot)) bool Sema__coll_elem_droppable(Sema* self, TrStr n) {
#line 1676 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL)))) {
#line 1676 "src/sema.tr"
        return true;
    }
#line 1677 "src/sema.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL))))) {
#line 1677 "src/sema.tr"
        return true;
    }
#line 1678 "src/sema.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL))))) {
#line 1678 "src/sema.tr"
        return true;
    }
#line 1679 "src/sema.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL))))) {
#line 1679 "src/sema.tr"
        return true;
    }
#line 1680 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))))) {
#line 1680 "src/sema.tr"
        return true;
    }
#line 1686 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
#line 1687 "src/sema.tr"
        if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)))->is_class)) {
#line 1687 "src/sema.tr"
            return false;
        }
#line 1691 "src/sema.tr"
        if ((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 1691 "src/sema.tr"
            return false;
        }
#line 1692 "src/sema.tr"
        return true;
    }
#line 1693 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema__scan_ptr_aliased_ty(Sema* self, AstType* t) {
#line 1700 "src/sema.tr"
    if ((_tr_str_eqv((t->name), (_tr_str_lit_len("Pointer", 7LL))) && (t->args->len > 0LL))) {
#line 1701 "src/sema.tr"
        AstType* inner = (*((AstType**)List_ptr_get(t->args, 0LL)));
#line 1702 "src/sema.tr"
        _tr_dict_set(self->ptr_aliased, _tr_strz(inner->name), true);
#line 1703 "src/sema.tr"
        Sema__scan_ptr_aliased_ty(self, inner);
#line 1704 "src/sema.tr"
        return;
    }
#line 1705 "src/sema.tr"
    long long ai = 0LL;
#line 1706 "src/sema.tr"
    while ((ai < t->args->len)) {
#line 1707 "src/sema.tr"
        Sema__scan_ptr_aliased_ty(self, (*((AstType**)List_ptr_get(t->args, ai))));
#line 1708 "src/sema.tr"
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) bool Sema__is_rc_class(Sema* self, TrStr n) {
#line 1713 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(n)))) {
#line 1713 "src/sema.tr"
        return false;
    }
#line 1714 "src/sema.tr"
    if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)))->is_class)) {
#line 1714 "src/sema.tr"
        return false;
    }
#line 1715 "src/sema.tr"
    if ((((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || (_tr_str_eqv((n), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 1715 "src/sema.tr"
        return false;
    }
#line 1716 "src/sema.tr"
    if (_tr_dict_contains(self->ptr_aliased, _tr_strz(n))) {
#line 1716 "src/sema.tr"
        return false;
    }
#line 1717 "src/sema.tr"
    return true;
}

__attribute__((hot)) bool Sema__ty_reaches_plain_rc(Sema* self, AstType* ft, long long depth) {
#line 1727 "src/sema.tr"
    if ((depth > 24LL)) {
#line 1727 "src/sema.tr"
        return false;
    }
#line 1728 "src/sema.tr"
    TrStr n = _tr_str_retain(ft->name);
#line 1729 "src/sema.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 1729 "src/sema.tr"
        _tr_str_release(n);
        return false;
    }
#line 1730 "src/sema.tr"
    if ((((((((_tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || (_tr_str_eqv((n), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL))))) {
#line 1731 "src/sema.tr"
        if ((ft->args->len > 0LL)) {
#line 1731 "src/sema.tr"
            bool _trret192 = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(ft->args, 0LL))), (depth + 1LL));
            _tr_str_release(n);
            return _trret192;
        }
#line 1732 "src/sema.tr"
        _tr_str_release(n);
        return false;
    }
#line 1733 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
#line 1734 "src/sema.tr"
        if ((ft->args->len > 1LL)) {
#line 1734 "src/sema.tr"
            bool _trret193 = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(ft->args, 1LL))), (depth + 1LL));
            _tr_str_release(n);
            return _trret193;
        }
#line 1735 "src/sema.tr"
        _tr_str_release(n);
        return false;
    }
#line 1736 "src/sema.tr"
    if (Sema__is_rc_class(self, n)) {
#line 1736 "src/sema.tr"
        _tr_str_release(n);
        return true;
    }
#line 1737 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
#line 1737 "src/sema.tr"
        bool _trret194 = Sema__sendable_reaches_plain_rc(self, n, (depth + 1LL));
        _tr_str_release(n);
        return _trret194;
    }
#line 1738 "src/sema.tr"
    _tr_str_release(n);
    return false;
}

__attribute__((hot)) bool Sema__sendable_reaches_plain_rc(Sema* self, TrStr tn, long long depth) {
#line 1741 "src/sema.tr"
    if ((depth > 24LL)) {
#line 1741 "src/sema.tr"
        return false;
    }
#line 1742 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
#line 1742 "src/sema.tr"
        return false;
    }
#line 1743 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(tn)));
#line 1744 "src/sema.tr"
    long long i = 0LL;
#line 1745 "src/sema.tr"
    while ((i < cls->fields->len)) {
#line 1746 "src/sema.tr"
        FieldDef* f = ((FieldDef*)List_ptr_get(cls->fields, i));
#line 1747 "src/sema.tr"
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
#line 1748 "src/sema.tr"
            if (Sema__ty_reaches_plain_rc(self, (*f->ty), (depth + 1LL))) {
#line 1748 "src/sema.tr"
                return true;
            }
        }
#line 1749 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1750 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__is_unsafe_sendable(Sema* self, TrStr n) {
#line 1763 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(n)))) {
#line 1763 "src/sema.tr"
        return false;
    }
#line 1764 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)));
#line 1765 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(cls->iface_names)))) {
#line 1765 "src/sema.tr"
        return false;
    }
#line 1766 "src/sema.tr"
    long long i = 0LL;
#line 1767 "src/sema.tr"
    while ((i < cls->iface_names->len)) {
#line 1768 "src/sema.tr"
        if (({ TrStr _cmpa_t1161 = (List_TrStr_get(cls->iface_names, i)); TrStr _cmpb_t1162 = (_tr_str_lit_len("UnsafeSendable", 14LL)); _Bool _cmpr_t1163 = _tr_str_eqv(_cmpa_t1161, _cmpb_t1162); _tr_str_release(_cmpa_t1161); _cmpr_t1163; })) {
#line 1768 "src/sema.tr"
            return true;
        }
#line 1769 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1770 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema__check_spawn_nested_rc(Sema* self, HirExpr* arg_expr) {
#line 1773 "src/sema.tr"
    if ((!self->strict_mode)) {
#line 1773 "src/sema.tr"
        return;
    }
#line 1774 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(arg_expr)))) {
#line 1774 "src/sema.tr"
        return;
    }
#line 1775 "src/sema.tr"
    AstType* at = hir_expr_type(arg_expr);
#line 1776 "src/sema.tr"
    TrStr n = _tr_str_retain(at->name);
#line 1778 "src/sema.tr"
    TrStr _cross_cls = _tr_str_retain(n);
#line 1779 "src/sema.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) && (at->args->len > 0LL))) {
#line 1779 "src/sema.tr"
        TrStr _strtmp_t1164 = _tr_str_retain((*((AstType**)List_ptr_get(at->args, 0LL)))->name);
        _tr_str_release(_cross_cls);
        _cross_cls = _strtmp_t1164;
    }
#line 1780 "src/sema.tr"
    if (Sema__is_unsafe_sendable(self, _cross_cls)) {
#line 1780 "src/sema.tr"
        _tr_str_release(n);
        _tr_str_release(_cross_cls);
        return;
    }
#line 1781 "src/sema.tr"
    bool reaches = false;
#line 1782 "src/sema.tr"
    if (Sema__expr_is_shared(self, arg_expr)) {
#line 1783 "src/sema.tr"
        TrStr inner = _tr_str_retain(n);
#line 1784 "src/sema.tr"
        if (((_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) && (at->args->len > 0LL))) {
#line 1784 "src/sema.tr"
            TrStr _strtmp_t1165 = _tr_str_retain((*((AstType**)List_ptr_get(at->args, 0LL)))->name);
            _tr_str_release(inner);
            inner = _strtmp_t1165;
        }
#line 1785 "src/sema.tr"
        reaches = Sema__sendable_reaches_plain_rc(self, inner, 0LL);
        _tr_str_release(inner);
    } else if (((((((((_tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || (_tr_str_eqv((n), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) && (at->args->len > 0LL))) {
#line 1787 "src/sema.tr"
        reaches = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(at->args, 0LL))), 0LL);
    } else if (((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) && (at->args->len > 1LL))) {
#line 1789 "src/sema.tr"
        reaches = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(at->args, 1LL))), 0LL);
    }
#line 1790 "src/sema.tr"
    if (reaches) {
#line 1791 "src/sema.tr"
        ({ TrStr _at_t1166 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] a value crossing the thread boundary transitively holds a plain (non-atomic) reference-counted class inside '", 115LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (e.g. via a Mutex/Vec/field). Even wrapped in Shared/Sendable, that nested class's refcount is thread-local and would race across threads.\n      FIX: make the nested state 'Shared[T]' (atomic, like Rust's 'Arc'), or restructure so only one thread ever owns it.", 262LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1166); _tr_str_release(_at_t1166); });
    }
    _tr_str_release(n);
    _tr_str_release(_cross_cls);
}

__attribute__((hot)) void Sema__collect_strong_edge(Sema* self, AstType* ft, List_TrStr* out) {
#line 1797 "src/sema.tr"
    TrStr n = _tr_str_retain(ft->name);
#line 1802 "src/sema.tr"
    if (ft->is_borrow) {
#line 1802 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1803 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL))))) {
#line 1803 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1804 "src/sema.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL))))) {
#line 1805 "src/sema.tr"
        if ((ft->args->len > 0LL)) {
#line 1805 "src/sema.tr"
            Sema__collect_strong_edge(self, (*((AstType**)List_ptr_get(ft->args, 0LL))), out);
        }
#line 1806 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1807 "src/sema.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
#line 1808 "src/sema.tr"
        if ((ft->args->len > 1LL)) {
#line 1808 "src/sema.tr"
            Sema__collect_strong_edge(self, (*((AstType**)List_ptr_get(ft->args, 1LL))), out);
        }
#line 1809 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1810 "src/sema.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) {
#line 1811 "src/sema.tr"
        if ((ft->args->len > 0LL)) {
#line 1812 "src/sema.tr"
            TrStr si = _tr_str_retain((*((AstType**)List_ptr_get(ft->args, 0LL)))->name);
#line 1813 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(si))) {
#line 1813 "src/sema.tr"
                List_TrStr_append(out, si);
            }
        }
#line 1814 "src/sema.tr"
        _tr_str_release(n);
        return;
    }
#line 1815 "src/sema.tr"
    if (Sema__is_rc_class(self, n)) {
#line 1815 "src/sema.tr"
        List_TrStr_append(out, n);
    }
    _tr_str_release(n);
}

__attribute__((hot)) List_TrStr* Sema__strong_owned(Sema* self, ClassDef* cd) {
#line 1818 "src/sema.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1819 "src/sema.tr"
    long long i = 0LL;
#line 1820 "src/sema.tr"
    while ((i < cd->fields->len)) {
#line 1821 "src/sema.tr"
        FieldDef* f = ((FieldDef*)List_ptr_get(cd->fields, i));
#line 1822 "src/sema.tr"
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
#line 1823 "src/sema.tr"
            Sema__collect_strong_edge(self, (*f->ty), out);
        }
#line 1824 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1825 "src/sema.tr"
    return out;
}

__attribute__((hot)) void Sema__cycle_dfs(Sema* self, TrStr cur, TrStr start, TrMap* on_path, TrMap* reported, long long depth) {
#line 1830 "src/sema.tr"
    if ((depth > 200LL)) {
#line 1830 "src/sema.tr"
        return;
    }
#line 1831 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cur)))) {
#line 1831 "src/sema.tr"
        return;
    }
#line 1832 "src/sema.tr"
    List_TrStr* edges = Sema__strong_owned(self, ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cur))));
#line 1833 "src/sema.tr"
    long long i = 0LL;
#line 1834 "src/sema.tr"
    while ((i < edges->len)) {
#line 1835 "src/sema.tr"
        TrStr nxt = List_TrStr_get(edges, i);
#line 1836 "src/sema.tr"
        if (_tr_str_eqv((nxt), (start))) {
#line 1838 "src/sema.tr"
            if ((!_tr_dict_contains(reported, _tr_strz(start)))) {
#line 1839 "src/sema.tr"
                _tr_dict_set(reported, _tr_strz(start), true);
#line 1840 "src/sema.tr"
                ({ TrStr _at_t1167 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[S-2] class '", 13LL)), (start))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is part of a strong reference CYCLE (via '", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' -> '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nxt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'), which reference counting cannot reclaim — it would leak.\n      FIX: make one back-edge non-owning with 'Weak[", 115LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (start)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (a weak reference) or 'Pointer[", 34LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (start)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (raw, manual).", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1167); _tr_str_release(_at_t1167); });
            }
        } else if ((!_tr_dict_contains(on_path, _tr_strz(nxt)))) {
#line 1842 "src/sema.tr"
            _tr_dict_set(on_path, _tr_strz(nxt), true);
#line 1843 "src/sema.tr"
            Sema__cycle_dfs(self, nxt, start, on_path, reported, (depth + 1LL));
#line 1844 "src/sema.tr"
            _tr_dict_set(on_path, _tr_strz(nxt), false);
        }
#line 1845 "src/sema.tr"
        i = (i + 1LL);
        _tr_str_release(nxt);
    }
}

__attribute__((hot)) void Sema_check_ownership_cycles(Sema* self, Program* prog) {
#line 1849 "src/sema.tr"
    long long di = 0LL;
#line 1850 "src/sema.tr"
    while ((di < prog->decls->len)) {
#line 1851 "src/sema.tr"
        __auto_type _t1168 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t1168.tag == Decl_DClass) {
            __auto_type c = _t1168.data.DClass.cls;
            Sema__scan_class_ptrs(self, c);
        } else if (_t1168.tag == Decl_DActor) {
            __auto_type c = _t1168.data.DActor.cls;
            Sema__scan_class_ptrs(self, c);
        } else if (_t1168.tag == Decl_DExtend) {
            __auto_type ms = _t1168.data.DExtend.methods;
#line 1855 "src/sema.tr"
            long long mi = 0LL;
#line 1856 "src/sema.tr"
            while ((mi < ms->len)) {
#line 1857 "src/sema.tr"
                Sema__scan_fn_ptrs(self, ((FunctionDef*)List_ptr_get(ms, mi)));
#line 1858 "src/sema.tr"
                mi = (mi + 1LL);
            }
        } else if (_t1168.tag == Decl_DFunction) {
            __auto_type f = _t1168.data.DFunction.func;
            Sema__scan_fn_ptrs(self, f);
        } else if (1) {
            __auto_type _ = _t1168;
            /* pass */
        }
#line 1861 "src/sema.tr"
        di = (di + 1LL);
    }
#line 1863 "src/sema.tr"
    TrMap* reported = _tr_dict_new(16LL);
#line 1864 "src/sema.tr"
    di = 0LL;
#line 1865 "src/sema.tr"
    while ((di < prog->decls->len)) {
#line 1866 "src/sema.tr"
        __auto_type _t1169 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t1169.tag == Decl_DClass) {
            __auto_type c = _t1169.data.DClass.cls;
#line 1868 "src/sema.tr"
            if (Sema__is_rc_class(self, c->name)) {
#line 1869 "src/sema.tr"
                TrMap* on_path = _tr_dict_new(16LL);
#line 1870 "src/sema.tr"
                _tr_dict_set(on_path, _tr_strz(c->name), true);
#line 1871 "src/sema.tr"
                Sema__cycle_dfs(self, c->name, c->name, on_path, reported, 0LL);
            }
        } else if (1) {
            __auto_type _ = _t1169;
            /* pass */
        }
#line 1873 "src/sema.tr"
        di = (di + 1LL);
    }
}

__attribute__((hot)) void Sema__scan_class_ptrs(Sema* self, ClassDef* c) {
#line 1876 "src/sema.tr"
    long long i = 0LL;
#line 1877 "src/sema.tr"
    while ((i < c->fields->len)) {
#line 1878 "src/sema.tr"
        FieldDef* f = ((FieldDef*)List_ptr_get(c->fields, i));
#line 1879 "src/sema.tr"
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
#line 1879 "src/sema.tr"
            Sema__scan_ptr_aliased_ty(self, (*f->ty));
        }
#line 1880 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1881 "src/sema.tr"
    i = 0LL;
#line 1882 "src/sema.tr"
    while ((i < c->methods->len)) {
#line 1883 "src/sema.tr"
        Sema__scan_fn_ptrs(self, ((FunctionDef*)List_ptr_get(c->methods, i)));
#line 1884 "src/sema.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema__scan_fn_ptrs(Sema* self, FunctionDef* f) {
#line 1887 "src/sema.tr"
    long long i = 0LL;
#line 1888 "src/sema.tr"
    while ((i < f->params->len)) {
#line 1889 "src/sema.tr"
        Param* p = ((Param*)List_ptr_get(f->params, i));
#line 1890 "src/sema.tr"
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
#line 1890 "src/sema.tr"
            Sema__scan_ptr_aliased_ty(self, (*p->ty));
        }
#line 1891 "src/sema.tr"
        i = (i + 1LL);
    }
#line 1892 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 1892 "src/sema.tr"
        Sema__scan_ptr_aliased_ty(self, (*f->ret_ty));
    }
}

__attribute__((hot)) bool Sema_is_droppable_sym(Sema* self, Symbol* sym) {
#line 1898 "src/sema.tr"
    if ((sym->kind.tag != SymbolKind_make_SVariable().tag)) {
#line 1898 "src/sema.tr"
        return false;
    }
#line 1899 "src/sema.tr"
    if (sym->is_param) {
#line 1899 "src/sema.tr"
        return false;
    }
#line 1900 "src/sema.tr"
    if ((((unsigned long long)(sym->ty)) == ((unsigned long long)(0LL)))) {
#line 1900 "src/sema.tr"
        return false;
    }
#line 1906 "src/sema.tr"
    if (sym->is_shared) {
#line 1907 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 1907 "src/sema.tr"
            return false;
        }
#line 1908 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 1908 "src/sema.tr"
            return false;
        }
#line 1909 "src/sema.tr"
        return true;
    }
#line 1910 "src/sema.tr"
    if (_tr_str_eqv(((*sym->ty)->name), (_tr_str_lit_len("Weak", 4LL)))) {
#line 1911 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 1911 "src/sema.tr"
            return false;
        }
#line 1912 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 1912 "src/sema.tr"
            return false;
        }
#line 1913 "src/sema.tr"
        return true;
    }
#line 1918 "src/sema.tr"
    if (_tr_str_eqv((sym->borrows_region), (_tr_str_lit_len("@borrowed", 9LL)))) {
#line 1918 "src/sema.tr"
        return false;
    }
#line 1919 "src/sema.tr"
    TrStr tn = _tr_str_retain((*sym->ty)->name);
#line 1926 "src/sema.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("str", 3LL)))) {
#line 1927 "src/sema.tr"
        if (sym->str_escaped) {
#line 1927 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1928 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 1928 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1929 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 1929 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1930 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 1930 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1931 "src/sema.tr"
        _tr_str_release(tn);
        return true;
    }
#line 1939 "src/sema.tr"
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 1940 "src/sema.tr"
        if (sym->coll_escaped) {
#line 1940 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1941 "src/sema.tr"
        if (_tr_dict_contains(self->alias_assigned, _tr_strz(sym->name))) {
#line 1941 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1942 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 1942 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1943 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 1943 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1944 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 1944 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1945 "src/sema.tr"
        List_ptr* targs = (*sym->ty)->args;
#line 1946 "src/sema.tr"
        if ((_tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL))))) {
#line 1947 "src/sema.tr"
            if ((targs->len < 2LL)) {
#line 1947 "src/sema.tr"
                _tr_str_release(tn);
                return false;
            }
#line 1948 "src/sema.tr"
            TrStr kt = _tr_str_retain((*((AstType**)List_ptr_get(targs, 0LL)))->name);
#line 1949 "src/sema.tr"
            TrStr vt = _tr_str_retain((*((AstType**)List_ptr_get(targs, 1LL)))->name);
#line 1950 "src/sema.tr"
            if ((!Sema__coll_elem_droppable(self, kt))) {
#line 1950 "src/sema.tr"
                _tr_str_release(tn);
                _tr_str_release(kt);
                _tr_str_release(vt);
                return false;
            }
#line 1954 "src/sema.tr"
            if ((!Sema__coll_elem_droppable(self, vt))) {
#line 1954 "src/sema.tr"
                _tr_str_release(tn);
                _tr_str_release(kt);
                _tr_str_release(vt);
                return false;
            }
#line 1955 "src/sema.tr"
            _tr_str_release(tn);
            _tr_str_release(kt);
            _tr_str_release(vt);
            return true;
        } else {
#line 1957 "src/sema.tr"
            if ((targs->len < 1LL)) {
#line 1957 "src/sema.tr"
                _tr_str_release(tn);
                return false;
            }
#line 1958 "src/sema.tr"
            TrStr et = _tr_str_retain((*((AstType**)List_ptr_get(targs, 0LL)))->name);
#line 1959 "src/sema.tr"
            if ((!Sema__coll_elem_droppable(self, et))) {
#line 1959 "src/sema.tr"
                _tr_str_release(tn);
                _tr_str_release(et);
                return false;
            }
#line 1960 "src/sema.tr"
            _tr_str_release(tn);
            _tr_str_release(et);
            return true;
        }
    }
#line 1966 "src/sema.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 1967 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 1967 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1968 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 1968 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1969 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 1969 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1970 "src/sema.tr"
        _tr_str_release(tn);
        return true;
    }
#line 1974 "src/sema.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL)))) {
#line 1975 "src/sema.tr"
        if (sym->coll_escaped) {
#line 1975 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1976 "src/sema.tr"
        if (_tr_dict_contains(self->alias_assigned, _tr_strz(sym->name))) {
#line 1976 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1977 "src/sema.tr"
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 1977 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1978 "src/sema.tr"
        if ((sym->active_borrows > 0LL)) {
#line 1978 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1979 "src/sema.tr"
        if (((!sym->is_init) || sym->is_maybe_init)) {
#line 1979 "src/sema.tr"
            _tr_str_release(tn);
            return false;
        }
#line 1980 "src/sema.tr"
        _tr_str_release(tn);
        return true;
    }
#line 1981 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
#line 1981 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 1984 "src/sema.tr"
    if (((((((((((_tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Chan", 4LL))))) {
#line 1984 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 1988 "src/sema.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("StringBuilder", 13LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 1988 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 1989 "src/sema.tr"
    if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
#line 1989 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 1990 "src/sema.tr"
    if ((sym->active_borrows > 0LL)) {
#line 1990 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 1991 "src/sema.tr"
    if (((!sym->is_init) || sym->is_maybe_init)) {
#line 1991 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 1992 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(tn)));
#line 1993 "src/sema.tr"
    long long mi = 0LL;
#line 1994 "src/sema.tr"
    bool has_dispose = false;
#line 1995 "src/sema.tr"
    while ((mi < cls->methods->len)) {
#line 1996 "src/sema.tr"
        TrStr mnm = _tr_str_retain(((FunctionDef*)List_ptr_get(cls->methods, mi))->name);
#line 1997 "src/sema.tr"
        if (_tr_str_eqv((mnm), (_tr_str_lit_len("free", 4LL)))) {
#line 2006 "src/sema.tr"
            if (sym->coll_escaped) {
#line 2006 "src/sema.tr"
                _tr_str_release(tn);
                _tr_str_release(mnm);
                return false;
            }
#line 2007 "src/sema.tr"
            _tr_str_release(tn);
            _tr_str_release(mnm);
            return true;
        }
#line 2008 "src/sema.tr"
        if (_tr_str_eqv((mnm), (_tr_str_lit_len("dispose", 7LL)))) {
#line 2008 "src/sema.tr"
            has_dispose = true;
        }
#line 2009 "src/sema.tr"
        mi = (mi + 1LL);
        _tr_str_release(mnm);
    }
#line 2011 "src/sema.tr"
    if (has_dispose) {
#line 2011 "src/sema.tr"
        _tr_str_release(tn);
        return false;
    }
#line 2017 "src/sema.tr"
    _tr_str_release(tn);
    return true;
}

__attribute__((hot)) void Sema_open_block(Sema* self) {
#line 2021 "src/sema.tr"
    self->next_block_id = (self->next_block_id + 1LL);
#line 2022 "src/sema.tr"
    List_i64_append(self->block_stack, self->next_block_id);
}

__attribute__((hot)) void Sema_close_block(Sema* self) {
#line 2026 "src/sema.tr"
    long long closing = 0LL;
#line 2027 "src/sema.tr"
    if ((self->block_stack->len > 0LL)) {
#line 2027 "src/sema.tr"
        closing = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
    }
#line 2029 "src/sema.tr"
    while (((self->shadow_blocks->len > 0LL) && (List_i64_get(self->shadow_blocks, (self->shadow_blocks->len - 1LL)) == closing))) {
#line 2030 "src/sema.tr"
        long long li = (self->shadow_blocks->len - 1LL);
#line 2031 "src/sema.tr"
        long long si = List_i64_get(self->shadow_scopes, li);
#line 2032 "src/sema.tr"
        if (((si >= 0LL) && (si < self->scopes->len))) {
#line 2033 "src/sema.tr"
            Scope* sc = ((Scope*)List_ptr_get(self->scopes, si));
#line 2034 "src/sema.tr"
            ({ TrStr _ord_t1170 = (List_TrStr_get(self->shadow_names, li)); __auto_type _ord_t1171 = (((Symbol*)List_ptr_get(self->shadow_prevs, li))); _tr_dict_set(sc->variables, _tr_strz(_ord_t1170), _ord_t1171); _tr_str_release(_ord_t1170); });
        }
#line 2035 "src/sema.tr"
        List_TrStr_pop(self->shadow_names);
#line 2036 "src/sema.tr"
        ((Symbol*)List_ptr_pop(self->shadow_prevs));
#line 2037 "src/sema.tr"
        List_i64_pop(self->shadow_blocks);
#line 2038 "src/sema.tr"
        List_i64_pop(self->shadow_scopes);
    }
#line 2039 "src/sema.tr"
    List_i64_pop(self->block_stack);
}

__attribute__((hot)) bool Sema_block_stack_contains(Sema* self, long long id) {
#line 2043 "src/sema.tr"
    long long i = 0LL;
#line 2044 "src/sema.tr"
    while ((i < self->block_stack->len)) {
#line 2045 "src/sema.tr"
        if ((List_i64_get(self->block_stack, i) == id)) {
#line 2045 "src/sema.tr"
            return true;
        }
#line 2046 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2047 "src/sema.tr"
    return false;
}

__attribute__((hot)) List_ptr* Sema_compute_scope_drops(Sema* self, long long scope_idx, bool is_innermost) {
#line 2054 "src/sema.tr"
    List_ptr* out = (void*)List_ptr_new();
#line 2055 "src/sema.tr"
    if (((scope_idx < 0LL) || (scope_idx >= self->scopes->len))) {
#line 2055 "src/sema.tr"
        return out;
    }
#line 2056 "src/sema.tr"
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, scope_idx));
#line 2057 "src/sema.tr"
    long long ki = 0LL;
#line 2058 "src/sema.tr"
    while ((ki < scope->decl_order->len)) {
#line 2059 "src/sema.tr"
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
#line 2065 "src/sema.tr"
        if (_tr_dict_contains(scope->variables, _tr_strz(nm))) {
#line 2066 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
#line 2067 "src/sema.tr"
            if ((((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL)))) {
#line 2068 "src/sema.tr"
                bool visible = (sym->decl_block_id == 0LL);
#line 2069 "src/sema.tr"
                if (((!visible) && is_innermost)) {
#line 2070 "src/sema.tr"
                    visible = Sema_block_stack_contains(self, sym->decl_block_id);
                }
#line 2071 "src/sema.tr"
                if (visible) {
#line 2072 "src/sema.tr"
                    if (Sema_is_droppable_sym(self, sym)) {
#line 2073 "src/sema.tr"
                        List_ptr_append(out, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
                    }
                }
            }
        }
#line 2074 "src/sema.tr"
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
#line 2075 "src/sema.tr"
    return out;
}

__attribute__((hot)) void Sema_append_drops_from(Sema* self, HirBlock* hb, long long from_idx) {
#line 2080 "src/sema.tr"
    Sema_append_drops_from_excl(self, hb, from_idx, _tr_str_lit_len("", 0LL));
}

__attribute__((hot)) void Sema_append_drops_from_excl(Sema* self, HirBlock* hb, long long from_idx, TrStr exclude) {
#line 2086 "src/sema.tr"
    List_TrStr* excl_list = (void*)List_TrStr_new();
#line 2087 "src/sema.tr"
    if ((!_tr_str_eqv((exclude), (_tr_str_lit_len("", 0LL))))) {
#line 2087 "src/sema.tr"
        List_TrStr_append(excl_list, exclude);
    }
#line 2088 "src/sema.tr"
    Sema_append_drops_from_excl_multi(self, hb, from_idx, excl_list);
}

__attribute__((hot)) void Sema_append_drops_from_excl_multi(Sema* self, HirBlock* hb, long long from_idx, List_TrStr* excludes) {
#line 2097 "src/sema.tr"
    long long idx = (self->scopes->len - 1LL);
#line 2098 "src/sema.tr"
    while ((idx >= from_idx)) {
#line 2099 "src/sema.tr"
        bool is_innermost = (idx == (self->scopes->len - 1LL));
#line 2100 "src/sema.tr"
        List_ptr* drops = Sema_compute_scope_drops(self, idx, is_innermost);
#line 2101 "src/sema.tr"
        long long di = 0LL;
#line 2102 "src/sema.tr"
        while ((di < drops->len)) {
#line 2103 "src/sema.tr"
            HirStmt* d = ((HirStmt*)List_ptr_get(drops, di));
#line 2104 "src/sema.tr"
            bool skip = false;
#line 2105 "src/sema.tr"
            __auto_type _t1172 = (*d);
            if (_t1172.tag == HirStmt_SAutoDrop) {
                __auto_type dn = _t1172.data.SAutoDrop.name;
#line 2107 "src/sema.tr"
                long long ei = 0LL;
#line 2108 "src/sema.tr"
                while ((ei < excludes->len)) {
#line 2109 "src/sema.tr"
                    if (({ TrStr _cmpa_t1173 = (List_TrStr_get(excludes, ei)); TrStr _cmpb_t1174 = (dn); _Bool _cmpr_t1175 = _tr_str_eqv(_cmpa_t1173, _cmpb_t1174); _tr_str_release(_cmpa_t1173); _cmpr_t1175; })) {
#line 2109 "src/sema.tr"
                        skip = true;
                    }
#line 2110 "src/sema.tr"
                    ei = (ei + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1172;
                /* pass */
            }
#line 2112 "src/sema.tr"
            if ((!skip)) {
#line 2112 "src/sema.tr"
                HirBlock_push(hb, d);
            }
#line 2113 "src/sema.tr"
            di = (di + 1LL);
        }
#line 2114 "src/sema.tr"
        idx = (idx - 1LL);
    }
}

__attribute__((hot)) AstType** Sema_closure_hint_for_fn(Sema* self, TrStr fname, long long k) {
#line 2123 "src/sema.tr"
    AstType** nohint = (AstType**)(0LL);
#line 2124 "src/sema.tr"
    if ((!_tr_dict_contains(self->fn_sigs, _tr_strz(fname)))) {
#line 2124 "src/sema.tr"
        return nohint;
    }
#line 2125 "src/sema.tr"
    AstType* sig = ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(fname)));
#line 2126 "src/sema.tr"
    if ((k >= (sig->args->len - 1LL))) {
#line 2126 "src/sema.tr"
        AstType** _trret195 = nohint;
        return _trret195;
    }
#line 2127 "src/sema.tr"
    AstType** pt = ((AstType**)List_ptr_get(sig->args, k));
#line 2128 "src/sema.tr"
    if ((((unsigned long long)(pt)) == ((unsigned long long)(0LL)))) {
#line 2128 "src/sema.tr"
        AstType** _trret196 = nohint;
        return _trret196;
    }
#line 2129 "src/sema.tr"
    if (_tr_str_eqv(((*pt)->name), (_tr_str_lit_len("def", 3LL)))) {
#line 2129 "src/sema.tr"
        AstType** _trret197 = pt;
        return _trret197;
    }
#line 2130 "src/sema.tr"
    AstType** _trret198 = nohint;
    return _trret198;
}

__attribute__((hot)) AstType** Sema_closure_hint_for_method(Sema* self, TrStr method, long long k) {
#line 2136 "src/sema.tr"
    AstType** nohint = (AstType**)(0LL);
#line 2137 "src/sema.tr"
    AstType** found = (AstType**)(0LL);
#line 2138 "src/sema.tr"
    bool ambiguous = false;
#line 2139 "src/sema.tr"
    List_TrStr* keys = _tr_dict_keys(self->classes);
#line 2140 "src/sema.tr"
    long long ci = 0LL;
#line 2141 "src/sema.tr"
    while ((ci < keys->len)) {
#line 2142 "src/sema.tr"
        ClassDef* cd = ({ TrStr _ord_t1176 = (List_TrStr_get(keys, ci)); __auto_type _wr = (((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_ord_t1176)))); _tr_str_release(_ord_t1176); _wr; });
#line 2143 "src/sema.tr"
        long long mi = 0LL;
#line 2144 "src/sema.tr"
        while ((mi < cd->methods->len)) {
#line 2145 "src/sema.tr"
            FunctionDef* md = ((FunctionDef*)List_ptr_get(cd->methods, mi));
#line 2146 "src/sema.tr"
            if (_tr_str_eqv((md->name), (method))) {
#line 2147 "src/sema.tr"
                long long idx = 0LL;
#line 2148 "src/sema.tr"
                long long pi = 0LL;
#line 2149 "src/sema.tr"
                while ((pi < md->params->len)) {
#line 2150 "src/sema.tr"
                    Param* prm = ((Param*)List_ptr_get(md->params, pi));
#line 2151 "src/sema.tr"
                    if ((!_tr_str_eqv((prm->name), (_tr_str_lit_len("self", 4LL))))) {
#line 2152 "src/sema.tr"
                        if ((((idx == k) && (((unsigned long long)(prm->ty)) != ((unsigned long long)(0LL)))) && _tr_str_eqv(((*prm->ty)->name), (_tr_str_lit_len("def", 3LL))))) {
#line 2153 "src/sema.tr"
                            if ((((unsigned long long)(found)) == ((unsigned long long)(0LL)))) {
#line 2154 "src/sema.tr"
                                found = prm->ty;
                            } else if (({ __auto_type _ord_t1177 = ((*found)->args->len); __auto_type _wr = ((_ord_t1177 != (*prm->ty)->args->len)); _wr; })) {
#line 2156 "src/sema.tr"
                                ambiguous = true;
                            }
                        }
#line 2157 "src/sema.tr"
                        idx = (idx + 1LL);
                    }
#line 2158 "src/sema.tr"
                    pi = (pi + 1LL);
                }
            }
#line 2159 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 2160 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 2161 "src/sema.tr"
    if (ambiguous) {
#line 2161 "src/sema.tr"
        AstType** _trret199 = nohint;
        List_TrStr_free(keys);
        return _trret199;
    }
#line 2162 "src/sema.tr"
    AstType** _trret200 = found;
    List_TrStr_free(keys);
    return _trret200;
}

__attribute__((hot)) AstType** Sema_list_hint_for_fn(Sema* self, TrStr fname, long long k) {
#line 2167 "src/sema.tr"
    AstType** nohint = (AstType**)(0LL);
#line 2168 "src/sema.tr"
    if ((!_tr_dict_contains(self->fn_sigs, _tr_strz(fname)))) {
#line 2168 "src/sema.tr"
        return nohint;
    }
#line 2169 "src/sema.tr"
    AstType* sig = ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(fname)));
#line 2170 "src/sema.tr"
    if ((k >= (sig->args->len - 1LL))) {
#line 2170 "src/sema.tr"
        AstType** _trret201 = nohint;
        return _trret201;
    }
#line 2171 "src/sema.tr"
    AstType** pt = ((AstType**)List_ptr_get(sig->args, k));
#line 2172 "src/sema.tr"
    if ((((unsigned long long)(pt)) == ((unsigned long long)(0LL)))) {
#line 2172 "src/sema.tr"
        AstType** _trret202 = nohint;
        return _trret202;
    }
#line 2173 "src/sema.tr"
    TrStr pn = _tr_str_retain((*pt)->name);
#line 2174 "src/sema.tr"
    if (((_tr_str_eqv((pn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((pn), (_tr_str_lit_len("Vec", 3LL)))) && ((*pt)->args->len > 0LL))) {
#line 2174 "src/sema.tr"
        AstType** _trret203 = pt;
        _tr_str_release(pn);
        return _trret203;
    }
#line 2175 "src/sema.tr"
    AstType** _trret204 = nohint;
    _tr_str_release(pn);
    return _trret204;
}

__attribute__((hot)) AstType** Sema_list_hint_for_method(Sema* self, TrStr method, long long k) {
#line 2180 "src/sema.tr"
    AstType** nohint = (AstType**)(0LL);
#line 2181 "src/sema.tr"
    AstType** found = (AstType**)(0LL);
#line 2182 "src/sema.tr"
    List_TrStr* keys = _tr_dict_keys(self->classes);
#line 2183 "src/sema.tr"
    long long ci = 0LL;
#line 2184 "src/sema.tr"
    while ((ci < keys->len)) {
#line 2185 "src/sema.tr"
        ClassDef* cd = ({ TrStr _ord_t1178 = (List_TrStr_get(keys, ci)); __auto_type _wr = (((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_ord_t1178)))); _tr_str_release(_ord_t1178); _wr; });
#line 2186 "src/sema.tr"
        long long mi = 0LL;
#line 2187 "src/sema.tr"
        while ((mi < cd->methods->len)) {
#line 2188 "src/sema.tr"
            FunctionDef* md = ((FunctionDef*)List_ptr_get(cd->methods, mi));
#line 2189 "src/sema.tr"
            if (_tr_str_eqv((md->name), (method))) {
#line 2190 "src/sema.tr"
                long long idx = 0LL;
#line 2191 "src/sema.tr"
                long long pi = 0LL;
#line 2192 "src/sema.tr"
                while ((pi < md->params->len)) {
#line 2193 "src/sema.tr"
                    Param* prm = ((Param*)List_ptr_get(md->params, pi));
#line 2194 "src/sema.tr"
                    if ((!_tr_str_eqv((prm->name), (_tr_str_lit_len("self", 4LL))))) {
#line 2195 "src/sema.tr"
                        if (((idx == k) && (((unsigned long long)(prm->ty)) != ((unsigned long long)(0LL))))) {
#line 2196 "src/sema.tr"
                            TrStr pn = _tr_str_retain((*prm->ty)->name);
#line 2197 "src/sema.tr"
                            if (((_tr_str_eqv((pn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((pn), (_tr_str_lit_len("Vec", 3LL)))) && ((*prm->ty)->args->len > 0LL))) {
#line 2198 "src/sema.tr"
                                TrStr en = _tr_str_retain((*((AstType**)List_ptr_get((*prm->ty)->args, 0LL)))->name);
#line 2199 "src/sema.tr"
                                if ((((unsigned long long)(found)) == ((unsigned long long)(0LL)))) {
#line 2200 "src/sema.tr"
                                    found = prm->ty;
                                } else if (((!_tr_str_eqv(((*found)->name), (pn))) || (!_tr_str_eqv(((*((AstType**)List_ptr_get((*found)->args, 0LL)))->name), (en))))) {
#line 2202 "src/sema.tr"
                                    AstType** _trret205 = nohint;
                                    List_TrStr_free(keys);
                                    _tr_str_release(pn);
                                    _tr_str_release(en);
                                    return _trret205;
                                }
                            } else {
#line 2204 "src/sema.tr"
                                AstType** _trret206 = nohint;
                                List_TrStr_free(keys);
                                _tr_str_release(pn);
                                return _trret206;
                            }
                        }
#line 2205 "src/sema.tr"
                        idx = (idx + 1LL);
                    }
#line 2206 "src/sema.tr"
                    pi = (pi + 1LL);
                }
            }
#line 2207 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 2208 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 2209 "src/sema.tr"
    AstType** _trret207 = found;
    List_TrStr_free(keys);
    return _trret207;
}

__attribute__((hot)) void Sema_collect_idents(Sema* self, HirExpr* e, List_TrStr* out) {
#line 2212 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2212 "src/sema.tr"
        return;
    }
#line 2213 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2213 "src/sema.tr"
        return;
    }
#line 2214 "src/sema.tr"
    __auto_type _t1179 = (*e);
    if (_t1179.tag == HirExpr_EIdent) {
        __auto_type nm = _t1179.data.EIdent.name;
        List_TrStr_append(out, nm);
    } else if (_t1179.tag == HirExpr_EBinOp) {
        __auto_type l = _t1179.data.EBinOp.left;
__auto_type r = _t1179.data.EBinOp.right;
#line 2217 "src/sema.tr"
        Sema_collect_idents(self, l, out);
#line 2218 "src/sema.tr"
        Sema_collect_idents(self, r, out);
    } else if (_t1179.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t1179.data.EUnaryOp.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1179.tag == HirExpr_ECall) {
        __auto_type callee = _t1179.data.ECall.callee;
__auto_type args = _t1179.data.ECall.args;
#line 2221 "src/sema.tr"
        Sema_collect_idents(self, callee, out);
#line 2222 "src/sema.tr"
        long long i = 0LL;
#line 2223 "src/sema.tr"
        while ((i < args->len)) {
#line 2224 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(args, i)), out);
#line 2225 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1179.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1179.data.EMethodCall.obj;
__auto_type args = _t1179.data.EMethodCall.args;
#line 2227 "src/sema.tr"
        Sema_collect_idents(self, obj, out);
#line 2228 "src/sema.tr"
        long long i = 0LL;
#line 2229 "src/sema.tr"
        while ((i < args->len)) {
#line 2230 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(args, i)), out);
#line 2231 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1179.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t1179.data.EPropAccess.obj;
        Sema_collect_idents(self, obj, out);
    } else if (_t1179.tag == HirExpr_EIndex) {
        __auto_type obj = _t1179.data.EIndex.obj;
__auto_type idx = _t1179.data.EIndex._tr_v_index;
#line 2234 "src/sema.tr"
        Sema_collect_idents(self, obj, out);
#line 2235 "src/sema.tr"
        Sema_collect_idents(self, idx, out);
    } else if (_t1179.tag == HirExpr_ECast) {
        __auto_type inner = _t1179.data.ECast.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1179.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t1179.data.ETryExpr.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1179.tag == HirExpr_EIfElse) {
        __auto_type c = _t1179.data.EIfElse.cond;
__auto_type t = _t1179.data.EIfElse.then_e;
__auto_type f = _t1179.data.EIfElse.else_e;
#line 2239 "src/sema.tr"
        Sema_collect_idents(self, c, out);
#line 2240 "src/sema.tr"
        Sema_collect_idents(self, t, out);
#line 2241 "src/sema.tr"
        Sema_collect_idents(self, f, out);
    } else if (_t1179.tag == HirExpr_EList) {
        __auto_type items = _t1179.data.EList.items;
#line 2243 "src/sema.tr"
        long long i = 0LL;
#line 2244 "src/sema.tr"
        while ((i < items->len)) {
#line 2245 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
#line 2246 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1179.tag == HirExpr_ESet) {
        __auto_type items = _t1179.data.ESet.items;
#line 2248 "src/sema.tr"
        long long i = 0LL;
#line 2249 "src/sema.tr"
        while ((i < items->len)) {
#line 2250 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
#line 2251 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1179.tag == HirExpr_ETuple) {
        __auto_type items = _t1179.data.ETuple.items;
#line 2253 "src/sema.tr"
        long long i = 0LL;
#line 2254 "src/sema.tr"
        while ((i < items->len)) {
#line 2255 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
#line 2256 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1179.tag == HirExpr_EDict) {
        __auto_type keys = _t1179.data.EDict.keys;
__auto_type vals = _t1179.data.EDict.vals;
#line 2258 "src/sema.tr"
        long long i = 0LL;
#line 2259 "src/sema.tr"
        while ((i < keys->len)) {
#line 2260 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(keys, i)), out);
#line 2261 "src/sema.tr"
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(vals, i)), out);
#line 2262 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1179.tag == HirExpr_ERange) {
        __auto_type s = _t1179.data.ERange.start;
__auto_type en = _t1179.data.ERange.end;
#line 2264 "src/sema.tr"
        Sema_collect_idents(self, s, out);
#line 2265 "src/sema.tr"
        Sema_collect_idents(self, en, out);
    } else if (_t1179.tag == HirExpr_EAwait) {
        __auto_type inner = _t1179.data.EAwait.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1179.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t1179.data.EAwaitTimeout.expr;
__auto_type to = _t1179.data.EAwaitTimeout.timeout_ms;
#line 2268 "src/sema.tr"
        Sema_collect_idents(self, inner, out);
#line 2269 "src/sema.tr"
        Sema_collect_idents(self, to, out);
    } else if (_t1179.tag == HirExpr_EYield) {
        __auto_type inner = _t1179.data.EYield.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t1179.tag == HirExpr_EFString) {
        __auto_type fparts = _t1179.data.EFString.parts;
#line 2274 "src/sema.tr"
        long long fpi = 0LL;
#line 2275 "src/sema.tr"
        while ((fpi < fparts->len)) {
#line 2276 "src/sema.tr"
            HirFStringPart* fpart = ((HirFStringPart*)List_ptr_get(fparts, fpi));
#line 2277 "src/sema.tr"
            if ((fpart->is_expr && (((unsigned long long)(fpart->expr)) != ((unsigned long long)(0LL))))) {
#line 2278 "src/sema.tr"
                Sema_collect_idents(self, fpart->expr, out);
            }
#line 2279 "src/sema.tr"
            long long fsa = 0LL;
#line 2280 "src/sema.tr"
            while ((fsa < fpart->spec_args->len)) {
#line 2281 "src/sema.tr"
                Sema_collect_idents(self, ((HirExpr*)List_ptr_get(fpart->spec_args, fsa)), out);
#line 2282 "src/sema.tr"
                fsa = (fsa + 1LL);
            }
#line 2283 "src/sema.tr"
            fpi = (fpi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1179;
        /* pass */
    }
}

__attribute__((hot)) bool Sema_is_local_var(Sema* self, TrStr name) {
#line 2288 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 2289 "src/sema.tr"
    while ((i >= 0LL)) {
#line 2290 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 2290 "src/sema.tr"
            return true;
        }
#line 2291 "src/sema.tr"
        i = (i - 1LL);
    }
#line 2292 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema_mark_str_escaped(Sema* self, TrStr name) {
#line 2295 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 2296 "src/sema.tr"
    while ((i >= 0LL)) {
#line 2297 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 2298 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 2299 "src/sema.tr"
            sym->str_escaped = true;
#line 2300 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 2301 "src/sema.tr"
            return;
        }
#line 2302 "src/sema.tr"
        i = (i - 1LL);
    }
}

__attribute__((hot)) void Sema_set_borrows_region(Sema* self, TrStr name, TrStr region) {
#line 2307 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 2308 "src/sema.tr"
    while ((i >= 0LL)) {
#line 2309 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 2310 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 2311 "src/sema.tr"
            sym->borrows_region = _tr_str_retain(region);
#line 2312 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 2313 "src/sema.tr"
            return;
        }
#line 2314 "src/sema.tr"
        i = (i - 1LL);
    }
}

__attribute__((hot)) TrStr Sema_compute_region(Sema* self, Expr* e) {
#line 2322 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2322 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 2323 "src/sema.tr"
    __auto_type _t1180 = (*e);
    if (_t1180.tag == Expr_EBinOp) {
        __auto_type cr_op = _t1180.data.EBinOp.op;
#line 2325 "src/sema.tr"
        if (_tr_str_eqv((cr_op), (_tr_str_lit_len("+", 1LL)))) {
#line 2325 "src/sema.tr"
            return _tr_str_lit_len("@owned", 6LL);
        }
#line 2326 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t1180.tag == Expr_EIdent) {
        __auto_type cr_nm = _t1180.data.EIdent.name;
#line 2328 "src/sema.tr"
        Symbol* cr_sym = Sema_resolve(self, cr_nm);
#line 2329 "src/sema.tr"
        if (_tr_str_eqv((cr_sym->name), (_tr_str_lit_len("", 0LL)))) {
#line 2329 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 2330 "src/sema.tr"
        if (cr_sym->is_param) {
#line 2330 "src/sema.tr"
            TrStr _trret208 = _tr_str_retain(cr_nm);
            return _trret208;
        }
#line 2331 "src/sema.tr"
        TrStr _trret209 = _tr_str_retain(cr_sym->borrows_region);
        return _trret209;
    } else if (_t1180.tag == Expr_ECall) {
        __auto_type cr_callee = _t1180.data.ECall.callee;
__auto_type cr_args = _t1180.data.ECall.args;
#line 2333 "src/sema.tr"
        if ((((unsigned long long)(cr_callee)) != ((unsigned long long)(0LL)))) {
#line 2334 "src/sema.tr"
            __auto_type _t1181 = (*cr_callee);
            if (_t1181.tag == Expr_EIdent) {
                __auto_type cr_fn = _t1181.data.EIdent.name;
#line 2337 "src/sema.tr"
                if (_tr_dict_contains(self->classes, _tr_strz(cr_fn))) {
#line 2337 "src/sema.tr"
                    return _tr_str_lit_len("@owned", 6LL);
                }
#line 2338 "src/sema.tr"
                Symbol* cr_fsym = Sema_resolve(self, cr_fn);
#line 2339 "src/sema.tr"
                if (((!_tr_str_eqv((cr_fsym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(cr_fsym->ty)) != ((unsigned long long)(0LL))))) {
#line 2340 "src/sema.tr"
                    AstType* cr_ret = (*cr_fsym->ty);
#line 2341 "src/sema.tr"
                    if (cr_ret->is_borrow) {
#line 2343 "src/sema.tr"
                        if (((cr_ret->from_index >= 0LL) && (cr_ret->from_index < cr_args->len))) {
#line 2344 "src/sema.tr"
                            TrStr _trret210 = Sema_compute_region(self, ((Expr*)List_ptr_get(cr_args, cr_ret->from_index)));
                            return _trret210;
                        }
#line 2345 "src/sema.tr"
                        return _tr_str_lit_len("", 0LL);
                    }
#line 2347 "src/sema.tr"
                    return _tr_str_lit_len("@owned", 6LL);
                }
            } else if (1) {
                __auto_type _ = _t1181;
                /* pass */
            }
        }
#line 2349 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t1180;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool Sema_region_outlives(Sema* self, TrStr longer, TrStr shorter) {
#line 2356 "src/sema.tr"
    long long i = 0LL;
#line 2357 "src/sema.tr"
    while ((i < self->current_func_outlives_a->len)) {
#line 2358 "src/sema.tr"
        if ((({ TrStr _cmpa_t1182 = (List_TrStr_get(self->current_func_outlives_a, i)); TrStr _cmpb_t1183 = (longer); _Bool _cmpr_t1184 = _tr_str_eqv(_cmpa_t1182, _cmpb_t1183); _tr_str_release(_cmpa_t1182); _cmpr_t1184; }) && ({ TrStr _cmpa_t1185 = (List_TrStr_get(self->current_func_outlives_b, i)); TrStr _cmpb_t1186 = (shorter); _Bool _cmpr_t1187 = _tr_str_eqv(_cmpa_t1185, _cmpb_t1186); _tr_str_release(_cmpa_t1185); _cmpr_t1187; }))) {
#line 2359 "src/sema.tr"
            return true;
        }
#line 2360 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2361 "src/sema.tr"
    long long j = 0LL;
#line 2362 "src/sema.tr"
    while ((j < self->current_func_outlives_a->len)) {
#line 2363 "src/sema.tr"
        if (({ TrStr _cmpa_t1188 = (List_TrStr_get(self->current_func_outlives_a, j)); TrStr _cmpb_t1189 = (longer); _Bool _cmpr_t1190 = _tr_str_eqv(_cmpa_t1188, _cmpb_t1189); _tr_str_release(_cmpa_t1188); _cmpr_t1190; })) {
#line 2364 "src/sema.tr"
            TrStr mid = List_TrStr_get(self->current_func_outlives_b, j);
#line 2365 "src/sema.tr"
            long long kk = 0LL;
#line 2366 "src/sema.tr"
            while ((kk < self->current_func_outlives_a->len)) {
#line 2367 "src/sema.tr"
                if ((({ TrStr _cmpa_t1191 = (List_TrStr_get(self->current_func_outlives_a, kk)); TrStr _cmpb_t1192 = (mid); _Bool _cmpr_t1193 = _tr_str_eqv(_cmpa_t1191, _cmpb_t1192); _tr_str_release(_cmpa_t1191); _cmpr_t1193; }) && ({ TrStr _cmpa_t1194 = (List_TrStr_get(self->current_func_outlives_b, kk)); TrStr _cmpb_t1195 = (shorter); _Bool _cmpr_t1196 = _tr_str_eqv(_cmpa_t1194, _cmpb_t1195); _tr_str_release(_cmpa_t1194); _cmpr_t1196; }))) {
#line 2368 "src/sema.tr"
                    _tr_str_release(mid);
                    return true;
                }
#line 2369 "src/sema.tr"
                kk = (kk + 1LL);
            }
        }
#line 2370 "src/sema.tr"
        j = (j + 1LL);
    }
#line 2371 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_field_is_borrow(Sema* self, Expr* obj, TrStr field) {
#line 2376 "src/sema.tr"
    if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
#line 2376 "src/sema.tr"
        return false;
    }
#line 2377 "src/sema.tr"
    TrStr cls_name = _tr_str_lit_len("", 0LL);
#line 2378 "src/sema.tr"
    __auto_type _t1197 = (*obj);
    if (_t1197.tag == Expr_EIdent) {
        __auto_type fb_obj = _t1197.data.EIdent.name;
#line 2380 "src/sema.tr"
        if (_tr_str_eqv((fb_obj), (_tr_str_lit_len("self", 4LL)))) {
#line 2381 "src/sema.tr"
            TrStr _strtmp_t1198 = _tr_str_retain(self->current_class_name);
            _tr_str_release(cls_name);
            cls_name = _strtmp_t1198;
        } else {
#line 2383 "src/sema.tr"
            Symbol* fb_sym = Sema_resolve(self, fb_obj);
#line 2384 "src/sema.tr"
            if (((!_tr_str_eqv((fb_sym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(fb_sym->ty)) != ((unsigned long long)(0LL))))) {
#line 2385 "src/sema.tr"
                TrStr _strtmp_t1199 = _tr_str_retain((*fb_sym->ty)->name);
                _tr_str_release(cls_name);
                cls_name = _strtmp_t1199;
            }
        }
    } else if (1) {
        __auto_type _ = _t1197;
        /* pass */
    }
#line 2387 "src/sema.tr"
    if (_tr_str_eqv((cls_name), (_tr_str_lit_len("", 0LL)))) {
#line 2387 "src/sema.tr"
        _tr_str_release(cls_name);
        return false;
    }
#line 2388 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
#line 2388 "src/sema.tr"
        _tr_str_release(cls_name);
        return false;
    }
#line 2389 "src/sema.tr"
    ClassDef* fb_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
#line 2390 "src/sema.tr"
    long long fb_i = 0LL;
#line 2391 "src/sema.tr"
    while ((fb_i < fb_cls->fields->len)) {
#line 2392 "src/sema.tr"
        FieldDef* fb_fld = ((FieldDef*)List_ptr_get(fb_cls->fields, fb_i));
#line 2393 "src/sema.tr"
        if (_tr_str_eqv((fb_fld->name), (field))) {
#line 2394 "src/sema.tr"
            if ((((unsigned long long)(fb_fld->ty)) != ((unsigned long long)(0LL)))) {
#line 2395 "src/sema.tr"
                bool _trret211 = (*fb_fld->ty)->is_borrow;
                _tr_str_release(cls_name);
                return _trret211;
            }
        }
#line 2396 "src/sema.tr"
        fb_i = (fb_i + 1LL);
    }
#line 2397 "src/sema.tr"
    _tr_str_release(cls_name);
    return false;
}

__attribute__((hot)) void Sema_mark_str_arg(Sema* self, HirExpr* e) {
#line 2405 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2405 "src/sema.tr"
        return;
    }
#line 2406 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2406 "src/sema.tr"
        return;
    }
#line 2407 "src/sema.tr"
    __auto_type _t1200 = (*e);
    if (_t1200.tag == HirExpr_EIdent) {
        __auto_type nm = _t1200.data.EIdent.name;
#line 2409 "src/sema.tr"
        if (_tr_str_eqv((hir_expr_type(e)->name), (_tr_str_lit_len("str", 3LL)))) {
#line 2409 "src/sema.tr"
            Sema_mark_str_escaped(self, nm);
        }
    } else if (1) {
        __auto_type _ = _t1200;
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_escaped_str_args(Sema* self, HirExpr* e) {
#line 2415 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2415 "src/sema.tr"
        return;
    }
#line 2416 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2416 "src/sema.tr"
        return;
    }
#line 2417 "src/sema.tr"
    __auto_type _t1201 = (*e);
    if (_t1201.tag == HirExpr_EBinOp) {
        __auto_type l = _t1201.data.EBinOp.left;
__auto_type r = _t1201.data.EBinOp.right;
#line 2419 "src/sema.tr"
        Sema_mark_escaped_str_args(self, l);
#line 2420 "src/sema.tr"
        Sema_mark_escaped_str_args(self, r);
    } else if (_t1201.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t1201.data.EUnaryOp.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1201.tag == HirExpr_ECall) {
        __auto_type callee = _t1201.data.ECall.callee;
__auto_type args = _t1201.data.ECall.args;
#line 2423 "src/sema.tr"
        Sema_mark_escaped_str_args(self, callee);
#line 2434 "src/sema.tr"
        bool _call_is_c_free = false;
#line 2435 "src/sema.tr"
        __auto_type _t1202 = (*callee);
        if (_t1202.tag == HirExpr_EIdent) {
            __auto_type _cnm = _t1202.data.EIdent.name;
#line 2437 "src/sema.tr"
            if (_tr_str_eqv((_cnm), (_tr_str_lit_len("_tr_c_free", 10LL)))) {
#line 2437 "src/sema.tr"
                _call_is_c_free = true;
            }
        } else if (1) {
            __auto_type _ = _t1202;
            /* pass */
        }
#line 2439 "src/sema.tr"
        long long i = 0LL;
#line 2440 "src/sema.tr"
        while ((i < args->len)) {
#line 2441 "src/sema.tr"
            if (_call_is_c_free) {
#line 2449 "src/sema.tr"
                __auto_type _t1203 = (*((HirExpr*)List_ptr_get(args, i)));
                if (_t1203.tag == HirExpr_ECast) {
                    __auto_type _cf_inner = _t1203.data.ECast.expr;
#line 2451 "src/sema.tr"
                    Sema_mark_str_arg(self, _cf_inner);
                } else if (1) {
                    __auto_type _ = _t1203;
                    /* pass */
                }
            }
#line 2453 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2454 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1201.data.EMethodCall.obj;
__auto_type mname = _t1201.data.EMethodCall.method;
__auto_type args = _t1201.data.EMethodCall.args;
#line 2456 "src/sema.tr"
        Sema_mark_escaped_str_args(self, obj);
#line 2475 "src/sema.tr"
        long long i = 0LL;
#line 2476 "src/sema.tr"
        while ((i < args->len)) {
#line 2477 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2478 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t1201.data.EPropAccess.obj;
        Sema_mark_escaped_str_args(self, obj);
    } else if (_t1201.tag == HirExpr_EIndex) {
        __auto_type obj = _t1201.data.EIndex.obj;
__auto_type idx = _t1201.data.EIndex._tr_v_index;
#line 2481 "src/sema.tr"
        Sema_mark_escaped_str_args(self, obj);
#line 2482 "src/sema.tr"
        Sema_mark_escaped_str_args(self, idx);
    } else if (_t1201.tag == HirExpr_ECast) {
        __auto_type inner = _t1201.data.ECast.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1201.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t1201.data.ETryExpr.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1201.tag == HirExpr_EIfElse) {
        __auto_type c = _t1201.data.EIfElse.cond;
__auto_type t = _t1201.data.EIfElse.then_e;
__auto_type f = _t1201.data.EIfElse.else_e;
#line 2486 "src/sema.tr"
        Sema_mark_escaped_str_args(self, c);
#line 2487 "src/sema.tr"
        Sema_mark_escaped_str_args(self, t);
#line 2488 "src/sema.tr"
        Sema_mark_escaped_str_args(self, f);
    } else if (_t1201.tag == HirExpr_EList) {
        __auto_type items = _t1201.data.EList.items;
#line 2490 "src/sema.tr"
        long long i = 0LL;
#line 2491 "src/sema.tr"
        while ((i < items->len)) {
#line 2492 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2493 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_ESet) {
        __auto_type items = _t1201.data.ESet.items;
#line 2495 "src/sema.tr"
        long long i = 0LL;
#line 2496 "src/sema.tr"
        while ((i < items->len)) {
#line 2497 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2498 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_ETuple) {
        __auto_type items = _t1201.data.ETuple.items;
#line 2500 "src/sema.tr"
        long long i = 0LL;
#line 2501 "src/sema.tr"
        while ((i < items->len)) {
#line 2502 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2503 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_EDict) {
        __auto_type keys = _t1201.data.EDict.keys;
__auto_type vals = _t1201.data.EDict.vals;
#line 2505 "src/sema.tr"
        long long i = 0LL;
#line 2506 "src/sema.tr"
        while ((i < keys->len)) {
#line 2507 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(keys, i)));
#line 2508 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(vals, i)));
#line 2509 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_ERange) {
        __auto_type s = _t1201.data.ERange.start;
__auto_type en = _t1201.data.ERange.end;
#line 2511 "src/sema.tr"
        Sema_mark_escaped_str_args(self, s);
#line 2512 "src/sema.tr"
        Sema_mark_escaped_str_args(self, en);
    } else if (_t1201.tag == HirExpr_EAwait) {
        __auto_type inner = _t1201.data.EAwait.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1201.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t1201.data.EAwaitTimeout.expr;
__auto_type to = _t1201.data.EAwaitTimeout.timeout_ms;
#line 2515 "src/sema.tr"
        Sema_mark_escaped_str_args(self, inner);
#line 2516 "src/sema.tr"
        Sema_mark_escaped_str_args(self, to);
    } else if (_t1201.tag == HirExpr_EYield) {
        __auto_type inner = _t1201.data.EYield.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t1201.tag == HirExpr_EFString) {
        __auto_type parts = _t1201.data.EFString.parts;
#line 2521 "src/sema.tr"
        long long i = 0LL;
#line 2522 "src/sema.tr"
        while ((i < parts->len)) {
#line 2523 "src/sema.tr"
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, i));
#line 2524 "src/sema.tr"
            if (part->is_expr) {
#line 2524 "src/sema.tr"
                Sema_mark_escaped_str_args(self, part->expr);
            }
#line 2525 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_EClosure) {
        __auto_type captures = _t1201.data.EClosure.captures;
#line 2532 "src/sema.tr"
        long long i = 0LL;
#line 2533 "src/sema.tr"
        while ((i < captures->len)) {
#line 2534 "src/sema.tr"
            HirParam* cap = ((HirParam*)List_ptr_get(captures, i));
#line 2535 "src/sema.tr"
            if (_tr_str_eqv((cap->ty->name), (_tr_str_lit_len("str", 3LL)))) {
#line 2535 "src/sema.tr"
                Sema_mark_str_escaped(self, cap->name);
            }
#line 2536 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t1201.data.ESuperMethodCall.args;
#line 2540 "src/sema.tr"
        long long i = 0LL;
#line 2541 "src/sema.tr"
        while ((i < args->len)) {
#line 2542 "src/sema.tr"
            Sema_mark_str_arg(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2543 "src/sema.tr"
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2544 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_ESuperPropAccess) {
        /* pass */
    } else if (_t1201.tag == HirExpr_EListComp) {
        __auto_type element = _t1201.data.EListComp.element;
__auto_type generators = _t1201.data.EListComp.generators;
#line 2550 "src/sema.tr"
        Sema_mark_str_arg(self, element);
#line 2551 "src/sema.tr"
        Sema_mark_escaped_str_args(self, element);
#line 2552 "src/sema.tr"
        long long i = 0LL;
#line 2553 "src/sema.tr"
        while ((i < generators->len)) {
#line 2554 "src/sema.tr"
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
#line 2555 "src/sema.tr"
            Sema_mark_escaped_str_args(self, gen->iter);
#line 2556 "src/sema.tr"
            long long j = 0LL;
#line 2557 "src/sema.tr"
            while ((j < gen->ifs->len)) {
#line 2558 "src/sema.tr"
                Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
#line 2559 "src/sema.tr"
                j = (j + 1LL);
            }
#line 2560 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_EGeneratorExpr) {
        __auto_type element = _t1201.data.EGeneratorExpr.element;
__auto_type generators = _t1201.data.EGeneratorExpr.generators;
#line 2562 "src/sema.tr"
        Sema_mark_str_arg(self, element);
#line 2563 "src/sema.tr"
        Sema_mark_escaped_str_args(self, element);
#line 2564 "src/sema.tr"
        long long i = 0LL;
#line 2565 "src/sema.tr"
        while ((i < generators->len)) {
#line 2566 "src/sema.tr"
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
#line 2567 "src/sema.tr"
            Sema_mark_escaped_str_args(self, gen->iter);
#line 2568 "src/sema.tr"
            long long j = 0LL;
#line 2569 "src/sema.tr"
            while ((j < gen->ifs->len)) {
#line 2570 "src/sema.tr"
                Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
#line 2571 "src/sema.tr"
                j = (j + 1LL);
            }
#line 2572 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1201.tag == HirExpr_ESlice) {
        __auto_type start = _t1201.data.ESlice.start;
__auto_type stop = _t1201.data.ESlice.stop;
__auto_type step = _t1201.data.ESlice.step;
#line 2574 "src/sema.tr"
        Sema_mark_escaped_str_args(self, start);
#line 2575 "src/sema.tr"
        Sema_mark_escaped_str_args(self, stop);
#line 2576 "src/sema.tr"
        Sema_mark_escaped_str_args(self, step);
    } else if (_t1201.tag == HirExpr_ETry) {
        /* pass */
    } else if (_t1201.tag == HirExpr_EDo) {
        __auto_type _do_b = _t1201.data.EDo.body;
        /* pass */
    } else if (_t1201.tag == HirExpr_EMatchExpr) {
        /* pass */
    } else if (_t1201.tag == HirExpr_ELoop) {
        /* pass */
    } else if (_t1201.tag == HirExpr_EWhileExpr) {
        /* pass */
    } else if (_t1201.tag == HirExpr_ELitInt) {
        /* pass */
    } else if (_t1201.tag == HirExpr_ELitFloat) {
        /* pass */
    } else if (_t1201.tag == HirExpr_ELitStr) {
        /* pass */
    } else if (_t1201.tag == HirExpr_ELitBytes) {
        /* pass */
    } else if (_t1201.tag == HirExpr_ERawStr) {
        /* pass */
    } else if (_t1201.tag == HirExpr_ELitChar) {
        /* pass */
    } else if (_t1201.tag == HirExpr_ELitBool) {
        /* pass */
    } else if (_t1201.tag == HirExpr_ELitNone) {
        __auto_type _ = _t1201.data.ELitNone.ty;
        /* pass */
    } else if (_t1201.tag == HirExpr_ESizeOf) {
        /* pass */
    } else if (_t1201.tag == HirExpr_EIdent) {
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_coll_escaped(Sema* self, TrStr name) {
#line 2605 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 2606 "src/sema.tr"
    while ((i >= 0LL)) {
#line 2607 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 2608 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
#line 2609 "src/sema.tr"
            sym->coll_escaped = true;
#line 2610 "src/sema.tr"
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
#line 2611 "src/sema.tr"
            return;
        }
#line 2612 "src/sema.tr"
        i = (i - 1LL);
    }
}

__attribute__((hot)) void Sema_mark_coll_arg(Sema* self, HirExpr* e) {
#line 2621 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2621 "src/sema.tr"
        return;
    }
#line 2622 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2622 "src/sema.tr"
        return;
    }
#line 2623 "src/sema.tr"
    __auto_type _t1204 = (*e);
    if (_t1204.tag == HirExpr_EIdent) {
        __auto_type nm = _t1204.data.EIdent.name;
#line 2625 "src/sema.tr"
        TrStr tn = _tr_str_retain(hir_expr_type(e)->name);
#line 2626 "src/sema.tr"
        if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 2627 "src/sema.tr"
            Sema_mark_coll_escaped(self, nm);
        } else if ((_tr_str_eqv((tn), (_tr_str_lit_len("def", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("lambda", 6LL))))) {
#line 2632 "src/sema.tr"
            Sema_mark_coll_escaped(self, nm);
        } else if (_tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL)))) {
#line 2636 "src/sema.tr"
            Sema_mark_coll_escaped(self, nm);
        } else if (_tr_dict_contains(self->classes, _tr_strz(tn))) {
#line 2643 "src/sema.tr"
            Sema_mark_coll_escaped(self, nm);
        }
        _tr_str_release(tn);
    } else if (1) {
        __auto_type _ = _t1204;
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_escaped_coll_args(Sema* self, HirExpr* e) {
#line 2652 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 2652 "src/sema.tr"
        return;
    }
#line 2653 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 2653 "src/sema.tr"
        return;
    }
#line 2654 "src/sema.tr"
    __auto_type _t1205 = (*e);
    if (_t1205.tag == HirExpr_EBinOp) {
        __auto_type l = _t1205.data.EBinOp.left;
__auto_type r = _t1205.data.EBinOp.right;
#line 2656 "src/sema.tr"
        Sema_mark_coll_arg(self, l);
#line 2657 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, l);
#line 2658 "src/sema.tr"
        Sema_mark_coll_arg(self, r);
#line 2659 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, r);
    } else if (_t1205.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t1205.data.EUnaryOp.expr;
#line 2661 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2662 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1205.tag == HirExpr_ECall) {
        __auto_type callee = _t1205.data.ECall.callee;
__auto_type args = _t1205.data.ECall.args;
#line 2664 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, callee);
#line 2665 "src/sema.tr"
        long long i = 0LL;
#line 2666 "src/sema.tr"
        while ((i < args->len)) {
#line 2667 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2668 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2669 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1205.data.EMethodCall.obj;
__auto_type method = _t1205.data.EMethodCall.method;
__auto_type args = _t1205.data.EMethodCall.args;
#line 2685 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("values", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("items", 5LL))))) {
#line 2686 "src/sema.tr"
            __auto_type _t1206 = (*obj);
            if (_t1206.tag == HirExpr_EIdent) {
#line 2688 "src/sema.tr"
                AstType* obj_ty = hir_expr_type(obj);
#line 2689 "src/sema.tr"
                if ((_tr_str_eqv((obj_ty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((obj_ty->name), (_tr_str_lit_len("Map", 3LL))))) {
#line 2690 "src/sema.tr"
                    if ((obj_ty->args->len > 1LL)) {
#line 2691 "src/sema.tr"
                        TrStr obj_vt = _tr_str_retain((*((AstType**)List_ptr_get(obj_ty->args, 1LL)))->name);
#line 2692 "src/sema.tr"
                        if ((_tr_str_eqv((obj_vt), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((obj_vt), (_tr_str_lit_len("String", 6LL))))) {
#line 2693 "src/sema.tr"
                            Sema_mark_coll_arg(self, obj);
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1206;
                /* pass */
            }
        }
#line 2702 "src/sema.tr"
        bool _coll_static_call = false;
#line 2703 "src/sema.tr"
        __auto_type _t1207 = (*obj);
        if (_t1207.tag == HirExpr_EIdent) {
            __auto_type _ocnm2 = _t1207.data.EIdent.name;
#line 2705 "src/sema.tr"
            if ((_tr_dict_contains(self->classes, _tr_strz(_ocnm2)) && (!Sema_is_local_var(self, _ocnm2)))) {
#line 2705 "src/sema.tr"
                _coll_static_call = true;
            }
        } else if (1) {
            __auto_type _ = _t1207;
            /* pass */
        }
#line 2707 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, obj);
#line 2708 "src/sema.tr"
        long long i = 0LL;
#line 2709 "src/sema.tr"
        while ((i < args->len)) {
#line 2710 "src/sema.tr"
            if ((!_coll_static_call)) {
#line 2710 "src/sema.tr"
                Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            }
#line 2711 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2712 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t1205.data.EPropAccess.obj;
        Sema_mark_escaped_coll_args(self, obj);
    } else if (_t1205.tag == HirExpr_EIndex) {
        __auto_type obj = _t1205.data.EIndex.obj;
__auto_type idx = _t1205.data.EIndex._tr_v_index;
#line 2715 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, obj);
#line 2716 "src/sema.tr"
        Sema_mark_coll_arg(self, idx);
#line 2717 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, idx);
    } else if (_t1205.tag == HirExpr_ECast) {
        __auto_type inner = _t1205.data.ECast.expr;
#line 2719 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2720 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1205.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t1205.data.ETryExpr.expr;
#line 2722 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2723 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1205.tag == HirExpr_EIfElse) {
        __auto_type c = _t1205.data.EIfElse.cond;
__auto_type t = _t1205.data.EIfElse.then_e;
__auto_type f = _t1205.data.EIfElse.else_e;
#line 2725 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, c);
#line 2726 "src/sema.tr"
        Sema_mark_coll_arg(self, t);
#line 2727 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, t);
#line 2728 "src/sema.tr"
        Sema_mark_coll_arg(self, f);
#line 2729 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, f);
    } else if (_t1205.tag == HirExpr_EList) {
        __auto_type items = _t1205.data.EList.items;
#line 2731 "src/sema.tr"
        long long i = 0LL;
#line 2732 "src/sema.tr"
        while ((i < items->len)) {
#line 2733 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2734 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2735 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_ESet) {
        __auto_type items = _t1205.data.ESet.items;
#line 2737 "src/sema.tr"
        long long i = 0LL;
#line 2738 "src/sema.tr"
        while ((i < items->len)) {
#line 2739 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2740 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2741 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_ETuple) {
        __auto_type items = _t1205.data.ETuple.items;
#line 2743 "src/sema.tr"
        long long i = 0LL;
#line 2744 "src/sema.tr"
        while ((i < items->len)) {
#line 2745 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2746 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
#line 2747 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_EDict) {
        __auto_type keys = _t1205.data.EDict.keys;
__auto_type vals = _t1205.data.EDict.vals;
#line 2749 "src/sema.tr"
        long long i = 0LL;
#line 2750 "src/sema.tr"
        while ((i < keys->len)) {
#line 2751 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(keys, i)));
#line 2752 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(keys, i)));
#line 2753 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(vals, i)));
#line 2754 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(vals, i)));
#line 2755 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_ERange) {
        __auto_type s = _t1205.data.ERange.start;
__auto_type en = _t1205.data.ERange.end;
#line 2757 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, s);
#line 2758 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, en);
    } else if (_t1205.tag == HirExpr_EAwait) {
        __auto_type inner = _t1205.data.EAwait.expr;
#line 2760 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2761 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1205.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t1205.data.EAwaitTimeout.expr;
__auto_type to = _t1205.data.EAwaitTimeout.timeout_ms;
#line 2763 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2764 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
#line 2765 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, to);
    } else if (_t1205.tag == HirExpr_EYield) {
        __auto_type inner = _t1205.data.EYield.expr;
#line 2767 "src/sema.tr"
        Sema_mark_coll_arg(self, inner);
#line 2768 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t1205.tag == HirExpr_EFString) {
        __auto_type parts = _t1205.data.EFString.parts;
#line 2772 "src/sema.tr"
        long long i = 0LL;
#line 2773 "src/sema.tr"
        while ((i < parts->len)) {
#line 2774 "src/sema.tr"
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, i));
#line 2775 "src/sema.tr"
            if (part->is_expr) {
#line 2775 "src/sema.tr"
                Sema_mark_escaped_coll_args(self, part->expr);
            }
#line 2776 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_EClosure) {
        __auto_type captures = _t1205.data.EClosure.captures;
#line 2783 "src/sema.tr"
        long long i = 0LL;
#line 2784 "src/sema.tr"
        while ((i < captures->len)) {
#line 2785 "src/sema.tr"
            HirParam* cap = ((HirParam*)List_ptr_get(captures, i));
#line 2786 "src/sema.tr"
            TrStr ctn = _tr_str_retain(cap->ty->name);
#line 2787 "src/sema.tr"
            if (((((_tr_str_eqv((ctn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("Set", 3LL))))) {
#line 2788 "src/sema.tr"
                Sema_mark_coll_escaped(self, cap->name);
            }
#line 2789 "src/sema.tr"
            i = (i + 1LL);
            _tr_str_release(ctn);
        }
    } else if (_t1205.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t1205.data.ESuperMethodCall.args;
#line 2793 "src/sema.tr"
        long long i = 0LL;
#line 2794 "src/sema.tr"
        while ((i < args->len)) {
#line 2795 "src/sema.tr"
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2796 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
#line 2797 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_ESuperPropAccess) {
        /* pass */
    } else if (_t1205.tag == HirExpr_EListComp) {
        __auto_type element = _t1205.data.EListComp.element;
__auto_type generators = _t1205.data.EListComp.generators;
#line 2804 "src/sema.tr"
        Sema_mark_coll_arg(self, element);
#line 2805 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, element);
#line 2806 "src/sema.tr"
        long long i = 0LL;
#line 2807 "src/sema.tr"
        while ((i < generators->len)) {
#line 2808 "src/sema.tr"
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
#line 2809 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, gen->iter);
#line 2810 "src/sema.tr"
            long long j = 0LL;
#line 2811 "src/sema.tr"
            while ((j < gen->ifs->len)) {
#line 2812 "src/sema.tr"
                Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
#line 2813 "src/sema.tr"
                j = (j + 1LL);
            }
#line 2814 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_EGeneratorExpr) {
        __auto_type element = _t1205.data.EGeneratorExpr.element;
__auto_type generators = _t1205.data.EGeneratorExpr.generators;
#line 2816 "src/sema.tr"
        Sema_mark_coll_arg(self, element);
#line 2817 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, element);
#line 2818 "src/sema.tr"
        long long i = 0LL;
#line 2819 "src/sema.tr"
        while ((i < generators->len)) {
#line 2820 "src/sema.tr"
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
#line 2821 "src/sema.tr"
            Sema_mark_escaped_coll_args(self, gen->iter);
#line 2822 "src/sema.tr"
            long long j = 0LL;
#line 2823 "src/sema.tr"
            while ((j < gen->ifs->len)) {
#line 2824 "src/sema.tr"
                Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
#line 2825 "src/sema.tr"
                j = (j + 1LL);
            }
#line 2826 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1205.tag == HirExpr_ESlice) {
        __auto_type start = _t1205.data.ESlice.start;
__auto_type stop = _t1205.data.ESlice.stop;
__auto_type step = _t1205.data.ESlice.step;
#line 2828 "src/sema.tr"
        Sema_mark_coll_arg(self, start);
#line 2829 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, start);
#line 2830 "src/sema.tr"
        Sema_mark_coll_arg(self, stop);
#line 2831 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, stop);
#line 2832 "src/sema.tr"
        Sema_mark_coll_arg(self, step);
#line 2833 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, step);
    } else if (_t1205.tag == HirExpr_ETry) {
        /* pass */
    } else if (_t1205.tag == HirExpr_EDo) {
        __auto_type _do_b = _t1205.data.EDo.body;
        /* pass */
    } else if (_t1205.tag == HirExpr_EMatchExpr) {
        /* pass */
    } else if (_t1205.tag == HirExpr_ELoop) {
        /* pass */
    } else if (_t1205.tag == HirExpr_EWhileExpr) {
        /* pass */
    } else if (_t1205.tag == HirExpr_ELitInt) {
        /* pass */
    } else if (_t1205.tag == HirExpr_ELitFloat) {
        /* pass */
    } else if (_t1205.tag == HirExpr_ELitStr) {
        /* pass */
    } else if (_t1205.tag == HirExpr_ELitBytes) {
        /* pass */
    } else if (_t1205.tag == HirExpr_ERawStr) {
        /* pass */
    } else if (_t1205.tag == HirExpr_ELitChar) {
        /* pass */
    } else if (_t1205.tag == HirExpr_ELitBool) {
        /* pass */
    } else if (_t1205.tag == HirExpr_ELitNone) {
        __auto_type _ = _t1205.data.ELitNone.ty;
        /* pass */
    } else if (_t1205.tag == HirExpr_ESizeOf) {
        /* pass */
    } else if (_t1205.tag == HirExpr_EIdent) {
        /* pass */
    }
}

__attribute__((hot)) bool Sema_block_ends_in_jump(Sema* self, HirBlock* b) {
#line 2864 "src/sema.tr"
    if ((b->stmts->len == 0LL)) {
#line 2864 "src/sema.tr"
        return false;
    }
#line 2865 "src/sema.tr"
    HirStmt* last = ((HirStmt*)List_ptr_get(b->stmts, (b->stmts->len - 1LL)));
#line 2866 "src/sema.tr"
    if ((((unsigned long long)(last)) == ((unsigned long long)(0LL)))) {
#line 2866 "src/sema.tr"
        return false;
    }
#line 2867 "src/sema.tr"
    __auto_type _t1208 = (*last);
    if (_t1208.tag == HirStmt_SReturn) {
        __auto_type _ = _t1208.data.SReturn.val;
        return true;
    } else if (_t1208.tag == HirStmt_SBreak) {
        __auto_type _ = _t1208.data.SBreak.val;
        return true;
    } else if (_t1208.tag == HirStmt_SContinue) {
        return true;
    } else if (1) {
        __auto_type _ = _t1208;
        return false;
    }
}

__attribute__((hot)) void Sema_append_block_local_drops(Sema* self, HirBlock* hb, long long block_id) {
#line 2879 "src/sema.tr"
    if (Sema_block_ends_in_jump(self, hb)) {
#line 2879 "src/sema.tr"
        return;
    }
#line 2880 "src/sema.tr"
    if ((self->scopes->len == 0LL)) {
#line 2880 "src/sema.tr"
        return;
    }
#line 2881 "src/sema.tr"
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 2882 "src/sema.tr"
    long long ki = 0LL;
#line 2883 "src/sema.tr"
    while ((ki < scope->decl_order->len)) {
#line 2884 "src/sema.tr"
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
#line 2885 "src/sema.tr"
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
#line 2886 "src/sema.tr"
        if (((sym->decl_block_id == block_id) && Sema_is_droppable_sym(self, sym))) {
#line 2887 "src/sema.tr"
            HirBlock_push(hb, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
        }
#line 2888 "src/sema.tr"
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
}

__attribute__((hot)) bool Sema__early_exit_drop_ok(Sema* self, TrStr tn) {
#line 2897 "src/sema.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("str", 3LL)))) {
#line 2897 "src/sema.tr"
        return true;
    }
#line 2898 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
#line 2898 "src/sema.tr"
        return false;
    }
#line 2899 "src/sema.tr"
    if ((((((((((((((((_tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL)))) || (_tr_str_eqv((tn), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((tn), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 2899 "src/sema.tr"
        return false;
    }
#line 2900 "src/sema.tr"
    if (Sema_class_has_free(self, tn)) {
#line 2900 "src/sema.tr"
        return false;
    }
#line 2901 "src/sema.tr"
    return true;
}

__attribute__((hot)) void Sema_append_while_exit_drops(Sema* self, HirBlock* hb) {
#line 2910 "src/sema.tr"
    long long top = (self->loop_blk_floor->len - 1LL);
#line 2911 "src/sema.tr"
    if ((top < 0LL)) {
#line 2911 "src/sema.tr"
        return;
    }
#line 2912 "src/sema.tr"
    long long floor = List_i64_get(self->loop_blk_floor, top);
#line 2913 "src/sema.tr"
    long long si = List_i64_get(self->loop_blk_scope, top);
#line 2914 "src/sema.tr"
    if ((((floor < 0LL) || (si < 0LL)) || (si >= self->scopes->len))) {
#line 2914 "src/sema.tr"
        return;
    }
#line 2915 "src/sema.tr"
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, si));
#line 2916 "src/sema.tr"
    long long ki = 0LL;
#line 2917 "src/sema.tr"
    while ((ki < scope->decl_order->len)) {
#line 2918 "src/sema.tr"
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
#line 2919 "src/sema.tr"
        if (_tr_dict_contains(scope->variables, _tr_strz(nm))) {
#line 2920 "src/sema.tr"
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
#line 2921 "src/sema.tr"
            if (((((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL))) && (sym->decl_block_id != 0LL))) {
#line 2922 "src/sema.tr"
                long long pos = floor;
#line 2923 "src/sema.tr"
                bool inside = false;
#line 2924 "src/sema.tr"
                while ((pos < self->block_stack->len)) {
#line 2925 "src/sema.tr"
                    if (({ __auto_type _ord_t1209 = (List_i64_get(self->block_stack, pos)); __auto_type _wr = ((_ord_t1209 == sym->decl_block_id)); _wr; })) {
#line 2925 "src/sema.tr"
                        inside = true;
                    }
#line 2926 "src/sema.tr"
                    pos = (pos + 1LL);
                }
#line 2927 "src/sema.tr"
                if (((inside && Sema_is_droppable_sym(self, sym)) && Sema__early_exit_drop_ok(self, (*sym->ty)->name))) {
#line 2928 "src/sema.tr"
                    HirBlock_push(hb, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
                }
            }
        }
#line 2929 "src/sema.tr"
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
}

__attribute__((hot)) void Sema_finalize_scope_drops(Sema* self, HirBlock* hb) {
#line 2934 "src/sema.tr"
    if ((!Sema_block_ends_in_jump(self, hb))) {
#line 2935 "src/sema.tr"
        Sema_append_drops_from(self, hb, (self->scopes->len - 1LL));
    }
}

__attribute__((hot)) bool Sema_block_str_decl(Sema* self, HirBlock* hb, TrStr nm) {
#line 2939 "src/sema.tr"
    long long i = 0LL;
#line 2940 "src/sema.tr"
    while ((i < hb->stmts->len)) {
#line 2941 "src/sema.tr"
        __auto_type _t1210 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t1210.tag == HirStmt_SLet) {
            __auto_type sn = _t1210.data.SLet.name;
__auto_type sty = _t1210.data.SLet.ty;
#line 2943 "src/sema.tr"
            if ((_tr_str_eqv((sn), (nm)) && _tr_str_eqv((sty->name), (_tr_str_lit_len("str", 3LL))))) {
#line 2943 "src/sema.tr"
                return true;
            }
        } else if (1) {
            __auto_type _ = _t1210;
            /* pass */
        }
#line 2945 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2946 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_block_has_autodrop(Sema* self, HirBlock* hb, TrStr nm) {
#line 2950 "src/sema.tr"
    long long i = 0LL;
#line 2951 "src/sema.tr"
    while ((i < hb->stmts->len)) {
#line 2952 "src/sema.tr"
        __auto_type _t1211 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t1211.tag == HirStmt_SAutoDrop) {
            __auto_type dn = _t1211.data.SAutoDrop.name;
#line 2954 "src/sema.tr"
            if (_tr_str_eqv((dn), (nm))) {
#line 2954 "src/sema.tr"
                return true;
            }
        } else if (1) {
            __auto_type _ = _t1211;
            /* pass */
        }
#line 2956 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2957 "src/sema.tr"
    return false;
}

__attribute__((hot)) TrStr Sema_block_coll_decl(Sema* self, HirBlock* hb, TrStr nm) {
#line 2962 "src/sema.tr"
    long long i = 0LL;
#line 2963 "src/sema.tr"
    while ((i < hb->stmts->len)) {
#line 2964 "src/sema.tr"
        __auto_type _t1212 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t1212.tag == HirStmt_SLet) {
            __auto_type sn = _t1212.data.SLet.name;
__auto_type sty = _t1212.data.SLet.ty;
#line 2966 "src/sema.tr"
            if (_tr_str_eqv((sn), (nm))) {
#line 2967 "src/sema.tr"
                TrStr tn = _tr_str_retain(sty->name);
#line 2968 "src/sema.tr"
                if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 2969 "src/sema.tr"
                    TrStr _trret212 = _tr_str_retain(tn);
                    _tr_str_release(tn);
                    return _trret212;
                }
            }
        } else if (1) {
            __auto_type _ = _t1212;
            /* pass */
        }
#line 2971 "src/sema.tr"
        i = (i + 1LL);
    }
#line 2972 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool Sema_coll_droppable_by_sema(Sema* self, TrStr nm) {
#line 2979 "src/sema.tr"
    Symbol* sym = Sema_resolve(self, nm);
#line 2980 "src/sema.tr"
    if (_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))) {
#line 2980 "src/sema.tr"
        return false;
    }
#line 2981 "src/sema.tr"
    bool _trret213 = Sema_is_droppable_sym(self, sym);
    return _trret213;
}

__attribute__((hot)) void Sema_apply_mir_if_drops(Sema* self, HirFunction* hf) {
#line 2992 "src/sema.tr"
    List_ptr* plan = mir_if_drop_plan(hf);
#line 2993 "src/sema.tr"
    long long psi = 0LL;
#line 2994 "src/sema.tr"
    while ((psi < plan->len)) {
#line 2995 "src/sema.tr"
        DropSite* site = ((DropSite*)List_ptr_get(plan, psi));
#line 2998 "src/sema.tr"
        if ((!Sema_block_ends_in_jump(self, site->hir_block))) {
#line 2999 "src/sema.tr"
            long long ppi = 0LL;
#line 3000 "src/sema.tr"
            while ((ppi < site->places->len)) {
#line 3001 "src/sema.tr"
                TrStr nm = List_TrStr_get(site->places, ppi);
#line 3002 "src/sema.tr"
                if ((!Sema_block_has_autodrop(self, site->hir_block, nm))) {
#line 3003 "src/sema.tr"
                    if (Sema_block_str_decl(self, site->hir_block, nm)) {
#line 3004 "src/sema.tr"
                        ({ __auto_type _ord_t1213 = (box_hirstmt(HirStmt_ctor_SAutoDrop(nm, _tr_str_lit_len("str", 3LL)))); HirBlock_push(site->hir_block, _ord_t1213); });
                    } else {
#line 3006 "src/sema.tr"
                        TrStr ctn = Sema_block_coll_decl(self, site->hir_block, nm);
#line 3007 "src/sema.tr"
                        if (((!_tr_str_eqv((ctn), (_tr_str_lit_len("", 0LL)))) && Sema_coll_droppable_by_sema(self, nm))) {
#line 3008 "src/sema.tr"
                            ({ __auto_type _ord_t1214 = (box_hirstmt(HirStmt_ctor_SAutoDrop(nm, ctn))); HirBlock_push(site->hir_block, _ord_t1214); });
                        }
                    }
                }
#line 3009 "src/sema.tr"
                ppi = (ppi + 1LL);
                _tr_str_release(nm);
            }
        }
#line 3010 "src/sema.tr"
        psi = (psi + 1LL);
    }
    List_ptr_free_obj(plan, _trdrop_DropSite);
}

__attribute__((hot)) void Sema_declare(Sema* self, TrStr name, SymbolKind kind, AstType** ty, bool is_mut) {
#line 3014 "src/sema.tr"
    if ((((!_tr_str_eqv((name), (_tr_str_lit_len("self", 4LL)))) && (!_tr_str_eqv((name), (_tr_str_lit_len("_", 1LL))))) && (_tr_strlen(_tr_strz(name)) > 1LL))) {
#line 3015 "src/sema.tr"
        TrStr cat = Sema_is_reserved_error(self, name);
#line 3016 "src/sema.tr"
        if ((!_tr_str_eqv((cat), (_tr_str_lit_len("", 0LL))))) {
#line 3017 "src/sema.tr"
            ({ TrStr _at_t1215 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and cannot be used as a name. Choose a different name (e.g. 'my_", 65LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("').", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1215); _tr_str_release(_at_t1215); });
        } else {
#line 3022 "src/sema.tr"
            bool is_toplevel_decl = (_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))) && (kind.tag != SymbolKind_make_SVariable().tag));
#line 3023 "src/sema.tr"
            if (is_toplevel_decl) {
#line 3024 "src/sema.tr"
                TrStr kcat = Sema_is_reserved_keyword(self, name);
#line 3025 "src/sema.tr"
                if ((!_tr_str_eqv((kcat), (_tr_str_lit_len("", 0LL))))) {
#line 3026 "src/sema.tr"
                    ({ TrStr _at_t1216 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kcat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and is reserved. Choose a different name (e.g. 'my_", 52LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("').", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1216); _tr_str_release(_at_t1216); });
                }
            }
        }
    }
#line 3027 "src/sema.tr"
    Symbol* sym = Symbol_init(name, kind, ty);
#line 3028 "src/sema.tr"
    sym->scope_depth = self->current_scope_depth;
#line 3029 "src/sema.tr"
    sym->is_mut = is_mut;
#line 3030 "src/sema.tr"
    sym->decl_block_depth = self->block_depth;
#line 3031 "src/sema.tr"
    long long bsbase = 0LL;
#line 3032 "src/sema.tr"
    if ((self->block_stack_base->len > 0LL)) {
#line 3032 "src/sema.tr"
        bsbase = List_i64_get(self->block_stack_base, (self->block_stack_base->len - 1LL));
    }
#line 3033 "src/sema.tr"
    if ((self->block_stack->len > bsbase)) {
#line 3034 "src/sema.tr"
        sym->decl_block_id = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
    } else {
#line 3036 "src/sema.tr"
        sym->decl_block_id = 0LL;
    }
#line 3038 "src/sema.tr"
    if ((self->scopes->len > 0LL)) {
#line 3039 "src/sema.tr"
        Scope* decl_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 3043 "src/sema.tr"
        if ((((kind.tag == SymbolKind_make_SVariable().tag) && _tr_dict_contains(decl_scope->variables, _tr_strz(name))) && (!_tr_str_starts_withv((name), (_tr_str_lit_len("_", 1LL)))))) {
#line 3044 "src/sema.tr"
            Symbol* prev = ((Symbol*)(uintptr_t)_tr_dict_get(decl_scope->variables, _tr_strz(name)));
#line 3045 "src/sema.tr"
            if ((((prev->kind.tag == SymbolKind_make_SVariable().tag) && (prev->decl_block_id == sym->decl_block_id)) && (!prev->is_param))) {
#line 3046 "src/sema.tr"
                ({ TrStr _at_t1217 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-2] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is already declared in this block. Assign to it without `mut` (`", 66LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ...`), or pick a different name.", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1217); _tr_str_release(_at_t1217); });
            }
        }
#line 3047 "src/sema.tr"
        if ((!_tr_dict_contains(decl_scope->variables, _tr_strz(name)))) {
#line 3047 "src/sema.tr"
            List_TrStr_append(decl_scope->decl_order, name);
        } else if ((((kind.tag == SymbolKind_make_SVariable().tag) && (!_tr_str_starts_withv((name), (_tr_str_lit_len("_", 1LL))))) && (sym->decl_block_id != 0LL))) {
#line 3050 "src/sema.tr"
            Symbol* shadowed = ((Symbol*)(uintptr_t)_tr_dict_get(decl_scope->variables, _tr_strz(name)));
#line 3051 "src/sema.tr"
            if (((shadowed->kind.tag == SymbolKind_make_SVariable().tag) && (shadowed->decl_block_id != sym->decl_block_id))) {
#line 3052 "src/sema.tr"
                List_TrStr_append(self->shadow_names, name);
#line 3053 "src/sema.tr"
                List_ptr_append(self->shadow_prevs, shadowed);
#line 3054 "src/sema.tr"
                List_i64_append(self->shadow_blocks, sym->decl_block_id);
#line 3055 "src/sema.tr"
                List_i64_append(self->shadow_scopes, (self->scopes->len - 1LL));
            }
        }
#line 3056 "src/sema.tr"
        _tr_dict_set(decl_scope->variables, _tr_strz(name), sym);
    } else {
#line 3058 "src/sema.tr"
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) Symbol* Sema_resolve(Sema* self, TrStr name) {
#line 3061 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 3062 "src/sema.tr"
    while ((i >= 0LL)) {
#line 3063 "src/sema.tr"
        Scope* scope = ((Scope*)List_ptr_get(self->scopes, i));
#line 3064 "src/sema.tr"
        if (_tr_dict_contains(scope->variables, _tr_strz(name))) {
#line 3065 "src/sema.tr"
            return ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(name)));
        }
#line 3066 "src/sema.tr"
        i = (i - 1LL);
    }
#line 3068 "src/sema.tr"
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
#line 3069 "src/sema.tr"
        return ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
    }
#line 3071 "src/sema.tr"
    return ({ __auto_type _ord_t1218 = (SymbolKind_make_SVariable()); __auto_type _ord_t1219 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); __auto_type _wr = (Symbol_init(_tr_str_lit_len("", 0LL), _ord_t1218, _ord_t1219)); _wr; });
}

__attribute__((hot)) bool Sema_is_known_name(Sema* self, TrStr name) {
#line 3086 "src/sema.tr"
    if (Sema__is_type_param_in_scope(self, name)) {
#line 3086 "src/sema.tr"
        return true;
    }
#line 3087 "src/sema.tr"
    if (_tr_dict_contains(self->fn_sigs, _tr_strz(name))) {
#line 3087 "src/sema.tr"
        return true;
    }
#line 3088 "src/sema.tr"
    if (_tr_dict_contains(self->extern_names, _tr_strz(name))) {
#line 3088 "src/sema.tr"
        return true;
    }
#line 3089 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
#line 3089 "src/sema.tr"
        return true;
    }
#line 3090 "src/sema.tr"
    if (_tr_dict_contains(self->enums, _tr_strz(name))) {
#line 3090 "src/sema.tr"
        return true;
    }
#line 3091 "src/sema.tr"
    if (_tr_dict_contains(self->interfaces, _tr_strz(name))) {
#line 3091 "src/sema.tr"
        return true;
    }
#line 3092 "src/sema.tr"
    if (_tr_dict_contains(self->type_aliases, _tr_strz(name))) {
#line 3092 "src/sema.tr"
        return true;
    }
#line 3093 "src/sema.tr"
    if (Sema_is_type_name(self, name)) {
#line 3093 "src/sema.tr"
        return true;
    }
#line 3095 "src/sema.tr"
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("dealloc", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("realloc", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("sizeof", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("repr", 4LL))))) {
#line 3095 "src/sema.tr"
        return true;
    }
#line 3096 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("c_callback", 10LL)))) {
#line 3096 "src/sema.tr"
        return true;
    }
#line 3098 "src/sema.tr"
    if (((_tr_str_lenv((name)) >= 4LL) && ({ TrStr _cmpa_t1220 = (_tr_str_slicev((name), 0LL, 4LL)); TrStr _cmpb_t1221 = (_tr_str_lit_len("_tr_", 4LL)); _Bool _cmpr_t1222 = _tr_str_eqv(_cmpa_t1220, _cmpb_t1221); _tr_str_release(_cmpa_t1220); _cmpr_t1222; }))) {
#line 3098 "src/sema.tr"
        return true;
    }
#line 3099 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_type_name(Sema* self, TrStr nm) {
#line 3103 "src/sema.tr"
    if (((_tr_dict_contains(self->classes, _tr_strz(nm)) || _tr_dict_contains(self->enums, _tr_strz(nm))) || _tr_dict_contains(self->interfaces, _tr_strz(nm)))) {
#line 3103 "src/sema.tr"
        return true;
    }
#line 3104 "src/sema.tr"
    if ((((((_tr_str_eqv((nm), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("void", 4LL))))) {
#line 3104 "src/sema.tr"
        return true;
    }
#line 3105 "src/sema.tr"
    if (((((_tr_str_eqv((nm), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("isize", 5LL))))) {
#line 3105 "src/sema.tr"
        return true;
    }
#line 3106 "src/sema.tr"
    if (((((_tr_str_eqv((nm), (_tr_str_lit_len("u8", 2LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("usize", 5LL))))) {
#line 3106 "src/sema.tr"
        return true;
    }
#line 3107 "src/sema.tr"
    if ((_tr_str_eqv((nm), (_tr_str_lit_len("f32", 3LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("f64", 3LL))))) {
#line 3107 "src/sema.tr"
        return true;
    }
#line 3108 "src/sema.tr"
    return false;
}

__attribute__((hot)) TrStr Sema_type_ref_name(Sema* self, Expr* raw) {
#line 3113 "src/sema.tr"
    if ((((unsigned long long)(raw)) == ((unsigned long long)(0LL)))) {
#line 3113 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 3114 "src/sema.tr"
    __auto_type _t1223 = (*raw);
    if (_t1223.tag == Expr_EIdent) {
        __auto_type nm = _t1223.data.EIdent.name;
#line 3116 "src/sema.tr"
        if ((!Sema_is_type_name(self, nm))) {
#line 3116 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 3121 "src/sema.tr"
        Symbol* s = Sema_resolve(self, nm);
#line 3122 "src/sema.tr"
        if (((!_tr_str_eqv((s->name), (_tr_str_lit_len("", 0LL)))) && (s->kind.tag == SymbolKind_make_SVariable().tag))) {
#line 3122 "src/sema.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 3123 "src/sema.tr"
        TrStr _trret214 = _tr_str_retain(nm);
        return _trret214;
    } else if (1) {
        __auto_type _ = _t1223;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool Sema_is_global_not_local(Sema* self, TrStr name) {
#line 3131 "src/sema.tr"
    long long i = (self->scopes->len - 1LL);
#line 3132 "src/sema.tr"
    while ((i >= 0LL)) {
#line 3133 "src/sema.tr"
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
#line 3133 "src/sema.tr"
            return false;
        }
#line 3134 "src/sema.tr"
        i = (i - 1LL);
    }
#line 3135 "src/sema.tr"
    return _tr_dict_contains(self->globals, _tr_strz(name));
}

__attribute__((hot)) HirProgram* Sema_analyze(Sema* self, Program* prog) {
#line 3138 "src/sema.tr"
    HirProgram* hp = HirProgram_init();
#line 3147 "src/sema.tr"
    long long ppi = 0LL;
#line 3148 "src/sema.tr"
    while ((ppi < prog->decls->len)) {
#line 3149 "src/sema.tr"
        __auto_type _t1224 = (*((Decl*)List_ptr_get(prog->decls, ppi)));
        if (_t1224.tag == Decl_DClass) {
            __auto_type c = _t1224.data.DClass.cls;
#line 3151 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(c->name))) {
#line 3151 "src/sema.tr"
                _tr_dict_set(self->ambiguous_classes, _tr_strz(c->name), true);
            } else {
#line 3152 "src/sema.tr"
                _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
            }
        } else if (_t1224.tag == Decl_DActor) {
            __auto_type c = _t1224.data.DActor.cls;
#line 3154 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(c->name))) {
#line 3154 "src/sema.tr"
                _tr_dict_set(self->ambiguous_classes, _tr_strz(c->name), true);
            } else {
#line 3155 "src/sema.tr"
                _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
            }
        } else if (1) {
            __auto_type _ = _t1224;
            /* pass */
        }
#line 3157 "src/sema.tr"
        ppi = (ppi + 1LL);
    }
#line 3172 "src/sema.tr"
    long long gpi = 0LL;
#line 3173 "src/sema.tr"
    while ((gpi < prog->decls->len)) {
#line 3174 "src/sema.tr"
        __auto_type _t1225 = (*((Decl*)List_ptr_get(prog->decls, gpi)));
        if (_t1225.tag == Decl_DTopLevelStmt) {
            __auto_type gs = _t1225.data.DTopLevelStmt.stmt;
#line 3176 "src/sema.tr"
            __auto_type _t1226 = (*gs);
            if (_t1226.tag == Stmt_SLet) {
                __auto_type g_name = _t1226.data.SLet.name;
__auto_type g_is_mut = _t1226.data.SLet.is_mut;
__auto_type g_ty_ptr = _t1226.data.SLet.ty;
__auto_type g_val_ptr = _t1226.data.SLet.val;
#line 3178 "src/sema.tr"
                AstType** g_ty = ((AstType**)(0LL));
#line 3179 "src/sema.tr"
                if ((((unsigned long long)(g_ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 3180 "src/sema.tr"
                    g_ty = g_ty_ptr;
                } else if ((((unsigned long long)(g_val_ptr)) != ((unsigned long long)(0LL)))) {
#line 3182 "src/sema.tr"
                    __auto_type _t1227 = (*g_val_ptr);
                    if (_t1227.tag == Expr_ELitInt) {
                        __auto_type _ = _t1227.data.ELitInt.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)));
                    } else if (_t1227.tag == Expr_ELitFloat) {
                        __auto_type _ = _t1227.data.ELitFloat.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("float", 5LL)));
                    } else if (_t1227.tag == Expr_ELitStr) {
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("str", 3LL)));
                    } else if (_t1227.tag == Expr_ELitBool) {
                        __auto_type _ = _t1227.data.ELitBool.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit_len("bool", 4LL)));
                    } else if (1) {
                        __auto_type _ = _t1227;
                        /* pass */
                    }
                }
#line 3188 "src/sema.tr"
                if ((((unsigned long long)(g_ty)) != ((unsigned long long)(0LL)))) {
#line 3189 "src/sema.tr"
                    Sema_declare(self, g_name, SymbolKind_make_SVariable(), g_ty, g_is_mut);
                }
            } else if (1) {
                __auto_type _ = _t1226;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t1225;
            /* pass */
        }
#line 3192 "src/sema.tr"
        gpi = (gpi + 1LL);
    }
#line 3194 "src/sema.tr"
    long long i = 0LL;
#line 3195 "src/sema.tr"
    while ((i < prog->decls->len)) {
#line 3196 "src/sema.tr"
        Sema_register_decl(self, ((Decl*)List_ptr_get(prog->decls, i)));
#line 3197 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3202 "src/sema.tr"
    Sema_check_type_arities(self, prog);
#line 3209 "src/sema.tr"
    if (self->strict_mode) {
#line 3210 "src/sema.tr"
        Sema_check_ownership_cycles(self, prog);
    }
#line 3215 "src/sema.tr"
    if (self->strict_mode) {
#line 3216 "src/sema.tr"
        i = 0LL;
#line 3217 "src/sema.tr"
        while ((i < prog->decls->len)) {
#line 3218 "src/sema.tr"
            __auto_type _t1228 = (*((Decl*)List_ptr_get(prog->decls, i)));
            if (_t1228.tag == Decl_DClass) {
                __auto_type mc = _t1228.data.DClass.cls;
#line 3220 "src/sema.tr"
                long long mmi = 0LL;
#line 3221 "src/sema.tr"
                while ((mmi < mc->methods->len)) {
#line 3222 "src/sema.tr"
                    FunctionDef* mm = ((FunctionDef*)List_ptr_get(mc->methods, mmi));
#line 3223 "src/sema.tr"
                    if (_block_mutates_self(mm->body)) {
#line 3223 "src/sema.tr"
                        ({ TrStr _dkt_t1229 = (({ TrStr _tr_cl = (_tr_strx_concatv((mc->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mm->name)); _tr_str_release(_tr_cl); _tr_cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t1229), true); _tr_str_release(_dkt_t1229); });
                    }
#line 3224 "src/sema.tr"
                    mmi = (mmi + 1LL);
                }
            } else if (_t1228.tag == Decl_DActor) {
                __auto_type ac = _t1228.data.DActor.cls;
#line 3226 "src/sema.tr"
                long long ami = 0LL;
#line 3227 "src/sema.tr"
                while ((ami < ac->methods->len)) {
#line 3228 "src/sema.tr"
                    FunctionDef* am = ((FunctionDef*)List_ptr_get(ac->methods, ami));
#line 3229 "src/sema.tr"
                    if (_block_mutates_self(am->body)) {
#line 3229 "src/sema.tr"
                        ({ TrStr _dkt_t1230 = (({ TrStr _tr_cl = (_tr_strx_concatv((ac->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (am->name)); _tr_str_release(_tr_cl); _tr_cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t1230), true); _tr_str_release(_dkt_t1230); });
                    }
#line 3230 "src/sema.tr"
                    ami = (ami + 1LL);
                }
            } else if (_t1228.tag == Decl_DExtend) {
                __auto_type mtarget = _t1228.data.DExtend.target;
__auto_type mmethods = _t1228.data.DExtend.methods;
#line 3232 "src/sema.tr"
                long long emi = 0LL;
#line 3233 "src/sema.tr"
                while ((emi < mmethods->len)) {
#line 3234 "src/sema.tr"
                    FunctionDef* em = ((FunctionDef*)List_ptr_get(mmethods, emi));
#line 3235 "src/sema.tr"
                    if (_block_mutates_self(em->body)) {
#line 3235 "src/sema.tr"
                        ({ TrStr _dkt_t1231 = (({ TrStr _tr_cl = (_tr_strx_concatv((mtarget), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (em->name)); _tr_str_release(_tr_cl); _tr_cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t1231), true); _tr_str_release(_dkt_t1231); });
                    }
#line 3236 "src/sema.tr"
                    emi = (emi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1228;
                /* pass */
            }
#line 3238 "src/sema.tr"
            i = (i + 1LL);
        }
    }
#line 3245 "src/sema.tr"
    TrIDict* pre_global_hir = _tr_idict_new(4LL);
#line 3246 "src/sema.tr"
    long long pgi = 0LL;
#line 3247 "src/sema.tr"
    while ((pgi < prog->decls->len)) {
#line 3248 "src/sema.tr"
        __auto_type _t1232 = (*((Decl*)List_ptr_get(prog->decls, pgi)));
        if (_t1232.tag == Decl_DTopLevelStmt) {
            __auto_type pgs = _t1232.data.DTopLevelStmt.stmt;
#line 3250 "src/sema.tr"
            __auto_type _t1233 = (*pgs);
            if (_t1233.tag == Stmt_SLet) {
                __auto_type pg_name = _t1233.data.SLet.name;
__auto_type pg_val = _t1233.data.SLet.val;
#line 3252 "src/sema.tr"
                if (((((unsigned long long)(pg_val)) != ((unsigned long long)(0LL))) && (!_tr_dict_contains(self->globals, _tr_strz(pg_name))))) {
#line 3253 "src/sema.tr"
                    _tr_idict_set(pre_global_hir, pgi, Sema_lower_stmt(self, pgs));
                }
            } else if (1) {
                __auto_type _ = _t1233;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t1232;
            /* pass */
        }
#line 3256 "src/sema.tr"
        pgi = (pgi + 1LL);
    }
#line 3258 "src/sema.tr"
    i = 0LL;
#line 3259 "src/sema.tr"
    while ((i < prog->decls->len)) {
#line 3260 "src/sema.tr"
        Decl* d = ((Decl*)List_ptr_get(prog->decls, i));
#line 3261 "src/sema.tr"
        __auto_type _t1234 = (*d);
        if (_t1234.tag == Decl_DFunction) {
            __auto_type f = _t1234.data.DFunction.func;
#line 3263 "src/sema.tr"
            ({ HirFunction* _ord_t1235 = (Sema_lower_func(self, f)); List_ptr_append(hp->functions, _tr_obj_retain(_ord_t1235)); _tr_obj_release(_ord_t1235, _trdrop_HirFunction); });
        } else if (_t1234.tag == Decl_DClass) {
            __auto_type c = _t1234.data.DClass.cls;
#line 3265 "src/sema.tr"
            ({ HirClass* _ord_t1236 = (Sema_lower_class(self, c)); List_ptr_append(hp->classes, _tr_obj_retain(_ord_t1236)); _tr_obj_release(_ord_t1236, _trdrop_HirClass); });
        } else if (_t1234.tag == Decl_DActor) {
            __auto_type c = _t1234.data.DActor.cls;
#line 3267 "src/sema.tr"
            ({ HirClass* _ord_t1237 = (Sema_lower_class(self, c)); List_ptr_append(hp->classes, _tr_obj_retain(_ord_t1237)); _tr_obj_release(_ord_t1237, _trdrop_HirClass); });
        } else if (_t1234.tag == Decl_DEnum) {
            __auto_type e = _t1234.data.DEnum.enm;
#line 3269 "src/sema.tr"
            ({ HirEnum* _ord_t1238 = (Sema_lower_enum(self, e)); List_ptr_append(hp->enums, _tr_obj_retain(_ord_t1238)); _tr_obj_release(_ord_t1238, _trdrop_HirEnum); });
        } else if (_t1234.tag == Decl_DInterface) {
            __auto_type i_def = _t1234.data.DInterface.iface;
#line 3271 "src/sema.tr"
            ({ HirInterface* _ord_t1239 = (Sema_lower_interface(self, i_def)); List_ptr_append(hp->interfaces, _tr_obj_retain(_ord_t1239)); _tr_obj_release(_ord_t1239, _trdrop_HirInterface); });
        } else if (_t1234.tag == Decl_DExtend) {
            __auto_type target = _t1234.data.DExtend.target;
__auto_type methods = _t1234.data.DExtend.methods;
#line 3273 "src/sema.tr"
            self->current_class_name = _tr_str_retain(target);
#line 3274 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(target))) {
#line 3275 "src/sema.tr"
                self->current_region_params = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(target)))->region_params;
            }
#line 3276 "src/sema.tr"
            long long hi = 0LL;
#line 3277 "src/sema.tr"
            while ((hi < methods->len)) {
#line 3278 "src/sema.tr"
                FunctionDef* f = ((FunctionDef*)List_ptr_get(methods, hi));
#line 3279 "src/sema.tr"
                ({ HirFunction* _ord_t1240 = (Sema_lower_func(self, f)); List_ptr_append(hp->functions, _tr_obj_retain(_ord_t1240)); _tr_obj_release(_ord_t1240, _trdrop_HirFunction); });
#line 3280 "src/sema.tr"
                hi = (hi + 1LL);
            }
#line 3281 "src/sema.tr"
            self->current_class_name = _tr_str_lit_len("", 0LL);
#line 3282 "src/sema.tr"
            self->current_region_params = (void*)List_TrStr_new();
        } else if (_t1234.tag == Decl_DTopLevelStmt) {
            __auto_type s = _t1234.data.DTopLevelStmt.stmt;
#line 3284 "src/sema.tr"
            if (_tr_idict_contains(pre_global_hir, i)) {
#line 3285 "src/sema.tr"
                ({ __auto_type _ord_t1241 = (((HirStmt*)(uintptr_t)_tr_idict_get(pre_global_hir, i))); List_ptr_append(hp->top_level_stmts, _ord_t1241); });
            } else {
#line 3287 "src/sema.tr"
                ({ __auto_type _ord_t1242 = (Sema_lower_stmt(self, s)); List_ptr_append(hp->top_level_stmts, _ord_t1242); });
            }
        } else if (_t1234.tag == Decl_DExtern) {
            __auto_type functions = _t1234.data.DExtern.functions;
#line 3289 "src/sema.tr"
            long long ei = 0LL;
#line 3290 "src/sema.tr"
            while ((ei < functions->len)) {
#line 3291 "src/sema.tr"
                FunctionDef* ef = ((FunctionDef*)List_ptr_get(functions, ei));
#line 3292 "src/sema.tr"
                HirFunction* hef = Sema_lower_func(self, ef);
#line 3293 "src/sema.tr"
                hef->is_extern = true;
#line 3294 "src/sema.tr"
                List_ptr_append(hp->extern_funcs, _tr_obj_retain(hef));
#line 3295 "src/sema.tr"
                ei = (ei + 1LL);
                _tr_obj_release(hef, _trdrop_HirFunction);
            }
        } else if (_t1234.tag == Decl_DDecoratorDef) {
            __auto_type f = _t1234.data.DDecoratorDef.func;
#line 3297 "src/sema.tr"
            HirFunction* hdf = Sema_lower_func(self, f);
#line 3298 "src/sema.tr"
            hdf->is_decorator = true;
#line 3299 "src/sema.tr"
            List_ptr_append(hp->decorator_defs, _tr_obj_retain(hdf));
            _tr_obj_release(hdf, _trdrop_HirFunction);
        } else if (_t1234.tag == Decl_DTypeAlias) {
            __auto_type alias_name = _t1234.data.DTypeAlias.name;
__auto_type target_ty = _t1234.data.DTypeAlias.target;
#line 3302 "src/sema.tr"
            List_TrStr_append(hp->type_alias_names, alias_name);
#line 3303 "src/sema.tr"
            AstType** ta_ty_ptr = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 3304 "src/sema.tr"
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
#line 3304 "src/sema.tr"
                ta_ty_ptr = target_ty;
            }
#line 3305 "src/sema.tr"
            List_ptr_append(hp->type_alias_types, ta_ty_ptr);
        } else if (1) {
            __auto_type _ = _t1234;
            /* pass */
        }
#line 3307 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3311 "src/sema.tr"
    long long nci = 0LL;
#line 3312 "src/sema.tr"
    while ((nci < self->nested_classes->len)) {
#line 3313 "src/sema.tr"
        ({ __auto_type _ord_t1243 = (((HirClass*)List_ptr_get(self->nested_classes, nci))); List_ptr_append(hp->classes, _tr_obj_retain(_ord_t1243)); });
#line 3314 "src/sema.tr"
        nci = (nci + 1LL);
    }
#line 3315 "src/sema.tr"
    long long nfi = 0LL;
#line 3316 "src/sema.tr"
    while ((nfi < self->nested_functions->len)) {
#line 3317 "src/sema.tr"
        ({ __auto_type _ord_t1244 = (((HirFunction*)List_ptr_get(self->nested_functions, nfi))); List_ptr_append(hp->functions, _tr_obj_retain(_ord_t1244)); });
#line 3318 "src/sema.tr"
        nfi = (nfi + 1LL);
    }
#line 3319 "src/sema.tr"
    long long nei = 0LL;
#line 3320 "src/sema.tr"
    while ((nei < self->nested_enums->len)) {
#line 3321 "src/sema.tr"
        ({ __auto_type _ord_t1245 = (((HirEnum*)List_ptr_get(self->nested_enums, nei))); List_ptr_append(hp->enums, _tr_obj_retain(_ord_t1245)); });
#line 3322 "src/sema.tr"
        nei = (nei + 1LL);
    }
#line 3323 "src/sema.tr"
    long long nii = 0LL;
#line 3324 "src/sema.tr"
    while ((nii < self->nested_interfaces->len)) {
#line 3325 "src/sema.tr"
        ({ __auto_type _ord_t1246 = (((HirInterface*)List_ptr_get(self->nested_interfaces, nii))); List_ptr_append(hp->interfaces, _tr_obj_retain(_ord_t1246)); });
#line 3326 "src/sema.tr"
        nii = (nii + 1LL);
    }
#line 3333 "src/sema.tr"
    Sema_compute_return_ownership(self, hp);
#line 3336 "src/sema.tr"
    Sema_compute_param_ownership(self, hp);
#line 3339 "src/sema.tr"
    Sema_apply_borrow_drops(self, hp);
#line 3340 "src/sema.tr"
    Sema_check_free_fn_method_c_name_collisions(self, hp);
#line 3341 "src/sema.tr"
    return hp;
}

__attribute__((hot)) void Sema_check_free_fn_method_c_name_collisions(Sema* self, HirProgram* hp) {
#line 3362 "src/sema.tr"
    TrMap* method_c_names = _tr_dict_new(32LL);
#line 3363 "src/sema.tr"
    long long mi = 0LL;
#line 3364 "src/sema.tr"
    while ((mi < hp->functions->len)) {
#line 3365 "src/sema.tr"
        HirFunction* mf = ((HirFunction*)List_ptr_get(hp->functions, mi));
#line 3366 "src/sema.tr"
        if (((!_tr_str_eqv((mf->class_name), (_tr_str_lit_len("", 0LL)))) && (!mf->is_extern))) {
#line 3367 "src/sema.tr"
            TrStr mc_name = ({ TrStr _tr_cl = (_tr_strx_concatv((mf->class_name), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mf->name)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3368 "src/sema.tr"
            if ((!_tr_dict_contains(method_c_names, _tr_strz(mc_name)))) {
#line 3369 "src/sema.tr"
                ({ TrStr _dvt_t1247 = (({ TrStr _tr_cl = (_tr_strx_concatv((mf->class_name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mf->name)); _tr_str_release(_tr_cl); _tr_cres; })); _tr_dict_set(method_c_names, _tr_strz(mc_name), _tr_str_box(_tr_str_retain(_dvt_t1247))); _tr_str_release(_dvt_t1247); });
            }
        }
#line 3370 "src/sema.tr"
        mi = (mi + 1LL);
    }
#line 3371 "src/sema.tr"
    long long fi = 0LL;
#line 3372 "src/sema.tr"
    while ((fi < hp->functions->len)) {
#line 3373 "src/sema.tr"
        HirFunction* ff = ((HirFunction*)List_ptr_get(hp->functions, fi));
#line 3374 "src/sema.tr"
        if ((((_tr_str_eqv((ff->class_name), (_tr_str_lit_len("", 0LL))) && (!ff->is_extern)) && (!_tr_str_eqv((ff->name), (_tr_str_lit_len("main", 4LL))))) && (ff->generics->len == 0LL))) {
#line 3375 "src/sema.tr"
            TrStr fc_name = _tr_str_retain(ff->name);
#line 3376 "src/sema.tr"
            if (_tr_dict_contains(method_c_names, _tr_strz(fc_name))) {
#line 3377 "src/sema.tr"
                ({ __auto_type _ord_t1248 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[C-1] free function '", 21LL)), (ff->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' generates the same C symbol name ('", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fc_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("') as class/enum method '", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1249 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1248); TrStr _tr_cr = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(method_c_names, _tr_strz(fc_name))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      This compiles cleanly on its own but fails at LINK time ('multiple definition of \"", 91LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fc_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"') the moment a build pulls in both definitions.\n      FIX: rename one of the two (the free function '", 103LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1250 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1249); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ff->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or the method '", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1251 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1250); TrStr _tr_cr = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(method_c_names, _tr_strz(fc_name))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("').", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1251); _tr_str_release(_at_t1251); });
            }
        }
#line 3378 "src/sema.tr"
        fi = (fi + 1LL);
    }
}

__attribute__((hot)) void Sema_compute_return_ownership(Sema* self, HirProgram* hp) {
#line 3392 "src/sema.tr"
    List_TrStr* keys = (void*)List_TrStr_new();
#line 3393 "src/sema.tr"
    List_ptr* fns = (void*)List_ptr_new();
#line 3394 "src/sema.tr"
    long long fi = 0LL;
#line 3395 "src/sema.tr"
    while ((fi < hp->functions->len)) {
#line 3396 "src/sema.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(hp->functions, fi));
#line 3397 "src/sema.tr"
        if (Sema__fn_ret_is_heap_class(self, f)) {
#line 3398 "src/sema.tr"
            List_TrStr_append(keys, f->name);
#line 3399 "src/sema.tr"
            List_ptr_append(fns, _tr_obj_retain(f));
        }
#line 3400 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 3401 "src/sema.tr"
    long long ci = 0LL;
#line 3402 "src/sema.tr"
    while ((ci < hp->classes->len)) {
#line 3403 "src/sema.tr"
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
#line 3404 "src/sema.tr"
        long long mi = 0LL;
#line 3405 "src/sema.tr"
        while ((mi < c->methods->len)) {
#line 3406 "src/sema.tr"
            HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
#line 3407 "src/sema.tr"
            if (Sema__fn_ret_is_heap_class(self, m)) {
#line 3408 "src/sema.tr"
                ({ TrStr _at_t1252 = (({ TrStr _tr_cl = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(keys, _at_t1252); _tr_str_release(_at_t1252); });
#line 3409 "src/sema.tr"
                List_ptr_append(fns, _tr_obj_retain(m));
            }
#line 3410 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 3411 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 3412 "src/sema.tr"
    long long ei = 0LL;
#line 3413 "src/sema.tr"
    while ((ei < hp->enums->len)) {
#line 3414 "src/sema.tr"
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, ei));
#line 3415 "src/sema.tr"
        long long emi = 0LL;
#line 3416 "src/sema.tr"
        while ((emi < en->methods->len)) {
#line 3417 "src/sema.tr"
            HirFunction* em = ((HirFunction*)List_ptr_get(en->methods, emi));
#line 3418 "src/sema.tr"
            if (Sema__fn_ret_is_heap_class(self, em)) {
#line 3419 "src/sema.tr"
                ({ TrStr _at_t1253 = (({ TrStr _tr_cl = (_tr_strx_concatv((en->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (em->name)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(keys, _at_t1253); _tr_str_release(_at_t1253); });
#line 3420 "src/sema.tr"
                List_ptr_append(fns, _tr_obj_retain(em));
            }
#line 3421 "src/sema.tr"
            emi = (emi + 1LL);
        }
#line 3422 "src/sema.tr"
        ei = (ei + 1LL);
    }
#line 3428 "src/sema.tr"
    long long ki = 0LL;
#line 3429 "src/sema.tr"
    while ((ki < keys->len)) {
#line 3430 "src/sema.tr"
        HirFunction* kf = ((HirFunction*)List_ptr_get(fns, ki));
#line 3431 "src/sema.tr"
        bool kf_owned = true;
#line 3432 "src/sema.tr"
        if ((((unsigned long long)(kf->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 3433 "src/sema.tr"
            if (((kf->ret_ty->is_borrow || (!_tr_str_eqv((kf->ret_ty->from_param), (_tr_str_lit_len("", 0LL))))) || (kf->ret_ty->from_regions->len > 0LL))) {
#line 3434 "src/sema.tr"
                kf_owned = false;
            }
        }
#line 3435 "src/sema.tr"
        ({ TrStr _ord_t1254 = (List_TrStr_get(keys, ki)); _tr_dict_set(self->fn_ret_owned, _tr_strz(_ord_t1254), kf_owned); _tr_str_release(_ord_t1254); });
#line 3436 "src/sema.tr"
        ki = (ki + 1LL);
    }
#line 3438 "src/sema.tr"
    bool changed = true;
#line 3439 "src/sema.tr"
    while (changed) {
#line 3440 "src/sema.tr"
        changed = false;
#line 3441 "src/sema.tr"
        long long xi = 0LL;
#line 3442 "src/sema.tr"
        while ((xi < fns->len)) {
#line 3443 "src/sema.tr"
            TrStr k = List_TrStr_get(keys, xi);
#line 3444 "src/sema.tr"
            if (((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(k)))) {
#line 3445 "src/sema.tr"
                List_ptr* rets = (void*)List_ptr_new();
#line 3446 "src/sema.tr"
                Sema__collect_returns(self, ((HirFunction*)List_ptr_get(fns, xi))->body, rets);
#line 3447 "src/sema.tr"
                bool all_owned = true;
#line 3448 "src/sema.tr"
                long long ri = 0LL;
#line 3449 "src/sema.tr"
                while ((ri < rets->len)) {
#line 3450 "src/sema.tr"
                    if ((!Sema__ret_yields_owned(self, ((HirExpr*)List_ptr_get(rets, ri))))) {
#line 3451 "src/sema.tr"
                        all_owned = false;
                    }
#line 3452 "src/sema.tr"
                    ri = (ri + 1LL);
                }
#line 3453 "src/sema.tr"
                if ((!all_owned)) {
#line 3454 "src/sema.tr"
                    _tr_dict_set(self->fn_ret_owned, _tr_strz(k), false);
#line 3455 "src/sema.tr"
                    changed = true;
                }
            }
#line 3456 "src/sema.tr"
            xi = (xi + 1LL);
            _tr_str_release(k);
        }
    }
#line 3458 "src/sema.tr"
    long long wi = 0LL;
#line 3459 "src/sema.tr"
    while ((wi < fns->len)) {
#line 3460 "src/sema.tr"
        HirFunction* hf2 = ((HirFunction*)List_ptr_get(fns, wi));
#line 3461 "src/sema.tr"
        hf2->returns_owned = ({ TrStr _ord_t1255 = (List_TrStr_get(keys, wi)); __auto_type _wr = (((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(_ord_t1255)))); _tr_str_release(_ord_t1255); _wr; });
#line 3462 "src/sema.tr"
        wi = (wi + 1LL);
    }
    List_TrStr_free(keys);
    List_ptr_free_obj(fns, _trdrop_HirFunction);
}

__attribute__((hot)) void Sema_compute_param_ownership(Sema* self, HirProgram* hp) {
#line 3472 "src/sema.tr"
    List_TrStr* keys = (void*)List_TrStr_new();
#line 3473 "src/sema.tr"
    List_ptr* fns = (void*)List_ptr_new();
#line 3474 "src/sema.tr"
    long long fi = 0LL;
#line 3475 "src/sema.tr"
    while ((fi < hp->functions->len)) {
#line 3476 "src/sema.tr"
        List_TrStr_append(keys, ((HirFunction*)List_ptr_get(hp->functions, fi))->name);
#line 3477 "src/sema.tr"
        List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(hp->functions, fi))));
#line 3478 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 3479 "src/sema.tr"
    long long ci = 0LL;
#line 3480 "src/sema.tr"
    while ((ci < hp->classes->len)) {
#line 3481 "src/sema.tr"
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
#line 3482 "src/sema.tr"
        long long mi = 0LL;
#line 3483 "src/sema.tr"
        while ((mi < c->methods->len)) {
#line 3484 "src/sema.tr"
            ({ __auto_type _ord_t1256 = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _at_t1257 = (({ TrStr _tr_cl = (_ord_t1256); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((HirFunction*)List_ptr_get(c->methods, mi))->name)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(keys, _at_t1257); _tr_str_release(_at_t1257); });
#line 3485 "src/sema.tr"
            List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(c->methods, mi))));
#line 3486 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 3487 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 3488 "src/sema.tr"
    long long eni = 0LL;
#line 3489 "src/sema.tr"
    while ((eni < hp->enums->len)) {
#line 3490 "src/sema.tr"
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, eni));
#line 3491 "src/sema.tr"
        long long emi = 0LL;
#line 3492 "src/sema.tr"
        while ((emi < en->methods->len)) {
#line 3493 "src/sema.tr"
            ({ __auto_type _ord_t1258 = (_tr_strx_concatv((en->name), (_tr_str_lit_len(".", 1LL)))); TrStr _at_t1259 = (({ TrStr _tr_cl = (_ord_t1258); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((HirFunction*)List_ptr_get(en->methods, emi))->name)); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(keys, _at_t1259); _tr_str_release(_at_t1259); });
#line 3494 "src/sema.tr"
            List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(en->methods, emi))));
#line 3495 "src/sema.tr"
            emi = (emi + 1LL);
        }
#line 3496 "src/sema.tr"
        eni = (eni + 1LL);
    }
#line 3498 "src/sema.tr"
    long long ii = 0LL;
#line 3499 "src/sema.tr"
    while ((ii < fns->len)) {
#line 3500 "src/sema.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(fns, ii));
#line 3501 "src/sema.tr"
        long long pi = 0LL;
#line 3502 "src/sema.tr"
        while ((pi < f->params->len)) {
#line 3503 "src/sema.tr"
            if ((!_tr_str_eqv((((HirParam*)List_ptr_get(f->params, pi))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 3504 "src/sema.tr"
                ({ __auto_type _ord_t1260 = (({ TrStr _tr_cl = (List_TrStr_get(keys, ii)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("#", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1261 = (({ TrStr _tr_cl = (_ord_t1260); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _tr_dict_set(self->fn_param_consumes, _tr_strz(_ord_t1261), false); _tr_str_release(_ord_t1261); });
            }
#line 3505 "src/sema.tr"
            pi = (pi + 1LL);
        }
#line 3506 "src/sema.tr"
        ii = (ii + 1LL);
    }
#line 3508 "src/sema.tr"
    bool changed = true;
#line 3509 "src/sema.tr"
    while (changed) {
#line 3510 "src/sema.tr"
        changed = false;
#line 3511 "src/sema.tr"
        long long xi = 0LL;
#line 3512 "src/sema.tr"
        while ((xi < fns->len)) {
#line 3513 "src/sema.tr"
            HirFunction* f = ((HirFunction*)List_ptr_get(fns, xi));
#line 3514 "src/sema.tr"
            long long pi = 0LL;
#line 3515 "src/sema.tr"
            while ((pi < f->params->len)) {
#line 3516 "src/sema.tr"
                TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pi))->name);
#line 3517 "src/sema.tr"
                TrStr pk = ({ __auto_type _ord_t1262 = (({ TrStr _tr_cl = (List_TrStr_get(keys, xi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("#", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t1262); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
#line 3518 "src/sema.tr"
                if (((!_tr_str_eqv((pn), (_tr_str_lit_len("self", 4LL)))) && _tr_dict_contains(self->fn_param_consumes, _tr_strz(pk)))) {
#line 3519 "src/sema.tr"
                    if ((!((bool)(uintptr_t)_tr_dict_get(self->fn_param_consumes, _tr_strz(pk))))) {
#line 3520 "src/sema.tr"
                        if (Sema__param_consumed_in_block(self, pn, f->body)) {
#line 3521 "src/sema.tr"
                            _tr_dict_set(self->fn_param_consumes, _tr_strz(pk), true);
#line 3522 "src/sema.tr"
                            changed = true;
                        }
                    }
                }
#line 3523 "src/sema.tr"
                pi = (pi + 1LL);
                _tr_str_release(pn);
                _tr_str_release(pk);
            }
#line 3524 "src/sema.tr"
            xi = (xi + 1LL);
        }
    }
    List_TrStr_free(keys);
    List_ptr_free_obj(fns, _trdrop_HirFunction);
}

__attribute__((hot)) bool Sema__is_pure_scalar(Sema* self, TrStr tn) {
#line 3530 "src/sema.tr"
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("void", 4LL))))) {
#line 3530 "src/sema.tr"
        return true;
    }
#line 3531 "src/sema.tr"
    if ((((_tr_str_eqv((tn), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i64", 3LL))))) {
#line 3531 "src/sema.tr"
        return true;
    }
#line 3532 "src/sema.tr"
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("u8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("usize", 5LL))))) {
#line 3532 "src/sema.tr"
        return true;
    }
#line 3533 "src/sema.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("f32", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("f64", 3LL))))) {
#line 3533 "src/sema.tr"
        return true;
    }
#line 3534 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__pc_owns_return(Sema* self, HirExpr* e) {
#line 3539 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 3539 "src/sema.tr"
        return true;
    }
#line 3540 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 3540 "src/sema.tr"
        return true;
    }
#line 3541 "src/sema.tr"
    return (!Sema__is_pure_scalar(self, hir_expr_type(e)->name));
}

__attribute__((hot)) void Sema_apply_borrow_drops(Sema* self, HirProgram* hp) {
#line 3558 "src/sema.tr"
    long long fi = 0LL;
#line 3559 "src/sema.tr"
    while ((fi < hp->functions->len)) {
#line 3560 "src/sema.tr"
        Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(hp->functions, fi))->body);
#line 3561 "src/sema.tr"
        fi = (fi + 1LL);
    }
#line 3562 "src/sema.tr"
    long long ci = 0LL;
#line 3563 "src/sema.tr"
    while ((ci < hp->classes->len)) {
#line 3564 "src/sema.tr"
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
#line 3565 "src/sema.tr"
        long long mi = 0LL;
#line 3566 "src/sema.tr"
        while ((mi < c->methods->len)) {
#line 3567 "src/sema.tr"
            Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(c->methods, mi))->body);
#line 3568 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 3569 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 3570 "src/sema.tr"
    long long eni = 0LL;
#line 3571 "src/sema.tr"
    while ((eni < hp->enums->len)) {
#line 3572 "src/sema.tr"
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, eni));
#line 3573 "src/sema.tr"
        long long emi = 0LL;
#line 3574 "src/sema.tr"
        while ((emi < en->methods->len)) {
#line 3575 "src/sema.tr"
            Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(en->methods, emi))->body);
#line 3576 "src/sema.tr"
            emi = (emi + 1LL);
        }
#line 3577 "src/sema.tr"
        eni = (eni + 1LL);
    }
}

__attribute__((hot)) void Sema__borrow_drops_block(Sema* self, HirBlock* b) {
#line 3580 "src/sema.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 3580 "src/sema.tr"
        return;
    }
#line 3581 "src/sema.tr"
    long long i = 0LL;
#line 3582 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 3583 "src/sema.tr"
        Sema__borrow_drops_recurse_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)));
#line 3584 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3585 "src/sema.tr"
    List_TrStr* add_names = (void*)List_TrStr_new();
#line 3586 "src/sema.tr"
    List_TrStr* add_cls = (void*)List_TrStr_new();
#line 3587 "src/sema.tr"
    i = 0LL;
#line 3588 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 3589 "src/sema.tr"
        __auto_type _t1263 = (*((HirStmt*)List_ptr_get(b->stmts, i)));
        if (_t1263.tag == HirStmt_SLet) {
            __auto_type nm = _t1263.data.SLet.name;
__auto_type _own = _t1263.data.SLet.ownership;
__auto_type _mut2 = _t1263.data.SLet.is_mut;
__auto_type _cst = _t1263.data.SLet.is_const;
__auto_type _shr = _t1263.data.SLet.is_shared;
__auto_type _ty = _t1263.data.SLet.ty;
__auto_type val = _t1263.data.SLet.val;
#line 3591 "src/sema.tr"
            TrStr cn = _tr_str_retain(_ty->name);
#line 3592 "src/sema.tr"
            if ((Sema__is_owned_droppable_class(self, cn) && Sema__is_fresh_owned_producer(self, val, cn))) {
#line 3593 "src/sema.tr"
                if ((!Sema__block_has_drop_or_reassign(self, b, nm))) {
#line 3597 "src/sema.tr"
                    self->pc_alias_consumes = Sema_class_method_exists(self, cn, _tr_str_lit_len("free", 4LL));
#line 3598 "src/sema.tr"
                    bool _bd_consumed = Sema__param_consumed_in_block(self, nm, b);
#line 3599 "src/sema.tr"
                    self->pc_alias_consumes = false;
#line 3600 "src/sema.tr"
                    if ((!_bd_consumed)) {
#line 3601 "src/sema.tr"
                        List_TrStr_append(add_names, nm);
#line 3602 "src/sema.tr"
                        List_TrStr_append(add_cls, cn);
                    }
                }
            }
            _tr_str_release(cn);
        } else if (1) {
            __auto_type _ = _t1263;
            /* pass */
        }
#line 3604 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3605 "src/sema.tr"
    long long ai = 0LL;
#line 3606 "src/sema.tr"
    while ((ai < add_names->len)) {
#line 3607 "src/sema.tr"
        ({ TrStr _ord_t1264 = (List_TrStr_get(add_names, ai)); TrStr _ord_t1265 = (List_TrStr_get(add_cls, ai)); Sema__insert_drop_before_terminator(self, b, _ord_t1264, _ord_t1265); _tr_str_release(_ord_t1264); _tr_str_release(_ord_t1265); });
#line 3608 "src/sema.tr"
        ai = (ai + 1LL);
    }
    List_TrStr_free(add_names);
    List_TrStr_free(add_cls);
}

__attribute__((hot)) void Sema__borrow_drops_recurse_stmt(Sema* self, HirStmt* sp) {
#line 3611 "src/sema.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 3611 "src/sema.tr"
        return;
    }
#line 3612 "src/sema.tr"
    __auto_type _t1266 = (*sp);
    if (_t1266.tag == HirStmt_SIf) {
        __auto_type tb = _t1266.data.SIf.then_b;
__auto_type eb = _t1266.data.SIf.else_b;
#line 3614 "src/sema.tr"
        Sema__borrow_drops_block(self, tb);
#line 3615 "src/sema.tr"
        Sema__borrow_drops_block(self, eb);
    } else if (_t1266.tag == HirStmt_SWhile) {
        __auto_type wb = _t1266.data.SWhile.body;
        Sema__borrow_drops_block(self, wb);
    } else if (_t1266.tag == HirStmt_SFor) {
        __auto_type fb = _t1266.data.SFor.body;
        Sema__borrow_drops_block(self, fb);
    } else if (_t1266.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t1266.data.SForUnpack.body;
        Sema__borrow_drops_block(self, fub);
    } else if (_t1266.tag == HirStmt_SMatch) {
        __auto_type arms = _t1266.data.SMatch.arms;
#line 3620 "src/sema.tr"
        long long ai = 0LL;
#line 3621 "src/sema.tr"
        while ((ai < arms->len)) {
#line 3622 "src/sema.tr"
            Sema__borrow_drops_block(self, ((HirMatchArm*)List_ptr_get(arms, ai))->body);
#line 3623 "src/sema.tr"
            ai = (ai + 1LL);
        }
    } else if (_t1266.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t1266.data.SUnsafe.body;
        Sema__borrow_drops_block(self, ub);
    } else if (_t1266.tag == HirStmt_SWith) {
        __auto_type wb = _t1266.data.SWith.body;
        Sema__borrow_drops_block(self, wb);
    } else if (_t1266.tag == HirStmt_STaskGroup) {
        __auto_type tb = _t1266.data.STaskGroup.body;
        Sema__borrow_drops_block(self, tb);
    } else if (_t1266.tag == HirStmt_STry) {
        __auto_type tb = _t1266.data.STry.try_body;
__auto_type catches = _t1266.data.STry.catches;
__auto_type fb = _t1266.data.STry.finally_b;
#line 3628 "src/sema.tr"
        Sema__borrow_drops_block(self, tb);
#line 3629 "src/sema.tr"
        Sema__borrow_drops_block(self, fb);
#line 3630 "src/sema.tr"
        long long ci = 0LL;
#line 3631 "src/sema.tr"
        while ((ci < catches->len)) {
#line 3632 "src/sema.tr"
            Sema__borrow_drops_block(self, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body);
#line 3633 "src/sema.tr"
            ci = (ci + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1266;
        /* pass */
    }
}

__attribute__((hot)) bool Sema__is_fresh_owned_producer(Sema* self, HirExpr* val, TrStr cn) {
#line 3640 "src/sema.tr"
    if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
#line 3640 "src/sema.tr"
        return false;
    }
#line 3641 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(val)))) {
#line 3641 "src/sema.tr"
        return false;
    }
#line 3642 "src/sema.tr"
    __auto_type _t1267 = (*val);
    if (_t1267.tag == HirExpr_ECall) {
        __auto_type callee = _t1267.data.ECall.callee;
#line 3644 "src/sema.tr"
        __auto_type _t1268 = (*callee);
        if (_t1268.tag == HirExpr_EIdent) {
            __auto_type fnm = _t1268.data.EIdent.name;
#line 3646 "src/sema.tr"
            if (_tr_str_eqv((fnm), (cn))) {
#line 3646 "src/sema.tr"
                return true;
            }
#line 3647 "src/sema.tr"
            if ((_tr_dict_contains(self->fn_ret_owned, _tr_strz(fnm)) && ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(fnm))))) {
#line 3647 "src/sema.tr"
                return true;
            }
        } else if (1) {
            __auto_type _ = _t1268;
            /* pass */
        }
    } else if (_t1267.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1267.data.EMethodCall.obj;
__auto_type m = _t1267.data.EMethodCall.method;
#line 3650 "src/sema.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("new", 3LL))))) {
#line 3650 "src/sema.tr"
            return true;
        }
#line 3651 "src/sema.tr"
        TrStr mkey = ({ TrStr _tr_cl = (_tr_strx_concatv((hir_expr_type(obj)->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3652 "src/sema.tr"
        if ((_tr_dict_contains(self->fn_ret_owned, _tr_strz(mkey)) && ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(mkey))))) {
#line 3652 "src/sema.tr"
            _tr_str_release(mkey);
            return true;
        }
        _tr_str_release(mkey);
    } else if (1) {
        __auto_type _ = _t1267;
        /* pass */
    }
#line 3654 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__is_owned_droppable_class(Sema* self, TrStr cn) {
#line 3657 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cn)))) {
#line 3657 "src/sema.tr"
        return false;
    }
#line 3658 "src/sema.tr"
    if ((((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cn)))->generics->len > 0LL)) {
#line 3658 "src/sema.tr"
        return false;
    }
#line 3659 "src/sema.tr"
    if (Sema_class_method_exists(self, cn, _tr_str_lit_len("free", 4LL))) {
#line 3659 "src/sema.tr"
        return true;
    }
#line 3660 "src/sema.tr"
    return Sema__is_rc_class(self, cn);
}

__attribute__((hot)) bool Sema__block_has_drop_or_reassign(Sema* self, HirBlock* b, TrStr nm) {
#line 3663 "src/sema.tr"
    long long i = 0LL;
#line 3664 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 3665 "src/sema.tr"
        __auto_type _t1269 = (*((HirStmt*)List_ptr_get(b->stmts, i)));
        if (_t1269.tag == HirStmt_SAutoDrop) {
            __auto_type dn = _t1269.data.SAutoDrop.name;
#line 3667 "src/sema.tr"
            if (_tr_str_eqv((dn), (nm))) {
#line 3667 "src/sema.tr"
                return true;
            }
        } else if (_t1269.tag == HirStmt_SFree) {
            __auto_type dn = _t1269.data.SFree.name;
#line 3669 "src/sema.tr"
            if (_tr_str_eqv((dn), (nm))) {
#line 3669 "src/sema.tr"
                return true;
            }
        } else if (_t1269.tag == HirStmt_SAssign) {
            __auto_type tgt = _t1269.data.SAssign.target;
#line 3671 "src/sema.tr"
            __auto_type _t1270 = (*tgt);
            if (_t1270.tag == HirExpr_EIdent) {
                __auto_type tn2 = _t1270.data.EIdent.name;
#line 3673 "src/sema.tr"
                if (_tr_str_eqv((tn2), (nm))) {
#line 3673 "src/sema.tr"
                    return true;
                }
            } else if (1) {
                __auto_type _ = _t1270;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t1269;
            /* pass */
        }
#line 3676 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3677 "src/sema.tr"
    return false;
}

__attribute__((hot)) void Sema__insert_drop_before_terminator(Sema* self, HirBlock* b, TrStr nm, TrStr cn) {
#line 3680 "src/sema.tr"
    HirStmt* drop = box_hirstmt(HirStmt_ctor_SAutoDrop(nm, cn));
#line 3681 "src/sema.tr"
    long long n = b->stmts->len;
#line 3682 "src/sema.tr"
    if ((n == 0LL)) {
#line 3683 "src/sema.tr"
        List_ptr_append(b->stmts, drop);
#line 3684 "src/sema.tr"
        return;
    }
#line 3685 "src/sema.tr"
    HirStmt* last = ((HirStmt*)List_ptr_get(b->stmts, (n - 1LL)));
#line 3686 "src/sema.tr"
    bool is_term = false;
#line 3687 "src/sema.tr"
    HirExpr* term_val = (HirExpr*)(0LL);
#line 3688 "src/sema.tr"
    __auto_type _t1271 = (*last);
    if (_t1271.tag == HirStmt_SReturn) {
        __auto_type rv = _t1271.data.SReturn.val;
#line 3690 "src/sema.tr"
        is_term = true;
#line 3691 "src/sema.tr"
        term_val = rv;
    } else if (_t1271.tag == HirStmt_SRaise) {
        __auto_type rv2 = _t1271.data.SRaise.val;
#line 3693 "src/sema.tr"
        is_term = true;
#line 3694 "src/sema.tr"
        term_val = rv2;
    } else if (_t1271.tag == HirStmt_SBreak) {
        __auto_type bv3 = _t1271.data.SBreak.val;
#line 3696 "src/sema.tr"
        is_term = true;
#line 3697 "src/sema.tr"
        term_val = bv3;
    } else if (_t1271.tag == HirStmt_SContinue) {
        is_term = true;
    } else if (1) {
        __auto_type _ = _t1271;
        /* pass */
    }
#line 3700 "src/sema.tr"
    if (is_term) {
#line 3713 "src/sema.tr"
        List_TrStr* term_refs = (void*)List_TrStr_new();
#line 3714 "src/sema.tr"
        if ((((unsigned long long)(term_val)) != ((unsigned long long)(0LL)))) {
#line 3714 "src/sema.tr"
            Sema_collect_idents(self, term_val, term_refs);
        }
#line 3715 "src/sema.tr"
        bool nm_referenced = false;
#line 3716 "src/sema.tr"
        long long tri = 0LL;
#line 3717 "src/sema.tr"
        while ((tri < term_refs->len)) {
#line 3718 "src/sema.tr"
            if (({ TrStr _cmpa_t1272 = (List_TrStr_get(term_refs, tri)); TrStr _cmpb_t1273 = (nm); _Bool _cmpr_t1274 = _tr_str_eqv(_cmpa_t1272, _cmpb_t1273); _tr_str_release(_cmpa_t1272); _cmpr_t1274; })) {
#line 3718 "src/sema.tr"
                nm_referenced = true;
            }
#line 3719 "src/sema.tr"
            tri = (tri + 1LL);
        }
#line 3720 "src/sema.tr"
        if (nm_referenced) {
#line 3720 "src/sema.tr"
            return;
        }
#line 3721 "src/sema.tr"
        List_ptr_remove(b->stmts, (n - 1LL));
#line 3722 "src/sema.tr"
        List_ptr_append(b->stmts, drop);
#line 3723 "src/sema.tr"
        List_ptr_append(b->stmts, last);
    } else {
#line 3725 "src/sema.tr"
        List_ptr_append(b->stmts, drop);
    }
}

__attribute__((hot)) bool Sema__pc_is_ident(Sema* self, HirExpr* ep, TrStr name) {
#line 3728 "src/sema.tr"
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
#line 3728 "src/sema.tr"
        return false;
    }
#line 3729 "src/sema.tr"
    __auto_type _t1275 = (*ep);
    if (_t1275.tag == HirExpr_EIdent) {
        __auto_type n = _t1275.data.EIdent.name;
        bool _trret215 = _tr_str_eqv((n), (name));
        return _trret215;
    } else if (1) {
        __auto_type _ = _t1275;
        return false;
    }
}

__attribute__((hot)) TrStr Sema__pc_callee_name(Sema* self, HirExpr* callee) {
#line 3734 "src/sema.tr"
    if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
#line 3734 "src/sema.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 3735 "src/sema.tr"
    __auto_type _t1276 = (*callee);
    if (_t1276.tag == HirExpr_EIdent) {
        __auto_type n = _t1276.data.EIdent.name;
        TrStr _trret216 = _tr_str_retain(n);
        return _trret216;
    } else if (1) {
        __auto_type _ = _t1276;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool Sema__pc_stores_method(Sema* self, TrStr m) {
#line 3740 "src/sema.tr"
    return ((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL))));
}

__attribute__((hot)) bool Sema__pc_is_store_target(Sema* self, HirExpr* tgt) {
#line 3743 "src/sema.tr"
    if ((((unsigned long long)(tgt)) == ((unsigned long long)(0LL)))) {
#line 3743 "src/sema.tr"
        return false;
    }
#line 3744 "src/sema.tr"
    __auto_type _t1277 = (*tgt);
    if (_t1277.tag == HirExpr_EPropAccess) {
        return true;
    } else if (_t1277.tag == HirExpr_EIndex) {
        return true;
    } else if (1) {
        __auto_type _ = _t1277;
        return false;
    }
}

__attribute__((hot)) bool Sema__pc_refs(Sema* self, HirExpr* ep, TrStr name) {
#line 3751 "src/sema.tr"
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
#line 3751 "src/sema.tr"
        return false;
    }
#line 3752 "src/sema.tr"
    __auto_type _t1278 = (*ep);
    if (_t1278.tag == HirExpr_EIdent) {
        __auto_type n = _t1278.data.EIdent.name;
        bool _trret217 = _tr_str_eqv((n), (name));
        return _trret217;
    } else if (_t1278.tag == HirExpr_EBinOp) {
        __auto_type l = _t1278.data.EBinOp.left;
__auto_type r = _t1278.data.EBinOp.right;
        return (Sema__pc_refs(self, l, name) || Sema__pc_refs(self, r, name));
    } else if (_t1278.tag == HirExpr_EUnaryOp) {
        __auto_type e = _t1278.data.EUnaryOp.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t1278.tag == HirExpr_ECast) {
        __auto_type e = _t1278.data.ECast.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t1278.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1278.data.EPropAccess.obj;
        return Sema__pc_refs(self, o, name);
    } else if (_t1278.tag == HirExpr_EIndex) {
        __auto_type o = _t1278.data.EIndex.obj;
__auto_type ix = _t1278.data.EIndex._tr_v_index;
        return (Sema__pc_refs(self, o, name) || Sema__pc_refs(self, ix, name));
    } else if (_t1278.tag == HirExpr_ECall) {
        __auto_type cl = _t1278.data.ECall.callee;
__auto_type args = _t1278.data.ECall.args;
        return (Sema__pc_refs(self, cl, name) || Sema__pc_anyrefs(self, args, name));
    } else if (_t1278.tag == HirExpr_EMethodCall) {
        __auto_type o = _t1278.data.EMethodCall.obj;
__auto_type args = _t1278.data.EMethodCall.args;
        return (Sema__pc_refs(self, o, name) || Sema__pc_anyrefs(self, args, name));
    } else if (_t1278.tag == HirExpr_EList) {
        __auto_type items = _t1278.data.EList.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t1278.tag == HirExpr_ESet) {
        __auto_type items = _t1278.data.ESet.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t1278.tag == HirExpr_ETuple) {
        __auto_type items = _t1278.data.ETuple.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t1278.tag == HirExpr_EDict) {
        __auto_type dks = _t1278.data.EDict.keys;
__auto_type dvs = _t1278.data.EDict.vals;
        return (Sema__pc_anyrefs(self, dks, name) || Sema__pc_anyrefs(self, dvs, name));
    } else if (_t1278.tag == HirExpr_ETryExpr) {
        __auto_type e = _t1278.data.ETryExpr.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t1278.tag == HirExpr_EAwait) {
        __auto_type e = _t1278.data.EAwait.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t1278.tag == HirExpr_EIfElse) {
        __auto_type cnd = _t1278.data.EIfElse.cond;
__auto_type te = _t1278.data.EIfElse.then_e;
__auto_type ee = _t1278.data.EIfElse.else_e;
        return ((Sema__pc_refs(self, cnd, name) || Sema__pc_refs(self, te, name)) || Sema__pc_refs(self, ee, name));
    } else if (_t1278.tag == HirExpr_ESlice) {
        __auto_type sa = _t1278.data.ESlice.start;
__auto_type sb = _t1278.data.ESlice.stop;
__auto_type sc = _t1278.data.ESlice.step;
        return ((Sema__pc_refs(self, sa, name) || Sema__pc_refs(self, sb, name)) || Sema__pc_refs(self, sc, name));
    } else if (_t1278.tag == HirExpr_ERange) {
        __auto_type ra = _t1278.data.ERange.start;
__auto_type rb = _t1278.data.ERange.end;
        return (Sema__pc_refs(self, ra, name) || Sema__pc_refs(self, rb, name));
    } else if (1) {
        __auto_type _ = _t1278;
        return false;
    }
}

__attribute__((hot)) bool Sema__pc_anyrefs(Sema* self, List_ptr* args, TrStr name) {
#line 3773 "src/sema.tr"
    long long i = 0LL;
#line 3774 "src/sema.tr"
    while ((i < args->len)) {
#line 3775 "src/sema.tr"
        if (Sema__pc_refs(self, ((HirExpr*)List_ptr_get(args, i)), name)) {
#line 3775 "src/sema.tr"
            return true;
        }
#line 3776 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3777 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__param_consumed_in_block(Sema* self, TrStr pname, HirBlock* b) {
#line 3780 "src/sema.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 3780 "src/sema.tr"
        return false;
    }
#line 3781 "src/sema.tr"
    long long i = 0LL;
#line 3782 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 3783 "src/sema.tr"
        if (Sema__param_consumed_in_stmt(self, pname, ((HirStmt*)List_ptr_get(b->stmts, i)))) {
#line 3783 "src/sema.tr"
            return true;
        }
#line 3784 "src/sema.tr"
        i = (i + 1LL);
    }
#line 3785 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__param_consumed_in_stmt(Sema* self, TrStr pname, HirStmt* sp) {
#line 3788 "src/sema.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 3788 "src/sema.tr"
        return false;
    }
#line 3789 "src/sema.tr"
    __auto_type _t1279 = (*sp);
    if (_t1279.tag == HirStmt_SReturn) {
        __auto_type e = _t1279.data.SReturn.val;
#line 3797 "src/sema.tr"
        if ((!Sema__pc_owns_return(self, e))) {
#line 3797 "src/sema.tr"
            return false;
        }
#line 3798 "src/sema.tr"
        return Sema__pc_refs(self, e, pname);
    } else if (_t1279.tag == HirStmt_SRaise) {
        __auto_type e = _t1279.data.SRaise.val;
#line 3800 "src/sema.tr"
        if ((!Sema__pc_owns_return(self, e))) {
#line 3800 "src/sema.tr"
            return false;
        }
#line 3801 "src/sema.tr"
        return Sema__pc_refs(self, e, pname);
    } else if (_t1279.tag == HirStmt_SFree) {
        __auto_type nm = _t1279.data.SFree.name;
        bool _trret218 = _tr_str_eqv((nm), (pname));
        return _trret218;
    } else if (_t1279.tag == HirStmt_SSpawn) {
        __auto_type e = _t1279.data.SSpawn.expr;
        return Sema__pc_refs(self, e, pname);
    } else if (_t1279.tag == HirStmt_SAssign) {
        __auto_type tgt = _t1279.data.SAssign.target;
__auto_type val = _t1279.data.SAssign.val;
#line 3805 "src/sema.tr"
        if ((Sema__pc_is_store_target(self, tgt) && Sema__pc_refs(self, val, pname))) {
#line 3805 "src/sema.tr"
            return true;
        }
#line 3806 "src/sema.tr"
        if ((self->pc_alias_consumes && Sema__pc_is_ident(self, val, pname))) {
#line 3806 "src/sema.tr"
            return true;
        }
#line 3807 "src/sema.tr"
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t1279.tag == HirStmt_SLet) {
        __auto_type val = _t1279.data.SLet.val;
#line 3809 "src/sema.tr"
        if ((self->pc_alias_consumes && Sema__pc_is_ident(self, val, pname))) {
#line 3809 "src/sema.tr"
            return true;
        }
#line 3810 "src/sema.tr"
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t1279.tag == HirStmt_SMultiLet) {
        __auto_type val = _t1279.data.SMultiLet.val;
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t1279.tag == HirStmt_SExpr) {
        __auto_type e = _t1279.data.SExpr.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t1279.tag == HirStmt_SIf) {
        __auto_type tb = _t1279.data.SIf.then_b;
__auto_type eb = _t1279.data.SIf.else_b;
        bool _trret219 = (Sema__param_consumed_in_block(self, pname, tb) || Sema__param_consumed_in_block(self, pname, eb));
        return _trret219;
    } else if (_t1279.tag == HirStmt_SWhile) {
        __auto_type wb = _t1279.data.SWhile.body;
        bool _trret220 = Sema__param_consumed_in_block(self, pname, wb);
        return _trret220;
    } else if (_t1279.tag == HirStmt_SFor) {
        __auto_type fb = _t1279.data.SFor.body;
        bool _trret221 = Sema__param_consumed_in_block(self, pname, fb);
        return _trret221;
    } else if (_t1279.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t1279.data.SForUnpack.body;
        bool _trret222 = Sema__param_consumed_in_block(self, pname, fub);
        return _trret222;
    } else if (_t1279.tag == HirStmt_SMatch) {
        __auto_type arms = _t1279.data.SMatch.arms;
#line 3818 "src/sema.tr"
        long long ai = 0LL;
#line 3819 "src/sema.tr"
        while ((ai < arms->len)) {
#line 3820 "src/sema.tr"
            if (Sema__param_consumed_in_block(self, pname, ((HirMatchArm*)List_ptr_get(arms, ai))->body)) {
#line 3820 "src/sema.tr"
                return true;
            }
#line 3821 "src/sema.tr"
            ai = (ai + 1LL);
        }
#line 3822 "src/sema.tr"
        return false;
    } else if (_t1279.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t1279.data.SUnsafe.body;
        bool _trret223 = Sema__param_consumed_in_block(self, pname, ub);
        return _trret223;
    } else if (_t1279.tag == HirStmt_SWith) {
        __auto_type wb = _t1279.data.SWith.body;
        bool _trret224 = Sema__param_consumed_in_block(self, pname, wb);
        return _trret224;
    } else if (_t1279.tag == HirStmt_STaskGroup) {
        __auto_type tb = _t1279.data.STaskGroup.body;
        bool _trret225 = Sema__param_consumed_in_block(self, pname, tb);
        return _trret225;
    } else if (_t1279.tag == HirStmt_SDefer) {
        __auto_type ds = _t1279.data.SDefer.stmt;
        return Sema__param_consumed_in_stmt(self, pname, ds);
    } else if (_t1279.tag == HirStmt_STry) {
        __auto_type tb = _t1279.data.STry.try_body;
__auto_type catches = _t1279.data.STry.catches;
__auto_type fb = _t1279.data.STry.finally_b;
#line 3828 "src/sema.tr"
        if (Sema__param_consumed_in_block(self, pname, tb)) {
#line 3828 "src/sema.tr"
            return true;
        }
#line 3829 "src/sema.tr"
        if (Sema__param_consumed_in_block(self, pname, fb)) {
#line 3829 "src/sema.tr"
            return true;
        }
#line 3830 "src/sema.tr"
        long long ci = 0LL;
#line 3831 "src/sema.tr"
        while ((ci < catches->len)) {
#line 3832 "src/sema.tr"
            if (Sema__param_consumed_in_block(self, pname, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body)) {
#line 3832 "src/sema.tr"
                return true;
            }
#line 3833 "src/sema.tr"
            ci = (ci + 1LL);
        }
#line 3834 "src/sema.tr"
        return false;
    } else if (1) {
        __auto_type _ = _t1279;
        return false;
    }
}

__attribute__((hot)) bool Sema__param_consumed_in_expr(Sema* self, TrStr pname, HirExpr* ep) {
#line 3840 "src/sema.tr"
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
#line 3840 "src/sema.tr"
        return false;
    }
#line 3841 "src/sema.tr"
    __auto_type _t1280 = (*ep);
    if (_t1280.tag == HirExpr_ECall) {
        __auto_type callee = _t1280.data.ECall.callee;
__auto_type args = _t1280.data.ECall.args;
#line 3843 "src/sema.tr"
        TrStr gname = Sema__pc_callee_name(self, callee);
#line 3844 "src/sema.tr"
        long long j = 0LL;
#line 3845 "src/sema.tr"
        while ((j < args->len)) {
#line 3846 "src/sema.tr"
            if (Sema__pc_is_ident(self, ((HirExpr*)List_ptr_get(args, j)), pname)) {
#line 3847 "src/sema.tr"
                if (_tr_str_eqv((gname), (_tr_str_lit_len("", 0LL)))) {
#line 3847 "src/sema.tr"
                    _tr_str_release(gname);
                    return true;
                }
#line 3848 "src/sema.tr"
                TrStr ck = ({ TrStr _tr_cl = (_tr_strx_concatv((gname), (_tr_str_lit_len("#", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(j)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 3849 "src/sema.tr"
                if ((!_tr_dict_contains(self->fn_param_consumes, _tr_strz(ck)))) {
#line 3849 "src/sema.tr"
                    _tr_str_release(gname);
                    _tr_str_release(ck);
                    return true;
                }
#line 3850 "src/sema.tr"
                if (((bool)(uintptr_t)_tr_dict_get(self->fn_param_consumes, _tr_strz(ck)))) {
#line 3850 "src/sema.tr"
                    _tr_str_release(gname);
                    _tr_str_release(ck);
                    return true;
                }
            } else {
#line 3852 "src/sema.tr"
                if (Sema__param_consumed_in_expr(self, pname, ((HirExpr*)List_ptr_get(args, j)))) {
#line 3852 "src/sema.tr"
                    _tr_str_release(gname);
                    return true;
                }
            }
#line 3853 "src/sema.tr"
            j = (j + 1LL);
        }
#line 3854 "src/sema.tr"
        bool _trret226 = Sema__param_consumed_in_expr(self, pname, callee);
        _tr_str_release(gname);
        return _trret226;
    } else if (_t1280.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t1280.data.EMethodCall.obj;
__auto_type method = _t1280.data.EMethodCall.method;
__auto_type args = _t1280.data.EMethodCall.args;
#line 3860 "src/sema.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL))) && Sema__pc_is_ident(self, obj, pname))) {
#line 3860 "src/sema.tr"
            return true;
        }
#line 3861 "src/sema.tr"
        long long j2 = 0LL;
#line 3862 "src/sema.tr"
        while ((j2 < args->len)) {
#line 3863 "src/sema.tr"
            if (Sema__pc_is_ident(self, ((HirExpr*)List_ptr_get(args, j2)), pname)) {
#line 3863 "src/sema.tr"
                return true;
            }
#line 3864 "src/sema.tr"
            if (Sema__param_consumed_in_expr(self, pname, ((HirExpr*)List_ptr_get(args, j2)))) {
#line 3864 "src/sema.tr"
                return true;
            }
#line 3865 "src/sema.tr"
            j2 = (j2 + 1LL);
        }
#line 3866 "src/sema.tr"
        bool _trret227 = Sema__param_consumed_in_expr(self, pname, obj);
        return _trret227;
    } else if (_t1280.tag == HirExpr_EList) {
        __auto_type items = _t1280.data.EList.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t1280.tag == HirExpr_ESet) {
        __auto_type items = _t1280.data.ESet.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t1280.tag == HirExpr_ETuple) {
        __auto_type items = _t1280.data.ETuple.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t1280.tag == HirExpr_EDict) {
        __auto_type dks = _t1280.data.EDict.keys;
__auto_type dvs = _t1280.data.EDict.vals;
        return (Sema__pc_anyrefs(self, dks, pname) || Sema__pc_anyrefs(self, dvs, pname));
    } else if (_t1280.tag == HirExpr_EClosure) {
        __auto_type caps = _t1280.data.EClosure.captures;
#line 3872 "src/sema.tr"
        long long cj = 0LL;
#line 3873 "src/sema.tr"
        while ((cj < caps->len)) {
#line 3874 "src/sema.tr"
            if (_tr_str_eqv((((HirParam*)List_ptr_get(caps, cj))->name), (pname))) {
#line 3874 "src/sema.tr"
                return true;
            }
#line 3875 "src/sema.tr"
            cj = (cj + 1LL);
        }
#line 3876 "src/sema.tr"
        return false;
    } else if (_t1280.tag == HirExpr_EBinOp) {
        __auto_type l = _t1280.data.EBinOp.left;
__auto_type r = _t1280.data.EBinOp.right;
        return (Sema__param_consumed_in_expr(self, pname, l) || Sema__param_consumed_in_expr(self, pname, r));
    } else if (_t1280.tag == HirExpr_EUnaryOp) {
        __auto_type e = _t1280.data.EUnaryOp.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t1280.tag == HirExpr_ECast) {
        __auto_type e = _t1280.data.ECast.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t1280.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1280.data.EPropAccess.obj;
        return Sema__param_consumed_in_expr(self, pname, o);
    } else if (_t1280.tag == HirExpr_EIndex) {
        __auto_type o = _t1280.data.EIndex.obj;
__auto_type ix = _t1280.data.EIndex._tr_v_index;
        return (Sema__param_consumed_in_expr(self, pname, o) || Sema__param_consumed_in_expr(self, pname, ix));
    } else if (_t1280.tag == HirExpr_EIfElse) {
        __auto_type cnd = _t1280.data.EIfElse.cond;
__auto_type te = _t1280.data.EIfElse.then_e;
__auto_type ee = _t1280.data.EIfElse.else_e;
        return ((Sema__param_consumed_in_expr(self, pname, cnd) || Sema__param_consumed_in_expr(self, pname, te)) || Sema__param_consumed_in_expr(self, pname, ee));
    } else if (_t1280.tag == HirExpr_EAwait) {
        __auto_type e = _t1280.data.EAwait.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t1280.tag == HirExpr_ETryExpr) {
        __auto_type e = _t1280.data.ETryExpr.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (1) {
        __auto_type _ = _t1280;
        return false;
    }
}

__attribute__((hot)) bool Sema__fn_ret_is_heap_class(Sema* self, HirFunction* f) {
#line 3888 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) == ((unsigned long long)(0LL)))) {
#line 3888 "src/sema.tr"
        return false;
    }
#line 3889 "src/sema.tr"
    TrStr rn = _tr_str_retain(f->ret_ty->name);
#line 3890 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(rn)))) {
#line 3890 "src/sema.tr"
        _tr_str_release(rn);
        return false;
    }
#line 3891 "src/sema.tr"
    if (((((((((((((((_tr_str_eqv((rn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((rn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Set", 3LL)))) || (_tr_str_eqv((rn), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((rn), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((rn), (_tr_str_lit_len("StringObj", 9LL))))) {
#line 3891 "src/sema.tr"
        _tr_str_release(rn);
        return false;
    }
#line 3893 "src/sema.tr"
    ClassDef* cinfo = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(rn)));
#line 3894 "src/sema.tr"
    if ((!cinfo->is_class)) {
#line 3894 "src/sema.tr"
        _tr_str_release(rn);
        return false;
    }
#line 3895 "src/sema.tr"
    _tr_str_release(rn);
    return true;
}

__attribute__((hot)) void Sema__collect_returns(Sema* self, HirBlock* b, List_ptr* out) {
#line 3901 "src/sema.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 3901 "src/sema.tr"
        return;
    }
#line 3902 "src/sema.tr"
    long long i = 0LL;
#line 3903 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 3904 "src/sema.tr"
        Sema__collect_returns_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)), out);
#line 3905 "src/sema.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema__collect_returns_stmt(Sema* self, HirStmt* sp, List_ptr* out) {
#line 3908 "src/sema.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 3908 "src/sema.tr"
        return;
    }
#line 3909 "src/sema.tr"
    __auto_type _t1281 = (*sp);
    if (_t1281.tag == HirStmt_SReturn) {
        __auto_type re = _t1281.data.SReturn.val;
#line 3911 "src/sema.tr"
        if ((((unsigned long long)(re)) != ((unsigned long long)(0LL)))) {
#line 3911 "src/sema.tr"
            List_ptr_append(out, re);
        }
    } else if (_t1281.tag == HirStmt_SIf) {
        __auto_type tb = _t1281.data.SIf.then_b;
__auto_type eb = _t1281.data.SIf.else_b;
#line 3913 "src/sema.tr"
        Sema__collect_returns(self, tb, out);
#line 3914 "src/sema.tr"
        Sema__collect_returns(self, eb, out);
    } else if (_t1281.tag == HirStmt_SWhile) {
        __auto_type wb = _t1281.data.SWhile.body;
        Sema__collect_returns(self, wb, out);
    } else if (_t1281.tag == HirStmt_SFor) {
        __auto_type fb = _t1281.data.SFor.body;
        Sema__collect_returns(self, fb, out);
    } else if (_t1281.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t1281.data.SForUnpack.body;
        Sema__collect_returns(self, fub, out);
    } else if (_t1281.tag == HirStmt_SMatch) {
        __auto_type marms = _t1281.data.SMatch.arms;
#line 3919 "src/sema.tr"
        long long ai = 0LL;
#line 3920 "src/sema.tr"
        while ((ai < marms->len)) {
#line 3921 "src/sema.tr"
            Sema__collect_returns(self, ((HirMatchArm*)List_ptr_get(marms, ai))->body, out);
#line 3922 "src/sema.tr"
            ai = (ai + 1LL);
        }
    } else if (_t1281.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t1281.data.SUnsafe.body;
        Sema__collect_returns(self, ub, out);
    } else if (_t1281.tag == HirStmt_SWith) {
        __auto_type wb2 = _t1281.data.SWith.body;
        Sema__collect_returns(self, wb2, out);
    } else if (_t1281.tag == HirStmt_STaskGroup) {
        __auto_type tgb = _t1281.data.STaskGroup.body;
        Sema__collect_returns(self, tgb, out);
    } else if (_t1281.tag == HirStmt_SGpuBlock) {
        __auto_type gb = _t1281.data.SGpuBlock.body;
        Sema__collect_returns(self, gb, out);
    } else if (_t1281.tag == HirStmt_SDefer) {
        __auto_type ds = _t1281.data.SDefer.stmt;
        Sema__collect_returns_stmt(self, ds, out);
    } else if (_t1281.tag == HirStmt_STry) {
        __auto_type tb2 = _t1281.data.STry.try_body;
__auto_type catches = _t1281.data.STry.catches;
__auto_type fb2 = _t1281.data.STry.finally_b;
#line 3929 "src/sema.tr"
        Sema__collect_returns(self, tb2, out);
#line 3930 "src/sema.tr"
        long long ci = 0LL;
#line 3931 "src/sema.tr"
        while ((ci < catches->len)) {
#line 3932 "src/sema.tr"
            HirCatchClause** cc = ((HirCatchClause**)List_ptr_get(catches, ci));
#line 3933 "src/sema.tr"
            if ((((unsigned long long)(cc)) != ((unsigned long long)(0LL)))) {
#line 3933 "src/sema.tr"
                Sema__collect_returns(self, (*cc)->body, out);
            }
#line 3934 "src/sema.tr"
            ci = (ci + 1LL);
        }
#line 3935 "src/sema.tr"
        Sema__collect_returns(self, fb2, out);
    } else if (1) {
        __auto_type _ = _t1281;
        /* pass */
    }
}

__attribute__((hot)) bool Sema__owned_of(Sema* self, TrStr key) {
#line 3940 "src/sema.tr"
    if (_tr_dict_contains(self->fn_ret_owned, _tr_strz(key))) {
#line 3940 "src/sema.tr"
        return ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(key)));
    }
#line 3941 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__ret_yields_owned(Sema* self, HirExpr* e) {
#line 3944 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 3944 "src/sema.tr"
        return false;
    }
#line 3945 "src/sema.tr"
    __auto_type _t1282 = (*e);
    if (_t1282.tag == HirExpr_EIdent) {
        return true;
    } else if (_t1282.tag == HirExpr_EPropAccess) {
        return true;
    } else if (_t1282.tag == HirExpr_ECall) {
        __auto_type callee = _t1282.data.ECall.callee;
#line 3949 "src/sema.tr"
        if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
#line 3949 "src/sema.tr"
            return false;
        }
#line 3950 "src/sema.tr"
        __auto_type _t1283 = (*callee);
        if (_t1283.tag == HirExpr_EIdent) {
            __auto_type cn = _t1283.data.EIdent.name;
#line 3952 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(cn))) {
#line 3952 "src/sema.tr"
                return true;
            }
#line 3953 "src/sema.tr"
            bool _trret228 = Sema__owned_of(self, cn);
            return _trret228;
        } else if (1) {
            __auto_type _ = _t1283;
            return false;
        }
    } else if (_t1282.tag == HirExpr_EMethodCall) {
        __auto_type _tr_v_recv = _t1282.data.EMethodCall.obj;
__auto_type m = _t1282.data.EMethodCall.method;
#line 3956 "src/sema.tr"
        TrStr rt = _tr_str_retain(hir_expr_type(_tr_v_recv)->name);
#line 3958 "src/sema.tr"
        if ((((((_tr_str_eqv((rt), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((rt), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((rt), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((rt), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((rt), (_tr_str_lit_len("Set", 3LL)))) && ((_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("last", 4LL)))))) {
#line 3959 "src/sema.tr"
            _tr_str_release(rt);
            return false;
        }
#line 3961 "src/sema.tr"
        __auto_type _t1284 = (*_tr_v_recv);
        if (_t1284.tag == HirExpr_EIdent) {
            __auto_type rn = _t1284.data.EIdent.name;
#line 3963 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(rn))) {
#line 3963 "src/sema.tr"
                TrStr _strtmp_t1285 = _tr_str_retain(rn);
                _tr_str_release(rt);
                rt = _strtmp_t1285;
            }
        } else if (1) {
            __auto_type _ = _t1284;
            /* pass */
        }
#line 3965 "src/sema.tr"
        bool _trret229 = ({ TrStr _at_t1286 = (({ TrStr _tr_cl = (_tr_strx_concatv((rt), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (Sema__owned_of(self, _at_t1286)); _tr_str_release(_at_t1286); _wr; });
        _tr_str_release(rt);
        return _trret229;
    } else if (_t1282.tag == HirExpr_EIndex) {
        return false;
    } else if (_t1282.tag == HirExpr_EIfElse) {
        __auto_type a = _t1282.data.EIfElse.then_e;
__auto_type b2 = _t1282.data.EIfElse.else_e;
#line 3968 "src/sema.tr"
        return (Sema__ret_yields_owned(self, a) && Sema__ret_yields_owned(self, b2));
    } else if (1) {
        __auto_type _ = _t1282;
        return false;
    }
}

__attribute__((hot)) bool Sema__is_type_param_in_scope(Sema* self, TrStr name) {
#line 3980 "src/sema.tr"
    if ((_tr_strlen(_tr_strz(name)) == 1LL)) {
#line 3980 "src/sema.tr"
        return true;
    }
#line 3981 "src/sema.tr"
    long long i = 0LL;
#line 3982 "src/sema.tr"
    while ((i < self->current_func_generics->len)) {
#line 3983 "src/sema.tr"
        if (({ TrStr _cmpa_t1287 = (List_TrStr_get(self->current_func_generics, i)); TrStr _cmpb_t1288 = (name); _Bool _cmpr_t1289 = _tr_str_eqv(_cmpa_t1287, _cmpb_t1288); _tr_str_release(_cmpa_t1287); _cmpr_t1289; })) {
#line 3983 "src/sema.tr"
            return true;
        }
#line 3984 "src/sema.tr"
        i = (i + 1LL);
    }
#line 4000 "src/sema.tr"
    if (((!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->classes, _tr_strz(self->current_class_name)))) {
#line 4001 "src/sema.tr"
        ClassDef* ccls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
#line 4002 "src/sema.tr"
        long long ci = 0LL;
#line 4003 "src/sema.tr"
        while ((ci < ccls->generics->len)) {
#line 4004 "src/sema.tr"
            if (({ TrStr _cmpa_t1290 = (List_TrStr_get(ccls->generics, ci)); TrStr _cmpb_t1291 = (name); _Bool _cmpr_t1292 = _tr_str_eqv(_cmpa_t1290, _cmpb_t1291); _tr_str_release(_cmpa_t1290); _cmpr_t1292; })) {
#line 4004 "src/sema.tr"
                return true;
            }
#line 4005 "src/sema.tr"
            ci = (ci + 1LL);
        }
    }
#line 4006 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema__type_satisfies_bound(Sema* self, TrStr type_name, TrStr iface_name) {
#line 4009 "src/sema.tr"
    if ((!_tr_dict_contains(self->interfaces, _tr_strz(iface_name)))) {
#line 4009 "src/sema.tr"
        return true;
    }
#line 4012 "src/sema.tr"
    if (Sema__is_type_param_in_scope(self, type_name)) {
#line 4012 "src/sema.tr"
        return true;
    }
#line 4013 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(type_name)))) {
#line 4013 "src/sema.tr"
        return false;
    }
#line 4014 "src/sema.tr"
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(type_name)));
#line 4015 "src/sema.tr"
    long long ii = 0LL;
#line 4016 "src/sema.tr"
    while ((ii < cls->iface_names->len)) {
#line 4017 "src/sema.tr"
        if (({ TrStr _cmpa_t1293 = (List_TrStr_get(cls->iface_names, ii)); TrStr _cmpb_t1294 = (iface_name); _Bool _cmpr_t1295 = _tr_str_eqv(_cmpa_t1293, _cmpb_t1294); _tr_str_release(_cmpa_t1293); _cmpr_t1295; })) {
#line 4017 "src/sema.tr"
            return true;
        }
#line 4018 "src/sema.tr"
        ii = (ii + 1LL);
    }
#line 4019 "src/sema.tr"
    InterfaceDef* idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(iface_name)));
#line 4020 "src/sema.tr"
    long long mi = 0LL;
#line 4021 "src/sema.tr"
    while ((mi < idef->methods->len)) {
#line 4022 "src/sema.tr"
        TrStr imeth_nm = _tr_str_retain(((FunctionDef*)List_ptr_get(idef->methods, mi))->name);
#line 4023 "src/sema.tr"
        bool found = false;
#line 4024 "src/sema.tr"
        long long cmi = 0LL;
#line 4025 "src/sema.tr"
        while ((cmi < cls->methods->len)) {
#line 4026 "src/sema.tr"
            if (_tr_str_eqv((((FunctionDef*)List_ptr_get(cls->methods, cmi))->name), (imeth_nm))) {
#line 4026 "src/sema.tr"
                found = true;
            }
#line 4027 "src/sema.tr"
            cmi = (cmi + 1LL);
        }
#line 4028 "src/sema.tr"
        if ((!found)) {
#line 4028 "src/sema.tr"
            _tr_str_release(imeth_nm);
            return false;
        }
#line 4029 "src/sema.tr"
        mi = (mi + 1LL);
        _tr_str_release(imeth_nm);
    }
#line 4030 "src/sema.tr"
    return true;
}

__attribute__((hot)) AstType** Sema__resolve_generic_bound_method_ret(Sema* self, TrStr pname, TrStr method) {
#line 4041 "src/sema.tr"
    long long ci = 0LL;
#line 4042 "src/sema.tr"
    while ((ci < self->current_func_constraints->len)) {
#line 4043 "src/sema.tr"
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(self->current_func_constraints, ci));
#line 4044 "src/sema.tr"
        if (_tr_str_eqv((gc->target), (pname))) {
#line 4045 "src/sema.tr"
            long long bi = 0LL;
#line 4046 "src/sema.tr"
            while ((bi < gc->bounds->len)) {
#line 4047 "src/sema.tr"
                AstType* bound_ty = (*((AstType**)List_ptr_get(gc->bounds, bi)));
#line 4048 "src/sema.tr"
                if (_tr_dict_contains(self->interfaces, _tr_strz(bound_ty->name))) {
#line 4049 "src/sema.tr"
                    InterfaceDef* idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(bound_ty->name)));
#line 4050 "src/sema.tr"
                    long long mi = 0LL;
#line 4051 "src/sema.tr"
                    while ((mi < idef->methods->len)) {
#line 4052 "src/sema.tr"
                        FunctionDef* imdef = ((FunctionDef*)List_ptr_get(idef->methods, mi));
#line 4053 "src/sema.tr"
                        if ((_tr_str_eqv((imdef->name), (method)) && (((unsigned long long)(imdef->ret_ty)) != ((unsigned long long)(0LL))))) {
#line 4054 "src/sema.tr"
                            AstType* found_ty = (*imdef->ret_ty);
#line 4058 "src/sema.tr"
                            if (((idef->generics->len > 0LL) && (bound_ty->args->len > 0LL))) {
#line 4059 "src/sema.tr"
                                long long gi = 0LL;
#line 4060 "src/sema.tr"
                                while ((gi < idef->generics->len)) {
#line 4061 "src/sema.tr"
                                    if ((({ __auto_type _ord_t1296 = (List_TrStr_get(idef->generics, gi)); __auto_type _wr = (({ TrStr _cmpa_t1297 = (_ord_t1296); TrStr _cmpb_t1298 = (found_ty->name); _Bool _cmpr_t1299 = _tr_str_eqv(_cmpa_t1297, _cmpb_t1298); _tr_str_release(_cmpa_t1297); _cmpr_t1299; })); _wr; }) && (gi < bound_ty->args->len))) {
#line 4062 "src/sema.tr"
                                        found_ty = (*((AstType**)List_ptr_get(bound_ty->args, gi)));
                                    }
#line 4063 "src/sema.tr"
                                    gi = (gi + 1LL);
                                }
                            }
#line 4064 "src/sema.tr"
                            AstType** _trret230 = box_asttype(found_ty);
                            return _trret230;
                        }
#line 4065 "src/sema.tr"
                        mi = (mi + 1LL);
                    }
                }
#line 4066 "src/sema.tr"
                bi = (bi + 1LL);
            }
        }
#line 4067 "src/sema.tr"
        ci = (ci + 1LL);
    }
#line 4068 "src/sema.tr"
    return ((AstType**)(0LL));
}

__attribute__((hot)) long long Sema_fn_value_throws_kind(Sema* self, AstType* ty) {
#line 4079 "src/sema.tr"
    if (((!_tr_str_eqv((ty->name), (_tr_str_lit_len("def", 3LL)))) || (ty->args->len == 0LL))) {
#line 4079 "src/sema.tr"
        return (0LL - 1LL);
    }
#line 4080 "src/sema.tr"
    if ((*((AstType**)List_ptr_get(ty->args, (ty->args->len - 1LL))))->is_throws) {
#line 4080 "src/sema.tr"
        return 1LL;
    }
#line 4081 "src/sema.tr"
    return 0LL;
}

__attribute__((hot)) void Sema_check_fn_value_throws(Sema* self, AstType** expected_p, AstType* actual, TrStr what) {
#line 4084 "src/sema.tr"
    if ((((unsigned long long)(expected_p)) == ((unsigned long long)(0LL)))) {
#line 4084 "src/sema.tr"
        return;
    }
#line 4085 "src/sema.tr"
    AstType* expected = (*expected_p);
#line 4089 "src/sema.tr"
    if (((_tr_str_eqv((expected->name), (_tr_str_lit_len("lambda", 6LL))) && _tr_str_eqv((actual->name), (_tr_str_lit_len("def", 3LL)))) && (actual->args->len > 0LL))) {
#line 4090 "src/sema.tr"
        AstType* lam_ret = (*((AstType**)List_ptr_get(actual->args, (actual->args->len - 1LL))));
#line 4091 "src/sema.tr"
        if (((((!_tr_str_eqv((lam_ret->name), (_tr_str_lit_len("int", 3LL)))) && (!_tr_str_eqv((lam_ret->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((lam_ret->name), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((lam_ret->name), (_tr_str_lit_len("None", 4LL)))))) {
#line 4092 "src/sema.tr"
            ({ TrStr _at_t1300 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-5] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is a `lambda` slot, which is called as returning `int`, but the function/closure passed returns '", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lam_ret->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: give the parameter a full function type (`def(...) -> ", 68LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lam_ret->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("`) so the result type is known.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1300); _tr_str_release(_at_t1300); });
#line 4093 "src/sema.tr"
            return;
        }
    }
#line 4094 "src/sema.tr"
    long long ek = Sema_fn_value_throws_kind(self, expected);
#line 4095 "src/sema.tr"
    long long ak = Sema_fn_value_throws_kind(self, actual);
#line 4096 "src/sema.tr"
    if ((((ek < 0LL) || (ak < 0LL)) || (ek == ak))) {
#line 4096 "src/sema.tr"
        return;
    }
#line 4097 "src/sema.tr"
    if ((ek == 1LL)) {
#line 4098 "src/sema.tr"
        ({ TrStr _at_t1301 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-4] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" expects a `throws` function value (`def(...) throws E -> R`) but a plain function/closure was passed. The two have different return types (a throws function returns a Result), so they cannot be exchanged.\n      FIX: declare the argument `throws` too (`def (...) throws E -> R:` / `def f(...) throws E -> R`), or change the parameter type to a plain `def(...) -> R`.", 366LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1301); _tr_str_release(_at_t1301); });
    } else {
#line 4100 "src/sema.tr"
        ({ TrStr _at_t1302 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-4] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" expects a plain function value (`def(...) -> R`) but a `throws` function/closure was passed. The two have different return types (a throws function returns a Result), so they cannot be exchanged.\n      FIX: handle the error inside a plain wrapper (`def (...) -> R:` that checks `.is_err`), or change the parameter type to `def(...) throws E -> R`.", 348LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1302); _tr_str_release(_at_t1302); });
    }
}

__attribute__((hot)) void Sema_check_call_fn_args(Sema* self, TrStr fname, List_ptr* hargs) {
#line 4104 "src/sema.tr"
    if ((!_tr_dict_contains(self->fn_defs, _tr_strz(fname)))) {
#line 4104 "src/sema.tr"
        return;
    }
#line 4105 "src/sema.tr"
    FunctionDef* fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(fname)));
#line 4106 "src/sema.tr"
    if ((fd->generics->len > 0LL)) {
#line 4106 "src/sema.tr"
        return;
    }
#line 4107 "src/sema.tr"
    long long sk = 0LL;
#line 4108 "src/sema.tr"
    if (((fd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(fd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4108 "src/sema.tr"
        sk = 1LL;
    }
#line 4109 "src/sema.tr"
    long long pi = sk;
#line 4110 "src/sema.tr"
    while ((pi < fd->params->len)) {
#line 4111 "src/sema.tr"
        long long aidx = (pi - sk);
#line 4112 "src/sema.tr"
        if ((aidx < hargs->len)) {
#line 4113 "src/sema.tr"
            ({ __auto_type _ord_t1303 = (((Param*)List_ptr_get(fd->params, pi))->ty); __auto_type _ord_t1304 = (hir_expr_type(((HirExpr*)List_ptr_get(hargs, aidx)))); TrStr _ord_t1305 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((aidx + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_fn_value_throws(self, _ord_t1303, _ord_t1304, _ord_t1305); _tr_str_release(_ord_t1305); });
        }
#line 4114 "src/sema.tr"
        pi = (pi + 1LL);
    }
}

__attribute__((hot)) AstType* Sema_closure_return_type(Sema* self, HirBlock* block) {
#line 4118 "src/sema.tr"
    AstType* nothing = AstType_init(_tr_str_lit_len("", 0LL));
#line 4119 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(block)))) {
#line 4119 "src/sema.tr"
        return nothing;
    }
#line 4120 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(block->stmts)))) {
#line 4120 "src/sema.tr"
        return nothing;
    }
#line 4121 "src/sema.tr"
    long long i = 0LL;
#line 4122 "src/sema.tr"
    while ((i < block->stmts->len)) {
#line 4123 "src/sema.tr"
        HirStmt* st = ((HirStmt*)List_ptr_get(block->stmts, i));
#line 4124 "src/sema.tr"
        if ((!_is_invalid_ptr(((unsigned long long)(st))))) {
#line 4125 "src/sema.tr"
            __auto_type _t1306 = (*st);
            if (_t1306.tag == HirStmt_SReturn) {
                __auto_type v = _t1306.data.SReturn.val;
#line 4127 "src/sema.tr"
                if ((!_is_invalid_ptr(((unsigned long long)(v))))) {
#line 4127 "src/sema.tr"
                    return hir_expr_type(v);
                }
            } else if (_t1306.tag == HirStmt_SIf) {
                __auto_type t = _t1306.data.SIf.then_b;
__auto_type e = _t1306.data.SIf.else_b;
#line 4129 "src/sema.tr"
                AstType* tt = Sema_closure_return_type(self, t);
#line 4130 "src/sema.tr"
                if ((!_tr_str_eqv((tt->name), (_tr_str_lit_len("", 0LL))))) {
#line 4130 "src/sema.tr"
                    return tt;
                }
#line 4131 "src/sema.tr"
                if ((!_is_invalid_ptr(((unsigned long long)(e))))) {
#line 4132 "src/sema.tr"
                    AstType* et = Sema_closure_return_type(self, e);
#line 4133 "src/sema.tr"
                    if ((!_tr_str_eqv((et->name), (_tr_str_lit_len("", 0LL))))) {
#line 4133 "src/sema.tr"
                        return et;
                    }
                }
            } else if (_t1306.tag == HirStmt_SWhile) {
                __auto_type wb = _t1306.data.SWhile.body;
#line 4135 "src/sema.tr"
                AstType* wt = Sema_closure_return_type(self, wb);
#line 4136 "src/sema.tr"
                if ((!_tr_str_eqv((wt->name), (_tr_str_lit_len("", 0LL))))) {
#line 4136 "src/sema.tr"
                    return wt;
                }
            } else if (_t1306.tag == HirStmt_SFor) {
                __auto_type fb = _t1306.data.SFor.body;
#line 4138 "src/sema.tr"
                AstType* ft = Sema_closure_return_type(self, fb);
#line 4139 "src/sema.tr"
                if ((!_tr_str_eqv((ft->name), (_tr_str_lit_len("", 0LL))))) {
#line 4139 "src/sema.tr"
                    return ft;
                }
            } else if (_t1306.tag == HirStmt_SUnsafe) {
                __auto_type ub = _t1306.data.SUnsafe.body;
#line 4141 "src/sema.tr"
                AstType* ut = Sema_closure_return_type(self, ub);
#line 4142 "src/sema.tr"
                if ((!_tr_str_eqv((ut->name), (_tr_str_lit_len("", 0LL))))) {
#line 4142 "src/sema.tr"
                    return ut;
                }
            } else if (1) {
                __auto_type _ = _t1306;
                /* pass */
            }
        }
#line 4144 "src/sema.tr"
        i = (i + 1LL);
    }
#line 4145 "src/sema.tr"
    return nothing;
}

__attribute__((hot)) void Sema_check_call_bounds(Sema* self, TrStr fname, List_ptr* hargs) {
#line 4148 "src/sema.tr"
    if ((!_tr_dict_contains(self->fn_defs, _tr_strz(fname)))) {
#line 4148 "src/sema.tr"
        return;
    }
#line 4149 "src/sema.tr"
    FunctionDef* fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(fname)));
#line 4150 "src/sema.tr"
    if ((fd->constraints->len == 0LL)) {
#line 4150 "src/sema.tr"
        return;
    }
#line 4151 "src/sema.tr"
    long long sk = 0LL;
#line 4152 "src/sema.tr"
    if (((fd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(fd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4152 "src/sema.tr"
        sk = 1LL;
    }
#line 4153 "src/sema.tr"
    long long ci = 0LL;
#line 4154 "src/sema.tr"
    while ((ci < fd->constraints->len)) {
#line 4155 "src/sema.tr"
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(fd->constraints, ci));
#line 4156 "src/sema.tr"
        long long pi = 0LL;
#line 4157 "src/sema.tr"
        while ((pi < fd->params->len)) {
#line 4158 "src/sema.tr"
            AstType** _pty = ((Param*)List_ptr_get(fd->params, pi))->ty;
#line 4159 "src/sema.tr"
            if (((((unsigned long long)(_pty)) != ((unsigned long long)(0LL))) && ({ __auto_type _ord_t1307 = ((*_pty)->name); __auto_type _wr = (_tr_str_eqv((_ord_t1307), (gc->target))); _wr; }))) {
#line 4160 "src/sema.tr"
                long long aidx = (pi - sk);
#line 4161 "src/sema.tr"
                if (((aidx >= 0LL) && (aidx < hargs->len))) {
#line 4162 "src/sema.tr"
                    TrStr concrete_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(hargs, aidx)))->name);
#line 4163 "src/sema.tr"
                    long long bi = 0LL;
#line 4164 "src/sema.tr"
                    while ((bi < gc->bounds->len)) {
#line 4165 "src/sema.tr"
                        TrStr iface_n = _tr_str_retain((*((AstType**)List_ptr_get(gc->bounds, bi)))->name);
#line 4166 "src/sema.tr"
                        if ((!Sema__type_satisfies_bound(self, concrete_n, iface_n))) {
#line 4167 "src/sema.tr"
                            ({ TrStr _at_t1308 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[G-1] Type '", 12LL)), (concrete_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' does not satisfy the bound '", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' required by type parameter '", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gc->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: implement all methods of interface '", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' on '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (concrete_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or add 'implements ", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1308); _tr_str_release(_at_t1308); });
                        }
#line 4168 "src/sema.tr"
                        bi = (bi + 1LL);
                        _tr_str_release(iface_n);
                    }
                }
            }
#line 4169 "src/sema.tr"
            pi = (pi + 1LL);
        }
#line 4170 "src/sema.tr"
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) AstType* Sema__subst_ret_generics(Sema* self, AstType* ty, List_TrStr* generics, List_ptr* concrete) {
#line 4177 "src/sema.tr"
    long long gi = 0LL;
#line 4178 "src/sema.tr"
    while ((gi < generics->len)) {
#line 4179 "src/sema.tr"
        if ((({ __auto_type _ord_t1309 = (List_TrStr_get(generics, gi)); __auto_type _wr = (({ TrStr _cmpa_t1310 = (_ord_t1309); TrStr _cmpb_t1311 = (ty->name); _Bool _cmpr_t1312 = _tr_str_eqv(_cmpa_t1310, _cmpb_t1311); _tr_str_release(_cmpa_t1310); _cmpr_t1312; })); _wr; }) && (gi < concrete->len))) {
#line 4180 "src/sema.tr"
            return (*((AstType**)List_ptr_get(concrete, gi)));
        }
#line 4181 "src/sema.tr"
        gi = (gi + 1LL);
    }
#line 4182 "src/sema.tr"
    if ((ty->args->len == 0LL)) {
#line 4182 "src/sema.tr"
        return ty;
    }
#line 4183 "src/sema.tr"
    AstType* nt = AstType_init(ty->name);
#line 4184 "src/sema.tr"
    nt->array_size = ty->array_size;
#line 4185 "src/sema.tr"
    nt->args = (void*)List_ptr_new();
#line 4186 "src/sema.tr"
    long long ai = 0LL;
#line 4187 "src/sema.tr"
    while ((ai < ty->args->len)) {
#line 4188 "src/sema.tr"
        ({ __auto_type _ord_t1313 = (box_asttype(Sema__subst_ret_generics(self, (*((AstType**)List_ptr_get(ty->args, ai))), generics, concrete))); List_ptr_append(nt->args, _ord_t1313); });
#line 4189 "src/sema.tr"
        ai = (ai + 1LL);
    }
#line 4190 "src/sema.tr"
    return nt;
}

__attribute__((hot)) void Sema_check_class_bounds(Sema* self, TrStr cls_name, List_ptr* arg_tys) {
#line 4196 "src/sema.tr"
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
#line 4196 "src/sema.tr"
        return;
    }
#line 4197 "src/sema.tr"
    ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
#line 4198 "src/sema.tr"
    if ((cd->constraints->len == 0LL)) {
#line 4198 "src/sema.tr"
        return;
    }
#line 4199 "src/sema.tr"
    long long ci = 0LL;
#line 4200 "src/sema.tr"
    while ((ci < cd->constraints->len)) {
#line 4201 "src/sema.tr"
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(cd->constraints, ci));
#line 4202 "src/sema.tr"
        long long gpi = 0LL;
#line 4203 "src/sema.tr"
        while ((gpi < cd->generics->len)) {
#line 4204 "src/sema.tr"
            if ((({ __auto_type _ord_t1314 = (List_TrStr_get(cd->generics, gpi)); __auto_type _wr = (({ TrStr _cmpa_t1315 = (_ord_t1314); TrStr _cmpb_t1316 = (gc->target); _Bool _cmpr_t1317 = _tr_str_eqv(_cmpa_t1315, _cmpb_t1316); _tr_str_release(_cmpa_t1315); _cmpr_t1317; })); _wr; }) && (gpi < arg_tys->len))) {
#line 4205 "src/sema.tr"
                TrStr concrete_n = _tr_str_retain((*((AstType**)List_ptr_get(arg_tys, gpi)))->name);
#line 4206 "src/sema.tr"
                long long bi = 0LL;
#line 4207 "src/sema.tr"
                while ((bi < gc->bounds->len)) {
#line 4208 "src/sema.tr"
                    TrStr iface_n = _tr_str_retain((*((AstType**)List_ptr_get(gc->bounds, bi)))->name);
#line 4209 "src/sema.tr"
                    if ((!Sema__type_satisfies_bound(self, concrete_n, iface_n))) {
#line 4210 "src/sema.tr"
                        ({ TrStr _at_t1318 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[G-1] Type '", 12LL)), (concrete_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' does not satisfy the bound '", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' required by type parameter '", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gc->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of class '", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cls_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: implement all methods of interface '", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' on '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (concrete_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or add 'implements ", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iface_n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1318); _tr_str_release(_at_t1318); });
                    }
#line 4211 "src/sema.tr"
                    bi = (bi + 1LL);
                    _tr_str_release(iface_n);
                }
            }
#line 4212 "src/sema.tr"
            gpi = (gpi + 1LL);
        }
#line 4213 "src/sema.tr"
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) void Sema__check_ty_arity(Sema* self, AstType** tp, TrStr where_, long long line) {
#line 4220 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(tp)))) {
#line 4220 "src/sema.tr"
        return;
    }
#line 4221 "src/sema.tr"
    AstType* t = (*tp);
#line 4222 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(t->args)))) {
#line 4222 "src/sema.tr"
        return;
    }
#line 4223 "src/sema.tr"
    if ((t->args->len == 0LL)) {
#line 4223 "src/sema.tr"
        return;
    }
#line 4224 "src/sema.tr"
    long long expected = (-1LL);
#line 4229 "src/sema.tr"
    if (((((_tr_str_eqv((t->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("Set", 3LL))))) {
#line 4230 "src/sema.tr"
        expected = (-1LL);
    } else if (_tr_dict_contains(self->classes, _tr_strz(t->name))) {
#line 4232 "src/sema.tr"
        expected = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(t->name)))->generics->len;
    } else if (_tr_dict_contains(self->enums, _tr_strz(t->name))) {
#line 4234 "src/sema.tr"
        expected = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(t->name)))->generics->len;
    }
#line 4235 "src/sema.tr"
    if (((expected > 0LL) && (t->args->len != expected))) {
#line 4236 "src/sema.tr"
        long long saved_line = self->current_line;
#line 4237 "src/sema.tr"
        if ((line > 0LL)) {
#line 4237 "src/sema.tr"
            self->current_line = line;
        }
#line 4238 "src/sema.tr"
        ({ __auto_type _ord_t1319 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[G-2] wrong number of type arguments for '", 42LL)), (t->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': expected ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1320 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1319); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(expected)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", found ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1321 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1320); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(t->args->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (in ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (where_)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").\n      FIX: write '", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1322 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1321); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' with exactly ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1323 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1322); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(expected)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" type argument(s).", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1323); _tr_str_release(_at_t1323); });
#line 4239 "src/sema.tr"
        self->current_line = saved_line;
    }
#line 4240 "src/sema.tr"
    long long ai = 0LL;
#line 4241 "src/sema.tr"
    while ((ai < t->args->len)) {
#line 4242 "src/sema.tr"
        Sema__check_ty_arity(self, ((AstType**)List_ptr_get(t->args, ai)), where_, line);
#line 4243 "src/sema.tr"
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) void Sema__check_fn_arities(Sema* self, FunctionDef* f, TrStr owner) {
#line 4246 "src/sema.tr"
    TrStr fn_where = _tr_strx_concatv((owner), (f->name));
#line 4247 "src/sema.tr"
    long long pi = 0LL;
#line 4248 "src/sema.tr"
    while ((pi < f->params->len)) {
#line 4249 "src/sema.tr"
        ({ __auto_type _ord_t1324 = (((Param*)List_ptr_get(f->params, pi))->ty); __auto_type _ord_t1325 = (f->line); Sema__check_ty_arity(self, _ord_t1324, fn_where, _ord_t1325); });
#line 4250 "src/sema.tr"
        pi = (pi + 1LL);
    }
#line 4251 "src/sema.tr"
    Sema__check_ty_arity(self, f->ret_ty, fn_where, f->line);
    _tr_str_release(fn_where);
}

__attribute__((hot)) void Sema_check_type_arities(Sema* self, Program* prog) {
#line 4254 "src/sema.tr"
    long long di = 0LL;
#line 4255 "src/sema.tr"
    while ((di < prog->decls->len)) {
#line 4256 "src/sema.tr"
        __auto_type _t1326 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t1326.tag == Decl_DFunction) {
            __auto_type f = _t1326.data.DFunction.func;
#line 4258 "src/sema.tr"
            Sema__check_fn_arities(self, f, _tr_str_lit_len("", 0LL));
        } else if (_t1326.tag == Decl_DClass) {
            __auto_type c = _t1326.data.DClass.cls;
#line 4260 "src/sema.tr"
            long long cfi = 0LL;
#line 4261 "src/sema.tr"
            while ((cfi < c->fields->len)) {
#line 4262 "src/sema.tr"
                ({ __auto_type _ord_t1327 = (((FieldDef*)List_ptr_get(c->fields, cfi))->ty); __auto_type _ord_t1328 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("field ", 6LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1329 = (({ TrStr _tr_cl = (_ord_t1328); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((FieldDef*)List_ptr_get(c->fields, cfi))->name)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1330 = (c->line); Sema__check_ty_arity(self, _ord_t1327, _ord_t1329, _ord_t1330); _tr_str_release(_ord_t1329); });
#line 4263 "src/sema.tr"
                cfi = (cfi + 1LL);
            }
#line 4264 "src/sema.tr"
            long long cmi = 0LL;
#line 4265 "src/sema.tr"
            while ((cmi < c->methods->len)) {
#line 4266 "src/sema.tr"
                ({ __auto_type _ord_t1331 = (((FunctionDef*)List_ptr_get(c->methods, cmi))); TrStr _ord_t1332 = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); Sema__check_fn_arities(self, _ord_t1331, _ord_t1332); _tr_str_release(_ord_t1332); });
#line 4267 "src/sema.tr"
                cmi = (cmi + 1LL);
            }
        } else if (_t1326.tag == Decl_DExtend) {
            __auto_type target = _t1326.data.DExtend.target;
__auto_type methods = _t1326.data.DExtend.methods;
#line 4269 "src/sema.tr"
            long long emi = 0LL;
#line 4270 "src/sema.tr"
            while ((emi < methods->len)) {
#line 4271 "src/sema.tr"
                ({ __auto_type _ord_t1334 = (((FunctionDef*)List_ptr_get(methods, emi))); TrStr _at_t1333 = (_tr_strx_concatv((target), (_tr_str_lit_len(".", 1LL)))); Sema__check_fn_arities(self, _ord_t1334, _at_t1333); _tr_str_release(_at_t1333); });
#line 4272 "src/sema.tr"
                emi = (emi + 1LL);
            }
        } else if (_t1326.tag == Decl_DEnum) {
            __auto_type e = _t1326.data.DEnum.enm;
#line 4274 "src/sema.tr"
            long long vi = 0LL;
#line 4275 "src/sema.tr"
            while ((vi < e->variants->len)) {
#line 4276 "src/sema.tr"
                long long vfi = 0LL;
#line 4277 "src/sema.tr"
                while ((vfi < ((VariantDef*)List_ptr_get(e->variants, vi))->fields->len)) {
#line 4278 "src/sema.tr"
                    ({ __auto_type _ord_t1335 = (((Param*)List_ptr_get(((VariantDef*)List_ptr_get(e->variants, vi))->fields, vfi))->ty); __auto_type _ord_t1336 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("enum ", 5LL)), (e->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1337 = (({ TrStr _tr_cl = (_ord_t1336); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((VariantDef*)List_ptr_get(e->variants, vi))->name)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1338 = (e->line); Sema__check_ty_arity(self, _ord_t1335, _ord_t1337, _ord_t1338); _tr_str_release(_ord_t1337); });
#line 4279 "src/sema.tr"
                    vfi = (vfi + 1LL);
                }
#line 4280 "src/sema.tr"
                vi = (vi + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t1326;
            /* pass */
        }
#line 4282 "src/sema.tr"
        di = (di + 1LL);
    }
}

__attribute__((hot)) void Sema_register_decl(Sema* self, Decl* d) {
#line 4285 "src/sema.tr"
    __auto_type _t1339 = (*d);
    if (_t1339.tag == Decl_DFunction) {
        __auto_type f = _t1339.data.DFunction.func;
#line 4287 "src/sema.tr"
        AstType** _f_ret = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 4288 "src/sema.tr"
        if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4288 "src/sema.tr"
            _f_ret = f->ret_ty;
        }
#line 4292 "src/sema.tr"
        if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4293 "src/sema.tr"
            AstType* _frt = (*f->ret_ty);
#line 4298 "src/sema.tr"
            if ((_frt->from_regions->len == 1LL)) {
#line 4299 "src/sema.tr"
                long long _fi = 0LL;
#line 4300 "src/sema.tr"
                while ((_fi < f->params->len)) {
#line 4301 "src/sema.tr"
                    if (({ __auto_type _ord_t1340 = (((Param*)List_ptr_get(f->params, _fi))->name); __auto_type _wr = (({ TrStr _cmpa_t1341 = (_ord_t1340); TrStr _cmpb_t1342 = (List_TrStr_get(_frt->from_regions, 0LL)); _Bool _cmpr_t1343 = _tr_str_eqv(_cmpa_t1341, _cmpb_t1342); _tr_str_release(_cmpb_t1342); _cmpr_t1343; })); _wr; })) {
#line 4302 "src/sema.tr"
                        _frt->from_index = _fi;
                    }
#line 4303 "src/sema.tr"
                    _fi = (_fi + 1LL);
                }
#line 4304 "src/sema.tr"
                _f_ret = box_asttype(_frt);
            }
        }
#line 4308 "src/sema.tr"
        AstType** _decl_ret = _f_ret;
#line 4309 "src/sema.tr"
        if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 4310 "src/sema.tr"
            if ((!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 4311 "src/sema.tr"
                AstType* _result_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 4312 "src/sema.tr"
                List_ptr_append(_result_ty->args, _f_ret);
#line 4313 "src/sema.tr"
                List_ptr_append(_result_ty->args, f->throws_ty);
#line 4314 "src/sema.tr"
                _decl_ret = box_asttype(_result_ty);
            }
        }
#line 4315 "src/sema.tr"
        Sema_declare(self, f->name, SymbolKind_make_SFunction(), _decl_ret, false);
#line 4319 "src/sema.tr"
        AstType* _fnty = AstType_init(_tr_str_lit_len("def", 3LL));
#line 4320 "src/sema.tr"
        long long _pi = 0LL;
#line 4321 "src/sema.tr"
        while ((_pi < f->params->len)) {
#line 4322 "src/sema.tr"
            ({ __auto_type _ord_t1344 = (((Param*)List_ptr_get(f->params, _pi))->ty); List_ptr_append(_fnty->args, _ord_t1344); });
#line 4323 "src/sema.tr"
            _pi = (_pi + 1LL);
        }
#line 4326 "src/sema.tr"
        if (((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 4327 "src/sema.tr"
            ({ __auto_type _ord_t1345 = (box_asttype(throws_result_type((*_f_ret), f->throws_ty))); List_ptr_append(_fnty->args, _ord_t1345); });
        } else {
#line 4329 "src/sema.tr"
            List_ptr_append(_fnty->args, _f_ret);
        }
#line 4330 "src/sema.tr"
        _tr_dict_set(self->fn_sigs, _tr_strz(f->name), _fnty);
#line 4331 "src/sema.tr"
        _tr_dict_set(self->fn_defs, _tr_strz(f->name), _tr_obj_retain(f));
#line 4334 "src/sema.tr"
        if ((f->params->len > 0LL)) {
#line 4335 "src/sema.tr"
            Param* _vp = ((Param*)List_ptr_get(f->params, (f->params->len - 1LL)));
#line 4336 "src/sema.tr"
            if (_vp->is_variadic) {
#line 4337 "src/sema.tr"
                AstType* _velem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 4338 "src/sema.tr"
                if ((((unsigned long long)(_vp->ty)) != ((unsigned long long)(0LL)))) {
#line 4338 "src/sema.tr"
                    _velem_ty = (*_vp->ty);
                }
#line 4339 "src/sema.tr"
                ({ __auto_type _ord_t1346 = (f->name); TrStr _ord_t1347 = (_tr_str_wrap(_tr_int_to_str((long long)((f->params->len - 1LL))))); _tr_dict_set(self->variadic_fns, _tr_strz(_ord_t1346), _tr_str_box(_tr_str_retain(_ord_t1347))); _tr_str_release(_ord_t1347); });
#line 4340 "src/sema.tr"
                ({ __auto_type _ord_t1348 = (f->name); __auto_type _ord_t1349 = (box_asttype(_velem_ty)); _tr_dict_set(self->variadic_elem_ty, _tr_strz(_ord_t1348), _ord_t1349); });
            }
        }
    } else if (_t1339.tag == Decl_DClass) {
        __auto_type c = _t1339.data.DClass.cls;
#line 4342 "src/sema.tr"
        ({ __auto_type _ord_t1350 = (c->name); __auto_type _ord_t1351 = (SymbolKind_make_SClass()); __auto_type _ord_t1352 = (box_asttype(AstType_init(c->name))); Sema_declare(self, _ord_t1350, _ord_t1351, _ord_t1352, false); });
#line 4343 "src/sema.tr"
        if ((_tr_dict_contains(self->classes, _tr_strz(c->name)) && (((unsigned long long)(((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(c->name))))) != ((unsigned long long)(c))))) {
#line 4343 "src/sema.tr"
            _tr_dict_set(self->ambiguous_classes, _tr_strz(c->name), true);
        }
#line 4344 "src/sema.tr"
        _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
#line 4346 "src/sema.tr"
        if (Sema_has_copy_decorator(self, c->decorators)) {
#line 4346 "src/sema.tr"
            _tr_dict_set(self->copy_classes, _tr_strz(c->name), true);
        }
    } else if (_t1339.tag == Decl_DActor) {
        __auto_type c = _t1339.data.DActor.cls;
#line 4348 "src/sema.tr"
        ({ __auto_type _ord_t1353 = (c->name); __auto_type _ord_t1354 = (SymbolKind_make_SClass()); __auto_type _ord_t1355 = (box_asttype(AstType_init(c->name))); Sema_declare(self, _ord_t1353, _ord_t1354, _ord_t1355, false); });
#line 4349 "src/sema.tr"
        _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
#line 4350 "src/sema.tr"
        if (Sema_has_copy_decorator(self, c->decorators)) {
#line 4350 "src/sema.tr"
            _tr_dict_set(self->copy_classes, _tr_strz(c->name), true);
        }
    } else if (_t1339.tag == Decl_DEnum) {
        __auto_type e = _t1339.data.DEnum.enm;
#line 4352 "src/sema.tr"
        ({ __auto_type _ord_t1356 = (e->name); __auto_type _ord_t1357 = (SymbolKind_make_SEnum()); __auto_type _ord_t1358 = (box_asttype(AstType_init(e->name))); Sema_declare(self, _ord_t1356, _ord_t1357, _ord_t1358, false); });
#line 4353 "src/sema.tr"
        _tr_dict_set(self->enums, _tr_strz(e->name), _tr_obj_retain(e));
#line 4354 "src/sema.tr"
        if (Sema_has_copy_decorator(self, e->decorators)) {
#line 4354 "src/sema.tr"
            _tr_dict_set(self->copy_classes, _tr_strz(e->name), true);
        }
    } else if (_t1339.tag == Decl_DInterface) {
        __auto_type i = _t1339.data.DInterface.iface;
#line 4356 "src/sema.tr"
        ({ __auto_type _ord_t1359 = (i->name); __auto_type _ord_t1360 = (SymbolKind_make_SInterface()); __auto_type _ord_t1361 = (box_asttype(AstType_init(i->name))); Sema_declare(self, _ord_t1359, _ord_t1360, _ord_t1361, false); });
#line 4357 "src/sema.tr"
        _tr_dict_set(self->interfaces, _tr_strz(i->name), _tr_obj_retain(i));
#line 4358 "src/sema.tr"
        if (Sema_has_copy_decorator(self, i->decorators)) {
#line 4358 "src/sema.tr"
            _tr_dict_set(self->copy_classes, _tr_strz(i->name), true);
        }
    } else if (_t1339.tag == Decl_DExtend) {
        __auto_type target = _t1339.data.DExtend.target;
__auto_type methods = _t1339.data.DExtend.methods;
#line 4360 "src/sema.tr"
        long long hi = 0LL;
#line 4361 "src/sema.tr"
        while ((hi < methods->len)) {
#line 4362 "src/sema.tr"
            FunctionDef* f = ((FunctionDef*)List_ptr_get(methods, hi));
#line 4363 "src/sema.tr"
            AstType** _m_ret = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 4364 "src/sema.tr"
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4364 "src/sema.tr"
                _m_ret = f->ret_ty;
            }
#line 4367 "src/sema.tr"
            TrStr _decl_key = ({ TrStr _tr_cl = (_tr_strx_concatv((target), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; });
#line 4368 "src/sema.tr"
            if (_tr_dict_contains(self->globals, _tr_strz(_decl_key))) {
#line 4370 "src/sema.tr"
                long long _pc = 0LL;
#line 4371 "src/sema.tr"
                long long _pci = 0LL;
#line 4372 "src/sema.tr"
                while ((_pci < f->params->len)) {
#line 4373 "src/sema.tr"
                    if ((!_tr_str_eqv((((Param*)List_ptr_get(f->params, _pci))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4373 "src/sema.tr"
                        _pc = (_pc + 1LL);
                    }
#line 4374 "src/sema.tr"
                    _pci = (_pci + 1LL);
                }
#line 4375 "src/sema.tr"
                ({ TrStr _ap_t1362 = (_tr_str_lit_len("_", 1LL)); TrStr _ap_t1363 = (_tr_str_wrap(_tr_int_to_str((long long)(_pc)))); TrStr _ap_t1364 = (_tr_str_lit_len("arg", 3LL)); _tr_str_append(&_decl_key, _ap_t1362); _tr_str_append(&_decl_key, _ap_t1363); _tr_str_append(&_decl_key, _ap_t1364); _tr_str_release(_ap_t1363); });
            }
#line 4376 "src/sema.tr"
            Sema_declare(self, _decl_key, SymbolKind_make_SFunction(), _m_ret, false);
#line 4377 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(target))) {
#line 4378 "src/sema.tr"
                List_ptr_append(((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(target)))->methods, _tr_obj_retain(f));
            }
#line 4379 "src/sema.tr"
            hi = (hi + 1LL);
            _tr_str_release(_decl_key);
        }
    } else if (_t1339.tag == Decl_DExtern) {
        __auto_type abi = _t1339.data.DExtern.abi;
__auto_type functions = _t1339.data.DExtern.functions;
#line 4381 "src/sema.tr"
        long long ei = 0LL;
#line 4382 "src/sema.tr"
        while ((ei < functions->len)) {
#line 4383 "src/sema.tr"
            FunctionDef* f = ((FunctionDef*)List_ptr_get(functions, ei));
#line 4384 "src/sema.tr"
            AstType** _e_ret = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 4385 "src/sema.tr"
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4385 "src/sema.tr"
                _e_ret = f->ret_ty;
            }
#line 4386 "src/sema.tr"
            Sema_declare(self, f->name, SymbolKind_make_SFunction(), _e_ret, false);
#line 4387 "src/sema.tr"
            _tr_dict_set(self->extern_names, _tr_strz(f->name), true);
#line 4388 "src/sema.tr"
            ei = (ei + 1LL);
        }
    } else if (_t1339.tag == Decl_DDecoratorDef) {
        __auto_type f = _t1339.data.DDecoratorDef.func;
#line 4390 "src/sema.tr"
        _tr_dict_set(self->decorator_names, _tr_strz(f->name), true);
#line 4391 "src/sema.tr"
        ({ __auto_type _ord_t1365 = (f->name); __auto_type _ord_t1366 = (SymbolKind_make_SFunction()); __auto_type _ord_t1367 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); Sema_declare(self, _ord_t1365, _ord_t1366, _ord_t1367, false); });
    } else if (_t1339.tag == Decl_DTypeAlias) {
        __auto_type alias_name = _t1339.data.DTypeAlias.name;
__auto_type target_ty = _t1339.data.DTypeAlias.target;
#line 4395 "src/sema.tr"
        AstType** alias_ty = box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)));
#line 4396 "src/sema.tr"
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
#line 4396 "src/sema.tr"
            alias_ty = target_ty;
        }
#line 4397 "src/sema.tr"
        Sema_declare(self, alias_name, SymbolKind_make_SClass(), alias_ty, false);
#line 4399 "src/sema.tr"
        TrStr resolved_name = _tr_str_lit_len("", 0LL);
#line 4400 "src/sema.tr"
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
#line 4400 "src/sema.tr"
            TrStr _strtmp_t1368 = _tr_str_retain((*target_ty)->name);
            _tr_str_release(resolved_name);
            resolved_name = _strtmp_t1368;
        }
#line 4401 "src/sema.tr"
        if ((_tr_str_lenv((resolved_name)) > 0LL)) {
#line 4402 "src/sema.tr"
            _tr_dict_set(self->type_aliases, _tr_strz(alias_name), _tr_str_box(_tr_str_retain(resolved_name)));
#line 4404 "src/sema.tr"
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
#line 4405 "src/sema.tr"
                AstType* ta_t = (*target_ty);
#line 4406 "src/sema.tr"
                if ((ta_t->args->len > 0LL)) {
#line 4407 "src/sema.tr"
                    ({ __auto_type _ord_t1369 = ((*((AstType**)List_ptr_get(ta_t->args, 0LL)))->name); _tr_dict_set(self->type_alias_elem, _tr_strz(alias_name), _tr_str_box(_tr_str_retain(_ord_t1369))); });
                }
            }
        }
        _tr_str_release(resolved_name);
    } else if (1) {
        __auto_type _ = _t1339;
        /* pass */
    }
}

__attribute__((hot)) HirFunction* Sema_lower_func(Sema* self, FunctionDef* f) {
#line 4411 "src/sema.tr"
    self->current_line = f->line;
#line 4412 "src/sema.tr"
    bool _saved_fn_is_lib = self->cur_fn_is_lib;
#line 4413 "src/sema.tr"
    self->cur_fn_is_lib = f->is_lib;
#line 4417 "src/sema.tr"
    if ((((!_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL)))) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("", 0LL))))) && (_tr_strlen(_tr_strz(f->name)) > 1LL))) {
#line 4418 "src/sema.tr"
        TrStr fn_cat = Sema_is_reserved_error(self, f->name);
#line 4419 "src/sema.tr"
        if ((!_tr_str_eqv((fn_cat), (_tr_str_lit_len("", 0LL))))) {
#line 4420 "src/sema.tr"
            ({ TrStr _at_t1370 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fn_cat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and cannot be used as a function name.", 39LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1370); _tr_str_release(_at_t1370); });
        } else if (_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))) {
#line 4422 "src/sema.tr"
            TrStr fn_kcat = Sema_is_reserved_keyword(self, f->name);
#line 4423 "src/sema.tr"
            if ((!_tr_str_eqv((fn_kcat), (_tr_str_lit_len("", 0LL))))) {
#line 4424 "src/sema.tr"
                ({ TrStr _at_t1371 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-1] '", 7LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fn_kcat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and is reserved. Choose a different function name.", 51LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1371); _tr_str_release(_at_t1371); });
            }
        }
    }
#line 4426 "src/sema.tr"
    bool saved_async = self->in_async_fn;
#line 4427 "src/sema.tr"
    self->in_async_fn = f->is_async;
#line 4428 "src/sema.tr"
    bool saved_try_ok = self->fn_try_ok;
#line 4429 "src/sema.tr"
    self->fn_try_ok = false;
#line 4430 "src/sema.tr"
    if (((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 4430 "src/sema.tr"
        self->fn_try_ok = true;
    }
#line 4431 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4432 "src/sema.tr"
        TrStr _rtn = _tr_str_retain((*f->ret_ty)->name);
#line 4433 "src/sema.tr"
        if ((_tr_str_eqv((_rtn), (_tr_str_lit_len("Result", 6LL))) || _tr_str_eqv((_rtn), (_tr_str_lit_len("Option", 6LL))))) {
#line 4433 "src/sema.tr"
            self->fn_try_ok = true;
        }
        _tr_str_release(_rtn);
    }
#line 4434 "src/sema.tr"
    self->current_func_name = _tr_str_retain(f->name);
#line 4435 "src/sema.tr"
    List_TrStr* saved_func_generics = self->current_func_generics;
#line 4436 "src/sema.tr"
    self->current_func_generics = f->generics;
#line 4437 "src/sema.tr"
    List_ptr* saved_func_constraints = self->current_func_constraints;
#line 4438 "src/sema.tr"
    self->current_func_constraints = f->constraints;
#line 4440 "src/sema.tr"
    self->container_borrows = _tr_dict_new(16LL);
#line 4442 "src/sema.tr"
    TrStr saved_ret_from = _tr_str_retain(self->current_func_ret_from);
#line 4443 "src/sema.tr"
    bool saved_ret_borrow_str = self->current_func_ret_borrow_str;
#line 4444 "src/sema.tr"
    TrStr saved_ret_family = _tr_str_retain(self->current_func_ret_family);
#line 4445 "src/sema.tr"
    self->current_func_ret_family = _tr_str_lit_len("", 0LL);
#line 4446 "src/sema.tr"
    if (((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))) && (!f->is_extern))) {
#line 4446 "src/sema.tr"
        ({ __auto_type _ord_t1373 = (f->ret_ty); TrStr _at_t1372 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the return type of '", 20LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_type_known(self, _ord_t1373, _at_t1372); _tr_str_release(_at_t1372); });
    }
#line 4447 "src/sema.tr"
    if (((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))) && (f->generics->len == 0LL))) {
#line 4448 "src/sema.tr"
        self->current_func_ret_family = Sema_decl_family(self, (*f->ret_ty));
    }
#line 4449 "src/sema.tr"
    List_TrStr* saved_ret_regions = self->current_func_ret_regions;
#line 4450 "src/sema.tr"
    List_TrStr* saved_outlives_a = self->current_func_outlives_a;
#line 4451 "src/sema.tr"
    List_TrStr* saved_outlives_b = self->current_func_outlives_b;
#line 4452 "src/sema.tr"
    self->current_func_ret_from = _tr_str_lit_len("", 0LL);
#line 4453 "src/sema.tr"
    self->current_func_ret_borrow_str = false;
#line 4454 "src/sema.tr"
    self->current_func_ret_regions = (void*)List_TrStr_new();
#line 4455 "src/sema.tr"
    self->current_func_outlives_a = f->outlives_a;
#line 4456 "src/sema.tr"
    self->current_func_outlives_b = f->outlives_b;
#line 4458 "src/sema.tr"
    if ((self->strict_mode && (f->outlives_a->len > 0LL))) {
#line 4459 "src/sema.tr"
        long long wbi = 0LL;
#line 4460 "src/sema.tr"
        while ((wbi < f->outlives_a->len)) {
#line 4461 "src/sema.tr"
            bool w_ok_a = false;
#line 4462 "src/sema.tr"
            bool w_ok_b = false;
#line 4463 "src/sema.tr"
            long long wpi = 0LL;
#line 4464 "src/sema.tr"
            while ((wpi < f->params->len)) {
#line 4465 "src/sema.tr"
                if (({ __auto_type _ord_t1374 = (((Param*)List_ptr_get(f->params, wpi))->name); __auto_type _wr = (({ TrStr _cmpa_t1375 = (_ord_t1374); TrStr _cmpb_t1376 = (List_TrStr_get(f->outlives_a, wbi)); _Bool _cmpr_t1377 = _tr_str_eqv(_cmpa_t1375, _cmpb_t1376); _tr_str_release(_cmpb_t1376); _cmpr_t1377; })); _wr; })) {
#line 4465 "src/sema.tr"
                    w_ok_a = true;
                }
#line 4466 "src/sema.tr"
                if (({ __auto_type _ord_t1378 = (((Param*)List_ptr_get(f->params, wpi))->name); __auto_type _wr = (({ TrStr _cmpa_t1379 = (_ord_t1378); TrStr _cmpb_t1380 = (List_TrStr_get(f->outlives_b, wbi)); _Bool _cmpr_t1381 = _tr_str_eqv(_cmpa_t1379, _cmpb_t1380); _tr_str_release(_cmpb_t1380); _cmpr_t1381; })); _wr; })) {
#line 4466 "src/sema.tr"
                    w_ok_b = true;
                }
#line 4467 "src/sema.tr"
                wpi = (wpi + 1LL);
            }
#line 4468 "src/sema.tr"
            if ((!w_ok_a)) {
#line 4469 "src/sema.tr"
                ({ TrStr _at_t1382 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(f->outlives_a, wbi)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[L-2] region '", 14LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in the `where ... outlives ...` clause is not a parameter of this function.", 77LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1382); _tr_str_release(_at_t1382); });
            }
#line 4470 "src/sema.tr"
            if ((!w_ok_b)) {
#line 4471 "src/sema.tr"
                ({ TrStr _at_t1383 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(f->outlives_b, wbi)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[L-2] region '", 14LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in the `where ... outlives ...` clause is not a parameter of this function.", 77LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1383); _tr_str_release(_at_t1383); });
            }
#line 4472 "src/sema.tr"
            wbi = (wbi + 1LL);
        }
    }
#line 4473 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4474 "src/sema.tr"
        self->current_func_ret_from = _tr_str_retain((*f->ret_ty)->from_param);
#line 4475 "src/sema.tr"
        self->current_func_ret_regions = (*f->ret_ty)->from_regions;
#line 4476 "src/sema.tr"
        if (((*f->ret_ty)->is_borrow && _tr_str_eqv(((*f->ret_ty)->name), (_tr_str_lit_len("str", 3LL))))) {
#line 4477 "src/sema.tr"
            self->current_func_ret_borrow_str = true;
        }
    }
#line 4481 "src/sema.tr"
    if ((_tr_str_eqv((self->current_func_ret_from), (_tr_str_lit_len("", 0LL))) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
#line 4482 "src/sema.tr"
        TrStr infer_ret_nm = _tr_str_retain((*f->ret_ty)->name);
#line 4483 "src/sema.tr"
        bool infer_is_borrow = (*f->ret_ty)->is_borrow;
#line 4484 "src/sema.tr"
        if ((((_tr_str_eqv((infer_ret_nm), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((infer_ret_nm), (_tr_str_lit_len("ref", 3LL)))) || _tr_str_eqv((infer_ret_nm), (_tr_str_lit_len("mut_ref", 7LL)))) || infer_is_borrow)) {
#line 4485 "src/sema.tr"
            TrStr infer_from = _tr_str_lit_len("", 0LL);
#line 4486 "src/sema.tr"
            long long infer_count = 0LL;
#line 4487 "src/sema.tr"
            long long infer_i = 0LL;
#line 4488 "src/sema.tr"
            while ((infer_i < f->params->len)) {
#line 4489 "src/sema.tr"
                Param* infer_p = ((Param*)List_ptr_get(f->params, infer_i));
#line 4490 "src/sema.tr"
                if (((!_tr_str_eqv((infer_p->name), (_tr_str_lit_len("self", 4LL)))) && (((unsigned long long)(infer_p->ty)) != ((unsigned long long)(0LL))))) {
#line 4491 "src/sema.tr"
                    AstType* infer_pty = (*infer_p->ty);
#line 4492 "src/sema.tr"
                    if ((!Sema_is_primitive_name(self, infer_pty->name))) {
#line 4493 "src/sema.tr"
                        TrStr _strtmp_t1384 = _tr_str_retain(infer_p->name);
                        _tr_str_release(infer_from);
                        infer_from = _strtmp_t1384;
#line 4494 "src/sema.tr"
                        infer_count = (infer_count + 1LL);
                    }
                }
#line 4495 "src/sema.tr"
                infer_i = (infer_i + 1LL);
            }
#line 4496 "src/sema.tr"
            if ((infer_count == 1LL)) {
#line 4497 "src/sema.tr"
                self->current_func_ret_from = _tr_str_retain(infer_from);
            }
        }
        _tr_str_release(infer_ret_nm);
    }
#line 4502 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4503 "src/sema.tr"
        List_TrStr* rregs = (*f->ret_ty)->from_regions;
#line 4504 "src/sema.tr"
        long long rri = 0LL;
#line 4505 "src/sema.tr"
        while ((rri < rregs->len)) {
#line 4506 "src/sema.tr"
            TrStr rnm = List_TrStr_get(rregs, rri);
#line 4507 "src/sema.tr"
            bool rok = false;
#line 4508 "src/sema.tr"
            long long rpi = 0LL;
#line 4509 "src/sema.tr"
            while ((rpi < f->params->len)) {
#line 4510 "src/sema.tr"
                if (_tr_str_eqv((((Param*)List_ptr_get(f->params, rpi))->name), (rnm))) {
#line 4511 "src/sema.tr"
                    rok = true;
                }
#line 4512 "src/sema.tr"
                rpi = (rpi + 1LL);
            }
#line 4513 "src/sema.tr"
            if ((!rok)) {
#line 4515 "src/sema.tr"
                long long rci = 0LL;
#line 4516 "src/sema.tr"
                while ((rci < self->current_region_params->len)) {
#line 4517 "src/sema.tr"
                    if (({ TrStr _cmpa_t1385 = (List_TrStr_get(self->current_region_params, rci)); TrStr _cmpb_t1386 = (rnm); _Bool _cmpr_t1387 = _tr_str_eqv(_cmpa_t1385, _cmpb_t1386); _tr_str_release(_cmpa_t1385); _cmpr_t1387; })) {
#line 4518 "src/sema.tr"
                        rok = true;
                    }
#line 4519 "src/sema.tr"
                    rci = (rci + 1LL);
                }
            }
#line 4520 "src/sema.tr"
            if ((!rok)) {
#line 4521 "src/sema.tr"
                ({ TrStr _at_t1388 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-2] region source '", 21LL)), (rnm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' named in 'from' is not a parameter or a region parameter of the enclosing type.\n      FIX: name a parameter the borrow comes from, or declare 'class/enum/interface <T> from ", 175LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rnm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":'.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1388); _tr_str_release(_at_t1388); });
            }
#line 4522 "src/sema.tr"
            rri = (rri + 1LL);
            _tr_str_release(rnm);
        }
    }
#line 4523 "src/sema.tr"
    Sema_enter_scope(self);
#line 4525 "src/sema.tr"
    List_ptr* hparams = (void*)List_ptr_new();
#line 4526 "src/sema.tr"
    long long j = 0LL;
#line 4527 "src/sema.tr"
    while ((j < f->params->len)) {
#line 4528 "src/sema.tr"
        Param* p = ((Param*)List_ptr_get(f->params, j));
#line 4529 "src/sema.tr"
        AstType* p_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 4530 "src/sema.tr"
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
#line 4531 "src/sema.tr"
            p_ty = (*p->ty);
#line 4532 "src/sema.tr"
            if ((!f->is_extern)) {
#line 4532 "src/sema.tr"
                ({ __auto_type _ord_t1390 = (p->ty); TrStr _at_t1389 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("parameter '", 11LL)), (p->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_type_known(self, _ord_t1390, _at_t1389); _tr_str_release(_at_t1389); });
            }
        } else if ((_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))))) {
#line 4534 "src/sema.tr"
            p_ty = AstType_init(self->current_class_name);
        }
#line 4535 "src/sema.tr"
        if (p->is_variadic) {
#line 4538 "src/sema.tr"
            AstType* _vp_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 4539 "src/sema.tr"
            ({ __auto_type _ord_t1391 = (box_asttype(p_ty)); List_ptr_append(_vp_ty->args, _ord_t1391); });
#line 4540 "src/sema.tr"
            p_ty = _vp_ty;
        }
#line 4542 "src/sema.tr"
        ({ __auto_type _ord_t1392 = (p->name); __auto_type _ord_t1393 = (SymbolKind_make_SVariable()); __auto_type _ord_t1394 = (box_asttype(p_ty)); Sema_declare(self, _ord_t1392, _ord_t1393, _ord_t1394, false); });
#line 4544 "src/sema.tr"
        if ((self->scopes->len > 0LL)) {
#line 4545 "src/sema.tr"
            Scope* pb_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 4546 "src/sema.tr"
            if (_tr_dict_contains(pb_scope->variables, _tr_strz(p->name))) {
#line 4547 "src/sema.tr"
                Symbol* pb_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pb_scope->variables, _tr_strz(p->name)));
#line 4548 "src/sema.tr"
                pb_sym->is_param = true;
#line 4549 "src/sema.tr"
                pb_sym->ptr_region = 2LL;
#line 4550 "src/sema.tr"
                if ((((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL))) && (!p->is_variadic)) && (f->generics->len == 0LL))) {
#line 4551 "src/sema.tr"
                    pb_sym->firm = Sema_decl_family(self, p_ty);
                }
#line 4552 "src/sema.tr"
                _tr_dict_set(pb_scope->variables, _tr_strz(p->name), pb_sym);
            }
        }
#line 4554 "src/sema.tr"
        HirParam* hp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 4555 "src/sema.tr"
        hp->name = _tr_str_retain(p->name);
#line 4556 "src/sema.tr"
        hp->ty = p_ty;
#line 4557 "src/sema.tr"
        List_ptr_append(hparams, _tr_obj_retain(hp));
#line 4558 "src/sema.tr"
        j = (j + 1LL);
        _tr_obj_release(hp, _trdrop_HirParam);
    }
#line 4560 "src/sema.tr"
    HirFunction* hf = ((HirFunction*)_tr_obj_alloc(sizeof(HirFunction)));
#line 4561 "src/sema.tr"
    hf->name = _tr_str_retain(f->name);
#line 4562 "src/sema.tr"
    hf->class_name = _tr_str_retain(self->current_class_name);
#line 4563 "src/sema.tr"
    hf->generics = f->generics;
#line 4564 "src/sema.tr"
    hf->constraints = f->constraints;
#line 4565 "src/sema.tr"
    hf->params = hparams;
#line 4566 "src/sema.tr"
    hf->ret_ty = AstType_init(_tr_str_lit_len("None", 4LL));
#line 4567 "src/sema.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4567 "src/sema.tr"
        hf->ret_ty = (*f->ret_ty);
    }
#line 4568 "src/sema.tr"
    hf->throws_ty = AstType_init(_tr_str_lit_len("", 0LL));
#line 4569 "src/sema.tr"
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 4569 "src/sema.tr"
        hf->throws_ty = (*f->throws_ty);
    }
#line 4571 "src/sema.tr"
    self->cur_func_borrowers = (void*)List_TrStr_new();
#line 4572 "src/sema.tr"
    self->cur_func_sources = (void*)List_TrStr_new();
#line 4573 "src/sema.tr"
    TrMap* saved_alias_assigned = self->alias_assigned;
#line 4574 "src/sema.tr"
    self->alias_assigned = _tr_dict_new(8LL);
#line 4575 "src/sema.tr"
    _block_alias_assigns(f->body, self->alias_assigned);
#line 4576 "src/sema.tr"
    HirBlock* _cltmp_t1395 = Sema_lower_block(self, f->body);
    _tr_obj_release(hf->body, _trdrop_HirBlock);
    hf->body = _cltmp_t1395;
#line 4577 "src/sema.tr"
    self->alias_assigned = saved_alias_assigned;
#line 4578 "src/sema.tr"
    hf->borrow_borrowers = self->cur_func_borrowers;
#line 4579 "src/sema.tr"
    hf->borrow_sources = self->cur_func_sources;
#line 4581 "src/sema.tr"
    hf->proven_borrows = mir_proven_borrows(hf);
#line 4585 "src/sema.tr"
    if (self->strict_mode) {
#line 4586 "src/sema.tr"
        List_TrStr* _bconf = mir_borrow_conflicts(hf, self->mutating_methods);
#line 4587 "src/sema.tr"
        long long _bci = 0LL;
#line 4588 "src/sema.tr"
        while ((_bci < _bconf->len)) {
#line 4589 "src/sema.tr"
            ({ TrStr _at_t1396 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(_bconf, _bci)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[B-1] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      A place may have MANY shared `ref` borrows, OR exactly ONE exclusive `mut` borrow — never both. End one borrow's last use before the other begins.", 156LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1396); _tr_str_release(_at_t1396); });
#line 4590 "src/sema.tr"
            _bci = (_bci + 1LL);
        }
#line 4592 "src/sema.tr"
        List_TrStr* _b3 = mir_shared_ref_param_violations(hf, self->mutating_methods);
#line 4593 "src/sema.tr"
        long long _b3i = 0LL;
#line 4594 "src/sema.tr"
        while ((_b3i < _b3->len)) {
#line 4595 "src/sema.tr"
            ({ TrStr _at_t1397 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(_b3, _b3i)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[B-3] ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      A shared `ref T` parameter cannot be mutated — declare it `mut ref T` for an exclusive (mutable) borrow.", 114LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1397); _tr_str_release(_at_t1397); });
#line 4596 "src/sema.tr"
            _b3i = (_b3i + 1LL);
        }
        List_TrStr_free(_bconf);
    }
#line 4597 "src/sema.tr"
    hf->decorators = f->decorators;
#line 4598 "src/sema.tr"
    hf->is_async = f->is_async;
#line 4599 "src/sema.tr"
    hf->is_extern = f->is_extern;
#line 4600 "src/sema.tr"
    hf->is_public = f->is_public;
#line 4601 "src/sema.tr"
    hf->is_export = f->is_export;
#line 4602 "src/sema.tr"
    bool _has_self = false;
#line 4603 "src/sema.tr"
    long long _si = 0LL;
#line 4604 "src/sema.tr"
    while ((_si < hparams->len)) {
#line 4605 "src/sema.tr"
        if (_tr_str_eqv((((HirParam*)List_ptr_get(hparams, _si))->name), (_tr_str_lit_len("self", 4LL)))) {
#line 4605 "src/sema.tr"
            _has_self = true;
        }
#line 4606 "src/sema.tr"
        _si = (_si + 1LL);
    }
#line 4607 "src/sema.tr"
    hf->is_static = ((!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))) && (!_has_self));
#line 4608 "src/sema.tr"
    hf->is_variadic = f->is_variadic;
#line 4609 "src/sema.tr"
    hf->is_decorator = false;
#line 4616 "src/sema.tr"
    bool _is_entry_main = (_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL))) && _tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))));
#line 4617 "src/sema.tr"
    if ((((!f->is_extern) && (f->body->stmts->len > 0LL)) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
#line 4618 "src/sema.tr"
        TrStr ret_nm = _tr_str_retain((*f->ret_ty)->name);
#line 4619 "src/sema.tr"
        if (((((((!_tr_str_eqv((ret_nm), (_tr_str_lit_len("void", 4LL)))) && (!_tr_str_eqv((ret_nm), (_tr_str_lit_len("None", 4LL))))) && (!_tr_str_eqv((ret_nm), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("init", 4LL))))) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("new", 3LL))))) && (!_is_entry_main))) {
#line 4620 "src/sema.tr"
            if ((!Sema_block_returns(self, f->body))) {
#line 4621 "src/sema.tr"
                self->current_line = f->line;
#line 4622 "src/sema.tr"
                ({ TrStr _at_t1398 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-3] Function '", 16LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' returns '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ret_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but is missing a return statement on at least one code path. FIX: Add a return at the end, or ensure all if/elif/else branches return.", 136LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1398); _tr_str_release(_at_t1398); });
            }
        }
    }
#line 4624 "src/sema.tr"
    Sema_finalize_scope_drops(self, hf->body);
#line 4629 "src/sema.tr"
    Sema_apply_mir_if_drops(self, hf);
#line 4630 "src/sema.tr"
    Sema_exit_scope(self);
#line 4632 "src/sema.tr"
    self->in_async_fn = saved_async;
#line 4633 "src/sema.tr"
    self->fn_try_ok = saved_try_ok;
#line 4634 "src/sema.tr"
    self->current_func_name = _tr_str_lit_len("", 0LL);
#line 4635 "src/sema.tr"
    self->current_func_generics = saved_func_generics;
#line 4636 "src/sema.tr"
    self->current_func_constraints = saved_func_constraints;
#line 4637 "src/sema.tr"
    self->current_func_ret_from = _tr_str_retain(saved_ret_from);
#line 4638 "src/sema.tr"
    self->current_func_ret_borrow_str = saved_ret_borrow_str;
#line 4639 "src/sema.tr"
    self->current_func_ret_family = _tr_str_retain(saved_ret_family);
#line 4640 "src/sema.tr"
    self->current_func_ret_regions = saved_ret_regions;
#line 4641 "src/sema.tr"
    self->current_func_outlives_a = saved_outlives_a;
#line 4642 "src/sema.tr"
    self->current_func_outlives_b = saved_outlives_b;
#line 4643 "src/sema.tr"
    self->cur_fn_is_lib = _saved_fn_is_lib;
#line 4644 "src/sema.tr"
    _tr_str_release(saved_ret_from);
    _tr_str_release(saved_ret_family);
    return hf;
}

__attribute__((hot)) HirClass* Sema_lower_class(Sema* self, ClassDef* c) {
#line 4647 "src/sema.tr"
    self->current_line = c->line;
#line 4648 "src/sema.tr"
    self->current_class_name = _tr_str_retain(c->name);
#line 4649 "src/sema.tr"
    self->current_region_params = c->region_params;
#line 4650 "src/sema.tr"
    List_ptr* hfields = (void*)List_ptr_new();
#line 4651 "src/sema.tr"
    long long i = 0LL;
#line 4652 "src/sema.tr"
    while ((i < c->fields->len)) {
#line 4653 "src/sema.tr"
        FieldDef* f = ((FieldDef*)List_ptr_get(c->fields, i));
#line 4654 "src/sema.tr"
        HirField* hf = ((HirField*)_tr_obj_alloc(sizeof(HirField)));
#line 4655 "src/sema.tr"
        hf->name = _tr_str_retain(f->name);
#line 4656 "src/sema.tr"
        AstType* f_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 4657 "src/sema.tr"
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
#line 4657 "src/sema.tr"
            f_ty = (*f->ty);
        }
#line 4658 "src/sema.tr"
        hf->ty = f_ty;
#line 4659 "src/sema.tr"
        List_ptr_append(hfields, _tr_obj_retain(hf));
#line 4660 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hf, _trdrop_HirField);
    }
#line 4663 "src/sema.tr"
    long long pf_i = 0LL;
#line 4664 "src/sema.tr"
    while ((pf_i < c->fields->len)) {
#line 4665 "src/sema.tr"
        FieldDef* pf_f = ((FieldDef*)List_ptr_get(c->fields, pf_i));
#line 4666 "src/sema.tr"
        if ((((unsigned long long)(pf_f->ty)) != ((unsigned long long)(0LL)))) {
#line 4667 "src/sema.tr"
            AstType* pf_ty = (*pf_f->ty);
#line 4668 "src/sema.tr"
            if ((_tr_str_eqv((pf_ty->name), (_tr_str_lit_len("Shared", 6LL))) && (pf_ty->args->len > 0LL))) {
#line 4669 "src/sema.tr"
                AstType* pf_inner = (*((AstType**)List_ptr_get(pf_ty->args, 0LL)));
#line 4670 "src/sema.tr"
                if (_tr_str_eqv((pf_inner->name), (c->name))) {
#line 4671 "src/sema.tr"
                    ({ TrStr _at_t1399 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[S-1] '", 7LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has a 'Shared[", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' field '", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pf_f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' - this creates a reference cycle that leaks memory.\n      FIX: Use 'Weak[", 75LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' for back-references to break the cycle.", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1399); _tr_str_release(_at_t1399); });
                }
            }
        }
#line 4672 "src/sema.tr"
        pf_i = (pf_i + 1LL);
    }
#line 4674 "src/sema.tr"
    List_ptr* hmethods = (void*)List_ptr_new();
#line 4675 "src/sema.tr"
    i = 0LL;
#line 4676 "src/sema.tr"
    while ((i < c->methods->len)) {
#line 4677 "src/sema.tr"
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(c->methods, i)));
#line 4678 "src/sema.tr"
        List_ptr_append(hmethods, _tr_obj_retain(hm));
#line 4679 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
#line 4682 "src/sema.tr"
    long long _ifc_i = 0LL;
#line 4683 "src/sema.tr"
    while ((_ifc_i < c->iface_names->len)) {
#line 4684 "src/sema.tr"
        TrStr _ifc_nm = List_TrStr_get(c->iface_names, _ifc_i);
#line 4685 "src/sema.tr"
        if (_tr_str_eqv((_ifc_nm), (_tr_str_lit_len("Sendable", 8LL)))) {
#line 4689 "src/sema.tr"
            self->sendable_tparams = c->generics;
#line 4690 "src/sema.tr"
            Sema_check_class_sendable_fields(self, c);
#line 4691 "src/sema.tr"
            self->sendable_tparams = (void*)List_TrStr_new();
        } else if (_tr_str_eqv((_ifc_nm), (_tr_str_lit_len("UnsafeSendable", 14LL)))) {
#line 4696 "src/sema.tr"
            /* pass */
        } else if ((!_tr_dict_contains(self->interfaces, _tr_strz(_ifc_nm)))) {
#line 4698 "src/sema.tr"
            ({ TrStr _at_t1400 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-1] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares 'implements ", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but interface '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not defined.\n      FIX: Define 'interface ", 47LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":' before this class, or check for typos.", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1400); _tr_str_release(_at_t1400); });
        } else {
#line 4700 "src/sema.tr"
            InterfaceDef* _idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(_ifc_nm)));
#line 4701 "src/sema.tr"
            long long _im_i = 0LL;
#line 4702 "src/sema.tr"
            while ((_im_i < _idef->methods->len)) {
#line 4703 "src/sema.tr"
                FunctionDef* _imeth = ((FunctionDef*)List_ptr_get(_idef->methods, _im_i));
#line 4704 "src/sema.tr"
                bool _found = false;
#line 4705 "src/sema.tr"
                long long _cm_i = 0LL;
#line 4706 "src/sema.tr"
                while ((_cm_i < c->methods->len)) {
#line 4707 "src/sema.tr"
                    FunctionDef* _cmeth = ((FunctionDef*)List_ptr_get(c->methods, _cm_i));
#line 4708 "src/sema.tr"
                    if (_tr_str_eqv((_cmeth->name), (_imeth->name))) {
#line 4709 "src/sema.tr"
                        _found = true;
#line 4711 "src/sema.tr"
                        TrStr _iret = _tr_str_lit_len("void", 4LL);
#line 4712 "src/sema.tr"
                        if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4713 "src/sema.tr"
                            TrStr _strtmp_t1401 = _tr_str_retain((*_imeth->ret_ty)->name);
                            _tr_str_release(_iret);
                            _iret = _strtmp_t1401;
                        }
#line 4714 "src/sema.tr"
                        TrStr _cret = _tr_str_lit_len("void", 4LL);
#line 4715 "src/sema.tr"
                        if ((((unsigned long long)(_cmeth->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4716 "src/sema.tr"
                            TrStr _strtmp_t1402 = _tr_str_retain((*_cmeth->ret_ty)->name);
                            _tr_str_release(_cret);
                            _cret = _strtmp_t1402;
                        }
#line 4718 "src/sema.tr"
                        bool _iret_is_generic = ((((_ifc_i >= 0LL) && (_idef->generics->len > 0LL)) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("", 0LL)))));
#line 4719 "src/sema.tr"
                        long long _gi = 0LL;
#line 4720 "src/sema.tr"
                        while ((_gi < _idef->generics->len)) {
#line 4721 "src/sema.tr"
                            if (({ TrStr _cmpa_t1403 = (List_TrStr_get(_idef->generics, _gi)); TrStr _cmpb_t1404 = (_iret); _Bool _cmpr_t1405 = _tr_str_eqv(_cmpa_t1403, _cmpb_t1404); _tr_str_release(_cmpa_t1403); _cmpr_t1405; })) {
#line 4721 "src/sema.tr"
                                _iret_is_generic = true;
                            }
#line 4722 "src/sema.tr"
                            _gi = (_gi + 1LL);
                        }
#line 4723 "src/sema.tr"
                        if (((((!_iret_is_generic) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((_iret), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((_cret), (_iret))))) {
#line 4724 "src/sema.tr"
                            ({ TrStr _at_t1406 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-3] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': method '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_cmeth->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' returns '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_cret)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but interface '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares '-> ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_iret)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: Change return type to '-> ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_iret)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1406); _tr_str_release(_at_t1406); });
                        }
#line 4726 "src/sema.tr"
                        long long _ip_cnt = 0LL;
#line 4727 "src/sema.tr"
                        long long _ip_i = 0LL;
#line 4728 "src/sema.tr"
                        while ((_ip_i < _imeth->params->len)) {
#line 4729 "src/sema.tr"
                            if ((!_tr_str_eqv((((Param*)List_ptr_get(_imeth->params, _ip_i))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4729 "src/sema.tr"
                                _ip_cnt = (_ip_cnt + 1LL);
                            }
#line 4730 "src/sema.tr"
                            _ip_i = (_ip_i + 1LL);
                        }
#line 4731 "src/sema.tr"
                        long long _cp_cnt = 0LL;
#line 4732 "src/sema.tr"
                        long long _cp_i = 0LL;
#line 4733 "src/sema.tr"
                        while ((_cp_i < _cmeth->params->len)) {
#line 4734 "src/sema.tr"
                            if ((!_tr_str_eqv((((Param*)List_ptr_get(_cmeth->params, _cp_i))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4734 "src/sema.tr"
                                _cp_cnt = (_cp_cnt + 1LL);
                            }
#line 4735 "src/sema.tr"
                            _cp_i = (_cp_i + 1LL);
                        }
#line 4736 "src/sema.tr"
                        if ((_ip_cnt != _cp_cnt)) {
#line 4737 "src/sema.tr"
                            ({ __auto_type _ord_t1407 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-3] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': method '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_cmeth->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1408 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1407); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_cp_cnt)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" parameter(s) but interface '", 29LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' requires ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1409 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1408); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_ip_cnt)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: Match the parameter list in '", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' exactly.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1409); _tr_str_release(_at_t1409); });
                        }
                    }
#line 4738 "src/sema.tr"
                    _cm_i = (_cm_i + 1LL);
                }
#line 4739 "src/sema.tr"
                if ((!_found)) {
#line 4740 "src/sema.tr"
                    TrStr _sig = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("pub def ", 8LL)), (_imeth->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(self", 5LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 4741 "src/sema.tr"
                    long long _pi = 0LL;
#line 4742 "src/sema.tr"
                    while ((_pi < _imeth->params->len)) {
#line 4743 "src/sema.tr"
                        Param* _p = ((Param*)List_ptr_get(_imeth->params, _pi));
#line 4744 "src/sema.tr"
                        if ((!_tr_str_eqv((_p->name), (_tr_str_lit_len("self", 4LL))))) {
#line 4745 "src/sema.tr"
                            TrStr _pty = _tr_str_lit_len("int", 3LL);
#line 4746 "src/sema.tr"
                            if ((((unsigned long long)(_p->ty)) != ((unsigned long long)(0LL)))) {
#line 4746 "src/sema.tr"
                                TrStr _strtmp_t1410 = _tr_str_retain((*_p->ty)->name);
                                _tr_str_release(_pty);
                                _pty = _strtmp_t1410;
                            }
#line 4747 "src/sema.tr"
                            ({ TrStr _ap_t1411 = (_tr_str_lit_len(", ", 2LL)); TrStr _ap_t1412 = (_p->name); TrStr _ap_t1413 = (_tr_str_lit_len(": ", 2LL)); TrStr _ap_t1414 = (_pty); _tr_str_append(&_sig, _ap_t1411); _tr_str_append(&_sig, _ap_t1412); _tr_str_append(&_sig, _ap_t1413); _tr_str_append(&_sig, _ap_t1414); });
                        }
#line 4748 "src/sema.tr"
                        _pi = (_pi + 1LL);
                    }
#line 4749 "src/sema.tr"
                    TrStr _iret2 = _tr_str_lit_len("void", 4LL);
#line 4750 "src/sema.tr"
                    if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 4750 "src/sema.tr"
                        TrStr _strtmp_t1415 = _tr_str_retain((*_imeth->ret_ty)->name);
                        _tr_str_release(_iret2);
                        _iret2 = _strtmp_t1415;
                    }
#line 4751 "src/sema.tr"
                    ({ TrStr _ap_t1416 = (_tr_str_lit_len(") -> ", 5LL)); TrStr _ap_t1417 = (_iret2); TrStr _ap_t1418 = (_tr_str_lit_len(":", 1LL)); _tr_str_append(&_sig, _ap_t1416); _tr_str_append(&_sig, _ap_t1417); _tr_str_append(&_sig, _ap_t1418); });
#line 4752 "src/sema.tr"
                    ({ TrStr _at_t1419 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-2] Class '", 13LL)), (c->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' implements '", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_ifc_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' but is missing method '", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_imeth->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: Add to 'extend ", 29LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":'  ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_sig)); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1419); _tr_str_release(_at_t1419); });
                }
#line 4753 "src/sema.tr"
                _im_i = (_im_i + 1LL);
            }
        }
#line 4754 "src/sema.tr"
        _ifc_i = (_ifc_i + 1LL);
        _tr_str_release(_ifc_nm);
    }
#line 4756 "src/sema.tr"
    HirClass* hc = ((HirClass*)_tr_obj_alloc(sizeof(HirClass)));
#line 4757 "src/sema.tr"
    hc->name = _tr_str_retain(c->name);
#line 4758 "src/sema.tr"
    hc->generics = c->generics;
#line 4759 "src/sema.tr"
    hc->fields = hfields;
#line 4760 "src/sema.tr"
    hc->methods = hmethods;
#line 4761 "src/sema.tr"
    hc->base_classes = c->base_classes;
#line 4762 "src/sema.tr"
    hc->iface_names = c->iface_names;
#line 4763 "src/sema.tr"
    hc->iface_targs = c->iface_targs;
#line 4764 "src/sema.tr"
    hc->decorators = c->decorators;
#line 4765 "src/sema.tr"
    hc->is_public = c->is_public;
#line 4766 "src/sema.tr"
    hc->is_class = c->is_class;
#line 4768 "src/sema.tr"
    self->current_class_name = _tr_str_lit_len("", 0LL);
#line 4769 "src/sema.tr"
    self->current_region_params = (void*)List_TrStr_new();
#line 4770 "src/sema.tr"
    return hc;
}

__attribute__((hot)) HirEnum* Sema_lower_enum(Sema* self, EnumDef* e) {
#line 4773 "src/sema.tr"
    self->current_class_name = _tr_str_retain(e->name);
#line 4774 "src/sema.tr"
    self->current_region_params = e->region_params;
#line 4775 "src/sema.tr"
    List_ptr* hvariants = (void*)List_ptr_new();
#line 4776 "src/sema.tr"
    long long i = 0LL;
#line 4777 "src/sema.tr"
    while ((i < e->variants->len)) {
#line 4778 "src/sema.tr"
        VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, i));
#line 4779 "src/sema.tr"
        List_ptr* hfields = (void*)List_ptr_new();
#line 4780 "src/sema.tr"
        long long j = 0LL;
#line 4781 "src/sema.tr"
        while ((j < v->fields->len)) {
#line 4782 "src/sema.tr"
            Param* p = ((Param*)List_ptr_get(v->fields, j));
#line 4786 "src/sema.tr"
            if ((self->strict_mode && (((unsigned long long)(p->ty)) != ((unsigned long long)(0LL))))) {
#line 4787 "src/sema.tr"
                AstType* _vty = (*p->ty);
#line 4788 "src/sema.tr"
                long long _vri = 0LL;
#line 4789 "src/sema.tr"
                while ((_vri < _vty->from_regions->len)) {
#line 4790 "src/sema.tr"
                    TrStr _vrn = List_TrStr_get(_vty->from_regions, _vri);
#line 4791 "src/sema.tr"
                    bool _vrok = false;
#line 4792 "src/sema.tr"
                    long long _vrk = 0LL;
#line 4793 "src/sema.tr"
                    while ((_vrk < e->region_params->len)) {
#line 4794 "src/sema.tr"
                        if (({ TrStr _cmpa_t1420 = (List_TrStr_get(e->region_params, _vrk)); TrStr _cmpb_t1421 = (_vrn); _Bool _cmpr_t1422 = _tr_str_eqv(_cmpa_t1420, _cmpb_t1421); _tr_str_release(_cmpa_t1420); _cmpr_t1422; })) {
#line 4794 "src/sema.tr"
                            _vrok = true;
                        }
#line 4795 "src/sema.tr"
                        _vrk = (_vrk + 1LL);
                    }
#line 4796 "src/sema.tr"
                    if ((!_vrok)) {
#line 4797 "src/sema.tr"
                        self->current_line = e->line;
#line 4798 "src/sema.tr"
                        ({ TrStr _at_t1423 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-2] region source '", 21LL)), (_vrn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in variant '", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of enum '", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (e->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not a region parameter.\n      FIX: declare 'enum ", 54LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (e->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" from ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_vrn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":'.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1423); _tr_str_release(_at_t1423); });
                    }
#line 4799 "src/sema.tr"
                    _vri = (_vri + 1LL);
                    _tr_str_release(_vrn);
                }
            }
#line 4800 "src/sema.tr"
            HirParam* hp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 4801 "src/sema.tr"
            hp->name = _tr_str_retain(p->name);
#line 4806 "src/sema.tr"
            AstType* p_ty = AstType_init(p->name);
#line 4807 "src/sema.tr"
            if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
#line 4807 "src/sema.tr"
                p_ty = (*p->ty);
            }
#line 4808 "src/sema.tr"
            hp->ty = p_ty;
#line 4809 "src/sema.tr"
            List_ptr_append(hfields, _tr_obj_retain(hp));
#line 4810 "src/sema.tr"
            j = (j + 1LL);
            _tr_obj_release(hp, _trdrop_HirParam);
        }
#line 4812 "src/sema.tr"
        HirVariant* hv = ((HirVariant*)_tr_obj_alloc(sizeof(HirVariant)));
#line 4813 "src/sema.tr"
        hv->name = _tr_str_retain(v->name);
#line 4814 "src/sema.tr"
        hv->fields = hfields;
#line 4815 "src/sema.tr"
        List_ptr_append(hvariants, _tr_obj_retain(hv));
#line 4816 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hv, _trdrop_HirVariant);
    }
#line 4818 "src/sema.tr"
    List_ptr* hmethods = (void*)List_ptr_new();
#line 4819 "src/sema.tr"
    i = 0LL;
#line 4820 "src/sema.tr"
    while ((i < e->methods->len)) {
#line 4821 "src/sema.tr"
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(e->methods, i)));
#line 4822 "src/sema.tr"
        List_ptr_append(hmethods, _tr_obj_retain(hm));
#line 4823 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
#line 4825 "src/sema.tr"
    HirEnum* he = ((HirEnum*)_tr_obj_alloc(sizeof(HirEnum)));
#line 4826 "src/sema.tr"
    he->name = _tr_str_retain(e->name);
#line 4827 "src/sema.tr"
    he->generics = e->generics;
#line 4828 "src/sema.tr"
    he->variants = hvariants;
#line 4829 "src/sema.tr"
    he->methods = hmethods;
#line 4830 "src/sema.tr"
    he->iface_names = e->iface_names;
#line 4831 "src/sema.tr"
    he->decorators = e->decorators;
#line 4832 "src/sema.tr"
    he->is_public = e->is_public;
#line 4834 "src/sema.tr"
    self->current_class_name = _tr_str_lit_len("", 0LL);
#line 4835 "src/sema.tr"
    self->current_region_params = (void*)List_TrStr_new();
#line 4836 "src/sema.tr"
    return he;
}

__attribute__((hot)) HirInterface* Sema_lower_interface(Sema* self, InterfaceDef* i_def) {
#line 4839 "src/sema.tr"
    self->current_region_params = i_def->region_params;
#line 4840 "src/sema.tr"
    List_ptr* hmethods = (void*)List_ptr_new();
#line 4841 "src/sema.tr"
    long long i = 0LL;
#line 4842 "src/sema.tr"
    while ((i < i_def->methods->len)) {
#line 4843 "src/sema.tr"
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(i_def->methods, i)));
#line 4844 "src/sema.tr"
        List_ptr_append(hmethods, _tr_obj_retain(hm));
#line 4845 "src/sema.tr"
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
#line 4847 "src/sema.tr"
    HirInterface* hi = ((HirInterface*)_tr_obj_alloc(sizeof(HirInterface)));
#line 4848 "src/sema.tr"
    hi->name = _tr_str_retain(i_def->name);
#line 4849 "src/sema.tr"
    hi->generics = i_def->generics;
#line 4850 "src/sema.tr"
    hi->methods = hmethods;
#line 4851 "src/sema.tr"
    self->current_region_params = (void*)List_TrStr_new();
#line 4852 "src/sema.tr"
    return hi;
}

__attribute__((hot)) void Sema_apply_escape_marks(Sema* self, HirStmt* _hs) {
#line 4858 "src/sema.tr"
    __auto_type _t1424 = (*_hs);
    if (_t1424.tag == HirStmt_SExpr) {
        __auto_type _es_e = _t1424.data.SExpr.expr;
#line 4860 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _es_e);
#line 4861 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _es_e);
    } else if (_t1424.tag == HirStmt_SLet) {
        __auto_type _sl_v = _t1424.data.SLet.val;
#line 4863 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sl_v);
#line 4864 "src/sema.tr"
        Sema_mark_coll_arg(self, _sl_v);
#line 4865 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sl_v);
    } else if (_t1424.tag == HirStmt_SAssign) {
        __auto_type _sa_t = _t1424.data.SAssign.target;
__auto_type _sa_v = _t1424.data.SAssign.val;
#line 4867 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sa_t);
#line 4868 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sa_v);
#line 4869 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sa_t);
#line 4870 "src/sema.tr"
        Sema_mark_coll_arg(self, _sa_v);
#line 4871 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sa_v);
    } else if (_t1424.tag == HirStmt_SReturn) {
        __auto_type _sr_e = _t1424.data.SReturn.val;
#line 4873 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sr_e);
#line 4874 "src/sema.tr"
        Sema_mark_coll_arg(self, _sr_e);
#line 4875 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sr_e);
    } else if (_t1424.tag == HirStmt_SIf) {
        __auto_type _si_c = _t1424.data.SIf.cond;
#line 4877 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _si_c);
#line 4878 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _si_c);
    } else if (_t1424.tag == HirStmt_SWhile) {
        __auto_type _sw_c = _t1424.data.SWhile.cond;
#line 4880 "src/sema.tr"
        Sema_mark_escaped_str_args(self, _sw_c);
#line 4881 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sw_c);
    } else if (_t1424.tag == HirStmt_SFor) {
        __auto_type _sf_iter = _t1424.data.SFor.iter;
#line 4883 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sf_iter);
    } else if (_t1424.tag == HirStmt_SForUnpack) {
        __auto_type _sfu_iter = _t1424.data.SForUnpack.iter;
#line 4885 "src/sema.tr"
        Sema_mark_escaped_coll_args(self, _sfu_iter);
    } else if (1) {
        __auto_type _ = _t1424;
        /* pass */
    }
}

__attribute__((hot)) HirBlock* Sema_lower_block(Sema* self, Block* b) {
#line 4889 "src/sema.tr"
    HirBlock* hb = HirBlock_init();
#line 4890 "src/sema.tr"
    long long i = 0LL;
#line 4891 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 4892 "src/sema.tr"
        Stmt* orig_ptr = ((Stmt*)List_ptr_get(b->stmts, i));
#line 4893 "src/sema.tr"
        HirStmt* _hs = Sema_lower_stmt(self, orig_ptr);
#line 4894 "src/sema.tr"
        Sema_apply_escape_marks(self, _hs);
#line 4898 "src/sema.tr"
        if ((((unsigned long long)(orig_ptr)) != ((unsigned long long)(0LL)))) {
#line 4899 "src/sema.tr"
            __auto_type _t1425 = (*orig_ptr);
            if (_t1425.tag == Stmt_SReturn) {
                __auto_type _ = _t1425.data.SReturn.val;
#line 4908 "src/sema.tr"
                List_TrStr* ret_excl = (void*)List_TrStr_new();
#line 4909 "src/sema.tr"
                __auto_type _t1426 = (*_hs);
                if (_t1426.tag == HirStmt_SReturn) {
                    __auto_type lowered_ret = _t1426.data.SReturn.val;
#line 4911 "src/sema.tr"
                    Sema_collect_idents(self, lowered_ret, ret_excl);
                } else if (1) {
                    __auto_type _ = _t1426;
                    /* pass */
                }
#line 4916 "src/sema.tr"
                long long ret_from = 0LL;
#line 4917 "src/sema.tr"
                if ((self->fn_scope_base->len > 0LL)) {
#line 4918 "src/sema.tr"
                    ret_from = List_i64_get(self->fn_scope_base, (self->fn_scope_base->len - 1LL));
                }
#line 4924 "src/sema.tr"
                bool ret_pre_done = false;
#line 4925 "src/sema.tr"
                if ((ret_excl->len > 0LL)) {
#line 4926 "src/sema.tr"
                    __auto_type _t1427 = (*_hs);
                    if (_t1427.tag == HirStmt_SReturn) {
                        __auto_type prim_ret = _t1427.data.SReturn.val;
#line 4928 "src/sema.tr"
                        if ((((unsigned long long)(prim_ret)) != ((unsigned long long)(0LL)))) {
#line 4929 "src/sema.tr"
                            AstType* prim_ty = hir_expr_type(prim_ret);
#line 4930 "src/sema.tr"
                            if ((((Sema_is_primitive_name(self, prim_ty->name) && (!_tr_str_eqv((prim_ty->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((prim_ty->name), (_tr_str_lit_len("None", 4LL))))) && (!_tr_str_eqv((prim_ty->name), (_tr_str_lit_len("lambda", 6LL)))))) {
#line 4931 "src/sema.tr"
                                HirBlock* all_drops = HirBlock_init();
#line 4932 "src/sema.tr"
                                Sema_append_drops_from_excl_multi(self, all_drops, ret_from, (void*)List_TrStr_new());
#line 4938 "src/sema.tr"
                                HirBlock* pre_drops = HirBlock_init();
#line 4939 "src/sema.tr"
                                long long adi = 0LL;
#line 4940 "src/sema.tr"
                                while ((adi < all_drops->stmts->len)) {
#line 4941 "src/sema.tr"
                                    HirStmt* ad = ((HirStmt*)List_ptr_get(all_drops->stmts, adi));
#line 4942 "src/sema.tr"
                                    bool keep_d = true;
#line 4943 "src/sema.tr"
                                    __auto_type _t1428 = (*ad);
                                    if (_t1428.tag == HirStmt_SAutoDrop) {
                                        __auto_type adn = _t1428.data.SAutoDrop.name;
__auto_type adt = _t1428.data.SAutoDrop.class_name;
#line 4945 "src/sema.tr"
                                        bool was_excl = false;
#line 4946 "src/sema.tr"
                                        long long aei = 0LL;
#line 4947 "src/sema.tr"
                                        while ((aei < ret_excl->len)) {
#line 4948 "src/sema.tr"
                                            if (({ TrStr _cmpa_t1429 = (List_TrStr_get(ret_excl, aei)); TrStr _cmpb_t1430 = (adn); _Bool _cmpr_t1431 = _tr_str_eqv(_cmpa_t1429, _cmpb_t1430); _tr_str_release(_cmpa_t1429); _cmpr_t1431; })) {
#line 4948 "src/sema.tr"
                                                was_excl = true;
                                            }
#line 4949 "src/sema.tr"
                                            aei = (aei + 1LL);
                                        }
#line 4950 "src/sema.tr"
                                        if ((was_excl && (!Sema__early_exit_drop_ok(self, adt)))) {
#line 4950 "src/sema.tr"
                                            keep_d = false;
                                        }
                                    } else if (1) {
                                        __auto_type _ = _t1428;
                                        /* pass */
                                    }
#line 4952 "src/sema.tr"
                                    if (keep_d) {
#line 4952 "src/sema.tr"
                                        HirBlock_push(pre_drops, ad);
                                    }
#line 4953 "src/sema.tr"
                                    adi = (adi + 1LL);
                                }
#line 4954 "src/sema.tr"
                                if ((pre_drops->stmts->len > 0LL)) {
#line 4955 "src/sema.tr"
                                    self->do_temp_ctr = (self->do_temp_ctr + 1LL);
#line 4956 "src/sema.tr"
                                    TrStr rt_name = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->do_temp_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_trret", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 4957 "src/sema.tr"
                                    HirBlock_push(hb, box_hirstmt(HirStmt_ctor_SLet(rt_name, Ownership_make_Own(), false, false, false, prim_ty, prim_ret)));
#line 4958 "src/sema.tr"
                                    long long pdi = 0LL;
#line 4959 "src/sema.tr"
                                    while ((pdi < pre_drops->stmts->len)) {
#line 4960 "src/sema.tr"
                                        HirBlock_push(hb, ((HirStmt*)List_ptr_get(pre_drops->stmts, pdi)));
#line 4961 "src/sema.tr"
                                        pdi = (pdi + 1LL);
                                    }
#line 4962 "src/sema.tr"
                                    _hs = box_hirstmt(HirStmt_ctor_SReturn(box_hirexpr(HirExpr_ctor_EIdent(rt_name, prim_ty, false))));
#line 4963 "src/sema.tr"
                                    ret_pre_done = true;
                                }
                                _tr_obj_release(pre_drops, _trdrop_HirBlock);
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t1427;
                        /* pass */
                    }
                }
#line 4965 "src/sema.tr"
                if ((!ret_pre_done)) {
#line 4966 "src/sema.tr"
                    Sema_append_drops_from_excl_multi(self, hb, ret_from, ret_excl);
                }
            } else if (_t1425.tag == Stmt_SBreak) {
                __auto_type _ = _t1425.data.SBreak.val;
#line 4968 "src/sema.tr"
                if ((self->loop_scope_base->len > 0LL)) {
#line 4969 "src/sema.tr"
                    Sema_append_drops_from(self, hb, List_i64_get(self->loop_scope_base, (self->loop_scope_base->len - 1LL)));
#line 4970 "src/sema.tr"
                    Sema_append_while_exit_drops(self, hb);
                }
            } else if (_t1425.tag == Stmt_SContinue) {
#line 4972 "src/sema.tr"
                if ((self->loop_scope_base->len > 0LL)) {
#line 4973 "src/sema.tr"
                    Sema_append_drops_from(self, hb, List_i64_get(self->loop_scope_base, (self->loop_scope_base->len - 1LL)));
#line 4974 "src/sema.tr"
                    Sema_append_while_exit_drops(self, hb);
                }
            } else if (1) {
                __auto_type _ = _t1425;
                /* pass */
            }
        }
#line 4976 "src/sema.tr"
        HirBlock_push(hb, _hs);
#line 4977 "src/sema.tr"
        i = (i + 1LL);
    }
#line 4978 "src/sema.tr"
    return hb;
}

__attribute__((hot)) HirStmt* Sema_lower_stmt(Sema* self, Stmt* s_ptr) {
#line 4981 "src/sema.tr"
    if ((((unsigned long long)(s_ptr)) == ((unsigned long long)(0LL)))) {
#line 4981 "src/sema.tr"
        return (HirStmt*)(0LL);
    }
#line 4982 "src/sema.tr"
    __auto_type s = (*s_ptr);
#line 4983 "src/sema.tr"
    __auto_type _t1432 = s;
    if (_t1432.tag == Stmt_SExpr) {
        __auto_type e = _t1432.data.SExpr.expr;
#line 4985 "src/sema.tr"
        HirStmt* h_s_expr = box_hirstmt(HirStmt_ctor_SExpr(Sema_lower_expr(self, e)));
#line 4987 "src/sema.tr"
        if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
#line 4988 "src/sema.tr"
            __auto_type _t1433 = (*e);
            if (_t1433.tag == Expr_ECall) {
                __auto_type callee = _t1433.data.ECall.callee;
__auto_type args = _t1433.data.ECall.args;
#line 4990 "src/sema.tr"
                if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 4991 "src/sema.tr"
                    __auto_type _t1434 = (*callee);
                    if (_t1434.tag == Expr_EIdent) {
                        __auto_type fn_name = _t1434.data.EIdent.name;
#line 4993 "src/sema.tr"
                        Symbol* fn_sym = Sema_resolve(self, fn_name);
#line 4994 "src/sema.tr"
                        if (((fn_sym->kind.tag == SymbolKind_make_SFunction().tag) && _tr_str_eqv(((*fn_sym->ty)->name), (_tr_str_lit_len("Result", 6LL))))) {
#line 4995 "src/sema.tr"
                            ({ TrStr _at_t1435 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-4] '", 7LL)), (fn_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' returns a Result and its error must be handled. FIX: Assign the result and match on it, use '?' to propagate, or '_ = ", 122LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fn_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' to explicitly discard.", 29LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1435); _tr_str_release(_at_t1435); });
                        }
                    } else if (1) {
                        __auto_type _ = _t1434;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1433;
                /* pass */
            }
        }
#line 4998 "src/sema.tr"
        return h_s_expr;
    } else if (_t1432.tag == Stmt_SReturn) {
        __auto_type e = _t1432.data.SReturn.val;
#line 5006 "src/sema.tr"
        if (((self->strict_mode && self->current_func_ret_borrow_str) && (((unsigned long long)(e)) != ((unsigned long long)(0LL))))) {
#line 5007 "src/sema.tr"
            __auto_type _t1436 = (*e);
            if (_t1436.tag == Expr_EBinOp) {
                __auto_type l3_op = _t1436.data.EBinOp.op;
#line 5009 "src/sema.tr"
                if (_tr_str_eqv((l3_op), (_tr_str_lit_len("+", 1LL)))) {
#line 5010 "src/sema.tr"
                    ({ TrStr _at_t1437 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-3] borrow-returning function returns a freshly-built string (owned), not a borrow of region '", 96LL)), (self->current_func_ret_from))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: return a slice/view of the region source, or change the return type to a plain owned 'str'.", 105LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1437); _tr_str_release(_at_t1437); });
                }
            } else if (_t1436.tag == Expr_EIdent) {
                __auto_type l3_nm = _t1436.data.EIdent.name;
#line 5012 "src/sema.tr"
                Symbol* l3_sym = Sema_resolve(self, l3_nm);
#line 5013 "src/sema.tr"
                if ((!_tr_str_eqv((l3_sym->name), (_tr_str_lit_len("", 0LL))))) {
#line 5016 "src/sema.tr"
                    TrStr l3_eff = _tr_str_lit_len("", 0LL);
#line 5017 "src/sema.tr"
                    if (l3_sym->is_param) {
#line 5018 "src/sema.tr"
                        TrStr _strtmp_t1438 = _tr_str_retain(l3_nm);
                        _tr_str_release(l3_eff);
                        l3_eff = _strtmp_t1438;
                    } else {
#line 5020 "src/sema.tr"
                        TrStr _strtmp_t1439 = _tr_str_retain(l3_sym->borrows_region);
                        _tr_str_release(l3_eff);
                        l3_eff = _strtmp_t1439;
                    }
#line 5021 "src/sema.tr"
                    if (_tr_str_eqv((l3_eff), (_tr_str_lit_len("@owned", 6LL)))) {
#line 5022 "src/sema.tr"
                        ({ TrStr _at_t1440 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-3] borrow-returning function returns '", 41LL)), (l3_nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', a freshly-built string (owned), not a borrow of region '", 59LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_ret_from)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: return a slice/view of the region source, or change the return type to a plain owned 'str'.", 105LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1440); _tr_str_release(_at_t1440); });
                    } else if (((!_tr_str_eqv((l3_eff), (_tr_str_lit_len("", 0LL)))) && (self->current_func_ret_regions->len > 0LL))) {
#line 5027 "src/sema.tr"
                        bool l3_in = false;
#line 5028 "src/sema.tr"
                        long long l3ri = 0LL;
#line 5029 "src/sema.tr"
                        while ((l3ri < self->current_func_ret_regions->len)) {
#line 5030 "src/sema.tr"
                            if (({ TrStr _cmpa_t1441 = (List_TrStr_get(self->current_func_ret_regions, l3ri)); TrStr _cmpb_t1442 = (l3_eff); _Bool _cmpr_t1443 = _tr_str_eqv(_cmpa_t1441, _cmpb_t1442); _tr_str_release(_cmpa_t1441); _cmpr_t1443; })) {
#line 5031 "src/sema.tr"
                                l3_in = true;
                            }
#line 5032 "src/sema.tr"
                            l3ri = (l3ri + 1LL);
                        }
#line 5036 "src/sema.tr"
                        if ((!l3_in)) {
#line 5037 "src/sema.tr"
                            long long l3di = 0LL;
#line 5038 "src/sema.tr"
                            while ((l3di < self->current_func_ret_regions->len)) {
#line 5039 "src/sema.tr"
                                if (({ TrStr _at_t1444 = (List_TrStr_get(self->current_func_ret_regions, l3di)); __auto_type _wr = (Sema_region_outlives(self, l3_eff, _at_t1444)); _tr_str_release(_at_t1444); _wr; })) {
#line 5040 "src/sema.tr"
                                    l3_in = true;
                                }
#line 5041 "src/sema.tr"
                                l3di = (l3di + 1LL);
                            }
                        }
#line 5042 "src/sema.tr"
                        if ((!l3_in)) {
#line 5043 "src/sema.tr"
                            ({ TrStr _at_t1445 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-4] borrow-returning function returns a borrow of region '", 60LL)), (l3_eff))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but its return is declared 'from ", 36LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_ret_from)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: add '", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (l3_eff)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to the 'from' list, declare 'where ", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (l3_eff)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" outlives ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_ret_from)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or return a borrow of a declared region.", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1445); _tr_str_release(_at_t1445); });
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1436;
                /* pass */
            }
        }
#line 5046 "src/sema.tr"
        if ((((((unsigned long long)(e)) != ((unsigned long long)(0LL))) && (!self->in_unsafe)) && _tr_str_eqv((self->current_func_ret_from), (_tr_str_lit_len("", 0LL))))) {
#line 5047 "src/sema.tr"
            __auto_type _t1446 = (*e);
            if (_t1446.tag == Expr_EIdent) {
                __auto_type ret_name = _t1446.data.EIdent.name;
#line 5049 "src/sema.tr"
                Symbol* ret_sym = Sema_resolve(self, ret_name);
#line 5050 "src/sema.tr"
                if (((!_tr_str_eqv((ret_sym->name), (_tr_str_lit_len("", 0LL)))) && (ret_sym->ptr_region == 0LL))) {
#line 5051 "src/sema.tr"
                    if ((((unsigned long long)(ret_sym->ty)) != ((unsigned long long)(0LL)))) {
#line 5052 "src/sema.tr"
                        if (_tr_str_eqv(((*ret_sym->ty)->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 5053 "src/sema.tr"
                            ({ TrStr _at_t1447 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-1] '", 7LL)), (ret_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a local Pointer that may not outlive this function call. Returning it is unsafe.\n      FIX: Annotate the return type with 'from <param>' if the pointer borrows from a parameter, or wrap the allocation in 'unsafe:' if it is heap-allocated.", 243LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1447); _tr_str_release(_at_t1447); });
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1446;
                /* pass */
            }
        }
#line 5055 "src/sema.tr"
        HirExpr* _ret_h = Sema_lower_expr(self, e);
#line 5056 "src/sema.tr"
        if ((((((unsigned long long)(e)) != ((unsigned long long)(0LL))) && (self->closure_depth == 0LL)) && (!_tr_str_eqv((self->current_func_ret_family), (_tr_str_lit_len("", 0LL)))))) {
#line 5057 "src/sema.tr"
            TrStr _rf = Sema_firm_family(self, _ret_h);
#line 5058 "src/sema.tr"
            if (Sema_family_mismatch(self, self->current_func_ret_family, _rf)) {
#line 5059 "src/sema.tr"
                ({ __auto_type _ord_t1448 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is declared to return ", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1449 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1448); TrStr _tr_cr = (Sema_family_word(self, self->current_func_ret_family)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but this returns ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1450 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1449); TrStr _tr_cr = (Sema_family_word(self, _rf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: return a value of the declared type, or change the function's return type.", 87LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1450); _tr_str_release(_at_t1450); });
            }
        }
#line 5060 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SReturn(_ret_h));
    } else if (_t1432.tag == Stmt_SLet) {
        __auto_type name = _t1432.data.SLet.name;
__auto_type ownership = _t1432.data.SLet.ownership;
__auto_type is_mut = _t1432.data.SLet.is_mut;
__auto_type is_const = _t1432.data.SLet.is_const;
__auto_type is_shared = _t1432.data.SLet.is_shared;
__auto_type ty_ptr = _t1432.data.SLet.ty;
__auto_type val_ptr = _t1432.data.SLet.val;
#line 5063 "src/sema.tr"
        if (((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))))) {
#line 5064 "src/sema.tr"
            __auto_type _t1451 = (*val_ptr);
            if (_t1451.tag == Expr_ELitNone) {
#line 5066 "src/sema.tr"
                TrStr m7_ty_name = _tr_str_retain((*ty_ptr)->name);
#line 5067 "src/sema.tr"
                if ((((((!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("Option", 6LL)))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("None", 4LL))))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((m7_ty_name), (_tr_str_lit_len("Pointer", 7LL)))))) {
#line 5068 "src/sema.tr"
                    ({ TrStr _at_t1452 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-7] Cannot assign 'none' to '", 31LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' which has type '", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m7_ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'. Only Option[T] can hold 'none'. FIX: Use 'Option[", 52LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m7_ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' as the type, or give '", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' a real initial value.", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1452); _tr_str_release(_at_t1452); });
                }
                _tr_str_release(m7_ty_name);
            } else if (1) {
                __auto_type _ = _t1451;
                /* pass */
            }
        }
#line 5070 "src/sema.tr"
        AstType* ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 5071 "src/sema.tr"
        if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 5072 "src/sema.tr"
            ty = (*ty_ptr);
#line 5073 "src/sema.tr"
            ({ TrStr _at_t1453 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declaration", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema__check_ty_arity(self, ty_ptr, _at_t1453, 0LL); _tr_str_release(_at_t1453); });
#line 5074 "src/sema.tr"
            ({ TrStr _at_t1454 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the declaration of '", 20LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_type_known(self, ty_ptr, _at_t1454); _tr_str_release(_at_t1454); });
        }
#line 5075 "src/sema.tr"
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
#line 5076 "src/sema.tr"
        if (((!_tr_str_eqv((ty->name), (_tr_str_lit_len("void", 4LL)))) && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("None", 4LL)))))) {
#line 5076 "src/sema.tr"
            hval = Sema__patch_empty_dict_hint(self, hval, ty);
        }
#line 5077 "src/sema.tr"
        if (((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))))) {
#line 5078 "src/sema.tr"
            ({ __auto_type _ord_t1456 = (hir_expr_type(hval)); TrStr _at_t1455 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the initializer of '", 20LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_fn_value_throws(self, ty_ptr, _ord_t1456, _at_t1455); _tr_str_release(_at_t1455); });
        }
#line 5079 "src/sema.tr"
        if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("None", 4LL))))) {
#line 5079 "src/sema.tr"
            ty = hir_expr_type(hval);
        }
#line 5084 "src/sema.tr"
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && (!is_shared))) {
#line 5085 "src/sema.tr"
            __auto_type _t1457 = (*val_ptr);
            if (_t1457.tag == Expr_EIdent) {
                __auto_type m1_src = _t1457.data.EIdent.name;
#line 5087 "src/sema.tr"
                Symbol* m1_sym = Sema_resolve(self, m1_src);
#line 5088 "src/sema.tr"
                AstType* m1_ty = (*m1_sym->ty);
#line 5090 "src/sema.tr"
                if (((((!Sema_is_primitive(self, m1_ty)) && (!Sema_is_copy_class(self, m1_ty->name))) && (!_tr_str_eqv((m1_sym->name), (_tr_str_lit_len("", 0LL))))) && (!m1_sym->is_shared))) {
#line 5091 "src/sema.tr"
                    Sema_check_not_moved(self, m1_src, m1_ty->name);
#line 5092 "src/sema.tr"
                    Sema_check_no_active_borrows(self, m1_src, m1_ty->name);
#line 5093 "src/sema.tr"
                    Sema_mark_moved(self, m1_src);
                } else if (((Sema_class_has_free(self, m1_ty->name) && (!_tr_str_eqv((m1_sym->name), (_tr_str_lit_len("", 0LL))))) && (!m1_sym->is_shared))) {
#line 5100 "src/sema.tr"
                    Sema_mark_moved(self, m1_src);
                }
            } else if (1) {
                __auto_type _ = _t1457;
                /* pass */
            }
        }
#line 5104 "src/sema.tr"
        if (((is_shared && _tr_str_eqv((ty->name), (_tr_str_lit_len("Shared", 6LL)))) && (ty->args->len > 0LL))) {
#line 5104 "src/sema.tr"
            ty = (*((AstType**)List_ptr_get(ty->args, 0LL)));
        }
#line 5105 "src/sema.tr"
        if ((((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) && (!is_shared))) {
#line 5106 "src/sema.tr"
            ({ __auto_type _ord_t1459 = ((*ty_ptr)); TrStr _at_t1458 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the initializer of '", 20LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1459, hval, _at_t1458); _tr_str_release(_at_t1458); });
        }
#line 5107 "src/sema.tr"
        TrStr _let_firm = _tr_str_lit_len("", 0LL);
#line 5108 "src/sema.tr"
        if ((!is_shared)) {
#line 5109 "src/sema.tr"
            if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 5110 "src/sema.tr"
                TrStr _strtmp_t1460 = Sema_decl_family(self, (*ty_ptr));
                _tr_str_release(_let_firm);
                _let_firm = _strtmp_t1460;
            } else if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
#line 5112 "src/sema.tr"
                TrStr _strtmp_t1461 = Sema_firm_family(self, hval);
                _tr_str_release(_let_firm);
                _let_firm = _strtmp_t1461;
            }
        }
#line 5113 "src/sema.tr"
        ({ __auto_type _ord_t1462 = (SymbolKind_make_SVariable()); __auto_type _ord_t1463 = (box_asttype(ty)); Sema_declare(self, name, _ord_t1462, _ord_t1463, is_mut); });
#line 5114 "src/sema.tr"
        if (((!_tr_str_eqv((_let_firm), (_tr_str_lit_len("", 0LL)))) && (self->scopes->len > 0LL))) {
#line 5115 "src/sema.tr"
            Scope* _fm_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 5116 "src/sema.tr"
            if (_tr_dict_contains(_fm_scope->variables, _tr_strz(name))) {
#line 5117 "src/sema.tr"
                Symbol* _fm_sym = ((Symbol*)(uintptr_t)_tr_dict_get(_fm_scope->variables, _tr_strz(name)));
#line 5118 "src/sema.tr"
                _fm_sym->firm = _tr_str_retain(_let_firm);
#line 5119 "src/sema.tr"
                _tr_dict_set(_fm_scope->variables, _tr_strz(name), _fm_sym);
            }
        }
#line 5123 "src/sema.tr"
        if ((is_shared && (self->scopes->len > 0LL))) {
#line 5124 "src/sema.tr"
            Scope* _sh_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 5125 "src/sema.tr"
            if (_tr_dict_contains(_sh_scope->variables, _tr_strz(name))) {
#line 5126 "src/sema.tr"
                Symbol* _sh_sym = ((Symbol*)(uintptr_t)_tr_dict_get(_sh_scope->variables, _tr_strz(name)));
#line 5127 "src/sema.tr"
                _sh_sym->is_shared = true;
#line 5128 "src/sema.tr"
                _tr_dict_set(_sh_scope->variables, _tr_strz(name), _sh_sym);
            }
        }
#line 5131 "src/sema.tr"
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
#line 5132 "src/sema.tr"
            ({ TrStr _at_t1464 = (Sema_compute_region(self, val_ptr)); Sema_set_borrows_region(self, name, _at_t1464); _tr_str_release(_at_t1464); });
        }
#line 5138 "src/sema.tr"
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
#line 5139 "src/sema.tr"
            __auto_type _t1465 = (*val_ptr);
            if (_t1465.tag == Expr_ECall) {
                __auto_type ce_callee = _t1465.data.ECall.callee;
__auto_type ce_args = _t1465.data.ECall.args;
#line 5141 "src/sema.tr"
                if ((((unsigned long long)(ce_callee)) != ((unsigned long long)(0LL)))) {
#line 5142 "src/sema.tr"
                    __auto_type _t1466 = (*ce_callee);
                    if (_t1466.tag == Expr_EIdent) {
                        __auto_type ce_fn = _t1466.data.EIdent.name;
#line 5144 "src/sema.tr"
                        Symbol* ce_sym = Sema_resolve(self, ce_fn);
#line 5145 "src/sema.tr"
                        if (((!_tr_str_eqv((ce_sym->name), (_tr_str_lit_len("", 0LL)))) && (((unsigned long long)(ce_sym->ty)) != ((unsigned long long)(0LL))))) {
#line 5146 "src/sema.tr"
                            AstType* ce_ret = (*ce_sym->ty);
#line 5147 "src/sema.tr"
                            if ((((ce_ret->is_borrow || (ce_ret->from_regions->len > 0LL)) && (ce_ret->from_index >= 0LL)) && (ce_ret->from_index < ce_args->len))) {
#line 5148 "src/sema.tr"
                                __auto_type _t1467 = (*((Expr*)List_ptr_get(ce_args, ce_ret->from_index)));
                                if (_t1467.tag == Expr_EIdent) {
                                    __auto_type ce_src = _t1467.data.EIdent.name;
#line 5150 "src/sema.tr"
                                    List_TrStr_append(self->cur_func_borrowers, name);
#line 5151 "src/sema.tr"
                                    List_TrStr_append(self->cur_func_sources, ce_src);
                                } else if (1) {
                                    __auto_type _ = _t1467;
                                    /* pass */
                                }
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t1466;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1465;
                /* pass */
            }
        }
#line 5158 "src/sema.tr"
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && _tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 5159 "src/sema.tr"
            bool heap_rhs = false;
#line 5160 "src/sema.tr"
            __auto_type _t1468 = (*val_ptr);
            if (_t1468.tag == Expr_ECall) {
                heap_rhs = true;
            } else if (_t1468.tag == Expr_EMethodCall) {
                heap_rhs = true;
            } else if (1) {
                __auto_type _ = _t1468;
                /* pass */
            }
#line 5164 "src/sema.tr"
            if (heap_rhs) {
#line 5165 "src/sema.tr"
                if ((self->scopes->len > 0LL)) {
#line 5166 "src/sema.tr"
                    Scope* alloc_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 5167 "src/sema.tr"
                    if (_tr_dict_contains(alloc_scope->variables, _tr_strz(name))) {
#line 5168 "src/sema.tr"
                        Symbol* alloc_sym = ((Symbol*)(uintptr_t)_tr_dict_get(alloc_scope->variables, _tr_strz(name)));
#line 5169 "src/sema.tr"
                        alloc_sym->ptr_region = 1LL;
#line 5170 "src/sema.tr"
                        _tr_dict_set(alloc_scope->variables, _tr_strz(name), alloc_sym);
                    }
                }
            }
        }
#line 5173 "src/sema.tr"
        bool _decl_fixed_arr = false;
#line 5174 "src/sema.tr"
        if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 5175 "src/sema.tr"
            if ((_tr_str_eqv(((*ty_ptr)->name), (_tr_str_lit_len("Array", 5LL))) && ((*ty_ptr)->array_size > 0LL))) {
#line 5175 "src/sema.tr"
                _decl_fixed_arr = true;
            }
        }
#line 5176 "src/sema.tr"
        if (((((unsigned long long)(val_ptr)) == ((unsigned long long)(0LL))) && (!_decl_fixed_arr))) {
#line 5177 "src/sema.tr"
            if ((self->scopes->len > 0LL)) {
#line 5178 "src/sema.tr"
                Scope* pd_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
#line 5179 "src/sema.tr"
                if (_tr_dict_contains(pd_scope->variables, _tr_strz(name))) {
#line 5180 "src/sema.tr"
                    Symbol* pd_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pd_scope->variables, _tr_strz(name)));
#line 5181 "src/sema.tr"
                    pd_sym->is_init = false;
#line 5182 "src/sema.tr"
                    _tr_dict_set(pd_scope->variables, _tr_strz(name), pd_sym);
                }
            }
        }
#line 5189 "src/sema.tr"
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
#line 5190 "src/sema.tr"
            TrStr pc_cont_nm = _tr_str_lit_len("", 0LL);
#line 5191 "src/sema.tr"
            TrStr pc_meth = _tr_str_lit_len("", 0LL);
#line 5192 "src/sema.tr"
            __auto_type _t1469 = (*val_ptr);
            if (_t1469.tag == Expr_EMethodCall) {
                __auto_type pc_obj = _t1469.data.EMethodCall.obj;
__auto_type pc_m = _t1469.data.EMethodCall.method;
#line 5194 "src/sema.tr"
                TrStr _strtmp_t1470 = _tr_str_retain(pc_m);
                _tr_str_release(pc_meth);
                pc_meth = _strtmp_t1470;
#line 5195 "src/sema.tr"
                __auto_type _t1471 = (*pc_obj);
                if (_t1471.tag == Expr_EIdent) {
                    __auto_type pc_src = _t1471.data.EIdent.name;
                    TrStr _strtmp_t1472 = _tr_str_retain(pc_src);
                    _tr_str_release(pc_cont_nm);
                    pc_cont_nm = _strtmp_t1472;
                } else if (1) {
                    __auto_type _ = _t1471;
                    /* pass */
                }
            } else if (_t1469.tag == Expr_EIndex) {
                __auto_type pc_iobj = _t1469.data.EIndex.obj;
#line 5199 "src/sema.tr"
                TrStr _strtmp_t1473 = _tr_str_lit_len("get", 3LL);
                _tr_str_release(pc_meth);
                pc_meth = _strtmp_t1473;
#line 5200 "src/sema.tr"
                __auto_type _t1474 = (*pc_iobj);
                if (_t1474.tag == Expr_EIdent) {
                    __auto_type pc_isrc = _t1474.data.EIdent.name;
                    TrStr _strtmp_t1475 = _tr_str_retain(pc_isrc);
                    _tr_str_release(pc_cont_nm);
                    pc_cont_nm = _strtmp_t1475;
                } else if (1) {
                    __auto_type _ = _t1474;
                    /* pass */
                }
            } else if (1) {
                __auto_type _ = _t1469;
                /* pass */
            }
#line 5210 "src/sema.tr"
            if ((_tr_str_eqv((pc_cont_nm), (_tr_str_lit_len("", 0LL))) && ((_tr_str_eqv((pc_meth), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("last", 4LL)))))) {
#line 5211 "src/sema.tr"
                __auto_type _t1476 = (*hval);
                if (_t1476.tag == HirExpr_EMethodCall) {
                    __auto_type pc_ho = _t1476.data.EMethodCall.obj;
#line 5213 "src/sema.tr"
                    TrStr pc_hn = _tr_str_retain(hir_expr_type(pc_ho)->name);
#line 5214 "src/sema.tr"
                    if (((((_tr_str_eqv((pc_hn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((pc_hn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((pc_hn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pc_hn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((pc_hn), (_tr_str_lit_len("Set", 3LL))))) {
#line 5215 "src/sema.tr"
                        TrStr pc_fe = _tr_str_retain(hir_expr_type(hval)->name);
#line 5216 "src/sema.tr"
                        if ((((((_tr_str_eqv((pc_fe), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((pc_fe), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((pc_fe), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pc_fe), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((pc_fe), (_tr_str_lit_len("Set", 3LL)))) || Sema_class_has_free(self, pc_fe))) {
#line 5217 "src/sema.tr"
                            Sema_set_borrows_region(self, name, _tr_str_lit_len("@borrowed", 9LL));
                        }
                    }
                    _tr_str_release(pc_hn);
                } else if (1) {
                    __auto_type _ = _t1476;
                    /* pass */
                }
            }
#line 5219 "src/sema.tr"
            if (((!_tr_str_eqv((pc_cont_nm), (_tr_str_lit_len("", 0LL)))) && ((_tr_str_eqv((pc_meth), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((pc_meth), (_tr_str_lit_len("last", 4LL)))))) {
#line 5220 "src/sema.tr"
                Symbol* pc_cont_sym = Sema_resolve(self, pc_cont_nm);
#line 5221 "src/sema.tr"
                AstType* pc_cont_ty = (*pc_cont_sym->ty);
#line 5222 "src/sema.tr"
                TrStr pc_ctn = _tr_str_retain(pc_cont_ty->name);
#line 5223 "src/sema.tr"
                if ((_tr_str_eqv((pc_ctn), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("RwLock", 6LL))))) {
#line 5229 "src/sema.tr"
                    Sema_set_borrows_region(self, name, _tr_str_lit_len("@borrowed", 9LL));
                } else if (((((_tr_str_eqv((pc_ctn), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((pc_ctn), (_tr_str_lit_len("Set", 3LL))))) {
#line 5231 "src/sema.tr"
                    TrStr pc_elem_ty = _tr_str_retain(hir_expr_type(hval)->name);
#line 5234 "src/sema.tr"
                    bool pc_is_vt = (_tr_dict_contains(self->classes, _tr_strz(pc_elem_ty)) && (!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(pc_elem_ty)))->is_class));
#line 5235 "src/sema.tr"
                    if (((!Sema_is_primitive(self, hir_expr_type(hval))) && (!pc_is_vt))) {
#line 5236 "src/sema.tr"
                        _tr_dict_set(self->container_borrows, _tr_strz(pc_cont_nm), _tr_str_box(_tr_str_retain(name)));
#line 5240 "src/sema.tr"
                        if (((((((!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("str", 3LL)))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Vec", 3LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Dict", 4LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Map", 3LL))))) && (!_tr_str_eqv((pc_elem_ty), (_tr_str_lit_len("Set", 3LL)))))) {
#line 5241 "src/sema.tr"
                            Sema_set_borrows_region(self, name, _tr_str_lit_len("@borrowed", 9LL));
                        }
                    }
                }
            }
        }
#line 5242 "src/sema.tr"
        if (((((unsigned long long)(hval)) != ((unsigned long long)(0LL))) && (!_is_invalid_ptr(((unsigned long long)(hval)))))) {
#line 5243 "src/sema.tr"
            __auto_type _t1477 = (*hval);
            if (_t1477.tag == HirExpr_EClosure) {
#line 5245 "src/sema.tr"
                List_TrStr_append(self->clo_let_names, name);
#line 5246 "src/sema.tr"
                List_ptr_append(self->clo_let_ptrs, hval);
#line 5247 "src/sema.tr"
                List_i64_append(self->clo_let_scopes, (self->scopes->len - 1LL));
            } else if (1) {
                __auto_type _ = _t1477;
                /* pass */
            }
        }
#line 5249 "src/sema.tr"
        hval = Sema_retype_none(self, hval, ty);
#line 5250 "src/sema.tr"
        HirStmt* _trret231 = box_hirstmt(HirStmt_ctor_SLet(name, ownership, is_mut, is_const, is_shared, ty, hval));
        _tr_str_release(_let_firm);
        return _trret231;
    } else if (_t1432.tag == Stmt_SAssign) {
        __auto_type target = _t1432.data.SAssign.target;
__auto_type val = _t1432.data.SAssign.val;
#line 5256 "src/sema.tr"
        if (((((unsigned long long)(target)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
#line 5257 "src/sema.tr"
            __auto_type _t1478 = (*target);
            if (_t1478.tag == Expr_EIdent) {
                __auto_type cl_decl_name = _t1478.data.EIdent.name;
#line 5259 "src/sema.tr"
                if (_tr_str_eqv((Sema_resolve(self, cl_decl_name)->name), (_tr_str_lit_len("", 0LL)))) {
#line 5260 "src/sema.tr"
                    __auto_type _t1479 = (*val);
                    if (_t1479.tag == Expr_EClosure) {
#line 5262 "src/sema.tr"
                        HirStmt* _trret232 = ({ __auto_type _ord_t1480 = (Ownership_make_Own()); __auto_type _ord_t1481 = ((AstType**)(0LL)); __auto_type _wr = (Sema_lower_stmt(self, box_astestmt(Stmt_ctor_SLet(cl_decl_name, _ord_t1480, false, false, false, _ord_t1481, val)))); _wr; });
                        return _trret232;
                    } else if (1) {
                        __auto_type _ = _t1479;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1478;
                /* pass */
            }
        }
#line 5265 "src/sema.tr"
        self->in_assign_target = true;
#line 5266 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5267 "src/sema.tr"
            __auto_type _t1482 = (*target);
            if (_t1482.tag == Expr_EIdent) {
                __auto_type _ = _t1482.data.EIdent.name;
                self->in_assign_target_plain = true;
            } else if (1) {
                __auto_type _ = _t1482;
                /* pass */
            }
        }
#line 5270 "src/sema.tr"
        HirExpr* htgt = Sema_lower_expr(self, target);
#line 5271 "src/sema.tr"
        self->in_assign_target = false;
#line 5272 "src/sema.tr"
        self->in_assign_target_plain = false;
#line 5273 "src/sema.tr"
        HirExpr* hv = Sema_lower_expr(self, val);
#line 5274 "src/sema.tr"
        hv = Sema__patch_empty_dict_hint(self, hv, hir_expr_type(htgt));
#line 5275 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5276 "src/sema.tr"
            __auto_type _t1483 = (*htgt);
            if (_t1483.tag == HirExpr_EPropAccess) {
                __auto_type _fo = _t1483.data.EPropAccess.obj;
__auto_type _fname = _t1483.data.EPropAccess.prop;
#line 5278 "src/sema.tr"
                AstType* _fo_ty = hir_expr_type(_fo);
#line 5279 "src/sema.tr"
                if (((_tr_dict_contains(self->classes, _tr_strz(_fo_ty->name)) && (!_tr_dict_contains(self->ambiguous_classes, _tr_strz(_fo_ty->name)))) && (!Sema_is_builtin_dispatch_class(self, _fo_ty->name)))) {
#line 5280 "src/sema.tr"
                    ClassDef* _fo_cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_fo_ty->name)));
#line 5281 "src/sema.tr"
                    if ((_fo_cd->generics->len == 0LL)) {
#line 5282 "src/sema.tr"
                        long long _fo_i = 0LL;
#line 5283 "src/sema.tr"
                        while ((_fo_i < _fo_cd->fields->len)) {
#line 5284 "src/sema.tr"
                            if ((_tr_str_eqv((((FieldDef*)List_ptr_get(_fo_cd->fields, _fo_i))->name), (_fname)) && (((unsigned long long)(((FieldDef*)List_ptr_get(_fo_cd->fields, _fo_i))->ty)) != ((unsigned long long)(0LL))))) {
#line 5285 "src/sema.tr"
                                ({ __auto_type _ord_t1484 = ((*((FieldDef*)List_ptr_get(_fo_cd->fields, _fo_i))->ty)); TrStr _ord_t1485 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("the field '", 11LL)), (_fname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_fo_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1484, hv, _ord_t1485); _tr_str_release(_ord_t1485); });
                            }
#line 5286 "src/sema.tr"
                            _fo_i = (_fo_i + 1LL);
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1483;
                /* pass */
            }
#line 5288 "src/sema.tr"
            __auto_type _t1486 = (*target);
            if (_t1486.tag == Expr_EIdent) {
                __auto_type _as_n = _t1486.data.EIdent.name;
#line 5290 "src/sema.tr"
                Symbol* _as_sym = Sema_resolve(self, _as_n);
#line 5291 "src/sema.tr"
                if ((((!_tr_str_eqv((_as_sym->name), (_tr_str_lit_len("", 0LL)))) && (_as_sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!_tr_str_eqv((_as_sym->firm), (_tr_str_lit_len("", 0LL)))))) {
#line 5292 "src/sema.tr"
                    TrStr _as_af = Sema_firm_family(self, hv);
#line 5293 "src/sema.tr"
                    if (Sema_family_mismatch(self, _as_sym->firm, _as_af)) {
#line 5294 "src/sema.tr"
                        ({ __auto_type _ord_t1487 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (_as_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' holds ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _as_sym->firm)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but is assigned ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1488 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1487); TrStr _tr_cr = (Sema_family_word(self, _as_af)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: assign a value of the same type, or declare a new variable.", 72LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1488); _tr_str_release(_at_t1488); });
                    }
                }
            } else if (1) {
                __auto_type _ = _t1486;
                /* pass */
            }
        }
#line 5298 "src/sema.tr"
        if ((self->strict_mode && (((unsigned long long)(target)) != ((unsigned long long)(0LL))))) {
#line 5299 "src/sema.tr"
            __auto_type _t1489 = (*target);
            if (_t1489.tag == Expr_EPropAccess) {
                __auto_type l5_obj = _t1489.data.EPropAccess.obj;
__auto_type l5_field = _t1489.data.EPropAccess.prop;
#line 5301 "src/sema.tr"
                if ((({ TrStr _cmpa_t1490 = (Sema_compute_region(self, val)); TrStr _cmpb_t1491 = (_tr_str_lit_len("@owned", 6LL)); _Bool _cmpr_t1492 = _tr_str_eqv(_cmpa_t1490, _cmpb_t1491); _tr_str_release(_cmpa_t1490); _cmpr_t1492; }) && Sema_field_is_borrow(self, l5_obj, l5_field))) {
#line 5302 "src/sema.tr"
                    ({ TrStr _at_t1493 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-5] storing a freshly-built (owned) string into the borrow field '", 68LL)), (l5_field))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (declared 'ref').\n      FIX: store a borrow of the field's region, or make the field a plain owned 'str'.", 107LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1493); _tr_str_release(_at_t1493); });
                }
            } else if (1) {
                __auto_type _ = _t1489;
                /* pass */
            }
        }
#line 5307 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5308 "src/sema.tr"
            __auto_type _t1494 = (*target);
            if (_t1494.tag == Expr_EIdent) {
                __auto_type sa_decl_name = _t1494.data.EIdent.name;
#line 5313 "src/sema.tr"
                ({ TrStr _at_t1495 = (Sema_compute_region(self, val)); Sema_set_borrows_region(self, sa_decl_name, _at_t1495); _tr_str_release(_at_t1495); });
#line 5314 "src/sema.tr"
                Symbol* sa_existing = Sema_resolve(self, sa_decl_name);
#line 5315 "src/sema.tr"
                if (_tr_str_eqv((sa_existing->name), (_tr_str_lit_len("", 0LL)))) {
#line 5318 "src/sema.tr"
                    ({ __auto_type _ord_t1496 = (SymbolKind_make_SVariable()); __auto_type _ord_t1497 = (box_asttype(hir_expr_type(hv))); Sema_declare(self, sa_decl_name, _ord_t1496, _ord_t1497, false); });
#line 5322 "src/sema.tr"
                    if ((_tr_str_eqv((hir_expr_type(htgt)->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((hir_expr_type(htgt)->name), (_tr_str_lit_len("", 0LL))))) {
#line 5323 "src/sema.tr"
                        htgt = box_hirexpr(HirExpr_ctor_EIdent(sa_decl_name, hir_expr_type(hv), false));
                    }
                } else if ((((self->scopes->len > 0LL) && Sema_is_global_not_local(self, sa_decl_name)) && (!sa_existing->is_mut))) {
#line 5332 "src/sema.tr"
                    ({ __auto_type _ord_t1498 = (SymbolKind_make_SVariable()); __auto_type _ord_t1499 = (box_asttype(hir_expr_type(hv))); Sema_declare(self, sa_decl_name, _ord_t1498, _ord_t1499, true); });
#line 5333 "src/sema.tr"
                    Sema_mark_init(self, sa_decl_name);
#line 5334 "src/sema.tr"
                    HirStmt* _trret233 = ({ __auto_type _ord_t1500 = (Ownership_make_Own()); __auto_type _ord_t1501 = (hir_expr_type(hv)); __auto_type _wr = (box_hirstmt(HirStmt_ctor_SLet(sa_decl_name, _ord_t1500, true, false, false, _ord_t1501, hv))); _wr; });
                    return _trret233;
                } else if (((sa_existing->kind.tag == SymbolKind_make_SVariable().tag) && (!sa_existing->is_mut))) {
#line 5336 "src/sema.tr"
                    ({ TrStr _at_t1502 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-8] Cannot assign to '", 24LL)), (sa_decl_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' a second time because it is immutable.\n      FIX: Declare it as 'mut ", 71LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sa_decl_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ...' if it needs to change.", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1502); _tr_str_release(_at_t1502); });
                }
            } else if (1) {
                __auto_type _ = _t1494;
                /* pass */
            }
        }
#line 5351 "src/sema.tr"
        bool sa_target_is_plain_ident = false;
#line 5352 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5353 "src/sema.tr"
            __auto_type _t1503 = (*target);
            if (_t1503.tag == Expr_EIdent) {
                __auto_type _ = _t1503.data.EIdent.name;
                sa_target_is_plain_ident = true;
            } else if (1) {
                __auto_type _ = _t1503;
                /* pass */
            }
        }
#line 5356 "src/sema.tr"
        if ((sa_target_is_plain_ident && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
#line 5357 "src/sema.tr"
            __auto_type _t1504 = (*val);
            if (_t1504.tag == Expr_EIdent) {
                __auto_type sa_src = _t1504.data.EIdent.name;
#line 5359 "src/sema.tr"
                Symbol* sa_sym = Sema_resolve(self, sa_src);
#line 5360 "src/sema.tr"
                AstType* sa_ty = (*sa_sym->ty);
#line 5361 "src/sema.tr"
                bool sa_is_known = (_tr_dict_contains(self->classes, _tr_strz(sa_ty->name)) || _tr_dict_contains(self->enums, _tr_strz(sa_ty->name)));
#line 5362 "src/sema.tr"
                if (((((sa_is_known && (!Sema_is_primitive(self, sa_ty))) && (!Sema_is_copy_class(self, sa_ty->name))) && (!_tr_str_eqv((sa_sym->name), (_tr_str_lit_len("", 0LL))))) && (!sa_sym->is_shared))) {
#line 5363 "src/sema.tr"
                    Sema_check_no_active_borrows(self, sa_src, sa_ty->name);
#line 5364 "src/sema.tr"
                    Sema_mark_moved(self, sa_src);
                } else if ((((sa_is_known && Sema_class_has_free(self, sa_ty->name)) && (!_tr_str_eqv((sa_sym->name), (_tr_str_lit_len("", 0LL))))) && (!sa_sym->is_shared))) {
#line 5366 "src/sema.tr"
                    Sema_mark_moved(self, sa_src);
                }
            } else if (1) {
                __auto_type _ = _t1504;
                /* pass */
            }
        }
#line 5369 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5370 "src/sema.tr"
            __auto_type _t1505 = (*target);
            if (_t1505.tag == Expr_EIdent) {
                __auto_type pd_tgt = _t1505.data.EIdent.name;
#line 5372 "src/sema.tr"
                Sema_mark_init(self, pd_tgt);
#line 5373 "src/sema.tr"
                Sema_revive_moved(self, pd_tgt);
            } else if (1) {
                __auto_type _ = _t1505;
                /* pass */
            }
        }
#line 5376 "src/sema.tr"
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
#line 5377 "src/sema.tr"
            __auto_type _t1506 = (*target);
            if (_t1506.tag == Expr_EIdent) {
                __auto_type pc_tgt = _t1506.data.EIdent.name;
                Sema_clear_container_borrow(self, pc_tgt);
            } else if (1) {
                __auto_type _ = _t1506;
                /* pass */
            }
        }
#line 5380 "src/sema.tr"
        hv = Sema_retype_none(self, hv, hir_expr_type(htgt));
#line 5381 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SAssign(htgt, hv));
    } else if (_t1432.tag == Stmt_SIf) {
        __auto_type cond = _t1432.data.SIf.cond;
__auto_type then_b = _t1432.data.SIf.then_b;
__auto_type elifs = _t1432.data.SIf.elifs;
__auto_type else_b = _t1432.data.SIf.else_b;
#line 5384 "src/sema.tr"
        if ((((unsigned long long)(cond)) != ((unsigned long long)(0LL)))) {
#line 5385 "src/sema.tr"
            __auto_type _t1507 = (*cond);
            if (_t1507.tag == Expr_EIdent) {
                __auto_type t5_name = _t1507.data.EIdent.name;
#line 5387 "src/sema.tr"
                Symbol* t5_sym = Sema_resolve(self, t5_name);
#line 5388 "src/sema.tr"
                TrStr t5_ty = _tr_str_retain((*t5_sym->ty)->name);
#line 5389 "src/sema.tr"
                if (((((_tr_str_eqv((t5_ty), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((t5_ty), (_tr_str_lit_len("f64", 3LL))))) {
#line 5390 "src/sema.tr"
                    ({ TrStr _at_t1508 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-5] '", 7LL)), (t5_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a number (", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t5_ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") and cannot be used as an 'if' condition. FIX: Write 'if ", 58LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (t5_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" != 0:' to explicitly check for non-zero.", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1508); _tr_str_release(_at_t1508); });
                }
                _tr_str_release(t5_ty);
            } else if (1) {
                __auto_type _ = _t1507;
                /* pass */
            }
        }
#line 5393 "src/sema.tr"
        bool si_outer_cap_m = self->capturing_moves;
#line 5394 "src/sema.tr"
        List_TrStr* si_outer_buf_m = self->branch_moved_buf;
#line 5395 "src/sema.tr"
        bool si_outer_cap_i = self->capturing_inits;
#line 5396 "src/sema.tr"
        List_TrStr* si_outer_buf_i = self->branch_init_buf;
#line 5398 "src/sema.tr"
        self->capturing_moves = true;
#line 5399 "src/sema.tr"
        self->capturing_inits = true;
#line 5400 "src/sema.tr"
        self->branch_moved_buf = (void*)List_TrStr_new();
#line 5401 "src/sema.tr"
        self->branch_init_buf = (void*)List_TrStr_new();
#line 5402 "src/sema.tr"
        HirExpr* hcond = Sema_lower_expr(self, cond);
#line 5403 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5404 "src/sema.tr"
        Sema_open_block(self);
#line 5405 "src/sema.tr"
        HirBlock* hthen = Sema_lower_block(self, then_b);
#line 5406 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5407 "src/sema.tr"
        Sema_close_block(self);
#line 5408 "src/sema.tr"
        List_TrStr* si_then_moved = self->branch_moved_buf;
#line 5409 "src/sema.tr"
        List_TrStr* si_then_inited = self->branch_init_buf;
#line 5411 "src/sema.tr"
        long long si_uti = 0LL;
#line 5412 "src/sema.tr"
        while ((si_uti < si_then_moved->len)) {
#line 5413 "src/sema.tr"
            ({ TrStr _at_t1509 = (List_TrStr_get(si_then_moved, si_uti)); Sema_unmark_moved(self, _at_t1509); _tr_str_release(_at_t1509); });
#line 5414 "src/sema.tr"
            si_uti = (si_uti + 1LL);
        }
#line 5415 "src/sema.tr"
        long long si_uii = 0LL;
#line 5416 "src/sema.tr"
        while ((si_uii < si_then_inited->len)) {
#line 5417 "src/sema.tr"
            ({ TrStr _at_t1510 = (List_TrStr_get(si_then_inited, si_uii)); Sema_unmark_init(self, _at_t1510); _tr_str_release(_at_t1510); });
#line 5418 "src/sema.tr"
            si_uii = (si_uii + 1LL);
        }
#line 5420 "src/sema.tr"
        self->branch_moved_buf = (void*)List_TrStr_new();
#line 5421 "src/sema.tr"
        self->branch_init_buf = (void*)List_TrStr_new();
#line 5422 "src/sema.tr"
        HirBlock* helse = HirBlock_init();
#line 5423 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5424 "src/sema.tr"
        Sema_open_block(self);
#line 5425 "src/sema.tr"
        if ((elifs->len > 0LL)) {
#line 5435 "src/sema.tr"
            Sema_open_block(self);
#line 5436 "src/sema.tr"
            HirBlock* base_else = Sema_lower_block(self, else_b);
#line 5437 "src/sema.tr"
            Sema_close_block(self);
#line 5438 "src/sema.tr"
            HirBlock* chain = _tr_obj_retain(base_else);
#line 5439 "src/sema.tr"
            long long k = (elifs->len - 1LL);
#line 5440 "src/sema.tr"
            while ((k >= 0LL)) {
#line 5441 "src/sema.tr"
                ElifClause* elif_c = ((ElifClause*)List_ptr_get(elifs, k));
#line 5442 "src/sema.tr"
                HirExpr* elif_cond = Sema_lower_expr(self, elif_c->cond);
#line 5443 "src/sema.tr"
                Sema_open_block(self);
#line 5444 "src/sema.tr"
                HirBlock* elif_body = Sema_lower_block(self, (*elif_c->body));
#line 5445 "src/sema.tr"
                Sema_close_block(self);
#line 5446 "src/sema.tr"
                HirBlock* nested = HirBlock_init();
#line 5447 "src/sema.tr"
                HirBlock_push(nested, box_hirstmt(HirStmt_ctor_SIf(elif_cond, elif_body, chain)));
#line 5448 "src/sema.tr"
                HirBlock* _cltmp_t1511 = _tr_obj_retain(nested);
                _tr_obj_release(chain, _trdrop_HirBlock);
                chain = _cltmp_t1511;
#line 5449 "src/sema.tr"
                k = (k - 1LL);
                _tr_obj_release(elif_body, _trdrop_HirBlock);
                _tr_obj_release(nested, _trdrop_HirBlock);
            }
#line 5450 "src/sema.tr"
            HirBlock* _cltmp_t1512 = _tr_obj_retain(chain);
            _tr_obj_release(helse, _trdrop_HirBlock);
            helse = _cltmp_t1512;
            _tr_obj_release(base_else, _trdrop_HirBlock);
        } else {
#line 5452 "src/sema.tr"
            HirBlock* _cltmp_t1513 = Sema_lower_block(self, else_b);
            _tr_obj_release(helse, _trdrop_HirBlock);
            helse = _cltmp_t1513;
        }
#line 5453 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5454 "src/sema.tr"
        Sema_close_block(self);
#line 5455 "src/sema.tr"
        List_TrStr* si_else_moved = self->branch_moved_buf;
#line 5456 "src/sema.tr"
        List_TrStr* si_else_inited = self->branch_init_buf;
#line 5458 "src/sema.tr"
        long long si_uei = 0LL;
#line 5459 "src/sema.tr"
        while ((si_uei < si_else_moved->len)) {
#line 5460 "src/sema.tr"
            ({ TrStr _at_t1514 = (List_TrStr_get(si_else_moved, si_uei)); Sema_unmark_moved(self, _at_t1514); _tr_str_release(_at_t1514); });
#line 5461 "src/sema.tr"
            si_uei = (si_uei + 1LL);
        }
#line 5462 "src/sema.tr"
        long long si_uei2 = 0LL;
#line 5463 "src/sema.tr"
        while ((si_uei2 < si_else_inited->len)) {
#line 5464 "src/sema.tr"
            ({ TrStr _at_t1515 = (List_TrStr_get(si_else_inited, si_uei2)); Sema_unmark_init(self, _at_t1515); _tr_str_release(_at_t1515); });
#line 5465 "src/sema.tr"
            si_uei2 = (si_uei2 + 1LL);
        }
#line 5467 "src/sema.tr"
        self->capturing_moves = si_outer_cap_m;
#line 5468 "src/sema.tr"
        self->branch_moved_buf = si_outer_buf_m;
#line 5469 "src/sema.tr"
        self->capturing_inits = si_outer_cap_i;
#line 5470 "src/sema.tr"
        self->branch_init_buf = si_outer_buf_i;
#line 5476 "src/sema.tr"
        bool si_then_jumps = Sema_block_ends_in_jump(self, hthen);
#line 5477 "src/sema.tr"
        bool si_else_jumps = Sema_block_ends_in_jump(self, helse);
#line 5478 "src/sema.tr"
        if ((si_then_jumps && (!si_else_jumps))) {
#line 5479 "src/sema.tr"
            long long si_ei0 = 0LL;
#line 5480 "src/sema.tr"
            while ((si_ei0 < si_else_moved->len)) {
#line 5481 "src/sema.tr"
                ({ TrStr _at_t1516 = (List_TrStr_get(si_else_moved, si_ei0)); Sema_mark_moved(self, _at_t1516); _tr_str_release(_at_t1516); });
#line 5482 "src/sema.tr"
                si_ei0 = (si_ei0 + 1LL);
            }
#line 5483 "src/sema.tr"
            long long si_ini0 = 0LL;
#line 5484 "src/sema.tr"
            while ((si_ini0 < si_else_inited->len)) {
#line 5485 "src/sema.tr"
                ({ TrStr _at_t1517 = (List_TrStr_get(si_else_inited, si_ini0)); Sema_mark_init(self, _at_t1517); _tr_str_release(_at_t1517); });
#line 5486 "src/sema.tr"
                si_ini0 = (si_ini0 + 1LL);
            }
        } else if ((si_else_jumps && (!si_then_jumps))) {
#line 5488 "src/sema.tr"
            long long si_mi0 = 0LL;
#line 5489 "src/sema.tr"
            while ((si_mi0 < si_then_moved->len)) {
#line 5490 "src/sema.tr"
                ({ TrStr _at_t1518 = (List_TrStr_get(si_then_moved, si_mi0)); Sema_mark_moved(self, _at_t1518); _tr_str_release(_at_t1518); });
#line 5491 "src/sema.tr"
                si_mi0 = (si_mi0 + 1LL);
            }
#line 5492 "src/sema.tr"
            long long si_ti0 = 0LL;
#line 5493 "src/sema.tr"
            while ((si_ti0 < si_then_inited->len)) {
#line 5494 "src/sema.tr"
                ({ TrStr _at_t1519 = (List_TrStr_get(si_then_inited, si_ti0)); Sema_mark_init(self, _at_t1519); _tr_str_release(_at_t1519); });
#line 5495 "src/sema.tr"
                si_ti0 = (si_ti0 + 1LL);
            }
        } else if ((si_then_jumps && si_else_jumps)) {
#line 5497 "src/sema.tr"
            /* pass */
        } else {
#line 5500 "src/sema.tr"
            long long si_mi = 0LL;
#line 5501 "src/sema.tr"
            while ((si_mi < si_then_moved->len)) {
#line 5502 "src/sema.tr"
                TrStr si_mn = List_TrStr_get(si_then_moved, si_mi);
#line 5503 "src/sema.tr"
                if (Sema_vec_str_contains(self, si_else_moved, si_mn)) {
#line 5504 "src/sema.tr"
                    Sema_mark_moved(self, si_mn);
                } else {
#line 5506 "src/sema.tr"
                    Sema_mark_maybe_moved(self, si_mn);
                }
#line 5507 "src/sema.tr"
                si_mi = (si_mi + 1LL);
                _tr_str_release(si_mn);
            }
#line 5508 "src/sema.tr"
            long long si_ei = 0LL;
#line 5509 "src/sema.tr"
            while ((si_ei < si_else_moved->len)) {
#line 5510 "src/sema.tr"
                TrStr si_en = List_TrStr_get(si_else_moved, si_ei);
#line 5511 "src/sema.tr"
                if ((!Sema_vec_str_contains(self, si_then_moved, si_en))) {
#line 5512 "src/sema.tr"
                    Sema_mark_maybe_moved(self, si_en);
                }
#line 5513 "src/sema.tr"
                si_ei = (si_ei + 1LL);
                _tr_str_release(si_en);
            }
#line 5515 "src/sema.tr"
            long long si_ini = 0LL;
#line 5516 "src/sema.tr"
            while ((si_ini < si_then_inited->len)) {
#line 5517 "src/sema.tr"
                TrStr si_inn = List_TrStr_get(si_then_inited, si_ini);
#line 5518 "src/sema.tr"
                if (Sema_vec_str_contains(self, si_else_inited, si_inn)) {
#line 5519 "src/sema.tr"
                    Sema_mark_init(self, si_inn);
                } else {
#line 5521 "src/sema.tr"
                    Sema_mark_maybe_init(self, si_inn);
                }
#line 5522 "src/sema.tr"
                si_ini = (si_ini + 1LL);
                _tr_str_release(si_inn);
            }
        }
#line 5523 "src/sema.tr"
        HirStmt* _trret234 = box_hirstmt(HirStmt_ctor_SIf(hcond, hthen, helse));
        _tr_obj_release(hthen, _trdrop_HirBlock);
        _tr_obj_release(helse, _trdrop_HirBlock);
        return _trret234;
    } else if (_t1432.tag == Stmt_SWhile) {
        __auto_type cond = _t1432.data.SWhile.cond;
__auto_type body = _t1432.data.SWhile.body;
__auto_type decorators = _t1432.data.SWhile.decorators;
#line 5525 "src/sema.tr"
        HirExpr* sw_cond = Sema_lower_expr(self, cond);
#line 5527 "src/sema.tr"
        bool sw_outer_cap_m = self->capturing_moves;
#line 5528 "src/sema.tr"
        List_TrStr* sw_outer_buf_m = self->branch_moved_buf;
#line 5529 "src/sema.tr"
        bool sw_outer_cap_i = self->capturing_inits;
#line 5530 "src/sema.tr"
        List_TrStr* sw_outer_buf_i = self->branch_init_buf;
#line 5531 "src/sema.tr"
        self->capturing_moves = true;
#line 5532 "src/sema.tr"
        self->capturing_inits = true;
#line 5533 "src/sema.tr"
        self->branch_moved_buf = (void*)List_TrStr_new();
#line 5534 "src/sema.tr"
        self->branch_init_buf = (void*)List_TrStr_new();
#line 5538 "src/sema.tr"
        List_i64_append(self->loop_scope_base, self->scopes->len);
#line 5539 "src/sema.tr"
        List_i64_append(self->loop_blk_floor, self->block_stack->len);
#line 5540 "src/sema.tr"
        List_i64_append(self->loop_blk_scope, (self->scopes->len - 1LL));
#line 5541 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5542 "src/sema.tr"
        Sema_open_block(self);
#line 5543 "src/sema.tr"
        long long sw_block_id = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
#line 5544 "src/sema.tr"
        HirBlock* sw_body = Sema_lower_block(self, body);
#line 5545 "src/sema.tr"
        Sema_append_block_local_drops(self, sw_body, sw_block_id);
#line 5546 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5547 "src/sema.tr"
        Sema_close_block(self);
#line 5548 "src/sema.tr"
        List_i64_pop(self->loop_scope_base);
#line 5549 "src/sema.tr"
        List_i64_pop(self->loop_blk_floor);
#line 5550 "src/sema.tr"
        List_i64_pop(self->loop_blk_scope);
#line 5551 "src/sema.tr"
        List_TrStr* sw_loop_moved = self->branch_moved_buf;
#line 5552 "src/sema.tr"
        List_TrStr* sw_loop_inited = self->branch_init_buf;
#line 5553 "src/sema.tr"
        self->capturing_moves = sw_outer_cap_m;
#line 5554 "src/sema.tr"
        self->branch_moved_buf = sw_outer_buf_m;
#line 5555 "src/sema.tr"
        self->capturing_inits = sw_outer_cap_i;
#line 5556 "src/sema.tr"
        self->branch_init_buf = sw_outer_buf_i;
#line 5557 "src/sema.tr"
        long long sw_mi = 0LL;
#line 5558 "src/sema.tr"
        while ((sw_mi < sw_loop_moved->len)) {
#line 5559 "src/sema.tr"
            ({ TrStr _at_t1520 = (List_TrStr_get(sw_loop_moved, sw_mi)); Sema_unmark_moved(self, _at_t1520); _tr_str_release(_at_t1520); });
#line 5560 "src/sema.tr"
            ({ TrStr _at_t1521 = (List_TrStr_get(sw_loop_moved, sw_mi)); Sema_mark_maybe_moved(self, _at_t1521); _tr_str_release(_at_t1521); });
#line 5561 "src/sema.tr"
            sw_mi = (sw_mi + 1LL);
        }
#line 5562 "src/sema.tr"
        long long sw_ii = 0LL;
#line 5563 "src/sema.tr"
        while ((sw_ii < sw_loop_inited->len)) {
#line 5564 "src/sema.tr"
            ({ TrStr _at_t1522 = (List_TrStr_get(sw_loop_inited, sw_ii)); Sema_unmark_init(self, _at_t1522); _tr_str_release(_at_t1522); });
#line 5565 "src/sema.tr"
            ({ TrStr _at_t1523 = (List_TrStr_get(sw_loop_inited, sw_ii)); Sema_mark_maybe_init(self, _at_t1523); _tr_str_release(_at_t1523); });
#line 5566 "src/sema.tr"
            sw_ii = (sw_ii + 1LL);
        }
#line 5567 "src/sema.tr"
        HirStmt* _trret235 = box_hirstmt(HirStmt_ctor_SWhile(sw_cond, sw_body));
        _tr_obj_release(sw_body, _trdrop_HirBlock);
        return _trret235;
    } else if (_t1432.tag == Stmt_SFor) {
        __auto_type var = _t1432.data.SFor.var;
__auto_type iter = _t1432.data.SFor.iter;
__auto_type body = _t1432.data.SFor.body;
__auto_type decorators = _t1432.data.SFor.decorators;
__auto_type for_is_ref = _t1432.data.SFor.is_ref;
#line 5569 "src/sema.tr"
        Sema_enter_scope(self);
#line 5570 "src/sema.tr"
        HirExpr* h_iter_for = Sema_lower_expr(self, iter);
#line 5571 "src/sema.tr"
        AstType* var_ty_for = AstType_init(_tr_str_lit_len("int", 3LL));
#line 5572 "src/sema.tr"
        TrStr iter_hn = _tr_str_retain(hir_expr_type(h_iter_for)->name);
#line 5573 "src/sema.tr"
        long long iter_hal = hir_expr_type(h_iter_for)->args->len;
#line 5574 "src/sema.tr"
        if (((_tr_str_eqv((iter_hn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((iter_hn), (_tr_str_lit_len("Vec", 3LL)))) && (iter_hal > 0LL))) {
#line 5575 "src/sema.tr"
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if ((_tr_str_eqv((iter_hn), (_tr_str_lit_len("Chan", 4LL))) && (iter_hal > 0LL))) {
#line 5577 "src/sema.tr"
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if (_tr_str_eqv((iter_hn), (_tr_str_lit_len("str", 3LL)))) {
#line 5579 "src/sema.tr"
            var_ty_for = AstType_init(_tr_str_lit_len("char", 4LL));
        }
#line 5580 "src/sema.tr"
        ({ __auto_type _ord_t1524 = (SymbolKind_make_SVariable()); __auto_type _ord_t1525 = (box_asttype(var_ty_for)); Sema_declare(self, var, _ord_t1524, _ord_t1525, false); });
#line 5585 "src/sema.tr"
        if (((_tr_str_eqv((iter_hn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((iter_hn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((iter_hn), (_tr_str_lit_len("Set", 3LL))))) {
#line 5586 "src/sema.tr"
            TrStr lv_n = _tr_str_retain(var_ty_for->name);
#line 5587 "src/sema.tr"
            if (((((_tr_str_eqv((lv_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((lv_n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((lv_n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((lv_n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((lv_n), (_tr_str_lit_len("Set", 3LL))))) {
#line 5588 "src/sema.tr"
                Sema_set_borrows_region(self, var, _tr_str_lit_len("@borrowed", 9LL));
            }
        }
#line 5592 "src/sema.tr"
        if (for_is_ref) {
#line 5593 "src/sema.tr"
            __auto_type _t1526 = (*iter);
            if (_t1526.tag == Expr_EIdent) {
                __auto_type for_coll = _t1526.data.EIdent.name;
#line 5595 "src/sema.tr"
                List_TrStr_append(self->cur_func_borrowers, var);
#line 5596 "src/sema.tr"
                List_TrStr_append(self->cur_func_sources, for_coll);
            } else if (1) {
                __auto_type _ = _t1526;
                /* pass */
            }
        }
#line 5599 "src/sema.tr"
        bool sf_outer_cap_m = self->capturing_moves;
#line 5600 "src/sema.tr"
        List_TrStr* sf_outer_buf_m = self->branch_moved_buf;
#line 5601 "src/sema.tr"
        bool sf_outer_cap_i = self->capturing_inits;
#line 5602 "src/sema.tr"
        List_TrStr* sf_outer_buf_i = self->branch_init_buf;
#line 5603 "src/sema.tr"
        self->capturing_moves = true;
#line 5604 "src/sema.tr"
        self->capturing_inits = true;
#line 5605 "src/sema.tr"
        self->branch_moved_buf = (void*)List_TrStr_new();
#line 5606 "src/sema.tr"
        self->branch_init_buf = (void*)List_TrStr_new();
#line 5607 "src/sema.tr"
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
#line 5608 "src/sema.tr"
        List_i64_append(self->loop_blk_floor, (-1LL));
#line 5609 "src/sema.tr"
        List_i64_append(self->loop_blk_scope, (-1LL));
#line 5610 "src/sema.tr"
        HirBlock* sf_body = Sema_lower_block(self, body);
#line 5611 "src/sema.tr"
        List_i64_pop(self->loop_scope_base);
#line 5612 "src/sema.tr"
        List_i64_pop(self->loop_blk_floor);
#line 5613 "src/sema.tr"
        List_i64_pop(self->loop_blk_scope);
#line 5614 "src/sema.tr"
        List_TrStr* sf_loop_moved = self->branch_moved_buf;
#line 5615 "src/sema.tr"
        List_TrStr* sf_loop_inited = self->branch_init_buf;
#line 5616 "src/sema.tr"
        self->capturing_moves = sf_outer_cap_m;
#line 5617 "src/sema.tr"
        self->branch_moved_buf = sf_outer_buf_m;
#line 5618 "src/sema.tr"
        self->capturing_inits = sf_outer_cap_i;
#line 5619 "src/sema.tr"
        self->branch_init_buf = sf_outer_buf_i;
#line 5620 "src/sema.tr"
        long long sf_mi = 0LL;
#line 5621 "src/sema.tr"
        while ((sf_mi < sf_loop_moved->len)) {
#line 5622 "src/sema.tr"
            ({ TrStr _at_t1527 = (List_TrStr_get(sf_loop_moved, sf_mi)); Sema_unmark_moved(self, _at_t1527); _tr_str_release(_at_t1527); });
#line 5623 "src/sema.tr"
            ({ TrStr _at_t1528 = (List_TrStr_get(sf_loop_moved, sf_mi)); Sema_mark_maybe_moved(self, _at_t1528); _tr_str_release(_at_t1528); });
#line 5624 "src/sema.tr"
            sf_mi = (sf_mi + 1LL);
        }
#line 5625 "src/sema.tr"
        long long sf_ii = 0LL;
#line 5626 "src/sema.tr"
        while ((sf_ii < sf_loop_inited->len)) {
#line 5627 "src/sema.tr"
            ({ TrStr _at_t1529 = (List_TrStr_get(sf_loop_inited, sf_ii)); Sema_unmark_init(self, _at_t1529); _tr_str_release(_at_t1529); });
#line 5628 "src/sema.tr"
            ({ TrStr _at_t1530 = (List_TrStr_get(sf_loop_inited, sf_ii)); Sema_mark_maybe_init(self, _at_t1530); _tr_str_release(_at_t1530); });
#line 5629 "src/sema.tr"
            sf_ii = (sf_ii + 1LL);
        }
#line 5630 "src/sema.tr"
        Sema_finalize_scope_drops(self, sf_body);
#line 5631 "src/sema.tr"
        HirStmt* hstmt = box_hirstmt(HirStmt_ctor_SFor(var, h_iter_for, sf_body));
#line 5632 "src/sema.tr"
        Sema_exit_scope(self);
#line 5633 "src/sema.tr"
        HirStmt* _trret236 = hstmt;
        _tr_str_release(iter_hn);
        _tr_obj_release(sf_body, _trdrop_HirBlock);
        return _trret236;
    } else if (_t1432.tag == Stmt_SForUnpack) {
        __auto_type vars = _t1432.data.SForUnpack.vars;
__auto_type iter = _t1432.data.SForUnpack.iter;
__auto_type body = _t1432.data.SForUnpack.body;
#line 5635 "src/sema.tr"
        Sema_enter_scope(self);
#line 5636 "src/sema.tr"
        HirExpr* h_iter_fu = Sema_lower_expr(self, iter);
#line 5638 "src/sema.tr"
        List_ptr* fu_tys = (void*)List_ptr_new();
#line 5639 "src/sema.tr"
        long long fu_ti = 0LL;
#line 5640 "src/sema.tr"
        while ((fu_ti < vars->len)) {
#line 5641 "src/sema.tr"
            List_ptr_append(fu_tys, AstType_init(_tr_str_lit_len("int", 3LL)));
#line 5642 "src/sema.tr"
            fu_ti = (fu_ti + 1LL);
        }
#line 5643 "src/sema.tr"
        __auto_type _t1531 = (*h_iter_fu);
        if (_t1531.tag == HirExpr_ECall) {
            __auto_type fu_callee = _t1531.data.ECall.callee;
__auto_type fu_args = _t1531.data.ECall.args;
#line 5645 "src/sema.tr"
            __auto_type _t1532 = (*fu_callee);
            if (_t1532.tag == HirExpr_EIdent) {
                __auto_type fu_fn = _t1532.data.EIdent.name;
#line 5647 "src/sema.tr"
                if (((_tr_str_eqv((fu_fn), (_tr_str_lit_len("enumerate", 9LL))) && (fu_args->len == 1LL)) && (vars->len >= 2LL))) {
#line 5649 "src/sema.tr"
                    TrStr fu_col_ty_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name);
#line 5650 "src/sema.tr"
                    long long fu_col_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
#line 5651 "src/sema.tr"
                    if (((_tr_str_eqv((fu_col_ty_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fu_col_ty_n), (_tr_str_lit_len("Vec", 3LL)))) && (fu_col_al > 0LL))) {
#line 5652 "src/sema.tr"
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
                } else if (((_tr_str_eqv((fu_fn), (_tr_str_lit_len("zip", 3LL))) && (fu_args->len == 2LL)) && (vars->len >= 2LL))) {
#line 5655 "src/sema.tr"
                    TrStr fu_a_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name);
#line 5656 "src/sema.tr"
                    long long fu_a_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
#line 5657 "src/sema.tr"
                    TrStr fu_b_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->name);
#line 5658 "src/sema.tr"
                    long long fu_b_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args->len;
#line 5659 "src/sema.tr"
                    if (((_tr_str_eqv((fu_a_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fu_a_n), (_tr_str_lit_len("Vec", 3LL)))) && (fu_a_al > 0LL))) {
#line 5660 "src/sema.tr"
                        List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
#line 5661 "src/sema.tr"
                    if (((_tr_str_eqv((fu_b_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((fu_b_n), (_tr_str_lit_len("Vec", 3LL)))) && (fu_b_al > 0LL))) {
#line 5662 "src/sema.tr"
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args, 0LL))));
                    }
                }
            } else if (1) {
                __auto_type _ = _t1532;
                /* pass */
            }
        } else if (_t1531.tag == HirExpr_EMethodCall) {
            __auto_type fu_obj = _t1531.data.EMethodCall.obj;
__auto_type fu_meth = _t1531.data.EMethodCall.method;
#line 5665 "src/sema.tr"
            if ((_tr_str_eqv((fu_meth), (_tr_str_lit_len("items", 5LL))) && (vars->len >= 2LL))) {
#line 5666 "src/sema.tr"
                TrStr fu_dty_n = _tr_str_retain(hir_expr_type(fu_obj)->name);
#line 5667 "src/sema.tr"
                long long fu_dty_al = hir_expr_type(fu_obj)->args->len;
#line 5668 "src/sema.tr"
                if (((_tr_str_eqv((fu_dty_n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((fu_dty_n), (_tr_str_lit_len("Map", 3LL)))) && (fu_dty_al >= 2LL))) {
#line 5669 "src/sema.tr"
                    List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 0LL))));
#line 5670 "src/sema.tr"
                    List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 1LL))));
                }
            }
        } else if (1) {
            __auto_type _ = _t1531;
            /* pass */
        }
#line 5672 "src/sema.tr"
        long long vi_fu = 0LL;
#line 5673 "src/sema.tr"
        while ((vi_fu < vars->len)) {
#line 5674 "src/sema.tr"
            ({ TrStr _ord_t1533 = (List_TrStr_get(vars, vi_fu)); __auto_type _ord_t1534 = (SymbolKind_make_SVariable()); __auto_type _ord_t1535 = (box_asttype(((AstType*)List_ptr_get(fu_tys, vi_fu)))); Sema_declare(self, _ord_t1533, _ord_t1534, _ord_t1535, false); _tr_str_release(_ord_t1533); });
#line 5675 "src/sema.tr"
            vi_fu = (vi_fu + 1LL);
        }
#line 5676 "src/sema.tr"
        HirBlock* hblk_fu = Sema_lower_block(self, body);
#line 5677 "src/sema.tr"
        Sema_finalize_scope_drops(self, hblk_fu);
#line 5678 "src/sema.tr"
        Sema_exit_scope(self);
#line 5679 "src/sema.tr"
        HirStmt* _trret237 = box_hirstmt(HirStmt_ctor_SForUnpack(vars, h_iter_fu, hblk_fu));
        List_ptr_free(fu_tys);
        _tr_obj_release(hblk_fu, _trdrop_HirBlock);
        return _trret237;
    } else if (_t1432.tag == Stmt_SMatch) {
        __auto_type subj = _t1432.data.SMatch.expr;
__auto_type arms = _t1432.data.SMatch.arms;
#line 5681 "src/sema.tr"
        HirExpr* hsubj = Sema_lower_expr(self, subj);
#line 5682 "src/sema.tr"
        TrStr ex_ty_name = _tr_str_retain(hir_expr_type(hsubj)->name);
#line 5683 "src/sema.tr"
        List_ptr* h_arms = (void*)List_ptr_new();
#line 5684 "src/sema.tr"
        bool ex_has_wild = false;
#line 5685 "src/sema.tr"
        List_TrStr* ex_covered = (void*)List_TrStr_new();
#line 5686 "src/sema.tr"
        long long k = 0LL;
#line 5687 "src/sema.tr"
        while ((k < arms->len)) {
#line 5688 "src/sema.tr"
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, k));
#line 5691 "src/sema.tr"
            bool ex_guarded = (((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)));
#line 5692 "src/sema.tr"
            __auto_type _t1536 = arm->pat;
            if (_t1536.tag == Pattern_PWild) {
#line 5694 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5694 "src/sema.tr"
                    ex_has_wild = true;
                }
            } else if (_t1536.tag == Pattern_PBind) {
                __auto_type _ = _t1536.data.PBind.name;
#line 5696 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5696 "src/sema.tr"
                    ex_has_wild = true;
                }
            } else if (_t1536.tag == Pattern_PVariant) {
                __auto_type ex_vn = _t1536.data.PVariant.variant;
#line 5698 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5698 "src/sema.tr"
                    List_TrStr_append(ex_covered, ex_vn);
                }
            } else if (_t1536.tag == Pattern_PVariantBind) {
                __auto_type ex_vn2 = _t1536.data.PVariantBind.variant;
#line 5700 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5700 "src/sema.tr"
                    List_TrStr_append(ex_covered, ex_vn2);
                }
            } else if (_t1536.tag == Pattern_PVariantBindMany) {
                __auto_type ex_vn3 = _t1536.data.PVariantBindMany.variant;
#line 5702 "src/sema.tr"
                if ((!ex_guarded)) {
#line 5702 "src/sema.tr"
                    List_TrStr_append(ex_covered, ex_vn3);
                }
            } else if (_t1536.tag == Pattern_POr) {
                __auto_type ex_orpats = _t1536.data.POr.patterns;
#line 5706 "src/sema.tr"
                long long ex_oi = 0LL;
#line 5707 "src/sema.tr"
                while ((ex_oi < ex_orpats->len)) {
#line 5708 "src/sema.tr"
                    __auto_type _t1537 = List_Pattern_get(ex_orpats, ex_oi);
                    if (_t1537.tag == Pattern_PVariant) {
                        __auto_type ex_ovn = _t1537.data.PVariant.variant;
#line 5710 "src/sema.tr"
                        if ((!ex_guarded)) {
#line 5710 "src/sema.tr"
                            List_TrStr_append(ex_covered, ex_ovn);
                        }
                    } else if (_t1537.tag == Pattern_PVariantBind) {
                        __auto_type ex_bvn = _t1537.data.PVariantBind.variant;
#line 5712 "src/sema.tr"
                        if ((!ex_guarded)) {
#line 5712 "src/sema.tr"
                            List_TrStr_append(ex_covered, ex_bvn);
                        }
#line 5713 "src/sema.tr"
                        ({ TrStr _at_t1538 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-3] Payload binding '", 23LL)), (ex_ty_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_bvn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(..)' is not allowed inside an or-pattern ('|').\n      FIX: Split the alternatives into separate 'case' arms, or match the variant without binding: 'case ", 154LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_bvn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ...:'.", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1538); _tr_str_release(_at_t1538); });
                    } else if (_t1537.tag == Pattern_PVariantBindMany) {
                        __auto_type ex_mvn = _t1537.data.PVariantBindMany.variant;
#line 5715 "src/sema.tr"
                        if ((!ex_guarded)) {
#line 5715 "src/sema.tr"
                            List_TrStr_append(ex_covered, ex_mvn);
                        }
#line 5716 "src/sema.tr"
                        ({ TrStr _at_t1539 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-3] Payload binding '", 23LL)), (ex_ty_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_mvn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(..)' is not allowed inside an or-pattern ('|').\n      FIX: Split the alternatives into separate 'case' arms, or match the variant without binding: 'case ", 154LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ex_mvn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ...:'.", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1539); _tr_str_release(_at_t1539); });
                    } else if (1) {
                        __auto_type _ = _t1537;
                        /* pass */
                    }
#line 5718 "src/sema.tr"
                    ex_oi = (ex_oi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1536;
                /* pass */
            }
#line 5720 "src/sema.tr"
            Sema_enter_scope(self);
#line 5722 "src/sema.tr"
            AstType* _subj_ty = hir_expr_type(hsubj);
#line 5723 "src/sema.tr"
            Sema_declare_pattern_binds_typed(self, arm->pat, _subj_ty);
#line 5724 "src/sema.tr"
            HirMatchArm* h_arm = ({ __auto_type _ord_t1540 = (arm->pat); HirBlock* _ord_t1541 = (Sema_lower_block(self, (*arm->body))); __auto_type _wr = (HirMatchArm_init(_ord_t1540, _ord_t1541)); _tr_obj_release(_ord_t1541, _trdrop_HirBlock); _wr; });
#line 5726 "src/sema.tr"
            if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
#line 5727 "src/sema.tr"
                h_arm->guard = Sema_lower_expr(self, arm->guard);
            }
#line 5728 "src/sema.tr"
            Sema_finalize_scope_drops(self, h_arm->body);
#line 5729 "src/sema.tr"
            List_ptr_append(h_arms, _tr_obj_retain(h_arm));
#line 5730 "src/sema.tr"
            Sema_exit_scope(self);
#line 5731 "src/sema.tr"
            k = (k + 1LL);
            _tr_obj_release(h_arm, _trdrop_HirMatchArm);
        }
#line 5733 "src/sema.tr"
        if (((!ex_has_wild) && _tr_dict_contains(self->enums, _tr_strz(ex_ty_name)))) {
#line 5734 "src/sema.tr"
            EnumDef* ex_edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(ex_ty_name)));
#line 5735 "src/sema.tr"
            List_TrStr* ex_missing = (void*)List_TrStr_new();
#line 5736 "src/sema.tr"
            long long ex_vi = 0LL;
#line 5737 "src/sema.tr"
            while ((ex_vi < ex_edef->variants->len)) {
#line 5738 "src/sema.tr"
                TrStr ex_vname = _tr_str_retain(((VariantDef*)List_ptr_get(ex_edef->variants, ex_vi))->name);
#line 5739 "src/sema.tr"
                if ((!List_TrStr_contains(ex_covered, ex_vname))) {
#line 5740 "src/sema.tr"
                    List_TrStr_append(ex_missing, ex_vname);
                }
#line 5741 "src/sema.tr"
                ex_vi = (ex_vi + 1LL);
                _tr_str_release(ex_vname);
            }
#line 5742 "src/sema.tr"
            if ((ex_missing->len > 0LL)) {
#line 5743 "src/sema.tr"
                TrStr ex_msg = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] Non-exhaustive match on '", 31LL)), (ex_ty_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': missing variant", 18LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 5744 "src/sema.tr"
                if ((ex_missing->len > 1LL)) {
#line 5744 "src/sema.tr"
                    ({ TrStr _ap_t1542 = (_tr_str_lit_len("s", 1LL)); _tr_str_append(&ex_msg, _ap_t1542); });
                }
#line 5745 "src/sema.tr"
                ({ TrStr _ap_t1543 = (_tr_str_lit_len(": ", 2LL)); _tr_str_append(&ex_msg, _ap_t1543); });
#line 5746 "src/sema.tr"
                long long ex_mi = 0LL;
#line 5747 "src/sema.tr"
                while ((ex_mi < ex_missing->len)) {
#line 5748 "src/sema.tr"
                    if ((ex_mi > 0LL)) {
#line 5748 "src/sema.tr"
                        ({ TrStr _ap_t1544 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ex_msg, _ap_t1544); });
                    }
#line 5749 "src/sema.tr"
                    ({ TrStr _ap_t1545 = (List_TrStr_get(ex_missing, ex_mi)); _tr_str_append(&ex_msg, _ap_t1545); _tr_str_release(_ap_t1545); });
#line 5750 "src/sema.tr"
                    ex_mi = (ex_mi + 1LL);
                }
#line 5751 "src/sema.tr"
                ({ TrStr _ap_t1546 = (_tr_str_lit_len(".\n      FIX: Add a 'case ", 25LL)); TrStr _ap_t1547 = (ex_ty_name); TrStr _ap_t1548 = (_tr_str_lit_len(".VariantName:' arm for each missing variant, or add 'case _:' to handle all remaining cases.", 92LL)); _tr_str_append(&ex_msg, _ap_t1546); _tr_str_append(&ex_msg, _ap_t1547); _tr_str_append(&ex_msg, _ap_t1548); });
#line 5752 "src/sema.tr"
                Sema_error(self, ex_msg);
            }
        }
#line 5753 "src/sema.tr"
        HirStmt* _trret238 = box_hirstmt(HirStmt_ctor_SMatch(hsubj, h_arms));
        _tr_str_release(ex_ty_name);
        List_TrStr_free(ex_covered);
        return _trret238;
    } else if (_t1432.tag == Stmt_STry) {
        __auto_type try_body = _t1432.data.STry.try_body;
__auto_type catches = _t1432.data.STry.catches;
__auto_type finally_b = _t1432.data.STry.finally_b;
#line 5755 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5756 "src/sema.tr"
        Sema_open_block(self);
#line 5757 "src/sema.tr"
        HirBlock* h_try_body = Sema_lower_block(self, try_body);
#line 5758 "src/sema.tr"
        List_ptr* h_catches = (void*)List_ptr_new();
#line 5759 "src/sema.tr"
        long long k = 0LL;
#line 5760 "src/sema.tr"
        while ((k < catches->len)) {
#line 5761 "src/sema.tr"
            CatchClause* cc = (*((CatchClause**)List_ptr_get(catches, k)));
#line 5762 "src/sema.tr"
            HirCatchClause* hcc_val = ((HirCatchClause*)_tr_obj_alloc(sizeof(HirCatchClause)));
#line 5763 "src/sema.tr"
            hcc_val->err_name = _tr_str_retain(cc->err_name);
#line 5764 "src/sema.tr"
            hcc_val->err_type = AstType_init(_tr_str_lit_len("str", 3LL));
#line 5765 "src/sema.tr"
            if ((((unsigned long long)(cc->err_type)) != ((unsigned long long)(0LL)))) {
#line 5765 "src/sema.tr"
                hcc_val->err_type = (*cc->err_type);
            }
#line 5768 "src/sema.tr"
            if ((!_tr_str_eqv((cc->err_name), (_tr_str_lit_len("", 0LL))))) {
#line 5769 "src/sema.tr"
                ({ __auto_type _ord_t1549 = (cc->err_name); __auto_type _ord_t1550 = (SymbolKind_make_SVariable()); __auto_type _ord_t1551 = (box_asttype(hcc_val->err_type)); Sema_declare(self, _ord_t1549, _ord_t1550, _ord_t1551, true); });
            }
#line 5770 "src/sema.tr"
            HirBlock* _cltmp_t1552 = Sema_lower_block(self, (*cc->body));
            _tr_obj_release(hcc_val->body, _trdrop_HirBlock);
            hcc_val->body = _cltmp_t1552;
#line 5771 "src/sema.tr"
            /* unsafe block */
#line 5772 "src/sema.tr"
            HirCatchClause** hcc = ((HirCatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(HirCatchClause*)));
#line 5773 "src/sema.tr"
            (*hcc = hcc_val);
#line 5774 "src/sema.tr"
            List_ptr_append(h_catches, hcc);
#line 5775 "src/sema.tr"
            k = (k + 1LL);
        }
#line 5776 "src/sema.tr"
        HirBlock* h_finally_b = Sema_lower_block(self, finally_b);
#line 5777 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5778 "src/sema.tr"
        Sema_close_block(self);
#line 5779 "src/sema.tr"
        HirStmt* _trret239 = box_hirstmt(HirStmt_ctor_STry(h_try_body, h_catches, h_finally_b));
        _tr_obj_release(h_try_body, _trdrop_HirBlock);
        _tr_obj_release(h_finally_b, _trdrop_HirBlock);
        return _trret239;
    } else if (_t1432.tag == Stmt_SRaise) {
        __auto_type e = _t1432.data.SRaise.val;
        return box_hirstmt(HirStmt_ctor_SRaise(Sema_lower_expr(self, e)));
    } else if (_t1432.tag == Stmt_SAssert) {
        __auto_type cond = _t1432.data.SAssert.cond;
__auto_type msg = _t1432.data.SAssert.msg;
        return ({ __auto_type _ord_t1553 = (Sema_lower_expr(self, cond)); __auto_type _ord_t1554 = (Sema_lower_expr(self, msg)); __auto_type _wr = (box_hirstmt(HirStmt_ctor_SAssert(_ord_t1553, _ord_t1554))); _wr; });
    } else if (_t1432.tag == Stmt_SDefer) {
        __auto_type inner = _t1432.data.SDefer.stmt;
#line 5783 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SDefer(Sema_lower_stmt(self, inner)));
    } else if (_t1432.tag == Stmt_SWith) {
        __auto_type items = _t1432.data.SWith.items;
__auto_type aliases = _t1432.data.SWith.aliases;
__auto_type body = _t1432.data.SWith.body;
#line 5785 "src/sema.tr"
        Sema_enter_scope(self);
#line 5786 "src/sema.tr"
        List_ptr* h_items = (void*)List_ptr_new();
#line 5787 "src/sema.tr"
        long long k = 0LL;
#line 5788 "src/sema.tr"
        while ((k < items->len)) {
#line 5789 "src/sema.tr"
            HirExpr* h_wi = Sema_lower_expr(self, ((Expr*)List_ptr_get(items, k)));
#line 5790 "src/sema.tr"
            List_ptr_append(h_items, h_wi);
#line 5792 "src/sema.tr"
            if (((k < aliases->len) && ({ TrStr _cmpa_t1555 = (List_TrStr_get(aliases, k)); TrStr _cmpb_t1556 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t1557 = !_tr_str_eqv(_cmpa_t1555, _cmpb_t1556); _tr_str_release(_cmpa_t1555); _cmpr_t1557; }))) {
#line 5793 "src/sema.tr"
                AstType* wi_ty = hir_expr_type(h_wi);
#line 5794 "src/sema.tr"
                ({ TrStr _ord_t1558 = (List_TrStr_get(aliases, k)); __auto_type _ord_t1559 = (SymbolKind_make_SVariable()); __auto_type _ord_t1560 = (box_asttype(wi_ty)); Sema_declare(self, _ord_t1558, _ord_t1559, _ord_t1560, true); _tr_str_release(_ord_t1558); });
            }
#line 5795 "src/sema.tr"
            k = (k + 1LL);
        }
#line 5796 "src/sema.tr"
        HirBlock* h_with_body = Sema_lower_block(self, body);
#line 5797 "src/sema.tr"
        Sema_finalize_scope_drops(self, h_with_body);
#line 5798 "src/sema.tr"
        Sema_exit_scope(self);
#line 5799 "src/sema.tr"
        HirStmt* _trret240 = box_hirstmt(HirStmt_ctor_SWith(h_items, aliases, h_with_body));
        _tr_obj_release(h_with_body, _trdrop_HirBlock);
        return _trret240;
    } else if (_t1432.tag == Stmt_SAsm) {
        __auto_type code = _t1432.data.SAsm.code;
__auto_type outputs = _t1432.data.SAsm.outputs;
__auto_type inputs = _t1432.data.SAsm.inputs;
__auto_type clobbers = _t1432.data.SAsm.clobbers;
#line 5801 "src/sema.tr"
        HirStmt* _trret241 = box_hirstmt(HirStmt_ctor_SAsm(code, outputs, inputs, clobbers));
        return _trret241;
    } else if (_t1432.tag == Stmt_SSpawn) {
        __auto_type e = _t1432.data.SSpawn.expr;
#line 5803 "src/sema.tr"
        if ((!self->in_async_fn)) {
#line 5804 "src/sema.tr"
            ({ TrStr _at_t1561 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[C-5] 'spawn:' used outside an async function. FIX: Declare '", 61LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as 'async def ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' to use spawn inside it.", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1561); _tr_str_release(_at_t1561); });
        }
#line 5805 "src/sema.tr"
        HirExpr* spawn_lowered = Sema_lower_expr(self, e);
#line 5806 "src/sema.tr"
        Sema_check_spawn_sendable(self, spawn_lowered);
#line 5807 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SSpawn(spawn_lowered));
    } else if (_t1432.tag == Stmt_STaskGroup) {
        __auto_type body = _t1432.data.STaskGroup.body;
#line 5809 "src/sema.tr"
        if ((!self->in_async_fn)) {
#line 5810 "src/sema.tr"
            ({ TrStr _at_t1562 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[C-6] 'taskgroup:' used outside an async function. FIX: Declare '", 65LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as 'async def ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' to use taskgroup inside it.", 34LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1562); _tr_str_release(_at_t1562); });
        }
#line 5811 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5812 "src/sema.tr"
        Sema_open_block(self);
#line 5813 "src/sema.tr"
        HirBlock* h_tg_body = Sema_lower_block(self, body);
#line 5814 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5815 "src/sema.tr"
        Sema_close_block(self);
#line 5816 "src/sema.tr"
        HirStmt* _trret242 = box_hirstmt(HirStmt_ctor_STaskGroup(h_tg_body));
        _tr_obj_release(h_tg_body, _trdrop_HirBlock);
        return _trret242;
    } else if (_t1432.tag == Stmt_SChanSelect) {
        __auto_type cs_cases = _t1432.data.SChanSelect.cases;
#line 5818 "src/sema.tr"
        List_ptr* hcs_cases = (void*)List_ptr_new();
#line 5819 "src/sema.tr"
        long long csi2 = 0LL;
#line 5820 "src/sema.tr"
        while ((csi2 < cs_cases->len)) {
#line 5821 "src/sema.tr"
            ChanSelectArm* arm = (*((ChanSelectArm**)List_ptr_get(cs_cases, csi2)));
#line 5822 "src/sema.tr"
            HirChanSelectArm** harm = (HirChanSelectArm**)(0LL);
#line 5823 "src/sema.tr"
            /* unsafe block */
#line 5823 "src/sema.tr"
            harm = ((HirChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(HirChanSelectArm*)));
#line 5824 "src/sema.tr"
            HirChanSelectArm* harm_v = ((HirChanSelectArm*)_tr_obj_alloc(sizeof(HirChanSelectArm)));
#line 5825 "src/sema.tr"
            harm_v->kind = arm->kind;
#line 5826 "src/sema.tr"
            harm_v->var_name = _tr_str_retain(arm->var_name);
#line 5827 "src/sema.tr"
            if ((((unsigned long long)(arm->chan_expr)) != ((unsigned long long)(0LL)))) {
#line 5828 "src/sema.tr"
                harm_v->chan_expr = Sema_lower_expr(self, arm->chan_expr);
            } else {
#line 5830 "src/sema.tr"
                harm_v->chan_expr = (HirExpr*)(0LL);
            }
#line 5831 "src/sema.tr"
            if ((((unsigned long long)(arm->val_expr)) != ((unsigned long long)(0LL)))) {
#line 5832 "src/sema.tr"
                harm_v->val_expr = Sema_lower_expr(self, arm->val_expr);
            } else {
#line 5834 "src/sema.tr"
                harm_v->val_expr = (HirExpr*)(0LL);
            }
#line 5835 "src/sema.tr"
            if ((((unsigned long long)(arm->timeout_ms)) != ((unsigned long long)(0LL)))) {
#line 5836 "src/sema.tr"
                harm_v->timeout_ms = Sema_lower_expr(self, arm->timeout_ms);
            } else {
#line 5838 "src/sema.tr"
                harm_v->timeout_ms = (HirExpr*)(0LL);
            }
#line 5840 "src/sema.tr"
            Sema_enter_scope(self);
#line 5841 "src/sema.tr"
            if (((arm->kind == 0LL) && (!_tr_str_eqv((arm->var_name), (_tr_str_lit_len("", 0LL)))))) {
#line 5842 "src/sema.tr"
                AstType* recv_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 5843 "src/sema.tr"
                if ((((unsigned long long)(arm->chan_expr)) != ((unsigned long long)(0LL)))) {
#line 5844 "src/sema.tr"
                    HirExpr* chan_hir = harm_v->chan_expr;
#line 5845 "src/sema.tr"
                    AstType* chan_ty = hir_expr_type(chan_hir);
#line 5846 "src/sema.tr"
                    if (((!_is_invalid_ptr(((unsigned long long)(chan_ty->args)))) && (chan_ty->args->len > 0LL))) {
#line 5847 "src/sema.tr"
                        recv_ty = (*((AstType**)List_ptr_get(chan_ty->args, 0LL)));
                    }
                }
#line 5848 "src/sema.tr"
                ({ __auto_type _ord_t1563 = (arm->var_name); __auto_type _ord_t1564 = (SymbolKind_make_SVariable()); __auto_type _ord_t1565 = (box_asttype(recv_ty)); Sema_declare(self, _ord_t1563, _ord_t1564, _ord_t1565, true); });
            }
#line 5849 "src/sema.tr"
            HirBlock* _cltmp_t1566 = Sema_lower_block(self, arm->body);
            _tr_obj_release(harm_v->body, _trdrop_HirBlock);
            harm_v->body = _cltmp_t1566;
#line 5850 "src/sema.tr"
            Sema_finalize_scope_drops(self, harm_v->body);
#line 5851 "src/sema.tr"
            Sema_exit_scope(self);
#line 5852 "src/sema.tr"
            /* unsafe block */
#line 5852 "src/sema.tr"
            (*harm = harm_v);
#line 5853 "src/sema.tr"
            List_ptr_append(hcs_cases, harm);
#line 5854 "src/sema.tr"
            csi2 = (csi2 + 1LL);
        }
#line 5855 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SChanSelect(hcs_cases));
    } else if (_t1432.tag == Stmt_SGpuBlock) {
        __auto_type body = _t1432.data.SGpuBlock.body;
#line 5857 "src/sema.tr"
        self->block_depth = (self->block_depth + 1LL);
#line 5858 "src/sema.tr"
        Sema_open_block(self);
#line 5859 "src/sema.tr"
        HirBlock* h_gpu_body = Sema_lower_block(self, body);
#line 5860 "src/sema.tr"
        self->block_depth = (self->block_depth - 1LL);
#line 5861 "src/sema.tr"
        Sema_close_block(self);
#line 5862 "src/sema.tr"
        HirStmt* _trret243 = box_hirstmt(HirStmt_ctor_SGpuBlock(h_gpu_body));
        _tr_obj_release(h_gpu_body, _trdrop_HirBlock);
        return _trret243;
    } else if (_t1432.tag == Stmt_SBreak) {
        __auto_type bv = _t1432.data.SBreak.val;
#line 5864 "src/sema.tr"
        HirExpr* hbv = (HirExpr*)(0LL);
#line 5865 "src/sema.tr"
        if ((((unsigned long long)(bv)) != ((unsigned long long)(0LL)))) {
#line 5865 "src/sema.tr"
            hbv = Sema_lower_expr(self, bv);
        }
#line 5866 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SBreak(hbv));
    } else if (_t1432.tag == Stmt_SContinue) {
        return box_hirstmt(HirStmt_make_SContinue());
    } else if (_t1432.tag == Stmt_SPass) {
        return box_hirstmt(HirStmt_make_SPass());
    } else if (_t1432.tag == Stmt_SLocalDecl) {
        __auto_type ldecl = _t1432.data.SLocalDecl.decl;
#line 5870 "src/sema.tr"
        if ((!_tr_str_eqv((self->current_func_name), (_tr_str_lit_len("main", 4LL))))) {
#line 5871 "src/sema.tr"
            Sema_error(self, _tr_str_lit_len("[E-2] Nested class/def/enum/interface declarations are only supported inside main().\n      FIX: Move this declaration to module (top-level) scope, or declare it inside main().", 175LL));
#line 5872 "src/sema.tr"
            return box_hirstmt(HirStmt_make_SPass());
        }
#line 5873 "src/sema.tr"
        Sema_register_decl(self, ldecl);
#line 5877 "src/sema.tr"
        TrStr saved_ld_func_name = _tr_str_retain(self->current_func_name);
#line 5878 "src/sema.tr"
        TrStr saved_ld_class_name = _tr_str_retain(self->current_class_name);
#line 5879 "src/sema.tr"
        __auto_type _t1567 = (*ldecl);
        if (_t1567.tag == Decl_DFunction) {
            __auto_type ld_f = _t1567.data.DFunction.func;
#line 5881 "src/sema.tr"
            ({ HirFunction* _ord_t1568 = (Sema_lower_func(self, ld_f)); List_ptr_append(self->nested_functions, _tr_obj_retain(_ord_t1568)); _tr_obj_release(_ord_t1568, _trdrop_HirFunction); });
        } else if (_t1567.tag == Decl_DClass) {
            __auto_type ld_c = _t1567.data.DClass.cls;
#line 5883 "src/sema.tr"
            ({ HirClass* _ord_t1569 = (Sema_lower_class(self, ld_c)); List_ptr_append(self->nested_classes, _tr_obj_retain(_ord_t1569)); _tr_obj_release(_ord_t1569, _trdrop_HirClass); });
        } else if (_t1567.tag == Decl_DActor) {
            __auto_type ld_c = _t1567.data.DActor.cls;
#line 5885 "src/sema.tr"
            ({ HirClass* _ord_t1570 = (Sema_lower_class(self, ld_c)); List_ptr_append(self->nested_classes, _tr_obj_retain(_ord_t1570)); _tr_obj_release(_ord_t1570, _trdrop_HirClass); });
        } else if (_t1567.tag == Decl_DEnum) {
            __auto_type ld_e = _t1567.data.DEnum.enm;
#line 5887 "src/sema.tr"
            ({ HirEnum* _ord_t1571 = (Sema_lower_enum(self, ld_e)); List_ptr_append(self->nested_enums, _tr_obj_retain(_ord_t1571)); _tr_obj_release(_ord_t1571, _trdrop_HirEnum); });
        } else if (_t1567.tag == Decl_DInterface) {
            __auto_type ld_i = _t1567.data.DInterface.iface;
#line 5889 "src/sema.tr"
            ({ HirInterface* _ord_t1572 = (Sema_lower_interface(self, ld_i)); List_ptr_append(self->nested_interfaces, _tr_obj_retain(_ord_t1572)); _tr_obj_release(_ord_t1572, _trdrop_HirInterface); });
        } else if (_t1567.tag == Decl_DExtend) {
            __auto_type ld_target = _t1567.data.DExtend.target;
__auto_type ld_methods = _t1567.data.DExtend.methods;
#line 5895 "src/sema.tr"
            self->current_class_name = _tr_str_retain(ld_target);
#line 5896 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(ld_target))) {
#line 5897 "src/sema.tr"
                self->current_region_params = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ld_target)))->region_params;
            }
#line 5898 "src/sema.tr"
            long long ld_ci = 0LL;
#line 5899 "src/sema.tr"
            bool ld_found = false;
#line 5900 "src/sema.tr"
            while ((ld_ci < self->nested_classes->len)) {
#line 5901 "src/sema.tr"
                HirClass* ld_nc = ((HirClass*)List_ptr_get(self->nested_classes, ld_ci));
#line 5902 "src/sema.tr"
                if (_tr_str_eqv((ld_nc->name), (ld_target))) {
#line 5903 "src/sema.tr"
                    long long ld_hi = 0LL;
#line 5904 "src/sema.tr"
                    while ((ld_hi < ld_methods->len)) {
#line 5905 "src/sema.tr"
                        ({ HirFunction* _ord_t1573 = (Sema_lower_func(self, ((FunctionDef*)List_ptr_get(ld_methods, ld_hi)))); List_ptr_append(ld_nc->methods, _tr_obj_retain(_ord_t1573)); _tr_obj_release(_ord_t1573, _trdrop_HirFunction); });
#line 5906 "src/sema.tr"
                        ld_hi = (ld_hi + 1LL);
                    }
#line 5907 "src/sema.tr"
                    ({ List_ptr* _sl = self->nested_classes; long long _si = (long long)(ld_ci); void* _sn = (void*)(_tr_obj_retain(ld_nc)); void* _so = (_sl && (size_t)_si < _sl->len) ? (void*)List_ptr_get(_sl, _si) : (void*)0; List_ptr_set(_sl, _si, _sn); if (_so) _tr_obj_release((HirClass*)_so, _trdrop_HirClass); });
#line 5908 "src/sema.tr"
                    ld_found = true;
                }
#line 5909 "src/sema.tr"
                ld_ci = (ld_ci + 1LL);
            }
#line 5910 "src/sema.tr"
            if ((!ld_found)) {
#line 5911 "src/sema.tr"
                long long ld_hi2 = 0LL;
#line 5912 "src/sema.tr"
                while ((ld_hi2 < ld_methods->len)) {
#line 5913 "src/sema.tr"
                    ({ HirFunction* _ord_t1574 = (Sema_lower_func(self, ((FunctionDef*)List_ptr_get(ld_methods, ld_hi2)))); List_ptr_append(self->nested_functions, _tr_obj_retain(_ord_t1574)); _tr_obj_release(_ord_t1574, _trdrop_HirFunction); });
#line 5914 "src/sema.tr"
                    ld_hi2 = (ld_hi2 + 1LL);
                }
            }
        } else if (1) {
            __auto_type _ = _t1567;
            /* pass */
        }
#line 5916 "src/sema.tr"
        self->current_func_name = _tr_str_retain(saved_ld_func_name);
#line 5917 "src/sema.tr"
        self->current_class_name = _tr_str_retain(saved_ld_class_name);
#line 5918 "src/sema.tr"
        self->current_region_params = (void*)List_TrStr_new();
#line 5919 "src/sema.tr"
        HirStmt* _trret244 = box_hirstmt(HirStmt_make_SPass());
        _tr_str_release(saved_ld_func_name);
        _tr_str_release(saved_ld_class_name);
        return _trret244;
    } else if (_t1432.tag == Stmt_SUnsafe) {
        __auto_type body = _t1432.data.SUnsafe.body;
#line 5922 "src/sema.tr"
        bool saved_unsafe = self->in_unsafe;
#line 5923 "src/sema.tr"
        self->in_unsafe = true;
#line 5924 "src/sema.tr"
        HirBlock* unsafe_hir = Sema_lower_block(self, body);
#line 5925 "src/sema.tr"
        self->in_unsafe = saved_unsafe;
#line 5926 "src/sema.tr"
        HirStmt* _trret245 = box_hirstmt(HirStmt_ctor_SUnsafe(unsafe_hir));
        _tr_obj_release(unsafe_hir, _trdrop_HirBlock);
        return _trret245;
    } else if (_t1432.tag == Stmt_SMultiLet) {
        __auto_type names = _t1432.data.SMultiLet.names;
__auto_type is_mut = _t1432.data.SMultiLet.is_mut;
__auto_type val_ptr = _t1432.data.SMultiLet.val;
#line 5928 "src/sema.tr"
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
#line 5929 "src/sema.tr"
        AstType* val_ty = hir_expr_type(hval);
#line 5930 "src/sema.tr"
        long long i = 0LL;
#line 5931 "src/sema.tr"
        while ((i < names->len)) {
#line 5932 "src/sema.tr"
            AstType* nty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 5933 "src/sema.tr"
            if ((val_ty->args->len > i)) {
#line 5933 "src/sema.tr"
                nty = (*((AstType**)List_ptr_get(val_ty->args, i)));
            }
#line 5934 "src/sema.tr"
            ({ TrStr _ord_t1575 = (List_TrStr_get(names, i)); __auto_type _ord_t1576 = (SymbolKind_make_SVariable()); __auto_type _ord_t1577 = (box_asttype(nty)); Sema_declare(self, _ord_t1575, _ord_t1576, _ord_t1577, is_mut); _tr_str_release(_ord_t1575); });
#line 5935 "src/sema.tr"
            i = (i + 1LL);
        }
#line 5936 "src/sema.tr"
        HirStmt* _trret246 = box_hirstmt(HirStmt_ctor_SMultiLet(names, is_mut, hval));
        return _trret246;
    } else if (_t1432.tag == Stmt_SLine) {
        __auto_type n = _t1432.data.SLine.n;
#line 5938 "src/sema.tr"
        self->current_line = n;
#line 5943 "src/sema.tr"
        return box_hirstmt(HirStmt_ctor_SLineMarker(n));
    } else if (1) {
        __auto_type _ = _t1432;
        return box_hirstmt(HirStmt_make_SPass());
    }
}

__attribute__((hot)) AstType* Sema_variant_field_ty(Sema* self, TrStr type_name, TrStr variant_name, long long field_idx) {
#line 5947 "src/sema.tr"
    if (_tr_dict_contains(self->enums, _tr_strz(type_name))) {
#line 5948 "src/sema.tr"
        EnumDef* enm = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(type_name)));
#line 5949 "src/sema.tr"
        long long vi = 0LL;
#line 5950 "src/sema.tr"
        while ((vi < enm->variants->len)) {
#line 5951 "src/sema.tr"
            VariantDef* v = ((VariantDef*)List_ptr_get(enm->variants, vi));
#line 5952 "src/sema.tr"
            if (_tr_str_eqv((v->name), (variant_name))) {
#line 5953 "src/sema.tr"
                if ((field_idx < v->fields->len)) {
#line 5954 "src/sema.tr"
                    Param* fp = ((Param*)List_ptr_get(v->fields, field_idx));
#line 5955 "src/sema.tr"
                    if ((((unsigned long long)(fp->ty)) != ((unsigned long long)(0LL)))) {
#line 5955 "src/sema.tr"
                        return (*fp->ty);
                    }
                }
#line 5956 "src/sema.tr"
                return AstType_init(_tr_str_lit_len("void", 4LL));
            }
#line 5957 "src/sema.tr"
            vi = (vi + 1LL);
        }
    }
#line 5958 "src/sema.tr"
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) void Sema_declare_pattern_binds(Sema* self, Pattern pat) {
#line 5961 "src/sema.tr"
    Sema_declare_pattern_binds_typed(self, pat, AstType_init(_tr_str_lit_len("int", 3LL)));
}

__attribute__((hot)) void Sema_declare_pattern_binds_typed(Sema* self, Pattern pat, AstType* subj_ty) {
#line 5965 "src/sema.tr"
    __auto_type _t1578 = pat;
    if (_t1578.tag == Pattern_PBind) {
        __auto_type name = _t1578.data.PBind.name;
        ({ __auto_type _ord_t1579 = (SymbolKind_make_SVariable()); __auto_type _ord_t1580 = (box_asttype(subj_ty)); Sema_declare(self, name, _ord_t1579, _ord_t1580, false); });
    } else if (_t1578.tag == Pattern_PVariantBind) {
        __auto_type type_name = _t1578.data.PVariantBind.type_name;
__auto_type variant_name = _t1578.data.PVariantBind.variant;
__auto_type field = _t1578.data.PVariantBind.field;
#line 5968 "src/sema.tr"
        AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, 0LL);
#line 5971 "src/sema.tr"
        if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Option", 6LL))) && _tr_str_eqv((variant_name), (_tr_str_lit_len("Some", 4LL)))) && (subj_ty->args->len > 0LL))) {
#line 5972 "src/sema.tr"
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 0LL)));
        } else if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((variant_name), (_tr_str_lit_len("Ok", 2LL)))) && (subj_ty->args->len > 0LL))) {
#line 5974 "src/sema.tr"
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 0LL)));
        } else if (((_tr_str_eqv((type_name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((variant_name), (_tr_str_lit_len("Err", 3LL)))) && (subj_ty->args->len > 1LL))) {
#line 5976 "src/sema.tr"
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 1LL)));
        }
#line 5977 "src/sema.tr"
        if (_tr_str_eqv((fty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 5977 "src/sema.tr"
            fty = AstType_init(_tr_str_lit_len("int", 3LL));
        }
#line 5978 "src/sema.tr"
        ({ __auto_type _ord_t1581 = (SymbolKind_make_SVariable()); __auto_type _ord_t1582 = (box_asttype(fty)); Sema_declare(self, field, _ord_t1581, _ord_t1582, false); });
    } else if (_t1578.tag == Pattern_PVariantBindMany) {
        __auto_type type_name = _t1578.data.PVariantBindMany.type_name;
__auto_type variant_name = _t1578.data.PVariantBindMany.variant;
__auto_type fields = _t1578.data.PVariantBindMany.fields;
#line 5980 "src/sema.tr"
        long long _pi = 0LL;
#line 5981 "src/sema.tr"
        while ((_pi < fields->len)) {
#line 5982 "src/sema.tr"
            TrStr _pf = List_TrStr_get(fields, _pi);
#line 5983 "src/sema.tr"
            if ((!_tr_str_eqv((_pf), (_tr_str_lit_len("_", 1LL))))) {
#line 5984 "src/sema.tr"
                AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, _pi);
#line 5985 "src/sema.tr"
                if (_tr_str_eqv((fty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 5985 "src/sema.tr"
                    fty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
#line 5986 "src/sema.tr"
                ({ __auto_type _ord_t1583 = (SymbolKind_make_SVariable()); __auto_type _ord_t1584 = (box_asttype(fty)); Sema_declare(self, _pf, _ord_t1583, _ord_t1584, false); });
            }
#line 5987 "src/sema.tr"
            _pi = (_pi + 1LL);
            _tr_str_release(_pf);
        }
    } else if (_t1578.tag == Pattern_PTuple) {
        __auto_type first = _t1578.data.PTuple.first;
__auto_type second = _t1578.data.PTuple.second;
#line 5989 "src/sema.tr"
        ({ __auto_type _ord_t1585 = (SymbolKind_make_SVariable()); __auto_type _ord_t1586 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); Sema_declare(self, first, _ord_t1585, _ord_t1586, false); });
#line 5990 "src/sema.tr"
        ({ __auto_type _ord_t1587 = (SymbolKind_make_SVariable()); __auto_type _ord_t1588 = (box_asttype(AstType_init(_tr_str_lit_len("int", 3LL)))); Sema_declare(self, second, _ord_t1587, _ord_t1588, false); });
    } else if (_t1578.tag == Pattern_POr) {
        __auto_type pats = _t1578.data.POr.patterns;
#line 5992 "src/sema.tr"
        long long i = 0LL;
#line 5993 "src/sema.tr"
        while ((i < pats->len)) {
#line 5994 "src/sema.tr"
            Sema_declare_pattern_binds_typed(self, List_Pattern_get(pats, i), subj_ty);
#line 5995 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1578;
        /* pass */
    }
}

__attribute__((hot)) void Sema_check_match_expr_exhaustive(Sema* self, TrStr ty_name, List_ptr* arms) {
#line 6001 "src/sema.tr"
    if ((!_tr_dict_contains(self->enums, _tr_strz(ty_name)))) {
#line 6001 "src/sema.tr"
        return;
    }
#line 6002 "src/sema.tr"
    List_TrStr* covered = (void*)List_TrStr_new();
#line 6003 "src/sema.tr"
    bool has_wild = false;
#line 6004 "src/sema.tr"
    long long i = 0LL;
#line 6005 "src/sema.tr"
    while ((i < arms->len)) {
#line 6006 "src/sema.tr"
        MatchArm* arm = ((MatchArm*)List_ptr_get(arms, i));
#line 6007 "src/sema.tr"
        bool guarded = (((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)));
#line 6008 "src/sema.tr"
        if ((!guarded)) {
#line 6009 "src/sema.tr"
            __auto_type _t1589 = arm->pat;
            if (_t1589.tag == Pattern_PWild) {
                has_wild = true;
            } else if (_t1589.tag == Pattern_PBind) {
                __auto_type _ = _t1589.data.PBind.name;
                has_wild = true;
            } else if (_t1589.tag == Pattern_PVariant) {
                __auto_type v1 = _t1589.data.PVariant.variant;
                List_TrStr_append(covered, v1);
            } else if (_t1589.tag == Pattern_PVariantBind) {
                __auto_type v2 = _t1589.data.PVariantBind.variant;
                List_TrStr_append(covered, v2);
            } else if (_t1589.tag == Pattern_PVariantBindMany) {
                __auto_type v3 = _t1589.data.PVariantBindMany.variant;
                List_TrStr_append(covered, v3);
            } else if (_t1589.tag == Pattern_POr) {
                __auto_type ors = _t1589.data.POr.patterns;
#line 6016 "src/sema.tr"
                long long oi = 0LL;
#line 6017 "src/sema.tr"
                while ((oi < ors->len)) {
#line 6018 "src/sema.tr"
                    __auto_type _t1590 = List_Pattern_get(ors, oi);
                    if (_t1590.tag == Pattern_PVariant) {
                        __auto_type ov1 = _t1590.data.PVariant.variant;
                        List_TrStr_append(covered, ov1);
                    } else if (_t1590.tag == Pattern_PVariantBind) {
                        __auto_type ov2 = _t1590.data.PVariantBind.variant;
                        List_TrStr_append(covered, ov2);
                    } else if (_t1590.tag == Pattern_PVariantBindMany) {
                        __auto_type ov3 = _t1590.data.PVariantBindMany.variant;
                        List_TrStr_append(covered, ov3);
                    } else if (1) {
                        __auto_type _ = _t1590;
                        /* pass */
                    }
#line 6023 "src/sema.tr"
                    oi = (oi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1589;
                /* pass */
            }
        }
#line 6025 "src/sema.tr"
        i = (i + 1LL);
    }
#line 6026 "src/sema.tr"
    if (has_wild) {
#line 6026 "src/sema.tr"
        List_TrStr_free(covered);
        return;
    }
#line 6027 "src/sema.tr"
    EnumDef* edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(ty_name)));
#line 6028 "src/sema.tr"
    TrStr missing = _tr_str_lit_len("", 0LL);
#line 6029 "src/sema.tr"
    long long nmiss = 0LL;
#line 6030 "src/sema.tr"
    long long vi = 0LL;
#line 6031 "src/sema.tr"
    while ((vi < edef->variants->len)) {
#line 6032 "src/sema.tr"
        TrStr vn = _tr_str_retain(((VariantDef*)List_ptr_get(edef->variants, vi))->name);
#line 6033 "src/sema.tr"
        if ((!List_TrStr_contains(covered, vn))) {
#line 6034 "src/sema.tr"
            if ((nmiss > 0LL)) {
#line 6034 "src/sema.tr"
                ({ TrStr _ap_t1591 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&missing, _ap_t1591); });
            }
#line 6035 "src/sema.tr"
            ({ TrStr _ap_t1592 = (vn); _tr_str_append(&missing, _ap_t1592); });
#line 6036 "src/sema.tr"
            nmiss = (nmiss + 1LL);
        }
#line 6037 "src/sema.tr"
        vi = (vi + 1LL);
        _tr_str_release(vn);
    }
#line 6038 "src/sema.tr"
    if ((nmiss > 0LL)) {
#line 6039 "src/sema.tr"
        TrStr plural = _tr_str_lit_len("", 0LL);
#line 6040 "src/sema.tr"
        if ((nmiss > 1LL)) {
#line 6040 "src/sema.tr"
            TrStr _strtmp_t1593 = _tr_str_lit_len("s", 1LL);
            _tr_str_release(plural);
            plural = _strtmp_t1593;
        }
#line 6041 "src/sema.tr"
        ({ TrStr _at_t1594 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] Non-exhaustive match expression on '", 42LL)), (ty_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': missing variant", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (plural)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (missing)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: Add a 'case ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ty_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".VariantName:' arm for each missing variant, or add 'case _:' to give the remaining cases a value.", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1594); _tr_str_release(_at_t1594); });
    }
    List_TrStr_free(covered);
    _tr_str_release(missing);
}

__attribute__((hot)) HirExpr* Sema_retype_none(Sema* self, HirExpr* e, AstType* expected) {
#line 6046 "src/sema.tr"
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
#line 6046 "src/sema.tr"
        return e;
    }
#line 6047 "src/sema.tr"
    if ((!_tr_str_eqv((expected->name), (_tr_str_lit_len("Option", 6LL))))) {
#line 6047 "src/sema.tr"
        return e;
    }
#line 6048 "src/sema.tr"
    __auto_type _t1595 = (*e);
    if (_t1595.tag == HirExpr_ELitNone) {
        __auto_type _ = _t1595.data.ELitNone.ty;
        HirExpr* _trret247 = box_hirexpr(HirExpr_ctor_ELitNone(expected));
        return _trret247;
    } else if (1) {
        __auto_type _ = _t1595;
        return e;
    }
}

__attribute__((hot)) AstType* Sema_str_method_ret_ty(Sema* self, TrStr method) {
#line 6053 "src/sema.tr"
    if ((((_tr_str_eqv((method), (_tr_str_lit_len("split", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("split_to_vec", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("split_once", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("split_exact", 11LL))))) {
#line 6054 "src/sema.tr"
        return AstType_init_generic(_tr_str_lit_len("Vec", 3LL), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL))));
    }
#line 6055 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("strip", 5LL)))) {
#line 6055 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6056 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("trim", 4LL)))) {
#line 6056 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6057 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("trim_left", 9LL)))) {
#line 6057 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6058 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("trim_right", 10LL)))) {
#line 6058 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6059 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("to_upper", 8LL)))) {
#line 6059 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6060 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("to_lower", 8LL)))) {
#line 6060 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6061 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("upper", 5LL)))) {
#line 6061 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6062 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("lower", 5LL)))) {
#line 6062 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6063 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("lstrip", 6LL)))) {
#line 6063 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6064 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("rstrip", 6LL)))) {
#line 6064 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6065 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("center", 6LL)))) {
#line 6065 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6066 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("pad_left", 8LL)))) {
#line 6066 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6067 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("pad_right", 9LL)))) {
#line 6067 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6068 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("zfill", 5LL)))) {
#line 6068 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6069 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("format", 6LL)))) {
#line 6069 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6070 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("chars", 5LL)))) {
#line 6070 "src/sema.tr"
        return AstType_init_generic(_tr_str_lit_len("Vec", 3LL), box_asttype(AstType_init(_tr_str_lit_len("str", 3LL))));
    }
#line 6071 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("capitalize", 10LL)))) {
#line 6071 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6072 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("title", 5LL)))) {
#line 6072 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6073 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL)))) {
#line 6073 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6074 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("repeat", 6LL)))) {
#line 6074 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6075 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("replace", 7LL)))) {
#line 6075 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6076 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("replace_first", 13LL)))) {
#line 6076 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6077 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL)))) {
#line 6077 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6078 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("strip_prefix", 12LL)))) {
#line 6078 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6079 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("strip_suffix", 12LL)))) {
#line 6079 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6080 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("remove_char", 11LL)))) {
#line 6080 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6081 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) {
#line 6081 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6082 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))) {
#line 6082 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6083 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("find", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
#line 6083 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6084 "src/sema.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("md5", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sha256", 6LL))))) {
#line 6084 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("str", 3LL));
    }
#line 6085 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("last_index_of", 13LL))) || _tr_str_eqv((method), (_tr_str_lit_len("rfind", 5LL))))) {
#line 6085 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6086 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL)))) {
#line 6086 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6087 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("char_at", 7LL)))) {
#line 6087 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6088 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_int", 9LL)))) {
#line 6088 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6089 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_int", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_i64", 6LL))))) {
#line 6089 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("int", 3LL));
    }
#line 6090 "src/sema.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_f64", 6LL))))) {
#line 6090 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("float", 5LL));
    }
#line 6091 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("starts_with", 11LL)))) {
#line 6091 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6092 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("ends_with", 9LL)))) {
#line 6092 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6093 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) {
#line 6093 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6094 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_bool", 10LL)))) {
#line 6094 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6095 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) {
#line 6095 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6096 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("contains_char", 13LL)))) {
#line 6096 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6097 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("eq", 2LL)))) {
#line 6097 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6098 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_digit", 8LL)))) {
#line 6098 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6099 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_alpha", 8LL)))) {
#line 6099 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6100 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_alnum", 8LL)))) {
#line 6100 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6101 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_space", 8LL)))) {
#line 6101 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6102 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_upper", 8LL)))) {
#line 6102 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6103 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_lower", 8LL)))) {
#line 6103 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6104 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_bool", 10LL)))) {
#line 6104 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("bool", 4LL));
    }
#line 6105 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("parse_float", 11LL)))) {
#line 6105 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("float", 5LL));
    }
#line 6106 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("lines", 5LL)))) {
#line 6106 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("Vec", 3LL));
    }
#line 6107 "src/sema.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("words", 5LL)))) {
#line 6107 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("Vec", 3LL));
    }
#line 6108 "src/sema.tr"
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) void Sema_collect_block_refs(Sema* self, HirBlock* b, List_TrStr* out) {
#line 6113 "src/sema.tr"
    long long i = 0LL;
#line 6114 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 6115 "src/sema.tr"
        Sema_collect_stmt_refs(self, ((HirStmt*)List_ptr_get(b->stmts, i)), out);
#line 6116 "src/sema.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema_collect_stmt_refs(Sema* self, HirStmt* s, List_TrStr* out) {
#line 6119 "src/sema.tr"
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
#line 6119 "src/sema.tr"
        return;
    }
#line 6120 "src/sema.tr"
    __auto_type _t1596 = (*s);
    if (_t1596.tag == HirStmt_SExpr) {
        __auto_type e = _t1596.data.SExpr.expr;
        Sema_collect_idents(self, e, out);
    } else if (_t1596.tag == HirStmt_SLet) {
        __auto_type v = _t1596.data.SLet.val;
        Sema_collect_idents(self, v, out);
    } else if (_t1596.tag == HirStmt_SAssign) {
        __auto_type t = _t1596.data.SAssign.target;
__auto_type v = _t1596.data.SAssign.val;
#line 6124 "src/sema.tr"
        Sema_collect_idents(self, t, out);
#line 6125 "src/sema.tr"
        Sema_collect_idents(self, v, out);
    } else if (_t1596.tag == HirStmt_SReturn) {
        __auto_type v = _t1596.data.SReturn.val;
#line 6127 "src/sema.tr"
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
#line 6127 "src/sema.tr"
            Sema_collect_idents(self, v, out);
        }
    } else if (_t1596.tag == HirStmt_SBreak) {
        __auto_type v = _t1596.data.SBreak.val;
#line 6129 "src/sema.tr"
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
#line 6129 "src/sema.tr"
            Sema_collect_idents(self, v, out);
        }
    } else if (_t1596.tag == HirStmt_SRaise) {
        __auto_type v = _t1596.data.SRaise.val;
        Sema_collect_idents(self, v, out);
    } else if (_t1596.tag == HirStmt_SIf) {
        __auto_type c = _t1596.data.SIf.cond;
__auto_type tb = _t1596.data.SIf.then_b;
__auto_type eb = _t1596.data.SIf.else_b;
#line 6132 "src/sema.tr"
        Sema_collect_idents(self, c, out);
#line 6133 "src/sema.tr"
        Sema_collect_block_refs(self, tb, out);
#line 6134 "src/sema.tr"
        Sema_collect_block_refs(self, eb, out);
    } else if (_t1596.tag == HirStmt_SWhile) {
        __auto_type c = _t1596.data.SWhile.cond;
__auto_type b2 = _t1596.data.SWhile.body;
#line 6136 "src/sema.tr"
        Sema_collect_idents(self, c, out);
#line 6137 "src/sema.tr"
        Sema_collect_block_refs(self, b2, out);
    } else if (_t1596.tag == HirStmt_SFor) {
        __auto_type it = _t1596.data.SFor.iter;
__auto_type b2 = _t1596.data.SFor.body;
#line 6139 "src/sema.tr"
        Sema_collect_idents(self, it, out);
#line 6140 "src/sema.tr"
        Sema_collect_block_refs(self, b2, out);
    } else if (_t1596.tag == HirStmt_SForUnpack) {
        __auto_type it = _t1596.data.SForUnpack.iter;
__auto_type b2 = _t1596.data.SForUnpack.body;
#line 6142 "src/sema.tr"
        Sema_collect_idents(self, it, out);
#line 6143 "src/sema.tr"
        Sema_collect_block_refs(self, b2, out);
    } else if (_t1596.tag == HirStmt_SMatch) {
        __auto_type subj = _t1596.data.SMatch.expr;
__auto_type arms = _t1596.data.SMatch.arms;
#line 6145 "src/sema.tr"
        Sema_collect_idents(self, subj, out);
#line 6146 "src/sema.tr"
        long long i = 0LL;
#line 6147 "src/sema.tr"
        while ((i < arms->len)) {
#line 6148 "src/sema.tr"
            Sema_collect_block_refs(self, ((HirMatchArm*)List_ptr_get(arms, i))->body, out);
#line 6149 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1596.tag == HirStmt_SUnsafe) {
        __auto_type b2 = _t1596.data.SUnsafe.body;
        Sema_collect_block_refs(self, b2, out);
    } else if (1) {
        __auto_type _ = _t1596;
        /* pass */
    }
}

__attribute__((hot)) HirExpr* Sema_lower_do_value(Sema* self, Block* do_body) {
#line 6161 "src/sema.tr"
    Sema_enter_scope(self);
#line 6162 "src/sema.tr"
    HirBlock* do_hb = HirBlock_init();
#line 6163 "src/sema.tr"
    long long do_n = do_body->stmts->len;
#line 6164 "src/sema.tr"
    long long do_i = 0LL;
#line 6165 "src/sema.tr"
    HirExpr* do_val = (HirExpr*)(0LL);
#line 6166 "src/sema.tr"
    AstType* do_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 6167 "src/sema.tr"
    while ((do_i < do_n)) {
#line 6168 "src/sema.tr"
        Stmt* do_s = ((Stmt*)List_ptr_get(do_body->stmts, do_i));
#line 6169 "src/sema.tr"
        if ((do_i == (do_n - 1LL))) {
#line 6170 "src/sema.tr"
            __auto_type _t1597 = (*do_s);
            if (_t1597.tag == Stmt_SExpr) {
                __auto_type de = _t1597.data.SExpr.expr;
#line 6172 "src/sema.tr"
                do_val = Sema_lower_expr(self, de);
#line 6173 "src/sema.tr"
                do_ty = hir_expr_type(do_val);
            } else if (1) {
                __auto_type _ = _t1597;
#line 6175 "src/sema.tr"
                HirStmt* do_hs = Sema_lower_stmt(self, do_s);
#line 6176 "src/sema.tr"
                if ((((unsigned long long)(do_hs)) != ((unsigned long long)(0LL)))) {
#line 6177 "src/sema.tr"
                    Sema_apply_escape_marks(self, do_hs);
#line 6178 "src/sema.tr"
                    HirBlock_push(do_hb, do_hs);
                }
            }
        } else {
#line 6180 "src/sema.tr"
            HirStmt* do_hs2 = Sema_lower_stmt(self, do_s);
#line 6181 "src/sema.tr"
            if ((((unsigned long long)(do_hs2)) != ((unsigned long long)(0LL)))) {
#line 6182 "src/sema.tr"
                Sema_apply_escape_marks(self, do_hs2);
#line 6183 "src/sema.tr"
                HirBlock_push(do_hb, do_hs2);
            }
        }
#line 6184 "src/sema.tr"
        do_i = (do_i + 1LL);
    }
#line 6185 "src/sema.tr"
    List_TrStr* do_excl = (void*)List_TrStr_new();
#line 6186 "src/sema.tr"
    if (((((unsigned long long)(do_val)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv((do_ty->name), (_tr_str_lit_len("str", 3LL)))))) {
#line 6187 "src/sema.tr"
        Sema_collect_idents(self, do_val, do_excl);
    }
#line 6188 "src/sema.tr"
    Sema_append_drops_from_excl_multi(self, do_hb, (self->scopes->len - 1LL), do_excl);
#line 6189 "src/sema.tr"
    if ((((unsigned long long)(do_val)) != ((unsigned long long)(0LL)))) {
#line 6189 "src/sema.tr"
        HirBlock_push(do_hb, box_hirstmt(HirStmt_ctor_SExpr(do_val)));
    }
#line 6190 "src/sema.tr"
    Sema_exit_scope(self);
#line 6191 "src/sema.tr"
    HirExpr* _trret248 = box_hirexpr(HirExpr_ctor_EDo(do_hb, do_ty));
    _tr_obj_release(do_hb, _trdrop_HirBlock);
    return _trret248;
}

__attribute__((hot)) AstType* Sema_infer_break_type(Sema* self, HirBlock* hb) {
#line 6197 "src/sema.tr"
    long long i = 0LL;
#line 6198 "src/sema.tr"
    while ((i < hb->stmts->len)) {
#line 6199 "src/sema.tr"
        AstType* t = Sema_infer_break_type_stmt(self, ((HirStmt*)List_ptr_get(hb->stmts, i)));
#line 6200 "src/sema.tr"
        if ((!_tr_str_eqv((t->name), (_tr_str_lit_len("void", 4LL))))) {
#line 6200 "src/sema.tr"
            return t;
        }
#line 6201 "src/sema.tr"
        i = (i + 1LL);
    }
#line 6202 "src/sema.tr"
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) AstType* Sema_infer_break_type_stmt(Sema* self, HirStmt* s) {
#line 6205 "src/sema.tr"
    __auto_type _t1598 = (*s);
    if (_t1598.tag == HirStmt_SBreak) {
        __auto_type bv = _t1598.data.SBreak.val;
#line 6207 "src/sema.tr"
        if ((((unsigned long long)(bv)) != ((unsigned long long)(0LL)))) {
#line 6207 "src/sema.tr"
            return hir_expr_type(bv);
        }
#line 6208 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (_t1598.tag == HirStmt_SIf) {
        __auto_type then_b = _t1598.data.SIf.then_b;
__auto_type else_b = _t1598.data.SIf.else_b;
#line 6210 "src/sema.tr"
        AstType* t = Sema_infer_break_type(self, then_b);
#line 6211 "src/sema.tr"
        if ((!_tr_str_eqv((t->name), (_tr_str_lit_len("void", 4LL))))) {
#line 6211 "src/sema.tr"
            return t;
        }
#line 6212 "src/sema.tr"
        return Sema_infer_break_type(self, else_b);
    } else if (_t1598.tag == HirStmt_SMatch) {
        __auto_type m_arms = _t1598.data.SMatch.arms;
#line 6214 "src/sema.tr"
        long long ai = 0LL;
#line 6215 "src/sema.tr"
        while ((ai < m_arms->len)) {
#line 6216 "src/sema.tr"
            AstType* t2 = Sema_infer_break_type(self, ((HirMatchArm*)List_ptr_get(m_arms, ai))->body);
#line 6217 "src/sema.tr"
            if ((!_tr_str_eqv((t2->name), (_tr_str_lit_len("void", 4LL))))) {
#line 6217 "src/sema.tr"
                return t2;
            }
#line 6218 "src/sema.tr"
            ai = (ai + 1LL);
        }
#line 6219 "src/sema.tr"
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (_t1598.tag == HirStmt_STry) {
        __auto_type tb = _t1598.data.STry.try_body;
__auto_type fb = _t1598.data.STry.finally_b;
#line 6221 "src/sema.tr"
        AstType* t3 = Sema_infer_break_type(self, tb);
#line 6222 "src/sema.tr"
        if ((!_tr_str_eqv((t3->name), (_tr_str_lit_len("void", 4LL))))) {
#line 6222 "src/sema.tr"
            return t3;
        }
#line 6223 "src/sema.tr"
        return Sema_infer_break_type(self, fb);
    } else if (1) {
        __auto_type _ = _t1598;
        return AstType_init(_tr_str_lit_len("void", 4LL));
    }
}

__attribute__((hot)) void Sema_check_no_heap(Sema* self, TrStr what, TrStr fix) {
#line 6232 "src/sema.tr"
    if (self->no_heap) {
#line 6233 "src/sema.tr"
        ({ TrStr _at_t1599 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[H-1] ", 6LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" allocates on the heap, which --no-heap forbids (bare-metal zero-heap build).\n      FIX: ", 89LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fix)); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1599); _tr_str_release(_at_t1599); });
    }
}

__attribute__((hot)) HirExpr* Sema_lower_expr(Sema* self, Expr* e_ptr) {
#line 6236 "src/sema.tr"
    if ((((unsigned long long)(e_ptr)) == ((unsigned long long)(0LL)))) {
#line 6236 "src/sema.tr"
        return (HirExpr*)(0LL);
    }
#line 6237 "src/sema.tr"
    __auto_type e = (*e_ptr);
#line 6238 "src/sema.tr"
    __auto_type _t1600 = e;
    if (_t1600.tag == Expr_ELitInt) {
        __auto_type v = _t1600.data.ELitInt.val;
        return box_hirexpr(HirExpr_ctor_ELitInt(v, AstType_init(_tr_str_lit_len("int", 3LL))));
    } else if (_t1600.tag == Expr_ELitFloat) {
        __auto_type v = _t1600.data.ELitFloat.val;
        return box_hirexpr(HirExpr_ctor_ELitFloat(v, AstType_init(_tr_str_lit_len("float", 5LL))));
    } else if (_t1600.tag == Expr_ELitStr) {
        __auto_type v = _t1600.data.ELitStr.val;
__auto_type blen = _t1600.data.ELitStr.blen;
        HirExpr* _trret249 = box_hirexpr(HirExpr_ctor_ELitStr(v, blen, AstType_init(_tr_str_lit_len("str", 3LL))));
        return _trret249;
    } else if (_t1600.tag == Expr_ERawStr) {
        __auto_type v = _t1600.data.ERawStr.val;
__auto_type blen = _t1600.data.ERawStr.blen;
        HirExpr* _trret250 = box_hirexpr(HirExpr_ctor_ERawStr(v, blen, AstType_init(_tr_str_lit_len("str", 3LL))));
        return _trret250;
    } else if (_t1600.tag == Expr_ELitBytes) {
        __auto_type v = _t1600.data.ELitBytes.val;
__auto_type blen = _t1600.data.ELitBytes.blen;
        HirExpr* _trret251 = box_hirexpr(HirExpr_ctor_ELitBytes(v, blen, AstType_init(_tr_str_lit_len("Bytes", 5LL))));
        return _trret251;
    } else if (_t1600.tag == Expr_ELitBool) {
        __auto_type v = _t1600.data.ELitBool.val;
        return box_hirexpr(HirExpr_ctor_ELitBool(v, AstType_init(_tr_str_lit_len("bool", 4LL))));
    } else if (_t1600.tag == Expr_ELitChar) {
        __auto_type v = _t1600.data.ELitChar.val;
        return box_hirexpr(HirExpr_ctor_ELitChar(v, AstType_init(_tr_str_lit_len("char", 4LL))));
    } else if (_t1600.tag == Expr_ELitNone) {
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init(_tr_str_lit_len("None", 4LL))));
    } else if (_t1600.tag == Expr_EIdent) {
        __auto_type name = _t1600.data.EIdent.name;
#line 6248 "src/sema.tr"
        Symbol* sym = Sema_resolve(self, name);
#line 6249 "src/sema.tr"
        AstType* ty = (*sym->ty);
#line 6254 "src/sema.tr"
        bool _n3_recv_ok = self->in_recv_pos;
#line 6255 "src/sema.tr"
        if (_n3_recv_ok) {
#line 6258 "src/sema.tr"
            TrStr _n3_c = _tr_str_slicev((name), 0LL, 1LL);
#line 6259 "src/sema.tr"
            if (((strcmp(_tr_strz(_n3_c), _tr_strz(_tr_str_lit_len("a", 1LL))) >= 0) && (strcmp(_tr_strz(_n3_c), _tr_strz(_tr_str_lit_len("z", 1LL))) <= 0))) {
#line 6259 "src/sema.tr"
                _n3_recv_ok = false;
            }
        }
#line 6260 "src/sema.tr"
        bool _n3_known = Sema_is_known_name(self, name);
#line 6261 "src/sema.tr"
        if (((((((_n3_known && (_tr_str_lenv((name)) == 1LL)) && (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit_len("a", 1LL))) >= 0)) && (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit_len("z", 1LL))) <= 0)) && (!_tr_dict_contains(self->fn_sigs, _tr_strz(name)))) && (!_tr_dict_contains(self->extern_names, _tr_strz(name)))) && (!_tr_dict_contains(self->classes, _tr_strz(name))))) {
#line 6262 "src/sema.tr"
            _n3_known = false;
        }
#line 6263 "src/sema.tr"
        if (((((_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))) && (!_tr_str_eqv((name), (_tr_str_lit_len("", 0LL))))) && (!self->in_assign_target)) && (!_n3_recv_ok)) && (!_n3_known))) {
#line 6264 "src/sema.tr"
            ({ TrStr _at_t1601 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[N-3] name '", 12LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not defined.\n      FIX: check the spelling, declare it with 'mut ", 70LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ...', or import it before use.", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1601); _tr_str_release(_at_t1601); });
        }
#line 6267 "src/sema.tr"
        if ((_tr_dict_contains(self->fn_sigs, _tr_strz(name)) && (sym->kind.tag == SymbolKind_make_SFunction().tag))) {
#line 6268 "src/sema.tr"
            HirExpr* _trret252 = box_hirexpr(HirExpr_ctor_EIdent(name, ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(name))), false));
            return _trret252;
        }
#line 6270 "src/sema.tr"
        if ((sym->is_freed && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL)))))) {
#line 6271 "src/sema.tr"
            ({ TrStr _at_t1602 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-6] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was freed by 'dealloc()' and can no longer be used.\n      FIX: Remove all uses of '", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' after 'dealloc()', or restructure so the pointer is freed only when no longer needed.", 87LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1602); _tr_str_release(_at_t1602); });
        } else if ((((((sym->is_moved && (!self->in_assign_target_plain)) && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (!sym->is_shared))) {
#line 6274 "src/sema.tr"
            ({ TrStr _at_t1603 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving.", 123LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1603); _tr_str_release(_at_t1603); });
        } else if ((((((sym->is_maybe_moved && (!self->in_assign_target_plain)) && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (!sym->is_shared))) {
#line 6277 "src/sema.tr"
            ({ TrStr _at_t1604 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-5] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' may have been moved on some code paths, making this use unsafe.\n      FIX: Ensure '", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not moved before this point on any branch, or restructure so the use is inside the branch where it's still valid.", 118LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1604); _tr_str_release(_at_t1604); });
        }
#line 6279 "src/sema.tr"
        if ((((((!sym->is_init) && (!sym->is_maybe_init)) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
#line 6280 "src/sema.tr"
            ({ TrStr _at_t1605 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-1] Variable '", 16LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is used before being assigned a value.\n      FIX: Assign a value before use, e.g. 'mut ", 89LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = <default>'.", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1605); _tr_str_release(_at_t1605); });
        } else if (((((sym->is_maybe_init && (!sym->is_init)) && (!_tr_str_eqv((sym->name), (_tr_str_lit_len("", 0LL))))) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
#line 6283 "src/sema.tr"
            ({ TrStr _at_t1606 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[I-2] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not initialized on all code paths before this use.\n      FIX: Initialize '", 79LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' before the if/loop, or ensure every branch assigns a value.", 61LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1606); _tr_str_release(_at_t1606); });
        }
#line 6284 "src/sema.tr"
        bool is_move = false;
#line 6285 "src/sema.tr"
        if ((_tr_dict_contains(self->assign_froms, _tr_strz(name)) && (!Sema_is_primitive(self, ty)))) {
#line 6285 "src/sema.tr"
            is_move = true;
        }
#line 6286 "src/sema.tr"
        HirExpr* _trret253 = box_hirexpr(HirExpr_ctor_EIdent(name, ty, is_move));
        return _trret253;
    } else if (_t1600.tag == Expr_EBinOp) {
        __auto_type op = _t1600.data.EBinOp.op;
__auto_type left = _t1600.data.EBinOp.left;
__auto_type right = _t1600.data.EBinOp.right;
#line 6288 "src/sema.tr"
        HirExpr* hleft = Sema_lower_expr(self, left);
#line 6289 "src/sema.tr"
        HirExpr* hright = Sema_lower_expr(self, right);
#line 6290 "src/sema.tr"
        if (true) {
#line 6291 "src/sema.tr"
            TrStr _bo_lf = Sema_firm_family(self, hleft);
#line 6292 "src/sema.tr"
            TrStr _bo_rf = Sema_firm_family(self, hright);
#line 6293 "src/sema.tr"
            TrStr _bo_bad = _tr_str_retain(_bo_rf);
#line 6294 "src/sema.tr"
            if (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6294 "src/sema.tr"
                TrStr _strtmp_t1607 = _tr_str_retain(_bo_lf);
                _tr_str_release(_bo_bad);
                _bo_bad = _strtmp_t1607;
            }
#line 6295 "src/sema.tr"
            if ((((((_tr_str_eqv((op), (_tr_str_lit_len("|", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("^", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<<", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">>", 2LL)))) && (((((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("Dict", 4LL)))))) {
#line 6296 "src/sema.tr"
                ({ TrStr _at_t1608 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' needs integer operands, but one side is ", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _bo_bad)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: convert it to an int first.", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1608); _tr_str_release(_at_t1608); });
            }
#line 6297 "src/sema.tr"
            if (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL)))) && ((((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))))) {
#line 6298 "src/sema.tr"
                ({ TrStr _at_t1609 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not defined for strings.\n      FIX: only '+' (concatenate), comparisons, 'str * int' (repeat) and 'in' work on strings; build text with f-strings or str(x).", 161LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1609); _tr_str_release(_at_t1609); });
            }
#line 6299 "src/sema.tr"
            if ((_tr_str_eqv((op), (_tr_str_lit_len("is", 2LL))) && (!_tr_str_eqv((_bo_rf), (_tr_str_lit_len("", 0LL)))))) {
#line 6300 "src/sema.tr"
                ({ TrStr _at_t1610 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Sema_family_word(self, _bo_rf)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the right-hand side of 'is' must be an enum variant or a type, not ", 88LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: compare values with '==' (e.g. `x == 3`); use `is` only as `shape is Shape.Circle`.", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1610); _tr_str_release(_at_t1610); });
            }
#line 6301 "src/sema.tr"
            if ((((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("not in", 6LL)))) && ((_tr_str_eqv((_bo_rf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("bool", 4LL))))) && (!(((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))))))) {
#line 6302 "src/sema.tr"
                ({ TrStr _at_t1611 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the right-hand side of '", 45LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' must be a list, dict, set or string, but it is ", 49LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _bo_rf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: put the collection (or the string to search in) on the right: `item in collection`.", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1611); _tr_str_release(_at_t1611); });
            }
#line 6303 "src/sema.tr"
            bool _bo_ls = (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))));
#line 6304 "src/sema.tr"
            bool _bo_rs = (((_tr_str_eqv((_bo_rf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL))));
#line 6305 "src/sema.tr"
            if ((_bo_ls && _bo_rs)) {
#line 6306 "src/sema.tr"
                bool _bo_mixed = (_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) != _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL))));
#line 6307 "src/sema.tr"
                bool _bo_cmp = (((((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL))));
#line 6308 "src/sema.tr"
                bool _bo_arith = ((((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("**", 2LL))));
#line 6309 "src/sema.tr"
                if ((_bo_mixed && (_bo_cmp || _bo_arith))) {
#line 6310 "src/sema.tr"
                    if ((!(_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL))) && (_tr_str_eqv((_bo_lf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("int", 3LL))))))) {
#line 6311 "src/sema.tr"
                        ({ __auto_type _ord_t1612 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: cannot apply '", 35LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _bo_lf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1613 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1612); TrStr _tr_cr = (Sema_family_word(self, _bo_rf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: convert one side first, e.g. str(x) or int(s), so both operands have the same type.", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1613); _tr_str_release(_at_t1613); });
                    }
                } else if (((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("not in", 6LL)))) && (((_tr_str_eqv((_bo_rf), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_bo_rf), (_tr_str_lit_len("bool", 4LL)))) || (_tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL))) && (!_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL)))))))) {
#line 6313 "src/sema.tr"
                    ({ TrStr _at_t1614 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: the right-hand side of '", 45LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' must be a list, dict, set or string, but it is ", 49LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _bo_rf)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".\n      FIX: put the collection (or the string to search in) on the right: `item in collection`.", 96LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1614); _tr_str_release(_at_t1614); });
                } else if (((_tr_str_eqv((_bo_lf), (_tr_str_lit_len("str", 3LL))) && _tr_str_eqv((_bo_rf), (_tr_str_lit_len("str", 3LL)))) && (((((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))))) {
#line 6315 "src/sema.tr"
                    ({ TrStr _at_t1615 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not defined for two strings.\n      FIX: only '+' (concatenate), comparisons, and 'str * int' (repeat) work on strings.", 123LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1615); _tr_str_release(_at_t1615); });
                }
            }
        }
#line 6317 "src/sema.tr"
        if ((self->no_heap && (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))))) {
#line 6318 "src/sema.tr"
            TrStr _nh_l = _tr_str_retain(hir_expr_type(hleft)->name);
#line 6319 "src/sema.tr"
            TrStr _nh_r = _tr_str_retain(hir_expr_type(hright)->name);
#line 6320 "src/sema.tr"
            if ((((_tr_str_eqv((_nh_l), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_nh_l), (_tr_str_lit_len("String", 6LL)))) || _tr_str_eqv((_nh_r), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((_nh_r), (_tr_str_lit_len("String", 6LL))))) {
#line 6321 "src/sema.tr"
                TrStr _nh_what = _tr_str_lit_len("string concatenation (+)", 24LL);
#line 6322 "src/sema.tr"
                if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 6322 "src/sema.tr"
                    TrStr _strtmp_t1616 = _tr_str_lit_len("string repetition (*)", 21LL);
                    _tr_str_release(_nh_what);
                    _nh_what = _strtmp_t1616;
                }
#line 6323 "src/sema.tr"
                Sema_check_no_heap(self, _nh_what, _tr_str_lit_len("keep string literals (they live in rodata) or use a StrView / fixed byte buffer; do not build new strings", 105LL));
            }
        }
#line 6330 "src/sema.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL))))) {
#line 6331 "src/sema.tr"
            TrStr lref = Sema_type_ref_name(self, left);
#line 6332 "src/sema.tr"
            TrStr rref = Sema_type_ref_name(self, right);
#line 6333 "src/sema.tr"
            if (((!_tr_str_eqv((lref), (_tr_str_lit_len("", 0LL)))) || (!_tr_str_eqv((rref), (_tr_str_lit_len("", 0LL)))))) {
#line 6334 "src/sema.tr"
                TrStr lname = _tr_str_retain(lref);
#line 6335 "src/sema.tr"
                if (_tr_str_eqv((lname), (_tr_str_lit_len("", 0LL)))) {
#line 6335 "src/sema.tr"
                    TrStr _strtmp_t1617 = _tr_str_retain(hir_expr_type(hleft)->name);
                    _tr_str_release(lname);
                    lname = _strtmp_t1617;
                }
#line 6336 "src/sema.tr"
                TrStr rname = _tr_str_retain(rref);
#line 6337 "src/sema.tr"
                if (_tr_str_eqv((rname), (_tr_str_lit_len("", 0LL)))) {
#line 6337 "src/sema.tr"
                    TrStr _strtmp_t1618 = _tr_str_retain(hir_expr_type(hright)->name);
                    _tr_str_release(rname);
                    rname = _strtmp_t1618;
                }
#line 6338 "src/sema.tr"
                HirExpr* _trret254 = ({ __auto_type _ord_t1619 = (_tr_str_lenv((lname))); __auto_type _ord_t1620 = (AstType_init(_tr_str_lit_len("str", 3LL))); __auto_type _ord_t1621 = (box_hirexpr(HirExpr_ctor_ELitStr(lname, _ord_t1619, _ord_t1620))); __auto_type _ord_t1622 = (_tr_str_lenv((rname))); __auto_type _ord_t1623 = (AstType_init(_tr_str_lit_len("str", 3LL))); __auto_type _ord_t1624 = (box_hirexpr(HirExpr_ctor_ELitStr(rname, _ord_t1622, _ord_t1623))); __auto_type _ord_t1625 = (AstType_init(_tr_str_lit_len("bool", 4LL))); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EBinOp(op, _ord_t1621, _ord_t1624, _ord_t1625))); _wr; });
                _tr_str_release(lref);
                _tr_str_release(rref);
                _tr_str_release(lname);
                _tr_str_release(rname);
                return _trret254;
            }
        }
#line 6339 "src/sema.tr"
        AstType* bin_ty = hir_expr_type(hleft);
#line 6340 "src/sema.tr"
        if (_tr_str_eqv((bin_ty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 6340 "src/sema.tr"
            bin_ty = hir_expr_type(hright);
        }
#line 6341 "src/sema.tr"
        if ((((((((((((_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("or", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("&&", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("||", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("in", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("not in", 6LL))))) {
#line 6342 "src/sema.tr"
            bin_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
        } else if ((_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL))) && (((_tr_str_eqv((hir_expr_type(hleft)->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((hir_expr_type(hleft)->name), (_tr_str_lit_len("String", 6LL)))) || _tr_str_eqv((hir_expr_type(hright)->name), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((hir_expr_type(hright)->name), (_tr_str_lit_len("String", 6LL)))))) {
#line 6344 "src/sema.tr"
            bin_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if (((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) && (_binop_is_float_name(hir_expr_type(hleft)->name) || _binop_is_float_name(hir_expr_type(hright)->name)))) {
#line 6346 "src/sema.tr"
            bin_ty = AstType_init(_tr_str_lit_len("float", 5LL));
        }
#line 6347 "src/sema.tr"
        HirExpr* _trret255 = box_hirexpr(HirExpr_ctor_EBinOp(op, hleft, hright, bin_ty));
        return _trret255;
    } else if (_t1600.tag == Expr_EUnaryOp) {
        __auto_type op = _t1600.data.EUnaryOp.op;
__auto_type expr = _t1600.data.EUnaryOp.expr;
#line 6349 "src/sema.tr"
        HirExpr* hexpr_inner = Sema_lower_expr(self, expr);
#line 6350 "src/sema.tr"
        if (((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) && ({ TrStr _cmpa_t1626 = (Sema_firm_family(self, hexpr_inner)); TrStr _cmpb_t1627 = (_tr_str_lit_len("str", 3LL)); _Bool _cmpr_t1628 = _tr_str_eqv(_cmpa_t1626, _cmpb_t1627); _tr_str_release(_cmpa_t1626); _cmpr_t1628; }))) {
#line 6351 "src/sema.tr"
            ({ TrStr _at_t1629 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-5] Type mismatch: unary '", 28LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' cannot be applied to a str.\n      FIX: convert it first, e.g. -int(s).", 72LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1629); _tr_str_release(_at_t1629); });
        }
#line 6352 "src/sema.tr"
        AstType* inner_ty = hir_expr_type(hexpr_inner);
#line 6353 "src/sema.tr"
        AstType* un_ty = inner_ty;
#line 6354 "src/sema.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
#line 6355 "src/sema.tr"
            AstType* addr_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
#line 6356 "src/sema.tr"
            ({ __auto_type _ord_t1630 = (box_asttype(inner_ty)); List_ptr_append(addr_ty->args, _ord_t1630); });
#line 6357 "src/sema.tr"
            un_ty = addr_ty;
        } else if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 6359 "src/sema.tr"
            if ((_tr_str_eqv((inner_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (inner_ty->args->len > 0LL))) {
#line 6360 "src/sema.tr"
                un_ty = (*((AstType**)List_ptr_get(inner_ty->args, 0LL)));
            }
        }
#line 6361 "src/sema.tr"
        HirExpr* _trret256 = box_hirexpr(HirExpr_ctor_EUnaryOp(op, hexpr_inner, un_ty));
        return _trret256;
    } else if (_t1600.tag == Expr_ECall) {
        __auto_type callee = _t1600.data.ECall.callee;
__auto_type args = _t1600.data.ECall.args;
#line 6364 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6365 "src/sema.tr"
            __auto_type _t1631 = (*callee);
            if (_t1631.tag == Expr_EIdent) {
                __auto_type ato_n = _t1631.data.EIdent.name;
#line 6368 "src/sema.tr"
                if ((self->no_heap && Sema__is_rc_class(self, ato_n))) {
#line 6369 "src/sema.tr"
                    ({ TrStr _at_t1632 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("constructing heap class '", 25LL)), (ato_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_no_heap(self, _at_t1632, _tr_str_lit_len("mark the class '@value_type' (stack; all-scalar fields), or place the instance in caller-owned memory via a Pointer[T]", 118LL)); _tr_str_release(_at_t1632); });
                }
#line 6370 "src/sema.tr"
                if (_tr_str_eqv((ato_n), (_tr_str_lit_len("await_timeout", 13LL)))) {
#line 6371 "src/sema.tr"
                    if ((!self->in_async_fn)) {
#line 6372 "src/sema.tr"
                        Sema_error(self, _tr_str_lit_len("[C-4] 'await_timeout' used outside an async function.", 53LL));
                    }
#line 6373 "src/sema.tr"
                    if ((args->len < 2LL)) {
#line 6374 "src/sema.tr"
                        Sema_error(self, _tr_str_lit_len("await_timeout requires 2 arguments: await_timeout(expr, timeout_ms)", 67LL));
#line 6375 "src/sema.tr"
                        HirExpr* _trret257 = box_hirexpr(HirExpr_ctor_ELitInt(0LL, AstType_init(_tr_str_lit_len("int", 3LL))));
                        return _trret257;
                    }
#line 6376 "src/sema.tr"
                    HirExpr* ato_inner = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
#line 6377 "src/sema.tr"
                    HirExpr* ato_ms = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 1LL)));
#line 6378 "src/sema.tr"
                    AstType* ato_ty = hir_expr_type(ato_inner);
#line 6379 "src/sema.tr"
                    HirExpr* _trret258 = box_hirexpr(HirExpr_ctor_EAwaitTimeout(ato_inner, ato_ms, ato_ty));
                    return _trret258;
                }
#line 6383 "src/sema.tr"
                if (_tr_str_eqv((ato_n), (_tr_str_lit_len("instanceOf", 10LL)))) {
#line 6384 "src/sema.tr"
                    if ((args->len < 2LL)) {
#line 6385 "src/sema.tr"
                        Sema_error(self, _tr_str_lit_len("instanceOf requires 2 arguments: instanceOf(obj, T)", 51LL));
#line 6386 "src/sema.tr"
                        HirExpr* _trret259 = box_hirexpr(HirExpr_ctor_ELitBool(false, AstType_init(_tr_str_lit_len("bool", 4LL))));
                        return _trret259;
                    }
#line 6387 "src/sema.tr"
                    HirExpr* io_obj = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
#line 6388 "src/sema.tr"
                    TrStr io_obj_ty_n = _tr_str_retain(hir_expr_type(io_obj)->name);
#line 6389 "src/sema.tr"
                    TrStr io_target_n = _tr_str_lit_len("", 0LL);
#line 6390 "src/sema.tr"
                    __auto_type _t1633 = (*((Expr*)List_ptr_get(args, 1LL)));
                    if (_t1633.tag == Expr_EIdent) {
                        __auto_type io_tn = _t1633.data.EIdent.name;
                        TrStr _strtmp_t1634 = _tr_str_retain(io_tn);
                        _tr_str_release(io_target_n);
                        io_target_n = _strtmp_t1634;
                    } else if (_t1633.tag == Expr_EIndex) {
                        __auto_type io_base = _t1633.data.EIndex.obj;
#line 6393 "src/sema.tr"
                        __auto_type _t1635 = (*io_base);
                        if (_t1635.tag == Expr_EIdent) {
                            __auto_type io_tn2 = _t1635.data.EIdent.name;
                            TrStr _strtmp_t1636 = _tr_str_retain(io_tn2);
                            _tr_str_release(io_target_n);
                            io_target_n = _strtmp_t1636;
                        } else if (1) {
                            __auto_type _ = _t1635;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t1633;
                        /* pass */
                    }
#line 6397 "src/sema.tr"
                    HirExpr* _trret260 = box_hirexpr(HirExpr_ctor_ELitBool(_tr_str_eqv((io_obj_ty_n), (io_target_n)), AstType_init(_tr_str_lit_len("bool", 4LL))));
                    _tr_str_release(io_obj_ty_n);
                    _tr_str_release(io_target_n);
                    return _trret260;
                }
#line 6401 "src/sema.tr"
                if (_tr_str_eqv((ato_n), (_tr_str_lit_len("inspect", 7LL)))) {
#line 6402 "src/sema.tr"
                    if ((args->len < 1LL)) {
#line 6403 "src/sema.tr"
                        Sema_error(self, _tr_str_lit_len("inspect requires 1 argument: inspect(T)", 39LL));
#line 6404 "src/sema.tr"
                        HirExpr* _trret261 = box_hirexpr(HirExpr_ctor_ELitStr(_tr_str_lit_len("", 0LL), 0LL, AstType_init(_tr_str_lit_len("str", 3LL))));
                        return _trret261;
                    }
#line 6405 "src/sema.tr"
                    TrStr isp_target_n = _tr_str_lit_len("", 0LL);
#line 6406 "src/sema.tr"
                    __auto_type _t1637 = (*((Expr*)List_ptr_get(args, 0LL)));
                    if (_t1637.tag == Expr_EIdent) {
                        __auto_type isp_tn = _t1637.data.EIdent.name;
                        TrStr _strtmp_t1638 = _tr_str_retain(isp_tn);
                        _tr_str_release(isp_target_n);
                        isp_target_n = _strtmp_t1638;
                    } else if (_t1637.tag == Expr_EIndex) {
                        __auto_type isp_base = _t1637.data.EIndex.obj;
#line 6409 "src/sema.tr"
                        __auto_type _t1639 = (*isp_base);
                        if (_t1639.tag == Expr_EIdent) {
                            __auto_type isp_tn2 = _t1639.data.EIdent.name;
                            TrStr _strtmp_t1640 = _tr_str_retain(isp_tn2);
                            _tr_str_release(isp_target_n);
                            isp_target_n = _strtmp_t1640;
                        } else if (1) {
                            __auto_type _ = _t1639;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t1637;
                        /* pass */
                    }
#line 6413 "src/sema.tr"
                    if (_tr_str_eqv((isp_target_n), (_tr_str_lit_len("", 0LL)))) {
#line 6414 "src/sema.tr"
                        HirExpr* isp_obj = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
#line 6415 "src/sema.tr"
                        TrStr _strtmp_t1641 = _tr_str_retain(hir_expr_type(isp_obj)->name);
                        _tr_str_release(isp_target_n);
                        isp_target_n = _strtmp_t1641;
                    }
#line 6416 "src/sema.tr"
                    TrStr isp_str = Sema_build_inspect_str(self, isp_target_n);
#line 6417 "src/sema.tr"
                    HirExpr* _trret262 = ({ __auto_type _ord_t1642 = (_tr_str_lenv((isp_str))); __auto_type _ord_t1643 = (AstType_init(_tr_str_lit_len("str", 3LL))); __auto_type _wr = (box_hirexpr(HirExpr_ctor_ELitStr(isp_str, _ord_t1642, _ord_t1643))); _wr; });
                    _tr_str_release(isp_target_n);
                    _tr_str_release(isp_str);
                    return _trret262;
                }
            } else if (1) {
                __auto_type _ = _t1631;
                /* pass */
            }
        }
#line 6420 "src/sema.tr"
        List_TrStr* p23_borrow_names = (void*)List_TrStr_new();
#line 6421 "src/sema.tr"
        TrMap* p23_seen = _tr_dict_new(4LL);
#line 6422 "src/sema.tr"
        long long p23_k = 0LL;
#line 6423 "src/sema.tr"
        while ((p23_k < args->len)) {
#line 6424 "src/sema.tr"
            Expr* p23_arg = ((Expr*)List_ptr_get(args, p23_k));
#line 6425 "src/sema.tr"
            TrStr p23_nm = _tr_str_lit_len("", 0LL);
#line 6426 "src/sema.tr"
            if ((((unsigned long long)(p23_arg)) != ((unsigned long long)(0LL)))) {
#line 6427 "src/sema.tr"
                __auto_type _t1644 = (*p23_arg);
                if (_t1644.tag == Expr_EIdent) {
                    __auto_type p23_n = _t1644.data.EIdent.name;
                    TrStr _strtmp_t1645 = _tr_str_retain(p23_n);
                    _tr_str_release(p23_nm);
                    p23_nm = _strtmp_t1645;
                } else if (1) {
                    __auto_type _ = _t1644;
                    /* pass */
                }
            }
#line 6430 "src/sema.tr"
            if ((!_tr_str_eqv((p23_nm), (_tr_str_lit_len("", 0LL))))) {
#line 6431 "src/sema.tr"
                Symbol* p23_sym = Sema_resolve(self, p23_nm);
#line 6432 "src/sema.tr"
                if (((!Sema_is_primitive(self, (*p23_sym->ty))) && (!_tr_str_eqv((p23_sym->name), (_tr_str_lit_len("", 0LL)))))) {
#line 6433 "src/sema.tr"
                    Sema_mark_borrow(self, p23_nm);
#line 6434 "src/sema.tr"
                    List_TrStr_append(p23_borrow_names, p23_nm);
#line 6435 "src/sema.tr"
                    if (_tr_dict_contains(p23_seen, _tr_strz(p23_nm))) {
#line 6436 "src/sema.tr"
                        ({ TrStr _at_t1646 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-3] '", 7LL)), (p23_nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' appears twice in the same call, creating aliased mutable access.\n      FIX: Clone one of the arguments: ", 106LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p23_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".clone()", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1646); _tr_str_release(_at_t1646); });
                    }
#line 6437 "src/sema.tr"
                    _tr_dict_set(p23_seen, _tr_strz(p23_nm), true);
                }
            }
#line 6438 "src/sema.tr"
            p23_k = (p23_k + 1LL);
            _tr_str_release(p23_nm);
        }
#line 6439 "src/sema.tr"
        List_ptr* hl = (void*)List_ptr_new();
#line 6440 "src/sema.tr"
        TrStr _ch_fname = _tr_str_lit_len("", 0LL);
#line 6441 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6442 "src/sema.tr"
            __auto_type _t1647 = (*callee);
            if (_t1647.tag == Expr_EIdent) {
                __auto_type _ch_n = _t1647.data.EIdent.name;
                TrStr _strtmp_t1648 = _tr_str_retain(_ch_n);
                _tr_str_release(_ch_fname);
                _ch_fname = _strtmp_t1648;
            } else if (1) {
                __auto_type _ = _t1647;
                /* pass */
            }
        }
#line 6445 "src/sema.tr"
        long long k = 0LL;
#line 6446 "src/sema.tr"
        while ((k < args->len)) {
#line 6447 "src/sema.tr"
            self->closure_hint = (AstType**)(0LL);
#line 6448 "src/sema.tr"
            if ((!_tr_str_eqv((_ch_fname), (_tr_str_lit_len("", 0LL))))) {
#line 6449 "src/sema.tr"
                __auto_type _t1649 = (*((Expr*)List_ptr_get(args, k)));
                if (_t1649.tag == Expr_EClosure) {
                    self->closure_hint = Sema_closure_hint_for_fn(self, _ch_fname, k);
                } else if (_t1649.tag == Expr_EList) {
                    __auto_type _lh_items = _t1649.data.EList.items;
#line 6452 "src/sema.tr"
                    if ((_lh_items->len == 0LL)) {
#line 6452 "src/sema.tr"
                        self->list_hint = Sema_list_hint_for_fn(self, _ch_fname, k);
                    }
                } else if (1) {
                    __auto_type _ = _t1649;
                    /* pass */
                }
            }
#line 6454 "src/sema.tr"
            HirExpr* harg = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k)));
#line 6456 "src/sema.tr"
            if (((!_tr_str_eqv((_ch_fname), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->fn_sigs, _tr_strz(_ch_fname)))) {
#line 6457 "src/sema.tr"
                AstType* sig_n = ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(_ch_fname)));
#line 6458 "src/sema.tr"
                if (((k < (sig_n->args->len - 1LL)) && (((unsigned long long)(((AstType**)List_ptr_get(sig_n->args, k)))) != ((unsigned long long)(0LL))))) {
#line 6459 "src/sema.tr"
                    harg = Sema_retype_none(self, harg, (*((AstType**)List_ptr_get(sig_n->args, k))));
                }
            }
#line 6460 "src/sema.tr"
            List_ptr_append(hl, harg);
#line 6461 "src/sema.tr"
            self->closure_hint = (AstType**)(0LL);
#line 6462 "src/sema.tr"
            self->list_hint = (AstType**)(0LL);
#line 6463 "src/sema.tr"
            k = (k + 1LL);
        }
#line 6465 "src/sema.tr"
        long long p23_ui = 0LL;
#line 6466 "src/sema.tr"
        while ((p23_ui < p23_borrow_names->len)) {
#line 6467 "src/sema.tr"
            ({ TrStr _at_t1650 = (List_TrStr_get(p23_borrow_names, p23_ui)); Sema_unmark_borrow(self, _at_t1650); _tr_str_release(_at_t1650); });
#line 6468 "src/sema.tr"
            p23_ui = (p23_ui + 1LL);
        }
#line 6471 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6472 "src/sema.tr"
            __auto_type _t1651 = (*callee);
            if (_t1651.tag == Expr_EIdent) {
                __auto_type vfn_n = _t1651.data.EIdent.name;
#line 6474 "src/sema.tr"
                if (_tr_dict_contains(self->variadic_fns, _tr_strz(vfn_n))) {
#line 6475 "src/sema.tr"
                    long long vfixed = _tr_str_to_int(_tr_strz(_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->variadic_fns, _tr_strz(vfn_n))))));
#line 6476 "src/sema.tr"
                    AstType* velem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 6477 "src/sema.tr"
                    if (_tr_dict_contains(self->variadic_elem_ty, _tr_strz(vfn_n))) {
#line 6478 "src/sema.tr"
                        velem_ty = (*((AstType**)(uintptr_t)_tr_dict_get(self->variadic_elem_ty, _tr_strz(vfn_n))));
                    }
#line 6479 "src/sema.tr"
                    if ((hl->len >= vfixed)) {
#line 6480 "src/sema.tr"
                        List_ptr* vargs = (void*)List_ptr_new();
#line 6481 "src/sema.tr"
                        long long vi = vfixed;
#line 6482 "src/sema.tr"
                        while ((vi < hl->len)) {
#line 6483 "src/sema.tr"
                            List_ptr_append(vargs, ((HirExpr*)List_ptr_get(hl, vi)));
#line 6484 "src/sema.tr"
                            vi = (vi + 1LL);
                        }
#line 6485 "src/sema.tr"
                        AstType* vlist_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 6486 "src/sema.tr"
                        ({ __auto_type _ord_t1652 = (box_asttype(velem_ty)); List_ptr_append(vlist_ty->args, _ord_t1652); });
#line 6487 "src/sema.tr"
                        List_ptr* vnew_hl = (void*)List_ptr_new();
#line 6488 "src/sema.tr"
                        long long vk = 0LL;
#line 6489 "src/sema.tr"
                        while ((vk < vfixed)) {
#line 6490 "src/sema.tr"
                            List_ptr_append(vnew_hl, ((HirExpr*)List_ptr_get(hl, vk)));
#line 6491 "src/sema.tr"
                            vk = (vk + 1LL);
                        }
#line 6492 "src/sema.tr"
                        List_ptr_append(vnew_hl, box_hirexpr(HirExpr_ctor_EList(vargs, vlist_ty)));
#line 6493 "src/sema.tr"
                        hl = vnew_hl;
                    }
                }
            } else if (1) {
                __auto_type _ = _t1651;
                /* pass */
            }
        }
#line 6495 "src/sema.tr"
        HirExpr* hcallee = Sema_lower_expr(self, callee);
#line 6496 "src/sema.tr"
        AstType* ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 6497 "src/sema.tr"
        if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
#line 6497 "src/sema.tr"
            HirExpr* _trret263 = box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
            List_TrStr_free(p23_borrow_names);
            Dict_free(p23_seen);
            _tr_str_release(_ch_fname);
            return _trret263;
        }
#line 6498 "src/sema.tr"
        __auto_type _t1653 = (*callee);
        if (_t1653.tag == Expr_EIdent) {
            __auto_type n = _t1653.data.EIdent.name;
#line 6501 "src/sema.tr"
            Sema_check_call_bounds(self, n, hl);
#line 6502 "src/sema.tr"
            Sema_check_call_fn_args(self, n, hl);
#line 6503 "src/sema.tr"
            if (_tr_str_eqv((n), (_tr_str_lit_len("main", 4LL)))) {
#line 6504 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[E-1] Explicit call to 'main()' is forbidden. The compiler automatically invokes main() as the program entry point. Remove the 'main()' call from your source.", 158LL));
#line 6505 "src/sema.tr"
                HirExpr* _trret264 = box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
                List_TrStr_free(p23_borrow_names);
                Dict_free(p23_seen);
                _tr_str_release(_ch_fname);
                return _trret264;
            }
#line 6506 "src/sema.tr"
            if ((((((((_tr_dict_contains(self->classes, _tr_strz(n)) || _tr_str_eqv((n), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6507 "src/sema.tr"
                ret_ty = AstType_init(n);
            } else if (_tr_dict_contains(self->enums, _tr_strz(n))) {
#line 6509 "src/sema.tr"
                ret_ty = AstType_init(n);
            } else if ((_tr_str_eqv((Sema_resolve(self, n)->name), (_tr_str_lit_len("", 0LL))) && Sema__is_type_param_in_scope(self, n))) {
#line 6518 "src/sema.tr"
                ret_ty = AstType_init(n);
            } else if ((((((_tr_str_eqv((n), (_tr_str_lit_len("min", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("sum", 3LL)))) && (hl->len == 1LL)) && (hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))->args->len > 0LL)) && (_tr_str_eqv((hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))->name), (_tr_str_lit_len("Vec", 3LL)))))) {
#line 6520 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))->args, 0LL)));
            } else if ((((_tr_str_eqv((n), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("max", 3LL)))) && _hl_has_float(hl))) {
#line 6522 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("float", 5LL));
            } else if (_tr_str_eqv((n), (_tr_str_lit_len("c_callback", 10LL)))) {
#line 6525 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
#line 6526 "src/sema.tr"
                ({ __auto_type _ord_t1654 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1654); });
            } else if ((_tr_str_eqv((n), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("dealloc", 7LL))))) {
#line 6528 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
#line 6529 "src/sema.tr"
                if ((!self->in_unsafe)) {
#line 6530 "src/sema.tr"
                    ({ TrStr _at_t1655 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[U-1] '", 7LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' used outside an 'unsafe:' block.\n      FIX: Wrap raw memory operations in 'unsafe:' to signal manual memory management, e.g.\n          unsafe:\n              p = ", 163LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T](n)", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1655); _tr_str_release(_at_t1655); });
                }
            } else {
#line 6532 "src/sema.tr"
                Symbol* _fsym = Sema_resolve(self, n);
#line 6533 "src/sema.tr"
                ret_ty = (*_fsym->ty);
#line 6536 "src/sema.tr"
                if ((_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("def", 3LL))) && (ret_ty->args->len > 0LL))) {
#line 6537 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(ret_ty->args, (ret_ty->args->len - 1LL))));
                } else if ((_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("lambda", 6LL))) && (_fsym->kind.tag == SymbolKind_make_SVariable().tag))) {
#line 6542 "src/sema.tr"
                    if ((ret_ty->args->len > 0LL)) {
#line 6543 "src/sema.tr"
                        ret_ty = (*((AstType**)List_ptr_get(ret_ty->args, (ret_ty->args->len - 1LL))));
                    } else {
#line 6545 "src/sema.tr"
                        ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                    }
                }
#line 6549 "src/sema.tr"
                if (_tr_dict_contains(self->fn_defs, _tr_strz(n))) {
#line 6550 "src/sema.tr"
                    FunctionDef* _gfd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(n)));
#line 6553 "src/sema.tr"
                    if ((((_gfd->generics->len > 0LL) && (_gfd->constraints->len == 0LL)) && (self->current_func_generics->len == 0LL))) {
#line 6554 "src/sema.tr"
                        long long _ngi = 0LL;
#line 6555 "src/sema.tr"
                        while ((_ngi < _gfd->generics->len)) {
#line 6556 "src/sema.tr"
                            TrStr _ngp = List_TrStr_get(_gfd->generics, _ngi);
#line 6557 "src/sema.tr"
                            bool _nfound = false;
#line 6558 "src/sema.tr"
                            long long _nsk = 0LL;
#line 6559 "src/sema.tr"
                            if (((_gfd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_gfd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 6559 "src/sema.tr"
                                _nsk = 1LL;
                            }
#line 6560 "src/sema.tr"
                            long long _npi = _nsk;
#line 6561 "src/sema.tr"
                            while ((_npi < _gfd->params->len)) {
#line 6562 "src/sema.tr"
                                AstType** _npty = ((Param*)List_ptr_get(_gfd->params, _npi))->ty;
#line 6563 "src/sema.tr"
                                if (((((unsigned long long)(_npty)) != ((unsigned long long)(0LL))) && type_mentions((*_npty), _ngp))) {
#line 6563 "src/sema.tr"
                                    _nfound = true;
                                }
#line 6564 "src/sema.tr"
                                _npi = (_npi + 1LL);
                            }
#line 6565 "src/sema.tr"
                            if (((!_nfound) && (_gfd->generics->len == 1LL))) {
#line 6566 "src/sema.tr"
                                ({ TrStr _at_t1656 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[G-2] Cannot infer type parameter '", 35LL)), (_ngp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of generic function '", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from its arguments.\n      FIX: give it explicitly: ", 53LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (n)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[Type](...)", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1656); _tr_str_release(_at_t1656); });
                            }
#line 6567 "src/sema.tr"
                            _ngi = (_ngi + 1LL);
                            _tr_str_release(_ngp);
                        }
                    }
#line 6568 "src/sema.tr"
                    if (({ __auto_type _ord_t1657 = (Sema_resolve(self, n)->kind); __auto_type _wr = ((_ord_t1657.tag == SymbolKind_make_SFunction().tag)); _wr; })) {
#line 6569 "src/sema.tr"
                        ({ TrStr _at_t1658 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_call_args(self, _gfd, hl, 0LL, _at_t1658); _tr_str_release(_at_t1658); });
                    }
#line 6570 "src/sema.tr"
                    if (((_gfd->generics->len > 0LL) && List_TrStr_contains(_gfd->generics, ret_ty->name))) {
#line 6571 "src/sema.tr"
                        long long _gsk = 0LL;
#line 6572 "src/sema.tr"
                        if (((_gfd->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_gfd->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 6572 "src/sema.tr"
                            _gsk = 1LL;
                        }
#line 6573 "src/sema.tr"
                        long long _gpi = 0LL;
#line 6574 "src/sema.tr"
                        while ((_gpi < _gfd->params->len)) {
#line 6575 "src/sema.tr"
                            AstType** _gpty = ((Param*)List_ptr_get(_gfd->params, _gpi))->ty;
#line 6576 "src/sema.tr"
                            if (((((unsigned long long)(_gpty)) != ((unsigned long long)(0LL))) && ({ __auto_type _ord_t1659 = ((*_gpty)->name); __auto_type _wr = (_tr_str_eqv((_ord_t1659), (ret_ty->name))); _wr; }))) {
#line 6577 "src/sema.tr"
                                long long _gaidx = (_gpi - _gsk);
#line 6578 "src/sema.tr"
                                if (((_gaidx >= 0LL) && (_gaidx < hl->len))) {
#line 6579 "src/sema.tr"
                                    ret_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _gaidx)));
#line 6580 "src/sema.tr"
                                    break;
                                }
                            } else if (((((unsigned long long)(_gpty)) != ((unsigned long long)(0LL))) && ((*_gpty)->args->len > 0LL))) {
#line 6582 "src/sema.tr"
                                long long _gaidx2 = (_gpi - _gsk);
#line 6583 "src/sema.tr"
                                if (((_gaidx2 >= 0LL) && (_gaidx2 < hl->len))) {
#line 6584 "src/sema.tr"
                                    TrStr _gut = ({ __auto_type _ord_t1660 = ((*_gpty)); __auto_type _wr = (unify_generic(_ord_t1660, hir_expr_type(((HirExpr*)List_ptr_get(hl, _gaidx2))), ret_ty->name)); _wr; });
#line 6585 "src/sema.tr"
                                    if ((!_tr_str_eqv((_gut), (_tr_str_lit_len("", 0LL))))) {
#line 6586 "src/sema.tr"
                                        ret_ty = AstType_init(_gut);
#line 6587 "src/sema.tr"
                                        _tr_str_release(_gut);
                                        break;
                                    }
                                }
                            }
#line 6588 "src/sema.tr"
                            _gpi = (_gpi + 1LL);
                        }
                    }
                }
            }
        } else if (_t1653.tag == Expr_EIndex) {
            __auto_type base = _t1653.data.EIndex.obj;
__auto_type idx = _t1653.data.EIndex._tr_v_index;
#line 6590 "src/sema.tr"
            __auto_type _t1661 = (*base);
            if (_t1661.tag == Expr_EIdent) {
                __auto_type gn = _t1661.data.EIdent.name;
#line 6592 "src/sema.tr"
                if ((_tr_str_eqv((gn), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((gn), (_tr_str_lit_len("dealloc", 7LL))))) {
#line 6593 "src/sema.tr"
                    __auto_type _t1662 = (*idx);
                    if (_t1662.tag == Expr_EIdent) {
                        __auto_type tn = _t1662.data.EIdent.name;
#line 6595 "src/sema.tr"
                        ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
#line 6596 "src/sema.tr"
                        ({ __auto_type _ord_t1663 = (box_asttype(AstType_init(tn))); List_ptr_append(ret_ty->args, _ord_t1663); });
                    } else if (1) {
                        __auto_type _ = _t1662;
#line 6598 "src/sema.tr"
                        ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
                    }
#line 6599 "src/sema.tr"
                    if ((!self->in_unsafe)) {
#line 6600 "src/sema.tr"
                        ({ TrStr _at_t1664 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[U-1] 'alloc'/'dealloc' used outside an 'unsafe:' block.\n      FIX: Wrap raw memory operations in 'unsafe:' to signal manual memory management, e.g.\n          unsafe:\n              p = ", 185LL)), (gn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T](n)", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1664); _tr_str_release(_at_t1664); });
                    }
                } else if (((((_tr_str_eqv((gn), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((gn), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((gn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((gn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((gn), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6602 "src/sema.tr"
                    __auto_type _t1665 = (*idx);
                    if (_t1665.tag == Expr_EIdent) {
                        __auto_type tn = _t1665.data.EIdent.name;
#line 6604 "src/sema.tr"
                        ret_ty = AstType_init(gn);
#line 6605 "src/sema.tr"
                        ({ __auto_type _ord_t1666 = (box_asttype(AstType_init(tn))); List_ptr_append(ret_ty->args, _ord_t1666); });
                    } else if (1) {
                        __auto_type _ = _t1665;
#line 6607 "src/sema.tr"
                        ret_ty = AstType_init(gn);
                    }
                } else if (_tr_dict_contains(self->classes, _tr_strz(gn))) {
#line 6609 "src/sema.tr"
                    ret_ty = AstType_init(gn);
#line 6610 "src/sema.tr"
                    __auto_type _t1667 = (*idx);
                    if (_t1667.tag == Expr_EIdent) {
                        __auto_type targ_ca = _t1667.data.EIdent.name;
                        ({ __auto_type _ord_t1668 = (box_asttype(AstType_init(targ_ca))); List_ptr_append(ret_ty->args, _ord_t1668); });
                    } else if (1) {
                        __auto_type _ = _t1667;
                        /* pass */
                    }
                } else if (_tr_dict_contains(self->enums, _tr_strz(gn))) {
#line 6614 "src/sema.tr"
                    ret_ty = AstType_init(gn);
                } else {
#line 6621 "src/sema.tr"
                    __auto_type _t1669 = (*idx);
                    if (_t1669.tag == Expr_EIdent) {
                        __auto_type farg_c = _t1669.data.EIdent.name;
#line 6641 "src/sema.tr"
                        AstType* fret = AstType_init(farg_c);
#line 6642 "src/sema.tr"
                        if (_tr_dict_contains(self->fn_defs, _tr_strz(gn))) {
#line 6643 "src/sema.tr"
                            FunctionDef* _sf_fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(gn)));
#line 6644 "src/sema.tr"
                            if ((((unsigned long long)(_sf_fd->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 6644 "src/sema.tr"
                                fret = (*_sf_fd->ret_ty);
                            }
#line 6645 "src/sema.tr"
                            if (((_sf_fd->generics->len > 0LL) && ({ __auto_type _ord_t1670 = (List_TrStr_get(_sf_fd->generics, 0LL)); __auto_type _wr = (({ TrStr _cmpa_t1671 = (_ord_t1670); TrStr _cmpb_t1672 = (fret->name); _Bool _cmpr_t1673 = _tr_str_eqv(_cmpa_t1671, _cmpb_t1672); _tr_str_release(_cmpa_t1671); _cmpr_t1673; })); _wr; }))) {
#line 6646 "src/sema.tr"
                                fret = AstType_init(farg_c);
                            }
#line 6660 "src/sema.tr"
                            if ((((unsigned long long)(_sf_fd->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 6661 "src/sema.tr"
                                if ((!_tr_str_eqv(((*_sf_fd->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 6662 "src/sema.tr"
                                    AstType* _sf_result = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 6663 "src/sema.tr"
                                    ({ __auto_type _ord_t1674 = (box_asttype(fret)); List_ptr_append(_sf_result->args, _ord_t1674); });
#line 6664 "src/sema.tr"
                                    List_ptr_append(_sf_result->args, _sf_fd->throws_ty);
#line 6665 "src/sema.tr"
                                    fret = _sf_result;
                                }
                            }
                        }
#line 6666 "src/sema.tr"
                        ret_ty = fret;
#line 6667 "src/sema.tr"
                        hcallee = ({ TrStr _at_t1675 = (({ TrStr _tr_cl = (_tr_strx_concatv((gn), (_tr_str_lit_len("__MONO_", 7LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (farg_c)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t1675, fret, false))); _tr_str_release(_at_t1675); _wr; });
                    } else if (_t1669.tag == Expr_ETuple) {
                        __auto_type ftargs = _t1669.data.ETuple.items;
#line 6674 "src/sema.tr"
                        List_TrStr* _mf_names = (void*)List_TrStr_new();
#line 6675 "src/sema.tr"
                        long long _mfi = 0LL;
#line 6676 "src/sema.tr"
                        while ((_mfi < ftargs->len)) {
#line 6677 "src/sema.tr"
                            TrStr _mf_nm = _tr_str_lit_len("", 0LL);
#line 6678 "src/sema.tr"
                            __auto_type _t1676 = (*((Expr*)List_ptr_get(ftargs, _mfi)));
                            if (_t1676.tag == Expr_EIdent) {
                                __auto_type _mfn = _t1676.data.EIdent.name;
                                TrStr _strtmp_t1677 = _tr_str_retain(_mfn);
                                _tr_str_release(_mf_nm);
                                _mf_nm = _strtmp_t1677;
                            } else if (1) {
                                __auto_type _ = _t1676;
                                /* pass */
                            }
#line 6681 "src/sema.tr"
                            List_TrStr_append(_mf_names, _mf_nm);
#line 6682 "src/sema.tr"
                            _mfi = (_mfi + 1LL);
                            _tr_str_release(_mf_nm);
                        }
#line 6683 "src/sema.tr"
                        TrStr _mf_mangled = _tr_str_retain(gn);
#line 6684 "src/sema.tr"
                        long long _mf_ji = 0LL;
#line 6685 "src/sema.tr"
                        while ((_mf_ji < _mf_names->len)) {
#line 6686 "src/sema.tr"
                            if ((_mf_ji == 0LL)) {
#line 6686 "src/sema.tr"
                                ({ TrStr _ap_t1678 = (_tr_str_lit_len("__MONO_", 7LL)); TrStr _ap_t1679 = (List_TrStr_get(_mf_names, _mf_ji)); _tr_str_append(&_mf_mangled, _ap_t1678); _tr_str_append(&_mf_mangled, _ap_t1679); _tr_str_release(_ap_t1679); });
                            } else {
#line 6687 "src/sema.tr"
                                ({ TrStr _ap_t1680 = (_tr_str_lit_len("_", 1LL)); TrStr _ap_t1681 = (List_TrStr_get(_mf_names, _mf_ji)); _tr_str_append(&_mf_mangled, _ap_t1680); _tr_str_append(&_mf_mangled, _ap_t1681); _tr_str_release(_ap_t1681); });
                            }
#line 6688 "src/sema.tr"
                            _mf_ji = (_mf_ji + 1LL);
                        }
#line 6689 "src/sema.tr"
                        AstType* fret2 = AstType_init(_tr_str_lit_len("void", 4LL));
#line 6690 "src/sema.tr"
                        if (_tr_dict_contains(self->fn_defs, _tr_strz(gn))) {
#line 6691 "src/sema.tr"
                            FunctionDef* _mf_fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(gn)));
#line 6692 "src/sema.tr"
                            if ((((unsigned long long)(_mf_fd->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 6692 "src/sema.tr"
                                fret2 = (*_mf_fd->ret_ty);
                            }
#line 6693 "src/sema.tr"
                            long long _mf_gi = 0LL;
#line 6694 "src/sema.tr"
                            while ((_mf_gi < _mf_fd->generics->len)) {
#line 6695 "src/sema.tr"
                                if ((({ __auto_type _ord_t1682 = (List_TrStr_get(_mf_fd->generics, _mf_gi)); __auto_type _wr = (({ TrStr _cmpa_t1683 = (_ord_t1682); TrStr _cmpb_t1684 = (fret2->name); _Bool _cmpr_t1685 = _tr_str_eqv(_cmpa_t1683, _cmpb_t1684); _tr_str_release(_cmpa_t1683); _cmpr_t1685; })); _wr; }) && (_mf_gi < _mf_names->len))) {
#line 6696 "src/sema.tr"
                                    fret2 = ({ TrStr _at_t1686 = (List_TrStr_get(_mf_names, _mf_gi)); __auto_type _wr = (AstType_init(_at_t1686)); _tr_str_release(_at_t1686); _wr; });
                                }
#line 6697 "src/sema.tr"
                                _mf_gi = (_mf_gi + 1LL);
                            }
#line 6701 "src/sema.tr"
                            if ((((unsigned long long)(_mf_fd->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 6702 "src/sema.tr"
                                if ((!_tr_str_eqv(((*_mf_fd->throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 6703 "src/sema.tr"
                                    AstType* _mf_result = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 6704 "src/sema.tr"
                                    ({ __auto_type _ord_t1687 = (box_asttype(fret2)); List_ptr_append(_mf_result->args, _ord_t1687); });
#line 6705 "src/sema.tr"
                                    List_ptr_append(_mf_result->args, _mf_fd->throws_ty);
#line 6706 "src/sema.tr"
                                    fret2 = _mf_result;
                                }
                            }
                        }
#line 6707 "src/sema.tr"
                        ret_ty = fret2;
#line 6708 "src/sema.tr"
                        hcallee = box_hirexpr(HirExpr_ctor_EIdent(_mf_mangled, fret2, false));
                        List_TrStr_free(_mf_names);
                        _tr_str_release(_mf_mangled);
                    } else if (1) {
                        __auto_type _ = _t1669;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1661;
                /* pass */
            }
        } else if (_t1653.tag == Expr_EPropAccess) {
            __auto_type obj = _t1653.data.EPropAccess.obj;
__auto_type variant = _t1653.data.EPropAccess.prop;
#line 6712 "src/sema.tr"
            __auto_type _t1688 = (*obj);
            if (_t1688.tag == Expr_EIdent) {
                __auto_type type_name = _t1688.data.EIdent.name;
#line 6714 "src/sema.tr"
                if (({ __auto_type _ord_t1689 = (Sema_resolve(self, type_name)->kind); __auto_type _wr = ((_ord_t1689.tag != SymbolKind_make_SVariable().tag)); _wr; })) {
#line 6714 "src/sema.tr"
                    Sema_check_static_call(self, type_name, variant, hl);
                }
#line 6715 "src/sema.tr"
                if (_tr_dict_contains(self->enums, _tr_strz(type_name))) {
#line 6716 "src/sema.tr"
                    ret_ty = AstType_init(type_name);
                } else if (((((_tr_str_eqv((type_name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((type_name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("ThreadLocal", 11LL)))) && ((_tr_str_eqv((variant), (_tr_str_lit_len("spawn", 5LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL)))))) {
#line 6719 "src/sema.tr"
                    ret_ty = AstType_init(type_name);
                } else if ((_tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL))))) {
#line 6721 "src/sema.tr"
                    if (((((_tr_str_eqv((type_name), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((type_name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((type_name), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6722 "src/sema.tr"
                        ret_ty = AstType_init(type_name);
                    } else if (_tr_dict_contains(self->classes, _tr_strz(type_name))) {
#line 6724 "src/sema.tr"
                        ret_ty = AstType_init(type_name);
                    }
                } else if (_tr_dict_contains(self->classes, _tr_strz(type_name))) {
#line 6729 "src/sema.tr"
                    ClassDef* _scdef = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(type_name)));
#line 6730 "src/sema.tr"
                    long long _smi = 0LL;
#line 6731 "src/sema.tr"
                    bool _sfound = false;
#line 6732 "src/sema.tr"
                    while (((_smi < _scdef->methods->len) && (!_sfound))) {
#line 6733 "src/sema.tr"
                        FunctionDef* _smdef = ((FunctionDef*)List_ptr_get(_scdef->methods, _smi));
#line 6734 "src/sema.tr"
                        if (_tr_str_eqv((_smdef->name), (variant))) {
#line 6735 "src/sema.tr"
                            long long _nself = _smdef->params->len;
#line 6736 "src/sema.tr"
                            if (((_smdef->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_smdef->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 6737 "src/sema.tr"
                                _nself = (_smdef->params->len - 1LL);
                            }
#line 6738 "src/sema.tr"
                            if ((_nself == hl->len)) {
#line 6739 "src/sema.tr"
                                if ((((unsigned long long)(_smdef->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 6740 "src/sema.tr"
                                    ret_ty = (*_smdef->ret_ty);
                                }
#line 6741 "src/sema.tr"
                                _sfound = true;
                            }
                        }
#line 6742 "src/sema.tr"
                        _smi = (_smi + 1LL);
                    }
#line 6743 "src/sema.tr"
                    if ((!_sfound)) {
#line 6744 "src/sema.tr"
                        _smi = 0LL;
#line 6745 "src/sema.tr"
                        while (((_smi < _scdef->methods->len) && (!_sfound))) {
#line 6746 "src/sema.tr"
                            FunctionDef* _smdef2 = ((FunctionDef*)List_ptr_get(_scdef->methods, _smi));
#line 6747 "src/sema.tr"
                            if (_tr_str_eqv((_smdef2->name), (variant))) {
#line 6748 "src/sema.tr"
                                if ((((unsigned long long)(_smdef2->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 6749 "src/sema.tr"
                                    ret_ty = (*_smdef2->ret_ty);
                                }
#line 6750 "src/sema.tr"
                                _sfound = true;
                            }
#line 6751 "src/sema.tr"
                            _smi = (_smi + 1LL);
                        }
                    }
                }
            } else if (_t1688.tag == Expr_EIndex) {
                __auto_type base2 = _t1688.data.EIndex.obj;
__auto_type idx2 = _t1688.data.EIndex._tr_v_index;
#line 6753 "src/sema.tr"
                if ((_tr_str_eqv((variant), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((variant), (_tr_str_lit_len("new", 3LL))))) {
#line 6754 "src/sema.tr"
                    __auto_type _t1690 = (*base2);
                    if (_t1690.tag == Expr_EIdent) {
                        __auto_type gn2 = _t1690.data.EIdent.name;
#line 6756 "src/sema.tr"
                        if (((((_tr_str_eqv((gn2), (_tr_str_lit_len("Pointer", 7LL))) || _tr_str_eqv((gn2), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((gn2), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((gn2), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((gn2), (_tr_str_lit_len("Dict", 4LL))))) {
#line 6757 "src/sema.tr"
                            __auto_type _t1691 = (*idx2);
                            if (_t1691.tag == Expr_EIdent) {
                                __auto_type tn2 = _t1691.data.EIdent.name;
#line 6759 "src/sema.tr"
                                ret_ty = AstType_init(gn2);
#line 6760 "src/sema.tr"
                                ({ __auto_type _ord_t1692 = (box_asttype(AstType_init(tn2))); List_ptr_append(ret_ty->args, _ord_t1692); });
                            } else if (1) {
                                __auto_type _ = _t1691;
#line 6762 "src/sema.tr"
                                ret_ty = AstType_init(gn2);
                            }
                        } else if (_tr_dict_contains(self->classes, _tr_strz(gn2))) {
#line 6764 "src/sema.tr"
                            ret_ty = AstType_init(gn2);
#line 6765 "src/sema.tr"
                            __auto_type _t1693 = (*idx2);
                            if (_t1693.tag == Expr_EIdent) {
                                __auto_type targ_cb = _t1693.data.EIdent.name;
                                ({ __auto_type _ord_t1694 = (box_asttype(AstType_init(targ_cb))); List_ptr_append(ret_ty->args, _ord_t1694); });
                            } else if (1) {
                                __auto_type _ = _t1693;
                                /* pass */
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t1690;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1688;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t1653;
            /* pass */
        }
#line 6772 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6773 "src/sema.tr"
            __auto_type _t1695 = (*callee);
            if (_t1695.tag == Expr_EIdent) {
                __auto_type aa_nm = _t1695.data.EIdent.name;
#line 6775 "src/sema.tr"
                if (_tr_str_eqv((aa_nm), (_tr_str_lit_len("await_all", 9LL)))) {
#line 6776 "src/sema.tr"
                    long long aa_i = 0LL;
#line 6777 "src/sema.tr"
                    while ((aa_i < hl->len)) {
#line 6778 "src/sema.tr"
                        Sema_check_spawn_sendable(self, ((HirExpr*)List_ptr_get(hl, aa_i)));
#line 6779 "src/sema.tr"
                        aa_i = (aa_i + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t1695;
                /* pass */
            }
        }
#line 6782 "src/sema.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 6783 "src/sema.tr"
            __auto_type _t1696 = (*callee);
            if (_t1696.tag == Expr_EIdent) {
                __auto_type df_nm = _t1696.data.EIdent.name;
#line 6785 "src/sema.tr"
                if ((_tr_str_eqv((df_nm), (_tr_str_lit_len("dealloc", 7LL))) && (args->len > 0LL))) {
#line 6786 "src/sema.tr"
                    Expr* df_arg0 = ((Expr*)List_ptr_get(args, 0LL));
#line 6787 "src/sema.tr"
                    if ((((unsigned long long)(df_arg0)) != ((unsigned long long)(0LL)))) {
#line 6788 "src/sema.tr"
                        __auto_type _t1697 = (*df_arg0);
                        if (_t1697.tag == Expr_EIdent) {
                            __auto_type df_ptr = _t1697.data.EIdent.name;
#line 6790 "src/sema.tr"
                            Symbol* df_sym = Sema_resolve(self, df_ptr);
#line 6791 "src/sema.tr"
                            if ((!_tr_str_eqv((df_sym->name), (_tr_str_lit_len("", 0LL))))) {
#line 6792 "src/sema.tr"
                                Sema_mark_freed(self, df_ptr);
                            }
                        } else if (1) {
                            __auto_type _ = _t1697;
                            /* pass */
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t1696;
                /* pass */
            }
        }
#line 6795 "src/sema.tr"
        HirExpr* _trret265 = box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
        List_TrStr_free(p23_borrow_names);
        Dict_free(p23_seen);
        _tr_str_release(_ch_fname);
        return _trret265;
    } else if (_t1600.tag == Expr_EMethodCall) {
        __auto_type obj = _t1600.data.EMethodCall.obj;
__auto_type method = _t1600.data.EMethodCall.method;
__auto_type args = _t1600.data.EMethodCall.args;
#line 6797 "src/sema.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("__index__", 9LL))) && (args->len > 0LL))) {
#line 6798 "src/sema.tr"
            HirExpr* _trret266 = Sema_lower_expr(self, box_expr(Expr_ctor_EIndex(obj, ((Expr*)List_ptr_get(args, 0LL)))));
            return _trret266;
        }
#line 6801 "src/sema.tr"
        if (((self->no_heap && ((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("with_capacity", 13LL))))) && (((unsigned long long)(obj)) != ((unsigned long long)(0LL))))) {
#line 6802 "src/sema.tr"
            TrStr _nh_rn = _tr_str_lit_len("", 0LL);
#line 6803 "src/sema.tr"
            __auto_type _t1698 = (*obj);
            if (_t1698.tag == Expr_EIdent) {
                __auto_type _nhn = _t1698.data.EIdent.name;
                TrStr _strtmp_t1699 = _tr_str_retain(_nhn);
                _tr_str_release(_nh_rn);
                _nh_rn = _strtmp_t1699;
            } else if (_t1698.tag == Expr_EIndex) {
                __auto_type _nhbase = _t1698.data.EIndex.obj;
#line 6806 "src/sema.tr"
                if ((((unsigned long long)(_nhbase)) != ((unsigned long long)(0LL)))) {
#line 6807 "src/sema.tr"
                    __auto_type _t1700 = (*_nhbase);
                    if (_t1700.tag == Expr_EIdent) {
                        __auto_type _nhn2 = _t1700.data.EIdent.name;
                        TrStr _strtmp_t1701 = _tr_str_retain(_nhn2);
                        _tr_str_release(_nh_rn);
                        _nh_rn = _strtmp_t1701;
                    } else if (1) {
                        __auto_type _ = _t1700;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t1698;
                /* pass */
            }
#line 6811 "src/sema.tr"
            if (((((_tr_str_eqv((_nh_rn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((_nh_rn), (_tr_str_lit_len("Set", 3LL))))) {
#line 6812 "src/sema.tr"
                ({ TrStr _at_t1702 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_nh_rn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() (a growable heap collection)", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_no_heap(self, _at_t1702, _tr_str_lit_len("use a fixed-size array [T; N] (stack), or parallel fixed arrays for a map", 73LL)); _tr_str_release(_at_t1702); });
            }
        }
#line 6813 "src/sema.tr"
        List_ptr* hl = (void*)List_ptr_new();
#line 6814 "src/sema.tr"
        long long k = 0LL;
#line 6815 "src/sema.tr"
        while ((k < args->len)) {
#line 6816 "src/sema.tr"
            self->closure_hint = (AstType**)(0LL);
#line 6817 "src/sema.tr"
            __auto_type _t1703 = (*((Expr*)List_ptr_get(args, k)));
            if (_t1703.tag == Expr_EClosure) {
                self->closure_hint = Sema_closure_hint_for_method(self, method, k);
            } else if (_t1703.tag == Expr_EList) {
                __auto_type _lhm_items = _t1703.data.EList.items;
#line 6820 "src/sema.tr"
                if ((_lhm_items->len == 0LL)) {
#line 6820 "src/sema.tr"
                    self->list_hint = Sema_list_hint_for_method(self, method, k);
                }
            } else if (1) {
                __auto_type _ = _t1703;
                /* pass */
            }
#line 6822 "src/sema.tr"
            List_ptr_append(hl, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k))));
#line 6823 "src/sema.tr"
            self->closure_hint = (AstType**)(0LL);
#line 6824 "src/sema.tr"
            self->list_hint = (AstType**)(0LL);
#line 6825 "src/sema.tr"
            k = (k + 1LL);
        }
#line 6826 "src/sema.tr"
        bool _saved_recv_mc = self->in_recv_pos;
#line 6827 "src/sema.tr"
        self->in_recv_pos = true;
#line 6828 "src/sema.tr"
        HirExpr* hobj = Sema_lower_expr(self, obj);
#line 6829 "src/sema.tr"
        self->in_recv_pos = _saved_recv_mc;
#line 6834 "src/sema.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
#line 6835 "src/sema.tr"
            __auto_type _t1704 = (*obj);
            if (_t1704.tag == Expr_EIdent) {
                __auto_type free_nm = _t1704.data.EIdent.name;
#line 6837 "src/sema.tr"
                Symbol* free_sym = Sema_resolve(self, free_nm);
#line 6838 "src/sema.tr"
                if ((!_tr_str_eqv((free_sym->name), (_tr_str_lit_len("", 0LL))))) {
#line 6838 "src/sema.tr"
                    Sema_mark_freed(self, free_nm);
                }
            } else if (1) {
                __auto_type _ = _t1704;
                /* pass */
            }
        }
#line 6840 "src/sema.tr"
        AstType* hobj_ty = hir_expr_type(hobj);
#line 6841 "src/sema.tr"
        if (true) {
#line 6842 "src/sema.tr"
            ({ TrStr _ord_t1705 = (Sema_firm_family(self, hobj)); __auto_type _ord_t1706 = (hl->len); Sema_check_builtin_method_arity(self, _ord_t1705, method, _ord_t1706); _tr_str_release(_ord_t1705); });
#line 6843 "src/sema.tr"
            ({ TrStr _ord_t1707 = (Sema_firm_family(self, hobj)); __auto_type _ord_t1708 = (hir_expr_type(hobj)->name); Sema_check_scalar_method(self, _ord_t1707, method, _ord_t1708); _tr_str_release(_ord_t1707); });
#line 6844 "src/sema.tr"
            Sema_check_builtin_class_method(self, hir_expr_type(hobj)->name, method, hl);
#line 6845 "src/sema.tr"
            Sema_check_collection_arg_types(self, hir_expr_type(hobj), method, hl);
#line 6846 "src/sema.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL))))) {
#line 6846 "src/sema.tr"
                Sema_check_user_method_call(self, hir_expr_type(hobj)->name, obj, method, hl);
            }
        }
#line 6848 "src/sema.tr"
        if (_tr_dict_contains(self->type_aliases, _tr_strz(hobj_ty->name))) {
#line 6849 "src/sema.tr"
            TrStr _alias_base = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->type_aliases, _tr_strz(hobj_ty->name))));
#line 6850 "src/sema.tr"
            AstType* _alias_ty = AstType_init(_alias_base);
#line 6851 "src/sema.tr"
            if (_tr_dict_contains(self->type_alias_elem, _tr_strz(hobj_ty->name))) {
#line 6852 "src/sema.tr"
                TrStr _elem_name = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->type_alias_elem, _tr_strz(hobj_ty->name))));
#line 6853 "src/sema.tr"
                _alias_ty = AstType_init_generic(_alias_base, box_asttype(AstType_init(_elem_name)));
            }
#line 6854 "src/sema.tr"
            hobj_ty = _alias_ty;
        }
#line 6860 "src/sema.tr"
        TrStr _recv_name = _tr_str_lit_len("", 0LL);
#line 6861 "src/sema.tr"
        bool _recv_is_shared = false;
#line 6862 "src/sema.tr"
        __auto_type _t1709 = (*obj);
        if (_t1709.tag == Expr_EIdent) {
            __auto_type _rn = _t1709.data.EIdent.name;
#line 6864 "src/sema.tr"
            TrStr _strtmp_t1710 = _tr_str_retain(_rn);
            _tr_str_release(_recv_name);
            _recv_name = _strtmp_t1710;
#line 6865 "src/sema.tr"
            Symbol* _rsym = Sema_resolve(self, _rn);
#line 6866 "src/sema.tr"
            if (((!_tr_str_eqv((_rsym->name), (_tr_str_lit_len("", 0LL)))) && _rsym->is_shared)) {
#line 6866 "src/sema.tr"
                _recv_is_shared = true;
            }
        } else if (1) {
            __auto_type _ = _t1709;
            /* pass */
        }
#line 6878 "src/sema.tr"
        if ((((!_tr_str_eqv((_recv_name), (_tr_str_lit_len("", 0LL)))) && (!Sema_is_local_var(self, _recv_name))) && (!Sema__is_known_type_name(self, _recv_name)))) {
#line 6879 "src/sema.tr"
            if (_tr_str_eqv((Sema_resolve(self, _recv_name)->name), (_tr_str_lit_len("", 0LL)))) {
#line 6880 "src/sema.tr"
                ({ TrStr _at_t1711 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] '", 7LL)), (_recv_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not a defined type or value, so '", 38LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_recv_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' cannot be resolved.\n      FIX: check the spelling, import the type, or define it — a call on an unknown name would otherwise emit invalid C.", 149LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1711); _tr_str_release(_at_t1711); });
            }
        }
#line 6883 "src/sema.tr"
        if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
#line 6884 "src/sema.tr"
            ClassDef* _cf_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
#line 6885 "src/sema.tr"
            long long _cf_i = 0LL;
#line 6886 "src/sema.tr"
            AstType* _cf_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 6887 "src/sema.tr"
            bool _cf_found = false;
#line 6888 "src/sema.tr"
            while ((_cf_i < _cf_cls->fields->len)) {
#line 6889 "src/sema.tr"
                FieldDef* _cf_fld = ((FieldDef*)List_ptr_get(_cf_cls->fields, _cf_i));
#line 6890 "src/sema.tr"
                if ((_tr_str_eqv((_cf_fld->name), (method)) && (((unsigned long long)(_cf_fld->ty)) != ((unsigned long long)(0LL))))) {
#line 6891 "src/sema.tr"
                    _cf_ty = (*_cf_fld->ty);
#line 6892 "src/sema.tr"
                    _cf_found = true;
                }
#line 6893 "src/sema.tr"
                _cf_i = (_cf_i + 1LL);
            }
#line 6894 "src/sema.tr"
            if ((_cf_found && _tr_str_eqv((_cf_ty->name), (_tr_str_lit_len("def", 3LL))))) {
#line 6895 "src/sema.tr"
                AstType* _cf_ret = AstType_init(_tr_str_lit_len("void", 4LL));
#line 6896 "src/sema.tr"
                if ((_cf_ty->args->len > 0LL)) {
#line 6896 "src/sema.tr"
                    _cf_ret = (*((AstType**)List_ptr_get(_cf_ty->args, (_cf_ty->args->len - 1LL))));
                }
#line 6907 "src/sema.tr"
                if (((_cf_cls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
#line 6908 "src/sema.tr"
                    _cf_ret = Sema__subst_ret_generics(self, _cf_ret, _cf_cls->generics, hobj_ty->args);
                }
#line 6909 "src/sema.tr"
                HirExpr* _cf_prop = box_hirexpr(HirExpr_ctor_EPropAccess(hobj, method, _cf_ty));
#line 6910 "src/sema.tr"
                HirExpr* _trret267 = box_hirexpr(HirExpr_ctor_ECall(_cf_prop, hl, _cf_ret));
                _tr_str_release(_recv_name);
                return _trret267;
            }
        }
#line 6912 "src/sema.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL))))) {
#line 6913 "src/sema.tr"
            TrStr pc_obj_nm = _tr_str_lit_len("", 0LL);
#line 6914 "src/sema.tr"
            __auto_type _t1712 = (*obj);
            if (_t1712.tag == Expr_EIdent) {
                __auto_type pc_src = _t1712.data.EIdent.name;
                TrStr _strtmp_t1713 = _tr_str_retain(pc_src);
                _tr_str_release(pc_obj_nm);
                pc_obj_nm = _strtmp_t1713;
            } else if (1) {
                __auto_type _ = _t1712;
                /* pass */
            }
#line 6917 "src/sema.tr"
            if (((!_tr_str_eqv((pc_obj_nm), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->container_borrows, _tr_strz(pc_obj_nm)))) {
#line 6918 "src/sema.tr"
                TrStr pc_borrow_var = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->container_borrows, _tr_strz(pc_obj_nm))));
#line 6919 "src/sema.tr"
                ({ TrStr _at_t1714 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-4] Cannot mutate '", 21LL)), (pc_obj_nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while '", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pc_borrow_var)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' holds a reference into it.\n      FIX: Finish using '", 54LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pc_borrow_var)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' before modifying '", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pc_obj_nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', or copy it first: 'mut copy = ", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pc_borrow_var)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1714); _tr_str_release(_at_t1714); });
            }
        }
#line 6921 "src/sema.tr"
        TrStr _bm_obj_nm = _tr_str_lit_len("", 0LL);
#line 6922 "src/sema.tr"
        __auto_type _t1715 = (*obj);
        if (_t1715.tag == Expr_EIdent) {
            __auto_type _bm_n = _t1715.data.EIdent.name;
            TrStr _strtmp_t1716 = _tr_str_retain(_bm_n);
            _tr_str_release(_bm_obj_nm);
            _bm_obj_nm = _strtmp_t1716;
        } else if (1) {
            __auto_type _ = _t1715;
            /* pass */
        }
#line 6925 "src/sema.tr"
        AstType* ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 6929 "src/sema.tr"
        AstType** _gpb_ret = ((AstType**)(0LL));
#line 6930 "src/sema.tr"
        if (((self->current_func_constraints->len > 0LL) && Sema__is_type_param_in_scope(self, hobj_ty->name))) {
#line 6931 "src/sema.tr"
            _gpb_ret = Sema__resolve_generic_bound_method_ret(self, hobj_ty->name, method);
        }
#line 6933 "src/sema.tr"
        if ((((unsigned long long)(_gpb_ret)) != ((unsigned long long)(0LL)))) {
#line 6934 "src/sema.tr"
            ret_ty = (*_gpb_ret);
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("OS", 2LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("OS", 2LL))))) {
#line 6936 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("cwd", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("platform", 8LL))))) {
#line 6936 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            } else if ((((_tr_str_eqv((method), (_tr_str_lit_len("is_windows", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_linux", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_darwin", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_macos", 8LL))))) {
#line 6937 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            }
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("Process", 7LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Process", 7LL))))) {
#line 6939 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("system", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("exit", 4LL))))) {
#line 6939 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("shell_output", 12LL)))) {
#line 6940 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("Env", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Env", 3LL))))) {
#line 6942 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("get_var", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("cwd", 3LL))))) {
#line 6942 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        } else if ((_tr_str_eqv((_bm_obj_nm), (_tr_str_lit_len("Hash", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Hash", 4LL))))) {
#line 6944 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("sha256", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("md5", 3LL))))) {
#line 6944 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        }
#line 6950 "src/sema.tr"
        if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && _tr_str_eqv((method), (_tr_str_lit_len("write", 5LL)))) && (!self->in_unsafe))) {
#line 6951 "src/sema.tr"
            Sema_error(self, _tr_str_lit_len("[P-1] '.write()' on a Pointer mutates raw memory and must be inside an 'unsafe:' block.\n      FIX: Wrap this call in 'unsafe:', e.g.\n          unsafe:\n              ... .write(...) ...", 184LL));
        }
#line 6964 "src/sema.tr"
        bool _auto_is_self_factory = true;
#line 6965 "src/sema.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("auto", 4LL))) && _tr_dict_contains(self->classes, _tr_strz(hobj_ty->name)))) {
#line 6966 "src/sema.tr"
            ClassDef* _af_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
#line 6967 "src/sema.tr"
            long long _af_i = 0LL;
#line 6968 "src/sema.tr"
            while ((_af_i < _af_cls->methods->len)) {
#line 6969 "src/sema.tr"
                FunctionDef* _af_m = ((FunctionDef*)List_ptr_get(_af_cls->methods, _af_i));
#line 6970 "src/sema.tr"
                if ((_tr_str_eqv((_af_m->name), (_tr_str_lit_len("auto", 4LL))) && (((unsigned long long)(_af_m->ret_ty)) != ((unsigned long long)(0LL))))) {
#line 6971 "src/sema.tr"
                    if (({ __auto_type _ord_t1717 = ((*_af_m->ret_ty)->name); __auto_type _wr = ((!_tr_str_eqv((_ord_t1717), (hobj_ty->name)))); _wr; })) {
#line 6971 "src/sema.tr"
                        _auto_is_self_factory = false;
                    }
                }
#line 6972 "src/sema.tr"
                _af_i = (_af_i + 1LL);
            }
        }
#line 6973 "src/sema.tr"
        if (((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))) || (_tr_str_eqv((method), (_tr_str_lit_len("auto", 4LL))) && _auto_is_self_factory))) {
#line 6974 "src/sema.tr"
            __auto_type _t1718 = (*obj);
            if (_t1718.tag == Expr_EIdent) {
                __auto_type _ic_n = _t1718.data.EIdent.name;
#line 6976 "src/sema.tr"
                if ((_tr_dict_contains(self->classes, _tr_strz(_ic_n)) && ({ __auto_type _ord_t1719 = (Sema_resolve(self, _ic_n)->kind); __auto_type _wr = ((_ord_t1719.tag != SymbolKind_make_SVariable().tag)); _wr; }))) {
#line 6976 "src/sema.tr"
                    Sema_check_static_call(self, _ic_n, method, hl);
                }
            } else if (1) {
                __auto_type _ = _t1718;
                /* pass */
            }
#line 6979 "src/sema.tr"
            if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("", 0LL))))) {
#line 6982 "src/sema.tr"
                __auto_type _t1720 = (*hobj);
                if (_t1720.tag == HirExpr_EIdent) {
                    __auto_type recv_nm = _t1720.data.EIdent.name;
                    ret_ty = AstType_init(recv_nm);
                } else if (_t1720.tag == HirExpr_EIndex) {
                    __auto_type idx_base = _t1720.data.EIndex.obj;
__auto_type idx_arg = _t1720.data.EIndex._tr_v_index;
#line 6986 "src/sema.tr"
                    __auto_type _t1721 = (*idx_base);
                    if (_t1721.tag == HirExpr_EIdent) {
                        __auto_type gn = _t1721.data.EIdent.name;
#line 6988 "src/sema.tr"
                        ret_ty = AstType_init(gn);
#line 6989 "src/sema.tr"
                        __auto_type _t1722 = (*idx_arg);
                        if (_t1722.tag == HirExpr_EIdent) {
                            __auto_type tn = _t1722.data.EIdent.name;
                            ({ __auto_type _ord_t1723 = (box_asttype(AstType_init(tn))); List_ptr_append(ret_ty->args, _ord_t1723); });
                        } else if (1) {
                            __auto_type _ = _t1722;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t1721;
                        ret_ty = hobj_ty;
                    }
                } else if (1) {
                    __auto_type _ = _t1720;
                    ret_ty = hobj_ty;
                }
            } else {
#line 6995 "src/sema.tr"
                ret_ty = hobj_ty;
            }
        } else if (_tr_str_eqv((method), (_tr_str_lit_len("offset", 6LL)))) {
#line 7000 "src/sema.tr"
            if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (!self->cur_fn_is_lib)) && (!self->in_unsafe))) {
#line 7001 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[P-2] '.offset()' does raw pointer arithmetic (can create an out-of-bounds pointer) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or use a safe slice / Vec[T] instead of pointer math.", 209LL));
            }
#line 7002 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((((_tr_str_eqv((method), (_tr_str_lit_len("sum", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("min_val", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max_val", 7LL)))))) {
#line 7005 "src/sema.tr"
            if ((hobj_ty->args->len > 0LL)) {
#line 7005 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
#line 7006 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((_tr_str_eqv((method), (_tr_str_lit_len("any", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("all", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))))) {
#line 7007 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL)))))) {
#line 7008 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("first", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("last", 4LL)))))) {
#line 7010 "src/sema.tr"
            if ((hobj_ty->args->len > 0LL)) {
#line 7010 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
#line 7011 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("reversed", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("reversed_copy", 13LL)))))) {
#line 7012 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL)))))) {
#line 7013 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && _tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL))))) {
#line 7014 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && ((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("last_index_of", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("count", 5LL)))))) {
#line 7015 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && _tr_str_eqv((method), (_tr_str_lit_len("join", 4LL))))) {
#line 7016 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) && _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 7018 "src/sema.tr"
            if ((hobj_ty->args->len > 0LL)) {
#line 7022 "src/sema.tr"
                if (((!self->cur_fn_is_lib) && (!self->in_unsafe))) {
#line 7023 "src/sema.tr"
                    Sema_error(self, _tr_str_lit_len("[P-2] '.read()' dereferences a raw pointer (may read freed or invalid memory) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or hold the value in a safe Box[T] / Vec[T] / class reference instead of a raw pointer.", 237LL));
                }
#line 7024 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
#line 7025 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("as_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))))) {
#line 7038 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("__len__", 7LL))))) {
#line 7039 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((method), (_tr_str_lit_len("checked_add", 11LL))) || _tr_str_eqv((method), (_tr_str_lit_len("checked_sub", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("checked_mul", 11LL))))) {
#line 7042 "src/sema.tr"
            ret_ty = AstType_init_generic(_tr_str_lit_len("Option", 6LL), box_asttype(AstType_init(_tr_str_lit_len("int", 3LL))));
        } else if (((((((((_tr_str_eqv((method), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sign", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clamp", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("gcd", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("lcm", 3LL)))) && ((((((((((((((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("isize", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("char", 4LL)))))) {
#line 7047 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("f32", 3LL))))) {
#line 7050 "src/sema.tr"
            if (((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("floor", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("ceil", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("round", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sqrt", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("fabs", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("log", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("log2", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("log10", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("exp", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sin", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cos", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("tan", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("asin", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("acos", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("atan", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("atan2", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL))))) {
#line 7051 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("float", 5LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_nan", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_inf", 6LL))))) {
#line 7052 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("str", 3LL)))) {
#line 7055 "src/sema.tr"
            ret_ty = Sema_str_method_ret_ty(self, method);
#line 7056 "src/sema.tr"
            if ((((!_str_method_known(method)) && (!Sema_class_method_exists(self, _tr_str_lit_len("str", 3LL), method))) && (!Sema_is_universal_method(self, method)))) {
#line 7059 "src/sema.tr"
                ({ TrStr _at_t1724 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] No method '", 17LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' found on type 'str'.\n      FIX: see docs/std/string.md for the str methods, or add one with 'extend str:'.", 108LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1724); _tr_str_release(_at_t1724); });
            }
        } else if (((((((((((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("char", 4LL))))) {
#line 7062 "src/sema.tr"
            if (((((((_tr_str_eqv((method), (_tr_str_lit_len("to_hex", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_HEX", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_hex_upper", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_octal", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_oct", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_binary", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("to_bin", 6LL))))) {
#line 7063 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_f64", 6LL))))) {
#line 7064 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("float", 5LL));
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("to_int", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_i64", 6LL))))) {
#line 7065 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        } else if (((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL)))) && (!_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("StringBuilder", 13LL)))))) {
#line 7069 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if (((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL))))) {
#line 7071 "src/sema.tr"
            if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (hobj_ty->args->len > 0LL))) {
#line 7072 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (hobj_ty->args->len > 1LL))) {
#line 7074 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (hobj_ty->args->len > 0LL))) {
#line 7090 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
#line 7102 "src/sema.tr"
                ClassDef* _gcls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
#line 7103 "src/sema.tr"
                long long _gmi = 0LL;
#line 7104 "src/sema.tr"
                bool _gfound = false;
#line 7105 "src/sema.tr"
                while ((_gmi < _gcls->methods->len)) {
#line 7106 "src/sema.tr"
                    FunctionDef* _gmdef = ((FunctionDef*)List_ptr_get(_gcls->methods, _gmi));
#line 7107 "src/sema.tr"
                    if (_tr_str_eqv((_gmdef->name), (method))) {
#line 7108 "src/sema.tr"
                        if ((((unsigned long long)(_gmdef->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 7109 "src/sema.tr"
                            ret_ty = (*_gmdef->ret_ty);
#line 7110 "src/sema.tr"
                            if (((_gcls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
#line 7111 "src/sema.tr"
                                ret_ty = Sema__subst_ret_generics(self, ret_ty, _gcls->generics, hobj_ty->args);
                            }
#line 7112 "src/sema.tr"
                            _gfound = true;
                        }
                    }
#line 7113 "src/sema.tr"
                    _gmi = (_gmi + 1LL);
                }
#line 7114 "src/sema.tr"
                if (((!_gfound) && (hobj_ty->args->len > 0LL))) {
#line 7115 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
            } else if ((hobj_ty->args->len > 0LL)) {
#line 7117 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            }
        } else if (_tr_str_eqv((method), (_tr_str_lit_len("alloc", 5LL)))) {
#line 7118 "src/sema.tr"
            ret_ty = hobj_ty;
        } else if (_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL)))) {
#line 7119 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Array", 5LL)))) {
#line 7122 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("as_ptr", 6LL)))) {
#line 7124 "src/sema.tr"
                AstType* _arr_elem = AstType_init(_tr_str_lit_len("u8", 2LL));
#line 7125 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7125 "src/sema.tr"
                    _arr_elem = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
#line 7126 "src/sema.tr"
                ret_ty = AstType_init_generic(_tr_str_lit_len("Pointer", 7LL), box_asttype(_arr_elem));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
#line 7127 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Set", 3LL)))) {
#line 7130 "src/sema.tr"
            if ((((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_subset", 9LL))))) {
#line 7130 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
#line 7131 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if (((_tr_str_eqv((method), (_tr_str_lit_len("add", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL))))) {
#line 7132 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("to_list", 7LL)))) {
#line 7135 "src/sema.tr"
                AstType* _set_elem_ty = AstType_init(_tr_str_lit_len("str", 3LL));
#line 7136 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7137 "src/sema.tr"
                    _set_elem_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
#line 7138 "src/sema.tr"
                ret_ty = AstType_init_generic(_tr_str_lit_len("List", 4LL), box_asttype(_set_elem_ty));
            } else if (((_tr_str_eqv((method), (_tr_str_lit_len("union", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("intersection", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("difference", 10LL))))) {
#line 7139 "src/sema.tr"
                ret_ty = hobj_ty;
            }
        } else if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Dict", 4LL))))) {
#line 7142 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("keys", 4LL)))) {
#line 7144 "src/sema.tr"
                AstType* _dict_key_ty = AstType_init(_tr_str_lit_len("str", 3LL));
#line 7145 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7146 "src/sema.tr"
                    _dict_key_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
#line 7147 "src/sema.tr"
                ret_ty = AstType_init_generic(_tr_str_lit_len("List", 4LL), box_asttype(_dict_key_ty));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("values", 6LL)))) {
#line 7150 "src/sema.tr"
                AstType* _dict_val_ty = AstType_init(_tr_str_lit_len("ptr", 3LL));
#line 7151 "src/sema.tr"
                if ((hobj_ty->args->len > 1LL)) {
#line 7152 "src/sema.tr"
                    _dict_val_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
                }
#line 7153 "src/sema.tr"
                ret_ty = AstType_init_generic(_tr_str_lit_len("List", 4LL), box_asttype(_dict_val_ty));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Option", 6LL)))) {
#line 7161 "src/sema.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("Some", 4LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("some", 4LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Option", 6LL))))) && (hl->len == 1LL))) {
#line 7162 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Option", 6LL));
#line 7163 "src/sema.tr"
                ({ __auto_type _ord_t1725 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1725); });
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("None", 4LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("none", 4LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Option", 6LL)))))) {
#line 7164 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Option", 6LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_some", 7LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_none", 7LL))))) {
#line 7165 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("unwrap", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("expect", 6LL))))) {
#line 7167 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7167 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7168 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("unwrap_or", 9LL)))) {
#line 7170 "src/sema.tr"
                if ((hl->len > 0LL)) {
#line 7170 "src/sema.tr"
                    ret_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)));
                } else if ((hobj_ty->args->len > 0LL)) {
#line 7171 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7172 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Result", 6LL)))) {
#line 7180 "src/sema.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("Ok", 2LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("ok", 2LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Result", 6LL))))) && (hl->len == 1LL))) {
#line 7181 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 7182 "src/sema.tr"
                ({ __auto_type _ord_t1726 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1726); });
#line 7183 "src/sema.tr"
                ({ __auto_type _ord_t1727 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1727); });
            } else if (((_tr_str_eqv((method), (_tr_str_lit_len("Err", 3LL))) || (_tr_str_eqv((method), (_tr_str_lit_len("err", 3LL))) && _tr_str_eqv((_recv_name), (_tr_str_lit_len("Result", 6LL))))) && (hl->len == 1LL))) {
#line 7185 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 7186 "src/sema.tr"
                ({ __auto_type _ord_t1728 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1728); });
#line 7187 "src/sema.tr"
                ({ __auto_type _ord_t1729 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1729); });
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_ok", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_err", 6LL))))) {
#line 7188 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("unwrap", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("ok", 2LL))))) {
#line 7190 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7190 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7191 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("unwrap_err", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("err", 3LL))))) {
#line 7193 "src/sema.tr"
                if ((hobj_ty->args->len > 1LL)) {
#line 7193 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
                } else {
#line 7194 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
                }
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && (_tr_str_eqv((_recv_name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Thread", 6LL)))))) {
#line 7200 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("Thread", 6LL));
#line 7202 "src/sema.tr"
            long long _tsi = 1LL;
#line 7203 "src/sema.tr"
            while ((_tsi < hl->len)) {
#line 7204 "src/sema.tr"
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _tsi)))) {
#line 7205 "src/sema.tr"
                    Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to Thread.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 254LL));
                }
#line 7206 "src/sema.tr"
                AstType* _tsa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _tsi)));
#line 7207 "src/sema.tr"
                if ((!Sema_is_sendable_ty(self, _tsa_ty))) {
#line 7208 "src/sema.tr"
                    ({ TrStr _at_t1730 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (_tsa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be passed to Thread.spawn.\n      FIX: Wrap in Mutex[", 81LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tsa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] for exclusive access, or add 'implements Sendable' to '", 57LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tsa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1730); _tr_str_release(_at_t1730); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _tsa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _tsi))))) && (!Sema__is_unsafe_sendable(self, _tsa_ty->name)))) {
#line 7210 "src/sema.tr"
                    ({ TrStr _at_t1731 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (_tsa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot be passed to Thread.spawn (exactly why Rust's 'Rc' is '!Send'): retaining/releasing it from another thread races the count.\n      FIX: share it as 'Shared[", 270LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tsa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc'). A 'Sendable' class must itself hold only Sendable + atomically-shared state.", 117LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1731); _tr_str_release(_at_t1731); });
                }
#line 7211 "src/sema.tr"
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _tsi)));
#line 7212 "src/sema.tr"
                _tsi = (_tsi + 1LL);
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("ThreadPool", 10LL))))) {
#line 7215 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7217 "src/sema.tr"
            long long _psi = 1LL;
#line 7218 "src/sema.tr"
            while ((_psi < hl->len)) {
#line 7219 "src/sema.tr"
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _psi)))) {
#line 7220 "src/sema.tr"
                    Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to ThreadPool.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 258LL));
                }
#line 7221 "src/sema.tr"
                AstType* _psa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _psi)));
#line 7222 "src/sema.tr"
                if ((!Sema_is_sendable_ty(self, _psa_ty))) {
#line 7223 "src/sema.tr"
                    ({ TrStr _at_t1732 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (_psa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be passed to ThreadPool.spawn.\n      FIX: Wrap in Mutex[", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_psa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] for exclusive access, or add 'implements Sendable' to '", 57LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_psa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1732); _tr_str_release(_at_t1732); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _psa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _psi))))) && (!Sema__is_unsafe_sendable(self, _psa_ty->name)))) {
#line 7225 "src/sema.tr"
                    ({ TrStr _at_t1733 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (_psa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it cannot be passed to ThreadPool.spawn (like Rust's 'Rc' being '!Send').\n      FIX: share it as 'Shared[", 192LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_psa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc').", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1733); _tr_str_release(_at_t1733); });
                }
#line 7226 "src/sema.tr"
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _psi)));
#line 7227 "src/sema.tr"
                _psi = (_psi + 1LL);
            }
        } else if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && _tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("AsyncPool", 9LL))))) {
#line 7237 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("AsyncTask", 9LL));
#line 7238 "src/sema.tr"
            long long _asi = 1LL;
#line 7239 "src/sema.tr"
            while ((_asi < hl->len)) {
#line 7240 "src/sema.tr"
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _asi)))) {
#line 7241 "src/sema.tr"
                    Sema_error(self, _tr_str_lit_len("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to AsyncPool.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow.", 257LL));
                }
#line 7242 "src/sema.tr"
                AstType* _asa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _asi)));
#line 7243 "src/sema.tr"
                if ((!Sema_is_sendable_ty(self, _asa_ty))) {
#line 7244 "src/sema.tr"
                    ({ TrStr _at_t1734 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-1] Type '", 12LL)), (_asa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is not Sendable and cannot be passed to AsyncPool.spawn.\n      FIX: Wrap in Mutex[", 84LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_asa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] for exclusive access, or add 'implements Sendable' to '", 57LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_asa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to confirm it is thread-safe.", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1734); _tr_str_release(_at_t1734); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _asa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _asi))))) && (!Sema__is_unsafe_sendable(self, _asa_ty->name)))) {
#line 7246 "src/sema.tr"
                    ({ TrStr _at_t1735 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[T-7] '", 7LL)), (_asa_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it cannot be passed to AsyncPool.spawn (like Rust's 'Rc' being '!Send'): the task may run on a different worker than the one that submitted it.\n      FIX: share it as 'Shared[", 262LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_asa_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]' (atomic refcount, like Rust's 'Arc').", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1735); _tr_str_release(_at_t1735); });
                }
#line 7247 "src/sema.tr"
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _asi)));
#line 7248 "src/sema.tr"
                _asi = (_asi + 1LL);
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("AsyncTask", 9LL)))) {
#line 7251 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) {
#line 7251 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("done", 4LL)))) {
#line 7252 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
#line 7253 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Atomic", 6LL)))) {
#line 7256 "src/sema.tr"
            if ((((((((((((((((_tr_str_eqv((method), (_tr_str_lit_len("load", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("swap", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("exchange", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_add", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_sub", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("load_relaxed", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("load_acquire", 12LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("load_seqcst", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add_relaxed", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add_release", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add_acqrel", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub_relaxed", 11LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sub_release", 11LL))))) {
#line 7257 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if ((((_tr_str_eqv((method), (_tr_str_lit_len("cas", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("compare_exchange", 16LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cas_weak", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("cas_acqrel", 10LL))))) {
#line 7259 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (((((_tr_str_eqv((method), (_tr_str_lit_len("store", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("store_relaxed", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("store_release", 13LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("store_seqcst", 12LL))))) {
#line 7261 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Chan", 4LL)))) {
#line 7264 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("recv", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("try_recv", 8LL))))) {
#line 7265 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7265 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7266 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("cap", 3LL))))) {
#line 7267 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if ((_tr_str_eqv((method), (_tr_str_lit_len("is_closed", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("try_send", 8LL))))) {
#line 7268 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("send_timeout", 12LL)))) {
#line 7269 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("recv_timeout", 12LL)))) {
#line 7271 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7271 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7272 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 7275 "src/sema.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("lock", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))))) {
#line 7276 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7276 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7277 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 7280 "src/sema.tr"
            if ((((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("read_lock", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("write_lock", 10LL))))) {
#line 7281 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7281 "src/sema.tr"
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
#line 7282 "src/sema.tr"
                    ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
                }
            }
        } else if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("StringBuilder", 13LL))) && _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))))) {
#line 7283 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("StringObj", 9LL));
        } else if ((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("StringBuilder", 13LL))) && _tr_str_eqv((method), (_tr_str_lit_len("to_owned", 8LL))))) {
#line 7284 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("str", 3LL));
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Shared", 6LL)))) {
#line 7289 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL)))) {
#line 7289 "src/sema.tr"
                ret_ty = hobj_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))) {
#line 7290 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("downgrade", 9LL)))) {
#line 7292 "src/sema.tr"
                AstType* weak_ty = AstType_init(_tr_str_lit_len("Weak", 4LL));
#line 7293 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7294 "src/sema.tr"
                    weak_ty->args = (void*)List_ptr_new();
#line 7295 "src/sema.tr"
                    ({ __auto_type _ord_t1736 = (box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL))))); List_ptr_append(weak_ty->args, _ord_t1736); });
                }
#line 7296 "src/sema.tr"
                ret_ty = weak_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("drop", 4LL)))) {
#line 7297 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            }
        } else if (_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Weak", 4LL)))) {
#line 7300 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("upgrade", 7LL)))) {
#line 7301 "src/sema.tr"
                AstType* opt_ty = AstType_init(_tr_str_lit_len("Option", 6LL));
#line 7302 "src/sema.tr"
                if ((hobj_ty->args->len > 0LL)) {
#line 7303 "src/sema.tr"
                    opt_ty->args = (void*)List_ptr_new();
#line 7304 "src/sema.tr"
                    ({ __auto_type _ord_t1737 = (box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL))))); List_ptr_append(opt_ty->args, _ord_t1737); });
                }
#line 7305 "src/sema.tr"
                ret_ty = opt_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("is_alive", 8LL)))) {
#line 7306 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            }
        } else if ((_recv_is_shared && (((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("downgrade", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("drop", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))))) {
#line 7311 "src/sema.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL)))) {
#line 7311 "src/sema.tr"
                ret_ty = hobj_ty;
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("is_null", 7LL)))) {
#line 7312 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("drop", 4LL)))) {
#line 7313 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
            } else if (_tr_str_eqv((method), (_tr_str_lit_len("downgrade", 9LL)))) {
#line 7315 "src/sema.tr"
                AstType* _wk_ty = AstType_init(_tr_str_lit_len("Weak", 4LL));
#line 7316 "src/sema.tr"
                _wk_ty->args = (void*)List_ptr_new();
#line 7317 "src/sema.tr"
                ({ __auto_type _ord_t1738 = (box_asttype(hobj_ty)); List_ptr_append(_wk_ty->args, _ord_t1738); });
#line 7318 "src/sema.tr"
                ret_ty = _wk_ty;
            }
        } else if (((((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))) && _tr_dict_contains(self->interfaces, _tr_strz(hobj_ty->name)))) {
#line 7320 "src/sema.tr"
            InterfaceDef* _iface_def = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(hobj_ty->name)));
#line 7321 "src/sema.tr"
            long long _imi = 0LL;
#line 7322 "src/sema.tr"
            while ((_imi < _iface_def->methods->len)) {
#line 7323 "src/sema.tr"
                FunctionDef* _imdef = ((FunctionDef*)List_ptr_get(_iface_def->methods, _imi));
#line 7324 "src/sema.tr"
                if (_tr_str_eqv((_imdef->name), (method))) {
#line 7325 "src/sema.tr"
                    if ((((unsigned long long)(_imdef->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 7326 "src/sema.tr"
                        ret_ty = (*_imdef->ret_ty);
                    }
                }
#line 7327 "src/sema.tr"
                _imi = (_imi + 1LL);
            }
#line 7329 "src/sema.tr"
            if (((_iface_def->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
#line 7330 "src/sema.tr"
                long long _gi5 = 0LL;
#line 7331 "src/sema.tr"
                while ((_gi5 < _iface_def->generics->len)) {
#line 7332 "src/sema.tr"
                    if (({ __auto_type _ord_t1739 = (List_TrStr_get(_iface_def->generics, _gi5)); __auto_type _wr = (({ TrStr _cmpa_t1740 = (_ord_t1739); TrStr _cmpb_t1741 = (ret_ty->name); _Bool _cmpr_t1742 = _tr_str_eqv(_cmpa_t1740, _cmpb_t1741); _tr_str_release(_cmpa_t1740); _cmpr_t1742; })); _wr; })) {
#line 7333 "src/sema.tr"
                        if ((_gi5 < hobj_ty->args->len)) {
#line 7334 "src/sema.tr"
                            ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, _gi5)));
                        }
                    }
#line 7335 "src/sema.tr"
                    _gi5 = (_gi5 + 1LL);
                }
            }
        } else if (_tr_dict_contains(self->enums, _tr_strz(hobj_ty->name))) {
#line 7337 "src/sema.tr"
            ret_ty = AstType_init(hobj_ty->name);
#line 7341 "src/sema.tr"
            if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Option", 6LL))) && _tr_str_eqv((method), (_tr_str_lit_len("Some", 4LL)))) && (hl->len == 1LL))) {
#line 7342 "src/sema.tr"
                ({ __auto_type _ord_t1743 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1743); });
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((method), (_tr_str_lit_len("Ok", 2LL)))) && (hl->len == 1LL))) {
#line 7344 "src/sema.tr"
                ({ __auto_type _ord_t1744 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1744); });
#line 7345 "src/sema.tr"
                ({ __auto_type _ord_t1745 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1745); });
            } else if (((_tr_str_eqv((hobj_ty->name), (_tr_str_lit_len("Result", 6LL))) && _tr_str_eqv((method), (_tr_str_lit_len("Err", 3LL)))) && (hl->len == 1LL))) {
#line 7347 "src/sema.tr"
                ({ __auto_type _ord_t1746 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(ret_ty->args, _ord_t1746); });
#line 7348 "src/sema.tr"
                ({ __auto_type _ord_t1747 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL))))); List_ptr_append(ret_ty->args, _ord_t1747); });
            }
        } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
#line 7350 "src/sema.tr"
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
#line 7351 "src/sema.tr"
            TrStr _hty_n = _tr_str_retain(hobj_ty->name);
#line 7352 "src/sema.tr"
            bool _is_builtin_dispatch = (((((((((((((((((_tr_str_eqv((_hty_n), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Channel", 7LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("OS", 2LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Process", 7LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Env", 3LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("Hash", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("File", 4LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((_hty_n), (_tr_str_lit_len("AsyncTask", 9LL))));
#line 7353 "src/sema.tr"
            if ((((!_is_builtin_dispatch) && (!Sema_class_method_exists(self, hobj_ty->name, method))) && (!Sema_is_universal_method(self, method)))) {
#line 7354 "src/sema.tr"
                ({ TrStr _at_t1748 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-1] No method '", 17LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' found on type '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: Define 'pub def ", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(self, ...)' in '", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' or its base class via 'extend ", 32LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":'.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1748); _tr_str_release(_at_t1748); });
            }
#line 7355 "src/sema.tr"
            long long _mi = 0LL;
#line 7356 "src/sema.tr"
            AstType** _mdef_throws_ty = ((AstType**)(0LL));
#line 7357 "src/sema.tr"
            long long _m_named = 0LL;
#line 7358 "src/sema.tr"
            bool _m_recv_is_type = false;
#line 7359 "src/sema.tr"
            __auto_type _t1749 = (*obj);
            if (_t1749.tag == Expr_EIdent) {
                __auto_type _m_rn = _t1749.data.EIdent.name;
#line 7361 "src/sema.tr"
                _m_recv_is_type = (({ __auto_type _ord_t1750 = (Sema_resolve(self, _m_rn)->kind); __auto_type _wr = ((_ord_t1750.tag == SymbolKind_make_SClass().tag)); _wr; }) || _tr_dict_contains(self->classes, _tr_strz(_m_rn)));
            } else if (1) {
                __auto_type _ = _t1749;
                /* pass */
            }
#line 7363 "src/sema.tr"
            bool _m_arity_ok = false;
#line 7364 "src/sema.tr"
            FunctionDef* _m_one = FunctionDef_init(_tr_str_lit_len("", 0LL));
#line 7365 "src/sema.tr"
            long long _m_first_want = 0LL;
#line 7366 "src/sema.tr"
            bool _m_variadic = false;
#line 7367 "src/sema.tr"
            while ((_mi < _cls->methods->len)) {
#line 7368 "src/sema.tr"
                FunctionDef* _mdef = ((FunctionDef*)List_ptr_get(_cls->methods, _mi));
#line 7369 "src/sema.tr"
                if (_tr_str_eqv((_mdef->name), (method))) {
#line 7370 "src/sema.tr"
                    long long _m_cnt = 0LL;
#line 7371 "src/sema.tr"
                    long long _m_ci = 0LL;
#line 7372 "src/sema.tr"
                    while ((_m_ci < _mdef->params->len)) {
#line 7373 "src/sema.tr"
                        if ((!_tr_str_eqv((((Param*)List_ptr_get(_mdef->params, _m_ci))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7373 "src/sema.tr"
                            _m_cnt = (_m_cnt + 1LL);
                        }
#line 7374 "src/sema.tr"
                        if (((Param*)List_ptr_get(_mdef->params, _m_ci))->is_variadic) {
#line 7374 "src/sema.tr"
                            _m_variadic = true;
                        }
#line 7375 "src/sema.tr"
                        _m_ci = (_m_ci + 1LL);
                    }
#line 7376 "src/sema.tr"
                    if ((_m_named == 0LL)) {
#line 7376 "src/sema.tr"
                        _m_first_want = _m_cnt;
                    }
#line 7377 "src/sema.tr"
                    _m_named = (_m_named + 1LL);
#line 7378 "src/sema.tr"
                    if ((_m_cnt == hl->len)) {
#line 7378 "src/sema.tr"
                        _m_arity_ok = true;
                    }
#line 7379 "src/sema.tr"
                    FunctionDef* _cltmp_t1751 = _tr_obj_retain(_mdef);
                    _tr_obj_release(_m_one, _trdrop_FunctionDef);
                    _m_one = _cltmp_t1751;
#line 7380 "src/sema.tr"
                    if ((((((_m_cnt + 1LL) == hl->len) && (_mdef->params->len > 0LL)) && _tr_str_eqv((((Param*)List_ptr_get(_mdef->params, 0LL))->name), (_tr_str_lit_len("self", 4LL)))) && _m_recv_is_type)) {
#line 7380 "src/sema.tr"
                        _m_arity_ok = true;
                    }
#line 7381 "src/sema.tr"
                    if ((((unsigned long long)(_mdef->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 7384 "src/sema.tr"
                        long long _mpc = 0LL;
#line 7385 "src/sema.tr"
                        long long _mpi = 0LL;
#line 7386 "src/sema.tr"
                        while ((_mpi < _mdef->params->len)) {
#line 7387 "src/sema.tr"
                            if ((!_tr_str_eqv((((Param*)List_ptr_get(_mdef->params, _mpi))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7387 "src/sema.tr"
                                _mpc = (_mpc + 1LL);
                            }
#line 7388 "src/sema.tr"
                            _mpi = (_mpi + 1LL);
                        }
#line 7389 "src/sema.tr"
                        if ((_mpc == hl->len)) {
#line 7390 "src/sema.tr"
                            ret_ty = (*_mdef->ret_ty);
#line 7391 "src/sema.tr"
                            _mdef_throws_ty = _mdef->throws_ty;
#line 7392 "src/sema.tr"
                            if (((_cls->generics->len == 0LL) && (_mdef->generics->len == 0LL))) {
#line 7393 "src/sema.tr"
                                long long _fa_sk = 0LL;
#line 7394 "src/sema.tr"
                                if (((_mdef->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_mdef->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7394 "src/sema.tr"
                                    _fa_sk = 1LL;
                                }
#line 7395 "src/sema.tr"
                                long long _fa_i = _fa_sk;
#line 7396 "src/sema.tr"
                                while ((_fa_i < _mdef->params->len)) {
#line 7397 "src/sema.tr"
                                    if (((_fa_i - _fa_sk) < hl->len)) {
#line 7398 "src/sema.tr"
                                        ({ __auto_type _ord_t1752 = (((Param*)List_ptr_get(_mdef->params, _fa_i))->ty); __auto_type _ord_t1753 = (hir_expr_type(((HirExpr*)List_ptr_get(hl, (_fa_i - _fa_sk))))); __auto_type _ord_t1754 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(((_fa_i - _fa_sk) + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1755 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1754); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_fn_value_throws(self, _ord_t1752, _ord_t1753, _ord_t1755); _tr_str_release(_ord_t1755); });
                                    }
#line 7399 "src/sema.tr"
                                    _fa_i = (_fa_i + 1LL);
                                }
                            }
                        } else if (_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 7401 "src/sema.tr"
                            ret_ty = (*_mdef->ret_ty);
#line 7402 "src/sema.tr"
                            _mdef_throws_ty = _mdef->throws_ty;
                        }
                    }
                }
#line 7403 "src/sema.tr"
                _mi = (_mi + 1LL);
            }
#line 7404 "src/sema.tr"
            if (((((((_m_named == 1LL) && _m_arity_ok) && (!_m_variadic)) && (!Sema_is_rt_builtin_class(self, hobj_ty->name))) && (_cls->generics->len == 0LL)) && (_m_one->generics->len == 0LL))) {
#line 7405 "src/sema.tr"
                long long _m1_sk = 0LL;
#line 7406 "src/sema.tr"
                if (((_m_one->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_m_one->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7406 "src/sema.tr"
                    _m1_sk = 1LL;
                }
#line 7407 "src/sema.tr"
                if (((_m_one->params->len - _m1_sk) == hl->len)) {
#line 7408 "src/sema.tr"
                    long long _m1_i = 0LL;
#line 7409 "src/sema.tr"
                    while ((_m1_i < hl->len)) {
#line 7410 "src/sema.tr"
                        AstType** _m1_pt = ((Param*)List_ptr_get(_m_one->params, (_m1_sk + _m1_i)))->ty;
#line 7411 "src/sema.tr"
                        if ((((unsigned long long)(_m1_pt)) != ((unsigned long long)(0LL)))) {
#line 7412 "src/sema.tr"
                            ({ __auto_type _ord_t1756 = ((*_m1_pt)); __auto_type _ord_t1757 = (((HirExpr*)List_ptr_get(hl, _m1_i))); __auto_type _ord_t1758 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((_m1_i + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t1759 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1758); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hobj_ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()'", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_check_expr_assignable(self, _ord_t1756, _ord_t1757, _ord_t1759); _tr_str_release(_ord_t1759); });
                        }
#line 7413 "src/sema.tr"
                        _m1_i = (_m1_i + 1LL);
                    }
                }
            }
#line 7414 "src/sema.tr"
            if (((((_m_named > 0LL) && (!_m_arity_ok)) && (!_m_variadic)) && (!Sema_is_rt_builtin_class(self, hobj_ty->name)))) {
#line 7415 "src/sema.tr"
                TrStr _m_pl = _tr_str_lit_len("s", 1LL);
#line 7416 "src/sema.tr"
                if ((_m_first_want == 1LL)) {
#line 7416 "src/sema.tr"
                    TrStr _strtmp_t1760 = _tr_str_lit_len("", 0LL);
                    _tr_str_release(_m_pl);
                    _m_pl = _strtmp_t1760;
                }
#line 7417 "src/sema.tr"
                TrStr _m_vb = _tr_str_lit_len("were", 4LL);
#line 7418 "src/sema.tr"
                if ((hl->len == 1LL)) {
#line 7418 "src/sema.tr"
                    TrStr _strtmp_t1761 = _tr_str_lit_len("was", 3LL);
                    _tr_str_release(_m_vb);
                    _m_vb = _strtmp_t1761;
                }
#line 7419 "src/sema.tr"
                ({ __auto_type _ord_t1762 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-4] '", 7LL)), (hobj_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' takes ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1763 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1762); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_m_first_want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_m_pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" but ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t1764 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1763); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(hl->len)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_m_vb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" given.\n      FIX: call it with exactly ", 40LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1765 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t1764); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_m_first_want)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" argument", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_m_pl)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (see its definition).", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1765); _tr_str_release(_at_t1765); });
            }
#line 7431 "src/sema.tr"
            if ((((unsigned long long)(_mdef_throws_ty)) != ((unsigned long long)(0LL)))) {
#line 7432 "src/sema.tr"
                if ((!_tr_str_eqv(((*_mdef_throws_ty)->name), (_tr_str_lit_len("", 0LL))))) {
#line 7433 "src/sema.tr"
                    AstType* _mres_ty = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 7434 "src/sema.tr"
                    ({ __auto_type _ord_t1766 = (box_asttype(ret_ty)); List_ptr_append(_mres_ty->args, _ord_t1766); });
#line 7435 "src/sema.tr"
                    List_ptr_append(_mres_ty->args, _mdef_throws_ty);
#line 7436 "src/sema.tr"
                    ret_ty = _mres_ty;
                }
            }
#line 7439 "src/sema.tr"
            if (((_cls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
#line 7440 "src/sema.tr"
                ret_ty = Sema__subst_ret_generics(self, ret_ty, _cls->generics, hobj_ty->args);
            }
            _tr_obj_release(_m_one, _trdrop_FunctionDef);
        }
#line 7441 "src/sema.tr"
        HirExpr* _trret268 = box_hirexpr(HirExpr_ctor_EMethodCall(hobj, method, hl, ret_ty));
        _tr_str_release(_recv_name);
        _tr_str_release(_bm_obj_nm);
        return _trret268;
    } else if (_t1600.tag == Expr_EPropAccess) {
        __auto_type obj = _t1600.data.EPropAccess.obj;
__auto_type prop = _t1600.data.EPropAccess.prop;
#line 7458 "src/sema.tr"
        __auto_type _t1767 = (*obj);
        if (_t1767.tag == Expr_EIdent) {
            __auto_type _pmv_cn = _t1767.data.EIdent.name;
#line 7460 "src/sema.tr"
            Symbol* _pmv_sym = Sema_resolve(self, _pmv_cn);
#line 7468 "src/sema.tr"
            if ((((_tr_str_eqv((_pmv_sym->name), (_tr_str_lit_len("", 0LL))) || (_pmv_sym->kind.tag == SymbolKind_make_SClass().tag)) && _tr_dict_contains(self->classes, _tr_strz(_pmv_cn))) && Sema_class_method_exists(self, _pmv_cn, prop))) {
#line 7469 "src/sema.tr"
                ClassDef* _pmv_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_pmv_cn)));
#line 7470 "src/sema.tr"
                FunctionDef* _pmv_m = ((FunctionDef*)List_ptr_get(_pmv_cls->methods, 0LL));
#line 7471 "src/sema.tr"
                bool _pmv_found = false;
#line 7472 "src/sema.tr"
                long long _pmv_mi = 0LL;
#line 7473 "src/sema.tr"
                while ((_pmv_mi < _pmv_cls->methods->len)) {
#line 7474 "src/sema.tr"
                    FunctionDef* _pmv_mm = ((FunctionDef*)List_ptr_get(_pmv_cls->methods, _pmv_mi));
#line 7475 "src/sema.tr"
                    if (_tr_str_eqv((_pmv_mm->name), (prop))) {
#line 7476 "src/sema.tr"
                        bool _pmv_is_static = true;
#line 7477 "src/sema.tr"
                        if (((_pmv_mm->params->len > 0LL) && _tr_str_eqv((((Param*)List_ptr_get(_pmv_mm->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 7478 "src/sema.tr"
                            _pmv_is_static = false;
                        }
#line 7479 "src/sema.tr"
                        if (_pmv_is_static) {
#line 7480 "src/sema.tr"
                            _pmv_m = _tr_obj_retain(_pmv_mm);
#line 7481 "src/sema.tr"
                            _pmv_found = true;
                        }
                    }
#line 7482 "src/sema.tr"
                    _pmv_mi = (_pmv_mi + 1LL);
                }
#line 7483 "src/sema.tr"
                if (_pmv_found) {
#line 7484 "src/sema.tr"
                    AstType* _pmv_fnty = AstType_init(_tr_str_lit_len("def", 3LL));
#line 7485 "src/sema.tr"
                    long long _pmv_pi = 0LL;
#line 7486 "src/sema.tr"
                    while ((_pmv_pi < _pmv_m->params->len)) {
#line 7487 "src/sema.tr"
                        ({ __auto_type _ord_t1768 = (((Param*)List_ptr_get(_pmv_m->params, _pmv_pi))->ty); List_ptr_append(_pmv_fnty->args, _ord_t1768); });
#line 7488 "src/sema.tr"
                        _pmv_pi = (_pmv_pi + 1LL);
                    }
#line 7489 "src/sema.tr"
                    if ((((unsigned long long)(_pmv_m->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 7490 "src/sema.tr"
                        if (((((unsigned long long)(_pmv_m->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*_pmv_m->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 7491 "src/sema.tr"
                            ({ __auto_type _ord_t1769 = (box_asttype(throws_result_type((*_pmv_m->ret_ty), _pmv_m->throws_ty))); List_ptr_append(_pmv_fnty->args, _ord_t1769); });
                        } else {
#line 7493 "src/sema.tr"
                            List_ptr_append(_pmv_fnty->args, _pmv_m->ret_ty);
                        }
                    } else {
#line 7495 "src/sema.tr"
                        ({ __auto_type _ord_t1770 = (box_asttype(AstType_init(_tr_str_lit_len("void", 4LL)))); List_ptr_append(_pmv_fnty->args, _ord_t1770); });
                    }
#line 7496 "src/sema.tr"
                    HirExpr* _trret269 = ({ TrStr _at_t1771 = (({ TrStr _tr_cl = (_tr_strx_concatv((_pmv_cn), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t1771, _pmv_fnty, false))); _tr_str_release(_at_t1771); _wr; });
                    return _trret269;
                }
            }
        } else if (1) {
            __auto_type _ = _t1767;
            /* pass */
        }
#line 7498 "src/sema.tr"
        bool _saved_recv_pa = self->in_recv_pos;
#line 7499 "src/sema.tr"
        self->in_recv_pos = true;
#line 7500 "src/sema.tr"
        HirExpr* hobj = Sema_lower_expr(self, obj);
#line 7501 "src/sema.tr"
        self->in_recv_pos = _saved_recv_pa;
#line 7502 "src/sema.tr"
        TrStr hobj_ty_n = _tr_str_retain(hir_expr_type(hobj)->name);
#line 7503 "src/sema.tr"
        AstType* hobj_ty_full = hir_expr_type(hobj);
#line 7504 "src/sema.tr"
        AstType* ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7505 "src/sema.tr"
        if (true) {
#line 7506 "src/sema.tr"
            TrStr _pa_ff = Sema_firm_family(self, hobj);
#line 7507 "src/sema.tr"
            if (((_tr_str_eqv((_pa_ff), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_pa_ff), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((_pa_ff), (_tr_str_lit_len("bool", 4LL))))) {
#line 7508 "src/sema.tr"
                ({ TrStr _at_t1772 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-6] Cannot read '.", 20LL)), (prop))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Sema_family_word(self, _pa_ff)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": a ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_pa_ff)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" has no fields.\n      FIX: check that the receiver is the object you meant; numbers and booleans only have methods such as abs() or to_str().", 141LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1772); _tr_str_release(_at_t1772); });
            }
        }
#line 7509 "src/sema.tr"
        if (((((_tr_dict_contains(self->classes, _tr_strz(hobj_ty_n)) && (!_tr_dict_contains(self->ambiguous_classes, _tr_strz(hobj_ty_n)))) && (!_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Result", 6LL))))) && (!_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Option", 6LL))))) && (!Sema_is_builtin_dispatch_class(self, hobj_ty_n)))) {
#line 7510 "src/sema.tr"
            if ((((!Sema_class_has_field(self, hobj_ty_n, prop)) && (!Sema_class_method_exists(self, hobj_ty_n, prop))) && (!Sema_is_universal_method(self, prop)))) {
#line 7511 "src/sema.tr"
                ({ TrStr _at_t1773 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-7] Type '", 12LL)), (hobj_ty_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has no field '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: check the spelling, or declare 'pub ", 50LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": <type>' in the class.", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1773); _tr_str_release(_at_t1773); });
            }
        }
#line 7512 "src/sema.tr"
        if ((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Result", 6LL))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Option", 6LL))))) {
#line 7513 "src/sema.tr"
            if ((!((((((((((((_tr_str_eqv((prop), (_tr_str_lit_len("err", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("ok", 2LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_err", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_ok", 5LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_some", 7LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_none", 7LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("val", 3LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("value", 5LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("some", 4LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("none", 4LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("tag", 3LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL)))) || (((_tr_str_lenv((prop)) > 0LL) && (strcmp(_tr_strz(_tr_str_slicev((prop), 0LL, 1LL)), _tr_strz(_tr_str_lit_len("A", 1LL))) >= 0)) && (strcmp(_tr_strz(_tr_str_slicev((prop), 0LL, 1LL)), _tr_strz(_tr_str_lit_len("Z", 1LL))) <= 0))))) {
#line 7514 "src/sema.tr"
                ({ TrStr _at_t1774 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[E-7] '", 7LL)), (hobj_ty_n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has no field '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.\n      FIX: use .is_ok / .is_err / .ok / .err (Result) or .is_some / .is_none / .value (Option), or the unwrap() family of methods.", 133LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1774); _tr_str_release(_at_t1774); });
            }
        }
#line 7515 "src/sema.tr"
        if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Result", 6LL)))) {
#line 7516 "src/sema.tr"
            if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_err", 6LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_ok", 5LL))))) {
#line 7516 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if ((_tr_str_eqv((prop), (_tr_str_lit_len("ok", 2LL))) && (hobj_ty_full->args->len > 0LL))) {
#line 7517 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            } else if ((_tr_str_eqv((prop), (_tr_str_lit_len("err", 3LL))) && (hobj_ty_full->args->len > 1LL))) {
#line 7518 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 1LL)));
            }
        } else if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Option", 6LL)))) {
#line 7520 "src/sema.tr"
            if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_some", 7LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_none", 7LL))))) {
#line 7520 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("bool", 4LL));
            } else if (((_tr_str_eqv((prop), (_tr_str_lit_len("value", 5LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("val", 3LL)))) && (hobj_ty_full->args->len > 0LL))) {
#line 7521 "src/sema.tr"
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            }
        } else if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("StringObj", 9LL)))) {
#line 7523 "src/sema.tr"
            if (_tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL)))) {
#line 7523 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
            } else if ((_tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL))))) {
#line 7524 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("StringBuilder", 13LL)))) {
#line 7526 "src/sema.tr"
            if (_tr_str_eqv((prop), (_tr_str_lit_len("buf", 3LL)))) {
#line 7526 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("StringObj", 9LL));
            }
        } else if ((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("List", 4LL))))) {
#line 7528 "src/sema.tr"
            if (((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL))))) {
#line 7528 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            } else if (_tr_str_eqv((prop), (_tr_str_lit_len("data", 4LL)))) {
#line 7529 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("Pointer", 7LL));
            }
        } else if (((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Set", 3LL))))) {
#line 7531 "src/sema.tr"
            if (((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL))))) {
#line 7531 "src/sema.tr"
                ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
            }
        } else if (((_tr_str_eqv((hobj_ty_n), (_tr_str_lit_len("Shared", 6LL))) && (hobj_ty_full->args->len > 0LL)) && ({ __auto_type _ord_t1775 = ((*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)))->name); __auto_type _wr = (_tr_dict_contains(self->classes, _tr_strz(_ord_t1775))); _wr; }))) {
#line 7536 "src/sema.tr"
            ClassDef* _scls = ({ __auto_type _ord_t1776 = ((*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)))->name); __auto_type _wr = (((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_ord_t1776)))); _wr; });
#line 7537 "src/sema.tr"
            long long _sfi = 0LL;
#line 7538 "src/sema.tr"
            while ((_sfi < _scls->fields->len)) {
#line 7539 "src/sema.tr"
                FieldDef* _sfld = ((FieldDef*)List_ptr_get(_scls->fields, _sfi));
#line 7540 "src/sema.tr"
                if (_tr_str_eqv((_sfld->name), (prop))) {
#line 7541 "src/sema.tr"
                    if ((((unsigned long long)(_sfld->ty)) != ((unsigned long long)(0LL)))) {
#line 7541 "src/sema.tr"
                        ret_ty = (*_sfld->ty);
                    }
                }
#line 7542 "src/sema.tr"
                _sfi = (_sfi + 1LL);
            }
        } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty_n))) {
#line 7544 "src/sema.tr"
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty_n)));
#line 7545 "src/sema.tr"
            long long _fi = 0LL;
#line 7546 "src/sema.tr"
            while ((_fi < _cls->fields->len)) {
#line 7547 "src/sema.tr"
                FieldDef* _fld = ((FieldDef*)List_ptr_get(_cls->fields, _fi));
#line 7548 "src/sema.tr"
                if (_tr_str_eqv((_fld->name), (prop))) {
#line 7549 "src/sema.tr"
                    if ((((unsigned long long)(_fld->ty)) != ((unsigned long long)(0LL)))) {
#line 7549 "src/sema.tr"
                        ret_ty = (*_fld->ty);
                    }
                }
#line 7550 "src/sema.tr"
                _fi = (_fi + 1LL);
            }
#line 7562 "src/sema.tr"
            if (((_cls->generics->len > 0LL) && (hobj_ty_full->args->len > 0LL))) {
#line 7563 "src/sema.tr"
                ret_ty = Sema__subst_ret_generics(self, ret_ty, _cls->generics, hobj_ty_full->args);
            }
        } else if (_tr_dict_contains(self->enums, _tr_strz(hobj_ty_n))) {
#line 7566 "src/sema.tr"
            ret_ty = AstType_init(hobj_ty_n);
        }
#line 7567 "src/sema.tr"
        if ((_tr_str_eqv((ret_ty->name), (_tr_str_lit_len("void", 4LL))) && ((_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("length", 6LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("capacity", 8LL)))))) {
#line 7568 "src/sema.tr"
            ret_ty = AstType_init(_tr_str_lit_len("int", 3LL));
        }
#line 7569 "src/sema.tr"
        HirExpr* _trret270 = box_hirexpr(HirExpr_ctor_EPropAccess(hobj, prop, ret_ty));
        _tr_str_release(hobj_ty_n);
        return _trret270;
    } else if (_t1600.tag == Expr_EIndex) {
        __auto_type obj = _t1600.data.EIndex.obj;
__auto_type idx_inner = _t1600.data.EIndex._tr_v_index;
#line 7571 "src/sema.tr"
        HirExpr* hexpr_obj = Sema_lower_expr(self, obj);
#line 7572 "src/sema.tr"
        TrStr obj_ty_n = _tr_str_retain(hir_expr_type(hexpr_obj)->name);
#line 7573 "src/sema.tr"
        TrStr obj_name = _tr_str_lit_len("", 0LL);
#line 7574 "src/sema.tr"
        __auto_type _t1777 = (*obj);
        if (_t1777.tag == Expr_EIdent) {
            __auto_type n = _t1777.data.EIdent.name;
            TrStr _strtmp_t1778 = _tr_str_retain(n);
            _tr_str_release(obj_name);
            obj_name = _strtmp_t1778;
        } else if (1) {
            __auto_type _ = _t1777;
            /* pass */
        }
#line 7577 "src/sema.tr"
        bool is_generic = false;
#line 7578 "src/sema.tr"
        TrStr generic_arg_n = _tr_str_lit_len("", 0LL);
#line 7579 "src/sema.tr"
        AstType** generic_arg_ty = ((AstType**)(0LL));
#line 7583 "src/sema.tr"
        List_ptr* generic_args = (void*)List_ptr_new();
#line 7584 "src/sema.tr"
        if ((((unsigned long long)(idx_inner)) == ((unsigned long long)(0LL)))) {
#line 7585 "src/sema.tr"
            HirExpr* _trret271 = hexpr_obj;
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return _trret271;
        }
#line 7586 "src/sema.tr"
        __auto_type _t1779 = (*idx_inner);
        if (_t1779.tag == Expr_ETuple) {
            __auto_type _tup_targs = _t1779.data.ETuple.items;
#line 7589 "src/sema.tr"
            is_generic = true;
#line 7590 "src/sema.tr"
            long long _tti = 0LL;
#line 7591 "src/sema.tr"
            while ((_tti < _tup_targs->len)) {
#line 7592 "src/sema.tr"
                List_ptr_append(generic_args, Sema__targ_of(self, ((Expr*)List_ptr_get(_tup_targs, _tti))));
#line 7593 "src/sema.tr"
                _tti = (_tti + 1LL);
            }
#line 7594 "src/sema.tr"
            if ((generic_args->len > 0LL)) {
#line 7595 "src/sema.tr"
                generic_arg_ty = ((AstType**)List_ptr_get(generic_args, 0LL));
#line 7596 "src/sema.tr"
                TrStr _strtmp_t1780 = _tr_str_retain((*((AstType**)List_ptr_get(generic_args, 0LL)))->name);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t1780;
            }
        } else if (_t1779.tag == Expr_ETypeArg) {
            __auto_type targ_ty = _t1779.data.ETypeArg.ty;
#line 7598 "src/sema.tr"
            is_generic = true;
#line 7599 "src/sema.tr"
            generic_arg_ty = targ_ty;
#line 7600 "src/sema.tr"
            TrStr _strtmp_t1781 = _tr_str_retain((*targ_ty)->name);
            _tr_str_release(generic_arg_n);
            generic_arg_n = _strtmp_t1781;
        } else if (_t1779.tag == Expr_EIdent) {
            __auto_type iname = _t1779.data.EIdent.name;
#line 7602 "src/sema.tr"
            bool is_param = false;
#line 7603 "src/sema.tr"
            if ((!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL))))) {
#line 7604 "src/sema.tr"
                if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
#line 7605 "src/sema.tr"
                    ClassDef* cc = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
#line 7606 "src/sema.tr"
                    long long ci = 0LL;
#line 7607 "src/sema.tr"
                    while ((ci < cc->generics->len)) {
#line 7608 "src/sema.tr"
                        if (({ TrStr _cmpa_t1782 = (List_TrStr_get(cc->generics, ci)); TrStr _cmpb_t1783 = (iname); _Bool _cmpr_t1784 = _tr_str_eqv(_cmpa_t1782, _cmpb_t1783); _tr_str_release(_cmpa_t1782); _cmpr_t1784; })) {
#line 7608 "src/sema.tr"
                            is_param = true;
                        }
#line 7609 "src/sema.tr"
                        ci = (ci + 1LL);
                    }
                }
            }
#line 7610 "src/sema.tr"
            long long fi = 0LL;
#line 7611 "src/sema.tr"
            while ((fi < self->current_func_generics->len)) {
#line 7612 "src/sema.tr"
                if (({ TrStr _cmpa_t1785 = (List_TrStr_get(self->current_func_generics, fi)); TrStr _cmpb_t1786 = (iname); _Bool _cmpr_t1787 = _tr_str_eqv(_cmpa_t1785, _cmpb_t1786); _tr_str_release(_cmpa_t1785); _cmpr_t1787; })) {
#line 7612 "src/sema.tr"
                    is_param = true;
                }
#line 7613 "src/sema.tr"
                fi = (fi + 1LL);
            }
#line 7615 "src/sema.tr"
            if ((((((((((((((Sema_is_primitive_name(self, iname) || _tr_str_eqv((iname), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("Str", 3LL)))) || is_param) || (_tr_strlen(_tr_strz(iname)) == 1LL)) || _tr_dict_contains(self->classes, _tr_strz(iname))) || _tr_str_eqv((iname), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_dict_contains(self->enums, _tr_strz(iname))) || _tr_str_eqv((iname), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((iname), (_tr_str_lit_len("tuple", 5LL))))) {
#line 7616 "src/sema.tr"
                is_generic = true;
#line 7617 "src/sema.tr"
                TrStr _strtmp_t1788 = _tr_str_retain(iname);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t1788;
            }
        } else if (_t1779.tag == Expr_EIndex) {
#line 7623 "src/sema.tr"
            is_generic = true;
#line 7624 "src/sema.tr"
            AstType** nested_ty = Sema_build_ast_type(self, idx_inner);
#line 7625 "src/sema.tr"
            if ((((unsigned long long)(nested_ty)) != ((unsigned long long)(0LL)))) {
#line 7626 "src/sema.tr"
                TrStr _strtmp_t1789 = _tr_str_retain((*nested_ty)->name);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t1789;
#line 7627 "src/sema.tr"
                generic_arg_ty = nested_ty;
            }
        } else if (1) {
            __auto_type _ = _t1779;
            /* pass */
        }
#line 7635 "src/sema.tr"
        bool obj_is_type = (((((((((((_tr_str_eqv((obj_name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Pointer", 7LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("alloc", 5LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("dealloc", 7LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("resize", 6LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("copy", 4LL)))) || _tr_dict_contains(self->classes, _tr_strz(obj_name))) || _tr_dict_contains(self->enums, _tr_strz(obj_name)));
#line 7636 "src/sema.tr"
        if ((is_generic && obj_is_type)) {
#line 7640 "src/sema.tr"
            TrStr eff_ty_n = _tr_str_retain(obj_ty_n);
#line 7641 "src/sema.tr"
            if (((_tr_str_eqv((eff_ty_n), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("", 0LL)))) && (((((_tr_str_eqv((obj_name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("Pointer", 7LL)))))) {
#line 7642 "src/sema.tr"
                TrStr _strtmp_t1790 = _tr_str_retain(obj_name);
                _tr_str_release(eff_ty_n);
                eff_ty_n = _strtmp_t1790;
            }
#line 7643 "src/sema.tr"
            if (((((((_tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((eff_ty_n), (_tr_str_lit_len("Pointer", 7LL)))) && (!_tr_str_eqv((generic_arg_n), (_tr_str_lit_len("", 0LL)))))) {
#line 7644 "src/sema.tr"
                AstType* container_ty = AstType_init(eff_ty_n);
#line 7645 "src/sema.tr"
                container_ty->args = (void*)List_ptr_new();
#line 7649 "src/sema.tr"
                if ((generic_args->len > 0LL)) {
#line 7650 "src/sema.tr"
                    long long _gci = 0LL;
#line 7651 "src/sema.tr"
                    while ((_gci < generic_args->len)) {
#line 7652 "src/sema.tr"
                        ({ __auto_type _ord_t1791 = (((AstType**)List_ptr_get(generic_args, _gci))); List_ptr_append(container_ty->args, _ord_t1791); });
#line 7653 "src/sema.tr"
                        _gci = (_gci + 1LL);
                    }
                } else if ((((unsigned long long)(generic_arg_ty)) != ((unsigned long long)(0LL)))) {
#line 7655 "src/sema.tr"
                    List_ptr_append(container_ty->args, generic_arg_ty);
                } else {
#line 7657 "src/sema.tr"
                    ({ __auto_type _ord_t1792 = (box_asttype(AstType_init(generic_arg_n))); List_ptr_append(container_ty->args, _ord_t1792); });
                }
#line 7658 "src/sema.tr"
                if ((((_tr_str_eqv((obj_name), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("dealloc", 7LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("resize", 6LL)))) || _tr_str_eqv((obj_name), (_tr_str_lit_len("copy", 4LL))))) {
#line 7659 "src/sema.tr"
                    HirExpr* _trret272 = box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_index_operand(self, idx_inner, hexpr_obj), container_ty));
                    _tr_str_release(obj_ty_n);
                    _tr_str_release(obj_name);
                    _tr_str_release(generic_arg_n);
                    _tr_str_release(eff_ty_n);
                    return _trret272;
                }
#line 7660 "src/sema.tr"
                HirExpr* _trret273 = box_hirexpr(HirExpr_ctor_EIdent(eff_ty_n, container_ty, false));
                _tr_str_release(obj_ty_n);
                _tr_str_release(obj_name);
                _tr_str_release(generic_arg_n);
                _tr_str_release(eff_ty_n);
                return _trret273;
            }
#line 7662 "src/sema.tr"
            if (((_tr_dict_contains(self->classes, _tr_strz(obj_name)) || _tr_dict_contains(self->enums, _tr_strz(obj_name))) && (!_tr_str_eqv((generic_arg_n), (_tr_str_lit_len("", 0LL)))))) {
#line 7663 "src/sema.tr"
                AstType* cls_ty = AstType_init(obj_name);
#line 7664 "src/sema.tr"
                cls_ty->args = (void*)List_ptr_new();
#line 7665 "src/sema.tr"
                if ((generic_args->len > 0LL)) {
#line 7666 "src/sema.tr"
                    long long _cgi = 0LL;
#line 7667 "src/sema.tr"
                    while ((_cgi < generic_args->len)) {
#line 7668 "src/sema.tr"
                        ({ __auto_type _ord_t1793 = (((AstType**)List_ptr_get(generic_args, _cgi))); List_ptr_append(cls_ty->args, _ord_t1793); });
#line 7669 "src/sema.tr"
                        _cgi = (_cgi + 1LL);
                    }
                } else if ((((unsigned long long)(generic_arg_ty)) != ((unsigned long long)(0LL)))) {
#line 7672 "src/sema.tr"
                    List_ptr_append(cls_ty->args, generic_arg_ty);
                } else {
#line 7674 "src/sema.tr"
                    ({ __auto_type _ord_t1794 = (box_asttype(AstType_init(generic_arg_n))); List_ptr_append(cls_ty->args, _ord_t1794); });
                }
#line 7676 "src/sema.tr"
                Sema_check_class_bounds(self, obj_name, cls_ty->args);
#line 7677 "src/sema.tr"
                HirExpr* _trret274 = box_hirexpr(HirExpr_ctor_EIdent(obj_name, cls_ty, false));
                _tr_str_release(obj_ty_n);
                _tr_str_release(obj_name);
                _tr_str_release(generic_arg_n);
                _tr_str_release(eff_ty_n);
                return _trret274;
            }
#line 7678 "src/sema.tr"
            HirExpr* _trret275 = hexpr_obj;
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            _tr_str_release(eff_ty_n);
            return _trret275;
        }
#line 7683 "src/sema.tr"
        if ((_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Dict", 4LL))))) {
#line 7684 "src/sema.tr"
            AstType* dval_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7685 "src/sema.tr"
            AstType* dobj_full = hir_expr_type(hexpr_obj);
#line 7686 "src/sema.tr"
            if ((dobj_full->args->len > 1LL)) {
#line 7686 "src/sema.tr"
                dval_ty = (*((AstType**)List_ptr_get(dobj_full->args, 1LL)));
            }
#line 7687 "src/sema.tr"
            List_ptr* didx_args = (void*)List_ptr_new();
#line 7688 "src/sema.tr"
            List_ptr_append(didx_args, Sema_lower_index_operand(self, idx_inner, hexpr_obj));
#line 7689 "src/sema.tr"
            HirExpr* _trret276 = box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit_len("get_index", 9LL), didx_args, dval_ty));
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return _trret276;
        }
#line 7692 "src/sema.tr"
        if (_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Tuple", 5LL)))) {
#line 7693 "src/sema.tr"
            AstType* tup_full = hir_expr_type(hexpr_obj);
#line 7694 "src/sema.tr"
            AstType* telem_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7695 "src/sema.tr"
            __auto_type _t1795 = (*idx_inner);
            if (_t1795.tag == Expr_ELitInt) {
                __auto_type tup_ci = _t1795.data.ELitInt.val;
#line 7697 "src/sema.tr"
                if (((tup_ci >= 0LL) && (tup_ci < tup_full->args->len))) {
#line 7698 "src/sema.tr"
                    telem_ty = (*((AstType**)List_ptr_get(tup_full->args, tup_ci)));
                }
            } else if (1) {
                __auto_type _ = _t1795;
                /* pass */
            }
#line 7700 "src/sema.tr"
            List_ptr* tup_args = (void*)List_ptr_new();
#line 7701 "src/sema.tr"
            List_ptr_append(tup_args, Sema_lower_index_operand(self, idx_inner, hexpr_obj));
#line 7702 "src/sema.tr"
            HirExpr* _trret277 = box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit_len("get_index", 9LL), tup_args, telem_ty));
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return _trret277;
        }
#line 7703 "src/sema.tr"
        if ((((((!Sema_is_primitive_name(self, obj_ty_n)) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("str", 3LL))))) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Pointer", 7LL))))) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Vec", 3LL)))))) {
#line 7704 "src/sema.tr"
            List_ptr* call_args = (void*)List_ptr_new();
#line 7705 "src/sema.tr"
            List_ptr_append(call_args, Sema_lower_index_operand(self, idx_inner, hexpr_obj));
#line 7706 "src/sema.tr"
            HirExpr* _trret278 = box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit_len("get_index", 9LL), call_args, AstType_init(_tr_str_lit_len("void", 4LL))));
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return _trret278;
        }
#line 7708 "src/sema.tr"
        AstType* elem_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7709 "src/sema.tr"
        if ((_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Vec", 3LL))))) {
#line 7710 "src/sema.tr"
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
#line 7711 "src/sema.tr"
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if (_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 7713 "src/sema.tr"
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
#line 7714 "src/sema.tr"
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if (_tr_str_eqv((obj_ty_n), (_tr_str_lit_len("str", 3LL)))) {
#line 7715 "src/sema.tr"
            elem_ty = AstType_init(_tr_str_lit_len("char", 4LL));
        }
#line 7716 "src/sema.tr"
        HirExpr* _trret279 = box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_index_operand(self, idx_inner, hexpr_obj), elem_ty));
        _tr_str_release(obj_ty_n);
        _tr_str_release(obj_name);
        _tr_str_release(generic_arg_n);
        return _trret279;
    } else if (_t1600.tag == Expr_ESizeOf) {
        __auto_type ty = _t1600.data.ESizeOf.ty;
#line 7718 "src/sema.tr"
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
#line 7718 "src/sema.tr"
            return ({ __auto_type _ord_t1796 = (AstType_init(_tr_str_lit_len("void", 4LL))); __auto_type _ord_t1797 = (AstType_init(_tr_str_lit_len("int", 3LL))); __auto_type _wr = (box_hirexpr(HirExpr_ctor_ESizeOf(_ord_t1796, _ord_t1797))); _wr; });
        }
#line 7719 "src/sema.tr"
        return ({ __auto_type _ord_t1798 = ((*ty)); __auto_type _ord_t1799 = (AstType_init(_tr_str_lit_len("int", 3LL))); __auto_type _wr = (box_hirexpr(HirExpr_ctor_ESizeOf(_ord_t1798, _ord_t1799))); _wr; });
    } else if (_t1600.tag == Expr_ECast) {
        __auto_type expr = _t1600.data.ECast.expr;
__auto_type ty = _t1600.data.ECast.ty;
#line 7721 "src/sema.tr"
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
#line 7721 "src/sema.tr"
            return Sema_lower_expr(self, expr);
        }
#line 7722 "src/sema.tr"
        return ({ __auto_type _ord_t1800 = (Sema_lower_expr(self, expr)); __auto_type _ord_t1801 = ((*ty)); __auto_type _wr = (box_hirexpr(HirExpr_ctor_ECast(_ord_t1800, _ord_t1801))); _wr; });
    } else if (_t1600.tag == Expr_EFString) {
        __auto_type parts = _t1600.data.EFString.parts;
#line 7724 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("an f-string", 11LL), _tr_str_lit_len("build output in a fixed byte buffer, or print the parts separately; plain string literals are fine (they live in rodata)", 120LL));
#line 7725 "src/sema.tr"
        List_ptr* hparts = (void*)List_ptr_new();
#line 7726 "src/sema.tr"
        long long m = 0LL;
#line 7727 "src/sema.tr"
        while ((m < parts->len)) {
#line 7728 "src/sema.tr"
            FStringPart* p_ast = ((FStringPart*)List_ptr_get(parts, m));
#line 7729 "src/sema.tr"
            HirFStringPart* hp = HirFStringPart_init();
#line 7730 "src/sema.tr"
            hp->is_expr = p_ast->is_expr;
#line 7731 "src/sema.tr"
            hp->text = _tr_str_retain(p_ast->text);
#line 7732 "src/sema.tr"
            hp->text_len = p_ast->text_len;
#line 7733 "src/sema.tr"
            hp->fmt_spec = _tr_str_retain(p_ast->fmt_spec);
#line 7734 "src/sema.tr"
            hp->expr = Sema_lower_expr(self, p_ast->expr);
#line 7735 "src/sema.tr"
            long long sai = 0LL;
#line 7736 "src/sema.tr"
            while ((sai < p_ast->spec_args->len)) {
#line 7737 "src/sema.tr"
                ({ __auto_type _ord_t1802 = (Sema_lower_expr(self, ((Expr*)List_ptr_get(p_ast->spec_args, sai)))); List_ptr_append(hp->spec_args, _ord_t1802); });
#line 7738 "src/sema.tr"
                sai = (sai + 1LL);
            }
#line 7739 "src/sema.tr"
            List_ptr_append(hparts, _tr_obj_retain(hp));
#line 7740 "src/sema.tr"
            m = (m + 1LL);
            _tr_obj_release(hp, _trdrop_HirFStringPart);
        }
#line 7741 "src/sema.tr"
        HirExpr* _trret280 = box_hirexpr(HirExpr_ctor_EFString(hparts, AstType_init(_tr_str_lit_len("str", 3LL))));
        return _trret280;
    } else if (_t1600.tag == Expr_ETuple) {
        __auto_type items = _t1600.data.ETuple.items;
#line 7746 "src/sema.tr"
        if ((self->no_heap && self->heap_boxes_tuples)) {
#line 7747 "src/sema.tr"
            Sema_check_no_heap(self, _tr_str_lit_len("a tuple literal (heap-boxed on the LLVM/native backend)", 55LL), _tr_str_lit_len("use the C backend (stack tuples) for --no-heap, or return values via out-params / a @value_type struct", 102LL));
        }
#line 7748 "src/sema.tr"
        List_ptr* hitems = (void*)List_ptr_new();
#line 7749 "src/sema.tr"
        long long n = 0LL;
#line 7750 "src/sema.tr"
        while ((n < items->len)) {
#line 7751 "src/sema.tr"
            List_ptr_append(hitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, n))));
#line 7752 "src/sema.tr"
            n = (n + 1LL);
        }
#line 7756 "src/sema.tr"
        AstType* tup_ty = AstType_init(_tr_str_lit_len("Tuple", 5LL));
#line 7757 "src/sema.tr"
        long long m = 0LL;
#line 7758 "src/sema.tr"
        while ((m < hitems->len)) {
#line 7759 "src/sema.tr"
            ({ __auto_type _ord_t1803 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hitems, m))))); List_ptr_append(tup_ty->args, _ord_t1803); });
#line 7760 "src/sema.tr"
            m = (m + 1LL);
        }
#line 7761 "src/sema.tr"
        HirExpr* _trret281 = box_hirexpr(HirExpr_ctor_ETuple(hitems, tup_ty));
        return _trret281;
    } else if (_t1600.tag == Expr_EList) {
        __auto_type items = _t1600.data.EList.items;
#line 7763 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("a list literal []", 17LL), _tr_str_lit_len("use a fixed-size array [T; N] (stack, zero-heap) instead", 56LL));
#line 7764 "src/sema.tr"
        List_ptr* hitems = (void*)List_ptr_new();
#line 7765 "src/sema.tr"
        long long n = 0LL;
#line 7766 "src/sema.tr"
        while ((n < items->len)) {
#line 7767 "src/sema.tr"
            List_ptr_append(hitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, n))));
#line 7768 "src/sema.tr"
            n = (n + 1LL);
        }
#line 7769 "src/sema.tr"
        AstType* list_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 7770 "src/sema.tr"
        if ((hitems->len > 1LL)) {
#line 7771 "src/sema.tr"
            bool _hl_str = false;
#line 7772 "src/sema.tr"
            bool _hl_num = false;
#line 7773 "src/sema.tr"
            long long _hl_i = 0LL;
#line 7774 "src/sema.tr"
            while ((_hl_i < hitems->len)) {
#line 7775 "src/sema.tr"
                TrStr _hl_f = Sema_firm_family(self, ((HirExpr*)List_ptr_get(hitems, _hl_i)));
#line 7776 "src/sema.tr"
                if (_tr_str_eqv((_hl_f), (_tr_str_lit_len("str", 3LL)))) {
#line 7776 "src/sema.tr"
                    _hl_str = true;
                }
#line 7777 "src/sema.tr"
                if ((_tr_str_eqv((_hl_f), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((_hl_f), (_tr_str_lit_len("float", 5LL))))) {
#line 7777 "src/sema.tr"
                    _hl_num = true;
                }
#line 7778 "src/sema.tr"
                _hl_i = (_hl_i + 1LL);
                _tr_str_release(_hl_f);
            }
#line 7779 "src/sema.tr"
            if ((_hl_str && _hl_num)) {
#line 7780 "src/sema.tr"
                Sema_error(self, _tr_str_lit_len("[E-5] Type mismatch: a list literal mixes strings and numbers.\n      FIX: a list holds one element type; convert the items (str(x) / int(s)) or use separate lists.", 163LL));
            }
        }
#line 7781 "src/sema.tr"
        if ((hitems->len > 0LL)) {
#line 7782 "src/sema.tr"
            list_ty->args = (void*)List_ptr_new();
#line 7783 "src/sema.tr"
            ({ __auto_type _ord_t1804 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hitems, 0LL))))); List_ptr_append(list_ty->args, _ord_t1804); });
        } else if ((((unsigned long long)(self->list_hint)) != ((unsigned long long)(0LL)))) {
#line 7786 "src/sema.tr"
            list_ty = (*self->list_hint);
        }
#line 7787 "src/sema.tr"
        self->list_hint = (AstType**)(0LL);
#line 7788 "src/sema.tr"
        HirExpr* _trret282 = box_hirexpr(HirExpr_ctor_EList(hitems, list_ty));
        return _trret282;
    } else if (_t1600.tag == Expr_ESet) {
        __auto_type items = _t1600.data.ESet.items;
#line 7790 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("a set literal", 13LL), _tr_str_lit_len("use a fixed-size array [T; N] + linear scan, or a bitset over a fixed word", 74LL));
#line 7791 "src/sema.tr"
        List_ptr* sitems = (void*)List_ptr_new();
#line 7792 "src/sema.tr"
        long long sn = 0LL;
#line 7793 "src/sema.tr"
        while ((sn < items->len)) {
#line 7794 "src/sema.tr"
            List_ptr_append(sitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, sn))));
#line 7795 "src/sema.tr"
            sn = (sn + 1LL);
        }
#line 7798 "src/sema.tr"
        AstType* set_ty = AstType_init(_tr_str_lit_len("Set", 3LL));
#line 7799 "src/sema.tr"
        if ((sitems->len > 0LL)) {
#line 7800 "src/sema.tr"
            set_ty->args = (void*)List_ptr_new();
#line 7801 "src/sema.tr"
            ({ __auto_type _ord_t1805 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(sitems, 0LL))))); List_ptr_append(set_ty->args, _ord_t1805); });
        }
#line 7802 "src/sema.tr"
        HirExpr* _trret283 = box_hirexpr(HirExpr_ctor_ESet(sitems, set_ty));
        return _trret283;
    } else if (_t1600.tag == Expr_EClosure) {
        __auto_type params = _t1600.data.EClosure.params;
__auto_type ret_ty = _t1600.data.EClosure.ret_ty;
__auto_type body = _t1600.data.EClosure.body;
__auto_type is_async = _t1600.data.EClosure.is_async;
#line 7804 "src/sema.tr"
        self->closure_depth = (self->closure_depth + 1LL);
#line 7807 "src/sema.tr"
        AstType** _cl_hint = self->closure_hint;
#line 7808 "src/sema.tr"
        self->closure_hint = (AstType**)(0LL);
#line 7809 "src/sema.tr"
        bool _cl_hint_ok = ((((unsigned long long)(_cl_hint)) != ((unsigned long long)(0LL))) && ({ __auto_type _ord_t1806 = ((*_cl_hint)->args->len); __auto_type _wr = ((_ord_t1806 == (params->len + 1LL))); _wr; }));
#line 7810 "src/sema.tr"
        Sema_enter_scope(self);
#line 7811 "src/sema.tr"
        long long cap_idx = (self->scopes->len - 1LL);
#line 7812 "src/sema.tr"
        List_ptr* hparams = (void*)List_ptr_new();
#line 7813 "src/sema.tr"
        long long l = 0LL;
#line 7814 "src/sema.tr"
        while ((l < params->len)) {
#line 7815 "src/sema.tr"
            Param* pa = ((Param*)List_ptr_get(params, l));
#line 7816 "src/sema.tr"
            AstType* pa_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 7817 "src/sema.tr"
            if ((((unsigned long long)(pa->ty)) != ((unsigned long long)(0LL)))) {
#line 7818 "src/sema.tr"
                pa_ty = (*pa->ty);
            } else if ((_cl_hint_ok && (((unsigned long long)(((AstType**)List_ptr_get((*_cl_hint)->args, l)))) != ((unsigned long long)(0LL))))) {
#line 7820 "src/sema.tr"
                pa_ty = (*((AstType**)List_ptr_get((*_cl_hint)->args, l)));
            }
#line 7821 "src/sema.tr"
            ({ __auto_type _ord_t1807 = (pa->name); __auto_type _ord_t1808 = (SymbolKind_make_SVariable()); __auto_type _ord_t1809 = (box_asttype(pa_ty)); Sema_declare(self, _ord_t1807, _ord_t1808, _ord_t1809, false); });
#line 7822 "src/sema.tr"
            HirParam* hpa = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 7823 "src/sema.tr"
            hpa->name = _tr_str_retain(pa->name);
#line 7824 "src/sema.tr"
            hpa->ty = pa_ty;
#line 7825 "src/sema.tr"
            List_ptr_append(hparams, _tr_obj_retain(hpa));
#line 7826 "src/sema.tr"
            l = (l + 1LL);
            _tr_obj_release(hpa, _trdrop_HirParam);
        }
#line 7827 "src/sema.tr"
        AstType* r_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7828 "src/sema.tr"
        if ((((unsigned long long)(ret_ty)) != ((unsigned long long)(0LL)))) {
#line 7828 "src/sema.tr"
            r_ty = (*ret_ty);
        }
#line 7831 "src/sema.tr"
        List_i64_append(self->fn_scope_base, (self->scopes->len - 1LL));
#line 7832 "src/sema.tr"
        HirBlock* clo_body = Sema_lower_block(self, body);
#line 7833 "src/sema.tr"
        List_i64_pop(self->fn_scope_base);
#line 7834 "src/sema.tr"
        Sema_finalize_scope_drops(self, clo_body);
#line 7840 "src/sema.tr"
        List_TrStr* clo_refs = (void*)List_TrStr_new();
#line 7841 "src/sema.tr"
        Sema_collect_block_refs(self, clo_body, clo_refs);
#line 7842 "src/sema.tr"
        List_ptr* clo_caps = (void*)List_ptr_new();
#line 7843 "src/sema.tr"
        TrMap* clo_seen = _tr_dict_new(8LL);
#line 7844 "src/sema.tr"
        long long cri = 0LL;
#line 7845 "src/sema.tr"
        while ((cri < clo_refs->len)) {
#line 7846 "src/sema.tr"
            TrStr rn = List_TrStr_get(clo_refs, cri);
#line 7847 "src/sema.tr"
            if ((!_tr_dict_contains(clo_seen, _tr_strz(rn)))) {
#line 7848 "src/sema.tr"
                _tr_dict_set(clo_seen, _tr_strz(rn), true);
#line 7849 "src/sema.tr"
                if ((!_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, cap_idx))->variables, _tr_strz(rn)))) {
#line 7850 "src/sema.tr"
                    long long si = (cap_idx - 1LL);
#line 7851 "src/sema.tr"
                    AstType** cty_p = (AstType**)(0LL);
#line 7852 "src/sema.tr"
                    bool cap_mut = true;
#line 7853 "src/sema.tr"
                    while ((si >= 0LL)) {
#line 7854 "src/sema.tr"
                        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, si))->variables, _tr_strz(rn))) {
#line 7855 "src/sema.tr"
                            Symbol* cap_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, si))->variables, _tr_strz(rn)));
#line 7856 "src/sema.tr"
                            cty_p = cap_sym->ty;
#line 7857 "src/sema.tr"
                            cap_mut = (cap_sym->is_mut || (cap_sym->kind.tag != SymbolKind_make_SVariable().tag));
#line 7858 "src/sema.tr"
                            si = (0LL - 1LL);
                        } else {
#line 7860 "src/sema.tr"
                            si = (si - 1LL);
                        }
                    }
#line 7861 "src/sema.tr"
                    if ((((unsigned long long)(cty_p)) != ((unsigned long long)(0LL)))) {
#line 7862 "src/sema.tr"
                        HirParam* ccp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
#line 7863 "src/sema.tr"
                        ccp->name = _tr_str_retain(rn);
#line 7864 "src/sema.tr"
                        ccp->ty = (*cty_p);
#line 7869 "src/sema.tr"
                        ccp->by_val = (!cap_mut);
#line 7870 "src/sema.tr"
                        if ((ccp->by_val && _tr_dict_contains(self->classes, _tr_strz(ccp->ty->name)))) {
#line 7871 "src/sema.tr"
                            if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ccp->ty->name)))->is_class)) {
#line 7871 "src/sema.tr"
                                ccp->by_val = false;
                            }
                        }
#line 7872 "src/sema.tr"
                        List_ptr_append(clo_caps, _tr_obj_retain(ccp));
                    }
                }
            }
#line 7873 "src/sema.tr"
            cri = (cri + 1LL);
            _tr_str_release(rn);
        }
#line 7877 "src/sema.tr"
        if ((((unsigned long long)(ret_ty)) == ((unsigned long long)(0LL)))) {
#line 7878 "src/sema.tr"
            AstType* inf_ty = Sema_closure_return_type(self, clo_body);
#line 7879 "src/sema.tr"
            if ((((!_tr_str_eqv((inf_ty->name), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((inf_ty->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((inf_ty->name), (_tr_str_lit_len("None", 4LL)))))) {
#line 7880 "src/sema.tr"
                r_ty = inf_ty;
            }
        }
#line 7881 "src/sema.tr"
        HirExpr hexpr = HirExpr_ctor_EClosure(hparams, r_ty, clo_body, is_async, clo_caps);
#line 7882 "src/sema.tr"
        self->closure_depth = (self->closure_depth - 1LL);
#line 7883 "src/sema.tr"
        Sema_exit_scope(self);
#line 7884 "src/sema.tr"
        HirExpr* _trret284 = box_hirexpr(hexpr);
        _tr_obj_release(clo_body, _trdrop_HirBlock);
        Dict_free(clo_seen);
        return _trret284;
    } else if (_t1600.tag == Expr_EIfElse) {
        __auto_type cond = _t1600.data.EIfElse.cond;
__auto_type then_e = _t1600.data.EIfElse.then_expr;
__auto_type else_e = _t1600.data.EIfElse.else_expr;
#line 7886 "src/sema.tr"
        HirExpr* hcond = Sema_lower_expr(self, cond);
#line 7887 "src/sema.tr"
        HirExpr* hthen = Sema_lower_expr(self, then_e);
#line 7888 "src/sema.tr"
        HirExpr* helse = Sema_lower_expr(self, else_e);
#line 7889 "src/sema.tr"
        AstType* ite_ty = hir_expr_type(hthen);
#line 7890 "src/sema.tr"
        HirExpr* _trret285 = box_hirexpr(HirExpr_ctor_EIfElse(hcond, hthen, helse, ite_ty));
        return _trret285;
    } else if (_t1600.tag == Expr_EDo) {
        __auto_type do_body = _t1600.data.EDo.body;
#line 7901 "src/sema.tr"
        HirExpr* _trret286 = Sema_lower_do_value(self, do_body);
        return _trret286;
    } else if (_t1600.tag == Expr_EMatch) {
        __auto_type m_subj = _t1600.data.EMatch.subj;
__auto_type m_arms = _t1600.data.EMatch.arms;
#line 7907 "src/sema.tr"
        HirExpr* hm_subj = Sema_lower_expr(self, m_subj);
#line 7908 "src/sema.tr"
        AstType* hm_subj_ty = hir_expr_type(hm_subj);
#line 7909 "src/sema.tr"
        List_ptr* hm_arms = (void*)List_ptr_new();
#line 7910 "src/sema.tr"
        AstType* hm_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7911 "src/sema.tr"
        long long hm_k = 0LL;
#line 7912 "src/sema.tr"
        while ((hm_k < m_arms->len)) {
#line 7913 "src/sema.tr"
            MatchArm* hm_arm = ((MatchArm*)List_ptr_get(m_arms, hm_k));
#line 7914 "src/sema.tr"
            Sema_enter_scope(self);
#line 7915 "src/sema.tr"
            Sema_declare_pattern_binds_typed(self, hm_arm->pat, hm_subj_ty);
#line 7916 "src/sema.tr"
            HirExpr* hm_edo = Sema_lower_do_value(self, (*hm_arm->body));
#line 7917 "src/sema.tr"
            if ((!_tr_str_eqv((hir_expr_type(hm_edo)->name), (_tr_str_lit_len("void", 4LL))))) {
#line 7917 "src/sema.tr"
                hm_ty = hir_expr_type(hm_edo);
            }
#line 7918 "src/sema.tr"
            HirBlock* hm_body = HirBlock_init();
#line 7919 "src/sema.tr"
            HirBlock_push(hm_body, box_hirstmt(HirStmt_ctor_SExpr(hm_edo)));
#line 7920 "src/sema.tr"
            HirMatchArm* h_marm = HirMatchArm_init(hm_arm->pat, hm_body);
#line 7921 "src/sema.tr"
            if ((((unsigned long long)(hm_arm->guard)) != ((unsigned long long)(0LL)))) {
#line 7922 "src/sema.tr"
                h_marm->guard = Sema_lower_expr(self, hm_arm->guard);
            }
#line 7923 "src/sema.tr"
            List_ptr_append(hm_arms, _tr_obj_retain(h_marm));
#line 7924 "src/sema.tr"
            Sema_exit_scope(self);
#line 7925 "src/sema.tr"
            hm_k = (hm_k + 1LL);
            _tr_obj_release(hm_body, _trdrop_HirBlock);
            _tr_obj_release(h_marm, _trdrop_HirMatchArm);
        }
#line 7926 "src/sema.tr"
        Sema_check_match_expr_exhaustive(self, hm_subj_ty->name, m_arms);
#line 7927 "src/sema.tr"
        HirExpr* _trret287 = box_hirexpr(HirExpr_ctor_EMatchExpr(hm_subj, hm_arms, hm_ty));
        return _trret287;
    } else if (_t1600.tag == Expr_ELoop) {
        __auto_type loop_body = _t1600.data.ELoop.body;
#line 7930 "src/sema.tr"
        Sema_enter_scope(self);
#line 7931 "src/sema.tr"
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
#line 7932 "src/sema.tr"
        List_i64_append(self->loop_blk_floor, (-1LL));
#line 7933 "src/sema.tr"
        List_i64_append(self->loop_blk_scope, (-1LL));
#line 7934 "src/sema.tr"
        HirBlock* h_loop_body = Sema_lower_block(self, loop_body);
#line 7935 "src/sema.tr"
        List_i64_pop(self->loop_scope_base);
#line 7936 "src/sema.tr"
        List_i64_pop(self->loop_blk_floor);
#line 7937 "src/sema.tr"
        List_i64_pop(self->loop_blk_scope);
#line 7938 "src/sema.tr"
        Sema_finalize_scope_drops(self, h_loop_body);
#line 7939 "src/sema.tr"
        Sema_exit_scope(self);
#line 7940 "src/sema.tr"
        AstType* loop_ty = Sema_infer_break_type(self, h_loop_body);
#line 7941 "src/sema.tr"
        HirExpr* _trret288 = box_hirexpr(HirExpr_ctor_ELoop(h_loop_body, loop_ty));
        _tr_obj_release(h_loop_body, _trdrop_HirBlock);
        return _trret288;
    } else if (_t1600.tag == Expr_EWhileExpr) {
        __auto_type we_cond = _t1600.data.EWhileExpr.cond;
__auto_type we_body = _t1600.data.EWhileExpr.body;
__auto_type we_else = _t1600.data.EWhileExpr.else_body;
#line 7945 "src/sema.tr"
        HirExpr* h_we_cond = Sema_lower_expr(self, we_cond);
#line 7946 "src/sema.tr"
        Sema_enter_scope(self);
#line 7947 "src/sema.tr"
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
#line 7948 "src/sema.tr"
        List_i64_append(self->loop_blk_floor, (-1LL));
#line 7949 "src/sema.tr"
        List_i64_append(self->loop_blk_scope, (-1LL));
#line 7950 "src/sema.tr"
        HirBlock* h_we_body = Sema_lower_block(self, we_body);
#line 7951 "src/sema.tr"
        List_i64_pop(self->loop_scope_base);
#line 7952 "src/sema.tr"
        List_i64_pop(self->loop_blk_floor);
#line 7953 "src/sema.tr"
        List_i64_pop(self->loop_blk_scope);
#line 7954 "src/sema.tr"
        Sema_finalize_scope_drops(self, h_we_body);
#line 7955 "src/sema.tr"
        Sema_exit_scope(self);
#line 7957 "src/sema.tr"
        HirExpr* h_we_else = Sema_lower_do_value(self, we_else);
#line 7958 "src/sema.tr"
        AstType* we_ty = Sema_infer_break_type(self, h_we_body);
#line 7959 "src/sema.tr"
        if (_tr_str_eqv((we_ty->name), (_tr_str_lit_len("void", 4LL)))) {
#line 7959 "src/sema.tr"
            we_ty = hir_expr_type(h_we_else);
        }
#line 7961 "src/sema.tr"
        HirBlock* h_we_else_b = HirBlock_init();
#line 7962 "src/sema.tr"
        HirBlock_push(h_we_else_b, box_hirstmt(HirStmt_ctor_SExpr(h_we_else)));
#line 7963 "src/sema.tr"
        HirExpr* _trret289 = box_hirexpr(HirExpr_ctor_EWhileExpr(h_we_cond, h_we_body, h_we_else_b, we_ty));
        _tr_obj_release(h_we_body, _trdrop_HirBlock);
        _tr_obj_release(h_we_else_b, _trdrop_HirBlock);
        return _trret289;
    } else if (_t1600.tag == Expr_ETryExpr) {
        __auto_type inner = _t1600.data.ETryExpr.expr;
#line 7965 "src/sema.tr"
        if ((((!self->fn_try_ok) && (self->closure_depth == 0LL)) && (!_tr_str_eqv((self->current_func_name), (_tr_str_lit_len("", 0LL)))))) {
#line 7966 "src/sema.tr"
            ({ TrStr _at_t1810 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[F-4] '?' used in '", 19LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which neither throws nor returns a Result/Option. FIX: declare it 'def ", 74LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...) throws str -> T' (or return Result[T, E]), or handle the error with 'if r.is_err:' instead of '?'.", 104LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1810); _tr_str_release(_at_t1810); });
        }
#line 7967 "src/sema.tr"
        HirExpr* hinner = Sema_lower_expr(self, inner);
#line 7968 "src/sema.tr"
        AstType* inner_ty = hir_expr_type(hinner);
#line 7970 "src/sema.tr"
        AstType* ok_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 7971 "src/sema.tr"
        if ((_tr_str_eqv((inner_ty->name), (_tr_str_lit_len("Result", 6LL))) && (inner_ty->args->len > 0LL))) {
#line 7972 "src/sema.tr"
            ok_ty = (*((AstType**)List_ptr_get(inner_ty->args, 0LL)));
        } else if ((!_tr_str_eqv((inner_ty->name), (_tr_str_lit_len("void", 4LL))))) {
#line 7974 "src/sema.tr"
            ok_ty = inner_ty;
        }
#line 7975 "src/sema.tr"
        HirExpr* _trret290 = box_hirexpr(HirExpr_ctor_ETryExpr(hinner, ok_ty));
        return _trret290;
    } else if (_t1600.tag == Expr_EAwait) {
        __auto_type inner_await = _t1600.data.EAwait.expr;
#line 7978 "src/sema.tr"
        if ((!self->in_async_fn)) {
#line 7979 "src/sema.tr"
            ({ TrStr _at_t1811 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[C-4] 'await' used outside an async function. FIX: Declare '", 60LL)), (self->current_func_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as 'async def ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->current_func_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...)' to use await inside it.", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); Sema_error(self, _at_t1811); _tr_str_release(_at_t1811); });
        }
#line 7980 "src/sema.tr"
        HirExpr* hinner_await = Sema_lower_expr(self, inner_await);
#line 7981 "src/sema.tr"
        AstType* await_ty = hir_expr_type(hinner_await);
#line 7982 "src/sema.tr"
        HirExpr* _trret291 = box_hirexpr(HirExpr_ctor_EAwait(hinner_await, await_ty));
        return _trret291;
    } else if (_t1600.tag == Expr_EDict) {
        __auto_type keys = _t1600.data.EDict.keys;
__auto_type vals = _t1600.data.EDict.vals;
#line 7984 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("a dict/map literal", 18LL), _tr_str_lit_len("use parallel fixed-size arrays [K; N]/[V; N], or a static lookup table", 70LL));
#line 7985 "src/sema.tr"
        List_ptr* h_keys = (void*)List_ptr_new();
#line 7986 "src/sema.tr"
        List_ptr* h_vals = (void*)List_ptr_new();
#line 7987 "src/sema.tr"
        long long di = 0LL;
#line 7988 "src/sema.tr"
        while ((di < keys->len)) {
#line 7989 "src/sema.tr"
            List_ptr_append(h_keys, Sema_lower_expr(self, ((Expr*)List_ptr_get(keys, di))));
#line 7990 "src/sema.tr"
            List_ptr_append(h_vals, Sema_lower_expr(self, ((Expr*)List_ptr_get(vals, di))));
#line 7991 "src/sema.tr"
            di = (di + 1LL);
        }
#line 7995 "src/sema.tr"
        AstType* dict_ty = AstType_init(_tr_str_lit_len("Dict", 4LL));
#line 7996 "src/sema.tr"
        if ((h_keys->len > 0LL)) {
#line 7997 "src/sema.tr"
            ({ __auto_type _ord_t1812 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(h_keys, 0LL))))); List_ptr_append(dict_ty->args, _ord_t1812); });
#line 7998 "src/sema.tr"
            ({ __auto_type _ord_t1813 = (box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(h_vals, 0LL))))); List_ptr_append(dict_ty->args, _ord_t1813); });
        }
#line 7999 "src/sema.tr"
        HirExpr* _trret292 = box_hirexpr(HirExpr_ctor_EDict(h_keys, h_vals, dict_ty));
        return _trret292;
    } else if (_t1600.tag == Expr_EListComp) {
        __auto_type element = _t1600.data.EListComp.element;
__auto_type generators = _t1600.data.EListComp.generators;
#line 8001 "src/sema.tr"
        Sema_check_no_heap(self, _tr_str_lit_len("a list comprehension", 20LL), _tr_str_lit_len("loop over a fixed-size array [T; N] and write into another", 58LL));
#line 8002 "src/sema.tr"
        Sema_enter_scope(self);
#line 8003 "src/sema.tr"
        List_ptr* hgens = (void*)List_ptr_new();
#line 8004 "src/sema.tr"
        long long gc = 0LL;
#line 8005 "src/sema.tr"
        while ((gc < generators->len)) {
#line 8006 "src/sema.tr"
            Comprehension* gen_ast = (*((Comprehension**)List_ptr_get(generators, gc)));
#line 8007 "src/sema.tr"
            HirComprehension** hgen_ptr = (HirComprehension**)(0LL);
#line 8008 "src/sema.tr"
            /* unsafe block */
#line 8008 "src/sema.tr"
            hgen_ptr = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
#line 8009 "src/sema.tr"
            HirComprehension* hgen_val = ((HirComprehension*)_tr_obj_alloc(sizeof(HirComprehension)));
#line 8010 "src/sema.tr"
            hgen_val->target = _tr_str_retain(gen_ast->target);
#line 8011 "src/sema.tr"
            HirExpr* h_iter_lc = Sema_lower_expr(self, gen_ast->iter);
#line 8012 "src/sema.tr"
            hgen_val->iter = h_iter_lc;
#line 8013 "src/sema.tr"
            TrStr lc_itn = _tr_str_retain(hir_expr_type(h_iter_lc)->name);
#line 8014 "src/sema.tr"
            long long lc_ial = hir_expr_type(h_iter_lc)->args->len;
#line 8015 "src/sema.tr"
            AstType* lc_elem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 8016 "src/sema.tr"
            if (((_tr_str_eqv((lc_itn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((lc_itn), (_tr_str_lit_len("Vec", 3LL)))) && (lc_ial > 0LL))) {
#line 8017 "src/sema.tr"
                lc_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_lc)->args, 0LL)));
            } else if (_tr_str_eqv((lc_itn), (_tr_str_lit_len("str", 3LL)))) {
#line 8019 "src/sema.tr"
                lc_elem_ty = AstType_init(_tr_str_lit_len("char", 4LL));
            }
#line 8020 "src/sema.tr"
            ({ __auto_type _ord_t1814 = (gen_ast->target); __auto_type _ord_t1815 = (SymbolKind_make_SVariable()); __auto_type _ord_t1816 = (box_asttype(lc_elem_ty)); Sema_declare(self, _ord_t1814, _ord_t1815, _ord_t1816, false); });
#line 8021 "src/sema.tr"
            List_ptr* hifs = (void*)List_ptr_new();
#line 8022 "src/sema.tr"
            long long lc_fi = 0LL;
#line 8023 "src/sema.tr"
            while ((lc_fi < gen_ast->ifs->len)) {
#line 8024 "src/sema.tr"
                List_ptr_append(hifs, Sema_lower_expr(self, ((Expr*)List_ptr_get(gen_ast->ifs, lc_fi))));
#line 8025 "src/sema.tr"
                lc_fi = (lc_fi + 1LL);
            }
#line 8026 "src/sema.tr"
            hgen_val->ifs = hifs;
#line 8027 "src/sema.tr"
            hgen_val->is_async = gen_ast->is_async;
#line 8028 "src/sema.tr"
            /* unsafe block */
#line 8028 "src/sema.tr"
            (*hgen_ptr = hgen_val);
#line 8029 "src/sema.tr"
            List_ptr_append(hgens, hgen_ptr);
#line 8030 "src/sema.tr"
            gc = (gc + 1LL);
            _tr_str_release(lc_itn);
        }
#line 8031 "src/sema.tr"
        HirExpr* h_lc_elem = Sema_lower_expr(self, element);
#line 8032 "src/sema.tr"
        AstType* lc_elem_hty = hir_expr_type(h_lc_elem);
#line 8033 "src/sema.tr"
        AstType* comp_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 8034 "src/sema.tr"
        comp_ty->args = (void*)List_ptr_new();
#line 8035 "src/sema.tr"
        ({ __auto_type _ord_t1817 = (box_asttype(lc_elem_hty)); List_ptr_append(comp_ty->args, _ord_t1817); });
#line 8036 "src/sema.tr"
        Sema_exit_scope(self);
#line 8037 "src/sema.tr"
        HirExpr* _trret293 = box_hirexpr(HirExpr_ctor_EListComp(h_lc_elem, hgens, comp_ty));
        return _trret293;
    } else if (_t1600.tag == Expr_EGeneratorExpr) {
        __auto_type element = _t1600.data.EGeneratorExpr.element;
__auto_type generators = _t1600.data.EGeneratorExpr.generators;
#line 8039 "src/sema.tr"
        Sema_enter_scope(self);
#line 8040 "src/sema.tr"
        List_ptr* hgens2 = (void*)List_ptr_new();
#line 8041 "src/sema.tr"
        long long gc2 = 0LL;
#line 8042 "src/sema.tr"
        while ((gc2 < generators->len)) {
#line 8043 "src/sema.tr"
            Comprehension* gen_ast2 = (*((Comprehension**)List_ptr_get(generators, gc2)));
#line 8044 "src/sema.tr"
            HirComprehension** hgen_ptr2 = (HirComprehension**)(0LL);
#line 8045 "src/sema.tr"
            /* unsafe block */
#line 8045 "src/sema.tr"
            hgen_ptr2 = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
#line 8046 "src/sema.tr"
            HirComprehension* hgen_val2 = ((HirComprehension*)_tr_obj_alloc(sizeof(HirComprehension)));
#line 8047 "src/sema.tr"
            hgen_val2->target = _tr_str_retain(gen_ast2->target);
#line 8048 "src/sema.tr"
            HirExpr* h_iter_ge = Sema_lower_expr(self, gen_ast2->iter);
#line 8049 "src/sema.tr"
            hgen_val2->iter = h_iter_ge;
#line 8050 "src/sema.tr"
            TrStr ge_itn = _tr_str_retain(hir_expr_type(h_iter_ge)->name);
#line 8051 "src/sema.tr"
            long long ge_ial = hir_expr_type(h_iter_ge)->args->len;
#line 8052 "src/sema.tr"
            AstType* ge_elem_ty = AstType_init(_tr_str_lit_len("int", 3LL));
#line 8053 "src/sema.tr"
            if (((_tr_str_eqv((ge_itn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ge_itn), (_tr_str_lit_len("Vec", 3LL)))) && (ge_ial > 0LL))) {
#line 8054 "src/sema.tr"
                ge_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_ge)->args, 0LL)));
            } else if (_tr_str_eqv((ge_itn), (_tr_str_lit_len("str", 3LL)))) {
#line 8056 "src/sema.tr"
                ge_elem_ty = AstType_init(_tr_str_lit_len("char", 4LL));
            }
#line 8057 "src/sema.tr"
            ({ __auto_type _ord_t1818 = (gen_ast2->target); __auto_type _ord_t1819 = (SymbolKind_make_SVariable()); __auto_type _ord_t1820 = (box_asttype(ge_elem_ty)); Sema_declare(self, _ord_t1818, _ord_t1819, _ord_t1820, false); });
#line 8058 "src/sema.tr"
            List_ptr* hifs2 = (void*)List_ptr_new();
#line 8059 "src/sema.tr"
            long long ge_fi = 0LL;
#line 8060 "src/sema.tr"
            while ((ge_fi < gen_ast2->ifs->len)) {
#line 8061 "src/sema.tr"
                List_ptr_append(hifs2, Sema_lower_expr(self, ((Expr*)List_ptr_get(gen_ast2->ifs, ge_fi))));
#line 8062 "src/sema.tr"
                ge_fi = (ge_fi + 1LL);
            }
#line 8063 "src/sema.tr"
            hgen_val2->ifs = hifs2;
#line 8064 "src/sema.tr"
            hgen_val2->is_async = gen_ast2->is_async;
#line 8065 "src/sema.tr"
            /* unsafe block */
#line 8065 "src/sema.tr"
            (*hgen_ptr2 = hgen_val2);
#line 8066 "src/sema.tr"
            List_ptr_append(hgens2, hgen_ptr2);
#line 8067 "src/sema.tr"
            gc2 = (gc2 + 1LL);
            _tr_str_release(ge_itn);
        }
#line 8068 "src/sema.tr"
        HirExpr* h_ge_elem = Sema_lower_expr(self, element);
#line 8069 "src/sema.tr"
        AstType* ge_elem_hty = hir_expr_type(h_ge_elem);
#line 8070 "src/sema.tr"
        AstType* gen_ty = AstType_init(_tr_str_lit_len("List", 4LL));
#line 8071 "src/sema.tr"
        gen_ty->args = (void*)List_ptr_new();
#line 8072 "src/sema.tr"
        ({ __auto_type _ord_t1821 = (box_asttype(ge_elem_hty)); List_ptr_append(gen_ty->args, _ord_t1821); });
#line 8073 "src/sema.tr"
        Sema_exit_scope(self);
#line 8074 "src/sema.tr"
        HirExpr* _trret294 = box_hirexpr(HirExpr_ctor_EGeneratorExpr(h_ge_elem, hgens2, gen_ty));
        return _trret294;
    } else if (_t1600.tag == Expr_ESuperMethodCall) {
        __auto_type base_class = _t1600.data.ESuperMethodCall.base_class;
__auto_type method = _t1600.data.ESuperMethodCall.method;
__auto_type args = _t1600.data.ESuperMethodCall.args;
#line 8076 "src/sema.tr"
        List_ptr* h_super_args = (void*)List_ptr_new();
#line 8077 "src/sema.tr"
        long long k_smc = 0LL;
#line 8078 "src/sema.tr"
        while ((k_smc < args->len)) {
#line 8079 "src/sema.tr"
            List_ptr_append(h_super_args, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k_smc))));
#line 8080 "src/sema.tr"
            k_smc = (k_smc + 1LL);
        }
#line 8082 "src/sema.tr"
        TrStr resolved_base = _tr_str_retain(base_class);
#line 8083 "src/sema.tr"
        if ((_tr_str_eqv((resolved_base), (_tr_str_lit_len("", 0LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))))) {
#line 8084 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
#line 8085 "src/sema.tr"
                ClassDef* cur_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
#line 8086 "src/sema.tr"
                if ((cur_cls->base_classes->len > 0LL)) {
#line 8087 "src/sema.tr"
                    TrStr _strtmp_t1822 = List_TrStr_get(cur_cls->base_classes, 0LL);
                    _tr_str_release(resolved_base);
                    resolved_base = _strtmp_t1822;
                }
            }
        }
#line 8088 "src/sema.tr"
        AstType* super_ret_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 8089 "src/sema.tr"
        if (_tr_dict_contains(self->classes, _tr_strz(resolved_base))) {
#line 8090 "src/sema.tr"
            ClassDef* bc_def = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(resolved_base)));
#line 8091 "src/sema.tr"
            long long k_smc2 = 0LL;
#line 8092 "src/sema.tr"
            while ((k_smc2 < bc_def->methods->len)) {
#line 8093 "src/sema.tr"
                if (_tr_str_eqv((((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->name), (method))) {
#line 8094 "src/sema.tr"
                    if ((((unsigned long long)(((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 8095 "src/sema.tr"
                        super_ret_ty = (*((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->ret_ty);
                    }
                }
#line 8096 "src/sema.tr"
                k_smc2 = (k_smc2 + 1LL);
            }
        }
#line 8097 "src/sema.tr"
        HirExpr* _trret295 = box_hirexpr(HirExpr_ctor_ESuperMethodCall(resolved_base, method, h_super_args, super_ret_ty));
        _tr_str_release(resolved_base);
        return _trret295;
    } else if (_t1600.tag == Expr_ESuperPropAccess) {
        __auto_type base_class = _t1600.data.ESuperPropAccess.base_class;
__auto_type prop = _t1600.data.ESuperPropAccess.prop;
#line 8099 "src/sema.tr"
        TrStr resolved_base2 = _tr_str_retain(base_class);
#line 8100 "src/sema.tr"
        if ((_tr_str_eqv((resolved_base2), (_tr_str_lit_len("", 0LL))) && (!_tr_str_eqv((self->current_class_name), (_tr_str_lit_len("", 0LL)))))) {
#line 8101 "src/sema.tr"
            if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
#line 8102 "src/sema.tr"
                ClassDef* cur_cls2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
#line 8103 "src/sema.tr"
                if ((cur_cls2->base_classes->len > 0LL)) {
#line 8104 "src/sema.tr"
                    TrStr _strtmp_t1823 = List_TrStr_get(cur_cls2->base_classes, 0LL);
                    _tr_str_release(resolved_base2);
                    resolved_base2 = _strtmp_t1823;
                }
            }
        }
#line 8105 "src/sema.tr"
        AstType* super_field_ty = AstType_init(_tr_str_lit_len("void", 4LL));
#line 8106 "src/sema.tr"
        if (_tr_dict_contains(self->classes, _tr_strz(resolved_base2))) {
#line 8107 "src/sema.tr"
            ClassDef* bc_def2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(resolved_base2)));
#line 8108 "src/sema.tr"
            long long k_spa = 0LL;
#line 8109 "src/sema.tr"
            while ((k_spa < bc_def2->fields->len)) {
#line 8110 "src/sema.tr"
                if (_tr_str_eqv((((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->name), (prop))) {
#line 8111 "src/sema.tr"
                    if ((((unsigned long long)(((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->ty)) != ((unsigned long long)(0LL)))) {
#line 8112 "src/sema.tr"
                        super_field_ty = (*((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->ty);
                    }
                }
#line 8113 "src/sema.tr"
                k_spa = (k_spa + 1LL);
            }
        }
#line 8114 "src/sema.tr"
        HirExpr* _trret296 = box_hirexpr(HirExpr_ctor_ESuperPropAccess(resolved_base2, prop, super_field_ty));
        _tr_str_release(resolved_base2);
        return _trret296;
    } else if (1) {
        __auto_type _ = _t1600;
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init(_tr_str_lit_len("None", 4LL))));
    }
}

__attribute__((hot)) TrStr Sema_is_reserved_error(Sema* self, TrStr name) {
#line 8119 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("true", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("True", 4LL))))) {
#line 8119 "src/sema.tr"
        return _tr_str_lit_len("built-in boolean constant 'true'", 32LL);
    }
#line 8120 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("false", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("False", 5LL))))) {
#line 8120 "src/sema.tr"
        return _tr_str_lit_len("built-in boolean constant 'false'", 33LL);
    }
#line 8121 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("none", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("None", 4LL))))) {
#line 8121 "src/sema.tr"
        return _tr_str_lit_len("built-in null constant 'none'", 29LL);
    }
#line 8122 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("Some", 4LL)))) {
#line 8122 "src/sema.tr"
        return _tr_str_lit_len("built-in Option constructor 'Some'", 34LL);
    }
#line 8123 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("Ok", 2LL)))) {
#line 8123 "src/sema.tr"
        return _tr_str_lit_len("built-in Result constructor 'Ok'", 32LL);
    }
#line 8124 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("Err", 3LL)))) {
#line 8124 "src/sema.tr"
        return _tr_str_lit_len("built-in Result constructor 'Err'", 33LL);
    }
#line 8125 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Sema_is_reserved_keyword(Sema* self, TrStr name) {
#line 8132 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("print", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("input", 5LL))))) {
#line 8132 "src/sema.tr"
        return _tr_str_lit_len("built-in I/O function", 21LL);
    }
#line 8133 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("range", 5LL)))) {
#line 8133 "src/sema.tr"
        return _tr_str_lit_len("built-in range function", 23LL);
    }
#line 8134 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("len", 3LL)))) {
#line 8134 "src/sema.tr"
        return _tr_str_lit_len("built-in len function", 21LL);
    }
#line 8135 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("type", 4LL)))) {
#line 8135 "src/sema.tr"
        return _tr_str_lit_len("built-in type function", 22LL);
    }
#line 8136 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("min", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("sum", 3LL))))) {
#line 8136 "src/sema.tr"
        return _tr_str_lit_len("built-in math function", 22LL);
    }
#line 8137 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("int", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL))))) {
#line 8137 "src/sema.tr"
        return _tr_str_lit_len("built-in primitive type", 23LL);
    }
#line 8138 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Dict", 4LL))))) {
#line 8138 "src/sema.tr"
        return _tr_str_lit_len("built-in container type", 23LL);
    }
#line 8139 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("Option", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Result", 6LL))))) {
#line 8139 "src/sema.tr"
        return _tr_str_lit_len("built-in enum type", 18LL);
    }
#line 8140 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("Exception", 9LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Error", 5LL))))) {
#line 8140 "src/sema.tr"
        return _tr_str_lit_len("built-in exception type", 23LL);
    }
#line 8143 "src/sema.tr"
    if ((((((((((_tr_str_eqv((name), (_tr_str_lit_len("_Complex", 8LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_Imaginary", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Bool", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Atomic", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Generic", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Noreturn", 9LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Alignas", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Alignof", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Static_assert", 14LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_Thread_local", 13LL))))) {
#line 8143 "src/sema.tr"
        return _tr_str_lit_len("C99/C11 keyword", 15LL);
    }
#line 8144 "src/sema.tr"
    if ((((((((_tr_str_eqv((name), (_tr_str_lit_len("DIR", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("FILE", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("EOF", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("NULL", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("errno", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("stdin", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("stdout", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("stderr", 6LL))))) {
#line 8144 "src/sema.tr"
        return _tr_str_lit_len("C library identifier", 20LL);
    }
#line 8145 "src/sema.tr"
    if ((((((((((((((_tr_str_eqv((name), (_tr_str_lit_len("ERROR", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("DELETE", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("IN", 2LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("OUT", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("OPTIONAL", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("NEAR", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("FAR", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("VOID", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("CONST", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("TRUE", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("FALSE", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("INFINITE", 8LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("SEVERITY_ERROR", 14LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("SEVERITY_SUCCESS", 16LL))))) {
#line 8145 "src/sema.tr"
        return _tr_str_lit_len("Windows SDK macro", 17LL);
    }
#line 8146 "src/sema.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool Sema_block_returns(Sema* self, Block* b) {
#line 8150 "src/sema.tr"
    if ((b->stmts->len == 0LL)) {
#line 8150 "src/sema.tr"
        return false;
    }
#line 8153 "src/sema.tr"
    long long last_idx = (b->stmts->len - 1LL);
#line 8154 "src/sema.tr"
    bool sl_going = true;
#line 8155 "src/sema.tr"
    while ((last_idx > 0LL)) {
#line 8156 "src/sema.tr"
        if ((!sl_going)) {
#line 8156 "src/sema.tr"
            break;
        }
#line 8157 "src/sema.tr"
        Stmt* sl_s = ((Stmt*)List_ptr_get(b->stmts, last_idx));
#line 8158 "src/sema.tr"
        if ((((unsigned long long)(sl_s)) == ((unsigned long long)(0LL)))) {
#line 8159 "src/sema.tr"
            last_idx = (last_idx - 1LL);
        } else {
#line 8161 "src/sema.tr"
            bool is_sline = false;
#line 8162 "src/sema.tr"
            __auto_type _t1824 = (*sl_s);
            if (_t1824.tag == Stmt_SLine) {
                __auto_type _ = _t1824.data.SLine.n;
                is_sline = true;
            } else if (1) {
                __auto_type _ = _t1824;
                /* pass */
            }
#line 8165 "src/sema.tr"
            if (is_sline) {
#line 8166 "src/sema.tr"
                last_idx = (last_idx - 1LL);
            } else {
#line 8168 "src/sema.tr"
                sl_going = false;
            }
        }
    }
#line 8169 "src/sema.tr"
    Stmt* last_s = ((Stmt*)List_ptr_get(b->stmts, last_idx));
#line 8170 "src/sema.tr"
    if ((((unsigned long long)(last_s)) == ((unsigned long long)(0LL)))) {
#line 8170 "src/sema.tr"
        return false;
    }
#line 8171 "src/sema.tr"
    __auto_type _t1825 = (*last_s);
    if (_t1825.tag == Stmt_SReturn) {
        __auto_type _ = _t1825.data.SReturn.val;
        return true;
    } else if (_t1825.tag == Stmt_SRaise) {
        __auto_type _ = _t1825.data.SRaise.val;
        return true;
    } else if (_t1825.tag == Stmt_SUnsafe) {
        __auto_type body = _t1825.data.SUnsafe.body;
        bool _trret297 = Sema_block_returns(self, body);
        return _trret297;
    } else if (_t1825.tag == Stmt_SIf) {
        __auto_type cond = _t1825.data.SIf.cond;
__auto_type then_b = _t1825.data.SIf.then_b;
__auto_type elifs = _t1825.data.SIf.elifs;
__auto_type else_b = _t1825.data.SIf.else_b;
#line 8176 "src/sema.tr"
        if ((else_b->stmts->len == 0LL)) {
#line 8176 "src/sema.tr"
            return false;
        }
#line 8177 "src/sema.tr"
        bool _trret298 = (Sema_block_returns(self, then_b) && Sema_block_returns(self, else_b));
        return _trret298;
    } else if (_t1825.tag == Stmt_SMatch) {
        __auto_type subj = _t1825.data.SMatch.expr;
__auto_type arms = _t1825.data.SMatch.arms;
#line 8184 "src/sema.tr"
        bool has_wild = false;
#line 8185 "src/sema.tr"
        bool all_ret = true;
#line 8186 "src/sema.tr"
        List_TrStr* _fr_tnames = (void*)List_TrStr_new();
#line 8187 "src/sema.tr"
        List_TrStr* _fr_vnames = (void*)List_TrStr_new();
#line 8188 "src/sema.tr"
        long long mi = 0LL;
#line 8189 "src/sema.tr"
        while ((mi < arms->len)) {
#line 8190 "src/sema.tr"
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, mi));
#line 8191 "src/sema.tr"
            __auto_type _t1826 = arm->pat;
            if (_t1826.tag == Pattern_PWild) {
                has_wild = true;
            } else if (_t1826.tag == Pattern_PBind) {
                __auto_type _ = _t1826.data.PBind.name;
                has_wild = true;
            } else if (1) {
                __auto_type _ = _t1826;
                Sema_pattern_covers(self, arm->pat, _fr_tnames, _fr_vnames);
            }
#line 8195 "src/sema.tr"
            if ((((unsigned long long)(arm->body)) != ((unsigned long long)(0LL)))) {
#line 8196 "src/sema.tr"
                if ((!Sema_block_returns(self, (*arm->body)))) {
#line 8197 "src/sema.tr"
                    all_ret = false;
                }
            }
#line 8198 "src/sema.tr"
            mi = (mi + 1LL);
        }
#line 8199 "src/sema.tr"
        if (has_wild) {
#line 8199 "src/sema.tr"
            bool _trret299 = all_ret;
            return _trret299;
        }
#line 8200 "src/sema.tr"
        if ((!all_ret)) {
#line 8200 "src/sema.tr"
            return false;
        }
#line 8201 "src/sema.tr"
        if ((_fr_vnames->len == 0LL)) {
#line 8201 "src/sema.tr"
            return false;
        }
#line 8202 "src/sema.tr"
        TrStr _fr_ety = List_TrStr_get(_fr_tnames, 0LL);
#line 8203 "src/sema.tr"
        bool _fr_same_ty = true;
#line 8204 "src/sema.tr"
        long long _fr_ti = 1LL;
#line 8205 "src/sema.tr"
        while ((_fr_ti < _fr_tnames->len)) {
#line 8206 "src/sema.tr"
            if (({ TrStr _cmpa_t1827 = (List_TrStr_get(_fr_tnames, _fr_ti)); TrStr _cmpb_t1828 = (_fr_ety); _Bool _cmpr_t1829 = !_tr_str_eqv(_cmpa_t1827, _cmpb_t1828); _tr_str_release(_cmpa_t1827); _cmpr_t1829; })) {
#line 8206 "src/sema.tr"
                _fr_same_ty = false;
            }
#line 8207 "src/sema.tr"
            _fr_ti = (_fr_ti + 1LL);
        }
#line 8208 "src/sema.tr"
        if ((!_fr_same_ty)) {
#line 8208 "src/sema.tr"
            _tr_str_release(_fr_ety);
            return false;
        }
#line 8209 "src/sema.tr"
        if ((!_tr_dict_contains(self->enums, _tr_strz(_fr_ety)))) {
#line 8209 "src/sema.tr"
            _tr_str_release(_fr_ety);
            return false;
        }
#line 8210 "src/sema.tr"
        EnumDef* _fr_edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(_fr_ety)));
#line 8211 "src/sema.tr"
        long long _fr_ei = 0LL;
#line 8212 "src/sema.tr"
        while ((_fr_ei < _fr_edef->variants->len)) {
#line 8213 "src/sema.tr"
            TrStr _fr_vn = _tr_str_retain(((VariantDef*)List_ptr_get(_fr_edef->variants, _fr_ei))->name);
#line 8214 "src/sema.tr"
            bool _fr_found = false;
#line 8215 "src/sema.tr"
            long long _fr_vi = 0LL;
#line 8216 "src/sema.tr"
            while ((_fr_vi < _fr_vnames->len)) {
#line 8217 "src/sema.tr"
                if (({ TrStr _cmpa_t1830 = (List_TrStr_get(_fr_vnames, _fr_vi)); TrStr _cmpb_t1831 = (_fr_vn); _Bool _cmpr_t1832 = _tr_str_eqv(_cmpa_t1830, _cmpb_t1831); _tr_str_release(_cmpa_t1830); _cmpr_t1832; })) {
#line 8217 "src/sema.tr"
                    _fr_found = true;
                }
#line 8218 "src/sema.tr"
                _fr_vi = (_fr_vi + 1LL);
            }
#line 8219 "src/sema.tr"
            if ((!_fr_found)) {
#line 8219 "src/sema.tr"
                _tr_str_release(_fr_ety);
                _tr_str_release(_fr_vn);
                return false;
            }
#line 8220 "src/sema.tr"
            _fr_ei = (_fr_ei + 1LL);
            _tr_str_release(_fr_vn);
        }
#line 8221 "src/sema.tr"
        _tr_str_release(_fr_ety);
        return true;
    } else if (1) {
        __auto_type _ = _t1825;
        return false;
    }
}

__attribute__((hot)) void Sema_pattern_covers(Sema* self, Pattern p, List_TrStr* tnames, List_TrStr* vnames) {
#line 8230 "src/sema.tr"
    __auto_type _t1833 = p;
    if (_t1833.tag == Pattern_PVariant) {
        __auto_type tn = _t1833.data.PVariant.type_name;
__auto_type vn = _t1833.data.PVariant.variant;
#line 8232 "src/sema.tr"
        List_TrStr_append(tnames, tn);
#line 8233 "src/sema.tr"
        List_TrStr_append(vnames, vn);
    } else if (_t1833.tag == Pattern_PVariantBind) {
        __auto_type tn = _t1833.data.PVariantBind.type_name;
__auto_type vn = _t1833.data.PVariantBind.variant;
#line 8235 "src/sema.tr"
        List_TrStr_append(tnames, tn);
#line 8236 "src/sema.tr"
        List_TrStr_append(vnames, vn);
    } else if (_t1833.tag == Pattern_PVariantBindMany) {
        __auto_type tn = _t1833.data.PVariantBindMany.type_name;
__auto_type vn = _t1833.data.PVariantBindMany.variant;
#line 8238 "src/sema.tr"
        List_TrStr_append(tnames, tn);
#line 8239 "src/sema.tr"
        List_TrStr_append(vnames, vn);
    } else if (_t1833.tag == Pattern_POr) {
        __auto_type pats = _t1833.data.POr.patterns;
#line 8241 "src/sema.tr"
        long long oi = 0LL;
#line 8242 "src/sema.tr"
        while ((oi < pats->len)) {
#line 8243 "src/sema.tr"
            Sema_pattern_covers(self, List_Pattern_get(pats, oi), tnames, vnames);
#line 8244 "src/sema.tr"
            oi = (oi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1833;
        /* pass */
    }
}

__attribute__((hot)) bool Sema_is_primitive(Sema* self, AstType* ty) {
#line 8247 "src/sema.tr"
    return Sema_is_primitive_name(self, ty->name);
}

__attribute__((hot)) bool Sema__is_known_type_name(Sema* self, TrStr name) {
#line 8254 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) {
#line 8254 "src/sema.tr"
        return true;
    }
#line 8255 "src/sema.tr"
    if (Sema_is_primitive_name(self, name)) {
#line 8255 "src/sema.tr"
        return true;
    }
#line 8256 "src/sema.tr"
    if ((((((_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Str", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("StringBuilder", 13LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("ptr", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("object", 6LL))))) {
#line 8256 "src/sema.tr"
        return true;
    }
#line 8257 "src/sema.tr"
    if ((_tr_strlen(_tr_strz(name)) == 1LL)) {
#line 8257 "src/sema.tr"
        return true;
    }
#line 8258 "src/sema.tr"
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
#line 8258 "src/sema.tr"
        return true;
    }
#line 8259 "src/sema.tr"
    if (_tr_dict_contains(self->enums, _tr_strz(name))) {
#line 8259 "src/sema.tr"
        return true;
    }
#line 8260 "src/sema.tr"
    if (((((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))) && _tr_dict_contains(self->interfaces, _tr_strz(name)))) {
#line 8260 "src/sema.tr"
        return true;
    }
#line 8261 "src/sema.tr"
    if (((((((((_tr_str_eqv((name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Set", 3LL)))) || (_tr_str_eqv((name), (_tr_str_lit_len("Box", 3LL))) && (!_tr_dict_contains(self->classes, _tr_strz(_tr_str_lit_len("Box", 3LL)))))) || _tr_str_eqv((name), (_tr_str_lit_len("Tuple", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("tuple", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 8261 "src/sema.tr"
        return true;
    }
#line 8262 "src/sema.tr"
    if (((((((((_tr_str_eqv((name), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Result", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Channel", 7LL))))) {
#line 8262 "src/sema.tr"
        return true;
    }
#line 8263 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Coro", 4LL))))) {
#line 8263 "src/sema.tr"
        return true;
    }
#line 8264 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("AsyncPool", 9LL))) || _tr_str_eqv((name), (_tr_str_lit_len("AsyncTask", 9LL))))) {
#line 8264 "src/sema.tr"
        return true;
    }
#line 8270 "src/sema.tr"
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("OS", 2LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Process", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Env", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Hash", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("File", 4LL))))) {
#line 8270 "src/sema.tr"
        return true;
    }
#line 8271 "src/sema.tr"
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("Math", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Clock", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Time", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Random", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Hmac", 4LL))))) {
#line 8271 "src/sema.tr"
        return true;
    }
#line 8272 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("Json", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("JsonDoc", 7LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("JsonWriter", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("JsonReader", 10LL))))) {
#line 8272 "src/sema.tr"
        return true;
    }
#line 8273 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool Sema_is_primitive_name(Sema* self, TrStr name) {
#line 8276 "src/sema.tr"
    if ((((((_tr_str_eqv((name), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("None", 4LL))))) {
#line 8276 "src/sema.tr"
        return true;
    }
#line 8277 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("i64", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("i8", 2LL))))) {
#line 8277 "src/sema.tr"
        return true;
    }
#line 8278 "src/sema.tr"
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("u64", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("usize", 5LL))))) {
#line 8278 "src/sema.tr"
        return true;
    }
#line 8279 "src/sema.tr"
    if (((_tr_str_eqv((name), (_tr_str_lit_len("f64", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("lambda", 6LL))))) {
#line 8279 "src/sema.tr"
        return true;
    }
#line 8282 "src/sema.tr"
    if (((_tr_str_starts_withv((name), (_tr_str_lit_len("c_", 2LL))) && (!_tr_str_eqv((name), (_tr_str_lit_len("c_void_ptr", 10LL))))) && (!_tr_str_eqv((name), (_tr_str_lit_len("c_FILE", 6LL)))))) {
#line 8282 "src/sema.tr"
        return true;
    }
#line 8286 "src/sema.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Str", 3LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("StringObj", 9LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Bytes", 5LL))))) {
#line 8286 "src/sema.tr"
        return true;
    }
#line 8287 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((name), (_tr_str_lit_len("Channel", 7LL))))) {
#line 8287 "src/sema.tr"
        return true;
    }
#line 8288 "src/sema.tr"
    if (((_tr_str_eqv((name), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((name), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("Atomic", 6LL))))) {
#line 8288 "src/sema.tr"
        return true;
    }
#line 8289 "src/sema.tr"
    if (((_tr_str_eqv((name), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
#line 8289 "src/sema.tr"
        return true;
    }
#line 8290 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("AsyncPool", 9LL))) || _tr_str_eqv((name), (_tr_str_lit_len("AsyncTask", 9LL))))) {
#line 8290 "src/sema.tr"
        return true;
    }
#line 8291 "src/sema.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 8291 "src/sema.tr"
        return true;
    }
#line 8293 "src/sema.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("ref", 3LL))) || _tr_str_eqv((name), (_tr_str_lit_len("mut_ref", 7LL))))) {
#line 8293 "src/sema.tr"
        return true;
    }
#line 8294 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool _is_builtin_type_word(TrStr n) {
#line 21 "src/sema.tr"
    if (((_tr_str_lenv((n)) > 2LL) && ({ TrStr _cmpa_t1834 = (_tr_str_slicev((n), 0LL, 2LL)); TrStr _cmpb_t1835 = (_tr_str_lit_len("c_", 2LL)); _Bool _cmpr_t1836 = _tr_str_eqv(_cmpa_t1834, _cmpb_t1835); _tr_str_release(_cmpa_t1834); _cmpr_t1836; }))) {
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
#line 95 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 95 "src/sema.tr"
        return false;
    }
#line 96 "src/sema.tr"
    __auto_type _t1837 = (*e);
    if (_t1837.tag == Expr_EPropAccess) {
        __auto_type obj = _t1837.data.EPropAccess.obj;
#line 98 "src/sema.tr"
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
#line 98 "src/sema.tr"
            return false;
        }
#line 99 "src/sema.tr"
        __auto_type _t1838 = (*obj);
        if (_t1838.tag == Expr_EIdent) {
            __auto_type nm = _t1838.data.EIdent.name;
            bool _trret164 = _tr_str_eqv((nm), (_tr_str_lit_len("self", 4LL)));
            return _trret164;
        } else if (1) {
            __auto_type _ = _t1838;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t1837;
        return false;
    }
}

__attribute__((hot)) bool _binop_is_float_name(TrStr n) {
#line 105 "src/sema.tr"
    return ((_tr_str_eqv((n), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL))));
}

__attribute__((hot)) bool _hl_has_float(List_ptr* hl) {
#line 109 "src/sema.tr"
    long long i = 0LL;
#line 110 "src/sema.tr"
    while ((i < hl->len)) {
#line 111 "src/sema.tr"
        if (_binop_is_float_name(hir_expr_type(((HirExpr*)List_ptr_get(hl, i)))->name)) {
#line 111 "src/sema.tr"
            return true;
        }
#line 112 "src/sema.tr"
        i = (i + 1LL);
    }
#line 113 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool _block_mutates_self(Block* b) {
#line 116 "src/sema.tr"
    long long i = 0LL;
#line 117 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 118 "src/sema.tr"
        if (_stmt_mutates_self(((Stmt*)List_ptr_get(b->stmts, i)))) {
#line 118 "src/sema.tr"
            return true;
        }
#line 119 "src/sema.tr"
        i = (i + 1LL);
    }
#line 120 "src/sema.tr"
    return false;
}

__attribute__((hot)) bool _pblock_mutates_self(Block** pb) {
#line 123 "src/sema.tr"
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
#line 123 "src/sema.tr"
        return false;
    }
#line 124 "src/sema.tr"
    return _block_mutates_self((*pb));
}

__attribute__((hot)) bool _stmt_mutates_self(Stmt* s) {
#line 127 "src/sema.tr"
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
#line 127 "src/sema.tr"
        return false;
    }
#line 128 "src/sema.tr"
    __auto_type _t1839 = (*s);
    if (_t1839.tag == Stmt_SAssign) {
        __auto_type tgt = _t1839.data.SAssign.target;
        return _expr_is_self_field(tgt);
    } else if (_t1839.tag == Stmt_SIf) {
        __auto_type then_b = _t1839.data.SIf.then_b;
__auto_type elifs = _t1839.data.SIf.elifs;
__auto_type else_b = _t1839.data.SIf.else_b;
#line 131 "src/sema.tr"
        if (_block_mutates_self(then_b)) {
#line 131 "src/sema.tr"
            return true;
        }
#line 132 "src/sema.tr"
        long long i = 0LL;
#line 133 "src/sema.tr"
        while ((i < elifs->len)) {
#line 134 "src/sema.tr"
            if (_pblock_mutates_self(((ElifClause*)List_ptr_get(elifs, i))->body)) {
#line 134 "src/sema.tr"
                return true;
            }
#line 135 "src/sema.tr"
            i = (i + 1LL);
        }
#line 136 "src/sema.tr"
        bool _trret165 = _block_mutates_self(else_b);
        return _trret165;
    } else if (_t1839.tag == Stmt_SWhile) {
        __auto_type body = _t1839.data.SWhile.body;
        bool _trret166 = _block_mutates_self(body);
        return _trret166;
    } else if (_t1839.tag == Stmt_SFor) {
        __auto_type body = _t1839.data.SFor.body;
        bool _trret167 = _block_mutates_self(body);
        return _trret167;
    } else if (_t1839.tag == Stmt_SForUnpack) {
        __auto_type body = _t1839.data.SForUnpack.body;
        bool _trret168 = _block_mutates_self(body);
        return _trret168;
    } else if (_t1839.tag == Stmt_SUnsafe) {
        __auto_type body = _t1839.data.SUnsafe.body;
        bool _trret169 = _block_mutates_self(body);
        return _trret169;
    } else if (_t1839.tag == Stmt_SWith) {
        __auto_type body = _t1839.data.SWith.body;
        bool _trret170 = _block_mutates_self(body);
        return _trret170;
    } else if (_t1839.tag == Stmt_STaskGroup) {
        __auto_type body = _t1839.data.STaskGroup.body;
        bool _trret171 = _block_mutates_self(body);
        return _trret171;
    } else if (_t1839.tag == Stmt_SGpuBlock) {
        __auto_type body = _t1839.data.SGpuBlock.body;
        bool _trret172 = _block_mutates_self(body);
        return _trret172;
    } else if (_t1839.tag == Stmt_SMatch) {
        __auto_type arms = _t1839.data.SMatch.arms;
#line 145 "src/sema.tr"
        long long i = 0LL;
#line 146 "src/sema.tr"
        while ((i < arms->len)) {
#line 147 "src/sema.tr"
            if (_pblock_mutates_self(((MatchArm*)List_ptr_get(arms, i))->body)) {
#line 147 "src/sema.tr"
                return true;
            }
#line 148 "src/sema.tr"
            i = (i + 1LL);
        }
#line 149 "src/sema.tr"
        return false;
    } else if (_t1839.tag == Stmt_STry) {
        __auto_type try_body = _t1839.data.STry.try_body;
__auto_type catches = _t1839.data.STry.catches;
__auto_type finally_b = _t1839.data.STry.finally_b;
#line 151 "src/sema.tr"
        if (_block_mutates_self(try_body)) {
#line 151 "src/sema.tr"
            return true;
        }
#line 152 "src/sema.tr"
        long long i = 0LL;
#line 153 "src/sema.tr"
        while ((i < catches->len)) {
#line 154 "src/sema.tr"
            if (_pblock_mutates_self((*((CatchClause**)List_ptr_get(catches, i)))->body)) {
#line 154 "src/sema.tr"
                return true;
            }
#line 155 "src/sema.tr"
            i = (i + 1LL);
        }
#line 156 "src/sema.tr"
        bool _trret173 = _block_mutates_self(finally_b);
        return _trret173;
    } else if (_t1839.tag == Stmt_SDefer) {
        __auto_type inner = _t1839.data.SDefer.stmt;
        return _stmt_mutates_self(inner);
    } else if (1) {
        __auto_type _ = _t1839;
        return false;
    }
}

__attribute__((hot)) bool _rhs_is_alias(Expr* e) {
#line 167 "src/sema.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 167 "src/sema.tr"
        return false;
    }
#line 168 "src/sema.tr"
    __auto_type _t1840 = (*e);
    if (_t1840.tag == Expr_EIdent) {
        __auto_type _ = _t1840.data.EIdent.name;
        return true;
    } else if (_t1840.tag == Expr_EPropAccess) {
        return true;
    } else if (_t1840.tag == Expr_EIndex) {
        return true;
    } else if (_t1840.tag == Expr_EMethodCall) {
        __auto_type m = _t1840.data.EMethodCall.method;
        bool _trret174 = ((_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((m), (_tr_str_lit_len("first", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("last", 4LL))));
        return _trret174;
    } else if (1) {
        __auto_type _ = _t1840;
        return false;
    }
}

__attribute__((hot)) void _block_alias_assigns(Block* b, TrMap* out) {
#line 176 "src/sema.tr"
    long long i = 0LL;
#line 177 "src/sema.tr"
    while ((i < b->stmts->len)) {
#line 178 "src/sema.tr"
        _stmt_alias_assigns(((Stmt*)List_ptr_get(b->stmts, i)), out);
#line 179 "src/sema.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void _pblock_alias_assigns(Block** pb, TrMap* out) {
#line 182 "src/sema.tr"
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
#line 182 "src/sema.tr"
        return;
    }
#line 183 "src/sema.tr"
    _block_alias_assigns((*pb), out);
}

__attribute__((hot)) void _stmt_alias_assigns(Stmt* s, TrMap* out) {
#line 186 "src/sema.tr"
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
#line 186 "src/sema.tr"
        return;
    }
#line 187 "src/sema.tr"
    __auto_type _t1841 = (*s);
    if (_t1841.tag == Stmt_SAssign) {
        __auto_type tgt = _t1841.data.SAssign.target;
__auto_type val = _t1841.data.SAssign.val;
#line 189 "src/sema.tr"
        if ((((unsigned long long)(tgt)) != ((unsigned long long)(0LL)))) {
#line 190 "src/sema.tr"
            __auto_type _t1842 = (*tgt);
            if (_t1842.tag == Expr_EIdent) {
                __auto_type tn = _t1842.data.EIdent.name;
#line 192 "src/sema.tr"
                if (_rhs_is_alias(val)) {
#line 192 "src/sema.tr"
                    _tr_dict_set(out, _tr_strz(tn), true);
                }
            } else if (1) {
                __auto_type _ = _t1842;
                /* pass */
            }
        }
    } else if (_t1841.tag == Stmt_SIf) {
        __auto_type then_b = _t1841.data.SIf.then_b;
__auto_type elifs = _t1841.data.SIf.elifs;
__auto_type else_b = _t1841.data.SIf.else_b;
#line 195 "src/sema.tr"
        _block_alias_assigns(then_b, out);
#line 196 "src/sema.tr"
        long long i = 0LL;
#line 197 "src/sema.tr"
        while ((i < elifs->len)) {
#line 198 "src/sema.tr"
            _pblock_alias_assigns(((ElifClause*)List_ptr_get(elifs, i))->body, out);
#line 199 "src/sema.tr"
            i = (i + 1LL);
        }
#line 200 "src/sema.tr"
        _block_alias_assigns(else_b, out);
    } else if (_t1841.tag == Stmt_SWhile) {
        __auto_type body = _t1841.data.SWhile.body;
        _block_alias_assigns(body, out);
    } else if (_t1841.tag == Stmt_SFor) {
        __auto_type body = _t1841.data.SFor.body;
        _block_alias_assigns(body, out);
    } else if (_t1841.tag == Stmt_SForUnpack) {
        __auto_type body = _t1841.data.SForUnpack.body;
        _block_alias_assigns(body, out);
    } else if (_t1841.tag == Stmt_SUnsafe) {
        __auto_type body = _t1841.data.SUnsafe.body;
        _block_alias_assigns(body, out);
    } else if (_t1841.tag == Stmt_SWith) {
        __auto_type body = _t1841.data.SWith.body;
        _block_alias_assigns(body, out);
    } else if (_t1841.tag == Stmt_STaskGroup) {
        __auto_type body = _t1841.data.STaskGroup.body;
        _block_alias_assigns(body, out);
    } else if (_t1841.tag == Stmt_SGpuBlock) {
        __auto_type body = _t1841.data.SGpuBlock.body;
        _block_alias_assigns(body, out);
    } else if (_t1841.tag == Stmt_SMatch) {
        __auto_type arms = _t1841.data.SMatch.arms;
#line 209 "src/sema.tr"
        long long i = 0LL;
#line 210 "src/sema.tr"
        while ((i < arms->len)) {
#line 211 "src/sema.tr"
            _pblock_alias_assigns(((MatchArm*)List_ptr_get(arms, i))->body, out);
#line 212 "src/sema.tr"
            i = (i + 1LL);
        }
    } else if (_t1841.tag == Stmt_STry) {
        __auto_type try_body = _t1841.data.STry.try_body;
__auto_type catches = _t1841.data.STry.catches;
__auto_type finally_b = _t1841.data.STry.finally_b;
#line 214 "src/sema.tr"
        _block_alias_assigns(try_body, out);
#line 215 "src/sema.tr"
        long long i = 0LL;
#line 216 "src/sema.tr"
        while ((i < catches->len)) {
#line 217 "src/sema.tr"
            _pblock_alias_assigns((*((CatchClause**)List_ptr_get(catches, i)))->body, out);
#line 218 "src/sema.tr"
            i = (i + 1LL);
        }
#line 219 "src/sema.tr"
        _block_alias_assigns(finally_b, out);
    } else if (_t1841.tag == Stmt_SDefer) {
        __auto_type inner = _t1841.data.SDefer.stmt;
        _stmt_alias_assigns(inner, out);
    } else if (1) {
        __auto_type _ = _t1841;
        /* pass */
    }
}

__attribute__((hot)) Stmt* box_astestmt(Stmt t) {
#line 8306 "src/sema.tr"
    /* unsafe block */
#line 8307 "src/sema.tr"
    Stmt* p = ((Stmt*)_tr_c_calloc((size_t)(1LL), sizeof(Stmt)));
#line 8308 "src/sema.tr"
    (*p = t);
#line 8309 "src/sema.tr"
    return p;
}

__attribute__((hot)) Symbol** box_symbol(Symbol* s) {
#line 8318 "src/sema.tr"
    /* unsafe block */
#line 8319 "src/sema.tr"
    Symbol** p = ((Symbol**)_tr_c_calloc((size_t)(1LL), sizeof(Symbol*)));
#line 8320 "src/sema.tr"
    (*p = s);
#line 8321 "src/sema.tr"
    return p;
}

__attribute__((hot)) bool _str_method_known(TrStr m) {
#line 8326 "src/sema.tr"
    return _tr_str_containsv((_tr_str_lit_len(" as_ptr as_str capitalize ceil center char_at chars contains contains_char count ends_with eq find floor format free get_index index_of is_alnum is_alpha is_digit is_empty is_lower is_space is_upper join last_index_of rfind __len__ len length lines lower lpad offset pad_left pad_right parse_bool parse_float parse_int pow read remove_char repeat replace replace_first reverse rpad slice split split_exact split_once split_to_vec starts_with strip strip_prefix strip_suffix title to_float to_int to_lower to_str to_string to_upper trim trim_left trim_right upper words write zfill clone hash bytes ", 598LL)), (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len(" ", 1LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })));
}

