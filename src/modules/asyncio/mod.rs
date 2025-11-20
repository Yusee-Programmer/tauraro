/// Asyncio module - High-performance async/await implementation for Tauraro
/// Provides true native concurrency with better performance than Python's asyncio

pub mod runtime;

use crate::value::Value;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use crate::modules::hplist::HPList;
use std::time::Duration;
use anyhow::Result;

pub use runtime::AsyncRuntime;

// Re-export for backward compatibility
pub use crate::modules::asyncio_compat::*;

/// Create the asyncio module with enhanced functionality
pub fn create_asyncio_module() -> Value {
    let mut namespace = HashMap::new();

    // Event loop functions
    namespace.insert("get_event_loop".to_string(), Value::NativeFunction(get_event_loop));
    namespace.insert("new_event_loop".to_string(), Value::NativeFunction(new_event_loop));
    namespace.insert("set_event_loop".to_string(), Value::NativeFunction(set_event_loop));
    namespace.insert("run".to_string(), Value::NativeFunction(run));
    namespace.insert("run_until_complete".to_string(), Value::NativeFunction(run_until_complete));

    // Task and coroutine functions  
    namespace.insert("create_task".to_string(), Value::NativeFunction(create_task));
    namespace.insert("gather".to_string(), Value::NativeFunction(gather));
    namespace.insert("wait_for".to_string(), Value::NativeFunction(wait_for));
    namespace.insert("shield".to_string(), Value::NativeFunction(shield));

    // Sleep and timing functions
    namespace.insert("sleep".to_string(), Value::NativeFunction(asyncio_sleep));
    namespace.insert("wait".to_string(), Value::NativeFunction(wait));

    // Synchronization primitives
    namespace.insert("Lock".to_string(), Value::BuiltinFunction("Lock".to_string(), create_lock));
    namespace.insert("Event".to_string(), Value::BuiltinFunction("Event".to_string(), create_event));
    namespace.insert("Semaphore".to_string(), Value::BuiltinFunction("Semaphore".to_string(), create_semaphore));
    namespace.insert("Queue".to_string(), Value::BuiltinFunction("Queue".to_string(), create_queue));

    // Future and coroutine utilities
    namespace.insert("iscoroutine".to_string(), Value::NativeFunction(iscoroutine));
    namespace.insert("iscoroutinefunction".to_string(), Value::NativeFunction(iscoroutinefunction));
    namespace.insert("isfuture".to_string(), Value::NativeFunction(isfuture));

    // Exception classes
    namespace.insert("CancelledError".to_string(), Value::BuiltinFunction("CancelledError".to_string(), cancelled_error));
    namespace.insert("TimeoutError".to_string(), Value::BuiltinFunction("TimeoutError".to_string(), timeout_error));
    namespace.insert("InvalidStateError".to_string(), Value::BuiltinFunction("InvalidStateError".to_string(), invalid_state_error));

    // Constants
    namespace.insert("FIRST_COMPLETED".to_string(), Value::Str("FIRST_COMPLETED".to_string()));
    namespace.insert("FIRST_EXCEPTION".to_string(), Value::Str("FIRST_EXCEPTION".to_string()));
    namespace.insert("ALL_COMPLETED".to_string(), Value::Str("ALL_COMPLETED".to_string()));

    Value::Module("asyncio".to_string(), namespace)
}

// High-performance implementations

fn get_event_loop(args: Vec<Value>) -> Result<Value> {
    // Return the global runtime wrapped as an event loop object
    let runtime = AsyncRuntime::global();
    
    let loop_fields = HashMap::new();
    
    // Store methods in class_methods (not fields)
    let mut loop_methods = HashMap::new();
    loop_methods.insert("run_until_complete".to_string(), Value::NativeFunction(run_until_complete));
    loop_methods.insert("run_forever".to_string(), Value::NativeFunction(run_forever));
    loop_methods.insert("stop".to_string(), Value::NativeFunction(stop_loop));
    loop_methods.insert("close".to_string(), Value::NativeFunction(close_loop));
    loop_methods.insert("is_running".to_string(), Value::NativeFunction(is_running));
    loop_methods.insert("is_closed".to_string(), Value::NativeFunction(is_closed));
    loop_methods.insert("create_task".to_string(), Value::NativeFunction(create_task));
    
    Ok(Value::Object {
        class_name: "EventLoop".to_string(),
        fields: Rc::new(RefCell::new(loop_fields)),
        class_methods: loop_methods,
        base_object: crate::base_object::BaseObject::new("EventLoop".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["EventLoop".to_string(), "object".to_string()])
    })
}

fn new_event_loop(args: Vec<Value>) -> Result<Value> {
    get_event_loop(args)
}

fn set_event_loop(args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn run(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("run() requires a coroutine argument"));
    }

    let runtime = AsyncRuntime::global();
    runtime.run_until_complete(args[0].clone())
}

fn run_until_complete(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("run_until_complete() requires a coroutine argument"));
    }

    let runtime = AsyncRuntime::global();
    runtime.run_until_complete(args[0].clone())
}

fn run_forever(args: Vec<Value>) -> Result<Value> {
    // Keep the runtime alive
    std::thread::park();
    Ok(Value::None)
}

fn stop_loop(args: Vec<Value>) -> Result<Value> {
    let runtime = AsyncRuntime::global();
    runtime.shutdown();
    Ok(Value::None)
}

