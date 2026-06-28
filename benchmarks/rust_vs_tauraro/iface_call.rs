use std::time::Instant;
use std::hint::black_box;
trait Shape { fn area(&self) -> i64; }
struct Square { side: i64 }
impl Shape for Square { fn area(&self) -> i64 { self.side * self.side } }
fn total_area(s: &dyn Shape, n: i64) -> i64 { s.area() + n }
fn main() {
    let sq = Square { side: 7 };
    let s: &dyn Shape = &sq;
    let mut total: i64 = 0;
    let start = Instant::now();
    for i in 0..20_000_000i64 {
        total += total_area(s, i % 5);
    }
    let ms = start.elapsed().as_millis();
    println!("{}", total);
    println!("TIME_MS:{}", ms);
    black_box(total);
}
