/*
 * std/geo/geo_native.c -- Karney geodesics for std.geo.
 *
 * Linked through `# tauraro-cpp-shim: geo_native.c` in std/geo/native.tr.
 * Wraps the vendored GeographicLib C library (vendor/geodesic.c, MIT,
 * Charles Karney) behind flat functions that write their results into a
 * caller-provided double array, so the Tauraro side never sees the structs.
 */
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vendor/geodesic.c"

/* out: s12, azi1, azi2, a12, m12, M12, M21, S12 */
void trc_geo_inverse(double a, double f, double lat1, double lon1,
                     double lat2, double lon2, double* out) {
    struct geod_geodesic g;
    geod_init(&g, a, f);
    out[3] = geod_geninverse(&g, lat1, lon1, lat2, lon2, &out[0], &out[1], &out[2],
                             &out[4], &out[5], &out[6], &out[7]);
}

/* out: lat2, lon2, azi2, a12, m12, M12, M21, S12 */
void trc_geo_direct(double a, double f, double lat1, double lon1,
                    double azi1, double s12, double* out) {
    struct geod_geodesic g;
    double s12o = 0;
    geod_init(&g, a, f);
    out[3] = geod_gendirect(&g, lat1, lon1, azi1, GEOD_NOFLAGS, s12,
                            &out[0], &out[1], &out[2], &s12o,
                            &out[4], &out[5], &out[6], &out[7]);
}

/* Area (signed, counter-clockwise positive) and perimeter of a polygon given
   as n vertices; out: area, perimeter. */
void trc_geo_polygon_area(double a, double f, const double* lats,
                          const double* lons, int64_t n, double* out) {
    struct geod_geodesic g;
    geod_init(&g, a, f);
    geod_polygonarea(&g, (double*)lats, (double*)lons, (int)n, &out[0], &out[1]);
}

/* Perimeter of a polyline (open path). */
double trc_geo_polyline_length(double a, double f, const double* lats,
                               const double* lons, int64_t n) {
    struct geod_geodesic g;
    struct geod_polygon p;
    double area = 0, perim = 0;
    int64_t i;
    geod_init(&g, a, f);
    geod_polygon_init(&p, 1);
    for (i = 0; i < n; ++i) geod_polygon_addpoint(&g, &p, lats[i], lons[i]);
    geod_polygon_compute(&g, &p, 0, 1, &area, &perim);
    return perim;
}

/* n+1 equally spaced points (by distance) along the geodesic from point 1
   to point 2, endpoints included. */
void trc_geo_line_points(double a, double f, double lat1, double lon1,
                         double lat2, double lon2, int64_t n,
                         double* olat, double* olon) {
    struct geod_geodesic g;
    struct geod_geodesicline l;
    int64_t i;
    geod_init(&g, a, f);
    geod_inverseline(&l, &g, lat1, lon1, lat2, lon2, GEOD_LATITUDE | GEOD_LONGITUDE | GEOD_DISTANCE_IN);
    for (i = 0; i <= n; ++i) {
        double azi;
        geod_position(&l, l.s13 * (double)i / (double)n, &olat[i], &olon[i], &azi);
    }
}

/* Shortest "%.{p}g" text (p = 1..17) that parses back to exactly x.
   out must hold 40 bytes; returns the length. */
int64_t trc_geo_fmt(double x, char* out) {
    int p, n = 0;
    double ax = fabs(x);
    if (x != x) { memcpy(out, "NaN", 4); return 3; }
    if (x == 0) { memcpy(out, "0", 2); return 1; }
    for (p = 1; p <= 17; ++p) {
        n = snprintf(out, 40, "%.*g", p, x);
        if (strtod(out, NULL) == x) break;
    }
    if (strchr(out, 'e') && ax >= 1e-5 && ax < 1e16) {
        /* positional form with the same significant digits */
        int mag = (int)floor(log10(ax));
        int dec = p - 1 - mag;
        if (dec < 0) dec = 0;
        n = snprintf(out, 40, "%.*f", dec, x);
        if (strtod(out, NULL) != x) n = snprintf(out, 40, "%.*f", dec + 1, x);
        if (strchr(out, '.')) {
            while (n > 0 && out[n - 1] == '0') out[--n] = 0;
            if (n > 0 && out[n - 1] == '.') out[--n] = 0;
        }
    }
    return n;
}

/* Parse one number from s[0..n) with strtod (C locale digits only).
   Returns the bytes consumed (0 = no number). */
int64_t trc_geo_parse_num(const char* s, int64_t n, double* out) {
    char buf[64];
    char* end = NULL;
    int64_t i = 0, m;
    while (i < n && i < 63) {
        char c = s[i];
        if ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.' || c == 'e' || c == 'E') { buf[i] = c; ++i; }
        else break;
    }
    if (i == 0) return 0;
    buf[i] = 0;
    *out = strtod(buf, &end);
    m = (int64_t)(end - buf);
    return m;
}
