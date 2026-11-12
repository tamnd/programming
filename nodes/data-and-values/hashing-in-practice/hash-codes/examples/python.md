```python run id=hash-and-eq-together
import os
import subprocess
import sys
from dataclasses import dataclass


@dataclass(frozen=True)  # frozen: __eq__ and __hash__ from the same fields
class Point:
    x: int
    y: int


seen = {Point(1, 2): "a"}
print(Point(1, 2) in seen, hash(Point(1, 2)) == hash((1, 2)))
print("1, 1.0 and True:", hash(1) == hash(1.0) == hash(True), len({1, 1.0, True}))
print("hash(-1) == hash(-2):", hash(-1) == hash(-2))
try:
    {[1, 2]: "list"}
except TypeError:
    print("TypeError: a list cannot be a key, because it is mutable")


def str_hash(seed):  # the hash of a str depends on a per-process seed
    env = dict(os.environ, PYTHONHASHSEED=str(seed))
    code = "print(hash('hello'))"
    return subprocess.run([sys.executable, "-c", code], env=env,
                          capture_output=True, text=True).stdout


print("same seed, same hash:", str_hash(1) == str_hash(1))
print("other seed, same hash:", str_hash(1) == str_hash(2))
```

```output for=hash-and-eq-together
True True
1, 1.0 and True: True 1
hash(-1) == hash(-2): True
TypeError: a list cannot be a key, because it is mutable
same seed, same hash: True
other seed, same hash: False
```

- Verdict: native. `hash()` and `__hash__` are part of the language, and `dict` and `set` are built in. A frozen dataclass makes `__eq__` and `__hash__` from the same fields, and its hash equals the hash of the tuple of the fields. Numbers that compare equal hash equal, so `1`, `1.0` and `True` are one key. A list has no hash, because it can change. The hash of a `str` is salted with a random value for each process; the example starts two interpreters with fixed `PYTHONHASHSEED` values to show that a different seed gives a different hash.
