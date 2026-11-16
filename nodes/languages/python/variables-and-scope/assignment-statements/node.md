---
tag: YGBMJF
kind: feature
owner: python
slug: assignment-statements
name: Assignment statements
aliases: [Python assignment, = in Python, chained assignment, multiple assignment, swap two variables in Python, slice assignment]
area: languages/python/variables-and-scope
level: intro
summary: >-
  An assignment statement evaluates the right side once and assigns the result to each target list, from left to right. A target is a name, an attribute, a subscription, a slice or a list of targets.
grammar: "assignment_stmt: (target_list '=')+ (starred_expression | yield_expression); target_list: target (',' target)* [',']; target: identifier | '(' [target_list] ')' | '[' [target_list] ']' | attributeref | subscription | '*' target (Language Reference, 7.2)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#assignment-statements", locator: "7.2. Assignment statements"}
see_also: [python/a-word-about-names-and-objects, python/iterable-unpacking, python/augmented-assignment, python/annotated-assignment, python/assignment-expressions]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#assignment-statements
    title: The Python Language Reference, Python 3.15
    locator: 7.2. Assignment statements
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Assignment_statements
    title: The Go Programming Language Specification
    locator: Assignment statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=forms
a = b = []
x, y = 1, 2
x, y = y, x
nums = [0, 1, 2, 3]
nums[0] = 10
nums[1:3] = ["a"]
print(a is b, x, y, nums)
```

```output for=forms
True 2 1 [10, 'a', 3]
```

## Semantics

**r1.** "Assignment statements are used to (re)bind names to values and to modify attributes or items of mutable objects".[^1] Example: [`forms`](#forms).

**r2.** "An assignment statement evaluates the expression list (remember that this can be a single expression or a comma-separated list, the latter yielding a tuple) and assigns the single resulting object to each of the target lists, from left to right."[^1] So `a = b = []` binds both names to one list. Example: [`forms`](#forms).

**r3.** If the target list has more than one target, "The object must be an iterable with the same number of items as there are targets in the target list, and the items are assigned, from left to right, to the corresponding targets."[^1] With one starred target, the starred target gets a list of the remaining items (see iterable unpacking). Examples: [`forms`](#forms), [`count-mismatch`](#count-mismatch).

**r4.** For a name, "If the name does not occur in a global or nonlocal statement in the current code block: the name is bound to the object in the current local namespace." Otherwise the name is bound in the global namespace or in the outer namespace that nonlocal names. "The name is rebound if it was already bound."[^1]

**r5.** For an attribute reference, the object "is then asked to assign the assigned object to the given attribute; if it cannot perform the assignment, it raises an exception (usually but not necessarily AttributeError)."[^1] On a class instance, the target `a.x` "is always set as an instance attribute, creating it if necessary."[^1] Example: [`class-attribute`](#class-attribute).

**r6.** For a subscription, Python calls the `__setitem__()` method of the object. For a list, the index must be in range: "If the index is out of range, IndexError is raised (assignment to a subscripted sequence cannot add new items to a list)." For a dictionary, the assignment replaces a key/value pair or inserts a new one.[^1] Examples: [`items`](#items), [`index-out-of-range`](#index-out-of-range).

**r7.** For a slice target, "the sequence object is asked to replace the slice with the items of the assigned sequence. The length of the slice may be different from the length of the assigned sequence, thus changing the length of the target sequence".[^1] Example: [`forms`](#forms).

**r8.** The right side is evaluated before the targets are assigned, so `a, b = b, a` swaps two variables. But "overlaps within the collection of assigned-to variables occur left-to-right, sometimes resulting in confusion."[^1] Example: [`overlap`](#overlap).

## Examples

```python run id=items
scores = {"ann": 1}
scores["bob"] = 2
scores["ann"] = 3
row = [0, 0, 0]
row[-1] = 9
print(scores, row)
```

```output for=items
{'ann': 3, 'bob': 2} [0, 0, 9]
```

A negative index counts from the end of the list (rule r6).

```python run id=class-attribute
class Cls:
    x = 3

inst = Cls()
inst.x = inst.x + 1
print(inst.x, Cls.x)
```

```output for=class-attribute
4 3
```

The right side reads the class attribute `Cls.x`. The target creates an instance attribute, and `Cls.x` does not change (rule r5).

```python fails id=index-out-of-range expect=exception:IndexError
row = [0, 0]
row[2] = 1
```

```python fails id=count-mismatch expect=exception:ValueError
a, b = [1, 2, 3]
```

## Pitfalls

Targets are assigned from left to right, so a later target can use a name that an earlier target just changed:

```python wrong id=overlap
x = [0, 1]
i = 0
i, x[i] = 1, 2
print(x)
```

```output for=overlap
[0, 2]
```

`i` becomes 1 first, and then `x[1]` becomes 2. The reference uses this program as its example (rule r8).

`a = b = []` does not make two lists. Both names refer to one list (rule r2). Use `a, b = [], []` to get two lists.

## Comparison

Go has tuple assignment too: "the number of operands on the left must equal the number of expressions on the right". But a Go assignment "proceeds in two phases. First, the operands of index expressions and pointer indirections (including implicit pointer indirections in selectors) on the left and the expressions on the right are all evaluated in the usual order. Second, the assignments are carried out in left-to-right order."[^2] So in Go, `i, x[i] = 1, 2` sets `x[0]`, because the index `i` is evaluated before `i` changes. In Python the same statement sets `x[1]` (rule r8). The Go grammar is `Assignment = ExpressionList assign_op ExpressionList .`, with one operator, so Go has no chained form such as `a = b = c`.[^2]
