# Asyncio - Asynchronous Programming

Tauraro includes full async/await support powered by Tokio, Rust's async runtime. Write concurrent code that's both fast and easy to understand.

## Overview

The `asyncio` module provides:
- **async/await syntax** - Write asynchronous code naturally
- **Tokio runtime** - High-performance async execution
- **Coroutines** - Lightweight concurrent tasks
- **Event loop** - Efficient I/O multiplexing

## Basic Usage

### Simple Async Function

```python
import asyncio

async def say_hello():
    print("Hello")
    await asyncio.sleep(1)  # Async sleep (non-blocking)
    print("World")

# Run the async function
asyncio.run(say_hello())
```

Output:
```
Hello
(1 second delay)
World
```

### Async/Await Syntax

```python
import asyncio

async def fetch_data():
    """Simulate fetching data."""
    print("Fetching data...")
    await asyncio.sleep(2)  # Simulate I/O operation
    return {"result": "data loaded"}

async def main():
    data = await fetch_data()
    print(f"Got: {data}")

asyncio.run(main())
```

## Core Functions

### asyncio.run()

Run an async function as the main entry point:

```python
import asyncio

async def main():
    print("Running async code")
    await asyncio.sleep(1)

# Start the event loop and run main()
asyncio.run(main())
```

### asyncio.sleep()

Asynchronous sleep (non-blocking):

```python
import asyncio

async def delayed_print(delay, message):
    await asyncio.sleep(delay)
    print(message)

asyncio.run(delayed_print(2, "Hello after 2 seconds"))
```

### asyncio.gather()

Run multiple coroutines concurrently:

```python
import asyncio

async def task_1():
    await asyncio.sleep(1)
    return "Task 1 complete"

async def task_2():
    await asyncio.sleep(2)
    return "Task 2 complete"

async def task_3():
    await asyncio.sleep(1.5)
    return "Task 3 complete"

async def main():
    # Run all tasks concurrently
    results = await asyncio.gather(task_1(), task_2(), task_3())
    print(results)

asyncio.run(main())
```

Output:
```
['Task 1 complete', 'Task 2 complete', 'Task 3 complete']
```

### asyncio.create_task()

Create a task that runs in the background:

```python
import asyncio

async def background_task():
    while True:
        print("Background task running...")
        await asyncio.sleep(1)

async def main():
    # Start background task
    task = asyncio.create_task(background_task())

    # Do other work
    await asyncio.sleep(3)

    # Cancel background task
    task.cancel()

asyncio.run(main())
```

## Concurrent Execution

### Running Multiple Tasks

```python
import asyncio

async def download_file(url: str, filename: str):
    print(f"Downloading {url}...")
    await asyncio.sleep(2)  # Simulate download
    print(f"Saved to {filename}")
    return filename

async def main():
    # Run downloads concurrently
    tasks = [
        download_file("https://example.com/file1", "file1.dat"),
        download_file("https://example.com/file2", "file2.dat"),
        download_file("https://example.com/file3", "file3.dat"),
    ]

    results = await asyncio.gather(*tasks)
    print(f"Downloaded: {results}")

asyncio.run(main())
```

### Task Cancellation

```python
import asyncio

async def long_running_task():
    try:
        for i in range(10):
            print(f"Working... {i}")
            await asyncio.sleep(1)
    except asyncio.CancelledError:
        print("Task was cancelled")
        raise

async def main():
    task = asyncio.create_task(long_running_task())

    # Let it run for 3 seconds
    await asyncio.sleep(3)

    # Cancel the task
    task.cancel()

    try:
        await task
    except asyncio.CancelledError:
        print("Task cancelled successfully")

asyncio.run(main())
```

## Timeouts

### asyncio.wait_for()

Set timeout for async operations:

```python
import asyncio

async def slow_operation():
    await asyncio.sleep(10)  # Very slow
    return "Done"

async def main():
    try:
        result = await asyncio.wait_for(slow_operation(), timeout=3.0)
        print(result)
    except asyncio.TimeoutError:
        print("Operation timed out after 3 seconds")

asyncio.run(main())
```

