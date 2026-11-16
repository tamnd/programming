```python run id=asyncio-limit
import asyncio

TASKS = 1_000


async def main():
    permits = asyncio.Semaphore(10)
    active = 0
    peak = 0

    async def work(n):
        nonlocal active, peak
        async with permits:
            active += 1
            peak = max(peak, active)
            await asyncio.sleep(0.01)  # the task gives the event loop back here
            active -= 1
            return n

    results = await asyncio.gather(*(work(n) for n in range(TASKS)))
    print(f"tasks: {len(results)}, sum: {sum(results)}")
    print(f"never more than 10 at once: {peak <= 10}")


asyncio.run(main())
```

```output for=asyncio-limit
tasks: 1000, sum: 499500
never more than 10 at once: True
```

- Verdict: absent. Python has no virtual threads. `asyncio` tasks are the alternative: many tasks share one thread, and each `await` is a point where a task can wait.
- `asyncio.Semaphore(10)` lets at most ten tasks into the `async with` block at the same time.
- All tasks run on one thread, so `active` and `peak` need no lock.
- A blocking call, for example `time.sleep`, inside a task stops every task on the event loop.
