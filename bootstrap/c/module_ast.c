#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) AstType* AstType_init(TrStr name) {
#line 34 "src/ast.tr"
    AstType* t = ((AstType*)_tr_obj_alloc(sizeof(AstType)));
#line 35 "src/ast.tr"
    t->name = _tr_str_retain(name);
#line 36 "src/ast.tr"
    t->args = (void*)List_ptr_new();
#line 37 "src/ast.tr"
    t->from_param = _tr_str_lit_len("", 0LL);
#line 38 "src/ast.tr"
    t->from_regions = (void*)List_TrStr_new();
#line 39 "src/ast.tr"
    t->from_index = (-1LL);
#line 40 "src/ast.tr"
    t->is_borrow = false;
#line 41 "src/ast.tr"
    t->is_mut_borrow = false;
#line 42 "src/ast.tr"
    t->array_size = 0LL;
#line 43 "src/ast.tr"
    t->is_throws = false;
#line 44 "src/ast.tr"
    return t;
}

__attribute__((hot)) AstType* AstType_init_generic(TrStr name, AstType** arg) {
#line 47 "src/ast.tr"
    AstType* t = ((AstType*)_tr_obj_alloc(sizeof(AstType)));
#line 48 "src/ast.tr"
    t->name = _tr_str_retain(name);
#line 49 "src/ast.tr"
    t->args = (void*)List_ptr_new();
#line 50 "src/ast.tr"
    List_ptr_append(t->args, arg);
#line 51 "src/ast.tr"
    t->from_param = _tr_str_lit_len("", 0LL);
#line 52 "src/ast.tr"
    t->from_regions = (void*)List_TrStr_new();
#line 53 "src/ast.tr"
    t->from_index = (-1LL);
#line 54 "src/ast.tr"
    t->is_borrow = false;
#line 55 "src/ast.tr"
    t->is_mut_borrow = false;
#line 56 "src/ast.tr"
    return t;
}

__attribute__((malloc,returns_nonnull,hot)) GenericConstraint* GenericConstraint_init(TrStr target) {
#line 64 "src/ast.tr"
    GenericConstraint* g = ((GenericConstraint*)_tr_obj_alloc(sizeof(GenericConstraint)));
#line 65 "src/ast.tr"
    g->target = _tr_str_retain(target);
#line 66 "src/ast.tr"
    g->bounds = (void*)List_ptr_new();
#line 67 "src/ast.tr"
    return g;
}

__attribute__((malloc,returns_nonnull,hot)) Decorator* Decorator_init(TrStr name) {
#line 77 "src/ast.tr"
    Decorator* d = ((Decorator*)_tr_obj_alloc(sizeof(Decorator)));
#line 78 "src/ast.tr"
    d->name = _tr_str_retain(name);
#line 79 "src/ast.tr"
    d->args = (void*)List_ptr_new();
#line 80 "src/ast.tr"
    return d;
}

__attribute__((malloc,returns_nonnull,hot)) Comprehension* Comprehension_init(TrStr target, Expr* iter) {
#line 108 "src/ast.tr"
    Comprehension* c = ((Comprehension*)_tr_obj_alloc(sizeof(Comprehension)));
#line 109 "src/ast.tr"
    c->target = _tr_str_retain(target);
#line 110 "src/ast.tr"
    c->iter = iter;
#line 111 "src/ast.tr"
    c->ifs = (void*)List_ptr_new();
#line 112 "src/ast.tr"
    c->is_async = false;
#line 113 "src/ast.tr"
    return c;
}

__attribute__((malloc,returns_nonnull,hot)) CatchClause* CatchClause_init(TrStr err_name, Block** body) {
#line 122 "src/ast.tr"
    CatchClause* c = ((CatchClause*)_tr_obj_alloc(sizeof(CatchClause)));
#line 123 "src/ast.tr"
    c->err_name = _tr_str_retain(err_name);
#line 124 "src/ast.tr"
    c->err_type = (AstType**)(0LL);
#line 125 "src/ast.tr"
    c->body = body;
#line 126 "src/ast.tr"
    return c;
}

__attribute__((malloc,returns_nonnull,hot)) MatchArm* MatchArm_init(Pattern pat, Block** body) {
#line 136 "src/ast.tr"
    MatchArm* a = ((MatchArm*)_tr_obj_alloc(sizeof(MatchArm)));
#line 137 "src/ast.tr"
    a->pat = pat;
#line 138 "src/ast.tr"
    a->guard = (Expr*)(0LL);
#line 139 "src/ast.tr"
    a->body = body;
#line 140 "src/ast.tr"
    return a;
}

