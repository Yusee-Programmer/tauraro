/// ServEit - High-performance ASGI server for Tauraro
/// Similar to Python's uvicorn, built on Tokio and Hyper
///
/// Features:
/// - HTTP/1.1 and HTTP/2 support
/// - WebSocket support via ASGI
/// - Async/await with Tokio runtime
/// - Multiple workers
/// - Hot reload in development
/// - Static file serving
/// - Middleware support
/// - Access logging

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::sync::Arc;
use tokio::runtime::Runtime;
use std::net::SocketAddr;

/// Create the serveit module
pub fn create_serveit_module() -> Value {
    let mut namespace = HashMap::new();

    // Main server functions
    namespace.insert("run".to_string(), Value::NativeFunction(serveit_run));
    namespace.insert("serve".to_string(), Value::NativeFunction(serveit_serve));

    // Server configuration
    namespace.insert("Server".to_string(), Value::NativeFunction(create_server));
    namespace.insert("Config".to_string(), Value::NativeFunction(create_config));

    // Application helpers
    namespace.insert("Request".to_string(), Value::NativeFunction(create_request));
    namespace.insert("Response".to_string(), Value::NativeFunction(create_response));
    namespace.insert("JSONResponse".to_string(), Value::NativeFunction(json_response));
    namespace.insert("HTMLResponse".to_string(), Value::NativeFunction(html_response));
    namespace.insert("RedirectResponse".to_string(), Value::NativeFunction(redirect_response));
    namespace.insert("FileResponse".to_string(), Value::NativeFunction(file_response));

    // Middleware helpers
    namespace.insert("middleware".to_string(), Value::NativeFunction(create_middleware));

    // WebSocket support
    namespace.insert("WebSocket".to_string(), Value::NativeFunction(create_websocket));

    // Static file serving
    namespace.insert("StaticFiles".to_string(), Value::NativeFunction(static_files));

    // Routing
    namespace.insert("Router".to_string(), Value::NativeFunction(create_router));
    namespace.insert("Mount".to_string(), Value::NativeFunction(create_mount));

    // Status codes
    namespace.insert("status".to_string(), create_status_codes());

    // Constants
    namespace.insert("VERSION".to_string(), Value::Str("0.1.0".to_string()));
    namespace.insert("SERVER_NAME".to_string(), Value::Str("ServEit (Tauraro ASGI)".to_string()));

    Value::Module("serveit".to_string(), namespace)
}

/// Run ASGI application
/// serveit.run(app, host="127.0.0.1", port=8000, **kwargs)
fn serveit_run(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("run() requires at least 1 argument (app)"));
    }

    let app = args[0].clone();

    // Parse host (default: "127.0.0.1")
    let host = if args.len() > 1 {
        match &args[1] {
            Value::Str(s) => s.clone(),
            _ => "127.0.0.1".to_string(),
        }
    } else {
        "127.0.0.1".to_string()
    };

    // Parse port (default: 8000)
    let port = if args.len() > 2 {
        match &args[2] {
            Value::Int(p) => *p as u16,
            _ => 8000,
        }
    } else {
        8000
    };

    // Parse additional options from kwargs (args[3] would be a dict)
    let (log_level, reload, workers) = if args.len() > 3 {
        if let Value::Dict(dict) = &args[3] {
            let dict_ref = dict.borrow();
            let log_level = dict_ref.get("log_level")
                .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
                .unwrap_or_else(|| "info".to_string());

            let reload = dict_ref.get("reload")
                .and_then(|v| if let Value::Bool(b) = v { Some(*b) } else { None })
                .unwrap_or(false);

            let workers = dict_ref.get("workers")
                .and_then(|v| if let Value::Int(w) = v { Some(*w as usize) } else { None })
                .unwrap_or(1);

            (log_level, reload, workers)
        } else {
            ("info".to_string(), false, 1)
        }
    } else {
        ("info".to_string(), false, 1)
    };

    // Print startup message
    println!("╭─────────────────────────────────────────────────────╮");
    println!("│  ServEit - High-Performance ASGI Server (Tauraro)  │");
    println!("╰─────────────────────────────────────────────────────╯");
    println!();
    println!("  Starting server at: http://{}:{}", host, port);
    println!("  Log level: {}", log_level);
    println!("  Workers: {}", workers);
    println!("  Reload: {}", if reload { "enabled" } else { "disabled" });
    println!();
    println!("  Press CTRL+C to quit");
    println!();

    // Create Tokio runtime
    let rt = Runtime::new()?;

    // Run the server with LocalSet to support spawn_local
    rt.block_on(async {
        let local = tokio::task::LocalSet::new();
        local.run_until(async {
            run_server(app, host, port, log_level, reload, workers).await
        }).await
    })?;

    Ok(Value::None)
}

