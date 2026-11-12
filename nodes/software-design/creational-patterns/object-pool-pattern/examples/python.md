```python run id=pool
import threading
from contextlib import contextmanager


class Buffer:
    def __init__(self, ident: int):
        self.ident = ident  # a real buffer would be costly to create
        self.parts: list[str] = []

    def write(self, text: str) -> None:
        self.parts.append(text)

    def text(self) -> str:
        return " ".join(self.parts)


class Pool:
    def __init__(self, create, reset, max_idle: int):
        self._create, self._reset, self._max_idle = create, reset, max_idle
        self._idle: list = []
        self._lock = threading.Lock()
        self.created = 0

    def acquire(self):
        with self._lock:
            if self._idle:
                return self._idle.pop()
            self.created += 1
            return self._create(self.created)

    def release(self, item) -> None:
        self._reset(item)  # clear the state before the next user sees it
        with self._lock:
            if len(self._idle) < self._max_idle:
                self._idle.append(item)  # else: drop it, the pool is full

    @contextmanager
    def lease(self):
        item = self.acquire()
        try:
            yield item
        finally:
            self.release(item)  # runs even if the body raises

    def idle(self) -> int:
        return len(self._idle)


pool = Pool(create=Buffer, reset=lambda b: b.parts.clear(), max_idle=2)
for i in range(1, 4):
    with pool.lease() as b:
        b.write(f"request {i}")
        print(f"request {i} -> buffer #{b.ident}: {b.text()}")

held = [pool.acquire() for _ in range(3)]
print("3 at once -> buffers", " ".join(f"#{b.ident}" for b in sorted(held, key=lambda b: b.ident)))
for b in held:
    pool.release(b)
print("idle after release:", pool.idle())
print("created:", pool.created)
```

```output
request 1 -> buffer #1: request 1
request 2 -> buffer #1: request 2
request 3 -> buffer #1: request 3
3 at once -> buffers #1 #2 #3
idle after release: 2
created: 3
```

- Verdict: idiom. The standard library has no general object pool. A list and a lock are enough, and a `contextmanager` makes the release automatic in a `with` block.
- For a pool of threads or processes, use `concurrent.futures.ThreadPoolExecutor` or `multiprocessing.Pool`. Database drivers and libraries such as SQLAlchemy have their own connection pools.
- CPython frees most objects as soon as the last reference goes, so a pool of plain Python objects rarely saves time. Pool the costly resource, not the wrapper.