__attribute__((hot)) FStringPart* FStringPart_init_text(TrStr s, long long blen) {
#line 154 "src/ast.tr"
    FStringPart* p = ((FStringPart*)_tr_obj_alloc(sizeof(FStringPart)));
#line 155 "src/ast.tr"
    p->is_expr = false;
#line 156 "src/ast.tr"
    p->text = _tr_str_retain(s);
#line 157 "src/ast.tr"
    p->text_len = blen;
#line 158 "src/ast.tr"
    p->expr = (Expr*)(0LL);
#line 159 "src/ast.tr"
    p->fmt_spec = _tr_str_lit_len("", 0LL);
#line 160 "src/ast.tr"
    p->spec_args = (void*)List_ptr_new();
#line 161 "src/ast.tr"
    return p;
}

__attribute__((hot)) FStringPart* FStringPart_init_expr(Expr* e) {
#line 164 "src/ast.tr"
    FStringPart* p = ((FStringPart*)_tr_obj_alloc(sizeof(FStringPart)));
#line 165 "src/ast.tr"
    p->is_expr = true;
#line 166 "src/ast.tr"
    p->text = _tr_str_lit_len("", 0LL);
#line 167 "src/ast.tr"
    p->text_len = 0LL;
#line 168 "src/ast.tr"
    p->expr = e;
#line 169 "src/ast.tr"
    p->fmt_spec = _tr_str_lit_len("", 0LL);
#line 170 "src/ast.tr"
    p->spec_args = (void*)List_ptr_new();
#line 171 "src/ast.tr"
    return p;
}

__attribute__((hot)) FStringPart* FStringPart_init_expr_fmt(Expr* e, TrStr spec) {
#line 174 "src/ast.tr"
    FStringPart* p = ((FStringPart*)_tr_obj_alloc(sizeof(FStringPart)));
#line 175 "src/ast.tr"
    p->is_expr = true;
#line 176 "src/ast.tr"
    p->text = _tr_str_lit_len("", 0LL);
#line 177 "src/ast.tr"
    p->text_len = 0LL;
#line 178 "src/ast.tr"
    p->expr = e;
#line 179 "src/ast.tr"
    p->fmt_spec = _tr_str_retain(spec);
#line 180 "src/ast.tr"
    p->spec_args = (void*)List_ptr_new();
#line 181 "src/ast.tr"
    return p;
}

__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_recv(Expr* chan, TrStr var, Block* body) {
#line 253 "src/ast.tr"
    ChanSelectArm* a = ((ChanSelectArm*)_tr_obj_alloc(sizeof(ChanSelectArm)));
#line 254 "src/ast.tr"
    a->kind = 0LL;
#line 254 "src/ast.tr"
    a->chan_expr = chan;
#line 254 "src/ast.tr"
    a->val_expr = (Expr*)(0LL);
#line 255 "src/ast.tr"
    a->var_name = _tr_str_retain(var);
#line 255 "src/ast.tr"
    a->timeout_ms = (Expr*)(0LL);
#line 255 "src/ast.tr"
    a->body = body;
#line 256 "src/ast.tr"
    return a;
}

__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_send(Expr* chan, Expr* val, Block* body) {
#line 258 "src/ast.tr"
    ChanSelectArm* a = ((ChanSelectArm*)_tr_obj_alloc(sizeof(ChanSelectArm)));
#line 259 "src/ast.tr"
    a->kind = 1LL;
#line 259 "src/ast.tr"
    a->chan_expr = chan;
#line 259 "src/ast.tr"
    a->val_expr = val;
#line 260 "src/ast.tr"
    a->var_name = _tr_str_lit_len("", 0LL);
#line 260 "src/ast.tr"
    a->timeout_ms = (Expr*)(0LL);
#line 260 "src/ast.tr"
    a->body = body;
#line 261 "src/ast.tr"
    return a;
}

__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_timeout(Expr* ms, Block* body) {
#line 263 "src/ast.tr"
    ChanSelectArm* a = ((ChanSelectArm*)_tr_obj_alloc(sizeof(ChanSelectArm)));
#line 264 "src/ast.tr"
    a->kind = 2LL;
#line 264 "src/ast.tr"
    a->chan_expr = (Expr*)(0LL);
#line 264 "src/ast.tr"
    a->val_expr = (Expr*)(0LL);
#line 265 "src/ast.tr"
    a->var_name = _tr_str_lit_len("", 0LL);
#line 265 "src/ast.tr"
    a->timeout_ms = ms;
#line 265 "src/ast.tr"
    a->body = body;
#line 266 "src/ast.tr"
    return a;
}

