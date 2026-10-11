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

---

## std.crypto.bytes — CryptoBytes (constant-time compare, secure wipe, binary helpers)

```tauraro
from std.crypto.bytes import CryptoBytes
```

Binary values in `std.crypto` (digests, keys, MACs) are `str`s used as raw byte
buffers; the `str` header carries the true length, so embedded NULs are fine.
Print them with `Hex.encode`, build them with `Hex.decode`.

| Method | Signature | Description |
|---|---|---|
| `CryptoBytes.ct_eq` | `(a: str, b: str) -> bool` | Constant-time equality (no early exit, no data-dependent branch; time depends only on the lengths). Different lengths return `false` immediately. Use it for MAC / token / password-hash comparison instead of `==`. |
| `CryptoBytes.ct_eq_n` | `(a: Pointer[char], b: Pointer[char], n: int) -> bool` | Same, for the first `n` bytes of two raw buffers. |
| `CryptoBytes.ct_eq_int` | `(x: int, y: int) -> int` | Branch-free `1` if the low 32 bits are equal else `0`. |
| `CryptoBytes.secure_zero` | `(p: Pointer[char], n: int)` | Wipe `n` bytes with volatile stores (a C runtime helper, `_tr_c_secure_zero`), so the compiler cannot drop it as a dead store. |
| `CryptoBytes.random` | `(n: int) -> str` | `n` bytes from the OS CSPRNG (`BCryptGenRandom` / `/dev/urandom`). |
| `CryptoBytes.at` | `(s: str, i: int) -> int` | Unsigned byte `i` (0..255). |
| `CryptoBytes.zeroed` / `wrap` / `release` | `(n) -> Pointer[char]` / `(p, n) -> str` / `(p)` | Allocate a zeroed `n`-byte buffer, turn it into a binary-safe `str` (ownership moves), or free it. `get`/`put` read/write a byte. |
| `CryptoBytes.slice` / `concat` | `(s, start, n) -> str` / `(a, b) -> str` | Byte-exact slice / concatenation. |

**Honest limits.** `ct_eq` is the standard accumulate-the-XOR idiom compiled by an
optimising C compiler: it holds on mainstream compilers but there is no
language-level guarantee. `secure_zero` wipes a buffer you own; `str` values are
reference-counted and may be copied by the runtime, so a secret that lived in a
`str` cannot be guaranteed erased. The std.crypto hashers and ciphers that keep
state in an explicit buffer wipe it in `free()`.

---

## std.crypto.sha3 — SHA-3, SHAKE, Keccak

```tauraro
from std.crypto.sha3 import Sha3, Sha3Hasher
```

FIPS 202 SHA3-224/256/384/512, the SHAKE128/256 extendable-output functions and the
original-padding Keccak-256/512 (as used by Ethereum), in pure Tauraro (the
Keccak-f[1600] permutation is computed from its definition, no big tables).

| Method | Signature | Returns |
|---|---|---|
| `Sha3.sha3_224` / `sha3_256` / `sha3_384` / `sha3_512` | `(data: str) -> str` | raw digest (28/32/48/64 bytes) |
| `Sha3.sha3_256_hex` (etc.) | `(data: str) -> str` | lowercase hex |
| `Sha3.shake128` / `shake256` | `(data: str, outlen: int) -> str` | `outlen` raw XOF bytes (`_hex` variants too) |
| `Sha3.keccak256` / `keccak512` | `(data: str) -> str` | raw digest with the pre-standard 0x01 padding |
| `Sha3Hasher.sha3_256()` (etc., `shake128()`, `shake256()`, `keccak256()`) | `() -> Sha3Hasher` | incremental hasher |
| `h.update(data)` / `h.update_bytes(data, n)` | | absorb |
| `h.digest()` / `h.digest_hex()` | `-> str` | fixed-length digest (finishes the hash) |
| `h.squeeze(n)` | `(n: int) -> str` | next `n` output bytes; call repeatedly for XOF streaming |
| `h.free()` | | wipe and release the sponge state |

```tauraro
print(Sha3.sha3_256_hex("abc"))
# 3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431532
mut x = Sha3Hasher.shake256()
x.update("seed")
mut block1 = x.squeeze(32)
mut block2 = x.squeeze(32)      # continues the stream
x.free()
```

Verified against the NIST FIPS 202 examples (empty, "abc", 448/896-bit messages, one million
`a`), Keccak-256/512 known answers, and Python `hashlib` for lengths straddling every rate
boundary (`tests/lang/110_sha3.tr`).

---

## std.crypto.blake2 — BLAKE2b and BLAKE2s (RFC 7693)

