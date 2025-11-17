// Disabled: Triple-quote pre-computation disabled; use error-based approach instead
// triple_quote_regions: Vec<(usize, usize, String)>,
// next_triple_quote_idx: usize,
use logos::Logos;
use std::fmt;

// Helper function to find all triple-quoted strings in the source
fn find_triple_quote_regions(source: &str) -> Vec<(usize, usize, String)> {
    let mut regions = Vec::new();
    let bytes = source.as_bytes();
    let mut i = 0;
    
    while i < bytes.len() {
        // Check for triple quotes starting at position i
        if i + 3 <= bytes.len() {
            if (bytes[i] == b'"' && bytes[i + 1] == b'"' && bytes[i + 2] == b'"') ||
               (bytes[i] == b'\'' && bytes[i + 1] == b'\'' && bytes[i + 2] == b'\'') {
                let quote_byte = bytes[i];
                let quote_char = bytes[i] as char;
                let search_start = i + 3;
                
                // Look for closing triple quotes
                let mut j = search_start;
                while j + 3 <= bytes.len() {
                    if bytes[j] == quote_byte && bytes[j + 1] == quote_byte && bytes[j + 2] == quote_byte {
                        // Found closing triple quotes
                        let content = source[search_start..j].to_string();

                        regions.push((i, j + 3, content));
                        i = j + 3;
                        break;
                    }
                    j += 1;
                }
                
                // If we didn't find closing, move forward
                if j + 3 > bytes.len() {
                    i += 1;
                }
                continue;
            }
        }
        i += 1;
    }
    

    regions
}

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

    // String literals - NOTE: Triple-quoted strings must be handled in the lexer iterator
    // because regex cannot properly match multi-line patterns
    // We detect and parse them manually when we see """ or '''
    #[regex(r#""([^"\\]|\\.)*""#, |lex| unescape_string(lex.slice()))]
    #[regex(r#"'([^'\\]|\\.)*'"#, |lex| unescape_string(lex.slice()))]
    #[regex(r#"r"[^"]*""#, |lex| lex.slice()[2..lex.slice().len()-1].to_string())]
    #[regex(r#"r'[^']*'"#, |lex| lex.slice()[2..lex.slice().len()-1].to_string())]
    StringLit(String),

    // Docstring literals - triple-quoted strings
    // Note: These are handled specially in the lexer iterator due to logos limitations
    

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

    #[regex(r"\n", |_| Token::Newline)]
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
    source: &'a str,
    byte_pos: usize,
    triple_quote_regions: Vec<(usize, usize, String)>,
    next_triple_quote_idx: usize,
}

