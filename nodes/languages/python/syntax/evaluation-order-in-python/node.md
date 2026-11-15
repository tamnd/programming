---
tag: ZKRK34
kind: feature
owner: python
slug: evaluation-order-in-python
name: Evaluation order in Python
aliases: [Python order of evaluation, left-to-right evaluation, operand evaluation order, side effects order]
area: languages/python/syntax
level: core
summary: >-
  Python evaluates the operands of an expression from left to right, whatever the precedence. In an assignment the right-hand side comes first; in an augmented assignment the target is looked up first.
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#evaluation-order", locator: "6.16 Evaluation order"}
see_also: [python/operator-precedence-in-python, python/boolean-operators, python/conditional-expressions, python/atoms-and-primaries]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#evaluation-order
    title: The Python Language Reference, Python 3.15
    locator: 6.16 Evaluation order
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#the-power-operator
    title: The Python Language Reference, Python 3.15
    locator: 6.5 The power operator
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#comprehensions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.6 Comprehensions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#assignment-statements
    title: The Python Language Reference, Python 3.15
    locator: 7.2 Assignment statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#augmented-assignment-statements
    title: The Python Language Reference, Python 3.15
    locator: 7.2.1 Augmented assignment statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#boolean-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.11 Boolean operations
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Order_of_evaluation
    title: The Go Programming Language Specification
    locator: Order of evaluation
    accessed: 2026-11-16
---

## Synopsis

```python run id=eval-order
def v(label, value):
    print("eval", label)
    return value

print(v("a", 1) + v("b", 2) * v("c", 3))
```

```output for=eval-order
eval a
eval b
eval c
7
```

The multiplication binds first, but `a` is still evaluated first.

## Semantics

**r1.** "Python evaluates expressions from left to right." The reference lists forms whose parts are evaluated in the order `expr1`, `expr2`, and so on: a tuple, a dict display `{expr1: expr2, expr3: expr4}`, an arithmetic expression `expr1 + expr2 * (expr3 - expr4)`, and a call `expr1(expr2, expr3, *expr4, **expr5)`.[^1] Example: [`call-order`](#call-order).

**r2.** Precedence decides the grouping, not the order of the operands. Even `**`, which groups from right to left, does not "constrain the evaluation order for the operands".[^2] Example: [`power-operands`](#power-operands).

**r3.** In a dict display the key is evaluated before its value (rule r1). The same holds in a dict comprehension; the reference notes that before version 3.8 this order was not well-defined there, and CPython evaluated the value first.[^1] [^3] Example: [`dict-order`](#dict-order).

**r4.** The reference notes that "while evaluating an assignment, the right-hand side is evaluated before the left-hand side".[^1] The statement then assigns the resulting object to each target list, "from left to right".[^4] Example: [`assign-order`](#assign-order).

**r5.** The targets of one assignment are assigned in turn, so a later target can see an earlier one: in `i, x[i] = 1, 2`, `i` is updated first and then `x[i]`.[^4] Example: [`overlap-targets`](#overlap-targets).

**r6.** "Unlike normal assignments, augmented assignments evaluate the left-hand side *before* evaluating the right-hand side": `a[i] += f(x)` looks up `a[i]`, then evaluates `f(x)`, then writes back to `a[i]`.[^5] Example: [`augmented-order`](#augmented-order).

**r7.** `and` and `or` evaluate their operands from left to right, but skip the right operand when the left one already decides the result.[^6] Example: [`skip-operands`](#skip-operands).

## Examples

A call evaluates the function first, then the arguments in order (rule r1).

```python run id=call-order
def v(label, value):
    print("eval", label)
    return value

v("func", print)(v("arg1", 1), v("arg2", 2), *v("star", [3]), **v("kw", {"sep": "-"}))
```

```output for=call-order
eval func
eval arg1
eval arg2
eval star
eval kw
1-2-3
```

The operands of a `**` chain still run left to right (rule r2).

```python run id=power-operands
def v(label, value):
    print("eval", label)
    return value

print(v("a", 2) ** v("b", 3) ** v("c", 2))
```

```output for=power-operands
eval a
eval b
eval c
512
```

Keys before values (rule r3).

```python run id=dict-order
def v(label, value):
    print("eval", label)
    return value

print({v("k1", "a"): v("v1", 1), v("k2", "b"): v("v2", 2)})
print({v("key", n): v("val", n * n) for n in [3]})
```

```output for=dict-order
eval k1
eval v1
eval k2
eval v2
{'a': 1, 'b': 2}
eval key
eval val
{3: 9}
```

The value is computed before the subscript of the target (rule r4).

```python run id=assign-order
def v(label, value):
    print("eval", label)
    return value

data = {}
data[v("key", "k")] = v("value", 1)
print(data)
```

```output for=assign-order
eval value
eval key
{'k': 1}
```

An augmented assignment looks up the target first (rule r6).

```python run id=augmented-order
def v(label, value):
    print("eval", label)
    return value

a = [10]
a[v("index", 0)] += v("rhs", 5)
print(a)
```

```output for=augmented-order
eval index
eval rhs
[15]
```

`and` and `or` stop early (rule r7).

```python run id=skip-operands
def v(label, value):
    print("eval", label)
    return value

print(v("a", 0) and v("b", 1), v("c", 2) or v("d", 3))
```

```output for=skip-operands
eval a
eval c
0 2
```

## Pitfalls

### Targets that depend on each other

The reference's own example: `i` is assigned before `x[i]` is evaluated (rule r5), so `x[1]` changes, not `x[0]`.

```python wrong id=overlap-targets
x = [0, 1]
i = 0
i, x[i] = 1, 2
print(x)
```

```output for=overlap-targets
[0, 2]
```

### Evaluated first is not printed first

An argument is evaluated in order, but a mutable object is printed when the call runs. Here `items` is evaluated before `append`, yet it is the same list, so it shows the appended item.

```python wrong id=mutated-argument
items = []
print(items, items.append(1), items)
```

```output for=mutated-argument
[1] None [1]
```

## Comparison

Go also evaluates function calls, method calls, receive operations and binary logical operations "in lexical left-to-right order", but "the order of those events compared to the evaluation and indexing of x and the evaluation of y and z is not specified" in its example assignment.[^7] Python specifies the order of every operand (rule r1), so code whose operands have side effects gives the same result on every implementation.