```tauraro
from std.crypto.blake2 import Blake2b, Blake2s, Blake2bHasher, Blake2sHasher
```

| Method | Signature | Description |
|---|---|---|
| `Blake2b.hash` / `hash_hex` | `(data: str, outlen: int) -> str` | BLAKE2b with `outlen` 1..64 bytes (raw / hex). Returns `""` for an invalid `outlen`. |
| `Blake2b.keyed` / `keyed_hex` | `(data: str, key: str, outlen: int) -> str` | Keyed (MAC) mode, key up to 64 bytes. |
| `Blake2b.hash_params` | `(data, outlen, key, salt, person) -> str` | Full parameter block; `salt`/`person` are `""` or exactly 16 bytes. |
| `Blake2s.*` | same, `outlen` 1..32, key up to 32, salt/person `""` or 8 bytes | BLAKE2s. |
| `Blake2bHasher.init(outlen, key)` / `init_full(...)` | `-> Blake2bHasher` | Incremental: `update(data)`, `update_bytes(data, n)`, `finalize()` / `finalize_hex()` (once), `is_ok()`, `free()`. `Blake2sHasher` is the same. |

```tauraro
print(Blake2b.hash_hex("abc", 64))
mut tag = Blake2b.keyed("message", "secret key", 32)   # 32-byte MAC
```

Sequential mode only (no tree hashing, no XOF mode). Verified against the RFC 7693
vectors, the official BLAKE2 keyed known-answer vectors and Python `hashlib`
(`tests/lang/111_blake2.tr`).

---

## std.crypto.x25519 — X25519 key exchange (RFC 7748)

```tauraro
from std.crypto.x25519 import X25519, X25519Keys
```

| Method | Signature | Description |
|---|---|---|
| `X25519.gen_private` | `() -> str` | 32 random bytes from the OS CSPRNG. |
| `X25519.public_key` | `(private_key: str) -> str` | `X25519(private, 9)`. |
| `X25519.shared_secret` | `(private_key: str, peer_public: str) -> str` | DH shared secret; returns `""` for wrong-size input **or an all-zero result** (low-order peer point). |
| `X25519.scalarmult` | `(scalar: str, point: str) -> str` | The raw RFC 7748 function (never rejects a result). |
| `X25519.keypair` | `() -> X25519Keys` | Fresh `private_key` / `public_key`. |
| `X25519.clamp`, `base_point`, `*_hex` helpers | | Utilities; hex in / hex out variants. |

```tauraro
mut a = X25519.keypair()
mut b = X25519.keypair()
mut s1 = X25519.shared_secret(a.private_key, b.public_key)
mut s2 = X25519.shared_secret(b.private_key, a.public_key)   # == s1
# derive the session key with a KDF (e.g. Blake2b.hash(s1 + both public keys, 32)); never use s1 directly
```

Montgomery ladder with a branch-free conditional swap (no secret-dependent branches or
table indices), 16 x 16-bit limbs. Verified against RFC 7748 section 5.2 (vectors and the
1/1000-iteration chain), section 6.1 and OpenSSL via Python `cryptography`
(`tests/lang/113_x25519.tr`). A scalar multiplication takes about 2 ms; constant-time
behaviour depends on the C compiler and has not been formally audited.

## Public-key toolkit (RSA, ECDSA, X.509)

These modules are pure Tauraro and **interoperate with OpenSSL**: the tests verify
signatures and decrypt ciphertexts produced by the `openssl` CLI, and our signatures
were checked with `openssl dgst -verify` / `openssl pkeyutl`. They share three helpers:

| Module | Purpose |
|---|---|
| `std.crypto.pkutil` | `Bytes` (byte buffers = `Vec[int]` 0..255: `from_str/to_str/from_hex/to_hex/slice/concat/eq/eq_ct/random`), `PkHash` (`digest(alg, data)`, `hmac(alg, key, msg)` for `"sha1"/"sha256"/"sha384"/"sha512"`, raw output) |
| `std.crypto.bn` | unsigned big numbers (28-bit limbs, `Vec[int]`): `bn_from_hex/bn_to_hex/bn_from_bytes/bn_to_bytes`, `bn_add/sub/mul/divmod/shl/shr/cmp`, `Mont` (Montgomery context: `mul`, `pow`, `inv_prime`, modular `add/sub`), `bn_modexp` |
| `std.encoding.asn1`, `std.encoding.pem` | DER and PEM (see `encoding.md`) |

Why not `std.math.bigint`? It is a decimal (base 10^9) type whose division is a per-limb
binary search: one 2048-bit modular exponentiation takes minutes there. `std.crypto.bn` does
a full 2048-bit `modexp` (2048-bit exponent) in roughly a second *including* a cold compile,
and an RSA-2048 signature (CRT) in well under that.

