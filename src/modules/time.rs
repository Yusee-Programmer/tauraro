/// TIME module - provides time-related functions similar to Python's time module
/// Includes functions for time measurement, formatting, and sleeping

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::thread;
use std::time::{Duration, SystemTime, UNIX_EPOCH};
use chrono::{DateTime, Local, Utc, TimeZone, NaiveDateTime, Timelike, Datelike};

/// Create the time module object with all its functions and constants
pub fn create_time_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Core time functions
    namespace.insert("time".to_string(), Value::NativeFunction(time_time));
    namespace.insert("sleep".to_string(), Value::NativeFunction(time_sleep));
    namespace.insert("perf_counter".to_string(), Value::NativeFunction(time_perf_counter));
    namespace.insert("process_time".to_string(), Value::NativeFunction(time_process_time));
    namespace.insert("monotonic".to_string(), Value::NativeFunction(time_monotonic));
    
    // Time formatting functions
    namespace.insert("strftime".to_string(), Value::NativeFunction(time_strftime));
    namespace.insert("strptime".to_string(), Value::NativeFunction(time_strptime));
    namespace.insert("asctime".to_string(), Value::NativeFunction(time_asctime));
    namespace.insert("ctime".to_string(), Value::NativeFunction(time_ctime));
    
    // Time conversion functions
    namespace.insert("gmtime".to_string(), Value::NativeFunction(time_gmtime));
    namespace.insert("localtime".to_string(), Value::NativeFunction(time_localtime));
    namespace.insert("mktime".to_string(), Value::NativeFunction(time_mktime));
    
    // Time zone functions
    namespace.insert("timezone".to_string(), Value::NativeFunction(time_timezone));
    namespace.insert("tzname".to_string(), Value::NativeFunction(time_tzname));
    namespace.insert("daylight".to_string(), Value::NativeFunction(time_daylight));
    
    // Constants
    namespace.insert("altzone".to_string(), Value::Int(0)); // Simplified
    
    Value::Module("time".to_string(), namespace)
}

/// Get current time as seconds since Unix epoch
pub fn time_time(_args: Vec<Value>) -> Result<Value> {
    let now = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map_err(|e| anyhow::anyhow!("System time error: {}", e))?;
    
    Ok(Value::Float(now.as_secs_f64()))
}

/// Sleep for the given number of seconds
pub fn time_sleep(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("sleep() takes exactly one argument ({} given)", args.len()));
    }
    
    let seconds = match &args[0] {
        Value::Int(i) => *i as f64,
        Value::Float(f) => *f,
        _ => return Err(anyhow::anyhow!("sleep() argument must be a number")),
    };
    
    if seconds < 0.0 {
        return Err(anyhow::anyhow!("sleep length must be non-negative"));
    }
    
    let duration = Duration::from_secs_f64(seconds);
    thread::sleep(duration);
    
    Ok(Value::None)
}

/// High-resolution performance counter
pub fn time_perf_counter(_args: Vec<Value>) -> Result<Value> {
    use std::time::Instant;
    static START_TIME: std::sync::OnceLock<Instant> = std::sync::OnceLock::new();
    
    let start = START_TIME.get_or_init(|| Instant::now());
    let elapsed = start.elapsed();
    
    Ok(Value::Float(elapsed.as_secs_f64()))
}

/// Process time (simplified - returns same as perf_counter for now)
pub fn time_process_time(_args: Vec<Value>) -> Result<Value> {
    time_perf_counter(_args)
}

/// Monotonic time (simplified - returns same as perf_counter for now)
pub fn time_monotonic(_args: Vec<Value>) -> Result<Value> {
    time_perf_counter(_args)
}

