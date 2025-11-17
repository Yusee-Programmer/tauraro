use crate::value::Value;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use crate::modules::hplist::HPList;

#[cfg(feature = "http")]
use httparse::{Request, Response, Status};

// Wrapper functions for extern "C" functions
fn parse_url_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(parse_url(args.as_ptr() as *const Value, args.len()))
}

fn parse_headers_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(parse_headers(args.as_ptr() as *const Value, args.len()))
}

fn parse_request_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(parse_request(args.as_ptr() as *const Value, args.len()))
}

fn parse_response_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(parse_response(args.as_ptr() as *const Value, args.len()))
}

fn build_request_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 3 {
        return Ok(Value::None);
    }
    Ok(build_request(args.as_ptr() as *const Value, args.len()))
}

fn build_response_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 {
        return Ok(Value::None);
    }
    Ok(build_response(args.as_ptr() as *const Value, args.len()))
}

fn build_headers_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(build_headers(args.as_ptr() as *const Value, args.len()))
}

fn url_quote_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(url_quote(args.as_ptr() as *const Value, args.len()))
}

fn url_unquote_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(url_unquote(args.as_ptr() as *const Value, args.len()))
}

fn url_quote_plus_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(url_quote_plus(args.as_ptr() as *const Value, args.len()))
}

fn url_unquote_plus_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(url_unquote_plus(args.as_ptr() as *const Value, args.len()))
}

fn get_status_text_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(get_status_text(args.as_ptr() as *const Value, args.len()))
}

fn is_informational_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(is_informational(args.as_ptr() as *const Value, args.len()))
}

fn is_success_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(is_success(args.as_ptr() as *const Value, args.len()))
}

fn is_redirect_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(is_redirect(args.as_ptr() as *const Value, args.len()))
}

fn is_client_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(is_client_error(args.as_ptr() as *const Value, args.len()))
}

fn is_server_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(is_server_error(args.as_ptr() as *const Value, args.len()))
}

fn is_safe_method_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(is_safe_method(args.as_ptr() as *const Value, args.len()))
}

fn is_idempotent_method_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::None);
    }
    Ok(is_idempotent_method(args.as_ptr() as *const Value, args.len()))
}

fn create_request_parser_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    Ok(create_request_parser(args.as_ptr() as *const Value, args.len()))
}

fn create_response_parser_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    Ok(create_response_parser(args.as_ptr() as *const Value, args.len()))
}

fn http_parser_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    Ok(http_parser_error(args.as_ptr() as *const Value, args.len()))
}

fn http_parser_upgrade_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    Ok(http_parser_upgrade(args.as_ptr() as *const Value, args.len()))
}

