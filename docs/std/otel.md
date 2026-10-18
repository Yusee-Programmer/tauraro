# std.otel — OpenTelemetry

```tauraro
from std.otel import Otel, Attributes, SPAN_KIND_INTERNAL, SPAN_KIND_SERVER
```

Traces, metrics and logs with W3C context propagation and OTLP/HTTP (JSON)
export. Pure Tauraro plus one small C shim (`std/otel/otel_native.c`: wall
clock, thread-local context stack, a thread-safe export queue).

Modules: `attr` (Attributes), `context` (SpanContext, TraceState, Baggage,
Context, ids), `resource`, `sampler`, `trace`, `metrics`, `logs`, `exporter`
(exporters, batch pipeline), `propagate`, `instrument`. `std.otel` re-exports
the common names.

## Quick start

```tauraro
mut r: Result[Otel, str] = Otel.init_from_env("checkout")   # honours OTEL_*
mut o = r.ok
mut tracer = o.tracer("checkout.http", "1.0")

with tracer.start_as_current("GET /cart", SPAN_KIND_SERVER) as sc:
    sc.span.set_str("http.request.method", "GET")
    mut db = tracer.start_span("db.query")          # parent = current span
    db.add_event("rows", Attributes.new().int("count", 3))
    db.end()
o.shutdown()                                        # flush + stop threads
```

## Tracing

* `TracerProvider.init(resource, sampler)`, then `add_simple_processor(exporter)`
  (exports on the calling thread) or `add_batch_processor(exporter, BatchConfig)`.
  `get_tracer(name, version)` is cached per scope.
* `tracer.start_span(name)` / `start_span_kind` / `start_root_span` /
  `start_span_with(name, kind, parent_ctx, attrs, start_ns)`.
  Parent = explicit context, else this thread's current context, else none.
* `Span`: `set_str/int/float/bool`, `set_attributes`, `add_event`, `add_link`,
  `record_exception`, `set_status` (OK is final), `update_name`, `end` / `end_at`.
  Calls after `end` are ignored. Limits (128 attributes/events/links by default,
  `OTEL_SPAN_*_LIMIT`) count drops into the `dropped*Count` fields.
* `with tracer.start_as_current(name, kind) as sc:` starts a child, makes it the
  thread's current span and ends it on exit; `with span.activate()` only
  activates. Spans are single-thread objects; hand a `SpanContext` (or a
  `Context`) to another thread and start a child there.
* Samplers: `Sampler.always_on/always_off/trace_id_ratio(r)/parent_based(root)`,
  `Sampler.from_env()` (`OTEL_TRACES_SAMPLER` = `always_on`, `always_off`,
  `traceidratio`, `parentbased_always_on` (default), `parentbased_always_off`,
  `parentbased_traceidratio`; `OTEL_TRACES_SAMPLER_ARG`). The ratio rule is
  deterministic on the trace id (low 63 bits vs ratio * 2^63).
  Not-sampled spans are non-recording and never exported.

## Context propagation

```tauraro
mut sc = TraceContext.parse_traceparent("00-4bf92f35...-00f067aa...-01")
sc.is_valid(); sc.is_sampled(); sc.traceparent()
TraceState.parse("a=1,b=2")   Baggage.parse("user=bob,node=DF%2028")
Propagator.inject_into(Context.current(), headers)   # client.HeaderList
mut ctx = Propagator.extract_values(tp, ts, baggage)  # remote context
```

Parsing follows the W3C rules: lower-case hex only, version `ff` and all-zero ids
rejected, version `00` must have exactly 55 characters, future versions are read
as `00`; an invalid `tracestate` is dropped, baggage is percent-decoded with
invalid members skipped. `Context.attach()` returns a token, `Context.detach(token)`
restores; the stack is thread-local (C `pthread_key`/FLS), freed at thread exit.

## Metrics

