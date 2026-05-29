# std.net — Networking

```tauraro
from std.net.tcp import TcpStream, TcpListener
from std.net.udp import UdpSocket
from std.net.dns import Dns
from std.net.url import Url, url_encode, url_decode
```

> **Platform note** — All socket classes require `-lws2_32` on Windows (Winsock).  
> The `tau_build.sh` script links this automatically.

---

## std.net.tcp — TCP client and server

**When**: You need a reliable, ordered byte stream — HTTP clients, chat servers, file transfer, RPC.
**Why**: Wraps POSIX/Winsock sockets into two clean classes: `TcpStream` for clients and `TcpListener` for servers.

### TcpStream — TCP client connection

```tauraro
mut s = TcpStream.connect("example.com", 80)
if s.connected:
    s.send("GET / HTTP/1.0\r\n\r\n")
    mut resp = s.recv(4096)
    s.close()
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `TcpStream.connect` | `(host: str, port: int) -> TcpStream` | `TcpStream` | Open a TCP connection to `host:port`. Check `.connected` before use. |
| `send` | `(data: str) -> int` | `int` | Send bytes. Returns number of bytes sent, or `-1` on error. |
| `recv` | `(cap: int) -> str` | `str` | Receive up to `cap` bytes. Returns `""` on disconnect or error. |
| `close` | `()` | `void` | Close the connection. |
| `is_connected` | `() -> bool` | `bool` | `true` while the connection is alive. |
| `peer_addr` | `() -> str` | `str` | Remote address as `"ip:port"`. Returns `""` if not connected. |

Fields: `connected: bool`, `host: str`, `port: int`, `fd: int`.

### TcpListener — TCP server

```tauraro
mut srv = TcpListener.listen("0.0.0.0", 8080)
if srv.listening:
    mut client = srv.accept()       # blocks until a client connects
    mut msg    = client.recv(1024)
    client.send("OK\r\n")
    client.close()
    srv.close()
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `TcpListener.bind` | `(host: str, port: int, backlog: int) -> TcpListener` | `TcpListener` | Bind and listen; `backlog` controls the connection queue depth. |
| `TcpListener.listen` | `(host: str, port: int) -> TcpListener` | `TcpListener` | `bind` with a default backlog of 128. |
| `accept` | `() -> TcpStream` | `TcpStream` | Block until a client connects; returns a ready `TcpStream`. |
| `close` | `()` | `void` | Stop accepting new connections. |
| `is_listening` | `() -> bool` | `bool` | `true` while the server socket is open. |

Fields: `listening: bool`, `host: str`, `port: int`, `fd: int`.

### Example — echo server

```tauraro
from std.net.tcp import TcpStream, TcpListener

mut srv = TcpListener.listen("127.0.0.1", 9000)
if not srv.listening:
    print("bind failed")
else:
    mut c = srv.accept()
    mut data = c.recv(256)
    c.send(data)           # echo back
    c.close()
    srv.close()
```

---

## std.net.udp — UDP sockets

**When**: You need low-latency, connectionless datagrams — DNS queries, game state, telemetry, multicast.
**Why**: Skips TCP's handshake and retransmit overhead; `UdpSocket` handles both sending-only and listening sockets.

### UdpSocket

| Method | Signature | Returns | Description |
|---|---|---|---|
| `UdpSocket.new` | `() -> UdpSocket` | `UdpSocket` | Create a new UDP socket (not yet bound). Use this for send-only sockets. |
| `UdpSocket.bind` | `(port: int) -> UdpSocket` | `UdpSocket` | Create and bind to a local port (for receiving). |
| `send_to` | `(data: str, host: str, port: int) -> int` | `int` | Send `data` to `host:port`. Returns bytes sent, or `-1` on error. |
| `recv` | `(cap: int) -> str` | `str` | Receive a datagram up to `cap` bytes. Returns `""` on error. |
| `recv_from` | `(cap: int) -> str` | `str` | Same as `recv`; sender address is captured internally. |
| `close` | `()` | `void` | Close the socket. |
| `is_open` | `() -> bool` | `bool` | `true` while the socket is valid. |

Fields: `open: bool`, `port: int`, `fd: int`.

### Example

```tauraro
from std.net.udp import UdpSocket

# Send a message
mut s = UdpSocket.new()
if s.is_open():
    s.send_to("hello", "127.0.0.1", 5555)
    s.close()

# Receive on port 5555
mut r = UdpSocket.bind(5555)
if r.is_open():
    mut msg = r.recv(512)
    print("got: " + msg)
    r.close()
```

---

## std.net.dns — DNS resolution

**When**: You need to look up a hostname before connecting, validate IP strings, or do reverse DNS queries.
**Why**: `Dns` is a static-method class wrapping `getaddrinfo`/`getnameinfo` — no setup required.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Dns.resolve` | `(hostname: str) -> str` | `str` | Resolve a hostname to its IPv4 address string. Returns `""` on failure. |
| `Dns.reverse` | `(ip: str) -> str` | `str` | Reverse-lookup an IPv4 address to its canonical hostname. Returns `""` on failure. |
| `Dns.is_ipv4` | `(s: str) -> bool` | `bool` | `true` when `s` looks like a valid IPv4 address (`w.x.y.z`). |

### Example

```tauraro
from std.net.dns import Dns

mut ip = Dns.resolve("localhost")
print("localhost → " + ip)       # "127.0.0.1"

print(str(Dns.is_ipv4("192.168.1.1")))   # true
print(str(Dns.is_ipv4("not-an-ip")))     # false

mut host = Dns.reverse("127.0.0.1")
print("127.0.0.1 → " + host)
```

---

## std.net.url — URL building and encoding

**When**: You need to construct HTTP request URLs or percent-encode query parameters.
**Why**: Avoids manual string concatenation of scheme, path, and query; `url_encode`/`url_decode` handle the % escaping correctly.

### Url class

Mutable URL builder with chainable setters.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Url.init` | `() -> Url` | `Url` | Create an empty URL. |
| `Url.parse` | `(raw: str) -> Url` | `Url` | Parse a URL string into its components. |
| `with_scheme` | `(scheme: str) -> Url` | `Url` | Set the scheme (e.g. `"https"`). |
| `with_path` | `(path: str) -> Url` | `Url` | Set the path component. |
| `with_query` | `(query: str) -> Url` | `Url` | Set the raw query string (without `?`). |
| `to_str` | `() -> str` | `str` | Reconstruct the full URL string. |
| `is_https` | `() -> bool` | `bool` | `true` if scheme is `"https"`. |

### url_encode / url_decode

Percent-encode and decode arbitrary strings for safe use in URL components.

| Function | Signature | Returns | Description |
|---|---|---|---|
| `url_encode` | `(s: str) -> str` | `str` | Encode all non-unreserved characters as `%XX`. Unreserved: letters, digits, `-`, `_`, `.`, `~`. |
| `url_decode` | `(s: str) -> str` | `str` | Decode `%XX` sequences back to raw bytes. |

### Example

```tauraro
from std.net.url import Url, url_encode, url_decode

mut u = Url.init()
u = u.with_scheme("https")
u = u.with_path("/api/v1/search")
u = u.with_query("q=" + url_encode("hello world"))
print(u.to_str())
# https:/api/v1/search?q=hello%20world

mut enc = url_encode("a+b=c&d=e")
print(enc)                     # "a%2Bb%3Dc%26d%3De"
print(url_decode(enc))         # "a+b=c&d=e"

mut parsed = Url.parse("https://example.com/path?q=1")
print(str(parsed.is_https()))  # true
```
