# Concurrency

Tauraro provides multiple approaches to concurrent programming: async/await, multiprocessing, and threading.

## Async/Await (Recommended)

For I/O-bound tasks, use async/await with the `asyncio` module.

### Basic Async

```python
import asyncio

async def fetch_data(url: str):
    # Async I/O operation
    await asyncio.sleep(1)
    return f"Data from {url}"

async def main():
    # Run tasks concurrently
    results = await asyncio.gather(
        fetch_data("url1"),
        fetch_data("url2"),
        fetch_data("url3")
    )
    print(results)

asyncio.run(main())
```

### Concurrent HTTP Requests

```python
import asyncio
import httpx

async def fetch_all(urls):
    async def fetch(url):
        response = await httpx.get(url)
        return response.json()

    # Fetch all URLs concurrently
    tasks = [fetch(url) for url in urls]
    return await asyncio.gather(*tasks)

urls = ["https://api.example.com/1", "https://api.example.com/2"]
results = asyncio.run(fetch_all(urls))
```

### WebSocket Concurrency

```python
import asyncio
import websockets

async def handle_client(websocket):
    # Handle each client concurrently
    async for message in websocket:
        response = process_message(message)
        await websocket.send(response)

async def main():
    # Server handles multiple clients concurrently
    server = await websockets.serve(handle_client, "localhost", 8080)
    await server.wait_closed()

asyncio.run(main())
```

## Multiprocessing

For CPU-bound tasks, use the `multiprocessing` module.

### Parallel Processing

```python
import multiprocessing

def cpu_intensive(n: int) -> int:
    """CPU-intensive computation."""
    result = 0
    for i in range(n):
        result += i * i
    return result

# Process in parallel
with multiprocessing.Pool() as pool:
    results = pool.map(cpu_intensive, [1000000, 2000000, 3000000])
    print(results)
```

### Process Pool Example

```python
import multiprocessing

def process_file(filename: str) -> dict:
    """Process a single file."""
    # CPU-intensive processing
    data = load_file(filename)
    return analyze(data)

def process_directory(directory: str):
    """Process all files in parallel."""
    files = list_files(directory)

    with multiprocessing.Pool(processes=4) as pool:
        results = pool.map(process_file, files)

    return results
```

## Task Distribution Patterns

### Fan-Out/Fan-In

```python
import asyncio

async def process_item(item):
    # Process individual item
    await asyncio.sleep(0.1)
    return item * 2

async def fan_out_fan_in(items):
    # Fan out: start all tasks
    tasks = [process_item(item) for item in items]

    # Fan in: collect all results
    results = await asyncio.gather(*tasks)

    return results

items = list(range(10))
results = asyncio.run(fan_out_fan_in(items))
```

### Pipeline Pattern

```python
import asyncio

async def stage1(item):
    await asyncio.sleep(0.1)
    return item * 2

async def stage2(item):
    await asyncio.sleep(0.1)
    return item + 10

async def stage3(item):
    await asyncio.sleep(0.1)
    return item ** 2

async def pipeline(items):
    results = []
    for item in items:
        result = await stage1(item)
        result = await stage2(result)
        result = await stage3(result)
        results.append(result)
    return results
```

### Worker Pool Pattern

```python
import asyncio

class WorkerPool:
    def __init__(self, num_workers: int):
        self.queue = asyncio.Queue()
        self.workers = []
        self.num_workers = num_workers

    async def worker(self, worker_id: int):
        while True:
            task = await self.queue.get()
            if task is None:
                break

            result = await self.process_task(task)
            print(f"Worker {worker_id}: {result}")

            self.queue.task_done()

    async def process_task(self, task):
        # Simulate work
        await asyncio.sleep(0.1)
        return task * 2

    async def run(self, tasks):
        # Start workers
        self.workers = [
            asyncio.create_task(self.worker(i))
            for i in range(self.num_workers)
        ]

        # Add tasks to queue
        for task in tasks:
            await self.queue.put(task)

        # Wait for all tasks to complete
        await self.queue.join()

        # Stop workers
        for _ in range(self.num_workers):
            await self.queue.put(None)

        await asyncio.gather(*self.workers)
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
        temp = counter
        await asyncio.sleep(0.01)
        counter = temp + 1

async def main():
    tasks = [increment() for _ in range(10)]
    await asyncio.gather(*tasks)
    print(f"Counter: {counter}")

asyncio.run(main())
```

### Semaphore

```python
import asyncio

# Limit to 3 concurrent operations
semaphore = asyncio.Semaphore(3)

async def limited_operation(num: int):
    async with semaphore:
        print(f"Running {num}")
        await asyncio.sleep(1)
        print(f"Done {num}")

async def main():
    tasks = [limited_operation(i) for i in range(10)]
    await asyncio.gather(*tasks)

asyncio.run(main())
```

