---
tag: 1706V8
kind: feature
owner: python
slug: augmented-assignment
name: Augmented assignment
aliases: [+= in Python, compound assignment, in-place operators, __iadd__, x += 1, Python has no ++]
area: languages/python/variables-and-scope
level: core
summary: >-
  x += y combines a binary operation and an assignment. The target is evaluated once, and a mutable object can change in place through __iadd__ and the other in-place methods.
grammar: "augmented_assignment_stmt: augtarget augop (expression_list | yield_expression); augtarget: identifier | attributeref | subscription; augop: '+=' | '-=' | '*=' | '@=' | '/=' | '//=' | '%=' | '**=' | '>>=' | '<<=' | '&=' | '^=' | '|=' (Language Reference, 7.2.1)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#augmented-assignment-statements", locator: "7.2.1. Augmented assignment statements"}
see_also: [python/assignment-statements, python/a-word-about-names-and-objects, python/iterable-unpacking]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#augmented-assignment-statements
    title: The Python Language Reference, Python 3.15
    locator: 7.2.1. Augmented assignment statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__iadd__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.8. Emulating numeric types, object.__iadd__()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#why-does-a-tuple-i-item-raise-an-exception-when-the-addition-works
    title: Programming FAQ, Python 3.15 documentation
    locator: Why does a_tuple[i] += ['item'] raise an exception when the addition works?
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Assignment_statements
    title: The Go Programming Language Specification
    locator: Assignment statements; IncDec statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
n = 10
n += 5
n //= 4
s = "ab"
s *= 2
bits = 0b1010
bits |= 0b0101
print(n, s, bin(bits))
```

```output for=basics
3 abab 0b1111
```

Each statement combines an operator with an assignment. Python has 13 forms, one for each operator in the grammar.

## Semantics

**r1.** "Augmented assignment is the combination, in a single statement, of a binary operation and an assignment statement".[^1] Example: [`basics`](#basics).

**r2.** "An augmented assignment evaluates the target (which, unlike normal assignment statements, cannot be an unpacking) and the expression list, performs the binary operation specific to the type of assignment on the two operands, and assigns the result to the original target. The target is only evaluated once."[^1] Examples: [`evaluated-once`](#evaluated-once), [`no-unpacking`](#no-unpacking).

**r3.** `x += 1` is like `x = x + 1`, "a similar, but not exactly equal effect. In the augmented version, x is only evaluated once. Also, when possible, the actual operation is performed in-place, meaning that rather than creating a new object and assigning that to the target, the old object is modified instead."[^1] Example: [`in-place`](#in-place).

**r4.** "Unlike normal assignments, augmented assignments evaluate the left-hand side before evaluating the right-hand side. For example, a[i] += f(x) first looks-up a[i], then it evaluates f(x) and performs the addition, and lastly, it writes the result back to a[i]."[^1] Example: [`evaluated-once`](#evaluated-once).

**r5.** The in-place methods, such as `__iadd__()`, "should attempt to do the operation in-place (modifying self) and return the result (which could be, but does not have to be, self). If a specific method is not defined, or if that method returns NotImplemented, the augmented assignment falls back to the normal methods." So "x += y is equivalent to x = x.__iadd__(y)" when `__iadd__()` exists.[^2] Example: [`custom-iadd`](#custom-iadd).

**r6.** The assignment part still runs after an in-place operation. An augmented assignment to an item of a tuple fails, because "we can't change what an element of a tuple points to".[^3] Example: [`tuple-item`](#tuple-item).

## Examples

```python run id=in-place
a = [1]
b = a
a += [2]
print(a, b, a is b)
c = (1,)
d = c
c += (2,)
print(c, d, c is d)
```

```output for=in-place
[1, 2] [1, 2] True
(1, 2) (1,) False
```

A list changes in place, so `b` sees the change. A tuple cannot change, so `c += (2,)` makes a new tuple and `d` keeps the old one (rule r3).

```python run id=evaluated-once
calls = []

def index():
    calls.append("index")
    return 0

def value():
    calls.append("value")
    return 5

row = [1]
row[index()] += value()
print(row, calls)
```

```output for=evaluated-once
[6] ['index', 'value']
```

The subscript `index()` runs once, and it runs before `value()` (rules r2 and r4).

```python run id=custom-iadd
class Bag:
    def __init__(self):
        self.items = []

    def __iadd__(self, item):
        self.items.append(item)
        return self

bag = Bag()
same = bag
bag += "x"
bag += "y"
print(bag.items, bag is same)
```

```output for=custom-iadd
['x', 'y'] True
```

`__iadd__()` changes the object and returns `self`, so the name `bag` is bound to the same object again (rule r5).

```python fails id=no-unpacking expect=exception:SyntaxError
a, b = 1, 2
a, b += 1, 1
```

## Pitfalls

An augmented assignment to a list in a tuple raises `TypeError`, but the list changes first:

```python wrong id=tuple-item
a_tuple = (["foo"], "bar")
try:
    a_tuple[0] += ["item"]
except TypeError as e:
    print(type(e).__name__)
print(a_tuple[0])
```

```output for=tuple-item
TypeError
['foo', 'item']
```

The FAQ calls this behavior "a bit more surprising, and even more surprising is the fact that even though there was an error, the append worked".[^3] Use `a_tuple[0].extend(["item"])` to change the list with no assignment.

`x += y` and `x = x + y` can give different results when another name refers to the same mutable object (example [`in-place`](#in-place)).

## Comparison

Go has the same form: "An assignment operation x op= y where op is a binary arithmetic operator is equivalent to x = x op (y) but evaluates x only once."[^4] Go also has `x++` and `x--`, which are statements equivalent to `x += 1` and `x -= 1`.[^4] The Python augop list has no `++` or `--`, so `x += 1` is the usual form. In Python, a class can also define how `+=` works with `__iadd__()` (rule r5).
