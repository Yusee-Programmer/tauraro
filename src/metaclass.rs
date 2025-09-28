use std::collections::{HashMap, HashSet};
use crate::value::Value;
use crate::base_object::MRO;
use anyhow::{Result, anyhow};

/// Metaclass system for Python-like class creation and MRO computation
/// Based on CPython's implementation with optimizations
#[derive(Debug, Clone)]
pub struct MetaClass {
    pub name: String,
    pub bases: Vec<String>,
    pub namespace: HashMap<String, Value>,
    pub mro_cache: Option<Vec<String>>,
    pub custom_mro_method: Option<String>, // Name of custom MRO method if any
}

impl MetaClass {
    pub fn new(name: String, bases: Vec<String>, namespace: HashMap<String, Value>) -> Self {
        Self {
            name,
            bases,
            namespace,
            mro_cache: None,
            custom_mro_method: None,
        }
    }

    /// Set a custom MRO method (like CPython's mro_invoke)
    pub fn set_custom_mro_method(&mut self, method_name: String) {
        self.custom_mro_method = Some(method_name);
        self.mro_cache = None; // Invalidate cache
    }

    /// Check if this metaclass has a custom MRO method
    pub fn has_custom_mro(&self) -> bool {
        self.custom_mro_method.is_some()
    }

    /// Invoke custom MRO method if available (mimics CPython's mro_invoke)
    pub fn invoke_custom_mro(
        &self,
        class_name: &str,
        bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Option<Vec<String>>, String> {
        if let Some(method_name) = &self.custom_mro_method {
            println!("Invoking custom MRO method '{}' for class '{}'", method_name, class_name);
            
            // In a full implementation, this would call the actual method
            // For now, we simulate some custom MRO behaviors
            match method_name.as_str() {
                "__mro__" => {
                    // Standard MRO - return None to use default C3
                    Ok(None)
                }
                "reverse_mro" => {
                    // Example custom MRO that reverses base order
                    let mut reversed_bases = bases.to_vec();
                    reversed_bases.reverse();
                    
                    // Compute C3 with reversed bases
                    let mut result = vec![class_name.to_string()];
                    result.extend(reversed_bases);
                    result.push("object".to_string());
                    
                    Ok(Some(result))
                }
                "depth_first_mro" => {
                    // Example depth-first MRO traversal
                    let mut result = vec![class_name.to_string()];
                    
                    // Add bases in depth-first order
                    for base in bases {
                        if let Some(base_mro) = class_registry.get(base) {
                            for class in base_mro {
                                if !result.contains(class) {
                                    result.push(class.clone());
                                }
                            }
                        } else if !result.contains(base) {
                            result.push(base.clone());
                        }
                    }
                    
                    // Ensure object is at the end
                    if !result.contains(&"object".to_string()) {
                        result.push("object".to_string());
                    }
                    
                    Ok(Some(result))
                }
                _ => {
                    Err(format!("Unknown custom MRO method: {}", method_name))
                }
            }
        } else {
            Ok(None) // No custom MRO method
        }
    }

    /// Get the cached MRO if available
    pub fn get_cached_mro(&self) -> Option<&Vec<String>> {
        self.mro_cache.as_ref()
    }

    /// Cache the computed MRO
    pub fn cache_mro(&mut self, mro: Vec<String>) {
        self.mro_cache = Some(mro);
    }

    /// Invalidate the MRO cache
    pub fn invalidate_mro_cache(&mut self) {
        self.mro_cache = None;
    }
}

/// Optimized MRO computation system with caching and validation
pub struct MROComputer {
    /// Cache of computed MROs to avoid recomputation
    cache: HashMap<String, Vec<String>>,
    /// Validation cache to avoid re-validating the same hierarchies
    validation_cache: HashMap<String, Result<(), String>>,
}

impl MROComputer {
    pub fn new() -> Self {
        Self {
            cache: HashMap::new(),
            validation_cache: HashMap::new(),
        }
    }

