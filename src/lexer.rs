//! Complete lexical analysis for TauraroLang - Tokenizes source code into tokens
use logos::Logos;
use std::fmt;

#[derive(Logos, Debug, PartialEq, Clone)]
pub enum Token {
    // --- Keywords (English + Hausa) ---
    #[token("func")] 
    #[token("aiki")]  // Hausa: function/work
    KwFunc,

    #[token("class")] 
    #[token("iri")]   // Hausa: type/kind
    KwClass,

    #[token("if")] 
    #[token("idan")]  // Hausa: if
    KwIf,

    #[token("elif")] 
    #[token("kokuma")] // Hausa: elif
    KwElif,
    
    #[token("else")] 
    #[token("akasi")]   // Hausa: else/otherwise
    KwElse,

    #[token("for")] 
    #[token("duk")]   // Hausa: for
    KwFor,

    #[token("while")] 
    #[token("yayinda")] // Hausa: while/during
    KwWhile,

    #[token("return")] 
    #[token("maido")] // Hausa: return
    KwReturn,

    #[token("break")] 
    #[token("tsaya")] // Hausa: stop
    KwBreak,

    #[token("continue")] 
    #[token("cigaba")] // Hausa: continue
    KwContinue,

    #[token("import")] 
    #[token("shigoda")] // Hausa: import/enter
    KwImport,

    #[token("from")] 
    #[token("daga")]  // Hausa: from
    KwFrom,

    #[token("as")] 
    #[token("dasunan")] // Hausa: as/like
    KwAs,

    #[token("extern")]
    KwExtern,

    #[token("export")]
    KwExport,

    #[token("async")]
    KwAsync,

    #[token("await")] 
    #[token("jira")]  // Hausa: wait
    KwAwait,

    #[token("true")] 
    #[token("gaskiyane")] // Hausa: truth
    True,

    #[token("false")] 
    #[token("karyane")]  // Hausa: falsehood
    False,

    #[token("none")] 
    #[token("babu")]   // Hausa: nothing
    None,

    #[token("=")] Assign,           // Add this

    #[token("in")]           // Add this
    #[token("cikin")] KwIn,         // Alternative if needed

    // --- Literals ---
    #[regex(r"[0-9]+", |lex| lex.slice().parse::<i64>().ok())]
    Int(i64),

    #[regex(r"[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?", |lex| lex.slice().parse::<f64>().ok())]
    Float(f64),

    // String literals with escape sequences
    #[regex(r#""([^"\\]|\\.)*""#, |lex| unescape_string(lex.slice()))]
    #[regex(r#"'([^'\\]|\\.)*'"#, |lex| unescape_string(lex.slice()))]
    StringLit(String),

    // --- Identifiers ---
    #[regex(r"[a-zA-Z_][a-zA-Z0-9_]*", |lex| lex.slice().to_string())]
    Identifier(String),

    // --- Operators ---
    #[token("+")] Plus,
    #[token("-")] Minus,
    #[token("*")] Star,
    #[token("/")] Slash,
    #[token("%")] Percent,
    #[token("**")] Power,
    #[token("//")] FloorDiv,

    // Compound assignment
    #[token("+=")] PlusEq,
    #[token("-=")] MinusEq,
    #[token("*=")] StarEq,
    #[token("/=")] SlashEq,
    #[token("%=")] PercentEq,
    #[token("**=")] PowerEq,
    #[token("//=")] FloorDivEq,

    // Comparison
    #[token("==")] Eq,
    #[token("!=")] Neq,
    #[token(">")] Gt,
    #[token("<")] Lt,
    #[token(">=")] Gte,
    #[token("<=")] Lte,

    // Logical
    #[token("and")] 
    #[token("dakuma")]  // Hausa: and
    And,
    
    #[token("or")] 
    #[token("kokuma")]  // Hausa: or
    Or,
    
    #[token("not")] 
    #[token("ba")]  // Hausa: not
    Not,

    // Bitwise
    #[token("&")] BitAnd,
    #[token("|")] BitOr,
    #[token("^")] BitXor,
    #[token("~")] BitNot,
    #[token("<<")] Shl,
    #[token(">>")] Shr,

    // Type annotation
    #[token(":")] Colon,
    #[token("->")] Arrow,

    // --- Delimiters ---
    #[token("(")] LParen,
    #[token(")")] RParen,
    #[token("{")] LBrace,
    #[token("}")] RBrace,
    #[token("[")] LBracket,
    #[token("]")] RBracket,
    #[token(";")] Semicolon,
    #[token(",")] Comma,
    #[token(".")] Dot,

    // --- Special ---
    #[token("@")] At,      // Decorators
    #[token("$")] Dollar,  // String interpolation

