//! Advanced Memory Management System for Tauraro
//!
//! Supports three memory management strategies:
//! 1. Automatic (default) - Reference counting with optional cycle detection
//! 2. Manual - Explicit allocation and deallocation
//! 3. Arena - Region-based bulk allocation and deallocation

use std::collections::{HashMap, HashSet};
use crate::ast::*;

/// Memory management strategy
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MemoryStrategy {
    /// Automatic reference counting (default, like Python)
    Automatic,
    /// Manual allocation and deallocation
    Manual,
    /// Arena/region-based allocation
    Arena,
}

impl Default for MemoryStrategy {
    fn default() -> Self {
        MemoryStrategy::Automatic
    }
}

/// Memory management context for tracking allocations and ownership
pub struct MemoryManagementContext {
    /// Current memory strategy
    pub strategy: MemoryStrategy,
    /// Track which variables own their memory
    pub owned_variables: HashSet<String>,
    /// Track borrowed references (variable -> owner)
    pub borrowed_refs: HashMap<String, String>,
    /// Track arena allocations
    pub arena_allocations: HashMap<String, Vec<String>>,
    /// Reference count tracking for automatic mode
    pub refcount_vars: HashMap<String, usize>,
}

impl MemoryManagementContext {
    pub fn new(strategy: MemoryStrategy) -> Self {
        Self {
            strategy,
            owned_variables: HashSet::new(),
            borrowed_refs: HashMap::new(),
            arena_allocations: HashMap::new(),
            refcount_vars: HashMap::new(),
        }
    }

    /// Mark a variable as owning its memory
    pub fn mark_owned(&mut self, var: String) {
        self.owned_variables.insert(var.clone());
        if self.strategy == MemoryStrategy::Automatic {
            self.refcount_vars.insert(var, 1);
        }
    }

    /// Mark a variable as a borrowed reference
    pub fn mark_borrowed(&mut self, var: String, owner: String) {
        self.borrowed_refs.insert(var, owner);
    }

    /// Track an arena allocation
    pub fn track_arena_alloc(&mut self, arena: String, var: String) {
        self.arena_allocations
            .entry(arena)
            .or_insert_with(Vec::new)
            .push(var);
    }

    /// Increment reference count (automatic mode)
    pub fn incref(&mut self, var: &str) {
        if self.strategy == MemoryStrategy::Automatic {
            *self.refcount_vars.entry(var.to_string()).or_insert(0) += 1;
        }
    }

    /// Decrement reference count (automatic mode)
    pub fn decref(&mut self, var: &str) {
        if self.strategy == MemoryStrategy::Automatic {
            if let Some(count) = self.refcount_vars.get_mut(var) {
                if *count > 0 {
                    *count -= 1;
                }
            }
        }
    }

    /// Check if variable should be freed
    pub fn should_free(&self, var: &str) -> bool {
        match self.strategy {
            MemoryStrategy::Automatic => {
                self.refcount_vars.get(var).map_or(false, |&count| count == 0)
            }
            MemoryStrategy::Manual => self.owned_variables.contains(var),
            MemoryStrategy::Arena => false, // Arena frees all at once
        }
    }
}

/// Generate C code for memory management
pub struct MemoryCodeGenerator {
    context: MemoryManagementContext,
}

impl MemoryCodeGenerator {
    pub fn new(strategy: MemoryStrategy) -> Self {
        Self {
            context: MemoryManagementContext::new(strategy),
        }
    }

