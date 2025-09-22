mod lexer;

use lexer::Token;
use logos::Logos;

fn main() {
    let source = r#"
        func hello(name):
            return "Hello " + name
    "#;

    let mut lex = Token::lexer(source);

    while let Some(token) = lex.next() {
        println!("{:?}", token);
    }
}