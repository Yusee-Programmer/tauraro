# std.crypto — Cryptographic Primitives

```tauraro
from std.crypto.hash import Hash
from std.crypto.hmac import Hmac
from std.crypto.uuid import UUID, ULID, MonotonicUlid
from std.crypto.aes import Aes
```

> SHA-256, HMAC-SHA256, and MD5 are implemented in pure C — no external library required.
> UUID v4 uses `/dev/urandom` on POSIX and `rand()` on Windows.
> AES is implemented in pure Tauraro (same approach as SHA-256 — a standard
> algorithm written directly in the language, compiled to native code, no
> external library required) and verified against the official NIST/FIPS-197
> known-answer test vectors.

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

---

## AES — `std.crypto.aes`

AES (Rijndael) symmetric block cipher, in pure Tauraro. Supports AES-128
(10 rounds, 16-byte key) and AES-256 (14 rounds, 32-byte key), selected
automatically from the key length passed in. CBC mode uses PKCS#7 padding.
AES-192 is not implemented.

Verified against the official NIST/FIPS-197 known-answer test vectors —
see `tests/lang/42_aes.tr`.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Aes.encrypt_block` | `(plaintext: str, key: str, klen: int) -> str` | `str` | Encrypt exactly one 16-byte block, no padding/chaining. `klen` is 16 or 32. Mostly useful for testing against known-answer vectors — most callers want `encrypt_cbc`. |
| `Aes.decrypt_block` | `(ciphertext: str, key: str, klen: int) -> str` | `str` | Decrypt exactly one 16-byte block. |
| `Aes.encrypt_cbc` | `(plaintext: str, plen: int, key: str, klen: int, iv: str) -> str` | `str` | Encrypt `plen` bytes of `plaintext` under CBC mode with PKCS#7 padding. `iv` must be exactly 16 bytes. Returns raw ciphertext bytes (may contain embedded NUL — track the length separately, don't rely on `strlen`/`.len()`). |
| `Aes.decrypt_cbc` | `(ciphertext: str, clen: int, key: str, klen: int, iv: str) -> str` | `str` | Decrypt `clen` bytes of CBC+PKCS#7 ciphertext. Returns `""` if PKCS#7 padding validation fails (corrupted ciphertext, wrong key, wrong IV). |
| `Aes.decrypt_cbc_checked` | `(ciphertext: str, clen: int, key: str, klen: int, iv: str, ok: List[bool]) -> str` | `str` | Same as `decrypt_cbc`, but also sets `ok[0]` to `true`/`false` so callers can distinguish a genuinely-empty plaintext from a padding failure. |

`klen` (16 or 32) is always required explicitly rather than inferred from
the key string's length, because key bytes are raw binary and may contain
embedded NUL bytes; the same is true of `plen`/`clen` for plaintext and
ciphertext.

**IV requirement:** `encrypt_cbc` does **not** generate the IV for you —
the caller must supply 16 fresh random bytes on every call with a given
key (reusing an IV with the same key leaks information about plaintext
relationships between messages). Use `std/math/random.tr` to generate
one. This is deliberate: a "convenient" auto-generated default would
hide IV-reuse bugs instead of preventing them.

**Security note — CBC alone is not authenticated.** An attacker who can
modify ciphertext in transit can flip bits that corrupt the decrypted
plaintext in ways `Aes` will not reliably detect (only PKCS#7 padding
corruption is caught, and only probabilistically — a lucky bit flip can
still produce validly-padded garbage). If you need tamper detection,
encrypt-then-MAC: compute an HMAC (see [`Hmac`](#stdcryptohmac--hmac))
over the ciphertext + IV using a *separate* key, send it alongside the
ciphertext, and verify it **before** decrypting.

### Example

```tauraro
from std.crypto.aes import Aes
from std.encoding.hex import Hex

# AES-128 key (16 bytes) and a fresh, caller-supplied 16-byte IV.
mut key = Hex.decode("000102030405060708090a0b0c0d0e0f")
mut iv  = "0123456789abcdef"   # MUST be fresh random bytes per message in real use

mut plaintext = "Attack at dawn!"
mut ct = Aes.encrypt_cbc(plaintext, plaintext.len(), key, 16, iv)

mut pt = Aes.decrypt_cbc(ct, 16, key, 16, iv)   # ct is exactly one 16-byte block here
print(pt)
# Attack at dawn!

# Detecting tampering (CBC alone can't do this -- combine with Hmac):
from std.crypto.hmac import Hmac
mut mac = Hmac.sha256_str("separate-mac-key", ct)
# ... send `ct` + `mac` together; verify `mac` before calling decrypt_cbc.
```
