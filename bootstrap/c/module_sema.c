#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) Symbol* Symbol_init(char* name, SymbolKind kind, AstType** ty) {
    /* pass */
    Symbol* s = ((Symbol*)_tr_checked_alloc(sizeof(Symbol)));
    /* pass */
    s->name = name;
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
    s->borrowed_by = (void*)List_str_new();
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
    return s;
}

__attribute__((malloc,returns_nonnull,hot)) Scope* Scope_init() {
    /* pass */
    Scope* s = ((Scope*)_tr_checked_alloc(sizeof(Scope)));
    /* pass */
    s->variables = _tr_dict_new(32LL);
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
    __auto_type _t129 = (*e);
    if (_t129.tag == Expr_EIdent) {
        __auto_type n = _t129.data.EIdent.name;
        /* pass */
        return box_asttype(AstType_init(n));
    } else if (_t129.tag == Expr_EIndex) {
        __auto_type obj = _t129.data.EIndex.obj;
__auto_type idx = _t129.data.EIndex.index;
        /* pass */
        __auto_type _t130 = (*obj);
        if (_t130.tag == Expr_EIdent) {
            __auto_type on = _t130.data.EIdent.name;
            /* pass */
            AstType* at = AstType_init(on);
            /* pass */
            at->args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(at->args, Sema_build_ast_type(self, idx));
            /* pass */
            return box_asttype(at);
        } else if (1) {
            __auto_type _ = _t130;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t129;
        /* pass */
    }
    /* pass */
    return box_asttype(AstType_init("void"));
}

__attribute__((malloc,returns_nonnull,hot)) Sema* Sema_init() {
    /* pass */
    Sema* s = ((Sema*)_tr_checked_alloc(sizeof(Sema)));
    /* pass */
    s->globals = _tr_dict_new(1024LL);
    /* pass */
    s->scopes = (void*)List_ptr_new();
    /* pass */
    s->errors = (void*)List_str_new();
    /* pass */
    s->warnings = (void*)List_str_new();
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
    s->current_file = "";
    /* pass */
    s->current_func_name = "";
    /* pass */
    s->current_class_name = "";
    /* pass */
    s->current_scope_depth = 0LL;
    /* pass */
    s->in_async_fn = false;
    /* pass */
    s->assign_froms = _tr_dict_new(32LL);
    /* pass */
    s->fn_sigs = _tr_dict_new(64LL);
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
    s->current_func_generics = (void*)List_str_new();
    /* pass */
    s->closure_boundary = (-1LL);
    /* pass */
    s->closure_caps = (void*)List_ptr_new();
    /* pass */
    s->closure_cap_set = _tr_dict_new(0LL);
    /* pass */
    s->in_assign_target = false;
    /* pass */
    s->container_borrows = _tr_dict_new(16LL);
    /* pass */
    s->capturing_moves = false;
    /* pass */
    s->branch_moved_buf = (void*)List_str_new();
    /* pass */
    s->capturing_inits = false;
    /* pass */
    s->branch_init_buf = (void*)List_str_new();
    /* pass */
    s->copy_classes = _tr_dict_new(32LL);
    /* pass */
    s->in_unsafe = false;
    /* pass */
    s->current_func_ret_from = "";
    /* pass */
    s->strict_mode = false;
    /* pass */
    s->decorator_names = _tr_dict_new(16LL);
    /* pass */
    _tr_dict_set(s->globals, "print", Symbol_init("print", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "input", Symbol_init("input", SymbolKind_make_SFunction(), box_asttype(AstType_init("str"))));
    /* pass */
    _tr_dict_set(s->globals, "len", Symbol_init("len", SymbolKind_make_SFunction(), box_asttype(AstType_init("int"))));
    /* pass */
    _tr_dict_set(s->globals, "range", Symbol_init("range", SymbolKind_make_SFunction(), box_asttype(AstType_init("List"))));
    /* pass */
    _tr_dict_set(s->globals, "type", Symbol_init("type", SymbolKind_make_SFunction(), box_asttype(AstType_init("str"))));
    /* pass */
    _tr_dict_set(s->globals, "str", Symbol_init("str", SymbolKind_make_SFunction(), box_asttype(AstType_init("str"))));
    /* pass */
    _tr_dict_set(s->globals, "int", Symbol_init("int", SymbolKind_make_SFunction(), box_asttype(AstType_init("int"))));
    /* pass */
    _tr_dict_set(s->globals, "float", Symbol_init("float", SymbolKind_make_SFunction(), box_asttype(AstType_init("float"))));
    /* pass */
    _tr_dict_set(s->globals, "bool", Symbol_init("bool", SymbolKind_make_SFunction(), box_asttype(AstType_init("bool"))));
    /* pass */
    _tr_dict_set(s->globals, "ord", Symbol_init("ord", SymbolKind_make_SFunction(), box_asttype(AstType_init("int"))));
    /* pass */
    _tr_dict_set(s->globals, "chr", Symbol_init("chr", SymbolKind_make_SFunction(), box_asttype(AstType_init("char"))));
    /* pass */
    _tr_dict_set(s->globals, "abs", Symbol_init("abs", SymbolKind_make_SFunction(), box_asttype(AstType_init("int"))));
    /* pass */
    _tr_dict_set(s->globals, "max", Symbol_init("max", SymbolKind_make_SFunction(), box_asttype(AstType_init("int"))));
    /* pass */
    _tr_dict_set(s->globals, "min", Symbol_init("min", SymbolKind_make_SFunction(), box_asttype(AstType_init("int"))));
    /* pass */
    _tr_dict_set(s->globals, "sum", Symbol_init("sum", SymbolKind_make_SFunction(), box_asttype(AstType_init("int"))));
    /* pass */
    _tr_dict_set(s->globals, "round", Symbol_init("round", SymbolKind_make_SFunction(), box_asttype(AstType_init("float"))));
    /* pass */
    _tr_dict_set(s->globals, "sorted", Symbol_init("sorted", SymbolKind_make_SFunction(), box_asttype(AstType_init("List"))));
    /* pass */
    _tr_dict_set(s->globals, "reversed", Symbol_init("reversed", SymbolKind_make_SFunction(), box_asttype(AstType_init("List"))));
    /* pass */
    _tr_dict_set(s->globals, "iter", Symbol_init("iter", SymbolKind_make_SFunction(), box_asttype(AstType_init("List"))));
    /* pass */
    _tr_dict_set(s->globals, "enumerate", Symbol_init("enumerate", SymbolKind_make_SFunction(), box_asttype(AstType_init("List"))));
    /* pass */
    _tr_dict_set(s->globals, "zip", Symbol_init("zip", SymbolKind_make_SFunction(), box_asttype(AstType_init("List"))));
    /* pass */
    _tr_dict_set(s->globals, "map", Symbol_init("map", SymbolKind_make_SFunction(), box_asttype(AstType_init("List"))));
    /* pass */
    _tr_dict_set(s->globals, "filter", Symbol_init("filter", SymbolKind_make_SFunction(), box_asttype(AstType_init("List"))));
    /* pass */
    _tr_dict_set(s->globals, "all", Symbol_init("all", SymbolKind_make_SFunction(), box_asttype(AstType_init("bool"))));
    /* pass */
    _tr_dict_set(s->globals, "any", Symbol_init("any", SymbolKind_make_SFunction(), box_asttype(AstType_init("bool"))));
    /* pass */
    _tr_dict_set(s->globals, "assert", Symbol_init("assert", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "assert_eq", Symbol_init("assert_eq", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "assert_ne", Symbol_init("assert_ne", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "assert_lt", Symbol_init("assert_lt", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "assert_le", Symbol_init("assert_le", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "assert_gt", Symbol_init("assert_gt", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "assert_ge", Symbol_init("assert_ge", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "Some", Symbol_init("Some", SymbolKind_make_SFunction(), box_asttype(AstType_init("Option"))));
    /* pass */
    _tr_dict_set(s->globals, "Ok", Symbol_init("Ok", SymbolKind_make_SFunction(), box_asttype(AstType_init("Result"))));
    /* pass */
    _tr_dict_set(s->globals, "Err", Symbol_init("Err", SymbolKind_make_SFunction(), box_asttype(AstType_init("Result"))));
    /* pass */
    _tr_dict_set(s->globals, "_tr_exit", Symbol_init("_tr_exit", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "_tr_strlen", Symbol_init("_tr_strlen", SymbolKind_make_SFunction(), box_asttype(AstType_init("int"))));
    /* pass */
    _tr_dict_set(s->globals, "read_file", Symbol_init("read_file", SymbolKind_make_SFunction(), box_asttype(AstType_init("str"))));
    /* pass */
    _tr_dict_set(s->globals, "write_file", Symbol_init("write_file", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "file_exists", Symbol_init("file_exists", SymbolKind_make_SFunction(), box_asttype(AstType_init("bool"))));
    /* pass */
    _tr_dict_set(s->globals, "_tr_system", Symbol_init("_tr_system", SymbolKind_make_SFunction(), box_asttype(AstType_init("int"))));
    /* pass */
    _tr_dict_set(s->globals, "List", Symbol_init("List", SymbolKind_make_SClass(), box_asttype(AstType_init("List"))));
    /* pass */
    _tr_dict_set(s->globals, "Pointer", Symbol_init("Pointer", SymbolKind_make_SClass(), box_asttype(AstType_init("Pointer"))));
    /* pass */
    _tr_dict_set(s->globals, "StringBuilder", Symbol_init("StringBuilder", SymbolKind_make_SClass(), box_asttype(AstType_init("StringBuilder"))));
    /* pass */
    _tr_dict_set(s->globals, "StringObj", Symbol_init("StringObj", SymbolKind_make_SClass(), box_asttype(AstType_init("StringObj"))));
    /* pass */
    _tr_dict_set(s->globals, "Dict", Symbol_init("Dict", SymbolKind_make_SClass(), box_asttype(AstType_init("Dict"))));
    /* pass */
    _tr_dict_set(s->globals, "Set", Symbol_init("Set", SymbolKind_make_SClass(), box_asttype(AstType_init("Set"))));
    /* pass */
    _tr_dict_set(s->globals, "Box", Symbol_init("Box", SymbolKind_make_SClass(), box_asttype(AstType_init("Box"))));
    /* pass */
    _tr_dict_set(s->globals, "Option", Symbol_init("Option", SymbolKind_make_SEnum(), box_asttype(AstType_init("Option"))));
    /* pass */
    _tr_dict_set(s->globals, "Result", Symbol_init("Result", SymbolKind_make_SEnum(), box_asttype(AstType_init("Result"))));
    /* pass */
    _tr_dict_set(s->globals, "Exception", Symbol_init("Exception", SymbolKind_make_SClass(), box_asttype(AstType_init("Exception"))));
    /* pass */
    _tr_dict_set(s->globals, "Error", Symbol_init("Error", SymbolKind_make_SClass(), box_asttype(AstType_init("Error"))));
    /* pass */
    _tr_dict_set(s->globals, "ValueError", Symbol_init("ValueError", SymbolKind_make_SClass(), box_asttype(AstType_init("ValueError"))));
    /* pass */
    _tr_dict_set(s->globals, "TypeError", Symbol_init("TypeError", SymbolKind_make_SClass(), box_asttype(AstType_init("TypeError"))));
    /* pass */
    _tr_dict_set(s->globals, "IndexError", Symbol_init("IndexError", SymbolKind_make_SClass(), box_asttype(AstType_init("IndexError"))));
    /* pass */
    _tr_dict_set(s->globals, "IOError", Symbol_init("IOError", SymbolKind_make_SClass(), box_asttype(AstType_init("IOError"))));
    /* pass */
    _tr_dict_set(s->globals, "KeyError", Symbol_init("KeyError", SymbolKind_make_SClass(), box_asttype(AstType_init("KeyError"))));
    /* pass */
    _tr_dict_set(s->globals, "Task", Symbol_init("Task", SymbolKind_make_SClass(), box_asttype(AstType_init("Task"))));
    /* pass */
    _tr_dict_set(s->globals, "Future", Symbol_init("Future", SymbolKind_make_SClass(), box_asttype(AstType_init("Future"))));
    /* pass */
    _tr_dict_set(s->globals, "Channel", Symbol_init("Channel", SymbolKind_make_SClass(), box_asttype(AstType_init("Channel"))));
    /* pass */
    _tr_dict_set(s->globals, "Chan", Symbol_init("Chan", SymbolKind_make_SClass(), box_asttype(AstType_init("Chan"))));
    /* pass */
    _tr_dict_set(s->globals, "Mutex", Symbol_init("Mutex", SymbolKind_make_SClass(), box_asttype(AstType_init("Mutex"))));
    /* pass */
    _tr_dict_set(s->globals, "RwLock", Symbol_init("RwLock", SymbolKind_make_SClass(), box_asttype(AstType_init("RwLock"))));
    /* pass */
    _tr_dict_set(s->globals, "ThreadPool", Symbol_init("ThreadPool", SymbolKind_make_SClass(), box_asttype(AstType_init("ThreadPool"))));
    /* pass */
    _tr_dict_set(s->globals, "Thread", Symbol_init("Thread", SymbolKind_make_SClass(), box_asttype(AstType_init("Thread"))));
    /* pass */
    _tr_dict_set(s->globals, "Atomic", Symbol_init("Atomic", SymbolKind_make_SClass(), box_asttype(AstType_init("Atomic"))));
    /* pass */
    _tr_dict_set(s->globals, "ThreadLocal", Symbol_init("ThreadLocal", SymbolKind_make_SClass(), box_asttype(AstType_init("ThreadLocal"))));
    /* pass */
    _tr_dict_set(s->globals, "await_all", Symbol_init("await_all", SymbolKind_make_SFunction(), box_asttype(AstType_init("void"))));
    /* pass */
    _tr_dict_set(s->globals, "Arc", Symbol_init("Arc", SymbolKind_make_SClass(), box_asttype(AstType_init("Arc"))));
    /* pass */
    _tr_dict_set(s->globals, "Rc", Symbol_init("Rc", SymbolKind_make_SClass(), box_asttype(AstType_init("Rc"))));
    /* pass */
    _tr_dict_set(s->globals, "WaitGroup", Symbol_init("WaitGroup", SymbolKind_make_SClass(), box_asttype(AstType_init("WaitGroup"))));
    /* pass */
    _tr_dict_set(s->globals, "Shared", Symbol_init("Shared", SymbolKind_make_SClass(), box_asttype(AstType_init("Shared"))));
    /* pass */
    _tr_dict_set(s->globals, "Weak", Symbol_init("Weak", SymbolKind_make_SClass(), box_asttype(AstType_init("Weak"))));
    /* pass */
    return s;
}

__attribute__((hot)) void Sema_error(Sema* self, char* msg) {
    /* pass */
    char* loc = "";
    /* pass */
    if ((_tr_strlen((char*)self->current_file) > 0LL)) {
        /* pass */
        loc = _tr_str_concat(self->current_file, ":");
    }
    /* pass */
    if ((self->current_line > 0LL)) {
        /* pass */
        List_str_append(self->errors, _tr_str_concat(_tr_str_concat(_tr_str_concat(loc, _tr_int_to_str((long long)(self->current_line))), ": "), msg));
    } else {
        /* pass */
        List_str_append(self->errors, msg);
    }
}

__attribute__((hot)) bool Sema_is_sendable_type(Sema* self, char* ty_name) {
    /* pass */
    if ((((((strcmp((char*)ty_name, (char*)"int") == 0) || (strcmp((char*)ty_name, (char*)"float") == 0)) || (strcmp((char*)ty_name, (char*)"bool") == 0)) || (strcmp((char*)ty_name, (char*)"char") == 0)) || (strcmp((char*)ty_name, (char*)"str") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp((char*)ty_name, (char*)"void") == 0) || (strcmp((char*)ty_name, (char*)"") == 0)) || (strcmp((char*)ty_name, (char*)"auto") == 0)) || (strcmp((char*)ty_name, (char*)"usize") == 0)) || (strcmp((char*)ty_name, (char*)"isize") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)ty_name, (char*)"Atomic") == 0) || (strcmp((char*)ty_name, (char*)"Mutex") == 0)) || (strcmp((char*)ty_name, (char*)"RwLock") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)ty_name, (char*)"Chan") == 0) || (strcmp((char*)ty_name, (char*)"Channel") == 0)) || (strcmp((char*)ty_name, (char*)"ThreadPool") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp((char*)ty_name, (char*)"Thread") == 0) || (strcmp((char*)ty_name, (char*)"ThreadLocal") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp((char*)ty_name, (char*)"Shared") == 0) || (strcmp((char*)ty_name, (char*)"Weak") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)ty_name, (char*)"List") == 0) || (strcmp((char*)ty_name, (char*)"Vec") == 0)) || (strcmp((char*)ty_name, (char*)"Dict") == 0)) || (strcmp((char*)ty_name, (char*)"Map") == 0))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, ty_name)) {
        /* pass */
        ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, ty_name));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(cls->iface_names))))) {
            /* pass */
            long long ii = 0LL;
            /* pass */
            while ((ii < cls->iface_names->len)) {
                /* pass */
                if ((strcmp((char*)List_str_get(cls->iface_names, ii), (char*)"Sendable") == 0)) {
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

__attribute__((hot)) bool Sema_class_method_exists(Sema* self, char* cls_name, char* method) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, cls_name))) {
        /* pass */
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, cls_name));
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < cls->methods->len)) {
        /* pass */
        if ((strcmp((char*)((FunctionDef*)List_ptr_get(cls->methods, mi))->name, (char*)method) == 0)) {
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
        if (Sema_class_method_exists(self, List_str_get(cls->base_classes, bi), method)) {
            /* pass */
            return true;
        }
        /* pass */
        bi = (bi + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_universal_method(Sema* self, char* method) {
    /* pass */
    if ((((strcmp((char*)method, (char*)"init") == 0) || (strcmp((char*)method, (char*)"new") == 0)) || (strcmp((char*)method, (char*)"free") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)method, (char*)"to_str") == 0) || (strcmp((char*)method, (char*)"to_string") == 0)) || (strcmp((char*)method, (char*)"as_str") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"length") == 0)) || (strcmp((char*)method, (char*)"__len__") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp((char*)method, (char*)"clone") == 0) || (strcmp((char*)method, (char*)"copy") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)method, (char*)"__getitem__") == 0) || (strcmp((char*)method, (char*)"get_index") == 0)) || (strcmp((char*)method, (char*)"__setitem__") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp((char*)method, (char*)"__eq__") == 0) || (strcmp((char*)method, (char*)"__ne__") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)method, (char*)"__lt__") == 0) || (strcmp((char*)method, (char*)"__gt__") == 0)) || (strcmp((char*)method, (char*)"__le__") == 0)) || (strcmp((char*)method, (char*)"__ge__") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp((char*)method, (char*)"__add__") == 0) || (strcmp((char*)method, (char*)"__sub__") == 0)) || (strcmp((char*)method, (char*)"__mul__") == 0)) || (strcmp((char*)method, (char*)"__div__") == 0)) || (strcmp((char*)method, (char*)"__mod__") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)method, (char*)"__hash__") == 0) || (strcmp((char*)method, (char*)"__iter__") == 0)) || (strcmp((char*)method, (char*)"__next__") == 0)) || (strcmp((char*)method, (char*)"__contains__") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)method, (char*)"__str__") == 0) || (strcmp((char*)method, (char*)"__repr__") == 0)) || (strcmp((char*)method, (char*)"__enter__") == 0)) || (strcmp((char*)method, (char*)"__exit__") == 0))) {
        /* pass */
        return true;
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
    __auto_type _t131 = (*e);
    if (_t131.tag == HirExpr_ECall) {
        __auto_type cs_args = _t131.data.ECall.args;
        /* pass */
        long long csi = 0LL;
        /* pass */
        while ((csi < cs_args->len)) {
            /* pass */
            AstType* arg_ty = hir_expr_type(((HirExpr*)List_ptr_get(cs_args, csi)));
            /* pass */
            if ((((strcmp((char*)arg_ty->name, (char*)"Shared") == 0) || (strcmp((char*)arg_ty->name, (char*)"Weak") == 0)) && (arg_ty->args->len > 0LL))) {
                /* pass */
                char* inner_nm = (*((AstType**)List_ptr_get(arg_ty->args, 0LL)))->name;
                /* pass */
                if ((!Sema_is_sendable_type(self, inner_nm))) {
                    /* pass */
                    Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-1] 'Shared[", inner_nm), "]' cannot safely cross thread boundaries because '"), inner_nm), "' is not Sendable.\n      FIX: Add 'implements Sendable' to '"), inner_nm), "' and ensure all mutable fields use Atomic[T] or Mutex[T]."));
                }
            } else if ((!Sema_is_sendable_type(self, arg_ty->name))) {
                /* pass */
                Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-1] Type '", arg_ty->name), "' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex["), arg_ty->name), "] for exclusive access, or Atomic[T] for counters/flags.\n      Or add 'implements Sendable' to '"), arg_ty->name), "' to confirm it is thread-safe."));
            }
            /* pass */
            csi = (csi + 1LL);
        }
    } else if (_t131.tag == HirExpr_EMethodCall) {
        __auto_type cs_margs = _t131.data.EMethodCall.args;
        /* pass */
        long long csmi = 0LL;
        /* pass */
        while ((csmi < cs_margs->len)) {
            /* pass */
            AstType* arg_ty2 = hir_expr_type(((HirExpr*)List_ptr_get(cs_margs, csmi)));
            /* pass */
            if ((((strcmp((char*)arg_ty2->name, (char*)"Shared") == 0) || (strcmp((char*)arg_ty2->name, (char*)"Weak") == 0)) && (arg_ty2->args->len > 0LL))) {
                /* pass */
                char* inner_nm2 = (*((AstType**)List_ptr_get(arg_ty2->args, 0LL)))->name;
                /* pass */
                if ((!Sema_is_sendable_type(self, inner_nm2))) {
                    /* pass */
                    Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-1] 'Shared[", inner_nm2), "]' cannot safely cross thread boundaries because '"), inner_nm2), "' is not Sendable.\n      FIX: Add 'implements Sendable' to '"), inner_nm2), "' and protect mutable fields with Atomic[T] or Mutex[T]."));
                }
            } else if ((!Sema_is_sendable_type(self, arg_ty2->name))) {
                /* pass */
                Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-1] Type '", arg_ty2->name), "' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex["), arg_ty2->name), "]."));
            }
            /* pass */
            csmi = (csmi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t131;
        /* pass */
    }
}

__attribute__((hot)) void Sema_check_class_sendable_fields(Sema* self, ClassDef* c) {
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
            if ((!Sema_is_sendable_type(self, fty->name))) {
                /* pass */
                Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-2] Class '", c->name), "' declares 'implements Sendable' but field '"), fd->name), ": "), fty->name), "' is not Sendable.\n      FIX: Wrap '"), fd->name), "' in Mutex["), fty->name), "] for exclusive access, RwLock["), fty->name), "] for reader-writer, or Atomic[T] for numeric/flag types.\n      Or remove 'implements Sendable' if '"), c->name), "' is only used on one thread."));
            } else if ((((strcmp((char*)fty->name, (char*)"int") == 0) || (strcmp((char*)fty->name, (char*)"float") == 0)) || (strcmp((char*)fty->name, (char*)"bool") == 0))) {
                /* pass */
                List_str_append(self->warnings, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-3] Sendable class '", c->name), "' has primitive field '"), fd->name), ": "), fty->name), "' that may cause data races if mutated from multiple threads.\n      FIX: Use 'Atomic["), fty->name), "]' for safe concurrent mutation, or ensure this field is written only before the object is shared across threads."));
            }
        }
        /* pass */
        cfi = (cfi + 1LL);
    }
}