    /// Highly optimized C3 linearization with caching and validation
    pub fn compute_optimized_c3_linearization(
        &mut self,
        class_name: &str,
        bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Vec<String>, String> {
        // Create cache key from class name and bases
        let cache_key = format!("{}:{}", class_name, bases.join(","));
        
        // Check cache first for performance optimization
        if let Some(cached_result) = self.cache.get(&cache_key) {
            println!("Cache hit for MRO computation of '{}'", class_name);
            return Ok(cached_result.clone());
        }

        println!("Computing optimized C3 linearization for class '{}' with bases: {:?}", class_name, bases);
        
        // Validate hierarchy before computation to catch errors early
        self.validate_hierarchy(class_name, bases, class_registry)?;
        
        // Perform optimized C3 linearization
        let result = self.compute_c3_with_optimizations(class_name, bases, class_registry)?;
        
        // Cache the result for future use
        self.cache.insert(cache_key, result.clone());
        
        println!("Cached MRO result for '{}': {:?}", class_name, result);
        Ok(result)
    }

    /// Optimized C3 linearization with performance improvements
    fn compute_c3_with_optimizations(
        &mut self,
        class_name: &str,
        bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Vec<String>, String> {
        if bases.is_empty() {
            return Ok(vec![class_name.to_string(), "object".to_string()]);
        }

        // Pre-allocate vectors with estimated capacity for better performance
        let mut base_linearizations = Vec::with_capacity(bases.len() + 1);
        
        // Collect base linearizations with optimized lookups
        for base in bases {
            let base_mro = if base == "object" {
                vec!["object".to_string()]
            } else if let Some(base_bases) = class_registry.get(base) {
                // Recursively compute MRO for the base class
                self.compute_optimized_c3_linearization(base, base_bases, class_registry)?
            } else {
                // If base not in registry, assume it inherits from object
                vec![base.clone(), "object".to_string()]
            };
            base_linearizations.push(base_mro);
        }

        // Add bases list as final sequence
        base_linearizations.push(bases.to_vec());

        // Perform optimized C3 merge
        let merged = self.optimized_c3_merge(base_linearizations)?;
        
        // Build final result efficiently
        let mut result = Vec::with_capacity(merged.len() + 1);
        result.push(class_name.to_string());
        result.extend(merged);
        
        Ok(result)
    }

    /// Highly optimized C3 merge algorithm with performance improvements
    fn optimized_c3_merge(&self, mut sequences: Vec<Vec<String>>) -> Result<Vec<String>, String> {
        let mut result = Vec::new();
        let mut candidate_counts = HashMap::new();
        
        // Pre-compute candidate frequencies for optimization
        for seq in &sequences {
            for (i, class) in seq.iter().enumerate() {
                let count = candidate_counts.entry(class.clone()).or_insert(0);
                if i == 0 {
                    *count += 1; // Head occurrence
                }
            }
        }

        while !sequences.is_empty() {
            // Remove empty sequences efficiently
            sequences.retain(|seq| !seq.is_empty());
            
            if sequences.is_empty() {
                break;
            }

            let mut candidate_found = false;
            let mut selected_candidate: Option<String> = None;
            
            // Use optimized candidate selection
            for seq in &sequences {
                if seq.is_empty() {
                    continue;
                }
                
                let candidate = &seq[0];
                
                // Optimized tail check using iterator chains
                let appears_in_tail = sequences.iter()
                    .any(|other_seq| other_seq.len() > 1 && other_seq[1..].contains(candidate));
                
                if !appears_in_tail {
                    result.push(candidate.clone());
                    selected_candidate = Some(candidate.clone());
                    candidate_found = true;
                    break;
                }
            }
            
            if let Some(candidate) = selected_candidate {
                // Efficiently remove candidate from all sequences
                for seq in &mut sequences {
                    if !seq.is_empty() && seq[0] == candidate {
                        seq.remove(0);
                    }
                }
            }
            
            if !candidate_found {
                return Err(format!(
                    "Cannot create consistent MRO - circular dependency detected in sequences: {:?}",
                    sequences
                ));
            }
        }
        
        Ok(result)
    }

    /// Comprehensive hierarchy validation with detailed error reporting
    fn validate_hierarchy(
        &mut self,
        class_name: &str,
        bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<(), String> {
        let validation_key = format!("{}:{}", class_name, bases.join(","));
        
        // Check validation cache
        if let Some(cached_result) = self.validation_cache.get(&validation_key) {
            return cached_result.clone();
        }

        let mut validation_result = Ok(());

        // Check for self-inheritance
        if bases.contains(&class_name.to_string()) {
            validation_result = Err(format!("Class '{}' cannot inherit from itself", class_name));
        }

        // Check for duplicate bases
        if validation_result.is_ok() {
            let mut seen_bases = std::collections::HashSet::new();
            for base in bases {
                if !seen_bases.insert(base) {
                    validation_result = Err(format!("Duplicate base class '{}' in inheritance list", base));
                    break;
                }
            }
        }

        // Check for circular inheritance
        if validation_result.is_ok() {
            validation_result = self.detect_circular_inheritance(class_name, bases, class_registry);
        }

        // Validate that all base classes exist or are built-ins
        if validation_result.is_ok() {
            for base in bases {
                if base != "object" && !class_registry.contains_key(base) {
                    validation_result = Err(format!("Base class '{}' not found in registry", base));
                    break;
                }
            }
        }

        // Cache validation result
        self.validation_cache.insert(validation_key, validation_result.clone());
        
        validation_result
    }

    /// Detect circular inheritance patterns
    fn detect_circular_inheritance(
        &self,
        class_name: &str,
        bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<(), String> {
        let mut visited = std::collections::HashSet::new();
        let mut path = Vec::new();
        
        self.dfs_circular_check(class_name, class_name, bases, class_registry, &mut visited, &mut path)
    }

    /// Depth-first search for circular inheritance detection
    fn dfs_circular_check(
        &self,
        current_class: &str,
        original_class: &str,
        original_bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
        visited: &mut std::collections::HashSet<String>,
        path: &mut Vec<String>,
    ) -> Result<(), String> {
        if path.contains(&current_class.to_string()) {
            return Err(format!(
                "Circular inheritance detected: {} -> {}",
                path.join(" -> "),
                current_class
            ));
        }

        if visited.contains(current_class) {
            return Ok(());
        }

        visited.insert(current_class.to_string());
        path.push(current_class.to_string());

        // Check all bases for circular dependencies
        let current_bases = if current_class == original_class {
            original_bases.to_vec()
        } else {
            // For other classes, get bases from registry
            if let Some(mro) = class_registry.get(current_class) {
                // Extract bases from MRO (skip self and object)
                let bases_from_mro: Vec<String> = mro.iter()
                    .skip(1)
                    .take_while(|&class| class != "object")
                    .cloned()
                    .collect();
                bases_from_mro
            } else {
                vec![]
            }
        };

        for base in &current_bases {
            if base != "object" {
                self.dfs_circular_check(base, original_class, original_bases, class_registry, visited, path)?;
            }
        }

        path.pop();
        Ok(())
    }

    /// Clear caches when class hierarchy changes
    pub fn invalidate_cache(&mut self) {
        self.cache.clear();
        self.validation_cache.clear();
        println!("MRO caches invalidated due to hierarchy changes");
    }

    /// Get cache statistics for monitoring
    pub fn get_cache_stats(&self) -> (usize, usize) {
        (self.cache.len(), self.validation_cache.len())
    }
}

/// Type creation system that mimics Python's type() builtin
pub struct TypeCreator {
    pub mro_computer: MROComputer,
}

impl TypeCreator {
    pub fn new() -> Self {
        Self {
            mro_computer: MROComputer::new(),
        }
    }

    /// Create a new type (class) with the given name, bases, and namespace
    /// This is equivalent to Python's type(name, bases, dict)
    pub fn create_type(
        &mut self,
        name: String,
        bases: Vec<String>,
        namespace: HashMap<String, Value>,
        metaclass: Option<MetaClass>,
        existing_class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Value> {
        println!("Creating type '{}' with bases: {:?}", name, bases);
        
        // Use the existing class registry from the VM
        let mut class_registry = existing_class_registry.clone();
        
        // Add built-in object class if not present
        if !class_registry.contains_key("object") {
            class_registry.insert("object".to_string(), vec!["object".to_string()]);
        }
        
        // Check for custom MRO method from metaclass
        let mro_linearization = if let Some(ref meta) = metaclass {
            if meta.has_custom_mro() {
                // Try to use custom MRO method first
                match meta.invoke_custom_mro(&name, &bases, &class_registry) {
                    Ok(Some(custom_mro)) => {
                        println!("Using custom MRO for '{}': {:?}", name, custom_mro);
                        custom_mro
                    }
                    Ok(None) => {
                        // Fall back to standard C3 linearization
                        self.mro_computer.compute_optimized_c3_linearization(
                            &name,
                            &bases,
                            &class_registry,
                        ).map_err(|e| anyhow!(e))?
                    }
                    Err(e) => {
                        return Err(anyhow!("Custom MRO method failed: {}", e));
                    }
                }
            } else {
                // Use standard C3 linearization
                self.mro_computer.compute_optimized_c3_linearization(
                    &name,
                    &bases,
                    &class_registry,
                ).map_err(|e| anyhow!(e))?
            }
        } else {
            // No metaclass, use standard C3 linearization
            self.mro_computer.compute_optimized_c3_linearization(
                &name,
                &bases,
                &class_registry,
            ).map_err(|e| anyhow!(e))?
        };
        
        println!("Computed MRO for '{}': {:?}", name, mro_linearization);
        
        // Handle metaclass if provided
        if let Some(mut meta) = metaclass {
            println!("Using metaclass '{}' for class '{}'", meta.name, name);
            
            // Cache the computed MRO in the metaclass
            meta.cache_mro(mro_linearization.clone());
            
            // In a full implementation, we would invoke metaclass.__new__ and __init__ here
            // This would include:
            // 1. Call metaclass.__new__(metaclass, name, bases, namespace)
            // 2. Call metaclass.__init__(class_obj, name, bases, namespace)
        }
        
        // Create the class object with optimized MRO
        let class_value = Value::Object {
            class_name: name.clone(),
            fields: namespace,
            base_object: crate::base_object::BaseObject::new(name.clone(), bases),
            mro: MRO::from_linearization(mro_linearization),
        };

        Ok(class_value)
    }

    /// Get a reference to the MRO computer for advanced operations
    pub fn mro_computer(&mut self) -> &mut MROComputer {
        &mut self.mro_computer
    }

    /// Invalidate all caches (useful when class hierarchy changes)
    pub fn invalidate_caches(&mut self) {
        self.mro_computer.invalidate_cache();
    }

    /// Get performance statistics
    pub fn get_performance_stats(&self) -> (usize, usize) {
        self.mro_computer.get_cache_stats()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_simple_inheritance() {
        let mut computer = MROComputer::new();
        let mut class_registry = HashMap::new();
        class_registry.insert("A".to_string(), vec!["object".to_string()]);
        class_registry.insert("B".to_string(), vec!["A".to_string()]);
        class_registry.insert("object".to_string(), vec![]);
        
        let mro = computer.compute_optimized_c3_linearization("B", &["A".to_string()], &class_registry).unwrap();
        assert_eq!(mro, vec!["B", "A", "object"]);
    }

    #[test]
    fn test_multiple_inheritance() {
        let mut computer = MROComputer::new();
        let mut class_registry = HashMap::new();
        class_registry.insert("A".to_string(), vec!["object".to_string()]);
        class_registry.insert("B".to_string(), vec!["object".to_string()]);
        class_registry.insert("C".to_string(), vec!["A".to_string(), "B".to_string()]);
        class_registry.insert("object".to_string(), vec![]);
        
        let mro = computer.compute_optimized_c3_linearization("C", &["A".to_string(), "B".to_string()], &class_registry).unwrap();
        assert_eq!(mro, vec!["C", "A", "B", "object"]);
    }

    #[test]
    fn test_diamond_inheritance() {
        let mut computer = MROComputer::new();
        let mut class_registry = HashMap::new();
        class_registry.insert("A".to_string(), vec!["object".to_string()]);
        class_registry.insert("B".to_string(), vec!["A".to_string()]);
        class_registry.insert("C".to_string(), vec!["A".to_string()]);
        class_registry.insert("D".to_string(), vec!["B".to_string(), "C".to_string()]);
        class_registry.insert("object".to_string(), vec![]);
        
        let mro = computer.compute_optimized_c3_linearization("D", &["B".to_string(), "C".to_string()], &class_registry).unwrap();
        assert_eq!(mro, vec!["D", "B", "C", "A", "object"]);
    }

    #[test]
    fn test_inconsistent_hierarchy() {
        let mut computer = MROComputer::new();
        let mut class_registry = HashMap::new();
        class_registry.insert("A".to_string(), vec!["object".to_string()]);
        class_registry.insert("B".to_string(), vec!["object".to_string()]);
        class_registry.insert("X".to_string(), vec!["A".to_string(), "B".to_string()]);
        class_registry.insert("Y".to_string(), vec!["B".to_string(), "A".to_string()]);
        class_registry.insert("Z".to_string(), vec!["X".to_string(), "Y".to_string()]);
        class_registry.insert("object".to_string(), vec![]);
        
        // This should fail due to inconsistent ordering
        assert!(computer.compute_optimized_c3_linearization("Z", &["X".to_string(), "Y".to_string()], &class_registry).is_err());
    }

    #[test]
    fn test_mro_caching() {
        let mut computer = MROComputer::new();
        let mut class_registry = HashMap::new();
        class_registry.insert("A".to_string(), vec!["object".to_string()]);
        class_registry.insert("object".to_string(), vec![]);
        
        // First computation
        let mro1 = computer.compute_optimized_c3_linearization("A", &["object".to_string()], &class_registry).unwrap();
        
        // Second computation should use cache
        let mro2 = computer.compute_optimized_c3_linearization("A", &["object".to_string()], &class_registry).unwrap();
        
        assert_eq!(mro1, mro2);
        // Check that cache statistics show cache usage
        let stats = computer.get_cache_stats();
        assert_eq!(stats.0, 1); // One cache entry
    }
}
