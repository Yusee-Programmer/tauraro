#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) AstType* AstType_init(char* name) {
    /* pass */
    AstType* t = ((AstType*)_tr_checked_alloc(sizeof(AstType)));
    /* pass */
    t->name = name;
    /* pass */
    t->args = (void*)List_ptr_new();
    /* pass */
    t->from_param = "";
    /* pass */
    return t;
}

__attribute__((hot)) AstType* AstType_init_generic(char* name, AstType** arg) {
    /* pass */
    AstType* t = ((AstType*)_tr_checked_alloc(sizeof(AstType)));
    /* pass */
    t->name = name;
    /* pass */
    t->args = (void*)List_ptr_new();
    /* pass */
    List_ptr_append(t->args, arg);
    /* pass */
    t->from_param = "";
    /* pass */
    return t;
}

__attribute__((malloc,returns_nonnull,hot)) GenericConstraint* GenericConstraint_init(char* target) {
    /* pass */
    GenericConstraint* g = ((GenericConstraint*)_tr_checked_alloc(sizeof(GenericConstraint)));
    /* pass */
    g->target = target;
    /* pass */
    g->bounds = (void*)List_ptr_new();
    /* pass */
    return g;
}

__attribute__((malloc,returns_nonnull,hot)) Decorator* Decorator_init(char* name) {
    /* pass */
    Decorator* d = ((Decorator*)_tr_checked_alloc(sizeof(Decorator)));
    /* pass */
    d->name = name;
    /* pass */
    d->args = (void*)List_ptr_new();
    /* pass */
    return d;
}

__attribute__((malloc,returns_nonnull,hot)) Comprehension* Comprehension_init(char* target, Expr* iter) {
    /* pass */
    Comprehension* c = ((Comprehension*)_tr_checked_alloc(sizeof(Comprehension)));
    /* pass */
    c->target = target;
    /* pass */
    c->iter = iter;
    /* pass */
    c->ifs = (void*)List_ptr_new();
    /* pass */
    c->is_async = false;
    /* pass */
    return c;
}

__attribute__((malloc,returns_nonnull,hot)) CatchClause* CatchClause_init(char* err_name, Block** body) {
    /* pass */
    CatchClause* c = ((CatchClause*)_tr_checked_alloc(sizeof(CatchClause)));
    /* pass */
    c->err_name = err_name;
    /* pass */
    c->err_type = (AstType**)(0LL);
    /* pass */
    c->body = body;
    /* pass */
    return c;
}

__attribute__((malloc,returns_nonnull,hot)) MatchArm* MatchArm_init(Pattern pat, Block** body) {
    /* pass */
    MatchArm* a = ((MatchArm*)_tr_checked_alloc(sizeof(MatchArm)));
    /* pass */
    a->pat = pat;
    /* pass */
    a->guard = (Expr*)(0LL);
    /* pass */
    a->body = body;
    /* pass */
    return a;
}

__attribute__((hot)) FStringPart* FStringPart_init_text(char* s) {
    /* pass */
    FStringPart* p = ((FStringPart*)_tr_checked_alloc(sizeof(FStringPart)));
    /* pass */
    p->is_expr = false;
    /* pass */
    p->text = s;
    /* pass */
    p->expr = (Expr*)(0LL);
    /* pass */
    p->fmt_spec = "";
    /* pass */
    return p;
}

__attribute__((hot)) FStringPart* FStringPart_init_expr(Expr* e) {
    /* pass */
    FStringPart* p = ((FStringPart*)_tr_checked_alloc(sizeof(FStringPart)));
    /* pass */
    p->is_expr = true;
    /* pass */
    p->text = "";
    /* pass */
    p->expr = e;
    /* pass */
    p->fmt_spec = "";
    /* pass */
    return p;
}

__attribute__((hot)) FStringPart* FStringPart_init_expr_fmt(Expr* e, char* spec) {
    /* pass */
    FStringPart* p = ((FStringPart*)_tr_checked_alloc(sizeof(FStringPart)));
    /* pass */
    p->is_expr = true;
    /* pass */
    p->text = "";
    /* pass */
    p->expr = e;
    /* pass */
    p->fmt_spec = spec;
    /* pass */
    return p;
}

__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_recv(Expr* chan, char* var, Block* body) {
    /* pass */
    ChanSelectArm* a = ((ChanSelectArm*)_tr_checked_alloc(sizeof(ChanSelectArm)));
    /* pass */
    a->kind = 0LL;
    /* pass */
    a->chan_expr = chan;
    /* pass */
    a->val_expr = (Expr*)(0LL);
    /* pass */
    a->var_name = var;
    /* pass */
    a->timeout_ms = (Expr*)(0LL);
    /* pass */
    a->body = body;
    /* pass */
    return a;
}