__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_default(Block* body) {
#line 268 "src/ast.tr"
    ChanSelectArm* a = ((ChanSelectArm*)_tr_obj_alloc(sizeof(ChanSelectArm)));
#line 269 "src/ast.tr"
    a->kind = 3LL;
#line 269 "src/ast.tr"
    a->chan_expr = (Expr*)(0LL);
#line 269 "src/ast.tr"
    a->val_expr = (Expr*)(0LL);
#line 270 "src/ast.tr"
    a->var_name = _tr_str_lit_len("", 0LL);
#line 270 "src/ast.tr"
    a->timeout_ms = (Expr*)(0LL);
#line 270 "src/ast.tr"
    a->body = body;
#line 271 "src/ast.tr"
    return a;
}

__attribute__((malloc,returns_nonnull,hot)) Block* Block_init() {
#line 310 "src/ast.tr"
    Block* b = ((Block*)_tr_obj_alloc(sizeof(Block)));
#line 311 "src/ast.tr"
    b->stmts = (void*)List_ptr_new();
#line 312 "src/ast.tr"
    return b;
}

__attribute__((hot)) void Block_push(Block* self, Stmt* s) {
#line 315 "src/ast.tr"
    List_ptr_append(self->stmts, s);
}

__attribute__((hot)) long long Block_len(Block* self) {
#line 318 "src/ast.tr"
    return self->stmts->len;
}

__attribute__((hot)) Stmt* Block_get(Block* self, long long i) {
#line 321 "src/ast.tr"
    return ((Stmt*)List_ptr_get(self->stmts, i));
}

__attribute__((malloc,returns_nonnull,hot)) ElifClause* ElifClause_init(Expr* cond, Block** body) {
#line 329 "src/ast.tr"
    ElifClause* c = ((ElifClause*)_tr_obj_alloc(sizeof(ElifClause)));
#line 330 "src/ast.tr"
    c->cond = cond;
#line 331 "src/ast.tr"
    c->body = body;
#line 332 "src/ast.tr"
    return c;
}

__attribute__((malloc,returns_nonnull,hot)) Param* Param_init(TrStr name, AstType** ty) {
#line 345 "src/ast.tr"
    Param* p = ((Param*)_tr_obj_alloc(sizeof(Param)));
#line 346 "src/ast.tr"
    p->name = _tr_str_retain(name);
#line 347 "src/ast.tr"
    p->ty = ty;
#line 348 "src/ast.tr"
    p->is_ref = false;
#line 349 "src/ast.tr"
    p->is_mut_ref = false;
#line 350 "src/ast.tr"
    p->is_variadic = false;
#line 351 "src/ast.tr"
    return p;
}

__attribute__((malloc,returns_nonnull,hot)) FunctionDef* FunctionDef_init(TrStr name) {
#line 378 "src/ast.tr"
    FunctionDef* f = ((FunctionDef*)_tr_obj_alloc(sizeof(FunctionDef)));
#line 379 "src/ast.tr"
    f->name = _tr_str_retain(name);
#line 380 "src/ast.tr"
    f->generics = (void*)List_TrStr_new();
#line 381 "src/ast.tr"
    f->params = (void*)List_ptr_new();
#line 382 "src/ast.tr"
    f->ret_ty = (AstType**)(0LL);
#line 383 "src/ast.tr"
    f->throws_ty = (AstType**)(0LL);
#line 384 "src/ast.tr"
    f->decorators = (void*)List_ptr_new();
#line 385 "src/ast.tr"
    f->constraints = (void*)List_ptr_new();
#line 386 "src/ast.tr"
    f->is_variadic = false;
#line 387 "src/ast.tr"
    f->is_async = false;
#line 388 "src/ast.tr"
    f->is_extern = false;
#line 389 "src/ast.tr"
    f->is_public = false;
#line 390 "src/ast.tr"
    f->is_export = false;
#line 391 "src/ast.tr"
    f->is_macro = false;
#line 392 "src/ast.tr"
    f->is_lib = false;
#line 393 "src/ast.tr"
    f->body = Block_init();
#line 394 "src/ast.tr"
    f->line = 0LL;
#line 395 "src/ast.tr"
    f->outlives_a = (void*)List_TrStr_new();
#line 396 "src/ast.tr"
    f->outlives_b = (void*)List_TrStr_new();
#line 397 "src/ast.tr"
    return f;
}

__attribute__((malloc,returns_nonnull,hot)) FieldDef* FieldDef_init(TrStr name, AstType** ty) {
#line 406 "src/ast.tr"
    FieldDef* fd = ((FieldDef*)_tr_obj_alloc(sizeof(FieldDef)));
#line 407 "src/ast.tr"
    fd->name = _tr_str_retain(name);
#line 408 "src/ast.tr"
    fd->ty = ty;
#line 409 "src/ast.tr"
    fd->default_val = (Expr*)(0LL);
#line 410 "src/ast.tr"
    return fd;
}

