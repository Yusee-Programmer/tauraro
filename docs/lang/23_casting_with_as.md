# 23 — Casting with `as` (the complete reference)

`expr as T` converts a value to type `T`. Tauraro compiles to C, so `as` is either a **numeric
conversion** (`(long long)x`, `(double)x`, …) or a **zero-cost pointer reinterpretation**
(`(T*)x`) — never a checked conversion. This page lists **every** case where you need `as`,
**why**, and **the exact error you get without it**. It matters most in the areas that touch raw
memory: FFI, systems programming, and bare-metal.

> Rule of thumb: Tauraro is permissive with **numbers** (C-style implicit conversions) but strict
> with **pointers and strings**, because the C compiler rejects mismatched pointer/struct types.
> When a value crosses a *type-kind* boundary — string↔pointer, pointer↔pointer, pointer↔integer,
> function↔pointer — you almost always need `as`.

---

## Quick reference

| Cast | Required? | Without the cast you get |
|---|---|---|
| `str as Pointer[char]` (pass a string to C `char*`) | **Required** | C: `error: incompatible type for argument …` (a `str` is the `TrStr` struct, not `char*`) |
| `Pointer[A] as Pointer[B]` (reinterpret) | **Required** | C: `error: … from incompatible pointer type [-Wincompatible-pointer-types]` |
| `fn as Pointer[void]` (pass a function as a callback) | **Required** | Type mismatch — a `def` value isn't a `Pointer` |
| `p as Pointer[void]` / `vp as Pointer[T]` (FFI userdata / handles) | **Required** | C: incompatible pointer type |
| `p as usize` (address as integer: compare, log, align) | **Required** for arithmetic | C: `-Wint-conversion` / invalid operands to `%`/`&` on a pointer |
| `addr as Pointer[T]` (integer → pointer, e.g. MMIO) | **Required** | Can't index/deref an integer |
| `List/Vec as Pointer[T]` (decay to the data array) | **Required** | Passing a container where a pointer is expected |
| `c_long`/`c_int` value `as int` (to print it) | **Required to print** | C: `_Generic`/`_tr__ptr_s` picks the wrong branch → compile error |
| `x as u8`/`as i32` (narrowing an integer) | Recommended | Compiles; `-Woverflow` if a constant doesn't fit — the cast makes the truncation explicit |
| `x as int` / `y as float` (int↔float) | Optional | Compiles (implicit C conversion); the cast documents intent and the rounding/truncation |

Everything below expands these rows with runnable snippets.

---

## 1. Numbers — usually implicit, cast for intent

Integer↔float and integer widening are **implicit** (C rules), so `as` here is about clarity and
controlling truncation, not satisfying the compiler.

```python
mut i = 10
mut f = 3.9
print(i + (f as int))        # 13 — `as int` truncates toward zero (3.9 -> 3)
print((i as float) / 4.0)    # 2.5 — widen before dividing, else integer division

mut big = 300
mut b = big as u8            # 44  — explicit two's-complement narrowing (300 & 0xFF)
```

- `float as int` **truncates** (never rounds). Without the cast, `f(y)` where `y: float` and the
  parameter is `int` still compiles (C truncates implicitly) — but the `as int` says you meant it.
- Narrowing to `u8`/`i16`/`u32`/… performs C's modular truncation. A constant that doesn't fit
  (`mut b: u8 = 300`) compiles with a `-Woverflow` warning; `300 as u8` documents the intent.

## 2. `str` ↔ `Pointer[char]` (FFI strings) — **required**

A Tauraro `str` is a reference-counted `TrStr` *struct*, not a bare `char*`. To hand a string to a
C function taking `char*`/`Pointer[char]`, extract the buffer with `as Pointer[char]`:

```python
extern "C":
    def c_log(msg: Pointer[char]) -> c_int

mut s = "hello"
c_log(s as Pointer[char])       # OK — passes the underlying char*
# c_log(s)                      # ERROR: incompatible type for argument 1 (TrStr vs char*)
```

The reverse — a freshly `malloc`'d `Pointer[char]` returned from C — becomes an **owned** string
with `as str` (the buffer is freed when the string is released):

```python
mut buf: Pointer[char] = read_c_buffer()   # C gave us a malloc'd char*
mut owned = buf as str                      # owns it now; released via ARC
```

> **Lifetime caveat:** `s as Pointer[char]` **borrows** the string's buffer. Keep `s` alive for as
> long as C holds the pointer — if `s` is dropped, the `char*` dangles.

## 3. Pointer ↔ pointer (reinterpret) — **required**

Different `Pointer[T]` types are distinct C pointer types. Reinterpreting one as another needs
`as`, or the C compiler rejects it:

```python
unsafe:
    mut p = alloc[int](4)
    mut bytes = p as Pointer[u8]     # view the ints as raw bytes
    # sink_expecting_u8(p)           # ERROR: incompatible pointer type [-Wincompatible-pointer-types]
    dealloc(p)
```

This is the workhorse of `Pointer[void]` type-erasure at FFI boundaries:

```python
extern "C":
    def register(cb: Pointer[void], userdata: Pointer[void])

register(on_event as Pointer[void], (&state) as Pointer[void])   # erase to void*
# … and cast back inside the callback:  mut s = ud as Pointer[State]
```

## 4. Pointer ↔ integer (`usize`) — addresses, null checks, alignment

`as usize` turns a pointer into its raw address so you can compare, log, or align it. `usize as
Pointer[T]` goes back — this is how you reach a fixed hardware address (see bare-metal below).

