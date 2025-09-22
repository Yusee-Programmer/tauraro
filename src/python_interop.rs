//! COMPLETE Python interoperability for TauraroLang
use pyo3::prelude::*;
use pyo3::types::{PyDict, PyList, PyTuple};
use anyhow::Result;
use std::collections::HashMap;

/// Python interop manager
pub struct PythonInterop {
    py: Python,
}

impl PythonInterop {
    pub fn new() -> Result<Self> {
        Python::with_gil(|py| {
            Ok(Self { py })
        })
    }
    
    /// Check if Python is available
    pub fn is_available() -> bool {
        Python::with_gil(|_py| true).is_ok()
    }
    
    /// Import Python module into TauraroLang
    pub fn import_module(&self, module_name: &str) -> Result<PyObject> {
        let module = self.py.import(module_name)?;
        Ok(module.to_object(self.py))
    }
    
    /// Call Python function from TauraroLang
    pub fn call_function(&self, py_obj: &PyObject, function_name: &str, args: Vec<crate::vm::Value>) -> Result<crate::vm::Value> {
        let args_py = self.tauraro_values_to_python(args)?;
        let result = py_obj.call_method(self.py, function_name, args_py, None)?;
        self.python_to_tauraro_value(result)
    }
    
    /// Evaluate Python code from TauraroLang
    pub fn eval_python_code(&self, code: &str) -> Result<crate::vm::Value> {
        let result = self.py.eval(code, None, None)?;
        self.python_to_tauraro_value(result)
    }
    
    /// Execute Python script
    pub fn exec_python_script(&self, code: &str) -> Result<()> {
        self.py.run(code, None, None)?;
        Ok(())
    }
    
    /// Convert Tauraro values to Python objects
    fn tauraro_values_to_python(&self, values: Vec<crate::vm::Value>) -> Result<Py<PyTuple>> {
        let py_values: Result<Vec<PyObject>> = values
            .into_iter()
            .map(|v| self.tauraro_value_to_python(v))
            .collect();
        
        Ok(PyTuple::new(self.py, &py_values?).into())
    }
    
    /// Convert single Tauraro value to Python object
    fn tauraro_value_to_python(&self, value: crate::vm::Value) -> Result<PyObject> {
        match value {
            crate::vm::Value::Int(n) => Ok(n.to_object(self.py)),
            crate::vm::Value::Float(n) => Ok(n.to_object(self.py)),
            crate::vm::Value::Bool(b) => Ok(b.to_object(self.py)),
            crate::vm::Value::String(s) => Ok(s.to_object(self.py)),
            crate::vm::Value::List(items) => {
                let py_list = PyList::empty(self.py);
                for item in items {
                    let py_item = self.tauraro_value_to_python(item)?;
                    py_list.append(py_item)?;
                }
                Ok(py_list.to_object(self.py))
            }
            crate::vm::Value::Dict(dict) => {
                let py_dict = PyDict::new(self.py);
                for (k, v) in dict {
                    let py_key = self.tauraro_value_to_python(crate::vm::Value::String(k))?;
                    let py_value = self.tauraro_value_to_python(v)?;
                    py_dict.set_item(py_key, py_value)?;
                }
                Ok(py_dict.to_object(self.py))
            }
            crate::vm::Value::None => Ok(self.py.None()),
            _ => Err(anyhow::anyhow!("Unsupported value type for Python conversion")),
        }
    }
    
    /// Convert Python object to Tauraro value
    fn python_to_tauraro_value(&self, obj: PyObject) -> Result<crate::vm::Value> {
        let py_obj = obj.as_ref(self.py);
        
        if py_obj.is_none() {
            return Ok(crate::vm::Value::None);
        }
        
        if let Ok(int_val) = py_obj.extract::<i64>() {
            return Ok(crate::vm::Value::Int(int_val));
        }
        
        if let Ok(float_val) = py_obj.extract::<f64>() {
            return Ok(crate::vm::Value::Float(float_val));
        }
        
        if let Ok(bool_val) = py_obj.extract::<bool>() {
            return Ok(crate::vm::Value::Bool(bool_val));
        }
        
        if let Ok(string_val) = py_obj.extract::<String>() {
            return Ok(crate::vm::Value::String(string_val));
        }
        
        if let Ok(list_val) = py_obj.downcast::<PyList>() {
            let mut items = Vec::new();
            for item in list_val.iter() {
                items.push(self.python_to_tauraro_value(item.to_object(self.py))?);
            }
            return Ok(crate::vm::Value::List(items));
        }
        
        if let Ok(dict_val) = py_obj.downcast::<PyDict>() {
            let mut dict = HashMap::new();
            for (key, value) in dict_val.iter() {
                let key_str = key.extract::<String>()?;
                let value_val = self.python_to_tauraro_value(value.to_object(self.py))?;
                dict.insert(key_str, value_val);
            }
            return Ok(crate::vm::Value::Dict(dict));
        }
        
        // Fallback: return as string representation
        Ok(crate::vm::Value::String(py_obj.repr()?.extract()?))
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
    match vm.execute_repl(code, 1) {
        Ok(Some(result)) => Ok(result.to_string()),
        Ok(None) => Ok("None".to_string()),
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
        match self.vm.execute_repl(code, 1) {
            Ok(Some(result)) => Ok(result.to_string()),
            Ok(None) => Ok("None".to_string()),
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
pub struct PythonIntegration {
    interop: PythonInterop,
    imported_modules: HashMap<String, PyObject>,
}

impl PythonIntegration {
    pub fn new() -> Result<Self> {
        let interop = PythonInterop::new()?;
        Ok(Self {
            interop,
            imported_modules: HashMap::new(),
        })
    }
    
    /// Import Python module and make it available in TauraroLang
    pub fn import_python_module(&mut self, module_name: &str, alias: Option<&str>) -> Result<()> {
        let module = self.interop.import_module(module_name)?;
        let name = alias.unwrap_or(module_name);
        self.imported_modules.insert(name.to_string(), module);
        Ok(())
    }
    
    /// Call Python function from TauraroLang VM
    pub fn call_python_function(&self, vm: &mut crate::vm::VM, module_name: &str, function_name: &str, args: Vec<crate::vm::Value>) -> Result<crate::vm::Value> {
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
    
    let mut integration = PythonIntegration::new()?;
    
    // Import Python math module
    integration.import_python_module("math", None)?;
    
    // Create TauraroLang VM
    let mut vm = crate::vm::VM::new();
    
    // Call Python math functions from TauraroLang
    let args = vec![crate::vm::Value::Float(3.14159)];
    let result = integration.call_python_function(&mut vm, "math", "sin", args)?;
    
    println!("Python math.sin(3.14159) = {}", result.to_string());
    
    Ok(())
}