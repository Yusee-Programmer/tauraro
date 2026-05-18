#pragma once
#include "tauraro_rt.h"

typedef struct Lexer Lexer;
typedef struct AstType AstType;
typedef struct GenericConstraint GenericConstraint;
typedef struct Decorator Decorator;
typedef struct Comprehension Comprehension;
typedef struct CatchClause CatchClause;
typedef struct MatchArm MatchArm;
typedef struct FStringPart FStringPart;
typedef struct Block Block;
typedef struct ElifClause ElifClause;
typedef struct Param Param;
typedef struct FunctionDef FunctionDef;
typedef struct FieldDef FieldDef;
typedef struct ClassDef ClassDef;
typedef struct VariantDef VariantDef;
typedef struct EnumDef EnumDef;
typedef struct InterfaceDef InterfaceDef;
typedef struct ImportItem ImportItem;
typedef struct Program Program;
typedef struct Parser Parser;
typedef struct ModuleResolver ModuleResolver;
typedef struct HirComprehension HirComprehension;
typedef struct HirCatchClause HirCatchClause;
typedef struct HirFStringPart HirFStringPart;
typedef struct HirMatchArm HirMatchArm;
typedef struct HirBlock HirBlock;
typedef struct HirParam HirParam;
typedef struct HirFunction HirFunction;
typedef struct HirField HirField;
typedef struct HirClass HirClass;
typedef struct HirVariant HirVariant;
typedef struct HirEnum HirEnum;
typedef struct HirInterface HirInterface;
typedef struct HirProgram HirProgram;
typedef struct Symbol Symbol;
typedef struct Scope Scope;
typedef struct Sema Sema;
typedef struct CGenerator CGenerator;
typedef struct LlvmGenerator LlvmGenerator;
typedef struct Token Token;
typedef struct Pattern Pattern;
typedef struct Ownership Ownership;
typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct Decl Decl;
typedef struct HirExpr HirExpr;
typedef struct HirStmt HirStmt;
typedef struct SymbolKind SymbolKind;
typedef struct List_Token List_Token;
typedef struct List_Pattern List_Pattern;

typedef enum {
    Token_IntLit,
    Token_FloatLit,
    Token_StrLit,
    Token_ByteStrLit,
    Token_RawStrLit,
    Token_CharLit,
    Token_FStrLit,
    Token_BoolLit,
    Token_Ident,
    Token_KwDef,
    Token_KwClass,
    Token_KwEnum,
    Token_KwInterface,
    Token_KwExtend,
    Token_KwStruct,
    Token_KwIf,
    Token_KwElif,
    Token_KwElse,
    Token_KwFor,
    Token_KwWhile,
    Token_KwReturn,
    Token_KwBreak,
    Token_KwContinue,
    Token_KwMatch,
    Token_KwCase,
    Token_KwTry,
    Token_KwAssert,
    Token_KwWith,
    Token_KwAsm,
    Token_KwTaskGroup,
    Token_KwGpu,
    Token_KwSizeOf,
    Token_KwExcept,
    Token_KwFinally,
    Token_KwRaise,
    Token_KwPass,
    Token_KwImplements,
    Token_KwImport,
    Token_KwFrom,
    Token_KwAs,
    Token_KwIn,
    Token_KwMut,
    Token_KwShared,
    Token_KwExtern,
    Token_KwUnsafe,
    Token_KwSpawn,
    Token_KwAsync,
    Token_KwThrows,
    Token_KwExtends,
    Token_KwAwait,
    Token_KwYield,
    Token_KwPub,
    Token_KwWhere,
    Token_KwStatic,
    Token_KwStack,
    Token_KwOwn,
    Token_KwBorrow,
    Token_KwMove,
    Token_KwConst,
    Token_KwActor,
    Token_KwSuper,
    Token_KwExport,
    Token_KwLambda,
    Token_KwDecorator,
    Token_KwTrue,
    Token_KwFalse,
    Token_KwNone,
    Token_KwAnd,
    Token_KwOr,
    Token_KwNot,
    Token_KwIs,
    Token_KwInt,
    Token_KwFloat,
    Token_KwBool,
    Token_KwI8,
    Token_KwI16,
    Token_KwI32,
    Token_KwI64,
    Token_KwI128,
    Token_KwISize,
    Token_KwU8,
    Token_KwU16,
    Token_KwU32,
    Token_KwU64,
    Token_KwU128,
    Token_KwUSize,
    Token_KwF32,
    Token_KwF64,
    Token_KwBoolTy,
    Token_KwChar,
    Token_KwStr,
    Token_KwString,
    Token_KwVoid,
    Token_Plus,
    Token_Minus,
    Token_Star,
    Token_Slash,
    Token_Percent,
    Token_StarStar,
    Token_FloorDiv,
    Token_FloorDivEq,
    Token_StarStarEq,
    Token_EqEq,
    Token_NotEq,
    Token_Lt,
    Token_Gt,
    Token_LtEq,
    Token_GtEq,
    Token_Amp,
    Token_Pipe,
    Token_Caret,
    Token_Tilde,
    Token_LtLt,
    Token_GtGt,
    Token_Eq,
    Token_PlusEq,
    Token_MinusEq,
    Token_StarEq,
    Token_SlashEq,
    Token_PercentEq,
    Token_AmpEq,
    Token_PipeEq,
    Token_CaretEq,
    Token_LtLtEq,
    Token_GtGtEq,
    Token_Arrow,
    Token_FatArrow,
    Token_Question,
    Token_At,
    Token_Dot,
    Token_DotDot,
    Token_DotDotEq,
    Token_DotDotDot,
    Token_LParen,
    Token_RParen,
    Token_LBracket,
    Token_RBracket,
    Token_LBrace,
    Token_RBrace,
    Token_Colon,
    Token_Comma,
    Token_Semicolon,
    Token_Hash,
    Token_Indent,
    Token_Dedent,
    Token_Newline,
    Token_Eof,
    Token_Error
} Token_tag;

typedef struct Token {
    Token_tag tag;
    union {
        struct {
            long long val;
        } IntLit;
        struct {
            double val;
        } FloatLit;
        struct {
            char* val;
        } StrLit;
        struct {
            char* val;
        } ByteStrLit;
        struct {
            char* val;
        } RawStrLit;
        struct {
            long long val;
        } CharLit;
        struct {
            char* val;
        } FStrLit;
        struct {
            bool val;
        } BoolLit;
        struct {
            char* name;
        } Ident;
        struct {
            char* msg;
        } Error;
    } data;
} Token;

