use crate::value::Value;
use std::collections::HashMap;
use std::rc::Rc;
use crate::modules::hplist::HPList;

#[cfg(feature = "http")]

// Wrapper functions to match NativeFunction and BuiltinFunction signatures
fn httpx_get_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = httpx_get(args.as_ptr(), args.len());
    Ok(result)
}

fn httpx_post_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = httpx_post(args.as_ptr(), args.len());
    Ok(result)
}

fn httpx_put_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = httpx_put(args.as_ptr(), args.len());
    Ok(result)
}

fn httpx_delete_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = httpx_delete(args.as_ptr(), args.len());
    Ok(result)
}

fn httpx_head_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = httpx_head(args.as_ptr(), args.len());
    Ok(result)
}

fn httpx_options_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = httpx_options(args.as_ptr(), args.len());
    Ok(result)
}

fn httpx_patch_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = httpx_patch(args.as_ptr(), args.len());
    Ok(result)
}

fn httpx_request_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = httpx_request(args.as_ptr(), args.len());
    Ok(result)
}

fn create_async_client_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = create_async_client(args.as_ptr(), args.len());
    Ok(result)
}

fn create_sync_client_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = create_sync_client(args.as_ptr(), args.len());
    Ok(result)
}

fn create_request_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = create_request(args.as_ptr(), args.len());
    Ok(result)
}

fn create_response_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = create_response(args.as_ptr(), args.len());
    Ok(result)
}

fn create_basic_auth_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = create_basic_auth(args.as_ptr(), args.len());
    Ok(result)
}

fn create_digest_auth_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = create_digest_auth(args.as_ptr(), args.len());
    Ok(result)
}

fn request_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = request_error(args.as_ptr(), args.len());
    Ok(result)
}

fn http_status_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = http_status_error(args.as_ptr(), args.len());
    Ok(result)
}

fn timeout_exception_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = timeout_exception(args.as_ptr(), args.len());
    Ok(result)
}

fn connect_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = connect_error(args.as_ptr(), args.len());
    Ok(result)
}

fn read_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = read_error(args.as_ptr(), args.len());
    Ok(result)
}

fn write_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = write_error(args.as_ptr(), args.len());
    Ok(result)
}

fn protocol_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = protocol_error(args.as_ptr(), args.len());
    Ok(result)
}

fn decoding_error_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = decoding_error(args.as_ptr(), args.len());
    Ok(result)
}

fn too_many_redirects_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = too_many_redirects(args.as_ptr(), args.len());
    Ok(result)
}

fn create_limits_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = create_limits(args.as_ptr(), args.len());
    Ok(result)
}

fn create_timeout_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = create_timeout(args.as_ptr(), args.len());
    Ok(result)
}

fn response_text_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = response_text(args.as_ptr(), args.len());
    Ok(result)
}

fn response_json_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = response_json(args.as_ptr(), args.len());
    Ok(result)
}

fn raise_for_status_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = raise_for_status(args.as_ptr(), args.len());
    Ok(result)
}

// Client method wrappers
fn client_get_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_get(args.as_ptr(), args.len());
    Ok(result)
}

fn client_post_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_post(args.as_ptr(), args.len());
    Ok(result)
}

fn client_put_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_put(args.as_ptr(), args.len());
    Ok(result)
}

fn client_delete_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_delete(args.as_ptr(), args.len());
    Ok(result)
}

fn client_head_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_head(args.as_ptr(), args.len());
    Ok(result)
}

fn client_options_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_options(args.as_ptr(), args.len());
    Ok(result)
}

fn client_patch_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_patch(args.as_ptr(), args.len());
    Ok(result)
}

fn client_request_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_request(args.as_ptr(), args.len());
    Ok(result)
}

fn client_stream_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_stream(args.as_ptr(), args.len());
    Ok(result)
}

fn client_send_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_send(args.as_ptr(), args.len());
    Ok(result)
}

fn client_build_request_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_build_request(args.as_ptr(), args.len());
    Ok(result)
}

fn client_close_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = client_close(args.as_ptr(), args.len());
    Ok(result)
}