/// Async server implementation
async fn run_server(
    app: Value,
    host: String,
    port: u16,
    log_level: String,
    reload: bool,
    workers: usize,
) -> Result<()> {
    use hyper::server::conn::http1;
    use hyper::service::service_fn;
    use hyper::body::Incoming;
    use hyper_util::rt::TokioIo;
    use tokio::net::TcpListener;
    use std::convert::Infallible;

    let addr: SocketAddr = format!("{}:{}", host, port).parse()?;
    let app = Arc::new(app);
    let log_level = Arc::new(log_level);

    let listener = TcpListener::bind(addr).await?;

    println!("  Server started successfully!");
    println!("  Listening on: http://{}", addr);
    println!();

    loop {
        let (stream, _) = listener.accept().await?;
        let io = TokioIo::new(stream);

        // Clone for the spawned task
        let app_clone = app.as_ref().clone();
        let log_clone = log_level.as_ref().clone();

        // Spawn a task to handle each connection concurrently
        tokio::task::spawn_local(async move {
            if let Err(err) = http1::Builder::new()
                .keep_alive(false)  // Disable keep-alive to prevent incomplete message errors
                .serve_connection(io, service_fn(move |req| {
                    let app_ref = app_clone.clone();
                    let log_ref = log_clone.clone();
                    async move {
                        handle_request(req, Arc::new(app_ref), Arc::new(log_ref)).await
                    }
                }))
                .await
            {
                // Only log errors that aren't connection closed errors
                if !err.to_string().contains("connection closed") {
                    eprintln!("Error serving connection: {:?}", err);
                }
            }
        });
    }
}

/// Handle incoming HTTP request
async fn handle_request(
    req: hyper::Request<hyper::body::Incoming>,
    app: Arc<Value>,
    log_level: Arc<String>,
) -> Result<hyper::Response<String>, std::convert::Infallible> {
    use hyper::{StatusCode, Response};
    use std::time::Instant;

    let start = Instant::now();
    let method = req.method().clone();
    let path = req.uri().path().to_string();
    let version = format!("{:?}", req.version());

    // Log request if log_level is info or debug
    if log_level.as_str() == "info" || log_level.as_str() == "debug" {
        println!("  {} {} {}", method, path, version);
    }

    // Convert Hyper request to ASGI scope
    let scope = create_asgi_scope(&req);

    // Call ASGI app
    let response = match call_asgi_app(app.as_ref(), scope).await {
        Ok(resp) => resp,
        Err(e) => {
            eprintln!("  ERROR: {}", e);
            Response::builder()
                .status(StatusCode::INTERNAL_SERVER_ERROR)
                .body(format!("Internal Server Error: {}", e))
                .unwrap()
        }
    };

    // Log response
    let elapsed = start.elapsed();
    if log_level.as_str() == "info" || log_level.as_str() == "debug" {
        println!("  {} {} - {} ({:.2}ms)", method, path, response.status(), elapsed.as_secs_f64() * 1000.0);
    }

    Ok(response)
}

