# std.net — Networking

```tauraro
from std.net.tcp  import TcpStream, TcpListener
from std.net.udp  import UdpSocket
from std.net.dns  import Dns
from std.net.url  import Url
from std.net.http import HttpClient, HttpClientResponse, HttpHeader
from std.net.client    import HttpSession, ClientRequest, HttpReply, HeaderList
from std.net.cookie    import CookieJar, Cookie
from std.net.multipart import FormBuilder, MultipartBuilder
from std.net.imap      import ImapClient
from std.net.pop3      import Pop3Client
from std.net.mime      import Mime, MediaType
from std.net.email     import EmailMessage, Address, Attachment, MimePart
from std.net.dnsmsg    import Dns, DnsMessage
from std.net.ntp       import Ntp
from std.net.mdns      import Mdns
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

## std.net.url — URL parsing and building

**When**: You need to parse or construct HTTP request URLs, or percent-encode query parameters.
**Why**: A single class covering parse, build, and encode — no manual string concatenation of scheme, host, port, path, query, and fragment.

### Fields

| Field | Type | Description |
|---|---|---|
| `scheme` | `str` | e.g. `"https"` |
| `host` | `str` | e.g. `"example.com"` |
| `port` | `int` | `0` = use default |
| `path` | `str` | e.g. `"/api/v1/items"` |
| `query` | `str` | Raw query string without `?` |
| `fragment` | `str` | Fragment identifier without `#` |

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Url.init` | `(host: str, port: int, path: str) -> Url` | `Url` | Create a URL with scheme `"http"` and empty query/fragment. |
| `Url.parse` | `(url_str: str) -> Url` | `Url` | Parse a full URL string into its components. |
| `with_scheme` | `(scheme: str) -> Url` | `Url` | Set the scheme and return `self`. |
| `with_query` | `(query: str) -> Url` | `Url` | Set the query string (without `?`) and return `self`. |
| `with_fragment` | `(fragment: str) -> Url` | `Url` | Set the fragment (without `#`) and return `self`. |
| `to_string` | `() -> str` | `str` | Reconstruct the full URL string. |
| `default_port` | `() -> int` | `int` | Conventional port for this scheme: `http`=80, `https`=443, `ftp`=21, `ssh`=22, `smtp`=25. |
| `is_https` | `() -> bool` | `bool` | `true` when scheme is `"https"`. |
| `is_valid` | `() -> bool` | `bool` | `true` when both `scheme` and `host` are non-empty. |
| `Url.encode_component` | `(s: str) -> str` | `str` | Percent-encode a URL component value. Unreserved chars (`A-Z a-z 0-9 - _ . ~`) are kept as-is. |
| `Url.decode_component` | `(s: str) -> str` | `str` | Decode `%XX` sequences back to raw bytes. |
| `free` | `()` | `void` | Release this `Url` instance. |

### Example

```tauraro
from std.net.url import Url

# Build a URL manually
mut u = Url.init("api.example.com", 0, "/search")
u = u.with_scheme("https")
u = u.with_query("q=" + Url.encode_component("hello world"))
u = u.with_fragment("results")
print(u.to_string())
# https://api.example.com/search?q=hello%20world#results

# Parse an existing URL
mut p = Url.parse("https://example.com:8443/path?page=2#top")
print(p.scheme)                    # "https"
print(p.host)                      # "example.com"
print(str(p.port))                 # 8443
print(p.path)                      # "/path"
print(p.query)                     # "page=2"
print(p.fragment)                  # "top"
print(str(p.is_valid()))           # true
print(str(p.default_port()))       # 443

# Percent-encoding
mut enc = Url.encode_component("a+b=c&d=e")
print(enc)                         # "a%2Bb%3Dc%26d%3De"
print(Url.decode_component(enc))   # "a+b=c&d=e"
```

---

## std.net.http — HTTP/1.0 Client

**When**: You need to make HTTP requests — REST APIs, web scraping, health checks, webhooks.
**Why**: A stateful client with configurable headers and timeout; all verbs return a structured `HttpClientResponse` with parsed status code and response headers.

### HttpHeader

A simple `name`/`value` pair.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `HttpHeader.init` | `(name: str, value: str) -> HttpHeader` | `HttpHeader` | Create a header pair. |
| `free` | `()` | `void` | Release this `HttpHeader` instance. |

### HttpClientResponse

| Field / Method | Type / Signature | Description |
|---|---|---|
| `status` | `int` | HTTP status code (e.g. `200`, `404`). |
| `body` | `str` | Response body text. |
| `headers` | `Map[str, str]` | Parsed response headers (name → value; a repeated header keeps only the last value — use `HttpReply` below if you need all of them, e.g. `Set-Cookie`). |
| `HttpClientResponse.init` | `(status: int, body: str) -> HttpClientResponse` | Construct a response directly (status + body; empty headers map). |
| `is_ok` | `() -> bool` | `true` for 2xx status codes. |
| `is_redirect` | `() -> bool` | `true` for 3xx status codes. |
| `is_error` | `() -> bool` | `true` for 4xx/5xx status codes. |
| `header` | `(name: str) -> str` | Return a response header value, or `""` if absent. |
| `to_string` | `() -> str` | Human-readable `"HTTP NNN Reason\n<body>"`. |
| `free` | `()` | Release this `HttpClientResponse` instance. |

### HttpClient

| Method | Signature | Returns | Description |
|---|---|---|---|
| `HttpClient.init` | `(host: str, port: int) -> HttpClient` | `HttpClient` | Create a client for `host:port`. Default timeout: 5000 ms. |
| `set_header` | `(name: str, value: str)` | `void` | Add a header to all subsequent requests. |
| `set_timeout` | `(ms: int)` | `void` | Set timeout hint in milliseconds. |
| `get` | `(path: str) -> HttpClientResponse` | `HttpClientResponse` | Send a `GET` request. |
| `post` | `(path: str, data: str) -> HttpClientResponse` | `HttpClientResponse` | Send a `POST` with `application/x-www-form-urlencoded` body. |
| `post_json` | `(path: str, data: str) -> HttpClientResponse` | `HttpClientResponse` | Send a `POST` with `application/json` body. |
| `put` | `(path: str, data: str) -> HttpClientResponse` | `HttpClientResponse` | Send a `PUT` with `application/octet-stream` body. |
| `patch` | `(path: str, data: str) -> HttpClientResponse` | `HttpClientResponse` | Send a `PATCH` with `application/octet-stream` body. |
| `delete` | `(path: str) -> HttpClientResponse` | `HttpClientResponse` | Send a `DELETE` request. |
| `head` | `(path: str) -> HttpClientResponse` | `HttpClientResponse` | Send a `HEAD` request (response body will be empty per HTTP spec). |

### Example

```tauraro
from std.net.http import HttpClient

mut c = HttpClient.init("httpbin.org", 80)
c.set_header("Accept", "application/json")

# GET
mut r = c.get("/get")
if r.is_ok():
    print("status: " + str(r.status))
    print("content-type: " + r.header("Content-Type"))
    print(r.body)

# POST JSON
mut body = "{\"name\": \"alice\", \"score\": 42}"
mut r2 = c.post_json("/post", body)
print("posted: " + str(r2.status))

# HEAD — inspect headers without downloading a body
mut r3 = c.head("/get")
print(str(r3.status))                      # 200
print(r3.header("Content-Type"))           # "application/json"
```

---

## std.net.tls — TLS 1.3 client

A self-contained TLS 1.3 client (RFC 8446), no OpenSSL. Verifies the server's certificate chain and
host name against the system trust store by default (`std.crypto.x509`). Key exchange X25519 /
secp256r1 (secp384r1 via HelloRetryRequest); AES-128-GCM, AES-256-GCM, ChaCha20-Poly1305; server
signatures ECDSA P-256/P-384 and RSA-PSS; SNI, ALPN, KeyUpdate. Not supported: TLS 1.2 and older,
resumption/0-RTT, client certificates.

