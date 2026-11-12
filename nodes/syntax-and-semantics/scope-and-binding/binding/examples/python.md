```python run id=global-nonlocal-and-del
count = 0


def bad():
    # An assignment anywhere in a function makes the name local in the whole function.
    try:
        print(count)
    except UnboundLocalError as e:
        print(type(e).__name__)
    count = 1


def good():
    global count  # bind the module-level name instead
    count += 1


def counter():
    n = 0

    def inc():
        nonlocal n  # rebind the name of the enclosing function
        n += 1
        return n

    return inc


bad()
good()
inc = counter()
inc()
print(count, inc())

# import, def, class, for and with all bind names, as assignment does.
import math as m
for i in range(2):
    pass
print(m.__name__, i)

# del removes the binding, not the object.
x = y = [1]
del x
print("x" in globals(), y)
```

```output for=global-nonlocal-and-del
UnboundLocalError
1 2
math 1
False [1]
```

- Verdict: native. An assignment anywhere in a function makes the name local in the whole function, so the read before it raises `UnboundLocalError`. `global` binds a name of the module, and `nonlocal` binds a name of the enclosing function. `import`, `for` and the other binding statements bind names as assignment does. `del` removes the binding, and the object stays alive while another name refers to it.
