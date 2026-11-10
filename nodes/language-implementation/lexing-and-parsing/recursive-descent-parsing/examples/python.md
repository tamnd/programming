```python run id=parser
import re
from dataclasses import dataclass

TOKEN = re.compile(r"\s*(?:(?P<num>\d+)|(?P<op>[-+*/^()])|(?P<bad>\S))")


@dataclass
class Num:
    value: int


@dataclass
class Neg:
    operand: object


@dataclass
class Bin:
    op: str
    left: object
    right: object


def tokenize(src):
    tokens = []
    for m in TOKEN.finditer(src):
        if m["bad"]:
            raise SyntaxError(f"unexpected character {m['bad']!r} at column {m.start('bad') + 1}")
        kind = "num" if m["num"] else m["op"]
        tokens.append((kind, m[m.lastgroup], m.start(m.lastgroup)))
    tokens.append(("end", "", len(src)))
    return tokens


class Parser:
    def __init__(self, src):
        self.tokens = tokenize(src)
        self.i = 0

    def peek(self):
        return self.tokens[self.i][0]

    def take(self):
        self.i += 1
        return self.tokens[self.i - 1]

    def fail(self, expected):
        raise SyntaxError(f"expected {expected} at column {self.tokens[self.i][2] + 1}")

    def parse(self):
        tree = self.expr()
        if self.peek() != "end":
            self.fail("an operator")
        return tree

    def expr(self):
        left = self.term()
        while self.peek() in ("+", "-"):
            left = Bin(self.take()[0], left, self.term())
        return left

    def term(self):
        left = self.unary()
        while self.peek() in ("*", "/"):
            left = Bin(self.take()[0], left, self.unary())
        return left

    def unary(self):
        if self.peek() != "-":
            return self.power()
        self.take()
        return Neg(self.unary())

    def power(self):
        base = self.atom()
        if self.peek() != "^":
            return base
        self.take()
        return Bin("^", base, self.unary())

    def atom(self):
        kind, text, _ = self.tokens[self.i]
        if kind == "num":
            self.take()
            return Num(int(text))
        if kind == "(":
            self.take()
            inner = self.expr()
            if self.peek() != ")":
                self.fail("')'")
            self.take()
            return inner
        self.fail("a number or '('")


def show(node):
    match node:
        case Num(value):
            return str(value)
        case Neg(operand):
            return f"(-{show(operand)})"
        case Bin(op, left, right):
            return f"({show(left)} {op} {show(right)})"


def evaluate(node):
    match node:
        case Num(value):
            return value
        case Neg(operand):
            return -evaluate(operand)
        case Bin("+", l, r):
            return evaluate(l) + evaluate(r)
        case Bin("-", l, r):
            return evaluate(l) - evaluate(r)
        case Bin("*", l, r):
            return evaluate(l) * evaluate(r)
        case Bin("/", l, r):
            return evaluate(l) // evaluate(r)
        case Bin("^", l, r):
            return evaluate(l) ** evaluate(r)


for src in ["1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2"]:
    try:
        tree = Parser(src).parse()
        print(f"{src} -> {show(tree)} = {evaluate(tree)}")
    except SyntaxError as e:
        print(f"{src} -> error: {e}")

print(Parser("1 - 2 - 3").parse())
```

```output for=parser
1 + 2 * 3 -> (1 + (2 * 3)) = 7
1 - 2 - 3 -> ((1 - 2) - 3) = -4
2 ^ 3 ^ 2 -> (2 ^ (3 ^ 2)) = 512
-2 ^ 2 -> (-(2 ^ 2)) = -4
(1 + 2) * 3 -> ((1 + 2) * 3) = 9
1 + * 2 -> error: expected a number or '(' at column 5
(1 + 2 -> error: expected ')' at column 7
1 $ 2 -> error: unexpected character '$' at column 3
Bin(op='-', left=Bin(op='-', left=Num(value=1), right=Num(value=2)), right=Num(value=3))
```

- Verdict: idiom. This version separates the two jobs. The parser builds an [[abstract-syntax-tree]] from dataclasses, and two functions walk the tree with `match`: one prints it and one evaluates it. The evaluator is a small [[tree-walking-interpreter]].
- The tokenizer is one regular expression with named groups. The group `bad` matches any other character, so an unknown character becomes an error and is never skipped.
- The last line prints the tree of `1 - 2 - 3`. The first subtraction is the left child of the second, which is what left associativity means.
