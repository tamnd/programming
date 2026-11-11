```python run id=everything-is-an-object
import math


class Logged:
    def __init__(self, target):
        self.target = target

    def __getattr__(self, name):  # called for a message with no method
        print("message:", name)
        return getattr(self.target, name)


for thing in (5, "hi", [1, 2], len, math, Logged):
    print(type(thing).__name__, isinstance(thing, object))

print((5).__add__(3), "hi".upper())  # operators are messages too
print(getattr([3, 1, 2], "index")(2))  # a message chosen at run time

log = Logged([3, 1, 2])
print(log.count(1))
```

```output for=everything-is-an-object
int True
str True
list True
builtin_function_or_method True
module True
type True
8 HI
2
message: count
1
```

```python wrong id=aliased-rows
row = [0, 0, 0]
grid = [row] * 3  # three references to one list object
grid[0][0] = 1
print(grid)
print(grid[0] is grid[1])
```

```output for=aliased-rows
[[1, 0, 0], [1, 0, 0], [1, 0, 0]]
True
```

- Verdict: native. Every value in Python is an object: numbers, strings, functions, modules and classes all have a type, an identity and attributes. Operators are method calls, so `5 + 3` sends the message `__add__` to `5`. `getattr` sends a message whose name is known only at run time. When an attribute lookup fails, Python calls `__getattr__`, which lets an object answer messages that it has no method for, as the `Logged` wrapper does.
- Variables hold references, so assignment never copies an object. The wrong block builds a grid with `[row] * 3`, which repeats one reference three times. A change through `grid[0]` appears in every row, and `is` shows that the rows are one object. Build each row anew, as in `[[0] * 3 for _ in range(3)]`.