// Stream method wrappers
fn stream_iter_bytes_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = stream_iter_bytes(args.as_ptr(), args.len());
    Ok(result)
}

fn stream_iter_text_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = stream_iter_text(args.as_ptr(), args.len());
    Ok(result)
}

fn stream_iter_lines_wrapper(args: Vec<Value>) -> anyhow::Result<Value> {
    let result = stream_iter_lines(args.as_ptr(), args.len());
    Ok(result)
}

/// Create the httpx module
pub fn create_httpx_module() -> Value {
    let mut namespace = HashMap::new();

    // HTTP client functions
    namespace.insert("get".to_string(), Value::NativeFunction(httpx_get_wrapper));
    namespace.insert("post".to_string(), Value::NativeFunction(httpx_post_wrapper));
    namespace.insert("put".to_string(), Value::NativeFunction(httpx_put_wrapper));
    namespace.insert("delete".to_string(), Value::NativeFunction(httpx_delete_wrapper));
    namespace.insert("head".to_string(), Value::NativeFunction(httpx_head_wrapper));
    namespace.insert("options".to_string(), Value::NativeFunction(httpx_options_wrapper));
    namespace.insert("patch".to_string(), Value::NativeFunction(httpx_patch_wrapper));
    namespace.insert("request".to_string(), Value::NativeFunction(httpx_request_wrapper));

    // Async HTTP client functions
    namespace.insert("AsyncClient".to_string(), Value::BuiltinFunction("AsyncClient".to_string(), create_async_client_wrapper));
    namespace.insert("Client".to_string(), Value::BuiltinFunction("Client".to_string(), create_sync_client_wrapper));

    // Request and Response classes
    namespace.insert("Request".to_string(), Value::BuiltinFunction("Request".to_string(), create_request_wrapper));
    namespace.insert("Response".to_string(), Value::BuiltinFunction("Response".to_string(), create_response_wrapper));

    // Authentication classes
    namespace.insert("BasicAuth".to_string(), Value::BuiltinFunction("BasicAuth".to_string(), create_basic_auth_wrapper));
    namespace.insert("DigestAuth".to_string(), Value::BuiltinFunction("DigestAuth".to_string(), create_digest_auth_wrapper));

    // Exception classes
    namespace.insert("RequestError".to_string(), Value::BuiltinFunction("RequestError".to_string(), request_error_wrapper));
    namespace.insert("HTTPStatusError".to_string(), Value::BuiltinFunction("HTTPStatusError".to_string(), http_status_error_wrapper));
    namespace.insert("TimeoutException".to_string(), Value::BuiltinFunction("TimeoutException".to_string(), timeout_exception_wrapper));
    namespace.insert("ConnectError".to_string(), Value::BuiltinFunction("ConnectError".to_string(), connect_error_wrapper));
    namespace.insert("ReadError".to_string(), Value::BuiltinFunction("ReadError".to_string(), read_error_wrapper));
    namespace.insert("WriteError".to_string(), Value::BuiltinFunction("WriteError".to_string(), write_error_wrapper));
    namespace.insert("ProtocolError".to_string(), Value::BuiltinFunction("ProtocolError".to_string(), protocol_error_wrapper));
    namespace.insert("DecodingError".to_string(), Value::BuiltinFunction("DecodingError".to_string(), decoding_error_wrapper));
    namespace.insert("TooManyRedirects".to_string(), Value::BuiltinFunction("TooManyRedirects".to_string(), too_many_redirects_wrapper));

    // Status codes
    namespace.insert("codes".to_string(), create_status_codes(std::ptr::null(), 0));

    // Limits and configuration
    namespace.insert("Limits".to_string(), Value::BuiltinFunction("Limits".to_string(), create_limits_wrapper));
    namespace.insert("Timeout".to_string(), Value::BuiltinFunction("Timeout".to_string(), create_timeout_wrapper));

    Value::Module("httpx".to_string(), namespace)
}

// HTTP client functions
extern "C" fn httpx_get(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("GET", args, argc)
}

extern "C" fn httpx_post(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("POST", args, argc)
}

extern "C" fn httpx_put(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("PUT", args, argc)
}

