```python run id=sums
import sys
import threading


def partial_sum(results, index, start, stop):
    results[index] = sum(range(start, stop))


results = [0] * 4
workers = [
    threading.Thread(target=partial_sum, args=(results, i, i * 250, (i + 1) * 250))
    for i in range(4)
]
for worker in workers:
    worker.start()
for worker in workers:
    worker.join()

print(results)
print("total:", sum(results))
print("daemon:", workers[0].daemon)
print("GIL enabled:", sys._is_gil_enabled())
```

```output for=sums
[31125, 93625, 156125, 218625]
total: 499500
daemon: False
GIL enabled: True
```

- Verdict: stdlib. `threading.Thread` runs a function on an OS thread. `join()` waits for it to end.
- A thread cannot return a value through `join()`, which returns `None`. Each thread writes its result to its own slot of a list. `concurrent.futures.ThreadPoolExecutor` returns results through futures instead.
- In the default build of CPython, the global interpreter lock lets one thread at a time run Python bytecode. The free-threaded build has no GIL, and there `sys._is_gil_enabled()` returns `False`.
- A thread is not a daemon unless you pass `daemon=True`. The interpreter waits for all non-daemon threads before it exits.
