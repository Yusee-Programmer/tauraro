/// Random module - provides random number generation and sampling functions
/// Similar to Python's random module

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use rand::{Rng, SeedableRng};
use rand::rngs::StdRng;
use rand::seq::SliceRandom;
use std::sync::Mutex;
// Import HPList
use crate::modules::hplist::HPList;

// Global random number generator
lazy_static::lazy_static! {
    static ref GLOBAL_RNG: Mutex<StdRng> = Mutex::new(StdRng::from_entropy());
}

/// Create the random module object with all its functions
pub fn create_random_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Basic random functions
    namespace.insert("random".to_string(), Value::NativeFunction(random_random));
    namespace.insert("uniform".to_string(), Value::NativeFunction(random_uniform));
    namespace.insert("randint".to_string(), Value::NativeFunction(random_randint));
    namespace.insert("randrange".to_string(), Value::NativeFunction(random_randrange));
    
    // Sequence functions
    namespace.insert("choice".to_string(), Value::NativeFunction(random_choice));
    namespace.insert("choices".to_string(), Value::NativeFunction(random_choices));
    namespace.insert("shuffle".to_string(), Value::NativeFunction(random_shuffle));
    namespace.insert("sample".to_string(), Value::NativeFunction(random_sample));
    
    // Distribution functions
    namespace.insert("gauss".to_string(), Value::NativeFunction(random_gauss));
    namespace.insert("normalvariate".to_string(), Value::NativeFunction(random_normalvariate));
    namespace.insert("lognormvariate".to_string(), Value::NativeFunction(random_lognormvariate));
    namespace.insert("expovariate".to_string(), Value::NativeFunction(random_expovariate));
    namespace.insert("vonmisesvariate".to_string(), Value::NativeFunction(random_vonmisesvariate));
    namespace.insert("gammavariate".to_string(), Value::NativeFunction(random_gammavariate));
    namespace.insert("betavariate".to_string(), Value::NativeFunction(random_betavariate));
    namespace.insert("paretovariate".to_string(), Value::NativeFunction(random_paretovariate));
    namespace.insert("weibullvariate".to_string(), Value::NativeFunction(random_weibullvariate));
    
    // Seed and state functions
    namespace.insert("seed".to_string(), Value::NativeFunction(random_seed));
    namespace.insert("getstate".to_string(), Value::NativeFunction(random_getstate));
    namespace.insert("setstate".to_string(), Value::NativeFunction(random_setstate));
    namespace.insert("getrandbits".to_string(), Value::NativeFunction(random_getrandbits));
    
    Value::Module("random".to_string(), namespace)
}

// Helper function to convert Value to f64
fn value_to_float(value: &Value) -> Result<f64> {
    match value {
        Value::Int(i) => Ok(*i as f64),
        Value::Float(f) => Ok(*f),
        _ => Err(anyhow!("Expected a number")),
    }
}

// Helper function to convert Value to i64
fn value_to_int(value: &Value) -> Result<i64> {
    match value {
        Value::Int(i) => Ok(*i),
        Value::Float(f) => Ok(*f as i64),
        _ => Err(anyhow!("Expected a number")),
    }
}

// Helper function to convert Value to list
fn value_to_list(value: &Value) -> Result<Vec<Value>> {
    match value {
        Value::List(list) => Ok(list.as_vec().clone()),
        _ => Err(anyhow!("Expected a list")),
    }
}