__attribute__((hot)) void Sema_mark_moved(Sema* self, char* name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, name)) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, name));
            /* pass */
            if ((!sym->is_moved)) {
                /* pass */
                sym->is_moved = true;
                /* pass */
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, name, sym);
                /* pass */
                if (self->capturing_moves) {
                    /* pass */
                    List_str_append(self->branch_moved_buf, name);
                }
            }
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
        /* pass */
        if ((!sym->is_moved)) {
            /* pass */
            sym->is_moved = true;
            /* pass */
            _tr_dict_set(self->globals, name, sym);
            /* pass */
            if (self->capturing_moves) {
                /* pass */
                List_str_append(self->branch_moved_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_mark_freed(Sema* self, char* name) {
    /* pass */
    long long mf_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mf_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, name)) {
            /* pass */
            Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, name));
            /* pass */
            mf_sym->is_freed = true;
            /* pass */
            mf_sym->is_moved = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, name, mf_sym);
            /* pass */
            if (self->capturing_moves) {
                /* pass */
                List_str_append(self->branch_moved_buf, name);
            }
            /* pass */
            return;
        }
        /* pass */
        mf_i = (mf_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
        /* pass */
        mf_sym->is_freed = true;
        /* pass */
        mf_sym->is_moved = true;
        /* pass */
        _tr_dict_set(self->globals, name, mf_sym);
        /* pass */
        if (self->capturing_moves) {
            /* pass */
            List_str_append(self->branch_moved_buf, name);
        }
    }
}

__attribute__((hot)) void Sema_check_not_moved(Sema* self, char* name, char* ty_name) {
    /* pass */
    if (Sema_is_primitive_name(self, ty_name)) {
        /* pass */
        return;
    }
    /* pass */
    Symbol* sym = Sema_resolve(self, name);
    /* pass */
    if (((strcmp((char*)sym->name, (char*)"") != 0) && sym->is_moved)) {
        /* pass */
        Sema_error(self, _tr_str_concat(_tr_str_concat("[M-1] '", name), "' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving."));
    }
}

__attribute__((hot)) void Sema_mark_borrow(Sema* self, char* name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, name)) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, name));
            /* pass */
            sym->active_borrows = (sym->active_borrows + 1LL);
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, name, sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
        /* pass */
        sym->active_borrows = (sym->active_borrows + 1LL);
        /* pass */
        _tr_dict_set(self->globals, name, sym);
    }
}

__attribute__((hot)) void Sema_unmark_borrow(Sema* self, char* name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, name)) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, name));
            /* pass */
            if ((sym->active_borrows > 0LL)) {
                /* pass */
                sym->active_borrows = (sym->active_borrows - 1LL);
            }
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, name, sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
        /* pass */
        if ((sym->active_borrows > 0LL)) {
            /* pass */
            sym->active_borrows = (sym->active_borrows - 1LL);
        }
        /* pass */
        _tr_dict_set(self->globals, name, sym);
    }
}

__attribute__((hot)) void Sema_check_no_active_borrows(Sema* self, char* name, char* ty_name) {
    /* pass */
    if (Sema_is_primitive_name(self, ty_name)) {
        /* pass */
        return;
    }
    /* pass */
    Symbol* sym = Sema_resolve(self, name);
    /* pass */
    if (((strcmp((char*)sym->name, (char*)"") != 0) && (sym->active_borrows > 0LL))) {
        /* pass */
        Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[M-2] Cannot move '", name), "' while it is borrowed.\n      FIX: The borrow must end before '"), name), "' can be moved."));
    }
}

__attribute__((hot)) void Sema_mark_init(Sema* self, char* name) {
    /* pass */
    long long mi_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mi_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, name)) {
            /* pass */
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, name));
            /* pass */
            if ((!mi_sym->is_init)) {
                /* pass */
                mi_sym->is_init = true;
                /* pass */
                mi_sym->is_maybe_init = false;
                /* pass */
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, name, mi_sym);
                /* pass */
                if (self->capturing_inits) {
                    /* pass */
                    List_str_append(self->branch_init_buf, name);
                }
            }
            /* pass */
            return;
        }
        /* pass */
        mi_i = (mi_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
        /* pass */
        if ((!mi_sym->is_init)) {
            /* pass */
            mi_sym->is_init = true;
            /* pass */
            mi_sym->is_maybe_init = false;
            /* pass */
            _tr_dict_set(self->globals, name, mi_sym);
            /* pass */
            if (self->capturing_inits) {
                /* pass */
                List_str_append(self->branch_init_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_clear_container_borrow(Sema* self, char* var_name) {
    /* pass */
    if (_tr_dict_contains(self->container_borrows, var_name)) {
        /* pass */
        _tr_dict_remove(self->container_borrows, var_name);
    }
}

__attribute__((hot)) void Sema_unmark_moved(Sema* self, char* name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, name)) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, name));
            /* pass */
            sym->is_moved = false;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, name, sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
        /* pass */
        sym->is_moved = false;
        /* pass */
        _tr_dict_set(self->globals, name, sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_moved(Sema* self, char* name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, name)) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, name));
            /* pass */
            sym->is_maybe_moved = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, name, sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
        /* pass */
        sym->is_maybe_moved = true;
        /* pass */
        _tr_dict_set(self->globals, name, sym);
    }
}

__attribute__((hot)) void Sema_unmark_init(Sema* self, char* name) {
    /* pass */
    long long mi_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mi_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, name)) {
            /* pass */
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, name));
            /* pass */
            mi_sym->is_init = false;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, name, mi_sym);
            /* pass */
            return;
        }
        /* pass */
        mi_i = (mi_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
        /* pass */
        mi_sym->is_init = false;
        /* pass */
        _tr_dict_set(self->globals, name, mi_sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_init(Sema* self, char* name) {
    /* pass */
    long long mi_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mi_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, name)) {
            /* pass */
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, name));
            /* pass */
            mi_sym->is_maybe_init = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, name, mi_sym);
            /* pass */
            return;
        }
        /* pass */
        mi_i = (mi_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
        /* pass */
        mi_sym->is_maybe_init = true;
        /* pass */
        _tr_dict_set(self->globals, name, mi_sym);
    }
}

__attribute__((hot)) bool Sema_vec_str_contains(Sema* self, List_str* v, char* s) {
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < v->len)) {
        /* pass */
        if ((strcmp((char*)List_str_get(v, vi), (char*)s) == 0)) {
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
        if ((strcmp((char*)((Decorator*)List_ptr_get(decs, i))->name, (char*)"copy") == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_copy_class(Sema* self, char* name) {
    /* pass */
    if (Sema_is_primitive_name(self, name)) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->copy_classes, name)) {
        /* pass */
        return ((bool)(uintptr_t)_tr_dict_get(self->copy_classes, name));
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, name))) {
        /* pass */
        return false;
    }
    /* pass */
    _tr_dict_set(self->copy_classes, name, false);
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, name));
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
            if ((!Sema_is_copy_class(self, fty->name))) {
                /* pass */
                all_copy = false;
            }
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    _tr_dict_set(self->copy_classes, name, all_copy);
    /* pass */
    return all_copy;
}

__attribute__((hot)) void Sema_enter_scope(Sema* self) {
    /* pass */
    Scope* s_obj = Scope_init();
    /* pass */
    List_ptr_append(self->scopes, s_obj);
    /* pass */
    self->current_scope_depth = (self->current_scope_depth + 1LL);
}

__attribute__((hot)) void Sema_exit_scope(Sema* self) {
    /* pass */
    ((Scope*)List_ptr_pop(self->scopes));
    /* pass */
    self->current_scope_depth = (self->current_scope_depth - 1LL);
}

__attribute__((hot)) void Sema_declare(Sema* self, char* name, SymbolKind kind, AstType** ty, bool is_mut) {
    /* pass */
    if ((((strcmp((char*)name, (char*)"self") != 0) && (strcmp((char*)name, (char*)"_") != 0)) && (_tr_strlen(name) > 1LL))) {
        /* pass */
        char* cat = Sema_is_reserved_error(self, name);
        /* pass */
        if ((strcmp((char*)cat, (char*)"") != 0)) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[N-1] '", name), "' is a "), cat), " and cannot be used as a name. Choose a different name (e.g. 'my_"), name), "')."));
        } else {
            /* pass */
            bool is_toplevel_decl = ((strcmp((char*)self->current_class_name, (char*)"") == 0) && (kind.tag != SymbolKind_make_SVariable().tag));
            /* pass */
            if (is_toplevel_decl) {
                /* pass */
                char* kcat = Sema_is_reserved_keyword(self, name);
                /* pass */
                if ((strcmp((char*)kcat, (char*)"") != 0)) {
                    /* pass */
                    Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[N-1] '", name), "' is a "), kcat), " and is reserved. Choose a different name (e.g. 'my_"), name), "')."));
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
    if ((self->scopes->len > 0LL)) {
        /* pass */
        _tr_dict_set(((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)))->variables, name, sym);
    } else {
        /* pass */
        _tr_dict_set(self->globals, name, sym);
    }
}

__attribute__((hot)) Symbol* Sema_resolve(Sema* self, char* name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        Scope* scope = ((Scope*)List_ptr_get(self->scopes, i));
        /* pass */
        if (_tr_dict_contains(scope->variables, name)) {
            /* pass */
            return ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, name));
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, name)) {
        /* pass */
        return ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, name));
    }
    /* pass */
    return Symbol_init("", SymbolKind_make_SVariable(), box_asttype(AstType_init("void")));
}

__attribute__((hot)) HirProgram* Sema_analyze(Sema* self, Program* prog) {
    /* pass */
    HirProgram* hp = HirProgram_init();
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
    i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        Decl* d = ((Decl*)List_ptr_get(prog->decls, i));
        /* pass */
        __auto_type _t132 = (*d);
        if (_t132.tag == Decl_DFunction) {
            __auto_type f = _t132.data.DFunction.func;
            /* pass */
            List_ptr_append(hp->functions, Sema_lower_func(self, f));
        } else if (_t132.tag == Decl_DClass) {
            __auto_type c = _t132.data.DClass.cls;
            /* pass */
            List_ptr_append(hp->classes, Sema_lower_class(self, c));
        } else if (_t132.tag == Decl_DActor) {
            __auto_type c = _t132.data.DActor.cls;
            /* pass */
            List_ptr_append(hp->classes, Sema_lower_class(self, c));
        } else if (_t132.tag == Decl_DEnum) {
            __auto_type e = _t132.data.DEnum.enm;
            /* pass */
            List_ptr_append(hp->enums, Sema_lower_enum(self, e));
        } else if (_t132.tag == Decl_DInterface) {
            __auto_type i_def = _t132.data.DInterface.iface;
            /* pass */
            List_ptr_append(hp->interfaces, Sema_lower_interface(self, i_def));
        } else if (_t132.tag == Decl_DExtend) {
            __auto_type target = _t132.data.DExtend.target;
__auto_type methods = _t132.data.DExtend.methods;
            /* pass */
            self->current_class_name = target;
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
            self->current_class_name = "";
        } else if (_t132.tag == Decl_DTopLevelStmt) {
            __auto_type s = _t132.data.DTopLevelStmt.stmt;
            /* pass */
            List_ptr_append(hp->top_level_stmts, Sema_lower_stmt(self, s));
        } else if (_t132.tag == Decl_DExtern) {
            __auto_type abi = _t132.data.DExtern.abi;
__auto_type functions = _t132.data.DExtern.functions;
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
                List_ptr_append(hp->extern_funcs, hef);
                /* pass */
                ei = (ei + 1LL);
            }
        } else if (_t132.tag == Decl_DDecoratorDef) {
            __auto_type f = _t132.data.DDecoratorDef.func;
            /* pass */
            HirFunction* hdf = Sema_lower_func(self, f);
            /* pass */
            hdf->is_decorator = true;
            /* pass */
            List_ptr_append(hp->decorator_defs, hdf);
        } else if (_t132.tag == Decl_DTypeAlias) {
            __auto_type alias_name = _t132.data.DTypeAlias.name;
__auto_type target_ty = _t132.data.DTypeAlias.target;
            /* pass */
            List_str_append(hp->type_alias_names, alias_name);
            /* pass */
            AstType** ta_ty_ptr = box_asttype(AstType_init("void"));
            /* pass */
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                ta_ty_ptr = target_ty;
            }
            /* pass */
            List_ptr_append(hp->type_alias_types, ta_ty_ptr);
        } else if (1) {
            __auto_type _ = _t132;
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
        List_ptr_append(hp->classes, ((HirClass*)List_ptr_get(self->nested_classes, nci)));
        /* pass */
        nci = (nci + 1LL);
    }
    /* pass */
    long long nfi = 0LL;
    /* pass */
    while ((nfi < self->nested_functions->len)) {
        /* pass */
        List_ptr_append(hp->functions, ((HirFunction*)List_ptr_get(self->nested_functions, nfi)));
        /* pass */
        nfi = (nfi + 1LL);
    }
    /* pass */
    long long nei = 0LL;
    /* pass */
    while ((nei < self->nested_enums->len)) {
        /* pass */
        List_ptr_append(hp->enums, ((HirEnum*)List_ptr_get(self->nested_enums, nei)));
        /* pass */
        nei = (nei + 1LL);
    }
    /* pass */
    long long nii = 0LL;
    /* pass */
    while ((nii < self->nested_interfaces->len)) {
        /* pass */
        List_ptr_append(hp->interfaces, ((HirInterface*)List_ptr_get(self->nested_interfaces, nii)));
        /* pass */
        nii = (nii + 1LL);
    }
    /* pass */
    return hp;
}

__attribute__((hot)) void Sema_register_decl(Sema* self, Decl* d) {
    /* pass */
    __auto_type _t133 = (*d);
    if (_t133.tag == Decl_DFunction) {
        __auto_type f = _t133.data.DFunction.func;
        /* pass */
        AstType** _f_ret = box_asttype(AstType_init("void"));
        /* pass */
        if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            _f_ret = f->ret_ty;
        }
        /* pass */
        Sema_declare(self, f->name, SymbolKind_make_SFunction(), _f_ret, false);
        /* pass */
        AstType* _fnty = AstType_init("def");
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
        _tr_dict_set(self->fn_sigs, f->name, _fnty);
    } else if (_t133.tag == Decl_DClass) {
        __auto_type c = _t133.data.DClass.cls;
        /* pass */
        Sema_declare(self, c->name, SymbolKind_make_SClass(), box_asttype(AstType_init(c->name)), false);
        /* pass */
        _tr_dict_set(self->classes, c->name, c);
        /* pass */
        if (Sema_has_copy_decorator(self, c->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, c->name, true);
        }
    } else if (_t133.tag == Decl_DActor) {
        __auto_type c = _t133.data.DActor.cls;
        /* pass */
        Sema_declare(self, c->name, SymbolKind_make_SClass(), box_asttype(AstType_init(c->name)), false);
        /* pass */
        _tr_dict_set(self->classes, c->name, c);
        /* pass */
        if (Sema_has_copy_decorator(self, c->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, c->name, true);
        }
    } else if (_t133.tag == Decl_DEnum) {
        __auto_type e = _t133.data.DEnum.enm;
        /* pass */
        Sema_declare(self, e->name, SymbolKind_make_SEnum(), box_asttype(AstType_init(e->name)), false);
        /* pass */
        _tr_dict_set(self->enums, e->name, e);
        /* pass */
        if (Sema_has_copy_decorator(self, e->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, e->name, true);
        }
    } else if (_t133.tag == Decl_DInterface) {
        __auto_type i = _t133.data.DInterface.iface;
        /* pass */
        Sema_declare(self, i->name, SymbolKind_make_SInterface(), box_asttype(AstType_init(i->name)), false);
        /* pass */
        _tr_dict_set(self->interfaces, i->name, i);
        /* pass */
        if (Sema_has_copy_decorator(self, i->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, i->name, true);
        }
    } else if (_t133.tag == Decl_DExtend) {
        __auto_type target = _t133.data.DExtend.target;
__auto_type methods = _t133.data.DExtend.methods;
        /* pass */
        long long hi = 0LL;
        /* pass */
        while ((hi < methods->len)) {
            /* pass */
            FunctionDef* f = ((FunctionDef*)List_ptr_get(methods, hi));
            /* pass */
            AstType** _m_ret = box_asttype(AstType_init("void"));
            /* pass */
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                _m_ret = f->ret_ty;
            }
            /* pass */
            char* _decl_key = _tr_str_concat(_tr_str_concat(target, "_"), f->name);
            /* pass */
            if (_tr_dict_contains(self->globals, _decl_key)) {
                /* pass */
                long long _pc = 0LL;
                /* pass */
                long long _pci = 0LL;
                /* pass */
                while ((_pci < f->params->len)) {
                    /* pass */
                    if ((strcmp((char*)((Param*)List_ptr_get(f->params, _pci))->name, (char*)"self") != 0)) {
                        /* pass */
                        _pc = (_pc + 1LL);
                    }
                    /* pass */
                    _pci = (_pci + 1LL);
                }
                /* pass */
                _decl_key = _tr_str_concat(_tr_str_concat(_tr_str_concat(_decl_key, "_"), _tr_int_to_str((long long)(_pc))), "arg");
            }
            /* pass */
            Sema_declare(self, _decl_key, SymbolKind_make_SFunction(), _m_ret, false);
            /* pass */
            if (_tr_dict_contains(self->classes, target)) {
                /* pass */
                List_ptr_append(((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, target))->methods, f);
            }
            /* pass */
            hi = (hi + 1LL);
        }
    } else if (_t133.tag == Decl_DExtern) {
        __auto_type abi = _t133.data.DExtern.abi;
__auto_type functions = _t133.data.DExtern.functions;
        /* pass */
        long long ei = 0LL;
        /* pass */
        while ((ei < functions->len)) {
            /* pass */
            FunctionDef* f = ((FunctionDef*)List_ptr_get(functions, ei));
            /* pass */
            AstType** _e_ret = box_asttype(AstType_init("void"));
            /* pass */
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                _e_ret = f->ret_ty;
            }
            /* pass */
            Sema_declare(self, f->name, SymbolKind_make_SFunction(), _e_ret, false);
            /* pass */
            ei = (ei + 1LL);
        }
    } else if (_t133.tag == Decl_DDecoratorDef) {
        __auto_type f = _t133.data.DDecoratorDef.func;
        /* pass */
        _tr_dict_set(self->decorator_names, f->name, true);
        /* pass */
        Sema_declare(self, f->name, SymbolKind_make_SFunction(), box_asttype(AstType_init("void")), false);
    } else if (_t133.tag == Decl_DTypeAlias) {
        __auto_type alias_name = _t133.data.DTypeAlias.name;
__auto_type target_ty = _t133.data.DTypeAlias.target;
        /* pass */
        AstType** alias_ty = box_asttype(AstType_init("void"));
        /* pass */
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            alias_ty = target_ty;
        }
        /* pass */
        Sema_declare(self, alias_name, SymbolKind_make_SClass(), alias_ty, false);
        /* pass */
        char* resolved_name = "";
        /* pass */
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            resolved_name = (*target_ty)->name;
        }
        /* pass */
        if ((_tr_strlen((char*)resolved_name) > 0LL)) {
            /* pass */
            _tr_dict_set(self->type_aliases, alias_name, resolved_name);
            /* pass */
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                AstType* ta_t = (*target_ty);
                /* pass */
                if ((ta_t->args->len > 0LL)) {
                    /* pass */
                    _tr_dict_set(self->type_alias_elem, alias_name, (*((AstType**)List_ptr_get(ta_t->args, 0LL)))->name);
                }
            }
        }
    } else if (1) {
        __auto_type _ = _t133;
        /* pass */
    }
}

