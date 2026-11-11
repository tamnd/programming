```python run id=pipeline
from functools import reduce
from itertools import accumulate


def compose(*fs):
    return reduce(lambda f, g: lambda x: g(f(x)), fs, lambda x: x)


words = ("map", "filter", "reduce", "fold", "zip")
shout = compose(str.upper, lambda s: s + "!")
print([shout(w) for w in words if len(w) > 3])
print(reduce(lambda acc, w: acc + len(w), words, 0))
print(list(accumulate([3, 1, 4, 1, 5])))
print(sorted(words, key=len))
print(words)
```

```output for=pipeline
['FILTER!', 'REDUCE!', 'FOLD!']
22
[3, 4, 8, 9, 14]
['map', 'zip', 'fold', 'filter', 'reduce']
('map', 'filter', 'reduce', 'fold', 'zip')
```

```python wrong id=sort-in-place
def top_two(scores):
    scores.sort(reverse=True)
    return scores[:2]


scores = [70, 95, 80]
print(top_two(scores))
print(scores)
```

```output for=sort-in-place
[95, 80]
[95, 80, 70]
```

- Verdict: native. Functions are values, and `lambda`, comprehensions, generators, `map`, `filter`, `functools.reduce` and `itertools` support the style. Tuples and `frozenset` are immutable.
- Python is not built for pure code. It has no tail call elimination, so deep recursion fails, and most built-in containers are mutable. The functional HOWTO says that Python programs in this style usually give "a functional-appearing interface" and use assignment inside.
- In the wrong block, `list.sort` changes the list of the caller. Use `sorted(scores, reverse=True)`, which returns a new list.
