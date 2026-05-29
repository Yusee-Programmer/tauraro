// Benchmark 4: XOR Shift PRNG — 1B xorshift64 steps
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let mut s: u64 = 0x123456789ABCDEF0;
    for _ in 0..1_000_000_000 {
        s ^= s << 13;
        s ^= s >> 7;
        s ^= s << 17;
    }

    let ms = t0.elapsed().as_millis();
    println!("{}", s);
    println!("TIME_MS:{}", ms);
}
