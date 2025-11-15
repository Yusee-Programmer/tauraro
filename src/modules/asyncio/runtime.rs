/// High-Performance Async Runtime for Tauraro
/// Provides async/await with better ergonomics than Python
/// 
/// Architecture: Uses Tokio for I/O but keeps coroutine execution on calling thread
/// This avoids Send/Sync issues with Rc<RefCell<>> in Value while still providing
/// true async I/O concurrency.

use std::rc::Rc;
use std::cell::RefCell;
use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering};
use std::collections::HashMap;
use std::time::Duration;
use tokio::runtime::{Runtime as TokioRuntime, Builder};
use crate::value::Value;
use anyhow::Result;

/// Task ID for tracking async tasks
type TaskId = usize;

/// Task status
#[derive(Clone, PartialEq, Debug)]
enum TaskStatus {
    Pending,
    Running,
    Completed,
    Cancelled,
}

/// Task information
struct TaskInfo {
    id: TaskId,
    status: TaskStatus,
    coroutine: Value,
    result: Option<Result<Value>>,
}

/// High-performance async runtime (thread-local)
/// Uses Tokio for I/O operations but keeps coroutine execution on the calling thread.
/// This design avoids Send/Sync constraints while providing true async I/O.
pub struct AsyncRuntime {
    /// Tokio runtime for I/O operations
    tokio_runtime: Rc<TokioRuntime>,
    
    /// Task registry (thread-local storage)
    tasks: Rc<RefCell<HashMap<TaskId, TaskInfo>>>,
    
    /// Next task ID
    next_task_id: Rc<AtomicUsize>,
    
    /// Runtime state
    is_running: Rc<AtomicBool>,
}

impl AsyncRuntime {
    /// Create a new async runtime with default configuration
    pub fn new() -> Result<Self> {
        // Use current_thread runtime since we're using thread-local storage
        // This avoids background threads that prevent clean shutdown
        let tokio_runtime = Builder::new_current_thread()
            .enable_all()
            .build()?;

        Ok(AsyncRuntime {
            tokio_runtime: Rc::new(tokio_runtime),
            tasks: Rc::new(RefCell::new(HashMap::new())),
            next_task_id: Rc::new(AtomicUsize::new(1)),
            is_running: Rc::new(AtomicBool::new(false)),
        })
    }

    /// Get the global runtime instance (thread-local)
    pub fn global() -> std::rc::Rc<Self> {
        thread_local! {
            static RUNTIME: std::rc::Rc<AsyncRuntime> = std::rc::Rc::new(
                AsyncRuntime::new().expect("Failed to create async runtime")
            );
        }
        RUNTIME.with(|r| r.clone())
    }

    /// Create a new task from a coroutine
    /// Returns a task ID that can be used to query status or cancel
    pub fn create_task(&self, coroutine: Value) -> Result<TaskId> {
        let task_id = self.next_task_id.fetch_add(1, Ordering::SeqCst);
        
        let task_info = TaskInfo {
            id: task_id,
            status: TaskStatus::Pending,
            coroutine: coroutine.clone(),
            result: None,
        };

        self.tasks.borrow_mut().insert(task_id, task_info);

        // Note: Actual execution happens when the event loop runs
        // This just registers the task
        
        Ok(task_id)
    }

    /// Run a coroutine until it completes (blocking)
    pub fn run_until_complete(&self, coroutine: Value) -> Result<Value> {
        self.is_running.store(true, Ordering::SeqCst);
        
        // Execute the coroutine directly on this thread
        // In a full implementation, this would step through the coroutine's bytecode
        let result = self.execute_coroutine(&coroutine)?;
        
        self.is_running.store(false, Ordering::SeqCst);
        Ok(result)
    }