    /// Generate memory management runtime header
    pub fn generate_runtime_header(&self) -> String {
        let mut code = String::new();

        code.push_str("// Tauraro Memory Management Runtime\n\n");

        // Common structures
        code.push_str(r#"
// Reference counting header for automatic memory management
typedef struct {
    void* ptr;
    size_t refcount;
    size_t size;
    void (*destructor)(void*);
} tauraro_refcounted_t;

// Arena allocator for region-based memory management
typedef struct tauraro_arena {
    char* memory;
    size_t size;
    size_t offset;
    struct tauraro_arena* next;
} tauraro_arena_t;

// Forward declarations for native container types
typedef struct tauraro_native_list tauraro_native_list_t;
typedef struct tauraro_native_dict tauraro_native_dict_t;
typedef struct tauraro_dict_entry tauraro_dict_entry_t;

"#);

        // Strategy-specific functions
        match self.context.strategy {
            MemoryStrategy::Automatic => {
                code.push_str(&self.generate_automatic_functions());
            }
            MemoryStrategy::Manual => {
                code.push_str(&self.generate_manual_functions());
            }
            MemoryStrategy::Arena => {
                code.push_str(&self.generate_arena_functions());
            }
        }

        code
    }

    fn generate_automatic_functions(&self) -> String {
        r#"
// Automatic Memory Management Functions

// Create a reference-counted object
static inline tauraro_refcounted_t* tauraro_alloc_rc(size_t size, void (*destructor)(void*)) {
    tauraro_refcounted_t* rc = (tauraro_refcounted_t*)malloc(sizeof(tauraro_refcounted_t));
    rc->ptr = malloc(size);
    rc->refcount = 1;
    rc->size = size;
    rc->destructor = destructor;
    return rc;
}

// Increment reference count
static inline void tauraro_incref(tauraro_refcounted_t* rc) {
    if (rc) {
        rc->refcount++;
    }
}

// Decrement reference count and free if zero
static inline void tauraro_decref(tauraro_refcounted_t* rc) {
    if (rc && --rc->refcount == 0) {
        if (rc->destructor) {
            rc->destructor(rc->ptr);
        }
        free(rc->ptr);
        free(rc);
    }
}

// Get pointer from reference-counted object
static inline void* tauraro_rc_ptr(tauraro_refcounted_t* rc) {
    return rc ? rc->ptr : NULL;
}

// String destructor for automatic mode
static inline void tauraro_str_destructor(void* ptr) {
    // Strings allocated with strdup/malloc
    free(ptr);
}

// Array destructor for automatic mode
static inline void tauraro_array_destructor(void* ptr) {
    // Simple free for arrays
    free(ptr);
}

"#.to_string()
    }

    fn generate_manual_functions(&self) -> String {
        r#"
// Manual Memory Management Functions

// Manual allocation (user must call free)
static inline void* tauraro_alloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Tauraro: Memory allocation failed\n");
        exit(1);
    }
    return ptr;
}

// Manual deallocation
static inline void tauraro_free(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}

// Reallocate memory
static inline void* tauraro_realloc(void* ptr, size_t new_size) {
    void* new_ptr = realloc(ptr, new_size);
    if (!new_ptr && new_size > 0) {
        fprintf(stderr, "Tauraro: Memory reallocation failed\n");
        exit(1);
    }
    return new_ptr;
}

"#.to_string()
    }

    fn generate_arena_functions(&self) -> String {
        r#"
// Arena Memory Management Functions

// Create a new arena
static inline tauraro_arena_t* tauraro_arena_create(size_t initial_size) {
    tauraro_arena_t* arena = (tauraro_arena_t*)malloc(sizeof(tauraro_arena_t));
    arena->memory = (char*)malloc(initial_size);
    arena->size = initial_size;
    arena->offset = 0;
    arena->next = NULL;
    return arena;
}

// Allocate from arena
static inline void* tauraro_arena_alloc(tauraro_arena_t* arena, size_t size) {
    // Align to 8 bytes
    size_t aligned_size = (size + 7) & ~7;

    // Check if we need a new block
    if (arena->offset + aligned_size > arena->size) {
        // Allocate new block
        size_t new_size = arena->size * 2;
        if (new_size < aligned_size) {
            new_size = aligned_size * 2;
        }

        tauraro_arena_t* new_block = (tauraro_arena_t*)malloc(sizeof(tauraro_arena_t));
        new_block->memory = (char*)malloc(new_size);
        new_block->size = new_size;
        new_block->offset = 0;
        new_block->next = arena->next;
        arena->next = new_block;

        arena = new_block;
    }

    void* ptr = arena->memory + arena->offset;
    arena->offset += aligned_size;
    return ptr;
}

// Free entire arena
static inline void tauraro_arena_destroy(tauraro_arena_t* arena) {
    while (arena) {
        tauraro_arena_t* next = arena->next;
        free(arena->memory);
        free(arena);
        arena = next;
    }
}

// Reset arena (keep memory, reset offset)
static inline void tauraro_arena_reset(tauraro_arena_t* arena) {
    arena->offset = 0;
    // Free additional blocks
    if (arena->next) {
        tauraro_arena_destroy(arena->next);
        arena->next = NULL;
    }
}

"#.to_string()
    }

    /// Generate allocation code for a variable
    pub fn generate_allocation(&mut self, var_name: &str, type_name: &str, size_expr: &str) -> String {
        match self.context.strategy {
            MemoryStrategy::Automatic => {
                self.context.mark_owned(var_name.to_string());
                format!(
                    "tauraro_refcounted_t* {}_rc = tauraro_alloc_rc({}, NULL);\n    {} = ({})tauraro_rc_ptr({}_rc);",
                    var_name, size_expr, var_name, type_name, var_name
                )
            }
            MemoryStrategy::Manual => {
                self.context.mark_owned(var_name.to_string());
                format!("{} = ({})tauraro_alloc({});", var_name, type_name, size_expr)
            }
            MemoryStrategy::Arena => {
                format!("{} = ({})tauraro_arena_alloc(_arena, {});", var_name, type_name, size_expr)
            }
        }
    }

    /// Generate deallocation code
    pub fn generate_deallocation(&mut self, var_name: &str) -> String {
        match self.context.strategy {
            MemoryStrategy::Automatic => {
                self.context.decref(var_name);
                format!("tauraro_decref({}_rc);", var_name)
            }
            MemoryStrategy::Manual => {
                format!("tauraro_free({});", var_name)
            }
            MemoryStrategy::Arena => {
                // Arena frees everything at once
                String::from("// Freed with arena")
            }
        }
    }

    /// Generate reference increment code (for assignments)
    pub fn generate_incref(&mut self, var_name: &str) -> String {
        match self.context.strategy {
            MemoryStrategy::Automatic => {
                self.context.incref(var_name);
                format!("tauraro_incref({}_rc);", var_name)
            }
            _ => String::new(),
        }
    }

    /// Generate scope cleanup code
    pub fn generate_scope_cleanup(&self, variables: &[String]) -> String {
        let mut code = String::new();

        match self.context.strategy {
            MemoryStrategy::Automatic => {
                code.push_str("    // Automatic cleanup (decref)\n");
                for var in variables {
                    if self.context.owned_variables.contains(var) {
                        code.push_str(&format!("    tauraro_decref({}_rc);\n", var));
                    }
                }
            }
            MemoryStrategy::Manual => {
                code.push_str("    // Manual cleanup (explicit free)\n");
                for var in variables {
                    if self.context.owned_variables.contains(var) {
                        code.push_str(&format!("    tauraro_free({});\n", var));
                    }
                }
            }
            MemoryStrategy::Arena => {
                code.push_str("    // Arena cleanup at scope end\n");
                code.push_str("    tauraro_arena_reset(_arena);\n");
            }
        }

        code
    }

    pub fn context_mut(&mut self) -> &mut MemoryManagementContext {
        &mut self.context
    }

    pub fn context(&self) -> &MemoryManagementContext {
        &self.context
    }
}