__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_send(Expr* chan, Expr* val, Block* body) {
    /* pass */
    ChanSelectArm* a = ((ChanSelectArm*)_tr_checked_alloc(sizeof(ChanSelectArm)));
    /* pass */
    a->kind = 1LL;
    /* pass */
    a->chan_expr = chan;
    /* pass */
    a->val_expr = val;
    /* pass */
    a->var_name = "";
    /* pass */
    a->timeout_ms = (Expr*)(0LL);
    /* pass */
    a->body = body;
    /* pass */
    return a;
}

__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_timeout(Expr* ms, Block* body) {
    /* pass */
    ChanSelectArm* a = ((ChanSelectArm*)_tr_checked_alloc(sizeof(ChanSelectArm)));
    /* pass */
    a->kind = 2LL;
    /* pass */
    a->chan_expr = (Expr*)(0LL);
    /* pass */
    a->val_expr = (Expr*)(0LL);
    /* pass */
    a->var_name = "";
    /* pass */
    a->timeout_ms = ms;
    /* pass */
    a->body = body;
    /* pass */
    return a;
}

__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_default(Block* body) {
    /* pass */
    ChanSelectArm* a = ((ChanSelectArm*)_tr_checked_alloc(sizeof(ChanSelectArm)));
    /* pass */
    a->kind = 3LL;
    /* pass */
    a->chan_expr = (Expr*)(0LL);
    /* pass */
    a->val_expr = (Expr*)(0LL);
    /* pass */
    a->var_name = "";
    /* pass */
    a->timeout_ms = (Expr*)(0LL);
    /* pass */
    a->body = body;
    /* pass */
    return a;
}

__attribute__((malloc,returns_nonnull,hot)) Block* Block_init() {
    /* pass */
    Block* b = ((Block*)_tr_checked_alloc(sizeof(Block)));
    /* pass */
    b->stmts = (void*)List_ptr_new();
    /* pass */
    return b;
}

__attribute__((hot)) void Block_push(Block* self, Stmt* s) {
    /* pass */
    List_ptr_append(self->stmts, s);
}

__attribute__((hot)) long long Block_len(Block* self) {
    /* pass */
    return self->stmts->len;
}

__attribute__((hot)) Stmt* Block_get(Block* self, long long i) {
    /* pass */
    return ((Stmt*)List_ptr_get(self->stmts, i));
}

__attribute__((malloc,returns_nonnull,hot)) ElifClause* ElifClause_init(Expr* cond, Block** body) {
    /* pass */
    ElifClause* c = ((ElifClause*)_tr_checked_alloc(sizeof(ElifClause)));
    /* pass */
    c->cond = cond;
    /* pass */
    c->body = body;
    /* pass */
    return c;
}

__attribute__((malloc,returns_nonnull,hot)) Param* Param_init(char* name, AstType** ty) {
    /* pass */
    Param* p = ((Param*)_tr_checked_alloc(sizeof(Param)));
    /* pass */
    p->name = name;
    /* pass */
    p->ty = ty;
    /* pass */
    p->is_ref = false;
    /* pass */
    p->is_mut_ref = false;
    /* pass */
    p->is_variadic = false;
    /* pass */
    return p;
}

__attribute__((malloc,returns_nonnull,hot)) FunctionDef* FunctionDef_init(char* name) {
    /* pass */
    FunctionDef* f = ((FunctionDef*)_tr_checked_alloc(sizeof(FunctionDef)));
    /* pass */
    f->name = name;
    /* pass */
    f->generics = (void*)List_str_new();
    /* pass */
    f->params = (void*)List_ptr_new();
    /* pass */
    f->ret_ty = (AstType**)(0LL);
    /* pass */
    f->throws_ty = (AstType**)(0LL);
    /* pass */
    f->decorators = (void*)List_ptr_new();
    /* pass */
    f->constraints = (void*)List_ptr_new();
    /* pass */
    f->is_variadic = false;
    /* pass */
    f->is_async = false;
    /* pass */
    f->is_extern = false;
    /* pass */
    f->is_public = false;
    /* pass */
    f->body = Block_init();
    /* pass */
    f->line = 0LL;
    /* pass */
    return f;
}

__attribute__((malloc,returns_nonnull,hot)) FieldDef* FieldDef_init(char* name, AstType** ty) {
    /* pass */
    FieldDef* fd = ((FieldDef*)_tr_checked_alloc(sizeof(FieldDef)));
    /* pass */
    fd->name = name;
    /* pass */
    fd->ty = ty;
    /* pass */
    fd->default_val = (Expr*)(0LL);
    /* pass */
    return fd;
}

