// Benchmark 3: Float Multiply — 1B f64 multiplications
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let mut x: f64 = 1.0;
    for _ in 0..1_000_000_000 {
        x *= 1.000_001_f64;
    }

    let ms = t0.elapsed().as_millis();
    println!("{:.6}", x);
    println!("TIME_MS:{}", ms);
}
