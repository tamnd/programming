```python run id=engine
from dataclasses import dataclass


# Values carry their type, as boxed values do in a dynamic language.
@dataclass(frozen=True)
class Int:
    n: int
    kind = "int"

    def __str__(self):
        return str(self.n)


@dataclass(frozen=True)
class Float:
    x: float
    kind = "float"

    def __str__(self):
        return str(self.x)


# The body of f(x) = x * x + 1 as a tree.
@dataclass(frozen=True)
class Num:
    value: int


@dataclass(frozen=True)
class Arg:
    pass


@dataclass(frozen=True)
class Bin:
    op: str
    left: object
    right: object


OPS = {"+": lambda a, b: a + b, "*": lambda a, b: a * b}


def arith(op, a, b):
    """Generic arithmetic: check both tags on every operation."""
    if isinstance(a, Int) and isinstance(b, Int):
        return Int(OPS[op](a.n, b.n))
    fa = float(a.n) if isinstance(a, Int) else a.x
    fb = float(b.n) if isinstance(b, Int) else b.x
    return Float(OPS[op](fa, fb))


def interpret(node, arg):
    """The interpreter: walk the tree on every call."""
    match node:
        case Num(value):
            return Int(value)
        case Arg():
            return arg
        case Bin(op, left, right):
            return arith(op, interpret(left, arg), interpret(right, arg))


def compile_int(node):
    """Compiled code for int: closures over raw ints, no tag checks inside."""
    match node:
        case Num(value):
            return lambda n: value
        case Arg():
            return lambda n: n
        case Bin(op, left, right):
            f, l, r = OPS[op], compile_int(left), compile_int(right)
            return lambda n: f(l(n), r(n))


def compile_generic(node):
    """Compiled code for any argument: closures that still check tags."""
    match node:
        case Num(value):
            boxed = Int(value)
            return lambda a: boxed
        case Arg():
            return lambda a: a
        case Bin(op, left, right):
            l, r = compile_generic(left), compile_generic(right)
            return lambda a: arith(op, l(a), r(a))


THRESHOLD = 3


class Function:
    def __init__(self, name, body):
        self.name, self.body = name, body
        self.calls = 0  # calls in the interpreter since the last compilation
        self.seen = set()  # the profile: the kinds of the arguments
        self.code = None  # compiled code, or None
        self.counts = {"interpreter": 0, "compiled": 0, "deoptimizations": 0}

    def compile(self):
        if self.seen == {"int"}:
            raw = compile_int(self.body)

            def code(arg):
                if not isinstance(arg, Int):  # the guard
                    return None
                return Int(raw(arg.n))  # unbox once, compute, box the result

            self.code, self.kind, how = code, "int", "specialized for int"
        else:
            self.code, self.kind, how = compile_generic(self.body), "generic", "generic"
        print(f"* compile {self.name}: arguments seen: {', '.join(sorted(self.seen))}; {how}")

    def __call__(self, arg):
        if self.code is None and self.calls >= THRESHOLD:
            self.compile()
        if self.code is not None:
            result = self.code(arg)
            if result is not None:
                self.counts["compiled"] += 1
                return result, f"compiled, {self.kind}"
            print(f"* deoptimize {self.name}: guard failed, {arg} is not an int")
            self.counts["deoptimizations"] += 1
            self.code, self.calls = None, 0
        self.calls += 1
        self.counts["interpreter"] += 1
        self.seen.add(arg.kind)
        return interpret(self.body, arg), f"interpreter, call {self.calls}"


f = Function("f", Bin("+", Bin("*", Arg(), Arg()), Num(1)))
for arg in [Int(1), Int(2), Int(3), Int(4), Float(2.5), Int(6), Int(7), Int(8)]:
    result, tier = f(arg)
    print(f"f({arg}) = {result} [{tier}]")
print(", ".join(f"{k}: {v}" for k, v in f.counts.items()))
```

```output for=engine
f(1) = 2 [interpreter, call 1]
f(2) = 5 [interpreter, call 2]
f(3) = 10 [interpreter, call 3]
* compile f: arguments seen: int; specialized for int
f(4) = 17 [compiled, int]
* deoptimize f: guard failed, 2.5 is not an int
f(2.5) = 7.25 [interpreter, call 1]
f(6) = 37 [interpreter, call 2]
f(7) = 50 [interpreter, call 3]
* compile f: arguments seen: float, int; generic
f(8) = 65 [compiled, generic]
interpreter: 6, compiled: 2, deoptimizations: 1
```

The second program shows the same idea inside CPython. The specializing interpreter of PEP 659 rewrites a generic instruction into a form for the types that it sees, and `dis.get_instructions(..., adaptive=True)` shows the rewritten instructions. The instruction names depend on the version of CPython that recorded the output.

```python run id=adaptive
import dis
import sys


def f(x):
    return x * x + 1


def binary_ops(func):
    return [i.opname for i in dis.get_instructions(func, adaptive=True) if i.opname.startswith("BINARY_OP")]


print(f"version: {sys.version_info.major}.{sys.version_info.minor}")
print("before any call:", binary_ops(f))
for i in range(100):
    f(i)
print("after 100 int calls:", binary_ops(f))
for i in range(100):
    f(i + 0.5)
print("after 100 float calls:", binary_ops(f))
```

```output for=adaptive
version: 3.14
before any call: ['BINARY_OP', 'BINARY_OP']
after 100 int calls: ['BINARY_OP_MULTIPLY_INT', 'BINARY_OP_ADD_INT']
after 100 float calls: ['BINARY_OP_MULTIPLY_FLOAT', 'BINARY_OP_EXTEND']
```

- Verdict: idiom. CPython specializes instructions in its interpreter for the types that it sees (PEP 659). Its JIT compiler is experimental, is not part of the default build, and PEP 744 says that it should not be used in production while it is experimental.
- In the recorded `adaptive` output, the two `BINARY_OP` instructions become forms for `int` after the int calls. After the float calls they change again. This is the same cycle as the model: specialize for the types seen, see a type that does not fit, and specialize again.
- `dis` calls the bytecode an implementation detail. A program must not depend on the names of the specialized instructions.
- The model keeps the profile in a `set` and sorts it before it prints, because the order of a `set` of strings can change from run to run.
