# Advanced — Channel Select

> This is an advanced topic. Core Tauraro development does not require understanding this. See the [Advanced Docs Index](README.md).

---

## Overview

Basic channel usage (see [chapter 16](../16_concurrency.md)) covers sending and receiving on a single channel. `select:` lets you wait on *multiple* channel operations simultaneously, taking whichever one is ready first.

---

## When You Need This

- You are reading from multiple producers and want to process whichever message arrives first (fan-in)
- You need a timeout: do the channel operation, but give up after N milliseconds
- You need a non-blocking channel check: attempt an operation, but don't block if nothing is ready
- You are implementing a cancellation mechanism using a "done" channel

---

## Syntax Reference

```python
select:
    case msg = chan_a.recv():
        # msg received from chan_a
        print(f"got {msg} from a")

    case chan_b.send(42):
        # successfully sent 42 to chan_b
        print("sent to b")

    case timeout(100):
        # no arm was ready within 100 milliseconds
        print("timed out")

    default:
        # no arm is ready right now (non-blocking)
        print("nothing ready")
```

Rules:
- Each `case` is a channel operation: `recv()` or `send(value)`
- At most one `timeout(ms)` arm is allowed per `select:`
- At most one `default` arm is allowed per `select:`
- `timeout` and `default` are mutually exclusive — a `select:` has one or the other, not both
- If multiple cases are ready simultaneously, one is chosen non-deterministically

---

## Examples

### Fan-in: reading from two producers

```python
def merge(chan_a: Chan[str], chan_b: Chan[str], out: Chan[str]) -> void:
    while true:
        select:
            case msg = chan_a.recv():
                out.send(msg)
            case msg = chan_b.recv():
                out.send(msg)
```

This loop non-deterministically reads from whichever channel has a message and forwards it to `out`. Neither producer blocks the other.

### Timeout: give up if nothing arrives

```python
def fetch_with_timeout(req_chan: Chan[str], resp_chan: Chan[str]) -> Option[str]:
    req_chan.send("fetch:data")
    select:
        case result = resp_chan.recv():
            return Option.some(result)
        case timeout(500):    # 500ms
            return Option.none()
```

### Non-blocking check with default

```python
def try_recv(ch: Chan[int]) -> Option[int]:
    select:
        case val = ch.recv():
            return Option.some(val)
        default:
            return Option.none()    # nothing in channel right now
```

Use `default` for polling — check channels without ever blocking.

### Cancellation via a "done" channel

A common pattern for stopping a worker task gracefully:

```python
def worker(work: Chan[str], done: Chan[bool]) -> void:
    while true:
        select:
            case task = work.recv():
                process(task)
            case _ = done.recv():
                print("worker stopping")
                return

def main():
    mut work = Chan[str].init(16)
    mut done = Chan[bool].init(1)

    task_group:
        spawn worker(work, done)
        work.send("task1")
        work.send("task2")
        # signal shutdown
        done.send(true)
```

### Sending to one of multiple channels

```python
def broadcast(msg: str, chans: List[Chan[str]]) -> void:
    mut sent = false
    while not sent:
        select:
            case chans[0].send(msg):
                sent = true
                print("sent to channel 0")
            case chans[1].send(msg):
                sent = true
                print("sent to channel 1")
            case timeout(1000):
                print("all channels full — retrying")
```

---

## Common Mistakes

**Forgetting `default` when you don't want to block.** A `select:` with no `default` and no `timeout` blocks until one arm is ready. If all channels are empty and will stay empty, this deadlocks:

```python
# DEADLOCK if both channels are empty and no sender exists:
select:
    case msg = chan_a.recv():
        process(msg)
    case msg = chan_b.recv():
        process(msg)
# hangs forever if no one sends to chan_a or chan_b
```

Add a `default:` arm or a `timeout(ms):` arm to prevent this.

**Using `select:` for a single channel.** If you only have one channel, `select:` adds no value. Just call `.recv()` or `.send()` directly.

**Expecting deterministic ordering.** When multiple arms are ready at the same time, the runtime picks one arbitrarily. Do not write code that depends on a specific arm being chosen first.

**Mixing `timeout` and `default`.** These are mutually exclusive. `default` fires immediately if nothing is ready; `timeout` waits for N ms first. If you have both, the compiler will reject the `select:` block.

---

## Best Practices

- **Always have an exit arm.** Every `select:` loop should have either a `timeout` arm or a `default` arm, or a `done` channel that can break the loop. Unbounded blocks make programs hard to shut down cleanly.
- **Use `timeout` for operations with SLAs.** Network requests, database queries, lock acquisition — anything that should fail rather than wait indefinitely.
- **Keep `select:` cases short.** The case body should be minimal — receive the value, record it, continue. Heavy work inside a `select:` arm delays the next iteration and may starve other arms.
- **Use a dedicated `done` channel for structured cancellation** rather than a flag variable. Channel-based cancellation is composable and works across goroutines.
- **Size channels appropriately.** A zero-capacity (unbuffered) channel in a `select:` arm blocks until a sender is ready. A buffered channel allows the arm to succeed immediately if space is available.

---

See also:
- [16 — Concurrency](../16_concurrency.md)
- [06 — Sendable](06_sendable.md)