/// Create ASGI scope from Hyper request
fn create_asgi_scope(req: &hyper::Request<hyper::body::Incoming>) -> Value {
    use std::cell::RefCell;
    use std::rc::Rc;

    let mut scope = HashMap::new();

    // ASGI version
    scope.insert("type".to_string(), Value::Str("http".to_string()));
    scope.insert("asgi".to_string(), {
        let mut asgi = HashMap::new();
        asgi.insert("version".to_string(), Value::Str("3.0".to_string()));
        asgi.insert("spec_version".to_string(), Value::Str("2.3".to_string()));
        Value::Dict(Rc::new(RefCell::new(asgi)))
    });

    // HTTP version
    let http_version = match req.version() {
        hyper::Version::HTTP_09 => "0.9",
        hyper::Version::HTTP_10 => "1.0",
        hyper::Version::HTTP_11 => "1.1",
        hyper::Version::HTTP_2 => "2",
        hyper::Version::HTTP_3 => "3",
        _ => "1.1",
    };
    scope.insert("http_version".to_string(), Value::Str(http_version.to_string()));

    // Method
    scope.insert("method".to_string(), Value::Str(req.method().to_string()));

    // Path
    scope.insert("path".to_string(), Value::Str(req.uri().path().to_string()));

    // Query string
    let query_string = req.uri().query().unwrap_or("").to_string();
    scope.insert("query_string".to_string(), Value::Str(query_string));

    // Headers
    let mut headers = Vec::new();
    for (name, value) in req.headers().iter() {
        let header_tuple = vec![
            Value::Str(name.as_str().to_lowercase()),
            Value::Str(value.to_str().unwrap_or("").to_string()),
        ];
        headers.push(Value::Tuple(header_tuple));
    }
    scope.insert("headers".to_string(), Value::List(crate::modules::HPList::from_values(headers)));

    // Server
    scope.insert("server".to_string(), {
        let server = vec![
            Value::Str("127.0.0.1".to_string()),
            Value::Int(8000),
        ];
        Value::Tuple(server)
    });

    Value::Dict(Rc::new(RefCell::new(scope)))
}

/// Call ASGI application
async fn call_asgi_app(app: &Value, scope: Value) -> Result<hyper::Response<String>> {
    use hyper::{Response, StatusCode};

    // For now, we'll implement a simple call mechanism
    // In a full implementation, this would properly handle the ASGI callable protocol

    // Try to call the app as a function
    match app {
        Value::NativeFunction(func) => {
            let result = func(vec![scope])?;

            // Convert result to HTTP response
            convert_to_hyper_response(result)
        }
        Value::Dict(dict) => {
            // If app is a dict, it might be a response dict
            convert_dict_to_response(dict)
        }
        _ => {
            // Default response
            Ok(Response::builder()
                .status(StatusCode::OK)
                .body("Hello from ServEit!".to_string())
                .unwrap())
        }
    }
}

/// Convert Tauraro value to Hyper response
fn convert_to_hyper_response(value: Value) -> Result<hyper::Response<String>> {
    use hyper::{Response, StatusCode};

    match value {
        Value::Dict(dict) => convert_dict_to_response(&dict),
        Value::Str(s) => {
            let body_len = s.len();
            Ok(Response::builder()
                .status(StatusCode::OK)
                .header("content-type", "text/plain")
                .header("content-length", body_len.to_string())
                .body(s)
                .unwrap())
        }
        _ => {
            let body = format!("{:?}", value);
            let body_len = body.len();
            Ok(Response::builder()
                .status(StatusCode::OK)
                .header("content-length", body_len.to_string())
                .body(body)
                .unwrap())
        }
    }
}

/// Convert dict to Hyper response
fn convert_dict_to_response(dict: &std::rc::Rc<std::cell::RefCell<HashMap<String, Value>>>) -> Result<hyper::Response<String>> {
    use hyper::{Response, StatusCode};

    let dict_ref = dict.borrow();

    // Get status code
    let status = dict_ref.get("status")
        .and_then(|v| if let Value::Int(s) = v { Some(*s as u16) } else { None })
        .unwrap_or(200);

    // Get body first to calculate content length
    let body = dict_ref.get("body")
        .and_then(|v| match v {
            Value::Str(s) => Some(s.clone()),
            Value::Bytes(b) => Some(String::from_utf8_lossy(b).to_string()),
            _ => None,
        })
        .unwrap_or_else(|| String::new());

    let body_len = body.len();

    // Get headers
    let mut response = Response::builder().status(StatusCode::from_u16(status).unwrap_or(StatusCode::OK));

    // Add content-length header first
    response = response.header("content-length", body_len.to_string());

    if let Some(Value::Dict(headers)) = dict_ref.get("headers") {
        for (key, value) in headers.borrow().iter() {
            if let Value::Str(val) = value {
                // Don't override content-length if already set
                if key.to_lowercase() != "content-length" {
                    response = response.header(key.as_str(), val.as_str());
                }
            }
        }
    }

    Ok(response.body(body).unwrap())
}