    // --- Whitespace and comments ---
    #[regex(r"[ \t\r]+", logos::skip)]
    #[regex(r"#.*", logos::skip)]  // Line comments
    #[regex(r"\n", |lex| {
        Some(lex.slice().to_string())
    })]
    Newline(String),
    
    // Error variant
}

/// Lexer error type
#[derive(Debug, Clone, PartialEq)]
pub struct LexError {
    pub message: String,
    pub position: usize,
}

impl fmt::Display for LexError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Lex error at position {}: {}", self.position, self.message)
    }
}

impl std::error::Error for LexError {}

/// Unescape string literals (remove quotes and process escape sequences)
fn unescape_string(s: &str) -> Option<String> {
    let content = &s[1..s.len()-1];  // Remove surrounding quotes
    let mut result = String::new();
    let mut chars = content.chars().peekable();
    
    while let Some(ch) = chars.next() {
        if ch == '\\' {
            match chars.next() {
                Some('n') => result.push('\n'),
                Some('t') => result.push('\t'),
                Some('r') => result.push('\r'),
                Some('"') => result.push('"'),
                Some('\'') => result.push('\''),
                Some('\\') => result.push('\\'),
                Some('x') => {
                    // Hex escape: \xXX
                    let hex_str: String = chars.by_ref().take(2).collect();
                    if hex_str.len() == 2 {
                        if let Ok(val) = u8::from_str_radix(&hex_str, 16) {
                            result.push(val as char);
                        }
                    }
                }
                Some(c) => {
                    // Keep invalid escape sequences as-is
                    result.push('\\');
                    result.push(c);
                }
                None => break,  // Unclosed escape
            }
        } else {
            result.push(ch);
        }
    }
    Some(result)
}

/// Enhanced Lexer with error handling and position tracking
pub struct Lexer<'a> {
    inner: logos::Lexer<'a, Token>,
    current_line: usize,
    current_column: usize,
}

impl<'a> Lexer<'a> {
    pub fn new(source: &'a str) -> Self {
        Self {
            inner: Token::lexer(source),
            current_line: 1,
            current_column: 1,
        }
    }
    
    /// Get current position in source
    pub fn position(&self) -> usize {
        self.inner.span().start
    }
    
    /// Get current line and column
    pub fn line_col(&self) -> (usize, usize) {
        (self.current_line, self.current_column)
    }
}

impl<'a> Iterator for Lexer<'a> {
    type Item = Result<Token, LexError>;
    
    fn next(&mut self) -> Option<Self::Item> {
        match self.inner.next() {
            Some(Ok(token)) => {
                // Update line/column tracking
                if let Token::Newline(_) = token {
                    self.current_line += 1;
                    self.current_column = 1;
                } else {
                    self.current_column += self.inner.slice().len();
                }
                
                Some(Ok(token))
            }
            Some(Err(_)) => {
                let pos = self.inner.span().start;
                Some(Err(LexError {
                    message: format!("Invalid token: '{}'", self.inner.slice()),
                    position: pos,
                }))
            }
            None => None,
        }
    }
}

// Utility methods for tokens
impl Token {
    /// Check if token is a keyword
    pub fn is_keyword(&self) -> bool {
        matches!(self,
            Token::KwFunc | Token::KwClass | Token::KwIf |
            Token::KwElse | Token::KwFor | Token::KwWhile |
            Token::KwReturn | Token::KwBreak | Token::KwContinue |
            Token::KwImport | Token::KwFrom | Token::KwAs |
            Token::KwExtern | Token::KwExport | Token::KwAsync |
            Token::KwAwait | Token::KwElif
        )
    }
    
    /// Get Hausa equivalent for keyword tokens
    pub fn to_hausa(&self) -> Option<&'static str> {
        match self {
            Token::KwFunc => Some("aiki"),
            Token::KwClass => Some("iri"),
            Token::KwIf => Some("idan"),
            Token::KwElse => Some("sai dai"),
            Token::KwFor => Some("don"),
            Token::KwWhile => Some("yayin"),
            Token::KwReturn => Some("mayar"),
            Token::KwBreak => Some("tsaya"),
            Token::KwContinue => Some("cigaba"),
            Token::KwImport => Some("shigo"),
            Token::KwFrom => Some("daga"),
            Token::KwAs => Some("kamar"),
            Token::KwAwait => Some("jira"),
            Token::True => Some("gaskiya"),
            Token::False => Some("karya"),
            Token::None => Some("babu"),
            Token::And => Some("da"),
            Token::Or => Some("ko"),
            Token::Not => Some("ba"),
            _ => None,
        }
    }
    
    /// Check if token can start an expression
    pub fn can_start_expression(&self) -> bool {
        matches!(self,
            Token::Int(_) | Token::Float(_) | Token::StringLit(_) |
            Token::True | Token::False | Token::None |
            Token::Identifier(_) | Token::LParen | Token::LBracket |
            Token::LBrace | Token::Plus | Token::Minus | Token::Not
        )
    }
}