```python run id=statements
a = b = 0
a, b = 1, 2
a, b = b, a
print(a, b)
xs = ys = []
xs.append(1)
print(ys)
words = ["tea", "milk", "coffee"]
if (n := len(words)) > 2:
    print(n)
```

```output for=statements
2 1
[1]
3
```

- Verdict: native. `=` is a statement, and one statement can have many targets, as in `a = b = 0`, or a tuple of targets, as in `a, b = b, a`. All targets of `xs = ys = []` get the same list. `:=` is an assignment expression, with a value, so it can sit in the condition of an `if`.
