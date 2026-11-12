```python run id=who-calls
lines = ["alpha", "beta", "gamma"]

# Your code is in control: it runs the loop.
for line in lines:
    print(f"plain: {len(line)}")


# Inverted: the framework runs the loop and calls your handler.
def each_line(source, handler):
    for line in source:
        handler(line)


each_line(lines, lambda line: print(f"inverted: {len(line)}"))
```

```output
plain: 5
plain: 4
plain: 5
inverted: 5
inverted: 4
inverted: 5
```