__attribute__((hot)) HirFunction* Sema_lower_func(Sema* self, FunctionDef* f) {
    /* pass */
    self->current_line = f->line;
    /* pass */
    if ((((strcmp((char*)f->name, (char*)"main") != 0) && (strcmp((char*)f->name, (char*)"") != 0)) && (_tr_strlen(f->name) > 1LL))) {
        /* pass */
        char* fn_cat = Sema_is_reserved_error(self, f->name);
        /* pass */
        if ((strcmp((char*)fn_cat, (char*)"") != 0)) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[N-1] '", f->name), "' is a "), fn_cat), " and cannot be used as a function name."));
        } else if ((strcmp((char*)self->current_class_name, (char*)"") == 0)) {
            /* pass */
            char* fn_kcat = Sema_is_reserved_keyword(self, f->name);
            /* pass */
            if ((strcmp((char*)fn_kcat, (char*)"") != 0)) {
                /* pass */
                Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[N-1] '", f->name), "' is a "), fn_kcat), " and is reserved. Choose a different function name."));
            }
        }
    }
    /* pass */
    bool saved_async = self->in_async_fn;
    /* pass */
    self->in_async_fn = f->is_async;
    /* pass */
    self->current_func_name = f->name;
    /* pass */
    List_str* saved_func_generics = self->current_func_generics;
    /* pass */
    self->current_func_generics = f->generics;
    /* pass */
    self->container_borrows = _tr_dict_new(16LL);
    /* pass */
    char* saved_ret_from = self->current_func_ret_from;
    /* pass */
    self->current_func_ret_from = "";
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        self->current_func_ret_from = (*f->ret_ty)->from_param;
    }
    /* pass */
    if (((strcmp((char*)self->current_func_ret_from, (char*)"") == 0) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
        /* pass */
        char* infer_ret_nm = (*f->ret_ty)->name;
        /* pass */
        if ((((strcmp((char*)infer_ret_nm, (char*)"Pointer") == 0) || (strcmp((char*)infer_ret_nm, (char*)"ref") == 0)) || (strcmp((char*)infer_ret_nm, (char*)"mut_ref") == 0))) {
            /* pass */
            char* infer_from = "";
            /* pass */
            long long infer_count = 0LL;
            /* pass */
            long long infer_i = 0LL;
            /* pass */
            while ((infer_i < f->params->len)) {
                /* pass */
                Param* infer_p = ((Param*)List_ptr_get(f->params, infer_i));
                /* pass */
                if (((strcmp((char*)infer_p->name, (char*)"self") != 0) && (((unsigned long long)(infer_p->ty)) != ((unsigned long long)(0LL))))) {
                    /* pass */
                    AstType* infer_pty = (*infer_p->ty);
                    /* pass */
                    if ((!Sema_is_primitive_name(self, infer_pty->name))) {
                        /* pass */
                        infer_from = infer_p->name;
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
                self->current_func_ret_from = infer_from;
            }
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
        AstType* p_ty = AstType_init("int");
        /* pass */
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            p_ty = (*p->ty);
        } else if (((strcmp((char*)p->name, (char*)"self") == 0) && (strcmp((char*)self->current_class_name, (char*)"") != 0))) {
            /* pass */
            p_ty = AstType_init(self->current_class_name);
        }
        /* pass */
        Sema_declare(self, p->name, SymbolKind_make_SVariable(), box_asttype(p_ty), false);
        /* pass */
        if ((self->scopes->len > 0LL)) {
            /* pass */
            Scope* pb_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
            /* pass */
            if (_tr_dict_contains(pb_scope->variables, p->name)) {
                /* pass */
                Symbol* pb_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pb_scope->variables, p->name));
                /* pass */
                pb_sym->is_param = true;
                /* pass */
                pb_sym->ptr_region = 2LL;
                /* pass */
                _tr_dict_set(pb_scope->variables, p->name, pb_sym);
            }
        }
        /* pass */
        HirParam* hp = ((HirParam*)_tr_checked_alloc(sizeof(HirParam)));
        /* pass */
        hp->name = p->name;
        /* pass */
        hp->ty = p_ty;
        /* pass */
        List_ptr_append(hparams, hp);
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    HirFunction* hf = ((HirFunction*)_tr_checked_alloc(sizeof(HirFunction)));
    /* pass */
    hf->name = f->name;
    /* pass */
    hf->class_name = self->current_class_name;
    /* pass */
    hf->generics = f->generics;
    /* pass */
    hf->params = hparams;
    /* pass */
    hf->ret_ty = AstType_init("None");
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        hf->ret_ty = (*f->ret_ty);
    }
    /* pass */
    hf->throws_ty = AstType_init("");
    /* pass */
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        hf->throws_ty = (*f->throws_ty);
    }
    /* pass */
    hf->body = Sema_lower_block(self, f->body);
    /* pass */
    hf->decorators = f->decorators;
    /* pass */
    hf->is_async = f->is_async;
    /* pass */
    hf->is_extern = f->is_extern;
    /* pass */
    hf->is_public = f->is_public;
    /* pass */
    bool _has_self = false;
    /* pass */
    long long _si = 0LL;
    /* pass */
    while ((_si < hparams->len)) {
        /* pass */
        if ((strcmp((char*)((HirParam*)List_ptr_get(hparams, _si))->name, (char*)"self") == 0)) {
            /* pass */
            _has_self = true;
        }
        /* pass */
        _si = (_si + 1LL);
    }
    /* pass */
    hf->is_static = ((strcmp((char*)self->current_class_name, (char*)"") != 0) && (!_has_self));
    /* pass */
    hf->is_variadic = f->is_variadic;
    /* pass */
    hf->is_decorator = false;
    /* pass */
    if ((((!f->is_extern) && (f->body->stmts->len > 0LL)) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
        /* pass */
        char* ret_nm = (*f->ret_ty)->name;
        /* pass */
        if ((((((strcmp((char*)ret_nm, (char*)"void") != 0) && (strcmp((char*)ret_nm, (char*)"None") != 0)) && (strcmp((char*)ret_nm, (char*)"") != 0)) && (strcmp((char*)f->name, (char*)"init") != 0)) && (strcmp((char*)f->name, (char*)"new") != 0))) {
            /* pass */
            if ((!Sema_block_returns(self, f->body))) {
                /* pass */
                self->current_line = f->line;
                /* pass */
                Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[F-3] Function '", f->name), "' returns '"), ret_nm), "' but is missing a return statement on at least one code path. FIX: Add a return at the end, or ensure all if/elif/else branches return."));
            }
        }
    }
    /* pass */
    Sema_exit_scope(self);
    /* pass */
    self->in_async_fn = saved_async;
    /* pass */
    self->current_func_name = "";
    /* pass */
    self->current_func_generics = saved_func_generics;
    /* pass */
    self->current_func_ret_from = saved_ret_from;
    /* pass */
    return hf;
}

__attribute__((hot)) HirClass* Sema_lower_class(Sema* self, ClassDef* c) {
    /* pass */
    self->current_line = c->line;
    /* pass */
    self->current_class_name = c->name;
    /* pass */
    List_ptr* hfields = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c->fields->len)) {
        /* pass */
        FieldDef* f = ((FieldDef*)List_ptr_get(c->fields, i));
        /* pass */
        HirField* hf = ((HirField*)_tr_checked_alloc(sizeof(HirField)));
        /* pass */
        hf->name = f->name;
        /* pass */
        AstType* f_ty = AstType_init("int");
        /* pass */
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            f_ty = (*f->ty);
        }
        /* pass */
        hf->ty = f_ty;
        /* pass */
        List_ptr_append(hfields, hf);
        /* pass */
        i = (i + 1LL);
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
            if (((strcmp((char*)pf_ty->name, (char*)"Shared") == 0) && (pf_ty->args->len > 0LL))) {
                /* pass */
                AstType* pf_inner = (*((AstType**)List_ptr_get(pf_ty->args, 0LL)));
                /* pass */
                if ((strcmp((char*)pf_inner->name, (char*)c->name) == 0)) {
                    /* pass */
                    Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[S-1] '", c->name), "' has a 'Shared["), c->name), "]' field '"), pf_f->name), "' - this creates a reference cycle that leaks memory.\n      FIX: Use 'Weak["), c->name), "]' for back-references to break the cycle."));
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
        List_ptr_append(hmethods, hm);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long _ifc_i = 0LL;
    /* pass */
    while ((_ifc_i < c->iface_names->len)) {
        /* pass */
        char* _ifc_nm = List_str_get(c->iface_names, _ifc_i);
        /* pass */
        if ((strcmp((char*)_ifc_nm, (char*)"Sendable") == 0)) {
            /* pass */
            if ((c->generics->len == 0LL)) {
                /* pass */
                Sema_check_class_sendable_fields(self, c);
            }
        } else if ((!_tr_dict_contains(self->interfaces, _ifc_nm))) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[I-1] Class '", c->name), "' declares 'implements "), _ifc_nm), "' but interface '"), _ifc_nm), "' is not defined.\n      FIX: Define 'interface "), _ifc_nm), ":' before this class, or check for typos."));
        } else {
            /* pass */
            InterfaceDef* _idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _ifc_nm));
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
                    if ((strcmp((char*)_cmeth->name, (char*)_imeth->name) == 0)) {
                        /* pass */
                        _found = true;
                        /* pass */
                        char* _iret = "void";
                        /* pass */
                        if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            _iret = (*_imeth->ret_ty)->name;
                        }
                        /* pass */
                        char* _cret = "void";
                        /* pass */
                        if ((((unsigned long long)(_cmeth->ret_ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            _cret = (*_cmeth->ret_ty)->name;
                        }
                        /* pass */
                        bool _iret_is_generic = ((((_ifc_i >= 0LL) && (_idef->generics->len > 0LL)) && (strcmp((char*)_iret, (char*)"void") != 0)) && (strcmp((char*)_iret, (char*)"") != 0));
                        /* pass */
                        long long _gi = 0LL;
                        /* pass */
                        while ((_gi < _idef->generics->len)) {
                            /* pass */
                            if ((strcmp((char*)List_str_get(_idef->generics, _gi), (char*)_iret) == 0)) {
                                /* pass */
                                _iret_is_generic = true;
                            }
                            /* pass */
                            _gi = (_gi + 1LL);
                        }
                        /* pass */
                        if (((((!_iret_is_generic) && (strcmp((char*)_iret, (char*)"void") != 0)) && (strcmp((char*)_iret, (char*)"") != 0)) && (strcmp((char*)_cret, (char*)_iret) != 0))) {
                            /* pass */
                            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[I-3] Class '", c->name), "': method '"), _cmeth->name), "' returns '"), _cret), "' but interface '"), _ifc_nm), "' declares '-> "), _iret), "'.\n      FIX: Change return type to '-> "), _iret), "'."));
                        }
                        /* pass */
                        long long _ip_cnt = 0LL;
                        /* pass */
                        long long _ip_i = 0LL;
                        /* pass */
                        while ((_ip_i < _imeth->params->len)) {
                            /* pass */
                            if ((strcmp((char*)((Param*)List_ptr_get(_imeth->params, _ip_i))->name, (char*)"self") != 0)) {
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
                            if ((strcmp((char*)((Param*)List_ptr_get(_cmeth->params, _cp_i))->name, (char*)"self") != 0)) {
                                /* pass */
                                _cp_cnt = (_cp_cnt + 1LL);
                            }
                            /* pass */
                            _cp_i = (_cp_i + 1LL);
                        }
                        /* pass */
                        if ((_ip_cnt != _cp_cnt)) {
                            /* pass */
                            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[I-3] Class '", c->name), "': method '"), _cmeth->name), "' has "), _tr_int_to_str((long long)(_cp_cnt))), " parameter(s) but interface '"), _ifc_nm), "' requires "), _tr_int_to_str((long long)(_ip_cnt))), ".\n      FIX: Match the parameter list in '"), _ifc_nm), "' exactly."));
                        }
                    }
                    /* pass */
                    _cm_i = (_cm_i + 1LL);
                }
                /* pass */
                if ((!_found)) {
                    /* pass */
                    char* _sig = _tr_str_concat(_tr_str_concat("pub def ", _imeth->name), "(self");
                    /* pass */
                    long long _pi = 0LL;
                    /* pass */
                    while ((_pi < _imeth->params->len)) {
                        /* pass */
                        Param* _p = ((Param*)List_ptr_get(_imeth->params, _pi));
                        /* pass */
                        if ((strcmp((char*)_p->name, (char*)"self") != 0)) {
                            /* pass */
                            char* _pty = "int";
                            /* pass */
                            if ((((unsigned long long)(_p->ty)) != ((unsigned long long)(0LL)))) {
                                /* pass */
                                _pty = (*_p->ty)->name;
                            }
                            /* pass */
                            _sig = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_sig, ", "), _p->name), ": "), _pty);
                        }
                        /* pass */
                        _pi = (_pi + 1LL);
                    }
                    /* pass */
                    char* _iret2 = "void";
                    /* pass */
                    if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        _iret2 = (*_imeth->ret_ty)->name;
                    }
                    /* pass */
                    _sig = _tr_str_concat(_tr_str_concat(_tr_str_concat(_sig, ") -> "), _iret2), ":");
                    /* pass */
                    Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[I-2] Class '", c->name), "' implements '"), _ifc_nm), "' but is missing method '"), _imeth->name), "'.\n      FIX: Add to 'extend "), c->name), ":'  "), _sig));
                }
                /* pass */
                _im_i = (_im_i + 1LL);
            }
        }
        /* pass */
        _ifc_i = (_ifc_i + 1LL);
    }
    /* pass */
    HirClass* hc = ((HirClass*)_tr_checked_alloc(sizeof(HirClass)));
    /* pass */
    hc->name = c->name;
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
    hc->decorators = c->decorators;
    /* pass */
    hc->is_public = c->is_public;
    /* pass */
    hc->is_class = c->is_class;
    /* pass */
    self->current_class_name = "";
    /* pass */
    return hc;
}

__attribute__((hot)) HirEnum* Sema_lower_enum(Sema* self, EnumDef* e) {
    /* pass */
    self->current_class_name = e->name;
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
            HirParam* hp = ((HirParam*)_tr_checked_alloc(sizeof(HirParam)));
            /* pass */
            hp->name = p->name;
            /* pass */
            AstType* p_ty = AstType_init("int");
            /* pass */
            if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                p_ty = (*p->ty);
            }
            /* pass */
            hp->ty = p_ty;
            /* pass */
            List_ptr_append(hfields, hp);
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        HirVariant* hv = ((HirVariant*)_tr_checked_alloc(sizeof(HirVariant)));
        /* pass */
        hv->name = v->name;
        /* pass */
        hv->fields = hfields;
        /* pass */
        List_ptr_append(hvariants, hv);
        /* pass */
        i = (i + 1LL);
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
        List_ptr_append(hmethods, hm);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    HirEnum* he = ((HirEnum*)_tr_checked_alloc(sizeof(HirEnum)));
    /* pass */
    he->name = e->name;
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
    self->current_class_name = "";
    /* pass */
    return he;
}

__attribute__((hot)) HirInterface* Sema_lower_interface(Sema* self, InterfaceDef* i_def) {
    /* pass */
    List_ptr* hmethods = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < i_def->methods->len)) {
        /* pass */
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(i_def->methods, i)));
        /* pass */
        List_ptr_append(hmethods, hm);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    HirInterface* hi = ((HirInterface*)_tr_checked_alloc(sizeof(HirInterface)));
    /* pass */
    hi->name = i_def->name;
    /* pass */
    hi->generics = i_def->generics;
    /* pass */
    hi->methods = hmethods;
    /* pass */
    return hi;
}