impl<'a> Lexer<'a> {
    pub fn new(source: &'a str, filename: String) -> Self {
        let triple_quote_regions = find_triple_quote_regions(source);
        Self {
            inner: Token::lexer(source),
            filename,
            line: 1,
            column: 1,
            indent_stack: vec![0],
            pending_dedents: 0,
            at_line_start: true,
            paren_depth: 0,
            buffered_token: None,
            source,
            byte_pos: 0,
            triple_quote_regions,
            next_triple_quote_idx: 0,
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

    // Try to parse a triple-quoted string starting at the current position
    fn try_parse_triple_quote(&self, pos: usize) -> Option<(String, usize)> {
        let remaining = &self.source[pos..];

        // Check for triple-quoted strings ("""...""" or '''...''')
        if remaining.starts_with("\"\"\"") {
            if let Some(end_pos) = remaining[3..].find("\"\"\"") {
                let content = remaining[3..3 + end_pos].to_string();
                let total_len = 6 + end_pos; // 3 quotes + content + 3 quotes
                return Some((content, total_len));
            }
        } else if remaining.starts_with("'''") {
            if let Some(end_pos) = remaining[3..].find("'''") {
                let content = remaining[3..3 + end_pos].to_string();
                let total_len = 6 + end_pos; // 3 quotes + content + 3 quotes
                return Some((content, total_len));
            }
        }
        None
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

        loop {
            // We'll consume the next logos token first, then check whether its span
            // falls inside a triple-quote region. If it does, replace the logos token
            // with a single StringLit containing the full triple-quoted content.

            match self.inner.next() {
                Some(Ok(orig_token)) => {
                    // Get the current logos span for this token
                    let span = self.inner.span();

                    // Precomputed triple-quote interception: if this token starts inside
                    // a known triple-quote region, replace the token with a single
                    // StringLit containing the full triple-quoted content and adjust
                    // the span accordingly.
                    let mut token = orig_token.clone();
                    let mut token_span_tuple = (span.start, span.end);

                    for idx in self.next_triple_quote_idx..self.triple_quote_regions.len() {
                        let (start, end, content) = self.triple_quote_regions[idx].clone();
                        // If the current logos token starts inside this triple-quote region
                        if span.start >= start && span.start < end {
                            // Advance logos until we've consumed tokens that end before
                            // the triple-quote region end. This avoids consuming the
                            // first token that starts at or after `end` (e.g. the
                            // closing parenthesis) which must remain available for
                            // normal parsing.
                            while self.inner.span().end < end {
                                if self.inner.next().is_none() {
                                    break;
                                }
                            }
                            // Update the next_triple_quote_idx to skip processed regions
                            self.next_triple_quote_idx = idx + 1;

                            // Replace token and span with the triple-quoted StringLit
                            token = Token::StringLit(content.clone());
                            token_span_tuple = (start, end);

                            // Update line/column tracking for the triple-quoted content
                            let newline_count = content.matches('\n').count();
                            if newline_count > 0 {
                                self.line += newline_count;
                                self.column = 1;
                                if let Some(last_line) = content.split('\n').last() {
                                    self.column = last_line.len() + 1;
                                }
                            } else {
                                self.column += end - start;
                            }

                            break;
                        } else if span.start >= end {
                            // We've passed this region; advance the index
                            self.next_triple_quote_idx = idx + 1;
                            continue;
                        } else {
                            // This and following regions start after current token
                            break;
                        }
                    }

                    // Use the possibly-updated token and span for further processing
                    let span = token_span_tuple.0..token_span_tuple.1;

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

                return Some(Ok(token_info));
            }
            Some(Err(_)) => {
                let span = self.inner.span();
                let pos = span.start;
                let source = self.inner.source();
                
                // Check if this is a triple-quoted string error
                // Triple quotes would cause an error because logos tries to parse """ as an empty string ""
                // followed by an unclosed quote "
                if let Some((content, length)) = self.try_parse_triple_quote(pos) {
                    // Count newlines in the content to update line tracking
                    let newline_count = content.matches('\n').count();
                    
                    let content_len = content.len();
                    let token_info = TokenInfo {
                        token: Token::StringLit(content),
                        span: (pos, pos + length),
                        line: self.line,
                        column: self.column,
                    };
                    
                    if newline_count > 0 {
                        self.line += newline_count;
                        self.column = 1;
                        if let Some(last_line) = source[pos..pos + length].split('\n').last() {
                            self.column = last_line.len() + 1;
                        }
                    } else {
                        self.column += length;
                    }
                    self.at_line_start = false;
                    
                    // Consume the error from logos and advance past the triple-quoted string
                    // Skip one character at a time to let logos skip the erroneous tokens
                    let target_end = pos + length;
                    while self.inner.span().start < target_end {
                        match self.inner.next() {
                            Some(_) => {
                                // Continue advancing
                            }
                            None => break,
                        }
                    }
                    
                    return Some(Ok(token_info));
                }
                
                // Not a triple-quoted string, report the error
                let source_up_to_error = &source[..span.start];
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
                return Some(Err(format!("Lexical error at {}:{}, {}", self.filename, line, column)));
            }
            None => {
                // Handle remaining dedents at EOF
                if !self.indent_stack.is_empty() && self.indent_stack.len() > 1 {
                    self.indent_stack.pop();
                    return Some(Ok(TokenInfo {
                        token: Token::Dedent,
                        span: (0, 0),
                        line: self.line,
                        column: self.column,
                    }));
                } else {
                    return None;
                }
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
