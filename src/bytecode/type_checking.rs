//! Type checking bytecode execution helpers

use crate::ast::Type;
use crate::value::Value;
use crate::type_checker::TypeChecker;
use anyhow::{Result, anyhow};

/// Parse a type string into a Type AST node
/// This is a simplified parser for common types
pub fn parse_type_string(type_str: &str) -> Result<Type> {
    let type_str = type_str.trim();

    // Handle Any
    if type_str == "Any" {
        return Ok(Type::Any);
    }

    // Handle None/NoneType
    if type_str == "None" || type_str == "NoneType" {
        return Ok(Type::Simple("None".to_string()));
    }

    // Handle Optional types (e.g., "int?" or "Optional[int]")
    if type_str.ends_with('?') {
        let inner_type = parse_type_string(&type_str[..type_str.len() - 1])?;
        return Ok(Type::Optional(Box::new(inner_type)));
    }
    if type_str.starts_with("Optional[") && type_str.ends_with(']') {
        let inner = &type_str[9..type_str.len() - 1];
        let inner_type = parse_type_string(inner)?;
        return Ok(Type::Optional(Box::new(inner_type)));
    }

    // Handle Union types (e.g., "int | str" or "Union[int, str]")
    if type_str.contains(" | ") {
        let parts: Vec<&str> = type_str.split(" | ").collect();
        let types: Result<Vec<Type>> = parts.iter().map(|p| parse_type_string(p)).collect();
        return Ok(Type::Union(types?));
    }
    if type_str.starts_with("Union[") && type_str.ends_with(']') {
        let inner = &type_str[6..type_str.len() - 1];
        let parts: Vec<&str> = inner.split(", ").collect();
        let types: Result<Vec<Type>> = parts.iter().map(|p| parse_type_string(p)).collect();
        return Ok(Type::Union(types?));
    }

    // Handle generic types (e.g., "List[int]", "Dict[str, int]", "Tuple[int, str]")
    if let Some(bracket_pos) = type_str.find('[') {
        if type_str.ends_with(']') {
            let name = &type_str[..bracket_pos];
            let args_str = &type_str[bracket_pos + 1..type_str.len() - 1];

            // Parse the type arguments
            let args: Result<Vec<Type>> = if args_str.contains(',') {
                args_str.split(", ").map(|s| parse_type_string(s.trim())).collect()
            } else {
                vec![parse_type_string(args_str.trim())].into_iter().collect()
            };

            return Ok(Type::Generic {
                name: name.to_string(),
                args: args?,
            });
        }
    }

    // Handle tuple types (e.g., "(int, str)")
    if type_str.starts_with('(') && type_str.ends_with(')') {
        let inner = &type_str[1..type_str.len() - 1];
        if inner.contains(',') {
            let parts: Vec<&str> = inner.split(", ").collect();
            let types: Result<Vec<Type>> = parts.iter().map(|p| parse_type_string(p)).collect();
            return Ok(Type::Tuple(types?));
        }
    }

    // Handle simple types (e.g., "int", "str", "bool", etc.)
    Ok(Type::Simple(type_str.to_string()))
}

/// Check if a value matches a type string
pub fn check_value_against_type_string(
    value: &Value,
    type_str: &str,
    type_checker: &TypeChecker,
) -> Result<()> {
    let parsed_type = parse_type_string(type_str)?;
    if type_checker.value_matches_type(value, &parsed_type) {
        Ok(())
    } else {
        Err(anyhow!(
            "TypeError: Expected type '{}', but got value of type '{}'",
            type_str,
            value.type_name()
        ))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_simple_types() {
        assert!(matches!(parse_type_string("int"), Ok(Type::Simple(_))));
        assert!(matches!(parse_type_string("str"), Ok(Type::Simple(_))));
        assert!(matches!(parse_type_string("bool"), Ok(Type::Simple(_))));
    }

    #[test]
    fn test_parse_optional_types() {
        assert!(matches!(parse_type_string("int?"), Ok(Type::Optional(_))));
        assert!(matches!(parse_type_string("Optional[int]"), Ok(Type::Optional(_))));
    }

    #[test]
    fn test_parse_union_types() {
        assert!(matches!(parse_type_string("int | str"), Ok(Type::Union(_))));
        assert!(matches!(parse_type_string("Union[int, str]"), Ok(Type::Union(_))));
    }

    #[test]
    fn test_parse_generic_types() {
        assert!(matches!(parse_type_string("List[int]"), Ok(Type::Generic { .. })));
        assert!(matches!(parse_type_string("Dict[str, int]"), Ok(Type::Generic { .. })));
    }

    #[test]
    fn test_parse_any_type() {
        assert!(matches!(parse_type_string("Any"), Ok(Type::Any)));
    }
}
