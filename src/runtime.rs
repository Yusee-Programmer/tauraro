//! COMPLETE TauraroLang Runtime - Innovative memory management with automatic GC and optional manual control
use std::collections::HashMap;
use std::sync::{Arc, RwLock, OnceLock};
use std::ptr;
use std::any::Any;
use std::fmt;

/// Memory management mode
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum MemoryMode {
    Automatic,    // Fully automatic memory management (default)
    Manual,       // Manual memory management with safety checks
    Arena,        // Arena-based allocation for high performance
}

/// Memory allocation with metadata
#[derive(Debug)]
pub struct Allocation {
    pub ptr: *mut u8,
    pub size: usize,
    pub type_id: &'static str,
    pub rc: usize,           // Reference count for automatic mode
    pub mode: MemoryMode,    // How this allocation is managed
    pub is_managed: bool,    // Whether GC can automatically collect
    pub trace_fn: Option<fn(*mut u8) -> Vec<*mut u8>>, // For tracing GC
}

/// Smart pointer for manual memory management
pub struct ManagedPtr<T> {
    ptr: *mut T,
    allocation: Option<Arc<Allocation>>,
    runtime: Arc<Runtime>,
}

/// The main runtime managing all allocations
#[derive(Clone)]
pub struct Runtime {
    allocations: Arc<RwLock<HashMap<*mut u8, Arc<Allocation>>>>,
    mode: MemoryMode,
    stats: Arc<RwLock<RuntimeStats>>,
}

/// Runtime statistics
#[derive(Debug, Default)]
pub struct RuntimeStats {
    pub total_allocations: usize,
    pub current_allocations: usize,
    pub total_bytes: usize,
    pub collections: usize,
    pub manual_allocations: usize,
    pub auto_allocations: usize,
}

impl Runtime {
    pub fn new() -> Self {
        Self {
            allocations: Arc::new(RwLock::new(HashMap::new())),
            mode: MemoryMode::Automatic,
            stats: Arc::new(RwLock::new(RuntimeStats::default())),
        }
    }
    
    /// Set global memory management mode
    pub fn set_mode(&mut self, mode: MemoryMode) {
        self.mode = mode;
    }
    
    /// Get current memory mode
    pub fn get_mode(&self) -> MemoryMode {
        self.mode
    }
    
    /// Allocate memory with automatic management
    pub fn allocate_auto<T: 'static>(&self, value: T) -> ManagedPtr<T> {
        let size = std::mem::size_of::<T>();
        let ptr = Box::into_raw(Box::new(value));
        
        let allocation = Allocation {
            ptr: ptr as *mut u8,
            size,
            type_id: std::any::type_name::<T>(),
            rc: 1,
            mode: MemoryMode::Automatic,
            is_managed: true,
            trace_fn: Some(Self::trace_object::<T>),
        };
        
        let arc_allocation = Arc::new(allocation);
        
        {
            let mut allocations = self.allocations.write().unwrap();
            allocations.insert(ptr as *mut u8, arc_allocation.clone());
        }
        
        {
            let mut stats = self.stats.write().unwrap();
            stats.total_allocations += 1;
            stats.current_allocations += 1;
            stats.total_bytes += size;
            stats.auto_allocations += 1;
        }
        
