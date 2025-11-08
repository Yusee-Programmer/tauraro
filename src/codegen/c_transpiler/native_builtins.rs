//! Native Built-in Functions
//!
//! Comprehensive implementation of Python built-in functions as native C code

use std::collections::HashMap;

/// Built-in function definitions and implementations
pub struct NativeBuiltins {
    /// Maps built-in function names to their C implementations
    implementations: HashMap<String, String>,
}

impl NativeBuiltins {
    pub fn new() -> Self {
        let mut implementations = HashMap::new();

        // Core type conversions
        implementations.insert("int".to_string(), Self::impl_int());
        implementations.insert("float".to_string(), Self::impl_float());
        implementations.insert("str".to_string(), Self::impl_str());
        implementations.insert("bool".to_string(), Self::impl_bool());

        // Container operations
        implementations.insert("len".to_string(), Self::impl_len());
        implementations.insert("min".to_string(), Self::impl_min());
        implementations.insert("max".to_string(), Self::impl_max());
        implementations.insert("sum".to_string(), Self::impl_sum());
        implementations.insert("sorted".to_string(), Self::impl_sorted());
        implementations.insert("reversed".to_string(), Self::impl_reversed());

        // Type checking
        implementations.insert("isinstance".to_string(), Self::impl_isinstance());
        implementations.insert("type".to_string(), Self::impl_type());

        // I/O
        implementations.insert("input".to_string(), Self::impl_input());

        // Numeric
        implementations.insert("abs".to_string(), Self::impl_abs());
        implementations.insert("pow".to_string(), Self::impl_pow());
        implementations.insert("round".to_string(), Self::impl_round());

        // Iterator/range
        implementations.insert("enumerate".to_string(), Self::impl_enumerate());
        implementations.insert("zip".to_string(), Self::impl_zip());

        Self { implementations }
    }

    /// Get C implementation for all built-in functions
    pub fn generate_all_implementations() -> String {
        let mut code = String::new();

        code.push_str("// Native built-in function implementations\n\n");

        code.push_str(&Self::impl_int());
        code.push_str(&Self::impl_float());
        code.push_str(&Self::impl_str());
        code.push_str(&Self::impl_bool());
        code.push_str(&Self::impl_len());
        code.push_str(&Self::impl_min());
        code.push_str(&Self::impl_max());
        code.push_str(&Self::impl_sum());
        code.push_str(&Self::impl_abs());
        code.push_str(&Self::impl_pow());
        code.push_str(&Self::impl_round());
        code.push_str(&Self::impl_input());
        code.push_str(&Self::impl_isinstance());

        code
    }

    fn impl_int() -> String {
        r#"
// int() - Convert to integer
int64_t tauraro_int_from_float(double x) {
    return (int64_t)x;
}

int64_t tauraro_int_from_str(const char* s) {
    return (int64_t)atoll(s);
}

int64_t tauraro_int_from_bool(bool b) {
    return b ? 1 : 0;
}
"#.to_string()
    }

    fn impl_float() -> String {
        r#"
// float() - Convert to float
double tauraro_float_from_int(int64_t x) {
    return (double)x;
}

double tauraro_float_from_str(const char* s) {
    return atof(s);
}

double tauraro_float_from_bool(bool b) {
    return b ? 1.0 : 0.0;
}
"#.to_string()
    }

    fn impl_str() -> String {
        r#"
// str() - Convert to string
char* tauraro_str_from_int(int64_t x) {
    char* buf = malloc(32);
    snprintf(buf, 32, "%lld", x);
    return buf;
}

char* tauraro_str_from_float(double x) {
    char* buf = malloc(32);
    snprintf(buf, 32, "%g", x);
    return buf;
}

char* tauraro_str_from_bool(bool b) {
    return strdup(b ? "True" : "False");
}

char* tauraro_str_copy(const char* s) {
    return strdup(s);
}
"#.to_string()
    }

    fn impl_bool() -> String {
        r#"
// bool() - Convert to boolean
bool tauraro_bool_from_int(int64_t x) {
    return x != 0;
}

bool tauraro_bool_from_float(double x) {
    return x != 0.0;
}

bool tauraro_bool_from_str(const char* s) {
    return s != NULL && s[0] != '\0';
}
"#.to_string()
    }

