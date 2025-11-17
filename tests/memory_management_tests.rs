//! Comprehensive tests for the memory management system

use tauraro::codegen::c_transpiler::memory_management::{
    MemoryCodeGenerator, MemoryManagementContext, MemoryStrategy,
};

#[test]
fn test_automatic_memory_header_generation() {
    let gen = MemoryCodeGenerator::new(MemoryStrategy::Automatic);
    let header = gen.generate_runtime_header();

    // Check for essential automatic mode functions
    assert!(
        header.contains("tauraro_refcounted_t"),
        "Should define refcounted type"
    );
    assert!(
        header.contains("tauraro_alloc_rc"),
        "Should have alloc_rc function"
    );
    assert!(
        header.contains("tauraro_incref"),
        "Should have incref function"
    );
    assert!(
        header.contains("tauraro_decref"),
        "Should have decref function"
    );
    assert!(
        header.contains("tauraro_rc_ptr"),
        "Should have rc_ptr function"
    );

    // Check for destructor support
    assert!(
        header.contains("void (*destructor)(void*)"),
        "Should support custom destructors"
    );
    assert!(
        header.contains("tauraro_str_destructor"),
        "Should have string destructor"
    );
    assert!(
        header.contains("tauraro_array_destructor"),
        "Should have array destructor"
    );
}

#[test]
fn test_manual_memory_header_generation() {
    let gen = MemoryCodeGenerator::new(MemoryStrategy::Manual);
    let header = gen.generate_runtime_header();

    // Check for essential manual mode functions
    assert!(
        header.contains("tauraro_alloc"),
        "Should have alloc function"
    );
    assert!(header.contains("tauraro_free"), "Should have free function");
    assert!(
        header.contains("tauraro_realloc"),
        "Should have realloc function"
    );

    // Manual mode should not have refcounting
    assert!(
        !header.contains("tauraro_incref"),
        "Should not have incref in manual mode"
    );
    assert!(
        !header.contains("tauraro_decref"),
        "Should not have decref in manual mode"
    );
}

#[test]
fn test_arena_memory_header_generation() {
    let gen = MemoryCodeGenerator::new(MemoryStrategy::Arena);
    let header = gen.generate_runtime_header();

    // Check for essential arena mode structures and functions
    assert!(
        header.contains("tauraro_arena_t"),
        "Should define arena type"
    );
    assert!(
        header.contains("tauraro_arena_create"),
        "Should have arena_create function"
    );
    assert!(
        header.contains("tauraro_arena_alloc"),
        "Should have arena_alloc function"
    );
    assert!(
        header.contains("tauraro_arena_destroy"),
        "Should have arena_destroy function"
    );
    assert!(
        header.contains("tauraro_arena_reset"),
        "Should have arena_reset function"
    );

    // Check for alignment
    assert!(
        header.contains("aligned_size"),
        "Should have alignment logic"
    );
}

#[test]
fn test_automatic_allocation_generation() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Automatic);

    let alloc_code = gen.generate_allocation("myvar", "int64_t*", "sizeof(int64_t)");

    assert!(
        alloc_code.contains("myvar_rc = tauraro_alloc_rc"),
        "Should create RC wrapper"
    );
    assert!(
        alloc_code.contains("sizeof(int64_t)"),
        "Should use provided size"
    );
    assert!(
        alloc_code.contains("myvar = (int64_t*)tauraro_rc_ptr(myvar_rc)"),
        "Should get pointer from RC"
    );
}

#[test]
fn test_manual_allocation_generation() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Manual);

    let alloc_code = gen.generate_allocation("myvar", "int64_t*", "sizeof(int64_t)");

    assert!(
        alloc_code.contains("myvar = (int64_t*)tauraro_alloc"),
        "Should use tauraro_alloc"
    );
    assert!(
        alloc_code.contains("sizeof(int64_t)"),
        "Should use provided size"
    );
    assert!(
        !alloc_code.contains("_rc"),
        "Should not use reference counting"
    );
}

