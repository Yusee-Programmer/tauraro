# std.crypto — Cryptographic Primitives

```tauraro
from std.crypto.hash import Hash
from std.crypto.hmac import Hmac
from std.crypto.uuid import UUID, ULID, MonotonicUlid
from std.crypto.aes import Aes
from std.crypto.sha512 import Sha512
from std.crypto.chacha20 import ChaCha20, Poly1305
from std.crypto.kdf import Kdf
from std.crypto.jwt import Jwt, JwtResult
```

> SHA-256, HMAC-SHA256, and MD5 are implemented in pure C — no external library required.
> UUID v4 uses `/dev/urandom` on POSIX and `rand()` on Windows.
> AES is implemented in pure Tauraro (same approach as SHA-256 — a standard
> algorithm written directly in the language, compiled to native code, no
> external library required) and verified against the official NIST/FIPS-197
> known-answer test vectors.
> SHA-512/384, ChaCha20-Poly1305, PBKDF2, and JWT (HS256) are all implemented
> in pure Tauraro as well, following that same approach — no OpenSSL or other
> C crypto library dependency anywhere in this package.

---

## std.crypto.hash — Hash

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Hash.sha256` | `(s: str) -> str` | `str` | SHA-256 digest of a null-terminated string. Returns 64-char lowercase hex. |
| `Hash.sha256_bytes` | `(data: str, len_: int) -> str` | `str` | SHA-256 of exactly `len_` bytes. Returns 32 **raw** bytes (not hex — may contain embedded NULs; use `Hex.encode_bytes` for a printable digest). |
| `Hash.md5` | `(s: str) -> str` | `str` | MD5 digest. Returns 32-char lowercase hex. **Not secure** — use for checksums only. |

### Example

```tauraro
from std.crypto.hash import Hash
from std.encoding.hex import Hex

mut h = Hash.sha256("hello")
print(h)
# 2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824

mut h2 = Hash.sha256_bytes("hello world", 11)   # 32 RAW bytes, not hex
mut h2_hex = Hex.encode_bytes(h2 as Pointer[char], 32)
print(h2_hex)
# b94d27b9934d3e08a52e52d7da7dabfac484efe04294e576f3a7a4e0c7e8b1a

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

---

## SHA-512 / SHA-384 — `std.crypto.sha512`

SHA-512 and SHA-384 (FIPS 180-4), in pure Tauraro — same approach as
`std.crypto.aes`: the standard algorithm written directly in the
language (64-bit word ops throughout), compiled to native code, no
external library required. SHA-384 is SHA-512 with a different initial
hash value and the digest truncated to 384 bits — the compression
function and message schedule are shared.

Verified against the official NIST/FIPS-180-4 known-answer test vectors
(`SHA-512("")`, `SHA-512("abc")`, and a multi-block message) — see
`tests/lang/44_crypto2.tr`.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Sha512.sha512` | `(s: str) -> str` | `str` | SHA-512 digest of a null-terminated string. Returns 128-char lowercase hex. |
| `Sha512.sha512_bytes` | `(data: str, len_: int) -> str` | `str` | SHA-512 of exactly `len_` bytes. Returns 128-char lowercase hex. |
| `Sha512.sha384` | `(s: str) -> str` | `str` | SHA-384 digest of a null-terminated string. Returns 96-char lowercase hex. |
| `Sha512.sha384_bytes` | `(data: str, len_: int) -> str` | `str` | SHA-384 of exactly `len_` bytes. Returns 96-char lowercase hex. |

### Example

```tauraro
from std.crypto.sha512 import Sha512

mut h = Sha512.sha512("abc")
print(h)
# ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49

