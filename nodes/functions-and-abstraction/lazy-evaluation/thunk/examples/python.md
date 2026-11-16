```python run id=thunks
from functools import cache


def load():
    print("loading")
    return [1, 2, 3]


plain = lambda: load()
memo = cache(load)

print(len(plain()), len(plain()))
print(len(memo()), len(memo()))
```

```output for=thunks
loading
loading
3 3
loading
3 3
```

- Verdict: idiom. A `lambda` with no parameters is a plain thunk: each call runs `load` again.
- `functools.cache` on a function with no parameters gives a memoized thunk. The first call runs `load`, and later calls return the stored list.
- `functools.cached_property` is the memoized thunk for one attribute of an object.
