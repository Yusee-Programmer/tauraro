// Benchmark 9: Collatz — total steps for all n in 1..10,000,000
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let mut total: i64 = 0;
    for n in 1_i64..=10_000_000 {
        let mut x = n;
        while x != 1 {
            if x % 2 == 0 { x /= 2; } else { x = 3 * x + 1; }
            total += 1;
        }
    }

    let ms = t0.elapsed().as_millis();
    println!("{}", total);
    println!("TIME_MS:{}", ms);
}
