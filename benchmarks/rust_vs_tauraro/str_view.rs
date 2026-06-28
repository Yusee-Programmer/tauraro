use std::time::Instant;
use std::hint::black_box;

fn main() {
    let mut src = String::from("abcdefghij");
    for _ in 0..7 { let c = src.clone(); src.push_str(&c); }   // ~1280 chars
    let mut held: Vec<&str> = Vec::new();
    let mut total: u64 = 0;
    let start = Instant::now();
    for _ in 0..100000 {
        let piece = &src[0..1000];        // zero-copy slice, no allocation
        total += piece.len() as u64;
        held.push(piece);
    }
    let ms = start.elapsed().as_millis();
    println!("{}", total);
    println!("TIME_MS:{}", ms);
    black_box(&held);
}
