---
tag: DMTCH5
kind: feature
owner: python
slug: for-statements
name: For Statements
aliases: [for loop, for in loop, foreach in Python, iterate over a list, for loop with range, loop variable after the loop]
area: languages/python/control-flow
level: intro
summary: >-
  A for loop takes the items of an iterable, such as a list, a string or a range, one at a time. It assigns each item to the target and runs the block. The loop variable keeps its last value.
grammar: "for_stmt: 'for' target_list 'in' starred_expression_list ':' suite ['else' ':' suite] (Language Reference, 8.3)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#the-for-statement", locator: "8.3. The for statement"}
see_also: [python/while-statement, python/looping-techniques, python/break-and-continue-statements, python/else-clauses-on-loops]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#for-statements
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.2. for Statements; 4.3. The range() Function
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-for-statement
    title: The Python Language Reference, Python 3.15
    locator: 8.3. The for statement
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_statements
    title: The Go Programming Language Specification
    locator: For statements with for clause; For statements with range clause
    accessed: 2026-11-16
---

## Synopsis

```python run id=words
words = ["cat", "window", "defenestrate"]
for w in words:
    print(w, len(w))
```

```output for=words
cat 3
window 6
defenestrate 12
```

This is the first example of the tutorial. The loop runs its block once for each item of the list.

## Semantics

**r1.** "Python’s for statement iterates over the items of any sequence (a list or a string), in the order that they appear in the sequence."[^1] It does not count with a start, a step and a stop, as the for loop of C does.[^1] Example: [`words`](#words).

**r2.** "The starred_expression_list expression is evaluated once; it should yield an iterable object. An iterator is created for that iterable. The first item provided by the iterator is then assigned to the target list using the standard rules for assignments".[^2] So the target can be a list of names, and the loop unpacks each item. Example: [`unpack`](#unpack).

**r3.** "When the iterator is exhausted, the suite in the else clause, if present, is executed, and the loop terminates."[^2]

**r4.** "The for-loop makes assignments to the variables in the target list. This overwrites all previous assignments to those variables including those made in the suite of the for-loop".[^2] Example: [`overwrite`](#overwrite).

**r5.** "Names in the target list are not deleted when the loop is finished, but if the sequence is empty, they will not have been assigned to at all by the loop."[^2] Example: [`after-loop`](#after-loop).

**r6.** To loop over numbers, use `range()`: "The given end point is never part of the generated sequence".[^1] Example: [`range`](#range).

## Examples

```python run id=unpack
pairs = [("a", 1), ("b", 2)]
for letter, number in pairs:
    print(letter * number)
```

```output for=unpack
a
bb
```

```python run id=overwrite
for i in range(3):
    print(i)
    i = 10
```

```output for=overwrite
0
1
2
```

The assignment `i = 10` does not change the next item, because the loop assigns the next item to `i` (rule r4).

```python run id=after-loop
for i in range(3):
    pass
print("after the loop, i =", i)

for j in []:
    pass
print("j" in globals())
```

```output for=after-loop
after the loop, i = 2
False
```

```python run id=range
print(list(range(5)))
print(list(range(5, 10)))
print(list(range(0, 10, 3)))
print(list(range(-10, -100, -30)))
```

```output for=range
[0, 1, 2, 3, 4]
[5, 6, 7, 8, 9]
[0, 3, 6, 9]
[-10, -40, -70]
```

## Pitfalls

The tutorial warns that "Code that modifies a collection while iterating over that same collection can be tricky to get right."[^1] A dictionary raises an error when its size changes during the loop:

```python fails id=change-dict expect=exception:RuntimeError
users = {"Hans": "active", "Ana": "inactive"}
for user, status in users.items():
    if status == "inactive":
        del users[user]
```

Loop over a copy, as in `users.copy().items()`, or build a new dictionary. The tutorial gives both strategies.[^1]

A list does not raise an error, but the loop skips items:

```python wrong id=change-list
numbers = [1, 2, 2, 3]
for n in numbers:
    if n == 2:
        numbers.remove(n)
print(numbers)
```

```output for=change-list
[1, 2, 3]
```

The first `remove` moves the second `2` to the position that the loop has already passed, so the loop never sees it. Use `[n for n in numbers if n != 2]`.

## Comparison

Go has one loop keyword, for, with three forms. The Go specification says that the iteration "may be controlled by a single condition", a for clause or a range clause.[^3] The for clause form, `for i := 0; i < 10; i++`, counts as the C loop does. Python has no such form. The range clause form is close to the Python for loop. In Go, "Each iteration has its own separate declared variable (or variables)".[^3] A Python loop variable is one variable of the enclosing scope, and it keeps its value after the loop (rule r5).
