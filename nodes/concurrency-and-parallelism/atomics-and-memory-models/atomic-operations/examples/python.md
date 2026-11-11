```python run id=lock
import threading
from collections import Counter
from concurrent.futures import ThreadPoolExecutor


class SafeCounter:
    def __init__(self):
        self._value = 0
        self._lock = threading.Lock()

    def add(self, n=1):
        with self._lock:
            self._value += n
            return self._value

    @property
    def value(self):
        with self._lock:
            return self._value


counter = SafeCounter()


def work(_):
    for _ in range(50_000):
        counter.add()


with ThreadPoolExecutor(max_workers=4) as pool:
    list(pool.map(work, range(4)))
print(counter.value)


def count_words(text):
    return Counter(text.split())


with ThreadPoolExecutor() as pool:
    parts = pool.map(count_words, ["a b a", "b c", "a"])
print(sum(parts, Counter()))
```

```output for=lock
200000
Counter({'a': 3, 'b': 2, 'c': 1})
```

- Verdict: absent. Python has no atomic integer type and no memory-order API. A `threading.Lock` makes a read-modify-write sequence indivisible.
- The global interpreter lock makes single operations on built-in types safe in the default build, and the free-threaded build uses internal locks to keep similar behavior. A sequence such as `counter += 1` is still several steps, and other threads can run between them.
- The second part avoids shared state: each thread counts its own part, and the main thread combines the results.
