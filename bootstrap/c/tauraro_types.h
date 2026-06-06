#pragma once
#define TAURARO_STD_LIB
#define TAURARO_RT_NO_STRINGBUILDER
#include "tauraro_rt.h"

typedef struct StringObj StringObj;
typedef struct StringBuilder StringBuilder;
typedef struct Lexer Lexer;
typedef struct AstType AstType;
typedef struct GenericConstraint GenericConstraint;
typedef struct Decorator Decorator;
typedef struct Comprehension Comprehension;
typedef struct CatchClause CatchClause;
typedef struct MatchArm MatchArm;
typedef struct FStringPart FStringPart;
typedef struct ChanSelectArm ChanSelectArm;
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
typedef struct HirChanSelectArm HirChanSelectArm;
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
    Token_TripleStrLit,
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
    Token_KwDefer,
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
        } TripleStrLit;
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
static inline __attribute__((always_inline)) Token Token_ctor_TripleStrLit(char* val) { Token _r = {.tag=Token_TripleStrLit}; _r.data.TripleStrLit.val = val; return _r; }
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
#define Token_make_KwDefer() ((Token){.tag=Token_KwDefer})
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
static inline __attribute__((always_inline)) Pattern Pattern_ctor_POr(List_Pattern* patterns) { Pattern _r = {.tag=Pattern_POr}; _r.data.POr.patterns = patterns; return _r; }

typedef enum {
    Ownership_Own,
    Ownership_Shared
} Ownership_tag;

typedef struct Ownership {
    Ownership_tag tag;
} Ownership;

#define Ownership_make_Own() ((Ownership){.tag=Ownership_Own})
#define Ownership_make_Shared() ((Ownership){.tag=Ownership_Shared})

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
    Stmt_SMultiLet,
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
    Stmt_SForUnpack,
    Stmt_SMatch,
    Stmt_STry,
    Stmt_SAssert,
    Stmt_SWith,
    Stmt_SAsm,
    Stmt_SSpawn,
    Stmt_STaskGroup,
    Stmt_SGpuBlock,
    Stmt_SChanSelect,
    Stmt_SDefer,
    Stmt_SLine
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
            List_str* names;
            bool is_mut;
            Expr* val;
        } SMultiLet;
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
            List_str* vars;
            Expr* iter;
            Block* body;
        } SForUnpack;
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
        struct {
            List_ptr* cases;
        } SChanSelect;
        struct {
            Stmt* stmt;
        } SDefer;
        struct {
            long long n;
        } SLine;
    } data;
} Stmt;