extern "C" fn httpx_delete(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("DELETE", args, argc)
}

extern "C" fn httpx_head(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("HEAD", args, argc)
}

extern "C" fn httpx_options(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("OPTIONS", args, argc)
}

extern "C" fn httpx_patch(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("PATCH", args, argc)
}

extern "C" fn httpx_request(args: *const Value, argc: usize) -> Value {
    if argc < 2 {
        return Value::None;
    }

    let method = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    httpx_request_helper(method, unsafe { args.add(1) }, argc - 1)
}

fn httpx_request_helper(method: &str, args: *const Value, argc: usize) -> Value {
    #[cfg(feature = "http")]
    {
        if argc == 0 {
            return Value::None;
        }

        let url = unsafe {
            match &*args {
                Value::Str(s) => s,
                _ => return Value::None,
            }
        };

        // Mock HTTP response
         let mut response_obj = HashMap::new();
         response_obj.insert("status_code".to_string(), Value::Int(200));
         response_obj.insert("text".to_string(), Value::NativeFunction(response_text_wrapper));
         response_obj.insert("json".to_string(), Value::NativeFunction(response_json_wrapper));
         response_obj.insert("content".to_string(), Value::Bytes(b"Mock response content".to_vec()));
         response_obj.insert("headers".to_string(), create_mock_headers());
         response_obj.insert("url".to_string(), Value::Str(url.clone()));
         response_obj.insert("method".to_string(), Value::Str(method.to_string()));
         response_obj.insert("is_error".to_string(), Value::Bool(false));
         response_obj.insert("is_redirect".to_string(), Value::Bool(false));
         response_obj.insert("is_client_error".to_string(), Value::Bool(false));
         response_obj.insert("is_server_error".to_string(), Value::Bool(false));
         response_obj.insert("raise_for_status".to_string(), Value::NativeFunction(raise_for_status_wrapper));
         response_obj.insert("elapsed".to_string(), Value::Float(0.123));
         response_obj.insert("encoding".to_string(), Value::Str("utf-8".to_string()));
        
        Value::Object {
        class_name: "Response".to_string(),
        fields: Rc::new(response_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Response".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Response".to_string(), "object".to_string()])
    }
    }
    #[cfg(not(feature = "http"))]
    {
        Value::Str("HTTP support not enabled".to_string())
    }
}

fn create_mock_headers() -> Value {
    let mut headers = HashMap::new();
    headers.insert("content-type".to_string(), Value::Str("application/json".to_string()));
    headers.insert("content-length".to_string(), Value::Str("23".to_string()));
    headers.insert("server".to_string(), Value::Str("httpx-mock/1.0".to_string()));
    
    Value::Object {
        class_name: "Headers".to_string(),
        fields: Rc::new(headers),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Headers".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Headers".to_string(), "object".to_string()])
    }
}

// Client classes
 extern "C" fn create_async_client(_args: *const Value, _argc: usize) -> Value {
     let mut client_obj = HashMap::new();
     client_obj.insert("get".to_string(), Value::NativeFunction(client_get_wrapper));
     client_obj.insert("post".to_string(), Value::NativeFunction(client_post_wrapper));
     client_obj.insert("put".to_string(), Value::NativeFunction(client_put_wrapper));
     client_obj.insert("delete".to_string(), Value::NativeFunction(client_delete_wrapper));
     client_obj.insert("head".to_string(), Value::NativeFunction(client_head_wrapper));
     client_obj.insert("options".to_string(), Value::NativeFunction(client_options_wrapper));
     client_obj.insert("patch".to_string(), Value::NativeFunction(client_patch_wrapper));
     client_obj.insert("request".to_string(), Value::NativeFunction(client_request_wrapper));
     client_obj.insert("stream".to_string(), Value::NativeFunction(client_stream_wrapper));
     client_obj.insert("send".to_string(), Value::NativeFunction(client_send_wrapper));
     client_obj.insert("build_request".to_string(), Value::NativeFunction(client_build_request_wrapper));
     client_obj.insert("close".to_string(), Value::NativeFunction(client_close_wrapper));
    client_obj.insert("is_closed".to_string(), Value::Bool(false));
    client_obj.insert("cookies".to_string(), Value::Object { 
        class_name: "Cookies".to_string(),
        fields: Rc::new(HashMap::new()),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Cookies".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Cookies".to_string(), "object".to_string()])
    });
    client_obj.insert("headers".to_string(), Value::Object { 
        class_name: "Headers".to_string(),
        fields: Rc::new(HashMap::new()),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Headers".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Headers".to_string(), "object".to_string()])
    });
    client_obj.insert("params".to_string(), Value::Object { 
        class_name: "Params".to_string(),
        fields: Rc::new(HashMap::new()),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Params".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Params".to_string(), "object".to_string()])
    });
    
    Value::Object { 
        class_name: "AsyncClient".to_string(),
        fields: Rc::new(client_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("AsyncClient".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["AsyncClient".to_string(), "object".to_string()])
    }
 }