        ManagedPtr {
            ptr,
            allocation: Some(arc_allocation),
            runtime: Arc::new(self.clone()),
        }
    }
    
    /// Allocate memory with manual management
    pub fn allocate_manual<T: 'static>(&self, value: T) -> ManagedPtr<T> {
        let size = std::mem::size_of::<T>();
        let ptr = Box::into_raw(Box::new(value));
        
        let allocation = Allocation {
            ptr: ptr as *mut u8,
            size,
            type_id: std::any::type_name::<T>(),
            rc: 0, // Manual mode doesn't use reference counting
            mode: MemoryMode::Manual,
            is_managed: false,
            trace_fn: None,
        };
        
        let arc_allocation = Arc::new(allocation);
        
        {
            let mut allocations = self.allocations.write().unwrap();
            allocations.insert(ptr as *mut u8, arc_allocation.clone());
        }
        
        {
            let mut stats = self.stats.write().unwrap();
            stats.total_allocations += 1;
            stats.current_allocations += 1;
            stats.total_bytes += size;
            stats.manual_allocations += 1;
        }
        
        ManagedPtr {
            ptr,
            allocation: Some(arc_allocation),
            runtime: Arc::new(self.clone()),
        }
    }
    
    /// Trace function for garbage collection
    fn trace_object<T: 'static>(_ptr: *mut u8) -> Vec<*mut u8> {
        // Default implementation: no internal pointers to trace
        // For complex objects, this would trace internal references
        Vec::new()
    }
    
    /// Increment reference count
    fn increment_rc(&self, ptr: *mut u8) {
        let mut allocations = self.allocations.write().unwrap();
        if let Some(allocation) = allocations.get_mut(&ptr) {
            if allocation.mode == MemoryMode::Automatic {
                // We need to clone the Arc to get mutable access to the underlying Allocation
                if let Some(allocation_clone) = Arc::get_mut(allocation) {
                    allocation_clone.rc += 1;
                }
            }
        }
    }
    
    /// Decrement reference count and collect if zero
    fn decrement_rc(&self, ptr: *mut u8) {
        let should_collect = {
            let mut allocations = self.allocations.write().unwrap();
            if let Some(allocation) = allocations.get_mut(&ptr) {
                if allocation.mode == MemoryMode::Automatic {
                    if let Some(allocation_clone) = Arc::get_mut(allocation) {
                        allocation_clone.rc -= 1;
                        return allocation_clone.rc == 0;
                    }
                }
            }
            false
        };
        
        if should_collect {
            self.collect_garbage();
        }
    }
    
    /// Perform garbage collection (automatic mode only)
    pub fn collect_garbage(&self) {
        let mut allocations = self.allocations.write().unwrap();
        let mut to_remove = Vec::new();
        
        for (ptr, allocation) in allocations.iter() {
            if allocation.mode == MemoryMode::Automatic && allocation.rc == 0 {
                unsafe {
                    // Call destructor if needed
                    drop(Box::from_raw(ptr as *mut u8 as *mut dyn Any));
                }
                to_remove.push(*ptr);
            }
        }
        
        for ptr in to_remove {
            allocations.remove(&ptr);
            let mut stats = self.stats.write().unwrap();
            stats.current_allocations -= 1;
        }
        
        let mut stats = self.stats.write().unwrap();
        stats.collections += 1;
    }
    
    /// Perform full tracing garbage collection
    pub fn collect_tracing_gc(&self) {
        let roots = self.find_gc_roots();
        let mut reachable = HashMap::new();
        
        // Mark phase
        for root in roots {
            self.mark_reachable(root, &mut reachable);
        }
        
        // Sweep phase
        let mut allocations = self.allocations.write().unwrap();
        let mut to_remove: Vec<*mut u8> = allocations.keys()
            .filter(|ptr| !reachable.contains_key(ptr))
            .cloned()
            .collect();
        
        for ptr in to_remove.drain(..) {
            if let Some(allocation) = allocations.remove(&ptr) {
                if allocation.mode == MemoryMode::Automatic {
                    unsafe {
                        drop(Box::from_raw(ptr as *mut u8 as *mut dyn Any));
                    }
                    let mut stats = self.stats.write().unwrap();
                    stats.current_allocations -= 1;
                }
            }
        }
        
        let mut stats = self.stats.write().unwrap();
        stats.collections += 1;
    }
    
    /// Find GC roots (stack variables, globals, etc.)
    fn find_gc_roots(&self) -> Vec<*mut u8> {
        // In a real implementation, this would scan the stack and global variables
        // For now, return an empty vector (simplified)
        Vec::new()
    }
    
    /// Mark reachable objects from a root
    fn mark_reachable(&self, root: *mut u8, reachable: &mut HashMap<*mut u8, bool>) {
        if reachable.contains_key(&root) {
            return;
        }
        
        reachable.insert(root, true);
        
        let allocations = self.allocations.read().unwrap();
        if let Some(allocation) = allocations.get(&root) {
            if let Some(trace_fn) = allocation.trace_fn {
                let references = trace_fn(root);
                for reference in references {
                    self.mark_reachable(reference, reachable);
                }
            }
        }
    }
    
    /// Get runtime statistics
    pub fn stats(&self) -> RuntimeStats {
        self.stats.read().unwrap().clone()
    }
    
    /// Get memory usage in human-readable format
    pub fn memory_usage(&self) -> String {
        let stats = self.stats();
        format!("Allocations: {}/{} ({} bytes), Mode: {:?}", 
            stats.current_allocations, stats.total_allocations, 
            stats.total_bytes, self.mode)
    }
}