    fn impl_len() -> String {
        r#"
// len() - Get length of string or container
int64_t tauraro_len_str(const char* s) {
    return s ? (int64_t)strlen(s) : 0;
}

int64_t tauraro_len_list(tauraro_native_list_t* list) {
    return list ? (int64_t)list->size : 0;
}

int64_t tauraro_len_dict(tauraro_native_dict_t* dict) {
    return dict ? (int64_t)dict->size : 0;
}
"#.to_string()
    }

    fn impl_min() -> String {
        r#"
// min() - Find minimum value
int64_t tauraro_min_int(int64_t a, int64_t b) {
    return a < b ? a : b;
}

double tauraro_min_float(double a, double b) {
    return a < b ? a : b;
}

int64_t tauraro_min_int_array(int64_t* arr, size_t len) {
    if (len == 0) return 0;
    int64_t min_val = arr[0];
    for (size_t i = 1; i < len; i++) {
        if (arr[i] < min_val) min_val = arr[i];
    }
    return min_val;
}
"#.to_string()
    }

    fn impl_max() -> String {
        r#"
// max() - Find maximum value
int64_t tauraro_max_int(int64_t a, int64_t b) {
    return a > b ? a : b;
}

double tauraro_max_float(double a, double b) {
    return a > b ? a : b;
}

int64_t tauraro_max_int_array(int64_t* arr, size_t len) {
    if (len == 0) return 0;
    int64_t max_val = arr[0];
    for (size_t i = 1; i < len; i++) {
        if (arr[i] > max_val) max_val = arr[i];
    }
    return max_val;
}
"#.to_string()
    }

    fn impl_sum() -> String {
        r#"
// sum() - Sum elements
int64_t tauraro_sum_int_array(int64_t* arr, size_t len) {
    int64_t total = 0;
    for (size_t i = 0; i < len; i++) {
        total += arr[i];
    }
    return total;
}

double tauraro_sum_float_array(double* arr, size_t len) {
    double total = 0.0;
    for (size_t i = 0; i < len; i++) {
        total += arr[i];
    }
    return total;
}
"#.to_string()
    }

