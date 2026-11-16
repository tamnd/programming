---
tag: Y3QGX6
kind: feature
owner: python
slug: comprehension-scope
name: Comprehension scope
aliases: [comprehension variable leak, loop variable in a comprehension, walrus in a comprehension, comprehension in a class body, inlined comprehensions, PEP 709]
area: languages/python/collections
level: advanced
summary: >-
  A comprehension runs in its own nested scope, so its loop variables do not leak. The leftmost iterable is evaluated in the enclosing scope, and a := target binds in the containing scope.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#comprehensions", locator: "6.2.6. Comprehensions"}
see_also: [python/list-comprehensions, python/set-and-dict-comprehensions, python/generator-expressions, python/assignment-expressions, python/python-scopes-and-namespaces]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#comprehensions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.6. Comprehensions; 6.2.7. Generator expressions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names
    title: The Python Language Reference, 4. Execution model
    locator: 4.2.2. Resolution of names
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0572/
    title: "PEP 572: Assignment Expressions"
    locator: Scope of the target
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0709/
    title: "PEP 709: Inlined comprehensions"
    locator: Abstract; Specification
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
    accessed: 2026-11-16
---

## Synopsis

```python run id=no-leak
x = "old value"
squares = [x ** 2 for x in range(4)]
print(x, squares)
```

```output for=no-leak
old value [0, 1, 4, 9]
```

The `x` inside the comprehension is a different variable from the `x` outside it.

## Semantics

**r1.** "Aside from the iterable expression in the leftmost for clause, the comprehension is executed in a separate implicitly nested scope. This ensures that names assigned to in the target list don’t “leak” into the enclosing scope."[^1] Example: [`no-leak`](#no-leak).

**r2.** "The iterable expression in the leftmost for clause is evaluated directly in the enclosing scope and then passed as an argument to the implicitly nested scope."[^1] For a generator expression, "The iterable expression in the leftmost for clause is evaluated immediately".[^1] Example: [`leftmost`](#leftmost).

**r3.** An assignment expression in a comprehension "binds the target in the containing scope, honoring a nonlocal or global declaration for the target in that scope, if one exists."[^3] Example: [`witness`](#witness).

**r4.** "Assignment expressions are not allowed inside comprehension iterable expressions (that is, the expressions after the in keyword), nor anywhere within comprehensions that appear directly in a class definition."[^1] Example: [`walrus-iterable`](#walrus-iterable).

**r5.** "The scope of names defined in a class block is limited to the class block; it does not extend to the code blocks of methods. This includes comprehensions and generator expressions".[^2] Only the leftmost iterable can use a name from the class body (rule r2). Examples: [`class-body`](#class-body), [`class-name`](#class-name).

**r6.** PEP 709 changed how CPython runs list, set and dict comprehensions. It proposes "to inline list, dictionary, and set comprehensions into the code where they are defined, and provide the expected isolation by pushing/popping clashing locals on the stack."[^4] The rules r1 to r5 stay the same. "Generator expressions are currently not inlined in the reference implementation of this PEP."[^4]

## Examples

```python run id=leftmost
nums = [1, 2]
gen = (n * 10 for n in nums)
nums = [7, 8]
print(list(gen))
```

```output for=leftmost
[10, 20]
```

The generator got the first list when it was made (rule r2). A later change to the name `nums` has no effect.

```python run id=witness
lines = ["code", "# note", "more code"]
if any((comment := line).startswith("#") for line in lines):
    print("First comment:", comment)

total = 0
partial_sums = [total := total + v for v in [1, 2, 3]]
print(partial_sums, total)
```

```output for=witness
First comment: # note
[1, 3, 6] 6
```

Both examples are adapted from PEP 572. The name after `:=` is set in the enclosing scope, so the program can read it after the comprehension (rule r3).

```python run id=class-body
class Grid:
    size = 3
    cells = [i * 2 for i in range(size)]

print(Grid.cells)
```

```output for=class-body
[0, 2, 4]
```

`size` is in the leftmost iterable, so it is evaluated in the class body (rule r2).

## Pitfalls

Other parts of a comprehension in a class body cannot see the class names:

```python fails id=class-name expect=exception:NameError
class A:
    a = 42
    b = list(a + i for i in range(10))
```

The execution model reference gives this example as one that fails (rule r5). Move the value into the leftmost iterable, or compute it outside the class.

An assignment expression cannot be in the iterable part:

```python fails id=walrus-iterable expect=exception:SyntaxError
result = [i + 1 for i in (j := [1, 2, 3])]
```

Assign the list in a separate statement first (rule r4).

## Comparison

A Go `for` statement with a range clause declares its iteration variables with `:=`. The Go spec says: "The iteration variables may be declared by the "range" clause using a form of short variable declaration (:=). In this case their scope is the block of the "for" statement".[^5] So in both languages the loop variable does not exist after the loop. Go has no comprehensions and no rule like r3.
