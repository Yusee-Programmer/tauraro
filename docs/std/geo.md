# std.geo -- geodesy, location encodings, projections, geometry

```tauraro
from std.geo import Geo, Ellipsoid, Geohash, PlusCode, Utm, Mgrs, WebMercator, Tiles, Ecef, Enu, Geometry, Wkt, GeoJson
```

Angles are degrees, distances metres, `(x, y)` = `(longitude, latitude)`. Everything is pure Tauraro except
the ellipsoidal geodesic solver, which is Charles Karney's GeographicLib C library (v2.2, MIT, vendored in
`std/geo/vendor/`, linked through `std/geo/geo_native.c`). Sub-modules: `std.geo.geodesy`, `std.geo.encode`,
`std.geo.proj`, `std.geo.geometry`.

## Distances and bearings

Sphere (`Geo`, mean radius 6371008.8 m): `haversine(lat1, lon1, lat2, lon2)`, `haversine_r(..., radius)`,
`initial_bearing`, `final_bearing`, `destination(lat, lon, bearing, dist)`, `midpoint`, `intermediate(..., fraction)`,
`cross_track(lat, lon, lat1, lon1, lat2, lon2)` (signed, negative = left), `along_track`,
`bounding_box(lat, lon, radius_m) -> BBox`. A `BBox` that crosses the antimeridian has `min_lon > max_lon`; near a
pole it spans all longitudes and reaches the pole. `BBox.contains(lat, lon)` honours the wrap.

Ellipsoid (`Ellipsoid.wgs84()`, `.grs80()`, `.sphere(r)`, `.init(a, f)`):

| Call | Description |
|---|---|
| `e.inverse(lat1, lon1, lat2, lon2) -> GeoInverse` | Karney. `distance`, `azi1`, `azi2`, `arc`, `reduced_length`, `area`. Accurate to about 15 nm for every pair, antipodes and poles included. |
| `e.direct(lat, lon, azi, dist) -> GeoDirect` | Karney. `lat`, `lon`, `azi2`. |
| `e.distance(...)`, `e.midpoint(...)`, `e.geodesic_points(..., n)` | Convenience on top of the above (`n + 1` evenly spaced points). |
| `e.path_length(lats, lons)`, `e.polygon_area(lats, lons)`, `polygon_area_signed`, `polygon_perimeter` | Geodesic polyline length and polygon area (Karney); rings around a pole work. |
| `e.vincenty_inverse(...)?` | Vincenty (1975), pure Tauraro, converges to 1e-12 rad. Raises for nearly antipodal points where the iteration fails -- use `inverse` there. |
| `e.vincenty_direct(...)` | Vincenty direct; always converges. |

## Encodings

* `Geohash.encode(lat, lon, precision)`, `decode(hash)? -> GeohashCell` (centre, errors, bounds), `neighbor(hash, "n"|"ne"|...)?`,
  `neighbors(hash)?` (n, ne, e, se, s, sw, w, nw). Longitude wraps at the antimeridian; past a pole the neighbour is `""`.
* `PlusCode.encode(lat, lon, length)?`, `decode(code)? -> CodeArea`, `is_valid/is_short/is_full`, `shorten(code, lat, lon)?`,
  `recover_nearest(short, lat, lon)?` -- Open Location Code, 2..15 digits.
* `Utm.from_latlon(lat, lon)? -> UtmCoord` (zone, band, easting, northing, convergence, scale), `from_latlon_zone`,
  `to_latlon(zone, northern, e, n)`, `zone_for` (Norway 32V and Svalbard 31X/33X/35X/37X exceptions), `band_for`. Krüger series
  to 6th order. Latitudes -80..84 only; the polar UPS grids are not implemented.
* `Mgrs.from_latlon(lat, lon, digits)?` (0..5 digits per axis), `to_latlon(ref)?`, `to_utm(ref)?`, `from_utm(u, digits)`.
  `to_latlon` returns the south-west corner of the referenced cell. H3 is not provided.

## Projections and frames

* `WebMercator.forward(lat, lon) -> Xy` / `inverse(x, y)` (EPSG:3857; latitude clamped to +-85.0511287798066).
* `Tiles.from_latlon(lat, lon, zoom) -> Tile`, `Tiles.bounds(x, y, zoom) -> BBox`, `quadkey`, `from_quadkey(qk)?`, `pixel(lat, lon, zoom, tile_size)`,
  `parent`, `children`.
* `Ecef.from_geodetic(e, lat, lon, h) -> Vec3`, `Ecef.to_geodetic(e, x, y, z) -> Geodetic` (iterative, sub-micrometre).
* `Enu.from_geodetic / to_geodetic / from_ecef / to_ecef` for a local east-north-up frame, and `Enu.aer(...)` (azimuth, elevation, range).

## Geometry, WKT, GeoJSON

A `Geometry` is Point, LineString, Polygon (exterior + holes), MultiPoint, MultiLineString or MultiPolygon, with optional z.
Build with `Geometry.point/line_string/polygon` or `Geometry.empty(kind)` + `add`, `end_ring`, `end_polygon`.

* `g.contains(x, y)` -- point-in-polygon, even-odd, holes excluded, MultiPolygon aware.
* `g.area_geodesic(e)`, `g.area_sphere(r)` (exact spherical excess), `g.length_geodesic(e)`, `g.bbox()`.
* `g.simplify(tolerance)` / `Simplify.douglas_peucker(xs, ys, tol)` -- Douglas-Peucker (planar, iterative); closed rings stay closed.
* `Wkt.parse(s)?` / `g.to_wkt()` -- all six types, `Z`/`M`/`ZM`, `EMPTY`, lower case, MULTIPOINT with or without inner parentheses.
  Numbers are written as the shortest text that round-trips.
* `GeoJson.parse_geometry(s)?`, `GeoJson.parse(s)? -> FeatureCollection` (accepts a FeatureCollection, a Feature or a bare geometry), `g.to_geojson()`,
  `Feature.to_geojson()`, `FeatureCollection.to_geojson()`. Feature `properties` and `id` are kept as compact JSON text. The input is syntax-checked first
  (std.encoding.json assumes well-formed input). `GeometryCollection` is not supported.

## Accuracy (tests/lang/146_geo.tr)

Compared with GeographicLib 2.1 (Python), PROJ 9 (pyproj 3.8), the `mgrs` package and the open-location-code test CSVs
(encoding 367, decoding 447, short-code and validity rows, all pass):

| Quantity | Worst error |
|---|---|
| Karney inverse distance / azimuth (120 random incl. near-antipodal and polar pairs) | 0 m / 3e-13 deg |
| Karney direct (60 pairs) | 3e-14 deg |
| Vincenty inverse where it converges (111 of 120) | 5e-5 m (9 near-antipodal pairs raise) |
| Geodesic polygon area / spherical area | 1.4e-15 / 2.5e-14 relative |
| UTM vs PROJ etmerc (72 points) | 5e-9 m |
| Web Mercator vs PROJ | 1.5e-8 m |
| ECEF forward / inverse | 0 / 2e-9 m |

Vincenty's iteration cannot be trusted close to the antipode (it oscillates or converges to the wrong geodesic); the
implementation raises `"vincenty: no convergence ..."` instead of returning a wrong number. Use `Ellipsoid.inverse` (Karney) for general work.
