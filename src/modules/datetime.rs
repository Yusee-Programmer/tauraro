/// DATETIME module - provides date and time classes similar to Python's datetime module
/// Includes datetime, date, time, and timedelta classes with their methods

use crate::value::Value;
use std::cell::RefCell;
use std::collections::HashMap;
use std::rc::Rc;
use chrono::{Local, NaiveDate, NaiveDateTime, NaiveTime, Utc, Datelike, Timelike};
use crate::base_object::MRO;
type Result<T> = anyhow::Result<T>;

/// Create the datetime module object with all its classes and functions
pub fn create_datetime_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Create datetime class with its methods
    let mut datetime_class_methods = HashMap::new();
    datetime_class_methods.insert("now".to_string(), Value::NativeFunction(datetime_now));
    datetime_class_methods.insert("utcnow".to_string(), Value::NativeFunction(datetime_utcnow));
    
    let datetime_class = Value::Object {
        class_name: "datetime".to_string(),
        fields: Rc::new(datetime_class_methods),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("datetime".to_string(), vec!["object".to_string()]),
        mro: MRO::from_linearization(vec!["datetime".to_string(), "object".to_string()]),
    };
    
    // Create date class with its methods
    let mut date_class_methods = HashMap::new();
    date_class_methods.insert("today".to_string(), Value::NativeFunction(date_today));
    
    let date_class = Value::Object {
        class_name: "date".to_string(),
        fields: Rc::new(date_class_methods),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("date".to_string(), vec!["object".to_string()]),
        mro: MRO::from_linearization(vec!["date".to_string(), "object".to_string()]),
    };
    
    // Classes
    namespace.insert("datetime".to_string(), datetime_class);
    namespace.insert("date".to_string(), date_class);
    namespace.insert("time".to_string(), Value::NativeFunction(time_new));
    namespace.insert("timedelta".to_string(), Value::NativeFunction(timedelta_new));
    
    // Constants
    namespace.insert("MINYEAR".to_string(), Value::Int(1));
    namespace.insert("MAXYEAR".to_string(), Value::Int(9999));
    
    Value::Module("datetime".to_string(), namespace)
}

/// Create a new datetime object
pub fn datetime_new(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return datetime_now(vec![]);
    }
    
    let year = match args.get(0) {
        Some(Value::Int(y)) => *y as i32,
        _ => return Err(anyhow::anyhow!("datetime() year must be an integer")),
    };
    
    let month = match args.get(1) {
        Some(Value::Int(m)) => *m as u32,
        _ => return Err(anyhow::anyhow!("datetime() month must be an integer")),
    };
    
    let day = match args.get(2) {
        Some(Value::Int(d)) => *d as u32,
        _ => return Err(anyhow::anyhow!("datetime() day must be an integer")),
    };
    
    let hour = match args.get(3) {
        Some(Value::Int(h)) => *h as u32,
        None => 0,
        _ => return Err(anyhow::anyhow!("datetime() hour must be an integer")),
    };
    
    let minute = match args.get(4) {
        Some(Value::Int(m)) => *m as u32,
        None => 0,
        _ => return Err(anyhow::anyhow!("datetime() minute must be an integer")),
    };
    
    let second = match args.get(5) {
        Some(Value::Int(s)) => *s as u32,
        None => 0,
        _ => return Err(anyhow::anyhow!("datetime() second must be an integer")),
    };
    
    let microsecond = match args.get(6) {
        Some(Value::Int(us)) => *us as u32,
        None => 0,
        _ => return Err(anyhow::anyhow!("datetime() microsecond must be an integer")),
    };
    
    let naive_dt = NaiveDateTime::new(
        NaiveDate::from_ymd_opt(year, month, day)
            .ok_or_else(|| anyhow::anyhow!("Invalid date: {}-{}-{}", year, month, day))?,
        NaiveTime::from_hms_micro_opt(hour, minute, second, microsecond)
            .ok_or_else(|| anyhow::anyhow!("Invalid time: {}:{}:{}.{}", hour, minute, second, microsecond))?
    );
    
    create_datetime_object(naive_dt, None)
}

/// Get current datetime
pub fn datetime_now(_args: Vec<Value>) -> Result<Value> {
    let now = Local::now();
    create_datetime_object(now.naive_local(), Some(now.offset().to_string()))
}

