use std::time::Instant;
use std::hint::black_box;
struct Point { x: i64, y: i64 }
fn dist2(p: &Point) -> i64 { p.x * p.x + p.y * p.y }
fn main() {
    let p = Point { x: 3, y: 4 };
    let mut total: i64 = 0;
    let start = Instant::now();
    for i in 0..50_000_000i64 {
        total += dist2(&p) + (i % 7);
    }
    let ms = start.elapsed().as_millis();
    println!("{}", total);
    println!("TIME_MS:{}", ms);
    black_box(total);
}