/// Create the httptools module
pub fn create_httptools_module() -> Value {
    let mut namespace = HashMap::new();

    // HTTP parsing functions
    namespace.insert("parse_url".to_string(), Value::NativeFunction(parse_url_wrapper));
    namespace.insert("parse_headers".to_string(), Value::NativeFunction(parse_headers_wrapper));
    namespace.insert("parse_request".to_string(), Value::NativeFunction(parse_request_wrapper));
    namespace.insert("parse_response".to_string(), Value::NativeFunction(parse_response_wrapper));

    // HTTP building functions
    namespace.insert("build_request".to_string(), Value::NativeFunction(build_request_wrapper));
    namespace.insert("build_response".to_string(), Value::NativeFunction(build_response_wrapper));
    namespace.insert("build_headers".to_string(), Value::NativeFunction(build_headers_wrapper));

    // URL utilities
    namespace.insert("quote".to_string(), Value::NativeFunction(url_quote_wrapper));
    namespace.insert("unquote".to_string(), Value::NativeFunction(url_unquote_wrapper));
    namespace.insert("quote_plus".to_string(), Value::NativeFunction(url_quote_plus_wrapper));
    namespace.insert("unquote_plus".to_string(), Value::NativeFunction(url_unquote_plus_wrapper));

    // HTTP status utilities
    namespace.insert("get_status_text".to_string(), Value::NativeFunction(get_status_text_wrapper));
    namespace.insert("is_informational".to_string(), Value::NativeFunction(is_informational_wrapper));
    namespace.insert("is_success".to_string(), Value::NativeFunction(is_success_wrapper));
    namespace.insert("is_redirect".to_string(), Value::NativeFunction(is_redirect_wrapper));
    namespace.insert("is_client_error".to_string(), Value::NativeFunction(is_client_error_wrapper));
    namespace.insert("is_server_error".to_string(), Value::NativeFunction(is_server_error_wrapper));

    // HTTP method utilities
    namespace.insert("is_safe_method".to_string(), Value::NativeFunction(is_safe_method_wrapper));
    namespace.insert("is_idempotent_method".to_string(), Value::NativeFunction(is_idempotent_method_wrapper));

    // Parser classes
    namespace.insert("HttpRequestParser".to_string(), Value::BuiltinFunction("HttpRequestParser".to_string(), create_request_parser_wrapper));
    namespace.insert("HttpResponseParser".to_string(), Value::BuiltinFunction("HttpResponseParser".to_string(), create_response_parser_wrapper));

    // Exception classes
    namespace.insert("HttpParserError".to_string(), Value::BuiltinFunction("HttpParserError".to_string(), http_parser_error_wrapper));
    namespace.insert("HttpParserUpgrade".to_string(), Value::BuiltinFunction("HttpParserUpgrade".to_string(), http_parser_upgrade_wrapper));

    // Constants
    namespace.insert("HTTP_METHODS".to_string(), Value::List(HPList::from_values(vec![
        Value::Str("GET".to_string()),
        Value::Str("POST".to_string()),
        Value::Str("PUT".to_string()),
        Value::Str("DELETE".to_string()),
        Value::Str("HEAD".to_string()),
        Value::Str("OPTIONS".to_string()),
        Value::Str("PATCH".to_string()),
        Value::Str("TRACE".to_string()),
        Value::Str("CONNECT".to_string()),
    ])));

    Value::Module("httptools".to_string(), namespace)
}

// HTTP parsing functions
extern "C" fn parse_url(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::None;
    }

    let url_str = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    // Mock URL parsing - would use a proper URL parser in real implementation
    let mut url_parts = HashMap::new();
    
    if let Some(scheme_end) = url_str.find("://") {
        let scheme = &url_str[..scheme_end];
        url_parts.insert("scheme".to_string(), Value::Str(scheme.to_string()));
        
        let rest = &url_str[scheme_end + 3..];
        if let Some(path_start) = rest.find('/') {
            let host = &rest[..path_start];
            let path = &rest[path_start..];
            
            url_parts.insert("host".to_string(), Value::Str(host.to_string()));
            url_parts.insert("path".to_string(), Value::Str(path.to_string()));
        } else {
            url_parts.insert("host".to_string(), Value::Str(rest.to_string()));
            url_parts.insert("path".to_string(), Value::Str("/".to_string()));
        }
    }

    Value::Object {
        class_name: "URLParts".to_string(),
        fields: Rc::new(RefCell::new(url_parts)),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("URLParts".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["URLParts".to_string(), "object".to_string()])
    }
}

extern "C" fn parse_headers(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::None;
    }

    let headers_str = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    let mut headers = HashMap::new();
    
    for line in headers_str.lines() {
        if let Some(colon_pos) = line.find(':') {
            let name = line[..colon_pos].trim().to_lowercase();
            let value = line[colon_pos + 1..].trim();
            headers.insert(name, Value::Str(value.to_string()));
        }
    }

    Value::Object {
        class_name: "Headers".to_string(),
        fields: Rc::new(RefCell::new(headers)),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Headers".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Headers".to_string(), "object".to_string()])
    }
}

