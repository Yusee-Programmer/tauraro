use logos::Logos;
use std::fmt;

#[derive(Logos, Debug, PartialEq, Clone)]
pub enum Token {
    // --- Keywords (English + Hausa) ---
    #[token("func")]
    #[token("def")]      // Python-like function definition
    #[token("aiki")]     // Hausa: function/work
    KwFunc,

    #[token("class")]
    #[token("iri")]      // Hausa: type/kind
    KwClass,

    #[token("if")]
    #[token("idan")]     // Hausa: if
    KwIf,

    #[token("elif")]
    #[token("kokuma idan")] // Hausa: elif
    KwElif,
    
    #[token("else")]
    #[token("akasi")]    // Hausa: else/otherwise
    KwElse,

    #[token("for")]
    #[token("duk")]      // Hausa: for
    KwFor,

    #[token("while")]
    #[token("yayinda")]  // Hausa: while/during
    KwWhile,

    #[token("return")]
    #[token("maido")]    // Hausa: return
    KwReturn,

    #[token("break")]
    #[token("tsaya")]    // Hausa: stop
    KwBreak,

    #[token("continue")]
    #[token("cigaba")]   // Hausa: continue
    KwContinue,

    #[token("import")]
    #[token("shigoda")]  // Hausa: import/enter
    KwImport,

    #[token("from")]
    #[token("daga")]     // Hausa: from
    KwFrom,

    #[token("as")]
    #[token("dasunan")]  // Hausa: as/like
    KwAs,

    #[token("extern")]
    #[token("waje")]     // Hausa: external
    KwExtern,

    #[token("export")]
    #[token("fitar")]    // Hausa: export/output
    KwExport,

    #[token("async")]
    #[token("marasa_jira")] // Hausa: asynchronous
    KwAsync,

    #[token("await")]
    #[token("jira")]     // Hausa: wait
    KwAwait,

    #[token("try")]
    #[token("gwada")]    // Hausa: try
    KwTry,

    #[token("except")]
    #[token("catch")]
    #[token("kama")]     // Hausa: catch
    KwExcept,

    #[token("finally")]
    #[token("karshe")]   // Hausa: finally/end
    KwFinally,

    #[token("raise")]
    #[token("throw")]
    #[token("jefa")]     // Hausa: throw
    KwRaise,

    #[token("with")]
    #[token("tare")]     // Hausa: with/together
    KwWith,

    #[token("yield")]
    #[token("bayar")]    // Hausa: give/yield
    KwYield,

    #[token("lambda")]
    #[token("dan_aiki")] // Hausa: small function
    KwLambda,

    #[token("match")]
    #[token("daidaita")] // Hausa: match/align
    KwMatch,

    #[token("case")]
    #[token("yanayi")]   // Hausa: situation/case
    KwCase,

    #[token("in")]
    #[token("acikin")]    // Hausa: in/inside
    KwIn,

    #[token("is")]
    #[token("shine")]    // Hausa: is
    KwIs,

    #[token("pass")]
    #[token("wuce")]     // Hausa: pass
    KwPass,

    #[token("global")]
    #[token("duniya")]   // Hausa: global/world
    KwGlobal,

    #[token("nonlocal")]
    #[token("ba_gida")]  // Hausa: not local
    KwNonlocal,

    #[token("del")]
    #[token("share")]    // Hausa: delete
    KwDel,

    #[token("assert")]
    #[token("tabbatar")] // Hausa: confirm/assert
    KwAssert,

    #[token("true")]
    #[token("True")]
    #[token("gaskiyane")] // Hausa: truth
    True,

    #[token("false")]
    #[token("False")]
    #[token("karyane")]  // Hausa: falsehood
    False,

    #[token("none")]
    #[token("None")]
    #[token("null")]
    #[token("babu")]     // Hausa: nothing
    None,

    // --- Literals ---
    #[regex(r"0[bB][01]+", |lex| i64::from_str_radix(&lex.slice()[2..], 2).ok())]
    #[regex(r"0[oO][0-7]+", |lex| i64::from_str_radix(&lex.slice()[2..], 8).ok())]
    #[regex(r"0[xX][0-9a-fA-F]+", |lex| i64::from_str_radix(&lex.slice()[2..], 16).ok())]
    #[regex(r"[0-9]+", |lex| lex.slice().parse::<i64>().ok())]
    Int(i64),