impl<T> ManagedPtr<T> {
    /// Create a new reference (increases RC in automatic mode)
    pub fn clone(&self) -> Self {
        if let Some(ref alloc) = self.allocation {
            if alloc.mode == MemoryMode::Automatic {
                self.runtime.increment_rc(self.ptr as *mut u8);
            }
        }
        
        Self {
            ptr: self.ptr,
            allocation: self.allocation.clone(),
            runtime: self.runtime.clone(),
        }
    }
    
    /// Access the underlying value
    pub fn get(&self) -> &T {
        unsafe { &*self.ptr }
    }
    
    /// Access mutably
    pub fn get_mut(&mut self) -> &mut T {
        unsafe { &mut *self.ptr }
    }
    
    /// Explicitly free memory (manual mode only)
    pub fn free(mut self) -> Result<(), &'static str> {
        if let Some(allocation) = self.allocation.take() {
            if allocation.mode == MemoryMode::Manual {
                unsafe {
                    drop(Box::from_raw(self.ptr));
                    let mut allocations = self.runtime.allocations.write().unwrap();
                    allocations.remove(&(self.ptr as *mut u8));
                }
                let mut stats = self.runtime.stats.write().unwrap();
                stats.current_allocations -= 1;
                self.ptr = ptr::null_mut();
                Ok(())
            } else {
                Err("Cannot free automatic pointer - use automatic memory management")
            }
        } else {
            Err("Pointer already freed")
        }
    }
    
    /// Check if this pointer is managed automatically
    pub fn is_automatic(&self) -> bool {
        self.allocation.as_ref().map_or(false, |a| a.mode == MemoryMode::Automatic)
    }
    
    /// Check if this pointer is managed manually
    pub fn is_manual(&self) -> bool {
        self.allocation.as_ref().map_or(false, |a| a.mode == MemoryMode::Manual)
    }
    
    /// Convert automatic pointer to manual management
    pub fn to_manual(self) -> Result<Self, &'static str> {
        if self.is_automatic() {
            // In real implementation, we'd need to handle the transition carefully
            // For now, we don't allow conversion
            Err("Cannot convert automatic pointer to manual")
        } else {
            Ok(self)
        }
    }
    
    /// Get the memory mode of this pointer
    pub fn memory_mode(&self) -> Option<MemoryMode> {
        self.allocation.as_ref().map(|a| a.mode)
    }
}

impl<T> Drop for ManagedPtr<T> {
    fn drop(&mut self) {
        if let Some(ref allocation) = self.allocation {
            match allocation.mode {
                MemoryMode::Automatic => {
                    // Decrease reference count
                    self.runtime.decrement_rc(self.ptr as *mut u8);
                }
                MemoryMode::Manual => {
                    // Manual pointers must be explicitly freed
                    if !ptr::eq(self.ptr, ptr::null_mut()) {
                        // In debug mode, panic to catch memory leaks
                        #[cfg(debug_assertions)]
                        panic!("Manual pointer dropped without calling free()! Use .free() explicitly.");
                        
                        // In release mode, just leak the memory
                        #[cfg(not(debug_assertions))]
                        eprintln!("WARNING: Manual pointer leaked. Call .free() explicitly.");
                    }
                }
                MemoryMode::Arena => {
                    // Arena allocations are freed when arena is dropped
                }
            }
        }
    }
}

/// Arena allocator for high-performance scenarios
pub struct Arena {
    blocks: Vec<Vec<u8>>,
    current_block: Vec<u8>,
    block_size: usize,
    runtime: Arc<Runtime>,
}

impl Arena {
    pub fn new(block_size: usize, runtime: Arc<Runtime>) -> Self {
        Self {
            blocks: Vec::new(),
            current_block: Vec::with_capacity(block_size),
            block_size,
            runtime,
        }
    }
    
    pub fn allocate<T: 'static>(&mut self, value: T) -> ManagedPtr<T> {
        let size = std::mem::size_of::<T>();
        let align = std::mem::align_of::<T>();
        
        // Ensure proper alignment
        let start = self.current_block.len();
        let aligned_start = (start + align - 1) & !(align - 1);
        
