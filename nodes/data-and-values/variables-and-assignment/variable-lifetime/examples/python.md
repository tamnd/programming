```python run id=closure-keeps-local
def make_counter():
    count = 0

    def step():
        nonlocal count
        count += 1
        return count

    return step


a = make_counter()
b = make_counter()
print(a(), a(), b())
```

```output for=closure-keeps-local
1 2 1
```

- Verdict: native. A local variable normally ends when its call returns, but `step` uses `count`, so `count` lives as long as the function `step`. Each call of `make_counter` makes a new `count`, so `a` and `b` count apart. The objects themselves end when no name refers to them, at a time that the garbage collector chooses.