static inline __attribute__((always_inline)) Stmt Stmt_ctor_SExpr(Expr* expr) { Stmt _r = {.tag=Stmt_SExpr}; _r.data.SExpr.expr = expr; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SLet(char* name, Ownership ownership, bool is_mut, bool is_const, bool is_shared, AstType** ty, Expr* val) { Stmt _r = {.tag=Stmt_SLet}; _r.data.SLet.name = name; _r.data.SLet.ownership = ownership; _r.data.SLet.is_mut = is_mut; _r.data.SLet.is_const = is_const; _r.data.SLet.is_shared = is_shared; _r.data.SLet.ty = ty; _r.data.SLet.val = val; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SMultiLet(List_str* names, bool is_mut, Expr* val) { Stmt _r = {.tag=Stmt_SMultiLet}; _r.data.SMultiLet.names = names; _r.data.SMultiLet.is_mut = is_mut; _r.data.SMultiLet.val = val; return _r; }
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
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SForUnpack(List_str* vars, Expr* iter, Block* body) { Stmt _r = {.tag=Stmt_SForUnpack}; _r.data.SForUnpack.vars = vars; _r.data.SForUnpack.iter = iter; _r.data.SForUnpack.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SMatch(Expr* expr, List_ptr* arms) { Stmt _r = {.tag=Stmt_SMatch}; _r.data.SMatch.expr = expr; _r.data.SMatch.arms = arms; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_STry(Block* try_body, List_ptr* catches, Block* finally_b) { Stmt _r = {.tag=Stmt_STry}; _r.data.STry.try_body = try_body; _r.data.STry.catches = catches; _r.data.STry.finally_b = finally_b; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SAssert(Expr* cond, Expr* msg) { Stmt _r = {.tag=Stmt_SAssert}; _r.data.SAssert.cond = cond; _r.data.SAssert.msg = msg; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SWith(List_ptr* items, List_str* aliases, Block* body) { Stmt _r = {.tag=Stmt_SWith}; _r.data.SWith.items = items; _r.data.SWith.aliases = aliases; _r.data.SWith.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SAsm(char* code, char* outputs, char* inputs, char* clobbers) { Stmt _r = {.tag=Stmt_SAsm}; _r.data.SAsm.code = code; _r.data.SAsm.outputs = outputs; _r.data.SAsm.inputs = inputs; _r.data.SAsm.clobbers = clobbers; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SSpawn(Expr* expr) { Stmt _r = {.tag=Stmt_SSpawn}; _r.data.SSpawn.expr = expr; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_STaskGroup(Block* body) { Stmt _r = {.tag=Stmt_STaskGroup}; _r.data.STaskGroup.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SGpuBlock(Block* body) { Stmt _r = {.tag=Stmt_SGpuBlock}; _r.data.SGpuBlock.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SChanSelect(List_ptr* cases) { Stmt _r = {.tag=Stmt_SChanSelect}; _r.data.SChanSelect.cases = cases; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SDefer(Stmt* stmt) { Stmt _r = {.tag=Stmt_SDefer}; _r.data.SDefer.stmt = stmt; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SLine(long long n) { Stmt _r = {.tag=Stmt_SLine}; _r.data.SLine.n = n; return _r; }

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
    Decl_DDecoratorDef,
    Decl_DTypeAlias
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
        struct {
            char* name;
            AstType** target;
        } DTypeAlias;
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
static inline __attribute__((always_inline)) Decl Decl_ctor_DTypeAlias(char* name, AstType** target) { Decl _r = {.tag=Decl_DTypeAlias}; _r.data.DTypeAlias.name = name; _r.data.DTypeAlias.target = target; return _r; }

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
    HirExpr_EAwaitTimeout,
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
            List_ptr* captures;
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
            HirExpr* timeout_ms;
            AstType* ty;
        } EAwaitTimeout;
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
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EClosure(List_ptr* params, AstType* ret_ty, HirBlock* body, bool is_async, List_ptr* captures) { HirExpr _r = {.tag=HirExpr_EClosure}; _r.data.EClosure.params = params; _r.data.EClosure.ret_ty = ret_ty; _r.data.EClosure.body = body; _r.data.EClosure.is_async = is_async; _r.data.EClosure.captures = captures; return _r; }
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
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EAwaitTimeout(HirExpr* expr, HirExpr* timeout_ms, AstType* ty) { HirExpr _r = {.tag=HirExpr_EAwaitTimeout}; _r.data.EAwaitTimeout.expr = expr; _r.data.EAwaitTimeout.timeout_ms = timeout_ms; _r.data.EAwaitTimeout.ty = ty; return _r; }
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
    HirStmt_SForUnpack,
    HirStmt_SMatch,
    HirStmt_STry,
    HirStmt_SAssert,
    HirStmt_SWith,
    HirStmt_SAsm,
    HirStmt_SSpawn,
    HirStmt_STaskGroup,
    HirStmt_SGpuBlock,
    HirStmt_SFree,
    HirStmt_SMultiLet,
    HirStmt_SChanSelect,
    HirStmt_SDefer
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
            List_str* vars;
            HirExpr* iter;
            HirBlock* body;
        } SForUnpack;
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
        struct {
            List_str* names;
            bool is_mut;
            HirExpr* val;
        } SMultiLet;
        struct {
            List_ptr* cases;
        } SChanSelect;
        struct {
            HirStmt* stmt;
        } SDefer;
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
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SForUnpack(List_str* vars, HirExpr* iter, HirBlock* body) { HirStmt _r = {.tag=HirStmt_SForUnpack}; _r.data.SForUnpack.vars = vars; _r.data.SForUnpack.iter = iter; _r.data.SForUnpack.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SMatch(HirExpr* expr, List_ptr* arms) { HirStmt _r = {.tag=HirStmt_SMatch}; _r.data.SMatch.expr = expr; _r.data.SMatch.arms = arms; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_STry(HirBlock* try_body, List_ptr* catches, HirBlock* finally_b) { HirStmt _r = {.tag=HirStmt_STry}; _r.data.STry.try_body = try_body; _r.data.STry.catches = catches; _r.data.STry.finally_b = finally_b; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SAssert(HirExpr* cond, HirExpr* msg) { HirStmt _r = {.tag=HirStmt_SAssert}; _r.data.SAssert.cond = cond; _r.data.SAssert.msg = msg; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SWith(List_ptr* items, List_str* aliases, HirBlock* body) { HirStmt _r = {.tag=HirStmt_SWith}; _r.data.SWith.items = items; _r.data.SWith.aliases = aliases; _r.data.SWith.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SAsm(char* code, char* outputs, char* inputs, char* clobbers) { HirStmt _r = {.tag=HirStmt_SAsm}; _r.data.SAsm.code = code; _r.data.SAsm.outputs = outputs; _r.data.SAsm.inputs = inputs; _r.data.SAsm.clobbers = clobbers; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SSpawn(HirExpr* expr) { HirStmt _r = {.tag=HirStmt_SSpawn}; _r.data.SSpawn.expr = expr; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_STaskGroup(HirBlock* body) { HirStmt _r = {.tag=HirStmt_STaskGroup}; _r.data.STaskGroup.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SGpuBlock(HirBlock* body) { HirStmt _r = {.tag=HirStmt_SGpuBlock}; _r.data.SGpuBlock.body = body; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SFree(char* name) { HirStmt _r = {.tag=HirStmt_SFree}; _r.data.SFree.name = name; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SMultiLet(List_str* names, bool is_mut, HirExpr* val) { HirStmt _r = {.tag=HirStmt_SMultiLet}; _r.data.SMultiLet.names = names; _r.data.SMultiLet.is_mut = is_mut; _r.data.SMultiLet.val = val; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SChanSelect(List_ptr* cases) { HirStmt _r = {.tag=HirStmt_SChanSelect}; _r.data.SChanSelect.cases = cases; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SDefer(HirStmt* stmt) { HirStmt _r = {.tag=HirStmt_SDefer}; _r.data.SDefer.stmt = stmt; return _r; }

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

#ifndef StringObj_STRUCT_DEFINED
#define StringObj_STRUCT_DEFINED
typedef struct StringObj {
    char* data;
    long long len;
    long long capacity;
} StringObj;
#endif

#ifndef StringBuilder_STRUCT_DEFINED
#define StringBuilder_STRUCT_DEFINED
typedef struct StringBuilder {
    StringObj* buf;
} StringBuilder;
#endif

#ifndef Lexer_STRUCT_DEFINED
#define Lexer_STRUCT_DEFINED
typedef struct Lexer {
    char* src;
    long long len;
    long long pos;
    long long line;
    List_i64* indent_stack;
    long long pending_dedents;
    List_i64* token_lines;
} Lexer;
#endif

#ifndef AstType_STRUCT_DEFINED
#define AstType_STRUCT_DEFINED
typedef struct AstType {
    char* name;
    List_ptr* args;
    char* from_param;
} AstType;
#endif

#ifndef GenericConstraint_STRUCT_DEFINED
#define GenericConstraint_STRUCT_DEFINED
typedef struct GenericConstraint {
    char* target;
    List_ptr* bounds;
} GenericConstraint;
#endif

#ifndef Decorator_STRUCT_DEFINED
#define Decorator_STRUCT_DEFINED
typedef struct Decorator {
    char* name;
    List_ptr* args;
} Decorator;
#endif

#ifndef Comprehension_STRUCT_DEFINED
#define Comprehension_STRUCT_DEFINED
typedef struct Comprehension {
    char* target;
    Expr* iter;
    List_ptr* ifs;
    bool is_async;
} Comprehension;
#endif

#ifndef CatchClause_STRUCT_DEFINED
#define CatchClause_STRUCT_DEFINED
typedef struct CatchClause {
    char* err_name;
    AstType** err_type;
    Block** body;
} CatchClause;
#endif

#ifndef MatchArm_STRUCT_DEFINED
#define MatchArm_STRUCT_DEFINED
typedef struct MatchArm {
    Pattern pat;
    Expr* guard;
    Block** body;
} MatchArm;
#endif

#ifndef FStringPart_STRUCT_DEFINED
#define FStringPart_STRUCT_DEFINED
typedef struct FStringPart {
    bool is_expr;
    char* text;
    Expr* expr;
    char* fmt_spec;
} FStringPart;
#endif

#ifndef ChanSelectArm_STRUCT_DEFINED
#define ChanSelectArm_STRUCT_DEFINED
typedef struct ChanSelectArm {
    long long kind;
    Expr* chan_expr;
    Expr* val_expr;
    char* var_name;
    Expr* timeout_ms;
    Block* body;
} ChanSelectArm;
#endif

#ifndef Block_STRUCT_DEFINED
#define Block_STRUCT_DEFINED
typedef struct Block {
    List_ptr* stmts;
} Block;
#endif

#ifndef ElifClause_STRUCT_DEFINED
#define ElifClause_STRUCT_DEFINED
typedef struct ElifClause {
    Expr* cond;
    Block** body;
} ElifClause;
#endif

#ifndef Param_STRUCT_DEFINED
#define Param_STRUCT_DEFINED
typedef struct Param {
    char* name;
    AstType** ty;
    bool is_ref;
    bool is_mut_ref;
} Param;
#endif

#ifndef FunctionDef_STRUCT_DEFINED
#define FunctionDef_STRUCT_DEFINED
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
    long long line;
} FunctionDef;
#endif

#ifndef FieldDef_STRUCT_DEFINED
#define FieldDef_STRUCT_DEFINED
typedef struct FieldDef {
    char* name;
    AstType** ty;
    Expr* default_val;
} FieldDef;
#endif

#ifndef ClassDef_STRUCT_DEFINED
#define ClassDef_STRUCT_DEFINED
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
    bool is_class;
    long long line;
} ClassDef;
#endif

#ifndef VariantDef_STRUCT_DEFINED
#define VariantDef_STRUCT_DEFINED
typedef struct VariantDef {
    char* name;
    List_ptr* fields;
} VariantDef;
#endif

#ifndef EnumDef_STRUCT_DEFINED
#define EnumDef_STRUCT_DEFINED
typedef struct EnumDef {
    char* name;
    List_str* generics;
    List_str* iface_names;
    List_ptr* variants;
    List_ptr* methods;
    List_ptr* decorators;
    bool is_public;
    long long line;
} EnumDef;
#endif

#ifndef InterfaceDef_STRUCT_DEFINED
#define InterfaceDef_STRUCT_DEFINED
typedef struct InterfaceDef {
    char* name;
    List_str* generics;
    List_ptr* methods;
    bool is_public;
    long long line;
    List_ptr* decorators;
} InterfaceDef;
#endif

#ifndef ImportItem_STRUCT_DEFINED
#define ImportItem_STRUCT_DEFINED
typedef struct ImportItem {
    char* name;
    char* alias;
} ImportItem;
#endif

#ifndef Program_STRUCT_DEFINED
#define Program_STRUCT_DEFINED
typedef struct Program {
    List_ptr* decls;
} Program;
#endif

#ifndef Parser_STRUCT_DEFINED
#define Parser_STRUCT_DEFINED
typedef struct Parser {
    List_Token* tokens;
    long long pos;
    List_i64* lines;
} Parser;
#endif

#ifndef ModuleResolver_STRUCT_DEFINED
#define ModuleResolver_STRUCT_DEFINED
typedef struct ModuleResolver {
    TrMap* visited;
    List_ptr* all_decls;
    List_str* search_paths;
    List_str* mod_dot_paths;
    List_str* mod_file_paths;
    List_str* all_decl_modules;
    char* current_mod;
} ModuleResolver;
#endif

#ifndef HirComprehension_STRUCT_DEFINED
#define HirComprehension_STRUCT_DEFINED
typedef struct HirComprehension {
    char* target;
    HirExpr* iter;
    List_ptr* ifs;
    bool is_async;
} HirComprehension;
#endif

#ifndef HirCatchClause_STRUCT_DEFINED
#define HirCatchClause_STRUCT_DEFINED
typedef struct HirCatchClause {
    char* err_name;
    AstType* err_type;
    HirBlock* body;
} HirCatchClause;
#endif

#ifndef HirFStringPart_STRUCT_DEFINED
#define HirFStringPart_STRUCT_DEFINED
typedef struct HirFStringPart {
    bool is_expr;
    char* text;
    HirExpr* expr;
    char* fmt_spec;
} HirFStringPart;
#endif

#ifndef HirMatchArm_STRUCT_DEFINED
#define HirMatchArm_STRUCT_DEFINED
typedef struct HirMatchArm {
    Pattern pat;
    HirBlock* body;
    HirExpr* guard;
} HirMatchArm;
#endif

#ifndef HirChanSelectArm_STRUCT_DEFINED
#define HirChanSelectArm_STRUCT_DEFINED
typedef struct HirChanSelectArm {
    long long kind;
    HirExpr* chan_expr;
    HirExpr* val_expr;
    char* var_name;
    HirExpr* timeout_ms;
    HirBlock* body;
} HirChanSelectArm;
#endif

#ifndef HirBlock_STRUCT_DEFINED
#define HirBlock_STRUCT_DEFINED
typedef struct HirBlock {
    List_ptr* stmts;
} HirBlock;
#endif

#ifndef HirParam_STRUCT_DEFINED
#define HirParam_STRUCT_DEFINED
typedef struct HirParam {
    char* name;
    AstType* ty;
} HirParam;
#endif

#ifndef HirFunction_STRUCT_DEFINED
#define HirFunction_STRUCT_DEFINED
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
    bool is_decorator;
} HirFunction;
#endif

#ifndef HirField_STRUCT_DEFINED
#define HirField_STRUCT_DEFINED
typedef struct HirField {
    char* name;
    AstType* ty;
    bool is_public;
} HirField;
#endif

#ifndef HirClass_STRUCT_DEFINED
#define HirClass_STRUCT_DEFINED
typedef struct HirClass {
    char* name;
    List_str* generics;
    List_str* base_classes;
    List_str* iface_names;
    List_ptr* fields;
    List_ptr* methods;
    List_ptr* decorators;
    bool is_public;
    bool is_class;
} HirClass;
#endif

#ifndef HirVariant_STRUCT_DEFINED
#define HirVariant_STRUCT_DEFINED
typedef struct HirVariant {
    char* name;
    List_ptr* fields;
} HirVariant;
#endif

#ifndef HirEnum_STRUCT_DEFINED
#define HirEnum_STRUCT_DEFINED
typedef struct HirEnum {
    char* name;
    List_str* generics;
    List_str* iface_names;
    List_ptr* variants;
    List_ptr* methods;
    List_ptr* decorators;
    bool is_public;
} HirEnum;
#endif

#ifndef HirInterface_STRUCT_DEFINED
#define HirInterface_STRUCT_DEFINED
typedef struct HirInterface {
    char* name;
    List_str* generics;
    List_ptr* methods;
    bool is_public;
} HirInterface;
#endif

#ifndef HirProgram_STRUCT_DEFINED
#define HirProgram_STRUCT_DEFINED
typedef struct HirProgram {
    List_ptr* functions;
    List_ptr* classes;
    List_ptr* enums;
    List_ptr* interfaces;
    List_ptr* top_level_stmts;
    List_ptr* extern_funcs;
    List_ptr* decorator_defs;
    List_str* type_alias_names;
    List_ptr* type_alias_types;
} HirProgram;
#endif

#ifndef Symbol_STRUCT_DEFINED
#define Symbol_STRUCT_DEFINED
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
    bool is_init;
    bool is_param;
    bool is_maybe_moved;
    bool is_maybe_init;
    long long ptr_region;
    bool is_freed;
} Symbol;
#endif

#ifndef Scope_STRUCT_DEFINED
#define Scope_STRUCT_DEFINED
typedef struct Scope {
    TrMap* variables;
} Scope;
#endif

#ifndef Sema_STRUCT_DEFINED
#define Sema_STRUCT_DEFINED
typedef struct Sema {
    TrMap* globals;
    List_ptr* scopes;
    List_str* errors;
    List_str* warnings;
    TrMap* classes;
    TrMap* enums;
    TrMap* interfaces;
    TrMap* type_aliases;
    TrMap* type_alias_elem;
    char* current_file;
    char* current_func_name;
    char* current_class_name;
    long long current_scope_depth;
    bool in_async_fn;
    TrMap* assign_froms;
    long long current_line;
    List_str* current_func_generics;
    long long closure_boundary;
    List_ptr* closure_caps;
    TrMap* closure_cap_set;
    bool in_assign_target;
    TrMap* container_borrows;
    bool capturing_moves;
    List_str* branch_moved_buf;
    bool capturing_inits;
    List_str* branch_init_buf;
    TrMap* copy_classes;
    bool in_unsafe;
    char* current_func_ret_from;
    bool strict_mode;
    TrMap* decorator_names;
} Sema;
#endif

#ifndef CGenerator_STRUCT_DEFINED
#define CGenerator_STRUCT_DEFINED
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
    TrMap* async_wrappers;
    TrMap* prescanned_fns;
    TrMap* shared_vars;
    char* cur_throws_ty;
    long long in_task_group;
    long long in_gpu_block;
    TrMap* value_types;
    TrMap* closure_cap_set;
    char* closure_env_var;
    TrMap* decorator_defs;
    TrMap* overloaded_sigs;
    TrMap* type_alias_map;
    List_str* defer_stack;
} CGenerator;
#endif

#ifndef LlvmGenerator_STRUCT_DEFINED
#define LlvmGenerator_STRUCT_DEFINED
typedef struct LlvmGenerator {
    StringBuilder* buf;
    long long temp;
    TrMap* classes;
    TrMap* enums;
    TrMap* functions;
} LlvmGenerator;
#endif

typedef struct List_Token { Token* data; size_t len; size_t capacity; } List_Token;
static inline List_Token* List_Token_new(void) { List_Token* l=(List_Token*)malloc(sizeof(List_Token)); l->data=(Token*)malloc(sizeof(Token)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Token_append(List_Token* l, Token val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Token*)realloc(l->data,sizeof(Token)*l->capacity); } l->data[l->len++]=val; }
static inline Token List_Token_get(List_Token* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Token List_Token_pop(List_Token* l) { if(!l||l->len==0) return (Token){0}; l->len--; return l->data[l->len]; }
static inline void List_Token_free(List_Token* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Pattern { Pattern* data; size_t len; size_t capacity; } List_Pattern;
static inline List_Pattern* List_Pattern_new(void) { List_Pattern* l=(List_Pattern*)malloc(sizeof(List_Pattern)); l->data=(Pattern*)malloc(sizeof(Pattern)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Pattern_append(List_Pattern* l, Pattern val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Pattern*)realloc(l->data,sizeof(Pattern)*l->capacity); } l->data[l->len++]=val; }
static inline Pattern List_Pattern_get(List_Pattern* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Pattern List_Pattern_pop(List_Pattern* l) { if(!l||l->len==0) return (Pattern){0}; l->len--; return l->data[l->len]; }
static inline void List_Pattern_free(List_Pattern* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Ownership { Ownership* data; size_t len; size_t capacity; } List_Ownership;
static inline List_Ownership* List_Ownership_new(void) { List_Ownership* l=(List_Ownership*)malloc(sizeof(List_Ownership)); l->data=(Ownership*)malloc(sizeof(Ownership)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Ownership_append(List_Ownership* l, Ownership val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Ownership*)realloc(l->data,sizeof(Ownership)*l->capacity); } l->data[l->len++]=val; }
static inline Ownership List_Ownership_get(List_Ownership* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Ownership List_Ownership_pop(List_Ownership* l) { if(!l||l->len==0) return (Ownership){0}; l->len--; return l->data[l->len]; }
static inline void List_Ownership_free(List_Ownership* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Expr { Expr* data; size_t len; size_t capacity; } List_Expr;
static inline List_Expr* List_Expr_new(void) { List_Expr* l=(List_Expr*)malloc(sizeof(List_Expr)); l->data=(Expr*)malloc(sizeof(Expr)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Expr_append(List_Expr* l, Expr val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Expr*)realloc(l->data,sizeof(Expr)*l->capacity); } l->data[l->len++]=val; }
static inline Expr List_Expr_get(List_Expr* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Expr List_Expr_pop(List_Expr* l) { if(!l||l->len==0) return (Expr){0}; l->len--; return l->data[l->len]; }
static inline void List_Expr_free(List_Expr* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Stmt { Stmt* data; size_t len; size_t capacity; } List_Stmt;
static inline List_Stmt* List_Stmt_new(void) { List_Stmt* l=(List_Stmt*)malloc(sizeof(List_Stmt)); l->data=(Stmt*)malloc(sizeof(Stmt)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Stmt_append(List_Stmt* l, Stmt val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Stmt*)realloc(l->data,sizeof(Stmt)*l->capacity); } l->data[l->len++]=val; }
static inline Stmt List_Stmt_get(List_Stmt* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Stmt List_Stmt_pop(List_Stmt* l) { if(!l||l->len==0) return (Stmt){0}; l->len--; return l->data[l->len]; }
static inline void List_Stmt_free(List_Stmt* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Decl { Decl* data; size_t len; size_t capacity; } List_Decl;
static inline List_Decl* List_Decl_new(void) { List_Decl* l=(List_Decl*)malloc(sizeof(List_Decl)); l->data=(Decl*)malloc(sizeof(Decl)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Decl_append(List_Decl* l, Decl val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Decl*)realloc(l->data,sizeof(Decl)*l->capacity); } l->data[l->len++]=val; }
static inline Decl List_Decl_get(List_Decl* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Decl List_Decl_pop(List_Decl* l) { if(!l||l->len==0) return (Decl){0}; l->len--; return l->data[l->len]; }
static inline void List_Decl_free(List_Decl* l) { if(l){ free(l->data); free(l); } }
typedef struct List_HirExpr { HirExpr* data; size_t len; size_t capacity; } List_HirExpr;
static inline List_HirExpr* List_HirExpr_new(void) { List_HirExpr* l=(List_HirExpr*)malloc(sizeof(List_HirExpr)); l->data=(HirExpr*)malloc(sizeof(HirExpr)*8); l->len=0; l->capacity=8; return l; }
static inline void List_HirExpr_append(List_HirExpr* l, HirExpr val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(HirExpr*)realloc(l->data,sizeof(HirExpr)*l->capacity); } l->data[l->len++]=val; }
static inline HirExpr List_HirExpr_get(List_HirExpr* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline HirExpr List_HirExpr_pop(List_HirExpr* l) { if(!l||l->len==0) return (HirExpr){0}; l->len--; return l->data[l->len]; }
static inline void List_HirExpr_free(List_HirExpr* l) { if(l){ free(l->data); free(l); } }
typedef struct List_HirStmt { HirStmt* data; size_t len; size_t capacity; } List_HirStmt;
static inline List_HirStmt* List_HirStmt_new(void) { List_HirStmt* l=(List_HirStmt*)malloc(sizeof(List_HirStmt)); l->data=(HirStmt*)malloc(sizeof(HirStmt)*8); l->len=0; l->capacity=8; return l; }
static inline void List_HirStmt_append(List_HirStmt* l, HirStmt val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(HirStmt*)realloc(l->data,sizeof(HirStmt)*l->capacity); } l->data[l->len++]=val; }
static inline HirStmt List_HirStmt_get(List_HirStmt* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline HirStmt List_HirStmt_pop(List_HirStmt* l) { if(!l||l->len==0) return (HirStmt){0}; l->len--; return l->data[l->len]; }
static inline void List_HirStmt_free(List_HirStmt* l) { if(l){ free(l->data); free(l); } }
typedef struct List_SymbolKind { SymbolKind* data; size_t len; size_t capacity; } List_SymbolKind;
static inline List_SymbolKind* List_SymbolKind_new(void) { List_SymbolKind* l=(List_SymbolKind*)malloc(sizeof(List_SymbolKind)); l->data=(SymbolKind*)malloc(sizeof(SymbolKind)*8); l->len=0; l->capacity=8; return l; }
static inline void List_SymbolKind_append(List_SymbolKind* l, SymbolKind val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(SymbolKind*)realloc(l->data,sizeof(SymbolKind)*l->capacity); } l->data[l->len++]=val; }
static inline SymbolKind List_SymbolKind_get(List_SymbolKind* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline SymbolKind List_SymbolKind_pop(List_SymbolKind* l) { if(!l||l->len==0) return (SymbolKind){0}; l->len--; return l->data[l->len]; }
static inline void List_SymbolKind_free(List_SymbolKind* l) { if(l){ free(l->data); free(l); } }

__attribute__((hot)) char* read_file(char* path);
__attribute__((hot)) bool file_exists(char* path);
__attribute__((hot)) bool write_file(char* path, char* content);
__attribute__((hot)) bool append_file(char* path, char* content);
__attribute__((hot)) long long _map_hash(void* key, long long cap);
__attribute__((malloc,returns_nonnull,hot)) StringObj* StringObj_init(char* s);
__attribute__((hot)) char* StringObj_as_str(StringObj* self);
__attribute__((hot)) void StringObj_append(StringObj* self, char* other);
__attribute__((hot)) void StringObj_destroy(StringObj* self);
__attribute__((malloc,returns_nonnull,hot)) StringBuilder* StringBuilder_init(long long initial_capacity);
__attribute__((hot)) void StringBuilder_append(StringBuilder* self, char* s);
__attribute__((hot)) void StringBuilder_append_char(StringBuilder* self, long long c);
__attribute__((hot)) void StringBuilder_append_int(StringBuilder* self, long long n);
__attribute__((hot)) void StringBuilder_append_float(StringBuilder* self, double f);
__attribute__((hot)) long long StringBuilder_len(StringBuilder* self);
__attribute__((hot)) StringObj* StringBuilder_to_string(StringBuilder* self);
__attribute__((hot)) char* StringBuilder_to_owned(StringBuilder* self);
__attribute__((hot)) char* StringBuilder_as_str(StringBuilder* self);
__attribute__((hot)) void StringBuilder_clear(StringBuilder* self);
__attribute__((hot)) void StringBuilder__tr_fn_free(StringBuilder* self);
__attribute__((hot)) bool Token_is_eof(Token self);
__attribute__((hot)) bool Token_is_newline(Token self);
__attribute__((hot)) bool Token_is_keyword(Token self);
__attribute__((hot)) char* Token_debug(Token self);
__attribute__((hot)) Token keyword_to_token(char* s);
__attribute__((hot)) bool char_is_digit(long long c);
__attribute__((hot)) bool char_is_alpha(long long c);
__attribute__((hot)) bool char_is_alnum(long long c);
__attribute__((hot)) bool char_is_space(long long c);
__attribute__((hot)) bool char_is_newline(long long c);
__attribute__((hot)) bool char_is_hex(long long c);
__attribute__((hot)) bool _peek_next_line_dot(char* src, long long pos, long long src_len);
__attribute__((hot)) char* raw_alloc(long long size);
__attribute__((hot)) char* raw_realloc(char* ptr, long long size);
__attribute__((hot)) void raw_free(char* ptr);
__attribute__((hot)) void raw_copy(char* dst, char* src, long long n);
__attribute__((hot)) void raw_zero(char* ptr, long long n);
__attribute__((hot)) void raw_move(char* dst, char* src, long long n);
__attribute__((hot)) void* alloc(long long n_elems);
__attribute__((hot)) void dealloc(void* ptr);
__attribute__((hot)) void* resize(void* ptr, long long new_count);
__attribute__((hot)) void copy(void* dst, void* src, long long n_elems);
__attribute__((malloc,returns_nonnull,hot)) AstType* AstType_init(char* name);
__attribute__((hot)) AstType* AstType_init_generic(char* name, AstType** arg);
__attribute__((malloc,returns_nonnull,hot)) GenericConstraint* GenericConstraint_init(char* target);
__attribute__((malloc,returns_nonnull,hot)) Decorator* Decorator_init(char* name);
__attribute__((malloc,returns_nonnull,hot)) Comprehension* Comprehension_init(char* target, Expr* iter);
__attribute__((malloc,returns_nonnull,hot)) CatchClause* CatchClause_init(char* err_name, Block** body);
__attribute__((malloc,returns_nonnull,hot)) MatchArm* MatchArm_init(Pattern pat, Block** body);
__attribute__((hot)) FStringPart* FStringPart_init_text(char* s);
__attribute__((hot)) FStringPart* FStringPart_init_expr(Expr* e);
__attribute__((hot)) FStringPart* FStringPart_init_expr_fmt(Expr* e, char* spec);
__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_recv(Expr* chan, char* var, Block* body);
__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_send(Expr* chan, Expr* val, Block* body);
__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_timeout(Expr* ms, Block* body);
__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_default(Block* body);
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
__attribute__((hot)) void _dummy_instantiations();
__attribute__((hot)) Expr* box_expr(Expr e);
__attribute__((hot)) Stmt* box_stmt(Stmt s);
__attribute__((hot)) Decl* box_decl(Decl d);
__attribute__((hot)) AstType** box_asttype(AstType* t);
__attribute__((malloc,returns_nonnull,hot)) Parser* Parser_init(List_Token* tokens, List_i64* lines);
__attribute__((hot)) Token Parser_peek(Parser* self);
__attribute__((hot)) Token Parser_advance(Parser* self);
__attribute__((hot)) void Parser_skip_newlines(Parser* self);
__attribute__((hot)) void Parser_skip_newlines_and_indent(Parser* self);
__attribute__((hot)) void Parser_expect_newline(Parser* self);
__attribute__((hot)) bool Parser_at_end(Parser* self);
__attribute__((hot)) long long Parser_cur_line(Parser* self);
__attribute__((hot)) char* Parser_consume_ident(Parser* self);
__attribute__((hot)) char* Parser_consume_module_ident(Parser* self);
__attribute__((hot)) AstType* Parser_parse_type(Parser* self);
__attribute__((hot)) List_ptr* Parser_parse_param_list(Parser* self);
__attribute__((hot)) Block* Parser_parse_block(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_try_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_assert_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_with_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_asm_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_spawn_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_taskgroup_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_chan_select_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_gpu_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_let_stmt(Parser* self, bool is_mut);
__attribute__((hot)) Stmt* Parser_parse_shared_let_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_const_let_stmt(Parser* self);
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
__attribute__((hot)) long long _find_fmt_colon(char* s);
__attribute__((hot)) bool decl_is_pub(Decl d);
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
__attribute__((hot)) HirExpr* box_hirexpr(HirExpr e);
__attribute__((hot)) HirStmt* box_hirstmt(HirStmt s);
__attribute__((hot)) AstType* hir_expr_type(HirExpr* e);
__attribute__((hot)) long long _tr_str_len(char* s);
__attribute__((malloc,returns_nonnull,hot)) Symbol* Symbol_init(char* name, SymbolKind kind, AstType** ty);
__attribute__((malloc,returns_nonnull,hot)) Scope* Scope_init();
__attribute__((hot)) AstType** Sema_build_ast_type(Sema* self, Expr* e);
__attribute__((malloc,returns_nonnull,hot)) Sema* Sema_init();
__attribute__((hot)) void Sema_error(Sema* self, char* msg);
__attribute__((hot)) bool Sema_is_sendable_type(Sema* self, char* ty_name);
__attribute__((hot)) void Sema_check_spawn_sendable(Sema* self, HirExpr* e);
__attribute__((hot)) void Sema_check_class_sendable_fields(Sema* self, ClassDef* c);
__attribute__((hot)) void Sema_mark_moved(Sema* self, char* name);
__attribute__((hot)) void Sema_mark_freed(Sema* self, char* name);
__attribute__((hot)) void Sema_check_not_moved(Sema* self, char* name, char* ty_name);
__attribute__((hot)) void Sema_mark_borrow(Sema* self, char* name);
__attribute__((hot)) void Sema_unmark_borrow(Sema* self, char* name);
__attribute__((hot)) void Sema_check_no_active_borrows(Sema* self, char* name, char* ty_name);
__attribute__((hot)) void Sema_mark_init(Sema* self, char* name);
__attribute__((hot)) void Sema_clear_container_borrow(Sema* self, char* var_name);
__attribute__((hot)) void Sema_unmark_moved(Sema* self, char* name);
__attribute__((hot)) void Sema_mark_maybe_moved(Sema* self, char* name);
__attribute__((hot)) void Sema_unmark_init(Sema* self, char* name);
__attribute__((hot)) void Sema_mark_maybe_init(Sema* self, char* name);
__attribute__((hot)) bool Sema_vec_str_contains(Sema* self, List_str* v, char* s);
__attribute__((hot)) bool Sema_has_copy_decorator(Sema* self, List_ptr* decs);
__attribute__((hot)) bool Sema_is_copy_class(Sema* self, char* name);
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
__attribute__((hot)) void Sema_declare_pattern_binds_typed(Sema* self, Pattern pat, AstType* subj_ty);
__attribute__((hot)) AstType* Sema_str_method_ret_ty(Sema* self, char* method);
__attribute__((hot)) HirExpr* Sema_lower_expr(Sema* self, Expr* e_ptr);
__attribute__((hot)) char* Sema_is_reserved_error(Sema* self, char* name);
__attribute__((hot)) char* Sema_is_reserved_keyword(Sema* self, char* name);
__attribute__((hot)) bool Sema_block_returns(Sema* self, Block* b);
__attribute__((hot)) bool Sema_is_primitive(Sema* self, AstType* ty);
__attribute__((hot)) bool Sema_is_primitive_name(Sema* self, char* name);
__attribute__((hot)) Symbol** box_symbol(Symbol* s);
__attribute__((hot)) char* _c_dot_to_safe(char* s);
__attribute__((hot)) char* _indent_str(long long n);
__attribute__((hot)) bool _is_invalid_ptr(unsigned long long addr);
__attribute__((hot)) bool _is_str_type(char* n);
__attribute__((hot)) bool _is_int_type(char* n);
__attribute__((hot)) bool _is_float_type(char* n);
__attribute__((hot)) char* _safe_c_varname(char* n);
__attribute__((hot)) bool _is_c_keyword(char* n);
__attribute__((hot)) bool _starts_with_tr(char* s);
__attribute__((hot)) bool _is_primitive(char* n);
__attribute__((hot)) char* _escape_str_for_c(char* s);
__attribute__((hot)) char* llvm_type(AstType* ty);
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
__attribute__((hot)) void print_version();
__attribute__((hot)) void print_usage();
__attribute__((hot)) bool str_ends_with_dot_tr(char* path);
__attribute__((hot)) char* strip_extension(char* path);
__attribute__((hot)) bool str_starts_with(char* s, char* prefix);
__attribute__((hot)) char* detect_c_compiler();
__attribute__((hot)) bool is_clang_compiler(char* cc);
__attribute__((hot)) char* resolve_target_triple(char* target);
__attribute__((hot)) char* target_extra_flags(char* triple);
__attribute__((hot)) char* detect_cross_compiler(char* triple);
__attribute__((hot)) char* dir_of_path(char* path);
__attribute__((hot)) char* strip_trailing_sep_inline(char* s);
__attribute__((hot)) char* read_runtime_header(char* bin_path, char* input_path);
__attribute__((hot)) void ensure_runtime_header(char* out_dir, char* bin_path, char* input_path);
__attribute__((hot)) void sync_headers_to_runtime(char* rt_content, char* types_content);
__attribute__((hot)) char* strip_trailing_sep(char* s);
__attribute__((hot)) long long count_path_env_entries(char* s);
__attribute__((hot)) char* get_path_env_entry(char* s, long long idx);
__attribute__((hot)) char* path_to_native(char* s);
__attribute__((hot)) char* dot_to_safe(char* s);
__attribute__((hot)) char* dot_last_seg(char* s);
__attribute__((hot)) char* get_filename(char* path);
__attribute__((hot)) long long get_dot_depth(char* dot_path);
__attribute__((hot)) char* ensure_builtin_dirs(char* build_dir, char* dot_path);
__attribute__((hot)) bool is_builtin_mod(char* dot_path);
__attribute__((hot)) void make_dir(char* path);
__attribute__((hot)) long long compile_all_c(List_str* c_files, char* exe_path, char* inc_dir, List_str* link_paths, List_str* lib_flags, char* opt_level, bool verbose, bool static_link, char* target, char* sysroot, bool debug_mode);
__attribute__((hot)) long long compile_c_to_exe(char* c_path, char* exe_path, char* opt_level, bool verbose);
__attribute__((hot)) void _print_diag(char* level, char* msg);
__attribute__((hot)) void cleanup_build(char* build_dir, List_str* all_c_files);

__attribute__((malloc,returns_nonnull,hot)) Lexer* Lexer_init(char* source);
__attribute__((hot)) long long Lexer_peek(Lexer* self);
__attribute__((hot)) long long Lexer_peek_at(Lexer* self, long long offset);
__attribute__((hot)) long long Lexer_advance(Lexer* self);
__attribute__((hot)) bool Lexer_at_end(Lexer* self);
__attribute__((hot)) void Lexer_skip_spaces(Lexer* self);
__attribute__((hot)) void Lexer_skip_comment(Lexer* self);
__attribute__((hot)) Token Lexer_read_int(Lexer* self);
__attribute__((hot)) Token Lexer_read_triple_string(Lexer* self, long long quote);
__attribute__((hot)) Token Lexer_read_string(Lexer* self, long long quote);
__attribute__((hot)) Token Lexer_read_char(Lexer* self);
__attribute__((hot)) Token Lexer_read_fstring(Lexer* self);
__attribute__((hot)) Token Lexer_read_raw_string(Lexer* self);
__attribute__((hot)) Token Lexer_read_byte_string(Lexer* self);
__attribute__((hot)) Token Lexer_read_ident(Lexer* self);
__attribute__((hot)) List_Token* Lexer_tokenize(Lexer* self);
__attribute__((malloc,returns_nonnull,hot)) CGenerator* CGenerator_init();
__attribute__((hot)) char* CGenerator_next_temp(CGenerator* self);
__attribute__((hot)) void CGenerator_reset_defer_stack(CGenerator* self);
__attribute__((hot)) void CGenerator_gen_func_body(CGenerator* self, HirBlock* body, long long indent);
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
__attribute__((hot)) char* CGenerator_synth_class_suffix(CGenerator* self, HirClass* ucls);
__attribute__((hot)) void CGenerator_ensure_mono(CGenerator* self, HirClass* cls, List_ptr* type_args);
__attribute__((hot)) void CGenerator_ensure_mono_func(CGenerator* self, char* fname, char* targ);
__attribute__((hot)) char* CGenerator_get_user_decorator_attr(CGenerator* self, char* name);
__attribute__((hot)) char* CGenerator_get_inline_attrs(CGenerator* self, HirFunction* f);
__attribute__((hot)) bool CGenerator_is_rt_concurrency_type(CGenerator* self, char* name);
__attribute__((hot)) char* CGenerator_get_proto_attrs(CGenerator* self, HirFunction* f);
__attribute__((hot)) char* CGenerator_gen_func_sig(CGenerator* self, HirFunction* f, char* class_name);
__attribute__((hot)) void CGenerator_emit_base_fields(CGenerator* self, char* base_name);
__attribute__((hot)) void CGenerator_gen_class_struct(CGenerator* self, HirClass* c);
__attribute__((hot)) void CGenerator_gen_enum_struct(CGenerator* self, HirEnum* e);
__attribute__((hot)) void CGenerator_gen_interface_vtable(CGenerator* self, HirInterface* iface);
__attribute__((hot)) char* CGenerator_gen_one_iface_wrap(CGenerator* self, char* cls_name, HirInterface* iface);
__attribute__((hot)) char* CGenerator_gen_expr(CGenerator* self, HirExpr* e_ptr);
__attribute__((hot)) bool CGenerator_has_method(CGenerator* self, char* cls_name, char* method);
__attribute__((hot)) AstType* CGenerator_cls_method_ret_ty(CGenerator* self, char* cls_name, char* method);
__attribute__((hot)) char* CGenerator_cls_method_c_call(CGenerator* self, char* cls_name, char* method, char* obj_s, char* extra_args);
__attribute__((hot)) char* CGenerator_gen_cond_expr(CGenerator* self, HirExpr* cond);
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
__attribute__((hot)) void CGenerator_emit_async_wrapper_for_call(CGenerator* self, char* fn_name, List_ptr* args, char* ret_name);
__attribute__((hot)) char* CGenerator_gen_await_call(CGenerator* self, HirExpr* expr);
__attribute__((hot)) char* CGenerator_gen_await_timeout_call(CGenerator* self, HirExpr* expr, HirExpr* ms_expr);
__attribute__((hot)) void CGenerator_gen_multi_let(CGenerator* self, List_str* names, bool is_mut, HirExpr* val, long long indent);
__attribute__((hot)) char* CGenerator_gen_list_literal(CGenerator* self, List_ptr* items, AstType* ty);
__attribute__((hot)) char* CGenerator_gen_dict_literal(CGenerator* self, List_ptr* keys, List_ptr* vals, AstType* hint_ty);
__attribute__((hot)) char* CGenerator_gen_list_comp(CGenerator* self, HirExpr* element, List_ptr* generators);
__attribute__((hot)) char* CGenerator_gen_closure(CGenerator* self, List_ptr* params, AstType* ret_ty, HirBlock* body, List_ptr* captures);
__attribute__((hot)) void CGenerator_emit_spawn_wrapper_for_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator_prescan_block_spawns(CGenerator* self, HirBlock* block);
__attribute__((hot)) void CGenerator_prescan_stmt_spawns(CGenerator* self, HirStmt* s);
__attribute__((hot)) void CGenerator_prescan_await_all_in_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator_prescan_spawns(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_prescan_expr_awaits(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator_prescan_block_awaits(CGenerator* self, HirBlock* block);
__attribute__((hot)) void CGenerator_prescan_stmt_awaits(CGenerator* self, HirStmt* s);
__attribute__((hot)) void CGenerator_prescan_awaits(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_gen_stmt(CGenerator* self, HirStmt* s_ptr, long long indent);
__attribute__((hot)) void CGenerator_gen_for_loop(CGenerator* self, char* var, HirExpr* iter, HirBlock* body, long long indent);
__attribute__((hot)) void CGenerator_gen_for_unpack(CGenerator* self, List_str* vars, HirExpr* iter, HirBlock* body, long long indent);
__attribute__((hot)) void CGenerator_gen_try(CGenerator* self, HirBlock* try_body, List_ptr* catches, HirBlock* finally_b, long long indent);
__attribute__((hot)) void CGenerator_gen_chan_select(CGenerator* self, List_ptr* arms, long long indent);
__attribute__((hot)) void CGenerator_gen_block(CGenerator* self, HirBlock* b, long long indent);
__attribute__((hot)) void CGenerator_gen_match(CGenerator* self, HirExpr* expr, List_ptr* arms, long long indent);
__attribute__((hot)) void CGenerator_register_program(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_scan_mono_ty(CGenerator* self, AstType* ty);
__attribute__((hot)) void CGenerator_scan_mono_block(CGenerator* self, HirBlock* block);
__attribute__((hot)) void CGenerator_scan_mono_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator_scan_mono_stmt(CGenerator* self, HirStmt* s_ptr);
__attribute__((hot)) void CGenerator_scan_mono_func(CGenerator* self, HirFunction* f);
__attribute__((hot)) void CGenerator_scan_mono_prog(CGenerator* self, HirProgram* prog);
__attribute__((hot)) char* CGenerator_generate(CGenerator* self, HirProgram* prog);
__attribute__((hot)) char* CGenerator_generate_types_header(CGenerator* self, HirProgram* prog);
__attribute__((hot)) char* CGenerator_generate_module_compat(CGenerator* self, List_str* all_decl_modules, List_ptr* all_decls);
__attribute__((hot)) char* CGenerator_generate_module_c(CGenerator* self, HirProgram* prog, TrMap* class_set, TrMap* fn_set, long long depth);
__attribute__((hot)) char* CGenerator_generate_main_c(CGenerator* self, HirProgram* prog, TrMap* class_set, TrMap* fn_set);


/* === Module-prefixed typedef aliases (auto-generated) === */
/* Maps module-qualified C names to short-name types in tauraro_types.h */

#ifndef TAURARO_RT_NO_STRINGBUILDER
typedef struct core_string_StringObj core_string_StringObj;
typedef core_string_StringObj StringObj;
typedef struct core_string_StringBuilder core_string_StringBuilder;
typedef core_string_StringBuilder StringBuilder;
#endif

typedef StringObj core_string_StringObj;
struct core_vec_Vec_core_string_StringObj { core_string_StringObj** data; long long len; long long capacity; };
typedef struct core_vec_Vec_core_string_StringObj core_vec_Vec_core_string_StringObj;
struct core_vec_Vec_core_string_StringObj_ptr { core_string_StringObj*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_core_string_StringObj_ptr core_vec_Vec_core_string_StringObj_ptr;
struct core_map_MapNode_str_core_string_StringObj { char* key; core_string_StringObj* value; struct core_map_MapNode_str_core_string_StringObj* next; };
typedef struct core_map_MapNode_str_core_string_StringObj core_map_MapNode_str_core_string_StringObj;
struct core_map_Map_str_core_string_StringObj { core_map_MapNode_str_core_string_StringObj** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_core_string_StringObj core_map_Map_str_core_string_StringObj;
__attribute__((hot)) core_string_StringObj** core_alloc_alloc_core_string_StringObj(long long count);
__attribute__((hot)) core_string_StringObj** core_alloc_resize_core_string_StringObj(core_string_StringObj** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_core_string_StringObj(core_string_StringObj** ptr);
__attribute__((hot)) core_string_StringObj*** core_alloc_alloc_core_string_StringObj_ptr(long long count);
__attribute__((hot)) core_string_StringObj*** core_alloc_resize_core_string_StringObj_ptr(core_string_StringObj*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_core_string_StringObj_ptr(core_string_StringObj*** ptr);
__attribute__((hot)) core_map_MapNode_str_core_string_StringObj** core_alloc_alloc_core_map_MapNode_str_core_string_StringObj(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_core_string_StringObj(core_map_MapNode_str_core_string_StringObj** ptr);

typedef StringBuilder core_string_StringBuilder;
struct core_vec_Vec_core_string_StringBuilder { core_string_StringBuilder** data; long long len; long long capacity; };
typedef struct core_vec_Vec_core_string_StringBuilder core_vec_Vec_core_string_StringBuilder;
struct core_vec_Vec_core_string_StringBuilder_ptr { core_string_StringBuilder*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_core_string_StringBuilder_ptr core_vec_Vec_core_string_StringBuilder_ptr;
struct core_map_MapNode_str_core_string_StringBuilder { char* key; core_string_StringBuilder* value; struct core_map_MapNode_str_core_string_StringBuilder* next; };
typedef struct core_map_MapNode_str_core_string_StringBuilder core_map_MapNode_str_core_string_StringBuilder;
struct core_map_Map_str_core_string_StringBuilder { core_map_MapNode_str_core_string_StringBuilder** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_core_string_StringBuilder core_map_Map_str_core_string_StringBuilder;
__attribute__((hot)) core_string_StringBuilder** core_alloc_alloc_core_string_StringBuilder(long long count);
__attribute__((hot)) core_string_StringBuilder** core_alloc_resize_core_string_StringBuilder(core_string_StringBuilder** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_core_string_StringBuilder(core_string_StringBuilder** ptr);
__attribute__((hot)) core_string_StringBuilder*** core_alloc_alloc_core_string_StringBuilder_ptr(long long count);
__attribute__((hot)) core_string_StringBuilder*** core_alloc_resize_core_string_StringBuilder_ptr(core_string_StringBuilder*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_core_string_StringBuilder_ptr(core_string_StringBuilder*** ptr);
__attribute__((hot)) core_map_MapNode_str_core_string_StringBuilder** core_alloc_alloc_core_map_MapNode_str_core_string_StringBuilder(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_core_string_StringBuilder(core_map_MapNode_str_core_string_StringBuilder** ptr);

typedef Token token_Token;
struct core_vec_Vec_token_Token { token_Token* data; long long len; long long capacity; };
typedef struct core_vec_Vec_token_Token core_vec_Vec_token_Token;
struct core_vec_Vec_token_Token_ptr { token_Token** data; long long len; long long capacity; };
typedef struct core_vec_Vec_token_Token_ptr core_vec_Vec_token_Token_ptr;
__attribute__((hot)) token_Token* core_alloc_alloc_token_Token(long long count);
__attribute__((hot)) token_Token* core_alloc_resize_token_Token(token_Token* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_token_Token(token_Token* ptr);
__attribute__((hot)) token_Token** core_alloc_alloc_token_Token_ptr(long long count);
__attribute__((hot)) token_Token** core_alloc_resize_token_Token_ptr(token_Token** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_token_Token_ptr(token_Token** ptr);

typedef Lexer lexer_Lexer;
struct core_vec_Vec_lexer_Lexer { lexer_Lexer** data; long long len; long long capacity; };
typedef struct core_vec_Vec_lexer_Lexer core_vec_Vec_lexer_Lexer;
struct core_vec_Vec_lexer_Lexer_ptr { lexer_Lexer*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_lexer_Lexer_ptr core_vec_Vec_lexer_Lexer_ptr;
struct core_map_MapNode_str_lexer_Lexer { char* key; lexer_Lexer* value; struct core_map_MapNode_str_lexer_Lexer* next; };
typedef struct core_map_MapNode_str_lexer_Lexer core_map_MapNode_str_lexer_Lexer;
struct core_map_Map_str_lexer_Lexer { core_map_MapNode_str_lexer_Lexer** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_lexer_Lexer core_map_Map_str_lexer_Lexer;
__attribute__((hot)) lexer_Lexer** core_alloc_alloc_lexer_Lexer(long long count);
__attribute__((hot)) lexer_Lexer** core_alloc_resize_lexer_Lexer(lexer_Lexer** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_lexer_Lexer(lexer_Lexer** ptr);
__attribute__((hot)) lexer_Lexer*** core_alloc_alloc_lexer_Lexer_ptr(long long count);
__attribute__((hot)) lexer_Lexer*** core_alloc_resize_lexer_Lexer_ptr(lexer_Lexer*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_lexer_Lexer_ptr(lexer_Lexer*** ptr);
__attribute__((hot)) core_map_MapNode_str_lexer_Lexer** core_alloc_alloc_core_map_MapNode_str_lexer_Lexer(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_lexer_Lexer(core_map_MapNode_str_lexer_Lexer** ptr);

typedef AstType ast_AstType;
struct core_vec_Vec_ast_AstType { ast_AstType** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_AstType core_vec_Vec_ast_AstType;
struct core_vec_Vec_ast_AstType_ptr { ast_AstType*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_AstType_ptr core_vec_Vec_ast_AstType_ptr;
struct core_map_MapNode_str_ast_AstType { char* key; ast_AstType* value; struct core_map_MapNode_str_ast_AstType* next; };
typedef struct core_map_MapNode_str_ast_AstType core_map_MapNode_str_ast_AstType;
struct core_map_Map_str_ast_AstType { core_map_MapNode_str_ast_AstType** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_AstType core_map_Map_str_ast_AstType;
__attribute__((hot)) ast_AstType** core_alloc_alloc_ast_AstType(long long count);
__attribute__((hot)) ast_AstType** core_alloc_resize_ast_AstType(ast_AstType** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_AstType(ast_AstType** ptr);
__attribute__((hot)) ast_AstType*** core_alloc_alloc_ast_AstType_ptr(long long count);
__attribute__((hot)) ast_AstType*** core_alloc_resize_ast_AstType_ptr(ast_AstType*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_AstType_ptr(ast_AstType*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_AstType** core_alloc_alloc_core_map_MapNode_str_ast_AstType(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_AstType(core_map_MapNode_str_ast_AstType** ptr);

typedef GenericConstraint ast_GenericConstraint;
struct core_vec_Vec_ast_GenericConstraint { ast_GenericConstraint** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_GenericConstraint core_vec_Vec_ast_GenericConstraint;
struct core_vec_Vec_ast_GenericConstraint_ptr { ast_GenericConstraint*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_GenericConstraint_ptr core_vec_Vec_ast_GenericConstraint_ptr;
struct core_map_MapNode_str_ast_GenericConstraint { char* key; ast_GenericConstraint* value; struct core_map_MapNode_str_ast_GenericConstraint* next; };
typedef struct core_map_MapNode_str_ast_GenericConstraint core_map_MapNode_str_ast_GenericConstraint;
struct core_map_Map_str_ast_GenericConstraint { core_map_MapNode_str_ast_GenericConstraint** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_GenericConstraint core_map_Map_str_ast_GenericConstraint;
__attribute__((hot)) ast_GenericConstraint** core_alloc_alloc_ast_GenericConstraint(long long count);
__attribute__((hot)) ast_GenericConstraint** core_alloc_resize_ast_GenericConstraint(ast_GenericConstraint** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_GenericConstraint(ast_GenericConstraint** ptr);
__attribute__((hot)) ast_GenericConstraint*** core_alloc_alloc_ast_GenericConstraint_ptr(long long count);
__attribute__((hot)) ast_GenericConstraint*** core_alloc_resize_ast_GenericConstraint_ptr(ast_GenericConstraint*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_GenericConstraint_ptr(ast_GenericConstraint*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_GenericConstraint** core_alloc_alloc_core_map_MapNode_str_ast_GenericConstraint(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_GenericConstraint(core_map_MapNode_str_ast_GenericConstraint** ptr);

typedef Decorator ast_Decorator;
struct core_vec_Vec_ast_Decorator { ast_Decorator** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Decorator core_vec_Vec_ast_Decorator;
struct core_vec_Vec_ast_Decorator_ptr { ast_Decorator*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Decorator_ptr core_vec_Vec_ast_Decorator_ptr;
struct core_map_MapNode_str_ast_Decorator { char* key; ast_Decorator* value; struct core_map_MapNode_str_ast_Decorator* next; };
typedef struct core_map_MapNode_str_ast_Decorator core_map_MapNode_str_ast_Decorator;
struct core_map_Map_str_ast_Decorator { core_map_MapNode_str_ast_Decorator** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Decorator core_map_Map_str_ast_Decorator;
__attribute__((hot)) ast_Decorator** core_alloc_alloc_ast_Decorator(long long count);
__attribute__((hot)) ast_Decorator** core_alloc_resize_ast_Decorator(ast_Decorator** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Decorator(ast_Decorator** ptr);
__attribute__((hot)) ast_Decorator*** core_alloc_alloc_ast_Decorator_ptr(long long count);
__attribute__((hot)) ast_Decorator*** core_alloc_resize_ast_Decorator_ptr(ast_Decorator*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Decorator_ptr(ast_Decorator*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Decorator** core_alloc_alloc_core_map_MapNode_str_ast_Decorator(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Decorator(core_map_MapNode_str_ast_Decorator** ptr);

typedef Pattern ast_Pattern;
struct core_vec_Vec_ast_Pattern { ast_Pattern* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Pattern core_vec_Vec_ast_Pattern;
struct core_vec_Vec_ast_Pattern_ptr { ast_Pattern** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Pattern_ptr core_vec_Vec_ast_Pattern_ptr;
__attribute__((hot)) ast_Pattern* core_alloc_alloc_ast_Pattern(long long count);
__attribute__((hot)) ast_Pattern* core_alloc_resize_ast_Pattern(ast_Pattern* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Pattern(ast_Pattern* ptr);
__attribute__((hot)) ast_Pattern** core_alloc_alloc_ast_Pattern_ptr(long long count);
__attribute__((hot)) ast_Pattern** core_alloc_resize_ast_Pattern_ptr(ast_Pattern** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Pattern_ptr(ast_Pattern** ptr);

typedef Ownership ast_Ownership;
struct core_vec_Vec_ast_Ownership { ast_Ownership* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Ownership core_vec_Vec_ast_Ownership;
struct core_vec_Vec_ast_Ownership_ptr { ast_Ownership** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Ownership_ptr core_vec_Vec_ast_Ownership_ptr;
__attribute__((hot)) ast_Ownership* core_alloc_alloc_ast_Ownership(long long count);
__attribute__((hot)) ast_Ownership* core_alloc_resize_ast_Ownership(ast_Ownership* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Ownership(ast_Ownership* ptr);
__attribute__((hot)) ast_Ownership** core_alloc_alloc_ast_Ownership_ptr(long long count);
__attribute__((hot)) ast_Ownership** core_alloc_resize_ast_Ownership_ptr(ast_Ownership** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Ownership_ptr(ast_Ownership** ptr);

typedef Comprehension ast_Comprehension;
struct core_vec_Vec_ast_Comprehension { ast_Comprehension** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Comprehension core_vec_Vec_ast_Comprehension;
struct core_vec_Vec_ast_Comprehension_ptr { ast_Comprehension*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Comprehension_ptr core_vec_Vec_ast_Comprehension_ptr;
struct core_map_MapNode_str_ast_Comprehension { char* key; ast_Comprehension* value; struct core_map_MapNode_str_ast_Comprehension* next; };
typedef struct core_map_MapNode_str_ast_Comprehension core_map_MapNode_str_ast_Comprehension;
struct core_map_Map_str_ast_Comprehension { core_map_MapNode_str_ast_Comprehension** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Comprehension core_map_Map_str_ast_Comprehension;
__attribute__((hot)) ast_Comprehension** core_alloc_alloc_ast_Comprehension(long long count);
__attribute__((hot)) ast_Comprehension** core_alloc_resize_ast_Comprehension(ast_Comprehension** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Comprehension(ast_Comprehension** ptr);
__attribute__((hot)) ast_Comprehension*** core_alloc_alloc_ast_Comprehension_ptr(long long count);
__attribute__((hot)) ast_Comprehension*** core_alloc_resize_ast_Comprehension_ptr(ast_Comprehension*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Comprehension_ptr(ast_Comprehension*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Comprehension** core_alloc_alloc_core_map_MapNode_str_ast_Comprehension(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Comprehension(core_map_MapNode_str_ast_Comprehension** ptr);

typedef CatchClause ast_CatchClause;
struct core_vec_Vec_ast_CatchClause { ast_CatchClause** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_CatchClause core_vec_Vec_ast_CatchClause;
struct core_vec_Vec_ast_CatchClause_ptr { ast_CatchClause*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_CatchClause_ptr core_vec_Vec_ast_CatchClause_ptr;
struct core_map_MapNode_str_ast_CatchClause { char* key; ast_CatchClause* value; struct core_map_MapNode_str_ast_CatchClause* next; };
typedef struct core_map_MapNode_str_ast_CatchClause core_map_MapNode_str_ast_CatchClause;
struct core_map_Map_str_ast_CatchClause { core_map_MapNode_str_ast_CatchClause** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_CatchClause core_map_Map_str_ast_CatchClause;
__attribute__((hot)) ast_CatchClause** core_alloc_alloc_ast_CatchClause(long long count);
__attribute__((hot)) ast_CatchClause** core_alloc_resize_ast_CatchClause(ast_CatchClause** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_CatchClause(ast_CatchClause** ptr);
__attribute__((hot)) ast_CatchClause*** core_alloc_alloc_ast_CatchClause_ptr(long long count);
__attribute__((hot)) ast_CatchClause*** core_alloc_resize_ast_CatchClause_ptr(ast_CatchClause*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_CatchClause_ptr(ast_CatchClause*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_CatchClause** core_alloc_alloc_core_map_MapNode_str_ast_CatchClause(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_CatchClause(core_map_MapNode_str_ast_CatchClause** ptr);

typedef MatchArm ast_MatchArm;
struct core_vec_Vec_ast_MatchArm { ast_MatchArm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_MatchArm core_vec_Vec_ast_MatchArm;
struct core_vec_Vec_ast_MatchArm_ptr { ast_MatchArm*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_MatchArm_ptr core_vec_Vec_ast_MatchArm_ptr;
struct core_map_MapNode_str_ast_MatchArm { char* key; ast_MatchArm* value; struct core_map_MapNode_str_ast_MatchArm* next; };
typedef struct core_map_MapNode_str_ast_MatchArm core_map_MapNode_str_ast_MatchArm;
struct core_map_Map_str_ast_MatchArm { core_map_MapNode_str_ast_MatchArm** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_MatchArm core_map_Map_str_ast_MatchArm;
__attribute__((hot)) ast_MatchArm** core_alloc_alloc_ast_MatchArm(long long count);
__attribute__((hot)) ast_MatchArm** core_alloc_resize_ast_MatchArm(ast_MatchArm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_MatchArm(ast_MatchArm** ptr);
__attribute__((hot)) ast_MatchArm*** core_alloc_alloc_ast_MatchArm_ptr(long long count);
__attribute__((hot)) ast_MatchArm*** core_alloc_resize_ast_MatchArm_ptr(ast_MatchArm*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_MatchArm_ptr(ast_MatchArm*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_MatchArm** core_alloc_alloc_core_map_MapNode_str_ast_MatchArm(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_MatchArm(core_map_MapNode_str_ast_MatchArm** ptr);

typedef FStringPart ast_FStringPart;
struct core_vec_Vec_ast_FStringPart { ast_FStringPart** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FStringPart core_vec_Vec_ast_FStringPart;
struct core_vec_Vec_ast_FStringPart_ptr { ast_FStringPart*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FStringPart_ptr core_vec_Vec_ast_FStringPart_ptr;
struct core_map_MapNode_str_ast_FStringPart { char* key; ast_FStringPart* value; struct core_map_MapNode_str_ast_FStringPart* next; };
typedef struct core_map_MapNode_str_ast_FStringPart core_map_MapNode_str_ast_FStringPart;
struct core_map_Map_str_ast_FStringPart { core_map_MapNode_str_ast_FStringPart** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_FStringPart core_map_Map_str_ast_FStringPart;
__attribute__((hot)) ast_FStringPart** core_alloc_alloc_ast_FStringPart(long long count);
__attribute__((hot)) ast_FStringPart** core_alloc_resize_ast_FStringPart(ast_FStringPart** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FStringPart(ast_FStringPart** ptr);
__attribute__((hot)) ast_FStringPart*** core_alloc_alloc_ast_FStringPart_ptr(long long count);
__attribute__((hot)) ast_FStringPart*** core_alloc_resize_ast_FStringPart_ptr(ast_FStringPart*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FStringPart_ptr(ast_FStringPart*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_FStringPart** core_alloc_alloc_core_map_MapNode_str_ast_FStringPart(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_FStringPart(core_map_MapNode_str_ast_FStringPart** ptr);

typedef Expr ast_Expr;
struct core_vec_Vec_ast_Expr { ast_Expr* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Expr core_vec_Vec_ast_Expr;
struct core_vec_Vec_ast_Expr_ptr { ast_Expr** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Expr_ptr core_vec_Vec_ast_Expr_ptr;
__attribute__((hot)) ast_Expr* core_alloc_alloc_ast_Expr(long long count);
__attribute__((hot)) ast_Expr* core_alloc_resize_ast_Expr(ast_Expr* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Expr(ast_Expr* ptr);
__attribute__((hot)) ast_Expr** core_alloc_alloc_ast_Expr_ptr(long long count);
__attribute__((hot)) ast_Expr** core_alloc_resize_ast_Expr_ptr(ast_Expr** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Expr_ptr(ast_Expr** ptr);

typedef ChanSelectArm ast_ChanSelectArm;
struct core_vec_Vec_ast_ChanSelectArm { ast_ChanSelectArm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ChanSelectArm core_vec_Vec_ast_ChanSelectArm;
struct core_vec_Vec_ast_ChanSelectArm_ptr { ast_ChanSelectArm*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ChanSelectArm_ptr core_vec_Vec_ast_ChanSelectArm_ptr;
struct core_map_MapNode_str_ast_ChanSelectArm { char* key; ast_ChanSelectArm* value; struct core_map_MapNode_str_ast_ChanSelectArm* next; };
typedef struct core_map_MapNode_str_ast_ChanSelectArm core_map_MapNode_str_ast_ChanSelectArm;
struct core_map_Map_str_ast_ChanSelectArm { core_map_MapNode_str_ast_ChanSelectArm** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_ChanSelectArm core_map_Map_str_ast_ChanSelectArm;
__attribute__((hot)) ast_ChanSelectArm** core_alloc_alloc_ast_ChanSelectArm(long long count);
__attribute__((hot)) ast_ChanSelectArm** core_alloc_resize_ast_ChanSelectArm(ast_ChanSelectArm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ChanSelectArm(ast_ChanSelectArm** ptr);
__attribute__((hot)) ast_ChanSelectArm*** core_alloc_alloc_ast_ChanSelectArm_ptr(long long count);
__attribute__((hot)) ast_ChanSelectArm*** core_alloc_resize_ast_ChanSelectArm_ptr(ast_ChanSelectArm*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ChanSelectArm_ptr(ast_ChanSelectArm*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_ChanSelectArm** core_alloc_alloc_core_map_MapNode_str_ast_ChanSelectArm(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_ChanSelectArm(core_map_MapNode_str_ast_ChanSelectArm** ptr);

typedef Stmt ast_Stmt;
struct core_vec_Vec_ast_Stmt { ast_Stmt* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Stmt core_vec_Vec_ast_Stmt;
struct core_vec_Vec_ast_Stmt_ptr { ast_Stmt** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Stmt_ptr core_vec_Vec_ast_Stmt_ptr;
__attribute__((hot)) ast_Stmt* core_alloc_alloc_ast_Stmt(long long count);
__attribute__((hot)) ast_Stmt* core_alloc_resize_ast_Stmt(ast_Stmt* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Stmt(ast_Stmt* ptr);
__attribute__((hot)) ast_Stmt** core_alloc_alloc_ast_Stmt_ptr(long long count);
__attribute__((hot)) ast_Stmt** core_alloc_resize_ast_Stmt_ptr(ast_Stmt** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Stmt_ptr(ast_Stmt** ptr);

typedef Block ast_Block;
struct core_vec_Vec_ast_Block { ast_Block** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Block core_vec_Vec_ast_Block;
struct core_vec_Vec_ast_Block_ptr { ast_Block*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Block_ptr core_vec_Vec_ast_Block_ptr;
struct core_map_MapNode_str_ast_Block { char* key; ast_Block* value; struct core_map_MapNode_str_ast_Block* next; };
typedef struct core_map_MapNode_str_ast_Block core_map_MapNode_str_ast_Block;
struct core_map_Map_str_ast_Block { core_map_MapNode_str_ast_Block** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Block core_map_Map_str_ast_Block;
__attribute__((hot)) ast_Block** core_alloc_alloc_ast_Block(long long count);
__attribute__((hot)) ast_Block** core_alloc_resize_ast_Block(ast_Block** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Block(ast_Block** ptr);
__attribute__((hot)) ast_Block*** core_alloc_alloc_ast_Block_ptr(long long count);
__attribute__((hot)) ast_Block*** core_alloc_resize_ast_Block_ptr(ast_Block*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Block_ptr(ast_Block*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Block** core_alloc_alloc_core_map_MapNode_str_ast_Block(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Block(core_map_MapNode_str_ast_Block** ptr);

typedef ElifClause ast_ElifClause;
struct core_vec_Vec_ast_ElifClause { ast_ElifClause** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ElifClause core_vec_Vec_ast_ElifClause;
struct core_vec_Vec_ast_ElifClause_ptr { ast_ElifClause*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ElifClause_ptr core_vec_Vec_ast_ElifClause_ptr;
struct core_map_MapNode_str_ast_ElifClause { char* key; ast_ElifClause* value; struct core_map_MapNode_str_ast_ElifClause* next; };
typedef struct core_map_MapNode_str_ast_ElifClause core_map_MapNode_str_ast_ElifClause;
struct core_map_Map_str_ast_ElifClause { core_map_MapNode_str_ast_ElifClause** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_ElifClause core_map_Map_str_ast_ElifClause;
__attribute__((hot)) ast_ElifClause** core_alloc_alloc_ast_ElifClause(long long count);
__attribute__((hot)) ast_ElifClause** core_alloc_resize_ast_ElifClause(ast_ElifClause** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ElifClause(ast_ElifClause** ptr);
__attribute__((hot)) ast_ElifClause*** core_alloc_alloc_ast_ElifClause_ptr(long long count);
__attribute__((hot)) ast_ElifClause*** core_alloc_resize_ast_ElifClause_ptr(ast_ElifClause*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ElifClause_ptr(ast_ElifClause*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_ElifClause** core_alloc_alloc_core_map_MapNode_str_ast_ElifClause(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_ElifClause(core_map_MapNode_str_ast_ElifClause** ptr);

typedef Param ast_Param;
struct core_vec_Vec_ast_Param { ast_Param** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Param core_vec_Vec_ast_Param;
struct core_vec_Vec_ast_Param_ptr { ast_Param*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Param_ptr core_vec_Vec_ast_Param_ptr;
struct core_map_MapNode_str_ast_Param { char* key; ast_Param* value; struct core_map_MapNode_str_ast_Param* next; };
typedef struct core_map_MapNode_str_ast_Param core_map_MapNode_str_ast_Param;
struct core_map_Map_str_ast_Param { core_map_MapNode_str_ast_Param** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Param core_map_Map_str_ast_Param;
__attribute__((hot)) ast_Param** core_alloc_alloc_ast_Param(long long count);
__attribute__((hot)) ast_Param** core_alloc_resize_ast_Param(ast_Param** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Param(ast_Param** ptr);
__attribute__((hot)) ast_Param*** core_alloc_alloc_ast_Param_ptr(long long count);
__attribute__((hot)) ast_Param*** core_alloc_resize_ast_Param_ptr(ast_Param*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Param_ptr(ast_Param*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Param** core_alloc_alloc_core_map_MapNode_str_ast_Param(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Param(core_map_MapNode_str_ast_Param** ptr);

typedef FunctionDef ast_FunctionDef;
struct core_vec_Vec_ast_FunctionDef { ast_FunctionDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FunctionDef core_vec_Vec_ast_FunctionDef;
struct core_vec_Vec_ast_FunctionDef_ptr { ast_FunctionDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FunctionDef_ptr core_vec_Vec_ast_FunctionDef_ptr;
struct core_map_MapNode_str_ast_FunctionDef { char* key; ast_FunctionDef* value; struct core_map_MapNode_str_ast_FunctionDef* next; };
typedef struct core_map_MapNode_str_ast_FunctionDef core_map_MapNode_str_ast_FunctionDef;
struct core_map_Map_str_ast_FunctionDef { core_map_MapNode_str_ast_FunctionDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_FunctionDef core_map_Map_str_ast_FunctionDef;
__attribute__((hot)) ast_FunctionDef** core_alloc_alloc_ast_FunctionDef(long long count);
__attribute__((hot)) ast_FunctionDef** core_alloc_resize_ast_FunctionDef(ast_FunctionDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FunctionDef(ast_FunctionDef** ptr);
__attribute__((hot)) ast_FunctionDef*** core_alloc_alloc_ast_FunctionDef_ptr(long long count);
__attribute__((hot)) ast_FunctionDef*** core_alloc_resize_ast_FunctionDef_ptr(ast_FunctionDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FunctionDef_ptr(ast_FunctionDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_FunctionDef** core_alloc_alloc_core_map_MapNode_str_ast_FunctionDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_FunctionDef(core_map_MapNode_str_ast_FunctionDef** ptr);

typedef FieldDef ast_FieldDef;
struct core_vec_Vec_ast_FieldDef { ast_FieldDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FieldDef core_vec_Vec_ast_FieldDef;
struct core_vec_Vec_ast_FieldDef_ptr { ast_FieldDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FieldDef_ptr core_vec_Vec_ast_FieldDef_ptr;
struct core_map_MapNode_str_ast_FieldDef { char* key; ast_FieldDef* value; struct core_map_MapNode_str_ast_FieldDef* next; };
typedef struct core_map_MapNode_str_ast_FieldDef core_map_MapNode_str_ast_FieldDef;
struct core_map_Map_str_ast_FieldDef { core_map_MapNode_str_ast_FieldDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_FieldDef core_map_Map_str_ast_FieldDef;
__attribute__((hot)) ast_FieldDef** core_alloc_alloc_ast_FieldDef(long long count);
__attribute__((hot)) ast_FieldDef** core_alloc_resize_ast_FieldDef(ast_FieldDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FieldDef(ast_FieldDef** ptr);
__attribute__((hot)) ast_FieldDef*** core_alloc_alloc_ast_FieldDef_ptr(long long count);
__attribute__((hot)) ast_FieldDef*** core_alloc_resize_ast_FieldDef_ptr(ast_FieldDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FieldDef_ptr(ast_FieldDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_FieldDef** core_alloc_alloc_core_map_MapNode_str_ast_FieldDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_FieldDef(core_map_MapNode_str_ast_FieldDef** ptr);

typedef ClassDef ast_ClassDef;
struct core_vec_Vec_ast_ClassDef { ast_ClassDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ClassDef core_vec_Vec_ast_ClassDef;
struct core_vec_Vec_ast_ClassDef_ptr { ast_ClassDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ClassDef_ptr core_vec_Vec_ast_ClassDef_ptr;
struct core_map_MapNode_str_ast_ClassDef { char* key; ast_ClassDef* value; struct core_map_MapNode_str_ast_ClassDef* next; };
typedef struct core_map_MapNode_str_ast_ClassDef core_map_MapNode_str_ast_ClassDef;
struct core_map_Map_str_ast_ClassDef { core_map_MapNode_str_ast_ClassDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_ClassDef core_map_Map_str_ast_ClassDef;
__attribute__((hot)) ast_ClassDef** core_alloc_alloc_ast_ClassDef(long long count);
__attribute__((hot)) ast_ClassDef** core_alloc_resize_ast_ClassDef(ast_ClassDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ClassDef(ast_ClassDef** ptr);
__attribute__((hot)) ast_ClassDef*** core_alloc_alloc_ast_ClassDef_ptr(long long count);
__attribute__((hot)) ast_ClassDef*** core_alloc_resize_ast_ClassDef_ptr(ast_ClassDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ClassDef_ptr(ast_ClassDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_ClassDef** core_alloc_alloc_core_map_MapNode_str_ast_ClassDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_ClassDef(core_map_MapNode_str_ast_ClassDef** ptr);

typedef VariantDef ast_VariantDef;
struct core_vec_Vec_ast_VariantDef { ast_VariantDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_VariantDef core_vec_Vec_ast_VariantDef;
struct core_vec_Vec_ast_VariantDef_ptr { ast_VariantDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_VariantDef_ptr core_vec_Vec_ast_VariantDef_ptr;
struct core_map_MapNode_str_ast_VariantDef { char* key; ast_VariantDef* value; struct core_map_MapNode_str_ast_VariantDef* next; };
typedef struct core_map_MapNode_str_ast_VariantDef core_map_MapNode_str_ast_VariantDef;
struct core_map_Map_str_ast_VariantDef { core_map_MapNode_str_ast_VariantDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_VariantDef core_map_Map_str_ast_VariantDef;
__attribute__((hot)) ast_VariantDef** core_alloc_alloc_ast_VariantDef(long long count);
__attribute__((hot)) ast_VariantDef** core_alloc_resize_ast_VariantDef(ast_VariantDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_VariantDef(ast_VariantDef** ptr);
__attribute__((hot)) ast_VariantDef*** core_alloc_alloc_ast_VariantDef_ptr(long long count);
__attribute__((hot)) ast_VariantDef*** core_alloc_resize_ast_VariantDef_ptr(ast_VariantDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_VariantDef_ptr(ast_VariantDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_VariantDef** core_alloc_alloc_core_map_MapNode_str_ast_VariantDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_VariantDef(core_map_MapNode_str_ast_VariantDef** ptr);

typedef EnumDef ast_EnumDef;
struct core_vec_Vec_ast_EnumDef { ast_EnumDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_EnumDef core_vec_Vec_ast_EnumDef;
struct core_vec_Vec_ast_EnumDef_ptr { ast_EnumDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_EnumDef_ptr core_vec_Vec_ast_EnumDef_ptr;
struct core_map_MapNode_str_ast_EnumDef { char* key; ast_EnumDef* value; struct core_map_MapNode_str_ast_EnumDef* next; };
typedef struct core_map_MapNode_str_ast_EnumDef core_map_MapNode_str_ast_EnumDef;
struct core_map_Map_str_ast_EnumDef { core_map_MapNode_str_ast_EnumDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_EnumDef core_map_Map_str_ast_EnumDef;
__attribute__((hot)) ast_EnumDef** core_alloc_alloc_ast_EnumDef(long long count);
__attribute__((hot)) ast_EnumDef** core_alloc_resize_ast_EnumDef(ast_EnumDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_EnumDef(ast_EnumDef** ptr);
__attribute__((hot)) ast_EnumDef*** core_alloc_alloc_ast_EnumDef_ptr(long long count);
__attribute__((hot)) ast_EnumDef*** core_alloc_resize_ast_EnumDef_ptr(ast_EnumDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_EnumDef_ptr(ast_EnumDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_EnumDef** core_alloc_alloc_core_map_MapNode_str_ast_EnumDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_EnumDef(core_map_MapNode_str_ast_EnumDef** ptr);

typedef InterfaceDef ast_InterfaceDef;
struct core_vec_Vec_ast_InterfaceDef { ast_InterfaceDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_InterfaceDef core_vec_Vec_ast_InterfaceDef;
struct core_vec_Vec_ast_InterfaceDef_ptr { ast_InterfaceDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_InterfaceDef_ptr core_vec_Vec_ast_InterfaceDef_ptr;
struct core_map_MapNode_str_ast_InterfaceDef { char* key; ast_InterfaceDef* value; struct core_map_MapNode_str_ast_InterfaceDef* next; };
typedef struct core_map_MapNode_str_ast_InterfaceDef core_map_MapNode_str_ast_InterfaceDef;
struct core_map_Map_str_ast_InterfaceDef { core_map_MapNode_str_ast_InterfaceDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_InterfaceDef core_map_Map_str_ast_InterfaceDef;
__attribute__((hot)) ast_InterfaceDef** core_alloc_alloc_ast_InterfaceDef(long long count);
__attribute__((hot)) ast_InterfaceDef** core_alloc_resize_ast_InterfaceDef(ast_InterfaceDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_InterfaceDef(ast_InterfaceDef** ptr);
__attribute__((hot)) ast_InterfaceDef*** core_alloc_alloc_ast_InterfaceDef_ptr(long long count);
__attribute__((hot)) ast_InterfaceDef*** core_alloc_resize_ast_InterfaceDef_ptr(ast_InterfaceDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_InterfaceDef_ptr(ast_InterfaceDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_InterfaceDef** core_alloc_alloc_core_map_MapNode_str_ast_InterfaceDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_InterfaceDef(core_map_MapNode_str_ast_InterfaceDef** ptr);

typedef ImportItem ast_ImportItem;
struct core_vec_Vec_ast_ImportItem { ast_ImportItem** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ImportItem core_vec_Vec_ast_ImportItem;
struct core_vec_Vec_ast_ImportItem_ptr { ast_ImportItem*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ImportItem_ptr core_vec_Vec_ast_ImportItem_ptr;
struct core_map_MapNode_str_ast_ImportItem { char* key; ast_ImportItem* value; struct core_map_MapNode_str_ast_ImportItem* next; };
typedef struct core_map_MapNode_str_ast_ImportItem core_map_MapNode_str_ast_ImportItem;
struct core_map_Map_str_ast_ImportItem { core_map_MapNode_str_ast_ImportItem** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_ImportItem core_map_Map_str_ast_ImportItem;
__attribute__((hot)) ast_ImportItem** core_alloc_alloc_ast_ImportItem(long long count);
__attribute__((hot)) ast_ImportItem** core_alloc_resize_ast_ImportItem(ast_ImportItem** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ImportItem(ast_ImportItem** ptr);
__attribute__((hot)) ast_ImportItem*** core_alloc_alloc_ast_ImportItem_ptr(long long count);
__attribute__((hot)) ast_ImportItem*** core_alloc_resize_ast_ImportItem_ptr(ast_ImportItem*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ImportItem_ptr(ast_ImportItem*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_ImportItem** core_alloc_alloc_core_map_MapNode_str_ast_ImportItem(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_ImportItem(core_map_MapNode_str_ast_ImportItem** ptr);

typedef Decl ast_Decl;
struct core_vec_Vec_ast_Decl { ast_Decl* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Decl core_vec_Vec_ast_Decl;
struct core_vec_Vec_ast_Decl_ptr { ast_Decl** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Decl_ptr core_vec_Vec_ast_Decl_ptr;
__attribute__((hot)) ast_Decl* core_alloc_alloc_ast_Decl(long long count);
__attribute__((hot)) ast_Decl* core_alloc_resize_ast_Decl(ast_Decl* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Decl(ast_Decl* ptr);
__attribute__((hot)) ast_Decl** core_alloc_alloc_ast_Decl_ptr(long long count);
__attribute__((hot)) ast_Decl** core_alloc_resize_ast_Decl_ptr(ast_Decl** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Decl_ptr(ast_Decl** ptr);

typedef Program ast_Program;
struct core_vec_Vec_ast_Program { ast_Program** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Program core_vec_Vec_ast_Program;
struct core_vec_Vec_ast_Program_ptr { ast_Program*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Program_ptr core_vec_Vec_ast_Program_ptr;
struct core_map_MapNode_str_ast_Program { char* key; ast_Program* value; struct core_map_MapNode_str_ast_Program* next; };
typedef struct core_map_MapNode_str_ast_Program core_map_MapNode_str_ast_Program;
struct core_map_Map_str_ast_Program { core_map_MapNode_str_ast_Program** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Program core_map_Map_str_ast_Program;
__attribute__((hot)) ast_Program** core_alloc_alloc_ast_Program(long long count);
__attribute__((hot)) ast_Program** core_alloc_resize_ast_Program(ast_Program** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Program(ast_Program** ptr);
__attribute__((hot)) ast_Program*** core_alloc_alloc_ast_Program_ptr(long long count);
__attribute__((hot)) ast_Program*** core_alloc_resize_ast_Program_ptr(ast_Program*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Program_ptr(ast_Program*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Program** core_alloc_alloc_core_map_MapNode_str_ast_Program(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Program(core_map_MapNode_str_ast_Program** ptr);

typedef Parser parser_Parser;
struct core_vec_Vec_parser_Parser { parser_Parser** data; long long len; long long capacity; };
typedef struct core_vec_Vec_parser_Parser core_vec_Vec_parser_Parser;
struct core_vec_Vec_parser_Parser_ptr { parser_Parser*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_parser_Parser_ptr core_vec_Vec_parser_Parser_ptr;
struct core_map_MapNode_str_parser_Parser { char* key; parser_Parser* value; struct core_map_MapNode_str_parser_Parser* next; };
typedef struct core_map_MapNode_str_parser_Parser core_map_MapNode_str_parser_Parser;
struct core_map_Map_str_parser_Parser { core_map_MapNode_str_parser_Parser** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_parser_Parser core_map_Map_str_parser_Parser;
__attribute__((hot)) parser_Parser** core_alloc_alloc_parser_Parser(long long count);
__attribute__((hot)) parser_Parser** core_alloc_resize_parser_Parser(parser_Parser** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_parser_Parser(parser_Parser** ptr);
__attribute__((hot)) parser_Parser*** core_alloc_alloc_parser_Parser_ptr(long long count);
__attribute__((hot)) parser_Parser*** core_alloc_resize_parser_Parser_ptr(parser_Parser*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_parser_Parser_ptr(parser_Parser*** ptr);
__attribute__((hot)) core_map_MapNode_str_parser_Parser** core_alloc_alloc_core_map_MapNode_str_parser_Parser(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_parser_Parser(core_map_MapNode_str_parser_Parser** ptr);

typedef ModuleResolver resolver_ModuleResolver;
struct core_vec_Vec_resolver_ModuleResolver { resolver_ModuleResolver** data; long long len; long long capacity; };
typedef struct core_vec_Vec_resolver_ModuleResolver core_vec_Vec_resolver_ModuleResolver;
struct core_vec_Vec_resolver_ModuleResolver_ptr { resolver_ModuleResolver*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_resolver_ModuleResolver_ptr core_vec_Vec_resolver_ModuleResolver_ptr;
struct core_map_MapNode_str_resolver_ModuleResolver { char* key; resolver_ModuleResolver* value; struct core_map_MapNode_str_resolver_ModuleResolver* next; };
typedef struct core_map_MapNode_str_resolver_ModuleResolver core_map_MapNode_str_resolver_ModuleResolver;
struct core_map_Map_str_resolver_ModuleResolver { core_map_MapNode_str_resolver_ModuleResolver** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_resolver_ModuleResolver core_map_Map_str_resolver_ModuleResolver;
__attribute__((hot)) resolver_ModuleResolver** core_alloc_alloc_resolver_ModuleResolver(long long count);
__attribute__((hot)) resolver_ModuleResolver** core_alloc_resize_resolver_ModuleResolver(resolver_ModuleResolver** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_resolver_ModuleResolver(resolver_ModuleResolver** ptr);
__attribute__((hot)) resolver_ModuleResolver*** core_alloc_alloc_resolver_ModuleResolver_ptr(long long count);
__attribute__((hot)) resolver_ModuleResolver*** core_alloc_resize_resolver_ModuleResolver_ptr(resolver_ModuleResolver*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_resolver_ModuleResolver_ptr(resolver_ModuleResolver*** ptr);
__attribute__((hot)) core_map_MapNode_str_resolver_ModuleResolver** core_alloc_alloc_core_map_MapNode_str_resolver_ModuleResolver(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_resolver_ModuleResolver(core_map_MapNode_str_resolver_ModuleResolver** ptr);

typedef HirExpr hir_HirExpr;
struct core_vec_Vec_hir_HirExpr { hir_HirExpr* data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirExpr core_vec_Vec_hir_HirExpr;
struct core_vec_Vec_hir_HirExpr_ptr { hir_HirExpr** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirExpr_ptr core_vec_Vec_hir_HirExpr_ptr;
__attribute__((hot)) hir_HirExpr* core_alloc_alloc_hir_HirExpr(long long count);
__attribute__((hot)) hir_HirExpr* core_alloc_resize_hir_HirExpr(hir_HirExpr* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirExpr(hir_HirExpr* ptr);
__attribute__((hot)) hir_HirExpr** core_alloc_alloc_hir_HirExpr_ptr(long long count);
__attribute__((hot)) hir_HirExpr** core_alloc_resize_hir_HirExpr_ptr(hir_HirExpr** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirExpr_ptr(hir_HirExpr** ptr);

typedef HirComprehension hir_HirComprehension;
struct core_vec_Vec_hir_HirComprehension { hir_HirComprehension** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirComprehension core_vec_Vec_hir_HirComprehension;
struct core_vec_Vec_hir_HirComprehension_ptr { hir_HirComprehension*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirComprehension_ptr core_vec_Vec_hir_HirComprehension_ptr;
struct core_map_MapNode_str_hir_HirComprehension { char* key; hir_HirComprehension* value; struct core_map_MapNode_str_hir_HirComprehension* next; };
typedef struct core_map_MapNode_str_hir_HirComprehension core_map_MapNode_str_hir_HirComprehension;
struct core_map_Map_str_hir_HirComprehension { core_map_MapNode_str_hir_HirComprehension** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirComprehension core_map_Map_str_hir_HirComprehension;
__attribute__((hot)) hir_HirComprehension** core_alloc_alloc_hir_HirComprehension(long long count);
__attribute__((hot)) hir_HirComprehension** core_alloc_resize_hir_HirComprehension(hir_HirComprehension** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirComprehension(hir_HirComprehension** ptr);
__attribute__((hot)) hir_HirComprehension*** core_alloc_alloc_hir_HirComprehension_ptr(long long count);
__attribute__((hot)) hir_HirComprehension*** core_alloc_resize_hir_HirComprehension_ptr(hir_HirComprehension*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirComprehension_ptr(hir_HirComprehension*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirComprehension** core_alloc_alloc_core_map_MapNode_str_hir_HirComprehension(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirComprehension(core_map_MapNode_str_hir_HirComprehension** ptr);

typedef HirCatchClause hir_HirCatchClause;
struct core_vec_Vec_hir_HirCatchClause { hir_HirCatchClause** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirCatchClause core_vec_Vec_hir_HirCatchClause;
struct core_vec_Vec_hir_HirCatchClause_ptr { hir_HirCatchClause*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirCatchClause_ptr core_vec_Vec_hir_HirCatchClause_ptr;
struct core_map_MapNode_str_hir_HirCatchClause { char* key; hir_HirCatchClause* value; struct core_map_MapNode_str_hir_HirCatchClause* next; };
typedef struct core_map_MapNode_str_hir_HirCatchClause core_map_MapNode_str_hir_HirCatchClause;
struct core_map_Map_str_hir_HirCatchClause { core_map_MapNode_str_hir_HirCatchClause** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirCatchClause core_map_Map_str_hir_HirCatchClause;
__attribute__((hot)) hir_HirCatchClause** core_alloc_alloc_hir_HirCatchClause(long long count);
__attribute__((hot)) hir_HirCatchClause** core_alloc_resize_hir_HirCatchClause(hir_HirCatchClause** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirCatchClause(hir_HirCatchClause** ptr);
__attribute__((hot)) hir_HirCatchClause*** core_alloc_alloc_hir_HirCatchClause_ptr(long long count);
__attribute__((hot)) hir_HirCatchClause*** core_alloc_resize_hir_HirCatchClause_ptr(hir_HirCatchClause*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirCatchClause_ptr(hir_HirCatchClause*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirCatchClause** core_alloc_alloc_core_map_MapNode_str_hir_HirCatchClause(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirCatchClause(core_map_MapNode_str_hir_HirCatchClause** ptr);

typedef HirFStringPart hir_HirFStringPart;
struct core_vec_Vec_hir_HirFStringPart { hir_HirFStringPart** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirFStringPart core_vec_Vec_hir_HirFStringPart;
struct core_vec_Vec_hir_HirFStringPart_ptr { hir_HirFStringPart*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirFStringPart_ptr core_vec_Vec_hir_HirFStringPart_ptr;
struct core_map_MapNode_str_hir_HirFStringPart { char* key; hir_HirFStringPart* value; struct core_map_MapNode_str_hir_HirFStringPart* next; };
typedef struct core_map_MapNode_str_hir_HirFStringPart core_map_MapNode_str_hir_HirFStringPart;
struct core_map_Map_str_hir_HirFStringPart { core_map_MapNode_str_hir_HirFStringPart** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirFStringPart core_map_Map_str_hir_HirFStringPart;
__attribute__((hot)) hir_HirFStringPart** core_alloc_alloc_hir_HirFStringPart(long long count);
__attribute__((hot)) hir_HirFStringPart** core_alloc_resize_hir_HirFStringPart(hir_HirFStringPart** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirFStringPart(hir_HirFStringPart** ptr);
__attribute__((hot)) hir_HirFStringPart*** core_alloc_alloc_hir_HirFStringPart_ptr(long long count);
__attribute__((hot)) hir_HirFStringPart*** core_alloc_resize_hir_HirFStringPart_ptr(hir_HirFStringPart*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirFStringPart_ptr(hir_HirFStringPart*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirFStringPart** core_alloc_alloc_core_map_MapNode_str_hir_HirFStringPart(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirFStringPart(core_map_MapNode_str_hir_HirFStringPart** ptr);

typedef HirMatchArm hir_HirMatchArm;
struct core_vec_Vec_hir_HirMatchArm { hir_HirMatchArm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirMatchArm core_vec_Vec_hir_HirMatchArm;
struct core_vec_Vec_hir_HirMatchArm_ptr { hir_HirMatchArm*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirMatchArm_ptr core_vec_Vec_hir_HirMatchArm_ptr;
struct core_map_MapNode_str_hir_HirMatchArm { char* key; hir_HirMatchArm* value; struct core_map_MapNode_str_hir_HirMatchArm* next; };
typedef struct core_map_MapNode_str_hir_HirMatchArm core_map_MapNode_str_hir_HirMatchArm;
struct core_map_Map_str_hir_HirMatchArm { core_map_MapNode_str_hir_HirMatchArm** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirMatchArm core_map_Map_str_hir_HirMatchArm;
__attribute__((hot)) hir_HirMatchArm** core_alloc_alloc_hir_HirMatchArm(long long count);
__attribute__((hot)) hir_HirMatchArm** core_alloc_resize_hir_HirMatchArm(hir_HirMatchArm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirMatchArm(hir_HirMatchArm** ptr);
__attribute__((hot)) hir_HirMatchArm*** core_alloc_alloc_hir_HirMatchArm_ptr(long long count);
__attribute__((hot)) hir_HirMatchArm*** core_alloc_resize_hir_HirMatchArm_ptr(hir_HirMatchArm*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirMatchArm_ptr(hir_HirMatchArm*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirMatchArm** core_alloc_alloc_core_map_MapNode_str_hir_HirMatchArm(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirMatchArm(core_map_MapNode_str_hir_HirMatchArm** ptr);

typedef HirChanSelectArm hir_HirChanSelectArm;
struct core_vec_Vec_hir_HirChanSelectArm { hir_HirChanSelectArm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirChanSelectArm core_vec_Vec_hir_HirChanSelectArm;
struct core_vec_Vec_hir_HirChanSelectArm_ptr { hir_HirChanSelectArm*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirChanSelectArm_ptr core_vec_Vec_hir_HirChanSelectArm_ptr;
struct core_map_MapNode_str_hir_HirChanSelectArm { char* key; hir_HirChanSelectArm* value; struct core_map_MapNode_str_hir_HirChanSelectArm* next; };
typedef struct core_map_MapNode_str_hir_HirChanSelectArm core_map_MapNode_str_hir_HirChanSelectArm;
struct core_map_Map_str_hir_HirChanSelectArm { core_map_MapNode_str_hir_HirChanSelectArm** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirChanSelectArm core_map_Map_str_hir_HirChanSelectArm;
__attribute__((hot)) hir_HirChanSelectArm** core_alloc_alloc_hir_HirChanSelectArm(long long count);
__attribute__((hot)) hir_HirChanSelectArm** core_alloc_resize_hir_HirChanSelectArm(hir_HirChanSelectArm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirChanSelectArm(hir_HirChanSelectArm** ptr);
__attribute__((hot)) hir_HirChanSelectArm*** core_alloc_alloc_hir_HirChanSelectArm_ptr(long long count);
__attribute__((hot)) hir_HirChanSelectArm*** core_alloc_resize_hir_HirChanSelectArm_ptr(hir_HirChanSelectArm*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirChanSelectArm_ptr(hir_HirChanSelectArm*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirChanSelectArm** core_alloc_alloc_core_map_MapNode_str_hir_HirChanSelectArm(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirChanSelectArm(core_map_MapNode_str_hir_HirChanSelectArm** ptr);

typedef HirStmt hir_HirStmt;
struct core_vec_Vec_hir_HirStmt { hir_HirStmt* data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirStmt core_vec_Vec_hir_HirStmt;
struct core_vec_Vec_hir_HirStmt_ptr { hir_HirStmt** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirStmt_ptr core_vec_Vec_hir_HirStmt_ptr;
__attribute__((hot)) hir_HirStmt* core_alloc_alloc_hir_HirStmt(long long count);
__attribute__((hot)) hir_HirStmt* core_alloc_resize_hir_HirStmt(hir_HirStmt* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirStmt(hir_HirStmt* ptr);
__attribute__((hot)) hir_HirStmt** core_alloc_alloc_hir_HirStmt_ptr(long long count);
__attribute__((hot)) hir_HirStmt** core_alloc_resize_hir_HirStmt_ptr(hir_HirStmt** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirStmt_ptr(hir_HirStmt** ptr);

typedef HirBlock hir_HirBlock;
struct core_vec_Vec_hir_HirBlock { hir_HirBlock** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirBlock core_vec_Vec_hir_HirBlock;
struct core_vec_Vec_hir_HirBlock_ptr { hir_HirBlock*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirBlock_ptr core_vec_Vec_hir_HirBlock_ptr;
struct core_map_MapNode_str_hir_HirBlock { char* key; hir_HirBlock* value; struct core_map_MapNode_str_hir_HirBlock* next; };
typedef struct core_map_MapNode_str_hir_HirBlock core_map_MapNode_str_hir_HirBlock;
struct core_map_Map_str_hir_HirBlock { core_map_MapNode_str_hir_HirBlock** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirBlock core_map_Map_str_hir_HirBlock;
__attribute__((hot)) hir_HirBlock** core_alloc_alloc_hir_HirBlock(long long count);
__attribute__((hot)) hir_HirBlock** core_alloc_resize_hir_HirBlock(hir_HirBlock** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirBlock(hir_HirBlock** ptr);
__attribute__((hot)) hir_HirBlock*** core_alloc_alloc_hir_HirBlock_ptr(long long count);
__attribute__((hot)) hir_HirBlock*** core_alloc_resize_hir_HirBlock_ptr(hir_HirBlock*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirBlock_ptr(hir_HirBlock*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirBlock** core_alloc_alloc_core_map_MapNode_str_hir_HirBlock(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirBlock(core_map_MapNode_str_hir_HirBlock** ptr);

typedef HirParam hir_HirParam;
struct core_vec_Vec_hir_HirParam { hir_HirParam** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirParam core_vec_Vec_hir_HirParam;
struct core_vec_Vec_hir_HirParam_ptr { hir_HirParam*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirParam_ptr core_vec_Vec_hir_HirParam_ptr;
struct core_map_MapNode_str_hir_HirParam { char* key; hir_HirParam* value; struct core_map_MapNode_str_hir_HirParam* next; };
typedef struct core_map_MapNode_str_hir_HirParam core_map_MapNode_str_hir_HirParam;
struct core_map_Map_str_hir_HirParam { core_map_MapNode_str_hir_HirParam** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirParam core_map_Map_str_hir_HirParam;
__attribute__((hot)) hir_HirParam** core_alloc_alloc_hir_HirParam(long long count);
__attribute__((hot)) hir_HirParam** core_alloc_resize_hir_HirParam(hir_HirParam** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirParam(hir_HirParam** ptr);
__attribute__((hot)) hir_HirParam*** core_alloc_alloc_hir_HirParam_ptr(long long count);
__attribute__((hot)) hir_HirParam*** core_alloc_resize_hir_HirParam_ptr(hir_HirParam*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirParam_ptr(hir_HirParam*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirParam** core_alloc_alloc_core_map_MapNode_str_hir_HirParam(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirParam(core_map_MapNode_str_hir_HirParam** ptr);

typedef HirFunction hir_HirFunction;
struct core_vec_Vec_hir_HirFunction { hir_HirFunction** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirFunction core_vec_Vec_hir_HirFunction;
struct core_vec_Vec_hir_HirFunction_ptr { hir_HirFunction*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirFunction_ptr core_vec_Vec_hir_HirFunction_ptr;
struct core_map_MapNode_str_hir_HirFunction { char* key; hir_HirFunction* value; struct core_map_MapNode_str_hir_HirFunction* next; };
typedef struct core_map_MapNode_str_hir_HirFunction core_map_MapNode_str_hir_HirFunction;
struct core_map_Map_str_hir_HirFunction { core_map_MapNode_str_hir_HirFunction** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirFunction core_map_Map_str_hir_HirFunction;
__attribute__((hot)) hir_HirFunction** core_alloc_alloc_hir_HirFunction(long long count);
__attribute__((hot)) hir_HirFunction** core_alloc_resize_hir_HirFunction(hir_HirFunction** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirFunction(hir_HirFunction** ptr);
__attribute__((hot)) hir_HirFunction*** core_alloc_alloc_hir_HirFunction_ptr(long long count);
__attribute__((hot)) hir_HirFunction*** core_alloc_resize_hir_HirFunction_ptr(hir_HirFunction*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirFunction_ptr(hir_HirFunction*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirFunction** core_alloc_alloc_core_map_MapNode_str_hir_HirFunction(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirFunction(core_map_MapNode_str_hir_HirFunction** ptr);

typedef HirField hir_HirField;
struct core_vec_Vec_hir_HirField { hir_HirField** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirField core_vec_Vec_hir_HirField;
struct core_vec_Vec_hir_HirField_ptr { hir_HirField*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirField_ptr core_vec_Vec_hir_HirField_ptr;
struct core_map_MapNode_str_hir_HirField { char* key; hir_HirField* value; struct core_map_MapNode_str_hir_HirField* next; };
typedef struct core_map_MapNode_str_hir_HirField core_map_MapNode_str_hir_HirField;
struct core_map_Map_str_hir_HirField { core_map_MapNode_str_hir_HirField** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirField core_map_Map_str_hir_HirField;
__attribute__((hot)) hir_HirField** core_alloc_alloc_hir_HirField(long long count);
__attribute__((hot)) hir_HirField** core_alloc_resize_hir_HirField(hir_HirField** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirField(hir_HirField** ptr);
__attribute__((hot)) hir_HirField*** core_alloc_alloc_hir_HirField_ptr(long long count);
__attribute__((hot)) hir_HirField*** core_alloc_resize_hir_HirField_ptr(hir_HirField*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirField_ptr(hir_HirField*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirField** core_alloc_alloc_core_map_MapNode_str_hir_HirField(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirField(core_map_MapNode_str_hir_HirField** ptr);

typedef HirClass hir_HirClass;
struct core_vec_Vec_hir_HirClass { hir_HirClass** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirClass core_vec_Vec_hir_HirClass;
struct core_vec_Vec_hir_HirClass_ptr { hir_HirClass*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirClass_ptr core_vec_Vec_hir_HirClass_ptr;
struct core_map_MapNode_str_hir_HirClass { char* key; hir_HirClass* value; struct core_map_MapNode_str_hir_HirClass* next; };
typedef struct core_map_MapNode_str_hir_HirClass core_map_MapNode_str_hir_HirClass;
struct core_map_Map_str_hir_HirClass { core_map_MapNode_str_hir_HirClass** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirClass core_map_Map_str_hir_HirClass;
__attribute__((hot)) hir_HirClass** core_alloc_alloc_hir_HirClass(long long count);
__attribute__((hot)) hir_HirClass** core_alloc_resize_hir_HirClass(hir_HirClass** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirClass(hir_HirClass** ptr);
__attribute__((hot)) hir_HirClass*** core_alloc_alloc_hir_HirClass_ptr(long long count);
__attribute__((hot)) hir_HirClass*** core_alloc_resize_hir_HirClass_ptr(hir_HirClass*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirClass_ptr(hir_HirClass*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirClass** core_alloc_alloc_core_map_MapNode_str_hir_HirClass(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirClass(core_map_MapNode_str_hir_HirClass** ptr);

typedef HirVariant hir_HirVariant;
struct core_vec_Vec_hir_HirVariant { hir_HirVariant** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirVariant core_vec_Vec_hir_HirVariant;
struct core_vec_Vec_hir_HirVariant_ptr { hir_HirVariant*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirVariant_ptr core_vec_Vec_hir_HirVariant_ptr;
struct core_map_MapNode_str_hir_HirVariant { char* key; hir_HirVariant* value; struct core_map_MapNode_str_hir_HirVariant* next; };
typedef struct core_map_MapNode_str_hir_HirVariant core_map_MapNode_str_hir_HirVariant;
struct core_map_Map_str_hir_HirVariant { core_map_MapNode_str_hir_HirVariant** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirVariant core_map_Map_str_hir_HirVariant;
__attribute__((hot)) hir_HirVariant** core_alloc_alloc_hir_HirVariant(long long count);
__attribute__((hot)) hir_HirVariant** core_alloc_resize_hir_HirVariant(hir_HirVariant** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirVariant(hir_HirVariant** ptr);
__attribute__((hot)) hir_HirVariant*** core_alloc_alloc_hir_HirVariant_ptr(long long count);
__attribute__((hot)) hir_HirVariant*** core_alloc_resize_hir_HirVariant_ptr(hir_HirVariant*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirVariant_ptr(hir_HirVariant*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirVariant** core_alloc_alloc_core_map_MapNode_str_hir_HirVariant(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirVariant(core_map_MapNode_str_hir_HirVariant** ptr);

typedef HirEnum hir_HirEnum;
struct core_vec_Vec_hir_HirEnum { hir_HirEnum** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirEnum core_vec_Vec_hir_HirEnum;
struct core_vec_Vec_hir_HirEnum_ptr { hir_HirEnum*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirEnum_ptr core_vec_Vec_hir_HirEnum_ptr;
struct core_map_MapNode_str_hir_HirEnum { char* key; hir_HirEnum* value; struct core_map_MapNode_str_hir_HirEnum* next; };
typedef struct core_map_MapNode_str_hir_HirEnum core_map_MapNode_str_hir_HirEnum;
struct core_map_Map_str_hir_HirEnum { core_map_MapNode_str_hir_HirEnum** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirEnum core_map_Map_str_hir_HirEnum;
__attribute__((hot)) hir_HirEnum** core_alloc_alloc_hir_HirEnum(long long count);
__attribute__((hot)) hir_HirEnum** core_alloc_resize_hir_HirEnum(hir_HirEnum** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirEnum(hir_HirEnum** ptr);
__attribute__((hot)) hir_HirEnum*** core_alloc_alloc_hir_HirEnum_ptr(long long count);
__attribute__((hot)) hir_HirEnum*** core_alloc_resize_hir_HirEnum_ptr(hir_HirEnum*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirEnum_ptr(hir_HirEnum*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirEnum** core_alloc_alloc_core_map_MapNode_str_hir_HirEnum(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirEnum(core_map_MapNode_str_hir_HirEnum** ptr);

typedef HirInterface hir_HirInterface;
struct core_vec_Vec_hir_HirInterface { hir_HirInterface** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirInterface core_vec_Vec_hir_HirInterface;
struct core_vec_Vec_hir_HirInterface_ptr { hir_HirInterface*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirInterface_ptr core_vec_Vec_hir_HirInterface_ptr;
struct core_map_MapNode_str_hir_HirInterface { char* key; hir_HirInterface* value; struct core_map_MapNode_str_hir_HirInterface* next; };
typedef struct core_map_MapNode_str_hir_HirInterface core_map_MapNode_str_hir_HirInterface;
struct core_map_Map_str_hir_HirInterface { core_map_MapNode_str_hir_HirInterface** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirInterface core_map_Map_str_hir_HirInterface;
__attribute__((hot)) hir_HirInterface** core_alloc_alloc_hir_HirInterface(long long count);
__attribute__((hot)) hir_HirInterface** core_alloc_resize_hir_HirInterface(hir_HirInterface** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirInterface(hir_HirInterface** ptr);
__attribute__((hot)) hir_HirInterface*** core_alloc_alloc_hir_HirInterface_ptr(long long count);
__attribute__((hot)) hir_HirInterface*** core_alloc_resize_hir_HirInterface_ptr(hir_HirInterface*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirInterface_ptr(hir_HirInterface*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirInterface** core_alloc_alloc_core_map_MapNode_str_hir_HirInterface(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirInterface(core_map_MapNode_str_hir_HirInterface** ptr);

typedef HirProgram hir_HirProgram;
struct core_vec_Vec_hir_HirProgram { hir_HirProgram** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirProgram core_vec_Vec_hir_HirProgram;
struct core_vec_Vec_hir_HirProgram_ptr { hir_HirProgram*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirProgram_ptr core_vec_Vec_hir_HirProgram_ptr;
struct core_map_MapNode_str_hir_HirProgram { char* key; hir_HirProgram* value; struct core_map_MapNode_str_hir_HirProgram* next; };
typedef struct core_map_MapNode_str_hir_HirProgram core_map_MapNode_str_hir_HirProgram;
struct core_map_Map_str_hir_HirProgram { core_map_MapNode_str_hir_HirProgram** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirProgram core_map_Map_str_hir_HirProgram;
__attribute__((hot)) hir_HirProgram** core_alloc_alloc_hir_HirProgram(long long count);
__attribute__((hot)) hir_HirProgram** core_alloc_resize_hir_HirProgram(hir_HirProgram** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirProgram(hir_HirProgram** ptr);
__attribute__((hot)) hir_HirProgram*** core_alloc_alloc_hir_HirProgram_ptr(long long count);
__attribute__((hot)) hir_HirProgram*** core_alloc_resize_hir_HirProgram_ptr(hir_HirProgram*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirProgram_ptr(hir_HirProgram*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirProgram** core_alloc_alloc_core_map_MapNode_str_hir_HirProgram(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirProgram(core_map_MapNode_str_hir_HirProgram** ptr);

typedef SymbolKind sema_SymbolKind;
struct core_vec_Vec_sema_SymbolKind { sema_SymbolKind* data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_SymbolKind core_vec_Vec_sema_SymbolKind;
struct core_vec_Vec_sema_SymbolKind_ptr { sema_SymbolKind** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_SymbolKind_ptr core_vec_Vec_sema_SymbolKind_ptr;
__attribute__((hot)) sema_SymbolKind* core_alloc_alloc_sema_SymbolKind(long long count);
__attribute__((hot)) sema_SymbolKind* core_alloc_resize_sema_SymbolKind(sema_SymbolKind* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_SymbolKind(sema_SymbolKind* ptr);
__attribute__((hot)) sema_SymbolKind** core_alloc_alloc_sema_SymbolKind_ptr(long long count);
__attribute__((hot)) sema_SymbolKind** core_alloc_resize_sema_SymbolKind_ptr(sema_SymbolKind** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_SymbolKind_ptr(sema_SymbolKind** ptr);

typedef Symbol sema_Symbol;
struct core_vec_Vec_sema_Symbol { sema_Symbol** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Symbol core_vec_Vec_sema_Symbol;
struct core_vec_Vec_sema_Symbol_ptr { sema_Symbol*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Symbol_ptr core_vec_Vec_sema_Symbol_ptr;
struct core_map_MapNode_str_sema_Symbol { char* key; sema_Symbol* value; struct core_map_MapNode_str_sema_Symbol* next; };
typedef struct core_map_MapNode_str_sema_Symbol core_map_MapNode_str_sema_Symbol;
struct core_map_Map_str_sema_Symbol { core_map_MapNode_str_sema_Symbol** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_sema_Symbol core_map_Map_str_sema_Symbol;
__attribute__((hot)) sema_Symbol** core_alloc_alloc_sema_Symbol(long long count);
__attribute__((hot)) sema_Symbol** core_alloc_resize_sema_Symbol(sema_Symbol** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Symbol(sema_Symbol** ptr);
__attribute__((hot)) sema_Symbol*** core_alloc_alloc_sema_Symbol_ptr(long long count);
__attribute__((hot)) sema_Symbol*** core_alloc_resize_sema_Symbol_ptr(sema_Symbol*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Symbol_ptr(sema_Symbol*** ptr);
__attribute__((hot)) core_map_MapNode_str_sema_Symbol** core_alloc_alloc_core_map_MapNode_str_sema_Symbol(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_sema_Symbol(core_map_MapNode_str_sema_Symbol** ptr);

typedef Scope sema_Scope;
struct core_vec_Vec_sema_Scope { sema_Scope** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Scope core_vec_Vec_sema_Scope;
struct core_vec_Vec_sema_Scope_ptr { sema_Scope*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Scope_ptr core_vec_Vec_sema_Scope_ptr;
struct core_map_MapNode_str_sema_Scope { char* key; sema_Scope* value; struct core_map_MapNode_str_sema_Scope* next; };
typedef struct core_map_MapNode_str_sema_Scope core_map_MapNode_str_sema_Scope;
struct core_map_Map_str_sema_Scope { core_map_MapNode_str_sema_Scope** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_sema_Scope core_map_Map_str_sema_Scope;
__attribute__((hot)) sema_Scope** core_alloc_alloc_sema_Scope(long long count);
__attribute__((hot)) sema_Scope** core_alloc_resize_sema_Scope(sema_Scope** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Scope(sema_Scope** ptr);
__attribute__((hot)) sema_Scope*** core_alloc_alloc_sema_Scope_ptr(long long count);
__attribute__((hot)) sema_Scope*** core_alloc_resize_sema_Scope_ptr(sema_Scope*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Scope_ptr(sema_Scope*** ptr);
__attribute__((hot)) core_map_MapNode_str_sema_Scope** core_alloc_alloc_core_map_MapNode_str_sema_Scope(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_sema_Scope(core_map_MapNode_str_sema_Scope** ptr);

typedef Sema sema_Sema;
struct core_vec_Vec_sema_Sema { sema_Sema** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Sema core_vec_Vec_sema_Sema;
struct core_vec_Vec_sema_Sema_ptr { sema_Sema*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Sema_ptr core_vec_Vec_sema_Sema_ptr;
struct core_map_MapNode_str_sema_Sema { char* key; sema_Sema* value; struct core_map_MapNode_str_sema_Sema* next; };
typedef struct core_map_MapNode_str_sema_Sema core_map_MapNode_str_sema_Sema;
struct core_map_Map_str_sema_Sema { core_map_MapNode_str_sema_Sema** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_sema_Sema core_map_Map_str_sema_Sema;
__attribute__((hot)) sema_Sema** core_alloc_alloc_sema_Sema(long long count);
__attribute__((hot)) sema_Sema** core_alloc_resize_sema_Sema(sema_Sema** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Sema(sema_Sema** ptr);
__attribute__((hot)) sema_Sema*** core_alloc_alloc_sema_Sema_ptr(long long count);
__attribute__((hot)) sema_Sema*** core_alloc_resize_sema_Sema_ptr(sema_Sema*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Sema_ptr(sema_Sema*** ptr);
__attribute__((hot)) core_map_MapNode_str_sema_Sema** core_alloc_alloc_core_map_MapNode_str_sema_Sema(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_sema_Sema(core_map_MapNode_str_sema_Sema** ptr);

typedef CGenerator codegen_c_CGenerator;
struct core_vec_Vec_codegen_c_CGenerator { codegen_c_CGenerator** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_c_CGenerator core_vec_Vec_codegen_c_CGenerator;
struct core_vec_Vec_codegen_c_CGenerator_ptr { codegen_c_CGenerator*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_c_CGenerator_ptr core_vec_Vec_codegen_c_CGenerator_ptr;
struct core_map_MapNode_str_codegen_c_CGenerator { char* key; codegen_c_CGenerator* value; struct core_map_MapNode_str_codegen_c_CGenerator* next; };
typedef struct core_map_MapNode_str_codegen_c_CGenerator core_map_MapNode_str_codegen_c_CGenerator;
struct core_map_Map_str_codegen_c_CGenerator { core_map_MapNode_str_codegen_c_CGenerator** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_c_CGenerator core_map_Map_str_codegen_c_CGenerator;
__attribute__((hot)) codegen_c_CGenerator** core_alloc_alloc_codegen_c_CGenerator(long long count);
__attribute__((hot)) codegen_c_CGenerator** core_alloc_resize_codegen_c_CGenerator(codegen_c_CGenerator** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_c_CGenerator(codegen_c_CGenerator** ptr);
__attribute__((hot)) codegen_c_CGenerator*** core_alloc_alloc_codegen_c_CGenerator_ptr(long long count);
__attribute__((hot)) codegen_c_CGenerator*** core_alloc_resize_codegen_c_CGenerator_ptr(codegen_c_CGenerator*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_c_CGenerator_ptr(codegen_c_CGenerator*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_c_CGenerator** core_alloc_alloc_core_map_MapNode_str_codegen_c_CGenerator(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_c_CGenerator(core_map_MapNode_str_codegen_c_CGenerator** ptr);

typedef LlvmGenerator codegen_llvm_LlvmGenerator;
struct core_vec_Vec_codegen_llvm_LlvmGenerator { codegen_llvm_LlvmGenerator** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_llvm_LlvmGenerator core_vec_Vec_codegen_llvm_LlvmGenerator;
struct core_vec_Vec_codegen_llvm_LlvmGenerator_ptr { codegen_llvm_LlvmGenerator*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_llvm_LlvmGenerator_ptr core_vec_Vec_codegen_llvm_LlvmGenerator_ptr;
struct core_map_MapNode_str_codegen_llvm_LlvmGenerator { char* key; codegen_llvm_LlvmGenerator* value; struct core_map_MapNode_str_codegen_llvm_LlvmGenerator* next; };
typedef struct core_map_MapNode_str_codegen_llvm_LlvmGenerator core_map_MapNode_str_codegen_llvm_LlvmGenerator;
struct core_map_Map_str_codegen_llvm_LlvmGenerator { core_map_MapNode_str_codegen_llvm_LlvmGenerator** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_llvm_LlvmGenerator core_map_Map_str_codegen_llvm_LlvmGenerator;
__attribute__((hot)) codegen_llvm_LlvmGenerator** core_alloc_alloc_codegen_llvm_LlvmGenerator(long long count);
__attribute__((hot)) codegen_llvm_LlvmGenerator** core_alloc_resize_codegen_llvm_LlvmGenerator(codegen_llvm_LlvmGenerator** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_llvm_LlvmGenerator(codegen_llvm_LlvmGenerator** ptr);
__attribute__((hot)) codegen_llvm_LlvmGenerator*** core_alloc_alloc_codegen_llvm_LlvmGenerator_ptr(long long count);
__attribute__((hot)) codegen_llvm_LlvmGenerator*** core_alloc_resize_codegen_llvm_LlvmGenerator_ptr(codegen_llvm_LlvmGenerator*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_llvm_LlvmGenerator_ptr(codegen_llvm_LlvmGenerator*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_llvm_LlvmGenerator** core_alloc_alloc_core_map_MapNode_str_codegen_llvm_LlvmGenerator(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_llvm_LlvmGenerator(core_map_MapNode_str_codegen_llvm_LlvmGenerator** ptr);

/* Primitive vec/map types for core modules */
struct core_vec_Vec_str { char** data; long long len; long long capacity; };
typedef struct core_vec_Vec_str core_vec_Vec_str;
__attribute__((hot)) char** core_alloc_alloc_str(long long count);
__attribute__((hot)) char** core_alloc_resize_str(char** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_str(char** ptr);
struct core_vec_Vec_i64 { long long* data; long long len; long long capacity; };
typedef struct core_vec_Vec_i64 core_vec_Vec_i64;
__attribute__((hot)) long long* core_alloc_alloc_i64(long long count);
__attribute__((hot)) long long* core_alloc_resize_i64(long long* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_i64(long long* ptr);
struct core_map_MapNode_str_bool { char* key; bool value; struct core_map_MapNode_str_bool* next; };
typedef struct core_map_MapNode_str_bool core_map_MapNode_str_bool;
struct core_map_Map_str_bool { core_map_MapNode_str_bool** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_bool core_map_Map_str_bool;
__attribute__((hot)) core_map_MapNode_str_bool** core_alloc_alloc_core_map_MapNode_str_bool(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_bool(core_map_MapNode_str_bool** ptr);
struct core_map_MapNode_str_str { char* key; char* value; struct core_map_MapNode_str_str* next; };
typedef struct core_map_MapNode_str_str core_map_MapNode_str_str;
struct core_map_Map_str_str { core_map_MapNode_str_str** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_str core_map_Map_str_str;
__attribute__((hot)) core_map_MapNode_str_str** core_alloc_alloc_core_map_MapNode_str_str(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_str(core_map_MapNode_str_str** ptr);
struct core_map_MapNode_str_i64 { char* key; long long value; struct core_map_MapNode_str_i64* next; };
typedef struct core_map_MapNode_str_i64 core_map_MapNode_str_i64;
struct core_map_Map_str_i64 { core_map_MapNode_str_i64** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_i64 core_map_Map_str_i64;
__attribute__((hot)) core_map_MapNode_str_i64** core_alloc_alloc_core_map_MapNode_str_i64(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_i64(core_map_MapNode_str_i64** ptr);
__attribute__((hot)) char* core_alloc_alloc_char(long long count);
__attribute__((hot)) void core_alloc_copy_char(char* dst, char* src, long long count);
__attribute__((hot)) char* core_alloc_resize_char(char* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_char(char* ptr);
