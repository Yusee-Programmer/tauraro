use std::time::Instant;
use std::hint::black_box;

enum Token<'a> {
    Word(&'a str),
    Number(i64),
    End,
}

fn main() {
    let src = String::from("the quick brown fox jumps over the lazy dog while the sun shines");
    let mut held: Vec<Token> = Vec::new();
    let mut total: u64 = 0;
    let start = Instant::now();
    for _ in 0..200000 {
        let t = Token::Word(src.as_str());   // borrowed payload, no allocation
        held.push(t);
        total += 1;
    }
    let ms = start.elapsed().as_millis();
    println!("{}", total);
    println!("TIME_MS:{}", ms);
    black_box(&held);
}
