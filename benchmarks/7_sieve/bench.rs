// Benchmark 7: Sieve of Eratosthenes — primes up to 50,000,000
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let n = 50_000_000usize;
    let mut sieve = vec![false; n + 1];
    sieve[0] = true;
    sieve[1] = true;
    let mut i = 2;
    while i * i <= n {
        if !sieve[i] {
            let mut j = i * i;
            while j <= n { sieve[j] = true; j += i; }
        }
        i += 1;
    }
    let count = (2..=n).filter(|&i| !sieve[i]).count();

    let ms = t0.elapsed().as_millis();
    println!("{}", count);
    println!("TIME_MS:{}", ms);
}
