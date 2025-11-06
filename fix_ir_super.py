#!/usr/bin/env python3
import re

# Read the IR file
with open('src/ir.rs', 'r') as f:
    content = f.read()

# Pattern to match the MethodCall handling in process_expression_for_instructions
# We need to wrap the existing logic with a check for super()

# Find all MethodCall pattern locations (we already fixed one, need to find the other)
# Look for the pattern at line 863 and 1295

# For process_expression_for_instructions at around line 863
old_pattern_1 = r'''            Expr::MethodCall \{ object, method, args, \.\. \} => \{
                // Process method call
                let object_name = self\.expression_to_string\(&object\);

                // Check if this is a module function call vs a method call
                if self\.imported_modules\.contains\(&object_name\) \{'''

new_code_1 = '''            Expr::MethodCall { object, method, args, .. } => {
                // Check if this is a super() method call
                let is_super_call = matches!(object.as_ref(), Expr::Call { func, .. }
                    if matches!(func.as_ref(), Expr::Identifier(name) if name == "super"));

                if is_super_call {
                    // Handle super().method() call
                    if let Some(current_class) = &self.current_class {
                        // Get the base class name
                        let base_classes = self.class_inheritance.get(current_class).cloned().unwrap_or_default();
                        let parent_class = base_classes.first().cloned().unwrap_or_else(|| "object".to_string());

                        // Process arguments
                        let mut arg_names: Vec<String> = Vec::new();
                        for (i, arg) in args.iter().enumerate() {
                            let arg_result = format!("method_arg_{}", i);
                            self.process_expression_for_instructions(instructions, arg)?;
                            instructions.push(IRInstruction::LoadGlobal {
                                name: "temp_result".to_string(),
                                result: arg_result.clone()
                            });
                            arg_names.push(arg_result);
                        }

                        // Create the parent method name (ParentClass__method)
                        let method_name = format!("{}__{}", parent_class, method);

                        // Call the parent method with self as first argument
                        let mut method_args = vec!["self".to_string()];
                        method_args.extend(arg_names);

                        instructions.push(IRInstruction::Call {
                            func: method_name,
                            args: method_args,
                            result: Some("temp_result".to_string())
                        });
                    } else {
                        // super() called outside of a class - error
                        instructions.push(IRInstruction::LoadConst {
                            value: Value::None,
                            result: "temp_result".to_string()
                        });
                    }
                } else {
                    // Process method call
                    let object_name = self.expression_to_string(&object);

                    // Check if this is a module function call vs a method call
                    if self.imported_modules.contains(&object_name) {'''

# Find and count occurrences
matches = list(re.finditer(old_pattern_1, content))
print(f"Found {len(matches)} matches of the pattern")

# For now, let's do a simpler approach - find the exact line numbers and patch manually
lines = content.split('\n')

# Find line 863
for i, line in enumerate(lines[860:920], start=860):
    print(f"{i}: {line[:80]}")

print("\n\nDone analyzing. Need to patch manually.")