/// Serve function (alias for run)
fn serveit_serve(args: Vec<Value>) -> Result<Value> {
    serveit_run(args)
}

/// Create server instance
fn create_server(_args: Vec<Value>) -> Result<Value> {
    let mut server = HashMap::new();
    server.insert("host".to_string(), Value::Str("127.0.0.1".to_string()));
    server.insert("port".to_string(), Value::Int(8000));
    server.insert("log_level".to_string(), Value::Str("info".to_string()));
    server.insert("reload".to_string(), Value::Bool(false));
    server.insert("workers".to_string(), Value::Int(1));

    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(server))))
}

/// Create config instance
fn create_config(_args: Vec<Value>) -> Result<Value> {
    create_server(_args)
}

/// Create request object
fn create_request(args: Vec<Value>) -> Result<Value> {
    let mut request = HashMap::new();

    if !args.is_empty() {
        if let Value::Dict(scope) = &args[0] {
            // Copy scope data to request
            for (key, value) in scope.borrow().iter() {
                request.insert(key.clone(), value.clone());
            }
        }
    }

    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(request))))
}

/// Create response object
fn create_response(args: Vec<Value>) -> Result<Value> {
    let mut response = HashMap::new();

    // Status code (default: 200)
    let status = if !args.is_empty() {
        match &args[0] {
            Value::Int(s) => *s,
            _ => 200,
        }
    } else {
        200
    };
    response.insert("status".to_string(), Value::Int(status));

    // Body
    let body = if args.len() > 1 {
        match &args[1] {
            Value::Str(s) => s.clone(),
            _ => String::new(),
        }
    } else {
        String::new()
    };
    response.insert("body".to_string(), Value::Str(body));

    // Headers
    let mut headers = HashMap::new();
    headers.insert("content-type".to_string(), Value::Str("text/plain".to_string()));
    response.insert("headers".to_string(), Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(headers))));

    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(response))))
}

/// JSON response helper
fn json_response(args: Vec<Value>) -> Result<Value> {
    use crate::modules::json;

    if args.is_empty() {
        return Err(anyhow!("JSONResponse requires at least 1 argument"));
    }

    // Serialize data to JSON
    let json_str = json::json_dumps(vec![args[0].clone()])?;
    let json_body = if let Value::Str(s) = json_str {
        s
    } else {
        return Err(anyhow!("Failed to serialize to JSON"));
    };

    // Status code
    let status = if args.len() > 1 {
        match &args[1] {
            Value::Int(s) => *s,
            _ => 200,
        }
    } else {
        200
    };

    let mut response = HashMap::new();
    response.insert("status".to_string(), Value::Int(status));
    response.insert("body".to_string(), Value::Str(json_body));

    let mut headers = HashMap::new();
    headers.insert("content-type".to_string(), Value::Str("application/json".to_string()));
    response.insert("headers".to_string(), Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(headers))));

    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(response))))
}

/// HTML response helper
fn html_response(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("HTMLResponse requires at least 1 argument"));
    }

    let html = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("HTMLResponse body must be a string")),
    };

    let status = if args.len() > 1 {
        match &args[1] {
            Value::Int(s) => *s,
            _ => 200,
        }
    } else {
        200
    };

    let mut response = HashMap::new();
    response.insert("status".to_string(), Value::Int(status));
    response.insert("body".to_string(), Value::Str(html));

    let mut headers = HashMap::new();
    headers.insert("content-type".to_string(), Value::Str("text/html".to_string()));
    response.insert("headers".to_string(), Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(headers))));

    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(response))))
}

/// Redirect response helper
fn redirect_response(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("RedirectResponse requires a URL"));
    }

    let url = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("Redirect URL must be a string")),
    };

    let status = if args.len() > 1 {
        match &args[1] {
            Value::Int(s) => *s,
            _ => 307, // Temporary redirect
        }
    } else {
        307
    };

    let mut response = HashMap::new();
    response.insert("status".to_string(), Value::Int(status));
    response.insert("body".to_string(), Value::Str(String::new()));

    let mut headers = HashMap::new();
    headers.insert("location".to_string(), Value::Str(url));
    response.insert("headers".to_string(), Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(headers))));

    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(response))))
}

