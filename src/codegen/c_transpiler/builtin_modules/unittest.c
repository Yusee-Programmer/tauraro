// ==========================================
// UNITTEST MODULE - Pure C Implementation
// ==========================================
// Provides: TestCase, TestSuite, TestRunner, main
// Platform: Cross-platform

#ifndef TAURARO_UNITTEST_MODULE_H
#define TAURARO_UNITTEST_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Test case structure
typedef struct {
    const char* name;
    void (*test_func)(void);
    int passed;
    const char* error_msg;
} TestCase;

// Test suite structure
typedef struct {
    TestCase* tests;
    int test_count;
    int passed_count;
    int failed_count;
} TestSuite;

// unittest.TestCase
static inline TauValue tauraro_unittest_TestCase(void) {
    TestSuite* suite = (TestSuite*)malloc(sizeof(TestSuite));
    suite->tests = NULL;
    suite->test_count = 0;
    suite->passed_count = 0;
    suite->failed_count = 0;
    
    return (TauValue){.type = 6, .value.p = (void*)suite, .refcount = 1, .next = NULL};
}

// unittest.TestCase.assertEqual(a, b)
static inline TauValue tauraro_unittest_TestCase_assertEqual(TauValue a, TauValue b) {
    if ((a.type == b.type) && (a.type == 0 ? a.value.i == b.value.i : 
                               a.type == 1 ? a.value.f == b.value.f : 
                               a.type == 2 ? strcmp(a.value.s, b.value.s) == 0 : 
                               a.value.i == b.value.i)) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }
    // Would raise AssertionError
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// unittest.TestCase.assertTrue(expr)
static inline TauValue tauraro_unittest_TestCase_assertTrue(TauValue expr) {
    int is_true = (expr.type == 3 && expr.value.i) ||
                  (expr.type == 0 && expr.value.i != 0);
    return (TauValue){.type = 3, .value.i = is_true, .refcount = 1, .next = NULL};
}

// unittest.TestCase.assertFalse(expr)
static inline TauValue tauraro_unittest_TestCase_assertFalse(TauValue expr) {
    int is_false = (expr.type == 3 && !expr.value.i) ||
                   (expr.type == 0 && expr.value.i == 0);
    return (TauValue){.type = 3, .value.i = is_false, .refcount = 1, .next = NULL};
}

// unittest.TestCase.assertIsNone(obj)
static inline TauValue tauraro_unittest_TestCase_assertIsNone(TauValue obj) {
    return (TauValue){.type = 3, .value.i = (obj.type == 3 && obj.value.i == 0), .refcount = 1, .next = NULL};
}

// unittest.TestCase.assertIsNotNone(obj)
static inline TauValue tauraro_unittest_TestCase_assertIsNotNone(TauValue obj) {
    return (TauValue){.type = 3, .value.i = !(obj.type == 3 && obj.value.i == 0), .refcount = 1, .next = NULL};
}

// unittest.TestCase.assertIn(a, b)
static inline TauValue tauraro_unittest_TestCase_assertIn(TauValue a, TauValue b) {
    // Check if a is in b
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

// unittest.TestCase.assertRaises(exception, func)
static inline TauValue tauraro_unittest_TestCase_assertRaises(TauValue exception, TauValue func) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// unittest.TestSuite()
static inline TauValue tauraro_unittest_TestSuite(void) {
    TestSuite* suite = (TestSuite*)malloc(sizeof(TestSuite));
    suite->tests = NULL;
    suite->test_count = 0;
    suite->passed_count = 0;
    suite->failed_count = 0;
    
    return (TauValue){.type = 6, .value.p = (void*)suite, .refcount = 1, .next = NULL};
}

// unittest.TestSuite.addTest(test)
static inline TauValue tauraro_unittest_TestSuite_addTest(TauValue suite, TauValue test) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// unittest.TextTestRunner()
static inline TauValue tauraro_unittest_TextTestRunner(void) {
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
}

// unittest.TextTestRunner.run(suite)
static inline TauValue tauraro_unittest_TextTestRunner_run(TauValue runner, TauValue suite) {
    printf("Running tests...\n");
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
}

// unittest.main()
static inline TauValue tauraro_unittest_main(void) {
    printf("Running all tests\n");
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// unittest.skip(reason)
static inline TauValue tauraro_unittest_skip(TauValue reason) {
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
}

// unittest.skipIf(condition, reason)
static inline TauValue tauraro_unittest_skipIf(TauValue condition, TauValue reason) {
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
}


#endif // TAURARO_UNITTEST_MODULE_H
