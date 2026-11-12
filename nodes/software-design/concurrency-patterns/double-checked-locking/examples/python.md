```python run id=dcl
import threading


class Config:
    created = 0

    def __init__(self):
        Config.created += 1


_instance = None
_lock = threading.Lock()
locked_calls = 0


def instance():
    global _instance, locked_calls
    if _instance is None:  # first check, no lock
        with _lock:
            locked_calls += 1
            if _instance is None:  # second check, lock held
                _instance = Config()
    return _instance


start = threading.Barrier(8)


def worker():
    start.wait()
    for _ in range(1000):
        instance()


threads = [threading.Thread(target=worker) for _ in range(8)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("instances created:", Config.created)
print("calls:", 8 * 1000)
print("calls that took the lock:", "8 or fewer" if locked_calls <= 8 else locked_calls)
```

```output
instances created: 1
calls: 8000
calls that took the lock: 8 or fewer
```

- Verdict: idiom. The module keeps the instance and the lock in globals. Python has no `volatile` field, so the code is the plain form of the two checks.
