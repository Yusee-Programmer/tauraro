//! Memory Management Integration for VM Runtime
//!
//! Provides manual and arena memory management capabilities for the VM,
//! mirroring the C transpiler's memory management system.

use crate::value::Value;
use std::collections::{HashMap, HashSet};
use std::rc::Rc;
use std::cell::RefCell;

/// Memory management strategy for VM execution
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum VMMemoryStrategy {
    /// Automatic reference counting (default, native Rust Rc)
    Automatic,
    /// Manual allocation tracking
    Manual,
    /// Arena-based bulk allocation
    Arena,
}

impl Default for VMMemoryStrategy {
    fn default() -> Self {
        VMMemoryStrategy::Automatic
    }
}

/// Represents a manually allocated buffer in the VM
#[derive(Debug, Clone)]
pub struct ManagedBuffer {
    pub data: Vec<u8>,
    pub size: usize,
    pub freed: bool,
}

impl ManagedBuffer {
    pub fn new(size: usize) -> Self {
        Self {
            data: vec![0; size],
            size,
            freed: false,
        }
    }

    pub fn is_freed(&self) -> bool {
        self.freed
    }

    pub fn free(&mut self) {
        self.freed = true;
        self.data.clear();
    }
}

/// Arena allocator for VM
#[derive(Debug, Clone)]
pub struct VMArena {
    pub name: String,
    pub buffers: Vec<Rc<RefCell<ManagedBuffer>>>,
    pub total_allocated: usize,
}

impl VMArena {
    pub fn new(name: String) -> Self {
        Self {
            name,
            buffers: Vec::new(),
            total_allocated: 0,
        }
    }

    pub fn allocate(&mut self, size: usize) -> Rc<RefCell<ManagedBuffer>> {
        let buffer = Rc::new(RefCell::new(ManagedBuffer::new(size)));
        self.buffers.push(buffer.clone());
        self.total_allocated += size;
        buffer
    }

    pub fn reset(&mut self) {
        for buffer in &self.buffers {
            buffer.borrow_mut().free();
        }
        self.buffers.clear();
        self.total_allocated = 0;
    }

    pub fn destroy(mut self) {
        self.reset();
    }
}

/// VM Memory Management Context
pub struct VMMemoryContext {
    /// Current memory strategy
    pub strategy: VMMemoryStrategy,
    /// Manually allocated buffers
    pub manual_buffers: HashMap<usize, Rc<RefCell<ManagedBuffer>>>,
    /// Arenas
    pub arenas: HashMap<String, VMArena>,
    /// Current arena for allocation
    pub current_arena: Option<String>,
    /// Buffer ID counter
    next_buffer_id: usize,
}

impl VMMemoryContext {
    pub fn new(strategy: VMMemoryStrategy) -> Self {
        Self {
            strategy,
            manual_buffers: HashMap::new(),
            arenas: HashMap::new(),
            current_arena: None,
            next_buffer_id: 1,
        }
    }

    /// Allocate a buffer
    pub fn allocate(&mut self, size: usize) -> (usize, Rc<RefCell<ManagedBuffer>>) {
        match self.strategy {
            VMMemoryStrategy::Automatic | VMMemoryStrategy::Manual => {
                let id = self.next_buffer_id;
                self.next_buffer_id += 1;
                let buffer = Rc::new(RefCell::new(ManagedBuffer::new(size)));
                self.manual_buffers.insert(id, buffer.clone());
                (id, buffer)
            }
            VMMemoryStrategy::Arena => {
                let arena_name = self.current_arena.clone()
                    .unwrap_or_else(|| "_default_arena".to_string());

                let arena = self.arenas.entry(arena_name.clone())
                    .or_insert_with(|| VMArena::new(arena_name));

                let id = self.next_buffer_id;
                self.next_buffer_id += 1;
                let buffer = arena.allocate(size);
                (id, buffer)
            }
        }
    }

