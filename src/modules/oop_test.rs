//! OOP test module for Tauraro

use crate::value::Value;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

/// Create the OOP test module with comprehensive class examples
pub fn create_oop_test_module() -> Value {
    let mut namespace = HashMap::new();

    // Create a simple Person class
    let mut person_methods = HashMap::new();
    
    // __init__ method for Person
    person_methods.insert("__init__".to_string(), Value::BuiltinFunction("Person.__init__".to_string(), |args| {
        if args.len() < 3 {
            return Err(anyhow::anyhow!("Person.__init__() missing required arguments"));
        }
        
        // args[0] is self, args[1] is name, args[2] is age
        if let Value::Object { fields, .. } = &args[0] {
            let mut fields_mut = (**fields).clone();
            if let Value::Str(name) = &args[1] {
                fields_mut.insert("name".to_string(), Value::Str(name.clone()));
            }
            if let Value::Int(age) = &args[2] {
                fields_mut.insert("age".to_string(), Value::Int(*age));
            }
            fields_mut.insert("friends".to_string(), Value::List(crate::modules::hplist::HPList::new()));
            
            // Update the object fields
            // Note: In a real implementation, we would need to modify the fields in place
            Ok(Value::None)
        } else {
            Err(anyhow::anyhow!("First argument must be an object instance"))
        }
    }));
    
    // introduce method for Person
    person_methods.insert("introduce".to_string(), Value::BuiltinFunction("Person.introduce".to_string(), |args| {
        if args.is_empty() {
            return Err(anyhow::anyhow!("introduce() missing self argument"));
        }
        
        if let Value::Object { fields, .. } = &args[0] {
            if let (Some(Value::Str(name)), Some(Value::Int(age))) = 
                (fields.get("name"), fields.get("age")) {
                Ok(Value::Str(format!("Hi, I'm {} and I'm {} years old.", name, age)))
            } else {
                Ok(Value::Str("Hi, I'm someone.".to_string()))
            }
        } else {
            Err(anyhow::anyhow!("First argument must be an object instance"))
        }
    }));
    
    // add_friend method for Person
    person_methods.insert("add_friend".to_string(), Value::BuiltinFunction("Person.add_friend".to_string(), |args| {
        if args.len() < 2 {
            return Err(anyhow::anyhow!("add_friend() missing friend argument"));
        }
        
        if let (Value::Object { fields, .. }, Value::Object { .. }) = (&args[0], &args[1]) {
            // In a real implementation, we would add the friend to the friends list
            Ok(Value::None)
        } else {
            Err(anyhow::anyhow!("Arguments must be object instances"))
        }
    }));
    
    // Create the Person class
    let person_class = Value::Class {
        name: "Person".to_string(),
        bases: vec!["object".to_string()],
        methods: person_methods,
        metaclass: None,
        mro: crate::base_object::MRO::from_linearization(vec!["Person".to_string(), "object".to_string()]),
        base_object: crate::base_object::BaseObject::new("Person".to_string(), vec!["object".to_string()]),
    };
    
    namespace.insert("Person".to_string(), person_class);

    // Create a Student class that inherits from Person
    let mut student_methods = HashMap::new();
    
    // __init__ method for Student
    student_methods.insert("__init__".to_string(), Value::BuiltinFunction("Student.__init__".to_string(), |args| {
        if args.len() < 4 {
            return Err(anyhow::anyhow!("Student.__init__() missing required arguments"));
        }
        
        // Call parent __init__ first (simplified)
        // In a real implementation, we would use super()
        
        // Then initialize student-specific attributes
        if let Value::Object { fields, .. } = &args[0] {
            let mut fields_mut = (**fields).clone();
            if let Value::Str(school) = &args[3] {
                fields_mut.insert("school".to_string(), Value::Str(school.clone()));
            }
            fields_mut.insert("grades".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));
            Ok(Value::None)
        } else {
            Err(anyhow::anyhow!("First argument must be an object instance"))
        }
    }));
    
    // study method for Student
    student_methods.insert("study".to_string(), Value::BuiltinFunction("Student.study".to_string(), |args| {
        if args.is_empty() {
            return Err(anyhow::anyhow!("study() missing self argument"));
        }
        
        if let Value::Object { fields, .. } = &args[0] {
            if let Some(Value::Str(name)) = fields.get("name") {
                Ok(Value::Str(format!("{} is studying hard!", name)))
            } else {
                Ok(Value::Str("Someone is studying hard!".to_string()))
            }
        } else {
            Err(anyhow::anyhow!("First argument must be an object instance"))
        }
    }));
    
    // Create the Student class
    let student_class = Value::Class {
        name: "Student".to_string(),
        bases: vec!["Person".to_string()],
        methods: student_methods,
        metaclass: None,
        mro: crate::base_object::MRO::from_linearization(vec!["Student".to_string(), "Person".to_string(), "object".to_string()]),
        base_object: crate::base_object::BaseObject::new("Student".to_string(), vec!["Person".to_string()]),
    };
    
    namespace.insert("Student".to_string(), student_class);

    // Create a simple Shape base class
    let mut shape_methods = HashMap::new();
    
    // area method for Shape (to be overridden)
    shape_methods.insert("area".to_string(), Value::BuiltinFunction("Shape.area".to_string(), |_args| {
        Ok(Value::Float(0.0))
    }));
    
    // perimeter method for Shape (to be overridden)
    shape_methods.insert("perimeter".to_string(), Value::BuiltinFunction("Shape.perimeter".to_string(), |_args| {
        Ok(Value::Float(0.0))
    }));
    
    // Create the Shape class
    let shape_class = Value::Class {
        name: "Shape".to_string(),
        bases: vec!["object".to_string()],
        methods: shape_methods,
        metaclass: None,
        mro: crate::base_object::MRO::from_linearization(vec!["Shape".to_string(), "object".to_string()]),
        base_object: crate::base_object::BaseObject::new("Shape".to_string(), vec!["object".to_string()]),
    };
    
    namespace.insert("Shape".to_string(), shape_class);

    // Create a Rectangle class that inherits from Shape
    let mut rectangle_methods = HashMap::new();
    
    // __init__ method for Rectangle
    rectangle_methods.insert("__init__".to_string(), Value::BuiltinFunction("Rectangle.__init__".to_string(), |args| {
        if args.len() < 3 {
            return Err(anyhow::anyhow!("Rectangle.__init__() missing required arguments"));
        }
        
        if let Value::Object { fields, .. } = &args[0] {
            let mut fields_mut = (**fields).clone();
            if let Value::Int(width) = &args[1] {
                fields_mut.insert("width".to_string(), Value::Int(*width));
            }
            if let Value::Int(height) = &args[2] {
                fields_mut.insert("height".to_string(), Value::Int(*height));
            }
            Ok(Value::None)
        } else {
            Err(anyhow::anyhow!("First argument must be an object instance"))
        }
    }));
    
    // area method for Rectangle
    rectangle_methods.insert("area".to_string(), Value::BuiltinFunction("Rectangle.area".to_string(), |args| {
        if args.is_empty() {
            return Err(anyhow::anyhow!("area() missing self argument"));
        }
        
        if let Value::Object { fields, .. } = &args[0] {
            if let (Some(Value::Int(width)), Some(Value::Int(height))) = 
                (fields.get("width"), fields.get("height")) {
                Ok(Value::Float((*width * *height) as f64))
            } else {
                Ok(Value::Float(0.0))
            }
        } else {
            Err(anyhow::anyhow!("First argument must be an object instance"))
        }
    }));
    
    // perimeter method for Rectangle
    rectangle_methods.insert("perimeter".to_string(), Value::BuiltinFunction("Rectangle.perimeter".to_string(), |args| {
        if args.is_empty() {
            return Err(anyhow::anyhow!("perimeter() missing self argument"));
        }
        
        if let Value::Object { fields, .. } = &args[0] {
            if let (Some(Value::Int(width)), Some(Value::Int(height))) = 
                (fields.get("width"), fields.get("height")) {
                Ok(Value::Float((2 * (*width + *height)) as f64))
            } else {
                Ok(Value::Float(0.0))
            }
        } else {
            Err(anyhow::anyhow!("First argument must be an object instance"))
        }
    }));
    
    // Create the Rectangle class
    let rectangle_class = Value::Class {
        name: "Rectangle".to_string(),
        bases: vec!["Shape".to_string()],
        methods: rectangle_methods,
        metaclass: None,
        mro: crate::base_object::MRO::from_linearization(vec!["Rectangle".to_string(), "Shape".to_string(), "object".to_string()]),
        base_object: crate::base_object::BaseObject::new("Rectangle".to_string(), vec!["Shape".to_string()]),
    };
    
    namespace.insert("Rectangle".to_string(), rectangle_class);

    // Add utility functions
    namespace.insert("create_person".to_string(), Value::BuiltinFunction("create_person".to_string(), |args| {
        if args.len() < 2 {
            return Err(anyhow::anyhow!("create_person() missing required arguments"));
        }
        
        // Create a new Person object
        let mut fields = HashMap::new();
        if let Value::Str(name) = &args[0] {
            fields.insert("name".to_string(), Value::Str(name.clone()));
        }
        if let Value::Int(age) = &args[1] {
            fields.insert("age".to_string(), Value::Int(*age));
        }
        fields.insert("friends".to_string(), Value::List(crate::modules::hplist::HPList::new()));
        
        Ok(Value::Object {
            class_name: "Person".to_string(),
            fields: Rc::new(fields),
            class_methods: HashMap::new(), // This would be populated in a real implementation
            base_object: crate::base_object::BaseObject::new("Person".to_string(), vec!["object".to_string()]),
            mro: crate::base_object::MRO::from_linearization(vec!["Person".to_string(), "object".to_string()]),
        })
    }));
    
    namespace.insert("create_rectangle".to_string(), Value::BuiltinFunction("create_rectangle".to_string(), |args| {
        if args.len() < 2 {
            return Err(anyhow::anyhow!("create_rectangle() missing required arguments"));
        }
        
        // Create a new Rectangle object
        let mut fields = HashMap::new();
        if let Value::Int(width) = &args[0] {
            fields.insert("width".to_string(), Value::Int(*width));
        }
        if let Value::Int(height) = &args[1] {
            fields.insert("height".to_string(), Value::Int(*height));
        }
        
        Ok(Value::Object {
            class_name: "Rectangle".to_string(),
            fields: Rc::new(fields),
            class_methods: HashMap::new(), // This would be populated in a real implementation
            base_object: crate::base_object::BaseObject::new("Rectangle".to_string(), vec!["Shape".to_string()]),
            mro: crate::base_object::MRO::from_linearization(vec!["Rectangle".to_string(), "Shape".to_string(), "object".to_string()]),
        })
    }));

    Value::Module("oop_test".to_string(), namespace)
}