```python
unsafe:
    mut p = alloc[int](8)
    if p as usize == 0 as usize:      # null check (the idiom; `0 as usize` keeps both sides usize)
        return
    mut aligned = (p as usize) & (0 as usize - 16 as usize)   # align down to 16
    print(f"addr = {p as usize}")
    dealloc(p)
```

- A plain `p == 0` *does* compile for a raw `Pointer[T]`, but the `p as usize == 0 as usize` form
  is the portable idiom and is **required** when the pointer comes from a struct value — e.g. a
  string's buffer: `(name as Pointer[char]) as usize == 0 as usize`.
- **Arithmetic** on an address (`%`, `&`, `<<`) requires `as usize` first — C forbids those
  operators on a pointer.

## 5. Function → `Pointer[void]` (callbacks) — **required**

A top-level function is a C function; pass it to a C callback slot with `as Pointer[void]` (see
`17_extern_and_ffi.md` → *Passing callbacks to C*):

```python
run_events(on_event as Pointer[void], (&acc) as Pointer[void], 1, 2, 3)
#          ^ function as void*        ^ state as void*
```

## 6. Containers → `Pointer[T]` (decay) — **required**

A `List[T]`/`Vec[T]` decays to its underlying data array with `as Pointer[T]`, for handing a
buffer to C:

```python
mut xs: Vec[u8] = make_buffer()
c_write(fd, xs as Pointer[u8], xs.len)     # passes xs->data
```

## 7. Printing `c_*` FFI scalars — `as int`

The `c_*` ABI-exact types (`c_int`, `c_long`, …) map to C `int`/`long`, which the auto-formatter
doesn't dispatch. To **print** one, cast to `int` first:

```python
mut sec = tv.tv_sec        # c_long
print(sec as int)          # cast to print; without it: a C _Generic/compile error
```

Their normal use (FFI signatures, struct-layout matching) needs no cast.

---

## Systems programming

```python
# Reinterpret a struct as bytes for hashing / serialization:
unsafe:
    mut hdr = Header()
    mut raw = (&hdr) as Pointer[u8]
    mut i = 0
    while i < sizeof(Header):
        checksum = checksum ^ (raw.offset(i).read() as int)
        i = i + 1
```

- `(&hdr) as Pointer[u8]` — address-of + reinterpret to a byte pointer (**required**: `Pointer[Header]`≠`Pointer[u8]`).
- `.read() as int` — a `u8` read widened to `int` for arithmetic (recommended for clarity).

## FFI

Everything in sections 2–7 is FFI. The three you will hit constantly:
1. `str as Pointer[char]` to pass strings,
2. `something as Pointer[void]` to erase types at the boundary (and `void_ptr as Pointer[T]` to recover them),
3. `fn as Pointer[void]` for callbacks.

Match integer widths with the `c_*` family (`c_int`, `c_size_t`, …) so you rarely need integer
casts across the boundary — see `17_extern_and_ffi.md`.

## Bare-metal / MMIO

Memory-mapped registers live at fixed addresses. Turn the address into a pointer with `as
Pointer[T]`, then read/write in `unsafe:`:

```python
# GPIO output register at a fixed address
def gpio_set(pin: int):
    unsafe:
        mut reg = 0x40020014 as Pointer[u32]   # integer -> MMIO pointer (REQUIRED)
        reg.write(reg.read() | (1 as u32 << pin))
```

- `0x40020014 as Pointer[u32]` — the integer→pointer cast is what makes MMIO possible; there is no
  other way to name a fixed address. Under `--no-heap`/`--freestanding` this is the standard idiom.
- Combine with `@value_type` + `Bits[T; N]` (see `17_extern_and_ffi.md`) to model register layouts,
  and `volatile`-style access via `.read()`/`.write()`.

---

## Why the compiler needs these (and what breaks)

Tauraro emits C. The C type system — not Tauraro's — is what rejects the un-cast cases:

- **`str` is a `TrStr` struct.** Passing it where `char*` is expected is a struct-vs-pointer
  mismatch → `error: incompatible type for argument`. `as Pointer[char]` emits the buffer extraction.
- **`Pointer[A]` and `Pointer[B]` are distinct C types** (`A*` vs `B*`) →
  `-Wincompatible-pointer-types` (an error in this codebase's build flags). `as` emits the `(B*)` cast.
- **Pointers aren't integers.** Comparing/doing arithmetic mixes kinds → `-Wint-conversion` or an
  invalid-operands error. `as usize` makes the address a real integer.
- **A function value isn't a `Pointer`.** `as Pointer[void]` takes its address as `void*`.

Because these are **zero-cost reinterpretations**, the compiler inserts no runtime checks — an
incorrect cast (wrong target type, an integer that isn't a valid address, a borrowed `char*` whose
string was dropped) is undefined behavior, exactly like C. That is why every raw-pointer operation
also requires an `unsafe:` block (`[P-1]`/`[U-1]`; see `14_unsafe_and_pointers.md`). Keep casts at
the FFI/hardware boundary and convert back to safe types (`str`, `@value_type`, `Box[T]`) as soon
as you're back in normal Tauraro code.

## See also

- [14 — Unsafe and Pointers](14_unsafe_and_pointers.md) — `unsafe:`, `Pointer[T]`, `alloc`/`dealloc`.
- [17 — Extern and FFI](17_extern_and_ffi.md) — `c_*` types, struct layout, callbacks.
- [22 — Compiling & Cross-Compilation](22_compiling_and_cross_compilation.md) — `--no-heap`, bare-metal.
