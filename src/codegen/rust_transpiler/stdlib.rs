//! Standard library module generation for Rust

use super::RustCodegenContext;
use anyhow::Result;

/// Standard library support
pub struct StdlibSupport;

impl StdlibSupport {
    /// Generate math module support
    pub fn gen_math_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.add_external_crate("std");
        ctx.emit("// Math module");
        ctx.emit("pub mod math {");
        ctx.indent();
        ctx.emit("use std::f64::consts::PI;");
        ctx.emit("");
        ctx.emit("pub fn sin(x: f64) -> f64 { x.sin() }");
        ctx.emit("pub fn cos(x: f64) -> f64 { x.cos() }");
        ctx.emit("pub fn tan(x: f64) -> f64 { x.tan() }");
        ctx.emit("pub fn sqrt(x: f64) -> f64 { x.sqrt() }");
        ctx.emit("pub fn pow(x: f64, y: f64) -> f64 { x.powf(y) }");
        ctx.emit("pub fn abs(x: f64) -> f64 { x.abs() }");
        ctx.emit("pub fn floor(x: f64) -> f64 { x.floor() }");
        ctx.emit("pub fn ceil(x: f64) -> f64 { x.ceil() }");
        ctx.emit("pub fn round(x: f64) -> f64 { x.round() }");
        ctx.emit("pub fn pi() -> f64 { PI }");
        ctx.emit("pub fn e() -> f64 { std::f64::consts::E }");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }

    /// Generate string module support
    pub fn gen_string_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.emit("// String module");
        ctx.emit("pub mod string {");
        ctx.indent();
        ctx.emit("pub fn upper(s: &str) -> String { s.to_uppercase() }");
        ctx.emit("pub fn lower(s: &str) -> String { s.to_lowercase() }");
        ctx.emit("pub fn replace(s: &str, old: &str, new: &str) -> String { s.replace(old, new) }");
        ctx.emit("pub fn split(s: &str, sep: &str) -> Vec<&str> { s.split(sep).collect() }");
        ctx.emit("pub fn strip(s: &str) -> String { s.trim().to_string() }");
        ctx.emit("pub fn startswith(s: &str, prefix: &str) -> bool { s.starts_with(prefix) }");
        ctx.emit("pub fn endswith(s: &str, suffix: &str) -> bool { s.ends_with(suffix) }");
        ctx.emit("pub fn contains(s: &str, substr: &str) -> bool { s.contains(substr) }");
        ctx.emit("pub fn find(s: &str, substr: &str) -> Option<usize> { s.find(substr) }");
        ctx.emit("pub fn index_of(s: &str, substr: &str) -> usize { s.find(substr).unwrap_or(0) }");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }

    /// Generate collections module support
    pub fn gen_collections_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.emit("// Collections module");
        ctx.emit("pub mod collections {");
        ctx.indent();
        ctx.emit("use std::collections::{HashMap, HashSet, VecDeque};");
        ctx.emit("");
        ctx.emit("pub fn list_extend<T: Clone>(v: &mut Vec<T>, other: &[T]) {");
        ctx.indent();
        ctx.emit("v.extend_from_slice(other);");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        ctx.emit("pub fn list_append<T>(v: &mut Vec<T>, item: T) {");
        ctx.indent();
        ctx.emit("v.push(item);");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        ctx.emit("pub fn list_remove<T: PartialEq>(v: &mut Vec<T>, item: &T) {");
        ctx.indent();
        ctx.emit("if let Some(pos) = v.iter().position(|x| x == item) {");
        ctx.indent();
        ctx.emit("v.remove(pos);");
        ctx.dedent();
        ctx.emit("}");
        ctx.dedent();
        ctx.emit("}");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }

    /// Generate io module support
    pub fn gen_io_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.emit("// IO module");
        ctx.emit("pub mod io {");
        ctx.indent();
        ctx.emit("use std::fs::File;");
        ctx.emit("use std::io::{Read, Write, BufRead, BufReader};");
        ctx.emit("");
        ctx.emit("pub fn read_file(path: &str) -> std::io::Result<String> {");
        ctx.indent();
        ctx.emit("std::fs::read_to_string(path)");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        ctx.emit("pub fn write_file(path: &str, content: &str) -> std::io::Result<()> {");
        ctx.indent();
        ctx.emit("std::fs::write(path, content)");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        ctx.emit("pub fn append_file(path: &str, content: &str) -> std::io::Result<()> {");
        ctx.indent();
        ctx.emit("let mut file = File::options().append(true).open(path)?;");
        ctx.emit("file.write_all(content.as_bytes())?;");
        ctx.emit("Ok(())");
        ctx.dedent();
        ctx.emit("}");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }

    /// Generate sys module support
    pub fn gen_sys_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.emit("// Sys module");
        ctx.emit("pub mod sys {");
        ctx.indent();
        ctx.emit("pub fn argv() -> Vec<String> { std::env::args().collect() }");
        ctx.emit("pub fn exit(code: i32) -> ! { std::process::exit(code) }");
        ctx.emit("pub fn getenv(key: &str) -> Option<String> { std::env::var(key).ok() }");
        ctx.emit("pub fn setenv(key: &str, value: &str) { std::env::set_var(key, value); }");
        ctx.emit("pub fn platform() -> &'static str { std::env::consts::OS }");
        ctx.emit("pub fn version() -> &'static str { \"1.0.0\" }");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }

    /// Generate time module support
    pub fn gen_time_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.emit("// Time module");
        ctx.emit("pub mod time {");
        ctx.indent();
        ctx.emit("use std::time::{SystemTime, UNIX_EPOCH, Duration};");
        ctx.emit("use std::thread;");
        ctx.emit("");
        ctx.emit("pub fn time() -> u64 {");
        ctx.indent();
        ctx.emit("SystemTime::now()");
        ctx.indent();
        ctx.emit(".duration_since(UNIX_EPOCH)");
        ctx.dedent();
        ctx.emit(".unwrap_or_default()");
        ctx.indent();
        ctx.emit(".as_secs()");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        ctx.emit("pub fn sleep(seconds: f64) {");
        ctx.indent();
        ctx.emit("thread::sleep(Duration::from_secs_f64(seconds));");
        ctx.dedent();
        ctx.emit("}");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }

    /// Generate json module support
    pub fn gen_json_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.add_external_crate("serde");
        ctx.emit("// JSON module");
        ctx.emit("pub mod json {");
        ctx.indent();
        ctx.emit("use serde_json::{json, Value, to_string, from_str};");
        ctx.emit("");
        ctx.emit("pub fn dumps(value: &Value) -> String {");
        ctx.indent();
        ctx.emit("value.to_string()");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        ctx.emit("pub fn loads(s: &str) -> Result<Value, Box<dyn std::error::Error>> {");
        ctx.indent();
        ctx.emit("Ok(from_str(s)?)");
        ctx.dedent();
        ctx.emit("}");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }

    /// Generate random module support
    pub fn gen_random_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.add_external_crate("rand");
        ctx.emit("// Random module");
        ctx.emit("pub mod random {");
        ctx.indent();
        ctx.emit("use rand::Rng;");
        ctx.emit("");
        ctx.emit("pub fn random() -> f64 {");
        ctx.indent();
        ctx.emit("let mut rng = rand::thread_rng();");
        ctx.emit("rng.gen::<f64>()");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        ctx.emit("pub fn randint(a: i64, b: i64) -> i64 {");
        ctx.indent();
        ctx.emit("let mut rng = rand::thread_rng();");
        ctx.emit("rng.gen_range(a..=b)");
        ctx.dedent();
        ctx.emit("}");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }

    /// Generate regex module support
    pub fn gen_regex_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.add_external_crate("regex");
        ctx.emit("// Regex module");
        ctx.emit("pub mod regex {");
        ctx.indent();
        ctx.emit("use regex::Regex;");
        ctx.emit("");
        ctx.emit("pub fn match_pattern(pattern: &str, text: &str) -> bool {");
        ctx.indent();
        ctx.emit("Regex::new(pattern).map(|re| re.is_match(text)).unwrap_or(false)");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        ctx.emit("pub fn find_all(pattern: &str, text: &str) -> Vec<String> {");
        ctx.indent();
        ctx.emit("Regex::new(pattern)");
        ctx.indent();
        ctx.emit(".map(|re| re.find_iter(text).map(|m| m.as_str().to_string()).collect())");
        ctx.dedent();
        ctx.emit(".unwrap_or_default()");
        ctx.dedent();
        ctx.emit("}");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }

    /// Generate path module support
    pub fn gen_path_module(ctx: &mut RustCodegenContext) -> Result<()> {
        ctx.emit("// Path module");
        ctx.emit("pub mod path {");
        ctx.indent();
        ctx.emit("use std::path::{Path, PathBuf};");
        ctx.emit("");
        ctx.emit("pub fn join(paths: &[&str]) -> PathBuf {");
        ctx.indent();
        ctx.emit("let mut p = PathBuf::new();");
        ctx.emit("for path in paths { p.push(path); }");
        ctx.emit("p");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        ctx.emit("pub fn exists(path: &str) -> bool {");
        ctx.indent();
        ctx.emit("Path::new(path).exists()");
        ctx.dedent();
        ctx.emit("}");
        ctx.dedent();
        ctx.emit("}");
        ctx.emit("");
        Ok(())
    }
}

impl RustCodegenContext {
    /// Generate all stdlib modules
    pub fn gen_all_stdlib_modules(&mut self) -> Result<()> {
        self.emit("// === Standard Library Modules ===");
        self.emit("");
        
        StdlibSupport::gen_math_module(self)?;
        StdlibSupport::gen_string_module(self)?;
        StdlibSupport::gen_collections_module(self)?;
        StdlibSupport::gen_io_module(self)?;
        StdlibSupport::gen_sys_module(self)?;
        StdlibSupport::gen_time_module(self)?;
        StdlibSupport::gen_json_module(self)?;
        StdlibSupport::gen_random_module(self)?;
        StdlibSupport::gen_regex_module(self)?;
        StdlibSupport::gen_path_module(self)?;
        
        Ok(())
    }
}
