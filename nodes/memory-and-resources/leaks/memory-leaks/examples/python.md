```python run id=cache-growth
import tracemalloc
import weakref


class Image:
    def __init__(self, name):
        self.name = name
        self.pixels = bytearray(10_000)


def growth(cache, count):
    """Loads count images through cache and returns the traced growth in bytes."""
    before = tracemalloc.take_snapshot()
    for i in range(count):
        image = Image(f"img-{i}")
        cache[image.name] = image
    after = tracemalloc.take_snapshot()
    return sum(stat.size_diff for stat in after.compare_to(before, "filename"))


tracemalloc.start()

strong = {}
print("strong cache grew over 500 KB:", growth(strong, 100) > 500_000)
print("strong cache size:", len(strong))

weak = weakref.WeakValueDictionary()
print("weak cache grew over 500 KB:", growth(weak, 100) > 500_000)
print("weak cache size:", len(weak))

kept = Image("logo")
weak[kept.name] = kept
print("weak cache size with one image in use:", len(weak))

tracemalloc.stop()
```

```output
strong cache grew over 500 KB: True
strong cache size: 100
weak cache grew over 500 KB: False
weak cache size: 0
weak cache size with one image in use: 1
```

- Verdict: native. CPython frees an object when its last reference goes. A plain `dict` used as a cache is a reference, so its values stay reachable after the program stops using them.
- `tracemalloc` compares two snapshots. Growth that stays after the work is done points to the leak; `compare_to` groups the difference by file or by line.
- `WeakValueDictionary` drops an entry when no other code refers to the value. For a cache that must keep values, use `functools.lru_cache` with a `maxsize`.
