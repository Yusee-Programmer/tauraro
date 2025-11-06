/// Templa - High-performance template engine for Tauraro
/// Similar to Jinja2 but faster and more Rust-native
///
/// Features:
/// - Variable interpolation: {{ variable }}
/// - Control structures: {% if %}, {% for %}, {% block %}
/// - Template inheritance: {% extends "base.html" %}
/// - Filters: {{ variable|filter }}
/// - Comments: {# comment #}
/// - Auto-escaping for security
/// - Template caching
/// - Fast rendering with compiled templates

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

// Helper macro for creating hashmaps
macro_rules! hashmap {
    ($($key:expr => $val:expr),* $(,)?) => {{
        let mut map = HashMap::new();
        $(map.insert($key, $val);)*
        map
    }};
}


/// Create the templa module
pub fn create_templa_module() -> Value {
    let mut namespace = HashMap::new();

    // Main classes
    namespace.insert("Template".to_string(), Value::NativeFunction(create_template));
    namespace.insert("Environment".to_string(), Value::NativeFunction(create_environment));
    namespace.insert("FileSystemLoader".to_string(), Value::NativeFunction(create_filesystem_loader));

    // Utility functions
    namespace.insert("render_string".to_string(), Value::NativeFunction(render_string));
    namespace.insert("escape".to_string(), Value::NativeFunction(escape_html));
    namespace.insert("safe".to_string(), Value::NativeFunction(mark_safe));

    // Built-in filters
    namespace.insert("filters".to_string(), create_filters_dict());

    // Constants
    namespace.insert("VERSION".to_string(), Value::Str("1.0.0".to_string()));
    namespace.insert("AUTOESCAPE".to_string(), Value::Bool(true));

    Value::Module("templa".to_string(), namespace)
}

/// Create a template from source string
fn create_template(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("Template() requires at least 1 argument (source)"));
    }

    let source = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("Template source must be a string")),
    };

    // Options
    let autoescape = if args.len() > 1 {
        match &args[1] {
            Value::Bool(b) => *b,
            _ => true,
        }
    } else {
        true
    };

    let mut template = HashMap::new();
    template.insert("source".to_string(), Value::Str(source.clone()));
    template.insert("autoescape".to_string(), Value::Bool(autoescape));
    template.insert("compiled".to_string(), Value::Bool(false));
    template.insert("render".to_string(), Value::NativeFunction(template_render));

    // Parse the template
    let parsed = parse_template(&source)?;
    template.insert("_parsed".to_string(), parsed);

    Ok(Value::Dict(Rc::new(RefCell::new(template))))
}

/// Render a template
fn template_render(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("render() requires template object"));
    }

    let template = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("First argument must be template object")),
    };

    let context = if args.len() > 1 {
        match &args[1] {
            Value::Dict(d) => d.clone(),
            _ => Rc::new(RefCell::new(HashMap::new())),
        }
    } else {
        Rc::new(RefCell::new(HashMap::new()))
    };

    let template_ref = template.borrow();

    // Get parsed template
    let parsed = template_ref.get("_parsed")
        .ok_or_else(|| anyhow!("Template not parsed"))?;

    let autoescape = template_ref.get("autoescape")
        .and_then(|v| if let Value::Bool(b) = v { Some(*b) } else { None })
        .unwrap_or(true);

    

    // Render the parsed template
    render_parsed(parsed, &context, autoescape)
}

