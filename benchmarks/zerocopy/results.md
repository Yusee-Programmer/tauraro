# Zero-copy vs ARC -- benchmark results

ARC variant = by-value / copying. Zero-copy variant = ref borrows / StrView /
borrowed payloads, compiled with --strict. Lower is better in every column.

| Case | Variant | Time (ms) | Peak (KB) | Live allocs | Live strs |
|------|---------|----------:|----------:|------------:|----------:|
| str_view | arc | 734 | 107392 | 200006 | 100003 |
| str_view | zc | 7 | 6212 | 4 | 3 |
| str_pass | arc | 10 | 3488 | 3 | 3 |
| str_pass | zc | 10 | 3488 | 3 | 3 |
| list_iter | arc | 143 | 3508 | 1004 | 1002 |
| list_iter | zc | 253 | 3512 | 1004 | 1002 |
| class_pass | arc | 8 | 3488 | 4 | 3 |
| class_pass | zc | 21 | 3492 | 4 | 3 |
| enum_payload | arc | 182 | 24068 | 400002 | 200002 |
| enum_payload | zc | 8 | 10828 | 2 | 2 |
| dict_pass | arc | 3418 | 99424 | 2009504 | 2000502 |
| dict_pass | zc | 3382 | 99428 | 2009504 | 2000502 |
| interface | arc | 241 | 3488 | 4 | 3 |
| interface | zc | 210 | 3488 | 4 | 3 |

## Ratio (ARC / Zero-copy) -- higher means zero-copy wins more

| Case | Speedup | Mem ratio | Strs ratio |
|------|--------:|----------:|-----------:|
| str_view | 104.86x | 17.29x | 33334.33x |
| str_pass | 1x | 1x | 1x |
| list_iter | 0.57x | 1x | 1x |
| class_pass | 0.38x | 1x | 1x |
| enum_payload | 22.75x | 2.22x | 100001x |
| dict_pass | 1.01x | 1x | 1x |
| interface | 1.15x | 1x | 1x |
