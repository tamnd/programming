```python run id=counter
def make_counter():
    count = 0

    def increment():
        nonlocal count
        count += 1
        return count

    return increment


first = make_counter()
second = make_counter()
print(first(), first(), first())
print(second())
print(first.__closure__[0].cell_contents)
```

```output for=counter
1 2 3
1
3
```

- Verdict: native. A nested `def` or a `lambda` is a closure over the variables of the enclosing functions.
- A closure can read an outer variable with no declaration. To assign to it, the inner function needs `nonlocal` (or `global` for a module variable).
- The captured variable lives in a cell object. `__closure__` gives the cells, and `cell_contents` gives the value.
- A `lambda` can hold only one expression, so state that changes usually goes in a nested `def`.