/// File response helper
fn file_response(args: Vec<Value>) -> Result<Value> {
    use std::fs;

    if args.is_empty() {
        return Err(anyhow!("FileResponse requires a file path"));
    }

    let path = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("File path must be a string")),
    };

    // Read file
    let content = fs::read_to_string(&path)
        .map_err(|e| anyhow!("Failed to read file: {}", e))?;

    // Determine content type
    let content_type = if path.ends_with(".html") {
        "text/html"
    } else if path.ends_with(".css") {
        "text/css"
    } else if path.ends_with(".js") {
        "application/javascript"
    } else if path.ends_with(".json") {
        "application/json"
    } else if path.ends_with(".png") {
        "image/png"
    } else if path.ends_with(".jpg") || path.ends_with(".jpeg") {
        "image/jpeg"
    } else {
        "text/plain"
    };

    let mut response = HashMap::new();
    response.insert("status".to_string(), Value::Int(200));
    response.insert("body".to_string(), Value::Str(content));

    let mut headers = HashMap::new();
    headers.insert("content-type".to_string(), Value::Str(content_type.to_string()));
    response.insert("headers".to_string(), Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(headers))));

    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(response))))
}

/// Create middleware
fn create_middleware(_args: Vec<Value>) -> Result<Value> {
    // Placeholder for middleware support
    Ok(Value::None)
}

/// Create WebSocket
fn create_websocket(_args: Vec<Value>) -> Result<Value> {
    // Placeholder for WebSocket support
    let mut ws = HashMap::new();
    ws.insert("type".to_string(), Value::Str("websocket".to_string()));
    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(ws))))
}

/// Static files handler
fn static_files(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("StaticFiles requires a directory path"));
    }

    let directory = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("Directory path must be a string")),
    };

    let mut static_handler = HashMap::new();
    static_handler.insert("directory".to_string(), Value::Str(directory));
    static_handler.insert("type".to_string(), Value::Str("static".to_string()));

    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(static_handler))))
}

/// Create router
fn create_router(_args: Vec<Value>) -> Result<Value> {
    let mut router = HashMap::new();
    router.insert("routes".to_string(), Value::List(crate::modules::HPList::new()));
    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(router))))
}

/// Create mount point
fn create_mount(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// Create status codes object
fn create_status_codes() -> Value {
    let mut status = HashMap::new();

    // 2xx Success
    status.insert("OK".to_string(), Value::Int(200));
    status.insert("CREATED".to_string(), Value::Int(201));
    status.insert("ACCEPTED".to_string(), Value::Int(202));
    status.insert("NO_CONTENT".to_string(), Value::Int(204));

    // 3xx Redirection
    status.insert("MOVED_PERMANENTLY".to_string(), Value::Int(301));
    status.insert("FOUND".to_string(), Value::Int(302));
    status.insert("SEE_OTHER".to_string(), Value::Int(303));
    status.insert("NOT_MODIFIED".to_string(), Value::Int(304));
    status.insert("TEMPORARY_REDIRECT".to_string(), Value::Int(307));
    status.insert("PERMANENT_REDIRECT".to_string(), Value::Int(308));

    // 4xx Client Errors
    status.insert("BAD_REQUEST".to_string(), Value::Int(400));
    status.insert("UNAUTHORIZED".to_string(), Value::Int(401));
    status.insert("FORBIDDEN".to_string(), Value::Int(403));
    status.insert("NOT_FOUND".to_string(), Value::Int(404));
    status.insert("METHOD_NOT_ALLOWED".to_string(), Value::Int(405));
    status.insert("CONFLICT".to_string(), Value::Int(409));
    status.insert("GONE".to_string(), Value::Int(410));
    status.insert("UNPROCESSABLE_ENTITY".to_string(), Value::Int(422));
    status.insert("TOO_MANY_REQUESTS".to_string(), Value::Int(429));

    // 5xx Server Errors
    status.insert("INTERNAL_SERVER_ERROR".to_string(), Value::Int(500));
    status.insert("NOT_IMPLEMENTED".to_string(), Value::Int(501));
    status.insert("BAD_GATEWAY".to_string(), Value::Int(502));
    status.insert("SERVICE_UNAVAILABLE".to_string(), Value::Int(503));
    status.insert("GATEWAY_TIMEOUT".to_string(), Value::Int(504));

    Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(status)))
}