static inline __attribute__((always_inline)) Token Token_ctor_IntLit(long long val) { Token _r = {.tag=Token_IntLit}; _r.data.IntLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_FloatLit(double val) { Token _r = {.tag=Token_FloatLit}; _r.data.FloatLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_StrLit(char* val) { Token _r = {.tag=Token_StrLit}; _r.data.StrLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_ByteStrLit(char* val) { Token _r = {.tag=Token_ByteStrLit}; _r.data.ByteStrLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_RawStrLit(char* val) { Token _r = {.tag=Token_RawStrLit}; _r.data.RawStrLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_CharLit(long long val) { Token _r = {.tag=Token_CharLit}; _r.data.CharLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_FStrLit(char* val) { Token _r = {.tag=Token_FStrLit}; _r.data.FStrLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_BoolLit(bool val) { Token _r = {.tag=Token_BoolLit}; _r.data.BoolLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_Ident(char* name) { Token _r = {.tag=Token_Ident}; _r.data.Ident.name = name; return _r; }
#define Token_make_KwDef() ((Token){.tag=Token_KwDef})
#define Token_make_KwClass() ((Token){.tag=Token_KwClass})
#define Token_make_KwEnum() ((Token){.tag=Token_KwEnum})
#define Token_make_KwInterface() ((Token){.tag=Token_KwInterface})
#define Token_make_KwExtend() ((Token){.tag=Token_KwExtend})
#define Token_make_KwStruct() ((Token){.tag=Token_KwStruct})
#define Token_make_KwIf() ((Token){.tag=Token_KwIf})
#define Token_make_KwElif() ((Token){.tag=Token_KwElif})
#define Token_make_KwElse() ((Token){.tag=Token_KwElse})
#define Token_make_KwFor() ((Token){.tag=Token_KwFor})
#define Token_make_KwWhile() ((Token){.tag=Token_KwWhile})
#define Token_make_KwReturn() ((Token){.tag=Token_KwReturn})
#define Token_make_KwBreak() ((Token){.tag=Token_KwBreak})
#define Token_make_KwContinue() ((Token){.tag=Token_KwContinue})
#define Token_make_KwMatch() ((Token){.tag=Token_KwMatch})
#define Token_make_KwCase() ((Token){.tag=Token_KwCase})
#define Token_make_KwTry() ((Token){.tag=Token_KwTry})
#define Token_make_KwAssert() ((Token){.tag=Token_KwAssert})
#define Token_make_KwWith() ((Token){.tag=Token_KwWith})
#define Token_make_KwAsm() ((Token){.tag=Token_KwAsm})
#define Token_make_KwTaskGroup() ((Token){.tag=Token_KwTaskGroup})
#define Token_make_KwGpu() ((Token){.tag=Token_KwGpu})
#define Token_make_KwSizeOf() ((Token){.tag=Token_KwSizeOf})
#define Token_make_KwExcept() ((Token){.tag=Token_KwExcept})
#define Token_make_KwFinally() ((Token){.tag=Token_KwFinally})
#define Token_make_KwRaise() ((Token){.tag=Token_KwRaise})
#define Token_make_KwPass() ((Token){.tag=Token_KwPass})
#define Token_make_KwImplements() ((Token){.tag=Token_KwImplements})
#define Token_make_KwImport() ((Token){.tag=Token_KwImport})
#define Token_make_KwFrom() ((Token){.tag=Token_KwFrom})
#define Token_make_KwAs() ((Token){.tag=Token_KwAs})
#define Token_make_KwIn() ((Token){.tag=Token_KwIn})
#define Token_make_KwMut() ((Token){.tag=Token_KwMut})
#define Token_make_KwShared() ((Token){.tag=Token_KwShared})
#define Token_make_KwExtern() ((Token){.tag=Token_KwExtern})
#define Token_make_KwUnsafe() ((Token){.tag=Token_KwUnsafe})
#define Token_make_KwSpawn() ((Token){.tag=Token_KwSpawn})
#define Token_make_KwAsync() ((Token){.tag=Token_KwAsync})
#define Token_make_KwThrows() ((Token){.tag=Token_KwThrows})
#define Token_make_KwExtends() ((Token){.tag=Token_KwExtends})
#define Token_make_KwAwait() ((Token){.tag=Token_KwAwait})
#define Token_make_KwYield() ((Token){.tag=Token_KwYield})
#define Token_make_KwPub() ((Token){.tag=Token_KwPub})
#define Token_make_KwWhere() ((Token){.tag=Token_KwWhere})
#define Token_make_KwStatic() ((Token){.tag=Token_KwStatic})
#define Token_make_KwStack() ((Token){.tag=Token_KwStack})
#define Token_make_KwOwn() ((Token){.tag=Token_KwOwn})
#define Token_make_KwBorrow() ((Token){.tag=Token_KwBorrow})
#define Token_make_KwMove() ((Token){.tag=Token_KwMove})
#define Token_make_KwConst() ((Token){.tag=Token_KwConst})
#define Token_make_KwActor() ((Token){.tag=Token_KwActor})
#define Token_make_KwSuper() ((Token){.tag=Token_KwSuper})
#define Token_make_KwExport() ((Token){.tag=Token_KwExport})
#define Token_make_KwLambda() ((Token){.tag=Token_KwLambda})
#define Token_make_KwDecorator() ((Token){.tag=Token_KwDecorator})
#define Token_make_KwTrue() ((Token){.tag=Token_KwTrue})
#define Token_make_KwFalse() ((Token){.tag=Token_KwFalse})
#define Token_make_KwNone() ((Token){.tag=Token_KwNone})
#define Token_make_KwAnd() ((Token){.tag=Token_KwAnd})
#define Token_make_KwOr() ((Token){.tag=Token_KwOr})
#define Token_make_KwNot() ((Token){.tag=Token_KwNot})
#define Token_make_KwIs() ((Token){.tag=Token_KwIs})
#define Token_make_KwInt() ((Token){.tag=Token_KwInt})
#define Token_make_KwFloat() ((Token){.tag=Token_KwFloat})
#define Token_make_KwBool() ((Token){.tag=Token_KwBool})
#define Token_make_KwI8() ((Token){.tag=Token_KwI8})
#define Token_make_KwI16() ((Token){.tag=Token_KwI16})
#define Token_make_KwI32() ((Token){.tag=Token_KwI32})
#define Token_make_KwI64() ((Token){.tag=Token_KwI64})
#define Token_make_KwI128() ((Token){.tag=Token_KwI128})
#define Token_make_KwISize() ((Token){.tag=Token_KwISize})
#define Token_make_KwU8() ((Token){.tag=Token_KwU8})
#define Token_make_KwU16() ((Token){.tag=Token_KwU16})
#define Token_make_KwU32() ((Token){.tag=Token_KwU32})
#define Token_make_KwU64() ((Token){.tag=Token_KwU64})
#define Token_make_KwU128() ((Token){.tag=Token_KwU128})
#define Token_make_KwUSize() ((Token){.tag=Token_KwUSize})
#define Token_make_KwF32() ((Token){.tag=Token_KwF32})
#define Token_make_KwF64() ((Token){.tag=Token_KwF64})
#define Token_make_KwBoolTy() ((Token){.tag=Token_KwBoolTy})
#define Token_make_KwChar() ((Token){.tag=Token_KwChar})
#define Token_make_KwStr() ((Token){.tag=Token_KwStr})
#define Token_make_KwString() ((Token){.tag=Token_KwString})
#define Token_make_KwVoid() ((Token){.tag=Token_KwVoid})
#define Token_make_Plus() ((Token){.tag=Token_Plus})
#define Token_make_Minus() ((Token){.tag=Token_Minus})
#define Token_make_Star() ((Token){.tag=Token_Star})
#define Token_make_Slash() ((Token){.tag=Token_Slash})
#define Token_make_Percent() ((Token){.tag=Token_Percent})
#define Token_make_StarStar() ((Token){.tag=Token_StarStar})
#define Token_make_FloorDiv() ((Token){.tag=Token_FloorDiv})
#define Token_make_FloorDivEq() ((Token){.tag=Token_FloorDivEq})
#define Token_make_StarStarEq() ((Token){.tag=Token_StarStarEq})
#define Token_make_EqEq() ((Token){.tag=Token_EqEq})
#define Token_make_NotEq() ((Token){.tag=Token_NotEq})
#define Token_make_Lt() ((Token){.tag=Token_Lt})
#define Token_make_Gt() ((Token){.tag=Token_Gt})
#define Token_make_LtEq() ((Token){.tag=Token_LtEq})
#define Token_make_GtEq() ((Token){.tag=Token_GtEq})
#define Token_make_Amp() ((Token){.tag=Token_Amp})
#define Token_make_Pipe() ((Token){.tag=Token_Pipe})
#define Token_make_Caret() ((Token){.tag=Token_Caret})
#define Token_make_Tilde() ((Token){.tag=Token_Tilde})
#define Token_make_LtLt() ((Token){.tag=Token_LtLt})
#define Token_make_GtGt() ((Token){.tag=Token_GtGt})
#define Token_make_Eq() ((Token){.tag=Token_Eq})
#define Token_make_PlusEq() ((Token){.tag=Token_PlusEq})
#define Token_make_MinusEq() ((Token){.tag=Token_MinusEq})
#define Token_make_StarEq() ((Token){.tag=Token_StarEq})
#define Token_make_SlashEq() ((Token){.tag=Token_SlashEq})
#define Token_make_PercentEq() ((Token){.tag=Token_PercentEq})
#define Token_make_AmpEq() ((Token){.tag=Token_AmpEq})
#define Token_make_PipeEq() ((Token){.tag=Token_PipeEq})
#define Token_make_CaretEq() ((Token){.tag=Token_CaretEq})
#define Token_make_LtLtEq() ((Token){.tag=Token_LtLtEq})
#define Token_make_GtGtEq() ((Token){.tag=Token_GtGtEq})
#define Token_make_Arrow() ((Token){.tag=Token_Arrow})
#define Token_make_FatArrow() ((Token){.tag=Token_FatArrow})
#define Token_make_Question() ((Token){.tag=Token_Question})
#define Token_make_At() ((Token){.tag=Token_At})
#define Token_make_Dot() ((Token){.tag=Token_Dot})
#define Token_make_DotDot() ((Token){.tag=Token_DotDot})
#define Token_make_DotDotEq() ((Token){.tag=Token_DotDotEq})
#define Token_make_DotDotDot() ((Token){.tag=Token_DotDotDot})
#define Token_make_LParen() ((Token){.tag=Token_LParen})
#define Token_make_RParen() ((Token){.tag=Token_RParen})
#define Token_make_LBracket() ((Token){.tag=Token_LBracket})
#define Token_make_RBracket() ((Token){.tag=Token_RBracket})
#define Token_make_LBrace() ((Token){.tag=Token_LBrace})
#define Token_make_RBrace() ((Token){.tag=Token_RBrace})
#define Token_make_Colon() ((Token){.tag=Token_Colon})
#define Token_make_Comma() ((Token){.tag=Token_Comma})
#define Token_make_Semicolon() ((Token){.tag=Token_Semicolon})
#define Token_make_Hash() ((Token){.tag=Token_Hash})
#define Token_make_Indent() ((Token){.tag=Token_Indent})
#define Token_make_Dedent() ((Token){.tag=Token_Dedent})
#define Token_make_Newline() ((Token){.tag=Token_Newline})
#define Token_make_Eof() ((Token){.tag=Token_Eof})
static inline __attribute__((always_inline)) Token Token_ctor_Error(char* msg) { Token _r = {.tag=Token_Error}; _r.data.Error.msg = msg; return _r; }

typedef enum {
    Pattern_PWild,
    Pattern_PBind,
    Pattern_PLitInt,
    Pattern_PLitStr,
    Pattern_PLitBool,
    Pattern_PVariant,
    Pattern_PVariantBind,
    Pattern_PVariantBindMany,
    Pattern_PTuple,
    Pattern_PRange,
    Pattern_POr
} Pattern_tag;

typedef struct Pattern {
    Pattern_tag tag;
    union {
        struct {
            char* name;
        } PBind;
        struct {
            long long val;
        } PLitInt;
        struct {
            char* val;
        } PLitStr;
        struct {
            bool val;
        } PLitBool;
        struct {
            char* type_name;
            char* variant;
        } PVariant;
        struct {
            char* type_name;
            char* variant;
            char* field;
        } PVariantBind;
        struct {
            char* type_name;
            char* variant;
            List_str* fields;
        } PVariantBindMany;
        struct {
            char* first;
            char* second;
        } PTuple;
        struct {
            Pattern* start;
            Pattern* end;
        } PRange;
        struct {
            List_Pattern* patterns;
        } POr;
    } data;
} Pattern;

#define Pattern_make_PWild() ((Pattern){.tag=Pattern_PWild})
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PBind(char* name) { Pattern _r = {.tag=Pattern_PBind}; _r.data.PBind.name = name; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PLitInt(long long val) { Pattern _r = {.tag=Pattern_PLitInt}; _r.data.PLitInt.val = val; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PLitStr(char* val) { Pattern _r = {.tag=Pattern_PLitStr}; _r.data.PLitStr.val = val; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PLitBool(bool val) { Pattern _r = {.tag=Pattern_PLitBool}; _r.data.PLitBool.val = val; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PVariant(char* type_name, char* variant) { Pattern _r = {.tag=Pattern_PVariant}; _r.data.PVariant.type_name = type_name; _r.data.PVariant.variant = variant; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PVariantBind(char* type_name, char* variant, char* field) { Pattern _r = {.tag=Pattern_PVariantBind}; _r.data.PVariantBind.type_name = type_name; _r.data.PVariantBind.variant = variant; _r.data.PVariantBind.field = field; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PVariantBindMany(char* type_name, char* variant, List_str* fields) { Pattern _r = {.tag=Pattern_PVariantBindMany}; _r.data.PVariantBindMany.type_name = type_name; _r.data.PVariantBindMany.variant = variant; _r.data.PVariantBindMany.fields = fields; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PTuple(char* first, char* second) { Pattern _r = {.tag=Pattern_PTuple}; _r.data.PTuple.first = first; _r.data.PTuple.second = second; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PRange(Pattern* start, Pattern* end) { Pattern _r = {.tag=Pattern_PRange}; _r.data.PRange.start = start; _r.data.PRange.end = end; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_POr(List_Pattern* patterns) { Pattern _r = {.tag=Pattern_POr}; _r.data.POr.patterns = patterns; return _r; }

typedef enum {
    Ownership_Own,
    Ownership_Borrow,
    Ownership_Move,
    Ownership_Shared,
    Ownership_Stack
} Ownership_tag;

typedef struct Ownership {
    Ownership_tag tag;
} Ownership;

#define Ownership_make_Own() ((Ownership){.tag=Ownership_Own})
#define Ownership_make_Borrow() ((Ownership){.tag=Ownership_Borrow})
#define Ownership_make_Move() ((Ownership){.tag=Ownership_Move})
#define Ownership_make_Shared() ((Ownership){.tag=Ownership_Shared})
#define Ownership_make_Stack() ((Ownership){.tag=Ownership_Stack})

typedef enum {
    Expr_ELitInt,
    Expr_ELitFloat,
    Expr_ELitStr,
    Expr_ELitBool,
    Expr_ELitChar,
    Expr_ELitBytes,
    Expr_ERawStr,
    Expr_ELitNone,
    Expr_EIdent,
    Expr_EBinOp,
    Expr_EUnaryOp,
    Expr_ECall,
    Expr_EMethodCall,
    Expr_EPropAccess,
    Expr_EIndex,
    Expr_ECast,
    Expr_EFString,
    Expr_ETryExpr,
    Expr_EClosure,
    Expr_ESuperMethodCall,
    Expr_ESuperPropAccess,
    Expr_EList,
    Expr_ESet,
    Expr_EDict,
    Expr_ETuple,
    Expr_EListComp,
    Expr_EGeneratorExpr,
    Expr_ESlice,
    Expr_EAwait,
    Expr_EYield,
    Expr_ETry,
    Expr_ERange,
    Expr_ESizeOf,
    Expr_EIfElse
} Expr_tag;

typedef struct Expr {
    Expr_tag tag;
    union {
        struct {
            long long val;
        } ELitInt;
        struct {
            double val;
        } ELitFloat;
        struct {
            char* val;
        } ELitStr;
        struct {
            bool val;
        } ELitBool;
        struct {
            long long val;
        } ELitChar;
        struct {
            char* val;
        } ELitBytes;
        struct {
            char* val;
        } ERawStr;
        struct {
            char* name;
        } EIdent;
        struct {
            char* op;
            Expr* left;
            Expr* right;
        } EBinOp;
        struct {
            char* op;
            Expr* expr;
        } EUnaryOp;
        struct {
            Expr* callee;
            List_ptr* args;
        } ECall;
        struct {
            Expr* obj;
            char* method;
            List_ptr* args;
        } EMethodCall;
        struct {
            Expr* obj;
            char* prop;
        } EPropAccess;
        struct {
            Expr* obj;
            Expr* index;
        } EIndex;
        struct {
            Expr* expr;
            AstType** ty;
        } ECast;
        struct {
            List_ptr* parts;
        } EFString;
        struct {
            Expr* expr;
        } ETryExpr;
        struct {
            List_ptr* params;
            AstType** ret_ty;
            Block* body;
            bool is_async;
        } EClosure;
        struct {
            char* base_class;
            char* method;
            List_ptr* args;
        } ESuperMethodCall;
        struct {
            char* base_class;
            char* prop;
        } ESuperPropAccess;
        struct {
            List_ptr* items;
        } EList;
        struct {
            List_ptr* items;
        } ESet;
        struct {
            List_ptr* keys;
            List_ptr* vals;
        } EDict;
        struct {
            List_ptr* items;
        } ETuple;
        struct {
            Expr* element;
            List_ptr* generators;
        } EListComp;
        struct {
            Expr* element;
            List_ptr* generators;
        } EGeneratorExpr;
        struct {
            Expr* start;
            Expr* stop;
            Expr* step;
        } ESlice;
        struct {
            Expr* expr;
        } EAwait;
        struct {
            Expr* expr;
        } EYield;
        struct {
            Block* try_body;
            List_ptr* catches;
            Block* finally_b;
        } ETry;
        struct {
            Expr* start;
            Expr* end;
            bool inclusive;
        } ERange;
        struct {
            AstType** ty;
        } ESizeOf;
        struct {
            Expr* cond;
            Expr* then_expr;
            Expr* else_expr;
        } EIfElse;
    } data;
} Expr;

static inline __attribute__((always_inline)) Expr Expr_ctor_ELitInt(long long val) { Expr _r = {.tag=Expr_ELitInt}; _r.data.ELitInt.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitFloat(double val) { Expr _r = {.tag=Expr_ELitFloat}; _r.data.ELitFloat.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitStr(char* val) { Expr _r = {.tag=Expr_ELitStr}; _r.data.ELitStr.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitBool(bool val) { Expr _r = {.tag=Expr_ELitBool}; _r.data.ELitBool.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitChar(long long val) { Expr _r = {.tag=Expr_ELitChar}; _r.data.ELitChar.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitBytes(char* val) { Expr _r = {.tag=Expr_ELitBytes}; _r.data.ELitBytes.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ERawStr(char* val) { Expr _r = {.tag=Expr_ERawStr}; _r.data.ERawStr.val = val; return _r; }
#define Expr_make_ELitNone() ((Expr){.tag=Expr_ELitNone})
static inline __attribute__((always_inline)) Expr Expr_ctor_EIdent(char* name) { Expr _r = {.tag=Expr_EIdent}; _r.data.EIdent.name = name; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EBinOp(char* op, Expr* left, Expr* right) { Expr _r = {.tag=Expr_EBinOp}; _r.data.EBinOp.op = op; _r.data.EBinOp.left = left; _r.data.EBinOp.right = right; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EUnaryOp(char* op, Expr* expr) { Expr _r = {.tag=Expr_EUnaryOp}; _r.data.EUnaryOp.op = op; _r.data.EUnaryOp.expr = expr; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ECall(Expr* callee, List_ptr* args) { Expr _r = {.tag=Expr_ECall}; _r.data.ECall.callee = callee; _r.data.ECall.args = args; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EMethodCall(Expr* obj, char* method, List_ptr* args) { Expr _r = {.tag=Expr_EMethodCall}; _r.data.EMethodCall.obj = obj; _r.data.EMethodCall.method = method; _r.data.EMethodCall.args = args; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EPropAccess(Expr* obj, char* prop) { Expr _r = {.tag=Expr_EPropAccess}; _r.data.EPropAccess.obj = obj; _r.data.EPropAccess.prop = prop; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EIndex(Expr* obj, Expr* index) { Expr _r = {.tag=Expr_EIndex}; _r.data.EIndex.obj = obj; _r.data.EIndex.index = index; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ECast(Expr* expr, AstType** ty) { Expr _r = {.tag=Expr_ECast}; _r.data.ECast.expr = expr; _r.data.ECast.ty = ty; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EFString(List_ptr* parts) { Expr _r = {.tag=Expr_EFString}; _r.data.EFString.parts = parts; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ETryExpr(Expr* expr) { Expr _r = {.tag=Expr_ETryExpr}; _r.data.ETryExpr.expr = expr; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EClosure(List_ptr* params, AstType** ret_ty, Block* body, bool is_async) { Expr _r = {.tag=Expr_EClosure}; _r.data.EClosure.params = params; _r.data.EClosure.ret_ty = ret_ty; _r.data.EClosure.body = body; _r.data.EClosure.is_async = is_async; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESuperMethodCall(char* base_class, char* method, List_ptr* args) { Expr _r = {.tag=Expr_ESuperMethodCall}; _r.data.ESuperMethodCall.base_class = base_class; _r.data.ESuperMethodCall.method = method; _r.data.ESuperMethodCall.args = args; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESuperPropAccess(char* base_class, char* prop) { Expr _r = {.tag=Expr_ESuperPropAccess}; _r.data.ESuperPropAccess.base_class = base_class; _r.data.ESuperPropAccess.prop = prop; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EList(List_ptr* items) { Expr _r = {.tag=Expr_EList}; _r.data.EList.items = items; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESet(List_ptr* items) { Expr _r = {.tag=Expr_ESet}; _r.data.ESet.items = items; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EDict(List_ptr* keys, List_ptr* vals) { Expr _r = {.tag=Expr_EDict}; _r.data.EDict.keys = keys; _r.data.EDict.vals = vals; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ETuple(List_ptr* items) { Expr _r = {.tag=Expr_ETuple}; _r.data.ETuple.items = items; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EListComp(Expr* element, List_ptr* generators) { Expr _r = {.tag=Expr_EListComp}; _r.data.EListComp.element = element; _r.data.EListComp.generators = generators; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EGeneratorExpr(Expr* element, List_ptr* generators) { Expr _r = {.tag=Expr_EGeneratorExpr}; _r.data.EGeneratorExpr.element = element; _r.data.EGeneratorExpr.generators = generators; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESlice(Expr* start, Expr* stop, Expr* step) { Expr _r = {.tag=Expr_ESlice}; _r.data.ESlice.start = start; _r.data.ESlice.stop = stop; _r.data.ESlice.step = step; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EAwait(Expr* expr) { Expr _r = {.tag=Expr_EAwait}; _r.data.EAwait.expr = expr; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EYield(Expr* expr) { Expr _r = {.tag=Expr_EYield}; _r.data.EYield.expr = expr; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ETry(Block* try_body, List_ptr* catches, Block* finally_b) { Expr _r = {.tag=Expr_ETry}; _r.data.ETry.try_body = try_body; _r.data.ETry.catches = catches; _r.data.ETry.finally_b = finally_b; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ERange(Expr* start, Expr* end, bool inclusive) { Expr _r = {.tag=Expr_ERange}; _r.data.ERange.start = start; _r.data.ERange.end = end; _r.data.ERange.inclusive = inclusive; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESizeOf(AstType** ty) { Expr _r = {.tag=Expr_ESizeOf}; _r.data.ESizeOf.ty = ty; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EIfElse(Expr* cond, Expr* then_expr, Expr* else_expr) { Expr _r = {.tag=Expr_EIfElse}; _r.data.EIfElse.cond = cond; _r.data.EIfElse.then_expr = then_expr; _r.data.EIfElse.else_expr = else_expr; return _r; }

typedef enum {
    Stmt_SExpr,
    Stmt_SLet,
    Stmt_SAssign,
    Stmt_SReturn,
    Stmt_SBreak,
    Stmt_SContinue,
    Stmt_SPass,
    Stmt_SRaise,
    Stmt_SUnsafe,
    Stmt_SIf,
    Stmt_SWhile,
    Stmt_SFor,
    Stmt_SMatch,
    Stmt_STry,
    Stmt_SAssert,
    Stmt_SWith,
    Stmt_SAsm,
    Stmt_SSpawn,
    Stmt_STaskGroup,
    Stmt_SGpuBlock
} Stmt_tag;

typedef struct Stmt {
    Stmt_tag tag;
    union {
        struct {
            Expr* expr;
        } SExpr;
        struct {
            char* name;
            Ownership ownership;
            bool is_mut;
            bool is_const;
            bool is_shared;
            AstType** ty;
            Expr* val;
        } SLet;
        struct {
            Expr* target;
            Expr* val;
        } SAssign;
        struct {
            Expr* val;
        } SReturn;
        struct {
            Expr* val;
        } SRaise;
        struct {
            Block* body;
        } SUnsafe;
        struct {
            Expr* cond;
            Block* then_b;
            List_ptr* elifs;
            Block* else_b;
        } SIf;
        struct {
            Expr* cond;
            Block* body;
            List_ptr* decorators;
        } SWhile;
        struct {
            char* var;
            Expr* iter;
            Block* body;
            List_ptr* decorators;
        } SFor;
        struct {
            Expr* expr;
            List_ptr* arms;
        } SMatch;
        struct {
            Block* try_body;
            List_ptr* catches;
            Block* finally_b;
        } STry;
        struct {
            Expr* cond;
            Expr* msg;
        } SAssert;
        struct {
            List_ptr* items;
            List_str* aliases;
            Block* body;
        } SWith;
        struct {
            char* code;
            char* outputs;
            char* inputs;
            char* clobbers;
        } SAsm;
        struct {
            Expr* expr;
        } SSpawn;
        struct {
            Block* body;
        } STaskGroup;
        struct {
            Block* body;
        } SGpuBlock;
    } data;
} Stmt;

static inline __attribute__((always_inline)) Stmt Stmt_ctor_SExpr(Expr* expr) { Stmt _r = {.tag=Stmt_SExpr}; _r.data.SExpr.expr = expr; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SLet(char* name, Ownership ownership, bool is_mut, bool is_const, bool is_shared, AstType** ty, Expr* val) { Stmt _r = {.tag=Stmt_SLet}; _r.data.SLet.name = name; _r.data.SLet.ownership = ownership; _r.data.SLet.is_mut = is_mut; _r.data.SLet.is_const = is_const; _r.data.SLet.is_shared = is_shared; _r.data.SLet.ty = ty; _r.data.SLet.val = val; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SAssign(Expr* target, Expr* val) { Stmt _r = {.tag=Stmt_SAssign}; _r.data.SAssign.target = target; _r.data.SAssign.val = val; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SReturn(Expr* val) { Stmt _r = {.tag=Stmt_SReturn}; _r.data.SReturn.val = val; return _r; }
#define Stmt_make_SBreak() ((Stmt){.tag=Stmt_SBreak})
#define Stmt_make_SContinue() ((Stmt){.tag=Stmt_SContinue})
#define Stmt_make_SPass() ((Stmt){.tag=Stmt_SPass})
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SRaise(Expr* val) { Stmt _r = {.tag=Stmt_SRaise}; _r.data.SRaise.val = val; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SUnsafe(Block* body) { Stmt _r = {.tag=Stmt_SUnsafe}; _r.data.SUnsafe.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SIf(Expr* cond, Block* then_b, List_ptr* elifs, Block* else_b) { Stmt _r = {.tag=Stmt_SIf}; _r.data.SIf.cond = cond; _r.data.SIf.then_b = then_b; _r.data.SIf.elifs = elifs; _r.data.SIf.else_b = else_b; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SWhile(Expr* cond, Block* body, List_ptr* decorators) { Stmt _r = {.tag=Stmt_SWhile}; _r.data.SWhile.cond = cond; _r.data.SWhile.body = body; _r.data.SWhile.decorators = decorators; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SFor(char* var, Expr* iter, Block* body, List_ptr* decorators) { Stmt _r = {.tag=Stmt_SFor}; _r.data.SFor.var = var; _r.data.SFor.iter = iter; _r.data.SFor.body = body; _r.data.SFor.decorators = decorators; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SMatch(Expr* expr, List_ptr* arms) { Stmt _r = {.tag=Stmt_SMatch}; _r.data.SMatch.expr = expr; _r.data.SMatch.arms = arms; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_STry(Block* try_body, List_ptr* catches, Block* finally_b) { Stmt _r = {.tag=Stmt_STry}; _r.data.STry.try_body = try_body; _r.data.STry.catches = catches; _r.data.STry.finally_b = finally_b; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SAssert(Expr* cond, Expr* msg) { Stmt _r = {.tag=Stmt_SAssert}; _r.data.SAssert.cond = cond; _r.data.SAssert.msg = msg; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SWith(List_ptr* items, List_str* aliases, Block* body) { Stmt _r = {.tag=Stmt_SWith}; _r.data.SWith.items = items; _r.data.SWith.aliases = aliases; _r.data.SWith.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SAsm(char* code, char* outputs, char* inputs, char* clobbers) { Stmt _r = {.tag=Stmt_SAsm}; _r.data.SAsm.code = code; _r.data.SAsm.outputs = outputs; _r.data.SAsm.inputs = inputs; _r.data.SAsm.clobbers = clobbers; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SSpawn(Expr* expr) { Stmt _r = {.tag=Stmt_SSpawn}; _r.data.SSpawn.expr = expr; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_STaskGroup(Block* body) { Stmt _r = {.tag=Stmt_STaskGroup}; _r.data.STaskGroup.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SGpuBlock(Block* body) { Stmt _r = {.tag=Stmt_SGpuBlock}; _r.data.SGpuBlock.body = body; return _r; }

typedef enum {
    Decl_DFunction,
    Decl_DClass,
    Decl_DEnum,
    Decl_DInterface,
    Decl_DExtend,
    Decl_DImport,
    Decl_DFromImport,
    Decl_DExtern,
    Decl_DTopLevelStmt,
    Decl_DActor,
    Decl_DDecoratorDef
} Decl_tag;

typedef struct Decl {
    Decl_tag tag;
    union {
        struct {
            FunctionDef* func;
        } DFunction;
        struct {
            ClassDef* cls;
        } DClass;
        struct {
            EnumDef* enm;
        } DEnum;
        struct {
            InterfaceDef* iface;
        } DInterface;
        struct {
            char* target;
            List_ptr* methods;
        } DExtend;
        struct {
            char* path;
            char* alias;
        } DImport;
        struct {
            char* path;
            List_ptr* items;
        } DFromImport;
        struct {
            char* abi;
            List_ptr* functions;
        } DExtern;
        struct {
            Stmt* stmt;
        } DTopLevelStmt;
        struct {
            ClassDef* cls;
        } DActor;
        struct {
            FunctionDef* func;
        } DDecoratorDef;
    } data;
} Decl;

static inline __attribute__((always_inline)) Decl Decl_ctor_DFunction(FunctionDef* func) { Decl _r = {.tag=Decl_DFunction}; _r.data.DFunction.func = func; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DClass(ClassDef* cls) { Decl _r = {.tag=Decl_DClass}; _r.data.DClass.cls = cls; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DEnum(EnumDef* enm) { Decl _r = {.tag=Decl_DEnum}; _r.data.DEnum.enm = enm; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DInterface(InterfaceDef* iface) { Decl _r = {.tag=Decl_DInterface}; _r.data.DInterface.iface = iface; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DExtend(char* target, List_ptr* methods) { Decl _r = {.tag=Decl_DExtend}; _r.data.DExtend.target = target; _r.data.DExtend.methods = methods; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DImport(char* path, char* alias) { Decl _r = {.tag=Decl_DImport}; _r.data.DImport.path = path; _r.data.DImport.alias = alias; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DFromImport(char* path, List_ptr* items) { Decl _r = {.tag=Decl_DFromImport}; _r.data.DFromImport.path = path; _r.data.DFromImport.items = items; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DExtern(char* abi, List_ptr* functions) { Decl _r = {.tag=Decl_DExtern}; _r.data.DExtern.abi = abi; _r.data.DExtern.functions = functions; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DTopLevelStmt(Stmt* stmt) { Decl _r = {.tag=Decl_DTopLevelStmt}; _r.data.DTopLevelStmt.stmt = stmt; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DActor(ClassDef* cls) { Decl _r = {.tag=Decl_DActor}; _r.data.DActor.cls = cls; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DDecoratorDef(FunctionDef* func) { Decl _r = {.tag=Decl_DDecoratorDef}; _r.data.DDecoratorDef.func = func; return _r; }

typedef enum {
    HirExpr_ELitInt,
    HirExpr_ELitFloat,
    HirExpr_ELitStr,
    HirExpr_ELitBytes,
    HirExpr_ERawStr,
    HirExpr_ELitChar,
    HirExpr_ELitBool,
    HirExpr_ELitNone,
    HirExpr_EIdent,
    HirExpr_EBinOp,
    HirExpr_EUnaryOp,
    HirExpr_ECall,
    HirExpr_EMethodCall,
    HirExpr_EPropAccess,
    HirExpr_EIndex,
    HirExpr_ECast,
    HirExpr_EFString,
    HirExpr_ETryExpr,
    HirExpr_EClosure,
    HirExpr_ESuperMethodCall,
    HirExpr_ESuperPropAccess,
    HirExpr_EList,
    HirExpr_ESet,
    HirExpr_EDict,
    HirExpr_ETuple,
    HirExpr_EListComp,
    HirExpr_EGeneratorExpr,
    HirExpr_ESlice,
    HirExpr_EAwait,
    HirExpr_EYield,
    HirExpr_ETry,
    HirExpr_ERange,
    HirExpr_ESizeOf,
    HirExpr_EIfElse
} HirExpr_tag;

typedef struct HirExpr {
    HirExpr_tag tag;
    union {
        struct {
            long long val;
            AstType* ty;
        } ELitInt;
        struct {
            double val;
            AstType* ty;
        } ELitFloat;
        struct {
            char* val;
            AstType* ty;
        } ELitStr;
        struct {
            char* val;
            AstType* ty;
        } ELitBytes;
        struct {
            char* val;
            AstType* ty;
        } ERawStr;
        struct {
            long long val;
            AstType* ty;
        } ELitChar;
        struct {
            bool val;
            AstType* ty;
        } ELitBool;
        struct {
            AstType* ty;
        } ELitNone;
        struct {
            char* name;
            AstType* ty;
            bool is_move;
        } EIdent;
        struct {
            char* op;
            HirExpr* left;
            HirExpr* right;
            AstType* ty;
        } EBinOp;
        struct {
            char* op;
            HirExpr* expr;
            AstType* ty;
        } EUnaryOp;
        struct {
            HirExpr* callee;
            List_ptr* args;
            AstType* ty;
        } ECall;
        struct {
            HirExpr* obj;
            char* method;
            List_ptr* args;
            AstType* ty;
        } EMethodCall;
        struct {
            HirExpr* obj;
            char* prop;
            AstType* ty;
        } EPropAccess;
        struct {
            HirExpr* obj;
            HirExpr* index;
            AstType* ty;
        } EIndex;
        struct {
            HirExpr* expr;
            AstType* target_ty;
        } ECast;
        struct {
            List_ptr* parts;
            AstType* ty;
        } EFString;
        struct {
            HirExpr* expr;
            AstType* ty;
        } ETryExpr;
        struct {
            List_ptr* params;
            AstType* ret_ty;
            HirBlock* body;
            bool is_async;
        } EClosure;
        struct {
            char* base_class;
            char* method;
            List_ptr* args;
            AstType* ty;
        } ESuperMethodCall;
        struct {
            char* base_class;
            char* prop;
            AstType* ty;
        } ESuperPropAccess;
        struct {
            List_ptr* items;
            AstType* ty;
        } EList;
        struct {
            List_ptr* items;
            AstType* ty;
        } ESet;
        struct {
            List_ptr* keys;
            List_ptr* vals;
            AstType* ty;
        } EDict;
        struct {
            List_ptr* items;
            AstType* ty;
        } ETuple;
        struct {
            HirExpr* element;
            List_ptr* generators;
            AstType* ty;
        } EListComp;
        struct {
            HirExpr* element;
            List_ptr* generators;
            AstType* ty;
        } EGeneratorExpr;
        struct {
            HirExpr* start;
            HirExpr* stop;
            HirExpr* step;
            AstType* ty;
        } ESlice;
        struct {
            HirExpr* expr;
            AstType* ty;
        } EAwait;
        struct {
            HirExpr* expr;
            AstType* ty;
        } EYield;
        struct {
            HirBlock* try_body;
            List_ptr* catches;
            HirBlock* finally_b;
            AstType* ty;
        } ETry;
        struct {
            HirExpr* start;
            HirExpr* end;
            bool inclusive;
            AstType* ty;
        } ERange;
        struct {
            AstType* target_ty;
            AstType* ty;
        } ESizeOf;
        struct {
            HirExpr* cond;
            HirExpr* then_e;
            HirExpr* else_e;
            AstType* ty;
        } EIfElse;
    } data;
} HirExpr;

static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitInt(long long val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitInt}; _r.data.ELitInt.val = val; _r.data.ELitInt.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitFloat(double val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitFloat}; _r.data.ELitFloat.val = val; _r.data.ELitFloat.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitStr(char* val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitStr}; _r.data.ELitStr.val = val; _r.data.ELitStr.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitBytes(char* val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitBytes}; _r.data.ELitBytes.val = val; _r.data.ELitBytes.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ERawStr(char* val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ERawStr}; _r.data.ERawStr.val = val; _r.data.ERawStr.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitChar(long long val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitChar}; _r.data.ELitChar.val = val; _r.data.ELitChar.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitBool(bool val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitBool}; _r.data.ELitBool.val = val; _r.data.ELitBool.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitNone(AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitNone}; _r.data.ELitNone.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EIdent(char* name, AstType* ty, bool is_move) { HirExpr _r = {.tag=HirExpr_EIdent}; _r.data.EIdent.name = name; _r.data.EIdent.ty = ty; _r.data.EIdent.is_move = is_move; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EBinOp(char* op, HirExpr* left, HirExpr* right, AstType* ty) { HirExpr _r = {.tag=HirExpr_EBinOp}; _r.data.EBinOp.op = op; _r.data.EBinOp.left = left; _r.data.EBinOp.right = right; _r.data.EBinOp.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EUnaryOp(char* op, HirExpr* expr, AstType* ty) { HirExpr _r = {.tag=HirExpr_EUnaryOp}; _r.data.EUnaryOp.op = op; _r.data.EUnaryOp.expr = expr; _r.data.EUnaryOp.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ECall(HirExpr* callee, List_ptr* args, AstType* ty) { HirExpr _r = {.tag=HirExpr_ECall}; _r.data.ECall.callee = callee; _r.data.ECall.args = args; _r.data.ECall.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EMethodCall(HirExpr* obj, char* method, List_ptr* args, AstType* ty) { HirExpr _r = {.tag=HirExpr_EMethodCall}; _r.data.EMethodCall.obj = obj; _r.data.EMethodCall.method = method; _r.data.EMethodCall.args = args; _r.data.EMethodCall.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EPropAccess(HirExpr* obj, char* prop, AstType* ty) { HirExpr _r = {.tag=HirExpr_EPropAccess}; _r.data.EPropAccess.obj = obj; _r.data.EPropAccess.prop = prop; _r.data.EPropAccess.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EIndex(HirExpr* obj, HirExpr* index, AstType* ty) { HirExpr _r = {.tag=HirExpr_EIndex}; _r.data.EIndex.obj = obj; _r.data.EIndex.index = index; _r.data.EIndex.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ECast(HirExpr* expr, AstType* target_ty) { HirExpr _r = {.tag=HirExpr_ECast}; _r.data.ECast.expr = expr; _r.data.ECast.target_ty = target_ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EFString(List_ptr* parts, AstType* ty) { HirExpr _r = {.tag=HirExpr_EFString}; _r.data.EFString.parts = parts; _r.data.EFString.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ETryExpr(HirExpr* expr, AstType* ty) { HirExpr _r = {.tag=HirExpr_ETryExpr}; _r.data.ETryExpr.expr = expr; _r.data.ETryExpr.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EClosure(List_ptr* params, AstType* ret_ty, HirBlock* body, bool is_async) { HirExpr _r = {.tag=HirExpr_EClosure}; _r.data.EClosure.params = params; _r.data.EClosure.ret_ty = ret_ty; _r.data.EClosure.body = body; _r.data.EClosure.is_async = is_async; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESuperMethodCall(char* base_class, char* method, List_ptr* args, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESuperMethodCall}; _r.data.ESuperMethodCall.base_class = base_class; _r.data.ESuperMethodCall.method = method; _r.data.ESuperMethodCall.args = args; _r.data.ESuperMethodCall.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESuperPropAccess(char* base_class, char* prop, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESuperPropAccess}; _r.data.ESuperPropAccess.base_class = base_class; _r.data.ESuperPropAccess.prop = prop; _r.data.ESuperPropAccess.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EList(List_ptr* items, AstType* ty) { HirExpr _r = {.tag=HirExpr_EList}; _r.data.EList.items = items; _r.data.EList.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESet(List_ptr* items, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESet}; _r.data.ESet.items = items; _r.data.ESet.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EDict(List_ptr* keys, List_ptr* vals, AstType* ty) { HirExpr _r = {.tag=HirExpr_EDict}; _r.data.EDict.keys = keys; _r.data.EDict.vals = vals; _r.data.EDict.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ETuple(List_ptr* items, AstType* ty) { HirExpr _r = {.tag=HirExpr_ETuple}; _r.data.ETuple.items = items; _r.data.ETuple.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EListComp(HirExpr* element, List_ptr* generators, AstType* ty) { HirExpr _r = {.tag=HirExpr_EListComp}; _r.data.EListComp.element = element; _r.data.EListComp.generators = generators; _r.data.EListComp.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EGeneratorExpr(HirExpr* element, List_ptr* generators, AstType* ty) { HirExpr _r = {.tag=HirExpr_EGeneratorExpr}; _r.data.EGeneratorExpr.element = element; _r.data.EGeneratorExpr.generators = generators; _r.data.EGeneratorExpr.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESlice(HirExpr* start, HirExpr* stop, HirExpr* step, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESlice}; _r.data.ESlice.start = start; _r.data.ESlice.stop = stop; _r.data.ESlice.step = step; _r.data.ESlice.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EAwait(HirExpr* expr, AstType* ty) { HirExpr _r = {.tag=HirExpr_EAwait}; _r.data.EAwait.expr = expr; _r.data.EAwait.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EYield(HirExpr* expr, AstType* ty) { HirExpr _r = {.tag=HirExpr_EYield}; _r.data.EYield.expr = expr; _r.data.EYield.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ETry(HirBlock* try_body, List_ptr* catches, HirBlock* finally_b, AstType* ty) { HirExpr _r = {.tag=HirExpr_ETry}; _r.data.ETry.try_body = try_body; _r.data.ETry.catches = catches; _r.data.ETry.finally_b = finally_b; _r.data.ETry.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ERange(HirExpr* start, HirExpr* end, bool inclusive, AstType* ty) { HirExpr _r = {.tag=HirExpr_ERange}; _r.data.ERange.start = start; _r.data.ERange.end = end; _r.data.ERange.inclusive = inclusive; _r.data.ERange.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESizeOf(AstType* target_ty, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESizeOf}; _r.data.ESizeOf.target_ty = target_ty; _r.data.ESizeOf.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EIfElse(HirExpr* cond, HirExpr* then_e, HirExpr* else_e, AstType* ty) { HirExpr _r = {.tag=HirExpr_EIfElse}; _r.data.EIfElse.cond = cond; _r.data.EIfElse.then_e = then_e; _r.data.EIfElse.else_e = else_e; _r.data.EIfElse.ty = ty; return _r; }

typedef enum {
    HirStmt_SExpr,
    HirStmt_SLet,
    HirStmt_SAssign,
    HirStmt_SReturn,
    HirStmt_SBreak,
    HirStmt_SContinue,
    HirStmt_SPass,
    HirStmt_SRaise,
    HirStmt_SUnsafe,
    HirStmt_SIf,
    HirStmt_SWhile,
    HirStmt_SFor,
    HirStmt_SMatch,
    HirStmt_STry,
    HirStmt_SAssert,
    HirStmt_SWith,
    HirStmt_SAsm,
    HirStmt_SSpawn,
    HirStmt_STaskGroup,
    HirStmt_SGpuBlock,
    HirStmt_SFree
} HirStmt_tag;

typedef struct HirStmt {
    HirStmt_tag tag;
    union {
        struct {
            HirExpr* expr;
        } SExpr;
        struct {
            char* name;
            Ownership ownership;
            bool is_mut;
            bool is_const;
            bool is_shared;
            AstType* ty;
            HirExpr* val;
        } SLet;
        struct {
            HirExpr* target;
            HirExpr* val;
        } SAssign;
        struct {
            HirExpr* val;
        } SReturn;
        struct {
            HirExpr* val;
        } SRaise;
        struct {
            HirBlock* body;
        } SUnsafe;
        struct {
            HirExpr* cond;
            HirBlock* then_b;
            HirBlock* else_b;
        } SIf;
        struct {
            HirExpr* cond;
            HirBlock* body;
        } SWhile;
        struct {
            char* var;
            HirExpr* iter;
            HirBlock* body;
        } SFor;
        struct {
            HirExpr* expr;
            List_ptr* arms;
        } SMatch;
        struct {
            HirBlock* try_body;
            List_ptr* catches;
            HirBlock* finally_b;
        } STry;
        struct {
            HirExpr* cond;
            HirExpr* msg;
        } SAssert;
        struct {
            List_ptr* items;
            List_str* aliases;
            HirBlock* body;
        } SWith;
        struct {
            char* code;
            char* outputs;
            char* inputs;
            char* clobbers;
        } SAsm;
        struct {
            HirExpr* expr;
        } SSpawn;
        struct {
            HirBlock* body;
        } STaskGroup;
        struct {
            HirBlock* body;
        } SGpuBlock;
        struct {
            char* name;
        } SFree;
    } data;
} HirStmt;

static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SExpr(HirExpr* expr) { HirStmt _r = {.tag=HirStmt_SExpr}; _r.data.SExpr.expr = expr; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SLet(char* name, Ownership ownership, bool is_mut, bool is_const, bool is_shared, AstType* ty, HirExpr* val) { HirStmt _r = {.tag=HirStmt_SLet}; _r.data.SLet.name = name; _r.data.SLet.ownership = ownership; _r.data.SLet.is_mut = is_mut; _r.data.SLet.is_const = is_const; _r.data.SLet.is_shared = is_shared; _r.data.SLet.ty = ty; _r.data.SLet.val = val; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SAssign(HirExpr* target, HirExpr* val) { HirStmt _r = {.tag=HirStmt_SAssign}; _r.data.SAssign.target = target; _r.data.SAssign.val = val; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SReturn(HirExpr* val) { HirStmt _r = {.tag=HirStmt_SReturn}; _r.data.SReturn.val = val; return _r; }
#define HirStmt_make_SBreak() ((HirStmt){.tag=HirStmt_SBreak})
#define HirStmt_make_SContinue() ((HirStmt){.tag=HirStmt_SContinue})
#define HirStmt_make_SPass() ((HirStmt){.tag=HirStmt_SPass})
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SRaise(HirExpr* val) { HirStmt _r = {.tag=HirStmt_SRaise}; _r.data.SRaise.val = val; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SUnsafe(HirBlock* body) { HirStmt _r = {.tag=HirStmt_SUnsafe}; _r.data.SUnsafe.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SIf(HirExpr* cond, HirBlock* then_b, HirBlock* else_b) { HirStmt _r = {.tag=HirStmt_SIf}; _r.data.SIf.cond = cond; _r.data.SIf.then_b = then_b; _r.data.SIf.else_b = else_b; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SWhile(HirExpr* cond, HirBlock* body) { HirStmt _r = {.tag=HirStmt_SWhile}; _r.data.SWhile.cond = cond; _r.data.SWhile.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SFor(char* var, HirExpr* iter, HirBlock* body) { HirStmt _r = {.tag=HirStmt_SFor}; _r.data.SFor.var = var; _r.data.SFor.iter = iter; _r.data.SFor.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SMatch(HirExpr* expr, List_ptr* arms) { HirStmt _r = {.tag=HirStmt_SMatch}; _r.data.SMatch.expr = expr; _r.data.SMatch.arms = arms; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_STry(HirBlock* try_body, List_ptr* catches, HirBlock* finally_b) { HirStmt _r = {.tag=HirStmt_STry}; _r.data.STry.try_body = try_body; _r.data.STry.catches = catches; _r.data.STry.finally_b = finally_b; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SAssert(HirExpr* cond, HirExpr* msg) { HirStmt _r = {.tag=HirStmt_SAssert}; _r.data.SAssert.cond = cond; _r.data.SAssert.msg = msg; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SWith(List_ptr* items, List_str* aliases, HirBlock* body) { HirStmt _r = {.tag=HirStmt_SWith}; _r.data.SWith.items = items; _r.data.SWith.aliases = aliases; _r.data.SWith.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SAsm(char* code, char* outputs, char* inputs, char* clobbers) { HirStmt _r = {.tag=HirStmt_SAsm}; _r.data.SAsm.code = code; _r.data.SAsm.outputs = outputs; _r.data.SAsm.inputs = inputs; _r.data.SAsm.clobbers = clobbers; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SSpawn(HirExpr* expr) { HirStmt _r = {.tag=HirStmt_SSpawn}; _r.data.SSpawn.expr = expr; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_STaskGroup(HirBlock* body) { HirStmt _r = {.tag=HirStmt_STaskGroup}; _r.data.STaskGroup.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SGpuBlock(HirBlock* body) { HirStmt _r = {.tag=HirStmt_SGpuBlock}; _r.data.SGpuBlock.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SFree(char* name) { HirStmt _r = {.tag=HirStmt_SFree}; _r.data.SFree.name = name; return _r; }

typedef enum {
    SymbolKind_SFunction,
    SymbolKind_SClass,
    SymbolKind_SEnum,
    SymbolKind_SVariable,
    SymbolKind_SInterface
} SymbolKind_tag;

typedef struct SymbolKind {
    SymbolKind_tag tag;
} SymbolKind;

#define SymbolKind_make_SFunction() ((SymbolKind){.tag=SymbolKind_SFunction})
#define SymbolKind_make_SClass() ((SymbolKind){.tag=SymbolKind_SClass})
#define SymbolKind_make_SEnum() ((SymbolKind){.tag=SymbolKind_SEnum})
#define SymbolKind_make_SVariable() ((SymbolKind){.tag=SymbolKind_SVariable})
#define SymbolKind_make_SInterface() ((SymbolKind){.tag=SymbolKind_SInterface})

typedef struct Lexer {
    char* src;
    long long len;
    long long pos;
    long long line;
    List_i64* indent_stack;
    long long pending_dedents;
} Lexer;

typedef struct AstType {
    char* name;
    List_ptr* args;
} AstType;

typedef struct GenericConstraint {
    char* target;
    List_ptr* bounds;
} GenericConstraint;

typedef struct Decorator {
    char* name;
    List_ptr* args;
} Decorator;

typedef struct Comprehension {
    char* target;
    Expr* iter;
    List_ptr* ifs;
    bool is_async;
} Comprehension;

typedef struct CatchClause {
    char* err_name;
    AstType** err_type;
    Block** body;
} CatchClause;

typedef struct MatchArm {
    Pattern pat;
    Expr* guard;
    Block** body;
} MatchArm;

typedef struct FStringPart {
    bool is_expr;
    char* text;
    Expr* expr;
} FStringPart;

typedef struct Block {
    List_ptr* stmts;
} Block;

typedef struct ElifClause {
    Expr* cond;
    Block** body;
} ElifClause;

typedef struct Param {
    char* name;
    AstType** ty;
} Param;

typedef struct FunctionDef {
    char* name;
    List_str* generics;
    List_ptr* params;
    AstType** ret_ty;
    AstType** throws_ty;
    List_ptr* decorators;
    List_ptr* constraints;
    bool is_variadic;
    bool is_async;
    bool is_extern;
    bool is_public;
    Block* body;
} FunctionDef;

typedef struct FieldDef {
    char* name;
    AstType** ty;
    Expr* default_val;
} FieldDef;

typedef struct ClassDef {
    char* name;
    List_str* generics;
    List_str* base_classes;
    List_str* iface_names;
    List_ptr* fields;
    List_ptr* methods;
    List_ptr* decorators;
    List_ptr* constraints;
    bool is_public;
} ClassDef;

typedef struct VariantDef {
    char* name;
    List_ptr* fields;
} VariantDef;

typedef struct EnumDef {
    char* name;
    List_str* generics;
    List_str* iface_names;
    List_ptr* variants;
    List_ptr* methods;
    List_ptr* decorators;
    bool is_public;
} EnumDef;

typedef struct InterfaceDef {
    char* name;
    List_str* generics;
    List_ptr* methods;
    bool is_public;
} InterfaceDef;

typedef struct ImportItem {
    char* name;
    char* alias;
} ImportItem;

typedef struct Program {
    List_ptr* decls;
} Program;

typedef struct Parser {
    List_Token* tokens;
    long long pos;
} Parser;

typedef struct ModuleResolver {
    TrMap* visited;
    List_ptr* all_decls;
    List_str* search_paths;
    List_str* mod_dot_paths;
    List_str* mod_file_paths;
    List_str* all_decl_modules;
    char* current_mod;
} ModuleResolver;

typedef struct HirComprehension {
    char* target;
    HirExpr* iter;
    List_ptr* ifs;
    bool is_async;
} HirComprehension;

typedef struct HirCatchClause {
    char* err_name;
    AstType* err_type;
    HirBlock* body;
} HirCatchClause;

typedef struct HirFStringPart {
    bool is_expr;
    char* text;
    HirExpr* expr;
} HirFStringPart;

typedef struct HirMatchArm {
    Pattern pat;
    HirBlock* body;
} HirMatchArm;

typedef struct HirBlock {
    List_ptr* stmts;
} HirBlock;

typedef struct HirParam {
    char* name;
    AstType* ty;
} HirParam;

typedef struct HirFunction {
    char* name;
    char* class_name;
    List_str* generics;
    List_ptr* params;
    AstType* ret_ty;
    AstType* throws_ty;
    HirBlock* body;
    List_ptr* decorators;
    bool is_async;
    bool is_extern;
    bool is_public;
    bool is_static;
    bool is_variadic;
} HirFunction;

typedef struct HirField {
    char* name;
    AstType* ty;
    bool is_public;
} HirField;

typedef struct HirClass {
    char* name;
    List_str* generics;
    List_str* base_classes;
    List_str* iface_names;
    List_ptr* fields;
    List_ptr* methods;
    List_ptr* decorators;
    bool is_public;
} HirClass;

typedef struct HirVariant {
    char* name;
    List_ptr* fields;
} HirVariant;

typedef struct HirEnum {
    char* name;
    List_str* generics;
    List_str* iface_names;
    List_ptr* variants;
    List_ptr* methods;
    List_ptr* decorators;
    bool is_public;
} HirEnum;

typedef struct HirInterface {
    char* name;
    List_str* generics;
    List_ptr* methods;
    bool is_public;
} HirInterface;

typedef struct HirProgram {
    List_ptr* functions;
    List_ptr* classes;
    List_ptr* enums;
    List_ptr* interfaces;
    List_ptr* top_level_stmts;
    List_ptr* extern_funcs;
} HirProgram;

typedef struct Symbol {
    char* name;
    SymbolKind kind;
    AstType** ty;
    long long scope_depth;
    bool is_mut;
    bool is_const;
    bool is_shared;
    bool is_moved;
    long long active_borrows;
    List_str* borrowed_by;
} Symbol;

typedef struct Scope {
    TrMap* variables;
} Scope;

typedef struct Sema {
    TrMap* globals;
    List_ptr* scopes;
    List_str* errors;
    List_str* warnings;
    TrMap* classes;
    TrMap* enums;
    TrMap* interfaces;
    char* current_func_name;
    char* current_class_name;
    long long current_scope_depth;
    bool in_async_fn;
    TrMap* assign_froms;
} Sema;

typedef struct CGenerator {
    StringBuilder* buf;
    StringBuilder* fwd_buf;
    StringBuilder* struct_buf;
    StringBuilder* list_types_buf;
    StringBuilder* proto_buf;
    StringBuilder* mono_buf;
    long long temp_count;
    TrMap* classes;
    TrMap* enums;
    TrMap* interfaces;
    TrMap* functions;
    TrMap* decl_vars;
    TrMap* type_subst;
    TrMap* mono_done;
    TrMap* list_type_done;
    TrMap* list_fwd_done;
    char* cur_class;
    char* cur_func;
    long long closure_count;
    TrMap* emitted_fns;
    TrMap* spawn_wrappers;
    TrMap* shared_vars;
    char* cur_throws_ty;
    long long in_task_group;
    long long in_gpu_block;
} CGenerator;

typedef struct LlvmGenerator {
    StringBuilder* buf;
    long long temp;
    TrMap* classes;
    TrMap* enums;
    TrMap* functions;
} LlvmGenerator;

typedef struct List_Pattern { Pattern* data; size_t len; size_t capacity; } List_Pattern;
static inline List_Pattern* List_Pattern_new(void) { List_Pattern* l=(List_Pattern*)malloc(sizeof(List_Pattern)); l->data=(Pattern*)malloc(sizeof(Pattern)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Pattern_append(List_Pattern* l, Pattern val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Pattern*)realloc(l->data,sizeof(Pattern)*l->capacity); } l->data[l->len++]=val; }
static inline Pattern List_Pattern_get(List_Pattern* l, long long i) { if(!l||(size_t)i>=l->len) return (Pattern){0}; return l->data[i]; }
static inline Pattern List_Pattern_pop(List_Pattern* l) { if(!l||l->len==0) return (Pattern){0}; l->len--; return l->data[l->len]; }
static inline void List_Pattern_free(List_Pattern* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Token { Token* data; size_t len; size_t capacity; } List_Token;
static inline List_Token* List_Token_new(void) { List_Token* l=(List_Token*)malloc(sizeof(List_Token)); l->data=(Token*)malloc(sizeof(Token)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Token_append(List_Token* l, Token val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Token*)realloc(l->data,sizeof(Token)*l->capacity); } l->data[l->len++]=val; }
static inline Token List_Token_get(List_Token* l, long long i) { if(!l||(size_t)i>=l->len) return (Token){0}; return l->data[i]; }
static inline Token List_Token_pop(List_Token* l) { if(!l||l->len==0) return (Token){0}; l->len--; return l->data[l->len]; }
static inline void List_Token_free(List_Token* l) { if(l){ free(l->data); free(l); } }

__attribute__((malloc,returns_nonnull,hot)) Lexer* Lexer_init(char* source);
__attribute__((hot)) long long Lexer_peek(Lexer* self);
__attribute__((hot)) long long Lexer_peek_at(Lexer* self, long long offset);
__attribute__((hot)) long long Lexer_advance(Lexer* self);
__attribute__((hot)) bool Lexer_at_end(Lexer* self);
__attribute__((hot)) void Lexer_skip_spaces(Lexer* self);
__attribute__((hot)) void Lexer_skip_comment(Lexer* self);
__attribute__((hot)) Token Lexer_read_int(Lexer* self);
__attribute__((hot)) Token Lexer_read_string(Lexer* self, long long quote);
__attribute__((hot)) Token Lexer_read_char(Lexer* self);
__attribute__((hot)) Token Lexer_read_fstring(Lexer* self);
__attribute__((hot)) Token Lexer_read_raw_string(Lexer* self);
__attribute__((hot)) Token Lexer_read_byte_string(Lexer* self);
__attribute__((hot)) Token Lexer_read_ident(Lexer* self);
__attribute__((hot)) List_Token* Lexer_tokenize(Lexer* self);
__attribute__((malloc,returns_nonnull,hot)) AstType* AstType_init(char* name);
__attribute__((hot)) AstType* AstType_init_generic(char* name, AstType** arg);
__attribute__((malloc,returns_nonnull,hot)) GenericConstraint* GenericConstraint_init(char* target);
__attribute__((malloc,returns_nonnull,hot)) Decorator* Decorator_init(char* name);
__attribute__((malloc,returns_nonnull,hot)) Comprehension* Comprehension_init(char* target, Expr* iter);
__attribute__((malloc,returns_nonnull,hot)) CatchClause* CatchClause_init(char* err_name, Block** body);
__attribute__((malloc,returns_nonnull,hot)) MatchArm* MatchArm_init(Pattern pat, Block** body);
__attribute__((hot)) FStringPart* FStringPart_init_text(char* s);
__attribute__((hot)) FStringPart* FStringPart_init_expr(Expr* e);
__attribute__((malloc,returns_nonnull,hot)) Block* Block_init();
__attribute__((hot)) void Block_push(Block* self, Stmt* s);
__attribute__((hot)) long long Block_len(Block* self);
__attribute__((hot)) Stmt* Block_get(Block* self, long long i);
__attribute__((malloc,returns_nonnull,hot)) ElifClause* ElifClause_init(Expr* cond, Block** body);
__attribute__((malloc,returns_nonnull,hot)) Param* Param_init(char* name, AstType** ty);
__attribute__((malloc,returns_nonnull,hot)) FunctionDef* FunctionDef_init(char* name);
__attribute__((malloc,returns_nonnull,hot)) FieldDef* FieldDef_init(char* name, AstType** ty);
__attribute__((malloc,returns_nonnull,hot)) ClassDef* ClassDef_init(char* name);
__attribute__((malloc,returns_nonnull,hot)) VariantDef* VariantDef_init(char* name);
__attribute__((malloc,returns_nonnull,hot)) EnumDef* EnumDef_init(char* name);
__attribute__((malloc,returns_nonnull,hot)) InterfaceDef* InterfaceDef_init(char* name);
__attribute__((malloc,returns_nonnull,hot)) ImportItem* ImportItem_init(char* name);
__attribute__((malloc,returns_nonnull,hot)) Program* Program_init();
__attribute__((hot)) void Program_push(Program* self, Decl* d);
__attribute__((hot)) long long Program_len(Program* self);
__attribute__((hot)) Decl* Program_get(Program* self, long long i);
__attribute__((malloc,returns_nonnull,hot)) Parser* Parser_init(List_Token* tokens);
__attribute__((hot)) Token Parser_peek(Parser* self);
__attribute__((hot)) Token Parser_advance(Parser* self);
__attribute__((hot)) void Parser_skip_newlines(Parser* self);
__attribute__((hot)) void Parser_skip_newlines_and_indent(Parser* self);
__attribute__((hot)) void Parser_expect_newline(Parser* self);
__attribute__((hot)) bool Parser_at_end(Parser* self);
__attribute__((hot)) char* Parser_consume_ident(Parser* self);
__attribute__((hot)) AstType* Parser_parse_type(Parser* self);
__attribute__((hot)) List_ptr* Parser_parse_param_list(Parser* self);
__attribute__((hot)) Block* Parser_parse_block(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_try_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_assert_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_asm_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_spawn_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_taskgroup_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_gpu_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_let_stmt(Parser* self, bool is_mut);
__attribute__((hot)) Stmt* Parser_parse_shared_let_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_if_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_while_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_for_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_match_stmt(Parser* self);
__attribute__((hot)) Pattern Parser_parse_pattern(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_assign_or_expr_stmt(Parser* self);
__attribute__((hot)) Expr* Parser_parse_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_ternary(Parser* self);
__attribute__((hot)) Expr* Parser_parse_or_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_and_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_not_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_comparison(Parser* self);
__attribute__((hot)) Expr* Parser_parse_bitor_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_bitxor_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_bitand_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_shift_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_additive(Parser* self);
__attribute__((hot)) Expr* Parser_parse_multiplicative(Parser* self);
__attribute__((hot)) Expr* Parser_parse_power(Parser* self);
__attribute__((hot)) Expr* Parser_parse_unary(Parser* self);
__attribute__((hot)) Expr* Parser_parse_postfix(Parser* self);
__attribute__((hot)) List_ptr* Parser_parse_arg_list(Parser* self);
__attribute__((hot)) Expr* Parser_parse_primary(Parser* self);
__attribute__((hot)) Expr* Parser_parse_fstring(Parser* self, char* raw);
__attribute__((hot)) Program* Parser_parse_program(Parser* self);
__attribute__((hot)) Decl* Parser_parse_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_from_import(Parser* self);
__attribute__((hot)) Decl* Parser_parse_import(Parser* self);
__attribute__((hot)) FunctionDef* Parser_parse_function_def(Parser* self, bool is_method);
__attribute__((hot)) Decl* Parser_parse_class_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_enum_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_interface_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_extend_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_extern_decl(Parser* self);
__attribute__((malloc,returns_nonnull,hot)) ModuleResolver* ModuleResolver_init();
__attribute__((hot)) void ModuleResolver_add_search_path(ModuleResolver* self, char* p);
__attribute__((hot)) Program* ModuleResolver_resolve_main(ModuleResolver* self, char* main_path);
__attribute__((hot)) void ModuleResolver_resolve_file(ModuleResolver* self, char* path, bool is_root);
__attribute__((hot)) void ModuleResolver_resolve_recursive(ModuleResolver* self, char* path);
__attribute__((hot)) void ModuleResolver_resolve_module_path(ModuleResolver* self, char* mod_path);
__attribute__((malloc,returns_nonnull,hot)) HirFStringPart* HirFStringPart_init();
__attribute__((malloc,returns_nonnull,hot)) HirMatchArm* HirMatchArm_init(Pattern pat, HirBlock* body);
__attribute__((malloc,returns_nonnull,hot)) HirBlock* HirBlock_init();
__attribute__((hot)) void HirBlock_push(HirBlock* self, HirStmt* s);
__attribute__((malloc,returns_nonnull,hot)) HirProgram* HirProgram_init();
__attribute__((malloc,returns_nonnull,hot)) Symbol* Symbol_init(char* name, SymbolKind kind, AstType** ty);
__attribute__((malloc,returns_nonnull,hot)) Scope* Scope_init();
__attribute__((malloc,returns_nonnull,hot)) Sema* Sema_init();
__attribute__((hot)) void Sema_error(Sema* self, char* msg);
__attribute__((hot)) void Sema_enter_scope(Sema* self);
__attribute__((hot)) void Sema_exit_scope(Sema* self);
__attribute__((hot)) void Sema_declare(Sema* self, char* name, SymbolKind kind, AstType** ty, bool is_mut);
__attribute__((hot)) Symbol* Sema_resolve(Sema* self, char* name);
__attribute__((hot)) HirProgram* Sema_analyze(Sema* self, Program* prog);
__attribute__((hot)) void Sema_register_decl(Sema* self, Decl* d);
__attribute__((hot)) HirFunction* Sema_lower_func(Sema* self, FunctionDef* f);
__attribute__((hot)) HirClass* Sema_lower_class(Sema* self, ClassDef* c);
__attribute__((hot)) HirEnum* Sema_lower_enum(Sema* self, EnumDef* e);
__attribute__((hot)) HirInterface* Sema_lower_interface(Sema* self, InterfaceDef* i_def);
__attribute__((hot)) HirBlock* Sema_lower_block(Sema* self, Block* b);
__attribute__((hot)) HirStmt* Sema_lower_stmt(Sema* self, Stmt* s_ptr);
__attribute__((hot)) AstType* Sema_variant_field_ty(Sema* self, char* type_name, char* variant_name, long long field_idx);
__attribute__((hot)) void Sema_declare_pattern_binds(Sema* self, Pattern pat);
__attribute__((hot)) HirExpr* Sema_lower_expr(Sema* self, Expr* e_ptr);
__attribute__((hot)) char* Sema_is_reserved_error(Sema* self, char* name);
__attribute__((hot)) char* Sema_is_reserved_warn(Sema* self, char* name);
__attribute__((hot)) bool Sema_block_returns(Sema* self, Block* b);
__attribute__((hot)) bool Sema_is_primitive(Sema* self, AstType* ty);
__attribute__((hot)) bool Sema_is_primitive_name(Sema* self, char* name);
__attribute__((malloc,returns_nonnull,hot)) CGenerator* CGenerator_init();
__attribute__((hot)) char* CGenerator_next_temp(CGenerator* self);
__attribute__((hot)) void CGenerator_seed_params(CGenerator* self, HirFunction* f);
__attribute__((hot)) void CGenerator_w(CGenerator* self, char* s);
__attribute__((hot)) void CGenerator_wf(CGenerator* self, char* s);
__attribute__((hot)) void CGenerator_ws(CGenerator* self, char* s);
__attribute__((hot)) void CGenerator_wp(CGenerator* self, char* s);
__attribute__((hot)) void CGenerator_wlt(CGenerator* self, char* s);
__attribute__((hot)) void CGenerator_ensure_list_type(CGenerator* self, char* n);
__attribute__((hot)) void CGenerator_check_and_emit_list_fwd(CGenerator* self, AstType* ty);
__attribute__((hot)) void CGenerator_emit_list_fwd_decls(CGenerator* self, HirProgram* prog);
__attribute__((hot)) char* CGenerator_type_to_c(CGenerator* self, AstType* ty);
__attribute__((hot)) char* CGenerator_type_suffix(CGenerator* self, char* n);
__attribute__((hot)) char* CGenerator_list_elem_suffix(CGenerator* self, char* n);
__attribute__((hot)) char* CGenerator_type_args_suffix(CGenerator* self, List_ptr* args);
__attribute__((hot)) void CGenerator_ensure_mono(CGenerator* self, HirClass* cls, List_ptr* type_args);
__attribute__((hot)) char* CGenerator_get_inline_attrs(CGenerator* self, HirFunction* f);
__attribute__((hot)) char* CGenerator_get_proto_attrs(CGenerator* self, HirFunction* f);
__attribute__((hot)) char* CGenerator_gen_func_sig(CGenerator* self, HirFunction* f, char* class_name);
__attribute__((hot)) void CGenerator_gen_class_struct(CGenerator* self, HirClass* c);
__attribute__((hot)) void CGenerator_gen_enum_struct(CGenerator* self, HirEnum* e);
__attribute__((hot)) void CGenerator_gen_interface_vtable(CGenerator* self, HirInterface* iface);
__attribute__((hot)) char* CGenerator_gen_one_iface_wrap(CGenerator* self, char* cls_name, HirInterface* iface);
__attribute__((hot)) char* CGenerator_gen_expr(CGenerator* self, HirExpr* e_ptr);
__attribute__((hot)) char* CGenerator_gen_binop(CGenerator* self, char* op, HirExpr* l, HirExpr* r);
__attribute__((hot)) char* CGenerator_gen_unary(CGenerator* self, char* op, HirExpr* expr);
__attribute__((hot)) char* CGenerator_gen_prop_access(CGenerator* self, HirExpr* o, char* p);
__attribute__((hot)) char* CGenerator_gen_index(CGenerator* self, HirExpr* o, HirExpr* idx);
__attribute__((hot)) char* CGenerator_gen_call(CGenerator* self, HirExpr* callee, List_ptr* args, AstType* call_ty);
__attribute__((hot)) char* CGenerator_gen_print_call(CGenerator* self, List_ptr* args);
__attribute__((hot)) char* CGenerator_gen_args(CGenerator* self, List_ptr* args);
__attribute__((hot)) char* CGenerator_gen_method_call(CGenerator* self, HirExpr* obj, char* method, List_ptr* args, AstType* call_ty);
__attribute__((hot)) char* CGenerator_gen_fstring(CGenerator* self, List_ptr* parts);
__attribute__((hot)) char* CGenerator_gen_tuple(CGenerator* self, List_ptr* items);
__attribute__((hot)) char* CGenerator_gen_list_literal(CGenerator* self, List_ptr* items, AstType* ty);
__attribute__((hot)) char* CGenerator_gen_dict_literal(CGenerator* self, List_ptr* keys, List_ptr* vals);
__attribute__((hot)) char* CGenerator_gen_list_comp(CGenerator* self, HirExpr* element, List_ptr* generators);
__attribute__((hot)) char* CGenerator_gen_closure(CGenerator* self, List_ptr* params, AstType* ret_ty, HirBlock* body);
__attribute__((hot)) void CGenerator_emit_spawn_wrapper_for_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator_prescan_block_spawns(CGenerator* self, HirBlock* block);
__attribute__((hot)) void CGenerator_prescan_stmt_spawns(CGenerator* self, HirStmt* s);
__attribute__((hot)) void CGenerator_prescan_spawns(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_gen_stmt(CGenerator* self, HirStmt* s_ptr, long long indent);
__attribute__((hot)) void CGenerator_gen_for_loop(CGenerator* self, char* var, HirExpr* iter, HirBlock* body, long long indent);
__attribute__((hot)) void CGenerator_gen_try(CGenerator* self, HirBlock* try_body, List_ptr* catches, HirBlock* finally_b, long long indent);
__attribute__((hot)) void CGenerator_gen_block(CGenerator* self, HirBlock* b, long long indent);
__attribute__((hot)) void CGenerator_gen_match(CGenerator* self, HirExpr* expr, List_ptr* arms, long long indent);
__attribute__((hot)) void CGenerator_register_program(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_scan_mono_ty(CGenerator* self, AstType* ty);
__attribute__((hot)) void CGenerator_scan_mono_block(CGenerator* self, HirBlock* block);
__attribute__((hot)) void CGenerator_scan_mono_stmt(CGenerator* self, HirStmt* s_ptr);
__attribute__((hot)) void CGenerator_scan_mono_func(CGenerator* self, HirFunction* f);
__attribute__((hot)) void CGenerator_scan_mono_prog(CGenerator* self, HirProgram* prog);
__attribute__((hot)) char* CGenerator_generate(CGenerator* self, HirProgram* prog);
__attribute__((hot)) char* CGenerator_generate_types_header(CGenerator* self, HirProgram* prog);
__attribute__((hot)) char* CGenerator_generate_module_c(CGenerator* self, HirProgram* prog, TrMap* class_set, TrMap* fn_set, long long depth);
__attribute__((hot)) char* CGenerator_generate_main_c(CGenerator* self, HirProgram* prog, TrMap* class_set, TrMap* fn_set);
__attribute__((malloc,returns_nonnull,hot)) LlvmGenerator* LlvmGenerator_init();
__attribute__((hot)) void LlvmGenerator_w(LlvmGenerator* self, char* s);
__attribute__((hot)) char* LlvmGenerator_next_reg(LlvmGenerator* self);
__attribute__((hot)) void LlvmGenerator__tr_fn_register(LlvmGenerator* self, HirProgram* prog);
__attribute__((hot)) void LlvmGenerator_emit_type_decls(LlvmGenerator* self, HirProgram* prog);
__attribute__((hot)) void LlvmGenerator_emit_func_sig(LlvmGenerator* self, HirFunction* f, char* class_name);
__attribute__((hot)) char* LlvmGenerator_gen_expr(LlvmGenerator* self, HirExpr* e_ptr);
__attribute__((hot)) char* LlvmGenerator_gen_binop_llvm(LlvmGenerator* self, char* op, HirExpr* left, HirExpr* right, AstType* ty);
__attribute__((hot)) char* LlvmGenerator_gen_call_llvm(LlvmGenerator* self, HirExpr* callee, List_ptr* args);
__attribute__((hot)) void LlvmGenerator_gen_stmt(LlvmGenerator* self, HirStmt* s_ptr);
__attribute__((hot)) void LlvmGenerator_gen_block(LlvmGenerator* self, HirBlock* b);
__attribute__((hot)) char* LlvmGenerator_generate(LlvmGenerator* self, HirProgram* prog);
__attribute__((hot)) Token keyword_to_token(char* s);
__attribute__((hot)) bool char_is_digit(long long c);
__attribute__((hot)) bool char_is_alpha(long long c);
__attribute__((hot)) bool char_is_alnum(long long c);
__attribute__((hot)) bool char_is_space(long long c);
__attribute__((hot)) bool char_is_newline(long long c);
__attribute__((hot)) bool char_is_hex(long long c);
__attribute__((hot)) void _dummy_instantiations();
__attribute__((hot)) Expr* box_expr(Expr e);
__attribute__((hot)) Stmt* box_stmt(Stmt s);
__attribute__((hot)) Decl* box_decl(Decl d);
__attribute__((hot)) AstType** box_asttype(AstType* t);
__attribute__((hot)) bool decl_is_pub(Decl d);
__attribute__((hot)) HirExpr* box_hirexpr(HirExpr e);
__attribute__((hot)) HirStmt* box_hirstmt(HirStmt s);
__attribute__((hot)) AstType* hir_expr_type(HirExpr* e);
__attribute__((hot)) Ownership hir_stmt_ownership(HirStmt* s);
__attribute__((hot)) long long _tr_str_len(char* s);
__attribute__((hot)) Symbol** box_symbol(Symbol* s);
__attribute__((hot)) char* _indent_str(long long n);
__attribute__((hot)) bool _is_str_type(char* n);
__attribute__((hot)) bool _is_int_type(char* n);
__attribute__((hot)) bool _is_float_type(char* n);
__attribute__((hot)) bool _is_c_keyword(char* n);
__attribute__((hot)) bool _is_primitive(char* n);
__attribute__((hot)) char* _escape_str_for_c(char* s);
__attribute__((hot)) char* llvm_type(AstType* ty);
__attribute__((hot)) void print_usage();
__attribute__((hot)) bool str_ends_with_dot_tr(char* path);
__attribute__((hot)) char* strip_extension(char* path);
__attribute__((hot)) bool str_starts_with(char* s, char* prefix);
__attribute__((hot)) char* detect_c_compiler();
__attribute__((hot)) char* dir_of_path(char* path);
__attribute__((hot)) char* read_runtime_header(char* bin_path);
__attribute__((hot)) void ensure_runtime_header(char* out_dir, char* bin_path);
__attribute__((hot)) void sync_headers_to_runtime(char* rt_content, char* types_content);
__attribute__((hot)) char* strip_trailing_sep(char* s);
__attribute__((hot)) char* path_to_native(char* s);
__attribute__((hot)) char* dot_to_safe(char* s);
__attribute__((hot)) char* dot_last_seg(char* s);
__attribute__((hot)) char* get_filename(char* path);
__attribute__((hot)) bool is_builtin_mod(char* dot_path);
__attribute__((hot)) void make_dir(char* path);
__attribute__((hot)) long long compile_all_c(List_str* c_files, char* exe_path, char* inc_dir, List_str* link_paths, List_str* lib_flags, char* opt_level, bool verbose);
__attribute__((hot)) long long compile_c_to_exe(char* c_path, char* exe_path, char* opt_level, bool verbose);