__attribute__((hot)) HirBlock* Sema_lower_block(Sema* self, Block* b) {
    /* pass */
    HirBlock* hb = HirBlock_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        HirStmt* _hs = Sema_lower_stmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
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
    __auto_type _t134 = s;
    if (_t134.tag == Stmt_SExpr) {
        __auto_type e = _t134.data.SExpr.expr;
        /* pass */
        HirStmt* h_s_expr = box_hirstmt(HirStmt_ctor_SExpr(Sema_lower_expr(self, e)));
        /* pass */
        if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t135 = (*e);
            if (_t135.tag == Expr_ECall) {
                __auto_type callee = _t135.data.ECall.callee;
__auto_type args = _t135.data.ECall.args;
                /* pass */
                if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    __auto_type _t136 = (*callee);
                    if (_t136.tag == Expr_EIdent) {
                        __auto_type fn_name = _t136.data.EIdent.name;
                        /* pass */
                        Symbol* fn_sym = Sema_resolve(self, fn_name);
                        /* pass */
                        if (((fn_sym->kind.tag == SymbolKind_make_SFunction().tag) && (strcmp((char*)(*fn_sym->ty)->name, (char*)"Result") == 0))) {
                            /* pass */
                            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-4] '", fn_name), "()' returns a Result and its error must be handled. FIX: Assign the result and match on it, use '?' to propagate, or '_ = "), fn_name), "(...)' to explicitly discard."));
                        }
                    } else if (1) {
                        __auto_type _ = _t136;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t135;
                /* pass */
            }
        }
        /* pass */
        return h_s_expr;
    } else if (_t134.tag == Stmt_SReturn) {
        __auto_type e = _t134.data.SReturn.val;
        /* pass */
        if ((((((unsigned long long)(e)) != ((unsigned long long)(0LL))) && (!self->in_unsafe)) && (strcmp((char*)self->current_func_ret_from, (char*)"") == 0))) {
            /* pass */
            __auto_type _t137 = (*e);
            if (_t137.tag == Expr_EIdent) {
                __auto_type ret_name = _t137.data.EIdent.name;
                /* pass */
                Symbol* ret_sym = Sema_resolve(self, ret_name);
                /* pass */
                if (((strcmp((char*)ret_sym->name, (char*)"") != 0) && (ret_sym->ptr_region == 0LL))) {
                    /* pass */
                    if ((((unsigned long long)(ret_sym->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        if ((strcmp((char*)(*ret_sym->ty)->name, (char*)"Pointer") == 0)) {
                            /* pass */
                            Sema_error(self, _tr_str_concat(_tr_str_concat("[L-1] '", ret_name), "' is a local Pointer that may not outlive this function call. Returning it is unsafe.\n      FIX: Annotate the return type with 'from <param>' if the pointer borrows from a parameter, or wrap the allocation in 'unsafe:' if it is heap-allocated."));
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t137;
                /* pass */
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SReturn(Sema_lower_expr(self, e)));
    } else if (_t134.tag == Stmt_SLet) {
        __auto_type name = _t134.data.SLet.name;
__auto_type ownership = _t134.data.SLet.ownership;
__auto_type is_mut = _t134.data.SLet.is_mut;
__auto_type is_const = _t134.data.SLet.is_const;
__auto_type is_shared = _t134.data.SLet.is_shared;
__auto_type ty_ptr = _t134.data.SLet.ty;
__auto_type val_ptr = _t134.data.SLet.val;
        /* pass */
        if (((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))))) {
            /* pass */
            __auto_type _t138 = (*val_ptr);
            if (_t138.tag == Expr_ELitNone) {
                /* pass */
                char* m7_ty_name = (*ty_ptr)->name;
                /* pass */
                if (((((strcmp((char*)m7_ty_name, (char*)"Option") != 0) && (strcmp((char*)m7_ty_name, (char*)"None") != 0)) && (strcmp((char*)m7_ty_name, (char*)"void") != 0)) && (strcmp((char*)m7_ty_name, (char*)"") != 0))) {
                    /* pass */
                    Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[M-7] Cannot assign 'none' to '", name), "' which has type '"), m7_ty_name), "'. Only Option[T] can hold 'none'. FIX: Use 'Option["), m7_ty_name), "]' as the type, or give '"), name), "' a real initial value."));
                }
            } else if (1) {
                __auto_type _ = _t138;
                /* pass */
            }
        }
        /* pass */
        AstType* ty = AstType_init("void");
        /* pass */
        if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            ty = (*ty_ptr);
        }
        /* pass */
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
        /* pass */
        if (((strcmp((char*)ty->name, (char*)"void") == 0) || (strcmp((char*)ty->name, (char*)"None") == 0))) {
            /* pass */
            ty = hir_expr_type(hval);
        }
        /* pass */
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && (!is_shared))) {
            /* pass */
            __auto_type _t139 = (*val_ptr);
            if (_t139.tag == Expr_EIdent) {
                __auto_type m1_src = _t139.data.EIdent.name;
                /* pass */
                Symbol* m1_sym = Sema_resolve(self, m1_src);
                /* pass */
                AstType* m1_ty = (*m1_sym->ty);
                /* pass */
                if (((((!Sema_is_primitive(self, m1_ty)) && (!Sema_is_copy_class(self, m1_ty->name))) && (strcmp((char*)m1_sym->name, (char*)"") != 0)) && (!m1_sym->is_shared))) {
                    /* pass */
                    Sema_check_not_moved(self, m1_src, m1_ty->name);
                    /* pass */
                    Sema_check_no_active_borrows(self, m1_src, m1_ty->name);
                    /* pass */
                    Sema_mark_moved(self, m1_src);
                }
            } else if (1) {
                __auto_type _ = _t139;
                /* pass */
            }
        }
        /* pass */
        Sema_declare(self, name, SymbolKind_make_SVariable(), box_asttype(ty), is_mut);
        /* pass */
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && (strcmp((char*)ty->name, (char*)"Pointer") == 0))) {
            /* pass */
            bool heap_rhs = false;
            /* pass */
            __auto_type _t140 = (*val_ptr);
            if (_t140.tag == Expr_ECall) {
                heap_rhs = true;
            } else if (_t140.tag == Expr_EMethodCall) {
                heap_rhs = true;
            } else if (1) {
                __auto_type _ = _t140;
                /* pass */
            }
            /* pass */
            if (heap_rhs) {
                /* pass */
                if ((self->scopes->len > 0LL)) {
                    /* pass */
                    Scope* alloc_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
                    /* pass */
                    if (_tr_dict_contains(alloc_scope->variables, name)) {
                        /* pass */
                        Symbol* alloc_sym = ((Symbol*)(uintptr_t)_tr_dict_get(alloc_scope->variables, name));
                        /* pass */
                        alloc_sym->ptr_region = 1LL;
                        /* pass */
                        _tr_dict_set(alloc_scope->variables, name, alloc_sym);
                    }
                }
            }
        }
        /* pass */
        if ((((unsigned long long)(val_ptr)) == ((unsigned long long)(0LL)))) {
            /* pass */
            if ((self->scopes->len > 0LL)) {
                /* pass */
                Scope* pd_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
                /* pass */
                if (_tr_dict_contains(pd_scope->variables, name)) {
                    /* pass */
                    Symbol* pd_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pd_scope->variables, name));
                    /* pass */
                    pd_sym->is_init = false;
                    /* pass */
                    _tr_dict_set(pd_scope->variables, name, pd_sym);
                }
            }
        }
        /* pass */
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            char* pc_cont_nm = "";
            /* pass */
            char* pc_meth = "";
            /* pass */
            __auto_type _t141 = (*val_ptr);
            if (_t141.tag == Expr_EMethodCall) {
                __auto_type pc_obj = _t141.data.EMethodCall.obj;
__auto_type pc_m = _t141.data.EMethodCall.method;
                /* pass */
                pc_meth = pc_m;
                /* pass */
                __auto_type _t142 = (*pc_obj);
                if (_t142.tag == Expr_EIdent) {
                    __auto_type pc_src = _t142.data.EIdent.name;
                    pc_cont_nm = pc_src;
                } else if (1) {
                    __auto_type _ = _t142;
                    /* pass */
                }
            } else if (1) {
                __auto_type _ = _t141;
                /* pass */
            }
            /* pass */
            if (((strcmp((char*)pc_cont_nm, (char*)"") != 0) && (((strcmp((char*)pc_meth, (char*)"get") == 0) || (strcmp((char*)pc_meth, (char*)"first") == 0)) || (strcmp((char*)pc_meth, (char*)"last") == 0)))) {
                /* pass */
                Symbol* pc_cont_sym = Sema_resolve(self, pc_cont_nm);
                /* pass */
                AstType* pc_cont_ty = (*pc_cont_sym->ty);
                /* pass */
                if (((strcmp((char*)pc_cont_ty->name, (char*)"Vec") == 0) || (strcmp((char*)pc_cont_ty->name, (char*)"List") == 0))) {
                    /* pass */
                    if ((!Sema_is_primitive(self, hir_expr_type(hval)))) {
                        /* pass */
                        _tr_dict_set(self->container_borrows, pc_cont_nm, name);
                    }
                }
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SLet(name, ownership, is_mut, is_const, is_shared, ty, hval));
    } else if (_t134.tag == Stmt_SAssign) {
        __auto_type target = _t134.data.SAssign.target;
__auto_type val = _t134.data.SAssign.val;
        /* pass */
        self->in_assign_target = true;
        /* pass */
        HirExpr* htgt = Sema_lower_expr(self, target);
        /* pass */
        self->in_assign_target = false;
        /* pass */
        HirExpr* hv = Sema_lower_expr(self, val);
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t143 = (*target);
            if (_t143.tag == Expr_EIdent) {
                __auto_type sa_decl_name = _t143.data.EIdent.name;
                /* pass */
                if ((strcmp((char*)Sema_resolve(self, sa_decl_name)->name, (char*)"") == 0)) {
                    /* pass */
                    Sema_declare(self, sa_decl_name, SymbolKind_make_SVariable(), box_asttype(hir_expr_type(hv)), true);
                }
            } else if (1) {
                __auto_type _ = _t143;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t144 = (*val);
            if (_t144.tag == Expr_EIdent) {
                __auto_type sa_src = _t144.data.EIdent.name;
                /* pass */
                Symbol* sa_sym = Sema_resolve(self, sa_src);
                /* pass */
                AstType* sa_ty = (*sa_sym->ty);
                /* pass */
                bool sa_is_known = (_tr_dict_contains(self->classes, sa_ty->name) || _tr_dict_contains(self->enums, sa_ty->name));
                /* pass */
                if (((((sa_is_known && (!Sema_is_primitive(self, sa_ty))) && (!Sema_is_copy_class(self, sa_ty->name))) && (strcmp((char*)sa_sym->name, (char*)"") != 0)) && (!sa_sym->is_shared))) {
                    /* pass */
                    Sema_check_no_active_borrows(self, sa_src, sa_ty->name);
                    /* pass */
                    Sema_mark_moved(self, sa_src);
                }
            } else if (1) {
                __auto_type _ = _t144;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t145 = (*target);
            if (_t145.tag == Expr_EIdent) {
                __auto_type pd_tgt = _t145.data.EIdent.name;
                Sema_mark_init(self, pd_tgt);
            } else if (1) {
                __auto_type _ = _t145;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t146 = (*target);
            if (_t146.tag == Expr_EIdent) {
                __auto_type pc_tgt = _t146.data.EIdent.name;
                Sema_clear_container_borrow(self, pc_tgt);
            } else if (1) {
                __auto_type _ = _t146;
                /* pass */
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SAssign(htgt, hv));
    } else if (_t134.tag == Stmt_SIf) {
        __auto_type cond = _t134.data.SIf.cond;
__auto_type then_b = _t134.data.SIf.then_b;
__auto_type elifs = _t134.data.SIf.elifs;
__auto_type else_b = _t134.data.SIf.else_b;
        /* pass */
        if ((((unsigned long long)(cond)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t147 = (*cond);
            if (_t147.tag == Expr_EIdent) {
                __auto_type t5_name = _t147.data.EIdent.name;
                /* pass */
                Symbol* t5_sym = Sema_resolve(self, t5_name);
                /* pass */
                char* t5_ty = (*t5_sym->ty)->name;
                /* pass */
                if ((((((strcmp((char*)t5_ty, (char*)"int") == 0) || (strcmp((char*)t5_ty, (char*)"i64") == 0)) || (strcmp((char*)t5_ty, (char*)"i32") == 0)) || (strcmp((char*)t5_ty, (char*)"float") == 0)) || (strcmp((char*)t5_ty, (char*)"f64") == 0))) {
                    /* pass */
                    Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-5] '", t5_name), "' is a number ("), t5_ty), ") and cannot be used as an 'if' condition. FIX: Write 'if "), t5_name), " != 0:' to explicitly check for non-zero."));
                }
            } else if (1) {
                __auto_type _ = _t147;
                /* pass */
            }
        }
        /* pass */
        bool si_outer_cap_m = self->capturing_moves;
        /* pass */
        List_str* si_outer_buf_m = self->branch_moved_buf;
        /* pass */
        bool si_outer_cap_i = self->capturing_inits;
        /* pass */
        List_str* si_outer_buf_i = self->branch_init_buf;
        /* pass */
        self->capturing_moves = true;
        /* pass */
        self->capturing_inits = true;
        /* pass */
        self->branch_moved_buf = (void*)List_str_new();
        /* pass */
        self->branch_init_buf = (void*)List_str_new();
        /* pass */
        HirExpr* hcond = Sema_lower_expr(self, cond);
        /* pass */
        HirBlock* hthen = Sema_lower_block(self, then_b);
        /* pass */
        List_str* si_then_moved = self->branch_moved_buf;
        /* pass */
        List_str* si_then_inited = self->branch_init_buf;
        /* pass */
        long long si_uti = 0LL;
        /* pass */
        while ((si_uti < si_then_moved->len)) {
            /* pass */
            Sema_unmark_moved(self, List_str_get(si_then_moved, si_uti));
            /* pass */
            si_uti = (si_uti + 1LL);
        }
        /* pass */
        long long si_uii = 0LL;
        /* pass */
        while ((si_uii < si_then_inited->len)) {
            /* pass */
            Sema_unmark_init(self, List_str_get(si_then_inited, si_uii));
            /* pass */
            si_uii = (si_uii + 1LL);
        }
        /* pass */
        self->branch_moved_buf = (void*)List_str_new();
        /* pass */
        self->branch_init_buf = (void*)List_str_new();
        /* pass */
        HirBlock* helse = HirBlock_init();
        /* pass */
        if ((elifs->len > 0LL)) {
            /* pass */
            HirBlock* base_else = Sema_lower_block(self, else_b);
            /* pass */
            HirBlock* chain = base_else;
            /* pass */
            long long k = (elifs->len - 1LL);
            /* pass */
            while ((k >= 0LL)) {
                /* pass */
                ElifClause* elif_c = ((ElifClause*)List_ptr_get(elifs, k));
                /* pass */
                HirExpr* elif_cond = Sema_lower_expr(self, elif_c->cond);
                /* pass */
                HirBlock* elif_body = Sema_lower_block(self, (*elif_c->body));
                /* pass */
                HirBlock* nested = HirBlock_init();
                /* pass */
                HirBlock_push(nested, box_hirstmt(HirStmt_ctor_SIf(elif_cond, elif_body, chain)));
                /* pass */
                chain = nested;
                /* pass */
                k = (k - 1LL);
            }
            /* pass */
            helse = chain;
        } else {
            /* pass */
            helse = Sema_lower_block(self, else_b);
        }
        /* pass */
        List_str* si_else_moved = self->branch_moved_buf;
        /* pass */
        List_str* si_else_inited = self->branch_init_buf;
        /* pass */
        long long si_uei = 0LL;
        /* pass */
        while ((si_uei < si_else_moved->len)) {
            /* pass */
            Sema_unmark_moved(self, List_str_get(si_else_moved, si_uei));
            /* pass */
            si_uei = (si_uei + 1LL);
        }
        /* pass */
        long long si_uei2 = 0LL;
        /* pass */
        while ((si_uei2 < si_else_inited->len)) {
            /* pass */
            Sema_unmark_init(self, List_str_get(si_else_inited, si_uei2));
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
        long long si_mi = 0LL;
        /* pass */
        while ((si_mi < si_then_moved->len)) {
            /* pass */
            char* si_mn = List_str_get(si_then_moved, si_mi);
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
        }
        /* pass */
        long long si_ei = 0LL;
        /* pass */
        while ((si_ei < si_else_moved->len)) {
            /* pass */
            char* si_en = List_str_get(si_else_moved, si_ei);
            /* pass */
            if ((!Sema_vec_str_contains(self, si_then_moved, si_en))) {
                /* pass */
                Sema_mark_maybe_moved(self, si_en);
            }
            /* pass */
            si_ei = (si_ei + 1LL);
        }
        /* pass */
        long long si_ini = 0LL;
        /* pass */
        while ((si_ini < si_then_inited->len)) {
            /* pass */
            char* si_inn = List_str_get(si_then_inited, si_ini);
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
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SIf(hcond, hthen, helse));
    } else if (_t134.tag == Stmt_SWhile) {
        __auto_type cond = _t134.data.SWhile.cond;
__auto_type body = _t134.data.SWhile.body;
__auto_type decorators = _t134.data.SWhile.decorators;
        /* pass */
        HirExpr* sw_cond = Sema_lower_expr(self, cond);
        /* pass */
        bool sw_outer_cap_m = self->capturing_moves;
        /* pass */
        List_str* sw_outer_buf_m = self->branch_moved_buf;
        /* pass */
        bool sw_outer_cap_i = self->capturing_inits;
        /* pass */
        List_str* sw_outer_buf_i = self->branch_init_buf;
        /* pass */
        self->capturing_moves = true;
        /* pass */
        self->capturing_inits = true;
        /* pass */
        self->branch_moved_buf = (void*)List_str_new();
        /* pass */
        self->branch_init_buf = (void*)List_str_new();
        /* pass */
        HirBlock* sw_body = Sema_lower_block(self, body);
        /* pass */
        List_str* sw_loop_moved = self->branch_moved_buf;
        /* pass */
        List_str* sw_loop_inited = self->branch_init_buf;
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
            Sema_unmark_moved(self, List_str_get(sw_loop_moved, sw_mi));
            /* pass */
            Sema_mark_maybe_moved(self, List_str_get(sw_loop_moved, sw_mi));
            /* pass */
            sw_mi = (sw_mi + 1LL);
        }
        /* pass */
        long long sw_ii = 0LL;
        /* pass */
        while ((sw_ii < sw_loop_inited->len)) {
            /* pass */
            Sema_unmark_init(self, List_str_get(sw_loop_inited, sw_ii));
            /* pass */
            Sema_mark_maybe_init(self, List_str_get(sw_loop_inited, sw_ii));
            /* pass */
            sw_ii = (sw_ii + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SWhile(sw_cond, sw_body));
    } else if (_t134.tag == Stmt_SFor) {
        __auto_type var = _t134.data.SFor.var;
__auto_type iter = _t134.data.SFor.iter;
__auto_type body = _t134.data.SFor.body;
__auto_type decorators = _t134.data.SFor.decorators;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        HirExpr* h_iter_for = Sema_lower_expr(self, iter);
        /* pass */
        AstType* var_ty_for = AstType_init("int");
        /* pass */
        char* iter_hn = hir_expr_type(h_iter_for)->name;
        /* pass */
        long long iter_hal = hir_expr_type(h_iter_for)->args->len;
        /* pass */
        if ((((strcmp((char*)iter_hn, (char*)"List") == 0) || (strcmp((char*)iter_hn, (char*)"Vec") == 0)) && (iter_hal > 0LL))) {
            /* pass */
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if (((strcmp((char*)iter_hn, (char*)"Chan") == 0) && (iter_hal > 0LL))) {
            /* pass */
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if ((strcmp((char*)iter_hn, (char*)"str") == 0)) {
            /* pass */
            var_ty_for = AstType_init("char");
        }
        /* pass */
        Sema_declare(self, var, SymbolKind_make_SVariable(), box_asttype(var_ty_for), false);
        /* pass */
        bool sf_outer_cap_m = self->capturing_moves;
        /* pass */
        List_str* sf_outer_buf_m = self->branch_moved_buf;
        /* pass */
        bool sf_outer_cap_i = self->capturing_inits;
        /* pass */
        List_str* sf_outer_buf_i = self->branch_init_buf;
        /* pass */
        self->capturing_moves = true;
        /* pass */
        self->capturing_inits = true;
        /* pass */
        self->branch_moved_buf = (void*)List_str_new();
        /* pass */
        self->branch_init_buf = (void*)List_str_new();
        /* pass */
        HirBlock* sf_body = Sema_lower_block(self, body);
        /* pass */
        List_str* sf_loop_moved = self->branch_moved_buf;
        /* pass */
        List_str* sf_loop_inited = self->branch_init_buf;
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
            Sema_unmark_moved(self, List_str_get(sf_loop_moved, sf_mi));
            /* pass */
            Sema_mark_maybe_moved(self, List_str_get(sf_loop_moved, sf_mi));
            /* pass */
            sf_mi = (sf_mi + 1LL);
        }
        /* pass */
        long long sf_ii = 0LL;
        /* pass */
        while ((sf_ii < sf_loop_inited->len)) {
            /* pass */
            Sema_unmark_init(self, List_str_get(sf_loop_inited, sf_ii));
            /* pass */
            Sema_mark_maybe_init(self, List_str_get(sf_loop_inited, sf_ii));
            /* pass */
            sf_ii = (sf_ii + 1LL);
        }
        /* pass */
        HirStmt* hstmt = box_hirstmt(HirStmt_ctor_SFor(var, h_iter_for, sf_body));
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return hstmt;
    } else if (_t134.tag == Stmt_SForUnpack) {
        __auto_type vars = _t134.data.SForUnpack.vars;
__auto_type iter = _t134.data.SForUnpack.iter;
__auto_type body = _t134.data.SForUnpack.body;
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
            List_ptr_append(fu_tys, AstType_init("int"));
            /* pass */
            fu_ti = (fu_ti + 1LL);
        }
        /* pass */
        __auto_type _t148 = (*h_iter_fu);
        if (_t148.tag == HirExpr_ECall) {
            __auto_type fu_callee = _t148.data.ECall.callee;
__auto_type fu_args = _t148.data.ECall.args;
            /* pass */
            __auto_type _t149 = (*fu_callee);
            if (_t149.tag == HirExpr_EIdent) {
                __auto_type fu_fn = _t149.data.EIdent.name;
                /* pass */
                if ((((strcmp((char*)fu_fn, (char*)"enumerate") == 0) && (fu_args->len == 1LL)) && (vars->len >= 2LL))) {
                    /* pass */
                    char* fu_col_ty_n = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name;
                    /* pass */
                    long long fu_col_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
                    /* pass */
                    if ((((strcmp((char*)fu_col_ty_n, (char*)"List") == 0) || (strcmp((char*)fu_col_ty_n, (char*)"Vec") == 0)) && (fu_col_al > 0LL))) {
                        /* pass */
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
                } else if ((((strcmp((char*)fu_fn, (char*)"zip") == 0) && (fu_args->len == 2LL)) && (vars->len >= 2LL))) {
                    /* pass */
                    char* fu_a_n = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name;
                    /* pass */
                    long long fu_a_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
                    /* pass */
                    char* fu_b_n = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->name;
                    /* pass */
                    long long fu_b_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args->len;
                    /* pass */
                    if ((((strcmp((char*)fu_a_n, (char*)"List") == 0) || (strcmp((char*)fu_a_n, (char*)"Vec") == 0)) && (fu_a_al > 0LL))) {
                        /* pass */
                        List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
                    /* pass */
                    if ((((strcmp((char*)fu_b_n, (char*)"List") == 0) || (strcmp((char*)fu_b_n, (char*)"Vec") == 0)) && (fu_b_al > 0LL))) {
                        /* pass */
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args, 0LL))));
                    }
                }
            } else if (1) {
                __auto_type _ = _t149;
                /* pass */
            }
        } else if (_t148.tag == HirExpr_EMethodCall) {
            __auto_type fu_obj = _t148.data.EMethodCall.obj;
__auto_type fu_meth = _t148.data.EMethodCall.method;
            /* pass */
            if (((strcmp((char*)fu_meth, (char*)"items") == 0) && (vars->len >= 2LL))) {
                /* pass */
                char* fu_dty_n = hir_expr_type(fu_obj)->name;
                /* pass */
                long long fu_dty_al = hir_expr_type(fu_obj)->args->len;
                /* pass */
                if ((((strcmp((char*)fu_dty_n, (char*)"Dict") == 0) || (strcmp((char*)fu_dty_n, (char*)"Map") == 0)) && (fu_dty_al >= 2LL))) {
                    /* pass */
                    List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 0LL))));
                    /* pass */
                    List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 1LL))));
                }
            }
        } else if (1) {
            __auto_type _ = _t148;
            /* pass */
        }
        /* pass */
        long long vi_fu = 0LL;
        /* pass */
        while ((vi_fu < vars->len)) {
            /* pass */
            Sema_declare(self, List_str_get(vars, vi_fu), SymbolKind_make_SVariable(), box_asttype(((AstType*)List_ptr_get(fu_tys, vi_fu))), false);
            /* pass */
            vi_fu = (vi_fu + 1LL);
        }
        /* pass */
        HirBlock* hblk_fu = Sema_lower_block(self, body);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_SForUnpack(vars, h_iter_fu, hblk_fu));
    } else if (_t134.tag == Stmt_SMatch) {
        __auto_type subj = _t134.data.SMatch.expr;
__auto_type arms = _t134.data.SMatch.arms;
        /* pass */
        HirExpr* hsubj = Sema_lower_expr(self, subj);
        /* pass */
        char* ex_ty_name = hir_expr_type(hsubj)->name;
        /* pass */
        List_ptr* h_arms = (void*)List_ptr_new();
        /* pass */
        bool ex_has_wild = false;
        /* pass */
        List_str* ex_covered = (void*)List_str_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < arms->len)) {
            /* pass */
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, k));
            /* pass */
            __auto_type _t150 = arm->pat;
            if (_t150.tag == Pattern_PWild) {
                ex_has_wild = true;
            } else if (_t150.tag == Pattern_PBind) {
                __auto_type _ = _t150.data.PBind.name;
                ex_has_wild = true;
            } else if (_t150.tag == Pattern_PVariant) {
                __auto_type ex_vn = _t150.data.PVariant.variant;
                List_str_append(ex_covered, ex_vn);
            } else if (_t150.tag == Pattern_PVariantBind) {
                __auto_type ex_vn2 = _t150.data.PVariantBind.variant;
                List_str_append(ex_covered, ex_vn2);
            } else if (_t150.tag == Pattern_PVariantBindMany) {
                __auto_type ex_vn3 = _t150.data.PVariantBindMany.variant;
                List_str_append(ex_covered, ex_vn3);
            } else if (1) {
                __auto_type _ = _t150;
                /* pass */
            }
            /* pass */
            Sema_enter_scope(self);
            /* pass */
            AstType* _subj_ty = hir_expr_type(hsubj);
            /* pass */
            Sema_declare_pattern_binds_typed(self, arm->pat, _subj_ty);
            /* pass */
            HirMatchArm* h_arm = HirMatchArm_init(arm->pat, Sema_lower_block(self, (*arm->body)));
            /* pass */
            if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
                /* pass */
                h_arm->guard = Sema_lower_expr(self, arm->guard);
            }
            /* pass */
            List_ptr_append(h_arms, h_arm);
            /* pass */
            Sema_exit_scope(self);
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        if (((!ex_has_wild) && _tr_dict_contains(self->enums, ex_ty_name))) {
            /* pass */
            EnumDef* ex_edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, ex_ty_name));
            /* pass */
            List_str* ex_missing = (void*)List_str_new();
            /* pass */
            long long ex_vi = 0LL;
            /* pass */
            while ((ex_vi < ex_edef->variants->len)) {
                /* pass */
                char* ex_vname = ((VariantDef*)List_ptr_get(ex_edef->variants, ex_vi))->name;
                /* pass */
                if ((!List_str_contains(ex_covered, ex_vname))) {
                    /* pass */
                    List_str_append(ex_missing, ex_vname);
                }
                /* pass */
                ex_vi = (ex_vi + 1LL);
            }
            /* pass */
            if ((ex_missing->len > 0LL)) {
                /* pass */
                char* ex_msg = _tr_str_concat(_tr_str_concat("[E-1] Non-exhaustive match on '", ex_ty_name), "': missing variant");
                /* pass */
                if ((ex_missing->len > 1LL)) {
                    /* pass */
                    ex_msg = _tr_str_concat(ex_msg, "s");
                }
                /* pass */
                ex_msg = _tr_str_concat(ex_msg, ": ");
                /* pass */
                long long ex_mi = 0LL;
                /* pass */
                while ((ex_mi < ex_missing->len)) {
                    /* pass */
                    if ((ex_mi > 0LL)) {
                        /* pass */
                        ex_msg = _tr_str_concat(ex_msg, ", ");
                    }
                    /* pass */
                    ex_msg = _tr_str_concat(ex_msg, List_str_get(ex_missing, ex_mi));
                    /* pass */
                    ex_mi = (ex_mi + 1LL);
                }
                /* pass */
                ex_msg = _tr_str_concat(_tr_str_concat(_tr_str_concat(ex_msg, ".\n      FIX: Add a 'case "), ex_ty_name), ".VariantName:' arm for each missing variant, or add 'case _:' to handle all remaining cases.");
                /* pass */
                Sema_error(self, ex_msg);
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SMatch(hsubj, h_arms));
    } else if (_t134.tag == Stmt_STry) {
        __auto_type try_body = _t134.data.STry.try_body;
__auto_type catches = _t134.data.STry.catches;
__auto_type finally_b = _t134.data.STry.finally_b;
        /* pass */
        List_ptr* h_catches = (void*)List_ptr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < catches->len)) {
            /* pass */
            CatchClause* cc = (*((CatchClause**)List_ptr_get(catches, k)));
            /* pass */
            HirCatchClause** hcc = ((HirCatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(HirCatchClause*)));
            /* pass */
            HirCatchClause* hcc_val = ((HirCatchClause*)_tr_checked_alloc(sizeof(HirCatchClause)));
            /* pass */
            hcc_val->err_name = cc->err_name;
            /* pass */
            hcc_val->err_type = AstType_init("str");
            /* pass */
            if ((((unsigned long long)(cc->err_type)) != ((unsigned long long)(0LL)))) {
                /* pass */
                hcc_val->err_type = (*cc->err_type);
            }
            /* pass */
            hcc_val->body = Sema_lower_block(self, (*cc->body));
            /* pass */
            (*hcc = hcc_val);
            /* pass */
            List_ptr_append(h_catches, hcc);
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_STry(Sema_lower_block(self, try_body), h_catches, Sema_lower_block(self, finally_b)));
    } else if (_t134.tag == Stmt_SRaise) {
        __auto_type e = _t134.data.SRaise.val;
        return box_hirstmt(HirStmt_ctor_SRaise(Sema_lower_expr(self, e)));
    } else if (_t134.tag == Stmt_SAssert) {
        __auto_type cond = _t134.data.SAssert.cond;
__auto_type msg = _t134.data.SAssert.msg;
        return box_hirstmt(HirStmt_ctor_SAssert(Sema_lower_expr(self, cond), Sema_lower_expr(self, msg)));
    } else if (_t134.tag == Stmt_SDefer) {
        __auto_type inner = _t134.data.SDefer.stmt;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SDefer(Sema_lower_stmt(self, inner)));
    } else if (_t134.tag == Stmt_SWith) {
        __auto_type items = _t134.data.SWith.items;
__auto_type aliases = _t134.data.SWith.aliases;
__auto_type body = _t134.data.SWith.body;
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
            if (((k < aliases->len) && (strcmp((char*)List_str_get(aliases, k), (char*)"") != 0))) {
                /* pass */
                AstType* wi_ty = hir_expr_type(h_wi);
                /* pass */
                Sema_declare(self, List_str_get(aliases, k), SymbolKind_make_SVariable(), box_asttype(wi_ty), true);
            }
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        HirBlock* h_with_body = Sema_lower_block(self, body);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_SWith(h_items, aliases, h_with_body));
    } else if (_t134.tag == Stmt_SAsm) {
        __auto_type code = _t134.data.SAsm.code;
__auto_type outputs = _t134.data.SAsm.outputs;
__auto_type inputs = _t134.data.SAsm.inputs;
__auto_type clobbers = _t134.data.SAsm.clobbers;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SAsm(code, outputs, inputs, clobbers));
    } else if (_t134.tag == Stmt_SSpawn) {
        __auto_type e = _t134.data.SSpawn.expr;
        /* pass */
        if ((!self->in_async_fn)) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[C-5] 'spawn:' used outside an async function. FIX: Declare '", self->current_func_name), "' as 'async def "), self->current_func_name), "(...)' to use spawn inside it."));
        }
        /* pass */
        HirExpr* spawn_lowered = Sema_lower_expr(self, e);
        /* pass */
        Sema_check_spawn_sendable(self, spawn_lowered);
        /* pass */
        return box_hirstmt(HirStmt_ctor_SSpawn(spawn_lowered));
    } else if (_t134.tag == Stmt_STaskGroup) {
        __auto_type body = _t134.data.STaskGroup.body;
        /* pass */
        if ((!self->in_async_fn)) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[C-6] 'taskgroup:' used outside an async function. FIX: Declare '", self->current_func_name), "' as 'async def "), self->current_func_name), "(...)' to use taskgroup inside it."));
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_STaskGroup(Sema_lower_block(self, body)));
    } else if (_t134.tag == Stmt_SChanSelect) {
        __auto_type cs_cases = _t134.data.SChanSelect.cases;
        /* pass */
        List_ptr* hcs_cases = (void*)List_ptr_new();
        /* pass */
        long long csi2 = 0LL;
        /* pass */
        while ((csi2 < cs_cases->len)) {
            /* pass */
            ChanSelectArm* arm = (*((ChanSelectArm**)List_ptr_get(cs_cases, csi2)));
            /* pass */
            HirChanSelectArm** harm = ((HirChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(HirChanSelectArm*)));
            /* pass */
            HirChanSelectArm* harm_v = ((HirChanSelectArm*)_tr_checked_alloc(sizeof(HirChanSelectArm)));
            /* pass */
            harm_v->kind = arm->kind;
            /* pass */
            harm_v->var_name = arm->var_name;
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
            if (((arm->kind == 0LL) && (strcmp((char*)arm->var_name, (char*)"") != 0))) {
                /* pass */
                AstType* recv_ty = AstType_init("int");
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
            harm_v->body = Sema_lower_block(self, arm->body);
            /* pass */
            Sema_exit_scope(self);
            /* pass */
            (*harm = harm_v);
            /* pass */
            List_ptr_append(hcs_cases, harm);
            /* pass */
            csi2 = (csi2 + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SChanSelect(hcs_cases));
    } else if (_t134.tag == Stmt_SGpuBlock) {
        __auto_type body = _t134.data.SGpuBlock.body;
        return box_hirstmt(HirStmt_ctor_SGpuBlock(Sema_lower_block(self, body)));
    } else if (_t134.tag == Stmt_SBreak) {
        return box_hirstmt(HirStmt_make_SBreak());
    } else if (_t134.tag == Stmt_SContinue) {
        return box_hirstmt(HirStmt_make_SContinue());
    } else if (_t134.tag == Stmt_SPass) {
        return box_hirstmt(HirStmt_make_SPass());
    } else if (_t134.tag == Stmt_SLocalDecl) {
        __auto_type ldecl = _t134.data.SLocalDecl.decl;
        /* pass */
        if ((strcmp((char*)self->current_func_name, (char*)"main") != 0)) {
            /* pass */
            Sema_error(self, "[E-2] Nested class/def/enum/interface declarations are only supported inside main().\n      FIX: Move this declaration to module (top-level) scope, or declare it inside main().");
            /* pass */
            return box_hirstmt(HirStmt_make_SPass());
        }
        /* pass */
        Sema_register_decl(self, ldecl);
        /* pass */
        char* saved_ld_func_name = self->current_func_name;
        /* pass */
        char* saved_ld_class_name = self->current_class_name;
        /* pass */
        __auto_type _t151 = (*ldecl);
        if (_t151.tag == Decl_DFunction) {
            __auto_type ld_f = _t151.data.DFunction.func;
            /* pass */
            List_ptr_append(self->nested_functions, Sema_lower_func(self, ld_f));
        } else if (_t151.tag == Decl_DClass) {
            __auto_type ld_c = _t151.data.DClass.cls;
            /* pass */
            List_ptr_append(self->nested_classes, Sema_lower_class(self, ld_c));
        } else if (_t151.tag == Decl_DActor) {
            __auto_type ld_c = _t151.data.DActor.cls;
            /* pass */
            List_ptr_append(self->nested_classes, Sema_lower_class(self, ld_c));
        } else if (_t151.tag == Decl_DEnum) {
            __auto_type ld_e = _t151.data.DEnum.enm;
            /* pass */
            List_ptr_append(self->nested_enums, Sema_lower_enum(self, ld_e));
        } else if (_t151.tag == Decl_DInterface) {
            __auto_type ld_i = _t151.data.DInterface.iface;
            /* pass */
            List_ptr_append(self->nested_interfaces, Sema_lower_interface(self, ld_i));
        } else if (_t151.tag == Decl_DExtend) {
            __auto_type ld_target = _t151.data.DExtend.target;
__auto_type ld_methods = _t151.data.DExtend.methods;
            /* pass */
            self->current_class_name = ld_target;
            /* pass */
            long long ld_ci = 0LL;
            /* pass */
            bool ld_found = false;
            /* pass */
            while ((ld_ci < self->nested_classes->len)) {
                /* pass */
                HirClass* ld_nc = ((HirClass*)List_ptr_get(self->nested_classes, ld_ci));
                /* pass */
                if ((strcmp((char*)ld_nc->name, (char*)ld_target) == 0)) {
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
            __auto_type _ = _t151;
            /* pass */
        }
        /* pass */
        self->current_func_name = saved_ld_func_name;
        /* pass */
        self->current_class_name = saved_ld_class_name;
        /* pass */
        return box_hirstmt(HirStmt_make_SPass());
    } else if (_t134.tag == Stmt_SUnsafe) {
        __auto_type body = _t134.data.SUnsafe.body;
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
    } else if (_t134.tag == Stmt_SMultiLet) {
        __auto_type names = _t134.data.SMultiLet.names;
__auto_type is_mut = _t134.data.SMultiLet.is_mut;
__auto_type val_ptr = _t134.data.SMultiLet.val;
        /* pass */
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < names->len)) {
            /* pass */
            Sema_declare(self, List_str_get(names, i), SymbolKind_make_SVariable(), box_asttype(AstType_init("int")), is_mut);
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SMultiLet(names, is_mut, hval));
    } else if (_t134.tag == Stmt_SLine) {
        __auto_type n = _t134.data.SLine.n;
        /* pass */
        self->current_line = n;
        /* pass */
        return box_hirstmt(HirStmt_make_SPass());
    } else if (1) {
        __auto_type _ = _t134;
        return box_hirstmt(HirStmt_make_SPass());
    }
}

__attribute__((hot)) AstType* Sema_variant_field_ty(Sema* self, char* type_name, char* variant_name, long long field_idx) {
    /* pass */
    if (_tr_dict_contains(self->enums, type_name)) {
        /* pass */
        EnumDef* enm = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, type_name));
        /* pass */
        long long vi = 0LL;
        /* pass */
        while ((vi < enm->variants->len)) {
            /* pass */
            VariantDef* v = ((VariantDef*)List_ptr_get(enm->variants, vi));
            /* pass */
            if ((strcmp((char*)v->name, (char*)variant_name) == 0)) {
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
                return AstType_init("void");
            }
            /* pass */
            vi = (vi + 1LL);
        }
    }
    /* pass */
    return AstType_init("void");
}