    #[regex(r"[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?", |lex| lex.slice().parse::<f64>().ok())]
    #[regex(r"[0-9]+[eE][+-]?[0-9]+", |lex| lex.slice().parse::<f64>().ok())]
    Float(f64),

    // Bytes literals (must come before string literals to take precedence)
    #[regex(r#"b"([^"\\]|\\.)*""#, |lex| unescape_string(&lex.slice()[1..]))]
    #[regex(r#"b'([^'\\]|\\.)*'"#, |lex| unescape_string(&lex.slice()[1..]))]
    BytesLit(String),

    // String literals with various formats
    #[regex(r#""([^"\\]|\\.)*""#, |lex| unescape_string(lex.slice()))]
    #[regex(r#"'([^'\\]|\\.)*'"#, |lex| unescape_string(lex.slice()))]
    #[regex(r#"r"[^"]*""#, |lex| lex.slice()[2..lex.slice().len()-1].to_string())]
    #[regex(r#"r'[^']*'"#, |lex| lex.slice()[2..lex.slice().len()-1].to_string())]
    StringLit(String),

    // Docstring literals - handled manually in lexer iterator
    DocString(String),

    // F-string literals (formatted strings)
    #[regex(r#"f"([^"\\]|\\.)*""#, |lex| lex.slice()[2..lex.slice().len()-1].to_string())]
    #[regex(r#"f'([^'\\]|\\.)*'"#, |lex| lex.slice()[2..lex.slice().len()-1].to_string())]
    FString(String),

    // --- Identifiers ---
    #[regex(r"[a-zA-Z_\u{0590}-\u{05FF}\u{0600}-\u{06FF}][a-zA-Z0-9_\u{0590}-\u{05FF}\u{0600}-\u{06FF}]*", |lex| lex.slice().to_string())]
    Identifier(String),

    // --- Operators ---
    // Arithmetic
    #[token("+")] Plus,
    #[token("-")] Minus,
    #[token("*")] Star,
    #[token("/")] Slash,
    #[token("%")] Percent,
    #[token("**")] Power,
    #[token("//")] FloorDiv,

    // Assignment operators
    #[token("+=")] PlusEq,
    #[token("-=")] MinusEq,
    #[token("*=")] StarEq,
    #[token("/=")] SlashEq,
    #[token("%=")] PercentEq,
    #[token("**=")] PowerEq,
    #[token("//=")] FloorDivEq,

    // Comparison operators
    #[token("==")] Eq,
    #[token("!=")] Neq,
    #[token(">")] Gt,
    #[token("<")] Lt,
    #[token(">=")] Gte,
    #[token("<=")] Lte,

    // Logical operators
    #[token("and")]
    #[token("dakuma")]  // Hausa: and
    And,
    
    #[token("or")]
    #[token("ko")]  // Hausa: or
    Or,
    
    #[token("not")]
    #[token("ba")]  // Hausa: not
    Not,

    // Bitwise operators
    #[token("&")] BitAnd,
    #[token("|")] BitOr,
    #[token("^")] BitXor,
    #[token("~")] BitNot,
    #[token("<<")] Shl,
    #[token(">>")] Shr,

    // Assignment and other operators
    #[token("=")] Assign,
    #[token(":=")] WalrusOp,  // Walrus operator (assignment expression)
    #[token(":")] Colon,
    #[token("->")] Arrow,
    #[token("=>")] FatArrow,

    // Delimiters
    #[token("(")] LParen,
    #[token(")")] RParen,
    #[token("{")] LBrace,
    #[token("}")] RBrace,
    #[token("[")] LBracket,
    #[token("]")] RBracket,
    #[token(";")] Semicolon,
    #[token(",")] Comma,
    #[token(".")] Dot,
    #[token("...")] Ellipsis,

    // Special tokens
    #[token("@")] At,      // Decorators
    #[token("$")] Dollar,  // String interpolation
    #[token("?")] Question, // Optional chaining
    #[token("!!")] DoubleExclamation, // Non-null assertion

