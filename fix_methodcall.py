#!/usr/bin/env python3

# Read the file
with open('src/ir.rs', 'r') as f:
    lines = f.readlines()

# Find the MethodCall at line 862 (index 861) and insert super() check
# The structure should be:
# 862:             Expr::MethodCall { object, method, args, .. } => {
# 863:                 // Process method call
# We need to insert the super() check here

insert_lines_at_863 = [
    "                // Check if this is a super() method call\n",
    "                let is_super_call = matches!(object.as_ref(), Expr::Call { func, .. }\n",
    '                    if matches!(func.as_ref(), Expr::Identifier(name) if name == "super"));\n',
    "\n",
    "                if is_super_call {\n",
    "                    // Handle super().method() call\n",
    "                    if let Some(current_class) = &self.current_class {\n",
    "                        // Get the base class name\n",
    "                        let base_classes = self.class_inheritance.get(current_class).cloned().unwrap_or_default();\n",
    '                        let parent_class = base_classes.first().cloned().unwrap_or_else(|| "object".to_string());\n',
    "\n",
    "                        // Process arguments\n",
    "                        let mut arg_names: Vec<String> = Vec::new();\n",
    "                        for (i, arg) in args.iter().enumerate() {\n",
    '                            let arg_result = format!("method_arg_{}", i);\n',
    "                            self.process_expression_for_instructions(instructions, arg)?;\n",
    "                            instructions.push(IRInstruction::LoadGlobal {\n",
    '                                name: "temp_result".to_string(),\n',
    "                                result: arg_result.clone()\n",
    "                            });\n",
    "                            arg_names.push(arg_result);\n",
    "                        }\n",
    "\n",
    "                        // Create the parent method name (ParentClass__method)\n",
    '                        let method_name = format!("{}__{}", parent_class, method);\n',
    "\n",
    "                        // Call the parent method with self as first argument\n",
    '                        let mut method_args = vec!["self".to_string()];\n',
    "                        method_args.extend(arg_names);\n",
    "\n",
    "                        instructions.push(IRInstruction::Call {\n",
    "                            func: method_name,\n",
    "                            args: method_args,\n",
    '                            result: Some("temp_result".to_string())\n',
    "                        });\n",
    "                    } else {\n",
    "                        // super() called outside of a class - error\n",
    "                        instructions.push(IRInstruction::LoadConst {\n",
    "                            value: Value::None,\n",
    '                            result: "temp_result".to_string()\n',
    "                        });\n",
    "                    }\n",
    "                } else {\n",
]

close_brace_at_917 = "                }\n"

# Insert at line 863 (after line 862, which is index 862)
# First, check if it's not already patched
if "is_super_call" not in lines[863]:
    new_lines = lines[:863] + insert_lines_at_863 + lines[863:]

    # Now find where to close the else block (should be after the existing closing brace at line 916)
    # Find line "            }," after the MethodCall block (around line 917 in original, now shifted)
    shift = len(insert_lines_at_863)
    insert_pos_2 = 917 + shift

    # Add the closing brace for the else block
    final_lines = new_lines[:insert_pos_2] + [close_brace_at_917] + new_lines[insert_pos_2:]

    # Write back
    with open('src/ir.rs', 'w') as f:
        f.writelines(final_lines)

    print(f"Patched first MethodCall handler. Added {len(insert_lines_at_863)} lines at 863, closing brace at {insert_pos_2}")
else:
    print("First MethodCall already patched")

# Now fix the second one around line 1295 (but we need to recalculate after the first patch)
with open('src/ir.rs', 'r') as f:
    lines = f.readlines()

# Find the second MethodCall in process_expression (not process_expression_for_instructions)
# Look for the pattern around line 1295 + shift
found_second = False
for i in range(1250, min(1400, len(lines))):
    if "Expr::MethodCall { object, method, args, .. } =>" in lines[i]:
        if "Check if this is a module function call vs a method call" in lines[i+3]:
            # This is the second one, check if not patched
            if "is_super_call" not in lines[i+1]:
                print(f"Found second MethodCall at line {i+1}")
                # Apply the same patch
                new_lines2 = lines[:i+1] + insert_lines_at_863 + lines[i+1:]

                # Find the closing brace
                for j in range(i+55, min(i+120, len(new_lines2))):
                    if new_lines2[j].strip() == "}":
                        if "            }," in new_lines2[j+1]:
                            print(f"Adding closing brace at line {j+1}")
                            final_lines2 = new_lines2[:j+1] + [close_brace_at_917] + new_lines2[j+1:]
                            with open('src/ir.rs', 'w') as f:
                                f.writelines(final_lines2)
                            found_second = True
                            break
                break

if found_second:
    print("Patched second MethodCall handler")
else:
    print("Second MethodCall already patched or not found")

print("Done!")