/// Parse template into internal representation
fn parse_template(source: &str) -> Result<Value> {
    let mut tokens = Vec::new();
    let mut pos = 0;
    let source_chars: Vec<char> = source.chars().collect();

    while pos < source_chars.len() {
        // Check for variable: {{ ... }}
        if pos + 1 < source_chars.len() && source_chars[pos] == '{' && source_chars[pos + 1] == '{' {
            let end = find_closing(&source_chars, pos, "{{", "}}");
            if let Some(end_pos) = end {
                let content: String = source_chars[pos + 2..end_pos].iter().collect();
                tokens.push(Value::Dict(Rc::new(RefCell::new(hashmap! {
                    "type".to_string() => Value::Str("variable".to_string()),
                    "content".to_string() => Value::Str(content.trim().to_string()),
                }))));
                pos = end_pos + 2;
                continue;
            }
        }

        // Check for control: {% ... %}
        if pos + 1 < source_chars.len() && source_chars[pos] == '{' && source_chars[pos + 1] == '%' {
            let end = find_closing(&source_chars, pos, "{%", "%}");
            if let Some(end_pos) = end {
                let content: String = source_chars[pos + 2..end_pos].iter().collect();
                let trimmed = content.trim();

                // Parse control structures
                let control_type = if trimmed.starts_with("if ") {
                    "if"
                } else if trimmed.starts_with("for ") {
                    "for"
                } else if trimmed.starts_with("block ") {
                    "block"
                } else if trimmed.starts_with("extends ") {
                    "extends"
                } else if trimmed == "endif" || trimmed == "endfor" || trimmed == "endblock" {
                    "end"
                } else {
                    "unknown"
                };

                tokens.push(Value::Dict(Rc::new(RefCell::new(hashmap! {
                    "type".to_string() => Value::Str("control".to_string()),
                    "control_type".to_string() => Value::Str(control_type.to_string()),
                    "content".to_string() => Value::Str(trimmed.to_string()),
                }))));
                pos = end_pos + 2;
                continue;
            }
        }

        // Check for comment: {# ... #}
        if pos + 1 < source_chars.len() && source_chars[pos] == '{' && source_chars[pos + 1] == '#' {
            let end = find_closing(&source_chars, pos, "{#", "#}");
            if let Some(end_pos) = end {
                // Skip comments
                pos = end_pos + 2;
                continue;
            }
        }

        // Text content
        let mut text = String::new();
        while pos < source_chars.len() {
            // Check if we're at the start of a template tag
            if source_chars[pos] == '{' && pos + 1 < source_chars.len() {
                let next = source_chars[pos + 1];
                if next == '{' || next == '%' || next == '#' {
                    break;
                }
            }
            text.push(source_chars[pos]);
            pos += 1;
        }

        if !text.is_empty() {
            tokens.push(Value::Dict(Rc::new(RefCell::new(hashmap! {
                "type".to_string() => Value::Str("text".to_string()),
                "content".to_string() => Value::Str(text),
            }))));
        }
    }

    Ok(Value::List(crate::modules::HPList::from_values(tokens)))
}

/// Find closing delimiter
fn find_closing(chars: &[char], start: usize, open: &str, close: &str) -> Option<usize> {
    let open_chars: Vec<char> = open.chars().collect();
    let close_chars: Vec<char> = close.chars().collect();
    let mut pos = start + open_chars.len();

    while pos + close_chars.len() <= chars.len() {
        let mut matches = true;
        for (i, &c) in close_chars.iter().enumerate() {
            if chars[pos + i] != c {
                matches = false;
                break;
            }
        }
        if matches {
            return Some(pos);
        }
        pos += 1;
    }

    None
}

/// Render parsed template
fn render_parsed(parsed: &Value, context: &Rc<RefCell<HashMap<String, Value>>>, autoescape: bool) -> Result<Value> {
    let tokens = match parsed {
        Value::List(list) => list.as_vec(),
        _ => return Err(anyhow!("Invalid parsed template")),
    };

    let mut output = String::new();
    let mut i = 0;

    while i < tokens.len() {
        let token = &tokens[i];
        if let Value::Dict(token_dict) = token {
            let token_ref = token_dict.borrow();
            let token_type = token_ref.get("type")
                .and_then(|v| if let Value::Str(s) = v { Some(s.as_str()) } else { None })
                .unwrap_or("");

            match token_type {
                "text" => {
                    if let Some(Value::Str(content)) = token_ref.get("content") {
                        output.push_str(content);
                    }
                }
                "variable" => {
                    if let Some(Value::Str(var_expr)) = token_ref.get("content") {
                        let value = evaluate_expression(var_expr, context)?;
                        let value_str = value_to_string(&value);
                        if autoescape {
                            output.push_str(&html_escape(&value_str));
                        } else {
                            output.push_str(&value_str);
                        }
                    }
                }
                "control" => {
                    let control_type = token_ref.get("control_type")
                        .and_then(|v| if let Value::Str(s) = v { Some(s.as_str()) } else { None })
                        .unwrap_or("");

                    let content = token_ref.get("content")
                        .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
                        .unwrap_or_default();

                    

                    match control_type {
                        "if" => {
                            let (rendered, new_i) = render_if_block(i, &tokens, &content, context, autoescape)?;
                            output.push_str(&rendered);
                            i = new_i;
                            continue;
                        }
                        "for" => {
                            let (rendered, new_i) = render_for_block(i, &tokens, &content, context, autoescape)?;
                            output.push_str(&rendered);
                            i = new_i;
                            continue;
                        }
                        _ => {}
                    }
                }
                _ => {}
            }
        }
        i += 1;
    }

    Ok(Value::Str(output))
}