extern "C" fn parse_request(args: *const Value, argc: usize) -> Value {
    #[cfg(feature = "http")]
    {
        if argc == 0 {
            return Value::None;
        }

        let request_str = unsafe {
            match &*args {
                Value::Str(s) => s,
                _ => return Value::None,
            }
        };

        let mut headers = [httparse::EMPTY_HEADER; 64];
        let mut req = Request::new(&mut headers);
        
        match req.parse(request_str.as_bytes()) {
            Ok(Status::Complete(_)) => {
                let mut result = HashMap::new();
                result.insert("method".to_string(), Value::Str(req.method.unwrap_or("").to_string()));
                result.insert("path".to_string(), Value::Str(req.path.unwrap_or("").to_string()));
                result.insert("version".to_string(), Value::Int(req.version.unwrap_or(0) as i64));
                
                let mut headers_map = HashMap::new();
                for header in req.headers {
                    if !header.name.is_empty() {
                        headers_map.insert(
                            header.name.to_lowercase(),
                            Value::Str(String::from_utf8_lossy(header.value).to_string())
                        );
                    }
                }
                result.insert("headers".to_string(), Value::Object {
                    class_name: "Headers".to_string(),
                    fields: Rc::new(RefCell::new(headers_map)),  // Wrap with Rc::new
                    class_methods: HashMap::new(),
                    base_object: crate::base_object::BaseObject::new("Headers".to_string(), vec!["object".to_string()]),
                    mro: crate::base_object::MRO::from_linearization(vec!["Headers".to_string(), "object".to_string()])
                });
                
                Value::Object {
                    class_name: "ParsedRequest".to_string(),
                    fields: Rc::new(RefCell::new(result)),
                    class_methods: HashMap::new(),
                    base_object: crate::base_object::BaseObject::new("ParsedRequest".to_string(), vec!["object".to_string()]),
                    mro: crate::base_object::MRO::from_linearization(vec!["ParsedRequest".to_string(), "object".to_string()])
                }
            }
            _ => Value::None,
        }
    }
    #[cfg(not(feature = "http"))]
    {
        Value::Str("HTTP support not enabled".to_string())
    }
}

extern "C" fn parse_response(args: *const Value, argc: usize) -> Value {
    #[cfg(feature = "http")]
    {
        if argc == 0 {
            return Value::None;
        }

        let response_str = unsafe {
            match &*args {
                Value::Str(s) => s,
                _ => return Value::None,
            }
        };

        let mut headers = [httparse::EMPTY_HEADER; 64];
        let mut resp = Response::new(&mut headers);
        
        match resp.parse(response_str.as_bytes()) {
            Ok(Status::Complete(_)) => {
                let mut result = HashMap::new();
                result.insert("version".to_string(), Value::Int(resp.version.unwrap_or(0) as i64));
                result.insert("status".to_string(), Value::Int(resp.code.unwrap_or(0) as i64));
                result.insert("reason".to_string(), Value::Str(resp.reason.unwrap_or("").to_string()));
                
                let mut headers_map = HashMap::new();
                for header in resp.headers {
                    if !header.name.is_empty() {
                        headers_map.insert(
                            header.name.to_lowercase(),
                            Value::Str(String::from_utf8_lossy(header.value).to_string())
                        );
                    }
                }
                result.insert("headers".to_string(), Value::Object {
                    class_name: "Headers".to_string(),
                    fields: Rc::new(RefCell::new(headers_map)),  // Wrap with Rc::new
                    class_methods: HashMap::new(),
                    base_object: crate::base_object::BaseObject::new("Headers".to_string(), vec!["object".to_string()]),
                    mro: crate::base_object::MRO::from_linearization(vec!["Headers".to_string(), "object".to_string()])
                });
                
                Value::Object {
                    class_name: "ParsedResponse".to_string(),
                    fields: Rc::new(RefCell::new(result)),
                    class_methods: HashMap::new(),
                    base_object: crate::base_object::BaseObject::new("ParsedResponse".to_string(), vec!["object".to_string()]),
                    mro: crate::base_object::MRO::from_linearization(vec!["ParsedResponse".to_string(), "object".to_string()])
                }
            }
            _ => Value::None,
        }
    }
    #[cfg(not(feature = "http"))]
    {
        Value::Str("HTTP support not enabled".to_string())
    }
}

