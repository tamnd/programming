```python run id=interpreter
from dataclasses import dataclass


@dataclass
class Num:
    value: int


@dataclass
class Var:
    name: str


@dataclass
class Bin:
    op: str
    left: object
    right: object


@dataclass
class Let:
    name: str
    init: object


@dataclass
class Assign:
    name: str
    value: object


@dataclass
class Block:
    body: list


@dataclass
class While:
    cond: object
    body: object


@dataclass
class Print:
    value: object


class Env:
    def __init__(self, parent=None):
        self.vars = {}
        self.parent = parent


class Interpreter:
    """Dispatch by method name, in the style of ast.NodeVisitor."""

    OPS = {
        "+": lambda a, b: a + b,
        "-": lambda a, b: a - b,
        "*": lambda a, b: a * b,
        "<": lambda a, b: int(a < b),
    }

    def __init__(self):
        self.nodes = self.lookups = self.links = 0

    def visit(self, node, env):
        self.nodes += 1
        method = getattr(self, "visit_" + type(node).__name__)
        return method(node, env)

    def find(self, env, name):
        self.lookups += 1
        while env is not None:
            if name in env.vars:
                return env
            self.links += 1
            env = env.parent
        raise NameError(f"undefined variable {name!r}")

    def visit_Num(self, node, env):
        return node.value

    def visit_Var(self, node, env):
        return self.find(env, node.name).vars[node.name]

    def visit_Bin(self, node, env):
        a = self.visit(node.left, env)
        b = self.visit(node.right, env)
        return self.OPS[node.op](a, b)

    def visit_Let(self, node, env):
        env.vars[node.name] = self.visit(node.init, env)

    def visit_Assign(self, node, env):
        result = self.visit(node.value, env)
        self.find(env, node.name).vars[node.name] = result

    def visit_Block(self, node, env):
        inner = Env(env)
        for stmt in node.body:
            self.visit(stmt, inner)

    def visit_While(self, node, env):
        while self.visit(node.cond, env) != 0:
            self.visit(node.body, env)

    def visit_Print(self, node, env):
        print(self.visit(node.value, env))


program = Block([
    Let("n", Num(5)),
    Let("acc", Num(1)),
    While(Bin("<", Num(0), Var("n")), Block([
        Let("step", Var("n")),
        Assign("acc", Bin("*", Var("acc"), Var("step"))),
        Assign("n", Bin("-", Var("n"), Num(1))),
    ])),
    Print(Var("acc")),
])

interp = Interpreter()
interp.visit(program, Env())
print(f"nodes visited: {interp.nodes}")
print(f"variable lookups: {interp.lookups}, parent links followed: {interp.links}")
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

- Verdict: idiom. The main page dispatches with `match`. This version looks up a method named `visit_` plus the class name of the node, which is how `ast.NodeVisitor` dispatches. One method per node type keeps each case short.
- `getattr` raises `AttributeError` for a node type that has no method. Nothing checks the cases before the run.
- The operators sit in a table of small functions. Adding an operator adds one entry, and the dispatch code does not change.