```tauraro
from std.net.tls import TlsStream, TlsOptions

mut s = TlsStream.connect("example.com", 443)?
s.write("GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n")?
mut resp = s.read_all()?
print(s.cipher_suite())                  # TLS_AES_128_GCM_SHA256
print(s.peer_certificates()[0].subject())
s.close()
```

| Member | Description |
|---|---|
| `TlsOptions.default()` / `TlsOptions.insecure()` | Settings: `verify` (default true), `alpn` (`"h2,http/1.1"`), `timeout_ms` (30000), `ca_pem` (extra roots), `only_ca_pem`. |
| `TlsStream.connect(host, port)?` / `connect_with(host, port, opts)?` | Resolve, connect (with timeout) and handshake. Raises with the TLS/verification error. |
| `TlsStream.wrap_fd(fd, host, opts)?` | Handshake on an already-connected socket (STARTTLS). `close()` leaves the fd open. |
| `write(data)?`, `read(cap)?`, `read_all()?`, `read_line(max)?` | I/O; `read` returns `""` at end of stream and raises on timeout. |
| `alpn()`, `cipher_suite()`, `version()`, `peer_certificates()` | Connection details. |
| `set_timeout(ms)`, `close()` | `close()` sends close_notify. |

---

## std.net.http2 — HTTP/2 client and HPACK

One connection, many concurrent requests (RFC 9113): https with ALPN `h2` (verified TLS 1.3) or
cleartext h2c with prior knowledge. Flow control, CONTINUATION, PING, SETTINGS, RST_STREAM and
GOAWAY are handled; server push is disabled. Response trailers are exposed (gRPC status).

```tauraro
from std.net.http2 import Http2Client, H2Headers

mut c = Http2Client.connect("nghttp2.org", 443)?
mut h = H2Headers.init()
mut a = c.submit("GET", "/httpbin/get", h, "", false)?
mut b = c.submit("POST", "/httpbin/post", h, "payload", true)?
c.wait_all()?                                   # both streams run concurrently
mut ra = c.response(a)
print(ra.status, ra.headers.get("content-type"), ra.body.len())
c.release(a)
c.release(b)
mut r = c.get("/httpbin/headers")?              # submit + wait + response + release
c.close()
```

| Member | Description |
|---|---|
| `Http2Client.connect(host, port)?` / `connect_with(host, port, Http2Options)?` | `Http2Options.default()` (https, verified) or `Http2Options.cleartext()`; fields `tls`, `verify`, `ca_pem`, `timeout_ms`. |
| `submit(method, path, headers, body, has_body)? -> int` | Start a request; returns the stream id. `default_headers` (user-agent) are merged in; hop-by-hop headers are dropped. |
| `wait(sid)?`, `wait_all()?`, `is_done(sid)` | Drive the connection. |
| `response(sid) -> Http2Response` | `status`, `headers`, `trailers` (`H2Headers`), `body` (binary-safe), `error`, `ok()`. |
| `request(...)?`, `get(path)?`, `post(path, body, content_type)?` | One-shot helpers. |
| `release(sid)`, `ping()`, `is_closing()`, `set_timeout(ms)`, `close()` | Housekeeping (`release` cancels an open stream). |

`H2Headers` is an ordered, case-insensitive header list (`add`, `set`, `get`, `has`, `remove`). `Hpack.init(table_size)`
exposes the compressor (`encode(headers)?`, `decode(block)?`, `set_max_table`, `table_size`) and `Huffman.encode/decode`
the RFC 7541 Huffman code. Sensitive headers (`authorization`, `cookie`, ...) are sent never-indexed.

---

## std.net.https — HTTPS/TLS Client

**When**: You need to call HTTPS endpoints — secure REST APIs, webhooks, OAuth flows.
**Why**: Identical API to `HttpClient`, over `std.net.tls` (built-in TLS 1.3, certificates verified).

> Certificate chain and host name are verified against the system trust store. On failure the
> response has `status: 0` and the TLS error as its body. `set_verify(false)` disables
> verification (testing only); `set_ca_pem(pem)` adds private roots. TLS 1.2-only servers are not
> supported.

```tauraro
from std.net.https import HttpsClient
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `HttpsClient.init` | `(host: str, port: int) -> HttpsClient` | `HttpsClient` | Create a TLS client. Default timeout: 10 000 ms. |
| `set_header` | `(name: str, value: str)` | `void` | Add a header to all requests. |
| `set_timeout` | `(ms: int)` | `void` | Connect and read timeout. |
| `set_verify` | `(v: bool)` | `void` | Turn certificate verification off/on (default on). |
| `set_ca_pem` | `(pem: str)` | `void` | Trust these PEM roots in addition to the system store. |
| `get` | `(path: str) -> HttpClientResponse` | `HttpClientResponse` | HTTPS GET. |
| `post` | `(path: str, data: str) -> HttpClientResponse` | `HttpClientResponse` | HTTPS POST (form-encoded). |
| `post_json` | `(path: str, data: str) -> HttpClientResponse` | `HttpClientResponse` | HTTPS POST (JSON). |
| `put` | `(path: str, data: str) -> HttpClientResponse` | `HttpClientResponse` | HTTPS PUT. |
| `patch` | `(path: str, data: str) -> HttpClientResponse` | `HttpClientResponse` | HTTPS PATCH. |
| `delete` | `(path: str) -> HttpClientResponse` | `HttpClientResponse` | HTTPS DELETE. |
| `head` | `(path: str) -> HttpClientResponse` | `HttpClientResponse` | HTTPS HEAD. |

### Example

```tauraro
from std.net.https import HttpsClient

mut c = HttpsClient.init("api.github.com", 443)
c.set_header("User-Agent", "tauraro/1.0")
c.set_header("Accept", "application/vnd.github.v3+json")

mut r = c.get("/users/octocat")
if r.is_ok():
    print(r.body)
