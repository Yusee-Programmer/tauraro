// Benchmark 5: Newton Sqrt — 1B Newton's method iterations
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let mut x: f64 = 1.5;
    for _ in 0..1_000_000_000 {
        x = (x + 2.0 / x) * 0.5;
    }

    let ms = t0.elapsed().as_millis();
    println!("{:.15}", x);
    println!("TIME_MS:{}", ms);
}