### Timeout Context Manager

```python
import asyncio

async def main():
    try:
        async with asyncio.timeout(5.0):
            await asyncio.sleep(10)
    except asyncio.TimeoutError:
        print("Timed out!")

asyncio.run(main())
```

## Async Iterators

### Async For Loops

```python
import asyncio

class AsyncCounter:
    def __init__(self, limit):
        self.current = 0
        self.limit = limit

    def __aiter__(self):
        return self

    async def __anext__(self):
        if self.current >= self.limit:
            raise StopAsyncIteration

        await asyncio.sleep(0.1)
        self.current += 1
        return self.current

async def main():
    async for number in AsyncCounter(5):
        print(number)

asyncio.run(main())
```

Output:
```
1
2
3
4
5
```

## Async Context Managers

```python
import asyncio

class AsyncResource:
    async def __aenter__(self):
        print("Acquiring resource...")
        await asyncio.sleep(0.5)
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        print("Releasing resource...")
        await asyncio.sleep(0.5)

    async def use(self):
        print("Using resource")

async def main():
    async with AsyncResource() as resource:
        await resource.use()

asyncio.run(main())
```

## Synchronization Primitives

### Lock

```python
import asyncio

lock = asyncio.Lock()
counter = 0

async def increment():
    global counter
    async with lock:
        # Critical section
        temp = counter
        await asyncio.sleep(0.01)  # Simulate work
        counter = temp + 1

async def main():
    tasks = [increment() for _ in range(10)]
    await asyncio.gather(*tasks)
    print(f"Counter: {counter}")  # Should be 10

asyncio.run(main())
```

### Event

```python
import asyncio

event = asyncio.Event()

async def waiter():
    print("Waiting for event...")
    await event.wait()
    print("Event occurred!")

async def setter():
    await asyncio.sleep(2)
    print("Setting event")
    event.set()

async def main():
    await asyncio.gather(waiter(), setter())

asyncio.run(main())
```

### Semaphore

```python
import asyncio

semaphore = asyncio.Semaphore(3)  # Allow 3 concurrent operations

async def limited_operation(num):
    async with semaphore:
        print(f"Operation {num} started")
        await asyncio.sleep(1)
        print(f"Operation {num} finished")

async def main():
    tasks = [limited_operation(i) for i in range(10)]
    await asyncio.gather(*tasks)

asyncio.run(main())
```

## Queues

### Async Queue

```python
import asyncio

async def producer(queue, num_items):
    for i in range(num_items):
        await asyncio.sleep(0.1)
        await queue.put(f"Item {i}")
        print(f"Produced: Item {i}")

    await queue.put(None)  # Signal completion

async def consumer(queue):
    while True:
        item = await queue.get()
        if item is None:
            break

        print(f"Consumed: {item}")
        await asyncio.sleep(0.2)

async def main():
    queue = asyncio.Queue()

    # Run producer and consumer concurrently
    await asyncio.gather(
        producer(queue, 5),
        consumer(queue)
    )

asyncio.run(main())
```

## Real-World Examples

### Concurrent HTTP Requests

```python
import asyncio
import httpx

async def fetch_url(url: str):
    """Fetch URL asynchronously."""
    print(f"Fetching {url}...")
    response = await httpx.get(url)
    print(f"Got {url}: {response.status_code}")
    return response

async def main():
    urls = [
        "https://api.github.com",
        "https://api.github.com/users",
        "https://api.github.com/repos",
    ]

    # Fetch all URLs concurrently
    responses = await asyncio.gather(*[fetch_url(url) for url in urls])

    print(f"Fetched {len(responses)} URLs")

asyncio.run(main())
```

### WebSocket Chat Client

