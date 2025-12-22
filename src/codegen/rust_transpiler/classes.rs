//! Rust code generator for classes and OOP

use anyhow::Result;
use super::RustCodegenContext;

impl RustCodegenContext {
    /// Generate a struct definition
    pub fn gen_struct(&mut self, name: &str, fields: Vec<(&str, &str)>) -> Result<()> {
        self.emit("#[derive(Clone, Debug)]");
        self.emit(&format!("pub struct {} {{", name));
        self.indent();
        for (field_name, field_type) in fields {
            self.emit(&format!("pub {}: {},", field_name, field_type));
        }
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate a trait definition
    pub fn gen_trait(&mut self, name: &str, methods: Vec<(&str, &str)>) -> Result<()> {
        self.emit(&format!("pub trait {} {{", name));
        self.indent();
        for (method_name, signature) in methods {
            self.emit(&format!("fn {}{}; ", method_name, signature));
        }
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate a class/struct with constructor
    pub fn gen_class(&mut self, name: &str, fields: Vec<(&str, &str)>) -> Result<()> {
        self.emit("#[derive(Clone, Debug)]");
        self.emit(&format!("pub struct {} {{", name));
        self.indent();
        for (field_name, field_type) in &fields {
            self.emit(&format!("pub {}: {},", field_name, field_type));
        }
        self.dedent();
        self.emit("}");
        self.emit("");

        // Generate impl block
        self.emit(&format!("impl {} {{", name));
        self.indent();

        // Generate new() constructor
        let params = fields.iter()
            .map(|(fname, ftype)| format!("{}: {}", fname, ftype))
            .collect::<Vec<_>>()
            .join(", ");
        
        self.emit(&format!("pub fn new({}) -> Self {{", params));
        self.indent();
        self.emit(&format!("{} {{", name));
        self.indent();
        for (fname, _) in &fields {
            self.emit(&format!("{},", fname));
        }
        self.dedent();
        self.emit("}");
        self.dedent();
        self.emit("}");

        Ok(())
    }

    /// Close class impl block
    pub fn close_class_impl(&mut self) -> Result<()> {
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate enum definition
    pub fn gen_enum(&mut self, name: &str, variants: Vec<&str>) -> Result<()> {
        self.emit(&format!("pub enum {} {{", name));
        self.indent();
        for variant in variants {
            self.emit(&format!("{},", variant));
        }
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate a static method
    pub fn gen_static_method(&mut self, struct_name: &str, method_name: &str, params: Vec<(&str, &str)>, return_type: &str) -> Result<()> {
        let params_str = params.iter()
            .map(|(pname, ptype)| format!("{}: {}", pname, ptype))
            .collect::<Vec<_>>()
            .join(", ");
        
        self.emit(&format!("impl {} {{", struct_name));
        self.indent();
        self.emit(&format!("pub fn {}({}) -> {} {{", method_name, params_str, return_type));
        self.indent();
        Ok(())
    }

    /// Generate property getter
    pub fn gen_property_getter(&mut self, struct_name: &str, prop_name: &str, return_type: &str) -> Result<()> {
        self.emit(&format!("impl {} {{", struct_name));
        self.indent();
        self.emit(&format!("pub fn get_{}(&self) -> {} {{", prop_name, return_type));
        self.indent();
        self.emit(&format!("self.{}.clone()", prop_name));
        self.dedent();
        self.emit("}");
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate property setter
    pub fn gen_property_setter(&mut self, struct_name: &str, prop_name: &str, prop_type: &str) -> Result<()> {
        self.emit(&format!("impl {} {{", struct_name));
        self.indent();
        self.emit(&format!("pub fn set_{}(&mut self, value: {}) {{", prop_name, prop_type));
        self.indent();
        self.emit(&format!("self.{} = value;", prop_name));
        self.dedent();
        self.emit("}");
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate inheritance/trait implementation
    pub fn gen_impl_trait(&mut self, struct_name: &str, trait_name: &str) -> Result<()> {
        self.emit(&format!("impl {} for {} {{", trait_name, struct_name));
        self.indent();
        Ok(())
    }

    /// Close trait implementation
    pub fn close_impl_trait(&mut self) -> Result<()> {
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate abstract base class pattern
    pub fn gen_abc_pattern(&mut self, name: &str, abstract_methods: Vec<&str>) -> Result<()> {
        // In Rust, use traits for abstract classes
        self.emit(&format!("pub trait {} {{", name));
        self.indent();
        for method in abstract_methods {
            self.emit(&format!("fn {}(&self);", method));
        }
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate dataclass
    pub fn gen_dataclass(&mut self, name: &str, fields: Vec<(&str, &str)>) -> Result<()> {
        self.emit("#[derive(Clone, Debug, PartialEq)]");
        self.emit(&format!("pub struct {} {{", name));
        self.indent();
        for (field_name, field_type) in fields {
            self.emit(&format!("pub {}: {},", field_name, field_type));
        }
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }
}
