```python run id=counter
import threading

counter = 0
counter_lock = threading.Lock()


def add_many(times):
    global counter
    for _ in range(times):
        with counter_lock:
            counter += 1


workers = [threading.Thread(target=add_many, args=(100_000,)) for _ in range(4)]
for worker in workers:
    worker.start()
for worker in workers:
    worker.join()
print("total:", counter)
```

```output
total: 400000
```

- Verdict: stdlib. The module `threading` gives `Lock` and `RLock`. The statement `with counter_lock:` acquires the lock and releases it at the end of the block, also when the block raises an exception.
- A `Lock` has no owner, so any thread may release it. An `RLock` has an owner and a recursion level, and only the owner may release it.
- `acquire(timeout=...)` returns `False` when the time runs out. Use it to stop a program that would otherwise wait forever.
- The statement `counter += 1` reads, adds and writes in separate steps, so threads can lose updates without the lock. The global interpreter lock does not make the statement atomic.