// HTTP building functions
extern "C" fn build_request(args: *const Value, argc: usize) -> Value {
    if argc < 3 {
        return Value::None;
    }

    let method = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    let path = unsafe {
        match &*args.add(1) {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    let headers = unsafe {
        match &*args.add(2) {
            Value::Object { fields, .. } => fields,
            _ => return Value::None,
        }
    };

    let mut request = format!("{} {} HTTP/1.1\r\n", method, path);
    
    for (name, value) in headers.borrow().iter() {
        if let Value::Str(val) = value {
            request.push_str(&format!("{}: {}\r\n", name, val));
        }
    }
    
    request.push_str("\r\n");
    
    Value::Str(request)
}

extern "C" fn build_response(args: *const Value, argc: usize) -> Value {
    if argc < 2 {
        return Value::None;
    }

    let status = unsafe {
        match &*args {
            Value::Int(s) => *s,
            _ => return Value::None,
        }
    };

    let headers = unsafe {
        match &*args.add(1) {
            Value::Object { fields, .. } => fields,
            _ => return Value::None,
        }
    };

    let reason = get_status_reason(status);
    let mut response = format!("HTTP/1.1 {} {}\r\n", status, reason);
    
    for (name, value) in headers.borrow().iter() {
        if let Value::Str(val) = value {
            response.push_str(&format!("{}: {}\r\n", name, val));
        }
    }
    
    response.push_str("\r\n");
    
    Value::Str(response)
}

extern "C" fn build_headers(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::None;
    }

    let headers = unsafe {
        match &*args {
            Value::Object { fields, .. } => fields,
            _ => return Value::None,
        }
    };

    let mut header_string = String::new();
    
    for (name, value) in headers.borrow().iter() {
        if let Value::Str(val) = value {
            header_string.push_str(&format!("{}: {}\r\n", name, val));
        }
    }
    
    Value::Str(header_string)
}

// URL utilities
extern "C" fn url_quote(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::None;
    }

    let input = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    // Simple URL encoding
    let encoded = input.chars()
        .map(|c| match c {
            'A'..='Z' | 'a'..='z' | '0'..='9' | '-' | '_' | '.' | '~' => c.to_string(),
            _ => format!("%{:02X}", c as u8),
        })
        .collect::<String>();

    Value::Str(encoded)
}

extern "C" fn url_unquote(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::None;
    }

    let input = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    // Simple URL decoding
    let mut decoded = String::new();
    let mut chars = input.chars();
    
    while let Some(c) = chars.next() {
        if c == '%' {
            if let (Some(h1), Some(h2)) = (chars.next(), chars.next()) {
                if let Ok(byte) = u8::from_str_radix(&format!("{}{}", h1, h2), 16) {
                    decoded.push(byte as char);
                } else {
                    decoded.push(c);
                    decoded.push(h1);
                    decoded.push(h2);
                }
            } else {
                decoded.push(c);
            }
        } else {
            decoded.push(c);
        }
    }

    Value::Str(decoded)
}

extern "C" fn url_quote_plus(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::None;
    }

    let input = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    // URL encoding with space as +
    let encoded = input.chars()
        .map(|c| match c {
            'A'..='Z' | 'a'..='z' | '0'..='9' | '-' | '_' | '.' | '~' => c.to_string(),
            ' ' => "+".to_string(),
            _ => format!("%{:02X}", c as u8),
        })
        .collect::<String>();

    Value::Str(encoded)
}

extern "C" fn url_unquote_plus(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::None;
    }

    let input = unsafe {
        match &*args {
            Value::Str(s) => s.replace('+', " "),
            _ => return Value::None,
        }
    };

    url_unquote(&Value::Str(input) as *const Value, 1)
}

// HTTP status utilities
extern "C" fn get_status_text(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::None;
    }

    let status = unsafe {
        match &*args {
            Value::Int(s) => *s,
            _ => return Value::None,
        }
    };

    Value::Str(get_status_reason(status))
}

