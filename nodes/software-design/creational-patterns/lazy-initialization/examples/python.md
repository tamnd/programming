```python run id=lazy
import threading


class Settings:
    def __init__(self):
        self._config = None
        self._lock = threading.Lock()
        self.loads = 0

    def _load(self) -> dict:
        print("loading config")  # stands for a slow read of a file
        self.loads += 1
        return {"timeout": 30}

    @property
    def config(self) -> dict:
        if self._config is None:  # fast path: no lock after the first load
            with self._lock:
                if self._config is None:  # check again under the lock
                    self._config = self._load()
        return self._config


s = Settings()
print("settings created, loads =", s.loads)
print("timeout =", s.config["timeout"])
print("timeout =", s.config["timeout"])
print("loads =", s.loads)

shared = Settings()
start = threading.Barrier(8)


def worker():
    start.wait()  # all eight threads ask at the same time
    shared.config


threads = [threading.Thread(target=worker) for _ in range(8)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("8 threads: loads =", shared.loads)
```

```output
settings created, loads = 0
loading config
timeout = 30
timeout = 30
loads = 1
loading config
8 threads: loads = 1
```

- Verdict: stdlib. `functools.cached_property` computes a value on first access and stores it as a normal attribute. Its documentation says that it does not prevent a race, so the getter can run more than once on one instance. The example uses a lock for that reason.
- `functools.cache` makes a function lazy and memoized: the first call computes, and later calls with the same arguments return the stored result.
- A module is a lazy singleton: Python runs its code at the first `import` and caches the module object.
