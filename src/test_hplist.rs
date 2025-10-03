//! Test file for high-performance list implementation

use tauraro::modules::hplist::HPList;
use tauraro::value::Value;

fn main() {
    println!("Testing high-performance list implementation...");
    
    // Create a new high-performance list
    let mut list = HPList::new();
    
    // Test basic operations
    println!("Testing basic operations...");
    list.append(Value::Int(1));
    list.append(Value::Int(2));
    list.append(Value::Int(3));
    
    assert_eq!(list.len(), 3);
    assert_eq!(list.get(0), Some(&Value::Int(1)));
    assert_eq!(list.get(1), Some(&Value::Int(2)));
    assert_eq!(list.get(2), Some(&Value::Int(3)));
    
    // Test negative indexing
    println!("Testing negative indexing...");
    assert_eq!(list.get(-1), Some(&Value::Int(3)));
    assert_eq!(list.get(-3), Some(&Value::Int(1)));
    
    // Test insertion
    println!("Testing insertion...");
    list.insert(1, Value::Int(99)).unwrap();
    assert_eq!(list.len(), 4);
    assert_eq!(list.get(1), Some(&Value::Int(99)));
    
    // Test removal
    println!("Testing removal...");
    list.remove(&Value::Int(99)).unwrap();
    assert_eq!(list.len(), 3);
    assert_eq!(list.get(1), Some(&Value::Int(2)));
    
    // Test popping
    println!("Testing popping...");
    let popped = list.pop();
    assert_eq!(popped, Some(Value::Int(3)));
    assert_eq!(list.len(), 2);
    
    // Test counting
    println!("Testing counting...");
    list.append(Value::Int(1));
    list.append(Value::Int(1));
    assert_eq!(list.count(&Value::Int(1)), 3);
    
    // Test indexing
    println!("Testing indexing...");
    assert_eq!(list.index(&Value::Int(1), None, None).unwrap(), 0);
    assert_eq!(list.index(&Value::Int(2), Some(1), None).unwrap(), 1);
    
    // Test sorting
    println!("Testing sorting...");
    let mut unsorted_list = HPList::new();
    unsorted_list.append(Value::Int(3));
    unsorted_list.append(Value::Int(1));
    unsorted_list.append(Value::Int(2));
    unsorted_list.sort();
    assert_eq!(unsorted_list.get(0), Some(&Value::Int(1)));
    assert_eq!(unsorted_list.get(1), Some(&Value::Int(2)));
    assert_eq!(unsorted_list.get(2), Some(&Value::Int(3)));
    
    // Test reversing
    println!("Testing reversing...");
    unsorted_list.reverse();
    assert_eq!(unsorted_list.get(0), Some(&Value::Int(3)));
    assert_eq!(unsorted_list.get(1), Some(&Value::Int(2)));
    assert_eq!(unsorted_list.get(2), Some(&Value::Int(1)));
    
    // Test slicing
    println!("Testing slicing...");
    let slice = list.slice(Some(0), Some(2), None).unwrap();
    assert_eq!(slice.len(), 2);
    assert_eq!(slice[0], Value::Int(1));
    assert_eq!(slice[1], Value::Int(2));
    
    println!("All tests passed! High-performance list implementation is working correctly.");
}