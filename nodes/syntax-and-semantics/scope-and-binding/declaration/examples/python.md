```python run id=global-nonlocal-and-annotations
count = 0


def bump():
    global count  # a declaration: count in this function is the module name
    count += 1


def counter():
    n = 0

    def step():
        nonlocal n  # a declaration: n is the name of the enclosing function
        n += 1
        return n

    return step


bump()
bump()
print("count =", count)
s = counter()
s()
print("step() =", s())

total: int  # an annotation alone declares a type but binds no value
print("total is bound:", "total" in globals())

if count > 1:
    def mode():
        return "many"
else:
    def mode():
        return "few"
print("mode() =", mode())
```

```output for=global-nonlocal-and-annotations
count = 2
step() = 2
total is bound: False
mode() = many
```

- Verdict: native. Python has no declaration of a variable, and an assignment makes the name. `global` and `nonlocal` are declarations that tell the compiler where a name is bound. An annotation without a value declares a type but binds nothing, so `total` is not in `globals()`. `def` runs as a statement, so a condition can choose which function to define.
