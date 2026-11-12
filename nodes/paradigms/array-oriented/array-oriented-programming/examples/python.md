```python run id=toolkit
from itertools import accumulate
import operator

a = [3, 1, 4, 1, 5]
b = [2, 7, 1, 8, 2]

print(list(map(operator.add, a, b)))       # element-wise sum
print(sum(map(operator.mul, a, b)))        # dot product
print(list(accumulate(a)))                 # scan: running totals
print(max(a), [x for x in a if x > 2])     # reduce, and compress by a mask
print([[x * y for y in range(1, 4)] for x in range(1, 4)])  # outer product
```

```output for=toolkit
[5, 8, 5, 9, 7]
35
[3, 4, 8, 9, 14]
5 [3, 4, 5]
[[1, 2, 3], [2, 4, 6], [3, 6, 9]]
```

```python wrong id=list-is-not-array
a = [1, 2, 3]
print(a * 2)
print(a + [10, 20, 30])
```

```output for=list-is-not-array
[1, 2, 3, 1, 2, 3]
[1, 2, 3, 10, 20, 30]
```

- Verdict: library. The core language has no operations on whole arrays. `map` with two lists, `sum`, `itertools.accumulate` and comprehensions give element-wise operations, reductions, scans and outer products, one Python call per element. NumPy is the library that makes Python an array language: its `ndarray` type has element-wise operators, broadcasting and reductions that run as compiled loops.
- The wrong block treats a list as an array of numbers. For a list, `*` repeats the list and `+` joins two lists. NumPy arrays give `[2, 4, 6]` and `[11, 22, 33]` for the same expressions.
