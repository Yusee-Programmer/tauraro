//! Tests for the high-performance list implementation

use tauraro::modules::hplist::HPList;
use tauraro::value::Value;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_basic_operations() {
        let mut list = HPList::new();
        assert_eq!(list.len(), 0);
        assert!(list.is_empty());

        // Test append
        list.append(Value::Int(1));
        list.append(Value::Int(2));
        list.append(Value::Int(3));
        
        assert_eq!(list.len(), 3);
        assert!(!list.is_empty());
        assert_eq!(list.get(0), Some(&Value::Int(1)));
        assert_eq!(list.get(1), Some(&Value::Int(2)));
        assert_eq!(list.get(2), Some(&Value::Int(3)));
    }

    #[test]
    fn test_negative_indexing() {
        let mut list = HPList::new();
        list.append(Value::Int(10));
        list.append(Value::Int(20));
        list.append(Value::Int(30));
        
        assert_eq!(list.get(0), Some(&Value::Int(10)));
        assert_eq!(list.get(-1), Some(&Value::Int(30)));
        assert_eq!(list.get(-3), Some(&Value::Int(10)));
        assert_eq!(list.get(3), None);
        assert_eq!(list.get(-4), None);
    }

    #[test]
    fn test_insert_and_remove() {
        let mut list = HPList::new();
        list.append(Value::Int(1));
        list.append(Value::Int(3));
        
        // Insert in the middle
        list.insert(1, Value::Int(2)).unwrap();
        assert_eq!(list.len(), 3);
        assert_eq!(list.get(0), Some(&Value::Int(1)));
        assert_eq!(list.get(1), Some(&Value::Int(2)));
        assert_eq!(list.get(2), Some(&Value::Int(3)));
        
        // Remove from middle
        list.remove(&Value::Int(2)).unwrap();
        assert_eq!(list.len(), 2);
        assert_eq!(list.get(0), Some(&Value::Int(1)));
        assert_eq!(list.get(1), Some(&Value::Int(3)));
    }

    #[test]
    fn test_pop_operations() {
        let mut list = HPList::new();
        list.append(Value::Int(1));
        list.append(Value::Int(2));
        list.append(Value::Int(3));
        
        // Pop from end
        assert_eq!(list.pop(), Some(Value::Int(3)));
        assert_eq!(list.len(), 2);
        
        // Pop from specific index
        assert_eq!(list.pop_at(0).unwrap(), Value::Int(1));
        assert_eq!(list.len(), 1);
        assert_eq!(list.get(0), Some(&Value::Int(2)));
    }

    #[test]
    fn test_count_and_index() {
        let mut list = HPList::new();
        list.append(Value::Int(1));
        list.append(Value::Int(2));
        list.append(Value::Int(1));
        list.append(Value::Int(3));
        list.append(Value::Int(1));
        
        // Test count
        assert_eq!(list.count(&Value::Int(1)), 3);
        assert_eq!(list.count(&Value::Int(2)), 1);
        assert_eq!(list.count(&Value::Int(4)), 0);
        
        // Test index
        assert_eq!(list.index(&Value::Int(1), None, None).unwrap(), 0);
        assert_eq!(list.index(&Value::Int(2), None, None).unwrap(), 1);
        assert_eq!(list.index(&Value::Int(3), None, None).unwrap(), 3);
        
        // Test index with start parameter
        assert_eq!(list.index(&Value::Int(1), Some(1), None).unwrap(), 2);
        assert_eq!(list.index(&Value::Int(1), Some(3), None).unwrap(), 4);
    }

    #[test]
    fn test_sort_and_reverse() {
        let mut list = HPList::new();
        list.append(Value::Int(3));
        list.append(Value::Int(1));
        list.append(Value::Int(4));
        list.append(Value::Int(1));
        list.append(Value::Int(5));
        
        // Test sort
        list.sort();
        assert_eq!(list.get(0), Some(&Value::Int(1)));
        assert_eq!(list.get(1), Some(&Value::Int(1)));
        assert_eq!(list.get(2), Some(&Value::Int(3)));
        assert_eq!(list.get(3), Some(&Value::Int(4)));
        assert_eq!(list.get(4), Some(&Value::Int(5)));
        
        // Test reverse
        list.reverse();
        assert_eq!(list.get(0), Some(&Value::Int(5)));
        assert_eq!(list.get(1), Some(&Value::Int(4)));
        assert_eq!(list.get(2), Some(&Value::Int(3)));
        assert_eq!(list.get(3), Some(&Value::Int(1)));
        assert_eq!(list.get(4), Some(&Value::Int(1)));
    }

    #[test]
    fn test_slice_operations() {
        let mut list = HPList::new();
        for i in 0..10 {
            list.append(Value::Int(i));
        }
        
        // Test basic slice
        let slice = list.slice(Some(2), Some(5), None).unwrap();
        assert_eq!(slice.len(), 3);
        assert_eq!(slice[0], Value::Int(2));
        assert_eq!(slice[1], Value::Int(3));
        assert_eq!(slice[2], Value::Int(4));
        
        // Test step slice
        let slice = list.slice(Some(0), Some(10), Some(2)).unwrap();
        assert_eq!(slice.len(), 5);
        assert_eq!(slice[0], Value::Int(0));
        assert_eq!(slice[1], Value::Int(2));
        assert_eq!(slice[2], Value::Int(4));
        assert_eq!(slice[3], Value::Int(6));
        assert_eq!(slice[4], Value::Int(8));
        
        // Test negative step
        let slice = list.slice(Some(9), Some(0), Some(-2)).unwrap();
        assert_eq!(slice.len(), 5);
        assert_eq!(slice[0], Value::Int(9));
        assert_eq!(slice[1], Value::Int(7));
        assert_eq!(slice[2], Value::Int(5));
        assert_eq!(slice[3], Value::Int(3));
        assert_eq!(slice[4], Value::Int(1));
    }

    #[test]
    fn test_extend_and_clear() {
        let mut list1 = HPList::new();
        list1.append(Value::Int(1));
        list1.append(Value::Int(2));
        
        let mut list2 = HPList::new();
        list2.append(Value::Int(3));
        list2.append(Value::Int(4));
        
        // Test extend
        list1.extend(list2.to_vec());
        assert_eq!(list1.len(), 4);
        assert_eq!(list1.get(0), Some(&Value::Int(1)));
        assert_eq!(list1.get(1), Some(&Value::Int(2)));
        assert_eq!(list1.get(2), Some(&Value::Int(3)));
        assert_eq!(list1.get(3), Some(&Value::Int(4)));
        
        // Test clear
        list1.clear();
        assert_eq!(list1.len(), 0);
        assert!(list1.is_empty());
    }

    #[test]
    fn test_error_conditions() {
        let mut list = HPList::new();
        list.append(Value::Int(1));
        list.append(Value::Int(2));
        
        // Test invalid index for set
        assert!(list.set(5, Value::Int(3)).is_err());
        assert!(list.set(-5, Value::Int(3)).is_err());
        
        // Test invalid index for pop_at
        assert!(list.pop_at(5).is_err());
        assert!(list.pop_at(-5).is_err());
        
        // Test remove non-existent element
        assert!(list.remove(&Value::Int(99)).is_err());
        
        // Test index non-existent element
        assert!(list.index(&Value::Int(99), None, None).is_err());
        
        // Test zero step in slice
        assert!(list.slice(Some(0), Some(5), Some(0)).is_err());
    }

    #[test]
    fn test_from_values() {
        let values = vec![Value::Int(1), Value::Int(2), Value::Int(3)];
        let list = HPList::from_values(values.clone());
        
        assert_eq!(list.len(), 3);
        assert_eq!(list.to_vec(), values);
    }

    #[test]
    fn test_with_capacity() {
        let list = HPList::with_capacity(100);
        assert_eq!(list.len(), 0);
        // Note: We can't directly test capacity without exposing it,
        // but this at least verifies the constructor works
    }

    #[test]
    fn test_iterators() {
        let mut list = HPList::new();
        list.append(Value::Int(1));
        list.append(Value::Int(2));
        list.append(Value::Int(3));
        
        // Test immutable iterator
        let mut sum = 0;
        for item in list.iter() {
            if let Value::Int(n) = item {
                sum += n;
            }
        }
        assert_eq!(sum, 6);
        
        // Test mutable iterator
        for item in list.iter_mut() {
            if let Value::Int(n) = item {
                *n *= 2;
            }
        }
        
        assert_eq!(list.get(0), Some(&Value::Int(2)));
        assert_eq!(list.get(1), Some(&Value::Int(4)));
        assert_eq!(list.get(2), Some(&Value::Int(6)));
    }

    #[test]
    fn test_display_formatting() {
        let mut list = HPList::new();
        list.append(Value::Int(1));
        list.append(Value::Str("hello".to_string()));
        list.append(Value::Bool(true));
        
        let display = format!("{}", list);
        assert!(display.contains("["));
        assert!(display.contains("]"));
        assert!(display.contains("1"));
        assert!(display.contains("hello"));
        assert!(display.contains("True"));
    }
}