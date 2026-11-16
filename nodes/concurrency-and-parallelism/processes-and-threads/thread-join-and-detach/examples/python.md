```python run id=join-timeout
import threading

release = threading.Event()
results = []


def work():
    release.wait()
    results.append("done")


worker = threading.Thread(target=work)
worker.start()
worker.join(timeout=0.05)
print("after join(timeout=0.05), alive:", worker.is_alive())
release.set()
worker.join()
print("after join(), alive:", worker.is_alive())
print("results:", results)
```

```output for=join-timeout
after join(timeout=0.05), alive: True
after join(), alive: False
results: ['done']
```

- Verdict: stdlib. `Thread.join` waits for the thread, with an optional timeout. Python has no detach; a daemon thread is the nearest choice, as the Idea section shows.
- `join` returns `None` in both cases, so the example calls `is_alive()` to see whether the first join timed out.
- The worker waits for an `Event`. The main thread sets the event and then joins, so the second join returns after the worker has added its result.
- A join on a thread that has not started, or on the current thread, raises `RuntimeError`.
