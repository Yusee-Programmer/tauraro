# Rust vs Tauraro -- zero-copy hot paths

Both do genuine zero-copy (borrowed slices / borrowed payloads). Tauraro is
built with --strict (the borrows are compile-time proven). Lower is better.

| Case | Rust time (ms) | Rust peak (KB) | Tauraro time (ms) | Tauraro peak (KB) |
|------|---------------:|---------------:|------------------:|------------------:|
| str_view | 3 | 6244 | 4 | 6200 |
| enum_payload | 8 | 10852 | 10 | 10824 |
