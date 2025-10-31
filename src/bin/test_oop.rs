use tauraro::vm::core::VM;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Simple OOP test
    let code = r#"
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    def greet(self):
        return f"Hello, my name is {self.name} and I am {self.age} years old"

# Create an instance
person = Person("Alice", 30)

# Test attribute access
print(person.name)  # Should print "Alice"
print(person.age)   # Should print 30

# Test method call
print(person.greet())  # Should print "Hello, my name is Alice and I am 30 years old"

# Test attribute assignment
person.age = 31
print(person.age)   # Should print 31

# Test isinstance
print(isinstance(person, Person))  # Should print True
"#;

    // Run the code using the VM
    VM::run_file_with_options(code, "test_oop.tr", "vm", 0, false)?;
    
    Ok(())
}