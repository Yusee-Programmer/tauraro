# std.kv -- embedded crash-safe key-value store

```tauraro
from std.kv import KvStore, KvOptions, KvBatch, KvPair, KvStats
```

A Bitcask-style store in one file: an append-only log of CRC-protected records, an in-memory index (hash table plus a
skip list for sorted keys) rebuilt on open, a hint file for fast start-up, compaction by atomic rename, per-key TTL,
atomic batches, and a lock file so two handles (or processes) never write the same store.

```tauraro
mut st = KvStore.open("app.kv", KvOptions.defaults())?
st.put("user:1", "alice")?
st.put_ttl("session:9", "tok", 60000)?
print(st.get("user:1")?)
for k in st.iter_prefix("user:")?: print(k)
mut b = KvBatch.init()
b.put("a", "1"); b.delete("user:1")
st.write_batch(b)?
st.compact()?
st.close()
```

## API

| Call | Description |
|---|---|
| `KvStore.open(path, opts)?` | Opens or creates `path` (also `path.lock`, `path.hint`). Recovers a damaged tail. |
| `put(k, v)?`, `put_ttl(k, v, ttl_ms)?` | Append a record. `put` clears any earlier TTL. |
| `get(k)?`, `get_or(k, fallback)?` | `get` raises `"kv: key not found"` for missing or expired keys. |
| `delete(k)? -> bool` | Writes a tombstone when the key existed. |
| `contains(k)`, `ttl(k)`, `len()` | `ttl`: ms left, `-1` none, `-2` missing. |
| `keys()?`, `iter_prefix(p)?`, `iter_prefix_items(p)?` | Sorted (byte order) live keys / `KvPair{key, value}`. |
| `write_batch(b)?` | Atomic group commit: one record, one CRC. |
| `purge_expired()? -> int` | Tombstones every expired key. |
| `compact()?` | Rewrites live records into `path.compact`, fsyncs, renames over the data file, rebuilds the index, rewrites the hint. |
| `sync()?`, `verify()? -> str`, `stats() -> KvStats`, `close()` | `verify` returns `""` when every record's CRC checks out. |

`KvOptions`: `sync_policy` (`KV_SYNC_ALWAYS` fsync every write, `KV_SYNC_INTERVAL` at most every `sync_interval_ms`,
`KV_SYNC_NEVER` only the OS decides), `max_key_size` (1024), `max_value_size` (64 MiB), `max_file_size` (0 = unlimited; a
put that would pass it raises `"store full"` -- compact to reclaim space), `create`, `use_hint`, `truncate_corrupt`, `lock`.
`KvStats`: `keys`, `file_size`, `garbage_bytes`, `garbage_ratio()`, `records`, `discarded_bytes`, `puts`, `gets`, `deletes`, `syncs`,
`compactions`, `hint_loaded`. `set_clock(ms)` freezes the store's clock (tests); `set_clock(-1)` restores it.

## File format

`"TAURAKV1"` + u64 generation, then records (little endian): `crc32 u32 | type u8 | expiry u64 | klen u32 | vlen u32 | key | value`. Types:
1 put, 2 delete, 3 batch (value = concatenated put/delete bodies, one CRC for the lot). Expiry is absolute ms since the epoch
(0 = never). The hint file (`"TKHINT01"`, generation, covered size, garbage, entries, CRC) is only used when its generation and CRC match
and it does not claim more than the file holds; records appended after it are replayed.

## Crash behaviour

On open the log is read until the first record that is short, malformed or fails its CRC; it and everything after it are dropped
(the file is truncated there, `stats().discarded_bytes` says how much; with `truncate_corrupt = false` open raises instead and leaves
the file alone). A write cut off at any byte therefore never appears half-applied, and a batch appears completely or not at all. Tests cut
the log at every byte of the last plain and batch record and flip every byte of it. Corruption in the middle of the log discards the
records after it too -- this is Bitcask's trade-off; take backups for that case.

## Limits and notes

* Keys are non-empty, NUL-free byte strings. Values are binary-safe.
* The whole key set (and 5 integers per key) lives in memory; values stay on disk and each `get` is one `pread`. CRCs are checked at open,
  `verify()` and compaction, not on every `get`. The hint file is trusted for the prefix it covers (delete it or set `use_hint = false` to force a scan).
* Single writer: the lock is an exclusive `flock` / `LockFileEx` on `path.lock` held while the store is open. The lock file itself is never deleted.
* Single-file mode only; there is no directory mode or multi-reader API.
* fsync: `KV_SYNC_ALWAYS` syncs the file after each record (and `close`); the directory is synced after compaction on POSIX.
* Expired keys are hidden lazily and reclaimed by `purge_expired()` / `compact()`.
