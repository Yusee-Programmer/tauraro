//! itertools - Functional programming tools
//! Similar to Python's itertools module

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;

// Import HPList
use crate::modules::hplist::HPList;
// Import call_function from functools
use crate::modules::functools::call_function;

/// Create the itertools module object with all its functions
pub fn create_itertools_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Infinite iterators
    namespace.insert("count".to_string(), Value::NativeFunction(itertools_count));
    namespace.insert("cycle".to_string(), Value::NativeFunction(itertools_cycle));
    namespace.insert("repeat".to_string(), Value::NativeFunction(itertools_repeat));
    
    // Iterators terminating on the shortest input sequence
    namespace.insert("accumulate".to_string(), Value::NativeFunction(itertools_accumulate));
    namespace.insert("chain".to_string(), Value::NativeFunction(itertools_chain));
    namespace.insert("compress".to_string(), Value::NativeFunction(itertools_compress));
    namespace.insert("dropwhile".to_string(), Value::NativeFunction(itertools_dropwhile));
    namespace.insert("filterfalse".to_string(), Value::NativeFunction(itertools_filterfalse));
    namespace.insert("groupby".to_string(), Value::NativeFunction(itertools_groupby));
    namespace.insert("islice".to_string(), Value::NativeFunction(itertools_islice));
    namespace.insert("starmap".to_string(), Value::NativeFunction(itertools_starmap));
    namespace.insert("takewhile".to_string(), Value::NativeFunction(itertools_takewhile));
    namespace.insert("tee".to_string(), Value::NativeFunction(itertools_tee));
    namespace.insert("zip_longest".to_string(), Value::NativeFunction(itertools_zip_longest));
    
    // Combinatorial iterators
    namespace.insert("product".to_string(), Value::NativeFunction(itertools_product));
    namespace.insert("permutations".to_string(), Value::NativeFunction(itertools_permutations));
    namespace.insert("combinations".to_string(), Value::NativeFunction(itertools_combinations));
    namespace.insert("combinations_with_replacement".to_string(), Value::NativeFunction(itertools_combinations_with_replacement));
    
    Value::Module("itertools".to_string(), namespace)
}

/// Count - make an iterator that returns evenly spaced values starting with number start
fn itertools_count(args: Vec<Value>) -> Result<Value> {
    let start = if !args.is_empty() {
        match &args[0] {
            Value::Int(n) => *n,
            Value::Float(f) => *f as i64,
            _ => return Err(anyhow::anyhow!("count() start must be a number")),
        }
    } else {
        0
    };
    
    let step = if args.len() > 1 {
        match &args[1] {
            Value::Int(n) => *n,
            Value::Float(f) => *f as i64,
            _ => return Err(anyhow::anyhow!("count() step must be a number")),
        }
    } else {
        1
    };
    
    let mut count_obj = HashMap::new();
    count_obj.insert("start".to_string(), Value::Int(start));
    count_obj.insert("step".to_string(), Value::Int(step));
    count_obj.insert("current".to_string(), Value::Int(start));
    count_obj.insert("__iter__".to_string(), Value::NativeFunction(count_iter));
    count_obj.insert("__next__".to_string(), Value::NativeFunction(count_next));
    
    Ok(Value::Object {
        class_name: "count".to_string(),
        fields: Rc::new(count_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("count".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["count".to_string(), "object".to_string()]),
    })
}

fn count_iter(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("count.__iter__() takes no arguments"));
    }
    Ok(args[0].clone())
}

fn count_next(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("count.__next__() takes no arguments"));
    }
    
    // This is a simplified implementation - in reality would need mutable state
    Ok(Value::Int(0))
}

/// Cycle - make an iterator returning elements from the iterable and saving a copy of each
fn itertools_cycle(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("cycle() takes exactly 1 argument"));
    }
    
    let iterable = to_list(&args[0])?;
    
    let mut cycle_obj = HashMap::new();
    cycle_obj.insert("iterable".to_string(), Value::List(HPList::from_values(iterable)));
    cycle_obj.insert("index".to_string(), Value::Int(0));
    cycle_obj.insert("__iter__".to_string(), Value::NativeFunction(cycle_iter));
    cycle_obj.insert("__next__".to_string(), Value::NativeFunction(cycle_next));
    
    Ok(Value::Object {
        class_name: "cycle".to_string(),
        fields: Rc::new(cycle_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("cycle".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["cycle".to_string(), "object".to_string()]),
    })
}

