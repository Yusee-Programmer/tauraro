//! Rust code generator for statements and control flow

use anyhow::Result;
use super::RustCodegenContext;

impl RustCodegenContext {
    /// Generate an if statement
    pub fn gen_if_statement(&mut self, condition: &str, then_body: Vec<&str>, else_body: Option<Vec<&str>>) -> Result<()> {
        self.emit(&format!("if {} {{", condition));
        self.indent();
        for stmt in then_body {
            self.emit(stmt);
        }
        self.dedent();
        
        if let Some(else_stmts) = else_body {
            self.emit("} else {");
            self.indent();
            for stmt in else_stmts {
                self.emit(stmt);
            }
            self.dedent();
        }
        
        self.emit("}");
        Ok(())
    }

    /// Generate a for loop
    pub fn gen_for_loop(&mut self, var: &str, iter: &str, body: Vec<&str>) -> Result<()> {
        self.emit(&format!("for {} in {} {{", var, iter));
        self.indent();
        for stmt in body {
            self.emit(stmt);
        }
        self.dedent();
        self.emit("}");
        Ok(())
    }

    /// Generate a while loop
    pub fn gen_while_loop(&mut self, condition: &str, body: Vec<&str>) -> Result<()> {
        self.emit(&format!("while {} {{", condition));
        self.indent();
        for stmt in body {
            self.emit(stmt);
        }
        self.dedent();
        self.emit("}");
        Ok(())
    }

    /// Generate a match statement
    pub fn gen_match(&mut self, expr: &str, arms: Vec<(&str, &str)>) -> Result<()> {
        self.emit(&format!("match {} {{", expr));
        self.indent();
        for (pattern, body) in arms {
            self.emit(&format!("{} => {}", pattern, body));
        }
        self.dedent();
        self.emit("}");
        Ok(())
    }

    /// Generate a variable assignment
    pub fn gen_assignment(&mut self, var: &str, value: &str) -> Result<()> {
        self.emit(&format!("let {} = {};", var, value));
        Ok(())
    }

    /// Generate a mutable variable assignment
    pub fn gen_mut_assignment(&mut self, var: &str, value: &str) -> Result<()> {
        self.emit(&format!("let mut {} = {};", var, value));
        Ok(())
    }

    /// Generate a try-catch block
    pub fn gen_try_catch(&mut self, try_body: Vec<&str>, catch_var: &str, catch_body: Vec<&str>) -> Result<()> {
        self.emit("match (|| {");
        self.indent();
        for stmt in try_body {
            self.emit(stmt);
        }
        self.dedent();
        self.emit("})() {");
        self.emit(&format!("    Err({}) => {{", catch_var));
        self.indent();
        for stmt in catch_body {
            self.emit(stmt);
        }
        self.dedent();
        self.emit("    }");
        self.emit("    Ok(val) => val,");
        self.emit("}");
        Ok(())
    }

    /// Generate a return statement
    pub fn gen_return(&mut self, value: Option<&str>) -> Result<()> {
        match value {
            Some(val) => self.emit(&format!("return {};", val)),
            None => self.emit("return TauObject::None;"),
        }
        Ok(())
    }

    /// Generate a break statement
    pub fn gen_break(&mut self) -> Result<()> {
        self.emit("break;");
        Ok(())
    }

    /// Generate a continue statement
    pub fn gen_continue(&mut self) -> Result<()> {
        self.emit("continue;");
        Ok(())
    }

    /// Generate a pass statement
    pub fn gen_pass(&mut self) -> Result<()> {
        self.emit("// pass");
        Ok(())
    }
}
