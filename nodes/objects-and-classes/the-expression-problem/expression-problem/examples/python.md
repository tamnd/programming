```python run id=classes
from abc import ABC, abstractmethod


class Expr(ABC):  # object-oriented: each operation is a method
    @abstractmethod
    def evaluate(self): ...

    @abstractmethod
    def show(self): ...


class Num(Expr):
    def __init__(self, value):
        self.value = value

    def evaluate(self):
        return self.value

    def show(self):
        return str(self.value)


class Add(Expr):
    def __init__(self, left, right):
        self.left, self.right = left, right

    def evaluate(self):
        return self.left.evaluate() + self.right.evaluate()

    def show(self):
        return f"({self.left.show()} + {self.right.show()})"


class Neg(Expr):  # a new case: one new class, no other change
    def __init__(self, inner):
        self.inner = inner

    def evaluate(self):
        return -self.inner.evaluate()

    def show(self):
        return f"-{self.inner.show()}"


e = Add(Num(1), Neg(Num(5)))
print(e.show(), "=", e.evaluate())


class Mul(Expr):  # a case that forgets one operation
    def __init__(self, left, right):
        self.left, self.right = left, right

    def evaluate(self):
        return self.left.evaluate() * self.right.evaluate()


try:
    Mul(Num(2), Num(3))
except TypeError as err:
    print(err)
```

```output for=classes
(1 + -5) = -4
Can't instantiate abstract class Mul without an implementation for abstract method 'show'
```

```python wrong id=missing-case
from dataclasses import dataclass


@dataclass
class Num:
    value: int


@dataclass
class Add:
    left: object
    right: object


@dataclass
class Neg:  # a case added after evaluate was written
    inner: object


def evaluate(e):  # functional: each operation is a function over all cases
    match e:
        case Num(v):
            return v
        case Add(l, r):
            return evaluate(l) + evaluate(r)
    # no case for Neg: the function returns None


print(evaluate(Add(Num(1), Num(2))))
print(evaluate(Neg(Num(5))))
```

```output for=missing-case
3
None
```

- Verdict: idiom. Python has no construct for the expression problem, but its dynamic types make both directions possible. A class hierarchy makes cases easy to add, and an abstract method makes a new class fail when it is created if it omits an operation, as `Mul` shows. `functools.singledispatch` gives an open generic function: any module can register a new type for an existing operation, or create a new operation for existing types. All the checks happen at run time.
- The wrong block writes the operation as a function with `match`. A case that the function does not handle falls out of the `match` and returns `None`, and the error appears later, far from its cause. End such a function with `case _: raise TypeError(...)`, or use `typing.assert_never` so that a type checker reports the missing case.