    /// Execute a coroutine
    /// Integrates with the VM to execute coroutine bytecode
    fn execute_coroutine(&self, coroutine: &Value) -> Result<Value> {
        match coroutine {
            Value::Coroutine { code, .. } => {
                // Create a VM instance to execute the coroutine
                use crate::bytecode::SuperBytecodeVM;
                let mut vm = SuperBytecodeVM::new();
                
                // Execute the coroutine's code
                vm.execute(*code.clone())?;
                
                // Get the return value from the VM
                // The result should be in the globals or returned somehow
                Ok(Value::None) // Placeholder - need to get actual return value
            }
            Value::Closure { compiled_code, .. } => {
                // Handle closures that might be coroutines
                if let Some(code) = compiled_code {
                    use crate::bytecode::SuperBytecodeVM;
                    let mut vm = SuperBytecodeVM::new();
                    vm.execute(*code.clone())?;
                    Ok(Value::None)
                } else {
                    Ok(Value::None)
                }
            }
            _ => Ok(coroutine.clone())
        }
    }

    /// Cancel a task
    pub fn cancel_task(&self, task_id: TaskId) -> Result<()> {
        let mut tasks = self.tasks.borrow_mut();
        if let Some(task) = tasks.get_mut(&task_id) {
            task.status = TaskStatus::Cancelled;
        }
        Ok(())
    }

    /// Check if a task is done
    pub fn is_task_done(&self, task_id: TaskId) -> bool {
        let tasks = self.tasks.borrow();
        tasks.get(&task_id)
            .map(|t| t.status == TaskStatus::Completed || t.status == TaskStatus::Cancelled)
            .unwrap_or(false)
    }

    /// Get task result (returns a cloned result, not Option<Result>)
    pub fn get_task_result(&self, task_id: TaskId) -> Option<Result<Value>> {
        let tasks = self.tasks.borrow();
        tasks.get(&task_id)
            .and_then(|t| {
                t.result.as_ref().map(|res| {
                    match res {
                        Ok(val) => Ok(val.clone()),
                        Err(e) => Err(anyhow::anyhow!("{}", e)),
                    }
                })
            })
    }

    /// Wait for multiple tasks to complete (gather)
    pub fn gather(&self, task_ids: Vec<TaskId>) -> Result<Vec<Value>> {
        let mut results = Vec::new();
        
        for task_id in task_ids {
            // Poll until task is done
            while !self.is_task_done(task_id) {
                std::thread::sleep(Duration::from_millis(1));
            }
            
            if let Some(result) = self.get_task_result(task_id) {
                results.push(result?);
            }
        }
        
        Ok(results)
    }

    /// Async sleep (delegates to Tokio)
    pub fn sleep(&self, duration: Duration) -> Result<()> {
        // Block on async sleep
        self.tokio_runtime.block_on(async {
            tokio::time::sleep(duration).await;
        });
        Ok(())
    }

    /// Shutdown the runtime
    pub fn shutdown(&self) {
        self.is_running.store(false, Ordering::SeqCst);
        // Tokio runtime will be dropped automatically
    }

    /// Get reference to Tokio runtime for I/O operations
    pub fn tokio(&self) -> &TokioRuntime {
        &self.tokio_runtime
    }
}

impl Drop for AsyncRuntime {
    fn drop(&mut self) {
        self.shutdown();
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_runtime_creation() {
        let runtime = AsyncRuntime::new().unwrap();
        assert!(!runtime.is_running.load(Ordering::SeqCst));
    }

    #[test]
    fn test_global_runtime() {
        let runtime1 = AsyncRuntime::global();
        let runtime2 = AsyncRuntime::global();
        assert!(Rc::ptr_eq(&runtime1, &runtime2));
    }

    #[test]
    fn test_task_creation() {
        let runtime = AsyncRuntime::new().unwrap();
        let task_id = runtime.create_task(Value::Int(42)).unwrap();
        assert_eq!(task_id, 1);
    }

    #[test]
    fn test_sleep() {
        let runtime = AsyncRuntime::new().unwrap();
        let start = std::time::Instant::now();
        runtime.sleep(Duration::from_millis(100)).unwrap();
        let elapsed = start.elapsed();
        assert!(elapsed >= Duration::from_millis(100));
    }
}
