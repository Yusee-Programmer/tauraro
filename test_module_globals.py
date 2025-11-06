# testmod_globals.py - test module with globals
_module_var = "module level variable"
_number = 42

def test_func():
    global _module_var, _number
    print("In function:")
    print("  _module_var =", _module_var)
    print("  _number =", _number)