```tauraro
mut mp = MeterProvider.init(resource)
mp.add_periodic_reader(Exporter.otlp_from_env(SIGNAL_METRICS), 60000, TEMPORALITY_CUMULATIVE)
mut m = mp.get_meter("api", "1")
mut c = m.create_counter("requests", "{request}", "")
c.add(1, Attributes.new().str("route", "/a"))
mut h = m.create_histogram("latency", "ms", "", [5.0, 10.0, 50.0])
m.create_observable_gauge("queue", "1", "", read_queue)   # def(Observer)
mp.tick()          # call periodically; collects when a reader's interval elapsed
```

Counter (non-negative; negatives dropped), UpDownCounter, Histogram (explicit
buckets, min/max), Gauge, ObservableGauge/Counter/UpDownCounter (callback runs at
collection). Temporality per reader: cumulative, delta (Counter, ObservableCounter,
Histogram delta; up-down cumulative) or low-memory (`OTEL_EXPORTER_OTLP_METRICS_TEMPORALITY_PREFERENCE`).
There is no background timer for collection: `tick()` (cheap) is the periodic
driver; `force_flush()` / `shutdown()` collect immediately.

## Logs

`LoggerProvider.get_logger(name, ver).info/warn/...(body, attrs)` emits OTLP
log records correlated with the current span (traceId, spanId, flags).
`LogBridge.install(lp)` routes `std.log` records to the provider's first batch
processor; `LogBridge.attach(logger)` sets the logger's new `set_hook` callback
(level 1/5/9/13/17 severity numbers; target = scope name; fields = attributes).
Install once per process (the queue lives in a C registry slot).

## Export

* `Exporter.otlp_http(url)`, `Exporter.otlp_from_env(signal)`
  (`OTEL_EXPORTER_OTLP_[TRACES_|METRICS_|LOGS_]ENDPOINT`, `_HEADERS`, `_TIMEOUT`;
  the generic endpoint gets `/v1/<signal>` appended), `console()`, `memory()`
  (`take_payloads()`), `callback(fn)`, `noop()`.
* HTTP: POST `application/json` through `std.net.client.HttpSession`, 2xx = ok;
  429/502/503/504 and transport errors retry with exponential backoff
  (`with_retries(n, base_ms)`, default 2) inside the timeout.
* `BatchConfig.init(max_queue, max_batch, delay_ms, timeout_ms)` /
  `BatchConfig.from_env("OTEL_BSP")`: items go to a bounded queue; a worker thread
  exports when `max_batch` are waiting, every `delay_ms`, on `force_flush` and on
  `shutdown`. A full queue drops new items (`pipeline.dropped()`).
* `Resource.default_for(name)`: SDK defaults, then `OTEL_RESOURCE_ATTRIBUTES`
  (`k=v,k2=v2`, percent-decoded), then `OTEL_SERVICE_NAME` (wins).
* The JSON follows the OTLP proto3 JSON mapping: lowerCamelCase names, hex
  trace/span ids (not base64), int64/fixed64 as decimal strings, enums as numbers.
  Doubles use the shortest round-trip form; NaN/Infinity become strings.

## HTTP instrumentation (opt-in, `std.net.*` untouched)

```tauraro
mut tc = TracedHttpClient.init(HttpSession.init(), tracer)
tc.get("http://svc/x")            # CLIENT span + traceparent/baggage headers

mut si = ServerInstrumentation.init(tracer)
mut sc = si.begin_route(req, "/users/:id")   # SERVER span, parent from headers
...
si.finish(sc, 200)
```

## Validation

Payloads are checked in `tests/lang/148_otel.tr` against the proto3 JSON mapping
(field names, hex ids, string int64s, enum numbers) using a fake OTLP collector
(`std.net.http_server` in a thread) and the W3C Trace Context spec vectors.
Neither `otelcol` nor Python `opentelemetry-proto` was available when this was
written, so no external-validator run was made.

## Limitations

* JSON only; OTLP/protobuf is not implemented.
* One HTTP connection per export (no keep-alive), no gzip, `https://` works via
  `std.net.tls`; no gRPC.
* Spans, tracers and instruments are not thread-safe objects; the export
  pipeline is. Observable callbacks run on the thread that calls `tick()`/`collect`.
* No exemplars, views, exponential histograms or span processors other than
  simple and batch; trace `Span.flags` carry the is-remote bits per the OTLP spec.
* The std.log bridge is process-wide (one installed provider at a time).
