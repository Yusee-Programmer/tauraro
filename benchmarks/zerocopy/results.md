# Zero-copy vs ARC -- benchmark results

ARC variant = by-value / copying. Zero-copy variant = ref borrows / StrView /
borrowed payloads, compiled with --strict. Lower is better in every column.

| Case | Variant | Time (ms) | Peak (KB) | Live allocs | Live strs |
|------|---------|----------:|----------:|------------:|----------:|
| str_view | arc | 348 | 107392 | 200006 | 100003 |
| str_view | zc | 3 | 6172 | 4 | 3 |
| str_pass | arc | 7 | 3492 | 3 | 3 |
| str_pass | zc | 6 | 3488 | 3 | 3 |
| list_iter | arc | 91 | 3516 | 1004 | 1002 |
| list_iter | zc | 64 | 3180 | 1004 | 1002 |
| class_pass | arc | 6 | 3492 | 4 | 3 |
| class_pass | zc | 6 | 3488 | 4 | 3 |
| enum_payload | arc | 107 | 24080 | 400002 | 200002 |
| enum_payload | zc | 7 | 10820 | 2 | 2 |
| dict_pass | arc | 2632 | 99428 | 2009504 | 2000502 |
| dict_pass | zc | 2800 | 99428 | 2009504 | 2000502 |
| interface | arc | 199 | 3488 | 4 | 3 |
| interface | zc | 200 | 3492 | 4 | 3 |

## Ratio (ARC / Zero-copy) -- higher means zero-copy wins more

| Case | Speedup | Mem ratio | Strs ratio |
|------|--------:|----------:|-----------:|
| str_view | 116x | 17.4x | 33334.33x |
| str_pass | 1.17x | 1x | 1x |
| list_iter | 1.42x | 1.11x | 1x |
| class_pass | 1x | 1x | 1x |
| enum_payload | 15.29x | 2.23x | 100001x |
| dict_pass | 0.94x | 1x | 1x |
| interface | 1x | 1x | 1x |
