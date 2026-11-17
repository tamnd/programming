---
tag: SKMYQT
kind: feature
owner: python
slug: unboundlocalerror
name: UnboundLocalError
aliases: [UnboundLocalError, cannot access local variable, local variable referenced before assignment, assignment makes a name local]
area: languages/python/pitfalls
level: core
summary: >-
  An assignment anywhere in a function makes the name local to the whole function. A read before the assignment then raises UnboundLocalError. Use global or nonlocal, or pass the value in.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names", locator: "4.2.2 Resolution of names"}
proposals: []
see_also: [python/naming-and-binding, python/name-resolution, python/global-statement, python/nonlocal-statement, python/closures-and-nonlocal, python/augmented-assignment, python/the-exception-hierarchy]
sources:
  - url: https://docs.python.org/3.15/reference/executionmodel.html#naming-and-binding
    title: The Python Language Reference, Python 3.15 documentation
    locator: 4.2.1 Binding of names; 4.2.2 Resolution of names
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/exceptions.html#UnboundLocalError
    title: Built-in Exceptions, Python 3.15 documentation
    locator: UnboundLocalError
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/faq/programming.html#why-am-i-getting-an-unboundlocalerror-when-the-variable-has-a-value
    title: Programming FAQ, Python 3.15 documentation
    locator: Why am I getting an UnboundLocalError when the variable has a value?; What are the rules for local and global variables in Python?
    accessed: 2026-11-18
---

## Synopsis

```python fails id=read-then-assign expect=exception:UnboundLocalError
x = 10


def foo():
    print(x)
    x += 1


foo()
```

The function has an assignment to `x`, so `x` is local in all of `foo`, also on the line before the assignment.

## Semantics

**r1.** "If a name is bound in a block, it is a local variable of that block, unless declared as nonlocal or global."[^1] An assignment, an augmented assignment, a `for` target, an `import`, a `def`, a `with ... as` and an `except ... as` all bind a name.

**r2.** The compiler decides which names are local before the function runs. "The local variables of a code block can be determined by scanning the entire text of the block for name binding operations."[^1] So "all uses of the name within the block are treated as references to the current block. This can lead to errors when a name is used within a block before it is bound."[^1]

**r3.** The error: "If the current scope is a function scope, and the name refers to a local variable that has not yet been bound to a value at the point where the name is used, an UnboundLocalError exception is raised. UnboundLocalError is a subclass of NameError."[^1] The exception is "Raised when a reference is made to a local variable in a function or method, but no value has been bound to that variable."[^2]

**r4.** The FAQ explains the surprise: "when you make an assignment to a variable in a scope, that variable becomes local to that scope and shadows any similarly named variable in the outer scope."[^3] A function that only reads `x` works, and the same function with one added assignment fails. Example: [`read-then-assign`](#read-then-assign).

**r5.** Fix for a module variable: declare it `global`. The FAQ says that "This explicit declaration is required in order to remind you that (unlike the superficially analogous situation with class and instance variables) you are actually modifying the value of the variable in the outer scope".[^3] Example: [`global-and-nonlocal`](#global-and-nonlocal).

**r6.** Fix for a variable of an enclosing function: "You can do a similar thing in a nested scope using the nonlocal keyword".[^3] Example: [`global-and-nonlocal`](#global-and-nonlocal).

**r7.** Often the better fix is no shared variable: pass the value in as an argument and return the new value. The FAQ gives the reason for the rule: "requiring global for assigned variables provides a bar against unintended side-effects."[^3]

## Examples

```python run id=global-and-nonlocal
x = 10


def bump():
    global x
    print(x)
    x += 1


bump()
print(x)


def outer():
    count = 0

    def inner():
        nonlocal count
        count += 1

    inner()
    inner()
    return count


print(outer())
```

```output for=global-and-nonlocal
10
11
2
```

`global x` makes `x` in `bump` the module variable (rule r5). `nonlocal count` makes `count` in `inner` the variable of `outer` (rule r6).

```python run id=pass-it-in
def bump(x):
    return x + 1


x = 10
x = bump(x)
print(x)
```

```output for=pass-it-in
11
```

The function has no outer state to change, so it needs no declaration (rule r7).

## Versions

The rule is the same in all Python 3 versions. In the current versions, the message of the error is "cannot access local variable 'x' where it is not associated with a value".[^3]

## Pitfalls

```python fails id=branch-only expect=exception:UnboundLocalError
def grade(score):
    if score >= 50:
        result = "pass"
    return result


print(grade(80))
print(grade(20))
```

The first call binds `result`. The second call takes no branch that binds it, so the `return` reads a local name with no value. Give the name a value on all paths, for example with an `else` clause.

```python fails id=del-then-read expect=exception:UnboundLocalError
def cleanup():
    data = [1, 2, 3]
    del data
    return len(data)


cleanup()
```

`del` unbinds a local name. After it, a read gives the same error as a read before the first assignment (rule r3).

```python fails id=late-assignment expect=exception:UnboundLocalError
total = 5


def show():
    values = [total + i for i in range(3)]
    total = sum(values)
    return values, total


print(show())
```

Without its second line, `show` only reads `total`, and it works. The added line `total = sum(values)` makes `total` local in all of `show`, so the read in the comprehension, on the line before, fails (rule r2). Use a new name such as `grand_total`.

## Comparison

In languages with declarations, a name is local because of a declaration, and the compiler can report a read before the declaration. Python has no declarations. As the reference says, "Python lacks declarations and allows name binding operations to occur anywhere within a code block."[^1] So the error comes at run time. For the full rules, see [[python/naming-and-binding]] and [[python/name-resolution]].