fn get_status_reason(status: i64) -> String {
    match status {
        200 => "OK",
        201 => "Created",
        204 => "No Content",
        301 => "Moved Permanently",
        302 => "Found",
        304 => "Not Modified",
        400 => "Bad Request",
        401 => "Unauthorized",
        403 => "Forbidden",
        404 => "Not Found",
        405 => "Method Not Allowed",
        500 => "Internal Server Error",
        502 => "Bad Gateway",
        503 => "Service Unavailable",
        _ => "Unknown",
    }.to_string()
}

extern "C" fn is_informational(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::Bool(false);
    }

    let status = unsafe {
        match &*args {
            Value::Int(s) => *s,
            _ => return Value::Bool(false),
        }
    };

    Value::Bool(status >= 100 && status < 200)
}

extern "C" fn is_success(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::Bool(false);
    }

    let status = unsafe {
        match &*args {
            Value::Int(s) => *s,
            _ => return Value::Bool(false),
        }
    };

    Value::Bool(status >= 200 && status < 300)
}

extern "C" fn is_redirect(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::Bool(false);
    }

    let status = unsafe {
        match &*args {
            Value::Int(s) => *s,
            _ => return Value::Bool(false),
        }
    };

    Value::Bool(status >= 300 && status < 400)
}

extern "C" fn is_client_error(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::Bool(false);
    }

    let status = unsafe {
        match &*args {
            Value::Int(s) => *s,
            _ => return Value::Bool(false),
        }
    };

    Value::Bool(status >= 400 && status < 500)
}

extern "C" fn is_server_error(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::Bool(false);
    }

    let status = unsafe {
        match &*args {
            Value::Int(s) => *s,
            _ => return Value::Bool(false),
        }
    };

    Value::Bool(status >= 500 && status < 600)
}

// HTTP method utilities
extern "C" fn is_safe_method(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::Bool(false);
    }

    let method = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::Bool(false),
        }
    };

    let safe_methods = ["GET", "HEAD", "OPTIONS", "TRACE"];
    Value::Bool(safe_methods.contains(&method.to_uppercase().as_str()))
}

extern "C" fn is_idempotent_method(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::Bool(false);
    }

    let method = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::Bool(false),
        }
    };

    let idempotent_methods = ["GET", "HEAD", "PUT", "DELETE", "OPTIONS", "TRACE"];
    Value::Bool(idempotent_methods.contains(&method.to_uppercase().as_str()))
}

// Parser classes
// Wrapper functions to match NativeFunction signature
fn parser_reset_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = parser_reset(args.as_ptr(), args.len());
    Ok(result)
}

extern "C" fn create_request_parser(_args: *const Value, _argc: usize) -> Value {
    let mut parser_obj = HashMap::new();
    parser_obj.insert("parse".to_string(), Value::NativeFunction(parse_request_wrapper));
    parser_obj.insert("reset".to_string(), Value::NativeFunction(parser_reset_wrapper));

    Value::Object {
        class_name: "RequestParser".to_string(),
        fields: Rc::new(RefCell::new(parser_obj)),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("RequestParser".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["RequestParser".to_string(), "object".to_string()])
    }
}

extern "C" fn create_response_parser(_args: *const Value, _argc: usize) -> Value {
    let mut parser_obj = HashMap::new();
    parser_obj.insert("parse".to_string(), Value::NativeFunction(parse_response_wrapper));
    parser_obj.insert("reset".to_string(), Value::NativeFunction(parser_reset_wrapper));

    Value::Object {
        class_name: "ResponseParser".to_string(),
        fields: Rc::new(RefCell::new(parser_obj)),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("ResponseParser".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["ResponseParser".to_string(), "object".to_string()])
    }
}

extern "C" fn parser_reset(_args: *const Value, _argc: usize) -> Value {
    Value::None
}

// Exception classes
extern "C" fn http_parser_error(_args: *const Value, _argc: usize) -> Value {
    Value::Str("HttpParserError".to_string())
}

extern "C" fn http_parser_upgrade(_args: *const Value, _argc: usize) -> Value {
    Value::Str("HttpParserUpgrade".to_string())
}