#[test]
fn test_arena_allocation_generation() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Arena);

    let alloc_code = gen.generate_allocation("myvar", "int64_t*", "sizeof(int64_t)");

    assert!(
        alloc_code.contains("myvar = (int64_t*)tauraro_arena_alloc"),
        "Should use arena_alloc"
    );
    assert!(alloc_code.contains("_arena"), "Should reference arena");
    assert!(
        alloc_code.contains("sizeof(int64_t)"),
        "Should use provided size"
    );
}

#[test]
fn test_automatic_deallocation_generation() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Automatic);

    // First mark as owned
    gen.context_mut().mark_owned("myvar".to_string());

    let dealloc_code = gen.generate_deallocation("myvar");

    assert!(
        dealloc_code.contains("tauraro_decref(myvar_rc)"),
        "Should decrement reference count"
    );
}

#[test]
fn test_manual_deallocation_generation() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Manual);

    // First mark as owned
    gen.context_mut().mark_owned("myvar".to_string());

    let dealloc_code = gen.generate_deallocation("myvar");

    assert!(
        dealloc_code.contains("tauraro_free(myvar)"),
        "Should explicitly free"
    );
}

#[test]
fn test_arena_deallocation_generation() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Arena);

    let dealloc_code = gen.generate_deallocation("myvar");

    assert!(
        dealloc_code.contains("Freed with arena"),
        "Should indicate arena cleanup"
    );
}

#[test]
fn test_reference_counting_context() {
    let mut ctx = MemoryManagementContext::new(MemoryStrategy::Automatic);

    // Mark variable as owned
    ctx.mark_owned("x".to_string());
    assert_eq!(
        ctx.refcount_vars.get("x"),
        Some(&1),
        "Should have refcount of 1"
    );

    // Increment reference
    ctx.incref("x");
    assert_eq!(
        ctx.refcount_vars.get("x"),
        Some(&2),
        "Should have refcount of 2"
    );

    // Decrement reference
    ctx.decref("x");
    assert_eq!(
        ctx.refcount_vars.get("x"),
        Some(&1),
        "Should have refcount of 1"
    );

    // Decrement to zero
    ctx.decref("x");
    assert!(ctx.should_free("x"), "Should free when refcount reaches 0");
}

#[test]
fn test_borrowed_references() {
    let mut ctx = MemoryManagementContext::new(MemoryStrategy::Automatic);

    ctx.mark_owned("owner".to_string());
    ctx.mark_borrowed("borrowed".to_string(), "owner".to_string());

    assert!(
        ctx.owned_variables.contains("owner"),
        "Owner should be marked as owned"
    );
    assert!(
        !ctx.owned_variables.contains("borrowed"),
        "Borrowed should not be marked as owned"
    );
    assert_eq!(
        ctx.borrowed_refs.get("borrowed"),
        Some(&"owner".to_string()),
        "Should track borrow relationship"
    );
}

#[test]
fn test_arena_allocations_tracking() {
    let mut ctx = MemoryManagementContext::new(MemoryStrategy::Arena);

    ctx.track_arena_alloc("main_arena".to_string(), "var1".to_string());
    ctx.track_arena_alloc("main_arena".to_string(), "var2".to_string());
    ctx.track_arena_alloc("temp_arena".to_string(), "temp_var".to_string());

    let main_arena_allocs = ctx.arena_allocations.get("main_arena").unwrap();
    assert_eq!(
        main_arena_allocs.len(),
        2,
        "Should track 2 allocations in main_arena"
    );
    assert!(
        main_arena_allocs.contains(&"var1".to_string()),
        "Should contain var1"
    );
    assert!(
        main_arena_allocs.contains(&"var2".to_string()),
        "Should contain var2"
    );

    let temp_arena_allocs = ctx.arena_allocations.get("temp_arena").unwrap();
    assert_eq!(
        temp_arena_allocs.len(),
        1,
        "Should track 1 allocation in temp_arena"
    );
}

