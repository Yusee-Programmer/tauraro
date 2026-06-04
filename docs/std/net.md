# std.net — Networking

```tauraro
from std.net.tcp  import TcpStream, TcpListener
from std.net.udp  import UdpSocket
from std.net.dns  import Dns
from std.net.url  import Url
from std.net.http import HttpClient, HttpResponse, HttpHeader
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
**Why**: A stateful client with configurable headers and timeout; all verbs return a structured `HttpResponse` with parsed status code and response headers.

### HttpResponse

| Field / Method | Type / Signature | Description |
|---|---|---|
| `status` | `int` | HTTP status code (e.g. `200`, `404`). |
| `body` | `str` | Response body text. |
| `headers` | `Map[str]` | Parsed response headers (name → value). |
| `is_ok` | `() -> bool` | `true` for 2xx status codes. |
| `is_redirect` | `() -> bool` | `true` for 3xx status codes. |
| `is_error` | `() -> bool` | `true` for 4xx/5xx status codes. |
| `header` | `(name: str) -> str` | Return a response header value, or `""` if absent. |
| `to_string` | `() -> str` | Human-readable `"HTTP NNN Reason\n<body>"`. |

### HttpClient

| Method | Signature | Returns | Description |
|---|---|---|---|
| `HttpClient.init` | `(host: str, port: int) -> HttpClient` | `HttpClient` | Create a client for `host:port`. Default timeout: 5000 ms. |
| `set_header` | `(name: str, value: str)` | `void` | Add a header to all subsequent requests. |
| `set_timeout` | `(ms: int)` | `void` | Set timeout hint in milliseconds. |
| `get` | `(path: str) -> HttpResponse` | `HttpResponse` | Send a `GET` request. |
| `post` | `(path: str, data: str) -> HttpResponse` | `HttpResponse` | Send a `POST` with `application/x-www-form-urlencoded` body. |
| `post_json` | `(path: str, data: str) -> HttpResponse` | `HttpResponse` | Send a `POST` with `application/json` body. |
| `put` | `(path: str, data: str) -> HttpResponse` | `HttpResponse` | Send a `PUT` with `application/octet-stream` body. |
| `patch` | `(path: str, data: str) -> HttpResponse` | `HttpResponse` | Send a `PATCH` with `application/octet-stream` body. |
| `delete` | `(path: str) -> HttpResponse` | `HttpResponse` | Send a `DELETE` request. |
| `head` | `(path: str) -> HttpResponse` | `HttpResponse` | Send a `HEAD` request (response body will be empty per HTTP spec). |

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

## std.net.https — HTTPS/TLS Client

**When**: You need to call HTTPS endpoints — secure REST APIs, webhooks, OAuth flows.
**Why**: Identical API to `HttpClient` but tunnelled through OpenSSL.

> **Opt-in** — compile with `-DTAURARO_TLS_OPENSSL -lssl -lcrypto`.
> Without those flags all methods return `HttpResponse { status: 0, body: "tls connect failed" }`.

```tauraro
from std.net.https import HttpsClient
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `HttpsClient.init` | `(host: str, port: int) -> HttpsClient` | `HttpsClient` | Create a TLS client. Default timeout: 10 000 ms. |
| `set_header` | `(name: str, value: str)` | `void` | Add a header to all requests. |
| `set_timeout` | `(ms: int)` | `void` | Set timeout hint. |
| `get` | `(path: str) -> HttpResponse` | `HttpResponse` | HTTPS GET. |
| `post` | `(path: str, data: str) -> HttpResponse` | `HttpResponse` | HTTPS POST (form-encoded). |
| `post_json` | `(path: str, data: str) -> HttpResponse` | `HttpResponse` | HTTPS POST (JSON). |
| `put` | `(path: str, data: str) -> HttpResponse` | `HttpResponse` | HTTPS PUT. |
| `patch` | `(path: str, data: str) -> HttpResponse` | `HttpResponse` | HTTPS PATCH. |
| `delete` | `(path: str) -> HttpResponse` | `HttpResponse` | HTTPS DELETE. |
| `head` | `(path: str) -> HttpResponse` | `HttpResponse` | HTTPS HEAD. |

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

## std.net.http_server — HTTP Server

**When**: Building a web API, microservice, or web framework.
**Why**: Provides a minimal but complete HTTP server foundation — request parsing, path-parameter routing, response helpers — designed so FastAPI/Flask-style frameworks can be built on top.

```tauraro
from std.net.http_server import HttpServer, HttpRequest, HttpConn, HttpRouter, HttpParser
```

### HttpRequest

| Field / Method | Type | Description |
|---|---|---|
| `method` | `str` | `"GET"`, `"POST"`, etc. |
| `path` | `str` | URL path, e.g. `"/users/42"`. |
| `query` | `str` | Raw query string without `?`. |
| `body` | `str` | Request body. |
| `headers` | `Map[str]` | Request headers. |
| `params` | `Map[str]` | Path parameters extracted by the router (`:id` → `"42"`). |
| `route_id` | `int` | Application route integer, `-1` when unmatched. |
| `header(name)` | `str` | Get a request header, or `""`. |
| `get_param(name)` | `str` | Get a path parameter, or `""`. |
| `query_param(key)` | `str` | Parse a `key=value` pair from the query string. |
| `content_type()` | `str` | `Content-Type` header shorthand. |
| `is_json()` | `bool` | `true` when `Content-Type` contains `application/json`. |
| `cookies()` | `str` | Raw `Cookie` header value. |

### HttpConn

| Method | Signature | Description |
|---|---|---|
| `send_response` | `(resp: HttpResponse)` | Write a pre-built response. |
| `send_text` | `(status: int, text: str)` | Plain-text response. |
| `send_json` | `(status: int, json: str)` | JSON response (`application/json`). |
| `send_html` | `(status: int, html: str)` | HTML response (`text/html; charset=utf-8`). |
| `send_status` | `(status: int)` | Status-only with empty body. |
| `redirect` | `(url: str, permanent: bool)` | 301/302 redirect. |
| `close` | `()` | Close the TCP connection. |

Fields: `request: HttpRequest`.

### HttpRouter

| Method | Signature | Description |
|---|---|---|
| `HttpRouter.init` | `() -> HttpRouter` | Create an empty router. |
| `add` | `(method, pattern, route_id)` | Register any method+pattern. |
| `get / post / put / patch / delete / head / options` | `(pattern, route_id)` | Method-specific shorthands. |
| `dispatch` | `(req: HttpRequest) -> bool` | Match and populate `req.route_id` + `req.params`. |

Patterns support `:name` segments: `/users/:id/posts/:pid`.

### HttpServer

| Method | Signature | Returns | Description |
|---|---|---|---|
| `HttpServer.init` | `(host: str, port: int) -> HttpServer` | `HttpServer` | Create server (not yet bound). |
| `start` | `() -> bool` | `bool` | Bind and listen. `true` on success. |
| `accept` | `() -> HttpConn` | `HttpConn` | Block until next request; parses and routes it. |
| `stop` | `()` | `void` | Stop accepting connections. |
| `is_running` | `() -> bool` | `bool` | `true` while the server is active. |
| `set_router` | `(router: HttpRouter)` | `void` | Replace the internal router. |
| `router` | `() -> HttpRouter` | `HttpRouter` | Access the internal router for inline registration. |
| `set_recv_buf` | `(bytes: int)` | `void` | Set recv buffer size (default 64 KiB). |
| `get / post / put / patch / delete / head / options / any` | `(pattern, route_id)` | `void` | Shorthand route registration on the server. |

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