        if aligned_start + size > self.current_block.capacity() {
            self.blocks.push(std::mem::take(&mut self.current_block));
            self.current_block = Vec::with_capacity(self.block_size.max(size));
        }
        
        // Allocate space
        self.current_block.resize(aligned_start + size, 0);
        let ptr = self.current_block.as_mut_ptr().wrapping_add(aligned_start) as *mut T;
        
        // Store the value
        unsafe {
            ptr.write(value);
        }
        
        // Create a managed pointer for the arena allocation
        let allocation = Allocation {
            ptr: ptr as *mut u8,
            size,
            type_id: std::any::type_name::<T>(),
            rc: 1,
            mode: MemoryMode::Arena,
            is_managed: true,
            trace_fn: None,
        };
        
        ManagedPtr {
            ptr,
            allocation: Some(Arc::new(allocation)),
            runtime: self.runtime.clone(),
        }
    }
    
    /// Clear the arena (free all allocations)
    pub fn clear(&mut self) {
        self.blocks.clear();
        self.current_block.clear();
    }
}

impl Drop for Arena {
    fn drop(&mut self) {
        // All arena allocations are freed when the arena is dropped
        self.clear();
    }
}

/// Memory management API exposed to TauraroLang code
#[derive(Clone)]
pub struct MemoryAPI {
    runtime: Arc<Runtime>,
}

impl MemoryAPI {
    pub fn new() -> Self {
        Self {
            runtime: Arc::new(Runtime::new()),
        }
    }
    
    /// Automatic allocation - the default
    pub fn auto<T: 'static>(&self, value: T) -> ManagedPtr<T> {
        self.runtime.allocate_auto(value)
    }
    
    /// Manual allocation - developer takes responsibility
    pub fn manual<T: 'static>(&self, value: T) -> ManagedPtr<T> {
        self.runtime.allocate_manual(value)
    }
    
    /// Create a new arena for batch allocations
    pub fn create_arena(&self, block_size: usize) -> Arena {
        Arena::new(block_size, self.runtime.clone())
    }
    
    /// Arena allocation for batch operations
    pub fn arena_alloc<T: 'static>(&self, arena: &mut Arena, value: T) -> ManagedPtr<T> {
        arena.allocate(value)
    }
    
    /// Force garbage collection
    pub fn collect(&self) {
        self.runtime.collect_garbage();
    }
    
    /// Force tracing garbage collection
    pub fn collect_tracing(&self) {
        self.runtime.collect_tracing_gc();
    }
    
    /// Get memory statistics
    pub fn stats(&self) -> RuntimeStats {
        self.runtime.stats()
    }
    
    /// Get memory usage summary
    pub fn memory_usage(&self) -> String {
        self.runtime.memory_usage()
    }
    
    /// Set global memory mode
    pub fn set_memory_mode(&self, mode: MemoryMode) {
        let mut runtime = self.runtime.as_ref().clone();
        runtime.set_mode(mode);
    }
}

// Thread-safe global runtime
lazy_static::lazy_static! {
    // Thread-safe global runtime using OnceLock
    static GLOBAL_RUNTIME: OnceLock<Arc<Runtime>> = OnceLock::new();
    static GLOBAL_MEMORY_API: OnceLock<MemoryAPI> = OnceLock::new();

    pub fn get_global_runtime() -> Arc<Runtime> {
        GLOBAL_RUNTIME.get_or_init(|| Arc::new(Runtime::new())).clone()
    }

    pub fn get_global_memory_api() -> MemoryAPI {
        GLOBAL_MEMORY_API.get_or_init(|| MemoryAPI::new()).clone()
    }
}

// Safe conversion traits for common types
impl<T> From<T> for ManagedPtr<T> {
    fn from(value: T) -> Self {
        get_global_memory_api().auto(value)
    }
}

impl<T> std::ops::Deref for ManagedPtr<T> {
    type Target = T;
    
    fn deref(&self) -> &T {
        self.get()
    }
}

impl<T> std::ops::DerefMut for ManagedPtr<T> {
    fn deref_mut(&mut self) -> &mut T {
        self.get_mut()
    }
}

impl fmt::Display for RuntimeStats {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "allocs: {}, frees: {}, total: {}", self.allocs, self.frees, self.total)
    }
}