```

---

## HTTP client toolkit — `std.net.client`, `std.net.cookie`, `std.net.multipart`

**When**: You need more than one fixed `host:port` GET — following redirects (even across hosts), keeping cookies between requests, sending form / multipart bodies, or streaming a large response to disk instead of holding it in memory.
**Why**: `HttpClient` above is bound to one host and does a single `recv()`. `HttpSession` is URL-based HTTP/1.1: it reads the whole response (Content-Length, chunked, or until close), follows redirects, and plugs into a `CookieJar`. It is a separate class (not a rewrite of `HttpClient`) so existing code keeps working.

```tauraro
from std.net.client    import HttpSession, ClientRequest
from std.net.multipart import FormBuilder, MultipartBuilder
```

### HttpSession

| Method | Signature | Description |
|---|---|---|
| `HttpSession.init` | `() -> HttpSession` | Defaults: idle timeout 30 s, follow up to 10 redirects, cookies off, UA `tauraro-http/1.0`, in-memory body cap 64 MiB. |
| `set_header` / `remove_header` | `(name, value)` / `(name)` | Default header sent on every request (a per-request header of the same name replaces it). |
| `set_basic_auth` / `set_bearer` / `set_user_agent` | `(user, pass)` / `(token)` / `(ua)` | Convenience for `Authorization` / `User-Agent`. |
| `set_timeout` | `(ms: int)` | Idle timeout: longest wait for more bytes (header or body). |
| `set_total_timeout` | `(ms: int)` | Wall-clock limit for the whole call, redirects included (`0` = none). |
| `set_follow_redirects` / `set_max_redirects` | `(bool)` / `(n)` | Redirect policy. `n <= 0` disables following. |
| `set_rewrite_post_on_301_302` | `(bool)` | `true` (default): `POST` becomes `GET` on 301/302 like browsers; `false`: keep the method. |
| `enable_cookies` / `disable_cookies` / `set_cookie_jar` / `cookie_jar` | | Cookie handling (off by default). |
| `set_max_body_bytes` | `(n)` | Cap for in-memory bodies (`0` = unlimited). |
| `set_allow_unverified_tls` | `(bool)` | Skip https certificate verification (testing only). |
| `set_ca_pem` | `(pem: str)` | Extra trusted roots (PEM) for https. |
| `get` / `head` / `delete` | `(url) -> HttpReply` | |
| `post` / `put` / `patch` | `(url, body, content_type) -> HttpReply` | |
| `post_json` | `(url, json) -> HttpReply` | |
| `post_form` | `(url, form: FormBuilder) -> HttpReply` | `application/x-www-form-urlencoded`. |
| `post_multipart` | `(url, mp: MultipartBuilder) -> HttpReply` | `multipart/form-data`. |
| `request` / `send` | `(method, url, body, ctype)` / `(req: ClientRequest)` | Full control; `ClientRequest` has `header()`, `with_body()`, `with_timeout()`, `with_follow()`. |
| `download_to_file` / `send_to_file` | `(url, path)` / `(req, path)` | Stream the body to disk (see below). |
| `stream_get` / `send_streaming` | `(url, on_chunk)` / `(req, on_chunk)` | Deliver the body to `on_chunk: def(str) -> bool` as it arrives; return `false` to abort. |

**Redirects.** 301/302/303/307/308 with a `Location` header are followed. 303 → `GET` (HEAD stays HEAD, body dropped); 301/302 turn `POST` into `GET` (configurable); 307/308 keep method and body. Relative `Location` values are resolved with `Url.resolve`. `Authorization`, `Proxy-Authorization` and an explicit `Cookie` header are dropped when a hop changes host, changes port, or downgrades https→http; jar cookies are re-evaluated for every hop. A loop or a long chain ends with `reply.error = "too many redirects (N)"`.

**Streaming.** `download_to_file` writes to `<path>.part` and renames it on success, so a failed download never leaves a partial `path`. The body is written only for 2xx responses (otherwise it is returned in `reply.body`, so you can read the error page). Writes come straight from raw receive buffers, so binary files are intact. `stream_get` hands decoded (de-chunked) text to the callback.

### HttpReply

`status`, `reason`, `body`, `url` (final URL), `error` (`""` when the exchange completed; otherwise e.g. `"timeout waiting for response"`, `"connection failed to host:port"`, `"too many redirects (10)"`, `"incomplete body: ..."`), `timed_out`, `redirects`, `bytes`, `chunked`, `saved_to`, `history` (every URL requested). Methods: `is_ok()` (no error and 2xx), `is_redirect()`, `is_error()`, `header(name)` (case-insensitive, first value), `header_all(name)`, `has_header`, `set_cookies()` (all `Set-Cookie` values), `content_type()`, `location()`, `content_length()`. A 4xx/5xx status is *not* a transport error: `error` stays `""`.

### CookieJar (`std.net.cookie`)

```tauraro
from std.net.cookie import CookieJar

mut jar = CookieJar.init()
jar.store_set_cookie("sid=abc; Path=/; HttpOnly; Max-Age=3600", "example.com", "/login")
print(jar.header_for("example.com", "/account", false))            # "sid=abc"
print(jar.header_for_url("https://example.com/account"))           # same, from a URL string
```

Parses `Domain`, `Path`, `Expires` (RFC 1123 / RFC 850 / asctime), `Max-Age` (wins over `Expires`), `Secure`, `HttpOnly`, `SameSite`. Host-only vs domain cookies, default-path, domain/path matching with proper boundaries, `Secure` only over https, expiry, `Max-Age=0` deletion, and RFC 6265 ordering (longer path first, then older) of the generated `Cookie` header. Also: `set(name, value, domain, path)`, `get`, `has`, `find`, `remove`, `purge_expired`, `clear`, `len`, and `*_at(..., now)` variants taking an explicit Unix time for deterministic tests. `http_date_parse(text) -> int` is exported too.

### FormBuilder / MultipartBuilder (`std.net.multipart`)

```tauraro
mut f = FormBuilder.init()
f.add("q", "hello world")            # build() -> "q=hello%20world"

mut m = MultipartBuilder.init()
m.add_field("title", "Report")
m.add_file_data("doc", "a.txt", "text/plain", "contents")
m.add_file("logo", "assets/logo.svg")      # from disk; Content-Type guessed from the extension
mut r = HttpSession.init().post_multipart("http://127.0.0.1:8080/upload", m)
```

`build()` returns the body and `content_type()` the header value (`multipart/form-data; boundary=...`); `with_boundary()` pins the boundary. Field/file names are quoted-escaped (`"` → `%22`, CR/LF removed). The server-side counterpart is watax's `req.multipart()`.

### Compression

In-memory requests (`get`, `post`, `request`, `send`, ...) send `Accept-Encoding: gzip, deflate, br`
and decode the response body: `reply.body` is the decoded content, `reply.content_encoding` names
the coding that was removed (`""` when none) and `reply.bytes` is the size on the wire. A body that
fails to decode sets `reply.error`. `download_to_file` and `stream_get` ask for `identity` and get the
bytes unchanged. `s.set_decompress(false)` turns it off; an explicit `Accept-Encoding` header is
sent as given. Decoded bodies are capped by `set_max_body_bytes`.

### Limitations

- `https://` uses `std.net.tls` (TLS 1.3 only, verified against the system roots); servers that only speak TLS 1.2 fail with a handshake error.
- TCP connect is blocking with no connect timeout; IPv4 only; no proxies, no keep-alive/connection reuse, no `Expect: 100-continue`, no `user:pass@host` or IPv6-literal URLs.
- Bodies are binary-safe everywhere (response bodies, request bodies, `stream_get` chunks and downloads keep NUL bytes).
- No public-suffix list for cookies (a dot-less `Domain` is rejected as a heuristic); `SameSite` is stored but not enforced; `Secure` cookies set over plain http are accepted but only sent over https.
- A closure passed as `on_chunk` cannot re-assign a captured `str` local (compiler limitation); accumulate into a class field instead.

---

## SMTP — `std.net.smtp`

**When**: You need to send outbound email — transactional notifications, alerts, password resets — from a Tauraro program, against a real SMTP server.
**Why**: A pure-Tauraro RFC 5321 client built on `TcpStream` and `std.net.tls` (verified TLS 1.3, both implicit TLS and STARTTLS). Handles multi-line reply parsing, RFC 5321 dot-stuffing, and `AUTH LOGIN`/`AUTH PLAIN` correctly (the parts most hand-rolled SMTP clients get subtly wrong).

```tauraro
from std.net.smtp import SmtpClient, SmtpResponse
```

### SmtpResponse

A parsed (possibly multi-line) SMTP reply.

| Field / Method | Type | Description |
|---|---|---|
| `code` | `int` | The 3-digit SMTP reply code (e.g. `250`, `550`). |
| `text` | `str` | Full reply text — all lines of a multi-line reply joined with `\n`, with the code and separator stripped from each. |
| `ok` | `bool` | `true` when `code` is 2xx or 3xx. |
| `is_error` | `() -> bool` | `true` when `code` is 4xx/5xx (or unparsed/`0`). |

### SmtpClient