#[test]
fn test_scope_cleanup_automatic() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Automatic);

    gen.context_mut().mark_owned("x".to_string());
    gen.context_mut().mark_owned("y".to_string());

    let cleanup = gen.generate_scope_cleanup(&vec!["x".to_string(), "y".to_string()]);

    assert!(
        cleanup.contains("Automatic cleanup"),
        "Should have cleanup comment"
    );
    assert!(cleanup.contains("tauraro_decref(x_rc)"), "Should decref x");
    assert!(cleanup.contains("tauraro_decref(y_rc)"), "Should decref y");
}

#[test]
fn test_scope_cleanup_manual() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Manual);

    gen.context_mut().mark_owned("x".to_string());
    gen.context_mut().mark_owned("y".to_string());

    let cleanup = gen.generate_scope_cleanup(&vec!["x".to_string(), "y".to_string()]);

    assert!(
        cleanup.contains("Manual cleanup"),
        "Should have cleanup comment"
    );
    assert!(cleanup.contains("tauraro_free(x)"), "Should free x");
    assert!(cleanup.contains("tauraro_free(y)"), "Should free y");
}

#[test]
fn test_scope_cleanup_arena() {
    let gen = MemoryCodeGenerator::new(MemoryStrategy::Arena);

    let cleanup = gen.generate_scope_cleanup(&vec!["x".to_string(), "y".to_string()]);

    assert!(
        cleanup.contains("Arena cleanup"),
        "Should have arena cleanup comment"
    );
    assert!(
        cleanup.contains("tauraro_arena_reset"),
        "Should reset arena"
    );
}

#[test]
fn test_incref_generation() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Automatic);

    let incref_code = gen.generate_incref("myvar");

    assert!(
        incref_code.contains("tauraro_incref(myvar_rc)"),
        "Should generate incref call"
    );
}

#[test]
fn test_incref_not_generated_for_manual() {
    let mut gen = MemoryCodeGenerator::new(MemoryStrategy::Manual);

    let incref_code = gen.generate_incref("myvar");

    assert!(
        incref_code.is_empty(),
        "Manual mode should not generate incref"
    );
}

#[test]
fn test_multiple_strategies_in_sequence() {
    // Test that we can create multiple generators with different strategies
    let auto_gen = MemoryCodeGenerator::new(MemoryStrategy::Automatic);
    let manual_gen = MemoryCodeGenerator::new(MemoryStrategy::Manual);
    let arena_gen = MemoryCodeGenerator::new(MemoryStrategy::Arena);

    let auto_header = auto_gen.generate_runtime_header();
    let manual_header = manual_gen.generate_runtime_header();
    let arena_header = arena_gen.generate_runtime_header();

    // Each should have its own unique functions
    assert!(auto_header.contains("tauraro_incref"));
    assert!(manual_header.contains("tauraro_alloc"));
    assert!(arena_header.contains("tauraro_arena_create"));

    // But not others' functions
    assert!(!manual_header.contains("tauraro_incref"));
    assert!(!arena_header.contains("tauraro_alloc"));
}

#[test]
fn test_memory_strategy_default() {
    let strategy = MemoryStrategy::default();
    assert_eq!(
        strategy,
        MemoryStrategy::Automatic,
        "Default strategy should be Automatic"
    );
}

#[test]
fn test_arena_block_chaining() {
    let gen = MemoryCodeGenerator::new(MemoryStrategy::Arena);
    let header = gen.generate_runtime_header();

    // Check that arena supports block chaining
    assert!(
        header.contains("struct tauraro_arena* next"),
        "Should have next pointer for chaining"
    );
    assert!(
        header.contains("new_block->next = arena->next"),
        "Should chain new blocks"
    );
}

#[test]
fn test_arena_alignment() {
    let gen = MemoryCodeGenerator::new(MemoryStrategy::Arena);
    let header = gen.generate_runtime_header();

    // Check that arena performs 8-byte alignment
    assert!(
        header.contains("(size + 7) & ~7"),
        "Should have 8-byte alignment logic"
    );
}