mut h384 = Sha512.sha384("abc")
print(h384)
# cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a
```

---

## ChaCha20-Poly1305 — `std.crypto.chacha20`

Authenticated encryption with associated data (AEAD), per RFC 8439: the
ChaCha20 stream cipher combined with a Poly1305 MAC. This is the
primitive [`Aes`](#aes--stdcryptoaes)'s own security note points at —
where AES-CBC needs a separate encrypt-then-MAC step for tamper
detection, `ChaCha20` provides authenticated encryption directly in one
call. Pure Tauraro, verified against RFC 8439's own worked test vectors
(the full section 2.8.2 AEAD example, the section 2.3.2 block-function
vector, and the section 2.5.2 standalone Poly1305 vector) — see
`tests/lang/44_crypto2.tr`.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `ChaCha20.encrypt` | `(plaintext: str, plen: int, key: str, nonce: str, aad: str, alen: int) -> (str, str)` | `(str, str)` | Encrypt `plen` bytes under ChaCha20-Poly1305. `key` must be 32 bytes, `nonce` 12 bytes, `aad` any length (`alen=0` for none). Returns `(ciphertext, tag)` — ciphertext is `plen` raw bytes, tag is always 16 raw bytes. |
| `ChaCha20.decrypt` | `(ciphertext: str, clen: int, key: str, nonce: str, aad: str, alen: int, tag: str) -> Option[str]` | `Option[str]` | Decrypt + **verify** `tag` before returning anything. `Option.Some(plaintext)` only if the tag matches; `Option.None` on any mismatch (tampered ciphertext, tampered aad, wrong key/nonce/tag). |
| `Poly1305.compute` | `(msg: str, mlen: int, key32: str) -> str` | `str` | Standalone Poly1305 MAC over `mlen` bytes using a 32-byte one-time key. Returns 16 raw bytes. Exposed mainly for testing against RFC 8439's own Poly1305-only vector; most callers want `ChaCha20.encrypt`/`decrypt`. |

**Nonce requirement:** like AES's IV, `nonce` must be exactly 12 bytes
and **must never be reused** with the same key — nonce reuse in
ChaCha20-Poly1305 catastrophically breaks both confidentiality and
authentication (it leaks the XOR of two plaintexts and lets an attacker
forge tags). Generate a fresh random nonce per message (see
`std/math/random.tr`), or use a counter that is guaranteed never to
repeat for a given key.

**Security note — `decrypt` authenticates before returning plaintext.**
This directly avoids the AEAD-decrypt vulnerability class where a
"helpful" decrypt function returns best-effort plaintext even when the
tag doesn't match — `ChaCha20.decrypt` returns `Option.None` and nothing
else on any verification failure; callers must not treat a `None` result
as "try again," it means the ciphertext/aad/tag are not trustworthy.

### Example

```tauraro
from std.crypto.chacha20 import ChaCha20
from std.encoding.hex import Hex

mut key   = Hex.decode("808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f")
mut nonce = Hex.decode("070000004041424344454647")   # MUST be fresh per message in real use
mut aad   = "header-metadata"

mut plaintext = "Ladies and Gentlemen of the class of '99!"
mut r = ChaCha20.encrypt(plaintext, plaintext.len(), key, nonce, aad, aad.len())
mut ciphertext = r[0]
mut tag        = r[1]

match ChaCha20.decrypt(ciphertext, plaintext.len(), key, nonce, aad, aad.len(), tag):
    case Option.Some(recovered):
        print(recovered)   # Ladies and Gentlemen of the class of '99!
    case Option.None:
        print("tag mismatch -- reject")
```

---

## PBKDF2 — `std.crypto.kdf`

Password-based key derivation, per RFC 2898 / PKCS#5 v2.1: PBKDF2-HMAC-
SHA256, built on iterating HMAC (using `std.crypto.hash`'s length-safe
`Hash.sha256_bytes` internally). Verified against well-known
PBKDF2-HMAC-SHA256 known-answer vectors (single- and multi-block output,
multiple iteration counts) — see `tests/lang/44_crypto2.tr`.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Kdf.pbkdf2` | `(password: str, slen: int, salt: str, iterations: int, dklen: int) -> str` | `str` | Derive `dklen` bytes of key material. `slen` is the salt length in bytes (explicit, since salt is raw binary and may contain embedded NULs). Returns `dklen` raw bytes. |
| `Kdf.pbkdf2_str` | `(password: str, salt: str, iterations: int, dklen: int) -> str` | `str` | Convenience overload: `salt` is a null-terminated string (no embedded NULs). |

