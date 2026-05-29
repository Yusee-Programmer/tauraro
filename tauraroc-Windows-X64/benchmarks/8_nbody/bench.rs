// Benchmark 8: N-Body — 3 bodies, 10M gravitational steps
use std::time::Instant;

fn main() {
    let t0 = Instant::now();

    let dt = 0.001_f64;
    let (m0, m1, m2) = (1.0_f64, 0.001_f64, 0.0003_f64);
    let (mut x0, mut y0, mut z0) = (0.0_f64, 0.0_f64, 0.0_f64);
    let (mut vx0, mut vy0, mut vz0) = (0.0_f64, 0.0_f64, 0.0_f64);
    let (mut x1, mut y1, mut z1) = (4.0_f64, 0.0_f64, 0.0_f64);
    let (mut vx1, mut vy1, mut vz1) = (0.0_f64, 3.0_f64, 0.0_f64);
    let (mut x2, mut y2, mut z2) = (8.0_f64, 0.0_f64, 0.0_f64);
    let (mut vx2, mut vy2, mut vz2) = (0.0_f64, 2.1_f64, 0.0_f64);

    for _ in 0..10_000_000 {
        let (dx, dy, dz) = (x1-x0, y1-y0, z1-z0);
        let r = (dx*dx+dy*dy+dz*dz).sqrt(); let f = dt/(r*r*r);
        vx0+=f*m1*dx; vy0+=f*m1*dy; vz0+=f*m1*dz;
        vx1-=f*m0*dx; vy1-=f*m0*dy; vz1-=f*m0*dz;

        let (dx, dy, dz) = (x2-x0, y2-y0, z2-z0);
        let r = (dx*dx+dy*dy+dz*dz).sqrt(); let f = dt/(r*r*r);
        vx0+=f*m2*dx; vy0+=f*m2*dy; vz0+=f*m2*dz;
        vx2-=f*m0*dx; vy2-=f*m0*dy; vz2-=f*m0*dz;

        let (dx, dy, dz) = (x2-x1, y2-y1, z2-z1);
        let r = (dx*dx+dy*dy+dz*dz).sqrt(); let f = dt/(r*r*r);
        vx1+=f*m2*dx; vy1+=f*m2*dy; vz1+=f*m2*dz;
        vx2-=f*m1*dx; vy2-=f*m1*dy; vz2-=f*m1*dz;

        x0+=dt*vx0; y0+=dt*vy0; z0+=dt*vz0;
        x1+=dt*vx1; y1+=dt*vy1; z1+=dt*vz1;
        x2+=dt*vx2; y2+=dt*vy2; z2+=dt*vz2;
    }

    let ke = 0.5 * (m0*(vx0*vx0+vy0*vy0+vz0*vz0)
                  + m1*(vx1*vx1+vy1*vy1+vz1*vz1)
                  + m2*(vx2*vx2+vy2*vy2+vz2*vz2));

    let ms = t0.elapsed().as_millis();
    println!("{:.6}", ke);
    println!("TIME_MS:{}", ms);
}
