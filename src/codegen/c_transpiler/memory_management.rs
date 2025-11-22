//! Memory Management for C Transpiler
//!
//! Implements multiple memory management strategies:
//! - Automatic reference counting
//! - Manual allocation/deallocation
//! - Arena/pool allocation
//! - Garbage collection
//! - Copy-on-Write

use std::collections::{HashMap, HashSet};

/// Memory management strategy
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum MemoryStrategy {
    /// Automatic reference counting with cycle detection
    Automatic,
    /// Manual malloc/free with ownership tracking
    Manual,
    /// Arena allocation for batch operations
    Arena,
    /// Generational garbage collection
    Generational,
    /// Copy-on-Write for efficient sharing
    CopyOnWrite,
}

impl Default for MemoryStrategy {
    fn default() -> Self {
        MemoryStrategy::Automatic
    }
}

/// Ownership information for a variable
#[derive(Clone, Debug)]
pub struct OwnershipInfo {
    pub var_name: String,
    pub owner_count: usize,
    pub type_info: String,
    pub is_borrowed: bool,
    pub scope_depth: usize,
}

/// Memory management context
pub struct MemoryManagementContext {
    pub strategy: MemoryStrategy,
    pub ownership_map: HashMap<String, OwnershipInfo>,
    pub scope_depth: usize,
    pub allocations: HashSet<String>,
    pub reference_counts: HashMap<String, usize>,
    pub generations: HashMap<String, usize>,
    pub current_generation: usize,
}

impl MemoryManagementContext {
    pub fn new(strategy: MemoryStrategy) -> Self {
        Self {
            strategy,
            ownership_map: HashMap::new(),
            scope_depth: 0,
            allocations: HashSet::new(),
            reference_counts: HashMap::new(),
            generations: HashMap::new(),
            current_generation: 0,
        }
    }

    /// Enter a new scope
    pub fn enter_scope(&mut self) {
        self.scope_depth += 1;
    }

    /// Exit current scope and clean up
    pub fn exit_scope(&mut self) -> Vec<String> {
        let mut cleanup = Vec::new();
        
        // Find all variables in current scope that need cleanup
        for (var_name, info) in self.ownership_map.iter() {
            if info.scope_depth == self.scope_depth && info.owner_count == 1 {
                cleanup.push(var_name.clone());
            }
        }
        
        // Remove them from tracking
        for var in &cleanup {
            self.ownership_map.remove(var);
            self.allocations.remove(var);
        }
        
        self.scope_depth -= 1;
        cleanup
    }

    /// Register a new allocation
    pub fn register_allocation(&mut self, var_name: String, type_info: String) {
        self.allocations.insert(var_name.clone());
        self.ownership_map.insert(var_name.clone(), OwnershipInfo {
            var_name: var_name.clone(),
            owner_count: 1,
            type_info,
            is_borrowed: false,
            scope_depth: self.scope_depth,
        });
        
        if self.strategy == MemoryStrategy::Automatic {
            self.reference_counts.insert(var_name.clone(), 1);
        }
        
        if self.strategy == MemoryStrategy::Generational {
            self.generations.insert(var_name, self.current_generation);
        }
    }

    /// Increment reference count
    pub fn increment_ref(&mut self, var_name: &str) {
        if let Some(count) = self.reference_counts.get_mut(var_name) {
            *count += 1;
        }
        if let Some(info) = self.ownership_map.get_mut(var_name) {
            info.owner_count += 1;
        }
    }

    /// Decrement reference count
    pub fn decrement_ref(&mut self, var_name: &str) -> bool {
        let mut should_free = false;
        
        if let Some(count) = self.reference_counts.get_mut(var_name) {
            *count -= 1;
            should_free = *count == 0;
        }
        
        if let Some(info) = self.ownership_map.get_mut(var_name) {
            info.owner_count -= 1;
        }
        
        should_free
    }

    /// Borrow a variable
    pub fn borrow(&mut self, var_name: &str) -> bool {
        if let Some(info) = self.ownership_map.get_mut(var_name) {
            if !info.is_borrowed {
                info.is_borrowed = true;
                return true;
            }
        }
        false
    }

    /// Return a borrowed variable
    pub fn return_borrow(&mut self, var_name: &str) {
        if let Some(info) = self.ownership_map.get_mut(var_name) {
            info.is_borrowed = false;
        }
    }

