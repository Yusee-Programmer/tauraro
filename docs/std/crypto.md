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
from std.crypto.random import SecureRandom
from std.crypto.ct import ConstantTime
from std.crypto.digest import Digest, Hasher
from std.crypto.sha3 import Sha3, Sha3Stream
from std.crypto.blake2 import Blake2, Blake2bStream
from std.crypto.blake3 import Blake3, Blake3Stream
from std.crypto.x25519 import X25519
from std.crypto.otp import Otp
from std.crypto.argon2 import Argon2, Argon2Params, Argon2Hash
from std.crypto.bcrypt import Bcrypt
from std.crypto.native import ALG_SHA1, ALG_SHA256, ALG_SHA384, ALG_SHA512
from std.crypto.aead import Aead, AEAD_AES_128_GCM, AEAD_AES_256_GCM, AEAD_CHACHA20_POLY1305
from std.crypto.pk import PKey, Pem, SIG_PKCS1, SIG_PSS, CURVE_P256, CURVE_P384
from std.crypto.x509 import Certificate, TrustStore, X509, PURPOSE_SERVER
```

> The modules from `random` onward are backed by `std/crypto/crypto_native.c`,
> a dependency-free C file that `tauraroc` compiles and links automatically
> (via the `# tauraro-cpp-shim:` pragma in `std/crypto/native.tr`) when any of
> them is imported. They are all binary-safe: a `str` argument's length is
> `s.len()`, so embedded NUL bytes count. Digests and keys are returned as RAW
> bytes; `_hex` variants return lower-case hex.

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

---

## Secure randomness — `std.crypto.random`

OS CSPRNG (`getrandom`/`/dev/urandom`, `BCryptGenRandom`). Every call raises
(`throws str`) if the OS generator is unavailable; it never falls back to `rand()`.

| Method | Returns | Description |
|---|---|---|
| `SecureRandom.bytes(n)` | `str` | `n` raw random bytes. |
| `SecureRandom.token_hex(n)` | `str` | `n` bytes as `2n` hex chars. |
| `SecureRandom.token_urlsafe(n)` | `str` | `n` bytes as unpadded base64url. |
| `SecureRandom.int63()` | `int` | Uniform non-negative 63-bit integer. |
| `SecureRandom.below(n)` | `int` | Uniform in `[0, n)`, no modulo bias. |
| `SecureRandom.range(lo, hi)` | `int` | Uniform in `[lo, hi]`. |
| `SecureRandom.unit()` | `float` | Uniform in `[0, 1)` (53 bits). |

## Constant-time comparison — `std.crypto.ct`

| Method | Description |
|---|---|
| `ConstantTime.eq(a, b) -> bool` | Equal bytes? Time depends only on the lengths. |
| `ConstantTime.eq_n(a, b, n) -> bool` | Compare the first `n` bytes. |
| `ConstantTime.select(cond, a, b) -> int` | `a` if `cond` else `b`, without a data-dependent branch. |

## Binary-safe SHA-1/SHA-2 — `std.crypto.digest`

`Digest.sha1/sha256/sha384/sha512(data)` return raw digests; `*_hex` return hex;
`Digest.compute(alg, data)` takes an `ALG_*` id. `Hasher.init(alg)` → `update(data)` →
`finish()` / `finish_hex()` hashes incrementally (call `dispose()` to abandon one).

## HMAC additions — `std.crypto.hmac`

`Hmac.compute(alg, key, msg)` (raw tag, any `ALG_*`), `Hmac.sha1_raw/sha256_raw/sha384_raw/sha512_raw(key, msg)`,
`Hmac.compute_hex(...)`, and `Hmac.verify(alg, key, msg, tag) -> bool` (constant time; accepts tags truncated to
≥ 10 bytes). The original `Hmac.sha256(key, klen, msg)` is unchanged.

## HKDF and PBKDF2 additions — `std.crypto.kdf`

| Method | Description |
|---|---|
| `Kdf.hkdf(alg, ikm, salt, info, length)` | RFC 5869 extract-then-expand. |
| `Kdf.hkdf_extract(alg, salt, ikm)` / `Kdf.hkdf_expand(alg, prk, info, length)` | The two halves. |
| `Kdf.pbkdf2_hmac(alg, password, salt, iterations, dklen)` | PBKDF2 with HMAC-SHA1/256/384/512, binary-safe, in C. |