    // Indentation tokens for Python-like syntax
    Indent,
    Dedent,

    // Docstring literals (triple-quoted strings) - handled by string literal patterns below
    // Note: For now, triple-quoted strings are treated as regular string literals
    // The regex engine limitations in Logos make it difficult to implement true docstrings
    // TODO: Implement proper multi-line docstring support

    // Comments (Python-style) - tokenize but will be filtered out in lexer iterator
    #[regex(r"#.*", |lex| lex.slice()[1..].trim().to_string())]
    Comment(String),

    // Whitespace - skip all horizontal whitespace and line continuations
    #[regex(r"[ \t\r]+", logos::skip)]
    #[regex(r"\\\r?", logos::skip)]  // Line continuation (backslash followed by newline)

    #[regex(r"", |_| Token::Newline)]
    Newline,

    // End of file
    Eof,
}

fn unescape_string(s: &str) -> String {
    let mut result = String::new();
    let mut chars = s[1..s.len()-1].chars(); // Remove quotes
    while let Some(ch) = chars.next() {
        if ch == '\\' {
            if let Some(escaped) = chars.next() {
                match escaped {
                    'n' => result.push('\n'),
                    't' => result.push('\t'),
                    'r' => result.push('\r'),
                    '\\' => result.push('\\'),
                    '\'' => result.push('\''),
                    '"' => result.push('"'),
                    _ => {
                        result.push('\\');
                        result.push(escaped);
                    }
                }
            }
        } else {
            result.push(ch);
        }
    }
    result
}

#[derive(Debug, Clone)]
pub struct TokenInfo {
    pub token: Token,
    pub span: (usize, usize),
    pub line: usize,
    pub column: usize,
}

pub struct Lexer<'a> {
    inner: logos::Lexer<'a, Token>,
    filename: String,
    line: usize,
    column: usize,
    indent_stack: Vec<usize>,
    pending_dedents: usize,
    at_line_start: bool,
    paren_depth: usize,
    buffered_token: Option<TokenInfo>,
}

impl<'a> Lexer<'a> {
    pub fn new(source: &'a str, filename: String) -> Self {
        Self {
            inner: Token::lexer(source),
            filename,
            line: 1,
            column: 1,
            indent_stack: vec![0],
            pending_dedents: 0,
            at_line_start: true,
            paren_depth: 0,buffered_token: None,

        }
    }

    fn handle_indentation(&mut self, line_start: &str) -> Option<Token> {
        if self.paren_depth > 0 {
            return None; // Ignore indentation inside parentheses
        }

        let indent_level = line_start.chars().take_while(|&c| c == ' ' || c == '\t').count();
        let current_indent = *self.indent_stack.last().unwrap();

        if indent_level > current_indent {
            self.indent_stack.push(indent_level);
            Some(Token::Indent)
        } else if indent_level < current_indent {
            let mut dedent_count = 0;
            while let Some(&stack_indent) = self.indent_stack.last() {
                if stack_indent <= indent_level {
                    break;
                }
                self.indent_stack.pop();
                dedent_count += 1;
            }
            
            if dedent_count > 0 {
                self.pending_dedents = dedent_count - 1;
                Some(Token::Dedent)
            } else {
                None
            }
        } else {
            None
        }
    }
}

impl<'a> Iterator for Lexer<'a> {
    type Item = Result<TokenInfo, String>;