| Method | Signature | Returns | Description |
|---|---|---|---|
| `SmtpClient.connect` | `(host: str, port: int) -> SmtpClient` | `SmtpClient` | Open a plaintext TCP connection and read the server's greeting. Check `.connected`. |
| `SmtpClient.connect_tls` | `(host: str, port: int) -> SmtpClient` | `SmtpClient` | Open an **implicit TLS** connection (e.g. port 465), verifying the server certificate, and read the greeting. On failure `.connected` is `false` and `last_text` holds the TLS error. |
| `SmtpClient.connect_tls_with` | `(host: str, port: int, opts: TlsOptions) -> SmtpClient` | `SmtpClient` | Same with explicit `TlsOptions` (private CA, timeout, verification). |
| `ehlo` | `(client_host: str) -> SmtpResponse` | `SmtpResponse` | Send `EHLO`, correctly parsing a multi-line capability reply (`250-...` continuations, final line `250 ...`). Falls back to `HELO` automatically if the server rejects `EHLO`. Records capabilities for `supports_starttls`/`supports_auth`. |
| `helo` | `(client_host: str) -> SmtpResponse` | `SmtpResponse` | Send plain `HELO` (no capability negotiation). |
| `supports_starttls` | `() -> bool` | `bool` | `true` if the last `ehlo()` capability list advertised `STARTTLS`. |
| `supports_auth` | `() -> bool` | `bool` | `true` if the last `ehlo()` capability list advertised `AUTH`. |
| `starttls` | `() -> SmtpResponse` | `SmtpResponse` | Send `STARTTLS`; on `220`, run the TLS handshake on the same socket (verified with `client.tls_opts`). Sets `use_tls`; call `ehlo()` again afterwards. A failed handshake returns code `0` with the TLS error and disconnects. |
| `auth_login` | `(username: str, password: str) -> SmtpResponse` | `SmtpResponse` | `AUTH LOGIN`: two round-trips, base64-encoded username then password. Returns the final (3rd) response. |
| `auth_plain` | `(username: str, password: str) -> SmtpResponse` | `SmtpResponse` | `AUTH PLAIN`: single command, base64 of `"\0username\0password"`. |
| `mail_from` | `(addr: str) -> SmtpResponse` | `SmtpResponse` | Send `MAIL FROM:<addr>`. |
| `rcpt_to` | `(addr: str) -> SmtpResponse` | `SmtpResponse` | Send `RCPT TO:<addr>`. Call once per recipient. |
| `data` | `(body: str) -> SmtpResponse` | `SmtpResponse` | Send `DATA`, then `body` (CRLF-translated and dot-stuffed per RFC 5321 — a body line starting with `.` is sent as `..`), then the terminating `.` line. Returns the server's final response. |
| `send_message` | `(msg: EmailMessage) -> SmtpResponse` | `SmtpResponse` | Send a `std.net.email` message: `MAIL FROM` the Sender/From address, `RCPT TO` every To + Cc + Bcc address (deduplicated), then `DATA` with `msg.to_string()` (Bcc is never written). Adds `BODY=8BITMIME` when the data has 8-bit bytes and `SMTPUTF8` when an envelope address is not ASCII, if the server advertises them. Stops at the first failing reply. |
| `send_message_to` | `(msg: EmailMessage, envelope_from: str, rcpts: List[str]) -> SmtpResponse` | `SmtpResponse` | Same with an explicit envelope. |
| `supports` | `(extension: str) -> bool` | `bool` | `true` if the last `ehlo()` advertised the extension (e.g. `"8BITMIME"`). |
| `send_mail` | `(from_addr: str, to: List[str], subject: str, body: str) -> SmtpResponse` | `SmtpResponse` | Convenience wrapper: `mail_from` + `rcpt_to` for each address in `to` + builds a `From`/`To`/`Subject`/`Date` message and calls `data`. Stops at the first error response. |
| `rset` | `() -> SmtpResponse` | `SmtpResponse` | Send `RSET` (abort the current mail transaction). |
| `noop` | `() -> SmtpResponse` | `SmtpResponse` | Send `NOOP`. |
| `quit` | `() -> SmtpResponse` | `SmtpResponse` | Send `QUIT` and close the connection. |
| `close` | `()` | `void` | Close the connection without sending `QUIT`. |
| `is_connected` | `() -> bool` | `bool` | `true` while the connection is open. |

Fields: `host: str`, `port: int`, `connected: bool`, `use_tls: bool`, `last_code: int` (most recent reply code), `last_text: str` (most recent reply text).

Every method that sends a command returns an `SmtpResponse` — check `.ok` / `.is_error()` (or compare `.code` directly) to detect 4xx/5xx failures; nothing is silently swallowed.

### Example — plaintext SMTP with STARTTLS negotiation attempt + AUTH LOGIN

```tauraro
from std.net.smtp import SmtpClient

mut client = SmtpClient.connect("smtp.example.com", 587)
if not client.connected:
    print("connect failed")
else:
    client.ehlo("myhost")
    if client.supports_auth():
        mut auth_r = client.auth_login("user@example.com", "password")
        if auth_r.is_error():
            print("auth failed: " + str(auth_r.code) + " " + auth_r.text)

    mut r = client.send_mail(
        "from@example.com",
        ["to1@example.com", "to2@example.com"],
        "Subject line",
        "Hello,\nThis is the body.\n.This line starts with a dot and is dot-stuffed automatically.\n"
    )
    if r.is_error():
        print("send failed: " + str(r.code) + " " + r.text)
    else:
        print("sent: " + str(r.code))

    client.quit()
```

### Example — implicit TLS (port 465)

```tauraro
from std.net.smtp import SmtpClient

mut client = SmtpClient.connect_tls("smtp.example.com", 465)
if client.connected:
    client.ehlo("myhost")
    client.auth_login("user@example.com", "password")
    client.send_mail("from@example.com", ["to@example.com"], "Hi", "Body text")
    client.quit()
```

### Limitations (v1)

- TLS is TLS 1.3 only; a server limited to TLS 1.2 fails the handshake.
- `send_mail` sends a single `text/plain` body. For HTML, attachments or non-ASCII headers build an `EmailMessage` (`std.net.email`, below) and call `send_message`.

---

## Mail — `std.net.pop3`, `std.net.imap`

Both run over `std.net.linestream.LineStream` (plain TCP or verified TLS, with an in-place
STARTTLS upgrade). Pass `tls = true` for implicit TLS (ports 995 / 993), or connect in plaintext
(110 / 143) and call `starttls()` before authenticating. `-ERR` / `NO` / `BAD` replies raise with
the server's text.

```tauraro
from std.net.imap import ImapClient
from std.net.pop3 import Pop3Client

mut c = ImapClient.connect("imap.example.com", 993, true)?
c.login("user", "secret")?
mut box = c.select("INBOX")?                # exists, recent, uidvalidity, uidnext, flags
for uid in c.uid_search("UNSEEN")?:
    mut raw = c.fetch_message(uid)?         # RFC 5322 bytes (BODY.PEEK[], keeps \Unseen)
    c.add_flags(uid, "\\Seen")?
c.logout()

mut p = Pop3Client.connect("pop.example.com", 995, true)?
p.login("user", "secret")?
mut st = p.stat()?                           # st.count, st.size
mut first = p.retr(1)?                       # dot-unstuffed, CRLF line ends
p.quit()
```

| `ImapClient` | Description |
|---|---|
| `connect(host, port, tls)?` / `connect_with(host, port, tls, opts)?` | Connect and read the greeting (`[CAPABILITY]` codes are picked up). |
| `starttls()?`, `login(user, pw)?`, `auth_plain(user, pw)?`, `logout()` | Session. `LOGIN` arguments are quoted and escaped. |
| `capability()?`, `has_capability(name)` | Capabilities (upper-cased). |
| `list_mailboxes(ref, pattern)?` | `ImapMailbox` (`name`, `delimiter`, `flags`); names sent as literals are handled. |
| `select(box)?` / `examine(box)?` | `ImapStatus` (`exists`, `recent`, `uidvalidity`, `uidnext`, `flags`, `read_only`). |
| `search(criteria)?` / `uid_search(criteria)?` | Matching sequence numbers / UIDs. |
| `fetch(set, items, by_uid)?`, `fetch_message(uid)?` | `ImapMessage` (`seq`, `uid`, `flags`, `size`, `body`); `{n}` literals are read exactly, so bodies are binary-safe. |
| `add_flags` / `remove_flags(uid, flags)?`, `copy` / `move(uid_set, box)?`, `expunge()?` | `move` uses `MOVE` when advertised, else COPY + `\Deleted` + EXPUNGE. |
| `create` / `delete_mailbox` / `rename`, `append(box, message, flags)?`, `noop()?` | Mailbox management and upload. |
| `command(text)?` / `command_raw(text)?` | Any other command; returns `ImapResponse` (`status`, `text`, `untagged` with their literals). |

