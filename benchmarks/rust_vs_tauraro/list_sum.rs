use std::time::Instant;
use std::hint::black_box;
fn main() {
    let mut xs: Vec<i64> = Vec::new();
    for i in 0..1000 { xs.push(i); }
    let mut total: i64 = 0;
    let start = Instant::now();
    for _ in 0..200_000 {
        for &x in &xs { total += x; }
        black_box(&xs);
    }
    let ms = start.elapsed().as_millis();
    println!("{}", total);
    println!("TIME_MS:{}", ms);
    black_box(total);
}
