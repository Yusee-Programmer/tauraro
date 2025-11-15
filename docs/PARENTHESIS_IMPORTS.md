# Parenthesis Bracket Style Imports

## Overview

Tauraro now supports Python-style multi-line imports using parentheses, making it easy to import multiple items from a module across multiple lines.

## Supported Syntax

### 1. Single-line with parentheses
```python
from webviewtk import (Window)
import (sys)
```

### 2. Multi-line imports (main use case)
```python
from webviewtk import (
    Window, div, h1, h2, p, button, render, 
    titlebar, menu, menu_item, menu_separator, 
    cdn_tailwind, footer, section, header
)
```

### 3. Multi-line with aliases
```python
from webviewtk import (
    Window as W,
    div as d,
    h1 as heading1,
    h2 as heading2,
    render as r
)
```

### 4. Multi-line with star import
```python
from os import (
    *
)
```

### 5. Standard imports (backward compatible)
```python
# These still work
from os import path, sep, getcwd
import sys
```

## Features

✅ **Multi-line support**: Break long import lists across multiple lines  
✅ **Comment support**: Add comments on each line (if supported by lexer)  
✅ **Alias support**: Use `as` keyword to rename imports  
✅ **Backward compatible**: All existing import syntax still works  
✅ **Both import styles**: Works with both `import` and `from...import`  
✅ **Star imports**: `from module import (*)` is supported  

## Benefits

1. **Better Readability**: Organize imports logically across multiple lines
2. **Version Control**: Easier diffs when adding/removing imports
3. **Maintainability**: Clear structure for large import lists
4. **Python Compatible**: Matches Python's standard import style
5. **No Breaking Changes**: Existing code continues to work

## Implementation Details

### Parser Changes

The parser has been enhanced to:
- Detect opening parenthesis after `import` keyword in both `import` and `from...import` statements
- Skip newlines between imports when parentheses are present
- Properly match closing parenthesis
- Maintain backward compatibility with single-line imports

### Key Modifications

**File**: `src/parser.rs`

#### `import_statement()` function:
- Added detection of `(` after import keyword
- Handles module path parsing with optional parentheses
- Consumes closing `)` if opening `(` was found

#### `from_import_statement()` function:
- Added detection of `(` after the `import` keyword
- Skips newlines inside parentheses for multi-line support
- Maintains proper handling of commas between import items
- Handles aliases with `as` keyword
- Supports star imports `*`

### How it Works

1. **Parenthesis Detection**: When parsing imports, check for opening `(`
2. **Newline Handling**: Inside parentheses, skip newlines to allow multi-line imports
3. **Item Parsing**: Parse import items normally (identifier, optional `as` alias, comma)
4. **Closing Paren**: Match the closing `)` with optional newline before it
5. **Fallback**: If no parenthesis, uses original single-line parsing

## Examples

### Before (Single Line)
```python
from webviewtk import Window, div, h1, h2, p, button, render, titlebar, menu, menu_item, menu_separator, cdn_tailwind, footer, section, header
```

### After (Multi-Line with Parentheses)
```python
from webviewtk import (
    Window, div, h1, h2, p, button, render, 
    titlebar, menu, menu_item, menu_separator, 
    cdn_tailwind, footer, section, header
)
```

## Edge Cases Handled

1. **Empty lines**: Newlines are properly skipped inside parentheses
2. **Trailing commas**: Can have a comma after the last item (though not required)
3. **Multiple spaces**: Handled by the lexer/tokenizer
4. **Mixed format**: Can mix single imports and aliased imports
5. **Star imports**: Special case for `from module import *`

## Backward Compatibility

✅ All existing code continues to work:
```python
# Old style - still works
from os import path
import sys
from collections import Counter, defaultdict, OrderedDict

# New style - now also works
from os import (path)
import (sys)
from collections import (
    Counter, 
    defaultdict, 
    OrderedDict
)
```

## Testing

Test files demonstrating the feature:
- `examples/test_parenthesis_imports.py` - Python version
- `examples/test_parenthesis_imports.tr` - Tauraro version

Both examples show various import styles and create a WebViewTK window to verify imports work correctly.

## Performance

No performance impact - parentheses are handled at parse time and don't affect runtime performance.

## Future Enhancements

Potential improvements:
1. Comment support within import parentheses
2. Trailing comma linting
3. Import reorganization tools
4. Auto-formatting of import lists

## Related Documentation

- [Python Import Documentation](https://docs.python.org/3/reference/simple_stmts.html#import)
- `TITLEBAR_MENU_GUIDE.md` - WebViewTK import examples
- `parser.rs` - Full parser implementation

## Summary

The parenthesis bracket style import support makes Tauraro imports more Pythonic and readable, especially for modules with many exported items. It maintains full backward compatibility while providing modern import formatting options.
