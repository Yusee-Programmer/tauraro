//! COMPLETE Python interoperability for TauraroLang
use pyo3::prelude::*;
use pyo3::wrap_pyfunction;
use pyo3::{Python, PyObject, PyResult};
use pyo3::types::{PyDict, PyList, PyTuple};
use anyhow::Result;
use std::collections::HashMap;
use crate::vm::VM;
use crate::value::Value;

/// Python interop manager
pub struct PythonInterop<'py> {
    py: Python<'py>,
}

impl<'py> PythonInterop<'py> {
    pub fn new(py: Python<'py>) -> Self {
        Self { py }
    }
    
    /// Check if Python is available
    pub fn is_available() -> bool {
        std::panic::catch_unwind(|| {
            Python::with_gil(|_py| true)
        }).is_ok()
    }
    
    /// Import Python module into TauraroLang
    pub fn import_module(&self, module_name: &str) -> Result<PyObject> {
        let module = self.py.import(module_name)?;
        Ok(module.to_object(self.py))
    }
    
    /// Call Python function from TauraroLang
    pub fn call_function(&self, py_obj: &PyObject, function_name: &str, args: Vec<Value>) -> Result<Value> {
        let args_py = self.tauraro_values_to_python(args)?;
        let result = py_obj.call_method(self.py, function_name, (args_py,), None)?;
        self.python_to_tauraro_value(result)
    }
    
    /// Evaluate Python code from TauraroLang
    pub fn eval_python_code(&self, code: &str) -> Result<Value> {
        let result = self.py.eval(code, None, None)?;
        self.python_to_tauraro_value(result.into())
    }
    
    /// Execute Python script
    pub fn exec_python_script(&self, code: &str) -> Result<()> {
        self.py.run(code, None, None)?;
        Ok(())
    }
    
    /// Convert Tauraro values to Python objects
    fn tauraro_values_to_python(&self, values: Vec<Value>) -> Result<&PyTuple> {
        let py_values: Result<Vec<PyObject>> = values
            .into_iter()
            .map(|v| self.tauraro_value_to_python(v))
            .collect();
        
        Ok(PyTuple::new(self.py, &py_values?))
    }
    
    /// Convert single Tauraro value to Python object
    fn tauraro_value_to_python(&self, value: Value) -> Result<PyObject> {
        match value {
            Value::Int(n) => Ok(n.into_py(self.py)),
            Value::Float(n) => Ok(n.into_py(self.py)),
            Value::Bool(b) => Ok(b.into_py(self.py)),
            Value::Str(s) => Ok(s.into_py(self.py)),
            Value::List(items) => {
                let py_list = PyList::empty(self.py);
                for item in items {
                    let py_item = self.tauraro_value_to_python(item)?;
                    py_list.append(py_item)?;
                }
                Ok(py_list.into_py(self.py))
            }
            Value::Dict(dict) => {
                let py_dict = PyDict::new(self.py);
                for (k, v) in dict {
                    let py_key = self.tauraro_value_to_python(Value::Str(k))?;
                    let py_value = self.tauraro_value_to_python(v)?;
                    py_dict.set_item(py_key, py_value)?;
                }
                Ok(py_dict.into_py(self.py))
            }
            Value::None => Ok(self.py.None()),
            Value::Super(current_class, parent_class) => {
                // Convert super object to a string representation for Python
                let super_str = format!("<super: {} -> {}>", current_class, parent_class);
                Ok(super_str.into_py(self.py))
            }
            _ => Err(anyhow::anyhow!("Unsupported value type for Python conversion")),
        }
    }
    
    /// Convert Python object to Tauraro value
    fn python_to_tauraro_value(&self, obj: PyObject) -> Result<Value> {
        let py_obj = obj.as_ref(self.py);
        
        if py_obj.is_none() {
            return Ok(Value::None);
        }
        
        if let Ok(int_val) = py_obj.extract::<i64>() {
            return Ok(Value::Int(int_val));
        }
        
        if let Ok(float_val) = py_obj.extract::<f64>() {
            return Ok(Value::Float(float_val));
        }
        
        if let Ok(bool_val) = py_obj.extract::<bool>() {
            return Ok(Value::Bool(bool_val));
        }
        
        if let Ok(string_val) = py_obj.extract::<String>() {
            return Ok(Value::Str(string_val));
        }
        
        if let Ok(list_val) = py_obj.downcast::<PyList>() {
            let mut items = Vec::new();
            for item in list_val.iter() {
                items.push(self.python_to_tauraro_value(item.to_object(self.py))?);
            }
            return Ok(Value::List(items));
        }
        
        if let Ok(dict_val) = py_obj.downcast::<PyDict>() {
            let mut dict = HashMap::new();
            for (key, value) in dict_val.iter() {
                let key_str = key.extract::<String>()?;
                let value_val = self.python_to_tauraro_value(value.to_object(self.py))?;
                dict.insert(key_str, value_val);
            }
            return Ok(Value::Dict(dict));
        }
        
        // Fallback: return as string representation
        Ok(Value::Str(py_obj.repr()?.extract()?))
    }
    
    /// Get Python version information
    pub fn get_python_info(&self) -> Result<String> {
        let sys = self.py.import("sys")?;
        let version: String = sys.getattr("version")?.extract()?;
        let platform: String = sys.getattr("platform")?.extract()?;
        Ok(format!("Python {} on {}", version, platform))
    }
    
