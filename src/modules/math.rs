/// Math module - provides mathematical functions and constants
/// Similar to Python's math module

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::f64::consts;

/// Create the math module object with all its functions and constants
pub fn create_math_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Mathematical constants
    namespace.insert("pi".to_string(), Value::Float(consts::PI));
    namespace.insert("e".to_string(), Value::Float(consts::E));
    namespace.insert("tau".to_string(), Value::Float(consts::TAU));
    namespace.insert("inf".to_string(), Value::Float(f64::INFINITY));
    namespace.insert("nan".to_string(), Value::Float(f64::NAN));
    
    // Power and logarithmic functions
    namespace.insert("pow".to_string(), Value::NativeFunction(math_pow));
    namespace.insert("sqrt".to_string(), Value::NativeFunction(math_sqrt));
    namespace.insert("exp".to_string(), Value::NativeFunction(math_exp));
    namespace.insert("exp2".to_string(), Value::NativeFunction(math_exp2));
    namespace.insert("expm1".to_string(), Value::NativeFunction(math_expm1));
    namespace.insert("log".to_string(), Value::NativeFunction(math_log));
    namespace.insert("log2".to_string(), Value::NativeFunction(math_log2));
    namespace.insert("log10".to_string(), Value::NativeFunction(math_log10));
    namespace.insert("log1p".to_string(), Value::NativeFunction(math_log1p));
    
    // Trigonometric functions
    namespace.insert("sin".to_string(), Value::NativeFunction(math_sin));
    namespace.insert("cos".to_string(), Value::NativeFunction(math_cos));
    namespace.insert("tan".to_string(), Value::NativeFunction(math_tan));
    namespace.insert("asin".to_string(), Value::NativeFunction(math_asin));
    namespace.insert("acos".to_string(), Value::NativeFunction(math_acos));
    namespace.insert("atan".to_string(), Value::NativeFunction(math_atan));
    namespace.insert("atan2".to_string(), Value::NativeFunction(math_atan2));
    
    // Hyperbolic functions
    namespace.insert("sinh".to_string(), Value::NativeFunction(math_sinh));
    namespace.insert("cosh".to_string(), Value::NativeFunction(math_cosh));
    namespace.insert("tanh".to_string(), Value::NativeFunction(math_tanh));
    namespace.insert("asinh".to_string(), Value::NativeFunction(math_asinh));
    namespace.insert("acosh".to_string(), Value::NativeFunction(math_acosh));
    namespace.insert("atanh".to_string(), Value::NativeFunction(math_atanh));
    
    // Angular conversion
    namespace.insert("degrees".to_string(), Value::NativeFunction(math_degrees));
    namespace.insert("radians".to_string(), Value::NativeFunction(math_radians));
    
    // Number-theoretic and representation functions
    namespace.insert("ceil".to_string(), Value::NativeFunction(math_ceil));
    namespace.insert("floor".to_string(), Value::NativeFunction(math_floor));
    namespace.insert("trunc".to_string(), Value::NativeFunction(math_trunc));
    namespace.insert("fabs".to_string(), Value::NativeFunction(math_fabs));
    namespace.insert("factorial".to_string(), Value::NativeFunction(math_factorial));
    namespace.insert("gcd".to_string(), Value::NativeFunction(math_gcd));
    namespace.insert("lcm".to_string(), Value::NativeFunction(math_lcm));
    
    // Floating point operations
    namespace.insert("fmod".to_string(), Value::NativeFunction(math_fmod));
    namespace.insert("remainder".to_string(), Value::NativeFunction(math_remainder));
    namespace.insert("modf".to_string(), Value::NativeFunction(math_modf));
    namespace.insert("frexp".to_string(), Value::NativeFunction(math_frexp));
    namespace.insert("ldexp".to_string(), Value::NativeFunction(math_ldexp));
    namespace.insert("copysign".to_string(), Value::NativeFunction(math_copysign));
    
    // Classification functions
    namespace.insert("isfinite".to_string(), Value::NativeFunction(math_isfinite));
    namespace.insert("isinf".to_string(), Value::NativeFunction(math_isinf));
    namespace.insert("isnan".to_string(), Value::NativeFunction(math_isnan));
    namespace.insert("isclose".to_string(), Value::NativeFunction(math_isclose));
    
    // Special functions
    namespace.insert("gamma".to_string(), Value::NativeFunction(math_gamma));
    namespace.insert("lgamma".to_string(), Value::NativeFunction(math_lgamma));
    namespace.insert("erf".to_string(), Value::NativeFunction(math_erf));
    namespace.insert("erfc".to_string(), Value::NativeFunction(math_erfc));
    
    Value::Module("math".to_string(), namespace)
}

