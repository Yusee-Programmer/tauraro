// Benchmark 6: Mandelbrot — 800x800 grid, 1000 max iterations
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let mut count: i64 = 0;
    for py in 0..800_i64 {
        let cy = (py as f64 / 800.0) * 2.0 - 1.0;
        for px in 0..800_i64 {
            let cx = (px as f64 / 800.0) * 3.5 - 2.5;
            let (mut x, mut y) = (0.0_f64, 0.0_f64);
            let mut iter = 0;
            while iter < 1000 && x * x + y * y < 4.0 {
                let tx = x * x - y * y + cx;
                y = 2.0 * x * y + cy;
                x = tx;
                iter += 1;
            }
            if iter == 1000 { count += 1; }
        }
    }

    let ms = t0.elapsed().as_millis();
    println!("{}", count);
    println!("TIME_MS:{}", ms);
}