__attribute__((malloc,returns_nonnull,hot)) ClassDef* ClassDef_init(char* name) {
    /* pass */
    ClassDef* c = ((ClassDef*)_tr_checked_alloc(sizeof(ClassDef)));
    /* pass */
    c->name = name;
    /* pass */
    c->generics = (void*)List_str_new();
    /* pass */
    c->base_classes = (void*)List_str_new();
    /* pass */
    c->iface_names = (void*)List_str_new();
    /* pass */
    c->fields = (void*)List_ptr_new();
    /* pass */
    c->methods = (void*)List_ptr_new();
    /* pass */
    c->decorators = (void*)List_ptr_new();
    /* pass */
    c->constraints = (void*)List_ptr_new();
    /* pass */
    c->is_public = false;
    /* pass */
    c->is_class = true;
    /* pass */
    c->line = 0LL;
    /* pass */
    return c;
}

__attribute__((malloc,returns_nonnull,hot)) VariantDef* VariantDef_init(char* name) {
    /* pass */
    VariantDef* v = ((VariantDef*)_tr_checked_alloc(sizeof(VariantDef)));
    /* pass */
    v->name = name;
    /* pass */
    v->fields = (void*)List_ptr_new();
    /* pass */
    return v;
}

__attribute__((malloc,returns_nonnull,hot)) EnumDef* EnumDef_init(char* name) {
    /* pass */
    EnumDef* e = ((EnumDef*)_tr_checked_alloc(sizeof(EnumDef)));
    /* pass */
    e->name = name;
    /* pass */
    e->generics = (void*)List_str_new();
    /* pass */
    e->iface_names = (void*)List_str_new();
    /* pass */
    e->variants = (void*)List_ptr_new();
    /* pass */
    e->methods = (void*)List_ptr_new();
    /* pass */
    e->decorators = (void*)List_ptr_new();
    /* pass */
    e->is_public = false;
    /* pass */
    e->line = 0LL;
    /* pass */
    return e;
}

__attribute__((malloc,returns_nonnull,hot)) InterfaceDef* InterfaceDef_init(char* name) {
    /* pass */
    InterfaceDef* i = ((InterfaceDef*)_tr_checked_alloc(sizeof(InterfaceDef)));
    /* pass */
    i->name = name;
    /* pass */
    i->generics = (void*)List_str_new();
    /* pass */
    i->methods = (void*)List_ptr_new();
    /* pass */
    i->is_public = false;
    /* pass */
    i->line = 0LL;
    /* pass */
    i->decorators = (void*)List_ptr_new();
    /* pass */
    return i;
}

__attribute__((malloc,returns_nonnull,hot)) ImportItem* ImportItem_init(char* name) {
    /* pass */
    ImportItem* it = ((ImportItem*)_tr_checked_alloc(sizeof(ImportItem)));
    /* pass */
    it->name = name;
    /* pass */
    it->alias = "";
    /* pass */
    return it;
}

__attribute__((malloc,returns_nonnull,hot)) Program* Program_init() {
    /* pass */
    Program* p = ((Program*)_tr_checked_alloc(sizeof(Program)));
    /* pass */
    p->decls = (void*)List_ptr_new();
    /* pass */
    return p;
}

__attribute__((hot)) void Program_push(Program* self, Decl* d) {
    /* pass */
    List_ptr_append(self->decls, d);
}

__attribute__((hot)) long long Program_len(Program* self) {
    /* pass */
    return self->decls->len;
}

__attribute__((hot)) Decl* Program_get(Program* self, long long i) {
    /* pass */
    return ((Decl*)List_ptr_get(self->decls, i));
}

__attribute__((hot)) void _dummy_instantiations() {
    /* pass */
    List_ptr* v1 = (void*)List_ptr_new();
    /* pass */
    List_ptr* v2 = (void*)List_ptr_new();
    /* pass */
    List_Pattern* v3 = (void*)List_Pattern_new();
    /* pass */
    List_ptr* v4 = (void*)List_ptr_new();
    /* pass */
    List_ptr* v5 = (void*)List_ptr_new();
    /* pass */
    List_ptr* v6 = (void*)List_ptr_new();
    /* pass */
    List_ptr* v7 = (void*)List_ptr_new();
    /* pass */
    List_ptr* v8 = (void*)List_ptr_new();
    /* pass */
    List_ptr* v9 = (void*)List_ptr_new();
    /* pass */
    List_ptr* v10 = (void*)List_ptr_new();
    /* pass */
    List_ptr* v11 = (void*)List_ptr_new();
    /* pass */
    List_ptr* v12 = (void*)List_ptr_new();
}

