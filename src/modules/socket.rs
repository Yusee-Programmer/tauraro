use crate::value::Value;
use anyhow::{anyhow, Result};
use std::collections::HashMap;
use std::net::{Ipv4Addr, Ipv6Addr, SocketAddr, SocketAddrV4, SocketAddrV6};

/// Create the socket module
pub fn create_socket_module() -> Value {
    let mut namespace = HashMap::new();

    // Socket family constants
    namespace.insert("AF_INET".to_string(), Value::Int(2));
    namespace.insert("AF_INET6".to_string(), Value::Int(10));
    namespace.insert("AF_UNSPEC".to_string(), Value::Int(0));

    // Socket type constants
    namespace.insert("SOCK_STREAM".to_string(), Value::Int(1));
    namespace.insert("SOCK_DGRAM".to_string(), Value::Int(2));
    namespace.insert("SOCK_RAW".to_string(), Value::Int(3));

    // Protocol constants
    namespace.insert("IPPROTO_TCP".to_string(), Value::Int(6));
    namespace.insert("IPPROTO_UDP".to_string(), Value::Int(17));
    namespace.insert("IPPROTO_IP".to_string(), Value::Int(0));

    // Socket level constants
    namespace.insert("SOL_SOCKET".to_string(), Value::Int(1));

    // Socket option constants
    namespace.insert("SO_REUSEADDR".to_string(), Value::Int(2));
    namespace.insert("SO_KEEPALIVE".to_string(), Value::Int(9));
    namespace.insert("SO_BROADCAST".to_string(), Value::Int(6));
    namespace.insert("SO_RCVBUF".to_string(), Value::Int(8));
    namespace.insert("SO_SNDBUF".to_string(), Value::Int(7));
    namespace.insert("SO_RCVTIMEO".to_string(), Value::Int(20));
    namespace.insert("SO_SNDTIMEO".to_string(), Value::Int(21));

    // Error constants
    namespace.insert("EADDRINUSE".to_string(), Value::Int(98));
    namespace.insert("ECONNREFUSED".to_string(), Value::Int(111));
    namespace.insert("ETIMEDOUT".to_string(), Value::Int(110));

    // Functions
    namespace.insert("socket".to_string(), Value::NativeFunction(socket_socket));
    namespace.insert("gethostname".to_string(), Value::NativeFunction(socket_gethostname));
    namespace.insert("gethostbyname".to_string(), Value::NativeFunction(socket_gethostbyname));
    namespace.insert("inet_aton".to_string(), Value::NativeFunction(socket_inet_aton));
    namespace.insert("inet_ntoa".to_string(), Value::NativeFunction(socket_inet_ntoa));
    namespace.insert("htons".to_string(), Value::NativeFunction(socket_htons));
    namespace.insert("ntohs".to_string(), Value::NativeFunction(socket_ntohs));
    namespace.insert("htonl".to_string(), Value::NativeFunction(socket_htonl));
    namespace.insert("ntohl".to_string(), Value::NativeFunction(socket_ntohl));

    // Exception classes
    namespace.insert("error".to_string(), Value::BuiltinFunction("error".to_string(), socket_error));
    namespace.insert("gaierror".to_string(), Value::BuiltinFunction("gaierror".to_string(), socket_gaierror));
    namespace.insert("herror".to_string(), Value::BuiltinFunction("herror".to_string(), socket_herror));
    namespace.insert("timeout".to_string(), Value::BuiltinFunction("timeout".to_string(), socket_timeout));

    Value::Module("socket".to_string(), namespace)
}

/// Get the function for a given name
pub fn get_socket_function(name: &str) -> Option<fn(Vec<Value>) -> Result<Value>> {
    match name {
        "socket" => Some(socket_socket),
        "gethostname" => Some(socket_gethostname),
        "gethostbyname" => Some(socket_gethostbyname),
        "inet_aton" => Some(socket_inet_aton),
        "inet_ntoa" => Some(socket_inet_ntoa),
        "htons" => Some(socket_htons),
        "ntohs" => Some(socket_ntohs),
        "htonl" => Some(socket_htonl),
        "ntohl" => Some(socket_ntohl),
        "error" => Some(socket_error),
        "gaierror" => Some(socket_gaierror),
        "herror" => Some(socket_herror),
        "timeout" => Some(socket_timeout),
        _ => None,
    }
}

