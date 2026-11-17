```python run id=deep
import sys


def depth(n):
    if n == 0:
        return 0
    return 1 + depth(n - 1)          # one frame for each step


def loop(n):
    count = 0
    while n > 0:                     # one frame for the whole loop
        n -= 1
        count += 1
    return count


print("depth(100) =", depth(100))
try:
    depth(1_000_000)
except RecursionError:
    print("depth(1000000): RecursionError, the limit is", sys.getrecursionlimit())
print("loop(1000000) =", loop(1_000_000))
```

```output for=deep
depth(100) = 100
depth(1000000): RecursionError, the limit is 1000
loop(1000000) = 1000000
```

- Verdict: native. Python counts the frames of Python functions and raises `RecursionError` past the limit. The program can catch it, and `sys.setrecursionlimit` changes the limit.