extern "C" fn create_sync_client(_args: *const Value, _argc: usize) -> Value {
    create_async_client(_args, _argc)
}

// Client methods
extern "C" fn client_get(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("GET", args, argc)
}

extern "C" fn client_post(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("POST", args, argc)
}

extern "C" fn client_put(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("PUT", args, argc)
}

extern "C" fn client_delete(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("DELETE", args, argc)
}

extern "C" fn client_head(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("HEAD", args, argc)
}

extern "C" fn client_options(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("OPTIONS", args, argc)
}

extern "C" fn client_patch(args: *const Value, argc: usize) -> Value {
    httpx_request_helper("PATCH", args, argc)
}

extern "C" fn client_request(args: *const Value, argc: usize) -> Value {
    if argc < 2 {
        return Value::None;
    }

    let method = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    httpx_request_helper(method, unsafe { args.add(1) }, argc - 1)
}

extern "C" fn client_stream(args: *const Value, argc: usize) -> Value {
     // Mock streaming response
     let mut stream_obj = HashMap::new();
     stream_obj.insert("iter_bytes".to_string(), Value::NativeFunction(stream_iter_bytes_wrapper));
    stream_obj.insert("iter_text".to_string(), Value::NativeFunction(stream_iter_text_wrapper));
    stream_obj.insert("iter_lines".to_string(), Value::NativeFunction(stream_iter_lines_wrapper));
    
    Value::Object { 
        class_name: "Response".to_string(),
        fields: Rc::new(stream_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Response".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Response".to_string(), "object".to_string()])
    }
 }

extern "C" fn client_send(args: *const Value, argc: usize) -> Value {
    if argc == 0 {
        return Value::None;
    }

    // Mock sending a request
    httpx_request_helper("GET", args, argc)
}

extern "C" fn client_build_request(args: *const Value, argc: usize) -> Value {
    if argc < 2 {
        return Value::None;
    }

    let method = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    let url = unsafe {
        match &*args.add(1) {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    // Mock request object
    let mut request_obj = HashMap::new();
    request_obj.insert("method".to_string(), Value::Str(method.clone()));
    request_obj.insert("url".to_string(), Value::Str(url.clone()));
    request_obj.insert("headers".to_string(), Value::Object { 
        class_name: "Headers".to_string(),
        fields: Rc::new(HashMap::new()),  // Wrap with Rc::new
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Headers".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Headers".to_string(), "object".to_string()])
    });
    request_obj.insert("content".to_string(), Value::None);
    
    Value::Object { 
        class_name: "Request".to_string(),
        fields: Rc::new(request_obj),  // Wrap with Rc::new
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Request".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Request".to_string(), "object".to_string()])
    }
}

extern "C" fn client_close(_args: *const Value, _argc: usize) -> Value {
    Value::None
}

// Stream methods
extern "C" fn stream_iter_bytes(_args: *const Value, _argc: usize) -> Value {
    // Mock byte iterator
    Value::List(HPList::from_values(vec![Value::Bytes(b"chunk1".to_vec()), Value::Bytes(b"chunk2".to_vec())]))
}

extern "C" fn stream_iter_text(_args: *const Value, _argc: usize) -> Value {
    // Mock text iterator
    Value::List(HPList::from_values(vec![Value::Str("line1".to_string()), Value::Str("line2".to_string())]))
}

