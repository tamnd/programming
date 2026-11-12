```python run id=balking-pattern
import threading


class Document:
    def __init__(self, write):
        self._lock = threading.Lock()
        self._write = write  # the slow part of a save
        self._changed = False
        self._saving = False
        self.saves = 0

    def edit(self):
        with self._lock:
            self._changed = True

    def save(self):
        """Save the document. Return False at once if a save cannot start now."""
        with self._lock:
            if self._saving or not self._changed:
                return False  # balk: the object is not in the right state
            self._saving = True
            self._changed = False
        self._write()
        with self._lock:
            self._saving = False
            self.saves += 1
        return True


started = threading.Event()
release = threading.Event()


def slow_write():
    started.set()
    release.wait()  # the first save stays in progress until the other calls end


doc = Document(slow_write)
doc.edit()
first = threading.Thread(target=doc.save)
first.start()
started.wait()

results = []
callers = [threading.Thread(target=lambda: results.append(doc.save())) for _ in range(7)]
for t in callers:
    t.start()
for t in callers:
    t.join()
release.set()
first.join()

print("calls during a save:", len(results))
print("calls that balked:", results.count(False))
print("saves run:", doc.saves)
print("save with no changes:", "ran" if doc.save() else "balked")
doc.edit()
print("save after an edit:", "ran" if doc.save() else "balked")
print("saves run:", doc.saves)
```

```output
calls during a save: 7
calls that balked: 7
saves run: 1
save with no changes: balked
save after an edit: ran
saves run: 2
```

- Verdict: idiom. A `threading.Lock` makes the test and the change of the `_saving` flag one step, and the write runs without the lock.
