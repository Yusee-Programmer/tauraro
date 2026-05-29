/* Benchmark 8: N-Body — 3 bodies, 10M gravitational steps */
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    double dt = 0.001;
    double m0 = 1.0, m1 = 0.001, m2 = 0.0003;
    double x0=0,y0=0,z0=0, vx0=0,vy0=0,vz0=0;
    double x1=4,y1=0,z1=0, vx1=0,vy1=3.0,vz1=0;
    double x2=8,y2=0,z2=0, vx2=0,vy2=2.1,vz2=0;

    for (int step = 0; step < 10000000; step++) {
        double dx, dy, dz, r, f;

        dx=x1-x0; dy=y1-y0; dz=z1-z0;
        r=sqrt(dx*dx+dy*dy+dz*dz); f=dt/(r*r*r);
        vx0+=f*m1*dx; vy0+=f*m1*dy; vz0+=f*m1*dz;
        vx1-=f*m0*dx; vy1-=f*m0*dy; vz1-=f*m0*dz;

        dx=x2-x0; dy=y2-y0; dz=z2-z0;
        r=sqrt(dx*dx+dy*dy+dz*dz); f=dt/(r*r*r);
        vx0+=f*m2*dx; vy0+=f*m2*dy; vz0+=f*m2*dz;
        vx2-=f*m0*dx; vy2-=f*m0*dy; vz2-=f*m0*dz;

        dx=x2-x1; dy=y2-y1; dz=z2-z1;
        r=sqrt(dx*dx+dy*dy+dz*dz); f=dt/(r*r*r);
        vx1+=f*m2*dx; vy1+=f*m2*dy; vz1+=f*m2*dz;
        vx2-=f*m1*dx; vy2-=f*m1*dy; vz2-=f*m1*dz;

        x0+=dt*vx0; y0+=dt*vy0; z0+=dt*vz0;
        x1+=dt*vx1; y1+=dt*vy1; z1+=dt*vz1;
        x2+=dt*vx2; y2+=dt*vy2; z2+=dt*vz2;
    }

    double ke = 0.5*(m0*(vx0*vx0+vy0*vy0+vz0*vz0)
                   + m1*(vx1*vx1+vy1*vy1+vz1*vz1)
                   + m2*(vx2*vx2+vy2*vy2+vz2*vz2));

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);
    printf("%.6f\n", ke);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
