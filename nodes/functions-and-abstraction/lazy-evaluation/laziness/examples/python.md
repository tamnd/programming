```python run id=generators
from functools import cached_property
from itertools import count, islice


def squares():
    for n in count(1):
        yield n * n


print(list(islice(squares(), 5)))


class Report:
    @cached_property
    def summary(self):
        print("building summary")
        return "3 errors"


report = Report()
print("report created")
print(report.summary)
print(report.summary)
```

```output for=generators
[1, 4, 9, 16, 25]
report created
building summary
3 errors
3 errors
```

- Verdict: stdlib. Python evaluates arguments eagerly. Generators, generator expressions, `range`, `map`, `filter`, `zip` and `itertools` produce values on demand.
- `functools.cached_property` computes an attribute on first access and stores it. `and`, `or` and the conditional expression evaluate only the operands that they need.

```python wrong id=used-up
lines = (line.upper() for line in ["a", "b"])
print(list(lines))
print(list(lines))
```

```output for=used-up
['A', 'B']
[]
```

A generator produces its values one time. The second pass finds it empty and gives no error. Use a list if the program must read the values more than once, or make a new generator for each pass.