**Security note — iteration count.** PBKDF2 with a low iteration count is
**not safe** for password storage: modern GPUs compute millions of
PBKDF2-SHA256 guesses per second at low counts, making offline
brute-force of a stolen hash database practical. Current OWASP guidance
recommends **at least 600,000 iterations** for PBKDF2-HMAC-SHA256 when
hashing passwords (target roughly 200ms of compute on typical server
hardware, and raise the count over time as hardware gets faster — this
is a moving target). If you don't have a specific reason to use PBKDF2
(e.g. FIPS compliance), a memory-hard KDF such as Argon2 or scrypt is
generally a stronger choice for **new** password-storage designs, since
PBKDF2 has no memory-hardness and is comparatively cheap to accelerate
on GPU/ASIC. (Only PBKDF2 is implemented in this stdlib today — scrypt
is a documented gap, not silently unsupported.)

### Example

```tauraro
from std.crypto.kdf import Kdf
from std.encoding.hex import Hex

mut salt = "16-byte-random-salt-here-please"   # MUST be random per user, >= 16 bytes in real use
mut key  = Kdf.pbkdf2_str("correct horse battery staple", salt, 600000, 32)
# key is 32 raw bytes -- use Hex.encode_bytes(key as Pointer[char], 32) for storage/comparison
```

---

## JWT — `std.crypto.jwt`

JSON Web Tokens (RFC 7519), **HS256 only** — HMAC-SHA256 signing, no
RSA/ECDSA ("RS256"/"ES256") or `"alg":"none"` support (asymmetric
algorithms are out of scope: this stdlib has no public-key crypto
primitives yet). Claims are `Dict[str, str]` (string-valued claims only).
Header/payload/signature segments are base64url-encoded per the spec
(via `std.encoding.base64`'s existing `encode_url`/`decode_url` — no
padding, `-`/`_` alphabet, not standard base64). Claims JSON is built via
`std.encoding.json`'s `JsonWriter` on encode and parsed via `Json.parse`
on decode.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Jwt.encode` | `(claims: Dict[str, str], secret: str) -> str` | `str` | Build and sign a compact JWT (`header.payload.signature`) using HS256 over `secret`. |
| `Jwt.decode` | `(token: str, secret: str) -> JwtResult` | `JwtResult` | Verify `token`'s signature against `secret`, then return its claims. `JwtResult.Ok(claims: Dict[str, str])` on success, `JwtResult.Err(message: str)` on any failure (malformed token, wrong secret, unsupported `alg`, tampered header/payload/signature). |

**Security note — signature is always verified before claims are
trusted.** This targets the classic JWT vulnerability class:
`"alg":"none"` / signature-not-checked decoders that let an attacker
forge arbitrary claims. `Jwt.decode`:
- never reads the token's own `alg` header to pick the verification
  algorithm (verification is hardcoded to HS256; an attacker-controlled
  field must never select how a token is checked),
- rejects any token whose header `alg` is not exactly `"HS256"`,
- uses a constant-time-ish comparison for the signature check (same
  accumulate-the-XOR pattern as `ChaCha20`'s tag check, to avoid a
  timing side-channel on a secret-dependent comparison), and
- **never returns `JwtResult.Ok` on any verification failure** — a
  tampered header, tampered payload, tampered signature, or wrong
  secret all produce `JwtResult.Err`, never partial or unverified
  claims.

Use a real random secret of at least 32 bytes in production — a short,
guessable HMAC secret defeats HS256 regardless of how carefully
`Jwt.decode` checks it.

### Example

```tauraro
from std.crypto.jwt import Jwt, JwtResult

mut claims: Dict[str, str] = {}
claims.set("sub", "user123")
claims.set("role", "admin")

mut token = Jwt.encode(claims, "a-real-random-secret-at-least-32-bytes-long")
print(token)
# eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9. ... .<signature>

match Jwt.decode(token, "a-real-random-secret-at-least-32-bytes-long"):
    case JwtResult.Ok(verified):
        print(verified.get("sub"))    # user123
    case JwtResult.Err(msg):
        print("rejected: " + msg)

# A token signed/verified with a different secret is always rejected:
match Jwt.decode(token, "wrong-secret"):
    case JwtResult.Ok(verified):
        print("should never print")
    case JwtResult.Err(msg):
        print("rejected: " + msg)    # rejected: signature verification failed
```
