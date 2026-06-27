use std::time::Instant;
use std::collections::HashMap;
use std::hint::black_box;
#[inline(never)]
fn sum_lens(d: &HashMap<String, String>) -> u64 {
    let mut s: u64 = 0;
    for k in d.keys() {
        let v: &str = &d[k];          // borrowed &str, no allocation
        s += v.len() as u64;
    }
    s
}
fn main() {
    let mut d: HashMap<String, String> = HashMap::new();
    d.insert("alpha".to_string(), "hello".to_string());
    d.insert("beta".to_string(), "world".to_string());
    d.insert("gamma".to_string(), "tauraro".to_string());
    d.insert("delta".to_string(), "zerocopy".to_string());
    let mut total: u64 = 0;
    let start = Instant::now();
    for _ in 0..1_000_000 {
        total += sum_lens(black_box(&d));
    }
    let ms = start.elapsed().as_millis();
    println!("{}", total);
    println!("TIME_MS:{}", ms);
    black_box(total);
}
