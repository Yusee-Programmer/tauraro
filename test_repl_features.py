#!/usr/bin/env python3
"""
Comprehensive Tauraro REPL Feature Test Script

This script tests all major Tauraro language features through the REPL.
It verifies that the REPL supports everything that the file-based execution does.

Usage: python3 test_repl_features.py <path_to_tauraro_executable>
"""

import subprocess
import sys
import os

class REPLTester:
    def __init__(self, tauraro_exe):
        self.tauraro_exe = tauraro_exe
        self.tests_passed = 0
        self.tests_failed = 0
        self.current_test = ""
        
    def run_repl_command(self, code):
        """Execute code in REPL and return output"""
        try:
            process = subprocess.Popen(
                [self.tauraro_exe, 'repl'],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            
            # Send code followed by exit command with a safety timeout
            input_data = code + "\n\nexit()\n"
            try:
                stdout, stderr = process.communicate(input=input_data, timeout=5)
            except AttributeError:
                # Older Python without timeout support
                stdout, stderr = process.communicate(input=input_data)
            
            # Strip REPL banner and prompts from output
            lines = stdout.split('\n')
            output_lines = []
            in_banner = True
            
            for i, line in enumerate(lines):
                # Skip banner lines (first few lines before actual output)
                if in_banner:
                    if line.startswith('Tauraro'):
                        continue
                    elif line.startswith('[Rust-based'):
                        continue
                    elif line.startswith('Type "help'):
                        continue
                    elif line.strip() == '':
                        # End of banner
                        in_banner = False
                        continue
                    else:
                        in_banner = False
                
                # Remove prompt characters but keep the output
                if line.startswith('>>> ') or line.startswith('... '):
                    # This is a prompt line with command, skip it
                    pass
                else:
                    output_lines.append(line)
            
            cleaned_stdout = '\n'.join(output_lines).strip()
            
            return cleaned_stdout, stderr, True
        except Exception as e:
            return "", str(e), False
    
    def test(self, name, code, expected_in_output=None, should_error=False):
        """Run a single test"""
        self.current_test = name
        print(f"\n[TEST] {name}")
        print(f"  Code: {code[:60]}{'...' if len(code) > 60 else ''}")
        
        stdout, stderr, success = self.run_repl_command(code)
        
        if not success:
            print(f"  FAILED: {stderr}")
            self.tests_failed += 1
            return
        
        if should_error:
            if "Error" in stderr or "Traceback" in stderr or "Error" in stdout:
                print(f"  PASSED (error as expected)")
                self.tests_passed += 1
            else:
                print(f"  FAILED: Expected error but got none")
                print(f"     stdout: {stdout[:100]}")
                self.tests_failed += 1
        elif expected_in_output:
            if expected_in_output in stdout:
                print(f"  PASSED")
                self.tests_passed += 1
            else:
                print(f"  FAILED: Expected '{expected_in_output}' in output")
                print(f"     Got: {stdout[:200]}")
                self.tests_failed += 1
        else:
            if "Error" not in stderr and "error" not in stdout.lower():
                print(f"  PASSED")
                self.tests_passed += 1
            else:
                print(f"  FAILED: Unexpected error")
                print(f"     stderr: {stderr[:100]}")
                self.tests_failed += 1
    
    def run_all_tests(self):
        """Run all feature tests"""
        print("=" * 70)
        print("TAURARO REPL COMPREHENSIVE FEATURE TEST")
        print("=" * 70)
        
        # Basic Operations
        print("\n### BASIC OPERATIONS ###")
        self.test("Integer arithmetic", "print(1 + 2)", "3")
        self.test("Float arithmetic", "print(1.5 + 2.5)", "4.0")
        self.test("String concatenation", "print('hello' + ' ' + 'world')", "hello world")
        self.test("Variable assignment", "x = 10\nprint(x)", "10")
        
        # Collections
        print("\n### COLLECTIONS ###")
        self.test("List creation", "x = [1, 2, 3]\nprint(len(x))", "3")
        self.test("List indexing", "x = [10, 20, 30]\nprint(x[1])", "20")
        self.test("Dictionary creation", "d = {'key': 'value'}\nprint(d['key'])", "value")
        self.test("Tuple creation", "t = (1, 2, 3)\nprint(t[0])", "1")
        self.test("Set creation", "s = {1, 2, 3}\nprint(len(s))", "3")
        
        # Control Flow
        print("\n### CONTROL FLOW ###")
        self.test("If statement", "if 5 > 3:\n    print('yes')", "yes")
        self.test("If-else statement", "if 5 < 3:\n    print('no')\nelse:\n    print('yes')", "yes")
        self.test("For loop", "for i in range(3):\n    print(i)", "0")
        self.test("While loop", "i = 0\nwhile i < 3:\n    print(i)\n    i = i + 1", "0")
        self.test("Break statement", "for i in range(10):\n    if i == 2:\n        break\n    print(i)", "0")
        self.test("Continue statement", "for i in range(3):\n    if i == 1:\n        continue\n    print(i)", "0")
        
        # Functions
        print("\n### FUNCTIONS ###")
        self.test("Function definition", "def add(a, b):\n    return a + b\nprint(add(2, 3))", "5")
        self.test("Function with default args", "def greet(name='World'):\n    return 'Hello, ' + name\nprint(greet())", "Hello, World")
        self.test("Function with *args", "def sum_all(*args):\n    return sum(args)\nprint(sum_all(1, 2, 3))", "6")
        self.test("Lambda function", "f = lambda x: x * 2\nprint(f(5))", "10")
        self.test("Nested function", "def outer():\n    def inner():\n        return 42\n    return inner()\nprint(outer())", "42")
        self.test("Closure", "def make_adder(n):\n    def adder(x):\n        return x + n\n    return adder\nadd5 = make_adder(5)\nprint(add5(3))", "8")
        
        # Classes and Objects
        print("\n### CLASSES AND OBJECTS ###")
        self.test("Class definition", "class Point:\n    def __init__(self, x, y):\n        self.x = x\n        self.y = y\np = Point(3, 4)\nprint(p.x)", "3")
        self.test("Class method", "class Math:\n    @staticmethod\n    def add(a, b):\n        return a + b\nprint(Math.add(2, 3))", "5")
        self.test("Class inheritance", "class Animal:\n    def speak(self):\n        return 'sound'\nclass Dog(Animal):\n    def speak(self):\n        return 'woof'\nd = Dog()\nprint(d.speak())", "woof")
        
        # Comprehensions
        print("\n### COMPREHENSIONS ###")
        self.test("List comprehension", "x = [i*2 for i in range(5)]\nprint(x[0])", "0")
        self.test("Dict comprehension", "d = {i: i*2 for i in range(3)}\nprint(d[1])", "2")
        self.test("Set comprehension", "s = {i*2 for i in range(3)}\nprint(len(s))", "3")
        
        # Exception Handling
        print("\n### EXCEPTION HANDLING ###")
        self.test("Try-except", "try:\n    x = 1 / 0\nexcept ZeroDivisionError:\n    print('caught')", "caught")
        self.test("Try-except-else", "try:\n    x = 5\nexcept:\n    print('error')\nelse:\n    print('no error')", "no error")
        self.test("Try-except-finally", "try:\n    x = 1\nfinally:\n    print('cleanup')", "cleanup")
        self.test("Raise exception", "try:\n    raise ValueError('test')\nexcept ValueError as e:\n    print('caught')", "caught")
        self.test("Exception hierarchy", "try:\n    x = [1][10]\nexcept Exception:\n    print('caught')", "caught")
        
        # String Features
        print("\n### STRING FEATURES ###")
        self.test("F-string", "name = 'world'\nprint(f'hello {name}')", "hello world")
        self.test("Triple quote string", "s = '''multi\nline'''\nprint(len(s))", "10")
        self.test("String methods", "s = 'hello'\nprint(s.upper())", "HELLO")
        self.test("String slicing", "s = 'hello'\nprint(s[1:3])", "el")
        
        # Operators
        print("\n### OPERATORS ###")
        self.test("Power operator", "print(2 ** 3)", "8")
        self.test("Modulo operator", "print(10 % 3)", "1")
        self.test("Comparison", "print(5 > 3)", "True")
        self.test("Logical AND", "print(True and True)", "True")
        self.test("Logical OR", "print(False or True)", "True")
        self.test("Logical NOT", "print(not False)", "True")
        self.test("In operator", "print(2 in [1, 2, 3])", "True")
        self.test("Is operator", "x = None\nprint(x is None)", "True")
        
        # Built-in Functions
        print("\n### BUILT-IN FUNCTIONS ###")
        self.test("print", "print('hello')", "hello")
        self.test("len", "print(len([1, 2, 3]))", "3")
        self.test("range", "print(list(range(3)))", "[0, 1, 2]")
        self.test("type", "print(type(5))", "int")
        self.test("int conversion", "print(int('10'))", "10")
        self.test("str conversion", "print(str(42))", "42")
        self.test("float conversion", "print(float('3.14'))", "3.14")
        self.test("list", "print(list('abc'))", "['a', 'b', 'c']")
        self.test("dict", "print(dict([('a', 1)]))", "{'a': 1}")
        self.test("sum", "print(sum([1, 2, 3]))", "6")
        self.test("max", "print(max([1, 5, 3]))", "5")
        self.test("min", "print(min([1, 5, 3]))", "1")
        self.test("sorted", "print(sorted([3, 1, 2]))", "[1, 2, 3]")
        self.test("reversed", "print(list(reversed([1, 2, 3])))", "[3, 2, 1]")
        self.test("zip", "print(list(zip([1, 2], ['a', 'b'])))", "[(1, 'a'), (2, 'b')]")
        self.test("enumerate", "for i, v in enumerate(['a', 'b']):\n    print(i)", "0")
        self.test("map", "print(list(map(lambda x: x*2, [1, 2])))", "[2, 4]")
        self.test("filter", "print(list(filter(lambda x: x > 2, [1, 2, 3])))", "[3]")
        
        # Advanced Features
        print("\n### ADVANCED FEATURES ###")
        self.test("Decorators", "@lambda f: lambda: f() + 1\ndef get_num():\n    return 41\nprint(get_num())", "42")
        self.test("Generator", "def gen():\n    yield 1\n    yield 2\nprint(list(gen()))", "[1, 2]")
        self.test("Context manager", "class Ctx:\n    def __enter__(self):\n        return self\n    def __exit__(self, *args):\n        pass\nwith Ctx():\n    print('ok')", "ok")
        
        # Print summary
        print("\n" + "=" * 70)
        print("TEST SUMMARY")
        print("=" * 70)
        print(f"[OK] Passed: {self.tests_passed}")
        print(f"[FAIL] Failed: {self.tests_failed}")
        print(f"[TOTAL] Total:  {self.tests_passed + self.tests_failed}")
        
        if self.tests_failed == 0:
            print("\nALL TESTS PASSED! REPL supports all major features.")
            return 0
        else:
            print(f"\n{self.tests_failed} tests failed. REPL may have gaps.")
            return 1

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 test_repl_features.py <path_to_tauraro_executable>")
        sys.exit(1)
    
    tauraro_exe = sys.argv[1]
    
    if not os.path.exists(tauraro_exe):
        print(f"Error: Tauraro executable not found at {tauraro_exe}")
        sys.exit(1)
    
    tester = REPLTester(tauraro_exe)
    exit_code = tester.run_all_tests()
    sys.exit(exit_code)