fn cycle_iter(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("cycle.__iter__() takes no arguments"));
    }
    Ok(args[0].clone())
}

fn cycle_next(_args: Vec<Value>) -> Result<Value> {
    // Simplified implementation
    Ok(Value::None)
}

/// Repeat - make an iterator that returns object over and over again
fn itertools_repeat(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("repeat() takes 1 or 2 arguments"));
    }
    
    let object = args[0].clone();
    let times = if args.len() > 1 {
        match &args[1] {
            Value::Int(n) => Some(*n),
            _ => return Err(anyhow::anyhow!("repeat() times must be an integer")),
        }
    } else {
        None
    };
    
    let mut repeat_obj = HashMap::new();
    repeat_obj.insert("object".to_string(), object);
    repeat_obj.insert("times".to_string(), 
        if let Some(t) = times { Value::Int(t) } else { Value::None });
    repeat_obj.insert("count".to_string(), Value::Int(0));
    repeat_obj.insert("__iter__".to_string(), Value::NativeFunction(repeat_iter));
    repeat_obj.insert("__next__".to_string(), Value::NativeFunction(repeat_next));
    
    Ok(Value::Object {
        class_name: "repeat".to_string(),
        fields: Rc::new(repeat_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("repeat".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["repeat".to_string(), "object".to_string()]),
    })
}

fn repeat_iter(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("repeat.__iter__() takes no arguments"));
    }
    Ok(args[0].clone())
}

fn repeat_next(_args: Vec<Value>) -> Result<Value> {
    // Simplified implementation
    Ok(Value::None)
}