/// Annotations for memory management in source code
#[derive(Debug, Clone, PartialEq)]
pub enum MemoryAnnotation {
    /// Use automatic memory management for this scope
    Auto,
    /// Use manual memory management for this scope
    Manual,
    /// Use arena allocation for this scope
    Arena,
    /// Mark as owned (takes ownership)
    Owned,
    /// Mark as borrowed (does not own)
    Borrowed,
    /// Explicit free
    Free,
}

impl MemoryAnnotation {
    pub fn from_decorator(name: &str) -> Option<Self> {
        match name {
            "auto_memory" => Some(MemoryAnnotation::Auto),
            "manual_memory" => Some(MemoryAnnotation::Manual),
            "arena_memory" => Some(MemoryAnnotation::Arena),
            "owned" => Some(MemoryAnnotation::Owned),
            "borrowed" => Some(MemoryAnnotation::Borrowed),
            _ => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_automatic_memory_management() {
        let gen = MemoryCodeGenerator::new(MemoryStrategy::Automatic);
        let header = gen.generate_runtime_header();

        assert!(header.contains("tauraro_incref"));
        assert!(header.contains("tauraro_decref"));
        assert!(header.contains("tauraro_refcounted_t"));
    }

    #[test]
    fn test_manual_memory_management() {
        let gen = MemoryCodeGenerator::new(MemoryStrategy::Manual);
        let header = gen.generate_runtime_header();

        assert!(header.contains("tauraro_alloc"));
        assert!(header.contains("tauraro_free"));
    }

    #[test]
    fn test_arena_memory_management() {
        let gen = MemoryCodeGenerator::new(MemoryStrategy::Arena);
        let header = gen.generate_runtime_header();

        assert!(header.contains("tauraro_arena_create"));
        assert!(header.contains("tauraro_arena_alloc"));
        assert!(header.contains("tauraro_arena_destroy"));
    }

    #[test]
    fn test_refcount_tracking() {
        let mut ctx = MemoryManagementContext::new(MemoryStrategy::Automatic);

        ctx.mark_owned("x".to_string());
        assert_eq!(ctx.refcount_vars.get("x"), Some(&1));

        ctx.incref("x");
        assert_eq!(ctx.refcount_vars.get("x"), Some(&2));

        ctx.decref("x");
        assert_eq!(ctx.refcount_vars.get("x"), Some(&1));

        ctx.decref("x");
        assert!(ctx.should_free("x"));
    }
}