__attribute__((hot)) void Sema_declare_pattern_binds(Sema* self, Pattern pat) {
    /* pass */
    Sema_declare_pattern_binds_typed(self, pat, AstType_init("int"));
}

__attribute__((hot)) void Sema_declare_pattern_binds_typed(Sema* self, Pattern pat, AstType* subj_ty) {
    /* pass */
    __auto_type _t152 = pat;
    if (_t152.tag == Pattern_PBind) {
        __auto_type name = _t152.data.PBind.name;
        Sema_declare(self, name, SymbolKind_make_SVariable(), box_asttype(subj_ty), false);
    } else if (_t152.tag == Pattern_PVariantBind) {
        __auto_type type_name = _t152.data.PVariantBind.type_name;
__auto_type variant_name = _t152.data.PVariantBind.variant;
__auto_type field = _t152.data.PVariantBind.field;
        /* pass */
        AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, 0LL);
        /* pass */
        if ((strcmp((char*)fty->name, (char*)"void") == 0)) {
            /* pass */
            fty = AstType_init("int");
        }
        /* pass */
        Sema_declare(self, field, SymbolKind_make_SVariable(), box_asttype(fty), false);
    } else if (_t152.tag == Pattern_PVariantBindMany) {
        __auto_type type_name = _t152.data.PVariantBindMany.type_name;
__auto_type variant_name = _t152.data.PVariantBindMany.variant;
__auto_type fields = _t152.data.PVariantBindMany.fields;
        /* pass */
        long long _pi = 0LL;
        /* pass */
        while ((_pi < fields->len)) {
            /* pass */
            char* _pf = List_str_get(fields, _pi);
            /* pass */
            if ((strcmp((char*)_pf, (char*)"_") != 0)) {
                /* pass */
                AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, _pi);
                /* pass */
                if ((strcmp((char*)fty->name, (char*)"void") == 0)) {
                    /* pass */
                    fty = AstType_init("int");
                }
                /* pass */
                Sema_declare(self, _pf, SymbolKind_make_SVariable(), box_asttype(fty), false);
            }
            /* pass */
            _pi = (_pi + 1LL);
        }
    } else if (_t152.tag == Pattern_PTuple) {
        __auto_type first = _t152.data.PTuple.first;
__auto_type second = _t152.data.PTuple.second;
        /* pass */
        Sema_declare(self, first, SymbolKind_make_SVariable(), box_asttype(AstType_init("int")), false);
        /* pass */
        Sema_declare(self, second, SymbolKind_make_SVariable(), box_asttype(AstType_init("int")), false);
    } else if (_t152.tag == Pattern_POr) {
        __auto_type pats = _t152.data.POr.patterns;
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
        __auto_type _ = _t152;
        /* pass */
    }
}