extern "C" fn stream_iter_lines(_args: *const Value, _argc: usize) -> Value {
    // Mock line iterator
    Value::List(HPList::from_values(vec![Value::Str("line1\n".to_string()), Value::Str("line2\n".to_string())]))
}

// Request and Response classes
extern "C" fn create_request(args: *const Value, argc: usize) -> Value {
    if argc < 2 {
        return Value::None;
    }

    let method = unsafe {
        match &*args {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    let url = unsafe {
        match &*args.add(1) {
            Value::Str(s) => s,
            _ => return Value::None,
        }
    };

    let mut request_obj = HashMap::new();
    request_obj.insert("method".to_string(), Value::Str(method.clone()));
    request_obj.insert("url".to_string(), Value::Str(url.clone()));
    request_obj.insert("headers".to_string(), Value::Object { 
        class_name: "Headers".to_string(),
        fields: Rc::new(HashMap::new()),  // Wrap with Rc::new
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Headers".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Headers".to_string(), "object".to_string()])
    });
    request_obj.insert("content".to_string(), Value::None);
    request_obj.insert("stream".to_string(), Value::Bool(false));
    
    Value::Object { 
        class_name: "Request".to_string(),
        fields: Rc::new(request_obj),  // Wrap with Rc::new
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Request".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Request".to_string(), "object".to_string()])
    }
}

extern "C" fn create_response(_args: *const Value, _argc: usize) -> Value {
     let mut response_obj = HashMap::new();
     response_obj.insert("status_code".to_string(), Value::Int(200));
     response_obj.insert("text".to_string(), Value::NativeFunction(response_text_wrapper));
     response_obj.insert("json".to_string(), Value::NativeFunction(response_json_wrapper));
     response_obj.insert("content".to_string(), Value::Bytes(b"Response content".to_vec()));
     response_obj.insert("headers".to_string(), create_mock_headers());
     response_obj.insert("url".to_string(), Value::Str("https://example.com".to_string()));
     response_obj.insert("method".to_string(), Value::Str("GET".to_string()));
     response_obj.insert("is_error".to_string(), Value::Bool(false));
     response_obj.insert("is_redirect".to_string(), Value::Bool(false));
     response_obj.insert("is_client_error".to_string(), Value::Bool(false));
     response_obj.insert("is_server_error".to_string(), Value::Bool(false));
     response_obj.insert("raise_for_status".to_string(), Value::NativeFunction(raise_for_status_wrapper));
     response_obj.insert("elapsed".to_string(), Value::Float(0.123));
     response_obj.insert("encoding".to_string(), Value::Str("utf-8".to_string()));
     
     Value::Object {
         class_name: "Response".to_string(),
         fields: Rc::new(response_obj),
         class_methods: HashMap::new(),
         base_object: crate::base_object::BaseObject::new("Response".to_string(), vec!["object".to_string()]),
         mro: crate::base_object::MRO::from_linearization(vec!["Response".to_string(), "object".to_string()])
     }
 }

// Response methods
extern "C" fn response_text(_args: *const Value, _argc: usize) -> Value {
    Value::Str("Mock response text".to_string())
}

