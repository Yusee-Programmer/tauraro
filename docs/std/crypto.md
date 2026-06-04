# std.crypto — Cryptographic Primitives

```tauraro
from std.crypto.hash import Hash
from std.crypto.hmac import Hmac
from std.crypto.uuid import UUID
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

### Example

```tauraro
from std.crypto.uuid import UUID

mut id = UUID.v4()
print(id)
# e.g. "550e8400-e29b-41d4-a716-446655440000"
```