__attribute__((malloc,returns_nonnull,hot)) ClassDef* ClassDef_init(TrStr name) {
#line 430 "src/ast.tr"
    ClassDef* c = ((ClassDef*)_tr_obj_alloc(sizeof(ClassDef)));
#line 431 "src/ast.tr"
    c->name = _tr_str_retain(name);
#line 432 "src/ast.tr"
    c->generics = (void*)List_TrStr_new();
#line 433 "src/ast.tr"
    c->base_classes = (void*)List_TrStr_new();
#line 434 "src/ast.tr"
    c->iface_names = (void*)List_TrStr_new();
#line 435 "src/ast.tr"
    c->iface_targs = (void*)List_ptr_new();
#line 436 "src/ast.tr"
    c->fields = (void*)List_ptr_new();
#line 437 "src/ast.tr"
    c->methods = (void*)List_ptr_new();
#line 438 "src/ast.tr"
    c->decorators = (void*)List_ptr_new();
#line 439 "src/ast.tr"
    c->constraints = (void*)List_ptr_new();
#line 440 "src/ast.tr"
    c->is_public = false;
#line 441 "src/ast.tr"
    c->is_class = true;
#line 442 "src/ast.tr"
    c->line = 0LL;
#line 443 "src/ast.tr"
    c->docstring = _tr_str_lit_len("", 0LL);
#line 444 "src/ast.tr"
    c->region_params = (void*)List_TrStr_new();
#line 445 "src/ast.tr"
    return c;
}

__attribute__((malloc,returns_nonnull,hot)) VariantDef* VariantDef_init(TrStr name) {
#line 453 "src/ast.tr"
    VariantDef* v = ((VariantDef*)_tr_obj_alloc(sizeof(VariantDef)));
#line 454 "src/ast.tr"
    v->name = _tr_str_retain(name);
#line 455 "src/ast.tr"
    v->fields = (void*)List_ptr_new();
#line 456 "src/ast.tr"
    return v;
}

__attribute__((malloc,returns_nonnull,hot)) EnumDef* EnumDef_init(TrStr name) {
#line 472 "src/ast.tr"
    EnumDef* e = ((EnumDef*)_tr_obj_alloc(sizeof(EnumDef)));
#line 473 "src/ast.tr"
    e->name = _tr_str_retain(name);
#line 474 "src/ast.tr"
    e->generics = (void*)List_TrStr_new();
#line 475 "src/ast.tr"
    e->iface_names = (void*)List_TrStr_new();
#line 476 "src/ast.tr"
    e->variants = (void*)List_ptr_new();
#line 477 "src/ast.tr"
    e->methods = (void*)List_ptr_new();
#line 478 "src/ast.tr"
    e->decorators = (void*)List_ptr_new();
#line 479 "src/ast.tr"
    e->is_public = false;
#line 480 "src/ast.tr"
    e->line = 0LL;
#line 481 "src/ast.tr"
    e->region_params = (void*)List_TrStr_new();
#line 482 "src/ast.tr"
    return e;
}

__attribute__((malloc,returns_nonnull,hot)) InterfaceDef* InterfaceDef_init(TrStr name) {
#line 496 "src/ast.tr"
    InterfaceDef* i = ((InterfaceDef*)_tr_obj_alloc(sizeof(InterfaceDef)));
#line 497 "src/ast.tr"
    i->name = _tr_str_retain(name);
#line 498 "src/ast.tr"
    i->generics = (void*)List_TrStr_new();
#line 499 "src/ast.tr"
    i->methods = (void*)List_ptr_new();
#line 500 "src/ast.tr"
    i->is_public = false;
#line 501 "src/ast.tr"
    i->line = 0LL;
#line 502 "src/ast.tr"
    i->decorators = (void*)List_ptr_new();
#line 503 "src/ast.tr"
    i->region_params = (void*)List_TrStr_new();
#line 504 "src/ast.tr"
    return i;
}

__attribute__((malloc,returns_nonnull,hot)) ImportItem* ImportItem_init(TrStr name) {
#line 512 "src/ast.tr"
    ImportItem* it = ((ImportItem*)_tr_obj_alloc(sizeof(ImportItem)));
#line 513 "src/ast.tr"
    it->name = _tr_str_retain(name);
#line 514 "src/ast.tr"
    it->alias = _tr_str_lit_len("", 0LL);
#line 515 "src/ast.tr"
    return it;
}