extern "C" fn response_json(_args: *const Value, _argc: usize) -> Value {
    let mut json_obj = HashMap::new();
    json_obj.insert("message".to_string(), Value::Str("Hello, World!".to_string()));
    json_obj.insert("status".to_string(), Value::Str("success".to_string()));
    
    Value::Object {
        class_name: "JSONResponse".to_string(),
        fields: Rc::new(json_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("JSONResponse".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["JSONResponse".to_string(), "object".to_string()])
    }
}

extern "C" fn raise_for_status(_args: *const Value, _argc: usize) -> Value {
    // Mock - would raise exception for 4xx/5xx status codes
    Value::None
}

// Authentication classes
extern "C" fn create_basic_auth(_args: *const Value, _argc: usize) -> Value {
    let mut auth_obj = HashMap::new();
    auth_obj.insert("username".to_string(), Value::Str("user".to_string()));
    auth_obj.insert("password".to_string(), Value::Str("pass".to_string()));
    
    Value::Object {
        class_name: "BasicAuth".to_string(),
        fields: Rc::new(auth_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("BasicAuth".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["BasicAuth".to_string(), "object".to_string()])
    }
}

extern "C" fn create_digest_auth(_args: *const Value, _argc: usize) -> Value {
    let mut auth_obj = HashMap::new();
    auth_obj.insert("username".to_string(), Value::Str("user".to_string()));
    auth_obj.insert("password".to_string(), Value::Str("pass".to_string()));
    auth_obj.insert("realm".to_string(), Value::Str("protected".to_string()));
    
    Value::Object {
        class_name: "DigestAuth".to_string(),
        fields: Rc::new(auth_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("DigestAuth".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["DigestAuth".to_string(), "object".to_string()])
    }
}

extern "C" fn create_limits(_args: *const Value, _argc: usize) -> Value {
    let mut limits_obj = HashMap::new();
    limits_obj.insert("max_keepalive_connections".to_string(), Value::Int(20));
    limits_obj.insert("max_connections".to_string(), Value::Int(100));
    limits_obj.insert("keepalive_expiry".to_string(), Value::Float(5.0));
    
    Value::Object {
        class_name: "Limits".to_string(),
        fields: Rc::new(limits_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Limits".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Limits".to_string(), "object".to_string()])
    }
}

extern "C" fn create_timeout(_args: *const Value, _argc: usize) -> Value {
    let mut timeout_obj = HashMap::new();
    timeout_obj.insert("connect".to_string(), Value::Float(5.0));
    timeout_obj.insert("read".to_string(), Value::Float(5.0));
    timeout_obj.insert("write".to_string(), Value::Float(5.0));
    timeout_obj.insert("pool".to_string(), Value::Float(5.0));
    
    Value::Object {
        class_name: "Timeout".to_string(),
        fields: Rc::new(timeout_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Timeout".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Timeout".to_string(), "object".to_string()])
    }
}

extern "C" fn create_status_codes(_args: *const Value, _argc: usize) -> Value {
    let mut codes = HashMap::new();
    codes.insert("OK".to_string(), Value::Int(200));
    codes.insert("NOT_FOUND".to_string(), Value::Int(404));
    codes.insert("INTERNAL_SERVER_ERROR".to_string(), Value::Int(500));
    codes.insert("BAD_REQUEST".to_string(), Value::Int(400));
    codes.insert("UNAUTHORIZED".to_string(), Value::Int(401));
    codes.insert("FORBIDDEN".to_string(), Value::Int(403));
    codes.insert("CREATED".to_string(), Value::Int(201));
    codes.insert("NO_CONTENT".to_string(), Value::Int(204));
    
    Value::Object {
        class_name: "StatusCodes".to_string(),
        fields: Rc::new(codes),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("StatusCodes".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["StatusCodes".to_string(), "object".to_string()])
    }
}

extern "C" fn request_error(_args: *const Value, _argc: usize) -> Value {
    Value::Str("RequestError".to_string())
}

extern "C" fn http_status_error(_args: *const Value, _argc: usize) -> Value {
    Value::Str("HTTPStatusError".to_string())
}

extern "C" fn timeout_exception(_args: *const Value, _argc: usize) -> Value {
    Value::Str("TimeoutException".to_string())
}

extern "C" fn connect_error(_args: *const Value, _argc: usize) -> Value {
    Value::Str("ConnectError".to_string())
}

extern "C" fn read_error(_args: *const Value, _argc: usize) -> Value {
    Value::Str("ReadError".to_string())
}

extern "C" fn write_error(_args: *const Value, _argc: usize) -> Value {
    Value::Str("WriteError".to_string())
}

extern "C" fn protocol_error(_args: *const Value, _argc: usize) -> Value {
    Value::Str("ProtocolError".to_string())
}

extern "C" fn decoding_error(_args: *const Value, _argc: usize) -> Value {
    Value::Str("DecodingError".to_string())
}

extern "C" fn too_many_redirects(_args: *const Value, _argc: usize) -> Value {
    Value::Str("TooManyRedirects".to_string())
}