### RSA — `std.crypto.rsa`

```tauraro
from std.crypto.rsa import Rsa, RsaPublicKey, RsaPrivateKey
from std.crypto.pkutil import Bytes

# (inside a `throws str` function)
mut pk = Rsa.public_from_pem(pem_text)?               # SPKI or PKCS#1 public
mut priv = Rsa.private_from_pem(key_text)?             # PKCS#1 or PKCS#8
mut sig = Rsa.sign_pkcs1(priv, "sha256", Bytes.from_str("hello"))?
mut ok = Rsa.verify_pkcs1(pk, "sha256", Bytes.from_str("hello"), sig)    # bool
mut pss = Rsa.sign_pss(priv, "sha256", msg, 32)?       # random 32-byte salt
mut ok2 = Rsa.verify_pss(pk, "sha256", msg, pss, 32)  # salt_len < 0: accept any
mut ct = Rsa.oaep_encrypt(pk, "sha256", secret, label)?
mut pt = Rsa.oaep_decrypt(priv, "sha256", ct, label)?
```

| API | Notes |
|---|---|
| `Rsa.public_from_pem/der`, `private_from_pem/der` | auto-detect SPKI vs PKCS#1, PKCS#8 vs PKCS#1; keys must be >= 512 bits; encrypted PEM unsupported |
| `Rsa.public_to_der/pem` (SPKI), `private_to_der/pem` (PKCS#1) | byte-identical to OpenSSL's output |
| `sign_pkcs1`, `verify_pkcs1`, `sign_pkcs1_digest`, `verify_pkcs1_digest` | EMSA-PKCS1-v1_5; verification compares the whole encoded message (no lenient parsing) |
| `sign_pss`, `sign_pss_with_salt`, `verify_pss` | RSASSA-PSS, MGF1 with the message hash |
| `oaep_encrypt`, `oaep_encrypt_with_seed`, `oaep_decrypt`, `oaep_max_message` | RSAES-OAEP (SHA-1/256/384/512 + label); decrypt failures all say `rsa: decryption error` |

`verify_*` never raise: any malformed input is simply `false`. PKCS#1 v1.5 *encryption* and
key generation are deliberately not provided.

**Limitations (honest):** the arithmetic is variable-time and there is no RSA blinding, so
**private-key operations are not hardened against local timing/cache attackers**; CRT results are
re-checked with the public key. Public-key verification handles only public data. SHA-1 is accepted for
legacy interop only.
---

## std.crypto.hmac — binary-safe HMAC (SHA-1 / 256 / 384 / 512)

`Hmac.sha256` / `sha256_str` (above) hash the message with `strlen`, so a message
containing a NUL byte is truncated. `Hmac.compute` takes the true length of its
`str` arguments and so is safe for counters, nonces and other binary data:

| Method | Signature | Description |
|---|---|---|
| `Hmac.compute` | `(alg: str, key: str, msg: str) -> str` | RFC 2104 HMAC; `alg` is `"sha1"`, `"sha256"`, `"sha384"` or `"sha512"`. Returns the RAW tag, or `""` for an unknown algorithm. Keys longer than the block size are hashed first. |
| `Hmac.compute_hex` | same | Lowercase hex tag. |
| `Hmac.verify` | `(alg, key, msg, tag) -> bool` | Constant-time tag check (`CryptoBytes.ct_eq`). |
| `Hmac.hash_raw` / `block_size` / `digest_size` | | Helpers. |

Verified against the RFC 2202 / 4231 cases and Python `hmac` (`tests/lang/115_hmac_otp.tr`).

---

## std.crypto.otp — HOTP and TOTP one-time passwords

```tauraro
from std.crypto.otp import Hotp, Totp, Otp
```

| Method | Signature | Description |
|---|---|---|
| `Hotp.generate` | `(secret: str, counter: int, digits: int, alg: str) -> str` | RFC 4226 code, zero-padded; `digits` 6..10, `alg` `"sha1"`/`"sha256"`/`"sha512"`; `""` on invalid parameters. |
| `Hotp.verify` | `(secret, code, counter, look_ahead, digits, alg) -> int` | Constant-time check over a counter window; returns the matching counter or `-1`. |
| `Totp.generate` | `(secret, unix_time, period, digits, alg) -> str` | RFC 6238 code. |
| `Totp.now` | `(secret_b32: str) -> str` | Code for the current time (SHA-1, 6 digits, 30 s) — what authenticator apps show. |
| `Totp.verify` / `verify_b32` | `(secret, code, unix_time, window, period, digits, alg) -> int` | Accepts steps within +/- `window`; returns the matched time step or `-1`. |
| `Otp.decode_secret` / `encode_secret` | `(b32) -> str` / `(raw) -> str` | Base32 secrets (via `std.encoding.base32`); decoding tolerates lower case, spaces, dashes and padding. |
| `Otp.generate_secret_b32` / `generate_secret` | `(nbytes: int) -> str` | Fresh random secret from the OS CSPRNG (20 bytes recommended). |
| `Otp.provisioning_uri` | `(kind, issuer, account, secret_b32, alg, digits, period, counter) -> str` | `otpauth://` Key URI for QR codes. |

```tauraro
mut secret = Otp.generate_secret_b32(20)          # store; show as QR via provisioning_uri
mut code = Totp.now(secret)                       # "492039"
mut step = Totp.verify_b32(secret, user_code, Totp.unix_now(), 1)
if step > last_used_step:                         # reject replays of the same step yourself
    last_used_step = step
```

Verified against the RFC 4226 Appendix D vectors and the RFC 6238 Appendix B vectors for
SHA-1, SHA-256 and SHA-512 (`tests/lang/115_hmac_otp.tr`). Verification does not store used
codes or rate-limit; both are the caller's job.


### ECDSA and ECDH — `std.crypto.ecdsa`

NIST **P-256** and **P-384** (`Ecdsa.p256()`, `Ecdsa.p384()`): ECDSA with **RFC 6979 deterministic nonces**,
and ECDH. Verified against the RFC 6979 appendix vectors, Python `cryptography` (deterministic
ECDSA), OpenSSL-made signatures and `openssl pkeyutl -derive`; our signatures pass `openssl dgst -verify`.

```tauraro
from std.crypto.ecdsa import Ecdsa, EcPublicKey, EcPrivateKey
from std.crypto.pkutil import Bytes

# (inside a `throws str` function)
mut priv = Ecdsa.generate(Ecdsa.p256())                  # or private_from_pem / private_from_scalar
mut sig = Ecdsa.sign(priv, "sha256", Bytes.from_str("hi"))?        # DER SEQUENCE{r, s}
mut ok = Ecdsa.verify(priv.public_key(), "sha256", Bytes.from_str("hi"), sig)
mut raw = Ecdsa.sign_raw(priv, "sha256", msg)?                     # r||s (JWS ES256 form)
mut pt = priv.public_key().to_point(true)                          # 33-byte compressed SEC1 point
mut pk = Ecdsa.public_from_pem(spki_text)?                         # validates the point is on the curve
mut shared = Ecdsa.ecdh(priv, peer_public_key)?                    # x coordinate, 32 bytes
```

| API | Notes |
|---|---|
| `Ecdsa.generate(curve)`, `private_from_scalar(curve, d_bytes)` | random / explicit private key |
| `private_from_pem/der`, `public_from_pem/der`, `public_from_point(curve, bytes)` | SEC1 `EC PRIVATE KEY`, PKCS#8, SPKI, `04||X||Y` and compressed `02/03||X` points; every point is validated |
| `private_to_pem/der` (SEC1), `public_to_pem/der` (SPKI), `pk.to_point(compressed)` | byte-identical to OpenSSL's encodings |
| `sign`, `sign_raw`, `sign_digest`, `sign_digest_sig` | hash names `sha1/sha256/sha384/sha512`; the same hash seeds the RFC 6979 nonce |
| `verify`, `verify_raw`, `verify_digest`, `verify_rs` | return `bool`, never raise |
| `sig_to_der(r, s)`, `sig_from_der(der)` | encoding conversion (`EcSignature{r, s}`) |
| `ecdh(priv, pk)` | raw shared secret — feed it to a KDF (`Kdf` / hash) before use |
| `sign_with_k` | explicit nonce, for test vectors only; never use a nonce twice |

**Limitations (honest):** scalar multiplication is plain left-to-right double-and-add on variable-time
bignum code — **not constant-time**; do not use it where an attacker can time private-key operations
(signing, ECDH with a long-lived key). Deterministic nonces remove the bad-RNG failure mode, not side
channels. Only P-256 and P-384 (prime field, `a = -3`, cofactor 1); no P-521, secp256k1 or Ed448.
About 10 ms per P-256 scalar multiplication (a verify costs two).
---

## std.crypto.argon2 — Argon2d / Argon2i / Argon2id (RFC 9106)

```tauraro
from std.crypto.argon2 import Argon2, Argon2Params
```

The memory-hard password-hashing function (`std.crypto.kdf` only offers PBKDF2). Pure Tauraro
on top of `std.crypto.blake2`. Argon2id is the variant to use for password storage.

| Method | Signature | Description |
|---|---|---|
| `Argon2.hash_password` | `(password: str) -> str` | PHC string `$argon2id$v=19$m=19456,t=2,p=1$<salt>$<tag>` with a random 16-byte salt (OWASP-minimum parameters). |
| `Argon2.hash_password_with` | `(password, variant, t, m, p) -> str` | Same with chosen variant (`"argon2id"`/`"argon2i"`/`"argon2d"`) and costs (`m` in KiB); `""` on invalid parameters. |
| `Argon2.verify` | `(encoded: str, password: str) -> bool` | Verifies against any PHC string (parameters come from the string); constant-time tag compare; malformed input is `false`. |
| `Argon2.needs_rehash` | `(encoded, t, m, p) -> bool` | True if the stored hash is weaker than the given minimums or not Argon2id. |
| `Argon2.hash_raw` | `(variant, password, salt, t, m, p, taglen, secret, ad) throws str -> str` | Raw tag with every RFC parameter (optional `secret` / `ad`, `""` if unused). |
| `Argon2.id` / `i` / `d` | `(password, salt, t, m, p, taglen) throws str -> str` | Raw tag, no secret/ad. |
| `Argon2.encode` / `decode` | | Build / parse PHC strings (`Argon2Params`: `ok`, `err`, `variant`, `m`, `t`, `p`, `salt`, `hash`). |

```tauraro
mut stored = Argon2.hash_password("correct horse battery staple")   # store this
if Argon2.verify(stored, attempt):
    if Argon2.needs_rehash(stored, 3, 65536, 1):
        stored = Argon2.hash_password_with(attempt, "argon2id", 3, 65536, 1)
```

Constraints (RFC 9106): `t >= 1`, `m >= 8 * p` KiB (rounded down to a multiple of `4 * p`),
`p >= 1`, tag >= 4 bytes, salt >= 8 bytes (use 16 random bytes). Only Argon2 version 19 (0x13)
is implemented; `v=16` strings are rejected. Lanes are computed one after another on a single
thread (the output is identical to a parallel run), at roughly 400 MiB/s of memory traffic per
core. The memory block is wiped before it is freed; the password `str` itself cannot be.

Verified against the three RFC 9106 section 5 vectors and ~25 further cases generated with
`openssl kdf ARGON2D/I/ID` (rounding, long tags, multi-lane, address-block regeneration, NULs,
empty inputs), plus the reference-implementation example hash (`tests/lang/116_argon2.tr`).
bcrypt is not provided (use Argon2id; it is the stronger, modern choice).

---

## std.crypto.blake3 — BLAKE3

```tauraro
from std.crypto.blake3 import Blake3, Blake3Hasher
```

| Method | Signature | Description |
|---|---|---|
| `Blake3.hash` / `hash_hex` | `(data: str, n: int) -> str` | Default mode with `n` output bytes (32 standard). BLAKE3 is an XOF: the first 32 bytes of a 64-byte output equal the 32-byte hash. |
| `Blake3.keyed_hash` / `keyed_hash_hex` | `(key: str, data: str, n: int) -> str` | MAC / PRF mode; `key` must be exactly 32 bytes (otherwise `""`). |
| `Blake3.derive_key` / `derive_key_hex` | `(context: str, material: str, n: int) -> str` | KDF mode; `context` should be a hard-coded, globally unique, application-specific string. |
| `Blake3Hasher.new()` / `new_keyed(key)` / `new_derive_key(context)` | `-> Blake3Hasher` | Incremental hasher: `update(data)`, `update_bytes(data, n)`, `finalize(n)` / `finalize_hex(n)` (repeatable, non-consuming), `is_ok()`, `free()`. |

```tauraro
print(Blake3.hash_hex("abc", 32))
mut session_key = Blake3.derive_key("myapp 2026-10-11 session key", shared_secret, 32)
```

The full tree mode (1024-byte chunks, chaining-value stack, parent nodes) is implemented as
in the reference implementation, sequentially (no SIMD / threads): about 50 MB/s. Verified
against the official vectors reproducible offline (empty, 1, 3, 1023, 1024, 1025 bytes, keyed
and derive_key at length 0) and a spec-derived Python model for 1..102400-byte inputs, keyed,
derive_key and long outputs (`tests/lang/112_blake3.tr`). The official `test_vectors.json`
file was not available offline, so its remaining entries were not run.

Rough speeds of the pure-Tauraro hashes on one x86-64 core: BLAKE2b ~100 MB/s, BLAKE3 ~50 MB/s,
SHA-3-256 ~20 MB/s.