__attribute__((malloc,returns_nonnull,hot)) Program* Program_init() {
#line 536 "src/ast.tr"
    Program* p = ((Program*)_tr_obj_alloc(sizeof(Program)));
#line 537 "src/ast.tr"
    p->decls = (void*)List_ptr_new();
#line 538 "src/ast.tr"
    return p;
}

__attribute__((hot)) void Program_push(Program* self, Decl* d) {
#line 541 "src/ast.tr"
    List_ptr_append(self->decls, d);
}

__attribute__((hot)) long long Program_len(Program* self) {
#line 544 "src/ast.tr"
    return self->decls->len;
}

__attribute__((hot)) Decl* Program_get(Program* self, long long i) {
#line 547 "src/ast.tr"
    return ((Decl*)List_ptr_get(self->decls, i));
}

__attribute__((hot)) TrStr unify_generic(AstType* pty, AstType* aty, TrStr gp) {
#line 555 "src/ast.tr"
    if (_tr_str_eqv((pty->name), (gp))) {
#line 556 "src/ast.tr"
        if (((_tr_str_eqv((aty->name), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((aty->name), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((aty->name), (gp)))) {
#line 556 "src/ast.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 557 "src/ast.tr"
        return _tr_str_retain(aty->name);
    }
#line 558 "src/ast.tr"
    if (((pty->args->len == 0LL) || (!_tr_str_eqv((pty->name), (aty->name))))) {
#line 558 "src/ast.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 559 "src/ast.tr"
    long long i = 0LL;
#line 560 "src/ast.tr"
    while (((i < pty->args->len) && (i < aty->args->len))) {
#line 561 "src/ast.tr"
        TrStr r = ({ __auto_type _ord_t32 = ((*((AstType**)List_ptr_get(pty->args, i)))); __auto_type _wr = (unify_generic(_ord_t32, (*((AstType**)List_ptr_get(aty->args, i))), gp)); _wr; });
#line 562 "src/ast.tr"
        if ((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL))))) {
#line 562 "src/ast.tr"
            TrStr _trret13 = _tr_str_retain(r);
            _tr_str_release(r);
            return _trret13;
        }
#line 563 "src/ast.tr"
        i = (i + 1LL);
        _tr_str_release(r);
    }
#line 564 "src/ast.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool type_mentions(AstType* ty, TrStr gp) {
#line 568 "src/ast.tr"
    if (_tr_str_eqv((ty->name), (gp))) {
#line 568 "src/ast.tr"
        return true;
    }
#line 569 "src/ast.tr"
    long long i = 0LL;
#line 570 "src/ast.tr"
    while ((i < ty->args->len)) {
#line 571 "src/ast.tr"
        if (type_mentions((*((AstType**)List_ptr_get(ty->args, i))), gp)) {
#line 571 "src/ast.tr"
            return true;
        }
#line 572 "src/ast.tr"
        i = (i + 1LL);
    }
#line 573 "src/ast.tr"
    return false;
}

__attribute__((hot)) void _dummy_instantiations() {
#line 576 "src/ast.tr"
    List_ptr* v1 = (void*)List_ptr_new();
#line 577 "src/ast.tr"
    List_ptr* v2 = (void*)List_ptr_new();
#line 578 "src/ast.tr"
    List_Pattern* v3 = (void*)List_Pattern_new();
#line 579 "src/ast.tr"
    List_ptr* v4 = (void*)List_ptr_new();
#line 580 "src/ast.tr"
    List_ptr* v5 = (void*)List_ptr_new();
#line 581 "src/ast.tr"
    List_ptr* v6 = (void*)List_ptr_new();
#line 582 "src/ast.tr"
    List_ptr* v7 = (void*)List_ptr_new();
#line 583 "src/ast.tr"
    List_ptr* v8 = (void*)List_ptr_new();
#line 584 "src/ast.tr"
    List_ptr* v9 = (void*)List_ptr_new();
#line 585 "src/ast.tr"
    List_ptr* v10 = (void*)List_ptr_new();
#line 586 "src/ast.tr"
    List_ptr* v11 = (void*)List_ptr_new();
#line 587 "src/ast.tr"
    List_ptr* v12 = (void*)List_ptr_new();
    List_ptr_free_obj(v4, _trdrop_ImportItem);
    List_ptr_free_obj(v5, _trdrop_Decorator);
    List_ptr_free_obj(v6, _trdrop_Param);
    List_ptr_free_obj(v7, _trdrop_VariantDef);
    List_ptr_free_obj(v8, _trdrop_FunctionDef);
    List_ptr_free_obj(v9, _trdrop_FieldDef);
}

