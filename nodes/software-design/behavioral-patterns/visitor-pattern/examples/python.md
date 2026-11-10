```python run id=expr
from dataclasses import dataclass


@dataclass(frozen=True)
class Num:
    value: int


@dataclass(frozen=True)
class Add:
    left: "Expr"
    right: "Expr"


@dataclass(frozen=True)
class Mul:
    left: "Expr"
    right: "Expr"


type Expr = Num | Add | Mul


# Each operation is one function with a case for each kind of node.
def show(e: Expr) -> str:
    match e:
        case Num(value):
            return str(value)
        case Add(left, right):
            return f"({show(left)} + {show(right)})"
        case Mul(left, right):
            return f"({show(left)} * {show(right)})"


def evaluate(e: Expr) -> int:
    match e:
        case Num(value):
            return value
        case Add(left, right):
            return evaluate(left) + evaluate(right)
        case Mul(left, right):
            return evaluate(left) * evaluate(right)


def count(e: Expr) -> int:
    match e:
        case Num():
            return 1
        case Add(left, right) | Mul(left, right):
            return 1 + count(left) + count(right)


tree = Mul(Add(Num(2), Num(3)), Num(4))
print("show:", show(tree))
print("evaluate:", evaluate(tree))
print("nodes:", count(tree))
```

```output
show: ((2 + 3) * 4)
evaluate: 20
nodes: 5
```

The standard library has a visitor for Python's own syntax trees. `ast.NodeVisitor.visit` calls the method `visit_` plus the class name of the node, and `generic_visit` visits the children:

```python run id=node-visitor
import ast


class Counter(ast.NodeVisitor):
    def __init__(self):
        self.operators = []
        self.constants = 0

    def visit_BinOp(self, node):
        self.operators.append(type(node.op).__name__)
        self.generic_visit(node)  # go on into the left and right operands

    def visit_Constant(self, node):
        self.constants += 1


counter = Counter()
counter.visit(ast.parse("(2 + 3) * 4"))
print("operators:", counter.operators)
print("constants:", counter.constants)
```

```output
operators: ['Mult', 'Add']
constants: 3
```

- Verdict: idiom. Frozen dataclasses are the node types, and a `match` statement with class patterns gives each operation a case for each node type. The positional patterns such as `Add(left, right)` work because a dataclass sets `__match_args__`.
- The `type Expr = ...` statement names the union for a type checker. The interpreter does not check it, so a missing case returns `None` at run time. The node page shows this error and the fix with `assert_never`.
- `ast.NodeVisitor` dispatches on the name of the class with `getattr`, so no `accept` method is needed. A visitor overrides only the node types that it cares about.
- `functools.singledispatch` is another form. It picks a function by the type of the first argument.