/// socket.socket(family=AF_INET, type=SOCK_STREAM, proto=0, fileno=None)
fn socket_socket(args: Vec<Value>) -> Result<Value> {
    let family = if args.is_empty() { 2 } else {
        match &args[0] {
            Value::Int(f) => *f,
            _ => return Err(anyhow!("socket family must be an integer")),
        }
    };

    let socket_type = if args.len() < 2 { 1 } else {
        match &args[1] {
            Value::Int(t) => *t,
            _ => return Err(anyhow!("socket type must be an integer")),
        }
    };

    // Create socket object
    let mut socket_obj = HashMap::new();
    socket_obj.insert("family".to_string(), Value::Int(family));
    socket_obj.insert("type".to_string(), Value::Int(socket_type));
    socket_obj.insert("proto".to_string(), Value::Int(0));
    socket_obj.insert("_closed".to_string(), Value::Bool(false));
    socket_obj.insert("_timeout".to_string(), Value::None);

    // Add socket methods
    socket_obj.insert("bind".to_string(), Value::NativeFunction(socket_bind));
    socket_obj.insert("listen".to_string(), Value::NativeFunction(socket_listen));
    socket_obj.insert("accept".to_string(), Value::NativeFunction(socket_accept));
    socket_obj.insert("connect".to_string(), Value::NativeFunction(socket_connect));
    socket_obj.insert("send".to_string(), Value::NativeFunction(socket_send));
    socket_obj.insert("recv".to_string(), Value::NativeFunction(socket_recv));
    socket_obj.insert("sendto".to_string(), Value::NativeFunction(socket_sendto));
    socket_obj.insert("recvfrom".to_string(), Value::NativeFunction(socket_recvfrom));
    socket_obj.insert("close".to_string(), Value::NativeFunction(socket_close));
    socket_obj.insert("shutdown".to_string(), Value::NativeFunction(socket_shutdown));
    socket_obj.insert("setsockopt".to_string(), Value::NativeFunction(socket_setsockopt));
    socket_obj.insert("getsockopt".to_string(), Value::NativeFunction(socket_getsockopt));
    socket_obj.insert("settimeout".to_string(), Value::NativeFunction(socket_settimeout));
    socket_obj.insert("gettimeout".to_string(), Value::NativeFunction(socket_gettimeout));
    socket_obj.insert("setblocking".to_string(), Value::NativeFunction(socket_setblocking));
    socket_obj.insert("getpeername".to_string(), Value::NativeFunction(socket_getpeername));
    socket_obj.insert("getsockname".to_string(), Value::NativeFunction(socket_getsockname));

    Ok(Value::Object {
        class_name: "socket".to_string(),
        fields: socket_obj,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("socket".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["socket".to_string(), "object".to_string()]),
    })
}

/// socket.bind(address)
fn socket_bind(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("bind() missing required argument: 'address'"));
    }

    // For now, just return None (successful bind)
    // In a full implementation, this would actually bind to the address
    Ok(Value::None)
}

/// socket.listen(backlog=5)
fn socket_listen(args: Vec<Value>) -> Result<Value> {
    let _backlog = if args.is_empty() { 5 } else {
        match &args[0] {
            Value::Int(b) => *b,
            _ => return Err(anyhow!("backlog must be an integer")),
        }
    };

    // For now, just return None (successful listen)
    Ok(Value::None)
}

/// socket.accept()
fn socket_accept(_args: Vec<Value>) -> Result<Value> {
    // For now, return a mock connection and address
    let conn = socket_socket(vec![Value::Int(2), Value::Int(1)])?;
    let addr = Value::Tuple(vec![Value::Str("127.0.0.1".to_string()), Value::Int(12345)]);
    
    Ok(Value::Tuple(vec![conn, addr]))
}

/// socket.connect(address)
fn socket_connect(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("connect() missing required argument: 'address'"));
    }

    // For now, just return None (successful connection)
    Ok(Value::None)
}

/// socket.send(data, flags=0)
fn socket_send(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("send() missing required argument: 'data'"));
    }

    let data = match &args[0] {
        Value::Str(s) => s.as_bytes(),
        Value::Bytes(b) => b,
        _ => return Err(anyhow!("data must be bytes or string")),
    };

    // Return the number of bytes "sent"
    Ok(Value::Int(data.len() as i64))
}

