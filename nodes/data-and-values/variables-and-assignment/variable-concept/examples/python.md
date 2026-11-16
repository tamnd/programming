```python run id=names
a = [1, 2]
b = a
b.append(3)
print(a, b, a is b)
b = [9]
print(a, b, a is b)
n = 1
n = "one"
print(n, type(n).__name__)
```

```output for=names
[1, 2, 3] [1, 2, 3] True
[1, 2, 3] [9] False
one str
```

- Verdict: native. A Python variable is a name that refers to an object, and `=` binds the name. `b = a` makes `b` refer to the same list, so `b.append(3)` changes the list that `a` shows too. `b = [9]` binds `b` to a new list, and `a` does not change. A name can refer to an object of any type.
