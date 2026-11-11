```python run id=trampoline
def sum_k(n, k):  # CPS, but every call still uses a Python stack frame
    if n == 0:
        return k(0)
    return sum_k(n - 1, lambda r: k(n + r))


try:
    sum_k(100_000, lambda r: r)
except RecursionError:
    print("RecursionError")


def sum_t(n, k):  # the same function, but it returns the next step instead of calling it
    if n == 0:
        return lambda: k(0)
    return lambda: sum_t(n - 1, lambda r: lambda: k(n + r))


def trampoline(step):
    while callable(step):
        step = step()
    return step


print(trampoline(sum_t(100_000, lambda r: r)))
```

```output for=trampoline
RecursionError
5000050000
```

```python wrong id=no-continuation
def abs_k(x, k):
    if x < 0:
        return k(-x)
    return x  # returns instead of calling the continuation


times_ten = lambda v: v * 10
print(abs_k(-3, times_ten), abs_k(3, times_ten))
```

```output for=no-continuation
30 3
```

- Verdict: idiom. Python has first-class functions and closures, so a function can take its continuation as a parameter, as `sum_k` does. CPython has no tail call elimination and limits the depth of the stack, so a CPS function that calls itself 100,000 times raises `RecursionError`. The trampoline fixes this: each step returns a function for the next step instead of calling it, and a loop runs the steps one at a time on a flat stack.
- The wrong block forgets the rule of CPS on one path. `abs_k` calls `k` for a negative number but returns the plain value for a positive one, so `abs_k(3, times_ten)` gives 3, not 30. In CPS, every path must end in a call to a continuation.
