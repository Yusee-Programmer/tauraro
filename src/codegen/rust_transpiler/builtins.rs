//! Built-in functions for Rust code generation

use super::RustCodegenContext;
use anyhow::Result;

impl RustCodegenContext {
    /// Generate print function
    pub fn gen_print(&self, args: Vec<&str>) -> String {
        if args.is_empty() {
            "println!()".to_string()
        } else {
            let format_str = args.iter().map(|_| "{}").collect::<Vec<_>>().join(" ");
            format!("println!(\"{}\"{})", format_str, 
                format!(", {}", args.join(", ")))
        }
    }

    /// Generate len function
    pub fn gen_len(&self, obj: &str) -> String {
        format!("{}.len()", obj)
    }

    /// Generate range function
    pub fn gen_range(&self, start: Option<&str>, end: &str, step: Option<&str>) -> String {
        match (start, step) {
            (Some(s), Some(st)) => format!("({}..{}).step_by({})", s, end, st),
            (Some(s), None) => format!("({}..{})", s, end),
            (None, Some(st)) => format!("(0..{}).step_by({})", end, st),
            (None, None) => format!("0..{}", end),
        }
    }

    /// Generate enumerate function
    pub fn gen_enumerate(&self, iterable: &str) -> String {
        format!("{}.iter().enumerate()", iterable)
    }

    /// Generate zip function
    pub fn gen_zip(&self, iterables: Vec<&str>) -> String {
        let iters = iterables.iter()
            .map(|it| format!("{}.iter()", it))
            .collect::<Vec<_>>()
            .join(", ");
        format!("izip!({})", iters)
    }

    /// Generate map function
    pub fn gen_map(&self, func: &str, iterable: &str) -> String {
        format!("{}.iter().map(|x| {}(x))", iterable, func)
    }

    /// Generate filter function
    pub fn gen_filter(&self, func: &str, iterable: &str) -> String {
        format!("{}.iter().filter(|x| {}(x))", iterable, func)
    }

    /// Generate sorted function
    pub fn gen_sorted(&self, obj: &str, reverse: bool) -> String {
        let obj_ref = format!("let mut sorted = {}.clone(); sorted.sort", obj);
        if reverse {
            format!("{}(); sorted.reverse(); sorted", obj_ref)
        } else {
            format!("{}(); sorted", obj_ref)
        }
    }

    /// Generate reversed function
    pub fn gen_reversed(&self, obj: &str) -> String {
        format!("{{ let mut rev = {}.clone(); rev.reverse(); rev }}", obj)
    }

    /// Generate str function
    pub fn gen_str(&self, obj: &str) -> String {
        format!("{}.to_string()", obj)
    }

    /// Generate int function
    pub fn gen_int(&self, obj: &str) -> String {
        format!("parse::<i64>({})", obj)
    }

    /// Generate float function
    pub fn gen_float(&self, obj: &str) -> String {
        format!("parse::<f64>({})", obj)
    }

    /// Generate bool function
    pub fn gen_bool(&self, obj: &str) -> String {
        format!("!{}.is_empty()", obj)
    }

    /// Generate list function
    pub fn gen_list(&self, obj: Option<&str>) -> String {
        match obj {
            Some(o) => format!("vec![{}]", o),
            None => "vec![]".to_string(),
        }
    }

    /// Generate dict function
    pub fn gen_dict(&self, items: Vec<(&str, &str)>) -> String {
        let pairs = items.iter()
            .map(|(k, v)| format!("(\"{}\".to_string(), {})", k, v))
            .collect::<Vec<_>>()
            .join(", ");
        format!("HashMap::from([{}])", pairs)
    }

    /// Generate set function
    pub fn gen_set(&self, items: Vec<&str>) -> String {
        let items_str = items.join(", ");
        format!("HashSet::from([{}])", items_str)
    }

    /// Generate tuple function
    pub fn gen_tuple(&self, items: Vec<&str>) -> String {
        format!("({})", items.join(", "))
    }

    /// Generate min function
    pub fn gen_min(&self, args: Vec<&str>) -> String {
        if args.len() == 1 {
            format!("*{}.iter().min().unwrap()", args[0])
        } else {
            format!("*[{}].iter().min().unwrap()", args.join(", "))
        }
    }

    /// Generate max function
    pub fn gen_max(&self, args: Vec<&str>) -> String {
        if args.len() == 1 {
            format!("*{}.iter().max().unwrap()", args[0])
        } else {
            format!("*[{}].iter().max().unwrap()", args.join(", "))
        }
    }