/// Format time using strftime format
pub fn time_strftime(args: Vec<Value>) -> Result<Value> {
    let format_str = match args.get(0) {
        Some(Value::Str(s)) => s,
        _ => return Err(anyhow::anyhow!("strftime() requires a format string")),
    };
    
    let timestamp = match args.get(1) {
        Some(Value::Float(f)) => *f,
        Some(Value::Int(i)) => *i as f64,
        Some(Value::Dict(dict)) => {
            // Handle struct_time object (dict representation)
            // Extract timestamp from the struct_time fields
            if let Some(Value::Int(year)) = dict.get("tm_year") {
                if let Some(Value::Int(mon)) = dict.get("tm_mon") {
                    if let Some(Value::Int(mday)) = dict.get("tm_mday") {
                        if let Some(Value::Int(hour)) = dict.get("tm_hour") {
                            if let Some(Value::Int(min)) = dict.get("tm_min") {
                                if let Some(Value::Int(sec)) = dict.get("tm_sec") {
                                    // Create a NaiveDateTime from the struct_time fields
                                    if let Some(naive_dt) = chrono::NaiveDate::from_ymd_opt(*year as i32, *mon as u32, *mday as u32)
                                        .and_then(|d| d.and_hms_opt(*hour as u32, *min as u32, *sec as u32)) {
                                        let dt = Local.from_local_datetime(&naive_dt).single()
                                            .unwrap_or_else(|| Local.from_utc_datetime(&naive_dt));
                                        let formatted = dt.format(format_str).to_string();
                                        return Ok(Value::Str(formatted));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return Err(anyhow::anyhow!("Invalid struct_time object"));
        }
        None => {
            // Use current time if no timestamp provided
            SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .map_err(|e| anyhow::anyhow!("System time error: {}", e))?
                .as_secs_f64()
        }
        _ => return Err(anyhow::anyhow!("strftime() timestamp must be a number or struct_time")),
    };
    
    let dt = Utc.timestamp_opt(timestamp as i64, (timestamp.fract() * 1_000_000_000.0) as u32)
        .single()
        .ok_or_else(|| anyhow::anyhow!("Invalid timestamp"))?;
    
    let formatted = dt.format(format_str).to_string();
    Ok(Value::Str(formatted))
}

/// Parse time string (simplified implementation)
pub fn time_strptime(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("strptime() takes exactly 2 arguments"));
    }
    
    let _time_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("strptime() first argument must be a string")),
    };
    
    let _format_str = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("strptime() second argument must be a string")),
    };
    
    // Simplified: return current time struct
    time_localtime(vec![])
}

/// Convert time to ASCII string
pub fn time_asctime(args: Vec<Value>) -> Result<Value> {
    let timestamp = match args.get(0) {
        Some(Value::Float(f)) => *f,
        Some(Value::Int(i)) => *i as f64,
        Some(Value::Dict(dict)) => {
            // Handle struct_time object (dict representation)
            // Extract timestamp from the struct_time fields
            if let Some(Value::Int(year)) = dict.get("tm_year") {
                if let Some(Value::Int(mon)) = dict.get("tm_mon") {
                    if let Some(Value::Int(mday)) = dict.get("tm_mday") {
                        if let Some(Value::Int(hour)) = dict.get("tm_hour") {
                            if let Some(Value::Int(min)) = dict.get("tm_min") {
                                if let Some(Value::Int(sec)) = dict.get("tm_sec") {
                                    // Create a NaiveDateTime from the struct_time fields
                                    if let Some(naive_dt) = chrono::NaiveDate::from_ymd_opt(*year as i32, *mon as u32, *mday as u32)
                                        .and_then(|d| d.and_hms_opt(*hour as u32, *min as u32, *sec as u32)) {
                                        let dt = Local.from_local_datetime(&naive_dt).single()
                                            .unwrap_or_else(|| Local.from_utc_datetime(&naive_dt));
                                        let formatted = dt.format("%a %b %d %H:%M:%S %Y").to_string();
                                        return Ok(Value::Str(formatted));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return Err(anyhow::anyhow!("Invalid struct_time object"));
        }
        None => {
            SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .map_err(|e| anyhow::anyhow!("System time error: {}", e))?
                .as_secs_f64()
        }
        _ => return Err(anyhow::anyhow!("asctime() argument must be a number or time struct")),
    };
    
    let dt = Local.timestamp_opt(timestamp as i64, 0)
        .single()
        .ok_or_else(|| anyhow::anyhow!("Invalid timestamp"))?;
    
    let formatted = dt.format("%a %b %d %H:%M:%S %Y").to_string();
    Ok(Value::Str(formatted))
}

/// Convert timestamp to string
pub fn time_ctime(args: Vec<Value>) -> Result<Value> {
    time_asctime(args)
}

/// Convert timestamp to UTC time struct
pub fn time_gmtime(args: Vec<Value>) -> Result<Value> {
    let timestamp = match args.get(0) {
        Some(Value::Float(f)) => *f,
        Some(Value::Int(i)) => *i as f64,
        None => {
            SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .map_err(|e| anyhow::anyhow!("System time error: {}", e))?
                .as_secs_f64()
        }
        _ => return Err(anyhow::anyhow!("gmtime() argument must be a number")),
    };
    
    let dt = Utc.timestamp_opt(timestamp as i64, (timestamp.fract() * 1_000_000_000.0) as u32)
        .single()
        .ok_or_else(|| anyhow::anyhow!("Invalid timestamp"))?;
    
    // Return a time struct as a tuple (year, month, day, hour, minute, second, weekday, yearday, dst)
    let date = dt.date_naive();
    let time = dt.time();
    let mut time_struct = HashMap::new();
    time_struct.insert("tm_year".to_string(), Value::Int(date.year() as i64));
    time_struct.insert("tm_mon".to_string(), Value::Int(date.month() as i64 - 1)); // 0-based
    time_struct.insert("tm_mday".to_string(), Value::Int(date.day() as i64));
    time_struct.insert("tm_hour".to_string(), Value::Int(time.hour() as i64));
    time_struct.insert("tm_min".to_string(), Value::Int(time.minute() as i64));
    time_struct.insert("tm_sec".to_string(), Value::Int(time.second() as i64));
    time_struct.insert("tm_wday".to_string(), Value::Int(dt.weekday().num_days_from_sunday() as i64));
    time_struct.insert("tm_yday".to_string(), Value::Int(date.ordinal() as i64 - 1)); // 0-based
    time_struct.insert("tm_isdst".to_string(), Value::Int(0)); // UTC has no DST
    
    Ok(Value::Object {
        class_name: "struct_time".to_string(),
        fields: time_struct,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("struct_time".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["struct_time".to_string(), "object".to_string()]),
    })
}

/// Convert timestamp to local time struct
pub fn time_localtime(args: Vec<Value>) -> Result<Value> {
    let timestamp = match args.get(0) {
        Some(Value::Float(f)) => *f,
        Some(Value::Int(i)) => *i as f64,
        None => {
            SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .map_err(|e| anyhow::anyhow!("System time error: {}", e))?
                .as_secs_f64()
        }
        _ => return Err(anyhow::anyhow!("localtime() argument must be a number")),
    };
    
    let dt = Local.timestamp_opt(timestamp as i64, 0)
        .single()
        .ok_or_else(|| anyhow::anyhow!("Invalid timestamp"))?;
    
    // Return a time struct as an object
    let date = dt.date_naive();
    let time = dt.time();
    let mut time_struct = HashMap::new();
    time_struct.insert("tm_year".to_string(), Value::Int(date.year() as i64));
    time_struct.insert("tm_mon".to_string(), Value::Int(date.month() as i64));
    time_struct.insert("tm_mday".to_string(), Value::Int(date.day() as i64));
    time_struct.insert("tm_hour".to_string(), Value::Int(time.hour() as i64));
    time_struct.insert("tm_min".to_string(), Value::Int(time.minute() as i64));
    time_struct.insert("tm_sec".to_string(), Value::Int(time.second() as i64));
    time_struct.insert("tm_wday".to_string(), Value::Int(dt.weekday().num_days_from_sunday() as i64));
    time_struct.insert("tm_yday".to_string(), Value::Int(date.ordinal() as i64));
    time_struct.insert("tm_isdst".to_string(), Value::Int(-1)); // Unknown DST status
    
    Ok(Value::Dict(time_struct))
}

/// Convert time struct to timestamp
pub fn time_mktime(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("mktime() takes exactly one argument"));
    }
    
    let time_struct = match &args[0] {
        Value::Dict(fields) => fields,
        _ => return Err(anyhow::anyhow!("mktime() argument must be a time struct")),
    };
    
    let year = match time_struct.get("tm_year") {
        Some(Value::Int(y)) => *y as i32,
        _ => return Err(anyhow::anyhow!("Invalid time struct: missing tm_year")),
    };
    
    let month = match time_struct.get("tm_mon") {
        Some(Value::Int(m)) => *m as u32,
        _ => return Err(anyhow::anyhow!("Invalid time struct: missing tm_mon")),
    };
    
    let day = match time_struct.get("tm_mday") {
        Some(Value::Int(d)) => *d as u32,
        _ => return Err(anyhow::anyhow!("Invalid time struct: missing tm_mday")),
    };
    
    let hour = match time_struct.get("tm_hour") {
        Some(Value::Int(h)) => *h as u32,
        _ => return Err(anyhow::anyhow!("Invalid time struct: missing tm_hour")),
    };
    
    let minute = match time_struct.get("tm_min") {
        Some(Value::Int(m)) => *m as u32,
        _ => return Err(anyhow::anyhow!("Invalid time struct: missing tm_min")),
    };
    
    let second = match time_struct.get("tm_sec") {
        Some(Value::Int(s)) => *s as u32,
        _ => return Err(anyhow::anyhow!("Invalid time struct: missing tm_sec")),
    };
    
    let naive_dt = NaiveDateTime::new(
        chrono::NaiveDate::from_ymd_opt(year, month, day)
            .ok_or_else(|| anyhow::anyhow!("Invalid date"))?,
        chrono::NaiveTime::from_hms_opt(hour, minute, second)
            .ok_or_else(|| anyhow::anyhow!("Invalid time"))?
    );
    
    let local_dt = Local.from_local_datetime(&naive_dt)
        .single()
        .ok_or_else(|| anyhow::anyhow!("Ambiguous local time"))?;
    
    Ok(Value::Float(local_dt.timestamp() as f64))
}

/// Get timezone offset
pub fn time_timezone(_args: Vec<Value>) -> Result<Value> {
    let local_offset = Local::now().offset().local_minus_utc();
    Ok(Value::Int(-local_offset as i64)) // Python uses negative offset
}

/// Get timezone names
pub fn time_tzname(_args: Vec<Value>) -> Result<Value> {
    // Simplified: return generic timezone names
    Ok(Value::Tuple(vec![
        Value::Str("UTC".to_string()),
        Value::Str("UTC".to_string()),
    ]))
}

/// Check if daylight saving time is in effect
pub fn time_daylight(_args: Vec<Value>) -> Result<Value> {
    // Simplified: always return 0 (no DST info available)
    Ok(Value::Int(0))
}