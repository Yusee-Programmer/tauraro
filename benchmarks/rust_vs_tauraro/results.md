# Rust vs Tauraro -- zero-copy hot paths

Both do genuine zero-copy (borrowed slices / borrowed payloads). Tauraro is
built with --strict (the borrows are compile-time proven). Lower is better.

| Case | Rust time (ms) | Rust peak (KB) | Tauraro time (ms) | Tauraro peak (KB) |
|------|---------------:|---------------:|------------------:|------------------:|
| str_view | 3 | 6260 | 19 | 7668 |
| enum_payload | 9 | 11164 | 9 | 10816 |
