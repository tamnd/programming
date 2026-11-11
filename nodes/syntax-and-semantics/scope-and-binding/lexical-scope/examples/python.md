```python run id=scopes
level = "module"


def outer():
    level = "outer"

    def inner():
        return level

    return inner()


def reads_module():
    return level


def caller():
    level = "caller"
    return reads_module()


print(outer())
print(caller())

for i in range(3):
    pass
print("i after the loop:", i)

squares = [n * n for n in range(3)]
print("n defined after the comprehension:", "n" in globals())


def count():
    total = 0

    def add(k):
        nonlocal total
        total += k

    for k in range(4):
        add(k)
    return total


print("count():", count())
```

```output for=scopes
outer
module
i after the loop: 2
n defined after the comprehension: False
count(): 6
```

- Verdict: native. A name resolves in the nearest enclosing function scope, then in the module, then in the builtins. `caller` does not change what `reads_module` sees, because the lookup follows the text, not the calls.
- Python has no block scope. A `for` loop or an `if` does not make a scope, so the loop variable `i` is still defined after the loop. A comprehension has its own scope, so `n` is not.
- An assignment anywhere in a function makes the name local to the whole function. `global` and `nonlocal` declare that an assignment goes to the module or to the enclosing function.

```python fails expect=exception:UnboundLocalError id=assign-makes-local
requests = 0


def handle():
    requests += 1
    return requests


print(handle())
```

`requests += 1` assigns to `requests`, so `requests` is a local variable in the whole body of `handle`. The read on the same line finds the local variable before it has a value. Add `global requests` in the function, or better, keep the counter in an object that the function receives.
