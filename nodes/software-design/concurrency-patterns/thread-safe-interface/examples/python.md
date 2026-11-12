```python run id=interface
import threading


class FileCache:
    def __init__(self):
        self._lock = threading.Lock()  # not reentrant
        self.files = {}
        self.acquires = 0

    # Interface methods: take the lock, then call an implementation method.
    def find(self, path):
        with self._lock:
            self.acquires += 1
            return self._find_i(path)

    def bind(self, path):
        with self._lock:
            self.acquires += 1
            self._bind_i(path)

    # Implementation methods: trust that the lock is held, never take it.
    def _find_i(self, path):
        file = self._check_cache_i(path)
        if file is None:
            self._bind_i(path)  # not bind(): that would take the lock again
            file = self._check_cache_i(path)
        return file

    def _check_cache_i(self, path):
        return self.files.get(path)

    def _bind_i(self, path):
        self.files[path] = "mapped " + path


cache = FileCache()
cache.bind("/index.html")
paths = ["/a.html", "/b.html", "/c.html"]


def worker():
    for i in range(1000):
        cache.find(paths[i % 3])


threads = [threading.Thread(target=worker) for _ in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("find /a.html:", cache.find("/a.html"))
print("files cached:", len(cache.files))
print("lock acquisitions:", cache.acquires)
```

```output
find /a.html: mapped /a.html
files cached: 4
lock acquisitions: 4002
```

- Verdict: idiom. The leading underscore marks the implementation methods by convention only. `threading.Lock` is not reentrant, so a call from `_find_i` to `find` would block.
