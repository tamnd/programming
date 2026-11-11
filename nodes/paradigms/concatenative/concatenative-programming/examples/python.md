```python run id=composition
from functools import reduce

# A word is a pure function from a stack (a tuple) to a new stack.
def push(n):
    return lambda s: s + (n,)


def lift2(f):
    return lambda s: s[:-2] + (f(s[-2], s[-1]),)


dup = lambda s: s + (s[-1],)
add = lift2(lambda a, b: a + b)
mul = lift2(lambda a, b: a * b)


def compose(*words):
    return reduce(lambda f, g: lambda s: g(f(s)), words, lambda s: s)


program = [push(2), push(3), add, dup, mul, push(1), add]  # (2 + 3) squared, plus 1
print(compose(*program)(()))

# Cut the program at every point: the two halves composed give the same result.
results = {compose(compose(*program[:k]), compose(*program[k:]))(())
           for k in range(len(program) + 1)}
print(results)
```

```output for=composition
(26,)
{(26,)}
```

```python wrong id=operand-order
def run(text):
    s = []
    for t in text.split():
        if t in "-*":
            b, a = s.pop(), s.pop()
            s.append(a - b if t == "-" else a * b)
        else:
            s.append(int(t))
    return s


# Intended: 10 minus the product of 2 and 3.
print(run("2 3 * 10 -"))
```

```output for=operand-order
[-4]
```

- Verdict: absent. Python applies functions to named arguments. The first block builds the concatenative model from closures: each word is a pure function from a tuple to a tuple, a program is the composition of its words, and the set of results shows that a cut at any point gives the same answer. CPython itself runs a stack machine: its bytecode pushes and pops values, as `dis.dis` shows.
- The wrong block computes 6 minus 10, not 10 minus 6. `-` takes the item under the top as its left operand, so the 10 must be pushed before the product: `10 2 3 * -` gives 4. A reader must track the stack to see the order of operands, which is the main cost of the style.
