```python run id=suites-comprehensions-and-with
# A compound statement has a header and a suite; the suite is the block.
total = 0
for n in range(1, 4):
    sq = n * n
    total += sq
print("total =", total, "sq =", sq)  # the names from the loop body stay

# A comprehension has its own scope, so its variable does not leak.
squares = [k * k for k in range(3)]
print("squares =", squares, "k" in globals())

# with is a block whose exit code runs even after an exception.
class Tracker:
    def __enter__(self):
        print("enter")
        return self

    def __exit__(self, kind, value, tb):
        print("exit", kind.__name__ if kind else None)
        return True  # suppress the exception


with Tracker():
    raise ValueError("inside")
print("after with")

# An empty block needs pass, because a suite cannot be empty.
if total > 0:
    pass
```

```output for=suites-comprehensions-and-with
total = 14 sq = 9
squares = [0, 1, 4] False
enter
exit ValueError
after with
```

- Verdict: native. A suite is the block of a compound statement, and it is not a scope, so `sq` and `n` stay after the loop. A comprehension has its own scope, so `k` does not leak. `with` calls `__exit__` at the end of its block, also after an exception, and here `__exit__` suppresses the exception. A suite cannot be empty, so an empty block needs `pass`.
