```python run id=return-value-python
def absolute(n):
    if n < 0:
        return -n
    return n


print(absolute(-7))
print(absolute(5) + 1)
```

```output for=return-value-python
7
6
```

- Verdict: native. `return` ends the function and gives the value. A function with no `return` gives back `None`.
