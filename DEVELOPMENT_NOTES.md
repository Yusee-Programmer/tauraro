# Development Notes

## Temporary Test Files

During development and debugging of the FFI modules and VM parser fixes, numerous temporary test files were created. These files are not committed to the repository as they were used for isolated testing and debugging.

### VM Parser Development Tests

The following files were created to isolate and test specific parser issues:

**Dotted Import Tests:**
- `test_import.py` - Testing `import urllib.parse`
- `test_import_line13.py` - Isolating specific import line
- `test_dotted_import.py` - Basic dotted import test

**Keyword as Identifier Tests:**
- `test_match_keyword.py` - Testing `match` as variable name
- `test_assign.py` - Basic assignment tests
- `test_assign_module_call.py` - Assignment with module calls
- `test_assign_module_simple.py` - Simplified assignment test

**Bytes Literal Tests:**
- `test_bytes.py` - Basic bytes literal
- `test_bytes2.py` - Alternative bytes test
- `test_raw_simple.py` - Raw string literals
- `test_raw_string.py` - More raw string tests
- `test_search.py` - Raw strings in re.search()
- `test_call_raw.py` - Raw strings in function calls
- `test_module_call_raw.py` - Raw strings in module calls

**Other Development Tests:**
- `test_decorator.py` - Decorator syntax tests
- `test_final_check.py` - Final integration check
- `simple_test.py` - Simple smoke test
- `test_builtin_modules_nobytes.py` - Test without bytes-using modules

### Why These Weren't Committed

These files were:
1. **Temporary**: Created for debugging specific issues
2. **Redundant**: Functionality covered by main test suites
3. **Incremental**: Small steps toward larger tests
4. **Exploratory**: Used to understand error messages

The key tests that **were** committed:
- `test_builtin_modules.py` - Comprehensive module testing
- `test_builtin_modules_simple.py` - VM-compatible subset
- `test_vm_final.py` - Final VM parser demonstration
- `test_ffi_modules.py` - FFI functionality tests
- `test_ffi.py` / `test_ffi.c` - FFI integration tests

## Adding to .gitignore

If you want to prevent these temporary files from showing up in `git status`, add this to `.gitignore`:

```gitignore
# Temporary development test files
test_assign*.py
test_bytes*.py
test_call*.py
test_decorator.py
test_dotted_import.py
test_final_check.py
test_import*.py
test_match*.py
test_module*.py
test_raw*.py
test_search.py
simple_test.py
test_builtin_modules_nobytes.py
```

## Development Workflow

The typical workflow for fixing a parser issue was:

1. **Identify Error**: Run comprehensive test to see failure
2. **Isolate Issue**: Create minimal test file reproducing error
3. **Debug**: Add print statements, check token stream
4. **Fix**: Modify parser/lexer
5. **Verify**: Run isolated test, then full suite
6. **Cleanup**: Remove temporary test files

Example for dotted import fix:
- Started with: `test_builtin_modules.py` failing on line 13
- Created: `test_import.py` with just `import urllib.parse`
- Fixed: Added `consume_module_path()` method
- Verified: All tests pass
- Committed: Main tests only

## Object Files

Build artifacts in `build/builtin/*.o` were committed for convenience, but these can be regenerated with:

```bash
python compile_all_ffi.py
```

Consider adding `build/` to `.gitignore` in production to exclude build artifacts.

## Local Configuration

The file `.claude/settings.local.json` contains local IDE/tool settings and should not be committed. It's marked as modified but intentionally not committed.

## Summary

- **Committed**: Essential tests, documentation, working code
- **Skipped**: Temporary debug files, local configuration
- **Artifacts**: Object files committed for convenience (optional)

This approach keeps the repository clean while documenting the development process.
