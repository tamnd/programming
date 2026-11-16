```python run id=map-filter-reduce-python
from functools import reduce

nums = range(1, 7)
squares = list(map(lambda x: x * x, filter(lambda x: x % 2 == 0, nums)))
print(*squares)
print(reduce(lambda acc, x: acc + x, squares, 0))
```

```output for=map-filter-reduce-python
4 16 36
56
```

- Verdict: native. `map` and `filter` are built-in functions, and `reduce` is in the module `functools`. `map` and `filter` return iterators, so `list` keeps the result for the second use.
