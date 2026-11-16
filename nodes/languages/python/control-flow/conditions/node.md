---
tag: F1N2T1
kind: feature
owner: python
slug: conditions
name: Conditions
aliases: [More on Conditions, Boolean conditions, truthiness, chained comparisons, short-circuit evaluation, and or not]
area: languages/python/control-flow
level: intro
summary: >-
  A condition in an if or while statement can be any expression. Comparisons can be chained, and and and or stop as soon as the result is known. They return the last value that they evaluated.
grammar: "or_test: and_test | or_test 'or' and_test; and_test: not_test | and_test 'and' not_test; not_test: comparison | 'not' not_test (Language Reference, 6.11)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#boolean-operations", locator: "6.10. Comparisons; 6.11. Boolean operations"}
see_also: [python/if-statements, python/while-statement, python/assignment-expressions]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#more-on-conditions
    title: The Python Tutorial, 5. Data Structures
    locator: 5.7. More on Conditions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#boolean-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.10. Comparisons; 6.11. Boolean operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#truth-value-testing
    title: Built-in Types, Python 3.15 documentation
    locator: Truth Value Testing
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Logical_operators
    title: The Go Programming Language Specification
    locator: Comparison operators; Logical operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=chain
x = 5
print(1 < x < 10, 1 < x > 10)
print(x in [1, 5, 9], x is not None)
print(not x or x > 3)
```

```output for=chain
True False
True True
True
```

`1 < x < 10` means `1 < x and x < 10`. `in` tests membership, and `is not` tests identity.

## Semantics

**r1.** "The conditions used in while and if statements can contain any operators, not just comparisons."[^1]

**r2.** "The comparison operators in and not in are membership tests that determine whether a value is in (or not in) a container. The operators is and is not compare whether two objects are really the same object."[^1] Example: [`chain`](#chain).

**r3.** "Comparisons can be chained arbitrarily, e.g., x < y <= z is equivalent to x < y and y <= z, except that y is evaluated only once (but in both cases z is not evaluated at all when x < y is found to be false)."[^2] Example: [`chain`](#chain).

**r4.** "between them, not has the highest priority and or the lowest, so that A and not B or C is equivalent to (A and (not B)) or C."[^1]

**r5.** "The Boolean operators and and or are so-called short-circuit operators: their arguments are evaluated from left to right, and evaluation stops as soon as the outcome is determined."[^1] Example: [`short-circuit`](#short-circuit).

**r6.** "When used as a general value and not as a Boolean, the return value of a short-circuit operator is the last evaluated argument."[^1] But "Because not has to create a new value, it returns a boolean value regardless of the type of its argument".[^2] Example: [`last-value`](#last-value).

**r7.** "By default, an object is considered true unless its class defines either a __bool__() method that returns False or a __len__() method that returns zero, when called with the object."[^3] Example: [`custom-truth`](#custom-truth).

## Examples

```python run id=short-circuit
def check(name, result):
    print("evaluate", name)
    return result

if check("A", True) and check("B", False) and check("C", True):
    print("all true")
print("done")
```

```output for=short-circuit
evaluate A
evaluate B
done
```

`B` is false, so Python does not evaluate `C` (rule r5).

```python run id=last-value
string1, string2, string3 = "", "Trondheim", "Hammer Dance"
non_null = string1 or string2 or string3
print(repr(non_null))
print(repr(0 and "x"), repr("" or 0), repr(not "foo"))
```

```output for=last-value
'Trondheim'
0 0 False
```

The first line is the example of the tutorial (rule r6).

```python run id=custom-truth
class Bag:
    def __init__(self, items):
        self.items = items

    def __len__(self):
        return len(self.items)

print(bool(Bag([])), bool(Bag([1])))
```

```output for=custom-truth
False True
```

## Pitfalls

`or` returns a value, so it can hide a valid false value:

```python wrong id=or-default
def show(count=None):
    count = count or 10
    print(count)

show(0)
```

```output for=or-default
10
```

`0` is false, so `count or 10` gives `10`. Test for `None` with `is`: `if count is None: count = 10`.

The tutorial also notes that "in Python, unlike C, assignment inside expressions must be done explicitly with the walrus operator :=."[^1] So `if x = 1:` is a SyntaxError.

## Comparison

In Go, "Logical operators apply to boolean values and yield a result of the same type as the operands." "The left operand is evaluated, and then the right if the condition requires it."[^4] So `&&` and `||` short-circuit as `and` and `or` do, but a Go operand must be a boolean, and the result is a boolean. Go comparisons cannot be chained, because "Comparison operators compare two operands and yield an untyped boolean value."[^4] A Go `a < b < c` compares a boolean with `c`, and it does not compile when `c` is a number.