/// Generate a random float in [0.0, 1.0): random()
fn random_random(args: Vec<Value>) -> Result<Value> {
    if !args.is_empty() {
        return Err(anyhow!("random() takes no arguments"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    Ok(Value::Float(rng.gen::<f64>()))
}

/// Generate a random float in [a, b]: uniform(a, b)
fn random_uniform(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("uniform() takes exactly 2 arguments"));
    }
    
    let a = value_to_float(&args[0])?;
    let b = value_to_float(&args[1])?;
    
    if a > b {
        return Err(anyhow!("uniform() requires a <= b"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let random_val: f64 = rng.gen();
    Ok(Value::Float(a + random_val * (b - a)))
}

/// Generate a random integer in [a, b]: randint(a, b)
fn random_randint(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("randint() takes exactly 2 arguments"));
    }
    
    let a = value_to_int(&args[0])?;
    let b = value_to_int(&args[1])?;
    
    if a > b {
        return Err(anyhow!("randint() requires a <= b"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let result = rng.gen_range(a..=b);
    Ok(Value::Int(result))
}

/// Generate a random integer in range: randrange(start, stop, step=1)
fn random_randrange(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow!("randrange() takes 1 to 3 arguments"));
    }
    
    let (start, stop, step) = if args.len() == 1 {
        (0, value_to_int(&args[0])?, 1)
    } else if args.len() == 2 {
        (value_to_int(&args[0])?, value_to_int(&args[1])?, 1)
    } else {
        (value_to_int(&args[0])?, value_to_int(&args[1])?, value_to_int(&args[2])?)
    };
    
    if step == 0 {
        return Err(anyhow!("randrange() step argument must not be zero"));
    }
    
    if step > 0 && start >= stop {
        return Err(anyhow!("empty range for randrange()"));
    }
    
    if step < 0 && start <= stop {
        return Err(anyhow!("empty range for randrange()"));
    }
    
    let range_size = ((stop - start) as f64 / step as f64).floor() as i64;
    if range_size <= 0 {
        return Err(anyhow!("empty range for randrange()"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let index = rng.gen_range(0..range_size);
    Ok(Value::Int(start + index * step))
}

/// Choose a random element from a sequence: choice(seq)
fn random_choice(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("choice() takes exactly 1 argument"));
    }
    
    let seq = value_to_list(&args[0])?;
    if seq.is_empty() {
        return Err(anyhow!("choice() cannot choose from an empty sequence"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let index = rng.gen_range(0..seq.len());
    Ok(seq[index].clone())
}

/// Choose k elements with replacement: choices(population, weights=None, k=1)
fn random_choices(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow!("choices() takes 1 to 3 arguments"));
    }
    
    let population = value_to_list(&args[0])?;
    if population.is_empty() {
        return Err(anyhow!("choices() cannot choose from an empty population"));
    }
    
    let weights = if args.len() > 1 && !matches!(args[1], Value::None) {
        Some(value_to_list(&args[1])?)
    } else {
        None
    };
    
    let k = if args.len() > 2 {
        value_to_int(&args[2])? as usize
    } else {
        1
    };
    
    if let Some(ref w) = weights {
        if w.len() != population.len() {
            return Err(anyhow!("weights must have same length as population"));
        }
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let mut result = Vec::new();
    
    for _ in 0..k {
        let index = if let Some(ref weights) = weights {
            // Weighted selection (simplified)
            let total_weight: f64 = weights.iter()
                .map(|w| value_to_float(w).unwrap_or(0.0))
                .sum();
            
            let mut random_weight = rng.gen::<f64>() * total_weight;
            let mut selected_index = 0;
            
            for (i, weight) in weights.iter().enumerate() {
                let w = value_to_float(weight).unwrap_or(0.0);
                if random_weight <= w {
                    selected_index = i;
                    break;
                }
                random_weight -= w;
            }
            selected_index
        } else {
            rng.gen_range(0..population.len())
        };
        
        result.push(population[index].clone());
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

/// Shuffle a sequence in place: shuffle(x)
fn random_shuffle(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("shuffle() takes exactly 1 argument"));
    }
    
    let seq = value_to_list(&args[0])?;
    let mut shuffled = seq.clone();
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    shuffled.shuffle(&mut *rng);
    
    Ok(Value::List(HPList::from_values(shuffled)))
}

/// Sample k unique elements: sample(population, k)
fn random_sample(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("sample() takes exactly 2 arguments"));
    }
    
    let population = value_to_list(&args[0])?;
    let k = value_to_int(&args[1])? as usize;
    
    if k > population.len() {
        return Err(anyhow!("sample larger than population"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let sample: Vec<Value> = population.choose_multiple(&mut *rng, k).cloned().collect();
    
    Ok(Value::List(HPList::from_values(sample)))
}

/// Gaussian distribution: gauss(mu, sigma)
fn random_gauss(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("gauss() takes exactly 2 arguments"));
    }
    
    let mu = value_to_float(&args[0])?;
    let sigma = value_to_float(&args[1])?;
    
    // Box-Muller transform
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let u1: f64 = rng.gen();
    let u2: f64 = rng.gen();
    
    let z0 = (-2.0 * u1.ln()).sqrt() * (2.0 * std::f64::consts::PI * u2).cos();
    Ok(Value::Float(mu + sigma * z0))
}

/// Normal distribution: normalvariate(mu, sigma)
fn random_normalvariate(args: Vec<Value>) -> Result<Value> {
    // Same as gauss
    random_gauss(args)
}

/// Log-normal distribution: lognormvariate(mu, sigma)
fn random_lognormvariate(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("lognormvariate() takes exactly 2 arguments"));
    }
    
    let normal_result = random_gauss(args)?;
    match normal_result {
        Value::Float(x) => Ok(Value::Float(x.exp())),
        _ => Err(anyhow!("Internal error in lognormvariate")),
    }
}

/// Exponential distribution: expovariate(lambd)
fn random_expovariate(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("expovariate() takes exactly 1 argument"));
    }
    
    let lambd = value_to_float(&args[0])?;
    if lambd <= 0.0 {
        return Err(anyhow!("expovariate() lambda must be positive"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let u: f64 = rng.gen();
    Ok(Value::Float(-u.ln() / lambd))
}

/// Von Mises distribution: vonmisesvariate(mu, kappa)
fn random_vonmisesvariate(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("vonmisesvariate() takes exactly 2 arguments"));
    }
    
    let mu = value_to_float(&args[0])?;
    let kappa = value_to_float(&args[1])?;
    
    // Simplified implementation
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let u: f64 = rng.gen();
    let result = mu + kappa * (2.0 * std::f64::consts::PI * u).cos();
    Ok(Value::Float(result))
}

/// Gamma distribution: gammavariate(alpha, beta)
fn random_gammavariate(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("gammavariate() takes exactly 2 arguments"));
    }
    
    let alpha = value_to_float(&args[0])?;
    let beta = value_to_float(&args[1])?;
    
    if alpha <= 0.0 || beta <= 0.0 {
        return Err(anyhow!("gammavariate() parameters must be positive"));
    }
    
    // Simplified implementation using exponential
    let exp_result = random_expovariate(vec![Value::Float(1.0 / beta)])?;
    match exp_result {
        Value::Float(x) => Ok(Value::Float(x * alpha)),
        _ => Err(anyhow!("Internal error in gammavariate")),
    }
}

