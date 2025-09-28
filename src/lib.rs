pub mod lexer;
pub mod parser;
pub mod ast;
pub mod semantic;
pub mod ir;
pub mod codegen;
pub mod value;
pub mod vm;
pub mod runtime;
pub mod object_system;
pub mod module_system;
pub mod modules;
pub mod builtins;
pub mod builtins_super;
pub mod base_object;
pub mod type_hierarchy;
pub mod metaclass;

use std::cell::RefCell;
use std::path::PathBuf;

#[cfg(feature = "ffi")]
pub mod ffi;

#[cfg(feature = "python-interop")]
pub mod python_interop;

pub use lexer::*;
pub use parser::*;
pub use ast::*;
pub use value::*;
pub use semantic::*;
pub use ir::*;
pub use vm::*;
pub use runtime::*;
pub use builtins::*;
pub use modules::*;
pub use module_system::*;
pub use object_system::*;
pub use metaclass::*;
#[cfg(feature = "ffi")]
pub use ffi::*;
