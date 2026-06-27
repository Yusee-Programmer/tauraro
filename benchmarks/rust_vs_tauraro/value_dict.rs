use std::time::Instant;
use std::collections::HashMap;
use std::hint::black_box;
#[derive(Clone, Copy)]
struct Point { x: i64, y: i64 }
fn main() {
    let mut total: i64 = 0;
    let start = Instant::now();
    for _ in 0..200_000 {
        let mut d: HashMap<String, Point> = HashMap::new();
        d.insert("a".to_string(), Point { x: 1, y: 2 });
        d.insert("b".to_string(), Point { x: 3, y: 4 });
        d.insert("c".to_string(), Point { x: 5, y: 6 });
        let pa = d["a"];
        let pc = d["c"];
        total += pa.x + pa.y + pc.x + pc.y;
        black_box(&d);
    }
    let ms = start.elapsed().as_millis();
    println!("{}", total);
    println!("TIME_MS:{}", ms);
    black_box(total);
}