## SHA-3 and SHAKE — `std.crypto.sha3`

`Sha3.sha3_224/256/384/512(data)` (+ `_hex`), `Sha3.shake128(data, n)` / `shake256(data, n)` (+ `_hex`).
`Sha3Stream.sha3(bits)?`, `Sha3Stream.shake128()`, `Sha3Stream.shake256()` → `update(data)?` →
`finish()` (SHA-3) or repeated `squeeze(n)` (SHAKE, continues the same output stream) → `dispose()`.

## BLAKE2 — `std.crypto.blake2`

`Blake2.b512(data)`, `Blake2.b(data, outlen)?` (1–64), `Blake2.b_keyed(data, key, outlen)?` (key ≤ 64 bytes —
a keyed BLAKE2b is a MAC), `Blake2.s256(data)`, `Blake2.s(data, outlen)?`, `Blake2.s_keyed(...)?`, plus `_hex`
forms. `Blake2bStream.init(outlen, key)?` hashes incrementally.

## BLAKE3 — `std.crypto.blake3`

`Blake3.hash(data)` / `hash_hex`, `Blake3.hash_xof(data, n)`, `Blake3.keyed(key32, data)?`,
`Blake3.keyed_xof(...)?`, `Blake3.derive_key(context, material, n)`. `Blake3Stream.init()` /
`init_keyed(key)?` / `init_derive_key(context)` → `update` → `finish(n)` (repeatable) → `dispose()`.

## X25519 — `std.crypto.x25519`

```tauraro
from std.crypto.x25519 import X25519
from std.crypto.kdf import Kdf
from std.crypto.native import ALG_SHA256

mut mine = X25519.generate_private()?
mut my_pub = X25519.public_key(mine)?          # send to the peer
mut shared = X25519.shared_secret(mine, peer_pub)?   # raises on a low-order peer key
mut key = Kdf.hkdf(ALG_SHA256, shared, "", "my protocol v1", 32)?
```

Also `X25519.scalar_mult(scalar, point)?`, `X25519.to_hex(key)`, `X25519.from_hex(hex)?`.

## One-time passwords — `std.crypto.otp`

| Method | Description |
|---|---|
| `Otp.hotp(secret, counter, digits, alg)?` | RFC 4226 code (6–10 digits, SHA-1/256/512). |
| `Otp.totp(secret, unix_time, step, digits, alg)?` | RFC 6238 code. |
| `Otp.verify_totp(secret, code, unix_time, step, digits, alg, window) -> bool` | Accepts ±`window` steps, constant-time compare. |
| `Otp.verify_hotp(secret, code, counter, digits, alg, look_ahead) -> int` | Next counter to store, or -1. |
| `Otp.totp_counter(unix_time, step)` | The time step (store it to block replays). |
| `Otp.generate_secret(nbytes)?` | Random secret (≥ 10 bytes). |
| `Otp.secret_to_base32(raw)` / `Otp.secret_from_base32(text)?` | Authenticator-app encoding. |
| `Otp.provisioning_uri(b32, account, issuer, alg, digits, period)` | `otpauth://totp/...` for QR codes (`provisioning_uri_hotp` for HOTP). |

## Argon2 — `std.crypto.argon2`

Argon2d / Argon2i / Argon2id (RFC 9106, version 0x13) with PHC strings
(`$argon2id$v=19$m=19456,t=2,p=1$salt$hash`) compatible with other Argon2 libraries.
Defaults (`Argon2Params.default()`): Argon2id, 19 MiB, t = 2, p = 1 (OWASP). Lanes run sequentially.

| Method | Description |
|---|---|
| `Argon2.hash(password)?` | PHC string with defaults and a random salt. |
| `Argon2.hash_with(password, params)?` / `hash_with_salt(password, salt, params)?` | Custom costs. |
| `Argon2.verify(encoded, password) -> bool` | Constant-time; false for malformed strings. |
| `Argon2.needs_rehash(encoded, params) -> bool` | True when the stored costs differ. |
| `Argon2.decode(encoded)? -> Argon2Hash` / `Argon2.encode(...)` | PHC parsing / formatting. |
| `Argon2.hash_raw(variant, password, salt, t, m_kib, p, outlen)?` | Raw KDF output. |
| `Argon2.hash_raw_keyed(variant, password, salt, secret, ad, t, m_kib, p, outlen)?` | With secret key K and associated data X. |

