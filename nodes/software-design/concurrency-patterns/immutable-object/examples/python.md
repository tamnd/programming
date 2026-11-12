```python run id=immutable-object
import threading
import time
from dataclasses import dataclass


@dataclass(frozen=True)
class Settings:
    port: int
    tag: str


first = Settings(8000, "port 8000")
current = first  # readers take this reference; the writer replaces it
mixed = 0
reads = 0
lock = threading.Lock()  # only for the counters of the demo


def reader():
    global mixed, reads
    for _ in range(1000):
        s = current  # one read of the reference gives a consistent snapshot
        time.sleep(0)  # let the writer run between the two field reads
        ok = s.tag == f"port {s.port}"
        with lock:
            reads += 1
            mixed += not ok


def writer():
    global current
    for port in range(8001, 8101):
        current = Settings(port, f"port {port}")  # a new object, never a change
        time.sleep(0)


threads = [threading.Thread(target=reader) for _ in range(4)] + [threading.Thread(target=writer)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("reads:", reads)
print("reads that saw a mixed pair:", mixed)
print("settings published:", 1 + (current.port - first.port))
print("first settings unchanged:", first == Settings(8000, "port 8000"))
```

```output
reads: 4000
reads that saw a mixed pair: 0
settings published: 101
first settings unchanged: True
```

- Verdict: stdlib. `@dataclass(frozen=True)` makes assignment to a field raise an exception, and the writer publishes each new `Settings` by one assignment to `current`.
