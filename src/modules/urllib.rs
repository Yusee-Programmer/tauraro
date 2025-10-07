use crate::value::Value;
use std::collections::HashMap;
use url::Url;
use anyhow::{Result, anyhow};

pub fn create_urllib_module() -> Value {
    let mut namespace = HashMap::new();

    // Create urllib.parse submodule
    let mut parse_namespace = HashMap::new();

    // Add urlparse function
    parse_namespace.insert(
        "urlparse".to_string(),
        Value::NativeFunction(urlparse),
    );

    // Add urlunparse function
    parse_namespace.insert(
        "urlunparse".to_string(),
        Value::NativeFunction(urlunparse),
    );

    // Add urlencode function
    parse_namespace.insert(
        "urlencode".to_string(),
        Value::NativeFunction(urlencode),
    );

    // Add parse_qs function
    parse_namespace.insert(
        "parse_qs".to_string(),
        Value::NativeFunction(parse_qs),
    );

    // Add parse_qsl function
    parse_namespace.insert(
        "parse_qsl".to_string(),
        Value::NativeFunction(parse_qsl),
    );

    // Add quote function
    parse_namespace.insert(
        "quote".to_string(),
        Value::NativeFunction(quote),
    );

    // Add quote_plus function
    parse_namespace.insert(
        "quote_plus".to_string(),
        Value::NativeFunction(quote_plus),
    );

    // Add unquote function
    parse_namespace.insert(
        "unquote".to_string(),
        Value::NativeFunction(unquote),
    );

    // Add unquote_plus function
    parse_namespace.insert(
        "unquote_plus".to_string(),
        Value::NativeFunction(unquote_plus),
    );

    // Create the parse submodule
    namespace.insert(
        "parse".to_string(),
        Value::Module("parse".to_string(), parse_namespace),
    );

    // Create urllib.request submodule
    let mut request_namespace = HashMap::new();

    request_namespace.insert(
        "urlopen".to_string(),
        Value::NativeFunction(urlopen),
    );

    request_namespace.insert(
        "Request".to_string(),
        Value::NativeFunction(create_request),
    );

    request_namespace.insert(
        "urlretrieve".to_string(),
        Value::NativeFunction(urlretrieve),
    );

    namespace.insert(
        "request".to_string(),
        Value::Module("request".to_string(), request_namespace),
    );

    // Create urllib.error submodule
    let mut error_namespace = HashMap::new();

    error_namespace.insert(
        "URLError".to_string(),
        Value::Str("URLError".to_string()),
    );

    error_namespace.insert(
        "HTTPError".to_string(),
        Value::Str("HTTPError".to_string()),
    );

    namespace.insert(
        "error".to_string(),
        Value::Module("error".to_string(), error_namespace),
    );

    Value::Module("urllib".to_string(), namespace)
}

fn urlparse(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("urlparse() missing required argument: 'url'"));
    }
    
    let url_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("urlparse() argument must be a string")),
    };
    
    match Url::parse(url_str) {
        Ok(url) => {
            let mut result = HashMap::new();
            
            result.insert("scheme".to_string(), Value::Str(url.scheme().to_string()));
            result.insert("netloc".to_string(), Value::Str(url.host_str().unwrap_or("").to_string()));
            result.insert("path".to_string(), Value::Str(url.path().to_string()));
            result.insert("params".to_string(), Value::Str("".to_string())); // URL crate doesn't separate params
            result.insert("query".to_string(), Value::Str(url.query().unwrap_or("").to_string()));
            result.insert("fragment".to_string(), Value::Str(url.fragment().unwrap_or("").to_string()));
            
            Ok(Value::Dict(result))
        }
        Err(_) => {
            // If URL parsing fails, return a basic structure with empty values
            let mut result = HashMap::new();
            result.insert("scheme".to_string(), Value::Str("".to_string()));
            result.insert("netloc".to_string(), Value::Str("".to_string()));
            result.insert("path".to_string(), Value::Str(url_str.to_string()));
            result.insert("params".to_string(), Value::Str("".to_string()));
            result.insert("query".to_string(), Value::Str("".to_string()));
            result.insert("fragment".to_string(), Value::Str("".to_string()));
            
            Ok(Value::Dict(result))
        }
    }
}

fn urlencode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("urlencode() missing required argument: 'query'"));
    }
    
    let query_dict = match &args[0] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("urlencode() argument must be a dictionary")),
    };
    
    let mut encoded_pairs = Vec::new();
    
    for (key, value) in query_dict {
        let key_str = match key {
            k => k.clone(),
        };
        
        let value_str = match value {
            Value::Str(s) => s.clone(),
            Value::Int(i) => i.to_string(),
            Value::Float(f) => f.to_string(),
            _ => return Err(anyhow!("urlencode() values must be strings or numbers")),
        };
        
        // URL encode the key and value
        let encoded_key = percent_encode(&key_str);
        let encoded_value = percent_encode(&value_str);
        
        encoded_pairs.push(format!("{}={}", encoded_key, encoded_value));
    }
    
    Ok(Value::Str(encoded_pairs.join("&")))
}