| `Pop3Client` | Description |
|---|---|
| `connect(host, port, tls)?` / `connect_with(...)?` | Connect and read the greeting. |
| `starttls()?` (STLS), `login(user, pw)?`, `apop(user, pw)?`, `auth_plain(user, pw)?` | Authentication; APOP only when the greeting carries a timestamp. |
| `capabilities()?` | CAPA list (empty if unsupported). |
| `stat()?`, `list()?`, `uidl()?` | `Pop3Stat` (`count`, `size`); `Pop3Entry` (`num`, `size`, `uid`). |
| `retr(n)?`, `top(n, lines)?` | Message text, dot-unstuffed, CRLF-terminated lines. |
| `dele(n)?`, `rset()?`, `noop()?`, `quit()` | `quit` commits deletions and closes. |

`ImapClient.fetch_email(uid)?` and `Pop3Client.retr_email(n)?` return the message already parsed as
a `std.net.email.EmailMessage` (`subject()`, `text_body()`, `attachments()`, ...).

Not supported: IMAP IDLE/NOTIFY push, CONDSTORE/QRESYNC, BODYSTRUCTURE parsing; OAuth2
(`XOAUTH2`) is available through `command(...)` with your own token.

---

## Media types — `std.net.mime`

**When**: picking a `Content-Type` for a file (static file servers, uploads, attachments), checking
what some bytes really are, or parsing/formatting a `Content-Type` header.

```tauraro
from std.net.mime import Mime, MediaType

Mime.type_for_path("site/app.wasm")        # "application/wasm"
Mime.type_for_path("notes")                # "application/octet-stream" (unknown)
Mime.guess_type("x.unknown")               # "" (no default)
Mime.content_type_for("index.html")        # "text/html; charset=utf-8"
Mime.extension_for_type("image/jpeg")      # "jpg"
Mime.sniff(first_bytes)                    # "image/png", "text/html", "application/json", ...
Mime.matches("image/webp", "image/*")      # true

mut mt = Mime.parse("Text/HTML; Charset=\"UTF-8\"")?
mt.essence()                               # "text/html"
mt.charset()                               # "utf-8"
mt.set_param("name", "résumé.pdf")
mt.to_str()                                # "text/html; charset=UTF-8; name*=utf-8''r%C3%A9sum%C3%A9.pdf"
```

| `Mime` | Description |
|---|---|
| `type_for_path(path)` / `guess_type(path)` / `type_for_ext(ext)` | Type from the last extension (case-insensitive; URL query/fragment ignored; dot-files have no extension). `type_for_path` defaults to `application/octet-stream`, the others return `""`. |
| `content_type_for(path)` | `type_for_path` plus `; charset=utf-8` for textual types. |
| `extension_for_type(t)` / `extensions_for_type(t)` | Preferred extension (no dot) / all of them, preferred first. |
| `default_charset(t)`, `is_text(t)` | `"utf-8"` for text/*, JSON, XML, JavaScript, YAML, TOML, `+json`, `+xml` (not SVG). |
| `sniff(data)` | WHATWG MIME Sniffing "unknown type" algorithm: HTML tags / `<?xml` / PDF / PostScript, BOMs, GIF/PNG/JPEG/WebP/BMP/ICO/TIFF/PSD/QOI/JPEG XL, MP4-family brands (MP4, QuickTime, 3GP, AVIF, HEIC/HEIF, M4A), WebM/Matroska, Ogg (Opus/Vorbis/Theora), MP3 (ID3 and frame sync), AAC, FLAC, WAV, AIFF, MIDI, AVI, fonts (WOFF/WOFF2/OTF/TTF/TTC/EOT), gzip/ZIP (EPUB/ODF via their `mimetype` entry)/RAR/7z/xz/bzip2/zstd/lz4/tar, wasm, SQLite, ELF/PE/Java class, a JSON heuristic, then `text/plain` or `application/octet-stream` by the binary-byte test. Pass at least 512 leading bytes. |
| `sniff_with_hint(data, declared)` | Keep a specific declared type; sniff when it is missing or generic. |
| `parse(s)?` / `MediaType.parse(s)?` | Throws on a malformed type/subtype; parameters are lenient (bad ones skipped). |
| `matches(t, pattern)` | `*`, `*/*`, `image/*`, `*/json` (also matches `+json`), or exact; parameters ignored. |
| `is_token(s)`, `quote(s)`, `format_param(name, value)`, `pct_encode_2231(v)` | RFC 9110 token / quoted-string / RFC 2231 helpers. |
| `normalize_charset(cs)`, `is_supported_charset(cs)` | Canonical names (`latin1` → `iso-8859-1`, `cp1252` → `windows-1252`, ...). |
| `to_utf8(bytes, cs)?` / `decode_text(bytes, cs)` | Convert UTF-8, US-ASCII, ISO-8859-1, ISO-8859-15, Windows-1252, UTF-16/LE/BE to UTF-8 (invalid UTF-8 → U+FFFD). `to_utf8` throws `"unsupported charset: X"`; `decode_text` returns other charsets' bytes unchanged. |
| `sanitize_utf8(s)`, `is_valid_utf8(s)` | UTF-8 validation (overlongs and surrogates rejected). |

`MediaType` fields: `type_`, `subtype` (lower case), `names` (lower case), `values` (unquoted, RFC 2231
continuations `name*0*=` … joined and converted to UTF-8). Methods: `essence()`, `suffix()`,
`param(name)`, `has_param`, `set_param`, `remove_param`, `charset()`, `is_multipart()`, `is_text()`,
`matches(pattern)`, `to_str()`, and `MediaType.parse_params("; a=1; b=\"x\"")` for headers such as
`Content-Disposition`.

The table is Python 3.14's `mimetypes` table merged with the Debian media-types list and a
hand-picked modern set; `.tr` maps to `text/x-tauraro`. Sniffing never returns `text/html` for
data that starts with a BOM (WHATWG rule).

---

## Email messages — `std.net.email`

**When**: sending anything beyond a plain-text note (HTML, attachments, inline images, non-ASCII
names and subjects), or reading what IMAP / POP3 fetched. RFC 5322, MIME (RFC 2045-2049),
encoded words (RFC 2047), parameter continuations (RFC 2231) and UTF-8 headers (RFC 6532, read side).

```tauraro
from std.net.email import EmailMessage, Address
from std.net.smtp import SmtpClient

mut m = EmailMessage.init()
m.set_from("Zoë Example <zoe@example.com>")
m.add_to("bob@example.com, \"Doe, Jane\" <jane@example.org>")
m.add_bcc("archive@example.com")                 # envelope only
m.set_subject("Quarterly report — draft")         # encoded words when not ASCII
m.set_text("Hi Bob,\nthe report is attached.\n")
m.set_html("<p>Hi Bob</p><img src=\"cid:logo\">")
m.add_inline("logo", "logo.png", png_bytes)       # multipart/related
m.attach("report.pdf", pdf_bytes)                 # type from std.net.mime
mut c = SmtpClient.connect("smtp.example.com", 587)
c.ehlo("me")
c.send_message(m)

mut p = EmailMessage.parse(raw)                   # lenient, never throws
p.subject()                                       # decoded UTF-8
p.get_from()[0].name                              # "Zoë Example"
p.text_body()                                     # UTF-8, '\n' line ends
for a in p.attachments():
    print(a.filename + " " + a.content_type + " " + str(a.data.len()))
