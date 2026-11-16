```python run id=unpack-targets
pairs = [("a", 1), ("b", 2)]
for key, value in pairs:
    print(key, value)

(a, b), c = (1, 2), 3
print(a, b, c)

first, *_, last = range(10)
print(first, last)

try:
    x, y = [1, 2, 3]
except ValueError as e:
    print(type(e).__name__)
```

```output for=unpack-targets
a 1
b 2
1 2 3
0 9
ValueError
```

- Verdict: native. Any iterable can be unpacked into a list of targets, in an assignment, a `for` loop or a `with` statement. Targets can nest, and one starred target takes the other items as a list. When the counts do not match, Python raises `ValueError`.
