```python run id=unbound-local
count = 10


def bump():
    try:
        count += 1
    except UnboundLocalError as e:
        print(type(e).__name__)


def bump_global():
    global count
    count += 1


bump()
bump_global()
print(count)
```

```output for=unbound-local
UnboundLocalError
11
```

- Verdict: native. Python checks each read at run time. `count += 1` binds `count`, so `count` is a local name in all of `bump`, and the read of the local name before it has a value raises `UnboundLocalError`, even though a global `count` exists. `global count` makes the name refer to the global variable.