__attribute__((hot)) AstType* Sema_str_method_ret_ty(Sema* self, char* method) {
    /* pass */
    if ((((strcmp((char*)method, (char*)"split") == 0) || (strcmp((char*)method, (char*)"split_to_vec") == 0)) || (strcmp((char*)method, (char*)"split_once") == 0))) {
        /* pass */
        AstType** _str_ty = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
        /* pass */
        (*_str_ty = AstType_init("str"));
        /* pass */
        return AstType_init_generic("Vec", _str_ty);
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"strip") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"trim") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"trim_left") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"trim_right") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"to_upper") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"to_lower") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"capitalize") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"title") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"reverse") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"repeat") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"replace") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"replace_first") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"slice") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"strip_prefix") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"strip_suffix") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"remove_char") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"join") == 0)) {
        /* pass */
        return AstType_init("str");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"len") == 0)) {
        /* pass */
        return AstType_init("int");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"index_of") == 0)) {
        /* pass */
        return AstType_init("int");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"last_index_of") == 0)) {
        /* pass */
        return AstType_init("int");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"count") == 0)) {
        /* pass */
        return AstType_init("int");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"char_at") == 0)) {
        /* pass */
        return AstType_init("int");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"parse_int") == 0)) {
        /* pass */
        return AstType_init("int");
    }
    /* pass */
    if (((strcmp((char*)method, (char*)"to_int") == 0) || (strcmp((char*)method, (char*)"to_i64") == 0))) {
        /* pass */
        return AstType_init("int");
    }
    /* pass */
    if (((strcmp((char*)method, (char*)"to_float") == 0) || (strcmp((char*)method, (char*)"to_f64") == 0))) {
        /* pass */
        return AstType_init("float");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"starts_with") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"ends_with") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"contains_char") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"eq") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"is_digit") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"is_alpha") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"is_alnum") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"is_space") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"is_upper") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"is_lower") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"parse_bool") == 0)) {
        /* pass */
        return AstType_init("bool");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"parse_float") == 0)) {
        /* pass */
        return AstType_init("float");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"lines") == 0)) {
        /* pass */
        return AstType_init("Vec");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"words") == 0)) {
        /* pass */
        return AstType_init("Vec");
    }
    /* pass */
    return AstType_init("void");
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
    __auto_type _t153 = e;
    if (_t153.tag == Expr_ELitInt) {
        __auto_type v = _t153.data.ELitInt.val;
        return box_hirexpr(HirExpr_ctor_ELitInt(v, AstType_init("int")));
    } else if (_t153.tag == Expr_ELitFloat) {
        __auto_type v = _t153.data.ELitFloat.val;
        return box_hirexpr(HirExpr_ctor_ELitFloat(v, AstType_init("float")));
    } else if (_t153.tag == Expr_ELitStr) {
        __auto_type v = _t153.data.ELitStr.val;
        return box_hirexpr(HirExpr_ctor_ELitStr(v, AstType_init("str")));
    } else if (_t153.tag == Expr_ERawStr) {
        __auto_type v = _t153.data.ERawStr.val;
        return box_hirexpr(HirExpr_ctor_ERawStr(v, AstType_init("str")));
    } else if (_t153.tag == Expr_ELitBytes) {
        __auto_type v = _t153.data.ELitBytes.val;
        return box_hirexpr(HirExpr_ctor_ELitBytes(v, AstType_init("Bytes")));
    } else if (_t153.tag == Expr_ELitBool) {
        __auto_type v = _t153.data.ELitBool.val;
        return box_hirexpr(HirExpr_ctor_ELitBool(v, AstType_init("bool")));
    } else if (_t153.tag == Expr_ELitChar) {
        __auto_type v = _t153.data.ELitChar.val;
        return box_hirexpr(HirExpr_ctor_ELitChar(v, AstType_init("char")));
    } else if (_t153.tag == Expr_ELitNone) {
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init("None")));
    } else if (_t153.tag == Expr_EIdent) {
        __auto_type name = _t153.data.EIdent.name;
        /* pass */
        Symbol* sym = Sema_resolve(self, name);
        /* pass */
        AstType* ty = (*sym->ty);
        /* pass */
        if ((_tr_dict_contains(self->fn_sigs, name) && (sym->kind.tag == SymbolKind_make_SFunction().tag))) {
            /* pass */
            return box_hirexpr(HirExpr_ctor_EIdent(name, ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, name)), false));
        }
        /* pass */
        if ((sym->is_freed && (strcmp((char*)sym->name, (char*)"") != 0))) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[M-6] '", name), "' was freed by 'dealloc()' and can no longer be used.\n      FIX: Remove all uses of '"), name), "' after 'dealloc()', or restructure so the pointer is freed only when no longer needed."));
        } else if (((((sym->is_moved && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (strcmp((char*)sym->name, (char*)"") != 0)) && (!sym->is_shared))) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat("[M-1] '", name), "' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving."));
        } else if (((((sym->is_maybe_moved && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (strcmp((char*)sym->name, (char*)"") != 0)) && (!sym->is_shared))) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[M-5] '", name), "' may have been moved on some code paths, making this use unsafe.\n      FIX: Ensure '"), name), "' is not moved before this point on any branch, or restructure so the use is inside the branch where it's still valid."));
        }
        /* pass */
        if ((((((!sym->is_init) && (!sym->is_maybe_init)) && (strcmp((char*)sym->name, (char*)"") != 0)) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[I-1] Variable '", name), "' is used before being assigned a value.\n      FIX: Assign a value before use, e.g. 'mut "), name), " = <default>'."));
        } else if (((((sym->is_maybe_init && (!sym->is_init)) && (strcmp((char*)sym->name, (char*)"") != 0)) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[I-2] '", name), "' is not initialized on all code paths before this use.\n      FIX: Initialize '"), name), "' before the if/loop, or ensure every branch assigns a value."));
        }
        /* pass */
        bool is_move = false;
        /* pass */
        if ((_tr_dict_contains(self->assign_froms, name) && (!Sema_is_primitive(self, ty)))) {
            /* pass */
            is_move = true;
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EIdent(name, ty, is_move));
    } else if (_t153.tag == Expr_EBinOp) {
        __auto_type op = _t153.data.EBinOp.op;
__auto_type left = _t153.data.EBinOp.left;
__auto_type right = _t153.data.EBinOp.right;
        /* pass */
        HirExpr* hleft = Sema_lower_expr(self, left);
        /* pass */
        HirExpr* hright = Sema_lower_expr(self, right);
        /* pass */
        AstType* bin_ty = hir_expr_type(hleft);
        /* pass */
        if ((strcmp((char*)bin_ty->name, (char*)"void") == 0)) {
            /* pass */
            bin_ty = hir_expr_type(hright);
        }
        /* pass */
        if (((((((((((strcmp((char*)op, (char*)"==") == 0) || (strcmp((char*)op, (char*)"!=") == 0)) || (strcmp((char*)op, (char*)"<") == 0)) || (strcmp((char*)op, (char*)">") == 0)) || (strcmp((char*)op, (char*)"<=") == 0)) || (strcmp((char*)op, (char*)">=") == 0)) || (strcmp((char*)op, (char*)"and") == 0)) || (strcmp((char*)op, (char*)"or") == 0)) || (strcmp((char*)op, (char*)"&&") == 0)) || (strcmp((char*)op, (char*)"||") == 0))) {
            /* pass */
            bin_ty = AstType_init("bool");
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EBinOp(op, hleft, hright, bin_ty));
    } else if (_t153.tag == Expr_EUnaryOp) {
        __auto_type op = _t153.data.EUnaryOp.op;
__auto_type expr = _t153.data.EUnaryOp.expr;
        /* pass */
        HirExpr* hexpr_inner = Sema_lower_expr(self, expr);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EUnaryOp(op, hexpr_inner, hir_expr_type(hexpr_inner)));
    } else if (_t153.tag == Expr_ECall) {
        __auto_type callee = _t153.data.ECall.callee;
__auto_type args = _t153.data.ECall.args;
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t154 = (*callee);
            if (_t154.tag == Expr_EIdent) {
                __auto_type ato_n = _t154.data.EIdent.name;
                /* pass */
                if ((strcmp((char*)ato_n, (char*)"await_timeout") == 0)) {
                    /* pass */
                    if ((!self->in_async_fn)) {
                        /* pass */
                        Sema_error(self, "[C-4] 'await_timeout' used outside an async function.");
                    }
                    /* pass */
                    if ((args->len < 2LL)) {
                        /* pass */
                        Sema_error(self, "await_timeout requires 2 arguments: await_timeout(expr, timeout_ms)");
                        /* pass */
                        return box_hirexpr(HirExpr_ctor_ELitInt(0LL, AstType_init("int")));
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
            } else if (1) {
                __auto_type _ = _t154;
                /* pass */
            }
        }
        /* pass */
        List_str* p23_borrow_names = (void*)List_str_new();
        /* pass */
        TrMap* p23_seen = _tr_dict_new(4LL);
        /* pass */
        long long p23_k = 0LL;
        /* pass */
        while ((p23_k < args->len)) {
            /* pass */
            Expr* p23_arg = ((Expr*)List_ptr_get(args, p23_k));
            /* pass */
            char* p23_nm = "";
            /* pass */
            if ((((unsigned long long)(p23_arg)) != ((unsigned long long)(0LL)))) {
                /* pass */
                __auto_type _t155 = (*p23_arg);
                if (_t155.tag == Expr_EIdent) {
                    __auto_type p23_n = _t155.data.EIdent.name;
                    p23_nm = p23_n;
                } else if (1) {
                    __auto_type _ = _t155;
                    /* pass */
                }
            }
            /* pass */
            if ((strcmp((char*)p23_nm, (char*)"") != 0)) {
                /* pass */
                Symbol* p23_sym = Sema_resolve(self, p23_nm);
                /* pass */
                if (((!Sema_is_primitive(self, (*p23_sym->ty))) && (strcmp((char*)p23_sym->name, (char*)"") != 0))) {
                    /* pass */
                    Sema_mark_borrow(self, p23_nm);
                    /* pass */
                    List_str_append(p23_borrow_names, p23_nm);
                    /* pass */
                    if (_tr_dict_contains(p23_seen, p23_nm)) {
                        /* pass */
                        Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[M-3] '", p23_nm), "' appears twice in the same call, creating aliased mutable access.\n      FIX: Clone one of the arguments: "), p23_nm), ".clone()"));
                    }
                    /* pass */
                    _tr_dict_set(p23_seen, p23_nm, true);
                }
            }
            /* pass */
            p23_k = (p23_k + 1LL);
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
            Sema_unmark_borrow(self, List_str_get(p23_borrow_names, p23_ui));
            /* pass */
            p23_ui = (p23_ui + 1LL);
        }
        /* pass */
        HirExpr* hcallee = Sema_lower_expr(self, callee);
        /* pass */
        AstType* ret_ty = AstType_init("void");
        /* pass */
        if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
        }
        /* pass */
        __auto_type _t156 = (*callee);
        if (_t156.tag == Expr_EIdent) {
            __auto_type n = _t156.data.EIdent.name;
            /* pass */
            if ((strcmp((char*)n, (char*)"main") == 0)) {
                /* pass */
                Sema_error(self, "[E-1] Explicit call to 'main()' is forbidden. The compiler automatically invokes main() as the program entry point. Remove the 'main()' call from your source.");
                /* pass */
                return box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
            }
            /* pass */
            if ((((((((_tr_dict_contains(self->classes, n) || (strcmp((char*)n, (char*)"StringObj") == 0)) || (strcmp((char*)n, (char*)"StringBuilder") == 0)) || (strcmp((char*)n, (char*)"List") == 0)) || (strcmp((char*)n, (char*)"Vec") == 0)) || (strcmp((char*)n, (char*)"Pointer") == 0)) || (strcmp((char*)n, (char*)"Map") == 0)) || (strcmp((char*)n, (char*)"Dict") == 0))) {
                /* pass */
                ret_ty = AstType_init(n);
            } else if (_tr_dict_contains(self->enums, n)) {
                /* pass */
                ret_ty = AstType_init(n);
            } else if (((strcmp((char*)n, (char*)"alloc") == 0) || (strcmp((char*)n, (char*)"dealloc") == 0))) {
                /* pass */
                ret_ty = AstType_init("Pointer");
            } else {
                /* pass */
                Symbol* _fsym = Sema_resolve(self, n);
                /* pass */
                ret_ty = (*_fsym->ty);
                /* pass */
                if (((strcmp((char*)ret_ty->name, (char*)"def") == 0) && (ret_ty->args->len > 0LL))) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(ret_ty->args, (ret_ty->args->len - 1LL))));
                }
            }
        } else if (_t156.tag == Expr_EIndex) {
            __auto_type base = _t156.data.EIndex.obj;
__auto_type idx = _t156.data.EIndex.index;
            /* pass */
            __auto_type _t157 = (*base);
            if (_t157.tag == Expr_EIdent) {
                __auto_type gn = _t157.data.EIdent.name;
                /* pass */
                if (((strcmp((char*)gn, (char*)"alloc") == 0) || (strcmp((char*)gn, (char*)"dealloc") == 0))) {
                    /* pass */
                    __auto_type _t158 = (*idx);
                    if (_t158.tag == Expr_EIdent) {
                        __auto_type tn = _t158.data.EIdent.name;
                        /* pass */
                        ret_ty = AstType_init("Pointer");
                        /* pass */
                        List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn)));
                    } else if (1) {
                        __auto_type _ = _t158;
                        /* pass */
                        ret_ty = AstType_init("Pointer");
                    }
                    /* pass */
                    if ((self->strict_mode && (!self->in_unsafe))) {
                        /* pass */
                        Sema_error(self, "[U-1] 'alloc'/'dealloc' used outside an 'unsafe:' block.\n      FIX: Wrap raw memory operations in 'unsafe:' to signal manual memory management.\n      Or remove --strict to allow this pattern.");
                    }
                } else if ((((((strcmp((char*)gn, (char*)"Pointer") == 0) || (strcmp((char*)gn, (char*)"List") == 0)) || (strcmp((char*)gn, (char*)"Vec") == 0)) || (strcmp((char*)gn, (char*)"Map") == 0)) || (strcmp((char*)gn, (char*)"Dict") == 0))) {
                    /* pass */
                    __auto_type _t159 = (*idx);
                    if (_t159.tag == Expr_EIdent) {
                        __auto_type tn = _t159.data.EIdent.name;
                        /* pass */
                        ret_ty = AstType_init(gn);
                        /* pass */
                        List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn)));
                    } else if (1) {
                        __auto_type _ = _t159;
                        /* pass */
                        ret_ty = AstType_init(gn);
                    }
                } else if (_tr_dict_contains(self->classes, gn)) {
                    /* pass */
                    ret_ty = AstType_init(gn);
                    /* pass */
                    __auto_type _t160 = (*idx);
                    if (_t160.tag == Expr_EIdent) {
                        __auto_type targ_ca = _t160.data.EIdent.name;
                        List_ptr_append(ret_ty->args, box_asttype(AstType_init(targ_ca)));
                    } else if (1) {
                        __auto_type _ = _t160;
                        /* pass */
                    }
                } else if (_tr_dict_contains(self->enums, gn)) {
                    /* pass */
                    ret_ty = AstType_init(gn);
                } else {
                    /* pass */
                    __auto_type _t161 = (*idx);
                    if (_t161.tag == Expr_EIdent) {
                        __auto_type farg_c = _t161.data.EIdent.name;
                        /* pass */
                        AstType* fret = AstType_init(farg_c);
                        /* pass */
                        ret_ty = fret;
                        /* pass */
                        hcallee = box_hirexpr(HirExpr_ctor_EIdent(_tr_str_concat(_tr_str_concat(gn, "__MONO_"), farg_c), fret, false));
                    } else if (1) {
                        __auto_type _ = _t161;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t157;
                /* pass */
            }
        } else if (_t156.tag == Expr_EPropAccess) {
            __auto_type obj = _t156.data.EPropAccess.obj;
__auto_type variant = _t156.data.EPropAccess.prop;
            /* pass */
            __auto_type _t162 = (*obj);
            if (_t162.tag == Expr_EIdent) {
                __auto_type type_name = _t162.data.EIdent.name;
                /* pass */
                if (_tr_dict_contains(self->enums, type_name)) {
                    /* pass */
                    ret_ty = AstType_init(type_name);
                } else if ((((((strcmp((char*)type_name, (char*)"Thread") == 0) || (strcmp((char*)type_name, (char*)"ThreadPool") == 0)) || (strcmp((char*)type_name, (char*)"Atomic") == 0)) || (strcmp((char*)type_name, (char*)"ThreadLocal") == 0)) && (((strcmp((char*)variant, (char*)"spawn") == 0) || (strcmp((char*)variant, (char*)"new") == 0)) || (strcmp((char*)variant, (char*)"init") == 0)))) {
                    /* pass */
                    ret_ty = AstType_init(type_name);
                } else if (((strcmp((char*)variant, (char*)"init") == 0) || (strcmp((char*)variant, (char*)"new") == 0))) {
                    /* pass */
                    if ((((((strcmp((char*)type_name, (char*)"Pointer") == 0) || (strcmp((char*)type_name, (char*)"List") == 0)) || (strcmp((char*)type_name, (char*)"Vec") == 0)) || (strcmp((char*)type_name, (char*)"Map") == 0)) || (strcmp((char*)type_name, (char*)"Dict") == 0))) {
                        /* pass */
                        ret_ty = AstType_init(type_name);
                    } else if (_tr_dict_contains(self->classes, type_name)) {
                        /* pass */
                        ret_ty = AstType_init(type_name);
                    }
                }
            } else if (_t162.tag == Expr_EIndex) {
                __auto_type base2 = _t162.data.EIndex.obj;
__auto_type idx2 = _t162.data.EIndex.index;
                /* pass */
                if (((strcmp((char*)variant, (char*)"init") == 0) || (strcmp((char*)variant, (char*)"new") == 0))) {
                    /* pass */
                    __auto_type _t163 = (*base2);
                    if (_t163.tag == Expr_EIdent) {
                        __auto_type gn2 = _t163.data.EIdent.name;
                        /* pass */
                        if ((((((strcmp((char*)gn2, (char*)"Pointer") == 0) || (strcmp((char*)gn2, (char*)"List") == 0)) || (strcmp((char*)gn2, (char*)"Vec") == 0)) || (strcmp((char*)gn2, (char*)"Map") == 0)) || (strcmp((char*)gn2, (char*)"Dict") == 0))) {
                            /* pass */
                            __auto_type _t164 = (*idx2);
                            if (_t164.tag == Expr_EIdent) {
                                __auto_type tn2 = _t164.data.EIdent.name;
                                /* pass */
                                ret_ty = AstType_init(gn2);
                                /* pass */
                                List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn2)));
                            } else if (1) {
                                __auto_type _ = _t164;
                                /* pass */
                                ret_ty = AstType_init(gn2);
                            }
                        } else if (_tr_dict_contains(self->classes, gn2)) {
                            /* pass */
                            ret_ty = AstType_init(gn2);
                            /* pass */
                            __auto_type _t165 = (*idx2);
                            if (_t165.tag == Expr_EIdent) {
                                __auto_type targ_cb = _t165.data.EIdent.name;
                                List_ptr_append(ret_ty->args, box_asttype(AstType_init(targ_cb)));
                            } else if (1) {
                                __auto_type _ = _t165;
                                /* pass */
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t163;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t162;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t156;
            /* pass */
        }
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t166 = (*callee);
            if (_t166.tag == Expr_EIdent) {
                __auto_type aa_nm = _t166.data.EIdent.name;
                /* pass */
                if ((strcmp((char*)aa_nm, (char*)"await_all") == 0)) {
                    /* pass */
                    long long aa_i = 0LL;
                    /* pass */
                    while ((aa_i < hl->len)) {
                        /* pass */
                        Sema_check_spawn_sendable(self, List_ptr_get(hl, aa_i));
                        /* pass */
                        aa_i = (aa_i + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t166;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t167 = (*callee);
            if (_t167.tag == Expr_EIdent) {
                __auto_type df_nm = _t167.data.EIdent.name;
                /* pass */
                if (((strcmp((char*)df_nm, (char*)"dealloc") == 0) && (args->len > 0LL))) {
                    /* pass */
                    Expr* df_arg0 = ((Expr*)List_ptr_get(args, 0LL));
                    /* pass */
                    if ((((unsigned long long)(df_arg0)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        __auto_type _t168 = (*df_arg0);
                        if (_t168.tag == Expr_EIdent) {
                            __auto_type df_ptr = _t168.data.EIdent.name;
                            /* pass */
                            Symbol* df_sym = Sema_resolve(self, df_ptr);
                            /* pass */
                            if ((strcmp((char*)df_sym->name, (char*)"") != 0)) {
                                /* pass */
                                Sema_mark_freed(self, df_ptr);
                            }
                        } else if (1) {
                            __auto_type _ = _t168;
                            /* pass */
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t167;
                /* pass */
            }
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
    } else if (_t153.tag == Expr_EMethodCall) {
        __auto_type obj = _t153.data.EMethodCall.obj;
__auto_type method = _t153.data.EMethodCall.method;
__auto_type args = _t153.data.EMethodCall.args;
        /* pass */
        if (((strcmp((char*)method, (char*)"__index__") == 0) && (args->len > 0LL))) {
            /* pass */
            return Sema_lower_expr(self, box_expr(Expr_ctor_EIndex(obj, ((Expr*)List_ptr_get(args, 0LL)))));
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
        HirExpr* hobj = Sema_lower_expr(self, obj);
        /* pass */
        AstType* hobj_ty = hir_expr_type(hobj);
        /* pass */
        if (_tr_dict_contains(self->type_aliases, hobj_ty->name)) {
            /* pass */
            char* _alias_base = ((char*)(uintptr_t)_tr_dict_get(self->type_aliases, hobj_ty->name));
            /* pass */
            AstType* _alias_ty = AstType_init(_alias_base);
            /* pass */
            if (_tr_dict_contains(self->type_alias_elem, hobj_ty->name)) {
                /* pass */
                char* _elem_name = ((char*)(uintptr_t)_tr_dict_get(self->type_alias_elem, hobj_ty->name));
                /* pass */
                AstType** _elem_ptr = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
                /* pass */
                (*_elem_ptr = AstType_init(_elem_name));
                /* pass */
                _alias_ty = AstType_init_generic(_alias_base, _elem_ptr);
            }
            /* pass */
            hobj_ty = _alias_ty;
        }
        /* pass */
        if (_tr_dict_contains(self->classes, hobj_ty->name)) {
            /* pass */
            ClassDef* _cf_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, hobj_ty->name));
            /* pass */
            long long _cf_i = 0LL;
            /* pass */
            AstType* _cf_ty = AstType_init("void");
            /* pass */
            bool _cf_found = false;
            /* pass */
            while ((_cf_i < _cf_cls->fields->len)) {
                /* pass */
                FieldDef* _cf_fld = ((FieldDef*)List_ptr_get(_cf_cls->fields, _cf_i));
                /* pass */
                if (((strcmp((char*)_cf_fld->name, (char*)method) == 0) && (((unsigned long long)(_cf_fld->ty)) != ((unsigned long long)(0LL))))) {
                    /* pass */
                    _cf_ty = (*_cf_fld->ty);
                    /* pass */
                    _cf_found = true;
                }
                /* pass */
                _cf_i = (_cf_i + 1LL);
            }
            /* pass */
            if ((_cf_found && (strcmp((char*)_cf_ty->name, (char*)"def") == 0))) {
                /* pass */
                AstType* _cf_ret = AstType_init("void");
                /* pass */
                if ((_cf_ty->args->len > 0LL)) {
                    /* pass */
                    _cf_ret = (*((AstType**)List_ptr_get(_cf_ty->args, (_cf_ty->args->len - 1LL))));
                }
                /* pass */
                HirExpr* _cf_prop = box_hirexpr(HirExpr_ctor_EPropAccess(hobj, method, _cf_ty));
                /* pass */
                return box_hirexpr(HirExpr_ctor_ECall(_cf_prop, hl, _cf_ret));
            }
        }
        /* pass */
        if (((((strcmp((char*)method, (char*)"push") == 0) || (strcmp((char*)method, (char*)"pop") == 0)) || (strcmp((char*)method, (char*)"insert") == 0)) || (strcmp((char*)method, (char*)"remove") == 0))) {
            /* pass */
            char* pc_obj_nm = "";
            /* pass */
            __auto_type _t169 = (*obj);
            if (_t169.tag == Expr_EIdent) {
                __auto_type pc_src = _t169.data.EIdent.name;
                pc_obj_nm = pc_src;
            } else if (1) {
                __auto_type _ = _t169;
                /* pass */
            }
            /* pass */
            if (((strcmp((char*)pc_obj_nm, (char*)"") != 0) && _tr_dict_contains(self->container_borrows, pc_obj_nm))) {
                /* pass */
                char* pc_borrow_var = ((char*)(uintptr_t)_tr_dict_get(self->container_borrows, pc_obj_nm));
                /* pass */
                Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[M-4] Cannot mutate '", pc_obj_nm), "' while '"), pc_borrow_var), "' holds a reference into it.\n      FIX: Finish using '"), pc_borrow_var), "' before modifying '"), pc_obj_nm), "', or copy it first: 'mut copy = "), pc_borrow_var), "'."));
            }
        }
        /* pass */
        char* _bm_obj_nm = "";
        /* pass */
        __auto_type _t170 = (*obj);
        if (_t170.tag == Expr_EIdent) {
            __auto_type _bm_n = _t170.data.EIdent.name;
            _bm_obj_nm = _bm_n;
        } else if (1) {
            __auto_type _ = _t170;
            /* pass */
        }
        /* pass */
        AstType* ret_ty = AstType_init("void");
        /* pass */
        if (((strcmp((char*)_bm_obj_nm, (char*)"OS") == 0) || (strcmp((char*)hobj_ty->name, (char*)"OS") == 0))) {
            /* pass */
            if (((strcmp((char*)method, (char*)"cwd") == 0) || (strcmp((char*)method, (char*)"platform") == 0))) {
                /* pass */
                ret_ty = AstType_init("str");
            } else if (((((strcmp((char*)method, (char*)"is_windows") == 0) || (strcmp((char*)method, (char*)"is_linux") == 0)) || (strcmp((char*)method, (char*)"is_darwin") == 0)) || (strcmp((char*)method, (char*)"is_macos") == 0))) {
                /* pass */
                ret_ty = AstType_init("bool");
            }
        } else if (((strcmp((char*)_bm_obj_nm, (char*)"Process") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Process") == 0))) {
            /* pass */
            if (((strcmp((char*)method, (char*)"system") == 0) || (strcmp((char*)method, (char*)"exit") == 0))) {
                /* pass */
                ret_ty = AstType_init("int");
            } else if ((strcmp((char*)method, (char*)"shell_output") == 0)) {
                /* pass */
                ret_ty = AstType_init("str");
            }
        } else if (((strcmp((char*)_bm_obj_nm, (char*)"Env") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Env") == 0))) {
            /* pass */
            if (((strcmp((char*)method, (char*)"get_var") == 0) || (strcmp((char*)method, (char*)"cwd") == 0))) {
                /* pass */
                ret_ty = AstType_init("str");
            }
        } else if (((strcmp((char*)_bm_obj_nm, (char*)"Hash") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Hash") == 0))) {
            /* pass */
            if (((strcmp((char*)method, (char*)"sha256") == 0) || (strcmp((char*)method, (char*)"md5") == 0))) {
                /* pass */
                ret_ty = AstType_init("str");
            }
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"init") == 0) || (strcmp((char*)method, (char*)"new") == 0))) {
            /* pass */
            if (((strcmp((char*)hobj_ty->name, (char*)"void") == 0) || (strcmp((char*)hobj_ty->name, (char*)"") == 0))) {
                /* pass */
                __auto_type _t171 = (*hobj);
                if (_t171.tag == HirExpr_EIdent) {
                    __auto_type recv_nm = _t171.data.EIdent.name;
                    ret_ty = AstType_init(recv_nm);
                } else if (_t171.tag == HirExpr_EIndex) {
                    __auto_type idx_base = _t171.data.EIndex.obj;
__auto_type idx_arg = _t171.data.EIndex.index;
                    /* pass */
                    __auto_type _t172 = (*idx_base);
                    if (_t172.tag == HirExpr_EIdent) {
                        __auto_type gn = _t172.data.EIdent.name;
                        /* pass */
                        ret_ty = AstType_init(gn);
                        /* pass */
                        __auto_type _t173 = (*idx_arg);
                        if (_t173.tag == HirExpr_EIdent) {
                            __auto_type tn = _t173.data.EIdent.name;
                            List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn)));
                        } else if (1) {
                            __auto_type _ = _t173;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t172;
                        ret_ty = hobj_ty;
                    }
                } else if (1) {
                    __auto_type _ = _t171;
                    ret_ty = hobj_ty;
                }
            } else {
                /* pass */
                ret_ty = hobj_ty;
            }
        } else if ((strcmp((char*)method, (char*)"offset") == 0)) {
            /* pass */
            ret_ty = hobj_ty;
        } else if ((((strcmp((char*)hobj_ty->name, (char*)"List") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Vec") == 0)) && (((((strcmp((char*)method, (char*)"sum") == 0) || (strcmp((char*)method, (char*)"min") == 0)) || (strcmp((char*)method, (char*)"max") == 0)) || (strcmp((char*)method, (char*)"min_val") == 0)) || (strcmp((char*)method, (char*)"max_val") == 0)))) {
            /* pass */
            if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
                /* pass */
                ret_ty = AstType_init("int");
            }
        } else if ((((strcmp((char*)hobj_ty->name, (char*)"List") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Vec") == 0)) && (((strcmp((char*)method, (char*)"any") == 0) || (strcmp((char*)method, (char*)"all") == 0)) || (strcmp((char*)method, (char*)"is_empty") == 0)))) {
            /* pass */
            ret_ty = AstType_init("bool");
        } else if ((((strcmp((char*)hobj_ty->name, (char*)"List") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Vec") == 0)) && ((strcmp((char*)method, (char*)"first") == 0) || (strcmp((char*)method, (char*)"last") == 0)))) {
            /* pass */
            if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
                /* pass */
                ret_ty = AstType_init("void");
            }
        } else if ((((strcmp((char*)hobj_ty->name, (char*)"List") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Vec") == 0)) && ((strcmp((char*)method, (char*)"reversed") == 0) || (strcmp((char*)method, (char*)"reversed_copy") == 0)))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if ((((strcmp((char*)hobj_ty->name, (char*)"List") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Vec") == 0)) && ((strcmp((char*)method, (char*)"clone") == 0) || (strcmp((char*)method, (char*)"copy") == 0)))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if ((((strcmp((char*)hobj_ty->name, (char*)"List") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Vec") == 0)) && (strcmp((char*)method, (char*)"reverse") == 0))) {
            /* pass */
            ret_ty = AstType_init("void");
        } else if ((((strcmp((char*)hobj_ty->name, (char*)"List") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Vec") == 0)) && (((strcmp((char*)method, (char*)"index_of") == 0) || (strcmp((char*)method, (char*)"last_index_of") == 0)) || (strcmp((char*)method, (char*)"count") == 0)))) {
            /* pass */
            ret_ty = AstType_init("int");
        } else if ((((strcmp((char*)hobj_ty->name, (char*)"List") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Vec") == 0)) && (strcmp((char*)method, (char*)"join") == 0))) {
            /* pass */
            ret_ty = AstType_init("str");
        } else if ((strcmp((char*)method, (char*)"read") == 0)) {
            /* pass */
            if (((strcmp((char*)hobj_ty->name, (char*)"Pointer") == 0) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
                /* pass */
                ret_ty = AstType_init("int");
            }
        } else if (((strcmp((char*)method, (char*)"as_str") == 0) || (strcmp((char*)method, (char*)"to_str") == 0))) {
            /* pass */
            ret_ty = AstType_init("str");
        } else if (((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"__len__") == 0))) {
            /* pass */
            ret_ty = AstType_init("int");
        } else if ((((strcmp((char*)method, (char*)"checked_add") == 0) || (strcmp((char*)method, (char*)"checked_sub") == 0)) || (strcmp((char*)method, (char*)"checked_mul") == 0))) {
            /* pass */
            AstType** _opt_int = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
            /* pass */
            (*_opt_int = AstType_init("int"));
            /* pass */
            ret_ty = AstType_init_generic("Option", _opt_int);
        } else if (((((((((strcmp((char*)method, (char*)"abs") == 0) || (strcmp((char*)method, (char*)"min") == 0)) || (strcmp((char*)method, (char*)"max") == 0)) || (strcmp((char*)method, (char*)"pow") == 0)) || (strcmp((char*)method, (char*)"sign") == 0)) || (strcmp((char*)method, (char*)"clamp") == 0)) || (strcmp((char*)method, (char*)"gcd") == 0)) || (strcmp((char*)method, (char*)"lcm") == 0))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if ((((strcmp((char*)hobj_ty->name, (char*)"float") == 0) || (strcmp((char*)hobj_ty->name, (char*)"f64") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"f32") == 0))) {
            /* pass */
            if ((((((((((((((((((strcmp((char*)method, (char*)"floor") == 0) || (strcmp((char*)method, (char*)"ceil") == 0)) || (strcmp((char*)method, (char*)"round") == 0)) || (strcmp((char*)method, (char*)"sqrt") == 0)) || (strcmp((char*)method, (char*)"fabs") == 0)) || (strcmp((char*)method, (char*)"log") == 0)) || (strcmp((char*)method, (char*)"log2") == 0)) || (strcmp((char*)method, (char*)"log10") == 0)) || (strcmp((char*)method, (char*)"exp") == 0)) || (strcmp((char*)method, (char*)"sin") == 0)) || (strcmp((char*)method, (char*)"cos") == 0)) || (strcmp((char*)method, (char*)"tan") == 0)) || (strcmp((char*)method, (char*)"asin") == 0)) || (strcmp((char*)method, (char*)"acos") == 0)) || (strcmp((char*)method, (char*)"atan") == 0)) || (strcmp((char*)method, (char*)"atan2") == 0)) || (strcmp((char*)method, (char*)"pow") == 0))) {
                /* pass */
                ret_ty = AstType_init("float");
            } else if (((strcmp((char*)method, (char*)"is_nan") == 0) || (strcmp((char*)method, (char*)"is_inf") == 0))) {
                /* pass */
                ret_ty = AstType_init("bool");
            }
        } else if ((strcmp((char*)hobj_ty->name, (char*)"str") == 0)) {
            /* pass */
            ret_ty = Sema_str_method_ret_ty(self, method);
        } else if ((((((((((((strcmp((char*)hobj_ty->name, (char*)"int") == 0) || (strcmp((char*)hobj_ty->name, (char*)"i64") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"i32") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"i16") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"i8") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"u64") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"u32") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"u16") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"u8") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"usize") == 0)) || (strcmp((char*)hobj_ty->name, (char*)"char") == 0))) {
            /* pass */
            if ((((((((strcmp((char*)method, (char*)"to_hex") == 0) || (strcmp((char*)method, (char*)"to_HEX") == 0)) || (strcmp((char*)method, (char*)"to_hex_upper") == 0)) || (strcmp((char*)method, (char*)"to_octal") == 0)) || (strcmp((char*)method, (char*)"to_oct") == 0)) || (strcmp((char*)method, (char*)"to_binary") == 0)) || (strcmp((char*)method, (char*)"to_bin") == 0))) {
                /* pass */
                ret_ty = AstType_init("str");
            }
        } else if (((strcmp((char*)method, (char*)"to_float") == 0) || (strcmp((char*)method, (char*)"to_f64") == 0))) {
            /* pass */
            ret_ty = AstType_init("float");
        } else if (((strcmp((char*)method, (char*)"to_int") == 0) || (strcmp((char*)method, (char*)"to_i64") == 0))) {
            /* pass */
            ret_ty = AstType_init("int");
        } else if ((((strcmp((char*)method, (char*)"to_str") == 0) || (strcmp((char*)method, (char*)"to_string") == 0)) && (strcmp((char*)hobj_ty->name, (char*)"StringBuilder") != 0))) {
            /* pass */
            ret_ty = AstType_init("str");
        } else if (((strcmp((char*)method, (char*)"get") == 0) || (strcmp((char*)method, (char*)"pop") == 0))) {
            /* pass */
            if ((((strcmp((char*)hobj_ty->name, (char*)"List") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Vec") == 0)) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else if ((((strcmp((char*)hobj_ty->name, (char*)"Map") == 0) || (strcmp((char*)hobj_ty->name, (char*)"Dict") == 0)) && (hobj_ty->args->len > 1LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
            } else if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else if (_tr_dict_contains(self->classes, hobj_ty->name)) {
                /* pass */
                ClassDef* _gcls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, hobj_ty->name));
                /* pass */
                long long _gmi = 0LL;
                /* pass */
                while ((_gmi < _gcls->methods->len)) {
                    /* pass */
                    FunctionDef* _gmdef = ((FunctionDef*)List_ptr_get(_gcls->methods, _gmi));
                    /* pass */
                    if ((strcmp((char*)_gmdef->name, (char*)method) == 0)) {
                        /* pass */
                        if ((((unsigned long long)(_gmdef->ret_ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            ret_ty = (*_gmdef->ret_ty);
                        }
                    }
                    /* pass */
                    _gmi = (_gmi + 1LL);
                }
            }
        } else if ((strcmp((char*)method, (char*)"alloc") == 0)) {
            /* pass */
            ret_ty = hobj_ty;
        } else if ((strcmp((char*)method, (char*)"contains") == 0)) {
            /* pass */
            ret_ty = AstType_init("bool");
        } else if ((strcmp((char*)hobj_ty->name, (char*)"Set") == 0)) {
            /* pass */
            if (((((strcmp((char*)method, (char*)"contains") == 0) || (strcmp((char*)method, (char*)"has") == 0)) || (strcmp((char*)method, (char*)"is_empty") == 0)) || (strcmp((char*)method, (char*)"is_subset") == 0))) {
                /* pass */
                ret_ty = AstType_init("bool");
            } else if (((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"length") == 0))) {
                /* pass */
                ret_ty = AstType_init("int");
            } else if ((((strcmp((char*)method, (char*)"add") == 0) || (strcmp((char*)method, (char*)"remove") == 0)) || (strcmp((char*)method, (char*)"clear") == 0))) {
                /* pass */
                ret_ty = AstType_init("void");
            } else if ((strcmp((char*)method, (char*)"to_list") == 0)) {
                /* pass */
                AstType** _set_elem = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    (*_set_elem = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL))));
                } else {
                    /* pass */
                    (*_set_elem = AstType_init("str"));
                }
                /* pass */
                ret_ty = AstType_init_generic("List", _set_elem);
            } else if ((((strcmp((char*)method, (char*)"union") == 0) || (strcmp((char*)method, (char*)"intersection") == 0)) || (strcmp((char*)method, (char*)"difference") == 0))) {
                /* pass */
                ret_ty = hobj_ty;
            }
        } else if ((strcmp((char*)hobj_ty->name, (char*)"Option") == 0)) {
            /* pass */
            if (((strcmp((char*)method, (char*)"is_some") == 0) || (strcmp((char*)method, (char*)"is_none") == 0))) {
                /* pass */
                ret_ty = AstType_init("bool");
            } else if (((strcmp((char*)method, (char*)"unwrap") == 0) || (strcmp((char*)method, (char*)"expect") == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init("int");
                }
            } else if ((strcmp((char*)method, (char*)"unwrap_or") == 0)) {
                /* pass */
                if ((hl->len > 0LL)) {
                    /* pass */
                    ret_ty = hir_expr_type(List_ptr_get(hl, 0LL));
                } else if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init("int");
                }
            }
        } else if ((strcmp((char*)hobj_ty->name, (char*)"Result") == 0)) {
            /* pass */
            if (((strcmp((char*)method, (char*)"is_ok") == 0) || (strcmp((char*)method, (char*)"is_err") == 0))) {
                /* pass */
                ret_ty = AstType_init("bool");
            } else if (((strcmp((char*)method, (char*)"unwrap") == 0) || (strcmp((char*)method, (char*)"ok") == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init("int");
                }
            } else if (((strcmp((char*)method, (char*)"unwrap_err") == 0) || (strcmp((char*)method, (char*)"err") == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 1LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init("str");
                }
            }
        } else if (((strcmp((char*)method, (char*)"spawn") == 0) && ((strcmp((char*)hobj_ty->name, (char*)"Thread") == 0) || (strcmp((char*)hobj_ty->name, (char*)"void") == 0)))) {
            /* pass */
            ret_ty = AstType_init("Thread");
            /* pass */
            long long _tsi = 1LL;
            /* pass */
            while ((_tsi < hl->len)) {
                /* pass */
                AstType* _tsa_ty = hir_expr_type(List_ptr_get(hl, _tsi));
                /* pass */
                if ((!Sema_is_sendable_type(self, _tsa_ty->name))) {
                    /* pass */
                    Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-1] Type '", _tsa_ty->name), "' is not Sendable and cannot be passed to Thread.spawn.\n      FIX: Wrap in Mutex["), _tsa_ty->name), "] for exclusive access, or add 'implements Sendable' to '"), _tsa_ty->name), "' to confirm it is thread-safe."));
                }
                /* pass */
                _tsi = (_tsi + 1LL);
            }
        } else if (((strcmp((char*)method, (char*)"spawn") == 0) && (strcmp((char*)hobj_ty->name, (char*)"ThreadPool") == 0))) {
            /* pass */
            ret_ty = AstType_init("void");
            /* pass */
            long long _psi = 1LL;
            /* pass */
            while ((_psi < hl->len)) {
                /* pass */
                AstType* _psa_ty = hir_expr_type(List_ptr_get(hl, _psi));
                /* pass */
                if ((!Sema_is_sendable_type(self, _psa_ty->name))) {
                    /* pass */
                    Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[T-1] Type '", _psa_ty->name), "' is not Sendable and cannot be passed to ThreadPool.spawn.\n      FIX: Wrap in Mutex["), _psa_ty->name), "] for exclusive access, or add 'implements Sendable' to '"), _psa_ty->name), "' to confirm it is thread-safe."));
                }
                /* pass */
                _psi = (_psi + 1LL);
            }
        } else if ((strcmp((char*)hobj_ty->name, (char*)"Atomic") == 0)) {
            /* pass */
            if (((((((((((((((((strcmp((char*)method, (char*)"load") == 0) || (strcmp((char*)method, (char*)"get") == 0)) || (strcmp((char*)method, (char*)"add") == 0)) || (strcmp((char*)method, (char*)"sub") == 0)) || (strcmp((char*)method, (char*)"swap") == 0)) || (strcmp((char*)method, (char*)"exchange") == 0)) || (strcmp((char*)method, (char*)"fetch_add") == 0)) || (strcmp((char*)method, (char*)"fetch_sub") == 0)) || (strcmp((char*)method, (char*)"load_relaxed") == 0)) || (strcmp((char*)method, (char*)"load_acquire") == 0)) || (strcmp((char*)method, (char*)"load_seqcst") == 0)) || (strcmp((char*)method, (char*)"add_relaxed") == 0)) || (strcmp((char*)method, (char*)"add_release") == 0)) || (strcmp((char*)method, (char*)"add_acqrel") == 0)) || (strcmp((char*)method, (char*)"sub_relaxed") == 0)) || (strcmp((char*)method, (char*)"sub_release") == 0))) {
                /* pass */
                ret_ty = AstType_init("int");
            } else if (((((strcmp((char*)method, (char*)"cas") == 0) || (strcmp((char*)method, (char*)"compare_exchange") == 0)) || (strcmp((char*)method, (char*)"cas_weak") == 0)) || (strcmp((char*)method, (char*)"cas_acqrel") == 0))) {
                /* pass */
                ret_ty = AstType_init("bool");
            } else if ((((((strcmp((char*)method, (char*)"store") == 0) || (strcmp((char*)method, (char*)"set") == 0)) || (strcmp((char*)method, (char*)"store_relaxed") == 0)) || (strcmp((char*)method, (char*)"store_release") == 0)) || (strcmp((char*)method, (char*)"store_seqcst") == 0))) {
                /* pass */
                ret_ty = AstType_init("void");
            }
        } else if ((strcmp((char*)hobj_ty->name, (char*)"Chan") == 0)) {
            /* pass */
            if (((strcmp((char*)method, (char*)"recv") == 0) || (strcmp((char*)method, (char*)"try_recv") == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init("int");
                }
            } else if (((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"cap") == 0))) {
                /* pass */
                ret_ty = AstType_init("int");
            } else if (((strcmp((char*)method, (char*)"is_closed") == 0) || (strcmp((char*)method, (char*)"try_send") == 0))) {
                /* pass */
                ret_ty = AstType_init("bool");
            } else if ((strcmp((char*)method, (char*)"send_timeout") == 0)) {
                /* pass */
                ret_ty = AstType_init("bool");
            } else if ((strcmp((char*)method, (char*)"recv_timeout") == 0)) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init("int");
                }
            }
        } else if ((strcmp((char*)hobj_ty->name, (char*)"Mutex") == 0)) {
            /* pass */
            if (((strcmp((char*)method, (char*)"lock") == 0) || (strcmp((char*)method, (char*)"get") == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init("int");
                }
            }
        } else if ((strcmp((char*)hobj_ty->name, (char*)"RwLock") == 0)) {
            /* pass */
            if (((((strcmp((char*)method, (char*)"read") == 0) || (strcmp((char*)method, (char*)"read_lock") == 0)) || (strcmp((char*)method, (char*)"write") == 0)) || (strcmp((char*)method, (char*)"write_lock") == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init("int");
                }
            }
        } else if (((strcmp((char*)hobj_ty->name, (char*)"StringBuilder") == 0) && (strcmp((char*)method, (char*)"to_string") == 0))) {
            /* pass */
            ret_ty = AstType_init("StringObj");
        } else if ((strcmp((char*)hobj_ty->name, (char*)"Shared") == 0)) {
            /* pass */
            if ((strcmp((char*)method, (char*)"clone") == 0)) {
                /* pass */
                ret_ty = hobj_ty;
            } else if ((strcmp((char*)method, (char*)"is_null") == 0)) {
                /* pass */
                ret_ty = AstType_init("bool");
            } else if ((strcmp((char*)method, (char*)"downgrade") == 0)) {
                /* pass */
                AstType* weak_ty = AstType_init("Weak");
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    weak_ty->args = (void*)List_ptr_new();
                    /* pass */
                    List_ptr_append(weak_ty->args, box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL)))));
                }
                /* pass */
                ret_ty = weak_ty;
            } else if ((strcmp((char*)method, (char*)"drop") == 0)) {
                /* pass */
                ret_ty = AstType_init("void");
            }
        } else if ((strcmp((char*)hobj_ty->name, (char*)"Weak") == 0)) {
            /* pass */
            if ((strcmp((char*)method, (char*)"upgrade") == 0)) {
                /* pass */
                AstType* opt_ty = AstType_init("Option");
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    opt_ty->args = (void*)List_ptr_new();
                    /* pass */
                    List_ptr_append(opt_ty->args, box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL)))));
                }
                /* pass */
                ret_ty = opt_ty;
            } else if ((strcmp((char*)method, (char*)"is_alive") == 0)) {
                /* pass */
                ret_ty = AstType_init("bool");
            }
        } else if (((((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))) && _tr_dict_contains(self->interfaces, hobj_ty->name))) {
            /* pass */
            InterfaceDef* _iface_def = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, hobj_ty->name));
            /* pass */
            long long _imi = 0LL;
            /* pass */
            while ((_imi < _iface_def->methods->len)) {
                /* pass */
                FunctionDef* _imdef = ((FunctionDef*)List_ptr_get(_iface_def->methods, _imi));
                /* pass */
                if ((strcmp((char*)_imdef->name, (char*)method) == 0)) {
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
                    if ((strcmp((char*)List_str_get(_iface_def->generics, _gi5), (char*)ret_ty->name) == 0)) {
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
        } else if (_tr_dict_contains(self->enums, hobj_ty->name)) {
            /* pass */
            ret_ty = AstType_init(hobj_ty->name);
        } else if (_tr_dict_contains(self->classes, hobj_ty->name)) {
            /* pass */
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, hobj_ty->name));
            /* pass */
            char* _hty_n = hobj_ty->name;
            /* pass */
            bool _is_builtin_dispatch = ((((((((((((((((strcmp((char*)_hty_n, (char*)"Thread") == 0) || (strcmp((char*)_hty_n, (char*)"Atomic") == 0)) || (strcmp((char*)_hty_n, (char*)"ThreadLocal") == 0)) || (strcmp((char*)_hty_n, (char*)"ThreadPool") == 0)) || (strcmp((char*)_hty_n, (char*)"Mutex") == 0)) || (strcmp((char*)_hty_n, (char*)"RwLock") == 0)) || (strcmp((char*)_hty_n, (char*)"Chan") == 0)) || (strcmp((char*)_hty_n, (char*)"Channel") == 0)) || (strcmp((char*)_hty_n, (char*)"Shared") == 0)) || (strcmp((char*)_hty_n, (char*)"Weak") == 0)) || (strcmp((char*)_hty_n, (char*)"StringBuilder") == 0)) || (strcmp((char*)_hty_n, (char*)"OS") == 0)) || (strcmp((char*)_hty_n, (char*)"Process") == 0)) || (strcmp((char*)_hty_n, (char*)"Env") == 0)) || (strcmp((char*)_hty_n, (char*)"Hash") == 0)) || (strcmp((char*)_hty_n, (char*)"File") == 0));
            /* pass */
            if ((((!_is_builtin_dispatch) && (!Sema_class_method_exists(self, hobj_ty->name, method))) && (!Sema_is_universal_method(self, method)))) {
                /* pass */
                Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[E-1] No method '", method), "' found on type '"), hobj_ty->name), "'.\n      FIX: Define 'pub def "), method), "(self, ...)' in '"), hobj_ty->name), "' or its base class via 'extend "), hobj_ty->name), ":'."));
            }
            /* pass */
            long long _mi = 0LL;
            /* pass */
            while ((_mi < _cls->methods->len)) {
                /* pass */
                FunctionDef* _mdef = ((FunctionDef*)List_ptr_get(_cls->methods, _mi));
                /* pass */
                if ((strcmp((char*)_mdef->name, (char*)method) == 0)) {
                    /* pass */
                    if ((((unsigned long long)(_mdef->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        long long _mpc = 0LL;
                        /* pass */
                        long long _mpi = 0LL;
                        /* pass */
                        while ((_mpi < _mdef->params->len)) {
                            /* pass */
                            if ((strcmp((char*)((Param*)List_ptr_get(_mdef->params, _mpi))->name, (char*)"self") != 0)) {
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
                        } else if ((strcmp((char*)ret_ty->name, (char*)"void") == 0)) {
                            /* pass */
                            ret_ty = (*_mdef->ret_ty);
                        }
                    }
                }
                /* pass */
                _mi = (_mi + 1LL);
            }
            /* pass */
            if (((_cls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                long long _gi4 = 0LL;
                /* pass */
                while ((_gi4 < _cls->generics->len)) {
                    /* pass */
                    if ((strcmp((char*)List_str_get(_cls->generics, _gi4), (char*)ret_ty->name) == 0)) {
                        /* pass */
                        if ((_gi4 < hobj_ty->args->len)) {
                            /* pass */
                            ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, _gi4)));
                        }
                    }
                    /* pass */
                    _gi4 = (_gi4 + 1LL);
                }
            }
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EMethodCall(hobj, method, hl, ret_ty));
    } else if (_t153.tag == Expr_EPropAccess) {
        __auto_type obj = _t153.data.EPropAccess.obj;
__auto_type prop = _t153.data.EPropAccess.prop;
        /* pass */
        HirExpr* hobj = Sema_lower_expr(self, obj);
        /* pass */
        char* hobj_ty_n = hir_expr_type(hobj)->name;
        /* pass */
        AstType* hobj_ty_full = hir_expr_type(hobj);
        /* pass */
        AstType* ret_ty = AstType_init("void");
        /* pass */
        if ((strcmp((char*)hobj_ty_n, (char*)"Result") == 0)) {
            /* pass */
            if (((strcmp((char*)prop, (char*)"is_err") == 0) || (strcmp((char*)prop, (char*)"is_ok") == 0))) {
                /* pass */
                ret_ty = AstType_init("bool");
            } else if (((strcmp((char*)prop, (char*)"ok") == 0) && (hobj_ty_full->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            } else if (((strcmp((char*)prop, (char*)"err") == 0) && (hobj_ty_full->args->len > 1LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 1LL)));
            }
        } else if ((strcmp((char*)hobj_ty_n, (char*)"Option") == 0)) {
            /* pass */
            if (((strcmp((char*)prop, (char*)"is_some") == 0) || (strcmp((char*)prop, (char*)"is_none") == 0))) {
                /* pass */
                ret_ty = AstType_init("bool");
            } else if ((((strcmp((char*)prop, (char*)"value") == 0) || (strcmp((char*)prop, (char*)"val") == 0)) && (hobj_ty_full->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            }
        } else if ((strcmp((char*)hobj_ty_n, (char*)"StringObj") == 0)) {
            /* pass */
            if ((strcmp((char*)prop, (char*)"data") == 0)) {
                /* pass */
                ret_ty = AstType_init("Pointer");
            } else if (((strcmp((char*)prop, (char*)"length") == 0) || (strcmp((char*)prop, (char*)"capacity") == 0))) {
                /* pass */
                ret_ty = AstType_init("int");
            }
        } else if ((strcmp((char*)hobj_ty_n, (char*)"StringBuilder") == 0)) {
            /* pass */
            if ((strcmp((char*)prop, (char*)"buf") == 0)) {
                /* pass */
                ret_ty = AstType_init("StringObj");
            }
        } else if (((strcmp((char*)hobj_ty_n, (char*)"Vec") == 0) || (strcmp((char*)hobj_ty_n, (char*)"List") == 0))) {
            /* pass */
            if ((((strcmp((char*)prop, (char*)"len") == 0) || (strcmp((char*)prop, (char*)"length") == 0)) || (strcmp((char*)prop, (char*)"capacity") == 0))) {
                /* pass */
                ret_ty = AstType_init("int");
            } else if ((strcmp((char*)prop, (char*)"data") == 0)) {
                /* pass */
                ret_ty = AstType_init("Pointer");
            }
        } else if ((((strcmp((char*)hobj_ty_n, (char*)"Map") == 0) || (strcmp((char*)hobj_ty_n, (char*)"Dict") == 0)) || (strcmp((char*)hobj_ty_n, (char*)"Set") == 0))) {
            /* pass */
            if ((((strcmp((char*)prop, (char*)"len") == 0) || (strcmp((char*)prop, (char*)"length") == 0)) || (strcmp((char*)prop, (char*)"capacity") == 0))) {
                /* pass */
                ret_ty = AstType_init("int");
            }
        } else if (_tr_dict_contains(self->classes, hobj_ty_n)) {
            /* pass */
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, hobj_ty_n));
            /* pass */
            long long _fi = 0LL;
            /* pass */
            while ((_fi < _cls->fields->len)) {
                /* pass */
                FieldDef* _fld = ((FieldDef*)List_ptr_get(_cls->fields, _fi));
                /* pass */
                if ((strcmp((char*)_fld->name, (char*)prop) == 0)) {
                    /* pass */
                    if ((((unsigned long long)(_fld->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        ret_ty = (*_fld->ty);
                    }
                }
                /* pass */
                _fi = (_fi + 1LL);
            }
        }
        /* pass */
        if (((strcmp((char*)ret_ty->name, (char*)"void") == 0) && (((strcmp((char*)prop, (char*)"len") == 0) || (strcmp((char*)prop, (char*)"length") == 0)) || (strcmp((char*)prop, (char*)"capacity") == 0)))) {
            /* pass */
            ret_ty = AstType_init("int");
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EPropAccess(hobj, prop, ret_ty));
    } else if (_t153.tag == Expr_EIndex) {
        __auto_type obj = _t153.data.EIndex.obj;
__auto_type idx_inner = _t153.data.EIndex.index;
        /* pass */
        HirExpr* hexpr_obj = Sema_lower_expr(self, obj);
        /* pass */
        char* obj_ty_n = hir_expr_type(hexpr_obj)->name;
        /* pass */
        char* obj_name = "";
        /* pass */
        __auto_type _t174 = (*obj);
        if (_t174.tag == Expr_EIdent) {
            __auto_type n = _t174.data.EIdent.name;
            obj_name = n;
        } else if (1) {
            __auto_type _ = _t174;
            /* pass */
        }
        /* pass */
        bool is_generic = false;
        /* pass */
        char* generic_arg_n = "";
        /* pass */
        if ((((unsigned long long)(idx_inner)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return hexpr_obj;
        }
        /* pass */
        __auto_type _t175 = (*idx_inner);
        if (_t175.tag == Expr_EIdent) {
            __auto_type iname = _t175.data.EIdent.name;
            /* pass */
            bool is_param = false;
            /* pass */
            if ((strcmp((char*)self->current_class_name, (char*)"") != 0)) {
                /* pass */
                if (_tr_dict_contains(self->classes, self->current_class_name)) {
                    /* pass */
                    ClassDef* cc = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, self->current_class_name));
                    /* pass */
                    long long ci = 0LL;
                    /* pass */
                    while ((ci < cc->generics->len)) {
                        /* pass */
                        if ((strcmp((char*)List_str_get(cc->generics, ci), (char*)iname) == 0)) {
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
                if ((strcmp((char*)List_str_get(self->current_func_generics, fi), (char*)iname) == 0)) {
                    /* pass */
                    is_param = true;
                }
                /* pass */
                fi = (fi + 1LL);
            }
            /* pass */
            if ((((((((((((Sema_is_primitive_name(self, iname) || (strcmp((char*)iname, (char*)"str") == 0)) || (strcmp((char*)iname, (char*)"Str") == 0)) || is_param) || (_tr_strlen(iname) == 1LL)) || _tr_dict_contains(self->classes, iname)) || (strcmp((char*)iname, (char*)"StringObj") == 0)) || (strcmp((char*)iname, (char*)"StringBuilder") == 0)) || _tr_dict_contains(self->enums, iname)) || (strcmp((char*)iname, (char*)"Vec") == 0)) || (strcmp((char*)iname, (char*)"List") == 0)) || (strcmp((char*)iname, (char*)"Map") == 0))) {
                /* pass */
                is_generic = true;
                /* pass */
                generic_arg_n = iname;
            }
        } else if (_t175.tag == Expr_EIndex) {
            /* pass */
            is_generic = true;
            /* pass */
            AstType** nested_ty = Sema_build_ast_type(self, idx_inner);
            /* pass */
            if ((((unsigned long long)(nested_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                generic_arg_n = (*nested_ty)->name;
            }
        } else if (1) {
            __auto_type _ = _t175;
            /* pass */
        }
        /* pass */
        bool obj_is_type = ((((((((((((strcmp((char*)obj_name, (char*)"Vec") == 0) || (strcmp((char*)obj_name, (char*)"List") == 0)) || (strcmp((char*)obj_name, (char*)"Map") == 0)) || (strcmp((char*)obj_name, (char*)"Dict") == 0)) || (strcmp((char*)obj_name, (char*)"Set") == 0)) || (strcmp((char*)obj_name, (char*)"Pointer") == 0)) || (strcmp((char*)obj_name, (char*)"alloc") == 0)) || (strcmp((char*)obj_name, (char*)"dealloc") == 0)) || (strcmp((char*)obj_name, (char*)"resize") == 0)) || (strcmp((char*)obj_name, (char*)"copy") == 0)) || _tr_dict_contains(self->classes, obj_name)) || _tr_dict_contains(self->enums, obj_name));
        /* pass */
        if ((is_generic && obj_is_type)) {
            /* pass */
            char* eff_ty_n = obj_ty_n;
            /* pass */
            if ((((strcmp((char*)eff_ty_n, (char*)"void") == 0) || (strcmp((char*)eff_ty_n, (char*)"") == 0)) && ((((((strcmp((char*)obj_name, (char*)"Vec") == 0) || (strcmp((char*)obj_name, (char*)"List") == 0)) || (strcmp((char*)obj_name, (char*)"Map") == 0)) || (strcmp((char*)obj_name, (char*)"Dict") == 0)) || (strcmp((char*)obj_name, (char*)"Set") == 0)) || (strcmp((char*)obj_name, (char*)"Pointer") == 0)))) {
                /* pass */
                eff_ty_n = obj_name;
            }
            /* pass */
            if ((((((((strcmp((char*)eff_ty_n, (char*)"Vec") == 0) || (strcmp((char*)eff_ty_n, (char*)"List") == 0)) || (strcmp((char*)eff_ty_n, (char*)"Map") == 0)) || (strcmp((char*)eff_ty_n, (char*)"Dict") == 0)) || (strcmp((char*)eff_ty_n, (char*)"Set") == 0)) || (strcmp((char*)eff_ty_n, (char*)"Pointer") == 0)) && (strcmp((char*)generic_arg_n, (char*)"") != 0))) {
                /* pass */
                AstType* container_ty = AstType_init(eff_ty_n);
                /* pass */
                container_ty->args = (void*)List_ptr_new();
                /* pass */
                List_ptr_append(container_ty->args, box_asttype(AstType_init(generic_arg_n)));
                /* pass */
                if (((((strcmp((char*)obj_name, (char*)"alloc") == 0) || (strcmp((char*)obj_name, (char*)"dealloc") == 0)) || (strcmp((char*)obj_name, (char*)"resize") == 0)) || (strcmp((char*)obj_name, (char*)"copy") == 0))) {
                    /* pass */
                    return box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_expr(self, idx_inner), container_ty));
                }
                /* pass */
                return box_hirexpr(HirExpr_ctor_EIdent(eff_ty_n, container_ty, false));
            }
            /* pass */
            if (((_tr_dict_contains(self->classes, obj_name) || _tr_dict_contains(self->enums, obj_name)) && (strcmp((char*)generic_arg_n, (char*)"") != 0))) {
                /* pass */
                AstType* cls_ty = AstType_init(obj_name);
                /* pass */
                cls_ty->args = (void*)List_ptr_new();
                /* pass */
                List_ptr_append(cls_ty->args, box_asttype(AstType_init(generic_arg_n)));
                /* pass */
                return box_hirexpr(HirExpr_ctor_EIdent(obj_name, cls_ty, false));
            }
            /* pass */
            return hexpr_obj;
        }
        /* pass */
        if ((((((!Sema_is_primitive_name(self, obj_ty_n)) && (strcmp((char*)obj_ty_n, (char*)"str") != 0)) && (strcmp((char*)obj_ty_n, (char*)"Pointer") != 0)) && (strcmp((char*)obj_ty_n, (char*)"List") != 0)) && (strcmp((char*)obj_ty_n, (char*)"Vec") != 0))) {
            /* pass */
            List_ptr* call_args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(call_args, Sema_lower_expr(self, idx_inner));
            /* pass */
            return box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, "get_index", call_args, AstType_init("void")));
        }
        /* pass */
        AstType* elem_ty = AstType_init("void");
        /* pass */
        if (((strcmp((char*)obj_ty_n, (char*)"List") == 0) || (strcmp((char*)obj_ty_n, (char*)"Vec") == 0))) {
            /* pass */
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
                /* pass */
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if ((strcmp((char*)obj_ty_n, (char*)"Pointer") == 0)) {
            /* pass */
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
                /* pass */
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if ((strcmp((char*)obj_ty_n, (char*)"str") == 0)) {
            /* pass */
            elem_ty = AstType_init("char");
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_expr(self, idx_inner), elem_ty));
    } else if (_t153.tag == Expr_ESizeOf) {
        __auto_type ty = _t153.data.ESizeOf.ty;
        /* pass */
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return box_hirexpr(HirExpr_ctor_ESizeOf(AstType_init("void"), AstType_init("int")));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ESizeOf((*ty), AstType_init("int")));
    } else if (_t153.tag == Expr_ECast) {
        __auto_type expr = _t153.data.ECast.expr;
__auto_type ty = _t153.data.ECast.ty;
        /* pass */
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return Sema_lower_expr(self, expr);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ECast(Sema_lower_expr(self, expr), (*ty)));
    } else if (_t153.tag == Expr_EFString) {
        __auto_type parts = _t153.data.EFString.parts;
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
            hp->text = p_ast->text;
            /* pass */
            hp->fmt_spec = p_ast->fmt_spec;
            /* pass */
            hp->expr = Sema_lower_expr(self, p_ast->expr);
            /* pass */
            List_ptr_append(hparts, hp);
            /* pass */
            m = (m + 1LL);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EFString(hparts, AstType_init("str")));
    } else if (_t153.tag == Expr_ETuple) {
        __auto_type items = _t153.data.ETuple.items;
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
        return box_hirexpr(HirExpr_ctor_ETuple(hitems, AstType_init("Tuple")));
    } else if (_t153.tag == Expr_EList) {
        __auto_type items = _t153.data.EList.items;
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
        AstType* list_ty = AstType_init("List");
        /* pass */
        if ((hitems->len > 0LL)) {
            /* pass */
            list_ty->args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(list_ty->args, box_asttype(hir_expr_type(List_ptr_get(hitems, 0LL))));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EList(hitems, list_ty));
    } else if (_t153.tag == Expr_EClosure) {
        __auto_type params = _t153.data.EClosure.params;
__auto_type ret_ty = _t153.data.EClosure.ret_ty;
__auto_type body = _t153.data.EClosure.body;
__auto_type is_async = _t153.data.EClosure.is_async;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_ptr* hparams = (void*)List_ptr_new();
        /* pass */
        long long l = 0LL;
        /* pass */
        while ((l < params->len)) {
            /* pass */
            Param* pa = ((Param*)List_ptr_get(params, l));
            /* pass */
            AstType* pa_ty = AstType_init("int");
            /* pass */
            if ((((unsigned long long)(pa->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                pa_ty = (*pa->ty);
            }
            /* pass */
            Sema_declare(self, pa->name, SymbolKind_make_SVariable(), box_asttype(pa_ty), false);
            /* pass */
            HirParam* hpa = ((HirParam*)_tr_checked_alloc(sizeof(HirParam)));
            /* pass */
            hpa->name = pa->name;
            /* pass */
            hpa->ty = pa_ty;
            /* pass */
            List_ptr_append(hparams, hpa);
            /* pass */
            l = (l + 1LL);
        }
        /* pass */
        AstType* r_ty = AstType_init("void");
        /* pass */
        if ((((unsigned long long)(ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            r_ty = (*ret_ty);
        }
        /* pass */
        HirExpr hexpr = HirExpr_ctor_EClosure(hparams, r_ty, Sema_lower_block(self, body), is_async, (void*)List_ptr_new());
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirexpr(hexpr);
    } else if (_t153.tag == Expr_EIfElse) {
        __auto_type cond = _t153.data.EIfElse.cond;
__auto_type then_e = _t153.data.EIfElse.then_expr;
__auto_type else_e = _t153.data.EIfElse.else_expr;
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
    } else if (_t153.tag == Expr_ETryExpr) {
        __auto_type inner = _t153.data.ETryExpr.expr;
        /* pass */
        HirExpr* hinner = Sema_lower_expr(self, inner);
        /* pass */
        AstType* inner_ty = hir_expr_type(hinner);
        /* pass */
        AstType* ok_ty = AstType_init("void");
        /* pass */
        if (((strcmp((char*)inner_ty->name, (char*)"Result") == 0) && (inner_ty->args->len > 0LL))) {
            /* pass */
            ok_ty = (*((AstType**)List_ptr_get(inner_ty->args, 0LL)));
        } else if ((strcmp((char*)inner_ty->name, (char*)"void") != 0)) {
            /* pass */
            ok_ty = inner_ty;
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ETryExpr(hinner, ok_ty));
    } else if (_t153.tag == Expr_EAwait) {
        __auto_type inner_await = _t153.data.EAwait.expr;
        /* pass */
        if ((!self->in_async_fn)) {
            /* pass */
            Sema_error(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("[C-4] 'await' used outside an async function. FIX: Declare '", self->current_func_name), "' as 'async def "), self->current_func_name), "(...)' to use await inside it."));
        }
        /* pass */
        HirExpr* hinner_await = Sema_lower_expr(self, inner_await);
        /* pass */
        AstType* await_ty = hir_expr_type(hinner_await);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EAwait(hinner_await, await_ty));
    } else if (_t153.tag == Expr_EDict) {
        __auto_type keys = _t153.data.EDict.keys;
__auto_type vals = _t153.data.EDict.vals;
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
        return box_hirexpr(HirExpr_ctor_EDict(h_keys, h_vals, AstType_init("Dict")));
    } else if (_t153.tag == Expr_EListComp) {
        __auto_type element = _t153.data.EListComp.element;
__auto_type generators = _t153.data.EListComp.generators;
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
            HirComprehension** hgen_ptr = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
            /* pass */
            HirComprehension* hgen_val = ((HirComprehension*)_tr_checked_alloc(sizeof(HirComprehension)));
            /* pass */
            hgen_val->target = gen_ast->target;
            /* pass */
            HirExpr* h_iter_lc = Sema_lower_expr(self, gen_ast->iter);
            /* pass */
            hgen_val->iter = h_iter_lc;
            /* pass */
            char* lc_itn = hir_expr_type(h_iter_lc)->name;
            /* pass */
            long long lc_ial = hir_expr_type(h_iter_lc)->args->len;
            /* pass */
            AstType* lc_elem_ty = AstType_init("int");
            /* pass */
            if ((((strcmp((char*)lc_itn, (char*)"List") == 0) || (strcmp((char*)lc_itn, (char*)"Vec") == 0)) && (lc_ial > 0LL))) {
                /* pass */
                lc_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_lc)->args, 0LL)));
            } else if ((strcmp((char*)lc_itn, (char*)"str") == 0)) {
                /* pass */
                lc_elem_ty = AstType_init("char");
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
            (*hgen_ptr = hgen_val);
            /* pass */
            List_ptr_append(hgens, hgen_ptr);
            /* pass */
            gc = (gc + 1LL);
        }
        /* pass */
        HirExpr* h_lc_elem = Sema_lower_expr(self, element);
        /* pass */
        AstType* lc_elem_hty = hir_expr_type(h_lc_elem);
        /* pass */
        AstType* comp_ty = AstType_init("List");
        /* pass */
        comp_ty->args = (void*)List_ptr_new();
        /* pass */
        List_ptr_append(comp_ty->args, box_asttype(lc_elem_hty));
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EListComp(h_lc_elem, hgens, comp_ty));
    } else if (_t153.tag == Expr_EGeneratorExpr) {
        __auto_type element = _t153.data.EGeneratorExpr.element;
__auto_type generators = _t153.data.EGeneratorExpr.generators;
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
            HirComprehension** hgen_ptr2 = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
            /* pass */
            HirComprehension* hgen_val2 = ((HirComprehension*)_tr_checked_alloc(sizeof(HirComprehension)));
            /* pass */
            hgen_val2->target = gen_ast2->target;
            /* pass */
            HirExpr* h_iter_ge = Sema_lower_expr(self, gen_ast2->iter);
            /* pass */
            hgen_val2->iter = h_iter_ge;
            /* pass */
            char* ge_itn = hir_expr_type(h_iter_ge)->name;
            /* pass */
            long long ge_ial = hir_expr_type(h_iter_ge)->args->len;
            /* pass */
            AstType* ge_elem_ty = AstType_init("int");
            /* pass */
            if ((((strcmp((char*)ge_itn, (char*)"List") == 0) || (strcmp((char*)ge_itn, (char*)"Vec") == 0)) && (ge_ial > 0LL))) {
                /* pass */
                ge_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_ge)->args, 0LL)));
            } else if ((strcmp((char*)ge_itn, (char*)"str") == 0)) {
                /* pass */
                ge_elem_ty = AstType_init("char");
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
            (*hgen_ptr2 = hgen_val2);
            /* pass */
            List_ptr_append(hgens2, hgen_ptr2);
            /* pass */
            gc2 = (gc2 + 1LL);
        }
        /* pass */
        HirExpr* h_ge_elem = Sema_lower_expr(self, element);
        /* pass */
        AstType* ge_elem_hty = hir_expr_type(h_ge_elem);
        /* pass */
        AstType* gen_ty = AstType_init("List");
        /* pass */
        gen_ty->args = (void*)List_ptr_new();
        /* pass */
        List_ptr_append(gen_ty->args, box_asttype(ge_elem_hty));
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EGeneratorExpr(h_ge_elem, hgens2, gen_ty));
    } else if (_t153.tag == Expr_ESuperMethodCall) {
        __auto_type base_class = _t153.data.ESuperMethodCall.base_class;
__auto_type method = _t153.data.ESuperMethodCall.method;
__auto_type args = _t153.data.ESuperMethodCall.args;
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
        char* resolved_base = base_class;
        /* pass */
        if (((strcmp((char*)resolved_base, (char*)"") == 0) && (strcmp((char*)self->current_class_name, (char*)"") != 0))) {
            /* pass */
            if (_tr_dict_contains(self->classes, self->current_class_name)) {
                /* pass */
                ClassDef* cur_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, self->current_class_name));
                /* pass */
                if ((cur_cls->base_classes->len > 0LL)) {
                    /* pass */
                    resolved_base = List_str_get(cur_cls->base_classes, 0LL);
                }
            }
        }
        /* pass */
        AstType* super_ret_ty = AstType_init("void");
        /* pass */
        if (_tr_dict_contains(self->classes, resolved_base)) {
            /* pass */
            ClassDef* bc_def = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, resolved_base));
            /* pass */
            long long k_smc2 = 0LL;
            /* pass */
            while ((k_smc2 < bc_def->methods->len)) {
                /* pass */
                if ((strcmp((char*)((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->name, (char*)method) == 0)) {
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
    } else if (_t153.tag == Expr_ESuperPropAccess) {
        __auto_type base_class = _t153.data.ESuperPropAccess.base_class;
__auto_type prop = _t153.data.ESuperPropAccess.prop;
        /* pass */
        char* resolved_base2 = base_class;
        /* pass */
        if (((strcmp((char*)resolved_base2, (char*)"") == 0) && (strcmp((char*)self->current_class_name, (char*)"") != 0))) {
            /* pass */
            if (_tr_dict_contains(self->classes, self->current_class_name)) {
                /* pass */
                ClassDef* cur_cls2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, self->current_class_name));
                /* pass */
                if ((cur_cls2->base_classes->len > 0LL)) {
                    /* pass */
                    resolved_base2 = List_str_get(cur_cls2->base_classes, 0LL);
                }
            }
        }
        /* pass */
        AstType* super_field_ty = AstType_init("void");
        /* pass */
        if (_tr_dict_contains(self->classes, resolved_base2)) {
            /* pass */
            ClassDef* bc_def2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, resolved_base2));
            /* pass */
            long long k_spa = 0LL;
            /* pass */
            while ((k_spa < bc_def2->fields->len)) {
                /* pass */
                if ((strcmp((char*)((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->name, (char*)prop) == 0)) {
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
        __auto_type _ = _t153;
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init("None")));
    }
}

__attribute__((hot)) char* Sema_is_reserved_error(Sema* self, char* name) {
    /* pass */
    if ((((strcmp((char*)name, (char*)"true") == 0) || (strcmp((char*)name, (char*)"True") == 0)) || (strcmp((char*)name, (char*)"gaskiya") == 0))) {
        /* pass */
        return "built-in boolean constant 'true'";
    }
    /* pass */
    if ((((strcmp((char*)name, (char*)"false") == 0) || (strcmp((char*)name, (char*)"False") == 0)) || (strcmp((char*)name, (char*)"karya") == 0))) {
        /* pass */
        return "built-in boolean constant 'false'";
    }
    /* pass */
    if ((((strcmp((char*)name, (char*)"none") == 0) || (strcmp((char*)name, (char*)"None") == 0)) || (strcmp((char*)name, (char*)"babu") == 0))) {
        /* pass */
        return "built-in null constant 'none'";
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"Some") == 0)) {
        /* pass */
        return "built-in Option constructor 'Some'";
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"Ok") == 0)) {
        /* pass */
        return "built-in Result constructor 'Ok'";
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"Err") == 0)) {
        /* pass */
        return "built-in Result constructor 'Err'";
    }
    /* pass */
    return "";
}

__attribute__((hot)) char* Sema_is_reserved_keyword(Sema* self, char* name) {
    /* pass */
    if (((strcmp((char*)name, (char*)"print") == 0) || (strcmp((char*)name, (char*)"input") == 0))) {
        /* pass */
        return "built-in I/O function";
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"range") == 0)) {
        /* pass */
        return "built-in range function";
    }
    /* pass */
    if (((strcmp((char*)name, (char*)"len") == 0) || (strcmp((char*)name, (char*)"tsawon") == 0))) {
        /* pass */
        return "built-in len function";
    }
    /* pass */
    if (((strcmp((char*)name, (char*)"type") == 0) || (strcmp((char*)name, (char*)"zuwa_rubutu") == 0))) {
        /* pass */
        return "built-in type function";
    }
    /* pass */
    if (((((strcmp((char*)name, (char*)"abs") == 0) || (strcmp((char*)name, (char*)"max") == 0)) || (strcmp((char*)name, (char*)"min") == 0)) || (strcmp((char*)name, (char*)"sum") == 0))) {
        /* pass */
        return "built-in math function";
    }
    /* pass */
    if (((((strcmp((char*)name, (char*)"str") == 0) || (strcmp((char*)name, (char*)"int") == 0)) || (strcmp((char*)name, (char*)"float") == 0)) || (strcmp((char*)name, (char*)"bool") == 0))) {
        /* pass */
        return "built-in primitive type";
    }
    /* pass */
    if (((strcmp((char*)name, (char*)"List") == 0) || (strcmp((char*)name, (char*)"Dict") == 0))) {
        /* pass */
        return "built-in container type";
    }
    /* pass */
    if (((strcmp((char*)name, (char*)"Option") == 0) || (strcmp((char*)name, (char*)"Result") == 0))) {
        /* pass */
        return "built-in enum type";
    }
    /* pass */
    if (((strcmp((char*)name, (char*)"Exception") == 0) || (strcmp((char*)name, (char*)"Error") == 0))) {
        /* pass */
        return "built-in exception type";
    }
    /* pass */
    return "";
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
            __auto_type _t176 = (*sl_s);
            if (_t176.tag == Stmt_SLine) {
                __auto_type _ = _t176.data.SLine.n;
                is_sline = true;
            } else if (1) {
                __auto_type _ = _t176;
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
    __auto_type _t177 = (*last_s);
    if (_t177.tag == Stmt_SReturn) {
        __auto_type _ = _t177.data.SReturn.val;
        return true;
    } else if (_t177.tag == Stmt_SRaise) {
        __auto_type _ = _t177.data.SRaise.val;
        return true;
    } else if (_t177.tag == Stmt_SUnsafe) {
        __auto_type body = _t177.data.SUnsafe.body;
        return Sema_block_returns(self, body);
    } else if (_t177.tag == Stmt_SIf) {
        __auto_type cond = _t177.data.SIf.cond;
__auto_type then_b = _t177.data.SIf.then_b;
__auto_type elifs = _t177.data.SIf.elifs;
__auto_type else_b = _t177.data.SIf.else_b;
        /* pass */
        if ((else_b->stmts->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        return (Sema_block_returns(self, then_b) && Sema_block_returns(self, else_b));
    } else if (_t177.tag == Stmt_SMatch) {
        __auto_type subj = _t177.data.SMatch.expr;
__auto_type arms = _t177.data.SMatch.arms;
        /* pass */
        bool has_wild = false;
        /* pass */
        bool all_ret = true;
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < arms->len)) {
            /* pass */
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, mi));
            /* pass */
            __auto_type _t178 = arm->pat;
            if (_t178.tag == Pattern_PWild) {
                has_wild = true;
            } else if (_t178.tag == Pattern_PBind) {
                __auto_type _ = _t178.data.PBind.name;
                has_wild = true;
            } else if (1) {
                __auto_type _ = _t178;
                /* pass */
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
        return (has_wild && all_ret);
    } else if (1) {
        __auto_type _ = _t177;
        return false;
    }
}

__attribute__((hot)) bool Sema_is_primitive(Sema* self, AstType* ty) {
    /* pass */
    return Sema_is_primitive_name(self, ty->name);
}

__attribute__((hot)) bool Sema_is_primitive_name(Sema* self, char* name) {
    /* pass */
    if (((((((strcmp((char*)name, (char*)"int") == 0) || (strcmp((char*)name, (char*)"float") == 0)) || (strcmp((char*)name, (char*)"bool") == 0)) || (strcmp((char*)name, (char*)"char") == 0)) || (strcmp((char*)name, (char*)"void") == 0)) || (strcmp((char*)name, (char*)"None") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)name, (char*)"i64") == 0) || (strcmp((char*)name, (char*)"i32") == 0)) || (strcmp((char*)name, (char*)"i16") == 0)) || (strcmp((char*)name, (char*)"i8") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp((char*)name, (char*)"u64") == 0) || (strcmp((char*)name, (char*)"u32") == 0)) || (strcmp((char*)name, (char*)"u16") == 0)) || (strcmp((char*)name, (char*)"u8") == 0)) || (strcmp((char*)name, (char*)"usize") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)name, (char*)"f64") == 0) || (strcmp((char*)name, (char*)"f32") == 0)) || (strcmp((char*)name, (char*)"lambda") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)name, (char*)"str") == 0) || (strcmp((char*)name, (char*)"Str") == 0)) || (strcmp((char*)name, (char*)"StringObj") == 0)) || (strcmp((char*)name, (char*)"Bytes") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp((char*)name, (char*)"Chan") == 0) || (strcmp((char*)name, (char*)"Channel") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)name, (char*)"Mutex") == 0) || (strcmp((char*)name, (char*)"RwLock") == 0)) || (strcmp((char*)name, (char*)"Atomic") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)name, (char*)"Thread") == 0) || (strcmp((char*)name, (char*)"ThreadPool") == 0)) || (strcmp((char*)name, (char*)"ThreadLocal") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"Pointer") == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp((char*)name, (char*)"ref") == 0) || (strcmp((char*)name, (char*)"mut_ref") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) Symbol** box_symbol(Symbol* s) {
    /* pass */
    Symbol** p = ((Symbol**)_tr_c_calloc((size_t)(1LL), sizeof(Symbol*)));
    /* pass */
    (*p = s);
    /* pass */
    return p;
}

