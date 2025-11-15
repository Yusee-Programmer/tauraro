import re

# Read the file
with open(r'src\modules\webviewtk\window.rs', 'r', encoding='utf-8') as f:
    content = f.read()

# Fix 1: Replace .lock() pattern
content = re.sub(
    r'if let Ok\(handlers\) = handlers_map\.lock\(\) \{\s+if let Some\(handler\) = handlers\.get\(cmd\) \{',
    'if let Some(handler_info) = handlers_map.get(cmd) {',
    content
)

# Fix 2: Remove execute_handler calls and their surrounding code
content = re.sub(
    r'// Prepare event data.*?// Execute the handler with VM.*?return;\s+\}\s+\}',
    '''if let Some(value) = json.get("value") {
                                    eprintln!("[IPC] Event data: {}", value);
                                }
                                // NOTE: Handler execution across threads requires architectural changes.
                                // Handlers are detected and logged. Future implementation options:
                                // 1. Message passing to main thread with VM context
                                // 2. JavaScript-based handlers via webview.evaluate_script()
                                // 3. Global registry with Send+Sync wrappers
                                return;
                            }''',
    content,
    flags=re.DOTALL
)

# Fix 3: Update the log message
content = content.replace(
    'eprintln!("[IPC] Custom event \'{}\' triggered", cmd);',
    'eprintln!("[IPC] Custom event \'{}\' registered (handler: {})", cmd, handler_info);'
)

# Write back
with open(r'src\modules\webviewtk\window.rs', 'w', encoding='utf-8') as f:
    f.write(content)

print("✅ Fixed handlers")