```

| Building (`EmailMessage`) | Description |
|---|---|
| `init()` | Empty message. Fields: `allow_8bit` (use 8bit instead of QP/base64 for text), `boundary_seed` (fixed boundaries, for tests). |
| `set_from`, `set_sender`, `set_to`, `add_to`, `add_cc`, `add_bcc`, `set_reply_to` | Take address-list text (`"a@x, Name <b@y>"`); display names may be UTF-8 and are written as atoms, quoted strings or encoded words. `add_address(field, name, addr)` skips parsing. |
| `set_subject(s)`, `set_header(name, text)`, `add_raw_header(name, wire)`, `remove_header(name)` | Unstructured text is RFC 2047-encoded when not plain ASCII (first word sized to fit after the header name). |
| `set_date(ts)`, `set_date_tz(ts, offset_min)` | RFC 5322 date; added automatically (now, UTC) when missing. |
| `set_message_id(id)`, `set_in_reply_to(id)`, `add_reference(id)`, `make_message_id()` | Angle brackets added; a random `<...@from-domain>` id is generated when missing. |
| `set_text(s)`, `set_html(s)` | Both → `multipart/alternative`. Text is labelled `charset=utf-8` and sent with CRLF line ends. |
| `attach(filename, data)`, `attach_as(filename, type, data)` | Attachments (`multipart/mixed`, base64; message/rfc822 as 7bit/8bit). Non-ASCII filenames use RFC 2231 `filename*=utf-8''…`. |
| `add_inline(cid, filename, data)`, `add_inline_as(...)` | Inline parts with `Content-ID`, related to the HTML body. |
| `build()`, `to_string()` | Build the MIME tree into `root` / serialise: CRLF, headers folded at 78 columns, Bcc omitted, MIME-Version added. Transfer encoding per part: 7bit for ASCII with lines ≤ 998, else the shorter of quoted-printable and base64. `root`, `message_id()` etc. reflect generated values only after `build()` / `to_string()`. |
| `recipients()`, `envelope_from()` | Envelope for SMTP: To + Cc + Bcc (deduplicated) / Sender or first From. |

| Reading | Description |
|---|---|
| `EmailMessage.parse(raw)` | CRLF or LF input; a leading mbox `From ` line is skipped; malformed parts degrade like Python's parser. Re-serialising with `to_string()` is lossless. |
| `subject()`, `get_from()`, `sender()`, `to()`, `cc()`, `bcc()`, `reply_to()` | Decoded; addresses are `Address` (`name`, `addr`, `group`; `local_part()`, `domain()`, `display()`, `to_str()`). A trailing comment (`a@x (Name)`) is used as the name, as `email.utils.getaddresses` does. |
| `date()`, `timestamp()`, `message_id()`, `in_reply_to()`, `references()` | `timestamp()` parses RFC 5322 dates incl. obsolete forms (2-digit years, named zones), `-1` when unparseable. |
| `header(name)`, `header_all(name)`, `decoded_header(name)`, `decoded_header_strict(name)?`, `header_names()` | Raw (unfolded) / decoded values. The strict form raises `"unsupported charset: X"`. |
| `text_body()`, `html_body()`, `has_text_body()`, `has_html_body()` | First non-attachment `text/plain` / `text/html` part converted to UTF-8 with `\n` line ends. |
| `attachments()` | `Attachment` (`filename`, `content_type`, `data` bytes, `content_id`, `is_inline`) for every leaf that is not the text/HTML body: attachment dispositions, named parts, related images and non-text parts. Unlike Python's `iter_attachments`, related (inline) images are included. |
| `parts()`, `root`, `content_type()`, `is_multipart()` | The `MimePart` tree: `names`/`values`, `ctype` (`MediaType`), `body` (transfer encoding removed), `raw_body`, `parts`, `preamble`, `epilogue`; methods `header`, `charset()`, `transfer_encoding()`, `disposition()`, `filename()` (RFC 2231 and 2047), `content_id()`, `text()`, `text_strict()?`, `walk()`, `as_message()` for message/rfc822 parts. |

Free functions: `decode_header(s)`, `decode_header_strict(s)?`, `encode_words(s)`,
`encode_header_text(s)`, `fold_header(name, value)`, `format_phrase(name)`,
`format_address_list(addrs)`, `format_date(ts, offset_min)`, `parse_date(s)`.

Charsets decoded: UTF-8, US-ASCII, ISO-8859-1, ISO-8859-15, Windows-1252, UTF-16. Others are
reported: encoded words in them stay as written in `decode_header`, `text()` returns the raw bytes,
and the strict variants raise. Not supported: S/MIME and PGP, DKIM signing, `message/partial`
reassembly.

---

## DNS messages, mDNS, NTP — `std.net.dnsmsg`, `std.net.mdns`, `std.net.ntp`, `std.net.udpx`

`std.net.dns` resolves names through the OS. `std.net.dnsmsg` speaks the DNS wire protocol
directly, so any record type can be asked of a chosen server:

```tauraro
from std.net.dnsmsg import Dns, DNS_MX, DNS_TXT, DNS_SRV
from std.net.ntp import Ntp
from std.net.mdns import Mdns

mut r = Dns.lookup("1.1.1.1", "gmail.com", DNS_MX)?
for rec in r.answers: print(rec.to_str())   # gmail.com 3600 MX 5 gmail-smtp-in.l.google.com

mut t = Ntp.query("time.google.com")?
print(f"clock offset {t.offset_ms} ms, delay {t.delay_ms} ms, stratum {t.stratum}")

for svc in Mdns.browse("_http._tcp", 1500)?:
    print(f"{svc.name} at {svc.host}:{svc.port} {svc.addresses}")
