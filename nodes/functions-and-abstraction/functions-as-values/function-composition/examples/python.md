```python run id=function-composition-python
from functools import reduce


def compose(f, g):
    return lambda x: f(g(x))


def compose_all(*fs):
    return reduce(compose, fs, lambda x: x)


def inc(x):
    return x + 1


def double(x):
    return x * 2


print(compose(double, inc)(5))
print(compose(inc, double)(5))
print(compose_all(inc, double, inc)(5))
```

```output for=function-composition-python
12
11
13
```

- Verdict: idiom. Python has no compose function or operator. A lambda that calls `f(g(x))` composes two functions, and `functools.reduce` with the identity function composes a list of them.