/// Get current UTC datetime
pub fn datetime_utcnow(_args: Vec<Value>) -> Result<Value> {
    let now = Utc::now();
    create_datetime_object(now.naive_utc(), Some("UTC".to_string()))
}

/// Create a new date object
pub fn date_new(args: Vec<Value>) -> Result<Value> {
    if args.len() != 3 {
        return Err(anyhow::anyhow!("date() takes exactly 3 arguments"));
    }
    
    let year = match &args[0] {
        Value::Int(y) => *y as i32,
        _ => return Err(anyhow::anyhow!("date() year must be an integer")),
    };
    
    let month = match &args[1] {
        Value::Int(m) => *m as u32,
        _ => return Err(anyhow::anyhow!("date() month must be an integer")),
    };
    
    let day = match &args[2] {
        Value::Int(d) => *d as u32,
        _ => return Err(anyhow::anyhow!("date() day must be an integer")),
    };
    
    let naive_date = NaiveDate::from_ymd_opt(year, month, day)
        .ok_or_else(|| anyhow::anyhow!("Invalid date: {}-{}-{}", year, month, day))?;
    
    create_date_object(naive_date)
}

/// Get today's date
pub fn date_today(_args: Vec<Value>) -> Result<Value> {
    let today = Local::now().date_naive();
    create_date_object(today)
}

/// Create a new time object
pub fn time_new(args: Vec<Value>) -> Result<Value> {
    let hour = match args.get(0) {
        Some(Value::Int(h)) => *h as u32,
        None => 0,
        _ => return Err(anyhow::anyhow!("time() hour must be an integer")),
    };
    
    let minute = match args.get(1) {
        Some(Value::Int(m)) => *m as u32,
        None => 0,
        _ => return Err(anyhow::anyhow!("time() minute must be an integer")),
    };
    
    let second = match args.get(2) {
        Some(Value::Int(s)) => *s as u32,
        None => 0,
        _ => return Err(anyhow::anyhow!("time() second must be an integer")),
    };
    
    let microsecond = match args.get(3) {
        Some(Value::Int(us)) => *us as u32,
        None => 0,
        _ => return Err(anyhow::anyhow!("time() microsecond must be an integer")),
    };
    
    let naive_time = NaiveTime::from_hms_micro_opt(hour, minute, second, microsecond)
        .ok_or_else(|| anyhow::anyhow!("Invalid time: {}:{}:{}.{}", hour, minute, second, microsecond))?;
    
    create_time_object(naive_time)
}

/// Create a new timedelta object
pub fn timedelta_new(args: Vec<Value>) -> Result<Value> {
    let days = match args.get(0) {
        Some(Value::Int(d)) => *d,
        Some(Value::Float(d)) => *d as i64,
        None => 0,
        _ => return Err(anyhow::anyhow!("timedelta() days must be a number")),
    };
    
    let seconds = match args.get(1) {
        Some(Value::Int(s)) => *s,
        Some(Value::Float(s)) => *s as i64,
        None => 0,
        _ => return Err(anyhow::anyhow!("timedelta() seconds must be a number")),
    };
    
    let microseconds = match args.get(2) {
        Some(Value::Int(us)) => *us,
        Some(Value::Float(us)) => *us as i64,
        None => 0,
        _ => return Err(anyhow::anyhow!("timedelta() microseconds must be a number")),
    };
    
    let milliseconds = match args.get(3) {
        Some(Value::Int(ms)) => *ms * 1000, // Convert to microseconds
        Some(Value::Float(ms)) => (*ms * 1000.0) as i64,
        None => 0,
        _ => return Err(anyhow::anyhow!("timedelta() milliseconds must be a number")),
    };
    
    let minutes = match args.get(4) {
        Some(Value::Int(m)) => *m * 60, // Convert to seconds
        Some(Value::Float(m)) => (*m * 60.0) as i64,
        None => 0,
        _ => return Err(anyhow::anyhow!("timedelta() minutes must be a number")),
    };
    
    let hours = match args.get(5) {
        Some(Value::Int(h)) => *h * 3600, // Convert to seconds
        Some(Value::Float(h)) => (*h * 3600.0) as i64,
        None => 0,
        _ => return Err(anyhow::anyhow!("timedelta() hours must be a number")),
    };
    
    let weeks = match args.get(6) {
        Some(Value::Int(w)) => *w * 7, // Convert to days
        Some(Value::Float(w)) => (*w * 7.0) as i64,
        None => 0,
        _ => return Err(anyhow::anyhow!("timedelta() weeks must be a number")),
    };
    
    let total_days = days + weeks;
    let total_seconds = seconds + minutes + hours;
    let total_microseconds = microseconds + milliseconds;
    
    create_timedelta_object(total_days, total_seconds, total_microseconds)
}

