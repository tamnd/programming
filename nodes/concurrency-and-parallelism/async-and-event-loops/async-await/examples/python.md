```python run id=load
import asyncio


async def load(name, delay):
    print("start", name)
    await asyncio.sleep(delay)
    print("end", name)
    return name


async def main():
    results = await asyncio.gather(load("slow", 0.2), load("fast", 0.1))
    print(results)


asyncio.run(main())
```

```output for=load
start slow
start fast
end fast
end slow
['slow', 'fast']
```

- Verdict: native. `async def` makes a coroutine function, and `await` suspends it.
- The `asyncio` package of the standard library gives the event loop, tasks, timers and network streams. `asyncio.run` starts the loop and runs one coroutine to its end.
- Calling a coroutine function does not run its body. The coroutine runs when it is awaited, or when `asyncio.create_task` or `asyncio.gather` schedules it.
- `asyncio.TaskGroup` starts a group of tasks and waits for all of them. It cancels the others when one of them fails.
