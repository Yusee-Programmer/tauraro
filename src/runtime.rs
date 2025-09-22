//! TauraroLang Runtime - Innovative memory management with automatic GC and optional manual control
//! 
//! Key Features:
//! - Automatic memory management by default (developer doesn't need to think about it)
//! - Optional manual memory management with Rust-like safety guarantees
//! - Hybrid approach: Reference counting + opportunistic garbage collection
//! - Zero-cost abstractions for safe manual memory management

use std::collections::HashMap;
use std::sync::{Arc, RwLock};
use std::cell::RefCell;
use std::ptr;

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
}

/// Smart pointer for manual memory management
pub struct ManagedPtr<T> {
    ptr: *mut T,
    allocation: Option<Arc<Allocation>>,
    runtime: Arc<Runtime>,
}

/// The main runtime managing all allocations
pub struct Runtime {
    allocations: RwLock<HashMap<*mut u8, Arc<Allocation>>>,
    mode: MemoryMode,
    stats: RuntimeStats,
}

/// Runtime statistics
#[derive(Debug, Default)]
pub struct RuntimeStats {
    pub total_allocations: usize,
    pub current_allocations: usize,
    pub total_bytes: usize,
    pub collections: usize,
}

impl Runtime {
    pub fn new() -> Self {
        Self {
            allocations: RwLock::new(HashMap::new()),
            mode: MemoryMode::Automatic,
            stats: RuntimeStats::default(),
        }
    }
    
    /// Set global memory management mode
    pub fn set_mode(&self, mode: MemoryMode) {
        // This would need to be handled carefully in a real implementation
        // For simplicity, we'll just track it
        let mut stats = self.stats;
        stats.total_allocations = 0; // Reset stats when mode changes
    }
    
    /// Allocate memory with automatic management
    pub fn allocate_auto<T>(&self, value: T) -> ManagedPtr<T> {
        let size = std::mem::size_of::<T>();
        let ptr = Box::into_raw(Box::new(value));
        
        let allocation = Allocation {
            ptr: ptr as *mut u8,
            size,
            type_id: std::any::type_name::<T>(),
            rc: 1,
            mode: MemoryMode::Automatic,
            is_managed: true,
        };
        
        let arc_allocation = Arc::new(allocation);
        
        {
            let mut allocations = self.allocations.write().unwrap();
            allocations.insert(ptr as *mut u8, arc_allocation.clone());
        }
        
        self.stats.total_allocations += 1;
        self.stats.current_allocations += 1;
        self.stats.total_bytes += size;
        
        ManagedPtr {
            ptr,
            allocation: Some(arc_allocation),
            runtime: Arc::new(self.clone()), // Simplified
        }
    }
    
    /// Allocate memory with manual management
    pub fn allocate_manual<T>(&self, value: T) -> ManagedPtr<T> {
        let size = std::mem::size_of::<T>();
        let ptr = Box::into_raw(Box::new(value));
        
        let allocation = Allocation {
            ptr: ptr as *mut u8,
            size,
            type_id: std::any::type_name::<T>(),
            rc: 0, // Manual mode doesn't use reference counting
            mode: MemoryMode::Manual,
            is_managed: false,
        };
        
        let arc_allocation = Arc::new(allocation);
        
        {
            let mut allocations = self.allocations.write().unwrap();
            allocations.insert(ptr as *mut u8, arc_allocation.clone());
        }
        
        self.stats.total_allocations += 1;
        self.stats.current_allocations += 1;
        self.stats.total_bytes += size;
        
        ManagedPtr {
            ptr,
            allocation: Some(arc_allocation),
            runtime: Arc::new(self.clone()),
        }
    }
    