    /// Generate sum function
    pub fn gen_sum(&self, iterable: &str) -> String {
        format!("{}.iter().sum()", iterable)
    }

    /// Generate all function
    pub fn gen_all(&self, iterable: &str) -> String {
        format!("{}.iter().all(|x| x.is_truthy())", iterable)
    }

    /// Generate any function
    pub fn gen_any(&self, iterable: &str) -> String {
        format!("{}.iter().any(|x| x.is_truthy())", iterable)
    }

    /// Generate abs function
    pub fn gen_abs(&self, num: &str) -> String {
        format!("{}.abs()", num)
    }

    /// Generate round function
    pub fn gen_round(&self, num: &str, ndigits: Option<&str>) -> String {
        match ndigits {
            Some(n) => format!("({} * 10_f64.powi({}) as i32).round() / 10_f64.powi({}) as i32", num, n, n),
            None => format!("{}.round()", num),
        }
    }

    /// Generate pow function
    pub fn gen_pow(&self, base: &str, exp: &str) -> String {
        format!("{}.pow({} as u32)", base, exp)
    }

    /// Generate isinstance function
    pub fn gen_isinstance(&self, obj: &str, class: &str) -> String {
        format!("matches!({}, TauObject::Custom(\"{}\", _))", obj, class)
    }

    /// Generate type function
    pub fn gen_type(&self, obj: &str) -> String {
        format!("std::any::type_name_of_val(&{})", obj)
    }

    /// Generate callable check
    pub fn gen_callable(&self, obj: &str) -> String {
        format!("match {} {{ TauObject::Custom(_, _) => true, _ => false }}", obj)
    }

    /// Generate hasattr function
    pub fn gen_hasattr(&self, obj: &str, attr: &str) -> String {
        format!("{{ if let TauObject::Custom(_, fields) = {} {{ fields.lock().unwrap().contains_key(\"{}\") }} else {{ false }} }}", obj, attr)
    }

    /// Generate getattr function
    pub fn gen_getattr(&self, obj: &str, attr: &str, default: Option<&str>) -> String {
        let default_val = default.unwrap_or("TauObject::None");
        format!(
            "{{ if let TauObject::Custom(_, fields) = {} {{ fields.lock().unwrap().get(\"{}\").cloned().unwrap_or({}) }} else {{ {} }} }}",
            obj, attr, default_val, default_val
        )
    }

    /// Generate setattr function
    pub fn gen_setattr(&mut self, obj: &str, attr: &str, value: &str) -> Result<()> {
        self.emit(&format!(
            "if let TauObject::Custom(_, fields) = {} {{ fields.lock().unwrap().insert(\"{}\".to_string(), {}); }}",
            obj, attr, value
        ));
        Ok(())
    }

    /// Generate format string
    pub fn gen_format(&self, template: &str, args: Vec<&str>) -> String {
        format!("format!(\"{}\"{})", 
            template.replace("{}", "{}"),
            if args.is_empty() { "".to_string() } else { format!(", {}", args.join(", ")) }
        )
    }

    /// Generate input function
    pub fn gen_input(&self, prompt: Option<&str>) -> String {
        match prompt {
            Some(p) => format!("{{ print!(\"{}\"); std::io::stdout().flush().unwrap(); let mut s = String::new(); std::io::stdin().read_line(&mut s).unwrap(); s.trim().to_string() }}", p),
            None => "{{ let mut s = String::new(); std::io::stdin().read_line(&mut s).unwrap(); s.trim().to_string() }}".to_string(),
        }
    }

    /// Generate open function for file I/O
    pub fn gen_open(&self, path: &str, mode: Option<&str>) -> String {
        let file_mode = mode.unwrap_or("r");
        match file_mode {
            "r" => format!("std::fs::read_to_string({})", path),
            "w" => format!("std::fs::File::create({})", path),
            "a" => format!("std::fs::OpenOptions::new().append(true).open({})", path),
            _ => format!("std::fs::File::open({})", path),
        }
    }

    /// Generate import for modules
    pub fn gen_import(&mut self, module: &str) -> Result<()> {
        self.add_import(&format!("use crate::modules::{};", module));
        Ok(())
    }

    /// Generate from/import
    pub fn gen_from_import(&mut self, module: &str, items: Vec<&str>) -> Result<()> {
        let items_str = items.join(", ");
        self.add_import(&format!("use crate::modules::{}::{{{}}};", module, items_str));
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_gen_range() {
        let ctx = RustCodegenContext::new("test".to_string());
        assert_eq!(ctx.gen_range(None, "10", None), "0..10");
        assert_eq!(ctx.gen_range(Some("1"), "10", None), "(1..10)");
    }
}