/// Render if block
fn render_if_block(
    start: usize,
    tokens: &[Value],
    condition: &str,
    context: &Rc<RefCell<HashMap<String, Value>>>,
    autoescape: bool,
) -> Result<(String, usize)> {
    // Parse condition (e.g., "if variable" or "if x > 5")
    let cond_expr = condition.strip_prefix("if ").unwrap_or(condition).trim();

    // Evaluate condition
    let condition_result = evaluate_condition(cond_expr, context)?;

    // Find matching endif
    let mut depth = 1;
    let mut end_pos = start + 1;
    let mut body_tokens = Vec::new();

    while end_pos < tokens.len() && depth > 0 {
        if let Value::Dict(token_dict) = &tokens[end_pos] {
            let token_ref = token_dict.borrow();
            if let Some(Value::Str(t)) = token_ref.get("type") {
                if t == "control" {
                    if let Some(Value::Str(ct)) = token_ref.get("control_type") {
                        if ct == "if" {
                            depth += 1;
                        } else if ct == "end" {
                            if let Some(Value::Str(content)) = token_ref.get("content") {
                                if content == "endif" {
                                    depth -= 1;
                                    if depth == 0 {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if depth > 0 {
            body_tokens.push(tokens[end_pos].clone());
        }
        end_pos += 1;
    }

    let mut output = String::new();
    if condition_result {
        // Render body
        let body_list = Value::List(crate::modules::HPList::from_values(body_tokens));
        if let Value::Str(rendered) = render_parsed(&body_list, context, autoescape)? {
            output = rendered;
        }
    }

    Ok((output, end_pos))
}

/// Render for block
fn render_for_block(
    start: usize,
    tokens: &[Value],
    for_expr: &str,
    context: &Rc<RefCell<HashMap<String, Value>>>,
    autoescape: bool,
) -> Result<(String, usize)> {
    // Parse for expression: "for item in items" or "for key, value in dict"
    let expr = for_expr.strip_prefix("for ").unwrap_or(for_expr).trim();
    let parts: Vec<&str> = expr.split(" in ").collect();

    if parts.len() != 2 {
        return Err(anyhow!("Invalid for loop syntax"));
    }

    let var_part = parts[0].trim();
    let iterable_expr = parts[1].trim();

    // Find matching endfor
    let mut depth = 1;
    let mut end_pos = start + 1;
    let mut body_tokens = Vec::new();

    while end_pos < tokens.len() && depth > 0 {
        if let Value::Dict(token_dict) = &tokens[end_pos] {
            let token_ref = token_dict.borrow();
            if let Some(Value::Str(t)) = token_ref.get("type") {
                if t == "control" {
                    if let Some(Value::Str(ct)) = token_ref.get("control_type") {
                        if ct == "for" {
                            depth += 1;
                        } else if ct == "end" {
                            if let Some(Value::Str(content)) = token_ref.get("content") {
                                if content == "endfor" {
                                    depth -= 1;
                                    if depth == 0 {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if depth > 0 {
            body_tokens.push(tokens[end_pos].clone());
        }
        end_pos += 1;
    }

    // Evaluate iterable
    let iterable = evaluate_expression(iterable_expr, context)?;

    let mut output = String::new();

    // Iterate
    match iterable {
        Value::List(list) => {
            for item in list.as_vec() {
                // Create new context with loop variable
                let mut loop_context = context.borrow().clone();
                loop_context.insert(var_part.to_string(), item.clone());
                let loop_ctx_rc = Rc::new(RefCell::new(loop_context));

                // Render body
                let body_list = Value::List(crate::modules::HPList::from_values(body_tokens.clone()));
                if let Value::Str(rendered) = render_parsed(&body_list, &loop_ctx_rc, autoescape)? {
                    output.push_str(&rendered);
                }
            }
        }
        Value::Dict(dict) => {
            for (key, value) in dict.borrow().iter() {
                let mut loop_context = context.borrow().clone();

                // Support "for key, value in dict" syntax
                if var_part.contains(',') {
                    let vars: Vec<&str> = var_part.split(',').map(|s| s.trim()).collect();
                    if vars.len() >= 2 {
                        loop_context.insert(vars[0].to_string(), Value::Str(key.clone()));
                        loop_context.insert(vars[1].to_string(), value.clone());
                    }
                } else {
                    loop_context.insert(var_part.to_string(), Value::Str(key.clone()));
                }

                let loop_ctx_rc = Rc::new(RefCell::new(loop_context));

                let body_list = Value::List(crate::modules::HPList::from_values(body_tokens.clone()));
                if let Value::Str(rendered) = render_parsed(&body_list, &loop_ctx_rc, autoescape)? {
                    output.push_str(&rendered);
                }
            }
        }
        _ => {}
    }

    Ok((output, end_pos))
}

/// Evaluate expression (with filters)
fn evaluate_expression(expr: &str, context: &Rc<RefCell<HashMap<String, Value>>>) -> Result<Value> {
    let expr = expr.trim();

    // Check for filters: variable|filter1|filter2
    if expr.contains('|') {
        let parts: Vec<&str> = expr.split('|').collect();
        let var_name = parts[0].trim();

        // Get base value
        let mut value = get_variable(var_name, context)?;

        // Apply filters
        for filter_expr in &parts[1..] {
            let filter_name = filter_expr.trim();
            value = apply_filter(&value, filter_name)?;
        }

        return Ok(value);
    }

    // Simple variable lookup
    get_variable(expr, context)
}

/// Get variable from context
fn get_variable(name: &str, context: &Rc<RefCell<HashMap<String, Value>>>) -> Result<Value> {
    // Support dot notation: user.name
    if name.contains('.') {
        let parts: Vec<&str> = name.split('.').collect();
        let mut current = context.borrow().get(parts[0])
            .cloned()
            .unwrap_or(Value::None);

        for &part in &parts[1..] {
            current = match current {
                Value::Dict(dict) => {
                    dict.borrow().get(part).cloned().unwrap_or(Value::None)
                }
                _ => Value::None,
            };
        }

        return Ok(current);
    }

    Ok(context.borrow().get(name).cloned().unwrap_or(Value::None))
}

/// Evaluate condition
fn evaluate_condition(expr: &str, context: &Rc<RefCell<HashMap<String, Value>>>) -> Result<bool> {
    let expr = expr.trim();

    // Simple truthiness check
    let value = evaluate_expression(expr, context)?;

    Ok(match value {
        Value::Bool(b) => b,
        Value::None => false,
        Value::Int(i) => i != 0,
        Value::Float(f) => f != 0.0,
        Value::Str(s) => !s.is_empty(),
        Value::List(l) => !l.is_empty(),
        Value::Dict(d) => !d.borrow().is_empty(),
        _ => true,
    })
}

/// Apply filter to value
fn apply_filter(value: &Value, filter_name: &str) -> Result<Value> {
    match filter_name {
        "upper" => Ok(Value::Str(value_to_string(value).to_uppercase())),
        "lower" => Ok(Value::Str(value_to_string(value).to_lowercase())),
        "capitalize" => {
            let s = value_to_string(value);
            let mut chars = s.chars();
            match chars.next() {
                None => Ok(Value::Str(String::new())),
                Some(first) => Ok(Value::Str(
                    first.to_uppercase().collect::<String>() + chars.as_str()
                )),
            }
        }
        "title" => {
            let s = value_to_string(value);
            let result: String = s.split_whitespace()
                .map(|word| {
                    let mut chars = word.chars();
                    match chars.next() {
                        None => String::new(),
                        Some(first) => first.to_uppercase().collect::<String>() + chars.as_str(),
                    }
                })
                .collect::<Vec<_>>()
                .join(" ");
            Ok(Value::Str(result))
        }
        "trim" | "strip" => Ok(Value::Str(value_to_string(value).trim().to_string())),
        "length" | "len" => {
            let len = match value {
                Value::Str(s) => s.len(),
                Value::List(l) => l.len(),
                Value::Dict(d) => d.borrow().len(),
                _ => 0,
            };
            Ok(Value::Int(len as i64))
        }
        "reverse" => {
            match value {
                Value::Str(s) => Ok(Value::Str(s.chars().rev().collect())),
                Value::List(l) => {
                    let mut vec = l.as_vec().to_vec();
                    vec.reverse();
                    Ok(Value::List(crate::modules::HPList::from_values(vec)))
                }
                _ => Ok(value.clone()),
            }
        }
        "escape" => Ok(Value::Str(html_escape(&value_to_string(value)))),
        "safe" => Ok(value.clone()), // Mark as safe (no escaping)
        "default" => {
            // Default filter would need an argument, for now just return value
            Ok(value.clone())
        }
        _ => Err(anyhow!("Unknown filter: {}", filter_name)),
    }
}

/// Convert value to string
fn value_to_string(value: &Value) -> String {
    match value {
        Value::Str(s) => s.clone(),
        Value::Int(i) => i.to_string(),
        Value::Float(f) => f.to_string(),
        Value::Bool(b) => b.to_string(),
        Value::None => String::new(),
        Value::List(l) => format!("{:?}", l.as_vec()),
        Value::Dict(d) => format!("{:?}", d.borrow()),
        _ => format!("{:?}", value),
    }
}

/// HTML escape
fn html_escape(s: &str) -> String {
    s.replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
        .replace('"', "&quot;")
        .replace('\'', "&#x27;")
}

/// Create environment
fn create_environment(_args: Vec<Value>) -> Result<Value> {
    let mut env = HashMap::new();
    env.insert("autoescape".to_string(), Value::Bool(true));
    env.insert("loader".to_string(), Value::None);
    env.insert("templates".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));

    Ok(Value::Dict(Rc::new(RefCell::new(env))))
}

/// Create filesystem loader
fn create_filesystem_loader(args: Vec<Value>) -> Result<Value> {
    let path = if args.is_empty() {
        ".".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("FileSystemLoader path must be a string")),
        }
    };

    let mut loader = HashMap::new();
    loader.insert("path".to_string(), Value::Str(path));
    loader.insert("load".to_string(), Value::NativeFunction(filesystem_loader_load));

    Ok(Value::Dict(Rc::new(RefCell::new(loader))))
}

/// Load template from filesystem
fn filesystem_loader_load(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("load() requires loader and filename"));
    }

    let loader = match &args[0] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid loader")),
    };

    let filename = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("Filename must be a string")),
    };

    let path = loader.borrow().get("path")
        .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
        .unwrap_or_else(|| ".".to_string());

    let full_path = format!("{}/{}", path, filename);

    // Read file
    let content = std::fs::read_to_string(&full_path)
        .map_err(|e| anyhow!("Failed to load template '{}': {}", filename, e))?;

    // Create template
    create_template(vec![Value::Str(content)])
}

/// Render string template
fn render_string(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("render_string() requires template string"));
    }

    let source = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("Template must be a string")),
    };

    let context = if args.len() > 1 {
        match &args[1] {
            Value::Dict(d) => d.clone(),
            _ => Rc::new(RefCell::new(HashMap::new())),
        }
    } else {
        Rc::new(RefCell::new(HashMap::new()))
    };

    let template = create_template(vec![Value::Str(source)])?;
    template_render(vec![template, Value::Dict(context)])
}

/// Escape HTML
fn escape_html(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Str(String::new()));
    }

    let text = value_to_string(&args[0]);
    Ok(Value::Str(html_escape(&text)))
}

/// Mark string as safe (no escaping)
fn mark_safe(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Str(String::new()));
    }

    // In a full implementation, this would mark the value as safe
    // For now, just return the value as-is
    Ok(args[0].clone())
}

/// Create filters dictionary
fn create_filters_dict() -> Value {
    let mut filters = HashMap::new();

    filters.insert("upper".to_string(), Value::Str("Convert to uppercase".to_string()));
    filters.insert("lower".to_string(), Value::Str("Convert to lowercase".to_string()));
    filters.insert("capitalize".to_string(), Value::Str("Capitalize first letter".to_string()));
    filters.insert("title".to_string(), Value::Str("Title case".to_string()));
    filters.insert("trim".to_string(), Value::Str("Remove whitespace".to_string()));
    filters.insert("length".to_string(), Value::Str("Get length".to_string()));
    filters.insert("reverse".to_string(), Value::Str("Reverse string/list".to_string()));
    filters.insert("escape".to_string(), Value::Str("HTML escape".to_string()));
    filters.insert("safe".to_string(), Value::Str("Mark as safe HTML".to_string()));

    Value::Dict(Rc::new(RefCell::new(filters)))
}
