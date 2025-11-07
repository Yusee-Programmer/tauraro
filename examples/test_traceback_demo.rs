// Demo program to test the new traceback formatter
// Run with: cargo run --example test_traceback_demo

use tauraro::traceback::*;

fn main() {
    println!("=== Tauraro Exception Traceback Demo ===\n");

    // Example 1: SyntaxError with source context
    println!("1. SyntaxError with caret:");
    println!("{}", "-".repeat(60));
    let syntax_err = create_syntax_error(
        "invalid syntax".to_string(),
        "test.py".to_string(),
        3,
        8,
        Some("if x == :".to_string()),
    );
    println!("{}\n", syntax_err);

    // Example 2: NameError with traceback
    println!("2. NameError with full traceback:");
    println!("{}", "-".repeat(60));
    let mut name_err = create_name_error(
        "undefined_variable".to_string(),
        "main.py".to_string(),
        15,
        10,
        Some("    result = undefined_variable + 5".to_string()),
    );
    name_err.add_frame(TracebackFrame::new(
        "utils.py".to_string(),
        42,
        5,
        "process_data".to_string(),
    ).with_source("    compute_result()".to_string()));
    name_err.add_frame(TracebackFrame::new(
        "main.py".to_string(),
        10,
        0,
        "<module>".to_string(),
    ).with_source("process_data(items)".to_string()));
    println!("{}\n", name_err);

    // Example 3: TypeError
    println!("3. TypeError:");
    println!("{}", "-".repeat(60));
    let type_err = create_type_error(
        "unsupported operand type(s) for +: 'int' and 'str'".to_string(),
        "calculator.py".to_string(),
        8,
        15,
        Some("    total = count + name".to_string()),
    );
    println!("{}\n", type_err);

    // Example 4: ZeroDivisionError
    println!("4. ZeroDivisionError:");
    println!("{}", "-".repeat(60));
    let zero_div = create_zero_division_error(
        "math_ops.py".to_string(),
        23,
        20,
        Some("    result = numerator / denominator".to_string()),
    );
    println!("{}\n", zero_div);

    // Example 5: IndexError
    println!("5. IndexError:");
    println!("{}", "-".repeat(60));
    let index_err = create_index_error(
        "list index out of range".to_string(),
        "data.py".to_string(),
        12,
        16,
        Some("    item = items[index]".to_string()),
    );
    println!("{}\n", index_err);

    // Example 6: KeyError
    println!("6. KeyError:");
    println!("{}", "-".repeat(60));
    let key_err = create_key_error(
        "missing_key".to_string(),
        "config.py".to_string(),
        7,
        18,
        Some("    value = config['missing_key']".to_string()),
    );
    println!("{}\n", key_err);

    // Example 7: AttributeError
    println!("7. AttributeError:");
    println!("{}", "-".repeat(60));
    let attr_err = create_attribute_error(
        "NoneType".to_string(),
        "value".to_string(),
        "app.py".to_string(),
        45,
        10,
        Some("    result.value".to_string()),
    );
    println!("{}\n", attr_err);

    // Example 8: Complex traceback with multiple frames
    println!("8. Complex traceback with multiple frames:");
    println!("{}", "-".repeat(60));
    let mut complex_err = create_runtime_error(
        "Something went wrong in nested function call".to_string(),
        "core.py".to_string(),
        100,
        8,
        Some("        raise RuntimeError('Something went wrong in nested function call')".to_string()),
    );
    complex_err.add_frame(TracebackFrame::new(
        "helpers.py".to_string(),
        25,
        4,
        "helper_function".to_string(),
    ).with_source("    core.process()".to_string()));
    complex_err.add_frame(TracebackFrame::new(
        "utils.py".to_string(),
        50,
        4,
        "utility_method".to_string(),
    ).with_source("    helpers.helper_function()".to_string()));
    complex_err.add_frame(TracebackFrame::new(
        "main.py".to_string(),
        15,
        0,
        "<module>".to_string(),
    ).with_source("utils.utility_method()".to_string()));
    println!("{}\n", complex_err);

    // Example 9: Recursion error
    println!("9. RecursionError:");
    println!("{}", "-".repeat(60));
    let mut recursion_err = create_recursion_error(
        "factorial.py".to_string(),
        5,
        15,
        Some("    return n * factorial(n - 1)".to_string()),
    );
    // Show only last few frames of deep recursion
    for i in 0..3 {
        recursion_err.add_frame(TracebackFrame::new(
            "factorial.py".to_string(),
            5,
            15,
            "factorial".to_string(),
        ).with_source("    return n * factorial(n - 1)".to_string()));
    }
    println!("{}\n", recursion_err);

    println!("=== Demo Complete ===");
    println!("\nNote: Colors are displayed when running in a terminal!");
    println!("Try: cargo run --example test_traceback_demo | cat");
    println!("(The output will be uncolored when piped)");
}
