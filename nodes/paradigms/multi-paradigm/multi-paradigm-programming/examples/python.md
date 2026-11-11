```python run id=three-styles
from functools import reduce

amounts = [30, 70, 25, 10, 5]


def total_loop(xs):  # procedural
    t = 0
    for x in xs:
        if x >= 10:
            t += x
    return t


def total_fn(xs):  # functional
    return reduce(lambda acc, x: acc + x, filter(lambda x: x >= 10, xs), 0)


class Till:  # object-oriented: state and behavior together
    def __init__(self):
        self.total = 0

    def ring(self, x):
        if x >= 10:
            self.total += x
        return self


till = Till()
for a in amounts:
    till.ring(a)

print(total_loop(amounts), total_fn(amounts), till.total,
      sum(x for x in amounts if x >= 10))
```

```output for=three-styles
135 135 135 135
```

```python wrong id=late-binding
# Build three functions that should return 0, 1 and 2.
fns = []
for i in range(3):
    fns.append(lambda: i)

print([f() for f in fns])
```

```output for=late-binding
[2, 2, 2]
```

- Verdict: native. Python has statements and loops, classes with inheritance and dynamic dispatch, first-class functions, comprehensions and generators, so procedural, object-oriented and functional code all look natural. The first block computes one total in four ways, the last a generator expression. Python leans toward the imperative style: `lambda` holds one expression, and the standard library has no persistent collections.
- The wrong block mixes a closure with a loop variable that changes. Each `lambda` captures the variable `i`, not its value at the time, and after the loop `i` is 2. Bind the value when the function is made, with `lambda i=i: i` or `functools.partial`.