Variants: `ARGON2D`, `ARGON2I`, `ARGON2ID`.

## bcrypt — `std.crypto.bcrypt`

`Bcrypt.hash(password, cost)?` → `$2b$<cost>$...` (60 chars), `Bcrypt.hash_with_salt(password, salt16, cost)?`,
`Bcrypt.verify(password, hashed) -> bool` (accepts `$2a$`/`$2b$`/`$2y$`), `Bcrypt.cost_of(hashed)`,
`Bcrypt.needs_rehash(hashed, cost)`. `hash` refuses passwords over 72 bytes instead of silently truncating;
`verify` truncates at 72 bytes for compatibility with hashes made elsewhere.

Tested in `tests/lang/105_crypto_modern.tr` against the RFC/FIPS vectors and hashes produced by
argon2-cffi and pyca/bcrypt.

## AEAD — `std.crypto.aead`

`Aead.seal(alg, key, nonce12, aad, plaintext)?` → ciphertext ‖ 16-byte tag; `Aead.open(alg, key, nonce12, aad, sealed)?`
raises on any modification (constant-time tag check). `alg`: `AEAD_AES_128_GCM` (16-byte key), `AEAD_AES_256_GCM`,
`AEAD_CHACHA20_POLY1305` (32-byte keys). Never reuse a nonce with the same key.

## Public keys and signatures — `std.crypto.pk`

`PKey` holds an RSA or EC (P-256/P-384) key, public or private, parsed from DER/PEM (SPKI, PKCS#8, PKCS#1, SEC1;
encrypted PEM is not supported) or generated (`PKey.generate_ec(CURVE_P256)?`).

| Method | Description |
|---|---|
| `PKey.from_pem(text)?` / `PKey.from_der(der)?` | Parse the first key block / a DER key. |
| `PKey.ec_from_private(curve, scalar)?`, `ec_from_public(curve, point)?`, `rsa_from_public(n, e)?` | Build from raw parts (points are validated). |
| `sign(msg, alg, scheme)?` / `verify(msg, sig, alg, scheme)` | RSA: `SIG_PKCS1` or `SIG_PSS` (salt = hash length). EC: DER ECDSA, scheme ignored, deterministic nonces (RFC 6979). |
| `sign_raw(msg, alg)?` / `verify_raw(msg, sig, alg)` | ECDSA as fixed-size `r‖s` (JOSE/WebAuthn). |
| `shared_secret(peer_point)?` | ECDH x-coordinate. |
| `kind()`, `bits()`, `has_private()`, `public_bytes()`, `ec_private_bytes()?`, `dispose()` | Introspection / cleanup. |

`Pem.decode(text, label)?`, `Pem.decode_nth(text, label, i)?`, `Pem.label_of(text, i)`, `Pem.encode(der, label)`.
SHA-1 is accepted for verification only. Private-key code avoids secret-dependent branches and lookups but has
not been independently audited for side channels.

## X.509 — `std.crypto.x509`

```tauraro
from std.crypto.x509 import Certificate, TrustStore, X509, PURPOSE_SERVER

mut chain = Certificate.all_from_pem(read_file("fullchain.pem"))?
mut code = X509.verify(TrustStore.system(), chain, "example.com", 0, PURPOSE_SERVER)
if code != 0: print(X509.error_text(code))
print(chain[0].subject(), chain[0].dns_names(), chain[0].not_after())
```

`Certificate`: `subject()`, `issuer()`, `serial_hex()`, `dns_names()`, `ip_addresses()`, `not_before()`/`not_after()`
(Unix time), `is_ca()`, `path_len()`, `key_usage()` (`KU_*` bits, -1 if absent), `ext_key_usage()` (`EKU_*`),
`signature_algorithm()`, `key_algorithm()`, `key_bits()`, `matches_host(h)`, `is_signed_by(issuer)`,
`is_self_signed()`, `fingerprint_sha256()`, `der()`, `to_pem()`, `public_key()?`.

