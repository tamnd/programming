```python run id=callback-python
def repeat(n, callback):
    for i in range(1, n + 1):
        callback(i)


def tick(i):
    print("tick", i)


print(" ".join(str(x) for x in sorted([3, 1, 2], key=lambda x: -x)))
repeat(2, tick)
```

```output for=callback-python
3 2 1
tick 1
tick 2
```

- Verdict: native. `sorted` calls the `key` function for each item, and `repeat` calls the function `tick` that the program gives it.
