```python run id=weakest-precondition
import ast
import copy
import itertools


class Subst(ast.NodeTransformer):  # replace a variable by an expression
    def __init__(self, name, expr):
        self.name, self.expr = name, expr

    def visit_Name(self, node):
        return copy.deepcopy(self.expr) if node.id == self.name else node


def wp(program, post):  # Hoare's assignment rule, applied from the last line up
    cond = ast.parse(post, mode="eval").body
    for line in reversed(program):
        target, value = line.split(" = ")
        cond = Subst(target, ast.parse(value, mode="eval").body).visit(cond)
    return ast.unparse(cond)


swap = ["x = x + y", "y = x - y", "x = x - y"]
pre = wp(swap, "x == B and y == A")
print("{", pre, "}")
print("; ".join(swap))
print("{ x == B and y == A }")
same = all(eval(pre, {"x": a, "y": b, "A": a, "B": b})
           for a, b in itertools.product(range(-3, 4), repeat=2))
print("the precondition holds whenever x == A and y == B:", same)
```

```output for=weakest-precondition
{ x + y - (x + y - y) == B and x + y - y == A }
x = x + y; y = x - y; x = x - y
{ x == B and y == A }
the precondition holds whenever x == A and y == B: True
```

- Verdict: absent. Python has no formal semantics; its reference manual is written in English, and CPython is the reference implementation. The example applies the assignment rule of Hoare logic with the `ast` module: to find what must be true before `x = e`, replace `x` by `e` in the condition that must be true after it. Applied from the last line of a swap without a temporary variable, the rule gives a precondition that simplifies to `y == B and x == A`, and the program checks this on 49 pairs of values.