/// socket.recv(bufsize, flags=0)
fn socket_recv(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("recv() missing required argument: 'bufsize'"));
    }

    let _bufsize = match &args[0] {
        Value::Int(size) => *size,
        _ => return Err(anyhow!("bufsize must be an integer")),
    };

    // Return empty bytes for now
    Ok(Value::Bytes(vec![]))
}

/// socket.sendto(data, address)
fn socket_sendto(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("sendto() missing required arguments"));
    }

    let data = match &args[0] {
        Value::Str(s) => s.as_bytes(),
        Value::Bytes(b) => b,
        _ => return Err(anyhow!("data must be bytes or string")),
    };

    // Return the number of bytes "sent"
    Ok(Value::Int(data.len() as i64))
}

/// socket.recvfrom(bufsize, flags=0)
fn socket_recvfrom(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("recvfrom() missing required argument: 'bufsize'"));
    }

    let _bufsize = match &args[0] {
        Value::Int(size) => *size,
        _ => return Err(anyhow!("bufsize must be an integer")),
    };

    // Return empty data and mock address
    let data = Value::Bytes(vec![]);
    let addr = Value::Tuple(vec![Value::Str("127.0.0.1".to_string()), Value::Int(12345)]);
    
    Ok(Value::Tuple(vec![data, addr]))
}

/// socket.close()
fn socket_close(_args: Vec<Value>) -> Result<Value> {
    // Mark socket as closed
    Ok(Value::None)
}

/// socket.shutdown(how)
fn socket_shutdown(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("shutdown() missing required argument: 'how'"));
    }

    let _how = match &args[0] {
        Value::Int(h) => *h,
        _ => return Err(anyhow!("how must be an integer")),
    };

    Ok(Value::None)
}

/// socket.setsockopt(level, optname, value)
fn socket_setsockopt(args: Vec<Value>) -> Result<Value> {
    if args.len() < 3 {
        return Err(anyhow!("setsockopt() missing required arguments"));
    }

    Ok(Value::None)
}

/// socket.getsockopt(level, optname, buflen=0)
fn socket_getsockopt(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("getsockopt() missing required arguments"));
    }

    // Return a default value
    Ok(Value::Int(0))
}

/// socket.settimeout(value)
fn socket_settimeout(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("settimeout() missing required argument: 'value'"));
    }

    Ok(Value::None)
}

/// socket.gettimeout()
fn socket_gettimeout(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// socket.setblocking(flag)
fn socket_setblocking(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("setblocking() missing required argument: 'flag'"));
    }

    Ok(Value::None)
}

/// socket.getpeername()
fn socket_getpeername(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Tuple(vec![Value::Str("127.0.0.1".to_string()), Value::Int(12345)]))
}

/// socket.getsockname()
fn socket_getsockname(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Tuple(vec![Value::Str("127.0.0.1".to_string()), Value::Int(8080)]))
}

/// socket.gethostname()
fn socket_gethostname(_args: Vec<Value>) -> Result<Value> {
    match hostname::get() {
        Ok(name) => Ok(Value::Str(name.to_string_lossy().to_string())),
        Err(_) => Ok(Value::Str("localhost".to_string())),
    }
}

/// socket.gethostbyname(hostname)
fn socket_gethostbyname(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("gethostbyname() missing required argument: 'hostname'"));
    }

    let hostname = match &args[0] {
        Value::Str(h) => h,
        _ => return Err(anyhow!("hostname must be a string")),
    };

    // Simple implementation - return localhost for localhost, otherwise a mock IP
    if hostname == "localhost" || hostname == "127.0.0.1" {
        Ok(Value::Str("127.0.0.1".to_string()))
    } else {
        Ok(Value::Str("192.168.1.1".to_string()))
    }
}

/// socket.inet_aton(ip_string)
fn socket_inet_aton(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("inet_aton() missing required argument: 'ip_string'"));
    }

    let ip_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("ip_string must be a string")),
    };

    match ip_str.parse::<Ipv4Addr>() {
        Ok(addr) => {
            let octets = addr.octets();
            let packed = u32::from_be_bytes(octets);
            Ok(Value::Bytes(packed.to_be_bytes().to_vec()))
        }
        Err(_) => Err(anyhow!("invalid IP address")),
    }
}

