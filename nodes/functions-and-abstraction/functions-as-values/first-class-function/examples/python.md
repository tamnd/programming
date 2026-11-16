```python run id=first-class-function-python
def double(x):
    return x * 2


def square(x):
    return x * x


def apply_twice(g, x):
    return g(g(x))


def adder(n):
    def add(x):
        return x + n

    return add


f = double
print(f(5))

ops = {"double": double, "square": square}
print(ops["square"](4))

print(apply_twice(double, 3))

add5 = adder(5)
print(add5(10))
```

```output for=first-class-function-python
10
16
12
15
```

- Verdict: native. A `def` makes a function object. The program stores it in a variable and a dict, passes it to `apply_twice` and returns the inner function `add` from `adder`.