    /// Perform garbage collection (automatic mode only)
    pub fn collect_garbage(&self) {
        let mut allocations = self.allocations.write().unwrap();
        let mut to_remove = Vec::new();
        
        for (ptr, allocation) in allocations.iter() {
            if allocation.mode == MemoryMode::Automatic && allocation.rc == 0 {
                unsafe {
                    Box::from_raw(ptr as *mut T); // Drop the value
                    libc::free(*ptr); // Free the memory
                }
                to_remove.push(*ptr);
            }
        }
        
        for ptr in to_remove {
            allocations.remove(&ptr);
            self.stats.current_allocations -= 1;
        }
        
        self.stats.collections += 1;
    }
    
    /// Get runtime statistics
    pub fn stats(&self) -> RuntimeStats {
        self.stats
    }
}

impl<T> ManagedPtr<T> {
    /// Create a new reference (increases RC in automatic mode)
    pub fn clone(&self) -> Self {
        if let Some(ref alloc) = self.allocation {
            if alloc.mode == MemoryMode::Automatic {
                // Increase reference count
                let mut allocations = self.runtime.allocations.write().unwrap();
                if let Some(existing) = allocations.get_mut(&(self.ptr as *mut u8)) {
                    // We'd need interior mutability here - simplified for example
                }
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
    pub fn free(mut self) {
        if let Some(allocation) = self.allocation.take() {
            if allocation.mode == MemoryMode::Manual {
                unsafe {
                    Box::from_raw(self.ptr);
                    let mut allocations = self.runtime.allocations.write().unwrap();
                    allocations.remove(&(self.ptr as *mut u8));
                }
                self.runtime.stats.current_allocations -= 1;
            }
        }
    }
    
    /// Check if this pointer is managed automatically
    pub fn is_automatic(&self) -> bool {
        self.allocation.as_ref().map_or(false, |a| a.mode == MemoryMode::Automatic)
    }
    
    /// Convert automatic pointer to manual management
    pub fn to_manual(self) -> Result<Self, &'static str> {
        if self.is_automatic() {
            // In real implementation, we'd need to handle the transition
            Err("Cannot convert automatic pointer to manual")
        } else {
            Ok(self)
        }
    }
}

impl<T> Drop for ManagedPtr<T> {
    fn drop(&mut self) {
        if let Some(ref allocation) = self.allocation {
            match allocation.mode {
                MemoryMode::Automatic => {
                    // Decrease reference count - if zero, GC will collect eventually
                    // Simplified implementation
                }
                MemoryMode::Manual => {
                    // Manual pointers must be explicitly freed
                    if !ptr::eq(self.ptr, ptr::null_mut()) {
                        panic!("Manual pointer dropped without calling free()! Use .free() explicitly.");
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
}

impl Arena {
    pub fn new(block_size: usize) -> Self {
        Self {
            blocks: Vec::new(),
            current_block: Vec::with_capacity(block_size),
            block_size,
        }
    }
    
    pub fn allocate<T>(&mut self, value: T) -> &mut T {
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
        let ptr = &mut self.current_block[aligned_start] as *mut u8;
        
        // Store the value
        unsafe {
            let target = ptr as *mut T;
            target.write(value);
            &mut *target
        }
    }
}

/// Memory management API exposed to TauraroLang code
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
    pub fn auto<T>(&self, value: T) -> ManagedPtr<T> {
        self.runtime.allocate_auto(value)
    }
    
    /// Manual allocation - developer takes responsibility
    pub fn manual<T>(&self, value: T) -> ManagedPtr<T> {
        self.runtime.allocate_manual(value)
    }
    
    /// Arena allocation for batch operations
    pub fn arena_alloc<T>(&self, arena: &mut Arena, value: T) -> &mut T {
        arena.allocate(value)
    }
    
    /// Force garbage collection
    pub fn collect(&self) {
        self.runtime.collect_garbage();
    }
    
    /// Get memory statistics
    pub fn stats(&self) -> RuntimeStats {
        self.runtime.stats()
    }
}

// Thread-safe global runtime
lazy_static::lazy_static! {
    pub static ref GLOBAL_RUNTIME: Arc<Runtime> = Arc::new(Runtime::new());
}