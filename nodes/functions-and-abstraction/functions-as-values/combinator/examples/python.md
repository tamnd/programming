```python run id=combinator-python
from functools import partial


def identity(x):
    return x


def const(x):
    return lambda _: x


def flip(f):
    return lambda a, b: f(b, a)


def both(p, q):
    return lambda x: p(x) and q(x)


def sub(a, b):
    return a - b


print(identity(5))
print(const(7)(99))
print(flip(sub)(2, 10))
is_even = partial(lambda m, x: x % m == 0, 2)
keep = both(is_even, lambda x: x > 0)
print(*[x for x in range(-2, 5) if keep(x)])
```

```output for=combinator-python
5
7
8
2 4
```

- Verdict: native. A Python function can return a lambda, so each combinator is a short function. `functools.partial` is a combinator in the standard library.
