```python run id=bound-and-unbound
class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height

    def scale(self, factor):
        self.width *= factor
        self.height *= factor


def area(r):  # a free function does the same job
    return r.width * r.height


r = Rectangle(3, 4)
unbound = Rectangle.area  # a plain function: the receiver is its first parameter
bound = r.area            # a method object: the function and r together
r.scale(2)
print(r.area(), unbound(r), bound(), area(r))
print(type(unbound).__name__, type(bound).__name__)
print(bound.__func__ is unbound, bound.__self__ is r)
print(sorted([Rectangle(5, 1), Rectangle(2, 2)], key=Rectangle.area)[0].width)
```

```output for=bound-and-unbound
48 48 48 48
function method
True True
2
```

- Verdict: native. A method is a function defined in the class body, and its first parameter, by convention `self`, is the instance. `r.area()` looks up `area` in the class of `r` and calls it with `r` as the first argument, so `Rectangle.area(r)` gives the same result.
- `r.area` without the call is a bound method object. It holds the function in `__func__` and the instance in `__self__`, and it keeps working after the instance changes, because it holds the instance itself, not a copy.
- `Rectangle.area` is an ordinary function, so it can be passed where a function of one argument is expected, such as the `key` of `sorted`.
