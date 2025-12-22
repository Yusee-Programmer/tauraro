//! Rust code generator for functions and methods

use anyhow::Result;
use super::RustCodegenContext;

impl RustCodegenContext {
    /// Generate a regular function
    pub fn gen_function_def(&mut self, name: &str, params: Vec<(&str, &str)>, return_type: &str, is_async: bool) -> Result<()> {
        let async_kw = if is_async { "async " } else { "" };
        let params_str = params.iter()
            .map(|(pname, ptype)| format!("{}: {}", pname, ptype))
            .collect::<Vec<_>>()
            .join(", ");
        
        self.emit(&format!("{}fn {}({}) -> {} {{", async_kw, name, params_str, return_type));
        self.indent();
        Ok(())
    }

    /// Close function definition
    pub fn close_function(&mut self) -> Result<()> {
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate a method definition
    pub fn gen_method_def(&mut self, struct_name: &str, method_name: &str, params: Vec<(&str, &str)>, return_type: &str, is_async: bool) -> Result<()> {
        let async_kw = if is_async { "async " } else { "" };
        let params_str = params.iter()
            .map(|(pname, ptype)| format!("{}: {}", pname, ptype))
            .collect::<Vec<_>>()
            .join(", ");
        
        self.emit(&format!("impl {} {{", struct_name));
        self.indent();
        self.emit(&format!("{}fn {}(self, {}) -> {} {{", async_kw, method_name, params_str, return_type));
        self.indent();
        Ok(())
    }

    /// Close method definition
    pub fn close_method(&mut self) -> Result<()> {
        self.dedent();
        self.emit("}");
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate function call
    pub fn gen_function_call(&self, func_name: &str, args: Vec<&str>) -> Result<String> {
        let args_str = args.join(", ");
        Ok(format!("{}({})", func_name, args_str))
    }

    /// Generate async function call
    pub fn gen_async_function_call(&self, func_name: &str, args: Vec<&str>) -> Result<String> {
        let args_str = args.join(", ");
        Ok(format!("{}.await({})", func_name, args_str))
    }

    /// Generate decorator
    pub fn gen_decorator(&mut self, decorator: &str) -> Result<()> {
        self.emit(&format!("#[{}]", decorator));
        Ok(())
    }

    /// Generate default parameter handling
    pub fn gen_default_param(&mut self, param: &str, default: &str) -> Result<()> {
        self.emit(&format!("let {} = {};", param, default));
        Ok(())
    }

    /// Generate varargs (*args)
    pub fn gen_varargs(&self, param_name: &str) -> String {
        format!("args: Vec<TauObject>")
    }

    /// Generate kwargs (**kwargs)
    pub fn gen_kwargs(&self, param_name: &str) -> String {
        format!("kwargs: HashMap<String, TauObject>")
    }

    /// Generate function with error handling
    pub fn gen_result_function(&mut self, name: &str, params: Vec<(&str, &str)>) -> Result<()> {
        let params_str = params.iter()
            .map(|(pname, ptype)| format!("{}: {}", pname, ptype))
            .collect::<Vec<_>>()
            .join(", ");
        
        self.emit(&format!("fn {}({}) -> Result<TauObject, String> {{", name, params_str));
        self.indent();
        Ok(())
    }

    /// Close result function
    pub fn close_result_function(&mut self) -> Result<()> {
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }
}
