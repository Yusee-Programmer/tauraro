# Rust vs Tauraro -- borrow / zero-copy hot paths

Tauraro uses explicit borrow/own/lifetime annotations, built --strict (borrows
compile-time proven; proven borrows elide ARC). Lower is better. See README.md
for the case descriptions and honest interpretation.

| Case | Rust time (ms) | Rust peak (KB) | Tauraro time (ms) | Tauraro peak (KB) |
|------|---------------:|---------------:|------------------:|------------------:|
| str_view | 3 | 0 | 4 | 0 |
| enum_payload | 8 | 0 | 8 | 0 |
| class_field | 304 | 0 | 235 | 0 |
| list_sum | 141 | 0 | 224 | 0 |
| dict_borrow | 688 | 0 | 725 | 0 |
| value_dict | 688 | 0 | 686 | 0 |
| iface_call | 77 | 0 | 70 | 0 |
