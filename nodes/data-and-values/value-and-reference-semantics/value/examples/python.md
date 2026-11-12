```python run id=identity-type-and-value
# Every object has an identity, a type and a value.
n = 1000
print(type(n).__name__, n, id(n) == id(n))

# The value of a list can change; its identity and type stay the same.
xs = [1, 2]
before = id(xs)
xs.append(3)
print(xs, id(xs) == before)

# An int value cannot change: += makes a new object and rebinds the name.
k = 10
first = id(k)
k += 1
print(k, id(k) == first)

# == compares values, and is compares identities.
a = [1, 2, 3]
b = list(a)
print(a == b, a is b)

# A tuple is immutable, but the value of a list inside it can still change.
t = (1, [2])
t[1].append(3)
print(t)

# None, True and functions are values that a variable can hold.
values = [None, True, len, 2.5, "s", b"b"]
print([type(v).__name__ for v in values])
```

```output for=identity-type-and-value
int 1000 True
[1, 2, 3] True
11 False
True False
(1, [2, 3])
['NoneType', 'bool', 'builtin_function_or_method', 'float', 'str', 'bytes']
```

- Verdict: native. Every value is an object with an identity, a type and a value. `append` changes the value of a list and keeps its identity, but `+=` on an `int` makes a new object and rebinds the name. `==` compares values and `is` compares identities. A tuple is immutable, but the list inside it can still change. `None`, `True` and functions are values like any other.