    /// Install Python package using pip
    pub fn install_package(&self, package_name: &str) -> Result<()> {
        let code = format!("import subprocess; subprocess.check_call(['pip', 'install', '{}'])", package_name);
        self.py.run(&code, None, None)?;
        Ok(())
    }
    
    /// Check if Python package is available
    pub fn is_package_available(&self, package_name: &str) -> bool {
        self.py.import(package_name).is_ok()
    }
}

// PyO3 bindings for TauraroLang functions
#[pymodule]
fn tauraro(_py: Python, m: &PyModule) -> PyResult<()> {
    m.add_function(wrap_pyfunction!(tauraro_eval, m)?)?;
    m.add_function(wrap_pyfunction!(tauraro_exec, m)?)?;
    m.add_function(wrap_pyfunction!(tauraro_call, m)?)?;
    m.add_class::<TauraroVM>()?;
    Ok(())
}

#[pyfunction]
fn tauraro_eval(code: &str) -> PyResult<String> {
    let mut vm = crate::vm::VM::new();
    match vm.execute_script(code, vec![]) {
        Ok(result) => Ok(result.to_string()),
        Err(e) => Err(PyErr::new::<pyo3::exceptions::PyRuntimeError, _>(e.to_string())),
    }
}

#[pyfunction]
fn tauraro_exec(code: &str) -> PyResult<()> {
    let mut vm = crate::vm::VM::new();
    vm.execute_script(code, Vec::new())
        .map_err(|e| PyErr::new::<pyo3::exceptions::PyRuntimeError, _>(e.to_string()))?;
    Ok(())
}

#[pyfunction]
fn tauraro_call(function_name: &str, args: Vec<String>) -> PyResult<String> {
    let code = format!("{}({})", function_name, args.join(", "));
    tauraro_eval(&code)
}

#[pyclass]
struct TauraroVM {
    vm: crate::vm::VM,
}

#[pymethods]
impl TauraroVM {
    #[new]
    fn new() -> Self {
        Self {
            vm: crate::vm::VM::new(),
        }
    }
    
    fn eval(&mut self, code: &str) -> PyResult<String> {
        match self.vm.execute_script(code, vec![]) {
            Ok(result) => Ok(result.to_string()),
            Err(e) => Err(PyErr::new::<pyo3::exceptions::PyRuntimeError, _>(e.to_string())),
        }
    }
    
    fn exec(&mut self, code: &str) -> PyResult<()> {
        self.vm.execute_script(code, Vec::new())
            .map_err(|e| PyErr::new::<pyo3::exceptions::PyRuntimeError, _>(e.to_string()))?;
        Ok(())
    }
    
    fn set_strict_types(&mut self, strict: bool) {
        self.vm.set_strict_types(strict);
    }
    
    fn get_memory_stats(&self) -> String {
        self.vm.memory_stats()
    }
}

/// Python integration for TauraroLang standard library
pub struct PythonIntegration<'py> {
    interop: PythonInterop<'py>,
    imported_modules: HashMap<String, PyObject>,
}

impl<'py> PythonIntegration<'py> {
    pub fn new(py: Python<'py>) -> Self {
        let interop = PythonInterop::new(py);
        Self {
            interop,
            imported_modules: HashMap::new(),
        }
    }
    
    /// Import Python module and make it available in TauraroLang
    pub fn import_python_module(&mut self, module_name: &str, alias: Option<&str>) -> Result<()> {
        let module = self.interop.import_module(module_name)?;
        let name = alias.unwrap_or(module_name);
        self.imported_modules.insert(name.to_string(), module);
        Ok(())
    }
    
    /// Call Python function from TauraroLang VM
    pub fn call_python_function(&self, _vm: &mut VM, module_name: &str, function_name: &str, args: Vec<Value>) -> Result<Value> {
        if let Some(module) = self.imported_modules.get(module_name) {
            self.interop.call_function(module, function_name, args)
        } else {
            // Try to import the module on-demand
            let module = self.interop.import_module(module_name)?;
            let result = self.interop.call_function(&module, function_name, args)?;
            // Cache the module for future use
            // Note: We can't modify self here because we have an immutable reference
            Ok(result)
        }
    }
    
    /// Get list of imported modules
    pub fn get_imported_modules(&self) -> Vec<&str> {
        self.imported_modules.keys().map(|s| s.as_str()).collect()
    }
    
    /// Check if a module is imported
    pub fn is_module_imported(&self, module_name: &str) -> bool {
        self.imported_modules.contains_key(module_name)
    }
}

// Example of using Python integration in TauraroLang
pub fn demonstrate_python_integration() -> Result<()> {
    if !PythonInterop::is_available() {
        println!("Python is not available");
        return Ok(());
    }
    
    Python::with_gil(|py| {
        let mut integration = PythonIntegration::new(py);
        
        // Import Python math module
        integration.import_python_module("math", None)?;
        
        // Create TauraroLang VM
        let mut vm = VM::new();
        
        // Call Python math functions from TauraroLang
        let args = vec![Value::Float(3.14159)];
        let result = integration.call_python_function(&mut vm, "math", "sin", args)?;
        
        println!("Python math.sin(3.14159) = {}", result.to_string());
        
        Ok(())
    })
}
