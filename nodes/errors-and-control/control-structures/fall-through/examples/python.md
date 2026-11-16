```python run id=fall-through-python
def rights(level):
    out = []
    match level:
        case 3:
            out.append("delete")
        case 2:
            out.append("write")
        case 1:
            out.append("read")
        case _:
            out.append("none")
    return out


def rights_with_ifs(level):
    out = []
    if level >= 3:
        out.append("delete")
    if level >= 2:
        out.append("write")
    if level >= 1:
        out.append("read")
    return out or ["none"]


print(rights(3), rights_with_ifs(3))
print(rights(0), rights_with_ifs(0))
```

```output for=fall-through-python
['delete'] ['delete', 'write', 'read']
['none'] ['none']
```

- Verdict: absent. A `match` statement runs only the first case that matches, so there is no fall-through and no `break`. To run the code of several cases, use separate `if` statements.