    fn impl_sorted() -> String {
        r#"
// sorted() - Return sorted copy
static int compare_int64(const void* a, const void* b) {
    int64_t arg1 = *(const int64_t*)a;
    int64_t arg2 = *(const int64_t*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int64_t* tauraro_sorted_int_array(int64_t* arr, size_t len) {
    int64_t* sorted = malloc(len * sizeof(int64_t));
    memcpy(sorted, arr, len * sizeof(int64_t));
    qsort(sorted, len, sizeof(int64_t), compare_int64);
    return sorted;
}
"#.to_string()
    }

    fn impl_reversed() -> String {
        r#"
// reversed() - Return reversed copy
int64_t* tauraro_reversed_int_array(int64_t* arr, size_t len) {
    int64_t* reversed = malloc(len * sizeof(int64_t));
    for (size_t i = 0; i < len; i++) {
        reversed[i] = arr[len - 1 - i];
    }
    return reversed;
}
"#.to_string()
    }

    fn impl_abs() -> String {
        r#"
// abs() - Absolute value
int64_t tauraro_abs_int(int64_t x) {
    return x < 0 ? -x : x;
}

double tauraro_abs_float(double x) {
    return fabs(x);
}
"#.to_string()
    }

    fn impl_pow() -> String {
        r#"
// pow() - Power operation
double tauraro_pow_native(double base, double exp) {
    return pow(base, exp);
}

int64_t tauraro_pow_int(int64_t base, int64_t exp) {
    if (exp == 0) return 1;
    if (exp < 0) return 0; // Integer division

    int64_t result = 1;
    int64_t b = base;
    int64_t e = exp;

    while (e > 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}
"#.to_string()
    }

    fn impl_round() -> String {
        r#"
// round() - Round to nearest integer
int64_t tauraro_round_to_int(double x) {
    return (int64_t)round(x);
}

double tauraro_round_to_places(double x, int places) {
    double multiplier = pow(10.0, places);
    return round(x * multiplier) / multiplier;
}
"#.to_string()
    }

    fn impl_input() -> String {
        r#"
// input() - Read line from stdin
char* tauraro_input(const char* prompt) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read = getline(&line, &len, stdin);

    if (read != -1) {
        // Remove trailing newline
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        return line;
    }

    return strdup("");
}
"#.to_string()
    }

    fn impl_isinstance() -> String {
        r#"
// isinstance() - Type checking
bool tauraro_isinstance_int(int64_t x) {
    return true; // If it's an int64_t, it's an int
}

bool tauraro_isinstance_float(double x) {
    return true;
}

bool tauraro_isinstance_str(const char* x) {
    return x != NULL;
}
"#.to_string()
    }

    fn impl_type() -> String {
        r#"
// type() - Get type name
const char* tauraro_type_int() {
    return "int";
}

const char* tauraro_type_float() {
    return "float";
}

const char* tauraro_type_str() {
    return "str";
}

const char* tauraro_type_bool() {
    return "bool";
}
"#.to_string()
    }

    fn impl_enumerate() -> String {
        r#"
// enumerate() - Create enumerated iterator
typedef struct {
    size_t index;
    void* value;
} tauraro_enumerate_item_t;
"#.to_string()
    }

    fn impl_zip() -> String {
        r#"
// zip() - Zip multiple iterators
typedef struct {
    void** items;
    size_t count;
} tauraro_zip_item_t;
"#.to_string()
    }

    /// Check if a function is a built-in
    pub fn is_builtin(&self, name: &str) -> bool {
        self.implementations.contains_key(name)
    }

    /// Get implementation for a specific built-in
    pub fn get_implementation(&self, name: &str) -> Option<&String> {
        self.implementations.get(name)
    }
}

/// Generate optimized call for a built-in function
pub fn generate_builtin_call(func_name: &str, args: &[String], arg_types: &[crate::codegen::c_transpiler::native_types::NativeType]) -> Option<String> {
    use crate::codegen::c_transpiler::native_types::NativeType;

    match func_name {
        "len" => {
            if args.len() == 1 {
                match &arg_types[0] {
                    NativeType::String => Some(format!("tauraro_len_str({})", args[0])),
                    NativeType::List(_) => Some(format!("tauraro_len_list({})", args[0])),
                    NativeType::Dict(_, _) => Some(format!("tauraro_len_dict({})", args[0])),
                    _ => Some(format!("strlen({})", args[0])), // Default to string
                }
            } else {
                None
            }
        }
        "abs" => {
            if args.len() == 1 {
                match &arg_types[0] {
                    NativeType::Int => Some(format!("tauraro_abs_int({})", args[0])),
                    NativeType::Float => Some(format!("tauraro_abs_float({})", args[0])),
                    _ => None,
                }
            } else {
                None
            }
        }
        "pow" => {
            if args.len() == 2 {
                match (&arg_types[0], &arg_types[1]) {
                    (NativeType::Int, NativeType::Int) => {
                        Some(format!("tauraro_pow_int({}, {})", args[0], args[1]))
                    }
                    _ => Some(format!("tauraro_pow_native({}, {})", args[0], args[1])),
                }
            } else {
                None
            }
        }
        "min" => {
            if args.len() == 2 {
                match &arg_types[0] {
                    NativeType::Int => Some(format!("tauraro_min_int({}, {})", args[0], args[1])),
                    NativeType::Float => Some(format!("tauraro_min_float({}, {})", args[0], args[1])),
                    _ => None,
                }
            } else {
                None
            }
        }
        "max" => {
            if args.len() == 2 {
                match &arg_types[0] {
                    NativeType::Int => Some(format!("tauraro_max_int({}, {})", args[0], args[1])),
                    NativeType::Float => Some(format!("tauraro_max_float({}, {})", args[0], args[1])),
                    _ => None,
                }
            } else {
                None
            }
        }
        "round" => {
            if args.len() == 1 {
                Some(format!("tauraro_round_to_int({})", args[0]))
            } else if args.len() == 2 {
                Some(format!("tauraro_round_to_places({}, {})", args[0], args[1]))
            } else {
                None
            }
        }
        "input" => {
            if args.is_empty() {
                Some("tauraro_input(NULL)".to_string())
            } else if args.len() == 1 {
                Some(format!("tauraro_input({})", args[0]))
            } else {
                None
            }
        }
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_builtins_creation() {
        let builtins = NativeBuiltins::new();
        assert!(builtins.is_builtin("len"));
        assert!(builtins.is_builtin("int"));
        assert!(!builtins.is_builtin("nonexistent"));
    }

    #[test]
    fn test_builtin_implementations() {
        let code = NativeBuiltins::generate_all_implementations();
        assert!(code.contains("tauraro_len_str"));
        assert!(code.contains("tauraro_abs_int"));
        assert!(code.contains("tauraro_pow_native"));
    }
}
