// Benchmark 2: Fibonacci — 1B iterative steps
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let (mut a, mut b): (i64, i64) = (0, 1);
    for _ in 0..1_000_000_000 {
        let c = a.wrapping_add(b);
        a = b;
        b = c;
    }

    let ms = t0.elapsed().as_millis();
    println!("{}", b);
    println!("TIME_MS:{}", ms);
}
