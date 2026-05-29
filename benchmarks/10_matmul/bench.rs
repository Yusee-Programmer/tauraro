// Benchmark 10: Matrix Multiply — naive 400x400 f64 (A * B = C)
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let n = 400usize;
    let nn = n * n;
    let mut a = vec![0.0_f64; nn];
    let mut b = vec![0.0_f64; nn];
    let mut c = vec![0.0_f64; nn];

    for i in 0..nn {
        a[i] = i as f64 * 0.000_001;
        b[i] = (nn - i) as f64 * 0.000_001;
    }

    for i in 0..n {
        for k in 0..n {
            let aik = a[i * n + k];
            for j in 0..n {
                c[i * n + j] += aik * b[k * n + j];
            }
        }
    }

    let trace: f64 = (0..n).map(|i| c[i * n + i]).sum();

    let ms = t0.elapsed().as_millis();
    println!("{:.6}", trace);
    println!("TIME_MS:{}", ms);
}
