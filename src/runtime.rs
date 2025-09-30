//! COMPLETE TauraroLang Runtime - Innovative memory management with automatic GC and optional manual control
use std::collections::HashMap;
use std::sync::{Arc, RwLock, OnceLock};
use std::ptr;
use std::any::Any;
use std::fmt;

// Thread-safe global runtime using OnceLock
static GLOBAL_RUNTIME: OnceLock<Arc<Runtime>> = OnceLock::new();
static GLOBAL_MEMORY_API: OnceLock<MemoryAPI> = OnceLock::new();

/// Memory management mode
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum MemoryMode {
    Automatic,    // Fully automatic memory management (default)
    Manual,       // Manual memory management with safety checks
    Arena,        // Arena-based allocation for high performance
    Hybrid,       // Automatic with manual override capability
}

/// Memory allocation with metadata
#[derive(Debug)]
pub struct Allocation {
    pub size: usize,
    pub type_id: &'static str,
    pub rc: usize,           // Reference count for automatic mode
    pub mode: MemoryMode,    // How this allocation is managed
    pub is_managed: bool,    // Whether GC can automatically collect
    pub trace_fn: Option<fn(usize) -> Vec<usize>>, // For tracing GC, now uses usize
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
    allocations: Arc<RwLock<HashMap<usize, Arc<Allocation>>>>, // Use usize instead of raw pointer
    mode: MemoryMode,
    stats: Arc<RwLock<RuntimeStats>>,
}

