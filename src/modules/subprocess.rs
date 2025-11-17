/// Subprocess module - provides process creation and management functionality
/// Similar to Python's subprocess module

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::process::{Command, Stdio};
use std::cell::RefCell;

/// Create the subprocess module object with all its functions and constants
pub fn create_subprocess_module() -> Value {
    let mut namespace = HashMap::new();

    // Main functions
    namespace.insert("run".to_string(), Value::NativeFunction(subprocess_run));
    namespace.insert("call".to_string(), Value::NativeFunction(subprocess_call));
    namespace.insert("check_call".to_string(), Value::NativeFunction(subprocess_check_call));
    namespace.insert("check_output".to_string(), Value::NativeFunction(subprocess_check_output));
    namespace.insert("getoutput".to_string(), Value::NativeFunction(subprocess_getoutput));
    namespace.insert("getstatusoutput".to_string(), Value::NativeFunction(subprocess_getstatusoutput));

    // Constants for stdio
    namespace.insert("PIPE".to_string(), Value::Int(-1));
    namespace.insert("STDOUT".to_string(), Value::Int(-2));
    namespace.insert("DEVNULL".to_string(), Value::Int(-3));

    Value::Module("subprocess".to_string(), namespace)
}

/// Run command and wait for it to complete
fn subprocess_run(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("run() requires at least 1 argument"));
    }

    // Parse command
    let cmd_str = match &args[0] {
        Value::Str(s) => s.clone(),
        Value::List(list) => {
            let items = list.as_vec();
            if items.is_empty() {
                return Err(anyhow!("Empty command list"));
            }
            items.iter()
                .map(|v| if let Value::Str(s) = v { s.clone() } else { String::new() })
                .collect::<Vec<_>>()
                .join(" ")
        }
        _ => return Err(anyhow!("Command must be string or list")),
    };

    // Execute command
    let output = if cfg!(target_os = "windows") {
        Command::new("cmd")
            .args(&["/C", &cmd_str])
            .output()?
    } else {
        Command::new("sh")
            .args(&["-c", &cmd_str])
            .output()?
    };

    let returncode = output.status.code().unwrap_or(-1);

    // Return CompletedProcess-like dict
    let mut result = HashMap::new();
    result.insert("returncode".to_string(), Value::Int(returncode as i64));
    result.insert("stdout".to_string(), Value::Bytes(output.stdout));
    result.insert("stderr".to_string(), Value::Bytes(output.stderr));

    Ok(Value::Dict(std::rc::Rc::new(std::cell::RefCell::new(result))))
}

/// call() - Run command and return returncode
fn subprocess_call(args: Vec<Value>) -> Result<Value> {
    let result = subprocess_run(args)?;
    if let Value::Dict(dict) = result {
        if let Some(Value::Int(code)) = dict.borrow().get("returncode") {
            return Ok(Value::Int(*code));
        }
    }
    Ok(Value::Int(-1))
}

/// check_call() - Run command, raise error if fails
fn subprocess_check_call(args: Vec<Value>) -> Result<Value> {
    let result = subprocess_run(args)?;
    if let Value::Dict(dict) = result {
        if let Some(Value::Int(code)) = dict.borrow().get("returncode") {
            if *code != 0 {
                return Err(anyhow!("Command returned non-zero exit status {}", code));
            }
        }
    }
    Ok(Value::Int(0))
}

/// check_output() - Run command and return stdout, raise error if fails
fn subprocess_check_output(args: Vec<Value>) -> Result<Value> {
    let result = subprocess_run(args)?;
    if let Value::Dict(dict) = result {
        let dict_borrow = dict.borrow();
        if let Some(Value::Int(code)) = dict_borrow.get("returncode") {
            if *code != 0 {
                return Err(anyhow!("Command returned non-zero exit status {}", code));
            }
        }
        if let Some(stdout) = dict_borrow.get("stdout") {
            return Ok(stdout.clone());
        }
    }
    Ok(Value::Bytes(vec![]))
}

/// getoutput() - Run command and return output as string
fn subprocess_getoutput(args: Vec<Value>) -> Result<Value> {
    let output = subprocess_check_output(args)?;
    if let Value::Bytes(bytes) = output {
        let s = String::from_utf8_lossy(&bytes).trim_end().to_string();
        return Ok(Value::Str(s));
    }
    Ok(Value::Str(String::new()))
}

/// getstatusoutput() - Run command and return (status, output)
fn subprocess_getstatusoutput(args: Vec<Value>) -> Result<Value> {
    let result = subprocess_run(args)?;
    if let Value::Dict(dict) = result {
        let dict_borrow = dict.borrow();
        let returncode = dict_borrow.get("returncode")
            .and_then(|v| if let Value::Int(i) = v { Some(*i) } else { None })
            .unwrap_or(-1);
        let stdout = dict_borrow.get("stdout")
            .and_then(|v| if let Value::Bytes(b) = v { Some(b.clone()) } else { None })
            .unwrap_or_default();

        let output_str = String::from_utf8_lossy(&stdout).trim_end().to_string();
        let tuple = vec![Value::Int(returncode), Value::Str(output_str)];
        return Ok(Value::Tuple(tuple));
    }
    Ok(Value::Tuple(vec![Value::Int(-1), Value::Str(String::new())]))
}