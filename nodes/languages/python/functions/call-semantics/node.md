---
tag: 1DBET2
kind: feature
owner: python
slug: call-semantics
name: Call semantics and argument binding
aliases: [how a call binds arguments, argument slots, call by object reference, call by sharing, pass by assignment, is Python pass by reference, missing required positional argument]
area: languages/python/functions
level: advanced
summary: >-
  A call evaluates its arguments, fills a slot for each parameter from the arguments and defaults, and runs the body. Each argument is an object reference, so the body can change a mutable argument.
grammar: "call: primary '(' [argument_list [','] | comprehension] ')' (Language Reference, 6.3.3)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#calls", locator: "6.3.3. Calls"}
see_also: [python/defining-functions, python/keyword-arguments, python/default-argument-values, python/unpacking-argument-lists, python/callable-objects, python/evaluation-order-in-python]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#calls
    title: The Python Language Reference, Python 3.15
    locator: 6.3.3. Calls
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#how-do-i-write-a-function-with-output-parameters-call-by-reference
    title: Programming FAQ, Python 3.15
    locator: How do I write a function with output parameters (call by reference)?
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#defining-functions
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.8. Defining Functions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Calls
    title: The Go Programming Language Specification
    locator: Calls
    accessed: 2026-11-16
---

## Synopsis

```python run id=slots
def f(a, b, c=3, *rest, d, **extra):
    print(a, b, c, rest, d, extra)

f(1, 2, d=4)
f(1, 2, 5, 6, 7, d=4, e=8)
f(b=2, a=1, d=4)
```

```output for=slots
1 2 3 () 4 {}
1 2 5 (6, 7) 4 {'e': 8}
1 2 3 () 4 {}
```

Python puts each argument in the slot of one parameter. A default fills an empty slot. Extra arguments go to `*rest` and `**extra`.

## Semantics

**r1.** "All argument expressions are evaluated before the call is attempted."[^1] Example: [`evaluated-first`](#evaluated-first).

**r2.** "First, a list of unfilled slots is created for the formal parameters. If there are N positional arguments, they are placed in the first N slots. Next, for each keyword argument, the identifier is used to determine the corresponding slot".[^1] "If the slot is already filled, a TypeError exception is raised."[^1]

**r3.** "When all arguments have been processed, the slots that are still unfilled are filled with the corresponding default value from the function definition."[^1] "If there are any unfilled slots for which no default value is specified, a TypeError exception is raised."[^1] Example: [`missing`](#missing).

**r4.** "If there are more positional arguments than there are formal parameter slots, a TypeError exception is raised, unless a formal parameter using the syntax *identifier is present".[^1] "If any keyword argument does not correspond to a formal parameter name, a TypeError exception is raised, unless a formal parameter using the syntax **identifier is present".[^1] Example: [`too-many`](#too-many).

**r5.** "A call always returns some value, possibly None, unless it raises an exception."[^1]

**r6.** Arguments "are passed using call by value (where the value is always an object reference, not the value of the object)."[^3] The FAQ says: "Remember that arguments are passed by assignment in Python. Since assignment just creates references to objects, there’s no alias between an argument name in the caller and callee, and consequently no call-by-reference."[^2] Examples: [`mutate`](#mutate), [`rebind`](#rebind).

## Examples

```python run id=evaluated-first
def trace(label, value):
    print("evaluate", label)
    return value

def add(x, y):
    print("body runs")
    return x + y

print(add(trace("x", 1), trace("y", 2)))
```

```output for=evaluated-first
evaluate x
evaluate y
body runs
3
```

Both arguments are evaluated before the body runs (rule r1).

```python run id=mutate
def add_item(items):
    items.append("new")

data = ["old"]
add_item(data)
print(data)
```

```output for=mutate
['old', 'new']
```

`items` and `data` refer to the same list, so a change to the list is seen by the caller (rule r6).

```python run id=output-tuple
def divide(a, b):
    return a // b, a % b

q, r = divide(17, 5)
print(q, r)
```

```output for=output-tuple
3 2
```

To give several results back, return a tuple. The FAQ lists this way first: "By returning a tuple of the results".[^2]

## Pitfalls

An assignment to a parameter does not change the variable of the caller:

```python wrong id=rebind
def reset(count):
    count = 0

n = 5
reset(n)
print(n)
```

```output for=rebind
5
```

The assignment binds the local name `count` to a new object. `n` still refers to 5 (rule r6).

A required parameter with no argument is a TypeError:

```python fails id=missing expect=exception:TypeError
def f(a, b):
    pass

f(1)
```

Too many positional arguments are a TypeError when there is no `*` parameter:

```python fails id=too-many expect=exception:TypeError
def f(a, b):
    pass

f(1, 2, 3)
```

## Comparison

Go also evaluates the arguments first: "In a function call, the function value and arguments are evaluated in the usual order. After they are evaluated, new storage is allocated for the function's variables, which includes its parameters and results."[^4] Then "the arguments of the call are passed to the function, which means that they are assigned to their corresponding function parameters".[^4] Python also binds each parameter by assignment, and the parameter refers to the object of the argument. Python does not copy the object (rule r6).