    /// Free a buffer (manual mode only)
    pub fn free(&mut self, buffer_id: usize) -> anyhow::Result<()> {
        match self.strategy {
            VMMemoryStrategy::Manual => {
                if let Some(buffer) = self.manual_buffers.get(&buffer_id) {
                    buffer.borrow_mut().free();
                    self.manual_buffers.remove(&buffer_id);
                    Ok(())
                } else {
                    Err(anyhow::anyhow!("Invalid buffer ID: {}", buffer_id))
                }
            }
            VMMemoryStrategy::Automatic => {
                // In automatic mode, free is a no-op (Rust handles it)
                Ok(())
            }
            VMMemoryStrategy::Arena => {
                // Arena mode frees in bulk
                Err(anyhow::anyhow!("Cannot free individual buffers in arena mode"))
            }
        }
    }

    /// Create a new arena
    pub fn create_arena(&mut self, name: String) {
        self.arenas.insert(name.clone(), VMArena::new(name));
    }

    /// Set current arena for allocations
    pub fn set_current_arena(&mut self, name: String) {
        self.current_arena = Some(name);
    }

    /// Reset an arena
    pub fn reset_arena(&mut self, name: &str) -> anyhow::Result<()> {
        if let Some(arena) = self.arenas.get_mut(name) {
            arena.reset();
            Ok(())
        } else {
            Err(anyhow::anyhow!("Arena not found: {}", name))
        }
    }

    /// Destroy an arena
    pub fn destroy_arena(&mut self, name: &str) -> anyhow::Result<()> {
        if let Some(arena) = self.arenas.remove(name) {
            arena.destroy();
            Ok(())
        } else {
            Err(anyhow::anyhow!("Arena not found: {}", name))
        }
    }

    /// Get memory statistics
    pub fn get_stats(&self) -> MemoryStats {
        let manual_count = self.manual_buffers.len();
        let manual_size: usize = self.manual_buffers.values()
            .map(|b| b.borrow().size)
            .sum();

        let arena_count = self.arenas.len();
        let arena_size: usize = self.arenas.values()
            .map(|a| a.total_allocated)
            .sum();

        MemoryStats {
            strategy: self.strategy,
            manual_buffers: manual_count,
            manual_bytes: manual_size,
            arenas: arena_count,
            arena_bytes: arena_size,
        }
    }
}

impl Default for VMMemoryContext {
    fn default() -> Self {
        Self::new(VMMemoryStrategy::default())
    }
}

/// Memory statistics
#[derive(Debug, Clone)]
pub struct MemoryStats {
    pub strategy: VMMemoryStrategy,
    pub manual_buffers: usize,
    pub manual_bytes: usize,
    pub arenas: usize,
    pub arena_bytes: usize,
}

impl std::fmt::Display for MemoryStats {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "Memory Strategy: {:?}\n", self.strategy)?;
        write!(f, "Manual Buffers: {} ({} bytes)\n", self.manual_buffers, self.manual_bytes)?;
        write!(f, "Arenas: {} ({} bytes)", self.arenas, self.arena_bytes)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_manual_allocation() {
        let mut ctx = VMMemoryContext::new(VMMemoryStrategy::Manual);

        let (id, buffer) = ctx.allocate(1024);
        assert_eq!(buffer.borrow().size, 1024);
        assert!(!buffer.borrow().is_freed());

        ctx.free(id).unwrap();
        assert!(buffer.borrow().is_freed());
    }

    #[test]
    fn test_arena_allocation() {
        let mut ctx = VMMemoryContext::new(VMMemoryStrategy::Arena);
        ctx.create_arena("test_arena".to_string());
        ctx.set_current_arena("test_arena".to_string());

        let (_, buffer1) = ctx.allocate(512);
        let (_, buffer2) = ctx.allocate(256);

        assert_eq!(buffer1.borrow().size, 512);
        assert_eq!(buffer2.borrow().size, 256);

        let stats = ctx.get_stats();
        assert_eq!(stats.arena_bytes, 768);
    }

    #[test]
    fn test_arena_reset() {
        let mut ctx = VMMemoryContext::new(VMMemoryStrategy::Arena);
        ctx.create_arena("test_arena".to_string());
        ctx.set_current_arena("test_arena".to_string());

        let (_, buffer) = ctx.allocate(1024);
        assert!(!buffer.borrow().is_freed());

        ctx.reset_arena("test_arena").unwrap();
        assert!(buffer.borrow().is_freed());
    }
}