// Helper function to convert Value to f64
fn value_to_float(value: &Value) -> Result<f64> {
    match value {
        Value::Int(i) => Ok(*i as f64),
        Value::Float(f) => Ok(*f),
        _ => Err(anyhow!("Expected a number")),
    }
}

// Helper function to convert Value to i64
fn value_to_int(value: &Value) -> Result<i64> {
    match value {
        Value::Int(i) => Ok(*i),
        Value::Float(f) => Ok(*f as i64),
        _ => Err(anyhow!("Expected a number")),
    }
}

/// Power function: pow(x, y)
fn math_pow(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("pow() takes exactly 2 arguments"));
    }
    
    let x = value_to_float(&args[0])?;
    let y = value_to_float(&args[1])?;
    
    Ok(Value::Float(x.powf(y)))
}

/// Square root function: sqrt(x)
fn math_sqrt(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("sqrt() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    if x < 0.0 {
        return Err(anyhow!("math domain error"));
    }
    
    Ok(Value::Float(x.sqrt()))
}

/// Exponential function: exp(x)
fn math_exp(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("exp() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.exp()))
}

/// Base-2 exponential function: exp2(x)
fn math_exp2(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("exp2() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.exp2()))
}

/// exp(x) - 1 function: expm1(x)
fn math_expm1(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("expm1() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.exp_m1()))
}

/// Natural logarithm function: log(x, base=e)
fn math_log(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow!("log() takes 1 or 2 arguments"));
    }
    
    let x = value_to_float(&args[0])?;
    if x <= 0.0 {
        return Err(anyhow!("math domain error"));
    }
    
    if args.len() == 1 {
        Ok(Value::Float(x.ln()))
    } else {
        let base = value_to_float(&args[1])?;
        if base <= 0.0 || base == 1.0 {
            return Err(anyhow!("math domain error"));
        }
        Ok(Value::Float(x.log(base)))
    }
}

/// Base-2 logarithm function: log2(x)
fn math_log2(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("log2() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    if x <= 0.0 {
        return Err(anyhow!("math domain error"));
    }
    
    Ok(Value::Float(x.log2()))
}

/// Base-10 logarithm function: log10(x)
fn math_log10(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("log10() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    if x <= 0.0 {
        return Err(anyhow!("math domain error"));
    }
    
    Ok(Value::Float(x.log10()))
}

/// log(1 + x) function: log1p(x)
fn math_log1p(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("log1p() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    if x <= -1.0 {
        return Err(anyhow!("math domain error"));
    }
    
    Ok(Value::Float(x.ln_1p()))
}

/// Sine function: sin(x)
fn math_sin(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("sin() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.sin()))
}

/// Cosine function: cos(x)
fn math_cos(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("cos() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.cos()))
}

/// Tangent function: tan(x)
fn math_tan(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("tan() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.tan()))
}

/// Arcsine function: asin(x)
fn math_asin(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("asin() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    if x < -1.0 || x > 1.0 {
        return Err(anyhow!("math domain error"));
    }
    
    Ok(Value::Float(x.asin()))
}

/// Arccosine function: acos(x)
fn math_acos(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("acos() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    if x < -1.0 || x > 1.0 {
        return Err(anyhow!("math domain error"));
    }
    
    Ok(Value::Float(x.acos()))
}

/// Arctangent function: atan(x)
fn math_atan(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("atan() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.atan()))
}

/// Two-argument arctangent function: atan2(y, x)
fn math_atan2(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("atan2() takes exactly 2 arguments"));
    }
    
    let y = value_to_float(&args[0])?;
    let x = value_to_float(&args[1])?;
    
    Ok(Value::Float(y.atan2(x)))
}

/// Hyperbolic sine function: sinh(x)
fn math_sinh(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("sinh() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.sinh()))
}

/// Hyperbolic cosine function: cosh(x)
fn math_cosh(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("cosh() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.cosh()))
}

/// Hyperbolic tangent function: tanh(x)
fn math_tanh(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("tanh() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.tanh()))
}

/// Inverse hyperbolic sine function: asinh(x)
fn math_asinh(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("asinh() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.asinh()))
}

/// Inverse hyperbolic cosine function: acosh(x)
fn math_acosh(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("acosh() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    if x < 1.0 {
        return Err(anyhow!("math domain error"));
    }
    
    Ok(Value::Float(x.acosh()))
}

/// Inverse hyperbolic tangent function: atanh(x)
fn math_atanh(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("atanh() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    if x <= -1.0 || x >= 1.0 {
        return Err(anyhow!("math domain error"));
    }
    
    Ok(Value::Float(x.atanh()))
}

/// Convert radians to degrees: degrees(x)
fn math_degrees(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("degrees() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.to_degrees()))
}

/// Convert degrees to radians: radians(x)
fn math_radians(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("radians() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.to_radians()))
}

/// Ceiling function: ceil(x)
fn math_ceil(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("ceil() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Int(x.ceil() as i64))
}

/// Floor function: floor(x)
fn math_floor(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("floor() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Int(x.floor() as i64))
}

/// Truncate function: trunc(x)
fn math_trunc(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("trunc() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Int(x.trunc() as i64))
}

/// Absolute value function: fabs(x)
fn math_fabs(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("fabs() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Float(x.abs()))
}

/// Factorial function: factorial(x)
fn math_factorial(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("factorial() takes exactly 1 argument"));
    }
    
    let x = value_to_int(&args[0])?;
    if x < 0 {
        return Err(anyhow!("factorial() not defined for negative values"));
    }
    
    let mut result = 1i64;
    for i in 1..=x {
        result = result.checked_mul(i)
            .ok_or_else(|| anyhow!("factorial() result too large"))?;
    }
    
    Ok(Value::Int(result))
}

/// Greatest common divisor function: gcd(a, b)
fn math_gcd(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("gcd() takes exactly 2 arguments"));
    }
    
    let mut a = value_to_int(&args[0])?.abs();
    let mut b = value_to_int(&args[1])?.abs();
    
    while b != 0 {
        let temp = b;
        b = a % b;
        a = temp;
    }
    
    Ok(Value::Int(a))
}

/// Least common multiple function: lcm(a, b)
fn math_lcm(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("lcm() takes exactly 2 arguments"));
    }
    
    let a = value_to_int(&args[0])?.abs();
    let b = value_to_int(&args[1])?.abs();
    
    if a == 0 || b == 0 {
        return Ok(Value::Int(0));
    }
    
    // Calculate GCD first
    let gcd_result = math_gcd(vec![Value::Int(a), Value::Int(b)])?;
    let gcd_val = match gcd_result {
        Value::Int(i) => i,
        _ => return Err(anyhow!("Internal error in lcm calculation")),
    };
    
    let lcm = (a / gcd_val).checked_mul(b)
        .ok_or_else(|| anyhow!("lcm() result too large"))?;
    
    Ok(Value::Int(lcm))
}

/// Floating point modulo function: fmod(x, y)
fn math_fmod(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("fmod() takes exactly 2 arguments"));
    }
    
    let x = value_to_float(&args[0])?;
    let y = value_to_float(&args[1])?;
    
    if y == 0.0 {
        return Err(anyhow!("math domain error"));
    }
    
    Ok(Value::Float(x % y))
}

/// IEEE remainder function: remainder(x, y)
fn math_remainder(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("remainder() takes exactly 2 arguments"));
    }
    
    let x = value_to_float(&args[0])?;
    let y = value_to_float(&args[1])?;
    
    if y == 0.0 {
        return Err(anyhow!("math domain error"));
    }
    
    // IEEE remainder: x - n*y where n is the integer nearest to x/y
    let n = (x / y).round();
    Ok(Value::Float(x - n * y))
}

/// Return fractional and integer parts: modf(x)
fn math_modf(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("modf() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    let integer_part = x.trunc();
    let fractional_part = x - integer_part;
    
    Ok(Value::Tuple(vec![
        Value::Float(fractional_part),
        Value::Float(integer_part),
    ]))
}

/// Return mantissa and exponent: frexp(x)
fn math_frexp(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("frexp() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    
    if x == 0.0 {
        return Ok(Value::Tuple(vec![Value::Float(0.0), Value::Int(0)]));
    }
    
    let exp = x.abs().log2().floor() as i64 + 1;
    let mantissa = x / (2.0_f64.powi(exp as i32));
    
    Ok(Value::Tuple(vec![
        Value::Float(mantissa),
        Value::Int(exp),
    ]))
}

/// Return x * 2^i: ldexp(x, i)
fn math_ldexp(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("ldexp() takes exactly 2 arguments"));
    }
    
    let x = value_to_float(&args[0])?;
    let i = value_to_int(&args[1])?;
    
    Ok(Value::Float(x * (2.0_f64.powi(i as i32))))
}

/// Return x with the sign of y: copysign(x, y)
fn math_copysign(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow!("copysign() takes exactly 2 arguments"));
    }
    
    let x = value_to_float(&args[0])?;
    let y = value_to_float(&args[1])?;
    
    Ok(Value::Float(x.copysign(y)))
}

/// Check if x is finite: isfinite(x)
fn math_isfinite(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("isfinite() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Bool(x.is_finite()))
}

/// Check if x is infinite: isinf(x)
fn math_isinf(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("isinf() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Bool(x.is_infinite()))
}

/// Check if x is NaN: isnan(x)
fn math_isnan(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("isnan() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    Ok(Value::Bool(x.is_nan()))
}

/// Check if values are close: isclose(a, b, rel_tol=1e-09, abs_tol=0.0)
fn math_isclose(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 4 {
        return Err(anyhow!("isclose() takes 2 to 4 arguments"));
    }
    
    let a = value_to_float(&args[0])?;
    let b = value_to_float(&args[1])?;
    
    let rel_tol = if args.len() > 2 {
        value_to_float(&args[2])?
    } else {
        1e-9
    };
    
    let abs_tol = if args.len() > 3 {
        value_to_float(&args[3])?
    } else {
        0.0
    };
    
    if rel_tol < 0.0 || abs_tol < 0.0 {
        return Err(anyhow!("tolerances must be non-negative"));
    }
    
    if a == b {
        return Ok(Value::Bool(true));
    }
    
    if a.is_infinite() || b.is_infinite() {
        return Ok(Value::Bool(false));
    }
    
    let diff = (a - b).abs();
    let close = diff <= (rel_tol * a.abs().max(b.abs())).max(abs_tol);
    
    Ok(Value::Bool(close))
}

/// Gamma function: gamma(x)
fn math_gamma(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("gamma() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    
    // Simple approximation using Stirling's formula for demonstration
    // In a real implementation, you'd use a proper gamma function
    if x <= 0.0 {
        return Err(anyhow!("math domain error"));
    }
    
    // For x > 1, use Γ(x) = (x-1)!
    if x == x.floor() && x <= 20.0 {
        let n = x as i64 - 1;
        if n >= 0 {
            let factorial_result = math_factorial(vec![Value::Int(n)])?;
            return match factorial_result {
                Value::Int(i) => Ok(Value::Float(i as f64)),
                _ => Err(anyhow!("Internal error in gamma calculation")),
            };
        }
    }
    
    // Stirling's approximation for large x
    let result = (2.0 * consts::PI / x).sqrt() * (x / consts::E).powf(x);
    Ok(Value::Float(result))
}

/// Log gamma function: lgamma(x)
fn math_lgamma(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("lgamma() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    
    if x <= 0.0 {
        return Err(anyhow!("math domain error"));
    }
    
    // Simple approximation
    let gamma_result = math_gamma(vec![Value::Float(x)])?;
    match gamma_result {
        Value::Float(g) => Ok(Value::Float(g.ln())),
        _ => Err(anyhow!("Internal error in lgamma calculation")),
    }
}

/// Error function: erf(x)
fn math_erf(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("erf() takes exactly 1 argument"));
    }
    
    let x = value_to_float(&args[0])?;
    
    // Abramowitz and Stegun approximation
    let a1 = 0.254829592;
    let a2 = -0.284496736;
    let a3 = 1.421413741;
    let a4 = -1.453152027;
    let a5 = 1.061405429;
    let p = 0.3275911;
    
    let sign = if x >= 0.0 { 1.0 } else { -1.0 };
    let x = x.abs();
    
    let t = 1.0 / (1.0 + p * x);
    let y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * (-x * x).exp();
    
    Ok(Value::Float(sign * y))
}

/// Complementary error function: erfc(x)
fn math_erfc(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow!("erfc() takes exactly 1 argument"));
    }
    
    let erf_result = math_erf(args)?;
    match erf_result {
        Value::Float(erf_val) => Ok(Value::Float(1.0 - erf_val)),
        _ => Err(anyhow!("Internal error in erfc calculation")),
    }
}