# Subprocess and Multiprocessing

Tauraro includes modules for process management, allowing you to run external commands and create parallel processes.

## subprocess - Running External Commands

The `subprocess` module lets you spawn new processes, connect to their input/output/error pipes, and obtain their return codes.

### Basic Usage

#### Running Simple Commands

```python
import subprocess

# Run a command
result = subprocess.run("ls -la")
print(f"Exit code: {result['returncode']}")

# Run on Windows
result = subprocess.run("dir")
```

#### Capturing Output

```python
import subprocess

# Capture standard output
output = subprocess.check_output("pwd")
print(f"Current directory: {output}")

# Capture both stdout and stderr
result = subprocess.run("ls -la", capture_output=True)
print(result['stdout'])
print(result['stderr'])
```

### subprocess.run()

Execute a command and wait for it to complete:

```python
import subprocess

# Basic usage
result = subprocess.run("echo Hello World")

# With arguments as list
result = subprocess.run(["python", "-c", "print('Hello')"])

# Access return code
if result['returncode'] == 0:
    print("Command succeeded")
else:
    print("Command failed")
```

### subprocess.check_output()

Run command and return its output:

```python
import subprocess

# Get command output as string
output = subprocess.check_output("date")
print(output)

# Get output from command with arguments
output = subprocess.check_output(["python", "--version"])
print(output)

# Handle errors
try:
    output = subprocess.check_output("non_existent_command")
except subprocess.CalledProcessError as e:
    print(f"Command failed with code {e.returncode}")
```

### Capturing Output

```python
import subprocess

# Capture both stdout and stderr
result = subprocess.run(
    "python script.py",
    capture_output=True
)

print("STDOUT:", result['stdout'])
print("STDERR:", result['stderr'])
print("Return code:", result['returncode'])
```

### Working with Shell Commands

```python
import subprocess

# Execute shell command
result = subprocess.run("echo $HOME", shell=True)

# Pipe commands
result = subprocess.run("ls -la | grep py", shell=True)

# Multiple commands
result = subprocess.run("cd /tmp && ls", shell=True)
```

### Environment Variables

```python
import subprocess

# Pass custom environment
env = {
    "PATH": "/usr/bin:/bin",
    "CUSTOM_VAR": "value"
}

result = subprocess.run("echo $CUSTOM_VAR", shell=True, env=env)
```

### Working Directory

```python
import subprocess

# Run command in specific directory
result = subprocess.run("ls", cwd="/tmp")

# Equivalent to: cd /tmp && ls
```

### Input to Process

```python
import subprocess

# Send input to process
result = subprocess.run(
    "python -c 'print(input())'",
    input=b"Hello World",
    capture_output=True
)

print(result['stdout'])  # "Hello World"
```

### Timeouts

```python
import subprocess

try:
    # Set timeout (in seconds)
    result = subprocess.run("sleep 10", timeout=5)
except subprocess.TimeoutExpired:
    print("Command timed out")
```

### Real-World Examples

#### Running Python Scripts

```python
import subprocess

def run_python_script(script_path: str, *args):
    """Run a Python script with arguments."""
    cmd = ["python", script_path] + list(args)

    result = subprocess.run(cmd, capture_output=True)

    if result['returncode'] == 0:
        return result['stdout']
    else:
        raise RuntimeError(f"Script failed: {result['stderr']}")

# Usage
output = run_python_script("process_data.py", "input.csv", "output.csv")
print(output)
```

#### Git Commands

```python
import subprocess

def git_status():
    """Get git repository status."""
    output = subprocess.check_output("git status")
    return output

def git_commit(message: str):
    """Create git commit."""
    result = subprocess.run(f'git commit -m "{message}"')
    return result['returncode'] == 0

# Usage
print(git_status())
success = git_commit("Add new feature")
```

#### System Information

```python
import subprocess
import sys

def get_system_info():
    """Get system information."""
    info = {}

    if sys.platform == "linux":
        info['os'] = subprocess.check_output("uname -s")
        info['kernel'] = subprocess.check_output("uname -r")
        info['cpu'] = subprocess.check_output("lscpu | grep 'Model name'")
    elif sys.platform == "darwin":  # macOS
        info['os'] = subprocess.check_output("sw_vers -productName")
        info['version'] = subprocess.check_output("sw_vers -productVersion")
    elif sys.platform == "win32":
        info['os'] = subprocess.check_output("ver")

    return info

print(get_system_info())
```

#### Database Backups

```python
import subprocess
from datetime import datetime

def backup_database(db_name: str, output_dir: str):
    """Backup PostgreSQL database."""
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    backup_file = f"{output_dir}/{db_name}_{timestamp}.sql"

    cmd = f"pg_dump {db_name} > {backup_file}"

    result = subprocess.run(cmd, shell=True)

    if result['returncode'] == 0:
        print(f"Backup saved to {backup_file}")
        return backup_file
    else:
        raise RuntimeError("Backup failed")
```

## multiprocessing - Process-Based Parallelism

The `multiprocessing` module provides process-based parallelism. (Currently implemented as thread-based in Tauraro.)