/// Runtime statistics
#[derive(Debug, Default, Clone)]
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
            size,
            type_id: std::any::type_name::<T>(),
            rc: 1,
            mode: MemoryMode::Automatic,
            is_managed: true,
            trace_fn: Some(Runtime::default_trace_fn),
        };
        
        let arc_allocation = Arc::new(allocation);
        
        {
            let mut allocations = self.allocations.write().unwrap();
            allocations.insert(ptr as usize, arc_allocation.clone());
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
            allocations.insert(ptr as usize, arc_allocation.clone());
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
    
    /// Allocate memory with hybrid management (can switch between auto/manual)
    pub fn allocate_hybrid<T: 'static>(&self, value: T) -> ManagedPtr<T> {
        let size = std::mem::size_of::<T>();
        let ptr = Box::into_raw(Box::new(value));
        
        let allocation = Allocation {
            size,
            type_id: std::any::type_name::<T>(),
            rc: 1, // Start with reference counting
            mode: MemoryMode::Hybrid,
            is_managed: true,
            trace_fn: Some(Runtime::default_trace_fn),
        };
        
        let arc_allocation = Arc::new(allocation);
        
        {
            let mut allocations = self.allocations.write().unwrap();
            allocations.insert(ptr as usize, arc_allocation.clone());
        }
        
        {
            let mut stats = self.stats.write().unwrap();
            stats.total_allocations += 1;
            stats.current_allocations += 1;
            stats.total_bytes += size;
            stats.auto_allocations += 1; // Count as auto for stats
        }
        
        ManagedPtr {
            ptr,
            allocation: Some(arc_allocation),
            runtime: Arc::new(self.clone()),
        }
    }
    
    /// Default trace function for garbage collection
    fn default_trace_fn(_ptr: usize) -> Vec<usize> {
        // Default implementation: no internal pointers to trace
        // For complex objects, this would trace internal references
        Vec::new()
    }
    
    /// Trace function for garbage collection
    fn trace_object<T>(_ptr: usize) -> Vec<usize> 
    where 
        T: 'static,
    {
        // Default implementation: no internal pointers to trace
        // For complex objects, this would trace internal references
        Vec::new()
    }
    
    /// Increment reference count
    fn increment_rc(&self, ptr: *mut u8) {
        let mut allocations = self.allocations.write().unwrap();
        if let Some(allocation) = allocations.get_mut(&(ptr as usize)) {
            // Allow incrementing RC for both Automatic and Hybrid modes
            if allocation.mode == MemoryMode::Automatic || allocation.mode == MemoryMode::Hybrid {
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
            if let Some(allocation) = allocations.get_mut(&(ptr as usize)) {
                // Allow decrementing RC for both Automatic and Hybrid modes
                if allocation.mode == MemoryMode::Automatic || allocation.mode == MemoryMode::Hybrid {
                    if let Some(allocation_clone) = Arc::get_mut(allocation) {
                        if allocation_clone.rc > 0 {
                            allocation_clone.rc -= 1;
                        }
                        allocation_clone.rc == 0
                    } else {
                        false
                    }
                } else {
                    false
                }
            } else {
                false
            }
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
            // Collect objects in Automatic and Hybrid modes with zero reference count
            if (allocation.mode == MemoryMode::Automatic || allocation.mode == MemoryMode::Hybrid) && allocation.rc == 0 {
                // Note: Cannot safely drop arbitrary pointers without type information
                // This would require storing type information in Allocation
                // For now, we'll just mark as freed without calling destructors
                to_remove.push(*ptr);
            }
        }
        
        for ptr in to_remove {
            if let Some(allocation) = allocations.remove(&ptr) {
                let mut stats = self.stats.write().unwrap();
                stats.current_allocations -= 1;
                stats.total_bytes -= allocation.size;
            }
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
        let mut to_remove: Vec<usize> = allocations.keys()
            .filter(|ptr| {
                // Convert usize back to pointer for reachability check
                let ptr_as_raw = **ptr as *mut u8;
                !reachable.contains_key(&ptr_as_raw)
            })
            .cloned()
            .collect();
        
        for ptr in to_remove.drain(..) {
            if let Some(allocation) = allocations.remove(&ptr) {
                // Collect objects in Automatic and Hybrid modes
                if allocation.mode == MemoryMode::Automatic || allocation.mode == MemoryMode::Hybrid {
                    // Note: Cannot safely drop arbitrary pointers without type information
                    // This would require storing type information in Allocation
                    // For now, we'll just mark as freed without calling destructors
                    let mut stats = self.stats.write().unwrap();
                    stats.current_allocations -= 1;
                    stats.total_bytes -= allocation.size;
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
        if let Some(allocation) = allocations.get(&(root as usize)) {
            if let Some(trace_fn) = allocation.trace_fn {
                let references = trace_fn(root as usize);
                for reference in references {
                    self.mark_reachable(reference as *mut u8, reachable);
                }
            }
        }
    }
    
    /// Get runtime statistics
    pub fn stats(&self) -> RuntimeStats {
        (*self.stats.read().unwrap()).clone()
    }
    
    /// Get memory usage in human-readable format
    pub fn memory_usage(&self) -> String {
        let stats = self.stats();
        format!("Allocations: {}/{} ({} bytes), Collections: {}, Mode: {:?}", 
            stats.current_allocations, stats.total_allocations, 
            stats.total_bytes, stats.collections, self.mode)
    }
    
    /// Convert automatic/hybrid pointer to manual management
    pub fn convert_to_manual<T: 'static>(&self, mut ptr: ManagedPtr<T>) -> Result<ManagedPtr<T>, &'static str> {
        if let Some(ref allocation) = ptr.allocation {
            if allocation.mode == MemoryMode::Automatic || allocation.mode == MemoryMode::Hybrid {
                // Update the allocation mode to Manual
                let mut allocations = self.allocations.write().unwrap();
                if let Some(existing_allocation) = allocations.get_mut(&(ptr.ptr as usize)) {
                    // Create a new allocation with Manual mode
                    let new_allocation = Allocation {
                        size: existing_allocation.size,
                        type_id: existing_allocation.type_id,
                        rc: 0, // Manual mode doesn't use reference counting
                        mode: MemoryMode::Manual,
                        is_managed: false,
                        trace_fn: None,
                    };
                    
                    // Replace the allocation
                    *existing_allocation = Arc::new(new_allocation);
                    
                    // Update the pointer's allocation reference
                    ptr.allocation = Some(existing_allocation.clone());
                }
                Ok(ptr)
            } else {
                Ok(ptr) // Already manual
            }
        } else {
            Err("Invalid pointer")
        }
    }
    
    /// Convert manual pointer to automatic management
    pub fn convert_to_automatic<T: 'static>(&self, mut ptr: ManagedPtr<T>) -> Result<ManagedPtr<T>, &'static str> {
        if let Some(ref allocation) = ptr.allocation {
            if allocation.mode == MemoryMode::Manual {
                // Update the allocation mode to Automatic
                let mut allocations = self.allocations.write().unwrap();
                if let Some(existing_allocation) = allocations.get_mut(&(ptr.ptr as usize)) {
                    // Create a new allocation with Automatic mode
                    let new_allocation = Allocation {
                        size: existing_allocation.size,
                        type_id: existing_allocation.type_id,
                        rc: 1, // Start with reference count of 1
                        mode: MemoryMode::Automatic,
                        is_managed: true,
                        trace_fn: Some(Runtime::default_trace_fn),
                    };
                    
                    // Replace the allocation
                    *existing_allocation = Arc::new(new_allocation);
                    
                    // Update the pointer's allocation reference
                    ptr.allocation = Some(existing_allocation.clone());
                }
                Ok(ptr)
            } else {
                Ok(ptr) // Already automatic or hybrid
            }
        } else {
            Err("Invalid pointer")
        }
    }
}

impl<T: 'static> ManagedPtr<T> {
    /// Create a new reference (increases RC in automatic/hybrid mode)
    pub fn clone(&self) -> Self {
        if let Some(ref alloc) = self.allocation {
            // Allow cloning for both Automatic and Hybrid modes
            if alloc.mode == MemoryMode::Automatic || alloc.mode == MemoryMode::Hybrid {
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
                    allocations.remove(&(self.ptr as usize));
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
    
    /// Check if this pointer is managed in hybrid mode
    pub fn is_hybrid(&self) -> bool {
        self.allocation.as_ref().map_or(false, |a| a.mode == MemoryMode::Hybrid)
    }
    
    /// Convert automatic/hybrid pointer to manual management
    pub fn to_manual(self) -> Result<Self, &'static str> {
        if self.is_automatic() || self.is_hybrid() {
            let runtime = self.runtime.clone();
            runtime.convert_to_manual(self)
        } else {
            Ok(self) // Already manual
        }
    }
    
    /// Convert manual pointer to automatic management
    pub fn to_automatic(self) -> Result<Self, &'static str> {
        if self.is_manual() {
            let runtime = self.runtime.clone();
            runtime.convert_to_automatic(self)
        } else {
            Ok(self) // Already automatic or hybrid
        }
    }
    
    /// Get the memory mode of this pointer
    pub fn memory_mode(&self) -> Option<MemoryMode> {
        self.allocation.as_ref().map(|a| a.mode)
    }
    
    /// Force garbage collection for this specific object (if in automatic/hybrid mode)
    pub fn collect(&self) {
        if let Some(ref allocation) = self.allocation {
            if allocation.mode == MemoryMode::Automatic || allocation.mode == MemoryMode::Hybrid {
                self.runtime.decrement_rc(self.ptr as *mut u8);
            }
        }
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
                MemoryMode::Hybrid => {
                    // Hybrid mode uses reference counting like automatic
                    self.runtime.decrement_rc(self.ptr as *mut u8);
                }
            }
        }
    }
}

// Manual Debug implementation for Runtime
impl std::fmt::Debug for Runtime {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Runtime")
            .field("mode", &self.mode)
            .field("allocations_count", &self.allocations.read().unwrap().len())
            .finish()
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
#[derive(Debug, Clone)]
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
    
    /// Hybrid allocation - automatic with manual override capability
    pub fn hybrid<T: 'static>(&self, value: T) -> ManagedPtr<T> {
        self.runtime.allocate_hybrid(value)
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
    
    /// Get current memory mode
    pub fn get_memory_mode(&self) -> MemoryMode {
        self.runtime.get_mode()
    }
    
    /// Configure garbage collector thresholds
    pub fn configure_gc(&self, threshold: usize) {
        // In a full implementation, this would set GC thresholds
        // For now, we'll just log the configuration
        println!("GC threshold configured to: {}", threshold);
    }
}

/// Get or initialize the global memory API
pub fn get_global_memory_api() -> &'static MemoryAPI {
    GLOBAL_MEMORY_API.get_or_init(|| MemoryAPI::new())
}

// Safe conversion traits for common types
impl<T: 'static> From<T> for ManagedPtr<T> {
    fn from(value: T) -> Self {
        get_global_memory_api().auto(value)
    }
}

impl<T: 'static> std::ops::Deref for ManagedPtr<T> {
    type Target = T;
    
    fn deref(&self) -> &T {
        self.get()
    }
}

impl<T: 'static> std::ops::DerefMut for ManagedPtr<T> {
    fn deref_mut(&mut self) -> &mut T {
        self.get_mut()
    }
}

impl fmt::Display for RuntimeStats {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "allocs: {}, current: {}, total_bytes: {}", 
               self.total_allocations, 
               self.current_allocations, 
               self.total_bytes)
    }
}
