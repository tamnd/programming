```python run id=special-methods-and-chained-comparisons
import operator
from functools import reduce


class Vec:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __add__(self, o):
        return Vec(self.x + o.x, self.y + o.y)

    def __mul__(self, k):  # Vec * number
        return Vec(self.x * k, self.y * k)

    def __rmul__(self, k):  # number * Vec: Python tries the right operand
        return self * k

    def __matmul__(self, o):  # @ is the dot product here
        return self.x * o.x + self.y * o.y

    def __eq__(self, o):
        return (self.x, self.y) == (o.x, o.y)

    def __repr__(self):
        return f"Vec({self.x}, {self.y})"


a, b = Vec(1, 2), Vec(3, 4)
print(a + b, a * 2, 2 * a, a @ b, a == Vec(1, 2))

# The operator module gives each operator as a function.
print(reduce(operator.mul, [1, 2, 3, 4]))
print(sorted([("b", 2), ("a", 3)], key=operator.itemgetter(1)))

# Comparisons chain: 1 < x < 5 means 1 < x and x < 5.
x = 3
print(1 < x < 5, 1 < x > 5)

# // and % round toward minus infinity.
print(-7 // 2, -7 % 2)

# The walrus operator := assigns inside an expression.
if (n := len("hello")) > 3:
    print("n =", n)
```

```output for=special-methods-and-chained-comparisons
Vec(4, 6) Vec(2, 4) Vec(2, 4) 11 True
24
[('b', 2), ('a', 3)]
True False
-4 1
n = 5
```

- Verdict: native. A class gives meaning to `+`, `*` and `@` with special methods, and `__rmul__` handles `2 * a`, where the left operand is a number. The `operator` module gives each operator as a function. Comparisons chain, so `1 < x < 5` means `1 < x and x < 5`. `//` and `%` round toward minus infinity. `:=` assigns inside an expression.