### Getting CPU Count

```python
import multiprocessing

# Get number of CPUs
cpu_count = multiprocessing.cpu_count()
print(f"Number of CPUs: {cpu_count}")

# Use this to determine worker pool size
workers = cpu_count * 2
```

### Process Pools

```python
import multiprocessing

def process_item(item):
    """Process a single item."""
    return item * 2

# Create pool of workers
with multiprocessing.Pool(processes=4) as pool:
    items = [1, 2, 3, 4, 5, 6, 7, 8]

    # Map function across items
    results = pool.map(process_item, items)

    print(results)  # [2, 4, 6, 8, 10, 12, 14, 16]
```

### Parallel Processing Example

```python
import multiprocessing

def expensive_computation(n: int) -> int:
    """Simulate expensive computation."""
    result = 0
    for i in range(n):
        result += i * i
    return result

# Sequential processing
def sequential_process(items):
    return [expensive_computation(item) for item in items]

# Parallel processing
def parallel_process(items):
    with multiprocessing.Pool() as pool:
        return pool.map(expensive_computation, items)

# Usage
items = [1000000, 2000000, 3000000, 4000000]

# This is much faster with multiprocessing!
results = parallel_process(items)
print(results)
```

### CPU-Bound vs I/O-Bound

```python
import multiprocessing
import asyncio

# CPU-bound: Use multiprocessing
def cpu_intensive_task(data):
    # Heavy computation
    result = sum(i**2 for i in range(data))
    return result

with multiprocessing.Pool() as pool:
    results = pool.map(cpu_intensive_task, [1000000, 2000000])

# I/O-bound: Use asyncio
async def io_intensive_task(url):
    import httpx
    response = await httpx.get(url)
    return response

async def main():
    urls = ["https://example.com"] * 10
    results = await asyncio.gather(*[io_intensive_task(url) for url in urls])

asyncio.run(main())
```

## Error Handling

### Handling Command Failures

```python
import subprocess

def run_command_safely(cmd: str):
    """Run command with error handling."""
    try:
        result = subprocess.run(cmd, capture_output=True, timeout=30)

        if result['returncode'] != 0:
            print(f"Command failed: {result['stderr']}")
            return None

        return result['stdout']

    except subprocess.TimeoutExpired:
        print("Command timed out")
        return None
    except subprocess.CalledProcessError as e:
        print(f"Error running command: {e}")
        return None
    except Exception as e:
        print(f"Unexpected error: {e}")
        return None

# Usage
output = run_command_safely("ls -la")
if output:
    print(output)
```

## Best Practices

### 1. Use Lists for Commands (Safer)

```python
# Good - no shell injection risk
subprocess.run(["ls", "-la", directory])

# Risky - shell injection possible
subprocess.run(f"ls -la {directory}", shell=True)
```

### 2. Set Timeouts

```python
# Prevent hanging forever
result = subprocess.run("long_command", timeout=60)
```

### 3. Capture Output Explicitly

```python
# Explicit is better
result = subprocess.run("command", capture_output=True)
stdout = result['stdout']
stderr = result['stderr']
```

### 4. Check Return Codes

```python
result = subprocess.run("command")
if result['returncode'] != 0:
    # Handle error
    print("Command failed")
```

### 5. Use Appropriate Tool

```python
# For CPU-bound tasks
use multiprocessing

# For I/O-bound tasks
use asyncio

# For running external commands
use subprocess
```

## Performance Considerations

| Task Type | Best Tool | Why |
|-----------|-----------|-----|
| CPU-intensive computation | `multiprocessing` | Utilizes multiple cores |
| I/O-bound operations | `asyncio` | Non-blocking I/O |
| Running external commands | `subprocess` | Process management |
| Simple scripts | `subprocess.run()` | Easy and straightforward |

## Complete Example: Parallel File Processing

```python
import subprocess
import multiprocessing
import os

def process_file(filename: str) -> dict:
    """Process a single file."""
    # Get file size
    size_output = subprocess.check_output(f"wc -c {filename}")
    size = int(size_output.split()[0])

    # Count lines
    lines_output = subprocess.check_output(f"wc -l {filename}")
    lines = int(lines_output.split()[0])

    return {
        'filename': filename,
        'size': size,
        'lines': lines
    }

def process_directory(directory: str):
    """Process all files in directory in parallel."""
    # Get all files
    files = [
        os.path.join(directory, f)
        for f in os.listdir(directory)
        if os.path.isfile(os.path.join(directory, f))
    ]

    # Process in parallel
    with multiprocessing.Pool() as pool:
        results = pool.map(process_file, files)

    return results

# Usage
results = process_directory("/path/to/files")
for result in results:
    print(f"{result['filename']}: {result['lines']} lines, {result['size']} bytes")
```

## Next Steps

- [Asyncio](asyncio.md) - Asynchronous I/O
- [HTTP Modules](http.md) - Network requests
- [System Module](sys.md) - System parameters
- [OS Module](#) - Operating system interface
- [Concurrency](../advanced/concurrency.md) - Advanced patterns