    fn next(&mut self) -> Option<Self::Item> {
        // Handle pending dedents first
        if self.pending_dedents > 0 {
            self.pending_dedents -= 1;
            return Some(Ok(TokenInfo {
                token: Token::Dedent,
                span: (0, 0),
                line: self.line,
                column: self.column,
            }));
        }

        // Return buffered token if available
        if let Some(buffered) = self.buffered_token.take() {
            return Some(Ok(buffered));
        }

        match self.inner.next() {
            Some(Ok(token)) => {
                let span = self.inner.span();

                // Handle indentation at the start of a line (only after newlines)
                // Do this BEFORE skipping comments so that comments on indented lines work correctly
                if self.at_line_start && !matches!(token, Token::Newline | Token::Eof) {
                    // Get the line content to check indentation
                    let source = self.inner.source();
                    
                    // Find the start of the current line using char_indices for UTF-8 safety
                    let mut line_begin = span.start;
                    let char_indices: Vec<(usize, char)> = source.char_indices().collect();
                    
                    // Find the character index that corresponds to span.start
                    let mut span_char_idx = 0;
                    for (i, (byte_idx, _)) in char_indices.iter().enumerate() {
                        if *byte_idx >= span.start {
                            span_char_idx = i;
                            break;
                        }
                    }
                    
                    // Find the beginning of the line by looking backwards for newline
                    let mut line_char_begin = span_char_idx;
                    while line_char_begin > 0 {
                        if char_indices[line_char_begin - 1].1 == '\n' {
                            break;
                        }
                        line_char_begin -= 1;
                    }
                    
                    // Get the byte index for the line beginning
                    line_begin = if line_char_begin < char_indices.len() {
                        char_indices[line_char_begin].0
                    } else {
                        0
                    };
                    
                    // Only handle indentation if we're actually at the start of a new line (after newline)
                    // Skip indentation handling for the very first line
                    if line_begin > 0 || (line_begin == 0 && self.line > 1) {
                        // Extract the whitespace at the beginning of the line safely
                        let line_content = if line_begin <= span.start && span.start <= source.len() {
                            &source[line_begin..span.start]
                        } else {
                            "" // Fallback to empty string if indices are invalid
                        };
                        
                        if let Some(indent_token) = self.handle_indentation(line_content) {
                            // Buffer the current token for next iteration
                            self.buffered_token = Some(TokenInfo {
                                token: token.clone(),
                                span: (span.start, span.end),
                                line: self.line,
                                column: self.column,
                            });
                            
                            // Mark that we're no longer at line start to prevent re-processing
                            self.at_line_start = false;
                            
                            return Some(Ok(TokenInfo {
                                token: indent_token,
                                span: (line_begin, span.start),
                                line: self.line,
                                column: 1,
                            }));
                        }
                    }
                    self.at_line_start = false;
                }

                // Filter out comments - skip them entirely like Python does
                // This happens AFTER indentation handling so comments on indented lines work correctly
                if matches!(token, Token::Comment(_)) {
                    // Update column position for the comment
                    self.column += span.end - span.start;
                    // Recursively get the next token
                    return self.next();
                }

                let token_info = TokenInfo {
                    token: token.clone(),
                    span: (span.start, span.end),
                    line: self.line,
                    column: self.column,
                };

                // Update position tracking
                match &token {
                    Token::Newline => {
                        self.line += 1;
                        self.column = 1;
                        self.at_line_start = true;
                    }
                    Token::LParen | Token::LBracket | Token::LBrace => {
                        self.paren_depth += 1;
                        self.column += span.end - span.start;
                        self.at_line_start = false;
                    }
                    Token::RParen | Token::RBracket | Token::RBrace => {
                        self.paren_depth = self.paren_depth.saturating_sub(1);
                        self.column += span.end - span.start;
                        self.at_line_start = false;
                    }
                    _ => {
                        self.column += span.end - span.start;
                        self.at_line_start = false;
                    }
                }

                Some(Ok(token_info))
            }
            Some(Err(_)) => {
                let span = self.inner.span();
                let source_up_to_error = &self.inner.source()[..span.start];
                let mut line = 1;
                let mut column = 1;
                for ch in source_up_to_error.chars() {
                    if ch == '\n' {
                        line += 1;
                        column = 1;
                    } else {
                        column += 1;
                    }
                }
                Some(Err(format!("Lexical error at {}:{}, {}", self.filename, line, column)))
            }
            None => {
                // Handle remaining dedents at EOF
                if !self.indent_stack.is_empty() && self.indent_stack.len() > 1 {
                    self.indent_stack.pop();
                    Some(Ok(TokenInfo {
                        token: Token::Dedent,
                        span: (0, 0),
                        line: self.line,
                        column: self.column,
                    }))
                } else {
                    None
                }
            }
        }
    }
}

