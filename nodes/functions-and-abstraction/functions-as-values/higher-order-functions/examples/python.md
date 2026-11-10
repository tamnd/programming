```python run id=hof
from functools import reduce


def twice(f, x):
    return f(f(x))


def compose(f, g):
    return lambda x: f(g(x))


words = ["pear", "fig", "banana"]
print(twice(lambda n: n * 3, 2))
print(list(map(len, words)))
print(list(filter(lambda w: len(w) > 3, words)))
print(reduce(lambda total, n: total + n, [1, 2, 3, 4], 0))
print(sorted(words, key=len))
add_then_double = compose(lambda n: n * 2, lambda n: n + 1)
print(add_then_double(5))
```

```output for=hof
18
[4, 3, 6]
['pear', 'banana']
10
['fig', 'pear', 'banana']
12
```

- Verdict: native. A function is an object. `def` and `lambda` both make one, and a function can take, store and return such objects.
- `map` and `filter` return iterators, not lists. The work happens when a consumer such as `list` asks for the items, and an iterator gives its items only once (see Pitfalls).
- `reduce` is in the `functools` module, not in the built-in names.
- `sorted`, `min` and `max` take a `key` function of one argument. The sort calls it once for each item and compares the results.
