```python run id=tree
import sys


def total(item):
    if isinstance(item, int):
        return item
    return sum(total(child) for child in item)


def depth(n):
    return 0 if n == 0 else 1 + depth(n - 1)


print(total([1, [2, 3], [[4], 5]]))
print(sys.getrecursionlimit())
print(depth(900))
try:
    depth(100_000)
except RecursionError as error:
    print(type(error).__name__, error)
```

```output for=tree
15
1000
900
RecursionError maximum recursion depth exceeded
```

- Verdict: native. A function can call itself by name, and each call gets its own frame with its own local variables.
- CPython limits the depth of Python calls. `sys.getrecursionlimit()` gives the limit, and a deeper call raises `RecursionError`, which a program can catch.
- `sys.setrecursionlimit()` raises the limit. The highest safe value depends on the platform, and a limit that is too high can crash the interpreter.
- A call in tail position takes a frame like any other call. The function below has its recursive call in tail position and still raises `RecursionError`.
- `functools.cache` stores the result of each call, which removes the repeated work of tree recursion such as the naive Fibonacci function.

```python fails expect=exception:RecursionError id=tail-call
def count(n, steps=0):
    if n == 0:
        return steps
    return count(n - 1, steps + 1)


print(count(100_000))
```