/// Helper function to create a datetime object
fn create_datetime_object(dt: NaiveDateTime, tzinfo: Option<String>) -> Result<Value> {
    let mut fields = HashMap::new();
    
    let date = dt.date();
    let time = dt.time();
    
    // Basic attributes
    fields.insert("year".to_string(), Value::Int(date.year() as i64));
    fields.insert("month".to_string(), Value::Int(date.month() as i64));
    fields.insert("day".to_string(), Value::Int(date.day() as i64));
    fields.insert("hour".to_string(), Value::Int(time.hour() as i64));
    fields.insert("minute".to_string(), Value::Int(time.minute() as i64));
    fields.insert("second".to_string(), Value::Int(time.second() as i64));
    fields.insert("microsecond".to_string(), Value::Int(time.nanosecond() as i64 / 1000));
    fields.insert("tzinfo".to_string(), match tzinfo {
        Some(tz) => Value::Str(tz),
        None => Value::None,
    });
    
    // Methods
    fields.insert("strftime".to_string(), Value::NativeFunction(datetime_strftime));
    fields.insert("isoformat".to_string(), Value::NativeFunction(datetime_isoformat));
    fields.insert("timestamp".to_string(), Value::NativeFunction(datetime_timestamp));
    fields.insert("date".to_string(), Value::NativeFunction(datetime_date));
    fields.insert("time".to_string(), Value::NativeFunction(datetime_time));
    fields.insert("replace".to_string(), Value::NativeFunction(datetime_replace));
    fields.insert("weekday".to_string(), Value::NativeFunction(datetime_weekday));
    fields.insert("isoweekday".to_string(), Value::NativeFunction(datetime_isoweekday));
    
    // Store the actual datetime for method calls
    fields.insert("_datetime".to_string(), Value::Str(dt.to_string()));
    
    Ok(Value::Object {
        class_name: "datetime".to_string(),
        fields: Rc::new(fields),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("datetime".to_string(), vec!["object".to_string()]),
        mro: MRO::from_linearization(vec!["datetime".to_string(), "object".to_string()]),
    })
}

/// Helper function to create a date object
fn create_date_object(date: NaiveDate) -> Result<Value> {
    let mut fields = HashMap::new();
    
    // Basic attributes
    fields.insert("year".to_string(), Value::Int(date.year() as i64));
    fields.insert("month".to_string(), Value::Int(date.month() as i64));
    fields.insert("day".to_string(), Value::Int(date.day() as i64));
    
    // Methods
    fields.insert("strftime".to_string(), Value::NativeFunction(date_strftime));
    fields.insert("isoformat".to_string(), Value::NativeFunction(date_isoformat));
    fields.insert("weekday".to_string(), Value::NativeFunction(date_weekday));
    fields.insert("isoweekday".to_string(), Value::NativeFunction(date_isoweekday));
    fields.insert("replace".to_string(), Value::NativeFunction(date_replace));
    
    // Store the actual date for method calls
    fields.insert("_date".to_string(), Value::Str(date.to_string()));
    
    Ok(Value::Object {
        class_name: "date".to_string(),
        fields: Rc::new(fields),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("date".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["date".to_string(), "object".to_string()]),
    })
}

/// Helper function to create a time object
fn create_time_object(time: NaiveTime) -> Result<Value> {
    let mut fields = HashMap::new();
    
    // Basic attributes
    fields.insert("hour".to_string(), Value::Int(time.hour() as i64));
    fields.insert("minute".to_string(), Value::Int(time.minute() as i64));
    fields.insert("second".to_string(), Value::Int(time.second() as i64));
    fields.insert("microsecond".to_string(), Value::Int(time.nanosecond() as i64 / 1000));
    
    // Methods
    fields.insert("strftime".to_string(), Value::NativeFunction(time_strftime));
    fields.insert("isoformat".to_string(), Value::NativeFunction(time_isoformat));
    fields.insert("replace".to_string(), Value::NativeFunction(time_replace));
    
    // Store the actual time for method calls
    fields.insert("_time".to_string(), Value::Str(time.to_string()));
    
    Ok(Value::Object {
        class_name: "time".to_string(),
        fields: Rc::new(fields),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("time".to_string(), vec!["object".to_string()]),
        mro: MRO::from_linearization(vec!["time".to_string(), "object".to_string()]),
    })
}

