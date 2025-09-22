use logos::Logos;

#[derive(Logos, Debug, PartialEq, Clone)]
pub enum Token {
    // --- Keywords (English + Hausa) ---
    #[token("func")] #[token("aiki")]
    Func,

    #[token("class")] #[token("irin")]
    Class,

    #[token("if")] #[token("idan")]
    If,

    #[token("elif")] #[token("idan kuma")]
    Elif,

    #[token("else")] #[token("akasi")]
    Else,

    #[token("for")] #[token("duk")]
    For,

    #[token("while")] #[token("yayinda")]
    While,

    #[token("return")] #[token("maido")]
    Return,

    #[token("break")] #[token("tsaya")]
    Break,

    #[token("continue")] #[token("cigaba")]
    Continue,

    #[token("import")] #[token("shigoda")]
    Import,

    #[token("from")] #[token("daga")]
    From,

    #[token("as")] #[token("amatsayin")]
    As,

    #[token("extern")]
    Extern,

    #[token("export")]
    Export,

    #[token("async")]
    Async,

    #[token("await")] #[token("jira")]
    Await,

    // --- Literals ---
    #[regex("[0-9]+", |lex| lex.slice().parse())]
    Int(i64),

    #[regex("[0-9]+\\.[0-9]+", |lex| lex.slice().parse())]
    Float(f64),

    #[regex(r#""([^"\\]|\\.)*""#, |lex| lex.slice().to_string())]
    String(String),

    #[token("true")] #[token("gaskiyane")]
    True,

    #[token("false")] #[token("karyane")]
    False,

    #[token("none")] #[token("babu")]
    None,

    // --- Identifiers ---
    #[regex("[a-zA-Z_][a-zA-Z0-9_]*", |lex| lex.slice().to_string())]
    Identifier(String),

    // --- Operators ---
    #[token("+")] Plus,
    #[token("-")] Minus,
    #[token("*")] Star,
    #[token("/")] Slash,
    #[token("%")] Percent,
    #[token("**")] Power,
    #[token("//")] FloorDiv,

    #[token("=")] Assign,
    #[token("==")] Eq,
    #[token("!=")] Neq,
    #[token(">")] Gt,
    #[token("<")] Lt,
    #[token(">=")] Gte,
    #[token("<=")] Lte,

    #[token("and")] #[token("dakuma")]
    And,

    #[token("or")] #[token("kokuma")]
    Or,

    #[token("not")] #[token("ba")]
    Not,

    #[token("&")] BitAnd,
    #[token("|")] BitOr,
    #[token("^")] BitXor,
    #[token("~")] BitNot,
    #[token("<<")] Shl,
    #[token(">>")] Shr,

    // --- Delimiters ---
    #[token("(")] LParen,
    #[token(")")] RParen,
    #[token("{")] LBrace,
    #[token("}")] RBrace,
    #[token("[")] LBracket,
    #[token("]")] RBracket,
    #[token(":")] Colon,
    #[token(";")] Semicolon,
    #[token(",")] Comma,
    #[token(".")] Dot,

    // --- Skip whitespace/comments ---
    #[regex(r"[ \t\n\f]+", logos::skip)]
    #[regex(r"#.*", logos::skip)]
    Error,
}
