```python run id=executor
import asyncio
from concurrent.futures import ThreadPoolExecutor


def square(n):
    return n * n


with ThreadPoolExecutor(max_workers=2) as pool:
    futures = [pool.submit(square, n) for n in range(4)]
    print([f.result() for f in futures])


async def main():
    loop = asyncio.get_running_loop()
    future = loop.create_future()
    loop.call_soon(future.set_result, "ready")
    print(future.done())
    print(await future, future.done())
    results = await asyncio.gather(asyncio.sleep(0, "a"), asyncio.sleep(0, "b"))
    print(results)


asyncio.run(main())
```

```output for=executor
[0, 1, 4, 9]
False
ready True
['a', 'b']
```

- Verdict: stdlib. `concurrent.futures` runs a callable in a pool of threads or processes, and `submit` returns a `Future` at once. `result()` waits for the value or raises the exception of the call.
- `asyncio.Future` is the future of the event loop. A coroutine waits for it with `await`. Code that sets the result plays the role of the promise.
- `asyncio.gather` waits for many awaitables and returns their results in the order of the arguments.

```python wrong id=lost-error
from concurrent.futures import ThreadPoolExecutor


def save(record):
    raise ValueError(f"bad record {record}")


with ThreadPoolExecutor() as pool:
    pool.submit(save, 1)
print("all saved")
```

```output for=lost-error
all saved
```

The exception is stored in the future, and nothing reads the future. The program reports success. Keep the futures and call `result()` on each one, or use `pool.map`, which raises the first exception.
