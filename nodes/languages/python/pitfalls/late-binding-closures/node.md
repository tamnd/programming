---
tag: Y7Y2C7
kind: feature
owner: python
slug: late-binding-closures
name: Late-binding closures
aliases: [late binding, lambda in a loop, closures in a loop, loop variable capture, B023, function-uses-loop-variable, cell-var-from-loop]
area: languages/python/pitfalls
level: core
summary: >-
  A function made in a loop looks up the loop variable when it is called, not when it is made. So all the functions see the last value. Bind the value with a default argument or functools.partial.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names", locator: "4.2.2 Resolution of names"}
proposals: []
see_also: [python/closures-and-nonlocal, python/lambda-expressions, python/functools, python/naming-and-binding, python/comprehension-scope, python/mutable-default-argument, python/asyncio]
sources:
  - url: https://docs.python.org/3.15/faq/programming.html#why-do-lambdas-defined-in-a-loop-with-different-values-all-return-the-same-result
    title: Programming FAQ, Python 3.15 documentation
    locator: Why do lambdas defined in a loop with different values all return the same result?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/executionmodel.html#naming-and-binding
    title: The Python Language Reference, Python 3.15 documentation
    locator: 4.2.1 Binding of names; 4.2.2 Resolution of names
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15 documentation
    locator: 8.7 Function definitions
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/functools.html#functools.partial
    title: functools, Higher-order functions and operations on callable objects, Python 3.15 documentation
    locator: functools.partial()
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/function-uses-loop-variable/
    title: function-uses-loop-variable (B023), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/warning/cell-var-from-loop.html
    title: cell-var-from-loop / W0640, Pylint documentation
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Closures
    title: Closures, MDN Web Docs
    locator: Creating closures in loops, A common mistake
    accessed: 2026-11-18
  - url: https://go.dev/doc/go1.22
    title: Go 1.22 Release Notes
    locator: Changes to the language
    accessed: 2026-11-18
---

## Synopsis

```python run id=all-same
squares = []
for x in range(5):
    squares.append(lambda: x ** 2)

print([f() for f in squares])

squares = []
for x in range(5):
    squares.append(lambda n=x: n ** 2)

print([f() for f in squares])
```

```output for=all-same
[16, 16, 16, 16, 16]
[0, 1, 4, 9, 16]
```

The first loop makes 5 functions that all read the same `x`. The second loop gives each function its own `n`.

## Semantics

**r1.** A function body reads a free variable when the function runs. The FAQ explains the loop case: "x is not local to the lambdas, but is defined in the outer scope, and it is accessed when the lambda is called — not when it is defined."[^1] At the end of the loop, `x` is 4, so each call returns 16.[^1] Example: [`all-same`](#all-same).

**r2.** The name, not the value, is shared. "If a variable is used in a code block but not defined there, it is a free variable."[^2] All the functions made in one scope share that scope variable, so a later change to it shows in all of them. Example: [`change-after`](#change-after).

**r3.** The trap is not only for `lambda`: "Note that this behaviour is not peculiar to lambdas, but applies to regular functions too."[^1] A `def` in a loop and a lambda in a comprehension act the same way. Example: [`comprehension`](#comprehension).

**r4.** Fix 1, a default argument: "save the values in variables local to the lambdas, so that they don’t rely on the value of the global x".[^1] A default is evaluated when the `def` or `lambda` runs, once for each loop pass.[^3] The FAQ says that `n=x` "creates a new variable n local to the lambda and computed when the lambda is defined".[^1] The cost is a visible parameter that a caller can override.

**r5.** Fix 2, `functools.partial`. It returns "a new partial object which when called will behave like func called with the positional arguments args and keyword arguments keywords."[^4] The arguments are stored when `partial` is called. Example: [`partial-fix`](#partial-fix).

**r6.** Fix 3, a factory function. A call of an outer function makes a new scope, so each closure gets its own variable. Example: [`factory`](#factory).

**r7.** Ruff rule B023 "Checks for function definitions that use a loop variable."[^5] Its reason: "The loop variable is not bound in the function definition, so it will always have the value it had in the last iteration when the function is called."[^5] Pylint reports the case as W0640 `cell-var-from-loop`: "A variable used in a closure is defined in a loop. This will result in all closures using the same value for the closed-over variable."[^6]

## Examples

```python run id=change-after
squares = []
for x in range(3):
    squares.append(lambda: x ** 2)

x = 8
print(squares[0](), squares[2]())
```

```output for=change-after
64 64
```

The functions read `x` at call time, so they see the new value 8 (rule r2).

```python run id=partial-fix
from functools import partial


def add(x, i):
    return x + i


adders = [partial(add, i=i) for i in range(3)]
print([adder(1) for adder in adders])
print(adders[2].keywords)
```

```output for=partial-fix
[1, 2, 3]
{'i': 2}
```

Each partial object stores its own value of `i` (rule r5).

```python run id=factory
def make_multiplier(factor):
    def multiply(x):
        return x * factor
    return multiply


times = [make_multiplier(k) for k in (2, 3, 10)]
print([f(5) for f in times])
```

```output for=factory
[10, 15, 50]
```

Each call of `make_multiplier` has its own `factor`, so the closures do not share a variable (rule r6).

## Versions

The behavior is the same in all Python 3 versions. A comprehension has its own scope, but all the functions made by one comprehension share that scope, so the trap is the same as in a loop.

## Pitfalls

```python wrong id=comprehension
handlers = {name: (lambda: f"clicked {name}") for name in ["ok", "cancel"]}
print(handlers["ok"](), handlers["cancel"]())
```

```output for=comprehension
clicked cancel clicked cancel
```

The comprehension made two functions that read the same `name`. After the comprehension, `name` is `cancel` (rule r3). Write `lambda name=name: f"clicked {name}"`.

```python wrong id=callbacks
import asyncio


async def main():
    results = []
    tasks = []
    for job in ["a", "b", "c"]:
        async def run():
            await asyncio.sleep(0)
            results.append(job)
        tasks.append(asyncio.create_task(run()))
    await asyncio.gather(*tasks)
    print(results)


asyncio.run(main())
```

```output for=callbacks
['c', 'c', 'c']
```

The tasks run after the loop ends, so each one reads the last `job`. Code that runs later, such as a callback, a task or a thread, shows this trap most often. Pass the value as an argument: `async def run(job=job)`, or better, `async def run(job)` with `run(job)`.

## Comparison

JavaScript had the same problem with `var`. MDN says that the problem was common "Prior to the introduction of the let keyword", and that with `const` "every closure binds the block-scoped variable".[^7] Go changed its loops: "In Go 1.22, each iteration of the loop creates new variables, to avoid accidental sharing bugs."[^8] Python has no such form, because a `for` loop does not make a new scope. In Python, use one of the fixes in rules r4 to r6. For closures in general, see [[python/closures-and-nonlocal]].
