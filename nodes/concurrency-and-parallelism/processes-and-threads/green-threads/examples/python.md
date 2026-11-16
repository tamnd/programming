```python run id=asyncio-turns
import asyncio
import threading


async def worker(name, steps):
    for i in range(steps):
        print(f"{name} step {i} on {threading.current_thread().name}")
        await asyncio.sleep(0)  # give the event loop a chance to run another task


async def main():
    await asyncio.gather(worker("a", 3), worker("b", 2))


asyncio.run(main())
```

```output for=asyncio-turns
a step 0 on MainThread
b step 0 on MainThread
a step 1 on MainThread
b step 1 on MainThread
a step 2 on MainThread
```

- Verdict: library. CPython has no green threads in the standard library. The libraries `greenlet`, `eventlet` and `gevent` add them.
- The standard library has the stackless form: generators, as in the Idea section, and `asyncio` coroutines, as here. Each `await` is a point where the task can give the thread back.
- `asyncio.sleep(0)` yields to the event loop without a delay. So the two tasks take turns, and all their steps run on `MainThread`.
- A coroutine that calls a blocking function, for example `time.sleep`, stops every task on the event loop until the call returns.