fn quote(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("quote() missing required argument: 'string'"));
    }
    
    let input_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("quote() argument must be a string")),
    };
    
    // URL encode the string
    let encoded = percent_encode(input_str);
    Ok(Value::Str(encoded))
}

fn unquote(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("unquote() missing required argument: 'string'"));
    }
    
    let input_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("unquote() argument must be a string")),
    };
    
    // URL decode the string
    let decoded = percent_decode(input_str);
    Ok(Value::Str(decoded))
}

fn percent_encode(input: &str) -> String {
    let mut result = String::new();
    
    for byte in input.bytes() {
        match byte {
            // Unreserved characters (don't encode)
            b'A'..=b'Z' | b'a'..=b'z' | b'0'..=b'9' | b'-' | b'.' | b'_' | b'~' => {
                result.push(byte as char);
            }
            // Space becomes %20 (not + for quote function)
            b' ' => {
                result.push_str("%20");
            }
            // Everything else gets percent-encoded
            _ => {
                result.push_str(&format!("%{:02X}", byte));
            }
        }
    }
    
    result
}

fn percent_decode(input: &str) -> String {
    let mut result = String::new();
    let mut chars = input.chars().peekable();

    while let Some(ch) = chars.next() {
        if ch == '%' {
            // Try to decode the next two characters as hex
            let hex1 = chars.next();
            let hex2 = chars.next();

            if let (Some(h1), Some(h2)) = (hex1, hex2) {
                let hex_str = format!("{}{}", h1, h2);
                if let Ok(byte_val) = u8::from_str_radix(&hex_str, 16) {
                    result.push(byte_val as char);
                    continue;
                }
            }

            // If decoding fails, just add the % character
            result.push(ch);
            if let Some(h1) = hex1 {
                result.push(h1);
            }
            if let Some(h2) = hex2 {
                result.push(h2);
            }
        } else {
            result.push(ch);
        }
    }

    result
}

fn urlunparse(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("urlunparse() missing required argument: 'parts'"));
    }

    // Parts should be a tuple/list of (scheme, netloc, path, params, query, fragment)
    let parts = match &args[0] {
        Value::List(list) => {
            let mut values = Vec::new();
            for i in 0..list.len() {
                if let Some(val) = list.get(i as isize) {
                    values.push(val.clone());
                }
            }
            values
        }
        _ => return Err(anyhow!("urlunparse() argument must be a list")),
    };

    if parts.len() < 6 {
        return Err(anyhow!("urlunparse() requires a list of 6 elements"));
    }

    let scheme = match &parts[0] {
        Value::Str(s) => s.clone(),
        _ => String::new(),
    };

    let netloc = match &parts[1] {
        Value::Str(s) => s.clone(),
        _ => String::new(),
    };

    let path = match &parts[2] {
        Value::Str(s) => s.clone(),
        _ => String::new(),
    };

    let query = match &parts[4] {
        Value::Str(s) => s.clone(),
        _ => String::new(),
    };

    let fragment = match &parts[5] {
        Value::Str(s) => s.clone(),
        _ => String::new(),
    };

    let mut url = format!("{}://{}{}", scheme, netloc, path);

    if !query.is_empty() {
        url.push_str("?");
        url.push_str(&query);
    }

    if !fragment.is_empty() {
        url.push_str("#");
        url.push_str(&fragment);
    }

    Ok(Value::Str(url))
}

fn parse_qs(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("parse_qs() missing required argument: 'qs'"));
    }

    let query_string = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("parse_qs() argument must be a string")),
    };

    let mut result = HashMap::new();

    for pair in query_string.split('&') {
        if let Some((key, value)) = pair.split_once('=') {
            let decoded_key = percent_decode(key);
            let decoded_value = percent_decode(value);

            // In Python's parse_qs, values are lists
            let mut list = crate::modules::hplist::HPList::new();
            list.append(Value::Str(decoded_value));
            result.insert(decoded_key, Value::List(list));
        }
    }

    Ok(Value::Dict(result))
}

