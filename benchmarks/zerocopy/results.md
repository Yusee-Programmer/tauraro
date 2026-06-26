# Zero-copy vs ARC -- benchmark results

ARC variant = by-value / copying. Zero-copy variant = ref borrows / StrView /
borrowed payloads, compiled with --strict. Lower is better in every column.

| Case | Variant | Time (ms) | Peak (KB) | Live allocs | Live strs |
|------|---------|----------:|----------:|------------:|----------:|
| str_view | arc | 529 | 107372 | 200006 | 100003 |
| str_view | zc | 15 | 6844 | 100006 | 3 |
| str_pass | arc | 5 | 3480 | 3 | 3 |
| str_pass | zc | 8 | 3476 | 3 | 3 |
| list_iter | arc | 147 | 3500 | 1004 | 1002 |
| list_iter | zc | 171 | 3496 | 1004 | 1002 |
| class_pass | arc | 7 | 3480 | 4 | 3 |
| class_pass | zc | 5 | 3476 | 4 | 3 |
| enum_payload | arc | 162 | 24072 | 400002 | 200002 |
| enum_payload | zc | 8 | 10820 | 2 | 2 |
| dict_pass | arc | 2454 | 99532 | 2009504 | 2000502 |
| dict_pass | zc | 3272 | 99532 | 2009504 | 2000502 |
| interface | arc | 238 | 3480 | 4 | 3 |
| interface | zc | 162 | 3484 | 4 | 3 |

## Ratio (ARC / Zero-copy) -- higher means zero-copy wins more

| Case | Speedup | Mem ratio | Strs ratio |
|------|--------:|----------:|-----------:|
| str_view | 35.27x | 15.69x | 33334.33x |
| str_pass | 0.62x | 1x | 1x |
| list_iter | 0.86x | 1x | 1x |
| class_pass | 1.4x | 1x | 1x |
| enum_payload | 20.25x | 2.22x | 100001x |
| dict_pass | 0.75x | 1x | 1x |
| interface | 1.47x | 1x | 1x |
