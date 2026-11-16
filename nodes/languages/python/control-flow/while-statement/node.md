---
tag: 1M5G6R
kind: feature
owner: python
slug: while-statement
name: The while statement
aliases: [while loop, while True loop, do while in Python, loop until a condition, while else]
area: languages/python/control-flow
level: intro
summary: >-
  A while loop runs its block again and again while its condition is true. It tests the condition before each pass. Python has no do-while loop, so use while True with an if and a break.
grammar: "while_stmt: 'while' assignment_expression ':' suite ['else' ':' suite] (Language Reference, 8.2)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#the-while-statement", locator: "8.2. The while statement"}
see_also: [python/for-statements, python/break-and-continue-statements, python/else-clauses-on-loops, python/if-statements]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-while-statement
    title: The Python Language Reference, Python 3.15
    locator: 8.2. The while statement
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0315/
    title: PEP 315 - Enhanced While Loop
    locator: Abstract; Notice
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#more-on-conditions
    title: The Python Tutorial, 5. Data Structures
    locator: 5.7. More on Conditions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_statements
    title: The Go Programming Language Specification
    locator: For statements; For statements with single condition
    accessed: 2026-11-16
---

## Synopsis

```python run id=fib
a, b = 0, 1
result = []
while a < 50:
    result.append(a)
    a, b = b, a + b
print(result)
```

```output for=fib
[0, 1, 1, 2, 3, 5, 8, 13, 21, 34]
```

The loop runs while `a < 50` is true. It stops when the test is false.

## Semantics

**r1.** The while statement "is used for repeated execution as long as an expression is true". It "repeatedly tests the expression and, if it is true, executes the first suite; if the expression is false (which may be the first time it is tested) the suite of the else clause, if present, is executed and the loop terminates."[^1] Examples: [`fib`](#fib), [`zero-passes`](#zero-passes).

**r2.** "A break statement executed in the first suite terminates the loop without executing the else clause’s suite. A continue statement executed in the first suite skips the rest of the suite and goes back to testing the expression."[^1] Example: [`do-while`](#do-while).

**r3.** "The conditions used in while and if statements can contain any operators, not just comparisons."[^3] A non-empty container is true, so `while items:` loops until the container is empty. Example: [`drain`](#drain).

**r4.** Python has no do-while loop. PEP 315 proposed one, and the PEP was rejected. It says: "Users of the language are advised to use the while-True form with an inner if-break when a do-while loop would have been appropriate."[^2] Example: [`do-while`](#do-while).

## Examples

```python run id=drain
stack = [1, 2, 3]
while stack:
    print("pop", stack.pop())
print("empty:", stack)
```

```output for=drain
pop 3
pop 2
pop 1
empty: []
```

```python run id=do-while
attempts = iter(["", "x", "yes"])
while True:
    answer = next(attempts)
    print("got", repr(answer))
    if answer == "yes":
        break
```

```output for=do-while
got ''
got 'x'
got 'yes'
```

The body runs at least once, and the test is at the end of the body (rule r4).

```python run id=zero-passes
n = 0
while n > 0:
    print("never")
else:
    print("condition false at the first test")
```

```output for=zero-passes
condition false at the first test
```

## Pitfalls

A loop whose condition never becomes false does not stop. For example, `x = 0.0` followed by `while x != 1.0: x += 0.1` runs forever, because the sum of ten `0.1` values is not exactly `1.0`:

```python wrong id=float-test
x = 0.0
for _ in range(10):
    x += 0.1
print(x, x == 1.0)
```

```output for=float-test
0.9999999999999999 False
```

Use a test such as `x < 1.0`, or count with integers.

## Comparison

Go has no while keyword. Its for statement has a simplest form with a single condition, which "specifies the repeated execution of a block as long as a boolean condition evaluates to true. The condition is evaluated before each iteration."[^4] So `for a < b { ... }` in Go is the same as `while a < b:` in Python. In Go, "If the condition is absent, it is equivalent to the boolean value true", so `for { ... }` is the Go form of `while True:`.[^4]
