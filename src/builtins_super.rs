use crate::value::Value;
use crate::vm::VM;
use anyhow::Result;

/// Implementation of the super() builtin function
/// 
/// super() returns a proxy object that allows access to parent class methods.
/// In Python, super() can be called with no arguments inside a method to automatically
/// determine the current class and instance, or with explicit arguments.
pub fn builtin_super(args: Vec<Value>, vm: Option<&VM>) -> Result<Value> {
    match args.len() {
        0 => {
            // Automatic detection of current class and instance
            if let Some(vm_ref) = vm {
                if let Some(self_value) = vm_ref.get_variable("self") {
                    if let Value::Object { class_name, mro, .. } = &self_value {
                        // Get the currently executing class from VM context
                        let current_executing_class = vm_ref.current_executing_class.as_ref()
                            .unwrap_or(class_name);
                        
                        // Find the next class in MRO after the currently executing class
                        let linearization = &mro.linearization;
                        if let Some(pos) = linearization.iter().position(|c| c == current_executing_class) {
                            if pos + 1 < linearization.len() {
                                let parent_class = linearization[pos + 1].clone();
                                return Ok(Value::Super(current_executing_class.clone(), parent_class, Some(Box::new(self_value.clone()))));
                            } else {
                                return Err(anyhow::anyhow!("No parent class found in MRO after '{}'", current_executing_class));
                            }
                        } else {
                            return Err(anyhow::anyhow!("Current executing class '{}' not found in MRO", current_executing_class));
                        }
                    } else {
                        return Err(anyhow::anyhow!("'self' is not an object instance"));
                    }
                } else {
                    return Err(anyhow::anyhow!("super() called outside of a method (no 'self' found)"));
                }
            } else {
                return Err(anyhow::anyhow!("VM context not available for super() resolution"));
            }
        }
        2 => {
            // super(current_class, obj) - explicit form
            let current_class = match &args[0] {
                Value::Str(s) => s.clone(),
                Value::Object { class_name, .. } => class_name.clone(),
                _ => return Err(anyhow::anyhow!("super() first argument must be a class name or object")),
            };
            
            let obj = &args[1];
            
            Ok(Value::Super(current_class, obj.type_name().to_string(), Some(Box::new(obj.clone()))))
        }
        _ => {
            Err(anyhow::anyhow!("super() takes 0 or 2 arguments, got {}", args.len()))
        }
    }
}