```

| API | Description |
|---|---|
| `DnsMessage.query(id, name, qtype, recursion)?` | Encode a query (with an EDNS0 OPT record, 4096-byte UDP payload). |
| `DnsMessage.multi_query(id, names, qtypes, unicast)?` | Several questions; `unicast` sets the mDNS QU bit. |
| `DnsMessage.parse(bytes)?` | Header flags, questions, `answers` / `authority` / `additional` as `DnsRecord`. Compression pointers are followed (loops and truncation raise). |
| `DnsRecord` | `name`, `rtype`, `rclass`, `cache_flush`, `ttl`, `rdata`, plus decoded `address` (A/AAAA), `target` (NS/CNAME/PTR/MX/SRV/SOA), `priority`, `weight`, `port`, `texts`, `soa_serial`; `to_str()`. |
| `Dns.lookup(server, name, qtype)?` / `lookup_port(server, port, name, qtype, timeout_ms)?` | One UDP query with a retry; checks the reply ID. |
| `DNS_A`, `DNS_NS`, `DNS_CNAME`, `DNS_SOA`, `DNS_PTR`, `DNS_MX`, `DNS_TXT`, `DNS_AAAA`, `DNS_SRV`, `DNS_ANY`, `dns_type_name(t)` | Record types. |
| `Ntp.query(server)?` / `query_port(server, port, timeout_ms)?` / `best_of(server, n)?` | SNTPv4 (RFC 4330): `NtpResult` with `offset_ms`, `delay_ms`, `server_time_ms`, `stratum`, `leap`, `version`, `ref_id`. Replies that do not echo the request, and kiss-o'-death packets, are rejected. |
| `Mdns.query(name, qtype, ms)?`, `query_many(...)?`, `resolve_host("x.local", ms)?` | One-shot multicast DNS (RFC 6762) on 224.0.0.251:5353; collects every reply within the window. |
| `Mdns.browse("_ipp._tcp", ms)?`, `service_types(ms)?`, `services_from_records(stype, recs)` | DNS-SD (RFC 6763) browsing into `MdnsService` (`name`, `instance`, `host`, `port`, `addresses`, `txt`). |
| `UdpEndpoint.open(family, port)?` / `open_shared(...)?` | UDP with IPv4/IPv6, receive timeouts (`recv(cap, ms)?` returns a `Datagram` with `data`, `addr`, `port`, `timed_out`), multicast join/TTL, `local_port()`. `unix_ms()` is the wall clock in ms. |

---

## std.net.http_server — HTTP Server

**When**: Building a web API, microservice, or web framework.
**Why**: Provides a minimal but complete HTTP server foundation — request parsing, path-parameter routing, response helpers — designed so FastAPI/Flask-style frameworks can be built on top.

```tauraro
from std.net.http_server import HttpServer, HttpRequest, HttpResponse, HttpConn, HttpRouter, HttpParser
```

### HttpRequest

| Field / Method | Type | Description |
|---|---|---|
| `method` | `str` | `"GET"`, `"POST"`, etc. |
| `path` | `str` | URL path, e.g. `"/users/42"`. |
| `query` | `str` | Raw query string without `?`. |
| `body` | `str` | Request body. |
| `headers` | `Map[str, str]` | Request headers (names stored lowercased). |
| `params` | `Map[str, str]` | Path parameters extracted by the router (`:id` → `"42"`). |
| `route_id` | `int` | Application route integer, `-1` when unmatched. |
| `version` | `str` | `"HTTP/1.1"` or `"HTTP/1.0"`, as sent by the client. |
| `recv_ms` | `int` | `Clock.now_ms()` timestamp when this request was parsed (for timing middleware). |
| `HttpRequest.init` | `() -> HttpRequest` | Construct an empty request (used internally by the parser). |
| `header(name)` | `str` | Get a request header (case-insensitive), or `""`. |
| `get_param(name)` | `str` | Get a path parameter, or `""`. |
| `query_param(key)` | `str` | First value of a `key=value` pair from the query string, percent-decoded. |
| `form_param(key)` | `str` | First value of a `key=value` pair from an `application/x-www-form-urlencoded` body, percent-decoded. |
| `is_form()` | `bool` | `true` when `Content-Type` contains `application/x-www-form-urlencoded`. |
| `content_type()` | `str` | `Content-Type` header shorthand. |
| `is_json()` | `bool` | `true` when `Content-Type` contains `application/json`. |
| `is_multipart()` | `bool` | `true` when `Content-Type` contains `multipart/form-data`. |
| `has_cookie()` | `bool` | `true` when the client sent a `Cookie` header. |
| `cookies()` | `str` | Raw `Cookie` header value. |
| `keep_alive()` | `bool` | Whether the connection should remain open for another request (HTTP/1.1 defaults to keep-alive unless `Connection: close`; HTTP/1.0 defaults to close unless `Connection: keep-alive`). |
| `free_owned()` | `void` | Release the request's headers/params maps and string fields, then the request itself. Called internally by `HttpConn.close()`/`dispose()`. |

### HttpResponse

A response builder: status code, body, and headers (in insertion order for deterministic wire output).

| Field / Method | Type / Signature | Description |
|---|---|---|
| `status` | `int` | HTTP status code. |
| `body` | `str` | Response body text. |
| `headers` | `Map[str, str]` | Response headers (name → value, last write wins). |
| `HttpResponse.init` | `(status: int, body: str) -> HttpResponse` | Create a response with empty headers. |
| `set_header` | `(name: str, value: str)` | Set/replace a header, tracking insertion order. |
| `is_ok()` | `() -> bool` | `true` for 2xx status codes. |
| `is_redirect()` | `() -> bool` | `true` for 3xx status codes. |
| `is_error()` | `() -> bool` | `true` for 4xx/5xx status codes. |
| `to_wire()` | `() -> str` | Serialize to a full HTTP/1.1 response (status line, headers, `Content-Length`, body). |
| `dispose()` | `()` | Free this `HttpResponse` instance (called by `HttpConn.send_response` after the headers/`_order`/body are dealt with — not part of auto-drop). |

### HttpConn

Combines a parsed `request: HttpRequest` with the live `TcpStream` so handlers can write a response.

| Method | Signature | Description |
|---|---|---|
| `HttpConn.init` | `(req: HttpRequest, stream: TcpStream) -> HttpConn` | Construct a connection (used internally by `HttpServer.accept()`). |
| `set_resp_header` | `(name: str, value: str)` | Queue a response header that is merged into every subsequent `send_*` call on this connection (e.g. cookies). |
| `send_response` | `(resp: HttpResponse)` | Write a pre-built response, merging connection-level headers and `Connection: keep-alive`/`close`, then disposes `resp`. |
| `send_text` | `(status: int, text: str)` | Plain-text response (`text/plain; charset=utf-8`). |
| `send_json` | `(status: int, json: str)` | JSON response (`application/json`). |
| `send_html` | `(status: int, html: str)` | HTML response (`text/html; charset=utf-8`). |
| `send_status` | `(status: int)` | Status-only response with empty body. |
| `redirect` | `(url: str, permanent: bool)` | Send a `Location` redirect — 302 (or 301 when `permanent`). |
| `set_cookie` | `(name: str, value: str, path: str, http_only: bool)` | Queue a `Set-Cookie` header for the next `send_*` call. |
| `close` | `()` | Close the underlying TCP connection and free the request/header state. |
| `reset_for` | `(req: HttpRequest)` | Replace `request` with a new parsed request and reset per-request response-header state, for serving another request on the same keep-alive connection. |
| `dispose` | `()` | Release the `HttpConn` instance itself (and any per-request state not already released by `close()`). |

Fields: `request: HttpRequest`, `last_status: int` (status of the most recent `send_response`, `0` if none yet).

### HttpRoute

A single registered route: `method: str`, `pattern: str`, `route_id: int`. Created via `HttpRoute.init(method, pattern, route_id) -> HttpRoute`; normally you don't construct these directly — use `HttpRouter`'s shorthand methods.

### HttpRouter

| Method | Signature | Description |
|---|---|---|
| `HttpRouter.init` | `() -> HttpRouter` | Create an empty router. |
| `add` | `(method, pattern, route_id)` | Register any method+pattern. |
| `get / post / put / patch / delete / head / options` | `(pattern, route_id)` | Method-specific shorthands. |
| `dispatch` | `(req: HttpRequest) -> bool` | Match and populate `req.route_id` + `req.params`. |

Patterns support `:name` segments: `/users/:id/posts/:pid`.

### HttpParser

| Method | Signature | Returns | Description |
|---|---|---|---|
| `HttpParser.parse` | `(raw: str) -> HttpRequest` | `HttpRequest` | Parse a raw HTTP request (request line, headers, body) into an `HttpRequest`. |
| `HttpParser.content_length` | `(hdr: str) -> int` | `int` | Extract the `Content-Length` value from a raw header section, or `0` when absent. |

### HttpMiddleware (interface)

| Method | Signature | Description |
|---|---|---|
| `before` | `(req: HttpRequest) -> bool` | Called before routing/handling; return `false` to short-circuit. |
| `after` | `(conn: HttpConn)` | Called after the response has been prepared. |

### HttpServer

| Method | Signature | Returns | Description |
|---|---|---|---|
| `HttpServer.init` | `(host: str, port: int) -> HttpServer` | `HttpServer` | Create server (not yet bound). |
| `start` | `() -> bool` | `bool` | Bind and listen. `true` on success. |
| `accept` | `() -> HttpConn` | `HttpConn` | Block until next request; parses and routes it. |
| `read_next` | `(stream: TcpStream) -> HttpRequest` | `HttpRequest` | Read and parse one request off an already-connected stream and run the router. Returns a request with empty `method` if the client closed the connection. |
| `stop` | `()` | `void` | Stop accepting connections. |
| `is_running` | `() -> bool` | `bool` | `true` while the server is active. |
| `set_router` | `(router: HttpRouter)` | `void` | Replace the internal router. |
| `router` | `() -> HttpRouter` | `HttpRouter` | Access the internal router for inline registration. |
| `set_recv_buf` | `(bytes: int)` | `void` | Set recv buffer size (default 64 KiB). Increase for large uploads. |
| `get / post / put / patch / delete / head / options` | `(pattern, route_id)` | `void` | Shorthand route registration on the server. |
| `any` | `(pattern, route_id)` | `void` | Register the same route id for any HTTP method (`"*"`). |

### Example — minimal REST API

```tauraro
from std.net.http_server import HttpServer, HttpRequest, HttpConn