### Event

```python
import asyncio

event = asyncio.Event()

async def waiter(name: str):
    print(f"{name} waiting...")
    await event.wait()
    print(f"{name} received event!")

async def setter():
    await asyncio.sleep(2)
    print("Setting event")
    event.set()

async def main():
    await asyncio.gather(
        waiter("Worker 1"),
        waiter("Worker 2"),
        waiter("Worker 3"),
        setter()
    )

asyncio.run(main())
```

## Rate Limiting

### Token Bucket

```python
import asyncio
import time

class RateLimiter:
    def __init__(self, rate: float, capacity: int):
        self.rate = rate  # tokens per second
        self.capacity = capacity
        self.tokens = capacity
        self.last_update = time.time()
        self.lock = asyncio.Lock()

    async def acquire(self):
        async with self.lock:
            now = time.time()
            elapsed = now - self.last_update
            self.tokens = min(self.capacity, self.tokens + elapsed * self.rate)
            self.last_update = now

            if self.tokens >= 1:
                self.tokens -= 1
                return

            # Wait for token
            wait_time = (1 - self.tokens) / self.rate
            await asyncio.sleep(wait_time)
            self.tokens = 0

# Usage
limiter = RateLimiter(rate=10, capacity=10)  # 10 requests/sec

async def make_request(url: str):
    await limiter.acquire()
    # Make actual request
    print(f"Request to {url}")

async def main():
    tasks = [make_request(f"url{i}") for i in range(20)]
    await asyncio.gather(*tasks)
```

## Error Handling in Concurrent Code

### Handling Task Exceptions

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
        return_exceptions=True
    )

    for i, result in enumerate(results):
        if isinstance(result, Exception):
            print(f"Task {i} failed: {result}")
        else:
            print(f"Task {i} succeeded: {result}")

asyncio.run(main())
```

## Performance Comparison

| Task Type | Best Approach | Speedup |
|-----------|---------------|---------|
| I/O-bound (HTTP, DB) | asyncio | 10-100x |
| CPU-bound (computation) | multiprocessing | 2-8x (# of CPUs) |
| Mixed workload | asyncio + multiprocessing | Varies |

## Best Practices

### 1. Choose the Right Tool

```python
# I/O-bound: Use asyncio
async def fetch_many_urls(urls):
    tasks = [httpx.get(url) for url in urls]
    return await asyncio.gather(*tasks)

# CPU-bound: Use multiprocessing
def compute_many(items):
    with multiprocessing.Pool() as pool:
        return pool.map(expensive_compute, items)
```

### 2. Set Timeouts

```python
# Always set timeouts for async operations
try:
    result = await asyncio.wait_for(slow_operation(), timeout=5.0)
except asyncio.TimeoutError:
    print("Operation timed out")
```

### 3. Limit Concurrency

```python
# Don't overwhelm resources
semaphore = asyncio.Semaphore(10)  # Max 10 concurrent

async def limited_task():
    async with semaphore:
        await perform_operation()
```

### 4. Handle Errors Gracefully

```python
# Use return_exceptions to handle errors without stopping other tasks
results = await asyncio.gather(*tasks, return_exceptions=True)
```

### 5. Clean Up Resources

```python
# Use context managers
async with httpx.AsyncClient() as client:
    response = await client.get(url)

# Or try/finally
task = asyncio.create_task(operation())
try:
    await task
finally:
    task.cancel()
```

## Complete Example: Concurrent Web Scraper

```python
import asyncio
import httpx
from typing import List

class WebScraper:
    def __init__(self, max_concurrent: int = 10):
        self.semaphore = asyncio.Semaphore(max_concurrent)
        self.client = None

    async def __aenter__(self):
        self.client = httpx.AsyncClient()
        return self

    async def __aexit__(self, *args):
        await self.client.aclose()

    async def fetch(self, url: str) -> str:
        async with self.semaphore:
            try:
                response = await self.client.get(url, timeout=10.0)
                return response.text
            except Exception as e:
                print(f"Error fetching {url}: {e}")
                return ""

    async def scrape_all(self, urls: List[str]) -> List[str]:
        tasks = [self.fetch(url) for url in urls]
        return await asyncio.gather(*tasks)

# Usage
async def main():
    urls = [f"https://example.com/page{i}" for i in range(100)]

    async with WebScraper(max_concurrent=10) as scraper:
        results = await scraper.scrape_all(urls)
        print(f"Scraped {len(results)} pages")

asyncio.run(main())
```

## Next Steps

- [Asyncio Module](../stdlib/asyncio.md) - Async programming guide
- [Multiprocessing](../stdlib/subprocess.md#multiprocessing) - Process parallelism
- [HTTP Modules](../stdlib/http.md) - Concurrent HTTP
- [Performance](performance.md) - Optimization techniques