`TrustStore.system()` (OS roots: CA bundle files or the Windows ROOT store, loaded once and shared; honours
`SSL_CERT_FILE`), `TrustStore.empty()`, `system_copy()`, `add_pem(text)`, `add_der(der)?`, `add(cert)?`, `count()`.

`X509.verify(store, chain, host, now, purpose)` / `verify_der(...)` build a path through untrusted intermediates
(any order) to a trust anchor and check signatures (SHA-1 refused), validity, basicConstraints/pathLen, keyUsage,
EKU, dNSName name constraints, unknown critical extensions and the host name (RFC 6125: SAN only, single-label
leftmost wildcards, IP SANs). Returns 0 or a negative code (`X509.error_text`). Revocation is not checked.

Tested by `tests/lang/106_x509_tls.tr`; the TLS client was also checked against OpenSSL `s_server` (every cipher
suite, HelloRetryRequest, KeyUpdate) and public HTTPS sites.

## DER, keys, CSRs and certificates — `std.crypto.der`, `std.crypto.csr`

`std.crypto.der` encodes and decodes ASN.1 DER: `Der.seq`, `set_of`, `integer`, `uint` (big-endian
magnitude), `oid("1.2.840.10045.2.1")`, `octets`, `bits`, `utf8`, `ia5`, `boolean`, `null_value`,
`time(unix)` (UTCTime, GeneralizedTime from 2050), `ctx(n, ...)` / `ctx_prim(n, ...)` for `[n]` tags;
`Der.parse(der)?` (one element), `Der.children(node)?`, `DerReader` (`next()?`, `expect(tag)?`),
`Der.oid_text`, `uint_bytes`, `bit_bytes`, `time_value(node)?`.

`std.crypto.csr`:

```tauraro
from std.crypto.csr import KeyPair, Csr, CertBuilder, name_der
from std.crypto.pk import CURVE_P256, Pem

mut key = KeyPair.generate_ec(CURVE_P256)?               # or KeyPair.from_pem(text)? (RSA / EC)
mut csr_pem = Csr.build_pem(key, ["example.com", "www.example.com", "192.0.2.1"])?
mut csr = Csr.from_pem(csr_pem)?                         # csr.dns_names, csr.ip_addresses, csr.verify()

mut ca = KeyPair.generate_ec(CURVE_P256)?
mut b = CertBuilder.init()                               # random serial, valid 90 days
b.subject_cn("Test CA")
b.is_ca = true
mut ca_pem = Pem.encode(b.self_signed(ca)?, "CERTIFICATE")
mut leaf = CertBuilder.init()
leaf.subject_cn("example.com")
leaf.issuer = name_der("Test CA")
leaf.add_name("example.com")
mut leaf_der = leaf.sign(key.spki(), ca)?                # signed by the CA key
```