# Route IDs
mut ROUTE_ROOT  = 0
mut ROUTE_USERS = 1
mut ROUTE_USER  = 2
mut ROUTE_ECHO  = 3

mut srv = HttpServer.init("0.0.0.0", 8080)
srv.get("/",           ROUTE_ROOT)
srv.get("/users",      ROUTE_USERS)
srv.get("/users/:id",  ROUTE_USER)
srv.post("/echo",      ROUTE_ECHO)

if not srv.start():
    print("failed to bind")
else:
    print("listening on :8080")
    while srv.is_running():
        mut conn = srv.accept()
        mut req  = conn.request

        if req.route_id == ROUTE_ROOT:
            conn.send_json(200, "{\"status\": \"ok\"}")

        elif req.route_id == ROUTE_USERS:
            conn.send_json(200, "[{\"id\": 1}, {\"id\": 2}]")

        elif req.route_id == ROUTE_USER:
            mut uid = req.get_param("id")
            conn.send_json(200, "{\"id\": \"" + uid + "\"}")

        elif req.route_id == ROUTE_ECHO:
            conn.send_text(200, req.body)

        else:
            conn.send_status(404)

        conn.close()
```

## WebSocket — `std.net.websocket`

**When**: You need a full-duplex, message-oriented channel over a single long-lived TCP connection — chat, live dashboards, game state sync, push notifications.
**Why**: RFC 6455 WebSocket (handshake + binary framing), client and server, built entirely in Tauraro on top of `TcpStream`/`TcpListener` — no new runtime C code. The handshake's `Sec-WebSocket-Accept` composes three existing stdlib primitives (`Hash.sha1` + `Hex.decode` + `Base64.encode`); masking/close/ping-pong are implemented directly against the RFC 6455 frame format.

```tauraro
from std.net.websocket import WebSocketClient, WebSocketServer, WsConn, WsMessage
```

### Coverage

Implemented: opening handshake (client and server side, with `Sec-WebSocket-Accept` verification), text frames (opcode `0x1`), binary frames (opcode `0x2`), correct masking in both directions (client→server frames are masked with a fresh random key per RFC 6455 §5.3; server→client frames are sent unmasked), Ping/Pong (`recv()` auto-answers an incoming Ping with a Pong), and a clean close handshake (`close()` sends a Close frame and waits for the peer's Close reply before closing the TCP connection).

**Not covered (v1 scope):**
- `wss://` (TLS) — only plain `ws://` is supported. `std.net.https`'s OpenSSL wrapping is a separate opt-in build (`-DTAURARO_TLS_OPENSSL`) and isn't wired into this module; layering it in is future work, not a quick win.
- Extensions (`permessage-deflate` and other `Sec-WebSocket-Extensions` negotiation).
- Subprotocol negotiation (`Sec-WebSocket-Protocol`).
- Fragmented messages: `recv()`/`recv_raw()` return each physical frame as received and do **not** reassemble a `FIN=0` continuation sequence into one logical message. This module's own `send_text`/`send_binary` always send a single unfragmented (`FIN=1`) frame, so round-tripping against another Tauraro `WsConn` is unaffected; a peer that deliberately fragments a message needs the caller to reassemble continuation frames itself.

### WsMessage

A tagged, fully-decoded received frame.

| Field / Method | Type / Signature | Description |
|---|---|---|
| `kind` | `int` | One of `WS_OP_TEXT()`, `WS_OP_BINARY()`, `WS_OP_CLOSE()`, `WS_OP_PING()`, `WS_OP_PONG()` (or `-1` on error — see `ok`). |
| `text` | `str` | Decoded text for a TEXT message (and the optional close-reason string for a CLOSE message); `""` for BINARY/PING/PONG. |
| `data` | `Pointer[char]` | Raw payload bytes (populated for every kind) — use this for binary payloads that may contain embedded `0x00` bytes. |
| `len` | `int` | Payload length in bytes. |
| `ok` | `bool` | `false` on a connection error or abrupt peer close — check this before using the message. |
| `is_text()` / `is_binary()` / `is_close()` / `is_ping()` / `is_pong()` | `() -> bool` | Opcode-kind checks. |
| `free()` | `()` | Release the message's raw payload buffer. Call once you're done with a message returned by `recv()`/`recv_raw()`. |

### WsConn

The live connection after a successful handshake — used identically by both client and server.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `send_text` | `(msg: str) -> bool` | `bool` | Send a complete (unfragmented) text frame. |
| `send_binary` | `(data: str, len_: int) -> bool` | `bool` | Send a complete binary frame of exactly `len_` bytes. |
| `send_ping` | `(msg: str) -> bool` | `bool` | Send a Ping frame, optionally carrying a small payload. |
| `send_pong` | `(msg: str) -> bool` | `bool` | Send an unsolicited Pong frame (incoming Pings are answered automatically by `recv()`). |
| `recv` | `() -> WsMessage` | `WsMessage` | Block for the next message. Auto-answers a Ping with a Pong and keeps waiting, so callers only ever see TEXT/BINARY/CLOSE from this method. |
| `recv_raw` | `() -> WsMessage` | `WsMessage` | Block for the next physical frame with no special handling — callers see PING/CLOSE themselves and must respond if desired. |
| `close` | `() -> bool` | `bool` | Perform the clean close handshake (send Close, wait for the peer's Close reply, then close the TCP connection). Safe to call more than once. Returns `true` only if the peer's Close reply was actually received. |
| `is_connected` | `() -> bool` | `bool` | `true` while the handshake succeeded and the underlying TCP connection is still open. |

Fields: `stream: TcpStream`, `is_client: bool`, `ok: bool` (handshake/connection succeeded), `closed: bool`, `err: str` (set on handshake/protocol failure).

### WebSocketClient

| Method | Signature | Returns | Description |
|---|---|---|---|
| `WebSocketClient.connect` | `(url: str) -> WsConn` | `WsConn` | Parse a `ws://host:port/path` URL, open a TCP connection, perform the client-side opening handshake (send a fresh random `Sec-WebSocket-Key`, verify the server's `Sec-WebSocket-Accept` matches). Check `.ok`; `.err` holds a message on failure (`"connect failed"`, `"server did not return HTTP 101 Switching Protocols"`, `"Sec-WebSocket-Accept mismatch"`, etc.). |

### WebSocketServer

| Method | Signature | Returns | Description |
|---|---|---|---|
| `WebSocketServer.accept_upgrade` | `(stream: TcpStream) -> WsConn` | `WsConn` | Given an already-`accept()`-ed `TcpStream` from a `TcpListener`, read the raw HTTP Upgrade request, verify it, and complete the server-side handshake (compute and send the correct `Sec-WebSocket-Accept`). Check `.ok`. |

### Example — echo server + client

```tauraro
from std.net.tcp        import TcpListener
from std.net.websocket  import WebSocketClient, WebSocketServer

# Server: accept one WebSocket connection and echo text messages back.
mut srv = TcpListener.listen("127.0.0.1", 9001)
if srv.listening:
    mut raw  = srv.accept()             # blocks until a client connects
    mut conn = WebSocketServer.accept_upgrade(raw)
    if conn.ok:
        mut msg = conn.recv()
        if msg.is_text():
            conn.send_text("echo: " + msg.text)
        msg.free()
        conn.close()                    # clean close handshake
    srv.close()
```

```tauraro
from std.net.websocket import WebSocketClient

# Client: connect, send a message, print the reply, close cleanly.
mut conn = WebSocketClient.connect("ws://127.0.0.1:9001/")
if conn.ok:
    conn.send_text("hello")
    mut msg = conn.recv()
    print(msg.text)                     # "echo: hello"
    msg.free()
    conn.close()
else:
    print("handshake failed: " + conn.err)
```