impl fmt::Display for TokenInfo {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.token)
    }
}

impl fmt::Display for Token {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Token::KwFunc => write!(f, "func"),
            Token::KwClass => write!(f, "class"),
            Token::KwIf => write!(f, "if"),
            Token::KwElif => write!(f, "elif"),
            Token::KwElse => write!(f, "else"),
            Token::KwFor => write!(f, "for"),
            Token::KwWhile => write!(f, "while"),
            Token::KwReturn => write!(f, "return"),
            Token::KwBreak => write!(f, "break"),
            Token::KwContinue => write!(f, "continue"),
            Token::KwImport => write!(f, "import"),
            Token::KwFrom => write!(f, "from"),
            Token::KwAs => write!(f, "as"),
            Token::KwExtern => write!(f, "extern"),
            Token::KwExport => write!(f, "export"),
            Token::KwAsync => write!(f, "async"),
            Token::KwAwait => write!(f, "await"),
            Token::KwTry => write!(f, "try"),
            Token::KwExcept => write!(f, "except"),
            Token::KwFinally => write!(f, "finally"),
            Token::KwRaise => write!(f, "raise"),
            Token::KwWith => write!(f, "with"),
            Token::KwYield => write!(f, "yield"),
            Token::KwLambda => write!(f, "lambda"),
            Token::KwMatch => write!(f, "match"),
            Token::KwCase => write!(f, "case"),
            Token::KwIn => write!(f, "in"),
            Token::KwIs => write!(f, "is"),
            Token::KwPass => write!(f, "pass"),
            Token::KwGlobal => write!(f, "global"),
            Token::KwNonlocal => write!(f, "nonlocal"),
            Token::KwDel => write!(f, "del"),
            Token::KwAssert => write!(f, "assert"),
            Token::True => write!(f, "true"),
            Token::False => write!(f, "false"),
            Token::None => write!(f, "none"),
            Token::Int(n) => write!(f, "{}", n),
            Token::Float(n) => write!(f, "{}", n),
            Token::BytesLit(s) => write!(f, "b\"{}\"", s),
            Token::StringLit(s) => write!(f, "\"{}\"", s),
            Token::FString(s) => write!(f, "f\"{}\"", s),
            Token::Identifier(s) => write!(f, "{}", s),
            Token::Plus => write!(f, "+"),
            Token::Minus => write!(f, "-"),
            Token::Star => write!(f, "*"),
            Token::Slash => write!(f, "/"),
            Token::Percent => write!(f, "%"),
            Token::Power => write!(f, "**"),
            Token::FloorDiv => write!(f, "//"),
            Token::Eq => write!(f, "=="),
            Token::Neq => write!(f, "!="),
            Token::Lt => write!(f, "<"),
            Token::Lte => write!(f, "<="),
            Token::Gt => write!(f, ">"),
            Token::Gte => write!(f, ">="),
            Token::And => write!(f, "and"),
            Token::Or => write!(f, "or"),
            Token::Not => write!(f, "not"),
            Token::Assign => write!(f, "="),
            Token::Colon => write!(f, ":"),
            Token::Arrow => write!(f, "->"),
            Token::FatArrow => write!(f, "=>"),
            Token::LParen => write!(f, "("),
            Token::RParen => write!(f, ")"),
            Token::LBrace => write!(f, "{{"),
            Token::RBrace => write!(f, "}}"),
            Token::LBracket => write!(f, "["),
            Token::RBracket => write!(f, "]"),
            Token::Semicolon => write!(f, ";"),
            Token::Comma => write!(f, ","),
            Token::Dot => write!(f, "."),
            Token::Ellipsis => write!(f, "..."),
            Token::At => write!(f, "@"),
            Token::Dollar => write!(f, "$"),
            Token::Question => write!(f, "?"),
            Token::DoubleExclamation => write!(f, "!!"),
            Token::Indent => write!(f, "INDENT"),
            Token::Dedent => write!(f, "DEDENT"),
            Token::Newline => write!(f, "NEWLINE"),
            Token::Eof => write!(f, "EOF"),
            _ => write!(f, "{:?}", self),
        }
    }
}