/// socket.inet_ntoa(packed_ip)
fn socket_inet_ntoa(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("inet_ntoa() missing required argument: 'packed_ip'"));
    }

    let packed = match &args[0] {
        Value::Bytes(b) if b.len() == 4 => {
            let mut bytes = [0u8; 4];
            bytes.copy_from_slice(b);
            bytes
        }
        _ => return Err(anyhow!("packed_ip must be 4 bytes")),
    };

    let addr = Ipv4Addr::from(packed);
    Ok(Value::Str(addr.to_string()))
}

/// socket.htons(x)
fn socket_htons(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("htons() missing required argument: 'x'"));
    }

    let x = match &args[0] {
        Value::Int(n) => *n as u16,
        _ => return Err(anyhow!("x must be an integer")),
    };

    Ok(Value::Int(x.to_be() as i64))
}

/// socket.ntohs(x)
fn socket_ntohs(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("ntohs() missing required argument: 'x'"));
    }

    let x = match &args[0] {
        Value::Int(n) => *n as u16,
        _ => return Err(anyhow!("x must be an integer")),
    };

    Ok(Value::Int(u16::from_be(x) as i64))
}

/// socket.htonl(x)
fn socket_htonl(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("htonl() missing required argument: 'x'"));
    }

    let x = match &args[0] {
        Value::Int(n) => *n as u32,
        _ => return Err(anyhow!("x must be an integer")),
    };

    Ok(Value::Int(x.to_be() as i64))
}

/// socket.ntohl(x)
fn socket_ntohl(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("ntohl() missing required argument: 'x'"));
    }

    let x = match &args[0] {
        Value::Int(n) => *n as u32,
        _ => return Err(anyhow!("x must be an integer")),
    };

    Ok(Value::Int(u32::from_be(x) as i64))
}

/// socket.error exception
fn socket_error(args: Vec<Value>) -> Result<Value> {
    let message = if args.is_empty() {
        "Socket error".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => "Socket error".to_string(),
        }
    };

    let mut error = HashMap::new();
    error.insert("message".to_string(), Value::Str(message));
    error.insert("args".to_string(), Value::Tuple(args));

    Ok(Value::Object {
        class_name: "error".to_string(),
        fields: error,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("error".to_string(), vec!["Exception".to_string(), "BaseException".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["error".to_string(), "Exception".to_string(), "BaseException".to_string(), "object".to_string()]),
    })
}

/// socket.gaierror exception
fn socket_gaierror(args: Vec<Value>) -> Result<Value> {
    let message = if args.is_empty() {
        "Address-related error".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => "Address-related error".to_string(),
        }
    };

    let mut error = HashMap::new();
    error.insert("message".to_string(), Value::Str(message));
    error.insert("args".to_string(), Value::Tuple(args));

    Ok(Value::Object {
        class_name: "gaierror".to_string(),
        fields: error,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("gaierror".to_string(), vec!["error".to_string(), "Exception".to_string(), "BaseException".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["gaierror".to_string(), "error".to_string(), "Exception".to_string(), "BaseException".to_string(), "object".to_string()]),
    })
}

/// socket.herror exception
fn socket_herror(args: Vec<Value>) -> Result<Value> {
    let message = if args.is_empty() {
        "Host-related error".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => "Host-related error".to_string(),
        }
    };

    let mut error = HashMap::new();
    error.insert("message".to_string(), Value::Str(message));
    error.insert("args".to_string(), Value::Tuple(args));

    Ok(Value::Object {
        class_name: "herror".to_string(),
        fields: error,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("herror".to_string(), vec!["error".to_string(), "Exception".to_string(), "BaseException".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["herror".to_string(), "error".to_string(), "Exception".to_string(), "BaseException".to_string(), "object".to_string()]),
    })
}

/// socket.timeout exception
fn socket_timeout(args: Vec<Value>) -> Result<Value> {
    let message = if args.is_empty() {
        "Socket timeout".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => "Socket timeout".to_string(),
        }
    };

    let mut error = HashMap::new();
    error.insert("message".to_string(), Value::Str(message));
    error.insert("args".to_string(), Value::Tuple(args));

    Ok(Value::Object {
        class_name: "timeout".to_string(),
        fields: error,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("timeout".to_string(), vec!["error".to_string(), "Exception".to_string(), "BaseException".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["timeout".to_string(), "error".to_string(), "Exception".to_string(), "BaseException".to_string(), "object".to_string()]),
    })
}