/// Beta distribution: betavariate(alpha, beta)
fn random_betavariate(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("betavariate() takes exactly 2 arguments"));
    }
    
    let alpha = value_to_float(&args[0])?;
    let beta = value_to_float(&args[1])?;
    
    if alpha <= 0.0 || beta <= 0.0 {
        return Err(anyhow!("betavariate() parameters must be positive"));
    }
    
    // Simplified implementation
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let u: f64 = rng.gen();
    let v: f64 = rng.gen();
    
    let x = u.powf(1.0 / alpha);
    let y = v.powf(1.0 / beta);
    
    Ok(Value::Float(x / (x + y)))
}

/// Pareto distribution: paretovariate(alpha)
fn random_paretovariate(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("paretovariate() takes exactly 1 argument"));
    }
    
    let alpha = value_to_float(&args[0])?;
    if alpha <= 0.0 {
        return Err(anyhow!("paretovariate() alpha must be positive"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let u: f64 = rng.gen();
    Ok(Value::Float((1.0 - u).powf(-1.0 / alpha)))
}

/// Weibull distribution: weibullvariate(alpha, beta)
fn random_weibullvariate(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("weibullvariate() takes exactly 2 arguments"));
    }
    
    let alpha = value_to_float(&args[0])?;
    let beta = value_to_float(&args[1])?;
    
    if alpha <= 0.0 || beta <= 0.0 {
        return Err(anyhow!("weibullvariate() parameters must be positive"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let u: f64 = rng.gen();
    Ok(Value::Float(alpha * (-u.ln()).powf(1.0 / beta)))
}

/// Seed the random number generator: seed(a=None)
fn random_seed(args: Vec<Value>) -> Result<Value> {
    if args.len() > 1 {
        return Err(anyhow!("seed() takes at most 1 argument"));
    }
    
    let seed_value = if args.is_empty() || matches!(args[0], Value::None) {
        // Use system entropy
        StdRng::from_entropy()
    } else {
        let seed = value_to_int(&args[0])? as u64;
        StdRng::seed_from_u64(seed)
    };
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    *rng = seed_value;
    
    Ok(Value::None)
}

/// Get the current state: getstate()
fn random_getstate(args: Vec<Value>) -> Result<Value> {
    if !args.is_empty() {
        return Err(anyhow!("getstate() takes no arguments"));
    }
    
    // In a real implementation, this would return the actual RNG state
    // For now, return a placeholder
    Ok(Value::Str("random_state_placeholder".to_string()))
}

/// Set the state: setstate(state)
fn random_setstate(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("setstate() takes exactly 1 argument"));
    }
    
    // In a real implementation, this would restore the RNG state
    // For now, just validate the argument
    match &args[0] {
        Value::Str(_) => Ok(Value::None),
        _ => Err(anyhow!("setstate() argument must be a state object")),
    }
}

/// Generate random bits: getrandbits(k)
fn random_getrandbits(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("getrandbits() takes exactly 1 argument"));
    }
    
    let k = value_to_int(&args[0])?;
    if k <= 0 {
        return Err(anyhow!("getrandbits() k must be positive"));
    }
    
    if k > 64 {
        return Err(anyhow!("getrandbits() k must be <= 64"));
    }
    
    let mut rng = GLOBAL_RNG.lock().unwrap();
    let mask = (1u64 << k) - 1;
    let result = rng.gen::<u64>() & mask;
    
    Ok(Value::Int(result as i64))
}
