```python run id=per-thread-values
import threading
from concurrent.futures import ThreadPoolExecutor

state = threading.local()


def bump(times):
    # Each thread sees its own state.counter. A new thread starts without one.
    for _ in range(times):
        state.counter = getattr(state, "counter", 0) + 1
    return state.counter


results = {}


def worker(name, times):
    results[name] = bump(times)


threads = [threading.Thread(target=worker, args=(f"thread {i}", i)) for i in (1, 2)]
for t in threads:
    t.start()
for t in threads:
    t.join()
results["main"] = bump(3)
for name in ("main", "thread 1", "thread 2"):
    print(f"{name}: counter = {results[name]}")

# A pool reuses its threads, so a value that one task leaves behind is seen by the next task.
with ThreadPoolExecutor(max_workers=1) as pool:
    pool.submit(setattr, state, "user", "alice").result()
    print("a later task on the same thread sees user:", pool.submit(getattr, state, "user", None).result())
    pool.submit(delattr, state, "user").result()
    print("after a task removes it, the next task sees user:", pool.submit(getattr, state, "user", None).result())
```

```output for=per-thread-values
main: counter = 3
thread 1: counter = 1
thread 2: counter = 2
a later task on the same thread sees user: alice
after a task removes it, the next task sees user: None
```

- Verdict: stdlib. `threading.local()` returns an object whose attributes are different in each thread. A new thread sees none of the attributes that other threads set.
- `getattr(state, "counter", 0)` gives the default for a thread that has not set the counter yet.
- The pool has one worker thread, so all its tasks run on that thread. The value that the first task sets is still there for the next task, until a task deletes it.
- For async code on one thread, use `contextvars` instead. A `ContextVar` belongs to a context, not to a thread.