`KeyPair`: `pkey` (a `PKey`), `kind`, `spki()` (SubjectPublicKeyInfo), `private_pem()` (PKCS#8 for EC),
`public_pem()`, `rsa_n` / `rsa_e` / `ec_x` / `ec_y`, `sign_der(data)?` (X.509 form), `sign_jose(data)?`
(JWS form), `rsa_from_components(...)?`, `ec_from_scalar(curve, d)?`. `CertBuilder` adds
subjectAltName, basicConstraints / keyUsage for CAs, keyUsage + serverAuth for leaves, and any
`add_extension(oid, critical, value_der)`. `cert_extension(der, oid)?` and `cert_names(der)?` read
extensions back. Signatures: SHA-256 with RSA (PKCS#1 v1.5) and P-256, SHA-384 with P-384. Generated
CSRs and certificates pass `openssl req -verify` / `openssl verify`. RSA keys cannot be generated.
## scrypt — `std.crypto.scrypt`

scrypt (RFC 7914), implemented in C. `Scrypt.derive(password, salt, n, r, p, dklen)?` returns `dklen` raw bytes.
Parameters are validated (`n` a power of two > 1, `r, p >= 1`, `r·p < 2^30`, `n < 2^(16r)`) and the working
memory (≈ `128·r·n` bytes) is capped at `SCRYPT_DEFAULT_MAX_MEM` (1 GiB); `derive_limited(..., max_mem)?` takes
an explicit cap, `Scrypt.memory_needed(n, r, p)` reports it.

Password hashes use the PHC-style string also produced by passlib: `$scrypt$ln=17,r=8,p=1$<salt>$<hash>`
(unpadded base64, `ln = log2(n)`).

| Method | Description |
|---|---|
| `Scrypt.hash(password)?` | Defaults (`ScryptParams.default()`): `ln=17, r=8, p=1` (128 MiB, OWASP), 16-byte salt, 32-byte hash. |
| `Scrypt.hash_with(password, params)?` / `hash_with_salt(password, salt, params)?` | Custom costs (`ScryptParams.init(log_n, r, p)`). |
| `Scrypt.verify(encoded, password) -> bool` | Constant-time; false for malformed or over-limit strings. |
| `Scrypt.needs_rehash(encoded, params) -> bool`, `Scrypt.decode(encoded)? -> ScryptHash`, `Scrypt.encode(...)` | |

## age file encryption — `std.crypto.age`

The [age v1 format](https://age-encryption.org/v1), interoperable with `age`/`rage`: X25519 recipients
(`age1...`) and identities (`AGE-SECRET-KEY-1...`), passphrase (scrypt) recipients, the HMAC-SHA256 header MAC,
the STREAM payload (ChaCha20-Poly1305, 64 KiB chunks) and ASCII armor.

```tauraro
from std.crypto.age import Age, AgeIdentity, AgeRecipient

mut id = AgeIdentity.generate()?
write_file("key.txt", id.to_string() + "\n")
mut rcpt = id.recipient()?                 # rcpt.to_string() -> "age1..."
mut rs: List[AgeRecipient] = []
rs.append(AgeRecipient.parse("age1xmwwc06ly3ee5rytxm9mflaz2u56jjj36s0mypdrwsvlul66mv4q47ryef")?)
rs.append(rcpt)
mut ct = Age.encrypt(data, rs)?            # binary; Age.encrypt_armored for text
mut ids = AgeIdentity.parse_file(read_file("key.txt"))?
mut pt = Age.decrypt(ct, ids)?             # binary or armored input
mut sealed = Age.encrypt_passphrase(data, "correct horse")?
```

| API | Description |
|---|---|
| `AgeRecipient.parse(text)?`, `x25519(pub32)`, `passphrase(pw)`, `passphrase_with_work_factor(pw, log_n)`, `to_string()` | Recipients (default scrypt work factor 18). A passphrase recipient must be the only one. |
| `AgeIdentity.generate()?`, `parse(text)?`, `parse_file(text)?`, `x25519(secret32)`, `passphrase(pw)`, `passphrase_with_max_work_factor(pw, n)`, `recipient()?`, `to_string()` | Identities; passphrase identities accept work factors up to 22 by default. |
| `Age.encrypt(data, recipients)?`, `encrypt_armored(...)?`, `encrypt_passphrase(data, pw)?` | One-shot encryption. |
| `Age.decrypt(data, identities)?`, `decrypt_binary(...)?`, `decrypt_passphrase(data, pw)?` | One-shot decryption (armor detected automatically by `decrypt`). |
| `Age.armor(bin)`, `Age.dearmor(text)?`, `Age.is_armored(data)` | Strict armor: LF or CRLF, 64-column lines, canonical padded base64, whitespace allowed only outside. |
| `AgeEncryptor.init(recipients)?` → `update(chunk)? -> str`, `finish()? -> str` | Streaming encryption; output is released as soon as each 64 KiB chunk is final. |
| `AgeDecryptor.init(identities)` → `update(chunk)? -> str`, `finish()? -> str`, `partial()` | Streaming decryption; only authenticated plaintext is released. |
| `Age.encrypt_file(in, out, recipients)?`, `Age.decrypt_file(in, out, identities)?` | Files in 1 MiB reads (binary format). On a decryption error delete the partial output. |

Decryption is strict and errors are categorised by prefix: `age: header:`, `age: no identity matched`,
`age: bad header MAC`, `age: payload:`, `age: armor:`. Post-quantum hybrid (`age1pq`) and SSH recipients are not
supported; their stanzas are ignored like any unknown type.

Tested in `tests/lang/138_crypto_age_ssh.tr` with the 108 small non-hybrid vectors of the C2SP/CCTV age testkit
(the full 128-vector non-hybrid set, including the multi-megabyte STREAM vectors, passed during development);
files encrypted here were decrypted by an independent pyca/cryptography implementation.

## SSH keys and signatures — `std.crypto.ssh`

OpenSSH formats for `ssh-ed25519`, `ecdsa-sha2-nistp256`, `ecdsa-sha2-nistp384` and `ssh-rsa` keys.

```tauraro
from std.crypto.ssh import SshKey, SshSig, SshAuthorizedKey

mut key = SshKey.parse_private(read_file("id_ed25519"), "passphrase or empty")?
print(key.public_line(), key.fingerprint_sha256())      # as in id_ed25519.pub / ssh-keygen -l
mut sig = SshSig.sign(key, data, "file")?                # ssh-keygen -Y sign -n file
mut signer = SshSig.verify(sig, data, "file")?           # returns the signing key
for ak in SshAuthorizedKey.parse_file(read_file("authorized_keys")): print(ak.options, ak.key.comment)
```

| API | Description |
|---|---|
| `SshKey.parse_public(line)?`, `from_blob(blob, comment)?`, `public_line()`, `blob`, `key_type`, `comment`, `bits()` | Public keys (`type base64 comment`); blobs are validated (EC points on the curve). |
| `fingerprint_sha256()`, `fingerprint_md5()` | `SHA256:...` / `MD5:aa:bb:...` exactly as `ssh-keygen -l [-E md5]`. |
| `SshKey.parse_private(text, passphrase)?`, `parse_private_binary(data, pw)?` | `openssh-key-v1`, unencrypted or `aes256-ctr` + `bcrypt` KDF; check bytes, padding and public/private consistency are verified. |
| `private_pem(passphrase, rounds)?` | Writes `openssh-key-v1` (encrypted with aes256-ctr / bcrypt-pbkdf when `passphrase` is non-empty; OpenSSH uses 16 rounds). |
| `SshKey.generate_ed25519(comment)?`, `ed25519_from_seed(seed, comment)?`, `generate_ecdsa(curve, comment)?` | New keys. |
| `SshKey.from_pkey(pk, comment)?`, `to_pkey()?`, `public_key()` | std.crypto.pk conversion (public keys of all types, EC and RSA private keys to PKey, EC private keys from PKey). |
| `sign_ssh(msg)?`, `verify_ssh(msg, sig_blob, allow_sha1) -> bool` | Raw SSH signature blobs (RSA signs with `rsa-sha2-512`). |
| `SshSig.sign(key, data, namespace)?`, `SshSig.verify(armored, data, namespace)? -> SshKey`, `SshSig.verify_with(armored, data, namespace, allowed) -> bool` | SSHSIG (`ssh-keygen -Y sign/verify`); SHA-512 for signing, SHA-256/512 accepted. |
| `SshAuthorizedKey.parse(line)?`, `parse_file(text)`, `options`, `key`, `has_option(name)`, `option(name)` | `authorized_keys` lines with options (`command="..."` quoting, `\"` escapes); `parse_file` skips bad lines like sshd. |

Limitations: Ed25519 keys convert to PKey as public keys only (PKey cannot sign with Ed25519); RSA private keys
cannot be built from a PKey (no private exponent) or generated here; `aes256-gcm@openssh.com` and
`chacha20-poly1305@openssh.com` encrypted keys, certificates and `sk-*` security-key types are not supported.
`std.crypto.native` also exposes `trc_bcrypt_pbkdf` (OpenBSD bcrypt_pbkdf) and `trc_aes_ctr`.

Verified against OpenSSH 10.2 `ssh-keygen`: keys, fingerprints and SSHSIG signatures it made are parsed and
verified, and keys (unencrypted and encrypted) and signatures written here are accepted by `ssh-keygen -y` and
`ssh-keygen -Y verify` (`tests/lang/138_crypto_age_ssh.tr`).