    /// Generate cleanup code for current scope
    pub fn generate_cleanup_code(&mut self) -> String {
        let cleanup = self.exit_scope();
        let mut code = String::new();
        
        match self.strategy {
            MemoryStrategy::Automatic => {
                for var in cleanup {
                    code.push_str(&format!("    tauraro_unref({});\n", var));
                }
            }
            MemoryStrategy::Manual => {
                for var in cleanup {
                    code.push_str(&format!("    free({});\n", var));
                }
            }
            MemoryStrategy::Arena => {
                // Arena cleanup happens at end of arena lifecycle
            }
            MemoryStrategy::Generational => {
                self.current_generation += 1;
                for var in cleanup {
                    code.push_str(&format!("    tauraro_gc_mark({});\n", var));
                }
            }
            MemoryStrategy::CopyOnWrite => {
                for var in cleanup {
                    code.push_str(&format!("    tauraro_cow_release({});\n", var));
                }
            }
        }
        
        code
    }

    /// Generate allocation code
    pub fn generate_alloc_code(&self, var_name: &str, type_info: &str, size: usize) -> String {
        match self.strategy {
            MemoryStrategy::Automatic => {
                format!("{} = tauraro_alloc({}, {});", var_name, size, type_info)
            }
            MemoryStrategy::Manual => {
                format!("{} = malloc({});", var_name, size)
            }
            MemoryStrategy::Arena => {
                format!("{} = arena_alloc({});", var_name, size)
            }
            MemoryStrategy::Generational => {
                format!("{} = gc_alloc({}, {});", var_name, size, self.current_generation)
            }
            MemoryStrategy::CopyOnWrite => {
                format!("{} = cow_alloc({});", var_name, size)
            }
        }
    }
}

/// String interning pool
pub struct StringPool {
    pool: HashMap<String, String>,
}

impl StringPool {
    pub fn new() -> Self {
        Self {
            pool: HashMap::new(),
        }
    }

    /// Intern a string (return reference to existing or insert new)
    pub fn intern(&mut self, s: String) -> String {
        self.pool.entry(s.clone())
            .or_insert_with(|| s.clone())
            .clone()
    }

    pub fn size(&self) -> usize {
        self.pool.len()
    }
}

/// Object pool for reusing allocations
pub struct ObjectPool {
    free_objects: Vec<*mut u8>,
    object_size: usize,
}

impl ObjectPool {
    pub fn new(object_size: usize, capacity: usize) -> Self {
        let mut pool = Self {
            free_objects: Vec::with_capacity(capacity),
            object_size,
        };
        
        // Pre-allocate objects
        for _ in 0..capacity {
            unsafe {
                let ptr = libc::malloc(object_size);
                if !ptr.is_null() {
                    pool.free_objects.push(ptr as *mut u8);
                }
            }
        }
        
        pool
    }

    /// Get object from pool
    pub fn get(&mut self) -> Option<*mut u8> {
        self.free_objects.pop()
    }

    /// Return object to pool
    pub fn return_object(&mut self, obj: *mut u8) {
        if self.free_objects.len() < self.free_objects.capacity() {
            unsafe {
                // Zero out object memory before reusing
                libc::memset(obj as *mut libc::c_void, 0, self.object_size);
            }
            self.free_objects.push(obj);
        } else {
            // Pool is full, free the object
            unsafe {
                libc::free(obj as *mut libc::c_void);
            }
        }
    }

    pub fn size(&self) -> usize {
        self.free_objects.len()
    }
}

impl Drop for ObjectPool {
    fn drop(&mut self) {
        for obj in self.free_objects.drain(..) {
            unsafe {
                libc::free(obj as *mut libc::c_void);
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_memory_context_creation() {
        let ctx = MemoryManagementContext::new(MemoryStrategy::Automatic);
        assert_eq!(ctx.strategy, MemoryStrategy::Automatic);
        assert_eq!(ctx.scope_depth, 0);
    }

    #[test]
    fn test_allocation_registration() {
        let mut ctx = MemoryManagementContext::new(MemoryStrategy::Automatic);
        ctx.register_allocation("x".to_string(), "int".to_string());
        
        assert!(ctx.allocations.contains("x"));
        assert_eq!(ctx.reference_counts.get("x"), Some(&1));
    }

    #[test]
    fn test_string_pool() {
        let mut pool = StringPool::new();
        let s1 = pool.intern("hello".to_string());
        let s2 = pool.intern("hello".to_string());
        
        assert_eq!(s1, s2);
        assert_eq!(pool.size(), 1);
    }
}