/// Accumulate - make an iterator that returns accumulated values
fn itertools_accumulate(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("accumulate() takes 1 to 3 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    
    // For now, just return the original list (simplified implementation)
    Ok(Value::List(HPList::from_values(iterable)))
}

/// Chain - make an iterator that returns elements from the first iterable until it is exhausted
fn itertools_chain(args: Vec<Value>) -> Result<Value> {
    let mut result = Vec::new();
    
    for arg in args {
        let items = to_list(&arg)?;
        result.extend(items);
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Compress - make an iterator that filters elements from data returning only those that have a corresponding element in selectors that evaluates to True
fn itertools_compress(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("compress() takes exactly 2 arguments"));
    }
    
    let data = to_list(&args[0])?;
    let selectors = to_list(&args[1])?;
    
    let mut result = Vec::new();
    for (item, selector) in data.iter().zip(selectors.iter()) {
        if is_truthy(selector) {
            result.push(item.clone());
        }
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Dropwhile - make an iterator that drops elements from the iterable as long as the predicate is true
fn itertools_dropwhile(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("dropwhile() takes exactly 2 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    let predicate = &args[1];
    
    let mut result = Vec::new();
    let mut dropping = true;
    
    for item in iterable {
        if dropping {
            // Test predicate
            let test_result = call_function(predicate, vec![item.clone()])?;
            if !is_truthy(&test_result) {
                dropping = false;
                result.push(item);
            }
        } else {
            result.push(item);
        }
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Filterfalse - make an iterator that filters elements from iterable returning only those for which the predicate is False
fn itertools_filterfalse(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("filterfalse() takes exactly 2 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    let predicate = &args[1];
    
    let mut result = Vec::new();
    
    for item in iterable {
        let test_result = call_function(predicate, vec![item.clone()])?;
        if !is_truthy(&test_result) {
            result.push(item);
        }
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Groupby - make an iterator that returns consecutive keys and groups from the iterable
fn itertools_groupby(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 && args.len() != 2 {
        return Err(anyhow::anyhow!("groupby() takes 1 or 2 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    let key_func = if args.len() > 1 {
        Some(args[1].clone())
    } else {
        None
    };
    
    if iterable.is_empty() {
        return Ok(Value::List(HPList::new()));
    }
    
    let mut result = Vec::new();
    let mut current_key = None;
    let mut current_group = Vec::new();
    
    for item in iterable {
        let key = if let Some(ref kf) = key_func {
            call_function(kf, vec![item.clone()])?
        } else {
            item.clone()
        };
        
        if current_key.is_none() || current_key.as_ref() != Some(&key) {
            if !current_group.is_empty() {
                result.push(Value::Tuple(vec![
                    current_key.unwrap(),
                    Value::List(HPList::from_values(current_group))
                ]));
            }
            current_key = Some(key);
            current_group = vec![item];
        } else {
            current_group.push(item);
        }
    }
    
    // Add the last group
    if !current_group.is_empty() {
        result.push(Value::Tuple(vec![
            current_key.unwrap(),
            Value::List(HPList::from_values(current_group))
        ]));
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Islice - make an iterator that returns selected elements from the iterable
fn itertools_islice(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 4 {
        return Err(anyhow::anyhow!("islice() takes 2 to 4 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    
    let (start, stop, step) = match args.len() {
        2 => {
            // islice(iterable, stop)
            let stop = match &args[1] {
                Value::Int(n) => *n as usize,
                _ => return Err(anyhow::anyhow!("islice() stop must be an integer")),
            };
            (0, stop, 1)
        }
        3 => {
            // islice(iterable, start, stop)
            let start = match &args[1] {
                Value::Int(n) => *n as usize,
                _ => return Err(anyhow::anyhow!("islice() start must be an integer")),
            };
            let stop = match &args[2] {
                Value::Int(n) => *n as usize,
                _ => return Err(anyhow::anyhow!("islice() stop must be an integer")),
            };
            (start, stop, 1)
        }
        4 => {
            // islice(iterable, start, stop, step)
            let start = match &args[1] {
                Value::Int(n) => *n as usize,
                _ => return Err(anyhow::anyhow!("islice() start must be an integer")),
            };
            let stop = match &args[2] {
                Value::Int(n) => *n as usize,
                _ => return Err(anyhow::anyhow!("islice() stop must be an integer")),
            };
            let step = match &args[3] {
                Value::Int(n) => *n as usize,
                _ => return Err(anyhow::anyhow!("islice() step must be an integer")),
            };
            (start, stop, step)
        }
        _ => unreachable!(),
    };
    
    let mut result = Vec::new();
    let mut i = start;
    while i < stop && i < iterable.len() {
        result.push(iterable[i].clone());
        i += step;
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Starmap - make an iterator that computes the function using arguments obtained from the iterable
fn itertools_starmap(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("starmap() takes exactly 2 arguments"));
    }
    
    let func = &args[0];
    let iterable = to_list(&args[1])?;
    
    let mut result = Vec::new();
    
    for item in iterable {
        let args_list = match item {
            Value::List(args) => args.to_vec(),
            Value::Tuple(args) => args,
            _ => return Err(anyhow::anyhow!("starmap() iterable must contain lists or tuples")),
        };
        
        let call_result = call_function(func, args_list)?;
        result.push(call_result);
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Takewhile - make an iterator that returns elements from the iterable as long as the predicate is true
fn itertools_takewhile(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("takewhile() takes exactly 2 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    let predicate = &args[1];
    
    let mut result = Vec::new();
    
    for item in iterable {
        let test_result = call_function(predicate, vec![item.clone()])?;
        if is_truthy(&test_result) {
            result.push(item);
        } else {
            break;
        }
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Tee - return n independent iterators from a single iterable
fn itertools_tee(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("tee() takes 1 or 2 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    let n = if args.len() > 1 {
        match &args[1] {
            Value::Int(num) => *num as usize,
            _ => return Err(anyhow::anyhow!("tee() n must be an integer")),
        }
    } else {
        2
    };
    
    // Return n copies of the iterable
    let mut result = Vec::new();
    for _ in 0..n {
        result.push(Value::List(HPList::from_values(iterable.clone())));
    }
    
    Ok(Value::Tuple(result))
}

/// Zip longest - make an iterator that aggregates elements from each of the iterables
fn itertools_zip_longest(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("zip_longest() requires at least 1 argument"));
    }
    
    let mut iterables = Vec::new();
    let fillvalue = Value::None;
    
    // Check if last argument is fillvalue keyword
    for arg in &args {
        let items = to_list(arg)?;
        iterables.push(items);
    }
    
    if iterables.is_empty() {
        return Ok(Value::List(HPList::new()));
    }
    
    let max_len = iterables.iter().map(|it| it.len()).max().unwrap_or(0);
    let mut result = Vec::new();
    
    for i in 0..max_len {
        let mut tuple_items = Vec::new();
        for iterable in &iterables {
            if i < iterable.len() {
                tuple_items.push(iterable[i].clone());
            } else {
                tuple_items.push(fillvalue.clone());
            }
        }
        result.push(Value::Tuple(tuple_items));
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Product - cartesian product of input iterables
fn itertools_product(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::List(HPList::new()));
    }
    
    let mut iterables = Vec::new();
    for arg in &args {
        let items = to_list(arg)?;
        iterables.push(items);
    }
    
    // Simple implementation for 2 iterables
    if iterables.len() == 2 {
        let mut result = Vec::new();
        for item1 in &iterables[0] {
            for item2 in &iterables[1] {
                result.push(Value::Tuple(vec![item1.clone(), item2.clone()]));
            }
        }
        return Ok(Value::List(HPList::from_values(result)));
    }
    
    // For more complex cases, return empty for now
    Ok(Value::List(HPList::new()))
}

/// Permutations - return successive r length permutations of elements in the iterable
fn itertools_permutations(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("permutations() takes 1 or 2 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    let _r = if args.len() > 1 {
        match &args[1] {
            Value::Int(n) => *n as usize,
            _ => return Err(anyhow::anyhow!("permutations() r must be an integer")),
        }
    } else {
        iterable.len()
    };
    
    // Simplified implementation - just return empty for now
    Ok(Value::List(HPList::new()))
}

/// Combinations - return r length subsequences of elements from the input iterable
fn itertools_combinations(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("combinations() takes exactly 2 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    let r = match &args[1] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow::anyhow!("combinations() r must be an integer")),
    };
    
    if r > iterable.len() {
        return Ok(Value::List(HPList::new()));
    }
    
    // Simple implementation for r=2
    if r == 2 {
        let mut result = Vec::new();
        for i in 0..iterable.len() {
            for j in (i + 1)..iterable.len() {
                result.push(Value::Tuple(vec![iterable[i].clone(), iterable[j].clone()]));
            }
        }
        return Ok(Value::List(HPList::from_values(result)));
    }
    
    // For other cases, return empty for now
    Ok(Value::List(HPList::new()))
}

/// Combinations with replacement - return r length subsequences of elements from the input iterable allowing individual elements to be repeated more than once
fn itertools_combinations_with_replacement(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("combinations_with_replacement() takes exactly 2 arguments"));
    }
    
    let iterable = to_list(&args[0])?;
    let r = match &args[1] {
        Value::Int(n) => *n as usize,
        _ => return Err(anyhow::anyhow!("combinations_with_replacement() r must be an integer")),
    };
    
    if r == 0 {
        return Ok(Value::List(HPList::new()));
    }
    
    if iterable.is_empty() {
        return Ok(Value::List(HPList::new()));
    }
    
    // Simple implementation for r=1
    if r == 1 {
        let mut result = Vec::new();
        for item in &iterable {
            result.push(Value::Tuple(vec![item.clone()]));
        }
        return Ok(Value::List(HPList::from_values(result)));
    }
    
    // Simple implementation for r=2
    if r == 2 {
        let mut result = Vec::new();
        for i in 0..iterable.len() {
            for j in i..iterable.len() {
                result.push(Value::Tuple(vec![iterable[i].clone(), iterable[j].clone()]));
            }
        }
        return Ok(Value::List(HPList::from_values(result)));
    }
    
    // For higher r values, return empty for now
    Ok(Value::List(HPList::new()))
}

/// Helper function to convert a Value to a list
fn to_list(value: &Value) -> Result<Vec<Value>> {
    match value {
        Value::List(items) => Ok(items.as_vec().clone()),
        Value::Tuple(items) => Ok(items.clone()),
        Value::Str(s) => Ok(s.chars().map(|c| Value::Str(c.to_string())).collect()),
        _ => Err(anyhow::anyhow!("Object is not iterable")),
    }
}

/// Helper function to check if a value is truthy
fn is_truthy(value: &Value) -> bool {
    match value {
        Value::None => false,
        Value::Bool(b) => *b,
        Value::Int(n) => *n != 0,
        Value::Float(f) => *f != 0.0,
        Value::Str(s) => !s.is_empty(),
        Value::List(items) => !items.is_empty(),
        Value::Tuple(items) => !items.is_empty(),
        Value::Dict(map) => !map.is_empty(),
        _ => true,
    }
}