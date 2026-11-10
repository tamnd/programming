```python run id=cycle
import gc
import weakref


class Item:
    def __init__(self):
        self.peer = None


def make_cycle():
    a, b = Item(), Item()
    a.peer, b.peer = b, a
    probe = weakref.ref(a)
    print("alive while referenced:", probe() is not None)
    return probe


probe = make_cycle()
print("unreachable objects found:", gc.collect())
print("collected after gc:", probe() is None)
```

```output
alive while referenced: True
unreachable objects found: 2
collected after gc: True
```

- Verdict: native. CPython frees most objects by reference counting, at the moment the last reference goes away. A tracing cycle collector frees groups of objects that refer to each other.
- `gc.collect()` runs a full collection at once and returns the number of unreachable objects that it found. Here they are the two `Item` objects.
- Only container objects (lists, dicts, instances of most classes) are tracked by the cycle collector. `gc.is_tracked` tells whether an object is.
- `weakref.ref` gives a reference that does not keep the object alive. The free-threaded build uses the same model, but each collection scans the whole heap.