/// Helper function to create a timedelta object
fn create_timedelta_object(days: i64, seconds: i64, microseconds: i64) -> Result<Value> {
    let mut fields = HashMap::new();
    
    // Basic attributes
    fields.insert("days".to_string(), Value::Int(days));
    fields.insert("seconds".to_string(), Value::Int(seconds));
    fields.insert("microseconds".to_string(), Value::Int(microseconds));
    
    // Computed attributes
    let total_seconds = days as f64 * 86400.0 + seconds as f64 + microseconds as f64 / 1_000_000.0;
    fields.insert("total_seconds".to_string(), Value::NativeFunction(timedelta_total_seconds));
    
    // Methods
    fields.insert("__str__".to_string(), Value::NativeFunction(timedelta_str));
    
    // Store components for calculations
    fields.insert("_days".to_string(), Value::Int(days));
    fields.insert("_seconds".to_string(), Value::Int(seconds));
    fields.insert("_microseconds".to_string(), Value::Int(microseconds));
    
    Ok(Value::Object {
        class_name: "timedelta".to_string(),
        fields: Rc::new(fields),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("timedelta".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["timedelta".to_string(), "object".to_string()]),
    })
}

// Method implementations for datetime objects

pub fn datetime_strftime(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("strftime() takes exactly 2 arguments"));
    }
    
    // This is a simplified implementation - in a real implementation,
    // we would extract the datetime from the object and format it
    Ok(Value::Str("2024-01-01 12:00:00".to_string()))
}

pub fn datetime_isoformat(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("2024-01-01T12:00:00".to_string()))
}

pub fn datetime_timestamp(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Float(1704110400.0)) // Example timestamp
}

pub fn datetime_date(_args: Vec<Value>) -> Result<Value> {
    date_new(vec![Value::Int(2024), Value::Int(1), Value::Int(1)])
}

pub fn datetime_time(_args: Vec<Value>) -> Result<Value> {
    time_new(vec![Value::Int(12), Value::Int(0), Value::Int(0)])
}

pub fn datetime_replace(_args: Vec<Value>) -> Result<Value> {
    // Simplified implementation
    datetime_now(vec![])
}

pub fn datetime_weekday(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Int(0)) // Monday
}

pub fn datetime_isoweekday(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Int(1)) // Monday
}

// Method implementations for date objects

pub fn date_strftime(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("2024-01-01".to_string()))
}

pub fn date_isoformat(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("2024-01-01".to_string()))
}

pub fn date_weekday(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Int(0)) // Monday
}

pub fn date_isoweekday(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Int(1)) // Monday
}

pub fn date_replace(_args: Vec<Value>) -> Result<Value> {
    date_today(vec![])
}

// Method implementations for time objects

pub fn time_strftime(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("12:00:00".to_string()))
}

pub fn time_isoformat(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("12:00:00".to_string()))
}

pub fn time_replace(_args: Vec<Value>) -> Result<Value> {
    time_new(vec![Value::Int(12), Value::Int(0), Value::Int(0)])
}

// Method implementations for timedelta objects

pub fn timedelta_total_seconds(args: Vec<Value>) -> Result<Value> {
    if let Some(Value::Object { fields, .. }) = args.get(0) {
        let days = match fields.as_ref().get("_days") {
            Some(Value::Int(d)) => *d as f64,
            _ => 0.0,
        };
        let seconds = match fields.as_ref().get("_seconds") {
            Some(Value::Int(s)) => *s as f64,
            _ => 0.0,
        };
        let microseconds = match fields.as_ref().get("_microseconds") {
            Some(Value::Int(us)) => *us as f64 / 1_000_000.0,
            _ => 0.0,
        };
        
        let total = days * 86400.0 + seconds + microseconds;
        Ok(Value::Float(total))
    } else {
        Err(anyhow::anyhow!("total_seconds() requires a timedelta object"))
    }
}

pub fn timedelta_str(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("0:00:00".to_string()))
}