fn parse_qsl(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("parse_qsl() missing required argument: 'qs'"));
    }

    let query_string = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("parse_qsl() argument must be a string")),
    };

    let mut list = crate::modules::hplist::HPList::new();

    for pair in query_string.split('&') {
        if let Some((key, value)) = pair.split_once('=') {
            let decoded_key = percent_decode(key);
            let decoded_value = percent_decode(value);

            let mut tuple_list = crate::modules::hplist::HPList::new();
            tuple_list.append(Value::Str(decoded_key));
            tuple_list.append(Value::Str(decoded_value));

            list.append(Value::List(tuple_list));
        }
    }

    Ok(Value::List(list))
}

fn quote_plus(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("quote_plus() missing required argument: 'string'"));
    }

    let input_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("quote_plus() argument must be a string")),
    };

    // Same as quote but spaces become '+'
    let mut result = String::new();

    for byte in input_str.bytes() {
        match byte {
            b'A'..=b'Z' | b'a'..=b'z' | b'0'..=b'9' | b'-' | b'.' | b'_' | b'~' => {
                result.push(byte as char);
            }
            b' ' => {
                result.push('+');
            }
            _ => {
                result.push_str(&format!("%{:02X}", byte));
            }
        }
    }

    Ok(Value::Str(result))
}

fn unquote_plus(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("unquote_plus() missing required argument: 'string'"));
    }

    let input_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("unquote_plus() argument must be a string")),
    };

    // Replace + with spaces first, then decode
    let with_spaces = input_str.replace('+', " ");
    let decoded = percent_decode(&with_spaces);
    Ok(Value::Str(decoded))
}

// urllib.request functions

fn urlopen(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("urlopen() missing required argument: 'url'"));
    }

    let url_str = match &args[0] {
        Value::Str(s) => s.clone(),
        Value::Dict(d) => {
            // Could be a Request object
            if let Some(Value::Str(url)) = d.get("url") {
                url.clone()
            } else {
                return Err(anyhow!("urlopen() invalid Request object"));
            }
        }
        _ => return Err(anyhow!("urlopen() argument must be a string or Request object")),
    };

    // Create a response object
    let mut response = HashMap::new();
    response.insert("url".to_string(), Value::Str(url_str.clone()));
    response.insert("status".to_string(), Value::Int(200));
    response.insert("read".to_string(), Value::NativeFunction(response_read));
    response.insert("getcode".to_string(), Value::NativeFunction(response_getcode));
    response.insert("geturl".to_string(), Value::NativeFunction(response_geturl));
    response.insert("info".to_string(), Value::NativeFunction(response_info));
    response.insert("close".to_string(), Value::NativeFunction(response_close));

    // Simplified - in a real implementation, this would make an HTTP request
    // For now, we'll just create a mock response
    response.insert("_data".to_string(), Value::Str("".to_string()));

    Ok(Value::Dict(response))
}

fn create_request(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("Request() missing required argument: 'url'"));
    }

    let url = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("Request() url must be a string")),
    };

    let data = if args.len() > 1 {
        args[1].clone()
    } else {
        Value::None
    };

    let headers = if args.len() > 2 {
        match &args[2] {
            Value::Dict(_) => args[2].clone(),
            _ => Value::Dict(HashMap::new()),
        }
    } else {
        Value::Dict(HashMap::new())
    };

    let mut request = HashMap::new();
    request.insert("url".to_string(), Value::Str(url));
    request.insert("data".to_string(), data);
    request.insert("headers".to_string(), headers);
    request.insert("method".to_string(), Value::Str("GET".to_string()));

    Ok(Value::Dict(request))
}

fn urlretrieve(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("urlretrieve() requires url and filename arguments"));
    }

    let url = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("urlretrieve() url must be a string")),
    };

    let filename = match &args[1] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("urlretrieve() filename must be a string")),
    };

    // Simplified - in a real implementation, this would download the file
    println!("Would download {} to {}", url, filename);

    Ok(Value::Str(filename))
}

// Response object methods

fn response_read(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("read() requires self argument"));
    }

    if let Value::Dict(response) = &args[0] {
        if let Some(Value::Str(data)) = response.get("_data") {
            return Ok(Value::Str(data.clone()));
        }
    }

    Ok(Value::Str("".to_string()))
}

fn response_getcode(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("getcode() requires self argument"));
    }

    if let Value::Dict(response) = &args[0] {
        if let Some(Value::Int(status)) = response.get("status") {
            return Ok(Value::Int(*status));
        }
    }

    Ok(Value::Int(200))
}

fn response_geturl(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("geturl() requires self argument"));
    }

    if let Value::Dict(response) = &args[0] {
        if let Some(Value::Str(url)) = response.get("url") {
            return Ok(Value::Str(url.clone()));
        }
    }

    Ok(Value::Str("".to_string()))
}

fn response_info(args: Vec<Value>) -> Result<Value> {
    // Return headers dict
    Ok(Value::Dict(HashMap::new()))
}

fn response_close(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}
