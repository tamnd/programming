```python run id=guard
import threading

lock = threading.Lock()
table = {}


def increment(path):
    with lock:  # acquired here, released on every way out
        if not path:
            return -1
        table[path] = table.get(path, 0) + 1
        return 0


def worker():
    for _ in range(1000):
        increment("/index")


threads = [threading.Thread(target=worker) for _ in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("hits /index:", table["/index"])
print("bad path returns", increment(""))
print("lock free after the early return:", not lock.locked())
increment("/index")
print("hits /index:", table["/index"])
```

```output
hits /index: 4000
bad path returns -1
lock free after the early return: True
hits /index: 4001
```

- Verdict: native. A lock is a context manager, so the `with` statement is the guard.
