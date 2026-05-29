// Benchmark 1: Integer Sum — sum 0..999_999_999 (1B additions)
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let mut sum: i64 = 0;
    for i in 0i64..1_000_000_000 {
        sum += i;
    }

    let ms = t0.elapsed().as_millis();
    println!("{}", sum);
    println!("TIME_MS:{}", ms);
}
