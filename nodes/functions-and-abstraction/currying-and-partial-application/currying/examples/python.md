```python run id=partial
from functools import partial


def power(base, exponent):
    return base**exponent


square = partial(power, exponent=2)
two_to = partial(power, 2)
print(square(5), two_to(10))


def curried_power(base):
    def with_exponent(exponent):
        return base**exponent

    return with_exponent


print(curried_power(3)(2))
print(int("ff", 16), partial(int, base=16)("ff"))
```

```output for=partial
25 1024
9
255 255
```

- Verdict: stdlib. `functools.partial` returns a new callable with some positional or keyword arguments fixed. Arguments in the later call are appended to the fixed positional arguments, and keyword arguments extend or override the fixed ones.
- A curried function is a function that returns a function, and a closure is the usual way to write it. Python does not curry functions automatically.
- `functools.Placeholder` reserves a position in the fixed arguments, so a later argument can fill a gap in the middle.

```python wrong id=late-binding
adders = [lambda x: x + n for n in range(3)]
print([add(10) for add in adders])
```

```output for=late-binding
[12, 12, 12]
```

Each lambda reads `n` when it is called, not when it is made, and at that time the loop has ended with `n` equal to 2. Fix the value at creation with `partial(operator.add, n)` or with a default argument, `lambda x, n=n: x + n`.
