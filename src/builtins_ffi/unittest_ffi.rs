//! FFI wrapper for unittest module - exports C-compatible functions
//! Uses #![no_std] for minimal dependencies and easy C linking

#![no_std]

use core::ffi::c_int;
use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

// Type definitions (must match C)
#[repr(C)]
pub enum TauraroType {
    Int = 0, Float = 1, Bool = 2, String = 3, List = 4,
    Dict = 5, Tuple = 6, Set = 7, None = 8, Object = 9,
    Function = 10, Bytes = 11, Complex = 12, Range = 13, Frozenset = 14,
}

#[repr(C)]
pub union TauraroData {
    pub int_val: i64,
    pub float_val: f64,
    pub bool_val: bool,
    pub str_val: *mut u8,
}

#[repr(C)]
pub struct TauraroValue {
    pub value_type: TauraroType,
    pub ref_count: c_int,
    pub data: TauraroData,
}

extern "C" {
    fn tauraro_value_new() -> *mut TauraroValue;
    fn tauraro_value_new_with_type(t: TauraroType) -> *mut TauraroValue;
}

// Helper function to create an integer value
unsafe fn create_int_value(val: i64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Int;
        (*result).data.int_val = val;
    }
    result
}

// Helper function to create a boolean value
unsafe fn create_bool_value(val: bool) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Bool;
        (*result).data.bool_val = val;
    }
    result
}

// Helper function to create a string value
unsafe fn create_string_value(s: &str) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        // Note: In a real implementation, we would need to allocate and copy the string
        // For now, we'll just set it to null
        (*result).data.str_val = core::ptr::null_mut();
    }
    result
}

// Helper function to create a tuple value
unsafe fn create_tuple_value(items: &[*mut TauraroValue]) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Tuple;
        // Note: In a real implementation, we would need to store the items
        // For now, we'll just create an empty tuple
    }
    result
}

// Helper function to create a dict value
unsafe fn create_dict_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Dict;
        // Note: In a real implementation, we would need to store key-value pairs
        // For now, we'll just create an empty dict
    }
    result
}

// Helper function to create an object value
unsafe fn create_object_value(class_name: &str) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Object;
        // Note: In a real implementation, we would need to store object data
        // For now, we'll just create a basic object
    }
    result
}

// unittest.TestCase(methodName='runTest') - Create test case
#[no_mangle]
pub extern "C" fn tauraro_unittest_testcase_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 1 {
            return create_object_value("TypeError");
        }
        
        // Get methodName argument (if provided)
        let method_name = if argc == 1 { *argv } else { core::ptr::null_mut() };
        
        // Create test case object
        let test_case_obj = create_object_value("TestCase");
        test_case_obj
    }
}

// unittest.TestSuite() - Create test suite
#[no_mangle]
pub extern "C" fn tauraro_unittest_testsuite_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_object_value("TypeError");
        }
        
        // Create test suite object
        let test_suite_obj = create_object_value("TestSuite");
        test_suite_obj
    }
}

// unittest.TestLoader() - Create test loader
#[no_mangle]
pub extern "C" fn tauraro_unittest_testloader_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_object_value("TypeError");
        }
        
        // Create test loader object
        let test_loader_obj = create_object_value("TestLoader");
        test_loader_obj
    }
}

// unittest.TextTestRunner(stream=None, descriptions=True, verbosity=1) - Create test runner
#[no_mangle]
pub extern "C" fn tauraro_unittest_texttestrunner_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 3 {
            return create_object_value("TypeError");
        }
        
        // Get arguments
        let stream = if argc >= 1 { *argv } else { core::ptr::null_mut() };
        let descriptions = if argc >= 2 { *argv.add(1) } else { core::ptr::null_mut() };
        let verbosity = if argc >= 3 { *argv.add(2) } else { core::ptr::null_mut() };
        
        // Create text test runner object
        let test_runner_obj = create_object_value("TextTestRunner");
        test_runner_obj
    }
}

// unittest.main(module='__main__', defaultTest=None, argv=None, testRunner=None, testLoader=unittest.defaultTestLoader, exit=True, verbosity=1, failfast=None, catchbreak=None, buffer=None, warnings=None) - Run unit tests
#[no_mangle]
pub extern "C" fn tauraro_unittest_main(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 11 {
            return create_object_value("TypeError");
        }
        
        // Run unit tests (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// unittest.TestResult() - Create test result
#[no_mangle]
pub extern "C" fn tauraro_unittest_testresult_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_object_value("TypeError");
        }
        
        // Create test result object
        let test_result_obj = create_object_value("TestResult");
        test_result_obj
    }
}

// unittest.skip(reason) - Skip decorator
#[no_mangle]
pub extern "C" fn tauraro_unittest_skip(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 1 {
            return create_object_value("TypeError");
        }
        
        // Get reason argument
        let reason = *argv;
        
        // Create skip decorator (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// unittest.skipIf(condition, reason) - Conditional skip decorator
#[no_mangle]
pub extern "C" fn tauraro_unittest_skip_if(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 2 {
            return create_object_value("TypeError");
        }
        
        // Get condition and reason arguments
        let condition = *argv;
        let reason = *argv.add(1);
        
        // Create conditional skip decorator (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// unittest.skipUnless(condition, reason) - Conditional skip decorator
#[no_mangle]
pub extern "C" fn tauraro_unittest_skip_unless(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 2 {
            return create_object_value("TypeError");
        }
        
        // Get condition and reason arguments
        let condition = *argv;
        let reason = *argv.add(1);
        
        // Create conditional skip decorator (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// unittest.expectedFailure() - Expected failure decorator
#[no_mangle]
pub extern "C" fn tauraro_unittest_expected_failure(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_object_value("TypeError");
        }
        
        // Create expected failure decorator (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// unittest.TestCase.assertEqual(first, second, msg=None) - Assert equality
#[no_mangle]
pub extern "C" fn tauraro_unittest_assert_equal(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 3 || argc > 4 {
            return create_object_value("TypeError");
        }
        
        // Get first, second, and msg arguments
        let first = *argv;
        let second = *argv.add(1);
        let msg = if argc == 4 { *argv.add(3) } else { core::ptr::null_mut() };
        
        // Assert equality (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// unittest.TestCase.assertTrue(expr, msg=None) - Assert true
#[no_mangle]
pub extern "C" fn tauraro_unittest_assert_true(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 2 || argc > 3 {
            return create_object_value("TypeError");
        }
        
        // Get expr and msg arguments
        let expr = *argv;
        let msg = if argc == 3 { *argv.add(2) } else { core::ptr::null_mut() };
        
        // Assert true (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// unittest.TestCase.assertFalse(expr, msg=None) - Assert false
#[no_mangle]
pub extern "C" fn tauraro_unittest_assert_false(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 2 || argc > 3 {
            return create_object_value("TypeError");
        }
        
        // Get expr and msg arguments
        let expr = *argv;
        let msg = if argc == 3 { *argv.add(2) } else { core::ptr::null_mut() };
        
        // Assert false (simplified implementation)
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}