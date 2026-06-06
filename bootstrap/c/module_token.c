#include "tauraro_types.h"


__attribute__((hot)) bool Token_is_eof(Token self) {
    /* pass */
    __auto_type _t1 = self;
    if (_t1.tag == Token_Eof) {
        /* pass */
        return true;
    } else if (1) {
        __auto_type _ = _t1;
        /* pass */
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Token_is_newline(Token self) {
    /* pass */
    __auto_type _t2 = self;
    if (_t2.tag == Token_Newline) {
        /* pass */
        return true;
    } else if (_t2.tag == Token_Indent) {
        /* pass */
        return true;
    } else if (_t2.tag == Token_Dedent) {
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

__attribute__((hot)) bool Token_is_keyword(Token self) {
    /* pass */
    __auto_type _t3 = self;
    if (_t3.tag == Token_KwDef) {
        return true;
    } else if (_t3.tag == Token_KwClass) {
        return true;
    } else if (_t3.tag == Token_KwIf) {
        return true;
    } else if (_t3.tag == Token_KwElif) {
        return true;
    } else if (_t3.tag == Token_KwElse) {
        return true;
    } else if (_t3.tag == Token_KwFor) {
        return true;
    } else if (_t3.tag == Token_KwWhile) {
        return true;
    } else if (_t3.tag == Token_KwReturn) {
        return true;
    } else if (_t3.tag == Token_KwMatch) {
        return true;
    } else if (_t3.tag == Token_KwTrue) {
        return true;
    } else if (_t3.tag == Token_KwFalse) {
        return true;
    } else if (_t3.tag == Token_KwNone) {
        return true;
    } else if (_t3.tag == Token_KwAnd) {
        return true;
    } else if (_t3.tag == Token_KwOr) {
        return true;
    } else if (_t3.tag == Token_KwNot) {
        return true;
    } else if (_t3.tag == Token_KwMut) {
        return true;
    } else if (_t3.tag == Token_KwEnum) {
        return true;
    } else if (_t3.tag == Token_KwWhere) {
        return true;
    } else if (_t3.tag == Token_KwStatic) {
        return true;
    } else if (_t3.tag == Token_KwStack) {
        return true;
    } else if (_t3.tag == Token_KwOwn) {
        return true;
    } else if (_t3.tag == Token_KwBorrow) {
        return true;
    } else if (_t3.tag == Token_KwMove) {
        return true;
    } else if (_t3.tag == Token_KwConst) {
        return true;
    } else if (_t3.tag == Token_KwActor) {
        return true;
    } else if (_t3.tag == Token_KwSuper) {
        return true;
    } else if (_t3.tag == Token_KwExport) {
        return true;
    } else if (_t3.tag == Token_KwLambda) {
        return true;
    } else if (_t3.tag == Token_KwDecorator) {
        return true;
    } else if (_t3.tag == Token_KwInt) {
        return true;
    } else if (_t3.tag == Token_KwFloat) {
        return true;
    } else if (_t3.tag == Token_KwBool) {
        return true;
    } else if (_t3.tag == Token_KwI8) {
        return true;
    } else if (_t3.tag == Token_KwI16) {
        return true;
    } else if (_t3.tag == Token_KwI32) {
        return true;
    } else if (_t3.tag == Token_KwI64) {
        return true;
    } else if (_t3.tag == Token_KwI128) {
        return true;
    } else if (_t3.tag == Token_KwISize) {
        return true;
    } else if (_t3.tag == Token_KwU8) {
        return true;
    } else if (_t3.tag == Token_KwU16) {
        return true;
    } else if (_t3.tag == Token_KwU32) {
        return true;
    } else if (_t3.tag == Token_KwU64) {
        return true;
    } else if (_t3.tag == Token_KwU128) {
        return true;
    } else if (_t3.tag == Token_KwUSize) {
        return true;
    } else if (_t3.tag == Token_KwF32) {
        return true;
    } else if (_t3.tag == Token_KwF64) {
        return true;
    } else if (_t3.tag == Token_KwBoolTy) {
        return true;
    } else if (_t3.tag == Token_KwChar) {
        return true;
    } else if (_t3.tag == Token_KwStr) {
        return true;
    } else if (_t3.tag == Token_KwString) {
        return true;
    } else if (_t3.tag == Token_KwVoid) {
        return true;
    } else if (1) {
        __auto_type _ = _t3;
        /* pass */
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) char* Token_debug(Token self) {
    /* pass */
    __auto_type _t4 = self;
    if (_t4.tag == Token_Ident) {
        __auto_type name = _t4.data.Ident.name;
        return name;
    } else if (_t4.tag == Token_IntLit) {
        __auto_type val = _t4.data.IntLit.val;
        return ({ int _fz = snprintf(NULL,0,"IntLit(%lld)", (long long)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"IntLit(%lld)", (long long)(val)); _fr; });
    } else if (_t4.tag == Token_FloatLit) {
        __auto_type val = _t4.data.FloatLit.val;
        return ({ int _fz = snprintf(NULL,0,"FloatLit(%g)", (double)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"FloatLit(%g)", (double)(val)); _fr; });
    } else if (_t4.tag == Token_StrLit) {
        __auto_type val = _t4.data.StrLit.val;
        return ({ int _fz = snprintf(NULL,0,"StrLit(%s)", (char*)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"StrLit(%s)", (char*)(val)); _fr; });
    } else if (_t4.tag == Token_TripleStrLit) {
        __auto_type val = _t4.data.TripleStrLit.val;
        return ({ int _fz = snprintf(NULL,0,"TripleStrLit(%s)", (char*)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"TripleStrLit(%s)", (char*)(val)); _fr; });
    } else if (_t4.tag == Token_ByteStrLit) {
        __auto_type val = _t4.data.ByteStrLit.val;
        return ({ int _fz = snprintf(NULL,0,"ByteStrLit(%s)", (char*)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"ByteStrLit(%s)", (char*)(val)); _fr; });
    } else if (_t4.tag == Token_RawStrLit) {
        __auto_type val = _t4.data.RawStrLit.val;
        return ({ int _fz = snprintf(NULL,0,"RawStrLit(%s)", (char*)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"RawStrLit(%s)", (char*)(val)); _fr; });
    } else if (_t4.tag == Token_CharLit) {
        __auto_type val = _t4.data.CharLit.val;
        return ({ int _fz = snprintf(NULL,0,"CharLit(%lld)", (long long)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"CharLit(%lld)", (long long)(val)); _fr; });
    } else if (_t4.tag == Token_FStrLit) {
        __auto_type val = _t4.data.FStrLit.val;
        return ({ int _fz = snprintf(NULL,0,"FStrLit(%s)", (char*)(val)); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"FStrLit(%s)", (char*)(val)); _fr; });
    } else if (_t4.tag == Token_BoolLit) {
        __auto_type val = _t4.data.BoolLit.val;
        return ({ int _fz = snprintf(NULL,0,"BoolLit(%s)", ((val) ? "true" : "false")); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,"BoolLit(%s)", ((val) ? "true" : "false")); _fr; });
    } else if (_t4.tag == Token_KwDef) {
        return "def";
    } else if (_t4.tag == Token_KwClass) {
        return "class";
    } else if (_t4.tag == Token_KwIf) {
        return "if";
    } else if (_t4.tag == Token_KwElif) {
        return "elif";
    } else if (_t4.tag == Token_KwElse) {
        return "else";
    } else if (_t4.tag == Token_KwFor) {
        return "for";
    } else if (_t4.tag == Token_KwWhile) {
        return "while";
    } else if (_t4.tag == Token_KwReturn) {
        return "return";
    } else if (_t4.tag == Token_KwMatch) {
        return "match";
    } else if (_t4.tag == Token_KwTrue) {
        return "True";
    } else if (_t4.tag == Token_KwFalse) {
        return "False";
    } else if (_t4.tag == Token_KwNone) {
        return "None";
    } else if (_t4.tag == Token_KwAnd) {
        return "and";
    } else if (_t4.tag == Token_KwOr) {
        return "or";
    } else if (_t4.tag == Token_KwNot) {
        return "not";
    } else if (_t4.tag == Token_KwIs) {
        return "is";
    } else if (_t4.tag == Token_KwMut) {
        return "mut";
    } else if (_t4.tag == Token_KwEnum) {
        return "enum";
    } else if (_t4.tag == Token_KwWhere) {
        return "where";
    } else if (_t4.tag == Token_KwStatic) {
        return "static";
    } else if (_t4.tag == Token_KwStack) {
        return "stack";
    } else if (_t4.tag == Token_KwOwn) {
        return "own";
    } else if (_t4.tag == Token_KwBorrow) {
        return "borrow";
    } else if (_t4.tag == Token_KwMove) {
        return "move";
    } else if (_t4.tag == Token_KwConst) {
        return "const";
    } else if (_t4.tag == Token_KwActor) {
        return "actor";
    } else if (_t4.tag == Token_KwSuper) {
        return "super";
    } else if (_t4.tag == Token_KwExport) {
        return "export";
    } else if (_t4.tag == Token_KwLambda) {
        return "lambda";
    } else if (_t4.tag == Token_KwDecorator) {
        return "decorator";
    } else if (_t4.tag == Token_KwInt) {
        return "int";
    } else if (_t4.tag == Token_KwFloat) {
        return "float";
    } else if (_t4.tag == Token_KwBool) {
        return "bool";
    } else if (_t4.tag == Token_KwI8) {
        return "i8";
    } else if (_t4.tag == Token_KwI16) {
        return "i16";
    } else if (_t4.tag == Token_KwI32) {
        return "i32";
    } else if (_t4.tag == Token_KwI64) {
        return "i64";
    } else if (_t4.tag == Token_KwI128) {
        return "i128";
    } else if (_t4.tag == Token_KwISize) {
        return "isize";
    } else if (_t4.tag == Token_KwU8) {
        return "u8";
    } else if (_t4.tag == Token_KwU16) {
        return "u16";
    } else if (_t4.tag == Token_KwU32) {
        return "u32";
    } else if (_t4.tag == Token_KwU64) {
        return "u64";
    } else if (_t4.tag == Token_KwU128) {
        return "u128";
    } else if (_t4.tag == Token_KwUSize) {
        return "usize";
    } else if (_t4.tag == Token_KwF32) {
        return "f32";
    } else if (_t4.tag == Token_KwF64) {
        return "f64";
    } else if (_t4.tag == Token_KwBoolTy) {
        return "bool";
    } else if (_t4.tag == Token_KwChar) {
        return "char";
    } else if (_t4.tag == Token_KwStr) {
        return "str";
    } else if (_t4.tag == Token_KwString) {
        return "String";
    } else if (_t4.tag == Token_KwVoid) {
        return "void";
    } else if (_t4.tag == Token_Plus) {
        return "+";
    } else if (_t4.tag == Token_Minus) {
        return "-";
    } else if (_t4.tag == Token_Star) {
        return "*";
    } else if (_t4.tag == Token_Slash) {
        return "/";
    } else if (_t4.tag == Token_EqEq) {
        return "==";
    } else if (_t4.tag == Token_NotEq) {
        return "!=";
    } else if (_t4.tag == Token_Lt) {
        return "<";
    } else if (_t4.tag == Token_Gt) {
        return ">";
    } else if (_t4.tag == Token_LtEq) {
        return "<=";
    } else if (_t4.tag == Token_GtEq) {
        return ">=";
    } else if (_t4.tag == Token_Eq) {
        return "=";
    } else if (_t4.tag == Token_Arrow) {
        return "->";
    } else if (_t4.tag == Token_LParen) {
        return "(";
    } else if (_t4.tag == Token_RParen) {
        return ")";
    } else if (_t4.tag == Token_LBracket) {
        return "[";
    } else if (_t4.tag == Token_RBracket) {
        return "]";
    } else if (_t4.tag == Token_Colon) {
        return ":";
    } else if (_t4.tag == Token_Comma) {
        return ",";
    } else if (_t4.tag == Token_Dot) {
        return ".";
    } else if (_t4.tag == Token_DotDot) {
        return "..";
    } else if (_t4.tag == Token_DotDotEq) {
        return "..=";
    } else if (_t4.tag == Token_DotDotDot) {
        return "...";
    } else if (_t4.tag == Token_Indent) {
        return "INDENT";
    } else if (_t4.tag == Token_Dedent) {
        return "DEDENT";
    } else if (_t4.tag == Token_Newline) {
        return "NEWLINE";
    } else if (_t4.tag == Token_Eof) {
        return "EOF";
    } else if (_t4.tag == Token_Error) {
        __auto_type msg = _t4.data.Error.msg;
        return msg;
    } else if (1) {
        __auto_type _ = _t4;
        /* pass */
        /* pass */
    }
    /* pass */
    return "?";
}

