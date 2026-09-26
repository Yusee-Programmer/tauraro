#include "tauraro_types.h"


__attribute__((hot)) bool Token_is_eof(Token self) {
    /* pass */
    __auto_type _t2 = self;
    if (_t2.tag == Token_Eof) {
        /* pass */
        return true;
    } else if (1) {
        __auto_type _ = _t2;
        /* pass */
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Token_is_newline(Token self) {
    /* pass */
    __auto_type _t3 = self;
    if (_t3.tag == Token_Newline) {
        /* pass */
        return true;
    } else if (_t3.tag == Token_Indent) {
        /* pass */
        return true;
    } else if (_t3.tag == Token_Dedent) {
        /* pass */
        return true;
    } else if (1) {
        __auto_type _ = _t3;
        /* pass */
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Token_is_keyword(Token self) {
    /* pass */
    __auto_type _t4 = self;
    if (_t4.tag == Token_KwDef) {
        return true;
    } else if (_t4.tag == Token_KwClass) {
        return true;
    } else if (_t4.tag == Token_KwIf) {
        return true;
    } else if (_t4.tag == Token_KwElif) {
        return true;
    } else if (_t4.tag == Token_KwElse) {
        return true;
    } else if (_t4.tag == Token_KwFor) {
        return true;
    } else if (_t4.tag == Token_KwWhile) {
        return true;
    } else if (_t4.tag == Token_KwLoop) {
        return true;
    } else if (_t4.tag == Token_KwReturn) {
        return true;
    } else if (_t4.tag == Token_KwMatch) {
        return true;
    } else if (_t4.tag == Token_KwTrue) {
        return true;
    } else if (_t4.tag == Token_KwFalse) {
        return true;
    } else if (_t4.tag == Token_KwNone) {
        return true;
    } else if (_t4.tag == Token_KwAnd) {
        return true;
    } else if (_t4.tag == Token_KwOr) {
        return true;
    } else if (_t4.tag == Token_KwNot) {
        return true;
    } else if (_t4.tag == Token_KwMut) {
        return true;
    } else if (_t4.tag == Token_KwEnum) {
        return true;
    } else if (_t4.tag == Token_KwWhere) {
        return true;
    } else if (_t4.tag == Token_KwStatic) {
        return true;
    } else if (_t4.tag == Token_KwStack) {
        return true;
    } else if (_t4.tag == Token_KwOwn) {
        return true;
    } else if (_t4.tag == Token_KwBorrow) {
        return true;
    } else if (_t4.tag == Token_KwMove) {
        return true;
    } else if (_t4.tag == Token_KwConst) {
        return true;
    } else if (_t4.tag == Token_KwActor) {
        return true;
    } else if (_t4.tag == Token_KwSuper) {
        return true;
    } else if (_t4.tag == Token_KwExport) {
        return true;
    } else if (_t4.tag == Token_KwLambda) {
        return true;
    } else if (_t4.tag == Token_KwDecorator) {
        return true;
    } else if (_t4.tag == Token_KwMacro) {
        return true;
    } else if (_t4.tag == Token_KwDo) {
        return true;
    } else if (_t4.tag == Token_KwInt) {
        return true;
    } else if (_t4.tag == Token_KwFloat) {
        return true;
    } else if (_t4.tag == Token_KwBool) {
        return true;
    } else if (_t4.tag == Token_KwI8) {
        return true;
    } else if (_t4.tag == Token_KwI16) {
        return true;
    } else if (_t4.tag == Token_KwI32) {
        return true;
    } else if (_t4.tag == Token_KwI64) {
        return true;
    } else if (_t4.tag == Token_KwI128) {
        return true;
    } else if (_t4.tag == Token_KwISize) {
        return true;
    } else if (_t4.tag == Token_KwU8) {
        return true;
    } else if (_t4.tag == Token_KwU16) {
        return true;
    } else if (_t4.tag == Token_KwU32) {
        return true;
    } else if (_t4.tag == Token_KwU64) {
        return true;
    } else if (_t4.tag == Token_KwU128) {
        return true;
    } else if (_t4.tag == Token_KwUSize) {
        return true;
    } else if (_t4.tag == Token_KwF32) {
        return true;
    } else if (_t4.tag == Token_KwF64) {
        return true;
    } else if (_t4.tag == Token_KwBoolTy) {
        return true;
    } else if (_t4.tag == Token_KwChar) {
        return true;
    } else if (_t4.tag == Token_KwStr) {
        return true;
    } else if (_t4.tag == Token_KwString) {
        return true;
    } else if (_t4.tag == Token_KwVoid) {
        return true;
    } else if (1) {
        __auto_type _ = _t4;
        /* pass */
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr Token_debug(Token self) {
    /* pass */
    __auto_type _t5 = self;
    if (_t5.tag == Token_Ident) {
        __auto_type name = _t5.data.Ident.name;
        return _tr_str_retain(name);
    } else if (_t5.tag == Token_IntLit) {
        __auto_type val = _t5.data.IntLit.val;
        return ({ int _fz = snprintf(NULL,0,"IntLit(%lld)", (long long)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"IntLit(%lld)", (long long)(val)); _tr_str_wrap_len(_fr, (size_t)_fz); });
    } else if (_t5.tag == Token_FloatLit) {
        __auto_type val = _t5.data.FloatLit.val;
        return ({ int _fz = snprintf(NULL,0,"FloatLit(%g)", (double)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"FloatLit(%g)", (double)(val)); _tr_str_wrap_len(_fr, (size_t)_fz); });
    } else if (_t5.tag == Token_StrLit) {
        __auto_type val = _t5.data.StrLit.val;
        return ({ int _fz = snprintf(NULL,0,"StrLit(%s)", _tr_strz(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"StrLit(%s)", _tr_strz(val)); _tr_str_wrap_len(_fr, (size_t)_fz); });
    } else if (_t5.tag == Token_TripleStrLit) {
        __auto_type val = _t5.data.TripleStrLit.val;
        return ({ int _fz = snprintf(NULL,0,"TripleStrLit(%s)", _tr_strz(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"TripleStrLit(%s)", _tr_strz(val)); _tr_str_wrap_len(_fr, (size_t)_fz); });
    } else if (_t5.tag == Token_ByteStrLit) {
        __auto_type val = _t5.data.ByteStrLit.val;
        return ({ int _fz = snprintf(NULL,0,"ByteStrLit(%s)", _tr_strz(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"ByteStrLit(%s)", _tr_strz(val)); _tr_str_wrap_len(_fr, (size_t)_fz); });
    } else if (_t5.tag == Token_RawStrLit) {
        __auto_type val = _t5.data.RawStrLit.val;
        return ({ int _fz = snprintf(NULL,0,"RawStrLit(%s)", _tr_strz(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"RawStrLit(%s)", _tr_strz(val)); _tr_str_wrap_len(_fr, (size_t)_fz); });
    } else if (_t5.tag == Token_CharLit) {
        __auto_type val = _t5.data.CharLit.val;
        return ({ int _fz = snprintf(NULL,0,"CharLit(%lld)", (long long)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"CharLit(%lld)", (long long)(val)); _tr_str_wrap_len(_fr, (size_t)_fz); });
    } else if (_t5.tag == Token_FStrLit) {
        __auto_type val = _t5.data.FStrLit.val;
        return ({ int _fz = snprintf(NULL,0,"FStrLit(%s)", _tr_strz(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"FStrLit(%s)", _tr_strz(val)); _tr_str_wrap_len(_fr, (size_t)_fz); });
    } else if (_t5.tag == Token_BoolLit) {
        __auto_type val = _t5.data.BoolLit.val;
        return ({ int _fz = snprintf(NULL,0,"BoolLit(%s)", ((val) ? "true" : "false")); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"BoolLit(%s)", ((val) ? "true" : "false")); _tr_str_wrap_len(_fr, (size_t)_fz); });
    } else if (_t5.tag == Token_KwDef) {
        return _tr_str_lit_len("def", 3LL);
    } else if (_t5.tag == Token_KwClass) {
        return _tr_str_lit_len("class", 5LL);
    } else if (_t5.tag == Token_KwIf) {
        return _tr_str_lit_len("if", 2LL);
    } else if (_t5.tag == Token_KwElif) {
        return _tr_str_lit_len("elif", 4LL);
    } else if (_t5.tag == Token_KwElse) {
        return _tr_str_lit_len("else", 4LL);
    } else if (_t5.tag == Token_KwFor) {
        return _tr_str_lit_len("for", 3LL);
    } else if (_t5.tag == Token_KwWhile) {
        return _tr_str_lit_len("while", 5LL);
    } else if (_t5.tag == Token_KwLoop) {
        return _tr_str_lit_len("loop", 4LL);
    } else if (_t5.tag == Token_KwReturn) {
        return _tr_str_lit_len("return", 6LL);
    } else if (_t5.tag == Token_KwMatch) {
        return _tr_str_lit_len("match", 5LL);
    } else if (_t5.tag == Token_KwTrue) {
        return _tr_str_lit_len("True", 4LL);
    } else if (_t5.tag == Token_KwFalse) {
        return _tr_str_lit_len("False", 5LL);
    } else if (_t5.tag == Token_KwNone) {
        return _tr_str_lit_len("None", 4LL);
    } else if (_t5.tag == Token_KwAnd) {
        return _tr_str_lit_len("and", 3LL);
    } else if (_t5.tag == Token_KwOr) {
        return _tr_str_lit_len("or", 2LL);
    } else if (_t5.tag == Token_KwNot) {
        return _tr_str_lit_len("not", 3LL);
    } else if (_t5.tag == Token_KwIs) {
        return _tr_str_lit_len("is", 2LL);
    } else if (_t5.tag == Token_KwMut) {
        return _tr_str_lit_len("mut", 3LL);
    } else if (_t5.tag == Token_KwEnum) {
        return _tr_str_lit_len("enum", 4LL);
    } else if (_t5.tag == Token_KwWhere) {
        return _tr_str_lit_len("where", 5LL);
    } else if (_t5.tag == Token_KwStatic) {
        return _tr_str_lit_len("static", 6LL);
    } else if (_t5.tag == Token_KwStack) {
        return _tr_str_lit_len("stack", 5LL);
    } else if (_t5.tag == Token_KwOwn) {
        return _tr_str_lit_len("own", 3LL);
    } else if (_t5.tag == Token_KwBorrow) {
        return _tr_str_lit_len("borrow", 6LL);
    } else if (_t5.tag == Token_KwMove) {
        return _tr_str_lit_len("move", 4LL);
    } else if (_t5.tag == Token_KwConst) {
        return _tr_str_lit_len("const", 5LL);
    } else if (_t5.tag == Token_KwActor) {
        return _tr_str_lit_len("actor", 5LL);
    } else if (_t5.tag == Token_KwSuper) {
        return _tr_str_lit_len("super", 5LL);
    } else if (_t5.tag == Token_KwExport) {
        return _tr_str_lit_len("export", 6LL);
    } else if (_t5.tag == Token_KwLambda) {
        return _tr_str_lit_len("lambda", 6LL);
    } else if (_t5.tag == Token_KwDecorator) {
        return _tr_str_lit_len("decorator", 9LL);
    } else if (_t5.tag == Token_KwMacro) {
        return _tr_str_lit_len("macro", 5LL);
    } else if (_t5.tag == Token_KwDo) {
        return _tr_str_lit_len("do", 2LL);
    } else if (_t5.tag == Token_KwInt) {
        return _tr_str_lit_len("int", 3LL);
    } else if (_t5.tag == Token_KwFloat) {
        return _tr_str_lit_len("float", 5LL);
    } else if (_t5.tag == Token_KwBool) {
        return _tr_str_lit_len("bool", 4LL);
    } else if (_t5.tag == Token_KwI8) {
        return _tr_str_lit_len("i8", 2LL);
    } else if (_t5.tag == Token_KwI16) {
        return _tr_str_lit_len("i16", 3LL);
    } else if (_t5.tag == Token_KwI32) {
        return _tr_str_lit_len("i32", 3LL);
    } else if (_t5.tag == Token_KwI64) {
        return _tr_str_lit_len("i64", 3LL);
    } else if (_t5.tag == Token_KwI128) {
        return _tr_str_lit_len("i128", 4LL);
    } else if (_t5.tag == Token_KwISize) {
        return _tr_str_lit_len("isize", 5LL);
    } else if (_t5.tag == Token_KwU8) {
        return _tr_str_lit_len("u8", 2LL);
    } else if (_t5.tag == Token_KwU16) {
        return _tr_str_lit_len("u16", 3LL);
    } else if (_t5.tag == Token_KwU32) {
        return _tr_str_lit_len("u32", 3LL);
    } else if (_t5.tag == Token_KwU64) {
        return _tr_str_lit_len("u64", 3LL);
    } else if (_t5.tag == Token_KwU128) {
        return _tr_str_lit_len("u128", 4LL);
    } else if (_t5.tag == Token_KwUSize) {
        return _tr_str_lit_len("usize", 5LL);
    } else if (_t5.tag == Token_KwF32) {
        return _tr_str_lit_len("f32", 3LL);
    } else if (_t5.tag == Token_KwF64) {
        return _tr_str_lit_len("f64", 3LL);
    } else if (_t5.tag == Token_KwBoolTy) {
        return _tr_str_lit_len("bool", 4LL);
    } else if (_t5.tag == Token_KwChar) {
        return _tr_str_lit_len("char", 4LL);
    } else if (_t5.tag == Token_KwStr) {
        return _tr_str_lit_len("str", 3LL);
    } else if (_t5.tag == Token_KwString) {
        return _tr_str_lit_len("String", 6LL);
    } else if (_t5.tag == Token_KwVoid) {
        return _tr_str_lit_len("void", 4LL);
    } else if (_t5.tag == Token_Plus) {
        return _tr_str_lit_len("+", 1LL);
    } else if (_t5.tag == Token_Minus) {
        return _tr_str_lit_len("-", 1LL);
    } else if (_t5.tag == Token_Star) {
        return _tr_str_lit_len("*", 1LL);
    } else if (_t5.tag == Token_Slash) {
        return _tr_str_lit_len("/", 1LL);
    } else if (_t5.tag == Token_EqEq) {
        return _tr_str_lit_len("==", 2LL);
    } else if (_t5.tag == Token_NotEq) {
        return _tr_str_lit_len("!=", 2LL);
    } else if (_t5.tag == Token_Bang) {
        return _tr_str_lit_len("!", 1LL);
    } else if (_t5.tag == Token_Lt) {
        return _tr_str_lit_len("<", 1LL);
    } else if (_t5.tag == Token_Gt) {
        return _tr_str_lit_len(">", 1LL);
    } else if (_t5.tag == Token_LtEq) {
        return _tr_str_lit_len("<=", 2LL);
    } else if (_t5.tag == Token_GtEq) {
        return _tr_str_lit_len(">=", 2LL);
    } else if (_t5.tag == Token_Eq) {
        return _tr_str_lit_len("=", 1LL);
    } else if (_t5.tag == Token_Arrow) {
        return _tr_str_lit_len("->", 2LL);
    } else if (_t5.tag == Token_LParen) {
        return _tr_str_lit_len("(", 1LL);
    } else if (_t5.tag == Token_RParen) {
        return _tr_str_lit_len(")", 1LL);
    } else if (_t5.tag == Token_LBracket) {
        return _tr_str_lit_len("[", 1LL);
    } else if (_t5.tag == Token_RBracket) {
        return _tr_str_lit_len("]", 1LL);
    } else if (_t5.tag == Token_Colon) {
        return _tr_str_lit_len(":", 1LL);
    } else if (_t5.tag == Token_Comma) {
        return _tr_str_lit_len(",", 1LL);
    } else if (_t5.tag == Token_Dot) {
        return _tr_str_lit_len(".", 1LL);
    } else if (_t5.tag == Token_DotDot) {
        return _tr_str_lit_len("..", 2LL);
    } else if (_t5.tag == Token_DotDotEq) {
        return _tr_str_lit_len("..=", 3LL);
    } else if (_t5.tag == Token_DotDotDot) {
        return _tr_str_lit_len("...", 3LL);
    } else if (_t5.tag == Token_Indent) {
        return _tr_str_lit_len("INDENT", 6LL);
    } else if (_t5.tag == Token_Dedent) {
        return _tr_str_lit_len("DEDENT", 6LL);
    } else if (_t5.tag == Token_Newline) {
        return _tr_str_lit_len("NEWLINE", 7LL);
    } else if (_t5.tag == Token_Eof) {
        return _tr_str_lit_len("EOF", 3LL);
    } else if (_t5.tag == Token_Error) {
        __auto_type msg = _t5.data.Error.msg;
        return _tr_str_retain(msg);
    } else if (1) {
        __auto_type _ = _t5;
        /* pass */
        /* pass */
    }
    /* pass */
    return _tr_str_lit_len("?", 1LL);
}

