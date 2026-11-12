```python run id=power-and-chained-comparisons
import ast

# Unary minus binds less tightly than ** on its right, so -2 ** 2 is -(2 ** 2).
print(-2 ** 2, (-2) ** 2, 2 ** -1)

# Comparisons bind less tightly than bitwise operators, so this is (x & 1) == 0.
x = 6
print(x & 1 == 0)

# Comparisons chain: 1 < x < 10 means 1 < x and x < 10.
print(1 < x < 10, 1 < x > 5)

# `not` binds less tightly than ==, and `and` binds more tightly than `or`.
print(not 1 == 2, True or False and False)

# The conditional expression has almost the lowest precedence.
print(1 + 2 if x > 5 else 0)

# ast shows how the parser grouped an expression.
print(ast.dump(ast.parse("-2 ** 2", mode="eval").body))
t = ast.parse("a or b and c", mode="eval").body
print(type(t.op).__name__, [type(v).__name__ for v in t.values])
```

```output for=power-and-chained-comparisons
-4 4 0.5
True
True True
True True
3
UnaryOp(op=USub(), operand=BinOp(left=Constant(value=2), op=Pow(), right=Constant(value=2)))
Or ['Name', 'BoolOp']
```

- Verdict: native. `**` binds more tightly than a unary minus on its left, so `-2 ** 2` is −4, and less tightly than one on its right, so `2 ** -1` is 0.5. Comparisons bind less tightly than `&`, so `x & 1 == 0` works as intended. `1 < x < 10` is a chain of two tests. `not` binds less tightly than `==`, and `and` binds more tightly than `or`. The `ast` module shows the tree that the parser built.