```python
import asyncio
import websockets

async def chat_client():
    uri = "ws://localhost:8080"

    async with websockets.connect(uri) as websocket:
        # Send messages
        send_task = asyncio.create_task(send_messages(websocket))

        # Receive messages
        receive_task = asyncio.create_task(receive_messages(websocket))

        # Wait for both tasks
        await asyncio.gather(send_task, receive_task)

async def send_messages(websocket):
    messages = ["Hello", "How are you?", "Goodbye"]
    for msg in messages:
        await websocket.send(msg)
        await asyncio.sleep(1)

async def receive_messages(websocket):
    async for message in websocket:
        print(f"Received: {message}")

asyncio.run(chat_client())
```

### Periodic Task

```python
import asyncio
from datetime import datetime

async def periodic_task(interval: float, task_name: str):
    """Run a task periodically."""
    while True:
        print(f"{task_name}: {datetime.now()}")
        await asyncio.sleep(interval)

async def main():
    # Start multiple periodic tasks
    await asyncio.gather(
        periodic_task(1.0, "Task A"),
        periodic_task(2.0, "Task B"),
        periodic_task(3.0, "Task C"),
    )

asyncio.run(main())
```

### Rate-Limited API Client

```python
import asyncio
import httpx

class RateLimitedClient:
    def __init__(self, rate_limit: int):
        self.semaphore = asyncio.Semaphore(rate_limit)

    async def fetch(self, url: str):
        async with self.semaphore:
            response = await httpx.get(url)
            await asyncio.sleep(1)  # Rate limiting
            return response

async def main():
    client = RateLimitedClient(rate_limit=3)  # Max 3 concurrent requests

    urls = [f"https://api.example.com/item/{i}" for i in range(10)]
    tasks = [client.fetch(url) for url in urls]

    responses = await asyncio.gather(*tasks)
    print(f"Fetched {len(responses)} items")

asyncio.run(main())
```

## Error Handling

### Try/Except with Async

```python
import asyncio

async def risky_operation():
    await asyncio.sleep(1)
    raise ValueError("Something went wrong")

async def main():
    try:
        await risky_operation()
    except ValueError as e:
        print(f"Caught error: {e}")

asyncio.run(main())
```

### Handling Multiple Task Errors

```python
import asyncio

async def task_that_fails():
    await asyncio.sleep(1)
    raise ValueError("Task failed")

async def task_that_succeeds():
    await asyncio.sleep(1)
    return "Success"

async def main():
    results = await asyncio.gather(
        task_that_succeeds(),
        task_that_fails(),
        task_that_succeeds(),
        return_exceptions=True  # Return exceptions instead of raising
    )

    for i, result in enumerate(results):
        if isinstance(result, Exception):
            print(f"Task {i} failed: {result}")
        else:
            print(f"Task {i} succeeded: {result}")

asyncio.run(main())
```

## Performance Tips

### 1. Use gather() for Concurrent Operations

```python
# Good - concurrent execution
results = await asyncio.gather(task1(), task2(), task3())

# Bad - sequential execution
result1 = await task1()
result2 = await task2()
result3 = await task3()
```

### 2. Don't Block the Event Loop

```python
# Bad - blocks event loop
def blocking_io():
    time.sleep(5)  # Blocks!

async def bad():
    blocking_io()  # Don't do this!

# Good - use asyncio.sleep()
async def good():
    await asyncio.sleep(5)  # Non-blocking
```

### 3. Use Semaphores for Rate Limiting

```python
# Limit concurrent operations
semaphore = asyncio.Semaphore(10)

async def limited_task():
    async with semaphore:
        await perform_operation()
```

## Best Practices

1. **Always await coroutines** - Don't forget to use `await`
2. **Use asyncio.run() for entry point** - Clean event loop management
3. **Handle cancellation** - Catch `CancelledError` for cleanup
4. **Set timeouts** - Prevent hanging operations
5. **Use gather() for parallelism** - Run tasks concurrently
6. **Don't block the event loop** - Use async I/O operations

## Next Steps

- [HTTP Modules](http.md) - Async HTTP requests
- [WebSockets](http.md#websockets) - Real-time communication
- [Process Management](subprocess.md) - Running external processes
- [Concurrency](../advanced/concurrency.md) - Advanced patterns