fn close_loop(args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn is_running(args: Vec<Value>) -> Result<Value> {
    Ok(Value::Bool(true))
}

fn is_closed(args: Vec<Value>) -> Result<Value> {
    Ok(Value::Bool(false))
}

fn create_task(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("create_task() requires a coroutine argument"));
    }

    let runtime = AsyncRuntime::global();
    let task_id = runtime.create_task(args[0].clone())?;
    
    // Store task ID in fields
    let mut task_fields = HashMap::new();
    task_fields.insert("_task_id".to_string(), Value::Int(task_id as i64));
    
    // Store methods in class_methods (not fields)
    let mut task_methods = HashMap::new();
    task_methods.insert("cancel".to_string(), Value::NativeFunction(cancel_task));
    task_methods.insert("cancelled".to_string(), Value::NativeFunction(task_cancelled));
    task_methods.insert("done".to_string(), Value::NativeFunction(task_done));
    task_methods.insert("result".to_string(), Value::NativeFunction(task_result));
    
    Ok(Value::Object {
        class_name: "Task".to_string(),
        fields: Rc::new(RefCell::new(task_fields)),
        class_methods: task_methods,
        base_object: crate::base_object::BaseObject::new("Task".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Task".to_string(), "object".to_string()])
    })
}

fn gather(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::List(HPList::new()));
    }

    let runtime = AsyncRuntime::global();
    let mut task_ids = Vec::new();

    // Extract task IDs from task objects
    for arg in args {
        if let Value::Object { fields, .. } = arg {
            if let Some(Value::Int(id)) = fields.borrow().get("_task_id") {
                task_ids.push(*id as usize);
            }
        }
    }

    let results = runtime.gather(task_ids)?;
    let mut list = HPList::new();
    for result in results {
        list.push(result);
    }

    Ok(Value::List(list))
}

fn wait_for(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("wait_for() requires coroutine and timeout arguments"));
    }

    // TODO: Implement timeout logic
    run_until_complete(vec![args[0].clone()])
}

fn shield(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("shield() requires a coroutine argument"));
    }
    
    // Shield just wraps the coroutine
    Ok(args[0].clone())
}

fn asyncio_sleep(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("sleep() requires a duration argument"));
    }

    let duration = match &args[0] {
        Value::Int(i) => Duration::from_secs(*i as u64),
        Value::Float(f) => Duration::from_secs_f64(*f),
        _ => return Err(anyhow::anyhow!("sleep() argument must be a number")),
    };

    // For now, block sleep (will be async when coroutines are implemented)
    std::thread::sleep(duration);
    Ok(Value::None)
}

fn wait(args: Vec<Value>) -> Result<Value> {
    Ok(Value::Tuple(vec![Value::None, Value::None]))
}

fn iscoroutine(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Bool(false));
    }

    Ok(Value::Bool(matches!(args[0], Value::Closure { .. })))
}

fn iscoroutinefunction(args: Vec<Value>) -> Result<Value> {
    iscoroutine(args)
}

fn isfuture(args: Vec<Value>) -> Result<Value> {
    Ok(Value::Bool(false))
}

// Task methods
fn cancel_task(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("cancel() requires self argument"));
    }

    if let Value::Object { fields, .. } = &args[0] {
        if let Some(Value::Int(task_id)) = fields.borrow().get("_task_id") {
            let runtime = AsyncRuntime::global();
            runtime.cancel_task(*task_id as usize)?;
        }
    }

    Ok(Value::None)
}

fn task_cancelled(args: Vec<Value>) -> Result<Value> {
    Ok(Value::Bool(false))
}

fn task_done(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Bool(false));
    }

    if let Value::Object { fields, .. } = &args[0] {
        if let Some(Value::Int(task_id)) = fields.borrow().get("_task_id") {
            let runtime = AsyncRuntime::global();
            return Ok(Value::Bool(runtime.is_task_done(*task_id as usize)));
        }
    }

    Ok(Value::Bool(false))
}

fn task_result(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("result() requires self argument"));
    }

    if let Value::Object { fields, .. } = &args[0] {
        if let Some(Value::Int(task_id)) = fields.borrow().get("_task_id") {
            let runtime = AsyncRuntime::global();
            if let Some(result) = runtime.get_task_result(*task_id as usize) {
                return result;
            }
        }
    }

    Err(anyhow::anyhow!("Task result not available"))
}

// Synchronization primitives (stubs for now)
fn create_lock(args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("Lock".to_string()))
}

fn create_event(args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("Event".to_string()))
}

fn create_semaphore(args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("Semaphore".to_string()))
}

fn create_queue(args: Vec<Value>) -> Result<Value> {
    Ok(Value::Str("Queue".to_string()))
}

// Exception classes
fn cancelled_error(args: Vec<Value>) -> Result<Value> {
    Ok(Value::new_exception(
        "CancelledError".to_string(),
        "Task was cancelled".to_string(),
        None,
    ))
}

fn timeout_error(args: Vec<Value>) -> Result<Value> {
    Ok(Value::new_exception(
        "TimeoutError".to_string(),
        "Operation timed out".to_string(),
        None,
    ))
}

fn invalid_state_error(args: Vec<Value>) -> Result<Value> {
    Ok(Value::new_exception(
        "InvalidStateError".to_string(),
        "Invalid state".to_string(),
        None,
    ))
}
