//! Test file to verify that HPList is the default list implementation

use tauraro::value::Value;
use tauraro::modules::hplist::HPList;
use tauraro::builtins::{builtin_list, builtin_len};

fn main() {
    println!("Testing that HPList is the default list implementation...");
    
    // Test that builtin_list creates an HPList
    let result = builtin_list(vec![]).unwrap();
    match result {
        Value::List(_) => println!("✓ builtin_list() correctly returns HPList"),
        _ => panic!("✗ builtin_list() does not return HPList"),
    }
    
    // Test creating a list with initial values
    let mut initial_values = HPList::new();
    initial_values.append(Value::Int(1));
    initial_values.append(Value::Int(2));
    initial_values.append(Value::Int(3));
    
    let args = vec![Value::List(initial_values)];
    let result = builtin_list(args).unwrap();
    match result {
        Value::List(items) => {
            assert_eq!(items.len(), 3);
            println!("✓ builtin_list() correctly handles existing lists");
        },
        _ => panic!("✗ builtin_list() does not return HPList for existing lists"),
    }
    
    // Test that len() works with HPList
    let mut test_list = HPList::new();
    test_list.append(Value::Int(1));
    test_list.append(Value::Int(2));
    
    let len_args = vec![Value::List(test_list)];
    let len_result = builtin_len(len_args).unwrap();
    match len_result {
        Value::Int(2) => println!("✓ len() correctly works with HPList"),
        _ => panic!("✗ len() does not work correctly with HPList"),
    }
    
    println!("All tests passed! HPList is correctly implemented as the default list type.");
}