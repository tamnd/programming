```python run id=frames
import sys


def descend(level):
    return descend(level + 1)


print("recursion limit:", sys.getrecursionlimit())
try:
    descend(1)
except RecursionError as error:
    print("caught:", type(error).__name__)

print(isinstance(7, object), isinstance(descend, object))
```

```output
recursion limit: 1000
caught: RecursionError
True True
```

- Verdict: native. CPython keeps every value, even an integer, as an object in its private heap, and a frame holds references to objects.
- Each call of `descend` adds a frame. When the depth passes the limit that `sys.getrecursionlimit()` returns, the interpreter raises `RecursionError`, and the program can catch it.
- The limit protects the C stack of the interpreter. `sys.setrecursionlimit` changes it, and a limit that is too high can crash the process.
