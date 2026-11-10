```python run id=expr
from abc import ABC, abstractmethod


class UnknownVariable(Exception):
    pass


class Expr(ABC):
    """AbstractExpression: every node of the tree can evaluate and show itself."""

    @abstractmethod
    def evaluate(self, env: dict[str, int]) -> int: ...

    @abstractmethod
    def show(self) -> str: ...


class Num(Expr):  # TerminalExpression
    def __init__(self, value: int) -> None:
        self.value = value

    def evaluate(self, env: dict[str, int]) -> int:
        return self.value

    def show(self) -> str:
        return str(self.value)


class Var(Expr):  # TerminalExpression
    def __init__(self, name: str) -> None:
        self.name = name

    def evaluate(self, env: dict[str, int]) -> int:
        if self.name not in env:
            raise UnknownVariable(f"unknown variable {self.name}")
        return env[self.name]

    def show(self) -> str:
        return self.name


class Add(Expr):  # NonterminalExpression
    def __init__(self, left: Expr, right: Expr) -> None:
        self.left, self.right = left, right

    def evaluate(self, env: dict[str, int]) -> int:
        return self.left.evaluate(env) + self.right.evaluate(env)

    def show(self) -> str:
        return f"{self.left.show()} + {self.right.show()}"


class Mul(Expr):  # NonterminalExpression
    def __init__(self, left: Expr, right: Expr) -> None:
        self.left, self.right = left, right

    def evaluate(self, env: dict[str, int]) -> int:
        return self.left.evaluate(env) * self.right.evaluate(env)

    def show(self) -> str:
        return f"{wrap(self.left)} * {wrap(self.right)}"


def wrap(e: Expr) -> str:
    """A sum inside a product needs parentheses."""
    return f"({e.show()})" if isinstance(e, Add) else e.show()


def report(e: Expr, env: dict[str, int]) -> None:
    label = " ".join(f"{k}={v}" for k, v in env.items())
    try:
        print(f"{label}: {e.evaluate(env)}")
    except UnknownVariable as err:
        print(f"{label}: error: {err}")


# The client builds the tree. Parsing is a separate job.
e = Mul(Var("x"), Add(Var("y"), Num(2)))
print("expression:", e.show())
report(e, {"x": 3, "y": 4})
report(e, {"x": 5, "y": 1})

f = Add(Var("z"), Num(1))
print("expression:", f.show())
report(f, {"x": 3, "y": 4})
```

```output
expression: x * (y + 2)
x=3 y=4: 18
x=5 y=1: 15
expression: z + 1
x=3 y=4: error: unknown variable z
```

- Verdict: idiom. Python has no construct for the pattern. A class for each rule of the grammar, with an abstract base class, is the classic form.
- Each class has one method for each operation, `evaluate` and `show`. A third operation means a third method in all four classes.
- The `env` dictionary is the Context of the pattern. It holds the values of the variables, and every node can read it.
- Python can also write each operation as one function with a `match` statement over dataclasses (see [[python/match-statement]]). The TypeScript, Rust and Java examples show that form.
