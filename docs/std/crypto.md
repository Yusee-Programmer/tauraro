# std.crypto — Cryptographic Primitives

```tauraro
from std.crypto.hash import Hash
from std.crypto.hmac import Hmac
from std.crypto.uuid import UUID, ULID, MonotonicUlid
```

> SHA-256, HMAC-SHA256, and MD5 are implemented in pure C — no external library required.
> UUID v4 uses `/dev/urandom` on POSIX and `rand()` on Windows.

---

## std.crypto.hash — Hash

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Hash.sha256` | `(s: str) -> str` | `str` | SHA-256 digest of a null-terminated string. Returns 64-char lowercase hex. |
| `Hash.sha256_bytes` | `(data: str, len_: int) -> str` | `str` | SHA-256 of exactly `len_` bytes. Returns 64-char lowercase hex. |
| `Hash.md5` | `(s: str) -> str` | `str` | MD5 digest. Returns 32-char lowercase hex. **Not secure** — use for checksums only. |

### Example

```tauraro
from std.crypto.hash import Hash

mut h = Hash.sha256("hello")
print(h)
# 2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824

mut h2 = Hash.sha256_bytes("hello world", 11)
print(h2)
# b94d27b9934d3e08a52e52d7da7dabfac484efe04294e576f3a7a4e0c7e8b1a (example)

mut m = Hash.md5("hello")
print(m)
# 5d41402abc4b2a76b9719d911017c592
```

---

## std.crypto.hmac — Hmac

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Hmac.sha256` | `(key: str, klen: int, msg: str) -> str` | `str` | HMAC-SHA256. `klen` bytes of `key`. Returns 64-char lowercase hex. |
| `Hmac.sha256_str` | `(key: str, msg: str) -> str` | `str` | Same, but `key` is treated as null-terminated. |

### Example

```tauraro
from std.crypto.hmac import Hmac

mut tag = Hmac.sha256("secret", 6, "message")
print(tag)

mut tag2 = Hmac.sha256_str("mysecret", "data")
print(tag2)
```

---

## std.crypto.uuid — UUID

| Method | Signature | Returns | Description |
|---|---|---|---|
| `UUID.v4` | `() -> str` | `str` | Random UUID v4 in canonical `8-4-4-4-12` hex format. |
| `UUID.v3` | `(namespace: str, name: str) -> str` | `str` | Namespace + name based UUID v3 (MD5). Deterministic: same namespace+name always produces the same UUID. |
| `UUID.v5` | `(namespace: str, name: str) -> str` | `str` | Namespace + name based UUID v5 (SHA-1). Preferred over v3 (MD5 is weaker); same determinism guarantee. |
| `UUID.NAMESPACE_DNS` / `_URL` / `_OID` / `_X500` | `() -> str` | `str` | RFC 4122 standard namespace UUIDs, for use as `UUID.v3`/`v5`'s `namespace` argument. |
| `UUID.nil` | `() -> str` | `str` | The nil UUID (`00000000-0000-0000-0000-000000000000`). |
| `UUID.is_valid` | `(s: str) -> bool` | `bool` | Whether `s` is a syntactically valid UUID string. |
| `UUID.version` | `(s: str) -> int` | `int` | The UUID's version nibble (4, 3, 5, ...), or -1 if invalid. |
| `UUID.equals` | `(a: str, b: str) -> bool` | `bool` | Case-insensitive UUID string equality. |

### Example

```tauraro
from std.crypto.uuid import UUID

mut id = UUID.v4()
print(id)
# e.g. "550e8400-e29b-41d4-a716-446655440000"

mut dns_id = UUID.v5(UUID.NAMESPACE_DNS(), "example.com")
print(dns_id)
# deterministic -- same namespace + name always produces this same UUID
```

---

## std.crypto.uuid — ULID

Lexicographically sortable, timestamp-prefixed identifiers (26 Crockford
base32 characters: 48-bit millisecond timestamp + 80 bits of randomness).

| Method | Signature | Returns | Description |
|---|---|---|---|
| `ULID.generate` | `() -> str` | `str` | A fresh ULID using the current time + fresh randomness. |
| `ULID.from_parts` | `(ms: int, rand_hi_lo: (int, int)) -> str` | `str` | Build a ULID from an explicit millisecond timestamp and randomness (for deterministic tests). |
| `ULID.is_valid` | `(s: str) -> bool` | `bool` | Whether `s` is a syntactically valid ULID string. |
| `ULID.timestamp_ms` | `(s: str) -> int` | `int` | Extract the millisecond timestamp encoded in a ULID. |

`MonotonicUlid` guarantees strictly increasing ULIDs even when multiple are
generated within the same millisecond (bumps the random component instead
of colliding):

```tauraro
from std.crypto.uuid import ULID, MonotonicUlid

mut a = ULID.generate()
mut b = ULID.generate()
# a and b sort lexicographically by generation time, but two ULIDs generated
# in the same millisecond via ULID.generate() alone are not guaranteed ordered

mut gen = MonotonicUlid.init()
mut u1 = gen.next()
mut u2 = gen.next()
# u1 < u2 always, even generated back-to-back in the same millisecond
```
