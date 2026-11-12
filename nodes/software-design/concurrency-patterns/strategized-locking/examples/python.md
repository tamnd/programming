```python run id=strategy
import threading


class NullMutex:
    """A lock for one thread: both methods do nothing."""

    def acquire(self):
        pass

    def release(self):
        pass


class FileCache:
    def __init__(self, lock):
        self._lock = lock  # the locking strategy is a parameter
        self.files = {}
        self.lookups = 0

    def find(self, path):
        self._lock.acquire()
        try:
            self.lookups += 1
            if path not in self.files:
                self.files[path] = "mapped " + path
            return self.files[path]
        finally:
            self._lock.release()


single = FileCache(NullMutex())
for path in ["/a.html", "/b.html", "/a.html"]:
    single.find(path)
print(f"Null_Mutex cache: {len(single.files)} files, {single.lookups} lookups")

multi = FileCache(threading.Lock())


def worker():
    for i in range(1000):
        multi.find(["/a.html", "/b.html", "/c.html"][i % 3])


threads = [threading.Thread(target=worker) for _ in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(f"Thread_Mutex cache: {len(multi.files)} files, {multi.lookups} lookups")
```

```output
Null_Mutex cache: 2 files, 3 lookups
Thread_Mutex cache: 3 files, 4000 lookups
```

- Verdict: idiom. Any object with `acquire` and `release` methods can be the strategy, so `threading.Lock` and the null class fit without a common base class.
