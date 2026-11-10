```python run id=parse-walk
import ast
import operator

tree = ast.parse("1 + 2 * (3 - x)", mode="eval")


def show(node, depth=0):
    label = type(node).__name__
    if isinstance(node, ast.Constant):
        label += f" {node.value}"
    elif isinstance(node, ast.Name):
        label += f" {node.id}"
    elif isinstance(node, ast.BinOp):
        label += f" {type(node.op).__name__}"
    print("  " * depth + label)
    for child in (node.left, node.right) if isinstance(node, ast.BinOp) else ():
        show(child, depth + 1)


class Names(ast.NodeVisitor):
    def __init__(self):
        self.names = []

    def visit_Name(self, node):
        self.names.append(node.id)


OPS = {ast.Add: operator.add, ast.Sub: operator.sub, ast.Mult: operator.mul}


def evaluate(node, env):
    match node:
        case ast.Constant(value=value):
            return value
        case ast.Name(id=name):
            return env[name]
        case ast.BinOp(left=left, op=op, right=right):
            return OPS[type(op)](evaluate(left, env), evaluate(right, env))
    raise ValueError(f"unsupported node {type(node).__name__}")


show(tree.body)
print("nodes:", sum(isinstance(n, ast.expr) for n in ast.walk(tree)))
finder = Names()
finder.visit(tree)
print("names:", finder.names)
print("value at x = 1:", evaluate(tree.body, {"x": 1}))
print("compiled:", eval(compile(tree, "<expr>", "eval"), {"x": 1}))
```

```output for=parse-walk
BinOp Add
  Constant 1
  BinOp Mult
    Constant 2
    BinOp Sub
      Constant 3
      Name x
nodes: 7
names: ['x']
value at x = 1: 5
compiled: 5
```

- Verdict: stdlib. `ast.parse` returns the tree that the CPython compiler uses, and `ast.NodeVisitor` walks it.
- The tree has no node for the parentheses. The operator of a `BinOp` is itself a node, such as `Add()`, so the example counts only expression nodes.
- `compile` accepts the tree in place of source text. A program can build or change a tree and then run it.
- The abstract grammar can change between Python versions, so code that matches on node fields may need updates.
